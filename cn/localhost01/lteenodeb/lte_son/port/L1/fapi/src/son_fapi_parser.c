/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_fapi_parser.c $
 *
 *******************************************************************************
 *
 * File Description: File son_fapi_parser.c added for SON module code
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#include "son_pack_unpack_utils.h"
#include "son_fapi_parser.h"
#include "son_utils.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
son_return_et
son_parse_vendor_specific_list
(
    vendor_specific_list_t *p_vendor_specific_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_cell_found_list
(
    cell_found_list_t *p_cell_found_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
son_return_et
son_parse_sib_info
(
    sib_info_t *p_sib_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

son_return_et
son_parse_son_phy_rssi_meas_req
(
    son_phy_rssi_meas_req_t *p_son_phy_rssi_meas_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_phy_rssi_meas_req, 0, sizeof(son_phy_rssi_meas_req_t));

    /* This function parses son_phy_rssi_meas_req */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing son_phy_rssi_meas_req");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_rssi_meas_req->freq_band_indicator, p_src + *p_length_read, "freq_band_indicator");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_phy_rssi_meas_req->freq_band_indicator < 1) || (p_son_phy_rssi_meas_req->freq_band_indicator > MAX_FREQ_BAND_INDICATOR))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_rssi_meas_req->freq_band_indicator] should be in range [1 <= %d <= MAX_FREQ_BAND_INDICATOR] !", p_son_phy_rssi_meas_req->freq_band_indicator);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_rssi_meas_req->meas_period, p_src + *p_length_read, "meas_period");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_rssi_meas_req->num_earfcn, p_src + *p_length_read, "num_earfcn");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_rssi_meas_req->num_earfcn > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_rssi_meas_req->num_earfcn] should be in range [%d <= 100] !", p_son_phy_rssi_meas_req->num_earfcn);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)(p_son_phy_rssi_meas_req->num_earfcn * sizeof(p_son_phy_rssi_meas_req->earfcn[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_rssi_meas_req->num_earfcn; loop++)
        {
            son_cp_unpack_U32((void*)&p_son_phy_rssi_meas_req->earfcn[loop], (void*)(p_src + *p_length_read), "earfcn[]");
            *p_length_read += sizeof(U32);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_rssi_meas_req->num_vendor_specific_list, p_src + *p_length_read, "num_vendor_specific_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_rssi_meas_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_rssi_meas_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_rssi_meas_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_rssi_meas_req->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_parse_vendor_specific_list(
                &p_son_phy_rssi_meas_req->vendor_specific_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_vendor_specific_list
(
    vendor_specific_list_t *p_vendor_specific_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_vendor_specific_list, 0, sizeof(vendor_specific_list_t));

    /* This function parses vendor_specific_list */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing vendor_specific_list");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_vendor_specific_list->type, p_src + *p_length_read, "type");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_vendor_specific_list->length, p_src + *p_length_read, "length");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)(p_vendor_specific_list->length * sizeof(p_vendor_specific_list->value[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_vendor_specific_list->length; loop++)
        {
            son_cp_unpack_U8((void*)&p_vendor_specific_list->value[loop], (void*)(p_src + *p_length_read), "value[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_phy_rssi_meas_ind
(
    son_phy_rssi_meas_ind_t *p_son_phy_rssi_meas_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_phy_rssi_meas_ind, 0, sizeof(son_phy_rssi_meas_ind_t));

    /* This function parses son_phy_rssi_meas_ind */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing son_phy_rssi_meas_ind");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_rssi_meas_ind->rssi, p_src + *p_length_read, "rssi");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_rssi_meas_ind->rssi > 128))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_rssi_meas_ind->rssi] should be in range [%d <= 128] !", p_son_phy_rssi_meas_ind->rssi);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_phy_rssi_meas_ind->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_rssi_meas_ind->num_vendor_specific_list, p_src + *p_length_read, "num_vendor_specific_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_rssi_meas_ind->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_rssi_meas_ind->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_rssi_meas_ind->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_rssi_meas_ind->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_parse_vendor_specific_list(
                &p_son_phy_rssi_meas_ind->vendor_specific_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_phy_rssi_meas_cnf
(
    son_phy_rssi_meas_cnf_t *p_son_phy_rssi_meas_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_phy_rssi_meas_cnf, 0, sizeof(son_phy_rssi_meas_cnf_t));

    /* This function parses son_phy_rssi_meas_cnf */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing son_phy_rssi_meas_cnf");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_rssi_meas_cnf->num_vendor_specific_list, p_src + *p_length_read, "num_vendor_specific_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_rssi_meas_cnf->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_rssi_meas_cnf->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_rssi_meas_cnf->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_rssi_meas_cnf->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_parse_vendor_specific_list(
                &p_son_phy_rssi_meas_cnf->vendor_specific_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_phy_stop_rssi_meas_req
(
    son_phy_stop_rssi_meas_req_t *p_son_phy_stop_rssi_meas_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_phy_stop_rssi_meas_req, 0, sizeof(son_phy_stop_rssi_meas_req_t));

    /* This function parses son_phy_stop_rssi_meas_req */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing son_phy_stop_rssi_meas_req");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_stop_rssi_meas_req->num_vendor_specific_list, p_src + *p_length_read, "num_vendor_specific_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_stop_rssi_meas_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_stop_rssi_meas_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_stop_rssi_meas_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_stop_rssi_meas_req->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_parse_vendor_specific_list(
                &p_son_phy_stop_rssi_meas_req->vendor_specific_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_phy_cell_search_req
(
    son_phy_cell_search_req_t *p_son_phy_cell_search_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_phy_cell_search_req, 0, sizeof(son_phy_cell_search_req_t));

    /* This function parses son_phy_cell_search_req */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing son_phy_cell_search_req");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_phy_cell_search_req->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_cell_search_req->exhaustive_search, p_src + *p_length_read, "exhaustive_search");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_cell_search_req->exhaustive_search > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_req->exhaustive_search] should be in range [%d <= 1] !", p_son_phy_cell_search_req->exhaustive_search);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_cell_search_req->num_pci_list, p_src + *p_length_read, "num_pci_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_cell_search_req->num_pci_list > 50))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_req->num_pci_list] should be in range [%d <= 50] !", p_son_phy_cell_search_req->num_pci_list);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)(p_son_phy_cell_search_req->num_pci_list * sizeof(p_son_phy_cell_search_req->pci_list[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_cell_search_req->num_pci_list; loop++)
        {
            son_cp_unpack_U16((void*)&p_son_phy_cell_search_req->pci_list[loop], (void*)(p_src + *p_length_read), "pci_list[]");
            *p_length_read += sizeof(U16);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_cell_search_req->meas_bandwidth, p_src + *p_length_read, "meas_bandwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_phy_cell_search_req->meas_bandwidth < 6) || (p_son_phy_cell_search_req->meas_bandwidth > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_req->meas_bandwidth] should be in range [6 <= %d <= 100] !", p_son_phy_cell_search_req->meas_bandwidth);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_cell_search_req->meas_period, p_src + *p_length_read, "meas_period");
    *p_length_read += sizeof(U8);

    /* Check for correct range [L - lower boundary] */
    if ((p_son_phy_cell_search_req->meas_period < 1))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_req->meas_period] should be in range [1 <= %d] !", p_son_phy_cell_search_req->meas_period);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_cell_search_req->num_vendor_specific_list, p_src + *p_length_read, "num_vendor_specific_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_cell_search_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_cell_search_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_cell_search_req->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_parse_vendor_specific_list(
                &p_son_phy_cell_search_req->vendor_specific_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_phy_cell_search_ind
(
    son_phy_cell_search_ind_t *p_son_phy_cell_search_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_phy_cell_search_ind, 0, sizeof(son_phy_cell_search_ind_t));

    /* This function parses son_phy_cell_search_ind */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing son_phy_cell_search_ind");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_cell_search_ind->num_pci, p_src + *p_length_read, "num_pci");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_cell_search_ind->num_pci > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_ind->num_pci] should be in range [%d <= 16] !", p_son_phy_cell_search_ind->num_pci);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_cell_search_ind->num_pci; loop++)
        {
            if (SON_FAILURE == son_parse_cell_found_list(
                &p_son_phy_cell_search_ind->cell_found_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_cell_search_ind->num_vendor_specific_list, p_src + *p_length_read, "num_vendor_specific_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_cell_search_ind->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_ind->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_cell_search_ind->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_cell_search_ind->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_parse_vendor_specific_list(
                &p_son_phy_cell_search_ind->vendor_specific_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_cell_found_list
(
    cell_found_list_t *p_cell_found_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_cell_found_list, 0, sizeof(cell_found_list_t));

    /* This function parses cell_found_list */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing cell_found_list");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_cell_found_list->rsrp, p_src + *p_length_read, "rsrp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_cell_found_list->rsrp > 128))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_cell_found_list->rsrp] should be in range [%d <= 128] !", p_cell_found_list->rsrp);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_cell_found_list->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_cell_found_list->pci > 503))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_cell_found_list->pci] should be in range [%d <= 503] !", p_cell_found_list->pci);
        return SON_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_phy_cell_search_cnf
