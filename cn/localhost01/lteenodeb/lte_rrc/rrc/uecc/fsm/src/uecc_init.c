/******************************************************************************
*
*   FILE NAME:
*       uecc_init.c
*
*   DESCRIPTION:
*       This is the main file of the UECC Initialization module. This module
*       provides initialization handler for UECC CSPL entity.
*
*       In addition the UECC clean up function is placed here.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   15 Jul 2009     GrygoriyS   ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

/****************************************************************************
 * Local includes
 ****************************************************************************/
#include "rrc_common_utils.h"
#include "uecc_common.h"
#include "uecc_ed.h"
#include "uecc_logger.h"
#include "uecc_ue_timer_utils.h"
#include "rrc_handlers.h" 
#include "rrc_shared_stats.h" 
/* These are used for compare function of a search tree*/
#define FIRST_IS_LESS_THAN_SECOND         -1
#define FIRST_IS_GREATER_THAN_SECOND       1
#define FIRST_IS_EQUAL_TO_SECOND           0
#define ERROR_IN_COMPARE                   0
#ifdef RRC_UNIT_TEST_FRAMEWORK
uecc_gb_context_t *p_uecc_gb_context = PNULL;
#endif

/******************************************************************************
 * Private functions definitions
 *****************************************************************************/
static void uecc_init_int
(
    uecc_gb_context_t *p_uecc_gb_context  /* UECC global context */
);

const void * key_of_ho_ongoing_ue_identity_search_tree(
        /* Pointer to Tree node */
        const YTNODE * p_node);

S32 compare_ho_ongoing_ue_identity_search_tree (
        /* Pointer to first element being compared */
        const void * p_elem1 ,

        /* Pointer to second element being compared */
        const void * p_elem2);

/*****************************************************************************
 * Function Name  : key_of_ho_ongoing_ue_identity_search_tree 
 * Inputs         : YTNODE - * p_node
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion ue id if p_node is not null  
 ********************************************************************************/
const void * key_of_ho_ongoing_ue_identity_search_tree
(
 /* Pointer to Tree node */
 const YTNODE * p_node
)
{
    return (p_node) ? &(((ho_ongoing_ue_identity_search_node_t *)p_node)->ue_identity) : PNULL;
}

/*****************************************************************************
 * Function Name  : compare_ho_ongoing_ue_identity_search_tree 
 * Inputs         : void *-  p_elem1
 *                  void *  - p_elem2
 * Outputs        : None
 * Returns        : ERROR_IN_COMPARE / FIRST_IS_LESS_THAN_SECOND/ FIRST_IS_EQUAL_TO_SECOND
 * Description    : Thid function compares two elements
 ********************************************************************************/
S32 compare_ho_ongoing_ue_identity_search_tree
(
 /* Pointer to first element being compared */
 const void * p_elem1 ,

 /* Pointer to second element being compared */
 const void * p_elem2
 )
{
    if (p_elem1 == PNULL || p_elem2 == PNULL)
    {
        return ERROR_IN_COMPARE;
    }

    if ( * ((const U32 *) p_elem1) == * ((const U32 *) p_elem2))
    {
        return FIRST_IS_EQUAL_TO_SECOND;
    }
    else if ( * ((const U32 *) p_elem1) < * ((const U32 *) p_elem2))
    {
        return FIRST_IS_LESS_THAN_SECOND;
    }
    else /*( * ((U32 *) p_elem1) > * ((U32 *) p_elem2))*/
    {
        return FIRST_IS_GREATER_THAN_SECOND;
    }
}


/******************************************************************************
 * Function implementation
 *****************************************************************************/

/******************************************************************************
*   FUNCTION NAME: rrc_uecc_init
*  
*   INPUT        : void *p_rrc_uecc_init_data
*
*   OUTPUT       : None
*
*   RETURNS      : p_uecc_gb_context
*                  
*   DESCRIPTION  : This is the main entry point of UECC module - CSPL callback. It handles
*                  initialization of UECC module.
*
*
******************************************************************************/
void* rrc_uecc_init
(
    void *p_rrc_uecc_init_data /* Pointer to RRC stack initialization data */
)
{

#ifndef RRC_UNIT_TEST_FRAMEWORK
    uecc_gb_context_t *p_uecc_gb_context = PNULL;
#endif

    U8                module_id = RRC_NULL;


    /* Check that in our PC architecture we can pass U16 as void* in queue */
    RRC_ASSERT(sizeof(void*) >= sizeof(U16));

    module_id = *(U8*)(p_rrc_uecc_init_data);

    /* Allocate Memory for UECC global context data structure */
    p_uecc_gb_context =
        (uecc_gb_context_t *)rrc_mem_get(sizeof(uecc_gb_context_t));
    
    if (0 == module_id)
    {
        module_id = RRC_UECC_1_MODULE_ID;
    }

    if(PNULL == p_uecc_gb_context)
    {
        qvPanic("[INIT] unable to Allocate Memory for UECC global context!");
        return PNULL;
    }
    else
    {
        /* Initialize UE global context */
        uecc_init_int(p_uecc_gb_context);
        p_uecc_gb_context->uecc_module_id = module_id;
        p_uecc_gb_context->offset_uecc_mod_id = (U8)(module_id - RRC_UECC_1_MODULE_ID); 
    }

    return p_uecc_gb_context;
}

