/******************************************************************************
 **
 **  ARICENT -
 **
 **  Copyright (C) 2011 Aricent Inc. All Rights Reserved.
 **
 *******************************************************************************
 **
 **  $Id: s1ap_rrm.c
 **
 *******************************************************************************
 **
 **  File Description : This file contains the definitions of functions handling
 **                     RRM requests.
 **
 *******************************************************************************
 **
 ******************************************************************************/

#include "s1ap_rrm_rim_intl.h"
#include "rrc_s1apRrm_intf.h"
#include "rrc_cp_common.h"
#include "rrc_ext_api.h"
#include "rrc_common_utils.h"
#include "rrc_defines.h"
#include "rrc_msg_mgmt.h"
/* SPR 17078 Fix Start */
#include "ranap_asn_enc_dec.h"
/* SPR 17078 Fix Stop */
#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))
#define RIM_MAX_LEN_IN_SINGLE_OCTET 127


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

extern rrc_return_et rim_asn_decode_global_enb_id 
(
 U8    *p_elem_source,
 rim_global_enb_id_t   *p_elem_target,
 U16 length
);


static
rrc_return_et
rrc_compose_rim_cell_id_intl
(
    U8  **pp_buffer,
    rim_cell_id_intl_t *p_rim_cell_id_intl
);

static
rrc_length_t
rrc_get_rim_app_id_intl_len
(
    rim_app_id_intl_t *p_rim_app_id_intl
);

static
rrc_length_t
rrc_get_rim_app_id_intl_tlv_len
(
    rim_app_id_intl_t *p_rim_app_id_intl

);

static
rrc_return_et
rrc_compose_rim_app_id_intl
(
    U8  **pp_buffer,
    rim_app_id_intl_t *p_rim_app_id_intl
);

static
rrc_length_t
rrc_get_rim_seq_num_intl_len
(
    rim_seq_num_intl_t *p_rim_seq_num_intl
);

static
rrc_length_t
rrc_get_rim_seq_num_intl_tlv_len
(
    rim_seq_num_intl_t *p_rim_seq_num_intl

);

static
rrc_return_et
rrc_compose_rim_seq_num_intl
(
    U8  **pp_buffer,
    rim_seq_num_intl_t *p_rim_seq_num_intl
);

static
rrc_length_t
rrc_get_rim_pdu_ind_intl_len
(
    rim_pdu_ind_intl_t *p_rim_pdu_ind_intl
);

static
rrc_length_t
rrc_get_rim_pdu_ind_intl_tlv_len
(
    rim_pdu_ind_intl_t *p_rim_pdu_ind_intl

);

static
rrc_return_et
rrc_compose_rim_pdu_ind_intl
(
    U8  **pp_buffer,
    rim_pdu_ind_intl_t *p_rim_pdu_ind_intl
);

static
rrc_length_t
rrc_get_rim_protocol_ver_intl_len
(
    rim_protocol_ver_intl_t *p_rim_protocol_ver_intl
);

static
rrc_length_t
rrc_get_rim_protocol_ver_intl_tlv_len
(
    rim_protocol_ver_intl_t *p_rim_protocol_ver_intl

);

static
rrc_return_et
rrc_compose_rim_protocol_ver_intl
(
    U8  **pp_buffer,
    rim_protocol_ver_intl_t *p_rim_protocol_ver_intl
);


static
rrc_return_et
rrc_compose_rim_req_app_contrn_nacc_intl
(
    U8  **pp_buffer,
    rim_req_app_contrn_nacc_intl_t *p_rim_req_app_contrn_nacc_intl
);


static
rrc_return_et
rrc_compose_rim_req_app_contrn_si3_intl
(
    U8  **pp_buffer,
    rim_req_app_contrn_si3_intl_t *p_rim_req_app_contrn_si3_intl
);


static
rrc_return_et
rrc_compose_rim_req_app_contrn_mbms_intl
(
    U8  **pp_buffer,
    rim_req_app_contrn_mbms_intl_t *p_rim_req_app_contrn_mbms_intl
);


static
rrc_return_et
rrc_compose_rim_req_app_contrn_son_intl
(
    U8  **pp_buffer,
    rim_req_app_contrn_son_intl_t *p_rim_req_app_contrn_son_intl
);

/* SPR 17078 Fix Deletion Start */
/* SPR 17078 Fix Deletion Stop */


static
rrc_return_et
rrc_compose_rim_req_app_container_intl
(
    U8  **pp_buffer,
    rim_req_app_container_intl_t *p_rim_req_app_container_intl
);

static
rrc_length_t
rrc_get_rim_son_transfer_app_idty_intl_len
(
    rim_son_transfer_app_idty_intl_t *p_rim_son_transfer_app_idty_intl
);

static
rrc_length_t
rrc_get_rim_son_transfer_app_idty_intl_tlv_len
(
    rim_son_transfer_app_idty_intl_t *p_rim_son_transfer_app_idty_intl

);

static
rrc_return_et
rrc_compose_rim_son_transfer_app_idty_intl
(
    U8  **pp_buffer,
    rim_son_transfer_app_idty_intl_t *p_rim_son_transfer_app_idty_intl
);

static
rrc_return_et
rrc_compose_rim_information_req_intl
(
    U8  **pp_buffer,
    rim_information_req_intl_t *p_rim_information_req_intl
);

static
rrc_return_et
rrc_il_parse_rim_routing_info
(
 rim_routing_info_t *p_rim_routing_info,
 U8  *p_src,
 S32 length_left,
 S32 *p_length_read
 /*SPR 17777 +-*/
 );


static
rrc_return_et
rrc_compose_rim_information_ack_intl
(
    U8  **pp_buffer,
    rim_information_ack_intl_t *p_rim_information_ack_intl
);

static
rrc_return_et
rrc_compose_rim_info_app_error_contrn_nacc_intl
(
    U8  **pp_buffer,
    rim_info_app_error_contrn_nacc_intl_t *p_rim_info_app_error_contrn_nacc_intl
);


static
rrc_return_et
rrc_compose_rim_info_app_error_contrn_si3_intl
(
    U8  **pp_buffer,
    rim_info_app_error_contrn_si3_intl_t *p_rim_info_app_error_contrn_si3_intl
);


static
rrc_return_et
rrc_compose_rim_info_app_error_contrn_mbms_intl
(
    U8  **pp_buffer,
    rim_info_app_error_contrn_mbms_intl_t *p_rim_info_app_error_contrn_mbms_intl
);


static
rrc_return_et
rrc_compose_rim_info_app_error_contrn_son_intl
(
    U8  **pp_buffer,
    rim_info_app_error_contrn_son_intl_t *p_rim_info_app_error_contrn_son_intl
);


static
rrc_return_et
rrc_compose_rim_info_app_error_contrn_utra_si_intl
(
    U8  **pp_buffer,
    rim_info_app_error_contrn_utra_si_intl_t *p_rim_info_app_error_contrn_utra_si_intl
);


static
rrc_return_et
rrc_compose_rim_info_app_error_container_intl
(
    U8  **pp_buffer,
    rim_info_app_error_container_intl_t *p_rim_info_app_error_container_intl
);


static
rrc_return_et
rrc_compose_rim_information_app_error_intl
(
    U8  **pp_buffer,
    rim_information_app_error_intl_t *p_rim_information_app_error_intl
);


static
rrc_return_et
rrc_parse_rim_app_id_intl
(
 rim_app_identity_t *p_rim_app_id_intl,
 U8  *p_src,
 S32 length_left,
 S32 *p_length_read,
 U8 *p_cause
 );

static
rrc_return_et
rrc_parse_rim_seq_num_intl
(
 rim_pdu_seq_num_t *p_rim_seq_num_intl,
 U8  *p_src,
 S32 length_left,
 S32 *p_length_read,
 U8 *p_cause
 );

static
rrc_return_et
rrc_parse_rim_pdu_ind_intl
(
 rim_pdu_ind_t *p_rim_pdu_ind_intl,
 U8  *p_src,
 S32 length_left,
 S32 *p_length_read,
 U8 *p_cause
 );

static
rrc_return_et
rrc_parse_rim_protocol_ver_intl
(
 rim_protocol_ver_t *p_rim_protocol_ver_intl,
 U8  *p_src,
 S32 length_left,
 S32 *p_length_read,
 U8 *p_cause
 );


static
rrc_return_et
rrc_parse_rim_son_transfer_app_idty_intl
(
    rim_son_transfer_app_idty_intl_t *p_rim_son_transfer_app_idty_intl,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);


static
rrc_return_et
rrc_parse_rim_info_app_container_intl
(
 rim_info_app_container_t *p_rim_info_app_container_intl,
 U8  *p_src,
 S32 length_left,
 S32 *p_length_read,
 rim_app_identity_t app_idty
 );

static
rrc_return_et
rrc_il_parse_rim_information_intl
(
 rim_information_t *p_rim_information_intl,
 U8  *p_src,
 S32 length_left,
 S32 *p_length_read,
 U8  *p_cause
 );