(
    son_phy_cell_search_cnf_t *p_son_phy_cell_search_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_phy_cell_search_cnf, 0, sizeof(son_phy_cell_search_cnf_t));

    /* This function parses son_phy_cell_search_cnf */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing son_phy_cell_search_cnf");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_cell_search_cnf->num_vendor_specific_list, p_src + *p_length_read, "num_vendor_specific_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_cell_search_cnf->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_cell_search_cnf->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_cell_search_cnf->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_cell_search_cnf->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_parse_vendor_specific_list(
                &p_son_phy_cell_search_cnf->vendor_specific_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_phy_stop_cell_search_req
(
    son_phy_stop_cell_search_req_t *p_son_phy_stop_cell_search_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_phy_stop_cell_search_req, 0, sizeof(son_phy_stop_cell_search_req_t));

    /* This function parses son_phy_stop_cell_search_req */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing son_phy_stop_cell_search_req");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_stop_cell_search_req->num_vendor_specific_list, p_src + *p_length_read, "num_vendor_specific_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_stop_cell_search_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_stop_cell_search_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_stop_cell_search_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_stop_cell_search_req->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_parse_vendor_specific_list(
                &p_son_phy_stop_cell_search_req->vendor_specific_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_phy_pbch_config_req
(
    son_phy_pbch_config_req_t *p_son_phy_pbch_config_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_phy_pbch_config_req, 0, sizeof(son_phy_pbch_config_req_t));

    /* This function parses son_phy_pbch_config_req */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing son_phy_pbch_config_req");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_phy_pbch_config_req->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_config_req->pci > 503))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_config_req->pci] should be in range [%d <= 503] !", p_son_phy_pbch_config_req->pci);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_phy_pbch_config_req->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_pbch_config_req->num_vendor_specific_list, p_src + *p_length_read, "num_vendor_specific_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_config_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_config_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_pbch_config_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_pbch_config_req->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_parse_vendor_specific_list(
                &p_son_phy_pbch_config_req->vendor_specific_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_phy_pbch_config_cnf
(
    son_phy_pbch_config_cnf_t *p_son_phy_pbch_config_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_phy_pbch_config_cnf, 0, sizeof(son_phy_pbch_config_cnf_t));

    /* This function parses son_phy_pbch_config_cnf */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing son_phy_pbch_config_cnf");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_pbch_config_cnf->num_vendor_specific_list, p_src + *p_length_read, "num_vendor_specific_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_config_cnf->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_config_cnf->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_pbch_config_cnf->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_pbch_config_cnf->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_parse_vendor_specific_list(
                &p_son_phy_pbch_config_cnf->vendor_specific_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_phy_pbch_data_ind
(
    son_phy_pbch_data_ind_t *p_son_phy_pbch_data_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_phy_pbch_data_ind, 0, sizeof(son_phy_pbch_data_ind_t));

    /* This function parses son_phy_pbch_data_ind */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing son_phy_pbch_data_ind");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_phy_pbch_data_ind->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_data_ind->pci > 503))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_data_ind->pci] should be in range [%d <= 503] !", p_son_phy_pbch_data_ind->pci);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_phy_pbch_data_ind->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_data_ind->cause > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_data_ind->cause] should be in range [%d <= 1] !", p_son_phy_pbch_data_ind->cause);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_pbch_data_ind->num_antenna, p_src + *p_length_read, "num_antenna");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_phy_pbch_data_ind->num_antenna < 1) || (p_son_phy_pbch_data_ind->num_antenna > 4))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_data_ind->num_antenna] should be in range [1 <= %d <= 4] !", p_son_phy_pbch_data_ind->num_antenna);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_son_phy_pbch_data_ind->sfn, p_src + *p_length_read, "sfn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_data_ind->sfn > 1023))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_data_ind->sfn] should be in range [%d <= 1023] !", p_son_phy_pbch_data_ind->sfn);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_phy_pbch_data_ind->phich_duration, p_src + *p_length_read, "phich_duration");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_data_ind->phich_duration > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_data_ind->phich_duration] should be in range [%d <= 1] !", p_son_phy_pbch_data_ind->phich_duration);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U32(&p_son_phy_pbch_data_ind->phich_resource, p_src + *p_length_read, "phich_resource");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_data_ind->phich_resource > 3))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_data_ind->phich_resource] should be in range [%d <= 3] !", p_son_phy_pbch_data_ind->phich_resource);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_pbch_data_ind->dl_sys_bandwidth, p_src + *p_length_read, "dl_sys_bandwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_phy_pbch_data_ind->dl_sys_bandwidth < 6) || (p_son_phy_pbch_data_ind->dl_sys_bandwidth > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_data_ind->dl_sys_bandwidth] should be in range [6 <= %d <= 100] !", p_son_phy_pbch_data_ind->dl_sys_bandwidth);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_pbch_data_ind->num_vendor_specific_list, p_src + *p_length_read, "num_vendor_specific_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_pbch_data_ind->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_pbch_data_ind->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_pbch_data_ind->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_pbch_data_ind->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_parse_vendor_specific_list(
                &p_son_phy_pbch_data_ind->vendor_specific_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_phy_stop_pbch_req
