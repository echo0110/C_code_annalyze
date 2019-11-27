/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2011 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rrc_s1apOam_intf.h,v 1.3 2011/01/03 09:50:40 gur18569 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the declarations of data types for
 *                     S1AP-OAM interface file.
 *
 ****************************************************************************
 *
 *
 ****************************************************************************/

#ifndef  RRC_S1APRRM_INTF_INC
#define  RRC_S1APRRM_INTF_INC
/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "rrc_defines.h"
#include "rrc_rrm_intf.h"
#include "rrc_s1apOam_intf.h"
#include "s1ap_api.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define RRC_S1APRRM_MODULE_ID    RRC_RRM_MODULE_ID


/****************************************************************************
 * Exported Types
 ****************************************************************************/

#pragma pack(push, 1)


#define MAX_BSSGP_PDU_LEN 1024
#define MAX_WARNING_TYPE                    2
#define MAX_SECURITY_INFORMATION            50
#define MAX_DATA_CODING_SCHEME              1
#define MAX_EUTRAN_CGI_SIZE                 65535
#define MAX_TAI_SIZE                        65535
#define MAX_EMERGENCY_SIZE                  65535
#define ENB_ID_NUMBITS    20
#define ENB_ID_NUMBIT     28

typedef struct
{
    plmn_identity_t              plmn_identity; /*^ M, 0, N, 0, 0 ^*/
    U8                                 location_area_code[LAC_OCTET_SIZE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrm_s1ap_lai_info_t;


typedef struct
{
    rrm_s1ap_lai_info_t               lai;  /*^ M, 0, N, 0, 0 ^*/
    U8                           routing_area_code; /*^ M, 0, N, 0, 0 ^*/
    U8                           ci[2]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrm_geran_cell_id_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/ 
#define GERAN_CELL_ID_PRESENT  0x01
#define TARGET_RNC_ID_PRESENT  0x02    

  U8       mme_id;       /*^ M, 0, H, 0, 15^*/

  U32      len_rim_info ; /*^ M,0,H,0,1024 ^*/
  U8       rim_info[MAX_BSSGP_PDU_LEN]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/

  rrm_geran_cell_id_t       geran_cell_id;
  /*^ O, GERAN_CELL_ID_PRESENT ^*/

  rrm_rnc_id_t       target_rnc_id;       
  /*^ O, TARGET_RNC_ID_PRESENT ^*/
} s1ap_rrm_enb_direct_info_transfer_t;   /*^ API, S1AP_RRM_ENB_DIRECT_INFO_TRANSFER ^*/

typedef struct
{
    U8       mme_id;       /*^ M, 0, H, 0, 15 ^*/
/* SPR-18445 start */
    U32      len_rim_info ; /*^ M,0,H,0,MAX_BSSGP_PDU_LEN ^*/
/* SPR-18445 stop */
    U8       rim_info[MAX_BSSGP_PDU_LEN]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/

} s1ap_rrm_mme_direct_info_transfer_t;   /*^ API, S1AP_RRM_MME_DIRECT_INFO_TRANSFER ^*/
#define RIM_RAN_INFO_PDU 0x70
#define RIM_RAN_INFO_REQ_PDU 0x71
#define RIM_RAN_INFO_ACK_PDU 0x72
#define RIM_RAN_INFO_ERR_PDU 0x73
#define RIM_RAN_INFO_APP_ERR_PDU 0x74


typedef struct
{
    U8 eNB_id[MACRO_ENB_ID_OCTET_SIZE]; /*^ M,0,OCTET_STRING,FIXED ^*/
} rrm_macro_enb_id_t;

typedef struct
{
    U8 eNB_id[HOME_ENB_ID_OCTET_SIZE]; /*^ M,0,OCTET_STRING,FIXED ^*/
} rrm_home_enb_id_t;

#define S1AP_OAM_MACRO_ENB_ID_PRESENT        0x01
#define S1AP_OAM_HOME_ENB_ID_PRESENT         0x02

typedef struct
{
    U8 presence_bitmask;    /*^ BITMASK ^*/

    rrm_macro_enb_id_t  macro_enb_id; /*^ O,1,N,0,0 ^*/

    rrm_home_enb_id_t   home_enb_id; /*^ O,2,N,0,0 ^*/
}rrm_enb_id_info_t;

typedef struct
{
    /*PLMN id */
    plmn_identity_t              plmn_identity; /*^ M, 0, N, 0, 0 ^*/

    /* eNB id info */
    rrm_enb_id_info_t       enb_id; /*^ M,0,N,0,0 ^*/

}rrm_gb_enb_id_t;

typedef struct
{
    plmn_identity_t              plmn_identity; /*^ M, 0, N, 0, 0 ^*/

    U8      tac[TAC_OCTET_SIZE]; /*^ M,0,OCTET_STRING,FIXED ^*/
}rrm_supp_ta_t;

typedef struct
{
    rrc_bitmask_t    bitmask;        /*^ BITMASK ^*/

#define ENB_CONFIGURATION_TRANSFER_REQ 0x01
#define ENB_CONFIGURATION_TRANSFER_RESP 0x02

    U8 mme_id;  /*^ M, 0, H, 0, 15 ^*/

    rrm_gb_enb_id_t gb_enb_id; /*^ M, 0, N, 0, 0 ^*/

    /* Target TA */
    rrm_supp_ta_t target_supp_ta; /*^ M, 0, N, 0, 0 ^*/

    /* Self TA */
    rrm_supp_ta_t source_supp_ta; /*^ M, 0, N, 0, 0 ^*/

} s1ap_rrm_enb_config_transfer_t; /*^ API, S1AP_RRM_ENB_CONFIG_TRANSFER ^*/

#define MAX_NUM_IP_SUPPORTED 2

typedef struct
{
        U8 ip_addr[RRC_MAX_IP_ADDR_LEN_IPV6]; /*^ M,0,OCTET_STRING,FIXED ^*/
} ip_address_t;

typedef struct
{
	/* SPR_17664_start*/
    U8     data_length; /*^ M, 0, H, 0, RRC_MAX_IP_ADDR_LEN_IPV6  ^*/
	/* SPR_17664_end*/
    U8     data[RRC_MAX_IP_ADDR_LEN_IPV6];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}trans_layer_addrs_t;


#define EXTEND_TRANS_LAYER_ADDRS_IPSEC_ADDRS_PRESENT    0x01
#define EXTEND_TRANS_LAYER_ADDRS_NUM_GTP_ADDRS_PRESENT  0x02
#define EXTEND_TRANS_LAYER_ADDRS_GTP_ADDRS_PRESENT      0x04
typedef struct
{
    U16                          bitmask;    /*^ BITMASK ^*/
    trans_layer_addrs_t ipsec_trans_layer_addrs;
    /*^ O,EXTEND_TRANS_LAYER_ADDRS_IPSEC_ADDRS_PRESENT,N,0,0 ^*/
    U8                           num_tlas; 

	/* SPR_17664_start*/
    /*^ O,EXTEND_TRANS_LAYER_ADDRS_NUM_GTP_ADDRS_PRESENT,B,1,MAX_NUM_ENB_X2AP_GTP_TLAS ^*/
	/* SPR_17664_end*/
    trans_layer_addrs_t   gtp_trans_layer_addrs[MAX_NUM_ENB_X2AP_GTP_TLAS];  /*^ O,EXTEND_TRANS_LAYER_ADDRS_GTP_ADDRS_PRESENT,OCTET_STRING,VARIABLE ^*/
}extend_trans_layer_addrs_t;

typedef struct 
{
    rrc_bitmask_t    bitmask;        /*^ BITMASK ^*/

#define MME_CONFIG_TRANSFER_SON_INFO_REQ              0x01
#define MME_CONFIG_TRANSFER_SON_INFO_RESP             0x02
#define MME_CONFIG_TRANSFER_X2TNL_IPv6                0x04
#define MME_CONFIG_TRANSFER_EXT_NUM_TLAS_PRESENT      0x08
#define MME_CONFIG_TRANSFER_EXT_TLAS_PRESENT          0x10

    U8 mme_id;  /*^ M, 0, H, 0, 15 ^*/

    rrm_gb_enb_id_t gb_enb_id; /*^ M, 0, N, 0, 0 ^*/

    /* MME's Source ENB TA */
    rrm_supp_ta_t source_supp_ta; /*^ M, 0, N, 0, 0 ^*/

    /* MME's Target ENB TA i.e. Self */
    rrm_supp_ta_t target_supp_ta; /*^ M, 0, N, 0, 0 ^*/

	/* SPR_17664_start*/
    U8 number_of_addr; /*^ M, 0, N, 0, MAX_NUM_IP_SUPPORTED ^*/
	/* SPR_17664_end*/
    ip_address_t ip_addr[MAX_NUM_IP_SUPPORTED]; /*^ M,0,OCTET_STRING,FIXED ^*/

	/* SPR_17664_start*/
    U8 number_of_ipv6_addr; /*^ O, MME_CONFIG_TRANSFER_X2TNL_IPv6, H, 1, MAX_NUM_IP_SUPPORTED^*/
	/* SPR_17664_end*/
    ip_address_t ip_v6_addr[MAX_NUM_IP_SUPPORTED]; /*^ O, MME_CONFIG_TRANSFER_X2TNL_IPv6,OCTET_STRING,FIXED ^*/

    U8           num_tlas; 
	/* SPR_17664_start*/
    /*^ O,MME_CONFIG_TRANSFER_EXT_NUM_TLAS_PRESENT,B,1,MAX_NUM_ENB_X2AP_EXT_TLAS ^*/
	/* SPR_17664_end*/
    extend_trans_layer_addrs_t extend_trans_layer_addrs[MAX_NUM_ENB_X2AP_EXT_TLAS];
    /*^ O,MME_CONFIG_TRANSFER_EXT_TLAS_PRESENT,OCTET_STRING,VARIABLE ^*/

} s1ap_rrm_mme_config_transfer_t; /*^ API, S1AP_RRM_MME_CONFIG_TRANSFER ^*/

typedef struct
{
	/* SPR_17664_start*/
    U16 count; /*^ M, 0, N, 0, MAX_EUTRAN_CGI_SIZE ^*/
	/* SPR_17664_end*/
    s1ap_cgi_eutra_t eutran_cgi[MAX_EUTRAN_CGI_SIZE];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

} cell_id_list_t;

typedef struct
{
    s1ap_plmn_identity_t plmn_identity; /*^ M, 0, N, 0, 0 ^*/

    U8 tracking_area_code[TAC_OCTET_SIZE];
/*^ M, 0, OCTET_STRING, FIXED ^*/

} tracking_area_identifier_t;

typedef struct
{
    U16 count; /*^ M, 0, N, 0, 0 ^*/
    
    tracking_area_identifier_t tracking_area_identifier[MAX_TAI_SIZE];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

} tracking_area_id_list_t;

typedef struct _emergency_area_identifier_t
{
    U8 emergency_area_id[EMERGENCY_AREA_SIZE];
/*^ M, 0, OCTET_STRING, FIXED ^*/
}emergency_area_identifier_t;
typedef struct
{
    U16 count; /*^ M, 0, N, 0, 0 ^*/

    emergency_area_identifier_t   emer_id[MAX_EMERGENCY_SIZE];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

} emergency_area_id_list_t;

typedef struct
{
    rrc_bitmask_t    bitmask;        /*^ BITMASK ^*/

#define WARNING_AREA_LIST_INFO_CELL_ID_LIST 0x01
#define WARNING_AREA_LIST_INFO_TRACKING_AREA_ID_LIST 0x02
#define WARNING_AREA_LIST_INFO_EMERGENCY_AREA_ID_LIST 0x04

    cell_id_list_t cell_id_list; 
/*^ O, WARNING_AREA_LIST_INFO_CELL_ID_LIST ^*/

    tracking_area_id_list_t tracking_area_id_list;
/*^ O, WARNING_AREA_LIST_INFO_TRACKING_AREA_ID_LIST ^*/

    emergency_area_id_list_t emergency_area_id_list;
/*^ O, WARNING_AREA_LIST_INFO_EMERGENCY_AREA_ID_LIST ^*/

} warning_area_list_info_t;

typedef struct 
{
    rrc_bitmask_t    bitmask;        /*^ BITMASK ^*/

#define S1AP_RRM_PWS_WARNING_AREA_LIST 0x01
#define S1AP_RRM_PWS_EXTENDED_REPETITION_PERIOD 0x02
#define S1AP_RRM_PWS_WARNING_TYPE 0x04
#define S1AP_RRM_PWS_WARNING_SECURITY_INFO 0x08
#define S1AP_RRM_PWS_DATA_CODING_SCHEME 0x10
#define S1AP_RRM_PWS_WARNING_MESSAGE_SIZE 0x20
#define S1AP_RRM_PWS_CONCURRENT_WARNING_MSG_INDICATOR 0x40

    U8 message_identifier[MSG_ID_OCTET_SIZE];  
/*^ M, 0, OCTET_STRING, FIXED ^*/

    U8 serial_number[SERIAL_NUMBER_OCTET_SIZE]; 
/*^ M, 0, OCTET_STRING, FIXED ^*/

    warning_area_list_info_t warning_area_list;
/*^ O, S1AP_RRM_PWS_WARNING_AREA_LIST ^*/

    U16 repetition_period; /*^ M, 0, H, 0, 4095 ^*/

    U32 extended_repetition_period; 
/*^ O, S1AP_RRM_PWS_EXTENDED_REPETITION_PERIOD, H, 4096, 131071 ^*/

    U16 number_of_broadcast_requested; /*^ M, 0, N, 0, 0 ^*/
    
    U8 warning_type[MAX_WARNING_TYPE]; 
/*^ O, S1AP_RRM_PWS_WARNING_TYPE, OCTET_STRING, FIXED ^*/

    U8 warning_security_info[MAX_SECURITY_INFORMATION];
/*^ O, S1AP_RRM_PWS_WARNING_SECURITY_INFO, OCTET_STRING, FIXED ^*/

    U8 data_coding_scheme[MAX_DATA_CODING_SCHEME];
/*^ O, S1AP_RRM_PWS_DATA_CODING_SCHEME, OCTET_STRING, FIXED ^*/

    U16 warning_message_size;
/*^ O, S1AP_RRM_PWS_WARNING_MESSAGE_SIZE, H, 0, 9600 ^*/

    U8  concurrent_warning_msg_indicator;
/*^ O, S1AP_RRM_PWS_CONCURRENT_WARNING_MSG_INDICATOR, H, 0, 0 ^*/ /* concurrent_warning_msg_indicator_et */

} s1ap_rrm_pws_request_t; /*^ API,S1AP_RRM_PWS_REQ  ^*/

typedef struct
{
    rrc_bitmask_t    bitmask;        /*^ BITMASK ^*/

#define RRC_RRM_PWS_CONTENTS_ETWS_SCHEDULING_INFO_PRESENT      0x01
#define RRC_RRM_PWS_CONTENTS_CMAS_SCHEDULING_INFO_LIST_PRESENT 0x02
#define RRC_RRM_PWS_CONTENTS_SI_TRANSMISSION_INFO_PRESENT 0x04

    rrc_cell_index_t            cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */

    scheduling_info_list_t       scheduling_info_list;      
/*^ M, 0, N, 0, 0 ^*/

    rrc_si_transmission_info_list_t    si_transmission_info;
/*^ O, RRC_RRM_PWS_CONTENTS_SI_TRANSMISSION_INFO_PRESENT ^*/

    etws_scheduling_info_t       etws_scheduling_info;
/*^ O, RRC_RRM_PWS_CONTENTS_ETWS_SCHEDULING_INFO_PRESENT ^*/

   cmas_scheduling_info_list_t   cmas_scheduling_info_list;
/*^ O, RRC_RRM_PWS_CONTENTS_CMAS_SCHEDULING_INFO_LIST_PRESENT ^*/

} rrc_rrm_pws_contents_t;

typedef struct
{
	/* SPR_17664_start*/
    U8 cell_count; /*^ M, 0, B, 1, MAX_NUM_CELLS ^*/ 
	/* SPR_17664_end*/
    rrc_rrm_pws_contents_t rrc_rrm_pws_contents[MAX_NUM_CELLS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

} rrc_rrm_pws_contents_list_t;

typedef struct 
{
    U16               bitmask;   /*^ BITMASK ^*/
#define S1AP_RRM_PWS_CONTENTS_LIST_PRESENT     0x01 
    
    U8                response;
/*^ M, 0, H, 0, 1 ^*/    /* rrc_return_et */

    rrc_rrm_pws_contents_list_t rrc_rrm_pws_contents_list;
/*^ O, S1AP_RRM_PWS_CONTENTS_LIST_PRESENT ^*/

} s1ap_rrm_pws_response_t; /*^ API,S1AP_RRM_PWS_RESP ^*/


typedef struct
{
    rrc_bitmask_t               bitmask;   /*^ BITMASK ^*/
#define S1AP_RRM_PWS_BROADCAST_LIST_FAIL_CAUSE_PRESENCE_FLAG     0x01

    rrc_cell_index_t  cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */

    U8                response;
/*^ M, 0, H, 0, 1 ^*/    /* rrc_return_et */

    U8                fail_cause;
/*^ O, S1AP_RRM_PWS_BROADCAST_LIST_FAIL_CAUSE_PRESENCE_FLAG, H, 0, 3 ^*/
/* rrm_fail_cause_et */

}pws_broadcast_resp_t;
 
typedef struct 
{
	/* SPR_17664_start*/
  U8                        count; /*^ M, 0, B, 1, MAX_NUM_CELLS ^*/
	/* SPR_17664_end*/
  pws_broadcast_resp_t     pws_broadcast_cell_resp[MAX_NUM_CELLS];
/*^ M, 0, OCTET_STRING, FIXED ^*/
}pws_broadcast_list_t;
 
typedef struct 
{
    rrc_bitmask_t               bitmask;   /*^ BITMASK ^*/
#define S1AP_RRM_PWS_BROADCAST_LIST_PRESENCE_FLAG    0x01

    U8 message_identifier[MSG_ID_OCTET_SIZE];  
/*^ M, 0, OCTET_STRING, FIXED ^*/

    U8 serial_number[SERIAL_NUMBER_OCTET_SIZE]; 
/*^ M, 0, OCTET_STRING, FIXED ^*/

    U8     pws_resp;
/*^ M, 0, H, 0, 1 ^*/    /* rrc_return_et */

    pws_broadcast_list_t pws_broadcast_list;
/*^ O, S1AP_RRM_PWS_BROADCAST_LIST_PRESENCE_FLAG ^*/

} s1ap_rrm_pws_cnf_t; /*^ API,S1AP_RRM_PWS_CNF ^*/

typedef struct 
{
    rrc_bitmask_t    bitmask;        /*^ BITMASK ^*/

#define KILL_WARNING_AREA_LIST 0x01

    U8 message_identifier[MSG_ID_OCTET_SIZE];  
/*^ M, 0, OCTET_STRING, FIXED ^*/

    U8 serial_number[SERIAL_NUMBER_OCTET_SIZE]; 
/*^ M, 0, OCTET_STRING, FIXED ^*/

    warning_area_list_info_t warning_area_list;
/*^ O, KILL_WARNING_AREA_LIST ^*/

} s1ap_rrm_kill_request_t; /*^ API,S1AP_RRM_KILL_REQ ^*/

typedef struct 
{
    U16               bitmask;   /*^ BITMASK ^*/

#define S1AP_RRM_KILL_REQ_FAIL_CAUSE_PRESENT        0x01
#define S1AP_RRM_KILL_REQ_PWS_CONTENTS_LIST_PRESENT 0x02 
    
    U8 message_identifier[MSG_ID_OCTET_SIZE];  
/*^ M, 0, OCTET_STRING, FIXED ^*/

    U8 serial_number[SERIAL_NUMBER_OCTET_SIZE]; 
/*^ M, 0, OCTET_STRING, FIXED ^*/
    
    U8                response;
/*^ M, 0, H, 0, 1 ^*/    /* rrc_return_et */

    U8                fail_cause;
/*^ O, S1AP_RRM_KILL_REQ_FAIL_CAUSE_PRESENT, H, 0, 4 ^*/
/* rrm_kill_fail_cause_et */

    rrc_rrm_pws_contents_list_t rrc_rrm_pws_contents_list;
/*^ O, S1AP_RRM_KILL_REQ_PWS_CONTENTS_LIST_PRESENT ^*/

} s1ap_rrm_kill_response_t; /*^ API,S1AP_RRM_KILL_RESP ^*/

typedef struct
{
    rrc_bitmask_t               bitmask;   /*^ BITMASK ^*/
#define S1AP_RRM_BROADCAST_CANCELLATION_FAIL_CAUSE_PRESENT  0x01

    rrc_cell_index_t  cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */

    U8                response;
/*^ M, 0, H, 0, 1 ^*/    /* rrc_return_et */

    U8                fail_cause;
/*^ O, S1AP_RRM_BROADCAST_CANCELLATION_FAIL_CAUSE_PRESENT, H, 0, 4 ^*/
/* rrc_kill_fail_cause_et */

}broadcast_cancellation_t;
 
typedef struct 
{
	/* SPR_17664_start*/
  U8                        count; /*^ M, 0, B, 1, MAX_NUM_CELLS ^*/
	/* SPR_17664_end*/
  broadcast_cancellation_t  broadcast_cancellation[MAX_NUM_CELLS];
/*^ M, 0, OCTET_STRING, FIXED ^*/
}broadcast_cancellation_list_t;
 
typedef struct 
{
    rrc_bitmask_t               bitmask;   /*^ BITMASK ^*/
    
#define S1AP_RRM_KILL_BROADCAST_CANCELLATION_LIST_PRESENT    0x01

    U8 message_identifier[MSG_ID_OCTET_SIZE];  
/*^ M, 0, OCTET_STRING, FIXED ^*/

    U8 serial_number[SERIAL_NUMBER_OCTET_SIZE]; 
/*^ M, 0, OCTET_STRING, FIXED ^*/

    U8     kill_resp;
/*^ M, 0, H, 0, 1 ^*/    /* rrc_return_et */

   broadcast_cancellation_list_t broadcast_cancellation_list;
/*^ O, S1AP_RRM_KILL_BROADCAST_CANCELLATION_LIST_PRESENT ^*/

} s1ap_rrm_kill_cnf_t; /*^ API,S1AP_RRM_KILL_CNF ^*/


#define RIM_PLMN_OCTETS             3
#define RIM_LAC_OCTETS              2
#define RIM_TAC_OCTETS              2

#define RIM_GERAN_CELL_OCTETS       2
#define RIM_RNC_ID_OCTETS           2
#define RIM_MAX_SI_PSI_PAYLOAD      127
/*SPR 19765 FIX START*/
#define RIM_MAX_SI_PAYLOAD_INDIV      21
/*SPR 19765 FIX END*/
#define RIM_MAX_SI_PSI_PAYLOAD_INDIV      22
#define RIM_MAX_UTRA_SI_PAYLOAD     2048

#define RIM_NACC_APP            1
#define RIM_SI3_APP             2
#define RIM_MBMS_APP            3
#define RIM_SON_TRANSFER_APP    4
#define RIM_UTRA_SI_APP         5

/* RAN-INFORMATION-REQUEST PDU Type Extension */
#define RAN_INFO_REQ_STOP                0x00
#define RAN_INFO_REQ_SINGLE_REPORT       0x01
#define RAN_INFO_REQ_MULTI_REPORT        0x02

/* RAN-INFORMATION PDU Type Extension */
#define RAN_INFO_STOP                    0x00
#define RAN_INFO_SINGLE_REPORT           0x01
#define RAN_INFO_INIT_MULTI_REPORT       0x02
#define RAN_INFO_MULTI_REPORT            0x03
#define RAN_INFO_END                     0x04

#define RIM_NO_ACK_REQUESTED    0 
#define RIM_ACK_REQUESTED       1 

#define RIM_ENB_TYPE_HOME_ENB   1
#define RIM_ENB_TYPE_MACRO_ENB  2

#define RIM_NACC_CAUSE_UNSPECIFIED              0x00
#define RIM_NACC_CAUSE_SYNTAX_ERROR             0x01
#define RIM_NACC_CAUSE_INVALID_CELL             0x02
#define RIM_NACC_CAUSE_SI_PSI_TYPE_ERROR        0x03
#define RIM_NACC_CAUSE_SI_PSI_INCONSISTENT_LEN  0x04
#define RIM_NACC_CAUSE_INCONSISTENT_MESSAGE     0x05

#define RIM_UTRA_SI_CAUSE_UNSPECIFIED           0x00
#define RIM_UTRA_SI_CAUSE_SYNTAX_ERROR          0x01
#define RIM_UTRA_SI_CAUSE_INCONSISTENT_CELL_ID  0x02

#define RIM_CAUSE_SEMANTICALLY_INCORRECT_PDU                0x20
#define RIM_CAUSE_INVALID_MANDATORY_INFORMATION             0x21
#define RIM_CAUSE_MISSING_MANDATORY_IE                      0x22
#define RIM_CAUSE_MISSING_CONDITIONAL_IE                    0x23
#define RIM_CAUSE_UNEXPECTED_CONDITIONAL_IE                 0x24
#define RIM_CAUSE_CONDITIONAL_IE_ERROR                      0x25
#define RIM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED                0x27
#define RIM_CAUSE_PDU_NOT_COMPATIBLE_WITH_THE_FEATURE_SET   0x28
#define RIM_CAUSE_UNKNOWN_DESTINATION_ADDRESS               0x2a
#define RIM_CAUSE_UNKNOWN_RIM_APPLICATION_IDENTITY          0x2b

typedef struct _rim_plmn
{
    /* - digits 0 to 9, encoded 0000 to 1001,
 *** - 1111 used as filler digit, two digits per octet,
 *** - bits 4 to 1 of octet n encoding digit 2n-1
 *** - bits 8 to 5 of octet n encoding digit 2n 
 *** -The PLMN identity consists of 3 digits from MCC followed by either
 *** - a filler digit plus 2 digits from MNC (in case of 2 digit MNC)
 *       *                               *      or
 *        *                                    * - 3 digits from MNC (in case of a 3 digit MNC).
 *         *                                         */
    
    U8  plmn[RIM_PLMN_OCTETS]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}rim_plmn_t;

typedef struct _rim_routing_identification
{
    rim_plmn_t  plmn; /*^ M, 0, N, 0, 0 ^*/
    U8          lac[RIM_LAC_OCTETS]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    U8          rac; /*^ M, 0, N, 0, 0 ^*/
}rim_routing_identification_t;

typedef struct _rim_ta_identity
{
    rim_plmn_t  plmn; /*^ M, 0, N, 0, 0 ^*/
    U8          tac[RIM_TAC_OCTETS]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}rim_ta_identity_t;

typedef struct _rim_global_enb_id
{
    rim_plmn_t      plmn; /*^ M, 0, N, 0, 0 ^*/
    rim_enb_type_t  enb_type; /*^ M, 0, N, 0, 0 ^*/
    U32             enb_id; /*^ M, 0, N, 0, 0 ^*/
}rim_global_enb_id_t;

typedef struct _rim_utran_cell_idty
{
    rim_plmn_t      plmn; /*^ M, 0, N, 0, 0 ^*/
    U32             cell_id; /*^ M, 0, N, 0, 0 ^*/
}rim_utran_cell_idty_t;

typedef struct _rim_geran_routing_addr
{
    rim_routing_identification_t routing_idty; /*^ M, 0, N, 0, 0 ^*/
    U8                           geran_cell_idty[RIM_GERAN_CELL_OCTETS]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}rim_geran_routing_addr_t;

typedef struct _rim_utran_routing_addr
{
    rim_routing_identification_t routing_idty; /*^ M, 0, N, 0, 0 ^*/
    U8                           rnc_id[RIM_RNC_ID_OCTETS]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}rim_utran_routing_addr_t;

typedef struct _rim_eutran_routing_addr
{
    rim_ta_identity_t   ta_idty; /*^ M, 0, N, 0, 0 ^*/
    rim_global_enb_id_t enb; /*^ M, 0, N, 0, 0 ^*/
}rim_eutran_routing_addr_t;

typedef struct _rim_routing_info
{
#define RIM_GERAN_ROUTING_ADDR_PRESENT      0x01
#define RIM_UTRAN_ROUTING_ADDR_PRESENT      0x02
#define RIM_EUTRAN_ROUTING_ADDR_PRESENT     0x04
    rim_bitmask_t               bitmask; /*^ BITMASK ^*/

    rim_geran_routing_addr_t    geran_rtng_addr; /*^ O, RIM_GERAN_ROUTING_ADDR_PRESENT, N, 0, 0 ^*/
    rim_utran_routing_addr_t    utran_rtng_addr; /*^ O, RIM_UTRAN_ROUTING_ADDR_PRESENT, N, 0, 0 ^*/
    rim_eutran_routing_addr_t   eutran_rtng_addr; /*^ O, RIM_EUTRAN_ROUTING_ADDR_PRESENT, N, 0, 0 ^*/
}rim_routing_info_t;

typedef struct _rim_req_app_contrn_nacc
{
    rim_geran_routing_addr_t  cell_idty; /*^ M, 0, N, 0, 0 ^*/
}rim_req_app_contrn_nacc_t;

typedef struct _rim_req_app_contrn_si3
{
    /* For future implementation */
   rim_bitmask_t               bitmask; /*^ BITMASK ^*/
}rim_req_app_contrn_si3_t;

typedef struct _rim_req_app_contrn_mbms
{
    /* For future implementation */
    rim_bitmask_t               bitmask; /*^ BITMASK ^*/
}rim_req_app_contrn_mbms_t;

typedef struct _rim_req_app_contrn_son
{
    /* For future implementation */
    rim_bitmask_t               bitmask; /*^ BITMASK ^*/
}rim_req_app_contrn_son_t;

typedef struct _rim_req_app_contrn_utra_si
{
    rim_utran_cell_idty_t   rpt_cell_id; /*^ M, 0, N, 0, 0 ^*/
}rim_req_app_contrn_utra_si_t;

typedef struct _rim_req_app_container
{
#define RIM_REQ_APP_CONTAINER_NACC_PRESENT      0x01
#define RIM_REQ_APP_CONTAINER_SI3_PRESENT       0x02
#define RIM_REQ_APP_CONTAINER_MBMS_PRESENT      0x04
#define RIM_REQ_APP_CONTAINER_SON_PRESENT       0x08
#define RIM_REQ_APP_CONTAINER_UTRA_SI_PRESENT   0x10
    rim_bitmask_t                   bitmask; /*^ BITMASK ^*/

    rim_req_app_contrn_nacc_t       nacc; /*^ O, RIM_REQ_APP_CONTAINER_NACC_PRESENT, N, 0, 0 ^*/
    rim_req_app_contrn_si3_t        si3; /*^ O, RIM_REQ_APP_CONTAINER_SI3_PRESENT, N, 0, 0 ^*/
    rim_req_app_contrn_mbms_t       mbms; /*^ O, RIM_REQ_APP_CONTAINER_MBMS_PRESENT, N, 0, 0 ^*/
    rim_req_app_contrn_son_t        son; /*^ O, RIM_REQ_APP_CONTAINER_SON_PRESENT, N, 0, 0 ^*/
    rim_req_app_contrn_utra_si_t    utra_si; /*^ O, RIM_REQ_APP_CONTAINER_UTRA_SI_PRESENT, N, 0, 0 ^*/
}rim_req_app_container_t;

typedef struct _rim_pdu_ind_rim_info_req
{
    rim_pdu_ext_t       pdu_ext; /*^ M, 0, H, 0, 2 ^*/
    rim_ack_t           ack; /*^ M, 0, H, 0, 1 ^*/
}rim_pdu_ind_rim_info_req_t;

typedef struct _rim_pdu_ind_rim_info
{
    rim_pdu_ext_t       pdu_ext; /*^ M, 0, H, 0, 4 ^*/
    rim_ack_t           ack; /*^ M, 0, H, 0, 1 ^*/
}rim_pdu_ind_rim_info_t;

typedef struct _rim_pdu_ind_rim_app_err
{
    rim_pdu_ext_t       pdu_ext; /*^ M, 0, B, 1, 1 ^*/
    rim_ack_t           ack; /*^ M, 0, H, 0, 1 ^*/
}rim_pdu_ind_rim_app_err_t;

typedef struct _rim_si_psi_individual
{
    U8                          si_psi[RIM_MAX_SI_PSI_PAYLOAD_INDIV]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}rim_si_psi_individual_t;

typedef struct _rim_info_app_contrn_nacc
{
#define RIM_SI_MSG  0 /* Each message is 21 octets long */
#define RIM_PSI_MSG 1 /* Each message is 22 octets long */

    rim_geran_routing_addr_t    cell_idty; /*^ M, 0, N, 0, 0 ^*/
    U8                          type; /*^ M, 0, H, 0, 1 ^*/
	/* SPR_17664_start*/
    U8                          num_si_psi; /*^ M, 0, H, 0, RIM_MAX_SI_PSI_PAYLOAD ^*/
	/* SPR_17664_end*/
rim_si_psi_individual_t     si_psi_indiv[RIM_MAX_SI_PSI_PAYLOAD]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rim_info_app_contrn_nacc_t;

typedef struct _rim_info_app_contrn_si3
{
    /* For future implementation */
    rim_bitmask_t               bitmask; /*^ BITMASK ^*/
}rim_info_app_contrn_si3_t;

typedef struct _rim_info_app_contrn_mbms
{
    /* For future implementation */
    rim_bitmask_t               bitmask; /*^ BITMASK ^*/
}rim_info_app_contrn_mbms_t;

typedef struct _rim_info_app_contrn_son
{
    /* For future implementation */
    rim_bitmask_t               bitmask; /*^ BITMASK ^*/
}rim_info_app_contrn_son_t;

typedef struct _rim_info_app_contrn_utra_si
{
    rim_utran_cell_idty_t   rpt_cell_id; /*^ M, 0, N, 0, 0 ^*/
	/* SPR_17664_start*/
    U16                     len; /*^ M,0,H,0,RIM_MAX_UTRA_SI_PAYLOAD ^*/
	/* SPR_17664_end*/
    U8                      si[RIM_MAX_UTRA_SI_PAYLOAD]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rim_info_app_contrn_utra_si_t;

typedef struct _rim_info_app_container
{
#define RIM_INFO_APP_CONTAINER_NACC_PRESENT     0x01
#define RIM_INFO_APP_CONTAINER_SI3_PRESENT      0x02
#define RIM_INFO_APP_CONTAINER_MBMS_PRESENT     0x04
#define RIM_INFO_APP_CONTAINER_SON_PRESENT      0x08
#define RIM_INFO_APP_CONTAINER_UTRA_SI_PRESENT  0x10
    rim_bitmask_t                   bitmask; /*^ BITMASK ^*/

    rim_info_app_contrn_nacc_t      nacc; /*^ O, RIM_INFO_APP_CONTAINER_NACC_PRESENT, N, 0, 0 ^*/
    rim_info_app_contrn_si3_t       si3; /*^ O, RIM_INFO_APP_CONTAINER_SI3_PRESENT, N, 0, 0 ^*/
    rim_info_app_contrn_mbms_t      mbms; /*^ O, RIM_INFO_APP_CONTAINER_MBMS_PRESENT, N, 0, 0 ^*/
    rim_info_app_contrn_son_t       son; /*^ O, RIM_INFO_APP_CONTAINER_SON_PRESENT, N, 0, 0 ^*/
    rim_info_app_contrn_utra_si_t   utra_si; /*^ O, RIM_INFO_APP_CONTAINER_UTRA_SI_PRESENT, N, 0, 0 ^*/
}rim_info_app_container_t;

typedef struct _rim_app_err_contrn_si3
{
    /* For future implementation */
rim_bitmask_t               bitmask; /*^ BITMASK ^*/
}rim_app_err_contrn_si3_t;

typedef struct _rim_app_err_contrn_mbms
{
    /* For future implementation */
rim_bitmask_t               bitmask; /*^ BITMASK ^*/
}rim_app_err_contrn_mbms_t;

typedef struct _rim_app_err_contrn_son
{
    /* For future implementation */
rim_bitmask_t               bitmask; /*^ BITMASK ^*/
}rim_app_err_contrn_son_t;


typedef struct _rim_son_transfer_app_idty
{
    /* For future implementation */
rim_bitmask_t               bitmask; /*^ BITMASK ^*/
}rim_son_transfer_app_idty_t;


typedef struct _rim_app_err_contrn_nacc
{
#define RIM_REQ_NACC_PDU_IN_ERROR_PRESENT 0x01
#define RIM_INFO_NACC_PDU_IN_ERROR_PRESENT 0x02
    rim_bitmask_t               bitmask; /*^ BITMASK ^*/
    rim_nacc_cause_t    cause; /*^ M, 0, H, 0, 5 ^*/
    rim_req_app_contrn_nacc_t  req_err_pdu; /*^ O, RIM_REQ_NACC_PDU_IN_ERROR_PRESENT, N, 0, 0 ^*/
    rim_info_app_contrn_nacc_t  info_err_pdu; /*^ O, RIM_INFO_NACC_PDU_IN_ERROR_PRESENT, N, 0, 0 ^*/
}rim_app_err_contrn_nacc_t;

typedef struct _rim_app_err_contrn_utra_si
{
#define RIM_REQ_UTRA_SI_PDU_IN_ERROR_PRESENT 0x01
#define RIM_INFO_UTRA_SI_PDU_IN_ERROR_PRESENT 0x02
    rim_bitmask_t               bitmask; /*^ BITMASK ^*/
    rim_utra_si_cause_t cause; /*^ M, 0, H, 0, 2 ^*/
    rim_req_app_contrn_utra_si_t  req_err_pdu; /*^ O, RIM_REQ_UTRA_SI_PDU_IN_ERROR_PRESENT, N, 0, 0 ^*/
    rim_info_app_contrn_utra_si_t  info_err_pdu; /*^ O, RIM_INFO_UTRA_SI_PDU_IN_ERROR_PRESENT, N, 0, 0 ^*/
}rim_app_err_contrn_utra_si_t;

typedef struct _rim_app_err_container
{
#define RIM_APP_ERR_CONTAINER_NACC_PRESENT      0x01
#define RIM_APP_ERR_CONTAINER_SI3_PRESENT       0x02
#define RIM_APP_ERR_CONTAINER_MBMS_PRESENT      0x04
#define RIM_APP_ERR_CONTAINER_SON_PRESENT       0x08
#define RIM_APP_ERR_CONTAINER_UTRA_SI_PRESENT   0x10
    rim_bitmask_t                   bitmask; /*^ BITMASK ^*/

    rim_app_err_contrn_nacc_t       nacc; /*^ O, RIM_APP_ERR_CONTAINER_NACC_PRESENT, N, 0, 0 ^*/
    rim_app_err_contrn_si3_t        si3; /*^ O, RIM_APP_ERR_CONTAINER_SI3_PRESENT, N, 0, 0 ^*/
    rim_app_err_contrn_mbms_t       mbms; /*^ O, RIM_APP_ERR_CONTAINER_MBMS_PRESENT, N, 0, 0 ^*/
    rim_app_err_contrn_son_t        son; /*^ O, RIM_APP_ERR_CONTAINER_SON_PRESENT, N, 0, 0 ^*/
    rim_app_err_contrn_utra_si_t    utra_si; /*^ O, RIM_APP_ERR_CONTAINER_UTRA_SI_PRESENT, N, 0, 0 ^*/
}rim_app_err_container_t;


/* RAN-INFORMATION-REQUEST RIM PDU */
typedef struct 
{
#define RIM_INFO_REQ_PROTOCOL_VER_PRESENT           0x01
#define RIM_INFO_REQ_APP_CONTAINER_PRESENT          0x02
#define RIM_INFO_REQ_SON_TRNFR_APP_IDNTY_PRESENT    0x04
#define RIM_INFO_REQ_MME_ID_PRESENT			0x08
    rim_bitmask_t               bitmask; /*^ BITMASK ^*/
    U8 mme_id;	/*^ M, 0, N, 0, 0 ^*/
    rim_routing_info_t          dst_cell; /*^ M, 0, N, 0, 0 ^*/
    rim_routing_info_t          src_cell; /*^ M, 0, N, 0, 0 ^*/

    rim_app_identity_t          app_idty; /*^ M, 0, B, 1, 5 ^*/
    rim_pdu_seq_num_t           seq_num; /*^ M, 0, N, 0, 0 ^*/
    rim_pdu_ind_rim_info_req_t  pdu_ind; /*^ M, 0, N, 0, 0 ^*/
    rim_protocol_ver_t          ver; /*^ O, RIM_INFO_REQ_PROTOCOL_VER_PRESENT, B, 1, 1 ^*/
    rim_req_app_container_t     app_cont; /*^ O, RIM_INFO_REQ_APP_CONTAINER_PRESENT, N, 0, 0 ^*/
    rim_son_transfer_app_idty_t son_app_idty; /*^ O, RIM_INFO_REQ_SON_TRNFR_APP_IDNTY_PRESENT, N, 0, 0 ^*/
}rim_information_req_t; /*^ API, S1AP_RRM_RIM_INFO_REQ ^*/

/* RAN-INFORMATION RIM PDU */
typedef struct 
{
#define RIM_INFO_PROTOCOL_VER_PRESENT           0x01
#define RIM_INFO_APP_CONTAINER_PRESENT          0x02
#define RIM_INFO_APP_ERR_CONTAINER_PRESENT           0x04
#define RIM_INFO_SON_TRANSFER_APP_IDNTY_PRESENT 0x08
#define RIM_INFO_MME_ID_PRESENT			0x10
    rim_bitmask_t               bitmask; /*^ BITMASK ^*/
    U8	mme_id;	/*^ M, 0, N, 0, 0 ^*/
    rim_routing_info_t          dst_cell; /*^ M, 0, N, 0, 0 ^*/
    rim_routing_info_t          src_cell; /*^ M, 0, N, 0, 0 ^*/

    rim_app_identity_t          app_idty; /*^ M, 0, B, 1, 5 ^*/
    rim_pdu_seq_num_t           seq_num; /*^ M, 0, N, 0, 0 ^*/
    rim_pdu_ind_rim_info_t      pdu_ind; /*^ M, 0, N, 0, 0 ^*/
    rim_protocol_ver_t          ver; /*^ O, RIM_INFO_PROTOCOL_VER_PRESENT, B, 1, 1 ^*/

    /* Depend on rim_app_identity_t */
    rim_info_app_container_t    app_cont; /*^ O, RIM_INFO_APP_CONTAINER_PRESENT, N, 0, 0 ^*/
    rim_app_err_container_t     err_cont; /*^ O, RIM_INFO_APP_ERR_CONTAINER_PRESENT, N, 0, 0 ^*/
    rim_son_transfer_app_idty_t son_app_idty; /*^ O, RIM_INFO_SON_TRANSFER_APP_IDNTY_PRESENT, N, 0, 0 ^*/
}rim_information_t; /*^ API, S1AP_RRM_RIM_INFO ^*/

/* RAN-INFORMATION-ACK RIM PDU */
typedef struct 
{
#define RIM_INFO_ACK_PROTOCOL_VER_PRESENT       0x01
#define RIM_INFO_ACK_SON_TRNF_APP_IDNTY_PRESENT 0x02
#define RIM_INFO_ACK_MME_ID_PRESENT 0x04
    rim_bitmask_t               bitmask; /*^ BITMASK ^*/
    U8 mme_id; /*^ M, 0, N, 0, 0 ^*/
    rim_routing_info_t          dst_cell; /*^ M, 0, N, 0, 0 ^*/
    rim_routing_info_t          src_cell; /*^ M, 0, N, 0, 0 ^*/

    rim_app_identity_t          app_idty; /*^ M, 0, B, 1, 5 ^*/
    rim_pdu_seq_num_t           seq_num; /*^ M, 0, N, 0, 0 ^*/
    rim_protocol_ver_t          ver; /*^ O, RIM_INFO_ACK_PROTOCOL_VER_PRESENT, B, 1, 1 ^*/
    rim_son_transfer_app_idty_t son_app_idty; /*^ O, RIM_INFO_ACK_SON_TRNF_APP_IDNTY_PRESENT, N, 0, 0 ^*/
}rim_information_ack_t; /*^ API, S1AP_RRM_RIM_INFO_ACK ^*/

/* RAN-INFORMATION-APPLICATION-ERROR RIM PDU */
typedef struct 
{
#define RIM_INFO_APP_ERR_PROTOCOL_VER_PRESENT       0x01
#define RIM_INFO_APP_ERR_SON_TRNF_APP_IDNTY_PRESENT 0x02
#define RIM_INFO_APP_ERR_MME_ID_PRESENT 0x04
    rim_bitmask_t               bitmask; /*^ BITMASK ^*/
    U8 mme_id; /*^ M, 0, N, 0, 0 ^*/
    rim_routing_info_t          dst_cell; /*^ M, 0, N, 0, 0 ^*/
    rim_routing_info_t          src_cell; /*^ M, 0, N, 0, 0 ^*/

    rim_app_identity_t          app_idty; /*^ M, 0, N, 0, 0 ^*/
    rim_pdu_seq_num_t           seq_num; /*^ M, 0, N, 0, 0 ^*/
    rim_pdu_ind_rim_app_err_t   pdu_ind; /*^ M, 0, N, 0, 0 ^*/
    rim_protocol_ver_t          ver; /*^ O, RIM_INFO_APP_ERR_PROTOCOL_VER_PRESENT, N, 0, 0 ^*/
    /* Depend on rim_app_identity_t */
    rim_app_err_container_t     err_cont; /*^ M, 0, N, 0, 0 ^*/ 
    rim_son_transfer_app_idty_t son_app_idty; /*^ O, RIM_INFO_APP_ERR_SON_TRNF_APP_IDNTY_PRESENT, N, 0, 0 ^*/
}rim_information_app_err_t; /*^ API, S1AP_RRM_RIM_INFO_APP_ERROR^*/

typedef struct _rim_pdu_in_error
{
    rim_bitmask_t                   bitmask; /*^ BITMASK ^*/

}rim_pdu_in_error_t;

/* RAN-INFORMATION-ERROR RIM PDU */
typedef struct 
{
#define RIM_INFO_ERR_PROTOCOL_VER_PRESENT       0x01
#define RIM_INFO_ERR_SON_TRNF_APP_IDNTY_PRESENT 0x02
#define RIM_INFO_ERR_MME_ID_PRESENT 0x04
#define RIM_INFO_PDU_IN_ERROR_PRESENT 0x08
    rim_bitmask_t               bitmask; /*^ BITMASK ^*/

    U8 mme_id; /*^ M, 0, N, 0, 0 ^*/
    rim_routing_info_t          dst_cell; /*^ M, 0, N, 0, 0 ^*/
    rim_routing_info_t          src_cell; /*^ M, 0, N, 0, 0 ^*/

    rim_app_identity_t          app_idty; /*^ M, 0, B, 1, 5 ^*/
    rim_cause_t                 cause; /*^ M, 0, H, 0, 75 ^*/
    rim_protocol_ver_t          ver; /*^ O, RIM_INFO_ERR_PROTOCOL_VER_PRESENT, B, 1, 1 ^*/
    rim_pdu_in_error_t          err_pdu; /*^ O, RIM_INFO_PDU_IN_ERROR_PRESENT, N, 0, 0 ^*/
    rim_son_transfer_app_idty_t son_app_idty; /*^ O, RIM_INFO_ERR_SON_TRNF_APP_IDNTY_PRESENT, N, 0, 0 ^*/
}rim_information_err_t; /*^ API, S1AP_RRM_RIM_INFO_ERROR ^*/ 

/******************************************************************************
 *    S1AP_OAM_MME_INFO 
 *****************************************************************************/                          

typedef struct 
{
    rrc_bitmask_t   bitmask;   /*^ BITMASK ^*/
    
    U8              mme_id; /*^ M,0,N,0,0 ^*/
    
    U8              ip_addr[MAX_IP_ADDRESS_LENGTH]; /*^ M,0,OCTET_STRING,FIXED ^*/
    
    U16             port; /*^ M,0,N,0,0 ^*/
    
    s1ap_mme_info_t mme_info; /*^ M, 0, N, 0, 0 ^*/

} s1ap_rrm_mme_info_t; /*^ API,S1AP_RRM_MME_INFO ^*/


#define MAX_NO_OF_MEAS            63
#define EUTRAN_OCTET_SIZE         4
#define SFN_OCTET_SIZE            8
#define MAX_NO_OTDA_TYPES         63
#define LPPA_MAX_CELL_REPORT      9
#define MAX_NO_ERRORS             256
#define MUTING_OCTET_ONE          1
#define MUTING_OCTET_TWO          2
#define MAX_NUM_OTDOA_TYPES       63

typedef enum
{
    LPPA_ONDEMAND,
    LPPA_PERIODIC
}lppa_report_char_et;

typedef enum
{
    UE_ASSO_LPPA,
    NON_UE_ASSO_LPPA
} lppa_asso_type;

typedef enum
{
    LPPA_MS120,
    LPPA_MS240,
    LPPA_MS480,
    LPPA_MS640,
    LPPA_MS1024,
    LPPA_MS2048,
    LPPA_MS5120,
    LPPA_MS10240,
    LPPA_MSMIN1,
    LPPA_MSMIN6,
    LPPA_MSMIN12,
    LPPA_MSMIN30,
    LPPA_MSMIN60
}lppa_meas_peridicity_et;

typedef enum
{
    LPPA_CELL_ID,
    LPPA_ANGLEOFARRIVAL,
    LPPA_TIMINGADVANCETYPE1,
    LPPA_TIMINGADVANCETYPE2,
    LPPA_RSRP,
    LPPA_RSRQ
}lppa_meas_quanify_item_et;

typedef enum
{
    LPPA_NORTH,
    LPPA_SOUTH
}lppa_latitude_sign_et;

typedef enum
{
    LPPA_HEIGHT,
    LPPA_DEPTH
}lppa_altitude_et;

typedef struct
{
    U32    meas_quantity_item;
    /*^ M, 0, H, 0, 5 ^*/  /* lppa_meas_quanify_item_et */
}lppa_meas_quantifier_t;

typedef struct
{
    U32   type; /*^ M, 0, N, 0, 0 ^*/
    U32  value; /*^M, 0, N, 0, 0 ^*/
}lppa_cause_t;

typedef struct
{
    U8 plmn_id[MAX_PLMN_ID_BYTES]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}lppa_plmn_identity_t;

typedef struct
{
    lppa_plmn_identity_t  plmn_identity;  /*^ M, 0, N, 0, 0 ^*/
    U8                    eutran_cell_id[EUTRAN_OCTET_SIZE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}lppa_ecgi_t;

typedef struct
{
    U32                   latitude_sign;/*^ M, 0, H, 0, 1 ^*//*lppa_latitude_sign_et*/
    U32                   deg_of_latitude;/*^ M, 0, H, 0, 8388607 ^*/
    S32                   deg_of_longitude;/*^ M, 0, B, -8388608, 8388607 ^*/
    U32                   deg_of_altitude;/*^ M, 0, H, 0, 1 ^*//*lppa_altitude_et*/
    U16                   altitude;       /*^ M, 0, H, 0, 32767 ^*/
    U8                    uncertainty_semi_major;/*^ M, 0, H, 0, 127 ^*/
    U8                    uncertainty_semi_minor;/*^ M, 0, H, 0, 127 ^*/
    U8                    orientation_of_major_axis;/*^ M, 0, H, 0, 179 ^*/
    U8                    uncertainty_altitude;/*^ M, 0, H, 0, 127 ^*/
    U8                    confidence;  /*^ M, 0, H, 0, 100 ^*/
}lppa_e_utran_access_point_pos_t;

typedef struct
{
    rrc_bitmask_t         bitmask; /*^ BITMASK ^*/
#define LPPA_RESULT_RSRP_ECGI_PRESENT  0x01
    U32                   pci; /*^ M, 0, H, 0, 503 ^*/
    U32                   earfcn;/*^ M, 0, H, 0, 65535 ^*/
    lppa_ecgi_t           ecgi;/*^ O, LPPA_RESULT_RSRP_ECGI_PRESENT, N, 0, 0 ^*/
    U32                   value_rsrp;/*^ M, 0, H, 0, 97 ^*/
}lppa_result_rsrp_t;

typedef struct
{
    rrc_bitmask_t         bitmask; /*^ BITMASK ^*/
#define LPPA_RESULT_RSRQ_ECGI_PRESENT  0x01
    U32                   pci;/*^ M, 0, H, 0, 503 ^*/
    U32                   earfcn;/*^ M, 0, H, 0, 65535 ^*/
    lppa_ecgi_t           ecgi;/*^ O, LPPA_RESULT_RSRQ_ECGI_PRESENT, N, 0, 0 ^*/
    U32                   value_rsrq;/*^ M, 0, H, 0, 34 ^*/
}lppa_result_rsrq_t;

typedef struct
{
    rrc_bitmask_t         bitmask; /*^ BITMASK ^*/
	/* SPR_17664_start*/
    U8                   rsrq_count;/*^ M, 0, B, 1, LPPA_MAX_CELL_REPORT ^*/
	/* SPR_17664_end*/
    lppa_result_rsrq_t    result_rsrq[LPPA_MAX_CELL_REPORT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}lppa_meas_result_rsrq_t;

typedef struct
{
    rrc_bitmask_t         bitmask; /*^ BITMASK ^*/
	/* SPR_17664_start*/
    U8                   rsrp_count;/*^ M, 0, B, 1, LPPA_MAX_CELL_REPORT ^*/
	/* SPR_17664_end*/
    lppa_result_rsrp_t    result_rsrp[LPPA_MAX_CELL_REPORT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}lppa_meas_result_rsrp_t;

typedef struct
{
    U32         bitmask; /*^ BITMASK ^*/
#define LPPA_MEAS_RESULT_VALUE_ANGLE_OF_ARRIVAL_PRESENT   0x01
#define LPPA_MEAS_RESULT_VALUE_TIMING_ADV_TYPE_1_PRESENT  0x02
#define LPPA_MEAS_RESULT_VALUE_TIMING_ADV_TYPE_2_PRESENT  0x04
#define LPPA_MEAS_RESULT_RESULT_RSRP_PRESENT              0x08
#define LPPA_MEAS_RESULT_RESULT_RSRQ_PRESENT              0x10
    U16                        value_angle_of_arrival;
    /*^ O, LPPA_MEAS_RESULT_VALUE_ANGLE_OF_ARRIVAL_PRESENT, H, 0, 719 ^*/
    U16                        value_timing_adv_type_1;
    /*^ O, LPPA_MEAS_RESULT_VALUE_TIMING_ADV_TYPE_1_PRESENT, H, 0, 7690 ^*/
    U16                        value_timing_adv_type_2;
    /*^ O, LPPA_MEAS_RESULT_VALUE_TIMING_ADV_TYPE_2_PRESENT, H, 0, 7690 ^*/
    lppa_meas_result_rsrp_t    meas_result_rsrp;
    /*^ O, LPPA_MEAS_RESULT_RESULT_RSRP_PRESENT, N, 0, 0 ^*/
    lppa_meas_result_rsrq_t    meas_result_rsrq;
    /*^ O, LPPA_MEAS_RESULT_RESULT_RSRQ_PRESENT, N, 0, 0 ^*/
}lppa_measured_res_t;

typedef struct
{
	/* SPR_17664_start*/
    U16                    num_measured_results; /*^ M, 0, H, 1, MAX_NO_OF_MEAS^*/
	/* SPR_17664_end*/
    lppa_measured_res_t    measured_results[MAX_NO_OF_MEAS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}lppa_meas_results_t;

typedef struct
{
    rrc_bitmask_t  bitmask;         /*^ BITMASK ^*/
#define LPPA_E_UTRAN_ACCESS_POINT_POSITION_PRESENT  0x01
#define LPPA_MEASUREMENT_RESULT_PRESENT             0x02
    lppa_ecgi_t    serving_cell_id; /*^ M, 0, N, 0, 0 ^*/
    U8  serving_cell_tac[TAC_OCTET_SIZE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    lppa_e_utran_access_point_pos_t   e_utran_access_point_pos;
    /*^ O, LPPA_E_UTRAN_ACCESS_POINT_POSITION_PRESENT, N, 0, 0 ^*/
    lppa_meas_results_t               meas_result;
    /*^ O, LPPA_MEASUREMENT_RESULT_PRESENT, N, 0, 0 ^*/
}lppa_e_cid_meas_result_t;

typedef enum {
    LPPA_INITIATING_MESSAGE = 0,
    LPPA_SUCCESSFUL_OUTCOME,
    LPPA_UNSUCCESSFUL_OUTCOME
}lppa_triggering_msg_et;

typedef enum {
    LPPA_REJECT = 0,
    LPPA_IGNORE,
    LPPA_NOTIFY
}lppa_proc_crit_et;

typedef struct{
    rrc_bitmask_t              bitmask; /*^ BITMASK ^*/
#define LPPA_CRIT_DIAGNO_PROCEDURE_CODE_PRESENT    0x01
#define LPPA_CRIT_DIAGNO_TRIGERRING_MSG_PRESENT    0x02
#define LPPA_CRIT_DIAGNO_PROC_CRITICALITY_PRESENT  0x04
#define LPPA_CRIT_DIAGNO_TRANS_ID_PRESENT          0x08
    U8                         procedure_code;
    /*SPR_17020_START*/
    /*^ O, LPPA_CRIT_DIAGNO_PROCEDURE_CODE_PRESENT, N, 0, 255 ^*/
    /*SPR_17020_END*/
    U32                        triggering_msg;
    /*^ O, LPPA_CRIT_DIAGNO_TRIGERRING_MSG_PRESENT, H, 0, 2 ^*//*lppa_triggering_msg_et*/
    U32                        proc_critic;
    /*^O, LPPA_CRIT_DIAGNO_PROC_CRITICALITY_PRESENT, H, 0, 2 ^*//*lppa_proc_crit_et*/
    U32                        lppa_trans_id;      
    /*^ O, LPPA_CRIT_DIAGNO_TRANS_ID_PRESENT, H, 0, 32767 ^*/
}lppa_criticality_diagno_t;

typedef enum
{
    LPPA_PCI,
    LPPA_CELL_Id,
    LPPA_TAC,
    LPPA_EARFCN,
    LPPA_PRSBANDWIDTH,
    LPPA_PRSCONFIGINDEX,
    LPPA_CPLENGTH,
    LPPA_NODIFRAMES,
    LPPA_NONANTENAPORTS,
    LPPA_SFNINITTIME,
    LPPA_E_UTRANACCESSPOINTPOSITION,
    LPPA_PRSMUTICONFIGURATION
}lppa_otdoa_info_item_et;

typedef struct
{
    rrc_bitmask_t  bitmask;            /*^ BITMASK ^*/
    U32        otdoa_info_item;
    /*^ M, 0, H, 0, 11 ^*//*lppa_otdoa_info_item_et*/
}lppa_otdoa_info_type_t;

typedef enum
{
    LPPA_N1_OR_N2,
    LPPA_N4
}lppa_no_of_antena_port_et;

typedef enum
{
    LPPA_SF1,
    LPPA_SF2,
    LPPA_SF3,
    LPPA_SF4
}lppa_no_of_dl_frames_et;

typedef enum
{
    LPPA_NORMAL,
    LPPA_EXTENDED
}lppa_cp_len_et;

typedef enum
{
    LPPA_BW6,
    LPPA_BW15,
    LPPA_BW25,
    LPPA_BW50,
    LPPA_BW75,
    LPPA_BW100
}lppa_earfcn_et;
typedef struct
{
    U8    two;/*^ M, 0, H, 0, 4 ^*/
}lppa_muting_config_two_t;
typedef struct
{
    U8    four;/*^ M, 0, H, 0, 15 ^*/
}lppa_muting_config_four_t;
typedef struct
{
    U8    eight[MUTING_OCTET_ONE];/*^ M, 0, OCTET_STRING, FIXED ^*/
}lppa_muting_config_eight_t;
typedef struct
{
    U8    sixteen[MUTING_OCTET_TWO];/*^ M, 0, OCTET_STRING, FIXED ^*/
}lppa_muting_config_sixteen_t;
typedef struct
{
    rrc_bitmask_t  bitmask;            /*^ BITMASK ^*/
#define LPPA_PRS_MUTING_CONFIG_TWO_PRESENT      0x01
#define LPPA_PRS_MUTING_CONFIG_FOUR_PRESENT     0x02
#define LPPA_PRS_MUTING_CONFIG_EIGHT_PRESENT    0x04
#define LPPA_PRS_MUTING_CONFIG_SIXTEEN_PRESENT  0x08
    lppa_muting_config_two_t  muting_config_two;
    /*^ O, LPPA_PRS_MUTING_CONFIG_TWO_PRESENT, N, 0, 0 ^*/
    lppa_muting_config_four_t  muting_config_four;
    /*^ O, LPPA_PRS_MUTING_CONFIG_FOUR_PRESENT, N, 0, 0 ^*/
    lppa_muting_config_eight_t  muting_config_eight;
    /*^ O, LPPA_PRS_MUTING_CONFIG_EIGHT_PRESENT, N, 0, 0 ^*/
    lppa_muting_config_sixteen_t  muting_config_sixteen;
    /*^ O, LPPA_PRS_MUTING_CONFIG_SIXTEEN_PRESENT, N, 0, 0 ^*/
}lppa_prs_muting_config_t;

typedef struct
{
    U8    tac[TAC_OCTET_SIZE];/*^ M, 0, OCTET_STRING, FIXED ^*/
}lppa_otdoa_cells_tac_t;

typedef struct
{
    U8    sfn_initiation_time[SFN_OCTET_SIZE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}lppa_otdoa_sfn_init_time_t;

typedef struct
{
    U32     bitmask;                 /*^ BITMASK ^*/
#define LPPA_OTDOA_CELLS_PCI_PRESENT                 0x01
#define LPPA_OTDOA_CELLS_CELL_ID_PRESENT             0x02
#define LPPA_OTDOA_CELLS_TAC_PRESENT                 0x04
#define LPPA_OTDOA_CELLS_EARFCN_PRESENT              0x08
#define LPPA_OTDOA_CELLS_PRS_BAND_WIDTH_PRESENT      0x10
#define LPPA_OTDOA_CELLS_CONFIG_INDEX_PRESENT        0x20
#define LPPA_OTDOA_CELLS_CP_LENGTH_PRESENT           0x40
#define LPPA_OTDOA_CELLS_DL_FRAMES_PRESENT           0x80
#define LPPA_OTDOA_CELLS_ANTENA_PORTS_PRESENT        0x100
#define LPPA_OTDOA_CELLS_SFN_INIT_TIME_PRESENT       0x200
#define LPPA_OTDOA_CELLS_ACCESS_POINT_POS_PRESENT    0x400
#define LPPA_OTDOA_CELLS_PRS_MUTING_CONFIG_PRESENT   0x800
    U32                 pci;                
    /*^ O, LPPA_OTDOA_CELLS_PCI_PRESENT, H, 0, 503 ^*/
    lppa_ecgi_t         cell_id;            
    /*^ O, LPPA_OTDOA_CELLS_CELL_ID_PRESENT, N, 0, 0 ^*/
    lppa_otdoa_cells_tac_t  otdoa_cells_tac;
    /*^ O, LPPA_OTDOA_CELLS_TAC_PRESENT, N, 0, 0 ^*/
    U32                 earfcn;             
    /*^ O, LPPA_OTDOA_CELLS_EARFCN_PRESENT, H, 0, 65535 ^*/
    U32                 prs_band_width;     
    /*^ O, LPPA_OTDOA_CELLS_PRS_BAND_WIDTH_PRESENT, H, 0, 5 ^*/ /*lppa_earfcn_et*/
    U32                 prs_config_index;   
    /*^ O, LPPA_OTDOA_CELLS_CONFIG_INDEX_PRESENT, H, 0, 4095 ^*/
    U32                 cp_length;          
    /*^ O, LPPA_OTDOA_CELLS_CP_LENGTH_PRESENT, H, 0, 1 ^*/ /*lppa_cp_len_et*/
    U32                 no_of_dl_frames;    
    /*^ O, LPPA_OTDOA_CELLS_DL_FRAMES_PRESENT, H, 0, 3 ^*/ /*lppa_no_of_dl_frames_et*/
    U32                 no_of_antenna_ports;
    /*^ O, LPPA_OTDOA_CELLS_ANTENA_PORTS_PRESENT, H, 0, 1 ^*/ /*lppa_no_of_antena_port_et*/
    lppa_otdoa_sfn_init_time_t  otdoa_sfn_init_time;
    /*^ O, LPPA_OTDOA_CELLS_SFN_INIT_TIME_PRESENT, N, 0, 0 ^*/
    lppa_e_utran_access_point_pos_t  e_utran_acces_point_pos;
    /*^ O, LPPA_OTDOA_CELLS_ACCESS_POINT_POS_PRESENT, N, 0, 0 ^*/
    lppa_prs_muting_config_t           prs_muting_config;
    /*^ O, LPPA_OTDOA_CELLS_PRS_MUTING_CONFIG_PRESENT, N, 0, 0 ^*/
}lppa_otdoa_cells_info_t;

typedef struct
{
    rrc_bitmask_t  bitmask;            /*^ BITMASK ^*/
#define LPPA_MEAS_INIT_REQ_MEAS_PERIODICITY_PRESENT    0x01
    U8             mme_id;             /*^ M, 0, H, 0, 15 ^*/
    U16            ue_index;           
    /*SPR_17020_START*/
    U8             routing_id;         /*^ M, 0, N, 0, 255 ^*/
    /*SPR_17020_END*/
    U32            lppa_trans_id;      /*^ M, 0, H, 0, 32767 ^*/
    U32            e_smlc_meas_id;     /*^ M, 0, B, 1, 15 ^*/
    U32            report_char;
    /*^ M, 0, H, 0, 1 ^*/ /* lppa_report_char_et */
    U32            meas_periodicity;   
    /*^ O, LPPA_MEAS_INIT_REQ_MEAS_PERIODICITY_PRESENT, H, 0, 12 ^*/ 
    /* lppa_meas_peridicity_et */
	/* SPR_17664_start*/
    U8             num_meas_quantifier;/*^ M, 0, B, 1, MAX_NO_OF_MEAS ^*/
	/* SPR_17664_end*/
    lppa_meas_quantifier_t  meas_quantifier[MAX_NO_OF_MEAS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}s1ap_rrm_lppa_ecid_meas_init_req_t;/*^ API, S1AP_RRM_LPPA_ECID_MEAS_INIT_REQ ^*/

typedef struct
{
    rrc_bitmask_t  bitmask;      /*^ BITMASK ^*/    
#define LPPA_MEAS_INIT_RESP_ECID_MEAS_RESULT_PRESENT    0x01
    U8             mme_id;             /*^ M, 0, H, 0, 15^*/
    U16            ue_index; 
    /*SPR_17020_START*/
    U8             routing_id;         /*^ M, 0, N, 0, 255 ^*/
    /*SPR_17020_END*/
    U32            lppa_trans_id;      /*^ M, 0, H, 0, 32767 ^*/
    U32            e_smlc_ue_meas_id;  /*^ M, 0, B, 1, 15 ^*/
    U32            enb_ue_meas_id;     /*^ M, 0, B, 1, 15 ^*/
    lppa_e_cid_meas_result_t   e_cid_meas_result;
    /*^ O, LPPA_MEAS_INIT_RESP_ECID_MEAS_RESULT_PRESENT, N, 0, 0 ^*/
}s1ap_rrm_lppa_ecid_meas_init_resp_t;/*^ API, S1AP_RRM_LPPA_ECID_MEAS_INIT_RESP ^*/

typedef struct
{
    rrc_bitmask_t  bitmask;            /*^ BITMASK ^*/    
    U8             mme_id;             /*^ M, 0, H, 0, 15^*/
    U16            ue_index; 
    /*SPR_17020_START*/
    U8             routing_id;         /*^ M, 0, N, 0, 255 ^*/
    /*SPR_17020_END*/
    U32            lppa_trans_id;      /*^ M, 0, H, 0, 32767 ^*/
    U32            e_smlc_ue_meas_id;  /*^ M, 0, B, 1, 15 ^*/
    lppa_cause_t   cause;              /*^ M, 0, N, 0, 0 ^*/ 
}s1ap_rrm_lppa_ecid_meas_init_fail_t;/*^ API, S1AP_RRM_LPPA_ECID_MEAS_INIT_FAIL ^*/

typedef struct
{
    rrc_bitmask_t  bitmask;            /*^ BITMASK ^*/    
    U8             mme_id;             /*^ M, 0, H, 0, 15^*/
    U16            ue_index;    
    /*SPR_17020_START*/
    U8             routing_id;         /*^ M, 0, N, 0, 255 ^*/
    /*SPR_17020_END*/
    U32            lppa_trans_id;      /*^ M, 0, H, 0, 32767 ^*/
    U32            e_smlc_ue_meas_id;  /*^ M, 0, B, 1, 15 ^*/
    U32            enb_ue_meas_id;     /*^ M, 0, B, 1, 15 ^*/
    lppa_cause_t   cause;              /*^ M, 0, N, 0, 0 ^*/ 
}s1ap_rrm_lppa_ecid_meas_fail_ind_t;/*^ API, S1AP_RRM_LPPA_ECID_MEAS_FAIL_IND ^*/

typedef struct
{
    rrc_bitmask_t  bitmask;            /*^ BITMASK ^*/    
    U8             mme_id;             /*^ M, 0, H, 0, 15^*/
    U16            ue_index; 
    U8             routing_id;         /*^ M, 0, N, 0, 255 ^*/
    U32            lppa_trans_id;      /*^ M, 0, H, 0, 32767 ^*/
    U32            e_smlc_ue_meas_id;  /*^ M, 0, B, 1, 15 ^*/
    U32            enb_ue_meas_id;     /*^ M, 0, B, 1, 15 ^*/
    lppa_e_cid_meas_result_t  e_cid_meas_result; /*^ M, 0, N, 0, 0 ^*/
}s1ap_rrm_lppa_ecid_meas_report_t;/*^ API, S1AP_RRM_LPPA_ECID_MEAS_REPORT ^*/

typedef struct
{
    rrc_bitmask_t  bitmask;            /*^ BITMASK ^*/    
    U8             mme_id;             /*^ M, 0, H, 0, 15^*/
    U16            ue_index;
    /*SPR_17020_START*/
    U8             routing_id;         /*^ M, 0, N, 0, 255 ^*/
    /*SPR_17020_END*/
    U32            lppa_trans_id;      /*^ M, 0, H, 0, 32767 ^*/
    U32            e_smlc_ue_meas_id;  /*^ M, 0, B, 1, 15 ^*/
    U32            enb_ue_meas_id;     /*^ M, 0, B, 1, 15 ^*/
}s1ap_rrm_lppa_ecid_meas_termination_cmd_t;/*^ API, S1AP_RRM_LPPA_ECID_MEAS_TERMINATION_COMMAND ^*/

typedef struct
{
    rrc_bitmask_t  bitmask;            /*^ BITMASK ^*/    
    U8             mme_id;             /*^ M, 0, H, 0, 15^*/
    /*SPR_17020_START*/
    U8             routing_id;         /*^ M, 0, N, 0, 255 ^*/
    /*SPR_17020_END*/
    U32            lppa_trans_id;      /*^ M, 0, H, 0, 32767 ^*/
	/* SPR_17664_start*/
    U16            otdoa_info_count;   /*^ M, 0, H, 0, MAX_NO_OTDA_TYPES ^*/
	/* SPR_17664_end*/
    lppa_otdoa_info_type_t  otda_info_type_arr[MAX_NO_OTDA_TYPES];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}s1ap_rrm_lppa_otdoa_info_req_t;/*^ API, S1AP_RRM_LPPA_OTDOA_INFO_REQ ^*/

typedef struct
{
	/* SPR_17664_start*/
    U8            otdoa_cell_info_count;      /*^ M, 0, H, 0,MAX_NUM_OTDOA_TYPES ^*/
	/* SPR_17664_end*/
    lppa_otdoa_cells_info_t   otda_cells_info_arr[MAX_NUM_OTDOA_TYPES];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}lppa_otdoa_cells_t;
typedef struct
{
    rrc_bitmask_t  bitmask;            /*^ BITMASK ^*/    
    U8             mme_id;             /*^ M, 0, H, 0, 15^*/
    U8             routing_id;         /*^ M, 0, N, 0, 255 ^*/
    U32            lppa_trans_id;      /*^ M, 0, H, 0, 32767 ^*/
	/* SPR_17664_start*/
    U16            otdoa_cell_count;   /*^ M, 0, B, 1,MAX_NUM_CELLS ^*/
	/* SPR_17664_end*/
    lppa_otdoa_cells_t  otda_cells_arr[MAX_NUM_CELLS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}s1ap_rrm_lppa_otdoa_info_resp_t;/*^ API, S1AP_RRM_LPPA_OTDOA_INFO_RESP ^*/

typedef struct
{
    rrc_bitmask_t  bitmask;         /*^ BITMASK ^*/    
    U8             mme_id;          /*^ M, 0, H, 0, 15^*/
    /*SPR_17020_START*/
    U8             routing_id;      /*^ M, 0, N, 0, 255 ^*/
    /*SPR_17020_END*/
    U32            lppa_trans_id;   /*^ M, 0, H, 0, 32767 ^*/
    lppa_cause_t   cause;           /*^ M, 0, N, 0, 0 ^*/
}s1ap_rrm_lppa_otdoa_info_fail_t;/*^ API, S1AP_RRM_LPPA_OTDOA_INFO_FAIL^*/

typedef struct
{
    rrc_bitmask_t  bitmask;      /*^ BITMASK ^*/
#define LPPA_ERROR_IND_UE_INDEX_PRESENT       0x01
#define LPPA_ERROR_IND_CAUSE_PRESENT          0x02
#define LPPA_ERROR_IND_CRITIC_DIAG_PRESENT    0x04
    U8             mme_id;         /*^ M, 0, H, 0, 15 ^*/
    U16            ue_index;       /*^ O, LPPA_ERROR_IND_UE_INDEX_PRESENT, N, 0, 0 ^*/
    /*SPR_17020_START*/
    U8             routing_id;      /*^ M, 0, N, 0, 255 ^*/
    /*SPR_17020_END*/
    U32            lppa_trans_id;  /*^ M, 0, H, 0, 32767 ^*/
    lppa_cause_t   cause;          /*^ O, LPPA_ERROR_IND_CAUSE_PRESENT, N, 0, 0 ^*/ 
    lppa_criticality_diagno_t  criticality_diagnostics;
    /*^ O, LPPA_ERROR_IND_CRITIC_DIAG_PRESENT, N, 0, 0 ^*/
}s1ap_rrm_lppa_error_ind_t;/*^ API, S1AP_RRM_LPPA_ERROR_IND ^*/

typedef struct
{
    rrc_bitmask_t  bitmask;      /*^ BITMASK ^*/
#define LPPA_ERROR_IND_UE_INDEX_PRESENT       0x01
    U8             mme_id;         /*^ M, 0, H, 0, 15 ^*/
    U16            ue_index;       /*^ O, LPPA_ERROR_IND_UE_INDEX_PRESENT, N, 0, 0 ^*/
    /*SPR_17020_START*/
    U8             routing_id;      /*^ M, 0, N, 0, 255 ^*/
    /*SPR_17020_END*/
    U32            lppa_trans_id;  /*^ M, 0, H, 0, 32767 ^*/
    lppa_cause_t   cause;          /*^ M, 0, N, 0, 0 ^*/ 
    lppa_criticality_diagno_t  criticality_diagnostics; /*^ M, 0, N, 0, 0 ^*/
}s1ap_rrm_lppa_error_ind_req_t;/*^ API, S1AP_RRM_LPPA_ERROR_IND_REQ ^*/

#pragma pack(pop)

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
#endif   /* ----- #ifndef RRC_S1APRRM_INTF_INC  ----- */

