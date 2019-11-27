/*****************************************************************************
 * File Name   : oam_config_tcb.c
 *
 * Description : This file contains OAM Hash implementation.
 *
 * Revision History:
 *
 * Date         Author           Reference            Comments
 * -----        ------           ---------            --------
 * May,2012     Rajesh Kumar    OAM Design Document  Initial Version
 * July,2012    Rajesh Kumar
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 ****************************************************************************/

/****************************************************************************
 * Includes
 ***************************************************************************/
#include    <oam_config_tcb.h>

extern prov_request_t prov_req;
/****************************************************************************
 * Global variables
 ***************************************************************************/
LOCAL OAM_HASH *p_hash;
LOCAL OAM_HASH_NODE **pp_table;


/****************************************************************************
 * Function Name  : oam_hash_init
 * Inputs         : hash - callback function for hash functionality.
 *                  compare - callback function for comparison of two keys.
 *                  keyof - keyof function to return the type of node.
 *
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Initialises the hash table.
 ****************************************************************************/
oam_return_et
oam_hash_init
(
    ULong32            (*hash)(const void *),
    Int                (*compare)(const void *, const void *),
    const void         *(*keyof)(const OAM_HASH_NODE *)
)
{
    OAM_FUN_ENTRY_TRACE();
    if (OAM_PNULL == p_hash || OAM_PNULL == hash || OAM_PNULL == compare
        || OAM_PNULL == keyof ||  OAM_PNULL == pp_table)
    {
        OAM_LOG(OAM, OAM_WARNING,
                "Input pointer received is OAM_PNULL");
	OAM_FUN_EXIT_TRACE();

	return OAM_FAILURE;
    }
    yhInit(p_hash, OAM_HASH_SIZE, hash, compare, keyof, pp_table);
    OAM_LOG(OAM, OAM_DETAILED,
                "Hash Table Initialized successfully");
    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

/******************************************************************************
 * Function Name  : oam_init_hash_table
 * Inputs         : void
 * Outputs        : None
 * Returns        : oam_return_et
 * Description    : This function creates hash table
 ******************************************************************************/
oam_return_et
oam_init_hash_table
(
    void
)
{
    OAM_FUN_ENTRY_TRACE();

    /* coverity 35631 */
    oam_error_code_et p_error_code = NO_ERROR;/*COVERITY_FIX_1.0*/
    if(OAM_FAILURE == oam_mem_alloc(sizeof(OAM_HASH),
                                            (void *)&p_hash,
                                        &p_error_code))/*COVERITY_FIX_1.0*/
    {
         OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_hash failed"
                " with Error Code:%d",
                p_error_code);
         OAM_FUN_EXIT_TRACE();
         return (OAM_FAILURE);
    }

    if(OAM_FAILURE == oam_mem_alloc(OAM_HASH_SIZE *sizeof(OAM_HASH_NODE),
		                                                       (void *)&pp_table,
                                                           &p_error_code))/*COVERITY_FIX_1.0*/
    {
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to pp_table failed"
                " with Error Code:%d",
                p_error_code);
        OAM_FUN_EXIT_TRACE();
        return (OAM_FAILURE);
    }
    if(OAM_FAILURE == oam_hash_init(&oam_hash,
			                     &oam_compare,
                                  &oam_keyOf))
    {
        OAM_LOG(OAM, OAM_ERROR,
                "Memory not allocated for OAM Hash Node");
        OAM_FUN_EXIT_TRACE();
        return (OAM_FAILURE);
    }
    OAM_LOG(OAM, OAM_DETAILED,
                "Hash Table Initialized successfully");
    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_hash
 * Inputs         : p_key :Pointer to hash entry
 * Outputs        : None
 * Returns        : New key value
 * Description    : This function generates the key for fresh hash entries.
 ****************************************************************************/
ULong32
oam_hash
(
    const void *p_key
)
{
    OAM_FUN_ENTRY_TRACE();
    if(OAM_PNULL == p_key)
    {
        OAM_LOG(OAM, OAM_WARNING,
                "NULL Key Received");
        OAM_FUN_EXIT_TRACE();
        return OAM_ZERO;
    }
    ULong32 key = *(ULong32 *)p_key;
    OAM_LOG(OAM, OAM_DETAILED,
                "key returned successfully");
    OAM_FUN_EXIT_TRACE();
    return key;
}