(
    son_phy_stop_pbch_req_t *p_son_phy_stop_pbch_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_phy_stop_pbch_req, 0, sizeof(son_phy_stop_pbch_req_t));

    /* This function parses son_phy_stop_pbch_req */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing son_phy_stop_pbch_req");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_stop_pbch_req->num_vendor_specific_list, p_src + *p_length_read, "num_vendor_specific_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_stop_pbch_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_stop_pbch_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_stop_pbch_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_stop_pbch_req->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_parse_vendor_specific_list(
                &p_son_phy_stop_pbch_req->vendor_specific_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_phy_sib1_config_req
(
    son_phy_sib1_config_req_t *p_son_phy_sib1_config_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_phy_sib1_config_req, 0, sizeof(son_phy_sib1_config_req_t));

    /* This function parses son_phy_sib1_config_req */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing son_phy_sib1_config_req");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_sib1_config_req->retry_count, p_src + *p_length_read, "retry_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_phy_sib1_config_req->retry_count < 1) || (p_son_phy_sib1_config_req->retry_count > 8))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_sib1_config_req->retry_count] should be in range [1 <= %d <= 8] !", p_son_phy_sib1_config_req->retry_count);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_sib1_config_req->num_vendor_specific_list, p_src + *p_length_read, "num_vendor_specific_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_sib1_config_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_sib1_config_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_sib1_config_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_sib1_config_req->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_parse_vendor_specific_list(
                &p_son_phy_sib1_config_req->vendor_specific_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_phy_sib1_config_cnf
(
    son_phy_sib1_config_cnf_t *p_son_phy_sib1_config_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_phy_sib1_config_cnf, 0, sizeof(son_phy_sib1_config_cnf_t));

    /* This function parses son_phy_sib1_config_cnf */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing son_phy_sib1_config_cnf");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_sib1_config_cnf->num_vendor_specific_list, p_src + *p_length_read, "num_vendor_specific_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_sib1_config_cnf->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_sib1_config_cnf->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_sib1_config_cnf->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_sib1_config_cnf->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_parse_vendor_specific_list(
                &p_son_phy_sib1_config_cnf->vendor_specific_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_phy_bcch_config_req
(
    son_phy_bcch_config_req_t *p_son_phy_bcch_config_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_phy_bcch_config_req, 0, sizeof(son_phy_bcch_config_req_t));

    /* This function parses son_phy_bcch_config_req */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing son_phy_bcch_config_req");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_bcch_config_req->num_si_periodicity_list, p_src + *p_length_read, "num_si_periodicity_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_bcch_config_req->num_si_periodicity_list > 32))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_config_req->num_si_periodicity_list] should be in range [%d <= 32] !", p_son_phy_bcch_config_req->num_si_periodicity_list);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)(p_son_phy_bcch_config_req->num_si_periodicity_list * sizeof(p_son_phy_bcch_config_req->si_periodicity_list[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_bcch_config_req->num_si_periodicity_list; loop++)
        {
            son_cp_unpack_U16((void*)&p_son_phy_bcch_config_req->si_periodicity_list[loop], (void*)(p_src + *p_length_read), "si_periodicity_list[]");
            *p_length_read += sizeof(U16);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_bcch_config_req->si_window, p_src + *p_length_read, "si_window");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_phy_bcch_config_req->si_window < 1) || (p_son_phy_bcch_config_req->si_window > 40))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_config_req->si_window] should be in range [1 <= %d <= 40] !", p_son_phy_bcch_config_req->si_window);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_bcch_config_req->retry_count, p_src + *p_length_read, "retry_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_phy_bcch_config_req->retry_count < 1) || (p_son_phy_bcch_config_req->retry_count > 8))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_config_req->retry_count] should be in range [1 <= %d <= 8] !", p_son_phy_bcch_config_req->retry_count);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_bcch_config_req->sub_frame_assignment, p_src + *p_length_read, "sub_frame_assignment");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_bcch_config_req->sub_frame_assignment > 6))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_config_req->sub_frame_assignment] should be in range [%d <= 6] !", p_son_phy_bcch_config_req->sub_frame_assignment);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_bcch_config_req->special_sub_frame_patterns, p_src + *p_length_read, "special_sub_frame_patterns");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_bcch_config_req->special_sub_frame_patterns > 8))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_config_req->special_sub_frame_patterns] should be in range [%d <= 8] !", p_son_phy_bcch_config_req->special_sub_frame_patterns);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_bcch_config_req->num_vendor_specific_list, p_src + *p_length_read, "num_vendor_specific_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_bcch_config_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_config_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_bcch_config_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_bcch_config_req->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_parse_vendor_specific_list(
                &p_son_phy_bcch_config_req->vendor_specific_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_phy_bcch_config_cnf
(
    son_phy_bcch_config_cnf_t *p_son_phy_bcch_config_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_phy_bcch_config_cnf, 0, sizeof(son_phy_bcch_config_cnf_t));

    /* This function parses son_phy_bcch_config_cnf */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing son_phy_bcch_config_cnf");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_bcch_config_cnf->num_vendor_specific_list, p_src + *p_length_read, "num_vendor_specific_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_bcch_config_cnf->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_config_cnf->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_bcch_config_cnf->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_bcch_config_cnf->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_parse_vendor_specific_list(
                &p_son_phy_bcch_config_cnf->vendor_specific_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_phy_bcch_data_ind
(
    son_phy_bcch_data_ind_t *p_son_phy_bcch_data_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_phy_bcch_data_ind, 0, sizeof(son_phy_bcch_data_ind_t));

    /* This function parses son_phy_bcch_data_ind */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing son_phy_bcch_data_ind");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_bcch_data_ind->num_bcch_data_list, p_src + *p_length_read, "num_bcch_data_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_bcch_data_ind->num_bcch_data_list > 32))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_data_ind->num_bcch_data_list] should be in range [%d <= 32] !", p_son_phy_bcch_data_ind->num_bcch_data_list);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_bcch_data_ind->num_bcch_data_list; loop++)
        {
            if (SON_FAILURE == son_parse_sib_info(
                &p_son_phy_bcch_data_ind->sib_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_bcch_data_ind->num_vendor_specific_list, p_src + *p_length_read, "num_vendor_specific_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_bcch_data_ind->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_bcch_data_ind->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_bcch_data_ind->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_bcch_data_ind->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_parse_vendor_specific_list(
                &p_son_phy_bcch_data_ind->vendor_specific_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

static
son_return_et
son_parse_sib_info
(
    sib_info_t *p_sib_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    SON_MEMSET(p_sib_info, 0, sizeof(sib_info_t));

    /* This function parses sib_info */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing sib_info");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_sib_info->sfn, p_src + *p_length_read, "sfn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_sib_info->sfn > 1023))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_sib_info->sfn] should be in range [%d <= 1023] !", p_sib_info->sfn);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_sib_info->sf, p_src + *p_length_read, "sf");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_sib_info->sf > 9))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_sib_info->sf] should be in range [%d <= 9] !", p_sib_info->sf);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U16(&p_sib_info->len, p_src + *p_length_read, "len");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_sib_info->len > 2048))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_sib_info->len] should be in range [%d <= 2048] !", p_sib_info->len);
        return SON_FAILURE;
    }

    if (*p_length_read + (S32)(p_sib_info->len * sizeof(p_sib_info->data[0])) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_sib_info->len; loop++)
        {
            son_cp_unpack_U8((void*)&p_sib_info->data[loop], (void*)(p_src + *p_length_read), "data[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

son_return_et
son_parse_son_phy_stop_bcch_req
(
    son_phy_stop_bcch_req_t *p_son_phy_stop_bcch_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    SON_MEMSET(p_son_phy_stop_bcch_req, 0, sizeof(son_phy_stop_bcch_req_t));

    /* This function parses son_phy_stop_bcch_req */
    SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_COMP_PARSE, "Parsing son_phy_stop_bcch_req");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    son_cp_unpack_U8(&p_son_phy_stop_bcch_req->num_vendor_specific_list, p_src + *p_length_read, "num_vendor_specific_list");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_phy_stop_bcch_req->num_vendor_specific_list > 16))
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Parameter [p_son_phy_stop_bcch_req->num_vendor_specific_list] should be in range [%d <= 16] !", p_son_phy_stop_bcch_req->num_vendor_specific_list);
        return SON_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_phy_stop_bcch_req->num_vendor_specific_list; loop++)
        {
            if (SON_FAILURE == son_parse_vendor_specific_list(
                &p_son_phy_stop_bcch_req->vendor_specific_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return SON_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        SON_LOG(SON_OAM_LOG_ON, SON_NULL,  SON_WARNING, "Incoming message damaged!");
        return SON_FAILURE;
    }

    return(SON_SUCCESS);
}

