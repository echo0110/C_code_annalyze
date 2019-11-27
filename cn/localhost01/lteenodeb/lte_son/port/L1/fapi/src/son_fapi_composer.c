/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_fapi_composer.c $
 *
 *******************************************************************************
 *
 * File Description: File son_fapi_composer.c added for SON module code
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#include "son_pack_unpack_utils.h"
#include "son_fapi_composer.h"
#include "son_utils.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
son_length_t
son_get_vendor_specific_list_len
(
    vendor_specific_list_t *p_vendor_specific_list
);

static
son_return_et
son_compose_vendor_specific_list
(
    U8  **pp_buffer,
    vendor_specific_list_t *p_vendor_specific_list
);

static
son_length_t
son_get_cell_found_list_len
(
    cell_found_list_t *p_cell_found_list
);

static
son_return_et
son_compose_cell_found_list
(
    U8  **pp_buffer,
    cell_found_list_t *p_cell_found_list
);

static
son_length_t
son_get_sib_info_len
(
    sib_info_t *p_sib_info
);

static
son_return_et
son_compose_sib_info
(
    U8  **pp_buffer,
    sib_info_t *p_sib_info
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

son_length_t
son_get_son_phy_rssi_meas_req_len
(
    son_phy_rssi_meas_req_t *p_son_phy_rssi_meas_req
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_phy_rssi_meas_req != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_rssi_meas_req->freq_band_indicator);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_rssi_meas_req->meas_period);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_rssi_meas_req->num_earfcn);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_son_phy_rssi_meas_req->num_earfcn * sizeof(p_son_phy_rssi_meas_req->earfcn[0]));
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_rssi_meas_req->num_vendor_specific_list);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_rssi_meas_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_rssi_meas_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_rssi_meas_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_rssi_meas_req->num_vendor_specific_list; loop++)
        {
            length += son_get_vendor_specific_list_len(&p_son_phy_rssi_meas_req->vendor_specific_list[loop]);
        }
    }

    return length;
}

son_return_et
son_compose_son_phy_rssi_meas_req
(
    U8  **pp_buffer,
    son_phy_rssi_meas_req_t *p_son_phy_rssi_meas_req
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_phy_rssi_meas_req != SON_PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_phy_rssi_meas_req->freq_band_indicator < 1) || (p_son_phy_rssi_meas_req->freq_band_indicator > MAX_FREQ_BAND_INDICATOR))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_rssi_meas_req->freq_band_indicator] should be in range [1 <= %d <= MAX_FREQ_BAND_INDICATOR] !", p_son_phy_rssi_meas_req->freq_band_indicator);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_rssi_meas_req->freq_band_indicator, "freq_band_indicator");
    *pp_buffer += sizeof(p_son_phy_rssi_meas_req->freq_band_indicator);

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_rssi_meas_req->meas_period, "meas_period");
    *pp_buffer += sizeof(p_son_phy_rssi_meas_req->meas_period);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_rssi_meas_req->num_earfcn > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_rssi_meas_req->num_earfcn] should be in range [%d <= 100] !", p_son_phy_rssi_meas_req->num_earfcn);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_rssi_meas_req->num_earfcn, "num_earfcn");
    *pp_buffer += sizeof(p_son_phy_rssi_meas_req->num_earfcn);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_rssi_meas_req->num_earfcn; loop++)
        {
            son_cp_pack_U32(*pp_buffer, &p_son_phy_rssi_meas_req->earfcn[loop], "earfcn[]");
            *pp_buffer += sizeof(U32);
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_rssi_meas_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_rssi_meas_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_rssi_meas_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_rssi_meas_req->num_vendor_specific_list, "num_vendor_specific_list");
    *pp_buffer += sizeof(p_son_phy_rssi_meas_req->num_vendor_specific_list);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_rssi_meas_req->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_compose_vendor_specific_list(pp_buffer, &p_son_phy_rssi_meas_req->vendor_specific_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_fapi_send_son_phy_rssi_meas_req
*
*   DESCRIPTION:
*       This function constructs and sends SON_PHY_RSSI_MEAS_REQ message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_fapi_send_son_phy_rssi_meas_req
(
    son_phy_rssi_meas_req_t  *p_son_phy_rssi_meas_req,           /* Pointer to API specific information. */
    U16                 src_module_id  /* Source module identifier */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_fapi_msg;

    SON_ASSERT(p_son_phy_rssi_meas_req != SON_PNULL);

    /* Get API length */
    msg_length = son_get_son_phy_rssi_meas_req_len(p_son_phy_rssi_meas_req);

    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_INFO, "Src(%u)->SON_PHY_RSSI_MEAS_REQ", src_module_id);

   if(msg_length == SON_FAILURE)
	{
        return SON_FAILURE;
	}

    msg_api_length = msg_length + SON_API_HEADER_SIZE + SON_PHY_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_fapi_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_fapi_msg, SON_VERSION_ID, src_module_id,
        SON_FAPI_MODULE_ID, SON_PHY_RSSI_MEAS_REQ,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_fapi_msg = p_fapi_msg + SON_API_HEADER_SIZE;
        son_phy_construct_interface_api_header(p_fapi_msg, SON_PHY_RSSI_MEAS_REQ, SON_NULL, msg_length);

    /* Fill SON_PHY_RSSI_MEAS_REQ message */
    p_fapi_msg = p_fapi_msg + SON_PHY_INTERFACE_API_HEADER_SIZE;
    if (SON_FAILURE == son_compose_son_phy_rssi_meas_req(&p_fapi_msg, p_son_phy_rssi_meas_req))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    SON_ASSERT((p_fapi_msg - msg_api_length) == p_msg);

    /* Send message to */
    son_send_msg(p_msg, SON_FAPI_MODULE_ID);

    return SON_SUCCESS;
}
static
son_length_t
son_get_vendor_specific_list_len
(
    vendor_specific_list_t *p_vendor_specific_list
)
{
    son_length_t length = 0;

    SON_ASSERT(p_vendor_specific_list != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_vendor_specific_list->type);
    /* Get length of parameter of basic type */
    length += sizeof(p_vendor_specific_list->length);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_vendor_specific_list->length * sizeof(p_vendor_specific_list->value[0]));

    return length;
}

static
son_return_et
son_compose_vendor_specific_list
(
    U8  **pp_buffer,
    vendor_specific_list_t *p_vendor_specific_list
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_vendor_specific_list != SON_PNULL);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_vendor_specific_list->type, "type");
    *pp_buffer += sizeof(p_vendor_specific_list->type);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_vendor_specific_list->length, "length");
    *pp_buffer += sizeof(p_vendor_specific_list->length);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_vendor_specific_list->length; loop++)
        {
            son_cp_pack_U8(*pp_buffer, &p_vendor_specific_list->value[loop], "value[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return SON_SUCCESS;
}

son_length_t
son_get_son_phy_rssi_meas_ind_len
(
    son_phy_rssi_meas_ind_t *p_son_phy_rssi_meas_ind
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_phy_rssi_meas_ind != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_rssi_meas_ind->rssi);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_rssi_meas_ind->earfcn);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_rssi_meas_ind->num_vendor_specific_list);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_rssi_meas_ind->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_rssi_meas_ind->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_rssi_meas_ind->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_rssi_meas_ind->num_vendor_specific_list; loop++)
        {
            length += son_get_vendor_specific_list_len(&p_son_phy_rssi_meas_ind->vendor_specific_list[loop]);
        }
    }

    return length;
}

