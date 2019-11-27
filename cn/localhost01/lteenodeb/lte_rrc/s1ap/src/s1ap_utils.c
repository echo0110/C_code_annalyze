/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 *  File Details
 *  ------------
 *  $Id: s1ap_utils.c,v 1.5 2010/12/02 12:39:49 gur21897 Exp $
 ****************************************************************************
 *
 *  File Description :
 *  This file has the utility functions for s1ap module.
 *
 ****************************************************************************
 *
 *  Revision Details
 *  ----------------
 *  $Log: s1ap_utils.c,v $
 *  Revision 1.5  2010/12/02 12:39:49  gur21897
 *  Merging With 2.0.1 Support Branch
 *
 *  Revision 1.4  2010/10/13 07:04:26  gur18569
 *  Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 *  Revision 1.3.4.1  2010/06/24 13:01:53  gur22047
 *  internal source and s1ap code
 *
 *  Revision 1.3  2010/03/24 09:50:45  gur18569
 *  Merged files from Rel 1.0 support branch
 *
 *  Revision 1.2  2010/01/04 16:10:05  ukr15916
 *  no message
 *
 *  Revision 1.1.2.11  2009/12/28 10:14:30  gur21006
 *  system calls removed
 *
 *  Revision 1.1.2.10  2009/12/28 05:09:42  gur18569
 *  fixed indentation
 *
 *  Revision 1.1.2.9  2009/12/27 08:39:05  gur18569
 *  removed tabs
 *
 *  Revision 1.1.2.8  2009/12/26 12:05:01  gur21006
 *  Warnings removed
 *
 *  Revision 1.1.2.7  2009/12/23 06:27:46  gur21006
 *  Warnings Removed
 *
 *  Revision 1.1.2.6  2009/12/03 10:28:48  ukr18877
 *  function s1ap_set_sd_from_header moved from s1ap_utils to sctp_wrappers
 *
 *  Revision 1.1.2.5  2009/12/03 09:24:08  ukr18877
 *  Points 5, 10 and 3MM (CSC) from mail "Questions about S1AP" fixed. Parameter RUNNER_RRC_UECC_FT set to 0 (for SSIT testing), OAM connect and cleanup SSIT tests added
 *
 *  Revision 1.1.2.4  2009/12/01 15:07:24  gur18569
 *  integration changes
 *
 *  Revision 1.1.2.3  2009/11/29 09:19:26  gur18569
 *  added s1ap_set_sctp_sd
 *
 *  Revision 1.1.2.2  2009/11/26 18:33:49  ukr16018
 *  Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 *  Revision 1.1.2.1  2009/11/25 13:11:05  gur18569
 *  Shifted to level of rrc dir
 *
 *  Revision 1.1.2.3  2009/11/20 08:41:43  gur18569
 *  changes in get ue id and get cell id
 *
 *  Revision 1.1.2.2  2009/11/17 13:47:00  gur20470
 *  Incorporated review comments
 *
 *  Revision 1.1.2.1  2009/11/09 10:36:06  gur18569
 *  intial version
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/



/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "s1ap_utils.h"
#include "s1ap_asn_enc_dec_3gpp.h"
#include "rrc_common_md.h"
/****************************************************************************
  Private Definitions
 ****************************************************************************/

/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static U32 s1ap_get_sd_from_header(
        U8 *p_header);

/*REL 1.2:HANDOVER PROCEDURE START*/
static U16 s1ap_get_stream_id_from_header(
        U8 *p_header);
/*REL 1.2:HANDOVER PROCEDURE END*/


/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/


/******************************************************************************
 * Function Name  : s1ap_get_sd_from_header
 * Inputs         : p_header - ptr to the message buffer
 * Outputs        : None
 * Returns        : socket_sd
 * Description    : This function processes the consequent four bytes to 
 *                  return the sd value.
 ******************************************************************************/

static U32 s1ap_get_sd_from_header(
        U8 *p_header 
        )
{
    U32 value = RRC_NULL;
    RRC_ASSERT(p_header != PNULL);
    value = (U32)(p_header[0] << 24 | p_header[1] << 16 | p_header[2] << 8 | 
        p_header[3]);
    return value;
}

