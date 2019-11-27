/******************************************************************************
 **
 **   FILE NAME:
 **       s1ap_rrm_rim_intl.h
 **
 **   DESCRIPTION:
 **       This is the header file for the message dispatcher macros.
 **
 **   DATE            AUTHOR      REFERENCE       REASON
 **   01 Mar 2012     L3 Team     ---------       Initial
 **
 **   Copyright (c) 2012, Aricent Inc. All Rights Reserved
 **
 ** ******************************************************************************/

#ifndef _S1AP_RRM_RIM_INTL_H_
#define _S1AP_RRM_RIM_INTL_H_ 
#include "rrc_defines.h"
#include "rrc_s1apRrm_intf.h"

#define RIM_RAN_INFO_PDU  0x70
#define RIM_RAN_INFO_REQ_PDU  0x71
#define RIM_RAN_INFO_ACK_PDU  0x72
#define RIM_RAN_INFO_ERR_PDU  0x73
#define RIM_RAN_INFO_APP_ERR_PDU  0x74

#define RIM_PUP_TL_LENGTH   sizeof(U8)+sizeof(U16)

/* Internal code */
#define RIM_INFORMATION_REQUEST_TAG                             0x57
#define RIM_APP_IDENTITY_TAG                                    0x4b
#define RIM_SEQUENCE_NUM_TAG                                    0x4c
#define RIM_PDU_IND_TAG                                                 0x4f
#define RIM_ROUTING_INFO_TAG        0x54
#define RIM_PROTOCOL_VER_NUM_TAG                                0x55
#define RIM_APP_CONTAINER_TAG                                   0x4d
#define RIM_APP_INFO_CONTAINER_TAG                              0x4e
#define RIM_APP_INFO_ERROR_CONTAINER_TAG                0x56
#define RIM_SON_TRANSFER_APP_ID_TAG                             0x84

#define RIM_INFORMATION_TAG 0x58
#define RIM_INFORMATION_ACK_TAG 0x5a
#define RIM_CELL_IDENTIFIER_TAG 0x08
#define RIM_INFORMATION_ERROR_TAG 0x5b
#define RIM_INFORMATION_APP_ERROR_TAG 0x59
#define RIM_CAUSE_TAG 0x07
#define RIM_ERROR_PDU_TAG 0x15
#define RIM_ROUT_ADD_DISC_GERAN_CELL    0
#define RIM_ROUT_ADD_DISC_UTRAN_RNC    1
#define RIM_ROUT_ADD_DISC_ENB_ID        2
#define RIM_MAX_LEN_IN_SINGLE_OCTET     127
#pragma pack(push, 1)

#define RIM_MAND_ROUT_INFO_PRESENT   0x01
#define RIM_MAND_APP_ID_PRESENT   0x02
#define RIM_MAND_SEQ_NUM_PRESENT   0x04
#define RIM_MAND_PDU_IND_PRESENT   0x08
#define RIM_SI_CELL_IDENTIFIER_OCTET_LENGTH 7