son_return_et
son_compose_son_phy_rssi_meas_ind
(
    U8  **pp_buffer,
    son_phy_rssi_meas_ind_t *p_son_phy_rssi_meas_ind
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_phy_rssi_meas_ind != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_rssi_meas_ind->rssi > 128))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_rssi_meas_ind->rssi] should be in range [%d <= 128] !", p_son_phy_rssi_meas_ind->rssi);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_rssi_meas_ind->rssi, "rssi");
    *pp_buffer += sizeof(p_son_phy_rssi_meas_ind->rssi);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_phy_rssi_meas_ind->earfcn, "earfcn");
    *pp_buffer += sizeof(p_son_phy_rssi_meas_ind->earfcn);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_rssi_meas_ind->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_rssi_meas_ind->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_rssi_meas_ind->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_rssi_meas_ind->num_vendor_specific_list, "num_vendor_specific_list");
    *pp_buffer += sizeof(p_son_phy_rssi_meas_ind->num_vendor_specific_list);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_rssi_meas_ind->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_compose_vendor_specific_list(pp_buffer, &p_son_phy_rssi_meas_ind->vendor_specific_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_fapi_send_son_phy_rssi_meas_ind
*
*   DESCRIPTION:
*       This function constructs and sends SON_PHY_RSSI_MEAS_IND message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_fapi_send_son_phy_rssi_meas_ind
(
    son_phy_rssi_meas_ind_t  *p_son_phy_rssi_meas_ind,           /* Pointer to API specific information. */
    U16                 src_module_id  /* Source module identifier */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_fapi_msg;

    SON_ASSERT(p_son_phy_rssi_meas_ind != SON_PNULL);

    /* Get API length */
    msg_length = son_get_son_phy_rssi_meas_ind_len(p_son_phy_rssi_meas_ind);

    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_INFO, "Src(%u)->SON_PHY_RSSI_MEAS_IND", src_module_id);

   if(msg_length == SON_FAILURE)
	{
        return SON_FAILURE;
	}

    msg_api_length = msg_length + SON_API_HEADER_SIZE + SON_PHY_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_fapi_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_fapi_msg, SON_VERSION_ID, src_module_id,
        SON_FAPI_MODULE_ID, SON_PHY_RSSI_MEAS_IND,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_fapi_msg = p_fapi_msg + SON_API_HEADER_SIZE;
        son_phy_construct_interface_api_header(p_fapi_msg, SON_PHY_RSSI_MEAS_IND, SON_NULL, msg_length);

    /* Fill SON_PHY_RSSI_MEAS_IND message */
    p_fapi_msg = p_fapi_msg + SON_PHY_INTERFACE_API_HEADER_SIZE;
    if (SON_FAILURE == son_compose_son_phy_rssi_meas_ind(&p_fapi_msg, p_son_phy_rssi_meas_ind))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    SON_ASSERT((p_fapi_msg - msg_api_length) == p_msg);

    /* Send message to */
    son_send_msg(p_msg, SON_FAPI_MODULE_ID);

    return SON_SUCCESS;
}
son_length_t
son_get_son_phy_rssi_meas_cnf_len
(
    son_phy_rssi_meas_cnf_t *p_son_phy_rssi_meas_cnf
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_phy_rssi_meas_cnf != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_rssi_meas_cnf->num_vendor_specific_list);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_rssi_meas_cnf->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_rssi_meas_cnf->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_rssi_meas_cnf->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_rssi_meas_cnf->num_vendor_specific_list; loop++)
        {
            length += son_get_vendor_specific_list_len(&p_son_phy_rssi_meas_cnf->vendor_specific_list[loop]);
        }
    }

    return length;
}

son_return_et
son_compose_son_phy_rssi_meas_cnf
(
    U8  **pp_buffer,
    son_phy_rssi_meas_cnf_t *p_son_phy_rssi_meas_cnf
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_phy_rssi_meas_cnf != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_rssi_meas_cnf->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_rssi_meas_cnf->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_rssi_meas_cnf->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_rssi_meas_cnf->num_vendor_specific_list, "num_vendor_specific_list");
    *pp_buffer += sizeof(p_son_phy_rssi_meas_cnf->num_vendor_specific_list);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_rssi_meas_cnf->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_compose_vendor_specific_list(pp_buffer, &p_son_phy_rssi_meas_cnf->vendor_specific_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_fapi_send_son_phy_rssi_meas_cnf
*
*   DESCRIPTION:
*       This function constructs and sends SON_PHY_RSSI_MEAS_CNF message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_fapi_send_son_phy_rssi_meas_cnf
(
    son_phy_rssi_meas_cnf_t  *p_son_phy_rssi_meas_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id  /* Source module identifier */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_fapi_msg;

    SON_ASSERT(p_son_phy_rssi_meas_cnf != SON_PNULL);

    /* Get API length */
    msg_length = son_get_son_phy_rssi_meas_cnf_len(p_son_phy_rssi_meas_cnf);

    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_INFO, "Src(%u)->SON_PHY_RSSI_MEAS_CNF", src_module_id);

   if(msg_length == SON_FAILURE)
	{
        return SON_FAILURE;
	}

    msg_api_length = msg_length + SON_API_HEADER_SIZE + SON_PHY_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_fapi_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_fapi_msg, SON_VERSION_ID, src_module_id,
        SON_FAPI_MODULE_ID, SON_PHY_RSSI_MEAS_CNF,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_fapi_msg = p_fapi_msg + SON_API_HEADER_SIZE;
        son_phy_construct_interface_api_header(p_fapi_msg, SON_PHY_RSSI_MEAS_CNF, SON_NULL, msg_length);

    /* Fill SON_PHY_RSSI_MEAS_CNF message */
    p_fapi_msg = p_fapi_msg + SON_PHY_INTERFACE_API_HEADER_SIZE;
    if (SON_FAILURE == son_compose_son_phy_rssi_meas_cnf(&p_fapi_msg, p_son_phy_rssi_meas_cnf))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    SON_ASSERT((p_fapi_msg - msg_api_length) == p_msg);

    /* Send message to */
    son_send_msg(p_msg, SON_FAPI_MODULE_ID);

    return SON_SUCCESS;
}
son_length_t
son_get_son_phy_stop_rssi_meas_req_len
(
    son_phy_stop_rssi_meas_req_t *p_son_phy_stop_rssi_meas_req
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_phy_stop_rssi_meas_req != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_stop_rssi_meas_req->num_vendor_specific_list);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_stop_rssi_meas_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_stop_rssi_meas_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_stop_rssi_meas_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_stop_rssi_meas_req->num_vendor_specific_list; loop++)
        {
            length += son_get_vendor_specific_list_len(&p_son_phy_stop_rssi_meas_req->vendor_specific_list[loop]);
        }
    }

    return length;
}

son_return_et
son_compose_son_phy_stop_rssi_meas_req
(
    U8  **pp_buffer,
    son_phy_stop_rssi_meas_req_t *p_son_phy_stop_rssi_meas_req
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_phy_stop_rssi_meas_req != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_stop_rssi_meas_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_stop_rssi_meas_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_stop_rssi_meas_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_stop_rssi_meas_req->num_vendor_specific_list, "num_vendor_specific_list");
    *pp_buffer += sizeof(p_son_phy_stop_rssi_meas_req->num_vendor_specific_list);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_stop_rssi_meas_req->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_compose_vendor_specific_list(pp_buffer, &p_son_phy_stop_rssi_meas_req->vendor_specific_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_fapi_send_son_phy_stop_rssi_meas_req
*
*   DESCRIPTION:
*       This function constructs and sends SON_PHY_STOP_MEAS_REQ message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_fapi_send_son_phy_stop_rssi_meas_req
(
    son_phy_stop_rssi_meas_req_t  *p_son_phy_stop_rssi_meas_req,           /* Pointer to API specific information. */
    U16                 src_module_id  /* Source module identifier */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_fapi_msg;

    SON_ASSERT(p_son_phy_stop_rssi_meas_req != SON_PNULL);

    /* Get API length */
    msg_length = son_get_son_phy_stop_rssi_meas_req_len(p_son_phy_stop_rssi_meas_req);

    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_INFO, "Src(%u)->SON_PHY_STOP_MEAS_REQ", src_module_id);

   if(msg_length == SON_FAILURE)
	{
        return SON_FAILURE;
	}

    msg_api_length = msg_length + SON_API_HEADER_SIZE + SON_PHY_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_fapi_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_fapi_msg, SON_VERSION_ID, src_module_id,
        SON_FAPI_MODULE_ID, SON_PHY_STOP_MEAS_REQ,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_fapi_msg = p_fapi_msg + SON_API_HEADER_SIZE;
        son_phy_construct_interface_api_header(p_fapi_msg, SON_PHY_STOP_MEAS_REQ, SON_NULL, msg_length);

    /* Fill SON_PHY_STOP_MEAS_REQ message */
    p_fapi_msg = p_fapi_msg + SON_PHY_INTERFACE_API_HEADER_SIZE;
    if (SON_FAILURE == son_compose_son_phy_stop_rssi_meas_req(&p_fapi_msg, p_son_phy_stop_rssi_meas_req))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    SON_ASSERT((p_fapi_msg - msg_api_length) == p_msg);

    /* Send message to */
    son_send_msg(p_msg, SON_FAPI_MODULE_ID);

    return SON_SUCCESS;
}
son_length_t
son_get_son_phy_cell_search_req_len
(
    son_phy_cell_search_req_t *p_son_phy_cell_search_req
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_phy_cell_search_req != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_cell_search_req->earfcn);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_cell_search_req->exhaustive_search);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_cell_search_req->num_pci_list);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_son_phy_cell_search_req->num_pci_list * sizeof(p_son_phy_cell_search_req->pci_list[0]));
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_cell_search_req->meas_bandwidth);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_cell_search_req->meas_period);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_cell_search_req->num_vendor_specific_list);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_cell_search_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_cell_search_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_cell_search_req->num_vendor_specific_list; loop++)
        {
            length += son_get_vendor_specific_list_len(&p_son_phy_cell_search_req->vendor_specific_list[loop]);
        }
    }

    return length;
}

