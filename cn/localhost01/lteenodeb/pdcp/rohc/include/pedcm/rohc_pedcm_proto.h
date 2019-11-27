/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_pedcm_proto.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the funtion prototypes for
 *                         Algorithm-1 : Repair of incorrect SN updates
 *                                              and
 *                         Algorithm-2 : Correction of SN LSB wraparound
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_pedcm_proto.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/28 05:32:09  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_PEDCM_PROTO_
#define _ROHC_PEDCM_PROTO_

#include "rohc_dcpm_ext_if.h"
#include "rohc_dcpm_u_done.h"
#include "rohc_dcpm_u_pending.h"
#include "rohc_dcpm_u_initiated.h"
#include "rohc_dcpm_o_done.h"
#include "rohc_dcpm_o_pending.h"
#include "rohc_dcpm_o_initiated.h"
#include "rohc_dcpm_r_done.h"
#include "rohc_dcpm_r_pending.h"
#include "rohc_dcpm_r_initiated.h"
#include "rohc_dcpm_event.h"
#include "rohc_def.h"
#include "rohc_trace_mcr.h"
#include "rohc_dcom_mcr.h"


/* + SPR 17439 */
rohc_return_t rohc_pedcm_incor_sn_update_algo ROHC_PROTO((
			rohc_entity_t *p_entity ,	
			rohc_dsc_t *p_dsc));

rohc_return_t rohc_pedcm_sn_lsb_algo ROHC_PROTO((
			rohc_entity_t *p_entity ,	
			rohc_dsc_t *p_dsc));
 
/* - SPR 17439 */
#endif /*_ROHC_PEDCM_PROTO_*/