/* UNPACK TLV LENGTH */
#define UNPACK_TLV_LENGTH \
    if (*(p_src + *p_length_read) > RIM_MAX_LEN_IN_SINGLE_OCTET)\
    {\
        rrc_cp_unpack_U8(&length, p_src + *p_length_read, "<length>");\
        length = length & 0x7F;\
        *p_length_read += sizeof(U8);\
    }\
    else\
    {\
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");\
        *p_length_read += sizeof(U16);\
    }


typedef struct
{
#define RIM_CELL_IDENTIFIER_OCTET_LENGTH 8
        U8 cell_identifier[RIM_CELL_IDENTIFIER_OCTET_LENGTH]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}rim_req_app_contrn_nacc_intl_t;

typedef struct
{
        U8 dummy;       /*^ M, 0, N, 0, 0 ^*/
}rim_req_app_contrn_si3_intl_t;

typedef struct
{
        U8 dummy;       /*^ M, 0, N, 0, 0 ^*/
}rim_req_app_contrn_mbms_intl_t;

typedef struct
{
        U8 dummy;       /*^ M, 0, N, 0, 0 ^*/
}rim_req_app_contrn_son_intl_t;

typedef struct
{
        U8 cell_identifier[RIM_SI_CELL_IDENTIFIER_OCTET_LENGTH]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}rim_req_app_contrn_utra_si_intl_t;

typedef struct
{
#define RIM_REQ_APP_CONTAINER_NACC_PRESENT      0x01
#define RIM_REQ_APP_CONTAINER_SI3_PRESENT       0x02
#define RIM_REQ_APP_CONTAINER_MBMS_PRESENT      0x04
#define RIM_REQ_APP_CONTAINER_SON_PRESENT       0x08
#define RIM_REQ_APP_CONTAINER_UTRA_SI_PRESENT   0x10
    rim_bitmask_t                   bitmask;    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    rim_req_app_contrn_nacc_intl_t       nacc;          /*^ O, RIM_REQ_APP_CONTAINER_NACC_PRESENT, N, 0, 0 ^*/
    rim_req_app_contrn_si3_intl_t        si3;           /*^ O, RIM_REQ_APP_CONTAINER_SI3_PRESENT, N, 0, 0 ^*/
    rim_req_app_contrn_mbms_intl_t       mbms;          /*^ O, RIM_REQ_APP_CONTAINER_MBMS_PRESENT, N, 0, 0 ^*/
    rim_req_app_contrn_son_intl_t        son;           /*^ O, RIM_REQ_APP_CONTAINER_SON_PRESENT, N, 0, 0 ^*/
    rim_req_app_contrn_utra_si_intl_t    utra_si;       /*^ O, RIM_REQ_APP_CONTAINER_UTRA_SI_PRESENT, N, 0, 0 ^*/
}rim_req_app_container_intl_t;

typedef struct
{
        U8 rim_son_app_idty; /*^ M, 0, H, 0, 1 ^*/
}rim_son_transfer_app_idty_intl_t;

typedef struct
{
        U8 rim_app_id;/*^ M, 0, N, 0, 0 ^*/
}rim_app_id_intl_t;

typedef struct
{
        U32 rim_seq_num;/*^ M, 0, N, 0, 0 ^*/
}rim_seq_num_intl_t;

typedef struct
{
        U8 rim_pdu_ind;/*^ M, 0, N, 0, 0 ^*/
}rim_pdu_ind_intl_t;

typedef struct
{
    U8       pdu_ext;
    U8       ack;
}rim_pdu_ind_t;

typedef struct
{
        U8      rim_protocol_ver;/*^ M, 0, N, 0, 0 ^*/
}rim_protocol_ver_intl_t;


/* RAN-INFORMATION-REQUEST RIM PDU */
typedef struct
{
#define RIM_INFO_REQ_APP_ID_PRESENT 0x08
#define RIM_INFO_REQ_SEQ_NUM_PRESENT 0x10
#define RIM_INFO_REQ_PDU_IND_PRESENT 0x20
    rim_bitmask_t bitmask;      /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    rim_app_id_intl_t   rim_app_id; /*^ TLV, RIM_APP_IDENTITY_TAG, RIM_INFO_REQ_APP_ID_PRESENT ^*/
    rim_seq_num_intl_t rim_seq_num;             /*^ TLV, RIM_SEQUENCE_NUM_TAG, RIM_INFO_REQ_SEQ_NUM_PRESENT ^*/
    rim_pdu_ind_intl_t rim_pdu_ind;             /*^ TLV, RIM_PDU_IND_TAG, RIM_INFO_REQ_PDU_IND_PRESENT ^*/
    rim_protocol_ver_intl_t rim_protocol_ver; /*^ TLV, RIM_PROTOCOL_VER_NUM_TAG, RIM_INFO_REQ_PROTOCOL_VER_PRESENT ^*/
    rim_req_app_container_intl_t app_cont; /*^ TLV, RIM_APP_CONTAINER_TAG, RIM_INFO_REQ_APP_CONTAINER_PRESENT ^*/
    rim_son_transfer_app_idty_intl_t son_app_idty; /*^ TLV, RIM_SON_TRANSFER_APP_ID_TAG, RIM_INFO_REQ_SON_TRNFR_APP_IDNTY_PRESENT ^*/
}rim_information_req_intl_t;

typedef struct
{
#define RIM_CELL_IDENTIFIER_MAX_OCTET_LENGTH 15
        U8 routing_add_disc;
        U8 length;
        U8 cell_identifier[RIM_CELL_IDENTIFIER_MAX_OCTET_LENGTH]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}rim_cell_id_intl_t;

typedef struct
{
#define RIM_INFO_DST_CELL_ID_PRESENT 0x01
#define RIM_INFO_SRC_CELL_ID_PRESENT 0x02
#define RIM_INFO_REQ_PRESENT 0x04
        rim_bitmask_t bitmask;  /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
        U8 pdu_type; /*^ M, 0, N, 0, 0 ^*/
        rim_cell_id_intl_t dst_cell_id; /*^ TLV, RIM_DST_CELL_IDENTIFIER_TAG, RIM_INFO_DST_CELL_ID_PRESENT ^*/
        rim_cell_id_intl_t src_cell_id; /*^ TLV, RIM_SRC_CELL_IDENTIFIER_TAG, RIM_INFO_SRC_CELL_ID_PRESENT ^*/
        rim_information_req_intl_t rim_req; /*^ TLV, RIM_INFORMATION_REQUEST_TAG, RIM_INFO_REQ_PRESENT ^*/
}ran_information_req_rim_t;  

typedef struct
{
#define RIM_SI_PSI_MAX_OCTET_LENGTH 127
        U8 cell_identifier[RIM_CELL_IDENTIFIER_OCTET_LENGTH]; /*^ M, 0, OCTET_STRING, FIXED ^*/
        U8 num_si_psi; /*^ M, 0, N, 0, 0 ^*/
        U8 counter; /*^ M, 0, BUFFER_SIZE, NOT_PRESENT_IN_MESSAGE ^*/
        U8 si_psi[RIM_SI_PSI_MAX_OCTET_LENGTH]; /*^ M, 0, OCTET_STRING, LIMITED_TILL_THE_END ^*/
}rim_info_app_contrn_nacc_intl_t;

typedef struct
{
        U8 dummy;       /*^ M, 0, N, 0, 0 ^*/
}rim_info_app_contrn_si3_intl_t;

typedef struct
{
        U8 dummy;       /*^ M, 0, N, 0, 0 ^*/
}rim_info_app_contrn_mbms_intl_t;

typedef struct
{
        U8 dummy;       /*^ M, 0, N, 0, 0 ^*/
}rim_info_app_contrn_son_intl_t;

typedef struct
{
#define RIM_UTRA_SI_OCTET_LENGTH 2048
        U8 cell_identifier[RIM_SI_CELL_IDENTIFIER_OCTET_LENGTH]; /*^ M, 0, OCTET_STRING, FIXED ^*/
        U8 utra_si[RIM_UTRA_SI_OCTET_LENGTH]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}rim_info_app_contrn_utra_si_intl_t;

typedef struct
{
#define RIM_INFO_APP_CONTAINER_NACC_PRESENT      0x01
#define RIM_INFO_APP_CONTAINER_SI3_PRESENT       0x02
#define RIM_INFO_APP_CONTAINER_MBMS_PRESENT      0x04
#define RIM_INFO_APP_CONTAINER_SON_PRESENT       0x08
#define RIM_INFO_APP_CONTAINER_UTRA_SI_PRESENT   0x10
    rim_bitmask_t                   bitmask;    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    rim_info_app_contrn_nacc_intl_t       nacc;                 /*^ O, RIM_INFO_APP_CONTAINER_NACC_PRESENT, N, 0, 0 ^*/
    rim_info_app_contrn_si3_intl_t        si3;          /*^ O, RIM_INFO_APP_CONTAINER_SI3_PRESENT, N, 0, 0 ^*/
    rim_info_app_contrn_mbms_intl_t       mbms;                 /*^ O, RIM_INFO_APP_CONTAINER_MBMS_PRESENT, N, 0, 0 ^*/
    rim_info_app_contrn_son_intl_t        son;          /*^ O, RIM_INFO_APP_CONTAINER_SON_PRESENT, N, 0, 0 ^*/
    rim_info_app_contrn_utra_si_intl_t    utra_si;      /*^ O, RIM_INFO_APP_CONTAINER_UTRA_SI_PRESENT, N, 0, 0 ^*/
}rim_info_app_container_intl_t;

/* RAN-INFORMATION RIM PDU */
typedef struct
{
#define RIM_INFO_APP_ID_PRESENT 0x01
#define RIM_INFO_SEQ_NUM_PRESENT 0x02
#define RIM_INFO_PDU_IND_PRESENT 0x04
#define RIM_INFO_PROTOCOL_VER_PRESENT_INTL 0x08
#define RIM_INFO_APP_CONTAINER_PRESENT_INTL 0x10
#define RIM_INFO_SON_TRNFR_APP_IDNTY_PRESENT 0x20
    rim_bitmask_t bitmask;      /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    rim_app_id_intl_t   rim_app_id; /*^ TLV, RIM_APP_IDENTITY_TAG, RIM_INFO_APP_ID_PRESENT ^*/
    rim_seq_num_intl_t rim_seq_num;             /*^ TLV, RIM_SEQUENCE_NUM_TAG, RIM_INFO_SEQ_NUM_PRESENT ^*/
    rim_pdu_ind_intl_t rim_pdu_ind;             /*^ TLV, RIM_PDU_IND_TAG, RIM_INFO_PDU_IND_PRESENT ^*/
    rim_protocol_ver_intl_t rim_protocol_ver; /*^ TLV, RIM_PROTOCOL_VER_NUM_TAG, RIM_INFO_PROTOCOL_VER_PRESENT_INTL ^*/
    rim_info_app_container_intl_t app_cont; /*^ TLV, RIM_APP_CONTAINER_TAG, RIM_INFO_APP_CONTAINER_PRESENT_INTL ^*/
    rim_son_transfer_app_idty_intl_t son_app_idty; /*^ TLV, RIM_SON_TRANSFER_APP_ID_TAG, RIM_INFO_SON_TRNFR_APP_IDNTY_PRESENT ^*/
}rim_information_intl_t;

typedef struct
{
#define RIM_INFO_DST_CELL_ID_PRESENT 0x01
#define RIM_INFO_SRC_CELL_ID_PRESENT 0x02
#define RIM_INFO_PRESENT 0x04
        rim_bitmask_t bitmask;  /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
        U8 pdu_type; /*^ M, 0, N, 0, 0 ^*/
        rim_cell_id_intl_t dst_cell_id; /*^ TLV, RIM_DST_CELL_IDENTIFIER_TAG, RIM_INFO_DST_CELL_ID_PRESENT ^*/
        rim_cell_id_intl_t src_cell_id; /*^ TLV, RIM_SRC_CELL_IDENTIFIER_TAG, RIM_INFO_SRC_CELL_ID_PRESENT ^*/
        rim_information_intl_t rim_info; /*^ TLV, RIM_INFORMATION_TAG, RIM_INFO_PRESENT ^*/
}ran_information_rim_t;  

typedef struct
{
#define RIM_INFO_ACK_APP_ID_PRESENT 0x01
#define RIM_INFO_ACK_SEQ_NUM_PRESENT 0x02
#define RIM_INFO_ACK_PROTOCOL_VER_PRESENT_INTL 0x04
#define RIM_INFO_ACK_SON_TRNFR_APP_IDNTY_PRESENT 0x08
    rim_bitmask_t bitmask;      /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    rim_app_id_intl_t   rim_app_id; /*^ TLV, RIM_APP_IDENTITY_TAG, RIM_INFO_ACK_APP_ID_PRESENT ^*/
    rim_seq_num_intl_t rim_seq_num;             /*^ TLV, RIM_SEQUENCE_NUM_TAG, RIM_INFO_ACK_SEQ_NUM_PRESENT ^*/
    rim_protocol_ver_intl_t rim_protocol_ver; /*^ TLV, RIM_PROTOCOL_VER_NUM_TAG, RIM_INFO_ACK_PROTOCOL_VER_PRESENT_INTL ^*/
    rim_son_transfer_app_idty_intl_t son_app_idty; /*^ TLV, RIM_SON_TRANSFER_APP_ID_TAG, RIM_INFO_ACK_SON_TRNFR_APP_IDNTY_PRESENT ^*/
}rim_information_ack_intl_t;

typedef struct
{
#define RIM_INFO_DST_CELL_ID_PRESENT 0x01
#define RIM_INFO_SRC_CELL_ID_PRESENT 0x02
#define RIM_INFO_ACK_PRESENT 0x04
        rim_bitmask_t bitmask;  /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
        U8 pdu_type; /*^ M, 0, N, 0, 0 ^*/
        rim_cell_id_intl_t dst_cell_id; /*^ TLV, RIM_DST_CELL_IDENTIFIER_TAG, RIM_INFO_DST_CELL_ID_PRESENT ^*/
        rim_cell_id_intl_t src_cell_id; /*^ TLV, RIM_SRC_CELL_IDENTIFIER_TAG, RIM_INFO_SRC_CELL_ID_PRESENT ^*/
        rim_information_ack_intl_t rim_info; /*^ TLV, RIM_INFORMATION_ACK_TAG, RIM_INFO_ACK_PRESENT ^*/
}ran_information_ack_rim_t;  

typedef struct
{
        U8 cause; /*^ M, 0, N, 0, 0 ^*/
}rim_cause_intl_t;

typedef struct
{
#define RIM_ERROR_PDU_MAX_OCTET_LENGTH 2048
/*SPR 19183 START*/
        U16 counter; /*^ M, 0, BUFFER_SIZE, NOT_PRESENT_IN_MESSAGE ^*/
/*SPR 19183 END*/
        U8 error_pdu[RIM_ERROR_PDU_MAX_OCTET_LENGTH]; /*^ M, 0, N, 0, 0 ^*/
}rim_error_pdu_intl_t;

typedef struct
{
#define RIM_INFO_ERROR_APP_ID_PRESENT 0x01
#define RIM_INFO_ERROR_CAUSE_PRESENT 0x02
#define RIM_INFO_ERROR_PROTOCOL_VER_PRESENT 0x04
#define RIM_INFO_ERROR_PDU_PRESENT 0x08
#define RIM_INFO_ERROR_SON_TRNFR_APP_IDNTY_PRESENT 0x10
    rim_bitmask_t bitmask;      /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    rim_app_id_intl_t   rim_app_id; /*^ TLV, RIM_APP_IDENTITY_TAG, RIM_INFO_ERROR_APP_ID_PRESENT ^*/
    rim_cause_intl_t rim_cause;                 /*^ TLV, RIM_CAUSE_TAG, RIM_INFO_ERROR_CAUSE_PRESENT ^*/
    rim_protocol_ver_intl_t rim_protocol_ver; /*^ TLV, RIM_PROTOCOL_VER_NUM_TAG, RIM_INFO_ERROR_PROTOCOL_VER_PRESENT ^*/
        rim_error_pdu_intl_t error_in_pdu; /*^ TLV, RIM_ERROR_PDU_TAG, RIM_INFO_ERROR_PDU_PRESENT ^*/
    rim_son_transfer_app_idty_intl_t son_app_idty; /*^ TLV, RIM_SON_TRANSFER_APP_ID_TAG, RIM_INFO_ERROR_SON_TRNFR_APP_IDNTY_PRESENT ^*/
}rim_information_error_intl_t;

typedef struct
{
#define RIM_INFO_DST_CELL_ID_PRESENT 0x01
#define RIM_INFO_SRC_CELL_ID_PRESENT 0x02
#define RIM_INFO_ERROR_PRESENT 0x04
        rim_bitmask_t bitmask;  /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
        U8 pdu_type; /*^ M, 0, N, 0, 0 ^*/
        rim_cell_id_intl_t dst_cell_id; /*^ TLV, RIM_DST_CELL_IDENTIFIER_TAG, RIM_INFO_DST_CELL_ID_PRESENT ^*/
        rim_cell_id_intl_t src_cell_id; /*^ TLV, RIM_SRC_CELL_IDENTIFIER_TAG, RIM_INFO_SRC_CELL_ID_PRESENT ^*/
        rim_information_error_intl_t rim_error; /*^ TLV, RIM_INFORMATION_ERROR_TAG, RIM_INFO_ERROR_PRESENT ^*/
}ran_information_error_rim_t;  

typedef struct
{
#define RIM_INFO_APP_ERROR_CONTAINER_MAX_OCTET_SIZE 4096
        U8 nacc_cause; /*^ M, 0, N, 0, 0 ^*/
        U8 counter; /*^ M, 0, BUFFER_SIZE, NOT_PRESENT_IN_MESSAGE ^*/
        U8 nacc_info[RIM_INFO_APP_ERROR_CONTAINER_MAX_OCTET_SIZE]; /*^ M, 0, OCTET_STRING, LIMITED_TILL_THE_END ^*/
}rim_info_app_error_contrn_nacc_intl_t;

typedef struct
{
        U8 dummy;       /*^ M, 0, N, 0, 0 ^*/
}rim_info_app_error_contrn_si3_intl_t;

typedef struct
{
        U8 dummy;       /*^ M, 0, N, 0, 0 ^*/
}rim_info_app_error_contrn_mbms_intl_t;

typedef struct
{
        U8 dummy;       /*^ M, 0, N, 0, 0 ^*/
}rim_info_app_error_contrn_son_intl_t;

typedef struct
{
        U8 utra_si_cause; /*^ M, 0, N, 0, 0 ^*/
        U8 counter; /*^ M, 0, BUFFER_SIZE, NOT_PRESENT_IN_MESSAGE ^*/
        U8 utra_si_info[RIM_INFO_APP_ERROR_CONTAINER_MAX_OCTET_SIZE]; /*^ M, 0, OCTET_STRING, LIMITED_TILL_THE_END ^*/
}rim_info_app_error_contrn_utra_si_intl_t;

typedef struct
{
#define RIM_INFO_APP_ERROR_CONTAINER_NACC_PRESENT      0x01
#define RIM_INFO_APP_ERROR_CONTAINER_SI3_PRESENT       0x02
#define RIM_INFO_APP_ERROR_CONTAINER_MBMS_PRESENT      0x04
#define RIM_INFO_APP_ERROR_CONTAINER_SON_PRESENT       0x08
#define RIM_INFO_APP_ERROR_CONTAINER_UTRA_SI_PRESENT   0x10
    rim_bitmask_t                   bitmask;    /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    rim_info_app_error_contrn_nacc_intl_t       nacc;           /*^ O, RIM_INFO_APP_ERROR_CONTAINER_NACC_PRESENT, N, 0, 0 ^*/
    rim_info_app_error_contrn_si3_intl_t        si3;            /*^ O, RIM_INFO_APP_ERROR_CONTAINER_SI3_PRESENT, N, 0, 0 ^*/
    rim_info_app_error_contrn_mbms_intl_t       mbms;           /*^ O, RIM_INFO_APP_ERROR_CONTAINER_MBMS_PRESENT, N, 0, 0 ^*/
    rim_info_app_error_contrn_son_intl_t        son;            /*^ O, RIM_INFO_APP_ERROR_CONTAINER_SON_PRESENT, N, 0, 0 ^*/
    rim_info_app_error_contrn_utra_si_intl_t    utra_si;        /*^ O, RIM_INFO_APP_ERROR_CONTAINER_UTRA_SI_PRESENT, N, 0, 0 ^*/
}rim_info_app_error_container_intl_t;

typedef struct
{
#define RIM_INFO_APP_ERROR_APP_ID_PRESENT 0x01
#define RIM_INFO_APP_ERROR_SEQ_NUM_PRESENT 0x02
#define RIM_INFO_APP_ERROR_PDU_IND_PRESENT 0x04
#define RIM_INFO_APP_ERROR_PROTOCOL_VER_PRESENT 0x08
#define RIM_INFO_APP_ERROR_APP_CONTAINER_PRESENT 0x10
#define RIM_INFO_APP_ERROR_SON_TRNFR_APP_IDNTY_PRESENT 0x20
    rim_bitmask_t bitmask;      /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
    rim_app_id_intl_t   rim_app_id; /*^ TLV, RIM_APP_IDENTITY_TAG, RIM_INFO_APP_ERROR_APP_ID_PRESENT ^*/
    rim_seq_num_intl_t rim_seq_num;             /*^ TLV, RIM_SEQUENCE_NUM_TAG, RIM_INFO_APP_ERROR_SEQ_NUM_PRESENT ^*/
    rim_pdu_ind_intl_t rim_pdu_ind;             /*^ TLV, RIM_PDU_IND_TAG, RIM_INFO_APP_ERROR_PDU_IND_PRESENT ^*/
    rim_protocol_ver_intl_t rim_protocol_ver; /*^ TLV, RIM_PROTOCOL_VER_NUM_TAG, RIM_INFO_APP_ERROR_PROTOCOL_VER_PRESENT ^*/
    rim_info_app_error_container_intl_t app_cont; /*^ TLV, RIM_APP_INFO_ERROR_CONTAINER_TAG, RIM_INFO_APP_ERROR_APP_CONTAINER_PRESENT ^*/
    rim_son_transfer_app_idty_intl_t son_app_idty; /*^ TLV, RIM_SON_TRANSFER_APP_ID_TAG, RIM_INFO_APP_ERROR_SON_TRNFR_APP_IDNTY_PRESENT ^*/
}rim_information_app_error_intl_t;

typedef struct
{
#define RIM_INFO_DST_CELL_ID_PRESENT 0x01
#define RIM_INFO_SRC_CELL_ID_PRESENT 0x02
#define RIM_INFO_APP_ERROR_PRESENT 0x04
        rim_bitmask_t bitmask;  /*^ M, 0, BITMASK, NOT_PRESENT_IN_MESSAGE ^*/
        U8 pdu_type; /*^ M, 0, N, 0, 0 ^*/
        rim_cell_id_intl_t dst_cell_id; /*^ TLV, RIM_DST_CELL_IDENTIFIER_TAG, RIM_INFO_DST_CELL_ID_PRESENT ^*/
        rim_cell_id_intl_t src_cell_id; /*^ TLV, RIM_SRC_CELL_IDENTIFIER_TAG, RIM_INFO_DST_CELL_ID_PRESENT ^*/
        rim_information_app_error_intl_t rim_app_error; /*^ TLV, RIM_INFORMATION_APP_ERROR_TAG, RIM_INFO_APP_ERROR_PRESENT ^*/
}ran_information_app_error_rim_t;  

rrc_return_et rrc_intl_compose_ran_information_req_rim
(
    U8  **pp_buffer,
    ran_information_req_rim_t *p_ran_information_req_rim,
    U32 *p_msg_length
);

rrc_return_et rrc_il_parse_ran_information_rim
(
    rim_information_t  *p_ran_information_rim,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read,
    U8  *p_cause
);

rrc_return_et rrc_il_parse_ran_information_error_rim
(
    rim_information_err_t *p_ran_information_error_rim,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

rrc_return_et rrc_compose_ran_information_ack_rim
(
    U8  **pp_buffer,
    ran_information_ack_rim_t *p_ran_information_ack_rim,
    U32 * p_msg_length
);

rrc_return_et rrc_compose_ran_information_app_error_rim
(
    U8  **pp_buffer,
    ran_information_app_error_rim_t *p_ran_information_app_error_rim,
    U32 * p_msg_length
);

/*SPR 19183 START*/
rrc_return_et
rrc_intl_compose_ran_information_error_rim
(
    U8  **pp_buffer,
    ran_information_error_rim_t *p_ran_information_error_rim,
    U32 *p_msg_length
);
/*SPR 19183 END*/
/*SPR 20191 Fix START*/
#pragma pack(pop)
/*SPR 20191 Fix END*/
#endif /*_S1AP_RRM_RIM_INTL_H_*/                                  