son_return_et
son_compose_son_phy_cell_search_req
(
    U8  **pp_buffer,
    son_phy_cell_search_req_t *p_son_phy_cell_search_req
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_phy_cell_search_req != SON_PNULL);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_phy_cell_search_req->earfcn, "earfcn");
    *pp_buffer += sizeof(p_son_phy_cell_search_req->earfcn);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_cell_search_req->exhaustive_search > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_req->exhaustive_search] should be in range [%d <= 1] !", p_son_phy_cell_search_req->exhaustive_search);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_cell_search_req->exhaustive_search, "exhaustive_search");
    *pp_buffer += sizeof(p_son_phy_cell_search_req->exhaustive_search);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_cell_search_req->num_pci_list > 50))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_req->num_pci_list] should be in range [%d <= 50] !", p_son_phy_cell_search_req->num_pci_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_cell_search_req->num_pci_list, "num_pci_list");
    *pp_buffer += sizeof(p_son_phy_cell_search_req->num_pci_list);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_cell_search_req->num_pci_list; loop++)
        {
            son_cp_pack_U16(*pp_buffer, &p_son_phy_cell_search_req->pci_list[loop], "pci_list[]");
            *pp_buffer += sizeof(U16);
        }
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_phy_cell_search_req->meas_bandwidth < 6) || (p_son_phy_cell_search_req->meas_bandwidth > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_req->meas_bandwidth] should be in range [6 <= %d <= 100] !", p_son_phy_cell_search_req->meas_bandwidth);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_cell_search_req->meas_bandwidth, "meas_bandwidth");
    *pp_buffer += sizeof(p_son_phy_cell_search_req->meas_bandwidth);

    /* Check for correct range [L - lower boundary] */
    if ((p_son_phy_cell_search_req->meas_period < 1))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_req->meas_period] should be in range [1 <= %d] !", p_son_phy_cell_search_req->meas_period);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_cell_search_req->meas_period, "meas_period");
    *pp_buffer += sizeof(p_son_phy_cell_search_req->meas_period);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_cell_search_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_cell_search_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_cell_search_req->num_vendor_specific_list, "num_vendor_specific_list");
    *pp_buffer += sizeof(p_son_phy_cell_search_req->num_vendor_specific_list);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_cell_search_req->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_compose_vendor_specific_list(pp_buffer, &p_son_phy_cell_search_req->vendor_specific_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_fapi_send_son_phy_cell_search_req
*
*   DESCRIPTION:
*       This function constructs and sends SON_PHY_CELL_SEARCH_REQ message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_fapi_send_son_phy_cell_search_req
(
    son_phy_cell_search_req_t  *p_son_phy_cell_search_req,           /* Pointer to API specific information. */
    U16                 src_module_id  /* Source module identifier */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_fapi_msg;

    SON_ASSERT(p_son_phy_cell_search_req != SON_PNULL);

    /* Get API length */
    msg_length = son_get_son_phy_cell_search_req_len(p_son_phy_cell_search_req);

    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_INFO, "Src(%u)->SON_PHY_CELL_SEARCH_REQ", src_module_id);

   if(msg_length == SON_FAILURE)
	{
        return SON_FAILURE;
	}

    msg_api_length = msg_length + SON_API_HEADER_SIZE + SON_PHY_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_fapi_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_fapi_msg, SON_VERSION_ID, src_module_id,
        SON_FAPI_MODULE_ID, SON_PHY_CELL_SEARCH_REQ,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_fapi_msg = p_fapi_msg + SON_API_HEADER_SIZE;
        son_phy_construct_interface_api_header(p_fapi_msg, SON_PHY_CELL_SEARCH_REQ, SON_NULL, msg_length);

    /* Fill SON_PHY_CELL_SEARCH_REQ message */
    p_fapi_msg = p_fapi_msg + SON_PHY_INTERFACE_API_HEADER_SIZE;
    if (SON_FAILURE == son_compose_son_phy_cell_search_req(&p_fapi_msg, p_son_phy_cell_search_req))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    SON_ASSERT((p_fapi_msg - msg_api_length) == p_msg);

    /* Send message to */
    son_send_msg(p_msg, SON_FAPI_MODULE_ID);

    return SON_SUCCESS;
}
son_length_t
son_get_son_phy_cell_search_ind_len
(
    son_phy_cell_search_ind_t *p_son_phy_cell_search_ind
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_phy_cell_search_ind != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_cell_search_ind->num_pci);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_cell_search_ind->num_pci > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_ind->num_pci] should be in range [%d <= 16] !", p_son_phy_cell_search_ind->num_pci);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_cell_search_ind->num_pci; loop++)
        {
            length += son_get_cell_found_list_len(&p_son_phy_cell_search_ind->cell_found_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_cell_search_ind->num_vendor_specific_list);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_cell_search_ind->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_ind->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_cell_search_ind->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_cell_search_ind->num_vendor_specific_list; loop++)
        {
            length += son_get_vendor_specific_list_len(&p_son_phy_cell_search_ind->vendor_specific_list[loop]);
        }
    }

    return length;
}

son_return_et
son_compose_son_phy_cell_search_ind
(
    U8  **pp_buffer,
    son_phy_cell_search_ind_t *p_son_phy_cell_search_ind
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_phy_cell_search_ind != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_cell_search_ind->num_pci > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_ind->num_pci] should be in range [%d <= 16] !", p_son_phy_cell_search_ind->num_pci);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_cell_search_ind->num_pci, "num_pci");
    *pp_buffer += sizeof(p_son_phy_cell_search_ind->num_pci);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_cell_search_ind->num_pci; loop++)
        {
            if (SON_FAILURE == son_compose_cell_found_list(pp_buffer, &p_son_phy_cell_search_ind->cell_found_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_cell_search_ind->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_ind->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_cell_search_ind->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_cell_search_ind->num_vendor_specific_list, "num_vendor_specific_list");
    *pp_buffer += sizeof(p_son_phy_cell_search_ind->num_vendor_specific_list);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_cell_search_ind->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_compose_vendor_specific_list(pp_buffer, &p_son_phy_cell_search_ind->vendor_specific_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_fapi_send_son_phy_cell_search_ind
*
*   DESCRIPTION:
*       This function constructs and sends SON_PHY_CELL_SEARCH_IND message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_fapi_send_son_phy_cell_search_ind
(
    son_phy_cell_search_ind_t  *p_son_phy_cell_search_ind,           /* Pointer to API specific information. */
    U16                 src_module_id  /* Source module identifier */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_fapi_msg;

    SON_ASSERT(p_son_phy_cell_search_ind != SON_PNULL);

    /* Get API length */
    msg_length = son_get_son_phy_cell_search_ind_len(p_son_phy_cell_search_ind);

    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_INFO, "Src(%u)->SON_PHY_CELL_SEARCH_IND", src_module_id);

   if(msg_length == SON_FAILURE)
	{
        return SON_FAILURE;
	}

    msg_api_length = msg_length + SON_API_HEADER_SIZE + SON_PHY_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_fapi_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_fapi_msg, SON_VERSION_ID, src_module_id,
        SON_FAPI_MODULE_ID, SON_PHY_CELL_SEARCH_IND,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_fapi_msg = p_fapi_msg + SON_API_HEADER_SIZE;
        son_phy_construct_interface_api_header(p_fapi_msg, SON_PHY_CELL_SEARCH_IND, SON_NULL, msg_length);

    /* Fill SON_PHY_CELL_SEARCH_IND message */
    p_fapi_msg = p_fapi_msg + SON_PHY_INTERFACE_API_HEADER_SIZE;
    if (SON_FAILURE == son_compose_son_phy_cell_search_ind(&p_fapi_msg, p_son_phy_cell_search_ind))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    SON_ASSERT((p_fapi_msg - msg_api_length) == p_msg);

    /* Send message to */
    son_send_msg(p_msg, SON_FAPI_MODULE_ID);

    return SON_SUCCESS;
}
static
son_length_t
son_get_cell_found_list_len
(
    cell_found_list_t *p_cell_found_list
)
{
    son_length_t length = 0;

    SON_ASSERT(p_cell_found_list != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_cell_found_list->rsrp);
    /* Get length of parameter of basic type */
    length += sizeof(p_cell_found_list->pci);

    return length;
}

static
son_return_et
son_compose_cell_found_list
(
    U8  **pp_buffer,
    cell_found_list_t *p_cell_found_list
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_cell_found_list != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_cell_found_list->rsrp > 128))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_cell_found_list->rsrp] should be in range [%d <= 128] !", p_cell_found_list->rsrp);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_cell_found_list->rsrp, "rsrp");
    *pp_buffer += sizeof(p_cell_found_list->rsrp);

    /* Check for correct range [H - higher boundary] */
    if ((p_cell_found_list->pci > 503))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_cell_found_list->pci] should be in range [%d <= 503] !", p_cell_found_list->pci);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_cell_found_list->pci, "pci");
    *pp_buffer += sizeof(p_cell_found_list->pci);

    return SON_SUCCESS;
}

