/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_cprm_init.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains #defines for the packet type used
 *                     in PRM
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_cprm_init.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/28 05:46:18  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_CPRM_INIT_H_
#define _ROHC_CPRM_INIT_H_

/* define for UO packets types for IP/UDP/RTP profile */
#define ROHC_P1_UO_BASE                             U0
#define ROHC_P1_UO_0                                ROHC_P1_UO_BASE     
#define ROHC_P1_UO_1_ID                             ROHC_P1_UO_BASE + 1 
#define ROHC_P1_UO_1_TS                             ROHC_P1_UO_BASE + 2 
#define ROHC_P1_UO_1                                ROHC_P1_UO_BASE + 3 
#define ROHC_P1_UO_1_ID_EXT_3_A1                    ROHC_P1_UO_BASE + 4 
#define ROHC_P1_UO_1_ID_EXT_3_A2                    ROHC_P1_UO_BASE + 5 
#define ROHC_P1_UO_1_ID_EXT_3_A3                    ROHC_P1_UO_BASE + 6 
#define ROHC_P1_UO_1_ID_EXT_3_A4                    ROHC_P1_UO_BASE + 7 
#define ROHC_P1_UOR_2_ID                            ROHC_P1_UO_BASE + 8 
#define ROHC_P1_UOR_2_TS                            ROHC_P1_UO_BASE + 9 
#define ROHC_P1_UOR_2                               ROHC_P1_UO_BASE + 10
#define ROHC_P1_UOR_2_ID_1_EXT_3_A1                 ROHC_P1_UO_BASE + 11
#define ROHC_P1_UOR_2_ts_1_EXT_3_A1                 ROHC_P1_UO_BASE + 12
#define ROHC_P1_UOR_2_1_EXT_3_A1                    ROHC_P1_UO_BASE + 13
#define ROHC_P1_UOR_2_ID_1_EXT_3_A2                 ROHC_P1_UO_BASE + 14
#define ROHC_P1_UOR_2_TS_1_EXT_3_A2                 ROHC_P1_UO_BASE + 15
#define ROHC_P1_UOR_2_1_EXT_3_A2                    ROHC_P1_UO_BASE + 16
#define ROHC_P1_UOR_2_ID_1_EXT_3_A3                 ROHC_P1_UO_BASE + 17
#define ROHC_P1_UOR_2_TS_1_EXT_3_A3                 ROHC_P1_UO_BASE + 18
#define ROHC_P1_UOR_2_1_EXT_3_A3                    ROHC_P1_UO_BASE + 19
#define ROHC_P1_UOR_2_ID_1_EXT_3_A4                 ROHC_P1_UO_BASE + 20
#define ROHC_P1_UOR_2_TS_1_EXT_3_A4                 ROHC_P1_UO_BASE + 21
#define ROHC_P1_UOR_2_1_EXT_3_A4                    ROHC_P1_UO_BASE + 22
#define ROHC_P1_UO_1_ID_EXT_0                       ROHC_P1_UO_BASE + 23
#define ROHC_P1_UO_1_ID_EXT_1                       ROHC_P1_UO_BASE + 24
#define ROHC_P1_UO_1_ID_EXT_2                       ROHC_P1_UO_BASE + 25
#define ROHC_P1_UOR_2_ID_EXT_0                      ROHC_P1_UO_BASE + 26
#define ROHC_P1_UOR_2_TS_EXT_0                      ROHC_P1_UO_BASE + 27
#define ROHC_P1_UOR_2_TS_EXT_1                      ROHC_P1_UO_BASE + 28
#define ROHC_P1_UOR_2_ID_EXT_1                      ROHC_P1_UO_BASE + 29
#define ROHC_P1_UOR_2_ID_EXT_2                      ROHC_P1_UO_BASE + 30
#define ROHC_P1_UOR_2_EXT_0                         ROHC_P1_UO_BASE + 31
#define ROHC_P1_UOR_2_TS_EXT_2                      ROHC_P1_UO_BASE + 32
#define ROHC_P1_UOR_2_EXT_1                         ROHC_P1_UO_BASE + 33
#define ROHC_P1_UOR_2_EXT_2                         ROHC_P1_UO_BASE + 34
#define ROHC_P1_UO_1_ID_EXT_3_B1                    ROHC_P1_UO_BASE + 35
#define ROHC_P1_UO_1_ID_EXT_3_B2                    ROHC_P1_UO_BASE + 36
#define ROHC_P1_UO_1_ID_EXT_3_B3                    ROHC_P1_UO_BASE + 37
#define ROHC_P1_UO_1_ID_EXT_3_B4                    ROHC_P1_UO_BASE + 38
#define ROHC_P1_UOR_2_ID_EXT_3_B1                   ROHC_P1_UO_BASE + 39
#define ROHC_P1_UOR_2_TS_EXT_3_B1                   ROHC_P1_UO_BASE + 40
#define ROHC_P1_UOR_2_EXT_3_B1                      ROHC_P1_UO_BASE + 41
#define ROHC_P1_UOR_2_ID_EXT_3_B2                   ROHC_P1_UO_BASE + 42
#define ROHC_P1_UOR_2_TS_EXT_3_B2                   ROHC_P1_UO_BASE + 43
#define ROHC_P1_UOR_2_EXT_3_B2                      ROHC_P1_UO_BASE + 44
#define ROHC_P1_UOR_2_ID_EXT_3_B3                   ROHC_P1_UO_BASE + 45
#define ROHC_P1_UOR_2_TS_EXT_3_B3                   ROHC_P1_UO_BASE + 46
#define ROHC_P1_UOR_2_EXT_3_B3                      ROHC_P1_UO_BASE + 47
#define ROHC_P1_UOR_2_TS_EXT_3_B4                   ROHC_P1_UO_BASE + 48
#define ROHC_P1_UOR_2_EXT_3_B4                      ROHC_P1_UO_BASE + 49

