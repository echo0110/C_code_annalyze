/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_enb_table.c$
 *
 *******************************************************************************
 *
 * File Description:
 *
 *******************************************************************************
 * Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *  Aug, 2012     Komal/Shekhar                  Initial
 *  May, 2014        Shilpi                      Coverity Fixes  
 *  July, 2014       Shilpi                      SPR 10331 Fix
 ******************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <son_enb_table.h>
#include <son_utils.h>
#include <son_anr_table.h>
#include <son_x2_ctxt_mgr.h>
#include <son_anr_enb_ctxt_mgr.h>
extern const son_8 *p_son_x2_facility_name;
extern x2_global_context_t g_x2_gb_ctxt;
extern const son_8 *p_son_anr_facility_name;

/******************************************************************************
 * Function Name  : x2_init_nbr_enb_hash_tuple_pool
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_FAILURE,SON_SUCCESS
 * Description    :  This function creates neighbor eNB hash tuple pool
 ******************************************************************************/
static son_return_et
x2_init_nbr_enb_hash_tuple_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	if (!cvContextPoolSetup(x2_get_nbr_enb_hash_rec_pool(),
				sizeof(son_nbr_enb_hash_tuple_t),
				X2_INIT_HASH_CTXT_POOL,
				X2_MAX_HASH_CTXT_POOL,
				X2_INIT_HASH_CTXT_POOL))
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_ERROR,
				"Neighbor eNB Hash Context Pool "
				"Setup could not be completed");
		return SON_FAILURE;
	}

	return SON_SUCCESS;
}

/******************************************************************************
 * Function Name  : x2_init_nbr_enb_ip_addr_hash_tuple_pool
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_FAILURE,SON_SUCCESS
 * Description    :  This function creates neighbor eNB hash tuple pool
 ******************************************************************************/
static son_return_et
x2_init_nbr_enb_ip_addr_hash_tuple_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	if (!cvContextPoolSetup(x2_get_nbr_enb_ip_addr_hash_rec_pool(),
				sizeof(son_nbr_enb_ip_addr_hash_tuple_t),
				X2_INIT_HASH_CTXT_POOL,
				X2_MAX_HASH_CTXT_POOL,
				X2_INIT_HASH_CTXT_POOL))
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_ERROR,
				" Neighbor eNB IP address  Hash Context Pool "
				"Setup could not be completed");
		return SON_FAILURE;
	}

	return SON_SUCCESS;
} /* x2_init_nbr_enb_ip_addr_hash_tuple_pool */

/* SPR Fix Start */

/******************************************************************************
 * Function Name  : x2_init_nbr_ecgi_enb_id_hash_tuple_pool
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_FAILURE,SON_SUCCESS
 * Description    : This function creates nbr_ecgi_enb_id_map tuple pool
 ******************************************************************************/
static son_return_et
x2_init_nbr_ecgi_enb_id_hash_tuple_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	if (!cvContextPoolSetup(x2_get_nbr_ecgi_enb_id_hash_rec_pool(),
				sizeof(son_nbr_ecgi_enb_id_hash_tuple_t),
				X2_INIT_HASH_CTXT_POOL,
				X2_MAX_HASH_CTXT_POOL,
				X2_INIT_HASH_CTXT_POOL))
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_ERROR,
				" Neighbor ECGI and eNB ID Hash Context Pool "
				"Setup could not be completed");
		return SON_FAILURE;
	}

	return SON_SUCCESS;
} /* x2_init_nbr_ecgi_enb_id_hash_tuple_pool */

/* SPR Fix End */

/*******************************************************************************
 * Function Name  : x2_destroy_nbr_enb_hash_tuple_pool
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function deletes the hash tuple pool for neighbor eNBs
 ******************************************************************************/
static son_void_t
x2_destroy_nbr_enb_hash_tuple_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	qvPoolDelete((x2_get_nbr_enb_hash_rec_pool())->pool);
}

/*******************************************************************************
 * Function Name  : x2_destroy_nbr_enb_ip_addr_hash_tuple_pool
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function deletes the hash tuple pool for neighbor eNBs
 *                  IP addresses
 ******************************************************************************/
static son_void_t
x2_destroy_nbr_enb_ip_addr_hash_tuple_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	qvPoolDelete((x2_get_nbr_enb_ip_addr_hash_rec_pool())->pool);
}

/* SPR Fix Start */

/*******************************************************************************
 * Function Name  : x2_destroy_nbr_ecgi_enb_id_hash_tuple_pool
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function deletes the hash tuple pool for neighbor ECGI
 *                  and eNB Id map
 ******************************************************************************/
static son_void_t
x2_destroy_nbr_ecgi_enb_id_hash_tuple_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	qvPoolDelete((x2_get_nbr_ecgi_enb_id_hash_rec_pool())->pool);
}

/* SPR Fix End */

/******************************************************************************
 * Function Name  : x2_destroy_nbr_enb_hash_node_pool
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function destroys neighbor eNB context table pool
 *****************************************************************************/
static son_void_t
x2_destroy_nbr_enb_hash_node_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	cvContextPoolDelete(x2_get_hash_node_pool());
}

/*******************************************************************************
 * Function Name  : x2_init_nbr_enb_hash_node_pool
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_SUCCESS,SON_FAILURE
 * Description    : This function created neighbor eNB hash table context pool
 ******************************************************************************/
static son_return_et
x2_init_nbr_enb_hash_node_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	if (!cvContextPoolSetup(x2_get_enb_hash_node_pool(),
				sizeof(SON_HASH_NODE *) *
				X2_MAX_INITIAL_NR_TBL_BKT,
				X2_INIT_HASH_CTXT_POOL,
				X2_MAX_HASH_CTXT_POOL,
				X2_INIT_HASH_CTXT_POOL))
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_ERROR,
				" Neighbor eNB Hash Table Pool Setup could not be "
				"completed");
		return SON_FAILURE;
	}

	return SON_SUCCESS;
} /* x2_init_nbr_enb_hash_node_pool */

/******************************************************************************
 * Function Name  : x2_init_nbr_enb_table
 * Inputs         : H  Pointer to SON_HASH
 * Outputs        : None
 * Returns        : son_return_et
 * Description    : This function creates neighbor eNB hash table context
 *                  present in global context
 ******************************************************************************/
son_return_et
x2_init_nbr_enb_table
(
 SON_HASH *H
 )
{
	SON_HASH_NODE * *table;

	ANR_GET_TABLE(x2_get_hash_node_pool(), table);
	hash_init(H, X2_MAX_INITIAL_NR_TBL_BKT, x2_nbr_enb_hash_func,
			x2_nbr_enb_cmp_func,
			x2_nbr_enb_key_off_nr_func, table);
	return SON_SUCCESS;
}

/******************************************************************************
 * Function Name  : x2_init_nbr_enb_ip_addr_map
 * Inputs         : H  Pointer to SON_HASH
 * Outputs        : None
 * Returns        : son_return_et
 * Description    : This function creates neighbor eNB IP address map present in
 *                   global context
 ******************************************************************************/
son_return_et
x2_init_nbr_enb_ip_addr_map
(
 SON_HASH *H
 )
{
	SON_HASH_NODE * *table;

	ANR_GET_TABLE(x2_get_hash_node_pool(), table);
	hash_init(H, X2_MAX_INITIAL_NR_TBL_BKT, x2_nbr_enb_ip_addr_hash_func,
			x2_nbr_enb_ip_addr_cmp_func,
			x2_nbr_enb_ip_addr_key_off_nr_func, table);
	return SON_SUCCESS;
}

/* SPR Fix Start */

/******************************************************************************
 * Function Name  : x2_init_nbr_ecgi_enb_id_map
 * Inputs         : p_hash  Pointer to SON_HASH
 * Outputs        : None
 * Returns        : SON_SUCCESS/SON_FAILURE
 * Description    : This function creates the map storing the mapping of
 *                  neighbor ECGI and its ENB Id
 ******************************************************************************/
son_return_et
x2_init_nbr_ecgi_enb_id_map
(
 SON_HASH *p_hash
 )
{
	SON_HASH_NODE * *table;

	ANR_GET_TABLE(x2_get_hash_node_pool(), table);
	hash_init(p_hash, X2_MAX_INITIAL_NR_TBL_BKT, x2_nbr_ecgi_enb_id_hash_func,
			son_compare_cell_id,
			x2_nbr_ecgi_enb_id_key_off_nr_func, table);
	return SON_SUCCESS;
}

/* SPR Fix End */

/******************************************************************************
 * Function Name  : x2_get_hash_of_nbr_enb_id
 * Inputs         : p_key - Pointer to the neighbor eNB Id
 * Outputs        : None
 * Returns        : The hash value for the given neighbor eNB Id
 * Description    : This is a function for generating the hash value for
 *                  the given neighbor  eNB Id.
 ****************************************************************************/
static son_ul32
x2_get_hash_of_nbr_enb_id
(
 son_void_t *p_key
 )
{
	son_ul32 key = 0;
	son_u8 key_len = 0;
	son_u8 temp[SON_MCC_OCTET_SIZE +
		SON_ENB_ID_OCTET_SIZE +
		SON_MAX_MNC_OCTET_SIZE] = {0};

	son_oam_log_on_off_et log_mode = X2_GET_LOG_MODE;
	son_global_enb_id_t  *p_nbr_enb_id  = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_key)
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_ERROR,
				"Could not found hash value, "
				"as given Neighbor eNB id  is NULL");

		SON_UT_TRACE_EXIT();
		return key;
	}

	p_nbr_enb_id = (son_global_enb_id_t *)p_key;

	/* Convert MCC + MNC + Cell ID */
	SON_MEMCPY(&temp[0], p_nbr_enb_id->plmn_id.mcc, SON_MCC_OCTET_SIZE);
	key_len = SON_MCC_OCTET_SIZE;

	SON_MEMCPY(&temp[key_len], p_nbr_enb_id->plmn_id.mnc,
			p_nbr_enb_id->plmn_id.num_mnc_digit);

	key_len += p_nbr_enb_id->plmn_id.num_mnc_digit;

	SON_MEMCPY(&temp[key_len], p_nbr_enb_id->enb_id, SON_ENB_ID_OCTET_SIZE);

	key_len += SON_ENB_ID_OCTET_SIZE;

	key = hasher(&temp[0], &temp[key_len - 1]);

	SON_UT_TRACE_EXIT();
	return key;
} /* x2_get_hash_of_nbr_enb_id */

/******************************************************************************
 * Function Name  : x2_get_hash_of_nbr_enb_ip_addr
 * Inputs         : p_key - Pointer to the neighbor eNB Id
 * Outputs        : None
 * Returns        : The hash value for the given neighbor eNB Id
 * Description    : This is a function for generating the hash value for
 *                  the given neighbor  eNB Id IP Address hash.
 ****************************************************************************/