static
rrc_return_et
rrc_parse_rim_cause_intl
(
    rim_cause_intl_t *p_rim_cause_intl,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_rim_error_pdu_intl
(
    rim_error_pdu_intl_t *p_rim_error_pdu_intl,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rim_information_error_intl
(
 rim_information_err_t *p_rim_information_error_intl,
 U8  *p_src,
 S32 length_left,
 S32 *p_length_read
 );


static
rrc_return_et
rrc_parse_rim_info_app_error_container_intl
(
 rim_app_err_container_t *p_rim_info_app_error_container_intl,
 U8  *p_src,
 /*SPR 17777 +-*/
 S32 *p_length_read,
 rim_app_identity_t app_idty
 );


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/


/*****************************************************************************
  * Function Name  : rrc_intl_compose_ran_information_req_rim
  * Inputs         : pp_buffer - pointer to data
  *                  p_ran_information_req_rim - pointer to the 
  *                                                ran_information_req_rim_t
  *                  p_msg_length - pointer to the msg length 
  * Outputs        : None
  * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS 
  * Description    : This function composes the RRC_RAN_INFO_REQ_RIM  msg.
********************************************************************************/
rrc_return_et
rrc_intl_compose_ran_information_req_rim
(
    U8  **pp_buffer,
    ran_information_req_rim_t *p_ran_information_req_rim,
    U32 *p_msg_length
)
{
    U8 * p_start_buf = RRC_NULL;
    /* ARGUSED */

    p_start_buf = *pp_buffer;

    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_ran_information_req_rim != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_ran_information_req_rim->pdu_type, "pdu_type");
    *pp_buffer += sizeof(p_ran_information_req_rim->pdu_type);

    /* Compose TLV  */
    if (p_ran_information_req_rim->bitmask & RIM_ROUTING_INFO_TAG)
    {
        if (RRC_FAILURE == rrc_compose_rim_cell_id_intl(pp_buffer, &p_ran_information_req_rim->dst_cell_id))
        {
	    /* SPR 20545 Fix Start */
            *pp_buffer = p_start_buf;
	    /* SPR 20545 Fix Stop */
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_ran_information_req_rim->bitmask & RIM_ROUTING_INFO_TAG)
    {
        if (RRC_FAILURE == rrc_compose_rim_cell_id_intl(pp_buffer, &p_ran_information_req_rim->src_cell_id))
        {
	    /* SPR 20545 Fix Start */
            *pp_buffer = p_start_buf;
	    /* SPR 20545 Fix Stop */
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_ran_information_req_rim->bitmask & RIM_INFO_REQ_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_information_req_intl(pp_buffer, &p_ran_information_req_rim->rim_req))
        {
	    /* SPR 20545 Fix Start */
            *pp_buffer = p_start_buf;
	    /* SPR 20545 Fix Stop */
            return RRC_FAILURE;
        }
    }

    *p_msg_length = (*pp_buffer - p_start_buf);
    *pp_buffer = p_start_buf;
    return RRC_SUCCESS;
}


/*****************************************************************************
  * Function Name  : rrc_compose_rim_cell_id_intl
  * Inputs         : pp_buffer - pointer to the buffer  
  *                  p_rim_cell_id_intl - pointer to rim_cell_id_intl_t
  * Outputs        : None
  * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS 
  * Description    : This function composes the Cell Id for RIM REQ.
********************************************************************************/
static
rrc_return_et
rrc_compose_rim_cell_id_intl
(
    U8  **pp_buffer,
    rim_cell_id_intl_t *p_rim_cell_id_intl
)
{
    U8 tag = RRC_NULL;
    U16 tlv_length = RRC_NULL;


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_cell_id_intl != PNULL);

    /* Compose TL of TLV */
    tag = RIM_ROUTING_INFO_TAG;
    tlv_length = p_rim_cell_id_intl->length + sizeof(U8); 

    rrc_cp_pack_U8(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(U8);

    if (tlv_length <= RIM_MAX_LEN_IN_SINGLE_OCTET){
        tlv_length |= 0x80;
        rrc_cp_pack_U8(*pp_buffer, &tlv_length, "<tlv_length>");
        *pp_buffer += sizeof(U8);
    } else {
        rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
        *pp_buffer += sizeof(U16);
    }

    rrc_cp_pack_U8(*pp_buffer, &p_rim_cell_id_intl->routing_add_disc, "routing_add_disc");
    *pp_buffer += sizeof(U8);
    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        if (RIM_CELL_IDENTIFIER_MAX_OCTET_LENGTH < (p_rim_cell_id_intl->length)){
    	    RRC_TRACE(RRC_ERROR, "CELL ID Length[%d] is incorrect\n",p_rim_cell_id_intl->length);
            return RRC_FAILURE;
        }
        for (loop = 0; loop < p_rim_cell_id_intl->length; loop++) 
        {
#ifdef RRC_DEBUG
         //   RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_DETAILED, "Array index : %d", loop);
#endif
            rrc_cp_pack_U8(*pp_buffer, &p_rim_cell_id_intl->cell_identifier[loop], "cell_identifier[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

/*****************************************************************************
  * Function Name  : rrc_get_rim_app_id_intl_len
  * Inputs         : p_rim_app_id_intl - pointer to rim_app_id_intl_t 
  * Outputs        : None 
  * Returns        : Length 
  * Description    : This function returns the RIM_API_ID length
********************************************************************************/
static
rrc_length_t
rrc_get_rim_app_id_intl_len
(
    rim_app_id_intl_t *p_rim_app_id_intl
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rim_app_id_intl != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rim_app_id_intl->rim_app_id);

    return length;
}

/*****************************************************************************
  * Function Name  : rrc_get_rim_app_id_intl_tlv_len
  * Inputs         : p_rim_app_id_intl - poinbter to rim_app_id_intl_t
  * Outputs        : None 
  * Returns        : Length 
  * Description    : This function internally calls a function which returns 
  *                  RIM_API_ID length
********************************************************************************/
static
rrc_length_t
rrc_get_rim_app_id_intl_tlv_len
(
    rim_app_id_intl_t *p_rim_app_id_intl

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rim_app_id_intl != PNULL);

    length += rrc_get_rim_app_id_intl_len(p_rim_app_id_intl);

    return length;
}

/*****************************************************************************
  * Function Name  : rrc_compose_rim_app_id_intl
  * Inputs         :  pp_buffer - pointer to data
  *                   p_rim_app_id_intl - pointer to rim_app_id_intl_t
  * Outputs        : None
  * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
  * Description    : This function composes the RIM_API_ID msg.
********************************************************************************/
static
rrc_return_et
rrc_compose_rim_app_id_intl
(
    U8  **pp_buffer,
    rim_app_id_intl_t *p_rim_app_id_intl
)
{
    U8 tag = RRC_NULL;
    U16 tlv_length = RRC_NULL;


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_app_id_intl != PNULL);

    /* Compose TL of TLV */
    tag = RIM_APP_IDENTITY_TAG;
    tlv_length = rrc_get_rim_app_id_intl_tlv_len(p_rim_app_id_intl);

    rrc_cp_pack_U8(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(U8);

    if (tlv_length <= RIM_MAX_LEN_IN_SINGLE_OCTET){
        tlv_length |= 0x80;
        rrc_cp_pack_U8(*pp_buffer, &tlv_length, "<tlv_length>");
        *pp_buffer += sizeof(U8);
    } else {
        rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
        *pp_buffer += sizeof(U16);
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rim_app_id_intl->rim_app_id, "rim_app_id");
    *pp_buffer += sizeof(p_rim_app_id_intl->rim_app_id);

    return RRC_SUCCESS;
}

/*****************************************************************************
  * Function Name  : rrc_get_rim_seq_num_intl_len
  * Inputs         : p_rim_seq_num_intl - pointer to rim_seq_num_intl_t
  * Outputs        : None
  * Returns        : Length 
  * Description    : This function returns the RIM_SEQ_NUM msg length
********************************************************************************/
static
rrc_length_t
rrc_get_rim_seq_num_intl_len
(
    rim_seq_num_intl_t *p_rim_seq_num_intl
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rim_seq_num_intl != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rim_seq_num_intl->rim_seq_num);

    return length;
}

/*****************************************************************************
  * Function Name  : rrc_get_rim_seq_num_intl_tlv_len
  * Inputs         : p_rim_seq_num_intl - pointer to rim_seq_num_intl_t
  * Outputs        : None
  * Returns        : Length 
  * Description    : This function calls a function internally which returns 
  *                  the RIM_SEQ_NUM msg length.
********************************************************************************/
static
rrc_length_t
rrc_get_rim_seq_num_intl_tlv_len
(
    rim_seq_num_intl_t *p_rim_seq_num_intl

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rim_seq_num_intl != PNULL);

    length += rrc_get_rim_seq_num_intl_len(p_rim_seq_num_intl);

    return length;
}

/*****************************************************************************
  * Function Name  : rrc_compose_rim_seq_num_intl
  * Inputs         : pp_buffer - pointer to data  
  *                  p_rim_seq_num_intl - pointer to rim_seq_num_intl_t
  * Outputs        : None
  * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
  * Description    : This function composes RIM_SEQ_NUM msg.
********************************************************************************/
static
rrc_return_et
rrc_compose_rim_seq_num_intl
(
    U8  **pp_buffer,
    rim_seq_num_intl_t *p_rim_seq_num_intl
)
{
    U8 tag = RRC_NULL;
    U16 tlv_length = RRC_NULL;


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_seq_num_intl != PNULL);

    /* Compose TL of TLV */
    tag = RIM_SEQUENCE_NUM_TAG;
    tlv_length = rrc_get_rim_seq_num_intl_tlv_len(p_rim_seq_num_intl);

    rrc_cp_pack_U8(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(U8);

    if (tlv_length <= RIM_MAX_LEN_IN_SINGLE_OCTET){
        tlv_length |= 0x80;
        rrc_cp_pack_U8(*pp_buffer, &tlv_length, "<tlv_length>");
        *pp_buffer += sizeof(U8);
    } else {
        rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
        *pp_buffer += sizeof(U16);
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rim_seq_num_intl->rim_seq_num, "rim_seq_num");
    *pp_buffer += sizeof(p_rim_seq_num_intl->rim_seq_num);

    return RRC_SUCCESS;
}

/*****************************************************************************
  * Function Name  : rrc_get_rim_pdu_ind_intl_len
  * Inputs         : p_rim_pdu_ind_intl - rim_pdu_ind_intl_t
  * Outputs        : None
  * Returns        : Length 
  * Description    : This function returns RIM_PDU_IND msg length .
********************************************************************************/
static
rrc_length_t
rrc_get_rim_pdu_ind_intl_len
(
    rim_pdu_ind_intl_t *p_rim_pdu_ind_intl
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rim_pdu_ind_intl != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rim_pdu_ind_intl->rim_pdu_ind);

    return length;
}

/*****************************************************************************
  * Function Name  : rrc_get_rim_pdu_ind_intl_tlv_len
  * Inputs         : p_rim_pdu_ind_intl - pointer to rim_pdu_ind_intl_t
  * Outputs        : None
  * Returns        : length
  * Description    : This function internally calls a function which 
  *                  returns RIM_PDU_IND msg length.
********************************************************************************/
static
rrc_length_t
rrc_get_rim_pdu_ind_intl_tlv_len
(
    rim_pdu_ind_intl_t *p_rim_pdu_ind_intl

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rim_pdu_ind_intl != PNULL);

    length += rrc_get_rim_pdu_ind_intl_len(p_rim_pdu_ind_intl);

    return length;
}

/*****************************************************************************
  * Function Name  : rrc_compose_rim_pdu_ind_intl
  * Inputs         : pp_buffer - pointer to data
  *                  p_rim_pdu_ind_intl - pointer to rim_pdu_ind_intl_t
  * Outputs        : None
  * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
  * Description    : This function composes RIM_PDU_IND msg .
********************************************************************************/
static
rrc_return_et
rrc_compose_rim_pdu_ind_intl
(
    U8  **pp_buffer,
    rim_pdu_ind_intl_t *p_rim_pdu_ind_intl
)
{
    U8 tag = RRC_NULL;
    U16 tlv_length = RRC_NULL;


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_pdu_ind_intl != PNULL);

    /* Compose TL of TLV */
    tag = RIM_PDU_IND_TAG;
    tlv_length = rrc_get_rim_pdu_ind_intl_tlv_len(p_rim_pdu_ind_intl);

    rrc_cp_pack_U8(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(U8);

    if (tlv_length <= RIM_MAX_LEN_IN_SINGLE_OCTET){
        tlv_length |= 0x80;
        rrc_cp_pack_U8(*pp_buffer, &tlv_length, "<tlv_length>");
        *pp_buffer += sizeof(U8);
    } else {
        rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
        *pp_buffer += sizeof(U16);
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rim_pdu_ind_intl->rim_pdu_ind, "rim_pdu_ind");
    *pp_buffer += sizeof(p_rim_pdu_ind_intl->rim_pdu_ind);

    return RRC_SUCCESS;
}

/*****************************************************************************
  * Function Name  : rrc_get_rim_protocol_ver_intl_len
  * Inputs         : p_rim_protocol_ver_intl - pointer to rim_protocol_ver_intl_t
  * Outputs        : None
  * Returns        : Length
  * Description    : This function retuens the rim_protocol_ver msg length.
********************************************************************************/
static
rrc_length_t
rrc_get_rim_protocol_ver_intl_len
(
    rim_protocol_ver_intl_t *p_rim_protocol_ver_intl
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rim_protocol_ver_intl != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rim_protocol_ver_intl->rim_protocol_ver);

    return length;
}

/*****************************************************************************
 * Function Name  : rrc_get_rim_protocol_ver_intl_tlv_len 
 * Inputs         : p_rim_protocol_ver_intl - pointer to rim_protocol_ver_intl_t
 * Outputs        : None
 * Returns        : Length
 * Description    : This function internally calls a function which returns 
 *                  the rim_protocol_ver length.
 ********************************************************************************/
static
rrc_length_t
rrc_get_rim_protocol_ver_intl_tlv_len
(
    rim_protocol_ver_intl_t *p_rim_protocol_ver_intl

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rim_protocol_ver_intl != PNULL);

    length += rrc_get_rim_protocol_ver_intl_len(p_rim_protocol_ver_intl);

    return length;
}

/*****************************************************************************
 * Function Name  : rrc_compose_rim_protocol_ver_intl 
 * Inputs         : pp_buffer - pointer to the data 
 *                  p_rim_protocol_ver_intl - pointer to rim_protocol_ver_intl_t
 * Outputs        : None 
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function composes the rim_protocol_ver msg 
 ********************************************************************************/
static
rrc_return_et
rrc_compose_rim_protocol_ver_intl
(
    U8  **pp_buffer,
    rim_protocol_ver_intl_t *p_rim_protocol_ver_intl
)
{
    U8 tag = RRC_NULL;
    U16 tlv_length = RRC_NULL;


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_protocol_ver_intl != PNULL);

    /* Compose TL of TLV */
    tag = RIM_PROTOCOL_VER_NUM_TAG;
    tlv_length = rrc_get_rim_protocol_ver_intl_tlv_len(p_rim_protocol_ver_intl);

    rrc_cp_pack_U8(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(U8);

    if (tlv_length <= RIM_MAX_LEN_IN_SINGLE_OCTET){
        tlv_length |= 0x80;
        rrc_cp_pack_U8(*pp_buffer, &tlv_length, "<tlv_length>");
        *pp_buffer += sizeof(U8);
    } else {
        rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
        *pp_buffer += sizeof(U16);
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rim_protocol_ver_intl->rim_protocol_ver, "rim_protocol_ver");
    *pp_buffer += sizeof(p_rim_protocol_ver_intl->rim_protocol_ver);

    return RRC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : rrc_compose_rim_req_app_contrn_nacc_intl 
 * Inputs         : pp_buffer - pointer to data
 *                  p_rim_req_app_contrn_nacc_intl - pointer to 
 *                                           rim_req_app_contrn_nacc_intl_t
 * Outputs        : None 
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS 
 * Description    : This function composes the rim_req_app_contrn_nacc msg
 ********************************************************************************/
static
rrc_return_et
rrc_compose_rim_req_app_contrn_nacc_intl
(
    U8  **pp_buffer,
    rim_req_app_contrn_nacc_intl_t *p_rim_req_app_contrn_nacc_intl
)
{

    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_req_app_contrn_nacc_intl != PNULL);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rim_req_app_contrn_nacc_intl->cell_identifier); loop++)
        {
#ifdef RRC_DEBUG
            RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_DETAILED, "Array index : %d", loop);
