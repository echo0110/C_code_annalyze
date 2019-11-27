/******************************************************************************
*
* ARICENT -
*
* Copyright (C) 2012 Aricent Inc . All Rights Reserved.
*
*******************************************************************************
*
* $$Id: son_anr_enb_ctxt_mgr.c $
*
*******************************************************************************
*
* File Description:  This file contains the functions for managing
*                     Pending Response context used in TNL discovery and
*                     X2 setup/release.
*
*******************************************************************************
* Revision Details
*----------------------
*   DATE            AUTHOR      REFERENCE       REASON
*  Sept, 2012     Komal/Shekhar                 Initial
*  July, 2014     Shilpi                        SPR 10331 Fix
******************************************************************************/

/****************************************************************************
* Project Includes
****************************************************************************/
#include <son_utils.h>
#include <son_anr_enb_ctxt_mgr.h>
#include <son_x2_ctxt_mgr.h>
#include <son_enb_table.h>
#include <son_defines.h>
#include <son_rrm_intf.h>
#include <son_es_intf.h>
#include <son_anr_utils.h>
#include <rrm_son_composer.h>
/* Spr 16142 Fix Start */
#include <son_rrm_composer.h>
/* Spr 16142 Fix End */

/****************************************************************************
* Global Variables
****************************************************************************/
extern const son_8 *p_son_anr_facility_name;
extern son_8 *p_son_x2_facility_name;

/*****************************************************************************
 * Function Name  : anr_add_x2_pending_resp_list
 * Inputs         : tid - Transaction Id
 *                  api - API Id
 * Outputs        : None
 * Returns        : anr_enb_pending_res_data_t * -Pointer to the node inserted
 * Description    : This function adds element to X2 Pending Response List
 ****************************************************************************/
anr_enb_pending_res_data_t *
anr_add_x2_pending_resp_list
(
    son_u16 tid,
    son_procedure_code_et api
)
{
    anr_enb_pending_res_node_t *p_list_node = SON_PNULL;
    anr_enb_pending_res_data_t *p_ret_val   = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_list_node = son_mem_get(sizeof(anr_enb_pending_res_node_t));
    if (SON_PNULL != p_list_node)
    {
        p_list_node->data.api_id = api;
        p_list_node->data.transaction_id = tid;
        p_list_node->data.expected_res_count = 0;
        p_list_node->data.switch_on_resp_received = SON_FALSE;
        p_list_node->data.switch_on_sent = SON_FALSE;
        list_init(&p_list_node->data.pending_nbr_enb_list);
        list_insert_node(anr_get_x2_pending_res_list(),
            &p_list_node->anchor);

        p_ret_val = &p_list_node->data;
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_DETAILED,
            "Added API [API Id: %d]"
            " and TID [Trans Id: %d] in X2 Pending Response", api, tid);
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR,
            "anr_add_x2_pending_resp_list: Memory allocation failure");
    }

    SON_UT_TRACE_EXIT();
    return p_ret_val;
} /* anr_add_x2_pending_resp_list */

/*****************************************************************************
 * Function Name  : anr_key_of_x2_list
 * Inputs         : p_node - anchor field of the X2 Pending Res list record
 * Outputs        : None
 * Returns        : son_void_t * - pointer to transaction Id as void pointer
 * Description    : This function determines the key of X2 Pending Res list
 ******************************************************************************/
const son_void_t *
anr_key_of_x2_list
(
    const SON_LIST_NODE *p_node
)
{
    anr_enb_pending_res_node_t *p_data = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_data = YMEMBEROF(anr_enb_pending_res_node_t, anchor, p_node);

    SON_UT_TRACE_EXIT();
    return (son_void_t *)(&p_data->data.transaction_id);
}

/*****************************************************************************
 * Function Name  : anr_key_of_enb_list
 * Inputs         : p_node - anchor field of the X2 Pending Res list record
 * Outputs        : None
 * Returns        : son_void_t * - pointer to transaction Id as void pointer
 * Description    : This function determines the key of eNB Pending Res list
 ******************************************************************************/
const son_void_t *
anr_key_of_enb_list
(
    const SON_LIST_NODE *p_node
)
{
    anr_pending_nbr_enb_info_node_t *p_data = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_data = YMEMBEROF(anr_pending_nbr_enb_info_node_t, anchor, p_node);

    SON_UT_TRACE_EXIT();
    return (son_void_t *)(&p_data->data.nb_enb.enb_id);
}

/*****************************************************************************
 * Function Name  : anr_compare_enb_id
 * Inputs         : p_val_1    - Pointer to the eNB Id as void pointer
 *                : p_val_2    - Pointer to the eNB Id as void pointer
 * Outputs        : None
 * Returns        : Zero -  If matches
 *                  Non Zero - If not matches
 * Description    : This function compares the eNB Ids
 ******************************************************************************/
son_s32
anr_compare_enb_id
(
    const son_void_t *p_val_1,
    const son_void_t *p_val_2
)
{
    son_s32 ret_value = -1;


    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR,
            "anr_compare_enb_id: Pointer to the eNB Id is NULL");
    }
    else
    {
        son_global_enb_id_t *p_enb_id_1 = (son_global_enb_id_t *)p_val_1;
        son_global_enb_id_t *p_enb_id_2 = (son_global_enb_id_t *)p_val_2;

        ret_value = SON_MEMCMP(p_enb_id_1, p_enb_id_2, sizeof(son_global_enb_id_t));
    }

    SON_UT_TRACE_EXIT();
    return ret_value;
} /* anr_compare_enb_id */

/*****************************************************************************
 * Function Name  : anr_find_x2_pending_resp_list
 * Inputs         : tid - Transaction Id
 * Outputs        : None
 * Returns        : anr_enb_pending_res_node_t * - Pointer to the node found
 * Description    : This function finds element in X2 Pending Response List
 *                  with matching tid
 ****************************************************************************/
anr_enb_pending_res_node_t *
anr_find_x2_pending_resp_list
(
    son_u16 tid
)
{
    SON_LIST_NODE                               *p_node                 = SON_PNULL;
    anr_enb_pending_res_node_t  *p_list_node    = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_node = list_find_node(anr_get_x2_pending_res_list(),
        (son_void_t *)&tid, anr_key_of_x2_list,
        anr_compare_txn_id);
    if (SON_PNULL != p_node)
    {
        p_list_node = YMEMBEROF(anr_enb_pending_res_node_t, anchor, p_node);
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_DETAILED,
            "Found API [API Id: %d] with "
            "[Trans Id: %d] in Pending Response",
            p_list_node->data.api_id, tid);
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_DETAILED,
            "No node for TID "
            "[Trans Id: %d] found in Pending Response",
            tid);
    }

    SON_UT_TRACE_EXIT();
    return p_list_node;
} /* anr_find_x2_pending_resp_list */

/*****************************************************************************
 * Function Name  : anr_del_x2_pending_resp_list
 * Inputs         : tid - Transaction Id
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function deletes element from X2 Pending Response List
 *                  with the matching TID.
 ****************************************************************************/
son_void_t
anr_del_x2_pending_resp_list
(
    son_u16 tid
)
{
    anr_enb_pending_res_node_t      *p_node = SON_PNULL;


    SON_UT_TRACE_ENTER();
    p_node = anr_find_x2_pending_resp_list(tid);

    if (SON_PNULL != p_node)
    {
        list_delete_node(anr_get_x2_pending_res_list(),
            &p_node->anchor);

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_DETAILED,
            "Deleted API [API Id: %d] with "
            "[Trans Id: %d] from Pending Response",
            p_node->data.api_id, tid);
        /*Coverity 83368 Fix Start*/
        son_mem_free(p_node);
        /*Coverity 83368 Fix End*/
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_DETAILED,
            "Node for TID [Trans Id: %d]"
            " not found", tid);
    }

    SON_UT_TRACE_EXIT();
} /* anr_del_x2_pending_resp_list */

/*****************************************************************************
 * Function Name  : anr_find_enb_x2_pending_resp_list
 * Inputs         : p_enb_id - Pointer to eNB Id
 * Outputs        : p_pending_resp - Double Pointer to X2 pending response
 *                                                                      element in which eNB is present
 * Returns        : anr_pending_nbr_enb_info_data_t * - Pointer to eNB info
 *                                  record available in pending response.
 * Description    : This function finds enB in X2 Pending Response List
 ****************************************************************************/
anr_pending_nbr_enb_info_data_t *
anr_find_enb_x2_pending_resp_list
(
    son_global_enb_id_t *p_enb_id,
    anr_enb_pending_res_data_t * *p_pending_resp
)
{
    anr_pending_nbr_enb_info_data_t *p_list_node                         = SON_PNULL;
    anr_enb_pending_res_node_t          *p_pending_res_list_node = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_enb_id || SON_PNULL == p_pending_resp)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR,
            "eNB could not be found as p_enb_id or p_pending_resp is SON_NULL");

        SON_UT_TRACE_EXIT();
        return p_list_node;
    }

    p_pending_res_list_node = anr_get_first_x2_pending_resp_list();
    while (SON_NULL != p_pending_res_list_node)
    {
        p_list_node = anr_find_enb_x2_pending_resp_elem(&p_pending_res_list_node->data,
            p_enb_id);

        if (SON_PNULL != p_list_node)
        {
            *p_pending_resp = &p_pending_res_list_node->data;

            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_BRIEF,
                "eNB [eNB Id: 0x%x]]"
                " found in pending response for API [API Id: %d] and TID [Trans Id: %d]",
                son_convert_char_cell_id_to_int_cell_id(
                    p_enb_id->enb_id), (*p_pending_resp)->api_id, (*p_pending_resp)->transaction_id);

            break;
        }

        p_pending_res_list_node = anr_get_next_x2_pending_resp_list(p_pending_res_list_node);
    }

    SON_UT_TRACE_EXIT();
    return p_list_node;
} /* anr_find_enb_x2_pending_resp_list */

/*****************************************************************************
 * Function Name  : anr_add_enb_x2_pending_resp_elem
 * Inputs         : p_pending_resp - Pointer to X2 pending Response element
 *                                : p_enb_info	   - Pointer to eNB Info of eNB to be added
 *                                : p_enb_rec	   - POinter to Hash Table rec of the eNB to be added
 *                                : error_code	   - Error Code to be set
 * Outputs        : None
 * Returns        : anr_pending_nbr_enb_info_data_t* - Pointer to eNB record
 *                                   added in the X2 pending response.
 * Description    : This function add eNB in X2 Pending Response element
 ****************************************************************************/
anr_pending_nbr_enb_info_data_t *
anr_add_enb_x2_pending_resp_elem
(
    anr_enb_pending_res_data_t *p_pending_resp,
    son_neighbor_enb_info_t *p_enb_info,
    son_nbr_enb_hash_rec_t *p_enb_rec,
    son_error_et error_code
)
{
    anr_pending_nbr_enb_info_data_t *p_node = SON_PNULL;
    anr_pending_nbr_enb_info_node_t *p_list_node = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_pending_resp || SON_PNULL == p_enb_info)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR,
            "eNB could not be added to pending response as p_enb_info or p_pending_resp is SON_NULL");

        SON_UT_TRACE_EXIT();
        return p_node;
    }

    p_list_node = son_mem_get(sizeof(anr_pending_nbr_enb_info_node_t));
    if (SON_PNULL != p_list_node)
    {
        p_list_node->data.error_code = error_code;
        p_list_node->data.expiry_time = 0;
        p_list_node->data.p_enb_rec = p_enb_rec;
        SON_MEMCPY(&p_list_node->data.nb_enb, p_enb_info, sizeof(son_neighbor_enb_info_t));

        list_insert_node(&p_pending_resp->pending_nbr_enb_list,
            &p_list_node->anchor);

        p_node = &p_list_node->data;

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_BRIEF,
            "Added eNB [eNB Id: 0x%x]]"
            " in X2 Pending Response of API [API Id: %d] and TID [Trans Id: %d]",
            son_convert_char_cell_id_to_int_cell_id(p_node->nb_enb.enb_id.enb_id),
            p_pending_resp->api_id,
            p_pending_resp->transaction_id);
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR,
            "anr_add_enb_x2_pending_resp_elem: Memory allocation failure");
    }

    SON_UT_TRACE_EXIT();
    return p_node;
} /* anr_add_enb_x2_pending_resp_elem */

/*****************************************************************************
 * Function Name  : anr_del_enb_x2_pending_resp_elem
 * Inputs         : p_pending_resp - Pointer to X2 pending Response element
 *                                : p_enb_id	   - Pointer to eNB Id of eNB to be deleted
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function deletes eNB record in
 *                  X2 Pending Response element
 ****************************************************************************/
son_void_t
anr_del_enb_x2_pending_resp_elem
(
    anr_enb_pending_res_data_t *p_pending_resp,
    son_global_enb_id_t *p_enb_id
)
{
    anr_pending_nbr_enb_info_node_t *p_tpl = SON_PNULL;
    SON_LIST_NODE                   *p_node = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_pending_resp || SON_PNULL == p_enb_id)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR,
            "eNB could not be deleted to pending response as p_enb_id or p_pending_resp is SON_NULL");

        SON_UT_TRACE_EXIT();
        return;
    }

    p_node = list_find_node(&p_pending_resp->pending_nbr_enb_list,
        (son_void_t *)p_enb_id, anr_key_of_enb_list,
        anr_compare_enb_id);
    if (SON_PNULL != p_node)
    {
        p_tpl = YMEMBEROF(anr_pending_nbr_enb_info_node_t, anchor, p_node);

        list_delete_node(&p_pending_resp->pending_nbr_enb_list,
            p_node);

        son_mem_free(p_tpl);

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_BRIEF,
            "Deleted eNB [eNB Id: 0x%x]] from API [API Id: %d] with "
            "[Trans Id: %d] from Pending Response",
            son_convert_char_cell_id_to_int_cell_id(p_enb_id->enb_id),
            p_pending_resp->api_id, p_pending_resp->transaction_id);
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_BRIEF,
            "Node for eNB [eNB Id: 0x%x]]"
            " not found", son_convert_char_cell_id_to_int_cell_id(p_enb_id->enb_id));
    }

    SON_UT_TRACE_EXIT();
} /* anr_del_enb_x2_pending_resp_elem */

/*****************************************************************************
 * Function Name  : anr_find_enb_x2_pending_resp_elem
 * Inputs         : p_pending_resp - Pointer to X2 pending Response element
 *                                : p_enb_id	   - Pointer to eNB Id of eNB to be found
 * Outputs        : None
 * Returns        : anr_pending_nbr_enb_info_data_t* -
 *                                   Pointer to the eNB record available
 *                                   in the pending response.
 * Description    : This function find eNB in X2 Pending Response element
 ****************************************************************************/
anr_pending_nbr_enb_info_data_t *
anr_find_enb_x2_pending_resp_elem
(
    anr_enb_pending_res_data_t *p_pending_resp,
    son_global_enb_id_t *p_enb_id
)
{
    SON_LIST_NODE                                               *p_node                 = SON_PNULL;
    anr_pending_nbr_enb_info_node_t     *p_list_node    = SON_PNULL;
    anr_pending_nbr_enb_info_data_t     *p_ret_val      = SON_PNULL;


    SON_UT_TRACE_ENTER();
    if (SON_PNULL == p_pending_resp || SON_PNULL == p_enb_id)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR,
            "eNB could not be found in pending response as p_enb_id or p_pending_resp is SON_NULL");

        SON_UT_TRACE_EXIT();
        return p_ret_val;
    }

    p_node = list_find_node(&p_pending_resp->pending_nbr_enb_list,
        (son_void_t *)p_enb_id, anr_key_of_enb_list,
        anr_compare_enb_id);
    if (SON_PNULL != p_node)
    {
        p_list_node = YMEMBEROF(anr_pending_nbr_enb_info_node_t, anchor, p_node);
        p_ret_val = &p_list_node->data;

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_BRIEF,
            "Found eNB [eNB Id: 0x%x]] in API [API Id: %d] with "
            "[Trans Id: %d] in Pending Response",
            son_convert_char_cell_id_to_int_cell_id(
                p_enb_id->enb_id), p_pending_resp->api_id, p_pending_resp->transaction_id);
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_BRIEF,
            "No node for eNB "
            "[eNB Id: 0x%x]] found in API [API Id: %d] with [Trans Id: %d] in Pending Response",
            son_convert_char_cell_id_to_int_cell_id(
                p_enb_id->enb_id), p_pending_resp->api_id, p_pending_resp->transaction_id);
    }

    SON_UT_TRACE_EXIT();
    return p_ret_val;
} /* anr_find_enb_x2_pending_resp_elem */

/*****************************************************************************
 * Function Name  : anr_get_first_x2_pending_resp_list
 * Inputs         : None
 * Outputs        : None
 * Returns        : anr_enb_pending_res_node_t* - Returns pointer
 *                      to the first element in list
 * Description    : This function gets first element from X2 Pending Response list
 ****************************************************************************/
anr_enb_pending_res_node_t *
anr_get_first_x2_pending_resp_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_LIST_NODE   *p_node = SON_PNULL;
    SON_LIST *p_list = anr_get_x2_pending_res_list();


    SON_UT_TRACE_ENTER();

    if (list_count(p_list) > 0)
    {
        p_node = get_first_list_node(p_list);
        if (SON_PNULL != p_node)
        {
            SON_UT_TRACE_EXIT();
            return (anr_enb_pending_res_node_t *)
                   YMEMBEROF(anr_enb_pending_res_node_t, anchor, p_node);
        }
    }

    SON_UT_TRACE_EXIT();
    return SON_PNULL;
} /* anr_get_first_x2_pending_resp_list */

/*****************************************************************************
 * Function Name  : anr_get_next_x2_pending_resp_list
 * Inputs         : p_node - Pointer to the current node
 * Outputs        : None
 * Returns        : anr_enb_pending_res_node_t * -
 *                  Returns pointer to the next element in list
 * Description    : This function gets next element from X2 Pending Response list
 ****************************************************************************/