/****************************************************************************
 * Function Name  : oam_compare
 * Inputs         : p_hashEntry1 :Pointer to hash entry 1 to be compared.
 *                  p_hashEntry2 :Pointer to hash entry 2 to be compared.
 * Outputs        : None
 * Returns        : TRUE if values match
 * Description    : This function compares the key of two hash entries.
 ****************************************************************************/
SInt32
oam_compare
(
    const void *p_hashEntry1,
    const void *p_hashEntry2
)
{
    OAM_FUN_ENTRY_TRACE();

    ULong32 newKey1 = *(ULong32 *)p_hashEntry1;
    ULong32 newKey2 = *(ULong32 *)p_hashEntry2;

    if (newKey1 == newKey2)
    {
    	OAM_FUN_EXIT_TRACE();
        return TCB_TRUE;
    }
    else
    {
        OAM_FUN_EXIT_TRACE();
        return TCB_FALSE;
    }
}

/****************************************************************************
 * Function Name  : oam_keyOf
 * Inputs         : p_hash_node :Pointer to OAM_HASH_NODE object.
 * Outputs        : None
 * Returns        : Void * containing key of the hash node.
 * Description    : to get key/transaction id of paricular hash node
 ***************************************************************************/
const void *oam_keyOf
(
    const OAM_HASH_NODE  *p_hash_node
)
{
    oam_node_t * p_oam_node;

    OAM_FUN_ENTRY_TRACE();

    p_oam_node = (oam_node_t *) YMEMBEROF(oam_node_t,
  	                          anchor,
                                  p_hash_node);
    if (p_oam_node)
    {
        if (&p_oam_node->oam_struct)
	{
		OAM_LOG(OAM, OAM_DETAILED,
				"Pointer to node returned successfully");
		OAM_FUN_EXIT_TRACE();
		return (&(p_oam_node->oam_struct.transaction_id));
	}
        else
	{
		OAM_LOG(OAM, OAM_DETAILED,
       		       "Pointer to Node returned is NULL");
		OAM_FUN_EXIT_TRACE();
            	return OAM_PNULL;
	}
    }
    else
    {
	    OAM_LOG(OAM, OAM_DETAILED,
                "Pointer to Node returned is NULL");
	    OAM_FUN_EXIT_TRACE();
        return OAM_PNULL;
    }

}

/****************************************************************************
 * Function Name  : oam_hash_insert
 * Inputs         : p_node - Pointer to the OAM_HASH_NODE object.
 * Outputs        : None.
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Variables      : None.
 * Description    : Inserts a node in the hash table.
 ****************************************************************************/
