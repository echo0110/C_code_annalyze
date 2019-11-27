/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dcpm_main.c,v 1.4.30.6 2010/11/24 03:32:34 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file provides implementation for D-CPM function
 *                     pointers table initialization and main entry point
 *                     function.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_dcpm_main.c,v $
 * Revision 1.4.30.6  2010/11/24 03:32:34  gur22059
 * Removal of conditional checks w.r.t profile1
 *
 * Revision 1.4.30.5  2010/11/09 05:02:18  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.4.30.4  2010/10/13 03:35:32  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.4.30.3  2010/10/04 05:45:10  gur22059
 * *** empty log message ***
 *
 * Revision 1.4.30.2  2010/10/01 14:11:40  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.4.30.1  2010/07/07 06:38:00  gur21145
 * SPR_FIX547
 *
 * Revision 1.4  2009/05/27 11:17:31  gur19836
 * File Header and function headers updated
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_dcpm_proto.h"
#include "rohc_pbm_ext_if.h"
#include "rohc_stats_def.h"
#include "rohc_opt_proto.h"
#include "rohc_gcom_mcr.h"

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
    /* +- SPR 17777 */
extern tickType_t pdcpCurrentTime_g;
    /* +- SPR 17777 */
     /* + SPR 17439 */
rohc_dcpm_fprtable_rtp rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U + 1]
        [ROHC_D_TST_PENDING - ROHC_D_TST_DONE + 1 ]
        [ROHC_D_R_MAX_ET - ROHC_D_U_START + 1]
        [ROHC_D_ST_FC - ROHC_D_ST_NC+ 1]
        [ROHC_ALL_PROFILE - ROHC_UNCOMP];
rohc_dcpm_fprtable_udp rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U + 1]
        [ROHC_D_TST_PENDING - ROHC_D_TST_DONE + 1 ]
        [ROHC_D_R_MAX_ET - ROHC_D_U_START + 1]
        [ROHC_D_ST_FC - ROHC_D_ST_NC+ 1]
        [ROHC_ALL_PROFILE - ROHC_UNCOMP];
rohc_dcpm_fprtable_tcp rohc_dcpm_fpt_tcp[ROHC_MODE_R - ROHC_MODE_U + 1]
     /* - SPR 17439 */
        [ROHC_D_TST_PENDING - ROHC_D_TST_DONE + 1 ]
        [ROHC_D_R_MAX_ET - ROHC_D_U_START + 1]
        [ROHC_D_ST_FC - ROHC_D_ST_NC+ 1]
        [ROHC_ALL_PROFILE - ROHC_UNCOMP];

rohc_dcpm_fprtable rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U + 1]
        [ROHC_D_TST_PENDING - ROHC_D_TST_DONE + 1 ]
        [ROHC_D_R_MAX_ET - ROHC_D_U_START + 1]
        [ROHC_D_ST_FC - ROHC_D_ST_NC+ 1];
    
rohc_U8bit_t rohc_dcpm_map_tbl[ROHC_MAX_PKT_TYPES][3];  

rohc_U32bit_t  rohc_pkt_val_str[ROHC_IP_ESP - ROHC_IP_UDP_RTP+1]
             [ROHC_D_ST_FC - ROHC_D_ST_NC + 1]
             [2]=
{
    {{0x0,0x600000},        /*for p1*//*NC*/
     {0xfe000000,0xe000ff},           /*SC*/
     {0xffffffff,0xe000ff}},          /*FC*/
    {{0x0,0x600000},        /* for p2*//*NC*/
     {0x0,0xe7c000},                   /*SC*/
     {0x7,0xe7ff00}},                  /*FC*/
    {{0x0,0x600000},  /* for p3*/  /*NC*/     
     {0x0,0xe7c000},               /*SC*/
     {0x7,0xe7ff00}},              /*FC*/
};           

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/




/*****************************************************************************
 * Function Name  : rohc_dcpm_init
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function initializes the DCPM function pointer table.
 *                  The table is of 4-dimension where
 *                  - 1st-dim specifies Modes 
 *                  - 2st-dim specifies compressor's transition states
 *                  - 3st-dim specifies compressor's  states
 *                  - 4st-dim specifies the events.
 *****************************************************************************/
rohc_void_t rohc_dcpm_init ROHC_NO_PARAMS()
{

    /*Funtion pointer table initialization for U-mode */
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_u_ir_nc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_ir_irdyn_uor2_sc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_all_fc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = ROHC_NULL;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_ir_irdyn_uor2_sc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_all_fc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = ROHC_NULL;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = ROHC_NULL;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_all_fc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = ROHC_NULL;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = ROHC_NULL;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_all_fc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = ROHC_NULL;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_ir_irdyn_uor2_sc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_all_fc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_u_ack_u_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_ack_u_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_ack_u_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_u_mt_u_to_o_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_mt_u_to_o_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_mt_u_to_o_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_u_mt_u_to_r_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_mt_u_to_r_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_mt_u_to_r_dn;
                
                
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_u_ir_nc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_ir_irdyn_uor2_sc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_ir_irdyn_uor2_fc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = ROHC_NULL;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_ir_irdyn_uor2_sc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_ir_irdyn_uor2_fc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = ROHC_NULL;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = ROHC_NULL;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_all_fc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = ROHC_NULL;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = ROHC_NULL;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_all_fc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = ROHC_NULL;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_ir_irdyn_uor2_sc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_ir_irdyn_uor2_fc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_u_ack_u_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_ack_u_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_ack_u_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_u_mt_u_to_o_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_mt_u_to_o_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_mt_u_to_o_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_u_mt_u_to_r_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_mt_u_to_r_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_mt_u_to_r_in;
    
    /* START_SPR_547_FIX */
    
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_u_ir_nc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_ir_sc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_ir_fc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_u_irdyn_nc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_irdyn_sc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_irdyn_fc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_u_uo_0_nc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_uo_0_sc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_uo_0_fc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_u_uo_1_nc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_uo_1_sc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_uo_1_fc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_u_uo_2_nc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_uo_2_sc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_uo_2_fc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_u_ack_u_nc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_ack_u_sc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_ack_u_fc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_u_mt_u_to_o_nc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_mt_u_to_o_sc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_mt_u_to_o_fc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_u_mt_u_to_r_nc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_u_mt_u_to_r_sc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_u_mt_u_to_r_fc_pnd;
    /* END_SPR_FIX_547 */

#ifdef ROHC_MODE_TRIGGER 
    /*Funtion pointer table initialization for O-mode */
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_ir_nc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_ir_sc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_ir_fc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_irdyn_nc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_irdyn_sc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_irdyn_fc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_uo_0_nc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_uo_0_sc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_uo_0_fc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_uo_1_nc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_uo_1_sc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_uo_1_fc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_uo_2_nc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_uo_2_sc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_uo_2_fc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_ack_o_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_ack_o_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_ack_o_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_static_nack_o_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_static_nack_o_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_static_nack_o_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_nack_o_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_nack_o_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_nack_o_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_mt_o_to_u_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_mt_o_to_u_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_mt_o_to_u_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_mt_o_to_r_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_mt_o_to_r_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_mt_o_to_r_dn;
           /* O-initiated */     
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_ir_nc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_ir_sc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_ir_fc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_irdyn_nc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_irdyn_sc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_irdyn_fc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_uo_0_nc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_uo_0_sc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_uo_0_fc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_uo_1_nc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_uo_1_sc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_uo_1_fc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_uo_2_nc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_uo_2_sc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_uo_2_fc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_ack_o_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_ack_o_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_ack_o_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_static_nack_o_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_static_nack_o_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_static_nack_o_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_nack_o_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_nack_o_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_nack_o_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_mt_o_to_u_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_mt_o_to_u_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_mt_o_to_u_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_mt_o_to_r_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_mt_o_to_r_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_mt_o_to_r_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_ir_nc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_ir_sc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_ir_fc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_irdyn_nc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_irdyn_sc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_irdyn_fc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_uo_0_nc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_uo_0_sc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_uo_0_fc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_uo_1_nc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_uo_1_sc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_uo_1_fc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_uo_2_nc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_uo_2_sc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_uo_2_fc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_ack_o_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_ack_o_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_ack_o_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_static_nack_o_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_static_nack_o_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_static_nack_o_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_nack_o_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_nack_o_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_nack_o_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_mt_o_to_u_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_mt_o_to_u_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_mt_o_to_u_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_o_mt_o_to_r_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_o_mt_o_to_r_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_o_mt_o_to_r_pnd;
    

    /*Funtion pointer table initialization for R-mode */
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_ir_nc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_ir_sc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_ir_fc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_irdyn_nc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_irdyn_sc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_irdyn_fc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_r_0_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_r_0_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_r_0_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_r_0_crc_nc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_r_0_crc_sc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_r_0_crc_fc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_r_1_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_r_1_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_r_1_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_r_2_nc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_r_2_sc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_r_2_fc_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_ack_r_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_ack_r_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_ack_r_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_static_nack_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_static_nack_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_static_nack_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_nack_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_nack_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_nack_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_mt_r_to_u_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_mt_r_to_u_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_mt_r_to_u_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_mt_r_to_o_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_mt_r_to_o_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_mt_r_to_o_dn;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_ir_nc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_ir_sc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_ir_fc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_irdyn_nc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_irdyn_sc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_irdyn_fc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_r_0_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_r_0_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_r_0_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_r_0_crc_nc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_r_0_crc_sc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_r_0_crc_fc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_r_1_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_r_1_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_r_1_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_r_2_nc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_r_2_sc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_r_2_fc_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_ack_r_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_ack_r_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_ack_r_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_static_nack_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_static_nack_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_static_nack_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_nack_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_nack_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_nack_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_mt_r_to_u_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_mt_r_to_u_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_mt_r_to_u_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_mt_r_to_o_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_mt_r_to_o_in;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_mt_r_to_o_in;
                
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_ir_nc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_ir_sc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_ir_fc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_irdyn_nc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_irdyn_sc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_irdyn_fc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_r_0_nc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_r_0_sc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_r_0_fc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_r_0_crc_nc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_r_0_crc_sc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_r_0_crc_fc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_r_1_nc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_r_1_sc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_r_1_fc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_r_2_nc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_r_2_sc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_r_2_fc_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_ack_r_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_ack_r_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_ack_r_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_static_nack_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_static_nack_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_static_nack_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_nack_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_nack_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_nack_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_mt_r_to_u_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_mt_r_to_u_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_mt_r_to_u_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC] = rohc_d_r_mt_r_to_o_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] = rohc_d_r_mt_r_to_o_pnd;
    rohc_dcpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] = rohc_d_r_mt_r_to_o_pnd;
#endif

     /* + SPR 17439 */
    ROHC_MEMSET(rohc_dcpm_fpt_rtp, 0,
                 sizeof(rohc_dcpm_fprtable_rtp)*
                 (ROHC_MODE_R - ROHC_MODE_U + 1)*
                 (ROHC_D_TST_PENDING - ROHC_D_TST_DONE + 1 )*
                 (ROHC_D_R_MAX_ET - ROHC_D_U_START + 1)*
                 (ROHC_D_ST_FC - ROHC_D_ST_NC+ 1)*
                 (ROHC_ALL_PROFILE - ROHC_UNCOMP));
    ROHC_MEMSET(rohc_dcpm_fpt_udp, 0,
                 sizeof(rohc_dcpm_fprtable_udp)*
                 (ROHC_MODE_R - ROHC_MODE_U + 1)*
                 (ROHC_D_TST_PENDING - ROHC_D_TST_DONE + 1 )*
                 (ROHC_D_R_MAX_ET - ROHC_D_U_START + 1)*
                 (ROHC_D_ST_FC - ROHC_D_ST_NC+ 1)*
                 (ROHC_ALL_PROFILE - ROHC_UNCOMP));
    ROHC_MEMSET(rohc_dcpm_fpt_tcp, 0,
                 sizeof(rohc_dcpm_fprtable_tcp)*
     /* - SPR 17439 */
                 (ROHC_MODE_R - ROHC_MODE_U + 1)*
                 (ROHC_D_TST_PENDING - ROHC_D_TST_DONE + 1 )*
                 (ROHC_D_R_MAX_ET - ROHC_D_U_START + 1)*
                 (ROHC_D_ST_FC - ROHC_D_ST_NC+ 1)*
                 (ROHC_ALL_PROFILE - ROHC_UNCOMP));

      
     /* + SPR 17439 */
    /*Funtion pointer table initialization for U-mode */
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ir_nc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ir_irdyn_uor2_sc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_all_fc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ir_irdyn_uor2_sc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_all_fc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_all_fc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_all_fc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ir_irdyn_uor2_sc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP]= rohc_d_p1_u_all_fc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ack_u_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ack_u_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ack_u_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_mt_u_to_o_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_mt_u_to_o_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_mt_u_to_o_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_mt_u_to_r_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_mt_u_to_r_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_mt_u_to_r_dn;
                
                
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ir_nc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ir_irdyn_uor2_sc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ir_irdyn_uor2_fc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ir_irdyn_uor2_sc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ir_irdyn_uor2_fc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_all_fc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_all_fc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ir_irdyn_uor2_sc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ir_irdyn_uor2_fc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ack_u_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ack_u_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ack_u_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_mt_u_to_o_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_mt_u_to_o_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_mt_u_to_o_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_mt_u_to_r_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_mt_u_to_r_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_mt_u_to_r_in;
    
    /* START_SPR_547_FIX */
    
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ir_nc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ir_sc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ir_fc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_irdyn_nc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_irdyn_sc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_irdyn_fc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_uo_0_nc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_uo_0_sc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_uo_0_fc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_uo_1_nc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_uo_1_sc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_uo_1_fc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_uo_2_nc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_uo_2_sc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_uo_2_fc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ack_u_nc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ack_u_sc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_ack_u_fc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_mt_u_to_o_nc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_mt_u_to_o_sc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_mt_u_to_o_fc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_mt_u_to_r_nc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_mt_u_to_r_sc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_u_mt_u_to_r_fc_pnd;
    /* END_SPR_FIX_547 */

