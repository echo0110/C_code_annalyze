/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: rrc_s1ap_csc_intf.h,v 1.4 2010/02/24 04:24:18 gur10248 Exp $
 ****************************************************************************
 *
 *  File Description :
 *      This file contains types used for
 *      representation of S1AP - CSC API.
 *      Based on LTE_L3_API_Rel_1_0_Rev_0_45.doc
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rrc_s1ap_csc_intf.h,v $
 * Revision 1.4  2010/02/24 04:24:18  gur10248
 * comments_dpisposed_cell_del_cell_setup_1_1
 *
 * Revision 1.3  2010/01/11 11:56:00  gur10248
 * *** empty log message ***
 *
 * Revision 1.2  2010/01/04 16:10:03  ukr15916
 * no message
 *
 * Revision 1.1.4.1  2009/11/26 18:33:42  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.2.3  2009/11/19 09:52:11  ukr15916
 * tabs -> spaces
 *
 * Revision 1.1.2.2  2009/11/16 17:30:51  ukr16018
 * S1AP - CSC interface updated.
 *
 * Revision 1.1.2.1  2009/11/16 13:37:03  ukr16018
 * S1AP is internal module now 1.
 *
 *
 *
 *
 ****************************************************************************/

#ifndef INCLUDED_RRC_S1AP_CSC_INTF_H
#define INCLUDED_RRC_S1AP_CSC_INTF_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "rrc_defines.h"


/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

#pragma pack(push, 1)



/******************************************************************************
    S1AP_PAGING_IND
******************************************************************************/

typedef struct
{
     U8  paging_msg_buf[0];
} s1ap_paging_ind_t;/*^ API ^*/ 

/******************************************************************************
    S1AP_CELL_REG_REQ
******************************************************************************/

typedef struct
{
    rrc_cell_index_t cell_index;
    cell_area_info_t     cell_area_info;      
    U16                  max_num_supported_ue;
} s1ap_cell_reg_req_t;/*^ API ^*/ 

/******************************************************************************
    S1AP_CELL_AREA_INFO_UPDATE_IND
******************************************************************************/


typedef struct 
{
    rrc_cell_index_t cell_index;
    cell_area_info_t cell_area_info;
}s1ap_cell_area_info_update_ind_t;/*^ API ^*/ 
    
/******************************************************************************
    S1AP_CELL_DREG_REQ
******************************************************************************/
typedef struct
{
  rrc_cell_index_t cell_index;
} s1ap_cell_dreg_req_t;/*^ API ^*/ 

/*****************************************************************************
 S1AP_PWS_REQ 
 ****************************************************************************/

#define PWS_EXTENDED_REPETITION_PERIOD_PRESENT      0x0001
#define PWS_WARNING_TYPE_PRESENT                    0x0002
#define PWS_WARNING_SECURITY_INFO_PRESENT           0x0004
#define PWS_DATA_CODING_SCHEME_PRESENT              0x0008
#define PWS_WARNING_MESSAGE_SIZE_PRESENT            0x0010
#define PWS_WARNING_MESSAGE_CONTENTS_PRESENT        0x0020
#define PWS_CONCURRENT_WARNING_MSG_INDICATOR        0x0040
#define PWS_ETWS_SCHEDULING_INFO                    0x0080
#define PWS_CMAS_SCHEDULING_INFO_LIST               0x0100
#define PWS_SI_TRANSMISSION_INFO_LIST               0x0200

typedef struct
{ 
    rrc_bitmask_t               bitmask;
    rrc_transaction_id_t        transaction_id; 
    rrc_cell_index_t            cell_index;
    U8                          message_identifier[MSG_ID_OCTET_SIZE];  
    U8                          serial_number[SERIAL_NUMBER_OCTET_SIZE]; 
    U16                         repetition_period;
    U32                         extended_repetition_period;
    U16                         number_of_broadcasts_requested;
    scheduling_info_list_t      scheduling_info_list;  
    U8                          warning_type[WARNING_TYPE_OCTET_SIZE];
    U8                          warning_security_info[SECURITY_INFORMATION_OCTET_SIZE];
    U8                          data_coding_scheme[DATA_CODING_SCHEME_OCTET_SIZE];
    U16                         warning_msg_size;
    U8                          wrw_message_contents[WARNING_MSG_SEGMENT_OCTET_SIZE];
    U8                          concurrent_warning_msg_indicator; /* concurrent_warning_msg_indicator_et */
    etws_scheduling_info_t      etws_scheduling_info;
    cmas_scheduling_info_list_t cmas_scheduling_info_list;
    rrc_si_transmission_info_list_t  si_transmission_info;   
}s1ap_pws_req_t;/*^ API ^*/ 


#define PWS_RESP_FAIL_CAUSE_PRESENT 0x0001
typedef struct
{ 
    rrc_bitmask_t               bitmask;
    rrc_transaction_id_t        transaction_id; 
    rrc_cell_index_t            cell_index;
    rrc_response_t              pws_response;
    U8                          pws_fail_cause; /* rrm_fail_cause_et*/
}s1ap_pws_resp_t;/*^ API ^*/ 

typedef struct
{
    rrc_transaction_id_t    transaction_id; 
    rrc_cell_index_t        cell_index;
}s1ap_pws_failure_req_t;/*^ API ^*/ 


typedef struct
{
    rrc_transaction_id_t    transaction_id; 
    rrc_cell_index_t        cell_index;
    rrc_response_t          response;
}s1ap_pws_failure_resp_t;/*^ API ^*/ 

/*****************************************************************************
 S1AP_KILL_REQ 
 ****************************************************************************/

#define KILL_ETWS_SCHEDULING_INFO                    0x0001
#define KILL_CMAS_SCHEDULING_INFO_LIST               0x0002
#define KILL_SI_TRANSMISSION_INFO_LIST               0x0004

typedef struct
{ 
    rrc_bitmask_t               bitmask;
    rrc_transaction_id_t        transaction_id; 
    rrc_cell_index_t            cell_index;
    U8                          message_id[MSG_ID_OCTET_SIZE];
    U8                          serial_number[SERIAL_NUMBER_OCTET_SIZE];
    scheduling_info_list_t      scheduling_info_list;  
    etws_scheduling_info_t      etws_scheduling_info;
    cmas_scheduling_info_list_t cmas_scheduling_info_list;
    rrc_si_transmission_info_list_t  si_transmission_info;   
}s1ap_kill_req_t;/*^ API ^*/ 


#define KILL_RESP_FAIL_CAUSE_PRESENT 0x0001
#define KILL_RESP_NUMBER_OF_BROADCASTS_PRESENT 0x0001
typedef struct
{ 
    rrc_bitmask_t               bitmask;
    rrc_transaction_id_t        transaction_id; 
    rrc_cell_index_t            cell_index;
    rrc_response_t              kill_response;
    U8                          kill_fail_cause; /* rrm_kill_fail_cause_et*/
    U16                         number_of_broadcasts;
}s1ap_kill_resp_t;/*^ API ^*/ 

typedef struct
{
    rrc_cell_index_t            cell_index;
}s1ap_kill_failure_ind_t;/*^ API ^*/ 


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

#endif  /* INCLUDED_RRC_S1AP_CSC_INTF_H */

