/*!
 * \file        rrm_api_types.h
 * \author      gur20435
 * \date        07 Mar 2012
 * \brief       This file contains the rrm types for external modules.
 */
 /*!
  *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
  */


#ifndef _RRM_API_TYPES_H_
#define _RRM_API_TYPES_H_

/*! \headerfile rrm_api_defines.h <>*/
#include "rrm_api_defines.h"



/*! \def HOME_ENB_ID_OCTET_SIZE 
 *  \brief Maximum number of octets in the eNB Id
 */
#define HOME_ENB_ID_OCTET_SIZE                              4
/*SPR 10329:start*/
#define RRM_MAX_NO_OF_ENB_SUPPORTED                         25
#define RRM_MME_MAX_POOLS                                   16
#define RRM_MME_ADD_DELETE_COUNT                            2*(RRM_MME_MAX_POOLS)
#define RRM_MAX_MME_GRP_BYTES                               2
/*SPR 10329:end*/

#define RRMCM_MCC_PRESENCE_BITMASK 0x0001


/*! \ rrm_oam_cell_plmn_info_t 
 *  \brief This struct contains cell PLMN information
 *  \param mcc[MAX_MCC_DIGITS]  Mobile Country Code
 *  \param num_mnc_digit        This can be 2 or 3
 *  \param mnc[MAX_MNC_DIGITS]  Mobile Network Code
 */
