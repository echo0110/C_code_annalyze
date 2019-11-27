/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dprm_init.c,v 1.3.36.4 2010/12/02 08:58:39 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file provides the function to initialize the
 *                     DPRM fucntion pointer tables.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_dprm_init.c,v $
 * Revision 1.3.36.4  2010/12/02 08:58:39  gur22059
 * Conflicting profile 1 functions were renamed acc to new Naming Convention
 *
 * Revision 1.3.36.3  2010/11/09 05:25:36  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.3.36.2  2010/10/13 03:59:06  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.3.36.1  2010/10/01 14:07:43  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.3  2009/05/28 04:26:50  gur19836
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
#include "rohc_dprm_proto.h"
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
/* + SPR 17439 */
rohc_dprm_fprtable_rtp rohc_dprm_p1_type_fpt[ROHC_MAX_PKT - ROHC_BASE_PKT ]; 

rohc_dprm_fprtable_udp rohc_dprm_p2_fpt[ROHC_MAX_PKT - ROHC_BASE_PKT ];

rohc_dprm_fprtable_tcp rohcDprmP6FptG[ROHC_MAX_PKT - ROHC_BASE_PKT ];
/* - SPR 17439 */
rohc_U8bit_t rohc_dprm_p1_pkt_cat[ROHC_MAX_PKT_TYPES];
rohc_U8bit_t rohc_dprm_p23_pkt_cat[ROHC_MAX_PKT_TYPES];
rohc_U8bit_t rohc_dprm_p6_pkt_cat_g[ROHC_MAX_PKT_TYPES];
rohc_U8bit_t rohc_dprm_pkt_crc_bit[ROHC_MAX_PKT_TYPES];

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/