static son_ul32
x2_get_hash_of_nbr_enb_ip_addr
(
 son_void_t *p_key
 )
{
	son_ul32 key = 0;
	son_u8 key_len = 0;
	son_u8 temp[SON_IPV6_SIZE] = {0};

	son_oam_log_on_off_et log_mode        = X2_GET_LOG_MODE;
	son_enb_tnl_address_t  *p_enb_tnl_add  = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_key)
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_ERROR,
				"Could not found hash value, "
				"as given Neighbor eNB id IP addr  is NULL");

		SON_UT_TRACE_EXIT();
		return key;
	}

	p_enb_tnl_add  = (son_enb_tnl_address_t *)p_key;

	if (IPV4_ADD_PRESENT & p_enb_tnl_add->bitmask)
	{
		SON_MEMCPY(temp, p_enb_tnl_add->ipv4_add, SON_IPV4_SIZE);
		key_len = SON_IPV4_SIZE;
	}
	else
	{
		SON_MEMCPY(temp, p_enb_tnl_add->ipv6_add, SON_IPV6_SIZE);
		key_len = SON_IPV6_SIZE;
	}

	key = hasher(&temp[0], &temp[key_len - 1]);

	SON_UT_TRACE_EXIT();
	return key;
} /* x2_get_hash_of_nbr_enb_ip_addr */

/******************************************************************************
 * Function Name  : x2_nbr_enb_hash_func
 * Inputs         : p_key - Pointer to the structure of type
 *                          son_nbr_enb_hash_rec_t
 * Outputs        : None
 * Returns        : The hash value for given neighbor eNB hash record
 * Description    : This is function for generating hash key for the
 *                  given neighbor eNB hash record
 ****************************************************************************/
son_ul32
x2_nbr_enb_hash_func
(
 const son_void_t *p_key
 )
{
	son_ul32 key = 0;
	son_global_enb_id_t  *p_nbr_enb_id = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_key)
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_BRIEF,
				"Hash Key Generation for "
				"Neighbor eNB Hash Table failed, p_key is NULL");
	}
	else
	{
		/* Get the nbr eNB Id */
		p_nbr_enb_id = &(((son_nbr_enb_hash_rec_t *)p_key)->enb_id);
		/* Get the hash value */
		key = x2_get_hash_of_nbr_enb_id((son_void_t *)p_nbr_enb_id);
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_DETAILED,
				"Hash Key generated for "
				"nbr eNB  Hash Table is [%u]", key);
	}

	SON_UT_TRACE_EXIT();
	return key;
} /* x2_nbr_enb_hash_func */

/******************************************************************************
 * Function Name  : x2_nbr_enb_ip_addr_hash_func
 * Inputs         : p_key - Pointer to the structure of type
 *                          son_nbr_enb_ip_addr_hash_rec_t
 * Outputs        : None
 * Returns        : The hash value for given neighbor eNB hash record
 * Description    : This is function for generating hash key for the
 *                  given neighbor eNB IP address  hash record
 ****************************************************************************/
son_ul32
x2_nbr_enb_ip_addr_hash_func
(
 const son_void_t *p_key
 )
{
	son_ul32 key = 0;
	son_enb_tnl_address_t  *p_nbr_enb_ip_addr = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_key)
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_WARNING,
				"Hash Key Generation for "
				"Neighbor eNB IP address Map failed, p_key is NULL");
	}
	else
	{
		/* Get the nbr eNB IP address */
		p_nbr_enb_ip_addr = &(((son_nbr_enb_ip_addr_hash_rec_t *)p_key)->enb_tnl_address);
		/* Get the hash value */
		key = x2_get_hash_of_nbr_enb_ip_addr((son_void_t *)p_nbr_enb_ip_addr);

		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_DETAILED,
				"Hash Key generated for "
				"nbr eNB IP address Hash Table is [%u]", key);
	}

	SON_UT_TRACE_EXIT();
	return key;
} /* x2_nbr_enb_ip_addr_hash_func */

/* SPR Fix Start */

/******************************************************************************
 * Function Name  : x2_nbr_ecgi_enb_id_hash_func
 * Inputs         : p_key - Pointer to the structure of type
 *                          son_nbr_ecgi_enb_id_hash_rec_t
 * Outputs        : None
 * Returns        : The hash value for given hash record
 * Description    : This is function for generating hash key for the
 *                  given neighbor ECGI and eNB ID map hash record
 ****************************************************************************/
son_ul32
x2_nbr_ecgi_enb_id_hash_func
(
 const son_void_t *p_key
 )
{
	son_ul32 key = 0;
	son_intra_rat_global_cell_id_t  *p_nr_cell_id;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_key)
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_WARNING,
				"Hash Key Generation for "
				"nbr_ecgi_enb_id_map failed, p_key is NULL");
	}
	else
	{
		/* Get the cell Id */
		p_nr_cell_id = &(((son_nbr_ecgi_enb_id_hash_rec_t *)p_key)->
				nbr_cell_id);
		/* Get the hash value */
		key = anr_get_hash_of_intra_rat_cell_id((son_void_t *)p_nr_cell_id);

		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_DETAILED,
				"Hash Key generated for "
				"NR Hash Table is [%u]", key);
	}

	SON_UT_TRACE_EXIT();
	return key;
} /* x2_nbr_ecgi_enb_id_hash_func */

/* SPR Fix End */

/*******************************************************************************
 * Function Name  : x2_nbr_enb_cmp_func
 * Inputs         : p_val_1 - Pointer to the structure of type
 *                          son_nbr_enb_hash_rec_t
 *                : p_val_2 - Pointer to the structure of type
 *                             son_nbr_enb_hash_rec_t
 * Outputs        : None
 * Returns        : SON_TRUE : In case comparision is successful
 *                : SON_FALSE : In case comparision failed
 * Description    : This is a hash function used for comparing the keys
 ******************************************************************************/
son_s32
x2_nbr_enb_cmp_func
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	const son_nbr_enb_hash_rec_t    *p_rec_1    = SON_PNULL;
	const son_nbr_enb_hash_rec_t    *p_rec_2    = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_rec_1 = (son_nbr_enb_hash_rec_t *)p_val_1;
	p_rec_2 = (son_nbr_enb_hash_rec_t *)p_val_2;

	SON_UT_TRACE_EXIT();
	return SON_MEMCMP(&p_rec_1->enb_id, &p_rec_2->enb_id,
			sizeof(son_global_enb_id_t));
}

/*******************************************************************************
 * Function Name  : x2_nbr_enb_ip_addr_cmp_func
 * Inputs         : p_val_1 - Pointer to the structure of type
 *                          son_nbr_enb_ip_addr_hash_rec_t
 *                : p_val_2 - Pointer to the structure of type
 *                             son_nbr_enb_ip_addr_hash_rec_t
 * Outputs        : None
 * Returns        : SON_TRUE : In case comparision is successful
 *                : SON_FALSE : In case comparision failed
 * Description    : This is a hash function used for comparing the keys
 ******************************************************************************/
son_s32
x2_nbr_enb_ip_addr_cmp_func
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	const son_nbr_enb_ip_addr_hash_rec_t    *p_rec_1    = SON_PNULL;
	const son_nbr_enb_ip_addr_hash_rec_t    *p_rec_2    = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_rec_1 = (son_nbr_enb_ip_addr_hash_rec_t *)p_val_1;
	p_rec_2 = (son_nbr_enb_ip_addr_hash_rec_t *)p_val_2;

	SON_UT_TRACE_EXIT();
	return SON_MEMCMP(&p_rec_1->enb_tnl_address, &p_rec_2->enb_tnl_address,
			sizeof(son_enb_tnl_address_t));
}

/******************************************************************************
 * Function Name  : x2_nbr_enb_key_off_nr_func
 * Inputs         : p_node -  Pointer to the anchor field of nbr eNB hash tuple
 *
 * Outputs        : None
 * Returns        : son_void_t*
 * Description    : This is a function used for find the data field
 *                   of the tuple corresponding to the given p_node
 ******************************************************************************/
const son_void_t *
x2_nbr_enb_key_off_nr_func
(
 const SON_HASH_NODE *p_node
 )
{
	const son_void_t                *p_data     = SON_PNULL;
	const son_nbr_enb_hash_tuple_t  *p_tpl      = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_node)
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_WARNING,
				"Failed to find the data for"
				" the given node, p_node is NULL");
	}
	else
	{
		p_tpl = (son_nbr_enb_hash_tuple_t *)
			YMEMBEROF(son_nbr_enb_hash_tuple_t, anchor, p_node);
		p_data = (son_void_t *)&p_tpl->data;
	}

	SON_UT_TRACE_EXIT();
	return p_data;
} /* x2_nbr_enb_key_off_nr_func */

/******************************************************************************
 * Function Name  : x2_nbr_enb_ip_addr_key_off_nr_func
 * Inputs         : p_node -  Pointer to the anchor field of nbr eNB IP address
 *                             hash tuple
 * Outputs        : None
 * Returns        : son_void_t*
 * Description    : This is a function used for find the data field
 *                   of the tuple corresponding to the given p_node
 ******************************************************************************/
const son_void_t *
x2_nbr_enb_ip_addr_key_off_nr_func
(
 const SON_HASH_NODE *p_node
 )
{
	const son_void_t                        *p_data     = SON_PNULL;
	const son_nbr_enb_ip_addr_hash_tuple_t  *p_tpl      = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_node)
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_WARNING,
				"Failed to find the data for"
				" the given node, p_node is NULL");
	}
	else
	{
		p_tpl = (son_nbr_enb_ip_addr_hash_tuple_t *)
			YMEMBEROF(son_nbr_enb_ip_addr_hash_tuple_t, anchor, p_node);
		p_data = (son_void_t *)&p_tpl->data;
	}

	SON_UT_TRACE_EXIT();
	return p_data;
} /* x2_nbr_enb_ip_addr_key_off_nr_func */

/* SPR Fix Start */

/******************************************************************************
 * Function Name  : x2_nbr_ecgi_enb_id_key_off_nr_func
 * Inputs         : p_node -  Pointer to the anchor field of
 *                            nbr_ecgi_enb_id_map hash tuple
 * Outputs        : None
 * Returns        : Pointer to the nbr_ecgi_enb_id_map data
 * Description    : This is a function used to find the data field
 *                   of the tuple corresponding for the given hash node
 ******************************************************************************/
const son_void_t *
x2_nbr_ecgi_enb_id_key_off_nr_func
(
 const SON_HASH_NODE *p_node
 )
{
	const son_void_t                        *p_data     = SON_PNULL;
	const son_nbr_ecgi_enb_id_hash_tuple_t  *p_tpl      = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_node)
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_WARNING,
				"Failed to find the data "
				"for the given node, p_node is NULL");
	}
	else
	{
		p_tpl = (son_nbr_ecgi_enb_id_hash_tuple_t *)
			YMEMBEROF(son_nbr_ecgi_enb_id_hash_tuple_t, anchor, p_node);
		p_data = (son_void_t *)&p_tpl->data;
	}

	SON_UT_TRACE_EXIT();
	return p_data;
} /* x2_nbr_ecgi_enb_id_key_off_nr_func */

