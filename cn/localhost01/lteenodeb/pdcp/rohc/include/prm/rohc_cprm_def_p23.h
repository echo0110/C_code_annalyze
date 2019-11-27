/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_cprm_def_p23.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains #defines for the packet type and bit
 *                     strings to resolve the packet for profile 2 and 3.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_cprm_def_p23.h,v $
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

#ifndef _ROHC_CPRM_DEF_P23_H_
#define _ROHC_CPRM_DEF_P23_H_

#include "rohc_c_typ.h"
#include "rohc_com_def.h"

#define ROHC_UOP23_UO_BASE 				0U 
#define ROHC_UOP23_UO_0 				ROHC_UOP23_UO_BASE + 0			
#define ROHC_UOP23_UOR_2 				ROHC_UOP23_UO_BASE + 1			
#define ROHC_UOP23_UO_1 				ROHC_UOP23_UO_BASE + 2          
#define ROHC_UOP23_UOR_2_EXT_0 			ROHC_UOP23_UO_BASE + 3   
#define ROHC_UOP23_UOR_2_EXT_1 			ROHC_UOP23_UO_BASE + 4  
#define ROHC_UOP23_UOR_2_EXT_2 			ROHC_UOP23_UO_BASE + 5   
#define ROHC_UOP23_UOR_2_EXT_3_B1 		ROHC_UOP23_UO_BASE + 6
#define ROHC_UOP23_UOR_2_EXT_3_B2     	ROHC_UOP23_UO_BASE + 7

#define ROHC_P23_UO_MAX_PACKETS 		ROHC_UOP23_UOR_2_EXT_3_B2 + 8

#define ROHC_RP23_R_BASE 				0U 	                          
#define ROHC_RP23_R_0 					ROHC_RP23_R_BASE + 0				
#define ROHC_RP23_R_0_CRC 				ROHC_RP23_R_BASE + 1			
#define ROHC_RP23_R_1 					ROHC_RP23_R_BASE + 2           
#define ROHC_RP23_R_1_EXT_0 			ROHC_RP23_R_BASE + 3     
#define ROHC_RP23_R_1_EXT_1 			ROHC_RP23_R_BASE + 4     
#define ROHC_RP23_R_1_EXT_2 			ROHC_RP23_R_BASE + 5     
#define ROHC_RP23_R_1_EXT_3_B1 			ROHC_RP23_R_BASE + 6  
#define ROHC_RP23_R_1_EXT_3_B2 			ROHC_RP23_R_BASE + 7  
#define ROHC_RP23_UOR_2 				ROHC_RP23_R_BASE + 8			
#define ROHC_RP23_UOR_2_EXT_0 			ROHC_RP23_R_BASE + 9   
#define ROHC_RP23_UOR_2_EXT_1 			ROHC_RP23_R_BASE + 10  
#define ROHC_RP23_UOR_2_EXT_2 			ROHC_RP23_R_BASE + 11 
#define ROHC_RP23_UOR_2_EXT_3_B1		ROHC_RP23_R_BASE + 12 
#define ROHC_RP23_UOR_2_EXT_3_B2		ROHC_RP23_R_BASE + 13 
#define ROHC_P23_R_MAX_PACKETS 			ROHC_RP23_UOR_2_EXT_2 + 14



#endif /*_ROHC_CPRM_EXT_IF_H_*/