#ifdef ROHC_MODE_TRIGGER 
    /*Funtion pointer table initialization for O-mode */
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_ir_nc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_ir_sc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_ir_fc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_irdyn_nc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_irdyn_sc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_irdyn_fc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_0_nc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_0_sc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_0_fc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_1_nc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_1_sc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_1_fc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_2_nc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_2_sc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_2_fc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_ack_o_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_ack_o_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_ack_o_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_static_nack_o_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_static_nack_o_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_static_nack_o_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_nack_o_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_nack_o_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_nack_o_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_mt_o_to_u_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_mt_o_to_u_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_mt_o_to_u_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_mt_o_to_r_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_mt_o_to_r_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_mt_o_to_r_dn;
           /* O-initiated */     
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_ir_nc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_ir_sc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_ir_fc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_irdyn_nc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_irdyn_sc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_irdyn_fc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_0_nc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_0_sc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_0_fc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_1_nc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_1_sc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_1_fc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_2_nc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_2_sc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_2_fc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_ack_o_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_ack_o_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_ack_o_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_static_nack_o_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_static_nack_o_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_static_nack_o_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_nack_o_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_nack_o_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_nack_o_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_mt_o_to_u_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_mt_o_to_u_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_mt_o_to_u_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_mt_o_to_r_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_mt_o_to_r_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_mt_o_to_r_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_ir_nc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_ir_sc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_ir_fc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_irdyn_nc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_irdyn_sc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_irdyn_fc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_0_nc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_0_sc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_0_fc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_1_nc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_1_sc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_1_fc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_2_nc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_2_sc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_uo_2_fc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_ack_o_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_ack_o_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_ack_o_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_static_nack_o_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_static_nack_o_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_static_nack_o_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_nack_o_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_nack_o_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_nack_o_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_mt_o_to_u_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_mt_o_to_u_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_mt_o_to_u_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_mt_o_to_r_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_mt_o_to_r_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_o_mt_o_to_r_pnd;
#endif
    

#ifdef ROHC_MODE_TRIGGER 
    /*Funtion pointer table initialization for R-mode */
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_ir_nc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_ir_sc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_ir_fc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_irdyn_nc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_irdyn_sc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_irdyn_fc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_0_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_0_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_0_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_0_crc_nc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_0_crc_sc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_0_crc_fc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_1_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_1_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_1_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_2_nc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP]= rohc_d_p1_r_r_2_sc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_2_fc_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_ack_r_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_ack_r_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_ack_r_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_static_nack_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_static_nack_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_static_nack_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_nack_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_nack_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_nack_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_mt_r_to_u_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_mt_r_to_u_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_mt_r_to_u_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_mt_r_to_o_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_mt_r_to_o_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_mt_r_to_o_dn;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_ir_nc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_ir_sc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_ir_fc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_irdyn_nc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_irdyn_sc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_irdyn_fc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_0_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_0_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_0_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_0_crc_nc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_0_crc_sc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_0_crc_fc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_1_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_1_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_1_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_2_nc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_2_sc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_2_fc_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_ack_r_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_ack_r_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_ack_r_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_static_nack_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_static_nack_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_static_nack_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_nack_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_nack_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_nack_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_mt_r_to_u_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_mt_r_to_u_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_mt_r_to_u_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_mt_r_to_o_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_mt_r_to_o_in;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_mt_r_to_o_in;
                
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_ir_nc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_ir_sc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_ir_fc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_irdyn_nc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_irdyn_sc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_irdyn_fc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_0_nc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_0_sc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_0_fc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_0_crc_nc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_0_crc_sc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_0_crc_fc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_1_nc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_1_sc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_1_fc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_2_nc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_2_sc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_r_2_fc_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_ack_r_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_ack_r_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_ack_r_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_static_nack_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_static_nack_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_static_nack_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_nack_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_nack_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_nack_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_mt_r_to_u_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_mt_r_to_u_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_mt_r_to_u_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_mt_r_to_o_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_mt_r_to_o_pnd;
    rohc_dcpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_d_p1_r_mt_r_to_o_pnd;
#endif

    /*Funtion pointer table initialization for U-mode */
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ir_nc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ir_irdyn_uor2_sc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_all_fc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ir_irdyn_uor2_sc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_all_fc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_all_fc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_all_fc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ir_irdyn_uor2_sc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC] 
                [ROHC_IP_UDP - ROHC_UNCOMP]= rohc_d_p2_u_all_fc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ack_u_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ack_u_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ack_u_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_mt_u_to_o_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_mt_u_to_o_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_mt_u_to_o_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_mt_u_to_r_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_mt_u_to_r_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_mt_u_to_r_dn;
                
                
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ir_nc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ir_irdyn_uor2_sc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ir_irdyn_uor2_fc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ir_irdyn_uor2_sc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ir_irdyn_uor2_fc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_all_fc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_all_fc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ir_irdyn_uor2_sc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ir_irdyn_uor2_fc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ack_u_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ack_u_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ack_u_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_mt_u_to_o_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_mt_u_to_o_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_mt_u_to_o_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_mt_u_to_r_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_mt_u_to_r_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_mt_u_to_r_in;
    
    /* START_SPR_547_FIX */
    
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ir_nc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ir_sc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ir_fc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_irdyn_nc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_irdyn_sc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_irdyn_fc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_uo_0_nc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_uo_0_sc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_uo_0_fc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_uo_1_nc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_uo_1_sc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_uo_1_fc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_uo_2_nc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_uo_2_sc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_uo_2_fc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ack_u_nc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ack_u_sc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_ACK_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_ack_u_fc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_mt_u_to_o_nc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_mt_u_to_o_sc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_mt_u_to_o_fc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_mt_u_to_r_nc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_mt_u_to_r_sc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_U_MT_U_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_u_mt_u_to_r_fc_pnd;
    /* END_SPR_FIX_547 */

#ifdef ROHC_MODE_TRIGGER 
    /*Funtion pointer table initialization for O-mode */
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_ir_nc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_ir_sc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_ir_fc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_irdyn_nc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_irdyn_sc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_irdyn_fc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_0_nc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_0_sc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_0_fc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_1_nc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_1_sc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_1_fc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_2_nc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_2_sc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_2_fc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_ack_o_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_ack_o_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_ack_o_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_static_nack_o_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_static_nack_o_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_static_nack_o_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_nack_o_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_nack_o_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_nack_o_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_mt_o_to_u_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_mt_o_to_u_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_mt_o_to_u_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_mt_o_to_r_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_mt_o_to_r_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_mt_o_to_r_dn;
           /* O-initiated */     
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_ir_nc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_ir_sc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_ir_fc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_irdyn_nc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_irdyn_sc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_irdyn_fc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_0_nc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_0_sc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_0_fc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_1_nc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_1_sc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_1_fc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_2_nc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_2_sc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_2_fc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_ack_o_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_ack_o_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_ack_o_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_static_nack_o_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_static_nack_o_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_static_nack_o_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_nack_o_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_nack_o_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_nack_o_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_mt_o_to_u_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_mt_o_to_u_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_mt_o_to_u_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_mt_o_to_r_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_mt_o_to_r_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_mt_o_to_r_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_ir_nc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_ir_sc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_ir_fc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_irdyn_nc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_irdyn_sc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_irdyn_fc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_0_nc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_0_sc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_0_fc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_1_nc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_1_sc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_1_fc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_2_nc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_2_sc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_UO_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_uo_2_fc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_ack_o_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_ack_o_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_ACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_ack_o_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_static_nack_o_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_static_nack_o_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_STATIC_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_static_nack_o_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_nack_o_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_nack_o_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_NACK_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_nack_o_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_mt_o_to_u_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_mt_o_to_u_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_mt_o_to_u_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_mt_o_to_r_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_mt_o_to_r_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_O_MT_O_TO_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_o_mt_o_to_r_pnd;
    

    /*Funtion pointer table initialization for R-mode */
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_ir_nc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_ir_sc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_ir_fc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_irdyn_nc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_irdyn_sc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_irdyn_fc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_0_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_0_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_0_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_0_crc_nc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_0_crc_sc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_0_crc_fc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_1_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_1_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_1_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_2_nc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC] 
                [ROHC_IP_UDP - ROHC_UNCOMP]= rohc_d_p2_r_r_2_sc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_2_fc_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_ack_r_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_ack_r_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_ack_r_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_static_nack_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_static_nack_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_static_nack_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_nack_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_nack_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_nack_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_mt_r_to_u_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_mt_r_to_u_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_mt_r_to_u_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_mt_r_to_o_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_mt_r_to_o_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_DONE - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_mt_r_to_o_dn;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_ir_nc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_ir_sc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_ir_fc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_irdyn_nc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_irdyn_sc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_irdyn_fc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_0_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_0_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_0_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_0_crc_nc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_0_crc_sc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_0_crc_fc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_1_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_1_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_1_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_2_nc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_2_sc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_2_fc_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_ack_r_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_ack_r_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_ack_r_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_static_nack_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_static_nack_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_static_nack_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_nack_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_nack_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_nack_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_mt_r_to_u_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_mt_r_to_u_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_mt_r_to_u_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_mt_r_to_o_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_mt_r_to_o_in;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_INITIATED  - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_mt_r_to_o_in;
                
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_ir_nc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_ir_sc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_IR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_ir_fc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_irdyn_nc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_irdyn_sc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_IRDYN_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_irdyn_fc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_0_nc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_0_sc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_0_fc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_0_crc_nc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_0_crc_sc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_0_CRC_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_0_crc_fc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_1_nc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_1_sc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_1_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_1_fc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_2_nc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_2_sc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_R_2_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_r_2_fc_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_ack_r_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_ack_r_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_ACK_R_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_ack_r_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_static_nack_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_static_nack_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_STATIC_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_static_nack_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_nack_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_nack_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_NACK_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_nack_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_mt_r_to_u_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_mt_r_to_u_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_U_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_mt_r_to_u_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_mt_r_to_o_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_mt_r_to_o_pnd;
    rohc_dcpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_D_TST_PENDING - ROHC_D_TST_DONE]
                [ROHC_D_R_MT_R_TO_O_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_d_p2_r_mt_r_to_o_pnd;
#endif
    /*Entry for P6*/
    /*Funtion pointer table initialization*/
    rohc_dcpm_fpt_tcp[ROHC_MODE_TCP - ROHC_MODE_TCP]
                [ROHC_D_TST_TCP - ROHC_D_TST_TCP]
                [ROHC_D_IR_IRCR_ET - ROHC_D_U_START]
                [ROHC_D_ST_NC - ROHC_D_ST_NC]
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohcDP6IrIrCRNoContext;
    rohc_dcpm_fpt_tcp[ROHC_MODE_TCP - ROHC_MODE_TCP]
                [ROHC_D_TST_TCP - ROHC_D_TST_DONE]
                [ROHC_D_IR_IRCR_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohcDP6IrIrCRStaticContext;
    rohc_dcpm_fpt_tcp[ROHC_MODE_TCP - ROHC_MODE_TCP]
                [ROHC_D_TST_TCP - ROHC_D_TST_TCP]
                [ROHC_D_IR_IRCR_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohcDP6AllFullContext;

    rohc_dcpm_fpt_tcp[ROHC_MODE_TCP - ROHC_MODE_TCP]
                [ROHC_D_TST_TCP - ROHC_D_TST_TCP]
                [ROHC_D_IR_DYN_CO_ET - ROHC_D_U_START]
                [ROHC_D_ST_SC - ROHC_D_ST_NC]
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohcDP6IrdynCOStaticContext;
    rohc_dcpm_fpt_tcp[ROHC_MODE_TCP - ROHC_MODE_TCP]
                [ROHC_D_TST_TCP - ROHC_D_TST_TCP]
                [ROHC_D_IR_DYN_CO_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohcDP6AllFullContext;
    rohc_dcpm_fpt_tcp[ROHC_MODE_TCP - ROHC_MODE_TCP]
                [ROHC_D_TST_TCP - ROHC_D_TST_TCP]
                [ROHC_D_OTHERS_ET - ROHC_D_U_START]
                [ROHC_D_ST_FC - ROHC_D_ST_NC]
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohcDP6AllFullContext;

     /* - SPR 17439 */
     rohc_dcpm_in_evt_tbl();
     ROHC_EXIT_FN("rohc_dcpm_init");
}

/*****************************************************************************
 * Function Name  : rohc_dcpm_exec
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 *                  pkt_type - Packet type
 *                  P-ecode - Pointer to error code
 * Outputs        : None
 * Returns        : None
 * Description    : This is the main entry point function for DCPM. This 
 *                  updates the state machine (state and transition state) and
 *                  initiated the received mode transition and ack/nack/static 
 *                  nack.
 *****************************************************************************/
rohc_return_t rohc_dcpm_exec 
            ROHC_PARAMS((p_entity,p_dsc, pkt_typ, p_ecode),rohc_entity_t *p_entity _AND_
            rohc_dsc_t *p_dsc _AND_ 
            register rohc_U8bit_t  pkt_typ _AND_
            rohc_error_code_t *p_ecode
        )
{
    rohc_U8bit_t event_type;

  rohc_U8bit_t d_mode = p_dsc->d_mode;

    ROHC_ENTER_FN("rohc_exec_dcpm");
    if(p_dsc->profile == ROHC_UNCOMP)
    {
            if( (p_dsc->d_state == ROHC_DUN_ST_NC) &&
                (pkt_typ == ROHC_IRP0_PKTYPE) && 
                (p_dsc->decom_res == ROHC_SUCCESS))
            {
                p_dsc->d_state = ROHC_DUN_ST_FC;
                ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                      "Handling for IRP0_PKTYPE packet, "
                                      "state is updated to ROHC_DUN_ST_FC\n",
                                      p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                      ROHC_GET_DSC_CID(p_dsc));
            }
            else if(pkt_typ == ROHC_NORMALP0_PKTYPE)
            {
                ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                      "Handling for IRP0_NORMAL_PKTYPE packet, "
                                      "Nothing needs to be doen\n",
                                      p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                      ROHC_GET_DSC_CID(p_dsc));
            }
    }
    else if((pkt_typ>= ROHC_R0P1_PKTTYPE && pkt_typ <ROHC_MAX_PKT_TYPES ) &&
        ( d_mode >= ROHC_MODE_U &&  d_mode <= ROHC_MODE_R) &&
        ((event_type=rohc_dcpm_map_tbl[pkt_typ][d_mode - ROHC_MODE_U])!=
        ROHC_NULL))
   {        
            if(!rohc_dcpm_exec_direct_evt(p_entity, p_dsc, event_type, p_ecode))
            {
                    ROHC_ENTER_FN("rohc_exec_dcpm");                
                    return ROHC_FAILURE;
            }
    }
    else
    {
            *p_ecode = ROHC_INVALID_DCPM_EVENT;
            ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                      "CCPM called in invalid scenario\n",
                                      p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                      ROHC_GET_DSC_CID(p_dsc));
            ROHC_EXIT_FN("rohc_exec_dcpm");
            return ROHC_FAILURE;                                      
        
    }
    ROHC_EXIT_FN("rohc_exec_dcpm");
    return ROHC_SUCCESS;           
}

