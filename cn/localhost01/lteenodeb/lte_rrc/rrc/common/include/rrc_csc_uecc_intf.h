/******************************************************************************
*
*   FILE NAME: rrc_csc_uecc_api_int.h
*
*   DESCRIPTION:
*       This file contains types used for representation of CSC-UECC API
*       inside RRC.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   05 June 2009    Yusuf R     ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _RRC_CSC_UECC_API_INT_H_
#define _RRC_CSC_UECC_API_INT_H_

#include "rrc_defines.h"
#include "rrc_rrm_intf.h"
#pragma pack(push, 1)

/*REL 1.2: HANDOVER PROCEDURE START*/

/******************************************************************************
*   CSC_UECC_INIT_SETUP_IND
******************************************************************************/

/*#define CSC_UECC_INIT_SETUP_CSG_ID_PRESENT  0x01*/

typedef enum _rrc_cell_state_et
{
    RRC_CSC_CELL_ACTIVE,
    RRC_CSC_CELL_STOPPED,
}cell_state_et;


typedef struct
{

/* CDMA2000_CSFB_HO start */
    rrc_bitmask_t           bitmask;/*^ BITMASK ^*/
#define RRC_CSC_UECC_CDMA2000_RAND_PRESENT 0x01
#define RRC_CSC_UECC_CDMA2000_1XRTT_CELL_ID_PRESENT 0x02
#define RRC_CSC_UECC_CDMA2000_HRPD_SECTOR_ID_PRESENT 0x04
/* CDMA2000_CSFB_HO stop */
/* CSR 00070260 Changes Start */
#define RRC_CSC_UECC_CDMA2000_1XRTT_REF_CELL_ID_PRESENCE_FLAG 0x08
#define RRC_CSC_UECC_CDMA2000_HRPD_REF_CELL_ID_PRESENCE_FLAG  0x10
/* CSR 00070260 Changes Stop */
/* SPR 23209 FIXED START*/
#define RRC_CSC_UECC_MOCN_OPERATOR_PRESENCE_FLAG  0x20
/* SPR 23209 FIXED END*/


    rrc_cell_index_t        cell_index;
    U8                      num_of_antennas;/*^ M, 0 , H, 1, 4 ^*/
/*** TDD changes start ***/
    duplexing_mode_et       duplexing_mode;
/*** TDD changes stop ***/
        
    mib_info_t              mib_info; 
    sib_type_1_Info_t       sib_type_1_Info;
    sib_type_2_Info_t       sib_type_2_Info; 

    U32                     eutra_dl_carrier_freq; /*^ M, 0, H, 0, MAX_EARFCN^*/ 
    rrc_phys_cell_id_t      phys_cell_id;    /*^ M, 0, H, 0, 503 ^*/
    U8                      cell_state;     /*^ M, 0 , H, 0, 1 ^*/ /*^ rrc_cell_state_et ^*/

/* CDMA2000_CSFB_HO start */
    U32             cdma2000_rand; 
    /*^ O, RRC_CSC_UECC_CDMA2000_RAND_PRESENT, N, 0, 0 ^*/
    cdma2000_1xrtt_cell_identifier_t     cell_id_1xrtt;
    /*^ O, RRC_CSC_UECC_CDMA2000_1XRTT_CELL_ID_PRESENT, N, 0, 0 ^*/
    cdma2000_hrpd_sector_identifier_t    sector_id_hrpd;
    /*^ O, RRC_CSC_UECC_CDMA2000_HRPD_SECTOR_ID_PRESENT, N, 0, 0 ^*/
/* CDMA2000_CSFB_HO stop */
/* CSR 00070260 Changes Start */
    cdma2000_1xrtt_ref_cell_identifier_t  ref_cell_id_1xrtt;  
    /*^ O, RRC_CSC_UECC_CDMA2000_1XRTT_REF_CELL_ID_PRESENCE_FLAG, N, 0, 0 ^*/
    cdma2000_hrpd_ref_sector_identifier_t ref_sector_id_hrpd;    
    /*^ O, RRC_CSC_UECC_CDMA2000_HRPD_REF_CELL_ID_PRESENCE_FLAG, N, 0, 0 ^*/
/* CSR 00070260 Changes Stop */

    /* SPR 23209 FIXED START*/
    U8 operator_id[MAX_PLMN_IDS];
    /*^ O, RRC_CSC_UECC_MOCN_OPERATOR_PRESENCE_FLAG, OCTET_STRING, FIXED,N, 0, 0 ^*/
    /* SPR 23209 FIXED END*/

} rrc_csc_uecc_init_setup_ind_t;/*^ API ^*/ 


