/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_dcpm_ext_if.h,v 1.2.36.3 2010/11/09 04:36:19 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains function declaration for 
 *                     initialization and entry point for D-CPM.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_dcpm_ext_if.h,v $
 * Revision 1.2.36.3  2010/11/09 04:36:19  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.2.36.2  2010/10/13 04:09:27  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2.36.1  2010/10/04 03:09:56  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.2  2009/05/28 04:57:01  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_DCPM_EXT_IF_
#define _ROHC_DCPM_EXT_IF_


#include "rohc_typ.h"
#include "rohc_com_def.h"
#include "rohc_g_typ.h"
#include "rohc_d_typ.h"

/* + SPR 17439 */
rohc_void_t rohc_dcpm_init 
        ROHC_PROTO((void));

rohc_return_t rohc_dcpm_exec 
        ROHC_PROTO((
        rohc_entity_t       *p_entity   ,
        rohc_dsc_t          *p_dsc      ,
        rohc_U8bit_t        event_type  ,
        rohc_error_code_t   *p_ecode));
        
rohc_return_t rohc_dcpm_p0_exec 
        ROHC_PROTO((
        rohc_entity_t       *p_entity   ,
        rohc_dsc_uncomp_t   *p_dsc      ,
        rohc_U8bit_t        event_type  ,
        rohc_error_code_t   *p_ecode));
        
rohc_return_t rohc_dcpm_p1_exec 
        ROHC_PROTO((
        rohc_entity_t       *p_entity   ,
        rohc_dsc_rtp_t      *p_dsc      ,
        rohc_U8bit_t        event_type  ,
        rohc_error_code_t   *p_ecode));
        
rohc_return_t rohc_dcpm_p2_exec 
        ROHC_PROTO((
        rohc_entity_t     *p_entity  ,
        rohc_dsc_udp_t    *p_dsc     ,
        rohc_U8bit_t      event_type ,
        rohc_error_code_t *p_ecode));        
/* - SPR 17439 */
rohc_return_t rohcDcpmP6Exec (
        rohc_entity_t     *p_entity  ,
        rohc_dsc_tcp_t    *p_dsc     ,
        rohc_U8bit_t      event_type ,
        rohc_error_code_t *p_ecode);
        
/* + SPR 17439 */
rohc_return_t rohc_dcpm_exec_direct_evt 
        ROHC_PROTO((
        rohc_entity_t       *p_entity   ,
        rohc_dsc_t          *p_dsc      , 
        rohc_U8bit_t        evt_typ     ,
         rohc_error_code_t  *p_ecode
        ));   

rohc_return_t rohc_dcpm_p1_exec_direct_evt 
        ROHC_PROTO((
        rohc_entity_t       *p_entity   ,
        rohc_dsc_rtp_t      *p_dsc      , 
        rohc_U8bit_t        evt_typ     ,
         rohc_error_code_t  *p_ecode
        ));   

rohc_return_t rohc_dcpm_p2_exec_direct_evt 
        ROHC_PROTO((
        rohc_entity_t      *p_entity ,
        rohc_dsc_udp_t     *p_dsc    , 
        rohc_U8bit_t       evt_typ   ,
         rohc_error_code_t *p_ecode
        ));   
/* - SPR 17439 */
rohc_return_t rohcDcpmP6ExecDirectEvt(
        rohc_entity_t      *p_entity ,
        rohc_dsc_tcp_t     *p_dsc    , 
        rohc_U8bit_t       evt_typ   ,
        rohc_error_code_t *p_ecode
);  

/* + SPR 17439 */
rohc_return_t rohc_dcpm_validate_pkt
        ROHC_PROTO((
        	rohc_entity_t      *p_entity   ,
        	rohc_dsc_common_t  *p_dsc      ,
	        rohc_U8bit_t       pkt_typ));
/* - SPR 17439 */
	
#endif

