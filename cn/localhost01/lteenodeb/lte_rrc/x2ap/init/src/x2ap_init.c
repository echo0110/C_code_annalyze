/*******************************************************************************
*
*  FILE NAME   : x2ap_init.c
*
*  DESCRIPTION : This file contains the definition of functions which will be
*                used to Initialize, register and run the X2AP module.
*
*  DATE          NAME             REFERENCE                     REASON
* 7-July-2010  Vaibhav Singh    36.423_880(3GPP Spec)         Initial
*
*  Copyright 2009, Aricent Inc.
*
*******************************************************************************/

#include "x2ap_init.h"
#include "rrc_handlers.h"

#ifdef ENDC_ENABLED
/* MENB CHNAGES - START */
/*****************************************************************************
 * Function Name  : x2ap_key_of_gnb_id_search_tree
 * Inputs         : p_node - Pointer to tree node.
 * Outputs        : None
 * Returns        : X2AP_P_NULL: Error
 *                  Pointer to key element in this case gnb_id_address.
 * Description    : This function returns pointer to key element in a search
 *                  node.
 *****************************************************************************/
const void * x2ap_key_of_gnb_id_search_tree
(
    /* Pointer to Tree node */
    const YTNODE * p_node
)
{   
    return (p_node) ? &(((sctp_gb_gnb_id_search_node_t *) p_node)->gb_gnb_id) : 
                X2AP_P_NULL;
}

/*****************************************************************************
 * Function Name  : x2ap_compare_gnb_id_search_tree
 * Inputs         : p_elem1 - Pointer to first element being compared
 *                  p_elem2- Pointer to second element being compared
 * Outputs        : None
 * Returns        : 0 if both elements are equal.
 *                  1 if first is greater than second.
 *                 -1 if first is less than second.
 * Description    : This function is used by CSPL Tree types to compare
 *                  elements in a tree. This function compares data of type
 *                  U32.
 *****************************************************************************/
S32
x2ap_compare_gnb_id_search_tree
(
    const void *p_elem1,
    const void *p_elem2
)
{
    x2_gb_gnb_id_t     *p_enb_id_1 = (x2_gb_gnb_id_t*)p_elem1;
    x2_gb_gnb_id_t     *p_enb_id_2 = (x2_gb_gnb_id_t*)p_elem2;
    U8                 *p_eNB_id_1 = X2AP_P_NULL;
    U8                 *p_eNB_id_2 = X2AP_P_NULL;
    U32                comp_len = X2AP_NULL;
    U32                comp_len1 = X2AP_NULL;

    if ( ( X2AP_P_NULL == p_elem1 ) || ( X2AP_P_NULL == p_elem2 ) )
    {
        X2AP_TRACE(X2AP_WARNING, "%s: passed null arg",__FUNCTION__);
        return X2AP_ERROR_IN_COMPARE;
    }

    p_eNB_id_1 = &(p_enb_id_1->x2_en_gnb_id.x2_gnb_id[X2AP_NULL]);
    comp_len = EN_GNB_ID_SIZE;
 
    p_eNB_id_2 = &(p_enb_id_2->x2_en_gnb_id.x2_gnb_id[X2AP_NULL]);
    comp_len1 = EN_GNB_ID_SIZE; 

    if ( ( X2AP_NULL == X2AP_MEMCMP(&p_enb_id_1->plmn_identity.plmn_id[X2AP_NULL],
                    &p_enb_id_2->plmn_identity.plmn_id[X2AP_NULL],
                    MAX_PLMN_ID_BYTES)) &&
           (comp_len == comp_len1) )
    {
        if ( X2AP_NULL == X2AP_MEMCMP(p_eNB_id_1, p_eNB_id_2, comp_len) )
        {
            return X2AP_FIRST_IS_EQUAL_TO_SECOND;
        }
        else
        {
            return X2AP_FIRST_IS_LESS_THAN_SECOND;
        }
    }
    else if ( X2AP_NULL > X2AP_MEMCMP(&p_enb_id_1->plmn_identity.plmn_id[X2AP_NULL]
                , &p_enb_id_2->plmn_identity.plmn_id[X2AP_NULL],
                MAX_PLMN_ID_BYTES))
    {
        return X2AP_FIRST_IS_LESS_THAN_SECOND;
    }
    else 
    {
        return X2AP_FIRST_IS_GREATER_THAN_SECOND;
    }
}
/* MENB CHNAGES - END */
#endif

