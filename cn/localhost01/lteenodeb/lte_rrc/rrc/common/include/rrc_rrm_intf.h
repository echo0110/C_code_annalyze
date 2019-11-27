/*********************************************************************
 *
 *  FILE NAME   : rrc_rrm_intf.h
 *
 *  DESCRIPTION : File contains the RRM interface API structures.
 *
 *  REVISION HISTORY :
 *
 *  DATE              Name           Reference               Comments
 *  May 15, 2009      Pankaj A       LTE_RRC_API_v0.2.doc    --------
 *  May 26, 2009                                    Added MAC Main Config params
 *  Aug 04, 2009      AlexK                   Some updates to match ASN1 ver 860
 *  Aug 13, 2009      Oleksandr M    sib8 message added
 *  May 31, 2010      Jnanendra Kumar HandOver specific rrc~rrm interface msg 
 *  Copyright (c) 2009, Aricent Inc.
 *
 *******************************************************************/

#ifndef __RRC_RRM_INTF__
#define __RRC_RRM_INTF__

#include "rrc_defines.h"

/* hashdefined value used in RRMIM APIs*/

#define MAX_AC_VALUE                        5
#define MAX_MBSFN_SUBFRAME_CONFIG           8
#define M2AP_SF_ALLOC_FOUR_FRAME_OCTET_SIZE 3
#define MAX_HNB_ID_SIZE                     48
#define MAX_CELL_INTRA                      16
#define MAX_CELL_BLACK_LIST                 16
#define MAX_HNB_ID_OCTET_SIZE               48
#define M_TMSI_OCTET_SIZE                   4
#define MME_OCTET_SIZE                      1
#define C_RNTI_OCTET_SIZE                   2
#define TPC_RNTI_OCTET_SIZE                 2
#define N_2_TX_ANTENNA_TM_3_OCTET_SIZE      1
#define N_4_TX_ANTENNA_TM_3_OCTET_SIZE      1
#define N_2_TX_ANTENNA_TM_4_OCTET_SIZE      1
#define N_4_TX_ANTENNA_TM_4_OCTET_SIZE      8
#define N_2_TX_ANTENNA_TM_5_OCTET_SIZE      1
#define N_4_TX_ANTENNA_TM_5_OCTET_SIZE      2
#define N_2_TX_ANTENNA_TM_6_OCTET_SIZE      1
#define N_4_TX_ANTENNA_TM_6_OCTET_SIZE      2
#define MAX_NUM_OF_SRB                      1
#define MAX_NUM_TARGET_CELL 32
#define MAX_NUM_RAB_PER_UE                  256
#define MAX_NUM_SRB_PER_UE                  9
#define MAX_NUM_SUB_FLOW_PER_RAB            7

#define MAX_EUTRA_CARRIER_FREQ              8
#define SUB_FRAME_ALLOC_ONE_FRAME_OCTET_SIZE    1
#define SUB_FRAME_ALLOC_FOUR_FRAME_OCTET_SIZE   3
#define STMSI_RANDOM_VALUE_OCTET_SIZE       5
#define MAX_N1_PUCCH_AN_PERSIST_SIZE        4
#define LONG_CODE_STATE_1_XRTT_OCTET_SIZE   6
#define XRTT_SID_OCTET_SIZE                 2
#define XRTT_NID_OCTET_SIZE                 2
#define XRTT_REG_PERIOD_OCTET_SIZE          1
#define XRTT_REG_ZONE_OCTET_SIZE            2
#define XRTT_TOTAL_ZONE_OCTET_SIZE          1
#define XRTT_ZONE_TIMER_OCTET_SIZE          1
#define SYNC_SYS_TIME_OCTET_SIZE            5
#define ASYNC_SYS_TIME_OCTET_SIZE           7
#define ECID_CURRENT_SFN_R9_OCTET_SIZE      2
#define MAX_PHYS_CELL_ID_LIST_CDMA2000      16
#define MAX_PHYS_CELL_ID_LIST_CDMA2000_v920      24
#define MAX_NCELLS_PER_BS_LIST_CDMA2000     16
#define MAX_NCELLS_PER_BS_LIST_CDMA2000_v920     16
#define MAX_NEIGH_CELL_LIST_CDMA2000        16
#define MAX_NEIGH_CELL_LIST_CDMA2000_v920   16
#define MAX_CDMA_BAND_CLASS                 32
#define MAX_FREQ                            8
#define MAX_GNFG                            16
#define NCC_PERMITTED_OCTET_SIZE            1
#define MAX_EXPL_ARFCNS                     31
#define MAX_VAR_BITMAP_OF_ARFCNS            16
#define MAX_UTRA_FDD_CARRIER                16
#define MAX_UTRA_TDD_CARRIER                16
#define MAX_MEAS_OBJECT_ID                  32
#define MAX_CELL_MEAS                       32
#define MAX_PN_OFFSET                       511
#define MAX_REPORT_CONFIG_ID                32
#define MAX_MEAS_ID                         32
#define MAX_CELL_REPORT                     8
#define MAX_S1U_QOS_PROFILE_DATA_OCTET_SIZE 254
#define MAX_RAT_CAPABILITY                  8
#define MAX_BAND_EUTRA                      64
/*NR_DC Code Change Start*/
#define MAX_BAND_COMB_EUTRA_NR              64      /*65536*/
#define MAX_SIMULTANEOUS_BANDS_EUTRA_NR     32
#define MAX_EUTRA_FREQ_BANDS                256     
#define MAX_NR_FREQ_BANDS                   1024
#define MAX_NR_FREQ_BANDS_OCT               (MAX_NR_FREQ_BANDS / 64)
#define MAX_RS_INDEX_REPORT_R15             32
#define MAX_RS_INDEX_R15                    63
/*NR_DC Code Change Stop*/
/* CR 543 changes */
/* SPR:9111 start */
/* SPR 13502 Fix Start */
/* As per SPEC 36.331
 * maxBandwidthClass-r10  = 16
 * maxSimultaneousBands-r10  = 64
 * can be used. Due to system memory limitation we have been 
 * changed to value 32 to decrease the size of rrm_ue_context.
 * If system memory supports, 
 * The value can be changed to supported value as per spec i.e 
 * #define MAX_SIM_BANDS_R10                   64
 * #define MAX_BANDWIDTH_CLASS_R10             16 */
/* SPR 19986 Fix Start */ 
#define MAX_SIM_BANDS_R10                   16
#define MAX_BANDWIDTH_CLASS_R10             16
/* SPR 19986 Fix End */ 
#define MAX_SERVING_CELL_R13                32
#define MAX_BANDWIDTH_CLASS_R13             MAX_BANDWIDTH_CLASS_R10
#define MAX_WLAN_BANDS_R13 					8
/* SPR 13502 Fix Stop */
#define MAX_ASN_BUFFER                      512

#define SECURITY_KEY_MAX_SIZE               16

#define RRC_RRM_FDD_DL_HARQ_PROCESS_MIN     6
#define RRC_RRM_FDD_DL_HARQ_PROCESS_MAX     8
#define MAX_CELL_INFO_GERAN_R9              32
#define MAX_CELL_INFO_UTRA_R9               16
#define MAX_GERAN_SI                        10
#define GERAN_SI_DATA_SIZE                  23
#define UTRA_BCCH_CONAINER_SIZE             200
#define MAX_DUMMY_SIZE                      1
#define MAXRSTD_FREQ                        3
#define MAX_IND_OCTET_SIZE                  4
/*CR 713 changes start*/
#define MAX_FREQ_UTRA_TDD_R10               6
/*CR 713 changes stop*/
/* CR 970 changes start*/
#define MAX_MULTI_BANDS                     8
/* CR 970 changes stop*/

/* ENDC_MEAS_CHANGES_START */
#define MAX_CELL_SFTD                       3
#define MAX_CELL_MEAS                      32
#define MAX_QUANT_SETS_NR                   2
/* ENDC_MEAS_CHANGES_END */
#pragma pack(push, 1)

/* rrm_cause_t */

typedef struct
{
    U8   type;/*^ M, 0, N, 0, 0 ^*/
    U16  value;/*^ M, 0, N, 0, 0 ^*/
} rrm_cause_t;

/* CDMA2000_CSFB_HO start */
typedef enum
{
  CDMA2000_1XRTT = 0,
  CDMA2000_HRPD,
  MAX_CDMA2000_NETWORK_TYPES
}rrc_rrm_cdma2000_rat_type_et;

/*BUG 604 changes start*/
typedef enum
{
  PDCP_COUNT_WRAPAROUND,
  UE_CONTEXT_MODIFICATION
}rrc_rrm_intracell_ho_cause_type_et;
/*BUG 604 changes stop*/

/* CDMA2000_CSFB_HO stop */


/* sync changes start */

typedef struct _rrc_rrm_ue_sync_status_t
{
    rrc_bitmask_t               bitmask;       /*^ BITMASK ^*/
    rrc_ue_index_t              ue_index;
    U32                         rrc_rrm_ue_sync_status;
    /*^ M, 0, H, 0, 2 ^*/
} rrc_rrm_ue_sync_status_t;     /*^ API, RRC_RRM_UE_SYNC_STATUS ^*/

/* sync changes stop */
/* SPR 16053 Fix Start */
typedef struct _rrc_rrm_ue_reestab_complete_ind_t
{
    rrc_bitmask_t               bitmask;       /*^ BITMASK ^*/
    rrc_cell_index_t            cell_index;
    /*^ M, 0, H, 0, 7 ^*/       /* MAX_NUM_CELLS - 1 */
    rrc_ue_index_t              ue_index;
    U8                          response;
    /*^ M, 0, H, 0, 1 ^*/   /* rrc_return_et */
}rrc_rrm_ue_reestab_complete_ind_t;       /*^ API, RRC_RRM_UE_REESTAB_COMPLETE_IND ^*/
/* SPR 16053 Fix Stop */


/*****************************************************************************
    RRC_RRM_UE_RELEASE_REQ
******************************************************************************/

typedef struct
{
    /* SPR 11316 Fix Start */ 
    rrc_bitmask_t               bitmask;       /*^ BITMASK ^*/
#define RRC_RRM_UE_RELEASE_CAUSE_PRESENCE_FLAG  0x01
    /* SPR 11316 Fix Stop */ 
    U16    ue_index;

    /* SPR 11316 Fix Start */ 
    rrm_cause_t           cause ; 
    /*^ O, RRC_RRM_UE_RELEASE_CAUSE_PRESENCE_FLAG, N, 0, 0 ^*/
    /* SPR 11316 Fix Stop */ 
}rrc_rrm_ue_release_req_t;  /*^ API, RRC_RRM_UE_RELEASE_REQ ^*/

/******************************************************************************
    RRC_RRM_UE_RELEASE_RESP
******************************************************************************/

typedef struct
{
  U16    ue_index;
  U8     response;       /*^ M, 0, H, 0, 1 ^*/   /* rrc_return_et */
}rrc_rrm_ue_release_resp_t;   /*^ API, RRC_RRM_UE_RELEASE_RESP ^*/

/******************************************************************************
    RRC_RRM_CELL_SETUP_REQ
******************************************************************************/

typedef enum
{
  RRC_RRM_PHICH_R_ONE_SIXTH,
  RRC_RRM_PHICH_R_HALF,
  RRC_RRM_PHICH_R_ONE,
  RRC_RRM_PHICH_R_TWO
}rrm_phich_resource_et;


typedef enum
{
  RRM_RRC_PHICH_D_NORMAL,
  RRM_RRC_PHICH_D_EXTENDED
}rrm_phich_duration_et;



typedef struct
{
  U8   phich_resource;     /*^ M, 0, H, 1, 3 ^*/   /* rrm_phich_resource_et */
  U8   phich_duration;     /*^ M, 0, H, 1, 1 ^*/   /* rrm_phich_duration_et */
}phich_config_t;




typedef enum
{
  RRM_RRC_BW_N_6,
  RRM_RRC_BW_N_15,
  RRM_RRC_BW_N_25,
  RRM_RRC_BW_N_50,
  RRM_RRC_BW_N_75,
  RRM_RRC_BW_N_100
}rrm_band_width_et;


typedef struct
{
  U8               dl_band_width;
/*^ M, 0, H, 1, 5 ^*/  /* rrm_band_width_et */
  phich_config_t   phich_config;    /*^ M, 0, N, 0, 0 ^*/
}mib_info_t;


typedef enum
{
  RRM_RRC_CELL_BARRED,
  RRM_RRC_CELL_NOT_BARRED
}rrm_cell_barred_et;

typedef enum
{
    RRC_RRM_ENTERING,
    RRC_RRM_LEAVING
}rrc_proximity_ind_type_et;

typedef enum
{
    RRC_RRM_INETR_FREQ_RESELECTION_ALLOWED,
    RRC_RRM_INETR_FREQ_RESELECTION_NOT_ALLOWED
}rrm_intra_freq_reselect_et;


#define CELL_ACCESS_INFO_CSG_ID_PRESENCE_FLAG   0x01


typedef struct
{
  U16   presence_bitmask;                /*^ BITMASK ^*/
  U8    tac[TAC_OCTET_SIZE];             /*^ M, 0, OCTET_STRING, FIXED ^*/
  U8    cell_Id[CELL_ID_OCTET_SIZE];     /*^ M, 0, OCTET_STRING, FIXED ^*/
  U8    cell_barred;
/*^ M, 0, H, 1, 1 ^*/   /* rrm_cell_barred_et */

  U8    intra_freq_reselection;
/*^ M, 0, H, 1, 1 ^*/   /* rrm_intra_freq_reselect_et */

  U8    csg_indication;
/*^ M, 0, H, 0, 1 ^*/   /* rrc_bool_et */

  U8    csg_identity[CSG_ID_OCTET_SIZE]; /*^ O, 1, OCTET_STRING, FIXED ^*/
  plmn_id_info_list_t   plmn_Id_info_list;  /*^ M, 0, N, 0, 0 ^*/
}cell_access_related_info_t;




#define CELL_SELECT_INFO_Q_RX_LEV_MIN_OFFSET_PRESENCE_FLAG  0x01

typedef struct
{
  U16   presence_bitmask;       /*^ BITMASK ^*/
  S8    q_rx_lev_min;           /*^ M, 0, B, -70, -22 ^*/
  U8    q_rx_lev_min_offset;    /*^ O, 1, B, 1, 8 ^*/
}cell_selection_Info_t;

typedef enum
{
  RRM_RRC_SI_WINDOW_LEN_MS_1,
  RRM_RRC_SI_WINDOW_LEN_MS_2,
  RRM_RRC_SI_WINDOW_LEN_MS_5,
  RRM_RRC_SI_WINDOW_LEN_MS_10,
  RRM_RRC_SI_WINDOW_LEN_MS_15,
  RRM_RRC_SI_WINDOW_LEN_MS_20,
  RRM_RRC_SI_WINDOW_LEN_MS_40
}rrm_si_window_length_et;




typedef enum
{
  RRM_RRC_RF_8,
  RRM_RRC_RF_16,
  RRM_RRC_RF_32,
  RRM_RRC_RF_64,
  RRM_RRC_RF_128,
  RRM_RRC_RF_256,
  RRM_RRC_RF_512
}rrm_si_periodicity_et;




typedef enum
{
  RRM_RRC_SIB_TYPE_3,
  RRM_RRC_SIB_TYPE_4,
  RRM_RRC_SIB_TYPE_5,
  RRM_RRC_SIB_TYPE_6,
  RRM_RRC_SIB_TYPE_7,
  RRM_RRC_SIB_TYPE_8,
  RRM_RRC_SIB_TYPE_9,
  RRM_RRC_SIB_TYPE_10,
  RRM_RRC_SIB_TYPE_11,
  RRM_RRC_SIB_TYPE_12,
  RRM_RRC_SIB_TYPE_13
}rrm_sib_type_et;

typedef struct
{
  sib_mapping_info_t   sib_mapping_info; /*^ M, 0, N, 0, 0 ^*/
  U8                   si_periodicity;
/*^ M, 0, H, 0, 6 ^*/ /* rrm_si_periodicity_et */
  U8                   si_index;   /*^M, 0, N, 0, 10 ^*/

}new_scheduling_info_t;

typedef struct
{
/*SPR_17664_start*/
  U8                      count;              /*^ M, 0, B, 1, MAX_SI_MESSAGE ^*/
/*SPR_17664_end*/
  new_scheduling_info_t   scheduling_info[MAX_SI_MESSAGE];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}new_scheduling_info_list_t;


typedef enum
{
sa_0, 
sa_1, 
sa_2, 
sa_3, 
sa_4, 
sa_5, 
sa_6
}rrm_sub_frame_assignment_et;


typedef enum 
{
ssp_0, 
ssp_1,
ssp_2, 
ssp_3, 
ssp_4,
ssp_5, 
ssp_6, 
ssp_7,
ssp_8
}rrm_special_sub_frame_pattern_et;

typedef struct
{
  U8   sub_frame_assignment;
/*^ M, 0, H, 0, 6 ^*/  /* rrm_sub_frame_assignment_et */
  /*** TDD changes start ***/
  U8   special_sub_frame_pattern;
/*^ M, 0, H, 0 , 8 ^*/  /* rrm_special_sub_frame_pattern_et */
  /*** TDD changes stop ***/
}tdd_config_t;

typedef enum
{
  RRM_RRC_TRUE
}ims_emergency_support_et;

/* cr_398 new structure*/
typedef struct
{
    U16                           presence_bitmask;         /*^ BITMASK ^*/
#define CELL_SELECTION_INFO_Q_QUAL_MIN_OFFSET_R9_PRESENT_FLAG  0x01
    S8                        q_qual_min_r9; /*^ M, 0, B, -34, -3 ^*/
    U8                        q_qual_min_offset_r9; 
/*^ O, CELL_SELECTION_INFO_Q_QUAL_MIN_OFFSET_R9_PRESENT_FLAG, B, 1, 8 ^*/

}cell_selection_info_v920_t;



/*eMTC Changes start*/

/****************************************
 * STRUCTURE CHANGES
 ***************************************/
/*! \ RrcRrmAdmissionEmtcConfigInfo_t
 *  \brief This struct is used to indicates the MPDCCH config parameters
 *         for Release 13
 *  \param bitmask 
 *         To check optional field is present or not
 *  \param EpdcchConfig
 *         It indicates epdcch config parameters for Release 13 
 *  \param PucchConfigDedicatedR13
 *         It indicates pucchConfigDedicated parameters for release 13
 *  \param ceMode
 *         It indicates the CE mode of eMTC UE
 */
#define RRC_RRM_ADMISSION_EMTC_PUCCH_CONFIG_DEDICATED_PRESENT   0x01
typedef struct _RrcRrmAdmissionEmtcConfigInfo_t
{
    RrcBitmaskU8_t 			        bitmask;
    /*^ BITMASK ^*/

    EpdcchConfig_t                  epdcchConfig;
    /*^ M, 0, N, 0, 0 ^*/

    U8                              ceMode;
    /*^ M, 0, H, 0, EMTC_CE_MODE_MAX ^*/
    /* EmtcCeMode_et */

    U8                              dlMcs;
    /*^ M, 0, H, 0, EMTC_MAX_MCS ^*/

    U8                              ulMcs;
    /*^ M, 0, H, 0, EMTC_MAX_MCS ^*/

    RrcPucchConfigDedicatedR13_t    pucchConfigDedicated;
    /*^ O,RRC_RRM_ADMISSION_EMTC_PUCCH_CONFIG_DEDICATED_PRESENT, N, 0, 0 ^*/

}RrcRrmAdmissionEmtcConfigInfo_t;

/*! \ FddDownlinkOrTddSubframeBitmapBrR13_t
 *  \brief This struct is used to describe the set of valid
 *         subframes for FDD downlink or TDD transmissions
 *  \param count 
 *         indicates number of subframe patterns
 *  \param subframePatternR13 
 *         indicates subframe pattern in 10 or 40 bit string
 */
typedef struct _FddDownlinkOrTddSubframeBitmapBrR13_t
{
    U8				count; 
    /*^ M, 0, B, 2, EMTC_MAX_DL_SF_PATTERN_BYTES ^*/

    U8				subframePatternR13[EMTC_MAX_DL_SF_PATTERN_BYTES];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}FddDownlinkOrTddSubframeBitmapBrR13_t;


/*! \ SystemInfoValueTagListR13_t
 *  \brief This struct describes SI message specific 
 *         value tags for BL UEs and UE in CE
 *  \param count 
 *         indicate count of Br SI value tag 
 *  \param systemInfoValueTagR13 
 *         List of Br SI value tags for particular SI
 */
typedef struct _SystemInfoValueTagListR13_t
{
    U8     count;
    /*^ M, 0, B, 1, EMTC_MAX_SI_COUNT ^*/

    U8     systemInfoValueTagR13[EMTC_MAX_SI_COUNT];
    /*^ M, 0, OCTET_STRING, VARIABLE, H, 0, EMTC_MAX_SYSTEM_INFO_VALUE_TAG^*/
}SystemInfoValueTagListR13_t;

/*! \ BrSibMappingInfo_t
 *  \brief This struct indicates list of Br SIB mapping 
 *  \param count
 *         indicate count of Br SIB mapping
 *  \param BrSibType
 *         indicate Br SIB to be mapped
 */
typedef struct _BrSibMappingInfo_t
{
    U8            count;
    /*^ M, 0, N, 0, EMTC_MAX_SIB_COUNT ^*/

    U8            brSibType[EMTC_MAX_SIB_COUNT];
    /*^ M, 0, OCTET_STRING, VARIABLE, H, 0, EMTC_MAX_SIB_COUNT ^*/
    /* EmtcSibType_et */
}BrSibMappingInfo_t;

/*! \ BrSchedulingInfo_t
 *  \brief This struct describes Br Secheduling info for BL UEs and UEs in CE
 *  \param siNarrowbandR13
 *         indciates narrowband to be used for SIs
 *  \param siPeriodicity
 *         indicates periodicity of BR SIs
 *  \param BrSibMappingInfo
 *         indicates mapping info of Br SIBs
 */
typedef struct _BrSchedulingInfo_t
{
    U8                    siNarrowbandR13;
    /*^ M, 0, B, 1, EMTC_MAX_AVAILABLE_NARROWBAND ^*/

    U8                    siPeriodicity;
    /*^ M, 0, H, 0, EMTC_SI_PERIODICITY_MAX ^*/
    /* EmtcSiPeriodicity_et */

    BrSibMappingInfo_t    sibMappingInfo;
    /*^ M, 0, N, 0, 0 ^*/
}BrSchedulingInfo_t;

/*! \ SchedulingInfoListBrR13_t
 *  \brief This struct indicates additional scheduling 
 *         information of SI messages for BL UEs and UEs in CE
 *  \param count 
 *         indicate count of scheduling Br SIs
 *  \param BrSchedulingInfo 
 *         indicate Br SIs to be scheduled 
 */
typedef struct _SchedulingInfoListBrR13_t
{
    U8 		              count;
    /*^ M, 0, B, 1, EMTC_MAX_SI_COUNT ^*/

    BrSchedulingInfo_t    brSchedulingInfo[EMTC_MAX_SI_COUNT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}SchedulingInfoListBrR13_t;

/*! \ BandwidthReducedAccessRelatedInfoR13_t
 *  \brief This struct describes Cell access related info for eMTC
 *  \param bitmask 
 *         To check optional field is present or not
 *  \param siWindowLengthBrR13 
 *         indicates common SI scheduling window for all SIs. 
 *  \param siRepetitionPatternR13 
 *         indicates the radio frames within the SI window 
 *         used for SI message transmission
 *  \param startSymbolBrR13 
 *         indicates the OFDM starting symbol for any MPDCCH, 
 *         PDSCH scheduled on the same cell 
 *  \param siHoppingConfigCommonR13 
 *         indicate frequency hopping activation/deactivation 
           for BR versions of SI messages and MPDCCH/PDSCH of paging
 *  \param schedulingInfoListBrR13 
 *         indicates additional scheduling information of SI 
 *         messages for BL UEs and UEs in CE
 *  \param fddDownlinkOrTddSubframeBitmapBrR13 
 *         indicates the set of valid subframes for FDD downlink 
 *         or TDD transmissions
 *  \param fddUplinkSubframeBitmapBrR13 
 *         indicate the set of valid subframes for FDD uplink 
 *         transmissions for BL UEs
 *  \param systemInfoValueTagListR13 
 *         indicates SI message specific value tags for BL UEs 
 *         and UE in CE
 */

#define RRC_RRM_SCHEDULING_INFO_LIST_BR_R13_PRESENT          0x01
#define RRC_RRM_FDD_DOWNLINK_TDD_SF_BITMAP_BR_R13_PRESENT    0x02
#define RRC_RRM_FDD_UPLINK_SF_BITMAP_BR_R13_PRESENT          0x04
#define RRC_RRM_SI_VALIDITY_TIME_R13_PRESENT                 0x08
#define RRC_RRM_SI_VALUE_TAG_LIST_R13_PRESENT                0x10
typedef struct _BandwidthReducedAccessRelatedInfoR13_t
{
    RrcBitmaskU8_t 			                bitmask;
    /*^ BITMASK ^*/

    U8		                                siWindowLengthBrR13;
    /*^ M, 0, H, 0, EMTC_SI_WINDOW_LEN_MAX ^*/
    /* EmtcSiWindowLength_et */

    U8		                                siRepetitionPatternR13;
    /*^ M, 0, H, 0, EMTC_SI_REPETITION_MAX ^*/
    /*EmtcSiRepetitionPattern_et */	

    U8 					                    startSymbolBrR13;
    /*^ M, 0, B, 1, EMTC_MAX_START_SYMBOL ^*/ 

    U8	                                    siHoppingConfigCommonR13;
    /*^ M, 0, H, 0, EMTC_SI_HOPPING_MAX ^*/
    /* EmtcSiHoppingConfigCommon_et */

    SchedulingInfoListBrR13_t		        schedulingInfoListBrR13; 
    /*^ O, RRC_RRM_SCHEDULING_INFO_LIST_BR_R13_PRESENT, N, 0, 0 ^*/

    FddDownlinkOrTddSubframeBitmapBrR13_t   fddDownlinkOrTddSubframeBitmapBr;
    /*^ O, RRC_RRM_FDD_DOWNLINK_TDD_SF_BITMAP_BR_R13_PRESENT, N, 0, 0 ^*/	

    U16 					                fddUplinkSubframeBitmapBr;
    /*^ O, RRC_RRM_FDD_UPLINK_SF_BITMAP_BR_R13_PRESENT, H, 0, EMTC_MAX_FDD_UPLINK_SF_BITMAP^*/

    SystemInfoValueTagListR13_t	            systemInfoValueTagList;
    /*^ O, RRC_RRM_SI_VALUE_TAG_LIST_R13_PRESENT, N, 0, 0 ^*/
}BandwidthReducedAccessRelatedInfoR13_t;


/*! \CellSelectionInfoCeR13_t
 *  \brief This struct describes cell selection information 
 *         for BL UEs and UEs in CE. 
 *  \param bitmask 
 *         To check optional field is present or not
 *  \param qRxLevMinCeR13 
 *         indicate the minimum Rx level of cell 
 *  \param qQualMinRrsqCeR13 
 *         indicate that when performing RSRQ measurements, UE 
 *         shall perform RSRQ measurement on all OFDM symbols
 */

#define SIB_1_Q_QUAL_MIN_RSRQ_CE_R13_PRESENT     0x01
typedef struct _CellSelectionInfoCeR13_t
{
    RrcBitmaskU8_t    bitmask;
    /*^ BITMASK ^*/

    S8	              qRxLevMinCeR13;
    /*^ M, 0, B, -70, EMTC_MAX_RXLEVMIN ^*/     

    S8                qQualMinRrsqCeR13;
    /*^ O, SIB_1_Q_QUAL_MIN_RSRQ_CE_R13_PRESENT, B, -34, EMTC_MAX_Q_QUALMINRSRQ ^*/
}CellSelectionInfoCeR13_t;


/*! \  SibType1V1310Ies_t
 *  \brief This struct is used for sib type 1 BR information for eMTC
 *  \      Possible value as per the 3GPP spec 36.331
 *  \param bitmask 
 *         To check optional field is present or not
 *  \param cellSelectionInfoCeR13 
 *         indicates cell selection information for CE 
 *  \param bandwidthReducedAccessRelatedInfoR13 
 *         Configuration for BL UE or UE in CE
 */

#define RRC_RRM_EDRX_ALLOWED_R13_PRESENT                     0x01
#define RRC_RRM_CELL_SELECTION_INFO_R13_PRESENT              0x02
#define RRC_RRM_BW_REDUCED_ACESS_RELATED_INFO_R13_PRESENT    0x04
typedef struct _SibType1V1310Ies_t
{
    RrcBitmaskU8_t                         bitmask;
    /*^ BITMASK ^*/

    U8                                     sib1BrRepetition;
    /*^ M, 0, H, 0, EMTC_SIB1_BR_REPETITION_NMAX ^*/
    /* EmtcSib1BrRepetitionNum_et */

    CellSelectionInfoCeR13_t      	       cellSelectionInfoCeR13;
    /*^ O, RRC_RRM_CELL_SELECTION_INFO_R13_PRESENT, N, 0, 0 ^*/

    BandwidthReducedAccessRelatedInfoR13_t bandwidthReducedAccessRelatedInfoR13;
    /*^ O, RRC_RRM_BW_REDUCED_ACESS_RELATED_INFO_R13_PRESENT, N, 0, 0 ^*/
}SibType1V1310Ies_t;

/*! \RsrpThresholdsPrachInfoListR13_t
 *  \brief This struct decribes the criterion for BL UEs and UEs 
 *         in CE to select PRACH resource set
 *  \param count 
 *         indicates the number of instances for RSRP thresholds
 *  \param rsrpRange 
 *         indicate the RSRP threshold values to determine the 
 *         CE level for PRACH
 */
typedef struct _RsrpThresholdsPrachInfoListR13_t
{
    U8    count;
    /*^ M, 0, B, 1, EMTC_MAX_RSRP_LIST_SIZE ^*/

    U8    rsrpRange[EMTC_MAX_RSRP_LIST_SIZE];
    /*^ M, 0, OCTET_STRING, VARIABLE, H, 0, EMTC_MAX_RSRP_RANGE ^*/
}RsrpThresholdsPrachInfoListR13_t;


/*! \MpdcchNarrowbandsToMonitorR13_t
 *  \brief This struct decribes the narrowbands to 
 *         monitor for MPDCCH for RAR
 *  \param count 
 *         indicates the number of narrowbands for MPDCCH
 *  \param mpdcchNarrowbandsToMonitorR13 
 *         indicate the narrowbands to monitor for MPDCCH 
 */
typedef struct _MpdcchNarrowbandsToMonitorR13_t
{
    U8        count;
    /*^ M, 0, B, 1 , EMTC_MAX_NARROWBAND_TO_MONITOR^*/

    U8        mpdcchNarrowbandsToMonitor[EMTC_MAX_NARROWBAND_TO_MONITOR];
    /*^ M, 0, OCTET_STRING, VARIABLE , B, 1, EMTC_MAX_AVAILABLE_NARROWBAND ^*/
}MpdcchNarrowbandsToMonitorR13_t;

/*! \PrachParametersCeR13_t
 *  \brief This struct contains MTC PRACH configuration 
 *         parameters
 *  \param prachConfigIndexR13 
 *         indicates prach-ConfigurationIndex 
 *  \param prachFreqOffsetR13 
 *         indicates prach-FrequencyOffset
 *  \param prachStartingSubframeR13 
 *         indicates PRACH starting subframe periodicity 
 *  \param maxNumPreambleAttemptCeR13 
 *         indicates Maximum number of preamble transmission 
 *         attempts per CE level
 *  \param numRepetitionPerPreambleAttemptR13 
 *         indicates Number of PRACH repetitions per attempt 
 *         for each CE level
 *  \param mpdcchNarrowbandsToMonitorR13 
 *         indicates Narrowbands to monitor for MPDCCH for RAR
 *  \param mpdcchNumRepetitionRaR13 
 *         indicates Maximum number of repetitions for 
 *         MPDCCH common search space 
 *  \param prachHoppingConfigR13 
 *         indicates Coverage level specific frequency hopping 
 *         configuration for PRACH
 */

#define RRC_PRACH_STARTING_SF_R13_PRESENT             0x01
#define RRC_MAX_NUM_PREAMBLE_ATTEMPT_CE_R13_PRESENT    0x02
typedef struct _PrachParametersCeR13_t
{
    RrcBitmaskU8_t                          bitmask;
    /*^ BITMASK ^*/

    U8                                      prachConfigIndexR13;
    /*^ M, 0, H, 0, EMTC_MAX_PRACH_CONFIG_INDEX ^*/

    U8                                      prachFreqOffsetR13;
    /*^ M, 0, H, 0, EMTC_MAX_PRACH_FREQ_OFFSET ^*/

    U8                                      numRepetitionPerPreambleAttemptR13;
    /*^ M, 0, H, 0, EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_MAX ^*/
    /* EmtcNumRepetitionPerPreambleAttempt_et */

    MpdcchNarrowbandsToMonitorR13_t         mpdcchNarrowbandsToMonitor; 
    /*^ M, 0, N, 0, 0 ^*/

    U8                                      mpdcchNumRepetitionRaR13 ;
    /*^ M, 0, H, 0, EMTC_MPDCCH_NUM_REPETITION_RA_MAX ^*/
    /* EmtcMpdcchNumRepetitionRa_et */

    U8                                      prachHoppingConfigR13;
    /*^ M, 0, H, 0, EMTC_PRACH_HOPPING_CONFIG_MAX ^*/
    /* EmtcPrachHoppingConfig_et */

    U8                                      prachStartingSubframeR13;
    /*^ O, RRC_PRACH_STARTING_SF_R13_PRESENT, H, 0, EMTC_PRACH_STARTING_SF_MAX ^*/
    /* EmtcPrachStartingSubframe_et */

    U8                                      maxNumPreambleAttemptCeR13;
    /*^ O, RRC_MAX_NUM_PREAMBLE_ATTEMPT_CE_R13_PRESENT, H, 0, EMTC_MAX_NUM_PREAMBLE_ATTEMPT_CE_MAX ^*/
    /* EmtcMaxNumPreambleAttemptCe_et */

}PrachParametersCeR13_t;


/*! \PrachParametersListCeR13_t
 *  \brief This struct contains PRACH parameters for 
 *         each CE level
 *  \param count 
 *         indicates maximum available CE level
 *  \param prachParametersCeR13 
 *         contains eMTC PRACH configuration parameters 
 */
typedef struct _PrachParametersListCeR13_t
{
    U8                        count;
    /*^ M, 0, B, 1, EMTC_MAX_CE_LEVEL ^*/

    PrachParametersCeR13_t    prachParametersCeR13[EMTC_MAX_CE_LEVEL];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}PrachParametersListCeR13_t;


/*! \PrachConfigV1310_t
 *  \brief This struct contains lte cell ran physical 
 *         layer PRACH params
 *  \param bitmask
 *         to check optional field is present or not
 *  \param prachParametersListCeR13 
 *         indicates PRACH parameters for each CE level.
 *  \param rsrpThresholdsPrachInfoListR13 
 *         indicates the criterion for BL UEs to select 
 *         PRACH resource set
 *  \param mpdcchStartSfCssRaR13 
 *         indicates FDD Starting subframe configuration for 
 *         MPDCCH common search space.
 */

#define RRC_RSRP_THRESHOLD_PRACH_INFO_LIST_R13_PRESENT    0x01
#define RRC_MPDCCH_START_SF_CSS_RA_R13_PRESENT            0x02
typedef struct _PrachConfigV1310_t
{
    RrcBitmaskU8_t                        bitmask;
    /*^ BITMASK ^*/

    PrachParametersListCeR13_t            prachParametersListCe;
    /*^ M, 0, N, 0, 0 ^*/

    RsrpThresholdsPrachInfoListR13_t      rsrpThresholdsPrachInfoList;
    /*^ O, RRC_RSRP_THRESHOLD_PRACH_INFO_LIST_R13_PRESENT, N, 0, 0 ^*/

    U8                                    mpdcchStartSfCssRaFdd;
    /*^ O, RRC_MPDCCH_START_SF_CSS_RA_R13_PRESENT, H, 0, EMTC_MPDCCH_START_SF_CSS_RA_FDD_MAX ^*/
    /* EmtcMpdcchStartSfCssRaFdd_et */

}PrachConfigV1310_t;


/*! \PucchConfigCommonV1310_t
 *  \brief This struct contains PUSCH parameters for Release 13
 *  \param bitmask 
 *         to check optional field is present or not
 *  \param n1PucchAnInfoListR13
 *         indicates list of N1 pucch an info for release 13
 *  \param pucchNumRepetitionCeMsg4Level0R13 
 *         indicates Number of repetitions for PUCCH carrying
 *         HARQ response for PRACH CE level 0
 *  \param pucchNumRepetitionCeMsg4Level1R13 
 *         indicates Number of repetitions for PUCCH carrying 
 *         HARQ response for PRACH CE level 1
 */

#define RRC_PUCCH_NUM_REPETITION_CE_MSG4_LEVEL0_R13_PRESENT 0x01
#define RRC_PUCCH_NUM_REPETITION_CE_MSG4_LEVEL1_R13_PRESENT 0x02
#define RRC_N1_PUUCH_AN_INFO_LIST_R13_PRESENT               0x04
typedef struct _PucchConfigCommonV1310_t
{
    RrcBitmaskU8_t               bitmask;
    /*^ BITMASK ^*/
    
    RrcN1PucchAnInfoListR13_t    n1PucchAnInfoList;
    /*^ O, RRC_N1_PUUCH_AN_INFO_LIST_R13_PRESENT, N, 0, 0 ^*/

    U8                           pucchNumRepetitionCeMsg4Level0;
   /*^ O, RRC_PUCCH_NUM_REPETITION_CE_MSG4_LEVEL0_R13_PRESENT, H, 0, EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L0_MAX ^*/
    /* EmtcPucchNumRepetitionCeMsg4Level0_et */

    U8                           pucchNumRepetitionCeMsg4Level1;
    /*^ O, RRC_PUCCH_NUM_REPETITION_CE_MSG4_LEVEL1_R13_PRESENT, H, 0,EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L1_MAX ^*/
    /* EmtcPucchNumRepetitionCeMsg4Level1_et */
}PucchConfigCommonV1310_t;


/*! \PuschConfigCommonV1310_t
 *  \brief This struct contains the PUSCH parameters 
 *         for Release 13
 *  \param puschMaxNumRepetitionCEmodeAR13 
 *         indicates PUSCH max number of repetitions
 *         for CE Mode A
 */
typedef struct _PuschConfigCommonV1310_t
{
    U8               puschMaxNumRepetitionCeModeA;
    /*^ M, 0, H, 0, EMTC_PUSCH_MAX_NUM_REPTITION_CE_MODE_A_MAX ^*/
    /* EmtcPuschMaxNumRepetitionCeModeA_et */
}PuschConfigCommonV1310_t;

/*! \PdschConfigCommonV1310_t
 *  \brief This struct contains the PDSCH parameters 
 *         for Release 13
 *  \param pdschMaxNumRepetitionCEmodeAR13 
 *         indicates the set of PDSCH repetition numbers 
 *         for CE mode A
 */
typedef struct _PdschConfigCommonV1310_t
{
    U8               pdschMaxNumRepetitionCeModeA;
    /*^ M, 0, H, 0, EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_MAX ^*/
    /* EmtcPdschMaxNumRepetitionCeModeA_et */
}PdschConfigCommonV1310_t;


/*! \ RrcPdschPagingRepetition_t
 *  \brief This structure indicates maximum number of repetitions for PDSCH
 *        common search space (CSS) for paging
 *  \param bitmask
 *         It indicates optional field is present or not
 *  \param pdschPagingRepetitionLevel1
 *         It indicates the number of pdsch paging repetition for CE level 1
 *  \param pdschPagingRepetitionLevel2
 *         It indicates the number of pdsch paging repetition for CE level 2
 *  \param pdschPagingRepetitionLevel3
 *         It indicates the number of pdsch paging repetition for CE level 3
 *  \param pdschPagingRepetitionLevel4
 *         It indicates the number of pdsch paging repetition for CE level 4
 */

#define RRC_PDSCH_PAGING_REP_LEVEL1_PRESENT        0x01
#define RRC_PDSCH_PAGING_REP_LEVEL2_PRESENT        0x02
#define RRC_PDSCH_PAGING_REP_LEVEL3_PRESENT        0x04
#define RRC_PDSCH_PAGING_REP_LEVEL4_PRESENT        0x08
typedef struct _RrcPdschPagingRepetition_t
{
     RrcBitmaskU8_t       bitmask;
     /*^ BITMASK ^*/

     U8                 pdschPagingRepetitionLevel1;
     /*^ O, RRC_PDSCH_PAGING_REP_LEVEL1_PRESENT, H, 0, EMTC_PDSCH_PAGING_REP_LEVEL1_MAX ^*/
     /* EmtcPdschPagingRepetitionLevel1_et */

     U8                 pdschPagingRepetitionLevel2;
     /*^ O, RRC_PDSCH_PAGING_REP_LEVEL2_PRESENT, H, 0, EMTC_PDSCH_PAGING_REP_LEVEL2_MAX ^*/
     /* EmtcPdschPagingRepetitionLevel2_et */

     U8                 pdschPagingRepetitionLevel3;
     /*^ O, RRC_PDSCH_PAGING_REP_LEVEL3_PRESENT, H, 0, EMTC_PDSCH_PAGING_REP_LEVEL3_MAX ^*/
     /* EmtcPdschPagingRepetitionLevel3_et */

     U8                 pdschPagingRepetitionLevel4;
     /*^ O, RRC_PDSCH_PAGING_REP_LEVEL4_PRESENT, H, 0, EMTC_PDSCH_PAGING_REP_LEVEL4_MAX ^*/
     /* EmtcPdschPagingRepetitionLevel4_et */

}RrcPdschPagingRepetition_t;

/*! \ PcchConfigV1310_t
 *  \brief This struct is used for PCCH config for eMTC
 *  \param bitmask 
 *         To check optional field is present or not
 *  \param pagingNarrowBandsR13 
 *         indicates number of narrowbands used for paging
 *  \param mpdcchNumRepetitionPagingR13 
 *         indicates maximum number of repetitions for MPDCCH common 
 *         search space (CSS) for paging
 *  \param nBv1310 
 *         Used to derive the Paging Frame and Paging Occasion
 *  \param pdschNumRepetitionPaging
 *         indicates maximum number of paging repetitions for PDSCH 
 */

#define RRC_PCCH_CONFIG_V1310_NB_PRESENT 0x01
typedef struct _PcchConfigV1310_t
{
    RrcBitmaskU8_t                bitmask;
    /*^ BITMASK ^*/

    U8                            pagingNarrowBandsR13;
    /*^ M, 0, B, 1, EMTC_MAX_AVAILABLE_NARROWBAND ^*/

    U8                            mpdcchMaxNumRepetitionPaging;
    /*^ M, 0, H, 0, EMTC_MPDCCH_MAX_NUM_REPETITION_PAGING_MAX ^*/

    U8                            mpdcchNumRepetitionPaging;
    /*^ M, 0, B, 1, EMTC_MPDCCH_NUM_REPETITION_PAGING_MAX ^*/

    U8                            pdschNumRepetitionPaging;
    /*^ M, 0, B, 1, EMTC_PDSCH_PAGING_REP_MAX ^*/

    U8                            nBv1310;
    /*^ O, RRC_PCCH_CONFIG_V1310_NB_PRESENT, H, 0, EMTC_NB_MAX ^*/
    /* EmtcNb_et */
    
}PcchConfigV1310_t;

/*! \RachCeLevelInfoR13_t
 *  \brief This struct provides configuration parameters 
 *         for each CE level of RACH
 *  \param preambleMappingInfoR13 
 *         provides the mapping of premables to groups 
 *         for each CE level
 *  \param raResponseWindowSizeR13 
 *         indicates size of the RA response window
 *  \param macContentionResolutionTimerR13 
 *         indicates timer for mac contention resolution
 *  \param rarHoppingConfigR13 
 *         indicates frequency hopping activation/deactivation 
 *         for RAR/Msg3/Msg4 for a CE level
 */
typedef struct _RachCeLevelInfoR13_t
{
    PreambleMappingInfoR13_t      preambleMappingInfoR13;
    /*^ M, 0, N, 0, 0 ^*/

    U8                            raResponseWindowSizeR13;
    /*^ M, 0, H, 0, EMTC_RA_RESPONSE_WINDOW_SIZE_SF_MAX ^*/
    /* EmtcRaResponseWindowSize_et */

    U8                            macContentionResolutionTimerR13;
    /*^ M, 0, H, 0, EMTC_MAC_CONTENTION_RESOLUTION_TIMER_MAX ^*/
    /* EmtcMacContentionResolutionTimer_et */

    U8                            rarHoppingConfigR13;
    /*^ M, 0, H, 0, EMTC_RAR_HOPPPING_CONFIG_MAX ^*/
    /* EmtcRarHoppingConfig_et  */
}RachCeLevelInfoR13_t;


/*! \RachCeLevelInfoListR13_t
 *  \brief This struct contains list of RACH information 
 *         for each coverage level
 *  \param count 
 *         indicates number of CE Level
 *  \param rachCeLevelInfoListR13 
 *         provides configuration parameters for each CE level
 */
typedef struct _RachCeLevelInfoListR13_t
{
    U8                       numCeLevels;
    /*^ M, 0, H, 1, EMTC_MAX_CE_LEVEL ^*/

    RachCeLevelInfoR13_t     rachCeLevelInfo[EMTC_MAX_CE_LEVEL];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}RachCeLevelInfoListR13_t;

/* eMTC changes stop */

/*! \ SibType1V1250Ies_t 
 *  \brief This struct decribe parameters of SIB1 for Release 12
 *  \param bitmask To check optional field is present or not
 *  \param non_critical_extension indicate the further extension of release
 */

#define SIB_TYPE_1_V1310_PRESENT 0x01
typedef struct 
{
    RrcBitmaskU8_t       presenceBitmask;
    /*^ BITMASK ^*/

    SibType1V1310Ies_t   nonCriticalExtension;
    /*^ O, SIB_TYPE_1_V1310_PRESENT, N, 0, 0 ^*/

}SibType1V1250Ies_t;


/*! \SibType1V1130Ies_t
 *  \brief This struct decribe parameters of SIB1 for Release 11
 *  \param bitmask To check optional field is present or not
 *  \param non_critical_extension indicate the  further extension of release
 */

#define SIB_TYPE_1_V1250_PRESENT 0x01
typedef struct 
{
    RrcBitmaskU8_t      presenceBitmask;
    /*^ BITMASK ^*/

    SibType1V1250Ies_t  nonCriticalExtension;
    /*^ O, SIB_TYPE_1_V1250_PRESENT, N, 0, 0 ^*/
}SibType1V1130Ies_t;

#define SIB_TYPE_1_V920_IES_IMS_EMERGENCY_SUPPORT_PRESENT_FLAG 0x01
#define SIB_TYPE_1_V920_IES_CELL_SELECTION_INFO_V920_PRESENT_FLAG   0x02

/*eMTC Changes start*/
#define SIB_TYPE_1_V920_IES_NON_CRITICAL_EXTENSION_PRESENT_FLAG     0x04
/*eMTC Changes stop*/

typedef struct
{
  U16                           presence_bitmask;         /*^ BITMASK ^*/
  U8                           ims_emergency_support;  
 /*^ O, 1, H, 0, 0 ^*/ /* ims_emergency_support_et */

  cell_selection_info_v920_t   cell_selection_info_v920;
  /*^ O, 2, N, 0, 0 ^*/
    /*eMTC Changes start*/
    SibType1V1130Ies_t              nonCriticalExtension;
    /*^ O, SIB_TYPE_1_V920_IES_NON_CRITICAL_EXTENSION_PRESENT_FLAG, N, 0, 0 ^*/
    /*eMTC Changes stop*/

}sib_type_1_v920_ies_t;


/*CR 970 changes start*/
typedef struct
{
/* SPR-18445 start */
    U8     count;  
    /*^ M, 0, B, 1, MAX_MULTI_BANDS^*/
/* SPR-18445 stop */
    U8      freq_band_indicator[MAX_MULTI_BANDS]; 
    /*^ M, 0, OCTET_STRING, VARIABLE , B, 1, MAX_FREQ_BAND_INDICATOR ^*/
}multi_band_info_list_t;

typedef struct
{
   U16                          bitmask;    /*^ BITMASK ^*/
#define SIB_TYPE_1_v8h0_IES_MULTI_BAND_INFO_LIST_PRESENT    0x01

   multi_band_info_list_t       multi_band_info_list;
   /*^ O, SIB_TYPE_1_v8h0_IES_MULTI_BAND_INFO_LIST_PRESENT, N, 0, 0 ^*/
}sib_type_1_v8h0_ies_t;
/*CR 970 changes stop*/


#define SIB_TYPE_1_V890_IES_NON_CROTICAL_EXTENSION_PRESENT_FLAG      0x01
  /*CR 970 changes start*/
#define SIB_TYPE_1_V890_IES_LATE_NON_CRITICAL_EXTENSION_PRESENT_FLAG 0x02
  /*CR 970 changes stop*/

typedef struct
{
  U16                           presence_bitmask;         /*^ BITMASK ^*/

  /*CR 970 changes start*/
  sib_type_1_v8h0_ies_t         late_non_critical_extension;
  /*^ O, SIB_TYPE_1_V890_IES_LATE_NON_CRITICAL_EXTENSION_PRESENT_FLAG, N, 0, 0^*/
  /*CR 970 changes stop*/

  sib_type_1_v920_ies_t         non_critical_extension;  /*^ O, 1, N, 0, 0 ^*/
}sib_type_1_v890_ies_t;

#define SIB_TYPE_1_P_MAX_PRESENCE_FLAG                  0x01
#define SIB_TYPE_1_TDD_CONFIG_PRESENT_FLAG              0x02
#define SIB_TYPE_1_NON_CRITICAL_EXTENSION_PRESENT_FLAG  0x04

typedef struct
{
  U16                          presence_bitmask;         /*^ BITMASK ^*/
  cell_access_related_info_t   cell_access_related_info; /*^ M, 0, N, 0, 0 ^*/
  cell_selection_Info_t        cell_selection_Info;      /*^ M, 0, N, 0, 0 ^*/
  S8                           p_max;                    /*^ O, 1, N, 0, 0 ^*/
  U8                           freq_band_indicator;      /*^ M, 0, B, 1, MAX_FREQ_BAND_INDICATOR ^*/
  U8                           si_window_length;
/*^ M, 0, H, 0, 6 ^*/ /* rrm_si_window_length_et */

  U8                           si_value_tag;             /*^ M, 0, H, 0, 31 ^*/
  scheduling_info_list_t       scheduling_info_list;      /*^ M, 0, N, 0, 0 ^*/
  tdd_config_t                  tdd_config;               /*^ O, 2, N, 0, 0 ^*/
  sib_type_1_v890_ies_t         non_critical_extension;  /*^ O, 4, N, 0, 0 ^*/
}sib_type_1_Info_t;


typedef enum
{
  RRM_RRC_AC_BARRING_FACTOR_P00,
  RRM_RRC_AC_BARRING_FACTOR_P05,
  RRM_RRC_AC_BARRING_FACTOR_P10,
  RRM_RRC_AC_BARRING_FACTOR_P15,
  RRM_RRC_AC_BARRING_FACTOR_P20,
  RRM_RRC_AC_BARRING_FACTOR_P25,
  RRM_RRC_AC_BARRING_FACTOR_P30,
  RRM_RRC_AC_BARRING_FACTOR_P40,
  RRM_RRC_AC_BARRING_FACTOR_P50,
  RRM_RRC_AC_BARRING_FACTOR_P60,
  RRM_RRC_AC_BARRING_FACTOR_P70,
  RRM_RRC_AC_BARRING_FACTOR_P75,
  RRM_RRC_AC_BARRING_FACTOR_P80,
  RRM_RRC_AC_BARRING_FACTOR_P85,
  RRM_RRC_AC_BARRING_FACTOR_P90,
  RRM_RRC_AC_BARRING_FACTOR_P95
}rrm_ac_barring_factor_et;


typedef enum
{
  RRM_RRC_AC_BARRING_TIME_S4,
  RRM_RRC_AC_BARRING_TIME_S8,
  RRM_RRC_AC_BARRING_TIME_S16,
  RRM_RRC_AC_BARRING_TIME_S32,
  RRM_RRC_AC_BARRING_TIME_S64,
  RRM_RRC_AC_BARRING_TIME_S128,
  RRM_RRC_AC_BARRING_TIME_S256,
  RRM_RRC_AC_BARRING_TIME_S512
}rrm_ac_barring_time_et;

/* CDMA2000_CSFB_HO start */
typedef enum
{
  POWER_DOWN_REG_R9_TRUE__8
}csfb_registration_param_1xrtt_v920_et;
/* CDMA2000_CSFB_HO stop */


typedef struct
{
  /*CR 524: Value 0 is allowed for ac_barring_factor*/
  U8   ac_barring_factor;
/*^ M, 0, H, 0, 15 ^*/   /* rrm_ac_barring_factor_et */

  U8   ac_barring_time;
/*^ M, 0, H, 0, 7 ^*/    /* rrm_ac_barring_time_et */

/* Bug 12189 Fix Start */
  U8   ac_barring_for_special_ac;
/*^ M, 0, N, 0, 0 ^*/ /* only 5 last bits are used 7..3 */
/* Bug 12189 Fix Stop */

}access_class_barring_Info_t;




#define AC_BARR_INFO_MO_SIG_PRESENCE_FLAG       0x01
#define AC_BARR_INFO_MO_DATA_PRESENCE_FLAG      0x02

typedef struct
{
  U16                           presence_bitmask;               /*^ BITMASK ^*/
  U8                            acBarringForEmergency;
/*^ M, 0, H, 0, 1 ^*/  /* rrc_bool_et */

  access_class_barring_Info_t   ac_barring_for_mo_signalling;
/*^ O, 1, N, 0, 0 ^*/

  access_class_barring_Info_t   ac_barring_for_mo_data;
/*^ O, 2, N, 0, 0 ^*/

}access_barring_info_t;



typedef enum
{
  RRM_RRC_RA_PREAMBLE_COUNT_N4,
  RRM_RRC_RA_PREAMBLE_COUNT_N8,
  RRM_RRC_RA_PREAMBLE_COUNT_N12,
  RRM_RRC_RA_PREAMBLE_COUNT_N16,
  RRM_RRC_RA_PREAMBLE_COUNT_N20,
  RRM_RRC_RA_PREAMBLE_COUNT_N24,
  RRM_RRC_RA_PREAMBLE_COUNT_N28,
  RRM_RRC_RA_PREAMBLE_COUNT_N32,
  RRM_RRC_RA_PREAMBLE_COUNT_N36,
  RRM_RRC_RA_PREAMBLE_COUNT_N40,
  RRM_RRC_RA_PREAMBLE_COUNT_N44,
  RRM_RRC_RA_PREAMBLE_COUNT_N48,
  RRM_RRC_RA_PREAMBLE_COUNT_N52,
  RRM_RRC_RA_PREAMBLE_COUNT_N56,
  RRM_RRC_RA_PREAMBLE_COUNT_N60,
  RRM_RRC_RA_PREAMBLE_COUNT_N64
}rrm_ra_preamble_count_et;


typedef enum
{
  RRM_RRC_RA_PREAMBLE_GROUP_A_SIZE_N4,
  RRM_RRC_RA_PREAMBLE_GROUP_A_SIZE_N8,
  RRM_RRC_RA_PREAMBLE_GROUP_A_SIZE_N12,
  RRM_RRC_RA_PREAMBLE_GROUP_A_SIZE_N16,
  RRM_RRC_RA_PREAMBLE_GROUP_A_SIZE_N20,
  RRM_RRC_RA_PREAMBLE_GROUP_A_SIZE_N24,
  RRM_RRC_RA_PREAMBLE_GROUP_A_SIZE_N28,
  RRM_RRC_RA_PREAMBLE_GROUP_A_SIZE_N32,
  RRM_RRC_RA_PREAMBLE_GROUP_A_SIZE_N36,
  RRM_RRC_RA_PREAMBLE_GROUP_A_SIZE_N40,
  RRM_RRC_RA_PREAMBLE_GROUP_A_SIZE_N44,
  RRM_RRC_RA_PREAMBLE_GROUP_A_SIZE_N48,
  RRM_RRC_RA_PREAMBLE_GROUP_A_SIZE_N52,
  RRM_RRC_RA_PREAMBLE_GROUP_A_SIZE_N56,
  RRM_RRC_RA_PREAMBLE_GROUP_A_SIZE_N60
}rrm_ra_preambles_group_a_size_et;


typedef enum
{
  RRM_RRC_GROUP_A_MSG_SIZE_B56,
  RRM_RRC_GROUP_A_MSG_SIZE_B144,
  RRM_RRC_GROUP_A_MSG_SIZE_B208,
  RRM_RRC_GROUP_A_MSG_SIZE_B256
}rrm_group_a_msg_size_et;


typedef enum
{
  RRM_RRC_GROUP_B_MSG_POWER_OFFSET_MINUSINFINITY,
  RRM_RRC_GROUP_B_MSG_POWER_OFFSET_DB0,
  RRM_RRC_GROUP_B_MSG_POWER_OFFSET_DB5,
  RRM_RRC_GROUP_B_MSG_POWER_OFFSET_DB8,
  RRM_RRC_GROUP_B_MSG_POWER_OFFSET_DB10,
  RRM_RRC_GROUP_B_MSG_POWER_OFFSET_DB12,
  RRM_RRC_GROUP_B_MSG_POWER_OFFSET_DB15,
  RRM_RRC_GROUP_B_MSG_POWER_OFFSET_DB18
}rrm_group_b_msg_power_offset_et;



typedef struct
{
  U8   ra_preambles_group_a_size;
/*^ M, 0, H, 0, 14 ^*/       /* rrm_ra_preambles_group_a_size_et */

  U8   group_a_msg_size;
/*^ M, 0, H, 1, 3 ^*/       /* rrm_group_a_msg_size_et */

  U8   group_b_msg_power_offset;
/*^ M, 0, H, 1, 7 ^*/       /* rrm_group_b_msg_power_offset_et */

}preambles_group_a_config_t;




#define PREAMBLE_INFO_GROUP_A_CONFIG_PRESENCE_FLAG      0x01

typedef struct
{
    U16                          presence_bitmask;       /*^ BITMASK ^*/
    U8                           ra_preamble_count;
/*^ M, 0, H, 1, 15 ^*/  /* rrm_ra_preamble_count_et */

    preambles_group_a_config_t   preambles_group_a_config; /*^ O, 1, N, 0, 0 ^*/
}preamble_info_t;


typedef enum
{
  RRM_RRC_POWER_RAMP_STEP_DB0,
  RRM_RRC_POWER_RAMP_STEP_DB2,
  RRM_RRC_POWER_RAMP_STEP_DB4,
  RRM_RRC_POWER_RAMP_STEP_DB6
}rrm_power_ramping_step_et;


typedef enum
{
  RRM_RRC_PREAMBLE_POWER_DBM_120,
  RRM_RRC_PREAMBLE_POWER_DBM_118,
  RRM_RRC_PREAMBLE_POWER_DBM_116,
  RRM_RRC_PREAMBLE_POWER_DBM_114,
  RRM_RRC_PREAMBLE_POWER_DBM_112,
  RRM_RRC_PREAMBLE_POWER_DBM_110,
  RRM_RRC_PREAMBLE_POWER_DBM_108,
  RRM_RRC_PREAMBLE_POWER_DBM_106,
  RRM_RRC_PREAMBLE_POWER_DBM_104,
  RRM_RRC_PREAMBLE_POWER_DBM_102,
  RRM_RRC_PREAMBLE_POWER_DBM_100,
  RRM_RRC_PREAMBLE_POWER_DBM_98,
  RRM_RRC_PREAMBLE_POWER_DBM_96,
  RRM_RRC_PREAMBLE_POWER_DBM_94,
  RRM_RRC_PREAMBLE_POWER_DBM_92,
  RRM_RRC_PREAMBLE_POWER_DBM_90
}rrm_preamble_init_rec_target_pow_et;




typedef struct
{
   U8   power_ramping_step;
/*^ M, 0, H, 0, 3 ^*/ /* rrm_power_ramping_step_et */

   U8   preamble_init_rec_target_pow;
/*^ M, 0, H, 0, 15 ^*/ /* rrm_preamble_init_rec_target_pow_et */

}power_ramping_params_t;




typedef enum
{
  RRM_RRC_PREAMBLE_TRANS_MAX_N3,
  RRM_RRC_PREAMBLE_TRANS_MAX_N4,
  RRM_RRC_PREAMBLE_TRANS_MAX_N5,
  RRM_RRC_PREAMBLE_TRANS_MAX_N6,
  RRM_RRC_PREAMBLE_TRANS_MAX_N7,
  RRM_RRC_PREAMBLE_TRANS_MAX_N8,
  RRM_RRC_PREAMBLE_TRANS_MAX_N10,
  RRM_RRC_PREAMBLE_TRANS_MAX_N20,
  RRM_RRC_PREAMBLE_TRANS_MAX_N50,
  RRM_RRC_PREAMBLE_TRANS_MAX_N100,
  RRM_RRC_PREAMBLE_TRANS_MAX_N200
}rrm_preamble_trans_max_et;


typedef enum
{
  RRM_RRC_RA_RESP_WIN_SIZE_SF2,
  RRM_RRC_RA_RESP_WIN_SIZE_SF3,
  RRM_RRC_RA_RESP_WIN_SIZE_SF4,
  RRM_RRC_RA_RESP_WIN_SIZE_SF5,
  RRM_RRC_RA_RESP_WIN_SIZE_SF6,
  RRM_RRC_RA_RESP_WIN_SIZE_SF7,
  RRM_RRC_RA_RESP_WIN_SIZE_SF8,
  RRM_RRC_RA_RESP_WIN_SIZE_SF10
}rrm_ra_resp_win_size_et;



typedef enum
{
  RRM_RRC_MAC_CONT_RES_TIMER_SF8,
  RRM_RRC_MAC_CONT_RES_TIMER_SF16,
  RRM_RRC_MAC_CONT_RES_TIMER_SF24,
  RRM_RRC_MAC_CONT_RES_TIMER_SF32,
  RRM_RRC_MAC_CONT_RES_TIMER_SF40,
  RRM_RRC_MAC_CONT_RES_TIMER_SF48,
  RRM_RRC_MAC_CONT_RES_TIMER_SF56,
  RRM_RRC_MAC_CONT_RES_TIMER_SF64
}rrm_mac_cont_resol_timer_et;



typedef struct
{
  U8    preamble_trans_max;
/*^ M, 0, H, 1, 10 ^*/ /* rrm_preamble_trans_max_et */

  U8    ra_resp_win_size;
/*^ M, 0, H, 1, 7 ^*/  /* rrm_ra_resp_win_size_et */

  U8    mac_cont_resol_timer;
/*^ M, 0, H, 1, 7 ^*/  /* rrm_mac_cont_resol_timer_et */

}ra_supervision_info_t;

/*! \rach_config_common_t
 *  \brief This struct contains RACH parameters
 *  \param preamble_info Defines the RACH preamble information
 *  \param power_ramping_params Denotes increase factor between 
 subsequent random access preamble transmissions
 *  \param max_harq_msg_3_tx Denotes max no of Msg3 HARQ transmissions
 *  \param preambleTransMaxCeR13 Denotes max no of random access preamble transmissions
 *  \param rachCeLevelInfoListR13 indiactes RACH info list for each CE level
 */

/* eMTC changes start */
#define RRC_RRM_PREAMBLE_TRANS_MAX_CE_R13_PRESENT      0x01
#define RRC_RRM_RACH_CE_LEVEL_INFO_LIST_R13_PRESENT    0x02
/* eMTC changes stop */
typedef struct
{
    /* eMTC changes start */
    RrcBitmaskU8_t           bitmask;
    /*^ BITMASK ^*/
    /* eMTC changes stop */

    preamble_info_t          preamble_info;          /*^ M, 0, N, 0, 0 ^*/
    power_ramping_params_t   power_ramping_params;   /*^ M, 0, N, 0, 0 ^*/
    ra_supervision_info_t    ra_supervision_info;    /*^ M, 1, N, 0, 0 ^*/
    U8                       max_harq_msg_3_tx;      /*^ M, 0, B, 1, 8 ^*/
    /* eMTC changes start */
    U8                       preambleTransMaxCeR13;
    /*^ O, RRC_RRM_PREAMBLE_TRANS_MAX_CE_R13_PRESENT, H, 0, EMTC_PREAMBLE_TRANS_MAX_CE_MAX ^*/
    /* EmtcPreambleTransMaxCe_et */

    RachCeLevelInfoListR13_t rachCeLevelInfoList;
    /*^ O, RRC_RRM_RACH_CE_LEVEL_INFO_LIST_R13_PRESENT, N, 0, 0 ^*/
    /* eMTC changes stop */

}rach_config_common_t;



typedef enum
{
  RRM_RRC_MOD_PERIOD_COEFF_N2,
  RRM_RRC_MOD_PERIOD_COEFF_N4,
  RRM_RRC_MOD_PERIOD_COEFF_N8,
  RRM_RRC_MOD_PERIOD_COEFF_N16
}rrm_mod_period_coeff_et;


/* eMTC changes start */
#define RRC_RRM_MODIFICATION_PERIOD_COEFF_R13_PRESENT    0x01
/* eMTC changes stop */
typedef struct
{
    /* eMTC changes start */
    RrcBitmaskU8_t    bitmask;
    /*^ BITMASK ^*/
    /* eMTC changes stop */

  U8    mod_period_coeff;
/*^ M, 0, H, 1, 3 ^*/   /* rrm_modif_period_coeff_et */
    /* eMTC changes start */
    U8    modificationPeriodCoeffR13;
    /*^ O, RRC_RRM_MODIFICATION_PERIOD_COEFF_R13_PRESENT, H, 0, EMTC_MOD_PERIOD_COEFF_MAX ^*/
    /* EmtcModificationPeriodCoeff_et */
    /* eMTC changes stop */


}bcch_config_t;



typedef enum
{
  RRM_RRC_DEF_PAG_CYCLE_RF32,
  RRM_RRC_DEF_PAG_CYCLE_RF64,
  RRM_RRC_DEF_PAG_CYCLE_RF128,
  RRM_RRC_DEF_PAG_CYCLE_RF256
}rrm_default_paging_cycle_et;


typedef enum
{
  RRM_RRC_NB_FOUR_T,
  RRM_RRC_NB_TWO_T,
  RRM_RRC_NB_ONE_T,
  RRM_RRC_NB_HALF_T,
  RRM_RRC_NB_QUARTER_T,
  RRM_RRC_NB_ONE_EIGHTH_T,
  RRM_RRC_NB_ONE_SIXTEENTH_T,
  RRM_RRC_NB_ONE_THIRTY_SECOND_T
}rrm_nb_et;


typedef struct
{
  U8    default_paging_cycle;
/*^ M, 0, H, 1, 3 ^*/   /* rrm_default_paging_cycle_et */

  U8    nb;                     /*^ M, 0, H, 0, 7 ^*/   /* rrm_nb_et */
}pcch_config_t;


typedef struct
{
  U8   prach_config_index;     /*^ M, 0, H, 1, 63 ^*/
  U8   high_speed_flag;    /*^ M, 0, H, 0, 1 ^*/  /* rrc_bool_et */
/*FALSE for unrestricted*/

  U8   zero_cor_zone_config;   /*^ M, 0, H, 1, 15 ^*/
  U8   prach_freq_offset;      /*^ M, 0, H, 1, 94 ^*/
}prach_config_info_t;


typedef struct
{
  U16                   root_seq_index;     /*^ M, 0, H, 1, 837 ^*/
  prach_config_info_t   prach_config_info;  /*^ M, 0, N, 0, 0 ^*/
}prach_config_sib_t;

typedef enum
{
    RRM_RRC_HM_INTER_SF,
    RRM_RRC_HM_INTRA_AND_INTER_SF
}rrm_hopping_mode_et;

typedef struct
{
  U8     nsb;                  /*^ M, 0, B, 1, 4 ^*/
  U8     hopping_mode;
/*^ M, 0, H, 1, 1 ^*/     /* rrm_hopping_mode_et */

  U8     pusch_hopping_offset; /*^ M, 0, H, 0, 98 ^*/
  U8     enable_64_qam;        /*^ M, 0, H, 0, 1 ^*/     /* rrc_bool_et */
}pusch_config_basic_t;

typedef struct
{
  pusch_config_basic_t     pusch_config_basic;     /*^ M, 0, N, 0, 0 ^*/
  ul_ref_signals_pusch_t   ul_ref_signals_pusch;   /*^ M, 0, N, 0, 0 ^*/
}pusch_config_common_t;



typedef enum
{
  RRM_RRC_DS1,
  RRM_RRC_DS2,
  RRM_RRC_DS3
}rrm_delta_pucch_shift_et;



typedef struct
{
  U8   delta_pucch_shift;
/*^ M, 0, H, 1, 2 ^*/   /* rrm_delta_pucch_shift_et */

  U8   n_rb_cqi;               /*^ M, 0, H, 0, 98 ^*/
  U8   n_cs_an;                /*^ M, 0, H, 1, 7 ^*/
  U16  n_one_pucch_an;         /*^ M, 0, H, 1, 2047 ^*/
}pucch_config_common_t;



typedef enum
{
  RRM_RRC_SRS_BW_CONFIG_BW0,
  RRM_RRC_SRS_BW_CONFIG_BW1,
  RRM_RRC_SRS_BW_CONFIG_BW2,
  RRM_RRC_SRS_BW_CONFIG_BW3,
  RRM_RRC_SRS_BW_CONFIG_BW4,
  RRM_RRC_SRS_BW_CONFIG_BW5,
  RRM_RRC_SRS_BW_CONFIG_BW6,
  RRM_RRC_SRS_BW_CONFIG_BW7
}rrm_srs_bw_config_et;

typedef enum
{
  RRM_RRC_SRS_SUBFRAME_CONFIG_SC0,
  RRM_RRC_SRS_SUBFRAME_CONFIG_SC1,
  RRM_RRC_SRS_SUBFRAME_CONFIG_SC2,
  RRM_RRC_SRS_SUBFRAME_CONFIG_SC3,
  RRM_RRC_SRS_SUBFRAME_CONFIG_SC4,
  RRM_RRC_SRS_SUBFRAME_CONFIG_SC5,
  RRM_RRC_SRS_SUBFRAME_CONFIG_SC6,
  RRM_RRC_SRS_SUBFRAME_CONFIG_SC7,
  RRM_RRC_SRS_SUBFRAME_CONFIG_SC8,
  RRM_RRC_SRS_SUBFRAME_CONFIG_SC9,
  RRM_RRC_SRS_SUBFRAME_CONFIG_SC10,
  RRM_RRC_SRS_SUBFRAME_CONFIG_SC11,
  RRM_RRC_SRS_SUBFRAME_CONFIG_SC12,
  RRM_RRC_SRS_SUBFRAME_CONFIG_SC13,
  RRM_RRC_SRS_SUBFRAME_CONFIG_SC14,
  RRM_RRC_SRS_SUBFRAME_CONFIG_SC15
}rrm_srs_subframe_config_et;

typedef enum
{
  RRM_SRS_MAX_UP_PTS_TRUE = 0
}rrm_srs_max_up_pts_et;


typedef enum
{
  RRM_RRC_DELTAF_NEG_TWO,
  RRM_RRC_DELTAF_ZERO,
  RRM_RRC_DELTAF_POS_TWO
}rrm_delta_f_pucch_format_one_et;


typedef enum
{
  RRM_RRC_DELTA_F1,
  RRM_RRC_DELTA_F3,
  RRM_RRC_DELTA_F5
}rrm_delta_f_pucch_format_one_b_et;


typedef struct
{
  U8   delta_f_pucch_format_one;
/*^ M, 0, H, 1, 2 ^*/  /* rrm_delta_f_pucch_format_one_et */

  U8   delta_f_pucch_format_one_b;
/*^ M, 0, H, 1, 2 ^*/  /* rrm_delta_f_pucch_format_one_b_et */

}delta_f_list_pucch_t;


typedef enum
{
  RRM_RRC_FORMAT2_DELTA_NEG_TWO,
  RRM_RRC_FORMAT2_DELTA_ZERO,
  RRM_RRC_FORMAT2_DELTA_POS_ONE,
  RRM_RRC_FORMAT2_DELTA_POS_TWO
}rrm_delta_f_pucch_format_two_et;


typedef enum
{
  RRM_RRC_DELTA_NEG_TWO,
  RRM_RRC_DELTA_ZERO,
  RRM_RRC_DELTA_POS_TWO
}rrm_delta_f_pucch_format_two_ab_et;



typedef struct
{
  U8   delta_f_pucch_format_two;
/*^ M, 0, H, 1, 3 ^*/ /* rrm_delta_f_pucch_format_two_et */

  U8   delta_f_pucch_format_two_a;
/*^ M, 0, H, 1, 2 ^*/ /* rrm_delta_f_pucch_format_two_ab_et */

  U8   delta_f_pucch_format_two_b;
/*^ M, 0, H, 1, 2 ^*/ /* rrm_delta_f_pucch_format_two_ab_et */

}delta_f_pucch_format_two_t;


typedef enum
{

  RRM_RRC_AL_0,
  RRM_RRC_AL_0_4,
  RRM_RRC_AL_0_5,
  RRM_RRC_AL_0_6,
  RRM_RRC_AL_0_7,
  RRM_RRC_AL_0_8,
  RRM_RRC_AL_0_9,
  RRM_RRC_AL_1
}rrm_alpha_et;


typedef struct
{
  S8                           p_zero_nominal_Pusch;
/*^ M, 0, B, -126, 24 ^*/

  U8                           alpha;
/*^ M, 0, H, 1, 7 ^*/  /* rrm_alpha_et */

  S8                           p_zero_nominal_pucch;
/*^ M, 0, B, -127, -96 ^*/

  delta_f_list_pucch_t         delta_f_list_pucch;      /*^ M, 0, N, 0, 0 ^*/
  delta_f_pucch_format_two_t   delta_f_pucch_format_two;/*^ M, 0, N, 0, 0 ^*/
  S8                           delta_preamble_msg_three;/*^ M, 0, B, -1, 6 ^*/
}uplink_power_control_common_t;



typedef enum
{
  RRM_RRC_UL_CYC_PREFIX_LEN_1,
  RRM_RRC_UL_CYC_PREFIX_LEN_2
}rrm_ul_cyclic_prefix_len_et;


/* eMTC changes start */
#define RRC_PRACH_CONFIG_SIB_V1310_PRESENT               0x02
#define RRC_PCCH_CONFIG_V1310_PRESENT                    0x04
#define RRC_PUSCH_CONFIG_COMMON_V1310_PRESENT            0x08
#define RRC_PUCCH_CONFIG_COMMON_V1310_PRESENT            0x10
#define RRC_PDSCH_CONFIG_COMMON_V1310_PRESENT            0x20
/* eMTC changes stop */
typedef struct
{
#define RRC_CONFIG_COMMON_SIB_POWER_CONTROL_CMN_V1020 0x01
  rach_config_common_t            rach_config_common; /*^ M, 0, N, 0, 0 ^*/
  bcch_config_t                   bcch_config;        /*^ M, 0, N, 0, 0 ^*/
  pcch_config_t                   pcch_config;        /*^ M, 0, N, 0, 0 ^*/
  prach_config_sib_t              prach_config_sib;   /*^ M, 0, N, 0, 0 ^*/
  U8                              ul_cyc_prefix_len;
/*^ M, 0, H, 1, 1 ^*/  /* rrm_ul_cyclic_prefix_len_et */

  pdsch_config_common_t           pdsch_config_common;
/*^ M, 0, N, 0, 0 ^*/

  pusch_config_common_t           pusch_config_common;
/*^ M, 0, N, 0, 0 ^*/

  pucch_config_common_t           pucch_config_common;
/*^ M, 0, N, 0, 0 ^*/

  sounding_rs_ul_config_common_t  sounding_rs_ul_config_common;
/*^ M, 0, N, 0, 0 ^*/ /* M since 331.860*/

  uplink_power_control_common_t   uplink_power_control_common;
/*^ M, 0, N, 0, 0 ^*/

  U16       presence_bitmask;       /*^ BITMASK ^*/

  uplink_power_control_common_v1020_t uplink_power_control_common_v1020;
/*^ O, RRC_CONFIG_COMMON_SIB_POWER_CONTROL_CMN_V1020, N, 0, 0 ^*/
    /* eMTC changes start */
    PcchConfigV1310_t                 pcchConfigV1310;
    /*^ O, RRC_PCCH_CONFIG_V1310_PRESENT, N, 0, 0 ^*/

    PdschConfigCommonV1310_t          pdschConfigV1310;
    /*^ O, RRC_PDSCH_CONFIG_COMMON_V1310_PRESENT, N, 0, 0 ^*/

    PuschConfigCommonV1310_t          puschConfigCommonV1310;
    /*^ O, RRC_PUSCH_CONFIG_COMMON_V1310_PRESENT, N, 0, 0 ^*/

    PrachConfigV1310_t                prachConfigV1310;
    /*^ O, RRC_PRACH_CONFIG_SIB_V1310_PRESENT, N, 0, 0 ^*/

    PucchConfigCommonV1310_t          pucchConfigCommonV1310 ;
    /*^ O, RRC_PUCCH_CONFIG_COMMON_V1310_PRESENT, N, 0, 0 ^*/
    /* eMTC changes stop */

}radio_resource_config_common_sib_t;

typedef enum
{
  RRM_RRC_TIMER_300_301_MS100,
  RRM_RRC_TIMER_300_301_MS200,
  RRM_RRC_TIMER_300_301_MS300,
  RRM_RRC_TIMER_300_301_MS400,
  RRM_RRC_TIMER_300_301_MS600,
  RRM_RRC_TIMER_300_301_MS1000,
  RRM_RRC_TIMER_300_301_MS1500,
  RRM_RRC_TIMER_300_301_MS2000
}rrm_timer_300_301_et;


typedef enum
{
  RRM_RRC_TIMER_310_MS0,
  RRM_RRC_TIMER_310_MS50,
  RRM_RRC_TIMER_310_MS100,
  RRM_RRC_TIMER_310_MS200,
  RRM_RRC_TIMER_310_MS500,
  RRM_RRC_TIMER_310_MS1000,
  RRM_RRC_TIMER_310_MS2000
}rrm_timer_310_et;

typedef enum
{
  RRM_RRC_TIMER_N310_N1,
  RRM_RRC_TIMER_N310_N2,
  RRM_RRC_TIMER_N310_N3,
  RRM_RRC_TIMER_N310_N4,
  RRM_RRC_TIMER_N310_N6,
  RRM_RRC_TIMER_N310_N8,
  RRM_RRC_TIMER_N310_N10,
  RRM_RRC_TIMER_N310_N20
}rrm_timer_n310_et;

typedef enum
{
  RRM_RRC_TIMER_311_MS1000,
  RRM_RRC_TIMER_311_MS3000,
  RRM_RRC_TIMER_311_MS5000,
  RRM_RRC_TIMER_311_MS10000,
  RRM_RRC_TIMER_311_MS15000,
  RRM_RRC_TIMER_311_MS20000,
  RRM_RRC_TIMER_311_MS30000
}rrm_timer_311_et;

typedef enum
{
  RRM_RRC_TIMER_N311_N1,
  RRM_RRC_TIMER_N311_N2,
  RRM_RRC_TIMER_N311_N3,
  RRM_RRC_TIMER_N311_N4,
  RRM_RRC_TIMER_N311_N5,
  RRM_RRC_TIMER_N311_N6,
  RRM_RRC_TIMER_N311_N8,
  RRM_RRC_TIMER_N311_N10
}rrm_timer_n311_et;

typedef struct
{
  U8      timer_300;    /*^ M, 0, H, 1, 7 ^*/ /* rrm_timer_300_301_et */
  U8      timer_301;    /*^ M, 0, H, 1, 7 ^*/ /* rrm_timer_300_301_et */
  U8      timer_310;    /*^ M, 0, H, 1, 6 ^*/ /* rrm_timer_310_et */
  U8      timer_n310;    /*^ M, 0, H, 0, 7 ^*/ /* rrm_timer_n310_et */
  U8      timer_311;    /*^ M, 0, H, 1, 6 ^*/ /* rrm_timer_311_et */
  U8      timer_n311;    /*^ M, 0, H, 0, 7 ^*/ /* rrm_timer_n311_et */
}ue_timers_and_constants_t;


typedef enum
{
  RRM_RRC_UL_BW_6RB,
  RRM_RRC_UL_BW_15RB,
  RRM_RRC_UL_BW_25RB,
  RRM_RRC_UL_BW_50RB,
  RRM_RRC_UL_BW_75RB,
  RRM_RRC_UL_BW_100RB
}rrm_ul_bandwidth_et;



#define FREQ_INFO_UL_CARRER_FREQ_PRESENCE_FLAG      0x01
#define FREQ_INFO_UL_BW_PRESENCE_FLAG               0x02

typedef struct
{
  U16       presence_bitmask;       /*^ BITMASK ^*/
  U16       ul_carrier_freq;        /*^ O, 1 ^*/
  U8        ul_bandwidth;
/*^ O, 2, H, 1, 5 ^*/   /* rrm_ul_bandwidth_et */

  U8        add_spectrum_emission;  /*^ M, 0, B, 1, 32 ^*/
}freq_info_t;

#define SUBFRAME_ALLOCATION_ONE_FRAME                0X01
#define SUBFRAME_ALLOCATION_FOUR_FRAMES              0X02

typedef struct
{
  U16       presence_bitmask;                                   /*^ BITMASK ^*/
  U8        one_frame[SUB_FRAME_ALLOC_ONE_FRAME_OCTET_SIZE];
/*^ O, 1, OCTET_STRING, FIXED ^*/

  U8        four_frames[SUB_FRAME_ALLOC_FOUR_FRAME_OCTET_SIZE];
/*^ O, 2, OCTET_STRING, FIXED ^*/

}subframe_allocation_t;


typedef enum
{
  RRM_RRC_RADIO_FRAME_ALLOC_PERIOD_1,
  RRM_RRC_RADIO_FRAME_ALLOC_PERIOD_2,
  RRM_RRC_RADIO_FRAME_ALLOC_PERIOD_4,
  RRM_RRC_RADIO_FRAME_ALLOC_PERIOD_8,
  RRM_RRC_RADIO_FRAME_ALLOC_PERIOD_16,
  RRM_RRC_RADIO_FRAME_ALLOC_PERIOD_32
}rrm_radio_frame_alloc_period_et;


typedef struct
{
    U16 bitmask; /*^BITMASK^*/
#define SYNC_STRATUM_ID_PRESENCE_FLAG    0x01
/*SPR 18551 : Start*/
  U8      radio_frame_alloc_period;
/*^ M, 0, H, 0, 5 ^*/ /* rrm_radio_frame_alloc_period_et */

  U8      radio_frame_alloc_offset;     /*^ M, 0, H, 0, 7 ^*/
/*SPR 18551 : End*/
  subframe_allocation_t      subframe_allocation;  /*^ M, 0, N, 0, 0 ^*/
    U8      sync_stratum_id; /*^ O, SYNC_STRATUM_ID_PRESENCE_FLAG, H, 1, 8 ^*/
}mbsfn_subframe_config_t;



typedef struct
{
/*SPR_17664_start*/
  U8                        count;                  /*^ M, 0, H, 1, MAX_MBSFN_SUBFRAME_CONFIG ^*/
/*SPR_17664_end*/
  mbsfn_subframe_config_t   mbsfn_subframe_config[MAX_MBSFN_SUBFRAME_CONFIG];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}mbsfn_subframe_config_list_t;



typedef enum
{
  RRM_RRC_TIME_ALLIGN_TIMER_SF_500,
  RRM_RRC_TIME_ALLIGN_TIMER_SF_750,
  RRM_RRC_TIME_ALLIGN_TIMER_SF_1280,
  RRM_RRC_TIME_ALLIGN_TIMER_SF_1920,
  RRM_RRC_TIME_ALLIGN_TIMER_SF_2560,
  RRM_RRC_TIME_ALLIGN_TIMER_SF_5120,
  RRM_RRC_TIME_ALLIGN_TIMER_SF_10240,
  RRM_RRC_TIME_ALLIGN_TIMER_INFINITY
}rrm_time_align_timer_et;


/* CR 970 changes start*/  
typedef struct
{
/* SPR-18445 start */
    U8  count;  /*^M, 0, B, 1, MAX_MULTI_BANDS^*/
/* SPR-18445 stop */

    U8  add_spectrum_emission[MAX_MULTI_BANDS];  /*^ M, 0, OCTET_STRING, VARIABLE, B, 1, 32 ^*/

}sib_2_multi_band_info_list_t;  

typedef struct
{   
    U16 bitmask; /*^BITMASK^*/
#define SIB_2_MULTI_BAND_INFO_LIST_PRESENCE_FLAG    0x01

    sib_2_multi_band_info_list_t    multi_band_info_list;
/*^ O, SIB_2_MULTI_BAND_INFO_LIST_PRESENCE_FLAG , N, 0, 0 ^*/    

}sib_type_2_v8h0_ies_t;
/* CR 970 changes stop*/  

#define SIB2_ACCESS_BARR_INFO_PRESENCE_FLAG                         0x01
#define SIB2_MBSFN_SUBFRAME_CONF_LIST_PRESENCE_FLAG                 0x02
/*cr_345 macro */
#define SIB2_SSAC_BARRING_FOR_MMTEL_VOICE_PRESENCE_FLAG             0x04
#define SIB2_SSAC_BARRING_FOR_MMTEL_VIDEO_PRESENCE_FLAG             0x08
/*cr_477 macro */
#define SIB2_AC_BARRING_FOR_CSFB_PRESENCE_FLAG                      0x10
/* CR 970 changes start*/  
#define SIB2_LATE_NON_CRITICAL_EXTENSION_PRESENT                    0x20
/* CR 970 changes stop*/  

typedef struct
{
  U16                                 presence_bitmask;
/*^ BITMASK ^*/

  access_barring_info_t               access_barring_info;
/*^ O, 1, N, 0, 0 ^*/

  radio_resource_config_common_sib_t  radio_resource_config_common_sib;
/*^ M, 0, N, 0, 0 ^*/

  ue_timers_and_constants_t           ue_timers_and_constants;
/*^ M, 0, N, 0, 0 ^*/

  freq_info_t                         freq_info;
/*^ M, 0, N, 0, 0 ^*/

  mbsfn_subframe_config_list_t        mbsfn_subframe_config_list;
/*^ O, 2, N, 0, 0 ^*/

  U8                                  time_align_timer;
/*^ M, 0, H, 0, 7 ^*/ /* rrm_time_align_timer_et */

  /*cr_345 new structures*/
  access_class_barring_Info_t          ssac_barring_for_mmtel_voice_r9;
/*^ O, 4, N, 0, 0 ^*/
  
  access_class_barring_Info_t          ssac_barring_for_mmtel_video_r9;
/*^ O, 8, N, 0, 0 ^*/
  
  /*cr_477 new structures*/
  access_class_barring_Info_t          ac_barring_for_csfb_r10;
/*^ O, 16, N, 0, 0 ^*/
    
/* CR 970 changes start*/  
  sib_type_2_v8h0_ies_t                sib_type_2_v8h0_ies;
/*^ O, 32, N, 0, 0 ^*/
/* CR 970 changes stop*/

}sib_type_2_Info_t;
typedef enum
{
  RRM_RRC_Q_HYST_DB0,
  RRM_RRC_Q_HYST_DB1,
  RRM_RRC_Q_HYST_DB2,
  RRM_RRC_Q_HYST_DB3,
  RRM_RRC_Q_HYST_DB4,
  RRM_RRC_Q_HYST_DB5,
  RRM_RRC_Q_HYST_DB6,
  RRM_RRC_Q_HYST_DB8,
  RRM_RRC_Q_HYST_DB10,
  RRM_RRC_Q_HYST_DB12,
  RRM_RRC_Q_HYST_DB14,
  RRM_RRC_Q_HYST_DB16,
  RRM_RRC_Q_HYST_DB18,
  RRM_RRC_Q_HYST_DB20,
  RRM_RRC_Q_HYST_DB22,
  RRM_RRC_Q_HYST_DB24
}rrm_q_hyst_et;

typedef enum
{
  RRM_RRC_T_EVAL_S_30,
  RRM_RRC_T_EVAL_S_60,
  RRM_RRC_T_EVAL_S_120,
  RRM_RRC_T_EVAL_S_180,
  RRM_RRC_T_EVAL_S_240
}rrm_t_evaluation_et;


typedef enum
{
  RRM_RRC_T_HYST_NORMAL_S_30,
  RRM_RRC_T_HYST_NORMAL_S_60,
  RRM_RRC_T_HYST_NORMAL_S_120,
  RRM_RRC_T_HYST_NORMAL_S_180,
  RRM_RRC_T_HYST_NORMAL_S_240
}rrm_t_hyst_normal_et;



typedef struct
{
  U8        t_evaluation;
/*^ M, 0, H, 1, 4 ^*/   /* rrm_t_evaluation_et */

  U8        t_hyst_normal;
/*^ M, 0, H, 1, 4 ^*/   /* rrm_t_hyst_normal_et */

  U8        n_cell_change_medium;   /*^ M, 0, B, 1, 16 ^*/
  U8        n_cell_change_high;     /*^ M, 0, B, 1, 16 ^*/
}mobility_state_params_t;

typedef enum
{
  RRM_RRC_Q_HYST_NEG_SIX,
  RRM_RRC_Q_HYST_NEG_FOUR,
  RRM_RRC_Q_HYST_NEG_TWO,
  RRM_RRC_Q_HYST_ZERO
}rrm_q_hyst_sf_et;

typedef struct
{
  U8        q_hyst_sf_medium;  /*^ M, 0, H, 1, 6 ^*/ /* rrm_q_hyst_sf_et */
  U8        q_hyst_sf_high;    /*^ M, 0, H, 1, 6 ^*/ /* rrm_q_hyst_sf_et */
}speed_depend_scaling_param_hyst_t;


typedef struct
{
  mobility_state_params_t           mobility_state_params;
/*^ M, 0, N, 0, 0 ^*/

  speed_depend_scaling_param_hyst_t speed_depend_scaling_param_hyst;
/*^ M, 0, N, 0, 0 ^*/

}speed_depend_reselect_t;


#define CELL_RESELECT_SPEED_DEPEND_RESELECT_PRESENCE_FLAG       0x01

typedef struct
{
  U16                      presence_bitmask;        /*^ BITMASK ^*/
  U8                       q_hyst;
/*^ M, 0, H, 1, 15 ^*/ /* rrm_q_hyst_et */

  speed_depend_reselect_t  speed_depend_reselect;   /*^ O, 1, N, 0, 0 ^*/
}cell_reselect_info_comm_t;


#define CELL_RESELECT_NON_INTRA_SEARCH_PRESENCE_FLAG            0x01

typedef struct
{
  U16       presence_bitmask;       /*^ BITMASK ^*/
  U8        s_non_intra_search;     /*^ O, 1, H, 1, 31 ^*/
  U8        thresh_serving_low;     /*^ M, 0, H, 1, 31 ^*/
  U8        cell_reselect_priority; /*^ M, 0, H, 1, 7 ^*/
}cell_reselect_serv_freq_info_t;

typedef U8 cell_resel_priority_t;

typedef U8 resel_threshold_t;

typedef U8 t_reselection_t;

typedef enum
{
  RRM_RRC_M_BW_6,
  RRM_RRC_M_BW_15,
  RRM_RRC_M_BW_25,
  RRM_RRC_M_BW_50,
  RRM_RRC_M_BW_75,
  RRM_RRC_M_BW_100
}rrm_measure_bw_et;

typedef enum
{
    RRM_RRC_O_DOT_25,
    RRM_RRC_O_DOT_5,
    RRM_RRC_O_DOT_75,
    RRM_RRC_l_DOT_0
} rrm_speed_state_scale_factors_et;

typedef struct
{
    U8  sf_medium; /*^ M, 0, H, 0, 3 ^*/  /* rrm_speed_state_scale_factors_et */
    U8  sf_high;   /*^ M, 0, H, 0, 3 ^*/  /* rrm_speed_state_scale_factors_et */
} speed_state_scale_factors_t;



#define INTRA_FREQ_CELL_RESELECT_P_MAX_PRESENCE_FLAG            0x01
#define INTRA_FREQ_CELL_RESELECT_INTRA_SEARCH_PRESENCE_FLAG     0x02
#define INTRA_FREQ_CELL_RESELECT_MEAS_BW_PRESENCE_FLAG          0x04
#define INTRA_FREQ_CELL_RESELECT_EUTRAN_SF_PRESENCE_FLAG        0x08

typedef struct
{
  U16       presence_bitmask;           /*^ BITMASK ^*/
  S8        q_rx_lev_min;               /*^ M, 0, B, -70, -22 ^*/
  S8        p_max;                      /*^ O, 1, B, -30, 33 ^*/
  U8        s_intra_search;             /*^ O, 2, H, 1, 31 ^*/
  U8        measure_bw;
/*^ O, 4, H, 1, 5 ^*/   /* rrm_measure_bw_et */

  U8        presence_antenna_port_1;    /*^ M, 0, H, 0, 1 ^*/
  U8        neigh_cell_config;          /*^ M, 0, N, 0, 0 ^*/
  U8        t_reselec_eutra;            /*^ M, 0, H, 1, 7 ^*/
  speed_state_scale_factors_t  t_reselect_eutra_sf; /*^ O, 8, N, 0, 0 ^*/
}intra_freq_cell_reselect_info_t;

/* cr_398 structure */
typedef struct
{
  U8        s_intra_search_p_r9;             /*^ M, 0, H, 0, 31 ^*/
  U8        s_intra_search_q_r9;             /*^ M, 0, H, 0, 31 ^*/
}sib3_intra_search_v920_t;

typedef struct
{
  U8        s_non_intra_search_p_r9;         /*^ M, 0, H, 0, 31 ^*/
  U8        s_non_intra_search_q_r9;         /*^ M, 0, H, 0, 31 ^*/
}sib3_non_intra_search_v920_t;

typedef struct
{
/*cr_398 macro and structure */
  U16       presence_bitmask;           /*^ BITMASK ^*/
#define SIB_3_INTRA_SEARCH_V920_PRESENCE_FLAG         0x01
#define SIB_3_NON_INTRA_SEARCH_V920_PRESENCE_FLAG     0x02
#define SIB_3_Q_QUAL_MIN_R9_PRESENCE_FLAG             0x04
#define SIB_3_THRESH_SERVING_LOW_Q_R9_PRESENCE_FLAG   0x08
  
  cell_reselect_info_comm_t       timecell_reselect_info_comm;
/*^ M, 0, N, 0, 0 ^*/

  cell_reselect_serv_freq_info_t  cell_reselect_serv_freq_info;
/*^ M, 0, N, 0, 0 ^*/

  intra_freq_cell_reselect_info_t intra_freq_cell_reselect_info;
/*^ M, 0, N, 0, 0 ^*/

  sib3_intra_search_v920_t        sib3_intra_search_v920;
/*^ O, SIB_3_INTRA_SEARCH_V920_PRESENCE_FLAG, N, 0, 0 ^*/

  sib3_non_intra_search_v920_t    sib3_non_intra_search_v920;
/*^ O, SIB_3_NON_INTRA_SEARCH_V920_PRESENCE_FLAG, N, 0, 0 ^*/
    
  S8                              q_qual_min_r9; 
/*^ O, SIB_3_Q_QUAL_MIN_R9_PRESENCE_FLAG, B, -34, -3 ^*/

  U8                              thresh_serving_low_q_r9;  
/*^ O, SIB_3_THRESH_SERVING_LOW_Q_R9_PRESENCE_FLAG, H, 0, 31 ^*/

}sib_type_3_Info_t;


typedef enum
{
  RRM_RRC_Q_OFFESET_RANGE_DB_24,
  RRM_RRC_Q_OFFESET_RANGE_DB_22,
  RRM_RRC_Q_OFFESET_RANGE_DB_20,
  RRM_RRC_Q_OFFESET_RANGE_DB_18,
  RRM_RRC_Q_OFFESET_RANGE_DB_16,
  RRM_RRC_Q_OFFESET_RANGE_DB_14,
  RRM_RRC_Q_OFFESET_RANGE_DB_12,
  RRM_RRC_Q_OFFESET_RANGE_DB_10,
  RRM_RRC_Q_OFFESET_RANGE_DB_8,
  RRM_RRC_Q_OFFESET_RANGE_DB_6,
  RRM_RRC_Q_OFFESET_RANGE_DB_5,
  RRM_RRC_Q_OFFESET_RANGE_DB_4,
  RRM_RRC_Q_OFFESET_RANGE_DB_3,
  RRM_RRC_Q_OFFESET_RANGE_DB_2,
  RRM_RRC_Q_OFFESET_RANGE_DB_1,
  RRM_RRC_Q_OFFESET_RANGE_DB0,
  RRM_RRC_Q_OFFESET_RANGE_DB1,
  RRM_RRC_Q_OFFESET_RANGE_DB2,
  RRM_RRC_Q_OFFESET_RANGE_DB3,
  RRM_RRC_Q_OFFESET_RANGE_DB4,
  RRM_RRC_Q_OFFESET_RANGE_DB5,
  RRM_RRC_Q_OFFESET_RANGE_DB6,
  RRM_RRC_Q_OFFESET_RANGE_DB8,
  RRM_RRC_Q_OFFESET_RANGE_DB10,
  RRM_RRC_Q_OFFESET_RANGE_DB12,
  RRM_RRC_Q_OFFESET_RANGE_DB14,
  RRM_RRC_Q_OFFESET_RANGE_DB16,
  RRM_RRC_Q_OFFESET_RANGE_DB18,
  RRM_RRC_Q_OFFESET_RANGE_DB20,
  RRM_RRC_Q_OFFESET_RANGE_DB22,
  RRM_RRC_Q_OFFESET_RANGE_DB24
}rrm_q_offset_range_et;


typedef struct
{
  rrc_phys_cell_id_t   phys_cell_id;    /*^ M, 0, H, 0, 503 ^*/
  U8                   q_offset_cell;
/*^ M, 0, H, 0, 30 ^*/   /* rrm_q_offset_range_et */

}neigh_cell_t;


typedef struct
{
/*SPR_17664_start*/
  U8            count;                      /*^ M, 0, H, 1, MAX_CELL_INTRA ^*/
/*SPR_17664_end*/
  neigh_cell_t  neigh_cell[MAX_CELL_INTRA]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}neigh_cell_list_t;


typedef enum
{
  RRM_RRC_RANGE_PCI_N_4,
  RRM_RRC_RANGE_PCI_N_8,
  RRM_RRC_RANGE_PCI_N_12,
  RRM_RRC_RANGE_PCI_N_16,
  RRM_RRC_RANGE_PCI_N_24,
  RRM_RRC_RANGE_PCI_N_32,
  RRM_RRC_RANGE_PCI_N_48,
  RRM_RRC_RANGE_PCI_N_64,
  RRM_RRC_RANGE_PCI_N_84,
  RRM_RRC_RANGE_PCI_N_96,
  RRM_RRC_RANGE_PCI_N_128,
  RRM_RRC_RANGE_PCI_N_168,
  RRM_RRC_RANGE_PCI_N_252,
  RRM_RRC_RANGE_PCI_N_504
}rrm_phy_cell_id_range_et;


#define PHY_CELL_ID_RANGE    0x01

typedef struct
{
  U16                 presence_bitmask; /*^ BITMASK ^*/
  rrc_phys_cell_id_t  start;            /*^ M, 0, H, 0, 503 ^*/
  U8                  range;
/*^ O, PHY_CELL_ID_RANGE, H, 0, 13 ^*/ /* rrm_phy_cell_id_range_et */

}phy_cell_id_range_t;



typedef struct
{
/*SPR_17664_start*/
  U8                    count;            /*^ M, 0, H, 1, MAX_CELL_BLACK_LIST ^*/
/*SPR_17664_end*/
  phy_cell_id_range_t   black_listed_cell[MAX_CELL_BLACK_LIST];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}black_listed_cell_list_t;



#define SIB_4_NEIGH_CELL_LIST_PRESENCE_FLAG         0x01
#define SIB_4_BLACK_LIST_LIST_PRESENCE_FLAG         0x02
#define SIB_4_PHY_CELL_ID_PRESENCE_FLAG             0x04


typedef struct
{
  U16                       presence_bitmask;                  /*^ BITMASK ^*/
  neigh_cell_list_t         intra_freq_neigh_cell_list;
/*^ O, 1, N, 0, 0 ^*/

  black_listed_cell_list_t  intra_freq_black_listed_cell_list;
/*^ O, 2, N, 0, 0 ^*/

  phy_cell_id_range_t       csg_phy_cell_id_range;
/*^ O, 4, N, 0, 0 ^*/

}sib_type_4_Info_t;

/*cr_398 structure */
typedef struct
{
  U8                        thresh_x_high_q_r9; /*^ M, 0, H, 0, 31 ^*/
  U8                        thresh_x_low_q_r9; /*^ M, 0, H, 0, 31 ^*/
}thresh_x_q_r9_t;

#define INTER_FREQ_CARR_P_MAX_PRESENCE_FLAG                     0x01
#define INTER_FREQ_CARR_SCALE_PARAM_PRESENCE_FLAG               0x02
#define INTER_FREQ_CARR_CELL_RESELCT_PRIOR_PRESENCE_FLAG        0x04
#define INTER_FREQ_CARR_NEIGH_CELL_LIST_PRESENCE_FLAG           0x08
#define INTER_FREQ_CARR_BLACK_LIST_CELL_LIST_PRESENCE_FLAG      0x10
#define INTER_FREQ_CARR_Q_QUAL_MIN_R9_PRESENCE_FLAG             0x20
#define INTER_FREQ_THRESH_X_Q_R9_PRESENCE_FLAG                  0x40

typedef struct
{
  U16                             presence_bitmask;         /*^ BITMASK ^*/

  U32                             eutra_dl_carrier_freq;    /*^ M, 0 ^*/
  S8                              qrx_lev_min;
/*^ M, 0, B, -70, -22 ^*/

  S8                              p_max;
/*^ O, 1, B, -30, 33 ^*/

  U8                              t_reselection_eutran;
/*^ M, 0, H, 1, 7 ^*/

  speed_state_scale_factors_t     speed_depend_scal_params;
/*^ O, 2, N, 0, 0 ^*/

  U8                              thresh_x_high;
/*^ M, 0, H, 1, 31 ^*/

  U8                              thresh_x_low;
/*^ M, 0, H, 1, 31 ^*/

  U8                              measurement_bandwidth;
/*^ M, 0, H, 1, 5 ^*/ /* rrm_band_width_et */

  U8                              presence_antenna_port_1;
/*^ M, 0, H, 0, 1 ^*/ /* OM: new field added according to TS 36.331-860 */

  U8                              cell_reselect_priority;
/*^ O, 4, H, 1, 7 ^*/

/* Bug 12189 Fix Start */
  U8                              neigh_cell_config;
/*^ M, 0, N, 0, 0 ^*/
/* Bug 12189 Fix Stop */

  U8                              q_offset_freq;
/*^ M, 0, H, 0, 30 ^*/  /* rrm_q_offset_range_et */

  neigh_cell_list_t               inter_freq_neigh_cell_list;
/*^ O, 8, N, 0, 0 ^*/

  black_listed_cell_list_t        inter_freq_black_listed_cell_list;
/*^ O, 16, N, 0, 0 ^*/

/* cr_398 new IE */
  S8                              q_qual_min_r9;
/*^ O, 32, B, -34, -3 ^*/

  thresh_x_q_r9_t                 thresh_x_q_r9;
/*^ O, 64, N, 0, 0 ^*/
}inter_freq_carrier_freq_t;


typedef struct
{
/*SPR_17664_start*/
  U8                              count;  /*^ M, 0, H, 1, MAX_EUTRA_CARRIER_FREQ ^*/
/*SPR_17664_end*/
  inter_freq_carrier_freq_t
      inter_freq_carrier_freq_list[MAX_EUTRA_CARRIER_FREQ];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}inter_freq_carrier_freq_list_list_t;

/*CR 970 changes start*/

typedef struct
{   
    U16     bitmask;    /*^ BITMASK ^*/
#define INTER_FREQ_CARRIER_FREQ_v8h0_MULTI_BAND_INFO_LIST_PRESENT 0x01

    multi_band_info_list_t      multi_band_info_list;
    /*^ O, INTER_FREQ_CARRIER_FREQ_v8h0_MULTI_BAND_INFO_LIST_PRESENT, N, 0, 0 ^*/
}inter_freq_carrier_freq_info_v8h0_t;

typedef struct
{
/* SPR-18445 start */
    U8 count; /*^ M, 0, B, 1, MAX_FREQ^*/
/* SPR-18445 stopt */

    inter_freq_carrier_freq_info_v8h0_t inter_freq_carrier_freq_info_v8h0[MAX_FREQ];
    /*^ M, 0, OCTET_STRING, VARIABLE , N, 0, 0 ^*/
}inter_freq_carrier_freq_list_v8h0_t;

typedef struct
{
    U16     bitmask;    /*^ BITMASK ^*/
#define SIB_5_v8h0_INTER_FREQ_CARRIER_FREQ_LIST_PRESENT 0x01

    inter_freq_carrier_freq_list_v8h0_t  inter_freq_carrier_freq_list_v8h0;
    /*^ O, SIB_5_v8h0_INTER_FREQ_CARRIER_FREQ_LIST_PRESENT, N, 0 ,0^*/

}sib_5_v8h0_ies_t;
/*CR 970 changes stop*/

typedef struct
{
    inter_freq_carrier_freq_list_list_t   inter_freq_carrier_freq_list_list;
    /*^ M, 0, N, 0, 0 ^*/
    
    /*CR 970 changes start*/
    U16     bitmask;    /*^ BITMASK ^*/
#define SIB_TYPE_5_v8h0_IES_PRESENT     0x01

    sib_5_v8h0_ies_t    sib_5_v8h0_ies;
    /*^ O, SIB_TYPE_5_v8h0_IES_PRESENT, N, 0, 0^*/
    /*CR 970 changes stop*/

}sib_type_5_Info_t;



typedef U16  arfcn_value_utra_t;

#define CR_FREQ_UTRA_FDD_CELL_RESEL_PRI_PRESENCE_FLAG             0x01
#define CR_FREQ_UTRA_FDD_THRESH_X_Q_R9_PRESENCE_FLAG              0x02

typedef struct
{
    U16                       presence_bitmask;          /*^ BITMASK ^*/
    arfcn_value_utra_t        carrier_freq;
/*^ M, 0, H, 0, 16383 ^*/

    cell_resel_priority_t     cell_reselection_priority;
/*^ O, CR_FREQ_UTRA_FDD_CELL_RESEL_PRI_PRESENCE_FLAG, H, 0, 7 ^*/

    resel_threshold_t         thresh_x_high;             /*^ M, 0, H, 0, 31 ^*/
    resel_threshold_t         thresh_x_low;              /*^ M, 0, H, 0, 31 ^*/
    S8                        q_rx_lev_min;
/*^ M, 0, B, -60, -13 ^*/

    S8                        p_max_utra;
/*^ M, 0, B, -50, 33 ^*/

    S8                        q_qual_min;                /*^ M, 0, B, -24, 0 ^*/

/*cr_398 new structure*/  
    thresh_x_q_r9_t           thresh_x_q_r9;
/*^ O, CR_FREQ_UTRA_FDD_THRESH_X_Q_R9_PRESENCE_FLAG, N, 0, 0 ^*/
} carrier_freq_utra_fdd_t;

typedef struct
{
/*SPR_17664_start*/
    U8                       count;                      /*^ M, 0, B, 1, MAX_UTRA_FDD_CARRIER ^*/
/*SPR_17664_end*/
    carrier_freq_utra_fdd_t  data[MAX_UTRA_FDD_CARRIER];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/  /*maxUTRA-FDD-Carrier*/

} carrier_freq_list_utra_fdd_t;

#define CR_FREQ_UTRA_TDD_CELL_RESEL_PRI_PRESENCE_FLAG    0x01

typedef struct
{
    U16                       presence_bitmask;          /*^ BITMASK ^*/
    arfcn_value_utra_t        carrier_freq;
/*^ M, 0, H, 0, 16383 ^*/

    cell_resel_priority_t     cell_reselection_priority;
/*^ O, CR_FREQ_UTRA_TDD_CELL_RESEL_PRI_PRESENCE_FLAG, H, 0, 7 ^*/

    resel_threshold_t         thresh_x_high;             /*^ M, 0, H, 0, 31 ^*/
    resel_threshold_t         thresh_x_low;              /*^ M, 0, H, 0, 31 ^*/
    S8                        q_rx_lev_min;
/*^ M, 0, B, -60, -13 ^*/

    S8                        p_max_utra;
/*^ M, 0, B, -50, 33 ^*/

} carrier_freq_utra_tdd_t;

typedef struct
{
/*SPR_17664_start*/
    U8                       count;                      /*^ M, 0, B, 1, MAX_UTRA_TDD_CARRIER ^*/
/*SPR_17664_end*/
    carrier_freq_utra_tdd_t  data[MAX_UTRA_TDD_CARRIER];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/  /*maxUTRA-TDD-Carrier*/

} carrier_freq_list_utra_tdd_t;

/*CR 970 changes start*/
typedef struct
{
/* SPR-18445 start */
    U8 count; /*^ M , 0, B, 1, MAX_MULTI_BANDS^*/
/* SPR-18445 stop */

    U8 freq_band_indicator_utra_fdd[MAX_MULTI_BANDS]; /*^ M, 0, OCTET_STRING, VARIABLE, B, 1, 86^*/
}utra_fdd_v8h0_multi_band_info_list_t;

typedef struct
{
    U16     bitmask;    /*^ BITMASK ^*/
#define CARRIER_FREQ_INFO_UTRA_FDD_v8h0_MULTI_BAND_INFO_LIST_PRESENT    0x01
    
    utra_fdd_v8h0_multi_band_info_list_t  multi_band_info_list;
/*^ O, CARRIER_FREQ_INFO_UTRA_FDD_v8h0_MULTI_BAND_INFO_LIST_PRESENT, N, 0, 0 ^*/

}carrier_freq_info_utra_fdd_v8h0_t;

typedef struct
{
/*SPR_17664_start*/
    U8  count; /*^ M, 0, B, 1, MAX_UTRA_FDD_CARRIER^*/
/*SPR_17664_end*/
    carrier_freq_info_utra_fdd_v8h0_t   carrier_freq_info_utra_fdd_v8h0[MAX_UTRA_FDD_CARRIER];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}carrier_freq_list_utra_fdd_v8h0_t;

typedef struct
{
    U16     bitmask;    /*^ BITMASK ^*/
#define SIB_6_v8h0_CARRIER_FREQ_LIST_UTRA_FDD_PRESENT     0x01
    
    carrier_freq_list_utra_fdd_v8h0_t   carrier_freq_list_utra_fdd_v8h0;
/*^ O, SIB_6_v8h0_CARRIER_FREQ_LIST_UTRA_FDD_PRESENT, N, 0, 0 ^*/    

}sib_6_v8h0_ies_t;
/*CR 970 changes stop*/

#define SIB_6_CARRIER_FREQ_LIST_UTRA_FDD_PRESENCE_FLAG    0x01
#define SIB_6_CARRIER_FREQ_LIST_UTRA_TDD_PRESENCE_FLAG    0x02
#define SIB_6_T_RESELECTION_UTRA_SF_PRESENCE_FLAG         0x04
    /*CR 970 changes start*/
#define SIB_6_LATE_NON_CRITICAL_EXTENSION_PRESENCE_FLAG   0x08
    /*CR 970 changes stop*/
typedef struct
{
    U16                          presence_bitmask;           /*^ BITMASK ^*/
    carrier_freq_list_utra_fdd_t carrier_freq_list_utra_fdd;
/*^ O, SIB_6_CARRIER_FREQ_LIST_UTRA_FDD_PRESENCE_FLAG, N, 0, 0 ^*/

    carrier_freq_list_utra_tdd_t carrier_freq_list_utra_tdd;
/*^ O, SIB_6_CARRIER_FREQ_LIST_UTRA_TDD_PRESENCE_FLAG, N, 0, 0 ^*/

    t_reselection_t              t_resel_utra;
/*^ M, 0, H, 0, 7 ^*/

    speed_state_scale_factors_t  t_resel_utra_sf;
/*^ O, SIB_6_T_RESELECTION_UTRA_SF_PRESENCE_FLAG, N, 0, 0 ^*/

    /*CR 970 changes start*/
    sib_6_v8h0_ies_t             sib_6_v8h0_ies;
/*^ O, SIB_6_LATE_NON_CRITICAL_EXTENSION_PRESENCE_FLAG, N, 0, 0 ^*/     
    /*CR 970 changes stop*/

}sib_type_6_Info_t;


typedef  U16 arfcn_value_geran_t;

typedef enum
{
    RRM_RRC_DCS_1800,
    RRM_RRC_PCS_1900
} rrm_band_indicator_geran_et;

typedef struct
{
/*SPR_17664_start*/
    U8                  count;                 /*^ M, 0, H, 0, MAX_EXPL_ARFCNS ^*/
/*SPR_17664_end*/
    arfcn_value_geran_t data[MAX_EXPL_ARFCNS];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

} explicit_list_arfcns_t;

typedef struct
{
    U8  arfcn_spacing;          /*^ M, 0, B, 1, 8 ^*/
    U8  num_of_following_arfcns;/*^ M, 0, H, 0, 31 ^*/
} equally_spaced_arfcns_t;

typedef struct
{
/*SPR_17664_start*/
    U8        count;                          /*^ M, 0, B, 1, MAX_VAR_BITMAP_OF_ARFCNS ^*/
/*SPR_17664_end*/
    U8        data[MAX_VAR_BITMAP_OF_ARFCNS];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

} var_bitmap_of_arfcns_t;

#define GERAN_EXPL_LIST_OF_ARFCNS_PRESENCE_FLAG    0x01
#define GERAN_EQ_SPACED_ARFCNS_PRESENCE_FLAG       0x02
#define GERAN_VAR_BITMAP_ARFCNS_PRESENCE_FLAG      0x04

/*CHOICE*/
typedef struct
{
    U16                      presence_bitmask;       /*^ BITMASK ^*/
    explicit_list_arfcns_t   explicit_list_of_arfcns;
/*^ O, GERAN_EXPL_LIST_OF_ARFCNS_PRESENCE_FLAG, N, 0, 0 ^*/

    equally_spaced_arfcns_t  equally_spaced_arfcns;
/*^ O, GERAN_EQ_SPACED_ARFCNS_PRESENCE_FLAG, N, 0, 0 ^*/

    var_bitmap_of_arfcns_t   var_bitmap_of_arfcns;
/*^ O, GERAN_VAR_BITMAP_ARFCNS_PRESENCE_FLAG, N, 0, 0 ^*/

} geran_following_arfcns_t;

typedef struct
{
    arfcn_value_geran_t       starting_arfcn; /*^ M, 0, H, 0, 1023 ^*/
    U8                        band_indicator;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_rrm_carrier_freq_geran_band_indicator_et */

    geran_following_arfcns_t  following_arfcns;
} carrier_freqs_geran_t;




#define CRFI_GERAN_CELL_RESEL_PRI_PRESENCE_FLAG 0x01
#define CRFI_GERAN_P_MAX_GERAN_PRESENCE_FLAG    0x02

typedef struct
{
    U16                   presence_bitmask;          /*^ BITMASK ^*/
    cell_resel_priority_t cell_reselection_priority;
/*^ O, CRFI_GERAN_CELL_RESEL_PRI_PRESENCE_FLAG, H, 0, 7 ^*/

    U8                    ncc_permitted[NCC_PERMITTED_OCTET_SIZE];
/*^ M, 0, OCTET_STRING, FIXED ^*/

    U8                    q_rx_lev_min;              /*^ M, 0, H, 0, 45 ^*/
    U8                    p_max_geran;
/*^ O, CRFI_GERAN_P_MAX_GERAN_PRESENCE_FLAG, H, 0, 39 ^*/

    resel_threshold_t     thresh_x_high;             /*^ M, 0, H, 0, 31 ^*/
    resel_threshold_t     thresh_x_low;              /*^ M, 0, H, 0, 31 ^*/
} crfi_geran_common_info_t;

typedef struct
{
    carrier_freqs_geran_t     carrier_freqs;
    crfi_geran_common_info_t  common_info;
} carrier_freqs_info_geran_t;

typedef struct
{
/*SPR_17664_start*/
    U8                         count;                  /*^ M, 0, H, 0, MAX_GNFG ^*/
/*SPR_17664_end*/
    carrier_freqs_info_geran_t cr_freq_info[MAX_GNFG];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/  /*ASN1V_maxGNFG*/

} carrier_freqs_info_list_geran_t;


#define SIB_7_T_RESEL_GERAN_SF_PRESENCE_FLAG  0x01
#define SIB_7_CR_FREQS_INFO_LST_PRESENCE_FLAG 0x02

typedef struct
{
    U16                              presence_bitmask;      /*^ BITMASK ^*/
    U8                               t_reselection_geran;
/*^ M, 0, H, 0, 7 ^*/

    speed_state_scale_factors_t      t_reselect_geran_sf;
/*^ O, SIB_7_T_RESEL_GERAN_SF_PRESENCE_FLAG, N, 0, 0 ^*/

    carrier_freqs_info_list_geran_t  carrier_freqs_info_lst;
/*^ O, SIB_7_CR_FREQS_INFO_LST_PRESENCE_FLAG, N, 0, 0 ^*/

}sib_type_7_Info_t;


typedef enum
{
    RRM_RRC_BAND_CLASS_BC_0,
    RRM_RRC_BAND_CLASS_BC_1,
    RRM_RRC_BAND_CLASS_BC_2,
    RRM_RRC_BAND_CLASS_BC_3,
    RRM_RRC_BAND_CLASS_BC_4,
    RRM_RRC_BAND_CLASS_BC_5,
    RRM_RRC_BAND_CLASS_BC_6,
    RRM_RRC_BAND_CLASS_BC_7,
    RRM_RRC_BAND_CLASS_BC_8,
    RRM_RRC_BAND_CLASS_BC_9,
    RRM_RRC_BAND_CLASS_BC_10,
    RRM_RRC_BAND_CLASS_BC_11,
    RRM_RRC_BAND_CLASS_BC_12,
    RRM_RRC_BAND_CLASS_BC_13,
    RRM_RRC_BAND_CLASS_BC_14,
    RRM_RRC_BAND_CLASS_BC_15,
    RRM_RRC_BAND_CLASS_BC_16,
    RRM_RRC_BAND_CLASS_BC_17,
    RRM_RRC_BAND_CLASS_BC_18_V9X0,
    RRM_RRC_BAND_CLASS_BC_19_V9X0,
    RRM_RRC_BAND_CLASS_BC_20_V9X0,
    RRM_RRC_BAND_CLASS_BC_21_V9X0
}rrm_bandclass_cdma2000_et;

#define SYS_TIME_INFO_CDMA2000_SYNC_SYS_TIME_PRESENCE_FLAG   0x01
#define SYS_TIME_INFO_CDMA2000_ASYNC_SYS_TIME_PRESENCE_FLAG  0x02

typedef struct
{
/*This IE is deprecated and will be ignored at L3 even if RRM sends it*/
    U16   presence_bitmask;                         /*^ BITMASK ^*/

    U8    cdma_eutra_sync;
/*^ M, 0, H, 0, 1 ^*/    /* rrc_bool_et */

/*This IE is deprecated and will be ignored at L3 even if RRM sends it*/
    U8    sync_sys_time[SYNC_SYS_TIME_OCTET_SIZE];
/*^ O,SYS_TIME_INFO_CDMA2000_SYNC_SYS_TIME_PRESENCE_FLAG,OCTET_STRING,FIXED ^*/

/*This IE is deprecated and will be ignored at L3 even if RRM sends it*/
    U8    async_sys_time[ASYNC_SYS_TIME_OCTET_SIZE];
/*^ O,SYS_TIME_INFO_CDMA2000_ASYNC_SYS_TIME_PRESENCE_FLAG,OCTET_STRING,FIXED ^*/

}sys_time_info_cdma2000_t;

typedef U16 phys_cell_id_cdma2000_t; /*0,511*/

typedef struct
{
    U8                        count;
/*SPR_17664_start*/
/*^ M, 0, B, 1, MAX_PHYS_CELL_ID_LIST_CDMA2000 ^*/

/*SPR_17664_end*/
    phys_cell_id_cdma2000_t
        phys_cell_id_cdma2000[MAX_PHYS_CELL_ID_LIST_CDMA2000];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}phys_cell_id_list_cdma2000_t;

typedef U16 arfcn_value_cdma2000_t;

typedef struct
{
    arfcn_value_cdma2000_t         arfcn;         /*^ M, 0, H, 0, 2047 ^*/
    phys_cell_id_list_cdma2000_t   phys_cell_id_list;
}neigh_cells_per_bandclass_cdma2000_t;

typedef struct
{
/*SPR_17664_start*/
    U8                                     count;     /*^ M, 0, B, 1, MAX_NCELLS_PER_BS_LIST_CDMA2000 ^*/
/*SPR_17664_end*/
    neigh_cells_per_bandclass_cdma2000_t
        neigh_cells_per_bandclass_cdma2000[MAX_NCELLS_PER_BS_LIST_CDMA2000];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}neigh_cells_per_bandclass_list_cdma2000_t;

typedef struct
{
    U8                                         band_class;
/*^ M, 0, H, 0, 31 ^*/   /* rrm_bandclass_cdma2000_et */

    neigh_cells_per_bandclass_list_cdma2000_t  neigh_cells_per_freq_list;
}neigh_cell_cdma2000_t;

typedef struct
{
/*SPR_17664_start*/
    U8                       count;        /*^ M, 0, B, 1, MAX_NEIGH_CELL_LIST_CDMA2000 ^*/
/*SPR_17664_end*/
    neigh_cell_cdma2000_t    neigh_cell_cdma2000[MAX_NEIGH_CELL_LIST_CDMA2000];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}neigh_cell_list_cdma2000_t;

#define BAND_CLASS_INFO_CDMA2000_CELL_RESELECT_PRI_PRESENCE_FLAG  0x01

typedef struct
{
    U16    presence_bitmask;          /*^ BITMASK ^*/
    U8     band_class;
/*^ M, 0, H, 0, 31 ^*/   /* rrm_bandclass_cdma2000_et */

    U8     cell_reselection_priority;
/*^ O, BAND_CLASS_INFO_CDMA2000_CELL_RESELECT_PRI_PRESENCE_FLAG, H, 1, 7 ^*/

    U8     thresh_x_high;             /*^ M, 0, H, 0, 63 ^*/
    U8     thresh_x_low;              /*^ M, 0, H, 0, 63 ^*/
}band_class_info_cdma2000_t;

typedef struct
{
/*SPR_17664_start*/
    U8                          count;                  /*^ M, 0, H, 1, MAX_CDMA_BAND_CLASS ^*/
/*SPR_17664_end*/
    band_class_info_cdma2000_t  band_class_info_cdma2000[MAX_CDMA_BAND_CLASS];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}band_class_list_cdma2000_t;



#define CELL_RESELECT_CDMA2000_SF_PRESENCE_FLAG 0x01

typedef struct
{
    U16                         presence_bitmask;       /*^ BITMASK ^*/
    band_class_list_cdma2000_t  band_class_list;
    neigh_cell_list_cdma2000_t  neigh_cell_list;
    U8                          t_reselection_cdma2000;    /*^ M, 0, H, 0, 7 ^*/
    speed_state_scale_factors_t t_reselection_cdma2000_sf;
/*^ O, CELL_RESELECT_CDMA2000_SF_PRESENCE_FLAG, N, 0, 0 ^*/

}cell_reselection_params_cdma2000_t;

typedef struct
{
    U8  sid[XRTT_SID_OCTET_SIZE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    U8  nid[XRTT_NID_OCTET_SIZE]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    U8  multiple_sid;             /*^ M, 0, H, 0, 1 ^*/     /* rrc_bool_et */
    U8  multiple_nid;             /*^ M, 0, H, 0, 1 ^*/     /* rrc_bool_et */
    U8  home_reg;                 /*^ M, 0, H, 0, 1 ^*/     /* rrc_bool_et */
    U8  foreign_sid_reg;          /*^ M, 0, H, 0, 1 ^*/     /* rrc_bool_et */
    U8  foreign_nid_reg;          /*^ M, 0, H, 0, 1 ^*/     /* rrc_bool_et */
    U8  parame_reg;               /*^ M, 0, H, 0, 1 ^*/     /* rrc_bool_et */
    U8  power_up_reg;             /*^ M, 0, H, 0, 1 ^*/     /* rrc_bool_et */
    U8  reg_period[XRTT_REG_PERIOD_OCTET_SIZE];
/*^ M, 0, OCTET_STRING, FIXED ^*/

    U8  reg_zone[XRTT_REG_ZONE_OCTET_SIZE];
/*^ M, 0, OCTET_STRING, FIXED ^*/

    U8  total_zone[XRTT_TOTAL_ZONE_OCTET_SIZE];
/*^ M, 0, OCTET_STRING, FIXED ^*/

    U8  zone_timer[XRTT_ZONE_TIMER_OCTET_SIZE];
/*^ M, 0, OCTET_STRING, FIXED ^*/

}csfb_reg_param_1_xrtt_t;

typedef struct
{
    U8 count;              /*^ M, 0, B, 1, 2 ^*/
    U8 pre_reg_zone_id[2]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}secondary_pre_reg_zone_id_list_hrpd_t;

#define PRE_REG_INFO_HRPD_ZONE_ID_PRESENCE_FLAG         0x01
#define PRE_REG_INFO_HRPD_SECONDARY_LST_PRESENCE_FLAG   0x02

typedef struct
{
    U16                                   presence_bitmask; /*^ BITMASK ^*/
    U8                                    pre_reg_allowed;
/*^ M, 0, H, 0, 1 ^*/    /* rrc_bool_et */

    U8                                    pre_reg_zone_id;
/*^ O, PRE_REG_INFO_HRPD_ZONE_ID_PRESENCE_FLAG, N, 0, 255 ^*/

    secondary_pre_reg_zone_id_list_hrpd_t secondary_list;
/*^ O, PRE_REG_INFO_HRPD_SECONDARY_LST_PRESENCE_FLAG, N, 0, 0 ^*/

}pre_reg_info_hrpd_t;

#define PARAMS_HRPD_CELL_RESELECTION_PARAMS_PRESENCE_FLAG 0x01

typedef struct
{
    U16                                 presence_bitmask;
/*^ BITMASK ^*/

    pre_reg_info_hrpd_t                 pre_reg_info_hrpd;
/*^ M, 0, N, 0, 0 ^*/

    cell_reselection_params_cdma2000_t  cell_reselection_params_hrpd;
/*^ O, PARAMS_HRPD_CELL_RESELECTION_PARAMS_PRESENCE_FLAG, N, 0, 0 ^*/

}params_hrpd_t;

#define CFSB_REG_PARAM_1_XRTT_PRESENCE_FLAG         0x01
#define LONG_CODE_STATE_1_XRTT_PRESENCE_FLAG        0x02
#define CELL_RESELECTION_PARAM_1_XRTT_PRESENCE_FLAG 0x04

typedef struct
{
    U16                                presence_bitmask;
/*^ BITMASK ^*/

    csfb_reg_param_1_xrtt_t            csfb_reg_param_1_xrtt;
/*^ O, CFSB_REG_PARAM_1_XRTT_PRESENCE_FLAG, N, 0, 0 ^*/

/*This IE is deprecated and will be ignored at L3 even if RRM sends it*/
    U8                long_code_state_1_xrtt[LONG_CODE_STATE_1_XRTT_OCTET_SIZE];
/*^ O, LONG_CODE_STATE_1_XRTT_PRESENCE_FLAG, OCTET_STRING, FIXED ^*/

    cell_reselection_params_cdma2000_t cell_reselection_params_1_xrtt;
/*^ O, CELL_RESELECTION_PARAM_1_XRTT_PRESENCE_FLAG, N, 0, 0 ^*/

}params_1_xrtt_t;

typedef struct
{
    U8                        count;
	/* SPR_17664_start*/
/*^ M, 0, H, 0, MAX_PHYS_CELL_ID_LIST_CDMA2000_v920^*/
	/* SPR_17664_end*/
    phys_cell_id_cdma2000_t
        phys_cell_id_cdma2000_v920[MAX_PHYS_CELL_ID_LIST_CDMA2000_v920];
/*^ M, 0, OCTET_STRING, VARIABLE, H, 0, 511 ^*/    

}phys_cell_id_list_cdma2000_v920_t;

typedef struct
{
   phys_cell_id_list_cdma2000_v920_t phys_cell_id_list_v920; 
/*^ M, 0, N, 0, 0 ^*/
}neigh_cells_per_bandclass_cdma2000_v920_t;

typedef struct
{
/*SPR_17664_start*/
    U8                 count;     /*^ M, 0, B, 1, MAX_NCELLS_PER_BS_LIST_CDMA2000_v920 ^*/  
/*SPR_17664_end*/
    neigh_cells_per_bandclass_cdma2000_v920_t 
          neigh_cells_per_bandclass_cdma2000_v920[MAX_NCELLS_PER_BS_LIST_CDMA2000_v920];  
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
  
}neigh_cells_per_bandclass_list_cdma2000_v920_t;

typedef struct
{
    neigh_cells_per_bandclass_list_cdma2000_v920_t 
              neigh_cells_per_freq_list_v920;
/*^ M, 0, N, 0, 0 ^*/
}neigh_cell_cdma2000_v920_t;


typedef struct
{
/*SPR_17664_start*/
    U8                          count; /*^ M, 0, B, 1, MAX_NEIGH_CELL_LIST_CDMA2000_v920 ^*/
/*SPR_17664_end*/
    neigh_cell_cdma2000_v920_t  
            neigh_cell_cdma2000_v920[MAX_NEIGH_CELL_LIST_CDMA2000_v920];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/  

}neigh_cell_list_cdma2000_v920_t;

typedef struct
{
   neigh_cell_list_cdma2000_v920_t          neigh_cell_list_v920; 
/*^ M, 0, N, 0, 0 ^*/
    
}cell_reselection_params_cdma2000_v920_t;

/*cr_374 new strcture */
typedef struct
{
   U8   ac_barring_0_to_9_r9; /*^ M, 0, H, 0, 63 ^*/
   U8   ac_barring_10_r9;   /*^ M, 0, H, 0, 7 ^*/
   U8   ac_barring_11_r9;   /*^ M, 0, H, 0, 7 ^*/
   U8   ac_barring_12_r9;   /*^ M, 0, H, 0, 7 ^*/
   U8   ac_barring_13_r9;   /*^ M, 0, H, 0, 7 ^*/
   U8   ac_barring_14_r9;   /*^ M, 0, H, 0, 7 ^*/
   U8   ac_barring_15_r9;   /*^ M, 0, H, 0, 7 ^*/
   U8   ac_barring_msg_r9;  /*^ M, 0, H, 0, 7 ^*/
   U8   ac_barring_reg_r9;  /*^ M, 0, H, 0, 7 ^*/
   U8   ac_barring_emg_r9;  /*^ M, 0, H, 0, 7 ^*/

}ac_barring_config_1_xrtt_r9_t;

/*CR 487 changes start*/
typedef enum 
{
    CSFB_DUAL_RX_TX_SUPPORT_TRUE
}csfb_dual_rx_tx_support_et;
/*CR 487 changes stop*/

#define SIB_8_SYS_TIME_INFO_PRESENCE_FLAG                     0x01
#define SIB_8_SEARCH_WIN_SIZE_PRESENCE_FLAG                   0x02
#define SIB_8_PARAMS_HRPD_PRESENCE_FLAG                       0x04
#define SIB_8_PARAMS_1_XRTT_PRESENCE_FLAG                     0x08
#define SIB_8_CELL_RESEL_PARAMS_HRPD_V920_PRESENCE_FLAG       0x10
#define SIB_8_CELL_RESEL_PARAMS_1_XRTT_V920_PRESENCE_FLAG     0x20
/*cr_309 macro */  
#define SIB_8_CSFB_SUPPORT_FOR_DUAL_RX_UES_R9_PRESENCE_FLAG   0x40
/*cr_374 macro*/  
#define SIB_8_AC_BARRING_CONFIG_1_XRTT_R9_PRESENCE_FLAG       0x80
#define SIB_8_CSFB_REG_PARAM_1_XRTT_R9_PRESENCE_FLAG          0x100
/*CR 487 changes start*/
#define SIB_8_CSFB_DUAL_RX_TX_SUPPORT_R10_PRESENCE_FLAG       0x200  
/*CR 487 changes stop*/
typedef struct
{
    U16                       presence_bitmask;    /*^ BITMASK ^*/

    sys_time_info_cdma2000_t  sys_time_info;
/*^ O, SIB_8_SYS_TIME_INFO_PRESENCE_FLAG, N, 0, 0 ^*/

    U8                        search_window_size;
/*^ O, SIB_8_SEARCH_WIN_SIZE_PRESENCE_FLAG, H, 0, 15 ^*/

    params_hrpd_t             params_hrpd;
/*^ O, SIB_8_PARAMS_HRPD_PRESENCE_FLAG, N, 0, 0 ^*/

    params_1_xrtt_t           params_1_xrtt;
/*^ O, SIB_8_PARAMS_1_XRTT_PRESENCE_FLAG, N, 0, 0 ^*/

    cell_reselection_params_cdma2000_v920_t 
                      cell_reselection_params_hrpd_v920;
/*^ O, SIB_8_CELL_RESEL_PARAMS_HRPD_V920_PRESENCE_FLAG, N, 0, 0 ^*/

    cell_reselection_params_cdma2000_v920_t 
                      cell_reselection_params_1_xrtt_v920;
/*^ O, SIB_8_CELL_RESEL_PARAMS_1_XRTT_V920_PRESENCE_FLAG, N, 0, 0 ^*/

    U8                csfb_support_for_dual_rx_ues_r9;
/*^ O, SIB_8_CSFB_SUPPORT_FOR_DUAL_RX_UES_R9_PRESENCE_FLAG, H, 0, 1 ^*/ /* rrc_bool_et */

    ac_barring_config_1_xrtt_r9_t    ac_barring_config_1_xrtt_r9;            
/*^ O, SIB_8_AC_BARRING_CONFIG_1_XRTT_R9_PRESENCE_FLAG, N, 0, 0 ^*/

/* CDMA2000_CSFB_HO start */
    U8                csfb_registration_param_1xrtt_v920;
/*^ O, SIB_8_CSFB_REG_PARAM_1_XRTT_R9_PRESENCE_FLAG, N, 0, 0 ^*/     
	/*csfb_registration_param_1xrtt_v920_et*/
/* CDMA2000_CSFB_HO stop */

    /*CR 487 changes start*/
    U8                csfb_dual_rx_tx_support_r10;
/*^ O, SIB_8_CSFB_DUAL_RX_TX_SUPPORT_R10_PRESENCE_FLAG, H, 0, 0 ^*/  
    /* csfb_dual_rx_tx_support_et */ 
    /*CR 487 changes stop*/
}sib_type_8_Info_t;



#define SIB_9_HNB_ID_PRESENCE_FLAG      0x01

typedef struct /*OM. changed since 331.860*/
{
  U16       presence_bitmask;              /*^ BITMASK ^*/
  U8        size_of_hnb_id;

/*SPR_17664_start*/
/*^ O, SIB_9_HNB_ID_PRESENCE_FLAG, H, 1, MAX_HNB_ID_OCTET_SIZE ^*/
/*SPR_17664_end*/
  U8        hnb_id[MAX_HNB_ID_OCTET_SIZE];
/*^ O, SIB_9_HNB_ID_PRESENCE_FLAG, OCTET_STRING, VARIABLE ^*/

}sib_type_9_Info_t;




typedef enum
{
  RRM_RRC_NOT_LAST_SEGMENT,
  RRM_RRC_LAST_SEGMENT

}rrm_warning_msg_segment_type_et;


typedef enum
{
  RRM_RRC_SSP_0,
  RRM_RRC_SSP_1,
  RRM_RRC_SSP_2,
  RRM_RRC_SSP_3,
  RRM_RRC_SSP_4,
  RRM_RRC_SSP_5,
  RRM_RRC_SSP_6,
  RRM_RRC_SSP_7,
  RRM_RRC_SSP_8
}rrm_ssp_et;

/* CDMA2000_CSFB_HO start */
typedef struct
{
  rrc_bitmask_t    bitmask; /*^ BITMASK ^*/
#define CDMA2000_PARAM_RAND_MIN_PRESENCE_FLAG             0x01
#define CDMA2000_PARAM_RAND_MAX_PRESENCE_FLAG             0x02

/*Initial Random Seed given by RRM to L3*/
  U32              rand_seed;     /*^ M, 0, N, 0, 0 ^*/

/*Minimun value of Rand*/
  U32              rand_min;     
/*^ O, CDMA2000_PARAM_RAND_MIN_PRESENCE_FLAG, N, 0, 0 ^*/

/*Maximum value of Rand*/
  U32              rand_max;     
/*^ O, CDMA2000_PARAM_RAND_MAX_PRESENCE_FLAG, N, 0, 0 ^*/

/* timer duration in seconds to regenerate new Random Value*/
  U32              rand_regenerate_timer;     /*^ M, 0, B, 1, 4294967 ^*/
}cdma2000_rand_t;


typedef struct
{
    rrc_bitmask_t    bitmask; /*^ BITMASK ^*/
#define CDMA2000_PARAM_1XRTT_CELL_ID_PRESENCE_FLAG             0x01
#define CDMA2000_PARAM_HRPD_SECTOR_ID_PRESENCE_FLAG            0x02
#define CDMA2000_PARAM_NETWORKS_IN_SYNC_PRESENCE_FLAG          0x04
/* CSR 00070260 Changes Start */
#define CDMA2000_PARAM_1XRTT_REFERENCE_CELL_ID_PRESENCE_FLAG   0x08
#define CDMA2000_PARAM_HRPD_REFERENCE_CELL_ID_PRESENCE_FLAG    0x10
/* CSR 00070260 Changes Stop */
    cdma2000_rand_t cdma2000_rand;    /*^ M, 0, N, 0, 0 ^*/
    cdma2000_1xrtt_cell_identifier_t     cell_id_1xrtt;  /*^ O, 1, N, 0, 0 ^*/
    cdma2000_hrpd_sector_identifier_t    sector_id_hrpd;    /*^ O, 2, N, 0, 0 ^*/
    U8                                   nw_in_sync;        /*^ O, 4, H, 0, 1 ^*/ /* rrc_bool_et */
/* CSR 00070260 Changes Start */
    cdma2000_1xrtt_ref_cell_identifier_t  ref_cell_id_1xrtt;  
    /*^ O, CDMA2000_PARAM_1XRTT_REFERENCE_CELL_ID_PRESENCE_FLAG, N, 0, 0 ^*/
    cdma2000_hrpd_ref_sector_identifier_t ref_sector_id_hrpd;    
    /*^ O, CDMA2000_PARAM_HRPD_REFERENCE_CELL_ID_PRESENCE_FLAG, N, 0, 0 ^*/
/* CSR 00070260 Changes Stop */
}cdma2000_cell_param_t;
/* CDMA2000_CSFB_HO stop */

/******************************************************************************
        RRC_RRM_CELL_SETUP_RESP
******************************************************************************/


#define CELL_SETUP_RESP_API_FAIL_CAUSE_PRESENCE_FLAG        0x01
#define CELL_SETUP_RESP_API_EARFCN_LIST_PRESENCE_FLAG       0x02

typedef struct
{
    U16               presence_bitmask;   /*^ BITMASK ^*/
    rrc_cell_index_t  cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */

    U8                response;
/*^ M, 0, H, 0, 1 ^*/    /* rrc_return_et */

    U8                fail_cause;
/*^ O, CELL_SETUP_RESP_API_FAIL_CAUSE_PRESENCE_FLAG, H, 0, 14 ^*/
/* rrm_fail_cause_et */
/* SPR_18211 start */
    rrc_dl_earfcn_t   dl_earfcn_list;
/*^ O, CELL_SETUP_RESP_API_EARFCN_LIST_PRESENCE_FLAG, N, 0, 0 ^*/
/* SPR_18211 stop */
/* rrc_dl_earfcn_t */
} rrc_rrm_cell_setup_resp_t;/*^ API,  RRC_RRM_CELL_SETUP_RESP ^*/


/******************************************************************************
 *             RRC_RRM_CELL_RECONFIG_REQ
 ******************************************************************************/

/* BROADCAST INFO PARAM */
typedef struct
{
#define CELL_RECONFIG_REQ_API_MIB_INFO_PRESENCE_FLAG                0x01
#define CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG              0x02
#define CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG              0x04
#define CELL_RECONFIG_REQ_API_SIB_3_INFO_PRESENCE_FLAG              0x08
#define CELL_RECONFIG_REQ_API_SIB_4_INFO_PRESENCE_FLAG              0x10
#define CELL_RECONFIG_REQ_API_SIB_5_INFO_PRESENCE_FLAG              0x20
#define CELL_RECONFIG_REQ_API_SIB_6_INFO_PRESENCE_FLAG              0x40
#define CELL_RECONFIG_REQ_API_SIB_7_INFO_PRESENCE_FLAG              0x80
#define CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG              0x100
#define CELL_RECONFIG_REQ_API_SIB_9_INFO_PRESENCE_FLAG              0x200
#define CELL_RECONFIG_REQ_API_SIB_13_INFO_PRESENCE_FLAG             0x2000
/*SPR 18093:start*/
#define CELL_RECONFIG_REQ_API_MBMS_AREA_INFO_LIST_FLAG              0x400
/*SPR 18093:end */
#define CELL_RECONFIG_REQ_API_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG    0x800
  U16                           presence_bitmask;   /*^ BITMASK ^*/
  mib_info_t                    mib_info;                   /*^ O, 1, N, 0, 0 ^*/
  sib_type_1_Info_t             sib_type_1_Info;            /*^ O, 2, N, 0, 0 ^*/
  sib_type_2_Info_t             sib_type_2_Info;            /*^ O, 4, N, 0, 0 ^*/
  sib_type_3_Info_t             sib_type_3_Info;            /*^ O, 8, N, 0, 0 ^*/
  sib_type_4_Info_t             sib_type_4_Info;            /*^ O, 16, N, 0, 0 ^*/
  sib_type_5_Info_t             sib_type_5_Info;            /*^ O, 32, N, 0, 0 ^*/
  sib_type_6_Info_t             sib_type_6_Info;            /*^ O, 64, N, 0, 0 ^*/
  sib_type_7_Info_t             sib_type_7_Info;            /*^ O, 128, N, 0, 0 ^*/
  sib_type_8_Info_t             sib_type_8_Info;            /*^ O, 256, N, 0, 0 ^*/
  sib_type_9_Info_t             sib_type_9_Info;            /*^ O, 512, N, 0, 0 ^*/
  sib_type_13_info_t            sib_type_13_info;           /*^ O, 8192, N, 0, 0 ^*/
/*SPR 18093:start*/
  mbms_updated_area_info_list_t mbms_updated_area_info_list;
  /*^ O, 1024, N, 0, 0 ^*/
/*SPR 18093:end */
/* SPR 18672 Fix Start */
  m2ap_reserved_mcch_related_bcch_info_list_t    m2ap_reserved_mcch_related_bcch_info_list;
  /*^ O,2048 , N, 0, 0 ^*/
/* SPR 18672 Fix End */
}broadcast_config_info_t;

/*NON BROAD CAST INFO PARAM */
typedef struct
{
#define CELL_RECONFIG_REQ_API_CELL_PARAMETER_INFO_PRESENCE_FLAG         0x01
#define CELL_RECONFIG_REQ_API_SYNC_SIGNAL_INFO_PRESENCE_FLAG            0x02
#define CELL_RECONFIG_REQ_API_MAC_RECONFIG_INFO_PRESENCE_FLAG           0x04

  U16               presence_bitmask;   /*^ BITMASK ^*/ 
  rrc_recfg_phy_cell_parameters_t       cell_parameters;            /*^ O, 1, N, 0, 0 ^*/
  rrc_phy_sync_signals_t        sync_signals;               /*^ O, 2, N, 0, 0 ^*/
  rrc_mac_reconfig_t            mac_reconfig_params;
  /*^ O, CELL_RECONFIG_REQ_API_MAC_RECONFIG_INFO_PRESENCE_FLAG, N, 0, 0 ^*/
}non_broadcast_config_info_t;


#define CELL_RECONFIG_REQ_API_BROADCAST_INFO_PRESENCE_FLAG      0x01
#define CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG  0x02
#define CELL_RECONFIG_REQ_API_SFN_PRESENCE_FLAG                 0x04
#define CELL_RECONFIG_REQ_API_EMERGENCY_AREA_ID_PRESENCE_FLAG   0x08
/*SPR_19066_START*/
/* CELL_RECONFIG_REQ_API_UE_INACTIVE_TIME_PRESENT has been deprecated */
/*SPR_19066_END*/
/* CLPC Start */
#define CELL_RECONFIG_DOWNLINK_POWER_CONTROL_COMMON_PRESENT     0x20
/* CLPC Stop */
#define CELL_RECONFIG_REQ_API_CONTENTION_FREE_RACH_TIMER_PRESENT  0x40
#define CELL_RECONFIG_REQ_API_CDMA2000_CELL_PARAM_PRESENT         0x80
#define CELL_RECONFIG_REQ_API_EMBMS_TRANSMISSION_POWER_INFO_PRESENCE_FLAG 0x100

typedef struct
{
  U16                               presence_bitmask;       /*^ BITMASK ^*/
  rrc_cell_index_t                  cell_index;     /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/  /* MAX_NUM_CELLS - 1 */
  broadcast_config_info_t           broadcast_info;     /*^ O, 1, N , 0 ,0 ^*/
  non_broadcast_config_info_t       non_broadcast_info; /*^ O, 2, N, 0, 0 ^*/
  U8                            emergency_area_id[EMERGENCY_AREA_SIZE]; 
                              /*^ O, 8, OCTET_STRING, FIXED ^*/

  /*SPR_19066_START*/
  /* Code Removed */
  /*SPR_19066_END*/

  U16  contention_free_rach_timer;
  /*^ O, CELL_RECONFIG_REQ_API_CONTENTION_FREE_RACH_TIMER_PRESENT, N, 0, 0 ^*/

  /* CLPC Start */
  rrc_mac_downlink_power_control_common_t downlink_power_control_common;
  /*^ O, CELL_RECONFIG_DOWNLINK_POWER_CONTROL_COMMON_PRESENT, N, 0, 0 ^*/
  /* CLPC Stop */

  /* CDMA2000_CSFB_HO start */
  cdma2000_cell_param_t       cdma2000_cell_param;
  /*^O, CELL_RECONFIG_REQ_API_CDMA2000_CELL_PARAM_PRESENT, N, 0, 0 ^*/
  /* CDMA2000_CSFB_HO stop */
  U16                           mbms_transmission_power;   
  /*^ O, 256, H, 0, 10000 ^*/

}rrc_rrm_cell_reconfig_req_t; /*^ API, RRC_RRM_CELL_RECONFIGURE_REQ ^*/

/******************************************************************************
        RRC_RRM_CELL_RECONFIG_RESP
******************************************************************************/

#define CELL_RECONFIG_RESP_API_FAIL_CAUSE_PRESENCE_FLAG        0x01

typedef struct
{
    U16               presence_bitmask;   /*^ BITMASK ^*/
    rrc_cell_index_t  cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */

    U8                response;
/*^ M, 0, H, 0, 1 ^*/    /* rrc_return_et */

    U8                fail_cause;
/*^ O, CELL_RECONFIG_RESP_API_FAIL_CAUSE_PRESENCE_FLAG, H, 0, 14 ^*/
/* rrm_fail_cause_et */

} rrc_rrm_cell_reconfig_resp_t;/*^ API,  RRC_RRM_CELL_RECONFIG_RESP ^*/


/*! \RrcUeSpecificNarrowbands
 *  \brief This structure contains the number of UE specific narrow
 *         bands and value of UE specific narrowbands.
 *  \param numUeSpecificNarrowbands
 *         indicates the number of UE specific narrow bands
 *  \param ueSpecificNarrowbands
 *         indicates the values of UE specific narrow bands
 */
typedef struct _RrcUeSpecificNarrowbands
{
    U8    numUeSpecificNarrowbands;
    /*^ M, 0, B, 1, EMTC_MAX_AVAILABLE_NARROWBAND ^*/

    U8    ueSpecificNarrowbands[EMTC_MAX_AVAILABLE_NARROWBAND];
    /*^ M, 0, OCTET_STRING, VARIABLE, B, 1, EMTC_MAX_AVAILABLE_NARROWBAND ^*/
}RrcUeSpecificNarrowbands;


typedef struct _EmtcCellParam
{
    U8      operationMode;
    /*^ M, 0, B, 1, EMTC_OPERATION_MODE_MAX ^*/
    /* EmtcOperationMode_et */

    U8      duplexingMode;
    /*^ M, 0, H, 0, EMTC_DUPLEXING_MODE_MAX ^*/
    /* EmtcDuplexingMode_et */
    
    U8      transmissionType;
    /*^ M, 0, H, 0, EMTC_TRANSMISSION_TYPE_MAX ^*/
    /* EmtcTransmissionType_et */

    RrcUeSpecificNarrowbands    ueSpecificNarrowbands;
    /*^ M, 0, N, 0 , 0 ^*/
}EmtcCellParam;


/******************************************************************************
            RRC_RRM_CELL_SETUP_REQ
******************************************************************************/

#define CELL_SETUP_REQ_API_SIB_3_INFO_PRESENCE_FLAG             0x01
#define CELL_SETUP_REQ_API_SIB_4_INFO_PRESENCE_FLAG             0x02
#define CELL_SETUP_REQ_API_SIB_5_INFO_PRESENCE_FLAG             0x04
#define CELL_SETUP_REQ_API_SIB_6_INFO_PRESENCE_FLAG             0x08
#define CELL_SETUP_REQ_API_SIB_7_INFO_PRESENCE_FLAG             0x10
#define CELL_SETUP_REQ_API_SIB_8_INFO_PRESENCE_FLAG             0x20
#define CELL_SETUP_REQ_API_SIB_9_INFO_PRESENCE_FLAG             0x40
#define CELL_SETUP_REQ_API_EXPLICIT_START_REQUIRED_PRESENCE_FLAG  0x80
#define CELL_SETUP_REQ_API_CDMA2000_PARAM_PRESENCE_FLAG          0x100
#define CELL_SETUP_REQ_API_SIB_13_INFO_PRESENCE_FLAG             0x200
#define CELL_SETUP_REQ_API_MCCH_CELL_RESERVATION_INFO_PRESENCE_FLAG 0x400
#define CELL_SETUP_REQ_API_EMBMS_TRANSMISSION_POWER_INFO_PRESENCE_FLAG 0x800
/*SPR 18093:start*/
#define CELL_SETUP_REQ_API_EMBMS_AREA_INFO_LIST_PRESENCE_FLAG 0x1000
/*SPR 18093:end*/
#define CELL_SETUP_REQ_API_EMTC_CELL_PARAM_PRESENT                   0x2000

typedef struct
{
  U16                         presence_bitmask;         /*^ BITMASK ^*/
  rrc_cell_index_t            cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/  /* MAX_NUM_CELLS - 1 */

  mib_info_t                  mib_info;                 /*^ M, 0, N, 0, 0 ^*/
  sib_type_1_Info_t           sib_type_1_Info;          /*^ M, 0, N, 0, 0 ^*/
  sib_type_2_Info_t           sib_type_2_Info;          /*^ M, 0, N, 0, 0 ^*/
  sib_type_3_Info_t           sib_type_3_Info;          /*^ O, 1, N, 0, 0 ^*/
  sib_type_4_Info_t           sib_type_4_Info;          /*^ O, 2, N, 0, 0 ^*/
  sib_type_5_Info_t           sib_type_5_Info;          /*^ O, 4, N, 0, 0 ^*/
  sib_type_6_Info_t           sib_type_6_Info;          /*^ O, 8, N, 0, 0 ^*/
  sib_type_7_Info_t           sib_type_7_Info;          /*^ O, 16, N, 0, 0 ^*/
  sib_type_8_Info_t           sib_type_8_Info;          /*^ O, 32, N, 0, 0 ^*/
  sib_type_9_Info_t           sib_type_9_Info;          /*^ O, 64, N, 0, 0 ^*/
  sib_type_13_info_t          sib_type_13_Info;          /*^ O, 512 , N, 0, 0 ^*/
  rrc_rrm_cell_config_t       cell_config_param;        /*^ M, 0, N, 0, 0 ^*/
  U8                          emergency_area_id[EMERGENCY_AREA_SIZE]; 
                              /*^ M, 0, OCTET_STRING, FIXED ^*/
  U8                          explicit_start_required;  /*^ O, 128, H, 0, 0 ^*/  
/* CDMA2000_CSFB_HO start */
  cdma2000_cell_param_t       cdma2000_cell_param;            /*^ O, 256, H, 0, 0 ^*/
/* CDMA2000_CSFB_HO stop */
  U16                           mbms_transmission_power;
  /*^ O, 2048, H, 0, 10000 ^*/
/*SPR 18093:start*/
  mbms_updated_area_info_list_t mbms_updated_area_info_list;
  /*^ O, 4096, N, 0, 0 ^*/
/*SPR 18093:end*/
  m2ap_reserved_mcch_related_bcch_info_list_t    m2ap_reserved_mcch_related_bcch_info_list;
  /*^ O,1024 , N, 0, 0 ^*/
  /*eMTC changes start*/
  EmtcCellParam                emtcCellParam;                         
  /*^ O, CELL_SETUP_REQ_API_EMTC_CELL_PARAM_PRESENT , N, 0, 0^*/
  /* EmtcOperationMode_et */
  /*eMTC changes end */

}rrc_rrm_cell_setup_req_t; /*^ API, RRC_RRM_CELL_SETUP_REQ ^*/

/******************************************************************************
            RRC_RRM_UE_ADMISSION_REQ
******************************************************************************/
/* TS 36.331 - 6.3.6 S-TMSI IE*/
typedef struct
{
  U8        mmec[MME_OCTET_SIZE];
/*^ M, 0, OCTET_STRING, FIXED ^*/

  U8        m_tmsi[M_TMSI_OCTET_SIZE];
/*^ M, 0, OCTET_STRING, FIXED ^*/

} rrc_s_tmsi_t;

/* TS 36.331 - 6.2.2 RRCConnectionrequest message InitialUE-Identity IE*/
typedef struct
{

    U16             bitmask;       /*^ BITMASK ^*/
#define RRC_INITIAL_UE_IDENTITY_S_TMSI_PRESENT        0x01
#define RRC_INITIAL_UE_IDENTITY_RANDOM_VALUE_PRESENT  0x02

    rrc_s_tmsi_t    s_tmsi;
/*^ O, RRC_INITIAL_UE_IDENTITY_S_TMSI_PRESENT ^*/

    U8              random_value[STMSI_RANDOM_VALUE_OCTET_SIZE];
/*^ O, RRC_INITIAL_UE_IDENTITY_RANDOM_VALUE_PRESENT, OCTET_STRING, FIXED ^*/

} rrc_initial_ue_identity_t;

/* TS 36.331 - 6.2.2 RRCConnectionrequest message EstablishmentCause IE*/
typedef enum
{
  RRC_ESTAB_CAUSE_EMERGENCY,
  RRC_ESTAB_CAUSE_HIGHPRIORITY_ACCESS,
  RRC_ESTAB_CAUSE_MT_ACCESS,
  RRC_ESTAB_CAUSE_MO_SIGNALLING,
  RRC_ESTAB_CAUSE_MO_DATA,
  RRC_ESTAB_CAUSE_DELAY_TOELERANCE
} rrc_establishment_cause_et;

/* eMTC changes start */
#define RRC_RRM_UE_ADMISSION_REQ_CE_LEVEL_PRESENT    0x01
/* eMTC changes stop */
typedef struct
{
    /* eMTC changes start */
    RrcBitmaskU8_t                bitmask;
    /*^ BITMASK ^*/
    /* eMTC changes stop */

  U16                           ue_index;
  rrc_initial_ue_identity_t     ue_identity;  /*^ M, 0, N, 0, 0 ^*/
  U8                            establishment_cause;
/*^ M, 0, H, 1, 5 ^*/ /* rrc_establishment_cause_et */

  /* CLPC Start */

  U16                           timing_advance;
  /*^ M, 0, H, 0, 1282 ^*/
  U8                            sinr;
  /*^ M, 0, N, 0, 0 ^*/

  /* CLPC Stop */

    /* eMTC changes start */
    U8                            ceLevel;
    /*^ O, RRC_RRM_UE_ADMISSION_REQ_CE_LEVEL_PRESENT, H, 0, EMTC_CE_LEVEL_MAX ^*/
    /* EmtcCeLevel_et */
    /* eMTC changes stop */

} rrc_rrm_ue_admission_req_t; /*^ API, RRC_RRM_UE_ADMISSION_REQ ^*/

/******************************************************************************
    TS 36.331 - 6.3.2 RLC-Config IE
******************************************************************************/
typedef enum
{
  RRC_POLL_RETRAS_MS_5,
  RRC_POLL_RETRAS_MS_10,
  RRC_POLL_RETRAS_MS_15,
  RRC_POLL_RETRAS_MS_20,
  RRC_POLL_RETRAS_MS_25,
  RRC_POLL_RETRAS_MS_30,
  RRC_POLL_RETRAS_MS_35,
  RRC_POLL_RETRAS_MS_40,
  RRC_POLL_RETRAS_MS_45,
  RRC_POLL_RETRAS_MS_50,
  RRC_POLL_RETRAS_MS_55,
  RRC_POLL_RETRAS_MS_60,
  RRC_POLL_RETRAS_MS_65,
  RRC_POLL_RETRAS_MS_70,
  RRC_POLL_RETRAS_MS_75,
  RRC_POLL_RETRAS_MS_80,
  RRC_POLL_RETRAS_MS_85,
  RRC_POLL_RETRAS_MS_90,
  RRC_POLL_RETRAS_MS_95,
  RRC_POLL_RETRAS_MS_100,
  RRC_POLL_RETRAS_MS_105,
  RRC_POLL_RETRAS_MS_110,
  RRC_POLL_RETRAS_MS_115,
  RRC_POLL_RETRAS_MS_120,
  RRC_POLL_RETRAS_MS_125,
  RRC_POLL_RETRAS_MS_130,
  RRC_POLL_RETRAS_MS_135,
  RRC_POLL_RETRAS_MS_140,
  RRC_POLL_RETRAS_MS_145,
  RRC_POLL_RETRAS_MS_150,
  RRC_POLL_RETRAS_MS_155,
  RRC_POLL_RETRAS_MS_160,
  RRC_POLL_RETRAS_MS_165,
  RRC_POLL_RETRAS_MS_170,
  RRC_POLL_RETRAS_MS_175,
  RRC_POLL_RETRAS_MS_180,
  RRC_POLL_RETRAS_MS_185,
  RRC_POLL_RETRAS_MS_190,
  RRC_POLL_RETRAS_MS_195,
  RRC_POLL_RETRAS_MS_200,
  RRC_POLL_RETRAS_MS_205,
  RRC_POLL_RETRAS_MS_210,
  RRC_POLL_RETRAS_MS_215,
  RRC_POLL_RETRAS_MS_220,
  RRC_POLL_RETRAS_MS_225,
  RRC_POLL_RETRAS_MS_230,
  RRC_POLL_RETRAS_MS_235,
  RRC_POLL_RETRAS_MS_240,
  RRC_POLL_RETRAS_MS_245,
  RRC_POLL_RETRAS_MS_250,
  RRC_POLL_RETRAS_MS_300,
  RRC_POLL_RETRAS_MS_350,
  RRC_POLL_RETRAS_MS_400,
  RRC_POLL_RETRAS_MS_450,
  RRC_POLL_RETRAS_MS_500
}rrc_t_poll_retransmit_et;

typedef enum
{
  RRC_POLL_PDU_4,
  RRC_POLL_PDU_8,
  RRC_POLL_PDU_16,
  RRC_POLL_PDU_32,
  RRC_POLL_PDU_64,
  RRC_POLL_PDU_128,
  RRC_POLL_PDU_256,
  RRC_POLL_PDU_INFINITY
}rrc_poll_pdu_et;

typedef enum
{
  RRC_POLL_BYTE_KB_25,
  RRC_POLL_BYTE_KB_50,
  RRC_POLL_BYTE_KB_75,
  RRC_POLL_BYTE_KB_100,
  RRC_POLL_BYTE_KB_125,
  RRC_POLL_BYTE_KB_250,
  RRC_POLL_BYTE_KB_375,
  RRC_POLL_BYTE_KB_500,
  RRC_POLL_BYTE_KB_750,
  RRC_POLL_BYTE_KB_1000,
  RRC_POLL_BYTE_KB_1250,
  RRC_POLL_BYTE_KB_1500,
  RRC_POLL_BYTE_KB_2000,
  RRC_POLL_BYTE_KB_3000,
  RRC_POLL_BYTE_KB_INFINITY
}rrc_poll_byte_et;

typedef enum
{
  RRC_MAX_RETRANS_THRESH_1,
  RRC_MAX_RETRANS_THRESH_2,
  RRC_MAX_RETRANS_THRESH_3,
  RRC_MAX_RETRANS_THRESH_4,
  RRC_MAX_RETRANS_THRESH_6,
  RRC_MAX_RETRANS_THRESH_8,
  RRC_MAX_RETRANS_THRESH_16,
  RRC_MAX_RETRANS_THRESH_32
}rrc_max_retx_threshold_et;

/* eMTC changes start */
#define RRC_RRM_T_POLL_RETRANSMIT_EMTC_PRESENT     0x01
/* eMTC changes stop */
typedef struct
{
    /* eMTC changes start */
	RrcBitmaskU8_t    bitmask;
    /*^ BITMASK ^*/
    /* eMTC changes stop */

  U8        t_poll_retransmit;
/*^ M, 0, H, 1, 54 ^*/      /* rrc_t_poll_retransmit_et */

  U8        poll_pdu;
/*^ M, 0, H, 1, 7 ^*/       /* rrc_poll_pdu_et */

  U8        poll_byte;
/*^ M, 0, H, 1, 14 ^*/      /* rrc_poll_byte_et */

  U8        max_retx_threshold;
/*^ M, 0, H, 1, 7 ^*/       /* rrc_max_retx_threshold_et */

    /* eMTC changes start */
    U8        t_poll_retransmit_emtc;
    /*^ O, RRC_RRM_T_POLL_RETRANSMIT_EMTC_PRESENT, H, 1, 54 ^*/
    /* rrc_t_poll_retransmit_et */
    /* eMTC changes stop */

} rrc_uplink_am_rlc_t;

typedef enum
{
  RRC_T_REORDER_MS_0,
  RRC_T_REORDER_MS_5,
  RRC_T_REORDER_MS_10,
  RRC_T_REORDER_MS_15,
  RRC_T_REORDER_MS_20,
  RRC_T_REORDER_MS_25,
  RRC_T_REORDER_MS_30,
  RRC_T_REORDER_MS_35,
  RRC_T_REORDER_MS_40,
  RRC_T_REORDER_MS_45,
  RRC_T_REORDER_MS_50,
  RRC_T_REORDER_MS_55,
  RRC_T_REORDER_MS_60,
  RRC_T_REORDER_MS_65,
  RRC_T_REORDER_MS_70,
  RRC_T_REORDER_MS_75,
  RRC_T_REORDER_MS_80,
  RRC_T_REORDER_MS_85,
  RRC_T_REORDER_MS_90,
  RRC_T_REORDER_MS_95,
  RRC_T_REORDER_MS_100,
  RRC_T_REORDER_MS_110,
  RRC_T_REORDER_MS_120,
  RRC_T_REORDER_MS_130,
  RRC_T_REORDER_MS_140,
  RRC_T_REORDER_MS_150,
  RRC_T_REORDER_MS_160,
  RRC_T_REORDER_MS_170,
  RRC_T_REORDER_MS_180,
  RRC_T_REORDER_MS_190,
  RRC_T_REORDER_MS_200
}rrc_t_reordering_et;

typedef enum
{
  RRC_T_STATUS_PROHB_MS_0,
  RRC_T_STATUS_PROHB_MS_5,
  RRC_T_STATUS_PROHB_MS_10,
  RRC_T_STATUS_PROHB_MS_15,
  RRC_T_STATUS_PROHB_MS_20,
  RRC_T_STATUS_PROHB_MS_25,
  RRC_T_STATUS_PROHB_MS_30,
  RRC_T_STATUS_PROHB_MS_35,
  RRC_T_STATUS_PROHB_MS_40,
  RRC_T_STATUS_PROHB_MS_45,
  RRC_T_STATUS_PROHB_MS_50,
  RRC_T_STATUS_PROHB_MS_55,
  RRC_T_STATUS_PROHB_MS_60,
  RRC_T_STATUS_PROHB_MS_65,
  RRC_T_STATUS_PROHB_MS_70,
  RRC_T_STATUS_PROHB_MS_75,
  RRC_T_STATUS_PROHB_MS_80,
  RRC_T_STATUS_PROHB_MS_85,
  RRC_T_STATUS_PROHB_MS_90,
  RRC_T_STATUS_PROHB_MS_95,
  RRC_T_STATUS_PROHB_MS_100,
  RRC_T_STATUS_PROHB_MS_105,
  RRC_T_STATUS_PROHB_MS_110,
  RRC_T_STATUS_PROHB_MS_115,
  RRC_T_STATUS_PROHB_MS_120,
  RRC_T_STATUS_PROHB_MS_125,
  RRC_T_STATUS_PROHB_MS_130,
  RRC_T_STATUS_PROHB_MS_135,
  RRC_T_STATUS_PROHB_MS_140,
  RRC_T_STATUS_PROHB_MS_145,
  RRC_T_STATUS_PROHB_MS_150,
  RRC_T_STATUS_PROHB_MS_155,
  RRC_T_STATUS_PROHB_MS_160,
  RRC_T_STATUS_PROHB_MS_165,
  RRC_T_STATUS_PROHB_MS_170,
  RRC_T_STATUS_PROHB_MS_175,
  RRC_T_STATUS_PROHB_MS_180,
  RRC_T_STATUS_PROHB_MS_185,
  RRC_T_STATUS_PROHB_MS_190,
  RRC_T_STATUS_PROHB_MS_195,
  RRC_T_STATUS_PROHB_MS_200,
  RRC_T_STATUS_PROHB_MS_205,
  RRC_T_STATUS_PROHB_MS_210,
  RRC_T_STATUS_PROHB_MS_215,
  RRC_T_STATUS_PROHB_MS_220,
  RRC_T_STATUS_PROHB_MS_225,
  RRC_T_STATUS_PROHB_MS_230,
  RRC_T_STATUS_PROHB_MS_235,
  RRC_T_STATUS_PROHB_MS_240,
  RRC_T_STATUS_PROHB_MS_245,
  RRC_T_STATUS_PROHB_MS_250,
  RRC_T_STATUS_PROHB_MS_300,
  RRC_T_STATUS_PROHB_MS_350,
  RRC_T_STATUS_PROHB_MS_400,
  RRC_T_STATUS_PROHB_MS_450,
  RRC_T_STATUS_PROHB_MS_500
} rrc_t_status_prohibit_et;

typedef struct
{
  U8        t_reordering;
/*^ M, 0, H, 1, 30 ^*/  /* rrc_t_reordering_et */

  U8        t_status_prohibit;
/*^ M, 0, H, 1, 55 ^*/  /* rrc_t_status_prohibit_et */

} rrc_downlink_am_rlc_t;

typedef struct
{
    rrc_uplink_am_rlc_t        ul_am_rlc;
    rrc_downlink_am_rlc_t      dl_am_rlc;
} rrc_am_config_t;

typedef enum
{
  RRC_SN_FIELD_LEN_5,
  RRC_SN_FIELD_LEN_10
} rrc_sn_field_length_et;

typedef enum
{
    ADDED,
    DELETED,
    MODIFIED,
    UNCHANGE
}mbms_area_flag_et;

typedef struct
{
    U8        sn_field_length;
/*^ M, 0, H, 0, 1 ^*/  /* rrc_sn_field_length_et */

    U8        t_reordering;    /*^ M, 0, H, 1, 30 ^*/ /* rrc_t_reordering_et */
} rrc_downlink_um_rlc_t;

typedef struct
{
    U8        sn_field_length;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_sn_field_length_et */

} rrc_uplink_um_rlc_t;

typedef struct
{
    rrc_uplink_um_rlc_t        ul_um_rlc;
    rrc_downlink_um_rlc_t      dl_um_rlc;
} rrc_um_bi_directional_config_t;

typedef struct
{
    rrc_uplink_um_rlc_t        ul_um_rlc;
} rrc_um_uni_directional_ul_config_t;

typedef struct
{
    rrc_downlink_um_rlc_t      dl_um_rlc;
} rrc_um_uni_directional_dl_config_t;

typedef struct
{
  U16                 bitmask;   /*^ BITMASK ^*/
#define RRC_RLC_CONFIG_AM_CONFIG_PRESENT                        0x01
#define RRC_RLC_CONFIG_UM_BI_DIRECTIONAL_CONFIG_PRESENT         0x02
#define RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_UL_PRESENT     0x04
#define RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_DL_PRESENT     0x08

  rrc_am_config_t                       am_config;
/*^ O, RRC_RLC_CONFIG_AM_CONFIG_PRESENT ^*/

  rrc_um_bi_directional_config_t        um_bi_directional_config;
/*^ O, RRC_RLC_CONFIG_UM_BI_DIRECTIONAL_CONFIG_PRESENT ^*/

  rrc_um_uni_directional_ul_config_t    um_uni_directional_ul_config;
/*^ O, RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_UL_PRESENT ^*/

  rrc_um_uni_directional_dl_config_t    um_uni_directional_dl_config;
/*^ O, RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_DL_PRESENT ^*/

} rrc_rlc_config_t;

typedef struct
{
    U16         bitmask; /*^ BITMASK ^*/
#define RRC_RLC_CONFIG_QCI_LENGTH_PRESENT   0x01
    rrc_rlc_config_t    rlc_config;

    U32         qci_length;
/*^ O, RRC_RLC_CONFIG_QCI_LENGTH_PRESENT, H, 0, 28311552 ^*/

} rrc_rlc_config_enb_t;

/******************************************************************************
    TS 36.331 - 6.3.2 LogicalChannelConfig IE
******************************************************************************/
typedef enum
{
  RRC_BIT_RATE_KBPS_0,
  RRC_BIT_RATE_KBPS_8,
  RRC_BIT_RATE_KBPS_16,
  RRC_BIT_RATE_KBPS_32,
  RRC_BIT_RATE_KBPS_64,
  RRC_BIT_RATE_KBPS_128,
  RRC_BIT_RATE_KBPS_256,
  RRC_BIT_RATE_INFINITY,    /* CR 746 changes start*/
  RRC_BIT_RATE_KBPS_512_v1020,
  RRC_BIT_RATE_KBPS_1024_v1020,
  RRC_BIT_RATE_KBPS_2048_v1020 /* CR 746 changes stop*/
}rrc_prioritized_bit_rate_et;

typedef enum
{
  RRC_BUCKET_SIZE_MS_50,
  RRC_BUCKET_SIZE_MS_100,
  RRC_BUCKET_SIZE_MS_150,
  RRC_BUCKET_SIZE_MS_300,
  RRC_BUCKET_SIZE_MS_500,
  RRC_BUCKET_SIZE_MS_1000
}rrc_bucket_size_duration_et;

typedef enum
{
  RRC_LC_SR_MASK_R9_SETUP_2  
}rrc_lc_sr_mask_r9_et;


typedef struct
{
  U16           bitmask;       /*^ BITMASK ^*/
#define RRC_UL_SPECIFIC_PARAMETERS_LOGICAL_CH_GROUP_PRESENT        0x01

  U8            priority;               /*^ M, 0, H, 1, 16 ^*/
  
  /* CR 746 changes start*/
  U8            prioritized_bit_rate;
/*^ M, 0, H, 1, 10 ^*/  /* rrc_prioritized_bit_rate_et */
  /* CR 746 changes stop*/

  U8            bucket_size_duration;
/*^ M, 0, H, 1, 5 ^*/  /* rrc_bucket_size_duration_et */

  U8            logical_channel_group;
/*^ O, RRC_UL_SPECIFIC_PARAMETERS_LOGICAL_CH_GROUP_PRESENT, H, 1, 3 ^*/

} rrc_ul_specific_parameters_t;

/******************************************************************************
    TS 36.331 - 6.3.2 MAC-MainConfig IE
******************************************************************************/
typedef enum
{
  RRC_MAX_HARQ_TX_N_1,
  RRC_MAX_HARQ_TX_N_2,
  RRC_MAX_HARQ_TX_N_3,
  RRC_MAX_HARQ_TX_N_4,
  RRC_MAX_HARQ_TX_N_5,
  RRC_MAX_HARQ_TX_N_6,
  RRC_MAX_HARQ_TX_N_7,
  RRC_MAX_HARQ_TX_N_8,
  RRC_MAX_HARQ_TX_N_10,
  RRC_MAX_HARQ_TX_N_12,
  RRC_MAX_HARQ_TX_N_16,
  RRC_MAX_HARQ_TX_N_20,
  RRC_MAX_HARQ_TX_N_24,
  RRC_MAX_HARQ_TX_N_28
} rrc_max_harq_tx_et;

typedef enum
{
  RRC_PERIODIC_BSR_TIMER_SF_5,
  RRC_PERIODIC_BSR_TIMER_SF_10,
  RRC_PERIODIC_BSR_TIMER_SF_16,
  RRC_PERIODIC_BSR_TIMER_SF_20,
  RRC_PERIODIC_BSR_TIMER_SF_32,
  RRC_PERIODIC_BSR_TIMER_SF_40,
  RRC_PERIODIC_BSR_TIMER_SF_64,
  RRC_PERIODIC_BSR_TIMER_SF_80,
  RRC_PERIODIC_BSR_TIMER_SF_128,
  RRC_PERIODIC_BSR_TIMER_SF_160,
  RRC_PERIODIC_BSR_TIMER_SF_320,
  RRC_PERIODIC_BSR_TIMER_SF_640,
  RRC_PERIODIC_BSR_TIMER_SF_1280,
  RRC_PERIODIC_BSR_TIMER_SF_2560,
  RRC_PERIODIC_BSR_TIMER_INFINITY
}rrc_periodic_bsr_timer_et;

typedef enum
{
  RRC_RETX_BSR_TIMER_SF_320,
  RRC_RETX_BSR_TIMER_SF_640,
  RRC_RETX_BSR_TIMER_SF_1280,
  RRC_RETX_BSR_TIMER_SF_2560,
  RRC_RETX_BSR_TIMER_SF_5120,
  RRC_RETX_BSR_TIMER_SF_10240
}rrc_retx_bsr_timer_et;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/
#define RRC_UL_SCH_CONFIG_MAX_HARQ_TX_PRESENT          0x01
#define RRC_UL_SCH_CONFIG_PERIODIC_BSR_TIMER_PRESENT   0x02

  U8                    max_harq_tx;
/*^ O, RRC_UL_SCH_CONFIG_MAX_HARQ_TX_PRESENT, H, 1, 13 ^*/
/* rrc_max_harq_tx_et */

  U8                    periodic_bsr_timer;
/*^ O, RRC_UL_SCH_CONFIG_PERIODIC_BSR_TIMER_PRESENT, H, 1, 14 ^*/
/* rrc_periodic_bsr_timer_et */

  U8                    retx_bsr_timer;
/*^ M, 0, H, 0, 5 ^*/ /* rrc_retx_bsr_timer_et */

  U8                    tti_bundling;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_bool_et */

}rrc_ul_sch_config_t;

typedef enum
{
   RRC_ON_DURATION_TIMER_PSF_1,
   RRC_ON_DURATION_TIMER_PSF_2,
   RRC_ON_DURATION_TIMER_PSF_3,
   RRC_ON_DURATION_TIMER_PSF_4,
   RRC_ON_DURATION_TIMER_PSF_5,
   RRC_ON_DURATION_TIMER_PSF_6,
   RRC_ON_DURATION_TIMER_PSF_8,
   RRC_ON_DURATION_TIMER_PSF_10,
   RRC_ON_DURATION_TIMER_PSF_20,
   RRC_ON_DURATION_TIMER_PSF_30,
   RRC_ON_DURATION_TIMER_PSF_40,
   RRC_ON_DURATION_TIMER_PSF_50,
   RRC_ON_DURATION_TIMER_PSF_60,
   RRC_ON_DURATION_TIMER_PSF_80,
   RRC_ON_DURATION_TIMER_PSF_100,
   RRC_ON_DURATION_TIMER_PSF_200
}rrc_on_duration_timer_et;

typedef enum
{
   RRC_DRX_INACTIVITY_TIMER_PSF_1,
   RRC_DRX_INACTIVITY_TIMER_PSF_2,
   RRC_DRX_INACTIVITY_TIMER_PSF_3,
   RRC_DRX_INACTIVITY_TIMER_PSF_4,
   RRC_DRX_INACTIVITY_TIMER_PSF_5,
   RRC_DRX_INACTIVITY_TIMER_PSF_6,
   RRC_DRX_INACTIVITY_TIMER_PSF_8,
   RRC_DRX_INACTIVITY_TIMER_PSF_10,
   RRC_DRX_INACTIVITY_TIMER_PSF_20,
   RRC_DRX_INACTIVITY_TIMER_PSF_30,
   RRC_DRX_INACTIVITY_TIMER_PSF_40,
   RRC_DRX_INACTIVITY_TIMER_PSF_50,
   RRC_DRX_INACTIVITY_TIMER_PSF_60,
   RRC_DRX_INACTIVITY_TIMER_PSF_80,
   RRC_DRX_INACTIVITY_TIMER_PSF_100,
   RRC_DRX_INACTIVITY_TIMER_PSF_200,
   RRC_DRX_INACTIVITY_TIMER_PSF_300,
   RRC_DRX_INACTIVITY_TIMER_PSF_500,
   RRC_DRX_INACTIVITY_TIMER_PSF_750,
   RRC_DRX_INACTIVITY_TIMER_PSF_1280,
   RRC_DRX_INACTIVITY_TIMER_PSF_1920,
   RRC_DRX_INACTIVITY_TIMER_PSF_2560,
   RRC_DRX_INACTIVITY_TIMER_PSF_0_V1020
}rrc_drx_inactivity_timer_et;

typedef enum
{
   RRC_DRX_RETRANS_TIMER_PSF_1,
   RRC_DRX_RETRANS_TIMER_PSF_2,
   RRC_DRX_RETRANS_TIMER_PSF_4,
   RRC_DRX_RETRANS_TIMER_PSF_6,
   RRC_DRX_RETRANS_TIMER_PSF_8,
   RRC_DRX_RETRANS_TIMER_PSF_16,
   RRC_DRX_RETRANS_TIMER_PSF_24,
   RRC_DRX_RETRANS_TIMER_PSF_33
}rrc_drx_retransmission_timer_et;

typedef enum
{
  RRC_DRX_CONFIG_TYPE_REL,
  RRC_DRX_CONFIG_TYPE_SETUP
} rrc_drx_config_type_et;
typedef struct
{
  U16           bitmask;    /*^ BITMASK ^*/
#define RRC_LONG_DRX_CYCLE_START_OFFSET_SF_10_PRESENT         0x01
#define RRC_LONG_DRX_CYCLE_START_OFFSET_SF_20_PRESENT         0x02
#define RRC_LONG_DRX_CYCLE_START_OFFSET_SF_32_PRESENT         0x04
#define RRC_LONG_DRX_CYCLE_START_OFFSET_SF_40_PRESENT         0x08
#define RRC_LONG_DRX_CYCLE_START_OFFSET_SF_64_PRESENT         0x10
#define RRC_LONG_DRX_CYCLE_START_OFFSET_SF_80_PRESENT         0x20
#define RRC_LONG_DRX_CYCLE_START_OFFSET_SF_128_PRESENT        0x40
#define RRC_LONG_DRX_CYCLE_START_OFFSET_SF_160_PRESENT        0x80
#define RRC_LONG_DRX_CYCLE_START_OFFSET_SF_256_PRESENT        0x100
#define RRC_LONG_DRX_CYCLE_START_OFFSET_SF_320_PRESENT        0x200
#define RRC_LONG_DRX_CYCLE_START_OFFSET_SF_512_PRESENT        0x400
#define RRC_LONG_DRX_CYCLE_START_OFFSET_SF_640_PRESENT        0x800
#define RRC_LONG_DRX_CYCLE_START_OFFSET_SF_1024_PRESENT       0x1000
#define RRC_LONG_DRX_CYCLE_START_OFFSET_SF_1280_PRESENT       0x2000
#define RRC_LONG_DRX_CYCLE_START_OFFSET_SF_2048_PRESENT       0x4000
#define RRC_LONG_DRX_CYCLE_START_OFFSET_SF_2560_PRESENT       0x8000
  U8            sf_10;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_SF_10_PRESENT, H, 0, 9 ^*/

  U8            sf_20;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_SF_20_PRESENT, H, 0, 19 ^*/

  U8            sf_32;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_SF_32_PRESENT, H, 0, 31 ^*/

  U8            sf_40;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_SF_40_PRESENT, H, 0, 39 ^*/

  U8            sf_64;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_SF_64_PRESENT, H, 0, 63 ^*/

  U8            sf_80;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_SF_80_PRESENT, H, 0, 79 ^*/

  U8            sf_128;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_SF_128_PRESENT, H, 0, 127 ^*/

  U8            sf_160;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_SF_160_PRESENT, H, 0, 159 ^*/

  U8            sf_256;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_SF_256_PRESENT, N, 0, 255 ^*/

  U16           sf_320;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_SF_320_PRESENT, H, 0, 319 ^*/

  U16           sf_512;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_SF_512_PRESENT, H, 0, 511 ^*/

  U16           sf_640;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_SF_640_PRESENT, H, 0, 639 ^*/

  U16           sf_1024;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_SF_1024_PRESENT, H, 0, 1023 ^*/

  U16           sf_1280;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_SF_1280_PRESENT, H, 0, 1279 ^*/

  U16           sf_2048;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_SF_2048_PRESENT, H, 0, 2047 ^*/

  U16           sf_2560;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_SF_2560_PRESENT, H, 0, 2559 ^*/

}rrc_long_drx_cycle_start_offset_t;

typedef struct
{
  U32           bitmask;    /*^ BITMASK ^*/
#define RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_10_PRESENT         0x01
#define RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_20_PRESENT         0x02
#define RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_32_PRESENT         0x04
#define RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_40_PRESENT         0x08
#define RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_64_PRESENT         0x10
#define RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_80_PRESENT         0x20
#define RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_128_PRESENT        0x40
#define RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_160_PRESENT        0x80
#define RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_256_PRESENT        0x100
#define RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_320_PRESENT        0x200
#define RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_512_PRESENT        0x400
#define RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_640_PRESENT        0x800
#define RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_1024_PRESENT       0x1000
#define RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_1280_PRESENT       0x2000
#define RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_2048_PRESENT       0x4000
#define RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_2560_PRESENT       0x8000
#define RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_5120_PRESENT       0x10000
#define RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_10240_PRESENT      0x20000
  U8            sf_10;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_10_PRESENT, H, 0, 9 ^*/

  U8            sf_20;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_20_PRESENT, H, 0, 19 ^*/

  U8            sf_32;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_32_PRESENT, H, 0, 31 ^*/

  U8            sf_40;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_40_PRESENT, H, 0, 39 ^*/

  U8            sf_64;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_64_PRESENT, H, 0, 63 ^*/

  U8            sf_80;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_80_PRESENT, H, 0, 79 ^*/

  U8            sf_128;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_128_PRESENT, H, 0, 127 ^*/

  U8            sf_160;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_160_PRESENT, H, 0, 159 ^*/

  U8            sf_256;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_256_PRESENT, N, 0, 255 ^*/

  U16           sf_320;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_320_PRESENT, H, 0, 319 ^*/

  U16           sf_512;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_512_PRESENT, H, 0, 511 ^*/

  U16           sf_640;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_640_PRESENT, H, 0, 639 ^*/

  U16           sf_1024;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_1024_PRESENT, H, 0, 1023 ^*/

  U16           sf_1280;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_1280_PRESENT, H, 0, 1279 ^*/

  U16           sf_2048;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_2048_PRESENT, H, 0, 2047 ^*/

  U16           sf_2560;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_2560_PRESENT, H, 0, 2559 ^*/

  U16           sf_5120;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_5120_PRESENT, H, 0, 5119 ^*/

  U16           sf_10240;
/*^ O, RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_10240_PRESENT, H, 0, 10239 ^*/

}rrc_long_drx_cycle_start_offset_r15_t;

typedef enum
{
  RRC_SHORT_DRX_CYCLE_SF_2,
  RRC_SHORT_DRX_CYCLE_SF_5,
  RRC_SHORT_DRX_CYCLE_SF_8,
  RRC_SHORT_DRX_CYCLE_SF_10,
  RRC_SHORT_DRX_CYCLE_SF_16,
  RRC_SHORT_DRX_CYCLE_SF_20,
  RRC_SHORT_DRX_CYCLE_SF_32,
  RRC_SHORT_DRX_CYCLE_SF_40,
  RRC_SHORT_DRX_CYCLE_SF_64,
  RRC_SHORT_DRX_CYCLE_SF_80,
  RRC_SHORT_DRX_CYCLE_SF_128,
  RRC_SHORT_DRX_CYCLE_SF_160,
  RRC_SHORT_DRX_CYCLE_SF_256,
  RRC_SHORT_DRX_CYCLE_SF_320,
  RRC_SHORT_DRX_CYCLE_SF_512,
  RRC_SHORT_DRX_CYCLE_SF_640
} rrc_short_drx_cycle_et;

typedef struct
{
  U8        short_drx_cycle;
/*^ M, 0, H, 1, 15 ^*/  /* rrc_short_drx_cycle_et */

  U8        short_drx_cycle_timer;  /*^ M, 0, B, 1, 16 ^*/
}rrc_short_drx_t;

/* DRX_MAC_CE_CMD  Start */
typedef struct
{
  U16        drx_mac_ce_timer;
/*^ M, 0, B, 2, 2559 ^*/  

  U16        drx_mac_ce_cmd_threshold;  /*^ M, 0, B, 1, 2559 ^*/
}rrc_drx_mac_ce_cmd_trigger_info_t;
/* DRX_MAC_CE_CMD  End */

typedef struct
{
  rrc_bitmask_t     bitmask;      /*^ BITMASK ^*/
#define RRC_DRX_CONFIG_SHORT_DRX_PRESENT            0x01
/* DRX_MAC_CE_CMD  Start */
#define RRC_DRX_CONFIG_DRX_MAC_CE_CMD_TRIGGER_INFO_PRESENT     0x02
/* DRX_MAC_CE_CMD  End */
#define RRC_DRX_CONFIG_LONG_DRX_CYCLE_START_OFFSET_R15_PRESENT    0x04
  U8                on_duration_timer;
/*^ M, 0, H, 1, 15 ^*/    /* rrc_on_duration_timer_et */

  U8                drx_inactivity_timer;
/*^ M, 0, H, 1, 22 ^*/   /* rrc_drx_inactivity_timer_et */

  U8                drx_retransmission_timer;
/*^ M, 0, H, 1, 7 ^*/   /* rrc_drx_retransmission_timer_et */

  rrc_long_drx_cycle_start_offset_t        long_drx_cycle_start_offset;

  rrc_short_drx_t   short_drx;
/*^ O, RRC_DRX_CONFIG_SHORT_DRX_PRESENT ^*/
  
/* DRX_MAC_CE_CMD  Start */
  rrc_drx_mac_ce_cmd_trigger_info_t   drx_mac_ce_cmd_trigger_info;
/*^ O, RRC_DRX_CONFIG_DRX_MAC_CE_CMD_TRIGGER_INFO_PRESENT ^*/
/* DRX_MAC_CE_CMD  End */

  rrc_long_drx_cycle_start_offset_r15_t        long_drx_cycle_start_offset_r15;
/*^ O, RRC_DRX_CONFIG_LONG_DRX_CYCLE_START_OFFSET_R15_PRESENT ^*/
} rrc_drx_config_param_t;

/* The Release action should be supported for DRX-Config IE*/
typedef struct
{
  rrc_bitmask_t bitmask;    /*^ BITMASK ^*/
#define RRC_DRX_CONFIG_PARAM_PRESENT    0x01
  U8                        drx_config_type; 
/*^ M, 0, H, 0, 1 ^*/     /* rrc_drx_config_type_et */

  rrc_drx_config_param_t    drx_config_param;
/*^ O, RRC_DRX_CONFIG_PARAM_PRESENT ^*/
} rrc_drx_config_t;

typedef enum
{
  RRC_PERIOD_PHR_TIMER_SF_10,
  RRC_PERIOD_PHR_TIMER_SF_20,
  RRC_PERIOD_PHR_TIMER_SF_50,
  RRC_PERIOD_PHR_TIMER_SF_100,
  RRC_PERIOD_PHR_TIMER_SF_200,
  RRC_PERIOD_PHR_TIMER_SF_500,
  RRC_PERIOD_PHR_TIMER_SF_1000,
  RRC_PERIOD_PHR_TIMER_INFINITY
} rrc_periodic_phr_timer_et;

typedef enum
{
  RRC_PROHB_PHR_TIMER_SF_0,
  RRC_PROHB_PHR_TIMER_SF_10,
  RRC_PROHB_PHR_TIMER_SF_20,
  RRC_PROHB_PHR_TIMER_SF_50,
  RRC_PROHB_PHR_TIMER_SF_100,
  RRC_PROHB_PHR_TIMER_SF_200,
  RRC_PROHB_PHR_TIMER_SF_500,
  RRC_PROHB_PHR_TIMER_SF_1000
} rrc_prohibit_phr_timer_et;

typedef enum
{
  RRC_DL_PATHLOSS_CHANGE_DB_1,
  RRC_DL_PATHLOSS_CHANGE_DB_3,
  RRC_DL_PATHLOSS_CHANGE_DB_6,
  RRC_DL_PATHLOSS_CHANGE_INFINITY
} rrc_dl_pathloss_change_et;

typedef struct
{
  U8        periodic_phr_timer;
/*^ M, 0, H, 1, 7 ^*/      /* rrc_periodic_phr_timer_et */

  U8        prohibit_phr_timer;
/*^ M, 0, H, 1, 7 ^*/      /* rrc_prohibit_phr_timer_et */

  U8        dl_pathloss_change;
/*^ M, 0, H, 1, 3 ^*/      /* rrc_dl_pathloss_change_et */

} rrc_phr_config_param_t;

/* The Release action should be supported for phr-Config IE*/
typedef struct
{
  rrc_bitmask_t bitmask;    /*^ BITMASK ^*/
#define RRC_PHR_CONFIG_PARAM_PRESENT    0x01

  rrc_phr_config_param_t phr_config_param;
/*^ O, RRC_PHR_CONFIG_PARAM_PRESENT ^*/

} rrc_phr_config_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/
#define RRC_MAC_MAIN_CONFIG_UL_SCH_CONFIG_PRESENT        0x01
#define RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT           0x02
#define RRC_MAC_MAIN_CONFIG_PHR_CONFIG_PRESENT           0x04
#define RRC_MAC_MAIN_CONFIG_SR_PROHIBIT_TIMER_R9_PRESENT 0x08
#define RRC_MAC_MAIN_CONFIG_V1020_PRESENT                0x10
/* SPR 21958 PUSCH RAT1 Support Start */
#define RRC_MAC_MAIN_CONFIG_UPLINK_RAT1_SUPPORT_INFO_PRESENT 0x20
/* SPR 21958 PUSCH RAT1 Support End */

  rrc_ul_sch_config_t   ul_sch_config;
/*^ O, RRC_MAC_MAIN_CONFIG_UL_SCH_CONFIG_PRESENT ^*/

  rrc_drx_config_t      drx_config;
/*^ O, RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT ^*/

  U8                    time_alignment_timer_dedicated;
/*^ M, 0, H, 0, 7 ^*/ /* rrm_time_align_timer_et */

  rrc_phr_config_t      phr_config;
/*^ O, RRC_MAC_MAIN_CONFIG_PHR_CONFIG_PRESENT ^*/

  U8                    sr_prohibit_timer_r9;
  /*^ O, RRC_MAC_MAIN_CONFIG_SR_PROHIBIT_TIMER_R9_PRESENT, H, 0, 7 ^*/

  rrc_mac_main_config_v1020_t         mac_main_config_v1020;
  /*^ O, RRC_MAC_MAIN_CONFIG_V1020_PRESENT, N, 0, 0 ^*/
/* SPR 21958 PUSCH RAT1 Support Start */ 
  U8                    is_uplink_rat1_supported;
  /*^ O, RRC_MAC_MAIN_CONFIG_UPLINK_RAT1_SUPPORT_INFO_PRESENT, H, 0, 1 ^*/
/* SPR 21958 PUSCH RAT1 Support End */
} rrc_mac_main_config_t;

/******************************************************************************
    TS 36.331 - 6.3.2 SPS-Config IE
******************************************************************************/

typedef enum
{

  RRC_SEMI_PERSIST_INT_SF_10,
  RRC_SEMI_PERSIST_INT_SF_20,
  RRC_SEMI_PERSIST_INT_SF_32,
  RRC_SEMI_PERSIST_INT_SF_40,
  RRC_SEMI_PERSIST_INT_SF_64,
  RRC_SEMI_PERSIST_INT_SF_80,
  RRC_SEMI_PERSIST_INT_SF_128,
  RRC_SEMI_PERSIST_INT_SF_160,
  RRC_SEMI_PERSIST_INT_SF_320,
  RRC_SEMI_PERSIST_INT_SF_640
} rrc_semi_persist_sched_interval_et;

typedef enum
{
  RRC_IMPLICIT_RELEASE_E_2,
  RRC_IMPLICIT_RELEASE_E_3,
  RRC_IMPLICIT_RELEASE_E_4,
  RRC_IMPLICIT_RELEASE_E_8
} rrc_implicit_release_after_et;

typedef struct
{
/*SPR_17664_start*/
  U8    count;      /*^ M, 0, B, 1,MAX_N1_PUCCH_AN_PERSIST_SIZE ^*/
/*SPR_17664_end*/
  U16   n1_pucch_an_persist[MAX_N1_PUCCH_AN_PERSIST_SIZE];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}rrc_n1_pucch_an_persist_list_t;

typedef struct
{
  U8                              semi_persist_sched_interval_dl;
/*^ M, 0, H, 0, 9 ^*/  /* rrc_semi_persist_sched_interval_et */

  U8                              number_of_conf_sps_processes;       /*^ M, 0, B, 1, 8 ^*/
  rrc_n1_pucch_an_persist_list_t  n1_pucch_an_persist_list;
}rrc_sps_config_dl_param_t;

typedef struct
{
    rrc_bitmask_t               bitmask;    /*^ BITMASK ^*/
#define RRC_SPS_CONFIG_DL_PARAM_PRESENT 0x01
/* SPS Start */
#define RRC_SPS_CONFIG_DL_PARAM_MAX_SPS_HARQ_RETX 0x02
#define RRC_SPS_CONFIG_DL_PARAM_EXPLICIT_RELEASE_AFTER 0x04
/* SPS Stop */

    rrc_sps_config_dl_param_t   sps_config_dl_param;
/*^ O, RRC_SPS_CONFIG_DL_PARAM_PRESENT ^*/

    /* SPS Start */
    U8    max_sps_harq_retx;
    /*^ O, RRC_SPS_CONFIG_DL_PARAM_MAX_SPS_HARQ_RETX, B, 1, 8 ^*/
    U8    expilcit_release_after;
    /*^ O, RRC_SPS_CONFIG_DL_PARAM_EXPLICIT_RELEASE_AFTER, B, 2, 8 ^*/

    /* SPS Stop */

} rrc_sps_config_dl_t;

typedef struct
{
  S8   p_zero_nominal_pusch_persistent;    /*^ M, 0, B, -126, 24 ^*/
  S8   p_zero_ue_pusch_persistent;         /*^ M, 0, B, -8, 7 ^*/
} rrc_p_zero_persistent_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/
#define RRC_SPS_CONFIG_UL_P_0_PERSISTENT_PRESENT                0x01
#define RRC_SPS_CONFIG_UL_TWO_INTERVALS_CONFIG_PRESENT          0x02

  U8          semi_persist_sched_interval_ul;
/*^ M, 0, H, 0, 9 ^*/  /* rrc_semi_persist_sched_interval_et */

  U8          implicit_release_after;
/*^ M, 0, H, 0, 3 ^*/  /* rrc_implicit_release_after_et */

  rrc_p_zero_persistent_t   p_zero_persistent;
/*^ O, RRC_SPS_CONFIG_UL_P_0_PERSISTENT_PRESENT ^*/

  U8          two_intervals_config;
/*^ O, RRC_SPS_CONFIG_UL_TWO_INTERVALS_CONFIG_PRESENT ^*/
/* This is just a flag and it's value is not used, only bitmask matters.
   In future version we can use only bitmask
   RRC_SPS_CONFIG_UL_TWO_INTERVALS_CONFIG_PRESENT without two_intervals_config
   field */

} rrc_sps_config_ul_param_t;

typedef struct
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_SPS_CONFIG_UL_PARAM_PRESENT 0x01

    rrc_sps_config_ul_param_t     sps_config_ul_param;
/*^ O, RRC_SPS_CONFIG_UL_PARAM_PRESENT ^*/

} rrc_sps_config_ul_t;

typedef struct
{
  U16               bitmask;           /*^ BITMASK ^*/
#define RRC_SPS_CONFIG_C_RNTI_PRESENT             0x01
#define RRC_SPS_CONFIG_SPS_CONFIG_DL_PRESENT      0x02
#define RRC_SPS_CONFIG_SPS_CONFIG_UL_PRESENT      0x04

  U8                semi_presist_sched_c_rnti[C_RNTI_OCTET_SIZE];
/*^ O, RRC_SPS_CONFIG_C_RNTI_PRESENT, OCTET_STRING, FIXED ^*/

  rrc_sps_config_dl_t   sps_config_dl;
/*^ O, RRC_SPS_CONFIG_SPS_CONFIG_DL_PRESENT ^*/

  rrc_sps_config_ul_t   sps_config_ul;
/*^ O, RRC_SPS_CONFIG_SPS_CONFIG_UL_PRESENT ^*/

} rrc_sps_config_t;


/******************************************************************************
    TS 36.331 - 6.3.5 Measurement information elements
******************************************************************************/

typedef struct
{
/*SPR_17664_start*/
  U8    count;          /*^ M, 0, H, 0, MAX_MEAS_OBJECT_ID ^*/
/*SPR_17664_end*/
  U8    meas_object_id[MAX_MEAS_OBJECT_ID];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}meas_object_to_remove_list_t;

typedef struct
{
/*SPR_17664_start*/
  U8    count;          /*^ M, 0, H, 0, MAX_CELL_MEAS ^*/
/*SPR_17664_end*/
  U8    cell_index[MAX_CELL_MEAS]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}cell_index_list_t;

typedef struct
{
  U8    cell_index;         /*^ M, 0, H, 0, 32 ^*/
  U16   phys_cell_id;       /*^ M, 0, H, 0, 503 ^*/
  U8    cell_individual_offset; /*^ M, 0, H, 0, 30 ^*/
}cells_to_add_mod_t;

typedef struct
{
/*SPR_17664_start*/
  U8                    count;  /*^ M, 0, H, 0,MAX_CELL_MEAS ^*/
/*SPR_17664_end*/
  cells_to_add_mod_t    cells_to_add_mod[MAX_CELL_MEAS];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}cells_to_add_mod_list_t;

typedef struct
{
  U8                    cell_index;         /*^ M, 0, H, 0, 32 ^*/
  phy_cell_id_range_t   phys_cell_id_range;
}black_cells_to_add_mod_t;

typedef struct
{
/*SPR_17664_start*/
  U8                    count;  /*^ M, 0, H, 0, MAX_CELL_MEAS ^*/
/*SPR_17664_end*/
  black_cells_to_add_mod_t  black_cells_to_add_mod[MAX_CELL_MEAS];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}black_cells_to_add_mod_list_t;

/* eICIC changes start */
typedef struct
{
/* SPR-18445 start */
    U8                                     count;  /*^ M, 0, H, 0, MAX_CELL_MEAS ^*/
/* SPR-18445 stop */
    phy_cell_id_range_t                    meas_subframe_cells[MAX_CELL_MEAS];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
    
}meas_subframe_cell_list_r10_t;

typedef struct
{
#define MEAS_SUBFRAME_CELL_LIST_R10_PRESENT    0x01
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
    rrc_phy_meas_subframe_pattern_r10_t            meas_subframe_pattern_neigh_r10;  /*^ M, 0, N, 0, 0 ^*/
    meas_subframe_cell_list_r10_t                  meas_subframe_cell_list_r10;
/*^ O, MEAS_SUBFRAME_CELL_LIST_R10_PRESENT, N, 0, 0 ^*/
}meas_subframe_pattern_config_neigh_r10_setup_t;
typedef struct
{
#define MEAS_SUBFRAME_PATTERN_CONFIG_NEIGH_R10_RELEASE_PRESENT     0x01
#define MEAS_SUBFRAME_PATTERN_CONFIG_NEIGH_R10_SETUP_PRESENT       0x02

   rrc_bitmask_t        bitmask;               /*^ BITMASK ^*/ 
   meas_subframe_pattern_config_neigh_r10_setup_t   meas_subframe_pattern_config_neigh_r10_setup;
   /*^ O, MEAS_SUBFRAME_PATTERN_CONFIG_NEIGH_R10_SETUP_PRESENT ^*/

}meas_subframe_pattern_config_neigh_r10_t;   
/* eICIC changes stop */

 
typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define MEAS_OBJECT_EUTRA_CELLS_TO_REMOVE_LIST_PRESENT              0x01
#define MEAS_OBJECT_EUTRA_CELLS_TO_ADD_MOD_LIST_PRESENT             0x02
#define MEAS_OBJECT_EUTRA_BLACK_CELLS_TO_REMOVE_LIST_PRESENT        0x04
#define MEAS_OBJECT_EUTRA_BLACK_CELLS_TO_ADD_MOD_LIST_PRESENT       0x08
#define MEAS_OBJECT_EUTRA_CELL_FOR_WHICH_TO_REPORT_CGI_PRESENT      0x10
#define MEAS_OBJECT_EUTRA_CYCLE_SCELL_R10_PRESENT                   0x20
/* eICIC changes start */
#define MEAS_SUBFRAME_PATTERN_CONFIG_NEIGH_R10_PRESENT              0x40
/* eICIC changes stop */

  U32   carrier_freq;
  U8    allowed_meas_bandwidth;
/*^ M, 0, H, 0, 5 ^*/ /*ENUMERATED {mbw6, mbw15, mbw25, mbw50, mbw75, mbw100}*/

  U8    presence_antenna_port1; /*^ M, 0, H, 0, 1 ^*/ /* rrc_bool_et */
  U8    neigh_cell_config;              /* BIT STRING (SIZE (2)) */
  U8    offset_freq;            /*^ M, 0, H, 0, 30 ^*/ /* rrm_q_offset_range_et */
  cell_index_list_t         cells_to_remove_list;
/*^ O, MEAS_OBJECT_EUTRA_CELLS_TO_REMOVE_LIST_PRESENT ^*/

  cells_to_add_mod_list_t   cells_to_add_mod_list;
/*^ O, MEAS_OBJECT_EUTRA_CELLS_TO_ADD_MOD_LIST_PRESENT ^*/

  cell_index_list_t black_cells_to_remove_list;
/*^ O, MEAS_OBJECT_EUTRA_BLACK_CELLS_TO_REMOVE_LIST_PRESENT ^*/

  black_cells_to_add_mod_list_t black_cells_to_add_mod_list;
/*^ O, MEAS_OBJECT_EUTRA_BLACK_CELLS_TO_ADD_MOD_LIST_PRESENT ^*/

  U16   cell_for_which_to_report_cgi;
/*^ O, MEAS_OBJECT_EUTRA_CELL_FOR_WHICH_TO_REPORT_CGI_PRESENT, H, 0, 503 ^*/

  U8    meas_cycle_scell_r10;
/*^ O, MEAS_OBJECT_EUTRA_CYCLE_SCELL_R10_PRESENT, H, 0, 7 ^*/
  /*meas_cycle_scell_r10_et*/

/* eICIC changes start */
   meas_subframe_pattern_config_neigh_r10_t     meas_subframe_pattern_config_neigh_r10;
/*^ O, MEAS_SUBFRAME_PATTERN_CONFIG_NEIGH_R10_PRESENT ^*/
/* eICIC changes stop */

}meas_object_eutra_t;

typedef struct
{
  U8    cell_index;         /*^ M, 0, H, 0, 32 ^*/
  U16   phys_cell_id;       /*^ M, 0, H, 0, 511 ^*/
}cells_to_add_mod_utra_fdd_t;

typedef struct
{
/*SPR_17664_start*/
  U8                    count;  /*^ M, 0, H, 0, MAX_CELL_MEAS ^*/
/*SPR_17664_end*/
  cells_to_add_mod_utra_fdd_t   cells_to_add_mod_utra_fdd[MAX_CELL_MEAS];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}cells_to_add_mod_list_utra_fdd_t;

typedef struct
{
  U8    cell_index;         /*^ M, 0, H, 0, 32 ^*/
  U8    phys_cell_id;       /*^ M, 0, H, 0, 127 ^*/
}cells_to_add_mod_utra_tdd_t;

typedef struct
{
/*SPR_17664_start*/
  U8                            count;  /*^ M, 0, H, 0, MAX_CELL_MEAS ^*/
/*SPR_17664_end*/
  cells_to_add_mod_utra_tdd_t   cells_to_add_mod_utra_tdd[MAX_CELL_MEAS];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}cells_to_add_mod_list_utra_tdd_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_FDD_PRESENT          0x01
#define MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_TDD_PRESENT          0x02

  cells_to_add_mod_list_utra_fdd_t cells_to_add_mod_list_utra_fdd;
/*^ O, MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_FDD_PRESENT ^*/

  cells_to_add_mod_list_utra_tdd_t cells_to_add_mod_list_utra_tdd;
/*^ O, MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_TDD_PRESENT ^*/

}meas_object_utra_cells_to_add_mod_list_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/
#define MEAS_OBJECT_UTRA_CELL_FOR_WHICH_TO_REPORT_CGI_UTRA_FDD_PRESENT  0x01
#define MEAS_OBJECT_UTRA_CELL_FOR_WHICH_TO_REPORT_CGI_UTRA_TDD_PRESENT  0x02

  U16 utra_fdd;
/*^ O,MEAS_OBJECT_UTRA_CELL_FOR_WHICH_TO_REPORT_CGI_UTRA_FDD_PRESENT,H,0,511 ^*/

  U8 utra_tdd;
/*^ O,MEAS_OBJECT_UTRA_CELL_FOR_WHICH_TO_REPORT_CGI_UTRA_TDD_PRESENT,H,0,127 ^*/

}meas_object_utra_cell_for_which_to_report_cgi_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define MEAS_OBJECT_UTRA_CELLS_TO_REMOVE_LIST_PRESENT               0x01
#define MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_PRESENT              0x02
#define MEAS_OBJECT_UTRA_CELL_FOR_WHICH_TO_REPORT_CGI_PRESENT       0x04

  U16   carrier_freq;   /*^ M, 0, H, 0, 16383 ^*/
  S8    offset_freq;    /*^ M, 0, B, -15, 15 ^*/
  cell_index_list_t         cells_to_remove_list;
/*^ O, MEAS_OBJECT_UTRA_CELLS_TO_REMOVE_LIST_PRESENT ^*/

  meas_object_utra_cells_to_add_mod_list_t  cells_to_add_mod_list;
/*^ O, MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_PRESENT ^*/

  meas_object_utra_cell_for_which_to_report_cgi_t  cell_for_which_to_report_cgi;
/*^ O, MEAS_OBJECT_UTRA_CELL_FOR_WHICH_TO_REPORT_CGI_PRESENT ^*/

}meas_object_utra_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define MEAS_OBJECT_GERAN_CELL_FOR_WHICH_TO_REPORT_CGI_PRESENT      0x01

  carrier_freqs_geran_t carrier_freqs;
  S8    offset_freq;    /*^ M, 0, B, -15, 15 ^*/    /*Default value is 0*/
  U8    ncc_Permitted;  /*BIT STRING(SIZE (8)), DEFAULT '11111111'B*/
  phys_cell_id_geran_t  cell_for_which_to_report_cgi;
/*^ O, MEAS_OBJECT_GERAN_CELL_FOR_WHICH_TO_REPORT_CGI_PRESENT ^*/

}meas_object_geran_t;

typedef enum
{
  RRM_RRC_TYPE1_XRTT,
  RRM_RRC_TYPE_HRPD
}rrm_meas_object_cdma2000_type_et;

typedef enum
{
  RRM_RRC_BC_0,
  RRM_RRC_BC_1,
  RRM_RRC_BC_2,
  RRM_RRC_BC_3,
  RRM_RRC_BC_4,
  RRM_RRC_BC_5,
  RRM_RRC_BC_6,
  RRM_RRC_BC_7,
  RRM_RRC_BC_8,
  RRM_RRC_BC_9,
  RRM_RRC_BC_10,
  RRM_RRC_BC_11,
  RRM_RRC_BC_12,
  RRM_RRC_BC_13,
  RRM_RRC_BC_14,
  RRM_RRC_BC_15,
  RRM_RRC_BC_16,
  RRM_RRC_BC_17,
  RRM_RRC_BC_18_V9A0,
  RRM_RRC_BC_19_V9A0,
  RRM_RRC_BC_20_V9A0,
  RRM_RRC_BC_21_V9A0
} rrm_band_class_cdma2000_et;

typedef struct
{
  U8    cell_index;         /*^ M, 0, H, 0, 32 ^*/
  U16   phys_cell_id;       /*^ M, 0, H, 0, 511 ^*/
}cells_to_add_mod_cdma2000_t;

typedef struct
{
/*SPR_17664_start*/
  U8                            count;  /*^ M, 0, H, 0, MAX_CELL_MEAS ^*/
/*SPR_17664_end*/
  cells_to_add_mod_cdma2000_t   cells_to_add_mod_cdma2000[MAX_CELL_MEAS];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}cells_to_add_mod_list_cdma2000_list_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define MEAS_OBJECT_CDMA2000_SEARCH_WINDOW_SIZE_PRESENT             0x01
#define MEAS_OBJECT_CDMA2000_CELLS_TO_REMOVE_LIST_PRESENT           0x02
#define MEAS_OBJECT_CDMA2000_CELLS_TO_ADD_MOD_LIST_PRESENT          0x04
#define MEAS_OBJECT_CDMA2000_CELL_FOR_WHICH_TO_REPEORT_CGI_PRESENT  0x08

  U8                                    cdma2000_type;
/*^ M, 0, H, 0, 1 ^*/  /* rrm_meas_object_cdma2000_type_et */

  carrier_freq_cdma2000_t               carrier_freq_cdma2000;
  U8                                    search_window_size;
/*^ O, MEAS_OBJECT_CDMA2000_SEARCH_WINDOW_SIZE_PRESENT, H, 0, 15 ^*/

  S8                                    offset_freq;
/*^ M, 0, B, -15, 15 ^*/

  cell_index_list_t                     cells_to_remove_list;
/*^ O, MEAS_OBJECT_CDMA2000_CELLS_TO_REMOVE_LIST_PRESENT ^*/

  cells_to_add_mod_list_cdma2000_list_t cells_to_add_mod_list;
/*^ O, MEAS_OBJECT_CDMA2000_CELLS_TO_ADD_MOD_LIST_PRESENT ^*/

  U16                                   cells_for_which_to_report_cgi;
/*^ O, MEAS_OBJECT_CDMA2000_CELL_FOR_WHICH_TO_REPEORT_CGI_PRESENT, H, 0, 511 ^*/

}meas_object_cdma2000_t;



#define MAX_WLAN_BAND           2
#define MAX_WLAN_CHANNEL        16
#define MAX_WLAN_CARRIER_INFO   8
#define MAX_CELL_INFO_WLAN_R13  8

typedef enum
{
  WLAN_BAND_2_DOT_4   = 0,
  WLAN_BAND_5         = 1,
  WLAN_BAND_SPARE6_9  = 2,
  WLAN_BAND_SPARE5_11 = 3,
  WLAN_BAND_SPARE4_15 = 4,
  WLAN_BAND_SPARE3_20 = 5,
  WLAN_BAND_SPARE2_30 = 6,
  WLAN_BAND_SPARE1_56 = 7,
}wlan_band_et;

typedef enum 
{
  WLAN_COUNTRY_CODE_US,
  WLAN_COUNTRY_CODE_EUROPE,
  WLAN_COUNTRY_CODE_JAPAN,
  WLAN_COUNTRY_CODE_GLOBAL
}wlan_country_code_et;

typedef enum
{
  RRM_RRC_WLAN_RSSI
}quantity_config_wlan_r13_et;


typedef struct 
{
  U8               count;
/*^ M, 0, H, 0, MAX_WLAN_BAND ^*/

  U8               wlan_band[MAX_WLAN_BAND];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/ 
/* wlan_band_et */

}wlan_band_t;

typedef struct
{
  U8               count;
/*^ M, 0, H, 0, MAX_WLAN_CHANNEL ^*/

  U8               channel[MAX_WLAN_CHANNEL];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
 
}wlan_channel_t;

typedef struct
{
  rrc_bitmask_t            bitmask;     /*^ BITMASK ^*/

#define WLAN_CARRIER_INFO_OPERATING_CLASS_PRESENT   0x01
#define WLAN_CARRIER_INFO_COUNTRY_CODE_PRESENT      0x02
#define WLAN_CARRIER_INFO_CHANNEL_LIST_PRESENT      0x04

  U8                       operating_class;
/*^ O, WLAN_CARRIER_INFO_OPERATING_CLASS_PRESENT, H, 0, 255 ^*/

  U8                       country_code; 
/*^ O, WLAN_CARRIER_INFO_COUNTRY_CODE_PRESENT, H, 0, 3 ^*/
/* wlan_country_code_et */

  wlan_channel_t           channel_list;  
/*^ O, WLAN_CARRIER_INFO_CHANNEL_LIST_PRESENT ^*/

}wlan_carrier_info_t;

typedef struct 
{
  U8                    count;
/*^ M, 0, H, 0, MAX_WLAN_CARRIER_INFO ^*/

  wlan_carrier_info_t   wlan_carrier_info[MAX_WLAN_CARRIER_INFO];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}wlan_carrier_info_list_t;

typedef struct 
{
  rrc_bitmask_t              bitmask;    /*^ BITMASK ^*/

#define WLAN_CARRIER_FREQ_BAND_LIST_PRESENT          0x01
#define WLAN_CARRIER_FREQ_CARRIER_INFO_LIST_PRESENT  0x02

  wlan_band_t                wlan_band_list;
/*^ O, WLAN_CARRIER_FREQ_BAND_LIST_PRESENT ^*/

  wlan_carrier_info_list_t   wlan_carrier_info_list;
/*^ O, WLAN_CARRIER_FREQ_CARRIER_INFO_LIST_PRESENT ^*/

}meas_object_wlan_carrier_freq_t;

typedef struct 
{
  U8                           count;
/*^ M, 0, H, 0, MAX_WLAN_ID ^*/

  wlan_identifier_t            wlan_to_add_mod[MAX_WLAN_ID];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
 
}meas_object_wlan_to_add_mod_list_t;

typedef struct 
{
  U8                           count;
/*^ M, 0, H, 0, MAX_WLAN_ID ^*/

  wlan_identifier_t            wlan_to_remove[MAX_WLAN_ID];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
 
}wlan_index_list_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define MEAS_OBJECT_WLAN_CARRIER_FREQ_PRESENT         0x01
#define MEAS_OBJECT_WLAN_ID_TO_ADD_MOD_LIST_PRESENT   0x02
#define MEAS_OBJECT_WLAN_ID_TO_REMOVE_LIST_PRESENT    0x04

  meas_object_wlan_carrier_freq_t       carrier_freq;
/*^ O, MEAS_OBJECT_WLAN_CARRIER_FREQ_PRESENT ^*/

  meas_object_wlan_to_add_mod_list_t    wlan_to_add_mod_list;
/*^ O, MEAS_OBJECT_WLAN_ID_TO_ADD_MOD_LIST_PRESENT ^*/

  wlan_index_list_t                     wlan_to_remove_list;
/*^ O, MEAS_OBJECT_WLAN_ID_TO_REMOVE_LIST_PRESENT ^*/

}meas_object_wlan_t;

/* ENDC_MEAS_CHANGES_START */
typedef enum
{
   SFTD_PSCELL,
   SFTD_NEIGHBORCELLS 
}rrm_report_sftd_meas_r15_et;

typedef struct
{
    U8     count;
    /*^ M, 0, B, 1, MAX_CELL_SFTD ^*/

    U16    cells_to_report_SFTD_r15[MAX_CELL_SFTD]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}cells_to_report_SFTD_r15_list_t;

typedef struct
{
    U8     cell_index_r15;
    /*^ M, 0, B, 1, 32 ^*/ 

    U16    phys_cellid_r15;
    /*^ M, 0, H, 0, 1007 ^*/
}black_cells_to_add_mod_r15_t;

typedef struct
{
    U8                              count;
    /*^ M, 0, H, 0, MAX_CELL_MEAS ^*/ 

    black_cells_to_add_mod_r15_t    black_cells_to_add_mod[MAX_CELL_MEAS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}black_cells_to_add_mod_list_r15_t;

typedef struct
{
    U16                                bitmask; /*^ BITMASK ^*/

#define THRESHOLD_LIST_NR_RSRP_PRESENT    0x01
#define THRESHOLD_LIST_NR_RSRQ_PRESENT    0x02
#define THRESHOLD_LIST_NR_SINR_PRESENT    0x04
    U8    rsrp_nr_r15;         /*^ O, THRESHOLD_LIST_NR_RSRP_PRESENT, H, 0, 127 ^*/
    U8    rsrq_nr_r15;         /*^ O, THRESHOLD_LIST_NR_RSRQ_PRESENT, H, 0, 127 ^*/
    U8    sinr_nr_r15;         /*^ O, THRESHOLD_LIST_NR_SINR_PRESENT, H, 0, 127 ^*/

}threshold_list_nr_r15_t;

typedef enum
{
  SUBCARRIER_SPACING_KHZ_15,
  SUBCARRIER_SPACING_KHZ_30,
  SUBCARRIER_SPACING_KHZ_120,
  SUBCARRIER_SPACING_KHZ_240
}rrm_subcarrier_spacing_ssb_r15_et;

typedef enum
{
   SSB_DURATION_SF1,
   SSB_DURATION_SF2,
   SSB_DURATION_SF3,
   SSB_DURATION_SF4,
   SSB_DURATION_SF5
}rrm_ssb_duration_r15_et;

typedef struct
{
    U16                                bitmask; /*^ BITMASK ^*/
#define PERIODICITY_AND_OFFSET_SF5_PRESENT        0x01
#define PERIODICITY_AND_OFFSET_SF10_PRESENT       0x02
#define PERIODICITY_AND_OFFSET_SF20_PRESENT       0x04
#define PERIODICITY_AND_OFFSET_SF40_PRESENT       0x08
#define PERIODICITY_AND_OFFSET_SF80_PRESENT       0x10
#define PERIODICITY_AND_OFFSET_SF160_PRESENT      0x20 
    U8    sf5_r15; /*^ O, PERIODICITY_AND_OFFSET_SF5_PRESENT , H, 0, 4 ^*/
    U8    sf10_r15; /*^ O, PERIODICITY_AND_OFFSET_SF10_PRESENT , H, 0, 9 ^*/
    U8    sf20_r15; /*^ O, PERIODICITY_AND_OFFSET_SF20_PRESENT , H, 0, 19 ^*/
    U8    sf40_r15; /*^ O, PERIODICITY_AND_OFFSET_SF40_PRESENT , H, 0, 39 ^*/
    U8    sf80_r15; /*^ O, PERIODICITY_AND_OFFSET_SF80_PRESENT , H, 0, 79 ^*/
    U8    sf160_r15; /*^ O, PERIODICITY_AND_OFFSET_SF160_PRESENT , H, 0, 159 ^*/ 
}periodicity_and_offfset_r15_t;

typedef struct
{
    periodicity_and_offfset_r15_t  periodicity_and_offfset_r15;
    /*^ M, 0 , N, 0, 0 ^*/
    U8                             ssb_duration_r15;    /* rrm_ssb_duration_r15_et */
    /*^ M, 0, H, 0, 4 ^*/
}mtc_ssb_nr_r15_t;

typedef struct
{
  mtc_ssb_nr_r15_t   meas_timing_config_r15;
  /*^ M, 0 , N, 0, 0 ^*/
  U8                 subcarrier_spacing_ssb_r15;
  /*^ M, 0, H, 0, 3 ^*/ /* rrm_subcarrier_spacing_ssb_r15_et */

}rs_config_ssb_nr_r15_t;

typedef struct
{
    U16                                bitmask; /*^ BITMASK ^*/

#define MEAS_OBJECT_NR_THRESHHOLD_RS_INDEX_PRESENT          0x01
#define MEAS_OBJECT_NR_MAX_RS_IDX_CELL_QUAL_PRESENT         0x02
#define MEAS_OBJECT_NR_BLACK_CELLS_TO_REMOVE_LIST_PRESENT   0x04
#define MEAS_OBJECT_NR_BLACK_CELLS_TO_ADD_MOD_LIST_PRESENT  0x08
#define MEAS_OBJECT_NR_CELLS_TO_REPORT_SFTD_PRESENT         0x10
    U32                      carrier_freq_r15;
    /*^ M, 0, H, 0, 3279165 ^*/

    rs_config_ssb_nr_r15_t   rs_config_ssb_nr_r15;
    /*^ M, 0, N, 0, 0 ^*/

    S8                       offset_freq_r15;  
    /*^ M, 0, B, -15, 15 ^*/

    U8                         quantity_config_set_r15; 
    /*^ M, 0, B, 1, 2 ^*/

    threshold_list_nr_r15_t    thresh_rs_index_r15;
    /*^ O, MEAS_OBJECT_NR_THRESHHOLD_RS_INDEX_PRESENT, N, 0, 0 ^*/

    U8                       max_rs_index_cell_qual_r15;
    /*^ O, MEAS_OBJECT_NR_MAX_RS_IDX_CELL_QUAL_PRESENT, B, 1, 16 ^*/

    cell_index_list_t        black_cells_to_remove_list; 
    /*^ O, MEAS_OBJECT_NR_BLACK_CELLS_TO_REMOVE_LIST_PRESENT, N, 0, 0 ^*/

    black_cells_to_add_mod_list_r15_t          black_cells_to_add_mod_list;
    /*^ O, MEAS_OBJECT_NR_BLACK_CELLS_TO_ADD_MOD_LIST_PRESENT, N, 0, 0 ^*/

    cells_to_report_SFTD_r15_list_t            cells_to_report_SFTD_r15_list;
    /*^ O, MEAS_OBJECT_NR_CELLS_TO_REPORT_SFTD_PRESENT, N, 0, 0 ^*/
}meas_object_nr_t;

/* ENDC_MEAS_CHANGES_END */

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define MEAS_OBJECT_TO_ADD_EUTRA_PRESENT            0x01
#define MEAS_OBJECT_TO_ADD_UTRA_PRESENT             0x02
#define MEAS_OBJECT_TO_ADD_GERAN_PRESENT            0x04
#define MEAS_OBJECT_TO_ADD_CDMA2000_PRESENT         0x08
#define MEAS_OBJECT_TO_ADD_WLAN_PRESENT             0x10
/* ENDC_MEAS_CHANGES_START */
#define MEAS_OBJECT_TO_ADD_NR_PRESENT               0x20
/* ENDC_MEAS_CHANGES_END */

  meas_object_eutra_t       meas_object_eutra;
/*^ O, MEAS_OBJECT_TO_ADD_EUTRA_PRESENT ^*/

  meas_object_utra_t        meas_object_utra;
/*^ O, MEAS_OBJECT_TO_ADD_UTRA_PRESENT ^*/

  meas_object_geran_t       meas_object_geran;
/*^ O, MEAS_OBJECT_TO_ADD_GERAN_PRESENT ^*/

  meas_object_cdma2000_t    meas_object_cdma2000;
/*^ O, MEAS_OBJECT_TO_ADD_CDMA2000_PRESENT ^*/

  meas_object_wlan_t        meas_object_wlan;
/*^ O, MEAS_OBJECT_TO_ADD_WLAN_PRESENT ^*/
  
/* ENDC_MEAS_CHANGES_START */
  meas_object_nr_t        meas_object_nr;
/*^ O, MEAS_OBJECT_TO_ADD_NR_PRESENT ^*/
/* ENDC_MEAS_CHANGES_END */

}meas_object_to_add_mod_meas_object_t;

typedef struct
{
  U8                                    meas_object_id; /*^ M, 0, B, 1, 32 ^*/
  meas_object_to_add_mod_meas_object_t  meas_object;
}meas_object_to_add_mod_t;

typedef struct
{
/*SPR_17664_start*/
  U8                        count;          /*^ M, 0, H, 0, MAX_MEAS_OBJECT_ID ^*/
/*SPR_17664_end*/
  meas_object_to_add_mod_t  meas_object_to_add_mod[MAX_MEAS_OBJECT_ID];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}meas_object_to_add_mod_list_t;

typedef struct
{
/*SPR_17664_start*/
  U8    count;  /*^ M, 0, H, 0, MAX_REPORT_CONFIG_ID ^*/
/*SPR_17664_end*/
  U8    report_config_id[MAX_REPORT_CONFIG_ID];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}report_config_to_remove_list_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define THRESHOLD_EUTRA_RSRP_PRESENT            0x01
#define THRESHOLD_EUTRA_RSRQ_PRESENT            0x02

  U8    threshold_rsrp;     /*^ O, THRESHOLD_EUTRA_RSRP_PRESENT, H, 0, 97 ^*/
  U8    threshold_rsrq;     /*^ O, THRESHOLD_EUTRA_RSRQ_PRESENT, H, 0, 34 ^*/
}threshold_eutra_t;

typedef struct
{
  threshold_eutra_t a1_threshold;
}report_config_eutra_trigger_type_event_eventid_event_a1_t;

typedef struct
{
  threshold_eutra_t a2_threshold;
}report_config_eutra_trigger_type_event_eventid_event_a2_t;

typedef struct
{
  S8    a3_offset;          /*^ M, 0, B, -30, 30 ^*/
  U8    report_on_leave;    /*^ M, 0, H, 0, 1 ^*/
}report_config_eutra_trigger_type_event_eventid_event_a3_t;

typedef struct
{
  threshold_eutra_t a4_threshold;
}report_config_eutra_trigger_type_event_eventid_event_a4_t;

typedef struct
{
  threshold_eutra_t a5_threshold1;
  threshold_eutra_t a5_threshold2;
}report_config_eutra_trigger_type_event_eventid_event_a5_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A1_PRESENT       0x01
#define REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A2_PRESENT       0x02
#define REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A3_PRESENT       0x04
#define REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A4_PRESENT       0x08
#define REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A5_PRESENT       0x10

  report_config_eutra_trigger_type_event_eventid_event_a1_t event_a1;
/*^ O, REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A1_PRESENT ^*/

  report_config_eutra_trigger_type_event_eventid_event_a2_t event_a2;
/*^ O, REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A2_PRESENT ^*/

  report_config_eutra_trigger_type_event_eventid_event_a3_t event_a3;
/*^ O, REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A3_PRESENT ^*/

  report_config_eutra_trigger_type_event_eventid_event_a4_t event_a4;
/*^ O, REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A4_PRESENT ^*/

  report_config_eutra_trigger_type_event_eventid_event_a5_t event_a5;
/*^ O, REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A5_PRESENT ^*/

}report_config_eutra_trigger_type_event_eventid_t;

typedef enum
{
    RRM_RRC_TIME_TO_TRIGGER_MS_0,
    RRM_RRC_TIME_TO_TRIGGER_MS_40,
    RRM_RRC_TIME_TO_TRIGGER_MS_64,
    RRM_RRC_TIME_TO_TRIGGER_MS_80,
    RRM_RRC_TIME_TO_TRIGGER_MS_100,
    RRM_RRC_TIME_TO_TRIGGER_MS_128,
    RRM_RRC_TIME_TO_TRIGGER_MS_160,
    RRM_RRC_TIME_TO_TRIGGER_MS_256,
    RRM_RRC_TIME_TO_TRIGGER_MS_320,
    RRM_RRC_TIME_TO_TRIGGER_MS_480,
    RRM_RRC_TIME_TO_TRIGGER_MS_512,
    RRM_RRC_TIME_TO_TRIGGER_MS_640,
    RRM_RRC_TIME_TO_TRIGGER_MS_1024,
    RRM_RRC_TIME_TO_TRIGGER_MS_1280,
    RRM_RRC_TIME_TO_TRIGGER_MS_2560,
    RRM_RRC_TIME_TO_TRIGGER_MS_5120
}rrm_time_to_trigger_et;

typedef struct
{
  report_config_eutra_trigger_type_event_eventid_t  event_id;
  U8                                                hysteresis;
/*^ M, 0, H, 0, 30 ^*/

  U8                                                time_to_trigger;
/*^ M, 0, H, 0, 15 ^*/ /* rrm_time_to_trigger_et */

}report_config_eutra_trigger_type_event_t;

typedef enum
{
    RRM_RRC_REPORT_STRONGEST_CELLS,
    RRM_RRC_REPORT_CGI
}rrm_trigger_type_periodical_purpose_et;

typedef struct
{
  U8    purpose;
/*^ M, 0, H, 0, 1 ^*/ /* rrm_trigger_type_periodical_purpose_et */

}report_config_eutra_trigger_type_periodical_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_PRESENT      0x01
#define REPORT_CONFIG_EUTRA_TRIGGER_TYPE_PERIODICAL_PRESENT 0x02

  report_config_eutra_trigger_type_event_t      event;
/*^ O, REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_PRESENT ^*/

  report_config_eutra_trigger_type_periodical_t periodical;
/*^ O, REPORT_CONFIG_EUTRA_TRIGGER_TYPE_PERIODICAL_PRESENT ^*/

}report_config_eutra_trigger_type_t;

typedef enum
{
  RRM_RRC_RSRP,
  RRM_RRC_RSRQ
}rrm_trigger_quantity_et;

typedef enum
{
  RRM_RRC_SAME_AS_TRIGGER_QUANTITY,
  RRM_RRC_BOTH
}rrm_report_quantity_et;


/* ENDC_MEAS_CHANGES_START */
typedef struct
{
    U8 ss_rsrp;   /* rrc_bool_et */
    /*^ M, 0, H, 0, 1 ^*/
    U8 ss_rsrq;   /* rrc_bool_et */
    /*^ M, 0, H, 0, 1 ^*/
    U8 ss_sinr;   /* rrc_bool_et */
    /*^ M, 0, H, 0, 1 ^*/
}report_quantity_cell_nr_r15_t;

/* ENDC_MEAS_CHANGES_END */

typedef enum
{
    RRM_RRC_REPORT_INTERVAL_MS_120,
    RRM_RRC_REPORT_INTERVAL_MS_240,
    RRM_RRC_REPORT_INTERVAL_MS_480,
    RRM_RRC_REPORT_INTERVAL_MS_640,
    RRM_RRC_REPORT_INTERVAL_MS_1024,
    RRM_RRC_REPORT_INTERVAL_MS_2048,
    RRM_RRC_REPORT_INTERVAL_MS_5120,
    RRM_RRC_REPORT_INTERVAL_MS_10240,
    RRM_RRC_REPORT_INTERVAL_MIN_1,
    RRM_RRC_REPORT_INTERVAL_MIN_6,
    RRM_RRC_REPORT_INTERVAL_MIN_12,
    RRM_RRC_REPORT_INTERVAL_MIN_30,
    RRM_RRC_REPORT_INTERVAL_MIN_60
}rrm_report_interval_et;

typedef enum
{
  RRM_RRC_REPORT_AMOUNT_1,
  RRM_RRC_REPORT_AMOUNT_2,
  RRM_RRC_REPORT_AMOUNT_4,
  RRM_RRC_REPORT_AMOUNT_8,
  RRM_RRC_REPORT_AMOUNT_16,
  RRM_RRC_REPORT_AMOUNT_32,
  RRM_RRC_REPORT_AMOUNT_64,
  RRM_RRC_REPORT_AMOUNT_INFINITY
} rrm_report_amount_et;

/*SPR8897fix_start*/
typedef enum
{
  RRM_RRC_SETUP_R9
}rrm_request_for_ho_r9_et;
/*SPR8897fix_stop*/

typedef struct
{
  report_config_eutra_trigger_type_t    trigger_type;
  U8                                    trigger_quantity;
/*^ M, 0, H, 0, 1 ^*/  /* rrm_trigger_quantity_et */

  U8                                    report_quantity;
/*^ M, 0, H, 0, 1 ^*/  /* rrm_report_quantity_et */

  U8                                    max_report_cells;
/*^ M, 0, B, 1, 8 ^*/

  U8                                    report_interval;
/*^ M, 0, H, 0, 15 ^*/  /* rrm_report_interval_et */

  U8                                    report_amount;
/*^ M, 0, H, 0, 7 ^*/  /* rrm_report_amount_et */

}report_config_eutra_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define THRESHOLD_UTRA_RSCP_PRESENT             0x01
#define THRESHOLD_UTRA_ECN0_PRESENT             0x02

  S8    threshold_rscp;     /*^ O, THRESHOLD_UTRA_RSCP_PRESENT, B, -5, 91 ^*/
  U8    threshold_ecn0;     /*^ O, THRESHOLD_UTRA_ECN0_PRESENT, H, 0, 49 ^*/
}threshold_utra_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_UTRA_PRESENT     0x01
#define REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_GERAN_PRESENT    0x02
#define REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_CDMA2000_PRESENT 0x04

  threshold_utra_t  b1_threshold_utra;
/*^ O,REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_UTRA_PRESENT ^*/

  U8    b1_threshold_geran;
/*^ O,REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_GERAN_PRESENT,H,0,63 ^*/

  U8    b1_threshold_cdma2000;
/*^ O,REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_CDMA2000_PRESENT,H,0,63 ^*/

}report_config_interrat_trigger_type_event_eventid_event_b1_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_UTRA_PRESENT     0x01
#define REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_GERAN_PRESENT    0x02
#define REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_CDMA2000_PRESENT 0x04

  threshold_eutra_t b2_threshold_eutra;
  threshold_utra_t  b2_threshold_utra;
/*^ O,REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_UTRA_PRESENT ^*/

  U8    b2_threshold_geran;
/*^ O,REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_GERAN_PRESENT,H,0,63 ^*/

  U8    b2_threshold_cdma2000;
/*^ O,REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_CDMA2000_PRESENT,H,0,63 ^*/

}report_config_interrat_trigger_type_event_eventid_event_b2_t;


typedef struct
{
  U8   w1_threshold_r13; /*^ M, 0, H, 0, 141 ^*/
  /* WLAN-RSSI-Range-r13 */

}report_config_interrat_trigger_type_event_eventid_event_w1_t;

typedef struct
{
  U8    w2_threshold1_r13;  /*^ M, 0, H, 0, 141 ^*/
  /* WLAN-RSSI-Range-r13 */

  U8    w2_threshold2_r13;  /*^ M, 0, H, 0, 141 ^*/
  /* WLAN-RSSI-Range-r13 */

}report_config_interrat_trigger_type_event_eventid_event_w2_t;

typedef struct
{
  U8    w3_threshold_r13;  /*^ M, 0, H, 0, 141 ^*/
  /* WLAN-RSSI-Range-r13 */

}report_config_interrat_trigger_type_event_eventid_event_w3_t;

/* ENDC_MEAS_CHANGES_START */
typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define THRESHOLD_NR_R15_RSRP_PRESENT            0x01
#define THRESHOLD_NR_R15_RSRQ_PRESENT            0x02
#define THRESHOLD_NR_R15_SINR_PRESENT            0x04

  U8    threshold_rsrp;     /*^ O, THRESHOLD_NR_R15_RSRP_PRESENT , H, 0, 127 ^*/
  U8    threshold_rsrq;     /*^ O, THRESHOLD_NR_R15_RSRQ_PRESENT , H, 0, 127 ^*/
  U8    threshold_sinr;     /*^ O, THRESHOLD_NR_R15_SINR_PRESENT , H, 0, 127 ^*/
}threshold_nr_r15_t;


typedef struct
{
  threshold_nr_r15_t    b1_threshold_nr_r15;
  /*^ M, 0, N, 0, 0 ^*/
  U8    report_on_leave_r15;   /* rrc_bool_et */
  /*^ M, 0, H, 0, 1 ^*/
}report_config_interrat_trigger_type_event_eventid_event_b1_nr_t;

typedef struct
{
  threshold_eutra_t   b2_threshold1_r15;
  /*^ M, 0, N, 0, 0 ^*/
  threshold_nr_r15_t  b2_threshold2_nr_r15;
  /*^ M, 0, N, 0, 0 ^*/
  U8                  report_on_leave_r15;/* rrc_bool_et */
  /*^ M, 0, H, 0, 1 ^*/
}report_config_interrat_trigger_type_event_eventid_event_b2_nr_t;
/* ENDC_MEAS_CHANGES_START */


typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_PRESENT        0x01
#define REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_PRESENT        0x02
#define REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_W1_PRESENT        0x04
#define REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_W2_PRESENT        0x08
#define REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_W3_PRESENT        0x10
/* ENDC_MEAS_CHANGES_START */
#define REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_NR_PRESENT     0x20
#define REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_NR_PRESENT     0x40
/* ENDC_MEAS_CHANGES_END */

  report_config_interrat_trigger_type_event_eventid_event_b1_t  event_b1;
/*^ O, REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_PRESENT ^*/

  report_config_interrat_trigger_type_event_eventid_event_b2_t  event_b2;
/*^ O, REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_PRESENT ^*/


  report_config_interrat_trigger_type_event_eventid_event_w1_t  event_w1;
/*^ O, REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_W1_PRESENT ^*/

  report_config_interrat_trigger_type_event_eventid_event_w2_t  event_w2;
/*^ O, REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_W2_PRESENT ^*/

  report_config_interrat_trigger_type_event_eventid_event_w3_t  event_w3;
/*^ O, REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_W3_PRESENT ^*/
  /* ENDC_MEAS_CHANGES_START */
  report_config_interrat_trigger_type_event_eventid_event_b1_nr_t  event_b1_nr;
/*^ O, REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_NR_PRESENT ^*/
  report_config_interrat_trigger_type_event_eventid_event_b2_nr_t  event_b2_nr;
/*^ O, REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_NR_PRESENT ^*/
  /* ENDC_MEAS_CHANGES_END */
}report_config_interrat_trigger_type_event_eventid_t;

typedef struct
{
  report_config_interrat_trigger_type_event_eventid_t   event_id;
  U8                                                hysteresis;
/*^ M, 0, H, 0, 30 ^*/

  U8                                                time_to_trigger;
/*^ M, 0, H, 0, 15 ^*/ /* rrm_time_to_trigger_et */

}report_config_interrat_trigger_type_event_t;

typedef struct
{
  U8    purpose;            /*^ M, 0, H, 0, 2 ^*/
  /* ENUMERATED {reportStrongestCells, reportStrongestCellsForSON, reportCGI}*/
}report_config_interrat_trigger_type_periodical_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_PRESENT       0x01
#define REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_PERIODICAL_PRESENT  0x02

  report_config_interrat_trigger_type_event_t       event;
/*^ O, REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_PRESENT ^*/

  report_config_interrat_trigger_type_periodical_t  periodical;
/*^ O, REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_PERIODICAL_PRESENT ^*/

}report_config_interrat_trigger_type_t;


typedef enum
{
  RRM_REQUEST_FOR_WLAN_BAND 
}rrm_request_for_band_et;

typedef enum
{
  RRM_REQUEST_FOR_WLAN_CARRIER_INFO
}rrm_request_for_carrier_info_et;

typedef enum
{
  RRM_REQUEST_FOR_WLAN_AVAILABLE_ADMISSION_CAPACITY
}rrm_request_for_available_admission_capacity_et;

typedef enum
{
  RRM_REQUEST_FOR_WLAN_BACKHAUL_DL_BANDWIDTH
}rrm_request_for_backhaul_dl_bandwidth_et;

typedef enum
{
  RRM_REQUEST_FOR_WLAN_BACKHAUL_UL_BANDWIDTH
}rrm_request_for_backhaul_ul_bandwidth_et;

typedef enum
{
  RRM_REQUEST_FOR_WLAN_CHANNEL_UTILIZATION
}rrm_request_for_channel_utilization_et;

typedef enum
{
  RRM_REQUEST_FOR_WLAN_STATION_COUNT
}rrm_request_for_station_count_et;

typedef struct 
{
  rrc_bitmask_t     bitmask;    /*^ BITMASK ^*/

#define BAND_REQUEST_WLAN_R13_PRESENT                        0x01
#define CARRIER_INFO_REQUEST_WLAN_R13_PRESENT                0x02
#define AVAILABLE_ADMISSION_CAPACITY_REQUEST_WLAN_PRESENT    0x04
#define BACKHAUL_DL_BANDWIDTH_REQUEST_WLAN_R13_PRESENT       0x08
#define BACKHAUL_UL_BANDWIDTH_REQUEST_WLAN_R13_PRESENT       0x10
#define CHANNEL_UTILIZATION_REQUEST_WLAN_R13_PRESENT         0x12
#define STATION_COUNT_REQUEST_WLAN_R13_PRESENT               0x14

  U8  band_request; 
/*^ M, 0, H, 0, 0 ^*/   /* rrm_request_for_band_et */

  U8  carrier_info_request;
/*^ M, 0, H, 0, 0 ^*/   /* rrm_request_for_carrier_info_et */

  U8  available_admission_capacity_request;
/*^ M, 0, H, 0, 0 ^*/   /* rrm_request_for_available_admission_capacity_et */

  U8  backhaul_dl_bandwidth_request;
/*^ M, 0, H, 0, 0 ^*/   /* rrm_request_for_backhaul_dl_bandwidth_et */

  U8  backhaul_ul_bandwidth_request;
/*^ M, 0, H, 0, 0 ^*/   /* rrm_request_for_backhaul_ul_bandwidth_et */

  U8  channel_utilization_request;
/*^ M, 0, H, 0, 0 ^*/   /* rrm_request_for_channel_utilization_et */

  U8  station_count_request;
/*^ M, 0, H, 0, 0 ^*/   /* rrm_request_for_station_count_et */

}reportQuantityWLAN_r13_t;


typedef struct
{

  report_config_interrat_trigger_type_t trigger_type;
  U8                                    max_report_cells;
/*^ M, 0, B, 1, 8 ^*/

  U8                                    report_interval;
/*^ M, 0, H, 0, 15 ^*/  /* rrm_report_interval_et */

  U8                                    report_amount;
/*^ M, 0, H, 0, 7 ^*/  /* rrm_report_amount_et */
  
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/
#define REPORT_QUANTITY_UTRA_FDD_R10_PRESENT     0x01
  /*SPR8897fix_start*/ 
#define SI_REQUEST_FOR_HO_R9_INTERRAT_PRESENT    0x02
  /*SPR8897fix_stop*/
  U8    report_quantity_UTRA_FDD_r10;
  /*^ O, REPORT_QUANTITY_UTRA_FDD_R10_PRESENT, H, 0, 0 ^*/
 /*SPR8897fix_start*/ 
  U8                                   si_request_for_ho_r9; 
/*^ O, SI_REQUEST_FOR_HO_R9_INTERRAT_PRESENT, H, 0, 0 ^*/ /* rrm_request_for_ho_r9_et */
  /*SPR8897fix_stop*/


#define REPORT_QUANTITY_WLAN_R13_PRESENT         0x04

  reportQuantityWLAN_r13_t             report_quantity_wlan_r13;
/*^ O, REPORT_QUANTITY_WLAN_R13_PRESENT ^*/

/* ENDC_MEAS_CHANGES_START */

#define REPORT_QUANTITY_CELL_NR_R15_PRESENT         0x08
#define MAX_REPORT_RS_INDEX_PRESENT                 0x10
#define REPORT_QUANTITY_RS_INDEX_NR_PRESENT         0x20  
#define REPORT_RS_INDEX_RESULTS_NR_PRESENT          0x40
#define REPORT_SFTD_MEAS_PRESENT                    0x80

  report_quantity_cell_nr_r15_t        report_quantity_cell_nr_r15;
  /*^ O, REPORT_QUANTITY_CELL_NR_R15_PRESENT , N, 0, 0 ^*/
  U8                                   max_report_rs_index_r15;
  /*^ O, MAX_REPORT_RS_INDEX_PRESENT, H, 0, 32 ^*/
  report_quantity_cell_nr_r15_t        report_quantity_rs_index_nr_r15;
  /*^ O, REPORT_QUANTITY_RS_INDEX_NR_PRESENT , N, 0, 0 ^*/
  U8     report_rs_index_results_nr;
  /*^ O, REPORT_RS_INDEX_RESULTS_NR_PRESENT, H, 0, 1 ^*/  /* rrc_bool_et */
  U8     report_sftd_meas_r15;
  /*^ O, REPORT_SFTD_MEAS_PRESENT, H, 0, 1 ^*/  /* rrm_report_sftd_meas_r15_et */
/* ENDC_MEAS_CHANGES_END */
}report_config_interrat_t;

typedef struct 
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define REPORT_CFG_EUTRA_UE_RXTX_TIME_DIFF_PRESENT   0x01
/*SPR8897fix_start*/
#define SI_REQUEST_FOR_HO_R9_EUTRA_PRESENT           0x02
/*SPR8897fix_stop*/
#define REPORT_CFG_EUTRA_REPORT_ADD_NEIGH_MEAS_PRESENT   0x04

  U8                                    ue_rxtx_time_diff;
/*^ O, REPORT_CFG_EUTRA_UE_RXTX_TIME_DIFF_PRESENT, H, 0, 0 ^*/
  /*SPR8897fix_start*/
  U8                                    si_request_for_ho_r9; 
/*^ O, SI_REQUEST_FOR_HO_R9_EUTRA_PRESENT, H, 0, 0 ^*/ /* rrm_request_for_ho_r9_et */
  /*SPR8897fix_stop*/
  U8                                    report_add_neigh_meas_r10;
/*^ O, REPORT_CFG_EUTRA_REPORT_ADD_NEIGH_MEAS_PRESENT, H, 0, 0 ^*/
}report_config_eutra_ext_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define REPORT_CONFIG_EUTRA_PRESENT     0x01
#define REPORT_CONFIG_INTERRAT_PRESENT  0x02
#define REPORT_CONFIG_EUTRA_EXT_PRESENT 0x04

  report_config_eutra_t     report_config_eutra;
/*^ O, REPORT_CONFIG_EUTRA_PRESENT ^*/

  report_config_interrat_t  report_config_interrat;
/*^ O, REPORT_CONFIG_INTERRAT_PRESENT ^*/

  report_config_eutra_ext_t  report_config_eutra_ext;
/*^ O, REPORT_CONFIG_EUTRA_EXT_PRESENT  ^*/

}report_config_to_add_mod_report_config_t;

typedef struct
{
  U8                                            report_config_id;
/*^ M, 0, B, 1, 32 ^*/

  report_config_to_add_mod_report_config_t      report_config;
}report_config_to_add_mod_t;

typedef struct
{
/*SPR_17664_start*/
  U8                            count;          /*^ M, 0, H, 0, MAX_REPORT_CONFIG_ID ^*/
/*SPR_17664_end*/
  report_config_to_add_mod_t    report_config_to_add_mod[MAX_REPORT_CONFIG_ID];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}report_config_to_add_mod_list_t;

typedef struct
{
/*SPR_17664_start*/
  U8    count;                  /*^ M, 0, H, 0, MAX_MEAS_ID ^*/
/*SPR_17664_end*/
  U8    meas_id[MAX_MEAS_ID]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}meas_id_to_remove_list_t;

typedef struct
{
  U8    meas_id;            /*^ M, 0, B, 1, 32 ^*/
  U8    meas_object_id;     /*^ M, 0, B, 1, 32 ^*/
  U8    report_config_id;   /*^ M, 0, B, 1, 32 ^*/
}meas_id_to_add_mod_t;

typedef struct
{
/*SPR_17664_start*/
  U8                    count;  /*^ M, 0, H, 0, MAX_MEAS_ID ^*/
/*SPR_17664_end*/
  meas_id_to_add_mod_t  meas_id_to_add_mod[MAX_MEAS_ID];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}meas_id_to_add_mod_list_t;

typedef struct
{
  U8    filter_coefficient_rsrp;
/*^ M, 0, H, 0, 14 ^*/  /* rrc_filter_coefficient_et */

  U8    filter_coefficient_rsrq;
/*^ M, 0, H, 0, 14 ^*/  /* rrc_filter_coefficient_et */

}quantity_config_eutra_t;

typedef enum
{
  RRM_RRC_CPICH_RSCP,
  RRM_RRC_CPICH_ECN0
}quantity_config_utra_meas_fdd_et;

typedef enum
{
  RRM_RRC_PCCPCH_RSCP
}quantity_config_utra_meas_tdd_et;

typedef struct
{
  U8    meas_quantity_utra_fdd;
/*^ M, 0, H, 0, 1 ^*/  /* quantity_config_utra_meas_fdd_et */

  U8    meas_quantity_utra_tdd;
/*^ M, 0, H, 0, 0 ^*/  /* quantity_config_utra_meas_tdd_et */

  U8    filter_coefficient;
/*^ M, 0, H, 0, 14 ^*/  /* rrc_filter_coefficient_et */

}quantity_config_utra_t;

typedef enum
{
  RRM_RRC_RSSI
}quantity_config_utra_meas_geran_et;

typedef struct
{
  U8    meas_quantity_geran;
/*^ M, 0, H, 0, 0 ^*/  /* quantity_config_utra_meas_geran_et */

  U8    filter_coefficient;
/*^ M, 0, H, 0, 14 ^*/  /* rrc_filter_coefficient_et */

}quantity_config_geran_t;

typedef enum
{
  RRM_RRC_PILOT_STRENGTH,
  RRM_RRC_PILOT_PN_PHASE_AND_PILOT_STRENGTH
}quantity_config_utra_meas_cdma2000_et;

typedef struct
{
  U8    meas_quantity_cdma2000;
/*^ M, 0, H, 0, 1 ^*/  /* quantity_config_utra_meas_cdma2000_et */

}quantity_config_cdma2000_t;

typedef struct
{
  
  U8    filterCoefficient2_FDD_r10;
/*^ M, 0, H, 0, 14 ^*/  /* rrc_filter_coefficient_et */

}quantity_config_UTRA_v1020_t;

typedef struct 
{
  U8   meas_quantity_wlan_r13;
/*^ M, 0, H, 0, 0 ^*/   /* quantity_config_wlan_r13_et */

  U8   filterCoefficient_r13;
/*^ M, 0, H, 0, 14 ^*/  /* rrc_filter_coefficient_et */

}quantityConfigWLAN_r13_t;

/* ENDC_MEAS_CHANGES_START */
typedef struct
{
    U8    filter_coeff_rsrp_r15;
    /*^ M, 0, H, 0, 14 ^*/  /* rrc_filter_coefficient_et */
    U8    filter_coeff_rsrq_r15;
    /*^ M, 0, H, 0, 14 ^*/  /* rrc_filter_coefficient_et */
    U8    filter_coefficient_sinr_r13;
    /*^ M, 0, H, 0, 14 ^*/  /* rrc_filter_coefficient_et */
}quantity_config_rs_nr_r15_t;

typedef struct
{
  U16     bitmask; /*^ BITMASK ^*/
#define QUANTITY_CONFIG_RS_INDEX_NR_PRESENT 0x01
  quantity_config_rs_nr_r15_t  meas_quantity_cell_nr_r15; 
  /*^ M, 0 , N, 0, 0 ^*/
  quantity_config_rs_nr_r15_t  meas_quantity_rs_index_nr_r15;
  /*^ O, QUANTITY_CONFIG_RS_INDEX_NR_PRESENT , N, 0, 0 ^*/
}quantity_config_nr_r15_t; 

typedef struct
{
  U8  count; 
  /*^ M, 0, B, 1, 2 ^*/
  quantity_config_nr_r15_t quantity_config_nr_r15[MAX_QUANT_SETS_NR];
 /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}quantity_config_nr_list_r15_t;
/* ENDC_MEAS_CHANGES_END */

typedef struct
{
  rrc_bitmask_t                 bitmask;                /*^ BITMASK ^*/

#define QUANTITY_CONFIG_EUTRA_PRESENT           0x01
#define QUANTITY_CONFIG_UTRA_PRESENT            0x02
#define QUANTITY_CONFIG_GERAN_PRESENT           0x04
#define QUANTITY_CONFIG_CDMA2000_PRESENT        0x08
#define QUANTITY_CONFIG_UTRA_v1020_PRESENT      0x10
#define QUANTITY_CONFIG_WLAN_R13_PRESENT        0x20
/* ENDC_MEAS_CHANGES_START */
#define QUANTITY_CONFIG_NR_LIST_R15_PRESENT     0x40
/* ENDC_MEAS_CHANGES_END */

  quantity_config_eutra_t       quantity_config_eutra;
/*^ O, QUANTITY_CONFIG_EUTRA_PRESENT ^*/

  quantity_config_utra_t        quantity_config_utra;
/*^ O, QUANTITY_CONFIG_UTRA_PRESENT ^*/

  quantity_config_geran_t       quantity_config_geran;
/*^ O, QUANTITY_CONFIG_GERAN_PRESENT ^*/

  quantity_config_cdma2000_t    quantity_config_cdma2000;
/*^ O, QUANTITY_CONFIG_CDMA2000_PRESENT ^*/
  
  quantity_config_UTRA_v1020_t    quantity_config_UTRA_v1020;
/*^ O, QUANTITY_CONFIG_UTRA_v1020_PRESENT ^*/

  quantityConfigWLAN_r13_t        quantity_config_wlan_r13;
/*^ O, QUANTITY_CONFIG_WLAN_R13_PRESENT ^*/

/* ENDC_MEAS_CHANGES_START */
  quantity_config_nr_list_r15_t   quantity_config_nr_list_r15;
/*^ O, QUANTITY_CONFIG_NR_LIST_R15_PRESENT ^*/
/* ENDC_MEAS_CHANGES_END */
}quantity_config_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/
#define MEAS_GAP_CONFIG_GP0_PRESENT             0x01
#define MEAS_GAP_CONFIG_GP1_PRESENT             0x02
/* ENDC_MEAS_CHANGES_START */
#define MEAS_GAP_CONFIG_GP4_PRESENT             0x04
#define MEAS_GAP_CONFIG_GP5_PRESENT             0x08
#define MEAS_GAP_CONFIG_GP6_PRESENT             0x10
#define MEAS_GAP_CONFIG_GP7_PRESENT             0x20
#define MEAS_GAP_CONFIG_GP8_PRESENT             0x40
#define MEAS_GAP_CONFIG_GP9_PRESENT             0x80
#define MEAS_GAP_CONFIG_GP10_PRESENT            0x100
#define MEAS_GAP_CONFIG_GP11_PRESENT            0x200
/* ENDC_MEAS_CHANGES_END */
  U8    gp0;           /*^ O, MEAS_GAP_CONFIG_GP0_PRESENT, H, 0, 39 ^*/
  U8    gp1;           /*^ O, MEAS_GAP_CONFIG_GP1_PRESENT, H, 0, 79 ^*/
/* ENDC_MEAS_CHANGES_START */
  U8    gp4_r15;       /*^ O, MEAS_GAP_CONFIG_GP4_PRESENT, H, 0, 19 ^*/
  U8    gp5_r15;       /*^ O, MEAS_GAP_CONFIG_GP5_PRESENT, H, 0, 159 ^*/
  U8    gp6_r15;       /*^ O, MEAS_GAP_CONFIG_GP6_PRESENT, H, 0, 19 ^*/
  U8    gp7_r15;       /*^ O, MEAS_GAP_CONFIG_GP7_PRESENT, H, 0, 39 ^*/
  U8    gp8_r15;       /*^ O, MEAS_GAP_CONFIG_GP8_PRESENT, H, 0, 79 ^*/
  U8    gp9_r15;       /*^ O, MEAS_GAP_CONFIG_GP9_PRESENT, H, 0, 159 ^*/        
  U8    gp10_r15;      /*^ O, MEAS_GAP_CONFIG_GP10_PRESENT, H, 0, 19 ^*/
  U8    gp11_r15;      /*^ O, MEAS_GAP_CONFIG_GP11_PRESENT, H, 0, 159 ^*/
/* ENDC_MEAS_CHANGES_END */
} meas_gap_setup_config_info_t; 

typedef struct
{
    rrc_bitmask_t      bitmask;
 /*^ BITMASK ^*/
#define MEAS_GAP_CONFIG_SETUP_CONFIG_INFO_PRESENT  0x01

    U8                 meas_gap_config_type;
/*^ M, 0, H, 0 , 1 ^*/ /* 0 - for release, 1 - for setup */
    meas_gap_setup_config_info_t    setup_config_info;
/*^ O, MEAS_GAP_CONFIG_SETUP_CONFIG_INFO_PRESENT, N, 0, 0 ^*/
}meas_gap_config_t;

typedef struct
{
  U8    count;  /*^ M, 0, B, 1, 2 ^*/
  U8    pre_registration_zone_id_hrpd[2];  /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}secondary_pre_registration_zone_id_list_hrpd_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define PRE_REGISTRATION_ZONE_ID_PRESENT                0x01
#define PRE_REGISTRATION_SECONDARY_ZONE_ID_LIST_PRESENT 0x02

  U8                                                pre_registration_allowed;
/*^ M, 0, H, 0, 1 ^*/

  U8                                                pre_registration_zone_id;
/*^ O, PRE_REGISTRATION_ZONE_ID_PRESENT, N, 0, 0 ^*/

  secondary_pre_registration_zone_id_list_hrpd_t
      secondary_pre_registration_zone_id_list;
/*^ O, PRE_REGISTRATION_SECONDARY_ZONE_ID_LIST_PRESENT ^*/

}pre_registration_info_hrpd_t;

typedef enum
{
  RRM_RRC_EV_S30,
  RRM_RRC_EV_S60,
  RRM_RRC_EV_S120,
  RRM_RRC_EV_S180,
  RRM_RRC_EV_S240
}mobility_state_parameters_t_evaluation_et;

typedef enum
{
  RRM_RRC_HN_S30,
  RRM_RRC_HN_S60,
  RRM_RRC_HN_S120,
  RRM_RRC_HN_S180,
  RRM_RRC_HN_S240
}mobility_state_parameters_t_hyst_normal_et;

typedef struct
{
  U8    t_evaluation;
/*^ M, 0, H, 0, 7 ^*/  /* mobility_state_parameters_t_evaluation_et */

  U8    t_hyst_normal;
/*^ M, 0, H, 0, 7 ^*/  /* mobility_state_parameters_t_hyst_normal_et */

  U8    m_cell_charge_medium;   /*^ M, 0, B, 1, 16 ^*/
  U8    m_cell_charge_high;     /*^ M, 0, B, 1, 16 ^*/
}mobility_state_parameters_t;

typedef struct
{
  mobility_state_parameters_t   mobility_state_parameters;
  speed_state_scale_factors_t   time_to_trigger_sf;
}meas_config_speed_state_pars_setup_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define MEAS_CONFIG_SPEED_STATE_SETUP_PRESENT     0x01

  meas_config_speed_state_pars_setup_t  setup;
/*^ O, MEAS_CONFIG_SPEED_STATE_SETUP_PRESENT ^*/

}meas_config_speed_state_pars_t;

typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define UE_ADM_MEAS_OBJECT_TO_REMOVE_LIST_PRESENT           0x01
#define UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT          0x02
#define UE_ADM_MEAS_REPORT_CONFIG_TO_REMOVE_LIST_PRESENT    0x04
#define UE_ADM_MEAS_REPORT_CONFIG_TO_ADD_MOD_LIST_PRESENT   0x08
#define UE_ADM_MEAS_ID_TO_REMOVE_LIST_PRESENT               0x10
#define UE_ADM_MEAS_ID_TO_ADD_MOD_LIST_PRESENT              0x20
#define UE_ADM_MEAS_QUANTITY_CONFIG_PRESENT                 0x40
#define UE_ADM_MEAS_GAP_CONFIG_PRESENT                      0x80
#define UE_ADM_MEAS_S_MEASURE_PRESENT                       0x100
#define UE_ADM_MEAS_PRE_REGISTRATION_INFO_PRESENT           0x200
#define UE_ADM_MEAS_SPEED_STATE_PARS_PRESENT                0x400
#define UE_ADM_FR1_GAP_R15_PRESENT                          0x800
#define UE_ADM_MGTA_R15_PRESENT                             0x1000

  meas_object_to_remove_list_t      meas_object_to_remove_list;
/*^ O, UE_ADM_MEAS_OBJECT_TO_REMOVE_LIST_PRESENT ^*/

  meas_object_to_add_mod_list_t     meas_object_to_add_mod_list;
/*^ O, UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT ^*/


  report_config_to_remove_list_t    report_config_to_remove_list;
/*^ O, UE_ADM_MEAS_REPORT_CONFIG_TO_REMOVE_LIST_PRESENT ^*/

  report_config_to_add_mod_list_t   report_config_to_add_mod_list;
/*^ O, UE_ADM_MEAS_REPORT_CONFIG_TO_ADD_MOD_LIST_PRESENT ^*/


  meas_id_to_remove_list_t          meas_id_to_remove_list;
/*^ O, UE_ADM_MEAS_ID_TO_REMOVE_LIST_PRESENT ^*/

  meas_id_to_add_mod_list_t         meas_id_to_add_mod_list;
/*^ O, UE_ADM_MEAS_ID_TO_ADD_MOD_LIST_PRESENT ^*/


  quantity_config_t                 quantity_config;
/*^ O, UE_ADM_MEAS_QUANTITY_CONFIG_PRESENT ^*/

  meas_gap_config_t                 meas_gap_config;
/*^ O, UE_ADM_MEAS_GAP_CONFIG_PRESENT ^*/

  U8                                s_measure;
/*^ O, UE_ADM_MEAS_S_MEASURE_PRESENT, H, 0, 97 ^*/


  pre_registration_info_hrpd_t      pre_registration_info_hrpd;
/*^ O, UE_ADM_MEAS_PRE_REGISTRATION_INFO_PRESENT ^*/

  meas_config_speed_state_pars_t    meas_config_speed_state_pars;
/*^ O, UE_ADM_MEAS_SPEED_STATE_PARS_PRESENT ^*/

  U8                                fr1_gap_r15;
  /*^ O, UE_ADM_FR1_GAP_R15_PRESENT, H, 0, 1 ^*/

  U8                                mgta_r15;
  /*^ O, UE_ADM_MGTA_R15_PRESENT, H, 0, 1 ^*/
} rrm_meas_config_t;

/* Measurment Re-config START */
typedef enum
{
    RRM_RRC_MC_VALIDATE_FAILURE,
    RRM_RRC_MC_VALIDATE_SUCCESS
} rrm_meas_config_validity_et;

/*current Meas-config*/
typedef struct
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define UE_ADM_CURR_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT          0x01
#define UE_ADM_CURR_MEAS_REPORT_CONFIG_TO_ADD_MOD_LIST_PRESENT   0x02
#define UE_ADM_CURR_MEAS_ID_TO_ADD_MOD_LIST_PRESENT              0x04
#define UE_ADM_CURR_MEAS_QUANTITY_CONFIG_PRESENT                 0x08
#define UE_ADM_CURR_MEAS_GAP_CONFIG_PRESENT                      0x10
#define UE_ADM_CURR_MEAS_S_MEASURE_PRESENT                       0x20
#define UE_ADM_CURR_MEAS_PRE_REGISTRATION_INFO_PRESENT           0x40
#define UE_ADM_CURR_MEAS_SPEED_STATE_PARS_PRESENT                0x80

  U8                                is_meas_config_valid;
  /*^ M, 0, H, 0, 1 ^*/ /*rrc_bool_et*/
  meas_object_to_add_mod_list_t     curr_meas_object_to_add_mod_list;

/*^ O, UE_ADM_CURR_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT ^*/

  report_config_to_add_mod_list_t   curr_report_config_to_add_mod_list;
/*^ O, UE_ADM_CURR_MEAS_REPORT_CONFIG_TO_ADD_MOD_LIST_PRESENT ^*/

  meas_id_to_add_mod_list_t         curr_meas_id_to_add_mod_list;
/*^ O, UE_ADM_CURR_MEAS_ID_TO_ADD_MOD_LIST_PRESENT ^*/


  quantity_config_t                 curr_quantity_config;
/*^ O, UE_ADM_CURR_MEAS_QUANTITY_CONFIG_PRESENT ^*/

  meas_gap_config_t                 curr_meas_gap_config;
/*^ O, UE_ADM_CURR_MEAS_GAP_CONFIG_PRESENT ^*/

  U8                                curr_s_measure;
/*^ O, UE_ADM_CURR_MEAS_S_MEASURE_PRESENT, H, 0, 97 ^*/

  pre_registration_info_hrpd_t      curr_pre_registration_info_hrpd;
/*^ O, UE_ADM_CURR_MEAS_PRE_REGISTRATION_INFO_PRESENT ^*/

  meas_config_speed_state_pars_t    curr_meas_config_speed_state_pars;
/*^ O, UE_ADM_CURR_MEAS_SPEED_STATE_PARS_PRESENT ^*/

} rrm_curr_meas_config_t;
/* Measurment Re-config END */


/******************************************************************************
 * MAC LC Parameters - See DL_LC_CREATE_REQ and
 *                     UL_LC_CREATE_REQ TLVs description in MAC API document.
 ******************************************************************************/
typedef struct
{
    rrc_bitmask_t       bitmask;       /*^ BITMASK ^*/
#define RRM_MAC_LC_CONFIG_UL_LC_G_ID_PRESENT   0x01
#define RRM_MAC_LC_CONFIG_DL_PRIORITY_PRESENT  0x02
#define RRM_MAC_CA_LC_BANDWIDTH_DIST_INFO_PRESENT 0x04

    U8  ul_lc_g_id;
/*^ O, RRM_MAC_LC_CONFIG_UL_LC_G_ID_PRESENT, H, 0, 3 ^*/

    U8  dl_lch_priority;
/*^ O, RRM_MAC_LC_CONFIG_DL_PRIORITY_PRESENT, H, 1, 16 ^*/

    rrc_mac_ca_lc_bandwidth_dist_info_t     ca_lc_bandwidth_dist_info;
    /*^ O, RRM_MAC_CA_LC_BANDWIDTH_DIST_INFO_PRESENT ^*/

} rrm_mac_lc_config_t;

/******************************************************************************
            RRC_RRM_UE_ADMISSION_RESP
******************************************************************************/

/* TS 36.331 - 6.3.2 RadioResourceConfigDedicated IE SRB-ToAddMod IE.
 *             The rlc-Config IE is optional and defaultValues should be
 *             supported for it */
typedef struct
{
    rrc_bitmask_t         bitmask;   /*^ BITMASK ^*/
#define RRM_SRB_AM_CONFIG_EXPLICIT_PRESENT              0x01

    rrc_am_config_t       am_config_explicit;
/*^ O, RRM_SRB_AM_CONFIG_EXPLICIT_PRESENT ^*/

} rrm_srb_am_config_t;

/* TS 36.331 - 6.3.2 RadioResourceConfigDedicated IE SRB-ToAddMod IE.
 *             The logicalChannelConfig IE is optional and defaultValues
 *             should be supported for it  */
typedef struct
{
    rrc_bitmask_t         bitmask;   /*^ BITMASK ^*/
#define RRM_UL_SPECIFIC_PARAMETERS_EXPLICIT_PRESENT        0x01
#define RRM_UL_SPECIFIC_PARAMETERS_LC_SR_MASK_R9_PRESENT   0x02

    rrc_ul_specific_parameters_t  ul_specific_parameters_explicit;
/*^ O, RRM_UL_SPECIFIC_PARAMETERS_EXPLICIT_PRESENT ^*/

    U8     lc_sr_mask_r9;
/*^ O, RRM_UL_SPECIFIC_PARAMETERS_LC_SR_MASK_R9_PRESENT, H, 0, 0 ^*/
/* rrc_lc_sr_mask_r9_et */

}rrm_logical_channel_config_t;

/* TS 36.331 - 6.3.2 RadioResourceConfigDedicated IE SRB-ToAddMod IE.
 *             The srb-Identity IE isn't used because SRB configuration
 *             provided in different named fields for each SRB
 *             in srb_config_t structure */
typedef struct
{
    rrc_bitmask_t         bitmask;   /*^ BITMASK ^*/
#define RRM_SRB_CONFIG_AM_CONFIG_PRESENT                0x01
#define RRM_SRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT   0x02
#define RRM_SRB_CONFIG_MAC_LC_CONFIG_ENB_PRESENT        0x04

    rrm_srb_am_config_t                srb_am_config;
/*^ O, RRM_SRB_CONFIG_AM_CONFIG_PRESENT ^*/

    rrm_logical_channel_config_t logical_channel_config;
/*^ O, RRM_SRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT ^*/

    rrm_mac_lc_config_t                mac_lc_config_enb;
/*^ O, RRM_SRB_CONFIG_MAC_LC_CONFIG_ENB_PRESENT ^*/

} rrm_srb_config_t;

/* SRBs configuration - if absent the default configuration should be used */
typedef struct
{
    rrc_bitmask_t    bitmask;     /*^ BITMASK ^*/
#define RRM_SRB_INFO_SRB1_CONFIG_PRESENT         0x01
#define RRM_SRB_INFO_SRB2_CONFIG_PRESENT         0x02

    rrm_srb_config_t     srb1_config;
/*^ O, RRM_SRB_INFO_SRB1_CONFIG_PRESENT ^*/

    rrm_srb_config_t     srb2_config;
/*^ O, RRM_SRB_INFO_SRB2_CONFIG_PRESENT ^*/

} rrm_srb_info_t;

/* MAC-MainConfig Extensions - It contains MAC parameters which are not
 * defined in TS 36.331  */
typedef enum
{
  RRM_RRC_MODE_SCHEME_QPSK = 2,
  RRM_RRC_MODE_SCHEME_16_QAM = 4,
  RRM_RRC_MODE_SCHEME_64_QAM = 6
} rrm_mode_scheme_et;

typedef struct
{
  rrc_bitmask_t         bitmask;
/*^ BITMASK ^*/
/* CLPC Start */
#define RRM_MAC_MAIN_CONFIG_EXT_PC_MAX_PRESENT    0x01
/* CLPC Stop */

#define RRM_MAC_MAIN_CONFIG_EXT_USER_LOCATION_TYPE_PRESENT  0x02
/* UL_MU_MIMO Start */
#define RRM_MAC_MAIN_CONFIG_EXT_UL_MU_MIMO_INFO_PRESENT     0x04
/* UL_MU_MIMO Stop */
/* min_ue_power start */
#define RRM_MAC_MAIN_CONFIG_EXT_MIN_UE_POWER_PRESENT        0x08  
/* min_ue_power stop */
/* Transmission_mode_7_8 start*/
#define RRM_MAC_MAIN_CONFIG_EXT_PDSCH_EPRE_TO_UE_RS_RATIO_PRESENT        0x10  
	/* Transmission_mode_7_8 stop*/
	/* eICIC changes start */
#define RRM_MAC_MAN_CONFIG_EXT_DL_INTERFERENCE_INFO_PRESENT      0x20
	/* eICIC changes stop */

  U8                ue_priority;            /*^ M, 0, H, 0, 3 ^*/
  U8                dl_num_harq_process;    /*^ M, 0, B, 1, 15 ^*/
  U8                dl_i_mcs;               /*^ M, 0, H, 0, 28 ^*/
  U8                dl_max_rb;              /*^ M, 0, B, 1, 100 ^*/
  U8                max_ul_harq_tx;         /*^ M, 0, H, 0, 13 ^*/
  U8                ul_i_mcs;               /*^ M, 0, H, 0, 28 ^*/
  U8                num_of_layer;           /*^ M, 0, B, 1, 4 ^*/
  U8                code_book_index;        /*^ M, 0, H, 0, 3 ^*/
  /* CLPC Start */
  S8                pc_max;                 
  /*^ O, RRM_MAC_MAIN_CONFIG_EXT_PC_MAX_PRESENT, B, -40, 23 ^*/
  /* CLPC Stop */

  U8                user_location_type;               
  /*^ O, RRM_MAC_MAIN_CONFIG_EXT_USER_LOCATION_TYPE_PRESENT, H, 0, 1 ^*/
  /* UL_MU_MIMO Start */
  U8                ul_mu_mimo_status;               
  /*^ O, RRM_MAC_MAIN_CONFIG_EXT_UL_MU_MIMO_INFO_PRESENT, H, 0, 1 ^*/
  /* UL_MU_MIMO Stop */
/* min_ue_power start */ 
  S8                min_ue_power;
  /*^ O, RRM_MAC_MAIN_CONFIG_EXT_MIN_UE_POWER_PRESENT, B, -40, 23 ^*/
/* min_ue_power stop */ 
/* Transmission_mode_7_8 start*/
  S8                pdsch_epre_to_ue_rs_ratio;
  /*^ O, RRM_MAC_MAIN_CONFIG_EXT_PDSCH_EPRE_TO_UE_RS_RATIO_PRESENT, B, -6, 3 ^*/
	/* Transmission_mode_7_8 stop*/

	/* eICIC changes start */
	U8                                        dl_interference_info;        /* dl_interference_info_et */
	/*^ O, RRM_MAC_MAN_CONFIG_EXT_DL_INTERFERENCE_INFO_PRESENT, H, 0, 1  ^*/
	/* eICIC changes stop */
} rrm_mac_main_config_extensions_t;

/* RRM MAC Main Configuration IE */
/* TS 36.331 - 6.3.2 RadioResourceConfigDedicated IE mac-MainConfig IE.
 *             The mac-MainConfig is optional and defaultValues
 *             should be supported for it  */
typedef struct
{
    rrc_bitmask_t             bitmask;     /*^ BITMASK ^*/
#define RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT         0x01

    rrc_mac_main_config_t             mac_main_config;
/*^ O, RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT ^*/

    rrm_mac_main_config_extensions_t  mac_main_config_extensions;
} rrm_mac_config_t;

/* RRM Radio Resource Configuration for UE - this IE should be presented in
 * case of successful UE admission */
typedef struct
{
    rrc_bitmask_t             bitmask;     /*^ BITMASK ^*/
#define RRM_UE_ADM_RADIO_RESP_SRB_INFO_PRESENT                  0x01
#define RRM_UE_ADM_RADIO_RESP_MAC_CONFIG_PRESENT                0x02
#define RRM_UE_ADM_RADIO_RESP_SPS_CONFIG_PRESENT                0x04
#define RRM_UE_ADM_RADIO_RESP_PHY_CONFIG_DED_PRESENT            0x08
#define RRM_UE_ADM_RADIO_RESP_MEAS_CONFIG_PRESENT               0x10
#define RRM_UE_ADM_RADIO_RESP_RLF_TIMERS_AND_CONSTANTS_PRESENT  0x20

  rrm_srb_info_t              srb_info;
/*^ O, RRM_UE_ADM_RADIO_RESP_SRB_INFO_PRESENT ^*/

  rrm_mac_config_t            mac_config;
/*^ O, RRM_UE_ADM_RADIO_RESP_MAC_CONFIG_PRESENT ^*/

  rrc_sps_config_t            sps_config;
/*^ O, RRM_UE_ADM_RADIO_RESP_SPS_CONFIG_PRESENT ^*/

  rrc_phy_physical_config_dedicated_t   physical_config_dedicated;
/*^ O, RRM_UE_ADM_RADIO_RESP_PHY_CONFIG_DED_PRESENT ^*/

  rrm_meas_config_t           meas_config;
/*^ O, RRM_UE_ADM_RADIO_RESP_MEAS_CONFIG_PRESENT ^*/


/* cr_220 structure */
  rlf_timers_and_constants_r9_t rlf_timers_and_constants_r9;
  /*^ O, RRM_UE_ADM_RADIO_RESP_RLF_TIMERS_AND_CONSTANTS_PRESENT ^*/

} rrm_ue_adm_radio_res_config_t;
/*CR 026 changes start*/
typedef enum
{
    FAIL_CAUSE_CONGESTION = 0,
    FAIL_CAUSE_ENERGY_SAVING,
    FAIL_CAUSE_UNSPECIFIED
}rrc_ue_adm_resp_fail_cause_et;
/*CR 026 changes stop*/
/* eMTC changes start */
#define RRM_UE_ADM_RESP_API_EMTC_CONFIG_INFO_PRESENT            0x08
/* eMTC changes stop */

typedef struct
{
    rrc_bitmask_t             bitmask;      /*^ BITMASK ^*/
#define RRM_UE_ADM_RESP_API_UE_ADM_RADIO_RESP_PRESENT   0x01
#define RRM_UE_ADM_RESP_API_UE_ADM_EXTENDED_WAIT_TIME_PRESENT   0x02
/*CR 026 changes start*/
#define RRM_UE_ADM_RESP_API_FAIL_CAUSE_PRESENT          0x04    
/*CR 026 changes stop*/
    U16                         ue_index;
    U8                          wait_time;    /*^ M, 0, B, 1, 16 ^*/
    U8                          response;
    /*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

    rrm_ue_adm_radio_res_config_t   radio_res_config;
    /*^ O, RRM_UE_ADM_RESP_API_UE_ADM_RADIO_RESP_PRESENT ^*/
    
    U16                          extended_wait_time;   
    /*^ O, RRM_UE_ADM_RESP_API_UE_ADM_EXTENDED_WAIT_TIME_PRESENT, B, 1, 1800 ^*/
    /*CR 026 changes start*/
    U8                           fail_cause;  
    /*^ O, RRM_UE_ADM_RESP_API_FAIL_CAUSE_PRESENT , H, 0, 2^*/ /*rrc_ue_adm_resp_fail_cause_et */
    /*CR 026 changes stop*/

    /* eMTC changes start */
    RrcRrmAdmissionEmtcConfigInfo_t    admissionEmtcConfigInfo;
    /*^ O, RRM_UE_ADM_RESP_API_EMTC_CONFIG_INFO_PRESENT, N, 0, 0 ^*/
    /* eMTC changes stop */

} rrc_rrm_ue_admission_resp_t; /*^ API, RRC_RRM_UE_ADMISSION_RESP ^*/

/******************************************************************************
            RRC_RRM_UE_ADMISSION_CNF
******************************************************************************/

typedef struct
{
/* SPS Start */
#define RRC_RRM_UE_ADM_CNF_SPS_ERROR_CODE_PRESENT    0x01
/* SPS Stop */

    U16           ue_index;
    U8            response;  /*^ M, 0, H, 0, 2 ^*/ /* rrc_return_et */
/* SPS Start */
    rrc_bitmask_t   bitmask;                 /*^ BITMASK ^*/
    U8           error_code;
    /*^ O, RRC_RRM_UE_ADM_CNF_SPS_ERROR_CODE_PRESENT, H, 0, 0 ^*/
    /* rrc_rrm_error_code_et */
/* SPS Stop */
} rrc_rrm_ue_admission_cnf_t; /*^ API, RRC_RRM_UE_ADMISSION_CNF ^*/

/* eICIC change start */
typedef struct
{
#define MEAS_SUBFRAME_PATTERN_PCELL_R10_RELEASE_PRESENT     0x01
#define MEAS_SUBFRAME_PATTERN_PCELL_R10_SETUP_PRESENT       0x02

    rrc_bitmask_t                           bitmask;   /*^ BITMASK ^*/
    rrc_phy_meas_subframe_pattern_r10_t     meas_subframe_pattern_r10_setup;
    /*^ O, MEAS_SUBFRAME_PATTERN_PCELL_R10_SETUP_PRESENT  ^*/
}rrc_meas_subframe_pattern_pcell_r10_t;
/* eICIC change stop */


/******************************************************************************
              RRC_RRM_UE_RECONFIG_REQ
 ********************************************************************************/
typedef struct
{
    rrc_bitmask_t                          presence_bitmask;   /*^ BITMASK ^*/
#define RRM_UE_RECONFIG_PHY_CONFIG_DED_PRESENT            0x01
#define RRM_UE_RECONFIG_MAC_CONFIG_PRESENT                0x02
	/* eICIC change start */
#define RRM_UE_RECONFIG_MEAS_SUBFRAME_PATTERN_PCELL_R10_PRESENT     0x04
	/* eICIC change stop */


#define RRM_UE_RECONFIG_DRB_TO_MOD_LIST_PRESENT       0x08
#define RRM_UE_RECONFIG_MEAS_CONFIG_PRESENT           0x10


    rrc_phy_physical_config_dedicated_t    rrc_rrm_ue_reconfig_physical_config_dedicated;
/*^ O, RRM_UE_RECONFIG_PHY_CONFIG_DED_PRESENT ^*/

    /* MAC configuration */
    rrm_mac_config_t                       mac_config;
	/*^ O, RRM_UE_RECONFIG_MAC_CONFIG_PRESENT ^*/
	/* eICIC change start */
	rrc_meas_subframe_pattern_pcell_r10_t  rrc_meas_subframe_pattern_pcell_r10;
	/*^ O, RRM_UE_RECONFIG_MEAS_SUBFRAME_PATTERN_PCELL_R10_PRESENT ^*/
	/* eICIC change stop */
}radio_resource_reconfig_t;

typedef struct
{
    rrc_bitmask_t              bitmask;     /*^ BITMASK ^*/
#define RRM_SCELL_TO_RELEASE_CELL_INDEX_PRESENT     0x01   

    rrc_cell_index_t   cell_index;
/*^ O, RRM_SCELL_TO_RELEASE_CELL_INDEX_PRESENT, H, 0, MAX_CELL_INDEX ^*/ /* MAX_NUM_CELLS - 1 */

    U8                 scellIndex;
    /*^ M, 0, B, 1, MAX_CELL_INDEX ^*/
}rrc_scell_to_release_t;

typedef struct
{
/* SPR-18445 start */
    U8                 count;    
    /*^ M, 0, B, 1, MAX_SCELL ^*/
/* SPR-18445 stop */

    rrc_scell_to_release_t   rrc_scell_to_release[MAX_SCELL];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

} rrc_scell_to_release_list_t;

typedef struct
{
    rrc_bitmask_t              bitmask;     /*^ BITMASK ^*/
#define RRM_SCELL_COMMOM_NON_UL_TDD_CONFIG_PRESENT                     0x01

    U8                   dl_bandwidth;
    /*^ M, 0, H, 0, 5 ^*/ /* rrc_rrm_scell_non_ul_dl_bandwidth_et */

    U8 antenna_port_count;             
    /*^ M, 0, H, 0, 2 ^*/ /* rrc_rrm_scell_non_ul_antenna_port_count_et */

    phich_config_t   phich_config;
    /*^ M, 0, N, 0, 0 ^*/

    pdsch_config_common_t           pdsch_config_common;
    /*^ M, 0, N, 0, 0 ^*/

    tdd_config_t                  tdd_config;  
    /*^ O, RRM_SCELL_COMMOM_NON_UL_TDD_CONFIG_PRESENT, N, 0, 0 ^*/

}rrc_radio_res_common_scell_non_ul_config_t;

typedef struct
{
  rrc_bitmask_t              bitmask;     /*^ BITMASK ^*/
#define RRM_RADIO_RES_CONFIG_COMMON_SCELL_UL_CONFIG_PRESENT               0x01

  rrc_radio_res_common_scell_non_ul_config_t  radio_res_common_scell_non_ul_config;             
  /*^ M, 0, N, 0, 0 ^*/

} rrc_radio_res_config_common_scell_t;

typedef struct
{
  rrc_bitmask_t         bitmask;
/*^ BITMASK ^*/
#define RRC_RRM_SCELL_MAC_MAIN_CONFIG_EXTN_DL_NUM_HARQ_PROC_PRESENT       0x01
#define RRC_RRM_SCELL_MAC_MAIN_CONFIG_EXTN_DL_INIT_MCS_PRESENT            0x02
#define RRC_RRM_SCELL_MAC_MAIN_CONFIG_EXTN_NUM_OF_LAYER_INFO_PRESENT      0x04
#define RRC_RRM_SCELL_MAC_MAIN_CONFIG_EXTN_CODE_BOOK_INDEX_INFO_PRESENT   0x08
#define RRC_RRM_SCELL_MAC_MAIN_CONFIG_EXTN_USER_LOCATION_TYPE_PRESENT     0x10
#define RRM_MAC_MAIN_CONFIG_EXT_RF_PARAMS_PRESENT                         0x20
#define RRM_MAC_SCELL_ACTIVATE_STATUS_PRESENT                             0x40

  U8                dl_num_harq_process;    
  /*^ O, RRC_RRM_SCELL_MAC_MAIN_CONFIG_EXTN_DL_NUM_HARQ_PROC_PRESENT, B, 1, 15 ^*/

  U8                dl_i_mcs;       
  /*^ O, RRC_RRM_SCELL_MAC_MAIN_CONFIG_EXTN_DL_INIT_MCS_PRESENT, H, 0, 28 ^*/ 

  U8                num_of_layer;   
  /*^ O, RRC_RRM_SCELL_MAC_MAIN_CONFIG_EXTN_NUM_OF_LAYER_INFO_PRESENT, B, 1, 4 ^*/ 

  U8                code_book_index;        
  /*^ O, RRC_RRM_SCELL_MAC_MAIN_CONFIG_EXTN_CODE_BOOK_INDEX_INFO_PRESENT, H, 0, 3 ^*/ 

  U8                user_location_type;               
  /*^ O, RRC_RRM_SCELL_MAC_MAIN_CONFIG_EXTN_USER_LOCATION_TYPE_PRESENT, H, 0, 1 ^*/ 

  U8                rf_params_half_duplex_fdd;
  /*^ O, RRM_MAC_MAIN_CONFIG_EXT_RF_PARAMS_PRESENT, H, 0, 1 ^*/ 

  U8 scell_activate_status;
  /*^ O, RRM_MAC_SCELL_ACTIVATE_STATUS_PRESENT, H, 0, 1 ^*/ 
  /*scell_activate_status_et */

} rrc_scell_mac_main_config_extensions_t;

typedef struct
{
  rrc_bitmask_t              bitmask;     /*^ BITMASK ^*/
#define RRM_SCELL_ADD_MOD_CELL_INDEX_PRESENT                              0x01
#define RRM_SCELL_ADD_MOD_CELL_IDENTIFICATION_PRESENT                     0x02
#define RRM_SCELL_ADD_MOD_RADIO_RES_CONFIG_COMMON_SCELL_PRESENT           0x04
#define RRM_SCELL_ADD_MOD_RADIO_RES_CONFIG_DEDICATED_SCELL_PRESENT        0x08
#define RRM_SCELL_ADD_MOD_MAC_SCELL_CONFIG_EXTENSION_SCELL_PRESENT        0x10
#define RRM_SCELL_ADD_MOD_MIMO_CAPABILITY_DL_R10_SCELL_PRESENT            0x20

  rrc_cell_index_t   cell_index;
/*^ O, RRM_SCELL_ADD_MOD_CELL_INDEX_PRESENT, H, 0, MAX_CELL_INDEX ^*/ /* MAX_NUM_CELLS - 1 */ 

  U8                           scellIndex;
/*^ M, 0, B, 1, MAX_CELL_INDEX ^*/

  rrc_scell_Identification_t   scell_Id;             
/*^ O, RRM_SCELL_ADD_MOD_CELL_IDENTIFICATION_PRESENT ^*/

  rrc_radio_res_config_common_scell_t   radio_res_config_common_scell;             
/*^ O, RRM_SCELL_ADD_MOD_RADIO_RES_CONFIG_COMMON_SCELL_PRESENT ^*/

  rrc_radio_res_config_dedicated_scell_t  radio_res_config_dedicated_scell;             
/*^ O, RRM_SCELL_ADD_MOD_RADIO_RES_CONFIG_DEDICATED_SCELL_PRESENT ^*/

  rrc_scell_mac_main_config_extensions_t  mac_main_config_extensions;
/*^ O, RRM_SCELL_ADD_MOD_MAC_SCELL_CONFIG_EXTENSION_SCELL_PRESENT ^*/

  U8                                      supported_mimo_capability_dl_r10;
/*^ O, RRM_SCELL_ADD_MOD_MIMO_CAPABILITY_DL_R10_SCELL_PRESENT ^*/
  /* mimo_capability_dl_r10_et */

} rrc_scell_to_add_mod_t;

typedef struct
{
/* SPR-18445 start */
    U8                     count;    
    /*^ M, 0, B, 1, MAX_SCELL ^*/
/* SPR-18445 start */

    rrc_scell_to_add_mod_t  rrc_scell_to_add_mod[MAX_SCELL];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrc_scell_to_add_mod_list_t;

typedef struct
{
  rrc_bitmask_t              bitmask;     /*^ BITMASK ^*/
#define RRM_SCELL_RELEASE_LIST_PRESENT              0x01
#define RRM_SCELL_ADD_MOD_LIST_PRESENT              0x02
#define RRM_SCELL_CONFIG_CA_LC_BANDWIDTH_DIST_INFO_PRESENT 0x04

  rrc_scell_to_release_list_t             scell_release_list;
/*^ O, RRM_SCELL_RELEASE_LIST_PRESENT ^*/
  rrc_scell_to_add_mod_list_t             scell_add_mod_list;
/*^ O, RRM_SCELL_ADD_MOD_LIST_PRESENT ^*/
  rrc_mac_ca_lc_bandwidth_dist_info_t     ca_lc_bandwidth_dist_info;
  /*^ O, RRM_SCELL_CONFIG_CA_LC_BANDWIDTH_DIST_INFO_PRESENT ^*/

} rrm_scell_config_t;

typedef struct
{
    rrc_bitmask_t             bitmask;    /*^ BITMASK ^*/
#define RRM_UE_RECONFIG_REQ_API_UE_RECONFIG_REQ_PRESENT   0x01
#define RRM_UE_RECONFIG_SCELL_CONFIG_PRESENT              0x02

    U16                       ue_index;
    radio_resource_reconfig_t    radio_resource_reconfig;
/*^ O, RRM_UE_RECONFIG_REQ_API_UE_RECONFIG_REQ_PRESENT ^*/

    rrm_scell_config_t                     scell_config; 
/*^ O, RRM_UE_RECONFIG_SCELL_CONFIG_PRESENT ^*/

}rrc_rrm_ue_reconfig_req_t; /*^ API, RRC_RRM_UE_RECONFIG_REQ ^*/

/******************************************************************************
 **             RRC_RRM_UE_RECONFIG_RESP
 *********************************************************************************/
typedef struct
{
    rrc_bitmask_t             bitmask;    /*^ BITMASK ^*/
#define RRM_UE_RECONFIG_RESP_API_FAIL_CAUSE_PRESENT      0x01

    U16                       ue_index;
    U8                        response;
/*^ M, 0, H, 0, 2 ^*/ /* rrc_return_et */
  U8                           fail_cause;
/*^ O, RRM_UE_RECONFIG_RESP_API_FAIL_CAUSE_PRESENT ^*/    
}rrc_rrm_ue_reconfig_resp_t; /*^ API, RRC_RRM_UE_RECONFIG_RESP ^*/

/******************************************************************************
            RRC_RRM_ERB_SETUP_REQ
******************************************************************************/

/* TS 36.413 - E-RAB Level QoS Parameters 9.2.1.60*/
typedef enum
{
  RRC_RRM_SHALL_NOT_TRIGGER_PRE_EMPTION,
  RRC_RRM_MAY_TRIGGER_PRE_EMPTION
}rrm_preemp_cap_et;

typedef enum
{
  RRC_RRM_NOT_PRE_EMPTABLE,
  RRC_RRM_PRE_EMPTABLE
} rrm_preemp_vul_et;


typedef struct
{
  U8        priority_level;           /*^ M, 0, H, 1, 15 ^*/
  U8        preemption_capability;
/*^ M, 0, H, 0, 1 ^*/ /*rrm_preemp_cap_et*/

  U8        preemption_vulnerability;
/*^ M, 0, H, 0, 1 ^*/ /*rrm_preemp_vul_et*/

} rrm_allocation_and_retention_priority_t;

/* TS 36.413 - E-RAB Level QoS Parameters 9.2.1.18*/
typedef struct
{
  U64      erab_max_bit_rate_dl;   /*^ M, 0, N, 0, 0 ^*/
  U64      erab_max_bit_rate_ul;   /*^ M, 0, N, 0, 0 ^*/
  U64      erab_guar_bit_rate_dl;  /*^ M, 0, N, 0, 0 ^*/
  U64      erab_guar_bit_rate_ul;  /*^ M, 0, N, 0, 0 ^*/
} rrm_gbr_qos_info_t;

/* TS 36.413 - E-RAB Level QoS Parameters 9.2.1.15*/
typedef struct
{
    rrc_bitmask_t               bitmask;       /*^ BITMASK ^*/
#define ERAB_LEVEL_QOS_PARAM_GBR_QOS_INFO_PRESENT     0x01
  U8                            qci;                    /*^ M, 0, N, 0, 0 ^*/
  rrm_allocation_and_retention_priority_t   alloc_and_reten_prior;
/*^ M, 0, N, 0, 0  ^*/

  rrm_gbr_qos_info_t            gbr_qos_info;           /*^ O, 1, N, 0, 0  ^*/
} rrm_erab_level_qos_params_t;

/* TS 36.413 - E-RAB to be Setup Item 9.1.4.1*/
typedef enum
{
    S1AP_DATA_FORWARDING_NOT_POSSIBLE
}rrc_data_fwding_not_possible_et;

#define ERAB_TO_BE_SETUP_DATA_FWDING_NOT_POSSIBLE_INFO_PRESENT_FLAG     0x01
/* LIPA start */
#define ERAB_TO_BE_SETUP_LIPA_RAB_IND_PRESENT_FLAG                      0x02
/* LIPA end */
typedef struct
{
  rrc_bitmask_t                 bitmask;      /*^ BITMASK ^*/
  U8                            erab_id; /*^ M, 0, H, 0, 15 ^*/
  U8                            dl_fwding; /*^ M, 0, H, 0, 1 ^*/
  rrm_erab_level_qos_params_t   erab_level_qos_params;     /*^ M, 0, N, 0, 0 ^*/
  U8                           data_fwding_not_possible;
  /*^ O, ERAB_TO_BE_SETUP_DATA_FWDING_NOT_POSSIBLE_INFO_PRESENT_FLAG, H, 0, 0 ^*/
  /* rrc_data_fwding_not_possible_et*/

  /* LIPA start */
  U8                           lipa_rab_ind;
  /*^ O, ERAB_TO_BE_SETUP_LIPA_RAB_IND_PRESENT_FLAG, H, 0, 1 ^*/ /*rrc_bool_et*/
  /* LIPA end */
  
} rrm_erab_to_be_setup_item_t;

/* TS 36.413 - E-RAB to be Setup Item list 9.1.4.1*/
typedef struct
{

	/* SPR_17664_start*/
    /* Coverity Fix 99933 Start */
    U16                          num_of_list;   /*^ M, 0, H, 1, MAX_ERAB_COUNT ^*/
    rrm_erab_to_be_setup_item_t  erab_to_be_setup_item[MAX_ERAB_COUNT]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    /* Coverity Fix 99933 End */
	/* SPR_17664_end*/
} rrm_erab_to_be_setup_item_list_t;

typedef struct
{
    U64              ext_ue_agg_max_bit_rate_ul;
    /*^ M, 0, N, 0, 0 ^*/

    U64              ext_ue_agg_max_bit_rate_dl;
    /*^ M, 0, N, 0, 0 ^*/
}rrm_ue_agg_max_bit_rate_ext_t;

/* TS 36.413 - UE Aggregate Maximum bit rate 9.2.1.20*/
#define RRC_RRM_EXT_UE_AGG_MAX_BITRATE_PRESENT    0x01 
typedef struct
{
    rrc_bitmask_t                    bitmask;
    /*^ BITMASK ^*/

    U64                              ue_agg_max_bit_rate_ul;
    /*^ M, 0, N, 0, 0 ^*/

    U64                              ue_agg_max_bit_rate_dl;
    /*^ M, 0, N, 0, 0 ^*/

    rrm_ue_agg_max_bit_rate_ext_t    ue_agg_max_bit_rate_ext; 
    /*^ O, RRC_RRM_EXT_UE_AGG_MAX_BITRATE_PRESENT, N, 0, 0 ^*/
} rrm_ue_agg_max_bit_rate_t;

typedef enum
{
    CS_PS_TYPE,
    CS_ONLY_TYPE
}rrc_srvcc_ho_type_et;

/*cr_407 enum*/
typedef enum
{
        PS_SERVICE_NOT_AVAILABLE
}rrc_ps_service_not_available_et;

typedef enum
{
    CS_FALLBACK_REQUIRED,
    CS_FALLBACK_HIGH_PRIORITY
}rrc_csf_indicator_et;


#define MAX_NO_OF_EQUIVALENT_PLMN_IDS 15
#define MAX_NO_OF_EPLMNS_PlUS_ONE     16
#define MAX_NO_OF_FORBIDDEN_LACS      4096
#define MAX_NO_OF_FORBIDDEN_TACS      4096


typedef struct
{
    U8   tac[2]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}forbidden_tac_t;

typedef struct
{
/*SPR_17664_start*/
    U16               count; /*^ M, 0, B, 1, MAX_NO_OF_FORBIDDEN_TACS ^*/
/*SPR_17664_end*/
    forbidden_tac_t   forbidden_tac[MAX_NO_OF_FORBIDDEN_TACS]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}forbidden_tac_list_t;


typedef struct
{
    plmn_identity_t         plmn_identity; /*^ M, 0, N, 0, 0 ^*/
    forbidden_tac_list_t    forbidden_tac; /*^ M, 0, N, 0, 0 ^*/
}forbidden_ta_t; 

typedef struct
{
/*SPR_17664_start*/
    U16              count; /*^ M, 0, B, 1, MAX_NO_OF_EPLMNS_PlUS_ONE ^*/
/*SPR_17664_end*/
    forbidden_ta_t   forbidden_ta[MAX_NO_OF_EPLMNS_PlUS_ONE]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}forbidden_ta_list_t;   

typedef struct
{
        U8                                 lac[2]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}forbidden_lac_t;

typedef struct
{
/*SPR_17664_start*/
    U16               count; /*^ M, 0, B, 1, MAX_NO_OF_FORBIDDEN_LACS ^*/
/*SPR_17664_end*/
    forbidden_lac_t   forbidden_lac[MAX_NO_OF_FORBIDDEN_LACS]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}forbidden_lac_list_t;

typedef struct
{
    plmn_identity_t                    plmn_identity; /*^ M, 0, N, 0, 0 ^*/
    forbidden_lac_list_t               forbidden_lacs_list; /*^ M, 0, N, 0, 0 ^*/
}forbidden_la_t;

typedef struct
{
/*SPR_17664_start*/
    U16             count; /*^ M, 0, B, 1,MAX_NO_OF_EPLMNS_PlUS_ONE ^*/
/*SPR_17664_end*/
    forbidden_la_t  forbidden_la[MAX_NO_OF_EPLMNS_PlUS_ONE]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}forbidden_la_list_t;

typedef struct
{
    plmn_identity_t   plmn_identity; /*^ M, 0, N, 0, 0 ^*/
}equiv_plmn_t;


typedef struct
{
/*SPR_17664_start*/
    U8            count; /*^ M, 0, B, 1, MAX_NO_OF_EQUIVALENT_PLMN_IDS ^*/
/*SPR_17664_end*/
    equiv_plmn_t  equiv_plmn[MAX_NO_OF_EQUIVALENT_PLMN_IDS]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}equiv_plmn_list_t;

typedef struct
{
    U16                                bitmask; /*^ BITMASK ^*/
#define HO_RESTRICTION_EQUIV_PLMN_LIST_PRESENT      0x01
#define HO_RESTRICTION_FORBIDDEN_TA_LIST_PRESENT    0x02
#define HO_RESTRICTION_FORBIDDEN_LA_LIST_PRESENT    0x04
#define HO_RESTRICTION_FORBIDDEN_INTER_RAT_PRESENT  0x08
#ifdef ENDC_ENABLED
#define HO_RESTRICTION_NR_RESTRICTION_PRESENT       0x10
#endif
    plmn_identity_t       serving_plmn_identity; /*^ M, 0, N, 0, 0 ^*/
    equiv_plmn_list_t     equiv_plmn_list;  
    /*^ O, HO_RESTRICTION_EQUIV_PLMN_LIST_PRESENT, N, 0, 0 ^*/
    
    forbidden_ta_list_t   forbidden_ta_list; 
    /*^ O, HO_RESTRICTION_FORBIDDEN_TA_LIST_PRESENT, N, 0, 0 ^*/
    
    forbidden_la_list_t   forbidden_la_list;
    /*^ O, HO_RESTRICTION_FORBIDDEN_LA_LIST_PRESENT, N, 0, 0 ^*/
    
    U8                    forbidden_inter_rat; 
    /*^ O, HO_RESTRICTION_FORBIDDEN_INTER_RAT_PRESENT, H, 0, 5 ^*//*rrc_forbidden_inter_rat_et*/
#ifdef ENDC_ENABLED
    U8                    nr_restriction_EPS_as_SecRat;
#endif
}ho_restriction_list_t;


typedef struct
{
  rrc_bitmask_t             bitmask;      /*^ BITMASK ^*/
#define RRC_RRM_ERB_SETUP_REQ_UE_AGG_MAX_BIT_RATE_PRESENT      0x01
#define RRC_RRM_ERB_SETUP_REQ_SRVCC_HO_OP_POSSIBLE_PRESENT     0x02
#define RRC_RRM_ERB_SETUP_REQ_CSF_INDICATOR_PRESENT            0x04
#define RRC_RRM_ERB_SETUP_REQ_HO_RESTRICTION_LIST_PRESENT      0x08
#define RRC_RRM_ERB_SETUP_REQ_SUB_PROFILE_ID_PRESENT           0x10
#define RRC_RRM_ERB_SETUP_REQ_REGISTERED_LAI_PRESENT           0x20
#define RRC_RRM_ERB_SETUP_REQ_CSG_MEM_STATUS                   0x40


  U16                               ue_index;
  rrm_ue_agg_max_bit_rate_t         ue_agg_max_bit_rate; 
  /*^ O, RRC_RRM_ERB_SETUP_REQ_UE_AGG_MAX_BIT_RATE_PRESENT, N, 0, 0 ^*/
  
  rrm_erab_to_be_setup_item_list_t  erab_to_be_setup_item_list; /*^ M, 0, N, 0, 0 ^*/
  U8                       srvcc_ho_op_possible;    
  /*^ O, RRC_RRM_ERB_SETUP_REQ_SRVCC_HO_OP_POSSIBLE_PRESENT, H, 0, 1 ^*/ /* rrc_srvcc_ho_type_et */
  
  U8                       csf_indicator;  
  /*^ O, RRC_RRM_ERB_SETUP_REQ_CSF_INDICATOR_PRESENT, H, 0, 1 ^*/ /* rrc_csf_indicator_et */
  
  ho_restriction_list_t    ho_restriction_list;  
  /*^ O, RRC_RRM_ERB_SETUP_REQ_HO_RESTRICTION_LIST_PRESENT, N, 0, 0 ^*/
  
  U16                      sub_profile_id; 
  /*^ O, RRC_RRM_ERB_SETUP_REQ_SUB_PROFILE_ID_PRESENT, B, 1, 256 ^*/
  
/* cr_761 sturcture */
  registered_lai_t          registered_lai;
  /*^ O, RRC_RRM_ERB_SETUP_REQ_REGISTERED_LAI_PRESENT, N, 0, 0 ^*/

  U8                       csg_mem_status;
  /*^ O, RRC_RRM_ERB_SETUP_REQ_CSG_MEM_STATUS, N, 0, 0 ^*/ /*rrc_csg_membership_status_et */

}rrc_rrm_erb_setup_req_t; /*^ API, RRC_RRM_ERB_SETUP_REQ ^*/

/******************************************************************************
    TS 36.331 - 6.3.2 PDCP-Config IE
******************************************************************************/

typedef enum
{
  RRC_PDCP_SN_SIZE_7_BITS,
  RRC_PDCP_SN_SIZE_12_BITS
} rrc_pdcp_sn_size_et;

typedef enum
{
  RRC_PDCP_DISCARD_TIMER_MS_50,
  RRC_PDCP_DISCARD_TIMER_MS_100,
  RRC_PDCP_DISCARD_TIMER_MS_150,
  RRC_PDCP_DISCARD_TIMER_MS_300,
  RRC_PDCP_DISCARD_TIMER_MS_500,
  RRC_PDCP_DISCARD_TIMER_MS_750,
  RRC_PDCP_DISCARD_TIMER_MS_1500,
  RRC_PDCP_DISCARD_TIMER_INFINITY
} rrc_pdcp_discard_timer_et;

typedef struct
{
  U8            profile0x0001;   /*^ M, 0, H, 0, 1 ^*/      /* rrc_bool_et */
  U8            profile0x0002;   /*^ M, 0, H, 0, 1 ^*/      /* rrc_bool_et */
  U8            profile0x0003;   /*^ M, 0, H, 0, 1 ^*/      /* rrc_bool_et */
  U8            profile0x0004;   /*^ M, 0, H, 0, 1 ^*/      /* rrc_bool_et */
  U8            profile0x0006;   /*^ M, 0, H, 0, 1 ^*/      /* rrc_bool_et */
  U8            profile0x0101;   /*^ M, 0, H, 0, 1 ^*/      /* rrc_bool_et */
  U8            profile0x0102;   /*^ M, 0, H, 0, 1 ^*/      /* rrc_bool_et */
  U8            profile0x0103;   /*^ M, 0, H, 0, 1 ^*/      /* rrc_bool_et */
  U8            profile0x0104;   /*^ M, 0, H, 0, 1 ^*/      /* rrc_bool_et */
} rrc_pdcp_rohc_profile_t;

typedef struct
{
  U16                        max_cid;        /*^ M, 0, B, 1, 16383 ^*/
  rrc_pdcp_rohc_profile_t    rohc_profile;   /*^ M, 0, N, 0, 0 ^*/
} rrc_pdcp_rohc_config_t;

/*PDCP-Config, see headerCompression IE (notUsed)*/
typedef struct
{
    rrc_bitmask_t         bitmask;                 /*^ BITMASK ^*/
#define RRC_PDCP_HEADER_COMPRESSION_ROHC_CONFIG_PRESENT  0x01

    rrc_pdcp_rohc_config_t  rohc_config;
/*^ O, RRC_PDCP_HEADER_COMPRESSION_ROHC_CONFIG_PRESENT ^*/

} rrc_pdcp_header_compression_config_t;

typedef struct
{
    rrc_bitmask_t         bitmask;                 /*^ BITMASK ^*/
#define RRC_PDCP_CONFIG_DISCARD_TIMER_PRESENT                   0x01
#define RRC_PDCP_CONFIG_RLC_AM_STATUS_REPORT_REQ_PRESENT        0x02
#define RRC_PDCP_CONFIG_RLC_UM_PDCP_SN_SIZE_PRESENT             0x04
/* PSR Start */
#define RRC_PDCP_CONFIG_RLC_AM_ENB_STATUS_REPORT_REQ_PRESENT    0x08
/* PSR End */

  U8                      discard_timer;
/*^ O, RRC_PDCP_CONFIG_DISCARD_TIMER_PRESENT, H, 1, 7 ^*/
/* rrc_pdcp_discard_timer_et */

  U8                      rlc_am_status_report_required;
/*^ O, RRC_PDCP_CONFIG_RLC_AM_STATUS_REPORT_REQ_PRESENT, H, 0, 1 ^*/
/* rrc_bool_et */

  U8                      rlc_um_pdcp_sn_size;
/*^ O, RRC_PDCP_CONFIG_RLC_UM_PDCP_SN_SIZE_PRESENT, H, 1, 1 ^*/
/* rrc_pdcp_sn_size_et */

  rrc_pdcp_header_compression_config_t   header_compression;

/* PSR Start */
  U8                      rlc_am_enb_status_report_required;
/*^ O, RRC_PDCP_CONFIG_RLC_AM_ENB_STATUS_REPORT_REQ_PRESENT, H, 0, 1 ^*/
/* rrc_bool_et */
/* PSR End */

} rrc_pdcp_config_t;

/******************************************************************************
            RRC_RRM_ERB_SETUP_RESP
******************************************************************************/

typedef struct
{
    rrc_bitmask_t      bitmask;       /*^ BITMASK ^*/
#define DRB_LWA_CONFIG_LWA_BEARER_TYPE    0x01

    /* Whether bearer is LWA type or not */
    U8                 lwa_bearer;

    /* Indicates the type of LWA bearer */
    U8                 lwa_bearer_type;
/*^ O, DRB_LWA_CONFIG_LWA_BEARER_TYPE, N, 0, 0 ^*/

}rrm_rrc_drb_lwa_config_t;


typedef struct
{
    U8 allocation_retention_priority;
    U8 qos_profile_data_size;

/*SPR_17664_start*/
/*^ M, 0, B, 3, MAX_S1U_QOS_PROFILE_DATA_OCTET_SIZE ^*/
/*SPR_17664_end*/
    U8 qos_profile_data[MAX_S1U_QOS_PROFILE_DATA_OCTET_SIZE];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

} rrm_s1u_qos_profile_t;

/*eMTC changes start*/
typedef struct
{
    U8 narrowband_idx;
    /*^ M, 0, B, 1, EMTC_MAX_AVAILABLE_NARROWBAND ^*/
    U8 prb_bitmap;
    /*^ M, 0, N, 0, 0 ^*/
}RrcRrmErabMpdcchConfigInfo_t;
/*eMTC changes end*/

typedef struct
{
    U32 qos_id;
    U8  sap_flags;
    rrm_s1u_qos_profile_t qos_profile;
    U8  seq_flag;   /*^ M, 0, H, 0, 1 ^*/ /* rrc_bool_et */
    U8  reordering_reqd;    /*^ M, 0, H, 0, 1 ^*/ /* rrc_bool_et */
} rrm_s1u_config_t;

/* TS 36.331 - 6.3.2 RadioResourceConfigDedicated IE DRB-ToAddMod IE. */
typedef struct
{
    rrc_bitmask_t      bitmask;       /*^ BITMASK ^*/
#define RRM_DRB_CONFIG_BEARED_ID_PRESENT                0x01
#define RRM_DRB_CONFIG_LOGICAL_CHANNEL_IDENTITY_PRESENT 0x02
#define RRM_DRB_CONFIG_PDCP_CONFIG_PRESENT              0x04
#define RRM_DRB_CONFIG_RLC_CONFIG_PRESENT               0x08
#define RRM_DRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT   0x10
#define RRM_DRB_CONFIG_S1U_CONFIG_PRESENT               0x20
#define RRM_DRB_CONFIG_MAC_LC_CONFIG_ENB_PRESENT        0x40
#define RRM_DRB_CONFIG_RLC_CONFIG_UE_PRESENT            0x80
#define RRM_DRB_CONFIG_BEARER_TYPE_PRESENT              0x100

#define RRM_DRB_CONFIG_LWA_CAPABILITY_PRESENT           0x200 
#define RRM_DRB_CONFIG_LWA_CONFIG_PRESENT               0x400
    /* MENB_CHANGES_START */
#define RRM_DRB_CONFIG_DC_BEARER_TYPE_PRESENT           0x800
    /* MENB_CHANGES_END */

    U8                    erab_id;                
    /*^ O, RRM_DRB_CONFIG_BEARED_ID_PRESENT, H, 0, 15 ^*/

    U8                    drb_id;                 /*^ M, 0, B, 1, 32 ^*/
    U8                    logical_channel_identity;
    /*^ O, RRM_DRB_CONFIG_LOGICAL_CHANNEL_IDENTITY_PRESENT, B, 3, 10 ^*/

    rrc_pdcp_config_t     pdcp_config;
    /*^ O, RRM_DRB_CONFIG_PDCP_CONFIG_PRESENT ^*/

    rrc_rlc_config_enb_t      rlc_config_enb;
    /*^ O, RRM_DRB_CONFIG_RLC_CONFIG_PRESENT ^*/

    rrm_logical_channel_config_t logical_channel_config;
    /*^ O, RRM_DRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT ^*/

    rrm_s1u_config_t      s1u_config;
    /*^ O, RRM_DRB_CONFIG_S1U_CONFIG_PRESENT ^*/

    rrm_mac_lc_config_t   mac_lc_config_enb;
    /*^ O, RRM_DRB_CONFIG_MAC_LC_CONFIG_ENB_PRESENT ^*/

    rrc_rlc_config_t      rlc_config_ue;
    /*^ O, RRM_DRB_CONFIG_RLC_CONFIG_UE_PRESENT ^*/

    U8                    bearer_type;
    /*^ O, RRM_DRB_CONFIG_BEARER_TYPE_PRESENT, H, 0, 1 ^*/
    /* COV_37478_Fix_Start */
    U8                        lwa_capable;
    /*^ O, RRM_DRB_CONFIG_LWA_CAPABILITY_PRESENT, H, 0, 1 ^*/
    /* COV_37478_Fix_End */

    rrm_rrc_drb_lwa_config_t  lwa_config;
    /*^ O, RRM_DRB_CONFIG_LWA_CONFIG_PRESENT ^*/

    /* MENB_CHANGES_START */
    U8                        dc_bearer_type;
    /* OPTION3X Changes Start */
    /*^ O, RRM_DRB_CONFIG_DC_BEARER_TYPE_PRESENT, H, 0, DC_BEARER_TYPE_LAST ^*/
    /* OPTION3X Changes End */
    /* MENB_CHANGES_END */
} rrm_drb_config_t;

typedef struct
{
/*SPR_17664_start*/
  U8                     drb_count; /*^ M, 0, B, 1, MAX_LC_COUNT ^*/
/*SPR_17664_end*/
  rrm_drb_config_t       drb_config[MAX_LC_COUNT];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

} rrm_drb_to_add_info_list_t;

typedef struct
{
  U8            erab_id; /*^ M, 0, H, 0, 15 ^*/
  rrm_cause_t   cause;
} rrm_drb_failed_to_add_item_t;

typedef struct
{
/* SPR_18490 start */
  U8                            count; /*^ M, 0, B, 1, 16 ^*/
  rrm_drb_failed_to_add_item_t  drb_failed_to_add[MAX_ERAB_COUNT];
/* SPR_18490 stop */
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

} rrm_drb_failed_to_add_info_list_t;

typedef struct
{
  rrc_bitmask_t              bitmask;     /*^ BITMASK ^*/
#define RRM_PROXIMITY_IND_EUTRA_PRESENT               0x01
#define RRM_PROXIMITY_IND_UTRA_PRESENT                0x02

  U8                           proximity_ind_eutra;
/*^ O, RRM_PROXIMITY_IND_EUTRA_PRESENT, H, 0, 0 ^*/

  U8                           proximity_ind_utra;
/*^ O, RRM_PROXIMITY_IND_UTRA_PRESENT, H, 0, 0 ^*/

} rrm_report_proximity_config_r9_t;

typedef struct
{
    rrc_bitmask_t              bitmask;     /*^ BITMASK ^*/
#define RRM_ERAB_SETUP_RADIO_RES_CONFIG_DRB_TO_ADD_LIST_PRESENT           0x01
#define RRM_ERAB_SETUP_RADIO_RES_CONFIG_MAC_CONFIG_PRESENT                0x02
#define RRM_ERAB_SETUP_RADIO_RES_CONFIG_SPS_CONFIG_PRESENT                0x04
#define RRM_ERAB_SETUP_RADIO_RES_CONFIG_PHY_CONFIG_DED_PRESENT            0x08
#define RRM_ERAB_SETUP_RADIO_RES_CONFIG_MEAS_CONFIG_PRESENT               0x10
#define RRM_ERAB_SETUP_RADIO_RES_CONFIG_DRB_FAILED_TO_ADD_LIST_PRESENT    0x20
#define RRM_ERAB_SETUP_RADIO_RES_CONFIG_SRB_INFO_PRESENT                  0x40
#define RRM_ERAB_SETUP_RADIO_RES_CONFIG_RLF_TIMERS_AND_CONSTANTS_PRESENT  0x80
#define RRM_ERAB_SETUP_RADIO_RES_CONFIG_PROXIMITY_PRESENT                 0x100
/* Bug 3961 Fix Start */
#define RRM_ERAB_SETUP_RADIO_RES_CONFIG_MEAS_SUBFRAME_PATTERN_PCELL_R10_PRESENT  0x200
/* Bug 3961 Fix End */

    rrm_drb_to_add_info_list_t  drb_to_add_list;
/*^ O, RRM_ERAB_SETUP_RADIO_RES_CONFIG_DRB_TO_ADD_LIST_PRESENT ^*/

    rrm_mac_config_t            mac_config;
/*^ O, RRM_ERAB_SETUP_RADIO_RES_CONFIG_MAC_CONFIG_PRESENT ^*/

    rrc_sps_config_t            sps_config;
/*^ O, RRM_ERAB_SETUP_RADIO_RES_CONFIG_SPS_CONFIG_PRESENT ^*/

    rrc_phy_physical_config_dedicated_t      physical_config_dedicated;
/*^ O, RRM_ERAB_SETUP_RADIO_RES_CONFIG_PHY_CONFIG_DED_PRESENT ^*/

    rrm_meas_config_t           meas_config;
/*^ O, RRM_ERAB_SETUP_RADIO_RES_CONFIG_MEAS_CONFIG_PRESENT ^*/

    rrm_drb_failed_to_add_info_list_t  drb_failed_to_add_list;
/*^ O, RRM_ERAB_SETUP_RADIO_RES_CONFIG_DRB_FAILED_TO_ADD_LIST_PRESENT ^*/
    rrm_srb_info_t              srb_info;
/*^ O, RRM_ERAB_SETUP_RADIO_RES_CONFIG_SRB_INFO_PRESENT ^*/

/* cr_220 structure */

    rlf_timers_and_constants_r9_t rlf_timers_and_constants_r9;
/*^ O, RRM_ERAB_SETUP_RADIO_RES_CONFIG_RLF_TIMERS_AND_CONSTANTS_PRESENT ^*/ 

    rrm_report_proximity_config_r9_t   proximity_config;
/*^ O, RRM_ERAB_SETUP_RADIO_RES_CONFIG_PROXIMITY_PRESENT ^*/ 

/* Bug 3961 Fix Start */
    rrc_meas_subframe_pattern_pcell_r10_t  rrc_meas_subframe_pattern_pcell_r10;
  /*^ O, RRM_ERAB_SETUP_RADIO_RES_CONFIG_MEAS_SUBFRAME_PATTERN_PCELL_R10_PRESENT ^*/
/* Bug 3961 Fix End */
} rrm_erab_setup_radio_res_config_t;

typedef struct
{
     U8                 erab_id;   
/*^ M, 0, H, 0, 15 ^*/ 
     rrm_cause_t   cause;    
} rrm_erab_failed_item_t;

typedef struct
{
     U16                     count;    
	/* SPR_17664_start*/
    /* Coverity Fix 99939 Start */
/*^ M, 0, B, 1, MAX_ERAB_COUNT ^*/
     rrm_erab_failed_item_t  erab_to_be_release_item[MAX_ERAB_COUNT];
     /* Coverity Fix 99939 End */
	/* SPR_17664_end*/
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

} rrm_erab_to_be_released_item_list_t;


typedef struct
{
    rrc_bitmask_t              bitmask;     /*^ BITMASK ^*/
#define RRM_CSI_RS_R10_SETUP                        0x01

   rrc_csi_rs_r10_setup_t  csi_rs_r10_setup;
    /*^ O, RRM_CSI_RS_R10_SETUP  ^*/ 

} rrc_csi_rs_r10_t;

typedef struct
{
    rrc_bitmask_t              bitmask;     /*^ BITMASK ^*/
#define RRM_ZERO_TX_POWER_CSI_SETUP     0x01

   rrc_zeroTxPowerCsi_setup_t  zeroTxPowerCsi_setup;
    /*^ O, RRM_ZERO_TX_POWER_CSI_SETUP  ^*/ 

} rrc_zeroTxPowerCsi_rs_r10_t;


/*NR_DC Code Change Start*/
typedef struct _scg_serv_cell_index_range_t
{
    /* coverity fix 254845 */
    U8                              low_bound_cell_index;   /*^ M, 0, H, 0, 31 ^*/
    U8                              up_bound_cell_index;    /*^ M, 0, H, 0, 31 ^*/
    /* coverity fix 254845 */
}scg_serv_cell_index_range_t;


typedef struct _band_combination_index_list_t
{
    U16   band_comb_count; /*^ M, 0, B, 1, MAX_BAND_COMB_EUTRA_NR^*/

    U8    band_comb_index[MAX_BAND_COMB_EUTRA_NR];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}band_combination_index_list_t;

#define POWER_COORDINATION_FR1_P_MAX_NR_FR1_PRESENT    0x01
#define POWER_COORDINATION_FR1_P_MAX_EUTRA_PRESENT     0x02
#define POWER_COORDINATION_FR1_P_MAX_UE_FR1_PRESENT    0x04
typedef struct _power_coordination_fr1_t
{
    rrc_bitmask_t    bitmask;
    /*^ BITMASK ^*/

    S8               p_max_nr_fr1;
    /*^ O, POWER_COORDINATION_FR1_P_MAX_NR_FR1_PRESENT, B, -30, 33 ^*/

    S8               p_max_eutra;
    /*^ O, POWER_COORDINATION_FR1_P_MAX_EUTRA_PRESENT, B, -30, 33 ^*/

    S8               p_max_ue_fr1;
    /*^ O, POWER_COORDINATION_FR1_P_MAX_UE_FR1_PRESENT,BB, -30, 33 ^*/
}power_coordination_fr1_t;

typedef struct _config_restrict_info_t
{
    rrc_bitmask_t                   bitmask;
    /*^ BITMASK ^*/
#define NR_CG_CONFIG_RESTRICT_INFO_ALLOWED_BC_LIST_PRESENT      0x01
#define NR_CG_CONFIG_RESTRICT_INFO_POWER_COORD_FR1_PRESENT      0x02
#define NR_CG_CONFIG_RESTRICT_INFO_SCG_SERV_CELL_INDEX_PRESENT  0x04
#define NR_CG_CONFIG_RESTRICT_INFO_MAX_MEAS_FREQ_SCG_NR_PRESENT 0x08
#define NR_CG_CONFIG_RESTRICT_INFO_MAX_MEAS_ID_SCG_NR_PRESENT   0x10

    band_combination_index_list_t   allowed_bc_list_mrdc;     
    /*^ O, NR_CG_CONFIG_RESTRICT_INFO_ALLOWED_BC_LIST_PRESENT ^*/ 

    scg_serv_cell_index_range_t     scg_serv_cell_index_range;
    /*^ O, NR_CG_CONFIG_RESTRICT_INFO_SCG_SERV_CELL_INDEX_PRESENT ^*/

    power_coordination_fr1_t        power_coordination_fr1;
    /*^ O, NR_CG_CONFIG_RESTRICT_INFO_POWER_COORD_FR1_PRESENT ^*/
 
    U8                              max_meas_freq_scg_nr;
    /*^ O, NR_CG_CONFIG_RESTRICT_INFO_MAX_MEAS_FREQ_SCG_NR_PRESENT, B, 1, 32 ^*/
 
    U8                              max_meas_id_scg_nr;
    /*^ O, NR_CG_CONFIG_RESTRICT_INFO_MAX_MEAS_ID_SCG_NR_PRESENT, B, 1, 62 ^*/ 
}config_restrict_info_t;

typedef struct _rrc_rrm_cn_association_t
{
    rrc_bitmask_t    bitmask;
    /*^ BITMASK ^*/
#define CN_ASSOCIATION_ERAB_ID_PRESENT           0x01

    U8               erab_id;
    /*^ M, 0, H, 0, 15 ^*/
}rrc_rrm_cn_association_t;

/* OPTION3X Changes Start */
typedef struct _drb_to_add_mod_t
{
    rrc_bitmask_t               bitmask;
    /*^ BITMASK ^*/
#define DRB_TO_ADD_MOD_CN_ASSOCIATION_PRESENT           0x01
#define DRB_TO_ADD_MOD_REESTABLISH_PDCP_PRESENT         0x02
#define DRB_TO_ADD_MOD_RECOVER_PDCP_PRESENT             0x04
#define DRB_TO_ADD_MOD_PDCP_CONFIG_PRESENT              0x08
    U8                          drb_id;
    /*^ M, 0, B, 1, 32 ^*/

    rrc_rrm_cn_association_t    cn_association;
    /*^ O, DRB_TO_ADD_MOD_CN_ASSOCIATION_PRESENT ^*/

    rrc_pdcp_config_t           pdcp_config;
    /*^ O, DRB_TO_ADD_MOD_PDCP_CONFIG_PRESENT ^*/

} drb_to_add_mod_t;

typedef struct _drb_to_add_mod_list_t
{
    U8                  count;
    /*^ M, 0, B, 1, MAX_LC_COUNT ^*/
    /* Max DRB is 29 as per spec 38.331 */

    drb_to_add_mod_t    drb_to_add_mod[MAX_LC_COUNT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} drb_to_add_mod_list_t;

typedef struct _radio_bearer_config_t
{
    rrc_bitmask_t                   bitmask;    /*^ BITMASK ^*/
#define RADIO_BEARER_CONFIG_SRB_TO_ADD_MOD_LIST_PRESENT     0x01
#define RADIO_BEARER_CONFIG_SRB3_TO_RELEASE_PRESENT         0x02
#define RADIO_BEARER_CONFIG_DRB_TO_ADD_MOD_LIST_PRESENT     0x04
#define RADIO_BEARER_CONFIG_DRB_TO_RELEASE_LIST_PRESENT     0x08
#define RADIO_BEARER_CONFIG_SECURITY_CONFIG_PRESENT         0x10

    drb_to_add_mod_list_t           drb_to_add_mod_list;
    /*^ O, RADIO_BEARER_CONFIG_DRB_TO_ADD_MOD_LIST_PRESENT ^*/
} radio_bearer_config_t;
/* OPTION3X Changes End */

typedef struct _rrc_rrm_meas_quantity_res_t
{
    rrc_bitmask_t               bitmask;
    /*^ BITMASK ^*/

#define RRC_RRM_MEAS_QUANTITY_RES_RSRP_PRESENT             0x01
#define RRC_RRM_MEAS_QUANTITY_RES_RSRQ_PRESENT             0x02
#define RRC_RRM_MEAS_QUANTITY_RES_SINR_PRESENT             0x04

    U8                          rsrp;
    /*^ O, RRC_RRM_MEAS_QUANTITY_RES_RSRP_PRESENT, H, 0, 127 ^*/

    U8                          rsrq;
    /*^ O, RRC_RRM_MEAS_QUANTITY_RES_RSRQ_PRESENT, H, 0, 127 ^*/

    U8                          sinr;
    /*^ O, RRC_RRM_MEAS_QUANTITY_RES_SINR_PRESENT, H, 0, 127 ^*/
}rrc_rrm_meas_quantity_res_t;

typedef struct _rrc_rrm_meas_cell_results_t
{ 
    rrc_bitmask_t               bitmask;
    /*^ BITMASK ^*/

#define RRC_RRM_MEAS_CELL_RESULT_SSB_RESULT_PRESENT             0x01
#define RRC_RRM_MEAS_CELL_RESULT_CSI_RS_RESULT_PRESENT          0x02

  rrc_rrm_meas_quantity_res_t   result_ssb_cell;
  /*^ O, RRC_RRM_MEAS_CELL_RESULT_SSB_RESULT_PRESENT, N, 0, 0 ^*/

  rrc_rrm_meas_quantity_res_t   result_csi_rs_cell;
  /*^ O, RRC_RRM_MEAS_CELL_RESULT_CSI_RS_RESULT_PRESENT, N, 0, 0 ^*/
}rrc_rrm_meas_cell_results_t;


typedef struct _rrc_rrm_results_per_ssb_idx_t
{
    rrc_bitmask_t               bitmask;
    /*^ BITMASK ^*/
#define RRC_RRM_RESULT_PER_SSB_SSB_RESULTS_PRESENT          0x01

    U8                          ssb_index;
    /*^ M, 0, H, 0, 63 ^*/ 

    rrc_rrm_meas_quantity_res_t ssb_results;
    /*^ O, RRC_RRM_RESULT_PER_SSB_SSB_RESULTS_PRESENT, N, 0, 0 ^*/ 
}rrc_rrm_results_per_ssb_idx_t;

typedef struct _rrc_rrm_results_per_ssb_idx_list_t
{
    U8                            count;
    /*^ M, 0, H, 0, MAX_NUM_OF_IDX_TO_REPORT2 ^*/ 

    rrc_rrm_results_per_ssb_idx_t rrc_rrm_results_per_ssb_idx[MAX_NUM_OF_IDX_TO_REPORT2];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrc_rrm_results_per_ssb_idx_list_t;

typedef struct _rrc_rrm_results_per_csi_rs_idx_t
{
    rrc_bitmask_t               bitmask;
    /*^ BITMASK ^*/
#define RRC_RRM_RESULT_PER_CSI_RS_CSI_RS_RESULTS_PRESENT          0x01

    U8                          csi_rs_index;
    /*^ M, 0, H, 0, 95 ^*/ 

    rrc_rrm_meas_quantity_res_t csi_rs_results;
    /*^ O, RRC_RRM_RESULT_PER_CSI_RS_CSI_RS_RESULTS_PRESENT, N, 0, 0 ^*/ 
}rrc_rrm_results_per_csi_rs_idx_t;

typedef struct _rrc_rrm_results_per_csi_rs_idx_list_t
{
    U8                               count;
    /*^ M, 0, H, 0, MAX_NUM_OF_IDX_TO_REPORT2 ^*/ 

    rrc_rrm_results_per_csi_rs_idx_t rrc_rrm_results_per_csi_rs_idx[MAX_NUM_OF_IDX_TO_REPORT2];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrc_rrm_results_per_csi_rs_idx_list_t;

typedef struct _rrc_rrm_meas_rs_idx_results_t
{
    rrc_bitmask_t                              bitmask;
    /*^ BITMASK ^*/
#define RRC_RRM_MEAS_RESULT_RS_PER_SSB_PRESENT              0x01
#define RRC_RRM_MEAS_RESULT_RS_PER_CSI_RS_PRESENT           0x02

    rrc_rrm_results_per_ssb_idx_list_t         results_per_ssb_idx_list; 
    /*^ O, RRC_RRM_MEAS_RESULT_RS_PER_SSB_PRESENT, N, 0, 0 ^*/

    rrc_rrm_results_per_csi_rs_idx_list_t      results_per_csi_rs_idx_list;
    /*^ O, RRC_RRM_MEAS_RESULT_RS_PER_CSI_RS_PRESENT, N, 0, 0 ^*/

}rrc_rrm_meas_rs_idx_results_t;

#define RRC_RRM_MEAS_RESULT_RS_IDX_RESULT_PRESENT           0x01
typedef struct _rrc_rrm_meas_result_t
{
    rrc_bitmask_t                 bitmask;
    /*^ BITMASK ^*/

    rrc_rrm_meas_cell_results_t   meas_cell_results;
    /*^ M, 0, N, 0, 0 ^*/

    rrc_rrm_meas_rs_idx_results_t meas_rs_idx_results;
    /*^ O, RRC_RRM_MEAS_RESULT_RS_IDX_RESULT_PRESENT, N, 0, 0 ^*/
}rrc_rrm_meas_result_t;

typedef struct _meas_result_nr_t
{
    rrc_bitmask_t               bitmask;
    /*^ BITMASK ^*/
#define RRC_RRM_MEAS_RESULT_NR_PHY_CELL_ID_PRESENT            0x01

    rrc_rrm_meas_result_t      rrc_rrm_meas_result;
    /*^ M, 0, N, 0, 0 ^*/

    U16                        phy_cell_id;
    /*^ O, RRC_RRM_MEAS_RESULT_NR_PHY_CELL_ID_PRESENT, H, 0, 1007 ^*/

}meas_result_nr_t;

typedef struct _meas_result_list_nr_t
{
    U8                count;
    /*^ M, 0, H, 0, MAX_NR_CELL_REPORT ^*/
 
    meas_result_nr_t  meas_result_nr[MAX_NR_CELL_REPORT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}meas_result_list_nr_t;

typedef struct _rrc_rrm_meas_result2_nr_t
{
    rrc_bitmask_t          bitmask;
    /*^ BITMASK ^*/

#define RRC_RRM_MEAS_RESULT2_NR_SSB_FREQ_PRESENT                  0x01
#define RRC_RRM_MEAS_RESULT2_NR_REF_FREQ_CSI_RS_PRESENT           0x02
#define RRC_RRM_MEAS_RESULT2_NR_MEAS_RES_SERVING_PRESENT          0x04
#define RRC_RRM_MEAS_RESULT2_NR_MEAS_RES_NEIGH_PRESENT            0x08

    U32                    ssb_frequency;
    /*^ O, RRC_RRM_MEAS_RESULT2_NR_SSB_FREQ_PRESENT, H, 0, NR_MAX_EARFCN ^*/

    U32                    ref_freq_csi_rs;
    /*^ O, RRC_RRM_MEAS_RESULT2_NR_REF_FREQ_CSI_RS_PRESENT, H, 0, NR_MAX_EARFCN ^*/

    meas_result_nr_t       meas_result_serving_cell;
    /*^ O, RRC_RRM_MEAS_RESULT2_NR_MEAS_RES_SERVING_PRESENT, N, 0, 0 ^*/

    meas_result_list_nr_t  meas_result_neigh_cell_list_nr;
    /*^ O, RRC_RRM_MEAS_RESULT2_NR_MEAS_RES_NEIGH_PRESENT, N, 0, 0 ^*/
}rrc_rrm_meas_result2_nr_t;

typedef struct _rrc_rrm_meas_result_list2_nr_t
{
    U8                        count;
    /*^ M, 0, H, 0, NR_MAX_FREQ ^*/ 

    rrc_rrm_meas_result2_nr_t rrc_rrm_meas_result2_nr[NR_MAX_FREQ];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrc_rrm_meas_result_list2_nr_t;
#define NR_CG_CONFIG_INFO_MN_CANDID_CELL_INFO_PRESENT   0x01
/*#define NR_CG_CONFIG_INFO_SN_CANDID_CELL_INFO_PRESENT   0x02
#define NR_CG_CONFIG_INFO_MEAS_RESULT_CELL_SFTD_PRESENT 0x04
#define NR_CG_CONFIG_INFO_SCG_FAILURE_INFO_PRESENT      0x08*/
#define NR_CG_CONFIG_INFO_CONFIG_RESTRICT_INFO_PRESENT  0x02
/* OPTION3X Changes Start */
#define NR_CG_CONFIG_INFO_MCG_RB_CONFIG_PRESENT         0x04
/* OPTION3X Changes End */
#define NR_CG_CONFIG_INFO_DRX_INFO_MCG_PRESENT          0x08
typedef struct _cg_confg_info_t
{
    rrc_bitmask_t                      bitmask;
    /*^ BITMASK ^*/

    rrc_rrm_meas_result_list2_nr_t     candidate_cell_info_list_mn;
    /*^ O, NR_CG_CONFIG_INFO_MN_CANDID_CELL_INFO_PRESENT, N, 0, 0 ^*/

    config_restrict_info_t             config_restrict_info_scg;
    /*^ O, NR_CG_CONFIG_INFO_CONFIG_RESTRICT_INFO_PRESENT ^*/ 

/* OPTION3X Changes Start */
    radio_bearer_config_t              mcg_rb_config;
    /*^ O, NR_CG_CONFIG_INFO_MCG_RB_CONFIG_PRESENT ^*/
/* OPTION3X Changes End */

    rrc_drx_config_t                   drx_config_mcg;
    /*^ O, NR_CG_CONFIG_INFO_DRX_INFO_MCG_PRESENT ^*/
}cg_config_info_t;

#define RRC_SGNB_ADD_PARAM_GNB_ID_PRESENT    0x01
typedef struct _rrc_rrm_sgnb_add_param_t
{
    rrc_bitmask_t                   bitmask;
    /*^ BITMASK ^*/

    rrm_ue_agg_max_bit_rate_t       ue_agg_max_bit_rate_sgnb;
    /*^ M, 0, N, 0, 0 ^*/

    cg_config_info_t                cg_config_info;
    /*^ M, 0, N, 0, 0 ^*/

    x2_gb_gnb_id_t                  gnb_id;
    /*^ O, RRC_SGNB_ADD_PARAM_GNB_ID_PRESENT, N, 0, 0 ^*/

}rrc_rrm_sgnb_add_param_t;
/*NR_DC Code Change Stop*/

typedef struct _rrc_rrm_erb_setup_resp_t
{
    rrc_bitmask_t             bitmask;    /*^ BITMASK ^*/
#define RRM_ERB_SETUP_RESP_API_ADM_RADIO_RES_PRESENT    0x01
#define RRM_ERB_SETUP_RESP_API_FAIL_CAUSE_PRESENT       0x02
#define RRM_ERB_SETUP_RESP_API_ERAB_TO_BE_RELEASED_ITEM_LIST_PRESENT  0x04
/*SPR_19066_START*/
#define RRM_ERB_SETUP_RESP_API_UE_INACTIVE_TIME_PRESENT                 0x08
/*SPR_19066_END*/
/* eMTC changes start */
#define  RRM_ERB_SETUP_RESP_API_MPDCCH_CONFIG_INFO_PRESENT              0x10
/* eMTC changes stop */
/*NR_DC Code Change Start*/
#define RRM_ERB_SETUP_RESP_API_SGNB_ADD_PARAM_PRESENT   0x20
/*NR_DC Code Change Stop*/

    U16                       ue_index;
    U8                        response;
/*^ M, 0, H, 0, 2 ^*/ /* rrc_return_et */
    rrm_erab_setup_radio_res_config_t    radio_rsource_config;
/*^ O, RRM_ERB_SETUP_RESP_API_ADM_RADIO_RES_PRESENT ^*/

    rrm_cause_t   fail_cause;
/*^ O, RRM_ERB_SETUP_RESP_API_FAIL_CAUSE_PRESENT ^*/    

    /* ARP changes begin */
    rrm_erab_to_be_released_item_list_t  erab_to_be_released_item_list; 
/*^ O, RRM_ERB_SETUP_RESP_API_ERAB_TO_BE_RELEASED_ITEM_LIST_PRESENT ^*/

/*SPR_19066_START*/
    U32                                  ue_inactive_time_config;
/*^ O, RRM_ERB_SETUP_RESP_API_UE_INACTIVE_TIME_PRESENT, N, 0, 0 ^*/
/*SPR_19066_END*/

    /* ARP changes end */
    /* eMTC changes start */
    RrcRrmErabMpdcchConfigInfo_t         mdcchConfigInfo;
    /*^ O, RRM_ERB_SETUP_RESP_API_MPDCCH_CONFIG_INFO_PRESENT, N, 0, 0 ^*/
    /* eMTC changes stop */
/*NR_DC Code Change Start*/
    rrc_rrm_sgnb_add_param_t             sgnb_add_param;
/*^ O, RRM_ERB_SETUP_RESP_API_SGNB_ADD_PARAM_PRESENT, N, 0, 0 ^*/
/*NR_DC Code Change Stop*/
}rrc_rrm_erb_setup_resp_t; /*^ API, RRC_RRM_ERB_SETUP_RESP ^*/

/******************************************************************************
            RRC_RRM_ERB_SETUP_CNF
******************************************************************************/
typedef struct
{
     U8      erab_id;   /*^ M, 0, H, 0, 15 ^*/
}rrm_erab_item_t;

typedef struct
{
    U8       erab_id;        /*^ M, 0, H, 0, 15 ^*/
    U32      error_code;     /*^ M, 0, N, 0, 0 ^*/
} rrm_erab_error_info_t;


typedef struct
{

/*SPR_17664_start*/
    rrc_counter_t       erab_count; /*^ M, 0, B, 1, MAX_LC_COUNT ^*/
/*SPR_17664_end*/
    rrm_erab_item_t     erab_cnf_info[MAX_LC_COUNT];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrm_erab_cnf_list_t;

typedef struct
{
/*SPR_17664_start*/
    rrc_counter_t          erab_count; /*^ M, 0, B, 1, MAX_LC_COUNT ^*/
/*SPR_17664_end*/
    rrm_erab_error_info_t  erab_error_info[MAX_LC_COUNT];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrm_erab_error_list_t;

/* ARP changes begin */
typedef struct
{
    rrc_bitmask_t   bitmask;                 /*^ BITMASK ^*/
#define RRC_RRM_ERB_REL_CNF_ERAB_CNF_LIST_PRESENT    0x01
#define RRC_RRM_ERB_REL_CNF_ERAB_ERROR_LIST_PRESENT  0x02

    rrm_erab_cnf_list_t    erab_cnf_list;
/*^ O, RRC_RRM_ERB_REL_CNF_ERAB_CNF_LIST_PRESENT ^*/

    rrm_erab_error_list_t  erab_error_list;
/*^ O, RRC_RRM_ERB_REL_CNF_ERAB_ERROR_LIST_PRESENT ^*/

}rrm_erab_release_cnf_list_t;
/* ARP changes end */

/* mme grp id feature changes start */
/*****************************************************************************
 *          MME INFORMATION
 * **************************************************************************/
typedef struct
{
    plmn_identity_t    plmn_identity;/*^ M, 0, N, 0, 0 ^*/
    U8                 mme_group_id[MAX_MME_GRP_BYTES]; /*^ M,0,OCTET_STRING,FIXED ^*/
    U8                 mmec;   /*^ M,0,N,0,0 ^*/
}mme_sel_gummei_info_t;
/* mme grp id feature changes stop */

typedef struct
{
    rrc_bitmask_t   bitmask;                 /*^ BITMASK ^*/
#define RRC_RRM_ERB_SETUP_CNF_ERAB_CNF_LIST_PRESENT     0x01
#define RRC_RRM_ERB_SETUP_CNF_ERAB_ERROR_LIST_PRESENT   0x02
    /* ARP changes begin */
#define RRC_RRM_ERB_SETUP_CNF_ERAB_REL_CNF_LIST_PRESENT 0x04
    /* SPS Start */
#define RRC_RRM_ERB_SETUP_CNF_SPS_ERROR_CODE_PRESENT    0x08
    /* SPS Stop */
    /* mme grp id feature changes start */
#define RRC_RRM_ERB_SETUP_CNF_GUMMEI_INFO_PRESENT       0x10
    /* mme grp id feature changes stop */
    /* SPR_21459 Fix - Start */
#define RRC_RRM_ERB_SETUP_CNF_MME_ID_PRESENT            0x20
    /* SPR_21459 Fix - End */

    /* ARP changes end */
    U16                  ue_index;
    U16                  response_code; /*^ M, 0, H, 0, 2 ^*/

    rrm_erab_cnf_list_t    erab_cnf_list;
/*^ O, RRC_RRM_ERB_SETUP_CNF_ERAB_CNF_LIST_PRESENT ^*/

    rrm_erab_error_list_t  erab_error_list;
/*^ O, RRC_RRM_ERB_SETUP_CNF_ERAB_ERROR_LIST_PRESENT ^*/

    /* ARP changes begin */
    rrm_erab_release_cnf_list_t erab_rel_cnf_list;
/*^ O, RRC_RRM_ERB_SETUP_CNF_ERAB_REL_CNF_LIST_PRESENT ^*/    
    /* ARP changes end */
    /* SPS Start */
    U8           error_code;
    /*^ O, RRC_RRM_ERB_SETUP_CNF_SPS_ERROR_CODE_PRESENT, H, 0, 0 ^*/
    /* rrc_rrm_error_code_et */
    
    /* SPS Stop */
    /* mme grp id feature changes start */
    mme_sel_gummei_info_t    gummei_info;
    /*^ O, RRC_RRM_ERB_SETUP_CNF_GUMMEI_INFO_PRESENT ^*/    
    /* mme grp id feature changes stop */

    /* SPR_21459 Fix - Start */
    U8                          mme_id;
    /*^ O, RRC_RRM_ERB_SETUP_CNF_MME_ID_PRESENT, N, 0, 0 ^*/
    /* SPR_21459 Fix - End */
} rrc_rrm_erb_setup_cnf_t; /*^ API, RRC_RRM_ERB_SETUP_CNF ^*/



/****************************************************************************
            RRC_RRM_ERB_RECONFIG_REQ
 ****************************************************************************/

/* Enum for WLAN association timer values */
typedef enum
{
    WLAN_ASSOCIATION_TIMER_10S  = 0,
    WLAN_ASSOCIATION_TIMER_30S  = 1,
    WLAN_ASSOCIATION_TIMER_60S  = 2,
    WLAN_ASSOCIATION_TIMER_120S = 3,
    WLAN_ASSOCIATION_TIMER_240S = 4
}wlan_association_timer_et;

/* Enum for WLAN success report requested values */
typedef enum
{
    WLAN_SUCCESS_REPORT_REQUESTED_TRUE = 0
}wlan_success_report_requested_et;

typedef struct 
{
    rrc_bitmask_t             bitmask;    /*^ BITMASK ^*/

#define WLAN_TO_RELEASE_LIST_PRESENT            0x01
#define WLAN_TO_ADD_LIST_PRESENT                0x02
#define WLAN_ASSOCIATION_TIMER_PRESENT          0x04
#define WLAN_SUCCESS_REPORT_REQUESTED_PRESENT   0x08

    wlan_identifier_list_t    wlan_to_add_list;
/*^ O, WLAN_TO_ADD_LIST_PRESENT ^*/

    wlan_identifier_list_t    wlan_to_remove_list;
/*^ O, WLAN_TO_RELEASE_LIST_PRESENT ^*/

    U32                       association_timer;
/*^ O, WLAN_ASSOCIATION_TIMER_PRESENT, H, 0, 4 ^*/
/* wlan_association_timer_et */

    U8                        success_report_requested;
/*^ O, WLAN_SUCCESS_REPORT_REQUESTED_PRESENT, N, 0, 0 ^*/
/* wlan_success_report_requested_et */

}wlan_mobility_config_t;

typedef struct 
{
    rrc_bitmask_t            bitmask;    /*^ BITMASK ^*/

#define RRM_LWA_CONFIG_MOBILITY_CONFIG_PRESENT    0x01
#define RRM_LWA_CONFIG_WT_COUNTER_PRESENT         0x02
    
    wlan_mobility_config_t   wlan_mobility_config;
/*^ O, RRM_LWA_CONFIG_MOBILITY_CONFIG_PRESENT ^*/

    U16                      lwa_wt_counter;
/*^ O, RRM_LWA_CONFIG_WT_COUNTER_PRESENT ^*/

}rrc_lwa_config_t;

typedef struct 
{
    rrc_bitmask_t             bitmask;    /*^ BITMASK ^*/
#define RRM_LWA_CONFIG_R13_RELEASE       0x01
#define RRM_LWA_CONFIG_R13_SETUP         0x02

    rrc_lwa_config_t          lwa_config;
/*^ O, RRM_LWA_CONFIG_R13_SETUP ^*/ 

}rrc_rrm_lwa_r13_config_t;

typedef struct 
{
#define RRM_ERB_TO_BE_RECONFIGURED_LWA_CONFIG_LWA_BEARER_TYPE_PRESENT  0x01

    rrc_bitmask_t  bitmask;
    /*^ BITMASK ^*/

    U8             lwa_bearer;
    /*^ M, 0, N, 0, 0 ^*/
/* coverity 113102 */
    U8             lwa_bearer_type;
    /*^ O, RRM_ERB_TO_BE_RECONFIGURED_LWA_CONFIG_LWA_BEARER_TYPE_PRESENT, N, 0, 0 ^*/

}rrm_erab_reconfig_lwa_config_t;

typedef struct 
{
    U8                               erab_id;
    /*^ M, 0, N, 0, 0 ^*/

    U8                               lc_id;
    /*^ M, 0, N, 0, 0 ^*/

    rrm_erab_reconfig_lwa_config_t   lwa_config;
    /*^ M, 0, N, 0, 0 ^*/

}rrm_erab_to_be_reconfigured_item_t;

typedef struct 
{
    U16   count;
    /*^ M, 0, B, 1, MAX_ERAB_COUNT ^*/

    rrm_erab_to_be_reconfigured_item_t   erab_to_be_reconfigured[MAX_ERAB_COUNT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

}rrm_erab_to_be_reconfigured_item_list_t;

typedef struct
{
#define RRC_RRM_ERB_RECONFIG_REQ_UE_AMBR_PRESENT        0x01
#define RRC_RRM_ERB_RECONFIG_REQ_MEAS_CONFIG_PRESENT    0x02
#define RRC_RRM_ERB_RECONFIG_REQ_LWA_CONFIG_PRESENT     0x04

    rrc_bitmask_t                            bitmask;     
    /*^ BITMASK ^*/

    U16                                      ue_index;
    /*^ M, 0, N, 0, 0 ^*/

    rrm_ue_agg_max_bit_rate_t                ue_agg_max_bit_rate;
    /*^ O, RRC_RRM_ERB_RECONFIG_REQ_UE_AMBR_PRESENT  ^*/

    rrm_erab_to_be_reconfigured_item_list_t  erab_to_be_reconfigured_item_list;
    /*^ M, 0, N, 0, 0 ^*/

    rrm_meas_config_t                        meas_config;
    /*^ O, RRC_RRM_ERB_RECONFIG_REQ_MEAS_CONFIG_PRESENT ^*/

    rrc_rrm_lwa_r13_config_t                 lwa_config_r13;
    /*^ O, RRC_RRM_ERB_RECONFIG_REQ_LWA_CONFIG_PRESENT ^*/

}rrc_rrm_erab_reconfig_req_t;    /*^ API, RRC_RRM_ERB_RECONFIG_REQ ^*/


/******************************************************************************
            RRC_RRM_ERB_RECONFIG_RESP
 ******************************************************************************/

typedef struct
{
#define RRC_RRM_ERB_RECONFIG_RESP_CONFIRM_LIST_PRESENT        0x01
#define RRC_RRM_ERB_RECONFIG_RESP_ERROR_LIST_PRESENT          0x02

    rrc_bitmask_t          bitmask;   
    /*^ BITMASK ^*/

    U16                    ue_index;
    /*^ M, 0, N, 0, 0 ^*/

    U16                    response;
    /*^ M, 0, H, 0, 2 ^*/

    rrm_erab_cnf_list_t    erab_cnf_list;
    /*^ O, RRC_RRM_ERB_RECONFIG_RESP_CONFIRM_LIST_PRESENT ^*/

    rrm_erab_error_list_t  erab_error_list;
    /*^ O, RRC_RRM_ERB_RECONFIG_RESP_ERROR_LIST_PRESENT ^*/
    /* rrm_erab_reconfig_fail_cause_et */

}rrc_rrm_erb_reconfig_resp_t; /*^ API, RRC_RRM_ERB_RECONFIG_RESP ^*/



/******************************************************************************
            RRC_RRM_ERB_MODIFY_REQ
******************************************************************************/
typedef struct
{
#define RRC_RRM_ERB_MODIFY_REQ_UE_AGG_MAX_BIT_RATE_PRESENT      0x01

  rrc_bitmask_t             bitmask;      /*^ BITMASK ^*/

  U16                               ue_index;
  rrm_ue_agg_max_bit_rate_t         ue_agg_max_bit_rate;
/*^ O, RRC_RRM_ERB_MODIFY_REQ_UE_AGG_MAX_BIT_RATE_PRESENT, N, 0, 0 ^*/

  rrm_erab_to_be_setup_item_list_t  erab_to_be_modified_item_list;
/*^ M, 0, N, 0, 0 ^*/

}rrc_rrm_erab_modify_req_t; /*^ API, RRC_RRM_ERB_MODIFY_REQ ^*/

/******************************************************************************
            RRC_RRM_ERB_MODIFY_RESP
******************************************************************************/
typedef struct
{
    rrc_bitmask_t              bitmask;     /*^ BITMASK ^*/
#define RRM_ERAB_MODIFY_RADIO_RES_CONFIG_DRB_TO_MODIFY_LIST_PRESENT         0x01
#define RRM_ERAB_MODIFY_RADIO_RES_CONFIG_MAC_CONFIG_PRESENT                 0x02
#define RRM_ERAB_MODIFY_RADIO_RES_CONFIG_SPS_CONFIG_PRESENT                 0x04
#define RRM_ERAB_MODIFY_RADIO_RES_CONFIG_PHY_CONFIG_DED_PRESENT             0x08
#define RRM_ERAB_MODIFY_RADIO_RES_CONFIG_MEAS_CONFIG_PRESENT                0x10
#define RRM_ERAB_MODIFY_RADIO_RES_CONFIG_DRB_FAILED_TO_MODIFY_LIST_PRESENT  0x20
#define RRM_ERAB_MODIFY_RADIO_RES_CONFIG_RLF_TIMERS_AND_CONSTANTS_PRESENT   0x40
#define RRM_ERAB_MODIFY_RADIO_RES_CONFIG_PROXIMITY_PRESENT                  0x80

    rrm_drb_to_add_info_list_t  drb_to_modify_list;
/*^ O, RRM_ERAB_MODIFY_RADIO_RES_CONFIG_DRB_TO_MODIFY_LIST_PRESENT ^*/

    rrm_mac_config_t            mac_config;
/*^ O, RRM_ERAB_MODIFY_RADIO_RES_CONFIG_MAC_CONFIG_PRESENT ^*/

    rrc_sps_config_t            sps_config;
/*^ O, RRM_ERAB_MODIFY_RADIO_RES_CONFIG_SPS_CONFIG_PRESENT ^*/

    rrm_meas_config_t           meas_config;
/*^ O, RRM_ERAB_SETUP_RADIO_RES_CONFIG_MEAS_CONFIG_PRESENT ^*/

    rrm_drb_failed_to_add_info_list_t  drb_failed_to_modify_list;
/*^ O, RRM_ERAB_MODIFY_RADIO_RES_CONFIG_DRB_FAILED_TO_MODIFY_LIST_PRESENT ^*/

  /* cr_220 structure*/

    rlf_timers_and_constants_r9_t rlf_timers_and_constants_r9; 
/*^ O, RRM_ERAB_MODIFY_RADIO_RES_CONFIG_RLF_TIMERS_AND_CONSTANTS_PRESENT ^*/

    rrm_report_proximity_config_r9_t   proximity_config;
/*^ O, RRM_ERAB_MODIFY_RADIO_RES_CONFIG_PROXIMITY_PRESENT ^*/ 

} rrm_erab_modify_radio_res_config_t;


typedef struct
{
    rrc_bitmask_t             bitmask;    /*^ BITMASK ^*/
#define RRM_ERB_MODIFY_RESP_API_ADM_RADIO_RES_PRESENT   0x01
    /*SPR_19066_START*/
#define RRM_ERB_MODIFY_RESP_API_UE_INACTIVE_TIME_PRESENT    0x02
    /*SPR_19066_END*/
    /*SPR 22036 Fix Start*/
#define RRM_ERB_MODIFY_RESP_API_ERAB_TO_BE_RELEASED_ITEM_LIST_PRESENT 0x04
    /*SPR 22036 Fix Stop*/

    U16                       ue_index;
    U16                       response; /*^ M, 0, H, 0, 2 ^*/

    rrm_erab_modify_radio_res_config_t    radio_rsource_config;
    /*^ O, RRM_ERB_MODIFY_RESP_API_ADM_RADIO_RES_PRESENT ^*/

    /*SPR_19066_START*/
    U32                       ue_inactive_time_config;
    /*^ O, RRM_ERB_MODIFY_RESP_API_UE_INACTIVE_TIME_PRESENT, N, 0, 0 ^*/
    /*SPR_19066_END*/
    /*SPR 22036 Fix Start*/
    rrm_erab_to_be_released_item_list_t  erab_to_be_released_item_list; 
    /*^ O, RRM_ERB_MODIFY_RESP_API_ERAB_TO_BE_RELEASED_ITEM_LIST_PRESENT ^*/
    /*SPR 22036 Fix Stop*/


}rrc_rrm_erab_modify_resp_t; /*^ API, RRC_RRM_ERB_MODIFY_RESP ^*/

/******************************************************************************
            RRC_RRM_ERB_MODIFY_CNF
******************************************************************************/

typedef struct
{
    rrc_bitmask_t   bitmask;                 /*^ BITMASK ^*/
#define RRM_ERB_MODIFY_CNF_CONFIRM_LIST_PRESENT        0x01
#define RRM_ERB_MODIFY_CNF_ERROR_LIST_PRESENT          0x02
/* SPS Start */
#define RRC_RRM_ERB_MODIFY_CNF_SPS_ERROR_CODE_PRESENT  0x04
/* SPS Stop */

    U16                  ue_index;
    U16                  response;/*^ M, 0, H, 0, 2 ^*/

    rrm_erab_cnf_list_t    erab_cnf_list;
/*^ O, RRM_ERB_MODIFY_CNF_CONFIRM_LIST_PRESENT ^*/
    rrm_erab_error_list_t  erab_error_list;
/*^ O, RRM_ERB_MODIFY_CNF_ERROR_LIST_PRESENT ^*/

    /* SPS Start */
    U8           error_code;
    /*^ O, RRC_RRM_ERB_MODIFY_CNF_SPS_ERROR_CODE_PRESENT, H, 0, 0 ^*/
    /* rrc_rrm_error_code_et */
    /* SPS Stop */
} rrc_rrm_erb_modify_cnf_t; /*^ API, RRC_RRM_ERB_MODIFY_CNF ^*/

/******************************************************************************
            RRC_RRM_UE_CONNECTION_RELEASE_IND
******************************************************************************/



/* For t320 ENUMERATED {min5, min10, min20, min30, min60, min120, min180}*/

typedef enum
{
  RRM_RRC_MIN5,
  RRM_RRC_MIN10, 
  RRM_RRC_MIN20, 
  RRM_RRC_MIN30, 
  RRM_RRC_MIN60, 
  RRM_RRC_MIN120, 
  RRM_RRC_MIN180,
  RRM_RRC_SPAREL_4
} rrc_rrm_idle_mode_mobility_control_info_t320_root_et;


/* For band_indicator ENUMERATED {dcs1800, pcs1900}*/

typedef enum
{
  RRM_RRC_DCS1800, 
  RRM_RRC_PCS1900
}rrc_rrm_carrier_freq_geran_band_indicator_et;




/* Integration Fix start*/
typedef struct 
{
    U8  count;

/*SPR_17664_start*/
    /*^ M, 0, B, 1, MAX_FREQ_UTRA_TDD_R10 ^*/
/*SPR_17664_end*/
    U16  utra_tdd_r10[MAX_FREQ_UTRA_TDD_R10];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}redirected_carrier_info_utra_tdd_r10_t;
/* Integration Fix stop*/

typedef struct
{
    rrc_bitmask_t             bitmask;    /*^ BITMASK ^*/
/*Choice - set only one bit*/
#define RRM_REDIRECTED_CARRIER_INFO_EUTRA_PRESENT           0x01
#define RRM_REDIRECTED_CARRIER_INFO_GERAN_PRESENT           0x02
#define RRM_REDIRECTED_CARRIER_INFO_UTRA_FDD_PRESENT        0x04
#define RRM_REDIRECTED_CARRIER_INFO_UTRA_TDD_PRESENT        0x08
#define RRM_REDIRECTED_CARRIER_INFO_CDMA2000_HRPD_PRESENT   0x10
#define RRM_REDIRECTED_CARRIER_INFO_CDMA2000_1XRTT_PRESENT  0x20
/*CR 713 changes start*/
#define RRM_REDIRECTED_CARRIER_INFO_UTRA_TDD_R10_PRESENT    0x40
/*CR 713 changes stop*/

    U32 eutra;
/*^ O, RRM_REDIRECTED_CARRIER_INFO_EUTRA_PRESENT ^*/ /*0..MAX_EARFCN*/

    carrier_freqs_geran_t geran;
/*^ O, RRM_REDIRECTED_CARRIER_INFO_GERAN_PRESENT ^*/

    U16 utra_fdd;
/*^ O, RRM_REDIRECTED_CARRIER_INFO_UTRA_FDD_PRESENT, H, 0, 16383 ^*/

    U16 utra_tdd;
/*^ O, RRM_REDIRECTED_CARRIER_INFO_UTRA_TDD_PRESENT, H, 0, 16383 ^*/

    carrier_freq_cdma2000_t cdma2000_hrpd;
/*^ O, RRM_REDIRECTED_CARRIER_INFO_CDMA2000_HRPD_PRESENT ^*/

    carrier_freq_cdma2000_t cdma2000_1xrtt;
/*^ O, RRM_REDIRECTED_CARRIER_INFO_CDMA2000_1XRTT_PRESENT ^*/
/*CR 713 changes start*/
/*Integration Fix start*/
    redirected_carrier_info_utra_tdd_r10_t redirected_carrier_info_utra_tdd_r10;
/*^ O, RRM_REDIRECTED_CARRIER_INFO_UTRA_TDD_R10_PRESENT ^*/    
/*Integration Fix stop*/
/*CR 713 changes stop*/
} rrm_redirected_carrier_info_t;

typedef struct
{
    U32 carrier_freq; /*0..MAX_EARFCN*/
    U8 cell_reselection_priority; /*^ M, 0, H, 0, 7 ^*/
} rrm_freq_priority_eutra_t;

typedef struct
{
/*SPR_17664_start*/
    U8           count;                 /*^ M, 0, B, 1, MAX_FREQ ^*/
/*SPR_17664_end*/
    rrm_freq_priority_eutra_t freq_priority_eutra[MAX_FREQ];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrm_freq_priority_list_eutra_t;

typedef struct
{
    carrier_freqs_geran_t carrier_freq;
    U8 cell_reselection_priority; /*^ M, 0, H, 0, 7 ^*/
} rrm_freqs_priority_geran_t;

typedef struct
{
/*SPR_17664_start*/
    U8           count;                 /*^ M, 0, B, 1, MAX_GNFG ^*/
/*SPR_17664_end*/
    rrm_freqs_priority_geran_t freqs_priority_geran[MAX_GNFG];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrm_freqs_priority_list_geran_t;

typedef struct
{
    U16 carrier_freq; /*^ M, 0, H, 0, 16383 ^*/
    U8 cell_reselection_priority; /*^ M, 0, H, 0, 7 ^*/
} rrm_freq_priority_utra_fdd_t;

typedef struct
{
/*SPR_17664_start*/
    U8           count;                 /*^ M, 0, B, 1, MAX_UTRA_FDD_CARRIER ^*/
/*SPR_17664_end*/
    rrm_freq_priority_utra_fdd_t freq_priority_utra_fdd[MAX_UTRA_FDD_CARRIER];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrm_freq_priority_list_utra_fdd_t;

typedef struct
{
    U16 carrier_freq; /*^ M, 0, H, 0, 16383 ^*/
    U8 cell_reselection_priority; /*^ M, 0, H, 0, 7 ^*/
} rrm_freq_priority_utra_tdd_t;

typedef struct
{
/*SPR_17664_start*/
    U8           count;                 /*^ M, 0, B, 1, MAX_UTRA_TDD_CARRIER ^*/
/*SPR_17664_end */
    rrm_freq_priority_utra_tdd_t freq_priority_utra_tdd[MAX_UTRA_TDD_CARRIER];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrm_freq_priority_list_utra_tdd_t;

typedef struct
{
    U8 band_class; /*^ M, 0, H, 0, 31 ^*/ /*rrm_bandclass_cdma2000_et*/
    U8 cell_reselection_priority; /*^ M, 0, H, 0, 7 ^*/
} rrm_band_class_priority_hrpd_t;

typedef struct
{
/*SPR_17664_start*/
    U8           count;                 /*^ M, 0, B, 1, MAX_CDMA_BAND_CLASS ^*/
/*SPR_17664_end*/
    rrm_band_class_priority_hrpd_t band_class_priority[MAX_CDMA_BAND_CLASS];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrm_band_class_priority_list_hrpd_t;

typedef struct
{
    U8 band_class; /*^ M, 0, H, 0, 31 ^*/ /*rrm_bandclass_cdma2000_et*/
    U8 cell_reselection_priority; /*^ M, 0, H, 0, 7 ^*/
} rrm_band_class_priority_1xrtt_t; /*same as rrm_band_class_priority_hrpd_t*/

typedef struct
{
/*SPR_17664_start*/
    U8           count;                 /*^ M, 0, B, 1, MAX_CDMA_BAND_CLASS ^*/
/*SPR_17664_end*/
    rrm_band_class_priority_1xrtt_t band_class_priority[MAX_CDMA_BAND_CLASS];
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrm_band_class_priority_list_1xrtt_t;

typedef struct
{
    rrc_bitmask_t             bitmask;    /*^ BITMASK ^*/
#define RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_FREQ_PRIORITY_LIST_EUTRA_PRESENT 0x01
#define RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_FREQ_PRIORITY_LIST_GERAN_PRESENT 0x02
#define RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_FREQ_PRIORITY_LIST_UTRA_FDD_PRESENT 0x04
#define RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_FREQ_PRIORITY_LIST_UTRA_TDD_PRESENT 0x08
#define RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_BAND_CLASS_PRIORITY_LIST_HRPD_PRESENT 0x10
#define RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_BAND_CLASS_PRIORITY_LIST_1XRTT_PRESENT 0x20
#define RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_T320_PRESENT 0x40

    rrm_freq_priority_list_eutra_t freq_priority_list_eutra;
/*^ O, RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_FREQ_PRIORITY_LIST_EUTRA_PRESENT ^*/

    rrm_freqs_priority_list_geran_t freq_priority_list_geran;
/*^ O, RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_FREQ_PRIORITY_LIST_GERAN_PRESENT ^*/

    rrm_freq_priority_list_utra_fdd_t freq_priority_list_utra_fdd;
/*^ O, RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_FREQ_PRIORITY_LIST_UTRA_FDD_PRESENT ^*/

    rrm_freq_priority_list_utra_tdd_t freq_priority_list_utra_tdd;
/*^ O, RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_FREQ_PRIORITY_LIST_UTRA_TDD_PRESENT ^*/

    rrm_band_class_priority_list_hrpd_t band_class_priority_list_hrpd;
/*^ O, RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_BAND_CLASS_PRIORITY_LIST_HRPD_PRESENT ^*/

    rrm_band_class_priority_list_1xrtt_t band_class_priority_list_1xrtt;
/*^ O, RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_BAND_CLASS_PRIORITY_LIST_1XRTT_PRESENT ^*/

    U8 t320;
/*^ O, RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_T320_PRESENT, H, 0, 7 ^*/
/* rrc_rrm_idle_mode_mobility_control_info_t320_root_et */

} rrm_idle_mode_mobility_control_info_t;



typedef struct
{
    U8 dummy[MAX_DUMMY_SIZE];
}utra_bcch_container_r9_expanded_si_info_t;    

typedef struct
{
/* SPR-18445 start */
    U16 utra_bcch_container_length;/*^ M, 0, H, 0, UTRA_BCCH_CONAINER_SIZE ^*/
/* SPR-18445 stop */

    U8 utra_bcch_container[UTRA_BCCH_CONAINER_SIZE];
   /*^ M, 0, OCTET_STRING, VARIABLE, N, 0, 0 ^*/

}utra_bcch_container_r9_encoded_si_info_t;

typedef struct
{
    rrc_bitmask_t             bitmask;    /*^ BITMASK ^*/
#define RRC_RRM_UTRA_TDD_BCCH_CONTAINER_ENCODED_SI_PRESENT 0x01
#define RRC_RRM_UTRA_TDD_BCCH_CONTAINTER_EXPANDED_SI_INFO_PRESENT 0x02    

    U8  phys_cell_id_r9;/*^ M, 0, H, 0, 127 ^*/

    utra_bcch_container_r9_encoded_si_info_t      
                    utra_tdd_bcch_cont_r9_encoded_si_info; 
/*^ O, RRC_RRM_UTRA_TDD_BCCH_CONTAINER_ENCODED_SI_PRESENT ^*/

    utra_bcch_container_r9_expanded_si_info_t 
                    utra_tdd_bcch_cont_expanded_si_info;
/*^ O, RRC_RRM_UTRA_TDD_BCCH_CONTAINTER_EXPANDED_SI_INFO_PRESENT ^*/
}cell_info_utra_tdd_r9_t;

typedef struct
{
/*SPR_17664_start*/
    U8 count;/*^ M, 0, B, 1, MAX_CELL_INFO_UTRA_R9 ^*/
/*SPR_17664_end*/
    cell_info_utra_tdd_r9_t  cell_info_utra_tdd_r9[MAX_CELL_INFO_UTRA_R9]; 
   /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}cell_info_list_utra_tdd_r9_t;



/*CR 713 change start*/
typedef struct
{
/* SPR-18445 start */
    U16 utra_bcch_container_length;/*^ M, 0, H, 0, UTRA_BCCH_CONAINER_SIZE ^*/
/* SPR-18445 stop */

    U8 utra_bcch_container[UTRA_BCCH_CONAINER_SIZE];
    /*^ M, 0, OCTET_STRING, VARIABLE, N, 0, 0 ^*/
}utra_bcch_container_r10_encoded_si_info_t;

typedef struct 
{
    rrc_bitmask_t             bitmask;    /*^ BITMASK ^*/
    U8      phys_cell_id_r10;
    /*^ M, 0, H, 0, 127 ^*/
    U16     carrier_freq_r10;
    /*^ M, 0, H, 0, 16383 ^*/
    utra_bcch_container_r10_encoded_si_info_t
            utra_BCCH_Container_r10;
    /*^ M, 0, N, 0, 0 ^*/
}cell_info_utra_tdd_r10_t;

typedef struct 
{
    U8 count;
/*SPR_17664_start*/
    /*^ M, 0, B, 1, MAX_CELL_INFO_UTRA_R9 ^*/
/*SPR_17664_end*/
    cell_info_utra_tdd_r10_t    cell_info_utra_tdd_r10[MAX_CELL_INFO_UTRA_R9];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}cell_info_list_utra_tdd_r10_t;
/*CR 713 change stop*/

typedef struct
{
    rrc_bitmask_t             bitmask;    /*^ BITMASK ^*/
#define RRC_RRM_UTRA_FDD_BCCH_CONTAINER_ENCODED_SI_PRESENT 0x01
#define RRC_RRM_UTRA_FDD_BCCH_CONTAINTER_EXPANDED_SI_INFO_PRESENT 0x02    
    
    U16  phys_cell_id_r9;/*^ M, 0, H, 0, 511 ^*/

    utra_bcch_container_r9_encoded_si_info_t      
                    utra_fdd_bcch_cont_r9_encoded_si_info; 
/*^ O, RRC_RRM_UTRA_FDD_BCCH_CONTAINER_ENCODED_SI_PRESENT ^*/

    utra_bcch_container_r9_expanded_si_info_t 
                    utra_fdd_bcch_cont_expanded_si_info;
/*^ O, RRC_RRM_UTRA_FDD_BCCH_CONTAINTER_EXPANDED_SI_INFO_PRESENT ^*/
}cell_info_utra_fdd_r9_t;

typedef struct
{
/*SPR_17664_start*/
    U8 count;/*^ M, 0, B, 1, MAX_CELL_INFO_UTRA_R9 ^*/
/*SPR_17664_end*/
    cell_info_utra_fdd_r9_t  cell_info_utra_fdd_r9[MAX_CELL_INFO_UTRA_R9]; 
   /*^ M, 0, OCTET_STRING, VARIABLE ^*/

}cell_info_list_utra_fdd_r9_t;

typedef struct
{
/* SPR-18445 start */
  U8 length;/*^ M, 0, B, 1, GERAN_SI_DATA_SIZE ^*/
/* SPR-18445 stop */

  U8 data[GERAN_SI_DATA_SIZE];
  /*^ M, 0, OCTET_STRING, VARIABLE, N, 0, 0 ^*/
}sys_info_container_encoded_si_info_t;

typedef struct
{
    U8 dummy[MAX_DUMMY_SIZE];
}sys_info_container_expanded_si_info_t;

typedef struct
{
    rrc_bitmask_t             bitmask;    /*^ BITMASK ^*/
#define RRC_RRM_SYS_INFO_CONTAINER_ENCODED_PRESENT 0x01
#define RRC_RRM_SYS_INFO_CONTAINTER_EXPANDED_PRESENT 0x02    

  sys_info_container_encoded_si_info_t sys_info_container_encoded_si_info;
/*^ O, RRC_RRM_SYS_INFO_CONTAINER_ENCODED_PRESENT ^*/

  sys_info_container_expanded_si_info_t sys_info_container_expanded_si_info;
/*^ O, RRC_RRM_SYS_INFO_CONTAINTER_EXPANDED_PRESENT ^*/

}system_info_list_geran_element_t;

typedef struct
{
/*SPR_17664_start*/
  U8 count;/*^ M, 0, B, 1, MAX_GERAN_SI ^*/
/*SPR_17664_end*/
  system_info_list_geran_element_t system_info_geran_elem[MAX_GERAN_SI];
   /*^ M, 0, OCTET_STRING, VARIABLE ^*/

}system_info_list_geran_t;

typedef struct
{
    phys_cell_id_geran_t  phys_cell_id_r9;
    carrier_freq_geran_t  carrier_freq_r9;

    system_info_list_geran_t  system_information_r9;
  
}cell_info_geran_r9_t;

typedef struct
{
/*SPR_17664_start*/
    U8 count;/*^ M, 0, B, 1, MAX_CELL_INFO_GERAN_R9 ^*/
/*SPR_17664_end*/
    cell_info_geran_r9_t  cell_info_geran_r9[MAX_CELL_INFO_GERAN_R9]; 
   /*^ M, 0, OCTET_STRING, VARIABLE ^*/

}cell_info_list_geran_r9_t;

/*CHOICE*/
typedef struct
{
    rrc_bitmask_t             bitmask;    /*^ BITMASK ^*/
#define RRC_RRM_CELL_INFO_LIST_GERAN_R9_PRESENT 0x01
#define RRC_RRM_CELL_INFO_LIST_UTRA_FDD_R9_PRESENT 0x02
#define RRC_RRM_CELL_INFO_LIST_UTRA_TDD_R9_PRESENT 0x04
/*CR 713 change start*/
#define RRC_RRM_CELL_INFO_LIST_UTRA_TDD_R10_PRESENT 0x08
/*CR 713 change stop*/    

    cell_info_list_geran_r9_t geran_r9;
/*^ O, RRC_RRM_CELL_INFO_LIST_GERAN_R9_PRESENT ^*/
  
    cell_info_list_utra_fdd_r9_t utra_fdd_r9;
/*^ O, RRC_RRM_CELL_INFO_LIST_UTRA_FDD_R9_PRESENT ^*/

    cell_info_list_utra_tdd_r9_t utra_tdd_r9;
/*^ O, RRC_RRM_CELL_INFO_LIST_UTRA_TDD_R9_PRESENT ^*/
/*CR 713 change start*/
    cell_info_list_utra_tdd_r10_t utra_tdd_r10;
/*^ O, RRC_RRM_CELL_INFO_LIST_UTRA_TDD_R10_PRESENT ^*/    
/*CR 713 change stop*/

}cell_info_list_r9_t;


typedef struct
{
     rrc_bitmask_t             bitmask;    /*^ BITMASK ^*/
#define RRC_RRM_EXTENDED_WAIT_TIME_PRESENT 0x01

    U16  extended_wait_time_r10;
/*^ O, RRC_RRM_EXTENDED_WAIT_TIME_PRESENT, B, 1, 1800 ^*/
    
}rrm_rrc_connection_release_v1020_ies_t;

typedef struct
{
     rrc_bitmask_t             bitmask;    /*^ BITMASK ^*/
#define RRC_RRM_CELL_INFO_LIST_R9_PRESENT           0x01
#define RRC_RRM_CONNECTION_RELEASE_V1020_PRESENT    0x02

    cell_info_list_r9_t  cell_info_list_r9;
/*^ O, RRC_RRM_CELL_INFO_LIST_R9_PRESENT ^*/
    
    rrm_rrc_connection_release_v1020_ies_t  rrc_connection_release_v1020_ies;
/*^ O, RRC_RRM_CONNECTION_RELEASE_V1020_PRESENT ^*/
}rrm_rrc_connection_release_v920_ies_t;

typedef struct
{
     rrc_bitmask_t             bitmask;    /*^ BITMASK ^*/
#define RRC_RRM_UE_CONNECTION_RELEASE_IND_RRC_CONN_REL_V920_IES_PRESENT 0x01

    rrm_rrc_connection_release_v920_ies_t  rrc_connection_release_v920_ies;
/*^ O, RRC_RRM_UE_CONNECTION_RELEASE_IND_RRC_CONN_REL_V920_IES_PRESENT ^*/

}rrm_rrc_connection_release_v890_ies_t;


typedef struct
{
    rrc_bitmask_t             bitmask;    /*^ BITMASK ^*/
#define RRC_RRM_UE_CONNECTION_RELEASE_IND_REDIRECTED_CARRIER_INFO_PRESENT    0x01
#define RRC_RRM_UE_CONNECTION_RELEASE_IND_IDLE_MODE_MOBILITY_CONTROL_PRESENT 0x02
#define RRC_RRM_UE_CONNECTION_RELEASE_IND_RRC_CONN_REL_V890_IES_PRESENT      0x04


    U16                  ue_index;
    rrm_cause_t          release_cause;
    rrm_redirected_carrier_info_t redirected_carrier_info;
/*^ O, RRC_RRM_UE_CONNECTION_RELEASE_IND_REDIRECTED_CARRIER_INFO_PRESENT ^*/

    rrm_idle_mode_mobility_control_info_t idle_mode_mobility_control;
/*^ O, RRC_RRM_UE_CONNECTION_RELEASE_IND_IDLE_MODE_MOBILITY_CONTROL_PRESENT ^*/

    rrm_rrc_connection_release_v890_ies_t    rrc_connection_release_v890_ies;
/*^ O, RRC_RRM_UE_CONNECTION_RELEASE_IND_RRC_CONN_REL_V890_IES_PRESENT ^*/

} rrc_rrm_ue_connection_release_ind_t;
/*^ API, RRC_RRM_UE_CONNECTION_RELEASE_IND ^*/

/******************************************************************************
            RRC_RRM_MEASURMENT_RESULTS_IND
******************************************************************************/
typedef struct
{
  U8    rsrp_result;    /*^ M, 0, H, 0, 97 ^*/
  U8    rsrq_result;    /*^ M, 0, H, 0, 34 ^*/
}rrc_meas_result_serv_cell_t;

#define MEAS_RESULT_SSB_INDEX_R15_PRESENT    0x01
typedef struct
{
  rrc_bitmask_t              bitmask;
  /*^ BITMASK ^*/

  U8                         ssb_index_r15;
  /*^ M, 0, H, 0, MAX_RS_INDEX_R15 ^*/

  threshold_list_nr_r15_t    meas_result_ssb_index_r15;
  /*^ O, MEAS_RESULT_SSB_INDEX_R15_PRESENT ^*/
}meas_result_ssb_index_r15_t;

typedef struct
{
    U8                               count;
    /*^ M, 0, B, 1, MAX_RS_INDEX_REPORT_R15 ^*/

    meas_result_ssb_index_r15_t      meas_result_ssb_index_r15[MAX_RS_INDEX_REPORT_R15];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}meas_result_ssb_index_list_r15_t;

#define MEAS_RESULT_NR_RS_INDEX_LIST_R15_PRESENT  0x01
typedef struct
{
  rrc_bitmask_t                     bitmask;
  /*^ BITMASK ^*/

  U16                               phys_cell_id_r15;
  /*^ M, 0, H, 0, 503 ^*/

  threshold_list_nr_r15_t           meas_result_cell_r15;
  /*^ M, 0, N, 0, 0 ^*/

  meas_result_ssb_index_list_r15_t  rs_index_list_r15;
  /*^ O, MEAS_RESULT_NR_RS_INDEX_LIST_R15_PRESENT ^*/

}meas_result_ind_nr_t;

typedef struct
{
    U8                        count;
    /*^ M, 0, H, 0, MAX_CELL_REPORT ^*/

    meas_result_ind_nr_t      meas_result_nr[MAX_CELL_REPORT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

}meas_results_list_nr_r15_t;

typedef struct
{
  rrc_bitmask_t                 bitmask;                    /*^ BITMASK ^*/

#define RRM_MEAS_RESULT_LIST_EUTRA_PRESENT  0x01
#define RRM_MEAS_RESULT_LIST_UTRA_PRESENT   0x02
#define RRM_MEAS_RESULT_LIST_GERAN_PRESENT  0x04
#define RRM_MEAS_RESULTS_CDMA2000_PRESENT   0x08
#define RRM_MEAS_RESULT_LIST_NR_R15_PRESENT 0x10

  meas_result_list_eutra_t      meas_result_list_eutra;
/*^ O, RRM_MEAS_RESULT_LIST_EUTRA_PRESENT ^*/

  meas_result_list_utra_t       meas_result_list_utra;
/*^ O, RRM_MEAS_RESULT_LIST_UTRA_PRESENT ^*/

  meas_result_list_geran_t      meas_result_list_geran;
/*^ O, RRM_MEAS_RESULT_LIST_GERAN_PRESENT ^*/

  meas_results_cdma2000_t       meas_result_cdma2000;
/*^ O, RRM_MEAS_RESULTS_CDMA2000_PRESENT ^*/

  meas_results_list_nr_r15_t    meas_result_list_nr_r15;
/*^ O, RRM_MEAS_RESULT_LIST_NR_R15_PRESENT ^*/

}rrc_meas_result_neigh_cells_t;

typedef struct
{
    U16 ue_rxtx_time_diff_result; /*^ M, 0, H, 0, 4095 ^*/
    U8  current_sfn_r9[ECID_CURRENT_SFN_R9_OCTET_SIZE];
    /*^ M, 0, OCTET_STRING, FIXED, N, 0, 0 ^*/
} rrc_meas_result_ecid_r9_t;


typedef struct 
{
  rrc_bitmask_t         bitmask;                /*^ BITMASK ^*/

#define WLAN_CARRIER_INFO_PRESENT                  0x01
#define WLAN_BAND_PRESENT                          0x02
#define WLAN_AVAILABLE_ADMISSION_CAPACITY_PRESENT  0x04
#define WLAN_BACKHAUL_DL_BANDWIDTH_PRESENT         0x08
#define WLAN_BACKHAUL_UL_BANDWIDTH_PRESENT         0x10
#define WLAN_CHANNEL_UTILIZATION_PRESENT           0x12
#define WLAN_STATION_COUNT_PRESENT                 0x14
#define WLAN_CONNECTED_WLAN_PRESENT                0x18

  wlan_identifier_t     wlan_identifier;
/*^ M, 0, N, 0, 0 ^*/

  wlan_carrier_info_t   wlan_carrier_info;
/*^ O, WLAN_CARRIER_INFO_PRESENT ^*/

  U8                    wlan_band;
/*^ O, WLAN_BAND_PRESENT, H, 0, 1 ^*/

  U8                    rssi; 
/*^ M, 0, H, 0, 141 ^*/

  U16                   available_admission_capacity;
/*^ O, WLAN_AVAILABLE_ADMISSION_CAPACITY_PRESENT, H, 0, 31250 ^*/

  U8                    backhaul_dl_bandwidth;
/*^ O, WLAN_BACKHAUL_DL_BANDWIDTH_PRESENT, H, 0, 32 ^*/

  U8                    backhaul_ul_bandwidth;
/*^ O, WLAN_BACKHAUL_UL_BANDWIDTH_PRESENT, H, 0, 32 ^*/

  U8                    channel_utilization;
/*^ O, WLAN_CHANNEL_UTILIZATION_PRESENT, H, 0, 255 ^*/

  U16                   station_count;
/*^ O, WLAN_STATION_COUNT_PRESENT, H, 0, 65535 ^*/

  U8                    connected_wlan;
/*^ O, WLAN_CONNECTED_WLAN_PRESENT, H, 0, 1 ^*/

}rrc_meas_result_wlan_r13_t;

typedef struct 
{
  U8                       count;
/*^ M, 0, H, 0, MAX_CELL_INFO_WLAN_R13 ^*/

  rrc_meas_result_wlan_r13_t   meas_result_wlan_r13[MAX_CELL_INFO_WLAN_R13]; 
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}rrc_meas_result_wlan_list_r13_t;


typedef struct
{
    rrc_bitmask_t                   bitmask;                    /*^ BITMASK ^*/

#define RRM_MEASURMENT_RESULTS_NEIGH_CELLS_PRESENT    0x01
#define RRM_MEASURMENT_RESULTS_FOR_ECID_R9_PRESENT    0x02
#define RRM_MEASURMENT_RESULTS_SERV_FREQ_LIST_R10_PRESENT    0x04
#define RRM_MEASURMENT_RESULTS_WLAN_R13_PRESENT              0x08

    U16                             ue_index;
    U8                              meas_id;
/*^ M, 0, H, 1, 32 ^*/

    rrc_meas_result_serv_cell_t     meas_result_serv_cell;
    rrc_meas_result_neigh_cells_t   meas_result_neigh_cells;
/*^ O, RRM_MEASURMENT_RESULTS_NEIGH_CELLS_PRESENT ^*/

    rrc_meas_result_ecid_r9_t       meas_result_ecid_r9;
/*^ O, RRM_MEASURMENT_RESULTS_FOR_ECID_R9_PRESENT ^*/

    rrc_meas_result_serv_freq_list_r10_t      meas_result_serv_freq_list_r10;
/*^ O, RRM_MEASURMENT_RESULTS_SERV_FREQ_LIST_R10_PRESENT ^*/

    rrc_meas_result_wlan_list_r13_t     meas_result_wlan_list_r13;
/*^ O, RRM_MEASURMENT_RESULTS_WLAN_R13_PRESENT ^*/

} rrc_rrm_measurment_results_ind_t; /*^ API, RRC_RRM_MEASURMENT_RESULTS_IND ^*/

/*****************************************************************************
      RRC_RRM_PROXIMITY_IND
 ******************************************************************************/
typedef struct
{
  rrc_bitmask_t                 presence_bitmask;        /*^ BITMASK ^*/

#define RRM_PROXIMITY_ARFCN_VALUE_EUTRA_PRESENT   0x01
#define RRM_PROXIMITY_ARFCN_VALUE_UTRA_PRESENT    0x02
         
      U16                       carrier_freq_eutra;
/*^ O, RRM_PROXIMITY_ARFCN_VALUE_EUTRA_PRESENT ^*/
      U16                       carrier_freq_utra;
/*^ O, RRM_PROXIMITY_ARFCN_VALUE_UTRA_PRESENT ^*/
}ProximityIndication_r9_IEs_carrierFreq_r9_t;   

typedef struct
{
    rrc_bitmask_t                 presence_bitmask;        /*^ BITMASK ^*/
    U16                                           ue_index;
    U8                                            proximity_ind_type;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_proximity_ind_type_et */
    ProximityIndication_r9_IEs_carrierFreq_r9_t   carrier_Freq;
} rrc_rrm_proximity_ind_t; /*^ API, RRC_RRM_PROXIMITY_IND ^*/

/*****************************************************************************
      RRC_RRM_CELL_DEL_RESP
 ******************************************************************************/

#define CELL_DEL_RESP_API_FAIL_CAUSE_PRESENCE_FLAG        0x01
typedef struct
{
   U16               presence_bitmask;   /*^ BITMASK ^*/
   rrc_cell_index_t  cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */

    U8                response;
                /*^ M, 0, H, 0, 1 ^*/    /* rrc_return_et */

    U8                fail_cause;
    /*^ O, CELL_DEL_RESP_API_FAIL_CAUSE_PRESENCE_FLAG, H, 0, 14 ^*/
    /* rrm_fail_cause_et */

} rrm_rrc_cell_del_resp_t;/*^ API,  RRC_RRM_CELL_DELETE_RESP ^*/
/******************************************************************************
    RRC_RRM_UE_CAPABILITY_ENQUIRY_REQ
******************************************************************************/

typedef enum
{
    RRC_RAT_TYPE_EUTRA,
    RRC_RAT_TYPE_UTRA,
    RRC_RAT_TYPE_GERAN_CS,
    RRC_RAT_TYPE_GERAN_PS,
    RRC_RAT_TYPE_CDMA2000_1XRTT,
/*NR_DC Code Changes Start*/
    RRC_RAT_TYPE_NR,
    RRC_RAT_TYPE_EUTRA_NR,
/*NR_DC Code Change Stop*/
    RRC_RAT_TYPE_SPARE1
}rrc_rat_type_et;

typedef struct
{
/*SPR_17664_start*/
    U8                          rat_type_count;                 /*^ M, 0, H, 0, MAX_RAT_CAPABILITY ^*/
/*SPR_17664_end*/
    U8              rat_type[MAX_RAT_CAPABILITY];   
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/       /* rrc_rat_type_et */
}rat_type_info_t;


/*****************************************************************************
      RRC_RRM_CELL_DEL_REQ
******************************************************************************/
/* SPR 21369 Start */
#define RRC_RRM_INSTANCE_RESET_FLAG_PRESENT			0x01
typedef struct
{
    U8               bitmask;    /*^ BITMASK ^*/
/* SPR 21369 End */
    rrc_cell_index_t cell_index; /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
}rrc_rrm_cell_del_req_t;/*^ API,  RRC_RRM_CELL_DELETE_REQ ^*/

typedef struct
{
    U16                                     bitmask;                    /*^ BITMASK ^*/

    #define RAT_TYPE_INFO_PRESENT   0x01


    U16                         ue_Index; 
    rat_type_info_t             rat_type_info;  /*^ O, RAT_TYPE_INFO_PRESENT,N,0,0 ^*/
}rrc_rrm_ue_capability_enquiry_req_t;
/*^ API, RRC_RRM_UE_CAPABILITY_ENQUIRY_REQ ^*/

/* ERB RELEASE COMMAND START */

/******************************************************************************
            RRC_RRM_ERB_RELEASE_REQ
******************************************************************************/

typedef struct
{
     U16                     count;    
     rrm_erab_failed_item_t  erab_failed_item[MAX_ERAB_COUNT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrm_erab_failed_item_list_t;

typedef struct
{
    rrc_bitmask_t                      bitmask;                 /*^ BITMASK ^*/
#define RRC_RRM_ERB_RELEASE_REQ_UE_AGG_MAX_BIT_RATE_PRESENT    0x01
    
    U16                               ue_index;

    rrm_ue_agg_max_bit_rate_t         ue_agg_max_bit_rate;
/*^ O, RRC_RRM_ERB_RELEASE_REQ_UE_AGG_MAX_BIT_RATE_PRESENT, N, 0, 0 ^*/

     rrm_erab_to_be_released_item_list_t  erab_to_be_released_item_list;
/*^ M, 0, N, 0, 0 ^*/

} rrc_rrm_erb_release_req_t; /*^ API, RRC_RRM_ERB_RELEASE_REQ ^*/

/******************************************************************************
            RRC_RRM_ERB_RELEASE_RESP
******************************************************************************/
typedef struct
{
     U16             num_of_list;   
	/* SPR_17664_start*/
/*^ M, 0, B, 1, MAX_LC_COUNT ^*/
     rrm_erab_item_t  drb_release_item[MAX_LC_COUNT];
	/* SPR_17664_end*/
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_erab_release_item_list_t;
typedef struct
{
     U8                            erab_id;    /*^ M, 0, H, 0, 15 ^*/
     rrm_cause_t                   cause;
}rrm_erab_failed_to_release_item_t;

typedef struct
{
     U16                                num_of_list; 
	/* SPR_17664_start*/
/*^ M, 0, B, 1, MAX_LC_COUNT ^*/
     rrm_erab_failed_to_release_item_t  drb_failed_to_release_item[MAX_LC_COUNT];
	/* SPR_17664_end*/
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_erab_failed_to_release_item_list_t; 

typedef struct
{
    rrc_bitmask_t   bitmask;                 /*^ BITMASK ^*/
#define RRC_RRM_DRB_RELEASE_ITEM_LIST_PRESENT            0x01
#define RRC_RRM_DRB_FAILED_TO_RELEASE_ITEM_LIST_PRESENT  0x02
    /* SPS Start */
#define RRC_RRM_ERB_RELEASE_RESP_SPS_CONFIG_PRESENT         0x04
    /* SPS Stop */
/*SPR_19066_START*/
#define RRM_RRM_ERB_RELEASE_RESP_UE_INACTIVE_TIME_PRESENT   0x08
/*SPR_19066_END*/

    U16                               ue_index;
    rrc_response_t                    response;
    
    rrm_erab_release_item_list_t       drb_release_item_list;
    /*^ O, RRC_RRM_DRB_RELEASE_ITEM_LIST_PRESENT ^*/
    rrm_erab_failed_to_release_item_list_t  drb_failed_to_release_item_list;
    /*^ O, RRC_RRM_DRB_FAILED_TO_RELEASE_ITEM_LIST_PRESENT ^*/ 

    /* SPS Start */
    rrc_sps_config_t                  sps_config; 
    /*^ O, RRC_RRM_ERB_RELEASE_RESP_SPS_CONFIG_PRESENT, N, 0, 0 ^*/
    /* SPS Stop */

/*SPR_19066_START*/
    U32                               ue_inactive_time_config;
/*^ O, RRM_RRM_ERB_RELEASE_RESP_UE_INACTIVE_TIME_PRESENT, N, 0, 0 ^*/
/*SPR_19066_END*/
}rrc_rrm_erb_release_resp_t; /*^ API, RRC_RRM_ERB_RELEASE_RESP ^*/

/******************************************************************************
            RRC_RRM_ERB_RELEASE_CNF
******************************************************************************/

typedef struct
{
    rrc_bitmask_t   bitmask;                 /*^ BITMASK ^*/
#define RRM_ERB_RELEASE_CNF_CONFIRM_LIST_PRESENT    0x01
#define RRM_ERB_RELEASE_CNF_ERROR_LIST_PRESENT      0x02
    /* SPS Start */
#define RRC_RRM_ERB_RELEASE_CNF_SPS_ERROR_CODE_PRESENT 0x04
    /* SPS Stop */
    U16                  ue_index;
    U16                  response;

    rrm_erab_cnf_list_t    erab_cnf_list;
/*^ O, RRM_ERB_RELEASE_CNF_CONFIRM_LIST_PRESENT ^*/
    rrm_erab_error_list_t  erab_error_list;
/*^ O, RRM_ERB_RELEASE_CNF_ERROR_LIST_PRESENT ^*/
    
    /* SPS Start */
    U8           error_code;
    /*^ O, RRC_RRM_ERB_RELEASE_CNF_SPS_ERROR_CODE_PRESENT, H, 0, 0 ^*/
    /* rrc_rrm_error_code_et */
    /* SPS Stop */

} rrc_rrm_erb_release_cnf_t; /*^ API, RRC_RRM_ERB_RELEASE_CNF ^*/


/******************************************************************************
            RRC_RRM_ERB_RELEASE_IND
******************************************************************************/
typedef struct
{
/*SPR_19066_START*/
    rrc_bitmask_t               bitmask;    /*^ BITMASK ^*/
#define RRM_ERB_RELEASE_IND_API_UE_INACTIVE_TIME_PRESENT            0x01
/*SPR_19066_END*/
    U16           ue_index;

     rrm_erab_to_be_released_item_list_t  erab_to_be_released_item_list;
 /*^ M, 0, N, 0, 0 ^*/

/*SPR_19066_START*/
    U32                                  ue_inactive_time_config;
/*^ O, RRM_ERB_RELEASE_IND_API_UE_INACTIVE_TIME_PRESENT, N, 0, 0 ^*/
/*SPR_19066_END*/
} rrc_rrm_erb_release_ind_t; /*^ API, RRC_RRM_ERB_RELEASE_IND ^*/ 
/******************************************************************************
    RRC_RRM_UE_CAPABILITY_ENQUIRY_RESP
******************************************************************************/

typedef enum
{
   rrm_supported_3
}rrc_irat_params_geran_v920_dtm_r9_root_et;

typedef enum
{
   RRM_SUPPORTED_4
}rrc_irat_params_geran_v920_e_redirection_geran_r9_root_et;

typedef enum
{
   RRM_RRC_E_REDIRECTION_UTRA_SUPPORTED 
}rrc_irat_params_utra_v920_e_redirection_utra_r9_et;

typedef enum
{
   ECSFB_1XRTT_R9_SUPPORTED
}rrc_rrm_eCSFB_1xrtt_r9_et;

typedef enum
{
   ECSFB_CONC_PS_MOB_1XRTT_R9_SUPPORTED
}rrc_rrm_eCSFB_conc_ps_mobility_1xrtt_r9_et;

/* CR 597 changes */
typedef enum
{
   ECSFB_1XRTT_DUAL_R10_SUPPORTED
}rrc_rrm_eCSFB_1xrtt_r10_et;
/* CR 597 changes */

typedef enum
{
    rrm_rel8,
    rrm_rel9,       
    rrm_rel10,
    RRM_REL_11,
    RRM_REL_12,
    RRM_REL_13,
    RRM_REL_SPARE2,
    RRM_REL_SPARE1,
    RRM_REL_MAX = RRM_REL_SPARE1
}rrc_access_stratum_release_et;

typedef enum
{
    rrm_cs2,
    rrm_cs4,
    rrm_cs8,
    rrm_cs12,
    rrm_cs16,
    rrm_cs24,
    rrm_cs32,
    rrm_cs48,
    rrm_cs64,
    rrm_cs128,
    rrm_cs256,
    rrm_cs512,
    rrm_cs1024,
    rrm_cs16384,
    rrm_spare2,
    rrm_spare1
}rrc_max_num_rohc_context_session_et;

/*CR 713 changes start*/
typedef enum
{
    RRM_RRC_E_REDIRECTION_UTRA_TDD_R10_SUPPORTED
}rrc_irat_parameters_utra_tdd_v1020_e_redirection_utra_tdd_r10_et;

typedef struct 
{
    U8  e_redirection_utra_tdd_r10;/*^ M, 0, H, 0, 0 ^*/
    /*rrc_irat_parameters_utra_tdd_v1020_e_redirection_utra_tdd_r10_et */

}irat_parameters_utra_tdd_v1020_t;
/* CR 713 changes stop*/


typedef struct
{
    rrc_pdcp_rohc_profile_t                 rohc_profile;   /*^ M, 0, N, 0, 0 ^*/   
    U8      max_num_rohc_context_session;   /*^ M, 0, H, 1, 15 ^*/   /* rrc_max_num_rohc_context_session_et */
}pdcp_parameters_t;

typedef struct
{
    U8      ue_TxAntennaSelectionSupported; /*^ M, 0, H, 1, 1 ^*/      /* rrc_bool_et */
    U8      ue_SpecificRefSigsSupported;    /*^ M, 0, H, 1, 1 ^*/      /* rrc_bool_et */
}physical_layer_params_t;

typedef struct
{
    U8      band_eutra;     /*^ M, 0, B, 1, 64 ^*/
    U8      half_duplex;    /*^ M, 0, H, 1, 1 ^*/   /* rrc_bool_et */
}supported_band_eutra_t;

typedef struct
{
/*SPR_17664_start*/
    U8          count;      /*^ M, 0, B, 1, MAX_BAND_EUTRA ^*/     /* MAX_BAND_EUTRA  */
/*SPR_17664_end*/
    supported_band_eutra_t supported_band_eutra[MAX_BAND_EUTRA]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/    
}supported_band_list_eutra_t;

typedef struct
{
    supported_band_list_eutra_t     supported_band_list_eutra;  /*^ M, 0, H, 1, 64 ^*/
}rf_parameters_t;

typedef struct
{
    U8      inter_freq_need_for_gaps;   /*^ M, 0, H, 0, 1 ^*/   /* rrc_bool_et */
}inter_freq_band_info_t;

typedef struct
{
/*SPR_17664_start*/
    U8                      count;      /*^ M, 0, B, 1, MAX_BAND_EUTRA ^*/     /* MAX_BAND_EUTRA  */
/*SPR_17664_end*/
    inter_freq_band_info_t  inter_freq_band_info[MAX_BAND_EUTRA]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/    
}inter_freq_band_list_t;

typedef struct
{
    U8      inter_rat_need_for_gaps;    /*^ M, 0, H, 0, 1 ^*/   /* rrc_bool_et */
}inter_rat_band_info_t;

typedef struct
{
/*SPR_17664_start*/
    U8                      count;      /*^ M, 0, B, 1, MAX_BAND_EUTRA ^*/     /* MAX_BAND_EUTRA  */
/*SPR_17664_end*/
    inter_rat_band_info_t   inter_rat_band_info[MAX_BAND_EUTRA];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/    
}inter_rat_band_list_t;

typedef struct
{
    U16                     bitmask;                    /*^ BITMASK ^*/
    #define EUTRA_INTER_RAT_BAND_LIST_PRESENT       0x01
    inter_freq_band_list_t  inter_freq_band_list;
    inter_rat_band_list_t   inter_rat_band_list;
    /*^ O, EUTRA_INTER_RAT_BAND_LIST_PRESENT,N,0,0 ^*/
}band_info_eutra_t;

typedef struct
{
/*SPR_17664_start*/
    U8                  count;      /*^ M, 0, B, 1, MAX_BAND_EUTRA ^*/     /* MAX_BAND_EUTRA  */
/*SPR_17664_end*/
    band_info_eutra_t   band_info_eutra[MAX_BAND_EUTRA]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/   
}band_list_eutra_t;

typedef struct
{
    band_list_eutra_t       band_list_eutra;
}meas_pameters_t;

typedef enum
{
    utra_fdd_bandI,
    utra_fdd_bandII,
    utra_fdd_bandIII,
    utra_fdd_bandIV,
    utra_fdd_bandV,
    utra_fdd_bandVI,
    utra_fdd_bandVII,
    utra_fdd_bandVIII,
    utra_fdd_bandIX,
    utra_fdd_bandX,
    utra_fdd_bandXI,
    utra_fdd_bandXII,
    utra_fdd_bandXIII,
    utra_fdd_bandXIV,
    utra_fdd_bandXV,
    utra_fdd_bandXVI,
    utra_fdd_bandXVII_8a0,    
    utra_fdd_bandXVIII_8a0,   
    utra_fdd_bandXIX_8a0,     
    utra_fdd_bandXX_8a0,      
    utra_fdd_bandXXI_8a0,     
    utra_fdd_bandXXII_8a0,    
    utra_fdd_bandXXIII_8a0,   
    utra_fdd_bandXXIV_8a0,    
    utra_fdd_bandXXV_8a0,     
    utra_fdd_bandXXVI_8a0,    
    utra_fdd_bandXXVII_8a0,   
    utra_fdd_bandXXVIII_8a0,  
    utra_fdd_bandXXIX_8a0,    
    utra_fdd_bandXXX_8a0,     
    utra_fdd_bandXXXI_8a0,   
    utra_fdd_bandXXXII_8a0
}supported_band_utra_fdd_et;

typedef struct
{
/*SPR_17664_start*/
    U8                          count;      /*^ M, 0, B, 1, MAX_BAND_EUTRA ^*/     /* MAX_BAND_EUTRA  */
/*SPR_17664_end*/
 U8  supported_band_utra_fdd[MAX_BAND_EUTRA];
        /*^ M,0,OCTET_STRING,VARIABLE ^*/   /* supported_band_utra_fdd_et */
}utra_fdd_t;

typedef enum
{
    tdd128_a,
    tdd128_b,
    tdd128_c,
    tdd128_d,
    tdd128_e,
    tdd128_f,
    tdd128_g,
    tdd128_h,
    tdd128_i,
    tdd128_j,
    tdd128_k,
    tdd128_l,
    tdd128_m,
    tdd128_n,
    tdd128_o,
    tdd128_p
}supported_band_utra_tdd128_et;

typedef struct
{
/*SPR_17664_start*/
    U8                          count;      /*^ M, 0, B, 1, MAX_BAND_EUTRA ^*/     /* MAX_BAND_EUTRA  */
/*SPR_17664_end*/
    U8  supported_band_utra_tdd128[MAX_BAND_EUTRA];
        /*^ M,0,OCTET_STRING,VARIABLE ^*/   /* supported_band_utra_tdd128_et */
}utra_tdd128_t;

typedef enum
{
    tdd384_a,
    tdd384_b,
    tdd384_c,
    tdd384_d,
    tdd384_e,
    tdd384_f,
    tdd384_g,
    tdd384_h,
    tdd384_i,
    tdd384_j,
    tdd384_k,
    tdd384_l,
    tdd384_m,
    tdd384_n,
    tdd384_o,
    tdd384_p
}supported_band_utra_tdd384_et;

typedef struct
{
/*SPR_17664_start*/
    U8                          count;      /*^ M, 0, B, 1, MAX_BAND_EUTRA ^*/     /* MAX_BAND_EUTRA  */
/*SPR_17664_end*/
    U8  supported_band_utra_tdd384[MAX_BAND_EUTRA];
        /*^ M,0,OCTET_STRING,VARIABLE ^*/   /* supported_band_utra_tdd384_et */
}utra_tdd384_t;

typedef enum
{
    tdd768_a,
    tdd768_b,
    tdd768_c,
    tdd768_d,
    tdd768_e,
    tdd768_f,
    tdd768_g,
    tdd768_h,
    tdd768_i,
    tdd768_j,
    tdd768_k,
    tdd768_l,
    tdd768_m,
    tdd768_n,
    tdd768_o,
    tdd768_p
}supported_band_utra_tdd768_et;

typedef struct
{
/*SPR_17664_start*/
    U8                              count;      /*^ M, 0, B, 1, MAX_BAND_EUTRA ^*/     /* MAX_BAND_EUTRA  */
/*SPR_17664_end*/
    U8  supported_band_utra_tdd768[MAX_BAND_EUTRA];
        /*^ M,0,OCTET_STRING,VARIABLE ^*/   /* supported_band_utra_tdd768_et */
}utra_tdd768_t;

typedef enum
{
    geran_gsm450,
    geran_gsm480,
    geran_gsm710,
    geran_gsm750, 
    geran_gsm810, 
    geran_gsm850,
    geran_gsm900P,
    geran_gsm900E, 
    geran_gsm900R, 
    geran_gsm1800, 
    geran_gsm1900,
    geran_spare5, 
    geran_spare4, 
    geran_spare3, 
    geran_spare2, 
    geran_spare1
}supported_band_geran_et;

typedef struct
{
/*SPR_17664_start*/
    U8                          count;      /*^ M, 0, B, 1, MAX_BAND_EUTRA ^*/     /* MAX_BAND_EUTRA  */
/*SPR_17664_end*/
    U8  supported_band_geran[MAX_BAND_EUTRA];
        /*^ M,0,OCTET_STRING,VARIABLE ^*/   /* supported_band_geran_et */
}supported_band_list_geran_t;

typedef struct
{
    supported_band_list_geran_t     supported_band_list_geran;
    U8                              inter_rat_ps_ho_to_geran;   /*^ M, 0, H, 1, 1 ^*/   /* rrc_bool_et */
}geran_t;

typedef struct
{
/*SPR_17664_start*/
    U8                          count;      /*^ M, 0, B, 1, MAX_CDMA_BAND_CLASS ^*/     /* MAX_CDMA_BAND_CLASS  */
/*SPR_17664_end*/
    U8  band_class_cdma2000[MAX_CDMA_BAND_CLASS];
        /*^ M,0,OCTET_STRING,VARIABLE ^*/   /* rrm_bandclass_cdma2000_et */
}supported_band_list_hrpd_t;

typedef enum
{
    tx_rx_single,
    tx_rx_dual
}tx_rx_config_hrpd_1xrtt;

typedef struct
{
    supported_band_list_hrpd_t      supported_band_list_hrpd;
    U8          tx_confiig_hrpd;    /*^ M, 0, H, 0, 16 ^*/   /* tx_rx_config_hrpd_1xrtt */
    U8          rx_confiig_hrpd;    /*^ M, 0, H, 0, 16 ^*/   /* tx_rx_config_hrpd_1xrtt */
}cdma2000_hrpd_t;

typedef struct
{
/*SPR_17664_start*/
    U8               count;     /*^ M, 0, B, 1, MAX_CDMA_BAND_CLASS ^*/     /* MAX_CDMA_BAND_CLASS  */
/*SPR_17664_end*/
    U8  band_class_cdma2000[MAX_CDMA_BAND_CLASS];
        /*^ M,0,OCTET_STRING,VARIABLE ^*/   /* rrm_bandclass_cdma2000_et */
}supported_band_list_1xrtt_t;

typedef struct
{
    supported_band_list_1xrtt_t     supported_band_list_1xrtt;
    U8          tx_confiig_1xrtt;   /*^ M, 0, H, 0, 16 ^*/   /* tx_rx_config_hrpd_1xrtt */
    U8          rx_confiig_1xrtt;   /*^ M, 0, H, 0, 16 ^*/   /* tx_rx_config_hrpd_1xrtt */
}cdma2000_1x_rtt_t;

typedef struct
{
    U16                   bitmask;                    /*^ BITMASK ^*/
    
    #define INTER_RAT_UTRA_FDD_PRESENT              0x01
    #define INTER_RAT_UTRA_TDD128_PRESENT           0x02
    #define INTER_RAT_UTRA_TDD384_PRESENT           0x04    
    #define INTER_RAT_UTRA_TDD768_PRESENT           0x08
    #define INTER_RAT_UTRA_GERAN_PRESENT            0x10    
    #define INTER_RAT_UTRA_CDMA2000_HRPD_PRESENT    0x20    
    #define INTER_RAT_UTRA_CDMA2000_1XRTT_PRESENT   0x40        

    utra_fdd_t          utra_fdd;       /*^ O, INTER_RAT_UTRA_FDD_PRESENT,N,0,0 ^*/
    utra_tdd128_t       utra_tdd128;    /*^ O, INTER_RAT_UTRA_TDD128_PRESENT,N,0,0 ^*/
    utra_tdd384_t       utra_tdd384;    /*^ O, INTER_RAT_UTRA_TDD384_PRESENT,N,0,0 ^*/
    utra_tdd768_t       utra_tdd768;    /*^ O, INTER_RAT_UTRA_TDD768_PRESENT,N,0,0 ^*/
    geran_t             geran;          /*^ O, INTER_RAT_UTRA_GERAN_PRESENT,N,0,0 ^*/
    cdma2000_hrpd_t     cdma2000_hrpd;  /*^ O, INTER_RAT_UTRA_CDMA2000_HRPD_PRESENT,N,0,0 ^*/
    cdma2000_1x_rtt_t   cdma2000_1x_rtt;    /*^ O, INTER_RAT_UTRA_CDMA2000_1XRTT_PRESENT,N,0,0 ^*/
}inter_rat_Parameters_t;

typedef struct
{
    U16                   bitmask;                    /*^ BITMASK ^*/
    
    #define EUTRA_FEATURE_GROUP_INDICATORS_PRESENT      0x01
    
    U8      access_stratum_release; /*^ M, 0, H, 1, 7 ^*/   /* rrc_access_stratum_release_et */
    U8                                  ue_category;    /*^ M, 0, B, 1, 5 ^*/
    pdcp_parameters_t                   pdcp_parameters;
    physical_layer_params_t             physical_layer_params;
    rf_parameters_t                     rf_parameters;
    meas_pameters_t                     meas_prameters;
    U8                                  feature_group_indicators[4];
    /*^ O, EUTRA_FEATURE_GROUP_INDICATORS_PRESENT, OCTET_STRING, FIXED ^*/
/*BIT STRING (SIZE (32))*/
    inter_rat_Parameters_t              inter_rat_Parameters;
}eutra_radio_capability_info_def_t;



/* CR_407 and 392 changes */
typedef struct
{
    U16                   bitmask;                    /*^ BITMASK ^*/
#define IRAT_PARAMS_GERAN_V920_DTM_R9_PRESENT                 0x01
#define IRAT_PARAMS_GERAN_V920_E_REDIRECTION_GERAN_R9_PRESENT 0x02

    U8                    dtm_r9;
 /*^ O, IRAT_PARAMS_GERAN_V920_DTM_R9_PRESENT, H, 0, 0 ^*/
 /*rrc_irat_params_geran_v920_dtm_r9_root_et*/

    U8                    e_redirection_geran_r9;
   /*^ O, IRAT_PARAMS_GERAN_V920_E_REDIRECTION_GERAN_R9_PRESENT, H, 0, 0 ^*/
   /* rrc_irat_params_geran_v920_e_redirection_geran_r9_root_et */

}irat_parameters_geran_v920_t;

typedef struct
{
    /*CSR 00058668 Fix start*/
    U8                    e_redirection_utra_r9;/*^ M, 0, N, 0, 0 ^*/   
 /*rrc_irat_params_utra_v920_e_redirection_utra_r9_et*/
    /*CSR 00058668 Fix stop*/
    
}irat_parameters_utra_v920_t;

/* CDMA2000_CSFB_HO start */
typedef struct
{
    U16                   bitmask;                    /*^ BITMASK ^*/
#define IRAT_PARAMS_CDMA_V920_E_CSFB_CONC_PS_MOB_1XRTT_R9_PRESENT                 0x01
    /*CSR 00058668 Fix start*/
    U8                    eCSFB_1xrtt_r9;/*^ M, 0, N, 0, 0 ^*/
 /*rrc_rrm_eCSFB_1xrtt_r9_et*/

    U8                    eCSFB_conc_ps_mobility_1xrtt_r9;
/*^ O, IRAT_PARAMS_CDMA_V920_E_CSFB_CONC_PS_MOB_1XRTT_R9_PRESENT, N, 0, 0 ^*/
 /*rrc_rrm_eCSFB_conc_ps_mobility_1xrtt_r9_et*/
    /*CSR 00058668 Fix stop*/

}irat_parameters_cdma2000_v920_t;
/* CDMA2000_CSFB_HO stop */

/* CR 597 changes */
typedef struct
{
    U8                   eCSFB_dual_1xrtt_r10;/*^ M, 0, H, 0, 0 ^*/
 /*rrc_rrm_eCSFB_1xrtt_r10_et*/
}irat_parameters_cdma2000_v1020_t;
/* CR 597 changes */

typedef struct
{
    U8 enhanced_dual_layer_fdd_supported_r9; 
    /*^ M, 0, H, 1, 1 ^*/ /* rrc_bool_et */

    U8 enhanced_dual_layer_tdd_supported_r9; 
    /*^ M, 0, H, 1, 1 ^*/ /* rrc_bool_et */

}phy_layer_parameters_v920_t;

typedef struct
{
    U8 intra_freq_proximity_indication_supported_r9;
    /*^ M, 0, H, 1, 1 ^*/ /* rrc_bool_et */

    U8 inter_freq_proximity_indication_supported_r9;
    /*^ M, 0, H, 1, 1 ^*/ /* rrc_bool_et */

    U8 utran_proximity_indication_supported_r9;
    /*^ M, 0, H, 1, 1 ^*/ /* rrc_bool_et */

}csg_proximity_indication_parameters_r9_t;

typedef struct
{
    U8 intra_freq_si_acquisition_for_ho_supported_r9;
    /*^ M, 0, H, 1, 1 ^*/ /* rrc_bool_et */

    U8 inter_freq_si_acquisition_for_ho_supported_r9;
    /*^ M, 0, H, 1, 1 ^*/ /* rrc_bool_et */

    U8 utran_si_acquisition_for_ho_supported_r9;
    /*^ M, 0, H, 1, 1 ^*/ /* rrc_bool_et */

}neigh_cell_si_acquisition_parameters_r9_t;

typedef struct
{
    U8 rach_report_supported_r9;
    /*^ M, 0, H, 1, 1 ^*/ /* rrc_bool_et */
     
}son_parameters_r9_t;

/* cr_346 chnages */

typedef enum
{
    NO_BEN_FROM_BAT_CONSUMP_OPT
}device_type_r9_et;

/*CR 543 changes */
typedef enum
{
    bw_a,
    bw_b,
    bw_c,
    bw_d,
    bw_e,
    bw_f
}ca_band_width_class_r10_et;

typedef enum
{
    UL_TWO_LAYERS,
    UL_FOUR_LAYERS
}mimo_capability_ul_r10_et;

typedef enum
{
    DL_TWO_LAYERS,
    DL_FOUR_LAYERS,
    DL_EIGHT_LAYERS
}mimo_capability_dl_r10_et;

typedef struct
{
    U8                              bitmask;    /*^ BITMASK ^*/
#define RRC_RRM_SUPPORTED_MIMO_CAPABILITY_UL_R10_IE_PRESENT 0x01
    /*CSR 00058668 Fix start*/
    U8                             ca_band_width_class_ul_r10;
    /*^ M, 0, H, 0, 5 ^*/ /* ca_band_width_class_r10_et */   
    U8                             supported_mimo_capability_ul_r10;
/*^ O, RRC_RRM_SUPPORTED_MIMO_CAPABILITY_UL_R10_IE_PRESENT, H, 0, 1 ^*/
    /* mimo_capability_ul_r10_et */
    /*CSR 00058668 Fix stop*/
}ca_mimo_parameters_ul_r10_t;

typedef struct
{
    /* SPR:9111 start */
/* SPR 13502 Fix Start */
    /*CID 93968 Start*/
/*SPR_17664_start*/
    U8                              count;      /*^ M, 0, B, 1, MAX_BANDWIDTH_CLASS_R10 ^*/   /* MAX_BANDWIDTH_CLASS_R10 */
/*SPR_17664_end*/
    /*CID 93968 End*/
/* SPR 13502 Fix Stop */
    /* SPR:9111 stop */
    ca_mimo_parameters_ul_r10_t     ca_mimo_parameters_ul_r10[MAX_BANDWIDTH_CLASS_R10];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/    
}band_parameters_ul_r10_t;

typedef struct
{
    U8                              bitmask;    /*^ BITMASK ^*/
#define RRC_RRM_SUPPORTED_MIMO_CAPABILITY_DL_R10_IE_PRESENT 0x01
    /*CSR 00058668 Fix start*/
    U8                             ca_band_width_class_dl_r10;
    /*^ M, 0, H, 0, 5 ^*/ /* ca_band_width_class_r10_et */
    U8                             supported_mimo_capability_dl_r10;
/*^ O, RRC_RRM_SUPPORTED_MIMO_CAPABILITY_DL_R10_IE_PRESENT, H, 0, 2 ^*/ 
    /* mimo_capability_dl_r10_et */
    /*CSR 00058668 Fix stop*/
}ca_mimo_parameters_dl_r10_t;

typedef struct
{
    /* SPR:9111 start */
/* SPR 13502 Fix Start */
    /*CID 93949 Start*/
/*SPR_17664_start*/
    U8                              count;      /*^ M, 0, B, 1, MAX_BANDWIDTH_CLASS_R10 ^*/   /* MAX_BANDWIDTH_CLASS_R10 */
/*SPR_17664_end*/
    /*CID 93949 End*/
/* SPR 13502 Fix Stop */
    /* SPR:9111 stop */
    ca_mimo_parameters_dl_r10_t     ca_mimo_parameters_dl_r10[MAX_BANDWIDTH_CLASS_R10];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/    
}band_parameters_dl_r10_t;

typedef struct
{
    U8                          bitmask;        /*^ BITMASK ^*/
#define RRC_RRM_BAND_PARAMETERS_UL_R10_IES_PRESENT      0x01
#define RRC_RRM_BAND_PARAMETERS_DL_R10_IES_PRESENT      0x02
    U8                          band_eutra_r10;     /*^ M, 0, B, 1, MAX_FREQ_BAND_INDICATOR ^*/
    band_parameters_ul_r10_t    band_parameters_ul_r10; 
/*^ O, RRC_RRM_BAND_PARAMETERS_UL_R10_IES_PRESENT ^*/
    band_parameters_dl_r10_t    band_parameters_dl_r10;
/*^ O, RRC_RRM_BAND_PARAMETERS_DL_R10_IES_PRESENT ^*/
}band_parameters_r10_t;

typedef struct
{
    /* SPR:9111 start */
/* SPR 13502 Fix Start */
    /*CID 93974 Start*/
/*SPR_17664_start*/
    U8              count;  /*^ M, 0, B, 1, MAX_SIM_BANDS_R10 ^*/  /* MAX_SIM_BANDS_R10 */ 
/*SPR_17664_end*/
    /*CID 93974 End*/
/* SPR 13502 Fix Stop */
    /* SPR:9111 stop */
    band_parameters_r10_t       band_parameters_info[MAX_SIM_BANDS_R10];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/    
}band_combination_parameters_r10_t;

typedef struct
{
    /* SPR:9111 start */
    /* SPR 13502 Fix Start */
    /*CID 93978 Start*/
/*SPR_17664_start*/
    U8              count;     /*^ M, 0, B, 1, MAX_BAND_COMB_R10 ^*/   /* MAX_BAND_COMB_R10 */ 
/*SPR_17664_end*/
    /*CID 93978 End*/
    /* SPR 13502 Fix Stop */
    /* SPR:9111 stop */
    band_combination_parameters_r10_t       band_combination_parameters_r10[MAX_BAND_COMB_R10]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/    
}supported_band_combination_r10_ies_t;

typedef struct
{
    supported_band_combination_r10_ies_t    supported_band_combination_r10;
}rf_parameters_v1020_ies_t;

typedef struct
{
    /* SPR:9111 start */
    /* SPR 13502 Fix Start */
    /*CID 93969 Start*/
/*SPR_17664_start*/
    U8                  count;     /*^ M, 0, B, 1, MAX_BAND_COMB_R10 ^*/   /* MAX_BAND_COMB_R10 */ 
/*SPR_17664_end*/
    /*CID 93969 End*/
    /* SPR 13502 Fix Stop */
    /* SPR:9111 stop */
    band_info_eutra_t   band_info_eutra[MAX_BAND_COMB_R10]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/   
}band_combination_list_eutra_r10_ies_t;

typedef struct
{
    band_combination_list_eutra_r10_ies_t band_combination_list_eutra_r10;
}measparameters_v1020_ies_t;
/*CR 543 changes */
/*CR 530,543 changes */
/*CR 710 changes */
typedef enum
{
    TWO_ANTENNA_SUPPORTED
}two_antenna_ports_for_pucch_r10_et;
typedef enum
{
    TM9_SUPPORTED
}tm9_with_8tx_fdd_r10_et;
typedef enum
{
    PMI_SUPPORTED
}pmi_disabling_r10_et;
typedef enum
{
    CROSS_CARRIER_SUPPORTED
}cross_carrier_scheduling_r10_et;
typedef enum
{
    SIMULTANEOUS_SUPPORTED
}simultaneous_pucch_pusch_r10_et;
typedef enum
{
    MULTI_CLUSTER_SUPPORTED
}multi_cluster_pusch_withincc_r10_et;
typedef enum
{
    NONCONTIGUOUSUL_SUPPORTED
}noncontiguousul_ra_withincc_info_r10_et;
typedef struct
{
    U16                   bitmask;                    /*^ BITMASK ^*/
#define RRC_RRM_NONCONTIGUOUSUL_RA_WITHINCC_INFO_R10    0x01
    /*CSR 00058668 Fix start*/
    U8                    noncontiguousul_ra_withincc_info_r10;
    /*^ O, RRC_RRM_NONCONTIGUOUSUL_RA_WITHINCC_INFO_R10 , N, 0, 0 ^*/ /* noncontiguousul_ra_withincc_info_r10_et */
    /*CSR 00058668 Fix stop*/
}noncontiguousul_ra_withincc_r10_ies_t;
typedef struct
{
/*SPR_17664_start*/
    U8                  count;     /*^ M, 0, B, 1, MAX_BAND_EUTRA ^*/   /* MAX_BAND_EUTRA */ 
/*SPR_17664_end*/
    noncontiguousul_ra_withincc_r10_ies_t       noncontiguousul_ra_withincc_r10[MAX_BAND_EUTRA];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}noncontiguousul_ra_withincc_list_r10_ies_t;


typedef struct
{
    U16                   bitmask;                    /*^ BITMASK ^*/
#define RRC_RRM_TWO_ANTENNA_PORTS_FOR_PUCCH_R10_PRESENT                    0x01
#define RRC_RRM_TM9_WITH_8TX_FDD_R10_PRESENT                               0x02
#define RRC_RRM_PMI_DISABLING_R10_PRESENT                                  0x04
#define RRC_RRM_CROSS_CARRIER_SCHEDULING_R10_PRESENT                       0x08
#define RRC_RRM_SIMULTANEOUS_PUCCH_PUSCH_R10_PRESENT                       0x10
#define RRC_RRM_MULTI_CLUSTER_PUSCH_WITHINCC_R10_PRESENT                   0x20
#define RRC_RRM_MULTI_NONCONTIGUOUSUL_RA_WITHINCC_LIST_R10_PRESENT         0x40
    /*CSR 00058668 Fix start*/
    U8          two_antenna_ports_for_pucch_r10;             
    /*^ O, RRC_RRM_TWO_ANTENNA_PORTS_FOR_PUCCH_R10_PRESENT , N, 0, 0 ^*/ /*two_antenna_ports_for_pucch_r10_et */
    U8          tm9_with_8tx_fdd_r10; 
    /*^ O, RRC_RRM_TM9_WITH_8TX_FDD_R10_PRESENT , N, 0, 0 ^*/ /* tm9_with_8tx_fdd_r10_et */
    U8          pmi_disabling_r10;
    /*^ O, RRC_RRM_PMI_DISABLING_R10_PRESENT , N, 0, 0 ^*/ /* pmi_disabling_r10_et */
    U8          cross_carrier_scheduling_r10;
    /*^ O, RRC_RRM_CROSS_CARRIER_SCHEDULING_R10_PRESENT , N, 0, 0 ^*/ /* cross_carrier_scheduling_r10_et */
    U8          simultaneous_pucch_pusch_r10;
    /*^ O, RRC_RRM_SIMULTANEOUS_PUCCH_PUSCH_R10_PRESENT , N, 0, 0 ^*/ /* simultaneous_pucch_pusch_r10_et */
    U8          multi_cluster_pusch_withincc_r10;
    /*^ O, RRC_RRM_MULTI_CLUSTER_PUSCH_WITHINCC_R10_PRESENT , N, 0, 0 ^*/ /* multi_cluster_pusch_withincc_r10_et */
    noncontiguousul_ra_withincc_list_r10_ies_t          noncontiguousul_ra_withincc_list_r10;
    /*^ O, RRC_RRM_MULTI_NONCONTIGUOUSUL_RA_WITHINCC_LIST_R10_PRESENT , N, 0, 0 ^*/
    /*CSR 00058668 Fix stop*/
}phylayer_parameters_v1020_ies_t;
/*CR 710 changes */

/* CR 919 changes start */
typedef struct
{
    U16                   bitmask;                    /*^ BITMASK ^*/
#define RRC_RRM_UE_EUTRA_CAPABILITIES_v1060_PHY_LAYER_PARAMETERS_PRESENT  0x01
#define RRC_RRM_UE_EUTRA_CAPABILITIES_v1060_FEATURE_GRP_IND_REL10_PRESENT 0x02
#define RRC_RRM_UE_EUTRA_CAPABILITIES_v1060_IRAT_PARAMS_CDMA2000_PRESENT  0x04
#define RRC_RRM_UE_EUTRA_CAPABILITIES_v1060_IRAT_PARAMS_UTRA_TDD_PRESENT  0x08

    phylayer_parameters_v1020_ies_t     phylayer_parameter_v1060;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITIES_v1060_PHY_LAYER_PARAMETERS_PRESENT, N, 0, 0 ^*/

    /*CSR 00058668 Fix start*/
    U8                                  featuregroup_ind_r10_v1060[MAX_IND_OCTET_SIZE];
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITIES_v1060_FEATURE_GRP_IND_REL10_PRESENT, OCTET_STRING, FIXED ^*/
    /*CSR 00058668 Fix stop*/

    irat_parameters_cdma2000_v1020_t    inter_rat_parameters_cdma2000_v1060;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITIES_v1060_IRAT_PARAMS_CDMA2000_PRESENT, N, 0, 0 ^*/

    irat_parameters_utra_tdd_v1020_t    inter_rat_parameters_utra_tdd_v1060;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITIES_v1060_IRAT_PARAMS_UTRA_TDD_PRESENT, N, 0, 0 ^*/

}ue_eutra_capability_addxdd_mode_v1060_t;

typedef struct _band_parameters_v1090_t 
{
    U16                                         bitmask;  
#define RRC_RRM_BAND_EUTRA_V1090                0x01
    U16                                         band_eutra_v1090; 
}band_parameters_v1090_t;

typedef struct _band_combination_parameters_v1090_t
{
    /*CID 97170 Start*/
/* SPR-18445 start */
    U8                                          count;     /*^ M, 0, B, 1 , MAX_SIM_BANDS_R10^*/   /* MAX_SIM_BANDS_R10*/
/* SPR-18445 stop */
    /*CID 97170 End*/
    band_parameters_v1090_t                     band_parameters_v1090[MAX_SIM_BANDS_R10];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}band_combination_parameters_v1090_t;

typedef struct _supported_band_combination_v1090_ies_t
{
/* SPR-18445 start */
    U8                                          count;     /*^ M, 0, B, 1, MAX_BAND_COMB_R10 ^*/   /* MAX_BAND_COMB_R10 */ 
/* SPR-18445 stop */
    band_combination_parameters_v1090_t         band_combination_parameters_v1090[MAX_BAND_COMB_R10]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/    
}supported_band_combination_v1090_ies_t;


typedef struct _rf_parameters_v1090_ies_t
{
    U16                                         bitmask;         /*^BITMASK^*/
#define RRC_RRM_RF_PARAMETERS_V1090_SUPPORTED_BAND_COMB_V1090_PRESENT     0x01
    
    supported_band_combination_v1090_ies_t      supported_band_combination_v1090_ies;
    /*^ O, RRC_RRM_RF_PARAMETERS_V1090_SUPPORTED_BAND_COMB_V1090_PRESENT^*/
}rf_parameters_v1090_ies_t;

typedef struct _band_parameters_r11_t
{
    U16                                         bitmask;         /*^BITMASK^*/
#define RRC_RRM_BAND_PARAMETERS_UL_R11_IES_PRESENT      0x01
#define RRC_RRM_BAND_PARAMETERS_DL_R11_IES_PRESENT      0x02

    U16                                         band_eutra_r11; /*^ M, 0, B, 1 , 256^*/ 
    
    band_parameters_ul_r10_t                    band_parameters_ul_r10; 
/*^ O, RRC_RRM_BAND_PARAMETERS_UL_R11_IES_PRESENT ^*/
    band_parameters_dl_r10_t                    band_parameters_dl_r10;
/*^ O, RRC_RRM_BAND_PARAMETERS_DL_R11_IES_PRESENT ^*/
}band_parameters_r11_t;

typedef struct _band_combination_parameters_r11_t
{
    /*CID 97171 Start*/
/* SPR-18445 start */
    U8                                          count;     /*^ M, 0, B, 1 , MAX_SIM_BANDS_R10^*/   /* MAX_SIM_BANDS_R10*/
/* SPR-18445 stop */
    /*CID 97171 End*/
    band_parameters_r11_t                       band_parameters_r11[MAX_SIM_BANDS_R10];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}band_combination_parameters_r11_t;

typedef struct _supported_band_combination_add_r11_ies_t
{
    /* Coverity Fix 97130 Start */
/* SPR-18445 start */
    U8                                          count;     /*^ M, 0, L, 1, MAX_BAND_COMB_R11 ^*/   /* MAX_BAND_COMB_R11 */ 
/* SPR-18445 stop */
    /* Coverity Fix 97130 End */
    band_combination_parameters_r11_t           band_combination_parameters_r11[MAX_BAND_COMB_R11]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/    
}supported_band_combination_add_r11_ies_t;

typedef struct _rf_parameters_v1180_ies_t
{
    U16                                         bitmask;         /*^BITMASK^*/
#define RRC_RRM_RF_PARAMETERS_V1180_SUPPORTED_BAND_COMB_ADD_R11_PRESENT     0x01
    
    supported_band_combination_add_r11_ies_t      supported_band_combination_add_r11_ies;
    /*^ O, RRC_RRM_RF_PARAMETERS_V1180_SUPPORTED_BAND_COMB_ADD_R11_PRESENT^*/
}rf_parameters_v1180_ies_t;

/* LAA Rel13 Code + */
/*! \  struct _rlc_parameters_v1310_t 
 *  \brief RLC parameters
 *  \param U8 rlc_sn_so_field_supported_r13
 */
typedef struct _rlc_parameters_v1310_t
{
	U8 rlc_sn_so_field_supported_r13;
	/*^ M, 0, H, 1, 1 ^*/ /* rrc_bool_et */

}rlc_parameters_v1310_t;

/*! \  struct _pdcp_parameters_v1310_t
 *  \brief PDCP parameters
 *  \param U8 pdcp_sn_ext_18bits_supported_r13
 */
typedef struct _pdcp_parameters_v1310_t
{
	U8 pdcp_sn_ext_18bits_supported_r13; 
	/*^ M, 0, H, 1, 1 ^*/ /* rrc_bool_et */

}pdcp_parameters_v1310_t;

/*! \  struct _laa_parameters_r13_t
 *  \brief LAA parameters
 *  \param U8 cross_carrier_scheduling_laa_dl_r13
 *  \param U8 csi_rs_drs_rrm_measurements_laa_r13
 *  \param U8 downlink_laa_r13
 *  \param U8 ending_DwPTS_r13
 *  \param U8 second_slot_starting_position_r13
 *  \param U8 tm9_laa_r13
 *  \param U8 tm10_laa_r13
 */
typedef struct _laa_parameters_r13_t
{
    U16                   bitmask;                    /*^ BITMASK ^*/

#define RRC_RRM_CROSS_CARRIER_SCHEDULING_LAA_DL_R13_PRESENT         0x01
#define RRC_RRM_CSI_RS_DRS_RRM_MEASUREMENTS_LAA_R13_PRESENT         0x02
#define RRC_RRM_DOWNLINK_LAA_R13_PRESENT                            0x04
#define RRC_RRM_ENDING_DWPTS_R13_PRESENT                            0x08
#define RRC_RRM_SECOND_SLOT_STARTING_POSITION_PRESENT               0x10
#define RRC_RRM_TM9_LAA_R13_PRESENT                                 0x20
#define RRC_RRM_TM10_LAA_R13_PRESENT                                0x40

	U8 cross_carrier_scheduling_laa_dl_r13;
    /*^ O, RRC_RRM_CROSS_CARRIER_SCHEDULING_LAA_DL_R13_PRESENT, H, 0, 1 ^*/ /* rrc_bool_et */

	U8 csi_rs_drs_rrm_measurements_laa_r13;	
    /*^ O, RRC_RRM_CSI_RS_DRS_RRM_MEASUREMENTS_LAA_R13_PRESENT, H, 0, 1 ^*/ /* rrc_bool_et */

	U8 downlink_laa_r13;
    /*^ O, RRC_RRM_DOWNLINK_LAA_R13_PRESENT, H, 0, 1 ^*/ /* rrc_bool_et */

	U8 ending_DwPTS_r13;
    /*^ O, RRC_RRM_ENDING_DWPTS_R13_PRESENT, H, 0, 1 ^*/ /* rrc_bool_et */

	U8 second_slot_starting_position_r13;
    /*^ O, RRC_RRM_SECOND_SLOT_STARTING_POSITION_PRESENT, H, 0, 1 ^*/ /* rrc_bool_et */

	U8 tm9_laa_r13;	
    /*^ O, RRC_RRM_TM9_LAA_R13_PRESENT, H, 0, 1 ^*/ /* rrc_bool_et */

	U8 tm10_laa_r13;
    /*^ O, RRC_RRM_TM10_LAA_R13_PRESENT, H, 0, 1 ^*/ /* rrc_bool_et */

}laa_parameters_r13_t;

/*! \  struct _wlan_iw_parameters_v1310_t
 *  \brief WLAN parameters
 *  \param U8 rclwi_r13
 */
typedef struct _wlan_iw_parameters_v1310_t
{
	U8 rclwi_r13;					/*^ M, 0, H, 1, 1 ^*/ /* rrc_bool_et */
}wlan_iw_parameters_v1310_t;

/*! \  struct _lwip_parameters_r13_t
 *  \brief LWIP parameters
 *  \param U8 lwip_r13
 */
typedef struct _lwip_parameters_r13_t
{
	U8 lwip_r13;					/*^ M, 0, H, 1, 1 ^*/ /* rrc_bool_et */
}lwip_parameters_r13_t;

typedef enum 
{
	BAND2DOT4, 
	BAND5, 
	SPARE6, 
	SPARE5, 
	SPARE4, 
	SPARE3, 
	SPARE2, 
	SPARE1, 
}supported_band_list_wlan_r13_et;


typedef struct _supported_band_list_wlan_r13_t
{
    U8                              count;      
    /*^ M, 0, B, 1, MAX_WLAN_BANDS_R13 ^*/    /* MAX_WLAN_BANDS_R13 */

     U8 data[MAX_WLAN_BANDS_R13]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/ /* supported_band_list_wlan_r13_et */
}supported_band_list_wlan_r13_t;


typedef struct _irat_parameters_wlan_r13_t
{
	U8 supported_band_list_wlan_r13[MAX_WLAN_BANDS_R13];/* supported_band_list_wlan_r13_et */
    /*^ M, 0, OCTET_STRING, FIXED ^*/    
}irat_parameters_wlan_r13_t;

typedef enum
{
    FOUR_LAYER_TM3_TM4_PER_CC_R12_SUPPORTED
}four_layer_tm3_tm4_per_cc_r12_et;


typedef enum
{
    n1_supported,
    n2_supported,
    n3_supported
}supported_csi_proc_r12_et;

typedef struct _band_parameters_ul_r13_t
{
    U8                              count;      
    /*^ M, 0, B, 1, MAX_BANDWIDTH_CLASS_R10 ^*/   /* MAX_BANDWIDTH_CLASS_R10 */ 
    ca_mimo_parameters_ul_r10_t     ca_mimo_parameters_ul_r10[MAX_BANDWIDTH_CLASS_R10];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/    
}band_parameters_ul_r13_t;

typedef struct 
{
    U8                                         bitmask;         /*^BITMASK^*/

#define FOUR_LAYER_TM3_TM4_PER_CC_R12_PRESENT       0x01
#define SUPPORTED_MIMO_CAPABILITY_DL_R12_PRESENT    0x02
#define SUPPORTED_CSI_PROC_R12_PRESENT              0x04

    U8 four_layer_tm3_tm4_per_cc_r12 ;
    /*^ O,FOUR_LAYER_TM3_TM4_PER_CC_R12_PRESENT, H, 0, 0 ^*/
    /* four_layer_tm3_tm4_per_cc_r12_et */

    U8 supported_mimo_capability_dl_r12; 
    /*^ O, RRC_RRM_SUPPORTED_MIMO_CAPABILITY_DL_R10_IE_PRESENT, H, 0, 2 ^*/ 
    /* mimo_capability_dl_r10_et */

    U8 supported_csi_proc_r12;
    /*^ O, SUPPORTED_CSI_PROC_R12_PRESENT, H, 0, 2 ^*/ 
    /* supported_csi_proc_r12_et */

}intra_band_contiguous_cc_info_r12_t;

typedef struct _intra_band_contiguous_cc_info_list_r13_t
{
    U8                                          count;     /*^ M, 0, B, 1, 32 ^*/   /* MAX_SERVING_CELL_R13 */
    intra_band_contiguous_cc_info_r12_t     intra_band_contiguous_cc_info_r12[MAX_SERVING_CELL_R13];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}intra_band_contiguous_cc_info_list_r13_t;

typedef struct _ca_mimo_parameters_dl_r13_t
{
    U8                              bitmask;    /*^ BITMASK ^*/
#define RRC_RRM_SUPPORTED_MIMO_CAPABILITY_DL_R10_IE_PRESENT 0x01

    U8                             ca_band_width_class_dl_r10;
    /*^ M, 0, H, 0, 5 ^*/ /* ca_band_width_class_r10_et */
    U8                             supported_mimo_capability_dl_r10;
    /*^ O, RRC_RRM_SUPPORTED_MIMO_CAPABILITY_DL_R10_IE_PRESENT, H, 0, 2 ^*/ 
    /* mimo_capability_dl_r10_et */

    intra_band_contiguous_cc_info_list_r13_t intra_band_contiguous_cc_info_list_r13;
    /*^ M, 0, N, 0, 0 ^*/
}ca_mimo_parameters_dl_r13_t;

typedef struct _band_parameters_dl_r13_t
{
    U8                              count;      
    /*^ M, 0, B, 1, MAX_BANDWIDTH_CLASS_R13 ^*/   /* MAX_BANDWIDTH_CLASS_R13 */ 
    ca_mimo_parameters_dl_r13_t     ca_mimo_parameters_dl_r13[MAX_BANDWIDTH_CLASS_R13];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/    
}band_parameters_dl_r13_t;


typedef struct _band_parameters_r13_t
{
    U16                                         bitmask;         /*^BITMASK^*/
#define RRC_RRM_BAND_PARAMETERS_UL_R13_IES_PRESENT      0x01
#define RRC_RRM_BAND_PARAMETERS_DL_R13_IES_PRESENT      0x02

    U16                          band_eutra_r13;     /*^ M, 0, B, 1, 64 ^*/ 
    band_parameters_ul_r13_t    band_parameters_ul_r13; 
    /*^ O, RRC_RRM_BAND_PARAMETERS_UL_R13_IES_PRESENT ^*/
    band_parameters_dl_r13_t    band_parameters_dl_r13;
    /*^ O, RRC_RRM_BAND_PARAMETERS_DL_R13_IES_PRESENT ^*/
}band_parameters_r13_t;

typedef struct _band_combination_parameters_r13_bandParameterList_r13_t
{
    U8                                          count;     /*^ M, 0, B, 1 , MAX_SIM_BANDS_R10 ^*/   /* MAX_SIM_BANDS_R10*/ 
    band_parameters_r13_t                 band_parameters_r13[MAX_SIM_BANDS_R10];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}band_combination_parameters_r13_bandParameterList_r13_t;


typedef struct _band_combination_parameters_r13_t
{
    band_combination_parameters_r13_bandParameterList_r13_t band_parameter_list_r13;
    /*^ M, 0, N, 0, 0 ^*/
}band_combination_parameters_r13_t;

typedef struct _supported_band_combination_reduced_r13_ies_t
{
    U8                                          count;     /*^ M, 0, B, 1, 46 ^*/   /* MAX_BAND_COMB_R13 */
    band_combination_parameters_r13_t     band_combination_parameters_r13[MAX_BAND_COMB_R13];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}supported_band_combination_reduced_r13_ies_t;

typedef struct _rf_parameters_v1310_ies_t
{
    U16                                         bitmask;         /*^BITMASK^*/
#define RRC_RRM_RF_PARAMETERS_V1310_SUPPORTED_BAND_COMB_V1310_PRESENT     0x01

    supported_band_combination_reduced_r13_ies_t      supported_band_combination_reduced_r13;
    /*^ O, RRC_RRM_RF_PARAMETERS_V1310_SUPPORTED_BAND_COMB_V1310_PRESENT^*/
}rf_parameters_v1310_ies_t;

#define WLAN_MAC_ADDRESS_SIZE   6

typedef enum
{
    LWA_CAPABILITY_LWA_SUPPORTED = 0
}ue_eutra_capbility_lwa_capability_lwa_supported_et;

typedef enum
{
    LWA_CAPABILITY_SPLIT_BEARER_SUPPORTED = 0
}ue_eutra_capability_lwa_capability_split_bearer_supported_et;

typedef enum
{
    LWA_CAPABILITY_LWA_BUFFER_SIZE_SUPPORTED = 0
}ue_eutra_capbility_lwa_capability_lwa_buffer_size_supported_et;

typedef struct 
{
    rrc_bitmask_t         bitmask;   /*^ BITMASK ^*/

#define LWA_CAPABILITY_LWA_SUPPORTED_PRESENT              0x01
#define LWA_CAPABILITY_SPLIT_BEARER_SUPPORTED_PRESENT     0x02
#define LWA_CAPABILITY_WLAN_MAC_ADDRESS_PRESENT           0x04
#define LWA_CAPABILITY_LWA_BUFFER_SIZE_SUPPORTED_PRESENT  0x08
    
    U8                    lwa_supported;
    /*^ O, LWA_CAPABILITY_LWA_SUPPORTED_PRESENT, N, 0, 0 ^*/

    U8                    split_bearer_supported;
    /*^ O, LWA_CAPABILITY_SPLIT_BEARER_SUPPORTED_PRESENT, N, 0, 0 ^*/

    U8                    wlan_mac_address[WLAN_MAC_ADDRESS_SIZE];
    /*^ O, LWA_CAPABILITY_WLAN_MAC_ADDRESS_PRESENT, N, 0, 0 ^*/

    U8                    lwa_buffer_size_supported;
    /*^ O, LWA_CAPABILITY_LWA_BUFFER_SIZE_SUPPORTED_PRESENT, N, 0, 0 ^*/

}ue_eutra_capability_lwa_parameters_t;
/* eMTC changes start */
#define RRC_RRM_UE_CATEGORY_DL_V1310_PRESENT    0x01
#define RRC_RRM_UE_CATEGORY_UL_V1310_PRESENT    0x02
#define RRC_RRM_CE_PARAMETERS_R13_PRESENT       0x04
typedef struct _UeEutraCapabilityEmtcParamaters_t
{
	RrcBitmaskU8_t    bitmask;
    /*^ BITMASK ^*/

    U8                ueCategoryDlV1310;
    /*^ O, RRC_RRM_UE_CATEGORY_DL_V1310_PRESENT, H, 0, EMTC_DL_UE_CATEGORY_MAX ^*/
    /* EmtcUeCategoryDl_et */

    U8                ueCategoryUlV1310;
    /*^ O, RRC_RRM_UE_CATEGORY_UL_V1310_PRESENT, H, 0, EMTC_UL_UE_CATEGORY_MAX ^*/
    /* EmtcUeCategoryUl_et */
    
    U8                ceParametersR13;
	/*^ O, RRC_RRM_CE_PARAMETERS_R13_PRESENT, H, 0, EMTC_CE_MODE_MAX_SUPPORTED ^*/
    /* EmtcCeParametersR13_et */
}UeEutraCapabilityEmtcParamaters_t;
/* eMTC changes stop */

/* MENB_CHANGES_START */
#define RRC_RRM_EN_DC_R15_PRESENT			0x01
typedef struct _UeEutraCapabilityIratParametersNrR15_t
{
    rrc_bitmask_t          bitmask;
    /*^ BITMASK ^*/

    U8                     enDcR15Supported;
    /*^ O, RRC_RRM_EN_DC_R15_PRESENT, H, 0, 1 ^*/
}UeEutraCapabilityIratParametersNrR15_t;

#define RRC_RRM_IRAT_PARAMETERS_NR_R15_PRESENT			0x01
typedef struct _UeEutraCapabilityEndcParamaters_t
{
    rrc_bitmask_t                             bitmask;
    /*^ BITMASK ^*/

    UeEutraCapabilityIratParametersNrR15_t    iratParametersNrR15;
    /*^ O, RRC_RRM_IRAT_PARAMETERS_NR_R15_PRESENT, N, 0, 0 ^*/
}UeEutraCapabilityEndcParamaters_t;

#define RRC_RRM_UE_EUTRA_CAPABILITY_ENDC_PARAMETERS_R15_PRESENT			0x01
typedef struct _ue_eutra_capability_v1510_ies_t
{
	rrc_bitmask_t                        bitmask;
    /*^ BITMASK ^*/

    UeEutraCapabilityEndcParamaters_t    endcParameters;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_ENDC_PARAMETERS_R15_PRESENT, N, 0, 0 ^*/
}ue_eutra_capability_v1510_ies_t;

#define RRC_RRM_UE_EUTRA_CAPABILITY_V1510_IES_PRESENT			0x01
typedef struct _ue_eutra_capability_v1460_ies_t
{
	rrc_bitmask_t                      bitmask;
    /*^ BITMASK ^*/

    ue_eutra_capability_v1510_ies_t    ue_eutra_capability_v1510_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1510_IES_PRESENT , N, 0, 0 ^*/
}ue_eutra_capability_v1460_ies_t;

#define RRC_RRM_UE_EUTRA_CAPABILITY_V1460_IES_PRESENT			0x01
typedef struct _ue_eutra_capability_v1450_ies_t
{
	rrc_bitmask_t                      bitmask;
    /*^ BITMASK ^*/

    ue_eutra_capability_v1460_ies_t    ue_eutra_capability_v1460_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1460_IES_PRESENT , N, 0, 0 ^*/
}ue_eutra_capability_v1450_ies_t;

#define RRC_RRM_UE_EUTRA_CAPABILITY_V1450_IES_PRESENT			0x01
typedef struct _ue_eutra_capability_v1440_ies_t
{
	rrc_bitmask_t                      bitmask;
    /*^ BITMASK ^*/

    ue_eutra_capability_v1450_ies_t    ue_eutra_capability_v1450_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1450_IES_PRESENT , N, 0, 0 ^*/
}ue_eutra_capability_v1440_ies_t;

#define RRC_RRM_UE_EUTRA_CAPABILITY_V1440_IES_PRESENT			0x01
typedef struct _ue_eutra_capability_v1430_ies_t
{
	rrc_bitmask_t                      bitmask;
    /*^ BITMASK ^*/

    ue_eutra_capability_v1440_ies_t    ue_eutra_capability_v1440_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1440_IES_PRESENT , N, 0, 0 ^*/
}ue_eutra_capability_v1430_ies_t;

#define RRC_RRM_UE_EUTRA_CAPABILITY_V1430_IES_PRESENT			0x01
typedef struct _ue_eutra_capability_v1360_ies_t
{
	rrc_bitmask_t                      bitmask;
    /*^ BITMASK ^*/

    ue_eutra_capability_v1430_ies_t    ue_eutra_capability_v1430_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1430_IES_PRESENT , N, 0, 0 ^*/
}ue_eutra_capability_v1360_ies_t;

#define RRC_RRM_UE_EUTRA_CAPABILITY_V1360_IES_PRESENT			0x01
typedef struct _ue_eutra_capability_v1350_ies_t
{
	rrc_bitmask_t                      bitmask;
    /*^ BITMASK ^*/

    ue_eutra_capability_v1360_ies_t    ue_eutra_capability_v1360_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1360_IES_PRESENT , N, 0, 0 ^*/
}ue_eutra_capability_v1350_ies_t;

#define RRC_RRM_UE_EUTRA_CAPABILITY_V1350_IES_PRESENT			0x01
typedef struct _ue_eutra_capability_v1340_ies_t
{
	rrc_bitmask_t                      bitmask;
    /*^ BITMASK ^*/

    ue_eutra_capability_v1350_ies_t    ue_eutra_capability_v1350_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1350_IES_PRESENT , N, 0, 0 ^*/
}ue_eutra_capability_v1340_ies_t;

#define RRC_RRM_UE_EUTRA_CAPABILITY_V1340_IES_PRESENT			0x01
typedef struct _ue_eutra_capability_v1330_ies_t
{
	rrc_bitmask_t                      bitmask;
    /*^ BITMASK ^*/

    ue_eutra_capability_v1340_ies_t    ue_eutra_capability_v1340_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1340_IES_PRESENT , N, 0, 0 ^*/
}ue_eutra_capability_v1330_ies_t;

#define RRC_RRM_UE_EUTRA_CAPABILITY_V1330_IES_PRESENT			0x01
typedef struct _ue_eutra_capability_v1320_ies_t
{
	rrc_bitmask_t                     bitmask;
    /*^ BITMASK ^*/

    ue_eutra_capability_v1330_ies_t   ue_eutra_capability_v1330_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1330_IES_PRESENT , N, 0, 0 ^*/
}ue_eutra_capability_v1320_ies_t;
/* MENB_CHANGES_END */

/* eMTC changes start */
#define RRC_RRM_UE_EUTRA_CAPABILITY_EMTC_PARAMETERS_R13_PRESENT         0x08
/* eMTC changes stop */

/* MENB_CHANGES_START */
#define RRC_RRM_UE_EUTRA_CAPABILITY_V1320_IES_PRESENT                   0x10
/* MENB_CHANGES_END */
typedef struct _ue_eutra_capability_v1310_ies_t
{
	U16                   bitmask;                    /*^ BITMASK ^*/

#define RRC_RRM_UE_EUTRA_CAPABILITY_V1310_RF_PARAMETERS_PRESENT 		0x01
#define RRC_RRM_UE_LAA_PARAMETERS_PRESENT                               0x02
#define RRC_RRM_UE_EUTRA_CAPABILITY_LWA_PARAMETERS_R13_PRESENT          0x04

	pdcp_parameters_v1310_t          pdcp_parameters_v1310;
	/*^ M, 0, N, 0, 0 ^*/
	rlc_parameters_v1310_t           rlc_parameters_v1310;
	/*^ M, 0, N, 0, 0 ^*/

	rf_parameters_v1310_ies_t           rf_parameters_v1310;
	/*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1310_RF_PARAMETERS_PRESENT^*/

	irat_parameters_wlan_r13_t       inter_rat_parameters_wlan_r13;
	/*^ M, 0, N, 0, 0 ^*/

	laa_parameters_r13_t		laa_parameters_r13;
	/*^ O, RRC_RRM_UE_LAA_PARAMETERS_PRESENT^*/

	wlan_iw_parameters_v1310_t	wlan_iw_parameters_v1310;
	/*^ M, 0, N, 0, 0 ^*/

	lwip_parameters_r13_t		lwip_parameters_r13;
	/*^ M, 0, N, 0, 0 ^*/
    ue_eutra_capability_lwa_parameters_t  lwa_parameters;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_LWA_PARAMETERS_R13_PRESENT, N, 0, 0 ^*/

    /* eMTC changes start */
    UeEutraCapabilityEmtcParamaters_t    emtcParameters;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_EMTC_PARAMETERS_R13_PRESENT, N, 0, 0 ^*/
    /* eMTC changes stop */

    /* MENB_CHANGES_START */
    ue_eutra_capability_v1320_ies_t      ue_eutra_capability_v1320_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1320_IES_PRESENT, N, 0, 0 ^*/
    /* MENB_CHANGES_END */
}ue_eutra_capability_v1310_ies_t;

typedef struct _ue_eutra_capability_v1280_ies_t
{
	U16                   bitmask;                    /*^ BITMASK ^*/
#define RRC_RRM_UE_EUTRA_CAPABILITY_v1310_IES_PRESENT			0x01

   ue_eutra_capability_v1310_ies_t             ue_eutra_capability_v1310_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_v1310_IES_PRESENT , N, 0, 0 ^*/

}ue_eutra_capability_v1280_ies_t;

typedef struct _ue_eutra_capability_v1270_ies_t
{
	U16                   bitmask;                    /*^ BITMASK ^*/
#define RRC_RRM_UE_EUTRA_CAPABILITY_v1280_IES_PRESENT			0x01

    ue_eutra_capability_v1280_ies_t             ue_eutra_capability_v1280_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_v1280_IES_PRESENT , N, 0, 0 ^*/

}ue_eutra_capability_v1270_ies_t;

typedef struct _ue_eutra_capability_v1260_ies_t
{
	U16                   bitmask;                    /*^ BITMASK ^*/
#define RRC_RRM_UE_EUTRA_CAPABILITY_v1270_IES_PRESENT			0x02

    ue_eutra_capability_v1270_ies_t             ue_eutra_capability_v1270_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_v1270_IES_PRESENT , N, 0, 0 ^*/

}ue_eutra_capability_v1260_ies_t;

typedef struct _ue_eutra_capability_v1250_ies_t
{
	U16                   bitmask;                    /*^ BITMASK ^*/
#define RRC_RRM_UE_EUTRA_CAPABILITY_v1260_IES_PRESENT			0x01

    ue_eutra_capability_v1260_ies_t             ue_eutra_capability_v1260_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_v1260_IES_PRESENT , N, 0, 0 ^*/

}ue_eutra_capability_v1250_ies_t;

typedef struct _ue_eutra_capability_v11a0_ies_t
{
	U16                   bitmask;                    /*^ BITMASK ^*/
#define RRC_RRM_UE_EUTRA_CAPABILITY_v1250_IES_PRESENT			0x01

    ue_eutra_capability_v1250_ies_t             ue_eutra_capability_v1250_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_v1250_IES_PRESENT , N, 0, 0 ^*/
}ue_eutra_capability_v11a0_ies_t;
/* LAA Rel13 Code - */

typedef struct _ue_eutra_capability_v1180_ies_t
{
    U16                                         bitmask;         /*^BITMASK^*/
#define RRC_RRM_UE_EUTRA_CAPABILITY_V1180_RF_PARAMETERS_PRESENT     0x01
/* LAA Rel13 Code + */
#define RRC_RRM_UE_EUTRA_CAPABILITY_v11a0_IES_PRESENT		0x02    
/* LAA Rel13 Code - */
    
    rf_parameters_v1180_ies_t                   rf_parameters_v1180;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1180_RF_PARAMETERS_PRESENT^*/
/* LAA Rel13 Code + */
    ue_eutra_capability_v11a0_ies_t             ue_eutra_capability_v11a0_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_v11a0_IES_PRESENT , N, 0, 0 ^*/
/* LAA Rel13 Code - */
}ue_eutra_capability_v1180_ies_t;

typedef struct _ue_eutra_capability_v1090_ies_t
{
    U16                                         bitmask;         /*^BITMASK^*/
#define RRC_RRM_UE_EUTRA_CAPABILITY_V1090_RF_PARAMETERS_PRESENT     0x01
#define RRC_RRM_UE_EUTRA_CAPABILITY_v1180_IES_PRESENT               0x02
    
    rf_parameters_v1090_ies_t                   rf_parameters_v1090;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1090_RF_PARAMETERS_PRESENT^*/

    ue_eutra_capability_v1180_ies_t             ue_eutra_capability_v1180_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_v1180_IES_PRESENT ^*/
}ue_eutra_capability_v1090_ies_t;

typedef struct
{
    U16                   bitmask;                    /*^ BITMASK ^*/
#define RRC_RRM_FDD_ADD_UE_EUTRA_CAPABILITIES_v1060_PRESENT     0x01
#define RRC_RRM_TDD_ADD_UE_EUTRA_CAPABILITIES_v1060_PRESENT     0x02    
#define RRC_RRM_UE_EUTRA_CAPABILITY_V1060_RF_PARAMETERS_PRESENT 0x04    
#define RRC_RRM_UE_EUTRA_CAPABILITY_v1090_IES_PRESENT           0x08   
    ue_eutra_capability_addxdd_mode_v1060_t    fdd_add_ue_eutra_capability_v1060;
    /*^ O ,RRC_RRM_FDD_ADD_UE_EUTRA_CAPABILITIES_v1060_PRESENT, N, 0, 0 ^*/

    ue_eutra_capability_addxdd_mode_v1060_t    tdd_add_ue_eutra_capability_v1060; 
    /*^ O ,RRC_RRM_TDD_ADD_UE_EUTRA_CAPABILITIES_v1060_PRESENT, N, 0, 0 ^*/

    rf_parameters_v1060_ies_t           rf_parameters_v1060;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1060_RF_PARAMETERS_PRESENT , N, 0, 0 ^*/

    ue_eutra_capability_v1090_ies_t             ue_eutra_capability_v1090_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_v1090_IES_PRESENT, N, 0, 0 ^*/
}ue_eutra_capability_v1060_ies_t;
/* CR 919 changes stop */

typedef enum
{
    LOGGED_MEAS_IDLE_R10_TRUE
}logged_measurements_idle_r10_et;

typedef enum
{
    STANDALONE_GNSS_LOCATION_R10_TRUE
}standalone_gnss_location_r10_et;

typedef struct
{
    U16                   bitmask;                    /*^ BITMASK ^*/
#define RRC_RRM_LOGGED_MEAS_IDLE_UE_EUTRA_CAPABILITY_R10_PRESENT            0x01 
#define RRC_RRM_STANDALONE_GNSS_LOC_UE_EUTRA_CAPABILITY_R10_PRESENT         0x02 
    U8                  logged_measurements_idle_r10;
    /*^ O,RRC_RRM_LOGGED_MEAS_IDLE_UE_EUTRA_CAPABILITY_R10_PRESENT, H, 0, 0 ^*/
    /* logged_measurements_idle_r10_et */
    U8                  standalone_gnss_location_r10;
    /*^ O,RRC_RRM_STANDALONE_GNSS_LOC_UE_EUTRA_CAPABILITY_R10_PRESENT, H, 0, 0 ^*/
    /* standalone_gnss_location_r10_et */
}ue_based_nw_perf_meas_params_t;

typedef struct
{
    U16                   bitmask;                    /*^ BITMASK ^*/
#define RRC_RRM_UE_EUTRA_CAPABILITY_V1020_UE_CATEGORY_PRESENT               0x01 
#define RRC_RRM_UE_EUTRA_CAPABILITY_V1020_RF_PARAMETERS_PRESENT             0x02 
#define RRC_RRM_UE_EUTRA_CAPABILITY_V1020_MEAS_PARAMETERS_PRESENT           0x04 
/* CR 597 changes */
#define RRC_RRM_UE_EUTRA_CAPABILITY_INTER_RAT_PARAMS_CDMA2000_V1020_PRESENT 0x08 
/* CR 597 changes */
/* CR 710 changes */
#define RRC_RRM_UE_EUTRA_CAPABILITY_V1020_PHYLAYERS_PARAMETERS_PRESENT      0x10 
#define RRC_RRM_UE_EUTRA_CAPABILITY_V1020_FEATUREGROUP_IND_R10_PRESENT      0x20     
/* CR 710 changes */
/* CR 713 changes start*/
#define RRC_RRM_UE_EUTRA_CAPABILITY_INTER_RAT_PARAMS_UTRA_TDD_V1020_PRESENT 0x40
/* CR 713 changes stop*/
/* CR 919 changes start*/
#define RRC_RRM_UE_EUTRA_CAPABILITY_v1060_IES_PRESENT                       0x80
/* CR 919 changes stop*/    
#define RRC_RRM_UE_EUTRA_CAPABILITY_NW_PERF_MEAS_PARAMS_PRESENT             0x100

    U8                                  ue_category_v1020;    
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1020_UE_CATEGORY_PRESENT, B, 6, 8 ^*/
/* CR 710 changes */
    phylayer_parameters_v1020_ies_t     phylayer_parameter_v1020;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1020_PHYLAYERS_PARAMETERS_PRESENT , N, 0, 0 ^*/
/* CR 710 changes */
    rf_parameters_v1020_ies_t           rf_parameters_v1020;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1020_RF_PARAMETERS_PRESENT , N, 0, 0 ^*/
    measparameters_v1020_ies_t          meas_parameters_v1020;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1020_MEAS_PARAMETERS_PRESENT , N, 0, 0 ^*/
/* CR 710 changes */
    U8                                  featuregroup_ind_r10[MAX_IND_OCTET_SIZE];
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1020_FEATUREGROUP_IND_R10_PRESENT , OCTET_STRING, FIXED ^*/
/* CR 710 changes */
/* CR 597 changes */
    irat_parameters_cdma2000_v1020_t    inter_rat_parameters_cdma2000_v1020;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_INTER_RAT_PARAMS_CDMA2000_V1020_PRESENT , N, 0, 0 ^*/
/* CR 597 changes */
/* CR 713 changes start*/    
    irat_parameters_utra_tdd_v1020_t    inter_rat_parameters_utra_tdd_v1020;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_INTER_RAT_PARAMS_UTRA_TDD_V1020_PRESENT, N, 0, 0 ^*/

/*CR 919 changes start*/
    ue_eutra_capability_v1060_ies_t     ue_eutra_capability_v1060_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_v1060_IES_PRESENT, N, 0, 0 ^*/
/*CR 919 changes stop*/    
    ue_based_nw_perf_meas_params_t      ue_based_nw_perf_meas_params;
    /*^ O,RRC_RRM_UE_EUTRA_CAPABILITY_NW_PERF_MEAS_PARAMS_PRESENT, N, 0, 0 ^*/

/* CR 713 changes stop*/    
}ue_eutra_capability_v1020_ies_t;

/*CR 895 changes start*/
typedef struct 
{
    rrc_bitmask_t                      bitmask; /*^ BITMASK ^*/
#define RRC_RRM_UE_EUTRA_CAPABILITY_PHY_LAYER_PARAMETERS_R9_PRESENT     0x01
#define RRC_RRM_UE_EUTRA_CAPABILITY_FEATURE_GRP_INDICATORS_R9_PRESENT   0x02
#define RRC_RRM_UE_EUTRA_CAPABILITY_FEATURE_GRP_IND_R9ADD_PRESENT       0x04
#define RRC_RRM_UE_EUTRA_CAPABILITY_IRAT_PARAMETERS_GERAN_R9_PRESENT    0x08
#define RRC_RRM_UE_EUTRA_CAPABILITY_IRAT_PARAMETERS_UTRA_R9_PRESENT     0x10
#define RRC_RRM_UE_EUTRA_CAPABILITY_IRAT_PARAMETERS_CDMA2000_R9_PRESENT 0x20
#define RRC_RRM_UE_EUTRA_CAPABILITY_NEIGH_CELLSI_ACQUISITION_PARAMS_R9_PRESENT 0x40
    
    physical_layer_params_t                 phy_layer_parameters_r9;
/*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_PHY_LAYER_PARAMETERS_R9_PRESENT ,N, 0, 0 ^*/

    /*CSR 00058668 Fix start*/
    U8                                      feature_group_indicators_r9[4];
/*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_FEATURE_GRP_INDICATORS_R9_PRESENT ,OCTET_STRING, FIXED ^*/

    U8                                      feature_group_ind_rel9add_r9[4];
/*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_FEATURE_GRP_IND_R9ADD_PRESENT ,OCTET_STRING, FIXED ^*/
    /*CSR 00058668 Fix stop*/

    geran_t                                 inter_rat_parameters_geran_r9;
/*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_IRAT_PARAMETERS_GERAN_R9_PRESENT ,N, 0, 0 ^*/

    irat_parameters_utra_v920_t             inter_rat_parameters_utra_r9;
/*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_IRAT_PARAMETERS_UTRA_R9_PRESENT ,N, 0, 0 ^*/

    irat_parameters_cdma2000_v920_t         inter_rat_parameters_cdma2000_r9;
/*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_IRAT_PARAMETERS_CDMA2000_R9_PRESENT, N, 0, 0 ^*/

    neigh_cell_si_acquisition_parameters_r9_t   neigh_cellsi_acquisition_parameters_r9;
/*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_NEIGH_CELLSI_ACQUISITION_PARAMS_R9_PRESENT, N, 0, 0 ^*/

}ue_eutra_capability_addxdd_mode_r9_t;

/* CR 1007 changes start*/
typedef enum
{
    VOICEOVERPS_HS_UTRA_FDD_R9_SUPPORTED
}voice_over_ps_hs_utra_fdd_r9_et;

typedef enum
{
    VOICEOVERPS_HS_UTRA_TDD_R9_SUPPORTED
}voice_over_ps_hs_utra_tdd128_r9_et;

typedef enum
{
    SRVCC_FROM_UTRA_FDD_TO_UTRA_FDD_r9_SUPPORTED
}srvcc_from_utra_fdd_to_utra_fdd_r9_et;

typedef enum
{
    SRVCC_FROM_UTRA_FDD_TO_GERAN_R9_SUPPORTED
}srvcc_from_utra_fdd_to_geran_r9_et;

typedef enum 
{
    SRVCC_FROM_UTRA_TDD128_TO_UTRA_TDD128_R9_SUPPORTED
}srvcc_from_utra_tdd128_to_utra_tdd128_r9_et;

typedef enum
{
    SRVCC_FROM_UTRA_TDD128_TO_GERAN_R9_SUPPORTED
}srvcc_from_utra_tdd128_to_geran_r9_et;

typedef struct
{
    rrc_bitmask_t                      bitmask; /*^ BITMASK ^*/
#define VOICEOVERPS_HS_UTRA_FDD_R9_PRESENT                  0x01
#define VOICEOVERPS_HS_UTRA_TDD_R9_PRESENT                  0x02
#define SRVCC_FROM_UTRA_FDD_TO_UTRA_FDD_r9_PRESENT          0x04    
#define SRVCC_FROM_UTRA_FDD_TO_GERAN_R9_PRESENT             0x08
#define SRVCC_FROM_UTRA_TDD128_TO_UTRA_TDD128_R9_PRESENT    0x10
#define SRVCC_FROM_UTRA_TDD128_TO_GERAN_R9_PRESENT          0x20

    /*CSR 00058668 Fix start*/
    U8          voice_over_ps_hs_utra_fdd_r9;
/*^ O, VOICEOVERPS_HS_UTRA_FDD_R9_PRESENT, N, 0, 0 ^*/ /* voice_over_ps_hs_utra_fdd_r9_et */
    U8          voice_over_ps_hs_utra_tdd128_r9;
/*^ O, VOICEOVERPS_HS_UTRA_TDD_R9_PRESENT, N, 0, 0 ^*/ /* voice_over_ps_hs_utra_tdd128_r9_et */
    U8          srvcc_from_utra_fdd_to_utra_fdd_r9;
/*^ O, SRVCC_FROM_UTRA_FDD_TO_UTRA_FDD_r9_PRESENT, N, 0, 0 ^*/ /* srvcc_from_utra_fdd_to_utra_fdd_r9_et */
    U8          srvcc_from_utra_fdd_to_geran_r9;
/*^ O, SRVCC_FROM_UTRA_FDD_TO_GERAN_R9_PRESENT, N, 0, 0 ^*/ /* srvcc_from_utra_fdd_to_geran_r9_et */
    U8          srvcc_from_utra_tdd128_to_utra_tdd128_r9;
/*^ O, SRVCC_FROM_UTRA_TDD128_TO_UTRA_TDD128_R9_PRESENT, N, 0, 0 ^*/ /* srvcc_from_utra_tdd128_to_utra_tdd128_r9_et */
    U8          srvcc_from_utra_tdd128_to_geran_r9;
/*^ O, SRVCC_FROM_UTRA_TDD128_TO_GERAN_R9_PRESENT, N, 0, 0 ^*/ /*srvcc_from_utra_tdd128_to_geran_r9_et */     
    /*CSR 00058668 Fix stop*/

}irat_parameters_utra_v9c0_t;

typedef struct
{
    rrc_bitmask_t                      bitmask; /*^ BITMASK ^*/
#define RRC_RRM_UE_EUTRA_CAPABILITY_IRAT_PARAMS_UTRA_v9c0_PRESENT   0x01

    irat_parameters_utra_v9c0_t        inter_rat_parameters_utra_v9c0;
/*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_IRAT_PARAMS_UTRA_v9c0_PRESENT, N, 0, 0 ^*/    
    
}ue_eutra_capability_v9c0_ies_t;
/* CR 1007 changes stop*/

typedef struct 
{
    rrc_bitmask_t                      bitmask; /*^ BITMASK ^*/
#define RRC_RRM_UE_EUTRA_CAPABILITY_FEATURE_GRP_IND_REL9_ADD_PRESENT 0x01
#define RRC_RRM_UE_EUTRA_CAPABILITY_ADDFDD_MODE_R9_PRESENT           0x02
#define RRC_RRM_UE_EUTRA_CAPABILITY_ADDTDD_MODE_R9_PRESENT           0x04
#define RRC_RRM_UE_EUTRA_CAPABILITY_V9c0_IES_PRESENT                 0x08    

    /*CSR 00058668 Fix start*/
    U8      feature_group_ind_rel9add_r9[4];
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_FEATURE_GRP_IND_REL9_ADD_PRESENT,OCTET_STRING, FIXED^*/
    /*CSR 00058668 Fix stop*/
    
    ue_eutra_capability_addxdd_mode_r9_t    fdd_add_ue_eutra_capabilities_r9;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_ADDFDD_MODE_R9_PRESENT ^*/

    ue_eutra_capability_addxdd_mode_r9_t    tdd_add_ue_eutra_capabilities_r9;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_ADDTDD_MODE_R9_PRESENT ^*/
    
    /* CR 1007 changes start*/
    ue_eutra_capability_v9c0_ies_t          ue_eutra_capability_v9c0_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V9c0_IES_PRESENT ^*/
    /* CR 1007 changes stop*/

}ue_eutra_capability_v9a0_ies_t;

/*CR 895 changes stop*/

typedef struct
{
    U16                   bitmask;                    /*^ BITMASK ^*/
#define RRC_RRM_UE_EUTRA_CAPABILITY_V1020_IES_PRESENT     0x01
/*CR 895 changes start*/
#define RRC_RRM_UE_EUTRA_CAPABILITY_V9a0_IES_PRESENT      0x02

    ue_eutra_capability_v9a0_ies_t         ue_eutra_capability_v9a0_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V9a0_IES_PRESENT , N, 0, 0 ^*/
/*CR 895 changes stop*/
    ue_eutra_capability_v1020_ies_t        ue_eutra_capability_v1020_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1020_IES_PRESENT , N, 0, 0 ^*/
}ue_eutra_capability_v940_ies_t;
/*CR 530,543 changes */

typedef struct
{
    U16                   bitmask;                    /*^ BITMASK ^*/
#define RRC_RRM_INTER_RAT_PARAMS_UTRA_V920_PRESENT       0x01
#define RRC_RRM_DEVICE_TYPE_R9_PRESENT                   0x02 
#define RRC_RRM_INTER_RAT_PARAMS_CDMA2000_V920_PRESENT   0x04 
/*CR 530,543 changes */
#define RRC_RRM_UE_EUTRA_CAPABILITY_V940_IES_PRESENT     0x08 
/*CR 530,543 changes */
    phy_layer_parameters_v920_t phy_layer_parameters_v920;
    /*^ M, 0, N, 0, 0 ^*/


    irat_parameters_geran_v920_t inter_rat_parameters_geran_v920;
    /*^ M, 0, N, 0, 0 ^*/
   
    irat_parameters_utra_v920_t inter_rat_parameters_utra_v920;
    /*^ O, RRC_RRM_INTER_RAT_PARAMS_UTRA_V920_PRESENT, N, 0, 0 ^*/

/* CDMA2000_CSFB_HO start */
    irat_parameters_cdma2000_v920_t  inter_rat_parameters_cdma2000_v920;
    /*^ O, RRC_RRM_INTER_RAT_PARAMS_CDMA2000_V920_PRESENT, N, 0, 0 ^*/
/* CDMA2000_CSFB_HO stop */
    
    csg_proximity_indication_parameters_r9_t csg_proximity_indication_parameters_r9;
    /*^ M, 0, N, 0, 0 ^*/

    neigh_cell_si_acquisition_parameters_r9_t neigh_cell_si_acquisition_parameters_r9;
    /*^ M, 0, N, 0, 0 ^*/

    son_parameters_r9_t son_parameters_r9;
    /*^ M, 0, N, 0, 0 ^*/

    /*CSR 00058668 Fix start*/
    U8 device_type_r9;
    /*^ O, RRC_RRM_DEVICE_TYPE_R9_PRESENT ,N,0,0^*/ /* device_type_r9_et  */
    /*CSR 00058668 Fix stop*/

    /*CR 530,543 changes */
    ue_eutra_capability_v940_ies_t        ue_eutra_capability_v940_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V940_IES_PRESENT,N,0,0 ^*/
    /*CR 530,543 changes */
}ue_eutra_capability_v920_ies_t;

typedef struct
{
    U16                   bitmask;                    /*^ BITMASK ^*/
#define UE_EUTRA_CAPABILITY_V920_IES_PRESENT          0x01

    eutra_radio_capability_info_def_t   eutra_radio_capability_info_def;
/*^ M, 0, N, 0, 0 ^*/


    ue_eutra_capability_v920_ies_t        ue_eutra_capability_v920_ies;
    /*^ O, UE_EUTRA_CAPABILITY_V920_IES_PRESENT,N,0,0 ^*/

}eutra_radio_capability_info_t;

typedef struct
{
/*SPR_17664_start*/
    U32          len_utra_radio_capability_info ; /*^ M,0,H,0,MAX_ASN_BUFFER ^*/  
/*SPR_17664_end*/
U8           utra_radio_capability_asn_buff[MAX_ASN_BUFFER]; 
    /*^ M,0,OCTET_STRING,VARIABLE ^*/   
}utra_radio_capability_info_t;

typedef struct
{
/*SPR_17664_start*/
    U32          len_cdma2000_radio_capability_info ; /*^ M,0,H,0,MAX_ASN_BUFFER ^*/  
/*SPR_17664_end*/
    U8           cdma2000_radio_capability_asn_buff[MAX_ASN_BUFFER]; 
    /*^ M,0,OCTET_STRING,VARIABLE ^*/   
}cdma2000_radio_capability_info_t;

typedef struct
{
/*SPR_17664_start*/
    U32          len_geran_cs_radio_capability_info ; /*^ M,0,H,0,MAX_ASN_BUFFER ^*/  
/*SPR_17664_end*/
    U8           geran_cs_radio_capability_asn_buff[MAX_ASN_BUFFER]; 
    /*^ M,0,OCTET_STRING,VARIABLE ^*/   
}geran_cs_radio_capability_info_t;

typedef struct
{
/*SPR_17664_start*/
    U32      len_geran_ps_radio_capability_info ; /*^ M,0,H,0,MAX_ASN_BUFFER ^*/  
/*SPR_17664_end*/
    U8       geran_ps_radio_capability_asn_buff[MAX_ASN_BUFFER]; 
    /*^ M,0,OCTET_STRING,VARIABLE ^*/   
}geran_ps_radio_capability_info_t;

/*NR_DC Code Change Start*/
typedef enum
{
    NR_RRC_A = 0,
    NR_RRC_B = 1,
    NR_RRC_C = 2,
    NR_RRC_D = 3,
    NR_RRC_E = 4,
    NR_RRC_F = 5,
    NR_RRC_G = 6,
    NR_RRC_H = 7,
    NR_RRC_I = 8,
    NR_RRC_J = 9,
    NR_RRC_K = 10,
    NR_RRC_L = 11,
    NR_RRC_M = 12,
    NR_RRC_N = 13,
    NR_RRC_O = 14,
    NR_RRC_P = 15,
    NR_RRC_Q = 16
}ca_bandwidth_class_nr_et;

typedef enum
{
    NR_RRC_A_1 = 0,
    NR_RRC_B_1 = 1,
    NR_RRC_C_1 = 2,
    NR_RRC_D_1 = 3,
    NR_RRC_E_1 = 4,
    NR_RRC_F_1 = 5
}ca_bandwidth_class_eutra_et;

typedef struct
{
    U16     bitmask;        /*^ BITMASK ^*/
#define BAND_PARAMETERS_CA_BANDWIDTH_CLASS_DL_PRESENT  0x01
#define BAND_PARAMETERS_CA_BANDWIDTH_CLASS_UL_PRESENT  0x02

    U16     band_eutra;
/*^ M, 0, B, 1, MAX_EUTRA_FREQ_BANDS ^*/

    U32     ca_bandwidth_class_dl_eutra; /*ca_bandwidth_class_eutra_et*/
/*^ O, BAND_PARAMETERS_CA_BANDWIDTH_CLASS_DL_PRESENT,N,0,0 ^*/

    U32     ca_bandwidth_class_ul_eutra; /*ca_bandwidth_class_eutra_et*/
/*^ O, BAND_PARAMETERS_CA_BANDWIDTH_CLASS_UL_PRESENT,N,0,0 ^*/

}band_parameters_eutra_t;

typedef struct
{
    U16     bitmask;        /*^ BITMASK ^*/
#define BAND_PARAMETERS_CA_BANDWIDTH_CLASS_DL_NR_PRESENT  0x01
#define BAND_PARAMETERS_CA_BANDWIDTH_CLASS_UL_NR_PRESENT  0x02

    U16     band_nr;
/*^ M, 0, B, 1, MAX_NR_FREQ_BANDS ^*/

    U32     ca_bandwidth_class_dl_nr; /*ca_bandwidth_class_nr_et*/
/*^ O, BAND_PARAMETERS_CA_BANDWIDTH_CLASS_DL_NR_PRESENT,N,0,0 ^*/

    U32     ca_bandwidth_class_ul_nr; /*ca_bandwidth_class_nr_et*/
/*^ O, BAND_PARAMETERS_CA_BANDWIDTH_CLASS_UL_NR_PRESENT,N,0,0 ^*/

}band_parameters_nr_t;

typedef struct
{
    U16                 bitmask;        /*^ BITMASK ^*/
    /*Choice - set only one bit*/
#define BAND_PARAMETERS_EUTRA_PRESENT       0x01
#define BAND_PARAMETERS_NR_PRESENT          0x02

    band_parameters_eutra_t eutra;
    /*^ O, BAND_PARAMETERS_EUTRA_PRESENT,N,0,0 ^*/

    band_parameters_nr_t    nr;
    /*^ O, BAND_PARAMETERS_NR_PRESENT,N,0,0 ^*/

} band_parameters_t;

typedef struct
{
    U16                count;
/*^ M, 0, H, 0, MAX_SIMULTANEOUS_BANDS_EUTRA_NR ^*/
    band_parameters_t  band[MAX_SIMULTANEOUS_BANDS_EUTRA_NR]; 
/*^ M, 0, OCTET_STRING, VARIABLE ^*/

}band_combination_band_list_t;

typedef struct
{
    band_combination_band_list_t
                        band_combination_band_list;
/*^ M, 0, N, 0, 0 ^*/

    U16                 feature_set_combination;
/*^ M, 0, N, 0, 0 ^*/
}band_combination_t;

typedef struct
{
    U16         count;
/*^ M, 0, H, 0, MAX_BAND_COMB_EUTRA_NR ^*/
    band_combination_t band_combination[MAX_BAND_COMB_EUTRA_NR]; 
/*^ M, 0, OCTET_STRING, VARIABLE ^*/
}supported_band_comb_list_t;

typedef struct
{
    U16                             bitmask;                        /*^ BITMASK ^*/
#define RF_PARAMETERS_MRDC_SUPPORTED_BAND_COMB_LIST_PRESENT      0x01

    supported_band_comb_list_t supported_band_comb_list;
/*^ O, RF_PARAMETERS_MRDC_SUPPORTED_BAND_COMB_LIST_PRESENT,N,0,0 ^*/

}rf_parameters_mrdc_t;

typedef struct
{
    rf_parameters_mrdc_t   rf_parameters_mrdc;
    /*^ M, 0, N, 0, 0 ^*/
}eutra_nr_radio_capability_info_t;
/*NR_DC Code Change Stop*/

#define RRC_NR_SUPPORTED_GAP_PATTERN_PRESENT    0x01
typedef struct
{
    rrc_bitmask_t    bitmask;
    /*^ BITMASK ^*/

    U32              supported_gap_pattern;
    /*^ O, RRC_NR_SUPPORTED_GAP_PATTERN_PRESENT, N, 0, 0^*/
}nr_radio_capability_info_t;

typedef struct
{
    U16                   bitmask;                    /*^ BITMASK ^*/

#define EUTRA_RADIO_CAPABILITY_PRESENT      0x01
#define UTRA_RADIO_CAPABILITY_PRESENT       0x02
#define CDMA2000_RADIO_CAPABILITY_PRESENT   0x04
#define GERAN_CS_RADIO_CAPABILITY_PRESENT   0x08
#define GERAN_PS_RADIO_CAPABILITY_PRESENT   0x10
/*NR_DC Code Change Start*/
#define EURTA_NR_RADIO_CAPABILITY_PRESENT   0x20
/*NR_DC Code Change Stop*/
#define NR_RADIO_CAPABILITY_PRESENT         0x40

    eutra_radio_capability_info_t eutra_radio_capability_info; 
/*^ O, EUTRA_RADIO_CAPABILITY_PRESENT,N,0,0 ^*/

    utra_radio_capability_info_t        utra_radio_capability_info; 
/*^ O, UTRA_RADIO_CAPABILITY_PRESENT,N,0,0 ^*/

    cdma2000_radio_capability_info_t    cdma2000_radio_capability_info; 
/*^ O, CDMA2000_RADIO_CAPABILITY_PRESENT,N,0,0 ^*/

    geran_cs_radio_capability_info_t    geran_cs_radio_capability_info; 
/*^ O, GERAN_CS_RADIO_CAPABILITY_PRESENT,N,0,0 ^*/

    geran_ps_radio_capability_info_t    geran_ps_radio_capability_info; 
/*^ O, GERAN_PS_RADIO_CAPABILITY_PRESENT,N,0,0 ^*/

/*NR_DC Code Change Start*/
    eutra_nr_radio_capability_info_t    eutra_nr_radio_capability_info; 
/*^ O, EURTA_NR_RADIO_CAPABILITY_PRESENT,N,0,0 ^*/
/*NR_DC Code Change Stop*/

    nr_radio_capability_info_t          nr_radio_capability_info; 
/*^ O, NR_RADIO_CAPABILITY_PRESENT,N,0,0 ^*/

}rrc_radio_capability_info_t;

typedef struct
{
    U16            bitmask;                    /*^ BITMASK ^*/
#define RADIO_CAPABILITY_INFO_PRESENT   0x01

    U16      ue_Index;
    U8        result;       /*^ M, 0, H, 0, 1 ^*/   /* rrc_return_et */
    rrc_radio_capability_info_t    radio_capability_info;
    /*^ O, RADIO_CAPABILITY_INFO_PRESENT,N,0,0 ^*/
}rrc_rrm_ue_capability_enquiry_resp_t;
/*^ API, RRC_RRM_UE_CAPABILITY_ENQUIRY_RESP ^*/


typedef struct
{
    U16                                    ue_Index;
    rrc_radio_capability_info_t    radio_capability_info;
    /*^ M, RADIO_CAPABILITY_INFO_PRESENT,N,0,0 ^*/
}rrc_rrm_ue_capability_ind_t;
/*^ API, RRC_RRM_UE_CAPABILITY_IND ^*/


/******************************************************************************
        RRC_RRM_HO_REQUIRED
******************************************************************************/
typedef enum
{
        STANDARD_UMTS_INTEGRITY_ALGO_1,
        STANDARD_UMTS_INTEGRITY_ALGO_2,
        NO_VALUE
}rrc_standard_umts_integrity_algo_et;


typedef enum
{
        NO_ENCRYPTION,
        STANDARD_UMTS_ENCRYPTION_ALGO_1,
        STANDARD_UMTS_ENCRYPTION_ALGO_2
}rrc_standard_umts_encryption_algo_et;


typedef enum
{
        UE_NOT_INVOLVED,
        UE_INVOLVED
}rrc_relocation_type_et;


typedef enum
{
        CS_DOMAIN,
        PS_DOMAIN 
}rrc_cn_domain_indicator_et;

typedef struct
{
        plmn_identity_t                    plmn_identity;    
        U8                                 cell_identity[4];  /*^ M, 0, OCTET_STRING, FIXED ^*/  
}cell_global_id_UTRA_t;



typedef struct _rrc_erab_list_t
{
	/* SPR_17664_start*/
    /* Coverity Fix 99932 Start */
        U8         erab_count;    /*^ M, 0, B, 1, MAX_ERAB_COUNT ^*/
        U8         erab_id[MAX_ERAB_COUNT];  /*^ M,0,OCTET_STRING,VARIABLE ^*/  
    /* Coverity Fix 99932 End */
	/* SPR_17664_end*/
}rrc_erab_list_t;

/*CR 764 changes start*/
typedef struct  _rrc_rrm_tansport_layer_address_t 
{
    U8      length; 
/*SPR_17664_start*/
    /*^ M, 0, H, 0, MAX_TRANSPORT_LAYER_ADDR_SIZE  ^*/
/*SPR_17664_end*/
    U8      data[MAX_TRANSPORT_LAYER_ADDR_SIZE];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrc_rrm_tansport_layer_address_t;
/*CR 764 changes stop*/


typedef struct _rrm_drb_sub_to_fwd_info_t
{
    U8 forward_direction; /*^ M, 0, H, 0, 2 ^*/ /*rrm_forwarding_direction_et*/
    U8 erab_id;  /*^ M, 0, H, 0, 15 ^*/
    /*CR 764 changes start*/
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define ERAB_SUB_TO_FORWARD_LIST_UL_GTP_TUNNEL_END_POINT_PRESENT    0x01
#define ERAB_SUB_TO_FORWARD_LIST_DL_GTP_TUNNEL_END_POINT_PRESENT    0x02
    
    rrc_rrm_tansport_layer_address_t    ul_transport_layer_address;
/*^ O, ERAB_SUB_TO_FORWARD_LIST_UL_GTP_TUNNEL_END_POINT_PRESENT ^*/

    rrc_rrm_tansport_layer_address_t    dl_transport_layer_address;
/*^ O, ERAB_SUB_TO_FORWARD_LIST_DL_GTP_TUNNEL_END_POINT_PRESENT ^*/

    /*CR 764 changes stop*/    
}rrm_drb_sub_to_fwd_info_t;


typedef struct _rrc_erab_info_list_t
{
/* SPR_18490 start */
        U8         erab_count;    /*^ M, 0, B, 1, 8 ^*/
        rrm_drb_sub_to_fwd_info_t        drb_sub_to_fwd[MAX_ERAB_COUNT];  /*^ M,0,OCTET_STRING,VARIABLE ^*/  
/* SPR_18490 stop */
}rrc_erab_info_list_t;


typedef struct  _target_cell_identity_t
{
    U8    target_cell_Id[CELL_ID_OCTET_SIZE];     /*^ M, 0, OCTET_STRING, FIXED ^*/
}target_cell_identity_t;

typedef struct _target_cell_identity_list_t
{
/*SPR_17664_start*/
    U8    target_cell_count;  /*^ M, 0, B, 1, MAX_NUM_TARGET_CELL ^*/
/*SPR_17664_end*/
    target_cell_identity_t    target_cell_identity[MAX_NUM_TARGET_CELL]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}target_cell_identity_list_t;

typedef struct _rrm_rrc_container_t
{
        U16                                bitmask; /*^ BITMASK ^*/
#define RRM_RRC_CONTAINER_TARGET_CELL_IDENTITY_LIST_PRESENT    0X01
#define RRM_RRC_CONTAINER_RRM_CONFIG_PRESENT                   0X02

        target_cell_identity_list_t    target_cell_identity_list; 
/*^ O, RRM_RRC_CONTAINER_TARGET_CELL_IDENTITY_LIST_PRESENT, N, 0, 0 ^*/

        rrm_config_t                  rrm_config;    
        /*^ O, RRM_RRC_CONTAINER_RRM_CONFIG_PRESENT, N, 0, 0 ^*/
}rrm_rrc_container_t;

typedef struct
{
    U8 cell_size; /*^ M, 0, H, 0, 3 ^*//*rrc_utran_cell_size_et*/
}rrc_utran_type_t;

typedef struct
{
        cell_global_id_utra_t    utran_cell_id; /*^ M, 0, N, 0, 0 ^*/
        rrc_utran_type_t         cell_type; /*^ M, 0, N, 0, 0 ^*/
        U16                      time_ue_stayed_in_cell; /*^ M, 0, H, 0, 4095 ^*/
}last_visited_utran_cell_information_t;

typedef struct
{
    U8 cell_size; /*^ M, 0, H, 0, 3 ^*//*rrc_eutran_cell_size_et*/
}rrc_eutran_type_t;

typedef struct
{
        cell_global_id_eutra_t   global_cell_id; /*^ M, 0, N, 0, 0 ^*/
        rrc_eutran_type_t        cell_type; /*^ M, 0, N, 0, 0 ^*/
        U16                             time_ue_stayed_in_cell; /*^ M, 0, H, 0, 4095 ^*/
}last_visited_e_utran_cell_information_t;


#define MAX_NO_OF_CELLS_UE_HISTORY 16

typedef struct
{
        U16                                bitmask; /*^ BITMASK ^*/
#define LAST_VISITED_CELL_INFO_E_UTRAN_CELL_INFORMATION_PRESENT 0X01
#define LAST_VISITED_CELL_INFO_UTRAN_CELL_INFORMATION_PRESENT 0X02

        last_visited_e_utran_cell_information_t    last_visited_e_utran_cell_information; 
/*^ O, LAST_VISITED_CELL_INFO_E_UTRAN_CELL_INFORMATION_PRESENT, N, 0, 0 ^*/

        last_visited_utran_cell_information_t       last_visited_utran_cell_information; 
/*^ O, LAST_VISITED_CELL_INFO_UTRAN_CELL_INFORMATION_PRESENT, N, 0, 0 ^*/
        
        /*GERAN cell info is NULL in 36.413 rel8.8 spec*/
}last_visited_cell_info_t;
    
typedef struct
{
/*SPR_17664_start*/
        U8 count; /*^ M, 0, B, 1, MAX_NO_OF_CELLS_UE_HISTORY ^*/
/*SPR_17664_end*/
        last_visited_cell_info_t last_visited_cell_info[MAX_NO_OF_CELLS_UE_HISTORY]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}last_visited_cell_list_t;

typedef struct
{
        last_visited_cell_list_t last_visited_cell_list; /*^ M, 0, N, 0, 0 ^*/
}ue_history_t;


#define MAX_SUB_PROFILE_ID  256

typedef struct
{
        U16                                bitmask; /*^ BITMASK ^*/  
#define SOURCE_ENB_TO_TARGET_ENB_TRANSPARENT_CONTAINER_ERAB_DL_FWDING_LIST_PRESENT   0X01
#define SOURCE_ENB_TO_TARGET_ENB_TRANSPARENT_CONTAINER_SUB_PROFILE_ID_PRESENT    0X02

        rrm_rrc_container_t  rrc_container; /*^ M, 0, N, 0, 0 ^*/
        rrc_erab_list_t          erab_dl_fwding_list; 
/*^ O, SOURCE_ENB_TO_TARGET_ENB_TRANSPARENT_CONTAINER_ERAB_DL_FWDING_LIST_PRESENT, N, 0, 0 ^*/ 
        ue_history_t         ue_histroy; /*^ M, 0, N, 0, 0 ^*/
        U16                  sub_profile_id; /*^ O, SOURCE_ENB_TO_TARGET_ENB_TRANSPARENT_CONTAINER_SUB_PROFILE_ID_PRESENT, B, 1, 256 ^*/
}rrm_sourceenb_to_targetenb_transparent_cotainer_t;


typedef struct
{
        U16 bitmask; /*^ BITMASK ^*/
#define SECURITY_INFO_CHOSEN_INTEGRITY_PROTECTION_ALGO_PRESENT   0x01
#define SECURITY_INFO_INTEGRITY_PROTECTION_KEY_PRESENT           0x02
#define SECURITY_INFO_CHOSEN_ENCRYPTION_ALGO_PRESENT             0x04
#define SECURITY_INFO_CIPHERING_KEY_PRESENT                      0x08
#define SECURITY_INFO_CHOSEN_CS_ENCRYPTION_ALGO_PRESENT          0x10
#define SECURITY_INFO_CHOSEN_PS_ENCRYPTION_ALGO_PRESENT          0x20

        U8  chosen_integrity_protection_algo; 
/*^ O, SECURITY_INFO_CHOSEN_INTEGRITY_PROTECTION_ALGO_PRESENT, H, 0, 1 ^*/  /*rrc_standard_umts_integrity_algo_et*/

        U8  integration_protection_key[SECURITY_KEY_MAX_SIZE]; 
/*^ O, SECURITY_INFO_INTEGRITY_PROTECTION_KEY_PRESENT, OCTET_STRING, FIXED ^*/

        U8  chosen_encryption_algo; 
/*^ O, SECURITY_INFO_CHOSEN_ENCRYPTION_ALGO_PRESENT, H, 0, 2 ^*/ /*rrc_standard_umts_encryption_algo_et*/

        U8  ciphering_key[SECURITY_KEY_MAX_SIZE]; 
/*^ O, SECURITY_INFO_CIPHERING_KEY_PRESENT, OCTET_STRING, FIXED ^*/

        U8  chosen_cs_encryption_algorithm; 
/*^ O, SECURITY_INFO_CHOSEN_CS_ENCRYPTION_ALGO_PRESENT, H, 0, 2 ^*/ /*rrc_standard_umts_encryption_algo_et*/

        U8  chosen_ps_encryption_algorithm; 
/*^ O, SECURITY_INFO_CHOSEN_PS_ENCRYPTION_ALGO_PRESENT, H, 0, 2 ^*/ /*rrc_standard_umts_encryption_algo_et*/
}security_info_t;

typedef struct
{
        U16  bitmask; /*^ BITMASK ^*/
#define TRANSPORT_CHANNEL_ID_DCH_ID_PRESENT                0x01
#define TRANSPORT_CHANNEL_ID_DSCH_ID_PRESENT               0x02
#define TRANSPORT_CHANNEL_ID_USCH_ID_PRESENT               0x04
#define TRANSPORT_CHANNEL_ID_HS_DSCH_MAC_D_FLOW_ID_PRESENT 0x08
#define TRANSPORT_CHANNEL_ID_E_DCH_MAC_D_FLOW_ID_PRESENT   0x10

        U8   dch_id;  /*^ O, TRANSPORT_CHANNEL_ID_DCH_ID_PRESENT, N, 0, 0 ^*/
        U8   dsch_id; /*^ O, TRANSPORT_CHANNEL_ID_DSCH_ID_PRESENT, N, 0, 0 ^*/
        U8   usch_id; /*^ O, TRANSPORT_CHANNEL_ID_USCH_ID_PRESENT, N, 0, 0 ^*/
        U8   hs_dsch_mac_d_flow_id;
        /*^ O, TRANSPORT_CHANNEL_ID_HS_DSCH_MAC_D_FLOW_ID_PRESENT, H, 0, 7 ^*/
        U8   e_dch_mac_d_flow_id; 
        /*^ O, TRANSPORT_CHANNEL_ID_E_DCH_MAC_D_FLOW_ID_PRESENT, H, 0, 7 ^*/
}transport_channel_id_t;



typedef struct
{
        U8                       rab_id;                                  
	/* SPR_17664_start*/
        U8                       no_of_rab_subflow; /*^ M, 0, B, 1, MAX_NUM_SUB_FLOW_PER_RAB ^*/
	/* SPR_17664_end*/
	transport_channel_id_t   transport_channel_id[MAX_NUM_SUB_FLOW_PER_RAB]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
        U8    cn_domain_indicator; /*^ M, 0, H, 0, 1 ^*/ /*rrc_cn_domain_indicator_et*/
}rab_transport_channel_mapping_t;

typedef struct
{
        U8                       srb_id; /*^ M, 0, N, 0, 0 ^*/
        transport_channel_id_t   srb_transport_channel_id; /*^ M, 0, N, 0, 0 ^*/
}srb_transport_channel_mapping_t;

typedef struct
{
        U8    trace_recording_length; /*^ M, 0, B, 2, 3 ^*/
        U8    trace_reference[3]; /*^ M,0,OCTET_STRING,VARIABLE ^*/
        U16   trace_recording_session_reference; /*^ M, 0, N, 0, 0 ^*/
}trace_recording_session_info_t; 

typedef struct
{
        U8                             nonce[16];  /*^ M, 0, OCTET_STRING, FIXED ^*/
}srvcc_information_t;

typedef enum
{
    UE_JOINED_MULTICASE_SERVICES
}rrc_mbms_linking_info_et;

typedef enum
{
   CSFB,
   CSFB_HIGH_PRIORITY
}rrc_csfb_info_et;

/*srvcc-u start*/
typedef enum
{
   ALLOWED
}rrc_mgmt_based_mdt_allowed_et;

typedef struct
{
    U16  bitmask; /*^ BITMASK ^*/
#define E_UTRA_FREQ_MEASUREMENT_BAND_WIDTH_PRESENT          0x01 
    U16  e_arfcn;/*^ M, 0, N, 0, 0 ^*/
    U8   measurement_bandwidth;
    /*^ O, E_UTRA_FREQ_MEASUREMENT_BAND_WIDTH_PRESENT, H, 0, 5 ^*//*rrm_band_width_et*/
}eutra_freq_t;

typedef struct
{
/*SPR_17664_start*/
    U8 count; /*^ M, 0, B, 1, MAX_EUTRA_CARRIER_FREQ^*/
/*SPR_17664_end*/
    eutra_freq_t  eutra_freq[MAX_EUTRA_CARRIER_FREQ]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}eutra_freq_list_t;

typedef struct
{
    U16   bitmask; /*^ BITMASK ^*/
#define IRAT_MEAS_PARAM_EUTRA_FREQ_LIST_PRESENT          0x01 
    U8    meas_duration;/*^ M, 0, B, 1, 100 ^*/
    eutra_freq_list_t  eutra_freq_list;
/*^ O, IRAT_MEAS_PARAM_EUTRA_FREQ_LIST_PRESENT, N, 0, 0 ^*/ 
}irat_meas_param_t;

typedef struct
{
    U16   bitmask; /*^ BITMASK ^*/
#define IRAT_MEAS_CONFIG_PARAM_RSRP_PRESENT          0x01 
#define IRAT_MEAS_CONFIG_PARAM_RSRQ_PRESENT          0x02 
    U8    rsrp;         
    /*^ O, IRAT_MEAS_CONFIG_PARAM_RSRP_PRESENT, H, 0, 97 ^*/
    U8    rsrq;
    /*^ O, IRAT_MEAS_CONFIG_PARAM_RSRQ_PRESENT, H, 0, 34 ^*/
    irat_meas_param_t  irat_meas_param; /*^ M, 0, N, 0, 0 ^*/ 
}irat_meas_config_t;
/*srvcc-u stop*/

typedef struct
{
        U16                                bitmask; /*^ BITMASK ^*/
#define RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_SECURITY_INFO_PRESENT   0x01
#define RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_D_RNTI_PRESENT          0x02
#define RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_TARGET_CELL_ID_PRESENT  0x04
#define RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_RAB_TRANSPORT_CH_MAP_PRESENT   0x08
#define RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_SRB_TRANSPORT_CH_MAP_PRESENT    0x10
#define RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_TRACE_REC_SESSION_INFO_PRESENT 0x20
#define RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_MBMS_LINKING_INFO_PRESENT           0x40
#define RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_DRNTI_FOR_NO_UI_CS_UP_PRESENT   0x80
#define RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_SUB_PROF_ID_FOR_RAT_OR_FREQ_PRI_PRESENT   0x100
#define RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_SRVCC_INFO_PRESENT   0x200
#define RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_PS_RAB_TO_BE_REPLACED_PRESENT   0x400
#define RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_CSFB_INFO_PRESENT   0x800
#define RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_IRAT_MEAS_CONFIG_INFO_PRESENT   0x1000
#define RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_MGMT_BASED_MDT_ALLOWED_PRESENT  0x2000

        U8  number_of_Iu_instances;  /*^ M, 0, B, 1, 2 ^*/
        U8  relocation_type; /*^ M, 0, H, 0, 1 ^*/ /*rrc_relocation_type_et*/
        security_info_t  security_info; 
        /*^ O, RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_SECURITY_INFO_PRESENT, N, 0, 0 ^*/
        
        U32 d_rnti; 
        /*^ O, RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_D_RNTI_PRESENT, H, 0, 1048575 ^*/
        
/*SPR 19090 Fix Start*/        
        U32  target_cell_id;
        /*^ O, RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_TARGET_CELL_ID_PRESENT, H, 0, 268435455 ^*/
/*SPR 19090 Fix End*/        
        
        U8  no_of_rab_transport_channel_mappings; /*^ M, 0, N, 0, 255 ^*/
        rab_transport_channel_mapping_t  rab_transport_channel_mapping[MAX_NUM_RAB_PER_UE];
             /*^ O, RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_RAB_TRANSPORT_CH_MAP_PRESENT, OCTET_STRING, VARIABLE ^*/ 
        
        U8  no_of_srb_transport_channel_mappings; /*^ M, 0, H, 0, 8 ^*/
        srb_transport_channel_mapping_t  srb_transport_channel_mapping[MAX_NUM_SRB_PER_UE]; 
             /*^ O, RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_SRB_TRANSPORT_CH_MAP_PRESENT, OCTET_STRING, VARIABLE ^*/ 
        
        trace_recording_session_info_t      trace_recording_session_info; 
        /*^ O, RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_TRACE_REC_SESSION_INFO_PRESENT, N, 0, 0 ^*/
        
        U8   mbms_linking_info; 
        /*^ O, RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_MBMS_LINKING_INFO_PRESENT, H, 0, 0 ^*/ /*rrc_mbms_linking_info_et*/
        
        U32 d_rnti_for_no_ui_cs_up; 
        /*^ O, RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_DRNTI_FOR_NO_UI_CS_UP_PRESENT, H, 0, 1048575 ^*/
        
        ue_history_t    ue_histroy_info; /*^ M, 0, N, 0, 0 ^*/
        U16  subscriber_profile_id_for_rat_or_frequency_priority; 
        /*^ O, RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_SUB_PROF_ID_FOR_RAT_OR_FREQ_PRI_PRESENT, B, 1, 256 ^*/
        
        srvcc_information_t   srvcc_info; 
        /*^ O, RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_SRVCC_INFO_PRESENT, N, 0, 0 ^*/
        
        U8  ps_rab_to_be_replaced;   
        /*^ O, RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_PS_RAB_TO_BE_REPLACED_PRESENT, N, 0, 0 ^*/
        U8  csfb_info;   
        /*^ O,RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_CSFB_INFO_PRESENT,H, 0, 1 ^*/ /*rrc_csfb_info_et*/
        /*srvcc-u start*/
        irat_meas_config_t   irat_meas_config;
        /*^ O, RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_IRAT_MEAS_CONFIG_INFO_PRESENT, N, 0, 0 ^*/

        U8  mgmt_based_mdt_allowed;
        /*^ O, RRM_SOURCE_RNC_TO_TARGET_RNC_TRANSPARENT_CONTAINER_MGMT_BASED_MDT_ALLOWED_PRESENT, H, 0, 0 ^*//*rrc_mgmt_based_mdt_allowed_et*/
        /*srvcc-u stop*/
}rrm_source_rnc_to_target_rnc_transparent_container_t;

/* SPR 20635 Fix - SRVCC to GERAN - Starts */
typedef struct
{
    U8     bitmask; /*^ BITMASK ^*/
#define RRM_BSS_EXTRA_INFORMATION_PRE_EMPTION_RECOMMEDATION_PRESENT 0x01 
#define RRM_BSS_EXTRA_INFORMATION_LCS_INFO_PRESENT                  0x02
#define RRM_BSS_EXTRA_INFORMATION_UMTS_HO_SUPPORT_PRESENT           0x04
}rrm_old_bss_to_new_bss_extra_information_t;

typedef struct
{
    U8                  reporting_threshold;    /*^ M, 0, N, 0, 0 ^*/
    U8                  rep_quant;              /*^ M, 0, H, 0, 1 ^*/
    irat_meas_param_t   irat_meas_param;        /*^ M, 0, N, 0, 0 ^*/ 
}rrm_old_bss_to_new_bss_irat_meas_config_t;

typedef struct
{
    U16     bitmask; /*^ BITMASK ^*/
#define RRM_SOURCE_BSS_TO_TARGET_BSS_TRANSPARENT_CONTAINER_PAGE_MODE_PRESENT        0x01 
#define RRM_SOURCE_BSS_TO_TARGET_BSS_TRANSPARENT_CONTAINER_CONTAINER_ID_PRESENT     0x02
#define RRM_SOURCE_BSS_TO_TARGET_BSS_TRANSPARENT_CONTAINER_GLOBAL_TFI_PRESENT       0x04
#define RRM_SOURCE_BSS_TO_TARGET_BSS_TRANSPARENT_CONTAINER_PS_INDICATION_PRESENT    0x08
#define RRM_SOURCE_BSS_TO_TARGET_BSS_TRANSPARENT_CONTAINER_CS_INDICATION_PRESENT    0x10 
#define RRM_SOURCE_BSS_TO_TARGET_BSS_TRANSPARENT_CONTAINER_IRAT_MEAS_CONFIG_PRESENT 0x20 

    U8      page_mode;
    /*^ O, RRM_SOURCE_BSS_TO_TARGET_BSS_TRANSPARENT_CONTAINER_PAGE_MODE_PRESENT, H, 0, 3 ^*/
    U8      container_id;
    /*^ O, RRM_SOURCE_BSS_TO_TARGET_BSS_TRANSPARENT_CONTAINER_CONTAINER_ID_PRESENT, H, 0, 3 ^*/
    U8      global_tfi;
    /*^ O, RRM_SOURCE_BSS_TO_TARGET_BSS_TRANSPARENT_CONTAINER_GLOBAL_TFI_PRESENT, H, 0, 63 ^*/
    U8      ps_handover_indication;
    /*^ O, RRM_SOURCE_BSS_TO_TARGET_BSS_TRANSPARENT_CONTAINER_PS_INDICATION_PRESENT, H, 0, 1 ^*/
    U8      cs_indication;
    /*^ O, RRM_SOURCE_BSS_TO_TARGET_BSS_TRANSPARENT_CONTAINER_CS_INDICATION_PRESENT, N, 0, 0 ^*/
    rrm_old_bss_to_new_bss_irat_meas_config_t           irat_meas_config;
    /*^ O, RRM_SOURCE_BSS_TO_TARGET_BSS_TRANSPARENT_CONTAINER_IRAT_MEAS_CONFIG_PRESENT, N, 0, 0 ^*/
}rrm_source_bss_to_target_bss_transparent_container_t;

typedef struct
{
    U16     bitmask; /*^ BITMASK ^*/
#define RRM_OLD_BSS_TO_NEW_BSS_INFO_EXTRA_INFO_PRESENT              0x01
#define RRM_OLD_BSS_TO_NEW_BSS_INFO_TARGET_CELL_RADIO_INFO_PRESENT  0x02
#define RRM_OLD_BSS_TO_NEW_BSS_INFO_PS_INDICATION_PRESENT           0x04
#define RRM_OLD_BSS_TO_NEW_BSS_INFO_IRAT_MEAS_CONFIG_INFO_PRESENT   0x08

    rrm_old_bss_to_new_bss_extra_information_t          extra_information; 
    /*^ O, RRM_OLD_BSS_TO_NEW_BSS_INFO_EXTRA_INFO_PRESENT, N, 0, 0 ^*/
    U8                                                  target_cell_rxlev; 
    /*^ O, RRM_OLD_BSS_TO_NEW_BSS_INFO_TARGET_CELL_RADIO_INFO_PRESENT, N, 0, 0 ^*/
    U8                                                  old_bss_to_new_bss_ps_indication; 
    /*^ O, RRM_OLD_BSS_TO_NEW_BSS_INFO_PS_INDICATION_PRESENT, N, 0, 0 ^*/
    rrm_old_bss_to_new_bss_irat_meas_config_t           irat_meas_config;
    /*^ O, RRM_OLD_BSS_TO_NEW_BSS_INFO_IRAT_MEAS_CONFIG_INFO_PRESENT, N, 0, 0 ^*/
}rrm_old_bss_to_new_bss_information_t;
/* SPR 20635 Fix - SRVCC to GERAN - Ends */

typedef struct
{
    U16                                                     bitmask; /*^ BITMASK ^*/
#define SOURCE_TO_TARGET_TRANSPARENT_ENB_CONTAINER_PRESENT  0x01
#define SOURCE_TO_TARGET_TRANSPARENT_RNC_CONTAINER_PRESENT  0x02
#define SOURCE_TO_TARGET_TRANSPARENT_BSS_CONTAINER_PRESENT  0x04 
#define OLD_TO_NEW_TRANSPARENT_BSS_CONTAINER_PRESENT        0x08 

    rrm_sourceenb_to_targetenb_transparent_cotainer_t       eNBContainer;
    /*^ O, SOURCE_TO_TARGET_TRANSPARENT_ENB_CONTAINER_PRESENT, N, 0, 0 ^*/
    rrm_source_rnc_to_target_rnc_transparent_container_t    rncContainer; 
    /*^ O, SOURCE_TO_TARGET_TRANSPARENT_RNC_CONTAINER_PRESENT, N, 0, 0 ^*/
    rrm_source_bss_to_target_bss_transparent_container_t    bssContainer; 
    /*^ O, SOURCE_TO_TARGET_TRANSPARENT_BSS_CONTAINER_PRESENT, N, 0, 0 ^*/
    rrm_old_bss_to_new_bss_information_t                    old_to_new_bssContainer; 
    /*^ O, OLD_TO_NEW_TRANSPARENT_BSS_CONTAINER_PRESENT, N, 0, 0 ^*/
}rrm_soruce_to_target_transparent_container_t;

typedef struct
{
    U16     length; /*^ M, 0, B, 1, 1024 ^*/
    U8      data[1024]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}ms_classmark_2;

typedef struct
{
    U16     length; /*^ M, 0, B, 1, 1024 ^*/
    U8      data[1024]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}ms_classmark_3;

typedef enum
{
   RRC_HYBRID
}rrc_cell_access_mode_et;

typedef struct
{
    U16                                t304_timer; /*^ M, 0, H, 0, 6 ^*/
    rrc_bitmask_t                      bitmask; /*^ BITMASK ^*/
#define RRC_RRM_UE_HO_ADM_RESP_MCI_UPLINK_POWER_CNTRL_COMMON_V1020_PRESENT 0x01
    uplink_power_control_common_t   uplink_power_control_common;
    /*^ O, RRC_RRM_UE_HO_ADM_RESP_MCI_UPLINK_POWER_CNTRL_COMMON_V1020_PRESENT, N, 0, 0 ^*/
}mobility_control_info_t;

/* CDMA2000_CSFB_HO start */
typedef struct
{
        rrc_bitmask_t                      bitmask; /*^ BITMASK ^*/
#define PZ_HYST_ENABLD_PRESENT                      0x01
#define PZ_HYST_INFO_INCL_PRESENT                   0x02
#define PZ_HYST_LIST_LEN_PRESENT                    0x04
#define PZ_HYST_ACT_TIMER_PRESENT                   0x08
#define PZ_HYST_TIMER_MUL_PRESENT                   0x10
#define PZ_HYST_TIMER_EXP_PRESENT                   0x20
        U8                                pz_hyst_enabled;   /*^ O, 1, H, 0, 1 ^*/
        U8                                pz_hyst_info_incl; /*^ O, 2, H, 0, 1 ^*/
        U8                                pz_hyst_list_len;  /*^ O, 4, H, 0, 1 ^*/
        U8                                pz_hyst_act_timer; /*^ O, 8, N, 0, 0 ^*/
        U8                                pz_hyst_timer_mul; /*^ O, 16, H, 0, 7 ^*/
        U8                                pz_hyst_timer_exp; /*^ O, 32, H, 0, 31 ^*/
}pz_hyst_parameters_included_t; 

typedef struct
{
        rrc_bitmask_t                      bitmask; /*^ BITMASK ^*/
#define FPC_FCH_INIT_SETPT_RC3_PRESENT              0x01
#define FPC_FCH_INIT_SETPT_RC4_PRESENT              0x02
#define FPC_FCH_INIT_SETPT_RC5_PRESENT               0x04
/* Bug 12674 Fix Start */
#define FPC_FCH_INIT_SETPT_RC11_PRESENT              0x08
#define FPC_FCH_INIT_SETPT_RC12_PRESENT              0x10
/* Bug 12674 Fix Stop */
        U8                                 fpc_fch_init_setpt_rc3; /*^ O, 1, N, 0, 0 ^*/
        U8                                 fpc_fch_init_setpt_rc4; /*^ O, 2, N, 0, 0 ^*/
        U8                                 fpc_fch_init_setpt_rc5; /*^ O, 4, N, 0, 0 ^*/
/* Bug 12674 Fix Start */
        U8                                 fpc_fch_init_setpt_rc11; /*^ O, 8, N, 0, 0 ^*/
        U8                                 fpc_fch_init_setpt_rc12; /*^ O, 16, N, 0, 0 ^*/
/* Bug 12674 Fix Stop */
}fpc_fch_included_t;


typedef struct
{
    rrc_bitmask_U32_t                      bitmask; /*^ BITMASK ^*/
#define SID_INCLUDED_PRESENT                        0x01
#define NID_INCLUDED_PRESENT                        0x02
#define MULT_SIDS_INCLUDED_PRESENT                  0x04
#define MULT_NIDS_INCLUDED_PRESENT                  0x08
#define REG_ZONE_INCLUDED_PRESENT                   0x10
#define TOTAL_ZONES_INCLUDED_PRESENT                0x20
#define ZONE_TIMER_INCLUDED_PRESENT                 0x40
#define PACKET_ZONE_ID_INCLUDED_PRESENT             0x80
#define PZ_ID_HYST_PARAMETERS_INCLUDED_PRESENT      0x100
#define P_REV_INCLUDED_PRESENT                      0x200
#define MIN_P_REV_INCLUDED_PRESENT                  0x400
#define NEG_SLOT_CYCLE_INDEX_SUP_INCLUDED_PRESENT   0x800
#define ENCRYPT_MODE_INCLUDED_PRESENT               0x1000
#define ENC_SUPPORTED_INCLUDED_PRESENT              0x2000
#define SIG_ENCRYPT_SUP_INCLUDED_PRESENT            0x4000
#define MSG_INTEGRITY_SUP_INCLUDED_PRESENT          0x8000
#define SIG_INTEGRITY_SUP_INCL_INCLUDED_PRESENT     0x10000
#define SIG_INTEGRITY_SUP_INCLUDED_PRESENT          0x20000
#define AUTH_INCLUDED_PRESENT                       0x40000
#define MAX_NUM_ALT_SO_INCLUDED_PRESENT             0x80000
#define USE_SYNC_ID_INCLUDED_PRESENT                0x100000
#define MS_INIT_POS_LOC_SUP_IND_INCLUDED_PRESENT    0x200000
#define MOB_QOS_INCLUDED_PRESENT                    0x400000
#define BAND_CLASS_INFO_REQ_INCLUDED_PRESENT        0x800000
#define BAND_CLASS_INCLUDED_PRESENT                 0x1000000
#define BYPASS_REG_IND_INCLUDED_PRESENT             0x2000000
#define ALT_BAND_CLASS_INCLUDED_PRESENT             0x4000000
#define MAX_ADD_SERV_INSTANCE_INCLUDED_PRESENT      0x8000000
#define HOME_REG_INCLUDED_PRESENT                   0x10000000
#define FOR_SID_REG_INCLUDED_PRESENT                0x20000000
#define FOR_NID_REG_INCLUDED_PRESENT                0x40000000
#define POWER_UP_REG_INCLUDED_PRESENT               0x80000000
/* Bug 12189 Fix Start */
    U16	 	sid;
    /*^ O,SID_INCLUDED_PRESENT , N, 0, 0 ^*/ 
    U16		nid;
    /*^ O,NID_INCLUDED_PRESENT , N, 0, 0 ^*/ 
/* Bug 12189 Fix Stop */
    U8		multi_sids;
    /*^ O,MULT_SIDS_INCLUDED_PRESENT , H, 0, 1 ^*/
    U8		multi_nids;
    /*^ O,MULT_NIDS_INCLUDED_PRESENT , H, 0, 1 ^*/
/* Bug 12189 Fix Start */
    U16		reg_zone;
    /*^ O,REG_ZONE_INCLUDED_PRESENT , N, 0, 0 ^*/
    U8		total_zones;
    /*^ O,TOTAL_ZONES_INCLUDED_PRESENT , N, 0, 0 ^*/
    U8		zone_timer;
    /*^ O,ZONE_TIMER_INCLUDED_PRESENT , N, 0, 0 ^*/
/* Bug 12189 Fix Stop */
    U8		packet_zone_id;
    /*^ O,PACKET_ZONE_ID_INCLUDED_PRESENT , N, 0, 0 ^*/
    pz_hyst_parameters_included_t 
        pz_hyst_parameters_included;
    /*^ O,PZ_ID_HYST_PARAMETERS_INCLUDED_PRESENT , N, 0, 0 ^*/
    U8		p_rev;
    /*^ O,P_REV_INCLUDED_PRESENT , N, 0, 0 ^*/
    U8		min_p_rev;
    /*^ O,MIN_P_REV_INCLUDED_PRESENT , N, 0, 0 ^*/
    U8		neg_slot_cycle_index_sup;
    /*^ O,NEG_SLOT_CYCLE_INDEX_SUP_INCLUDED_PRESENT , H, 0, 1 ^*/
    U8		encrypt_mode;
    /*^ O,ENCRYPT_MODE_INCLUDED_PRESENT , H, 0, 3 ^*/
    U8		enc_supported;
    /*^ O,ENC_SUPPORTED_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		sig_encrypt_sup;
    /*^ O,SIG_ENCRYPT_SUP_INCLUDED_PRESENT, N, 0, 0 ^*/
    U8		msg_integrity_sup;
    /*^ O,MSG_INTEGRITY_SUP_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		sig_integrity_sup_incl;
    /*^ O,SIG_INTEGRITY_SUP_INCL_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		sig_integrity_sup;
    /*^ O,SIG_INTEGRITY_SUP_INCLUDED_PRESENT, N, 0, 0 ^*/
    U8		auth;
    /*^ O,AUTH_INCLUDED_PRESENT, H, 0, 3 ^*/
    U8		max_num_alt_so;
    /*^ O,MAX_NUM_ALT_SO_INCLUDED_PRESENT, H, 0, 7 ^*/
    U8		use_sync_id;
    /*^ O,USE_SYNC_ID_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		ms_init_pos_loc_sup_ind;
    /*^ O,MS_INIT_POS_LOC_SUP_IND_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		mob_qos;
    /*^ O,MOB_QOS_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		band_class_info_req;
    /*^ O,BAND_CLASS_INFO_REQ_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		band_class;
    /*^ O,BAND_CLASS_INCLUDED_PRESENT, H, 0, 31 ^*/
    U8		bypass_reg_ind;
    /*^ O,BYPASS_REG_IND_INCLUDED_PRESENT, H, 0, 31 ^*/
    U8		alt_band_class;
    /*^ O,ALT_BAND_CLASS_INCLUDED_PRESENT, H, 0, 31 ^*/
    U8		max_add_serv_instance;
    /*^ O,MAX_ADD_SERV_INSTANCE_INCLUDED_PRESENT, H, 0, 7 ^*/
    U8		home_reg;
    /*^ O,HOME_REG_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		for_sid_reg;
    /*^ O,FOR_SID_REG_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		for_nid_reg;
    /*^ O,FOR_NID_REG_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		power_up_reg;
    /*^ O,POWER_UP_REG_INCLUDED_PRESENT, H, 0, 1 ^*/
}mobility_parameters_t;

typedef struct
{
    rrc_bitmask_U32_t                      bitmask; /*^ BITMASK ^*/
#define POWER_DOWN_REG_INCLUDED_PRESENT             0x01
#define PARAMETER_REG_INCLUDED_PRESENT              0x02
#define REG_PRD_INCLUDED_PRESENT                    0x04
#define REG_DIST_INCLUDED_PRESENT                   0x08
#define PREF_MSID_TYPE_INCLUDED_PRESENT             0x10
#define EXT_PREF_MSID_TYPE_INCLUDED_PRESENT         0x20
#define MEID_REQD_INCLUDED_PRESENT                  0x40
#define MCC_INCLUDED_PRESENT                        0x80
#define IMSI_11_12_INCLUDED_PRESENT                 0x100
#define IMSI_T_SUPPORTED_INCLUDED_PRESENT           0x200
#define RECONNECT_MSG_IND_INCLUDED_PRESENT          0x400
#define RER_MODE_SUPPORTED_INCLUDED_PRESENT         0x800
#define TKZ_MODE_SUPPORTED_INCLUDED_PRESENT         0x1000
#define TKZ_ID_INCLUDED_PRESENT                     0x2000
#define PILOT_REPORT_INCLUDED_PRESENT               0x4000
#define SDB_SUPPORTED_INCLUDED_PRESENT              0x8000
#define AUTO_FCSO_ALLOWED_INCLUDED_PRESENT          0x10000
#define SDB_IN_RCNM_IND_INCLUDED_PRESENT            0x20000
#define FPC_FCH_INCLUDED_PRESENT                    0x40000
#define T_ADD_INCLUDED_PRESENT                      0x80000
#define PILOT_INC_INCLUDED_PRESENT                  0x100000
#define LP_SEC_INCLUDED_PRESENT                       0x200000
#define DAYLT_INCLUDED_PRESENT                     0x400000
#define GCSNA_L2_ACK_TIMER_INCLUDED_PRESENT                      0x800000
#define GCSNA_SEQUENCE_CONTEXT_TIMER_INCLUDED_PRESENT         0x1000000
/* Bug 12674 Fix Start */
/* Only Bitmasks are required. Values will be calculated at Layer3. */
#define RAND_INCLUDED_PRESENT                          0x2000000
#define LTM_OFF_INCLUDED_PRESENT                       0x4000000
/* Bug 12674 Fix Stop */

    U8		power_down_reg;
    /*^ O,POWER_DOWN_REG_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		parameter_reg;
    /*^ O,PARAMETER_REG_INCLUDED_PRESENT, H, 0, 1 ^*/
/* Bug 12189 Fix Start */
    U8		reg_prd;
    /*^ O,REG_PRD_INCLUDED_PRESENT, N, 0, 0 ^*/
/* Bug 12189 Fix Stop */
    U16		reg_dist;
    /*^ O,REG_DIST_INCLUDED_PRESENT, H, 0, 2047 ^*/
    U8		pref_msid_type;
    /*^ O,PREF_MSID_TYPE_INCLUDED_PRESENT, H, 0, 3 ^*/
    U8		ext_pref_msid_type;
    /*^ O,EXT_PREF_MSID_TYPE_INCLUDED_PRESENT, H, 0, 3 ^*/
    U8		meid_reqd;
    /*^ O,MEID_REQD_INCLUDED_PRESENT, H, 0, 1 ^*/
    U16		mcc;
    /*^ O,MCC_INCLUDED_PRESENT, H, 0, 1023 ^*/
    U8		imsi_11_12;
    /*^ O,IMSI_11_12_INCLUDED_PRESENT, H, 0, 127 ^*/
    U8		imsi_t_supported;
    /*^ O,IMSI_T_SUPPORTED_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		reconnect_msg_ind;
    /*^ O,RECONNECT_MSG_IND_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		rer_mode_supported;
    /*^ O,RER_MODE_SUPPORTED_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		tkz_mode_supported;
    /*^ O,TKZ_MODE_SUPPORTED_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		tkz_id;
    /*^ O,TKZ_ID_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		pilot_report;
    /*^ O,PILOT_REPORT_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		sdb_supported;
    /*^ O,SDB_SUPPORTED_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		auto_fcso_allowed;
    /*^ O,AUTO_FCSO_ALLOWED_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		sdb_in_rcnm_ind;
    /*^ O,SDB_IN_RCNM_IND_INCLUDED_PRESENT, H, 0, 1 ^*/
    fpc_fch_included_t fpc_fch_included;
    /*^ O,FPC_FCH_INCLUDED_PRESENT, N, 0, 0 ^*/
    U8		t_add;
    /*^ O,T_ADD_INCLUDED_PRESENT, H, 0, 63 ^*/
    U8		pilot_inc;
    /*^ O,PILOT_INC_INCLUDED_PRESENT, H, 0, 15 ^*/
    U8		lp_sec;
    /*^ O,LP_SEC_INCLUDED_PRESENT, N, 0, 0 ^*/
    U8		daylt;
    /*^ O,DAYLT_INCLUDED_PRESENT, H, 0, 1 ^*/
    U8		gcsna_l2_ack_timer;
    /*^ O,GCSNA_L2_ACK_TIMER_INCLUDED_PRESENT, N, 0, 0 ^*/
    U8		gcsna_sequence_context_timer;
    /*^ O,GCSNA_SEQUENCE_CONTEXT_TIMER_INCLUDED_PRESENT, N, 0, 0 ^*/
}additional_mobility_parameters_t;

typedef struct
{
    rrc_bitmask_t   bitmask; /*^ BITMASK ^*/

    U8  system_type;     /*^ M, 0, H, 0, 2 ^*/

    U8  band_class;      /*^ M, 0, H, 0, 31 ^*/
    /* rrm_bandclass_cdma2000_et */

    U16  channel_number; /*^ M, 0, H, 0, 2047 ^*/
}channel_record_t;


typedef struct
{
    rrc_bitmask_t   bitmask; /*^ BITMASK ^*/
#define PILOT_ENTRY_1X_CELL_IDENTIFIER_PRESENT          0x01
#define PILOT_ENTRY_HRPD_SECTOR_IDENTIFIER_PRESENT      0x02
#define PILOT_ENTRY_REFERENCE_PILOT_PRESENT             0x04
#define PILOT_ENTRY_PILOT_PN_INFORMATION_PRESENT        0x08
#define PILOT_ENTRY_PILOT_STRENGTH_PRESENT              0x10
#define PILOT_ENTRY_PILOT_ONE_WAY_DELAY_PRESENT         0x20

    channel_record_t              channel_record;
    /*^ M, 0, N, 0, 0 ^*/

    /* field is included for each pilot that is reported */
    U8                            cell_id_info;
    /*^ M, 0, H, 0, 7 ^*/ /* cell_id_info_et */

    /* If the Cell ID Info field indicates that a 1x cell identifier is 
       included, then this record is included */
    cdma2000_1xrtt_cell_identifier_t cdma2000_1xrtt_cell_id;
    /*^ O, PILOT_ENTRY_1X_CELL_IDENTIFIER_PRESENT, N, 0, 0 ^*/

    /* If the Cell ID Info field indicates that an HRPD sector identifier 
       is included, then this record is included */
    cdma2000_hrpd_sector_identifier_t cdma2000_hrpd_sector_id;
    /*^ O, PILOT_ENTRY_HRPD_SECTOR_IDENTIFIER_PRESENT, N, 0, 0 ^*/

    /*reference pilot shall always be an HRPD pilot*/
    U8                            reference_pilot;
    /*^ O, PILOT_ENTRY_REFERENCE_PILOT_PRESENT, H, 0, 1 ^*/

    /*pilot_pn_phase received from UE in measurement report*/
    U16                           pilot_pn_info;
    /*^ O, PILOT_ENTRY_PILOT_PN_INFORMATION_PRESENT, H, 0, 32767 ^*/

    /* This field shall be set to .1. when including the Pilot One Way Delay */
    U8                            pilot_one_way_delay_included;  
    /*^ M, 0, H, 0, 1 ^*/

    /* The HRPD AN will report last pilot strength measurement */
    U8                            pilot_strength;                         
    /*^ O, PILOT_ENTRY_PILOT_STRENGTH_PRESENT, H, 0, 63 ^*/

    /* estimate of the one-way delay from AT to cell site in units of 100 ns*/
    U16                           pilot_one_way_delay;
    /*^ O, PILOT_ENTRY_PILOT_ONE_WAY_DELAY_PRESENT, N, 0, 0 ^*/
}pilot_entry_t;

typedef struct 
{
/*MAX_PILOT_ENTRY is derived from maxCellReport of MeasResultListCDMA2000*/
#define MAX_PILOT_ENTRY   8
/*SPR_17664_start*/
    U8 num_of_pilots;/*^ M, 0, B, 1, MAX_PILOT_ENTRY ^*/
/*SPR_17664_end*/
    pilot_entry_t pilot_entry[MAX_PILOT_ENTRY];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}pilot_list_t;

/*CR 487 changes start*/
typedef enum 
{
    DUAL_RX_TX_REDIRECT_IND_TRUE
}dual_rx_tx_redirect_ind_r10_et;

typedef struct
{   
    rrc_bitmask_t               bitmask; /*^ BITMASK ^*/
#define RRC_RRM_HO_FROM_EUTRA_DUAL_RX_TX_REDIRECT_IND_PRESENT           0x01
#define RRC_RRM_HO_FROM_EUTRA_REDIRECT_CARRIER_CDMA2000_1XRTT_PRESENT   0x02

    U8                          dual_rx_tx_redirect_ind_r10;
/*^ O, RRC_RRM_HO_FROM_EUTRA_DUAL_RX_TX_REDIRECT_IND_PRESENT, H, 0, 0 ^*/
    /*dual_rx_tx_redirect_ind_r10_et */

    carrier_freq_cdma2000_t     redirect_carrier_cdma2000_1XRTT_r10;
/*^ O, RRC_RRM_HO_FROM_EUTRA_REDIRECT_CARRIER_CDMA2000_1XRTT_PRESENT, N, 0, 0 ^*/

}ho_from_eutra_preperation_req_r10_ies_t;
/*CR 487 changes stop*/

typedef struct
{
    rrc_bitmask_t                      bitmask; /*^ BITMASK ^*/
#define RRC_RRM_HO_REQUIRED_MOBILITY_PARAMETERS_PRESENT              0x01
#define RRC_RRM_HO_REQUIRED_ADDITIONAL_MOBILITY_PARAMETERS_PRESENT   0x02
#define RRC_RRM_HO_REQUIRED_PILOT_LIST_PRESENT                       0x04
#define RRC_RRM_EUTRAN_ROUND_TRIP_DELAY_EST_INFO_PRESENT             0x08
/*CR 487 changes start*/
#define RRC_RRM_HO_REQUIRED_HO_FROM_EUTRA_PREPERATION_REQ_PRESENT    0x10
/*CR 487 changes stop*/    
    mobility_parameters_t mobility_parameters;
    /*^ O, RRC_RRM_HO_REQUIRED_MOBILITY_PARAMETERS_PRESENT, N, 0, 0 ^*/
    additional_mobility_parameters_t add_mobility_parameters;
    /*^ O, RRC_RRM_HO_REQUIRED_ADDITIONAL_MOBILITY_PARAMETERS_PRESENT, N, 0, 0 ^*/

    pilot_list_t pilot_list;
    /*^ O, RRC_RRM_HO_REQUIRED_PILOT_LIST_PRESENT, N, 0, 0 ^*/

    U16                  eutran_round_trip_delay_estimation_info;
    /*^ O, RRC_RRM_EUTRAN_ROUND_TRIP_DELAY_EST_INFO_PRESENT, H, 0, 2047 ^*/
    
    /*CR 487 changes start*/
    ho_from_eutra_preperation_req_r10_ies_t ho_from_eutra_preperation_req_r10;
    /*^ O, RRC_RRM_HO_REQUIRED_HO_FROM_EUTRA_PREPERATION_REQ_PRESENT, N, 0, 0^*/
    /*CR 487 changes stop*/
}rrm_cdma2000_information_t;
/* CDMA2000_CSFB_HO stop */

typedef enum
{
    RRC_NC0,
    RRC_NC1,
    RRC_NC2,
    RRC_RESET
}rrc_nco_et;

typedef struct
{
    rrc_bitmask_t           bitmask; /*^ BITMASK ^*/
#define SYSTEM_INFO_LIST_GERAN_SI_PRESENT       0x0001
#define SYSTEM_INFO_LIST_GERAN_PSI_PRESENT      0x0002
    system_info_list_geran_t si;
    /*^ O, SYSTEM_INFO_LIST_GERAN_SI_PRESENT, N, 0, 0 ^*/
    system_info_list_geran_t psi;
    /*^ O, SYSTEM_INFO_LIST_GERAN_PSI_PRESENT, N, 0, 0 ^*/
}system_info_list_geran_si_psi_t;

typedef struct
{
    rrc_bitmask_t           bitmask; /*^ BITMASK ^*/
#define RRC_RRM_UE_CCO_TARGET_RAT_GERAN_TYPE_NCO_PRESENT    0x0001
#define RRC_RRM_UE_CCO_TARGET_RAT_GERAN_TYPE_SI_PSI_PRESENT     0x0002
    phys_cell_id_geran_t            pci_geran;
    carrier_freq_geran_t            carr_freq;
    U8              nco; 
    /*^ O, RRC_RRM_UE_CCO_TARGET_RAT_GERAN_TYPE_NCO_PRESENT, H, 0, 3 ^*/ /*rrc_nco_et*/
    system_info_list_geran_si_psi_t si_list;
    /*^ O, RRC_RRM_UE_CCO_TARGET_RAT_GERAN_TYPE_SI_PSI_PRESENT, N, 0, 0 ^*/ 
}rrc_target_rat_type_geran_t;

typedef struct
{
    rrc_bitmask_t                     bitmask; /*^ BITMASK ^*/
#define RRC_RRM_UE_CCO_TARGET_RAT_GERAN_TYPE_PRESENT     0x0001
    rrc_target_rat_type_geran_t       geran_params;
    /*^ O, RRC_RRM_UE_CCO_TARGET_RAT_GERAN_TYPE_PRESENT, N, 0, 0 ^*/ 
}rrc_target_rat_type_t;

typedef struct
{
    rrc_bitmask_t                     bitmask; /*^ BITMASK ^*/
    U8                                cs_fallback_indicator; /*^ M, 0, H, 0, 1 ^*/ 
    /*rrc_bool_et*/
    rrc_target_rat_type_t             target_rat_params; /*^ M, 0, N, 0, 0 ^*/ 
}cco_parameters_t;

typedef struct
{
    rrc_bitmask_t                      bitmask; /*^ BITMASK ^*/
#define RRC_RRM_UE_HO_ADM_REQ_DIRECT_FORWARD_PATH_AVAILABLE_PRESENT     0x0001
#define RRC_RRM_UE_HO_ADM_REQ_SRVCC_HO_INDICATION_PRESENT               0x0002
#define RRC_RRM_UE_HO_ADM_RE_SRC_TO_TARGET_CONTAINER_PRESENT            0x0004
#define RRC_RRM_UE_HO_ADM_RE_SRC_TO_TARGET_CONTAINER_SECONDARY_PRESENT  0x0008
#define RRC_RRM_UE_HO_ADM_REQ_PS_SERVICE_NOT_AVAILABLE_PRESENT          0x0010
#define RRC_RRM_UE_HO_ADM_REQ_CELL_ACCESS_MODE_PRESENT                  0x0020
#define RRC_RRM_UE_HO_ADM_RE_BAND_INDICATOR_PRESENT                     0x0040
#define RRC_RRM_UE_CSG_ID_PRESENT                                       0x0080
#define RRC_RRM_UE_HO_ADM_REQ_MOBILITY_CONTROL_INFO_PRESENT             0x0100
#define RRC_RRM_UE_HO_ADM_RE_HO_FREQ_TYPE_PRESENT                       0x0200
#define RRC_RRM_CDMA2000_INFO_PRESENT                                   0x0400
#define RRC_RRM_UE_HO_CCO_PARAMS_PRESENT                                0x0800
#define RRC_RRM_UE_HO_ADM_REQ_SRVCC_HO_OP_POSSIBLE_PRESENT              0x1000
/* CSR 00070260 Changes Start */
#define RRC_RRM_CDMA2000_1XRTT_REF_CELL_ID_PRESENCE_FLAG                0x2000
#define RRC_RRM_CDMA2000_HRPD_REF_CELL_ID_PRESENCE_FLAG                 0x4000
/* CSR 00070260 Changes Stop */
/* SPR 16477 Fix Start */
#define RRC_RRM_UE_HO_REQUIRED_GUMMEI_INFO_PRESENT                      0x8000
/* SPR 16477 Fix Stop */

        rrc_cell_index_t           cell_index;
/*^ M, 0, H, 0, 7 ^*/    /* MAX_NUM_CELLS - 1 */

    U16                   ue_index;
    U8                    ho_type;  /*^ M, 0, H, 0, 11 ^*/ /*rrc_handover_type_et*/ 
    rrm_cause_t           cause ; /*^ M, 0, N, 0, 0 ^*/
    rrm_target_id_t       target_id; /*^ M, 0, N, 0, 0 ^*/
    U8    direct_fwd_path_available; 
    /*^ O, RRC_RRM_UE_HO_ADM_REQ_DIRECT_FORWARD_PATH_AVAILABLE_PRESENT, H, 0, 0 ^*/ /* rrc_bool_et */

    U8    srvcc_ho_indication; 
    /*^ O, RRC_RRM_UE_HO_ADM_REQ_SRVCC_HO_INDICATION_PRESENT, H, 0, 1 ^*/ /* rrc_srvcc_ho_type_et */

    rrm_soruce_to_target_transparent_container_t   source_to_target_container;
    /*^ O, RRC_RRM_UE_HO_ADM_RE_SRC_TO_TARGET_CONTAINER_PRESENT, N, 0, 0 ^*/

    rrm_old_bss_to_new_bss_information_t   source_to_target_container_secondary; 
    /*^ O, RRC_RRM_UE_HO_ADM_RE_SRC_TO_TARGET_CONTAINER_SECONDARY_PRESENT, N, 0, 0 ^*/
    U8    ps_service_not_available;
    /*^ O, RRC_RRM_UE_HO_ADM_REQ_PS_SERVICE_NOT_AVAILABLE_PRESENT, H, 0, 0 ^*/
    /* rrc_ps_service_not_available_et */

    U32   cell_access_mode;
    /*^ O, RRC_RRM_UE_HO_ADM_REQ_CELL_ACCESS_MODE_PRESENT, N, 0, 0 ^*/ /* rrc_cell_access_mode_et */ 

    U8     band_indicator; /*^ O,RRC_RRM_UE_HO_ADM_RE_BAND_INDICATOR_PRESENT, H, 0, 1 ^*/  
    /* rrc_rrm_carrier_freq_geran_band_indicator_et */

    U8    csg_identity[CSG_ID_OCTET_SIZE]; /*^ O, RRC_RRM_UE_CSG_ID_PRESENT, OCTET_STRING, FIXED ^*/

    mobility_control_info_t            mobility_control_info;
    /*^ O, RRC_RRM_UE_HO_ADM_REQ_MOBILITY_CONTROL_INFO_PRESENT, N, 0, 0 ^*/

    U8                    ho_freq_type;  
    /*^ O, RRC_RRM_UE_HO_ADM_RE_HO_FREQ_TYPE_PRESENT, H, 0, 1 ^*/ /*rrc_handover_freq_type_et*/

    rrm_cdma2000_information_t  cdma2000_information;
    /*^ O, RRC_RRM_CDMA2000_INFO_PRESENT, N, 0, 0 ^*/

    cco_parameters_t                   cco_params;
    /*^ O, RRC_RRM_UE_HO_CCO_PARAMS_PRESENT, N, 0, 0 ^*/ 
    U8    srvcc_ho_op_possible;
    /*^ O, RRC_RRM_UE_HO_ADM_REQ_SRVCC_HO_OP_POSSIBLE_PRESENT, B, 1, 1 ^*/ /* rrc_bool_et */ 
/* CSR 00070260 Changes Start */
    cdma2000_1xrtt_ref_cell_identifier_t  ref_cell_id_1xrtt;  
/*^ O, RRC_RRM_CDMA2000_1XRTT_REF_CELL_ID_PRESENCE_FLAG, N, 0, 0 ^*/

    cdma2000_hrpd_ref_sector_identifier_t ref_sector_id_hrpd;    
/*^ O, RRC_RRM_CDMA2000_HRPD_REF_CELL_ID_PRESENCE_FLAG, N, 0, 0 ^*/
/* CSR 00070260 Changes Stop */
    /* SPR 16477 Fix Start */
    mme_sel_gummei_info_t    gummei_info;
    /*^ O, RRC_RRM_UE_HO_REQUIRED_GUMMEI_INFO_PRESENT, N, 0, 0 ^*/    
    /* SPR 16477 Fix Stop */

}rrc_rrm_ho_required_t; /*^ API, RRC_RRM_HO_REQUIRED ^*/

/******************************************************************************
        RRC_RRM_UE_HO_ADM_REQ
******************************************************************************/

typedef enum
{
        CIPHERING_ALGO_EEA0,
        CIPHERING_ALGO_EEA1,
        CIPHERING_ALGO_EEA2,
        CIPHERING_ALGO_SPARE1,
        CIPHERING_ALGO_SPARE2,
        CIPHERING_ALGO_SPARE3,
        CIPHERING_ALGO_SPARE4,
        CIPHERING_ALGO_SPARE5               
}rrc_security_ciphering_algo_et;


typedef enum
{
        INTEGRITY_PROT_ALGO_EIA0,
        INTEGRITY_PROT_ALGO_EIA1,
        INTEGRITY_PROT_ALGO_EIA2,
        INTEGRITY_PROT_ALGO_SPARE1,
        INTEGRITY_PROT_ALGO_SPARE2,
        INTEGRITY_PROT_ALGO_SPARE3,
        INTEGRITY_PROT_ALGO_SPARE4,
        INTEGRITY_PROT_ALGO_SPARE5
}rrc_security_integrity_prot_algo_et; 


typedef enum
{
        FORBIDDEN_INTER_RAT_ALL,
        FORBIDDEN_INTER_RAT_GEREN_TYPE,
        FORBIDDEN_INTER_RAT_UTRAN_TYPE,
        FORBIDDEN_INTER_RAT_CDMA2000_TYPE,
/*cr_711 changes*/
        FORBIDDEN_INTER_RAT_GERAN_AND_UTRAN,
        FORBIDDEN_INTER_RAT_CDMA2000_AND_UTRAN
}rrc_forbidden_inter_rat_et;


typedef enum
{
    EUTRAN_CELL_SIZE_VERY_SMALL,
    EUTRAN_CELL_SIZE_SMALL, 
    EUTRAN_CELL_SIZE_MEDIUM, 
    EUTRAN_CELL_SIZE_LARGE
}rrc_eutran_cell_size_et;

typedef enum
{
    UTRAN_CELL_SIZE_MACRO,
    UTRAN_CELL_SIZE_MICRO, 
    UTRAN_CELL_SIZE_PICO, 
    UTRAN_CELL_SIZE_FEMTO
}rrc_utran_cell_size_et;

typedef struct
{
        U16                                bitmask; /*^ BITMASK ^*/
#define RADIO_RESOURCE_CONFIG_DEDICATED_SPS_CONFIG_PRESENT 0x01
#define RADIO_RESOURCE_CONFIG_DEDICATED_PHYSICAL_CONFIG_DEDICATED_PRESENT 0x02
#define RADIO_RESOURCE_CONFIG_DEDICATED_RLF_TIMERS_AND_CONSTANTS_PRESENT 0x04

        rrm_srb_info_t                     rrm_srb_to_add_info; /*^ M, 0, N, 0, 0 ^*/
        rrm_drb_to_add_info_list_t   rrm_drb_to_add_info_list; /*^ M, 0, N, 0, 0 ^*/
        rrc_mac_main_config_t        mac_main_config; /*^ M, 0, N, 0, 0 ^*/
        rrc_sps_config_t                  sps_config; 
        /*^ O, RADIO_RESOURCE_CONFIG_DEDICATED_SPS_CONFIG_PRESENT, N, 0, 0 ^*/
        
        rrc_phy_physical_config_dedicated_t rrc_phy_physical_config_dedicated; /*^ O, RADIO_RESOURCE_CONFIG_DEDICATED_PHYSICAL_CONFIG_DEDICATED_PRESENT, N, 0, 0 ^*/

/* cr_220 changes */
        rlf_timers_and_constants_r9_t rlf_timers_and_constants_r9;
        /*^ O, RADIO_RESOURCE_CONFIG_DEDICATED_RLF_TIMERS_AND_CONSTANTS_PRESENT ^*/
      
}radio_resource_config_dedicated_t;

typedef struct
{
        U8     ciphering_algo; /*^ M, 0, H, 0, 2 ^*/ /*rrc_security_ciphering_algo_et*/
        U8     integrity_protect_algo; /*^ M, 0, H, 0, 2 ^*/ /*rrc_security_integrity_prot_algo_et*/ 
}security_algorithm_config_t;

typedef struct
{
    rrm_meas_config_t                  meas_config; /*^ M, 0, N, 0, 0 ^*/
    radio_resource_config_dedicated_t  radio_resource_config_dedicated; /*^ M, 0, N, 0, 0 ^*/
    security_algorithm_config_t        security_algorithm_config; /*^ M, 0, N, 0, 0 ^*/
    mib_info_t                         master_information_block; /*^ M, 0, N, 0, 0 ^*/
    sib_type_1_Info_t                  sib_type1_info; /*^ M, 0, N, 0, 0 ^*/
    sib_type_2_Info_t                  sib_type2_info; /*^ M, 0, N, 0, 0 ^*/
    U8                                 antenna_info_common; /*^ M, 0, H, 0, 2 ^*/ /*ENUMERATED {an1, an2, an4, spare1}*/
    arfcn_value_utra_t                 dl_carrier_freq; /*^ M, 0, N, 0, 0 ^*/
}as_config_t;  

typedef struct
{
        U8    cell_Id[CELL_ID_OCTET_SIZE];     /*^ M, 0, OCTET_STRING, FIXED ^*/
        U16  short_mac_i;  /*^ M, 0, N, 0, 0 ^*/
}additional_reestab_info_t;

typedef struct
{
/*SPR_17664_start*/
        U8 count; /*^ M, 0, B, 1, MAX_NUM_TARGET_CELL ^*/
/*SPR_17664_end*/
        additional_reestab_info_t additional_reestab_info[MAX_NUM_TARGET_CELL]; 
        /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}additional_reestab_info_list_t;

typedef struct
{
        U16                                bitmask; /*^ BITMASK ^*/
#define AS_CONTEXT_ADDITIONAL_REESTAB_INFO_PRESENT 0x01

        rrc_phys_cell_id_t  source_phy_cell_id; /*^ M, 0, H, 0, 503 ^*/
        U16                      target_cell_short_mac_i; /*^ M, 0, N, 0, 0 ^*/
        additional_reestab_info_list_t  additional_reestab_info_list; 
        /*^ O, AS_CONTEXT_ADDITIONAL_REESTAB_INFO_PRESENT, N, 0, 0 ^*/
}as_context_t;

typedef struct
{
        U16                                bitmask; /*^ BITMASK ^*/
#define SRC_TO_TRG_CONTAINER_AS_CONFIG_SCELL_ADD_MOD_LIST_PRESENT       0x01

       rrc_scell_to_add_mod_list_t             scell_add_mod_list;
/*^ O, SRC_TO_TRG_CONTAINER_AS_CONFIG_SCELL_ADD_MOD_LIST_PRESENT ^*/

} as_config_ext_t;


typedef struct
{
        U16                                bitmask; /*^ BITMASK ^*/
#define SRC_TO_TRG_CONTAINER_AS_CONFIG_PRESENT       0x01
/*SPR_19066_START*/
/* SRC_TO_TRG_CONTAINER_RRM_CONFIG_PRESENT has been deprecated */
/*SPR_19066_END*/
#define SRC_TO_TRG_CONTAINER_AS_CONTEXT_PRESENT      0x04
#define SRC_TO_TRG_CONTAINER_SUB_PROFILE_ID_PRESENT  0x08
#define SRC_TO_TRG_CONTAINER_AS_CONFIG_PROXIMITY_CONFIG_PRESENT 0x10
#define SRC_TO_TRG_CONTAINER_RRM_CONFIG_EXT_PRESENT             0x20
#define SRC_TO_TRG_CONTAINER_AS_CONFIG_EXT_PRESENT              0x40

        rrc_radio_capability_info_t  rrc_radio_capability_info; /*^ M, 0, N, 0, 0 ^*/
        as_config_t                  as_config;    
        /*^ O, SRC_TO_TRG_CONTAINER_AS_CONFIG_PRESENT, N, 0, 0 ^*/
        
        /*SPR_19066_START*/
        /* Code Removed */
        /*SPR_19066_END*/
        as_context_t                 as_context;   
        /*^ O, SRC_TO_TRG_CONTAINER_AS_CONTEXT_PRESENT, N, 0, 0 ^*/
        
        U16                          sub_profile_id; 
        /*^ O, SRC_TO_TRG_CONTAINER_SUB_PROFILE_ID_PRESENT, N, 0, 0 ^*/
        
        ue_history_t                 ue_history;  /*^ M, 0, N, 0, 0 ^*/
        rrm_report_proximity_config_r9_t   proximity_config;
        /*^ O, SRC_TO_TRG_CONTAINER_AS_CONFIG_PROXIMITY_CONFIG_PRESENT ^*/ 

       rrm_config_t                 rrm_config;    
        /*^ O, SRC_TO_TRG_CONTAINER_RRM_CONFIG_EXT_PRESENT, N, 0, 0 ^*/

       as_config_ext_t                  as_config_ext;    
       /*^ O, SRC_TO_TRG_CONTAINER_AS_CONFIG_EXT_PRESENT, N, 0, 0 ^*/
}src_to_trg_container_t;

typedef struct
{
        U16                                 ciphering_algo; /*^ M, 0, N, 0, 0 ^*/
        U16                                 integrity_prot_algo; /*^ M, 0, N, 0, 0 ^*/
}ue_security_capabilities_t;

typedef struct
{
        U8  event_type; /*^ M, 0, H, 0, 2 ^*/
        U8  report_area; /*^ M, 0, H, 0, 0 ^*/
}request_type_t;

/* cr_592 enum*/
typedef enum
{        
  RRC_MEMBER,
  RRC_NOT_MEMBER
}rrc_csg_membership_status_et; 

typedef struct
{
    U16                                bitmask; /*^ BITMASK ^*/

#define RRC_RRM_UE_HO_ADM_REQ_HO_RESTRICTION_LIST_PRESENT   0x01
#define RRC_RRM_UE_HO_ADM_REQ_REQUEST_TYPE_PRESENT          0x02
#define RRC_RRM_UE_HO_ADM_REQ_SRVCC_OP_POSSIBLE_PRESENT     0x04
    /* cr_592 macro*/
#define RRC_RRM_UE_HO_ADM_REQ_CSG_MEMBERSHIP_STATUS_PRESENT 0x08
    /* mme grp id feature changes start */
#define RRC_RRM_UE_HO_ADM_REQ_GUMMEI_INFO_PRESENT           0x10
    /* mme grp id feature changes stop */
    /*SPR 20632 21459 START*/
#define RRC_RRM_UE_HO_ADM_REQ_MME_ID_PRESENT                0x20
    /*SPR 20632 21459 END*/

    U16                                 ue_index;
    U8                                  ho_type; /*^ M, 0, H, 0, 11 ^*/  /*rrc_handover_type_et*/
    rrm_cause_t                         cause; /*^ M, 0, N, 0, 0 ^*/
    rrm_ue_agg_max_bit_rate_t           ue_agg_max_bit_rate; /*^ M, 0, N, 0, 0 ^*/
    rrm_erab_to_be_setup_item_list_t    erab_to_be_setup_item_list; /*^ M, 0, N, 0, 0 ^*/
    src_to_trg_container_t              src_to_trg_container; /*^ M, 0, N, 0, 0 ^*/
    ho_restriction_list_t               ho_restriction_list;
    /*^ O, RRC_RRM_UE_HO_ADM_REQ_HO_RESTRICTION_LIST_PRESENT, N, 0, 0 ^*/
    request_type_t                      request_type;
    /*^ O, RRC_RRM_UE_HO_ADM_REQ_REQUEST_TYPE_PRESENT, N, 0, 0 ^*/
    U8                                  srvcc_op_possible;
    /*^ O, RRC_RRM_UE_HO_ADM_REQ_SRVCC_OP_POSSIBLE_PRESENT, N, 0, 0 ^*//*rrc_srvcc_ho_type_et*/
    U32                                 csg_membership_status;
    /*^ O, RRC_RRM_UE_HO_ADM_REQ_CSG_MEMBERSHIP_STATUS_PRESENT, N, 0, 0 ^*/
    /* rrc_csg_membership_status_et */
    /* mme grp id feature changes start */
    mme_sel_gummei_info_t               gummei_info;
    /*^ O, RRC_RRM_UE_HO_ADM_REQ_GUMMEI_INFO_PRESENT, N, 0, 0 ^*/ 
    /* mme grp id feature changes stop */
    /*SPR 20632 21459 START*/
    U8                                  mme_id;
    /*^ O, RRC_RRM_UE_HO_ADM_REQ_MME_ID_PRESENT, N, 0, 0 ^*/
    /*SPR 20632 21459 END*/
}rrc_rrm_ue_ho_adm_req_t; /*^ API, RRC_RRM_UE_HO_ADM_REQ ^*/

/******************************************************************************
        RRC_RRM_UE_HO_ADM_RESP
******************************************************************************/
typedef struct
{
        U8 forward_direction; /*^ M, 0, H, 0, 2 ^*/ /*rrm_forwarding_direction_et*/
        U8 erab_id;  /*^ M, 0, H, 0, 15 ^*/
}rrm_drb_sub_to_fwd_t;

typedef struct
{
	/* SPR_17664_start*/
        U8 count;/*^ M, 0, H, 0, MAX_LC_COUNT ^*/
        rrm_drb_sub_to_fwd_t drb_sub_to_fwd[MAX_LC_COUNT]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
	/* SPR_17664_end*/
}rrm_drb_sub_to_fwd_list_t;

typedef struct
{
        U16                                bitmask; /*^ BITMASK ^*/ 
#define RADIO_RESOURCE_CONFIG_SPS_CONFIG_PRESENT                0x01
#define RADIO_RESOURCE_CONFIG_MEAS_CONFIG_PRESENT               0x02
#define RADIO_RESOURCE_CONFIG_DRB_SUB_FWR_LIST_PRESENT          0x04
#define RADIO_RESOURCE_CONFIG_DRB_FAIL_TO_ADD_LIST_PRESENT      0x08
#define RADIO_RESOURCE_CONFIG_RLF_TIMERS_AND_CONSTANTS_PRESENT  0x10
/* eICIC_Changes_Start*/
/*Bug  3223 Code Changes Start */
#define RRM_UE_CONFIG_MEAS_SUBFRAME_PATTERN_PCELL_R10_PRESENT   0x20
/*Bug  3223 Code Changes End */
/* eICIC_Changes_End*/

        rrm_srb_info_t              srb_info; /*^ M, 0, N, 0, 0 ^*/
        rrm_drb_to_add_info_list_t  drb_to_add_list; /*^ M, 0, N, 0, 0 ^*/
        rrm_mac_config_t            mac_config; /*^ M, 0, N, 0, 0 ^*/
        rrc_sps_config_t            rrc_sps_config; 
        /*^ O, RADIO_RESOURCE_CONFIG_SPS_CONFIG_PRESENT, N, 0, 0 ^*/
        
        rrc_phy_physical_config_dedicated_t physical_config_dedicated; /*^ M, 0, N, 0, 0 ^*/
        rrm_meas_config_t                   meas_config; 
        /*^ O, RADIO_RESOURCE_CONFIG_MEAS_CONFIG_PRESENT, N, 0, 0 ^*/
        
        rrm_drb_sub_to_fwd_list_t           drb_sub_to_fwd_list;/*Legacy */ 
        /*^ O, RADIO_RESOURCE_CONFIG_DRB_SUB_FWR_LIST_PRESENT, N, 0, 0 ^*/
        
        rrm_drb_failed_to_add_info_list_t   drb_failed_to_add_list; 
        /*^ O, RADIO_RESOURCE_CONFIG_DRB_FAIL_TO_ADD_LIST_PRESENT, N, 0, 0 ^*/

/* cr_220 change */
        rlf_timers_and_constants_r9_t rlf_timers_and_constants_r9;
        /*^ O, RADIO_RESOURCE_CONFIG_RLF_TIMERS_AND_CONSTANTS_PRESENT ^*/
        /* eICIC_Changes_Start */
        /*Bug  3223 Code Changes Start */
        rrc_meas_subframe_pattern_pcell_r10_t  rrc_meas_subframe_pattern_pcell_r10;
        /*^ O, RRM_UE_CONFIG_MEAS_SUBFRAME_PATTERN_PCELL_R10_PRESENT ^*/
        /*Bug  3223 Code Changes End */
        /* eICIC_Changes_End*/

}radio_resource_config_t;

typedef struct
{
        U16                                bitmask; /*^ BITMASK ^*/ 
#define RRM_RESPONSE_CAUSE_PRESENT 0x01

        U8   response; /*^ M, 0, H, 0 , 2 ^*//* rrc_return_et */
        rrm_cause_t   cause; /*^ O, RRM_RESPONSE_CAUSE_PRESENT, N, 0, 0 ^*/
}rrm_response_t;


typedef struct
{
        U16                                bitmask; /*^ BITMASK ^*/
#define RRC_RRM_UE_HO_ADM_RESP_RADIO_REOURCE_CONFIG_PRESENT               0X01
#define RRC_RRM_UE_HO_ADM_RESP_MOBILITY_CONTROL_INFO_PRESENT               0x02
# define RRC_RRM_UE_HO_ADM_RESP_CONFIG_PROXIMITY_PRESENT  0x04
# define RRC_RRM_UE_HO_ADM_RESP_CONFIG_SCELL_PRESENT      0x08
/*SPR 13469 Fix start*/
# define RRC_RRM_UE_HO_ADM_RESP_FULL_CONFIG_PRESENT 0x10
/*SPR 13469 Fix stop*/

       
        U16                                       ue_index;
/*SPR_19066_START*/
        U32                                ue_inactive_time_config; /*^ M, 0, N, 0, 0 ^*/
/*SPR_19066_END*/
        rrm_response_t                      response; /*^ M, 0, N, 0, 0 ^*/
        radio_resource_config_t          radio_resource_config;  
        /*^ O, RRC_RRM_UE_HO_ADM_RESP_RADIO_REOURCE_CONFIG_PRESENT, N, 0, 0 ^*/
        mobility_control_info_t            mobility_control_info;
        /*^ O, RRC_RRM_UE_HO_ADM_RESP_MOBILITY_CONTROL_INFO_PRESENT, N, 0, 0 ^*/
        rrm_report_proximity_config_r9_t   proximity_config;
        /*^ O, RRC_RRM_UE_HO_ADM_RESP_CONFIG_PROXIMITY_PRESENT ^*/ 
        rrm_scell_config_t                 scell_config; 
        /*^ O, RRC_RRM_UE_HO_ADM_RESP_CONFIG_SCELL_PRESENT ^*/
}rrc_rrm_ue_ho_adm_resp_t; /*^ API, RRC_RRM_UE_HO_ADM_RESP ^*/

/******************************************************************************
      RC_RRM_UE_HO_ADM_CNF
******************************************************************************/

typedef struct
{
        U16        count; /*^ M, 0, H, 1, 8 ^*/
        U16        erab_id[MAX_ERAB_COUNT]; /*^ M, 0, OCTET_STRING, VARIABLE, H, 0, 15 ^*/
}erab_item_list_t;


typedef struct
{
        U16                 bitmask; /*^ BITMASK ^*/
#define RRC_RRM_UE_HO_ADM_CNF_ERAB_ITEM_LIST_PRESENT            0x01
#define RRC_RRM_UE_HO_ADM_CNF_ERAB_FAILED_ITEM_LIST_PRESENT     0x02        
#define RRC_RRM_UE_HO_ADM_CNF_ERAB_RELEASE_ITEM_LIST_PRESENT    0x04       
/* SPS Start */
#define RRC_RRM_UE_HO_ADM_CNF_ERROR_CODE_PRESENT                0x08
/* SPS Stop */
        
        U16                 ue_index;

        U8                  response; /*^ M, 0, H, 0 , 2 ^*//* rrc_return_et */

        erab_item_list_t    erab_item_list; 
        /*^ O, RRC_RRM_UE_HO_ADM_CNF_ERAB_ITEM_LIST_PRESENT, N, 0 , 0 ^*/

        rrm_erab_failed_item_list_t  erab_failed_item_list;
        /*^ O, RRC_RRM_UE_HO_ADM_CNF_ERAB_FAILED_ITEM_LIST_PRESENT, N, 0 , 0 ^*/

        rrm_erab_to_be_released_item_list_t  erab_to_be_released_item_list;
        /*^ O, RRC_RRM_UE_HO_ADM_CNF_ERAB_RELEASE_ITEM_LIST_PRESENT, N, 0 , 0 ^*/
        
    /* SPS Start */
    U8           error_code;
    /*^ O, RRC_RRM_UE_HO_ADM_CNF_ERROR_CODE_PRESENT, H, 0, 1 ^*/
    /* rrc_rrm_error_code_et */
    /* SPS Stop */
}rrc_rrm_ue_ho_adm_cnf_t;   /*^ API, RRC_RRM_UE_HO_ADM_CNF ^*/


/******************************************************************************
     RRC_RRM_HO_FAILURE
******************************************************************************/

typedef struct
{
        U16                            ue_index;

        rrm_cause_t                    rrm_cause; /*^ M, 0, N, 0 , 0 ^*/
}rrc_rrm_ho_failure_t; /*^ API, RRC_RRM_HO_FAILURE ^*/


/******************************************************************************
     RRC_RRM_HO_CANCEL_REQ
******************************************************************************/

typedef struct
{
        U16                                ue_index;
        rrm_cause_t                    rrm_cause; /*^ M, 0, N, 0 , 0 ^*/
}rrc_rrm_ho_cancel_req_t; /*^ API, RRC_RRM_HO_CANCEL_REQ ^*/


/******************************************************************************
     RRC_RRM_HO_CANCEL_RESP
******************************************************************************/

typedef struct
{
        U16               ue_index;
        U8                 response; /*^ M, 0, H, 0 , 1 ^*//* rrc_return_et */
}rrc_rrm_ho_cancel_resp_t; /*^ API, RRC_RRM_HO_CANCEL_RESP ^*/


/******************************************************************************
     RRC_RRM_HO_RESTRICTION_LIST_IND
******************************************************************************/

typedef struct
{
        U16                         ue_index;
        ho_restriction_list_t   ho_restriction_list; /*^ M, 0, N, 0, 0 ^*/
}rrc_rrm_ho_restriction_list_ind_t; /*^ API, RRC_RRM_HO_RESTRICTION_LIST_IND ^*/


/******************************************************************************
     RRC_RRM_DL_NAS_TRANSPORT_IND
******************************************************************************/

typedef struct
{
        U16                        bitmask; /*^ BITMASK ^*/
#define RRC_RRM_DL_NAS_TRANSPORT_IND_SUBS_PROF_ID_PRESENT            0x01

        U16                        ue_index;
        U16                        subscriber_profile_id_for_rfp; 
        /*^ O, RRC_RRM_DL_NAS_TRANSPORT_IND_SUBS_PROF_ID_PRESENT, B, 1, 256 ^*/

}rrc_rrm_dl_nas_transport_ind_t; /*^ API, RRC_RRM_DL_NAS_TRANSPORT_IND ^*/

/******************************************************************************
     RRC_RRM_UE_HO_CMD_REQ
******************************************************************************/

typedef struct
{
        U8            nas_msg_length;  /*^ M,0,B,1,2 ^*/
        U8            nas_security_param[2];  /*^ M,0,OCTET_STRING,VARIABLE ^*/
}nas_security_param_t;

typedef struct
{
        U16            erab_id; /*^ M, 0, H, 0, 15 ^*/
        rrm_cause_t    rrm_cause;
}erab_to_release_t;

typedef struct _erab_to_release_list_t
{
	/* SPR_17664_start*/
        U8    erab_count; /*^ M, 0, B, 1, MAX_LC_COUNT^*/
        erab_to_release_t   erab_to_release[MAX_LC_COUNT]; 
	/* SPR_17664_end*/
/*^ M,0,OCTET_STRING,VARIABLE ^*/ 
}erab_to_release_list_t;

/* CDMA2000_CSFB_HO start */
typedef enum
{
    CDMA2000_HO_SUCCESS,
    CDMA2000_HO_FAILURE,
    CDMA2000_HO_NO_REPORT
}rrc_rrm_cdma2000_ho_status_et;
/* CDMA2000_CSFB_HO stop */


typedef struct
{ 
    U16                     bitmask; /*^ BITMASK ^*/
#define RRC_RRM_UE_HO_CMD_REQ_NAS_SECURITY_PARAM_PRESENT         0x01
#define RRC_RRM_UE_HO_CMD_REQ_ERAB_SUB_TO_FORWARD_LIST_PRESENT   0x02
#define RRC_RRM_UE_HO_CMD_REQ_ERAB_TO_RELEASE_LIST_PRESENT       0x04
#define RRC_RRM_UE_HO_CMD_REQ_CDMA2000_HO_STATUS_PRESENT         0x08

    U16                     ue_index;
    nas_security_param_t    nas_security_param; 
    /*^ O, RRC_RRM_UE_HO_CMD_REQ_NAS_SECURITY_PARAM_PRESENT, N, 0, 0 ^*/
    rrc_erab_info_list_t        erab_sub_to_fwd_list;
    /*^ O, RRC_RRM_UE_HO_CMD_REQ_ERAB_SUB_TO_FORWARD_LIST_PRESENT, N, 0, 0 ^*/ 
    erab_to_release_list_t  erab_to_release_list; 
    /*^ O, RRC_RRM_UE_HO_CMD_REQ_ERAB_TO_RELEASE_LIST_PRESENT, N, 0, 0 ^*/ 
    
    U8  cdma2000_ho_status[MAX_CDMA2000_NETWORK_TYPES];
    /*^ O, RRC_RRM_UE_HO_CMD_REQ_CDMA2000_HO_STATUS_PRESENT, OCTET_STRING,FIXED ^*/ 
    /* rrc_rrm_cdma2000_ho_status_et */
}rrc_rrm_ue_ho_command_request_t; /*^ API, RRC_RRM_UE_HO_CMD_REQ ^*/

/******************************************************************************
   RRC_RRM_UE_HO_CMD_RESP 
******************************************************************************/

typedef struct
{
    U16                  bitmask; /*^ BITMASK ^*/
#define HO_RESPONSE_CAUSE_PRESENT                   0x01
#define HO_RESPONSE_CS_FALLBACK_INDICATOR_PRESENT   0x02
#define ERAB_LIST_PRESENT                           0x04
#define HO_RESPONSE_CARRIER_FREQ_CDMA2000_PRESENT   0x08
#define HO_RESPONSE_ERAB_LOSSY_HO_LIST_PRESENT      0x10

    U8             response;  /*^ M, 0, H, 0, 1 ^*/   /* rrc_return_et */
    rrm_cause_t    rrm_cause; /*^ O, HO_RESPONSE_CAUSE_PRESENT, N, 0, 0 ^*/
    U8             cs_fallback_indicator;  
    /*^ O, HO_RESPONSE_CS_FALLBACK_INDICATOR_PRESENT, H, 0, 1 ^*/ 
    /*rrc_bool_et*/
    rrc_erab_info_list_t        erab_sub_to_fwd_list;
    /*^ O, ERAB_LIST_PRESENT, N, 0, 0 ^*/
    
    /* Carrier freq shall be present only if RRM decides to send
       redirection info for HRPD */
    carrier_freq_cdma2000_t     carrier_freq_cdma2000;
    /*^ O, HO_RESPONSE_CARRIER_FREQ_CDMA2000_PRESENT, N, 0, 0 ^*/ 
    /* lossy ho chnages start */
    erab_item_list_t            erab_lossy_ho_list;
    /*^ O, HO_RESPONSE_ERAB_LOSSY_HO_LIST_PRESENT, N, 0, 0 ^*/ 
    /* lossy ho changes stop */
}ho_response_t;

typedef struct
{
    U16                          ue_index;
    ho_response_t                ho_command_response; /*^ M, 0, N, 0, 0 ^*/
}rrc_rrm_ue_ho_command_response_t; /*^ API, RRC_RRM_UE_HO_CMD_RESP ^*/

/******************************************************************************
    RRC_RRM_MEAS_CONFIG_REQ 
******************************************************************************/

typedef struct
{
        U16                                ue_index;
        rrm_meas_config_t          meas_config; /*^ M, 0, N, 0, 0 ^*/
}rrc_rrm_meas_config_req_t; /*^ API, RRC_RRM_MEAS_CONFIG_REQ ^*/

/******************************************************************************
        RRC_RRM_MEAS_CONFIG_RESP
******************************************************************************/

typedef struct
{
        U16                  bitmask; /*^ BITMASK ^*/
#define RRC_RRM_MEAS_CONFIG_RESP_CAUSE_PRESENT       0x01
        U16                ue_index;
        U8                  response;      
        /*^ M, 0, H, 0, 1 ^*/   /* rrc_return_et */
        rrm_cause_t    cause;  
        /*^ O, RRC_RRM_MEAS_CONFIG_RESP_CAUSE_PRESENT, N, 0, 0 ^*/
}rrc_rrm_meas_config_response_t; /*^ API, RRC_RRM_MEAS_CONFIG_RESP ^*/

typedef struct
{
    #define RRC_RRM_UE_CONTXT_MOD_AMBR_PRESENT 0x01
    #define RRC_RRM_UE_CONTXT_MOD_CS_FALLBACK_PRESENT 0x02
    #define RRC_RRM_UE_CONTXT_MOD_SPID_PRESENT 0x04
    #define RRC_RRM_UE_CONTXT_MOD_REGISTERED_LAI_PRESENT 0x08
    #define RRC_RRM_UE_CONTXT_MOD_CSG_MEMBERSHIP_STATUS_PRESENT 0x10 
    #define RRC_RRM_UE_CONTXT_MOD_SECURITY_KEY_CHANGE_HO_ONGOING_PRESENT 0x20
    
    U16                                     bitmask;/*^ BITMASK ^*/
    U16                                     ue_index;
    U16                               SPID; /*^ O, RRC_RRM_UE_CONTXT_MOD_SPID_PRESENT, N, 0, 0 ^*/
    rrm_ue_agg_max_bit_rate_t         ue_agg_max_bit_rate; /*^ O, RRC_RRM_UE_CONTXT_MOD_AMBR_PRESENT, N, 0, 0 ^*/
    U32                               CS_Fallback_Indicator; /*^ O, RRC_RRM_UE_CONTXT_MOD_CS_FALLBACK_PRESENT, N, 0, 0 ^*/  
    /* cr_761 structure */
    registered_lai_t                  registered_lai;    /*^ O, RRC_RRM_UE_CONTXT_MOD_REGISTERED_LAI_PRESENT, N, 0, 0 ^*/
    U8                                csg_membership_status;
 /*^ O, RRC_RRM_UE_CONTXT_MOD_CSG_MEMBERSHIP_STATUS_PRESENT, H, 0, 1 ^*/ /* rrc_csg_membership_status_et */

    U8                                security_key_ho_ongoing;  
 /*^ O, RRC_RRM_UE_CONTXT_MOD_SECURITY_KEY_CHANGE_HO_ONGOING_PRESENT, B, 1, 1 ^*/
}rrc_rrm_ue_contxt_mod_req_t;
/*^ API, RRC_RRM_UE_CONTEXT_MOD_REQ ^*/
    
typedef struct
{
    rrc_bitmask_t         bitmask;   /*^ BITMASK ^*/
#define UE_CONTEXT_MOD_RESP_API_FAIL_CAUSE_PRESENCE_FLAG        0x01
    /* CSR#59144 fix start */
#define UE_CONTEXT_MOD_RESP_T304_PRESENCE_FLAG                  0x02
    /* CSR#59144 fix stop */

    /* SPS Stop */
    U16                                     ue_index;
    U8                                      response;       /*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */
    rrm_cause_t           cause; /*^ O, UE_CONTEXT_MOD_RESP_API_FAIL_CAUSE_PRESENCE_FLAG ^*/
    /* CSR#59144 fix start */
    /*BUG 604 changes start*/
    U16                                t304_timer; /*^ O, UE_CONTEXT_MOD_RESP_T304_PRESENCE_FLAG, H, 0, 6 ^*/ /* Depricated IE*/
    /*BUG 604 changes stop*/
    /* CSR#59144 fix stop */

}rrc_rrm_ue_contxt_mod_resp_t;
/*^ API, RRC_RRM_UE_CONTEXT_MOD_RESP ^*/

typedef struct
{

    U16                                     ue_index;
    U8                                      response;       /*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */
}rrc_rrm_ue_contxt_mod_cnf_t;
/*^ API, RRC_RRM_UE_CONTEXT_MOD_CNF ^*/

typedef struct
{
    
    rrc_cell_index_t           cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */

    rrc_warning_info_list_t        warning_info_list;
}rrc_rrm_updated_pws_si_list_req_t;
/*^ API, RRC_RRM_UPDATED_PWS_SI_LIST_REQ ^*/


#define UPDATED_PWS_SI_LIST_RESP_ETWS_INFO_PRESENT    0x0001
#define UPDATED_PWS_SI_LIST_RESP_CMAS_INFO_PRESENT    0x0002
#define UPDATED_PWS_SI_LIST_RESP_SI_TRANSMISSION_INFO_PRESENT   0x0004
typedef struct
{ 
    rrc_bitmask_t    bitmask; /*^ BITMASK ^*/
    rrc_cell_index_t           cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */

    U8               response; 
    
    scheduling_info_list_t         scheduling_info_list;      
   
   rrc_si_transmission_info_list_t    si_transmission_info;
   /*^ O, UPDATED_PWS_SI_LIST_RESP_SI_TRANSMISSION_INFO_PRESENT ^*/

   etws_scheduling_info_t       etws_scheduling_info;
   /*^ O, UPDATED_PWS_SI_LIST_RESP_ETWS_INFO_PRESENT ^*/

   cmas_scheduling_info_list_t   cmas_scheduling_info_list;
   /*^ O, UPDATED_PWS_SI_LIST_RESP_CMAS_INFO_PRESENT ^*/
   
}rrc_rrm_updated_pws_si_list_resp_t;
/*^ API, RRC_RRM_UPDATED_PWS_SI_LIST_RESP ^*/

typedef struct 
{
    rrc_cell_index_t           cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */

    U8 response;
    rrc_warning_info_list_t        warning_info_list;
}rrc_rrm_updated_pws_si_list_cnf_t;
/*^ API, RRC_RRM_UPDATED_PWS_SI_LIST_CNF ^*/

typedef struct _rrc_rrm_inactive_ue_info_t
{
    rrc_ue_index_t    ue_index;
}rrc_rrm_inactive_ue_info_t;

typedef struct _rrc_rrm_inactive_ues_ind_t
{
/*SPR_17664_start*/
    U8        num_of_inactive_ue;  /*^ M, 0, B, 1, RRC_MAX_INACTIVE_UES ^*/
/*SPR_17664_end*/
    rrc_rrm_inactive_ue_info_t  inactive_ue_info[RRC_MAX_INACTIVE_UES]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrc_rrm_inactive_ues_ind_t;
/*^ API, RRC_RRM_INACTIVE_UES_IND ^*/

/*****************************************************************************************
 *   RRC_RRM_CELL_START_RESP
 *************************************************************************************/
#define CELL_START_RESP_API_FAIL_CAUSE_PRESENCE_FLAG        0x01
typedef struct _rrc_rrm_cell_start_resp_t
{
    U16               presence_bitmask;   /*^ BITMASK ^*/
    rrc_cell_index_t    cell_index; 
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
    U8      response;    /*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */
    U8      fail_cause;
/*^ O, CELL_START_RESP_API_FAIL_CAUSE_PRESENCE_FLAG, H, 0, 14 ^*/ /* rrm_fail_cause_et */
}rrc_rrm_cell_start_resp_t;
/*^ API, RRC_RRM_CELL_START_RESP ^*/

/**********************************************************************************
*    RRC_RRM_CELL_START_REQ
*******************************************************************************/
typedef struct _rrc_rrm_cell_start_req_t
{
    rrc_cell_index_t   cell_index; 
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
}rrc_rrm_cell_start_req_t;
/*^ API, RRC_RRM_CELL_START_REQ ^*/

/***********************************************************************************
 *   RRC_RRM_CELL_STOP_REQ
 **********************************************************************************/
typedef struct _rrc_rrm_cell_stop_req_t
{
    rrc_cell_index_t    cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
}rrc_rrm_cell_stop_req_t;
/*^ API, RRC_RRM_CELL_STOP_REQ ^*/

/***********************************************************************************
 *   RRC_RRM_CELL_STOP_RESP
 *********************************************************************************/ 
#define CELL_STOP_RESP_API_FAIL_CAUSE_PRESENCE_FLAG        0x01
typedef struct _rrc_rrm_cell_stop_resp_t
{
    U16                 presence_bitmask;   /*^ BITMASK ^*/
    rrc_cell_index_t    cell_index;
/*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
    
    U8                  response;    /*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */
    U8                  fail_cause;
/*^ O, CELL_STOP_RESP_API_FAIL_CAUSE_PRESENCE_FLAG, H, 0, 14 ^*/ /* rrm_fail_cause_et */
}rrc_rrm_cell_stop_resp_t;
/*^ API, RRC_RRM_CELL_STOP_RESP ^*/

/***********************************************************************************
 *   RRC_RRM_INTRA_ENB_HO_IND
 *********************************************************************************/ 
typedef struct _rrc_rrm_intra_enb_ho_ind_t
{
/*BUG 604 changes start*/
#define RRC_RRM_INTRA_CELL_HO_CAUSE_TYPE_PRESENT 0x01
/*BUG 604 changes stop*/
    rrc_bitmask_t   bitmask;                 /*^ BITMASK ^*/
    rrc_cell_index_t           cell_index;
/*^ M, 0, H, 0, 7 ^*/    /* MAX_NUM_CELLS - 1 */
    U16             ue_index;
/*BUG 604 changes start*/
    U8              intracell_ho_cause;
/*^ O, RRC_RRM_INTRA_CELL_HO_CAUSE_TYPE_PRESENT, H, 0, 1 ^*/ /* rrc_rrm_intracell_ho_cause_type_et */
/*BUG 604 changes stop*/
}rrc_rrm_intra_enb_ho_ind_t; /*^ API, RRC_RRM_INTRA_ENB_HO_IND ^*/

/* CDMA2000_CSFB_HO start */
/*****************************************************************************
    RRC_RRM_CSFB_PARAMETERS_REQ_CDMA2000
******************************************************************************/
typedef struct
{ 
    rrc_bitmask_t     bitmask; /*^ BITMASK ^*/
    rrc_ue_index_t    ue_index;
}rrc_rrm_csfb_parameters_req_cdma2000_t; /*^ API,  RRC_RRM_CSFB_PARAMETERS_REQ_CDMA2000 ^*/

/*****************************************************************************
    RRC_RRM_CSFB_PARAMETERS_RESP_CDMA2000
******************************************************************************/
typedef struct
{
    rrc_bitmask_t    		bitmask;  /*^ BITMASK ^*/
    rrc_ue_index_t   		ue_index;
    mobility_parameters_t   mobility_parameters;  /*^ M, 0, N, 0, 0 ^*/
    additional_mobility_parameters_t    add_mobility_parameters;  /*^ M, 0, N, 0, 0 ^*/
}rrc_rrm_csfb_parameters_resp_cdma2000_t; /*^ API,  RRC_RRM_CSFB_PARAMETERS_RESP_CDMA2000 ^*/
/* CDMA2000_CSFB_HO stop */


/***********************************************************************************
 *   RRC_RRM_COUNTER_CHECK_REQ
 **********************************************************************************/

#define COUNTER_CHECK_REQ_DRB_LIST_PRESENCE_FLAG        0x01
typedef struct _rrc_rrm_counter_check_req_t
{
    U16                          bitmask;/*^ BITMASK ^*/
    U16                          ue_index;
    drb_list_t                   drb_list; 
    /*^ O, COUNTER_CHECK_REQ_DRB_LIST_PRESENCE_FLAG, N, 0, 0 ^*/

}rrc_rrm_counter_check_req_t;
/*^ API, RRC_RRM_COUNTER_CHECK_REQ ^*/

/***********************************************************************************
 *   RRC_RRM_COUNTER_CHECK_RESP
 **********************************************************************************/

#define RRM_COUNTER_CHECK_RESP_FAIL_CAUSE_PRESENCE_FLAG                 0x01
#define RRM_COUNTER_CHECK_RESP_DRB_COUNT_INFO_LIST_PRESENCE_FLAG        0x02
#define RRM_COUNTER_CHECK_RESP_DRB_COUNTMSB_INFO_LIST_PRESENCE_FLAG     0x04
typedef struct _rrc_rrm_counter_check_resp_t
{
    U16                          bitmask;/*^ BITMASK ^*/
    U16                          ue_index;
    U8                           response;
/*^ M, 0, H, 0, 1 ^*/    /* rrc_return_et */
    U8                           fail_cause;
/*^ O, RRM_COUNTER_CHECK_RESP_FAIL_CAUSE_PRESENCE_FLAG ^*/    
    drb_count_info_list_t        drb_count_info_list; 
/*^ O, RRM_COUNTER_CHECK_RESP_DRB_COUNT_INFO_LIST_PRESENCE_FLAG, N, 0, 0 ^*/
    drb_countMSB_info_list_t     drb_countMSB_info_list; 
/*^ O, RRM_COUNTER_CHECK_RESP_DRB_COUNTMSB_INFO_LIST_PRESENCE_FLAG, N, 0, 0 ^*/
}rrc_rrm_counter_check_resp_t;
/*^ API, RRC_RRM_COUNTER_CHECK_RESP ^*/

/***********************************************************************************
 *   RRC_RRM_UE_INFORMATION_REQ
 *********************************************************************************/ 
typedef struct
{
    rrc_bitmask_t              bitmask;                /*^ BITMASK ^*/
    U16                        ue_Index; 
    U8                         rach_report_req;
    /*^ M, 0, H, 0, 1 ^*/   /* rrc_bool_et */
    U8                         rlf_report_req;
    /*^ M, 0, H, 0, 1 ^*/   /* rrc_bool_et */

}rrc_rrm_ue_information_req_t;
/*^ API, RRC_RRM_UE_INFORMATION_REQ ^*/

/***********************************************************************************
 *   RRC_RRM_UE_INFORMATION_RESP
 *********************************************************************************/ 
typedef struct
{
    rrc_bitmask_t              bitmask;                /*^ BITMASK ^*/
    U8                         no_of_preamables_sent;
    /*^ M, 0, B, 1, 200 ^*/
    U8                         contention_detected;
    /*^ M, 0, H, 0, 1 ^*/   /* rrc_bool_et */
}rrc_rach_report_t;

typedef struct
{
    rrc_bitmask_t              bitmask;                /*^ BITMASK ^*/
#define RRC_RRM_UE_INFO_RESP_CAUSE_PRESENT       0x01
#define RRC_UE_INFO_RACH_REPORT_PRESENT          0x02
#define RRC_UE_INFO_RLF_REPORT_PRESENT           0x04
    U16                        ue_Index; 
    U8                         response;    /*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */
    U8                         fail_cause;  
    /*^ O, RRC_RRM_UE_INFO_RESP_CAUSE_PRESENT, H, 0, 2 ^*/
    /* rrm_ue_info_fail_cause_et */
    rrc_rach_report_t          rach_report;
    /*^ O, RRC_UE_INFO_RACH_REPORT_PRESENT ^*/
    rrc_rlf_report_t           rlf_report;
    /*^ O, RRC_UE_INFO_RLF_REPORT_PRESENT ^*/
}rrc_rrm_ue_information_resp_t;
/*^ API, RRC_RRM_UE_INFORMATION_RESP ^*/
typedef struct
{
    arfcn_value_utra_t    carrier_freq;
    /*^ M, 0, N, 0, 0 ^*/
    U8                    meas_prs_offset;
    /*^ M, 0, H, 0, 39 ^*/

}RstdInterFreqInfoList_r10_t;

typedef struct
{
    U8                       num_rstd_intfreq_list;
/*SPR_17664_start*/
    /*^ M, 0, B, 1, MAXRSTD_FREQ ^*/
/*SPR_17664_end*/
    RstdInterFreqInfoList_r10_t                   rstd_interfreq_list[MAXRSTD_FREQ];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

}RstdInterFreqInfoList_t;

typedef struct
{
#define RRM_INTER_FREQ_RSTD_START                      0x01
#define RRM_INTER_FREQ_RSTD_STOP                       0x02
#define RRM_INTER_FREQ_RSTD_INFO_LIST_PRESENT          0x04

    U16                                           bitmask;        /*^ BITMASK ^*/
    U16                                           ue_index;
    RstdInterFreqInfoList_t                       rstd_inter_freq_info_list;
    /*^ O, RRM_INTER_FREQ_RSTD_INFO_LIST_PRESENT, N, 0, 0 ^*/

} rrc_rrm_inter_freq_rstd_measurement_ind_t; /*^ API, RRC_RRM_INTER_FREQ_MEASUREMENT_IND ^*/


/* LIPA start */
/***********************************************************************************
 *   RRC_RRM_LOCAL_ERROR_IND
 *********************************************************************************/ 
typedef struct
{
#define RRM_LOCAL_ERROR_IND_ERAB_ID_PRESENT       0x01
    rrc_bitmask_t              bitmask;   /*^ BITMASK ^*/
    U16                        ue_index; 
    U8                         erab_id; /*^ M, 0, H, 0, 15 ^*/
}rrc_impacted_items_list_t;

typedef struct
{
/*SPR_17664_start*/
    U8        impacted_items_count;  /*^ M, 0, L, 1, MAX_LOCAL_ERROR_IMPACTED_ITEMS ^*/
/*SPR_17664_end*/
    rrc_impacted_items_list_t  impacted_items_list[MAX_LOCAL_ERROR_IMPACTED_ITEMS]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrc_impacted_items_t;

typedef struct
{
#define RRM_LOCAL_ERROR_IND_IMPACTED_ITEM_LIST_PRESENT       0x01
    rrc_bitmask_t              bitmask;   /*^ BITMASK ^*/

    U8                         cause;
    /*^ M, 0,B, 1, 3 ^*/ /* rrc_rrm_error_ind_cause_et */

    rrc_impacted_items_t       impacted_items ;
    /*^ O, RRM_LOCAL_ERROR_IND_IMPACTED_ITEM_LIST_PRESENT ^*/

}rrc_rrm_local_error_ind_t; /*^ API, RRC_RRM_LOCAL_ERROR_IND ^*/
/* LIPA end */


/* re-estab changes start */
typedef struct _rrc_rrm_ue_reestab_config_req_t
{
    /* reestab pcell changes start */
    #define RRC_RRM_UE_REESTAB_FAILURE_CELL_PCI_PRESENT  0x01
   /* reestab pcell changes stop */
    rrc_bitmask_t                 bitmask;       /*^ BITMASK ^*/ 
    rrc_ue_index_t                ue_index;
    U32                           rrc_rrm_re_estab_cause;
    /*^ M, 0, H, 0, 3 ^*/
    /* reestab pcell changes start */
    U16                           failure_cell_pci;
    /*^ O, RRC_RRM_UE_REESTAB_FAILURE_CELL_PCI_PRESENT, N, 0, 0 ^*/
    /* reestab pcell changes stop */
}rrc_rrm_ue_reestab_config_req_t;       /*^ API, RRC_RRM_UE_REESTAB_CONFIG_REQ  ^*/

typedef struct _rrc_rrm_ue_reestab_config_cnf_t
{
    rrc_bitmask_t               bitmask;       /*^ BITMASK ^*/
#define RRC_RRM_UE_REESTAB_CONFIG_CNF_FAIL_CAUSE_PRESENT 0x01

    rrc_ue_index_t               ue_index;

    U8                response;
/*^ M, 0, H, 0, 1 ^*/    /* rrc_return_et */

}rrc_rrm_ue_reestab_config_cnf_t; /*^ API, RRC_RRM_UE_REESTAB_CONFIG_CNF ^*/
/* re-estab changes stop */


/***********************************************************************************
 *   RRC_RRM_WLAN_CONNECTION_STATUS_IND
 *********************************************************************************/ 
/* ENUM for WLAN connection status values */
typedef enum 
{
    WLAN_STATUS_SUCCESSFUL_ASSOCIATION   = 0,
    WLAN_STATUS_FAILURE_WLAN_RADIO_LINK  = 1,
    WLAN_STATUS_FAILURE_WLAN_UNAVAILABLE = 2,
    WLAN_STATUS_FAILURE_TIMEOUT          = 3
}wlan_status_t;

/* WLAN Connection status indication message */
typedef struct _rrc_rrm_wlan_connection_status_ind
{
    rrc_ue_index_t    ue_index;
    /*^ M, O, H, 0, 1 ^*/
  
    wlan_status_t     status;
    /*^ M, 0, H, 0, WLAN_STATUS_FAILURE_TIMEOUT ^*/

}rrc_rrm_wlan_connection_status_ind; /*^ API, RRC_RRM_WLAN_CONNECTION_STATUS_IND ^*/

/* ENDC_MEAS_CHANGES_START */
/***********************************************************************************
 *   RRC_RRM_ENDC_ENABLE_REQ
 *********************************************************************************/

typedef struct _rrc_rrm_endc_meas_config_t
{
    meas_object_to_remove_list_t      meas_object_to_remove_list;
    /*^ M, 0, N, 0, 0 ^*/

    report_config_to_remove_list_t    report_config_to_remove_list;
    /*^ M, 0, N, 0, 0 ^*/

    meas_id_to_remove_list_t          meas_id_to_remove_list;
    /*^ M, 0, N, 0, 0 ^*/

    meas_gap_config_t                 meas_gap_config;
    /*^ M, 0, N, 0, 0 ^*/
}rrc_rrm_endc_meas_config_t;
/* ENDC_MEAS_CHANGES_END */

/* OPTION3X Changes Start */
/***********************************************************************************
 *   RRC_RRM_DC_BEARER_CHANGE_REQ
 *********************************************************************************/ 
typedef struct _rrc_rrm_dc_bearer_change_req_t
{
    rrc_bitmask_t               bitmask;
    /*^ BITMASK ^*/

#define RRM_DC_BEARER_CHANGE_REQ_DRB_TO_MODIFY_LIST_PRESENT    0x01
#define RRM_DC_BEARER_CHANGE_REQ_SGNB_ADD_PARAMS_PRESENT       0x02
#define RRM_DC_BEARER_REQ_MEAS_CONFIG_PRESENT                  0x04

    U16                         ue_index;
    /*^ M, 0, N, 0, 0 ^*/

    rrm_drb_to_add_info_list_t  drb_to_modify_list;
    /*^ O, RRM_DC_BEARER_CHANGE_REQ_DRB_TO_MODIFY_LIST_PRESENT, N, 0, 0 ^*/

    rrc_rrm_sgnb_add_param_t    sgnb_add_param;
    /*^ O, RRM_DC_BEARER_CHANGE_REQ_SGNB_ADD_PARAMS_PRESENT, N, 0, 0 ^*/

    rrc_rrm_endc_meas_config_t  rrc_rrm_endc_meas_config;
    /*^ O, RRM_DC_BEARER_REQ_MEAS_CONFIG_PRESENT, N, 0, 0 ^*/

}rrc_rrm_dc_bearer_change_req_t; /*^ API, RRC_RRM_DC_BEARER_CHANGE_REQ ^*/

/***********************************************************************************
 *   RRC_RRM_DC_BEARER_CHANGE_CNF
 *********************************************************************************/ 
typedef struct _rrc_rrm_dc_bearer_change_cnf_t
{
    rrc_bitmask_t               bitmask;    /*^ BITMASK ^*/
#define RRC_RRM_DC_BEARER_CHANGE_CNF_ERAB_CNF_LIST_PRESENT             0x01
#define RRC_RRM_DC_BEARER_CHANGE_CNF_ERAB_ERROR_LIST_PRESENT           0x02

    U16                         ue_index;

    U8                         response; /* rrc_return_et */
    /*^ M, 0, H, 0, 2 ^*/
    rrm_erab_cnf_list_t         erab_cnf_list;
    /*^ O, RRC_RRM_DC_BEARER_CHANGE_CNF_ERAB_CNF_LIST_PRESENT ^*/
    rrm_erab_error_list_t       erab_error_list;
    /*^ O, RRC_RRM_DC_BEARER_CHANGE_CNF_ERAB_ERROR_LIST_PRESENT ^*/

}rrc_rrm_dc_bearer_change_cnf_t; /*^ API, RRC_RRM_DC_BEARER_CHANGE_CNF ^*/
/* OPTION3X Changes End */

#pragma pack(pop)
#endif  
