/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: rrm_oam_platform_parser.c $
 *
 *******************************************************************************
 *
 * File Description: RRM OAM platform APIs parser function definations.
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#include "rrm_pack_unpack_utils.h"
#include "rrm_oam_platform_parser.h"
#include "rrm_oam_platform_intf.h"
#include "rrm_utils.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrm_return_et
rrm_parse_rrm_oam_cell_plmn_info
(
    rrm_oam_cell_plmn_info_t *p_rrm_oam_cell_plmn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_oam_eutran_global_cell_id
(
    rrm_oam_eutran_global_cell_id_t *p_rrm_oam_eutran_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_oam_load_status
(
    rrm_oam_load_status_t *p_rrm_oam_load_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_rrm_plat_cell_load
(
    rrm_plat_cell_load_t *p_rrm_plat_cell_load,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrm_return_et
rrm_parse_platform_rrm_load_ind
(
    platform_rrm_load_ind_t *p_platform_rrm_load_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    /* ARGUSED */
     
     

    *p_length_read = 0;

    RRM_MEMSET(p_platform_rrm_load_ind, 0, sizeof(platform_rrm_load_ind_t));

    /* This function parses platform_rrm_load_ind */
#ifdef RRM_DEBUG
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing platform_rrm_load_ind");
#endif


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
#endif
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_platform_rrm_load_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
#endif
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_platform_rrm_load_ind->num_of_cell, p_src + *p_length_read, "num_of_cell");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_platform_rrm_load_ind->num_of_cell < 1) || (p_platform_rrm_load_ind->num_of_cell > 6))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_platform_rrm_load_ind->num_of_cell] should be in range [1 <= %d <= 6] !", p_platform_rrm_load_ind->num_of_cell);
#endif
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_platform_rrm_load_ind->num_of_cell; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_plat_cell_load(
                &p_platform_rrm_load_ind->cell_load[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
#endif
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_oam_cell_plmn_info
(
    rrm_oam_cell_plmn_info_t *p_rrm_oam_cell_plmn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{


    /* ARGUSED */
     
     

    *p_length_read = 0;

    RRM_MEMSET(p_rrm_oam_cell_plmn_info, 0, sizeof(rrm_oam_cell_plmn_info_t));

    /* This function parses rrm_oam_cell_plmn_info */
#ifdef RRM_DEBUG
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing rrm_oam_cell_plmn_info");
#endif


    if (*p_length_read + (S32)sizeof(p_rrm_oam_cell_plmn_info->mcc) > length_left)
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
#endif
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_cell_plmn_info->mcc); loop++)
        {
#ifdef RRM_DEBUG
            RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Array index : %d", loop);
#endif
            rrm_cp_unpack_U8((void*)&p_rrm_oam_cell_plmn_info->mcc[loop], (void*)(p_src + *p_length_read), "mcc[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
#endif
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_oam_cell_plmn_info->num_mnc_digit, p_src + *p_length_read, "num_mnc_digit");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_oam_cell_plmn_info->num_mnc_digit < 2) || (p_rrm_oam_cell_plmn_info->num_mnc_digit > 3))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_oam_cell_plmn_info->num_mnc_digit] should be in range [2 <= %d <= 3] !", p_rrm_oam_cell_plmn_info->num_mnc_digit);
#endif
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)(p_rrm_oam_cell_plmn_info->num_mnc_digit * sizeof(p_rrm_oam_cell_plmn_info->mnc[0])) > length_left)
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
#endif
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_oam_cell_plmn_info->num_mnc_digit; loop++)
        {
#ifdef RRM_DEBUG
            RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Array index : %d", loop);
#endif
            rrm_cp_unpack_U8((void*)&p_rrm_oam_cell_plmn_info->mnc[loop], (void*)(p_src + *p_length_read), "mnc[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
#endif
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_oam_eutran_global_cell_id
(
    rrm_oam_eutran_global_cell_id_t *p_rrm_oam_eutran_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    /* ARGUSED */
     
     

    *p_length_read = 0;

    RRM_MEMSET(p_rrm_oam_eutran_global_cell_id, 0, sizeof(rrm_oam_eutran_global_cell_id_t));

    /* This function parses rrm_oam_eutran_global_cell_id */
#ifdef RRM_DEBUG
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing rrm_oam_eutran_global_cell_id");
#endif


    if (RRM_FAILURE == rrm_parse_rrm_oam_cell_plmn_info(
        &p_rrm_oam_eutran_global_cell_id->primary_plmn_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_rrm_oam_eutran_global_cell_id->cell_identity) > length_left)
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
#endif
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrm_oam_eutran_global_cell_id->cell_identity); loop++)
        {
#ifdef RRM_DEBUG
            RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Array index : %d", loop);
#endif
            rrm_cp_unpack_U8((void*)&p_rrm_oam_eutran_global_cell_id->cell_identity[loop], (void*)(p_src + *p_length_read), "cell_identity[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
#endif
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_oam_load_status
(
    rrm_oam_load_status_t *p_rrm_oam_load_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{


    /* ARGUSED */
     
     

    *p_length_read = 0;

    RRM_MEMSET(p_rrm_oam_load_status, 0, sizeof(rrm_oam_load_status_t));

    /* This function parses rrm_oam_load_status */
#ifdef RRM_DEBUG
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing rrm_oam_load_status");
#endif


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
#endif
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_oam_load_status->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
#endif
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_rrm_oam_load_status->load_src, p_src + *p_length_read, "load_src");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_oam_load_status->load_src > 2))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_oam_load_status->load_src] should be in range [%d <= 2] !", p_rrm_oam_load_status->load_src);
#endif
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
#endif
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_oam_load_status->load_level, p_src + *p_length_read, "load_level");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
#endif
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_rrm_plat_cell_load
(
    rrm_plat_cell_load_t *p_rrm_plat_cell_load,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    /* ARGUSED */
     
     

    *p_length_read = 0;

    RRM_MEMSET(p_rrm_plat_cell_load, 0, sizeof(rrm_plat_cell_load_t));

    /* This function parses rrm_plat_cell_load */
#ifdef RRM_DEBUG
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing rrm_plat_cell_load");
#endif


    if (RRM_FAILURE == rrm_parse_rrm_oam_eutran_global_cell_id(
        &p_rrm_plat_cell_load->global_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
#endif
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_plat_cell_load->ind_type, p_src + *p_length_read, "ind_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrm_plat_cell_load->ind_type > 1))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_plat_cell_load->ind_type] should be in range [%d <= 1] !", p_rrm_plat_cell_load->ind_type);
#endif
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
#endif
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_rrm_plat_cell_load->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrm_plat_cell_load->count < 1) || (p_rrm_plat_cell_load->count > 3))
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_rrm_plat_cell_load->count] should be in range [1 <= %d <= 3] !", p_rrm_plat_cell_load->count);
#endif
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_rrm_plat_cell_load->count; loop++)
        {
            if (RRM_FAILURE == rrm_parse_rrm_oam_load_status(
                &p_rrm_plat_cell_load->load_source[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
#ifdef RRM_DEBUG
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
#endif
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

