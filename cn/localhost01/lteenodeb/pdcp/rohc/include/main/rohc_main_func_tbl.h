/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_main_func_tbl.h,v 1.1.1.1.16.1 2010/11/03 06:42:18 gur22059 Exp $
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
 * $Log: rohc_main_func_tbl.h,v $
 * Revision 1.1.1.1.16.1  2010/11/03 06:42:18  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.2.36.1  2010/10/13 04:28:10  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.2  2009/05/28 05:27:38  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_MAIN_FUNC_TBL_H_
#define _ROHC_MAIN_FUNC_TBL_H_

#include "rohc_typ.h"
#include "rohc_def.h"
#include "rohc_ex.h"

#include "rohc_g_typ.h"
#include "rohc_com_def.h"



/* + SPR 17439 */
typedef rohc_return_t   (*rohc_api_fptr_tbl_t) 	ROHC_PROTO 
	((
		rohc_entity_t 		*p_rohc_entity	,
		rohc_U8bit_t		*p_in_buff	,
		rohc_U8bit_t 		**p_p_out_buff	,
		rohc_api_info_t 	*p_rohc_api_info	,
		rohc_error_code_t 	*p_ecode
	));

/* - SPR 17439 */
rohc_api_fptr_tbl_t	rohc_api_fptr_tbl[ROHC_IN_API_LAST - ROHC_IN_API_BASE + 1];

#endif /* _ROHC_MAIN_FUNC_TBL_H_ */