typedef struct _rrm_oam_cell_plmn_info_t
{
    U8 						mcc[MAX_MCC_DIGITS]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    U8 						num_mnc_digit; /*^ M, 0, B, 2, 3 ^*/
    U8 						mnc[MAX_MNC_DIGITS]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_oam_cell_plmn_info_t;


/*! \ rrm_oam_eutran_global_cell_id_t 
 *  \brief This struct contains information about Eutran Global Cell Id
 *  \param primary_plmn_id  Public Land Mobile Network Identifier
 *  \param cell_identity[MAX_CELL_IDENTITY_OCTETS] Cell Identity
 */
typedef struct _rrm_oam_eutran_global_cell_id_t
{
    rrm_oam_cell_plmn_info_t   primary_plmn_id; /*^ M, 0, N, 0, 0 ^*/
    U8 			       cell_identity[MAX_CELL_IDENTITY_OCTETS]; /*^ M, 0, OCTET_STRING, FIXED ^*/ 
}rrm_oam_eutran_global_cell_id_t;

/*! \typedef rrm_oam_kpi_t 
 *  \brief   This Struct contains the bitmap for which KPI is requested
 */
typedef struct _rrm_oam_kpi_t
{
    U8 bitmap[RRM_MAX_KPI]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrm_oam_kpi_t;

/*! \typedef rrm_oam_kpi_data_t 
 *  \brief   This Struct contains the parameters. Need to be sent for KPI
 */
typedef struct _rrm_oam_kpi_data_t
{
  rrm_oam_kpi_t kpi_to_report;/*^ M, 0, N, 0, 0 ^*/
  U32 num_of_admitted_csg_user; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_admitted_non_csg_user; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_ue_admission_success; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_ue_admission_fail; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_erb_setup_success; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_erb_setup_fail; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_erb_modify_success; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_erb_modify_fail; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_erb_release_success; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_erb_release_fail; /*^ M, 0, N, 0, 0 ^*/
  U32 avg_dl_allocated_gbr_prb; /*^ M, 0, N, 0, 0 ^*/
  U32 avg_ul_allocated_gbr_prb; /*^ M, 0, N, 0, 0 ^*/
  U32 dl_allocated_ngbr_prb; /*^ M, 0, N, 0, 0 ^*/
  U32 ul_allocated_ngbr_prb; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_geran_ho_attempt; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_geran_ho_fail; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_utran_ho_attempt; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_utran_ho_fail; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_eutran_ho_attempt; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_eutran_ho_fail; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_geran_hi_success; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_geran_hi_fail; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_utran_hi_success; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_utran_hi_fail; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_eutran_hi_success; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_eutran_hi_fail; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_enb_init_ho_csg_usr; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_enb_init_ho_non_csg_usr; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_enb_init_ue_release; /*^ M, 0, N, 0, 0 ^*/
  U32 num_pucch_res_alloc_attempts; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_sr_res_alloc_fail; /*^ M, 0, N, 0, 0 ^*/
  U32 num_of_sr_cqi_alloc_fail; /*^ M, 0, N, 0, 0 ^*/
  U32 total_cell_unavailable_time; /*^ M, 0, N, 0, 0 ^*/ 
  /* Carrier_Aggregation_Start */
  U32 num_of_ca_ue;             /*^ M, 0, N, 0, 0 ^*/
  /* Carrier_Aggregation_End */
}rrm_oam_kpi_data_t;
/* Rat Type enumeration */
/*! \enum rrm_rat_type_et
 *  \brief An enum that specifies the SON RAT type.
 *         Possible Values:
 *         --- EUTRAN
 *         --- UTRAN
 *         --- GERAN
 *         --- CELL UNDEFINED
 */
typedef enum
{
    RRM_EUTRAN_CELL = 1,  /*!< Cell type is EUTRAN */
    RRM_UTRAN_CELL,       /*!< Cell type is UTRAN */
    RRM_GERAN_CELL,       /*!< Cell type is GERAN */
    RRM_CDMA_CELL,
    /* TDD Support SON Start */
    RRM_UTRAN_TDD_CELL,       /*!< Cell type is UTRAN TDD*/
    /* TDD Support SON End */
    RRM_CELL_UNDEFINED = 255
}rrm_rat_type_et;

/*! \  rrm_utran_geran_global_cell_id_t
 *  \brief This struct contains information about Utran Global Cell ID
 *  \param rat_type  Enum specifying rat type
 *  \param plmn_id   Public Land Mobile Network Identifier
 *  \param cell_id   Cell Identity
 */
typedef struct _rrm_utran_geran_global_cell_id_t
{
    rrm_rat_type_et             rat_type;       /*^ M, 0, N, 1, 3 ^*/
    rrm_oam_cell_plmn_info_t    plmn_id;        /*^ M, 0, N, 0, 0 ^*/
    U8                          cell_identity[MAX_CELL_IDENTITY_OCTETS]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrm_utran_geran_global_cell_id_t;

/*! \   rrm_others_global_cell_id_t
 *  \brief This struct contains Others Global Cell ID for furure use
 *  \param bitmask  Bitmask Value
 */
typedef struct _rrm_others_global_cell_id_t
{
    rrm_bitmask_t       bitmask;  /* For Future use */
    /*  To Be Defined Later */
}rrm_others_global_cell_id_t;

#define RRM_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT        0x01
#define RRM_OTHERS_GLOBAL_CELL_ID_PRESENT             0x02

/*! \   rrm_inter_rat_global_cell_id_t
 *  \brief This struct contains Global Cell ID for UTRAN_GERAN and for OTHERS
 *  \param bitmask                      Bitmask Value
 *  \param utran_geran_global_cell_id   Global Cell ID for GERAN and UTRAN
 *  \param others_global_cell_id        Global Cell ID for OTHERS (e.g CDMA)
 */
typedef struct _rrm_inter_rat_global_cell_id_t
{
    rrm_bitmask_t                      bitmask;    /*^ BITMASK ^*/
    rrm_utran_geran_global_cell_id_t   utran_geran_global_cell_id; /*^ O, RRM_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT, N, 0, 0 ^*/
    rrm_others_global_cell_id_t        others_global_cell_id; /*^ O, RRM_OTHERS_GLOBAL_CELL_ID_PRESENT, N, 0, 0 ^*/
}rrm_inter_rat_global_cell_id_t;

#define RRM_INTRA_RAT_GLOBAL_CELL_ID_PRESENT            0x01
#define RRM_INTER_RAT_GLOBAL_CELL_ID_PRESENT            0x02
/*! \   rrm_oam_global_cell_id_t
*  \brief This struct contains Global Cell ID of EUTRAN/UTRAN_GERAN/OTHERS cell types
*  \param bitmask                        Bitmask Value
*  \param intra_rat_global_cell_id       Global cell id for eutran cell type
*  \param inter_rat_global_cell_id       Global cell id for utran, geran and
*                                         others cell types
*/
typedef struct _rrm_oam_global_cell_id_t
{
    rrm_bitmask_t                    bitmask;    /*^ BITMASK ^*/
    rrm_oam_eutran_global_cell_id_t  intra_rat_global_cell_id; /*^ O, RRM_INTRA_RAT_GLOBAL_CELL_ID_PRESENT, N, 0, 0 ^*/
    rrm_inter_rat_global_cell_id_t   inter_rat_global_cell_id;  /*^ O, RRM_INTER_RAT_GLOBAL_CELL_ID_PRESENT, N, 0, 0 ^*/

}rrm_oam_global_cell_id_t;

#define RRM_IPV4_ADD_PRESENT        0x01
#define RRM_IPV6_ADD_PRESENT        0x02
#define RRM_IPV4_SIZE               4
#define RRM_IPV6_SIZE               16

/*! \ rrm_enb_tnl_address_t
 *  \brief    This struct defines the Transport Layer Address
 *  \param    bitmask - To indicate if the optional fields are present or not
              Only one of this bit should be set.Bit 1: ipv4_add and
              Bit 2: ipv6_add
 *  \param    ipv4_add - 32bits IPV4 IP Address.
 *  \param    ipv6_add - 128bits IPV6 IP Address.
 */
typedef struct _rrm_enb_tnl_address_t
{
    rrm_bitmask_t   bitmask;/*^BITMASK^*/
    U8    ipv4_add[RRM_IPV4_SIZE]; /*^O,RRM_IPV4_ADD_PRESENT,OCTET_STRING,FIXED^*/
    U8    ipv6_add[RRM_IPV6_SIZE]; /*^O,RRM_IPV6_ADD_PRESENT,OCTET_STRING,FIXED^*/
}rrm_enb_tnl_address_t;

/*! \enum rrm_enb_type_et
 *  \brief An enum that specifies the enb type.
 */
typedef enum
{
    RRM_ENB_TYPE_HOME = 1,
    RRM_ENB_TYPE_MACRO
}rrm_enb_type_et;

/*! \ rrm_global_enb_id_t
 *  \brief    This struct defines the global eNB Id
 *  \param    enb_type - eNB Type i.e. HOME or MACRO
 *  \param    plmn_id  - Public Land Mobile Network Identifier
 *  \param    enb_id[HOME_ENB_ID_OCTET_SIZE]   - 28 bits Home eNB id or 20 bits Macro eNB id.
                         If eNB type is SON_HOME_ENB, this field is
                         equal to the 28 leftmost bits of the Cell
                         Identity contained in the E-UTRAN CGI of
                         the cell served by the eNB.If eNB type is
                         SON_MACRO_ENB, this field is equal to the 20
                         leftmost bits of the Cell Identity contained
                         in the E-UTRAN CGI of each cell served by the eNB.
 */
typedef struct _rrm_global_enb_id_t
{
    rrm_enb_type_et        enb_type; /*^M,0,B,1,2^*/
    rrm_oam_cell_plmn_info_t   plmn_id; /*^M,0,N,0,0^*/
    U8                 eNb_id[HOME_ENB_ID_OCTET_SIZE]; /*^M,0,OCTET_STRING,FIXED ^*/
}rrm_global_enb_id_t;
/** Values for rrm_return_et, rrm_return_et */
typedef enum
{
    RRM_INDEX_ERR = -1,
    RRM_FAILURE,
    RRM_SUCCESS,
    RRM_PARTIAL_SUCCESS,
    RRM_CELL_INVALID_STATE_FOR_PWS
#ifdef LTE_EMBMS_SUPPORTED
    ,RRM_CELL_NOT_AVAILABLE
#endif
}rrm_return_et;

/*! \enum rrm_bool_et
 *  \brief An enum that specifies values for true/false
*/
typedef enum
{
    RRM_FALSE,
    RRM_TRUE
}rrm_bool_et;

/*! \enum rrm_res_status_et
 *  \brief An enum that specifies status of the resource(Busy/Free)
*/
typedef enum
{
    RRM_RES_FREE,
    RRM_RES_BUSY
}rrm_res_status_et;

/*! \enum rrm_tpc_rnti_status_et
 *  \brief An enum that specifies status of the tpc
*/
typedef enum
{
    RRM_RNTI_FREE,
    RRM_RNTI_BUSY
}rrm_tpc_rnti_status_et;

/*clean_up_start*/
/*! \enum rrm_error_et
 *  \brief An enum that specifies type of erab procedure 
 */
typedef enum
{
        ERAB_SETUP,
        ERAB_MODIFY,
    /* SPR 13718 Fix Start */
        ERAB_HO_ADM
    /* SPR 13718 Fix End */

}rrm_erab_procedure_et;
/*clean_up_end*/

/*csg start*/
/*! \enum rrm_cell_type_t
 *  \brief An enum that specifies type of Cell (UTRA/EUTRA/GERAN/DAHO)
 */
typedef enum
{
#ifndef OAM_IPR
    INVALID,
#endif
    UTRA_CELL,
    EUTRA_CELL,
    GERAN_CELL,
    CDMA2000_CELL,
    DAHO_CELL
}rrm_cell_type_t;
/*csg end*/
/*csfb tgt selection*/
typedef enum
{
    RRM_DO_HO,
    RRM_DO_RELEASE_REDIRECT
}rrm_ho_or_release_et;
/*csfg tgt selection*/
/*! \enum rrm_dci_format_et
 *  \brief An enum that specifies type of dci formats 
 */
typedef enum
{
    RRM_DCI_FORMAT_3,
    RRM_DCI_FORMAT_3A
}rrm_dci_format_et;
/**CLPC_MR_END*/

/*! \enum rrm_error_et
 *  \brief An enum that specifies type of errors
 */
typedef enum
{
    RRM_NO_ERROR                      = 0,     /* Not Error (Success case) */

    /* System Errors */     
    RRM_ERR_SYS_CPU_OVERLOAD          = 1,     /* CPU Overload */
    RRM_ERR_SYS_MEMORY_OVERLOAD       = 2,     /* Memory Overload */
    RRM_ERR_SYS_MEM_ALLOC_FAILURE     = 3,     /* Memory Allocation failure */
    RRM_ERR_SYS_IPC_FAILURE           = 4,     /* IPC failure */
    RRM_ERR_SYS_SYSCALL_FAILURE       = 5,     /* System call failures other than IPC */
    RRM_ERR_LOG_ENABLE_FAILURE        = 6,     /* Log cannot be enabled due to system/platform restrictions */
    RRM_ERR_TIMER_EXPIRED             = 7,     /* Timer expiry */
    RRM_ERR_INIT_CONFIG_FAILURE       = 8,     /* Failure in applying initial configuration other than
                                                  failure in enabling logs */
    RRM_ERR_OBJECT_NON_EXISTENT       = 9,     /* Requested  Object not available at RRM */

    /*Cell Related Errors*/
    RRM_ERR_CALL_ONGOING              = 1001,  /* Call Ongoing */
    RRM_ERR_CELL_ALREADY_SWITCHED_ON  = 1002, /* Cell Already switched on */ 
    RRM_ERR_CELL_ALREADY_SWITCHED_OFF = 1003, /* Cell Already switched off */
    RRM_ERR_CELL_DELETION_FAILURE     = 1004,  /* Cell Deletion Failure  */
    RRM_ERR_CELL_UNOPERATIONAL        = 1005,  /* Cell un-operational */
    RRM_ERR_CELL_UNCONFIGURED         = 1006,  /* Cell un-configured */
    RRM_ERR_CELL_ALREADY_OPERATIONAL  = 1007,  /* Cell already operational */
    RRM_ERR_CELL_ALREADY_CONFIGURED   = 1008,  /* Cell already configured */
    RRM_ERR_CELL_START_FAILURE        = 1009,  /* Cell could not be started */
    RRM_ERR_CELL_CONFIG_FAILURE       = 1010,  /* Cell could not be configured */ 
    RRM_ERR_CELL_STOP_FAILURE         = 1011,  /* Cell could not be stopped  */
    RRM_ERR_CELL_SW_OFF_FAILURE       = 1012,  /* Cell could not be switched off  */
    RRM_ERR_EMERGENCY_CALL_ONGOING    = 1013,  /* Emergency Call Ongoing, (not applicable in the release 1.0) */
    RRM_ERR_ANR_NR_NOT_FOUND          = 1014,  /* No Neighbor Found */
    RRM_ERR_CELL_DELETION_EXPECTED    = 1015,  /* Cell Deleteion is possible*/
    RRM_ERR_EVENT_NOT_POSSIBLE        = 1016,  /* Wrong event recieved in state*/
    RRM_ERR_CELL_ALREADY_REGISTER_FAILURE =1017, /*Cell Already Registered with SON*/ 
    RRM_ERR_CELL_ALREADY_DEREGISTER_FAILURE = 1018, /*Cell Already Deregistered with SON*/
    RRM_ERR_CELL_SET_ATTR_FAILURE = 1019,           /*Failure in Set ATTR For SON*/
    RRM_ERR_MAX_INTER_FREQS_CONFIGURED = 1020,
    RRM_ERR_MAX_UTRA_FDD_FREQS_CONFIGURED  = 1021,
    RRM_ERR_MAX_GERAN_FREQS_CONFIGURED = 1022,
    RRM_ERR_NO_UTRAN_FREQ_CONFIGURED   = 1023,
    RRM_ERR_NO_GERAN_FREQ_CONFIGURED   = 1024,
    RRM_ERR_MANDATORY_PARAMS_ABSENT    = 1025,
    /* SPS related changes start */
    RRM_ERR_CELL_SPS_CONFIG_ERR        = 1026,    /* SPS related error */
    /* SPS related changes end */
    RRM_ERR_MAX_CDMA_FREQS_CONFIGURED = 1027,
    RRM_ERR_NO_CDMA_FREQ_CONFIGURED   = 1028,
    /* TDD Support SON Start */
    RRM_ERR_MAX_UTRA_TDD_FREQS_CONFIGURED  = 1029,
    RRM_ERR_ECGI_NOT_FOUND     = 1030,
    /* TDD Support SON End */

    /*UE  Related Errors*/
    RRM_ERR_RAC_ENABLE_FAILURE        = 2001,  /* RAC cannot be enabled  */
    RRM_ERR_UE_NOT_CONFIGURED         = 2002,
    RRM_ERR_MEAS_OBJ_NOT_FOUND        = 2003,
    RRM_ERR_MAX_MEAS_OBJ_CREATED      = 2004,
    RRM_ERR_PARTIAL_MEAS_CGI_REPORTING= 2005,


    /* Generic Errors */        
    RRM_ERR_REGISTERATION_FAILURE     = 3001,  /* Registration with RRM Failed */
    RRM_ERR_DEREGISTERATION_FAILURE   = 3002,  /* De-Registration with RRM Failed */
    RRM_ERR_INVALID_PARAMS            = 3003,  /* Invalid API Parameters */
    RRM_ERR_UNEXPECTED_MSG            = 3004,  /* Unexpected Message */
    RRM_ERR_INTERNAL_FAILURE          = 3005,   /* Unknown Failure at RRM */
    RRM_ERR_RRC_FAILURE               = 3006,
    /*RACH_optimization_changes_start*/
    RRM_ERR_RACH_CONFIG_FAILURE       = 3007,
    /*RACH_optimization_changes_end*/
    RRM_ERR_TRANS_ID_UNMATCHED        = 3008, /* Coverity: element moved from rrm_reponse_et to rrm_error_et */
    /*SPR 10329:start*/
    RRM_ERR_GU_GROUP_INFO_INVALID     = 3009,
    RRM_ERR_ENB_NOT_FOUND             = 3010,
    RRM_ERR_GU_GROUP_ID_LIMIT_REACHED = 3011,
    RRM_ERR_GU_GROUP_INFO_EMPTY       = 3012,
    RRM_ERR_ENB_GU_GROUP_ID_LIST_EMPTY = 3013,
    /*SPR 10329:end*/

    /* SOFT LOCK Feature related errors */
    RRM_ERR_BLOCK_CELL_TIMER_NOT_PRESENT = 3050,
    RRM_ERR_CELL_ALREADY_BLOCKED         = 3051,
    RRM_ERR_CELL_RECONFIG_ONGOING        = 3052,
    RRM_ERR_CELL_DELETION_ONGOING        = 3053,
    RRM_ERR_CELL_START_ONGOING           = 3054,
    RRM_ERR_CELL_STOP_ONGOING            = 3055,
    RRM_ERR_CELL_SWITCHON_INPROGRESS     = 3056,
    RRM_ERR_CELL_BLOCK_INPROCESS         = 3057,
    RRM_ERR_CELL_NOT_BLOCKED             = 3058,
    RRM_ERR_CELL_BLOCK_HO_FAILURE        = 3059,
    /*DYNAMIC ICIC CHANGES START*/
    RRM_ERR_INCORRECT_PARTITION_INFO     = 3060,
    /*DYNAMIC ICIC CHANGES END*/
    /* cell_index_from_api_header_start */
    RRM_ERR_INVALID_CELL_ID              = 3061,    
    /* cell_index_from_api_header_end */
    /* SPR_13117_FIX_START */
    /*Logging related errors*/
    RRM_ERR_LOGGING_DISABLED             = 3062,   /* Logs are disabled */
    RRM_ERR_REQ_ALREADY_IN_PROGRESS      = 3063,    /* Api is in process */
        /* SPR_13117_FIX_END */
   /* SPR_17367_Start */
    /* SPR_13117_FIX_END */
    RRM_ERR_NO_MME_CONNECTED_TO_TARGET   = 3064    /* No MME found which is connnected to target eNB */
    /* SPR_17367_End */

}rrm_error_et;

/*DYNAMIC ICIC CHANGES START*/
/*! \enum rrm_partitioning_info_action_type_et
 *  \brief An enum that specifies the enb type.
 */
typedef enum
{
    RRM_UL_PARTITION_EXPAND = 1,
    RRM_UL_PARTITION_CONTRACT,
    RRM_UL_PARTITION_NO_CHANGE,
    RRM_DL_PARTITION_EXPAND,
    RRM_DL_PARTITION_CONTRACT,
    RRM_DL_PARTITION_NO_CHANGE
}rrm_partitioning_info_action_type_et;
/*DYNAMIC ICIC CHANGES END*/

/*added for access barring by shivaji */
/*! \enum rrm_oam_ac_barring_factor_et
 *  \brief An enum that specifies the RRM access barring factors
 */

typedef enum
{
  RRM_AC_BARRING_FACTOR_P00,
  RRM_AC_BARRING_FACTOR_P05,
  RRM_AC_BARRING_FACTOR_P10,
  RRM_AC_BARRING_FACTOR_P15,
  RRM_AC_BARRING_FACTOR_P20,
  RRM_AC_BARRING_FACTOR_P25,
  RRM_AC_BARRING_FACTOR_P30,
  RRM_AC_BARRING_FACTOR_P40,
  RRM_AC_BARRING_FACTOR_P50,
  RRM_AC_BARRING_FACTOR_P60,
  RRM_AC_BARRING_FACTOR_P70,
  RRM_AC_BARRING_FACTOR_P75,
  RRM_AC_BARRING_FACTOR_P80,
  RRM_AC_BARRING_FACTOR_P85,
  RRM_AC_BARRING_FACTOR_P90,
  RRM_AC_BARRING_FACTOR_P95
}rrm_oam_ac_barring_factor_et;
/*! \enum rrm_oam_ac_barring_time_et
 *  \brief An enum that specifies the RRM access barring time info
 */
typedef enum
{
  RRM_AC_BARRING_TIME_S4,
  RRM_AC_BARRING_TIME_S8,
  RRM_AC_BARRING_TIME_S16,
  RRM_AC_BARRING_TIME_S32,
  RRM_AC_BARRING_TIME_S64,
  RRM_AC_BARRING_TIME_S128,
  RRM_AC_BARRING_TIME_S256,
  RRM_AC_BARRING_TIME_S512
}rrm_oam_ac_barring_time_et;
/*! \enum rrm_oam_fail_cause_et
 *  \brief An enum that specifies the cause of Failure
 */
typedef enum
{
 RRM_OAM_ERROR_1
}rrm_oam_fail_cause_et;


/*! \enum rrm_son_geran_band_indicator_et
 *  \brief An enum that specifies the Geran Band Indicator
 */
typedef enum
{
    RRM_SON_GERAN_DCS_1800,
    RRM_SON_GERAN_PCS_1900
}rrm_son_geran_band_indicator_et;

/* Soft lock feature related ENUM Start */
/*! \enum rrm_oam_cell_block_priority_et
 *  \brief An enum that specifies the Block Priority of Cell
 */ 
typedef enum
{
    RRM_CELL_BLOCK_HIGH_PRIORITY,
    RRM_CELL_BLOCK_LOW_PRIORITY
}rrm_oam_cell_block_priority_et;

/* Soft lock feature related ENUM End */
/*! \enum rrm_cqi_reporting_mode_et 
 *  \brief An enum that specifies the Periodicity of cqi support
 */
typedef enum
{
        RRM_DISABLE_CQI_REPORTING = 0,
        RRM_PERIODIC_REPORTING_OF_CQI_SUPPORTED,
        RRM_APERIODIC_REPORTING_OF_CQI_SUPPORTED,
        RRM_PERIODIC_APERIODIC_CQI_REPORTING_SUPPORTED
}rrm_cqi_reporting_mode_et;

/* Additional parameters for Cell Config Added STARTS*/
/*! \enum rrm_dci_format_for_si_msg_et
 *  \brief An enum that specifies the DCI format for SI message
 */
typedef enum
{
    RRM_DCI_FORMAT_FORMAT_FOR_SI_MSG_IS_1A = 0,
    RRM_DCI_FORMAT_FORMAT_FOR_SI_MSG_IS_1C
}rrm_dci_format_for_si_msg_et;

/* SPR 15698 Start */
typedef enum
{
   MEAS_REP_EVENT_NONE = 0,              /*!< MEAS_REP_EVENT_NONE */
   MEAS_REP_EVENT_A1,                         /*!< MEAS_REP_EVENT_A1 */
   MEAS_REP_EVENT_A2,                         /*!< MEAS_REP_EVENT_A2 */
   MEAS_REP_EVENT_A3,                         /*!< MEAS_REP_EVENT_A3 */
   MEAS_REP_EVENT_A4,                         /*!< MEAS_REP_EVENT_A4 */
   MEAS_REP_EVENT_A5,                         /*!< MEAS_REP_EVENT_A5 */
   MEAS_REP_EVENT_B1,                         /*!< MEAS_REP_EVENT_B1 */
   MEAS_REP_EVENT_B2,                         /*!< MEAS_REP_EVENT_B2 */
    /* SPR_14564_fix: start */
    MEAS_REP_EVENT_STRONGEST_CELL_FOR_SON,      /*!< MEAS_REP_EVENT_STRONGEST_CELL_FOR_SON */
    /* SPR_14564_fix; end */
    /* SPR 15509 start */
    MEAS_REP_EVENT_FOR_CGI,                     /*!< MEAS_REP_EVENT_FOR_CGI */
    /* SPR 15509 end */
   /* SPR 13445 Fix Start */
   MEAS_REP_EVENT_FORCEFUL_A2,                /*!<MEAS_REP_EVENT_A2 */ /* required in cases when forceful HO has to be done for UE */
   /* SPR 13445 Fix End */
   /* SPR 14247 start */
   MEAS_REP_EVENT_PERIODICAL,                 /*!< MEAS_REP_EVENT_PERIODICAL */
   /* SPR 14247 end  */
/* ENDC_MEAS_CHANGES_START */
#ifdef ENDC_ENABLED
   MEAS_REP_EVENT_B1_NR,                         /*!< MEAS_REP_EVENT_B1_NR */
   MEAS_REP_EVENT_B2_NR,                         /*!< MEAS_REP_EVENT_B2_NR */
#endif
/* ENDC_MEAS_CHANGES_END */
   MEAS_REP_MAX_EVENT                        /*!< MAX no of meas events */
} rrmc_meas_report_event_et;
/* SPR 15698 End */

/*! \enum rrm_ul_sync_loss_timer_et
 *  \brief An enum that specifies Uplink Sync Loss timer
 */
typedef enum
{
    RRM_UL_SYNC_LOSS_TIMER_INFINITY,
    RRM_UL_SYNC_LOSS_TIMER_100,
    RRM_UL_SYNC_LOSS_TIMER_150,
    RRM_UL_SYNC_LOSS_TIMER_200,
    RRM_UL_SYNC_LOSS_TIMER_300,
    RRM_UL_SYNC_LOSS_TIMER_400,
    RRM_UL_SYNC_LOSS_TIMER_500,
    RRM_UL_SYNC_LOSS_TIMER_750,
    RRM_UL_SYNC_LOSS_TIMER_1280,
    RRM_UL_SYNC_LOSS_TIMER_1920,
    RRM_UL_SYNC_LOSS_TIMER_2560,
    RRM_UL_SYNC_LOSS_TIMER_5120,
    RRM_UL_SYNC_LOSS_TIMER_10240
}rrm_ul_sync_loss_timer_et;

/*! \enum rrm_n_gap_et
 *  \brief An enum that specifies N Gap
 */
typedef enum
{
    RRM_NGAP_1 = 0,
    RRM_NGAP_2 = 1
}rrm_n_gap_et;

/* Additional parameters for Cell Config Added gur29990 ENDS*/
/*! \enum rrm_frequency_selective_sched_et 
 *  \brief An enum that specifies enable/disable of frequency selective scheduling in UL/DL
 */
typedef enum
{
    RRM_FREQ_SELECTIVE_SCHEDULING_DISABLE = 0,
    RRM_FREQ_SELECTIVE_SCHEDULING_ENABLE
}rrm_frequency_selective_sched_et;

/*! \enum rrm_ul_dl_freq_selective_sched_et 
 *  \brief An enum that specifies enable/disable of frequency selective scheduling in UL/DL
 */
typedef enum
{
    RRM_FREQ_SELECTIVE_SCHEDULING_DISABLE_UL_DL_BOTH = 0,
    RRM_FREQ_SELECTIVE_SCHEDULING_ENABLE_DL_ONLY,
    RRM_FREQ_SELECTIVE_SCHEDULING_ENABLE_UL_ONLY,
    RRM_FREQ_SELECTIVE_SCHEDULING_ENABLE_UL_DL_BOTH
}rrm_ul_dl_freq_selective_sched_et;

/*csg start*/

/*! \enum rrm_csg_membership_status_et
 *  \brief An enum that specifies membership status of rrm csg
 */
typedef enum
{
    RRM_INVALID,
    RRM_MEMBER,
    RRM_NOT_MEMBER
}rrm_csg_membership_status_et;
/*csg end*/
/*! \enum rrm_eutran_cell_types_et
 *  \brief An enum that specifies Eutran cell types
 */
typedef enum
{
    RRM_EUTRAN_INTRA_FREQ_CELL,
    RRM_EUTRAN_INTER_FREQ_CELL,
    RRM_EUTRAN_INVALID_FREQ_CELL
}rrm_eutran_cell_types_et;

typedef enum
{
    RRM_UE_MEASURMENT_DISABLE = 0,
    RRM_UE_MEASURMENT_ENABLE
}rrm_enable_ue_measurement_et;

/* Start changes for LA feature */
typedef enum
{
    RRM_ESTIMATION_FACTOR_PL = 0,
    RRM_ESTIMATION_FACTOR_SINR
}rrm_ue_speed_estimation_factor_et;

typedef enum
{
    RRM_UE_SPEED_CATEGORY_1 = 1, /*Low speed UE*/
    RRM_UE_SPEED_CATEGORY_2 /*High speed UE*/
}rrm_ue_speed_category_et;
/* End changes for LA feature */

/*TM mode 7/8 start*/
typedef enum
{
    TDD_MODE,
    FDD_MODE
}rrm_enb_mode_t;

typedef enum
{
   BEAM_FORMING_SINR,
   BEAM_FORMING_AOA
}rrm_beam_forming_algo_et;
/*TM mode 7/8 end*/

/*SPR 10329:start*/

typedef struct _rrm_oam_gu_group_id_t 
{
    rrm_oam_cell_plmn_info_t   plmn_id;                              /*^ M, 0, N, 0, 0 ^*/
    U8                         mme_group_id[RRM_MAX_MME_GRP_BYTES];  /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrm_oam_gu_group_id_t;
/*SPR 10329:end*/
/* SPR 22248 MBMS Changes Start */
/* Structures for eMBMS */
#define SUB_FRAME_ALLOC_FOUR_FRAME_OCTET_SIZE   3   
#define RRM_MAX_MBSFN_ALLOCATIONS 8


typedef enum
{
    RRM_OAM_FRAME_ALLOCATION_PERIOD_N1 = 0,
    RRM_OAM_FRAME_ALLOCATION_PERIOD_N2,
    RRM_OAM_FRAME_ALLOCATION_PERIOD_N4,
    RRM_OAM_FRAME_ALLOCATION_PERIOD_N8,
    RRM_OAM_FRAME_ALLOCATION_PERIOD_N16,
    RRM_OAM_FRAME_ALLOCATION_PERIOD_N32 
}rrm_frame_allocation_period_et;

typedef enum
{
    RRM_OAM_NOTIFICATION_REP_COEFF_N2 = 0,
    RRM_OAM_NOTIFICATION_REP_COEFF_N4 
}rrm_oam_notification_rep_coeff_et;

typedef enum
{
    RRM_OAM_MBMS_POWER_OFFSET_1 = 0,
    RRM_OAM_MBMS_POWER_OFFSET_2 ,
    RRM_OAM_MBMS_POWER_OFFSET_3 ,
    RRM_OAM_MBMS_POWER_OFFSET_4 ,
}rrm_oam_cell_power_offset_et;

/*! \ _rrm_subframe_allocation_t 
 *  \brief This struct defines the subframes that are allocated for MBSFN.
 *  \param sf_alloc_choice indicates whether subframe alloacations is in 
 *         one frame or four consecutive frames. 
 *  \param subframe_allocation either one subframe or four subframes are 
 *         allocated 
 */
typedef struct _rrm_subframe_allocation_t
{
    U8  sf_alloc_choice;        /*^ M, 0, H, 0, 1 ^*/
    U8  subframe_allocation[SUB_FRAME_ALLOC_FOUR_FRAME_OCTET_SIZE]; 
                                /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrm_subframe_allocation_t;

/*! \ rrm_mbsfn_config_info_t
 *  \brief This struct contains 
 *  \param sf_alloc_choice indicates whether subframe alloacations is in 
 *          one frame or four consecutive frames. 
 *  \param subframe_allocation either one subframe or four subframes are 
 *          allocated 
 */

#define RRM_OAM_SYNC_STRATUM_ID_PRESENT                 0x01
typedef struct _rrm_mbsfn_config_info_t
{
    rrm_bitmask_t                      bitmask;     /*^ BITMASK ^*/
    rrm_frame_allocation_period_et     radio_frame_allocation_period; 
                     /*^ M, 0, H, 0, 5 ^*/  /*rrm_frame_allocation_period_et*/
    U8                                 radio_frame_allocation_offset; 
                    /*^ M, 0, H, 0, 7 ^*/
    rrm_subframe_allocation_t          subframe_allocation;
                    /*^ M, 0, N, 0, 0 ^*/
    U8                                 sync_stratum_id; 
                    /*^ O, RRM_OAM_SYNC_STRATUM_ID_PRESENT, B, 1, 8 ^*/
}rrm_mbsfn_config_info_t;

/*! \ rrm_mbsfn_subframe_config_list_t
 *  \brief This struct contains 
 *  \param count indicates the number of MBSFN subframe configurations that 
 *          are configured. 
 *  \param mbsfn_subframe_config_info defines the subframes that are reserved 
 *          for MBSFN in downlink. 
 */
typedef struct _rrm_mbsfn_subframe_config_list_t 
{
    U8                      count; /*^ M, 0, B, 1, 8 ^*/
    rrm_mbsfn_config_info_t mbsfn_subframe_config_info[RRM_MAX_MBSFN_ALLOCATIONS]; 
                    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_mbsfn_subframe_config_list_t;


/*!  rrm_oam_sib_type_13_info_t 
 *  \brief This struct contains MBMS NotificationConfig Congurations
 *  \param notification_repetition_coeff indicates the Actual change 
 *      notification repetition period common for all MCCHs
 *  \param notification_offset indicates the  radio frames in which the MCCH 
 *      information change notification is scheduled
 *  \param notification_SF_index indicates the subframe used to transmit MCCH 
 *      change notifications on PDCCH
 *
 */
typedef struct _rrm_oam_sib_type_13_info_t
{
    rrm_oam_notification_rep_coeff_et    notification_repetition_coeff;      
                   /*^ M, 0, H, 0, 1 ^*/ /*rrm_oam_notification_rep_coeff_et*/
    U8             notification_offset;                /*^ M, 0, H, 0, 10 ^*/
    U8             notification_SF_index;              /*^ M, 0, B, 1, 6 ^*/

}rrm_oam_sib_type_13_info_t;

typedef struct _rrm_oam_mbms_power_info_t
{
    rrm_oam_cell_power_offset_et   cell_pwer_offset;  /*^ M, 0, H, 0, 3 ^*/
    U16                            transmission_power; /*^ M, 0, H, 0, 10000 ^*/
}rrm_oam_mbms_power_info_t;
/* Structures for eMBMS */
/* SPR 22248 MBMS Changes Stop */

#endif /*! _RRM_API_TYPES_H_ */
