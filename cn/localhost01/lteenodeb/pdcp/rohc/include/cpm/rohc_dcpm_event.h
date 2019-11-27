/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_dcpm_event.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file defines the events for decompressor side CPM.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_dcpm_event.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/28 04:57:01  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_DCPM_ET_
#define _ROHC_DCPM_ET_ 

/* U-mode Decompressor event list  */
#define ROHC_D_U_START                   1
#define ROHC_D_U_IR_ET              	(ROHC_D_U_START + 0)
#define ROHC_D_U_IRDYN_ET           	(ROHC_D_U_START + 1)
#define ROHC_D_U_UO_0_ET            	(ROHC_D_U_START + 2)
#define ROHC_D_U_UO_1_ET            	(ROHC_D_U_START + 3)
#define ROHC_D_U_UO_2_ET            	(ROHC_D_U_START + 4)
#define ROHC_D_U_ACK_U_ET           	(ROHC_D_U_START + 5)
#define ROHC_D_U_MT_U_TO_O_ET       	(ROHC_D_U_START + 6)
#define ROHC_D_U_MT_U_TO_R_ET       	(ROHC_D_U_START + 7)
#define ROHC_D_IR_IRCR_ET                 	(ROHC_D_U_START + 8)
#define ROHC_D_IR_DYN_CO_ET          	(ROHC_D_U_START + 9)
#define ROHC_D_OTHERS_ET             	(ROHC_D_U_START + 10)

#define ROHC_D_U_MAX_ET             ROHC_D_U_OTHERS_ET

/* O-mode Decompressor event list */
#define ROHC_D_O_START              	1
#define ROHC_D_O_IR_ET              	(ROHC_D_O_START + 0) 
#define ROHC_D_O_IRDYN_ET           	(ROHC_D_O_START + 1) 
#define ROHC_D_O_UO_0_ET            	(ROHC_D_O_START + 2) 
#define ROHC_D_O_UO_1_ET            	(ROHC_D_O_START + 3) 
#define ROHC_D_O_UO_2_ET            	(ROHC_D_O_START + 4) 
#define ROHC_D_O_ACK_O_ET           	(ROHC_D_O_START + 5) 
#define ROHC_D_O_STATIC_NACK_O_ET   	(ROHC_D_O_START + 6) 
#define ROHC_D_O_NACK_O_ET          	(ROHC_D_O_START + 7) 
#define ROHC_D_O_MT_O_TO_U_ET       	(ROHC_D_O_START + 8) 
#define ROHC_D_O_MT_O_TO_R_ET       	(ROHC_D_O_START + 9)  
                                  
#define ROHC_D_O_MAX_ET             ROHC_D_O_MT_R_TO_R_ET   
                                      
/* R-mode Decompressor event list */ 
#define ROHC_D_R_START              	1 
#define ROHC_D_R_IR_ET              	(ROHC_D_R_START + 0)  
#define ROHC_D_R_IRDYN_ET           	(ROHC_D_R_START + 1)  
#define ROHC_D_R_R_0_ET             	(ROHC_D_R_START + 2)  
#define ROHC_D_R_R_0_CRC_ET         	(ROHC_D_R_START + 3)  
#define ROHC_D_R_R_1_ET             	(ROHC_D_R_START + 4)  
#define ROHC_D_R_R_2_ET             	(ROHC_D_R_START + 5)  
#define ROHC_D_R_ACK_R_ET           	(ROHC_D_R_START + 6)  
#define ROHC_D_R_STATIC_NACK_ET     	(ROHC_D_R_START + 7)  
#define ROHC_D_R_NACK_ET            	(ROHC_D_R_START + 8)  
#define ROHC_D_R_MT_R_TO_U_ET          	(ROHC_D_R_START + 9)  
#define ROHC_D_R_MT_R_TO_O_ET          	(ROHC_D_R_START + 10) 

#define ROHC_D_R_MAX_ET ROHC_D_R_MT_R_TO_O_ET
#define ROHC_MAX_D_EVENT  ROHC_D_R_MAX_ET /*Max number of event in a mode */


#endif /* ROHC_C_ET */