son_length_t
son_get_son_phy_cell_search_cnf_len
(
    son_phy_cell_search_cnf_t *p_son_phy_cell_search_cnf
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_phy_cell_search_cnf != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_cell_search_cnf->num_vendor_specific_list);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_cell_search_cnf->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_cnf->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_cell_search_cnf->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_cell_search_cnf->num_vendor_specific_list; loop++)
        {
            length += son_get_vendor_specific_list_len(&p_son_phy_cell_search_cnf->vendor_specific_list[loop]);
        }
    }

    return length;
}

son_return_et
son_compose_son_phy_cell_search_cnf
(
    U8  **pp_buffer,
    son_phy_cell_search_cnf_t *p_son_phy_cell_search_cnf
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_phy_cell_search_cnf != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_cell_search_cnf->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_cnf->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_cell_search_cnf->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_cell_search_cnf->num_vendor_specific_list, "num_vendor_specific_list");
    *pp_buffer += sizeof(p_son_phy_cell_search_cnf->num_vendor_specific_list);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_cell_search_cnf->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_compose_vendor_specific_list(pp_buffer, &p_son_phy_cell_search_cnf->vendor_specific_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_fapi_send_son_phy_cell_search_cnf
*
*   DESCRIPTION:
*       This function constructs and sends SON_PHY_CELL_SEARCH_CNF message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_fapi_send_son_phy_cell_search_cnf
(
    son_phy_cell_search_cnf_t  *p_son_phy_cell_search_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id  /* Source module identifier */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_fapi_msg;

    SON_ASSERT(p_son_phy_cell_search_cnf != SON_PNULL);

    /* Get API length */
    msg_length = son_get_son_phy_cell_search_cnf_len(p_son_phy_cell_search_cnf);

    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_INFO, "Src(%u)->SON_PHY_CELL_SEARCH_CNF", src_module_id);

   if(msg_length == SON_FAILURE)
	{
        return SON_FAILURE;
	}

    msg_api_length = msg_length + SON_API_HEADER_SIZE + SON_PHY_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_fapi_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_fapi_msg, SON_VERSION_ID, src_module_id,
        SON_FAPI_MODULE_ID, SON_PHY_CELL_SEARCH_CNF,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_fapi_msg = p_fapi_msg + SON_API_HEADER_SIZE;
        son_phy_construct_interface_api_header(p_fapi_msg, SON_PHY_CELL_SEARCH_CNF, SON_NULL, msg_length);

    /* Fill SON_PHY_CELL_SEARCH_CNF message */
    p_fapi_msg = p_fapi_msg + SON_PHY_INTERFACE_API_HEADER_SIZE;
    if (SON_FAILURE == son_compose_son_phy_cell_search_cnf(&p_fapi_msg, p_son_phy_cell_search_cnf))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    SON_ASSERT((p_fapi_msg - msg_api_length) == p_msg);

    /* Send message to */
    son_send_msg(p_msg, SON_FAPI_MODULE_ID);

    return SON_SUCCESS;
}
son_length_t
son_get_son_phy_stop_cell_search_req_len
(
    son_phy_stop_cell_search_req_t *p_son_phy_stop_cell_search_req
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_phy_stop_cell_search_req != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_stop_cell_search_req->num_vendor_specific_list);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_stop_cell_search_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_stop_cell_search_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_stop_cell_search_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_stop_cell_search_req->num_vendor_specific_list; loop++)
        {
            length += son_get_vendor_specific_list_len(&p_son_phy_stop_cell_search_req->vendor_specific_list[loop]);
        }
    }

    return length;
}

son_return_et
son_compose_son_phy_stop_cell_search_req
(
    U8  **pp_buffer,
    son_phy_stop_cell_search_req_t *p_son_phy_stop_cell_search_req
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_phy_stop_cell_search_req != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_stop_cell_search_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_stop_cell_search_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_stop_cell_search_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_stop_cell_search_req->num_vendor_specific_list, "num_vendor_specific_list");
    *pp_buffer += sizeof(p_son_phy_stop_cell_search_req->num_vendor_specific_list);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_stop_cell_search_req->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_compose_vendor_specific_list(pp_buffer, &p_son_phy_stop_cell_search_req->vendor_specific_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_fapi_send_son_phy_stop_cell_search_req
*
*   DESCRIPTION:
*       This function constructs and sends SON_PHY_STOP_CELL_SEARCH_REQ message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_fapi_send_son_phy_stop_cell_search_req
(
    son_phy_stop_cell_search_req_t  *p_son_phy_stop_cell_search_req,           /* Pointer to API specific information. */
    U16                 src_module_id  /* Source module identifier */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_fapi_msg;

    SON_ASSERT(p_son_phy_stop_cell_search_req != SON_PNULL);

    /* Get API length */
    msg_length = son_get_son_phy_stop_cell_search_req_len(p_son_phy_stop_cell_search_req);

    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_INFO, "Src(%u)->SON_PHY_STOP_CELL_SEARCH_REQ", src_module_id);

   if(msg_length == SON_FAILURE)
	{
        return SON_FAILURE;
	}

    msg_api_length = msg_length + SON_API_HEADER_SIZE + SON_PHY_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_fapi_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_fapi_msg, SON_VERSION_ID, src_module_id,
        SON_FAPI_MODULE_ID, SON_PHY_STOP_CELL_SEARCH_REQ,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_fapi_msg = p_fapi_msg + SON_API_HEADER_SIZE;
        son_phy_construct_interface_api_header(p_fapi_msg, SON_PHY_STOP_CELL_SEARCH_REQ, SON_NULL, msg_length);

    /* Fill SON_PHY_STOP_CELL_SEARCH_REQ message */
    p_fapi_msg = p_fapi_msg + SON_PHY_INTERFACE_API_HEADER_SIZE;
    if (SON_FAILURE == son_compose_son_phy_stop_cell_search_req(&p_fapi_msg, p_son_phy_stop_cell_search_req))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    SON_ASSERT((p_fapi_msg - msg_api_length) == p_msg);

    /* Send message to */
    son_send_msg(p_msg, SON_FAPI_MODULE_ID);

    return SON_SUCCESS;
}
son_length_t
son_get_son_phy_pbch_config_req_len
(
    son_phy_pbch_config_req_t *p_son_phy_pbch_config_req
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_phy_pbch_config_req != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_pbch_config_req->pci);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_pbch_config_req->earfcn);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_pbch_config_req->num_vendor_specific_list);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_config_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_config_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_pbch_config_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_pbch_config_req->num_vendor_specific_list; loop++)
        {
            length += son_get_vendor_specific_list_len(&p_son_phy_pbch_config_req->vendor_specific_list[loop]);
        }
    }

    return length;
}

son_return_et
son_compose_son_phy_pbch_config_req
(
    U8  **pp_buffer,
    son_phy_pbch_config_req_t *p_son_phy_pbch_config_req
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_phy_pbch_config_req != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_config_req->pci > 503))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_config_req->pci] should be in range [%d <= 503] !", p_son_phy_pbch_config_req->pci);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_phy_pbch_config_req->pci, "pci");
    *pp_buffer += sizeof(p_son_phy_pbch_config_req->pci);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_phy_pbch_config_req->earfcn, "earfcn");
    *pp_buffer += sizeof(p_son_phy_pbch_config_req->earfcn);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_config_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_config_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_pbch_config_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_pbch_config_req->num_vendor_specific_list, "num_vendor_specific_list");
    *pp_buffer += sizeof(p_son_phy_pbch_config_req->num_vendor_specific_list);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_pbch_config_req->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_compose_vendor_specific_list(pp_buffer, &p_son_phy_pbch_config_req->vendor_specific_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_fapi_send_son_phy_pbch_config_req
*
*   DESCRIPTION:
*       This function constructs and sends SON_PHY_PBCH_CONFIG_REQ message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_fapi_send_son_phy_pbch_config_req
(
    son_phy_pbch_config_req_t  *p_son_phy_pbch_config_req,           /* Pointer to API specific information. */
    U16                 src_module_id  /* Source module identifier */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_fapi_msg;

    SON_ASSERT(p_son_phy_pbch_config_req != SON_PNULL);

    /* Get API length */
    msg_length = son_get_son_phy_pbch_config_req_len(p_son_phy_pbch_config_req);

    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_INFO, "Src(%u)->SON_PHY_PBCH_CONFIG_REQ", src_module_id);

   if(msg_length == SON_FAILURE)
	{
        return SON_FAILURE;
	}

    msg_api_length = msg_length + SON_API_HEADER_SIZE + SON_PHY_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_fapi_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_fapi_msg, SON_VERSION_ID, src_module_id,
        SON_FAPI_MODULE_ID, SON_PHY_PBCH_CONFIG_REQ,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_fapi_msg = p_fapi_msg + SON_API_HEADER_SIZE;
        son_phy_construct_interface_api_header(p_fapi_msg, SON_PHY_PBCH_CONFIG_REQ, SON_NULL, msg_length);

    /* Fill SON_PHY_PBCH_CONFIG_REQ message */
    p_fapi_msg = p_fapi_msg + SON_PHY_INTERFACE_API_HEADER_SIZE;
    if (SON_FAILURE == son_compose_son_phy_pbch_config_req(&p_fapi_msg, p_son_phy_pbch_config_req))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    SON_ASSERT((p_fapi_msg - msg_api_length) == p_msg);

    /* Send message to */
    son_send_msg(p_msg, SON_FAPI_MODULE_ID);

    return SON_SUCCESS;
}
son_length_t
son_get_son_phy_pbch_config_cnf_len
(
    son_phy_pbch_config_cnf_t *p_son_phy_pbch_config_cnf
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_phy_pbch_config_cnf != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_pbch_config_cnf->num_vendor_specific_list);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_config_cnf->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_config_cnf->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_pbch_config_cnf->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_pbch_config_cnf->num_vendor_specific_list; loop++)
        {
            length += son_get_vendor_specific_list_len(&p_son_phy_pbch_config_cnf->vendor_specific_list[loop]);
        }
    }

    return length;
}

