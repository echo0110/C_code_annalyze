/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ***************************************************************************
 * File Details
 * ------------
 *  $Id: csc_cell_ctx.c,v 1.5 2010/04/20 05:43:17 gur21006 Exp $
 ***************************************************************************
 *
 *  File Description : This file contains RRC CSC cell contexts
 *                     related functions implementations.
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * Jan 8, 2010   grag -   initial creation
 * $Log: csc_cell_ctx.c,v $
 * Revision 1.5  2010/04/20 05:43:17  gur21006
 * paging for si mod period added
 *
 * Revision 1.4  2010/04/07 08:14:15  gur21006
 * sfn timer should be deleted along with cell context
 *
 * Revision 1.3  2010/04/06 04:00:56  gur18569
 * put Null check on p_cell_ctx->p_temp_si_schdl_info
 *
 * Revision 1.2  2010/03/29 10:36:28  gur21006
 * code merged
 *
 * Revision 1.1  2010/01/13 16:38:56  ukr16032
 * CSC Multi UE Paging feature has been implemented.
 *
 *
 ***************************************************************************/

/****************************************************************************
 * Inculdes
 ****************************************************************************/
#include "csc_cell_ctx.h"
#include "csc_logger.h"
#include "csc_cell_m_fsm.h"
#include "csc_sfn_m.h"
#include "csc_paging_m.h"

/*Used for tree comparison*/
#define COMPARE_ERROR               -1
#define FIRST_LESS_THAN_SECOND      -1
#define FIRST_EQUAL_TO_SECOND       0
#define FIRST_GREATER_THAN_SECOND   1

/******************************************************************************
 * Function implementation
 *****************************************************************************/
const void * key_of_cmas_info_key_search_tree(
                        /* Pointer to Tree node */
                        const YTNODE * p_node);

S32 compare_cmas_info_key_search_tree (
                        /* Pointer to first element being compared */
                        const void * p_elem1 ,

                        /* Pointer to second element being compared */
                        const void * p_elem2);

const void * key_of_cmas_info_key_search_tree
(
   /* Pointer to Tree node */
   const YTNODE * p_node
)
{
    return (p_node) ? &(((cmas_info_t *)p_node)->cmas_key) : PNULL;
}

S32 compare_cmas_info_key_search_tree
(
   /* Pointer to first element being compared */
   const void * p_elem1 ,

   /* Pointer to second element being compared */
   const void * p_elem2
)
{
    if (p_elem1 == PNULL || p_elem2 == PNULL)
    {
        RRC_CSC_TRACE(RRC_WARNING, "%s: passed null arg",__FUNCTION__);
        return COMPARE_ERROR;
    }

    if ( * ((const U32 *) p_elem1) == * ((const U32 *) p_elem2))
    {
        return FIRST_EQUAL_TO_SECOND;
    }
    else if ( * ((const U32 *) p_elem1) < * ((const U32 *) p_elem2))
    {
        return FIRST_LESS_THAN_SECOND;
    }
    else
    {
        return FIRST_GREATER_THAN_SECOND;
    }
}


