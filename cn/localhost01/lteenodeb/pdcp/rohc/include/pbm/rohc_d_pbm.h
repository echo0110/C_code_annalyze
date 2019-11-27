/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_d_pbm.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains C-PBM packet encoding routines.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_d_pbm.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/28 05:29:58  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_D_PBM_H_
#define _ROHC_D_PBM_H_

#include "rohc_pbm_ext_if.h"
#include "rohc_com_def.h"
#include "rohc_dcom_mcr.h"
#include "rohc_g_typ.h"
#include "rohc_protocol_header.h"
#include "rohc_db_ext_if.h"

void rohcDecompMakeTcpIrregChain(
                struct rohc_dsc_tcp_t   * p_dsc             ,
                rohc_dprm_update_blk_t  *p_dprm_update_blk );
void rohcDecompMakeTcpBlockP6(
                struct rohc_dsc_tcp_t   * p_dsc             ,
                rohc_dprm_update_blk_t  *p_dprm_update_blk );
void rohcDecompMakeInnerIp4BlockP6(
                struct rohc_dsc_tcp_t   * p_dsc             ,
                rohc_dprm_update_blk_t  *p_dprm_update_blk );


#endif /* _ROHC_D_PBM_H_ */