/*****************************************************************************
 * Function Name  : x2ap_key_of_enb_id_search_tree
 * Inputs         : p_node - Pointer to tree node.
 * Outputs        : None
 * Returns        : X2AP_P_NULL: Error
 *                  Pointer to key element in this case enb_id_address.
 * Description    : This function returns pointer to key element in a search
 *                  node.
 *****************************************************************************/
const void * x2ap_key_of_enb_id_search_tree
(
 /* Pointer to Tree node */
 const YTNODE * p_node
 )
{   
    return (p_node) ? &(((sctp_gb_enb_id_search_node_t *) p_node)->gb_enb_id) : 
                X2AP_P_NULL;
}


/*****************************************************************************
 * Function Name  : x2ap_compare_enb_id_search_tree
 * Inputs         : p_elem1 - Pointer to first element being compared
 *                  p_elem2- Pointer to second element being compared
 * Outputs        : None
 * Returns        : 0 if both elements are equal.
 *                  1 if first is greater than second.
 *                 -1 if first is less than second.
 * Description    : This function is used by CSPL Tree types to compare
 *                  elements in a tree. This function compares data of type
 *                  U32.
 *****************************************************************************/
S32 x2ap_compare_enb_id_search_tree
(
 /* Pointer to first element being compared */
 const void * p_elem1 ,

 /* Pointer to second element being compared */
 const void * p_elem2
 )
{
    x2_gb_enb_id_t     *p_enb_id_1 = (x2_gb_enb_id_t*)p_elem1;
    x2_gb_enb_id_t     *p_enb_id_2 = (x2_gb_enb_id_t*)p_elem2;
    U8               *p_eNB_id_1 = X2AP_P_NULL;
    U8               *p_eNB_id_2 = X2AP_P_NULL;
    U32 comp_len = X2AP_NULL;
    U32 comp_len1 = X2AP_NULL;
    if (p_elem1 == X2AP_P_NULL || p_elem2 == X2AP_P_NULL)
    {
        X2AP_TRACE(X2AP_WARNING, "%s: passed null arg",__FUNCTION__);
        return X2AP_ERROR_IN_COMPARE;
    }
    
    if (p_enb_id_1->enb_id.presence_bitmask & X2AP_OAM_MACRO_ENB_ID_PRESENT)
    {
        p_eNB_id_1 = &(p_enb_id_1->enb_id.macro_enb_id.eNB_id[0]);
        comp_len = MACRO_ENB_ID_OCTET_SIZE; 
    }
    else
    {
        p_eNB_id_1 = &(p_enb_id_1->enb_id.home_enb_id.eNB_id[0]);
        comp_len = HOME_ENB_ID_OCTET_SIZE; 
    }
    if (p_enb_id_2->enb_id.presence_bitmask & X2AP_OAM_MACRO_ENB_ID_PRESENT)
    {
        p_eNB_id_2 = &(p_enb_id_2->enb_id.macro_enb_id.eNB_id[0]);
        comp_len1 = MACRO_ENB_ID_OCTET_SIZE; 
    }
    else
    {
        p_eNB_id_2 = &(p_enb_id_2->enb_id.home_enb_id.eNB_id[0]);
        comp_len1 = HOME_ENB_ID_OCTET_SIZE; 
    }


    if  (( 0 == X2AP_MEMCMP(&p_enb_id_1->plmn_identity.plmn_id[0]
                    , &p_enb_id_2->plmn_identity.plmn_id[0],
                    MAX_PLMN_ID_BYTES))
            && (comp_len == comp_len1))
    {
        if(0 == X2AP_MEMCMP(p_eNB_id_1, p_eNB_id_2,comp_len))
        {
            return X2AP_FIRST_IS_EQUAL_TO_SECOND;
        }
        else
        {
            return X2AP_FIRST_IS_LESS_THAN_SECOND;
        }
    }
    else if  ( 0 > X2AP_MEMCMP(&p_enb_id_1->plmn_identity.plmn_id[0]
                    , &p_enb_id_2->plmn_identity.plmn_id[0],
                      MAX_PLMN_ID_BYTES))
    {
        return X2AP_FIRST_IS_LESS_THAN_SECOND;
    }
    else 
    {
        return X2AP_FIRST_IS_GREATER_THAN_SECOND;
    }
}
/*****************************************************************************
 * Function Name  : x2ap_key_of_ecgi_search_tree
 * Inputs         : p_node - Pointer to tree node.
 * Outputs        : None
 * Returns        : X2AP_P_NULL: Error
 *                  Pointer to key element in this case ecgi case.
 * Description    : This function returns pointer to key element in a search
 *                  node.
 *****************************************************************************/
