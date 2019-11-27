/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_main.c,v 1.1.1.1.16.1 2010/11/03 07:01:05 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : Entry point Functions for ROHC entity message parsing.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_main.c,v $
 * Revision 1.1.1.1.16.1  2010/11/03 07:01:05  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.4.36.3  2010/10/13 03:53:05  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.4.36.2  2010/10/01 14:06:58  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.4.36.1  2010/09/23 05:18:07  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.4  2009/05/28 03:26:54  gur19836
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

#include "rohc_ccom_mcr.h"
#include "rohc_g_typ.h"
#include "rohc_trace_mcr.h"

#include "rohc_main_func_tbl.h"
#include "rohc_main_proto.h"
#include "rohcTimer.h"
#include "rohc_ppm_ext_if.h"
#include "rohc_cprm_ext_if.h"
#include "rohc_dprm_ext_if.h"
#include "rohc_ccpm_ext_if.h"
#include "rohc_dcpm_ext_if.h"
#include "rohc_pbm_ext_if.h"
#include "rohc_led_ext_if.h"
#include "rohc_crc_ext_if.h"
#include "rohc_opt_proto.h"
#include "rohc_opt_def.h"
/* + SPR 17439 */
#include "rohc_main_ex.h"
/* - SPR 17439 */
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
#define ROHC_MAX_HDR_SIZE 120
#define ROHC_MAX_UNCOM_PKT_LEN 1024
/******************************************************************************
 * Exported Variables
 *****************************************************************************/
rohc_global_t rohc_global;


/*ROHC_MAX_HDR_SIZE*/
rohc_U8bit_t *rohc_process_data_req_buff_p = ROHC_NULL;
/* 50 */
rohc_U8bit_t *rohc_process_p0_compr_pkt_out_buff_p = ROHC_NULL;
rohc_U8bit_t *rohc_process_p1_compr_pkt_out_buff_p = ROHC_NULL;
rohc_U8bit_t *rohc_process_p2_compr_pkt_out_buff_p = ROHC_NULL;
rohc_U8bit_t *rohc_process_p6_compr_pkt_out_buff_p = ROHC_NULL;

/*ROHC_MAX_HDR_SIZE*/
rohc_U8bit_t *rohc_process_rlc_data_ind_buff_p = ROHC_NULL;

/*ROHC_MAX_UNCOM_PKT_LEN*/
rohc_U8bit_t *rohc_process_p1_decompr_pkt_uncom_msg_p = ROHC_NULL;
rohc_U8bit_t *rohc_process_p2_decompr_pkt_uncom_msg_p = ROHC_NULL;
rohc_U8bit_t *rohc_process_p6_decompr_pkt_uncom_msg_p = ROHC_NULL;


/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

rohc_void_t   rohc_main_func_hand_init  ROHC_NO_PARAMS ()
{
  rohc_api_fptr_tbl[ROHC_DATA_REQ - 
                    ROHC_IN_API_BASE] = rohc_process_data_req;
  rohc_api_fptr_tbl[ROHC_RLC_DATA_IND - 
                    ROHC_IN_API_BASE] = rohc_process_rlc_data_ind ;
  rohc_api_fptr_tbl[ROHC_CONFIG_REQ - 
                    ROHC_IN_API_BASE] = rohc_process_config_req;
  rohc_api_fptr_tbl[ROHC_RELEASE_REQ - 
                    ROHC_IN_API_BASE] = rohc_process_release_req ;
  rohc_api_fptr_tbl[ROHC_RECONFIG_REQ - 
                    ROHC_IN_API_BASE] = rohc_process_reconfig_req ;
  rohc_api_fptr_tbl[ROHC_RESET_REQ - 
                    ROHC_IN_API_BASE] = rohc_process_reset_req;
  rohc_api_fptr_tbl[ROHC_RO_INITDB_REQ - 
                    ROHC_IN_API_BASE] = rohc_process_initdb_req ;
  rohc_api_fptr_tbl[ROHC_GET_RO_STATS_REQ - 
                    ROHC_IN_API_BASE] = rohc_process_get_init_stats_req ;
  rohc_api_fptr_tbl[ROHC_GET_RO_INIT_STATS_REQ - 
                    ROHC_IN_API_BASE] = rohc_process_get_init_stats_req ;
}

