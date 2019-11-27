/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_cprm_init_p23.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the function definition to initialize
 *                     the packet info table for profile p2 and p3.  
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_cprm_init_p23.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
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
#include "rohc_cprm_proto.h"

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



rohc_p23_pkt_typ_info_t  
        rohc_p23_pkt_typ_info_uo[ROHC_P23_UO_MAX_PACKETS];

rohc_p23_pkt_typ_info_t  
        rohc_p23_pkt_typ_info_r[ROHC_P23_R_MAX_PACKETS];


const rohc_U8bit_t rohc_p23_uo_fo_pkt_typ[2] = {
                                    0xfe,
                                    0x0,
                                  };
const rohc_U8bit_t rohc_p23_uo_crc_3[2] = {
                                    0x5,
                                    0x0,
                                  };
const rohc_U8bit_t rohc_p23_uo_crc_7[2] = {
                                    0xfa,
                                    0x0,
                                  };
const rohc_U8bit_t rohc_p23_uo_both_ipid[2] = {
                                    0xa0,
                                    0x0,
                                  };
const rohc_U8bit_t rohc_p23_uo_one_ipid[2] = {
                                    0x5e,
                                    0x0,
                                  };
const rohc_U8bit_t rohc_p23_uo_ext_3[2] = {
                                    0xc0,
                                    0x0,
                                  };
const rohc_U8bit_t rohc_p23_uo_mt_pending[2] = {
                                    0xfa,
                                    0x0,
                                  };
const rohc_U8bit_t rohc_p23_r_fo_pkt_typ_1[2] = {
                                    0xfc,
                                    0x0,
                                  };
const rohc_U8bit_t rohc_p23_r_fo_pkt_typ_2[2] = {
                                    0x0,
                                    0x3f,
                                  };
const rohc_U8bit_t rohc_p23_r_both_ipid[2] = {
                                    0xa0,
                                    0x28,
                                  };
const rohc_U8bit_t rohc_p23_r_one_ipid[2] = {
                                    0x5c,
                                    0x17,
                                  };
const rohc_U8bit_t rohc_p23_r_ext_3[2] = {
                                    0xc0,
                                    0x30,
                                  };
const rohc_U8bit_t rohc_p23_r_mt_pending[2] = {
                                    0x00,
                                    0x3f,
                                  };



/*****************************************************************************
**      FUNCTION:
**              rohc_init_p23_uo_table
******************************************************************************
**
**      DESCRIPTION:
**              This is funtion initializes the rohc_p23_pkt_typ_info_uo table 
**              of UO mode packets for IP/UDP and IP/ESP profiles. 
**      ARGUMENTS:
**      RETURN VALUE:
**      NOTES:
**               
*****************************************************************************/
                        

rohc_void_t rohc_init_p23_uo_table ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_init_p23_uo_table");
 /* Defines for UO mode packet types for IP/UDP and IP/ESP profile */
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UO_0].sn_bits        = 4;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UO_0].in_ip_id_bits  = 0;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UO_0].out_ip_id_bits = 0;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UO_0].pbm_pkt_typ   =  
                                            ROHC_UO0P1_PKTTYPE ;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2].sn_bits        = 5;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2].in_ip_id_bits  = 0;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2].out_ip_id_bits = 0;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2].pbm_pkt_typ   =  
                                            ROHC_UOR2P23_PKTTYPE ;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UO_1].sn_bits        = 5;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UO_1].in_ip_id_bits  = 6;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UO_1].out_ip_id_bits = 0;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UO_1].pbm_pkt_typ   =  
                                            ROHC_UO1P23_PKTTYPE ;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_0].sn_bits        = 8;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_0].in_ip_id_bits  = 3;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_0].out_ip_id_bits = 0;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_0].pbm_pkt_typ   =  
                                            ROHC_UOR2E0P23_PKTTYPE  ;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_1].sn_bits        = 8;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_1].in_ip_id_bits  = 11;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_1].out_ip_id_bits = 0;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_1].pbm_pkt_typ   =  
                                            ROHC_UOR2E1P23_PKTTYPE;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_2].sn_bits        = 8;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_2].in_ip_id_bits  = 11;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_2].out_ip_id_bits = 8;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_2].pbm_pkt_typ   =  
                                            ROHC_UOR2E2P23_PKTTYPE  ;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_3_B1].sn_bits        = 13;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_3_B1].in_ip_id_bits  = 16;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_3_B1].out_ip_id_bits = 0;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_3_B1].pbm_pkt_typ   =  
                                            ROHC_UOR2E3P23_PKTTYPE  ;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_3_B2].sn_bits        = 13;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_3_B2].in_ip_id_bits  = 16;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_3_B2].out_ip_id_bits = 16;
    rohc_p23_pkt_typ_info_uo[ROHC_UOP23_UOR_2_EXT_3_B2].pbm_pkt_typ   =  
                                            ROHC_UOR2E3P23_PKTTYPE;             
    ROHC_EXIT_FN("rohc_init_p23_uo_table");
}