const void * x2ap_key_of_ecgi_search_tree
(
 /* Pointer to Tree node */
 const YTNODE * p_node
 )
{   
    return (p_node) ? &(((x2ap_nbr_ecgi_search_node_t*) p_node)->ecgi) : 
                X2AP_P_NULL;
}

/*****************************************************************************
 * Function Name  : x2ap_compare_ecgi_search_tree
 * Inputs         : p_elem1-  Pointer to first element being compared
 *                  p_elem2 - Pointer to second element being compared
 * Outputs        : None
 * Returns        : 0 if both elements are equal.
 *                  1 if first is greater than second.
 *                 -1 if first is less than second.
 * Description    : This function is used by CSPL Tree types to compare
 *                  elements in a tree. This function compares data of type
 *                  U32.
 *****************************************************************************/
S32 x2ap_compare_ecgi_search_tree
(
 /* Pointer to first element being compared */
 const void * p_elem1 ,

 /* Pointer to second element being compared */
 const void * p_elem2
 )
{
    if (p_elem1 == X2AP_P_NULL || p_elem2 == X2AP_P_NULL)
    {
        X2AP_TRACE(X2AP_WARNING, "%s: passed null arg",__FUNCTION__);
        return X2AP_ERROR_IN_COMPARE;
    }
    
    if ( 0 == X2AP_MEMCMP(p_elem1, p_elem2, sizeof(x2ap_ecgi_t)))
    {
        return X2AP_FIRST_IS_EQUAL_TO_SECOND;
    }
    /*Bug_11608_fix_start*/
    else if (  0 > (X2AP_MEMCMP( p_elem1 , p_elem2,sizeof(x2ap_ecgi_t))))
    {
        return X2AP_FIRST_IS_LESS_THAN_SECOND;
    }
    else 
    {
        return X2AP_FIRST_IS_GREATER_THAN_SECOND;
    }
    /*Bug_11608_fix_stop*/
}

/*****************************************************************************
 * Function Name  : x2ap_key_of_nr_cgi_search_tree
 * Inputs         : p_node - Pointer to tree node.
 * Outputs        : None
 * Returns        : X2AP_P_NULL: Error
 *                  Pointer to key element in this case nr_cgi case.
 * Description    : This function returns pointer to key element in a search
 *                  node.
 *****************************************************************************/
const void * x2ap_key_of_nr_cgi_search_tree
(
 /* Pointer to Tree node */
 const YTNODE * p_node
 )
{   
    return (p_node) ? &(((x2ap_nbr_nr_cgi_search_node_t*) p_node)->nr_cgi) : 
                X2AP_P_NULL;
}

/*****************************************************************************
 * Function Name  : x2ap_compare_nr_cgi_search_tree
 * Inputs         : p_elem1-  Pointer to first element being compared
 *                  p_elem2 - Pointer to second element being compared
 * Outputs        : None
 * Returns        : 0 if both elements are equal.
 *                  1 if first is greater than second.
 *                 -1 if first is less than second.
 * Description    : This function is used by CSPL Tree types to compare
 *                  elements in a tree. This function compares data of type
 *                  U32.
 *****************************************************************************/
