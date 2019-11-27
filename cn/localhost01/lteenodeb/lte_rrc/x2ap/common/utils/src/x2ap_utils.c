/******************************************************************************
*
*   FILE NAME : x2ap_utils.c
*
*   DESCRIPTION:
*       This source file contains common utility functions like APi header
*       construction.
*
*   DATE            AUTHOR             REFERENCE       REASON
*   01-JULY-2010    Vaibhav Singh      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include<x2ap_utils.h>
#include "rrc_msg_mgmt.h"

/******************************************************************************
*   The format of the X2AP API Header structure is following:
*           (In case of U16, HI is transmitted first)
*
*   U8  - version
*   U8  - from (HI)
*   U8  - from (LOW)
*   U8  - to (HI)
*   U8  - to (LOW)
*   U8  - api id (HI)
*   U8  - api id (LOW)
*   U8  - api size (HI)
*   U8  - api size (LOW)
*   10-16 bytes - spare
******************************************************************************/
/****************************************************************************
* Private Function Prototypes
****************************************************************************/
static U32 x2ap_get_sd_from_header(
       U8 *p_header);

static void x2ap_set_sd_in_header(
    U8 *p_header,
    U32 value);

/******************************************************************************
*   FUNCTION NAME: x2ap_get_byte_from_header
 * INPUTS        : p_header - pointer to  X2AP header
 * OUTPUTS       : None
 * RETURNS       : Byte
 * DESCRIPTION   : Function retreives byte from X2AP header
******************************************************************************/
static U8 x2ap_get_byte_from_header(U8 *p_header) /* X2AP header */
{
    X2AP_ASSERT(p_header != X2AP_NULL);

    return *p_header;
}

/******************************************************************************
*   FUNCTION NAME: x2ap_get_word_from_header
 * INPUTS        : p_header - pointer to  X2AP header
 * OUTPUTS       : None
 * RETURNS       : Word
 * DESCRIPTION   : Function retreives word from X2AP header
******************************************************************************/
static U16 x2ap_get_word_from_header(U8 *p_header) /* X2AP header */
{
    U16 value = X2AP_NULL;

    X2AP_ASSERT(p_header != X2AP_NULL);

    value = (U16)(p_header[0] << 8 | p_header[1]);
    
    return value;
}

/******************************************************************************
*   FUNCTION NAME: x2ap_get_version
 * INPUTS        : p_header - pointer to  X2AP header
 * OUTPUTS       : None
 * RETURNS       : Header version
 * DESCRIPTION   : Function retreives version from X2AP header
******************************************************************************/
U8 x2ap_get_version(void *p_header)/* X2AP header */
{
    X2AP_ASSERT(p_header != X2AP_NULL);

    return(x2ap_get_byte_from_header(&(((U8 *)p_header)[0])));
}

/******************************************************************************
*   FUNCTION NAME: x2ap_get_src_module_id
 * INPUTS        : p_header - pointer to  X2AP header
 * OUTPUTS       : None
 * RETURNS       : Source module Id
 * DESCRIPTION   : Function retreives source module Id from X2AP header
******************************************************************************/
x2ap_module_id_t x2ap_get_src_module_id(void *p_header) /* X2AP header */
{
    X2AP_ASSERT(p_header != X2AP_NULL);

    return((x2ap_module_id_t)
            (x2ap_get_word_from_header(&(((U8 *)p_header)[1]))));
}

/******************************************************************************
*   FUNCTION NAME: x2ap_get_dst_module_id
 * INPUTS        : p_header - pointer to  X2AP header
 * OUTPUTS       : None
 * RETURNS       : Destination module Id
 * DESCRIPTION   : Function retreives destination module Id from X2AP header
******************************************************************************/
x2ap_module_id_t x2ap_get_dst_module_id(U8 *p_header) /* X2AP header */
{
    return((x2ap_module_id_t)
            (x2ap_get_word_from_header(&(((U8 *)p_header)[3]))));
}

/******************************************************************************
*   FUNCTION NAME: x2ap_get_api_id
 * INPUTS        : p_header - pointer to  X2AP header
 * OUTPUTS       : None
 * RETURNS       : API Id
 * DESCRIPTION   : Function retreives API Id from X2AP header
******************************************************************************/
U16 x2ap_get_api_id(U8 *p_header) /* X2AP header */
{
    return(x2ap_get_word_from_header(&(((U8 *)p_header)[5])));
}

/******************************************************************************
*   FUNCTION NAME: x2ap_get_api_buf_size
 * INPUTS        : p_header - pointer to  X2AP header
 * OUTPUTS       : None
 * RETURNS       : API buffer size
 * DESCRIPTION   : Function retreives API buffer size from X2AP header
******************************************************************************/
U16 x2ap_get_api_buf_size(U8 *p_header)/* X2AP header */
{
    return(x2ap_get_word_from_header(&(((U8 *)p_header)[7])));
}

/******************************************************************************
*   FUNCTION NAME: x2ap_set_api_buf_size
 * INPUTS        : p_header - pointer to  X2AP header
 *                 api_buf_size - API message size
 * OUTPUTS       : p_header
 * RETURNS       : None
 * DESCRIPTION   : Function set API buffer size in X2AP header
******************************************************************************/
void x2ap_set_api_buf_size(
    U8      *p_header,      /* X2AP header */
    U16     api_buf_size)    /* API message size */
{
    /* api size (HI) */
    p_header[7] = (U8)((api_buf_size & 0xFF00) >> 8);

    /* api size (LOW) */
    p_header[8] = (U8)(api_buf_size & 0x00FF);
}

/******************************************************************************
*   FUNCTION NAME: x2ap_construct_api_header
 * INPUTS        : p_header - pointer to X2AP header
 *                 version_id - API version Id
 *                 src_module_id - Source module Id 
 *                 dst_module_id - Destination module Id
 *                 api_id - API Id
 *                 api_buf_size - API buffer size
 * OUTPUTS       : p_header
 * RETURNS       : None
 * DESCRIPTION   : Function constructs X2AP header from given parameters
******************************************************************************/
void x2ap_construct_api_header(
    U8                  *p_header,      /* X2AP header */
    U8                  version_id,     /* API version Id */
    x2ap_module_id_t    src_module_id,  /* Source module Id */
    x2ap_module_id_t    dst_module_id,  /* Destination module Id */
    U16                 api_id,         /* API Id */
    U16                 api_buf_size)    /* API buffer size */
{
    /* version */
    p_header[0] = version_id;

    /* from (HI) */
    p_header[1] = (U8)((src_module_id & 0xFF00) >> 8);

    /* from (LOW) */
    p_header[2] = (U8)(src_module_id & 0x00FF);

    /* to (HI) */
    p_header[3] = (U8)((dst_module_id & 0xFF00) >> 8);

    /* to (LOW) */
    p_header[4] = (U8)(dst_module_id & 0x00FF);

    /* api id (HI) */
    p_header[5] = (U8)((api_id & 0xFF00) >> 8);

    /* api id (LOW) */
    p_header[6] = (U8)(api_id & 0x00FF);

    /* api size (HI) */
    p_header[7] = (U8)((api_buf_size & 0xFF00) >> 8);

    /* api size (LOW) */
    p_header[8] = (U8)(api_buf_size & 0x00FF);

    /* spare bytes */
    p_header[9]     = 0x00;
    p_header[10]    = 0x00;
    p_header[11]    = 0x00;
    p_header[12]    = 0x00;
    p_header[13]    = 0x00;
    p_header[14]    = 0x00;
    p_header[15]    = 0x00;
}



/******************************************************************************
*   FUNCTION NAME: x2ap_get_transaction_id
 * INPUTS        : p_api - pointer to external API message
 * OUTPUTS       : None
 * RETURNS       : Transaction id
 * DESCRIPTION   : Function retreives transaction_id from external API message
******************************************************************************/
U16 x2ap_get_transaction_id(void *p_api)/* external API message */
{
    return(x2ap_get_word_from_header((U8 *)p_api + RRC_API_HEADER_SIZE));
}

