/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_sdvl.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Self Described Variable Length  encoding
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_sdvl.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/28 05:15:10  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_SDVL_H_
#define _ROHC_SDVL_H_
#include "rohc_com_def.h"

#define ROHC_SDVL_GET_BIT_7(x)        ((*x) & 0x80)
#define ROHC_SDVL_GET_BIT_6_7(x)      ( ((*x) & 0xc0) >> 6 )
#define ROHC_SDVL_GET_BIT_5_7(x)      ( ((*x) & 0xe0) >> 5 )
#define ROHC_SDVL_GET_BIT_0_7(x)      ((*x) & 0xff)
#define ROHC_SDVL_GET_BIT_0_4(x)      ((*x) & 0x1f)
#define ROHC_SDVL_GET_BIT_0_5(x)  	  ((*x) & 0x3f)
#define ROHC_SDVL_GET_BIT_0_6(x)      ((*x) & 0x7f)

#endif

