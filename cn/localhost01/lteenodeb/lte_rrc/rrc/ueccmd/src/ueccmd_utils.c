/******************************************************************************
*
*   FILE NAME:
*       ueccmd_utils.c
*
*   DESCRIPTION:
*       This header file contains implementation of UECCMD specific utilities
*
*   Copyright (c) 2012, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include "ueccmd_utils.h"
#include "ueccmd_logger.h"
#include "rrc_common_utils.h"
#include "rrc_msg_mgmt.h"
#include "rrc_oamh_intf.h"
#include "ueccmd_global_ctx.h"
#include "rrc_csc_uecc_intf.h"
#include "rrc_uecc_llim_intf.h"

/*Used for tree comparison*/
#define COMPARE_ERROR               -1
#define FIRST_LESS_THAN_SECOND      -1
#define FIRST_EQUAL_TO_SECOND       0
#define FIRST_GREATER_THAN_SECOND   1

/******************************************************************************
*   FUNCTION NAME: ueccmd_alloc_intrl_msg
*   INPUT        : rrc_module_id_t             src_module_id
*                  rrc_module_id_t             dst_module_id
*                  U16                         api_id
*                  rrc_size_t                  msg_size
*   OUTPUT       : none
*   RETURNS      : PNULL in failure case
*                  pointer on the very begining of message
*   DESCRIPTION  : Allocate memory and fill RRC API Header for sending message from UECCMD.
*                  Is used for sending messages to other RRC submodules.
******************************************************************************/
void* ueccmd_alloc_intrl_msg
(
    /*Source module identifier */
    rrc_module_id_t             src_module_id,
    /* Destination module identifier */
    rrc_module_id_t             dst_module_id,
    U16                         api_id,         /* API Id */
    rrc_size_t                  msg_size        /* Size of message (payload) */
)
{
    void *p_api = PNULL;
    rrc_size_t msg_api_length = (rrc_size_t)(msg_size + RRC_API_HEADER_SIZE);

    RRC_UECCMD_UT_TRACE_ENTER();

    /* Allocate buffer */
    p_api = rrc_msg_mem_get(msg_api_length);
    if (PNULL != p_api)
    {
        memset_wrapper(p_api, 0, msg_api_length);

        /* Fill RRC header */
        rrc_construct_api_header(   p_api, RRC_VERSION_ID, src_module_id,
                                    dst_module_id, api_id, (U16)msg_api_length);
    }

    RRC_UECCMD_UT_TRACE_EXIT();
    return p_api;
}
/******************************************************************************
 * *   FUNCTION NAME: ueccmd_broadcast_msg
 * *   INPUT        : void            *p_api
 *                    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
 *     OUTPUT       : none
 * *   RETURNS      : None
 * *   DESCRIPTION  : This function broadcasts message to all the instances of UECC.
 * ******************************************************************************/