/******************************************************************************
*   FUNCTION NAME: x2ap_construct_interface_api_header
 * INPUTS        : p_header - pointer to X2AP interface header
 *                 transaction_id - Interface transaction identifier
 *                 src_module_id - Source module identifier
 *                 dst_module_id - Destination module identifier
 *                 api_id - API identifier
 *                 api_buf_size - API buffer size
 * OUTPUTS       : p_header
 * RETURNS       : None
 * DESCRIPTION   : Function constructs interface api header from given parameters
******************************************************************************/
void x2ap_construct_interface_api_header(
    U8                  *p_header,      /* X2AP interface header */
    U16                 transaction_id, /* Interface transaction identifier */
    x2ap_module_id_t     src_module_id,  /* Source module identifier */
    x2ap_module_id_t     dst_module_id,  /* Destination module identifier */
    U16                 api_id,         /* API identifier */
    U16                 api_buf_size)    /* API buffer size */
{
 /* transaction identifier (HI) */
    p_header[0] = (U8)((transaction_id & 0xFF00) >> 8);

    /* transaction identifier (LOW) */
    p_header[1] = (U8)(transaction_id & 0x00FF);

    /* from (HI) */
    p_header[2] = (U8)((src_module_id & 0xFF00) >> 8);

    /* from (LOW) */
    p_header[3] = (U8)(src_module_id & 0x00FF);

    /* to (HI) */
    p_header[4] = (U8)((dst_module_id & 0xFF00) >> 8);

    /* to (LOW) */
    p_header[5] = (U8)(dst_module_id & 0x00FF);

    /* api id (HI) */
    p_header[6] = (U8)((api_id & 0xFF00) >> 8);

    /* api id (LOW) */
    p_header[7] = (U8)(api_id & 0x00FF);

    /*size includes length of header*/
    api_buf_size = (U16)(api_buf_size + RRC_INTERFACE_API_HEADER_SIZE);

    /* api size (HI) */
    p_header[8] = (U8)((api_buf_size & 0xFF00) >> 8);

    /* api size (LOW) */
    p_header[9] = (U8)(api_buf_size & 0x00FF);
}


/*****************************************************************************
 * FUNCTION NAME  : x2ap_get_sctp_sd
 * INPUTS         : p_header - ptr to the message buffer
 * OUTPUTS        : None
 * RETURNS        : socket_sd
 * DESCRIPTION    : This function calls the internal function that processes
 *              the consequent four bytes to return the sd value.
 ****************************************************************************/
U32 x2ap_get_sctp_sd(
        U32 *p_header)
{
    return x2ap_get_sd_from_header(&(((U8 *)p_header)[0]));
}


/* SPR_14436_Fix Start */
/******************************************************************************
 * Function Name  : x2ap_get_stream_id_from_header
 * Inputs         : p_header - ptr to the message buffer
 * Outputs        : None 
 * Returns        : stream_id
 * Description    : This function processes the consequent two bytes to 
 *                  return the stream_id value.
 *******************************************************************************/

U16 x2ap_get_stream_id_from_header(
        U8 *p_header
        )
{
    U16 value = X2AP_NULL;
    X2AP_ASSERT(p_header != X2AP_P_NULL);
    value = (U16)(p_header[0] << 8 | p_header[1]);
    return value;
}


/*****************************************************************************
 * Function Name  : x2ap_get_stream_id
 * Inputs         : p_header - ptr to the message buffer
 * Outputs        : None 
 * Returns        : stream_id
 * Description    : This function calls the internal function that processes
 *                  the consequent two bytes to return the stream_id value.
 *****************************************************************************/
U16 x2ap_get_stream_id(
        U16 *p_header 
)
{   
    return x2ap_get_stream_id_from_header(&(((U8 *)p_header)[0]));
}           
/* SPR_14436_Fix Stop */

/******************************************************************************
 * FUNCTION NAME : x2ap_get_sd_from_header
 * INPUTS        : p_header - ptr to the message buffer
 * OUTPUTS       : None
 * RETURNS       : socket_sd
 * DSCRIPTION    : This function processes the consequent four bytes to
 *               return the sd value.
 ******************************************************************************/

static U32 x2ap_get_sd_from_header(
        U8 *p_header)
{
    U32 value = X2AP_NULL;
    X2AP_ASSERT(p_header != X2AP_P_NULL);
    value = (U32)(p_header[0] << 24 | p_header[1] << 16 | p_header[2] << 8 |
            p_header[3]);
    return value;
}

/******************************************************************************
 * FUNCTION NAME  : x2ap_set_sd_in_header
 * INPUTS         : p_header - ptr to the message buffer
 *                  value - SD value to be set in message buffer
 * OUTPUTS        : p_header
 * RETURNS        : None
 * DESCRIPTION    : This function processes the consequent four bytes to
 *            set the sd value.
******************************************************************************/

static void x2ap_set_sd_in_header(
    U8 *p_header,
    U32 value)
{
    X2AP_ASSERT(p_header != X2AP_P_NULL);
    p_header[0] = (U8) (value & 0xFF000000);
    p_header[1] = (U8) (value & 0x00FF0000);
    p_header[2] = (U8) (value & 0x0000FF00);
    p_header[3] = (U8) (value & 0x000000FF);
}

/*****************************************************************************
 * FUNCTION NAME : x2ap_set_sctp_sd
 * INPUTS        : p_header - ptr to the message buffer
 *                  value - SD value to be set in message buffer
 * OUTPUTS       : p_header
 * RETURNS       : None
 * DESCRIPTION   : This function sets the sctp sd 
 ****************************************************************************/
void x2ap_set_sctp_sd(
    U32 *p_header ,
    S32 value
)
{
    x2ap_set_sd_in_header(&(((U8 *)p_header)[0]),(U32)value);
}

/* SPR_14436_Fix Start */
/******************************************************************************
 *  Function Name  : x2ap_set_stream_from_header
 *  Inputs         : p_header - ptr to the message buffer
 *                   value - STREAM value to be set in message buffer
 * 
 *  Outputs        : None
 *  Returns        : None
 *  Description    : This function processes the consequent two bytes to
 *                   set the stream value.
 * ******************************************************************************/
void x2ap_set_stream_from_header(
    U8 *p_header ,U16 value
)
{
    X2AP_ASSERT(p_header != X2AP_P_NULL);
    p_header[0] = (U8) (value & 0xFF00);
    p_header[1] = (U8) (value & 0x00FF);
}



/*****************************************************************************
 * Function Name  : x2ap_set_sctp_stream
 * Inputs         : p_header - ptr to the message buffer
 *                  value - STREAM value to be set in message buffer
 * Outputs        : None
 * Returns        : None
 * Description    : This function calls the internal function that processes
 * 	           the consequent two bytes to set the stream value.
 *****************************************************************************/
void x2ap_set_sctp_stream(
	 U16 *p_header ,U16 value
)
{
    x2ap_set_stream_from_header(&(((U8 *)p_header)[0]),(U16)value);
}

/* SPR_14436_Fix Stop */


/******************************************************************************
 * FUNCTION NAME : x2ap_get_nbr_index_from_ecgi
 * INPUTS        : p_x2ap_gb_ctxt - pointer to X2AP global context
 *                 nbr_ecgi - neighbour ecgi
 *                 insert_flag - to check the insert permission
 *                 cell_id - cell id
 * OUTPUTS       : None
 * RETURNS       : nbr index
 * DESCRIPTION   : This function gets the nbr index from ecgi values
 ******************************************************************************/

U32 x2ap_get_nbr_index_from_ecgi(
        x2ap_gb_context_t *p_x2ap_gb_ctxt,
        x2ap_ecgi_t nbr_ecgi,
        x2ap_bool_t insert_flag,
        U8 cell_id)
{
	/*
	 * 1. goto ecgi search tree
	 * 2. find entry for ecgi
	 *  2a. if not found insert node and generate nbr index 
	 *      by incrementing  nbr counter
	 *  2b else return index value found
	 */
	U32 nbr_index = X2AP_NULL;
	/* enb_configured changes start */
	/* coverity_fix_64001_start */
	U16 index = RRC_NULL;
	/* coverity_fix_64001_stop */
	/* enb_configured changes stop */
	x2ap_nbr_ecgi_search_node_t *p_node = X2AP_P_NULL;
	p_node = (x2ap_nbr_ecgi_search_node_t *)x2ap_st_find_tree_node (
			(x2ap_search_tree_t *)&
			(p_x2ap_gb_ctxt->nbr_ecgi_search_tree),
			(void*)(&nbr_ecgi) ); 

	if (!p_node)
	{
		if (X2AP_TRUE == insert_flag)
		{
			X2AP_TRACE(X2AP_DETAILED,
					"Neighbour ecgi not found in search tree..inserting\n");
			p_node = x2ap_mem_get(sizeof(x2ap_nbr_ecgi_search_node_t));
			/* coverity_fix_27756_start */
			if(X2AP_P_NULL == p_node)
			{
				X2AP_TRACE(X2AP_FATAL,"\nMemeory allocation failed ");
				return -1;
			}
			/* coverity_fix_27756_stop */
			/* enb_configured changes start */
			X2AP_MEMSET(p_node, 0,sizeof(x2ap_nbr_ecgi_search_node_t));
			/* enb_configured changes stop */

			p_node->ecgi = nbr_ecgi;
			p_node->serving_cell_index = cell_id;
			/* enb_configured changes start */
			for(index = 0; index < MAX_NEIGHBOURS; index++)
			{
				if(RRC_NULL == p_x2ap_gb_ctxt->peer_enb_cell_info[index].enb_configured)
				{
					nbr_index = index;
					p_node->logical_id = index;
					p_x2ap_gb_ctxt->peer_enb_cell_info[index].enb_configured = 1;

					break;
				}
			}    
			if(index == MAX_NEIGHBOURS)
			{
				nbr_index = -1;
				x2ap_mem_free(p_node);
			}
			else
            {
#ifdef ENDC_ENABLED
                p_x2ap_gb_ctxt->peer_enb_cell_info[p_x2ap_gb_ctxt->num_peer_enb_cells].oam_configured = RRC_TRUE;
#endif
                /* enb_configured changes stop */
                p_x2ap_gb_ctxt->num_peer_enb_cells++;
                X2AP_TRACE(X2AP_INFO,"Number of peer eNB neighbour cells : %u", p_x2ap_gb_ctxt->num_peer_enb_cells);

                /* calling function to insert in the tree */
                x2ap_st_insert_node(
                        &p_x2ap_gb_ctxt->nbr_ecgi_search_tree,
                        &(p_node->nodeAnchor));
            }
		}
		else
		{
			nbr_index = -1;
		}
	}
	else
	{
		nbr_index = p_node->logical_id;
	}
	return nbr_index;
}

