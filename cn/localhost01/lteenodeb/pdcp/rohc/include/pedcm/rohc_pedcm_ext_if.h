/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_pedcm_ext_if.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the entry point funtion prototype
 *                     for PEDCM (Packet error detection and recovery module).
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_pedcm_ext_if.h,v $
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

#ifndef _ROHC_CCPM_EXT_IF_
#define _ROHC_CCPM_EXT_IF_

rohc_return_t rohc_pedcm_hndlr ROHC_PROTO((rohc_entity_t *p_entity, rohc_dsc_t *p_dsc));

#endif /*_ROHC_CCPM_EXT_IF_*/