/*******************************************************************************
**  FUNCTION :	rfc3095_process_msg
********************************************************************************
**  DESCRIPTION :
**      This function is the entry point for the RFC3095 Compression entity. 
**
**  INPUT:
**
** 	1) *p_ro_entity  = pointer to the rfc 3095 entity, the entry for rohc entity
**					   in db is made in PDCP. 
**	2)	*p_in_buff  = pointer to the incoming buffer.
**	3)	**p_p_out_buff = pointer to outgoing buffer, in which ROHC will fill data,
**                     and return to PDCP.
**	4)	*p_api_buff_info  = pointer to the info for different ROHC APIs.
**	5)	*p_ecode = pointer to error code.
**
**  RETURN VALUES:
**      
**      1. ROHC_SUCCESS:        If action is success.
**      2. ROHC_FAILURE:        If action is failure.
**
*******************************************************************************/
rohc_return_t   rfc3095_process_msg 	ROHC_PARAMS
	(
		(p_ro_entity, p_in_buff, p_p_out_buff, p_api_buff_info, p_ecode),
		rohc_void_t 	*p_ro_entity _AND_
		rohc_U8bit_t 	*p_in_buff _AND_
		rohc_U8bit_t	**p_p_out_buff _AND_
		rohc_void_t 	*p_api_buff_info _AND_
		rohc_error_t 	*p_ecode
	)
{
	rohc_entity_t 		*p_rohc_entity = ROHC_NULL;
    rohc_return_t       ret_val = ROHC_SUCCESS;
	rohc_api_fptr_tbl_t function_handler;
	
	rohc_api_info_t *p_rohc_api_info = (rohc_api_info_t*)p_api_buff_info;

	p_rohc_entity = (rohc_entity_t*)p_ro_entity;

	ROHC_ENTER_FN("rfc3095_process_msg");

	/* See if its a valid API Id. If not, drop it. */
	/* SPR 3444 changes start */
    /* + SPR 5584 (COVERITY CID 24604) FIX */
	if ( p_rohc_api_info->api_id < ROHC_IN_API_BASE || 
    /* - SPR 5584 (COVERITY CID 24604) FIX */
             p_rohc_api_info->api_id > ROHC_IN_API_LAST )
	/* SPR 3444 changes end */
	{
		*p_ecode = ROHC_INVALID_API_ID;
  		ROHC_LOG(LOG_INFO, "ED:EID[%u:%u] " \
		"Invalid API received", \
		p_rohc_entity->rohc_entity_id.ueIndex,
                p_rohc_entity->rohc_entity_id.lcId);
		return RO_FAILURE;
	}
	if( (rohc_global.rohc_state != ROHC_INIT) &&
		((p_rohc_api_info->api_id != ROHC_RO_INITDB_REQ)))
	{
	        *p_ecode = ROHC_DB_NOT_INIT;
                ROHC_LOG(LOG_INFO, "ED:EID[%u:%u] " \
                "Rohc DB is not initialized", \
                p_rohc_entity->rohc_entity_id.ueIndex,
                p_rohc_entity->rohc_entity_id.lcId);
                return RO_FAILURE;
	}

	if ((function_handler = rohc_api_fptr_tbl[p_rohc_api_info->api_id - 
                                            ROHC_IN_API_BASE]) != ROHC_NULL)
	{
		ret_val = function_handler(p_rohc_entity, p_in_buff, p_p_out_buff,
		   			p_rohc_api_info, (rohc_error_code_t * )p_ecode);
	}
  else
	{
		*p_ecode = ROHC_INVALID_API_ID;
  		ROHC_LOG(LOG_INFO, "ED:EID[%u:%u] " \
		"Function Pointer Table not initialized for API-ID : [%u]", \
		p_rohc_entity->rohc_entity_id.ueIndex,
        p_rohc_entity->rohc_entity_id.lcId, p_rohc_api_info->api_id );
		ret_val = RO_FAILURE;
	}
  
	ROHC_EXIT_FN("rfc3095_process_msg");
    return ret_val ;
}