#endif
            rrc_cp_pack_U8(*pp_buffer, &p_rim_req_app_contrn_nacc_intl->cell_identifier[loop], "cell_identifier[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rrc_compose_rim_req_app_contrn_si3_intl 
 * Inputs         : pp_buffer - pointer to the data
 *                  p_rim_req_app_contrn_si3_intl - pointer to 
 *                                        rim_req_app_contrn_si3_intl_t
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : this function composes the rim_req_app_contrn_si3_intl msg
 ********************************************************************************/
static
rrc_return_et
rrc_compose_rim_req_app_contrn_si3_intl
(
    U8  **pp_buffer,
    rim_req_app_contrn_si3_intl_t *p_rim_req_app_contrn_si3_intl
)
{


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_req_app_contrn_si3_intl != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rim_req_app_contrn_si3_intl->dummy, "dummy");
    *pp_buffer += sizeof(p_rim_req_app_contrn_si3_intl->dummy);

    return RRC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : rrc_compose_rim_req_app_contrn_mbms_intl 
 * Inputs         : pp_buffer - pointer to the data
 *                  p_rim_req_app_contrn_mbms_intl - pointer to the 
 *                                              rim_req_app_contrn_mbms_intl_t
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCSESS
 * Description    : This function composes the rim_req_app_contrn_mbms msg.
 ********************************************************************************/
static
rrc_return_et
rrc_compose_rim_req_app_contrn_mbms_intl
(
    U8  **pp_buffer,
    rim_req_app_contrn_mbms_intl_t *p_rim_req_app_contrn_mbms_intl
)
{


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_req_app_contrn_mbms_intl != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rim_req_app_contrn_mbms_intl->dummy, "dummy");
    *pp_buffer += sizeof(p_rim_req_app_contrn_mbms_intl->dummy);

    return RRC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : rrc_compose_rim_req_app_contrn_son_intl 
 * Inputs         : pp_buffer - pointer to the data
 *                  p_rim_req_app_contrn_son_intl - pointer to 
 *                                            rim_req_app_contrn_son_intl_t 
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function composes the rim_req_app_contrn_son msg 
 ********************************************************************************/
static
rrc_return_et
rrc_compose_rim_req_app_contrn_son_intl
(
    U8  **pp_buffer,
    rim_req_app_contrn_son_intl_t *p_rim_req_app_contrn_son_intl
)
{


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_req_app_contrn_son_intl != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rim_req_app_contrn_son_intl->dummy, "dummy");
    *pp_buffer += sizeof(p_rim_req_app_contrn_son_intl->dummy);

    return RRC_SUCCESS;
}


/* SPR 17078 Fix Deletion Start */
/* SPR 17078 Fix Deletion Stop */


/*****************************************************************************
 * Function Name  : rrc_compose_rim_req_app_container_intl 
 * Inputs         : pp_buffer - pointer to data
 *                  p_rim_req_app_container_intl - pointer to 
 *                                              rim_req_app_container_intl_t
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCSES
 * Description    : This function composes the rim_req_app_container msg
 ********************************************************************************/
static
rrc_return_et
rrc_compose_rim_req_app_container_intl
(
    U8  **pp_buffer,
    rim_req_app_container_intl_t *p_rim_req_app_container_intl
)
{
    U8 tag = RRC_NULL;
    /* SPR 17078 Fix Start */
    U8 tlv_length = RRC_NULL;
    /* SPR 17078 Fix Stop */
    U8 *p_tag_length = RRC_NULL;

    /* SPR 17078 Fix Start */
    U8 index = RRC_NULL;
    ranap_SourceCellID ranap_source_cell_id;
    OSCTXT 	asn1_ctx;
    U8 		msg_buf[S1AP_MAX_ASN1_BUF_LEN];
    U8         	*p_data = PNULL;
    OSCTXT  	asn1_ctx_2;
    U32 	length = RRC_NULL;
    /* SPR 20545 Fix Start */
    U8 		*p_src_utran_cell_id = PNULL;

    #define SOURCE_UTRAN_CELL_ID_WITH_PADDING 	16
    #define SOURCE_UTRAN_PLMN_NUMOCTS_OFFSET 	4
    #define SOURCE_UTRAN_PLMN_DATA_OFFSET    	8
    #define SOURCE_UTRAN_CELL_ID_START		9
    /* SPR 20545 Fix Stop */
    /* SPR 17078 Fix Stop */
    

    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_req_app_container_intl != PNULL);

    /* SPR 17078 Fix Start */
    asn1Init_ranap_SourceCellID(&ranap_source_cell_id);
     /* ASN Encode message */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {     
        RRC_TRACE(RRC_ERROR, "ASN1 context initialization failed. \n");
        return RRC_FAILURE;
    } 
    if (RT_OK != rtInitContext(&asn1_ctx_2))
    {     
        RRC_TRACE(RRC_ERROR, "ASN1 context initialization failed. \n");
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        /*SPR_18125_END*/
        return RRC_FAILURE;
    } 
    
    /* SPR 17078 Fix Stop */

    /* Compose TL of TLV */
    tag = RIM_APP_CONTAINER_TAG;

    rrc_cp_pack_U8(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(U8);

    p_tag_length = *pp_buffer;
    /* SPR 17078 Fix Start */
    *pp_buffer += sizeof(U8);
    /* SPR 17078 Fix Stop */

    /* Optional element */
    if(p_rim_req_app_container_intl->bitmask & RIM_REQ_APP_CONTAINER_NACC_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_rim_req_app_contrn_nacc_intl(pp_buffer, &p_rim_req_app_container_intl->nacc))
    {
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        rtFreeContext(&asn1_ctx_2);
        /*SPR_18125_END*/
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_req_app_container_intl->bitmask & RIM_REQ_APP_CONTAINER_SI3_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_rim_req_app_contrn_si3_intl(pp_buffer, &p_rim_req_app_container_intl->si3))
    {
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        rtFreeContext(&asn1_ctx_2);
        /*SPR_18125_END*/
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_req_app_container_intl->bitmask & RIM_REQ_APP_CONTAINER_MBMS_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_rim_req_app_contrn_mbms_intl(pp_buffer, &p_rim_req_app_container_intl->mbms))
    {
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        rtFreeContext(&asn1_ctx_2);
        /*SPR_18125_END*/
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_req_app_container_intl->bitmask & RIM_REQ_APP_CONTAINER_SON_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_rim_req_app_contrn_son_intl(pp_buffer, &p_rim_req_app_container_intl->son))
    {
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        rtFreeContext(&asn1_ctx_2);
        /*SPR_18125_END*/
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_req_app_container_intl->bitmask & RIM_REQ_APP_CONTAINER_UTRA_SI_PRESENT)
    {
        /* SPR 17078 Fix Start */
	ranap_source_cell_id.t = T_ranap_SourceCellID_sourceUTRANCellID;
	
        /* SPR 20545 Fix Start */
 	ranap_source_cell_id.u.sourceUTRANCellID = 
	    (ranap_SourceUTRANCellID *)rtxMemAlloc(&asn1_ctx, SOURCE_UTRAN_CELL_ID_WITH_PADDING);

	if( PNULL == ranap_source_cell_id.u.sourceUTRANCellID )
	{
           RRC_TRACE(RRC_ERROR, "mem alloc failed \n");
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        rtFreeContext(&asn1_ctx_2);
        /*SPR_18125_END*/
           return RRC_FAILURE;
	}

	p_src_utran_cell_id = (U8 *)ranap_source_cell_id.u.sourceUTRANCellID;
	memset_wrapper(p_src_utran_cell_id, RRC_NULL, SOURCE_UTRAN_CELL_ID_WITH_PADDING ) ;

	p_src_utran_cell_id[SOURCE_UTRAN_PLMN_NUMOCTS_OFFSET] = MAX_PLMN_ID_BYTES;
	
    	for ( index = RRC_NULL ; index < MAX_PLMN_ID_BYTES; index++ )
	{
	    p_src_utran_cell_id[SOURCE_UTRAN_PLMN_DATA_OFFSET + index] = 
		p_rim_req_app_container_intl->utra_si.cell_identifier[index];
	}
	
	for ( ; index < RIM_SI_CELL_IDENTIFIER_OCTET_LENGTH; index++ )
	{
	    p_src_utran_cell_id[SOURCE_UTRAN_CELL_ID_START + index] = 
		p_rim_req_app_container_intl->utra_si.cell_identifier[index];
            
	}

        /* SPR 20545 Fix Stop */
        pu_setBuffer(&asn1_ctx, msg_buf, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);    

	if (RT_OK != asn1PE_ranap_SourceCellID(&asn1_ctx, &ranap_source_cell_id))
	{
	    RRC_TRACE(RRC_ERROR, "asn1PE_ranap_SourceCellID failed \n");
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        rtFreeContext(&asn1_ctx_2);
        /*SPR_18125_END*/
	    return RRC_FAILURE;
	}

	length = pe_GetMsgLen(&asn1_ctx);
	p_data = (U8 *)rtxMemAlloc(&asn1_ctx_2, length);
	 if (PNULL == p_data)
        {
            RRC_TRACE(RRC_ERROR, "memeory allocation failed \n");
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            rtFreeContext(&asn1_ctx_2);
            /*SPR_18125_END*/
            return RRC_FAILURE;
        }

	l3_memcpy_wrapper(p_data, msg_buf, length);

    	for ( index = RRC_NULL ; index < length; index++ )
	{
	    rrc_cp_pack_U8(*pp_buffer, p_data , "cell_identifier[]");
	    *pp_buffer += sizeof(U8);
	    p_data++;	
	}
    }

    tlv_length = *pp_buffer - (p_tag_length + sizeof(U8) );
    tlv_length |= 0x80;
    rrc_cp_pack_U8(p_tag_length, &tlv_length, "<tlv_length>");

    /* SPR 17078 Fix Stop */
    /*SPR_18125_START*/
    rtFreeContext(&asn1_ctx);
    rtFreeContext(&asn1_ctx_2);
    /*SPR_18125_END*/
    return RRC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rrc_get_rim_son_transfer_app_idty_intl_len 
 * Inputs         : p_rim_son_transfer_app_idty_intl - pointer to 
 *                                            rim_son_transfer_app_idty_intl_t
 * Outputs        : None
 * Returns        : Length 
 * Description    : This function retuens the _rim_son_transfer_app_idty length
 ********************************************************************************/
static
rrc_length_t
rrc_get_rim_son_transfer_app_idty_intl_len
(
    rim_son_transfer_app_idty_intl_t *p_rim_son_transfer_app_idty_intl
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rim_son_transfer_app_idty_intl != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rim_son_transfer_app_idty_intl->rim_son_app_idty);

    return length;
}

/*****************************************************************************
 * Function Name  : rrc_get_rim_son_transfer_app_idty_intl_tlv_len 
 * Inputs         : p_rim_son_transfer_app_idty_intl - pointer to 
 *                                            rim_son_transfer_app_idty_intl_t
 * Outputs        : None
 * Returns        : Length
 * Description    : This function intenally calls a function which retuens the 
 *                  rim_son_transfer_app_idty length.
 ********************************************************************************/
static
rrc_length_t
rrc_get_rim_son_transfer_app_idty_intl_tlv_len
(
    rim_son_transfer_app_idty_intl_t *p_rim_son_transfer_app_idty_intl

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rim_son_transfer_app_idty_intl != PNULL);

    length += rrc_get_rim_son_transfer_app_idty_intl_len(p_rim_son_transfer_app_idty_intl);

    return length;
}

/*****************************************************************************
 * Function Name  : rrc_compose_rim_son_transfer_app_idty_intl 
 * Inputs         : pp_buffer - pointer to the data
 *                  p_rim_son_transfer_app_idty_intl - pointer to 
 *                                            rim_son_transfer_app_idty_intl_t 
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function returns the rim_son_transfer_app_idty length.
 ********************************************************************************/
static
rrc_return_et
rrc_compose_rim_son_transfer_app_idty_intl
(
    U8  **pp_buffer,
    rim_son_transfer_app_idty_intl_t *p_rim_son_transfer_app_idty_intl
)
{
    U8 tag = RRC_NULL;
    U16 tlv_length = RRC_NULL;


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_son_transfer_app_idty_intl != PNULL);

    /* Compose TL of TLV */
    tag = RIM_SON_TRANSFER_APP_ID_TAG;
    tlv_length = rrc_get_rim_son_transfer_app_idty_intl_tlv_len(p_rim_son_transfer_app_idty_intl);

    rrc_cp_pack_U8(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(U8);

    if (tlv_length <= RIM_MAX_LEN_IN_SINGLE_OCTET){
        tlv_length |= 0x80;
        rrc_cp_pack_U8(*pp_buffer, &tlv_length, "<tlv_length>");
        *pp_buffer += sizeof(U16);
    } else {
        rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
        *pp_buffer += sizeof(U16);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_son_transfer_app_idty_intl->rim_son_app_idty > 1))
    {
#ifdef RRC_DEBUG
        RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_WARNING, "Parameter [p_rim_son_transfer_app_idty_intl->rim_son_app_idty] should be in range [%d <= 1] !", p_rim_son_transfer_app_idty_intl->rim_son_app_idty);
#endif
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rim_son_transfer_app_idty_intl->rim_son_app_idty, "rim_son_app_idty");
    *pp_buffer += sizeof(p_rim_son_transfer_app_idty_intl->rim_son_app_idty);

    return RRC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : rrc_compose_rim_information_req_intl 
 * Inputs         : pp_buffer - pointer to data
 *                  p_rim_information_req_intl - pointer to 
 *                                                rim_information_req_intl_t
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function composes the rim_information_req.
 ********************************************************************************/