S32 x2ap_compare_nr_cgi_search_tree
(
 /* Pointer to first element being compared */
 const void * p_elem1 ,

 /* Pointer to second element being compared */
 const void * p_elem2
 )
{
    if (p_elem1 == X2AP_P_NULL || p_elem2 == X2AP_P_NULL)
    {
        X2AP_TRACE(X2AP_WARNING, "%s: passed null arg",__FUNCTION__);
        return X2AP_ERROR_IN_COMPARE;
    }
    
    if ( 0 == X2AP_MEMCMP(p_elem1, p_elem2, sizeof(x2ap_nr_ecgi_t)))
    {
        return X2AP_FIRST_IS_EQUAL_TO_SECOND;
    }
    /*Bug_11608_fix_start*/
    else if (  0 > (X2AP_MEMCMP( p_elem1 , p_elem2,sizeof(x2ap_nr_ecgi_t))))
    {
        return X2AP_FIRST_IS_LESS_THAN_SECOND;
    }
    else 
    {
        return X2AP_FIRST_IS_GREATER_THAN_SECOND;
    }
    /*Bug_11608_fix_stop*/
}

/*****************************************************************************
 * Function Name  : x2ap_key_of_sd_search_tree
 * Inputs         : p_node - Pointer to tree node.
 * Outputs        : None
 * Returns        : X2AP_P_NULL: Error
 *                  Pointer to key element in this case sctp_sd.
 * Description    : This function returns pointer to key element in a search
 *                  node.
 *****************************************************************************/
const void * x2ap_key_of_sd_search_tree
(
 /* Pointer to Tree node */
 const YTNODE * p_node
 )
{
    return (p_node) ? &(((x2_sctp_sd_search_node_t *) p_node)->sctp_sd) : X2AP_P_NULL;
}


/*****************************************************************************
 * Function Name  : x2ap_compare_sd_search_tree
 * Inputs         : p_elem1 -  pointer to 1st the elements to compare
 *                  p_elem2 - pointer to 2nd the elements to compare
 * Outputs        : None
 * Returns        : 0 if both elements are equal.
 *                  1 if first is greater than second.
 *                 -1 if first is less than second.
 * Description    : This function is used by CSPL Tree types to compare
 *                  elements in a tree. This function compares data of type
 *                  U32.
 *****************************************************************************/
S32 x2ap_compare_sd_search_tree
(
 /* Pointer to first element being compared */
 const void * p_elem1 ,

 /* Pointer to second element being compared */
 const void * p_elem2
 )
{
    if (p_elem1 == X2AP_P_NULL || p_elem2 == X2AP_P_NULL)
    {
        X2AP_TRACE(X2AP_WARNING, "%s: passed null arg",__FUNCTION__);
        return X2AP_ERROR_IN_COMPARE;
    }

    if ( * ((U32 *) p_elem1) == * ((U32 *) p_elem2))
    {
        return X2AP_FIRST_IS_EQUAL_TO_SECOND;
    }
    else if ( * ((U32 *) p_elem1) < * ((U32 *) p_elem2))
    {
        return X2AP_FIRST_IS_LESS_THAN_SECOND;
    }
    else 
    {
        return X2AP_FIRST_IS_GREATER_THAN_SECOND;
    }
}
/*****************************************************************************
 * Function Name  : x2ap_key_of_ip_search_tree
 * Inputs         : p_node - Pointer to tree node.
 * Outputs        : None
 * Returns        : X2AP_P_NULL: Error
 *                  Pointer to key element in this case enb_ip_address.
 * Description    : This function returns pointer to key element in a search
 *                  node.
 *****************************************************************************/
const void * x2ap_key_of_ip_search_tree
(
 /* Pointer to Tree node */
 const YTNODE * p_node
 )
{   
    return (p_node) ? (((sctp_ip_search_node_t *) p_node)->enb_ip_address) : X2AP_P_NULL;
}
/*spr8341 IPv6 fix start*/  
/*****************************************************************************
 * Function Name  : x2ap_key_of_ipv6_search_tree
 * Inputs         : p_node - Pointer to tree node.
 * Outputs        : None
 * Returns        : X2AP_P_NULL: Error
 *                  Pointer to key element in this case enb_ipv6_address.
 * Description    : This function returns pointer to key element in a search
 *                  node.
 *****************************************************************************/