anr_enb_pending_res_node_t *
anr_get_next_x2_pending_resp_list
(
    anr_enb_pending_res_node_t *p_node
)
{
    SON_LIST_NODE   *p_list_node    = SON_PNULL;


    SON_UT_TRACE_ENTER();
    if (SON_PNULL != p_node)
    {
        p_list_node = get_next_list_node(&p_node->anchor);
        if (SON_PNULL != p_list_node)
        {
            return (anr_enb_pending_res_node_t *)
                   YMEMBEROF(anr_enb_pending_res_node_t, anchor, p_list_node);
        }
    }

    SON_UT_TRACE_EXIT();
    return SON_PNULL;
} /* anr_get_next_x2_pending_resp_list */

/*****************************************************************************
 * Function Name  : anr_get_first_enb_x2_pending_resp_elem
 * Inputs         : p_pending_resp - Pointer to the X2 Pending response element
 * Outputs        : None
 * Returns        : anr_pending_nbr_enb_info_node_t * - Returns pointer
 *                                                  to the first element in list
 * Description    : This function gets first element from eNB list
 *                  in X2 Pending Response element
 ****************************************************************************/
anr_pending_nbr_enb_info_node_t *
anr_get_first_enb_x2_pending_resp_elem
(
    anr_enb_pending_res_data_t *p_pending_resp
)
{
    SON_LIST_NODE   *p_node = SON_PNULL;
    SON_LIST                *p_list = &p_pending_resp->pending_nbr_enb_list;


    SON_UT_TRACE_ENTER();

    if (list_count(p_list) > 0)
    {
        p_node = get_first_list_node(p_list);
        if (SON_PNULL != p_node)
        {
            SON_UT_TRACE_EXIT();
            return (anr_pending_nbr_enb_info_node_t *)
                   YMEMBEROF(anr_pending_nbr_enb_info_node_t, anchor, p_node);
        }
    }

    SON_UT_TRACE_EXIT();
    return SON_PNULL;
} /* anr_get_first_enb_x2_pending_resp_elem */

/*****************************************************************************
 * Function Name  : anr_get_next_enb_x2_pending_resp_elem
 * Inputs         : p_node - Pointer to the eNB node in eNB List in X2 Pending response element
 * Outputs        : None
 * Returns        : Returns pointer to the next element in list
 * Description    : This function gets next element from eNB list in X2 Pending Response element
 ****************************************************************************/
anr_pending_nbr_enb_info_node_t *
anr_get_next_enb_x2_pending_resp_elem
(
    anr_pending_nbr_enb_info_node_t *p_node
)
{
    SON_LIST_NODE   *p_list_node    = SON_PNULL;


    SON_UT_TRACE_ENTER();
    if (SON_PNULL != p_node)
    {
        p_list_node = get_next_list_node(&p_node->anchor);
        if (SON_PNULL != p_list_node)
        {
            SON_UT_TRACE_EXIT();
            return (anr_pending_nbr_enb_info_node_t *)
                   YMEMBEROF(anr_pending_nbr_enb_info_node_t, anchor, p_list_node);
        }
    }

    SON_UT_TRACE_EXIT();
    return SON_PNULL;
} /* anr_get_next_enb_x2_pending_resp_elem */

/******************************************************************************
* Function Name  : anr_clear_x2_pending_resp_list
* Inputs         : None
* Outputs        : None
* Returns        : son_void_t
* Description    : This function clears the entire X2 Pending Response list
******************************************************************************/
son_void_t
anr_clear_x2_pending_resp_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    SON_LIST_NODE *p_list_node = SON_PNULL;
    SON_LIST      *p_list = anr_get_x2_pending_res_list();


    SON_UT_TRACE_ENTER();

    p_list_node = list_pop_node(p_list);

    while (p_list_node)
    {
        anr_enb_pending_res_node_t *p_data = YMEMBEROF(anr_enb_pending_res_node_t,
            anchor, p_list_node);

        anr_clear_enb_list_x2_pending_resp_elem(&p_data->data.pending_nbr_enb_list);

        /* Free the memory occupied by the node */
        son_mem_free(p_data);

        p_list_node = list_pop_node(p_list);
    }

    SON_UT_TRACE_EXIT();
} /* anr_clear_x2_pending_resp_list */

/******************************************************************************
* Function Name  : anr_clear_enb_list_x2_pending_resp_elem
* Inputs         : p_list - Pointer to the list
* Outputs        : None
* Returns        : son_void_t
* Description    : This function clears the eNB List in X2 Pending Response element
******************************************************************************/
son_void_t
anr_clear_enb_list_x2_pending_resp_elem
(
    SON_LIST *p_list
)
{
    SON_LIST_NODE *p_list_node = SON_PNULL;
    anr_pending_nbr_enb_info_node_t *p_data = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_list_node = list_pop_node(p_list);

    while (p_list_node)
    {
        p_data = YMEMBEROF(anr_pending_nbr_enb_info_node_t,
            anchor, p_list_node);

        /* Free the memory occupied by the node */
        son_mem_free(p_data);

        p_list_node = list_pop_node(p_list);
    }

    SON_UT_TRACE_EXIT();
} /* anr_clear_enb_list_x2_pending_resp_elem */

/*****************************************************************************
 * Function Name  : anr_construct_send_message_to_x2
 * Inputs         : p_pending_resp - Pointer to the X2 Pending response element
 * Outputs        : None
 * Returns        : None
 * Description    : This function constructs and sends message to SON_X2 if required
 ****************************************************************************/
son_void_t
anr_construct_send_message_to_x2
(
    anr_enb_pending_res_data_t *p_pending_resp
)
{
    anr_pending_nbr_enb_info_node_t *p_list_node = SON_PNULL;
    anr_pending_nbr_enb_info_data_t *p_data = SON_PNULL;
    son_x2_add_enb_req_t add_req;
    son_x2_delete_enb_req_t del_req;
    son_es_cell_switchon_for_x2_setup_req_t switch_on_req = {0};


    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_pending_resp)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR,
            "Message to X2 could not be sent"
            " as p_pending_resp is SON_NULL");

        SON_UT_TRACE_EXIT();
        return;
    }

    SON_MEMSET(&add_req, 0, sizeof(son_x2_add_enb_req_t));
    SON_MEMSET(&del_req, 0, sizeof(son_x2_delete_enb_req_t));

    /* Traverse eNB List in X2 Pending Response element */
    p_list_node = anr_get_first_enb_x2_pending_resp_elem(p_pending_resp);

    while (SON_PNULL != p_list_node)
    {
        p_data = &p_list_node->data;

        /* If No error has been set, then process this message for X2 */
        if (SON_NO_ERROR == p_data->error_code)
        {
            /* If X2 connection establishment is requested */
            if ((SON_X2_CONNECT == p_data->nb_enb.x2_connection_status ||
                 SON_X2_CONNECT_WHITELIST == p_data->nb_enb.x2_connection_status) &&
                SON_PNULL != p_data->p_enb_rec)
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_INFO,
                    "X2 connection establishment is requested");

                /* If X2 Connection is already established or establishment in progress or
                 * termination in progress, reject the request */
                if (X2_ENB_STATE_DISCONNECTED != p_data->p_enb_rec->enb_curr_fsm_state)
                {
                    son_error_et error_code = SON_NO_ERROR;

                    if (X2_ENB_STATE_CONNECTING == p_data->p_enb_rec->enb_curr_fsm_state)
                    {
                        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                            SON_ERROR,
                            "Message to X2 could not be sent"
                            " as X2 Connection establishment is in progress");

                        error_code = SON_ERR_X2_CONNECTION_IN_PROGRESS;
                    }

                    if (X2_ENB_STATE_DISCONNECTING == p_data->p_enb_rec->enb_curr_fsm_state)
                    {
                        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                            SON_ERROR,
                            "Message to X2 could not be sent"
                            " as X2 Connection termination is in progress");

                        error_code = SON_ERR_X2_DISCONNECTION_IN_PROGRESS;
                    }

                    /* Set Error Code */
                    SONANR_SET_ENB_PENDING_RES_ERROR_CODE(p_data,
                        error_code,
                        p_pending_resp->api_id,
                        p_pending_resp->transaction_id);
                    p_list_node = anr_get_next_enb_x2_pending_resp_elem(p_list_node);
                    continue;
                }

                /* If all cells are not switched on at RRM, request ES to switch-on cells */
                if (SON_FALSE == p_pending_resp->switch_on_resp_received)
                {
                    /* Set Error Code */
                    SONANR_SET_ENB_PENDING_RES_ERROR_CODE(p_data,
                        SON_ERR_ES_CELL_SWITCHED_OFF,
                        p_pending_resp->api_id,
                        p_pending_resp->transaction_id);

                    /* If Switch-on Request has not been initiated, send request to ES */
                    if (SON_FALSE == p_pending_resp->switch_on_sent)
                    {
                        p_pending_resp->switch_on_sent = SON_TRUE;
                        switch_on_req.transaction_id = p_pending_resp->transaction_id;

                        /* Increment Pending Response Count */
                        SONANR_INCR_X2_PENDING_RES_COUNT(p_pending_resp);

                        son_create_send_buffer((son_u8 *)&switch_on_req,
                            SON_ANR_MODULE_ID,
                            SON_ES_MODULE_ID,
                            SONES_CELL_SWITCH_ON_FOR_X2_SETUP_REQ,
                            sizeof(son_es_cell_switchon_for_x2_setup_req_t));
                    }

                    p_list_node = anr_get_next_enb_x2_pending_resp_elem(p_list_node);
                    continue;
                }

                /* SPR 11340 Fix Starts */
                /* Lines Deleted */
                /* SPR 11340 Fix Ends */

                /* Set Error Code */
                SONANR_SET_ENB_PENDING_RES_ERROR_CODE(p_data,
                    SON_ERR_X2_CONNECT_FAILURE,
                    p_pending_resp->api_id,
                    p_pending_resp->transaction_id);

                /* Increment Pending Response Count */
                SONANR_INCR_X2_PENDING_RES_COUNT(p_pending_resp);
                /*ENB CONFIG CHANGE REQ SPR:STOP*/

                /* Add enB to X2 eNB Add Request */
                add_req.transaction_id = p_pending_resp->transaction_id;
                x2_insert_eNB_in_list(&add_req.enb_list, p_data->p_enb_rec);
            }
            /* If X2 connection establishment is requested */
            else if ((SON_X2_DISCONNECT == p_data->nb_enb.x2_connection_status ||
                      SON_X2_DISCONNECT_BLACKLIST == p_data->nb_enb.x2_connection_status) &&
                     SON_PNULL != p_data->p_enb_rec)
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_INFO,
                    "X2 connection termination is requested");
                /*SPR_17670_START*/
                /* If X2 Connection is already terminated or establishment in progress or
                 * termination in progress, reject the request */
                if((X2_ENB_STATE_CONNECTED != p_data->p_enb_rec->enb_curr_fsm_state) &&
                        (X2_ENB_STATE_CONNECTING != p_data->p_enb_rec->enb_curr_fsm_state))
                {
                    son_error_et error_code = SON_NO_ERROR;

                    if (X2_ENB_STATE_DISCONNECTING == p_data->p_enb_rec->enb_curr_fsm_state)
                    {

                        /*SPR_17670_END*/
                        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                            SON_ERROR,
                            "Message to X2 could not be sent"
                            " as X2 Connection termination is in progress");

                        error_code = SON_ERR_X2_DISCONNECTION_IN_PROGRESS;
                    }

                    /* Set Error Code */
                    SONANR_SET_ENB_PENDING_RES_ERROR_CODE(p_data,
                        error_code,
                        p_pending_resp->api_id,
                        p_pending_resp->transaction_id);
                    p_list_node = anr_get_next_enb_x2_pending_resp_elem(p_list_node);
                    continue;
                }

                /* SPR 11340 Fix Starts */
                /* Lines Deleted */
                /* SPR 11340 Fix Ends */

                /* Set Error Code */
                SONANR_SET_ENB_PENDING_RES_ERROR_CODE(p_data,
                    SON_ERR_X2_DISCONNECT_FAILURE,
                    p_pending_resp->api_id,
                    p_pending_resp->transaction_id);

                /* Increment Pending Response Count */
                SONANR_INCR_X2_PENDING_RES_COUNT(p_pending_resp);

                del_req.transaction_id = p_pending_resp->transaction_id;
                x2_insert_eNB_in_list(&del_req.enb_list, p_data->p_enb_rec);
            }
        }

        p_list_node = anr_get_next_enb_x2_pending_resp_elem(p_list_node);
    }

    if (list_count(&add_req.enb_list))
    {
        if (0 == x2_get_active_nbr_count())
        {
            son_x2_nrt_update_ind_t nrt_upd_ind;
            anr_cell_context_node_t *p_node = SON_PNULL;
            SON_MEMSET(&nrt_upd_ind, 0, sizeof(nrt_upd_ind));
            nrt_upd_ind.is_local_update = SON_TRUE;
            p_node = anr_get_first_cell_ctxt();
            while (SON_PNULL != p_node)
            {
                anr_populate_x2_nrt_update_ind(&nrt_upd_ind, &p_node->data.cell_id, SON_NRT_OP_UPDATE);
                p_node = anr_get_next_node(p_node);
            }

            son_create_send_buffer((son_u8 *)&nrt_upd_ind, SON_ANR_MODULE_ID,
                SON_X2_MODULE_ID, SONX2_NRT_UPDATE_IND,
                sizeof(nrt_upd_ind));
        }

        son_create_send_buffer((son_u8 *)&add_req,
            SON_ANR_MODULE_ID,
            SON_X2_MODULE_ID,
            SONX2_ADD_ENB_REQ,
            sizeof(add_req));
    }

    if (list_count(&del_req.enb_list))
    {
        son_create_send_buffer((son_u8 *)&del_req,
            SON_ANR_MODULE_ID,
            SON_X2_MODULE_ID,
            SONX2_DELETE_ENB_REQ,
            sizeof(del_req));
    }

    SON_UT_TRACE_EXIT();
} /* anr_construct_send_message_to_x2 */

/*****************************************************************************
 * Function Name  : anr_construct_send_message_to_rrm
 * Inputs         : p_pending_resp_enb - Pointer to the eNB in eNB list in X2
 *                                                                              Pending Response element
 *                                : p_pending_resp - Pointer to the X2 Pending response element
 * Outputs        : None
 * Returns        : None
 * Description    : This function constructs and sends TNL Discovery
 *                  message to RRM module
 ****************************************************************************/
son_void_t
anr_construct_send_message_to_rrm
(
    anr_pending_nbr_enb_info_data_t *p_pending_resp_enb,
    anr_enb_pending_res_data_t *p_pending_resp
)
{
    son_rrm_tnl_discovery_req_t tnl_req;


    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_pending_resp_enb)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR,
            "Message to RRM could not be sent"
            " as p_pending_resp or p_pending_resp_enb is SON_NULL");

        SON_UT_TRACE_EXIT();
        return;
    }

    SON_MEMSET(&tnl_req, 0, sizeof(son_rrm_tnl_discovery_req_t));

    /* Set expiry time */
    SONANR_SET_ENB_PENDING_RES_EXPIRY_TIME(p_pending_resp_enb,
        TNL_DISCOVERY_TIMEOUT,
        p_pending_resp->api_id,
        p_pending_resp->transaction_id);

    /* Set error Code */
    SONANR_SET_ENB_PENDING_RES_ERROR_CODE(p_pending_resp_enb,
        SON_ERR_TNL_DISCOVERY_FAILURE,
        p_pending_resp->api_id,
        p_pending_resp->transaction_id);

    /* Increment Pending Response Count */
    SONANR_INCR_X2_PENDING_RES_COUNT(p_pending_resp);

    tnl_req.transaction_id = p_pending_resp->transaction_id;
    SON_MEMCPY(&tnl_req.tnl_req.target_enb_id,
        &p_pending_resp_enb->nb_enb.enb_id,
        sizeof(son_global_enb_id_t));

    SON_MEMCPY(tnl_req.tnl_req.tac,
        p_pending_resp_enb->nb_enb.tac_list[0].tac,
        sizeof(tnl_req.tnl_req.tac));

    /*Invoke PUP tool call to send message to RRM*/
    if (RRM_FAILURE != rrm_son_send_rrm_son_tnl_discovery_req(&tnl_req.tnl_req,
            SON_ANR_MODULE_ID, SON_RRM_MODULE_ID, 
            tnl_req.transaction_id, SON_NULL))
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_BRIEF,
            "Message %u send from %u to"
            " %u for eNB [ENB Id: 0x%x]]",
            RRM_SON_TNL_DISCOVERY_REQ,
            SON_ANR_MODULE_ID,
            SON_RRM_MODULE_ID,
            son_convert_char_cell_id_to_int_cell_id(p_pending_resp_enb->nb_enb.enb_id.enb_id));
    }

    SON_UT_TRACE_EXIT();
} /* anr_construct_send_message_to_rrm */

/*SPR 13795 Changes Start - Function optimized*/

/*****************************************************************************
 * Function Name  : anr_construct_send_message_to_oam
 * Inputs         : p_pending_resp - Pointer to the X2 Pending response element
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function constructs and sends response
 *                  message to OAM module
 ****************************************************************************/