static
rrc_return_et
rrc_compose_rim_information_req_intl
(
    U8  **pp_buffer,
    rim_information_req_intl_t *p_rim_information_req_intl
)
{
	/* coverity_fix_54734_start */
	U8 tag = RRC_NULL;
	U16 tlv_length = RRC_NULL;
	U8 *p_tag_length = RRC_NULL;


	RRC_ASSERT(pp_buffer != PNULL);
	RRC_ASSERT(*pp_buffer != PNULL);
	RRC_ASSERT(p_rim_information_req_intl != PNULL);

	/* Compose TL of TLV */
	tag = RIM_INFORMATION_REQUEST_TAG;

	rrc_cp_pack_U8(*pp_buffer, &tag, "<tag>");
	*pp_buffer += sizeof(U8);

	p_tag_length = *pp_buffer;
	*pp_buffer += sizeof (U16);

	/* Compose TLV  */
	if (p_rim_information_req_intl->bitmask & RIM_INFO_REQ_APP_ID_PRESENT)
	{
		if (RRC_FAILURE == rrc_compose_rim_app_id_intl(pp_buffer, &p_rim_information_req_intl->rim_app_id))
		{
			return RRC_FAILURE;
		}
	}

	/* Compose TLV  */
	if (p_rim_information_req_intl->bitmask & RIM_INFO_REQ_SEQ_NUM_PRESENT)
	{
		if (RRC_FAILURE == rrc_compose_rim_seq_num_intl(pp_buffer, &p_rim_information_req_intl->rim_seq_num))
		{
			return RRC_FAILURE;
		}
	}

	/* Compose TLV  */
	if (p_rim_information_req_intl->bitmask & RIM_INFO_REQ_PDU_IND_PRESENT)
	{
		if (RRC_FAILURE == rrc_compose_rim_pdu_ind_intl(pp_buffer, &p_rim_information_req_intl->rim_pdu_ind))
		{
			return RRC_FAILURE;
		}
	}

	/* Compose TLV  */
	if (RRC_FAILURE == rrc_compose_rim_protocol_ver_intl(pp_buffer, &p_rim_information_req_intl->rim_protocol_ver))
	{
		return RRC_FAILURE;
	}

	if (RIM_SON_TRANSFER_APP == p_rim_information_req_intl->rim_app_id.rim_app_id){
		/* Compose TLV  */
		if (RRC_FAILURE == rrc_compose_rim_son_transfer_app_idty_intl(pp_buffer, 
					&p_rim_information_req_intl->son_app_idty))
		{
			return RRC_FAILURE;
		}
	} else {
		/* Compose TLV  */
		if (RRC_FAILURE == rrc_compose_rim_req_app_container_intl(pp_buffer, 
					&p_rim_information_req_intl->app_cont))
		{
			return RRC_FAILURE;
		}
	}

	tlv_length = *pp_buffer - ( p_tag_length + sizeof(U16));
	rrc_cp_pack_U16(p_tag_length, &tlv_length, "<tlv_length>");
	return RRC_SUCCESS;
}
/* coverity_fix_54734_stop */


/*****************************************************************************
 * Function Name  : rrc_compose_ran_information_ack_rim 
 * Inputs         : pp_buffer - pointer to data
 *                  p_ran_information_ack_rim - pointer to 
 *                                                     ran_information_ack_rim_t
 *                  p_msg_length - pointr to the msg length 
 * Outputs        : p_ran_information_ack_rim-
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function compose the ran_information_ack_rim msg and 
 *                  fills p_ran_information_ack_rim with ran_information_ack_rim
 *                  info.
 ********************************************************************************/
rrc_return_et
rrc_compose_ran_information_ack_rim
(
    U8  **pp_buffer,
    ran_information_ack_rim_t *p_ran_information_ack_rim,
    U32 * p_msg_length
)
{
    U8 * p_start_buf = RRC_NULL;
    p_start_buf = *pp_buffer;


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_ran_information_ack_rim != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_ran_information_ack_rim->pdu_type, "pdu_type");
    *pp_buffer += sizeof(p_ran_information_ack_rim->pdu_type);

    /* Compose TLV  */
    if (p_ran_information_ack_rim->bitmask & RIM_ROUTING_INFO_TAG)
    {
        if (RRC_FAILURE == rrc_compose_rim_cell_id_intl(pp_buffer, &p_ran_information_ack_rim->dst_cell_id))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_ran_information_ack_rim->bitmask & RIM_ROUTING_INFO_TAG)
    {
        if (RRC_FAILURE == rrc_compose_rim_cell_id_intl(pp_buffer, &p_ran_information_ack_rim->src_cell_id))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_ran_information_ack_rim->bitmask & RIM_INFO_ACK_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_information_ack_intl(pp_buffer, &p_ran_information_ack_rim->rim_info))
        {
            return RRC_FAILURE;
        }
    }
    
    *p_msg_length = *pp_buffer - p_start_buf;
    *pp_buffer = p_start_buf;
    return RRC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : rrc_compose_rim_information_ack_intl 
 * Inputs         : p_rim_information_ack_intl - pointer to 
 *                                               rim_information_ack_intl_t
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function composes the _rim_information_ack msg.
 ********************************************************************************/
static
rrc_return_et
rrc_compose_rim_information_ack_intl
(
    U8  **pp_buffer,
    rim_information_ack_intl_t *p_rim_information_ack_intl
)
{
    U8 tag = RRC_NULL;
    U16 tlv_length = RRC_NULL;
    U8 *p_tag_length = RRC_NULL;


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_information_ack_intl != PNULL);

    /* Compose TL of TLV */
    tag = RIM_INFORMATION_ACK_TAG;

    rrc_cp_pack_U8(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(U8);

    p_tag_length = *pp_buffer;
    *pp_buffer += sizeof (U16);

    /* Compose TLV  */
    if (p_rim_information_ack_intl->bitmask & RIM_INFO_ACK_APP_ID_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_app_id_intl(pp_buffer, &p_rim_information_ack_intl->rim_app_id))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rim_information_ack_intl->bitmask & RIM_INFO_ACK_SEQ_NUM_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_seq_num_intl(pp_buffer, &p_rim_information_ack_intl->rim_seq_num))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rim_information_ack_intl->bitmask & RIM_INFO_ACK_PROTOCOL_VER_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_protocol_ver_intl(pp_buffer, &p_rim_information_ack_intl->rim_protocol_ver))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rim_information_ack_intl->bitmask & RIM_INFO_ACK_SON_TRNFR_APP_IDNTY_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_son_transfer_app_idty_intl(pp_buffer, &p_rim_information_ack_intl->son_app_idty))
        {
            return RRC_FAILURE;
        }
    }

    tlv_length = *pp_buffer - ( p_tag_length + sizeof(U16));
    rrc_cp_pack_U16(p_tag_length, &tlv_length, "<tlv_length>");
    return RRC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : rrc_compose_ran_information_app_error_rim 
 * Inputs         : pp_buffer - pointer to the data
 *                  p_ran_information_app_error_rim - pointer to 
 *                                          ran_information_app_error_rim_t
 *                  p_msg_length - pointer to nsg length 
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function composes ran_information_app_error msg.
 ********************************************************************************/
rrc_return_et
rrc_compose_ran_information_app_error_rim
(
    U8  **pp_buffer,
    ran_information_app_error_rim_t *p_ran_information_app_error_rim,
    U32 * p_msg_length
)
{
    U8 * p_start_buf = RRC_NULL;
    p_start_buf = *pp_buffer;


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_ran_information_app_error_rim != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_ran_information_app_error_rim->pdu_type, "pdu_type");
    *pp_buffer += sizeof(p_ran_information_app_error_rim->pdu_type);

    /* Compose TLV  */
    if (p_ran_information_app_error_rim->bitmask & RIM_ROUTING_INFO_TAG)
    {
        if (RRC_FAILURE == rrc_compose_rim_cell_id_intl(pp_buffer, &p_ran_information_app_error_rim->dst_cell_id))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_ran_information_app_error_rim->bitmask & RIM_ROUTING_INFO_TAG)
    {
        if (RRC_FAILURE == rrc_compose_rim_cell_id_intl(pp_buffer, &p_ran_information_app_error_rim->src_cell_id))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_ran_information_app_error_rim->bitmask & RIM_INFO_APP_ERROR_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_information_app_error_intl(pp_buffer, &p_ran_information_app_error_rim->rim_app_error))
        {
            return RRC_FAILURE;
        }
    }
    *p_msg_length = *pp_buffer - p_start_buf;
    *pp_buffer = p_start_buf;
    return RRC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rrc_compose_rim_info_app_error_contrn_nacc_intl 
 * Inputs         : pp_buffer - pointer to buffer 
 *                  p_rim_info_app_error_contrn_nacc_intl - pointer to 
 *                                  rim_info_app_error_contrn_nacc_intl_t 
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function composes rim_info_app_error_contrn_nacc msg
 ********************************************************************************/
static
rrc_return_et
rrc_compose_rim_info_app_error_contrn_nacc_intl
(
    U8  **pp_buffer,
    rim_info_app_error_contrn_nacc_intl_t *p_rim_info_app_error_contrn_nacc_intl
)
{


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_info_app_error_contrn_nacc_intl != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rim_info_app_error_contrn_nacc_intl->nacc_cause, "nacc_cause");
    *pp_buffer += sizeof(p_rim_info_app_error_contrn_nacc_intl->nacc_cause);

    /* Compose OCTET_STRING LIMITED_TILL_THE_END */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rim_info_app_error_contrn_nacc_intl->counter; loop++)
        {
#ifdef RRC_DEBUG
            RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_DETAILED, "Array index : %d", loop);
#endif
            rrc_cp_pack_U8(*pp_buffer, &p_rim_info_app_error_contrn_nacc_intl->nacc_info[loop], "nacc_info[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : rrc_compose_rim_info_app_error_contrn_si3_intl 
 * Inputs         : pp_buffer - pointer to data
 *                  p_rim_info_app_error_contrn_si3_intl - pointer to 
 *                                      rim_info_app_error_contrn_si3_intl_t  
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function composes rim_info_app_error_contrn_si3 msg.
 ********************************************************************************/
static
rrc_return_et
rrc_compose_rim_info_app_error_contrn_si3_intl
(
    U8  **pp_buffer,
    rim_info_app_error_contrn_si3_intl_t *p_rim_info_app_error_contrn_si3_intl
)
{


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_info_app_error_contrn_si3_intl != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rim_info_app_error_contrn_si3_intl->dummy, "dummy");
    *pp_buffer += sizeof(p_rim_info_app_error_contrn_si3_intl->dummy);

    return RRC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : rrc_compose_rim_info_app_error_contrn_mbms_intl 
 * Inputs         : pp_buffer - pointer to tha data to be filled 
 *                  p_rim_info_app_error_contrn_mbms_intl - pointer to 
 *                                       rim_info_app_error_contrn_mbms_intl_t
 * Outputs        : pp_buffre
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function composes the rim_info_app_error_contrn_mbms msg.
 ********************************************************************************/
static
rrc_return_et
rrc_compose_rim_info_app_error_contrn_mbms_intl
(
    U8  **pp_buffer,
    rim_info_app_error_contrn_mbms_intl_t *p_rim_info_app_error_contrn_mbms_intl
)
{


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_info_app_error_contrn_mbms_intl != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rim_info_app_error_contrn_mbms_intl->dummy, "dummy");
    *pp_buffer += sizeof(p_rim_info_app_error_contrn_mbms_intl->dummy);

    return RRC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : rrc_compose_rim_info_app_error_contrn_son_intl 
 * Inputs         : pp_buffer - pointer to the data
 *                  p_rim_info_app_error_contrn_son_intl - pointer to the 
 *                                        rim_info_app_error_contrn_son_intl_t
 * Outputs        : pp_buffer
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function composes rim_info_app_error_contrn_son msg.
 ********************************************************************************/
static
rrc_return_et
rrc_compose_rim_info_app_error_contrn_son_intl
(
    U8  **pp_buffer,
    rim_info_app_error_contrn_son_intl_t *p_rim_info_app_error_contrn_son_intl
)
{


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_info_app_error_contrn_son_intl != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rim_info_app_error_contrn_son_intl->dummy, "dummy");
    *pp_buffer += sizeof(p_rim_info_app_error_contrn_son_intl->dummy);

    return RRC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rrc_compose_rim_info_app_error_contrn_utra_si_intl 
 * Inputs         : pp_buffer - pointer to tha data 
 *                  p_rim_info_app_error_contrn_utra_si_intl - pointer to 
 *                                    rim_info_app_error_contrn_utra_si_intl_t
 * Outputs        : pp_buffer 
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function composes rim_info_app_error_contrn_utra msg.
 ********************************************************************************/
static
rrc_return_et
rrc_compose_rim_info_app_error_contrn_utra_si_intl
(
    U8  **pp_buffer,
    rim_info_app_error_contrn_utra_si_intl_t *p_rim_info_app_error_contrn_utra_si_intl
)
{


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_info_app_error_contrn_utra_si_intl != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rim_info_app_error_contrn_utra_si_intl->utra_si_cause, "utra_si_cause");
    *pp_buffer += sizeof(p_rim_info_app_error_contrn_utra_si_intl->utra_si_cause);

    /* Compose OCTET_STRING LIMITED_TILL_THE_END */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rim_info_app_error_contrn_utra_si_intl->counter; loop++)
        {
#ifdef RRC_DEBUG
            RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_DETAILED, "Array index : %d", loop);
