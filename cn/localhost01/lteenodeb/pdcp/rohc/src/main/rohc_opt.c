/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_opt.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : Optional Utility Functions for ROHC entity message parsing.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_opt.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/28 03:26:54  gur19836
 * File Header updated
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_typ.h"
#include "rohc_def.h"
#include "rohc_ex.h"

#include "rohc_trace_mcr.h"

#include "rohc_opt_def.h"
#include "rohc_opt_proto.h"

#include "rohc_main_ex.h"

#include "rohc_db_ext_if.h"
#include "rohc_stats_def.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* +- SPR 17777 */
extern tickType_t pdcpCurrentTime_g;
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/




/*******************************************************************************
**  FUNCTION :	rohc_process_initdb_req	
********************************************************************************
**  DESCRIPTION :
**      This function processes INIT_DB_REQ. and creates pools for entity, csc and dsc.
**
**  INPUT:
**
** 	1) *p_rohc_entity  = pointer to the rfc 3095 entity, the entry for rohc entity
**					   in db is made in PDCP. 
**	2)	*p_in_buff  = pointer to the incoming buffer.
**	3)	**p_p_out_buff = pointer to outgoing buffer, in which ROHC will fill data,
**                     and return to PDCP.
**	4)	*p_rohc_api_info  = pointer to the info for different ROHC APIs.
**	5)	*p_ecode = pointer to error code.
**
**  RETURN VALUES:
**      
**      1. RO_SUCCESS:        If action is success.
**      2. RO_FAILURE:        If action is failure.
**
*******************************************************************************/
rohc_return_t   rohc_process_initdb_req		ROHC_PARAMS
	(
		(p_rohc_entity, p_in_buff, p_p_out_buff, p_rohc_api_info, p_ecode),
		rohc_entity_t 	*p_rohc_entity _AND_
		rohc_U8bit_t	*p_in_buff	_AND_
		rohc_U8bit_t 	**p_p_out_buff	_AND_
		rohc_api_info_t *p_rohc_api_info	_AND_
		rohc_error_code_t 	*p_ecode
	)
{
	rohc_return_t   ret_val = RO_SUCCESS;
	
	ROHC_ENTER_FN("rohc_process_initdb_req");

	ret_val = \
	rohc_init_db(p_rohc_api_info->rohc_api_union.initdb_req.max_size_entity_pool, \
	p_rohc_api_info->rohc_api_union.initdb_req.max_size_csc_pool, \
	p_rohc_api_info->rohc_api_union.initdb_req.max_size_dsc_pool);
	
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,sizeof(p_ecode),sizeof(p_p_out_buff),sizeof(p_in_buff),
            sizeof(p_rohc_entity),0,0, __func__,
            "PdcpRxStateMachine");
    /* +- SPR 17777 */
	ROHC_EXIT_FN("rohc_process_initdb_req");
	return !(ret_val);
}

/*******************************************************************************
**  FUNCTION :rohc_process_get_init_stats_req		
********************************************************************************
**  DESCRIPTION :
**      This function collects the complete statistics for all entities.
**
**  INPUT:
**
** 	2) *p_rohc_entity  = pointer to the rfc 3095 entity, the entry for rohc entity
**					   in db is made in PDCP. 
**	2)	*p_in_buff  = pointer to the incoming buffer.
**	3)	**p_p_out_buff = pointer to outgoing buffer, in which ROHC will fill data,
**                     and return to PDCP.
**	4)	*p_rohc_api_info  = pointer to the info for different ROHC APIs.
**	5)	*p_ecode = pointer to error code.
**
**  RETURN VALUES:
**      
**      1. RO_SUCCESS:        If action is success.
**      2. RO_FAILURE:        If action is failure.
**
*******************************************************************************/
rohc_return_t   rohc_process_get_init_stats_req		ROHC_PARAMS
	(
		(p_rohc_entity, p_in_buff, p_p_out_buff, p_rohc_api_info, p_ecode),
		rohc_entity_t 	*p_rohc_entity _AND_
		rohc_U8bit_t	*p_in_buff	_AND_
		rohc_U8bit_t 	**p_p_out_buff	_AND_
		rohc_api_info_t *p_rohc_api_info	_AND_
		rohc_error_code_t 	*p_ecode
	)
{
	rohc_return_t 	result = RO_SUCCESS;
	ROHC_ENTER_FN("rohc_process_get_init_stats_req");

    result = RO_FAILURE ;
    *p_ecode = ROHC_STATS_DISABLED;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,sizeof(p_rohc_api_info),sizeof(p_p_out_buff),sizeof(p_in_buff),
            sizeof(p_rohc_entity),0,0, __func__,
            "PdcpRxStateMachine");
    /* +- SPR 17777 */
	ROHC_EXIT_FN("rohc_process_get_init_stats_req");
	return result;
}