son_void_t
anr_construct_send_message_to_oam
(
    anr_enb_pending_res_data_t *p_pending_resp
)
{
    SON_LIST_NODE *p_list_node = SON_PNULL;
    anr_pending_nbr_enb_info_data_t *p_data = SON_PNULL;
    anr_pending_nbr_enb_info_node_t *p_tpl = SON_PNULL;
    son_anr_enb_add_resp_t add_res;
    son_anr_enb_update_resp_t upd_res;
    son_anr_enb_delete_resp_t del_res;
    son_u8                                          *p_res = SON_PNULL;
    son_enb_status_t                        *p_enb_stst = SON_PNULL;
    son_u8                                          *p_enb_sts_size = SON_PNULL;
    son_u16 length = 0;
    son_bool_et process_enb_link_list = SON_TRUE;
    son_return_et               *p_result = SON_PNULL;
    son_error_et                *p_error = SON_PNULL;
    son_u8 req_list_count = 0;


    SON_UT_TRACE_ENTER();
    if (SON_PNULL == p_pending_resp)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR,
            "Message to OAM could not be sent"
            " as p_pending_resp is SON_NULL");

        SON_UT_TRACE_EXIT();
        return;
    }

    SON_MEMSET(&add_res, 0, sizeof(son_anr_enb_add_resp_t));
    SON_MEMSET(&upd_res, 0, sizeof(son_anr_enb_update_resp_t));
    SON_MEMSET(&del_res, 0, sizeof(son_anr_enb_delete_resp_t));

    switch (p_pending_resp->api_id)
    {
        case SONANR_NR_ENB_ADD_RESP:
        {
            add_res.transaction_id = p_pending_resp->transaction_id;
            p_res = (son_u8 *)&add_res;
            p_result = &add_res.enb_add_res.result;
            p_error = &add_res.enb_add_res.error_code;
            p_enb_stst = add_res.enb_add_res.nbr_enb_status_list;
            p_enb_sts_size = &add_res.enb_add_res.nbr_enb_status_list_size;
            length = sizeof(son_anr_enb_add_resp_t);
            break;
        }

        case SONANR_NR_ENB_DELETE_RESP:
        {
            del_res.transaction_id = p_pending_resp->transaction_id;
            p_res = (son_u8 *)&del_res;
            p_result = &del_res.enb_del_res.result;
            p_error = &del_res.enb_del_res.error_code;
            p_enb_stst = del_res.enb_del_res.nbr_enb_status_list;
            p_enb_sts_size = &del_res.enb_del_res.nbr_enb_status_list_size;
            length = sizeof(son_anr_enb_delete_resp_t);
            break;
        }

        case SONANR_NR_ENB_UPDATE_RESP:
        {
            upd_res.transaction_id = p_pending_resp->transaction_id;
            p_res = (son_u8 *)&upd_res;
            p_result = &upd_res.enb_update_res.result;
            p_error = &upd_res.enb_update_res.error_code;
            p_enb_stst = upd_res.enb_update_res.nbr_enb_status_list;
            p_enb_sts_size = &upd_res.enb_update_res.nbr_enb_status_list_size;
            length = sizeof(son_anr_enb_update_resp_t);
            break;
        }

        default:
        {
            process_enb_link_list = SON_FALSE;
            break;
        }
    } /* switch */

    req_list_count = p_pending_resp->pending_nbr_enb_list.count;
    p_list_node = list_pop_node(&p_pending_resp->pending_nbr_enb_list);
    while (SON_PNULL != p_list_node)
    {
        p_tpl = SON_PNULL;
        p_data = SON_PNULL;
        p_tpl = YMEMBEROF(anr_pending_nbr_enb_info_node_t,
            anchor, p_list_node);
        p_data = &p_tpl->data;

        if (SON_NO_ERROR != p_data->error_code &&
            SON_TRUE == process_enb_link_list)
        {
            p_enb_stst[*p_enb_sts_size].error_code = p_data->error_code;
            SON_MEMCPY(&p_enb_stst[*p_enb_sts_size].enb_id,
                &p_data->nb_enb.enb_id,
                sizeof(son_global_enb_id_t));
            (*p_enb_sts_size)++;
        }

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_BRIEF,
            "enb id[0x%x] is deleted from the pending response"
            " with API [%d] and Transaction id [%d]",
            son_convert_char_cell_id_to_int_cell_id(
                p_data->nb_enb.enb_id.enb_id), p_pending_resp->api_id, p_pending_resp->transaction_id);

        /* Free the memory occupied by the node */
        son_mem_free(p_tpl);

        p_list_node = list_pop_node(&p_pending_resp->pending_nbr_enb_list);
    }

    if (SON_PNULL != p_res)
    {
        *p_error = SON_NO_ERROR;
        if (!(*p_enb_sts_size))
        {
            *p_result = SON_SUCCESS;
        }
        else
        {
            if (req_list_count != *p_enb_sts_size)
            {
                *p_result = SON_PARTIAL_SUCCESS;
            }
            else
            {
                *p_result = SON_FAILURE;
            }
        }

        son_create_send_buffer((son_u8 *)p_res,
            SON_ANR_MODULE_ID,
            SON_MIF_MODULE_ID,
            p_pending_resp->api_id,
            length);
    }

    anr_del_x2_pending_resp_list(p_pending_resp->transaction_id);
    SON_UT_TRACE_EXIT();
} /* anr_construct_send_message_to_oam */

/*SPR 13795 Changes - End*/

/*****************************************************************************
 * Function Name  : anr_determine_macro_enb_id
 * Inputs         : p_cell_id - Pointer to cell id
 * Outputs        : p_enb_id - Pointer to eNB id
 * Returns        : None
 * Description    : This function determines eNB Id from ECGI
 ****************************************************************************/
son_void_t
anr_determine_macro_enb_id
(
    son_intra_rat_global_cell_id_t *p_cell_id,
    son_global_enb_id_t *p_enb_id
)
{
    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_cell_id || SON_PNULL == p_enb_id)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR,
            "Neighbor eNB ID could not be determined as "
            " as p_cell_id or p_enb_id is SON_PNULL");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_enb_id->enb_id[0] = p_cell_id->cell_identity[0];
    p_enb_id->enb_id[1] = p_cell_id->cell_identity[1];
    p_enb_id->enb_id[2] = p_cell_id->cell_identity[2] & 0xF0;
    SON_UT_TRACE_EXIT();
} /* anr_determine_macro_enb_id */

/*****************************************************************************
 * Function Name  : anr_determine_enb_id
 * Inputs         : p_nbr - Pointer to hash record
 * Outputs        : p_enb_id - Pointer to eNB id
 * Returns        : None
 * Description    : This function determines eNB Id from ECGI
 ****************************************************************************/
son_void_t
anr_determine_enb_id
(
    anr_intra_rat_hash_rec_t *p_nbr,
    son_global_enb_id_t *p_enb_id
)
{
    SON_UT_TRACE_ENTER();
    if (SON_PNULL == p_nbr || SON_PNULL == p_enb_id)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "Neighbor eNB ID could not be determined as "
                "p_nbr or p_enb_id is SON_PNULL");
        SON_UT_TRACE_EXIT();
        return;
    }

    SON_MEMCPY(&p_enb_id->plmn_id,
            &p_nbr->neighbor_info.nbr_cgi.plmn_id,
            sizeof(son_cell_plmn_info_t));

    /*SPR-10230-Start*/

    /* Macro Cell */
    if ((p_nbr->neighbor_info.bitmask & SON_ACCESS_MODE_PRESENT) &&
            (p_nbr->neighbor_info.access_mode == SON_OPEN))
    {
        p_enb_id->enb_type = SON_MACRO_ENB;
        anr_determine_macro_enb_id(&p_nbr->neighbor_info.nbr_cgi, p_enb_id);
    }
    else
    {
        p_enb_id->enb_type = SON_HOME_ENB;
        p_enb_id->enb_id[0] = p_nbr->neighbor_info.nbr_cgi.cell_identity[0];
        p_enb_id->enb_id[1] = p_nbr->neighbor_info.nbr_cgi.cell_identity[1];
        p_enb_id->enb_id[2] = p_nbr->neighbor_info.nbr_cgi.cell_identity[2];
        p_enb_id->enb_id[3] = p_nbr->neighbor_info.nbr_cgi.cell_identity[3] & 0xF0;
    }

    /*SPR-10230-End*/

    SON_UT_TRACE_EXIT();
} /* anr_determine_enb_id */

/*****************************************************************************
 * Function Name  : anr_neighbor_eNB_x2_setup
 * Inputs         : p_nbr - Pointer to Neighbor Hash Record
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function initiates x2 setup for the newly added neighbor cell
 ****************************************************************************/
son_void_t
anr_neighbor_eNB_x2_setup
(
    anr_intra_rat_hash_rec_t *p_nbr
)
{
    son_global_enb_id_t enb_id;
    son_error_et error_code = SON_NO_ERROR;
    son_nbr_enb_hash_tuple_t *p_enb_tpl = SON_NULL;
    son_neighbor_enb_info_t enb_info;
    anr_enb_pending_res_data_t              *p_pending_resp         = SON_PNULL;
    anr_pending_nbr_enb_info_data_t *p_pending_resp_enb = SON_PNULL;
    son_x2_add_enb_req_t add_req;


    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_nbr)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR,
            "Neighbor eNB X2 Setup could not be initiated "
            " as p_nbr is SON_NULL");
        SON_UT_TRACE_EXIT();
        return;
    }

    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
        SON_INFO,
        "X2 Setup for newly detected neighbor eNB Id: 0x%x",
        son_convert_char_cell_id_to_int_cell_id(enb_id.enb_id));

    SON_MEMSET(&enb_id, 0, sizeof(enb_id));
    SON_MEMSET(&enb_info, 0, sizeof(enb_info));
    SON_MEMSET(&add_req, 0, sizeof(add_req));

    anr_determine_enb_id(p_nbr, &enb_id);

    p_enb_tpl = x2_find_nbr_enb(x2_get_enb_nrt(), &enb_id, &error_code);

    if (SON_PNULL == p_enb_tpl)
    {
        /*COverity fix of making index as 1*/
        if (p_nbr->neighbor_info.tac[0] == 0x00 && p_nbr->neighbor_info.tac[1] == 0x00)
        {
            SON_LOG(
                anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "TNL Discovery could not be initiated as invalid Tac[0] received for eNB [ENB Id: 0x%x]",
                son_convert_char_cell_id_to_int_cell_id(enb_id.enb_id));
            SON_UT_TRACE_EXIT();
            return;
        }

        error_code = SON_NO_ERROR;

        if (SON_PNULL != (p_pending_resp_enb =
                              anr_find_enb_x2_pending_resp_list(&enb_id,
                                  &p_pending_resp)))
        {
            if (SON_ERR_TNL_DISCOVERY_FAILURE == p_pending_resp_enb->error_code)
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_WARNING,
                    "TNL Discovery is already in progress for eNB [ENB Id: 0x%x]",
                    son_convert_char_cell_id_to_int_cell_id(enb_id.enb_id));
                SON_UT_TRACE_EXIT();
                return;
            }
        }

        x2_init_enb_neighbor_info(&enb_info, &enb_id, 0, SON_PNULL, p_nbr->neighbor_info.tac);

        p_pending_resp = anr_add_x2_pending_resp_list(son_generate_txn_id(),
            (son_procedure_code_et)RRM_SON_TNL_DISCOVERY_REQ);

        p_pending_resp_enb = anr_add_enb_x2_pending_resp_elem(p_pending_resp,
            &enb_info,
            SON_PNULL,
            error_code);

        /* Send TNL Discovery Request to RRM */
        anr_construct_send_message_to_rrm(p_pending_resp_enb, p_pending_resp);
    }
    else
    {
        if (X2_ENB_STATE_DISCONNECTED == p_enb_tpl->data.enb_curr_fsm_state &&
            SON_NR_UNLOCKED == p_enb_tpl->data.nr_status)
        {
            add_req.transaction_id = son_generate_txn_id();
            x2_insert_eNB_in_list(&add_req.enb_list, &p_enb_tpl->data);
            son_create_send_buffer((son_u8 *)&add_req,
                SON_ANR_MODULE_ID,
                SON_X2_MODULE_ID,
                SONX2_ADD_ENB_REQ,
                sizeof(add_req));
        }
    }

    SON_UT_TRACE_EXIT();
} /* anr_neighbor_eNB_x2_setup */

/*****************************************************************************
 * Function Name  : anr_x2_pending_resp_timer_expiry
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function does X2 Pending response cleanup on timer expiry
 ****************************************************************************/
son_void_t
anr_x2_pending_resp_timer_expiry
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    anr_pending_nbr_enb_info_node_t *p_enb = SON_PNULL;
    anr_enb_pending_res_node_t *p_msg = SON_PNULL;
    time_stamp_t curr;


    SON_UT_TRACE_ENTER();
    p_msg = anr_get_first_x2_pending_resp_list();
    while (SON_PNULL != p_msg)
    {
        if (p_msg->data.expected_res_count)
        {
            p_enb = anr_get_first_enb_x2_pending_resp_elem(&p_msg->data);
            while (SON_PNULL != p_enb)
            {
                son_time(&curr);
                if (p_enb->data.expiry_time &&
                    ((son_u32)(son_difftime(curr, p_enb->data.start_ts))) >= p_enb->data.expiry_time)
                {
                    SON_LOG(
                        anr_get_log_mode(), p_son_anr_facility_name,
                        SON_DETAILED,
                        "Expected Response timer for API [API Id: %d] with [Trans Id: %d] expired",
                        p_msg->data.api_id, p_msg->data.transaction_id);
                    if (!p_enb->data.p_enb_rec ||
                        (p_enb->data.p_enb_rec &&
                         (X2_ENB_STATE_CONNECTED != p_enb->data.p_enb_rec->enb_curr_fsm_state) &&
                         (X2_ENB_STATE_DISCONNECTED != p_enb->data.p_enb_rec->enb_curr_fsm_state)))
                    {
                        SONANR_SET_ENB_PENDING_RES_EXPIRY_TIME((&p_enb->data),
                            0,
                            p_msg->data.api_id,
                            p_msg->data.transaction_id);

                        SONANR_DECR_X2_PENDING_RES_COUNT((&p_msg->data));

                        if (p_enb->data.p_enb_rec)
                        {
                            x2_enb_fsm_state_et prev_state = p_enb->data.p_enb_rec->enb_prev_fsm_state;
                            SONX2_ENB_FSM_SET_STATE((p_enb->data.p_enb_rec),
                                (p_enb->data.p_enb_rec->enb_prev_fsm_state),
                                (p_enb->data.p_enb_rec->enb_curr_fsm_state),
                                prev_state);
                        }
                    }
                    /* Spr 16142 Fix Start */
                    /* Sending Tnl discovery stop to rrm */
                    if((p_msg->data.api_id == SONANR_NR_ENB_ADD_RESP)&&
                            (p_enb->data.error_code == SON_ERR_TNL_DISCOVERY_FAILURE))
                    {

                        anr_construct_send_stop_tnl_discovery_ind_to_rrm
                            (&p_enb->data.nb_enb.enb_id);
                    }
                    /* Spr 16142 Fix End */
                }

                p_enb = anr_get_next_enb_x2_pending_resp_elem(p_enb);
            }

            if (!p_msg->data.expected_res_count)
            {
                anr_construct_send_message_to_oam(&p_msg->data);
                p_msg = anr_get_first_x2_pending_resp_list();
            }
            else
            {
                p_msg = anr_get_next_x2_pending_resp_list(p_msg);
            }
        }
    }

    SON_UT_TRACE_EXIT();
} /* anr_x2_pending_resp_timer_expiry */