/*REL 1.2:HANDOVER PROCEDURE START*/
/******************************************************************************
 * Function Name  : s1ap_get_stream_id_from_header
 * Inputs         : p_header - ptr to the message buffer
 * Outputs        : None 
 * Returns        : stream_id
 * Description    : This function processes the consequent two bytes to 
 *                  return the stream_id value.
 ******************************************************************************/

static U16 s1ap_get_stream_id_from_header(
        U8 *p_header 
        )
{
    U16 value = RRC_NULL;
    RRC_ASSERT(p_header != PNULL);
    value = (U16)(p_header[0] << 8 | p_header[1]);
    return value;
}
/*REL 1.2:HANDOVER PROCEDURE END*/

/*****************************************************************************
 * Function Name  : s1ap_alloc_intrl_msg
 * Inputs         : dst_module_id - Destination module identifier
 *                  api_id - API id
 *                  msg_size - Size of message (payload)
 * Outputs        : None 
 * Returns        : None
 * Description    : Send message only with RRC API Header from S1AP.
 *                  Is used for sending messages to other local RRC 
 *                   submodules
 ****************************************************************************/
void* s1ap_alloc_intrl_msg(
        rrc_module_id_t     dst_module_id,  
        U16                 api_id,         
        U16                 msg_size)
{
    void *p_api = PNULL;
    U16 msg_api_length = (U16)(msg_size + RRC_API_HEADER_SIZE);

    RRC_S1AP_UT_TRACE_ENTER();

    /* Allocate buffer */
    p_api = rrc_msg_mem_get(msg_api_length);

    if (PNULL != p_api)
    {
        memset_wrapper(
                p_api,
                0,
                msg_api_length);

        /* Fill RRC header */
        rrc_construct_api_header(
                (U8*)p_api,
                RRC_VERSION_ID,
                RRC_S1AP_MODULE_ID,
                dst_module_id,
                api_id,
                msg_api_length);
    }

    RRC_S1AP_UT_TRACE_EXIT();
    return p_api;
}

/*****************************************************************************
 * Function Name  : s1ap_get_sctp_sd
 * Inputs         : p_header - ptr to the message buffer
 * Outputs        : None 
 * Returns        : socket_sd
 * Description    : This function calls the internal function that processes
 *                  the consequent four bytes to return the sd value.
 ****************************************************************************/
U32 s1ap_get_sctp_sd(
        U32 *p_header 
        )
{
    return s1ap_get_sd_from_header(&(((U8 *)p_header)[0]));
}

/*REL 1.2:HANDOVER PROCEDURE START*/
/*****************************************************************************
 * Function Name  : s1ap_get_stream_id
 * Inputs         : p_header - ptr to the message buffer
 * Outputs        : None 
 * Returns        : stream_id
 * Description    : This function calls the internal function that processes
 *                  the consequent two bytes to return the stream_id value.
 ****************************************************************************/
U16 s1ap_get_stream_id(
        U16 *p_header 
        )
{
    return s1ap_get_stream_id_from_header(&(((U8 *)p_header)[0]));
}
/*REL 1.2:HANDOVER PROCEDURE END*/

/*****************************************************************************
 * Function Name  : s1ap_get_cell_id
 * Inputs         : p_header - ptr to the message buffer
 * Outputs        : None 
 * Returns        : socket_sd
 * Description    : This function calls the internal function that processes
 *                  the consequent bytes to return the cell id.
 ****************************************************************************/
U8 s1ap_get_cell_id
(
    s1ap_gb_context_t    *p_s1ap_gb_context,
    U32 p_header 
)
{
    /* 1. Get UECC module Id from eNodeUeS1apId */
    /* 2. Get UE module Id from eNodeUeS1apId */
    /* 3. Get Cell Index from Ue Context stored in Global Context */
   

/*BUG 604 changes start*/
    if ((PNULL != p_s1ap_gb_context) && (PNULL != 
                p_s1ap_gb_context->p_p_ue_s1ap_contexts))
    {
        if (PNULL != (p_s1ap_gb_context->p_p_ue_s1ap_contexts
                    [p_header]))
        {

            return ((p_s1ap_gb_context->p_p_ue_s1ap_contexts
                        [p_header])->cell_index);
        }

    }
/*BUG 604 changes stop*/
    return RRC_INVALID_CELL_INDEX;
}


