/*! \file son_rrm_intf.h
 *  \brief This file contains the structure and enumurations for NMM and RRC interface.
 *  \date January 02, 2012
 *  \author Komal Thareja
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _son_RRM_INTF_H_
#define _son_RRM_INTF_H_

/*! \headerfile son_oam_intf.h <>
 */
#include <son_oam_intf.h>

/* MRO Changes Start */

/*! \enum son_rrm_message_et
 *  \brief An enum that specifies the APIs exposed by SON towards RRM
 */
typedef enum
{
    SON_RRM_RES_API_BASE = 10500,
    SON_RRM_TTT_UPDATE_IND = SON_RRM_RES_API_BASE,

    /*This should always be the last value of enum*/
    SON_RRM_MAX_RES_API
}son_rrm_message_et;

/* MRO Changes End */

/*! \enum son_anr_rrm_message_et
 *  \brief An enum that specifies the APIs exposed by SON towards RRM
 */
typedef enum
{
    SON_ANR_RRM_REQ_API_BASE = 9000,
    SON_APPS_RRM_CARRIER_FREQ_AND_DL_POWER_PARAMS_REQ = SON_ANR_RRM_REQ_API_BASE,
    SON_RRM_TNL_DISCOVERY_REQ,
    /*This should always be the last value of enum*/
    SON_ANR_RRM_MAX_REQ_API,

    SON_ANR_RRM_RES_API_BASE = 9500,
    SON_ANR_RRM_UPDATED_NRT_INFO = SON_ANR_RRM_RES_API_BASE,
    SON_APPS_RRM_CARRIER_FREQ_AND_DL_POWER_PARAMS_RESP,
    SON_RRM_TNL_DISCOVERY_RES,
    SON_RRM_HO_REPORT,
    /* Spr 16142 Fix Start */
    SON_RRM_STOP_TNL_DISCOVERY_IND,
    /* Spr 16142 Fix End */
    /*This should always be the last value of enum*/
    SON_ANR_RRM_MAX_RES_API
}son_anr_rrm_message_et;

/*! \  son_rrm_cell_plmn_info_t
 *  \brief   Cell PLMN Info
 *  \param   mcc[SON_MCC_OCTET_SIZE]    Mobile Country Code
 *  \param   num_mnc_digit                This can be 2 or 3
 *  \param   mnc[SON_MAX_MNC_OCTET_SIZE]  Mobile Network Code
 */