/*****************************************************************************
 * Function Name  : rohc_dcpm_p0_exec
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_dsc - Pointer to rohc compressor stream context block
 *                  pkt_type - Packet type
 *                  P-ecode - Pointer to error code
 * Outputs        : None
 * Returns        : None
 * Description    : This is the main entry point function for DCPM. This 
 *                  updates the state machine (state and transition state) and
 *                  initiated the received mode transition and ack/nack/static 
 *                  nack.
 *****************************************************************************/
rohc_return_t rohc_dcpm_p0_exec 
            ROHC_PARAMS((p_entity,p_dsc, pkt_typ, p_ecode),rohc_entity_t *p_entity _AND_
            rohc_dsc_uncomp_t      *p_dsc  _AND_ 
            register rohc_U8bit_t  pkt_typ _AND_
            rohc_error_code_t      *p_ecode
        )
{
    ROHC_ENTER_FN("rohc_dcpm_p0_exec");
    if( (p_dsc->d_state == ROHC_DUN_ST_NC) &&
        (pkt_typ == ROHC_IRP0_PKTYPE) && 
        (p_dsc->decom_res == ROHC_SUCCESS))
    {
        p_dsc->d_state = ROHC_DUN_ST_FC;
    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                (ADDR)p_ecode,0,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
        ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]"
                          "Handling for IRP0_PKTYPE packet, "
                          "state is updated to ROHC_DUN_ST_FC\n",
                          p_entity->rohc_entity_id.ueIndex,
                           p_entity->rohc_entity_id.lcId);
    }
    else if(pkt_typ == ROHC_NORMALP0_PKTYPE)
    {
        ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]"
                          "Handling for IRP0_NORMAL_PKTYPE packet, "
                          "Nothing needs to be doen\n",
                           p_entity->rohc_entity_id.ueIndex,
                           p_entity->rohc_entity_id.lcId);
    }
    ROHC_EXIT_FN("rohc_dcpm_p0_exec");
    return ROHC_SUCCESS;           
}

/*****************************************************************************
 * Function Name  : rohc_dcpm_p1_exec
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 *                  pkt_type - Packet type
 *                  P-ecode - Pointer to error code
 * Outputs        : None
 * Returns        : None
 * Description    : This is the main entry point function for DCPM. This 
 *                  updates the state machine (state and transition state) and
 *                  initiated the received mode transition and ack/nack/static 
 *                  nack.
 *****************************************************************************/
rohc_return_t rohc_dcpm_p1_exec 
            ROHC_PARAMS((p_entity,p_dsc, pkt_typ, p_ecode),rohc_entity_t *p_entity _AND_
            rohc_dsc_rtp_t *p_dsc _AND_ 
            register rohc_U8bit_t  pkt_typ _AND_
            rohc_error_code_t *p_ecode
        )
{
    rohc_U8bit_t event_type;

  rohc_U8bit_t d_mode = p_dsc->d_mode;

    ROHC_ENTER_FN("rohc_dcpm_p1_exec");
    if(((rohc_dsc_common_t *)p_dsc)->profile == ROHC_UNCOMP)
    {
        if(pkt_typ == ROHC_IRP0_PKTYPE ||
            pkt_typ == ROHC_NORMALP0_PKTYPE)
        {
            return ROHC_SUCCESS;
        }
        else
        {  
            ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                      "CCPM called in invalid scenario\n",
                                      p_entity->rohc_entity_id.ueIndex,
                                      p_entity->rohc_entity_id.lcId,
                                      ROHC_GET_DSC_CID(p_dsc));
            return ROHC_FAILURE;
        }                    
    }
    else if((pkt_typ>= ROHC_R0P1_PKTTYPE && pkt_typ <ROHC_MAX_PKT_TYPES ) &&
        ( d_mode >= ROHC_MODE_U &&  d_mode <= ROHC_MODE_R) &&
        ((event_type=rohc_dcpm_map_tbl[pkt_typ][d_mode - ROHC_MODE_U])!=
        ROHC_NULL))
   {        
            if(!rohc_dcpm_p1_exec_direct_evt(p_entity, p_dsc, event_type, p_ecode))
            {
                    ROHC_ENTER_FN("rohc_dcpm_p1_exec");                
                    return ROHC_FAILURE;
            }
    }
    else
    {
            *p_ecode = ROHC_INVALID_DCPM_EVENT;
            ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                      "CCPM called in invalid scenario\n",
                                      p_entity->rohc_entity_id.ueIndex,
                                      p_entity->rohc_entity_id.lcId,
                                      ROHC_GET_DSC_CID(p_dsc));
            ROHC_EXIT_FN("rohc_dcpm_p1_exec");
            return ROHC_FAILURE;                                      
        
    }
    ROHC_EXIT_FN("rohc_dcpm_p1_exec");
    return ROHC_SUCCESS;           
}

/*****************************************************************************
 * Function Name  : rohc_dcpm_p2_exec
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 *                  pkt_type - Packet type
 *                  P-ecode - Pointer to error code
 * Outputs        : None
 * Returns        : None
 * Description    : This is the main entry point function for DCPM. This 
 *                  updates the state machine (state and transition state) and
 *                  initiated the received mode transition and ack/nack/static 
 *                  nack.
 *****************************************************************************/
rohc_return_t rohc_dcpm_p2_exec 
            ROHC_PARAMS((p_entity,p_dsc, pkt_typ, p_ecode),rohc_entity_t *p_entity _AND_
            rohc_dsc_udp_t         *p_dsc  _AND_ 
            register rohc_U8bit_t  pkt_typ _AND_
            rohc_error_code_t      *p_ecode
        )
{
    ROHC_ENTER_FN("rohc_dcpm_p2_exec");
    rohc_U8bit_t event_type;

  rohc_U8bit_t d_mode = p_dsc->d_mode;

    if(((rohc_dsc_common_t *)p_dsc)->profile == ROHC_UNCOMP)
    {
        if(pkt_typ == ROHC_IRP0_PKTYPE ||
            pkt_typ == ROHC_NORMALP0_PKTYPE)
        {
            return ROHC_SUCCESS;
        }
        else
        {    
            ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                      "CCPM called in invalid scenario\n",
                                      p_entity->rohc_entity_id.ueIndex,
                                      p_entity->rohc_entity_id.lcId,
                                      ROHC_GET_DSC_CID(p_dsc));
            return ROHC_FAILURE;
        }                   
    }
    else if((pkt_typ>= ROHC_R0P1_PKTTYPE && pkt_typ <ROHC_MAX_PKT_TYPES ) &&
        ( d_mode >= ROHC_MODE_U &&  d_mode <= ROHC_MODE_R) &&
        ((event_type=rohc_dcpm_map_tbl[pkt_typ][d_mode - ROHC_MODE_U])!=
        ROHC_NULL))
   {        
            if(!rohc_dcpm_p2_exec_direct_evt(p_entity, p_dsc, event_type, p_ecode))
            {
                    ROHC_ENTER_FN("rohc_dcpm_p2_exec");                
                    return ROHC_FAILURE;
            }
    }
    else
    {
            *p_ecode = ROHC_INVALID_DCPM_EVENT;
            ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                      "CCPM called in invalid scenario\n",
                                      p_entity->rohc_entity_id.ueIndex,
                                      p_entity->rohc_entity_id.lcId,
                                      ROHC_GET_DSC_CID(p_dsc));
            ROHC_EXIT_FN("rohc_dcpm_p2_exec");
            return ROHC_FAILURE;                                      
        
    }
    ROHC_EXIT_FN("rohc_dcpm_p2_exec");
    return ROHC_SUCCESS;           
}

/*****************************************************************************
 * Function Name  : rohcDcpmP6Exec
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 *                  pkt_type - Packet type
 *                  P-ecode - Pointer to error code
 * Outputs        : None
 * Returns        : None
 * Description    : This is the main entry point function for DCPM. This 
 *                  updates the state machine (state and transition state) and
 *                  initiated the received mode transition and ack/nack/static 
 *                  nack.
 *****************************************************************************/