/* SPR Fix End */

/******************************************************************************
 * Function Name  : x2_convert_enb_info_to_enb_hash_rec
 * Inputs         : p_nr   -  Pointer to a structure containing the NR
 *                            information
 *
 * Outputs        : p_hash_rec - Pointer to the data field of
 *                  the new NR record to be inserted.
 *
 * Returns        : None.
 * Description    : This function is used to populate node data of eNB NR
 *                  hash table using the given neighbor info.
 ******************************************************************************/
static son_void_t
x2_convert_enb_info_to_enb_hash_rec
(
 const son_neighbor_enb_info_t *p_nr,
 son_nbr_enb_hash_rec_t *p_hash_rec
 )
{
	son_oam_log_on_off_et log_mode     = X2_GET_LOG_MODE;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_nr || SON_PNULL == p_hash_rec)
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_ERROR,
				"Could not "
				"populate the eNB Hash record, p_nr or p_hash_rec is NULL");
		SON_UT_TRACE_EXIT();
		return;
	}

	SON_MEMSET(p_hash_rec, 0, sizeof(son_nbr_enb_hash_rec_t));

	p_hash_rec->enb_curr_fsm_state = X2_ENB_STATE_DISCONNECTED;
	p_hash_rec->enb_prev_fsm_state = X2_ENB_STATE_DISCONNECTED;
	p_hash_rec->is_self_del_enb_req_triggered = SON_FALSE;

	SON_MEMCPY(&p_hash_rec->enb_id, &p_nr->enb_id, sizeof(son_global_enb_id_t));

	if (p_nr->bitmask & SON_ANR_ENB_TNL_ADD_LIST_PRESENT)
	{
		p_hash_rec->enb_tnl_address_list_size = p_nr->enb_tnl_address_list_size;

		SON_MEMCPY(p_hash_rec->enb_tnl_address_list,
				p_nr->enb_tnl_address_list,
				p_nr->enb_tnl_address_list_size *
				sizeof(son_enb_tnl_address_t));
	}

	if (p_nr->bitmask & SON_ANR_ENB_X2_HO_STATUS_PRESENT)
	{
		p_hash_rec->x2_status = p_nr->x2_status;
	}

	if (p_nr->bitmask & SON_ANR_ENB_TAC_PRESENT)
	{
		SON_MEMCPY(p_hash_rec->tac_list, p_nr->tac_list, sizeof(p_hash_rec->tac_list));
		p_hash_rec->tac_list_size = p_nr->tac_list_size;
	}
	/* SPR 11340 Fix Starts */
	if ((p_nr->bitmask & SON_ANR_ENB_CUR_X2_CONN_PRESENT) &&
			((p_nr->x2_connection_status == SON_X2_DISCONNECT_BLACKLIST) ||
			 (SON_X2_CONNECT_WHITELIST == p_nr->x2_connection_status)))
		/* SPR 11340 Fix Ends */
	{
		p_hash_rec->nr_status = SON_NR_LOCKED;
	}
	else
	{
		p_hash_rec->nr_status = SON_NR_UNLOCKED;
	}

	SON_UT_TRACE_EXIT();
} /* x2_convert_enb_info_to_enb_hash_rec */

/******************************************************************************
 * Function Name  : x2_insert_entry_in_nbr_enb_ip_addr_map
 * Inputs         : p_hash              - Pointer to son hash record
 *                  p_ip_addr_hash_rec  - Pointer to ip addr hash record
 *                  p_error_code        - Error code
 * Outputs        : None
 * Returns        : son_nbr_enb_ip_addr_hash_rec_t
 * Description    : This function initialized the data to insert in eNB IP Address
 *                  context
 ******************************************************************************/
son_nbr_enb_ip_addr_hash_rec_t *
x2_insert_entry_in_nbr_enb_ip_addr_map
(
 SON_HASH *p_hash,
 son_nbr_enb_ip_addr_hash_rec_t *p_ip_addr_hash_rec,
 son_error_et *p_error_code
 )
{
	son_nbr_enb_ip_addr_hash_tuple_t *p_hash_tpl =  SON_PNULL;
	son_oam_log_on_off_et log_mode       = X2_GET_LOG_MODE;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_hash || SON_PNULL == p_ip_addr_hash_rec || SON_PNULL == p_error_code ||
			0 == p_ip_addr_hash_rec->enb_tnl_address.bitmask)
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_WARNING,
				"Could not "
				"initialize as  either IP address in not available or"
				"the p_hash or p_ip_addr_hash_rec or p_error_code is NULL");

		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	if (SON_PNULL != hash_find(p_hash, (son_void_t *)p_ip_addr_hash_rec))
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_WARNING, 
				" IP Already Present. Duplicate Entry");

		*p_error_code = SON_ERR_ANR_DUPLICATE_NR;
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	ANR_GET_HASH_REC(x2_get_enb_ip_addr_hash_rec_pool(), p_hash_tpl);

	if (SON_PNULL == p_hash_tpl)
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_WARNING,
				"Neighbor eNB"
				" could not be added, memory allocation failed");

		SON_UT_TRACE_EXIT();
		*p_error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;
		return SON_PNULL;
	}

	SON_MEMCPY(&p_hash_tpl->data, p_ip_addr_hash_rec,
			sizeof(son_nbr_enb_ip_addr_hash_rec_t));

	hash_insert(p_hash, &p_hash_tpl->anchor);
	SON_LOG(log_mode, p_son_x2_facility_name,
			SON_BRIEF,
			"Received IP is added to the IP address");

	SON_UT_TRACE_EXIT();
	*p_error_code = SON_NO_ERROR;
	return &p_hash_tpl->data;
} /* x2_insert_entry_in_nbr_enb_ip_addr_map */

/******************************************************************************
 * Function Name  : x2_delete_entry_from_nbr_enb_ip_addr_map
 * Inputs         : p_hash           - Pointer to the neighbor eNB hash context
 *                : p_nbr_ip_addr_id - Pointer containing Transport Layer Address
 * Outputs        : p_error_code   - Pointer to a variable for storing error code
 *
 * Returns        : SON_SUCCESS: The message is been handled successfully
 *                  SON_FAILURE: The message is not been handled successfully
 * Description    : This function is used for removing selected neighbor eNB
 *                  records from the nbr eNB IP address context table
 ******************************************************************************/
son_return_et
x2_delete_entry_from_nbr_enb_ip_addr_map
(
 SON_HASH *p_hash,
 son_enb_tnl_address_t  *p_nbr_ip_addr_id,
 son_error_et *p_error_code
 )
{
	son_nbr_enb_ip_addr_hash_rec_t hash_data;
	son_nbr_enb_ip_addr_hash_tuple_t      *p_tpl         = SON_PNULL;
	SON_HASH_NODE                        *p_hash_node    = SON_PNULL;
	son_oam_log_on_off_et log_mode        = X2_GET_LOG_MODE;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_hash || SON_PNULL == p_nbr_ip_addr_id || SON_PNULL == p_error_code ||
			0 == p_nbr_ip_addr_id->bitmask)
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_WARNING,
				" Neighbor eNB could not be deleted, "
				"p_hash or p_nbr_ip_addr_id or p_error_code  is NULL"
				"or IP address is not available");
		SON_UT_TRACE_EXIT();
		return SON_FAILURE;
	}

	SON_MEMSET(&hash_data, 0, sizeof(son_nbr_enb_ip_addr_hash_rec_t));
	SON_MEMCPY(&hash_data.enb_tnl_address, p_nbr_ip_addr_id, sizeof(son_enb_tnl_address_t));

	if (SON_PNULL != (p_hash_node = hash_find(p_hash, (son_void_t *)&hash_data)))
	{
		p_tpl = (son_nbr_enb_ip_addr_hash_tuple_t *)
			YMEMBEROF(son_nbr_enb_ip_addr_hash_tuple_t, anchor, p_hash_node);

		ANR_FREE_HASH_REC(x2_get_enb_ip_addr_hash_rec_pool(), p_hash, p_tpl,
				p_hash_node);

		*p_error_code = SON_NO_ERROR;

		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_INFO,
				"Neighbor eNB IP Address"
				" is successfully deleted from the table");

		SON_UT_TRACE_EXIT();
		return SON_SUCCESS;
	}

	SON_LOG(log_mode, p_son_x2_facility_name,
			SON_WARNING,
			"IP address"
			" not found in the table");

	SON_UT_TRACE_EXIT();
	*p_error_code = SON_ERR_ANR_NR_NOT_FOUND;
	return SON_FAILURE;
} /* x2_delete_entry_from_nbr_enb_ip_addr_map */

/******************************************************************************
 * Function Name  : x2_get_enb_id_from_nbr_enb_ip_addr_map
 * Inputs         : p_hash           - Pointer to the neighbor eNB IP address hash
 *                                      context
 *                : p_nbr_ip_addr_id  - Pointer to the neighbor enb IP address
 * Outputs        : p_error_code   - Pointer to a variable for storing error code
 *
 * Returns        : Pointer to the global enb id.
 * Description    : This function is used for getting neighbor eNB Ids for given
 *                   IP address
 ******************************************************************************/
son_global_enb_id_t *
x2_get_enb_id_from_nbr_enb_ip_addr_map
(
 SON_HASH *p_hash,
 son_enb_tnl_address_t  *p_nbr_ip_addr_id,
 son_error_et *p_error_code
 )
{
	son_nbr_enb_ip_addr_hash_rec_t hash_data;
	son_nbr_enb_ip_addr_hash_tuple_t      *p_tpl         = SON_PNULL;
	SON_HASH_NODE                       *p_hash_node    = SON_PNULL;
	son_oam_log_on_off_et log_mode        = X2_GET_LOG_MODE;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_hash || SON_PNULL == p_nbr_ip_addr_id || SON_PNULL == p_error_code ||
			0 == p_nbr_ip_addr_id->bitmask)
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_ERROR, 
				" Neighbor eNB IP address could not be find"
				"p_hash or p_nbr_ip_addr_id or p_error_code  is NULL"
				"or ip address is absent");
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	SON_MEMSET(&hash_data, 0, sizeof(son_nbr_enb_ip_addr_hash_rec_t));
	SON_MEMCPY(&hash_data.enb_tnl_address, p_nbr_ip_addr_id, sizeof(son_enb_tnl_address_t));

	if (SON_PNULL != (p_hash_node = hash_find(p_hash, (son_void_t *)&hash_data)))
	{
		p_tpl = (son_nbr_enb_ip_addr_hash_tuple_t *)
			YMEMBEROF(son_nbr_enb_ip_addr_hash_tuple_t, anchor, p_hash_node);

		*p_error_code = SON_NO_ERROR;

		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_BRIEF,
				"Neighbor eNB Id"
				" is found from the table");

		SON_UT_TRACE_EXIT();
		return &p_tpl->data.enb_id;
	}

	SON_LOG(log_mode, p_son_x2_facility_name,
			SON_WARNING,
			"IP address"
			" not found in the table");

	SON_UT_TRACE_EXIT();
	*p_error_code = SON_ERR_ANR_NR_NOT_FOUND;
	return SON_PNULL;
} /* x2_get_enb_id_from_nbr_enb_ip_addr_map */

