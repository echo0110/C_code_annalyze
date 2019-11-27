/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_cprm_init_p1.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the function definition to initialize
 *                     the packet info table for profile p1.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_cprm_init_p1.c,v $
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


rohc_p1_pkt_typ_info_t 
                        rohc_p1_pkt_typ_info_uo[ROHC_P1_UO_MAX_PACKETS];

rohc_p1_pkt_typ_info_t
                        rohc_p1_pkt_typ_info_r[ROHC_P1_R_MAX_PACKETS];
    

const rohc_U8bit_t rohc_p1_r_fo_pkt_typ_1[9] = {
                                    0x2a,
                                    0x0,
                                    0x0,
                                    0x55,
                                    0x65,
                                    0x1,
                                    0x0,
                                    0x0,
                                    0x0,
                                  };
const rohc_U8bit_t rohc_p1_r_fo_pkt_typ_2[9] = {
                                    0xd4,
                                    0xdd,
                                    0x5d,
                                    0xaa,
                                    0x9a,
                                    0xaa,
                                    0xaa,
                                    0x4a,
                                    0x1,
                                  };
const rohc_U8bit_t rohc_p1_r_bothrand1_or_both6[9] = {
                                    0x40,
                                    0x44,
                                    0x44,
                                    0x0,
                                    0x88,
                                    0x82,
                                    0x20,
                                    0x8,
                                    0x1,
                                  };
const rohc_U8bit_t rohc_p1_r_bothrand1_or_both6_else[9] = {
                                    0x9e,
                                    0xd9,
                                    0x19,
                                    0xf7,
                                    0x93,
                                    0x28,
                                    0x8a,
                                    0x42,
                                    0x0,
                                  };
const rohc_U8bit_t rohc_p1_r_crc_0[9] = {
                                    0x2a,
                                    0x0,
                                    0x0,
                                    0x55,
                                    0x65,
                                    0x15,
                                    0x0,
                                    0x0,
                                    0x0,
                                  };
const rohc_U8bit_t rohc_p1_r_crc_7[9] = {
                                    0xd4,
                                    0xdd,
                                    0x5d,
                                    0xaa,
                                    0x9a,
                                    0xaa,
                                    0xaa,
                                    0x4a,
                                    0x1,
                                  };
const rohc_U8bit_t rohc_p1_r_ext_3[9] = {
                                    0x80,
                                    0xdd,
                                    0x5d,
                                    0x0,
                                    0x0,
                                    0xa8,
                                    0xaa,
                                    0x4a,
                                    0x1,
                                  };
const rohc_U8bit_t rohc_p1_r_mt_pending[9] = {
                                    0xd4,
                                    0xdd,
                                    0x5d,
                                    0xaa,
                                    0x9a,
                                    0xaa,
                                    0xaa,
                                    0x4a,
                                    0x1,
                                  };
const rohc_U8bit_t rohc_p1_uo_fo_pkt_typ[7] = {
                                    0xe,
                                    0xff,
                                    0xff,
                                    0xff,
                                    0x87,
                                    0xff,
                                    0x3,
                                  };
const rohc_U8bit_t rohc_p1_uo_bothrand1_or_both6[7] = {
                                    0x8,
                                    0x24,
                                    0x49,
                                    0x80,
                                    0x6,
                                    0x92,
                                    0x2,
                                  };
const rohc_U8bit_t rohc_p1_uo_bothrand1_or_both6_else[7] = {
                                    0x6,
                                    0xdb,
                                    0xb6,
                                    0x7f,
                                    0x81,
                                    0x6d,
                                    0x1,
                                  };
const rohc_U8bit_t rohc_p1_uo_ext_3[7] = {
                                    0x0,
                                    0xf8,
                                    0x7f,
                                    0x0,
                                    0x80,
                                    0xff,
                                    0x3,
                                  };
const rohc_U8bit_t rohc_p1_uo_crc_7[7] = {
                                    0x0,
                                    0xff,
                                    0x7f,
                                    0xfc,
                                    0x87,
                                    0xff,
                                    0x3,
                                  };
const rohc_U8bit_t rohc_p1_uo_crc_3[7] = {
                                    0xf,
                                    0x0,
                                    0x80,
                                    0x3,
                                    0x0,
                                    0x0,
                                    0x0,
                                  };