oam_return_et
oam_hash_insert
(
    OAM_HASH_NODE   *p_node
)
{
    OAM_FUN_ENTRY_TRACE();

    if (OAM_PNULL == p_hash || OAM_PNULL == p_node)
    {
        OAM_LOG(OAM, OAM_WARNING,
                "Input pointer received is OAM_PNULL");
        OAM_FUN_EXIT_TRACE();
	    return OAM_FAILURE;
    }
    yhInsert(p_hash, p_node);
	OAM_LOG(OAM, OAM_DETAILED,
                "Insert in hash table successful");
    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_hash_delete
 * Inputs         : p_node - Pointer to the OAM_HASH_NODE object.
 * Outputs        : None.
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Variables      : None.
 * Description    : Deletes a node from the hash table.
 ****************************************************************************/
oam_return_et
oam_hash_delete
(
    OAM_HASH_NODE   *p_node
)
{
    OAM_FUN_ENTRY_TRACE();
    /* coverity 35630 */
    oam_error_code_et error_code=NO_ERROR;
    if (OAM_PNULL == p_hash || OAM_PNULL == p_node)
    {
        OAM_LOG(OAM, OAM_WARNING,
                "Input pointer received is OAM_PNULL");
        OAM_FUN_EXIT_TRACE();
	    return OAM_FAILURE;
    }
    yhDelete(p_hash, p_node);
	OAM_LOG(OAM, OAM_DETAILED,
                "Deletion of node successful");
    if(p_node)
       oam_mem_free(p_node,&error_code); 
    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_hash_count
 * Inputs         : void
 * Outputs        : None.
 * Returns        : Number of nodes.
 * Variables      : None.
 * Description    : Returns the number of nodes in the hash table.
 ****************************************************************************/
UInt32
oam_hash_count
(
    void
)
{
    OAM_FUN_ENTRY_TRACE();

    UInt32 count = OAM_ZERO;
    if (OAM_PNULL == p_hash)
    {
        OAM_LOG(OAM, OAM_WARNING,
                "Input pointer received is OAM_PNULL");
    }
    else
    {
        count = yhCount(p_hash);
		OAM_LOG(OAM, OAM_DETAILED,
                "Count in hash table successful");
    }
    OAM_FUN_EXIT_TRACE();
    return count;
}

/****************************************************************************
 * Function Name  : oam_hash_find
 * Inputs         : p_item - Pointer to the key node to be found.
 * Outputs        : None.
 * Returns        : Node, if found.
 * Variables      : None.
 * Description    : Finds a key node in the hash table.
 ****************************************************************************/
OAM_HASH_NODE *
oam_hash_find
(
    const void    *p_item
)
{
    OAM_HASH_NODE *p_node = OAM_PNULL;

    OAM_FUN_ENTRY_TRACE();
    if (OAM_PNULL == p_hash || OAM_PNULL == p_item)
    {
        OAM_LOG(OAM, OAM_WARNING,
                "Input pointer received is OAM_PNULL");
    }
    else
    {
        p_node = yhFind(p_hash, p_item);
		OAM_LOG(OAM, OAM_DETAILED,
                "Find in Hash table successful");
    }
    OAM_FUN_EXIT_TRACE();
    return p_node;
}
/****************************************************************************
 * Function Name  : oam_hash_get_first
 * Inputs         : none
 * Outputs        : None.
 * Returns        : First node in the hash table.
 * Variables      : None.
 * Description    : Returns the first node in the hash table.
 ****************************************************************************/
OAM_HASH_NODE *
oam_hash_get_first
(
  void
)
{
    OAM_HASH_NODE *p_node = OAM_PNULL;
    OAM_FUN_ENTRY_TRACE();
    if (OAM_PNULL == p_hash)
    {
        OAM_LOG(OAM, OAM_WARNING,
                "Input pointer received is OAM_PNULL");
    }
    else
    {
        p_node = yhFirst(p_hash);
	OAM_LOG(OAM, OAM_DETAILED,
                "find first node in hash table successful");
    }
    OAM_FUN_EXIT_TRACE();
    return p_node;
}

/****************************************************************************
 * Function Name  : oam_hash_get_next
 * Inputs         : p_node - Pointer to the OAM_HASH_NODE object.
 * Outputs        : None.
 * Returns        : Next node to the current node.
 * Variables      : None.
 * Description    : Returns the next node in the hash table.
 ****************************************************************************/
OAM_HASH_NODE *
oam_hash_get_next
(
    OAM_HASH_NODE   *p_node
)
{
    OAM_HASH_NODE *p_next_node = OAM_PNULL;
    OAM_FUN_ENTRY_TRACE();
    if (OAM_PNULL == p_hash || OAM_PNULL == p_node)
    {
         OAM_LOG(OAM, OAM_WARNING,
                 "Input pointer received is OAM_PNULL");
    }
    else
    {
        p_next_node = yhNext(p_hash, p_node);
	OAM_LOG(OAM, OAM_DETAILED,
                "Find next node in hash table successful");
    }
    OAM_FUN_EXIT_TRACE();
    return p_next_node;
}

/****************************************************************************
 * Function Name  : oam_tcb_insert
 * Inputs         : trans_id :transaction identifier
 *                  timer_id :timer id
 *                  trans_type :transcation type
 *                  msg_len :message length
 *                  p_msg :Pointer to message
 * Outputs        : None.
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Variables      : None.
 * Description    : insert item in tcb
 ****************************************************************************/
oam_return_et
oam_tcb_insert
(
    UInt32 trans_id,
    QTIMER timer_id,
    oam_trans_type_et trans_type,
    UInt16 msg_len,
    void *p_msg
)
{
    oam_error_code_et   error_code;
    oam_node_t          *p_oam_node;
    UInt32 bitmask    =  OAM_ZERO;
    OAM_FUN_ENTRY_TRACE();
    /* coverity 35628 */
    error_code = NO_ERROR;
    if(OAM_PNULL == p_msg)
    {
        OAM_LOG(OAM, OAM_WARNING,
                    "Received argument is NULL");
        OAM_FUN_EXIT_TRACE();
        return (OAM_FAILURE);
    }
    if(trans_type == OAM_TRANS_SON)
    {
        bitmask = TCB_BITMASK_SON;
    }
/* CSR No 00055803 Start */
    /*SPR_10579 Start*/ 
    else if(trans_type == OAM_TRANS_RRC_MAC_PDCP_RRM)
    {
        bitmask = TCB_BITMASK_RRC_MAC_PDCP_RRM;
    }
    /*SPR_10579 End*/ 
/* CSR No 00055803 End */
    else if(trans_type == OAM_TRANS_RRM)
    {
        bitmask = TCB_BITMASK_RRM;
    }
    else if(trans_type == OAM_TRANS_MAC)
    {
        bitmask = TCB_BITMASK_MAC;
    }
    else if(trans_type == OAM_TRANS_RLC)
    {
        bitmask = TCB_BITMASK_RLC;
    }
    else if(trans_type == OAM_TRANS_PDCP)
    {
        bitmask = TCB_BITMASK_PDCP;
    }
    else if(trans_type == OAM_TRANS_EGTPU)
    {
        bitmask = TCB_BITMASK_EGTPU;
    }
    else if(trans_type == OAM_TRANS_RRC)
    {
        bitmask = TCB_BITMASK_RRC;
    }
    else if(trans_type == OAM_TRANS_S1AP)
    {
        bitmask = TCB_BITMASK_S1AP;
    }
    else if(trans_type == OAM_TRANS_X2AP)
    {
        bitmask = TCB_BITMASK_X2AP;
    }
    else if(trans_type == OAM_TRANS_L2ALL)
    {
        bitmask = TCB_BITMASK_L2ALL;
    }
    else if(trans_type == OAM_TRANS_L3ALL)
    {
        bitmask = TCB_BITMASK_L3ALL;
    }
    else if(trans_type == OAM_TRANS_ALL)
    {
        bitmask = TCB_BITMASK_ALL;
    }
#ifdef OAM_RADIO_HEAD_SUPPORT
    else if(OAM_TRANS_RRH == trans_type) {
        bitmask = TCB_BITMASK_RRH;
    }
#endif
    else if(trans_type == OAM_TRANS_FM)
    {
        bitmask = TCB_BITMASK_FM;
    }   
    else if(trans_type == OAM_TRANS_PM)
    {
        bitmask = TCB_BITMASK_PM;
    }
    else if(trans_type == OAM_TRANS_SM)
    {
        bitmask = TCB_BITMASK_SM;
    }
    else if(trans_type == OAM_TRANS_SYSTEM_LOG)
    {
        bitmask = TCB_BITMASK_SYS;
    }
    /* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
    else if(trans_type == OAM_TRANS_M2AP)
    {
        bitmask = TCB_BITMASK_M2AP;
    }
#endif
    /* EMBMS Changes End */
    else
    {
        OAM_LOG(OAM, OAM_WARNING,
                "INVALID TRANSACTION TYPE[%d]", trans_type);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    if(OAM_FAILURE == oam_mem_alloc(sizeof(oam_node_t),
                (void *)&p_oam_node,
                &error_code))
    {
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_oam_node failed"
                " with Error Code:%d",
                error_code);
        OAM_FUN_EXIT_TRACE();
        return (OAM_FAILURE);
    }

    p_oam_node->oam_struct.transaction_id=trans_id;
    p_oam_node->oam_struct.timer_id=timer_id;
    p_oam_node->oam_struct.bitmask = bitmask;
    /* Setting the oam_procedure_specific_context to NULL, as it is 
     * only used when tcb is insert for procedure specific data */
    /* SPR_20467_FIX_START */
    p_oam_node->oam_struct.oam_procedure_specific_context = OAM_NULL;
    /* SPR_20467_FIX_END */

    if(OAM_FAILURE == oam_mem_alloc(msg_len,
                (void *)&p_oam_node->oam_struct.oam_tcb_context,
                &error_code))
    {
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_oam_node->oam_struct.oam_tcb_context failed"
                " with Error Code:%d",
                error_code);
        /* Coverity ID : 72368 Fix Start*/
        oam_mem_free(p_oam_node, &error_code);	
        /* Coverity ID : 72368 Fix End*/	
        OAM_FUN_EXIT_TRACE();
        return (OAM_FAILURE);
    }
    oam_memcpy(p_oam_node->oam_struct.oam_tcb_context,p_msg,msg_len);
    if(OAM_FAILURE == oam_hash_insert(&p_oam_node->anchor))
    {
        OAM_LOG(OAM, OAM_WARNING,
                "Insertion of Hash Node Fails");
        /* Coverity ID : 72368 Fix Start*/
        oam_mem_free(p_oam_node->oam_struct.oam_tcb_context, &error_code);
        oam_mem_free(p_oam_node, &error_code);	
        /* Coverity ID : 72368 Fix End*/	
        OAM_FUN_EXIT_TRACE();
        return (OAM_FAILURE);

    }
    OAM_LOG(OAM, OAM_DETAILED,
            "Insertion of Hash Node successful");
    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

/*********************************************************************************
 * Function Name  : oam_tcb_delete
 * Inputs         : p_item :Pointer to item to be deleted i.e key / transaction id
 * Outputs        : None.
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Variables      : None.
 * Description    : delete item from tcb
 **********************************************************************************/

oam_return_et
oam_tcb_delete
(
    void *p_item
)
{
    OAM_FUN_ENTRY_TRACE();
    oam_struct_t * p_node = OAM_NULL;
    /* coverity 35629 */
    oam_error_code_et error_code = NO_ERROR;
    p_node=oam_tcb_get_detail(p_item);
    if(p_node != OAM_NULL && p_node->oam_tcb_context)
    {
       oam_mem_free(p_node->oam_tcb_context,&error_code);
    }
    /* SPR_20467_FIX_START */
    if(p_node != OAM_NULL && p_node->oam_procedure_specific_context)
    {
       oam_mem_free(p_node->oam_procedure_specific_context,&error_code);
    }
    /* SPR_20467_FIX_END */
    OAM_FUN_EXIT_TRACE();
    return(oam_hash_delete(oam_hash_find(p_item)));
}

/**********************************************************************************
 * Function Name  : oam_tcb_get_detail
 * Inputs         : p_item :Pointer to item to be deleted i.e key / transaction id
 * Outputs        : None
 * Returns        : Pinter to structure oam_struct_t containing details of item(key)
 * Variables      : None
 * Description    : to get details of key/transaction id from tcb
 ***********************************************************************************/
oam_struct_t *
oam_tcb_get_detail
(
    void *p_item
)
{
    oam_node_t *p_oam_node;

    OAM_FUN_ENTRY_TRACE();

    p_oam_node = (oam_node_t *) YMEMBEROF(oam_node_t,
     			                      anchor,
                               oam_hash_find(p_item));
    if (p_oam_node)
    {
        if (&p_oam_node->oam_struct)
	{
	    OAM_LOG(OAM, OAM_DETAILED,
               "Pointer to oam_struct returned successful");
	    OAM_FUN_EXIT_TRACE();
            return (&(p_oam_node->oam_struct));
	}
        else
	{
	    OAM_LOG(OAM, OAM_DETAILED,
                   "Pointer to oam_struct returned is NULL");
	    OAM_FUN_EXIT_TRACE();
            return OAM_PNULL;
	}
    }
    else
    {
        OAM_LOG(OAM, OAM_DETAILED,
                "Pointer to oam_struct returned is NULL");
        OAM_FUN_EXIT_TRACE();
        return OAM_PNULL;
    }
}

/****************************************************************************
 * Function Name  : oam_tcb_update_bitmask
 * Inputs         : transaction id :transaction identifier
 *                  response type :Response type
 *                  bitmask :Bitmask
 * Outputs        : it will loads the new bitmask if it will 0 then all the
 *                : expected responses are being handled
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Variables      : None
 * Description    : to update the bitmask of tcb enrty for keeping track of
 *                : multiple response for single transaction
 ***************************************************************************/
oam_return_et
oam_tcb_update_bitmask
(
   UInt16 trans_id,
   oam_trans_type_et trans_type,
   UInt32 *bitmask
)
{
    OAM_FUN_ENTRY_TRACE();
    UInt32 trans_type_bitmask=OAM_ZERO;
    /*SPR 20430 Changes Start*/
#if defined(__x86_64__)    
    ULong32 temp_tcb = trans_id;
#else
    UInt32 temp_tcb = trans_id;
#endif
    /*SPR 20430 Changes End*/
/*OAM_REVIEW_CHANGES*/
    oam_struct_t * p_node = oam_tcb_get_detail(&temp_tcb);
/*OAM_REVIEW_CHANGES*/
    if (!p_node)
    {
        OAM_LOG(OAM, OAM_WARNING,
               "Pointer to oam_struct returned is NULL");
        OAM_FUN_EXIT_TRACE();
            return OAM_FAILURE;
    }
    if(trans_type == OAM_TRANS_SON)
    {
        trans_type_bitmask = TCB_BITMASK_SON;
    }
/* CSR No 00055803 Start */
    /*SPR_10579 Start*/ 
    else if (trans_type == OAM_TRANS_RRC_MAC_PDCP_RRM)
    {
    trans_type_bitmask = TCB_BITMASK_RRC_MAC_PDCP_RRM;
    }
    /*SPR_10579 End*/ 
/* CSR No 00055803 End */
    else if(trans_type == OAM_TRANS_RRM)
    {
    trans_type_bitmask = TCB_BITMASK_RRM;
    }
    else if(trans_type == OAM_TRANS_MAC)
    {
    trans_type_bitmask = TCB_BITMASK_MAC;
    }
    else if(trans_type == OAM_TRANS_RLC)
    {
    trans_type_bitmask = TCB_BITMASK_RLC;
    }
    else if(trans_type == OAM_TRANS_PDCP)
    {
    trans_type_bitmask = TCB_BITMASK_PDCP;
    }
    else if(trans_type == OAM_TRANS_EGTPU)
    {
    trans_type_bitmask = TCB_BITMASK_EGTPU;
    }
    else if(trans_type == OAM_TRANS_RRC)
    {
    trans_type_bitmask = TCB_BITMASK_RRC;
    }
    else if(trans_type == OAM_TRANS_S1AP)
    {
    trans_type_bitmask = TCB_BITMASK_S1AP;
    }
    else if(trans_type == OAM_TRANS_X2AP)
    {
    trans_type_bitmask = TCB_BITMASK_X2AP;
    }
    else if(trans_type == OAM_TRANS_SYSTEM_LOG)
    {
    trans_type_bitmask = TCB_BITMASK_SYS;
    }
#ifdef OAM_RADIO_HEAD_SUPPORT
    else if(trans_type == TCB_BITMASK_RRH)
    {
    trans_type_bitmask = TCB_BITMASK_RRH;
    }
#endif
    else
    {
        OAM_LOG(OAM, OAM_WARNING,
                    "INVALID TRANSACTION TYPE[%d]", trans_type);
        OAM_FUN_EXIT_TRACE();
 }

    if((p_node->bitmask & trans_type_bitmask)!= OAM_ZERO)
    {
          p_node->bitmask ^= trans_type_bitmask;
          *bitmask=p_node->bitmask;
          OAM_FUN_EXIT_TRACE();
          return OAM_SUCCESS;
    }
    else
    {
          OAM_LOG(OAM, OAM_DETAILED,
                    "Bitmask was not set for TRANSACTION TYPE[%d]",trans_type);
          OAM_FUN_EXIT_TRACE();
          return OAM_FAILURE;
    }
}