/******************************************************************************
 * Function Name  : is_tac_list_valid
 *                : p_enb_info       - Pointer to enb data
 * Description    : This function is used to determine if the TAC list is
 *                 : valid
 ******************************************************************************/
static son_bool_et
is_tac_list_valid
(
 son_neighbor_enb_info_t *p_enb_info
 )
{
	son_u8 idx = 0;
	son_bool_et ret  = SON_TRUE;
	son_u16 *p_tac = (son_u16 *)p_enb_info->tac_list[idx].tac;

	for (idx = 0; idx < p_enb_info->tac_list_size; idx++)
	{
		if (0 == *p_tac)
		{
			SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
					SON_WARNING, "Invalid TAC 0x%x", SON_NTOHS(*p_tac));
			ret = SON_FALSE;
			break;
		}
	}

	return ret;
}

/******************************************************************************
 * Function Name  : x2_insert_nbr_enb
 * Inputs         : p_hash           -  Pointer to the nbr eNB hash context.
 *                : p_enb_info       - Pointer to enb data
 * Outputs        : p_error_code     -  Pointer to a variable for storing error code
 * Returns        : son_nbr_enb_hash_rec_t* - Pointer to the data field of the
 *                                              new neighbor eNB tuple inserted.
 * Description    : This function is used to insert an entry in the
 *                   neighbor eNB context Table to store neighbor eNB information
 ******************************************************************************/
son_nbr_enb_hash_rec_t *
x2_insert_nbr_enb
(
 SON_HASH *p_hash,
 son_neighbor_enb_info_t *p_enb_info,
 son_error_et *p_error_code
 )
{
	son_nbr_enb_hash_tuple_t  *p_hash_tpl  = SON_PNULL;
	son_nbr_enb_hash_rec_t hash_rec;
	son_oam_log_on_off_et log_mode        = X2_GET_LOG_MODE;
	son_nbr_enb_ip_addr_hash_rec_t ip_addr_hash_rec;
	son_u32 idx           = 0;
	son_u32 idx1           = 0;
	son_error_et error_code   = SON_NO_ERROR;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_enb_info || SON_PNULL == p_hash || SON_PNULL == p_error_code)
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_WARNING,
				"Neighbor eNB could not be added,"
				"p_hash or p_nr is NULL");

		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	SON_MEMSET(&hash_rec, 0, sizeof(son_nbr_enb_hash_rec_t));
	SON_MEMSET(&ip_addr_hash_rec, 0, sizeof(son_nbr_enb_ip_addr_hash_rec_t));

	x2_convert_enb_info_to_enb_hash_rec(p_enb_info, &hash_rec);

	if (SON_PNULL != hash_find(p_hash, (son_void_t *)&hash_rec))
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_WARNING,
				"Neighbor eNB 0x%x Already Present."
				" Duplicate Entry",
				son_convert_char_cell_id_to_int_cell_id(hash_rec.enb_id.enb_id));

		*p_error_code = SON_ERR_ANR_DUPLICATE_NR;
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	if (!((p_enb_info->bitmask & SON_ANR_ENB_CUR_X2_CONN_PRESENT) &&
				(p_enb_info->bitmask & SON_ANR_ENB_X2_HO_STATUS_PRESENT) &&
				((p_enb_info->bitmask & SON_ANR_ENB_TAC_PRESENT && p_enb_info->tac_list_size) ||
				 (p_enb_info->bitmask & SON_ANR_ENB_TNL_ADD_LIST_PRESENT))))
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_WARNING, 
				" Neighbor eNB 0x%x could not be added."
				" Mandatory parameters TNL address/TAC, x2_connection_status and X2 Status are not present",
				son_convert_char_cell_id_to_int_cell_id(p_enb_info->enb_id.enb_id));
		*p_error_code = SON_ERR_INVALID_PARAMS;
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	if ((p_enb_info->x2_connection_status !=  SON_X2_CONNECT) &&
			(p_enb_info->x2_connection_status !=  SON_X2_CONNECT_WHITELIST) &&
			(p_enb_info->x2_connection_status !=  SON_X2_DISCONNECT) &&
			(p_enb_info->x2_connection_status != SON_X2_DISCONNECT_BLACKLIST))
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_WARNING, 
				" Neighbor eNB 0x%x could not be added."
				" Incorrect value of x2_connection_status",
				son_convert_char_cell_id_to_int_cell_id(p_enb_info->enb_id.enb_id));
		*p_error_code = SON_ERR_INVALID_PARAMS;
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	if (((p_enb_info->bitmask & SON_ANR_ENB_TAC_PRESENT) && p_enb_info->tac_list_size) &&
			SON_FALSE == is_tac_list_valid(p_enb_info))
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_WARNING, 
				" Neighbor eNB 0x%x could not be added."
				" Incorrect value of TAC",
				son_convert_char_cell_id_to_int_cell_id(p_enb_info->enb_id.enb_id));
		*p_error_code = SON_ERR_INVALID_PARAMS;
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	if (!(p_enb_info->bitmask & SON_ANR_ENB_TNL_ADD_LIST_PRESENT))
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_WARNING,
				" Neighbor eNB 0x%x could not be added."
				" as TNL Discovery is required",
				son_convert_char_cell_id_to_int_cell_id(p_enb_info->enb_id.enb_id));
		*p_error_code = SON_ERR_TNL_DISCOVERY_FAILURE;
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	/*Check if any of the IPs is availble in the IP Address table*/
	ip_addr_hash_rec.enb_id =   p_enb_info->enb_id;

	for (idx = 0; idx < p_enb_info->enb_tnl_address_list_size; idx++)
	{
		ip_addr_hash_rec.enb_tnl_address = p_enb_info->enb_tnl_address_list[idx];

		if ((p_enb_info->enb_tnl_address_list[idx].bitmask & IPV6_ADD_PRESENT) &&
				(p_enb_info->enb_tnl_address_list[idx].bitmask & IPV4_ADD_PRESENT))
		{
			SON_LOG(log_mode, p_son_x2_facility_name,
					SON_WARNING,
					" Neighbor eNB 0x%x could not be added."
					" As both IPv4 and IPV6 address specified in single tnl address entry",
					son_convert_char_cell_id_to_int_cell_id(p_enb_info->enb_id.enb_id));
			*p_error_code = SON_ERR_INVALID_PARAMS;
			return SON_PNULL;
		}
		/* SPR 7528 IPV6 Support Start */
		if (!(p_enb_info->enb_tnl_address_list[idx].bitmask & IPV6_ADD_PRESENT) &&
				!(p_enb_info->enb_tnl_address_list[idx].bitmask & IPV4_ADD_PRESENT))
		{
			SON_LOG(log_mode, p_son_x2_facility_name,
					SON_WARNING,
					" Neighbor eNB 0x%x could not be added."
					" As none of the IPV4 or IPV6 address is received",
					son_convert_char_cell_id_to_int_cell_id(p_enb_info->enb_id.enb_id));
			*p_error_code = SON_ERR_INVALID_PARAMS;
			return SON_PNULL;
		}
		/* SPR 7528 IPV6 Support End */
		/*find the corresponding IP address in the mapping table */
		if (SON_PNULL != hash_find(&g_x2_gb_ctxt.nbr_enb_ip_addr_map,
					(son_void_t *)&ip_addr_hash_rec))
		{
			SON_LOG(log_mode, p_son_x2_facility_name,
					SON_WARNING,
					" Neighbor eNB 0x%x could not be added."
					" As another eNB is already configured with IP address of given eNB",
					son_convert_char_cell_id_to_int_cell_id(p_enb_info->enb_id.enb_id));
			/*SPR_17670_START*/
			/*Error code set to SON_ERR_ANR_DUPLICATE_NR so as to trigger
			  Delete eNB req to L3 in this case */
			*p_error_code = SON_ERR_DUPLICATE_IP_IN_LINK_UP;
			/*SPR_17670_END*/
			return SON_PNULL;
		}
	}

	ANR_GET_HASH_REC(x2_get_enb_hash_rec_pool(), p_hash_tpl);

	if (SON_PNULL == p_hash_tpl)
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_ERROR,
				"Memory allocation failed for p_hash_tpl. Neighbor eNB 0x%x"
				" could not be added",
				son_convert_char_cell_id_to_int_cell_id(hash_rec.enb_id.enb_id));

		*p_error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	/*Insert all the IP addresses in the IP addresses mapping*/

	for (idx = 0; idx < p_enb_info->enb_tnl_address_list_size; idx++)
	{
		SON_MEMCPY(&ip_addr_hash_rec.enb_tnl_address,
				&p_enb_info->enb_tnl_address_list[idx],
				sizeof(ip_addr_hash_rec.enb_tnl_address));

		x2_insert_entry_in_nbr_enb_ip_addr_map(&g_x2_gb_ctxt.nbr_enb_ip_addr_map,
				(son_void_t *)&ip_addr_hash_rec,
				&error_code);

		/*Mem allocation falure*/
		if (SON_ERR_SYS_MEM_ALLOC_FAILURE == error_code)
		{
			SON_LOG(log_mode, p_son_x2_facility_name,
					SON_ERROR,
					"Neighbor eNB 0x%x"
					" could not be inserted as memory allocation failed at IP address insertions",
					son_convert_char_cell_id_to_int_cell_id(hash_rec.enb_id.enb_id));

			/*delete the idxs inserted till now*/
			for (idx1 = 0; idx1 < idx; idx1++)
			{
				x2_delete_entry_from_nbr_enb_ip_addr_map(&g_x2_gb_ctxt.nbr_enb_ip_addr_map,
						&p_enb_info->enb_tnl_address_list[idx1],
						&error_code);
			}

			ANR_FREE_HASH_REC(x2_get_enb_hash_rec_pool(), p_hash, p_hash_tpl,
					&p_hash_tpl->anchor);
			*p_error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;
			SON_UT_TRACE_EXIT();
			return SON_PNULL;
		}
	}

	/* SPR-10331 Fix Starts */
	list_init(&hash_rec.pending_gu_group_id_data.add_gu_id_list);
	list_init(&hash_rec.pending_gu_group_id_data.delete_gu_id_list);
	list_init(&hash_rec.sent_gu_group_id_data.add_gu_id_list);
	list_init(&hash_rec.sent_gu_group_id_data.delete_gu_id_list);
	/* SPR-10331 Fix Ends */
	/*SPR_16835_START*/	   
	hash_rec.txn_id_for_del_req = SON_INVALID_TXN_ID;  
	/*SPR_16835_END*/	    
	SON_MEMCPY(&p_hash_tpl->data, &hash_rec, sizeof(son_nbr_enb_hash_rec_t));

	SON_LOG(log_mode, p_son_x2_facility_name,
			SON_INFO,
			"Neighbor eNB 0x%x"
			" received is added to the NR hash table",
			son_convert_char_cell_id_to_int_cell_id(hash_rec.enb_id.enb_id));

	hash_insert(p_hash, &p_hash_tpl->anchor);

	if (SON_ANR_ENB_X2_HO_STATUS_PRESENT & p_enb_info->bitmask)
	{
		anr_update_x2_status_nbr_cells_nrt(&p_enb_info->enb_id);
	}

	*p_error_code = SON_NO_ERROR;
	SON_UT_TRACE_EXIT();
	return &p_hash_tpl->data;
} /* x2_insert_nbr_enb */

