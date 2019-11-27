/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_proto.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
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
 * $Log: rohc_proto.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.4.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.4  2009/05/28 05:18:04  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_PROTO_H_
#define _ROHC_PROTO_H_

#include "lteFramework.h"
#include "rohc_com_def.h"
/* COVERITY 10216 */
//#include "rohc_def.h"
/* COVERITY 10216 */
#include "rohc_typ.h"
/* + SPR 17439 */
void rohc_start_timer ROHC_PROTO(( 
                                        rohc_timer_data_t *td ,    
                                        rohc_U32bit_t duration ,
                                        rohc_timer_id_t *tid
                                        ));

void rohc_stop_timer ROHC_PROTO (( rohc_timer_data_t tdata , rohc_timer_id_t tid ));
/* - SPR 17439 */
rohc_void_t *rohc_mem_get   ROHC_PROTO ((rohc_U32bit_t  size));

rohc_void_t rohc_mem_free   ROHC_PROTO (( rohc_void_tp  p_buf));


#endif