#define ROHC_P1_UO_MAX_PACKETS                      ROHC_P1_UO_BASE + 50

/* Defines for R mode packet types for IP/UDP/RTP profile */
                  
#define ROHC_P1_UO_BASE                     U0                   
                                                                 
#define ROHC_P1_R_0                         ROHC_P1_UO_BASE      
#define ROHC_P1_R_1_ID                      ROHC_P1_UO_BASE + 1  
#define ROHC_P1_UOR_2_ID                    ROHC_P1_UO_BASE + 2  
#define ROHC_P1_R_1_TS                      ROHC_P1_UO_BASE + 3  
#define ROHC_P1_UOR_2_TS                    ROHC_P1_UO_BASE + 4  
#define ROHC_P1_R_1                         ROHC_P1_UO_BASE + 5  
#define ROHC_P1_UOR_2                       ROHC_P1_UO_BASE + 6  
#define ROHC_P1_UOR_2_ID_1_EXT_3_A1         ROHC_P1_UO_BASE + 7  
#define ROHC_P1_UOR_2_ts_1_EXT_3_A1         ROHC_P1_UO_BASE + 8  
#define ROHC_P1_R_1_EXT_3_A_1               ROHC_P1_UO_BASE + 9  
#define ROHC_P1_UOR_2_1_EXT_3_A1            ROHC_P1_UO_BASE + 10 
#define ROHC_P1_UOR_2_ID_1_EXT_3_A2         ROHC_P1_UO_BASE + 11 
#define ROHC_P1_UOR_2_TS_1_EXT_3_A2         ROHC_P1_UO_BASE + 12 
#define ROHC_P1_R_1_EXT_3_A_2               ROHC_P1_UO_BASE + 13 
#define ROHC_P1_UOR_2_1_EXT_3_A2            ROHC_P1_UO_BASE + 14 
#define ROHC_P1_UOR_2_ID_1_EXT_3_A3         ROHC_P1_UO_BASE + 15 
#define ROHC_P1_UOR_2_TS_1_EXT_3_A3         ROHC_P1_UO_BASE + 16 
#define ROHC_P1_R_1_EXT_3_A_3               ROHC_P1_UO_BASE + 17 
#define ROHC_P1_UOR_2_1_EXT_3_A3            ROHC_P1_UO_BASE + 18 
#define ROHC_P1_UOR_2_ID_1_EXT_3_A4         ROHC_P1_UO_BASE + 19 
#define ROHC_P1_UOR_2_TS_1_EXT_3_A4         ROHC_P1_UO_BASE + 20 
#define ROHC_P1_R_1_EXT_3_A_4               ROHC_P1_UO_BASE + 21 
#define ROHC_P1_UOR_2_1_EXT_3_A4            ROHC_P1_UO_BASE + 22 
#define ROHC_P1_R_0_CRC                     ROHC_P1_UO_BASE + 23 
#define ROHC_P1_R_1_ID_EXT_0                ROHC_P1_UO_BASE + 24 
#define ROHC_P1_UOR_2_ID_EXT_0              ROHC_P1_UO_BASE + 25 
#define ROHC_P1_R_1_TS_EXT_0                ROHC_P1_UO_BASE + 26 
#define ROHC_P1_UOR_2_TS_EXT_0              ROHC_P1_UO_BASE + 27 
#define ROHC_P1_R_1_TS_EXT_1                ROHC_P1_UO_BASE + 28 
#define ROHC_P1_UOR_2_TS_EXT_1              ROHC_P1_UO_BASE + 29 
#define ROHC_P1_R_1_ID_EXT_1                ROHC_P1_UO_BASE + 30 
#define ROHC_P1_UOR_2_ID_EXT_1              ROHC_P1_UO_BASE + 31 
#define ROHC_P1_R_1_ID_EXT_2                ROHC_P1_UO_BASE + 32 
#define ROHC_P1_UOR_2_ID_EXT_2              ROHC_P1_UO_BASE + 33 
#define ROHC_P1_R_1_EXT_0                   ROHC_P1_UO_BASE + 34 
#define ROHC_P1_UOR_2_EXT_0                 ROHC_P1_UO_BASE + 35 
#define ROHC_P1_UOR_2_TS_EXT_2              ROHC_P1_UO_BASE + 36 
#define ROHC_P1_R_1_EXT_1                   ROHC_P1_UO_BASE + 37 
#define ROHC_P1_R_1_TS_EXT_2                ROHC_P1_UO_BASE + 38 
#define ROHC_P1_UOR_2_EXT_1                 ROHC_P1_UO_BASE + 39 
#define ROHC_P1_R_1_EXT_2                   ROHC_P1_UO_BASE + 40 
#define ROHC_P1_UOR_2_EXT_2                 ROHC_P1_UO_BASE + 41 
#define ROHC_P1_R_1_ID_EXT_3_A1             ROHC_P1_UO_BASE + 42 
#define ROHC_P1_UOR_2_ID_EXT_3_B1           ROHC_P1_UO_BASE + 43 
#define ROHC_P1_R_1_TS_EXT_3_A1             ROHC_P1_UO_BASE + 44 
#define ROHC_P1_UOR_2_TS_EXT_3_B1           ROHC_P1_UO_BASE + 45 
#define ROHC_P1_R_1_EXT_3_B1                ROHC_P1_UO_BASE + 46 
#define ROHC_P1_UOR_2_EXT_3_B1              ROHC_P1_UO_BASE + 47 
#define ROHC_P1_R_1_ID_EXT_3_A2             ROHC_P1_UO_BASE + 48 
#define ROHC_P1_UOR_2_ID_EXT_3_B2           ROHC_P1_UO_BASE + 49 
#define ROHC_P1_R_1_TS_EXT_3_A2             ROHC_P1_UO_BASE + 50 
#define ROHC_P1_UOR_2_TS_EXT_3_B2           ROHC_P1_UO_BASE + 51 
#define ROHC_P1_R_1_EXT_3_B2                ROHC_P1_UO_BASE + 52 
#define ROHC_P1_UOR_2_EXT_3_B2              ROHC_P1_UO_BASE + 53 
#define ROHC_P1_R_1_ID_EXT_3_A3             ROHC_P1_UO_BASE + 54 
#define ROHC_P1_UOR_2_ID_EXT_3_B3           ROHC_P1_UO_BASE + 55 
#define ROHC_P1_R_1_TS_EXT_3_A3             ROHC_P1_UO_BASE + 56 
#define ROHC_P1_UOR_2_TS_EXT_3_B3           ROHC_P1_UO_BASE + 57 
#define ROHC_P1_R_1_EXT_3_B3                ROHC_P1_UO_BASE + 58 
#define ROHC_P1_UOR_2_EXT_3_B3              ROHC_P1_UO_BASE + 59 
#define ROHC_P1_R_1_ID_EXT_3_A4             ROHC_P1_UO_BASE + 60 
#define ROHC_P1_R_1_TS_EXT_3_A4             ROHC_P1_UO_BASE + 61 
#define ROHC_P1_UOR_2_TS_EXT_3_B4           ROHC_P1_UO_BASE + 62 
#define ROHC_P1_R_1_EXT_3_B4                ROHC_P1_UO_BASE + 63 
#define ROHC_P1_UOR_2_EXT_3_B4              ROHC_P1_UO_BASE + 64 

#define ROHC_P1_R_MAX_PACKETS               ROHC_P1_UO_BASE + 65



/* for computing delta bit map */
#define ROHC_CPRM_CH_TS                 Ox00000001
#define ROHC_CPRM_CH_IP_ID              Ox00000002
#define ROHC_CPRM_CH_M                  Ox00000004
#define ROHC_CPRM_CH_RTP_PAD            Ox00000008
#define ROHC_CPRM_CH_SCRC               Ox00000010
#define ROHC_CPRM_CH_TOS                Ox00000020
#define ROHC_CPRM_CH_TTL                Ox00000040
#define ROHC_CPRM_CH_DF                 Ox00000080
#define ROHC_CPRM_CH_TC                 Ox00000100
#define ROHC_CPRM_CH_HL                 Ox00000200
#define ROHC_CPRM_CH_CHK                Ox00000400
#define ROHC_CPRM_CH_IN_EXT             Ox00000800
#define ROHC_CPRM_CH_OUT_EXT            Ox00001000
                                  

#endif /* _ROHC_CPRM_EXT_IF_H_ */