rohc_return_t rohcDcpmP6Exec(
            rohc_entity_t *p_entity ,
            rohc_dsc_tcp_t         *p_dsc , 
            register rohc_U8bit_t  pkt_typ ,
            rohc_error_code_t      *p_ecode
        )
{
    ROHC_ENTER_FN("rohcDcpmP6Exec");
    rohc_U8bit_t event_type;
  
    if(ROHC_UNCOMP == ((rohc_dsc_common_t *)p_dsc)->profile)
    {
        if(( ROHC_IRP0_PKTYPE == pkt_typ) ||
            ( ROHC_NORMALP0_PKTYPE == pkt_typ) )
        {
            return ROHC_SUCCESS;
        }
        else
        {    
            ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                      "CCPM called in invalid scenario\n",
                                      p_entity->rohc_entity_id.ueIndex,
                                      p_entity->rohc_entity_id.lcId,
                                      ROHC_GET_DSC_CID(p_dsc));
            return ROHC_FAILURE;
        }                   
    }
    else if((pkt_typ>= ROHC_CO_P6_COMMON_PKTYPE && pkt_typ <ROHC_MAX_PKT_TYPES ) &&
        ((event_type=rohc_dcpm_map_tbl[pkt_typ][ROHC_MODE_TCP])!=                  
        ROHC_NULL))
    {
        if(!rohcDcpmP6ExecDirectEvt(p_entity, p_dsc, event_type, p_ecode))
        {
            ROHC_ENTER_FN("rohcDcpmP6Exec");                
            return ROHC_FAILURE;
        }
    }
    else
    {
        *p_ecode = ROHC_INVALID_DCPM_EVENT;
        ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                "CCPM called in invalid scenario\n",
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc));
        ROHC_EXIT_FN("rohcDcpmP6Exec");
        return ROHC_FAILURE;                                      

    }
    ROHC_EXIT_FN("rohcDcpmP6Exec");
    return ROHC_SUCCESS;          
}
/*****************************************************************************
 * Function Name  : rohc_dcpm_exec_direct_evt
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 *                  evt_typ - DCPM event type
 *                  P-ecode - Pointer to error code
 * Outputs        : None
 * Returns        : None
 * Description    : This is the main entry point function for DCPM. This
 *                  updates the state machine (state and transition state) and
 *                  initiated the received mode transition and ack/nack/static
 *                  nack with specified evt type.
 *****************************************************************************/
rohc_return_t rohc_dcpm_exec_direct_evt 
        ROHC_PARAMS((p_entity, p_dsc, evt_typ, p_ecode),
        rohc_entity_t *p_entity _AND_
        rohc_dsc_t *p_dsc _AND_ 
        register rohc_U8bit_t  evt_typ _AND_
        rohc_error_code_t *p_ecode
        )
{
    rohc_dcpm_fprtable temp_fptr = ROHC_NULL;
 
    /* + coverity 32370 */
    rohc_error_code_t ecode = ROHC_NO_ERR;
    /* - coverity 32370 */

  rohc_U8bit_t d_mode = p_dsc->d_mode;
  rohc_U8bit_t d_trans_state = p_dsc->d_trans_state;
  rohc_U8bit_t d_state = p_dsc->d_state;

    ROHC_EXIT_FN("rohc_dcpm_exec_direct_evt");
    
    ROHC_LOG(LOG_INFO,"DPU:EID[%u:%u]:CID[%u]:"
                      "Before performing DCPM funtionality for event type[%u]"
                      "## mode[%u], profile[%u], state[%u], "
                      "transition-state[%u] ##\n",
                       p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                       ROHC_GET_DSC_CID(p_dsc),
                       evt_typ,p_dsc->d_mode,p_dsc->profile,
                       p_dsc->d_state,p_dsc->d_trans_state);

    if((d_mode >= ROHC_MODE_U && d_mode <= ROHC_MODE_R) &&
            (d_trans_state >= ROHC_D_TST_DONE &&
             d_trans_state <= ROHC_D_TST_PENDING) &&
            (evt_typ >= ROHC_D_U_START && evt_typ <=ROHC_MAX_D_EVENT) &&
             (d_state >= ROHC_D_ST_NC && d_state <= ROHC_D_ST_FC))
        
    {
            temp_fptr = rohc_dcpm_gen_fpt[d_mode - ROHC_MODE_U]
                    [d_trans_state - ROHC_D_TST_DONE]
                    [evt_typ - ROHC_D_U_START]
                    [d_state - ROHC_D_ST_NC];
        
            if(temp_fptr != ROHC_NULL)
            {
                p_dsc->p_tecode=p_ecode;
				ecode = *p_ecode;
                (temp_fptr)(p_entity, p_dsc );
                if(*(p_dsc->p_tecode)!= ecode)
                    return ROHC_FAILURE;    
            }
            else
            {       
                ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                              "Invalid operation",
                              p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                              ROHC_GET_DSC_CID(p_dsc));
            }   

    }
    else
    {
            *p_ecode = ROHC_INVALID_DCPM_EVENT;
            ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                      "CCPM called in invalid scenario\n",
                                      p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                      ROHC_GET_DSC_CID(p_dsc));
            ROHC_EXIT_FN("rohc_dcpm_exec_direct_evt");
            return ROHC_FAILURE;                                      
        
    }
    
    ROHC_LOG(LOG_INFO,"DPU:EID[%u:%u]:CID[%u]:"
                      "After performing DCPM funtionality for event type[%u]"
                      "## mode[%u], profile[%u], state[%u], "
                      "transition-state[%u] ##\n",
                       p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                       ROHC_GET_DSC_CID(p_dsc),
                       evt_typ,p_dsc->d_mode,p_dsc->profile,
                       p_dsc->d_state,p_dsc->d_trans_state);

    ROHC_EXIT_FN("rohc_dcpm_exec_direct_evt");  
    return ROHC_SUCCESS;    
}   

/*****************************************************************************
 * Function Name  : rohc_dcpm_p1_exec_direct_evt
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 *                  evt_typ - DCPM event type
 *                  P-ecode - Pointer to error code
 * Outputs        : None
 * Returns        : None
 * Description    : This is the main entry point function for DCPM. This
 *                  updates the state machine (state and transition state) and
 *                  initiated the received mode transition and ack/nack/static
 *                  nack with specified evt type.
 *****************************************************************************/
rohc_return_t rohc_dcpm_p1_exec_direct_evt 
        ROHC_PARAMS((p_entity, p_dsc, evt_typ, p_ecode),
        rohc_entity_t *p_entity _AND_
        rohc_dsc_rtp_t *p_dsc _AND_ 
        register rohc_U8bit_t  evt_typ _AND_
         rohc_error_code_t *p_ecode
        )
{
     /* + SPR 17439 */
    rohc_dcpm_fprtable_rtp temp_fptr = ROHC_NULL;
     /* - SPR 17439 */
    /*Coverity Fix 32370 Start*/
    rohc_error_code_t ecode=ROHC_NO_ERR;
    /*Coverity Fix 32370 Stop*/

  rohc_U8bit_t d_mode = p_dsc->d_mode;
  rohc_U8bit_t d_trans_state = p_dsc->d_trans_state;
  rohc_U8bit_t d_state = p_dsc->d_state;

    ROHC_EXIT_FN("rohc_dcpm_p1_exec_direct_evt");
    
    ROHC_LOG(LOG_INFO,"DPU:EID[%u:%u]:CID[%u]:"
                      "Before performing DCPM funtionality for event type[%u]"
                      "## mode[%u], profile[%u], state[%u], "
                      "transition-state[%u] ##\n",
                       p_entity->rohc_entity_id.ueIndex,
                       p_entity->rohc_entity_id.lcId,
                       ROHC_GET_DSC_CID(p_dsc),
                       evt_typ,p_dsc->d_mode,p_dsc->profile,
                       p_dsc->d_state,p_dsc->d_trans_state);

    if((d_mode >= ROHC_MODE_U && d_mode <= ROHC_MODE_R) &&
            (d_trans_state >= ROHC_D_TST_DONE &&
             d_trans_state <= ROHC_D_TST_PENDING) &&
            (evt_typ >= ROHC_D_U_START && evt_typ <=ROHC_MAX_D_EVENT) &&
             (d_state >= ROHC_D_ST_NC && d_state <= ROHC_D_ST_FC))
        
    {
            /* + SPR 17439 */
            temp_fptr = rohc_dcpm_fpt_rtp[d_mode - ROHC_MODE_U]
            /* - SPR 17439 */
                    [d_trans_state - ROHC_D_TST_DONE]
                    [evt_typ - ROHC_D_U_START]
                    [d_state - ROHC_D_ST_NC]
                    [p_dsc->profile - ROHC_UNCOMP];
        
            if(temp_fptr != ROHC_NULL)
            {
                p_dsc->p_tecode=p_ecode;
				ecode = *p_ecode;
                (temp_fptr)(p_entity, p_dsc );
                if(*(p_dsc->p_tecode)!= ecode)
                    return ROHC_FAILURE;    
            }
            else
            {       
                ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                              "Invalid operation",
                              p_entity->rohc_entity_id.ueIndex,
                              p_entity->rohc_entity_id.lcId,
                              ROHC_GET_DSC_CID(p_dsc));
            }   

    }
    else
    {
            *p_ecode = ROHC_INVALID_DCPM_EVENT;
            ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                      "CCPM called in invalid scenario\n",
                                      p_entity->rohc_entity_id.ueIndex,
                                      p_entity->rohc_entity_id.lcId,
                                      ROHC_GET_DSC_CID(p_dsc));
            ROHC_EXIT_FN("rohc_dcpm_p1_exec_direct_evt");
            return ROHC_FAILURE;                                      
        
    }
    
    ROHC_LOG(LOG_INFO,"DPU:EID[%u:%u]:CID[%u]:"
                      "After performing DCPM funtionality for event type[%u]"
                      "## mode[%u], profile[%u], state[%u], "
                      "transition-state[%u] ##\n",
                       p_entity->rohc_entity_id.ueIndex,
                       p_entity->rohc_entity_id.lcId,
                       ROHC_GET_DSC_CID(p_dsc),
                       evt_typ,p_dsc->d_mode,p_dsc->profile,
                       p_dsc->d_state,p_dsc->d_trans_state);

    ROHC_EXIT_FN("rohc_dcpm_p1_exec_direct_evt");  
    return ROHC_SUCCESS;    
}   

/*****************************************************************************
 * Function Name  : rohc_dcpm_p2_exec_direct_evt
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 *                  evt_typ - DCPM event type
 *                  P-ecode - Pointer to error code
 * Outputs        : None
 * Returns        : None
 * Description    : This is the main entry point function for DCPM. This
 *                  updates the state machine (state and transition state) and
 *                  initiated the received mode transition and ack/nack/static
 *                  nack with specified evt type.
 *****************************************************************************/
rohc_return_t rohc_dcpm_p2_exec_direct_evt 
        ROHC_PARAMS((p_entity, p_dsc, evt_typ, p_ecode),
        rohc_entity_t      *p_entity _AND_
        rohc_dsc_udp_t     *p_dsc    _AND_ 
        register rohc_U8bit_t  evt_typ _AND_
         rohc_error_code_t *p_ecode
        )
{
     /* + SPR 17439 */
    rohc_dcpm_fprtable_udp temp_fptr = ROHC_NULL;
     /* - SPR 17439 */
    /* Coverity Fix 32370 start */
    rohc_error_code_t ecode = ROHC_NO_ERR;
    /* Coverity Fix 32370 end */

    rohc_U8bit_t d_mode = p_dsc->d_mode;
    rohc_U8bit_t d_trans_state = p_dsc->d_trans_state;
    rohc_U8bit_t d_state = p_dsc->d_state;

    ROHC_EXIT_FN("rohc_dcpm_p2_exec_direct_evt");
    
    ROHC_LOG(LOG_INFO,"DPU:EID[%u:%u]:CID[%u]:"
                      "Before performing DCPM funtionality for event type[%u]"
                      "## mode[%u], profile[%u], state[%u], "
                      "transition-state[%u] ##\n",
                       p_entity->rohc_entity_id.ueIndex,
                       p_entity->rohc_entity_id.lcId,
                       ROHC_GET_DSC_CID(p_dsc),
                       evt_typ,p_dsc->d_mode,p_dsc->profile,
                       p_dsc->d_state,p_dsc->d_trans_state);

    if((d_mode >= ROHC_MODE_U && d_mode <= ROHC_MODE_R) &&
            (d_trans_state >= ROHC_D_TST_DONE &&
             d_trans_state <= ROHC_D_TST_PENDING) &&
            (evt_typ >= ROHC_D_U_START && evt_typ <=ROHC_MAX_D_EVENT) &&
             (d_state >= ROHC_D_ST_NC && d_state <= ROHC_D_ST_FC))
    {
            /* + SPR 17439 */
            temp_fptr = rohc_dcpm_fpt_udp[p_dsc->d_mode - ROHC_MODE_U]
            /* - SPR 17439 */
                    [d_trans_state - ROHC_D_TST_DONE]
                    [evt_typ - ROHC_D_U_START]
                    [d_state - ROHC_D_ST_NC]
                    [p_dsc->profile - ROHC_UNCOMP];
        
            if(temp_fptr != ROHC_NULL)
            {
                p_dsc->p_tecode=p_ecode;
				ecode = *p_ecode;
                (temp_fptr)(p_entity, p_dsc );
                if(*(p_dsc->p_tecode)!= ecode)
                    return ROHC_FAILURE;    
            }
            else
            {       
                ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                              "Invalid operation",
                              p_entity->rohc_entity_id.ueIndex,
                              p_entity->rohc_entity_id.lcId,
                              ROHC_GET_DSC_CID(p_dsc));
            }   

    }
    else
    {
            *p_ecode = ROHC_INVALID_DCPM_EVENT;
            ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                              "CCPM called in invalid scenario\n",
                              p_entity->rohc_entity_id.ueIndex,
                              p_entity->rohc_entity_id.lcId,
                              ROHC_GET_DSC_CID(p_dsc));
            ROHC_EXIT_FN("rohc_dcpm_p2_exec_direct_evt");
            return ROHC_FAILURE;                                      
        
    }
    
    ROHC_LOG(LOG_INFO,"DPU:EID[%u:%u]:CID[%u]:"
                      "After performing DCPM funtionality for event type[%u]"
                      "## mode[%u], profile[%u], state[%u], "
                      "transition-state[%u] ##\n",
                       p_entity->rohc_entity_id.ueIndex,
                       p_entity->rohc_entity_id.lcId,
                       ROHC_GET_DSC_CID(p_dsc),
                       evt_typ,p_dsc->d_mode,p_dsc->profile,
                       p_dsc->d_state,p_dsc->d_trans_state);

    ROHC_EXIT_FN("rohc_dcpm_p2_exec_direct_evt");  
    return ROHC_SUCCESS;    
}   