/******************************************************************************
*   FUNCTION NAME:  csc_cell_ctx_create
*   INPUT        :  csc_gl_ctx_t  *p_csc_gb_context
*                   rrc_cell_index_t  cell_index
*   OUTPUT       :  None
*   RETURNS      : pointer on CSC cell context which fields are initialized by default
*   DESCRIPTION  : Allocates and initializes CSC Cell context.
******************************************************************************/
csc_cell_ctx_t* csc_cell_ctx_create(
    csc_gl_ctx_t  *p_csc_gb_context,  /* CSC global context */
    rrc_cell_index_t  cell_index)  /* Cell Index */
{
    csc_cell_ctx_t *p_cell_ctx  = PNULL;

    RRC_CSC_UT_TRACE_ENTER();
    
    RRC_ASSERT( PNULL != p_csc_gb_context );
    RRC_ASSERT( PNULL != p_csc_gb_context->p_p_cell_ctx );
    if ( p_csc_gb_context->max_num_supported_cells <= p_csc_gb_context->num_allocated_cell_ctx_curr )
    {
        RRC_CSC_TRACE(RRC_ERROR, "Maximum Cell Contexts already allocated");
    } 
    else if (!(p_csc_gb_context->csc_cell_index_status >> cell_index))
    {
        RRC_CSC_TRACE(RRC_ERROR, "Communication/Provision Info doesn't exist for cell Index [%d]", cell_index);
     }
    else
    {
        p_cell_ctx = (csc_cell_ctx_t*)rrc_mem_get(sizeof(csc_cell_ctx_t));

        if(PNULL == p_cell_ctx)
        {
            RRC_CSC_TRACE(RRC_ERROR, "[CELL:%u] Memory allocation failed",
                    cell_index);
            return p_cell_ctx;
        }

        memset_wrapper( p_cell_ctx, 0, sizeof(csc_cell_ctx_t) );
        
        RRC_CSC_TRACE(RRC_BRIEF, 
                "[CELL_INDEX:%u] context allocated", cell_index);

        /* Initialize main IEs of cell context */
        p_cell_ctx->cell_index  = cell_index;
        /* SPR 23608/SES-43 Fix Start */
        if (PNULL != p_cell_ctx->guard_timer)
        {
            RRC_CSC_TRACE(RRC_BRIEF,"Stopping Cell Context Guard Timer"); 
            /* Stop Timer */
            rrc_stop_timer(p_cell_ctx->guard_timer);
            p_cell_ctx->guard_timer = PNULL;
        }
        /* SPR 23608/SES-43 Fix End */
        p_cell_ctx->sfn_timer   = PNULL;
        p_cell_ctx->mac_sfn_sync_timer = PNULL;

        /* SPR 11229 Fix Start */
        //Initilaize the last sent sib8 sfn to an unacceptable value per MAX SFN LIMIT
        //so that on cell reconfiguration, this can be initilized to the correct last_sent_sib_sfn
        p_cell_ctx->last_sent_sib8_sfn = LAST_SENT_SIB8_SFN_UNINIT;
        /* SPR 11229 Fix Stop */

        /* Initialize Cell FSM */
        CELL_M_FSM_INIT(p_cell_ctx);

        /* Initialize Paging records storage */
        p_cell_ctx->p_paging_storage = csc_paging_storage_init();
        if (PNULL == p_cell_ctx->p_paging_storage)
        {
            RRC_CSC_TRACE(RRC_ERROR,
                    "[CELL:%u] Paging records storage initialization failed",
                    cell_index);
            rrc_mem_free(p_cell_ctx);
            p_cell_ctx = PNULL;
            return p_cell_ctx;
        }

        /* Init CMAS warning search tree */
        st_init(&(p_cell_ctx->cmas_info_list),
                &compare_cmas_info_key_search_tree,
                key_of_cmas_info_key_search_tree);


        p_csc_gb_context->num_allocated_cell_ctx_curr++;
        p_csc_gb_context->p_p_cell_ctx[cell_index] = p_cell_ctx;

        RRC_CSC_TRACE(RRC_INFO, "[CELL:%u] Context Initialized.", cell_index);
    }

    RRC_CSC_UT_TRACE_EXIT();

    return p_cell_ctx;
}

/******************************************************************************
*   FUNCTION NAME: csc_cell_ctx_cleanup
*   INPUT        :  csc_gl_ctx_t  *p_csc_gb_context
*                   rrc_cell_index_t  cell_index
*   OUTPUT       :  None
*   RETURNS      : None
*   DESCRIPTION  : Cleans up CSC Cell context.
******************************************************************************/
void csc_cell_ctx_cleanup(
        csc_gl_ctx_t   *p_csc_gb_context,  /* CSC global context */
        csc_cell_ctx_t *p_cell_ctx)
{
    rrc_cell_index_t cell_index;
    cmas_info_t *temp_cmas_info_node = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    if (PNULL == p_cell_ctx)
    {
        RRC_CSC_TRACE(RRC_ERROR, "[CELL] Context Not initialized");
        return;
    }

    cell_index = p_cell_ctx->cell_index;

    /* Free timers */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF,
                "[CELL:%u] Stop timer [CSC_CELL_GUARD_TIMER]", cell_index);
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }

    if (PNULL != p_cell_ctx->mac_sfn_sync_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF,
                "[CELL:%u] Stop timer [CELL_M_SFN_SYNC_TIMER]", cell_index);

        rrc_stop_timer(p_cell_ctx->mac_sfn_sync_timer);
        p_cell_ctx->mac_sfn_sync_timer = PNULL;
    }

    /* Stop SFN update process */
    sfn_m_stop_sfn(p_cell_ctx);

    /* final cleanup*/
    /* free storage */
    rrc_mem_free(p_cell_ctx->p_paging_storage);
    p_cell_ctx->p_paging_storage = PNULL;