/******************************************************************************
 * FUNCTION NAME : x2ap_get_ctxt_from_sd
 * INPUTS        : p_x2ap_gb_ctxt - pointer to X2AP global context
 *                 sd - value of type sctp_sd_t
 * OUTPUTS       : None
 * RETURNS       : peer eNb context
 * DESCRIPTION   : This function gets the peer eNb context. 
 ******************************************************************************/

x2ap_peer_enb_context_t * x2ap_get_ctxt_from_sd(
        x2ap_gb_context_t *p_x2ap_gb_ctxt,
        sctp_sd_t sd)
{
	x2_sctp_sd_search_node_t *p_node = X2AP_P_NULL;
	p_node = (x2_sctp_sd_search_node_t *)x2ap_st_find_tree_node (
			(x2ap_search_tree_t *)&
			(p_x2ap_gb_ctxt->enb_ctxt_trees.sd_search_tree),
			(void*)(&sd) ); 

	if (!p_node)
	{
		return (x2ap_peer_enb_context_t *) X2AP_P_NULL;
	}
	else
	{
		return p_node->p_enb_context;
	}

}


/******************************************************************************
 * FUNCTION NAME  : x2ap_get_ctxt_from_ip
 * INPUTS         : p_x2ap_gb_ctxt - pointer to X2AP global context
 *                  ip_addr  - IP address
 * Outputs        : None
 * Returns        : peer eNB context
 * Description    : This function gets the peer eNb context from IP address
 ******************************************************************************/

x2ap_peer_enb_context_t * x2ap_get_ctxt_from_ip(
        x2ap_gb_context_t *p_x2ap_gb_ctxt,
        U8 *ip_addr
        )
{
	sctp_ip_search_node_t *p_node = X2AP_P_NULL;
	p_node = (sctp_ip_search_node_t *)x2ap_st_find_tree_node(
			(x2ap_search_tree_t *)&
			(p_x2ap_gb_ctxt->enb_ctxt_trees.ip_search_tree),
			(void*)(ip_addr)); 

	if (!p_node)
	{
		return (x2ap_peer_enb_context_t *) X2AP_P_NULL;
	}
	else
	{
		return p_node->p_enb_context;
	}

}


/******************************************************************************
 * FUNCTION NAME  : x2ap_get_peer_enb_ctxt_from_enbid
 * INPUTS         : p_x2ap_gb_ctxt - pointer to X2AP global context
 *                  enbid - eNB id
 * OUTPUTS        : None
 * RETURNS        : peer eNB id
 * DESCRIPTION    : This function ges the peer eNB id from eNB id
 ******************************************************************************/

x2ap_peer_enb_context_t * x2ap_get_peer_enb_ctxt_from_enbid(
        x2ap_gb_context_t *p_x2ap_gb_ctxt,
        x2_gb_enb_id_t enbid)
{
    sctp_gb_enb_id_search_node_t *p_node = X2AP_P_NULL;
    p_node = (sctp_gb_enb_id_search_node_t *)x2ap_st_find_tree_node(
            (x2ap_search_tree_t *)&
            (p_x2ap_gb_ctxt->enb_ctxt_trees.enb_id_search_tree),
            (void*)(&enbid)); 

    if (!p_node)
    {
        return (x2ap_peer_enb_context_t *) X2AP_P_NULL;
    }
    else
    {
        return p_node->p_enb_context;
    }

}

		/*SPR 21554 Start*/
/******************************************************************************
 * FUNCTION NAME  : x2ap_populate_ecgi_from_cell_id
 * INPUTS         : 
 *                  p_x2ap_gb_ctx - x2ap global context 
 *                  cell_index - this is cell index of the cell
 *                  ecgi_val - pointer to x2ap_ecgi_t
 * OUTPUTS        : ecgi_val
 * RETURNS        : None
 * DESCRIPTION    : This function copy ecgi from global Context.
 ******************************************************************************/

void x2ap_populate_ecgi_from_cell_id(
        x2ap_gb_context_t* p_x2ap_gb_ctx,
        U8 cell_index,
        x2ap_ecgi_t *ecgi_val
        )
{
    ecgi_val->plmn_identity = p_x2ap_gb_ctx->global_enb_id.plmn_identity;

    /*initialize string to null*/
    l3_strncpy_wrapper((S8 *)(ecgi_val->eutran_cell_id),(const S8 *)"", HOME_ENB_ID_OCTET_SIZE);
    /*If enb is home enb, the ecgi cell_index param will be same as enbid*/
    if (p_x2ap_gb_ctx->global_enb_id.enb_id.presence_bitmask & X2AP_OAM_HOME_ENB_ID_PRESENT)
    {
        X2AP_MEMCPY(
            ecgi_val->eutran_cell_id, 
             /*SPR 21554 Start*/
             &p_x2ap_gb_ctx->global_enb_id.enb_id.home_enb_id.eNB_id,
             /*SPR 21554 End*/
            HOME_ENB_ID_OCTET_SIZE);
        X2AP_TRACE(X2AP_INFO, 
                "Composed ECGI(home):0x%02X%02X%02X%02X", ecgi_val->eutran_cell_id[0],ecgi_val->eutran_cell_id[1],
                ecgi_val->eutran_cell_id[2],ecgi_val->eutran_cell_id[3]);
    }
    else
    {
        /*copy ecgi  from global context*/
        X2AP_MEMCPY(
                ecgi_val,
                &p_x2ap_gb_ctx->served_cell_info[cell_index].ecgi,
                sizeof(x2ap_ecgi_t));
                X2AP_TRACE(X2AP_INFO, 
            "Populated ECGI(Macro):0x%02X%02X%02X%02X", ecgi_val->eutran_cell_id[0],ecgi_val->eutran_cell_id[1],
            ecgi_val->eutran_cell_id[2],ecgi_val->eutran_cell_id[3]);
    }
}
        /*SPR 21554 End*/


/******************************************************************************
 * FUNCTION NAME  : x2ap_compose_ecgi_from_cell_id
 * INPUTS         : 
 *                  g_enb_id - global eNB id
 *                  cell_id - cell id
 *                  ecgi_val - pointer to x2ap_ecgi_t
 * OUTPUTS        : ecgi_val
 * RETURNS        : None
 * DESCRIPTION    : This function composses the ecgi value from cell id.
 ******************************************************************************/