const void * x2ap_key_of_ipv6_search_tree
(
 /* Pointer to Tree node */
 const YTNODE * p_node
 )
{   
    return (p_node) ? (((sctp_ip_search_node_t *) p_node)->enb_ipv6_address) : X2AP_P_NULL; 
}
/*spr8341 IPv6 fix end*/
/*****************************************************************************
 * Function Name  : x2ap_compare_enb_id_search_tree
 * Inputs         : p_elem1 - pointer to 1st element being compared
 *                  p_elem2 - pointer to 2st element being compared
 * Outputs        : None
 * Returns        : 0 if both elements are equal.
 *                  1 if first is greater than second.
 *                 -1 if first is less than second.
 * Description    : This function is used by CSPL Tree types to compare
 *                  elements in a tree. This function compares data of type
 *                  U32.
 *****************************************************************************/
S32 x2ap_compare_ip_search_tree
(
 /* Pointer to first element being compared */
 const void * p_elem1 ,

 /* Pointer to second element being compared */
 const void * p_elem2
 )
{
    if (p_elem1 == X2AP_P_NULL || p_elem2 == X2AP_P_NULL)
    {
        X2AP_TRACE(X2AP_WARNING, "%s: passed null arg",__FUNCTION__);
        return X2AP_ERROR_IN_COMPARE;
    }

    if ( 0 == (strcmp_wrapper( ((const S8 *) p_elem1) , ((const S8 *)p_elem2))))
    {
        return X2AP_FIRST_IS_EQUAL_TO_SECOND;
    }
    else if (  0 > (strcmp_wrapper( ((const S8 *) p_elem1) , ((const S8 *)p_elem2))))
    {
        return X2AP_FIRST_IS_LESS_THAN_SECOND;
    }
    else 
    {
        return X2AP_FIRST_IS_GREATER_THAN_SECOND;
    }
}

/*****************************************************************************
 * Function Name  : x2ap_init
 * Inputs         : p_x2ap_init_data - Pointer to X2AP stack initialization
 *                  data. Unused
 * Outputs        : None
 * Returns        : Buffer that will be passed in x2ap_process_message
 *                  and in x2ap_process_timer_msg callbacks
 * Description    : This function is CSPL callback. It initialize X2AP entity.
 *****************************************************************************/
void x2ap_init_int (x2ap_gb_context_t* p_x2ap_gl_ctx)
{
    X2AP_UT_TRACE_ENTER();

    /* Init search tree for SD Search tree */
    x2ap_search_tree_init( &(p_x2ap_gl_ctx->enb_ctxt_trees.sd_search_tree),
            &x2ap_compare_sd_search_tree,
            x2ap_key_of_sd_search_tree);

    /* Init search tree for ip search tree :*/
    x2ap_search_tree_init( &(p_x2ap_gl_ctx->enb_ctxt_trees.ip_search_tree),
            &x2ap_compare_ip_search_tree,
            x2ap_key_of_ip_search_tree);
    /* Init search tree for ip search tree :*/
    x2ap_search_tree_init( &(p_x2ap_gl_ctx->enb_ctxt_trees.enb_id_search_tree),
            &x2ap_compare_enb_id_search_tree,
            x2ap_key_of_enb_id_search_tree);

    /* Init search tree for ip search tree :*/
    x2ap_search_tree_init( &(p_x2ap_gl_ctx->nbr_ecgi_search_tree),
            &x2ap_compare_ecgi_search_tree,
            x2ap_key_of_ecgi_search_tree);
    /*spr8341 IPv6 fix start*/
    x2ap_search_tree_init( &(p_x2ap_gl_ctx->enb_ctxt_trees.ipv6_search_tree),
            &x2ap_compare_ip_search_tree,
            x2ap_key_of_ipv6_search_tree);
    /*spr8341 IPv6 fix end*/

#ifdef ENDC_ENABLED
    /* MENB CHNAGES - START */
    /* Init search tree for ip search tree :*/
    x2ap_search_tree_init( &(p_x2ap_gl_ctx->enb_ctxt_trees.gnb_id_search_tree),
            &x2ap_compare_gnb_id_search_tree,
            x2ap_key_of_gnb_id_search_tree);

    /* Init search tree for ip search tree :*/
    x2ap_search_tree_init( &(p_x2ap_gl_ctx->nbr_nr_cgi_search_tree),
            &x2ap_compare_nr_cgi_search_tree,
            x2ap_key_of_nr_cgi_search_tree);        
    /* MENB CHNAGES - END */
#endif

    X2AP_UT_TRACE_EXIT();
}
/*****************************************************************************
 * Function Name  : x2ap_init
 * Inputs         : p_x2ap_init_data - Pointer to X2AP stack initialization
 *                  data. Unused
 * Outputs        : None
 * Returns        : None
 * Description    : This function is CSPL callback. It initialize X2AP entity.
 *****************************************************************************/