/* SPR 21369 Start */
    rrc_mem_free(p_cell_ctx->p_api);
    p_cell_ctx->p_api = PNULL;
/* SPR 21369 End */
 
    if (p_cell_ctx->p_temp_si_schdl_info)
    {
        rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
        p_cell_ctx->p_temp_si_schdl_info = PNULL;
    }
    p_cell_ctx->p_temp_si_schdl_info = PNULL;
    

    if (PNULL != p_cell_ctx->p_enb_pws_req)
    {
        rrc_mem_free(p_cell_ctx->p_enb_pws_req);
        p_cell_ctx->p_enb_pws_req = PNULL;
    }

    if (PNULL != p_cell_ctx->p_enb_kill_req)
    {
        rrc_mem_free(p_cell_ctx->p_enb_kill_req);
        p_cell_ctx->p_enb_kill_req = PNULL;
    }

    p_cell_ctx->cmas_flag = RRC_FALSE;

    if (PNULL != p_cell_ctx->etws_info)
    {
        rrc_mem_free (p_cell_ctx->etws_info);
        p_cell_ctx->etws_info = PNULL;
    }
    while (st_get_node_count(
            (const search_tree_t *)&(p_cell_ctx->cmas_info_list)) > 0)
    {
        temp_cmas_info_node = (cmas_info_t *)(st_get_first_node
            ( (const search_tree_t *)&(p_cell_ctx->cmas_info_list)));
        if (PNULL != temp_cmas_info_node)
        {
            st_delete_node (&(p_cell_ctx->cmas_info_list),
                    &(temp_cmas_info_node->anchor));
            rrc_mem_free(temp_cmas_info_node);
            temp_cmas_info_node = PNULL;
        }
    }
    if (PNULL != p_cell_ctx)
    {
        rrc_mem_free(p_cell_ctx);
/* coverity_96750 start */
        p_cell_ctx = PNULL;
/* coverity_96750 stop */
    }
    p_csc_gb_context->p_p_cell_ctx[cell_index] = PNULL;
    /*Free cell context allocated at cell index*/
    p_csc_gb_context->num_allocated_cell_ctx_curr--;

    RRC_CSC_TRACE(RRC_INFO, "[CELL:%u] Context Deallocated.", cell_index);

    RRC_CSC_UT_TRACE_EXIT();
}



/******************************************************************************
*   FUNCTION NAME: csc_cell_ctx_out_of_service_cleanup
*   INPUT        :  csc_gl_ctx_t  *p_csc_gb_context
*   OUTPUT       :  None
*   RETURNS      : None
*   DESCRIPTION  : Cleans up CSC Cell context but paging is not deinitialized again.
******************************************************************************/
void csc_cell_ctx_out_of_service_cleanup(csc_cell_ctx_t *p_cell_ctx,
        csc_gl_ctx_t   *p_csc_gb_context  /* CSC global context */
        )
{
    rrc_cell_index_t cell_index;
    cmas_info_t *temp_cmas_info_node = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    if (PNULL == p_cell_ctx)
    {
        RRC_CSC_TRACE(RRC_ERROR, "[CELL] Context Not initialized");
        return;
    }

    cell_index = p_cell_ctx->cell_index;

    /* Free timers */
    if (PNULL != p_cell_ctx->guard_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF,
                "[CELL:%u] Stop timer [CSC_CELL_GUARD_TIMER]", cell_index);
        rrc_stop_timer(p_cell_ctx->guard_timer);
        p_cell_ctx->guard_timer = PNULL;
    }

    if (PNULL != p_cell_ctx->mac_sfn_sync_timer)
    {
        RRC_CSC_TRACE(RRC_BRIEF,
                "[CELL:%u] Stop timer [CELL_M_SFN_SYNC_TIMER]", cell_index);

        rrc_stop_timer(p_cell_ctx->mac_sfn_sync_timer);
        p_cell_ctx->mac_sfn_sync_timer = PNULL;
    }

    /* Stop SFN update process */
    sfn_m_stop_sfn(p_cell_ctx);

    /* Free Paging records storage */
    p_cell_ctx->p_temp_si_schdl_info = PNULL;
    

    if (PNULL != p_cell_ctx->p_enb_pws_req)
    {
        rrc_mem_free(p_cell_ctx->p_enb_pws_req);
        p_cell_ctx->p_enb_pws_req = PNULL;
    }

    if (PNULL != p_cell_ctx->p_enb_kill_req)
    {
        rrc_mem_free(p_cell_ctx->p_enb_kill_req);
        p_cell_ctx->p_enb_kill_req = PNULL;
    }

    p_cell_ctx->cmas_flag = RRC_FALSE;

    if (PNULL != p_cell_ctx->etws_info)
    {
        rrc_mem_free (p_cell_ctx->etws_info);
        p_cell_ctx->etws_info = PNULL;
    }
    while (st_get_node_count(
            (const search_tree_t *)&(p_cell_ctx->cmas_info_list)) > 0)
    {
        temp_cmas_info_node = (cmas_info_t *)(st_get_first_node
            ( (const search_tree_t *)&(p_cell_ctx->cmas_info_list)));
        if (PNULL != temp_cmas_info_node)
        {
            st_delete_node (&(p_cell_ctx->cmas_info_list),
                        &(temp_cmas_info_node->anchor));
            rrc_mem_free(temp_cmas_info_node);
            temp_cmas_info_node = PNULL;
        }
    }
    if (PNULL != p_cell_ctx)
    {
        rrc_mem_free(p_cell_ctx);
    }

    p_csc_gb_context->p_p_cell_ctx[cell_index] = PNULL;
    /*Free cell context allocated at cell index*/
    p_csc_gb_context->num_allocated_cell_ctx_curr--;

    RRC_CSC_TRACE(RRC_INFO, "[CELL:%u] Context Deallocated.", cell_index);

    RRC_CSC_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME:  csc_cell_ctx_db_init
