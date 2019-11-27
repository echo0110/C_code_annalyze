
/*******************************************************************************
*
*  FILE NAME   : x2ap_utility.c
*
*  DESCRIPTION : This file contains the utility functions for the X2AP 
*                Resource status update state machine 
*                and the state machine Entry function definition. 
*
*  DATE 05-March-2012  NAME Deepak Sah
*                     REFERENCE  36.423_960(3GPP Spec) REASON Initial
*
*  Copyright 2010, Aricent Inc.
*
*******************************************************************************/

#include "x2ap_rsu_sm.h"
#include "x2ap_utility.h"

/*****************************************************************************
 * Function Name  : x2ap_pack_U32 
 * Inputs         : p_dest - pointer to destianation
 *                  p_src - pointer to source
 * Outputs        : p_dest
 * Returns        : None
 * Description    : this function packs a unsigned int 
 ********************************************************************************/
void
x2ap_pack_U32
(
 void    *p_dest,
 void    *p_src
 )
{
    /* Packing U32 */

    ((U8*)p_dest)[0]    = (U8)((*(U32 *)p_src) >> 24);
    ((U8*)p_dest)[1]    = (U8)((*(U32 *)p_src) >> 16);
    ((U8*)p_dest)[2]    = (U8)((*(U32 *)p_src) >> 8);
    ((U8*)p_dest)[3]    = (U8)((*(U32 *)p_src));

}

/*****************************************************************************
 * Function Name  : x2ap_unpack_U32
 * Inputs         : p_dest - pointer to destianation
 *                  p_src -  pointer to source
 * Outputs        : p_dest
 * Returns        : None
 * Description    : this function unpacks the unsigned int 
 ********************************************************************************/
void
x2ap_unpack_U32
(
 void    *p_dest,
 void    *p_src
 )
{
    /* Unpacking U32 */

    *(U32 *)p_dest  = 0;

    *(U32 *)p_dest  |= (U32)((U8*)p_src)[0] << 24;
    *(U32 *)p_dest  |= (U32)((U8*)p_src)[1] << 16;
    *(U32 *)p_dest  |= (U32)((U8*)p_src)[2] << 8;
    *(U32 *)p_dest  |= (U32)((U8*)p_src)[3];

}


/******************************************************************************
 * Function Name : x2ap_compose_asn_plmn_identity
 * Inputs        : p_x2ap_plmn_identity - pointer to OSOCTET
 *                 p_plmn_identity - pointer to plmn_identity_t
 * Outputs       : p_x2ap_plmn_identity
 * Returns       : None                
 * Description   : This function converts PLMN identiy from internal format to
  *                X2AP format according to TS 36.413.
 *******************************************************************************/
void x2ap_compose_asn_plmn_identity(
        OSOCTET            *p_x2ap_plmn_identity,
        plmn_identity_t    *p_plmn_identity)
{

    /* MCC should be present always */
    X2AP_ASSERT(PLMN_IDENTITY_MCC_PRESENCE_FLAG &
            p_plmn_identity->presence_bitmask);

    /* PLMN Identity 1st octet : Pack first 2 digits of MCC */
    p_x2ap_plmn_identity[0] =
        (OSOCTET)( (p_plmn_identity->mcc[0] & 0xF) |
                   ((p_plmn_identity->mcc[1] & 0xF) << 4));

    /* PLMN Identity 2nd octet: Pack 3rd digit of MCC */
    p_x2ap_plmn_identity[1] =
        (OSOCTET) (p_plmn_identity->mcc[2] & 0xF);

    if (2 == p_plmn_identity->mnc.count)
    {
        /* Add filler digit */
        p_x2ap_plmn_identity[1] |= 0xF0;

        /* PLMN Identity 3rd octet : 1st and 2nd digit of MNC */
        p_x2ap_plmn_identity[2] =
            (OSOCTET) ((p_plmn_identity->mnc.mnc[0] & 0xF) |
                       ((p_plmn_identity->mnc.mnc[1] & 0xF) << 4));
    }
    else
    {
        /* PLMN Identity 3rd octet : Pack 1st digit of MNC */
        p_x2ap_plmn_identity[1] =
            (OSOCTET)(p_x2ap_plmn_identity[1] |
                      ((p_plmn_identity->mnc.mnc[0] & 0xF) << 4));

        /* PLMN Identity 3rd octet : 2nd and 3rd digit of MNC */
        p_x2ap_plmn_identity[2] =
            (OSOCTET)  ((p_plmn_identity->mnc.mnc[1] & 0xF) |
                        ((p_plmn_identity->mnc.mnc[2] & 0xF) << 4));
    }
}