/*****************************************************************************
 * Function Name  : rohcDcpmP6ExecDirectEvt
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_dsc - Pointer to rohc decompressor stream context block
 *                  evt_typ - DCPM event type
 *                  P-ecode - Pointer to error code
 * Outputs        : None
 * Returns        : None
 * Description    : This is the main entry point function for DCPM. This
 *                  updates the state machine (state and transition state) and
 *                  initiated the received mode transition and ack/nack/static
 *                  nack with specified evt type.
 *****************************************************************************/
rohc_return_t rohcDcpmP6ExecDirectEvt(
        rohc_entity_t      *p_entity ,
        rohc_dsc_tcp_t     *p_dsc    , 
        register rohc_U8bit_t  evt_typ ,
         rohc_error_code_t *p_ecode
        )
{
     /* + SPR 17439 */
    rohc_dcpm_fprtable_tcp temp_fptr = ROHC_NULL;
     /* - SPR 17439 */
    /*Coverity Fix 40844 start*/
    rohc_error_code_t ecode =ROHC_NO_ERR;
    /*Coverity Fix 40844 end*/

    rohc_U8bit_t d_state = p_dsc->d_state;

    ROHC_EXIT_FN("rohcDcpmP6ExecDirectEvt");
    
    ROHC_LOG(LOG_INFO,"DPU:EID[%u:%u]:CID[%u]:"
                      "Before performing DCPM funtionality for event type[%u]"
                      "profile[%u], state[%u], "
                      "##\n",
                       p_entity->rohc_entity_id.ueIndex,
                       p_entity->rohc_entity_id.lcId,
                       ROHC_GET_DSC_CID(p_dsc),
                       evt_typ,p_dsc->profile,
                       p_dsc->d_state);

    if((evt_typ >= ROHC_D_U_START && evt_typ <=ROHC_MAX_D_EVENT) &&
            (d_state >= ROHC_D_ST_NC && d_state <= ROHC_D_ST_FC))
    {
        /* + SPR 17439 */
        temp_fptr = rohc_dcpm_fpt_tcp[ROHC_MODE_TCP - ROHC_MODE_TCP]      
        /* - SPR 17439 */
            [ROHC_D_TST_TCP - ROHC_D_TST_TCP]
            [evt_typ - ROHC_D_U_START]
            [d_state - ROHC_D_ST_NC]
            [p_dsc->dpib->profile - ROHC_UNCOMP];

        if(temp_fptr != ROHC_NULL)
        {
            p_dsc->p_tecode=p_ecode;
            ecode = *p_ecode;
            (temp_fptr)(p_entity, p_dsc );
            if(*(p_dsc->p_tecode)!= ecode)
                return ROHC_FAILURE;    
        }
        else
        {       
            ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                    "Invalid operation",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc));
        }   

    }
    else
    {
        *p_ecode = ROHC_INVALID_DCPM_EVENT;
        ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                "CCPM called in invalid scenario\n",
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc));
        ROHC_EXIT_FN("rohcDcpmP6ExecDirectEvt");
        return ROHC_FAILURE;                                      

    }

    ROHC_LOG(LOG_INFO,"DPU:EID[%u:%u]:CID[%u]:"
                      "After performing DCPM funtionality for event type[%u]"
                      "## mode[%u], profile[%u], state[%u], "
                      "##\n",
                       p_entity->rohc_entity_id.ueIndex,
                       p_entity->rohc_entity_id.lcId,
                       ROHC_GET_DSC_CID(p_dsc),
                       evt_typ,p_dsc->d_mode,p_dsc->profile,
                       p_dsc->d_state);

    ROHC_EXIT_FN("rohcDcpmP6ExecDirectEvt");  
    return ROHC_SUCCESS;   
}
/*****************************************************************************
 * Function Name  : rohc_dcpm_validate_pkt
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : 
 * Returns        : ROHC_TRUE or ROHC_FALSE
 * Description    : This function is used to validate the packet type.
 *****************************************************************************/
rohc_return_t rohc_dcpm_validate_pkt
        ROHC_PARAMS((p_entity,p_dsc,pkt_typ),
        rohc_entity_t         *p_entity _AND_
        rohc_dsc_common_t     *p_dsc _AND_
        register rohc_U8bit_t pkt_typ
        )

{
    ROHC_ENTER_FN("rohc_dcpm_validate_pkt");
              /* SPR 6174 changes Start*/
            /* This is to handle change of Profile from Compressed to Uncompressed 
 *             ** Header Packets. No Validation is needed in case of IR Profile 0.  
 *                         */

            /* Determine the Correctness of the Packet on the basis of 
 *              * Profile received in IR , IR -With Dynamic , IR-Dyn 
 *                           * for these packet Validation is moved to parse_ir_irdyn_pkts 
 *                                        * function as there it is Parsing the Profile 
 *                                                     */
            if( ( ROHC_IRP0_PKTYPE == pkt_typ ) ||
                ( ROHC_IRDYN_PKTTYPE == pkt_typ  ) ||
                ( ROHC_IR_WITH_DYN_PKTTYPE == pkt_typ ) ||
                ( ROHC_IR_WITHOUT_DYN_PKTTYPE == pkt_typ ))
            {
                return ROHC_TRUE;
            }
            /* SPR 6174 changes End*/
    switch(p_dsc->profile)
    {
      case ROHC_UNCOMP:
            /* Coverity Fix 72762 start */
                /* deleted dead code */
            /* Coverity Fix 72762 end */
            if(p_dsc->d_state ==ROHC_DUN_ST_FC)
            { 
                if(pkt_typ == ROHC_NORMALP0_PKTYPE)
                {
                    return ROHC_TRUE;
                }
            }   
            /* - coverity 72762 */
            
            return ROHC_FALSE;
                            
            break;

      case ROHC_IP_UDP_RTP:
      case ROHC_IP_UDP:
      case ROHC_IP_ESP:
            if(pkt_typ>= ROHC_R0P1_PKTTYPE && pkt_typ < (ROHC_UOR2E3P23_PKTTYPE + 1) )
            { 
                if((rohc_pkt_val_str[p_dsc->profile - ROHC_IP_UDP_RTP]
                                [p_dsc->d_state - ROHC_D_ST_NC] 
                                [(pkt_typ-1)/32]) & (1<<((pkt_typ-1)%32)) )
                {
                    return ROHC_TRUE;
                }
                else
                {
                    /* +- SPR 17777 */
                    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
                    /* +- SPR 17777 */
                    ROHC_LOG(LOG_INFO,
                                "DCPM:EID[%u:%u]:CID[%u]:"
                                 "Packet type [%u] is received in invalid"
                                 "state [Profile:%u, Decompressor state:%u]\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc),
                                  pkt_typ,p_dsc->d_state,
                                  p_dsc->profile);
                    return ROHC_FALSE;
                }  
             }
             else
             {   
                ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                                  "Unsupported Packet type [%u] \n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc),
                                  pkt_typ);
                return ROHC_FALSE;
             }  
            break;
      case ROHC_IP_TCP :
            if (ROHC_MAX_PKT_TYPES > pkt_typ)
            {
                return ROHC_TRUE;
            }
            else 
            {
                return ROHC_FALSE;
            }
      default :
            ROHC_LOG(LOG_INFO,"DCPM:EID[%u:%u]:CID[%u]:"
                    "Unsupported profile [%u] \n",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc),
                    p_dsc->profile); 

            ROHC_EXIT_FN("rohc_dcpm_validate_pkt");   
            return ROHC_FALSE;                  

    }   
    ROHC_EXIT_FN("rohc_dcpm_validate_pkt");
    return ROHC_FALSE;                  
}

/*****************************************************************************
 * Function Name  : rohc_dcpm_send_ack
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to send ACK to peer side compressor.
 *****************************************************************************/
rohc_void_t rohc_dcpm_send_ack
            ROHC_PARAMS( (p_entity, p_dsc, crc_flag),
            rohc_entity_t *p_entity _AND_ 
            rohc_dsc_t *p_dsc _AND_
            rohc_U8bit_t crc_flag)
{
    rohc_feedback_t fb;
    rohc_U8bit_t index;
    
    ROHC_ENTER_FN("rohc_dcpm_send_ack");
    for(index = 0; index < ROHC_MAX_OPT; index++)
    {
        ROHC_DCPM_INIT_OPT(fb.opt_type[index], fb.opt_val[index]);
    }


#ifdef ROHC_ENABLE_SPARSE_ACK
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        /*only first R-0-CRC must be acked*/
        if(!(p_dsc->dpib->rcv_pkt_typ == p_dsc->pkt_typ &&
            p_dsc->dpib->rcv_pkt_typ == ROHC_R0CRCP1_PKTTYPE &&
            p_dsc->pkt_count == 1))
        {
            ROHC_EXIT_FN("rohc_dcpm_send_ack");
            return;         
        }
        /* only first n packet must be acked */
        else if(p_dsc->r_n++ > ROHC_MAX_N_VAL)
        {
            ROHC_EXIT_FN("rohc_dcpm_send_ack");
            return ;
        }
        /* one packet per RTT must be acked except R-0-CRC pkt*/
        else if(p_dsc->pkt_count != p_dsc->prtt)
        {
            ROHC_EXIT_FN("rohc_dcpm_send_ack");
            return; 
        }
    }
    
#endif  
    
    if(p_dsc->decom_res) /* send ack with the */
    {
        if(crc_flag)
        {
            fb.feedback_type = ROHC_FEEDBACK_TYPE2;          
        }   
        fb.sn = p_dsc->sn_prev;                     
        fb.ack_type = ROHC_ACK;   
    }
    else
    {
        if(p_dsc->d_state == ROHC_D_ST_FC)
        {
            fb.ack_type = ROHC_STATIC_NACK;   
        }
        else
        {
            fb.ack_type = ROHC_NACK;   
        }   
    }       
    fb.mode = p_dsc->d_mode;
    if(crc_flag)
    {
        fb.opt_type[0] = ROHC_FBO_CRC;
        fb.opt_type[1] = ROHC_INVALID_OPT_TYPE;
    }
    else
    {
        fb.opt_type[0] = ROHC_INVALID_OPT_TYPE;
    }   
    fb.cid =p_dsc->cid; 
    /*: call PBM to create the feedback packet */
    if(ROHC_FAILURE == rohc_d_make_feedback_packet(p_entity,&fb,
                                                   p_dsc->p_tecode))
    {
        return;
    }   
    ROHC_EXIT_FN("rohc_dcpm_send_ack");
}

/*****************************************************************************
 * Function Name  : rohc_dcpm_p1_send_ack
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to send ACK to peer side compressor.
 *****************************************************************************/
