/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_ccpm_event.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_ccpm_event.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/28 04:57:01  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef ROHC_C_ET
#define ROHC_C_ET 

/* U-mode compressor event list */
#define ROHC_C_U_START				1
#define ROHC_C_U_IR_ET 				(ROHC_C_U_START	+ 0)
#define ROHC_C_U_IRDYN_ET			(ROHC_C_U_START	+ 1)
#define ROHC_C_U_UO_0_ET 			(ROHC_C_U_START	+ 2)
#define ROHC_C_U_UO_1_ET 			(ROHC_C_U_START	+ 3)
#define ROHC_C_U_UO_2_ET 			(ROHC_C_U_START	+ 4)
#define ROHC_C_U_IR_RTN_TMR_ET		(ROHC_C_U_START	+ 5)
#define ROHC_C_U_FO_RTN_TMR_ET		(ROHC_C_U_START	+ 6)
#define ROHC_C_U_ACK_U_ET			(ROHC_C_U_START	+ 7)
/* reusing event index 7 for prof6 ack event */
#define ROHC_C_ACK_ET			    (ROHC_C_U_START	+ 7)
#define ROHC_C_U_MT_U_TO_O_ET		(ROHC_C_U_START	+ 8)
#define ROHC_C_U_MT_U_TO_R_ET		(ROHC_C_U_START	+ 9)
/* reusing event index 8,9 for prof6 nack and static nack events */
#define ROHC_C_NACK_ET			    (ROHC_C_U_START	+ 8)
#define ROHC_C_STATIC_NACK_ET	    (ROHC_C_U_START	+ 9)
/* prof 6 specific */
#define ROHC_C_U_CO_IRP6CR_ET		(ROHC_C_U_START	+ 10)
#define ROHC_C_U_CO_P6_RND_ET		(ROHC_C_U_START	+ 11)
#define ROHC_C_U_CO_P6_COMMON_ET    (ROHC_C_U_START	+ 12)

#define ROHC_C_U_MAX_ET				ROHC_C_U_CO_P6_COMMON_ET

/* O-mode compressor event list */
#define ROHC_C_O_START 				1
#define ROHC_C_O_IR_ET 				(ROHC_C_O_START	+ 0) 
#define ROHC_C_O_IRDYN_ET			(ROHC_C_O_START	+ 1) 
#define ROHC_C_O_UO_0_ET 			(ROHC_C_O_START	+ 2) 
#define ROHC_C_O_UO_1_ET 			(ROHC_C_O_START	+ 3) 
#define ROHC_C_O_UO_2_ET 			(ROHC_C_O_START	+ 4) 
#define ROHC_C_O_ACK_O_ET			(ROHC_C_O_START	+ 5) 
#define ROHC_C_O_STATIC_NACK_O_ET	(ROHC_C_O_START	+ 6) 
#define ROHC_C_O_NACK_O_ET			(ROHC_C_O_START	+ 7) 
#define ROHC_C_O_MT_O_TO_U_ET		(ROHC_C_O_START	+ 8) 
#define ROHC_C_O_MT_O_TO_R_ET		(ROHC_C_O_START	+ 9)  
                                    
#define ROHC_C_O_MAX_ET				ROHC_C_O_MT_R_TO_R_ET   
                                      
/* R-mode compressor event list */  
#define ROHC_C_R_START 				1 
#define ROHC_C_R_IR_ET 				(ROHC_C_R_START	+ 0)  
#define ROHC_C_R_IRDYN_ET			(ROHC_C_R_START	+ 1)  
#define ROHC_C_R_R_0_ET 			(ROHC_C_R_START	+ 2)  
#define ROHC_C_R_R_0_CRC_ET			(ROHC_C_R_START	+ 3)  
#define ROHC_C_R_R_1_ET 			(ROHC_C_R_START	+ 4)  
#define ROHC_C_R_R_2_ET 			(ROHC_C_R_START	+ 5)  
#define ROHC_C_R_ACK_R_ET			(ROHC_C_R_START	+ 6)  
#define ROHC_C_R_STATIC_NACK_ET		(ROHC_C_R_START	+ 7)  
#define ROHC_C_R_NACK_ET			(ROHC_C_R_START	+ 8)  
#define ROHC_C_R_MT_R_TO_U_ET		(ROHC_C_R_START	+ 9)  
#define ROHC_C_R_MT_R_TO_O_ET		(ROHC_C_R_START	+ 10) 

#define ROHC_C_R_MAX_ET				ROHC_C_R_MT_R_TO_O_ET		

#endif /* ROHC_C_ET */