const rohc_U8bit_t rohc_p1_uo_mt_pending[7] = {
                                    0x0,
                                    0xff,
                                    0x7f,
                                    0xfc,
                                    0x87,
                                    0xff,
                                    0x3,
                                  };




/*****************************************************************************
**      FUNCTION:
**              rohc_init_p1_uo_table
******************************************************************************
**
**      DESCRIPTION:
**              This is funtion initializes the rohc_p1_pkt_typ_info_uo table 
**              of UO mode packets for IP/UDP/RTP profile. 
**      ARGUMENTS:
**      RETURN VALUE:
**      NOTES:
**               
*****************************************************************************/
                        

rohc_void_t rohc_init_p1_uo_table ROHC_NO_PARAMS()
{

    ROHC_ENTER_FN("rohc_init_p1_uo_table");
    /* Initialize UO packet info list */
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_0].sn_bits      = 4;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_0].ts_bits      = 0;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_0].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_0].pbm_pkt_typ =  
                                           ROHC_UO0P1_PKTTYPE ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID].sn_bits      = 4;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID].ts_bits      = 0;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID].ip_id_bits   = 5;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID].pbm_pkt_typ =  
                                            ROHC_UO1IDP1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_TS].sn_bits      = 4;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_TS].ts_bits      = 5;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_TS].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_TS].pbm_pkt_typ =  
                                            ROHC_UO1TSP1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1].sn_bits      = 4;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1].ts_bits      = 6;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1].pbm_pkt_typ =  
                                            ROHC_UO1P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_A1].sn_bits      = 4;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_A1].ts_bits      = 7;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_A1].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_A1].pbm_pkt_typ =  
                                            ROHC_UO1IDE3P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_A2].sn_bits      = 4;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_A2].ts_bits      = 14;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_A2].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_A2].pbm_pkt_typ =  
                                            ROHC_UO1IDE3P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_A3].sn_bits      = 4;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_A3].ts_bits      = 21;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_A3].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_A3].pbm_pkt_typ =  
                                            ROHC_UO1IDE3P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_A4].sn_bits      = 4;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_A4].ts_bits      = 29;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_A4].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_A4].pbm_pkt_typ =  
                                            ROHC_UO1IDE3P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID].sn_bits      = 6;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID].ts_bits      = 0;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID].ip_id_bits   = 5;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID].pbm_pkt_typ =  
                                            ROHC_UOR2IDP1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS].sn_bits      = 6;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS].ts_bits      = 5;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS].pbm_pkt_typ =  
                                            ROHC_UOR2TSP1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2].sn_bits      = 6;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2].ts_bits      = 6;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2].pbm_pkt_typ =  
                                            ROHC_UOR2P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_A1].sn_bits      = 6;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_A1].ts_bits      = 7;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_A1].ip_id_bits   = 21;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_A1].pbm_pkt_typ =  
                                            ROHC_UOR2IDE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_A1].sn_bits      = 6;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_A1].ts_bits      = 12;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_A1].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_A1].pbm_pkt_typ =  
                                            ROHC_UOR2TSE3P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_A1].sn_bits      = 6;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_A1].ts_bits      = 13;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_A1].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_A1].pbm_pkt_typ =  
                                            ROHC_UOR2E3P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_A2].sn_bits      = 6;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_A2].ts_bits      = 14;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_A2].ip_id_bits   = 21;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_A2].pbm_pkt_typ =  
                                            ROHC_UOR2IDE3P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_A2].sn_bits      = 6;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_A2].ts_bits      = 19;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_A2].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_A2].pbm_pkt_typ =  
                                            ROHC_UOR2TSE3P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_A2].sn_bits      = 6;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_A2].ts_bits      = 20;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_A2].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_A2].pbm_pkt_typ =  
                                            ROHC_UOR2E3P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_A3].sn_bits      = 6;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_A3].ts_bits      = 21;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_A3].ip_id_bits   = 21;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_A3].pbm_pkt_typ =  
                                            ROHC_UOR2IDE3P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_A3].sn_bits      = 6;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_A3].ts_bits      = 26;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_A3].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_A3].pbm_pkt_typ =  
                                            ROHC_UOR2TSE3P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_A3].sn_bits      = 6;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_A3].ts_bits      = 27;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_A3].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_A3].pbm_pkt_typ =  
                                            ROHC_UOR2E3P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_A4].sn_bits      = 6;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_A4].ts_bits      = 29;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_A4].ip_id_bits   = 21;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_A4].pbm_pkt_typ =  
                                            ROHC_UOR2IDE3P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_A4].sn_bits      = 6;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_A4].ts_bits      = 34;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_A4].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_A4].pbm_pkt_typ =  
                                            ROHC_UOR2TSE3P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_A4].sn_bits      = 6;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_A4].ts_bits      = 35;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_A4].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_A4].pbm_pkt_typ =  
                                            ROHC_UOR2E3P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_0].sn_bits      = 7;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_0].ts_bits      = 0;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_0].ip_id_bits   = 8;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_0].pbm_pkt_typ =  
                                            ROHC_UO1IDE0P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_1].sn_bits      = 7;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_1].ts_bits      = 8;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_1].ip_id_bits   = 8;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_1].pbm_pkt_typ =  
                                            ROHC_UO1IDE1P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_2].sn_bits      = 7;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_2].ts_bits      = 8;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_2].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_2].pbm_pkt_typ =  
                                            ROHC_UO1IDE2P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_0].sn_bits      = 9;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_0].ts_bits      = 0;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_0].ip_id_bits   = 8;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_0].pbm_pkt_typ =  
                                            ROHC_UOR2IDE0P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_0].sn_bits      = 9;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_0].ts_bits      = 8;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_0].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_0].pbm_pkt_typ =  
                                            ROHC_UOR2TSE0P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_1].sn_bits      = 9;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_1].ts_bits      = 8;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_1].ip_id_bits   = 8;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_1].pbm_pkt_typ =  
                                            ROHC_UOR2TSE1P1_PKTTYPE ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_1].sn_bits      = 9;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_1].ts_bits      = 8;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_1].ip_id_bits   = 8;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_1].pbm_pkt_typ =  
                                            ROHC_UOR2IDE1P1_PKTTYPE ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_2].sn_bits      = 9;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_2].ts_bits      = 8;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_2].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_2].pbm_pkt_typ =  
                                            ROHC_UOR2IDE2P1_PKTTYPE ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_0].sn_bits      = 9;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_0].ts_bits      = 9;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_0].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_0].pbm_pkt_typ =  
                                            ROHC_UOR2E0P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_2].sn_bits      = 9;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_2].ts_bits      = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_2].ip_id_bits   = 8;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_2].pbm_pkt_typ =  
                                            ROHC_UOR2TSE2P1_PKTTYPE ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_1].sn_bits      = 9;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_1].ts_bits      = 17;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_1].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_1].pbm_pkt_typ =  
                                            ROHC_UOR2E1P1_PKTTYPE ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_2].sn_bits      = 9;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_2].ts_bits      = 25;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_2].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_2].pbm_pkt_typ =  
                                            ROHC_UOR2E2P1_PKTTYPE;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_B1].sn_bits      = 12;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_B1].ts_bits      = 7;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_B1].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_B1].pbm_pkt_typ =  
                                            ROHC_UO1IDE3P1_PKTTYPE ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_B2].sn_bits      = 12;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_B2].ts_bits      = 14;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_B2].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_B2].pbm_pkt_typ =  
                                            ROHC_UO1IDE3P1_PKTTYPE ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_B3].sn_bits      = 12;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_B3].ts_bits      = 21;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_B3].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_B3].pbm_pkt_typ =  
                                            ROHC_UO1IDE3P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_B4].sn_bits      = 12;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_B4].ts_bits      = 29;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_B4].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UO_1_ID_EXT_3_B4].pbm_pkt_typ =  
                                            ROHC_UO1IDE3P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_B1].sn_bits      = 14;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_B1].ts_bits      = 7;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_B1].ip_id_bits   = 21;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_B1].pbm_pkt_typ =  
                                            ROHC_UOR2IDE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_B1].sn_bits      = 14;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_B1].ts_bits      = 12;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_B1].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_B1].pbm_pkt_typ =  
                                            ROHC_UOR2TSE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_B1].sn_bits      = 14;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_B1].ts_bits      = 13;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_B1].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_B1].pbm_pkt_typ =  
                                            ROHC_UOR2E3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_B2].sn_bits     = 14;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_B2].ts_bits     = 14;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_B2].ip_id_bits  = 21;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_B2].pbm_pkt_typ =  
                                            ROHC_UOR2IDE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_B2].sn_bits     = 14;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_B2].ts_bits     = 19;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_B2].ip_id_bits  = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_B2].pbm_pkt_typ=  
                                            ROHC_UOR2TSE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_B2].sn_bits      = 14;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_B2].ts_bits      = 20;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_B2].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_B2].pbm_pkt_typ =  
                                            ROHC_UOR2E3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_B3].sn_bits     = 14;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_B3].ts_bits     = 21;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_B3].ip_id_bits  = 21;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_ID_EXT_3_B3].pbm_pkt_typ=  
                                            ROHC_UOR2IDE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_B3].sn_bits     = 14;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_B3].ts_bits     = 26;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_B3].ip_id_bits  = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_B3].pbm_pkt_typ=  
                                            ROHC_UOR2TSE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_B3].sn_bits      = 14;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_B3].ts_bits      = 27;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_B3].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_B3].pbm_pkt_typ =  
                                            ROHC_UOR2E3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_B4].sn_bits     = 14;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_B4].ts_bits     = 34;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_B4].ip_id_bits  = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_TS_EXT_3_B4].pbm_pkt_typ=  
                                            ROHC_UOR2TSE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_B4].sn_bits      = 14;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_B4].ts_bits      = 35;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_B4].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_uo[ROHC_UOP1_UOR_2_EXT_3_B4].pbm_pkt_typ =  
                                            ROHC_UOR2E3P1_PKTTYPE;
    ROHC_EXIT_FN("rohc_init_p1_uo_table");                                           
}