typedef struct _son_rrm_cell_plmn_info_t
{
    son_u8 mcc[SON_MCC_OCTET_SIZE];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    son_u8 num_mnc_digit;
/*SPR_17664_start*/
    /*^ M, 0, B, 2, SON_MAX_MNC_OCTET_SIZE ^*/
/*SPR_17664_end*/
    son_u8 mnc[SON_MAX_MNC_OCTET_SIZE];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_rrm_cell_plmn_info_t;

/*! \   son_rrm_intra_rat_global_cell_id_t
 *  \brief Global Cell ID
 *  \param plmn_id          PLMN Id
 *  \param cell_id          Cell Identity
 */
typedef struct _son_rrm_intra_rat_global_cell_id_t
{
    son_rrm_cell_plmn_info_t plmn_id;
    /*^ M, 0, N, 0, 0 ^*/
    son_u8 cell_identity[SON_CELL_ID_OCTET_SIZE];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
}son_rrm_intra_rat_global_cell_id_t;

/*! \   son_rrm_utran_geran_global_cell_id_t
 *  \brief Global Cell ID
 *  \param plmn_id          PLMN Id
 *  \param cell_id          Cell Identity
 */
typedef struct _son_rrm_utran_geran_global_cell_id_t
{
    son_rat_type_et rat_type;      /*^ M, 0, N, 1, 3 ^*/
    son_rrm_cell_plmn_info_t plmn_id;
    /*^ M, 0, N, 0, 0 ^*/
    son_u8 cell_identity[SON_CELL_ID_OCTET_SIZE];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
}son_rrm_utran_geran_global_cell_id_t;

/*! \   son_rrm_others_global_cell_id_t
 *  \brief Global Cell ID
 *  \param plmn_id          PLMN Id
 *  \param cell_id          Cell Identity
 */
typedef struct _son_rrm_others_global_cell_id_t
{
    son_u32 bitmask; /* For Future use */
    /* SPR-612 Coverity Fix Starts */
    /* Commented line deleted */
    /* SPR-612 Coverity Fix Ends */
}son_rrm_others_global_cell_id_t;
#define SON_RRM_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT      0x01
#define SON_RRM_OTHERS_GLOBAL_CELL_ID_PRESENT           0x02

/*! \   son_rrm_inter_rat_global_cell_id_t
 *  \brief Global Cell ID for UTRAN_GERAN and for OTHERS
 *  \param bitmask
 *  \param son_rrm_utran_geran_global_cell_id_t  - Global Cell ID for UTRAN_GERAN
 *  \param son_rrm_others_global_cell_id_t  - Global Cell ID for OTHERS (e.g CDMA)
 */
typedef struct _son_rrm_inter_rat_global_cell_id_t
{
    son_u32 bitmask;   /*^ BITMASK ^*/
    son_rrm_utran_geran_global_cell_id_t utran_geran_global_cell_id;
    /*^ O, SON_RRM_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT, N, 0, 0 ^*/
    son_rrm_others_global_cell_id_t others_global_cell_id;
    /*^ O, SON_RRM_OTHERS_GLOBAL_CELL_ID_PRESENT, N, 0, 0 ^*/
}son_rrm_inter_rat_global_cell_id_t;

#define SON_RRM_INTRA_RAT_GLOBAL_CELL_ID_PRESENT            0x01
#define SON_RRM_INTER_RAT_GLOBAL_CELL_ID_PRESENT            0x02

/*! \   son_rrm_global_cell_id_t
 *  \brief Can have Global Cell ID of EUTRAN/UTRAN_GERAN/OTHERS cell types
 *  \param bitmask
 *  \param intra_rat_global_cell_id    -   Global cell id for eutran cell type
 *  \param inter_rat_global_cell_id    -   Global cell id for utran_getran and others cell type
 */
typedef struct _son_rrm_global_cell_id_t
{
    son_u32 bitmask;   /*^ BITMASK ^*/
    son_rrm_intra_rat_global_cell_id_t intra_rat_global_cell_id;
    /*^ O, SON_RRM_INTRA_RAT_GLOBAL_CELL_ID_PRESENT, N, 0, 0 ^*/
    son_rrm_inter_rat_global_cell_id_t inter_rat_global_cell_id;
    /*^ O, SON_RRM_INTER_RAT_GLOBAL_CELL_ID_PRESENT, N, 0, 0 ^*/
}son_rrm_global_cell_id_t;

/*SPR_5193*/
#define SON_RRM_INTRA_RAT_NR_INFO_OFFSET_PRESENT       0x01
#define SON_RRM_ENABLE_X2_STATUS_PRESENT               0x02
#define SON_RRM_TAC_PRESENT                            0x04
#define SON_RRM_INTRA_RAT_NR_INFO_CM_OFFSET_PRESENT    0x08
#define SON_RRM_ACCESS_MODE_PRESENT                    0x10
#define SON_RRM_BROADCAST_STATUS_PRESENT               0x20
#define SON_RRM_INTRA_CSG_IDENTITY_PRESENT             0x40
/*SPR 14209 start*/
#define SON_RRM_CELL_INTF_SCHEME_TYPE_PRESENT             0x80 
/*SPR 14209 end*/
/* SPR 22248 MBMS Changes Start */
#define SON_RRM_MBSFN_CONFIG_LIST_PRESENT               0x100
/*SPR 18605:Fix Start*/
#define SON_RRM_TDD_SF_ASSN_PRESENT                     0x200
/*SPR 18605:Fix Start*/
/* SPR 22248 MBMS Changes Stop */
/*! \  struct son_rrm_intra_rat_neighbor_cell_t
 *  \brief This is neighbor information sent to RRM
 *  \param cgi                      Neighbor cell CGI
 *  \param pci                      Physical cell id
 *  \param cell_specific_offset     Cell Specific Offset
 *  \param x2_status                Indicates whether X2 connection
 *                                  is available and can be used for
 *                                  HO or not with the corresponding eNB
 *  \param tac                      TAC value
 *  \param cm_cell_specific_offset  Connected Mode Cell Specific Offset
 *  \param access_mode              Indicates access mode of the neighbor
 *  \param broadcast_status         Indicates if the neighbor cell is to
 *                                  be broadcasted in SIB4 or SIB5.
 *  \param csg_identity             CSG Identity
 */
typedef struct _son_rrm_intra_rat_neighbor_cell_t
{
    son_u32 bitmask;   /*^ BITMASK ^*/
    son_rrm_intra_rat_global_cell_id_t cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_u16 pci;
    /*^ M, 0, H, 0, 503 ^*/
    son_q_offset_range_et cell_specific_offset;
    /*^ O, SON_RRM_INTRA_RAT_NR_INFO_OFFSET_PRESENT, H, 0, 30 ^*/
    /* Rel 3.0 Multi Sector code changes Start */
    son_x2_status_et x2_status;
    /*^ O, SON_RRM_ENABLE_X2_STATUS_PRESENT, B, 3, 7 ^*/
    /* Rel 3.0 Multi Sector code changes Stop */
    son_u8 tac[SON_TAC_OCTET_SIZE];
    /*^O,SON_RRM_TAC_PRESENT,OCTET_STRING, FIXED ^*/
    son_q_offset_range_et cm_cell_specific_offset;
    /*^ O, SON_RRM_INTRA_RAT_NR_INFO_CM_OFFSET_PRESENT, H, 0, 30 ^*/
    son_access_mode_et access_mode;
    /*^ O, SON_RRM_ACCESS_MODE_PRESENT, H, 0, 3 ^*/
    son_bool_et broadcast_status;
    /*^ O, SON_RRM_BROADCAST_STATUS_PRESENT, H, 0, 1 ^*/
    son_u8 csg_identity[SON_CSG_ID_OCTET_SIZE];
    /*^O,SON_RRM_INTRA_CSG_IDENTITY_PRESENT,OCTET_STRING, FIXED ^*/
    /*SPR 14209 start*/
    son_intf_scheme_type_et    interference_scheme;    /*^ O, SON_RRM_CELL_INTF_SCHEME_TYPE_PRESENT, H, 0, 2 ^*/
    /*SPR 14209 end*/
/* SPR 22248 MBMS Changes Start */
    son_mbms_configuration_t mbms_config; /*^ O, SON_RRM_MBSFN_CONFIG_LIST_PRESENT, N, 0, 0 ^*/
    //KB:ADD
/*SPR 18605:Fix Start*/
    son_u8                  subframe_assn ; /*^ O, SON_RRM_TDD_SF_ASSN_PRESENT, H, 0, 6 ^*/ 
/*SPR 18605:Fix End*/
/* SPR 22248 MBMS Changes Stop */
}son_rrm_intra_rat_neighbor_cell_t;
/*SPR_5193*/

/*! \  struct son_rrm_intra_rat_neighbor_list_t
 *  \brief  Structure containing neighbor cell list on a particular earfcn
 *  \param  earfcn          Carrier Frequency
 *  \param  cell_list_size  Cell List Size
 *  \param  cell_list       Cell List
 */
typedef struct _son_rrm_intra_rat_neighbor_list_t
{
    son_u32 earfcn;    /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u16 cell_list_size;  /*^ M, 0, H, 0, SON_RRM_MAX_INTRA_RAT_HO_ALLWED_BLKLISTED_NBRS_PER_EARFCN^*/
/*SPR_17664_end*/
    son_rrm_intra_rat_neighbor_cell_t cell_list[SON_RRM_MAX_INTRA_RAT_HO_ALLWED_BLKLISTED_NBRS_PER_EARFCN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_rrm_intra_rat_neighbor_list_t;

/*! \  struct son_rrm_intra_rat_neighbor_cell_list_t
 *  \brief Structure containing intra-rat neighbor cell list information
 *  \param  intra_rat_cells_to_add_mod_list_size                Size of list of added or modified neighbors
 *  \param  intra_rat_cells_to_add_mod_list                     List of added or modified neighbors
 *  \param  intra_rat_cells_to_remove_list_size                 Size of list of removed neighbors
 *  \param  intra_rat_cells_to_remove_list                      List of removed neighbors
 *  \param  intra_rat_black_list_cells_to_add_mod_list_size     Size of list of added or modified blacklisted neighbors
 *  \param  intra_rat_black_list_cells_to_add_mod_list          List of added or modified blacklisted neighbors
 *  \param  intra_rat_black_list_cells_to_remove_list_size      Size of list of removed  blacklisted neighbors
 *  \param  intra_rat_black_list_cells_to_remove_list           List of removed  blacklisted neighbors
 */
typedef struct _son_rrm_intra_rat_neighbor_cell_list_t
{
    son_u16 intra_rat_cells_to_add_mod_list_size;
/*SPR_17664_start*/
/*^ M, 0, H, 0,SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED ^*/
/*SPR_17664_end*/
    son_rrm_intra_rat_neighbor_list_t intra_rat_cells_to_add_mod_list[SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    son_u16 intra_rat_cells_to_remove_list_size;

/*SPR_17664_strat*/
    /*^ M, 0, H, 0,SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED ^*/
/*SPR_17664_end*/
    son_rrm_intra_rat_neighbor_list_t intra_rat_cells_to_remove_list[SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    son_u16 intra_rat_black_list_cells_to_add_mod_list_size;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED ^*/
/*SPR_17664_end*/
    son_rrm_intra_rat_neighbor_list_t intra_rat_black_list_cells_to_add_mod_list[SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    son_u16 intra_rat_black_list_cells_to_remove_list_size;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED ^*/
/*SPR_17664_end*/
    son_rrm_intra_rat_neighbor_list_t intra_rat_black_list_cells_to_remove_list[SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_rrm_intra_rat_neighbor_cell_list_t;

#define SON_RRM_PCI_FDD_PRESENT                  0x01
#define SON_RRM_PCI_TDD_PRESENT                  0x02

/*! \  son_rrm_utran_physical_cell_id_t
 *  \brief Physical Cell ID
 *  \param bitmask - Bitmask Value
 *  \param pci_fdd - PCI (FDD Mode)
 *  \param pci_tdd - PCI (TDD Mode)
 */
typedef struct _son_rrm_utran_physical_cell_id_t
{
    son_u32 bitmask;       /*^ BITMASK ^*/
    son_u16 pci_fdd;   /*^ O, SON_PCI_FDD_PRESENT, H, 0, 511 ^*/
    son_u8 pci_tdd;       /*^ O, SON_PCI_TDD_PRESENT, H, 0, 127 ^*/
}son_rrm_utran_physical_cell_id_t;

#define SON_RRM_UTRAN_NR_INFO_OFFSET_PRESENT    0x01
#define SON_RRM_UTRAN_ACCESS_MODE_PRESENT       0x02
#define SON_RRM_UTRAN_LAC_PRESENT               0x04
#define SON_RRM_UTRAN_RAC_PRESENT               0x08
#define SON_RRM_UTRAN_CSG_ID_PRESENT            0x10
#define SON_RRM_UTRAN_PS_HO_SUPPORT_PRESENT     0x20
#define SON_RRM_UTRAN_VOIP_CAPABLE_PRESENT      0x40

/*! \  struct son_rrm_utran_neighbor_cell_t
 *  \brief This is neighbor information sent to RRM
 *  \param cgi                    Neighbor cell CGI
 *  \param pci                    Physical cell id
 *  \param cell_specific_offset   Cell Specific Offset
 *  \param access_mode            Indicates access mode
 *                                of the neighbor
 *  \param lac                    LAC value
 *  \param rac                    RAC
 *  \param csg_identity           CSG Identity
 *  \param ps_ho_supported        PS HO Support Value(Added for SRVCC)
 *  \param voip_capable           Voip Capable Value(Added for SRVCC)
 */
typedef struct _son_rrm_utran_neighbor_cell_t
{
    son_u32 bitmask;   /*^ BITMASK ^*/
    son_rrm_utran_geran_global_cell_id_t cgi; /*^ M, 0, N, 0, 0 ^*/
    son_rrm_utran_physical_cell_id_t pci;
    /*^ M, 0, N, 0, 0 ^*/
    son_s8 cell_specific_offset;
    /*^ O, SON_RRM_UTRAN_NR_INFO_OFFSET_PRESENT, B, -15, 15 ^*/
    son_access_mode_et access_mode;
    /*^ O, SON_RRM_UTRAN_ACCESS_MODE_PRESENT, H,0 ,3 ^*/
    son_u8 lac[SON_LAC_OCTET_SIZE];
    /*^O,SON_RRM_UTRAN_LAC_PRESENT,OCTET_STRING, FIXED ^*/
    son_u8 rac;
    /*^ O,SON_RRM_UTRAN_RAC_PRESENT, N, 0, 0 ^*/
    son_u8 csg_identity[SON_CSG_ID_OCTET_SIZE];
    /*^ O,SON_RRM_UTRAN_CSG_ID_PRESENT, OCTET_STRING, FIXED ^*/
    son_bool_et                 ps_ho_supported;
    /*^ O, SON_RRM_UTRAN_PS_HO_SUPPORT_PRESENT, H, 0, 1 ^*/
    son_bool_et                 voip_capable;
    /*^ O, SON_RRM_UTRAN_VOIP_CAPABLE_PRESENT, H, 0, 1 ^*/
}son_rrm_utran_neighbor_cell_t;

/*! \  struct son_rrm_utran_neighbor_list_t
 *  \brief  Structure containing neighbor cell list on a particular earfcn
 *  \param  uarfcn          Carrier Frequency
 *  \param  cell_list_size  Cell List Size
 *  \param  cell_list       Cell List
 */
typedef struct _son_rrm_utran_neighbor_list_t
{
    son_u16 uarfcn;    /*^ M, 0, H, 0, 16383 ^*/
/*SPR_17664_start*/
    son_u16 cell_list_size;  /*^ M, 0, H, 0, SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN ^*/
/*SPR_17664_end*/
    son_rrm_utran_neighbor_cell_t cell_list[SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_rrm_utran_neighbor_list_t;

/*! \  struct son_rrm_utran_neighbor_cell_list_t
 *  \brief Structure containing utran neighbor cell list information
 *  \param  cells_to_add_mod_list_size                Size of list of added or modified neighbors
 *  \param  cells_to_add_mod_list                     List of added or modified neighbors
 *  \param  cells_to_remove_list_size                 Size of list of removed neighbors
 *  \param  cells_to_remove_list                      List of removed neighbors
 *  \param  black_list_cells_to_add_mod_list_size     Size of list of added or modified blacklisted neighbors
 *  \param  black_list_cells_to_add_mod_list          List of added or modified blacklisted neighbors
 *  \param  black_list_cells_to_remove_list_size      Size of list of removed  blacklisted neighbors
 *  \param  black_list_cells_to_remove_list           List of removed  blacklisted neighbors
 */
typedef struct _son_rrm_utran_neighbor_cell_list_t
{
    son_u16 cells_to_add_mod_list_size;
/* SPR-18445 start */
    /*^ M, 0, H, 0, SON_RRM_MAX_UTRAN_FREQ_ALLOWED ^*/
    son_rrm_utran_neighbor_list_t cells_to_add_mod_list[SON_RRM_MAX_UTRAN_FREQ_ALLOWED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    son_u16 cells_to_remove_list_size;
    /*^ M, 0, H, 0, SON_RRM_MAX_UTRAN_FREQ_ALLOWED ^*/
    son_rrm_utran_neighbor_list_t cells_to_remove_list[SON_RRM_MAX_UTRAN_FREQ_ALLOWED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    son_u16 black_list_cells_to_add_mod_list_size;
    /*^ M, 0, H, 0, SON_RRM_MAX_UTRAN_FREQ_ALLOWED ^*/
    son_rrm_utran_neighbor_list_t black_list_cells_to_add_mod_list[SON_RRM_MAX_UTRAN_FREQ_ALLOWED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    son_u16 black_list_cells_to_remove_list_size;
    /*^ M, 0, H, 0, SON_RRM_MAX_UTRAN_FREQ_ALLOWED ^*/
/* SPR-18445 stop */
    son_rrm_utran_neighbor_list_t black_list_cells_to_remove_list[SON_RRM_MAX_UTRAN_FREQ_ALLOWED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_rrm_utran_neighbor_cell_list_t;

/*! \  son_rrm_geran_physical_cell_id_t
 *  \brief This is GERAN pci inforamtion
 *  \param ncc                  Network Colour Code def in 23.003
 *  \param bscc                 Base station Colour Code def in 23.003
 */
typedef struct _son_rrm_geran_physical_cell_id_t
{
    son_u8 ncc;
    /*^ M, 0, H, 0, 7 ^*/
    son_u8 bscc;
    /*^ M, 0, H, 0, 7 ^*/
}son_rrm_geran_physical_cell_id_t;

#define SON_RRM_GERAN_NR_INFO_OFFSET_PRESENT    0x01
#define SON_RRM_GERAN_NR_DTM_SUPPORT_PRESENT    0x02
/*SPR 15393 Fix Start*/
#define SON_RRM_GERAN_RAC_PRESENT               0x04
/*SPR 15393 Fix Stop*/

/*! \  struct son_rrm_geran_neighbor_cell_t
 *  \brief This is neighbor information sent to RRM
 *  \param cgi                    neighbor cell CGI
 *  \param pci                    Physical cell id
 *  \param cell_specific_offset   Cell Specific Offset
 *  \param dtm_supported          DTM Support(Added for SPR-11121)
 */
typedef struct _son_rrm_geran_neighbor_cell_t
{
    son_u32 bitmask;   /*^ BITMASK ^*/
    son_rrm_utran_geran_global_cell_id_t cgi;
    /*^ M, 0, N, 0, 0 ^*/

    son_rrm_geran_physical_cell_id_t pci;
    /*^ M, 0, N, 0, 0 ^*/
    son_s8 cell_specific_offset;
    /*^ O, SON_RRM_GERAN_NR_INFO_OFFSET_PRESENT, B, -15, 15 ^*/

    son_u8 dtm_supported;
    /*^ O, SON_RRM_GERAN_NR_DTM_SUPPORT_PRESENT, H, 0, 1 ^*/
    /*SPR 15393 Fix Start*/
    son_u8                           rac;
    /*^ O, SON_RRM_GERAN_RAC_PRESENT, N, 0, 0 ^*/
   /*SPR 15393 Fix Stop*/
}son_rrm_geran_neighbor_cell_t;

/*! \  struct son_rrm_geran_neighbor_list_t
 *  \brief  Structure containing neighbor cell list on a particular earfcn
 *  \param  uarfcn          Carrier Frequency
 *  \param  cell_list_size  Cell List Size
 *  \param  cell_list       Cell List
 */
typedef struct _son_rrm_geran_neighbor_list_t
{
    son_u16 arfcn;
    /*^ M, 0, H, 0, 1023 ^*/
    son_geran_band_indicator_et band_ind;
    /*^ M, 0, H, 0, 1 ^*/
/*SPR_17664_start*/
    son_u16 cell_list_size;  /*^ M, 0, H, 0, SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN^*/
/*SPR_17664_end*/
    son_rrm_geran_neighbor_cell_t cell_list[SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_rrm_geran_neighbor_list_t;

#define SON_CELL_GLOBAL_ID_1X_RTT_SIZE   6
#define SON_CELL_GLOBAL_ID_HRPD_SIZE     16
#define SON_RRM_CGI_1X_RTT_PRESENT 0x01
#define SON_RRM_CGI_HRPD_PRESENT 0x02

/*! \  struct son_rrm_cdma_global_cell_id_t
 *  \brief  Structure containing CDMA Global Cell Id
 *  \param  bitmask                 Bitmaksk Value
 *  \param  cell_global_id_1X_RTT   CDMA 1X_RTT global cell id
 *  \param  cell_global_id_HRPD     CDMA HRPD global cell id
 */
typedef struct _son_rrm_cdma_global_cell_id_t
{
    son_u32 bitmask; /*^ BITMASK ^*/
    son_u8 cell_global_id_1X_RTT [SON_CELL_GLOBAL_ID_1X_RTT_SIZE];
    /*^ O, SON_RRM_CGI_1X_RTT_PRESENT, OCTET_STRING, FIXED ^*/
    son_u8 cell_global_id_HRPD [SON_CELL_GLOBAL_ID_HRPD_SIZE];
    /*^ O, SON_RRM_CGI_HRPD_PRESENT, OCTET_STRING, FIXED ^*/
}son_rrm_cdma_global_cell_id_t;

#define SON_RRM_CDMA_PZ_HYST_ENABLED_PRESENT     0x01
#define SON_RRM_CDMA_PZ_HYST_INFO_INCL_PRESENT   0x02
#define SON_RRM_CDMA_PZ_HYST_TIMER_PRESENT       0x04
#define SON_RRM_CDMA_PZ_HYST_TIMER_MUL_PRESENT   0x08
#define SON_RRM_CDMA_PZ_HYST_TIMER_EXP_PRESENT   0x10
#define SON_RRM_CDMA_PZ_HYST_LIST_LEN_PRESENT    0x20

/*! \ son_cdma_pz_hyst_params_included_t
 *  \brief CDMA Hysteresis Feature
 *  \param bitmask                  - Bitmask value
 *  \param pz_hyst_enabled          - Hysteresis feature to be enabled
 *  \param pz_hyst_info_incl        - Hysteresis information included
 *  \param pz_hyst_act_timer        - Hysteresis activity timer
 *  \param pz_hyst_timer_mul        - Hysteresis timer multiplier
 *  \param pz_hyst_timer_exp        - Hysteresis timer exponent
 *  \param pz_hyst_list_len         - Hysteresis list length
 */
typedef struct _son_cdma_pz_hyst_params_included_t
{
    son_u32 bitmask;
    /*^BITMASK ^*/
    son_u8 pz_hyst_enabled;
    /*^ O, SON_RRM_CDMA_PZ_HYST_ENABLED_PRESENT, H, 0, 1 ^*/
    son_u8 pz_hyst_info_incl;
    /*^ O, SON_RRM_CDMA_PZ_HYST_INFO_INCL_PRESENT, H, 0, 1 ^*/
    son_u8 pz_hyst_act_timer;
    /*^ O, SON_RRM_CDMA_PZ_HYST_TIMER_PRESENT, N, 0, 0 ^*/
    son_u8 pz_hyst_timer_mul;
    /*^ O, SON_RRM_CDMA_PZ_HYST_TIMER_MUL_PRESENT, H, 0, 7 ^*/
    son_u8 pz_hyst_timer_exp;
    /*^ O, SON_RRM_CDMA_PZ_HYST_TIMER_EXP_PRESENT, H, 0, 31 ^*/
    son_u8 pz_hyst_list_len;
    /*^ O, SON_RRM_CDMA_PZ_HYST_LIST_LEN_PRESENT, H, 0, 1 ^*/
}son_cdma_pz_hyst_params_included_t;

#define SON_RRM_CDMA_FPC_FCH_INCL_RC3_PRESENT    0x01
#define SON_RRM_CDMA_FPC_FCH_INCL_RC4_PRESENT    0x02
#define SON_RRM_CDMA_FPC_FCH_INCL_RC5_PRESENT    0x04

/*! \ son_cdma_fpc_fch_included_t
 *  \brief CDMA Fundamental channels params
 *  \param bitmask                  - Bitmask value
 *  \param fpc_fch_init_setpt_rc3   - Initial Fundamental channel RC3
 *  \param fpc_fch_init_setpt_rc4   - Initial Fundamental channel RC4
 *  \param fpc_fch_init_setpt_rc5   - Initial Fundamental channel RC5
 */
typedef struct _son_cdma_fpc_fch_included_t
{
    son_u32 bitmask;
    /*^ BITMASK ^*/
    son_u8 fpc_fch_init_setpt_rc3;
    /*^ O, SON_RRM_CDMA_FPC_FCH_INCL_RC3_PRESENT, N, 0, 0 ^*/
    son_u8 fpc_fch_init_setpt_rc4;
    /*^ O, SON_RRM_CDMA_FPC_FCH_INCL_RC4_PRESENT, N, 0, 0 ^*/
    son_u8 fpc_fch_init_setpt_rc5;
    /*^ O, SON_RRM_CDMA_FPC_FCH_INCL_RC5_PRESENT, N, 0, 0 ^*/
}son_cdma_fpc_fch_included_t;

#define SON_RRM_CDMA_CSP_PZ_HYST_PARAMS_INCLUDED_PRESENT    0x01
#define SON_RRM_CDMA_CSP_P_REV_PRESENT                      0x02
#define SON_RRM_CDMA_CSP_MIN_P_REV_PRESENT                  0x04
#define SON_RRM_CDMA_CSP_NEG_SLOT_CYCLE_INDEX_PRESENT       0x08
#define SON_RRM_CDMA_CSP_ENCRYPT_MODE_PRESENT               0x10
#define SON_RRM_CDMA_CSP_ENCRYPT_SUPPORTED_PRESENT          0x20
#define SON_RRM_CDMA_CSP_SYG_ENCRYPT_SUPPORTED_PRESENT      0x40
#define SON_RRM_CDMA_CSP_MSG_INT_SUPPORTED_PRESENT          0x80
#define SON_RRM_CDMA_CSP_SIG_INT_SUPPORTED_INCL_PRESENT     0x100
#define SON_RRM_CDMA_CSP_SIG_INT_SUPPORTED_PRESENT          0x200
#define SON_RRM_CDMA_CSP_MS_INIT_POS_LOC_SUP_IND_PRESENT    0x400
#define SON_RRM_CDMA_CSP_BAND_CLASS_INFO_REQ_PRESENT        0x800
#define SON_RRM_CDMA_CSP_BAND_CLASS_PRESENT                 0x1000
#define SON_RRM_CDMA_CSP_ALT_BAND_CLASS_PRESENT             0x2000
#define SON_RRM_CDMA_CSP_TKZ_MODE_SUP_PRESENT               0x4000
#define SON_RRM_CDMA_CSP_TKZ_ID_PRESENT                     0x8000
#define SON_RRM_CDMA_CSP_FPC_FCH_INCLUDED                   0x10000
#define SON_RRM_CDMA_CSP_T_ADD                              0x20000
#define SON_RRM_CDMA_CSP_PILOT_INC_PRESENT                  0x40000

/*! \  son_rrm_cdma_cell_specific_params_t
 *  \brief CDMA Cell specific params
 *  \param bitmask                  - Bitmask value
 *  \param pz_hyst_params_included  - Hysteresis params included
 *  \param p_rev                    - Protocol revision level
 *  \param min_p_rev                - Minimum mobile station protocol revision level
 *  \param neg_slot_cycle_index_sup - Set to 1, if it supports negative values of the preferred slot cycle index
 *  \param encrypt_mode             - Message encryption mode(00 - Encryption disabled,
 *                                    01 - Basic encryption of call control messages,
 *                                    10 - Enhanced encryption of call control messages,
 *                                    11 - Extended encryption of call control messages)
 *  \param encrypt_suported         - Set to 1 if the encryption related fields are included
 *  \param sig_encrypt_suported     - Indicates which signaling encryption algorithms are supported by the
 *                                    base station. If ENC_SUPPORTED is equal to 1, the base station shall
 *                                    include this field
 *  \param msg_integrity_sup        - If the base station supports message integrity, the base station
 *                                    shall set this field to 1
 *  \param sig_integrity_sup_incl   - If the base station supports other integrity algorithm(s)
 *                                    in addition to the default integrity algorithm, the base station
 *                                    shall set this field to 1
 *  \param sig_integrity_sup        - Indicates the supported message integrity algorithms in
 *                                    addition to the default integrity algorithm
 *  \param ms_init_pos_loc_sup_ind  - If the base station supports mobile station initiated position
 *                                    determination, the base station shall set this field to 1
 *  \param band_class_info_req      - The base station shall set this field to 1 if the ALT_BAND_CLASS
 *                                    field is included in this message
 *  \param band_class               - CDMA2000 band class
 *  \param alt_band_class           - The base station shall set this field to an alternate
 *                                    CDMA band class supported by the base station
 *  \param tkz_mode_supported       - The base station shall set this field to 1 if the tracking
 *                                    zone mode is supported
 *  \param tkz_id                   - If TKZ_MODE_SUPPORTED is set to 1, the base station shall
 *                                    set this field to its tracking zone identifier
 *  \param fpc_fch_included         - Fundamental channel included
 *  \param t_add                    - This value is used by the mobile station to trigger the
 *                                    transfer of a pilot from the Neighbor Set or Remaining Set
 *                                    to the Candidate Set and to trigger the sending of the
 *                                    Pilot Strength Measurement Message or Extended Pilot Strength
 *                                    Measurement Message initiating the handoff process
 *  \param pilot_inc                - A mobile station searches for Remaining-Set pilots at
 *                                    pilot PN sequence index values that are multiples of this value
 */
typedef struct _son_rrm_cdma_cell_specific_params_t
{
    son_u32 bitmask;
    /*^ BITMASK ^*/
    son_cdma_pz_hyst_params_included_t pz_hyst_params_included;
    /*^ O, SON_RRM_CDMA_CSP_PZ_HYST_PARAMS_INCLUDED_PRESENT, N, 0, 0 ^*/
    son_u8 p_rev;
    /*^ O, SON_RRM_CDMA_CSP_P_REV_PRESENT, N, 0, 0 ^*/
    son_u8 min_p_rev;
    /*^ O, SON_RRM_CDMA_CSP_MIN_P_REV_PRESENT, N, 0, 0 ^*/
    son_u8 neg_slot_cycle_index_sup;
    /*^ O, SON_RRM_CDMA_CSP_NEG_SLOT_CYCLE_INDEX_PRESENT, H, 0, 1 ^*/
    son_u8 encrypt_mode;
    /*^ O, SON_RRM_CDMA_CSP_ENCRYPT_MODE_PRESENT, N, 0, 0 ^*/
    son_u8 encrypt_suported;
    /*^ O, SON_RRM_CDMA_CSP_ENCRYPT_SUPPORTED_PRESENT, H, 0, 1 ^*/
    son_u8 syg_encrypt_suported;
    /*^ O, SON_RRM_CDMA_CSP_SYG_ENCRYPT_SUPPORTED_PRESENT, N, 0, 0 ^*/
    son_u8 msg_integrity_sup;
    /*^ O, SON_RRM_CDMA_CSP_MSG_INT_SUPPORTED_PRESENT, H, 0, 1 ^*/
    son_u8 sig_integrity_sup_incl;
    /*^ O, SON_RRM_CDMA_CSP_SIG_INT_SUPPORTED_INCL_PRESENT, H, 0, 1 ^*/
    son_u8 sig_integrity_sup;
    /*^ O, SON_RRM_CDMA_CSP_SIG_INT_SUPPORTED_PRESENT, N, 0, 0 ^*/
    son_u8 ms_init_pos_loc_sup_ind;
    /*^ O, SON_RRM_CDMA_CSP_MS_INIT_POS_LOC_SUP_IND_PRESENT, H, 0, 1 ^*/
    son_u8 band_class_info_req;
    /*^ O, SON_RRM_CDMA_CSP_BAND_CLASS_INFO_REQ_PRESENT, H, 0, 1 ^*/
    son_cdma_band_class_et band_class;
    /*^ O, SON_RRM_CDMA_CSP_BAND_CLASS_PRESENT, H, 0, 31 ^*/
    son_cdma_band_class_et alt_band_class;
    /*^ O, SON_RRM_CDMA_CSP_ALT_BAND_CLASS_PRESENT, H, 0, 31^*/
    son_u8 tkz_mode_supported;
    /*^ O, SON_RRM_CDMA_CSP_TKZ_MODE_SUP_PRESENT, H, 0, 1 ^*/
    son_u8 tkz_id;
    /*^ O, SON_RRM_CDMA_CSP_TKZ_ID_PRESENT, H, 0, 1 ^*/
    son_cdma_fpc_fch_included_t fpc_fch_included;
    /*^ O, SON_RRM_CDMA_CSP_FPC_FCH_INCLUDED, N, 0, 0 ^*/
    son_u8 t_add;
    /*^ O, SON_RRM_CDMA_CSP_T_ADD, H, 0, 63 ^*/
    son_u8 pilot_inc;
    /*^ O, SON_RRM_CDMA_CSP_PILOT_INC_PRESENT, H, 0, 15 ^*/
}son_rrm_cdma_cell_specific_params_t;

#define SON_RRM_CDMA_HRPD_CELL_SPECIFIC_OFFSET_PRESENT  0x01
#define SON_RRM_CDMA_HRPD_CELL_SPECIFIC_PARAMS_PRESENT  0x02

/*! \ son_rrm_cdma_neighbor_cell_hrpd_t
 *  \brief CDMA HRPD Neighbor Cell
 *  \param bitmask              - Bitmask value
 *  \param pci                  - Physical Cell Id
 *  \param cgi                  - Neighbor Cell CGI
 *  \param arfcn                - This field indicates the CDMA2000 carrier frequency within a CDMA2000 band
 *  \param cell_specific_offset - Cell Specific Offset
 *  \param cell_specific_params - Indicates CDMA cell specific params
 */
typedef struct _son_rrm_cdma_neighbor_cell_hrpd_t
{
    son_u32 bitmask;  /*^ BITMASK ^*/
    son_u16 pci;
    /*^ M, 0, N, 0, 0 ^*/
    son_rrm_cdma_global_cell_id_t cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_u16 arfcn;
    /*^ M, 0, N, 0, 0 ^*/
    son_s8 cell_specific_offset;
    /*^ O, SON_RRM_CDMA_HRPD_CELL_SPECIFIC_OFFSET_PRESENT, B, -15, 15 ^*/
    son_rrm_cdma_cell_specific_params_t cell_specific_params;
    /*^ O, SON_RRM_CDMA_HRPD_CELL_SPECIFIC_PARAMS_PRESENT, N, 0, 0 ^*/
}son_rrm_cdma_neighbor_cell_hrpd_t;

#define SON_RRM_CDMA_HRPD_NBR_SIZE      512

/*! \ son_rrm_cdma_neighbor_cell_hrpd_t
 *  \brief CDMA HRPD Neighbor Cell list
 *  \param band_class       - CDMA band class
 *  \param cell_list_size   - CDMA Neighbor cell list size
 *  \param cell_list        - CDMA Neighbor cell list
 */
typedef struct _son_rrm_cdma_neighbor_list_hrpd_t
{
    son_u16 band_class;
    /*^ M, 0, H, 0, 31 ^*/
    son_u16 cell_list_size;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_RRM_CDMA_HRPD_NBR_SIZE ^*/
/*SPR_17664_end*/
    son_rrm_cdma_neighbor_cell_hrpd_t cell_list[SON_RRM_CDMA_HRPD_NBR_SIZE];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_rrm_cdma_neighbor_list_hrpd_t;

#define SON_RRM_CDMA_XRTT_CELL_SPECIFIC_OFFSET_PRESENT  0x01
#define SON_RRM_CDMA_XRTT_CELL_SPECIFIC_PARAMS_PRESENT  0x02

/*! \  son_rrm_cdma_neighbor_cell_xrtt_t
 *  \brief CDMA XRTT Neighbor Cell
 *  \param bitmask              - Bitmask value
 *  \param pci                  - Physical Cell Id
 *  \param cgi                  - Neighbor Cell CGI
 *  \param arfcn                - This field indicates the CDMA2000 carrier frequency within a CDMA2000 band
 *  \param cell_specific_offset - Cell Specific Offset
 *  \param cell_specific_params - Indicates CDMA cell specific params
 */
typedef struct _son_rrm_cdma_neighbor_cell_xrtt_t
{
    son_u32 bitmask;  /*^ BITMASK ^*/
    son_u16 pci;
    /*^ M, 0, N, 0, 0 ^*/
    son_rrm_cdma_global_cell_id_t cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_u16 arfcn;
    /*^ M, 0, N, 0, 0 ^*/
    son_s8 cell_specific_offset;
    /*^ O, SON_RRM_CDMA_XRTT_CELL_SPECIFIC_OFFSET_PRESENT, B, -15, 15 ^*/
    son_rrm_cdma_cell_specific_params_t cell_specific_params;
    /*^ O, SON_RRM_CDMA_XRTT_CELL_SPECIFIC_PARAMS_PRESENT, N, 0, 0 ^*/
}son_rrm_cdma_neighbor_cell_xrtt_t;

#define SON_RRM_CDMA_XRTT_NBR_SIZE      640

/*! \  son_rrm_cdma_neighbor_list_xrtt_t
 *  \brief CDMA XRTT Neighbor Cell list
 *  \param band_class       - CDMA band class
 *  \param cell_list_size   - CDMA Neighbor cell list size
 *  \param cell_list        - CDMA Neighbor cell list
 */
typedef struct _son_rrm_cdma_neighbor_list_xrtt_t
{
    son_u16 band_class;
    /*^ M, 0, H, 0, 31 ^*/
    son_u16 cell_list_size;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_RRM_CDMA_XRTT_NBR_SIZE ^*/
/*SPR_17664_end*/
    son_rrm_cdma_neighbor_cell_xrtt_t cell_list[SON_RRM_CDMA_XRTT_NBR_SIZE];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_rrm_cdma_neighbor_list_xrtt_t;

/*! \  struct son_rrm_geran_neighbor_cell_list_t
 *  \brief Structure containing geran neighbor cell list information
 *  \param  cells_to_add_mod_list_size                Size of list of added or modified neighbors
 *  \param  cells_to_add_mod_list                     List of added or modified neighbors
 *  \param  cells_to_remove_list_size                 Size of list of removed neighbors
 *  \param  cells_to_remove_list                      List of removed neighbors
 *  \param  black_list_cells_to_add_mod_list_size     Size of list of added or modified blacklisted neighbors
 *  \param  black_list_cells_to_add_mod_list          List of added or modified blacklisted neighbors
 *  \param  black_list_cells_to_remove_list_size      Size of list of removed  blacklisted neighbors
 *  \param  black_list_cells_to_remove_list           List of removed  blacklisted neighbors
 */
typedef struct _son_rrm_geran_neighbor_cell_list_t
{
    son_u16 cells_to_add_mod_list_size;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_RRM_MAX_GERAN_FREQ_ALLOWED ^*/
/*SPR_17664_end*/
    son_rrm_geran_neighbor_list_t cells_to_add_mod_list[SON_RRM_MAX_GERAN_FREQ_ALLOWED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    son_u16 cells_to_remove_list_size;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_RRM_MAX_GERAN_FREQ_ALLOWED ^*/
/*SPR_17664_end*/
    son_rrm_geran_neighbor_list_t cells_to_remove_list[SON_RRM_MAX_GERAN_FREQ_ALLOWED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    son_u16 black_list_cells_to_add_mod_list_size;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_RRM_MAX_GERAN_FREQ_ALLOWED ^*/
/*SPR_17664_end*/
    son_rrm_geran_neighbor_list_t black_list_cells_to_add_mod_list[SON_RRM_MAX_GERAN_FREQ_ALLOWED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    son_u16 black_list_cells_to_remove_list_size;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_RRM_MAX_GERAN_FREQ_ALLOWED ^*/
/*SPR_17664_end*/
    son_rrm_geran_neighbor_list_t black_list_cells_to_remove_list[SON_RRM_MAX_GERAN_FREQ_ALLOWED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_rrm_geran_neighbor_cell_list_t;

/*! \  struct son_rrm_cdma_neighbor_cell_list_hrpd_t
 *  \brief Structure containing cdma hrpd neighbor cell list information
 *  \param  cells_to_add_mod_list_size                Size of list of added or modified neighbors
 *  \param  cells_to_add_mod_list                     List of added or modified neighbors
 *  \param  cells_to_remove_list_size                 Size of list of removed neighbors
 *  \param  cells_to_remove_list                      List of removed neighbors
 *  \param  black_list_cells_to_add_mod_list_size     Size of list of added or modified blacklisted neighbors
 *  \param  black_list_cells_to_add_mod_list          List of added or modified blacklisted neighbors
 *  \param  black_list_cells_to_remove_list_size      Size of list of removed  blacklisted neighbors
 *  \param  black_list_cells_to_remove_list           List of removed  blacklisted neighbors
 */
typedef struct _son_rrm_cdma_neighbor_cell_list_hrpd_t
{
    son_u16 cells_to_add_mod_list_size;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_RRM_MAX_CDMA_BAND_ALLOWED ^*/
/*SPR_17664_end*/
    son_rrm_cdma_neighbor_list_hrpd_t cells_to_add_mod_list[SON_RRM_MAX_CDMA_BAND_ALLOWED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    son_u16 cells_to_remove_list_size;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_RRM_MAX_CDMA_BAND_ALLOWED ^*/
/*SPR_17664_end*/
    son_rrm_cdma_neighbor_list_hrpd_t cells_to_remove_list[SON_RRM_MAX_CDMA_BAND_ALLOWED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    son_u16 black_list_cells_to_add_mod_list_size;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_RRM_MAX_CDMA_BAND_ALLOWED ^*/
/*SPR_17664_end*/
    son_rrm_cdma_neighbor_list_hrpd_t black_list_cells_to_add_mod_list[SON_RRM_MAX_CDMA_BAND_ALLOWED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    son_u16 black_list_cells_to_remove_list_size;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_RRM_MAX_CDMA_BAND_ALLOWED ^*/
/*SPR_17664_end*/
    son_rrm_cdma_neighbor_list_hrpd_t black_list_cells_to_remove_list[SON_RRM_MAX_CDMA_BAND_ALLOWED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_rrm_cdma_neighbor_cell_list_hrpd_t;

/*! \  struct son_rrm_cdma_neighbor_cell_list_xrtt_t
 *  \brief Structure containing cdma xrtt neighbor cell list information
 *  \param  cells_to_add_mod_list_size                Size of list of added or modified neighbors
 *  \param  cells_to_add_mod_list                     List of added or modified neighbors
 *  \param  cells_to_remove_list_size                 Size of list of removed neighbors
 *  \param  cells_to_remove_list                      List of removed neighbors
 *  \param  black_list_cells_to_add_mod_list_size     Size of list of added or modified blacklisted neighbors
 *  \param  black_list_cells_to_add_mod_list          List of added or modified blacklisted neighbors
 *  \param  black_list_cells_to_remove_list_size      Size of list of removed  blacklisted neighbors
 *  \param  black_list_cells_to_remove_list           List of removed  blacklisted neighbors
 */
typedef struct _son_rrm_cdma_neighbor_cell_list_xrtt_t
{
    son_u16 cells_to_add_mod_list_size;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_RRM_MAX_CDMA_BAND_ALLOWED ^*/
/*SPR_17664_end*/
    son_rrm_cdma_neighbor_list_xrtt_t cells_to_add_mod_list[SON_RRM_MAX_CDMA_BAND_ALLOWED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    son_u16 cells_to_remove_list_size;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_RRM_MAX_CDMA_BAND_ALLOWED ^*/
/*SPR_17664_end*/
    son_rrm_cdma_neighbor_list_xrtt_t cells_to_remove_list[SON_RRM_MAX_CDMA_BAND_ALLOWED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    son_u16 black_list_cells_to_add_mod_list_size;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_RRM_MAX_CDMA_BAND_ALLOWED ^*/
    son_rrm_cdma_neighbor_list_xrtt_t black_list_cells_to_add_mod_list[SON_RRM_MAX_CDMA_BAND_ALLOWED];
/*SPR_17664_end*/
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    son_u16 black_list_cells_to_remove_list_size;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_RRM_MAX_CDMA_BAND_ALLOWED ^*/
/*SPR_17664_end*/
    son_rrm_cdma_neighbor_list_xrtt_t black_list_cells_to_remove_list[SON_RRM_MAX_CDMA_BAND_ALLOWED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_rrm_cdma_neighbor_cell_list_xrtt_t;

#define SON_RRM_UTRAN_NR_LIST_PRESENT       0x01
#define SON_RRM_GERAN_NR_LIST_PRESENT       0x02
#define SON_RRM_CDMA_NR_LIST_XRTT_PRESENT   0x04
#define SON_RRM_CDMA_NR_LIST_HRPD_PRESENT   0x08

/*! \  struct son_rrm_inter_rat_neighbor_cell_list_t
 *  \brief  Structure containing inter-rat neighbor cell list information
 */
typedef struct _son_rrm_inter_rat_neighbor_cell_list_t
{
    son_u32 bitmask; /*^ BITMASK ^*/
    son_rrm_utran_neighbor_cell_list_t utran_neighbor_list;
    /*^ O, SON_RRM_UTRAN_NR_LIST_PRESENT, N, 0, 0 ^*/
    son_rrm_geran_neighbor_cell_list_t geran_neighbor_list;
    /*^ O, SON_RRM_GERAN_NR_LIST_PRESENT, N, 0, 0 ^*/
    son_rrm_cdma_neighbor_cell_list_xrtt_t cdma_neighbor_list_xrtt;
    /*^ O, SON_RRM_CDMA_NR_LIST_XRTT_PRESENT, N, 0, 0 ^*/
    son_rrm_cdma_neighbor_cell_list_hrpd_t cdma_neighbor_list_hrpd;
    /*^ O, SON_RRM_CDMA_NR_LIST_HRPD_PRESENT, N, 0, 0 ^*/
}son_rrm_inter_rat_neighbor_cell_list_t;

/*
 * SON_ANR_RRM_UPDATED_NRT_INFO
 */

/*! \  son_anr_rrm_updated_nrt_info_t
 *  \brief It gives updated Neighbour Relation Table information.
 *  \param src_cgi          CGI of source cell
 *  \param nrt_op              NRT Operation performed on the neighbors contained in the message
 *  \param nr_list          Updated list of neighbours and its attribute
 *                          values of source cell id.
 *              Max list size is 255.
 */
typedef struct _son_anr_rrm_updated_nrt_info_t
{
    son_rrm_intra_rat_global_cell_id_t src_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_rrm_intra_rat_neighbor_cell_list_t intra_rat_ncl;
    /*^ M, 0, N, 0, 0 ^*/
    son_rrm_inter_rat_neighbor_cell_list_t inter_rat_ncl;
    /*^ M, 0, N, 0, 0 ^*/
}son_anr_rrm_updated_nrt_info_t;    /*^ API, SON_ANR_RRM_UPDATED_NRT_INFO ^*/

/*
 *  SON_APPS_RRM_CARRIER_FREQ_AND_DL_POWER_PARAMS_REQ
 */

/*! \   son_apps_carrier_freq_and_dl_power_params_req_t
 *  \brief    Request from RRM to select least interference EARFCN
 *            and DL power params from the list provided by RRM.
 *  \param    meas_bandwidth - Measurement Bandwidth
 *  \param    num_earfcn     - Num of EARFCNs
 *  \param    earfcn         - List of EARFCNs
 */
typedef struct _son_apps_rrm_carrier_freq_and_dl_power_params_req_t
{
    son_u8 meas_bandwidth; /*^ M, 0, B, 6, 100 ^*/
    son_u8 num_earfcn;    /*^ M, 0, N, 0, 0 ^*/
    son_u32 earfcn[SON_MAX_NO_EARFCN];  /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_apps_rrm_carrier_freq_and_dl_power_params_req_t;
/*^ API, SON_APPS_RRM_CARRIER_FREQ_AND_DL_POWER_PARAMS_REQ ^*/

/*
 *  SON_APPS_RRM_CARRIER_FREQ_AND_DL_POWER_PARAMS_RESP
 */

/*! \   son_rrm_neighbor_cell_signal_power_t
 *  \brief    Neighboring cells signal power
 *  \param    pci_id - pci Id of cells
 *  \param    rsrq   - RSRQ
 *  \param    rsrp   - RSRP
 *  \param    reference_signal_power - Reference Signal Power
 */
typedef struct _son_rrm_neighbor_cell_signal_power_t
{
    son_u16 pci_id; /*^ M, 0, H, 0, 503 ^*/
    son_u8 rsrq;  /*^ M, 0, H, 0, 34 ^*/
    son_u8 rsrp;  /*^ M, 0, H, 0, 128 ^*/
    /* SPR 13018 changes start */
    son_s8 reference_signal_power; /*^ M, 0, N, 0, 0 ^*/
    /* SPR 13018 changes end */

}son_rrm_neighbor_cell_signal_power_t;

/*! \   son_rrm_carrier_freq_and_dl_power_params_resp_t
 *  \brief    Carrier frequency and DL power parameters
 *  \param    earfcn - Selected EARFCN
 *  \param    rssi   - RSSI
 *  \param    neighbor_cell_list_size - Neighbor cell list size
 *  \param    neighbor_cell_signal_power - neighboring cells signal power
 */
typedef struct _son_rrm_carrier_freq_and_dl_power_params_resp_t
{
    son_u32 earfcn; /*^ M, 0, N, 0, 0 ^*/
    son_u8 rssi;  /*^ M, 0, H, 0, 128 ^*/
/*SPR_17664_start*/
    son_u16 neighbor_cell_list_size;    /*^ M, 0, H, 0, SON_MAX_NO_CELLS_PER_EARFCN ^*/
/*SPR_17664_end*/
    son_rrm_neighbor_cell_signal_power_t neighbor_cell_signal_power[SON_MAX_NO_CELLS_PER_EARFCN];  /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_rrm_carrier_freq_and_dl_power_params_resp_t;

/*! \   son_apps_carrier_freq_and_dl_power_params_resp_t
 *  \brief    Response of carrier frequency and DL power parameter request
 *  \param    result - SUCCESS/FAILURE
 *  \param    error_code - Error code (valid in case of failure)
 *  \param    carrier_freq_and_dl_power_params_resp - Carrier frequency and other DL Power Parameters
 */
typedef struct _son_apps_rrm_carrier_freq_and_dl_power_params_resp_t
{
    son_return_et result; /*^ M, 0, H, 0, 1 ^*/
    son_error_et error_code; /*^ M, 0, N, 0, 0 ^*/
    son_rrm_carrier_freq_and_dl_power_params_resp_t carrier_freq_and_dl_power_params_resp;  /*^ M, 0, N, 0, 0 ^*/
}son_apps_rrm_carrier_freq_and_dl_power_params_resp_t;
/*^ API, SON_APPS_RRM_CARRIER_FREQ_AND_DL_POWER_PARAMS_RESP ^*/

#define RRM_IPV4_ADD_PRESENT        0x01
#define RRM_IPV6_ADD_PRESENT        0x02

/*! \ son_rrm_enb_tnl_address_t
 *  \brief    Structure to define the Transport Layer Address
 *  \param    bitmask - To indicate if the optional fields are present or not
 *                      Only one of this bit should be set.Bit 1: ipv4_add and
 *                      Bit 2: ipv6_add
 *  \param    ipv4_add - 32bits IPV4 IP Address.
 *  \param    ipv6_add - 128bits IPV6 IP Address.
 */
typedef struct _son_rrm_enb_tnl_address_t
{
    son_u32 bitmask; /*^BITMASK^*/
    son_u8 ipv4_add[SON_IPV4_SIZE];
    /*^O,RRM_IPV4_ADD_PRESENT,OCTET_STRING,FIXED^*/
    son_u8 ipv6_add[SON_IPV6_SIZE];
    /*^O,RRM_IPV6_ADD_PRESENT,OCTET_STRING,FIXED^*/
}son_rrm_enb_tnl_address_t;

/*! \ son_rrm_global_enb_id_t
 *  \brief    Structure to define the global eNB Id
 *  \param    enb_type - eNB Type i.e. HOME or MACRO
 *  \param    plmn_id  - Public Land Mobile Network Identifier
 *  \param    enb_id   - 28 bits Home eNB id or 20 bits Macro eNB id.
 *                       If eNB type is SON_HOME_ENB, this field is
 *                       equal to the 28 leftmost bits of the Cell
 *                       Identity contained in the E-UTRAN CGI of
 *                       the cell served by the eNB.If eNB type is
 *                       SON_MACRO_ENB, this field is equal to the 20
 *                       leftmost bits of the Cell Identity contained
 *                       in the E-UTRAN CGI of each cell served by the eNB.
 */
typedef struct _son_rrm_global_enb_id_t
{
    son_enb_type_et enb_type;
    /*^M,0,B,1,2^*/

    son_rrm_cell_plmn_info_t plmn_id;
    /*^M,0,N,0,0^*/

    son_u8 enb_id[SON_ENB_ID_OCTET_SIZE];
    /*^M,0,OCTET_STRING,FIXED ^*/
}son_rrm_global_enb_id_t;

/* MRO Changes Start */

/*! \ son_rrm_ttt_update_ind_t
 *  \brief    Structure defining the data of TTT update indication send by MRO to RRM whenever TTT needs to be updated.
 *  \param srv_cgi      Serving cell Global identifier
 *  \param updated_ttt  Updated value of Time to Trigger IE
 */
typedef struct _son_rrm_ttt_update_ind_t
{
    son_rrm_intra_rat_global_cell_id_t srv_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_time_to_trigger_range_et updated_ttt;
    /*^ M, 0, N, 0, 0 ^*/
}son_rrm_ttt_update_ind_t; /*^ API, SON_RRM_TTT_UPDATE_IND ^*/
/* MRO Changes End */

/* Spr 16142 Fix Start */
/*! \ son_rrm_stop_tnl_discovery_ind_t
 *  \brief This struct contains the info about stop tnl discovery Ind
 *  \param target_enb_id target enb id
 */
typedef struct _son_rrm_stop_tnl_discovery_ind_t
{
    son_rrm_global_enb_id_t     target_enb_id;
    /*^ M, 0, N, 0, 0 ^*/
}son_rrm_stop_tnl_discovery_ind_t; /*^ API, SON_RRM_STOP_TNL_DISCOVERY_IND ^*/
/* Spr 16142 Fix End */

#endif /* ifndef _son_RRM_INTF_H_ */