/*****************************************************************************
 * Function Name  : compare_rsu_enb_meas_id_search_tree
 * Inputs         : p_elem1        : Pointer to first element being compared
 *                  p_elem2        : Pointer to second element being compared
 * Outputs        : None
 * Returns        : 0 : if both elements are equal
 *                  1 :if first is greater than second.
 *                  2 if first is less than second.
 * Description    : This function is used by CSPL Tree types to compare
 *                  elements in a tree.This function compares data of type U32.
 ******************************************************************************/
 S32
compare_rsu_enb_meas_id_search_tree(
        const void * p_elem1,
        const void * p_elem2)
{  
    X2AP_UT_TRACE_ENTER();
    if (p_elem1 == PNULL || p_elem2 == PNULL)
    {
        X2AP_TRACE(X2AP_WARNING, "%s: passed null arg",__FUNCTION__);
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
     X2AP_UT_TRACE_EXIT();
}

/*****************************************************************************
* Function Name  : key_of_rsu_enb_meas_id_search_tree
* Inputs         : p_node            : Pointer to Tree node
* Outputs        : None
* Returns        : PNULL: Error
*                   Pointer to key element in this case enb_meas_id_search.
* Description    : This function returns pointer to key element in a search
*                  node.
******************************************************************************/

const void *
key_of_rsu_enb_meas_id_search_tree(
        const YTNODE * p_node)
{
    /* SPR_5966 */
    return (p_node) ? &(((const rsu_enb_meas_id_search_node_t *)
                p_node)->x2ap_rsu_ctx.self_meas_id) : PNULL;
    /* SPR_5966 */
}

/******************************************************************************
 * Function Name  : x2ap_rsu_get_meas_id
 * Inputs         : p_x2ap_gl_ctx  pointer to X2AP global context
 * Outputs        : None
 * Returns        : None 
 * Description    : This function is called to init the global pool for the
 *                  measurement ID for RSU.
 *******************************************************************************/
U32 x2ap_rsu_get_meas_id (x2ap_gb_context_t* p_x2ap_gb_ctx)
{

    rsu_enb_meas_id_search_node_t   *p_rsu_tree_node = X2AP_P_NULL;
    /*x2ap_rsu_context_t              *p_x2ap_rsu_ctx = X2AP_P_NULL;*/

    U16 meas_id = X2AP_NULL;
    U16 counter = 0;
    
    for (counter = 0; counter< X2AP_RSU_MEASUREMENT_ID_MAX; counter++)
    {
        meas_id = (p_x2ap_gb_ctx->global_x2ap_rsu_meas_id++)%(X2AP_RSU_MEASUREMENT_ID_MAX + 1);
        if( meas_id > 0 && meas_id < (X2AP_RSU_MEASUREMENT_ID_MAX + 1))
        {
            /*Fetching the context*/
            p_rsu_tree_node = (rsu_enb_meas_id_search_node_t*)x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),&(meas_id));
            if(X2AP_P_NULL == p_rsu_tree_node)
            {
                 return meas_id;
            }
        }
    }
    if (counter == X2AP_RSU_MEASUREMENT_ID_MAX)
    {
        X2AP_TRACE(X2AP_ERROR,"%s: Measurement ID Not allocatable\n",__FUNCTION__);
        return X2AP_RSU_MEASUREMENT_ID_INVALID;

    }
    return meas_id;
}

/******************************************************************************
 * Function Name : x2ap_parse_asn_plmn_identity
 * Inputs        : p_plmn_identity - pointer to plmn_identity_t
 *                 p_s1ap_plmn_identity - pointer to OSOCTET
 * Outputs       : p_plmn_identity
 * Returns       : None
 * Description   : This function converts PLMN identiy from S1AP format to internal
 *                 format.
 *******************************************************************************/