/*****************************************************************************
**      FUNCTION:
**              rohc_init_p1_r_table
******************************************************************************
**
**      DESCRIPTION:
**              This is funtion initializes the rohc_p1_pkt_typ_info_r table 
**              of R mode packets for IP/UDP/RTP profile. 
**      ARGUMENTS:
**      RETURN VALUE:
**      NOTES:
**               
*****************************************************************************/
rohc_void_t rohc_init_p1_r_table ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_init_p1_r_table");
    /* Defines for R mode packet types for IP/UDP/RTP profile */
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_0].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_0].ts_bits      = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_0].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_0].pbm_pkt_typ =  
                                            ROHC_R0P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID].ts_bits      = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID].ip_id_bits   = 5;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID].pbm_pkt_typ =  
                                            ROHC_R1IDP1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID].ts_bits      = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID].ip_id_bits   = 5;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID].pbm_pkt_typ =  
                                            ROHC_UOR2IDP1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS].ts_bits      = 5;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS].pbm_pkt_typ =  
                                            ROHC_R1TSP1_PKTTYPE  ; 
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS].ts_bits      = 5;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS].pbm_pkt_typ =  
                                            ROHC_UOR2TSP1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1].ts_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1].pbm_pkt_typ =  
                                            ROHC_R1P1_PKTTYPE  ;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2].ts_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2].pbm_pkt_typ =  
                                            ROHC_UOR2P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_A1].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_A1].ts_bits      = 7;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_A1].ip_id_bits   = 21;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_A1].pbm_pkt_typ =  
                                            ROHC_UOR2IDE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_A1].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_A1].ts_bits      = 12;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_A1].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_A1].pbm_pkt_typ =  
                                            ROHC_UOR2TSE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_A1].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_A1].ts_bits      = 13;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_A1].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_A1].pbm_pkt_typ =  
                                            ROHC_R1E3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_A1].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_A1].ts_bits      = 13;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_A1].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_A1].pbm_pkt_typ =  
                                            ROHC_UOR2E3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_A2].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_A2].ts_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_A2].ip_id_bits   = 21;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_A2].pbm_pkt_typ =  
                                            ROHC_UOR2IDE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_A2].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_A2].ts_bits      = 19;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_A2].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_A2].pbm_pkt_typ =  
                                            ROHC_UOR2TSE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_A2].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_A2].ts_bits      = 20;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_A2].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_A2].pbm_pkt_typ =  
                                            ROHC_R1E3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_A2].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_A2].ts_bits      = 20;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_A2].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_A2].pbm_pkt_typ =  
                                            ROHC_UOR2E3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_A3].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_A3].ts_bits      = 21;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_A3].ip_id_bits   = 21;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_A3].pbm_pkt_typ =  
                                            ROHC_UOR2IDE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_A3].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_A3].ts_bits      = 26;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_A3].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_A3].pbm_pkt_typ =  
                                            ROHC_UOR2TSE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_A3].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_A3].ts_bits      = 27;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_A3].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_A3].pbm_pkt_typ =  
                                            ROHC_R1E3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_A3].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_A3].ts_bits      = 27;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_A3].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_A3].pbm_pkt_typ =  
                                            ROHC_UOR2E3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_A4].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_A4].ts_bits      = 29;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_A4].ip_id_bits   = 21;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_A4].pbm_pkt_typ =  
                                            ROHC_UOR2IDE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_A4].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_A4].ts_bits      = 34;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_A4].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_A4].pbm_pkt_typ =  
                                            ROHC_UOR2TSE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_A4].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_A4].ts_bits      = 35;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_A4].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_A4].pbm_pkt_typ =  
                                            ROHC_R1E3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_A4].sn_bits      = 6;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_A4].ts_bits      = 35;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_A4].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_A4].pbm_pkt_typ =  
                                            ROHC_UOR2E3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_0_CRC].sn_bits      = 7;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_0_CRC].ts_bits      = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_0_CRC].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_0_CRC].pbm_pkt_typ =  
                                            ROHC_R0CRCP1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_0].sn_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_0].ts_bits      = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_0].ip_id_bits   = 8;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_0].pbm_pkt_typ =  
                                            ROHC_R1IDE0P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_0].sn_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_0].ts_bits      = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_0].ip_id_bits   = 8;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_0].pbm_pkt_typ =  
                                            ROHC_UOR2IDE0P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_0].sn_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_0].ts_bits      = 8;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_0].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_0].pbm_pkt_typ =  
                                            ROHC_R1TSE0P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_0].sn_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_0].ts_bits      = 8;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_0].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_0].pbm_pkt_typ =  
                                            ROHC_UOR2TSE0P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_1].sn_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_1].ts_bits      = 8;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_1].ip_id_bits   = 8;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_1].pbm_pkt_typ =  
                                            ROHC_R1TSE1P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_1].sn_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_1].ts_bits      = 8;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_1].ip_id_bits   = 8;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_1].pbm_pkt_typ =  
                                            ROHC_UOR2TSE1P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_1].sn_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_1].ts_bits      = 8;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_1].ip_id_bits   = 8;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_1].pbm_pkt_typ =  
                                            ROHC_R1IDE1P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_1].sn_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_1].ts_bits      = 8;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_1].ip_id_bits   = 8;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_1].pbm_pkt_typ =  
                                            ROHC_UOR2IDE1P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_2].sn_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_2].ts_bits      = 8;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_2].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_2].pbm_pkt_typ =  
                                            ROHC_R1IDE2P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_2].sn_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_2].ts_bits      = 8;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_2].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_2].pbm_pkt_typ =  
                                            ROHC_UOR2IDE2P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_0].sn_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_0].ts_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_0].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_0].pbm_pkt_typ =  
                                            ROHC_R1E0P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_0].sn_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_0].ts_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_0].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_0].pbm_pkt_typ =  
                                            ROHC_UOR2E0P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_2].sn_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_2].ts_bits      = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_2].ip_id_bits   = 8;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_2].pbm_pkt_typ =  
                                            ROHC_UOR2TSE2P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_1].sn_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_1].ts_bits      = 17;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_1].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_1].pbm_pkt_typ =  
                                            ROHC_R1E1P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_2].sn_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_2].ts_bits      = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_2].ip_id_bits   = 8;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_2].pbm_pkt_typ =  
                                            ROHC_R1TSE2P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_1].sn_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_1].ts_bits      = 17;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_1].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_1].pbm_pkt_typ =  
                                            ROHC_UOR2E1P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_2].sn_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_2].ts_bits      = 25;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_2].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_2].pbm_pkt_typ =  
                                            ROHC_R1E2P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_2].sn_bits      = 9;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_2].ts_bits      = 25;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_2].ip_id_bits   = 0;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_2].pbm_pkt_typ =  
                                            ROHC_UOR2E2P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_3_A1].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_3_A1].ts_bits      = 7;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_3_A1].ip_id_bits   = 21;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_3_A1].pbm_pkt_typ =  
                                            ROHC_R1IDE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_B1].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_B1].ts_bits      = 7;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_B1].ip_id_bits   = 21;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_B1].pbm_pkt_typ =  
                                            ROHC_UOR2IDE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_3_A1].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_3_A1].ts_bits      = 12;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_3_A1].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_3_A1].pbm_pkt_typ =  
                                            ROHC_R1TSE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_B1].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_B1].ts_bits      = 12;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_B1].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_B1].pbm_pkt_typ =  
                                            ROHC_UOR2TSE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_B1].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_B1].ts_bits      = 13;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_B1].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_B1].pbm_pkt_typ =  
                                            ROHC_R1E3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_B1].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_B1].ts_bits      = 13;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_B1].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_B1].pbm_pkt_typ =  
                                            ROHC_UOR2E3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_3_A2].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_3_A2].ts_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_3_A2].ip_id_bits   = 21;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_3_A2].pbm_pkt_typ =  
                                            ROHC_R1IDE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_B2].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_B2].ts_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_B2].ip_id_bits   = 21;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_B2].pbm_pkt_typ =  
                                            ROHC_UOR2IDE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_3_A2].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_3_A2].ts_bits      = 19;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_3_A2].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_3_A2].pbm_pkt_typ =  
                                            ROHC_R1TSE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_B2].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_B2].ts_bits      = 19;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_B2].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_B2].pbm_pkt_typ =  
                                            ROHC_UOR2TSE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_B2].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_B2].ts_bits      = 20;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_B2].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_B2].pbm_pkt_typ =  
                                            ROHC_R1E3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_B2].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_B2].ts_bits      = 20;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_B2].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_B2].pbm_pkt_typ =  
                                            ROHC_UOR2E3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_3_A3].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_3_A3].ts_bits      = 21;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_3_A3].ip_id_bits   = 21;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_3_A3].pbm_pkt_typ =  
                                            ROHC_R1IDE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_B3].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_B3].ts_bits      = 21;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_B3].ip_id_bits   = 21;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_ID_EXT_3_B3].pbm_pkt_typ =  
                                            ROHC_UOR2IDE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_3_A3].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_3_A3].ts_bits      = 26;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_3_A3].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_3_A3].pbm_pkt_typ =  
                                            ROHC_R1TSE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_B3].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_B3].ts_bits      = 26;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_B3].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_B3].pbm_pkt_typ =  
                                            ROHC_UOR2TSE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_B3].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_B3].ts_bits      = 27;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_B3].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_B3].pbm_pkt_typ =  
                                            ROHC_R1E3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_B3].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_B3].ts_bits      = 27;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_B3].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_B3].pbm_pkt_typ =  
                                            ROHC_UOR2E3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_3_A4].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_3_A4].ts_bits      = 29;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_3_A4].ip_id_bits   = 21;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_ID_EXT_3_A4].pbm_pkt_typ =  
                                            ROHC_R1IDE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_3_A4].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_3_A4].ts_bits      = 34;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_3_A4].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_TS_EXT_3_A4].pbm_pkt_typ =  
                                            ROHC_R1TSE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_B4].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_B4].ts_bits      = 34;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_B4].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_TS_EXT_3_B4].pbm_pkt_typ =  
                                            ROHC_UOR2TSE3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_B4].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_B4].ts_bits      = 35;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_B4].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1_EXT_3_B4].pbm_pkt_typ =  
                                            ROHC_R1E3P1_PKTTYPE;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_B4].sn_bits      = 14;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_B4].ts_bits      = 35;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_B4].ip_id_bits   = 16;
    rohc_p1_pkt_typ_info_r[ROHC_RP1_UOR_2_EXT_3_B4].pbm_pkt_typ =  
                                            ROHC_UOR2E3P1_PKTTYPE;
    
    ROHC_EXIT_FN("rohc_init_p1_r_table");
}