/********************************************************************************
  * CSC_UECC_CELL_RECONFIG_IND
********************************************************************************/
typedef struct
{
/* CDMA2000_CSFB_HO start */
    rrc_bitmask_t           bitmask;/*^ BITMASK ^*/
#define RRC_CSC_UECC_CDMA2000_RAND_PRESENT 0x01
#define RRC_CSC_UECC_CDMA2000_1XRTT_CELL_ID_PRESENT 0x02
#define RRC_CSC_UECC_CDMA2000_HRPD_SECTOR_ID_PRESENT 0x04
/* CDMA2000_CSFB_HO stop */
/* CSR 00070260 Changes Start */
#define RRC_CSC_UECC_CDMA2000_1XRTT_REF_CELL_ID_PRESENCE_FLAG 0x08
#define RRC_CSC_UECC_CDMA2000_HRPD_REF_CELL_ID_PRESENCE_FLAG  0x10
/* CSR 00070260 Changes Stop */

    rrc_cell_index_t        cell_index;
    U8                      num_of_antennas;

    mib_info_t              mib_info;
    sib_type_1_Info_t       sib_type_1_Info;
    sib_type_2_Info_t       sib_type_2_Info;

    U32                     eutra_dl_carrier_freq;
    rrc_phys_cell_id_t      phys_cell_id;   
/* CDMA2000_CSFB_HO start */
    U32             cdma2000_rand; 
    /*^ O, RRC_CSC_UECC_CDMA2000_RAND_PRESENT, N, 0, 0 ^*/
    cdma2000_1xrtt_cell_identifier_t     cell_id_1xrtt;
    /*^ O, RRC_CSC_UECC_CDMA2000_1XRTT_CELL_ID_PRESENT, N, 0, 0 ^*/
    cdma2000_hrpd_sector_identifier_t    sector_id_hrpd;
    /*^ O, RRC_CSC_UECC_CDMA2000_HRPD_SECTOR_ID_PRESENT, N, 0, 0 ^*/
/* CDMA2000_CSFB_HO stop */
/* CSR 00070260 Changes Start */
    cdma2000_1xrtt_ref_cell_identifier_t  ref_cell_id_1xrtt;  
    /*^ O, RRC_CSC_UECC_CDMA2000_1XRTT_REF_CELL_ID_PRESENCE_FLAG, N, 0, 0 ^*/
    cdma2000_hrpd_ref_sector_identifier_t ref_sector_id_hrpd;    
    /*^ O, RRC_CSC_UECC_CDMA2000_HRPD_REF_CELL_ID_PRESENCE_FLAG, N, 0, 0 ^*/
/* CSR 00070260 Changes Stop */
} rrc_csc_uecc_cell_reconfig_ind_t;/*^ API ^*/ 

/*REL 1.2:HANDOVER PROCEDURE END*/

/********************************************************************************
  * CSC_UECC_DELETE_ALL_UE_REQ
********************************************************************************/

typedef struct
{
     rrc_cell_index_t        cell_index;
     rrc_bool_et             is_cell_stopped;
    /* SPR 21458 Start */
    U8                          l2_instance_reset_flag; /* Indicates the Layer2 presence during cell delete */ 
    /* SPR 21458 End */


} rrc_csc_uecc_delete_all_ue_req_t;/*^ API ^*/ 

/********************************************************************************
  * CSC_UECC_DELETE_ALL_UE_RESP
********************************************************************************/
typedef struct
{
    rrc_cell_index_t  cell_index;
    U8                result;
}rrc_csc_uecc_delete_all_ue_resp_t;/*^ API ^*/ 

/********************************************************************************
  * CSC_UECC_CELL_START_IND
********************************************************************************/
typedef struct _rrc_csc_uecc_cell_start_ind_t
{
    rrc_cell_index_t  cell_index;
}rrc_csc_uecc_cell_start_ind_t;/*^ API ^*/ 

/********************************************************************************
  * CSC_UECC_CELL_STOP_IND
********************************************************************************/
typedef struct _rrc_csc_uecc_cell_stop_ind_t
{
    rrc_cell_index_t  cell_index;
}rrc_csc_uecc_cell_stop_ind_t;/*^ API ^*/ 

#pragma pack(pop)

#endif /* _RRC_CSC_UECC_API_INT_H_ */