rohc_void_t rohc_dcpm_p1_send_ack
            ROHC_PARAMS( (p_entity, p_dsc, crc_flag),
            rohc_entity_t   *p_entity   _AND_ 
            rohc_dsc_rtp_t  *p_dsc      _AND_
            rohc_U8bit_t    crc_flag)
{
    rohc_feedback_t fb;
    rohc_U8bit_t index;
    ROHC_ENTER_FN("rohc_dcpm_send_ack");
    for(index = 0; index < ROHC_MAX_OPT; index++)
    {
        ROHC_DCPM_INIT_OPT(fb.opt_type[index], fb.opt_val[index]);
    }
#ifdef ROHC_ENABLE_SPARSE_ACK
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        /*only first R-0-CRC must be acked*/
        if(!(p_dsc->dpib->rcv_pkt_typ == p_dsc->pkt_typ &&
            p_dsc->dpib->rcv_pkt_typ == ROHC_R0CRCP1_PKTTYPE &&
            p_dsc->pkt_count == 1))
        {
            ROHC_EXIT_FN("rohc_dcpm_p1_send_ack");
            return;         
        }
        /* only first n packet must be acked */
        else if(p_dsc->r_n++ > ROHC_MAX_N_VAL)
        {
            ROHC_EXIT_FN("rohc_dcpm_p1_send_ack");
            return ;
        }
        /* one packet per RTT must be acked except R-0-CRC pkt*/
        else if(p_dsc->pkt_count != p_dsc->prtt)
        {
            ROHC_EXIT_FN("rohc_dcpm_p1_send_ack");
            return; 
        }
    }
    
#endif  
    
    if(p_dsc->decom_res) /* send ack with the */
    {
        if(crc_flag)
        {
            fb.feedback_type = ROHC_FEEDBACK_TYPE2;          
        }   
        fb.sn = p_dsc->sn_prev;                     
        fb.ack_type = ROHC_ACK;   
    }
    else
    {
        if(p_dsc->d_state == ROHC_D_ST_FC)
        {
            fb.ack_type = ROHC_STATIC_NACK;   
        }
        else
        {
            fb.ack_type = ROHC_NACK;   
        }   
    }       
    fb.mode = p_dsc->d_mode;
    if(crc_flag)
    {
        fb.opt_type[0] = ROHC_FBO_CRC;
        fb.opt_type[1] = ROHC_INVALID_OPT_TYPE;
    }
    else
    {
        fb.opt_type[0] = ROHC_INVALID_OPT_TYPE;
    }   
    fb.cid =p_dsc->cid; 
    /*: call PBM to create the feedback packet */
    if(ROHC_FAILURE == rohc_d_make_feedback_packet(p_entity,&fb,
                                                   p_dsc->p_tecode))
    {
        return;
    }   

    /* Incrementing Stats */
    if(fb.ack_type == ROHC_ACK)
    {
        pdcpStats.statsRohc.numberOfACKCreated++;
    }   
    else if(fb.ack_type == ROHC_STATIC_NACK)
    {
        pdcpStats.statsRohc.numberOfSNACKCreated++;
    }
    else if(fb.ack_type == ROHC_NACK)
    {
        pdcpStats.statsRohc.numberOfNACKCreated++;
    }   
    ROHC_EXIT_FN("rohc_dcpm_p1_send_ack");
}

/*****************************************************************************
 * Function Name  : rohc_dcpm_p2_send_ack
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to send ACK to peer side compressor.
 *****************************************************************************/
rohc_void_t rohc_dcpm_p2_send_ack
            ROHC_PARAMS( (p_entity, p_dsc, crc_flag),
            rohc_entity_t  *p_entity _AND_ 
            rohc_dsc_udp_t *p_dsc    _AND_
            rohc_U8bit_t   crc_flag)
{
    rohc_feedback_t fb;
    rohc_U8bit_t index;
    ROHC_ENTER_FN("rohc_dcpm_p2_send_ack");
    for(index = 0; index < ROHC_MAX_OPT; index++)
    {
        ROHC_DCPM_INIT_OPT(fb.opt_type[index], fb.opt_val[index]);
    }
#ifdef ROHC_ENABLE_SPARSE_ACK
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        /*only first R-0-CRC must be acked*/
        if(!(p_dsc->dpib->rcv_pkt_typ == p_dsc->pkt_typ &&
            p_dsc->dpib->rcv_pkt_typ == ROHC_R0CRCP1_PKTTYPE &&
            p_dsc->pkt_count == 1))
        {
            ROHC_EXIT_FN("rohc_dcpm_p2_send_ack");
            return;         
        }
        /* only first n packet must be acked */
        else if(p_dsc->r_n++ > ROHC_MAX_N_VAL)
        {
            ROHC_EXIT_FN("rohc_dcpm_p2_send_ack");
            return ;
        }
        /* one packet per RTT must be acked except R-0-CRC pkt*/
        else if(p_dsc->pkt_count != p_dsc->prtt)
        {
            ROHC_EXIT_FN("rohc_dcpm_p2_send_ack");
            return; 
        }
    }
    
#endif  
    
    if(p_dsc->decom_res) /* send ack with the */
    {
        if(crc_flag)
        {
            fb.feedback_type = ROHC_FEEDBACK_TYPE2;          
        }   
        fb.sn = p_dsc->sn_prev;                     
        fb.ack_type = ROHC_ACK;   
    }
    else
    {
        if(p_dsc->d_state == ROHC_D_ST_FC)
        {
            fb.ack_type = ROHC_STATIC_NACK;   
        }
        else
        {
            fb.ack_type = ROHC_NACK;   
        }   
    }       
    fb.mode = p_dsc->d_mode;
    if(crc_flag)
    {
        fb.opt_type[0] = ROHC_FBO_CRC;
        fb.opt_type[1] = ROHC_INVALID_OPT_TYPE;
    }
    else
    {
        fb.opt_type[0] = ROHC_INVALID_OPT_TYPE;
    }   
    fb.cid =p_dsc->cid; 
    /*: call PBM to create the feedback packet */
    if(ROHC_FAILURE == rohc_d_make_feedback_packet(p_entity,&fb,
                                                   p_dsc->p_tecode))
    {
        return;
    }   

    /* Incrementing Stats */
    if(fb.ack_type == ROHC_ACK)
    {
        pdcpStats.statsRohc.numberOfACKCreated++;
    }   
    else if(fb.ack_type == ROHC_STATIC_NACK)
    {
        pdcpStats.statsRohc.numberOfSNACKCreated++;
    }
    else if(fb.ack_type == ROHC_NACK)
    {
        pdcpStats.statsRohc.numberOfNACKCreated++;
    }   

    ROHC_EXIT_FN("rohc_dcpm_p2_send_ack");
}

/*****************************************************************************
 * Function Name  : rohcDcpmP6SendAck
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_dsc - Pointer to rohc decompressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to send ACK to peer side compressor.
 *****************************************************************************/
rohc_void_t rohcDcpmP6SendAck(
            rohc_entity_t  *p_entity , 
            rohc_dsc_tcp_t *p_dsc)
{
    rohc_feedback_t fb;
    rohc_U8bit_t index;
    ROHC_ENTER_FN("rohcDcpmP6SendAck");
    for(index = 0; index < ROHC_MAX_OPT; index++)
    {
        ROHC_DCPM_INIT_OPT(fb.opt_type[index], fb.opt_val[index]);
    }
#ifdef ROHC_ENABLE_SPARSE_ACK
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        /*only first R-0-CRC must be acked*/
        if(!(p_dsc->dpib->rcv_pkt_typ == p_dsc->pkt_typ &&
            p_dsc->dpib->rcv_pkt_typ == ROHC_R0CRCP1_PKTTYPE &&
            p_dsc->pkt_count == 1))
        {
            ROHC_EXIT_FN("rohcDcpmP6SendAck");
            return;         
        }
        /* only first n packet must be acked */
        else if(p_dsc->r_n++ > ROHC_MAX_N_VAL)
        {
            ROHC_EXIT_FN("rohcDcpmP6SendAck");
            return ;
        }
        /* one packet per RTT must be acked except R-0-CRC pkt*/
        else if(p_dsc->pkt_count != p_dsc->prtt)
        {
            ROHC_EXIT_FN("rohcDcpmP6SendAck");
            return; 
        }
    }

#endif  

    fb.feedback_type = ROHC_FEEDBACK_TYPE1;          
    fb.msn = p_dsc->dpib->decoded_msn;                     
    fb.ack_type = ROHC_ACK;   
    fb.cid =p_dsc->cid; 
    /*: call PBM to create the feedback packet */
    if(ROHC_FAILURE == rohcDP6MakeFeedbackPacket(p_entity,&fb,
                p_dsc->p_tecode))
    {
        return;
    }   

    /* Incrementing Stats */
    if(ROHC_ACK == fb.ack_type)
    {
        pdcpStats.statsRohc.numberOfACKCreated++;
    }   

    ROHC_EXIT_FN("rohcDcpmP6SendAck");
}

/*****************************************************************************
 * Function Name  : rohc_dcpm_send_nack
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to send NACK to peer side compressor.
 *****************************************************************************/
rohc_void_t rohc_dcpm_send_nack 
            ROHC_PARAMS( (p_entity, p_dsc, crc_flag),
            rohc_entity_t *p_entity _AND_ 
            rohc_dsc_t *p_dsc _AND_
            rohc_U8bit_t crc_flag)
{
    rohc_feedback_t fb;
    rohc_U8bit_t index;
    ROHC_ENTER_FN("rohc_dcpm_send_nack  ");
    for(index = 0; index < ROHC_MAX_OPT; index++)
    {
        ROHC_DCPM_INIT_OPT(fb.opt_type[index], fb.opt_val[index]);
    }
    fb.feedback_type = ROHC_FEEDBACK_TYPE2;
    fb.ack_type = ROHC_NACK;
    fb.mode = p_dsc->d_mode;
    fb.cid =p_dsc->cid; 
    if(crc_flag)
    {
        fb.opt_type[0] = ROHC_FBO_CRC;
        fb.opt_val[0] = 0;
        fb.opt_type[1] = ROHC_INVALID_OPT_TYPE;
    }
    else
    {
        fb.opt_type[0] = ROHC_INVALID_OPT_TYPE;
    }   

    fb.sn = p_dsc->sn_prev; 
    
    if(ROHC_FAILURE == rohc_d_make_feedback_packet(p_entity,&fb,
                                                   p_dsc->p_tecode))
    {
        return;
    }   
    /* call PBM to create the feedback packet */
    ROHC_EXIT_FN("rohc_dcpm_send_nack");
}   

/*****************************************************************************
 * Function Name  : rohc_dcpm_p1_send_nack
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to send NACK to peer side compressor.
 *****************************************************************************/
rohc_void_t rohc_dcpm_p1_send_nack 
            ROHC_PARAMS( (p_entity, p_dsc, crc_flag),
            rohc_entity_t   *p_entity   _AND_ 
            rohc_dsc_rtp_t  *p_dsc      _AND_
            rohc_U8bit_t    crc_flag)
{
    rohc_feedback_t fb;
    rohc_U8bit_t index;
    ROHC_ENTER_FN("rohc_dcpm_p1_send_nack  ");
    for(index = 0; index < ROHC_MAX_OPT; index++)
    {
        ROHC_DCPM_INIT_OPT(fb.opt_type[index], fb.opt_val[index]);
    }
    fb.feedback_type = ROHC_FEEDBACK_TYPE2;
    fb.ack_type = ROHC_NACK;
    fb.mode = p_dsc->d_mode;
    fb.cid =p_dsc->cid; 
    if(crc_flag)
    {
        fb.opt_type[0] = ROHC_FBO_CRC;
        fb.opt_val[0] = 0;
        fb.opt_type[1] = ROHC_INVALID_OPT_TYPE;
    }
    else
    {
        fb.opt_type[0] = ROHC_INVALID_OPT_TYPE;
    }   

    fb.sn = p_dsc->sn_prev; 
    
    if(ROHC_FAILURE == rohc_d_make_feedback_packet(p_entity,&fb,
                                                   p_dsc->p_tecode))
    {
        return;
    }   
    /* call PBM to create the feedback packet */
    ROHC_EXIT_FN("rohc_dcpm_p1_send_nack");
}   

/*****************************************************************************
 * Function Name  : rohc_dcpm_p2_send_nack
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to send NACK to peer side compressor.
 *****************************************************************************/
