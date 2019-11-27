/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_ccpm_ext_if.h,v 1.2.36.3 2010/11/09 04:27:48 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains function declaration for
 *                     initialization and entry point for C-CPM.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_ccpm_ext_if.h,v $
 * Revision 1.2.36.3  2010/11/09 04:27:48  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2.36.2  2010/10/13 04:02:26  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2.36.1  2010/09/23 06:53:47  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.2  2009/05/28 04:57:01  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_CCPM_EXT_IF_
#define _ROHC_CCPM_EXT_IF_

#include "rohc_typ.h"
#include "rohc_com_def.h"
#include "rohc_g_typ.h"
#include "rohc_c_typ.h"

rohc_void_t rohc_ccpm_init ROHC_PROTO(ROHC_NO_PARAMS());

/* + SPR 17439 */
/* Event type is pkt type */
rohc_return_t rohc_ccpm_exec ROHC_PROTO((rohc_entity_t *p_entity ,
                     rohc_csc_t *p_csc ,
                     rohc_U8bit_t event_type ,
                     rohc_error_code_t *p_ecode 
                     ));
rohc_return_t rohc_ccpm_p0_exec ROHC_PROTO((rohc_entity_t *p_entity ,
                     rohc_csc_uncomp_t *p_csc ,
                     rohc_U8bit_t event_type ,
                     rohc_error_code_t *p_ecode 
                     ));
rohc_return_t rohc_ccpm_p1_exec ROHC_PROTO((rohc_entity_t *p_entity ,
                     rohc_csc_rtp_t     *p_csc      ,
                     rohc_U8bit_t       event_type  ,
                     rohc_error_code_t  *p_ecode 
                     ));
rohc_return_t rohc_ccpm_p2_exec ROHC_PROTO((rohc_entity_t *p_entity ,
                     rohc_csc_udp_t *p_csc ,
                     rohc_U8bit_t event_type ,
                     rohc_error_code_t *p_ecode 
                     ));

rohc_return_t rohc_ccpm_p6_exec ROHC_PROTO((rohc_entity_t *p_entity ,
                     rohc_csc_tcp_t *p_csc ,
                     rohc_U8bit_t event_type ,
                     rohc_error_code_t *p_ecode 
                     ));
/* Event type is the same as defined in CCPM*/
rohc_return_t rohc_ccpm_exec_direct_evt ROHC_PROTO((rohc_entity_t *p_entity ,
                     rohc_csc_t *p_csc ,
                     rohc_U8bit_t event_type ,
                     rohc_error_code_t *p_ecode 
                     ));
rohc_return_t rohc_ccpm_p1_exec_direct_evt ROHC_PROTO((rohc_entity_t *p_entity ,
                     rohc_csc_rtp_t     *p_csc      ,
                     rohc_U8bit_t       event_type  ,
                     rohc_error_code_t  *p_ecode 
                     ));
rohc_return_t rohc_ccpm_p2_exec_direct_evt ROHC_PROTO((rohc_entity_t *p_entity ,
                     rohc_csc_udp_t    *p_csc     ,
                     rohc_U8bit_t      event_type ,
                     rohc_error_code_t *p_ecode
                     ));

rohc_return_t rohc_ccpm_p6_exec_direct_evt ROHC_PROTO((rohc_entity_t *p_entity ,
                     rohc_csc_tcp_t    *p_csc     ,
                     rohc_U8bit_t      event_type ,
                     rohc_error_code_t *p_ecode
                     ));
rohc_return_t rohc_ccpm_init_timer_data ROHC_PROTO((
        rohc_entity_t *p_entity , 
        rohc_csc_t *p_csc 
        ));
/* - SPR 17439 */
#endif