/******************************************************************************
 * Function Name  : x2_del_nbr_enb
 * Inputs         : p_hash         - Pointer to the neighbor eNB hash context
 *                : p_nbr_enb_id   - Pointer to the neighbor enb id
 * Outputs        : p_error_code   - Pointer to a variable for storing error code
 *
 * Returns        : SON_SUCCESS: The message is been handled successfully
 *                  SON_FAILURE: The message is not been handled successfully
 * Description    : This function is used for removing selected neighbor eNB
 *                  records from the nbr eNB context table
 ******************************************************************************/
son_return_et
x2_del_nbr_enb
(
 SON_HASH *p_hash,
 son_global_enb_id_t  *p_nbr_enb_id,
 son_error_et *p_error_code
 )
{
	son_nbr_enb_hash_rec_t hash_data;
	son_nbr_enb_hash_tuple_t      *p_tpl       = SON_PNULL;
	SON_HASH_NODE                 *p_hash_node = SON_PNULL;
	son_oam_log_on_off_et log_mode     = X2_GET_LOG_MODE;
	son_u32 idx           = 0;
	son_error_et error_code   = SON_NO_ERROR;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_hash || SON_PNULL == p_nbr_enb_id || SON_PNULL == p_error_code)
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_WARNING,
				" Neighbor eNB could not be deleted, "
				"p_hash or p_nbr_enb_id or p_error_code or p_hash_rec is NULL");
		SON_UT_TRACE_EXIT();
		return SON_FAILURE;
	}

	SON_MEMSET(&hash_data, 0, sizeof(son_nbr_enb_hash_rec_t));
	SON_MEMCPY(&hash_data.enb_id, p_nbr_enb_id, sizeof(son_global_enb_id_t));

	if (SON_PNULL != (p_hash_node = hash_find(p_hash, (son_void_t *)&hash_data)))
	{
		p_tpl = (son_nbr_enb_hash_tuple_t *)
			YMEMBEROF(son_nbr_enb_hash_tuple_t, anchor, p_hash_node);

		/*Delete the latest IP addresses*/
		for (idx = 0; idx < p_tpl->data.enb_tnl_address_list_size; idx++)
		{
			x2_delete_entry_from_nbr_enb_ip_addr_map(&g_x2_gb_ctxt.nbr_enb_ip_addr_map,
					&p_tpl->data.enb_tnl_address_list[idx],
					&error_code);
		}

		ANR_FREE_HASH_REC(x2_get_enb_hash_rec_pool(), p_hash, p_tpl,
				p_hash_node);

		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_INFO,
				"Neighbor eNB 0x%x"
				" is successfully deleted from the table",
				son_convert_char_cell_id_to_int_cell_id(p_nbr_enb_id->enb_id));

		anr_delete_nbr_cells_nrt(p_nbr_enb_id);

		SON_UT_TRACE_EXIT();
		*p_error_code = SON_NO_ERROR;
		return SON_SUCCESS;
	}

	SON_LOG(log_mode, p_son_x2_facility_name,
			SON_WARNING,
			"Neighbor eNB 0x%x"
			" not found in the table", son_convert_char_cell_id_to_int_cell_id(p_nbr_enb_id->enb_id));

	SON_UT_TRACE_EXIT();
	*p_error_code = SON_ERR_ANR_NR_NOT_FOUND;
	return SON_FAILURE;
} /* x2_del_nbr_enb */

/******************************************************************************
 * Function Name  : x2_modify_nbr_enb
 * Inputs         : p_hash          - Pointer to the neighbor eNB  hash context.
 *                : p_enb_info      - Pointer to a structure for updating the existing
 *                                     neighbor eNB record
 * Outputs        : p_error_code    -  Pointer to a variable for storing error code
 *
 * Returns        :son_nbr_enb_hash_rec_t* -Pointer to the data field of the modified
 *                                           in the nbr eNB context table
 * Description    : This function is used for updating selected neighbor eNB
 *                  record
 ******************************************************************************/
son_nbr_enb_hash_rec_t *
x2_modify_nbr_enb
(
 SON_HASH *p_hash,
 son_neighbor_enb_info_t *p_enb_info,
 son_error_et *p_error_code
 )
{
	son_nbr_enb_hash_tuple_t          *p_tpl       = SON_PNULL;
	son_nbr_enb_hash_rec_t            *p_ret_val = SON_PNULL;
	son_u32 bitmask       = 0;
	son_oam_log_on_off_et log_mode        = X2_GET_LOG_MODE;
	son_error_et error_code  = SON_NO_ERROR;
	son_global_enb_id_t             *p_gl_enb_id  = SON_PNULL;
	son_nbr_enb_ip_addr_hash_rec_t ip_addr_hash_rec;
	son_u32 idx = 0;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_hash || SON_PNULL == p_enb_info || SON_PNULL == p_error_code)
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_WARNING,
				"Neighbor eNB could not be modified,"
				" p_hash or p_enb_info  is NULL");
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	if (!p_enb_info->bitmask)
	{
		SON_LOG(log_mode, p_son_x2_facility_name, SON_WARNING,
				"Neighbor eNB info for eNB 0x%x"
				"is empty in update request",
				son_convert_char_cell_id_to_int_cell_id(p_enb_info->enb_id.enb_id));
		*p_error_code = SON_ERR_INVALID_PARAMS;
		return SON_PNULL;
	}

	if ((p_enb_info->bitmask & SON_ANR_ENB_TAC_PRESENT) &&
			((0 == p_enb_info->tac_list_size) ||
			 (SON_FALSE == is_tac_list_valid(p_enb_info))))
	{
		SON_LOG(log_mode, p_son_x2_facility_name, SON_WARNING,
				"Neighbor eNB info for eNB 0x%x"
				"is having INVALID TAC values",
				son_convert_char_cell_id_to_int_cell_id(p_enb_info->enb_id.enb_id));
		*p_error_code = SON_ERR_INVALID_PARAMS;
		return SON_PNULL;
	}

	SON_MEMSET(&ip_addr_hash_rec, 0, sizeof(son_nbr_enb_ip_addr_hash_rec_t));

	bitmask = SON_ANR_ENB_TNL_ADD_LIST_PRESENT & p_enb_info->bitmask;

	for (idx = 0; bitmask  && idx < p_enb_info->enb_tnl_address_list_size; idx++)
	{
		if ((p_enb_info->enb_tnl_address_list[idx].bitmask & IPV6_ADD_PRESENT) &&
				(p_enb_info->enb_tnl_address_list[idx].bitmask & IPV4_ADD_PRESENT))
		{
			SON_LOG(log_mode, p_son_x2_facility_name,
					SON_WARNING, 
					" Neighbor eNB 0x%x could not be update"
					" as both IPv4 and IPV6 address specified in single tnl address entry",
					son_convert_char_cell_id_to_int_cell_id(p_enb_info->enb_id.enb_id));
			*p_error_code = SON_ERR_INVALID_PARAMS;
			return SON_PNULL;
		}

		/* SPR 7528 IPV6 Support Start */
		if (!(p_enb_info->enb_tnl_address_list[idx].bitmask & IPV6_ADD_PRESENT) &&
				!(p_enb_info->enb_tnl_address_list[idx].bitmask & IPV4_ADD_PRESENT))
		{
			SON_LOG(log_mode, p_son_x2_facility_name,
					SON_WARNING, 
					" Neighbor eNB 0x%x could not be updated"
					" As none of the IPV4 or IPV6 address is received",
					son_convert_char_cell_id_to_int_cell_id(p_enb_info->enb_id.enb_id));
			*p_error_code = SON_ERR_INVALID_PARAMS;
			return SON_PNULL;
		}
		/* SPR 7528 IPV6 Support End */
		/*find the corresponding IP address in the mapping table */
		p_gl_enb_id  =  x2_get_enb_id_from_nbr_enb_ip_addr_map(&g_x2_gb_ctxt.nbr_enb_ip_addr_map,
				&p_enb_info->enb_tnl_address_list[idx],
				&error_code);

		/*check the existing */
		if (SON_PNULL !=  p_gl_enb_id &&
				SON_MEMCMP(&p_enb_info->enb_id, p_gl_enb_id, sizeof(son_global_enb_id_t)))
		{
			SON_LOG(log_mode, p_son_x2_facility_name,
					SON_WARNING, 
					"Neighbor eNB 0x%x could not be updated"
					" as another eNB 0x%x is already configured with IP address of given eNB",
					son_convert_char_cell_id_to_int_cell_id(p_enb_info->enb_id.enb_id),
					son_convert_char_cell_id_to_int_cell_id(p_gl_enb_id->enb_id));
			*p_error_code = SON_ERR_INVALID_PARAMS;
			return SON_PNULL;
		}
	}

	if (SON_PNULL != (p_tpl = x2_find_nbr_enb(p_hash, &p_enb_info->enb_id,
					&error_code)))
	{
		p_ret_val = &p_tpl->data;
		*p_error_code = SON_NO_ERROR;

		if (SON_ANR_ENB_CUR_X2_CONN_PRESENT & p_enb_info->bitmask)
		{
			switch (p_enb_info->x2_connection_status)
			{
				case SON_X2_CONNECT_WHITELIST:
					{
						if (X2_ENB_STATE_DISCONNECTED != p_tpl->data.enb_curr_fsm_state)
						{
							p_ret_val = SON_PNULL;
							if (X2_ENB_STATE_CONNECTED == p_tpl->data.enb_curr_fsm_state)
							{
								p_tpl->data.nr_status = SON_NR_LOCKED;
							}
						}

						break;
					}

				case SON_X2_CONNECT:
					{
						if (X2_ENB_STATE_DISCONNECTED != p_tpl->data.enb_curr_fsm_state)
						{
							p_ret_val = SON_PNULL;
						}
                        /* BugId - 21401 start */
                        if (X2_ENB_STATE_CONNECTED == p_tpl->data.enb_curr_fsm_state)
                        {
                            SON_LOG(log_mode, p_son_x2_facility_name,
                                    SON_INFO, "nr_status to UNLOCK");
                            p_tpl->data.nr_status = SON_NR_UNLOCKED;
                        }
                        /* BugId - 21401 end */

						break;
					}

				case SON_X2_DISCONNECT_BLACKLIST:
					{
						if (X2_ENB_STATE_CONNECTED != p_tpl->data.enb_curr_fsm_state)
						{
							p_ret_val = SON_PNULL;
							if (X2_ENB_STATE_DISCONNECTED == p_tpl->data.enb_curr_fsm_state)
							{
								p_tpl->data.nr_status = SON_NR_LOCKED;
							}
						}

						break;
					}

				case SON_X2_DISCONNECT:
					{
						if (X2_ENB_STATE_CONNECTED != p_tpl->data.enb_curr_fsm_state)
						{
							p_ret_val = SON_PNULL;
						}

						break;
					}

				default:
					{
						*p_error_code = SON_ERR_INVALID_PARAMS;
						p_ret_val = SON_PNULL;
						break;
					}
			} /* switch */

			if (SON_NO_ERROR == *p_error_code)
			{
				if (X2_ENB_STATE_CONNECTING == p_tpl->data.enb_curr_fsm_state)
				{
					*p_error_code = SON_ERR_X2_CONNECTION_IN_PROGRESS;
					p_ret_val = SON_PNULL;
				}
				else if (X2_ENB_STATE_DISCONNECTING == p_tpl->data.enb_curr_fsm_state)
				{
					*p_error_code = SON_ERR_X2_DISCONNECTION_IN_PROGRESS;
					p_ret_val = SON_PNULL;
				}
			}
		}

		if (SON_NO_ERROR == *p_error_code)
		{
			if (bitmask && p_enb_info->enb_tnl_address_list_size)
			{
				ip_addr_hash_rec.enb_id =   p_enb_info->enb_id;

				/*Delete the Old  IP addresses*/
				for (idx = 0; idx < p_tpl->data.enb_tnl_address_list_size; idx++)
				{
					x2_delete_entry_from_nbr_enb_ip_addr_map(&g_x2_gb_ctxt.nbr_enb_ip_addr_map,
							&p_tpl->data.enb_tnl_address_list[idx],
							&error_code);
				}

				/*Add the latest IP addresses. Assuming that insertion ndoes not fails*/
				for (idx = 0; idx < p_enb_info->enb_tnl_address_list_size; idx++)
				{
					SON_MEMCPY(&ip_addr_hash_rec.enb_tnl_address,
							&p_enb_info->enb_tnl_address_list[idx],
							sizeof(ip_addr_hash_rec.enb_tnl_address));
					x2_insert_entry_in_nbr_enb_ip_addr_map(&g_x2_gb_ctxt.nbr_enb_ip_addr_map,
							&ip_addr_hash_rec,
							&error_code);
				}

				p_tpl->data.enb_tnl_address_list_size = p_enb_info->enb_tnl_address_list_size;

				SON_MEMCPY(p_tpl->data.enb_tnl_address_list,
						p_enb_info->enb_tnl_address_list,
						sizeof(p_tpl->data.enb_tnl_address_list));
			}

			bitmask = SON_ANR_ENB_X2_HO_STATUS_PRESENT & p_enb_info->bitmask;
			if (bitmask && (p_tpl->data.x2_status !=
						p_enb_info->x2_status))
			{
				p_tpl->data.x2_status = p_enb_info->x2_status;
				anr_update_x2_status_nbr_cells_nrt(&p_enb_info->enb_id);
			}

			if (SON_ANR_ENB_TAC_PRESENT & p_enb_info->bitmask)
			{
				SON_MEMCPY(p_tpl->data.tac_list, p_enb_info->tac_list, sizeof(p_tpl->data.tac_list));
				p_tpl->data.tac_list_size = p_enb_info->tac_list_size;
			}
		}

		SON_UT_TRACE_EXIT();
		return p_ret_val;
	}

	SON_LOG(log_mode, p_son_x2_facility_name, SON_WARNING,
			"Neighbor eNB 0x%x"
			" not present in the table",
			son_convert_char_cell_id_to_int_cell_id(p_enb_info->enb_id.enb_id));

	*p_error_code = SON_ERR_ANR_NR_NOT_FOUND;

	SON_UT_TRACE_EXIT();
	return p_ret_val;
} /* x2_modify_nbr_enb */