void x2ap_parse_asn_plmn_identity(
    plmn_identity_t    *p_plmn_identity,
    const OSOCTET      *p_s1ap_plmn_identity)
{

    /* MCC should be present always */
    p_plmn_identity->presence_bitmask |=
        PLMN_IDENTITY_MCC_PRESENCE_FLAG;

    /* PLMN Identity 1st octet : Pack first 2 digits of MCC */
    p_plmn_identity->mcc[0] = 
        p_s1ap_plmn_identity[0] & 0x0F;
    p_plmn_identity->mcc[1] =
        (p_s1ap_plmn_identity[0] & 0xF0) >> 4;

    /* PLMN Identity 2nd octet: Pack 3rd digit of MCC */
    p_plmn_identity->mcc[2] = 
        p_s1ap_plmn_identity[1] & 0x0F;

        /* if filler digit */
    if ((p_s1ap_plmn_identity[1] & 0xF0) == 0xF0)
    {
        p_plmn_identity->mnc.count = 2;

        /* PLMN Identity 3rd octet : 1st and 2nd digit of MNC */
        p_plmn_identity->mnc.mnc[0] = 
            p_s1ap_plmn_identity[2] & 0x0F;
        p_plmn_identity->mnc.mnc[1] = 
            (p_s1ap_plmn_identity[2] & 0xF0) >> 4;
    }
    else
    {
        /* PLMN Identity 3rd octet : Pack 1st digit of MNC */
        p_plmn_identity->mnc.count = 3;

        p_plmn_identity->mnc.mnc[0] = 
            (p_s1ap_plmn_identity[1] & 0xF0) >> 4;

        /* PLMN Identity 3rd octet : 2nd and 3rd digit of MNC */
        p_plmn_identity->mnc.mnc[1] = 
            p_s1ap_plmn_identity[2] & 0x0F;
        p_plmn_identity->mnc.mnc[2] = 
            (p_s1ap_plmn_identity[2] & 0xF0) >> 4;
    }
}

/******************************************************************************
 * Function Name : x2ap_validate_peer_cell_info_and_get_peer_enb_context 
 * Inputs        : p_x2ap_gb_ctx - pointer to X2AP global context
 *                 p_x2ap_rsu_start_req - pointer to x2ap_rrm_rsu_rrm_start_req_t
 *                 peer_enodeb_id - peer eNB id
 * Outputs       : None
 * Returns       : X2AP_SUCCESS or X2AP_FAILURE
 * DEscription   : This function validate the ecgi for the target cell of peer eNB, 
 *                 received in X2AP_RRM_RSU_START_REQ_RRM_ENB 
 *******************************************************************************/
x2ap_return_et
x2ap_validate_peer_cell_info_and_get_peer_enb_context
(
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    x2ap_rrm_rsu_rrm_start_req_t *p_x2ap_rsu_start_req,
    U8 *peer_enodeb_id
)
{

    U8 o_c, i_c = 0;
    U16 num_served_cells = 0;
    void* p_srvd_cell_info = X2AP_P_NULL;

    x2ap_return_et retVal = X2AP_FAILURE;

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(X2AP_P_NULL!=p_x2ap_gb_ctx);
    X2AP_ASSERT(X2AP_P_NULL!=p_x2ap_rsu_start_req);


    for(o_c = 0; o_c < MAX_PEER_ENB; o_c ++)
    {
        if(X2AP_TRUE == 
                p_x2ap_gb_ctx->x2ap_link_info[o_c].
                connection_status)
        {

            if( X2AP_P_NULL != (p_srvd_cell_info = p_x2ap_gb_ctx->x2ap_link_info[o_c].
                                             p_srvd_cell_info))
            {
                num_served_cells = p_x2ap_gb_ctx->
                    x2ap_link_info[o_c].num_served_cells;

                for(i_c = 0 ;i_c < num_served_cells && i_c < MAX_SERVED_CELLS; i_c ++)
                {
                    if (0 == 
                            /*Bug_12881_fix_start*/
                            X2AP_MEMCMP(p_x2ap_rsu_start_req->cell_info[0].cell_identity , (((x2ap_ecgi_t*)
                                        ((U8*)p_srvd_cell_info +(sizeof(x2ap_ecgi_t)* i_c)))->eutran_cell_id),
                                        HOME_ENB_ID_OCTET_SIZE))/*Need to update*/
                            /*Bug_12881_fix_stop*/
                    {
                        *peer_enodeb_id = o_c;
                        retVal  =  X2AP_SUCCESS;
                        break;
                    }
                }

                if(retVal  ==  X2AP_SUCCESS)
                {
                    break;
                }
            }
        }
    }
    if(MAX_PEER_ENB == o_c)
    {
        X2AP_TRACE(X2AP_WARNING,"No serving cell info present in peer eNodeB Context"
                "ECGI value received is invalid");
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;

}