#endif
            rrc_cp_pack_U8(*pp_buffer, &p_rim_info_app_error_contrn_utra_si_intl->utra_si_info[loop], "utra_si_info[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : rrc_compose_rim_info_app_error_container_intl 
 * Inputs         : pp_buffer - pointer to the data
 *                  p_rim_info_app_error_container_intl - pointer to 
 *                                         rim_info_app_error_container_intl_t
 * Outputs        : pp_buffer
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function composes the rim_info_app_error_container msg. 
 ********************************************************************************/
static
rrc_return_et
rrc_compose_rim_info_app_error_container_intl
(
    U8  **pp_buffer,
    rim_info_app_error_container_intl_t *p_rim_info_app_error_container_intl
)
{
    U8 tag = RRC_NULL;
    U16 tlv_length = RRC_NULL;
    U8 *p_tag_length = RRC_NULL;


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_info_app_error_container_intl != PNULL);

    /* Compose TL of TLV */
    tag = RIM_APP_INFO_ERROR_CONTAINER_TAG;

    rrc_cp_pack_U8(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(U8);

    p_tag_length = *pp_buffer;
    *pp_buffer += sizeof (U16);


    /* Optional element */
    if(p_rim_info_app_error_container_intl->bitmask & RIM_INFO_APP_ERROR_CONTAINER_NACC_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_rim_info_app_error_contrn_nacc_intl(pp_buffer, &p_rim_info_app_error_container_intl->nacc))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_info_app_error_container_intl->bitmask & RIM_INFO_APP_ERROR_CONTAINER_SI3_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_rim_info_app_error_contrn_si3_intl(pp_buffer, &p_rim_info_app_error_container_intl->si3))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_info_app_error_container_intl->bitmask & RIM_INFO_APP_ERROR_CONTAINER_MBMS_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_rim_info_app_error_contrn_mbms_intl(pp_buffer, &p_rim_info_app_error_container_intl->mbms))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_info_app_error_container_intl->bitmask & RIM_INFO_APP_ERROR_CONTAINER_SON_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_rim_info_app_error_contrn_son_intl(pp_buffer, &p_rim_info_app_error_container_intl->son))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_rim_info_app_error_container_intl->bitmask & RIM_INFO_APP_ERROR_CONTAINER_UTRA_SI_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_rim_info_app_error_contrn_utra_si_intl(pp_buffer, &p_rim_info_app_error_container_intl->utra_si))
    {
        return RRC_FAILURE;
    }
    }

    tlv_length = *pp_buffer - ( p_tag_length + sizeof(U16));
    rrc_cp_pack_U16(p_tag_length, &tlv_length, "<tlv_length>");
    return RRC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : rrc_compose_rim_information_app_error_intl 
 * Inputs         : pp_buffer - pointer of the data to be filled
 *                  p_rim_information_app_error_intl - pointer of
 *                            rim_information_app_error_intl_t
 * Outputs        : pp_buffer 
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function composes rim_information_app_error.
 ********************************************************************************/
static
rrc_return_et
rrc_compose_rim_information_app_error_intl
(
    U8  **pp_buffer,
    rim_information_app_error_intl_t *p_rim_information_app_error_intl
)
{
    U8 tag = RRC_NULL;
    U16 tlv_length = RRC_NULL;
    U8 *p_tag_length = RRC_NULL;


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_information_app_error_intl != PNULL);

    /* Compose TL of TLV */
    tag = RIM_INFORMATION_APP_ERROR_TAG;

    rrc_cp_pack_U8(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(U8);

    p_tag_length = *pp_buffer;
    *pp_buffer += sizeof (U16);

    /* Compose TLV  */
    if (p_rim_information_app_error_intl->bitmask & RIM_INFO_APP_ERROR_APP_ID_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_app_id_intl(pp_buffer, &p_rim_information_app_error_intl->rim_app_id))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rim_information_app_error_intl->bitmask & RIM_INFO_APP_ERROR_SEQ_NUM_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_seq_num_intl(pp_buffer, &p_rim_information_app_error_intl->rim_seq_num))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rim_information_app_error_intl->bitmask & RIM_INFO_APP_ERROR_PDU_IND_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_pdu_ind_intl(pp_buffer, &p_rim_information_app_error_intl->rim_pdu_ind))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rim_information_app_error_intl->bitmask & RIM_INFO_APP_ERROR_PROTOCOL_VER_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_protocol_ver_intl(pp_buffer, &p_rim_information_app_error_intl->rim_protocol_ver))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rim_information_app_error_intl->bitmask & RIM_INFO_APP_ERROR_APP_CONTAINER_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_info_app_error_container_intl(pp_buffer, &p_rim_information_app_error_intl->app_cont))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rim_information_app_error_intl->bitmask & RIM_INFO_APP_ERROR_SON_TRNFR_APP_IDNTY_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_son_transfer_app_idty_intl(pp_buffer, &p_rim_information_app_error_intl->son_app_idty))
        {
            return RRC_FAILURE;
        }
    }

    tlv_length = *pp_buffer - ( p_tag_length + sizeof(U16));
    rrc_cp_pack_U16(p_tag_length, &tlv_length, "<tlv_length>");
    return RRC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : rrc_parse_rim_app_id_intl 
 * Inputs         : p_rim_app_id_intl - poinetr to rim_app_identity_t
 *                  p_src - pointer to source 
 *                  length_left - pointer to the length left 
 *                  p_length_read - pointer to the length to read
 *                  p_cause - pointer to cause 
 * Outputs        : p_rim_app_id_intl
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function parses rim_app_id.
 ********************************************************************************/
static
rrc_return_et
rrc_parse_rim_app_id_intl
(
 rim_app_identity_t *p_rim_app_id_intl,
 U8  *p_src,
 S32 length_left,
 S32 *p_length_read,
 U8 *p_cause
 )
{


    *p_length_read = 0;


    /* This function parses rim_app_id_intl */
    RRC_TRACE(RRC_DETAILED, "Parsing rim_app_id_intl");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        *p_cause = RIM_CAUSE_SEMANTICALLY_INCORRECT_PDU;
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(p_rim_app_id_intl, p_src + *p_length_read, "rim_app_id");
    *p_length_read += sizeof(U8);

    if ((*(p_rim_app_id_intl) < RIM_NACC_APP) || 
            (*(p_rim_app_id_intl) > RIM_UTRA_SI_APP))
    {
        RRC_TRACE( RRC_WARNING, "Recived incorrect app identity %u", *(p_rim_app_id_intl));
        *p_cause = RIM_CAUSE_UNKNOWN_RIM_APPLICATION_IDENTITY;
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

/*****************************************************************************
 * Function Name  : rrc_parse_rim_seq_num_intl 
 * Inputs         : p_rim_seq_num_intl - pointer to rim_pdu_seq_num_t
 *                  p_src - pointer to source 
 *                  length_left - pointer to length left unread 
 *                  p_length_read - pointer to length to read
 *                  p_cause - pointer to cause
 * Outputs        : p_rim_seq_num_intl
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function parses rim_seq_num.
 ********************************************************************************/
static
rrc_return_et
rrc_parse_rim_seq_num_intl
(
 rim_pdu_seq_num_t *p_rim_seq_num_intl,
 U8  *p_src,
 S32 length_left,
 S32 *p_length_read,
 U8 *p_cause
 )
{


    *p_length_read = 0;


    /* This function parses rim_seq_num_intl */
    RRC_TRACE(RRC_DETAILED, "Parsing rim_seq_num_intl");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        *p_cause = RIM_CAUSE_SEMANTICALLY_INCORRECT_PDU;
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(p_rim_seq_num_intl, p_src + *p_length_read, "rim_seq_num");
    *p_length_read += sizeof(U32);

    return(RRC_SUCCESS);
}

/*****************************************************************************
 * Function Name  : rrc_parse_rim_pdu_ind_intl 
 * Inputs         : p_rim_pdu_ind_intl - pointer to rim_pdu_ind_t
 *                  p_src - pointer to source
 *                  length_left - pointer to the length left to read
 *                  p_length_read - pointer to the length to read
 *                  p_cause - pointer to cause
 * Outputs        : p_rim_pdu_ind_intl
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function parses rim_pdu_ind_intl .
 ********************************************************************************/
static
rrc_return_et
rrc_parse_rim_pdu_ind_intl
(
 rim_pdu_ind_t *p_rim_pdu_ind_intl,
 U8  *p_src,
 S32 length_left,
 S32 *p_length_read,
 U8 *p_cause
 )
{
    U8  pdu_ind = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_pdu_ind_intl, 0, sizeof(rim_pdu_ind_t));

    /* This function parses rim_pdu_ind_intl */
    RRC_TRACE(RRC_DETAILED, "Parsing rim_pdu_ind_intl");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
         RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        *p_cause = RIM_CAUSE_SEMANTICALLY_INCORRECT_PDU;
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&pdu_ind, p_src + *p_length_read, "rim_pdu_ind");
    *p_length_read += sizeof(U8);

    if (pdu_ind & 0x01)
    {
        p_rim_pdu_ind_intl->ack = RIM_ACK_REQUESTED;
    }
    else
    {
        p_rim_pdu_ind_intl->ack = RIM_NO_ACK_REQUESTED;
    }

    pdu_ind = pdu_ind >>1;
    p_rim_pdu_ind_intl->pdu_ext = pdu_ind;

    /* range check not done here as range is different for Info Req and Info PDU
     * */
    return(RRC_SUCCESS);
}

/*****************************************************************************
 * Function Name  : rrc_parse_rim_protocol_ver_intl 
 * Inputs         : p_rim_protocol_ver_intl - pointer to rim_protocol_ver_t
 *                  p_src - pointer to source
 *                  length_left - pointer to length left unread
 *                  p_length_read - poinert to the length to read
 *                  p_cause - pointer to cause
 * Outputs        : p_rim_protocol_ver_intl
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function parses rim_protocol_ver.
 ********************************************************************************/
static
rrc_return_et
rrc_parse_rim_protocol_ver_intl
(
 rim_protocol_ver_t *p_rim_protocol_ver_intl,
 U8  *p_src,
 S32 length_left,
 S32 *p_length_read,
 U8 *p_cause
 )
{


    *p_length_read = 0;
    /* This function parses rim_protocol_ver_intl */
    RRC_TRACE(RRC_DETAILED, "Parsing rim_protocol_ver_intl");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        *p_cause = RIM_CAUSE_SEMANTICALLY_INCORRECT_PDU;
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(p_rim_protocol_ver_intl, p_src + *p_length_read, "rim_protocol_ver");
    *p_length_read += sizeof(U8);

    if (*p_rim_protocol_ver_intl != 0x01)
    {
        RRC_TRACE(RRC_WARNING, "Not supported version received..");
        *p_cause = RIM_CAUSE_PDU_NOT_COMPATIBLE_WITH_THE_FEATURE_SET;
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef RRC_DEBUG
        RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_WARNING, "Incoming message damaged!");
#endif
        *p_cause = RIM_CAUSE_SEMANTICALLY_INCORRECT_PDU;
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}


/*****************************************************************************
 * Function Name  : rrc_parse_rim_son_transfer_app_idty_intl 
 * Inputs         : p_rim_son_transfer_app_idty_intl - pointer to 
 *                                        p_rim_son_transfer_app_idty_intl
 *                  p_src - pointer to the source  
 *                  length_left - pointer to the length left as unread 
 *                  p_length_read - pointer to the length to be read 
 * Outputs        : p_rim_son_transfer_app_idty_intl
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function parses the rim_son_transfer_app_idty.
 ********************************************************************************/
static
rrc_return_et
rrc_parse_rim_son_transfer_app_idty_intl
(
    rim_son_transfer_app_idty_intl_t *p_rim_son_transfer_app_idty_intl,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{


    *p_length_read = 0;

    memset_wrapper(p_rim_son_transfer_app_idty_intl, 0, sizeof(rim_son_transfer_app_idty_intl_t));

    /* This function parses rim_son_transfer_app_idty_intl */
#ifdef RRC_DEBUG
    RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_DETAILED, "Parsing rim_son_transfer_app_idty_intl");
#endif

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
#ifdef RRC_DEBUG
        RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_WARNING, "Incoming message damaged!");
#endif
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_son_transfer_app_idty_intl->rim_son_app_idty, p_src + *p_length_read, "rim_son_app_idty");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rim_son_transfer_app_idty_intl->rim_son_app_idty > 1))
    {
#ifdef RRC_DEBUG
        RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_WARNING, "Parameter [p_rim_son_transfer_app_idty_intl->rim_son_app_idty] should be in range [%d <= 1] !", p_rim_son_transfer_app_idty_intl->rim_son_app_idty);
#endif
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
#ifdef RRC_DEBUG
        RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_WARNING, "Incoming message damaged!");
#endif
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}


/*****************************************************************************
 * Function Name  : rrc_il_parse_ran_information_rim 
 * Inputs         : p_ran_information_rim - pointer to rim_information_t
 *                  p_src - pointer to source 
 *                  length_left - pointer to length left as unread
 *                  p_length_read - pointer to the length to read
 *                  p_cause - pointer to teh cause
 * Outputs        : p_ran_information_rim
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function parses ran_information_rim.
 ********************************************************************************/