/******************************************************************************
* Function Name   : anr_construct_l3_to_son_intra_rat_hash_rec
* Inputs         : p_intra_rat_hash_rec - hash record to be created.
*                 : p_srv_cell_data      - Pointer to the  neighbor info from L3.
*                 : p_enb_id             - Pointer to the global eNB id
* Outputs        : None.
* Returns        : son_void_t
* Description    : This function creates  the hash record from
*                    neighbor info from L3
******************************************************************************/
son_void_t
anr_construct_l3_to_son_intra_rat_hash_rec
(
    anr_intra_rat_hash_rec_t    *p_intra_rat_hash_rec,
    x2ap_served_cell_info_arr_t *p_srv_cell_data,
    son_global_enb_id_t         *p_enb_id
)
{
    /* Coverity Fix: 19334 */
    x2ap_served_cell_info_t *p_srv_cell_info = SON_PNULL;
    son_u16 idx = 0;
#ifdef LTE_EMBMS_SUPPORTED
    son_u8 mbsfn_count = 0;
#endif


    SON_UT_TRACE_ENTER();

    p_srv_cell_info = &p_srv_cell_data->cell_info;

    /*ecgi*/
    get_ecgi_from_enb_id_cell_id(
            &p_intra_rat_hash_rec->neighbor_info.nbr_cgi,
            p_srv_cell_info->cell_id, p_enb_id);

    /*Set access mode*/
    if (SON_MACRO_ENB == p_enb_id->enb_type)
    {
        p_intra_rat_hash_rec->neighbor_info.access_mode = SON_OPEN;
        p_intra_rat_hash_rec->neighbor_info.bitmask |= SON_ACCESS_MODE_PRESENT;
    /*SPR 15799 Fix Start*/
    }
    else if(!(X2AP_CSG_ID_INFO_PRESENT & p_srv_cell_info->bitmask))
    {
        p_intra_rat_hash_rec->neighbor_info.access_mode = SON_OPEN_FEMTO;
        p_intra_rat_hash_rec->neighbor_info.bitmask |= SON_ACCESS_MODE_PRESENT;
    }
    /*SPR 15799 Fix Stop*/

    /*pci*/
    p_intra_rat_hash_rec->neighbor_info.pci = p_srv_cell_info->pci;

    /*tac*/
    SON_MEMCPY(p_intra_rat_hash_rec->neighbor_info.tac,
            p_srv_cell_info->tac, SON_TAC_OCTET_SIZE);

    /*Plmn info*/
    for (idx = 0; idx < SON_NR_MAX_NUM_PLMN &&
            idx < p_srv_cell_info->bcast_plmn_info.num_bcast_plmn_id; idx++)
    {
        x2_convert_l3_plmn_id_to_son_plmn_id(
                &p_srv_cell_info->bcast_plmn_info.plmn_identity[idx],
                &p_intra_rat_hash_rec->neighbor_info.plmn_id[idx]);

        p_intra_rat_hash_rec->neighbor_info.num_plmn_id++;
    }

    /*EARFCN*/
    if (p_srv_cell_info->choice_eutra_mode.choice_eutra_mode.bitmask
            & X2AP_EUTRA_MODE_FDD)
    {
        p_intra_rat_hash_rec->neighbor_info.bitmask |= SON_EARFCN_PRESENT;
        p_intra_rat_hash_rec->neighbor_info.earfcn
            = p_srv_cell_info->choice_eutra_mode.
            choice_eutra_mode.fdd_info.dl_earfcn.earfcn;

        p_intra_rat_hash_rec->neighbor_info.bitmask |= SON_UL_EARFCN_PRESENT;
        p_intra_rat_hash_rec->neighbor_info.ul_earfcn
            = p_srv_cell_info->choice_eutra_mode.
            choice_eutra_mode.fdd_info.ul_earfcn.earfcn;

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_BRIEF, "EARFCN given is FDD.");
    }
    else if (p_srv_cell_info->choice_eutra_mode.choice_eutra_mode.bitmask
            & X2AP_EUTRA_MODE_TDD)
    {
        p_intra_rat_hash_rec->neighbor_info.bitmask |= SON_EARFCN_PRESENT;
        p_intra_rat_hash_rec->neighbor_info.earfcn
            = p_srv_cell_info->choice_eutra_mode.
            choice_eutra_mode.tdd_info.earfcn.earfcn;

        p_intra_rat_hash_rec->neighbor_info.bitmask |= SON_UL_EARFCN_PRESENT;
        p_intra_rat_hash_rec->neighbor_info.ul_earfcn
            = p_srv_cell_info->choice_eutra_mode.
            choice_eutra_mode.tdd_info.earfcn.earfcn;

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_BRIEF, "EARFCN given is TDD.");
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR, "EARFCN is not present");
    }
    anr_set_the_ul_earfcn_dl_earfcn_of_eutran_neighbor(p_intra_rat_hash_rec);

    if(X2AP_PRACH_CONFIG_INFO_PRESENT & p_srv_cell_info->bitmask)
    {
        p_intra_rat_hash_rec->neighbor_info.bitmask |= SON_PRACH_CONFIGURATION_PRESENT;

        p_intra_rat_hash_rec->neighbor_info.prach_config.root_sequence_index
            = p_srv_cell_info->prach_config.root_sequence_index;

        p_intra_rat_hash_rec->neighbor_info.prach_config.zero_correlation_zone_config
            = p_srv_cell_info->prach_config.zero_correlation_zone_config;

        p_intra_rat_hash_rec->neighbor_info.prach_config.high_speed_flag = 
            p_srv_cell_info->prach_config.high_speed_flag;

        p_intra_rat_hash_rec->neighbor_info.prach_config.prach_frequency_offset =
            p_srv_cell_info->prach_config.prach_frequency_offset;

        if(X2AP_PRACH_CONFIG_INDEX_PRESENT & p_srv_cell_info->prach_config.bitmask)
        {
            p_intra_rat_hash_rec->neighbor_info.prach_config.bitmask |= SON_PRACH_CONFIG_INDEX_PRESENT;
            p_intra_rat_hash_rec->neighbor_info.prach_config.prach_configuration_index 
                = p_srv_cell_info->prach_config.prach_config_index;
        }
    }

    if(X2AP_CSG_ID_INFO_PRESENT & p_srv_cell_info->bitmask)
    {
        p_intra_rat_hash_rec->neighbor_info.bitmask |= SON_INTRA_CSG_ID_PRESENT;
        SON_MEMCPY(p_intra_rat_hash_rec->neighbor_info.csg_identity,
                p_srv_cell_info->csg_identity,
                sizeof(p_intra_rat_hash_rec->neighbor_info.csg_identity));
    }

#ifdef LTE_EMBMS_SUPPORTED
    if(X2AP_MBSFN_SUBFRAME_INFO_PRESENT & p_srv_cell_info->bitmask)
    {
        p_intra_rat_hash_rec->neighbor_info.bitmask |= SON_EMBMS_MBMSFN_INFO_PRESENT;
        p_intra_rat_hash_rec->neighbor_info.mbms_config.num_mbsfn  
            = p_srv_cell_info->mbsfn_subframe_info.num_mbsfn;
        for(mbsfn_count = 0; mbsfn_count < p_srv_cell_info->mbsfn_subframe_info.num_mbsfn; mbsfn_count++)
        {
            p_intra_rat_hash_rec->neighbor_info.mbms_config.mbsfn_info[mbsfn_count].radioframe_alloc_period  
                = p_srv_cell_info->mbsfn_subframe_info.mbsfn_info[mbsfn_count].radioframe_alloc_period;
            p_intra_rat_hash_rec->neighbor_info.mbms_config.mbsfn_info[mbsfn_count].radioframe_alloc_offset   
                = p_srv_cell_info->mbsfn_subframe_info.mbsfn_info[mbsfn_count].radioframe_alloc_offset;
            p_intra_rat_hash_rec->neighbor_info.mbms_config.
                mbsfn_info[mbsfn_count].subframe_alloc.bitmask = 
                p_srv_cell_info->mbsfn_subframe_info.mbsfn_info[mbsfn_count].subframe_alloc.bitmask;
            SON_MEMCPY(&(p_intra_rat_hash_rec->neighbor_info.mbms_config.
                        mbsfn_info[mbsfn_count].subframe_alloc.oneframe),
                    &(p_srv_cell_info->mbsfn_subframe_info.mbsfn_info[mbsfn_count].
                        subframe_alloc.oneframe), SON_SUBFRAME_ALLOC_ONE_FRAME_OCTET_SIZE);
            SON_MEMCPY(&(p_intra_rat_hash_rec->neighbor_info.mbms_config.
                        mbsfn_info[mbsfn_count].subframe_alloc.fourframe),
                    &(p_srv_cell_info->mbsfn_subframe_info.mbsfn_info[mbsfn_count].
                        subframe_alloc.fourframe), SON_SUBFRAME_ALLOC_FOUR_FRAME_OCTET_SIZE);
        }
    }
#endif
    /* SPR 10103 FIX START */
    SON_MEMCPY(&p_intra_rat_hash_rec->enb_id, p_enb_id, sizeof(son_global_enb_id_t));
    /* SPR 10103 FIX END */

    /*Add bitmasks*/
    p_intra_rat_hash_rec->neighbor_info.bitmask
        |= SON_PCI_PRESENT |
        SON_TAC_PRESENT |
        SON_PLMN_ID_PRESENT;

    SON_UT_TRACE_EXIT();
} /* anr_construct_l3_to_son_intra_rat_hash_rec */

/******************************************************************************
* Function Name   : anr_nr_add_nbr_cells
* Inputs         : p_cell_ctxt         - Pointer to cell context
*                 : p_add_nbr           - Pointer to the neighbor info from L3 added
*                                         in NRT
*                 : x2_status           - X2  status
*                 : p_enb_id            - Pointer to  Global eNB context
* Outputs         : p_oam_notif_for_add - Pointer to oam notification msg for add
*                 : p_oam_notif_for_upd - Pointer to oam notification msg for
*                                         update
*                 : p_rrm_notif         - Pointer to rrm notification message
* Returns        : son_bool_et
* Description    : This function  adds,updates  the given neighbor in the NRT.
******************************************************************************/
son_bool_et
anr_nr_add_nbr_cells
(
    anr_cell_context_t                      *p_cell_ctxt,
    x2ap_served_cell_info_arr_t             *p_add_nbr,
        /*SPR 17777 +-*/ 
    son_global_enb_id_t                     *p_enb_id,
    son_anr_updated_nrt_info_t               *p_oam_notif_for_add,
    son_anr_updated_nrt_info_t               *p_oam_notif_for_upd,
    son_anr_rrm_updated_nrt_info_t              *p_rrm_notif
)
{
    anr_intra_rat_hash_rec_t *p_inserted_nr     = SON_PNULL;
    anr_intra_rat_hash_rec_t intra_rat_hash_rec;
    anr_intra_rat_hash_rec_t old_hash_rec;
    son_error_et error_code          = SON_NO_ERROR;
    son_bool_et ret                 = SON_FALSE;
    SON_HASH_NODE                       *p_node                  = SON_PNULL;
    anr_intra_rat_hash_nr_tuple_t     *p_tpl                     = SON_PNULL;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&intra_rat_hash_rec, 0, sizeof(anr_intra_rat_hash_rec_t));
    SON_MEMSET(&old_hash_rec, 0, sizeof(anr_intra_rat_hash_rec_t));
    list_init(&intra_rat_hash_rec.anr_nr_nbr_list);

    anr_construct_l3_to_son_intra_rat_hash_rec(&intra_rat_hash_rec,
        p_add_nbr, p_enb_id);

    /*Do not do the setting of the X2status
     * as it is a READonly filed*/

    /*modify the data pointed by p_hash_tpl*/
    p_inserted_nr  = anr_intra_rat_modify_nr(p_cell_ctxt,
        &intra_rat_hash_rec,
        SON_FALSE,
        &error_code,
        p_oam_notif_for_add,
        p_oam_notif_for_upd,
        p_rrm_notif,
        SON_TRUE,
        SON_FALSE);

    if (SON_PNULL != p_inserted_nr &&
        ((intra_rat_hash_rec.neighbor_info.pci != p_inserted_nr->neighbor_info.pci) ||
         (intra_rat_hash_rec.neighbor_info.earfcn != p_inserted_nr->neighbor_info.earfcn) ||
	 /*SPR 16523 fix start*/
	 (intra_rat_hash_rec.neighbor_info.tac != p_inserted_nr->neighbor_info.tac) ))
	 /*SPR 16523 fix stop*/
    {
        ret = SON_TRUE;
    }

    if ((SON_NO_ERROR ==  error_code) &&
        (SON_PNULL != (p_node = hash_find(&p_cell_ctxt->intra_rat_neighbor_table,
                           (son_void_t *)&intra_rat_hash_rec))))
    {
        p_tpl = (anr_intra_rat_hash_nr_tuple_t *)
                YMEMBEROF(anr_intra_rat_hash_nr_tuple_t, anchor, p_node);
        /* Spr 17753 Changes Start */  
        anr_process_inra_rat_nbr_nbrs(&p_tpl->data, &p_add_nbr->nbour_cell_info,p_cell_ctxt);
        /* Spr 17753 Changes End */  
    }

    if (SON_ERR_ANR_NR_NOT_FOUND == error_code)
    {
        intra_rat_hash_rec.neighbor_info.nr_status = SON_NR_UNLOCKED;
        intra_rat_hash_rec.neighbor_info.ho_status = SON_HO_ALLOWED;
        intra_rat_hash_rec.neighbor_info.bitmask  |= SON_ENABLE_NR_STATUS_PRESENT |
                                                     SON_ENABLE_HO_STATUS_PRESENT;
	/*SPR 15438 Lines deleted*/

        /*SPR 15438 - Start*/
        /*Report the neighbors to OAM*/
        if (p_oam_notif_for_add)
        {
            /*SPR 15799 Fix Start*/
            son_anr_set_x2_status_for_new_neighbor(p_cell_ctxt,&intra_rat_hash_rec,p_enb_id);
            /*SPR 15799 Fix Stop*/
            p_oam_notif_for_add->nrt_op = SON_NRT_OP_ADD;
            anr_construct_oam_updated_ncl(&p_oam_notif_for_add->nr_list,
                    &intra_rat_hash_rec.neighbor_info,
                    SON_FALSE);
            /* Spr 17753 Changes Start */
            /* Insert Second Degree nbr to link list */
            if(p_cell_ctxt->nr_add_wait_timer_val)
            {   
                if(SON_FAILURE  == anr_insert_neigh_info_in_link_list
                        (p_add_nbr, p_cell_ctxt ,p_enb_id))
                {
                    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                            SON_ERROR, "anr_process_x2_link_up_ind_for_nrt_updation:"
                            "Failed to add second degree node in Link List");
                }
            }
            /* Spr 17753 Changes End */
        }
        /*SPR 15438 - End*/
 
    }

    SON_UT_TRACE_EXIT();

    return ret;
} /* anr_nr_add_nbr_cells */

/******************************************************************************
* Function Name  : anr_traverse_x2_link_up_ind_list
* Inputs         : p_cell_ctxt         - Pointer to the cell context.
*                 : p_add_list          - Pointer to the Neighbor list from x2 link
*                                         up ind  to be added in NRT
*                 : x2_status           - x2 ho status
*                 : srv_cell_list       - List size
*                 : p_enb_id            - Pointer to the global eNB id
* Outputs        : p_oam_notif_for_add - Pointer to oam notify message for add
*                 : p_oam_notif_for_upd - Pointer to oam notify msg for update
*                 : p_rrm_notif         - Pointer to rrm notification msg
* Returns        : son_bool_et
* Description    : This function traverses the list of neighbors to be added
*                   in x2 link up ind msg to add them to NRT.
******************************************************************************/
son_bool_et
anr_traverse_x2_link_up_ind_list
(
    anr_cell_context_t                       *p_cell_ctxt,
    x2ap_served_cell_info_arr_t              *p_add_list,
    /*SPR 17777 +-*/
    son_u16 srv_cell_list,
    son_global_enb_id_t                      *p_enb_id,
    son_anr_updated_nrt_info_t               *p_oam_notif_for_add,
    son_anr_updated_nrt_info_t               *p_oam_notif_for_upd,
    son_anr_rrm_updated_nrt_info_t               *p_rrm_notif
)
{
    son_bool_et ret = SON_FALSE;
    son_u16 idx = 0;


    SON_UT_TRACE_ENTER();
    for (idx = 0; idx < srv_cell_list; idx++)
    {
        ret |= anr_nr_add_nbr_cells(p_cell_ctxt,
            &p_add_list[idx],
        /*SPR 17777 +-*/ 
            p_enb_id,
            p_oam_notif_for_add,
            p_oam_notif_for_upd,
            p_rrm_notif);
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_traverse_x2_link_up_ind_list */

/******************************************************************************
* Function Name  : anr_populate_x2_nrt_update_ind
* Inputs         : p_cell_id     - Pointer to intra rat global cell id struct.
*                 : nr_op         - operation performed
* Outputs        : p_nrt_upd_ind - Pointer to nrt update ind msg.
* Returns        : son_void_t
* Description    : This function populates the  nrt update indication msg.
******************************************************************************/
son_void_t
anr_populate_x2_nrt_update_ind
(
    son_x2_nrt_update_ind_t *p_nrt_upd_ind,
    son_intra_rat_global_cell_id_t      *p_cell_id,
    son_nrt_operation_performed_et nr_op
)
{
    SON_UT_TRACE_ENTER();
    /*klockwork fix*/
    if (p_nrt_upd_ind->num_srv_cells_list_size <  MAX_SERVED_CELLS)
    {
        p_nrt_upd_ind->srv_cell_updated_info_list[
            p_nrt_upd_ind->num_srv_cells_list_size].src_cgi
            = *p_cell_id;

        p_nrt_upd_ind->srv_cell_updated_info_list[
            p_nrt_upd_ind->num_srv_cells_list_size].nrt_op
            = nr_op;

        p_nrt_upd_ind->num_srv_cells_list_size++;
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_populate_x2_nrt_update_ind:"
                       "List size full");
    }

    SON_UT_TRACE_EXIT();
} /* anr_populate_x2_nrt_update_ind */

/*SPR 16523 fix start*/
/******************************************************************************
* Function Name  : is_x2_connection_available
* Inputs         : None
* Outputs        : None
* Returns        : son_bool_et   ( SON_TRUE/SON_FALSE)
* Description    : This function returns true if X2 connection is available or 
*                  in resetting state.
******************************************************************************/
/*SPR 16523 fix stop*/
son_bool_et
is_x2_connection_available
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    son_bool_et ret_val = SON_FALSE;
    son_nbr_enb_hash_tuple_t *p_enb_tpl = SON_PNULL;


    p_enb_tpl = x2_get_first_nbr_enb_from_nrt(x2_get_enb_nrt());
    while (SON_PNULL != p_enb_tpl)
    {
	    /*SPR 16523 fix start*/
	    /*SPR_16835_START*/
        if ((X2_ENB_STATE_CONNECTED == p_enb_tpl->data.enb_curr_fsm_state) ||
            (X2_ENB_STATE_RESETTING == p_enb_tpl->data.enb_curr_fsm_state) ||
	    (X2_ENB_STATE_DISCONNECTING == p_enb_tpl->data.enb_curr_fsm_state))
	    /*SPR_16835_END*/
	    /*SPR 16523 fix stop*/
        {
            ret_val = SON_TRUE;
	    /*SPR 16523 fix start*/
            break;
	    /*SPR 16523 fix stop*/
        }

        p_enb_tpl = x2_get_next_nbr_enb_from_nrt(x2_get_enb_nrt(), &p_enb_tpl->anchor);
    }

    return ret_val;
} /* is_x2_connection_available */