/*****************************************************************************
 * Function Name  : x2_find_nbr_enb
 * Inputs         : p_hash -   Pointer to the  neighbor eNB hash context.
 *                : p_nbr_enb_id   - Pointer to the neighbor eNB Id.
 * Outputs        : p_error_code -  Pointer to a variable for storing error code
 *
 * Returns        : son_nbr_enb_hash_tuple_t* - Pointer to the neighbor eNB tuple
 * Description     : This function returns the eNB info of a specified neighbor
 ******************************************************************************/
son_nbr_enb_hash_tuple_t *
x2_find_nbr_enb
(
 SON_HASH            *p_hash,
 son_global_enb_id_t *p_nbr_enb_id,
 son_error_et        *p_error_code
 )
{
	son_nbr_enb_hash_rec_t hash_data;
	SON_HASH_NODE                *p_node     = SON_PNULL;
	son_nbr_enb_hash_tuple_t     *p_tpl      = SON_PNULL;
	son_oam_log_on_off_et log_mode        = X2_GET_LOG_MODE;

	SON_UT_TRACE_ENTER();
	SON_MEMSET(&hash_data, 0, sizeof(son_nbr_enb_hash_rec_t));

	if (SON_PNULL == p_hash || SON_PNULL == p_nbr_enb_id ||
			SON_PNULL == p_error_code)
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_WARNING,
				"Neighbor eNB could not be found, "
				"p_hash or p_nbr_enb_id or p_error_code is NULL");
		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	SON_MEMCPY(&hash_data.enb_id, p_nbr_enb_id, sizeof(son_global_enb_id_t));

	if (SON_PNULL != (p_node = hash_find(p_hash, (son_void_t *)&hash_data)))
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_BRIEF,
				"Neighbor eNB Id 0x%x"
				" is found in the table",
				son_convert_char_cell_id_to_int_cell_id(p_nbr_enb_id->enb_id));

		p_tpl = (son_nbr_enb_hash_tuple_t *)
			YMEMBEROF(son_nbr_enb_hash_tuple_t, anchor, p_node);
		SON_UT_TRACE_EXIT();
		return p_tpl;
	}

	SON_LOG(log_mode, p_son_x2_facility_name,
			SON_WARNING,
			"Neighbor eNB 0x%x"
			" not found in the table", son_convert_char_cell_id_to_int_cell_id(p_nbr_enb_id->enb_id));

	*p_error_code = SON_ERR_ANR_NR_NOT_FOUND;

	SON_UT_TRACE_EXIT();

	return SON_PNULL;
} /* x2_find_nbr_enb */

/*****************************************************************************
 * Function Name  : x2_get_first_nbr_enb_from_nrt
 * Inputs         : p_hash -   Pointer to the neighbor eNB hash context
 * Returns        : son_nbr_enb_hash_tuple_t*- Pointer to the first neighbor eNB
 * Description    : Function to return the first neighbor eNB record from neighbor
 *                 eNB context table
 ******************************************************************************/
son_nbr_enb_hash_tuple_t *
x2_get_first_nbr_enb_from_nrt
(
 SON_HASH *p_hash
 )
{
	son_nbr_enb_hash_tuple_t *p_tpl = SON_PNULL;
	SON_HASH_NODE *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_hash)
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_WARNING,
				"NR could not be fetched, p_hash is NULL");
		SON_UT_TRACE_EXIT();
		return p_tpl;
	}

	p_node = hash_get_first(p_hash);
	if (SON_PNULL != p_node)
	{
		p_tpl = (son_nbr_enb_hash_tuple_t *)
			YMEMBEROF(son_nbr_enb_hash_tuple_t, anchor, p_node);
	}

	SON_UT_TRACE_EXIT();
	return p_tpl;
} /* x2_get_first_nbr_enb_from_nrt */

/******************************************************************************
 * Function Name  : x2_get_next_nbr_enb_from_nrt
 * Inputs         : p_hash -   Pointer to the neighbor eNB hash context.
 *                : p_node -   Pointer to the current hash node
 * Returns        : son_nbr_enb_hash_tuple_t* - Pointer to the next
 *                              neighbor eNB hash tuple
 * Description    : Function to return the next neighbor eNB in the table
 ******************************************************************************/
son_nbr_enb_hash_tuple_t *
x2_get_next_nbr_enb_from_nrt
(
 SON_HASH        *p_hash,
 SON_HASH_NODE   *p_node
 )
{
	son_nbr_enb_hash_tuple_t *p_tpl = SON_PNULL;

	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_hash || SON_PNULL == p_node)
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_ERROR,
				" Next Neighbor eNB could not be fetched, p_hash is NULL");
		SON_UT_TRACE_EXIT();
		return p_tpl;
	}

	p_node = hash_get_next(p_hash, p_node);
	if (SON_PNULL != p_node)
	{
		p_tpl = (son_nbr_enb_hash_tuple_t *)
			YMEMBEROF(son_nbr_enb_hash_tuple_t, anchor, p_node);
	}

	SON_UT_TRACE_EXIT();
	return p_tpl;
} /* x2_get_next_nbr_enb_from_nrt */

/******************************************************************************
 * Function Name  : x2_clear_enb_table
 * Inputs         : p_hash -   Pointer to the neighbor eNB hash context
 * Returns        : son_void_t
 * Description    : This function clears entire eNB hash context.
 ******************************************************************************/
son_void_t
x2_clear_enb_table
(
 SON_HASH *p_hash
 )
{
	SON_HASH_NODE *p_node        = SON_PNULL;
	SON_HASH_NODE *p_node_tmp    = SON_PNULL;
	son_nbr_enb_hash_tuple_t *p_tpl = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_node =  hash_get_first(p_hash);

	while (p_node)
	{
		p_node_tmp = p_node;
		p_tpl = (son_nbr_enb_hash_tuple_t *)YMEMBEROF(son_nbr_enb_hash_tuple_t,
				anchor,
				p_node);
		p_node = hash_get_next(p_hash, p_node);
		x2_clear_enb_cfg_pending_list(&p_tpl->data.enb_config_cell_info);
		/* SPR-10331 Fix Starts */
		x2_clear_ecu_gu_group_id_data(&p_tpl->data.pending_gu_group_id_data);
		x2_clear_ecu_gu_group_id_data(&p_tpl->data.sent_gu_group_id_data);
		/* SPR-10331 Fix Ends */

		ANR_FREE_HASH_REC(x2_get_enb_hash_rec_pool(), p_hash, p_tpl,
				p_node_tmp);
	}

	/* Coverity CID-48281 Fix Starts */
	ANR_FREE_TABLE(p_hash->table);
	/* Coverity CID-48281 Fix Ends */
	SON_UT_TRACE_EXIT();
} /* x2_clear_enb_table */

/******************************************************************************
 * Function Name  : x2_clear_enb_ip_addr_table
 * Inputs         : p_hash -   Pointer to the neighbor eNB IP address hash context
 * Returns        : son_void_t
 * Description    : This function clears entire eNB IP address hash context.
 ******************************************************************************/