void x2ap_compose_ecgi_from_cell_id(
        x2_gb_enb_id_t g_enb_id,
        U8 cell_id,
        x2ap_ecgi_t *ecgi_val
        )
{
    ecgi_val->plmn_identity = g_enb_id.plmn_identity;

    /*initialize string to null*/
    l3_strncpy_wrapper((S8 *)(ecgi_val->eutran_cell_id),(const S8 *)"", HOME_ENB_ID_OCTET_SIZE);
    /*If enb is home enb, the ecgi cell_id param will be same as enbid*/
    if (g_enb_id.enb_id.presence_bitmask & X2AP_OAM_HOME_ENB_ID_PRESENT)
    {
        X2AP_MEMCPY(
            ecgi_val->eutran_cell_id, 
            g_enb_id.enb_id.home_enb_id.eNB_id,
            HOME_ENB_ID_OCTET_SIZE);
        X2AP_TRACE(X2AP_INFO, 
                "Composed ECGI(home):0x%2X%2X%2X%2X", ecgi_val->eutran_cell_id[0],ecgi_val->eutran_cell_id[1],
                ecgi_val->eutran_cell_id[2],ecgi_val->eutran_cell_id[3]);
    }
    else
    {
        /*copy into first 3 bytes the enb_id */
        X2AP_MEMCPY(
                ecgi_val->eutran_cell_id,
                g_enb_id.enb_id.macro_enb_id.eNB_id,
                MACRO_ENB_ID_OCTET_SIZE);
        /* reset the LSB quadret of 3rd byte of Macro eNB ID */
        ecgi_val->eutran_cell_id[MACRO_ENB_ID_OCTET_SIZE-1] =
            ecgi_val->eutran_cell_id[MACRO_ENB_ID_OCTET_SIZE-1] & 0xF0;

        /*
         *  copy the MSB quadret of cell id into LSB quadret of 3rd byte of 
         *  Macro eNB ID
         */
        ecgi_val->eutran_cell_id[MACRO_ENB_ID_OCTET_SIZE-1] =
         ecgi_val->eutran_cell_id[MACRO_ENB_ID_OCTET_SIZE-1] | (cell_id >> 4);
        /*
         *  copy the LSB quadret of cell id into MSB quadret of 4th byte of 
         *  Macro eNB ID
         */
        ecgi_val->eutran_cell_id[HOME_ENB_ID_OCTET_SIZE-1] =
            (cell_id << 4 | ecgi_val->eutran_cell_id[HOME_ENB_ID_OCTET_SIZE-1]);

        X2AP_TRACE(X2AP_INFO, 
            "Composed ECGI(Macro):0x%2X%2X%2X%2X", ecgi_val->eutran_cell_id[0],ecgi_val->eutran_cell_id[1],
            ecgi_val->eutran_cell_id[2],ecgi_val->eutran_cell_id[3]);
    }
}

/*SPR_19279_START*/
/******************************************************************************
 * FUNCTION NAME  : x2ap_compose_ecgi_from_cell_info
 * INPUTS         : 
 *                  g_enb_id - global eNB id
 *                  cell_info - peer cell info
 *                  ecgi_val - pointer to x2ap_ecgi_t
 * OUTPUTS        : ecgi_val
 * RETURNS        : None
 * DESCRIPTION    : This function composses the ecgi value from cell info struct
 ******************************************************************************/

void x2ap_compose_ecgi_from_cell_info(
        x2_gb_enb_id_t g_enb_id,
        x2ap_served_cell_info_t* p_cell_info,
        x2ap_ecgi_t *ecgi_val
        )
{
    U8 cell_id = p_cell_info->cell_id;

    ecgi_val->plmn_identity = p_cell_info->bcast_plmn_info.plmn_identity[0];
    //ecgi_val->plmn_identity = g_enb_id.plmn_identity;

    /*initialize string to null*/
    l3_strncpy_wrapper((S8 *)(ecgi_val->eutran_cell_id),(const S8 *)"", HOME_ENB_ID_OCTET_SIZE);
    /*If enb is home enb, the ecgi cell_id param will be same as enbid*/
    if (g_enb_id.enb_id.presence_bitmask & X2AP_OAM_HOME_ENB_ID_PRESENT)
    {
        X2AP_MEMCPY(
            ecgi_val->eutran_cell_id, 
            g_enb_id.enb_id.home_enb_id.eNB_id,
            HOME_ENB_ID_OCTET_SIZE);
    }
    else
    {
        /*copy into first 3 bytes the enb_id */
        X2AP_MEMCPY(
                ecgi_val->eutran_cell_id,
                g_enb_id.enb_id.macro_enb_id.eNB_id,
                MACRO_ENB_ID_OCTET_SIZE);
        /* reset the LSB quadret of 3rd byte of Macro eNB ID */
        ecgi_val->eutran_cell_id[MACRO_ENB_ID_OCTET_SIZE-1] =
            ecgi_val->eutran_cell_id[MACRO_ENB_ID_OCTET_SIZE-1] & 0xF0;

        /*
         *  copy the MSB quadret of cell id into LSB quadret of 3rd byte of 
         *  Macro eNB ID
         */
        ecgi_val->eutran_cell_id[MACRO_ENB_ID_OCTET_SIZE-1] =
         ecgi_val->eutran_cell_id[MACRO_ENB_ID_OCTET_SIZE-1] | (cell_id >> 4);
        /*
         *  copy the LSB quadret of cell id into MSB quadret of 4th byte of 
         *  Macro eNB ID
         */
        ecgi_val->eutran_cell_id[HOME_ENB_ID_OCTET_SIZE-1] =
            (cell_id << 4 | ecgi_val->eutran_cell_id[HOME_ENB_ID_OCTET_SIZE-1]);

    }
}
/*SPR_19279_END*/
/****************************************************************************
 * FUNCTION NAME  : x2ap_check_active_cell
 * INPUTS         : p_num_act_cell - number of active cell to found
 *                  p_act_cell_id  - logical id of active cell to found
 *                  p_x2ap_gl_ctx  - pointer to X2AP global context
 * OUTPUTS        : p_num_act_cell - number of active cell
 *                : p_act_cell_id  - logical id of active cell
 * RETURNS        : None
 * DESCRIPTION    : This function is called to know the number of active cell
 *                  and cell logical id.
 ****************************************************************************/
void x2ap_check_active_cell(U16                 *p_num_act_cell,
                            U16                 *p_act_cell_id,
                            x2ap_gb_context_t  *p_x2ap_gl_ctx)
{
    U16           cell_count = 0;
    X2AP_UT_TRACE_ENTER();
    for (cell_count = 0; cell_count < MAX_NUM_CELL_IN_X2AP; cell_count++)
    {
        if (X2AP_TRUE == 
                p_x2ap_gl_ctx->act_cell_info[cell_count].is_cell_active)
        {
             p_act_cell_id[*p_num_act_cell] =
                 p_x2ap_gl_ctx->act_cell_info[cell_count].cell_id;
             
             X2AP_TRACE(X2AP_INFO, "Active Cell ID:%d",
                     p_act_cell_id[*p_num_act_cell]);

            *p_num_act_cell = *p_num_act_cell + 1;
        }
    }
    X2AP_UT_TRACE_EXIT();
}

/****************************************************************************
 * FUNCTION NAME  : x2ap_check_active_cell_status
 * INPUTS         : p_x2ap_gl_ctx - pointer to X2AP global context
 *                  cell_id - cell Id
 *                  cell_present - to check cell exists or not
 * OUTPUTS        : None
 * RETURNS        : X2AP_SUCCESS/ X2AP_FAILURE
 * DESCRIPTION    : This function is called to verify active status of cell id
 ****************************************************************************/
 x2ap_return_et x2ap_check_active_cell_status(
                                x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                U8                 cell_id,
                                x2ap_bool_et       *cell_present)
{
    U16           cell_count = 0;
    x2ap_return_et retval =  X2AP_FAILURE;

    X2AP_UT_TRACE_ENTER();

    for (cell_count = 0; cell_count < MAX_NUM_CELL_IN_X2AP; cell_count++)
    {
        if (cell_id == p_x2ap_gl_ctx->act_cell_info[cell_count].cell_id)
        {
            *cell_present = X2AP_TRUE;
            
            if(X2AP_TRUE ==
                    p_x2ap_gl_ctx->act_cell_info[cell_count].is_cell_active)
            {
                retval  =  X2AP_SUCCESS;
            }
            break;
        }
    }

    if(*cell_present == X2AP_FALSE)
    {
        X2AP_TRACE(X2AP_ERROR, "Active cell info: %d as received from CSC does"
            " not match cell info received during OAM Provisioning", cell_id);
    }
    
    X2AP_UT_TRACE_EXIT();
    return retval;
}
/*SPR_16723_START*/
/****************************************************************************
 * FUNCTION NAME  : x2ap_update_cell_specific_params
 * INPUTS         : p_x2ap_gl_ctx - pointer to X2AP global context
 *                  p_x2ap_cell_add - pointer to cell_add msg recieved from csc
 * OUTPUTS        : None
 * RETURNS        : None
 * DESCRIPTION    : This function updates cell specific parameters in x2ap 
 *                  served_cell_info structure populated in provisioning 
 *                  done from OAM. This is done if after provisioning the
 *                  PCI/ARFCN of cell are modified by SON PCI/ARFCN selection
 *                  procedure which are shared in initial cell-config
 ****************************************************************************/