/******************************************************************
**      FUNCTION:
**              rohc_main_init
*******************************************************************
**
**      DESCRIPTION:
**              Initializes the MAIN MODULE data structures.
**				calls init modules of various sub-modules.
**				This function is called by late init when pdcp comes up.
**      ARGUMENTS:
**				None
**      RETURN VALUE:
**              ROHC_SUCCESS/ROHC_FAILURE
**      NOTES:
**
********************************************************************/
rohc_return_t	rohc_main_init 	ROHC_NO_PARAMS ()
{
    rohc_U32bit_t	count = 0;

    ROHC_ENTER_FN("rohc_main_init");

    for (count = ROHC_IN_API_BASE; count <= ROHC_IN_API_LAST; count++)
    {
        rohc_api_fptr_tbl[count-ROHC_IN_API_BASE] = ROHC_NULL;
    }

    rohc_main_func_hand_init();

    /* calling init of PRM, CPM, */
    rohc_d_ppm_init();
    rohc_cprm_init();
    rohc_dprm_init();
    rohc_ccpm_init();
    rohc_dcpm_init();
    rohc_c_pbm_init();
    rohc_led_p1_init();
    rohc_led_p2_init();
    rohc_init_crc();

    initRohcTimer();
    rohc_global.rohc_state = 0;

    /* reserving memory for global pointers*/
    rohc_process_data_req_buff_p = (rohc_U8bit_t *)ROHC_MALLOC(
            (sizeof(rohc_U8bit_t))*ROHC_MAX_HDR_SIZE);
    rohc_process_p0_compr_pkt_out_buff_p = (rohc_U8bit_t *)ROHC_MALLOC(
            (sizeof(rohc_U8bit_t))*50); 	
    rohc_process_p1_compr_pkt_out_buff_p = (rohc_U8bit_t *)ROHC_MALLOC(
            (sizeof(rohc_U8bit_t))*ROHC_MAX_HDR_SIZE); 	
    rohc_process_p2_compr_pkt_out_buff_p = (rohc_U8bit_t *)ROHC_MALLOC(
            (sizeof(rohc_U8bit_t))*ROHC_MAX_HDR_SIZE);
    rohc_process_p6_compr_pkt_out_buff_p = (rohc_U8bit_t *)ROHC_MALLOC(
            (sizeof(rohc_U8bit_t))*ROHC_MAX_HDR_SIZE); 	
    rohc_process_rlc_data_ind_buff_p = (rohc_U8bit_t *)ROHC_MALLOC(
            (sizeof(rohc_U8bit_t))*ROHC_MAX_HDR_SIZE);
    rohc_process_p1_decompr_pkt_uncom_msg_p = (rohc_U8bit_t *)ROHC_MALLOC(
            (sizeof(rohc_U8bit_t))*ROHC_MAX_UNCOM_PKT_LEN);
    rohc_process_p2_decompr_pkt_uncom_msg_p = (rohc_U8bit_t *)ROHC_MALLOC(
            (sizeof(rohc_U8bit_t))*ROHC_MAX_UNCOM_PKT_LEN);
    rohc_process_p6_decompr_pkt_uncom_msg_p = (rohc_U8bit_t *)ROHC_MALLOC(
            (sizeof(rohc_U8bit_t))*ROHC_MAX_UNCOM_PKT_LEN);

    ROHC_EXIT_FN("rohc_main_init");
    return RO_SUCCESS;
}


    /* Coverity 54622/54623*/
/*****************************************************************************
 * Function Name  : rohcFreeFromPool
 * Inputs         : None
 * Outputs        : 
 * Returns        : None
 * Description    : This function is used to destroy all user memory pools
 *****************************************************************************/
void rohcFreeFromPool()
{
    freeMemPoolAndReturn(rohc_global.entity_pool);
    rohc_global.entity_pool = ROHC_NULL;
    freeMemPoolAndReturn(rohc_global.dsc_pool);
    rohc_global.dsc_pool = ROHC_NULL;
    freeMemPoolAndReturn(rohc_global.ip4_key_pool);
    rohc_global.ip4_key_pool = ROHC_NULL;
    freeMemPoolAndReturn(rohc_global.ip6_key_pool);
    rohc_global.ip6_key_pool = ROHC_NULL;
/* Fix for Memory Optimisation  Start */
    /* Removed CPIB and DPIB Pool Allocation */
/* Fix for Memory Optimisation  End */
    freeMemPoolAndReturn(rohc_global.node_pool);
    rohc_global.node_pool = ROHC_NULL;
    freeMemPoolAndReturn(rohc_global.dsc_node_pool);
    rohc_global.dsc_node_pool = ROHC_NULL;
    freeMemPoolAndReturn(rohc_global.csc_node_pool);
    rohc_global.csc_node_pool = ROHC_NULL;
    freeMemPoolAndReturn(rohc_global.list_node_pool);
    rohc_global.list_node_pool = ROHC_NULL;
    cleanRohcTimer();
}

    /* Coverity 54622/54623*/