son_return_et
son_compose_son_phy_pbch_config_cnf
(
    U8  **pp_buffer,
    son_phy_pbch_config_cnf_t *p_son_phy_pbch_config_cnf
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_phy_pbch_config_cnf != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_config_cnf->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_config_cnf->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_pbch_config_cnf->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_pbch_config_cnf->num_vendor_specific_list, "num_vendor_specific_list");
    *pp_buffer += sizeof(p_son_phy_pbch_config_cnf->num_vendor_specific_list);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_pbch_config_cnf->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_compose_vendor_specific_list(pp_buffer, &p_son_phy_pbch_config_cnf->vendor_specific_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_fapi_send_son_phy_pbch_config_cnf
*
*   DESCRIPTION:
*       This function constructs and sends SON_PHY_PBCH_CONFIG_CNF message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_fapi_send_son_phy_pbch_config_cnf
(
    son_phy_pbch_config_cnf_t  *p_son_phy_pbch_config_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id  /* Source module identifier */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_fapi_msg;

    SON_ASSERT(p_son_phy_pbch_config_cnf != SON_PNULL);

    /* Get API length */
    msg_length = son_get_son_phy_pbch_config_cnf_len(p_son_phy_pbch_config_cnf);

    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_INFO, "Src(%u)->SON_PHY_PBCH_CONFIG_CNF", src_module_id);

   if(msg_length == SON_FAILURE)
	{
        return SON_FAILURE;
	}

    msg_api_length = msg_length + SON_API_HEADER_SIZE + SON_PHY_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_fapi_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_fapi_msg, SON_VERSION_ID, src_module_id,
        SON_FAPI_MODULE_ID, SON_PHY_PBCH_CONFIG_CNF,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_fapi_msg = p_fapi_msg + SON_API_HEADER_SIZE;
        son_phy_construct_interface_api_header(p_fapi_msg, SON_PHY_PBCH_CONFIG_CNF, SON_NULL, msg_length);

    /* Fill SON_PHY_PBCH_CONFIG_CNF message */
    p_fapi_msg = p_fapi_msg + SON_PHY_INTERFACE_API_HEADER_SIZE;
    if (SON_FAILURE == son_compose_son_phy_pbch_config_cnf(&p_fapi_msg, p_son_phy_pbch_config_cnf))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    SON_ASSERT((p_fapi_msg - msg_api_length) == p_msg);

    /* Send message to */
    son_send_msg(p_msg, SON_FAPI_MODULE_ID);

    return SON_SUCCESS;
}
son_length_t
son_get_son_phy_pbch_data_ind_len
(
    son_phy_pbch_data_ind_t *p_son_phy_pbch_data_ind
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_phy_pbch_data_ind != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_pbch_data_ind->pci);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_pbch_data_ind->cause);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_pbch_data_ind->num_antenna);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_pbch_data_ind->sfn);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_pbch_data_ind->phich_duration);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_pbch_data_ind->phich_resource);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_pbch_data_ind->dl_sys_bandwidth);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_pbch_data_ind->num_vendor_specific_list);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_data_ind->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_data_ind->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_pbch_data_ind->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_pbch_data_ind->num_vendor_specific_list; loop++)
        {
            length += son_get_vendor_specific_list_len(&p_son_phy_pbch_data_ind->vendor_specific_list[loop]);
        }
    }

    return length;
}

son_return_et
son_compose_son_phy_pbch_data_ind
(
    U8  **pp_buffer,
    son_phy_pbch_data_ind_t *p_son_phy_pbch_data_ind
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_phy_pbch_data_ind != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_data_ind->pci > 503))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_data_ind->pci] should be in range [%d <= 503] !", p_son_phy_pbch_data_ind->pci);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_phy_pbch_data_ind->pci, "pci");
    *pp_buffer += sizeof(p_son_phy_pbch_data_ind->pci);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_data_ind->cause > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_data_ind->cause] should be in range [%d <= 1] !", p_son_phy_pbch_data_ind->cause);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_phy_pbch_data_ind->cause, "cause");
    *pp_buffer += sizeof(p_son_phy_pbch_data_ind->cause);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_phy_pbch_data_ind->num_antenna < 1) || (p_son_phy_pbch_data_ind->num_antenna > 4))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_data_ind->num_antenna] should be in range [1 <= %d <= 4] !", p_son_phy_pbch_data_ind->num_antenna);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_pbch_data_ind->num_antenna, "num_antenna");
    *pp_buffer += sizeof(p_son_phy_pbch_data_ind->num_antenna);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_data_ind->sfn > 1023))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_data_ind->sfn] should be in range [%d <= 1023] !", p_son_phy_pbch_data_ind->sfn);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_phy_pbch_data_ind->sfn, "sfn");
    *pp_buffer += sizeof(p_son_phy_pbch_data_ind->sfn);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_data_ind->phich_duration > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_data_ind->phich_duration] should be in range [%d <= 1] !", p_son_phy_pbch_data_ind->phich_duration);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_phy_pbch_data_ind->phich_duration, "phich_duration");
    *pp_buffer += sizeof(p_son_phy_pbch_data_ind->phich_duration);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_data_ind->phich_resource > 3))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_data_ind->phich_resource] should be in range [%d <= 3] !", p_son_phy_pbch_data_ind->phich_resource);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_phy_pbch_data_ind->phich_resource, "phich_resource");
    *pp_buffer += sizeof(p_son_phy_pbch_data_ind->phich_resource);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_phy_pbch_data_ind->dl_sys_bandwidth < 6) || (p_son_phy_pbch_data_ind->dl_sys_bandwidth > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_data_ind->dl_sys_bandwidth] should be in range [6 <= %d <= 100] !", p_son_phy_pbch_data_ind->dl_sys_bandwidth);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_pbch_data_ind->dl_sys_bandwidth, "dl_sys_bandwidth");
    *pp_buffer += sizeof(p_son_phy_pbch_data_ind->dl_sys_bandwidth);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_data_ind->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_data_ind->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_pbch_data_ind->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_pbch_data_ind->num_vendor_specific_list, "num_vendor_specific_list");
    *pp_buffer += sizeof(p_son_phy_pbch_data_ind->num_vendor_specific_list);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_pbch_data_ind->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_compose_vendor_specific_list(pp_buffer, &p_son_phy_pbch_data_ind->vendor_specific_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_fapi_send_son_phy_pbch_data_ind
*
*   DESCRIPTION:
*       This function constructs and sends SON_PHY_PBCH_DATA_IND message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_fapi_send_son_phy_pbch_data_ind
(
    son_phy_pbch_data_ind_t  *p_son_phy_pbch_data_ind,           /* Pointer to API specific information. */
    U16                 src_module_id  /* Source module identifier */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_fapi_msg;

    SON_ASSERT(p_son_phy_pbch_data_ind != SON_PNULL);

    /* Get API length */
    msg_length = son_get_son_phy_pbch_data_ind_len(p_son_phy_pbch_data_ind);

    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_INFO, "Src(%u)->SON_PHY_PBCH_DATA_IND", src_module_id);

   if(msg_length == SON_FAILURE)
	{
        return SON_FAILURE;
	}

    msg_api_length = msg_length + SON_API_HEADER_SIZE + SON_PHY_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_fapi_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_fapi_msg, SON_VERSION_ID, src_module_id,
        SON_FAPI_MODULE_ID, SON_PHY_PBCH_DATA_IND,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_fapi_msg = p_fapi_msg + SON_API_HEADER_SIZE;
        son_phy_construct_interface_api_header(p_fapi_msg, SON_PHY_PBCH_DATA_IND, SON_NULL, msg_length);

    /* Fill SON_PHY_PBCH_DATA_IND message */
    p_fapi_msg = p_fapi_msg + SON_PHY_INTERFACE_API_HEADER_SIZE;
    if (SON_FAILURE == son_compose_son_phy_pbch_data_ind(&p_fapi_msg, p_son_phy_pbch_data_ind))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    SON_ASSERT((p_fapi_msg - msg_api_length) == p_msg);

    /* Send message to */
    son_send_msg(p_msg, SON_FAPI_MODULE_ID);

    return SON_SUCCESS;
}
son_length_t
son_get_son_phy_stop_pbch_req_len
(
    son_phy_stop_pbch_req_t *p_son_phy_stop_pbch_req
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_phy_stop_pbch_req != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_stop_pbch_req->num_vendor_specific_list);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_stop_pbch_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_stop_pbch_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_stop_pbch_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_stop_pbch_req->num_vendor_specific_list; loop++)
        {
            length += son_get_vendor_specific_list_len(&p_son_phy_stop_pbch_req->vendor_specific_list[loop]);
        }
    }

    return length;
}