/******************************************************************************
* Function Name  : anr_populate_send_x2_nrt_update_ind
* Inputs         : p_cell_id     - Pointer to intra rat global cell id
*                 : nr_op         - operation performedi
* Outputs        : p_nrt_upd_ind - Pointer to nrt update ind msg.
* Returns        : son_void_t
* Description    : This function populates and sends the  nrt update indication msg.
******************************************************************************/
son_void_t
anr_populate_send_x2_nrt_update_ind
(
    son_x2_nrt_update_ind_t *p_nrt_upd_ind,
    son_intra_rat_global_cell_id_t      *p_cell_id,
    son_nrt_operation_performed_et nr_op
)
{
    SON_UT_TRACE_ENTER();
    /*klockwork fix*/
    if (p_nrt_upd_ind->num_srv_cells_list_size < MAX_SERVED_CELLS)
    {
        p_nrt_upd_ind->srv_cell_updated_info_list[
            p_nrt_upd_ind->num_srv_cells_list_size].src_cgi
            = *p_cell_id;

        p_nrt_upd_ind->srv_cell_updated_info_list[
            p_nrt_upd_ind->num_srv_cells_list_size].nrt_op
            = nr_op;

        p_nrt_upd_ind->num_srv_cells_list_size++;
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_populate_send_x2_nrt_update_ind:"
                       "List size full");
    }

    /*Send the msg to X2*/
    if (p_nrt_upd_ind->num_srv_cells_list_size && is_x2_connection_available())
    {
        son_create_send_buffer((son_u8 *)p_nrt_upd_ind, SON_ANR_MODULE_ID,
            SON_X2_MODULE_ID, SONX2_NRT_UPDATE_IND,
            sizeof(son_x2_nrt_update_ind_t));
    }

    SON_UT_TRACE_EXIT();
} /* anr_populate_send_x2_nrt_update_ind */

/******************************************************************************
* Function Name  : get_x2_sts_value_for_nbr_from_enb_rec
* Inputs         : p_enb_hash_rec    - Pointer to enb hash record
* Outputs        : None
* Returns        : son_x2_status_et  - X2 status and connection status
* Description    : This function determines the HO_status and Connection status
*                   for the neighbor X2_status filed.
*                   (x2_status in nbrs  = ho_status + connection_status)
******************************************************************************/
son_x2_status_et
get_x2_sts_value_for_nbr_from_enb_rec
(
    son_nbr_enb_hash_rec_t *p_enb_hash_rec
)
{
    son_x2_status_et x2_status = SON_X2_ALLOWED_DISCONNECTED;


    SON_UT_TRACE_ENTER();

    if (SON_PNULL != p_enb_hash_rec)
    {
        if ((X2_ENB_STATE_DISCONNECTED ==  p_enb_hash_rec->enb_curr_fsm_state ||
             X2_ENB_STATE_CONNECTING ==  p_enb_hash_rec->enb_curr_fsm_state))
        {
            if (SON_X2_PROHIBITED == p_enb_hash_rec->x2_status)
            {
                x2_status =  SON_X2_PROHIBITED_DISCONNECTED;
            }
        }
        else
        {
            x2_status = SON_X2_ALLOWED_CONNECTED;
            if ((X2_ENB_STATE_CONNECTED ==  p_enb_hash_rec->enb_curr_fsm_state ||
                 X2_ENB_STATE_DISCONNECTING ==  p_enb_hash_rec->enb_curr_fsm_state))
            {
                if (SON_X2_PROHIBITED == p_enb_hash_rec->x2_status)
                {
                    x2_status = SON_X2_PROHIBITED_CONNECTED;
                }
            }
        }
        /* Coverity:61999 Fix-Start */
    /* SPR 10103 FIX START */
    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_DETAILED,
            "x2_status value stored in hash record"
            " is %u and the value returned by this function is %u for eNB [eNB Id: 0x%x]]",
            p_enb_hash_rec->x2_status, x2_status,
            son_convert_char_cell_id_to_int_cell_id(p_enb_hash_rec->enb_id.enb_id));
    /* SPR 10103 FIX END */
        /* Coverity:61999 Fix-End */
    }

        /* Coverity:61999 Fix-Delete Trace */

    SON_UT_TRACE_EXIT();
    return x2_status;
} /* get_x2_sts_value_for_nbr_from_enb_rec */

/******************************************************************************
* Function Name  : anr_process_x2_link_up_ind_for_nrt_updation
* Inputs         : p_srv_cell_info   - Pointer to the list of neighbors in X2
*                                       link up indication to be added
*                                       in NRT
*                 : srv_cell_list     - List size
*                 : p_enb_id          - Pointer to global eNB id
*                 : p_enb_hash_rec    - Pointer to enb hash record
*                 : is_x2_conn_triggred_by_oam - booloean value to determine
*                                         this message is triggered due to
*                                         peer enb.
* Outputs        : None
* Returns        : son_void_t
* Description    : This function processs  x2 link up  indication msg
******************************************************************************/
son_void_t
anr_process_x2_link_up_ind_for_nrt_updation
(
    x2ap_served_cell_info_arr_t  *p_srv_cell_info,
    son_u16 srv_cell_list,
    son_global_enb_id_t          *p_enb_id,
    son_nbr_enb_hash_rec_t       *p_enb_hash_rec
        /*SPR 17777 +-*/ 

)
{
    anr_cell_context_node_t       *p_cell_ctxt_tpl     = SON_PNULL;
    anr_cell_context_t            *p_cell_ctxt         = SON_PNULL;
    son_bool_et is_ant_cell_enabled  = SON_FALSE;
    son_x2_nrt_update_ind_t nrt_upd_ind         = {0};
    son_anr_updated_nrt_info_t     *p_oam_update_notification_for_add_op = SON_PNULL;
    son_anr_updated_nrt_info_t     *p_oam_update_notification_for_update_op = SON_PNULL;
    son_anr_rrm_updated_nrt_info_t *p_rrm_update_notification = SON_PNULL;
    /*SPR 16523 fix start*/
    son_bool_et                    send_ecu_to_all_peers      = SON_FALSE; 
    /*SPR 16523 fix stop*/
    /*SPR 17777 +-*/


    SON_UT_TRACE_ENTER();
    if (SON_PNULL == p_srv_cell_info || SON_PNULL == p_enb_id || SON_PNULL ==  p_enb_hash_rec)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_process_x2_link_up_ind_for_nrt_updation:"
                       "Invalid input params. p_srv_cell_info or p_enb_id or p_enb_hash_rec is NULL!");
        SON_UT_TRACE_EXIT();
        return;
    }

    /*SPR 17777 +-*/

    /*SPR 16523 fix start*/
    /*ENB CONFIG CHANGE REQ SPR:START*/
    /*if (SON_TRUE ==  is_x2_conn_triggred_by_oam ||
            1 < x2_get_active_nbr_count())
    {
        nrt_upd_ind.p_enb_rec_to_exclude  = p_enb_hash_rec;
    }

    if (SON_TRUE  == is_x2_conn_triggred_by_oam &&
            1 ==  x2_get_active_nbr_count())
    {
        nrt_upd_ind.is_local_update = SON_TRUE;
    }*/

    /*ENB CONFIG CHANGE REQ SPR:STOP*/
    /*SPR 16523 fix stop*/

    p_cell_ctxt_tpl =  anr_get_first_cell_ctxt();

    if (SON_PNULL == p_cell_ctxt_tpl)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_process_x2_link_up_ind_for_nrt_updation:"
                       "No Serving cell is configured in the Cell Context List");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_oam_update_notification_for_add_op = (son_anr_updated_nrt_info_t *)
                                           son_mem_get(sizeof(son_anr_updated_nrt_info_t));
    if (SON_PNULL == p_oam_update_notification_for_add_op)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_process_x2_link_up_ind_for_nrt_updation:"
                       "Memory Allocation failure");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_oam_update_notification_for_update_op = (son_anr_updated_nrt_info_t * )
                                              son_mem_get(sizeof(son_anr_updated_nrt_info_t));
    if (SON_PNULL == p_oam_update_notification_for_update_op)
    {
        if (SON_PNULL != p_oam_update_notification_for_add_op)
        {
            son_mem_free(p_oam_update_notification_for_add_op);
        }

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_process_x2_link_up_ind_for_nrt_updation:"
                       "Memory Allocation failure");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_rrm_update_notification = (son_anr_rrm_updated_nrt_info_t *)
                                son_mem_get(sizeof(son_anr_rrm_updated_nrt_info_t));
    if (SON_PNULL == p_rrm_update_notification)
    {
        if (SON_PNULL != p_oam_update_notification_for_add_op)
        {
            son_mem_free(p_oam_update_notification_for_add_op);
        }

        if (SON_PNULL != p_oam_update_notification_for_update_op)
        {
            son_mem_free(p_oam_update_notification_for_update_op);
        }

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_process_x2_link_up_ind_for_nrt_updation:"
                       "Memory Allocation failure");
        SON_UT_TRACE_EXIT();
        return;
    }

    while (SON_PNULL != p_cell_ctxt_tpl)
    {
        p_cell_ctxt = &p_cell_ctxt_tpl->data;

        if (ANR_CELL_STATE_ENABLED == p_cell_ctxt->current_cell_fsm_state)
        {
            /*Set all the notifications msg to various entites*/
            SON_MEMSET(p_oam_update_notification_for_add_op, 0,
                sizeof(son_anr_updated_nrt_info_t));
            SON_MEMSET(p_oam_update_notification_for_update_op, 0,
                sizeof(son_anr_updated_nrt_info_t));
            SON_MEMSET(p_rrm_update_notification, 0,
                sizeof(son_anr_rrm_updated_nrt_info_t));

            /*oam msg for added nbrs*/
            SON_MEMCPY(&p_oam_update_notification_for_add_op->src_cgi,
                &p_cell_ctxt->cell_id, sizeof(son_intra_rat_global_cell_id_t));

            /*oam msg for updated nbrs*/
            SON_MEMCPY(&p_oam_update_notification_for_update_op->src_cgi,
                &p_cell_ctxt->cell_id, sizeof(son_intra_rat_global_cell_id_t));

            /*rrm notification for all the updated nbrs*/
            SON_MEMCPY(&p_rrm_update_notification->src_cgi,
                &p_cell_ctxt->cell_id, sizeof(son_intra_rat_global_cell_id_t));

            is_ant_cell_enabled = SON_TRUE;
            if (SON_TRUE == anr_traverse_x2_link_up_ind_list(p_cell_ctxt,
                        p_srv_cell_info,
                        /*SPR 17777 +-*/
                        srv_cell_list,
                        p_enb_id,
                        p_oam_update_notification_for_add_op,
                        p_oam_update_notification_for_update_op,
                        p_rrm_update_notification))
            {
			/*SPR 16523 fix start*/
			/*If a first degree neighbor cell is modified such that
			 *it shall be communicated to all peers, set the flag to TRUE */
			send_ecu_to_all_peers = SON_TRUE;
		}
                anr_populate_x2_nrt_update_ind(&nrt_upd_ind,
                        &p_cell_ctxt->cell_id, SON_NRT_OP_UPDATE);
		/*
            else
            {
		// ENB CONFIG CHANGE REQ SPR:START
		//\SPR 15595 Fix Start - If condition modified/
                if ((SON_FALSE ==  is_x2_conn_triggred_by_oam ||
                            1 == x2_get_active_nbr_count()) &&
                        (p_oam_update_notification_for_update_op->nr_list.intra_rat_nr_list_size))
		 *SPR 15595 Fix End* 
                {
                    anr_populate_x2_nrt_update_ind(&nrt_upd_ind,
                            &p_cell_ctxt->cell_id, SON_NRT_OP_UPDATE);
                }

		//ENB CONFIG CHANGE REQ SPR:STOP
		}*/
		/*SPR 16523 fix stop*/

            /* Send update notification to OAM with operation performed as ADD if add
             * operation is done in NRT */
            anr_send_updated_ncl_to_oam(p_oam_update_notification_for_add_op);
            anr_send_updated_ncl_to_oam(p_oam_update_notification_for_update_op);

            anr_send_updated_ncl_to_rrm(p_rrm_update_notification,
                son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
        }

        anr_print_cell_context(p_cell_ctxt);
        /* Spr 17753 Changes Start */
        /* Start NR add timer if no timer is running */
        if(0 < p_oam_update_notification_for_add_op->nr_list.intra_rat_nr_list_size)
        {
            if((p_cell_ctxt->nr_add_wait_timer_val)&&(!p_cell_ctxt->son_nr_add_timer_id))
            {
                p_cell_ctxt->son_nr_add_timer_id = 
                    son_start_timer(p_cell_ctxt->nr_add_wait_timer_val * SON_MILLISECONDS_IN_A_SECOND, 
                                 SON_PNULL,
                                 0,
                                 SON_FALSE);

                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_BRIEF, "anr_process_x2_link_up_ind_for_nrt_updation:"
                    "NR add timer [Id: %p] started to wait for NR add req from OAM\n",
                    p_cell_ctxt->son_nr_add_timer_id);
            }
            else
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_BRIEF, "anr_process_x2_link_up_ind_for_nrt_updation:"
                    " either nr add timer is already running"
                    " ot timer val is zero"
                    " not starting timer again\n");
            } 
        }
        anr_find_conflict_in_nrt_n_send_conflict_ind(p_cell_ctxt);
        /* Spr 17753 Changes End */
        p_cell_ctxt_tpl = anr_get_next_node(p_cell_ctxt_tpl);
    }

    /*Send X2 msg*/
    if (SON_FALSE == is_ant_cell_enabled)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_process_x2_link_up_ind_for_nrt_updation:"
                       "No Serving cell is in ANR ENABLED state");
    }
    else if (nrt_upd_ind.num_srv_cells_list_size)
    {
        /*SPR 16523 fix start*/
	if(SON_FALSE ==  send_ecu_to_all_peers)
	{
	   /*In case no modification of a neighbor cell happened such that
            * it shall be communicated,send the ECU to the peer eNB, for which
            * link up ind has been received*/
	   nrt_upd_ind.p_enb_rec  = p_enb_hash_rec; 
	}
        nrt_upd_ind.is_local_update = SON_FALSE;	
        /*SPR 16523 fix stop*/
        son_create_send_buffer((son_u8 *)&nrt_upd_ind, SON_ANR_MODULE_ID,
            SON_X2_MODULE_ID, SONX2_NRT_UPDATE_IND,
            sizeof(nrt_upd_ind));

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_BRIEF, "anr_process_x2_link_up_ind_for_nrt_updation:"
                       "SONX2 NRT UPDATE sent to X2 ");
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_process_x2_link_up_ind_for_nrt_updation:"
                       "SONX2 NRT UPDATE can not be send to X2");
    }

    if (SON_PNULL != p_oam_update_notification_for_add_op)
    {
        son_mem_free(p_oam_update_notification_for_add_op);
    }

    if (SON_PNULL != p_oam_update_notification_for_update_op)
    {
        son_mem_free(p_oam_update_notification_for_update_op);
    }

    if (SON_PNULL != p_rrm_update_notification)
    {
        son_mem_free(p_rrm_update_notification);
    }

    SON_UT_TRACE_EXIT();
} /* anr_process_x2_link_up_ind_for_nrt_updation */