void * x2ap_init (void * p_x2ap_init_data)
{
	x2ap_gb_context_t *p_x2ap_gl_ctx = X2AP_P_NULL;
	comm_info_init_param_t *comm_info_init_param = PNULL;
    x2ap_timer_buffer_t timer_buf;

    X2AP_UT_TRACE_ENTER();
    X2AP_TRACE(X2AP_DETAILED, "x2ap Init.");

    comm_info_init_param = (comm_info_init_param_t *)p_x2ap_init_data;

    /* Allocate Memory for X2AP global context data structure */
    p_x2ap_gl_ctx = (x2ap_gb_context_t *)x2ap_mem_get(sizeof(x2ap_gb_context_t));

    if(X2AP_NULL == p_x2ap_gl_ctx)
    {
        X2AP_TRACE(X2AP_FATAL, "x2ap_init: unable to Allocate Memory for "\
                "X2AP global context!");
        return X2AP_NULL;
    }
    else
    {
        /* Init x2ap DB */
        X2AP_MEMSET((void*)p_x2ap_gl_ctx, X2AP_NULL, sizeof(x2ap_gb_context_t));
        p_x2ap_gl_ctx->p_comm_info_int = (comm_info_init_param_t *)comm_info_init_param;

         /* SPR 17842 Fix Start */
        #if (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
        /* Initialise logging parameters to default values */
        p_x2ap_gl_ctx->log_level    = RRC_BRIEF;
        /* Coverity Fix 97033 Start */
        p_x2ap_gl_ctx->log_enabled  = X2AP_TRUE;
        /* Coverity Fix 97033 End */
        #endif
        /* SPR 17842 Fix Stop */

        /*Timer for X2AP init indication to OAM */
        timer_buf.timer_type = X2AP_OAM_INIT_TIMER;
        timer_buf.auto_delete = X2AP_FALSE;
        p_x2ap_gl_ctx->x2ap_init_sm_blk.oam_init_ind_tim_id = X2AP_NULL;

        p_x2ap_gl_ctx->x2ap_init_sm_blk.x2ap_current_state = X2AP_INIT_ST;
        /* Init x2ap DB */
        x2ap_init_int(p_x2ap_gl_ctx);

        /* Send X2AP INIT to OAM */
        x2ap_process_init_timer();

        /* Start repeated timer for sending X2AP_OAM_INIT_IND */
        if(PNULL != (p_x2ap_gl_ctx->x2ap_init_sm_blk.oam_init_ind_tim_id =
                    x2ap_start_timer(X2AP_TIMER_OAM_INIT_IND_DEFAULT_DURATION,
                        &timer_buf,
                        sizeof(timer_buf),
                        X2AP_TRUE)))
        {
            /* set the state to Wait for init cnf from OAM */
            p_x2ap_gl_ctx->x2ap_init_sm_blk.x2ap_current_state = 
                                        X2AP_WF_PROVISIONING_REQ_ST;
        }
    }

    X2AP_UT_TRACE_EXIT();

    return p_x2ap_gl_ctx;
 
}