/*****************************************************************************
 * Function Name  : s1ap_get_ue_id
 * Inputs         : p_header - ptr to the message buffer
 * Outputs        : None 
 * Returns        : socket_sd
 * Description    : This function calls the internal function that processes
 *                  the consequent two bytes to return the ue id.
 ****************************************************************************/
U16 s1ap_get_ue_id(
        U32 p_header 
        )
{
    return ((U16)((0x1fff)&(p_header)));
}

/*****************************************************************************
 * Function Name  : s1ap_enb_mgr_get_next_transaction_id 
 * Inputs         : p_s1ap_gb_ctx - pointer to S1AP global data
 * Outputs        : None
 * Returns        : rrc_transaction_id_t.
 * Description    : Get next s1ap enb mgr Transaction id value.
 ********************************************************************************/
rrc_transaction_id_t s1ap_enb_mgr_get_next_transaction_id(
    s1ap_gb_context_t     *p_s1ap_gb_ctx
)
{
    return ++p_s1ap_gb_ctx->trans_id;
}

/*****************************************************************************
 * Function Name  : get_ue_ctx_from_ue_id 
 * Inputs         : p_s1ap_gb_ctx - pointer to S1AP global data
 *                  pp_s1ap_ue_context - pointer to UE context
 *                  ue_index - UE index 
 * Outputs        : None
 * Returns        : None
 * Description    : Get Ue Context from Global context based on ue Index.
 ********************************************************************************/
void get_ue_ctx_from_ue_id(
    s1ap_gb_context_t     *p_s1ap_gb_ctx,
    ue_s1ap_context_t     **pp_s1ap_ue_context,
/*BUG 604 changes start*/
    U32                   ue_index /* enbS1apId */
/*BUG 604 changes stop*/
)
{
    RRC_ASSERT(p_s1ap_gb_ctx != PNULL);

/*BUG 604 changes start*/
/*    if (RRC_GET_UECC_MODULE_INDEX(ue_index) >=
            p_s1ap_gb_ctx->no_of_uecc_instances)
    {
        return;
    }*/

    if (PNULL != p_s1ap_gb_ctx->p_p_ue_s1ap_contexts)
    {
        *pp_s1ap_ue_context = (p_s1ap_gb_ctx->
                p_p_ue_s1ap_contexts[ue_index]);

    }
/*BUG 604 changes stop*/
}

/* SPR 22789 Fix Start */
/*****************************************************************************
 * Function Name  : set_ue_ctx_from_ue_id
 * Inputs         : p_s1ap_gb_ctx - pointer to S1AP global data
 *                  p_s1ap_ue_context - pointter to UE context 
 *                  ue_index - UE index
 * Outputs        : None
 * Returns        : None
 * Description    : Set Ue Context in Global context based on ue Index.
 * Note: mme_id should be assigned to p_s1ap_ue_context before calling this function
 ********************************************************************************/
 /* SPR 22789 Fix End */