son_return_et
son_compose_son_phy_stop_pbch_req
(
    U8  **pp_buffer,
    son_phy_stop_pbch_req_t *p_son_phy_stop_pbch_req
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_phy_stop_pbch_req != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_stop_pbch_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_stop_pbch_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_stop_pbch_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_stop_pbch_req->num_vendor_specific_list, "num_vendor_specific_list");
    *pp_buffer += sizeof(p_son_phy_stop_pbch_req->num_vendor_specific_list);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_stop_pbch_req->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_compose_vendor_specific_list(pp_buffer, &p_son_phy_stop_pbch_req->vendor_specific_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_fapi_send_son_phy_stop_pbch_req
*
*   DESCRIPTION:
*       This function constructs and sends SON_PHY_STOP_PBCH_REQ message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_fapi_send_son_phy_stop_pbch_req
(
    son_phy_stop_pbch_req_t  *p_son_phy_stop_pbch_req,           /* Pointer to API specific information. */
    U16                 src_module_id  /* Source module identifier */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_fapi_msg;

    SON_ASSERT(p_son_phy_stop_pbch_req != SON_PNULL);

    /* Get API length */
    msg_length = son_get_son_phy_stop_pbch_req_len(p_son_phy_stop_pbch_req);

    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_INFO, "Src(%u)->SON_PHY_STOP_PBCH_REQ", src_module_id);

   if(msg_length == SON_FAILURE)
	{
        return SON_FAILURE;
	}

    msg_api_length = msg_length + SON_API_HEADER_SIZE + SON_PHY_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_fapi_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_fapi_msg, SON_VERSION_ID, src_module_id,
        SON_FAPI_MODULE_ID, SON_PHY_STOP_PBCH_REQ,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_fapi_msg = p_fapi_msg + SON_API_HEADER_SIZE;
        son_phy_construct_interface_api_header(p_fapi_msg, SON_PHY_STOP_PBCH_REQ, SON_NULL, msg_length);

    /* Fill SON_PHY_STOP_PBCH_REQ message */
    p_fapi_msg = p_fapi_msg + SON_PHY_INTERFACE_API_HEADER_SIZE;
    if (SON_FAILURE == son_compose_son_phy_stop_pbch_req(&p_fapi_msg, p_son_phy_stop_pbch_req))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    SON_ASSERT((p_fapi_msg - msg_api_length) == p_msg);

    /* Send message to */
    son_send_msg(p_msg, SON_FAPI_MODULE_ID);

    return SON_SUCCESS;
}
son_length_t
son_get_son_phy_sib1_config_req_len
(
    son_phy_sib1_config_req_t *p_son_phy_sib1_config_req
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_phy_sib1_config_req != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_sib1_config_req->retry_count);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_sib1_config_req->num_vendor_specific_list);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_sib1_config_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_sib1_config_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_sib1_config_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_sib1_config_req->num_vendor_specific_list; loop++)
        {
            length += son_get_vendor_specific_list_len(&p_son_phy_sib1_config_req->vendor_specific_list[loop]);
        }
    }

    return length;
}

son_return_et
son_compose_son_phy_sib1_config_req
(
    U8  **pp_buffer,
    son_phy_sib1_config_req_t *p_son_phy_sib1_config_req
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_phy_sib1_config_req != SON_PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_phy_sib1_config_req->retry_count < 1) || (p_son_phy_sib1_config_req->retry_count > 8))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_sib1_config_req->retry_count] should be in range [1 <= %d <= 8] !", p_son_phy_sib1_config_req->retry_count);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_sib1_config_req->retry_count, "retry_count");
    *pp_buffer += sizeof(p_son_phy_sib1_config_req->retry_count);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_sib1_config_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_sib1_config_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_sib1_config_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_sib1_config_req->num_vendor_specific_list, "num_vendor_specific_list");
    *pp_buffer += sizeof(p_son_phy_sib1_config_req->num_vendor_specific_list);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_sib1_config_req->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_compose_vendor_specific_list(pp_buffer, &p_son_phy_sib1_config_req->vendor_specific_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_fapi_send_son_phy_sib1_config_req
*
*   DESCRIPTION:
*       This function constructs and sends SON_PHY_SIB1_CONFIG_REQ message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_fapi_send_son_phy_sib1_config_req
(
    son_phy_sib1_config_req_t  *p_son_phy_sib1_config_req,           /* Pointer to API specific information. */
    U16                 src_module_id  /* Source module identifier */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_fapi_msg;

    SON_ASSERT(p_son_phy_sib1_config_req != SON_PNULL);

    /* Get API length */
    msg_length = son_get_son_phy_sib1_config_req_len(p_son_phy_sib1_config_req);

    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_INFO, "Src(%u)->SON_PHY_SIB1_CONFIG_REQ", src_module_id);

   if(msg_length == SON_FAILURE)
	{
        return SON_FAILURE;
	}

    msg_api_length = msg_length + SON_API_HEADER_SIZE + SON_PHY_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_fapi_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_fapi_msg, SON_VERSION_ID, src_module_id,
        SON_FAPI_MODULE_ID, SON_PHY_SIB1_CONFIG_REQ,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_fapi_msg = p_fapi_msg + SON_API_HEADER_SIZE;
        son_phy_construct_interface_api_header(p_fapi_msg, SON_PHY_SIB1_CONFIG_REQ, SON_NULL, msg_length);

    /* Fill SON_PHY_SIB1_CONFIG_REQ message */
    p_fapi_msg = p_fapi_msg + SON_PHY_INTERFACE_API_HEADER_SIZE;
    if (SON_FAILURE == son_compose_son_phy_sib1_config_req(&p_fapi_msg, p_son_phy_sib1_config_req))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    SON_ASSERT((p_fapi_msg - msg_api_length) == p_msg);

    /* Send message to */
    son_send_msg(p_msg, SON_FAPI_MODULE_ID);

    return SON_SUCCESS;
}
son_length_t
son_get_son_phy_sib1_config_cnf_len
(
    son_phy_sib1_config_cnf_t *p_son_phy_sib1_config_cnf
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_phy_sib1_config_cnf != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_sib1_config_cnf->num_vendor_specific_list);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_sib1_config_cnf->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_sib1_config_cnf->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_sib1_config_cnf->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_sib1_config_cnf->num_vendor_specific_list; loop++)
        {
            length += son_get_vendor_specific_list_len(&p_son_phy_sib1_config_cnf->vendor_specific_list[loop]);
        }
    }

    return length;
}

son_return_et
son_compose_son_phy_sib1_config_cnf
(
    U8  **pp_buffer,
    son_phy_sib1_config_cnf_t *p_son_phy_sib1_config_cnf
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_phy_sib1_config_cnf != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_sib1_config_cnf->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_sib1_config_cnf->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_sib1_config_cnf->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_sib1_config_cnf->num_vendor_specific_list, "num_vendor_specific_list");
    *pp_buffer += sizeof(p_son_phy_sib1_config_cnf->num_vendor_specific_list);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_sib1_config_cnf->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_compose_vendor_specific_list(pp_buffer, &p_son_phy_sib1_config_cnf->vendor_specific_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_fapi_send_son_phy_sib1_config_cnf
*
*   DESCRIPTION:
*       This function constructs and sends SON_PHY_SIB1_CONFIG_CNF message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_fapi_send_son_phy_sib1_config_cnf
(
    son_phy_sib1_config_cnf_t  *p_son_phy_sib1_config_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id  /* Source module identifier */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_fapi_msg;

    SON_ASSERT(p_son_phy_sib1_config_cnf != SON_PNULL);

    /* Get API length */
    msg_length = son_get_son_phy_sib1_config_cnf_len(p_son_phy_sib1_config_cnf);

    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_INFO, "Src(%u)->SON_PHY_SIB1_CONFIG_CNF", src_module_id);

   if(msg_length == SON_FAILURE)
	{
        return SON_FAILURE;
	}

    msg_api_length = msg_length + SON_API_HEADER_SIZE + SON_PHY_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_fapi_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_fapi_msg, SON_VERSION_ID, src_module_id,
        SON_FAPI_MODULE_ID, SON_PHY_SIB1_CONFIG_CNF,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_fapi_msg = p_fapi_msg + SON_API_HEADER_SIZE;
        son_phy_construct_interface_api_header(p_fapi_msg, SON_PHY_SIB1_CONFIG_CNF, SON_NULL, msg_length);

    /* Fill SON_PHY_SIB1_CONFIG_CNF message */
    p_fapi_msg = p_fapi_msg + SON_PHY_INTERFACE_API_HEADER_SIZE;
    if (SON_FAILURE == son_compose_son_phy_sib1_config_cnf(&p_fapi_msg, p_son_phy_sib1_config_cnf))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    SON_ASSERT((p_fapi_msg - msg_api_length) == p_msg);

    /* Send message to */
    son_send_msg(p_msg, SON_FAPI_MODULE_ID);

    return SON_SUCCESS;
}
son_length_t
son_get_son_phy_bcch_config_req_len
(
    son_phy_bcch_config_req_t *p_son_phy_bcch_config_req
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_phy_bcch_config_req != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_bcch_config_req->num_si_periodicity_list);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_son_phy_bcch_config_req->num_si_periodicity_list * sizeof(p_son_phy_bcch_config_req->si_periodicity_list[0]));
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_bcch_config_req->si_window);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_bcch_config_req->retry_count);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_bcch_config_req->sub_frame_assignment);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_bcch_config_req->special_sub_frame_patterns);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_bcch_config_req->num_vendor_specific_list);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_bcch_config_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_config_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_bcch_config_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_bcch_config_req->num_vendor_specific_list; loop++)
        {
            length += son_get_vendor_specific_list_len(&p_son_phy_bcch_config_req->vendor_specific_list[loop]);
        }
    }

    return length;
}