rrc_return_et
rrc_il_parse_ran_information_rim
(
 rim_information_t  *p_ran_information_rim,
 U8  *p_src,
 S32 length_left,
 S32 *p_length_read,
 U8  *p_cause
 )
{
    S32 length_read = 0;


    *p_length_read = 0;

    p_ran_information_rim->bitmask= 0;
    /* This function parses ran_information_rim */
    RRC_TRACE(RRC_DETAILED, "Parsing ran_information_rim");


    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        return RRC_FAILURE;
    }

    /* Move pointer for pdu */
    *p_length_read += sizeof(U8);

    /* Parse TLVs */
    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(RIM_PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U8(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U8);

        /* Unpack length */
        if (*(p_src + *p_length_read) > RIM_MAX_LEN_IN_SINGLE_OCTET)
        {
            rrc_cp_unpack_U8(&length, p_src + *p_length_read, "<length>");
            length = length & 0x7F;
            *p_length_read += sizeof(U8);
        }
        else
        {
            rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
            *p_length_read += sizeof(U16);
        }
        /* Check * whether * length * is * enough * to * store * TLV * */
        if (length  > (length_left - *p_length_read))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            *p_cause = RIM_CAUSE_SEMANTICALLY_INCORRECT_PDU;
            return(RRC_FAILURE);
        }

        switch(tag)
        {
            case RIM_ROUTING_INFO_TAG:
                /* rim_cell_id_intl TLV */
                {
                    if (RRC_FAILURE == rrc_il_parse_rim_routing_info(
                                &p_ran_information_rim->dst_cell,
                                p_src + *p_length_read,
                                length,
                                &length_read
                                ))/*SPR 17777 +-*/
                    {
                        return RRC_FAILURE;
                    }
                    *p_length_read += length;
                    if ((length_left - *p_length_read) <= (S32)(RIM_PUP_TL_LENGTH))
                    {
                        RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
                        *p_cause = RIM_CAUSE_SEMANTICALLY_INCORRECT_PDU;
                        return(RRC_FAILURE);
                    }
                    rrc_cp_unpack_U8(&tag, p_src + *p_length_read, "<tag>");
                    *p_length_read += sizeof(U8);

                    UNPACK_TLV_LENGTH
                        if (length  > (length_left - *p_length_read))
                        {
                            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
                            *p_cause = RIM_CAUSE_SEMANTICALLY_INCORRECT_PDU;
                            return(RRC_FAILURE);
                        }
                    if (RRC_FAILURE == rrc_il_parse_rim_routing_info(
                                &p_ran_information_rim->src_cell,
                                p_src + *p_length_read,
                                length,
                                &length_read
                                ))/*SPR 17777 +-*/
                    {
                        return RRC_FAILURE;
                    }
                    *p_length_read += length;
                    break;
                }
            case RIM_INFORMATION_TAG:
                {
                /* rim_information_intl TLV */
                if (RRC_FAILURE == rrc_il_parse_rim_information_intl(
                            p_ran_information_rim,
                            p_src + *p_length_read,
                            length,
                            &length_read,p_cause))
                {
                    return RRC_FAILURE;
                }

                p_ran_information_rim->bitmask |= RIM_INFO_PRESENT;

                *p_length_read += length;

                break;
                }
            default:
                /* Unknown tag*/
                *p_cause = RIM_CAUSE_SEMANTICALLY_INCORRECT_PDU;
                return RRC_FAILURE;

        }
    }

    if (*p_length_read > length_left)
    {
        *p_cause = RIM_CAUSE_SEMANTICALLY_INCORRECT_PDU;
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}


/*****************************************************************************
 * Function Name  : rrc_parse_rim_info_app_container_intl 
 * Inputs         : p_rim_info_app_container_intl - pointer to 
 *                                               p_rim_info_app_container_intl
 *                  p_src - pointer to source 
 *                  length_left - pointer to the length left as unread
 *                  p_length_read - pointer to length to read
 *                  app_idty - pointer to rim_app_identity_t
 * Outputs        : p_rim_info_app_container_intl
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : this function parses rim_info_app_container msg 
 ********************************************************************************/
static
rrc_return_et
rrc_parse_rim_info_app_container_intl
(
 rim_info_app_container_t *p_rim_info_app_container_intl,
 U8  *p_src,
 S32 length_left,
 S32 *p_length_read,
 rim_app_identity_t app_idty
 )
{
    U8 i;
    /* SPR 19610 fix start */
    U8 sourceCellId = 0;
    /* SPR 19610 fix end */

    *p_length_read = 0;

    p_rim_info_app_container_intl->bitmask = 0;

    /* This function parses rim_info_app_container_intl */
    RRC_TRACE(RRC_DETAILED, "Parsing rim_info_app_container_intl");
    switch(app_idty)
    {
        case RIM_NACC_APP :
            {
                l3_memcpy_wrapper(p_rim_info_app_container_intl->nacc.cell_idty.routing_idty.plmn.plmn,
                        p_src + *p_length_read, RIM_PLMN_OCTETS);
                *p_length_read += RIM_PLMN_OCTETS;

                l3_memcpy_wrapper(p_rim_info_app_container_intl->nacc.cell_idty.routing_idty.lac,
                        p_src + *p_length_read, RIM_LAC_OCTETS);
                *p_length_read += RIM_LAC_OCTETS;

                p_rim_info_app_container_intl->nacc.cell_idty.routing_idty.rac = *(p_src + *p_length_read);
                *p_length_read += sizeof(U8);

                l3_memcpy_wrapper(p_rim_info_app_container_intl->nacc.cell_idty.geran_cell_idty,
                        p_src + *p_length_read, RIM_GERAN_CELL_OCTETS);
                *p_length_read += RIM_GERAN_CELL_OCTETS;

                U8 temp=0;

                rrc_cp_unpack_U8(&temp, p_src + *p_length_read, "<temp>");
                *p_length_read += sizeof(U8);

                if (temp & 0x01)
                {
                    p_rim_info_app_container_intl->nacc.type = RIM_PSI_MSG;
                }
                else
                {
                    p_rim_info_app_container_intl->nacc.type = RIM_SI_MSG;
                }

                temp = temp >>1;
                p_rim_info_app_container_intl->nacc.num_si_psi = temp;


                if (p_rim_info_app_container_intl->nacc.type == RIM_SI_MSG)
                {
                    for (i=0; i<temp; i++)
                    {
                        if ((length_left - *p_length_read) < RIM_MAX_SI_PAYLOAD_INDIV)
                        {
                            RRC_TRACE(RRC_WARNING, "Insufficient Length of Buffer Received ");
                            return(RRC_FAILURE);

                        }
                        l3_memcpy_wrapper(p_rim_info_app_container_intl->nacc.si_psi_indiv[i].si_psi, p_src + *p_length_read, RIM_MAX_SI_PAYLOAD_INDIV);

                        *p_length_read += RIM_MAX_SI_PAYLOAD_INDIV;

                    }
                }
                else
                {
                    for (i=0; i<temp; i++)
                    {
                        if ((length_left - *p_length_read) < RIM_MAX_SI_PSI_PAYLOAD_INDIV)
                        {
                            RRC_TRACE(RRC_WARNING, "Insufficient Length of Buffer Received ");
                            return(RRC_FAILURE);

                        }
                        l3_memcpy_wrapper(p_rim_info_app_container_intl->nacc.si_psi_indiv[i].si_psi, p_src + *p_length_read, RIM_MAX_SI_PSI_PAYLOAD_INDIV);

                        *p_length_read += RIM_MAX_SI_PSI_PAYLOAD_INDIV;

                    }
                }

                p_rim_info_app_container_intl->bitmask |= RIM_INFO_APP_CONTAINER_NACC_PRESENT;

                break;
            }
        case RIM_UTRA_SI_APP:
            {
                /* SPR 19610 fix start */
                /* Refer 9.2.1.61 of 25.413 */
                /* Parse/Unpack parameter of basic type "Choice Source Cell Identifier" GERAN or UTRAN */
                rrc_cp_unpack_U8(&sourceCellId, p_src + *p_length_read, "sourceCellId");
                *p_length_read += sizeof(U8);
                /* in this case, the type should be UTRAN and not GERAN */
                if (T_ranap_SourceCellID_sourceUTRANCellID != sourceCellId)
                {
                    RRC_TRACE(RRC_DETAILED, "RIM INFO: Incorrect Cell type received :%u", sourceCellId);
                    /* do not return as it is not expected */
                }
                /* SPR 19610 fix end */

                l3_memcpy_wrapper(p_rim_info_app_container_intl->utra_si.rpt_cell_id.plmn.plmn,
                        p_src + *p_length_read, RIM_PLMN_OCTETS);
                *p_length_read += RIM_PLMN_OCTETS;

                rrc_cp_unpack_U32(&p_rim_info_app_container_intl->utra_si.rpt_cell_id.cell_id, p_src + *p_length_read, "cell_id");
                *p_length_read += sizeof(U32);

                if ((length_left - *p_length_read) > RIM_MAX_UTRA_SI_PAYLOAD)
                {
                    RRC_TRACE(RRC_WARNING, " incorrect length for Utra si payload");
                    return(RRC_FAILURE);
                }

                p_rim_info_app_container_intl->utra_si.len =length_left - *p_length_read;
                l3_memcpy_wrapper(p_rim_info_app_container_intl->utra_si.si,p_src + *p_length_read,
                        p_rim_info_app_container_intl->utra_si.len);
                *p_length_read += p_rim_info_app_container_intl->utra_si.len;

                p_rim_info_app_container_intl->bitmask |= RIM_INFO_APP_CONTAINER_UTRA_SI_PRESENT;
                break;
            }
        default :
            RRC_TRACE(RRC_WARNING, "APP ID not handled currently %u..",app_idty);
            return(RRC_FAILURE);

    }

    return(RRC_SUCCESS);
}

/*****************************************************************************
 * Function Name  : rrc_il_parse_rim_information_intl 
 * Inputs         : p_rim_information_intl - pointer to rim_information_t
 *                  p_src - pointer to source 
 *                  length_left - pointer to the unread length 
 *                  p_length_read - pointer to length to be read
 *                  p_cause - pointer to cause
 * Outputs        : p_rim_information_intl
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function parses rim_information_intl.
 ********************************************************************************/
/* MODIFIED */
static
rrc_return_et
rrc_il_parse_rim_information_intl
(
 rim_information_t *p_rim_information_intl,
 U8  *p_src,
 S32 length_left,
 S32 *p_length_read,
 U8  *p_cause
 )
{
    S32 length_read = 0;

    *p_length_read = 0;
    U16 mand_bitmask = 0;


    /* This function parses rim_information_intl */
    RRC_TRACE(RRC_DETAILED, "Parsing rim_information_intl");

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U8(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U8);

        /* Unpack length */
        UNPACK_TLV_LENGTH
            if (length  > (length_left - *p_length_read))
            {
                RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
                *p_cause = RIM_CAUSE_SEMANTICALLY_INCORRECT_PDU;
                return(RRC_FAILURE);
            }

        switch(tag)
        {
            case RIM_APP_IDENTITY_TAG:
                /* rim_app_id_intl TLV */
                if (RRC_FAILURE == rrc_parse_rim_app_id_intl(
                            &p_rim_information_intl->app_idty,
                            p_src + *p_length_read,
                            length,
                            &length_read,
                            p_cause))
                {
                    return RRC_FAILURE;
                }

                if ((p_rim_information_intl->app_idty < RIM_NACC_APP) || 
                        (p_rim_information_intl->app_idty > RIM_UTRA_SI_APP))
                {
                    RRC_TRACE( RRC_WARNING, "Recived incorrect app idtyi %u", p_rim_information_intl->app_idty);
                    *p_cause = RIM_CAUSE_UNKNOWN_RIM_APPLICATION_IDENTITY;
                    return RRC_FAILURE;

                }
                *p_length_read += length;

                mand_bitmask |= RIM_MAND_APP_ID_PRESENT;

                break;
            case RIM_SEQUENCE_NUM_TAG:
                /* rim_seq_num_intl TLV */
                if (RRC_FAILURE == rrc_parse_rim_seq_num_intl(
                            &p_rim_information_intl->seq_num,
                            p_src + *p_length_read,
                            length,
                            &length_read,
                            p_cause))
                {
                    return RRC_FAILURE;
                }

                *p_length_read += length;

                mand_bitmask |= RIM_MAND_SEQ_NUM_PRESENT;

                break;
            case RIM_PDU_IND_TAG:
                /* rim_pdu_ind_intl TLV */
                if (RRC_FAILURE == rrc_parse_rim_pdu_ind_intl(
                            (rim_pdu_ind_t *)&p_rim_information_intl->pdu_ind,
                            p_src + *p_length_read,
                            length,
                            &length_read,
                            p_cause))
                {
                    return RRC_FAILURE;
                }

                if (p_rim_information_intl->pdu_ind.pdu_ext > RAN_INFO_END)
                {
                    *p_cause = RIM_CAUSE_PDU_NOT_COMPATIBLE_WITH_THE_FEATURE_SET;
                    return RRC_FAILURE;
                }

                *p_length_read += length;

                mand_bitmask |= RIM_MAND_PDU_IND_PRESENT;

                break;
            case RIM_PROTOCOL_VER_NUM_TAG:
                /* rim_protocol_ver_intl TLV */
                if (RRC_FAILURE == rrc_parse_rim_protocol_ver_intl(
                            &p_rim_information_intl->ver,
                            p_src + *p_length_read,
                            length,
                            &length_read,
                            p_cause))
                {
                    return RRC_FAILURE;
                }

                p_rim_information_intl->bitmask |= RIM_INFO_PROTOCOL_VER_PRESENT;

                *p_length_read += length;

                break;
              /* SPR_18011_FIX_START */
            case RIM_APP_INFO_CONTAINER_TAG:
              /* SPR_18011_FIX_END */
                /* rim_info_app_container_intl TLV */
                if (RRC_FAILURE == rrc_parse_rim_info_app_container_intl(
                            &p_rim_information_intl->app_cont,
                            p_src + *p_length_read,
                            length,
                            &length_read,
                            p_rim_information_intl->app_idty
                            ))
                {
                    return RRC_FAILURE;
                }

                p_rim_information_intl->bitmask |= RIM_INFO_APP_CONTAINER_PRESENT;

                *p_length_read += length;

                break;

            case RIM_APP_INFO_ERROR_CONTAINER_TAG:
                {
                    if (RRC_FAILURE == rrc_parse_rim_info_app_error_container_intl(
                                &p_rim_information_intl->err_cont,
                                p_src + *p_length_read,
                                /*SPR 17777 +-*/
                                &length_read, p_rim_information_intl->app_idty))
                    {
                        return RRC_FAILURE;
                    }
                    p_rim_information_intl->bitmask |= RIM_INFO_APP_ERR_CONTAINER_PRESENT;
                    *p_length_read += length;
                    break;
                }

            case RIM_SON_TRANSFER_APP_ID_TAG:
                {
                    RRC_TRACE(RRC_WARNING, "Received tag not currently supportedi %u", tag);
                    *p_cause = RIM_CAUSE_PDU_NOT_COMPATIBLE_WITH_THE_FEATURE_SET;
                    return RRC_FAILURE;
                }
                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                *p_length_read += length;
                break;
        }
    }

    if (!( mand_bitmask & ((U8)(RIM_MAND_APP_ID_PRESENT | RIM_MAND_SEQ_NUM_PRESENT |
                    RIM_MAND_PDU_IND_PRESENT))))
    {
        /* mandatory IE absent */
        *p_cause = RIM_CAUSE_MISSING_MANDATORY_IE;
        return RRC_FAILURE;
    }

    if ((p_rim_information_intl->bitmask & RIM_INFO_APP_CONTAINER_PRESENT) &&
            (p_rim_information_intl->bitmask & RIM_INFO_APP_ERR_CONTAINER_PRESENT))
    {
        /* incorrect conditional IE */
        *p_cause = RIM_CAUSE_UNEXPECTED_CONDITIONAL_IE;
        return RRC_FAILURE;
    }

    if (!((p_rim_information_intl->bitmask & RIM_INFO_APP_CONTAINER_PRESENT) ||
            (p_rim_information_intl->bitmask & RIM_INFO_APP_ERR_CONTAINER_PRESENT)))
    {
        /* incorrect conditional IE */
        *p_cause = RIM_CAUSE_MISSING_CONDITIONAL_IE;
        return RRC_FAILURE;
    }
    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        *p_cause = RIM_CAUSE_SEMANTICALLY_INCORRECT_PDU;
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}