void set_ue_ctx_from_ue_id(
    s1ap_gb_context_t     *p_s1ap_gb_ctx,
    ue_s1ap_context_t     *p_s1ap_ue_context,
/*BUG 604 changes start*/
    U32                   ue_index /* ue_index */
/*BUG 604 changes stop*/
)
{
    /*SPR_21818_START*/
    ue_s1ap_context_t     *p_s1ap_ue_existing_context = PNULL;
    /*SPR_21818_END*/
    RRC_ASSERT(p_s1ap_gb_ctx != PNULL);

/*BUG 604 changes start*/
    if (PNULL != p_s1ap_gb_ctx->p_p_ue_s1ap_contexts)
    {
        /*SPR_21818_START*/
        /* SPR 22789 Fix Start */
        if(PNULL != p_s1ap_ue_context)
        {
            /* Add context */
            p_s1ap_gb_ctx->mme_contexts[p_s1ap_ue_context->mme_id]->ue_connected ++;          
            RRC_S1AP_TRACE(RRC_INFO,"S1AP context for [S1AP_UEID:%u] [MME_UEID:%u]"
                    " allocated for [UE:%u] [is_mme_ue_s1ap_id_valid:%u] on MME %u",
                    p_s1ap_ue_context->enb_ue_s1ap_id,
                    p_s1ap_ue_context->mme_ue_s1ap_id,
                    p_s1ap_ue_context->ue_index,
                    p_s1ap_ue_context->is_mme_ue_s1ap_id_valid,
                    p_s1ap_ue_context->mme_id);
        }
        else
        {
            /* Delete context */
            p_s1ap_ue_existing_context =
                p_s1ap_gb_ctx->p_p_ue_s1ap_contexts[ue_index];
            if (PNULL != p_s1ap_ue_existing_context)
            {
                /* UE count decrement */
                if (RRC_NULL < p_s1ap_gb_ctx->mme_contexts[p_s1ap_ue_existing_context->mme_id]->ue_connected)
                {
                    p_s1ap_gb_ctx->mme_contexts[p_s1ap_ue_existing_context->mme_id]->ue_connected--;          
                }
                RRC_S1AP_TRACE(RRC_INFO,"S1AP context for [S1AP_UEID:%u] with [MME_UEID:%u]"
                    " de-allocated for [UE:%u] [is_mme_ue_s1ap_id_valid:%u] from MME %u",
                    p_s1ap_ue_existing_context->enb_ue_s1ap_id,
                    p_s1ap_ue_existing_context->mme_ue_s1ap_id,
                    p_s1ap_ue_existing_context->ue_index,
                    p_s1ap_ue_existing_context->is_mme_ue_s1ap_id_valid,
                    p_s1ap_ue_existing_context->mme_id);
            }
        }
        /* SPR 22789 Fix End */
        /*SPR_21818_END*/
        (p_s1ap_gb_ctx->p_p_ue_s1ap_contexts[ue_index]) = p_s1ap_ue_context;
    }
/*BUG 604 changes stop*/
}

/*BUG 604 changes start*/
/******************************************************************************
*   FUNCTION NAME: s1ap_ueid_allocate
*
*   DESCRIPTION:
*       This function allocates enb_s1ap_ueid .
*
*   RETURNS: S1AP_UEID or INVALID_S1AP_UE_ID case of error.
*
******************************************************************************/
U32 s1ap_ueid_allocate(
    s1ap_gb_context_t     *p_s1ap_gb_ctx  /* S1AP global context */
)
{

	RRC_ASSERT( PNULL != p_s1ap_gb_ctx );

	U32   s1ap_ueid = p_s1ap_gb_ctx->total_ue_supported;

	if (p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_count > 0)
	{
		/*Get unused S1AP_UEID*/
		s1ap_ueid = p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids[
			p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_cur];

		p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_count--;
		p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_cur++;
		if (p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_cur >=
				p_s1ap_gb_ctx->total_ue_supported)
		{
			p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_cur = 0;
		}

        /*SPR_21818_START*/
        RRC_S1AP_TRACE(RRC_INFO,"[S1AP_UEID:%u] allocated."
                " [not_allocated_ue_ids_cur:%u] and [not_allocated_ue_ids_count:%u]",
                s1ap_ueid, 
                p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_cur,
                p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_count);
        /*SPR_21818_END*/
	}
	return s1ap_ueid;
}