rohc_void_t rohc_dcpm_p2_send_nack 
            ROHC_PARAMS( (p_entity, p_dsc, crc_flag),
            rohc_entity_t  *p_entity _AND_ 
            rohc_dsc_udp_t *p_dsc    _AND_
            rohc_U8bit_t   crc_flag)
{
    rohc_feedback_t fb;
    rohc_U8bit_t index;
    ROHC_ENTER_FN("rohc_dcpm_p2_send_nack  ");
    for(index = 0; index < ROHC_MAX_OPT; index++)
    {
        ROHC_DCPM_INIT_OPT(fb.opt_type[index], fb.opt_val[index]);
    }
    fb.feedback_type = ROHC_FEEDBACK_TYPE2;
    fb.ack_type = ROHC_NACK;
    fb.mode = p_dsc->d_mode;
    fb.cid =p_dsc->cid; 
    if(crc_flag)
    {
        fb.opt_type[0] = ROHC_FBO_CRC;
        fb.opt_val[0] = 0;
        fb.opt_type[1] = ROHC_INVALID_OPT_TYPE;
    }
    else
    {
        fb.opt_type[0] = ROHC_INVALID_OPT_TYPE;
    }   

    fb.sn = p_dsc->sn_prev; 
    
    if(ROHC_FAILURE == rohc_d_make_feedback_packet(p_entity,&fb,
                                                   p_dsc->p_tecode))
    {
        return;
    }   
    /* call PBM to create the feedback packet */
    ROHC_EXIT_FN("rohc_dcpm_p2_send_nack");
}   

/*****************************************************************************
 * Function Name  : rohcDcpmP6SendNack
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to send NACK to peer side compressor.
 *****************************************************************************/
rohc_void_t rohcDcpmP6SendNack (
            rohc_entity_t  *p_entity ,  
            rohc_dsc_tcp_t *p_dsc)     
{
    rohc_feedback_t fb;
    rohc_U8bit_t index;
    ROHC_ENTER_FN("rohcDcpmP6SendNack");
    for(index = 0; index < ROHC_MAX_OPT; index++)
    {
        ROHC_DCPM_INIT_OPT(fb.opt_type[index], fb.opt_val[index]);
    }
    fb.feedback_type = ROHC_FEEDBACK_TYPE2;
    fb.ack_type = ROHC_NACK;
    fb.cid =p_dsc->cid; 
    fb.msn = p_dsc->msn_prev; 
    
    if(ROHC_FAILURE == rohcDP6MakeFeedbackPacket(p_entity,&fb,
                                                   p_dsc->p_tecode))
    {
        return;
    }   
    /* call PBM to create the feedback packet */
    if(ROHC_NACK == fb.ack_type)
    {
        pdcpStats.statsRohc.numberOfNACKCreated++;
    }   
    ROHC_EXIT_FN("rohcDcpmP6SendNack");
}   

/*****************************************************************************
 * Function Name  : rohc_dcpm_send_static_nack
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to send STATIC NACK to peer side
 *                  compressor.
 *****************************************************************************/
rohc_void_t rohc_dcpm_send_static_nack 
            ROHC_PARAMS( (p_entity, p_dsc, crc_flag),
            rohc_entity_t *p_entity _AND_ 
            rohc_dsc_t *p_dsc _AND_
            rohc_U8bit_t crc_flag)
{
    rohc_feedback_t fb;
    rohc_U8bit_t index;
    ROHC_ENTER_FN("rohc_dcpm_send_static_nack");
    for(index = 0; index < ROHC_MAX_OPT; index++)
    {
        ROHC_DCPM_INIT_OPT(fb.opt_type[index], fb.opt_val[index]);
    }
    fb.feedback_type = ROHC_FEEDBACK_TYPE2;
    fb.ack_type = ROHC_STATIC_NACK;
    fb.mode = p_dsc->d_mode;
    fb.sn = p_dsc->sn_prev;
    fb.cid =p_dsc->cid; 
    if(crc_flag)
    {
        fb.opt_type[0] = ROHC_FBO_CRC;
        fb.opt_type[1] = ROHC_INVALID_OPT_TYPE;
    }
    else
    {
        fb.opt_type[0] = ROHC_INVALID_OPT_TYPE;
    }   
    fb.sn = p_dsc->sn_prev; 
    if(ROHC_FAILURE == rohc_d_make_feedback_packet(p_entity,&fb,
                                                   p_dsc->p_tecode))
    {
        return;
    }   
    /* call PBM to create the feedback packet */
    ROHC_EXIT_FN("rohc_dcpm_send_static_nack");
}

/*****************************************************************************
 * Function Name  : rohc_dcpm_p1_send_static_nack
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to send STATIC NACK to peer side
 *                  compressor.
 *****************************************************************************/
rohc_void_t rohc_dcpm_p1_send_static_nack 
            ROHC_PARAMS( (p_entity, p_dsc, crc_flag),
            rohc_entity_t   *p_entity   _AND_ 
            rohc_dsc_rtp_t  *p_dsc      _AND_
            rohc_U8bit_t    crc_flag)
{
    rohc_feedback_t fb;
    rohc_U8bit_t index;
    ROHC_ENTER_FN("rohc_dcpm_p1_send_static_nack");
    for(index = 0; index < ROHC_MAX_OPT; index++)
    {
        ROHC_DCPM_INIT_OPT(fb.opt_type[index], fb.opt_val[index]);
    }
    fb.feedback_type = ROHC_FEEDBACK_TYPE2;
    fb.ack_type = ROHC_STATIC_NACK;
    fb.mode = p_dsc->d_mode;
    fb.sn = p_dsc->sn_prev;
    fb.cid =p_dsc->cid; 
    if(crc_flag)
    {
        fb.opt_type[0] = ROHC_FBO_CRC;
        fb.opt_type[1] = ROHC_INVALID_OPT_TYPE;
    }
    else
    {
        fb.opt_type[0] = ROHC_INVALID_OPT_TYPE;
    }   
    fb.sn = p_dsc->sn_prev; 
    if(ROHC_FAILURE == rohc_d_make_feedback_packet(p_entity,&fb,
                                                   p_dsc->p_tecode))
    {
        return;
    }   
    /* call PBM to create the feedback packet */
    ROHC_EXIT_FN("rohc_dcpm_p1_send_static_nack");
}

/*****************************************************************************
 * Function Name  : rohc_dcpm_p2_send_static_nack
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to send STATIC NACK to peer side
 *                  compressor.
 *****************************************************************************/
rohc_void_t rohc_dcpm_p2_send_static_nack 
            ROHC_PARAMS( (p_entity, p_dsc, crc_flag),
            rohc_entity_t  *p_entity _AND_ 
            rohc_dsc_udp_t *p_dsc    _AND_
            rohc_U8bit_t   crc_flag)
{
    rohc_feedback_t fb;
    rohc_U8bit_t index;
    ROHC_ENTER_FN("rohc_dcpm_p2_send_static_nack");
    for(index = 0; index < ROHC_MAX_OPT; index++)
    {
        ROHC_DCPM_INIT_OPT(fb.opt_type[index], fb.opt_val[index]);
    }
    fb.feedback_type = ROHC_FEEDBACK_TYPE2;
    fb.ack_type = ROHC_STATIC_NACK;
    fb.mode = p_dsc->d_mode;
    fb.sn = p_dsc->sn_prev;
    fb.cid =p_dsc->cid; 
    if(crc_flag)
    {
        fb.opt_type[0] = ROHC_FBO_CRC;
        fb.opt_type[1] = ROHC_INVALID_OPT_TYPE;
    }
    else
    {
        fb.opt_type[0] = ROHC_INVALID_OPT_TYPE;
    }   
    fb.sn = p_dsc->sn_prev; 
    if(ROHC_FAILURE == rohc_d_make_feedback_packet(p_entity,&fb,
                                                   p_dsc->p_tecode))
    {
        return;
    }   
    /* call PBM to create the feedback packet */
    ROHC_EXIT_FN("rohc_dcpm_p2_send_static_nack");
}

/*****************************************************************************
 * Function Name  : rohcDcpmP6SendStaticNack
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to send STATIC NACK to peer side
 *                  compressor.
 *****************************************************************************/
rohc_void_t rohcDcpmP6SendStaticNack( 
            rohc_entity_t  *p_entity , 
            rohc_dsc_tcp_t *p_dsc)    
{
    rohc_feedback_t fb;
    rohc_U8bit_t index;
    ROHC_ENTER_FN("rohcDcpmP6SendStaticNack");
    for(index = 0; index < ROHC_MAX_OPT; index++)
    {
        ROHC_DCPM_INIT_OPT(fb.opt_type[index], fb.opt_val[index]);
    }
    fb.feedback_type = ROHC_FEEDBACK_TYPE2;
    fb.ack_type = ROHC_STATIC_NACK;
    fb.msn = p_dsc->msn_prev;
    fb.cid =p_dsc->cid; 
    if(ROHC_FAILURE == rohcDP6MakeFeedbackPacket(p_entity,&fb,
                                                   p_dsc->p_tecode))
    {
        return;
    }   
    /* call PBM to create the feedback packet */
    if(ROHC_STATIC_NACK == fb.ack_type)
    {
        pdcpStats.statsRohc.numberOfSNACKCreated++;
    }
    ROHC_EXIT_FN("rohcDcpmP6SendStaticNack");
}