/*****************************************************************************
 * Function Name  : rrc_il_parse_ran_information_error_rim 
 * Inputs         : p_ran_information_error_rim - pointer to 
 *                                                 rim_information_err_t
 *                  p_src - pointer to the source 
 *                  length_left - pointer to length unread 
 *                  p_length_read - pointer of the buffer to be read 
 * Outputs        : p_ran_information_error_rim
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_SUCCESS
 * Description    : This function parses ran_information_error_rim msg. 
 ********************************************************************************/
rrc_return_et
rrc_il_parse_ran_information_error_rim
(
 rim_information_err_t *p_ran_information_error_rim,
 U8  *p_src,
 S32 length_left,
 S32 *p_length_read
 )
{
    S32 length_read = 0;

    /*SPR 17777 +-*/
    *p_length_read = 0;


    /* This function parses ran_information_error_rim */
    RRC_TRACE(RRC_DETAILED, "Parsing ran_information_error_rim");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    *p_length_read += sizeof(U8);

    /* Parse TLVs */

    /* Check whether size is enough to
     *      * store TL */
    while ((length_left - *p_length_read) >= (S32)(RIM_PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag*/
        rrc_cp_unpack_U8(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U8);

        /* Unpack length */
        UNPACK_TLV_LENGTH        
            /* Check  whether  length  is enough to store TLV */
            if( length > (length_left -*p_length_read))
            {
                RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
                return(RRC_FAILURE);
            }

        switch(tag)
        {
            case RIM_ROUTING_INFO_TAG:

                /* rim_cell_id_intl TLV */
                {
                    if (RRC_FAILURE == rrc_il_parse_rim_routing_info(
                                &p_ran_information_error_rim->dst_cell,
                                p_src + *p_length_read,
                                length,
                                &length_read))/*SPR 17777 +-*/
                    {
                        return RRC_FAILURE;
                    }
                    *p_length_read += length;
                    if ((length_left - *p_length_read) <= (S32)(RIM_PUP_TL_LENGTH))
                    {
                        RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
                        return(RRC_FAILURE);
                    }
                    rrc_cp_unpack_U8(&tag, p_src + *p_length_read, "<tag>");
                    *p_length_read += sizeof(U8);

                    UNPACK_TLV_LENGTH
                        if (length  > (length_left - *p_length_read))
                        {
                            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
                            return(RRC_FAILURE);
                        }
                    if (RRC_FAILURE == rrc_il_parse_rim_routing_info(
                                &p_ran_information_error_rim->src_cell,
                                p_src + *p_length_read,
                                length,
                                &length_read))/*SPR 17777 +-*/
                    {
                        return RRC_FAILURE;
                    }
                    *p_length_read += length;
                    break;
                }
            case RIM_INFORMATION_ERROR_TAG:

                /* rim_information_error_intl TLV */
                if (RRC_FAILURE == rrc_il_parse_rim_information_error_intl(
                            p_ran_information_error_rim,
                            p_src + *p_length_read,
                            length,
                            &length_read))
                {
                    return RRC_FAILURE;
                }

                p_ran_information_error_rim->bitmask |= RIM_INFO_ERROR_PRESENT;

                *p_length_read += length;

                break;
            default:
#ifdef RRC_DEBUG
                RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_WARNING, "Received unknown tag %u", tag);
#endif
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
        }
    }

    if (*p_length_read > length_left)
    {
#ifdef RRC_DEBUG
        RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_WARNING, "Incoming message damaged!");
#endif
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

/*****************************************************************************
 * Function Name  : rrc_parse_rim_cause_intl 
 * Inputs         : p_rim_cause_intl - pointer to the rim_cause_intl_t
 *                  p_src - pointer to the source 
 *                  length_left - pointer to the length left as unread
 *                  p_length_read - pointer to the length to be read 
 * Outputs        : p_rim_cause_intl
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function parses rim_cause msg. 
 ********************************************************************************/
static
rrc_return_et
rrc_parse_rim_cause_intl
(
    rim_cause_intl_t *p_rim_cause_intl,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{


    *p_length_read = 0;

    memset_wrapper(p_rim_cause_intl, 0, sizeof(rim_cause_intl_t));

    /* This function parses rim_cause_intl */
#ifdef RRC_DEBUG
    RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_DETAILED, "Parsing rim_cause_intl");
#endif

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
#ifdef RRC_DEBUG
        RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_WARNING, "Incoming message damaged!");
#endif
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_cause_intl->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
#ifdef RRC_DEBUG
        RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_WARNING, "Incoming message damaged!");
#endif
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

/*****************************************************************************
 * Function Name  : rrc_parse_rim_error_pdu_intl 
 * Inputs         : p_rim_error_pdu_intl - pointer to the rim_error_pdu_intl_t
 *                  p_src - pointer to source 
 *                  length_left - pointer to length left as unread 
 *                  p_length_read - pointer to the length of msg to read 
 * Outputs        : p_rim_error_pdu_intl
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function parses rim_error_pdu.
 ********************************************************************************/
static
rrc_return_et
rrc_parse_rim_error_pdu_intl
(
    rim_error_pdu_intl_t *p_rim_error_pdu_intl,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{


    *p_length_read = 0;

    memset_wrapper(p_rim_error_pdu_intl, 0, sizeof(rim_error_pdu_intl_t));

    /* This function parses rim_error_pdu_intl */
#ifdef RRC_DEBUG
    RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_DETAILED, "Parsing rim_error_pdu_intl");
#endif

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
#ifdef RRC_DEBUG
        RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_WARNING, "Incoming message damaged!");
#endif
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rim_error_pdu_intl->error_pdu, p_src + *p_length_read, "error_pdu");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
#ifdef RRC_DEBUG
        RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_WARNING, "Incoming message damaged!");
#endif
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

/*****************************************************************************
 * Function Name  : rrc_il_parse_rim_information_error_intl 
 * Inputs         : p_rim_information_error_intl - pointer to 
 *                                                 rim_information_err_t
 *                  p_src - pointer to source 
 *                  length_left - pointer to the length left as unread 
 *                  p_length_read - pointer to the length to read 
 * Outputs        : p_rim_information_error_intl
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function parses rim_information_error.
 ********************************************************************************/
static
rrc_return_et
rrc_il_parse_rim_information_error_intl
(
 rim_information_err_t *p_rim_information_error_intl,
 U8  *p_src,
 S32 length_left,
 S32 *p_length_read
 )
{
    rim_son_transfer_app_idty_intl_t rim_son_transfer_app_idty_temp;
    rim_error_pdu_intl_t rim_err_pdu_temp;
    rim_cause_intl_t rim_cause_temp;
    S32 length_read = 0;
    U8 cause = 0;

    *p_length_read = 0;

    memset_wrapper(p_rim_information_error_intl, 0, sizeof(rim_information_err_t));

    /* This function parses rim_information_error_intl */
#ifdef RRC_DEBUG
    RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_DETAILED, "Parsing rim_information_error_intl");