/******************************************************************************
* Function Name   : update_new_rec_with_existing_rec
* Inputs         : p_old_hash_rec  - Pointer to intra rat hash rec to be modfied.
*                 : p_intra_rat_hash_rec - Pointer to intra rat hash rec with
*                                           new values.
* Outputs        : None.
* Returns        : son_void_t
* Description    : This function updates the given intra rat hash rec with given
*                   values.
******************************************************************************/
static son_bool_et
update_new_rec_with_existing_rec
(
    anr_intra_rat_hash_rec_t *p_old_hash_rec,
    anr_intra_rat_hash_rec_t *p_intra_rat_hash_rec
)
{
    son_bool_et ret = SON_FALSE;


    SON_UT_TRACE_ENTER();

    p_intra_rat_hash_rec->neighbor_info.nr_status = p_old_hash_rec->neighbor_info.nr_status;
    p_intra_rat_hash_rec->neighbor_info.ho_status = p_old_hash_rec->neighbor_info.ho_status;
    p_intra_rat_hash_rec->neighbor_info.bitmask |= SON_ENABLE_NR_STATUS_PRESENT | SON_ENABLE_HO_STATUS_PRESENT;
    if (p_old_hash_rec->neighbor_info.bitmask & SON_INTRA_CSG_ID_PRESENT)
    {
        SON_MEMCPY(p_intra_rat_hash_rec->neighbor_info.csg_identity,
            p_old_hash_rec->neighbor_info.csg_identity,
            sizeof(p_intra_rat_hash_rec->neighbor_info.csg_identity));
        p_intra_rat_hash_rec->neighbor_info.bitmask |= SON_INTRA_CSG_ID_PRESENT;
    }

    if (p_old_hash_rec->neighbor_info.bitmask & SON_CELL_OFFSET_PRESENT)
    {
        p_intra_rat_hash_rec->neighbor_info.cell_specific_offset = p_old_hash_rec->neighbor_info.cell_specific_offset;
        p_intra_rat_hash_rec->neighbor_info.bitmask |= SON_CELL_OFFSET_PRESENT;
    }

    if (p_old_hash_rec->neighbor_info.bitmask & SON_RSRQ_PRESENT)
    {
        p_intra_rat_hash_rec->neighbor_info.rsrp = p_old_hash_rec->neighbor_info.rsrp;
        p_intra_rat_hash_rec->neighbor_info.bitmask |= SON_RSRQ_PRESENT;
    }

    if (p_old_hash_rec->neighbor_info.bitmask & SON_RSRP_PRESENT)
    {
        p_intra_rat_hash_rec->neighbor_info.rsrq = p_old_hash_rec->neighbor_info.rsrq;
        p_intra_rat_hash_rec->neighbor_info.bitmask |= SON_RSRP_PRESENT;
    }

    if (p_old_hash_rec->neighbor_info.bitmask & SON_ACCESS_MODE_PRESENT)
    {
        p_intra_rat_hash_rec->neighbor_info.access_mode = p_old_hash_rec->neighbor_info.access_mode;
        p_intra_rat_hash_rec->neighbor_info.bitmask |= SON_ACCESS_MODE_PRESENT;
    }

    if (p_old_hash_rec->neighbor_info.bitmask & SON_CM_CELL_OFFSET_PRESENT)
    {
        p_intra_rat_hash_rec->neighbor_info.cm_cell_specific_offset =
            p_old_hash_rec->neighbor_info.cm_cell_specific_offset;
        p_intra_rat_hash_rec->neighbor_info.bitmask |= SON_CM_CELL_OFFSET_PRESENT;
    }

    if (p_old_hash_rec->neighbor_info.bitmask & SON_BROADCAST_STATUS_PRESENT)
    {
        p_intra_rat_hash_rec->neighbor_info.broadcast_status = p_old_hash_rec->neighbor_info.broadcast_status;
        p_intra_rat_hash_rec->neighbor_info.bitmask |= SON_BROADCAST_STATUS_PRESENT;
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* update_new_rec_with_existing_rec */

/******************************************************************************
* Function Name   : anr_update_nrt_for_change_in_cgi
* Inputs         : p_cell_ctxt         - Pointer to cell ctx
*                 : p_old_hash_rec      - Pointer to old hash record.
*                 : p_new_hash_rec      - Pointer to new hash record.
*                 : trigger_pci_confl_conf - boolean to trigger the pci conflict
*                                            confusion indication
*                 : p_nbrs_nbr_list      - Pointer to the neighbors neighbor
* Outputs         : p_oam_notif_for_add  - Pointer to OAM notification for
*                                           neighbors added in NRT.
*                 : p_oam_notif_for_upd  - Pointer to OAM notification for
*                                          neighbors  updated in nRT.
*                 : p_oam_notif_for_del  - Pointer to the OAM notification
*                                          for the neighbor deleted from NRT.
*                 : p_rrm_notif          - Pointer to the RRM notification
* Returns        : son_void_t
* Description    : This function can only be called when there is modifcation in CGI
*                   via eNB config udpate indication msg and no modification in
*                   the pci and earfcn of the neighbor cell.This function
*                   shall not be called for any other situation
******************************************************************************/
son_void_t
anr_update_nrt_for_change_in_cgi
(
    anr_cell_context_t                   *p_cell_ctxt,
    anr_intra_rat_hash_nr_tuple_t        *p_old_hash_tpl,
    anr_intra_rat_hash_rec_t             *p_new_hash_rec,
    son_anr_updated_nrt_info_t           *p_oam_notif_for_add,
        /*SPR 17777 +-*/ 
    son_anr_updated_nrt_info_t           *p_oam_notif_for_del,
    son_anr_rrm_updated_nrt_info_t       *p_rrm_notif,
    son_bool_et trigger_pci_confl_conf
        /*SPR 17777 +-*/ 
)
{
    /*SPR 15438 Lines deleted*/
    son_error_et error_code                   = SON_NO_ERROR;
    anr_intra_rat_hash_rec_t old_hash_rec;

    /*SPR 15438 Lines deleted*/


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&old_hash_rec, 0, sizeof(anr_intra_rat_hash_rec_t));
    update_new_rec_with_existing_rec(&p_old_hash_tpl->data, p_new_hash_rec);

    /*SPR 15438 Lines deleted*/

    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
        SON_DETAILED, 
                      "Form the eNB config updated ind msg's modify list"
                      "NR has Old ECGI[Cell id:0x%x]] with PCI [%d] and EARFCN [%d] and has"
                      "new ECGI [Cell id:0x%x] with  PCI [%d] and EARFCN [%d]",
        son_convert_char_cell_id_to_int_cell_id(p_old_hash_tpl->data.neighbor_info.nbr_cgi.cell_identity),
        p_old_hash_tpl->data.neighbor_info.pci, p_old_hash_tpl->data.neighbor_info.earfcn,
        son_convert_char_cell_id_to_int_cell_id(p_new_hash_rec->neighbor_info.nbr_cgi.cell_identity),
        p_new_hash_rec->neighbor_info.pci, p_new_hash_rec->neighbor_info.earfcn);

    /*Delete the existing CGI and place all its data in old_hash_rec*/
    anr_intra_rat_del_nr(p_cell_ctxt,
        &p_old_hash_tpl->data.neighbor_info.nbr_cgi,
        SON_TRUE, &error_code,
        &old_hash_rec, p_oam_notif_for_del,
        p_rrm_notif, trigger_pci_confl_conf);

    /* SPR 15438 start */
    p_old_hash_tpl = SON_PNULL;
    /* SPR 15438 end */
    error_code = SON_NO_ERROR;

    /*SPR 15438 Lines deleted*/

	    /*SPR 15438 - Start*/
            /*Add the hash record in the OAM notifcation msg*/
            if (p_oam_notif_for_add)
            {
                p_oam_notif_for_add->nrt_op = SON_NRT_OP_ADD;
                anr_construct_oam_updated_ncl(&p_oam_notif_for_add->nr_list,
                    &p_new_hash_rec->neighbor_info, SON_FALSE);
            }
	    /*SPR 15438 - End*/


    SON_UT_TRACE_EXIT();
} /* anr_update_nrt_for_change_in_cgi */

/******************************************************************************
* Function Name   : anr_nr_update_nbr_cells
* Inputs         : p_cell_ctxt         - Pointer to cell ctxt
*                 : old_cell_id         - Old Cell Id.
*                 : p_srv_cell_info     - Pointer to the list of neighbor
*                                         to be modified
*                 : x2_status           - x2 status flag
*                 : p_old_enb_id            - Pointer to global eNB id
*                 : p_new_enb_id            - Pointer to global eNB id
* Outputs        : p_oam_notif_for_add - Pointer to oam notification for add
*                                         nieghbor from nrt
*                 : p_oam_notif_for_upd - Pointer to oam notification for
*                                         update neighbor  from nrt
*                 : p_oam_notif_for_del - Pointer to oam notification for
*                                         delete neighbor from nrt
*                 : p_rrm_notif         - Pointer to the RRM notification
*                                         message
* Returns        : son_bool_et         - (SON_TRUE/SON_FALSE)
* Description    : This function is used to process the neighbors available
*                   in the modify list eNB Config update indication message.
******************************************************************************/
static son_bool_et
anr_nr_update_nbr_cells
(
    anr_cell_context_t                      *p_cell_ctxt,
    son_u8 old_cell_id,
    x2ap_served_cell_info_arr_t             *p_srv_cell_info,
    /*SPR 17777 +-*/
    /* SPR_19279_START */
    son_global_enb_id_t                     *p_old_enb_id,
    son_global_enb_id_t                     *p_new_enb_id,
    /* SPR_19279_END */
    son_anr_updated_nrt_info_t               *p_oam_notif_for_add,
    son_anr_updated_nrt_info_t               *p_oam_notif_for_upd,
    son_anr_updated_nrt_info_t               *p_oam_notif_for_del,
    son_anr_rrm_updated_nrt_info_t              *p_rrm_notif
)
{
    anr_intra_rat_hash_nr_tuple_t *p_tpl        = SON_PNULL;

    anr_intra_rat_hash_rec_t *p_hash_rec        = SON_PNULL;
    anr_intra_rat_hash_rec_t intra_rat_hash_rec;
    anr_intra_rat_hash_rec_t old_hash_rec;
    anr_intra_rat_hash_rec_t temp_hash_rec;
    son_bool_et trigger_pci_conf_con = SON_FALSE;

    son_bool_et ret                = SON_FALSE;
    son_error_et error_code          = SON_NO_ERROR;
    SON_HASH                *p_hash             =
        &p_cell_ctxt->intra_rat_neighbor_table;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&intra_rat_hash_rec, 0, sizeof(anr_intra_rat_hash_rec_t));
    SON_MEMSET(&temp_hash_rec, 0, sizeof(anr_intra_rat_hash_rec_t));

    /*Create ECGI*/
    get_ecgi_from_enb_id_cell_id(&temp_hash_rec.neighbor_info.nbr_cgi,
        /* SPR_19279_START */
        old_cell_id, p_old_enb_id);
        /* SPR_19279_END */

    p_tpl =  anr_intra_rat_find_nr(p_hash, &temp_hash_rec.neighbor_info.nbr_cgi,
        &error_code);

    /*klockwork fix*/
    if (SON_NO_ERROR == error_code && SON_PNULL != p_tpl)
    {
        /*take the current NRT data in a local hash rec*/
        old_hash_rec = p_tpl->data;

        /*construct the intra rat hash rec from l3 record*/
        anr_construct_l3_to_son_intra_rat_hash_rec(&intra_rat_hash_rec,
            p_srv_cell_info,
            /* SPR_19279_START */
            p_new_enb_id);
            /* SPR_19279_END */


        /*IF EARFCN is not available then return form the
         * function with SON_FALSE*/
        /* Coverity Fix: 23467 */
        if (!(SON_EARFCN_PRESENT & intra_rat_hash_rec.neighbor_info.bitmask))
        {
            return ret;
        }

        /*Do not do the assignment for the x2
         * status as it is READ only filed*/

        /* SPR_19279_START */
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,SON_BRIEF,
        "p_new_plmn_id %d %d %d %d %d %d, p_old_plmn_id %d %d %d %d %d %d, intra_rat_hash_rec.plmn_id %d %d %d %d %d %d, Brdcst %d %d %d",
                p_new_enb_id->plmn_id.mcc[0],
                p_new_enb_id->plmn_id.mcc[1],
                p_new_enb_id->plmn_id.mcc[2],
                p_new_enb_id->plmn_id.mnc[0],
                p_new_enb_id->plmn_id.mnc[1],
                p_new_enb_id->plmn_id.mnc[2],
                p_old_enb_id->plmn_id.mcc[0],
                p_old_enb_id->plmn_id.mcc[1],
                p_old_enb_id->plmn_id.mcc[2],
                p_old_enb_id->plmn_id.mnc[0],
                p_old_enb_id->plmn_id.mnc[1],
                p_old_enb_id->plmn_id.mnc[2],
                intra_rat_hash_rec.enb_id.plmn_id.mcc[0],
                intra_rat_hash_rec.enb_id.plmn_id.mcc[1],
                intra_rat_hash_rec.enb_id.plmn_id.mcc[2],
                intra_rat_hash_rec.enb_id.plmn_id.mnc[0],
                intra_rat_hash_rec.enb_id.plmn_id.mnc[1],
                intra_rat_hash_rec.enb_id.plmn_id.mnc[2],
                p_srv_cell_info->cell_info.bcast_plmn_info.plmn_identity[0].plmn_id[0],
                p_srv_cell_info->cell_info.bcast_plmn_info.plmn_identity[0].plmn_id[1],
                p_srv_cell_info->cell_info.bcast_plmn_info.plmn_identity[0].plmn_id[2]);
        /* SPR_19279_END */

        /*If Old Cell Id is same as New CGI*/
        /* SPR_19279_START */
        if(0 == SON_MEMCMP(&old_hash_rec.neighbor_info.nbr_cgi,
                           &intra_rat_hash_rec.neighbor_info.nbr_cgi,sizeof(son_intra_rat_global_cell_id_t)))
        /* SPR_19279_END */
        {
            p_hash_rec = anr_intra_rat_modify_nr(p_cell_ctxt,
                &intra_rat_hash_rec,
                SON_FALSE,
                &error_code,
                p_oam_notif_for_add,
                p_oam_notif_for_upd,
                p_rrm_notif,
                SON_TRUE,
                SON_FALSE);

            if (SON_NO_ERROR == error_code)
            {
                /* Spr 17753 Changes Start */  
                anr_process_inra_rat_nbr_nbrs(&p_tpl->data, &p_srv_cell_info->nbour_cell_info,p_cell_ctxt);
                /* Spr 17753 Changes End */  
            }

            /*Modification is done successfully*/
            if (SON_PNULL != p_hash_rec &&
                ((p_hash_rec->neighbor_info.pci != intra_rat_hash_rec.neighbor_info.pci) ||
                 (p_hash_rec->neighbor_info.earfcn != intra_rat_hash_rec.neighbor_info.earfcn) ||
		 /*SPR 16523 fix start*/
		 (p_hash_rec->neighbor_info.tac != intra_rat_hash_rec.neighbor_info.tac) ))
		 /*SPR 16523 fix stop*/
            {
                ret = SON_TRUE;
            }
        }
        else
        {
            if (SON_NR_LOCKED != old_hash_rec.neighbor_info.nr_status)
            {
		/*SPR 15438 Lines deleted*/


                anr_update_nrt_for_change_in_cgi(p_cell_ctxt,
                    p_tpl,
                    &intra_rat_hash_rec,
                    p_oam_notif_for_add,
        /*SPR 17777 +-*/ 
                    p_oam_notif_for_del,
                    p_rrm_notif, trigger_pci_conf_con);
        /*SPR 17777 +-*/ 

		/* SPR 15438 start */
		p_tpl = SON_PNULL;
		/* SPR 15438 end */
                ret = SON_TRUE;
            }
            else
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_WARNING, 
                               "NR with old ECGI 0x%x is hash NR LOCKED status"
                               " no modifications allowed",
                    son_convert_char_cell_id_to_int_cell_id(old_hash_rec.neighbor_info.nbr_cgi.cell_identity));
            }
        }
    }
    else
    {
        ret =      anr_nr_add_nbr_cells(p_cell_ctxt,
            p_srv_cell_info,
        /*SPR 17777 +-*/ 
            /* SPR_19279_START */
            p_new_enb_id,
            /* SPR_19279_END */
            p_oam_notif_for_add,
            p_oam_notif_for_upd,
            p_rrm_notif);

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_INFO, "No Such neighbor found with given old ECGI.Cell Added in NRT");
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_nr_update_nbr_cells */