/*****************************************************************************
 * Function Name  : rohc_dcpm_in_evt_tbl
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *****************************************************************************/
rohc_void_t rohc_dcpm_in_evt_tbl ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_dcpm_in_evt_tbl");
    rohc_dcpm_map_tbl[ROHC_R0P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_dcpm_map_tbl[ROHC_R0P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_dcpm_map_tbl[ROHC_R0P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_0_ET;

    rohc_dcpm_map_tbl[ROHC_R0CRCP1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_dcpm_map_tbl[ROHC_R0CRCP1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_dcpm_map_tbl[ROHC_R0CRCP1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_0_CRC_ET;

    rohc_dcpm_map_tbl[ROHC_UO0P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_D_U_UO_0_ET;
    rohc_dcpm_map_tbl[ROHC_UO0P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_UO_0_ET;
    rohc_dcpm_map_tbl[ROHC_UO0P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_NULL;

    rohc_dcpm_map_tbl[ROHC_R1P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                  ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                  ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                  ROHC_D_R_R_1_ET;    

    rohc_dcpm_map_tbl[ROHC_R1E0P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1E0P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1E0P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_D_R_R_1_ET;    

    rohc_dcpm_map_tbl[ROHC_R1E1P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1E1P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1E1P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_D_R_R_1_ET;    

    rohc_dcpm_map_tbl[ROHC_R1E2P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1E2P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1E2P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_D_R_R_1_ET;    

    rohc_dcpm_map_tbl[ROHC_R1E3P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1E3P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1E3P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_D_R_R_1_ET;    

    rohc_dcpm_map_tbl[ROHC_R1IDP1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1IDP1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1IDP1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_D_R_R_1_ET;    

    rohc_dcpm_map_tbl[ROHC_R1IDE0P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1IDE0P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1IDE0P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_1_ET;    

    rohc_dcpm_map_tbl[ROHC_R1IDE1P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1IDE1P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1IDE1P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_1_ET;    

    rohc_dcpm_map_tbl[ROHC_R1IDE2P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1IDE2P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1IDE2P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_1_ET;    

    rohc_dcpm_map_tbl[ROHC_R1IDE3P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1IDE3P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1IDE3P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_1_ET;    

    rohc_dcpm_map_tbl[ROHC_R1TSP1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1TSP1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1TSP1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_D_R_R_1_ET;    

    rohc_dcpm_map_tbl[ROHC_R1TSE0P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1TSE0P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1TSE0P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_1_ET;    

    rohc_dcpm_map_tbl[ROHC_R1TSE1P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1TSE1P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1TSE1P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_1_ET;    

    rohc_dcpm_map_tbl[ROHC_R1TSE2P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1TSE2P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_dcpm_map_tbl[ROHC_R1TSE2P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_1_ET;    

    rohc_dcpm_map_tbl[ROHC_R1TSE3P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_dcpm_map_tbl[ROHC_R1TSE3P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_dcpm_map_tbl[ROHC_R1TSE3P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_1_ET;

    rohc_dcpm_map_tbl[ROHC_UO1P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] =  
                                                    ROHC_D_U_UO_1_ET;   
    rohc_dcpm_map_tbl[ROHC_UO1P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] =  
                                                    ROHC_D_O_UO_1_ET;   
    rohc_dcpm_map_tbl[ROHC_UO1P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] =  
                                                    ROHC_NULL;          

    rohc_dcpm_map_tbl[ROHC_UO1IDP1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] =  
                                                      ROHC_D_U_UO_1_ET;   
    rohc_dcpm_map_tbl[ROHC_UO1IDP1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] =  
                                                      ROHC_D_O_UO_1_ET;   
    rohc_dcpm_map_tbl[ROHC_UO1IDP1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] =  
                                                      ROHC_NULL;          

    rohc_dcpm_map_tbl[ROHC_UO1IDE0P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] =  
                                                        ROHC_D_U_UO_1_ET;   
    rohc_dcpm_map_tbl[ROHC_UO1IDE0P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] =  
                                                        ROHC_D_O_UO_1_ET;   
    rohc_dcpm_map_tbl[ROHC_UO1IDE0P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] =  
                                                        ROHC_NULL;          

    rohc_dcpm_map_tbl[ROHC_UO1IDE1P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] =  
                                                        ROHC_D_U_UO_1_ET;   
    rohc_dcpm_map_tbl[ROHC_UO1IDE1P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] =  
                                                        ROHC_D_O_UO_1_ET;   
    rohc_dcpm_map_tbl[ROHC_UO1IDE1P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] =  
                                                        ROHC_NULL;          

    rohc_dcpm_map_tbl[ROHC_UO1IDE2P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] =  
                                                        ROHC_D_U_UO_1_ET;   
    rohc_dcpm_map_tbl[ROHC_UO1IDE2P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] =  
                                                        ROHC_D_O_UO_1_ET;   
    rohc_dcpm_map_tbl[ROHC_UO1IDE2P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] =  
                                                        ROHC_NULL;          

    rohc_dcpm_map_tbl[ROHC_UO1IDE3P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] =  
                                                        ROHC_D_U_UO_1_ET;   
    rohc_dcpm_map_tbl[ROHC_UO1IDE3P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] =  
                                                        ROHC_D_O_UO_1_ET;   
    rohc_dcpm_map_tbl[ROHC_UO1IDE3P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] =  
                                                        ROHC_NULL;          

    rohc_dcpm_map_tbl[ROHC_UO1TSP1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_D_U_UO_1_ET;
    rohc_dcpm_map_tbl[ROHC_UO1TSP1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_UO_1_ET;
    rohc_dcpm_map_tbl[ROHC_UO1TSP1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_NULL;

    rohc_dcpm_map_tbl[ROHC_UOR2P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                  ROHC_D_U_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                  ROHC_D_O_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                  ROHC_D_R_R_2_ET;      

    rohc_dcpm_map_tbl[ROHC_UOR2E0P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_D_U_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2E0P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_D_O_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2E0P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_D_R_R_2_ET;      

    rohc_dcpm_map_tbl[ROHC_UOR2E1P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_D_U_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2E1P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_D_O_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2E1P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_D_R_R_2_ET;      

    rohc_dcpm_map_tbl[ROHC_UOR2E2P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_D_U_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2E2P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_D_O_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2E2P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_D_R_R_2_ET;      

    rohc_dcpm_map_tbl[ROHC_UOR2E3P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_D_U_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2E3P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_D_O_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2E3P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_D_R_R_2_ET;      

    rohc_dcpm_map_tbl[ROHC_UOR2IDP1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_D_U_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2IDP1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_D_O_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2IDP1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_D_R_R_2_ET;      

    rohc_dcpm_map_tbl[ROHC_UOR2IDE0P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_D_U_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2IDE0P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2IDE0P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_2_ET;      

    rohc_dcpm_map_tbl[ROHC_UOR2IDE1P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_D_U_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2IDE1P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2IDE1P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_2_ET;      

    rohc_dcpm_map_tbl[ROHC_UOR2IDE2P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_D_U_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2IDE2P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2IDE2P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_2_ET;      

    rohc_dcpm_map_tbl[ROHC_UOR2IDE3P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_D_U_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2IDE3P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2IDE3P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_2_ET;      

    rohc_dcpm_map_tbl[ROHC_UOR2TSP1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_D_U_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2TSP1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_D_O_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2TSP1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_D_R_R_2_ET;      

    rohc_dcpm_map_tbl[ROHC_UOR2TSE0P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_D_U_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2TSE0P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2TSE0P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_2_ET;      

    rohc_dcpm_map_tbl[ROHC_UOR2TSE1P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_D_U_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2TSE1P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2TSE1P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_2_ET;      

    rohc_dcpm_map_tbl[ROHC_UOR2TSE2P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_D_U_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2TSE2P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_UO_2_ET;     
    rohc_dcpm_map_tbl[ROHC_UOR2TSE2P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_2_ET;      

    rohc_dcpm_map_tbl[ROHC_UOR2TSE3P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_D_U_UO_2_ET;
    rohc_dcpm_map_tbl[ROHC_UOR2TSE3P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_UO_2_ET;
    rohc_dcpm_map_tbl[ROHC_UOR2TSE3P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_2_ET;

    rohc_dcpm_map_tbl[ROHC_R1P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_dcpm_map_tbl[ROHC_R1P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_dcpm_map_tbl[ROHC_R1P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_1_ET;

    rohc_dcpm_map_tbl[ROHC_R1E0P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_dcpm_map_tbl[ROHC_R1E0P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_dcpm_map_tbl[ROHC_R1E0P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_1_ET;

    rohc_dcpm_map_tbl[ROHC_R1E1P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_dcpm_map_tbl[ROHC_R1E1P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_dcpm_map_tbl[ROHC_R1E1P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_1_ET;

    rohc_dcpm_map_tbl[ROHC_R1E2P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_dcpm_map_tbl[ROHC_R1E2P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_dcpm_map_tbl[ROHC_R1E2P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_1_ET;

    rohc_dcpm_map_tbl[ROHC_R1E3P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_dcpm_map_tbl[ROHC_R1E3P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_dcpm_map_tbl[ROHC_R1E3P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_1_ET;

    rohc_dcpm_map_tbl[ROHC_UO1P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_D_O_UO_1_ET;
    rohc_dcpm_map_tbl[ROHC_UO1P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_UO_1_ET;
    rohc_dcpm_map_tbl[ROHC_UO1P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_NULL;

    rohc_dcpm_map_tbl[ROHC_UOR2P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_D_U_UO_2_ET;
    rohc_dcpm_map_tbl[ROHC_UOR2P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_UO_2_ET;
    rohc_dcpm_map_tbl[ROHC_UOR2P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_2_ET;

    rohc_dcpm_map_tbl[ROHC_UOR2E0P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_D_U_UO_2_ET;
    rohc_dcpm_map_tbl[ROHC_UOR2E0P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_UO_2_ET;
    rohc_dcpm_map_tbl[ROHC_UOR2E0P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_2_ET;

    rohc_dcpm_map_tbl[ROHC_UOR2E1P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_D_U_UO_2_ET;
    rohc_dcpm_map_tbl[ROHC_UOR2E1P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_UO_2_ET;
    rohc_dcpm_map_tbl[ROHC_UOR2E1P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_2_ET;

    rohc_dcpm_map_tbl[ROHC_UOR2E2P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_D_U_UO_2_ET;
    rohc_dcpm_map_tbl[ROHC_UOR2E2P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_UO_2_ET;
    rohc_dcpm_map_tbl[ROHC_UOR2E2P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_2_ET;

    rohc_dcpm_map_tbl[ROHC_UOR2E3P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_D_U_UO_2_ET;
    rohc_dcpm_map_tbl[ROHC_UOR2E3P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_UO_2_ET;
    rohc_dcpm_map_tbl[ROHC_UOR2E3P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_R_2_ET;

    rohc_dcpm_map_tbl[ROHC_IRP0_PKTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_D_U_IR_ET;
    rohc_dcpm_map_tbl[ROHC_IRP0_PKTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_IR_ET;
    rohc_dcpm_map_tbl[ROHC_IRP0_PKTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_IR_ET;

    rohc_dcpm_map_tbl[ROHC_NORMALP0_PKTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_dcpm_map_tbl[ROHC_NORMALP0_PKTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_dcpm_map_tbl[ROHC_NORMALP0_PKTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_NULL;

    rohc_dcpm_map_tbl[ROHC_IR_WITHOUT_DYN_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_D_U_IR_ET;
    rohc_dcpm_map_tbl[ROHC_IR_WITHOUT_DYN_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_IR_ET;
    rohc_dcpm_map_tbl[ROHC_IR_WITHOUT_DYN_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_IR_ET;

    rohc_dcpm_map_tbl[ROHC_IR_WITH_DYN_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_D_U_IR_ET;
    rohc_dcpm_map_tbl[ROHC_IR_WITH_DYN_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_IR_ET;
    rohc_dcpm_map_tbl[ROHC_IR_WITH_DYN_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_IR_ET;

    rohc_dcpm_map_tbl[ROHC_IRDYN_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                     ROHC_D_U_IRDYN_ET;
    rohc_dcpm_map_tbl[ROHC_IRDYN_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_D_O_IRDYN_ET;
    rohc_dcpm_map_tbl[ROHC_IRDYN_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_D_R_IRDYN_ET;

    /* Initializing for Profile 6 */
    rohc_dcpm_map_tbl[ROHC_IR_WITH_DYN_PKTTYPE][ROHC_MODE_TCP]= ROHC_D_IR_IRCR_ET;
                                                      
    rohc_dcpm_map_tbl[ROHC_IRP6CR_PKTYPE][ROHC_MODE_TCP] = 
                                                      ROHC_D_IR_IRCR_ET;

    rohc_dcpm_map_tbl[ROHC_IRDYN_PKTTYPE][ROHC_MODE_TCP] = 
                                                      ROHC_D_IR_DYN_CO_ET;

    rohc_dcpm_map_tbl[ROHC_CO_P6_COMMON_PKTYPE][ROHC_MODE_TCP] = 
                                                      ROHC_D_IR_DYN_CO_ET;

    rohc_dcpm_map_tbl[ROHC_CO_P6_RND1_PKTYPE][ROHC_MODE_TCP] = 
                                                      ROHC_D_OTHERS_ET;

    rohc_dcpm_map_tbl[ROHC_CO_P6_RND2_PKTYPE][ROHC_MODE_TCP] = 
                                                      ROHC_D_OTHERS_ET;

    rohc_dcpm_map_tbl[ROHC_CO_P6_RND3_PKTYPE][ROHC_MODE_TCP] = 
                                                      ROHC_D_OTHERS_ET;

    rohc_dcpm_map_tbl[ROHC_CO_P6_RND4_PKTYPE][ROHC_MODE_TCP] = 
                                                      ROHC_D_OTHERS_ET;

    rohc_dcpm_map_tbl[ROHC_CO_P6_RND5_PKTYPE][ROHC_MODE_TCP] = 
                                                      ROHC_D_OTHERS_ET;

    rohc_dcpm_map_tbl[ROHC_CO_P6_RND6_PKTYPE][ROHC_MODE_TCP] = 
                                                      ROHC_D_OTHERS_ET;

    rohc_dcpm_map_tbl[ROHC_CO_P6_RND7_PKTYPE][ROHC_MODE_TCP] = 
                                                      ROHC_D_OTHERS_ET;

    rohc_dcpm_map_tbl[ROHC_CO_P6_RND8_PKTYPE][ROHC_MODE_TCP] = 
                                                      ROHC_D_OTHERS_ET;

    rohc_dcpm_map_tbl[ROHC_CO_P6_SEQ1_PKTYPE][ROHC_MODE_TCP] = 
                                                      ROHC_D_OTHERS_ET;

    rohc_dcpm_map_tbl[ROHC_CO_P6_SEQ2_PKTYPE][ROHC_MODE_TCP] = 
                                                      ROHC_D_OTHERS_ET;

    rohc_dcpm_map_tbl[ROHC_CO_P6_SEQ3_PKTYPE][ROHC_MODE_TCP] = 
                                                      ROHC_D_OTHERS_ET;

    rohc_dcpm_map_tbl[ROHC_CO_P6_SEQ4_PKTYPE][ROHC_MODE_TCP] = 
                                                      ROHC_D_OTHERS_ET;

    rohc_dcpm_map_tbl[ROHC_CO_P6_SEQ5_PKTYPE][ROHC_MODE_TCP] = 
                                                      ROHC_D_OTHERS_ET;

    rohc_dcpm_map_tbl[ROHC_CO_P6_SEQ6_PKTYPE][ROHC_MODE_TCP] = 
                                                      ROHC_D_OTHERS_ET;

    rohc_dcpm_map_tbl[ROHC_CO_P6_SEQ7_PKTYPE][ROHC_MODE_TCP] = 
                                                      ROHC_D_OTHERS_ET;

    rohc_dcpm_map_tbl[ROHC_CO_P6_SEQ8_PKTYPE][ROHC_MODE_TCP] = 
                                                      ROHC_D_OTHERS_ET;

    ROHC_EXIT_FN("rohc_dcpm_in_evt_tbl");
}
        

