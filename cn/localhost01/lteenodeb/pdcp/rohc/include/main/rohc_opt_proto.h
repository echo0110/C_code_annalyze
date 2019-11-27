/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_opt_proto.h,v 1.4.36.3 2010/11/09 04:51:17 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the function prototypes used in OPT
 *                     module.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_opt_proto.h,v $
 * Revision 1.4.36.3  2010/11/09 04:51:17  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.4.36.2  2010/10/13 04:30:04  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.4.36.1  2010/10/04 03:12:31  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.4  2009/05/28 05:27:38  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_OPT_PROTO_H_
#define _ROHC_OPT_PROTO_H_

#include "rohc_ex.h"
#include "rohc_g_typ.h"
#include "rohc_com_def.h"
/* + SPR 17439 */
rohc_return_t   rohc_process_initdb_req	ROHC_PROTO
	((
		rohc_entity_t 	*p_rohc_entity ,
		rohc_U8bit_t	*p_in_buff	,
		rohc_U8bit_t 	**p_p_out_buff	,
		rohc_api_info_t *p_rohc_api_info	,
		rohc_error_code_t 	*p_ecode
	));

rohc_return_t   rohc_process_get_init_stats_req	ROHC_PROTO
	((
		rohc_entity_t 	*p_rohc_entity ,
		rohc_U8bit_t	*p_in_buff	,
		rohc_U8bit_t 	**p_p_out_buff	,
		rohc_api_info_t *p_rohc_api_info	,
		rohc_error_code_t 	*p_ecode
	));

rohc_return_t rohc_check_mode_transition_p1_required ROHC_PROTO
    ((
        rohc_entity_t   *p_rohc_entity ,
        rohc_dsc_rtp_t  *p_dsc
    ));

rohc_return_t rohc_check_mode_transition_p2_required ROHC_PROTO
    ((
        rohc_entity_t   *p_rohc_entity ,
        rohc_dsc_udp_t  *p_dsc
    ));


rohc_return_t rohc_send_feedback_to_pdcp ROHC_PROTO
    ((
        rohc_entity_t   *p_rohc_entity 
    ));
/* SPR 5599_129036 Fix Start */
rohc_return_t rohcStatsModeTransUpdate ROHC_PROTO
/* SPR 5599_129036 Fix End */
    ((
        rohc_U8bit_t type ,
        rohc_U8bit_t dir
    ));

/* SPR 5599_129036 Fix Start */
rohc_return_t rohcStatsPktTypeUpdate ROHC_PROTO
/* SPR 5599_129036 Fix End */
    ((
        rohc_U8bit_t type ,
        rohc_U8bit_t dir  ,
        rohc_U8bit_t profile
    ));
/* + SPR 17439 */
void rohcStatsUpdateTxStats(rohc_U8bit_t type, rohc_U8bit_t profile);
void rohcStatsUpdateRxStats(rohc_U8bit_t type, rohc_U8bit_t profile);


#endif /* _ROHC_OPT_PROTO_H_ */