/******************************************************************************
* Function Name  : anr_nr_delete_nbr_cells
* Inputs         : p_cell_ctxt      - Pointer to cell context
*                 : cell_id          - Cell Id
*                 : p_enb_id         - Pointer to global eNB Id
* Outputs        : p_oam_notif      - Pointer to oam notification msg
*                 : p_rrm_notif      - Pointer to rrm notification msg
* Returns        : son_bool_et      - (SON_TRUE/SON_FALSE)
* Description    : This function deletes NR identified in enB config update
******************************************************************************/
son_bool_et
anr_nr_delete_nbr_cells
(
    anr_cell_context_t *p_cell_ctxt,
    son_u8 cell_id,
    son_global_enb_id_t *p_enb_id,
    son_anr_updated_nrt_info_t               *p_oam_notif,
    son_anr_rrm_updated_nrt_info_t               *p_rrm_notif
)
{
    anr_intra_rat_hash_rec_t intra_rat_hash_rec;
    anr_intra_rat_hash_rec_t hash_rec;
    anr_intra_rat_hash_nr_tuple_t  *p_tpl = SON_PNULL;
    son_error_et error_code          = SON_NO_ERROR;
    son_bool_et ret = SON_FALSE;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&intra_rat_hash_rec, 0, sizeof(anr_intra_rat_hash_rec_t));
    SON_MEMSET(&hash_rec, 0, sizeof(hash_rec));

    /* Get the SON Intra rat CEll id */
    get_ecgi_from_enb_id_cell_id(&hash_rec.neighbor_info.nbr_cgi,
        cell_id, p_enb_id);

    /*Find the Cell from the NRT*/
    p_tpl = anr_intra_rat_find_nr(&p_cell_ctxt->intra_rat_neighbor_table, &hash_rec.neighbor_info.nbr_cgi, &error_code);

    /*Check if nr status is NRLOCKED*/
    if (SON_PNULL != p_tpl && SON_NR_LOCKED != p_tpl->data.neighbor_info.nr_status)
    {
        /*Delete the given NR form the NRT*/
        anr_intra_rat_del_nr(p_cell_ctxt, &hash_rec.neighbor_info.nbr_cgi, SON_TRUE,
            &error_code, &intra_rat_hash_rec, p_oam_notif, p_rrm_notif, SON_TRUE);
        if (SON_NO_ERROR == error_code)
        {
            ret = SON_TRUE;
        }
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_nr_delete_nbr_cells */

/******************************************************************************
* Function Name  : anr_traverse_enb_config_update_add_list
* Inputs         : p_cell_ctxt - Cell Context pointer
*                 : p_add_list - Neighbor List from L3 to be added in NRT
*                 : x2_status   - X2 status value
*                 : p_enb_id  -  Pointer to the eNB Id
* Outputs         : p_oam_notif_for_add - Pointer to oam notification msg for add
*                 : p_oam_notif_for_upd - Pointer to oam notification msg for upd
*                 : p_rrm_notif         - Pointer to rrm notification msg
* Returns        : son_bool_et         - (SON_TRUE/SON_FALSE)
* Description    : This function traverses all the neigbors in the eNB config update
*                   add list to add them in NRT.
******************************************************************************/
son_bool_et
anr_traverse_enb_config_update_add_list
(
    anr_cell_context_t                      *p_cell_ctxt,
    x2ap_served_cells_to_add_list_t         *p_add_list,
    /*SPR 17777 +-*/
    son_global_enb_id_t                     *p_enb_id,
    son_anr_updated_nrt_info_t               *p_oam_notif_for_add,
    son_anr_updated_nrt_info_t               *p_oam_notif_for_upd,
    son_anr_rrm_updated_nrt_info_t               *p_rrm_notif

)
{
    son_u16 idx = 0;
    son_bool_et ret  = SON_FALSE;


    SON_UT_TRACE_ENTER();
    for (idx = 0; idx < p_add_list->num_served_cell; idx++)
    {
        ret |=  anr_nr_add_nbr_cells(p_cell_ctxt,
            &p_add_list->cell_list[idx],
        /*SPR 17777 +-*/ 
            p_enb_id,
            p_oam_notif_for_add,
            p_oam_notif_for_upd,
            p_rrm_notif);
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_traverse_enb_config_update_add_list */

/******************************************************************************
* Function Name  : anr_traverse_enb_config_update_ind_del_list
* Inputs         : p_cell_ctxt - Cell Context pointer
*                 : p_del_list - Neighbor List from L3 to be deleted in NRT
*                 : p_enb_id  -  Pointer to the eNB Id
* Outputs        : p_oam_notif - Pointer to oam notification msg
*                 : p_rrm_notif - Pointer to rrm notification msg
* Returns        : son_bool_et
* Description    : This function traverses all the neigbors in the eNB config update
*                   delete list to deletes NRs in the NRT
******************************************************************************/
son_bool_et
anr_traverse_enb_config_update_ind_del_list
(
    anr_cell_context_t                      *p_cell_ctxt,
    x2ap_served_cells_to_delete_list_t      *p_del_list,
    son_global_enb_id_t                     *p_enb_id,
    son_anr_updated_nrt_info_t               *p_oam_notif,
    son_anr_rrm_updated_nrt_info_t               *p_rrm_notif
)
{
    son_u16 idx = 0;
    son_bool_et ret = SON_FALSE;


    SON_UT_TRACE_ENTER();
    for (idx = 0; idx < p_del_list->num_served_cell; idx++)
    {
        ret |= anr_nr_delete_nbr_cells(p_cell_ctxt,
            p_del_list->cell_id_list[idx],
            p_enb_id,
            p_oam_notif,
            p_rrm_notif);
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_traverse_enb_config_update_ind_del_list */

/******************************************************************************
* Function Name  : anr_traverse_enb_config_update_ind_modify_list
* Inputs         : p_cell_ctxt - Cell Context pointer
*                 : p_modify_list - Neighbor List from L3 to be modifed in NRT
*                 : x2_status     - X2 status value
*                 : p_enb_id  -  Pointer to the eNB Id
* Outputs        : p_oam_notif_for_add - Pointer to oam notification msg for add
*                                         nr
*                 : p_oam_notif_for_upd - Pointer to oam notification msg for upd
*                                         nr
*                 : p_oam_notif_for_del - Pointer to oam notification msg for del
*                                         nr
*                 : p_rrm_notif         - Pointer to rrm notification msg
* Returns        : son_bool_et
* Description    : This function traverses all the neigbors in the eNB config update
*                   modify list to modify/delete NRs in the NRT
******************************************************************************/
son_bool_et
anr_traverse_enb_config_update_ind_modify_list
(
    anr_cell_context_t                      *p_cell_ctxt,
    x2ap_served_cells_to_modify_list_t      *p_modify_list,
    /*SPR 17777 +-*/
    /* SPR_19279_START */
    son_global_enb_id_t                     *p_old_enb_id,
    son_global_enb_id_t                     *p_new_enb_id,
    /* SPR_19279_END */
    son_anr_updated_nrt_info_t               *p_oam_notif_for_add,
    son_anr_updated_nrt_info_t               *p_oam_notif_for_upd,
    son_anr_updated_nrt_info_t               *p_oam_notif_for_del,
    son_anr_rrm_updated_nrt_info_t               *p_rrm_notif
)
{
    son_u16 idx  = 0;
    son_bool_et ret = SON_FALSE;


    SON_UT_TRACE_ENTER();
    for (idx = 0; idx < p_modify_list->num_served_cell; idx++)
    {
        if (X2AP_DEACTIVATION_IND_PRESENT & p_modify_list->cell_list[idx].bitmask)
        {
            /*Delete the data in the old cgi*/
            ret |=    anr_nr_delete_nbr_cells(p_cell_ctxt,
                p_modify_list->cell_list[idx].old_cell_id,
                /* SPR_19279_START */
                p_old_enb_id,
                /* SPR_19279_END */
                p_oam_notif_for_del,
                p_rrm_notif);
        }
        else
        {
            ret |=  anr_nr_update_nbr_cells(p_cell_ctxt,
                p_modify_list->cell_list[idx].old_cell_id,
                &p_modify_list->cell_list[idx].served_cell_info,
                /*SPR 17777 +-*/
                /* SPR_19279_START */
                p_old_enb_id,
                p_new_enb_id,
                /* SPR_19279_END */
                p_oam_notif_for_add,
                p_oam_notif_for_upd,
                p_oam_notif_for_del,
                p_rrm_notif);
        }
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_traverse_enb_config_update_ind_modify_list */

/******************************************************************************
* Function Name  : anr_clear_nr_nbrs_list
* Inputs         : p_list -   Pointer to the transaction Id list
* Returns        : son_void_t
* Description    : This function clears the entire NR nbrs list for a
*                  configured cell
******************************************************************************/
son_void_t
anr_clear_nr_nbrs_list
(
    SON_LIST *p_list
)
{
    SON_LIST_NODE *p_list_node = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_list_node = list_pop_node(p_list);

    while (p_list_node)
    {
        anr_nr_nbr_list_node_t *p_data = YMEMBEROF(anr_nr_nbr_list_node_t,
            nr_nbr_anchor, p_list_node);
        /* Free the memory occupied by the node */
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_BRIEF,
            "Cell Identity [Cell id:0x%x]",
            son_convert_char_cell_id_to_int_cell_id(p_data->nr_nbr_data.nr_nbr_cgi.cell_identity));
        son_mem_free(p_data);

        p_list_node = list_pop_node(p_list);
    }

    SON_UT_TRACE_EXIT();
} /* anr_clear_nr_nbrs_list */

/*****************************************************************************
 * Function Name  : anr_get_first_nr_nbr
 * Inputs         : anr_nr_nbr_list_node_t*
 * Outputs        : None
 * Returns        : Pointer to first record  in nr neighbor list
 * Description    : This function determine the first element in the nr neighbor
 *                : List
 ******************************************************************************/
anr_nr_nbr_list_node_t *
anr_get_first_nr_nbr
(
    SON_LIST *p_list
)
{
    SON_LIST_NODE   *p_node = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (list_count(p_list) > 0)
    {
        p_node = get_first_list_node(p_list);
        if (SON_PNULL != p_node)
        {
            SON_UT_TRACE_EXIT();
            return (anr_nr_nbr_list_node_t *)
                   YMEMBEROF(anr_nr_nbr_list_node_t, nr_nbr_anchor, p_node);
        }
    }

    SON_UT_TRACE_EXIT();
    return SON_PNULL;
} /* anr_get_first_nr_nbr */

/*****************************************************************************
 * Function Name  : anr_get_next_nr_nbr
 * Inputs         : p_node   Pointer to the structure in nr nbr list
 * Outputs        : None
 * Returns        : cell context pointer of the next cell
 * Description    : This function gets the  next cell context in the list
 ******************************************************************************/
anr_nr_nbr_list_node_t *
anr_get_next_nr_nbr
(
    anr_nr_nbr_list_node_t  *p_node
)
{
    SON_LIST_NODE   *p_list_node    = SON_PNULL;


    SON_UT_TRACE_ENTER();
    if (SON_PNULL != p_node)
    {
        p_list_node = get_next_list_node(&p_node->nr_nbr_anchor);
        if (SON_PNULL != p_list_node)
        {
            SON_UT_TRACE_EXIT();
            return (anr_nr_nbr_list_node_t *)
                   YMEMBEROF(anr_nr_nbr_list_node_t, nr_nbr_anchor, p_list_node);
        }
    }

    SON_UT_TRACE_EXIT();
    return SON_PNULL;
} /* anr_get_next_nr_nbr */

/*****************************************************************************
* Function Name  : anr_insert_in_nr_nbr_list
* Inputs         : p_list  - NR Nbr List
*                : p_x2ap_nbr_info - L3 nr nbr element
* Outputs        : None
* Returns        : son_void_t
* Description    : This function inserts a nr neighbors in NR nbr list
*****************************************************************************/
son_bool_et
anr_insert_in_nr_nbr_list
(
    SON_LIST *p_list,
    /* Spr 17753 Changes Start */  
    x2ap_nbour_info_t *p_x2ap_nbr_info,
    anr_cell_context_t *p_cell_ctxt
    /* Spr 17753 Changes End */  
)
{
    anr_nr_nbr_list_node_t  *p_list_node = SON_PNULL;
    son_bool_et ret_val      = SON_FALSE;


    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_list || SON_PNULL == p_x2ap_nbr_info)
    {
        SON_LOG( anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR,
            "Input parameters p_list or p_x2ap_nbr_info is NULL");
        SON_UT_TRACE_EXIT();
        return ret_val;
    }

    p_list_node = son_mem_get(sizeof(anr_nr_nbr_list_node_t));
    if (SON_PNULL != p_list_node)
    {
        SON_MEMSET(p_list_node, 0, sizeof(anr_nr_nbr_list_node_t));

        x2_convert_l3_plmn_id_to_son_plmn_id(
            &p_x2ap_nbr_info->ecgi.plmn_identity,
            &p_list_node->nr_nbr_data.nr_nbr_cgi.plmn_id);

        SON_MEMCPY(p_list_node->nr_nbr_data.nr_nbr_cgi.cell_identity
            , p_x2ap_nbr_info->ecgi.eutran_cell_id, SON_CELL_ID_OCTET_SIZE);

        /* Spr 17753 Changes Start */ 
        /* Not inserting the serving cell as second degree neighbor */
        if(0 != SON_MEMCMP(&p_list_node->nr_nbr_data.nr_nbr_cgi,&p_cell_ctxt->cell_id,
                                       sizeof(son_intra_rat_global_cell_id_t)))
        {
        p_list_node->nr_nbr_data.pci  = p_x2ap_nbr_info->pci;

        p_list_node->nr_nbr_data.earfcn = p_x2ap_nbr_info->earfcn.earfcn;

        list_insert_node(p_list,
            &p_list_node->nr_nbr_anchor);

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_INFO,
            "Added NR Neighbor [Cell id:0x%x] in the NR Neighbor list",
            son_convert_char_cell_id_to_int_cell_id(p_list_node->nr_nbr_data.nr_nbr_cgi.cell_identity));
    }
        ret_val = SON_TRUE;
        /* Spr 17753 Changes End */ 
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR,
            "Memory allocation failure");
    }

    SON_UT_TRACE_EXIT();
    return ret_val;
} /* anr_insert_in_nr_nbr_list */

/******************************************************************************
* Function Name  : anr_process_inra_rat_nbr_nbrs
* Inputs         : p_nr            - Pointer to intra rat hash table
*                 : p_nbrs_nbr_list - Pointer to x2 nbr list
* Outputs        : None
* Returns        : son_void_t
* Description    : This function processes intra rat nbr
******************************************************************************/
son_void_t
anr_process_inra_rat_nbr_nbrs
(
    anr_intra_rat_hash_rec_t *p_nr,
    /* Spr 17753 Changes Start */ 
    x2ap_nbour_cell_info_t *p_nbrs_nbr_list,
    anr_cell_context_t     *p_cell_ctxt
    /* Spr 17753 Changes End */ 
)
{
    SON_UT_TRACE_ENTER();
    son_u16 idx = 0;
    if (SON_PNULL == p_nbrs_nbr_list || SON_PNULL == p_nr)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR,
            "Neighbor's neighbor could not be processed, p_nr or p_nbrs_nbr_list is NULL");
        SON_UT_TRACE_EXIT();
        return;
    }

    /*If there is no neighbor */
    if (!p_nbrs_nbr_list->num_neighbour_cell)
    {
        if (!p_nr->no_nr_neighbors_ts || list_count(&p_nr->anr_nr_nbr_list))
        {
            anr_clear_nr_nbrs_list(&p_nr->anr_nr_nbr_list);
            list_init(&p_nr->anr_nr_nbr_list);

        }

        SON_UT_TRACE_EXIT();
        return;
    }

    p_nr->no_nr_neighbors_ts = 0;
    if (list_count(&p_nr->anr_nr_nbr_list))
    {
        anr_clear_nr_nbrs_list(&p_nr->anr_nr_nbr_list);
        list_init(&p_nr->anr_nr_nbr_list);
    }

    for (idx = 0; idx < p_nbrs_nbr_list->num_neighbour_cell; idx++)
    {
        anr_insert_in_nr_nbr_list(&p_nr->anr_nr_nbr_list,
           /* Spr 17753 Changes Start */ 
            &p_nbrs_nbr_list->neighbour_info_arr[idx],
                    p_cell_ctxt);
           /* Spr 17753 Changes End */ 
    }

    SON_UT_TRACE_EXIT();
} /* anr_process_inra_rat_nbr_nbrs */

/* SPR-10331 Fix Starts */
/*****************************************************************************
 * Function Name  : anr_convert_son_enbid_to_rrm_enbid
 * Inputs         : p_son_enb_id - Pointer to the eNB Id value defined by SON
 * Outputs        : p_rrm_enb_id - Pointer to the eNB Id value defined by RRM
 * Returns        : SON_SUCCESS - If conversion is successful
 *                  SON_FAILURE - Otherwise
 * Description    : This is the function which is used to convert eNB Id value 
 *                  stored by SON to the eNB Id value required by RRM
 *****************************************************************************/
son_return_et
anr_convert_son_enbid_to_rrm_enbid
(
    son_global_enb_id_t *p_son_enb_id,
    rrm_global_enb_id_t *p_rrm_enb_id
)
{
    son_return_et res = SON_SUCCESS;
    SON_UT_TRACE_ENTER();

    switch (p_son_enb_id->enb_type)
    {
        case SON_HOME_ENB:
            {
                p_rrm_enb_id->enb_type = RRM_ENB_TYPE_HOME;
                SON_MEMCPY(&p_rrm_enb_id->plmn_id, &p_son_enb_id->plmn_id,
                        sizeof(rrm_oam_cell_plmn_info_t));
                SON_MEMCPY(&p_rrm_enb_id->eNb_id, &p_son_enb_id->enb_id,
                        HOME_ENB_ID_OCTET_SIZE * sizeof(son_u8));
                break;
            }
        case SON_MACRO_ENB:
            {
                p_rrm_enb_id->enb_type = RRM_ENB_TYPE_MACRO;
                SON_MEMCPY(&p_rrm_enb_id->plmn_id, &p_son_enb_id->plmn_id,
                        sizeof(rrm_oam_cell_plmn_info_t));
                SON_MEMCPY(&p_rrm_enb_id->eNb_id, &p_son_enb_id->enb_id,
                        HOME_ENB_ID_OCTET_SIZE * sizeof(son_u8));
                break;
            }
        default: 
            {
                res = SON_FAILURE;
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_ERROR, "anr_convert_son_enbid_to_rrm_enbid:"
                        "Invalid SON eNB Type[%u]", p_son_enb_id->enb_type);
                break;
            }
    }

    SON_UT_TRACE_EXIT();
    return res;
}

/*****************************************************************************
 * Function Name  : anr_send_enb_config_update_ind_to_rrm
 * Inputs         : p_cspl_hdr    - Pointer to the CSPL Header Information
 *                  p_peer_enb_id - Pointer to peer eNB Id
 *                : p_l3_ind      - Pointer to the incoming message from L3 X2AP
 * Outputs        : None
 * Returns        : None
 * Description    : This is the function which is used to forward the GU Group
 *                  Id information recevied from L3 to RRM
 *****************************************************************************/
son_void_t
anr_send_enb_config_update_ind_to_rrm
(
    STACKAPIHDR         *p_cspl_hdr,
    son_global_enb_id_t *p_peer_enb_id,
    son_void_t          *p_l3_ind
)
{
    son_rrm_enb_config_update_ind_t ind;
    son_u8 idx = 0;
    SON_UT_TRACE_ENTER();

    SON_MEMSET(&ind, 0, sizeof(son_rrm_enb_config_update_ind_t));

    if (SON_FAILURE == 
            anr_convert_son_enbid_to_rrm_enbid(p_peer_enb_id, 
                &ind.nbr_enb_id))
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "Message %u could not "
                "be send from %u to %u",
                RRM_SON_ENB_CONFIG_UPDATE_IND,
                SON_ANR_MODULE_ID,
                SON_RRM_MODULE_ID);
        SON_UT_TRACE_EXIT();
        return;
    }

    if (SONX2_ENB_CONFIG_UPDATE_IND == p_cspl_hdr->api)
    {
        son_x2_enb_config_update_ind_t *p_l3_ecu_ind = p_l3_ind;
        if (X2AP_GU_GROUP_ID_TO_ADD_LIST_PRESENT & 
                p_l3_ecu_ind->p_update_ind->bitmask)
        {
            ind.bitmask |= RRM_SON_GU_GROUP_ID_TO_ADD_LIST_PRESENT;
            ind.add_gu_id.num_gu_group_id = 
                p_l3_ecu_ind->p_update_ind->add_gu_id.num_gu_group_id;
            for (idx = 0; idx < ind.add_gu_id.num_gu_group_id; idx++)
            {
                x2_convert_l3_plmn_id_to_son_plmn_id(&p_l3_ecu_ind->
                        p_update_ind->add_gu_id.gu_id_list[idx].plmn_id,
                        (son_cell_plmn_info_t *)&ind.add_gu_id.
                        gu_id_list[idx].plmn_id);

                SON_MEMCPY(ind.add_gu_id.gu_id_list[idx].mme_group_id, 
                        p_l3_ecu_ind->p_update_ind->add_gu_id.gu_id_list[idx].
                        mme_group_id,
                        sizeof(son_u8) * RRM_MAX_MME_GRP_BYTES);
            }
        }
        if (X2AP_GU_GROUP_ID_TO_DELETE_LIST_PRESENT & 
                p_l3_ecu_ind->p_update_ind->bitmask)
        {
            ind.bitmask |= RRM_SON_GU_GROUP_ID_TO_DELETE_LIST_PRESENT;
            ind.delete_gu_id.num_gu_group_id = 
                p_l3_ecu_ind->p_update_ind->delete_gu_id.num_gu_group_id;
            for (idx = 0; idx < ind.delete_gu_id.num_gu_group_id; idx++)
            {
                x2_convert_l3_plmn_id_to_son_plmn_id(&p_l3_ecu_ind->
                        p_update_ind->delete_gu_id.gu_id_list[idx].plmn_id, 
                        (son_cell_plmn_info_t *)&ind.delete_gu_id.
                        gu_id_list[idx].plmn_id);

                SON_MEMCPY(ind.delete_gu_id.gu_id_list[idx].mme_group_id, 
                        p_l3_ecu_ind->p_update_ind->delete_gu_id.
                        gu_id_list[idx].mme_group_id,
                        sizeof(son_u8) * RRM_MAX_MME_GRP_BYTES);
            }
        }
    }
    else if (SONX2_LINK_UP_IND == p_cspl_hdr->api)
    {
        son_x2_link_up_ind_t *p_link_up_ind = p_l3_ind;
        ind.bitmask |= RRM_SON_GU_GROUP_ID_TO_ADD_LIST_PRESENT;
        ind.add_gu_id.num_gu_group_id = 
            p_link_up_ind->gu_group_id.num_gu_group_id;
        for (idx = 0; idx < p_link_up_ind->gu_group_id.num_gu_group_id; 
                idx++)
        {
            x2_convert_l3_plmn_id_to_son_plmn_id(&p_link_up_ind->
                    gu_group_id.gu_id_list[idx].plmn_id,
                    (son_cell_plmn_info_t *)&ind.add_gu_id.
                    gu_id_list[idx].plmn_id);

            SON_MEMCPY(ind.add_gu_id.gu_id_list[idx].mme_group_id, 
                    p_link_up_ind->gu_group_id.gu_id_list[idx].
                    mme_group_id,
                    sizeof(son_u8) * RRM_MAX_MME_GRP_BYTES);
        }
    }

    if ((0 != ind.add_gu_id.num_gu_group_id) || 
            (0 != ind.delete_gu_id.num_gu_group_id))
    {
        /*Invoke PUP tool call to send message to RRM*/
        if (RRM_FAILURE == rrm_son_send_rrm_son_enb_config_update_ind(&ind,
                    SON_ANR_MODULE_ID,
                    SON_RRM_MODULE_ID,
                    SON_NULL,
                    SON_NULL))
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_WARNING,
                    "Failed to send "
                    "message %u from %u to %u",
                    RRM_SON_ENB_CONFIG_UPDATE_IND,
                    SON_ANR_MODULE_ID,
                    SON_RRM_MODULE_ID);
        }
    }

    SON_UT_TRACE_EXIT();
}