/******************************************************************************
*   FUNCTION NAME: uecc_clean_up
*
*   INPUT        : uecc_gb_context_t *p_uecc_gb_context
*
*   OUTPUT       : None
*
*   RETURNS      : None
*
*   DESCRIPTION  : This is the UECC clean up function. After its execution the UECC will
*                  has same state as just after rrc_uecc_init() function execution.
*
******************************************************************************/
void rrc_uecc_clean_up
(
    uecc_gb_context_t *p_uecc_gb_context  /* UECC global context */
)
{
    U8 cell_index = RRC_NULL;
    /********************************
     * Clean up UECC
     ********************************/

    /* Delete all UE CTXs */
    uecc_ue_ctx_db_free(p_uecc_gb_context);

    /*Clean csc context */
    if (p_uecc_gb_context->p_p_csc_context != PNULL )
    {
        for (cell_index=0; cell_index<MAX_NUM_CELLS; cell_index++)
        {
            if (p_uecc_gb_context->p_p_csc_context[cell_index] != PNULL )
            {
                if (p_uecc_gb_context->p_p_csc_context[cell_index]->
                        p_csc_init_setup_ind != PNULL)
                {
                    rrc_mem_free (p_uecc_gb_context->p_p_csc_context[cell_index]->
                            p_csc_init_setup_ind);
                    p_uecc_gb_context->p_p_csc_context[cell_index]->
                            p_csc_init_setup_ind = PNULL;
                }
                if (p_uecc_gb_context->p_p_csc_context[cell_index]->
                         p_rrc_uecc_statistic != PNULL)
                {
                    rrc_mem_free (p_uecc_gb_context->p_p_csc_context[cell_index]->
                            p_rrc_uecc_statistic);
                    p_uecc_gb_context->p_p_csc_context[cell_index]->
                            p_rrc_uecc_statistic = PNULL;
                }
                if (p_uecc_gb_context->p_p_csc_context[cell_index]->
                         p_cell_traffic_trace_info != PNULL)
                {
                    rrc_mem_free (p_uecc_gb_context->p_p_csc_context[cell_index]->
                            p_cell_traffic_trace_info);
                    p_uecc_gb_context->p_p_csc_context[cell_index]->
                            p_cell_traffic_trace_info = PNULL;
                }

                rrc_mem_free(p_uecc_gb_context->p_p_csc_context[cell_index]);
                p_uecc_gb_context->p_p_csc_context[cell_index] = PNULL;
            }

        }
        rrc_mem_free(p_uecc_gb_context->p_p_csc_context);
        p_uecc_gb_context->p_p_csc_context = PNULL;

    }

    /* Initialize UE global context */
    uecc_init_int(p_uecc_gb_context);
}

/******************************************************************************
*   FUNCTION NAME: uecc_init_int
*
*   INPUT        : uecc_gb_context_t *p_uecc_gb_context
*
*   OUTPUT       : None
*
*   RETURNS      : None
*
*   DESCRIPTION  : Initializes UECC global context data.
*
******************************************************************************/
void uecc_init_int
(
    uecc_gb_context_t *p_uecc_gb_context  /* UECC global context */
)
{
    U16          x2ap_ueid = RRC_NULL;
    /* Reset UECC CTX */
    memset_wrapper(p_uecc_gb_context, 0, sizeof(uecc_gb_context_t));

    /* Init UE DB */
    /* Already done in memset_wrapper */

    /* Init timers */
    uecc_ue_timers_load_default_durations(p_uecc_gb_context, 
            &p_uecc_gb_context->ue_timers_data);

    p_uecc_gb_context->oamh_init_ind_duration
        = UECC_TIMER_OAMH_INIT_IND_DEFAULT_DURATION; /* in ms */
    p_uecc_gb_context->ue_timers_data.durations[UECC_UE_CAPABILITY_ENQ_TIMER]
        = UECC_TIMER_RRM_UE_CAPABILITY_ENQ_DURATION; /* in ms */

    /* Initialize X2AP_CTX */
    for (x2ap_ueid=0; x2ap_ueid<MAX_NUM_SUPPORTED_X2AP_UEID; x2ap_ueid++)
    {
        p_uecc_gb_context->x2ap_ctx.not_allocated_ue_ids[x2ap_ueid] = x2ap_ueid;
    }
    p_uecc_gb_context->x2ap_ctx.not_allocated_ue_ids_count =
        MAX_NUM_SUPPORTED_X2AP_UEID;
    p_uecc_gb_context->x2ap_ctx.not_allocated_ue_ids_cur = 0;

    /* Init Ho_ongoing search tree */
    st_init(&(p_uecc_gb_context->ho_ongoing_ue_identity_search_tree),
            &compare_ho_ongoing_ue_identity_search_tree,
            key_of_ho_ongoing_ue_identity_search_tree);

}