rohc_void_t rohc_dprm_init_fpt ROHC_NO_PARAMS()
{

        ROHC_ENTER_FN("rohc_dprm_init");
        
        rohc_dprm_p1_type_fpt[ROHC_NONE_UPDATING_PKT]= 
                            rohc_dprm_p1_none_updating_pkt;   
        rohc_dprm_p1_type_fpt[ROHC_NOT_SUPPORTED_PKT]=
                            rohc_dprm_p1_not_supported_pkt;       
        rohc_dprm_p1_type_fpt[ROHC_BASE_UPDATE_PKT]=     
                            rohc_dprm_p1_type_base_update_pkt;   
        rohc_dprm_p1_type_fpt[ROHC_BASE_EXT3_UPDATE_PKT]=    
                            rohc_dprm_p1_type_base_ext3_update_pkt;  
        rohc_dprm_p1_type_fpt[ROHC_IR_IRDYN_UPDATE_PKT]=
                            rohc_dprm_p1_type_ir_irdyn_update_pkt;       

        
        rohc_dprm_p1_pkt_cat[ROHC_R0P1_PKTTYPE] = 
                                        ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R0CRCP1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UO0P1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1P1_PKTTYPE] = 
                                        ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1E0P1_PKTTYPE] = 
                                        ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1E1P1_PKTTYPE] = 
                                        ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1E2P1_PKTTYPE] = 
                                        ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1E3P1_PKTTYPE] = 
                                        ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1IDP1_PKTTYPE] = 
                                        ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1IDE0P1_PKTTYPE] = 
                                        ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1IDE1P1_PKTTYPE] = 
                                        ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1IDE2P1_PKTTYPE] = 
                                        ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1IDE3P1_PKTTYPE] = 
                                        ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1TSP1_PKTTYPE] = 
                                        ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1TSE0P1_PKTTYPE] = 
                                        ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1TSE1P1_PKTTYPE] = 
                                        ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1TSE2P1_PKTTYPE] = 
                                        ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1TSE3P1_PKTTYPE] = 
                                        ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UO1P1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UO1IDP1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UO1IDE0P1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UO1IDE1P1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UO1IDE2P1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UO1IDE3P1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UO1TSP1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2P1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2E0P1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2E1P1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2E2P1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2E3P1_PKTTYPE] = 
                                        ROHC_BASE_EXT3_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2IDP1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2IDE0P1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2IDE1P1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2IDE2P1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2IDE3P1_PKTTYPE] = 
                                        ROHC_BASE_EXT3_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2TSP1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2TSE0P1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2TSE1P1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2TSE2P1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2TSE3P1_PKTTYPE] = 
                                        ROHC_BASE_EXT3_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1P23_PKTTYPE] = 
                                        ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1E0P23_PKTTYPE] = 
                                        ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1E1P23_PKTTYPE] = 
                                        ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1E2P23_PKTTYPE] = 
                                        ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_R1E3P23_PKTTYPE] = 
                                        ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UO1P23_PKTTYPE] = 
                                        ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2P23_PKTTYPE] = 
                                        ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2E0P23_PKTTYPE] = 
                                        ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2E1P23_PKTTYPE] = 
                                        ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2E2P23_PKTTYPE] = 
                                        ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_UOR2E3P23_PKTTYPE] = 
                                        ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_IRP0_PKTYPE] = 
                                        ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_NORMALP0_PKTYPE] = 
                                        ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_IR_WITHOUT_DYN_PKTTYPE] = 
                                        ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_IR_WITH_DYN_PKTTYPE] = 
                                        ROHC_IR_IRDYN_UPDATE_PKT;
        rohc_dprm_p1_pkt_cat[ROHC_IRDYN_PKTTYPE] = 
                                        ROHC_IR_IRDYN_UPDATE_PKT;
 

        /*p2 -fpt */
        
        rohc_dprm_p2_fpt[ROHC_NONE_UPDATING_PKT]= 
                            rohc_dprm_p2_none_updating_pkt;   
        rohc_dprm_p2_fpt[ROHC_NOT_SUPPORTED_PKT]=
                            rohc_dprm_p2_not_supported_pkt;       
        rohc_dprm_p2_fpt[ROHC_BASE_UPDATE_PKT]=        
                            rohc_dprm_p2_base_update_pkt;  
        rohc_dprm_p2_fpt[ROHC_BASE_EXT3_UPDATE_PKT]=   
                            rohc_dprm_p2_base_ext3_update_pkt;     
        rohc_dprm_p2_fpt[ROHC_IR_IRDYN_UPDATE_PKT]=
                            rohc_dprm_p2_ir_irdyn_update_pkt; 
         

        /*p6 -fpt */
        
        rohcDprmP6FptG[ROHC_NONE_UPDATING_PKT]= 
                            rohc_dprm_p6_none_updating_pkt;       
        rohcDprmP6FptG[ROHC_NOT_SUPPORTED_PKT]=
                            rohc_dprm_p6_not_supported_pkt;         
        rohcDprmP6FptG[ROHC_CR_BASE_UPDATE_PKT]=
                             rohcDprmP6CRBaseUpdatePkt;
        rohcDprmP6FptG[ROHC_BASE_UPDATE_PKT]=        
                            rohcDprmP6BaseUpdatePkt;        
        rohcDprmP6FptG[ROHC_IR_IRDYN_UPDATE_PKT]=
                            rohc_dprm_p6_ir_irdyn_update_pkt; 
         
        /*init table for profile p6*/
        rohc_dprm_p6_pkt_cat_g[ROHC_IR_WITH_DYN_PKTTYPE] = 
                                        ROHC_IR_IRDYN_UPDATE_PKT;
        rohc_dprm_p6_pkt_cat_g[ROHC_IRDYN_PKTTYPE] = 
                                        ROHC_IR_IRDYN_UPDATE_PKT;
        rohc_dprm_p6_pkt_cat_g[ROHC_IRP6CR_PKTYPE] =
                                        ROHC_CR_BASE_UPDATE_PKT;
        rohc_dprm_p6_pkt_cat_g[ROHC_CO_P6_COMMON_PKTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;   
        rohc_dprm_p6_pkt_cat_g[ROHC_CO_P6_RND1_PKTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p6_pkt_cat_g[ROHC_CO_P6_RND2_PKTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p6_pkt_cat_g[ROHC_CO_P6_RND3_PKTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p6_pkt_cat_g[ROHC_CO_P6_RND4_PKTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p6_pkt_cat_g[ROHC_CO_P6_RND5_PKTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p6_pkt_cat_g[ROHC_CO_P6_RND6_PKTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p6_pkt_cat_g[ROHC_CO_P6_RND7_PKTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p6_pkt_cat_g[ROHC_CO_P6_RND8_PKTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p6_pkt_cat_g[ROHC_CO_P6_SEQ1_PKTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p6_pkt_cat_g[ROHC_CO_P6_SEQ2_PKTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p6_pkt_cat_g[ROHC_CO_P6_SEQ3_PKTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p6_pkt_cat_g[ROHC_CO_P6_SEQ4_PKTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p6_pkt_cat_g[ROHC_CO_P6_SEQ5_PKTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p6_pkt_cat_g[ROHC_CO_P6_SEQ6_PKTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p6_pkt_cat_g[ROHC_CO_P6_SEQ7_PKTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p6_pkt_cat_g[ROHC_CO_P6_SEQ8_PKTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;

        /*  init table for profile p23   */
        rohc_dprm_p23_pkt_cat[ROHC_R0P1_PKTTYPE] = 
                                        ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R0CRCP1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UO0P1_PKTTYPE] = 
                                        ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1E0P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1E1P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1E2P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1E3P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1IDP1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1IDE0P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1IDE1P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1IDE2P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1IDE3P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1TSP1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1TSE0P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1TSE1P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1TSE2P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1TSE3P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UO1P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UO1IDP1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UO1IDE0P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UO1IDE1P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UO1IDE2P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UO1IDE3P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UO1TSP1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2E0P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2E1P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2E2P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2E3P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2IDP1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2IDE0P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2IDE1P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2IDE2P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2IDE3P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2TSP1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2TSE0P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2TSE1P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2TSE2P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2TSE3P1_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1P23_PKTTYPE] = 
                                                ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1E0P23_PKTTYPE] = 
                                                ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1E1P23_PKTTYPE] = 
                                                ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1E2P23_PKTTYPE] = 
                                                ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_R1E3P23_PKTTYPE] = 
                                                ROHC_NONE_UPDATING_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UO1P23_PKTTYPE] = 
                                                ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2P23_PKTTYPE] = 
                                                ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2E0P23_PKTTYPE] = 
                                                ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2E1P23_PKTTYPE] = 
                                                ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2E2P23_PKTTYPE] = 
                                                ROHC_BASE_UPDATE_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_UOR2E3P23_PKTTYPE] = 
                                                ROHC_BASE_EXT3_UPDATE_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_IRP0_PKTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_NORMALP0_PKTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_IR_WITHOUT_DYN_PKTTYPE] = 
                                                ROHC_NOT_SUPPORTED_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_IR_WITH_DYN_PKTTYPE] = 
                                                ROHC_IR_IRDYN_UPDATE_PKT;
        rohc_dprm_p23_pkt_cat[ROHC_IRDYN_PKTTYPE] = 
                                                ROHC_IR_IRDYN_UPDATE_PKT;
                                                
                                            
    /* init crc-bit */
    
        rohc_dprm_pkt_crc_bit[ROHC_R0P1_PKTTYPE] = 0;
        rohc_dprm_pkt_crc_bit[ROHC_R0CRCP1_PKTTYPE] =7; 
        rohc_dprm_pkt_crc_bit[ROHC_UO0P1_PKTTYPE] = 3;
        rohc_dprm_pkt_crc_bit[ROHC_R1P1_PKTTYPE] = 0;
        rohc_dprm_pkt_crc_bit[ROHC_R1E0P1_PKTTYPE] =  0;
        rohc_dprm_pkt_crc_bit[ROHC_R1E1P1_PKTTYPE] =  0;
        rohc_dprm_pkt_crc_bit[ROHC_R1E2P1_PKTTYPE] =  0;
        rohc_dprm_pkt_crc_bit[ROHC_R1E3P1_PKTTYPE] =  0;
        rohc_dprm_pkt_crc_bit[ROHC_R1IDP1_PKTTYPE] =  0;
        rohc_dprm_pkt_crc_bit[ROHC_R1IDE0P1_PKTTYPE] =0; 
        rohc_dprm_pkt_crc_bit[ROHC_R1IDE1P1_PKTTYPE] =0; 
        rohc_dprm_pkt_crc_bit[ROHC_R1IDE2P1_PKTTYPE] =0; 
        rohc_dprm_pkt_crc_bit[ROHC_R1IDE3P1_PKTTYPE] =0; 
        rohc_dprm_pkt_crc_bit[ROHC_R1TSP1_PKTTYPE] =  0;
        rohc_dprm_pkt_crc_bit[ROHC_R1TSE0P1_PKTTYPE] =0; 
        rohc_dprm_pkt_crc_bit[ROHC_R1TSE1P1_PKTTYPE] =0; 
        rohc_dprm_pkt_crc_bit[ROHC_R1TSE2P1_PKTTYPE] =0; 
        rohc_dprm_pkt_crc_bit[ROHC_R1TSE3P1_PKTTYPE] =0; 
        rohc_dprm_pkt_crc_bit[ROHC_UO1P1_PKTTYPE] =     3; 
        rohc_dprm_pkt_crc_bit[ROHC_UO1IDP1_PKTTYPE] =   3;
        rohc_dprm_pkt_crc_bit[ROHC_UO1IDE0P1_PKTTYPE] = 3;
        rohc_dprm_pkt_crc_bit[ROHC_UO1IDE1P1_PKTTYPE] = 3;
        rohc_dprm_pkt_crc_bit[ROHC_UO1IDE2P1_PKTTYPE] = 3;
        rohc_dprm_pkt_crc_bit[ROHC_UO1IDE3P1_PKTTYPE] = 3;
        rohc_dprm_pkt_crc_bit[ROHC_UO1TSP1_PKTTYPE] =   3;
        rohc_dprm_pkt_crc_bit[ROHC_UOR2P1_PKTTYPE] =    7; 
        rohc_dprm_pkt_crc_bit[ROHC_UOR2E0P1_PKTTYPE] =  7;
        rohc_dprm_pkt_crc_bit[ROHC_UOR2E1P1_PKTTYPE] =  7;
        rohc_dprm_pkt_crc_bit[ROHC_UOR2E2P1_PKTTYPE] =  7;
        rohc_dprm_pkt_crc_bit[ROHC_UOR2E3P1_PKTTYPE] =  7;
        rohc_dprm_pkt_crc_bit[ROHC_UOR2IDP1_PKTTYPE] =  7;
        rohc_dprm_pkt_crc_bit[ROHC_UOR2IDE0P1_PKTTYPE] =7; 
        rohc_dprm_pkt_crc_bit[ROHC_UOR2IDE1P1_PKTTYPE] =7; 
        rohc_dprm_pkt_crc_bit[ROHC_UOR2IDE2P1_PKTTYPE] =7; 
        rohc_dprm_pkt_crc_bit[ROHC_UOR2IDE3P1_PKTTYPE] =7; 
        rohc_dprm_pkt_crc_bit[ROHC_UOR2TSP1_PKTTYPE] =  7;
        rohc_dprm_pkt_crc_bit[ROHC_UOR2TSE0P1_PKTTYPE] =7; 
        rohc_dprm_pkt_crc_bit[ROHC_UOR2TSE1P1_PKTTYPE] =7; 
        rohc_dprm_pkt_crc_bit[ROHC_UOR2TSE2P1_PKTTYPE] =7; 
        rohc_dprm_pkt_crc_bit[ROHC_UOR2TSE3P1_PKTTYPE] =7; 
        rohc_dprm_pkt_crc_bit[ROHC_R1P23_PKTTYPE] =     0;
        rohc_dprm_pkt_crc_bit[ROHC_R1E0P23_PKTTYPE] =   0;
        rohc_dprm_pkt_crc_bit[ROHC_R1E1P23_PKTTYPE] =   0;
        rohc_dprm_pkt_crc_bit[ROHC_R1E2P23_PKTTYPE] =   0;
        rohc_dprm_pkt_crc_bit[ROHC_R1E3P23_PKTTYPE] =   0;
        rohc_dprm_pkt_crc_bit[ROHC_UO1P23_PKTTYPE] =    3;
        rohc_dprm_pkt_crc_bit[ROHC_UOR2P23_PKTTYPE] =   7;
        rohc_dprm_pkt_crc_bit[ROHC_UOR2E0P23_PKTTYPE] = 7;
        rohc_dprm_pkt_crc_bit[ROHC_UOR2E1P23_PKTTYPE] = 7;
        rohc_dprm_pkt_crc_bit[ROHC_UOR2E2P23_PKTTYPE] = 7;
        rohc_dprm_pkt_crc_bit[ROHC_UOR2E3P23_PKTTYPE] = 7;
        rohc_dprm_pkt_crc_bit[ROHC_IRP0_PKTYPE] =       0;  
        rohc_dprm_pkt_crc_bit[ROHC_NORMALP0_PKTYPE] =   0;
        rohc_dprm_pkt_crc_bit[ROHC_IR_WITHOUT_DYN_PKTTYPE] =  0;
        rohc_dprm_pkt_crc_bit[ROHC_IR_WITH_DYN_PKTTYPE] = 8;
        rohc_dprm_pkt_crc_bit[ROHC_IRDYN_PKTTYPE] =       8;    
	/* TCP Packet crc initialization */
        rohc_dprm_pkt_crc_bit[ROHC_CO_P6_COMMON_PKTYPE] = 7;    
        rohc_dprm_pkt_crc_bit[ROHC_CO_P6_RND1_PKTYPE] =  3;    
        rohc_dprm_pkt_crc_bit[ROHC_CO_P6_RND2_PKTYPE] =  3;    
        rohc_dprm_pkt_crc_bit[ROHC_CO_P6_RND3_PKTYPE] =  3;    
        rohc_dprm_pkt_crc_bit[ROHC_CO_P6_RND4_PKTYPE] =  3;    
        rohc_dprm_pkt_crc_bit[ROHC_CO_P6_RND5_PKTYPE] =  3;    
        rohc_dprm_pkt_crc_bit[ROHC_CO_P6_RND6_PKTYPE] =  3;    
        rohc_dprm_pkt_crc_bit[ROHC_CO_P6_RND7_PKTYPE] =  3;    
        rohc_dprm_pkt_crc_bit[ROHC_CO_P6_RND8_PKTYPE] =  7;    
        rohc_dprm_pkt_crc_bit[ROHC_CO_P6_SEQ1_PKTYPE] =  3;    
        rohc_dprm_pkt_crc_bit[ROHC_CO_P6_SEQ2_PKTYPE] =  3;    
        rohc_dprm_pkt_crc_bit[ROHC_CO_P6_SEQ3_PKTYPE] =  3;    
        rohc_dprm_pkt_crc_bit[ROHC_CO_P6_SEQ4_PKTYPE] =  3;    
        rohc_dprm_pkt_crc_bit[ROHC_CO_P6_SEQ5_PKTYPE] =  3;    
        rohc_dprm_pkt_crc_bit[ROHC_CO_P6_SEQ6_PKTYPE] =  3;    
        rohc_dprm_pkt_crc_bit[ROHC_CO_P6_SEQ7_PKTYPE] =  3;    
        rohc_dprm_pkt_crc_bit[ROHC_CO_P6_SEQ8_PKTYPE] =  7;    
        ROHC_EXIT_FN("rohc_dprm_init");                                             
}