*   INPUT        :  csc_gl_ctx_t  *p_csc_gb_context
*   OUTPUT       :  None
*   RETURNS: SUCCESS/FAILURE
*   DESCRIPTION:  This function initializes CELL contexts DB.
******************************************************************************/

rrc_return_t csc_cell_ctx_db_init(
    csc_gl_ctx_t  *p_csc_gb_context  /* CSC global context */
    )
{
    rrc_return_t result = RRC_FAILURE;
    csc_cell_ctx_t **p_contexts_db = PNULL;  /* Pointer on cell contexts DB */
    
    RRC_CSC_UT_TRACE_ENTER();
    
    /*Allocate memory */
    p_contexts_db = (csc_cell_ctx_t **)
        rrc_mem_get(sizeof(csc_cell_ctx_t*)* MAX_NUM_CELLS);
    if (PNULL == p_contexts_db)
    {
        RRC_CSC_TRACE(RRC_BRIEF,
            "[cell_DB] Initialization failed (contexts_db)");
    }
    else
    {
        memset_wrapper(p_contexts_db,0,sizeof(csc_cell_ctx_t*)* MAX_NUM_CELLS);
        p_csc_gb_context->p_p_cell_ctx = p_contexts_db;
        p_csc_gb_context->num_allocated_cell_ctx_curr = 0;
        RRC_CSC_TRACE(RRC_BRIEF, "[cell_DB] Initialized");
        result = RRC_SUCCESS;
    }
    RRC_CSC_UT_TRACE_EXIT();

    return result;
}


/*******************************************************************************
*   FUNCTION NAME:  csc_cell_ctx_db_free
*   INPUT        :  csc_gl_ctx_t  *p_csc_gb_context
*   OUTPUT       :  None
*   RETURNS      :  NONE
*   DESCRIPTION  :  This function free all cell contexts.
******************************************************************************/

void csc_cell_ctx_db_free(
        csc_gl_ctx_t  *p_csc_gb_context  /* CSC global context */
        )
{
    U8     cell_index = 0;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT ( PNULL != p_csc_gb_context );
    
    /* Cleanup CSC cell contexts */
    if ( PNULL != p_csc_gb_context->p_p_cell_ctx )
    {
        for(cell_index = 0; cell_index < MAX_NUM_CELLS; cell_index++)
        {
            if(PNULL != p_csc_gb_context->p_p_cell_ctx[cell_index])
            {
                csc_cell_ctx_cleanup(p_csc_gb_context,
                        p_csc_gb_context->p_p_cell_ctx[cell_index]);
            }
        }
    }
    if(0 == p_csc_gb_context->num_allocated_cell_ctx_curr)
    {
        /*Free allocated memory */
        if(PNULL != p_csc_gb_context->p_p_cell_ctx)
        {
            rrc_mem_free(p_csc_gb_context->p_p_cell_ctx);
            p_csc_gb_context->p_p_cell_ctx = PNULL;
            RRC_CSC_TRACE(RRC_BRIEF, "[CELL_DB] Cleaned up");
        }
    }

    RRC_CSC_UT_TRACE_EXIT();
}