void x2ap_update_cell_specific_params(
                                x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                x2ap_csc_cell_add_t *p_x2ap_cell_add)
{
    X2AP_UT_TRACE_ENTER();

    U16  cell_count = 0;
        /*SPR 21554 Start*/
    U8  plmn_count = 0;
        /*SPR 21554 End*/

    for (cell_count = 0; cell_count < MAX_NUM_CELL_IN_X2AP; cell_count++)
    {
        /* Check ECGI */
        /*SPR 21554 Start*/
        if(p_x2ap_cell_add->cell_index==p_x2ap_gl_ctx->act_cell_info[cell_count].cell_id)
        {
            X2AP_MEMCPY((void*)p_x2ap_gl_ctx->served_cell_info[cell_count].ecgi.eutran_cell_id,
                    (const void*)p_x2ap_cell_add->eutran_cell_id, HOME_ENB_ID_OCTET_SIZE);
            /* Check PCI */
            for(plmn_count =0;plmn_count < p_x2ap_cell_add->broadcast_plmn_info.num_bcast_plmn_id;plmn_count++)
            {
            X2AP_MEMCPY((void*)&p_x2ap_gl_ctx->served_cell_info[cell_count].broadcast_plmn_info.plmn_identity[plmn_count],
                    (const void*)&p_x2ap_cell_add->broadcast_plmn_info.plmn_identity[plmn_count],sizeof(x2ap_plmn_identity_t));
            }
            p_x2ap_gl_ctx->served_cell_info[cell_count].broadcast_plmn_info.num_bcast_plmn_id=
                p_x2ap_cell_add->broadcast_plmn_info.num_bcast_plmn_id;
            /*SPR 21554 End */
            if(p_x2ap_cell_add->phys_cell_id != 
                    p_x2ap_gl_ctx->served_cell_info[cell_count].pci)
            {
                /*Update PCI */
                X2AP_TRACE(X2AP_INFO, 
                        "Updating PCI %u to PCI %u in x2ap served_cell_info",
                        p_x2ap_gl_ctx->served_cell_info[cell_count].pci, 
                        p_x2ap_cell_add->phys_cell_id);

                p_x2ap_gl_ctx->served_cell_info[cell_count].pci = 
                    p_x2ap_cell_add->phys_cell_id;
            }

            if(p_x2ap_gl_ctx->served_cell_info[cell_count].choice_eutra_mode.choice_eutra_mode.bitmask & X2AP_EUTRA_MODE_FDD)
            {
                /* Check EARFCNDL */
                if((U32)p_x2ap_cell_add->dl_earfcn != 
                        p_x2ap_gl_ctx->served_cell_info[cell_count].
                        choice_eutra_mode.choice_eutra_mode.fdd_info.dl_earfcn.earfcn)
                {
                    /*Update EARFCNDL */
                    X2AP_TRACE(X2AP_INFO, 
                            "Updating EARFCNDL %u from Old value: %u in x2ap served_cell_info",
                            p_x2ap_cell_add->dl_earfcn,
                            p_x2ap_gl_ctx->served_cell_info[cell_count].
                            choice_eutra_mode.choice_eutra_mode.fdd_info.dl_earfcn.earfcn);

                    p_x2ap_gl_ctx->served_cell_info[cell_count].
                        choice_eutra_mode.choice_eutra_mode.fdd_info.dl_earfcn.earfcn = 
                        (U32)p_x2ap_cell_add->dl_earfcn;
                }
                /* Check EARFCNUL */
                if(((U32)p_x2ap_cell_add->ul_earfcn != 
                            p_x2ap_gl_ctx->served_cell_info[cell_count].
                            choice_eutra_mode.choice_eutra_mode.fdd_info.ul_earfcn.earfcn))
                {
                    /*Update EARFCNUL */
                    X2AP_TRACE(X2AP_INFO, 
                            "Updating EARFCNUL %u from Old value: %u in x2ap served_cell_info",
                            p_x2ap_cell_add->ul_earfcn,
                            p_x2ap_gl_ctx->served_cell_info[cell_count].
                            choice_eutra_mode.choice_eutra_mode.fdd_info.ul_earfcn.earfcn);

                    p_x2ap_gl_ctx->served_cell_info[cell_count].
                        choice_eutra_mode.choice_eutra_mode.fdd_info.ul_earfcn.earfcn = 
                        (U32)p_x2ap_cell_add->ul_earfcn;
                }
            }
            if(p_x2ap_gl_ctx->served_cell_info[cell_count].choice_eutra_mode.choice_eutra_mode.bitmask & X2AP_EUTRA_MODE_TDD)
            {
                /* Check EARFCNDL */
                if((U32)p_x2ap_cell_add->dl_earfcn != 
                        p_x2ap_gl_ctx->served_cell_info[cell_count].
                        choice_eutra_mode.choice_eutra_mode.tdd_info.earfcn.earfcn)
                {
                    /*Update EARFCNDL */
                    X2AP_TRACE(X2AP_INFO, 
                            "Updating EARFCNUL %u from Old value: %u in x2ap served_cell_info",
                            p_x2ap_cell_add->dl_earfcn,
                            p_x2ap_gl_ctx->served_cell_info[cell_count].
                            choice_eutra_mode.choice_eutra_mode.tdd_info.earfcn.earfcn);

                    p_x2ap_gl_ctx->served_cell_info[cell_count].
                        choice_eutra_mode.choice_eutra_mode.tdd_info.earfcn.earfcn = 
                        (U32)p_x2ap_cell_add->dl_earfcn;
                }
            }
            break;
        }
    }
    X2AP_UT_TRACE_EXIT();
    return;
}
/*SPR_16723_END*/