/******************************************************************************
*   FUNCTION NAME: s1ap_ueid_free
*
*   DESCRIPTION:
*       This function frees S1AP_UEID.
*
*   RETURNS: none.
*
******************************************************************************/
void s1ap_ueid_free(
    s1ap_gb_context_t*  p_s1ap_gb_ctx,  /* S1AP global context */
    U32    s1ap_ueid                        /* S1AP_UEID  */
)
{
	RRC_ASSERT( PNULL != p_s1ap_gb_ctx );

	/* SPR 16149 Fix Start */
	if (s1ap_ueid  < p_s1ap_gb_ctx->total_ue_supported)
	{    
		RRC_S1AP_TRACE(RRC_WARNING,"[S1AP_UEID:%u] released",s1ap_ueid);

		p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids[
			(p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_cur +
			 p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_count) %
			p_s1ap_gb_ctx->total_ue_supported] = s1ap_ueid;

    /*SPR_21818_START*/
    if(p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_count
            < p_s1ap_gb_ctx->total_ue_supported)
    {
		p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_count++;
	}
    else
    {
        RRC_S1AP_TRACE(RRC_INFO,"[not_allocated_ue_ids_count:%u] already equals "
            "to [total_ue_supported:%u] and still received UE free."
            " Not incrementing not_allocated_ue_ids_count.",
            p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_count,
            p_s1ap_gb_ctx->total_ue_supported);
    }

    RRC_S1AP_TRACE(RRC_INFO,"[S1AP_UEID:%u] released."
            " [not_allocated_ue_ids_cur:%u] and [not_allocated_ue_ids_count:%u]",
            s1ap_ueid, 
            p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_cur,
            p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_count);
    /*SPR_21818_END*/
    }
}
/******************************************************************************
 *   FUNCTION NAME: s1ap_ueid_ctx_init
 *
 *   INPUT:	   s1ap_gb_context_t*  p_s1ap_gb_ctx
 *   
 *   OUTPUT:        None
 *
 *   RETURNS:       RRC_SUCCESS or RRC_FAILURE
 *
 *   DESCRIPTION:   This function initializes S1AP context.
 *
 ******************************************************************************/
rrc_return_t s1ap_ueid_ctx_init(
		s1ap_gb_context_t*  p_s1ap_gb_ctx  /* S1AP global context */
		)
{
	rrc_return_t result = RRC_FAILURE;
	U32*         p_not_allocated_ue_ids = PNULL; /*S1AP UE IDs ready to allocate*/
	U32 s1ap_ueid = RRC_NULL;

    /*SPR_21818_START*/
    if(PNULL != p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids)
    {
        rrc_mem_free(p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids);
        p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids = PNULL;
        RRC_S1AP_TRACE(RRC_INFO,"Already allocated memory"
            " p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids released.");
    }
    /*SPR_21818_END*/

	p_not_allocated_ue_ids = (U32*)rrc_mem_get(sizeof(U32)*
			(p_s1ap_gb_ctx->total_ue_supported));
	if (PNULL == p_not_allocated_ue_ids)
	{
		RRC_S1AP_TRACE(RRC_WARNING,"Memory allocation failed (not_allocated_s1ap_ue_ids )");
	}
	else
	{
		memset_wrapper(p_not_allocated_ue_ids, 0,
				sizeof(U32)*(p_s1ap_gb_ctx->total_ue_supported) );

		for (s1ap_ueid = 0; s1ap_ueid < (p_s1ap_gb_ctx->
					total_ue_supported); s1ap_ueid++)
		{
			p_not_allocated_ue_ids[s1ap_ueid] = s1ap_ueid;
		}
		p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_cur = 0;
		p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_count =
			p_s1ap_gb_ctx->total_ue_supported;
		p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids = p_not_allocated_ue_ids;

    /*SPR_21818_START*/
        RRC_S1AP_TRACE(RRC_INFO,"s1ap_ueid_ctx Initialized."
                " [not_allocated_ue_ids_cur:%u] and"
                " [not_allocated_ue_ids_count:%u]",
                p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_cur,
                p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_count);

    /*SPR_21818_END*/

		result=RRC_SUCCESS;
	}

	return result;
}
/*BUG 604 changes stop*/
/*bugid 117989 begin*/
rrc_return_t s1ap_check_cell_index(
		rrc_cell_index_t   cell_index,
		s1ap_gb_context_t*  p_s1ap_gb_ctx  /* S1AP global context */
		)
{
	rrc_return_t result = RRC_FAILURE;

	if (PNULL != p_s1ap_gb_ctx->cell_s1ap_contexts[cell_index])
    {
    	result = RRC_SUCCESS;
	}

	return result;
}
/*bugid 117989 end*/