void ueccmd_broadcast_msg
(
 void            *p_api,           /* Pointer to input API buffer */
 ueccmd_gl_ctx_t *p_ueccmd_gl_ctx  /* Pointer to the UECCMD global context */
 )
{
    U8         uecc_index = RRC_NULL;
    void       *p_intrl_msg = PNULL;

    RRC_UECCMD_UT_TRACE_ENTER();

    /* Sending the same message to all the instances of UECC */
    for (uecc_index = 0; uecc_index < p_ueccmd_gl_ctx->num_uecc_instances;
            uecc_index++)
    {
        p_intrl_msg = (void *)ueccmd_alloc_intrl_msg(RRC_NULL,
                RRC_NULL,
                RRC_NULL, rrc_get_api_buf_size(p_api));

        if (PNULL != p_intrl_msg)
        {
            l3_memcpy_wrapper(p_intrl_msg, p_api, rrc_get_api_buf_size(p_api));

            rrc_send_message (p_intrl_msg, (rrc_module_id_t)(RRC_UECC_1_MODULE_ID + uecc_index));
        }
    }

    RRC_UECCMD_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: ueccmd_round_robin_ue_allocation_to_uecc
*   INPUT        : void            *p_api
*                  ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
*   OUTPUT       : none
*   RETURNS      : None
*   DESCRIPTION  : This function forward the message to a UECC based 
*                  on round robin algo
******************************************************************************/
void ueccmd_round_robin_ue_allocation_to_uecc
(
    void            *p_api,           /* Pointer to input API buffer */
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx  /* Pointer to the UECCMD global context */
)
{
    void       *p_intrl_msg = PNULL;

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) <= RRC_API_HEADER_SIZE + 
            sizeof(rrc_uecc_llim_ccch_data_ind_t))
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }

    p_intrl_msg = (void *)ueccmd_alloc_intrl_msg(RRC_NULL,
            RRC_NULL,
            RRC_NULL, rrc_get_api_buf_size(p_api));

    if (PNULL != p_intrl_msg)
    {
        l3_memcpy_wrapper(p_intrl_msg, p_api, rrc_get_api_buf_size(p_api));

        /* If currently message is sent to last UECC instance
         * then next time message will be sent to first UECC instance 
         * as number of UECC instances are 0-7 */
        rrc_send_message(p_intrl_msg, (rrc_module_id_t)(RRC_UECC_1_MODULE_ID 
                    + (p_ueccmd_gl_ctx->not_allocated_uecc_index_cur)%(p_ueccmd_gl_ctx->num_uecc_instances)));
        
        /* Bug 8871 fix start */
        if ( 1 != p_ueccmd_gl_ctx->num_uecc_instances )
        {
            p_ueccmd_gl_ctx->not_allocated_uecc_index_cur++;
        }
        /* Bug 8871 fix stop */
    }

    RRC_UECCMD_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: ueccmd_get_cell_index
*   INPUT        : void                      *p_api
*                  U8                         current_proc 
*   OUTPUT       : none
*   RETURNS      : rrc_cell_index_t
*   DESCRIPTION  : This function gets the cell_index from p_api
*                  according to message received.
*
******************************************************************************/
rrc_cell_index_t ueccmd_get_cell_index
(
    void                      *p_api,           /* Pointer to input API buffer */
    U8                         current_proc     /* Bitmask value for Procedure */
)
{
    rrc_cell_index_t      cell_index = RRC_INVALID_CELL_INDEX;

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT (p_api != PNULL);

    switch(current_proc)
    {
        case UECCMD_GET_CELL_STATUS_PROC_RUNNING:

            if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
                    (rrc_uecc_oamh_get_cell_status_req_t))
            {
                RRC_UECCMD_TRACE(RRC_WARNING, "Wrong message size - ignore.");
                break;
            }

            /* Get message payload */
            cell_index = ((rrc_uecc_oamh_get_cell_status_req_t *)((U8* )p_api +
                        RRC_API_HEADER_SIZE))->cell_index;
            break;

        case UECCMD_DELETE_ALL_UE_PROC_RUNNING:

            if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
                    (rrc_csc_uecc_delete_all_ue_req_t))
            {
                RRC_UECCMD_TRACE(RRC_WARNING, "Wrong message size - ignore.");
                break;
            }

            /* Get message payload */
            cell_index = ((rrc_csc_uecc_delete_all_ue_req_t *)((U8* )p_api +
                        RRC_API_HEADER_SIZE))->cell_index;
            break;

        case UECCMD_TRAFFIC_TRACE_START_PROC_RUNNING:

            if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
                    (rrc_uecc_oamh_cell_traffic_trace_start_t))
            {
                RRC_UECCMD_TRACE(RRC_WARNING, "Wrong message size - ignore.");
                break;
            }

            /* Get message payload */
            cell_index = ((rrc_uecc_oamh_cell_traffic_trace_start_t *)((U8* )p_api +
                        RRC_API_HEADER_SIZE))->cell_index;
            break;

        case UECCMD_GET_UE_STATUS_PROC_RUNNING:

            if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
                    (rrc_uecc_oamh_get_ue_status_req_t))
            {
                RRC_UECCMD_TRACE(RRC_WARNING, "Wrong message size - ignore.");
                break;
            }

            /* Get message payload */
            cell_index = ((rrc_uecc_oamh_get_ue_status_req_t *)((U8* )p_api +
                        RRC_API_HEADER_SIZE))->cell_index;
            break;
        default:
                RRC_UECCMD_TRACE(RRC_WARNING, "Invalid Procedure for Cell");
    }

    return cell_index;
}