#endif

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        UNPACK_TLV_LENGTH
            if (length  > (length_left - *p_length_read))
            {
                RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
                cause = 0xFF;
                return(RRC_FAILURE);
            }

        switch(tag)
        {
            case RIM_APP_IDENTITY_TAG:
                /* rim_app_id_intl TLV */
                if (RRC_FAILURE == rrc_parse_rim_app_id_intl(
                            &p_rim_information_error_intl->app_idty,
                            p_src + *p_length_read,
                            length,
                            &length_read,&cause))
                {
                    return RRC_FAILURE;
                }

                p_rim_information_error_intl->bitmask |= RIM_INFO_ERROR_APP_ID_PRESENT;

                *p_length_read += length;

                break;
            case RIM_CAUSE_TAG:
                /* rim_cause_intl TLV */
                if (RRC_FAILURE == rrc_parse_rim_cause_intl(
                            &rim_cause_temp,
                            p_src + *p_length_read,
                            length,
                            &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rim_information_error_intl->bitmask |= RIM_INFO_ERROR_CAUSE_PRESENT;

                *p_length_read += length;

                break;
            case RIM_PROTOCOL_VER_NUM_TAG:
                /* rim_protocol_ver_intl TLV */
                if (RRC_FAILURE == rrc_parse_rim_protocol_ver_intl(
                            &p_rim_information_error_intl->ver,
                            p_src + *p_length_read,
                            length,
                            &length_read,&cause))
                {
                    return RRC_FAILURE;
                }

                p_rim_information_error_intl->bitmask |= RIM_INFO_ERROR_PROTOCOL_VER_PRESENT;

                *p_length_read += length;

                break;
            case RIM_ERROR_PDU_TAG:
                /* rim_error_pdu_intl TLV */
                if (RRC_FAILURE == rrc_parse_rim_error_pdu_intl(
                            &rim_err_pdu_temp,
                            p_src + *p_length_read,
                            length,
                            &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rim_information_error_intl->bitmask |= RIM_INFO_ERROR_PDU_PRESENT;

                *p_length_read += length;

                break;
            case RIM_SON_TRANSFER_APP_ID_TAG:
                /* rim_son_transfer_app_idty_intl TLV */
                if (RRC_FAILURE == rrc_parse_rim_son_transfer_app_idty_intl(
                    &rim_son_transfer_app_idty_temp,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rim_information_error_intl->bitmask |= RIM_INFO_ERROR_SON_TRNFR_APP_IDNTY_PRESENT;

                *p_length_read += length;

                break;
            default:
#ifdef RRC_DEBUG
                RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_WARNING, "Received unknown tag %u", tag);
#endif
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
        }
    }

    if (*p_length_read > length_left)
    {
#ifdef RRC_DEBUG
        RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_WARNING, "Incoming message damaged!");
#endif
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}


/*****************************************************************************
 * Function Name  : rrc_parse_rim_info_app_error_container_intl 
 * Inputs         : p_rim_info_app_error_container_intl - pointer to the 
 *                                                rim_app_err_container_t
 *                  p_src - pointer to source 
 *                  length_left - pointer to the length left as unread
 *                  p_length_read - pointer to the length to read 
 *                  app_idty - pointer to the rim_app_identity_t
 * Outputs        : p_rim_info_app_error_container_intl
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function parses rim_info_app_error_container.
 ********************************************************************************/
/* MODIFIED */
static
rrc_return_et
rrc_parse_rim_info_app_error_container_intl
(
 rim_app_err_container_t *p_rim_info_app_error_container_intl,
 U8  *p_src,
 /*SPR 17777 +-*/
 S32 *p_length_read,
 rim_app_identity_t app_idty
 )
{


    *p_length_read = 0;

    /* This function parses rim_info_app_error_container_intl */
    RRC_TRACE(RRC_DETAILED, "Parsing rim_info_app_error_container_intl");

    p_rim_info_app_error_container_intl->bitmask = 0;


    switch(app_idty)
    {
        case RIM_NACC_APP :
            {

                rrc_cp_unpack_U32(&p_rim_info_app_error_container_intl->nacc.cause, p_src + *p_length_read, "<temp>");
                *p_length_read += sizeof(U32);

                p_rim_info_app_error_container_intl->nacc.bitmask = 0;
                p_rim_info_app_error_container_intl->bitmask |= RIM_APP_ERR_CONTAINER_NACC_PRESENT;

                break;
            }
        case RIM_UTRA_SI_APP:
            {
                rrc_cp_unpack_U32(&p_rim_info_app_error_container_intl->utra_si.cause, p_src + *p_length_read, "<temp>");
                *p_length_read += sizeof(U32);

                p_rim_info_app_error_container_intl->utra_si.bitmask = 0;
                p_rim_info_app_error_container_intl->bitmask |= RIM_APP_ERR_CONTAINER_UTRA_SI_PRESENT;

                break;
            }
        default :
            {
                RRC_TRACE(RRC_WARNING, "App Identity not handled currently %u..",app_idty);
                return(RRC_FAILURE);
            }

    }
    return(RRC_SUCCESS);
}


/*****************************************************************************
 * Function Name  : rrc_il_parse_rim_routing_info 
 * Inputs         : p_routing_info - pointer to the rim_routing_info_t
 *                  p_src - pointer to the source
 *                  length_left - pointer to the length left unread 
 *                  p_length_read - pointer to length to read 
 *                  p_cause - pointer to cause 
 * Outputs        : p_routing_info 
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function parses rim_routing_inTo
 ********************************************************************************/

static
rrc_return_et
rrc_il_parse_rim_routing_info
(
 rim_routing_info_t *p_routing_info,
 U8  *p_src,
 S32 length_left,
 S32 *p_length_read
 /*SPR 17777 +-*/
 )
{
    U8 routing_choice = 0;
    U16 length = 0; 


    *p_length_read = 0;

    p_routing_info->bitmask= 0;

    /* This function parses rim_routing_info */
    RRC_TRACE(RRC_DETAILED, "Parsing r_nrouting_info");

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&routing_choice, p_src + *p_length_read, "Routing add disc");
    *p_length_read += sizeof(U8);

    switch (routing_choice)
    {
        case RIM_ROUT_ADD_DISC_GERAN_CELL:
            {

                l3_memcpy_wrapper(p_routing_info->geran_rtng_addr.routing_idty.plmn.plmn,
                        p_src + *p_length_read, RIM_PLMN_OCTETS);
                *p_length_read += RIM_PLMN_OCTETS;

                l3_memcpy_wrapper(p_routing_info->geran_rtng_addr.routing_idty.lac,
                        p_src + *p_length_read, RIM_LAC_OCTETS);
                *p_length_read += RIM_LAC_OCTETS;
                p_routing_info->geran_rtng_addr.routing_idty.rac = *(p_src + *p_length_read);
                *p_length_read += sizeof(U8);

                l3_memcpy_wrapper(p_routing_info->geran_rtng_addr.geran_cell_idty,
                        p_src + *p_length_read, RIM_GERAN_CELL_OCTETS);
                *p_length_read += RIM_GERAN_CELL_OCTETS;

                p_routing_info->bitmask |= RIM_GERAN_ROUTING_ADDR_PRESENT;

                break;
            }
        case RIM_ROUT_ADD_DISC_UTRAN_RNC:
            {
                l3_memcpy_wrapper(p_routing_info->utran_rtng_addr.routing_idty.plmn.plmn,
                        p_src + *p_length_read, RIM_PLMN_OCTETS);
                *p_length_read += RIM_PLMN_OCTETS;
                l3_memcpy_wrapper(p_routing_info->utran_rtng_addr.routing_idty.lac,
                        p_src + *p_length_read, RIM_LAC_OCTETS);
                *p_length_read += RIM_LAC_OCTETS;

                p_routing_info->utran_rtng_addr.routing_idty.rac = *(p_src + *p_length_read);
                *p_length_read += sizeof(U8);

                l3_memcpy_wrapper(p_routing_info->utran_rtng_addr.rnc_id,
                        p_src + *p_length_read, RIM_RNC_ID_OCTETS);
                *p_length_read += RIM_RNC_ID_OCTETS;

                p_routing_info->bitmask |= RIM_UTRAN_ROUTING_ADDR_PRESENT;

                break;
            }
        case RIM_ROUT_ADD_DISC_ENB_ID:
            {

                l3_memcpy_wrapper(p_routing_info->eutran_rtng_addr.ta_idty.plmn.plmn,
                        p_src + *p_length_read, RIM_PLMN_OCTETS);
                *p_length_read += RIM_PLMN_OCTETS;
                l3_memcpy_wrapper(p_routing_info->eutran_rtng_addr.ta_idty.tac,
                        p_src + *p_length_read, RIM_TAC_OCTETS);
                *p_length_read += RIM_TAC_OCTETS;
                length = (U16)length_left - sizeof(routing_choice) - RIM_TAC_OCTETS - RIM_PLMN_OCTETS;
                rim_asn_decode_global_enb_id(p_src + *p_length_read, 
                                             &p_routing_info->eutran_rtng_addr.enb,
                                             length);
                *p_length_read += length;
                p_routing_info->bitmask |= RIM_EUTRAN_ROUTING_ADDR_PRESENT;
            }
        default:
            {
                /* cause */
            }
    }

    return(RRC_SUCCESS);
}

/*SPR 19183 START*/
/*****************************************************************************
 * Function Name  : rrc_compose_rim_error_pdu_intl 
 * Inputs         : pp_buffer - pointer to the data
 *                  p_rim_info_app_error_container_intl - pointer to 
 *                                         rim_info_app_error_container_intl_t
 * Outputs        : pp_buffer
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function composes the rim_info_app_error_container msg. 
 ********************************************************************************/
static
rrc_return_et
rrc_compose_rim_error_pdu_intl
(
    U8  **pp_buffer,
    rim_error_pdu_intl_t *p_error_in_pdu
)
{
    U8 tag = RRC_NULL;
    U16 tlv_length = RRC_NULL;
    U8 *p_tag_length = RRC_NULL;
    rrc_counter_t loop;


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_error_in_pdu != PNULL);

    /* Compose TL of TLV */
    tag = RIM_ERROR_PDU_TAG;

    rrc_cp_pack_U8(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(U8);

    p_tag_length = *pp_buffer;
    *pp_buffer += sizeof (U16);

    /* Compose IE */
    for (loop = 0; loop < p_error_in_pdu->counter; loop++)
    {
#ifdef RRC_DEBUG
        RRC_TRACE(RRC_TRUE, RRC_PNULL, RRC_DETAILED, "Array index : %d", loop);
#endif
        rrc_cp_pack_U8(*pp_buffer, &p_error_in_pdu->error_pdu[loop], "error_pdu[]");
        *pp_buffer += sizeof(U8);
    }

    tlv_length = *pp_buffer - ( p_tag_length + sizeof(U16));
    rrc_cp_pack_U16(p_tag_length, &tlv_length, "<tlv_length>");
    return RRC_SUCCESS;
}
/*****************************************************************************
  * Function Name  : rrc_get_rim_cause_intl_len
  * Inputs         : p_rim_cause_intl - pointer to rim_cause_intl_t 
  * Outputs        : None 
  * Returns        : Length 
  * Description    : This function returns the RIM_CAUSE length
********************************************************************************/
static
rrc_length_t
rrc_get_rim_cause_intl_len
(
    rim_cause_intl_t  *p_rim_cause_intl
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rim_cause_intl != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_rim_cause_intl->cause);

    return length;
}
/*****************************************************************************
  * Function Name  : rrc_get_rim_cause_intl_tlv_len
  * Inputs         : p_rim_cause_intl  - poinbter to rim_cause_intl_t
  * Outputs        : None 
  * Returns        : rrc_length_t 
  * Description    : This function internally calls a function which returns 
  *                  RIM_CAUSE length
********************************************************************************/
static
rrc_length_t
rrc_get_rim_cause_intl_tlv_len
(
    rim_cause_intl_t  *p_rim_cause_intl

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rim_cause_intl != PNULL);

    length += rrc_get_rim_cause_intl_len(p_rim_cause_intl);

    return length;
}
/*****************************************************************************
  * Function Name  : rrc_compose_rim_cause_intl
  * Inputs         :  pp_buffer - pointer to data
  *                   p_rim_cause_intl - Pointer to structure rim_cause_intl_t
  * Outputs        : pp_buffer - pointer to data
  * Returns        : RRC_SUCCESS/RRC_FAILURE
  * Description    : This function composes TLV string from rim_cause_intl_t.
********************************************************************************/
static
rrc_return_et
rrc_compose_rim_cause_intl
(
    U8  **pp_buffer,
    rim_cause_intl_t *p_rim_cause_intl
)
{
    U8 tag = RRC_NULL;
    U16 tlv_length = RRC_NULL;


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_cause_intl != PNULL);

    /* Compose TL of TLV */
    tag = RIM_CAUSE_TAG;
    tlv_length = rrc_get_rim_cause_intl_tlv_len(p_rim_cause_intl);

    rrc_cp_pack_U8(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(U8);

    if (tlv_length <= RIM_MAX_LEN_IN_SINGLE_OCTET){
        tlv_length |= 0x80;
        rrc_cp_pack_U8(*pp_buffer, &tlv_length, "<tlv_length>");
        *pp_buffer += sizeof(U8);
    } else {
        rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
        *pp_buffer += sizeof(U16);
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rim_cause_intl->cause, "rim_cause");
    *pp_buffer += sizeof(p_rim_cause_intl->cause);

    return RRC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : rrc_compose_rim_information_error_intl 
 * Inputs         : p_rim_information_error_intl - pointer to 
 *                                                rim_information_error_intl_t
 * Outputs        : pp_buffer - pointer to data
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function composes TLV string from rim_information_error_intl_t.
 ********************************************************************************/
static
rrc_return_et
rrc_compose_rim_information_error_intl
(
    U8  **pp_buffer,
    rim_information_error_intl_t *p_rim_information_error_intl
)
{
    U8 tag = RRC_NULL;
    U16 tlv_length = RRC_NULL;
    U8 *p_tag_length = RRC_NULL;


    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rim_information_error_intl != PNULL);

    /* Compose TL of TLV */
    tag = RIM_INFORMATION_ERROR_TAG;

    rrc_cp_pack_U8(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(U8);

    p_tag_length = *pp_buffer;
    *pp_buffer += sizeof (U16);

    /* Compose TLV RIM APP IDENTITY */
    if (p_rim_information_error_intl->bitmask & RIM_INFO_ERROR_APP_ID_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_app_id_intl(pp_buffer, &p_rim_information_error_intl->rim_app_id))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV ERROR CAUSE */
    if (p_rim_information_error_intl->bitmask & RIM_INFO_ERROR_CAUSE_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_cause_intl(pp_buffer,&p_rim_information_error_intl->rim_cause))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV ERROR PROTOCOL VER */
    if (p_rim_information_error_intl->bitmask & RIM_INFO_ERROR_PROTOCOL_VER_PRESENT)
    {
        if(RRC_FAILURE == rrc_compose_rim_protocol_ver_intl(pp_buffer, &p_rim_information_error_intl->rim_protocol_ver))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV ERROR PDU */
    if (p_rim_information_error_intl->bitmask & RIM_INFO_ERROR_PDU_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_error_pdu_intl(pp_buffer, &p_rim_information_error_intl->error_in_pdu))
        {
            return RRC_FAILURE;
        }
    }    

    /* Compose TLV SON TRANSFER APP ID */
    if (p_rim_information_error_intl->bitmask & RIM_INFO_ERROR_SON_TRNFR_APP_IDNTY_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_son_transfer_app_idty_intl(pp_buffer, &p_rim_information_error_intl->son_app_idty))
        {
            return RRC_FAILURE;
        }
    }

    tlv_length = *pp_buffer - ( p_tag_length + sizeof(U16));
    rrc_cp_pack_U16(p_tag_length, &tlv_length, "<tlv_length>");
    return RRC_SUCCESS;
}
/*****************************************************************************
  * Function Name  : rrc_intl_compose_ran_information_error_rim
  * Inputs         : p_ran_information_error_rim - pointer to the 
  *                                                ran_information_error_rim_t
  *                  p_msg_length - pointer to the msg length 
  * Outputs        : pp_buffer - pointer to data
  * Returns        : RRC_SUCCESS/RRC_FAILURE 
  * Description    : This function composes the ran_information_error_rim_t to TLV string.
********************************************************************************/
rrc_return_et
rrc_intl_compose_ran_information_error_rim
(
    U8  **pp_buffer,
    ran_information_error_rim_t *p_ran_information_error_rim,
    U32 *p_msg_length
)
{
    U8 * p_start_buf = RRC_NULL;
    /* ARGUSED */

    p_start_buf = *pp_buffer;

    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_ran_information_error_rim != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_ran_information_error_rim->pdu_type, "pdu_type");
    *pp_buffer += sizeof(p_ran_information_error_rim->pdu_type);

    /* Compose TLV  */
    if (p_ran_information_error_rim->bitmask & RIM_INFO_DST_CELL_ID_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_cell_id_intl(pp_buffer, &p_ran_information_error_rim->dst_cell_id))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_ran_information_error_rim->bitmask & RIM_INFO_SRC_CELL_ID_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_cell_id_intl(pp_buffer, &p_ran_information_error_rim->src_cell_id))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_ran_information_error_rim->bitmask & RIM_INFO_ERROR_PRESENT)
    {
        if (RRC_FAILURE == rrc_compose_rim_information_error_intl(pp_buffer, &p_ran_information_error_rim->rim_error))
        {
            return RRC_FAILURE;
        }
    }

    *p_msg_length = (*pp_buffer - p_start_buf);
    *pp_buffer = p_start_buf;
    return RRC_SUCCESS;
}
/*SPR 19183 END*/