/*****************************************************************************
**      FUNCTION:
**              rohc_init_p23_r_table
******************************************************************************
**
**      DESCRIPTION:
**              This is funtion initializes the rohc_p23_pkt_typ_info_r table 
**              of UO mode packets for IP/UDP/RTP profile. 
**      ARGUMENTS:
**      RETURN VALUE:
**      NOTES:
**               
*****************************************************************************/
                        

rohc_void_t rohc_init_p23_r_table ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_init_p23_r_table");
     /* Defines for R mode packet types for IP/UDP and IP/ESP profile */
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_0].sn_bits        = 6;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_0].in_ip_id_bits  = 0;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_0].out_ip_id_bits = 0;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_0].pbm_pkt_typ   =  
                                            ROHC_R0P1_PKTTYPE  ;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_0_CRC].sn_bits        = 7;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_0_CRC].in_ip_id_bits  = 0;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_0_CRC].out_ip_id_bits = 0;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_0_CRC].pbm_pkt_typ   =  
                                            ROHC_R0CRCP1_PKTTYPE  ;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1].sn_bits        = 5;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1].in_ip_id_bits  = 6;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1].out_ip_id_bits = 0;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1].pbm_pkt_typ   =  
                                            ROHC_R1P23_PKTTYPE  ;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_0].sn_bits        = 8;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_0].in_ip_id_bits  = 9;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_0].out_ip_id_bits = 0;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_0].pbm_pkt_typ   =  
                                            ROHC_R1E0P23_PKTTYPE  ;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_1].sn_bits        = 8;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_1].in_ip_id_bits  = 17;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_1].out_ip_id_bits = 0;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_1].pbm_pkt_typ   =  
                                            ROHC_R1E1P23_PKTTYPE  ;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_2].sn_bits        = 8;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_2].in_ip_id_bits  = 14;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_2].out_ip_id_bits = 11;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_2].pbm_pkt_typ   =  
                                            ROHC_R1E2P23_PKTTYPE  ;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_3_B1].sn_bits        = 8;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_3_B1].in_ip_id_bits  = 16;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_3_B1].out_ip_id_bits = 0;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_3_B1].pbm_pkt_typ   =  
                                            ROHC_R1E3P23_PKTTYPE  ;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_3_B2].sn_bits        = 8;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_3_B2].in_ip_id_bits  = 16;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_3_B2].out_ip_id_bits = 16;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_R_1_EXT_3_B2].pbm_pkt_typ   =  
                                            ROHC_R1E3P23_PKTTYPE ;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2].sn_bits        = 5;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2].in_ip_id_bits  = 0;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2].out_ip_id_bits = 0;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2].pbm_pkt_typ   =  
                                            ROHC_UOR2P23_PKTTYPE  ;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_0].sn_bits        = 8;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_0].in_ip_id_bits  = 3;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_0].out_ip_id_bits = 0;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_0].pbm_pkt_typ   =  
                                            ROHC_UOR2E0P23_PKTTYPE  ;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_1].sn_bits        = 8;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_1].in_ip_id_bits  = 11;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_1].out_ip_id_bits = 0;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_1].pbm_pkt_typ   =  
                                            ROHC_UOR2E1P23_PKTTYPE  ;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_2].sn_bits        = 8;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_2].in_ip_id_bits  = 11;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_2].out_ip_id_bits = 8;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_2].pbm_pkt_typ   =  
                                            ROHC_UOR2E2P23_PKTTYPE  ;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_3_B1].sn_bits        = 13;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_3_B1].in_ip_id_bits  = 16;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_3_B1].out_ip_id_bits = 0;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_3_B1].pbm_pkt_typ   =  
                                            ROHC_UOR2E3P23_PKTTYPE  ;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_3_B2].sn_bits        = 13;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_3_B2].in_ip_id_bits  = 16;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_3_B2].out_ip_id_bits = 16;
    rohc_p23_pkt_typ_info_r[ROHC_RP23_UOR_2_EXT_3_B2].pbm_pkt_typ   =  
                                            ROHC_UOR2E3P23_PKTTYPE  ;
    ROHC_EXIT_FN("rohc_init_p23_r_table");
}