/****************************************************************************
 * FUNCTION NAME  : x2ap_get_cell_context_by_cell_id
 * Inputs         : p_x2ap_gl_ctx - pointer to X2AP global context
 *                  cell_id - cell id
 *                  cell_index - cell index
 * OUTPUTS        : None
 * Returns        : X2AP_SUCCESS/ X2AP_FAILURE
 * DESCRIPTION    : This function is called to get cell context in Global eNB
 *                  context by cell ID   
 ****************************************************************************/
 x2ap_return_et x2ap_get_cell_context_by_cell_id(
                                x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                   U8               cell_id,
                                   U8*              cell_index)
{
    x2ap_return_et   retVal = X2AP_FAILURE;
    U8    cell_count = 0;

    X2AP_UT_TRACE_ENTER();

    for (cell_count = 0; cell_count < MAX_NUM_CELL_IN_X2AP; cell_count++)
    {
        if (cell_id == p_x2ap_gl_ctx->act_cell_info[cell_count].cell_id)
        {
            *cell_index = cell_count;
            retVal = X2AP_SUCCESS;
            break;
        }
    }
    
    if (retVal == X2AP_FAILURE)
    {
        X2AP_TRACE(X2AP_ERROR, "Active cell Info not present in X2AP "
                "Global context. Received Active Cell Id: %d\n",cell_id ); 
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/* X2AP AG_1 : Start */
/****************************************************************************
 * FUNCTION NAME  : x2ap_update_active_cell_status
 * Inputs         : p_x2ap_gl_ctx - pointer to X2AP global context
 *                  cell_id - cell id
 *                  cell_status - cell status
 * OUTPUTS        : None
 * RETURNS        : X2AP_SUCCESS/ X2AP_FAILURE
 * DESCRIPTION    : This function is called to update active status of cell id
                    in x2ap global context
 ****************************************************************************/
 x2ap_return_et x2ap_update_active_cell_status(     
                                   x2ap_gb_context_t *p_x2ap_gb_ctxt,
                                   U8                  cell_id,
                                   x2ap_bool_et        cell_status)
{
    U8           cell_count = 0;
    x2ap_return_et retval =  X2AP_FAILURE;

    X2AP_UT_TRACE_ENTER();

    for (cell_count = 0; cell_count < MAX_NUM_CELL_IN_X2AP; cell_count++)
    {
        if (cell_id == p_x2ap_gb_ctxt->act_cell_info[cell_count].cell_id)
        {
            p_x2ap_gb_ctxt->act_cell_info[cell_count].is_cell_active =
                cell_status;
            retval  =  X2AP_SUCCESS;
            X2AP_TRACE(X2AP_INFO, "Updated cell status %d of cell id %d in"
                    " X2AP global context", cell_status, cell_id); 
                break;
        }
    }

    if(retval == X2AP_FAILURE)
    {
        X2AP_TRACE(X2AP_ERROR, "Active Cell Id received from CSC is not"
                "configured at X2AP during OAM provisioning."
                "Received Active Cell Id: %d",cell_id );
    }

    X2AP_UT_TRACE_EXIT();
    return retval;
}
/* X2AP AG_1 : End */

/****************************************************************************
 * FUNCTION NAME  : x2ap_get_ngh_cell_index
 * INPUTS         : p_num_ngh_cell - number of neighbour cell
 *                  p_ngh_cell_index - logical index of neighbour cell
 *                  srvd_cell_index - served cell logical Id
 *                  is_only_active - only acknowleged cell need to consider
 *                  p_x2ap_gl_ctx  - pointer to X2AP global context
 * OUTPUTS        : p_num_ngh_cell - number of neighbour cell
 *                  p_ngh_cell_index  - logical index of neighbour cell
 * RETURNS        : None
 * DESCRIPTION    : This function is called to find out the number of 
 *                  neighbour cell and its Index in global context.
 ****************************************************************************/
void x2ap_get_ngh_cell_index(U16                *p_num_ngh_cell,
                             U16                *p_ngh_cell_index,
                             U16                 srvd_cell_index,
                             x2ap_gb_context_t *p_x2ap_gl_ctx,
                             x2ap_bool_t         is_only_active)
{
    U16    ngh_cell_count = 0;
    U16    index_count = 0;

    X2AP_UT_TRACE_ENTER();
    for (ngh_cell_count = 0; ngh_cell_count < MAX_NEIGHBOURS; ngh_cell_count++)
    {
        if (X2AP_FALSE == is_only_active)
        {
            if((DIRECT_NEIGHBOUR_INACTIVE == 
                        p_x2ap_gl_ctx->
                            relation_table[srvd_cell_index][ngh_cell_count].relation) ||
                    (DIRECT_NEIGHBOUR_ACTIVE ==
                     p_x2ap_gl_ctx->
                            relation_table[srvd_cell_index][ngh_cell_count].relation))
            {
                p_ngh_cell_index[index_count] = ngh_cell_count;
                X2AP_TRACE(X2AP_INFO,
                        "Served Cell ID:%d, Neighbour Cell Index:"
                        "%d Relation Table Value:%d",
                        srvd_cell_index,
                        ngh_cell_count,
                        p_x2ap_gl_ctx->
                        relation_table[srvd_cell_index][ngh_cell_count].relation);
                index_count++;
            }
        }
        else
        {
            if (DIRECT_NEIGHBOUR_ACTIVE ==
                    p_x2ap_gl_ctx->
                        relation_table[srvd_cell_index][ngh_cell_count].relation)
            {
                p_ngh_cell_index[index_count] = ngh_cell_count;
                X2AP_TRACE(X2AP_INFO,
                        " Served Cell ID:%d, Neighbour Cell Index:%d,"
                        "Relation Table Value:%d",
                        srvd_cell_index,
                        ngh_cell_count,
                        p_x2ap_gl_ctx->
                        relation_table[srvd_cell_index][ngh_cell_count].relation);

                index_count++;
            }
        }
    }
    *p_num_ngh_cell = index_count;
    X2AP_UT_TRACE_EXIT();
}


/****************************************************************************
 * FUNCTION NAME  : x2ap_get_enbid_from_nbr_cell_index
 * INPUTS         : p_x2ap_gl_ctx  - pointer to X2AP global context
 *                  nbr_cell_index - cell index of neighbour cell
 *                  p_gb_enb_id - gb eNB id
 * Outputs        : p_gb_enb_id
 * RETURNS        : None
 * DESCRIPTION    : This function id used to get eNB id from cell index of 
 *                  neighbour cel
 ****************************************************************************/
void x2ap_get_gb_enbid_from_nbr_cell_index(
                             x2ap_gb_context_t  *p_x2ap_gl_ctx,
                             U16                nbr_cell_index,
                             x2_gb_enb_id_t        *p_gb_enb_id)
{
    x2ap_peer_enb_cell_info_t *p_nbr_cell_info = 
        &(p_x2ap_gl_ctx->peer_enb_cell_info[nbr_cell_index]);
    
    X2AP_TRACE(X2AP_INFO, "ENB ID of Neighbour Cell Index:%d :Plmn ID:%s"
           " enb id(Macro/Home):%d, enbid:%s",nbr_cell_index,
           p_nbr_cell_info->peer_gb_enb_id.plmn_identity.plmn_id,
           p_nbr_cell_info->peer_gb_enb_id.enb_id.presence_bitmask,
           ((p_nbr_cell_info->peer_gb_enb_id.enb_id.presence_bitmask &
           X2AP_OAM_MACRO_ENB_ID_PRESENT)?p_nbr_cell_info->peer_gb_enb_id.
           enb_id.macro_enb_id.
           eNB_id: p_nbr_cell_info->peer_gb_enb_id.enb_id.home_enb_id.eNB_id));
           
    *p_gb_enb_id = p_nbr_cell_info->peer_gb_enb_id;
}

/****************************************************************************
 * FUNCTION NAME  : x2ap_get_enbid_set_from_nbr_cell_indexes
 * INPUTS         : p_x2ap_gl_ctx  - pointer to X2AP global context
 *                  num_ngh_cell - number of cells in neighbour hood 
 *                  p_ngh_cell_index_arr - pointer to nhd cells array
 *                  p_gb_enb_id_arr - pointer to x2_gb_enb_id_t
 * OUTPUTS        : p_gb_enb_id_arr
 * RETURNS        : None
 * DESCRIPTION    : This function is eNB Ids set using neighbour cells 
 ****************************************************************************/
void x2ap_get_enbid_set_from_nbr_cell_indexes(
        x2ap_gb_context_t *p_x2ap_gl_ctx,
        U16                num_ngh_cell,
        U16                *p_ngh_cell_index_arr,
        x2_gb_enb_id_t     *p_gb_enb_id_arr)
{
    U16 counter = 0;

    for (counter = 0; counter < num_ngh_cell; counter++)
    {
        x2ap_get_gb_enbid_from_nbr_cell_index(
                p_x2ap_gl_ctx, 
                p_ngh_cell_index_arr[counter],
                &p_gb_enb_id_arr[counter]);
    }
}

/****************************************************************************
 * FUNCTION NAME  : x2ap_get_uniq_enbid_set_from_nbr_cell_indexes
 * INPUTS         : p_x2ap_gl_ctx  - pointer to X2AP global context
 *                  num_ngh_cell - number fo cells in neighbour hood
 *                  p_ngh_cell_index_arr - pointer to cell index array 
 *                  p_num_enbid - num of eNB id 
 *                  p_gb_enb_id_arr - pointer to x2_gb_enb_id_t
 * OUTPUTS        : p_num_enbid
 *                : p_gb_enb_id_arr
 * Returns        : None
 * Description    : This function is celled to get unique eNB id
 ****************************************************************************/
void x2ap_get_uniq_enbid_set_from_nbr_cell_indexes(
        x2ap_gb_context_t *p_x2ap_gl_ctx,
        U16                num_ngh_cell,
        U16                *p_ngh_cell_index_arr,
        U16                *p_num_enbid,
        x2_gb_enb_id_t     *p_gb_enb_id_arr)
{
    U32 count1 = 0;
    U32 count2 = 0;

    /*gb_enb_id_t enb_id = {X2AP_NULL};*/
    x2ap_get_enbid_set_from_nbr_cell_indexes(p_x2ap_gl_ctx, num_ngh_cell, 
            p_ngh_cell_index_arr, p_gb_enb_id_arr);
    for (count1 = 0; count1 < num_ngh_cell; count1++) 
    {
        for (count2 = count1 + 1; count2 < num_ngh_cell; count2++)
        {
            if (!X2AP_MEMCMP(&p_gb_enb_id_arr[count2], &p_gb_enb_id_arr[count1], 
                                                       sizeof(x2_gb_enb_id_t)))
            {
                p_gb_enb_id_arr[count2] = p_gb_enb_id_arr[num_ngh_cell - 1];
                num_ngh_cell--;

            }
        }
    }
    *p_num_enbid = num_ngh_cell;
}


/****************************************************************************
 * FUNCTION NAME  : x2ap_get_gu_grpid_index
 * INPUTS         : p_x2ap_gl_ctx  - pointer to X2AP global context
 *                : gu_grp_id - gu grp id to find
 * OUTPUTS        : None
 * RETURNS        : index of the gu grp id found
 * DESCRIPTION    : This function is called to find out the index
 *                  of given grp id in the array in gb cntxt
 ****************************************************************************/

S32 x2ap_get_gu_grpid_index(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_gu_group_id_t gu_group_id)
{
    /*CID 81368 fix end*/
    S32 loop = 0;

    for (; loop < p_x2ap_gb_ctx->group_ids.num_gu_group_id; loop++)
    {
        if ( X2AP_NULL == X2AP_MEMCMP(&p_x2ap_gb_ctx->group_ids.
                            group_id_arr[loop], 
                            &gu_group_id, sizeof(x2ap_gu_group_id_t) ) )
        {
            break;
        }
    }

    if (loop == p_x2ap_gb_ctx->group_ids.num_gu_group_id)
    {
        loop = -1;
    }
    return loop;
}
/*x2ap-start*/
/*****************************************************************************
 * FUNCTION NAME  : x2ap_get_ue_id
 * INPUTS         : p_header - ptr to the message buffer
 * OUTPUTS        : None
 * RETURNS        : ue_id
 * DESCRIPTION    : This function processes consequent two bytes to return ue id.
 ****************************************************************************/
U16 x2ap_get_ue_id(
        U32 p_header)
{
    return ((U16)((0xffff)&(p_header)));
}

/*****************************************************************************
 * FUNCTION NAME  : x2ap_alloc_intrl_msg
 * INPUTS         : dst_module_id - Destination module identifier
 *                  api_id - API id
 *                  msg_size - Size of message (payload)
 * OUTPUTS        : NONE
 * RETURNS        : pointer to the allocated memory for buffer
 * DESCRIPTION    : This function is used to allocate memory for buffer and 
 *                  construct RRC API HEADER. 
 ****************************************************************************/
void *x2ap_alloc_intrl_msg(
        x2ap_module_id_t     dst_module_id,
        U16                 api_id,
        U16                 msg_size)
{
	/* coverity_fix_54708_start */
	void *p_api = X2AP_P_NULL;
	U16 msg_api_length = (U16)(msg_size + CV_HDRSIZE);

	X2AP_UT_TRACE_ENTER();

	/* Allocate buffer */
	p_api = x2ap_msg_mem_get(msg_api_length);

	if (X2AP_P_NULL == p_api)
	{
		X2AP_TRACE(X2AP_WARNING, "%s:"
				"x2ap_msg_mem_get failed.",__FUNCTION__);
		return X2AP_P_NULL;
	}

	X2AP_MEMSET(
			p_api,
			0,
			msg_api_length);

	/* Fill RRC header */
	x2ap_construct_api_header(
			(U8*)p_api,
			RRC_VERSION_ID,
			RRC_X2AP_MODULE_ID,
			dst_module_id,
			api_id,
			msg_api_length);

	X2AP_UT_TRACE_EXIT();
	return p_api;
}
/* coverity_fix_54708_stop */

/*****************************************************************************
 * FUNCTION NAME  : x2ap_populate_configuration_parameters
 * INPUTS         : p_src - pointer  to source location
 *                  p_tgt - pointer  to target location
 * OUTPUTS        : p_tgt - ptr to target location
 * RETURNS        : None
 * DESCRIPTION    : This function populates x2ap configuration parameters.
 ****************************************************************************/
void x2ap_populate_configuration_parameters(x2ap_conf_param_t* p_tgt, x2ap_conf_param_t* p_src)
{
    X2AP_MEMCPY((void*)&p_tgt->sctp_conf_param, (const void*)&p_src->sctp_conf_param, sizeof(x2ap_sctp_conf_info_t));
    p_tgt->timer_conf_info.bitmask = p_src->timer_conf_info.bitmask;
    p_tgt->timer_conf_info.sctp_connect_tim_val = p_src->timer_conf_info.sctp_connect_tim_val;
    p_tgt->timer_conf_info.x2ap_csi_guard_tim_val = p_src->timer_conf_info.x2ap_csi_guard_tim_val;

    p_tgt->timer_conf_info.x2ap_setup_retry_tim_val = p_src->timer_conf_info.x2ap_setup_retry_tim_val;
    p_tgt->timer_conf_info.sctp_connect_retry_cnt = p_src->timer_conf_info.sctp_connect_retry_cnt;
    p_tgt->timer_conf_info.x2ap_setup_retry_cnt = p_src->timer_conf_info.x2ap_setup_retry_cnt;
    p_tgt->timer_conf_info.eNB_Config_Update_retry_cnt = p_src->timer_conf_info.eNB_Config_Update_retry_cnt;

    p_tgt->timer_conf_info.x2ap_rsu_enb_guard_tim_val = p_src->timer_conf_info.x2ap_rsu_enb_guard_tim_val;
    p_tgt->timer_conf_info.x2ap_rsu_rrm_guard_tim_val = p_src->timer_conf_info.x2ap_rsu_rrm_guard_tim_val;

    if(p_src->timer_conf_info.bitmask & X2AP_TIME_TO_WAIT_INDEX_PRESENT)
    {
    	p_tgt->timer_conf_info.x2ap_time_to_wait_index = p_src->timer_conf_info.x2ap_time_to_wait_index;
    }
    else
    {
    	X2AP_TRACE(X2AP_WARNING, "Time to wait index not present\n");
    	p_tgt->timer_conf_info.x2ap_time_to_wait_index = X2AP_TIME_TO_WAIT_VALUE_DEFAULT;
    }

	if(p_src->timer_conf_info.bitmask & X2AP_ENB_CFG_INFO_PRESENT)
	{
	    p_tgt->timer_conf_info.eNB_Config_Update_retry_timer_val = p_src->timer_conf_info.eNB_Config_Update_retry_timer_val;
	    p_tgt->timer_conf_info.eNB_Config_Update_time_to_wait_retry_cnt = p_src->timer_conf_info.eNB_Config_Update_time_to_wait_retry_cnt;
	    p_tgt->timer_conf_info.eNB_Config_Update_time_to_wait_offset = p_src->timer_conf_info.eNB_Config_Update_time_to_wait_offset;
	}
	else
	{
		X2AP_TRACE(X2AP_WARNING, "eNB Config Update configuration parameters not present\n");
		p_tgt->timer_conf_info.eNB_Config_Update_retry_timer_val = X2AP_ENB_CFG_RETRY_TIMER_VAL_DEFAULT;
	    p_tgt->timer_conf_info.eNB_Config_Update_time_to_wait_retry_cnt = X2AP_ENB_CFG_TIME_TO_WAIT_RETRY_COUNT_DEFAULT;
	    p_tgt->timer_conf_info.eNB_Config_Update_time_to_wait_offset = X2AP_ENB_CFG_TIME_TO_WAIT_OFFSET_TIMER_VAL_DEFAULT;
	}
    if(p_src->timer_conf_info.bitmask & X2AP_RESET_CFG_INFO_PRESENT)
	{
        p_tgt->timer_conf_info.x2ap_enb_reset_retry_cnt = p_src->timer_conf_info.x2ap_enb_reset_retry_cnt;
        p_tgt->timer_conf_info.x2ap_enb_reset_guard_tim_val = p_src->timer_conf_info.x2ap_enb_reset_guard_tim_val;
	}
	else
	{
		X2AP_TRACE(X2AP_WARNING, "Reset configuration parameters not present\n");
		p_tgt->timer_conf_info.x2ap_enb_reset_retry_cnt = X2AP_RESET_RETRY_COUNT_DEFAULT;
	    p_tgt->timer_conf_info.x2ap_enb_reset_guard_tim_val = X2AP_RESET_RETRY_TIMER_VAL_DEFAULT;
	}

    if(p_src->timer_conf_info.bitmask & X2AP_CELL_ACTIVATION_TIMER_INFO_PRESENT)
	{
        p_tgt->timer_conf_info.x2ap_cell_activation_retry_cnt =
                            p_src->timer_conf_info.x2ap_cell_activation_retry_cnt;
        p_tgt->timer_conf_info.x2ap_cell_activation_guard_tim_val = p_src->timer_conf_info.x2ap_cell_activation_guard_tim_val;
	}
	else
	{
		X2AP_TRACE(X2AP_WARNING, "Cell activation timer info not present: using default values\n");
		p_tgt->timer_conf_info.x2ap_cell_activation_retry_cnt = X2AP_CELl_ACTIVATION_RETRY_COUNT_DEFAULT;
	    p_tgt->timer_conf_info.x2ap_cell_activation_guard_tim_val = X2AP_CELl_ACTIVATION_TIMER_VAL_DEFAULT;
	}

    if(p_src->timer_conf_info.bitmask & X2AP_MOBILITY_CHANGE_INFO_PRESENT)
	{
        p_tgt->timer_conf_info.x2ap_mobility_setting_change_retry_cnt = 
            p_src->timer_conf_info.x2ap_mobility_setting_change_retry_cnt;
        p_tgt->timer_conf_info.x2ap_mobility_setting_change_guard_tim_val = 
            p_src->timer_conf_info.x2ap_mobility_setting_change_guard_tim_val;
	}
	else
	{
		X2AP_TRACE(X2AP_WARNING, "Mobility Change parameters not present\n");
		p_tgt->timer_conf_info.x2ap_mobility_setting_change_retry_cnt = X2AP_MOBILITY_CHANGE_RETRY_COUNT_DEFAULT;
	    p_tgt->timer_conf_info.x2ap_mobility_setting_change_guard_tim_val = X2AP_MOBILITY_CHANGE_RETRY_TIMER_VAL_DEFAULT;
	}
}

/*****************************************************************************
 * FUNCTION NAME  : validate_x2setup_time_to_wait_index_range
 * INPUTS         : x2ap_time_to_wait_index - index to time to wait presentt 
 * OUTPUTS        : None
 * RETURNS        : X2AP_SUCCESS/X2AP_FAILURE
 * DESCRIPTION    : This function validates X2 time to wait value.
 ****************************************************************************/
x2ap_return_et validate_x2setup_time_to_wait_index_range(
        U8 x2ap_time_to_wait_index)
{

    X2AP_UT_TRACE_ENTER();

    switch(x2ap_time_to_wait_index)
    {
        case SEC_1:
        case SEC_2:
        case SEC_5:
        case SEC_10:
        case SEC_20:
        case SEC_60:
        break;
        default:
        X2AP_TRACE(X2AP_ERROR,"Invalid x2ap_time_to_wait_index value received %d",
                x2ap_time_to_wait_index);
        return X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;
}


/*****************************************************************************
 * FUNCTION NAME  : x2ap_compose_cell_id_from_ecgi
 * INPUTS         : global_enb_id - global eNB id
 *                  eutran_cell_id - eutran cell id
 *                  p_cell_id - pointer to cell id
 * OUTPUTS        : p_cell_id
 * RETURNS        : None
 * DESCRIPTION    : This function composes the cell id from ecgi value of eNB
 ****************************************************************************/
void x2ap_compose_cell_id_from_ecgi(x2_gb_enb_id_t global_enb_id, U8* eutran_cell_id, U8* p_cell_id)
{
    U8 local_eutran_cell_id[HOME_ENB_ID_OCTET_SIZE];

    X2AP_MEMCPY(local_eutran_cell_id, eutran_cell_id, HOME_ENB_ID_OCTET_SIZE);

	if (global_enb_id.enb_id.presence_bitmask & X2AP_OAM_HOME_ENB_ID_PRESENT)
    {
    	/*cell_id is 0 in case of HOME eNB ID*/
    	*p_cell_id = 0;
    }
    else if (global_enb_id.enb_id.presence_bitmask & X2AP_OAM_MACRO_ENB_ID_PRESENT)
    {
    	/*cell_id is eight bits from 21-18 bit of EUTRAN CELL ID in case of MACRO eNB ID*/

        /* Copy the LSB of 3rd byte of EUTRAN CELL ID into MSB of cell_id  */
    	local_eutran_cell_id[MACRO_ENB_ID_OCTET_SIZE-1] = local_eutran_cell_id[MACRO_ENB_ID_OCTET_SIZE-1] & 0x0F;
        *p_cell_id = (local_eutran_cell_id[MACRO_ENB_ID_OCTET_SIZE-1]<<4);

        /* Copy the MSB of 4rd byte of EUTRAN CELL ID into LSB of cell_id  */
        local_eutran_cell_id[MACRO_ENB_ID_OCTET_SIZE] = local_eutran_cell_id[MACRO_ENB_ID_OCTET_SIZE] & 0xF0;
        *p_cell_id |= (local_eutran_cell_id[MACRO_ENB_ID_OCTET_SIZE]>>4);
    }
}
 /*SPR_20743_FIX_START*/
/*****************************************************************************
 * FUNCTION NAME  : x2ap_delete_peer_enb_from_search_tree 
 * INPUTS         : x2ap_gb_context_t   *p_x2ap_gl_ctx,
 *                : x2ap_search_tree_t  *p_search_tree,
 *                : x2ap_peer_enb_context_t *p_peer_enb_ctxt
 *                : x2ap_search_tree_node_t *p_node_to_delete
 * OUTPUTS        : None 
 * RETURNS        : void 
 * DESCRIPTION    : This function deletes passed node from ip_search_tree or
 *                  ipv6_search_tree. 
 ****************************************************************************/
void x2ap_delete_peer_enb_from_search_tree(
        x2ap_gb_context_t   *p_x2ap_gl_ctx,
        x2ap_search_tree_t  *p_search_tree,
        x2ap_peer_enb_context_t *p_peer_enb_ctxt,
        x2ap_search_tree_node_t *p_node_to_delete)
{
    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_search_tree != X2AP_P_NULL);
    X2AP_ASSERT(p_peer_enb_ctxt != X2AP_P_NULL);
    X2AP_ASSERT(p_node_to_delete != X2AP_P_NULL);
    
    x2ap_st_delete_node(
            p_search_tree, 
            p_node_to_delete);
    x2ap_mem_free(p_node_to_delete);
    if ((RRC_TRUE == p_peer_enb_ctxt->is_enb_conf) && (p_x2ap_gl_ctx->num_peer_enb > RRC_NULL))
    {
        p_x2ap_gl_ctx->num_peer_enb--;
    }
    else
    {
        X2AP_TRACE (X2AP_WARNING,"p_x2ap_gl_ctx->num_peer_enb [%u] is_enb_conf[%u]",
                p_x2ap_gl_ctx->num_peer_enb, p_peer_enb_ctxt->is_enb_conf);
    }
    X2AP_TRACE (X2AP_INFO,"Deleted node from ip_search_tree."
            " Connected Peer eNB Count[%u]", p_x2ap_gl_ctx->num_peer_enb);
    p_peer_enb_ctxt->is_enb_conf = RRC_FALSE;
    X2AP_UT_TRACE_EXIT();
}

#ifdef ENDC_ENABLED
/******************************************************************************
 * FUNCTION NAME : x2ap_get_nbr_index_from_nr_cgi
 * INPUTS        : p_x2ap_gb_ctxt - pointer to X2AP global context
 *                 nbr_nr_cgi - neighbour nr cgi
 *                 insert_flag - to check the insert permission
 *                 cell_id - cell id
 * OUTPUTS       : None
 * RETURNS       : nbr index
 * DESCRIPTION   : This function gets the nbr index from nr cgi values
 ******************************************************************************/

S32 x2ap_get_nbr_index_from_nr_cgi(
        x2ap_gb_context_t *p_x2ap_gb_ctxt,
        x2ap_nr_ecgi_t nbr_nr_cgi,
        x2ap_bool_t insert_flag,
        U8 cell_id)
{
    /*
    * 1. goto nr_cgi search tree
    * 2. find entry for nr_cgi
    *  2a. if not found insert node and generate nbr index 
    *      by incrementing  nbr counter
    *  2b else return index value found
    */
    S32 nbr_index = X2AP_NULL;
    U16 index = RRC_NULL;
    x2ap_nbr_nr_cgi_search_node_t *p_node = X2AP_P_NULL;
    
    p_node = (x2ap_nbr_nr_cgi_search_node_t *)x2ap_st_find_tree_node (
                (x2ap_search_tree_t *)&
                (p_x2ap_gb_ctxt->nbr_nr_cgi_search_tree),
                (void*)(&nbr_nr_cgi) ); 

    if (!p_node)
    {
        if (X2AP_TRUE == insert_flag)
        {
            X2AP_TRACE(X2AP_DETAILED,
	        "Neighbour nr_cgi not found in search tree..inserting\n");
            p_node = x2ap_mem_get(sizeof(x2ap_nbr_nr_cgi_search_node_t));
	   
            if(X2AP_P_NULL == p_node)
            {
                X2AP_TRACE(X2AP_FATAL,"\nMemeory allocation failed ");
                return -1;
            }
            
            X2AP_MEMSET(p_node, 0,sizeof(x2ap_nbr_nr_cgi_search_node_t));

            p_node->nr_cgi = nbr_nr_cgi;
            p_node->serving_cell_index = cell_id;

            if (MAX_NEIGHBOURS <= p_x2ap_gb_ctxt->num_peer_enb_cells)
            {
                nbr_index = -1;
                x2ap_mem_free(p_node);
            }
            else
            {
                for(index = 0; index < MAX_NEIGHBOURS; index++)
                {
                    if(RRC_NULL == p_x2ap_gb_ctxt->peer_enb_cell_info[index].enb_configured)
                    {
                        nbr_index = index;
                        p_node->logical_id = index;
                        p_x2ap_gb_ctxt->peer_enb_cell_info[index].enb_configured = 1;
                        p_x2ap_gb_ctxt->peer_enb_cell_info[index].oam_configured = RRC_FALSE;//RRC_TRUE;

                        break;
                    }
                }    
                
                p_x2ap_gb_ctxt->num_peer_enb_cells++;
                X2AP_TRACE(X2AP_INFO,"Number of peer eNB neighbour cells : %u", p_x2ap_gb_ctxt->num_peer_enb_cells);

                /* calling function to insert in the tree */
                x2ap_st_insert_node(
                        &p_x2ap_gb_ctxt->nbr_nr_cgi_search_tree,
                        &(p_node->nodeAnchor));
            }
        }
        else
        {
            nbr_index = -1;
        }
    }
    else
    {
        nbr_index = p_node->logical_id;
    }
    
    return nbr_index;
}
#endif

  /*SPR_20743_FIX_END*/