son_return_et
son_compose_son_phy_bcch_config_req
(
    U8  **pp_buffer,
    son_phy_bcch_config_req_t *p_son_phy_bcch_config_req
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_phy_bcch_config_req != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_bcch_config_req->num_si_periodicity_list > 32))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_config_req->num_si_periodicity_list] should be in range [%d <= 32] !", p_son_phy_bcch_config_req->num_si_periodicity_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_bcch_config_req->num_si_periodicity_list, "num_si_periodicity_list");
    *pp_buffer += sizeof(p_son_phy_bcch_config_req->num_si_periodicity_list);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_bcch_config_req->num_si_periodicity_list; loop++)
        {
            son_cp_pack_U16(*pp_buffer, &p_son_phy_bcch_config_req->si_periodicity_list[loop], "si_periodicity_list[]");
            *pp_buffer += sizeof(U16);
        }
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_phy_bcch_config_req->si_window < 1) || (p_son_phy_bcch_config_req->si_window > 40))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_config_req->si_window] should be in range [1 <= %d <= 40] !", p_son_phy_bcch_config_req->si_window);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_bcch_config_req->si_window, "si_window");
    *pp_buffer += sizeof(p_son_phy_bcch_config_req->si_window);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_phy_bcch_config_req->retry_count < 1) || (p_son_phy_bcch_config_req->retry_count > 8))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_config_req->retry_count] should be in range [1 <= %d <= 8] !", p_son_phy_bcch_config_req->retry_count);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_bcch_config_req->retry_count, "retry_count");
    *pp_buffer += sizeof(p_son_phy_bcch_config_req->retry_count);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_bcch_config_req->sub_frame_assignment > 6))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_config_req->sub_frame_assignment] should be in range [%d <= 6] !", p_son_phy_bcch_config_req->sub_frame_assignment);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_bcch_config_req->sub_frame_assignment, "sub_frame_assignment");
    *pp_buffer += sizeof(p_son_phy_bcch_config_req->sub_frame_assignment);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_bcch_config_req->special_sub_frame_patterns > 8))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_config_req->special_sub_frame_patterns] should be in range [%d <= 8] !", p_son_phy_bcch_config_req->special_sub_frame_patterns);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_bcch_config_req->special_sub_frame_patterns, "special_sub_frame_patterns");
    *pp_buffer += sizeof(p_son_phy_bcch_config_req->special_sub_frame_patterns);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_bcch_config_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_config_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_bcch_config_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_bcch_config_req->num_vendor_specific_list, "num_vendor_specific_list");
    *pp_buffer += sizeof(p_son_phy_bcch_config_req->num_vendor_specific_list);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_bcch_config_req->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_compose_vendor_specific_list(pp_buffer, &p_son_phy_bcch_config_req->vendor_specific_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_fapi_send_son_phy_bcch_config_req
*
*   DESCRIPTION:
*       This function constructs and sends SON_PHY_BCCH_CONFIG_REQ message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_fapi_send_son_phy_bcch_config_req
(
    son_phy_bcch_config_req_t  *p_son_phy_bcch_config_req,           /* Pointer to API specific information. */
    U16                 src_module_id  /* Source module identifier */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_fapi_msg;

    SON_ASSERT(p_son_phy_bcch_config_req != SON_PNULL);

    /* Get API length */
    msg_length = son_get_son_phy_bcch_config_req_len(p_son_phy_bcch_config_req);

    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_INFO, "Src(%u)->SON_PHY_BCCH_CONFIG_REQ", src_module_id);

   if(msg_length == SON_FAILURE)
	{
        return SON_FAILURE;
	}

    msg_api_length = msg_length + SON_API_HEADER_SIZE + SON_PHY_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_fapi_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_fapi_msg, SON_VERSION_ID, src_module_id,
        SON_FAPI_MODULE_ID, SON_PHY_BCCH_CONFIG_REQ,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_fapi_msg = p_fapi_msg + SON_API_HEADER_SIZE;
        son_phy_construct_interface_api_header(p_fapi_msg, SON_PHY_BCCH_CONFIG_REQ, SON_NULL, msg_length);

    /* Fill SON_PHY_BCCH_CONFIG_REQ message */
    p_fapi_msg = p_fapi_msg + SON_PHY_INTERFACE_API_HEADER_SIZE;
    if (SON_FAILURE == son_compose_son_phy_bcch_config_req(&p_fapi_msg, p_son_phy_bcch_config_req))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    SON_ASSERT((p_fapi_msg - msg_api_length) == p_msg);

    /* Send message to */
    son_send_msg(p_msg, SON_FAPI_MODULE_ID);

    return SON_SUCCESS;
}
son_length_t
son_get_son_phy_bcch_config_cnf_len
(
    son_phy_bcch_config_cnf_t *p_son_phy_bcch_config_cnf
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_phy_bcch_config_cnf != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_bcch_config_cnf->num_vendor_specific_list);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_bcch_config_cnf->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_config_cnf->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_bcch_config_cnf->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_bcch_config_cnf->num_vendor_specific_list; loop++)
        {
            length += son_get_vendor_specific_list_len(&p_son_phy_bcch_config_cnf->vendor_specific_list[loop]);
        }
    }

    return length;
}

son_return_et
son_compose_son_phy_bcch_config_cnf
(
    U8  **pp_buffer,
    son_phy_bcch_config_cnf_t *p_son_phy_bcch_config_cnf
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_phy_bcch_config_cnf != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_bcch_config_cnf->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_config_cnf->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_bcch_config_cnf->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_bcch_config_cnf->num_vendor_specific_list, "num_vendor_specific_list");
    *pp_buffer += sizeof(p_son_phy_bcch_config_cnf->num_vendor_specific_list);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_bcch_config_cnf->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_compose_vendor_specific_list(pp_buffer, &p_son_phy_bcch_config_cnf->vendor_specific_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_fapi_send_son_phy_bcch_config_cnf
*
*   DESCRIPTION:
*       This function constructs and sends SON_PHY_BCCH_CONFIG_CNF message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_fapi_send_son_phy_bcch_config_cnf
(
    son_phy_bcch_config_cnf_t  *p_son_phy_bcch_config_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id  /* Source module identifier */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_fapi_msg;

    SON_ASSERT(p_son_phy_bcch_config_cnf != SON_PNULL);

    /* Get API length */
    msg_length = son_get_son_phy_bcch_config_cnf_len(p_son_phy_bcch_config_cnf);

    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_INFO, "Src(%u)->SON_PHY_BCCH_CONFIG_CNF", src_module_id);

   if(msg_length == SON_FAILURE)
	{
        return SON_FAILURE;
	}

    msg_api_length = msg_length + SON_API_HEADER_SIZE + SON_PHY_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_fapi_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_fapi_msg, SON_VERSION_ID, src_module_id,
        SON_FAPI_MODULE_ID, SON_PHY_BCCH_CONFIG_CNF,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_fapi_msg = p_fapi_msg + SON_API_HEADER_SIZE;
        son_phy_construct_interface_api_header(p_fapi_msg, SON_PHY_BCCH_CONFIG_CNF, SON_NULL, msg_length);

    /* Fill SON_PHY_BCCH_CONFIG_CNF message */
    p_fapi_msg = p_fapi_msg + SON_PHY_INTERFACE_API_HEADER_SIZE;
    if (SON_FAILURE == son_compose_son_phy_bcch_config_cnf(&p_fapi_msg, p_son_phy_bcch_config_cnf))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    SON_ASSERT((p_fapi_msg - msg_api_length) == p_msg);

    /* Send message to */
    son_send_msg(p_msg, SON_FAPI_MODULE_ID);

    return SON_SUCCESS;
}
son_length_t
son_get_son_phy_bcch_data_ind_len
(
    son_phy_bcch_data_ind_t *p_son_phy_bcch_data_ind
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_phy_bcch_data_ind != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_bcch_data_ind->num_bcch_data_list);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_bcch_data_ind->num_bcch_data_list > 32))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_data_ind->num_bcch_data_list] should be in range [%d <= 32] !", p_son_phy_bcch_data_ind->num_bcch_data_list);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_bcch_data_ind->num_bcch_data_list; loop++)
        {
            length += son_get_sib_info_len(&p_son_phy_bcch_data_ind->sib_info[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_bcch_data_ind->num_vendor_specific_list);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_bcch_data_ind->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_data_ind->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_bcch_data_ind->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_bcch_data_ind->num_vendor_specific_list; loop++)
        {
            length += son_get_vendor_specific_list_len(&p_son_phy_bcch_data_ind->vendor_specific_list[loop]);
        }
    }

    return length;
}