/******************************************************************************
*   FUNCTION NAME: ueccmd_check_cell_state_and_broadcast_msg
*   INPUT        : ueccmd_gl_ctx_t           *p_ueccmd_gl_ctx
*                  void                      *p_api
*                  U8                         current_proc
*   OUTPUT       : none
*   RETURNS      : rrc_bool_t
*   DESCRIPTION  : This function checks the current running proc of cell and if no 
*                  procedure is running then set current proc
******************************************************************************/
void ueccmd_check_cell_state_and_broadcast_msg
(
    ueccmd_gl_ctx_t           *p_ueccmd_gl_ctx,  /* Pointer to the UECCMD global context */
    void                      *p_api,           /* Pointer to input API buffer */
    U8                         current_proc     /* Bitmask value for Procedure */
)
{
    rrc_cell_index_t      cell_index = RRC_INVALID_CELL_INDEX;

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT (p_ueccmd_gl_ctx != PNULL);
    RRC_ASSERT (p_api != PNULL);

    cell_index =  ueccmd_get_cell_index(p_api,current_proc);

    SET_CELL_INDEX(cell_index);

    if ((cell_index == RRC_INVALID_CELL_INDEX) ||
            (MAX_NUM_CELLS <= cell_index))
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Invalid Cell Index: %d", cell_index);
        RRC_UECCMD_UT_TRACE_EXIT();
        return ;
    }

    if ( ((p_ueccmd_gl_ctx->ueccmd_cell_ctx[cell_index].bitmask) &  
            current_proc) == current_proc)
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Procedure already running for Cell: %d"
                " Ignore..", cell_index);
        RRC_UECCMD_UT_TRACE_EXIT();
        return ;
    }

    p_ueccmd_gl_ctx->ueccmd_cell_ctx[cell_index].bitmask |= current_proc;

    ueccmd_broadcast_msg (p_api, p_ueccmd_gl_ctx);

    RRC_UECCMD_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: compare_ue_ctx_search_tree
*   INPUT        : const void * p_elem1
*                  const void * p_elem2
*   OUTPUT       : none
*   RETURNS      : S32
*   DESCRIPTION  : This function compares the values of two keys
*                  in a search tree
******************************************************************************/
S32 compare_ue_ctx_search_tree
(
    /* Pointer to first element being compared */
    const void * p_elem1 ,
    /* Pointer to second element being compared */
    const void * p_elem2
)
{
    if (p_elem1 == PNULL || p_elem2 == PNULL)
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "%s: passed null arg",__FUNCTION__);
        return COMPARE_ERROR;
    }
/* Reestablishment_FIX start */
    if ( * ((const U16 *) p_elem1) == * ((const U16 *) p_elem2))
    {
        return FIRST_EQUAL_TO_SECOND;
    }
    else if ( * ((const U16 *) p_elem1) < * ((const U16 *) p_elem2))
    {
        return FIRST_LESS_THAN_SECOND;
    }
/* Reestablishment_FIX stop */
    else
    {
        return FIRST_GREATER_THAN_SECOND;
    }
}


/******************************************************************************
 *   FUNCTION NAME: key_of_ue_ctx_search_tree
 *   INPUT        : const YTNODE * p_node
 *   OUTPUT       : none
 *   RETURNS      : void 
 *   DESCRIPTION  : This function returns the key of node in search tree
 *
 ******************************************************************************/
const void * key_of_ue_ctx_search_tree
(
     /* Pointer to Tree node */
     const YTNODE * p_node
)
{
    return (p_node) ? &(((ueccmd_ue_ctx_release_node_t *)p_node)->mme_ue_id) : PNULL;
}

/******************************************************************************
 *   FUNCTION NAME: key_of_ue_conn_reestab_search_tree
 *   INPUT        : const YTNODE * p_node
 *   OUTPUT       : none
 *   RETURNS      : void 
 *   DESCRIPTION  : This function returns the key of ue_conn_reestab_search_tree 
 *                  node
 *
 ******************************************************************************/
const void * key_of_ue_conn_reestab_search_tree
(
     /* Pointer to Tree node */
     const YTNODE * p_node
)
{
    return (p_node) ? &(((ueccmd_ue_conn_reestab_node_t *)p_node)->rnti) : PNULL;
}
