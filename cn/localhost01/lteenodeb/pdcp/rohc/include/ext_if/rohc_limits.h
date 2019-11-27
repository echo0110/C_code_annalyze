/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_limits.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
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
 * $Log: rohc_limits.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/28 05:18:04  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/


#ifndef ROHC_LIMITS_H
#define ROHC_LIMITS_H


#define ROHC_PKT_BEH_NO	(2U)
#define ROHC_IP_ID_JUMP_CONTROL	(15U)
/* START_COVERITY_25484_FIX*/
#define ROHC_MAX_FEEDBACK_PKT_SIZE (150U)
/* END_COVERITY_25484_FIX*/
#define ROHC_PDCP_API_HEADER_PRAM_SIZE (50U)
#define MAX_ROHC_C_PKT_BUFF	(5U)
#endif /* ROHC_LIMITS_H */