son_void_t
x2_clear_enb_ip_addr_table
(
 SON_HASH *p_hash
 )
{
	SON_HASH_NODE *p_node        = SON_PNULL;
	SON_HASH_NODE *p_node_tmp    = SON_PNULL;
	son_nbr_enb_ip_addr_hash_tuple_t *p_tpl = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_node =  hash_get_first(p_hash);

	while (p_node)
	{
		p_node_tmp = p_node;
		p_tpl = (son_nbr_enb_ip_addr_hash_tuple_t *)YMEMBEROF(son_nbr_enb_ip_addr_hash_tuple_t,
				anchor,
				p_node);
		p_node = hash_get_next(p_hash, p_node);
		ANR_FREE_HASH_REC(x2_get_enb_ip_addr_hash_rec_pool(), p_hash, p_tpl,
				p_node_tmp);
	}

	/* Coverity CID-48280 Fix Starts */
	ANR_FREE_TABLE(p_hash->table);
	/* Coverity CID-48280 Fix Ends */

	SON_UT_TRACE_EXIT();
} /* x2_clear_enb_ip_addr_table */

/* SPR Fix Start */

/******************************************************************************
 * Function Name  : x2_clear_nbr_ecgi_enb_id_map
 * Inputs         : p_hash      Pointer to nbr_ecgi_enb_id_map hash table
 * Returns        : None
 * Description    : This function clears entire nbr_ecgi_enb_id_map
 ******************************************************************************/
son_void_t
x2_clear_nbr_ecgi_enb_id_map
(
 SON_HASH *p_hash
 )
{
	SON_HASH_NODE *p_node        = SON_PNULL;
	SON_HASH_NODE *p_node_tmp    = SON_PNULL;
	son_nbr_ecgi_enb_id_hash_tuple_t *p_tpl = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_node =  hash_get_first(p_hash);
	while (p_node)
	{
		p_node_tmp = p_node;
		p_tpl = (son_nbr_ecgi_enb_id_hash_tuple_t *)YMEMBEROF(
				son_nbr_ecgi_enb_id_hash_tuple_t,
				anchor,
				p_node);
		p_node = hash_get_next(p_hash, p_node);
		ANR_FREE_HASH_REC(x2_get_nbr_ecgi_enb_id_hash_rec_pool(), p_hash,
				p_tpl,
				p_node_tmp);
	}

	SON_UT_TRACE_EXIT();
} /* x2_clear_nbr_ecgi_enb_id_map */

/* SPR Fix End */

/******************************************************************************
 * Function Name  : x2_init_nbr_enb_db
 * Inputs         : None
 * Returns        : son_return_et
 * Description    : This function initialize db for neighbor eNBs
 ******************************************************************************/
son_return_et
x2_init_nbr_enb_db
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	son_return_et ret_val = SON_SUCCESS;

	SON_UT_TRACE_ENTER();

	do
	{
		if (SON_SUCCESS != (ret_val = x2_init_nbr_enb_hash_node_pool()))
		{
			break;
		}

		if (SON_SUCCESS != (ret_val = x2_init_nbr_enb_hash_tuple_pool()))
		{
			x2_destroy_nbr_enb_hash_node_pool();
			break;
		}

		if (SON_SUCCESS != (ret_val = x2_init_nbr_enb_ip_addr_hash_tuple_pool()))
		{
			x2_destroy_nbr_enb_hash_node_pool();
			x2_destroy_nbr_enb_hash_tuple_pool();
			break;
		}

		/* SPR Fix Start */
		if (SON_SUCCESS != (ret_val = x2_init_nbr_ecgi_enb_id_hash_tuple_pool()))
		{
			x2_destroy_nbr_enb_hash_node_pool();
			x2_destroy_nbr_enb_hash_tuple_pool();
			x2_destroy_nbr_enb_ip_addr_hash_tuple_pool();
			break;
		}

		/* SPR Fix End */
	} while (0);

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* x2_init_nbr_enb_db */

/******************************************************************************
 * Function Name  : x2_clear_nbr_enb_db
 * Inputs         : None
 * Returns        : son_void_t
 * Description    : This function clears db for neighbor eNBs
 ******************************************************************************/
son_void_t
x2_clear_nbr_enb_db
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	SON_UT_TRACE_ENTER();
	x2_destroy_nbr_enb_hash_tuple_pool();
	x2_destroy_nbr_enb_ip_addr_hash_tuple_pool();
	x2_destroy_nbr_enb_hash_node_pool();
	/* SPR Fix Start */
	x2_destroy_nbr_ecgi_enb_id_hash_tuple_pool();
	/* SPR Fix End */
	SON_UT_TRACE_EXIT();
}

/******************************************************************************
 * Function Name  : x2_init_enb_neighbor_info
 * Inputs         : p_enb_id      - Enb id
 *                  p_enb_info    - Pointer to enb data
 *                  tnl_addr_size - TNL address list size
 *                  p_tnl_add - Pointer to the TNL addresses
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function initialized the data to insert in eNB context
 ******************************************************************************/
son_void_t
x2_init_enb_neighbor_info
(
 son_neighbor_enb_info_t *p_enb_info,
 son_global_enb_id_t *p_enb_id,
 son_u8 tnl_addr_size,
 son_enb_tnl_address_t *p_tnl_add,
 son_u8 *p_tac
 )
{
	son_oam_log_on_off_et log_mode     = X2_GET_LOG_MODE;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_enb_info || SON_PNULL == p_enb_id)
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_WARNING,
				"Could not "
				"initialize the eNB Info, p_enb_info or p_enb_id is NULL");
		SON_UT_TRACE_EXIT();
		return;
	}

	SON_MEMCPY(&p_enb_info->enb_id, p_enb_id, sizeof(son_global_enb_id_t));

	if (tnl_addr_size && SON_PNULL != p_tnl_add)
	{
		p_enb_info->enb_tnl_address_list_size = tnl_addr_size;

		SON_MEMCPY(p_enb_info->enb_tnl_address_list,
				p_tnl_add,
				p_enb_info->enb_tnl_address_list_size * (sizeof(son_enb_tnl_address_t)));

		p_enb_info->bitmask |= SON_ANR_ENB_TNL_ADD_LIST_PRESENT;
	}

	if (SON_PNULL != p_tac)
	{
		SON_MEMCPY(p_enb_info->tac_list[0].tac, p_tac, sizeof(p_enb_info->tac_list[0].tac));
		p_enb_info->tac_list_size = 1;
		p_enb_info->bitmask |= SON_ANR_ENB_TAC_PRESENT;
	}

	p_enb_info->x2_status = SON_X2_ALLOWED;
	p_enb_info->bitmask |= SON_ANR_ENB_X2_HO_STATUS_PRESENT;

	p_enb_info->bitmask |= SON_ANR_ENB_CUR_X2_CONN_PRESENT;
	p_enb_info->x2_connection_status = SON_X2_CONNECT;

	SON_UT_TRACE_EXIT();
} /* x2_init_enb_neighbor_info */

/* SPR Fix Start */

/******************************************************************************
 * Function Name  : x2_insert_entry_in_nbr_ecgi_enb_id_map
 * Inputs         : p_hash              - Pointer to nbr_ecgi_enb_id_map
 *                                        hash record
 *                  p_hash_rec          - Pointer to the nbr_ecgi_enb_id_map
 *                                        hash record
 * Outputs        : None
 * Returns        : Pointer to the data of nbr_ecgi_enb_id_map
 * Description    : This function is used to insert entry in
 *                  nbr_ecgi_enb_id_map storing the mapping of neighbor cells
 *                  ECGI and eNB IDs
 ******************************************************************************/
son_nbr_ecgi_enb_id_hash_rec_t *
x2_insert_entry_in_nbr_ecgi_enb_id_map
(
 SON_HASH                        *p_hash,
 son_nbr_ecgi_enb_id_hash_rec_t  *p_hash_rec
 )
{
	son_nbr_ecgi_enb_id_hash_tuple_t *p_hash_tpl =  SON_PNULL;
	son_oam_log_on_off_et log_mode = X2_GET_LOG_MODE;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_hash || SON_PNULL == p_hash_rec)
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_WARNING,
				"Could not "
				"insert entry for neighbor as either p_hash"
				" or p_hash_rec or p_error_code is NULL");

		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	if (SON_PNULL != hash_find(p_hash, (son_void_t *)p_hash_rec))
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_INFO,
				"Neighbor EGCI already present in the nbr_ecgi_enb_id_map.");

		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	ANR_GET_HASH_REC(x2_get_nbr_ecgi_enb_id_hash_rec_pool(), p_hash_tpl);

	if (SON_PNULL == p_hash_tpl)
	{
		SON_LOG(log_mode, p_son_x2_facility_name,
				SON_WARNING,
				"Memory allocation failed for p_hash_tpl. Neighbor ECGI "
				"could not be added");

		SON_UT_TRACE_EXIT();
		return SON_PNULL;
	}

	SON_MEMCPY(&p_hash_tpl->data, p_hash_rec,
			sizeof(son_nbr_ecgi_enb_id_hash_rec_t));

	hash_insert(p_hash, &p_hash_tpl->anchor);
	SON_LOG(log_mode, p_son_x2_facility_name,
			SON_INFO,
			"Neighbor Cell"
			" 0x%x is added to the nbr_ecgi_enb_id_map",
			son_convert_char_cell_id_to_int_cell_id(p_hash_tpl->data.nbr_cell_id.cell_identity));

	SON_UT_TRACE_EXIT();
	return &p_hash_tpl->data;
} /* x2_insert_entry_in_nbr_ecgi_enb_id_map */

/******************************************************************************
 * Function Name  : x2_get_first_nbr_info_from_nbr_ecgi_enb_id_map
 * Inputs         : p_hash -   Pointer to the table nbr_ecgi_enb_id_map
 * Outputs        : None
 * Returns        : Pointer to first node of nbr_ecgi_enb_id_map hash table
 * Description    : Function to return the first neighhbor information from
 *                  nbr_ecgi_enb_id_map
 ******************************************************************************/
son_nbr_ecgi_enb_id_hash_tuple_t *
x2_get_first_nbr_info_from_nbr_ecgi_enb_id_map
(
 SON_HASH *p_hash
 )
{
	son_nbr_ecgi_enb_id_hash_tuple_t *p_tpl = SON_PNULL;
	SON_HASH_NODE *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_hash)
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_WARNING,
				"Failed to fetch entry from nbr_ecgi_enb_id_map, "
				"p_hash is NULL");
		SON_UT_TRACE_EXIT();
		return p_tpl;
	}

	p_node = hash_get_first(p_hash);
	if (SON_PNULL != p_node)
	{
		p_tpl = (son_nbr_ecgi_enb_id_hash_tuple_t *)
			YMEMBEROF(son_nbr_ecgi_enb_id_hash_tuple_t, anchor, p_node);
	}

	SON_UT_TRACE_EXIT();
	return p_tpl;
} /* x2_get_first_nbr_info_from_nbr_ecgi_enb_id_map */