son_return_et
son_compose_son_phy_bcch_data_ind
(
    U8  **pp_buffer,
    son_phy_bcch_data_ind_t *p_son_phy_bcch_data_ind
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_phy_bcch_data_ind != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_bcch_data_ind->num_bcch_data_list > 32))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_data_ind->num_bcch_data_list] should be in range [%d <= 32] !", p_son_phy_bcch_data_ind->num_bcch_data_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_bcch_data_ind->num_bcch_data_list, "num_bcch_data_list");
    *pp_buffer += sizeof(p_son_phy_bcch_data_ind->num_bcch_data_list);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_bcch_data_ind->num_bcch_data_list; loop++)
        {
            if (SON_FAILURE == son_compose_sib_info(pp_buffer, &p_son_phy_bcch_data_ind->sib_info[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_bcch_data_ind->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_data_ind->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_bcch_data_ind->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_bcch_data_ind->num_vendor_specific_list, "num_vendor_specific_list");
    *pp_buffer += sizeof(p_son_phy_bcch_data_ind->num_vendor_specific_list);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_bcch_data_ind->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_compose_vendor_specific_list(pp_buffer, &p_son_phy_bcch_data_ind->vendor_specific_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_fapi_send_son_phy_bcch_data_ind
*
*   DESCRIPTION:
*       This function constructs and sends SON_PHY_BCCH_DATA_IND message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_fapi_send_son_phy_bcch_data_ind
(
    son_phy_bcch_data_ind_t  *p_son_phy_bcch_data_ind,           /* Pointer to API specific information. */
    U16                 src_module_id  /* Source module identifier */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_fapi_msg;

    SON_ASSERT(p_son_phy_bcch_data_ind != SON_PNULL);

    /* Get API length */
    msg_length = son_get_son_phy_bcch_data_ind_len(p_son_phy_bcch_data_ind);

    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_INFO, "Src(%u)->SON_PHY_BCCH_DATA_IND", src_module_id);

   if(msg_length == SON_FAILURE)
	{
        return SON_FAILURE;
	}

    msg_api_length = msg_length + SON_API_HEADER_SIZE + SON_PHY_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_fapi_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_fapi_msg, SON_VERSION_ID, src_module_id,
        SON_FAPI_MODULE_ID, SON_PHY_BCCH_DATA_IND,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_fapi_msg = p_fapi_msg + SON_API_HEADER_SIZE;
        son_phy_construct_interface_api_header(p_fapi_msg, SON_PHY_BCCH_DATA_IND, SON_NULL, msg_length);

    /* Fill SON_PHY_BCCH_DATA_IND message */
    p_fapi_msg = p_fapi_msg + SON_PHY_INTERFACE_API_HEADER_SIZE;
    if (SON_FAILURE == son_compose_son_phy_bcch_data_ind(&p_fapi_msg, p_son_phy_bcch_data_ind))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    SON_ASSERT((p_fapi_msg - msg_api_length) == p_msg);

    /* Send message to */
    son_send_msg(p_msg, SON_FAPI_MODULE_ID);

    return SON_SUCCESS;
}
static
son_length_t
son_get_sib_info_len
(
    sib_info_t *p_sib_info
)
{
    son_length_t length = 0;

    SON_ASSERT(p_sib_info != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_sib_info->sfn);
    /* Get length of parameter of basic type */
    length += sizeof(p_sib_info->sf);
    /* Get length of parameter of basic type */
    length += sizeof(p_sib_info->len);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_sib_info->len * sizeof(p_sib_info->data[0]));

    return length;
}

static
son_return_et
son_compose_sib_info
(
    U8  **pp_buffer,
    sib_info_t *p_sib_info
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_sib_info != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_sib_info->sfn > 1023))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_sib_info->sfn] should be in range [%d <= 1023] !", p_sib_info->sfn);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_sib_info->sfn, "sfn");
    *pp_buffer += sizeof(p_sib_info->sfn);

    /* Check for correct range [H - higher boundary] */
    if ((p_sib_info->sf > 9))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_sib_info->sf] should be in range [%d <= 9] !", p_sib_info->sf);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_sib_info->sf, "sf");
    *pp_buffer += sizeof(p_sib_info->sf);

    /* Check for correct range [H - higher boundary] */
    if ((p_sib_info->len > 2048))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_sib_info->len] should be in range [%d <= 2048] !", p_sib_info->len);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_sib_info->len, "len");
    *pp_buffer += sizeof(p_sib_info->len);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_sib_info->len; loop++)
        {
            son_cp_pack_U8(*pp_buffer, &p_sib_info->data[loop], "data[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return SON_SUCCESS;
}

son_length_t
son_get_son_phy_stop_bcch_req_len
(
    son_phy_stop_bcch_req_t *p_son_phy_stop_bcch_req
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_phy_stop_bcch_req != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_phy_stop_bcch_req->num_vendor_specific_list);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_stop_bcch_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_stop_bcch_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_stop_bcch_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_stop_bcch_req->num_vendor_specific_list; loop++)
        {
            length += son_get_vendor_specific_list_len(&p_son_phy_stop_bcch_req->vendor_specific_list[loop]);
        }
    }

    return length;
}

son_return_et
son_compose_son_phy_stop_bcch_req
(
    U8  **pp_buffer,
    son_phy_stop_bcch_req_t *p_son_phy_stop_bcch_req
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_phy_stop_bcch_req != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_stop_bcch_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_stop_bcch_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_stop_bcch_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_phy_stop_bcch_req->num_vendor_specific_list, "num_vendor_specific_list");
    *pp_buffer += sizeof(p_son_phy_stop_bcch_req->num_vendor_specific_list);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_stop_bcch_req->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_compose_vendor_specific_list(pp_buffer, &p_son_phy_stop_bcch_req->vendor_specific_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_fapi_send_son_phy_stop_bcch_req
*
*   DESCRIPTION:
*       This function constructs and sends SON_PHY_STOP_BCCH_REQ message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_fapi_send_son_phy_stop_bcch_req
(
    son_phy_stop_bcch_req_t  *p_son_phy_stop_bcch_req,           /* Pointer to API specific information. */
    U16                 src_module_id  /* Source module identifier */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_fapi_msg;

    SON_ASSERT(p_son_phy_stop_bcch_req != SON_PNULL);

    /* Get API length */
    msg_length = son_get_son_phy_stop_bcch_req_len(p_son_phy_stop_bcch_req);

    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_INFO, "Src(%u)->SON_PHY_STOP_BCCH_REQ", src_module_id);

   if(msg_length == SON_FAILURE)
	{
        return SON_FAILURE;
	}

    msg_api_length = msg_length + SON_API_HEADER_SIZE + SON_PHY_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_fapi_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_fapi_msg, SON_VERSION_ID, src_module_id,
        SON_FAPI_MODULE_ID, SON_PHY_STOP_BCCH_REQ,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_fapi_msg = p_fapi_msg + SON_API_HEADER_SIZE;
        son_phy_construct_interface_api_header(p_fapi_msg, SON_PHY_STOP_BCCH_REQ, SON_NULL, msg_length);

    /* Fill SON_PHY_STOP_BCCH_REQ message */
    p_fapi_msg = p_fapi_msg + SON_PHY_INTERFACE_API_HEADER_SIZE;
    if (SON_FAILURE == son_compose_son_phy_stop_bcch_req(&p_fapi_msg, p_son_phy_stop_bcch_req))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    SON_ASSERT((p_fapi_msg - msg_api_length) == p_msg);

    /* Send message to */
    son_send_msg(p_msg, SON_FAPI_MODULE_ID);

    return SON_SUCCESS;
}