/* Spr 16142 Fix Start */
/*****************************************************************************
 * Function Name  : anr_construct_send_stop_tnl_discovery_ind_to_rrm
 * Inputs         : p_enb_id    - Pointer to the enbId
 * Outputs        : None
 * Returns        : None
 * Description    : This Function send stop tnl discovery ind to rrm
 *****************************************************************************/
son_void_t
anr_construct_send_stop_tnl_discovery_ind_to_rrm
(
 son_global_enb_id_t *p_enb_id
 )
{
    son_rrm_stop_tnl_discovery_ind_t stop_tnl_req;


    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_enb_id)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                " Message to RRM could not be sent as p_enb_id is SON_NULL");

        SON_UT_TRACE_EXIT();
        return;
    }

    SON_MEMSET(&stop_tnl_req, 0, sizeof(son_rrm_stop_tnl_discovery_ind_t));

    SON_MEMCPY(&stop_tnl_req.target_enb_id,
            p_enb_id,sizeof(son_global_enb_id_t));

    /*Invoke PUP tool call to send message to RRM*/
    if (SON_FAILURE != son_rrm_send_son_rrm_stop_tnl_discovery_ind(&stop_tnl_req,
                SON_ANR_MODULE_ID,
                SON_RRM_MODULE_ID,
                SON_NULL, SON_NULL))
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_INFO,
                "SON_RRM_STOP_TNL_DISCOVERY_IND send for eNB [ENB Id: 0x%x]]",
                son_convert_char_cell_id_to_int_cell_id(p_enb_id->enb_id));
    }
    SON_UT_TRACE_EXIT();

}
/* Spr 16142 Fix End */
/* Spr 17753 Changes Start */
/*****************************************************************************
* Function Name  : anr_insert_node_in_secode_degree_nbr_list
* Inputs         : second_degree_nbr_list  - NR List
*                  p_list_node     - Node to be inserted
* Outputs        : None
* Returns        : son_bool_et - SON_TRUE/SON_FALSE 
* Description    : This function inserts a node in second degree nbr list
*****************************************************************************/
son_void_t
anr_insert_node_in_secode_degree_nbr_list
(
 SON_LIST *p_second_degree_nbr_list,
 anr_second_degree_nr_list_node_t *p_list_node
 )
{
    SON_UT_TRACE_ENTER();

    if (SON_PNULL != p_list_node)
    {    
        list_insert_node(p_second_degree_nbr_list,
                &p_list_node->nr_nbr_anchor);

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_DETAILED,
                "anr_insert_node_in_secode_degree_nbr_list :"
                " Added node for first degree nbr cell id [0x%x] in second degree nbr List\n",
                son_convert_char_cell_id_to_int_cell_id(p_list_node->nr_nbr_data.first_degree_nbr_cgi.cell_identity));
    }

    SON_UT_TRACE_EXIT();
}/* anr_insert_node_in_secode_degree_nbr_list */

/*****************************************************************************
* Function Name  : anr_find_record_in_second_degree_nbr_list
* Inputs         : SON_LIST  - NR List
*                  p_cgi     - cgi of neighbor to find in list
* Outputs        : None
* Returns        : anr_second_degree_nr_list_node_t node of nbr 
* Description    : This function search a node in second degree nbr list
*****************************************************************************/
anr_second_degree_nr_list_node_t *
anr_find_record_in_second_degree_nbr_list
(
 SON_LIST                            *p_list,
 son_intra_rat_global_cell_id_t      *p_cgi 
 )
{
    SON_LIST_NODE                *p_cgi_node                 = SON_PNULL;
    anr_second_degree_nr_list_node_t  *p_second_degree_node  = SON_PNULL;


    SON_UT_TRACE_ENTER();

    /* CGI of first degree nbr search */
    p_cgi_node = list_find_node(p_list, (son_void_t *)(p_cgi),
            anr_key_of_second_degree_list,
            anr_compare_cgi_value_second_degree_list_node);

    if (p_cgi_node != SON_PNULL)
    {
        p_second_degree_node = YMEMBEROF(anr_second_degree_nr_list_node_t,
                nr_nbr_anchor, p_cgi_node);
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "anr_find_record_in_second_degree_nbr_list:"
                "cgi not found in the list\n");
    }

    SON_UT_TRACE_EXIT();
    return p_second_degree_node;
} /* anr_find_record_in_second_degree_nbr_list */

/*****************************************************************************
 * Function Name  : anr_key_of_second_degree_list
 * Inputs         : p_node -  Anchor to nbr 
 * Outputs        : None
 * Returns        : son_void_t * - void type pointer to node
 * Description    : This function generates the key
 ******************************************************************************/
const son_void_t *
anr_key_of_second_degree_list
(
 const SON_LIST_NODE *p_node
 )
{
    anr_second_degree_nr_list_node_t  *p_data  = SON_PNULL;

    SON_UT_TRACE_ENTER();
    SON_ASSERT(SON_PNULL != p_node);

    p_data = YMEMBEROF(anr_second_degree_nr_list_node_t,
            nr_nbr_anchor, p_node);

    return (son_void_t *)(&p_data->nr_nbr_data.first_degree_nbr_cgi);
}/* anr_key_of_second_degree_list */

/*****************************************************************************
 * Function Name  : anr_compare_cgi_value_second_degree_list_node
 * Inputs         : p_val_1  - Pointer to the key field(cgi)as void pointer
 *                : p_val_2  - Pointer to the key field(cgi)as void pointer
 * Outputs        : None
 * Returns        : son_s32  - 0 If matches
 *                             Non zero if not matches
 * Description    : This function compares the key field(cgi) of the
 *                  nbr node in the list
 ******************************************************************************/
son_s32
anr_compare_cgi_value_second_degree_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
    son_s32 ret_value = -1;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "anr_compare_cgi_value_second_degree_list_node: Pointers passed are  NULL");
        SON_UT_TRACE_EXIT();
        return ret_value;
    }
    else
    {
        ret_value = SON_MEMCMP(p_val_1, p_val_2, sizeof(son_intra_rat_global_cell_id_t));
    }

    SON_UT_TRACE_EXIT();
    return ret_value;
} /* anr_compare_cgi_value_second_degree_list_node */

/*****************************************************************************
* Function Name  : anr_insert_neigh_info_in_link_list
* Inputs         : p_srv_cell_info,  - X2 nbr info
*                  srv_cell_list     - num of served cell of first degree nbr
*                  p_cell_ctxt       - cell context of serving cell 
* Outputs        : None
* Returns        : son_bool_et - SON_TRUE/SON_FALSE 
* Description    : This function inserts a node in second degree nbr list
*****************************************************************************/
son_return_et
anr_insert_neigh_info_in_link_list
(
 x2ap_served_cell_info_arr_t  *p_srv_cell_info,
 anr_cell_context_t            *p_cell_ctxt,
 son_global_enb_id_t          *p_enb_id
 )
{
    son_u16 idx1                                      = 0;
    anr_second_degree_nr_list_node_t *p_list_node    = SON_PNULL;
    son_intra_rat_global_cell_id_t intra_rat_cell_id; 

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&intra_rat_cell_id,0,sizeof(son_intra_rat_global_cell_id_t));

    get_ecgi_from_enb_id_cell_id(
            &intra_rat_cell_id,
            p_srv_cell_info->cell_info.cell_id, p_enb_id);

    /*Finding the nbr CGI value in second degree nbr list*/
    p_list_node = anr_find_record_in_second_degree_nbr_list
        (&p_cell_ctxt->second_degree_nbr_list,
         &intra_rat_cell_id);

    if(p_list_node) /* Neighbour found in second degree nbr list */
    {
        /* Update the seond degree nbr list */ 
        if (list_count(&p_list_node->nr_nbr_data.anr_nr_nbr_list))
        {
            anr_clear_nr_nbrs_list(&p_list_node->nr_nbr_data.anr_nr_nbr_list);
            list_init(&p_list_node->nr_nbr_data.anr_nr_nbr_list);
        }
        /* Coverity_Fix 97184 ++ */
        for (idx1 = 0; (idx1 < (p_srv_cell_info->nbour_cell_info.num_neighbour_cell)) &&
                (idx1 < MAX_NEIGHBOURS) ; idx1++)
            /* Coverity_Fix 97184 -- */
        {    
            anr_insert_in_nr_nbr_list(&p_list_node->nr_nbr_data.anr_nr_nbr_list,
                    &p_srv_cell_info->nbour_cell_info.neighbour_info_arr[idx1],
                    p_cell_ctxt);
        }
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_DETAILED,
                " anr_insert_neigh_info_in_link_list: nbr cellid [0x%x]"
                " already present in the second degree list"
                " updating the second degree list only",
                son_convert_char_cell_id_to_int_cell_id(
                    p_list_node->nr_nbr_data.first_degree_nbr_cgi.cell_identity));

    }
    else if (SON_PNULL == p_list_node)
    {  
        /* Neighbor not present in the list */
        p_list_node = son_mem_get(sizeof(anr_second_degree_nr_list_node_t));
        if(SON_PNULL != p_list_node)
        {
            SON_MEMSET(p_list_node, 0, sizeof(anr_second_degree_nr_list_node_t));
            /* First degree nbr ecgi */

            SON_MEMCPY(&p_list_node->nr_nbr_data.first_degree_nbr_cgi ,&intra_rat_cell_id,  
                    sizeof(son_intra_rat_global_cell_id_t));

            /* Fill anr second degree neighbor list */
            /* Coverity_Fix 97184 ++ */
            for (idx1 = 0; (idx1 < (p_srv_cell_info->nbour_cell_info.num_neighbour_cell)) &&
                    (idx1 < MAX_NEIGHBOURS) ; idx1++)
                /* Coverity_Fix 97184 -- */
            {    
                anr_insert_in_nr_nbr_list(&p_list_node->nr_nbr_data.anr_nr_nbr_list,
                        &p_srv_cell_info->nbour_cell_info.neighbour_info_arr[idx1],
                        p_cell_ctxt);
            } 

            son_time(&p_list_node->ts);

            anr_insert_node_in_secode_degree_nbr_list
                (&p_cell_ctxt->second_degree_nbr_list,p_list_node);

        }
        else
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR,
                    " anr_insert_neigh_info_in_link_list: Memory allocation failure");
            SON_UT_TRACE_EXIT();
            return SON_FAILURE;
        }
    }
    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
}/* anr_insert_neigh_info_in_link_list */

/****************************************************************************************
 * Function Name  : anr_find_conflict_in_nrt_n_send_conflict_ind
 * Inputs         : p_cell_ctxt - CELL CONTEXT 
 * Outputs        : None
 * Returns        : son_void_t   
 * Description    : This function finds the conflict in NRT and either send conflict ind
 *                  or start the conflict back off ind timer
 * **************************************************************************************/
son_void_t
anr_find_conflict_in_nrt_n_send_conflict_ind
(
 anr_cell_context_t *p_cell_ctxt
 )
{
    son_anr_pci_conflict_confusion_ind_t *p_msg     = SON_PNULL;
    SON_HASH * p_hash                               = SON_PNULL;
    anr_intra_rat_hash_nr_tuple_t * p_tpl           = SON_PNULL;
    anr_intra_rat_hash_rec_t * p_hash_rec           = SON_PNULL;
    son_bool_et is_conflict_found                   = SON_FALSE;
    son_bool_et is_serving_cgi_greater_then_nbr_cgi = SON_FALSE;
    son_u32 timer_duration                          = 0;                                        
    SON_UT_TRACE_ENTER();


    p_msg = (son_anr_pci_conflict_confusion_ind_t *)
        son_mem_get(sizeof(son_anr_pci_conflict_confusion_ind_t));

    if(SON_PNULL == p_msg)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                " anr_find_conflict_in_nrt_n_send_conflict_ind: Memory allocation failure");

        SON_UT_TRACE_EXIT();
        return ;
    }

    /* finding conflict in nrt */
    p_hash = &p_cell_ctxt->intra_rat_neighbor_table;
    p_tpl = anr_get_first_nr_from_intra_rat_hash_table(p_hash);
    while (SON_PNULL != p_tpl)
    {
        p_hash_rec = &p_tpl->data;

        if(SON_TRUE == anr_is_serving_conflicts_with_inserted_neighbour(p_cell_ctxt,p_hash_rec,p_msg))
        {
            is_conflict_found = SON_TRUE;
            break;
        }
        p_tpl = anr_get_next_nr_from_intra_rat_hash_table(p_hash,
                &p_tpl->anchor);
    }
    if (SON_TRUE == is_conflict_found)
    {
        is_serving_cgi_greater_then_nbr_cgi = son_check_is_serving_cgi_greater_then_nbr_cgi 
                                          (&p_msg->cgi_1.intra_rat_global_cell_id,
                                           &p_msg->cgi_2.intra_rat_global_cell_id); 

        if((0 == p_cell_ctxt->conflict_detection_backoff_timer_val)||
                (SON_TRUE == is_serving_cgi_greater_then_nbr_cgi))
        {     
            if(SON_PCI_ENABLE ==  anr_get_pci_module_mode())
            {
                if(0 == p_cell_ctxt->conflict_detection_backoff_timer_val)
                {
                    /*second degrre conflict detection not enabled */
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_DETAILED,
                        "anr_find_conflict_in_nrt_n_send_conflict_ind: "
                            " Conflict back off timer value configured from OAM is = Zero" 
                            " Sending Conflict Ind to OAM \n");
                }
                else
                {
                    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_DETAILED,
                            "anr_find_conflict_in_nrt_n_send_conflict_ind: "
                            " Serving ECGI is having greater value from Neighbor ECGI"
                            " Not starting back off timer"
                            " Sending Conflict Ind to OAM \n");
                }
                son_create_send_buffer((son_void_t *)p_msg, SON_ANR_MODULE_ID, SON_APPS_MODULE_ID,
                        SONANR_PCI_CONFLICT_CONFUSION_IND, sizeof(son_anr_pci_conflict_confusion_ind_t));
            }
        }     
        else  
        {     
            /* Conflict back off timer needs to be started here */
            if(SON_PNULL == p_cell_ctxt->son_backoff_timer_id)
            {     
                /* randomized value to be used for timer duration */
                /* to randomize the backoff timer between different eNBs, a random value from 0-value configured
                 * is calculated to configured timer value */
                timer_duration = (son_anr_get_random_timer_val
                                         (p_cell_ctxt->conflict_detection_backoff_timer_val,
                                        /* SPR 23260 + */
#ifndef __x86_64__
                                        (U32)p_msg));
#else
                                        (U64)p_msg));
#endif
                                        /* SPR 23260 - */

                p_cell_ctxt->son_backoff_timer_id = son_start_timer(timer_duration,
                        SON_PNULL, 
                        0,
                        SON_FALSE);

                    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
                            "anr_find_conflict_in_nrt_n_send_conflict_ind: "
                            " Starting Conflict back off timer started with timer ID [ID:%p]",
                            p_cell_ctxt->son_backoff_timer_id);

            }
            else
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
                        "anr_find_conflict_in_nrt_n_send_conflict_ind: "
                        " Conflict back off timer is already running"
                        " Not starting the timer again");
            }
        }
    }
    else
    {
        if(p_cell_ctxt->son_backoff_timer_id)       
        {
            son_stop_timer(p_cell_ctxt->son_backoff_timer_id);

            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
                    "anr_find_conflict_in_nrt_n_send_conflict_ind: "
                    " Conflict back off timer Stopped as Conflict is resolved");
            p_cell_ctxt->son_backoff_timer_id = SON_PNULL;
        }
    }
    /* Free the allocated memory */
    if (SON_PNULL != p_msg)
    {
        son_mem_free(p_msg);
    }
    SON_UT_TRACE_EXIT();
}/* anr_find_conflict_in_nrt_n_send_conflict_indi */
/* Spr 17753 Changes End */