/******************************************************************************
 * Function Name  : x2_get_next_nbr_info_from_nbr_ecgi_enb_id_map
 * Inputs         : p_hash -   Pointer to the table nbr_ecgi_enb_id_map
 *                : p_node -   Pointer to the current hash node
 * Returns        : Pointer to next node of nbr_ecgi_enb_id_map hash table
 * Description    : Function to return the next neighhbor information from
 *                  nbr_ecgi_enb_id_map
 ******************************************************************************/
son_nbr_ecgi_enb_id_hash_tuple_t *
x2_get_next_nbr_info_from_nbr_ecgi_enb_id_map
(
 SON_HASH        *p_hash,
 SON_HASH_NODE   *p_node
 )
{
	son_nbr_ecgi_enb_id_hash_tuple_t *p_tpl = SON_PNULL;

	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_hash || SON_PNULL == p_node)
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_WARNING,
				"Failed to fetch entry from nbr_ecgi_enb_id_map,"
				" p_hash is NULL");
		SON_UT_TRACE_EXIT();
		return p_tpl;
	}

	p_node = hash_get_next(p_hash, p_node);
	if (SON_PNULL != p_node)
	{
		p_tpl = (son_nbr_ecgi_enb_id_hash_tuple_t *)
			YMEMBEROF(son_nbr_ecgi_enb_id_hash_tuple_t, anchor, p_node);
	}

	SON_UT_TRACE_EXIT();
	return p_tpl;
} /* x2_get_next_nbr_info_from_nbr_ecgi_enb_id_map */

/* SPR Fix End */

/* SPR-10331 Fix Starts */
/*****************************************************************************
 * Function Name  : x2_insert_gu_grp_id_in_list
 * Inputs         : p_list      - Pointer to the list
 *                  p_gu_grp_id - Pointer to the GU Group Id to be inserted
 * Outputs        : None
 * Returns        : Pointer to newly inserted node in the sent GU Group 
 *                  Id List
 * Description    : This function inserts a node in the sent GU Group Id List
 ****************************************************************************/
x2_gu_group_id_node_t *
x2_insert_gu_grp_id_in_list
(
 SON_LIST            *p_list,
 x2ap_gu_group_id_t  *p_gu_grp_id
 )
{
	x2_gu_group_id_node_t *p_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_gu_grp_id || SON_PNULL == p_list)
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_WARNING,
				"Record could not be "
				"added as p_gu_grp_id or p_list is NULL");
		SON_UT_TRACE_EXIT();
		return p_list_node;
	}

	p_list_node = son_mem_get(sizeof(x2_gu_group_id_node_t));
	if (SON_PNULL != p_list_node)
	{
		SON_MEMCPY(&p_list_node->gu_group_id,
				p_gu_grp_id,
				sizeof(x2ap_gu_group_id_t));

		list_insert_node(p_list, &p_list_node->anchor);

		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_DETAILED,
				"GU Group Id data "
				"added in the given List");
	}
	else
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_ERROR,
				"Memory allocation failure for p_list_node");
	}

	SON_UT_TRACE_EXIT();
	return p_list_node;
} /* x2_insert_gu_grp_id_in_list */

/*****************************************************************************
 * Function Name  : son_compare_gu_grp_id
 * Inputs         : p_val_1     Pointer to the GU Group id as void pointer
 *                : p_val_2     Pointer to the GU Group id as void pointer
 * Outputs        : None
 * Returns        : Zero -  If matches
 *                  Non Zero - If not matches
 * Description    : This function compares the GU Group Ids
 ******************************************************************************/
son_s32
son_compare_gu_grp_id
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 )
{
	son_s32 ret_value = -1;

	const x2ap_gu_group_id_t *p_gu_grp_id_1 = SON_PNULL;
	const x2ap_gu_group_id_t *p_gu_grp_id_2 = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_ERROR,
				"Pointer to the GU Group Id is NULL");
	}
	else
	{
		p_gu_grp_id_1 = (x2ap_gu_group_id_t  *)p_val_1;
		p_gu_grp_id_2 = (x2ap_gu_group_id_t  *)p_val_2;
		ret_value = SON_MEMCMP(p_gu_grp_id_1, p_gu_grp_id_2,
				sizeof(x2ap_gu_group_id_t));
	}

	SON_UT_TRACE_EXIT();
	return ret_value;
} /* son_compare_gu_grp_id */

/*****************************************************************************
 * Function Name  : son_gu_grp_id_key
 * Inputs         : p_node - anchor field of the record
 * Outputs        : None
 * Returns        : pointer to GU Group id as void pointer
 * Description    : This function determines the GU Group Id which is the key
 *                  of various GU Group Ids list
 ******************************************************************************/
const son_void_t *
son_gu_grp_id_key
(
 const SON_LIST_NODE *p_node
 )
{
	x2_gu_group_id_node_t *p_data = SON_PNULL;

	SON_UT_TRACE_ENTER();
	if (SON_PNULL == p_node)
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_ERROR,
				"Pointer to node is NULL");
	}
	else
	{
		p_data = YMEMBEROF(x2_gu_group_id_node_t, anchor, p_node);
	}

	SON_UT_TRACE_EXIT();
	return (son_void_t *)&(p_data->gu_group_id);
} /* son_gu_grp_id_key */

/*****************************************************************************
 * Function Name  : x2_delete_gu_grp_id_from_list
 * Inputs         : p_list      - Pointer to the pending GU Group Id list
 *                  p_gu_grp_id - Pointer to the GU Group Id information
 * Outputs        : None
 * Returns        : SON_SUCCESS - If node is deleted succesfully
 *                  SON_FAILURE otherwise
 * Description    : This function finds the GU Group Id in the pending GU 
 *                  Group Id list and if found deletes the entry
 ******************************************************************************/
son_return_et
x2_delete_gu_grp_id_from_list
(
 SON_LIST            *p_list,
 x2ap_gu_group_id_t  *p_gu_grp_id
 )
{
	SON_LIST_NODE *p_list_node = SON_PNULL;
	x2_gu_group_id_node_t *p_data = SON_PNULL;
	son_return_et node_deleted = SON_FAILURE;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_gu_grp_id || SON_PNULL == p_list)
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_ERROR,
				"p_list or p_gu_grp_id is NULL");

		SON_UT_TRACE_EXIT();
		return node_deleted;
	}

	p_list_node = list_find_node(p_list,
			p_gu_grp_id, son_gu_grp_id_key, son_compare_gu_grp_id);

	if (SON_PNULL != p_list_node)
	{
		p_data = (x2_gu_group_id_node_t *)
			YMEMBEROF(x2_gu_group_id_node_t, anchor, p_list_node);
		list_delete_node(p_list, p_list_node);
		/* Free the memory occupied by the node */
		son_mem_free(p_data);
		node_deleted = SON_SUCCESS;
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_INFO,
				"GU Group "
				"Id [MME Id: 0x%x] deleted from the pending GU Group Id list",
				son_convert_char_cell_id_to_int_cell_id(
					p_gu_grp_id->mme_group_id));
	}
	else
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_WARNING,
				"GU Group "
				"Id [MME Id: 0x%x] not found in pending GU Group Id list",
				son_convert_char_cell_id_to_int_cell_id(
					p_gu_grp_id->mme_group_id));
	}

	SON_UT_TRACE_EXIT();
	return node_deleted;
} /* x2_delete_gu_grp_id_from_list */

/*****************************************************************************
 * Function Name  : x2_clear_ecu_gu_group_id_data
 * Inputs         : p_data Pointer to stored eNB config update request's GU 
 *                  group Id data
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Clean up the stored GU group Id data structure 
 ****************************************************************************/
son_void_t
x2_clear_ecu_gu_group_id_data
(
 son_nbr_enb_gu_group_id_info_t  *p_data
 )
{
	SON_LIST_NODE *p_node = SON_PNULL;
	x2_gu_group_id_node_t *p_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_node = list_pop_node(&p_data->add_gu_id_list);

	while (SON_PNULL != p_node)
	{
		p_list_node =  YMEMBEROF(x2_gu_group_id_node_t, anchor, p_node);
		son_mem_free(p_list_node);
		p_node = list_pop_node(&p_data->add_gu_id_list);
	}

	SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
			SON_DETAILED,
			"Cleared data of ECU GU Group "
			"Id Add List");

	p_node = list_pop_node(&p_data->delete_gu_id_list);
	while (SON_PNULL != p_node)
	{
		p_list_node =  YMEMBEROF(x2_gu_group_id_node_t, anchor, p_node);
		son_mem_free(p_list_node);
		p_node = list_pop_node(&p_data->delete_gu_id_list);
	}

	SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
			SON_DETAILED,
			"Cleared data of ECU GU Group "
			"Id Delete List");

	SON_UT_TRACE_EXIT();
} /* x2_clear_ecu_gu_group_id_data */

/*****************************************************************************
 * Function Name  : x2_get_first_node_in_gu_group_id_list
 * Inputs         : p_list - Pointer to pending GU Group Id add or delete list
 * Outputs        : None
 * Returns        : x2_gu_group_id_node_t *  - pointer to first pending GU 
 *                                             Group Id list node 
 * Description    : This function gets the first node of given GU Group Id list
 ******************************************************************************/
x2_gu_group_id_node_t *
x2_get_first_node_in_gu_group_id_list
(
 x2_gu_group_id_list_t *p_list
 )
{
	SON_LIST_NODE *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (list_count(p_list) > 0)
	{
		p_node = get_first_list_node(p_list);
		if (SON_PNULL != p_node)
		{
			SON_UT_TRACE_EXIT();
			return (x2_gu_group_id_node_t *)
				YMEMBEROF(x2_gu_group_id_node_t, anchor, p_node);
		}
	}

	SON_UT_TRACE_EXIT();
	return SON_PNULL;
} /* x2_get_first_node_in_gu_group_id_list */

/*****************************************************************************
 * Function Name  : x2_get_next_node_in_gu_group_id_list
 * Inputs         : p_node - Pointer to pending GU Group Id list node
 * Outputs        : None
 * Returns        : x2_gu_group_id_node_t * - Pointer to next node in pending 
 *                                            GU Group Id list
 * Description    : This function gets the next node of given GU Group Id list
 ******************************************************************************/
x2_gu_group_id_node_t *
x2_get_next_node_in_gu_group_id_list
(
 x2_gu_group_id_node_t *p_node
 )
{
	SON_LIST_NODE   *p_list_node    = SON_PNULL;
	SON_UT_TRACE_ENTER();

	if (SON_PNULL != p_node)
	{
		p_list_node = get_next_list_node(&p_node->anchor);
		if (SON_PNULL != p_list_node)
		{
			return (x2_gu_group_id_node_t *)
				YMEMBEROF(x2_gu_group_id_node_t, anchor, p_list_node);
		}
	}

	SON_UT_TRACE_EXIT();
	return SON_PNULL;
} /* x2_get_next_node_in_gu_group_id_list */

/* SPR-10331 Fix Ends */
