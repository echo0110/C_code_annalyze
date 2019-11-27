/*********************************************************************
 *
 *  FILE NAME   :umts_sib_decoder_intf.h
 *
 *  DESCRIPTION : File contains the interface API structures.
 *
 *  REVISION HISTORY :
 *
 *  DATE              Name           Reference               Comments
 *  May 28, 2012      Reetesh        --------               Initial
 *  Mar 01, 2014      Rashi          --------               Initial
 *  Mar 01, 2014      Sankar Pradhan --------               Initial
 *
 *  Copyright (c) 2011, Aricent Inc.
 *
 *******************************************************************/
#ifndef __UMTS_SIB_INTF__
#define __UMTS_SIB_INTF__

#include <umtsSibParsing.h>

#define MAX_UTRA_CELL                   8
#define MAX_NAS_GSM_DATA                8
#define MAX_NAS_ANSI_41_DATA            256
#define MAX_CN_DOAMINSYS                4
#define MAX_CELL_MEAS                   32
#define MAX_NO_OF_TIMESLOTS             14
#define MAX_NO_OF_TIMESLOTS_LCR         6
#define MAX_MEAS_EVENT                  8
#define MAX_RAT                         16
#define MAX_MEAS_INTERVALS              3
#define MAX_OTHER_RAT                   15
#define MAX_TR_CH                       32
#define MAX_MEAS_PAR_EVENT              2
#define MAX_NO_OF_TIMESLOTS_LCR_MINUS_1 5
#define MAX_FACH_PCH                    8
#define MAX_PRACH                       16
#define MAC_COMMON_QUEUE_ID             2
#define MAX_PRACH_FPACH                 8
#define MAX_DEDICATED_CSG_FREQ          4
#define MAX_TF                          32
#define MAX_ASC                         8
#define MAX_PUSCH                       8
#define MAX_PDSCH                       8
#define MAX_TFC                         1024
#define MAX_PDSCH_TFCI_GROUPS           256
#define MAX_SCCPCH                      16
#define MAX_NO_OF_TIMESLOTS_MINUS_1     13
#define MAX_HSSCCHS                     4
#define MAX_E_DCHMAC_D_FLOW             8
#define MAX_NUM_E_HICH                  4
#define MAX_NUM_E_AGCH                  4
#define MAX_ERUCCH                      256
#define MAX_EDCHS                       32
#define MAX_TS_LCR                      6
#define MAX_MBSFN_CLUSTERS              16
#define MAX_MNC_OCTET_SIZE              3
#define MAX_MCC_OCTET_SIZE              3
#define UMTS_MAX_PLMN_ID_INFO_SIZE      32
typedef struct
{
    U32         count;
    U8          data[MAX_NAS_GSM_DATA];
}nas_system_gsm_map_info_t;

typedef struct
{
    U32         count;
    U8          data[MAX_NAS_ANSI_41_DATA];
}nas_system_ansi_41_info_t;

#define RRC_UMTS_CN_DOMAINSYSINFO_CN_TYPE_GSM_MAP 1
#define RRC_UMTS_CN_DOMAINSYSINFO_CN_TYPE_ANSI_41 2
typedef struct
{
    U32     t;
    union
    {
        nas_system_gsm_map_info_t            nas_system_gsm_map_info;
        nas_system_ansi_41_info_t            nas_system_ansi_41_info;
    }u;
}cn_domainsysinfo_cn_type_t;

typedef struct
{
    U32                             cn_domain_identity;
    cn_domainsysinfo_cn_type_t      cn_type;
    U8                              cn_drx_cyclelenghtcoefficient;
}cn_domainsys_info_t;        

typedef struct
{
    U8                              count;
    cn_domainsys_info_t             cn_domainsys_info[MAX_CN_DOAMINSYS];
}cn_domainsys_info_list_t;

typedef struct 
{
    U32  t_301;
    U8   n_301;
    U32  t_302;
    U8   n_302;
    U32  t_304;
    U8   n_304;
    U32  t_305;
    U32  t_307;
    U32  t_308;
    U8   t_309;
    U32  t_310;
    U8   n_310;
    U32  t_311;
    U8   t_312;
    U32  n_312;
    U8   t_313;
    U32  n_313;
    U32  t_314;
    U32  t_315;
    U32  n_315;
    U32  t_316;
    U32  t_317;
}ue_conn_timers_consts_t;

typedef struct
{
    U32 t_300;
    U8   n_300;
    U8   t_312;
    U32  n_312;
}ue_idle_timers_consts_t;    

#define UMTS_SIB_TYPE_1_UE_CONNTIMERS_CONST_V3A0EXT_N_312_PRESENCE_FLAG     0x01
#define UMTS_SIB_TYPE_1_UE_CONNTIMERS_CONST_V3A0EXT_N_315_PRESENCE_FLAG     0x02
typedef struct
{
    U16                             presence_bitmask;
    U32                             n_312;
    U32                             n_315;
}ue_conntimers_constants_v3a0ext_t;

#define UMTS_SIB_TYPE_1_UE_IDLETIMERS_CONST_V3A0EXT_N_312_PRESENCE_FLAG     0x01
typedef struct
{
    U16                             presence_bitmask;
    U32                             n_312;
}ue_idletimers_constants_v3a0ext_t;

#define UMTS_SIB_TYPE_1_UE_CONNTIMERS_CONST_V860EXT_T_323_PRESENCE_FLAG    0x01
typedef struct
{
    U16                            presence_bitmask;
    U32                            t_323;
}ue_conntimers_constants_v860ext_t;

typedef struct
{
    ue_conntimers_constants_v860ext_t ue_conntimers_constants_v860ext;
}sysinfotype1_v860ext_ies_t;

typedef struct
{
    U8      __dummy__;
}sysinfotype1_v3a0_nce_v860_nce_nces_t;

#define UMTS_SIB_TYPE_1_SYSINFOTYPE1_V3A0_NCE_V860_NCE_NCES_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                     presence_bitmask;    
    sysinfotype1_v860ext_ies_t              sysinfotype1_v860ext_ies;
    sysinfotype1_v3a0_nce_v860_nce_nces_t   sysinfotype1_v3a0_nce_v860_nce_nces;
}sysInfotype1_v3a0_nce_v860_nce_t;

typedef struct
{
    ue_conntimers_constants_v3a0ext_t   ue_conntimers_constants_v3a0ext;
    ue_idletimers_constants_v3a0ext_t   ue_idletimers_constants_v3a0ext;
}sysinfotype1_v3a0ext_ies_t;

#define UMTS_SIB_TYPE_1_SYSINFOTYPE1_V3A0_NCE_V860_NCE_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                 presence_bitmask;
    sysinfotype1_v3a0ext_ies_t          sysinfotype1_v3a0ext_ies;
    sysInfotype1_v3a0_nce_v860_nce_t    sysInfotype1_v3a0_nce_v860_nce;
}sysinfotype1_v3a0_nce_t;

/********************************************************************/
/*                                                                  */
/*  umts_sib_type_1_info_t - System Information Block type 1        */
/*                                                                  */
/********************************************************************/
#define UMTS_SIB_TYPE_1_CONN_TIMERS_AND_CONST_PRESENCE_FLAG     0x01
#define UMTS_SIB_TYPE_1_IDLE_TIMERS_AND_CONST_PRESENCE_FLAG     0x02
#define UMTS_SIB_TYPE_1_SYSINFOTYPE1_V3A0_NCE_PRESENCE_FLAG     0x04
typedef struct
{
    U16                             presence_bitmask;
    nas_system_gsm_map_info_t       nas_system_gsm_map_info;
    cn_domainsys_info_list_t        cn_domainsys_info_list;
    ue_conn_timers_consts_t         ue_conn_timers_consts;
    ue_idle_timers_consts_t         ue_idle_timers_consts;
    sysinfotype1_v3a0_nce_t         sysinfotype1_v3a0_nce;
}umts_sib_type_1_info_t;

typedef struct
{
    U32                 numbits;
    U8                  ura_identity[4];
}identity_node_t;

typedef struct
{
    U16                     count;
    identity_node_t         identity_node[MAX_UTRA_CELL];
}utra_identity_list_t;

typedef struct 
{
    U8              __dummy__;
}sysinfotype2_non_critical_extensions_t;

/********************************************************************/
/*                                                                  */
/*  umts_sib_type_2_info_t - System Information Block type 2        */
/*                                                                  */
/********************************************************************/
#define UMTS_SIB_UMTS_SIB_TYPE_2_INFO_NON_CRITICAL_EXTENSIONS_PRESENCE_FLAG 0x01
typedef struct 
{
    U16                                         presence_bitmask;
    utra_identity_list_t                        ura_identity_list;
    sysinfotype2_non_critical_extensions_t      non_critical_extensions;
}umts_sib_type_2_info_t;

typedef struct
{
    U32         numbits;
    U8          data[4];
}cell_identity_t;

#define UMTS_SIB_TYPE_3_MAPPING_FUNCTION_PARAM_MAP_PARAM1_PRESENCE_FLAG     0x01
#define UMTS_SIB_TYPE_3_MAPPING_FUNCTION_PARAM_UPPER_LIMIT_PRESENCE_FLAG    0x02
typedef struct
{
    U16             presence_bitmask;
    U32             mapping_function_type;
    U8              map_parameter1;
    U8              map_parameter2;
    U8              upper_limit;
}mapping_function_parameter_t;

typedef struct
{
    U8                              count;
    mapping_function_parameter_t    mapping_function_parameter[MAX_MEAS_INTERVALS];
}mapping_function_parameter_list_t;

typedef struct
{
    U32     rat;
    mapping_function_parameter_list_t       mapping_function_parameter_list;
}mapping_t;

typedef struct
{
    U8          count;
    mapping_t   mapping[MAX_RAT];
}mapping_info_t;

#define UMTS_SIB_TYPE_3_CELL_SELECT_RESELECT_INFO_QUALITY_MEASURE_CPICH_EC_NO_Q_HYST_2_S_PRESENCE_FLAG  0x01
typedef struct
{
    U16             presence_bitmask;
    U8              q_hyst_2_s;
}call_select_reselect_infoSIB3_4_cell_select_quality_measure_cpich_Ec_N0_t;

#define RRC_UMTS_CELLSELECTRESELECTINFOSIB_3_4_CELLSELECTQUALITYMEASURE_CPICH_EC_N0 1
#define RRC_UMTS_CELLSELECTRESELECTINFOSIB_3_4_CELLSELECTQUALITYMEASURE_CPICH_RSCP 2
typedef struct
{
    U32             t;
    union
    {
        /* t = 1 */
        call_select_reselect_infoSIB3_4_cell_select_quality_measure_cpich_Ec_N0_t   cpich_Ec_N0;
        /* t = 2 */
    }u;
}call_select_reselect_infoSIB3_4_cell_select_quality_measure_t;

typedef struct
{
    U8      n_cr;
    U32     t_ct_max_hyst;
}n_cr_t_cr_max_hyst_t;

#define RRC_UMTS_T_CRMAX_NOTUSED          1
#define RRC_UMTS_T_CRMAX_T30              2
#define RRC_UMTS_T_CRMAX_T60              3
#define RRC_UMTS_T_CRMAX_T120             4
#define RRC_UMTS_T_CRMAX_T180             5
#define RRC_UMTS_T_CRMAX_T240             6
typedef struct
{
    U32             t;
    union 
    {   
        /* t = 1 */ 
        /* t = 2 */ 
        n_cr_t_cr_max_hyst_t      t30;
        /* t = 3 */ 
        n_cr_t_cr_max_hyst_t      t60;
        /* t = 4 */ 
        n_cr_t_cr_max_hyst_t      t120;
        /* t = 5 */ 
        n_cr_t_cr_max_hyst_t      t180;
        /* t = 6 */ 
        n_cr_t_cr_max_hyst_t      t240;
    }u;
}T_cr_max_t;

#define UMTS_SIB_TYPE_3_CELL_SELECT_RESELECT_INFO_HSC_SERVING_CELL_INFO_T_CR_MAX_PRESENCE_FLAG     0x01
typedef struct
{
    U16                                 presence_bitmask;
    U8                                  hsc_prio;
    U8                                  q_hcs;
    T_cr_max_t                          t_cr_max;
}hsc_serving_cell_information_t;

#define UMTS_SIB_TYPE_3_CELL_SELECT_RESELECT_INFO_MODE_SPECIFIC_INFO_P_FDD_RAT_FDD_INFO_S_HCS_RAT_PRESENCE_FLAG     0x01
typedef struct
{
    U16             presence_bitmask;
    U32             rat_identifier;
    S8              s_search_rat;
    S8              s_hcs_rat;
    S8              s_limit_search_rat;
}rat_fdd_info_t;

typedef struct
{
    U8                  count;
    rat_fdd_info_t      rat_fdd_info[MAX_OTHER_RAT];
}rat_fdd_info_list_t;

#define UMTS_SIB_TYPE_3_CELL_SELECT_RESELECT_INFO_MODE_SPECIFIC_INFO_P_FDD_INTRA_SEARCH_PRESENCE_FLAG     0x01
#define UMTS_SIB_TYPE_3_CELL_SELECT_RESELECT_INFO_MODE_SPECIFIC_INFO_P_FDD_INTER_SERACH_PRESENCE_FLAG     0x02
#define UMTS_SIB_TYPE_3_CELL_SELECT_RESELECT_INFO_MODE_SPECIFIC_INFO_P_FDD_SEARCH_HCS_PRESENCE_FLAG       0x04
#define UMTS_SIB_TYPE_3_CELL_SELECT_RESELECT_INFO_MODE_SPECIFIC_INFO_P_FDD_RAT_LIST_PRESENCE_FLAG         0x08
typedef struct
{
    U16                               presence_bitmask;
    S8                                s_intra_search;
    S8                                s_inter_search;
    S8                                s_search_hcs;
    rat_fdd_info_list_t               rat_fdd_info_list;
    S8                                q_qual_min;
    S8                                q_rxlev_min;
}call_select_reselect_infoSIB3_4_mode_specific_info_fdd_t;

#define UMTS_SIB_TYPE_3_CELL_SELECT_RESELECT_INFO_MODE_SPECIFIC_INFO_P_TDD_RAT_TDD_INFO_S_HCS_RAT_PRESENCE_FLAG     0x01
typedef struct
{
    U16             presence_bitmask;
    U32             rat_identifier;
    S8              s_search_rat;
    S8              s_hcs_rat;
    S8              s_limit_search_rat;
}rat_tdd_info_t;

typedef struct
{
    U8                  count;
    rat_tdd_info_t      rat_tdd_info[MAX_OTHER_RAT];
}rat_tdd_info_list_t;

#define UMTS_SIB_TYPE_3_CELL_SELECT_RESELECT_INFO_MODE_SPECIFIC_INFO_P_TDD_INTRA_SEARCH_PRESENCE_FLAG     0x01
#define UMTS_SIB_TYPE_3_CELL_SELECT_RESELECT_INFO_MODE_SPECIFIC_INFO_P_TDD_INTER_SEARCH_PRESENCE_FLAG     0x02
#define UMTS_SIB_TYPE_3_CELL_SELECT_RESELECT_INFO_MODE_SPECIFIC_INFO_P_TDD_SEARCH_HCS_PRESENCE_FLAG       0x04
#define UMTS_SIB_TYPE_3_CELL_SELECT_RESELECT_INFO_MODE_SPECIFIC_INFO_P_TDD_RAT_LIST_PRESENCE_FLAG         0x08
typedef struct
{
    U16                                 presence_bitmask;
    S8                                  s_intra_search;
    S8                                  s_inter_search;
    S8                                  s_search_hcs;
    rat_tdd_info_list_t                 rat_tdd_info_list;
    S8                                  q_rxlev_min;
}call_select_reselect_infoSIB3_4_mode_specific_info_tdd_t;

#define RRC_UMTS_CELLSELECTRESELECTINFOSIB_3_4_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_CELLSELECTRESELECTINFOSIB_3_4_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32             t;
    union
    {
        /* t = 1 */
        call_select_reselect_infoSIB3_4_mode_specific_info_fdd_t       fdd;
        /* t = 2 */
        call_select_reselect_infoSIB3_4_mode_specific_info_tdd_t       tdd;
    }u;
}call_select_reselect_infoSIB3_4_mode_specific_info_t;

#define UMTS_SIB_TYPE_3_CELL_SELECT_RESELECT_INFO_MAPPING_INFO_PRESENCE_FLAG            0x01
#define UMTS_SIB_TYPE_3_CELL_SELECT_RESELECT_INFO_HSC_SERVING_CELL_INFO_PRESENCE_FLAG   0x02
typedef struct
{
    U16                                 presence_bitmask;
    mapping_info_t                      mapping_Info;
    call_select_reselect_infoSIB3_4_cell_select_quality_measure_t         cell_select_quality_measure;
    call_select_reselect_infoSIB3_4_mode_specific_info_t                  mode_specific_info;
    U8                                  Hyst_1_S;
    U8                                  Reselection_S;
    hsc_serving_cell_information_t      hsc_serving_cell_Info;
    S8                                  max_AllowedUL_TX_Power;
}cell_select_reselect_infoSIB_3_4_t;

typedef struct
{   
    U32               allowed_indicator;
    U32               t_barred;
}callBarred_barred_t;

#define RRC_UMTS_CELLBARRED_BARRED        1
#define RRC_UMTS_CELLBARRED_NOTBARRED     2
typedef struct
{
    U32             t;
    union 
    {
        /* t = 1 */
        callBarred_barred_t     barred;
        /* t = 2 */
    }u;
}call_barred_t;

typedef struct
{
    U32         n;
    U32         elem[16];
}access_class_barred_list_t;

#define UMTS_SIB_TYPE_3_CELL_ACCESS_RESTRICTION_ACCESS_CLASS_BARRED_LIST_PRESENCE_FLAG  0x01
typedef struct
{
    U16                         presence_bitmask;
    call_barred_t               cell_barred;
    U32                         cell_reserved_for_operator_use;
    U32                         cell_reservation_extenstion;
    access_class_barred_list_t  accessClass_barred_list;
}cell_access_restriction_t;

typedef struct
{
    U32         n;
    U32         elem[2];
}intra_freq_meas_quantity_tdd_sib3_list_t;

typedef struct
{
    intra_freq_meas_quantity_tdd_sib3_list_t    intra_freq_meas_quantity_tdd_sib3_list;
}deferred_measurement_control_reading_support_mode_specific_info_tdd_t;

typedef struct
{
    U32     intra_freq_meas_quantity_fdd;
}deferred_measurement_control_reading_support_mode_specific_info_fdd_t;

#define RRC_UMTS_DEFERREDMEASUREMENTCONTROLREADINGSUPPORT_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_DEFERREDMEASUREMENTCONTROLREADINGSUPPORT_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32         t;
    union
    {
        deferred_measurement_control_reading_support_mode_specific_info_fdd_t   fdd;
        deferred_measurement_control_reading_support_mode_specific_info_tdd_t   tdd;
    }u;
}deferred_measurement_control_reading_support_mode_specific_info_t;

#define UMTS_SIB_DEFERRED_MEASUREMENT_CONTROL_READING_SUPPORT_MODE_SPECIFIC_INFO_PRESENCE_FLAG  0x01
typedef struct
{
    U16             presence_bitmask;
    deferred_measurement_control_reading_support_mode_specific_info_t   mode_specific_info;
}deferred_measurement_control_reading_support_t;

#define UMTS_SIB_TYPE_3_DEFERRED_MEASUREMENT_CONTROL_READING_SUPPORT_PRESENCE_FLAG  0x01
#define UMTS_SIB_TYPE_3_Q_QUALMIN_OFFSET_PRESENCE_FLAG                              0x02
#define UMTS_SIB_TYPE_3_Q_RXLEVMIN_OFFSET_PRESENCE_FLAG                             0x04
#define UMTS_SIB_TYPE_3_MBSFN_ONLYSERVICE_PRESENCE_FLAG                             0x08
typedef struct
{
    U16                     presence_bitmask;
    deferred_measurement_control_reading_support_t  deferred_measurement_control_reading_support;
    U8      q_QualMin_offset;
    U8      q_RxlevMin_offset;
    U32     mbsfn_OnlyService;
}sysinfotype3_v770ext_ies_t;

#define UMTS_SIB_DOMAIN_SPECIFIC_ACCESS_RESTRICTION_V670EXT_RESTRICTION_PRESENCE_FLAG    0x01
typedef struct
{
    U16                             presence_bitmask;
    access_class_barred_list_t      domain_specfic_access_class_barred_list;
}domain_specific_access_restriction_v670ext_restriction_t;

#define RRC_UMTS_DOMAINSPECIFICACCESSRESTRICTION_V670EXT_NORESTRICTION 1
#define RRC_UMTS_DOMAINSPECIFICACCESSRESTRICTION_V670EXT_RESTRICTION 2
typedef struct
{
    U32         t;
    union 
    {
        /* t = 1 */
        /* t = 2 */
        domain_specific_access_restriction_v670ext_restriction_t   restriction;
    }u;
}domain_specific_access_restriction_v670ext_t;

typedef struct
{
    domain_specific_access_restriction_v670ext_t    cs_domain_specific_access_restriction;
    domain_specific_access_restriction_v670ext_t    ps_domain_specific_access_restriction;
}domain_specific_access_restriction_param_v670ext_t;

#define UMTS_SIB_DOMAIN_SPECIFIC_ACCESS_RESTRICTION_PARAMETERS_FOR_OPERATOR1_PRESENCE_FLAG  0x01
#define UMTS_SIB_DOMAIN_SPECIFIC_ACCESS_RESTRICTION_PARAMETERS_FOR_OPERATOR2_PRESENCE_FLAG  0x02
#define UMTS_SIB_DOMAIN_SPECIFIC_ACCESS_RESTRICTION_PARAMETERS_FOR_OPERATOR3_PRESENCE_FLAG  0x04
#define UMTS_SIB_DOMAIN_SPECIFIC_ACCESS_RESTRICTION_PARAMETERS_FOR_OPERATOR4_PRESENCE_FLAG  0x08
#define UMTS_SIB_DOMAIN_SPECIFIC_ACCESS_RESTRICTION_PARAMETERS_FOR_OPERATOR5_PRESENCE_FLAG  0x10
typedef struct
{
    U16                                                 presence_bitmask;
    domain_specific_access_restriction_param_v670ext_t  domain_specific_access_restriction_parameters_for_operator1;
    domain_specific_access_restriction_param_v670ext_t  domain_specific_access_restriction_parameters_for_operator2;
    domain_specific_access_restriction_param_v670ext_t  domain_specific_access_restriction_parameters_for_operator3;
    domain_specific_access_restriction_param_v670ext_t  domain_specific_access_restriction_parameters_for_operator4;
    domain_specific_access_restriction_param_v670ext_t  domain_specific_access_restriction_parameters_for_operator5;
}domain_specific_access_restriction_list_v670ext_t;

#define RRC_UMTS_DOMAINSPECIFICACCESSRESTRICTIONFORSHAREDNETWORK_V670EXT_DOMAINSPECIFICACCESSRESTICTIONLIST 1
#define RRC_UMTS_DOMAINSPECIFICACCESSRESTRICTIONFORSHAREDNETWORK_V670EXT_DOMAINSPECIFICACCESSRESTICTIONPARAMETERSFORALL 2
typedef struct
{
    U32         t;
    union 
    {
        domain_specific_access_restriction_list_v670ext_t   domain_specific_access_restiction_list;
        domain_specific_access_restriction_param_v670ext_t  domain_specific_access_restiction_parameters_forall;
    }u;
}domain_specific_access_restriction_for_shared_network_v670ext_t;

#define UMTS_SIB_DOMAIN_SPECIFIC_ACCESS_RESTRICTION_PARAM_V670EXT_PRESENCE_FLAG                 0x01
#define UMTS_SIB_DOMAIN_SPECIFIC_ACCESS_RESTRICTION_FOR_SHARED_NETWORK_V670EXT_PRESENCE_FLAG    0x02
typedef struct
{
    U16                                                 presence_bitmask;
    domain_specific_access_restriction_param_v670ext_t  domain_specific_access_restriction_parameters_for_plmn_of_mib;
    domain_specific_access_restriction_for_shared_network_v670ext_t   domain_specific_access_restiction_for_shared_network;
}sysinfotype3_v670ext_t;

typedef struct
{
    U32         n;
    U32         elem[15];
}location_registration_access_class_barred_list_t;

#define RRC_UMTS_LOCATIONREGISTRATIONPARAMETERS_NORESTRICTION 1
#define RRC_UMTS_LOCATIONREGISTRATIONPARAMETERS_RESTRICTION 2
typedef struct
{
    U32     t;
    union 
    {
        /* t = 1 */
        /* t = 2 */
        location_registration_access_class_barred_list_t    restriction;
    }u;
}location_registration_parameters_t;

typedef struct
{
    U32     paging_response_restriction_indicator;
    U32     location_registration_restriction_indicator;
    location_registration_parameters_t  location_registration;
}paging_permission_with_access_control_parameters_t;

#define UMTS_SIB_PAGING_PERMISSION_WITH_ACCESS_CONTROL_PARAMETERS_FOR_OPERATOR1_PRESENCE_FLAG   0x01
#define UMTS_SIB_PAGING_PERMISSION_WITH_ACCESS_CONTROL_PARAMETERS_FOR_OPERATOR2_PRESENCE_FLAG   0x02
#define UMTS_SIB_PAGING_PERMISSION_WITH_ACCESS_CONTROL_PARAMETERS_FOR_OPERATOR3_PRESENCE_FLAG   0x04
#define UMTS_SIB_PAGING_PERMISSION_WITH_ACCESS_CONTROL_PARAMETERS_FOR_OPERATOR4_PRESENCE_FLAG   0x08
#define UMTS_SIB_PAGING_PERMISSION_WITH_ACCESS_CONTROL_PARAMETERS_FOR_OPERATOR5_PRESENCE_FLAG   0x10
typedef struct
{
    U16                                                 presence_bitmask;
    paging_permission_with_access_control_parameters_t  paging_permission_with_access_control_parameters_for_operator1;
    paging_permission_with_access_control_parameters_t  paging_permission_with_access_control_parameters_for_operator2;
    paging_permission_with_access_control_parameters_t  paging_permission_with_access_control_parameters_for_operator3;
    paging_permission_with_access_control_parameters_t  paging_permission_with_access_control_parameters_for_operator4;
    paging_permission_with_access_control_parameters_t  paging_permission_with_access_control_parameters_for_operator5;
}paging_permission_with_access_control_list_t;

#define RRC_UMTS_PAGINGPERMISSIONWITHACCESSCONTROLFORSHAREDNETWORK_PAGINGPERMISSIONWITHACCESSCONTROLLIST 1
#define RRC_UMTS_PAGINGPERMISSIONWITHACCESSCONTROLFORSHAREDNETWORK_PAGINGPERMISSIONWITHACCESSCONTROLFORALL 2
typedef struct
{
    U32     t;
    union 
    {
        /* t = 1 */
        paging_permission_with_access_control_list_t        paging_permission_with_access_control_list;
        /* t = 2 */
        paging_permission_with_access_control_parameters_t  paging_permission_with_access_control_for_all;
    }u;
}paging_permission_with_access_control_for_shared_network_t;

#define UMTS_SIB_PAGING_PERMISSION_WITH_ACCESS_CONTROL_PARAMETERS_FOR_PLMN_OF_MIB_PRESENCE_FLAG     0x01
#define UMTS_SIB_PAGING_PERMISSION_WITH_ACCESS_CONTROL_FOR_SHARED_NETWORK_PRESENCE_FLAG             0x02
typedef struct
{
    U16                                                 presence_bitmask;
    paging_permission_with_access_control_parameters_t  paging_permission_with_access_control_parameters_for_plmn_of_mib;
    paging_permission_with_access_control_for_shared_network_t  paging_permission_with_access_control_for_shared_network;
}sysinfotype3_v830ext_ies_t;

typedef struct
{
    U32         n;
    U8          data[4];
}csg_identity_t;

#define UMTS_SIB_PSC_RANGE_2_OFFSET_PRESENCE_FLAG   0x01
typedef struct
{
    U16         presence_bitmask;
    U8          start_psc;
    U32         number_of_pscs;
    U8          psc_range_2_offset;
}csg_psc_split_info_t;

#define UMTS_SIB_TYPE_3_CSG_IDENTITY_PRESENCE_FLAG          0x01
#define UMTS_SIB_TYPE_3_CSG_PSC_SPLIT_INFO_PRESENCE_FLAG    0x02
typedef struct
{
    U16                     presence_bitmask;
    csg_identity_t          csg_identity;
    csg_psc_split_info_t    csg_psc_split_info;
}sysinfotype3_v860ext_ies_t;

#define UMTS_SIB_TYPE_3_CELL_RESERVED_FOR_CSG_PRESENCE_FLAG     0x01
typedef struct
{
    U16     presence_bitmask;
    U32     cell_reserved_for_csg;
}cell_access_restriction_v870ext_t;

#define UMTS_SIB_TYPE_3_CELL_ACCESS_RESTRICTION_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                 presence_bitmask;
    cell_access_restriction_v870ext_t   cell_access_restriction;
}sysinfotype3_v870ext_ies_t;

#define UMTS_SIB_TYPE_3_IMS_EMERGENCY_SUPPORT_INDICATOR_PRESENCE_FLAG   0x01
typedef struct
{
    U16         presence_bitmask;
    U32         ims_emergency_support_indicator;
}sysinfotype3_v920ext_ies_t;

typedef struct
{
    U8      __dummy__;
}sysinfotype3_v4b0nces_v590nce_v5c0nce_v670nce_v770nce_v830nce_v860nce_v870nce_v900nce_nce_t;

#define UMTS_SIB_TYPE_3_V4B0_V590_V5C0_V670_V770_V830_V860_V870_V900_NCE_NCE_PRESENCE_FLAG  0x01
typedef struct
{
    U16                                 presence_bitmask;
    sysinfotype3_v920ext_ies_t          sys_info_type3_v920ext;
    sysinfotype3_v4b0nces_v590nce_v5c0nce_v670nce_v770nce_v830nce_v860nce_v870nce_v900nce_nce_t v900nce_nce;
}sysinfotype3_v4b0nces_v590nce_v5c0nce_v670nce_v770nce_v830nce_v860nce_v870nce_v900nce_t;

#define UMTS_SIB_TYPE_3_V4B0_V590_V5C0_V670_V770_V830_V860_V870_V900_NCE_PRESENCE_FLAG  0x01
typedef struct
{
    U16                                 presence_bitmask;
    sysinfotype3_v870ext_ies_t          sys_info_type3_v870ext;
    sysinfotype3_v4b0nces_v590nce_v5c0nce_v670nce_v770nce_v830nce_v860nce_v870nce_v900nce_t     v900_non_critical_extension;
}sysinfotype3_v4b0nces_v590nce_v5c0nce_v670nce_v770nce_v830nce_v860nce_v870nce_t;

#define UMTS_SIB_TYPE_3_V4B0_V590_V5C0_V670_V770_V830_V860_V870_NCE_PRESENCE_FLAG  0x01
typedef struct
{
    U16                                     presence_bitmask;
    sysinfotype3_v860ext_ies_t              sysInfoType3_v860ext;
    sysinfotype3_v4b0nces_v590nce_v5c0nce_v670nce_v770nce_v830nce_v860nce_v870nce_t      v870_non_critical_extension;
}sysinfotype3_v4b0nces_v590nce_v5c0nce_v670nce_v770nce_v830nce_v860nce_t;

#define UMTS_SIB_TYPE_3_V4B0_V590_V5C0_V670_V770_V830_V860_NCE_PRESENCE_FLAG  0x01
typedef struct
{
    U16                                 presence_bitmask;
    sysinfotype3_v830ext_ies_t          sys_info_type3_v830ext;
    sysinfotype3_v4b0nces_v590nce_v5c0nce_v670nce_v770nce_v830nce_v860nce_t     v860non_critical_extension;
}sysinfotype3_v4b0nces_v590nce_v5c0nce_v670nce_v770nce_v830nce_t;

#define UMTS_SIB_TYPE_3_V4B0_V590_V5C0_V670_V770_V830_NCE_PRESENCE_FLAG  0x01
typedef struct
{
    U16                                 presence_bitmask;
    sysinfotype3_v770ext_ies_t          sys_info_type3_v770ext;
    sysinfotype3_v4b0nces_v590nce_v5c0nce_v670nce_v770nce_v830nce_t v830_non_critical_extension;
}sysinfotype3_v4b0nces_v590nce_v5c0nce_v670nce_v770nce_t;

#define UMTS_SIB_TYPE_3_V4B0_V590_V5C0_V670_V770_NCE_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                                     presence_bitmask;
    sysinfotype3_v670ext_t                                  sys_info_type3_v670ext;
    sysinfotype3_v4b0nces_v590nce_v5c0nce_v670nce_v770nce_t v770non_critical_extension;
}sysinfotype3_v4b0nces_v590nce_v5c0nce_v670nce_t;

#define UMTS_SIB_T_CR_MAX_PRESENCE_FLAG                                         0x01
#define UMTS_SIB_SPEED_DEPENDENT_SCALING_FACTOR_PRESENCE_FLAG                   0x02
#define UMTS_SIB_INTER_FREQUENCY_T_RESELECTION_SCALING_FACTOR_PRESENCE_FLAG     0x04
#define UMTS_SIB_INTER_RAT_T_RESELECTION_SCALING_FACTOR_PRESENCE_FLAG           0x08
typedef struct
{
    U16             presence_bitmask;
    T_cr_max_t      non_hcs_t_cr_max;
    U8              speed_dependent_scaling_factor;
    U8              inter_frequency_t_reselection_scaling_factor;
    U8              inter_rat_t_reselection_scaling_factor;
}cell_select_reselect_info_t_reselection_scaling_v5c0ext_t;

#define UMTS_SIB_TYPE_3_CELL_SELECT_RESELECT_INFO_T_RESELECTION_SCALING_V5C0EXT_PRESENCE_FLAG  0x01
typedef struct
{
    U16                                                         presence_bitmask;
    cell_select_reselect_info_t_reselection_scaling_v5c0ext_t   cell_select_reselect_info_t_reselection_scaling_v5c0ext;
}sysinfotype3_v5c0ext_ies_t;

#define UMTS_SIB_TYPE_3_V4B0_NC_EXT_V590_V5C0_V670_V770_NCE_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                             presence_bitmask;
    sysinfotype3_v5c0ext_ies_t                      sys_info_type3_v5c0ext;
    sysinfotype3_v4b0nces_v590nce_v5c0nce_v670nce_t v670non_critical_extension;
}sysinfotype3_v4b0nces_v590nce_v5c0nce_t;

#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_V590_EXT_DELTA_Q_RX_LEV_MIN_PRESENCE_FLAG    0x01
#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_V590_EXT_DELTA_Q_HCS_PRESENCE_FLAG           0x02
typedef struct
{
    U16             presence_bitmask;
    S8              delta_q_rx_lev_min;
    S8              delta_q_hcs;
}cell_select_reselect_info_v590_ext_t;

#define UMTS_SIB_TYPE_3_CELL_SELECT_RESELECT_INFO_V590_EXT_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                     presence_bitmask;
    cell_select_reselect_info_v590_ext_t    cell_select_reselect_info_v590_ext;
}sysinfotype3_v590ext_t;

#define UMTS_SIB_TYPE_3_V4B0_V590_V5C0_NCE_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                     presence_bitmask;
    sysinfotype3_v590ext_t                  sys_info_type3_v590ext;
    sysinfotype3_v4b0nces_v590nce_v5c0nce_t v5c0noncritical_extension;
}sysinfotype3_v4b0nces_v590nce_t;

typedef struct
{
    mapping_function_parameter_list_t   mapping_function_parameter_list;
}mapping_lcr_r4_t;

#define SYSINFOTYPE3_V4B0EXT_IES_MAPPING_LCR_PRESENCE_FLAG    0x01
typedef struct
{
    U16                 presence_bitmask;
    mapping_lcr_r4_t    mapping_LCR;
}sysinfotype3_v4b0ext_ies_t;

#define UMTS_SIB_TYPE_3_V4B0_V590_NCE_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                 presence_bitmask;
    sysinfotype3_v4b0ext_ies_t          sysinfotype3_v4b0ext_ies;
    sysinfotype3_v4b0nces_v590nce_t     v590NonCriticalExtension;
}sysinfotype3_v4b0_nc_extensions_t;

/********************************************************************/
/*                                                                  */
/*  umts_sib_type_3_info_t - System Information Block type 3        */
/*                                                                  */
/********************************************************************/
#define UMTS_SIB_TYPE_3_V4B0_NC_EXT_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                 presence_bitmask;
    U8                                  sib_4_indicator;
    cell_identity_t                     cell_identity;
    cell_select_reselect_infoSIB_3_4_t  cell_select_reselect_info;
    cell_access_restriction_t           cell_access_restriction;
    sysinfotype3_v4b0_nc_extensions_t   v4b0_nc_extensions;
}umts_sib_type_3_info_t;

typedef struct
{
    S8           aich_poweroffset;
}sysinfo_type5_modespecific_info_fdd_t;

#define RRC_UMTS_REPETITIONPERIODANDLENGTH_REPETITIONPERIOD1 1
#define RRC_UMTS_REPETITIONPERIODANDLENGTH_REPETITIONPERIOD2 2
#define RRC_UMTS_REPETITIONPERIODANDLENGTH_REPETITIONPERIOD4 3
#define RRC_UMTS_REPETITIONPERIODANDLENGTH_REPETITIONPERIOD8 4
#define RRC_UMTS_REPETITIONPERIODANDLENGTH_REPETITIONPERIOD16 5
#define RRC_UMTS_REPETITIONPERIODANDLENGTH_REPETITIONPERIOD32 6
#define RRC_UMTS_REPETITIONPERIODANDLENGTH_REPETITIONPERIOD64 7
typedef struct
{
    U32             t;
    union 
    {   /* t=2 */ 
        U8          repetition_period_2;
        /* t=3 */ 
        U8          repetition_period_4;
        /* t=4 */ 
        U8          repetition_period_8;
        /* t=5 */ 
        U8          repetition_period_16;
        /* t=6 */ 
        U8          repetition_period_32;
        /* t=7 */ 
        U8          repetition_period_64;
    }u;
}repetition_period_n_length_t;

#define UMTS_SIB_TYPE_5_CELL_SELECT_MODE_SPECIFIC_INFO_TDD_PUSCH_SYSINFO_COMMON_TIMESLOT_INFO_TFCI_CODING_PRESENCE_FLAG                 0x01
#define UMTS_SIB_TYPE_5_CELL_SELECT_MODE_SPECIFIC_INFO_TDD_PUSCH_SYSINFO_COMMON_TIMESLOT_INFO_REPETITION_PERIOD_AND_LEN_PRESENCE_FLAG   0x02
typedef struct
{
    U16                             presence_bitmask;
    U32                             second_interLeaving_mode;   
    U32                             tfci_coding;
    U32                             puncturing_limit;
    repetition_period_n_length_t    repetition_PerionAndlength;
}common_timeslot_info_t;

typedef struct
{
    U8          midamble_shift;
}midamble_shift_and_burst_type_burst_type_type1_midamble_allocation_mode_ue_specific_midamble_t;

#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_BURSTTYPE_TYPE1_MIDAMBLEALLOCATIONMODE_DEFAULTMIDAMBLE 1
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_BURSTTYPE_TYPE1_MIDAMBLEALLOCATIONMODE_COMMONMIDAMBLE 2
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_BURSTTYPE_TYPE1_MIDAMBLEALLOCATIONMODE_UESPECIFICMIDAMBLE 3
typedef struct
{
    U32         t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        /* t = 3 */
        midamble_shift_and_burst_type_burst_type_type1_midamble_allocation_mode_ue_specific_midamble_t  ue_specific_midamble;
    }u;
}midamble_shift_and_burst_type_burst_type_type1_midamble_allocation_mode_t;

typedef struct
{
    U32             midamble_configuration_burst_type1_and_3;
    midamble_shift_and_burst_type_burst_type_type1_midamble_allocation_mode_t   midamble_allocation_mode;
}midamble_shift_and_burst_type_burst_type_type1_t;

typedef struct
{
    U8          midamble_shift;
}midamble_shift_and_burst_type_burst_type_type2_midamble_allocation_mode_ue_specific_midamble_t;

#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_BURSTTYPE_TYPE2_MIDAMBLEALLOCATIONMODE_DEFAULTMIDAMBLE 1
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_BURSTTYPE_TYPE2_MIDAMBLEALLOCATIONMODE_COMMONMIDAMBLE 2
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_BURSTTYPE_TYPE2_MIDAMBLEALLOCATIONMODE_UESPECIFICMIDAMBLE 3
typedef struct
{
    U32         t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        /* t = 3 */
        midamble_shift_and_burst_type_burst_type_type2_midamble_allocation_mode_ue_specific_midamble_t  ue_specific_midamble;
    }u;
}midamble_shift_and_burst_type_burst_type_type2_midamble_allocation_mode_t;

typedef struct
{
    U32             midamble_configuration_burst_type2;
    midamble_shift_and_burst_type_burst_type_type2_midamble_allocation_mode_t   midamble_allocation_mode;
}midamble_shift_and_burst_type_burst_type_type2_t;

typedef struct
{
    U8          midamble_shift;
}midamble_shift_and_burst_type_burst_type_type3_midamble_allocation_mode_ue_specific_midamble_t;

#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_BURSTTYPE_TYPE3_MIDAMBLEALLOCATIONMODE_DEFAULTMIDAMBLE 1
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_BURSTTYPE_TYPE3_MIDAMBLEALLOCATIONMODE_UESPECIFICMIDAMBLE 2
typedef struct
{
    U32         t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        midamble_shift_and_burst_type_burst_type_type3_midamble_allocation_mode_ue_specific_midamble_t  ue_specific_midamble;
    }u;
}midamble_shift_and_burst_type_burst_type_type3_midamble_allocation_mode_t;

typedef struct
{
    U32             midamble_configuration_burst_type1_and_3;
    midamble_shift_and_burst_type_burst_type_type3_midamble_allocation_mode_t   midamble_allocation_mode;
}midamble_shift_and_burst_type_burst_type_type3_t;

#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_BURSTTYPE_TYPE1 1
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_BURSTTYPE_TYPE2 2
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_BURSTTYPE_TYPE3 3
typedef struct
{
    U32             t;
    union
    {
        midamble_shift_and_burst_type_burst_type_type1_t    type1;
        midamble_shift_and_burst_type_burst_type_type2_t    type2;
        midamble_shift_and_burst_type_burst_type_type3_t    type3;
    }u;
}midamble_shift_and_burst_type_burst_type_t;

typedef struct
{
    midamble_shift_and_burst_type_burst_type_t  burst_type;
}midamble_shift_and_burst_type_t;



typedef struct
{
    U8                                timeslot_num;
    U8                                tfci_existence;
    midamble_shift_and_burst_type_t   midamble_shift_and_burst_type;
}individual_timeslot_info_t;

typedef struct
{
    U8              num_additional_timeslots;
}uplink_timeslots_codes_more_timeslots_additional_timeslots_consecutive_t;

typedef struct
{
    U8              timeslot_number;
}uplink_additional_timeslots_parameters_same_as_last_t;

typedef struct
{
    U32      n;
    U32      elem[2];
}ul_ts_channelisation_code_list_t;

typedef struct
{
    individual_timeslot_info_t          individual_timeslot_info;
    ul_ts_channelisation_code_list_t    ul_ts_channelisation_code_list;
}uplink_additional_timeslots_parameters_new_parameters_t;

#define RRC_UMTS_UPLINKADDITIONALTIMESLOTS_PARAMETERS_SAMEASLAST 1
#define RRC_UMTS_UPLINKADDITIONALTIMESLOTS_PARAMETERS_NEWPARAMETERS 2
typedef struct
{
    U32                 t;
    union
    {
        uplink_additional_timeslots_parameters_same_as_last_t   same_as_last;
        uplink_additional_timeslots_parameters_new_parameters_t new_parameters;
    }u;
}uplink_additional_timeslots_parameters_t;

typedef struct
{
    uplink_additional_timeslots_parameters_t    parameters;
}uplink_additional_timeslots_t;

typedef struct
{
    U8                              count;
    uplink_additional_timeslots_t   uplink_additional_timeslots[MAX_NO_OF_TIMESLOTS_MINUS_1];
}uplink_timeslots_codes_more_timeslots_additional_timeslots_timeslot_list_t;

#define RRC_UMTS_UPLINKTIMESLOTSCODES_MORETIMESLOTS_ADDITIONALTIMESLOTS_CONSECUTIVE 1
#define RRC_UMTS_UPLINKTIMESLOTSCODES_MORETIMESLOTS_ADDITIONALTIMESLOTS_TIMESLOTLIST 2
typedef struct
{
    U32             t;
    union
    {
        uplink_timeslots_codes_more_timeslots_additional_timeslots_consecutive_t    consecutive;
        uplink_timeslots_codes_more_timeslots_additional_timeslots_timeslot_list_t  timeslot_list;
    }u;
}uplink_timeslots_codes_more_timeslots_additional_timeslots_t;

#define RRC_UMTS_UPLINKTIMESLOTSCODES_MORETIMESLOTS_NOMORE 1
#define RRC_UMTS_UPLINKTIMESLOTSCODES_MORETIMESLOTS_ADDITIONALTIMESLOTS 2
typedef struct
{
    U32         t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        uplink_timeslots_codes_more_timeslots_additional_timeslots_t    additional_timeslots;
    }u;
}uplink_timeslots_codes_more_timeslots_t;

typedef struct
{
    U8                                       dynamic_fusage;
    individual_timeslot_info_t               first_individual_timeslot_info;
    ul_ts_channelisation_code_list_t         ul_ts_channelisation_code_list;
    uplink_timeslots_codes_more_timeslots_t  more_timeslots;
}uplink_timeslots_codes_t;

#define UMTS_SIB_TYPE_5_CELL_SELECT_MODE_SPECIFIC_INFO_TDD_PUSCH_SYSINFO_COMMON_TIMESLOT_INFO_PRESENCE_FLAG     0x01
#define UMTS_SIB_TYPE_5_CELL_SELECT_MODE_SPECIFIC_INFO_TDD_PUSCH_SYSINFO_TIMESLOTS_CODES_PRESENCE_FLAG          0x02
typedef struct
{
    U16                             presence_bitmask;
    U8                              tfcs_id;
    common_timeslot_info_t          common_timeslot_info;
    uplink_timeslots_codes_t        pusch_timeslots_codes;  
}pusch_info_t;

#define UMTS_SIB_TYPE_5_CELL_SELECT_MODE_SPECIFIC_INFO_TDD_PUSCH_SYSINFO_BITMODE_RLC_SIZEINFO_SIZETYPE2_PART2_PRESENCE_FLAG     0x01
typedef struct
{
    U16             presence_bitmask;
    U8              part1;
    U8              part2;
}bitmode_rlc_sizeinfo_sizetype2_t;

#define UMTS_SIB_TYPE_5_CELL_SELECT_MODE_SPECIFIC_INFO_TDD_PUSCH_SYSINFO_BITMODE_RLC_SIZEINFO_SIZETYPE3_PART2_PRESENCE_FLAG     0x01
typedef struct
{
    U16             presence_bitmask;
    U8              part1;
    U8              part2;
}bitmode_rlc_sizeinfo_sizetype3_t;

#define UMTS_SIB_TYPE_5_CELL_SELECT_MODE_SPECIFIC_INFO_TDD_PUSCH_SYSINFO_BITMODE_RLC_SIZEINFO_SIZETYPE4_PART2_PRESENCE_FLAG     0x01
typedef struct
{
    U16             presence_bitmask;
    U8              part1;
    U8              part2;
}bitmode_rlc_sizeinfo_sizetype4_t;

#define RRC_UMTS_BITMODERLC_SIZEINFO_SIZETYPE1 1
#define RRC_UMTS_BITMODERLC_SIZEINFO_SIZETYPE2 2
#define RRC_UMTS_BITMODERLC_SIZEINFO_SIZETYPE3 3
#define RRC_UMTS_BITMODERLC_SIZEINFO_SIZETYPE4 4
typedef struct
{
    U32         t;
    union
    {
        U8                                               size_Type1;
        bitmode_rlc_sizeinfo_sizetype2_t                 size_Type2;
        bitmode_rlc_sizeinfo_sizetype3_t                 size_Type3;
        bitmode_rlc_sizeinfo_sizetype4_t                 size_Type4;
    }u;
}bitmode_rlc_sizeinfo_t;

#define UMTS_SIB_TYPE_5_CELL_SELECT_MODE_SPECIFIC_INFO_TDD_PUSCH_SYSINFO_OCTETMODE_RLC_SIZEINFO_TYPE1_SIZETYPE2_PART2_PRESENCE_FLAG     0x01
typedef struct
{
    U16             presence_bitmask;
    U8              part1;
    U8              part2;
}octetmode_rlc_sizeinfo_type1_sizetype2_t;

#define UMTS_SIB_TYPE_5_CELL_SELECT_MODE_SPECIFIC_INFO_TDD_PUSCH_SYSINFO_OCTETMODE_RLC_SIZEINFO_TYPE1_SIZETYPE3_PART2_PRESENCE_FLAG     0x01
typedef struct
{
    U16             presence_bitmask;
    U8              part1;
    U8              part2;
}octetmode_rlc_sizeinfo_type1_sizetype3_t;

#define RRC_UMTS_OCTETMODERLC_SIZEINFOTYPE1_SIZETYPE1 1
#define RRC_UMTS_OCTETMODERLC_SIZEINFOTYPE1_SIZETYPE2 2
#define RRC_UMTS_OCTETMODERLC_SIZEINFOTYPE1_SIZETYPE3 3
typedef struct
{
    U32         t;
    union
    {
        U8                                            size_type1;
        octetmode_rlc_sizeinfo_type1_sizetype2_t      sizeType2;
        octetmode_rlc_sizeinfo_type1_sizetype3_t      sizeType3;
    }u;
}octetmode_rlc_sizeinfo_type1_t;

#define RRC_UMTS_DEDICATEDDYNAMICTF_INFO_RLC_SIZE_BITMODE 1
#define RRC_UMTS_DEDICATEDDYNAMICTF_INFO_RLC_SIZE_OCTETMODETYPE1 2
typedef struct
{
    U32         t;
    union
    {  
        bitmode_rlc_sizeinfo_t                           bit_mode;
        octetmode_rlc_sizeinfo_type1_t                   octet_modeType1;
    }u;
}dedicated_dynamic_tf_info_rlc_size_t;

#define RRC_UMTS_NUMBEROFTRANSPORTBLOCKS_ZERO 1
#define RRC_UMTS_NUMBEROFTRANSPORTBLOCKS_ONE 2
#define RRC_UMTS_NUMBEROFTRANSPORTBLOCKS_SMALL_ 3
#define RRC_UMTS_NUMBEROFTRANSPORTBLOCKS_LARGE_ 4
typedef struct
{
    U32         t;
    union
    {
        U8                          small_;
        U16                         large_;
    }u;
}number_of_transport_blocks_t;

typedef struct
{
    U8                  count;
    number_of_transport_blocks_t        numTransport_blocks[32];
}dedicated_dynamic_tf_info_num_of_tb_size_list_t;

#define UMTS_SIB_TYPE_5_CELL_SELECT_MODE_SPECIFIC_INFO_TDD_PUSCH_SYSINFO_LOGICAL_CHANNEL_BY_RB_PRESENCE_FLAG     0x01
typedef struct
{
    U16                     presence_bitmask;
    U8                      rb_identity;
    U8                      logCh_of_Rb;
}logicalChannel_by_RB_t;

typedef struct
{
    U8                          count;
    logicalChannel_by_RB_t      logicalChannel_by_RB[15];
}logical_channel_list_explicitList_t;

#define RRC_UMTS_LOGICALCHANNELLIST_ALLSIZES 1
#define RRC_UMTS_LOGICALCHANNELLIST_CONFIGURED 2
#define RRC_UMTS_LOGICALCHANNELLIST_EXPLICITLIST 3
typedef struct
{
    U32         t;
    union   
    {
        /* t = 1 */
        /* t = 2 */
        /* t = 3 */
        logical_channel_list_explicitList_t     explicit_list;
    }u;
}logical_channel_list_t;

typedef struct
{
    dedicated_dynamic_tf_info_rlc_size_t                rlc_size;
    dedicated_dynamic_tf_info_num_of_tb_size_list_t     num_of_Tb_size_list;
    logical_channel_list_t                              logical_channel_list;
}dedicated_dynamic_tf_info_t;

typedef struct
{
    U8                              count;
    dedicated_dynamic_tf_info_t     dedicated_dynamic_tf_info[MAX_TF];
}dedicated_dynamic_tf_info_list_t;

#define RRC_UMTS_DEDICATEDDYNAMICTF_INFO_DYNAMICTTI_RLC_SIZE_BITMODE 1
#define RRC_UMTS_DEDICATEDDYNAMICTF_INFO_DYNAMICTTI_RLC_SIZE_OCTETMODETYPE1 2
typedef struct
{
    U32         t;
    union
    {
        bitmode_rlc_sizeinfo_t                            bit_mode;
        octetmode_rlc_sizeinfo_type1_t                    octet_modeType1;
    }u;
}dedicated_dynamic_tf_info_dynamic_tti_rlc_size_t;

typedef struct
{
    number_of_transport_blocks_t        numTransport_blocks; 
    U32                                 transmissionTime_inverval;
}number_of_tb_size_n_tti_list_element_t;

typedef struct
{
    U8                                         count;
    number_of_tb_size_n_tti_list_element_t     num_of_tb_sizeAndTTL_listElemet[32];
}number_of_tb_size_n_tti_list_t;

typedef struct
{
    dedicated_dynamic_tf_info_dynamic_tti_rlc_size_t        rlc_size;
    number_of_tb_size_n_tti_list_t                          num_of_Tb_size_and_TTIList;
    logical_channel_list_t                                  logical_channel_list;
}dedicated_dynamic_tf_info_dynamic_tti_t;

typedef struct
{
    U8                                          count;
    dedicated_dynamic_tf_info_dynamic_tti_t     dedicatedDynamicTF_info_DynamicTTI[MAX_TF];
}dedicated_dynamic_tf_info_list_dynamic_tti_t;

#define RRC_UMTS_DEDICATEDTRANSCHTFS_TTI_TTI10 1
#define RRC_UMTS_DEDICATEDTRANSCHTFS_TTI_TTI20 2
#define RRC_UMTS_DEDICATEDTRANSCHTFS_TTI_TTI40 3
#define RRC_UMTS_DEDICATEDTRANSCHTFS_TTI_TTI80 4
#define RRC_UMTS_DEDICATEDTRANSCHTFS_TTI_DYNAMIC 5
typedef struct
{
    U32         t;
    union 
    {   
        dedicated_dynamic_tf_info_list_t                 tti10;
        dedicated_dynamic_tf_info_list_t                 tti20;
        dedicated_dynamic_tf_info_list_t                 tti40;
        dedicated_dynamic_tf_info_list_t                 tti80;
        dedicated_dynamic_tf_info_list_dynamic_tti_t     dynamic;
    }u;
}dedicated_trans_ch_tfs_tti_t;

#define RRC_UMTS_CHANNELCODINGTYPE_NOCODING 1
#define RRC_UMTS_CHANNELCODINGTYPE_CONVOLUTIONAL 2
#define RRC_UMTS_CHANNELCODINGTYPE_TURBO  3
typedef struct
{
    U32         t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        U32                     codingRate_convolutional;
        /* t = 3 */
    }u;
}channel_coding_type_t;

typedef struct
{   
    channel_coding_type_t           channel_codingType;
    U16                             rateMatchingAttribute;
    U32                             CRC_size;
}semistatic_tf_information_t;

typedef struct
{   
    dedicated_trans_ch_tfs_tti_t        tti;    
    semistatic_tf_information_t         semistaticTF_information; 
}dedicated_trans_ch_tfs_t;

#define RRC_UMTS_OCTETMODERLC_SIZEINFOTYPE2_SIZETYPE1 1
#define RRC_UMTS_OCTETMODERLC_SIZEINFOTYPE2_SIZETYPE2 2
#define RRC_UMTS_OCTETMODERLC_SIZEINFOTYPE2_SIZETYPE3 3
typedef struct
{
    U32         t;
    union
    {
        U8                  size_Type1;
        U8                  size_Type2;
        U8                  size_Type3;
    }u;
}octet_mode_rlc_size_info_type2_t;

typedef struct
{
    octet_mode_rlc_size_info_type2_t        octet_mode_RLC_sizeInfoType2;
}common_dynamic_tf_info_rlc_size_fdd_t;

#define RRC_UMTS_COMMONDYNAMICTF_INFO_RLC_SIZE_TDD_COMMONTDD_CHOICE_BITMODERLC_SIZEINFO 1
#define RRC_UMTS_COMMONDYNAMICTF_INFO_RLC_SIZE_TDD_COMMONTDD_CHOICE_OCTETMODERLC_SIZEINFOTYPE1 2
typedef struct
{
    U16             t;
    union
    {
        bitmode_rlc_sizeinfo_t                           bit_modeRLC_sizeinfo;
        octetmode_rlc_sizeinfo_type1_t                   octet_mode_RLC_sizeinfoType1;
    }u;
}common_dynamic_tf_info_rlc_size_tdd_common_tdd_choice_t;

typedef struct
{
    common_dynamic_tf_info_rlc_size_tdd_common_tdd_choice_t   commonTDD_choice;
}common_dynamic_tf_info_rlc_size_tdd_t;

#define RRC_UMTS_COMMONDYNAMICTF_INFO_RLC_SIZE_FDD 1
#define RRC_UMTS_COMMONDYNAMICTF_INFO_RLC_SIZE_TDD 2
typedef struct
{
    U32         t;
    union
    {
        common_dynamic_tf_info_rlc_size_fdd_t      fdd;
        common_dynamic_tf_info_rlc_size_tdd_t      tdd;
    }u;
}common_dynamic_tf_info_rlc_size_t;

typedef struct
{
    U8              count;
    number_of_transport_blocks_t        numTransportBlocks[32];
}common_dynamic_tf_info_num_of_tb_size_list_t;

typedef struct
{
    common_dynamic_tf_info_rlc_size_t               rlc_size;
    common_dynamic_tf_info_num_of_tb_size_list_t    num_of_TBsize_list;
    logical_channel_list_t                          logical_channel_list;
}common_dynamic_tf_info_t;

typedef struct
{
    U8                              count;
    common_dynamic_tf_info_t        common_dynamicTF_info[MAX_TF];
}common_dynamic_tf_info_list_t;

#define RRC_UMTS_COMMONDYNAMICTF_INFO_DYNAMICTTI_COMMONTDD_CHOICE_BITMODERLC_SIZEINFO 1
#define RRC_UMTS_COMMONDYNAMICTF_INFO_DYNAMICTTI_COMMONTDD_CHOICE_OCTETMODERLC_SIZEINFOTYPE1 2
typedef struct
{
    U16                                 t;
    union
    {
        bitmode_rlc_sizeinfo_t          bitmode_RLC_sizeInfo;
        octetmode_rlc_sizeinfo_type1_t  octetmode_RLC_sizeInfoType1;
    }u;
}common_dynamic_tf_info_dynamic_tti_common_tdd_choice_t;

typedef struct
{
    common_dynamic_tf_info_dynamic_tti_common_tdd_choice_t      commonTDD_choice;
    number_of_tb_size_n_tti_list_t                              num_of_Tb_size_and_TTIList;
    logical_channel_list_t                                      logical_channel_list;
}common_dynamic_tf_info_dynamic_tti_t;

typedef struct
{
    U8                                       count;
    common_dynamic_tf_info_dynamic_tti_t     common_dynamic_tf_info_dynamicTTI[MAX_TF];
}common_dynamic_tf_info_list_dynamic_tti_t;

#define RRC_UMTS_COMMONTRANSCHTFS_TTI_TTI10 1
#define RRC_UMTS_COMMONTRANSCHTFS_TTI_TTI20 2
#define RRC_UMTS_COMMONTRANSCHTFS_TTI_TTI40 3
#define RRC_UMTS_COMMONTRANSCHTFS_TTI_TTI80 4
#define RRC_UMTS_COMMONTRANSCHTFS_TTI_DYNAMIC 5
typedef struct
{
    U32                                             t;
    union
    {
        common_dynamic_tf_info_list_t                tti10;
        common_dynamic_tf_info_list_t                tti20;
        common_dynamic_tf_info_list_t                tti40;
        common_dynamic_tf_info_list_t                tti80;
        common_dynamic_tf_info_list_dynamic_tti_t    dynamic;
    }u;
}common_trans_ch_tfs_tti_t;

typedef struct
{
    common_trans_ch_tfs_tti_t           tti;
    semistatic_tf_information_t         semistatic_TF_Information;
}common_trans_ch_tfs_t;

#define RRC_UMTS_TRANSPORTFORMATSET_DEDICATEDTRANSCHTFS 1
#define RRC_UMTS_TRANSPORTFORMATSET_COMMONTRANSCHTFS 2
typedef struct
{
    U32         t;
    union
    {
        dedicated_trans_ch_tfs_t    dedicated_trans_ChTFS;
        common_trans_ch_tfs_t       common_transChTFS;
    }u;
}transport_format_set_t;

typedef struct
{
    U8          gain_factor_beta_c;
}signalled_gain_factors_mode_specific_info_fdd_t;

#define RRC_UMTS_SIGNALLEDGAINFACTORS_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_SIGNALLEDGAINFACTORS_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32                                      t;
    union
    {
        /* t = 1 */
        signalled_gain_factors_mode_specific_info_fdd_t   fdd;
        /* t = 2 */
    }u;
}signalled_gain_factors_mode_specific_info_t;

#define UMTS_SIB_TYPE_5_TFCS_RECONF_ADD_CTFCSIZE_CTFC2BIT_ELEMENT_SIGNALLED_GAIN_FACTORS_REF_TFC_ID_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                             presence_bitmask;
    signalled_gain_factors_mode_specific_info_t     mode_specific_info;
    U8                                              gain_factor_beta_d;
    U8                                              reference_tfc_id;
}signalled_gain_factors_t;

#define RRC_UMTS_GAINFACTORINFORMATION_SIGNALLEDGAINFACTORS 1
#define RRC_UMTS_GAINFACTORINFORMATION_COMPUTEDGAINFACTORS 2
typedef struct
{
    U32                 t;
    union
    {
        signalled_gain_factors_t    signalled_gain_factors;
        U8                          computed_gain_factors;
    }u;
}gain_factor_information_t;

#define UMTS_SIB_TYPE_5_TFCS_RECONF_ADD_CTFCSIZE_CTFC2BIT_ELEMENT_POWER_OFF_INFO_PP_M_PRESENCE_FLAG  0x01
typedef struct
{
    U16                             presence_bitmask;
    gain_factor_information_t       gainFactor_Information;
    S8                              powerOffset_Pp_m;
}power_offset_information_t;

#define UMTS_SIB_TYPE_5_TFCS_RECONF_ADD_CTFCSIZE_CTFC2BIT_ELEMENT_POWER_OFF_INFO_PRESENCE_FLAG  0x01
typedef struct
{
    U16                             presence_bitmask;
    U8                              ctfc2;
    power_offset_information_t      power_offser_information;
}tfcs_reconf_add_ctfcSize_ctfc2Bit_element_t;

typedef struct
{
    U16                                             count;
    tfcs_reconf_add_ctfcSize_ctfc2Bit_element_t     ctfc2Bit_elem[MAX_TFC];
}tfcs_reconf_add_ctfcSize_ctfc2Bit_t;

#define UMTS_SIB_TYPE_5_TFCS_RECONF_ADD_CTFCSIZE_CTFC4BIT_ELEMENT_POWER_OFF_INFO_PRESENCE_FLAG  0x01
typedef struct
{
    U16                             presence_bitmask;
    U8                              ctfc4;
    power_offset_information_t      power_offser_information;
}tfcs_reconf_add_ctfcSize_ctfc4Bit_element_t;

typedef struct
{
    U16                                             count;
    tfcs_reconf_add_ctfcSize_ctfc4Bit_element_t     ctfc4Bit_elem[MAX_TFC];
}tfcs_reconf_add_ctfcSize_ctfc4Bit_t;

#define UMTS_SIB_TYPE_5_TFCS_RECONF_ADD_CTFCSIZE_CTFC6BIT_ELEMENT_POWER_OFF_INFO_PRESENCE_FLAG  0x01
typedef struct
{
    U16                             presence_bitmask;
    U8                              ctfc6;
    power_offset_information_t      power_offser_information;
}tfcs_reconf_add_ctfcSize_ctfc6Bit_element_t;

typedef struct
{
    U16                                             count;
    tfcs_reconf_add_ctfcSize_ctfc6Bit_element_t     ctfc6Bit_elem[MAX_TFC];
}tfcs_reconf_add_ctfcSize_ctfc6Bit_t;

#define UMTS_SIB_TYPE_5_TFCS_RECONF_ADD_CTFCSIZE_CTFC8BIT_ELEMENT_POWER_OFF_INFO_PRESENCE_FLAG  0x01
typedef struct
{
    U16                             presence_bitmask;
    U8                              ctfc8;
    power_offset_information_t      power_offser_information;
}tfcs_reconf_add_ctfcSize_ctfc8Bit_element_t;

typedef struct
{
    U16                                             count;
    tfcs_reconf_add_ctfcSize_ctfc8Bit_element_t     ctfc8Bit_elem[MAX_TFC];
}tfcs_reconf_add_ctfcSize_ctfc8Bit_t;

#define UMTS_SIB_TYPE_5_TFCS_RECONF_ADD_CTFCSIZE_CTFC12BIT_ELEMENT_POWER_OFF_INFO_PRESENCE_FLAG  0x01
typedef struct
{
    U16                             presence_bitmask;
    U8                              ctfc12;
    power_offset_information_t      power_offser_information;
}tfcs_reconf_add_ctfcSize_ctfc12Bit_element_t;

typedef struct
{
    U16                                              count;
    tfcs_reconf_add_ctfcSize_ctfc12Bit_element_t     ctfc12Bit_elem[MAX_TFC];
}tfcs_reconf_add_ctfcSize_ctfc12Bit_t;

#define UMTS_SIB_TYPE_5_TFCS_RECONF_ADD_CTFCSIZE_CTFC16BIT_ELEMENT_POWER_OFF_INFO_PRESENCE_FLAG  0x01
typedef struct
{
    U16                             presence_bitmask;
    U8                              ctfc16;
    power_offset_information_t      power_offser_information;
}tfcs_reconf_add_ctfcSize_ctfc16Bit_element_t;

typedef struct
{
    U16                                              count;
    tfcs_reconf_add_ctfcSize_ctfc16Bit_element_t     ctfc16Bit_elem[MAX_TFC];
}tfcs_reconf_add_ctfcSize_ctfc16Bit_t;

#define UMTS_SIB_TYPE_5_TFCS_RECONF_ADD_CTFCSIZE_CTFC24BIT_ELEMENT_POWER_OFF_INFO_PRESENCE_FLAG  0x01
typedef struct
{
    U16                             presence_bitmask;
    U8                              ctfc24;
    power_offset_information_t      power_offser_information;
}tfcs_reconf_add_ctfcSize_ctfc24Bit_element_t;

typedef struct
{
    U16                                                 count;
    tfcs_reconf_add_ctfcSize_ctfc24Bit_element_t        ctfc24Bit_elem[MAX_TFC];
}tfcs_reconf_add_ctfcSize_ctfc24Bit_t;

#define RRC_UMTS_TFCS_RECONFADD_CTFCSIZE_CTFC2BIT 1
#define RRC_UMTS_TFCS_RECONFADD_CTFCSIZE_CTFC4BIT 2
#define RRC_UMTS_TFCS_RECONFADD_CTFCSIZE_CTFC6BIT 3
#define RRC_UMTS_TFCS_RECONFADD_CTFCSIZE_CTFC8BIT 4
#define RRC_UMTS_TFCS_RECONFADD_CTFCSIZE_CTFC12BIT 5
#define RRC_UMTS_TFCS_RECONFADD_CTFCSIZE_CTFC16BIT 6
#define RRC_UMTS_TFCS_RECONFADD_CTFCSIZE_CTFC24BIT 7
typedef struct
{
    U32     t;
    union
    {
        tfcs_reconf_add_ctfcSize_ctfc2Bit_t     ctfc2Bit;
        tfcs_reconf_add_ctfcSize_ctfc4Bit_t     ctfc4Bit;
        tfcs_reconf_add_ctfcSize_ctfc6Bit_t     ctfc6Bit;
        tfcs_reconf_add_ctfcSize_ctfc8Bit_t     ctfc8Bit;
        tfcs_reconf_add_ctfcSize_ctfc12Bit_t    ctfc12Bit;
        tfcs_reconf_add_ctfcSize_ctfc16Bit_t    ctfc16Bit;
        tfcs_reconf_add_ctfcSize_ctfc24Bit_t    ctfc248Bit;
    }u;
}tfcs_reconf_add_ctfcSize_t;

typedef struct
{
    tfcs_reconf_add_ctfcSize_t        ctfc_Size;
}tfcs_reconf_add_t;

typedef struct
{
    U16     tfci;
}tfcs_removal_t;

typedef struct
{
    U16                     count;
    tfcs_removal_t          tfcs_removal[MAX_TFC];
}tfcs_removal_list_t;

typedef struct
{   
    tfcs_removal_list_t     tfcs_Removal;
    tfcs_reconf_add_t       tfcs_Add; 
}explicit_tfcs_configuration_replacement_t;

#define RRC_UMTS_EXPLICITTFCS_CONFIGURATION_COMPLETE 1
#define RRC_UMTS_EXPLICITTFCS_CONFIGURATION_ADDITION 2
#define RRC_UMTS_EXPLICITTFCS_CONFIGURATION_REMOVAL 3
#define RRC_UMTS_EXPLICITTFCS_CONFIGURATION_REPLACEMENT 4
typedef struct
{
    U32             t;
    union
    {
        tfcs_reconf_add_t                          complete;
        tfcs_reconf_add_t                          addition;
        tfcs_removal_list_t                        removal;
        explicit_tfcs_configuration_replacement_t  replacement;
    }u;
}explicit_tfcs_configuration_t;

#define RRC_UMTS_TFCS_INFOFORDSCH_CTFC2BIT 1
#define RRC_UMTS_TFCS_INFOFORDSCH_CTFC4BIT 2
#define RRC_UMTS_TFCS_INFOFORDSCH_CTFC6BIT 3
#define RRC_UMTS_TFCS_INFOFORDSCH_CTFC8BIT 4
#define RRC_UMTS_TFCS_INFOFORDSCH_CTFC12BIT 5
#define RRC_UMTS_TFCS_INFOFORDSCH_CTFC16BIT 6
#define RRC_UMTS_TFCS_INFOFORDSCH_CTFC24BIT 7
typedef struct
{
    U32                 t;
    union
    {
        U8          ctfc2bit;
        U8          ctfc4bit;
        U8          ctfc6bit;
        U8          ctfc8bit;
        U16         ctfc12bit;
        U16         ctfc16bit;
        U32         ctfc24bit;
    }u;
}tfcs_info_for_dsch_t;

typedef struct
{
    U16                     max_TFCIField2_Value;
    tfcs_info_for_dsch_t    tfcs_InfoFor_DSCH;
}tfci_range_t;

typedef struct
{
    U8                      count;
    tfci_range_t            tfci_range[MAX_PDSCH_TFCI_GROUPS];
}tfci_rangelist_t;

#define RRC_UMTS_TFCI_FIELD2_INFORMATION_TFCI_RANGE 1
#define RRC_UMTS_TFCI_FIELD2_INFORMATION_EXPLICIT_CONFIG 2
typedef struct
{
    U32                             t;  
    union
    {
        tfci_rangelist_t               tfci_Range;
        explicit_tfcs_configuration_t  explicit_config;
    }u;
}tfci_field2_information_t;

#define UMTS_SIB_TYPE_5_USCH_TFCS_SPLIT_TFCI_SIGNALLING_SPLIT_TYPE_PRESENCE_FLAG        0x01
#define UMTS_SIB_TYPE_5_USCH_TFCS_SPLIT_TFCI_SIGNALLING_FIELD2_LENGTH_PRESENCE_FLAG     0x02
#define UMTS_SIB_TYPE_5_USCH_TFCS_SPLIT_TFCI_SIGNALLING_FIELD1_INFO_PRESENCE_FLAG       0x04
#define UMTS_SIB_TYPE_5_USCH_TFCS_SPLIT_TFCI_SIGNALLING_FIELD2_INFO_PRESENCE_FLAG       0x08
typedef struct
{
    U16                                 presence_bitmask;
    U32                                 split_type;
    U8                                  tfci_Field2_Length;
    explicit_tfcs_configuration_t       tfci_Field1_Information;
    tfci_field2_information_t           tfci_Field2_Information;
}split_tfci_signalling_t;

#define RRC_UMTS_TFCS_NORMALTFCI_SIGNALLING 1
#define RRC_UMTS_TFCS_DUMMY               2
typedef struct
{
    U32             t;
    union
    {   
        explicit_tfcs_configuration_t   normal_TFCI_Signalling;
        split_tfci_signalling_t         dummy;
    }u;
}tfcs_t;

#define UMTS_SIB_TYPE_5_CELL_SELECT_MODE_SPECIFIC_INFO_TDD_PUSCH_SYSINFO_USCH_TFS_PRESENCE_FLAG     0x01
#define UMTS_SIB_TYPE_5_CELL_SELECT_MODE_SPECIFIC_INFO_TDD_PUSCH_SYSINFO_USCH_TFCS_PRESENCE_FLAG    0x02
typedef struct
{
    U16                           presence_bitmask;
    U8                            pusch_identity;
    pusch_info_t                  pusch_info; 
    transport_format_set_t        uschTFS;
    tfcs_t                        usch_tfcs;
}pusch_sysinfo_t;

typedef struct
{
    U16                 activation_time_sfn;
    U16                 phys_ch_duration;
}sfn_time_info_t;

#define UMTS_SIB_TYPE_5_CELL_SELECT_MODE_SPECIFIC_INFO_TDD_PUSCH_SYSINFO_SFN_TIMEINFO_PRESENCE_FLAG   0x01
typedef struct
{
    U16             presence_bitmask;
    pusch_sysinfo_t pusch_sysinfo;
    sfn_time_info_t  sfn_timeinfo;
}pusch_SysInfoList_SFN_element_t;

typedef struct
{
    U8                                  count;
    pusch_SysInfoList_SFN_element_t     pusch_SysInfo_list_SFN_elememt[8];           
}pusch_sysinfo_list_sfn_t;

typedef struct
{
    U32             first_channelisation_code;
    U32             last_channelisation_code;
}dl_ts_channelisation_codes_short_codes_representation_consecutive_t;

typedef struct
{
    U32             numbits;
    U8              data[2];
}dl_ts_channelisation_codes_short_codes_representation_bitmap_t;

#define RRC_UMTS_DL_TS_CHANNELISATIONCODESSHORT_CODESREPRESENTATION_CONSECUTIVE 1
#define RRC_UMTS_DL_TS_CHANNELISATIONCODESSHORT_CODESREPRESENTATION_BITMAP 2
typedef struct
{
    U32                                                            t;   
    union
    {
        dl_ts_channelisation_codes_short_codes_representation_consecutive_t     consecutive;
        dl_ts_channelisation_codes_short_codes_representation_bitmap_t          bitmap;
    }u;
}dl_ts_channelisation_codes_short_codes_representation_t;

typedef struct
{
    dl_ts_channelisation_codes_short_codes_representation_t     codes_representation;
}dl_ts_channelisation_codes_short_t;

typedef struct
{
    U8          timeslot_number;
}downlink_additional_timeslots_parameters_same_as_last_t;

typedef struct
{
    individual_timeslot_info_t          individual_timeslot_info;
    dl_ts_channelisation_codes_short_t  dl_ts_channelisation_codes_short; 
}downlink_additional_timeslots_parameters_new_parameters_t;

#define RRC_UMTS_DOWNLINKADDITIONALTIMESLOTS_PARAMETERS_SAMEASLAST 1
#define RRC_UMTS_DOWNLINKADDITIONALTIMESLOTS_PARAMETERS_NEWPARAMETERS 2
typedef struct
{
    U32                             t;
    union
    {
        downlink_additional_timeslots_parameters_same_as_last_t     same_as_last;
        downlink_additional_timeslots_parameters_new_parameters_t   new_parameters;
    }u;
}downlink_additional_timeslots_parameters_t;

typedef struct
{   
    downlink_additional_timeslots_parameters_t      parameters;
}downlink_additional_timeslots_t;

typedef struct
{
    U8                                  count;
    downlink_additional_timeslots_t     downlink_additional_timeslots[MAX_NO_OF_TIMESLOTS_MINUS_1];
}downlink_timeslots_codes_more_timeslots_additional_timeslots_timeslot_list_t;

#define RRC_UMTS_DOWNLINKTIMESLOTSCODES_MORETIMESLOTS_ADDITIONALTIMESLOTS_CONSECUTIVE 1
#define RRC_UMTS_DOWNLINKTIMESLOTSCODES_MORETIMESLOTS_ADDITIONALTIMESLOTS_TIMESLOTLIST 2
typedef struct
{
    U32                 t;
    union   
    {
        U8                                                                              consecutive;
        downlink_timeslots_codes_more_timeslots_additional_timeslots_timeslot_list_t    timeslotList;
    }u;
}downlink_timeslots_codes_more_timeslots_additional_timeslots_t;

#define RRC_UMTS_DOWNLINKTIMESLOTSCODES_MORETIMESLOTS_NOMORE 1
#define RRC_UMTS_DOWNLINKTIMESLOTSCODES_MORETIMESLOTS_ADDITIONALTIMESLOTS 2
typedef struct
{
    U32             t; 
    union
    {
        /* t = 1 */
        /* t = 2 */
        downlink_timeslots_codes_more_timeslots_additional_timeslots_t  additional_timeslots;
    }u;
}downlink_timeslots_codes_more_timeslots_t;

typedef struct
{
    individual_timeslot_info_t                  first_individual_timeslot_info;
    dl_ts_channelisation_codes_short_t          dl_ts_channelisation_codes_short; 
    downlink_timeslots_codes_more_timeslots_t   more_timeslots;
}downlink_timeslots_codes_t;

#define UMTS_SIB_TYPE_5_PDSCH_SYSINFOLIST_COMMON_TIMESLOT_INFO_PRESENCE_FLAG    0x01
#define UMTS_SIB_TYPE_5_PDSCH_SYSINFOLIST_PDSCH_TIMESLOTS_CODES_PRESENCE_FLAG   0x02
typedef struct
{
    U16                             presence_bitmask;
    U8                              tfcs_id;
    common_timeslot_info_t          common_timeslot_info;
    downlink_timeslots_codes_t      pdsch_timeslots_codes;
}pdsch_info_t;

#define UMTS_SIB_TYPE_5_PDSCH_SYSINFOLIST_SFN_DSCH_TFS_PRESENCE_FLAG    0x01
#define UMTS_SIB_TYPE_5_PDSCH_SYSINFOLIST_SFN_DSCH_TFCS_PRESENCE_FLAG   0x02
typedef struct
{
    U16                             presence_bitmask;
    U8                              pdsch_identity;
    pdsch_info_t                    pdsch_info;
    transport_format_set_t          dsch_TFS;
    tfcs_t                          dsch_TFCS;
}pdsch_SysInfo_t;

#define UMTS_SIB_TYPE_5_PDSCH_SYSINFOLIST_SFN_TIMEINFO_PRESENCE_FLAG    0x01
typedef struct
{
    U16                             presence_bitmask;
    pdsch_SysInfo_t                 pdsch_SysInfo;
    sfn_time_info_t                  sfn_timeInfo;
}pdsch_SysInfoList_SFN_element_t;

typedef struct
{
    U8                                  count;
    pdsch_SysInfoList_SFN_element_t     pdsch_SysInfo_list_SFN_elememt[8];
}pdsch_sysinfo_list_sfn_t;

#define UMTS_SIB_TYPE_5_CELL_SELECT_MODE_SPECIFIC_INFO_OPENLOOP_PWRCTRL_TDD_ALPHA_PRESENCE_FLAG             0x01
#define UMTS_SIB_TYPE_5_CELL_SELECT_MODE_SPECIFIC_INFO_OPENLOOP_PWRCTRL_TDD_PUSCH_CONST_VAL_PRESENCE_FLAG   0x02
typedef struct
{
    U16                             presence_bitmask;
    U8                              primary_ccpx_tx_power;
    U8                              alpha;
    S8                              prach_constant_value_tdd;
    S8                              dpch_constant_value_tdd;
    S8                              pusch_constant_value_tdd;
}openLoop_powerControl_tdd_t;

#define UMTS_SIB_TYPE_5_CELL_SELECT_MODE_SPECIFIC_INFO_TDD_PUSCH_SYSINFO_LIST_PRESENCE_FLAG 0x01
#define UMTS_SIB_TYPE_5_CELL_SELECT_MODE_SPECIFIC_INFO_TDD_PDSCH_SYSINFO_LIST_PRESENCE_FLAG 0x02
typedef struct
{
    U16                             presence_bitmask;
    pusch_sysinfo_list_sfn_t        pusch_SysInfo_list_SFN;
    pdsch_sysinfo_list_sfn_t        pdsch_SysInfo_list_SFN;
    openLoop_powerControl_tdd_t     openLoop_PowerControl_TDD;
}sysinfo_type5_modespecific_info_tdd_t;

#define RRC_UMTS_SYSINFOTYPE5_MODESPECIFICiNFO_FDD 1
#define RRC_UMTS_SYSINFOTYPE5_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32             t;  
    union
    {
        sysinfo_type5_modespecific_info_fdd_t       fdd;
        sysinfo_type5_modespecific_info_tdd_t       tdd;
    }u;
}sysinfo_type5_modespecific_info_t;

typedef struct
{
    U8      tx_diversity_indicator;
}primary_ccpch_info_fdd_t;

typedef struct
{
    U8      time_slot_Number;
}primary_ccpch_info_tdd_synch_case_sync_case1_t;

typedef struct
{
    U8      timeslot_Sync2;
}primary_ccpch_info_tdd_synch_case_sync_case2_t;

#define RRC_UMTS_PRIMARYCCPCH_INFO_TDD_SYNCCASE_SYNCCASE1 1
#define RRC_UMTS_PRIMARYCCPCH_INFO_TDD_SYNCCASE_SYNCCASE2 2
typedef struct
{
    U32                 t;  
    union
    {
        primary_ccpch_info_tdd_synch_case_sync_case1_t       syncCase1;
        primary_ccpch_info_tdd_synch_case_sync_case2_t       syncCase2;
    }u;
}primary_ccpch_info_tdd_synch_case_t;

#define UMTS_SIB_TYPE_5_INFO_PRIMARY_CCPCH_INFO_TDD_SYNCH_CASE_PRESENCE_FLAG    0x01
#define UMTS_SIB_TYPE_5_INFO_PRIMARY_CCPCH_INFO_TDD_CELL_PARAMS_PRESENCE_FLAG   0x02
typedef struct
{
    U16                                         presence_bitmask;
    primary_ccpch_info_tdd_synch_case_t         synch_case;
    U8                                          cell_parametersID;
    U8                                          sctd_indicator;
}primary_ccpch_info_tdd_t;

#define RRC_UMTS_PRIMARYCCPCH_INFO_FDD    1
#define RRC_UMTS_PRIMARYCCPCH_INFO_TDD    2
typedef struct
{
    U32         t;
    union
    {
        primary_ccpch_info_fdd_t                       fdd;
        primary_ccpch_info_tdd_t                       tdd;
    }u;
}primary_ccpch_info_t;

typedef struct
{
    U32         numbits;
    U8          data[2];
}available_signatures_t;

typedef struct
{
    U32         numbits;
    U8          data[2];
}available_subchannel_numbers_t;

typedef struct
{
    available_signatures_t          available_Signatures;
    U32                             available_SF;
    U8                              preambleScramblingCodeWordNumber;
    U32                             puncturing_Limit;
    available_subchannel_numbers_t  available_subchannel_numbers;
}prach_rach_info_modespecific_info_fdd_t;

typedef struct
{
    U32             n;  
    U32             elem[8];
}tdd_prach_ccode_list_sf8_t;

typedef struct
{
    U32             n;
    U32             elem[8];
}tdd_prach_ccode_list_sf16_t;

#define RRC_UMTS_TDD_PRACH_CCODELIST_SF8  1
#define RRC_UMTS_TDD_PRACH_CCODELIST_SF16 2
typedef struct
{
    U32                         t;  
    union
    {
        tdd_prach_ccode_list_sf8_t    sf8;
        tdd_prach_ccode_list_sf16_t   sf16;
    }u;
}tdd_prach_ccode_list_t;

typedef struct
{
    U8                          timeslot_number;
    tdd_prach_ccode_list_t      channelisation_CodeList;
    U32                         prach_Midamble;
}prach_rach_info_modespecific_info_tdd_t;

#define RRC_UMTS_PRACH_RACH_INFO_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_PRACH_RACH_INFO_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32                     t;  
    union
    {
        prach_rach_info_modespecific_info_fdd_t    fdd;
        prach_rach_info_modespecific_info_tdd_t    tdd;
    }u;
}prach_rach_info_modespecific_info_t;

typedef struct
{
    prach_rach_info_modespecific_info_t     modeSpecific_info;
}prach_rach_info_t;

typedef struct
{
    U32         numbits;
    U8          data[1];
}access_service_class_fdd_assigned_sub_channel_number_t;

typedef struct
{
    U8          available_signature_start_index;
    U8          available_signature_end_index;
    access_service_class_fdd_assigned_sub_channel_number_t  assigned_sub_channel_number;
}access_service_class_fdd_t;

#define UMTS_SIB_TYPE_5_INFO_PRACH_SYS_INFO_PRACH_PARTITIONING_ASCS_SETTING_FDD_PRESENCE_FLAG   0x01
typedef struct
{
    U16                         presence_bitmask;
    access_service_class_fdd_t  access_service_class_FDD; 
}ascs_setting_fdd_t;

typedef struct
{
    U32                 count;
    ascs_setting_fdd_t  ascs_settingFDD[8];
}prach_partitioning_fdd_t;

typedef struct
{
    U32                 numbits;
    U8                  data[1];
}access_service_class_tdd_channelisation_code_indices_t;

#define UMTS_SIB_TYPE_5_INFO_PRACH_SYS_INFO_PRACH_PARTITIONING_ACCESS_SERVICE_CLASS_TDD_SUB_CHANN_PRESENCE_FLAG   0x01
typedef struct
{
    U16                         presence_bitmask;
    U32                         sub_channels;
}access_service_class_tdd_subchannel_size_size2_t;

typedef struct
{
    U32             numbits;
    U8              data[1];
}access_service_class_tdd_subchannel_size_size4_subchannels_t;

#define UMTS_SIB_TYPE_5_INFO_PRACH_SYS_INFO_PRACH_PARTITIONING_ACCESS_SERVICE_CLASS_TDD_SUB_CHANN_SIZE4_PRESENCE_FLAG   0x01
typedef struct
{
    U16                         presence_bitmask;
    access_service_class_tdd_subchannel_size_size4_subchannels_t     sub_channels;
}access_service_class_tdd_subchannel_size_size4_t;

typedef struct
{
    U32             numbits;
    U8              data[1];
}access_service_class_tdd_subchannel_size_size8_subchannels_t;

#define UMTS_SIB_TYPE_5_INFO_PRACH_SYS_INFO_PRACH_PARTITIONING_ACCESS_SERVICE_CLASS_TDD_SUB_CHANN_SIZE8_PRESENCE_FLAG   0x01
typedef struct
{
    U16                         presence_bitmask;
    access_service_class_tdd_subchannel_size_size8_subchannels_t     sub_channels;
}access_service_class_tdd_subchannel_size_size8_t;

#define RRC_UMTS_ACCESSSERVICECLASS_TDD_SUBCHANNELSIZE_SIZE1 1
#define RRC_UMTS_ACCESSSERVICECLASS_TDD_SUBCHANNELSIZE_SIZE2 2
#define RRC_UMTS_ACCESSSERVICECLASS_TDD_SUBCHANNELSIZE_SIZE4 3
#define RRC_UMTS_ACCESSSERVICECLASS_TDD_SUBCHANNELsIZE_SIZE8 4
typedef struct
{
    U32             t;
    union
    {
        access_service_class_tdd_subchannel_size_size2_t   size2;
        access_service_class_tdd_subchannel_size_size4_t   size4;
        access_service_class_tdd_subchannel_size_size8_t   size8;
    }u;
}access_service_class_tdd_subchannel_size_t;

#define UMTS_SIB_TYPE_5_INFO_PRACH_SYS_INFO_PRACH_PARTITIONING_ACCESS_SERVICE_CLASS_TDD_CHANNEL_CODE_INDICES_PRESENCE_FLAG   0x01
typedef struct
{
    U16                         presence_bitmask;
    access_service_class_tdd_channelisation_code_indices_t   channelisation_CodeIndices; 
    access_service_class_tdd_subchannel_size_t               subchannel_Size;
}access_service_class_tdd_t;

#define UMTS_SIB_TYPE_5_INFO_PRACH_SYS_INFO_PRACH_PARTITIONING_ASCS_SETTING_TDD_PRESENCE_FLAG   0x01
typedef struct
{
    U16                         presence_bitmask;
    access_service_class_tdd_t  access_service_class_TDD;
}ascs_setting_tdd_t;

typedef struct
{
    U32                 count;
    ascs_setting_tdd_t  ascs_settingTDD[8];
}prach_partitioning_tdd_t;

#define RRC_UMTS_PRACH_PARTITIONING_FDD   1
#define RRC_UMTS_PRACH_PARTITIONING_TDD   2
typedef struct
{
    U32         t;  
    union
    {
        prach_partitioning_fdd_t   fdd;
        prach_partitioning_tdd_t   tdd;
    }u;
}prach_partitioning_t;


typedef struct
{
    U32             n;
    U32             elem[6];
}persistence_scaling_factorlist_t;

typedef struct
{
    U32             n;
    U32             elem[7];
}ac_to_asc_mapping_table_t;

typedef struct
{
    U8          powerRampStep;
    U8          preambleRetransMax;
}prach_poweroffset_t;

typedef struct
{
    U8          mmax;
    U8          nb01_Min;
    U8          nb01_Max;
}rach_transmission_parameters_t;

typedef struct
{
    U8          channelisation_Code256;
    U8          sttd_indicator;
    U32         aich_Transmission_Timing;
}aich_info_t;

#define UMTS_SIB_TYPE_5_INFO_PRACH_SYS_INFO_PRACH_SYSINFO_MODE_SPECIFIC_INFO_FDD_PRIM_CPICH_TX_PWR_PRESENCE_FLAG    0x01
#define UMTS_SIB_TYPE_5_INFO_PRACH_SYS_INFO_PRACH_SYSINFO_MODE_SPECIFIC_INFO_FDD_CONST_VAL_PRESENCE_FLAG            0x02
#define UMTS_SIB_TYPE_5_INFO_PRACH_SYS_INFO_PRACH_SYSINFO_MODE_SPECIFIC_INFO_FDD_PRACH_PWROFFSET_PRESENCE_FLAG      0x04
#define UMTS_SIB_TYPE_5_INFO_PRACH_SYS_INFO_PRACH_SYSINFO_MODE_SPECIFIC_INFO_FDD_RACH_TRANS_PARAM_PRESENCE_FLAG     0x08
#define UMTS_SIB_TYPE_5_INFO_PRACH_SYS_INFO_PRACH_SYSINFO_MODE_SPECIFIC_INFO_FDD_AICH_INFO_PRESENCE_FLAG            0x10
typedef struct
{
    U16                     presence_bitmask;
    S8                      primaryCPICH_TX_Power; 
    S8                      constant_value;
    prach_poweroffset_t     prach_powerOffset;
    rach_transmission_parameters_t  rach_transmissionParameters;
    aich_info_t                     aich_info; 
}prach_system_information_modespecific_info_fdd_t;

#define RRC_UMTS_PRACH_SYSTEMINFORMATION_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_PRACH_SYSTEMINFORMATION_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32                 t;
    union
    {
        prach_system_information_modespecific_info_fdd_t   fdd;
    }u;
}prach_system_information_modespecific_info_t;

#define UMTS_SIB_TYPE_5_INFO_PRACH_SYS_INFO_RACH_TRANSPORT_FORMAT_SET_PRESENCE_FLAG         0x01
#define UMTS_SIB_TYPE_5_INFO_PRACH_SYS_INFO_RACH_TFCS_PRESENCE_FLAG                         0x02
#define UMTS_SIB_TYPE_5_INFO_PRACH_SYS_INFO_PRACH_PARTITIONING_PRESENCE_FLAG                0x04
#define UMTS_SIB_TYPE_5_INFO_PRACH_SYS_INFO_PERSISTENCE_SCALING_FACTOR_LIST_PRESENCE_FLAG   0x08
#define UMTS_SIB_TYPE_5_INFO_PRACH_SYS_INFO_AC_TO_ASC_MAPPING_TABLE_PRESENCE_FLAG           0x10
typedef struct
{
    U16                     presence_bitmask;
    prach_rach_info_t       prach_RACH_info;
    U8                      transportChannel_Identity;
    transport_format_set_t  rach_TransportFormatSet;
    tfcs_t                  rach_TFCS;
    prach_partitioning_t    prach_Partitioning; 
    persistence_scaling_factorlist_t                persistence_scaling_factor_list;
    ac_to_asc_mapping_table_t                       ac_to_ASC_MappingTable;
    prach_system_information_modespecific_info_t    modeSpecific_Info;
}prach_system_information_t;

typedef struct
{
    U8                              count;
    prach_system_information_t      prach_sys_info[16];
}prach_system_information_list_t;

#define UMTS_SIB_TYPE_5_INFO_SECONDARY_CCPCH_INFO_MODESPECIFIC_INFO_FDD_SECOND_CPICH_INFO_SCRAMBLING_CODE_PRESENCE_FLAG 0x01
typedef struct
{
    U16                 presence_bitmask;
    U8                  secondaryDL_ScramblingCode;
    U8                  channelisation_Code;
}secondary_cpich_info_t;

#define RRC_UMTS_SF256_ANDCODENUMBER_SF4  1
#define RRC_UMTS_SF256_ANDCODENUMBER_SF8  2
#define RRC_UMTS_SF256_ANDCODENUMBER_SF16 3
#define RRC_UMTS_SF256_ANDCODENUMBER_SF32 4
#define RRC_UMTS_SF256_ANDCODENUMBER_SF64 5
#define RRC_UMTS_SF256_ANDCODENUMBER_SF128 6
#define RRC_UMTS_SF256_ANDCODENUMBER_SF256 7
typedef struct
{
    U32          t;
    union 
    {
        /* t = 1 */
        U8 sf4;
        /* t = 2 */
        U8 sf8;
        /* t = 3 */
        U8 sf16;
        /* t = 4 */
        U8 sf32;
        /* t = 5 */
        U8 sf64;
        /* t = 6 */
        U8 sf128;
        /* t = 7 */
        U8 sf256;
    }u;
}sf256_and_code_number_t;

#define UMTS_SIB_TYPE_5_INFO_SECONDARY_CCPCH_INFO_MODESPECIFIC_INFO_FDD_DUMMY2_PRESENCE_FLAG                0x01
#define UMTS_SIB_TYPE_5_INFO_SECONDARY_CCPCH_INFO_MODESPECIFIC_INFO_FDD_SEC_SCRAMBLING_CODE_PRESENCE_FLAG   0x02
typedef struct
{
    U16                                         presence_bitmask;
    U32                                         dummy1_PCPICH_usageForForChannelEst;
    secondary_cpich_info_t                      dummy2;
    U8                                          secondary_ScramblingCode;
    U8                                          sttd_indicator;
    sf256_and_code_number_t                     sf_andCodeNumber;
    U8                                          pilot_SymbolExistence;
    U8                                          tfci_Existence;
    U32                                         positionFixedOrFlexible;
    U32                                         timingOffset;
}secondary_ccpch_info_modespecific_info_fdd_t;

typedef struct
{
    U8      offset;
}repetition_period_length_and_offset_repetition_period2;

typedef struct
{
    U8          length;
    U8          offset;
}repetition_period_length_and_offset_repetition_period4;

typedef struct
{
    U8          length;
    U8          offset;
}repetition_period_length_and_offset_repetition_period8;

typedef struct
{
    U8          length;
    U8          offset;
}repetition_period_length_and_offset_repetition_period16;

typedef struct
{
    U8          length;
    U8          offset;
}repetition_period_length_and_offset_repetition_period32;

typedef struct
{
    U8          length;
    U8          offset;
}repetition_period_length_and_offset_repetition_period64;

#define RRC_UMTS_REPETITIONPERIODLENGTHANDOFFSET_REPETITIONPERIOD1 1
#define RRC_UMTS_REPETITIONPERIODLENGTHANDOFFSET_REPETITIONPERIOD2 2
#define RRC_UMTS_REPETITIONPERIODLENGTHANDOFFSET_REPETITIONPERIOD4 3
#define RRC_UMTS_REPETITIONPERIODLENGTHANDOFFSET_REPETITIONPERIOD8 4
#define RRC_UMTS_REPETITIONPERIODLENGTHANDOFFSET_REPETITIONPERIOD16 5
#define RRC_UMTS_REPETITIONPERIODLENGTHANDOFFSET_REPETITIONPERIOd32 6
#define RRC_UMTS_REPETITIONPERIODLENGTHANDOFFSET_REPETITIONPERIOD64 7
typedef struct
{
    U32                 t;
    union
    {
        repetition_period_length_and_offset_repetition_period2     repetitionPeriod2;
        repetition_period_length_and_offset_repetition_period4     repetitionPeriod4;
        repetition_period_length_and_offset_repetition_period8     repetitionPeriod8;
        repetition_period_length_and_offset_repetition_period16    repetitionPeriod16;
        repetition_period_length_and_offset_repetition_period32    repetitionPeriod32;
        repetition_period_length_and_offset_repetition_period64    repetitionPeriod64;
    }u;
}repetition_period_length_and_offset_t;

#define UMTS_SIB_TYPE_5_INFO_SCCPCH_MODESPECIFIC_TDD_COMMON_TIMESLOT_TFCI_CODING_PRESENCE_FLAG                  0x01
#define UMTS_SIB_TYPE_5_INFO_SCCPCH_MODESPECIFIC_TDD_COMMON_TIMESLOT_REPETITION_PRD_LEN_OFFSET_PRESENCE_FLAG    0x02
typedef struct
{
    U16             presence_bitmask;
    U32             second_InterleavingMode;
    U32             tfci_Coding;
    U32             puncturing_Limit;
    repetition_period_length_and_offset_t      repetitionPeriodLengthAndOffset;
}common_timeslot_info_sccpch_t;

typedef struct
{
    U32         n;
    U32         elem[16];
}sccpch_channelisation_code_list_t;

typedef struct
{
    common_timeslot_info_sccpch_t       commonTimeSlot_Info;
    individual_timeslot_info_t          individualTimeSlot_Info;
    sccpch_channelisation_code_list_t   channelisation_Code;
}secondary_ccpch_info_modespecific_info_tdd_t;

#define RRC_UMTS_SECONDARYCCPCH_INFO_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_SECONDARYCCPCH_INFO_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32             t;  
    union
    {
        secondary_ccpch_info_modespecific_info_fdd_t   fdd;
        secondary_ccpch_info_modespecific_info_tdd_t   tdd;
    }u;
}secondary_ccpch_info_modespecific_info_t;

typedef struct
{
    secondary_ccpch_info_modespecific_info_t    modeSpecific_Info;
}secondary_ccpch_info_t;

typedef struct
{
    transport_format_set_t      transport_format_set;
    U8                          transport_channel_identity;
    U8                          ctch_indicator;
}fach_pch_information_t;

typedef struct
{
    U32                         count;
    fach_pch_information_t      fach_pch_info[MAX_FACH_PCH];
}fach_pch_information_list_t;

typedef struct
{
    U8              channelisation_Code256;
    U32             pi_CountPerFrame;
    U8              sttd_Indicator;
}pich_info_fdd;

#define RRC_UMTS_REPPERLENGTHOFFSET_PICH_RPP4_2 1
#define RRC_UMTS_REPPERLENGTHOFFSET_PICH_RPP8_2 2
#define RRC_UMTS_REPPERLENGTHOFFSET_PICH_RPP8_4 3
#define RRC_UMTS_REPPERLENGTHOFFSET_PICH_RPP16_2 4
#define RRC_UMTS_REPPERLENGTHOFFSET_PICH_RPP16_4 5
#define RRC_UMTS_REPPERLENGTHOFFSET_PICH_RPP32_2 6
#define RRC_UMTS_REPPERLENGTHOFFSET_PICH_RPP32_4 7
#define RRC_UMTS_REPPERLENGTHOFFSET_PICH_RPP64_2 8
#define RRC_UMTS_REPPERLENGTHOFFSET_PICH_RPP64_4 9
typedef struct
{
    U32      t;
    union 
    {
        U8 rpp4_2;
        U8 rpp8_2;
        U8 rpp8_4;
        U8 rpp16_2;
        U8 rpp16_4;
        U8 rpp32_2;
        U8 rpp32_4;
        U8 rpp64_2;
        U8 rpp64_4;
    }u;
}rep_per_length_offset_pich_t;

#define UMTS_SIB_TYPE_5_INFO_SCCPCH_SYSINFO_PICH_TDD_INFO_CHANNELISATION_CODE_PRESENCE_FLAG     0x01
#define UMTS_SIB_TYPE_5_INFO_SCCPCH_SYSINFO_PICH_TDD_INFO_TIMESLOT_NUMBER_PRESENCE_FLAG         0x02
#define UMTS_SIB_TYPE_5_INFO_SCCPCH_SYSINFO_PICH_TDD_INFO_REPETITION_PRD_LEN_OFF_PRESENCE_FLAG  0x04
typedef struct
{
    U16             presence_bitmask;
    U32             channelisation_code;
    U8              timeslot_number;
    midamble_shift_and_burst_type_t     midamble_shift_and_burst_type;
    rep_per_length_offset_pich_t        rep_per_length_offset_pich;
    U32                             paging_indicator_length;
    U32                             n_gap;
    U8                              n_pch;
}pich_info_tdd;

#define RRC_UMTS_PICH_INFO_FDD            1
#define RRC_UMTS_PICH_INFO_TDD            2
typedef struct
{
    U32                 t;
    union
    {
        pich_info_fdd  fdd;
        pich_info_tdd  tdd;
    }u;
}pich_info_t;

#define UMTS_SIB_TYPE_5_INFO_SCCPCH_SYSINFO_TFCS_PRESENCE_FLAG                  0x01
#define UMTS_SIB_TYPE_5_INFO_SCCPCH_SYSINFO_FACH_PCH_INFO_LIST_PRESENCE_FLAG    0x02
#define UMTS_SIB_TYPE_5_INFO_SCCPCH_SYSINFO_PICH_INFO_PRESENCE_FLAG             0x04
typedef struct
{   
    U16                                         presence_bitmask;
    secondary_ccpch_info_t                      secondary_CCPCH_Info;
    tfcs_t                                      tfcs;
    fach_pch_information_list_t                 fach_PCH_Information_List;
    pich_info_t                                 pich_info;
}sccpch_systemInformation_t;

typedef struct
{
    U8                          count;
    sccpch_systemInformation_t  sccpcf_system_info[16];
}sccpch_system_information_list_t;

typedef struct
{
    U16                          ctch_allocation_period;
    U8                           cbs_frame_offset;
}cbs_drx_level1_information_t;

typedef struct
{
    U32      number_of_repetitions_per_sfn_period;
}pnbsch_allocation_r4_t;

typedef struct 
{
    U8       ipdl_alpha;
    U8       max_power_increase;
}open_loop_power_control_ipdl_tdd_r4_t;

typedef struct
{
    U8          midamble_shift;
}midamble_shift_and_burst_type_lcr_r4_midamble_allocation_mode_ue_specific_midamble_t;

#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_LCR_R4_MIDAMBLEALLOCATIONMODE_DEFAULTMIDAMBLE 1
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_LCR_R4_MIDAMBLEALLOCATIONMODE_COMMONMIDAMBLE 2
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_LCR_R4_MIDAMBLEALLOCATIONMODE_UESPECIFICMIDAMBLE 3
typedef struct
{
    U32                 t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        /* t = 3 */
        midamble_shift_and_burst_type_lcr_r4_midamble_allocation_mode_ue_specific_midamble_t    ue_specific_midamble;
    }u;
}midamble_shift_and_burst_type_lcr_r4_midamble_allocation_mode_t;

typedef struct
{
    midamble_shift_and_burst_type_lcr_r4_midamble_allocation_mode_t midamble_allocation_mode;
    U8                                                              midamble_configuration;
}midamble_shift_and_burst_type_lcr_r4_t;

typedef struct
{
    midamble_shift_and_burst_type_lcr_r4_t   midamble_shift_and_burst_type;
    U32                                      modulation;
    U32                                      ss_tpc_symbols;
}individual_timeslot_info_lcr_r4_ext_t;

typedef struct
{
    individual_timeslot_info_lcr_r4_ext_t    individual_timeslot_info_lcr_r4_ext;
}secondary_ccpch_info_lcr_r4_ext_t;

typedef struct
{
    U32          n;
    U32          elem[2];
}pich_channelisation_code_list_lcr_r4_t;

#define UMTS_SIB_PICH_INFO_LCR_R4_TIMESLOT_PRESENCE_FLAG                            0x01
#define UMTS_SIB_PICH_INFO_LCR_R4_REPETITION_PERIOD_LENGTH_OFFSET_PRESENCE_FLAG     0x02
typedef struct
{
    U16      				    			presence_bitmask;
    U8                       				timeslot;
    pich_channelisation_code_list_lcr_r4_t	pich_channelisation_code_list;
    midamble_shift_and_burst_type_lcr_r4_t	midamble_shift_and_burst_type;
    rep_per_length_offset_pich_t			    rep_per_length_offset_pich;
    U32                      				paging_indicator_length;
    U32                      				n_gap;
    U8                       				n_pch;
}pich_info_lcr_r4_t;

#define UMTS_SIB_SCCPCH_SYSTEM_INFORMATION_LCR_R4_EXT_PICH_INFO_PRESENCE_FLAG  0x01
typedef struct
{
    U16                                  presence_bitmask;
    secondary_ccpch_info_lcr_r4_ext_t	secondary_ccpch_info_lcr_r4_ext;
    pich_info_lcr_r4_t				    pich_Info;
}sccpch_system_information_lcr_r4_ext_t;

typedef struct
{
    U32             numbits;
    U8              data[1];
}sync_ul_codes_bitmap_t;

typedef struct
{
    sync_ul_codes_bitmap_t      sync_ul_codes_bitmap;
    U8                          prx_up_pch_des;
    U8                          power_ramp_step;
    U32                         max_sync_ul_transmissions;
    U8                          mmax;
}sync_ul_info_r4_t;

typedef struct
{
    U32         n;
    U32         elem[4];
}prach_chan_codes_lcr_r4_t;

typedef struct
{
    U8                  timeslot;
    U32                 channelisation_code;
    midamble_shift_and_burst_type_lcr_r4_t  midamble_shift_and_burst_type_lcr_r4;
    U8                  wi;
}fpach_info_r4_t;

typedef struct
{
    U8                                      timeslot;
    prach_chan_codes_lcr_r4_t               prach_chan_codes_lcr_r4;
    midamble_shift_and_burst_type_lcr_r4_t  midamble_shift_and_burst_type_lcr_r4;
    fpach_info_r4_t                         fpach_info_r4;
}prach_definition_lcr_r4_t;

typedef struct
{
    U8                          count;
    prach_definition_lcr_r4_t   prach_definition_lcr_r4[MAX_PRACH_FPACH];
}prach_rach_info_lcr_r4_prach_definition_list_t;

typedef struct
{
    sync_ul_info_r4_t                               sync_ul_info_r4;
    prach_rach_info_lcr_r4_prach_definition_list_t  prach_definition_list;
}prach_rach_info_lcr_r4_t;

#define RRC_UMTS_COMMONTRANSCHTFS_LCR_TTI_TTI5 1
#define RRC_UMTS_COMMONTRANSCHTFS_LCR_TTI_TTI10 2
#define RRC_UMTS_COMMONTRANSCHTFS_LCR_TTI_TTI20 3
#define RRC_UMTS_COMMONTRANSCHTFS_LCR_TTI_TTI40 4
#define RRC_UMTS_COMMONTRANSCHTFS_LCR_TTI_TTI80 5
#define RRC_UMTS_COMMONTRANSCHTFS_LCR_TTI_DYNAMIC 6
typedef struct
{
    U32                                             t;
    union
    {
        /* t = 1 */
        common_dynamic_tf_info_list_t                tti5;
        /* t = 2 */
        common_dynamic_tf_info_list_t                tti10;
        /* t = 3 */
        common_dynamic_tf_info_list_t                tti20;
        /* t = 4 */
        common_dynamic_tf_info_list_t                tti40;
        /* t = 5 */
        common_dynamic_tf_info_list_t                tti80;
        /* t = 6 */
        common_dynamic_tf_info_list_dynamic_tti_t    dynamic;
    }u;
}common_trans_ch_tfs_lcr_tti_t;

typedef struct
{
    common_trans_ch_tfs_lcr_tti_t   tti;
    semistatic_tf_information_t     semistatic_tf_information;
}common_trans_ch_tfs_lcr_t;

#define RRC_UMTS_TRANSPORTFORMATSET_LCR_DEDICATEDTRANSCHTFS 1
#define RRC_UMTS_TRANSPORTFORMATSET_LCR_COMMONTRANSCHTFS_LCR 2
typedef struct
{
    U32         t;
    union
    {
        dedicated_trans_ch_tfs_t    dedicated_trans_ch_tfs;
        common_trans_ch_tfs_lcr_t   ccommon_trans_ch_tfs_lcr;
    }u;
}transport_format_set_lcr_t;

#define UMTS_SIB_ACCESS_SERVICE_CLASS_TDD_LCR_R4_SUBCHANNEL_SIZE_SIZE2_SUBCHANNELS_PRESENCE_FLAG    0x01
typedef struct
{
    U16     presence_bitmask;
    U32     subchannels;
}access_service_class_tdd_lcr_r4_subchannel_size_size2_t;

typedef struct
{
    U32             numbits;
    U8              data[1];
}access_service_class_tdd_lcr_r4_subchannel_size_size4_subchannels_t;

#define UMTS_SIB_ACCESS_SERVICE_CLASS_TDD_LCR_R4_SUBCHANNEL_SIZE_SIZE4_SUBCHANNELS_PRESENCE_FLAG    0x01
typedef struct
{
    U16                 presence_bitmask;
    access_service_class_tdd_lcr_r4_subchannel_size_size4_subchannels_t subchannels;
}access_service_class_tdd_lcr_r4_subchannel_size_size4_t;

typedef struct
{
    U32             numbits;
    U8              data[1];
}access_service_class_tdd_lcr_r4_subchannel_size_size8_subchannels_t;

#define UMTS_SIB_ACCESS_SERVICE_CLASS_TDD_LCR_R4_SUBCHANNEL_SIZE_SIZE8_SUBCHANNELS_PRESENCE_FLAG    0x01
typedef struct
{
    U16                 presence_bitmask;
    access_service_class_tdd_lcr_r4_subchannel_size_size8_subchannels_t subchannels;
}access_service_class_tdd_lcr_r4_subchannel_size_size8_t;

#define RRC_UMTS_ACCESSSERVICECLASS_TDD_LCR_R4_SUBCHANNELSIZE_SIZE1 1
#define RRC_UMTS_ACCESSSERVICECLASS_TDD_LCR_R4_SUBCHANNELSIZE_SIZE2 2
#define RRC_UMTS_ACCESSSERVICECLASS_TDD_LCR_R4_SUBCHANNELSIZE_SIZE4 3
#define RRC_UMTS_ACCESSSERVICECLASS_TDD_LCR_R4_SUBCHANNELSIZE_SIZE8 4
typedef struct
{
    U32             t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        access_service_class_tdd_lcr_r4_subchannel_size_size2_t   size2;
        /* t = 3 */
        access_service_class_tdd_lcr_r4_subchannel_size_size4_t   size4;
        /* t = 4 */
        access_service_class_tdd_lcr_r4_subchannel_size_size8_t   size8;
    }u;
}access_service_class_tdd_lcr_r4_sub_channel_size_t;

typedef struct
{
    U32         numbits;
    U8          data[1];
}access_service_class_tdd_lcr_r4_available_sync_ul_codes_indics_t;

#define UMTS_SIB_ACCESS_SERVICE_CLASS_TDD_LCR_R4_AVAILABLE_SYNC_UL_CODES_INDICS_PRESENCE_FLAG   0x01
typedef struct
{
    U16                 presence_bitmask;
    access_service_class_tdd_lcr_r4_available_sync_ul_codes_indics_t    available_sync_ul_codes_indics;
    access_service_class_tdd_lcr_r4_sub_channel_size_t                  sub_channel_size;
}access_service_class_tdd_lcr_r4_t;

#define UMTS_SIB_ASC_SETTING_TDD_LCR_R4_ACCESS_SERVICE_CLASS_TDD_LCR_R4_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                 presence_bitmask;
    access_service_class_tdd_lcr_r4_t   access_service_class_tdd_lcr_r4;
}asc_setting_tdd_lcr_r4_t;

typedef struct
{
    U8                          count;
    asc_setting_tdd_lcr_r4_t    asc_setting_tdd_lcr_r4[MAX_ASC];
}prach_partitioning_lcr_r4_t;

#define UMTS_SIB_PRACH_SYSTEM_INFORMATION_LCR_R4_TRANSPORT_FORMAT_SET_PRESENCE_FLAG         0x01
#define UMTS_SIB_PRACH_SYSTEM_INFORMATION_LCR_R4_PRACH_PARTITIONING_LCR_R4_PRESENCE_FLAG    0x02
typedef struct
{
    U16                         presence_bitmask;
    prach_rach_info_lcr_r4_t    prach_rach_info_lcr_r4;
    transport_format_set_lcr_t  transport_format_set_lcr;
    prach_partitioning_lcr_r4_t prach_partitioning_lcr_r4;
}prach_system_information_lcr_r4_t;

typedef struct
{
    U8                              	count;
    prach_system_information_lcr_r4_t   prach_system_information_lcr_r4[MAX_PRACH];
}prach_system_information_list_lcr_r4_t;

typedef struct
{
    U8       num_additional_timeslots;
}uplink_timeslots_codes_lcr_r4_more_timeslots_additional_timeslots_consecutive_t;

typedef struct
{
    U8       timeslot_number;
}uplink_additional_timeslots_lcr_r4_parameters_same_as_last_t;

#define UMTS_SIB_INDIVIDUAL_TIMESLOT_INFO_LCR_R4_ADDITIONAL_SS_TPC_SYMBOLS_PRESENCE_BITMASK 0x01
typedef struct
{
    U16             presence_bitmask;
    U8              time_slot_number;
    U8              tfci_existence;
    midamble_shift_and_burst_type_lcr_r4_t  midamble_shift_and_burst_type_lcr_r4;
    U32             modulation;
    U32             ss_tpc_symbols;
    U8              additional_ss_tpc_symbols;
}individual_timeslot_info_lcr_r4_t;

typedef struct
{
    individual_timeslot_info_lcr_r4_t	individual_timeslot_info;
    ul_ts_channelisation_code_list_t	    ul_ts_channelisation_code_list;
}uplink_additional_timeslots_lcr_r4_parameters_new_parameters_t;

#define RRC_UMTS_UPLINKADDITIONALTIMESLOTS_LCR_R4_PARAMETERS_SAMEASLAST 1
#define RRC_UMTS_UPLINKADDITIONALTIMESLOTS_LCR_R4_PARAMETERS_NEWPARAMETERS 2
typedef struct
{
    U32              t;
    union 
    {
        uplink_additional_timeslots_lcr_r4_parameters_same_as_last_t      same_as_last;
        uplink_additional_timeslots_lcr_r4_parameters_new_parameters_t    new_parameters;
    }u;
}uplink_additional_timeslots_lcr_r4_parameters_t;

typedef struct
{
    uplink_additional_timeslots_lcr_r4_parameters_t  parameters;
}uplink_additional_timeslots_lcr_r4_t;

typedef struct
{
    U8          count;
    uplink_additional_timeslots_lcr_r4_t    uplink_additional_timeslots_lcr_r4[MAX_NO_OF_TIMESLOTS_LCR_MINUS_1];
}uplink_timeslots_codes_lcr_r4_more_timeslots_additional_timeslots_timeslot_list_t;

#define RRC_UMTS_UPLINKTIMESLOTSCODES_LCR_R4_MORETIMESLOTS_ADDITIONALTIMESLOTS_CONSECUTIVE 1
#define RRC_UMTS_UPLINKTIMESLOTSCODES_LCR_R4_MORETIMESLOTS_ADDITIONALTIMESLOTS_TIMESLOTLIST 2
typedef struct
{
    U32          t;
    union 
    {
        /* t = 1 */
        uplink_timeslots_codes_lcr_r4_more_timeslots_additional_timeslots_consecutive_t   consecutive;
        /* t = 2 */
        uplink_timeslots_codes_lcr_r4_more_timeslots_additional_timeslots_timeslot_list_t timeslotlist;
    }u;
}uplink_timeslots_codes_lcr_r4_more_timeslots_additional_timeslots_t;

#define RRC_UMTS_UPLINKTIMESLOTSCODES_LCR_R4_MORETIMESLOTS_NOMORE 1
#define RRC_UMTS_UPLINKTIMESLOTSCODES_LCR_R4_MORETIMESLOTS_ADDITIONALTIMESLOTS 2
typedef struct
{
    U32          t;
    union 
    {
        /* t = 1 */
        /* t = 2 */
        uplink_timeslots_codes_lcr_r4_more_timeslots_additional_timeslots_t additional_timeslots;
    }u;
}uplink_timeslots_codes_lcr_r4_more_timeslots_t;

typedef struct
{
    U8                                   dynamicsfusage;
    individual_timeslot_info_lcr_r4_t	firstindividualtimeslotinfo;
    ul_ts_channelisation_code_list_t		ul_ts_channelisationcodelist;
    uplink_timeslots_codes_lcr_r4_more_timeslots_t   moretimeslots;
}uplink_timeslots_codes_lcr_r4_t;

#define UMTS_SIB_PUSCH_INFO_LCR_R4_COMMONTIMESLOTINFO_PRESENCE_FLAG		0x01
#define UMTS_SIB_PUSCH_INFO_LCR_R4_PUSCH_TIMESLOTSCODES_PRESENCE_FLAG	0x02
typedef struct
{
    U16      				presence_bitmask;
    U8                       tfcs_id;
    common_timeslot_info_t	common_timeslot_info;
    uplink_timeslots_codes_lcr_r4_t	pusch_timeslotscodes;
}pusch_info_lcr_r4_t;

#define UMTS_SIB_PUSCH_SYS_INFO_LCR_R4_USCH_TFS_PRESENCE_FLAG    0x01
#define UMTS_SIB_CH_SYS_INFO_LCR_R4_USCH_TFCS_PRESENCE_FLAG      0x02
typedef struct
{
    U16      				presence_bitmask;
    U8                       pusch_identity;
    pusch_info_lcr_r4_t		pusch_info;
    transport_format_set_t   usch_tfs;
    tfcs_t					usch_tfcs;
}pusch_sys_info_lcr_r4_t;

#define UMTS_SIB_PUSCH_SYS_INFO_LIST_SFN_LCR_R4_ELEMENT_SFN_TIMEINFO_PRESENCE_FLAG  0x01
typedef struct
{
    U16      				presence_bitmask;
    pusch_sys_info_lcr_r4_t	pusch_sys_info_lcr_r4;
    sfn_time_info_t			sfn_timeinfo;
}pusch_sys_info_list_sfn_lcr_r4_element_t;

typedef struct
{
    U8                              count;
    pusch_sys_info_list_sfn_lcr_r4_element_t    pusch_sys_info_list_sfn_lcr_r4_element[MAX_PUSCH];
}pusch_sys_info_list_sfn_lcr_r4_t;

typedef struct
{
    U8   timeslot_number;
}downlink_additional_timeslots_lcr_r4_parameters_same_as_last_t;

typedef struct
{
    individual_timeslot_info_lcr_r4_t     individual_timeslot_info;
    dl_ts_channelisation_codes_short_t    dl_ts_channelisation_codes_short;
}downlink_additional_timeslots_lcr_r4_parameters_new_paramaters_t;

#define RRC_UMTS_DOWNLINKADDITIONALTIMESLOTS_LCR_R4_PARAMETERS_SAMEASLAST 1
#define RRC_UMTS_DOWNLINKADDITIONALTIMESLOTS_LCR_R4_PARAMETERS_NEWPARAMETERS 2
typedef struct
{
    U32          t;
    union 
    {
        downlink_additional_timeslots_lcr_r4_parameters_same_as_last_t    same_as_last;
        downlink_additional_timeslots_lcr_r4_parameters_new_paramaters_t	new_paramaters;
    }u;
}downlink_additional_timeslots_lcr_r4_parameters_t;

typedef struct
{
    downlink_additional_timeslots_lcr_r4_parameters_t    parameters;
}downlink_additional_timeslots_lcr_r4_t;

typedef struct
{
    U8                                      count;
    downlink_additional_timeslots_lcr_r4_t	downlink_additional_timeslots_lcr_r4[MAX_NO_OF_TIMESLOTS_LCR_MINUS_1];
}downlink_timeslots_codes_lcr_r4_more_timeslots_additional_timeslots_timeslot_list_t;

#define RRC_UMTS_DOWNLINKTIMESLOTSCODES_LCR_R4_MORETIMESLOTS_ADDITIONALTIMESLOTS_CONSECUTIVE 1
#define RRC_UMTS_DOWNLINKTIMESLOTSCODES_LCR_R4_MORETIMESLOtS_ADDITIONALTIMESLOTS_TIMESLOTLIST 2
typedef struct
{
    U32              t;
    union 
    {
        /* t = 1 */
        U8 consecutive;
        /* t = 2 */
        downlink_timeslots_codes_lcr_r4_more_timeslots_additional_timeslots_timeslot_list_t    timeslot_list;
    } u;
}downlink_timeslots_codes_lcr_r4_more_timeslots_additional_timeslots_t;

#define RRC_UMTS_DOWNLINKTIMESLOTSCODES_LCR_R4_MORETIMESLOTS_NOMORE 1
#define RRC_UMTS_DOWNLINKTIMESLOTSCODES_LCR_R4_MORETIMESLOTS_ADDITIONALTIMESLOTS 2
typedef struct
{
    U32          t;
    union 
    {
        /* t = 1 */
        /* t = 2 */
        downlink_timeslots_codes_lcr_r4_more_timeslots_additional_timeslots_t additional_timeslots;
    }u;
}downlink_timeslots_codes_lcr_r4_more_timeslots_t;

typedef struct
{
    individual_timeslot_info_lcr_r4_t    first_individual_timeslot_info_lcr_r4;
    dl_ts_channelisation_codes_short_t   dl_ts_channelisation_codes_short;
    downlink_timeslots_codes_lcr_r4_more_timeslots_t more_timeslots;
}downlink_timeslots_codes_lcr_r4_t;


#define UMTS_SIB_PDSCH_INFO_LCR_R4_COMMON_TIMESLOT_INFO_PRESENCE_FLAG   0x01
#define UMTS_SIB_PDSCH_INFO_LCR_R4_PDSCH_TIMESLOTS_CODES_PRESENCE_FLAG	0x02
typedef struct
{
    U16						        presence_bitmask;
    U8               		        tfcs_id;
    common_timeslot_info_t		    common_timeslot_info;
    downlink_timeslots_codes_lcr_r4_t	pdsch_timeslots_codes;
}pdsch_info_lcr_r4_t;

#define UMTS_SIB_PDSCH_SYSINFO_LCR_R4_DSCH_TFS_PRESENCE_FLAG    0x01
#define UMTS_SIB_PDSCH_SYSINFO_LCR_R4_DSCH_TFCS_PRESENCE_FLAG	0x02
typedef struct
{
    U16					presence_bitmask;
    U8                   pdsch_identity;
    pdsch_info_lcr_r4_t	pdsch_info;
    transport_format_set_t   dsch_tfs;
    tfcs_t				    dsch_tfcs;
}pdsch_sysinfo_lcr_r4_t;

#define UMTS_SIB_PDSCH_SYSINFOLIST_SFN_LCR_R4_ELEMENT_SFN_TIMEINFO_PRESENCE_FLAG    0x01
typedef struct
{
    U16      				presence_bitmask;
    pdsch_sysinfo_lcr_r4_t	pdsch_sysinfo;
    sfn_time_info_t			sfn_timeinfo;
}pdsch_sysInfolist_sfn_lcr_r4_element_t;

typedef struct
{
    U8                              count;
    pdsch_sysInfolist_sfn_lcr_r4_element_t pdsch_sysInfolist_sfn_lcr_r4_element[MAX_PDSCH];
}pdsch_sys_info_list_sfn_lcr_r4_t;

typedef struct
{
    U8       tstd_indicator;
}primary_ccpch_info_lcr_r4_ext_t;

typedef struct
{
    U8                                      count;
    sccpch_system_information_lcr_r4_ext_t	sccpch_system_information_lcr_r4_ext[MAX_SCCPCH];
}sccpch_system_information_list_lcr_r4_ext_t;

#define UMTS_SIB_TYPE_5_V4B0EXT_IES_TDD_128_SPECIFIC_INFO_PUSCH_SYS_INFO_LIST_SFN_LCR_R4_PRESENCE_FLAG  0x01
#define UMTS_SIB_TYPE_5_V4B0EXT_IES_TDD_128_SPECIFIC_INFO_PDSCH_SYS_INFO_LIST_SFN_LCR_R4_PRESENCE_FLAG  0x02
#define UMTS_SIB_TYPE_5_V4B0EXT_IES_TDD_128_SPECIFIC_INFO_PRIMARY_CCPCH_INFO_LCR_R4_EXT_PRESENCE_FLAG   0x04
typedef struct 
{
    U16                                  presence_bitmask;
    pusch_sys_info_list_sfn_lcr_r4_t     pusch_sys_info_list_sfn_lcr_r4;
    pdsch_sys_info_list_sfn_lcr_r4_t     pdsch_sys_info_list_sfn_lcr_r4;
    primary_ccpch_info_lcr_r4_ext_t      primary_ccpch_info_lcr_r4_ext;
    sccpch_system_information_list_lcr_r4_ext_t  sccpch_system_information_list_lcr_r4_ext;
}sysinfotype5_v4b0ext_ies_tdd_128_specific_info_t;

#define UMTS_SIB_TYPE_5_PNBSCH_ALLOCATION_R4_PRESENCE_FLAG                  0x01
#define UMTS_SIB_TYPE_5_OPEN_LOOP_POWER_CONTROL_IPDL_TDD_R4_PRESENCE_FLAG   0x02
#define UMTS_SIB_TYPE_5_PRACH_SYSTEM_INFORMATION_LIST_LCR_R4_PRESENCE_FLAG  0x04
#define UMTS_SIB_TYPE_5_TDD_128_SPECIFIC_INFO_PRESENCE_FLAG                 0x08
#define UMTS_SIB_TYPE_5_FREQUENCY_BAND_INDICATOR_PRESENCE_FLAG              0x10
typedef struct 
{
    U16                                      presence_bitmask;
    pnbsch_allocation_r4_t                   pnbsch_allocation_r4;
    open_loop_power_control_ipdl_tdd_r4_t    open_loop_power_control_ipdl_tdd_r4;
    prach_system_information_list_lcr_r4_t   prach_system_information_list_lcr_r4;
    sysinfotype5_v4b0ext_ies_tdd_128_specific_info_t tdd_128_specific_info;
    U32                                      frequency_band_indicator;
}sysinfotype5_v4b0ext_ies_t;

typedef struct
{
    U8                       usch_transport_channel_identity;
    transport_format_set_t	usch_tfs;
}usch_transport_channels_info_element_t;

typedef struct
{
    U8                                      count;
    usch_transport_channels_info_element_t  usch_transport_channels_info_element[MAX_TR_CH];
}usch_transport_channels_info_t;

#define UMTS_SIB_PUSCH_SYS_INFO_HCR_R5_USCH_TRANSPORT_CHANNELS_INFO_PRESENCE_FLAG   0x01
#define UMTS_SIB_PUSCH_SYS_INFO_HCR_R5_USCH_TFCS_PRESENCE_FLAG      				0x02
typedef struct
{
    U16		      				    presence_bitmask;
    U8           				    pusch_identity;
    pusch_info_t					    pusch_info;
    usch_transport_channels_info_t	usch_transport_channels_info;
    tfcs_t					        usch_tfcs;
}pusch_sys_info_hcr_r5_t;

#define UMTS_SIB_PUSCH_SYS_INFO_LIST_SFN_HCR_R5_ELEMENT_SFN_TIMEINFO_PRESENCE_FLAG  0x01
typedef struct
{
    U16      				presence_bitmask;
    pusch_sys_info_hcr_r5_t	pusch_sysinfo;
    sfn_time_info_t			sfn_timeinfo;
}pusch_sys_info_list_sfn_hcr_r5_element_t;

typedef struct
{
    U8                                          count;
    pusch_sys_info_list_sfn_hcr_r5_element_t    pusch_sys_info_list_sfn_hcr_r5_element[MAX_PUSCH];
}pusch_sys_info_list_sfn_hcr_r5_t;

typedef struct
{
    U8                       dsch_transport_channel_identity;
    transport_format_set_t	dsch_tfs;
}dsch_transport_channels_info_element_t;

typedef struct
{
    U8                                      count;
    dsch_transport_channels_info_element_t  dsch_transport_channels_info_element[MAX_TR_CH];
}dsch_transport_channels_info_t;

#define UMTS_SIB_PDSCH_SYS_INFO_HCR_R5_DSCH_TRANSPORTCHANNELSINFO_PRESENCE_FLAG     0x01
#define UMTS_SIB_PDSCH_SYS_INFO_HCR_R5_DSCH_TFCS_PRESENCE_FLAG						0x02
typedef struct
{
    U16      						presence_bitmask;
    U8           					pdsch_identity;
    pdsch_info_t						pdsch_info;
    dsch_transport_channels_info_t   dsch_transport_channels_info;
    tfcs_t							dsch_tfcs;
}pdsch_sys_info_hcr_r5_t;

#define UMTS_SIB_PDSCH_SYS_INFO_LIST_SFN_HCR_R5_ELEMENT_SFN_TIMEINFO_PRESENCE_FLAG	0x01
typedef struct
{
    U16      				presence_bitmask;
    pdsch_sys_info_hcr_r5_t	pdsch_sys_info;
    sfn_time_info_t 			sfn_timeinfo;
}pdsch_sys_info_list_sfn_hcr_r5_element_t;

typedef struct
{
    U8                                          count;
    pdsch_sys_info_list_sfn_hcr_r5_element_t	pdsch_sys_info_list_sfn_hcr_r5_element[MAX_PDSCH];
}pdsch_sys_info_list_sfn_hcr_r5_t;

#define UMTS_SIB_TYPE_5_V590EXT_IES_HCR_R5_PUSCH_SYS_INFO_LIST_SFN_PRESENCE_FLAG    0x01
#define UMTS_SIB_TYPE_5_V590EXT_IES_HCR_R5_PDSCH_SYS_INFO_LIST_SFN_PRESENCE_FLAG    0x02
typedef struct
{
    U16                                  presence_bitmask;
    pusch_sys_info_list_sfn_hcr_r5_t     pusch_sys_info_list_sfn;
    pdsch_sys_info_list_sfn_hcr_r5_t     pdsch_sys_info_list_sfn;
}sysinfotype5_v590ext_ies_hcr_r5_specific_info_t;

#define UMTS_SIB_TYPE_5_V590EXT_IES_HCR_R5_SPECIFICINFO_PRESENCE_FLAG   0x01
typedef struct
{
    U16      presence_bitmask;
    sysinfotype5_v590ext_ies_hcr_r5_specific_info_t  hcr_r5_specific_info;
}sysinfotype5_v590ext_ies_t;

typedef struct 
{
    U32     frequency_band_indicator2;
} sysinfotype5_v650ext_ies_t;

#define  UMTS_SIB_TYPE_5_V680EXT_IES_HSDPA_CELLINDICATOR_PRESENCE_FLAG  0x01
typedef struct
{
    U16      presence_bitmask;
    U32      hsdpa_cell_indicator;
}sysinfotype5_v680ext_ies_t;

#define UMTS_SIB_UM_RLC_OUT_O_SEQ_DELIVERY_INFO_R6_TIMER_OSD_PRESENCE_FLAG  0x01
typedef struct
{
    U16     presence_bitmask;
    U32     timer_osd;
    U32     window_size_osd;
}um_rlc_out_o_seq_delivery_info_r6_t;

#define UMTS_SIB_RLC_INFO_MCCH_R6_UM_RLC_OUT_O_SEQ_DELIVERY_INFO_R6_PRESENCE_FLAG  0x01
typedef struct
{
    U16                     presence_bitmask;
    U32                     dl_um_rlc_li_size;
    um_rlc_out_o_seq_delivery_info_r6_t um_rlc_out_o_seq_delivery_info_r6;
}rlc_info_mcch_r6_t;

#define UMTS_SIB_MBMS_MCCH_CONFIGURATION_INFO_R6_TCTF_PRESENCE_FLAG     0x01
typedef struct
{
    U16                 presence_bitmask;
    U8                  access_info_period_coefficient;
    U8                  repetition_period_coefficient;
    U8                  modification_period_coefficient;
    rlc_info_mcch_r6_t  rlc_info;
    U32                 tctf_presence;
}mbms_mcch_configuration_info_r6_t;

#define UMTS_SIB_SECONDARY_CCPCH_INFO_MBMS_R6_MODE_SPECIFIC_INFO_FDD_SECONDARY_SCRAMBLING_CODE_PRESENCE_FLAG  0x01
typedef struct
{
    U16                      presence_bitmask;
    U8                       secondary_scrambling_code;
    U8                       sttd_indicator;
    sf256_and_code_number_t  sf256_and_code_number;
    U8                       timing_offset;
}secondary_ccpch_info_mbms_r6_mode_specific_info_fdd_t;

#define UMTS_SIB_COMMON_TIMESLOT_INFO_MBMS_TFCI_CODING_PRESENCE_FLAG    0x01
typedef struct
{
    U16          presence_bitmask;
    U32          second_interleaving_mode;
    U32          tfci_coding;
    U32          puncturing_limit;
}common_timeslot_info_mbms_t;

typedef struct
{
    common_timeslot_info_mbms_t  common_timeslot_info_mbms;
    downlink_timeslots_codes_t   downlink_timeslots_codes;
}secondary_ccpch_info_mbms_r6_mode_specific_info_tdd384_t;

typedef struct
{
    common_timeslot_info_mbms_t          common_timeslot_info_mbms;
    downlink_timeslots_codes_lcr_r4_t    downlink_timeslots_codes;
}secondary_ccpch_info_mbms_r6_mode_specific_info_tdd128_t;

#define RRC_UMTS_SECONDARYCCPCHINFO_MBMS_R6_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_SECONDARYCCPCHINFO_MBMS_R6_MODESPECIFICINFO_TDD384 2
#define RRC_UMTS_SECONDARYCCPCHINFO_MBMS_R6_MODESPECIFICINFO_TDD128 3
typedef struct
{
    U32      t;
    union 
    {
        /* t = 1 */
        secondary_ccpch_info_mbms_r6_mode_specific_info_fdd_t     fdd;
        /* t = 2 */
        secondary_ccpch_info_mbms_r6_mode_specific_info_tdd384_t  tdd384;
        /* t = 3 */
        secondary_ccpch_info_mbms_r6_mode_specific_info_tdd128_t  tdd128;
    }u;
}secondary_ccpch_info_mbms_r6_mode_specific_info_t;

typedef struct
{
    secondary_ccpch_info_mbms_r6_mode_specific_info_t   mode_specific_info;
}secondary_ccpch_info_mbms_r6_t;

typedef struct
{
    transport_format_set_t               mcch_transport_format_set;
    mbms_mcch_configuration_info_r6_t    mcch_configuration_info;
}sccpch_system_information_mbms_r6_fach_carrying_mcch_t;

typedef struct
{
    U8                          count;
    transport_format_set_t      transport_format_set[MAX_FACH_PCH];
}mbms_fach_carrying_mtch_list_t;

#define RRC_UMTS_MBMS_MSCHSCHEDULINGINFO_SCHEDULINGPERIOD_32_OFFSET 1
#define RRC_UMTS_MBMS_MSCHSCHEDULINGINFO_SCHEDULINGPERIOD_64_OFFSET 2
#define RRC_UMTS_MBMS_MSCHSCHEDULINGINFO_SCHEDULINGPERIOD_128_OFFSET 3
#define RRC_UMTS_MBMS_MSCHSCHEDULINGINFO_SCHEDULINGPERIOD_256_OFFSET 4
#define RRC_UMTS_MBMS_MSCHSCHEDULINGINFO_SCHEDULINGPERIOD_512_OFFSET 5
#define RRC_UMTS_MBMS_MSCHSCHEDULINGINFO_SCHEDULINGPERIOD_1024_OFFSET 6
typedef struct
{
    U32      t;
    union 
    {
        /* t = 1 */
        U8    schedulingPeriod_32_Offset;
        /* t = 2 */
        U8    schedulingPeriod_64_Offset;
        /* t = 3 */
        U8    schedulingPeriod_128_Offset;
        /* t = 4 */
        U8    schedulingPeriod_256_Offset;
        /* t = 5 */
        U16   schedulingPeriod_512_Offset;
        /* t = 6 */
        U16   schedulingPeriod_1024_Offset;
    } u;
}mbms_msch_scheduling_info_t;

typedef struct
{
    U32      dl_um_rlc_li_size;
}rlc_info_msch_r6_t;

#define UMTS_SIB_MBMS_MSCH_CONFIGURATION_INFO_R6_MBMS_MSCH_SCHEDULING_INFO_PRESENCE_FLAG    0x01
#define UMTS_SIB_MBMS_MSCH_CONFIGURATION_INFO_R6_RLC_INFO_PRESENCE_FLAG                     0x02
#define UMTS_SIB_MBMS_MSCH_CONFIGURATION_INFO_R6_TCTF_PRESENCE_PRESENCE_FLAG                0x04
typedef struct
{
    U16                          presence_bitmask;
    mbms_msch_scheduling_info_t  mbms_msch_scheduling_info;
    rlc_info_msch_r6_t           rlc_info; 
    U32                          tctf_presence;
}mbms_msch_configuration_info_r6_t;

typedef struct
{
    transport_format_set_t               msch_transport_format_set;
    mbms_msch_configuration_info_r6_t    dummy;
}sccpch_system_information_mbms_r6_fach_carrying_msch_t;

#define UMTS_SIB_SCCPCH_SYSTEM_INFORMATION_MBMS_R6_FACH_CARRYING_MTCH_LIST_PRESENCE_FLAG    0x01
#define UMTS_SIB_SCCPCH_SYSTEM_INFORMATION_MBMS_R6_FACH_CARRYING_MSCH_PRESENCE_FLAG         0x02
typedef struct
{
    U16                              presence_bitmask;
    secondary_ccpch_info_mbms_r6_t   secondary_ccpch_info_mbms_r6;
    tfcs_t                           transport_format_combination_set;
    sccpch_system_information_mbms_r6_fach_carrying_mcch_t   fach_carrying_mcch;
    mbms_fach_carrying_mtch_list_t                           fach_carrying_mtch_list;
    sccpch_system_information_mbms_r6_fach_carrying_msch_t   fach_carrying_msch;
}sccpch_system_information_mbms_r6_t;

#define RRC_UMTS_SYSINFOTYPE5_V690EXT_IEs_SCCPCH_SYSTEMINFORMATION_MBMS_MCCHONSCCPCHUSEDFORNONMBMS 1
#define RRC_UMTS_SYSINFOTYPE5_V690EXT_IEs_SCCPCH_SYSTEMINFORMATION_MBMS_MCCHONSCCPCHUSEDONLYFORMBMS 2
typedef struct
{
    U32          t;
    union 
    {
        /* t = 1 */
        mbms_mcch_configuration_info_r6_t     mcch_on_sccpch_used_for_non_mbms;
        /* t = 2 */
        sccpch_system_information_mbms_r6_t   mcch_on_sccpch_used_only_for_mbms;
    }u;
}sysinfotype5_v690ext_ies_sccpch_system_information_mbms_t;

typedef struct
{
    octet_mode_rlc_size_info_type2_t     octet_mode_rlc_size_info_type2;
}dynamic_tf_information_ccch_t;

typedef struct
{
    power_offset_information_t       power_offset_information;
    dynamic_tf_information_ccch_t    dynamic_tf_information_ccch;
}additional_prach_tf_and_tfcs_ccch_ies_t;

#define UMTS_SIB_ADDITIONAL_PRACH_TF_AND_TFCS_CCCH_IES_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                      presence_bitmask;
    additional_prach_tf_and_tfcs_ccch_ies_t  additional_prach_tf_and_tfcs_ccch_ies;
}additional_prach_tf_and_tfcs_ccch_t;

typedef struct
{
    U8                                      count;
    additional_prach_tf_and_tfcs_ccch_t     additional_prach_tf_and_tfcs_ccch[MAX_PRACH];
}additional_prach_tf_and_tfcs_ccch_list_t;

#define UMTS_SIB_TYPE_5_V690EXT_IES_EDCH_CELLINDICATOR_PRESENCE_FLAG                        0x01
#define UMTS_SIB_TYPE_5_V690EXT_IES_SYSTEM_INFORMATION_MBMS_PRESENCE_FLAG                   0x02
#define UMTS_SIB_TYPE_5_V690EXT_IES_ADDITIONAL_PRACH_TF_AND_TFCS_CCCH_LIST_PRESENCE_FLAG    0x04
#define UMTS_SIB_TYPE_5_V690EXT_IES_CBS_DRX_LEVEL1_INFORMATION_EXTENSION_R6_PRESENCE_FLAG   0x08
typedef struct
{
    U16                                          presence_bitmask;
    U32                                          edch_cellindicator;
    sysinfotype5_v690ext_ies_sccpch_system_information_mbms_t    system_information_mbms;
    additional_prach_tf_and_tfcs_ccch_list_t     additional_prach_tf_and_tfcs_ccch_list;
    U32	                                        cbs_drx_level1_information_extension_r6;
}sysinfotype5_v690ext_ies_t;

typedef struct
{
    U32              t_321;
    U32              hs_dsch_drx_cycle_fach;
    U32              hs_dsch_drx_burst_fach;
    U8               drx_interruption_hs_dsch;     
}hs_dsch_drx_cell_fach_info_t;

#define UMTS_SIB_SYSINFOTYPE5_V860EXT_IES_MODE_SPECIFIC_INFO_FDD_HS_DSCH_DRX_CELL_FACH_INFO_PRESENCE_FLAG   0x01
typedef struct
{
    U16                          presence_bitmask;
    hs_dsch_drx_cell_fach_info_t	hs_dsch_drx_cell_fach_info;
}sysinfotype5_v860ext_ies_mode_specific_info_fdd_t;

typedef struct
{
    U8       logical_channel_identity;
    U8       mac_ehs_queue_id;
}common_rb_mapping_info_t;

#define UMTS_SIB_COMMON_MAC_EHS_REORDERING_QUEUE_REORDERING_RESET_TIMER_PRESENCE_FLAG   0x01
typedef struct
{
    U16          presence_bitmask;
    U8           mac_ehs_queueid;
    U32          t1_release_timer;
    U32          reordering_reset_timer;
    U32          mac_ehs_window_size;
}common_mac_ehs_reordering_queue_t;

typedef struct 
{
    U8                                  count;
    common_mac_ehs_reordering_queue_t   common_mac_ehs_reordering_queue[MAC_COMMON_QUEUE_ID];
}common_mac_ehs_reordering_queue_list_t;

#define RRC_UMTS_HS_SCCH_TDD128_R6_MIDAMBLEALLOCATIONMODE_DEFAULTMIDAMBLE 1
#define RRC_UMTS_HS_SCCH_TDD128_R6_MIDAMBLEALLOCATIONMODE_COMMONMIDAMBLE 2
#define RRC_UMTS_HS_SCCH_TDD128_R6_MIDAMBLEALLOCATIONMODE_UESPECIFICMIDAMBLE 3
typedef struct
{
    U32         t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        /* t = 3 */
        U8      ue_specific_midamble;
    }u;
}hs_scch_tdd128_r6_midamble_allocation_mode_t;

typedef struct
{
    U8          midamble_shift;
}hs_sich_configuration_tdd128_r6_midamble_allocation_mode_ue_specific_midamble_t;

#define RRC_UMTS_HS_SICH_CONFIGURATION_TDD128_R6_MIDAMBLEALLOCATIONMODE_DEFAULTMIDAMBLE 1
#define RRC_UMTS_HS_SICH_CONFIGURATION_TDD128_R6_MIDAMBLEALLOCATIONMODE_UESPECIFICMIDAMBLE 2
typedef struct
{
    U32         t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        hs_sich_configuration_tdd128_r6_midamble_allocation_mode_ue_specific_midamble_t ue_specific_midamble;
    }u;
}hs_sich_configuration_tdd128_r6_midamble_allocation_mode_t;

typedef struct
{
    U8                  timeslot_number;
    U32                 channelisation_code;
    hs_sich_configuration_tdd128_r6_midamble_allocation_mode_t  midamble_allocation_mode;
    U8                  midamble_configuration;
}hs_sich_configuration_tdd128_r6_t;

typedef struct
{
    U8              timeslot_number;
    U32             first_channelisation_code;
    U32             second_channelisation_code;
    hs_scch_tdd128_r6_midamble_allocation_mode_t    midamble_allocation_mode;
    U8              midamble_configuration;
    hs_sich_configuration_tdd128_r6_t               hs_sich_configuration;
}hs_scch_tdd128_r6_t;

typedef struct 
{
    U8                  count;
    hs_scch_tdd128_r6_t hs_scch_tdd128_r6[MAX_HSSCCHS];
}hs_scch_system_info_tdd128_hs_scch_set_configuration_t;

#define UMTS_SIB_HS_SCCH_SYSTEMINFO_TDD128_POWER_CONTROL_GAP_PRESENCE_FLAG				    0x01
#define UMTS_SIB_HS_SCCH_SYSTEMINFO_TDD128_PATHLOSS_COMPENSATION_SWITCH_PRESENCE_FLAG		0x02
typedef struct
{
    U16                  presence_bitmask;
    hs_scch_system_info_tdd128_hs_scch_set_configuration_t   hs_scch_set_configuration;
    S8                   power_level_hssich;
    S8                   nack_ack_power_offset;
    U32                  tpc_step_size;
    U8                   bler_target;
    U8                   power_control_gap;
    U8                   pathloss_compensation_switch;
}hs_scch_system_info_tdd128_t;

typedef struct
{
    U32              n;
    U32              elem[8];
}harq_info_memory_partitioning_explicit_t;

#define RRC_UMTS_HARQ_INFO_MEMORYPARTITIONING_IMPLICIT 1
#define RRC_UMTS_HARQ_INFO_MEMORYPARTITIONING_EXPLICIT_ 2
typedef struct
{
    U32      t;
    union 
    {
        /* t = 1 */
        /* t = 2 */
        harq_info_memory_partitioning_explicit_t explicit_;
    } u;
}harq_info_memory_partitioning_t;

typedef struct
{
    U8                               number_of_processes;
    harq_info_memory_partitioning_t  memory_partitioning;
}harq_info_t;

#define RRC_UMTS_HS_PDSCH_MIDAMBLE_CONFIGURATION_TDD128_MIDAMBLEALLOCATIONMODE_DEFAULTMIDAMBLE 1
#define RRC_UMTS_HS_PDSCH_MIDAMBLE_CONFIGURATION_TDD128_MIDAMBLEALLOCATIONMODE_COMMONMIDAMBLE 2
#define RRC_UMTS_HS_PDSCH_MIDAMBLE_CONFIGURATION_TDD128_MIDAMBLEALLOCATIONMODE_UESPECIFICMIDAMBLE 3
typedef struct
{
    U32 t;
    union {
        /* t = 1 */
        /* t = 2 */
        /* t = 3 */
        U8 ue_specific_midamble;
    } u;
}hs_pdsch_midamble_configuration_tdd128_midamble_allocation_mode_t;

typedef struct
{
    hs_pdsch_midamble_configuration_tdd128_midamble_allocation_mode_t  midamble_allocation_mode;
    U8           midamble_configuration;
}hs_pdsch_midamble_configuration_tdd128_t;

typedef struct
{
    U32         numbits;
    U8          data[4];
}h_rnti_t;

typedef struct
{
    U32      n;
    h_rnti_t elem[4];
}hs_dsch_common_system_information_tdd128_common_h_rnti_information_t;

#define UMTS_SIB_HS_DSCH_COMMON_SYSTEM_INFORMATION_TDD128_SRB1_MAPPING_INFO_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                      presence_bitmask;
    common_rb_mapping_info_t                 ccch_mapping_info;
    common_rb_mapping_info_t                 srb1_mapping_info;
    common_mac_ehs_reordering_queue_list_t   common_mac_ehs_reordering_queue_list;
    hs_scch_system_info_tdd128_t             hs_scch_systeminfo_tdd128;
    harq_info_t                              harq_system_info;
    hs_pdsch_midamble_configuration_tdd128_t hs_pdsch_midamble_configuration;
    hs_dsch_common_system_information_tdd128_common_h_rnti_information_t common_h_rnti_information;
    h_rnti_t                                 bcch_specific_h_rnti;
}hs_dsch_common_system_information_tdd128_t;

#define UMTS_SIB_PICH_FOR_HSDPA_SUPPORTED_PAGING_TDD128_IMPLICIT_OCCURRENCE_SEQUENCE_NUMBER_OF_PICH_PRESENCE_FLAG   0x01
typedef struct
{
    U16      presence_bitmask;
    U8       occurrence_sequence_number_of_pich;
}pich_for_hsdpa_supported_paging_tdd128_implicit_t;

#define RRC_UMTS_PICH_FORHSDPASUPPORTEDPAGING_TDD128_IMPLICIT 1
#define RRC_UMTS_PICH_FORHSDPASUPPORTEDPAGING_TDD128_EXPLICIT_ 2
typedef struct
{
    U32      t;
    union 
    {
        pich_for_hsdpa_supported_paging_tdd128_implicit_t implicit;
        pich_info_lcr_r4_t                                explicit_;
    } u;
}pich_for_hsdpa_supported_paging_tdd128_t;

typedef struct 
{ 
    U8          count;
    pich_for_hsdpa_supported_paging_tdd128_t    pich_for_hsdpa_supported_paging_tdd128[MAX_SCCPCH];
}hs_dsch_paging_system_information_tdd128_pich_for_hs_dsch_list_t;

typedef struct
{
    U32                 numbits;
    U8                  data[4];
}umts1_bit_str32_t;

typedef struct
{
    U32         start_code;
    U32         stop_code;
}code_resource_information_tdd128_t;

typedef struct
{
    hs_pdsch_midamble_configuration_tdd128_t    hs_pdsch_midamble_configuration_tdd128;
    umts1_bit_str32_t                           timeslot_resource_related_info;
    code_resource_information_tdd128_t          code_resource_information;
}pcch_information_list_paging_associated_hs_pdsch_info_element_t;

typedef struct 
{ 
    U8          count;
    pcch_information_list_paging_associated_hs_pdsch_info_element_t hs_pdsch_info_element[MAX_SCCPCH];
}pcch_information_list_paging_associated_hs_pdsch_info_t;

typedef struct
{
    U32      n;
    U8       elem[2];
}pcch_information_list_transport_block_size_list_t;

typedef struct
{
    pcch_information_list_paging_associated_hs_pdsch_info_t  paging_associated_hs_pdsch_info;
    U8                                                       paging_sub_channel_size;
    pcch_information_list_transport_block_size_list_t        transport_block_size_list;
}pcch_information_list_t;

#define UMTS_SIB_HS_DSCH_PAGING_SYSTEM_INFORMATION_TDD128_PCCH_INFORMATION_LIST_PRESENCE_FLAG   0x01
typedef struct
{
    U16                      presence_bitmask;
    hs_dsch_paging_system_information_tdd128_pich_for_hs_dsch_list_t    pich_for_hs_dsch_list;
    U8                       dtch_dcch_reception_window_size;
    pcch_information_list_t  pcch_information_list;
}hs_dsch_paging_system_information_tdd128_t;

#define UMTS_SIB_SYSINFOTYPE5_V860EXT_IES_MODE_SPECIFIC_INFO_TDD128_HS_DSCH_COMMON_SYS_INFO_PAGING_SYSTEM_INFO_PRESENCE_FLAG    0x01
typedef struct
{
    U16              presence_bitmask;
    hs_dsch_common_system_information_tdd128_t   hs_dsch_common_system_information;
    hs_dsch_paging_system_information_tdd128_t   hs_dsch_paging_system_information;
}sysinfotype5_v860ext_ies_mode_specific_info_tdd128_hs_dsch_common_sys_info_t;

typedef struct
{
    U32          t_321;
    U32          hs_dsch_drx_cycle_fach;
    U32          hs_dsch_drx_burst_fach;
}hs_dsch_drx_cell_fach_info_tdd128_t;

#define UMTS_SIB_SYSINFOTYPE5_V860EXT_IES_MODE_SPECIFIC_INFO_TDD128_HS_DSCH_COMMON_SYS_INFO_PRESENCE_FLAG       0x01
#define UMTS_SIB_SYSINFOTYPE5_V860EXT_IES_MODE_SPECIFIC_INFO_TDD128_HS_DSCH_DRX_CELL_FACH_INFO_PRESENCE_FLAG    0x02
typedef struct
{
    U16                      presence_bitmask;
    sysinfotype5_v860ext_ies_mode_specific_info_tdd128_hs_dsch_common_sys_info_t hs_dsch_common_sys_info;
    hs_dsch_drx_cell_fach_info_tdd128_t  hs_dsch_drx_cell_fach_info;
}sysinfotype5_v860ext_ies_mode_specific_info_tdd128_t;

#define RRC_UMTS_SYSINFOTYPE5_V860EXT_IES_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_SYSINFOTYPE5_V860EXT_IES_MODESPECIFICINFO_TDD128 2
typedef struct
{
    U32 t;
    union 
    {
        sysinfotype5_v860ext_ies_mode_specific_info_fdd_t     fdd;
        sysinfotype5_v860ext_ies_mode_specific_info_tdd128_t  tdd128;
    } u;
}sysinfotype5_v860ext_ies_mode_specific_info_t;

typedef struct
{
    U8 __dummy__;
}sysinfotype5_v860ext_ies_dummy_t;

#define UMTS_SIB_SYSINFOTYPE5_V860EXT_IES_DUMMY_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                  presence_bitmask;
    sysinfotype5_v860ext_ies_dummy_t     dummy;
    sysinfotype5_v860ext_ies_mode_specific_info_t    mode_specific_info;
}sysinfotype5_v860ext_ies_t;

#define UMTS_SIB_SYSINFOTYPE5_V8B0EXT_IES_TRESET_USAGE_INDICATOR_PRESENCE_FLAG  0x01
#define UMTS_SIB_SYSINFOTYPE5_V8B0EXT_IES_UP_PCH_POSITION_INFO_PRESENCE_FLAG    0x02
typedef struct
{
    U16                 presence_bitmask;
    U32                 treset_usage_indicator;
    U8                  up_pch_position_info;
}sysinfotype5_v8b0ext_ies_t;

typedef struct
{
    U32         numbits;
    U8          data[2];
}available_sub_channel_numbers_t;

typedef struct
{
    U8              power_ramp_step;
    U8              preamble_retrans_max;
}prach_power_offset_t;

typedef struct 
{
    U32      numbits;
    U8       data[2];
}access_service_class_tdd_r7_channelisation_code_indices_t;

#define UMTS_SIB_ACCESS_SERVICE_CLASS_TDD_R7_SUBCHANNEL_SIZE_SIZE2_SUBCHANNELS_PRESENCE_FLAG    0x01
typedef struct 
{
    U16     presence_bitmask;
    U32     subchannels;
}access_service_class_tdd_r7_subchannel_size_size2_t;

typedef struct
{
    U32      numbits;
    U8       data[1];
}access_service_class_tdd_r7_subchannel_size_size4_subchannels_t;

#define UMTS_SIB_ACCESS_SERVICE_CLASS_TDD_R7_SUBCHANNEL_SIZE_SIZE4_SUBCHANNELS_PRESENCE_FLAG    0x01
typedef struct
{
    U16     presence_bitmask;
    access_service_class_tdd_r7_subchannel_size_size4_subchannels_t     subchannels;
}access_service_class_tdd_r7_subchannel_size_size4_t;

typedef struct
{
    U32      numbits;
    U8       data[1];
}access_service_class_tdd_r7_subchannel_size_size8_subchannels_t;

#define UMTS_SIB_ACCESS_SERVICE_CLASS_TDD_R7_SUBCHANNEL_SIZE_SIZE8_SUBCHANNELS_PRESENCE_FLAG    0x01
typedef struct
{
    U16     presence_bitmask;
    access_service_class_tdd_r7_subchannel_size_size8_subchannels_t     subchannels;
}access_service_class_tdd_r7_subchannel_size_size8_t;

typedef struct
{
    U32      numbits;
    U8       data[2];
}access_service_class_tdd_r7_subchannel_size_size16_subchannels_t;

#define UMTS_SIB_ACCESS_SERVICE_CLASS_TDD_R7_SUBCHANNEL_SIZE_SIZE16_SUBCHANNELS_PRESENCE_FLAG    0x01
typedef struct
{
    U16     presence_bitmask;
    access_service_class_tdd_r7_subchannel_size_size16_subchannels_t    subchannels;
}access_service_class_tdd_r7_subchannel_size_size16_t;

#define RRC_UMTS_ACCESSSERVICECLASS_TDD_R7_SUBCHANNELSIZE_SIZE1 1
#define RRC_UMTS_ACCESSSERVICECLASS_TDD_R7_SUBCHANNELSIZE_SIZE2 2
#define RRC_UMTS_ACCESSSERVICECLASS_TDD_R7_SUBCHANNELSIZE_SIZE4 3
#define RRC_UMTS_ACCESSSERVICECLASS_TDD_R7_SUBCHANNELSIZE_SIZE8 4
#define RRC_UMTS_ACCESSSERVICECLASS_TDD_R7_SUBCHANNELSIZE_SIZE16 5
typedef struct
{
    U32      t;
    union 
    {
        access_service_class_tdd_r7_subchannel_size_size2_t   size2;
        access_service_class_tdd_r7_subchannel_size_size4_t   size4;
        access_service_class_tdd_r7_subchannel_size_size8_t   size8;
        access_service_class_tdd_r7_subchannel_size_size16_t  size16;
    }u;
}access_service_class_tdd_r7_subchannel_size_t;

#define UMTS_SIB_ACCESS_SERVICE_CLASS_TDD_R7_CHANNELISATION_CODE_INDICES_PRESENCE_FLAG  0x01
typedef struct
{
    U16                 presence_bitmask;
    access_service_class_tdd_r7_channelisation_code_indices_t    channelisation_code_indices;
    access_service_class_tdd_r7_subchannel_size_t                subchannel_size;
}access_service_class_tdd_r7_t;

#define UMTS_SIB_ASC_SETTING_TDD_R7_ACCESS_SERVICE_CLASS_TDD_R7_PRESENCE_FLAG   0x01
typedef struct
{
    U16                              presence_bitmask;
    access_service_class_tdd_r7_t    access_service_class_tdd_r7;
}asc_setting_tdd_r7_t;

typedef struct
{
    U8                      count;
    asc_setting_tdd_r7_t    asc_setting_tdd_r7[MAX_ASC];
}prach_partitioning_r7_tdd_t;

#define UMTS_SIB_ASC_SETTING_FDD_ACCESS_SERVICE_CLASS_FDD_PRESENCE_FLAG  0x01
typedef struct
{
    U16                          presence_bitmask;
    access_service_class_fdd_t   access_service_class_fdd;
}asc_setting_fdd_t;

typedef struct
{
    U8                  count;
    asc_setting_fdd_t   asc_setting_fdd[MAX_ASC]; 
}prach_partitioning_r7_fdd_t;

#define RRC_UMTS_PRACH_PARTITIONING_R7_FDD 1
#define RRC_UMTS_PRACH_PARTITIONING_R7_TDD 2
typedef struct
{
    U32      t;
    union 
    {
        prach_partitioning_r7_fdd_t   fdd;
        prach_partitioning_r7_tdd_t   tdd;
    }u;
}prach_partitioning_r7_t;

typedef struct
{
    U32                 n;
    U32                 elem[6];
}persistence_scaling_factor_list_t;

#define UMTS_SIB_PRACH_PREAMBLE_FOR_ENHANCED_UPLINK_AVAILABLE_SIGNATURES_PRESENCE_FLAG                  0x01
#define UMTS_SIB_PRACH_PREAMBLE_FOR_ENHANCED_UPLINK_PREAMBLE_SCRAMBLING_CODE_WORD_NUMBER_PRESENCE_FLAG  0x02
#define UMTS_SIB_PRACH_PREAMBLE_FOR_ENHANCED_UPLINK_AVAILABLE_SUB_CHANNEL_NUMBERS_PRESENCE_FLAG         0x04
#define UMTS_SIB_PRACH_PREAMBLE_FOR_ENHANCED_UPLINK_PRACH_PARTITIONING_R7_PRESENCE_FLAG                 0x08
#define UMTS_SIB_PRACH_PREAMBLE_FOR_ENHANCED_UPLINK_PERSISTENCE_SCALING_FACTOR_LIST_PRESENCE_FLAG       0x10
#define UMTS_SIB_PRACH_PREAMBLE_FOR_ENHANCED_UPLINK_AC_TO_ASC_MAPPING_TABLE_PRESENCE_FLAG               0x20
#define UMTS_SIB_PRACH_PREAMBLE_FOR_ENHANCED_UPLINK_PRIMARY_CPICH_TX_POWER_PRESENCE_FLAG                0x40
#define UMTS_SIB_PRACH_PREAMBLE_FOR_ENHANCED_UPLINK_CONSTANT_VALUE_PRESENCE_FLAG                        0x80
#define UMTS_SIB_PRACH_PREAMBLE_FOR_ENHANCED_UPLINK_PRACH_POWER_OFFSET_PRESENCE_FLAG                    0x100
#define UMTS_SIB_PRACH_PREAMBLE_FOR_ENHANCED_UPLINK_RACH_TRANSMISSION_PARAMETERS_PRESENCE_FLAG          0x200
#define UMTS_SIB_PRACH_PREAMBLE_FOR_ENHANCED_UPLINK_AICH_INFO_PRESENCE_FLAG                             0x400
typedef struct
{
    U16                             presence_bitmask;
    available_signatures_t          available_signatures;
    U8                              e_ai_indication;
    U8                              preamble_scrambling_code_word_number;
    available_sub_channel_numbers_t available_sub_channel_numbers;
    prach_partitioning_r7_t         prach_partitioning_r7;
    persistence_scaling_factor_list_t   persistence_scaling_factor_list;
    ac_to_asc_mapping_table_t           ac_to_asc_mapping_table;
    S8                                  primary_cpich_tx_power;
    S8                                  constant_value;
    prach_power_offset_t                prach_power_offset;
    rach_transmission_parameters_t      rach_transmission_parameters;
    aich_info_t                         aich_info;
    S8                                  power_offset_pp_e;
}prach_preamble_for_enhanced_uplink_t;

typedef struct
{
    U8      e_agch_channelisation_code;
}e_agch_information_t;

#define RRC_UMTS_POWERCONTROLALGORITHM_ALGORITHM1 1
#define RRC_UMTS_POWERCONTROLALGORITHM_ALGORITHM2 2
typedef struct
{
    U32                 t;
    union
    {  
        /* t = 1 */ 
        U8              algorithm1;
        /* t = 2 */
    }u;
}power_control_algorithm_t;

#define UL_DPCH_POWER_CONTROL_INFO_FOR_COMMON_EDCH_DELTA_ACK_PRESENCE_FLAG                  0x01
#define UL_DPCH_POWER_CONTROL_INFO_FOR_COMMON_EDCH_DELTA_NACK_PRESENCE_FLAG                 0x02
#define UL_DPCH_POWER_CONTROL_INFO_FOR_COMMON_EDCH_ACK_NACK_REPETITION_FACTOR_PRESENCE_FLAG 0x04
typedef struct
{
    U16                         presence_bitmask;
    power_control_algorithm_t   power_control_algorithm;
    U8                          delta_ack;
    U8                          delta_nack;
    U8                          ack_nack_repetition_factor;
}ul_dpch_power_control_info_for_common_edch_t;

#define UMTS_SIB_E_TFC_BOOST_INFO_R7_DELTA_T2TP_PRESENCE_FLAG   0x01
typedef struct
{
    U16                     presence_bitmask;
    U8                      e_tfci_boost;
    U8                      delta_t2tp;
}e_tfc_boost_info_r7_t;

#define UMTS_SIB_E_DPCCH_INFO_R7_E_TFC_BOOST_INFO_PRESENCE_FLAG             0x01
#define UMTS_SIB_E_DPCCH_INFO_R7_E_DPDCH_POWER_INTERPOLATION_PRESENCE_FLAG  0x02
typedef struct
{
    U16                     presence_bitmask;
    U8                      e_dpcch_dpcch_power_offset;
    U32                     happy_bit_delay_condition;
    e_tfc_boost_info_r7_t   e_tfc_boost_info;
    U8                      e_dpdch_power_interpolation;
}e_dpcch_info_r7_t;

typedef struct
{
    U8      reference_e_tfci;
    U8      reference_e_tfci_po_r7;
}e_dpdch_reference_e_tfci_r7_t;

typedef struct
{
    U8                              count;
    e_dpdch_reference_e_tfci_r7_t   e_dpdch_reference_e_tfci_r7[8];
}e_dpdch_reference_e_tfci_list_r7_t;

#define UMTS_SIB_E_DPDCH_SCHEDULING_INFO_CONFIGURATION_PERIODICITY_OF_SCHED_INFO_NO_GRANT_PRESENCE_FLAG 0x01
#define UMTS_SIB_E_DPDCH_SCHEDULING_INFO_CONFIGURATION_PERIODICITY_OF_SCHED_INFO_GRANT_PRESENCE_FLAG    0x02
typedef struct
{
    U16                 presence_bitmask;
    U32                 periodicity_of_sched_info_no_grant;
    U32                 periodicity_of_sched_info_grant;
    U8                  power_offset_for_sched_info;
}e_dpdch_scheduling_info_configuration_t;

#define UMTS_SIB_E_DPDCH_INFO_R8_E_DCH_MINIMUM_SET_E_TFCI_PRESENCE_FLAG         0x10
#define UMTS_SIB_E_DPDCH_INFO_R8_MIN_REDUCED_E_DPDCH_GAIN_FACTOR_PRESENCE_FLAG  0x20
#define UMTS_SIB_E_DPDCH_INFO_R8_THREE_INDEX_STEP_THRESHOLD_PRESENCE_FLAG       0x40
#define UMTS_SIB_E_DPDCH_INFO_R8_TWO_INDEX_STEP_THRESHOLD_PRESENCE_FLAG         0x80
typedef struct
{
    U16                     presence_bitmask;
    U8                      e_tfci_table_index;
    U8                      e_dch_minimum_set_e_tfci;
    e_dpdch_reference_e_tfci_list_r7_t  e_dpdch_reference_e_tfci_list;
    U32                     min_reduced_e_dpdch_gain_factor;
    U32                     max_channelisation_codes;
    U8                      pl_non_max;
    e_dpdch_scheduling_info_configuration_t e_dpdch_scheduling_info_configuration;
    U8                      three_index_step_threshold;
    U8                      two_index_step_threshold;
}e_dpdch_info_r8_t;

typedef struct
{
    S8              measurement_power_offset;
    U32             feedback_cycle;
    U8              cqi_repetition_factor;
    U8              delta_cqi;
}measurement_feedback_info_r7_mode_specific_info_fdd_t;

#define RRC_UMTS_MEASUREMENT_FEEDBACK_INFO_R7_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_MEASUREMENT_FEEDBACK_INFO_R7_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32         t;
    union
    {
        /* t = 1 */
        measurement_feedback_info_r7_mode_specific_info_fdd_t   fdd;
        /* t = 2 */
    }u;
}measurement_feedback_info_r7_mode_specific_info_t;

typedef struct
{
    measurement_feedback_info_r7_mode_specific_info_t   mode_specific_info;
}measurement_feedback_info_r7_t;

#define UMTS_SIB_E_RGCH_INFORMATION_COMMON_EDCH_PRESENCE_FLAG   0x01
typedef struct
{
    U16             presence_bitmask;
    U8              signature_sequence;
}e_rgch_information_common_edch_t;

#define UMTS_SIB_E_HICH_INFORMATION_COMMON_EDCH_CHANNELISATION_CODE_PRESENCE_FLAG   0x01
#define UMTS_SIB_E_HICH_INFORMATION_COMMON_EDCH_SIGNATURE_SEQUENCE_PRESENCE_FLAG    0x02
typedef struct
{
    U16             presence_bitmask; 
    U8              channelisation_code;
    U8              signature_sequence;
}e_hich_information_common_edch_t;

#define UMTS_SIB_UL_DPCH_CODE_INFO_FOR_COMMON_EDCH_UL_DPCCH_SCRAMBLING_CODE_PRESENCE_FLAG   0x01
typedef struct
{
    U16             presence_bitmask;
    U32             ul_dpcch_scrambling_code_type;
    U32             ul_dpcch_scrambling_code;
}ul_dpch_code_info_for_common_edch_t;

#define UMTS_SIB_COMMON_E_DCH_RESOURCE_INFO_LIST_S_OFFSET_PRESENCE_FLAG                             0x01
#define UMTS_SIB_COMMON_E_DCH_RESOURCE_INFO_LIST_F_DPCH_CHANNELISATION_CODE_NUMBER_PRESENCE_FLAG    0x02
#define UMTS_SIB_COMMON_E_DCH_RESOURCE_INFO_LIST_E_RGCH_INFORMATION_COMMON_EDCH_PRESENCE_FLAG       0x04
typedef struct
{
    U16                 presence_bitmask;
    U8                  s_offset;
    U8                  f_dpch_channelisation_code_number;
    e_rgch_information_common_edch_t    e_rgch_information_common_edch;
    e_hich_information_common_edch_t    e_hich_information_common_edch;
    ul_dpch_code_info_for_common_edch_t ul_dpch_code_info_for_common_edch;
}common_e_dch_resource_info_list_t;

typedef struct
{
    U8                                  count;
    common_e_dch_resource_info_list_t   common_e_dch_resource_info_list[MAX_EDCHS];
}common_edch_system_info_fdd_common_e_dch_resource_info_list_t;

#define UMTS_SIB_COMMON_E_DCH_MAC_D_FLOW_MAC_D_FLOW_MULTIPLEXING_LIST_PRESENCE_FLAG             0x01
#define UMTS_SIB_COMMON_E_DCH_MAC_D_FLOW_E_DCH_MAC_D_FLOW_RETRANSMISSION_TIMER_PRESENCE_FLAG    0x02
typedef struct
{
    U16                 presence_bitmask;
    U8                  mac_d_flow_identity;
    U8                  mac_d_flow_power_offset;
    U8                  mac_d_flow_max_retrans;
    umts1_bit_str32_t   mac_d_flow_multiplexing_list;
    U32                 e_dch_mac_d_flow_retransmission_timer;
}common_e_dch_mac_d_flow_t;

typedef struct
{
    U8                          count;
    common_e_dch_mac_d_flow_t   common_e_dch_mac_d_flow[MAX_E_DCHMAC_D_FLOW];
}common_e_dch_mac_d_flow_list_t;

#define UMTS_SIB_COMMON_EDCH_SYSTEM_INFO_FDD_UL_INTERFERENCE_FOR_COMMON_EDCH_PRESENCE_FLAG  0x01
#define UMTS_SIB_COMMON_EDCH_SYSTEM_INFO_FDD_MEASUREMENT_FEEDBACK_INFO_PRESENCE_FLAG        0x02
typedef struct
{
    U16                     presence_bitmask;
    S8                      ul_interference_for_common_edch;
    common_e_dch_mac_d_flow_list_t  common_e_dch_mac_d_flow_list;
    prach_preamble_for_enhanced_uplink_t    prach_preamble_for_enhanced_uplink;
    U8                                      initial_serving_grant_value;
    U32                                     e_dch_tti;
    e_agch_information_t                    e_agch_information;
    U32                                     harq_info;
    ul_dpch_power_control_info_for_common_edch_t    ul_dpch_power_control_info_for_common_edch;
    e_dpcch_info_r7_t                               e_dpcch_info;
    e_dpdch_info_r8_t                               e_dpdch_info;
    U8                                              dl_fdpch_tpc_command_error_rate;
    U8                                              additional_e_dch_transmit_backoff;
    U32                                             max_ccch_resource_allocation;
    U8                                              max_period_for_collision_resolution;
    U32                                             e_dch_transmit_continuation_offset;
    U8                                              ack_nack_support_on_hs_dpcch;
    measurement_feedback_info_r7_t                  measurement_feedback_info;
    common_edch_system_info_fdd_common_e_dch_resource_info_list_t   common_e_dch_resource_info_list;
}common_edch_system_info_fdd_t;

#define UMTS_SIB_SYSINFOTYPE5_V8D0EXT_IES_COMMON_EDCH_SYSTEM_INFO_FDD_PRESENCE_FLAG 0x01
typedef struct
{
    U16                             presence_bitmask;
    common_edch_system_info_fdd_t   common_edch_system_info_fdd;
}sysinfotype5_v8d0ext_ies_t;

typedef struct
{
    U8          __dummy__;
}sysinfotype5_v4b0nces_v590nces_v650nces_v680nces_v690nces_v770nces_v860nces_v890nces_v8b0nces_v8d0nces_nces_t;

#define UMTS_SIB_SYSINFOTYPE5_V4B0NCES_V590NCES_V650NCES_V680NCES_V690NCES_V770NCES_V860NCES_V890NCES_V8B0NCES_V8D0NCES_NCES_PRESENCE_FLAG  0x01
typedef struct
{
    U16                             presence_bitmask;
    sysinfotype5_v8d0ext_ies_t      sysinfotype5_v8d0ext_ies;
    sysinfotype5_v4b0nces_v590nces_v650nces_v680nces_v690nces_v770nces_v860nces_v890nces_v8b0nces_v8d0nces_nces_t   v8d0nces_nces;
}sysinfotype5_v4b0nces_v590nces_v650nces_v680nces_v690nces_v770nces_v860nces_v890nces_v8b0nces_v8d0nces_t;

#define UMTS_SIB_SYSINFOTYPE5_V4B0NCES_V590NCES_V650NCES_V680NCES_V690NCES_V770NCES_V860NCES_V890NCES_V8B0NCES_V8D0NCES_PRESENCE_FLAG  0x01
typedef struct
{
    U16                                 presence_bitmask;
    sysinfotype5_v8b0ext_ies_t          sysinfotype5_v8b0ext_ies;
    sysinfotype5_v4b0nces_v590nces_v650nces_v680nces_v690nces_v770nces_v860nces_v890nces_v8b0nces_v8d0nces_t   v8d0nces;
}sysinfotype5_v4b0nces_v590nces_v650nces_v680nces_v690nces_v770nces_v860nces_v890nces_v8b0nces_t;

typedef struct
{
    U8          __dummy__;
}common_edch_system_info_mode_specific_info_dummy_t;

typedef struct
{
    U8                          count;
    asc_setting_tdd_lcr_r4_t    ascsetting_tdd_lcr_r4[MAX_ASC];
}e_rucch_access_service_class_t;

#define UMTS_SIB_SYNC_UL_INFO_FOR_E_RUCCH_PRX_UP_PCH_DES_PRESENCE_FLAG  0x01
#define UMTS_SIB_SYNC_UL_INFO_FOR_E_RUCCH_POWER_RAMP_STEP_PRESENCE_FLAG 0x02
#define UMTS_SIB_SYNC_UL_INFO_FOR_E_RUCCH_MAX_SYNC_UL_TRANSMISSIONS_PRESENCE_FLAG   0x04
typedef struct
{
    U16                     presence_bitmask;
    U8                      prx_up_pch_des;
    U8                      power_ramp_step;
    U32                     max_sync_ul_transmissions;
    sync_ul_codes_bitmap_t  sync_ul_codes_bitmap;
    U8                      mmax;
}sync_ul_info_for_e_rucch_t;

typedef struct
{
    U32             n;
    U32             elem[2];
}prach_chan_codes_list_lcr_t;

#define UMTS_SIB_PRACH_INFORMATION_LCR_FPACH_INFO_PRESENCE_FLAG     0x01
typedef struct
{
    U16                     presence_bitmask;
    U8                      timeslot;
    prach_chan_codes_list_lcr_t prach_chan_codes_list_lcr;
    midamble_shift_and_burst_type_lcr_r4_t  midamble_shift_and_burst_type;
    fpach_info_r4_t         fpach_info;
}prach_information_lcr_t;

typedef struct
{
    U8                      count;
    prach_information_lcr_t prach_information_lcr[MAX_PRACH_FPACH];
}prach_information_lcr_list_t;

#define UMTS_SIB_E_RUCCH_INFO_TDD128_T_RUCCH_PRESENCE_FLAG      0x01
#define UMTS_SIB_E_RUCCH_INFO_TDD128_N_RUCCH_PRESENCE_FLAG      0x02
#define UMTS_SIB_E_RUCCH_INFO_TDD128_T_WAIT_PRESENCE_FLAG       0x04
#define UMTS_SIB_E_RUCCH_INFO_TDD128_T_SI_PRESENCE_FLAG         0x08
#define UMTS_SIB_E_RUCCH_INFO_TDD128_EXTENDED_ESTIMATION_WINDOW_PRESENCE_FLAG   0x10
#define UMTS_SIB_E_RUCCH_INFO_TDD128_E_RUCCH_ACCESS_SERVICE_CLASS_PRESENCE_FLAG 0x20
#define UMTS_SIB_E_RUCCH_INFO_TDD128_PERSISTENCE_SCALING_FACTOR_LIST_PRESENCE_FLAG  0x40
#define UMTS_SIB_E_RUCCH_INFO_TDD128_SYNC_UL_INFO_FOR_E_RUCCH_PRESENCE_FLAG         0x80
#define UMTS_SIB_E_RUCCH_INFO_TDD128_PRACH_INFORMATION_LCR_LIST_PRESENCE_FLAG       0x100
typedef struct
{
    U16             presence_bitmask;
    U32             t_rucch;
    U8              n_rucch;
    U32             t_wait;
    U32             t_si;
    U8              extended_estimation_window;
    e_rucch_access_service_class_t      e_rucch_access_service_class;
    persistence_scaling_factor_list_t   persistence_scaling_factor_list;
    sync_ul_info_for_e_rucch_t          sync_ul_info_for_e_rucch;
    prach_information_lcr_list_t        prach_information_lcr_list;
}e_rucch_info_tdd128_t;

typedef struct
{
    U8      reference_code_rate;
    S8      reference_beta;
}reference_beta_qpsk_t;

typedef struct
{
    U8                      count;
    reference_beta_qpsk_t   reference_beta_qpsk[8];
}e_tfcs_info_reference_beta_qpsk_list_t;

typedef struct
{
    U8      reference_code_rate;
    S8      reference_beta;
}reference_beta_16qam_t;

typedef struct
{
    U8                      count;
    reference_beta_16qam_t  reference_beta_16qam[8];
}e_tfcs_info_reference_beta_16qam_list_t;

typedef struct
{
    e_tfcs_info_reference_beta_qpsk_list_t  info_reference_beta_qpsk_list;
    e_tfcs_info_reference_beta_16qam_list_t info_reference_beta_16qam_list;
}e_tfcs_info_t;

typedef struct
{
    U8          step_size;
    U8          frequency;
}ul_synchronisation_parameters_r4_t;

#define RRC_UMTS_E_PUCH_TS_SLOTS_LCR_MIDAMBLEALLOCATIONMODE_DEFAULTMIDAMBLE 1
#define RRC_UMTS_E_PUCH_TS_SLOTS_LCR_MIDAMBLEALLOCATIONMODE_UESPECIFICMIDAMBLE 2
typedef struct
{
    U32             t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        U8  ue_specific_midamble;
    }u;
}e_puch_ts_slots_lcr_midamble_allocation_mode_t;

typedef struct
{
    U8              timeslot_number;
    e_puch_ts_slots_lcr_midamble_allocation_mode_t  midamble_allocation_mode;
    U8              midamble_configuration;
}e_puch_ts_slots_lcr_t;

typedef struct
{
    U8                      count;
    e_puch_ts_slots_lcr_t   e_puch_ts_slots_lcr[MAX_NO_OF_TIMESLOTS_LCR_MINUS_1];
}e_puch_info_tdd128_e_puch_ts_configuration_list_t;

#define UMTS_SIB_E_PUCH_INFO_TDD128_SNPL_REPORT_TYPE_PRESENCE_FLAG                  0x01
#define UMTS_SIB_E_PUCH_INFO_TDD128_BEACON_PLE_ST_PRESENCE_FLAG                     0x02
#define UMTS_SIB_E_PUCH_INFO_TDD128_PEBASE_POWER_CONTROL_GAP_PRESENCE_FLAG          0x04
#define UMTS_SIB_E_PUCH_INFO_TDD128_UL_SYNCHRONISATION_PARAMETERS_R4_PRESENCE_FLAG  0x08
#define UMTS_SIB_E_PUCH_INFO_TDD128_DUMMY_PRESENCE_FLAG                             0x10
typedef struct
{
    U16             presence_bitmask;
    e_tfcs_info_t   e_tfcs_info;
    U32             snpl_report_type;
    S8              prx_base_des;
    U8              beacon_ple_st;
    U8              tpc_stepsize;
    U8              pebase_power_control_gap;
    ul_synchronisation_parameters_r4_t  ul_synchronisation_parameters_r4;
    e_puch_info_tdd128_e_puch_ts_configuration_list_t   e_puch_ts_configuration_list;
    U8              minimum_allowed_code_rate;
    U8              maximum_allowed_code_rate;
    U8              maximum_num_of_retrans_sched_info;
    U32             retrans_timer_for_sched_info;
    U8              dummy;
}e_puch_info_tdd128_t;

#define RRC_UMTS_E_HICH_INFORMATION_LCR_MIDAMBLEALLOCATIONMODE_DEFAULTMIDAMBLE 1
#define RRC_UMTS_E_HICH_INFORMATION_LCR_MIDAMBLEALLOCATIONMODE_UESPECIFICMIDAMBLE 2
typedef struct
{
    U32             t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        U8  ue_specific_midamble;
    }u;
}e_hich_information_lcr_midamble_allocation_mode_t;

typedef struct
{
    U8                  ei;
    U8                  timeslot_number;
    U32                 channelisation_code;
    e_hich_information_lcr_midamble_allocation_mode_t   midamble_allocation_mode;
    U8                  midamble_configuration;
}e_hich_information_lcr_t;

typedef struct
{
    U8                          count;
    e_hich_information_lcr_t    e_hich_information_lcr[MAX_NUM_E_HICH];
}e_hich_information_lcr_list_t;

typedef struct
{
    U8                              n_e_hich;
    e_hich_information_lcr_list_t   e_hich_information_lcr_list;
}e_hich_information_tdd128_t;

#define RRC_UMTS_E_AGCH_INDIVIDUAL_LCR_MIDAMBLEALLOCATIONMODE_DEFAULTMIDAMBLE 1
#define RRC_UMTS_E_AGCH_INDIVIDUAL_LCR_MIDAMBLEALLOCATIONMODE_COMMONMIDAMBLE 2
#define RRC_UMTS_E_AGCH_INDIVIDUAL_LCR_MIDAMBLEALLOCATIONMODE_UESPECIFICMIDAMBLE 3
typedef struct
{
    U32             t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        /* t = 3 */
        U8  ue_specific_midamble;
    }u;
}e_agch_individual_lcr_midamble_allocation_mode_t;

typedef struct
{
    U8                  timeslot_number;
    U32                 first_channelisation_code;
    U32                 second_channelisation_code;
    e_agch_individual_lcr_midamble_allocation_mode_t    midamble_allocation_mode;
    U8                  midamble_configuration;
}e_agch_individual_lcr_t;

typedef struct
{
    U8                          count;
    e_agch_individual_lcr_t     e_agch_individual_lcr[MAX_NUM_E_AGCH];
}e_agch_set_config_lcr_t;

typedef struct
{
    U8              rdi_indicator;
    U8              tpc_step_size;
    e_agch_set_config_lcr_t e_agch_set_config_lcr;
    S8              e_agch_bler_target;
}e_agch_information_tdd128_t;

typedef struct
{
    umts1_bit_str32_t   starting_e_rnti;
    U8                  number_of_group;
    U8                  number_of_enrti_per_group;
}common_e_rnti_info_element_t;

typedef struct
{
    U8                              count;
    common_e_rnti_info_element_t    common_e_rnti_info_element[MAX_ERUCCH];
}common_e_rnti_info_t;

typedef struct
{
    common_e_rnti_info_t    common_e_rnti_info;
    U8                      harq_maximum_number_of_retransmissions;
    U32                     harq_retransmission_timer;
    U8                      harq_power_offset;
}common_edch_system_info_mode_specific_info_tdd_tdd128_ccch_transmission_info_t;

typedef struct
{
    e_rucch_info_tdd128_t       e_rucch_info;
    e_puch_info_tdd128_t        e_puch_info;
    e_hich_information_tdd128_t e_hich_information;
    e_agch_information_tdd128_t e_agch_information;
    U32                         harq_info;
    common_edch_system_info_mode_specific_info_tdd_tdd128_ccch_transmission_info_t  ccch_transmission_info;
}common_edch_system_info_mode_specific_info_tdd_tdd128_t;

#define RRC_UMTS_COMMONEDCHSYSTEMINFO_MODESPECIFICINFO_TDD_TDD768 1
#define RRC_UMTS_COMMONEDCHSYSTEMINFO_MODESPECIFICINFO_TDD_TDD384 2
#define RRC_UMTS_COMMONEDCHSYSTEMINFO_MODESPECIFICINFO_TDD_TDD128 3
typedef struct
{
    U32             t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        /* t = 3 */
        common_edch_system_info_mode_specific_info_tdd_tdd128_t tdd128;
    }u;
}common_edch_system_info_mode_specific_info_tdd_t;

#define RRC_UMTS_COMMONEDCHSYSTEMINFO_MODESPECIFICINFO_DUMMY 1
#define RRC_UMTS_COMMONEDCHSYSTEMINFO_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32             t;
    union
    {
        common_edch_system_info_mode_specific_info_dummy_t  dummy;
        common_edch_system_info_mode_specific_info_tdd_t    tdd;
    }u;
}common_edch_system_info_mode_specific_info_t;

#define UMTS_SIB_COMMON_EDCH_SYSTEM_INFO_UL_INTERFERENCE_FOR_COMMON_EDCH_PRESENCE_FLAG  0x01
typedef struct
{
    U16                                             presence_bitmask;
    S8                                              ul_interference_for_common_edch;
    common_e_dch_mac_d_flow_list_t                  common_e_dch_mac_d_flow_list;
    common_edch_system_info_mode_specific_info_t    mode_specific_info;
}common_edch_system_info_t;

typedef struct
{
    U16             uarfcn_nt;
}frequency_info_tdd_t;

#define UMTS_SIB_SYSINFOTYPE5_V890EXT_IES_SECOND_FREQUENCY_INFO_PRESENCE_FLAG   0x01
#define UMTS_SIB_SYSINFOTYPE5_V890EXT_IES_COMMON_EDCH_SYSTEM_INFO_PRESENCE_FLAG 0x02
typedef struct
{
    U16                         presence_bitmask;
    frequency_info_tdd_t        second_frequency_info;
    common_edch_system_info_t   common_edch_system_info; 
}sysinfotype5_v890ext_ies_t;

#define UMTS_SIB_SYSINFOTYPE5_V4B0NCES_V590NCES_V650NCES_V680NCES_V690NCES_V770NCES_V860NCES_V890NCES_V8B0NONCRITICALEXTENSIONS_PRESENCE_FLAG  0x01
typedef struct
{
    U16                              presence_bitmask;
    sysinfotype5_v890ext_ies_t       sysinfotype5_v890ext;
    sysinfotype5_v4b0nces_v590nces_v650nces_v680nces_v690nces_v770nces_v860nces_v890nces_v8b0nces_t v8b0NonCriticalExtensions;
}sysinfotype5_v4b0nces_v590nces_v650nces_v680nces_v690nces_v770nces_v860nces_v890nces_t;

#define UMTS_SIB_TYPE_5_V4B0NCESS_V590NCES_V650NCES_V680NCES_V690NCES_V770NCES_V860NCES_890NCES_PRESENCE_FLAG   0x01
typedef struct
{
    U16                          presence_bitmask;
    sysinfotype5_v860ext_ies_t   sysInfoType5_v860ext;
    sysinfotype5_v4b0nces_v590nces_v650nces_v680nces_v690nces_v770nces_v860nces_v890nces_t v890nces;
}sysinfotype5_v4b0nces_v590nces_v650nces_v680nces_v690nces_v770nces_v860nces_t;

typedef struct
{
    U32          n;
    U8           elem[4];
}hs_scch_system_info_hs_scch_channelisation_code_info_t;

#define UMTS_SIB_HS_SCCH_SYSTEM_INFO_DL_SCRAMBLING_CODE_PRESENCE_FLAG   0x01
typedef struct
{
    U16         presence_bitmask;
    U8          dl_scrambling_code;
    hs_scch_system_info_hs_scch_channelisation_code_info_t   hs_scch_channelisation_code_info;
}hs_scch_system_info_t;

typedef struct
{
    U32      n;
    h_rnti_t elem[4];
}hs_dsch_common_system_information_common_h_rnti_information_t;

#define UMTS_SIB_HS_DSCH_COMMON_SYSTEM_INFORMATION_SRB1_MAPPING_INFO_PRESENCE_FLAG  0x01
typedef struct
{
    U16                          presence_bitmask;
    common_rb_mapping_info_t     ccch_mapping_info;
    common_rb_mapping_info_t     srb1_mapping_info;
    common_mac_ehs_reordering_queue_list_t   common_mac_ehs_reordering_queue_list;
    hs_scch_system_info_t        hs_scch_system_info;
    harq_info_t                  harq_system_info;
    hs_dsch_common_system_information_common_h_rnti_information_t    common_h_rnti_information;
    h_rnti_t                     bcch_specific_h_rnti;
}hs_dsch_common_system_information_t;

typedef struct
{
    pich_info_t      hsdpa_associated_pich_info;
    U8               hs_pdsch_channelisation_code;
}pich_for_hsdpa_supported_paging_t;

typedef struct
{
    U8                                  count;
    pich_for_hsdpa_supported_paging_t   pich_for_hsdpa_supported_paging[MAX_SCCPCH];
}hs_dsch_paging_system_information_pich_for_hsdpa_supported_paging_list_t;

typedef struct
{
    U32      n;
    U8       elem[2];
}hs_dsch_paging_system_information_transport_block_size_list_t;

#define UMTS_SIB_HS_DSCH_PAGING_SYSTEM_INFORMATION_DL_SCRAMBLING_CODE_PRESENCE_FLAG 0x01
typedef struct
{
    U16                      presence_bitmask;
    U8                       dl_scrambling_code;
    hs_dsch_paging_system_information_pich_for_hsdpa_supported_paging_list_t pich_for_hsdpa_supported_paging_list;
    U8                       number_of_pcch_transmissions;
    hs_dsch_paging_system_information_transport_block_size_list_t            transport_block_size_list;
}hs_dsch_paging_system_information_t;

#define UMTS_SIB_SYSINFOTYPE5_V770EXT_IES_MODE_SPECIFIC_INFO_FDD_HS_DSCH_PAGING_SYSTEM_INFORMATION_PRESENCE_FLAG    0x01
typedef struct
{
    U16      presence_bitmask;
    hs_dsch_common_system_information_t  hs_dsch_common_system_information;
    hs_dsch_paging_system_information_t  hs_dsch_paging_system_information;
}sysinfotype5_v770ext_ies_mode_specific_info_fdd_t;

typedef struct
{
    U32          n;
    U32          elem[2];
}ul_ts_channelisation_code_list_vhcr_t;

typedef struct
{
    U8       num_additional_timeslots;
}uplink_timeslots_codes_vhcr_more_timeslots_additional_timeslots_consecutive_t;

typedef struct 
{
    U8       timeslot_number;
}uplink_additional_timeslots_vhcr_parameters_same_as_last_t;

typedef struct
{
    U8       midamble_shift;
}midamble_shift_and_burst_type_vhcr_burst_type_type1_midamble_allocation_mode_ue_specific_midamble_t;

#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_VHCR_BURSTTYPE_TYPE1_MIDAMBLEALLOCATIONMODE_DEFAULTMIDAMBLE 1
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_VHCR_BURSTTYPE_TYPE1_MIDAMBLEALLOCATIONMODE_COMMONMIDAMBLE 2
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_VHCR_BURSTTYPE_TYPE1_MIDAMBLEALLOCATIONMODE_UESPECIFICMIDAMBLE 3
typedef struct
{
    U32      t;
    union 
    {
        /* t = 1 */
        /* t = 2 */
        /* t = 3 */
        midamble_shift_and_burst_type_vhcr_burst_type_type1_midamble_allocation_mode_ue_specific_midamble_t ue_specific_midamble;
    } u;
}midamble_shift_and_burst_type_vhcr_burst_type_type1_midamble_allocation_mode_t;

typedef struct
{
    U32      midamble_configuration_burst_type1_and_3;
    midamble_shift_and_burst_type_vhcr_burst_type_type1_midamble_allocation_mode_t   midamble_allocation_mode;
}midamble_shift_and_burst_type_vhcr_burst_type_type1_t;

typedef struct
{
    U8       midamble_shift;
}midamble_shift_and_burst_type_vhcr_burst_type_type2_midamble_allocation_mode_ue_specific_midamble_t;

#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_VHCR_BURSTTYPE_TYPE2_MIDAMBLEALLOCATIONMODE_DEFAULTMIDAMBLE 1
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_VHCR_BURSTTYPE_TYPE2_MIDAMBLEALLOCATIONMODE_COMMONMIDAMBLE 2
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_VHCR_BURSTTYPE_TYPE2_MIDAMBLEALLOCATIONMODE_UESPECIFICMIDAMBLE 3
typedef struct
{
    U32      t;
    union 
    {
        /* t = 1 */
        /* t = 2 */
        /* t = 3 */
        midamble_shift_and_burst_type_vhcr_burst_type_type2_midamble_allocation_mode_ue_specific_midamble_t ue_specific_midamble;
    } u;
}midamble_shift_and_burst_type_vhcr_burst_type_type2_midamble_allocation_mode_t;

typedef struct
{
    U32          midamble_configuration_burst_type2;
    midamble_shift_and_burst_type_vhcr_burst_type_type2_midamble_allocation_mode_t midamble_allocation_mode;
}midamble_shift_and_burst_type_vhcr_burst_type_type2_t;

typedef struct
{
    U8           midamble_shift;
}midamble_shift_and_burst_type_vhcr_burst_type_type3_midamble_allocation_mode_ue_specific_midamble_t;

#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_VHCR_BURSTTYPE_TYPE3_MIDAMBLEALLOCATIONMODE_DEFAULTMIDAMBLE 1
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_VHCR_BURSTTYPE_TYPE3_MIDAMBLEALLOCATIONMODE_UESPECIFICMIDAMBLE 2
typedef struct
{
    U32      t;
    union 
    {
        /* t = 1 */
        /* t = 2 */
        midamble_shift_and_burst_type_vhcr_burst_type_type3_midamble_allocation_mode_ue_specific_midamble_t   ue_specific_midamble;
    } u;
}midamble_shift_and_burst_type_vhcr_burst_type_type3_midamble_allocation_mode_t;

typedef struct
{
    U32          midamble_configuration_burst_type1_and_3;
    midamble_shift_and_burst_type_vhcr_burst_type_type3_midamble_allocation_mode_t   midamble_allocation_mode;
}midamble_shift_and_burst_type_vhcr_burst_type_type3_t;

#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_VHCR_BURSTTYPE_TYPE1 1
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_VHCR_BURSTTYPE_TYPE2 2
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_VHCR_BURSTTYPE_TYPE3 3
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_VHCR_BURSTTYPE_MBSFNBURSTTYPE4 4
typedef struct
{
    U32      t;
    union 
    {
        /* t = 1 */
        midamble_shift_and_burst_type_vhcr_burst_type_type1_t type1;
        /* t = 2 */
        midamble_shift_and_burst_type_vhcr_burst_type_type2_t type2;
        /* t = 3 */
        midamble_shift_and_burst_type_vhcr_burst_type_type3_t type3;
        /* t = 4 */
    } u;
}midamble_shift_and_burst_type_vhcr_burst_type_t;

typedef struct
{
    midamble_shift_and_burst_type_vhcr_burst_type_t  burst_type;
}midamble_shift_and_burst_type_vhcr_t;

typedef struct
{
    U8       timeslot_number;
    U8       tfci_existence;
    midamble_shift_and_burst_type_vhcr_t midamble_shift_and_burst_type_vhcr;
}individual_timeslot_info_vhcr_t;

typedef struct
{
    individual_timeslot_info_vhcr_t  individualTimeslotInfo;
    ul_ts_channelisation_code_list_t ul_TS_ChannelisationCodeList;
}uplink_additional_timeslots_vhcr_parameters_new_parameters_t;

#define RRC_UMTS_UPLINKADDITIONALTIMESLOTS_VHCR_PARAMETERS_SAMEASLAST 1
#define RRC_UMTS_UPLINKADDITIONALTIMESLOTS_VHCR_PARAMETERS_NEWPARAMETERS 2
typedef struct
{
    U32      t;
    union 
    {
        uplink_additional_timeslots_vhcr_parameters_same_as_last_t      same_as_last;
        uplink_additional_timeslots_vhcr_parameters_new_parameters_t    new_parameters;
    } u;
}uplink_additional_timeslots_vhcr_parameters_t;

typedef struct
{
    uplink_additional_timeslots_vhcr_parameters_t    parameters;
}uplink_additional_timeslots_vhcr_t;

typedef struct
{
    U8                                  count;
    uplink_additional_timeslots_vhcr_t  uplink_additional_timeslots_vhcr[MAX_NO_OF_TIMESLOTS_MINUS_1];
}uplink_timeslots_codes_vhcr_more_timeslots_additional_timeslots_timeslot_list_t;

#define RRC_UMTS_UPLINKTIMESLOTSCODES_VHCR_MORETIMESLOTS_ADDITIONALTIMESLOTS_CONSECUTIVE 1
#define RRC_UMTS_UPLINKTIMESLOTSCODES_VHCR_MORETIMESLOTS_ADDITIONALTIMESLOTS_TIMESLOTLIST 2
typedef struct
{
    U32      t;
    union 
    {
        uplink_timeslots_codes_vhcr_more_timeslots_additional_timeslots_consecutive_t     consecutive;
        uplink_timeslots_codes_vhcr_more_timeslots_additional_timeslots_timeslot_list_t   timeslot_list;
    }u;
}uplink_timeslots_codes_vhcr_more_timeslots_additional_timeslots_t;

#define RRC_UMTS_UPLINKTIMESLOTSCODES_VHCR_MORETIMESLOTS_NOMORE 1
#define RRC_UMTS_UPLINKTIMESLOTSCODES_VHCR_MORETIMESLOTS_ADDITIONALTIMESLOTS 2
typedef struct
{
    U32      t;
    union 
    {
        /* t = 1 */
        /* t = 2 */
        uplink_timeslots_codes_vhcr_more_timeslots_additional_timeslots_t   additional_timeslots;
    } u;
}uplink_timeslots_codes_vhcr_more_timeslots_t;

typedef struct
{
    U8                                       dynamic_s_fusage;
    individual_timeslot_info_vhcr_t		    first_individual_timeslot_info_vhcr;
    ul_ts_channelisation_code_list_vhcr_t	ul_ts_channelisation_code_list;
    uplink_timeslots_codes_vhcr_more_timeslots_t more_timeslots;
}uplink_timeslots_codes_vhcr_t;

#define UMTS_SIB_PUSCH_INFO_VHCR_COMMON_TIMESLOT_INFO_PRESENCE_FLAG         0x01
#define UMTS_SIB_PUSCH_INFO_VHCR_UPLINK_TIMESLOTS_CODES_VHCR_PRESENCE_FLAG  0x02
typedef struct
{
    U16                              presence_bitmask;
    U8                               tfcs_id;
    common_timeslot_info_t           common_timeslot_info_t;
    uplink_timeslots_codes_vhcr_t    uplink_timeslots_codes_vhcr;
}pusch_info_vhcr_t;

#define UMTS_SIB_PUSCH_SYS_INFO_VHCR_USCH_TRANSPORT_CHANNELS_INFO_PRESENCE_FLAG 0x01
#define UMTS_SIB_PUSCH_SYS_INFO_VHCR_USCH_TFCS_PRESENCE_FLAG                    0x02
typedef struct
{
    U16                 presence_bitmask;
    U8                  pusch_identity;
    pusch_info_vhcr_t   pusch_info_vhcr;
    usch_transport_channels_info_t  usch_transport_channels_info;
    tfcs_t              usch_tfcs;
}pusch_sys_info_vhcr_t;

#define UMTS_SIB_PUSCH_SYS_INFO_LIST_SFN_VHCR_ELEMENT_SFN_TIME_INFO_PRESENCE_FLAG   0x01
typedef struct
{
    U16                      presence_bitmask;
    pusch_sys_info_vhcr_t    pusch_sys_info_vhcr;
    sfn_time_info_t          sfn_time_info;
}pusch_sys_info_list_sfn_vhcr_element_t;

typedef struct
{
    U8                                      count;
    pusch_sys_info_list_sfn_vhcr_element_t  pusch_sys_info_list_sfn_vhcr_element[MAX_PUSCH];
}pusch_sys_info_list_sfn_vhcr_t;

#define UMTS_SIB_PDSCH_INFO_R7_TDD_OPTION_TDD384_PDSCH_TIMESLOTS_CODES_PRESENCE_FLAG   0x01
typedef struct
{
    U16                          presence_bitmask;
    downlink_timeslots_codes_t   pdsch_timeslots_codes;
}pdsch_info_r7_tdd_option_tdd384_t;

typedef struct
{
    U32      first_channelisation_code;
    U32      last_channelisation_code;
}dl_ts_channelisation_codes_short_vhcr_codes_representation_consecutive_t;

typedef struct
{
    U32  numbits;
    U8   data[4];
}dl_ts_channelisation_codes_short_vhcr_codes_representation_bitmap_t;

#define RRC_UMTS_DL_TS_CHANNELISATIONCODESSHORT_VHCR_CODESREPRESENTATION_CONSECUTIVE 1
#define RRC_UMTS_DL_TS_CHANNELISATIONCODESSHORT_VHCR_CODESREPRESENTATION_BITMAP 2
typedef struct
{
    U32      t;
    union 
    {
        /* t = 1 */
        dl_ts_channelisation_codes_short_vhcr_codes_representation_consecutive_t  consecutive;
        /* t = 2 */
        dl_ts_channelisation_codes_short_vhcr_codes_representation_bitmap_t       bitmap;
    } u;
}dl_ts_channelisation_codes_short_vhcr_codes_representation_t;

typedef struct
{
    dl_ts_channelisation_codes_short_vhcr_codes_representation_t  codes_representation;
}dl_ts_channelisation_codes_short_vhcr_t;

typedef struct
{
    U8          timeslot_number;
}downlink_additional_timeslots_vhcr_parameters_same_as_last_t;

typedef struct
{
    individual_timeslot_info_vhcr_t     first_individual_timeslot_info;
    dl_ts_channelisation_codes_short_vhcr_t dl_ts_channelisation_codes_short;
}downlink_additional_timeslots_vhcr_parameters_new_parameters_t;

#define RRC_UMTS_DOWNLINKADDITIONALTIMESLOTS_VHCR_PARAMETERS_SAMEASLAST 1
#define RRC_UMTS_DOWNLINKADDITIONALTIMESLOTS_VHCR_PARAMETERS_NEWPARAMETERS 2
typedef struct
{
    U32         t;
    union
    {
        /* t = 1 */
        downlink_additional_timeslots_vhcr_parameters_same_as_last_t    same_as_last;
        /* t = 2 */
        downlink_additional_timeslots_vhcr_parameters_new_parameters_t  new_parameters;
    }u;
}downlink_additional_timeslots_vhcr_parameters_t;

typedef struct
{
    downlink_additional_timeslots_vhcr_parameters_t parameters;
}downlink_additional_timeslots_vhcr_t;

typedef struct
{
    U8                                      count;
    downlink_additional_timeslots_vhcr_t    downlink_additional_timeslots_vhcr[MAX_NO_OF_TIMESLOTS_MINUS_1];
}downlink_timeslots_codes_vhcr_more_timeslots_additional_timeslots_timeslot_list_t;

#define RRC_UMTS_DOWNLINKTIMESLOTSCODES_VHCR_MORETIMESLOTS_ADDITIONALTIMESLOTS_CONSECUTIVE 1
#define RRC_UMTS_DOWNLINKTIMESLOTSCODES_VHCR_MORETIMESLOTS_ADDITIONALTIMESLOTS_TIMESLOTLIST 2
typedef struct
{
    U32      t;
    union 
    {
        /* t = 1 */
        U8 consecutive;
        /* t = 2 */
        downlink_timeslots_codes_vhcr_more_timeslots_additional_timeslots_timeslot_list_t timeslot_list;
    } u;
}downlink_timeslots_codes_vhcr_more_timeslots_additional_timeslots_t;

#define RRC_UMTS_DOWNLINKTIMESLOTSCODES_VHCR_MORETIMESLOTS_NOMORE 1
#define RRC_UMTS_DOWNLINKTIMESLOTSCODES_VHCR_MORETIMESLOTS_ADDITIONALTIMESLOTS 2
typedef struct
{
    U32  t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        downlink_timeslots_codes_vhcr_more_timeslots_additional_timeslots_t   additional_timeslots;
    } u;
}downlink_timeslots_codes_vhcr_more_timeslots_t;

typedef struct
{
    individual_timeslot_info_vhcr_t          first_individual_timeslot_info;
    dl_ts_channelisation_codes_short_vhcr_t  dl_ts_channelisation_codes_short;
    downlink_timeslots_codes_vhcr_more_timeslots_t   more_timeslots;
}downlink_timeslots_codes_vhcr_t;


#define UMTS_SIB_PDSCH_INFO_R7_TDD_OPTION_TDD768_PDSCH_TIMESLOTS_CODES_PRESENCE_FLAG	0x01
typedef struct 
{
    U16                             presence_bitmask;
    downlink_timeslots_codes_vhcr_t pdsch_timeslots_codes;
}pdsch_info_r7_tdd_option_tdd768_t;

#define UMTS_SIB_PDSCH_INFO_R7_TDD_OPTION_TDD128_PDSCH_TIMESLOTS_CODES_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                  presence_bitmask;
    downlink_timeslots_codes_lcr_r4_t    pdsch_timeslots_codes;
}pdsch_info_r7_tdd_option_tdd128_t;

#define RRC_UMTS_PDSCH_INFO_R7_TDDOPTION_TDD384 1
#define RRC_UMTS_PDSCH_INFO_R7_TDDOPTION_TDD768 2
#define RRC_UMTS_PDSCH_INFO_R7_TDDOPTION_TDD128 3
typedef struct
{
    U32      t;
    union 
    {
        pdsch_info_r7_tdd_option_tdd384_t     tdd384;
        pdsch_info_r7_tdd_option_tdd768_t     tdd768;
        pdsch_info_r7_tdd_option_tdd128_t     tdd128;
    } u;
}pdsch_info_r7_tdd_option_t;

#define UMTS_SIB_PDSCH_INFO_R7_COMMON_TIMESLOT_INFO_PRESENCE_FLAG   0x01
typedef struct
{
    U16                      presence_bitmask;
    U8                       tfcs_id;
    common_timeslot_info_t   common_timeslot_info;
    pdsch_info_r7_tdd_option_t   tdd_option;
}pdsch_info_r7_t;

#define UMTS_SIB_PDSCH_SYS_INFO_VHCR_R7_PDSCH_SYS_INFO_VHCR_R7_T_PRESENCE_FLAG  0x01
#define UMTS_SID_PDSCH_SYS_INFO_VHCR_R7_DSCH_TFCS_PRESENCE_FLAG           		0x02
typedef struct
{
    U16                              presence_bitmask;
    U8                               pdsch_identity;
    pdsch_info_r7_t                  pdsch_info;
    dsch_transport_channels_info_t   dsch_transport_channels_info;
    tfcs_t                           dsch_tfcs;
}pdsch_sys_info_vhcr_r7_t;

typedef struct
{
    U8                          count;
    pdsch_sys_info_vhcr_r7_t    pdsch_sys_info_vhcr_r7[MAX_PDSCH];
}pdsch_sys_info_list_vhcr_r7_t;

typedef struct 
{
    U32      n;
    U32      elem[16];
}tdd768_prach_c_code_list_sf32_t;

typedef struct
{
    U32      n;
    U32      elem[16];
}tdd768_prach_c_code_list_sf16_t;

#define RRC_UMTS_TDD768_PRACH_CCODELIST_SF32 1
#define RRC_UMTS_TDD768_PRACH_CCODELIST_SF16 2
typedef struct
{
    U32      t;
    union 
    {
        tdd768_prach_c_code_list_sf32_t   sf32;
        tdd768_prach_c_code_list_sf16_t   sf16;
    } u;
}tdd768_prach_c_code_list_t;

typedef struct
{
    U8                           timeslot;
    tdd768_prach_c_code_list_t   channelisation_code_list;
    U32                      	prach_midamble;
}prach_rach_info_vhcr_r7_t;

#define UMTS_SIB_PRACH_SYSTEM_INFORMATION_VHCR_R7_PRACH_PARTITIONING_PRESENCE_FLAG  0x01
typedef struct
{
    U16                          presence_bitmask;
    prach_rach_info_vhcr_r7_t    prach_rach_info;
    prach_partitioning_r7_t      prach_partitioning;
}prach_system_information_vhcr_r7_t;

typedef struct
{
    U8                                  count;
    prach_system_information_vhcr_r7_t  prach_system_information_vhcr_r7[MAX_PRACH];
}prach_system_information_list_vhcr_r7_t;

#define UMTS_SIB_SYSINFOTYPE5_V770EXT_IES_MODESPECIFICINFO_TDD768_PUSCH_SYSINFOLIST_SFN_PRESENCE_FLAG           0x01
#define UMTS_SIB_SYSINFOTYPE5_V770EXT_IES_MODESPECIFICINFO_TDD768_PDSCH_SYSINFOLIST_SFN_PRESENCE_FLAG		    0x02
#define UMTS_SIB_SYSINFOTYPE5_V770EXT_IES_MODESPECIFICINFO_TDD768_PRACH_SYSTEMINFORMATIONLIST_PRESENCE_FLAG	    0x04
typedef struct
{
    U16                              presence_bitmask;
    pusch_sys_info_list_sfn_vhcr_t   pusch_sys_info_list_sfn;
    pdsch_sys_info_list_vhcr_r7_t    pdsch_sys_info_list_sfn;
    prach_system_information_list_vhcr_r7_t  prach_system_information_list;
}sysinfotype5_v770ext_ies_mode_specific_info_tdd768_t;

typedef struct 
{
    sync_ul_codes_bitmap_t   e_rucch_sync_ul_codes_bitmap;
}prach_rach_info_lcr_v770ext_t;

typedef struct
{
    prach_rach_info_lcr_v770ext_t    prach_rach_info_lcr;
}prach_system_information_lcr_v770ext_t;

typedef struct
{
    U8                                      count;
    prach_system_information_lcr_v770ext_t  prach_system_information_lcr_v770ext[MAX_PRACH];
}prach_system_information_list_lcr_v770ext_t;

#define UMTS_SIB_SYSINFOTYPE5_V770EXT_IES_MODE_SPECIFIC_INFO_TDD128_PRACH_SYSTEM_INFORMATION_LIST_PRESENCE_FLAG 0x01
typedef struct
{
    U16                                          presence_bitmask;
    prach_system_information_list_lcr_v770ext_t  prach_system_information_list;
}sysinfotype5_v770ext_ies_mode_specific_info_tdd128_t;

#define RRC_UMTS_SYSINFOTYPE5_V770EXT_IES_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_SYSINFOTYPE5_V770EXT_IES_MODESPECIFICINFO_TDD768 2
#define RRC_UMTS_SYSINFOTYPE5_V770EXT_IEs_MODESPECIFICINFO_TDD128 3
typedef struct
{
    U32          t;
    union 
    {
        /* t = 1 */
        sysinfotype5_v770ext_ies_mode_specific_info_fdd_t     fdd;
        /* t = 2 */
        sysinfotype5_v770ext_ies_mode_specific_info_tdd768_t  tdd768;
        /* t = 3 */
        sysinfotype5_v770ext_ies_mode_specific_info_tdd128_t  tdd128;
    } u;
}sysinfotype5_v770ext_ies_mode_specific_info_t;

#define RRC_UMTS_SECONDARYCCPCHINFO_MBMS_R7_MODESPECIFICINFO_FDD_MODULATION_MODQPSK 1
#define RRC_UMTS_SECONDARYCCPCHINFO_MBMS_R7_MODESPECIFICINFO_FDD_MODULATION_MOD16QAM 2
typedef struct
{
    U32          t;
    union 
    {
        /* t = 1 */
        /* t = 2 */
        S8    mod_16_qam;
    } u;
}secondary_ccpch_info_mbms_r7_mode_specific_info_fdd_modulation_t;

#define UMTS_SIB_SECONDARY_CCPCH_INFO_MBMS_R7_MODE_SPECIFIC_INFO_FDD_SECONDARY_SCRAMBLING_CODE_PRESENCE_FLAG    0x01
#define UMTS_SIB_SECONDARY_CCPCH_INFO_MBMS_R7_MODE_SPECIFIC_INFO_FDD_MODULATION_PRESENCE_FLAG                   0x02
typedef struct
{
    U16                          presence_bitmask;
    U8                           secondary_scrambling_code;
    U8                           sttd_indicator;
    sf256_and_code_number_t      sf_and_code_number;
    U8                           timing_offset;
    secondary_ccpch_info_mbms_r7_mode_specific_info_fdd_modulation_t modulation;
}secondary_ccpch_info_mbms_r7_mode_specific_info_fdd_t;

typedef struct
{
    U8              timeslot_number;
}downlink_additional_timeslots_r7_parameters_same_as_last_t;

typedef struct
{
    U8              midamble_shift;
}midamble_shift_and_burst_type_r7_burst_type_type1_midamble_allocation_mode_ue_specific_midamble_t;

#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_R7_BURSTTYPE_TYPE1_MIDAMBLEALLOCATIONMODE_DEFAULTMIDAMBLE 1
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_R7_BURSTTYPE_TYPE1_MIDAMBLEALLOCATIONMODE_COMMONMIDAMBLE 2
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_R7_BURSTTYPE_TYPE1_MIDAMBLEALLOCATIONMODE_UESPECIFICMIDAMBLE 3
typedef struct
{
    U32             t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        /* t = 3 */
        midamble_shift_and_burst_type_r7_burst_type_type1_midamble_allocation_mode_ue_specific_midamble_t   ue_specific_midamble;
    }u;
}midamble_shift_and_burst_type_r7_burst_type_type1_midamble_allocation_mode_t;

typedef struct
{
    U32                 midamble_configuration_burst_type1_and_3;
    midamble_shift_and_burst_type_r7_burst_type_type1_midamble_allocation_mode_t    midamble_allocation_mode;
}midamble_shift_and_burst_type_r7_burst_type_type1_t;

typedef struct
{
    U8              midamble_shift;
}midamble_shift_and_burst_type_r7_burst_type_type2_midamble_allocation_mode_ue_specific_midamble_t;

#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_R7_BURSTTYPE_TYPE2_MIDAMBLEALLOCATIONMODE_DEFAULTMIDAMBLE 1
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_R7_BURSTTYPE_TYPE2_MIDAMBLEALLOCATIONMODE_COMMONMIDAMBLE 2
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_R7_BURSTTYPE_TYPE2_MIDAMBLEALLOCATIONMODE_UESPECIFICMIDAMBLE 3
typedef struct
{
    U32             t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        /* t = 3 */
        midamble_shift_and_burst_type_r7_burst_type_type2_midamble_allocation_mode_ue_specific_midamble_t   ue_specific_midamble;
    }u;
}midamble_shift_and_burst_type_r7_burst_type_type2_midamble_allocation_mode_t;

typedef struct
{
    U32                 midamble_configuration_burst_type2;
    midamble_shift_and_burst_type_r7_burst_type_type2_midamble_allocation_mode_t    midamble_allocation_mode;
}midamble_shift_and_burst_type_r7_burst_type_type2_t;

typedef struct
{
    U8              midamble_shift;
}midamble_shift_and_burst_type_r7_burst_type_type3_midamble_allocation_mode_ue_specific_midamble_t;

#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_R7_BURSTTYPE_TYPE3_MIDAMBLEALLOCATIONMODE_DEFAULTMIDAMBLE 1
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_R7_BURSTTYPE_TYPE3_MIDAMBLEALLOCATIONMODE_UESPECIFICMIDAMBLE 2
typedef struct
{
    U32             t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        midamble_shift_and_burst_type_r7_burst_type_type3_midamble_allocation_mode_ue_specific_midamble_t   ue_specific_midamble;
    }u;
}midamble_shift_and_burst_type_r7_burst_type_type3_midamble_allocation_mode_t;

typedef struct
{
    U32                 midamble_configuration_burst_type1_and_3;
    midamble_shift_and_burst_type_r7_burst_type_type3_midamble_allocation_mode_t    midamble_allocation_mode;
}midamble_shift_and_burst_type_r7_burst_type_type3_t;

#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_R7_BURSTTYPE_TYPE1 1
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_R7_BURSTTYPE_TYPE2 2
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_R7_BURSTTYPE_TYPE3 3
#define RRC_UMTS_MIDAMBLESHIFTANDBURSTTYPE_R7_BURSTTYPE_MBSFNBURSTTYPE4 4
typedef struct
{
    U32             t;
    union
    {
        /* t = 1 */
        midamble_shift_and_burst_type_r7_burst_type_type1_t     type1;
        /* t = 2 */
        midamble_shift_and_burst_type_r7_burst_type_type2_t     type2;
        /* t = 3 */
        midamble_shift_and_burst_type_r7_burst_type_type3_t     type3;
        /* t = 4 */
    }u;
}midamble_shift_and_burst_type_r7_burst_type_t;

typedef struct
{
    midamble_shift_and_burst_type_r7_burst_type_t   burst_type;
}midamble_shift_and_burst_type_r7_t;

typedef struct
{
    U8                  timeslot_number;
    U8                  tfci_existence;
    midamble_shift_and_burst_type_r7_t  midamble_shift_and_burst_type_r7;
}individual_timeslot_info_r7_t;

typedef struct
{
    individual_timeslot_info_r7_t       individual_timeslot_info;
    dl_ts_channelisation_codes_short_t  dl_ts_channelisation_codes_short;
}downlink_additional_timeslots_r7_parameters_new_parameters_t;

#define RRC_UMTS_DOWNLINKADDITIONALTIMESLOTS_R7_PARAMETERS_SAMEASLAST 1
#define RRC_UMTS_DOWNLINKADDITIONALTIMESLOTS_R7_PARAMETERS_NEWPARAMETERS 2
typedef struct
{
    U32             t;
    union
    {
        downlink_additional_timeslots_r7_parameters_same_as_last_t      same_as_last;
        downlink_additional_timeslots_r7_parameters_new_parameters_t    new_parameters;
    }u;
}downlink_additional_timeslots_r7_parameters_t;

typedef struct
{
    downlink_additional_timeslots_r7_parameters_t   parameters;
}downlink_additional_timeslots_r7_t;

typedef struct
{
    U8                                  count;
    downlink_additional_timeslots_r7_t  downlink_additional_timeslots_r7[MAX_NO_OF_TIMESLOTS_MINUS_1];
}downlink_timeslots_codes_r7_more_timeslots_additional_timeslots_timeslot_list_t;

#define RRC_UMTS_DOWNLINKTIMESLOTSCODES_R7_MORETIMESLOTS_ADDITIONALTIMESLOTS_CONSECUTIVE 1
#define RRC_UMTS_DOWNLINKTIMESLOTSCODES_R7_MORETIMESLOTS_ADDITIONALTIMESLOTS_TIMESLOTLIST 2
typedef struct
{
    U32             t;
    union
    {
        U8              consecutive;
        downlink_timeslots_codes_r7_more_timeslots_additional_timeslots_timeslot_list_t timeslot_list;
    }u;
}downlink_timeslots_codes_r7_more_timeslots_additional_timeslots_t;

#define RRC_UMTS_DOWNLINKTIMESLOTSCODES_R7_MORETIMESLOTS_NOMORE 1
#define RRC_UMTS_DOWNLINKTIMESLOTSCODES_R7_MORETIMESLOTS_ADDITIONALTIMESLOTS 2
typedef struct
{
    U32             t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        downlink_timeslots_codes_r7_more_timeslots_additional_timeslots_t   additional_timeslots;
    }u;
}downlink_timeslots_codes_r7_more_timeslots_t;

typedef struct
{
    individual_timeslot_info_r7_t       first_individual_timeslot_info;
    dl_ts_channelisation_codes_short_t  dl_ts_channelisation_codes_short;
    downlink_timeslots_codes_r7_more_timeslots_t    more_timeslots;
}downlink_timeslots_codes_r7_t;

typedef struct
{
    common_timeslot_info_mbms_t      first_individual_timeslot_info;
    downlink_timeslots_codes_r7_t    downlink_timeslots_codes;
    U32                              modulation;
}secondary_ccpch_info_mbms_r7_mode_specific_info_tdd384_t;

typedef struct
{
    common_timeslot_info_mbms_t      common_timeslot_info_mbms;
    downlink_timeslots_codes_vhcr_t  downlink_timeslots_codes;
    U32                              modulation;
}secondary_ccpch_info_mbms_r7_mode_specific_info_tdd768_t;

#define UMTS_SIB_SECONDARY_CCPCH_INFO_MBMS_R7_MODE_SPECIFIC_INFO_TDD128_MBSFN_SPECIAL_TIME_SLOT_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                  presence_bitmaskl;
    common_timeslot_info_mbms_t          common_timeslot_info_mbms;
    downlink_timeslots_codes_lcr_r4_t    downlink_timeslots_codes;
    U32                                  mbsfn_special_time_slot;
    U32                                  modulation;
}secondary_ccpch_info_mbms_r7_mode_specific_info_tdd128_t;

#define RRC_UMTS_SECONDARYCCPCHINFO_MBMS_R7_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_SECONDARYCCPCHINFO_MBMS_R7_MODESPECIFICINFO_TDD384 2
#define RRC_UMTS_SECONDARYCCPCHINFO_MBMS_R7_MODESPECIFICINFO_TDD768 3
#define RRC_UMTS_SECONDARYCCPCHINFO_MBMS_R7_MODESPECIFICINFO_TDD128 4
typedef struct
{
    U32      t;
    union 
    {
        /* t = 1 */
        secondary_ccpch_info_mbms_r7_mode_specific_info_fdd_t     fdd;
        /* t = 2 */
        secondary_ccpch_info_mbms_r7_mode_specific_info_tdd384_t  tdd384;
        /* t = 3 */
        secondary_ccpch_info_mbms_r7_mode_specific_info_tdd768_t  tdd768;
        /* t = 4 */
        secondary_ccpch_info_mbms_r7_mode_specific_info_tdd128_t  tdd128;
    } u;
}secondary_ccpch_info_mbms_r7_mode_specific_info_t;

typedef struct
{
    secondary_ccpch_info_mbms_r7_mode_specific_info_t    mode_specific_info;
}secondary_ccpch_info_mbms_r7_t;

typedef struct
{
    transport_format_set_t               mcch_transport_format_set;
    mbms_mcch_configuration_info_r6_t    mcch_configuration_info;
}sccpch_system_information_mbms_r7_fach_carrying_mcch_t;

typedef struct
{
    transport_format_set_t  transport_format_set;
}sccpch_system_information_mbms_r7_fach_carrying_msch_t;

#define UMTS_SIB_SCCPCH_SYSTEM_INFORMATION_MBMS_R7_MBMS_FACH_CARRYING_MTCH_LIST_PRESENCE_FLAG   0x01
#define UMTS_SIB_SCCPCH_SYSTEM_INFORMATION_MBMS_R7_FACH_CARRYING_MSCH_PRESENCE_FLAG             0x02
typedef struct
{
    U16                              presence_bitmask;
    secondary_ccpch_info_mbms_r7_t   secondary_ccpch_info_mbms;
    tfcs_t                           transport_format_combination_set;
    sccpch_system_information_mbms_r7_fach_carrying_mcch_t   fach_carrying_mcch;
    mbms_fach_carrying_mtch_list_t   mbms_fach_carrying_mtch_list;
    sccpch_system_information_mbms_r7_fach_carrying_msch_t   fach_carrying_msch;
}sccpch_system_information_mbms_r7_t;

#define RRC_UMTS_SYSINFOTYPE5_V770EXT_IES_SCCPCH_SYSTEMINFORMATION_MBMS_MCCHONSCCPCHUSEDONLYFORMBMS 1
typedef struct
{
    U32      t;
    union 
    {
        /* t = 1 */
        sccpch_system_information_mbms_r7_t   mcch_on_sccpch_used_only_for_mbms;
    } u;
}sysinfotype5_v770ext_ies_sccpch_system_information_mbms_t;

typedef struct
{
    individual_timeslot_info_r7_t       individual_timeslot_info;
    sccpch_channelisation_code_list_t   sccpch_channelisation_code_list;
}secondary_ccpch_info_hcr_vhcr_r7_mode_specific_info_tdd384_t;

typedef struct
{
    U32             n;
    U32             elem[32];
}sccpch_channelisation_code_list_vhcr_t;

typedef struct
{
    individual_timeslot_info_vhcr_t         individual_timeslot_info;
    sccpch_channelisation_code_list_vhcr_t  channelisation_code;
}secondary_ccpch_info_hcr_vhcr_r7_mode_specific_info_tdd768_t;

#define RRC_UMTS_SECONDARYCCPCH_INFO_HCR_VHCR_R7_MODESPECIFICINFO_TDD384 1
#define RRC_UMTS_SECONDARYCCPCH_INFO_HCR_VHCR_R7_MODESPECIFICINFO_TDD768 2
typedef struct
{
    U32             t;
    union
    {
        secondary_ccpch_info_hcr_vhcr_r7_mode_specific_info_tdd384_t    tdd384;
        secondary_ccpch_info_hcr_vhcr_r7_mode_specific_info_tdd768_t    tdd768;
    }u;
}secondary_ccpch_info_hcr_vhcr_r7_mode_specific_info_t;

typedef struct
{
    secondary_ccpch_info_hcr_vhcr_r7_mode_specific_info_t   mode_specific_info;
}secondary_ccpch_info_hcr_vhcr_r7_t;

#define RRC_UMTS_PICH_INFO_HCR_VHCR_R7_CHANNELISATIONCODE_TDD384 1
#define RRC_UMTS_PICH_INFO_HCR_VHCR_R7_CHANNELISATIONCODE_TDD768 2
typedef struct
{
    U32             t;
    union
    {
        U32         tdd384;
        U32         tdd768;
    }u;
}pich_info_hcr_vhcr_r7_channelisation_code_t;

#define UMTS_SIB_PICH_INFO_HCR_VHCR_R7_CHANNELISATION_CODE_PRESENCE_FLAG        0x01
#define UMTS_SIB_PICH_INFO_HCR_VHCR_R7_TIMESLOT_PRESENCE_FLAG                   0x02
#define UMTS_SIB_PICH_INFO_HCR_VHCR_R7_REP_PER_LENGTH_OFFSET_PICH_PRESENCE_FLAG 0x04
typedef struct
{
    U16                                 presence_bitmask;
    pich_info_hcr_vhcr_r7_channelisation_code_t     channelisation_code;
    U8                                  timeslot;
    midamble_shift_and_burst_type_t     midamble_shift_and_burst_type;
    rep_per_length_offset_pich_t        rep_per_length_offset_pich;
    U32                                 paging_indicator_length;
    U32                                 n_gap;
    U8                                  n_pch;
}pich_info_hcr_vhcr_r7_t;

#define UMTS_SIB_SCCPCH_SYSTEM_INFORMATION_HCR_VHCR_R7_TFCS_PRESENCE_FLAG                       0x01
#define UMTS_SIB_SCCPCH_SYSTEM_INFORMATION_HCR_VHCR_R7_FACH_PCH_INFORMATION_LIST_PRESENCE_FLAG  0x02
#define UMTS_SIB_SCCPCH_SYSTEM_INFORMATION_HCR_VHCR_R7_PICH_INFO_PRESENCE_FLAG                  0x04
typedef struct
{
    U16                                 presence_bitmask;
    secondary_ccpch_info_hcr_vhcr_r7_t  secondary_ccpch_info;
    tfcs_t                              tfcs;
    fach_pch_information_list_t         fach_pch_information_list;
    pich_info_hcr_vhcr_r7_t             pich_info;
}sccpch_system_information_hcr_vhcr_r7_t;

typedef struct
{
    U8                                          count;
    sccpch_system_information_hcr_vhcr_r7_t     sccpch_system_information_hcr_vhcr_r7[MAX_SCCPCH];
}sccpch_system_information_list_hcr_vhcr_r7_t;

typedef struct
{
    U8          time_slot_number;
    U8          cell_parameters_id;
}tdd_mbsfnt_slot_info_t;

typedef struct
{
    U8                      count;
    tdd_mbsfnt_slot_info_t  tdd_mbsfnt_slot_info[MAX_NO_OF_TIMESLOTS];
}tdd_mbsfn_information_t;

#define UMTS_SIB_SYSINFOTYPE5_V770EXT_IES_MODE_SPECIFIC_INFO_PRESENCE_FLAG                   0x01
#define UMTS_SIB_SYSINFOTYPE5_V770EXT_IES_SCCPCH_SYSTEM_INFORMATION_LIST_PRESENCE_FLAG       0x02
#define UMTS_SIB_SYSINFOTYPE5_V770EXT_IES_SCCPCH_SYSTEM_INFORMATION_MBMS_PRESENCE_FLAG       0x04
#define UMTS_SIB_SYSINFOTYPE5_V770EXT_IES_TDD_MBSFN_INFORMATION_PRESENCE_FLAG                0x08
typedef struct
{
    U16                                              presence_bitmask;
    sysinfotype5_v770ext_ies_mode_specific_info_t    mode_specific_info;
    sccpch_system_information_list_hcr_vhcr_r7_t     sccpch_system_information_list;
    sysinfotype5_v770ext_ies_sccpch_system_information_mbms_t    sccpch_system_information_mbms;
    tdd_mbsfn_information_t                          tdd_mbsfn_information;
}sysinfotype5_v770ext_ies_t;

#define UMTS_SIB_TYPE_5_V4B0NCESS_V590NCES_V650NCES_V680NCES_V690NCES_V770NCES_V860NONCRITICALEXTENSIONS_PRESENCE_FLAG  0x01
typedef struct
{
    U16                          presence_bitmask;
    sysinfotype5_v770ext_ies_t   sysInfoType5_v770ext;
    sysinfotype5_v4b0nces_v590nces_v650nces_v680nces_v690nces_v770nces_v860nces_t v860NonCriticalExtensions;
}sysinfotype5_v4b0ncess_v590nces_v650nces_v680nces_v690nces_v770nces_t;

#define  UMTS_SIB_TYPE_5_V4B0NCES_V590NCES_V650NCES_V680NCES_V690NCES_V770NCES_PRESENCE_FLAG   0x01
typedef struct
{
    U16                          presence_bitmask;
    sysinfotype5_v690ext_ies_t   sysInfoType5_v690ext;
    sysinfotype5_v4b0ncess_v590nces_v650nces_v680nces_v690nces_v770nces_t    v770NonCriticalExtensions;
}sysinfotype5_v4b0nces_v590nces_v650nces_v680nces_v690nces_t;

#define UMTS_SIB_TYPE_5_V4B0NCES_V590NCES_V650NCES_V680NCES_V680EXT_PRESENCE_FLAG  0x01
#define UMTS_SIB_TYPE_5_V4B0NCES_V590NCES_V650NCES_V680NCES_V690NCES_PRESENCE_FLAG 0x02
typedef struct
{
    U16                          presence_bitmask;
    sysinfotype5_v680ext_ies_t   sysinfotype5_v680ext;
    sysinfotype5_v4b0nces_v590nces_v650nces_v680nces_v690nces_t  v690NonCriticalExtensions;
}sysinfotype5_v4b0nces_v590nces_v650nces_v680nces_t;

#define UMTS_SIB_TYPE_5V4B0NCES_V590NCES_V650NCES_V650EXT_PRESENCE_FLAG           0x01
#define UMTS_SIB_TYPE_5V4B0NCES_V590NCES_V680NONCRITICALEXTENSIONS_PRESENCE_FLAG  0x02
typedef struct
{
    U16                          presence_bitmask;
    sysinfotype5_v650ext_ies_t   sysInfoType5_v650ext;
    sysinfotype5_v4b0nces_v590nces_v650nces_v680nces_t   v680NonCriticalExtensions;
}sysinfotype5_v4b0nces_v590nces_v650nces_t;

#define UMTS_SIB_TYPE_5_V590EXT_PRESENCE_FLAG                       0x01
#define UMTS_SIB_TYPE_5_V650NONCRITICALEXTENSION_PRESENCE_FLAG      0x02
typedef struct
{
    U16                          presence_bitmask;
    sysinfotype5_v590ext_ies_t   sysInfoType5_v590ext;
    sysinfotype5_v4b0nces_v590nces_v650nces_t    v650NonCriticalExtensions;
}sysinfotype5_v4b0noncriticalextensions_v590noncriticalextensions_t;

#define  UMTS_SIB_TYPE_5_V4B0EXT_PRESENCE_FLAG          0x01
#define  UMTS_SIB_TYPE_5_V590NCE_PRESENCE_FLAG          0x02
typedef struct 
{
    U16                          presence_bitmask;
    sysinfotype5_v4b0ext_ies_t   sysInfoType5_v4b0ext;
    sysinfotype5_v4b0noncriticalextensions_v590noncriticalextensions_t   v590NonCriticalExtensions;
}sysinfotype5_v4b0_non_critical_extensions_t;

/********************************************************************/
/*                                                                  */
/*  umts_sib_type_5_info_t - System Information Block type 5        */
/*                                                                  */
/********************************************************************/
#define UMTS_SIB_TYPE_5_INFO_PRIMARY_CCPCH_INFO_PRESENCE_FLAG       0x01
#define UMTS_SIB_TYPE_5_INFO_CBS_DRX_LEVEL1_INFO_PRESENCE_FLAG      0x02
#define UMTS_SIB_TYPE_5_INFO_V4B0_NON_CRITICAL_EXTNS_PRESENCE_FLAG  0x04
typedef struct
{
    U16                                         presence_bitmask;
    U8                                          sib_6_indicator;
    S8                                          pich_poweroffset;
    sysinfo_type5_modespecific_info_t           modespecific_info;
    primary_ccpch_info_t                        primary_ccpch_info;
    prach_system_information_list_t             prach_systemInformation_list;
    sccpch_system_information_list_t            sccpch_systemInformation_list;
    cbs_drx_level1_information_t                cbs_DRX_level1_information;
    sysinfotype5_v4b0_non_critical_extensions_t   v4b0_Non_Critical_Extensions;
}umts_sib_type_5_info_t;

typedef struct
{
    U8      ul_Interference;
}sysinfotype7_modespecificinfo_fdd_t;

#define RRC_UMTS_SYSINFOTYPE7_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_SYSINFOTYPE7_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32                                   t;
    union
    {
        /* t = 1 */
        sysinfotype7_modespecificinfo_fdd_t   fdd;
        /* t = 2 */
    }u;
}sysinfotype7_modespecificinfo_t;

#define DYNAMIC_PERSISTENCE_LEVEL_ELEMNENTS 16
typedef struct
{
    U32          count;
    U8           dynamic_persistencelevel[DYNAMIC_PERSISTENCE_LEVEL_ELEMNENTS];
}dynamic_persistencelevel_list_t;

typedef struct
{
    U8      __dummy__;
}sysinfotype7_nce_t;

/********************************************************************/
/*                                                                  */
/*  umts_sib_type_7_info_t - System Information Block type 7        */
/*                                                                  */
/********************************************************************/
#define UMTS_SIB_TYPE_7_PRAC_INFO_SIB6_LIST_PRESENCE_FLAG       0x01
#define UMTS_SIB_TYPE_7_EXPIRATION_TIME_FACTOR_PRESENCE_FLAG    0x02
#define UMTS_SIB_TYPE_7_NC_EXTN_PRESENCE_FLAG                   0x04
typedef struct 
{
    U16                                 presence_bitmask;
    sysinfotype7_modespecificinfo_t     sysinfotype7_modespecificinfo;
    dynamic_persistencelevel_list_t     prach_information_sib5_list;
    dynamic_persistencelevel_list_t     prach_information_sib6_list;
    U8                                  expirationtime_factor;
    sysinfotype7_nce_t                  sysinfotype7_nce;
}umts_sib_type_7_info_t;

typedef struct
{
    U32                                 n;
    U8                                  digit_elem[MAX_MCC_OCTET_SIZE];
}mcc_t;

typedef struct
{
    U32                                 n;
    U8                                  digit_elem[MAX_MNC_OCTET_SIZE];
}umts_mnc_t;

typedef struct
{
    mcc_t                               mcc;
    umts_mnc_t                          mnc;
}umts_plmn_identity_t;

#define UMTS_SIB_TYPE_18_PLMN_IDENTITIES_INTRA_FREQ_CELLS_LIST_PLMN_ID_PRESENCE_FLAG  0x01
typedef struct
{
    U16                    presence_bitmask;
    umts_plmn_identity_t   plmn_identity; 
}plmns_of_intra_freq_cells_t;

typedef struct
{
    U8                              count;
    plmns_of_intra_freq_cells_t     plmns_of_intra_freq_cells[UMTS_MAX_PLMN_ID_INFO_SIZE]; 
}plmns_of_intra_freq_cells_list_t;

#define UMTS_SIB_TYPE_18_PLMN_IDENTITIES_INTER_FREQ_CELLS_LIST_PLMN_ID_PRESENCE_FLAG  0x01
typedef struct
{
    U16                    presence_bitmask;
    umts_plmn_identity_t   plmn_identity; 
}plmns_of_inter_freq_cells_t;

typedef struct
{
    U8                              count;
    plmns_of_inter_freq_cells_t     plmns_of_inter_freq_cells[UMTS_MAX_PLMN_ID_INFO_SIZE]; 
}plmns_of_inter_freq_cells_list_t;

#define UMTS_SIB_TYPE_18_PLMN_IDENTITIES_INTER_RAT_CELLS_LIST_PLMN_ID_PRESENCE_FLAG   0x01
typedef struct
{
    U16                        presence_bitmask;
    umts_plmn_identity_t       plmn_identity; 
}plmns_of_inter_rat_cells_t;

typedef struct
{
    U8                              count;
    plmns_of_inter_rat_cells_t      plmns_of_inter_rat_cells[UMTS_MAX_PLMN_ID_INFO_SIZE]; 
}plmns_of_inter_rat_cells_list_t;

#define UMTS_SIB_TYPE_18_PLMN_IDENTITIES_INTRA_FREQ_CELLS_LIST_PRESENCE_FLAG           0x01
#define UMTS_SIB_TYPE_18_PLMN_IDENTITIES_INTER_FREQ_CELLS_LIST_PRESENCE_FLAG           0x02
#define UMTS_SIB_TYPE_18_PLMN_IDENTITIES_INTER_RAT_CELLS_LIST_PRESENCE_FLAG            0x04
typedef struct
{
    U16                                     presence_bitmask;
    plmns_of_intra_freq_cells_list_t        plmns_of_intra_freq_cells_list;
    plmns_of_inter_freq_cells_list_t        plmns_of_inter_freq_cells_list;
    plmns_of_inter_rat_cells_list_t         plmns_of_inter_rat_cells_list;
}plmn_identities_of_neighbour_cells_t;

#define UMTS_SIB_TYPE_18_V6B0_NCE_IDLE_MODE_PLMN_IDS_PRESENCE_FLAG    0x01
#define UMTS_SIB_TYPE_18_V6B0_NCE_CONN_MODE_PLMN_IDS_PRESENCE_FLAG    0x02
typedef struct
{
    U16                                     presence_bitmask;
    plmn_identities_of_neighbour_cells_t    idle_mode_plmn_identities;
    plmn_identities_of_neighbour_cells_t    connected_mode_plmn_identities;
}sysinfotype18_v6b0ext_t;

#define UMTS_PLMN_IDENTITY_WITH_OPTIONAL_MCC_R6_MCC_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                         presence_bitmask;
    mcc_t                                       mcc;
    umts_mnc_t                                  mnc;
}plmn_identity_with_optional_mcc_r6_t;

typedef struct
{
    U8                                      count;
    plmn_identity_with_optional_mcc_r6_t    plmn_identity_with_optional_mcc_r6[6];
}multiple_plmns_of_intra_freq_cells_list_element_multiple_plmn_list_t;

#define UMTS_SIB_TYPE_18_V860_NCE_PLMN_IDENTITIES_MUL_PLMNS_OF_INTRA_FREQ_CELL_MUL_PLMN_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                                                     presence_bitmask;
    multiple_plmns_of_intra_freq_cells_list_element_multiple_plmn_list_t    multiple_plmn_list;
}multiple_plms_of_intra_freq_cells_list_element_t;

typedef struct
{
    U8                              count;
    multiple_plms_of_intra_freq_cells_list_element_t multiple_plms_of_intra_freq_cells_list_element[MAX_CELL_MEAS];
}multiple_plmns_of_intra_freq_cells_list_t;

typedef multiple_plmns_of_intra_freq_cells_list_element_multiple_plmn_list_t multiple_plmns_of_inter_freq_cells_list_element_multiple_plmn_list_t;

#define UMTS_SIB_TYPE_18_V860_NCE_PLMN_IDENTITIES_MUL_PLMNS_OF_INTER_FREQ_CELL_MUL_PLMN_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                                                     presence_bitmask;
    multiple_plmns_of_inter_freq_cells_list_element_multiple_plmn_list_t    multiple_plmn_list;
}multiple_plms_of_inter_freq_cells_list_element_t;

typedef struct
{
    U8                              count;
    multiple_plms_of_inter_freq_cells_list_element_t multiple_plms_of_inter_freq_cells_list_element[MAX_CELL_MEAS];
}multiple_plmns_of_inter_freq_cells_list_t;

#define UMTS_SIB_TYPE_18_V860_NCE_PLMN_IDENTITIES_MUL_PLMNS_OF_INTRA_FREQ_CELL_PRESENCE_FLAG   0x01
#define UMTS_SIB_TYPE_18_V860_NCE_PLMN_IDENTITIES_MUL_PLMNS_OF_INTER_FREQ_CELL_PRESENCE_FLAG   0x02
typedef struct
{
    U16                                             presence_bitmask;
    multiple_plmns_of_intra_freq_cells_list_t       multiple_plmns_of_intra_freq_cells_list;
    multiple_plmns_of_inter_freq_cells_list_t       multiple_plmns_of_inter_freq_cells_list;
}plmn_identities_of_neighbour_cells_v860ext_t;

#define UMTS_SIB_TYPE_18_V860_NCE_IDLE_MODE_PLMN_IDENTITIES_PRESENCE_FLAG   0x01
#define UMTS_SIB_TYPE_18_V860_NCE_CONN_MODE_PLMN_IDENTITIES_PRESENCE_FLAG   0x02
typedef struct
{
    U16                                             presence_bitmask;
    plmn_identities_of_neighbour_cells_v860ext_t    idle_mode_plmn_identities;
    plmn_identities_of_neighbour_cells_v860ext_t    connected_mode_plmn_identities;
}sysinfotype18_v860ext_t;

typedef struct
{
    U8                  dummy;
}sysinfotype18_v6b0_nce_v860_nce_nce_t;

#define UMTS_SIB_TYPE_18_V6B0_NCE_V860_NCE_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                     presence_bitmask;
    sysinfotype18_v860ext_t                 sysInfoType18_v860ext;
    sysinfotype18_v6b0_nce_v860_nce_nce_t   nonCriticalExtensions;
}sysinfotype18_v6b0_nce_v860_nce_t;

#define UMTS_SIB_TYPE_18_V6B0_V860_NCE_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                     presence_bitmask;
    sysinfotype18_v6b0ext_t                 sysinfotype18_v6b0ext;
    sysinfotype18_v6b0_nce_v860_nce_t       v860_non_critical_extensions;
}sysinfotype18_v6b0_nce_t;

/********************************************************************/
/*                                                                  */
/*  umts_sib_type_18_info_t - System Information Block type 18      */
/*                                                                  */
/********************************************************************/
#define UMTS_SIB_TYPE_18_IDLE_MODE_PLMN_IDENTITIES_PRESENCE_FLAG        0x01
#define UMTS_SIB_TYPE_18_CONNECTED_MODE_PLMN_IDENTITIES_PRESENCE_FLAG   0x02
#define UMTS_SIB_TYPE_18_NC_EXTN_PRESENCE_FLAG                          0x04
typedef struct
{
    U16                                     presence_bitmask;
    plmn_identities_of_neighbour_cells_t    idle_mode_plmn_identities;
    plmn_identities_of_neighbour_cells_t    connected_mode_plmn_identities;
    sysinfotype18_v6b0_nce_t                v6b0_non_ritical_extensions;
}umts_sib_type_18_info_t;

/********************************************************************/
/*                                                                  */
/*  umts_sib_type_16_info                                            */
/*                                                                  */
/********************************************************************/
typedef struct
{
    U32                                     timer_mrw;
    U32                                     timer_discard;
    U32                                     max_mrw;
}explicit_discard_t;

typedef struct
{
    U32                                     max_dat;
    U32                                     timer_mrw;
    U32                                     max_mrw;
}max_dat_retransmissions_t;

#define RRC_UMTS_TRANSMISSIONRLC_DISCARD_TIMERBASEDEXPLICIT 1
#define RRC_UMTS_TRANSMISSIONRLC_DISCARD_TIMERBASEDNOEXPLICIT 2
#define RRC_UMTS_TRANSMISSIONRLC_DISCARD_MAXDAT_RETRANSMISSIONS 3
#define RRC_UMTS_TRANSMISSIONRLC_DISCARD_NODISCARD 4
typedef struct
{
    U32                             t;
    union
    {
        explicit_discard_t                      timer_based_explicit;
        U32                                     timer_based_no_explicit;
        max_dat_retransmissions_t               max_dat_retransmissions;
        U32                                     no_discard;
    }u;
}transmission_rlc_discard_t;

#define UMTS_SIB_POLLING_INFO_TIMER_POLL_PROHIBIT_PRESENCE_FLAG 0x01
#define UMTS_SIB_POLLING_INFO_TIMER_POLL_PRESENCE_FLAG          0x02
#define UMTS_SIB_POLLING_INFO_POLL_PDU_PRESENCE_FLAG            0x04
#define UMTS_SIB_POLLING_INFO_POLL_SDU_PRESENCE_FLAG            0x08
#define UMTS_SIB_POLLING_INFO_POLL_WINDOW_PRESENCE_FLAG         0x10
#define UMTS_SIB_POLLING_INFO_TIMERPOLLPERIODIC_PRESENCE_FLAG   0x20
typedef struct
{
    U16                                     presence_bitmask;
    U32                                     timer_poll_prohibit;
    U32                                     timer_poll;
    U32                                     poll_pdu;
    U32                                     poll_sdu;
    U8                                      last_transmission_pdu_poll;
    U8                                      last_retransmission_pdu_poll;
    U32                                     poll_window;
    U32                                     timerPollPeriodic;
}polling_info_t;

#define UMTS_SIB_UL_AM_RLC_MODE_POLLING_INFO_PRESENCE_FLAG  0x01
typedef struct
{
    U16                                     presence_bitmask;
    transmission_rlc_discard_t              transmission_rlc_discard;
    U32                                     transmission_window_size;
    U32                                     timer_rst;
    U32                                     max_RST;
    polling_info_t                          polling_info;
}ul_am_rlc_mode_t;

#define UMTS_SIB_UL_UM_RLC_MODE_TRANSMISSION_RLC_DISCARD_PRESENCE_FLAG  0x01
typedef struct
{
    U16                                     presence_bitmask;
    transmission_rlc_discard_t              transmission_rlc_discard;
}ul_um_rlc_mode_t;

#define UMTS_SIB_UL_TM_RLC_MODE_TRANSMISSION_RLC_DISCARD_PRESENCE_FLAG  0x01
typedef struct
{
    U16                                     presence_bitmask;
    transmission_rlc_discard_t              transmission_rlc_discard;
    U8                                      segmentation_indication;
}ul_tm_rlc_mode_t;

#define RRC_UMTS_UL_RLC_MODE_UL_AM_RLC_MODE 1
#define RRC_UMTS_UL_RLC_MODE_UL_UM_RLC_MODE 2
#define RRC_UMTS_UL_RLC_MODE_UL_TM_RLC_MODE 3
#define RRC_UMTS_UL_RLC_MODE_SPARE        4
typedef struct
{
    U32         t;
    union 
    {
        /* t = 1 */
        ul_am_rlc_mode_t        ul_am_rlc_mode;
        /* t = 2 */
        ul_um_rlc_mode_t        ul_um_rlc_mode;
        /* t = 3 */
        ul_tm_rlc_mode_t        ul_tm_rlc_mode;
        /* t = 4 */
    } u;
}ul_rlc_mode_t;

#define UMTS_SIB_DL_RLC_STATUS_INFO_TIMER_STATUS_PROHIBIT_PRESENCE_FLAG 0x01
#define UMTS_SIB_DL_RLC_STATUS_INFO_DUMMY_PRESENCE_FLAG                 0x02
#define UMTS_SIB_DL_RLC_STATUS_INFO_TIMER_STATUS_PERIODIC_PRESENCE_FLAG 0x04
typedef struct
{
    U16                                     presence_bitmask;
    U32                                     timer_status_prohibit;
    U32                                     dummy;
    U8                                      missing_pdu_indicator;
    U32                                     timer_status_periodic;
}dl_rlc_status_info_t;

typedef struct
{
    U8                                      in_sequence_delivery;
    U32                                     receiving_window_size;
    dl_rlc_status_info_t                    dl_rlc_status_info;
}dl_am_rlc_mode_t; 

typedef struct
{
    U8                                      segmentation_indication;
}dl_tm_rlc_mode_t; 

#define RRC_UMTS_DL_RLC_MODE_DL_AM_RLC_MODE 1
#define RRC_UMTS_DL_RLC_MODE_DL_UM_RLC_MODE 2
#define RRC_UMTS_DL_RLC_MODE_DL_TM_RLC_MODE 3
typedef struct 
{
    U32             t;
    union 
    {
        /* t = 1 */
        dl_am_rlc_mode_t          dl_am_rlc_mode;
        /* t = 2 */
        /* t = 3 */
        dl_tm_rlc_mode_t          dl_tm_rlc_mode;
    } u;
}dl_rlc_mode_t;

#define UMTS_SIB_RLC_INFO_UL_RLC_MODE_PRESENCE_FLAG 0x01
#define UMTS_SIB_RLC_INFO_DL_RLC_MODE_PRESENCE_FLAG 0x02
typedef struct
{
    U16                                     presence_bitmask;
    ul_rlc_mode_t                           ul_rlc_mode;
    dl_rlc_mode_t                           dl_rlc_mode;
}rlc_info_t;

#define RRC_UMTS_RLC_INFOCHOICE_RLC_INFO  1
#define RRC_UMTS_RLC_INFOCHOICE_SAME_AS_RB 2
typedef struct
{
    U32     t;
    union 
    {
        /* t = 1 */
        rlc_info_t   rlc_Info;
        /* t = 2 */
        U8            same_as_rb;
    } u;
}rlc_info_choice_t;

#define RRC_UMTS_UL_TRANSPORTCHANNELTYPE_DCH 1
#define RRC_UMTS_UL_TRANSPORTCHANNELTYPE_RACH 2
#define RRC_UMTS_UL_TRANSPORTCHANNELTYPE_DUMMY 3
#define RRC_UMTS_UL_TRANSPORTCHANNELTYPE_USCH 4
typedef struct
{
    U32     t;
    union 
    {
        /* t = 1 */
        U8    dch;
        /* t = 2 */
        /* t = 3 */
        /* t = 4 */
        U8    usch;
    } u;
}ul_transport_channel_type_t;

typedef struct
{
    U8                              rlc_size_index;
}rlc_size_info_t;

typedef struct
{
    U8                              count;
    rlc_size_info_t                 rlc_size_info[];
}rlc_size_explicit_list_t;

#define RRC_UMTS_UL_LOGICALCHANNELMAPPING_RLC_SIZELIST_ALLSIZES 1
#define RRC_UMTS_UL_LOGICALCHANNELMAPPING_RLC_SIZELIST_CONFIGURED 2
#define RRC_UMTS_UL_LOGICALCHANNELMAPPING_RLC_SIZELIST_EXPLICITLIST 3
typedef struct
{
    U32     t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        /* t = 3 */
        rlc_size_explicit_list_t                    explicit_list;
    }u;
}ul_logical_channel_mapping_rlc_size_list_t;

#define UMTS_SIB_UL_LOGICAL_CHANNEL_MAPPING_LOGICALCHANNELIDENTITY_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                         presence_bitmask;
    ul_transport_channel_type_t                 ul_transport_channel_type;
    U8                                          logicalChannelIdentity;
    ul_logical_channel_mapping_rlc_size_list_t  rlc_size_list;  
    U8                                          mac_LogicalChannelPriority;
}ul_logical_channel_mapping_t;

typedef struct
{
    U8                              count;
    ul_logical_channel_mapping_t    ul_logical_channel_mapping[];
}ul_logicalchannelmappinglist_ul_logicalchannelmapping_t;

typedef struct
{
    U8                              rlc_logical_channel_mapping_indicator;
    ul_logicalchannelmappinglist_ul_logicalchannelmapping_t ul_logical_channel_mapping;
}ul_logical_channel_mapping_list_t;

#define RRC_UMTS_UL_LOGICALCHANNELMAPPINGS_ONELOGICALCHANNEL 1
#define RRC_UMTS_UL_LOGICALCHANNELMAPPINGS_TWOLOGICALCHANNELS 2
typedef struct
{
    U32     t;
    union 
    {
        /* t = 1 */
        ul_logical_channel_mapping_t              one_logical_channel;
        /* t = 2 */
        ul_logical_channel_mapping_list_t         two_logical_channels;
    } u;
}ul_logical_channel_mappings_t;

typedef struct
{
    U8                                      dch_transport_ch_id;
    U8                                      dsch_transport_ch_id;
}transport_channel_identity_dch_dsch_t;

#define RRC_UMTS_DL_TRANSPORTCHANNELTYPE_DCH 1
#define RRC_UMTS_DL_TRANSPORTCHANNELTYPE_FACH 2
#define RRC_UMTS_DL_TRANSPORTCHANNELTYPE_DSCH 3
#define RRC_UMTS_DL_TRANSPORTCHANNELTYPE_DCH_AND_DSCH 4
typedef struct
{
    U32     t;
    union 
    {
        /* t = 1 */
        U8        dch;
        /* t = 2 */
        /* t = 3 */
        U8        dsch;
        /* t = 4 */
        transport_channel_identity_dch_dsch_t dch_and_dsch;
    } u;
}dl_transport_channel_type_t;

#define UMTS_SIB_DL_LOGICAL_CHANNEL_MAPPING_LOGICAL_CHANNEL_IDENTITY_PRESENCE_FLAG  0x01
typedef struct
{
    U16                                     presence_bitmask;
    dl_transport_channel_type_t             dl_transport_channel_type;              
    U8                                      logical_channel_identity;
}dl_logical_channel_mapping_t;

typedef struct
{
    U8                                      count;
    dl_logical_channel_mapping_t            dl_logical_channel_mapping[];
}dl_logical_channel_mapping_list_t;

#define UMTS_SIB_RB_MAPPING_OPTION_UL_LOGICAL_CHANNEL_MAPPINGS_PRESENCE_FLAG        0x01
#define UMTS_SIB_RB_MAPPING_OPTION_DL_LOGICAL_CHANNEL_MAPPING_LIST_PRESENCE_FLAG    0x02
typedef struct
{
    U16                                     presence_bitmask;
    ul_logical_channel_mappings_t           ul_logical_channel_mappings;
    dl_logical_channel_mapping_list_t       dl_logical_channel_mapping_list;
}rb_mapping_option_t;

typedef struct
{
    U8                              count;
    rb_mapping_option_t             rb_mapping_option[];    
}rb_mapping_info_list_t;

#define UMTS_SIB_SRB_INFORMATION_SETUP_RB_IDENTITY_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                     presence_bitmask;
    U8                                      rb_identity;
    rlc_info_choice_t                       rlc_info_choice;
    rb_mapping_info_list_t                  rb_mapping_info_list;
}srb_information_setup_t;

typedef struct
{
    U8                              count;
    srb_information_setup_t         srb_information_setup[];
}srb_information_setup_list_t;

#define RRC_UMTS_LOSSLESSSRNS_RELOCSUPPORT_SUPPORTED 1
#define RRC_UMTS_LOSSLESSSRNS_RELOCSUPPORT_NOTSUPPORTED 2
typedef struct
{
    U32         t;
    union
    {
        /* t = 1 */
        U32                                     supported;
        /* t = 2 */
    }u;
}lossless_srns_reloc_support_t;

typedef struct
{
    U16                                     f_MAX_PERIOD;
    U8                                      f_MAX_TIME;
    U16                                     max_HEADER;
    U8                                      tcp_SPACE;
    U16                                     non_TCP_SPACE;
    U32                                     expect_reordering;
}rfc2507_info_t;

#define RRC_UMTS_ALGORITHMSPECIFICINFO_RFC2507_INFO 1
typedef struct
{
    U32                 t;
    union
    {
        /* t = 1 */
        rfc2507_info_t                          rfc2507_info;
    }u;
}algorithm_specific_info_t;

typedef struct
{
    algorithm_specific_info_t               algorithm_specific_info;
}header_compression_info_t;

typedef struct
{
    U8                              count;
    header_compression_info_t       header_compression_info[];
}header_compression_info_list_t;

#define UMTS_SIB_PDCP_INFO_LOSSLESS_SRNS_RELOC_SUPPORT_PRESENCE_FLAG    0x01
#define UMTS_SIB_PDCP_INFO_HEADER_COMPRESSION_INFO_LIST_PRESENCE_FLAG   0x02
typedef struct
{
    U16                                     presence_bitmask;
    lossless_srns_reloc_support_t           lossless_srns_reloc_support;
    U32                                     pdcp_pdu_header;
    header_compression_info_list_t          header_compression_info_list;
}pdcp_info_t;

#define UMTS_SIB_RB_INFORMATION_SETUP_PDCP_INFO_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                     presence_bitmask;
    U8                                      rb_identity;
    pdcp_info_t                             pdcp_info;
    rlc_info_choice_t                       rlc_info_choice;
    rb_mapping_info_list_t                  rb_mapping;
}rb_information_setup_t;

typedef struct
{
    U8                              count;
    rb_information_setup_t          rb_information_setup[];
}rb_information_setup_list_t;

typedef struct
{
    U32                                     re_establishment_timer;
    srb_information_setup_list_t            srb_information_setup_list;
    rb_information_setup_list_t             rb_information_setup_list;
}predefined_rb_configuration_t;

typedef struct
{
    U32                                     count;
    U16                                     elem[];
}allowed_tfc_list_t;

typedef struct
{
    U32                                     count;
    U16                                     elem[];
}non_allowed_tfc_list_t;

typedef struct
{
    U32                                     count;
    U16                                     elem[];
}allowed_tfi_list_t;

#define UMTS_SIB_RESTRICTED_TR_CH_INFO_ALLOWED_TFI_LIST_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                     presence_bitmask;
    U32                                     ul_transport_channel_type;
    U8                                      restricted_tr_ch_identity;
    allowed_tfi_list_t                      allowed_tfi_list;
}restricted_tr_ch_info_t;

typedef struct
{
    U16                                     count;
    restricted_tr_ch_info_t                 restricted_tr_ch_info[];
}restrictedt_rch_info_list_t;

#define RRC_UMTS_TFC_SUBSET_MINIMUMALLOWEDTFC_NUMBER 1
#define RRC_UMTS_TFC_SUBSET_ALLOWEDTFC_LIST 2
#define RRC_UMTS_TFC_SUBSET_NON_ALLOWEDTFC_LIST 3
#define RRC_UMTS_TFC_SUBSET_RESTRICTEDTRCHINFOLIST 4
#define RRC_UMTS_TFC_SUBSET_FULLTFCS      5
typedef struct
{
    U32     t;
    union 
    {
        /* t = 1 */
        U16                                   minimumAllowedTFC_Number;
        /* t = 2 */
        allowed_tfc_list_t                    allowedtfc_list;

        /* t = 3 */
        non_allowed_tfc_list_t                non_allowedtfc_list;
        /* t = 4 */
        restrictedt_rch_info_list_t           restricted_tr_ch_info_list;
        /* t = 5 */
    } u;
}tfc_subset_t;

#define UMTS_SIB_UL_COMMON_TRANS_CH_INFO_TFC_SUBSET_PRESENCE_FLAG           0x01
#define UMTS_SIB_UL_COMMON_TRANS_CH_INFO_PRACH_TFCS_PRESENCE_FLAG           0x02
#define UMTS_SIB_UL_COMMON_TRANS_CH_INFO_MODE_SPECIFIC_INFO_PRESENCE_FLAG   0x04
typedef struct
{
    U16                                     presence_bitmask;
    tfc_subset_t                            tfc_subset;
    tfcs_t                                  prach_tfcs;
    /*  ul_common_trans_ch_info_mode_specific_info_t    mode_specific_info;    */
}ul_common_trans_ch_info_t;

typedef struct
{
    ul_common_trans_ch_info_t               ul_common_trans_ch_info;
    /*
       UL_AddReconfTransChInfoList
       DL_CommonTransChInfo
       DL_AddReconfTransChInfoList
       */
}predef_trans_ch_configuration_t;

typedef struct
{
    power_control_algorithm_t                   power_control_algorithm;
}ul_dpch_power_control_info_pre_def_fdd_t;

typedef struct
{
    U8                                          dpch_constant_value;
}ul_dpch_power_control_info_pre_def_tdd_t;

#define RRC_UMTS_UL_DPCH_POWERCONTROLINFOPREDEF_FDD 1
#define RRC_UMTS_UL_DPCH_POWERCONTROLINFOPREDEF_TDD 2
typedef struct
{
    U32     t;
    union 
    {
        /* t = 1 */
        ul_dpch_power_control_info_pre_def_fdd_t  fdd;
        /* t = 2 */
        ul_dpch_power_control_info_pre_def_tdd_t  tdd;
    } u;
}ul_dpch_power_control_info_predef_t;

typedef struct
{
    U8                                      tfci_existence;
    U32                                     puncturing_limit;
}ul_dpch_info_pre_def_mode_specific_info_fdd_t;

typedef struct
{
    common_timeslot_info_t              common_timeslot_info;
}ul_dpch_info_pre_def_mode_specific_info_tdd_t;

#define RRC_UMTS_UL_DPCH_INFOPREDEF_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_UL_DPCH_INFOPREDEF_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32         t;
    union 
    {
        /* t = 1 */
        ul_dpch_info_pre_def_mode_specific_info_fdd_t fdd;
        /* t = 2 */
        ul_dpch_info_pre_def_mode_specific_info_tdd_t tdd;
    } u;

}ul_dpch_info_predef_mode_specific_info_t;

typedef struct
{
    ul_dpch_power_control_info_predef_t     ul_dpch_power_control_info_predef;
    ul_dpch_info_predef_mode_specific_info_t    mode_specific_info;    
}ul_dpch_info_predef_t;

#define RRC_UMTS_SF512_ANDPILOT_SFD4      1
#define RRC_UMTS_SF512_ANDPILOT_SFD8      2
#define RRC_UMTS_SF512_ANDPILOT_SFD16     3
#define RRC_UMTS_SF512_ANDPILOT_SFD32     4
#define RRC_UMTS_SF512_ANDPILOT_SFD64     5
#define RRC_UMTS_SF512_ANDPILOT_SFD128    6
#define RRC_UMTS_SF512_ANDPILOT_SFD256    7
#define RRC_UMTS_SF512_ANDPILOT_SFD512    8
typedef struct
{
    U32     t;
    union {
        /* t = 1 */
        /* t = 2 */
        /* t = 3 */
        /* t = 4 */
        /* t = 5 */
        /* t = 6 */
        OSUINT32    sfd128;
        /* t = 7 */
        OSUINT32    sfd256;
        /* t = 8 */
    } u;
}sf512_and_pilot_t;

typedef struct
{
    sf512_and_pilot_t       spreading_factor_and_pilot;
    U32                     position_fixed_or_flexible;
    U8                      tfci_existence;
}dl_dpch_info_common_pre_def_mode_specific_info_fdd_t;

typedef struct
{
    common_timeslot_info_t     common_time_slot_info;
}dl_dpch_info_common_pre_def_mode_specific_info_tdd_t;

#define RRC_UMTS_DL_DPCH_INFOCOMMONPREDEF_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_DL_DPCH_INFOCOMMONPREDEF_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32            t;
    union 
    {
        /* t = 1 */
        dl_dpch_info_common_pre_def_mode_specific_info_fdd_t   fdd;
        /* t = 2 */
        dl_dpch_info_common_pre_def_mode_specific_info_tdd_t    tdd;
    } u;
}dl_dpch_info_common_pre_def_mode_specific_info_t;

typedef struct
{
    dl_dpch_info_common_pre_def_mode_specific_info_t    mode_specific_info;
}dl_dpch_info_common_predef_t;

#define UMTS_SIB_DL_COMMON_INFORMATION_PREDEF_DL_DPCH_INFO_COMMON_PREDEF_PRESENCE_FLAG  0x01
typedef struct
{
    U16                                     presence_bitmask;
    dl_dpch_info_common_predef_t            dl_dpch_info_common_predef;
}dl_common_information_predef_t;

#define UMTS_SIB_PRE_DEF_PHY_CH_CONFIGURATION_DL_COMMON_INFORMATION_PREDEF_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                     presence_bitmask;
    ul_dpch_info_predef_t                   ul_dpch_info_predef;
    dl_common_information_predef_t          dl_common_information_predef;
}pre_def_phy_ch_configuration_t;

typedef struct
{
    predefined_rb_configuration_t           predefined_rb_configuration;
    predef_trans_ch_configuration_t         predef_trans_ch_configuration;
    pre_def_phy_ch_configuration_t          pre_def_phy_ch_configuration;
}pre_def_radio_configuration_t;

#define UMTS_SIB_TYPE_16_NC_EXTN_PRESENCE_FLAG       0x01
typedef struct
{
    U16                                     presence_bitmask;
    pre_def_radio_configuration_t           pre_def_radio_configuration;
    /*    SysInfoType16_v770NonCriticalExtensions v770NonCriticalExtensions; */
}umts_sib_type_16_info_t;

#define P_REV_OCTET_SIZE        1
#define MIN_P_REV_OCTET_SIZE    1
#define SID_OCTET_SIZE          2
#define NID_OCTET_SIZE          2
#define MAX_SIB_SB_IN_REF_LIST  32
#define MAX_SIB_IN_REF_LIST     32
#define SIB_POS_OFFSET_SIZE     15
#define MAX_PLMN_ID_LIST_SIZE   5
typedef struct
{
    umts_plmn_identity_t     plmn_identity;
}plmn_type_gsm_map_t;

typedef struct
{
    U8              p_rev[P_REV_OCTET_SIZE];
    U8              min_p_rev[MIN_P_REV_OCTET_SIZE];
    U8              sid[SID_OCTET_SIZE];
    U8              nid[NID_OCTET_SIZE];
}plmn_type_ansi_41_t;


typedef struct
{
    umts_plmn_identity_t    plmn_Identity; 
    U8                      p_rev[P_REV_OCTET_SIZE];
    U8                      min_p_rev[MIN_P_REV_OCTET_SIZE];
    U8                      sid[SID_OCTET_SIZE];
    U8                      nid[NID_OCTET_SIZE];
}plmn_type_gsm_map_and_ansi_41_t;


typedef struct
{
    U8      predefined_config_identity;
    U8      predefined_config_value_tag;
}predefined_config_identity_value_tag_t;

typedef struct
{
    U8      sib_occur_identity;
    U8      sib_occur_value_tag;
}sib_occurrence_identity_value_tag_t;

/********************************************************************
 *                                                                  *
 *  sib_type_and_tag_t - SIB and SB type identifies a specific      *
 *                       system information block                   *
 *                                                                  *
 *******************************************************************/
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE1 1
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE2 2
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE3 3
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE4 4
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE5 5
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE6 6
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE7 7
#define RRC_UMTS_SIB_TYPEANDTAG_DUMMY     8
#define RRC_UMTS_SIB_TYPEANDTAG_DUMMY2    9
#define RRC_UMTS_SIB_TYPEANDTAG_DUMMY3    10
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE11 11
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE12 12
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE13 13
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE13_1 14
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE13_2 15
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE13_3 16
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE13_4 17
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE14 18
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE15 19
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE16 20
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE17 21
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE15_1 22
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE15_2 23
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE15_3 24
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE15_4 25
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE18 26
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE15_5 27
#define RRC_UMTS_SIB_TYPEANDTAG_SYSINFOTYPE5BIS 28
#define RRC_UMTS_SIB_TYPEANDTAG_SPARE4    29
#define RRC_UMTS_SIB_TYPEANDTAG_SPARE3    30
#define RRC_UMTS_SIB_TYPEANDTAG_SPARE2    31
#define RRC_UMTS_SIB_TYPEANDTAG_SPARE1    32
typedef struct {
    U32     t;
    union {
        /* t = 1 */
        U16 sysInfoType1;
        /* t = 2 */
        U8 sysInfoType2;
        /* t = 3 */
        U8 sysInfoType3;
        /* t = 4 */
        U8 sysInfoType4;
        /* t = 5 */
        U8 sysInfoType5;
        /* t = 6 */
        U8 sysInfoType6;
        /* t = 7 */
        /* t = 8 */
        U8 dummy;
        /* t = 9 */
        /* t = 10 */
        /* t = 11 */
        U8 sysInfoType11;
        /* t = 12 */
        U8 sysInfoType12;
        /* t = 13 */
        U8 sysInfoType13;
        /* t = 14 */
        U8 sysInfoType13_1;
        /* t = 15 */
        U8 sysInfoType13_2;
        /* t = 16 */
        U8 sysInfoType13_3;
        /* t = 17 */
        U8 sysInfoType13_4;
        /* t = 18 */
        /* t = 19 */
        U8 sysInfoType15;
        /* t = 20 */
        predefined_config_identity_value_tag_t  sysInfoType16;
        /* t = 21 */
        /* t = 22 */
        U8 sysInfoType15_1;
        /* t = 23 */
        sib_occurrence_identity_value_tag_t     sysInfoType15_2;
        /* t = 24 */
        sib_occurrence_identity_value_tag_t     sysInfoType15_3;
        /* t = 25 */
        U8 sysInfoType15_4;
        /* t = 26 */
        U8 sysInfoType18;
        /* t = 27 */
        U8 sysInfoType15_5;
        /* t = 28 */
        U8 sysInfoType5bis;
        /* t = 29 */
        /* t = 30 */
        /* t = 31 */
        /* t = 32 */
    } u;
}sib_type_and_tag_t;

#define RRC_UMTS_SCHEDULINGINFORMATION_SCHEDULING_SIB_POS_REP4 1
#define RRC_UMTS_SCHEDULINGINFORMATION_SCHEDULING_SIB_POS_REP8 2
#define RRC_UMTS_SCHEDULINGINFORMATION_SCHEDULING_SIB_POS_REP16 3
#define RRC_UMTS_SCHEDULINGINFORMATION_SCHEDULING_SIB_POS_REP32 4
#define RRC_UMTS_SCHEDULINGINFORMATION_SCHEDULING_SIB_POS_REP64 5
#define RRC_UMTS_SCHEDULINGINFORMATION_SCHEDULING_SIB_POS_REP128 6
#define RRC_UMTS_SCHEDULINGINFORMATION_SCHEDULING_SIB_POS_REP256 7
#define RRC_UMTS_SCHEDULINGINFORMATION_SCHEDULING_SIB_POS_REP512 8
#define RRC_UMTS_SCHEDULINGINFORMATION_SCHEDULING_SIB_POS_REP1024 9
#define RRC_UMTS_SCHEDULINGINFORMATION_SCHEDULING_SIB_POS_REP2048 10
#define RRC_UMTS_SCHEDULINGINFORMATION_SCHEDULING_SIB_POS_REP4096 11
typedef struct
{
    U32      t;
    union {
        /* t = 1 */
        U8 rep4;
        /* t = 2 */
        U8 rep8;
        /* t = 3 */
        U8 rep16;
        /* t = 4 */
        U8 rep32;
        /* t = 5 */
        U8 rep64;
        /* t = 6 */
        U8 rep128;
        /* t = 7 */
        U8 rep256;
        /* t = 8 */
        U8 rep512;
        /* t = 9 */
        U16 rep1024;
        /* t = 10 */
        U16 rep2048;
        /* t = 11 */
        U16 rep4096;
    } u;
}schedulingInformation_scheduling_sib_pos_t;

typedef struct
{
    U32                      count;
    U32                      sib_off_elem[SIB_POS_OFFSET_SIZE];
}sib_off_list_t;

#define UMTS_SCHEDULING_INFO_SIB_OFF_LIST_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                         presence_bitmask;
    U8                                          seg_count;
    schedulingInformation_scheduling_sib_pos_t  sib_Pos;
    sib_off_list_t                              sib_pos_offset_info;
}scheduling_information_scheduling_t;

typedef struct
{
    U8                                      count;
    plmn_identity_with_optional_mcc_r6_t    plmn_identity_with_optional_mcc_r6[MAX_PLMN_ID_LIST_SIZE];
}multiple_plmn_list_r6_multiple_plmns_t;

typedef struct
{
    U8                                      mib_plmn_identity;
    multiple_plmn_list_r6_multiple_plmns_t  multiple_plmn_list_r6_multiple_plmns;
}multiple_plmn_list_r6_t;

#define UMTS_MULTIPLE_PLMN_LIST_R6_PRESENCE_FLAG     0x01
typedef struct
{
    U16                                     presence_bitmask;
    multiple_plmn_list_r6_t                 multiple_plmn_list_r6;
}mib_v690ext_t;

/********************************************************************
 *                                                                  *
 *  sib_type_ext2_t - SIB type extension2                           *
 *                                                                  *
 *******************************************************************/
#define RRC_UMTS_SIB_TYPEEXT2_SYSTEMINFOTYPE19 1
#define RRC_UMTS_SIB_TYPEEXT2_SYSTEMINFOTYPE15_2TER 2
#define RRC_UMTS_SIB_TYPEEXT2_SYSTEMINFOTYPE20 3
#define RRC_UMTS_SIB_TYPEEXT2_SPARE5      4
#define RRC_UMTS_SIB_TYPEEXT2_SPARE4      5
#define RRC_UMTS_SIB_TYPEEXT2_SPARE3      6
#define RRC_UMTS_SIB_TYPEEXT2_SPARE2      7
#define RRC_UMTS_SIB_TYPEEXT2_SPARE1      8
typedef struct
{
    U32         t;
}sib_type_ext2_t;

/********************************************************************
 *                                                                  *
 *  scheduling_information_t - Scheduling information               *
 *                                                                  *
 *******************************************************************/
typedef struct
{
    scheduling_information_scheduling_t scheduling;
}scheduling_information_t;

#define RRC_UMTS_VALUETAGINFO_NONE        1
#define RRC_UMTS_VALUETAGINFO_CELLVALUETAG 2
#define RRC_UMTS_VALUETAGINFO_PLMN_VALUETAG 3
#define RRC_UMTS_VALUETAGINFO_SIBOCCURRENCEIDENTITYANDVALUETAG 4
typedef struct
{
    U32                     t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        U8                  cell_value_tag;
        /* t = 3 */
        U16                 plmn_value_tag;
        /* t = 4 */
        sib_occurrence_identity_value_tag_t sib_occurrence_identity_value_tag;
    }u;
}value_tag_info_t;

typedef struct
{
    sib_type_ext2_t                 sib_type_ext2;
    scheduling_information_t        scheduling_information;
    value_tag_info_t                value_tag_info;
}ext_sib_type_info_scheduling_info2_t;

typedef struct
{
    U8                                      count;
    ext_sib_type_info_scheduling_info2_t    ext_sib_type_info_scheduling_info2[MAX_SIB_SB_IN_REF_LIST];
}ext_sib_type_info_scheduling_info_list2_t;

#define RRC_UMTS_SIB_TYPEEXTGANSS_SYSTEMINFOTYPE15_1BIS 1
#define RRC_UMTS_SIB_TYPEEXTGANSS_SYSTEMINFOTYPE15_2BIS 2
#define RRC_UMTS_SIB_TYPEEXTGANSS_SYSTEMINFOTYPE15_2TER 3
#define RRC_UMTS_SIB_TYPEEXTGANSS_SYSTEMINFOTYPE15_3BIS 4
#define RRC_UMTS_SIB_TYPEEXTGANSS_SYSTEMINFOTYPE15_6 5
#define RRC_UMTS_SIB_TYPEEXTGANSS_SYSTEMINFOTYPE15_7 6
#define RRC_UMTS_SIB_TYPEEXTGANSS_SYSTEMINFOTYPE15_8 7
#define RRC_UMTS_SIB_TYPEEXTGANSS_SPARE9  8
#define RRC_UMTS_SIB_TYPEEXTGANSS_SPARE8  9
#define RRC_UMTS_SIB_TYPEEXTGANSS_SPARE7  10
#define RRC_UMTS_SIB_TYPEEXTGANSS_SPARE6  11
#define RRC_UMTS_SIB_TYPEEXTGANSS_SPARE5  12
#define RRC_UMTS_SIB_TYPEEXTGANSS_SPARE4  13
#define RRC_UMTS_SIB_TYPEEXTGANSS_SPARE3  14
#define RRC_UMTS_SIB_TYPEEXTGANSS_SPARE2  15
#define RRC_UMTS_SIB_TYPEEXTGANSS_SPARE1  16
typedef struct
{
    U32             t;
}sib_type_ext_ganss_t;

typedef struct
{
    sib_type_ext_ganss_t            sib_type_ext_ganss;
    scheduling_information_t        scheduling_information;
    value_tag_info_t                value_tag_info;
}ext_ganss_scheduling_info_t;

typedef struct
{
    U8                              count;
    ext_ganss_scheduling_info_t     ext_ganss_scheduling_info[MAX_SIB_SB_IN_REF_LIST];
}ext_ganss_sib_type_info_scheduling_info_ganss_scheduling_t;

#define UMTS_EXT_GANSS_SIB_TYPE_INFO_SCHEDULING_INFO_GANSS_ID_PRESENCE_FLAG     0x01
#define UMTS_EXT_GANSS_SIB_TYPE_INFO_SCHEDULING_INFO_SBAS_ID_PRESENCE_FLAG      0x02
typedef struct
{
    U16                     presence_bitmask;
    U8                      ganss_id;
    U32                     ue_positioning_ganss_sbas_id;
    ext_ganss_sib_type_info_scheduling_info_ganss_scheduling_t  ganss_scheduling;
}ext_ganss_sib_type_info_scheduling_info_t;

#define MAX_NUM_OF_GANSS    8
typedef struct
{
    U8                                          count;
    ext_ganss_sib_type_info_scheduling_info_t   ext_ganss_sib_type_info_scheduling_info[MAX_NUM_OF_GANSS];
}ext_ganss_sib_type_info_scheduling_info_list_t;

#define UMTS_EXT_SIB_TYPE_INFO_SCHEDULING_INFO_LIST2_PRESENCE_FLAG        0x01
#define UMTS_EXT_GANSS_SIB_TYPE_INFO_SCHEDULING_INFO_LIST_PRESENCE_FLAG   0x02
#define UMTS_V860_EXT_IE_CSG_INDICATOR_PRESENCE_FLAG                      0x04
typedef struct
{
    U16                                               presence_bitmask;
    ext_sib_type_info_scheduling_info_list2_t         ext_sib_type_info_scheduling_info_list2;
    ext_ganss_sib_type_info_scheduling_info_list_t    ext_ganss_sib_type_info_scheduling_info_list;
    U32                                               csg_indicator;
}mib_v860ext_ies_t;

typedef struct
{
    U8          __dummy__;
}mib_v690_nce_v6b0_nce_v860_nce_nce_t;

#define UMTS_MIB_V690_NCE_V6B0_NCE_V860_NCE_NCE_PRESENCE_FLAG     0x01     
typedef struct
{
    U16                                     presence_bitmask;
    mib_v860ext_ies_t                       mib_v860ext_ies;
    mib_v690_nce_v6b0_nce_v860_nce_nce_t    mib_v690_nce_v6b0_nce_v860_nce_nce;
}mib_v690_nce_v6b0_nce_v860_nce_t;

/********************************************************************
 *                                                                  *
 *  sib_type_ext - Extension SIB type                               *
 *                                                                  *
 *******************************************************************/
#define RRC_UMTS_SIB_TYPEEXT_SYSTEMINFOTYPE11BIS 1
#define RRC_UMTS_SIB_TYPEEXT_SYSTEMINFOTYPE15BIS 2
#define RRC_UMTS_SIB_TYPEEXT_SYSTEMINFOTYPE15_1BIS 3
#define RRC_UMTS_SIB_TYPEEXT_SYSTEMINFOTYPE15_2BIS 4
#define RRC_UMTS_SIB_TYPEEXT_SYSTEMINFOTYPE15_3BIS 5
#define RRC_UMTS_SIB_TYPEEXT_SYSTEMINFOTYPE15_6 6
#define RRC_UMTS_SIB_TYPEEXT_SYSTEMINFOTYPE15_7 7
#define RRC_UMTS_SIB_TYPEEXT_SYSTEMINFOTYPE15_8 8
typedef struct
{
    U32             t;
}sib_type_ext_t;

/********************************************************************
 *                                                                  *
 *  scheduling_info_t - Scheduling information                      *
 *                                                                  *
 *******************************************************************/
typedef struct
{
    scheduling_information_scheduling_t scheduling;
}scheduling_info_t;

typedef struct
{
    sib_type_ext_t          extension_sib_type;
    scheduling_info_t       scheduling_info;
    value_tag_info_t        value_tag_info;
}ext_sib_type_info_scheduling_info_t;

typedef struct
{
    U8                                      count;
    ext_sib_type_info_scheduling_info_t     ext_sib_type_info_scheduling_info[MAX_SIB_SB_IN_REF_LIST];
}ext_sib_type_info_scheduling_info_list_t;

#define UMTS_MIB_EXT_SIB_TYPE_INFO_SCHEDULING_INFO_LIST_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                         presence_bitmask;
    ext_sib_type_info_scheduling_info_list_t    ext_sib_type_info_scheduling_info_list;
}mib_v6b0_ext_ie_t; 

#define UMTS_MIB_V690_NCE_V6B0_NCE_V860_NCE_PRESENCE_FLAG     0x01        
typedef struct
{
    U16                                     presence_bitmask;
    mib_v6b0_ext_ie_t                       mib_v6b0_ext_ie;
    mib_v690_nce_v6b0_nce_v860_nce_t        mib_v690_nce_v6b0_nce_v860_nce;
}mib_v690_nce_v6b0_nce_t;

#define UMTS_MIB_V690_NCE_V6B0_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                     presence_bitmask;
    mib_v690ext_t                           mib_v690ext;
    mib_v690_nce_v6b0_nce_t                 mib_v690_nce_v6b0_nce;
}mib_v690_nce_t;

/********************************************************************
 *                                                                  *
 *  plmn_type_t - Identifies the type of Public Land Mobile         *
 *                Network (PLMN).                                   *
 *                                                                  *
 *******************************************************************/
#define RRC_UMTS_PLMN_TYPE_GSM_MAP        1
#define RRC_UMTS_PLMN_TYPE_ANSI_41        2
#define RRC_UMTS_PLMN_TYPE_GSM_MAP_AND_ANSI_41 3
#define RRC_UMTS_PLMN_TYPE_SPARE          4
typedef struct
{
    U32                                     t;
    union {
        /* t = 1 */
        plmn_type_gsm_map_t                 gsm_map;
        /* t = 2 */
        plmn_type_ansi_41_t                 ansi_41;
        /* t = 3 */
        plmn_type_gsm_map_and_ansi_41_t     gsm_map_and_ansi_41;
        /* t = 4 */
    } u;
}plmn_type_t;

/********************************************************************
 *                                                                  *
 *  sib_sb_type_and_tag_t - SIB and SB type identifies a specific   *
 *                          system information block                *
 *                                                                  *
 *******************************************************************/
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE1 1
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE2 2
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE3 3
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE4 4
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE5 5
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE6 6
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE7 7
#define RRC_UMTS_SIBSB_TYPEANDTAG_DUMMY   8
#define RRC_UMTS_SIBSB_TYPEANDTAG_DUMMY2  9
#define RRC_UMTS_SIBSB_TYPEANDTAG_DUMMY3  10
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE11 11
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE12 12
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE13 13
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE13_1 14
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE13_2 15
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE13_3 16
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE13_4 17
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE14 18
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE15 19
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE16 20
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE17 21
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPESB1 22
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPESB2 23
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE15_1 24
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE15_2 25
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE15_3 26
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE15_4 27
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE18 28
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE15_5 29
#define RRC_UMTS_SIBSB_TYPEANDTAG_SYSINFOTYPE5BIS 30
#define RRC_UMTS_SIBSB_TYPEANDTAG_SPARE2  31
#define RRC_UMTS_SIBSB_TYPEANDTAG_SPARE1  32
typedef struct 
{
    U32  t;
    union {
        /* t = 1 */
        U16   sysInfoType1;
        /* t = 2 */
        U8 sysInfoType2;
        /* t = 3 */
        U8 sysInfoType3;
        /* t = 4 */
        U8 sysInfoType4;
        /* t = 5 */
        U8 sysInfoType5;
        /* t = 6 */
        U8 sysInfoType6;
        /* t = 7 */
        /* t = 8 */
        U8 dummy;
        /* t = 9 */
        /* t = 10 */
        /* t = 11 */
        U8 sysInfoType11;
        /* t = 12 */
        U8 sysInfoType12;
        /* t = 13 */
        U8 sysInfoType13;
        /* t = 14 */
        U8 sysInfoType13_1;
        /* t = 15 */
        U8 sysInfoType13_2;
        /* t = 16 */
        U8 sysInfoType13_3;
        /* t = 17 */
        U8 sysInfoType13_4;
        /* t = 18 */
        /* t = 19 */
        U8 sysInfoType15;
        /* t = 20 */
        predefined_config_identity_value_tag_t sysInfoType16;
        /* t = 21 */
        /* t = 22 */
        U8 sysInfoTypeSB1;
        /* t = 23 */
        U8 sysInfoTypeSB2;
        /* t = 24 */
        U8 sysInfoType15_1;
        /* t = 25 */
        sib_occurrence_identity_value_tag_t   sysInfoType15_2;
        /* t = 26 */
        sib_occurrence_identity_value_tag_t   sysInfoType15_3;
        /* t = 27 */
        U8 sysInfoType15_4;
        /* t = 28 */
        U8 sysInfoType18;
        /* t = 29 */
        U8 sysInfoType15_5;
        /* t = 30 */
        U8 sysInfoType5bis;
        /* t = 31 */
        /* t = 32 */
    } u;
}sib_sb_type_and_tag_t;

/********************************************************************
 *                                                                  *
 *  scheduling_information_sib_sb_t - References to other system    *
 *                             information blocks and scheduling    *
 *                             blocks                               *
 *                                                                  *
 *******************************************************************/
typedef struct
{
    sib_sb_type_and_tag_t       sib_sb_Type;
    scheduling_information_t    scheduling;
}scheduling_information_sib_sb_t;

typedef struct
{
    U8                                      count;
    scheduling_information_sib_sb_t         scheduling_informationi_sib_sb[MAX_SIB_SB_IN_REF_LIST];
}sibsb_reference_list_t;

/********************************************************************/
/*                                                                  */
/*  umts_mib_info_t - Master Information Block                      */
/*                                                                  */
/********************************************************************/
#define UMTS_MIB_NON_CRITICAL_EXTN_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                     presence_bitmask;
    U8                                      mib_value_tag;
    plmn_type_t                             plmn_Type;
    sibsb_reference_list_t                  sibsb_reference_list;
    mib_v690_nce_t                          mib_v690_nce;
}umts_mib_info_t;

#define UMTS_SB1_EXT_SIB_TYPE_INFO_SCHEDULING_INFO_LIST_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                         presence_bitmask;
    ext_sib_type_info_scheduling_info_list_t    ext_sib_type_info_scheduling_info_list;
}sysinfotypesb1_v6b0_ext_t;

#define UMTS_SB1_EXT_SIB_TYPE_INFO_SCHEDULING_INFO_LIST_PRESENCE_FLAG           0x01
#define UMTS_SB1_EXT_GANSS_SIB_TYPE_INFO_SCHEDULING_INFO_LIST_PRESENCE_FLAG     0x02
typedef struct
{
    U16                                             presence_bitmask;
    ext_sib_type_info_scheduling_info_list2_t       ext_sib_type_info_scheduling_info_list2;
    ext_ganss_sib_type_info_scheduling_info_list_t  ext_ganss_sib_type_info_scheduling_info_list;
}sysinfotypesb1_v860_ext_t;

typedef struct
{
    U8          __dummy__;
}sysinfotypesb1_v6b0_nc_extensions_v860_nc_extensions_nc_extensions_t;

/********************************************************************
 *                                                                  *
 *  sysinfotypesb1_v6b0_nc_extensions_v860_nc_extensions_t -        *
 *      References to other extension system information blocks2    *
 *                                                                  *
 *******************************************************************/
#define UMTS_SB1_V6B0_V860_NCE_NCE_PRESENCE_FLAG  0x01
typedef struct
{
    U16                             presence_bitmask;
    sysinfotypesb1_v860_ext_t       sysinfotypesb1_v860_ext;
    sysinfotypesb1_v6b0_nc_extensions_v860_nc_extensions_nc_extensions_t    nc_extensions;
}sysinfotypesb1_v6b0_nc_extensions_v860_nc_extensions_t;

/********************************************************************
 *                                                                  *
 *  sysinfotypesb1_v6b0_nc_extensions_t - References to other       *
 *                              extension system information blocks *
 *                                                                  *
 *******************************************************************/
#define UMTS_SB1_V6B0_V860_NCE_PRESENCE_FLAG     0x01
typedef struct
{
    U16                             presence_bitmask;
    sysinfotypesb1_v6b0_ext_t       sysinfotypesb1_v6b0_ext;
    sysinfotypesb1_v6b0_nc_extensions_v860_nc_extensions_t  v860_nc_extensions;
}sysinfotypesb1_v6b0_nc_extensions_t;

/********************************************************************
 *                                                                  *
 *  scheduling_information_sib_sb_t - References to other system    *
 *                             information blocks and scheduling    *
 *                             blocks                               *
 *                                                                  *
 *******************************************************************/
typedef struct
{
    sib_type_and_tag_t                      sib_Type;
    scheduling_information_t                scheduling;
}scheduling_information_sib_t;

/********************************************************************
 *                                                                  *
 *  sib_reference_list_t - References to other system information   *
 *                         blocks                                   *
 *                                                                  *
 *******************************************************************/
typedef struct
{
    U8                                      count;
    scheduling_information_sib_t            scheduling_informationi_sib[MAX_SIB_IN_REF_LIST];
}sib_reference_list_t;

/********************************************************************/
/*                                                                  */
/*  umts_scheduling_block_1_info_t - Scheduling Block 1             */
/*                                                                  */
/********************************************************************/
#define UMTS_SB1_V6B0_NC_EXTN_PRESENCE_FLAG  0x01
typedef struct
{
    U16                                 presence_bitmask;
    sib_reference_list_t                sib_reference_list;
    sysinfotypesb1_v6b0_nc_extensions_t v6b0_nc_extensions; 
}umts_scheduling_block_1_info_t;

/********************************************************************
 *                                                                  *
 *  sysinfotypesb2_v6b0_ext_t - References to other extension       *
 *                              system information blocks           *
 *                                                                  *
 *******************************************************************/
#define UMTS_SB2_EXT_SIB_TYPE_INFO_SCHEDULING_INFO_LIST_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                         presence_bitmask;
    ext_sib_type_info_scheduling_info_list_t    ext_sib_type_info_scheduling_info_list;
}sysinfotypesb2_v6b0_ext_t;

#define UMTS_SB2_EXT_SIB_TYPE_INFO_SCHEDULING_INFO_LIST2_PRESENCE_FLAG          0x01
#define UMTS_SB2_EXT_GANSS_SIB_TYPE_INFO_SCHEDULING_INFO_LIST_PRESENCE_FLAG     0x02
typedef struct
{
    U16                 presence_bitmask;
    ext_sib_type_info_scheduling_info_list2_t       ext_sib_type_info_scheduling_info_list2;
    ext_ganss_sib_type_info_scheduling_info_list_t  ext_ganss_sib_type_info_scheduling_info_list;
}sysinfotypesb2_v860_ext_t;

typedef struct
{
    U8          __dummy__;
}sysinfotypesb2_v6b0_nc_extensions_v860_nc_extensions_nc_extensions_t;

/********************************************************************
 *                                                                  *
 *  sysinfotypesb2_v6b0_nc_extensions_v860_nc_extensions_t -        *
 *      References to other extension system information blocks2    *
 *                                                                  *
 *******************************************************************/
#define UMTS_SB2_V6B0_V860_NCE_NCE_PRESENCE_FLAG  0x01
typedef struct
{
    U16                                 presence_bitmask;
    sysinfotypesb2_v860_ext_t           sysinfotypesb2_v860_ext;
    sysinfotypesb2_v6b0_nc_extensions_v860_nc_extensions_nc_extensions_t    nc_extensions;
}sysinfotypesb2_v6b0_nc_extensions_v860_nc_extensions_t;

#define UMTS_SB2_V6B0_V860_NCE_PRESENCE_FLAG     0x01
typedef struct
{
    U16                                 presence_bitmask;
    sysinfotypesb2_v6b0_ext_t           sysinfotypesb2_v6b0_ext;
    sysinfotypesb2_v6b0_nc_extensions_v860_nc_extensions_t  v860_nc_extensions;
}sysinfotypesb2_v6b0_nc_extensions_t;

/********************************************************************/
/*                                                                  */
/*  umts_scheduling_block_2_info_t                                  */
/*                                                                  */
/********************************************************************/
#define UMTS_SB2_V6B0_NC_EXTN_PRESENCE_FLAG  0x01
typedef struct
{
    U16                                 presence_bitmask;
    sib_reference_list_t                sib_reference_list;
    sysinfotypesb2_v6b0_nc_extensions_t v6b0_nc_extensions; 
}umts_scheduling_block_2_info_t;

typedef struct
{
    U32         n;
    U8          elem[32];
}removed_intra_freq_cell_list_remove_some_intra_freq_cells_t;

#define RRC_UMTS_REMOVEDINTRAFREQCELLLIST_REMOVEALLINTRAFREQCELLS 1
#define RRC_UMTS_REMOVEDINTRAFREQCELLLIST_REMOVESOMEINTRAFREQCELLS 2
#define RRC_UMTS_REMOVEDINTRAFREQCELLLIST_REMOVENOINTRAFREQCELLS 3
typedef struct
{
    U32             t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        removed_intra_freq_cell_list_remove_some_intra_freq_cells_t    remove_some_intra_freq_cells;
        /* t = 3 */
    }u;
}removed_intra_freq_cell_list_t;

#define RRC_UMTS_REFERENCETIMEDIFFERENCETOCELL_ACCURACY40 1
#define RRC_UMTS_REFERENCETIMEDIFFERENCETOCELL_ACCURACY256 2
#define RRC_UMTS_REFERENCETIMEDIFFERENCETOCELL_ACCURACY2560 3
typedef struct
{
    U32                 t;
    union
    {
        /* t = 1 */
        U16             accuracy40;
        /* t = 2 */
        U8              accuracy256;
        /* t = 3 */
        U8              accuracy2560;
    }u;
}reference_time_difference_to_cell_t;

typedef struct
{
    U16     primary_scrambling_code;
}primary_cpich_info_t;

#define UMTS_SIB_PRIMARY_CPICH_INFO_PRESENCE_FLAG       0x01
#define UMTS_SIB_PRIMARY_CPICH_TX_POWER_PRESENCE_FLAG   0x02
typedef struct
{
    U16                         presence_bitmask;
    primary_cpich_info_t        primary_cpich_info;
    S8                          primary_cpich_tx_power;
    U8                          read_sfn_indicator;
    U8                          tx_diversity_indicator; 
}cell_info_si_rscp_mode_specific_info_fdd_t;

typedef struct
{
    U8              timeslot_number;
    U32             burst_type;
}timeslot_info_t;

typedef struct
{
    U8                  count;
    timeslot_info_t     timeslot_info[MAX_NO_OF_TIMESLOTS];
}timeslot_info_list_t;

#define UMTS_SIB_PRIMARY_CCPCH_TX_POWER_PRESENCE_FLAG       0x01
#define UMTS_SIB_TIME_SLOT_INFO_LIST_PRESENCE_FLAG          0x02
typedef struct
{
    U16                     presence_bitmask;
    primary_ccpch_info_t    primary_ccpch_info;
    U8                      primary_ccpch_tx_power;
    timeslot_info_list_t    timeslot_info_list;
    U8                      read_sfn_indicator;
}cell_info_si_rscp_mode_specific_info_tdd_t;

#define RRC_UMTS_CELLINFOSI_RSCP_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_CELLINFOSI_RSCP_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32                 t;
    union
    {
        cell_info_si_rscp_mode_specific_info_fdd_t  fdd;
        cell_info_si_rscp_mode_specific_info_tdd_t  tdd;
    }u;
}cell_info_si_rscp_mode_specific_info_t;

#define UMTS_SIB_MODE_SPECIFIC_INFO_FDD_Q_QUALMIN_PRESENCE_FLAG        0x01
#define UMTS_SIB_MODE_SPECIFIC_INFO_FDD_Q_RX_LEV_MIN_PRESENCE_FLAG     0x02
typedef struct
{
    U16     presence_bitmask;
    S8      q_qual_min;
    S8      q_rx_lev_min;
}cell_select_reselect_info_sib_11_12_rscp_mode_specific_info_fdd_t;

#define UMTS_SIB_MODE_SPECIFIC_INFO_TDD_Q_RX_LEV_MIN_PRESENCE_FLAG     0x01
typedef struct
{
    U16     presence_bitmask;
    S8      q_rx_lev_min;
}cell_select_reselect_info_sib_11_12_rscp_mode_specific_info_tdd_t;

#define UMTS_SIB_MODE_SPECIFIC_INFO_GSM_Q_RX_LEV_MIN_PRESENCE_FLAG     0x01
typedef struct
{
    U16     presence_bitmask;
    S8      q_rx_lev_min;
}cell_select_reselect_info_sib_11_12_rscp_mode_specific_info_gsm_t;

#define RRC_UMTS_CELLSELECTRESELECTINFOSIB_11_12_RSCP_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_CELLSELECTRESELECTINFOSIB_11_12_RSCP_MODESPECIFICINFO_TDD 2
#define RRC_UMTS_CELLSELECTRESELECTINFOSIB_11_12_RSCP_MODESPECIFICINFO_GSM 3
typedef struct
{
    U32             t;
    union
    {
        cell_select_reselect_info_sib_11_12_rscp_mode_specific_info_fdd_t   fdd;
        cell_select_reselect_info_sib_11_12_rscp_mode_specific_info_tdd_t   tdd;
        cell_select_reselect_info_sib_11_12_rscp_mode_specific_info_gsm_t   gsm;
    }u;
}cell_select_reselect_info_sib_11_12_rscp_mode_specific_info_t;

#define UMTS_SIB_MAX_ALLOWED_UL_TX_POWER_PRESENCE_FLAG  0x01
typedef struct
{
    U16         presence_bitmask;
    S8          q_offset_s_n;
    S8          max_allowed_ul_tx_power;
    cell_select_reselect_info_sib_11_12_rscp_mode_specific_info_t   mode_specific_info;
}cell_select_reselect_info_sib_11_12_rscp_t;

#define UMTS_SIB_REFERENCE_TIME_DIFF_TO_CELL_PRESENCE_FLAG      0x01
#define UMTS_SIB_CELL_SELECTION_RESELECTION_INFO_PRESENCE_FLAG  0x02
typedef struct
{
    U16                                         presence_bitmask;
    S8                                          cell_individual_offset;
    reference_time_difference_to_cell_t         reference_time_difference_to_cell;
    cell_info_si_rscp_mode_specific_info_t      mode_specific_info;
    cell_select_reselect_info_sib_11_12_rscp_t  cell_select_reselect_info_sib_11_12_rscp;    
}cell_info_si_rscp_t;

#define UMTS_SIB_NEW_INTRA_FREQ_CELL_SI_RSCP_INTRA_FREQ_CELL_ID_PRESENCE_FLAG   0x01
typedef struct
{
    U16                     presence_bitmask;
    U8                      intra_freq_cell_id;
    cell_info_si_rscp_t     cell_info_si_rscp; 
}new_intra_freq_cell_si_rscp_t;

typedef struct
{
    U8                              count;
    new_intra_freq_cell_si_rscp_t   new_intra_freq_cell_si_rscp[MAX_CELL_MEAS];
}new_intra_freq_cell_si_list_rscp_t;

#define UMTS_SIB_INTRA_FREQ_CELL_SI_LIST_RSCP_REMOVED_CELL_LIST_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                 presence_bitmask;
    removed_intra_freq_cell_list_t      removed_intra_freq_cell_list;
    new_intra_freq_cell_si_list_rscp_t  new_intra_freq_cell_si_list_rscp;
}intra_freq_cell_info_si_list_rscp_t;

typedef struct
{
    U32         n;
    U32         elem[4];
}intra_freq_meas_quantity_tdd_list_t;

typedef struct
{
    intra_freq_meas_quantity_tdd_list_t intra_freq_meas_quantity_tdd_list;
}intra_freq_meas_quantity_mode_specific_info_tdd_t;

typedef struct
{
    U32    intra_freq_meas_quantity_fdd; 
}intra_freq_meas_quantity_mode_specific_info_fdd_t;

#define RRC_UMTS_INTRAFREQMEASQUANTITY_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_INTRAFREQMEASQUANTITY_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32                             t;
    union
    {
        intra_freq_meas_quantity_mode_specific_info_fdd_t   fdd;
        intra_freq_meas_quantity_mode_specific_info_tdd_t   tdd;
    }u;
}intra_freq_meas_quantity_mode_specific_info_t;

typedef struct
{
    U32     filter_coefficient;
    intra_freq_meas_quantity_mode_specific_info_t   mode_specific_info;
}intra_freq_meas_quantity_t;

typedef struct
{
    U32             intra_freq_reporting_quantity_for_rach_fdd;
}intra_freq_reporting_quantity_for_rach_mode_specific_info_fdd_t;

typedef struct
{
    U32         n;
    U32         elem[2];
}intra_freq_reporting_quantity_for_rach_tdd_list_t;

typedef struct
{
    intra_freq_reporting_quantity_for_rach_tdd_list_t   intra_freq_reporting_quantity_for_rach_tdd_list; 
}intra_freq_reporting_quantity_for_rach_mode_specific_info_tdd_t;

#define RRC_UMTS_INTRAFREQREPORTINGQUANTITYFORRACH_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_INTRAFREQREPORTINGQUANTITYFORRACH_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32                         t;
    union
    {
        intra_freq_reporting_quantity_for_rach_mode_specific_info_fdd_t fdd;
        intra_freq_reporting_quantity_for_rach_mode_specific_info_tdd_t tdd; 
    }u;
}intra_freq_reporting_quantity_for_rach_mode_specific_info_t;

typedef struct
{
    U32             sfn_sfn_otd_type;
    intra_freq_reporting_quantity_for_rach_mode_specific_info_t mode_specific_info;
}intra_freq_reporting_quantity_for_rach_t;

typedef struct
{
    U8              timeslot_iscp_reporting_indicator;
    U8              proposed_tgsn_reporting_required;
    U8              primary_ccpch_rscp_reporting_indicator;
    U8              pathloss_reporting_indicator;
}cell_reporting_quantities_mode_specific_info_tdd_t;

typedef struct
{
    U8              cpich_ec_no_reporting_indicator;
    U8              cpich_rscp_reporting_indicator;
    U8              pathloss_reporting_indicator; 
}cell_reporting_quantities_mode_specific_info_fdd_t;

#define RRC_UMTS_CELLREPORTINGQUANTITIES_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_CELLREPORTINGQUANTITIES_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32                     t;
    union
    {
        cell_reporting_quantities_mode_specific_info_fdd_t  fdd;
        cell_reporting_quantities_mode_specific_info_tdd_t  tdd;
    }u;
}cell_reporting_quantities_mode_specific_info_t;

typedef struct
{
    U32                 dummy;
    U8                  cell_identity_reporting_indicator;
    U8                  cell_sync_info_reporting_indicator;
    cell_reporting_quantities_mode_specific_info_t  cell_reporting_quantities_mode_specific_info;
}cell_reporting_quantities_t;

#define UMTS_SIB_INTRA_FREQ_REPORTING_QTY_DETECTED_SET_REPORTING_QUANTITIES_PRESENCE_FLAG      0x01
typedef struct
{
    U16                                 presence_bitmask;
    cell_reporting_quantities_t         active_set_reporting_quantities;
    cell_reporting_quantities_t         monitored_set_reporting_quantities;
    cell_reporting_quantities_t         detected_set_reporting_quantities; 
}intra_freq_reporting_quantity_t;

typedef struct
{
    U32                 measurement_report_transfer_mode;
    U32                 periodical_or_event_trigger;
}measurement_reporting_mode_t;

#define RRC_UMTS_FORBIDDENAFFECTCELL_FDD  1
#define RRC_UMTS_FORBIDDENAFFECTCELL_TDD  2
typedef struct
{
    U32                 t;
    union
    {
        primary_cpich_info_t    fdd;
        primary_ccpch_info_t    tdd;
    }u;
}forbidden_affect_cell_t;

typedef struct
{
    U8                      count;
    forbidden_affect_cell_t forbidden_affect_cell[MAX_CELL_MEAS];
}forbidden_affect_cell_list_t;

#define UMTS_SIB_EVENT_1A_FORBIDDEN_AFFECT_CELL_LIST_PRESENCE_FLAG   0x01
typedef struct
{
    U16                 presence_bitmask;
    U32                 triggering_condition;
    U8                  reporting_range;
    forbidden_affect_cell_list_t    forbidden_affect_cell_list;
    U8                  w;
    U32                 report_deactivation_threshold;
    U32                 reporting_amount;
    U32                 reporting_interval;
}event_1a_t;

#define UMTS_SIB_EVENT_1B_FORBIDDEN_AFFECT_CELL_LIST_PRESENCE_FLAG   0x01
typedef struct
{
    U16                 presence_bitmask;
    U32                 triggering_condition;
    U8                  reporting_range;
    forbidden_affect_cell_list_t    forbidden_affect_cell_list;
    U8                  w;
}event_1b_t;

typedef struct
{
    U32     replacement_activation_threshold;
    U32     reporting_amount;
    U32     reporting_interval;
}event_1c_t;

typedef struct
{
    U32             triggering_condition;
    U16             threshold_used_freq;
}event_1e_t;

typedef struct
{
    U32             triggering_condition;
    U16             threshold_used_freq;
}event_1f_t;

#define RRC_UMTS_INTRAFREQEVENT_E1A       1
#define RRC_UMTS_INTRAFREQEVENT_E1B       2
#define RRC_UMTS_INTRAFREQEVENT_E1C       3
#define RRC_UMTS_INTRAFREQEVENT_E1D       4
#define RRC_UMTS_INTRAFREQEVENT_E1E       5
#define RRC_UMTS_INTRAFREQEVENT_E1F       6
#define RRC_UMTS_INTRAFREQEVENT_E1G       7
#define RRC_UMTS_INTRAFREQEVENT_E1H       8
#define RRC_UMTS_INTRAFREQEVENT_E1I       9
typedef struct
{
    U32                 t;
    union
    {
        /* t = 1 */
        event_1a_t      e1a;
        /* t = 2 */
        event_1b_t      e1b;
        /* t = 3 */
        event_1c_t      e1c;
        /* t = 4 */
        /* t = 5 */
        event_1e_t      e1e;
        /* t = 6 */
        event_1f_t      e1f;
        /* t = 7 */
        /* t = 8 */
        U16             e1h;
        /* t = 9 */
        U16             e1i;
    }u;
}intra_freq_event_t;

#define RRC_UMTS_REPORTINGCELLSTATUS_WITHINACTIVESET 1
#define RRC_UMTS_REPORTINGCELLSTATUS_WITHINMONITOREDSETUSEDFREQ 2
#define RRC_UMTS_REPORTINGCELLSTATUS_WITHINACTIVEANDORMONITOREDUSEDFREQ 3
#define RRC_UMTS_REPORTINGCELLSTATUS_WITHINDETECTEDSETUSEDFREQ 4
#define RRC_UMTS_REPORTINGCELLSTATUS_WITHINMONITOREDANDORDETECTEDUSEDFREQ 5
#define RRC_UMTS_REPORTINGCELLSTATUS_ALLACTIVEPLUSMONITOREDSET 6
#define RRC_UMTS_REPORTINGCELLSTATUS_ALLACTIVEPLUSDETECTEDSET 7
#define RRC_UMTS_REPORTINGCELLSTATUS_ALLACTIVEPLUSMONITOREDANDORDETECTEDSET 8
#define RRC_UMTS_REPORTINGCELLSTATUS_WITHINVIRTUALACTSET 9
#define RRC_UMTS_REPORTINGCELLSTATUS_WITHINMONITOREDSETNONUSEDFREQ 10
#define RRC_UMTS_REPORTINGCELLSTATUS_WITHINMONITOREDANDORVIRTUALACTIVESETNONUSEDFREQ 11
#define RRC_UMTS_REPORTINGCELLSTATUS_ALLVIRTUALACTSETPLUSMONITOREDSETNONUSEDFREQ 12
#define RRC_UMTS_REPORTINGCELLSTATUS_WITHINACTSETORVIRTUALACTSET_INTERRATCELLS 13
#define RRC_UMTS_REPORTINGCELLSTATUS_WITHINACTSETANDORMONITOREDUSEDFREQORVIRTUALACTSETANDORMONITOREDNONUSEDFREQ 14
typedef struct
{
    U32         t;
    union
    {
        U32     within_active_set;
        U32     within_monitored_set_used_freq;
        U32     within_active_and_or_monitored_used_freq;
        U32     within_detected_set_used_freq;
        U32     within_monitored_and_or_detected_used_freq;
        U32     all_active_plus_monitored_set;
        U32     all_active_plus_detected_set;
        U32     all_active_plus_monitored_and_or_detected_set;
        U32     within_virtual_act_set;
        U32     within_monitored_set_non_used_freq;
        U32     within_monitored_and_or_virtual_active_set_non_used_freq;
        U32     all_virtual_act_set_plus_monitored_set_non_used_freq;
        U32     within_act_set_or_virtual_act_set_inter_rat_cells;
        U32     within_act_set_and_or_monitored_used_freq_or_virtual_act_set_and_or_monitored_non_used_freq;
    }u;
}reporting_cell_status_t;

#define UMTS_SIB_REPORTING_CELL_STATUS_PRESENCE_FLAG    0x01
typedef struct
{
    U16                     presence_bitmask;
    intra_freq_event_t      event;
    U8                      hysteresis;
    U32                     time_to_trigger;
    reporting_cell_status_t reporting_cell_status;
}intra_freq_event_criteria_t;

typedef struct
{
    U8                          count;
    intra_freq_event_criteria_t intra_freq_event_criteria[MAX_MEAS_EVENT];
}intra_freq_event_criteria_list_t;

#define UMTS_SIB_INTRA_FREQ_EVENT_CRITERIA_LIST_PRESENCE_FLAG       0x01
typedef struct
{
    U16                                 presence_bitmask;
    intra_freq_event_criteria_list_t    intra_freq_event_criteria_list;
}intra_freq_reporting_criteria_t;

typedef struct
{
    U32             reporting_amount;
    U32             reporting_interval;
}periodical_reporting_criteria_t;

#define RRC_UMTS_CELLDCH_REPORTCRITERIA_INTRAFREQREPORTINGCRITERIA 1
#define RRC_UMTS_CELLDCH_REPORTCRITERIA_PERIODICALREPORTINGCRITERIA 2
typedef struct
{
    U32                     t;
    union
    {
        intra_freq_reporting_criteria_t     intra_freq_reporting_criteria;
        periodical_reporting_criteria_t     periodical_reporting_criteria;
    }u;
}cell_dch_report_criteria_t;

typedef struct
{
    intra_freq_reporting_quantity_t         intra_freq_reporting_quantity;
    measurement_reporting_mode_t            measurement_reporting_mode;
    cell_dch_report_criteria_t              report_criteria;
}reporting_info_for_cell_dch_t;

#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_RSCP_SI_LIST_PRESENCE_FLAG                     0x01
#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_RSCP_MEAS_QTY_PRESENCE_FLAG                    0x02
#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_RSCP_REPORTING_QTY_FOR_RACH_PRESENCE_FLAG      0x04
#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_RSCP_MAX_REPORTED_CELLS_ON_RACH_PRESENCE_FLAG  0x08
#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_RSCP_REPORTING_INFO_FOR_CELL_DCH_PRESENCE_FLAG 0x10
typedef struct
{
    U16                                 presence_bitmask;
    U8                                  intra_freq_meas_id;
    intra_freq_cell_info_si_list_rscp_t intra_freq_cell_info_si_list_rscp;
    intra_freq_meas_quantity_t          intra_freq_meas_quantity;
    intra_freq_reporting_quantity_for_rach_t    intra_freq_reporting_quantity_for_rach;
    U32                                 max_reported_cells_on_rach;
    reporting_info_for_cell_dch_t       reporting_info_for_cell_dch;
}intra_freq_measurement_sysinfo_rscp_t;

typedef struct
{
    U32             n;
    U8              elem[32];
}removed_inter_freq_cell_list_remove_some_inter_freq_cells_t;

#define RRC_UMTS_REMOVEDINTERFREQCELLLIST_REMOVEALLINTERFREQCELLS 1
#define RRC_UMTS_REMOVEDINTERFREQCELLLIST_REMOVESOMEINTERFREQCELLS 2
#define RRC_UMTS_REMOVEDINTERFREQCELLLIST_REMOVENOINTERFREQCELLS 3
typedef struct
{
    U32             t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        removed_inter_freq_cell_list_remove_some_inter_freq_cells_t remove_some_inter_freq_cells;
        /* t = 3 */
    }u;
}removed_inter_freq_cell_list_t;

#define UMTS_SIB_FREQ_INFO_FDD_UARFCN_UL_PRESENCE_FLAG  0x01
typedef struct
{
    U16             presence_bitmask;
    U16             uarfcn_ul;
    U16             uarfcn_dl;
}frequency_info_fdd_t;

#define RRC_UMTS_FREQUENCYINFO_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_FREQUENCYINFO_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32                 t;
    union
    {
        frequency_info_fdd_t    fdd;
        frequency_info_tdd_t    tdd;
    }u;
}frequency_info_mode_specific_info_t;

typedef struct
{
    frequency_info_mode_specific_info_t mode_specific_info;
}frequency_info_t;

#define UMTS_SIB_NEW_INTER_FREQ_CELL_SI_RSCP_INTER_FREQ_CELL_ID_PRESENCE_FLAG   0x01
#define UMTS_SIB_NEW_INTER_FREQ_CELL_SI_RSCP_INTER_FREQ_INFO_PRESENCE_FLAG      0x02
typedef struct
{
    U16                 presence_bitmask;
    U8                  inter_freq_cell_id;
    frequency_info_t    frequency_info;
    cell_info_si_rscp_t cell_info;
}new_inter_freq_cell_si_rscp_t;

typedef struct
{
    U8                              count;
    new_inter_freq_cell_si_rscp_t   new_inter_freq_cell_si_rscp[MAX_CELL_MEAS];
}new_inter_freq_cell_si_list_rscp_t;

#define UMTS_SIB_INTER_FREQ_CELL_INFO_SI_LIST_RSCP_REMOVED_LIST_PRESENCE_FLAG   0x01
#define UMTS_SIB_INTER_FREQ_CELL_INFO_SI_LIST_RSCP_NEW_LIST_PRESENCE_FLAG       0x02
typedef struct
{
    U16                                 presence_bitmask;
    removed_inter_freq_cell_list_t      removed_inter_freq_cell_list;
    new_inter_freq_cell_si_list_rscp_t  new_inter_freq_cell_si_list_rscp;
}inter_freq_cell_info_si_list_rscp_t;

#define UMTS_SIB_INTER_FREQ_MEAS_SYSINFO_RSCP_CELL_INFO_SI_LIST_PRESENCE_FLAG     0x01
typedef struct
{
    U16                                 presence_bitmask;
    inter_freq_cell_info_si_list_rscp_t inter_freq_cell_info_si_list_rscp; 
}inter_freq_measurement_sysinfo_rscp_t;

#define UMTS_SIB_TYPE_11_MEAS_CTRL_SYSINFO_USE_OF_HCS_NOT_USED_CELL_SELECT_QLTY_MEAS_CPICH_RSCP_INTRA_FREQ_PRESENCE_FLAG     0x01
#define UMTS_SIB_TYPE_11_MEAS_CTRL_SYSINFO_USE_OF_HCS_NOT_USED_CELL_SELECT_QLTY_MEAS_CPICH_RSCP_INTER_FREQ_PRESENCE_FLAG     0x02
typedef struct
{
    U16                                     presence_bitmask;
    intra_freq_measurement_sysinfo_rscp_t   intra_freq_measurement_sysinfo_rscp;
    inter_freq_measurement_sysinfo_rscp_t   inter_freq_measurement_sysinfo_rscp;
}measurement_control_sys_info_use_of_hcs_hcs_not_used_cell_select_quality_measure_cpich_rscp_t;

#define UMTS_SIB_CELL_INFO_SI_ECNO_MODE_SPECIFIC_INFO_FDD_PRIMARY_CPICH_INFO_PRESENCE_FLAG      0x01
#define UMTS_SIB_CELL_INFO_SI_ECNO_MODE_SPECIFIC_INFO_FDD_PRIMARY_CPICH_TX_PWR_PRESENCE_FLAG    0x02
typedef struct
{
    U16                             presence_bitmask;
    primary_cpich_info_t            primary_cpich_info;
    S8                              primary_cpich_tx_power;
    U8                              read_sfn_indicator;
    U8                              tx_diversity_indicator;
}cell_info_si_ecno_mode_specific_info_fdd_t;

#define UMTS_SIB_CELL_INFO_SI_ECNO_MODE_SPECIFIC_INFO_TDD_PRIMARY_CCPCH_TX_PWR_PRESENCE_FLAG    0x01
#define UMTS_SIB_CELL_INFO_SI_ECNO_MODE_SPECIFIC_INFO_TDD_TIME_SLOT_INFO_LIST_PRESENCE_FLAG     0x02
typedef struct
{
    U16                             presence_bitmask;
    primary_ccpch_info_t            primary_ccpch_info;
    U8                              primary_ccpch_tx_power;
    timeslot_info_list_t            timeslot_info_list;
    U8                              read_sfn_indicator;
}cell_info_si_ecno_mode_specific_info_tdd_t;

#define RRC_UMTS_CELLINFOSI_ECN0_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_CELLINFOSI_ECN0_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32                     t;
    union
    {
        cell_info_si_ecno_mode_specific_info_fdd_t  fdd;
        cell_info_si_ecno_mode_specific_info_tdd_t  tdd;
    }u;
}cell_info_si_ecno_mode_specific_info_t;

#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_SIB_11_12_ECNO_MODE_SPECIFIC_INFO_FDD_Q_QUAL_MIN_PRESENCE_FLAG   0x01
#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_SIB_11_12_ECNO_MODE_SPECIFIC_INFO_FDD_Q_RX_LEV_MIN_PRESENCE_FLAG 0x02
typedef struct
{
    U16             presence_bitmask;
    S8              q_qual_min;
    S8              q_rx_lev_min;
}cell_select_reselect_info_sib_11_12_ecno_mode_specific_info_fdd_t;

#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_SIB_11_12_ECNO_MODE_SPECIFIC_INFO_TDD_Q_RX_LEV_MIN_PRESENCE_FLAG 0x01
typedef struct
{
    U16             presence_bitmask;
    S8              q_rx_lev_min;
}cell_select_reselect_info_sib_11_12_ecno_mode_specific_info_tdd_t;

#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_SIB_11_12_ECNO_MODE_SPECIFIC_INFO_GSM_Q_RX_LEV_MIN_PRESENCE_FLAG 0x01
typedef struct
{
    U16             presence_bitmask;
    S8              q_rx_lev_min;
}cell_select_reselect_info_sib_11_12_ecno_mode_specific_info_gsm_t;

#define RRC_UMTS_CELLSELECTRESELECTINFOSIB_11_12_ECN0_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_CELLSELECTRESELECTINFOSIB_11_12_ECN0_MODESPECIFICINFO_TDD 2
#define RRC_UMTS_CELLSELECTRESELECTINFOSIB_11_12_ECN0_MODESPECIFICINFO_GSM 3
typedef struct
{
    U32                     t;
    union
    {
        cell_select_reselect_info_sib_11_12_ecno_mode_specific_info_fdd_t   fdd;
        cell_select_reselect_info_sib_11_12_ecno_mode_specific_info_tdd_t   tdd;
        cell_select_reselect_info_sib_11_12_ecno_mode_specific_info_gsm_t   gsm;
    }u;
}cell_select_reselect_info_sib_11_12_ecno_mode_specific_info_t;

#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_SIB_11_12_ECNO_MAX_ALLOWED_UL_TX_PWR_PRESENCE_FLAG   0x01
typedef struct
{
    U16                 presence_bitmask;
    S8                  q_offset_1s_n;
    S8                  q_offset_2s_n;
    S8                  max_allowed_ul_tx_power;
    cell_select_reselect_info_sib_11_12_ecno_mode_specific_info_t   mode_specific_info;
}cell_select_reselect_info_sib_11_12_ecno_t;

#define UMTS_SIB_CELL_INFO_SI_ECNO_REF_TIME_DIFF_TO_CELL_PRESENCE_FLAG      0x01
#define UMTS_SIB_CELL_INFO_SI_ECNO_CELL_SELECT_RESELECT_INFO_PRESENCE_FLAG  0x02
typedef struct
{
    U16                                     presence_bitmask;
    S8                                      cell_individual_offset;
    reference_time_difference_to_cell_t     reference_time_difference_to_cell;
    cell_info_si_ecno_mode_specific_info_t  mode_specific_info;
    cell_select_reselect_info_sib_11_12_ecno_t  cell_select_reselect_info;
}cell_info_si_ecno_t;

#define UMTS_SIB_NEW_INTRA_FREQ_CELL_SI_ECNO_INTRA_FREQ_CELL_ID_PRESENCE_FLAG   0x01
typedef struct
{
    U16                 presence_bitmask;
    U8                  intra_freq_cell_id;
    cell_info_si_ecno_t cell_info_si_ecno;
}new_intra_freq_cell_si_ecno_t;

typedef struct
{
    U8                              count;
    new_intra_freq_cell_si_ecno_t   new_intra_freq_cell_si_ecno[MAX_CELL_MEAS];
}new_intra_freq_cell_si_list_ecno_t;

#define UMTS_SIB_INTRA_FREQ_CELL_INFO_SI_LIST_ECNO_REMOVED_INTRA_FREQ_CELL_LIST_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                     presence_bitmask;
    removed_intra_freq_cell_list_t          removed_intra_freq_cell_list;
    new_intra_freq_cell_si_list_ecno_t      new_intra_freq_cell_si_list_ecno;
}intra_freq_cell_info_si_list_ecno_t;

#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_ECNO_CELL_INFO_SI_LIST_PRESENCE_FLAG           0x01
#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_ECNO_MEAS_QTY_PRESENCE_FLAG                    0x02
#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_ECNO_REPORTING_QTY_FOR_RACH_PRESENCE_FLAG      0x04
#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_ECNO_MAX_REPORTED_CELLS_ON_RACH_PRESENCE_FLAG  0x08
#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_ECNO_REPORTING_INFO_FOR_CELL_DCH_PRESENCE_FLAG 0x10
typedef struct
{
    U16                                     presence_bitmask;
    U8                                      intra_freq_meas_id;
    intra_freq_cell_info_si_list_ecno_t     intra_freq_cell_info_si_list_ecno;
    intra_freq_meas_quantity_t              intra_freq_meas_quantity;
    intra_freq_reporting_quantity_for_rach_t    intra_freq_reporting_quantity_for_rach;
    U32                                     max_reported_cells_on_rach;
    reporting_info_for_cell_dch_t           reporting_info_for_cell_dch;
}intra_freq_measurement_sysinfo_ecno_t;

#define UMTS_SIB_NEW_INTER_FREQ_CELL_SI_ECNO_INTER_FREQ_CELL_ID_PRESENCE_FLAG   0x01
#define UMTS_SIB_NEW_INTER_FREQ_CELL_SI_ECNO_INTER_FREQ_FREQ_INFO_PRESENCE_FLAG 0x02
typedef struct
{
    U16                     presence_bitmask;
    U8                      inter_freq_cell_id;
    frequency_info_t        frequency_info;
    cell_info_si_ecno_t     cell_info_si_ecno;
}new_inter_freq_cell_si_ecno_t;

typedef struct
{
    U8                              count;
    new_inter_freq_cell_si_ecno_t   new_inter_freq_cell_si_ecno[MAX_CELL_MEAS];
}new_inter_freq_cell_si_list_ecno_t;

#define UMTS_SIB_INTER_FREQ_CELL_INFO_SI_LIST_ECNO_REMOVED_INTER_FREQ_CELL_LIST_PRESENCE_FLAG       0x01
#define UMTS_SIB_INTER_FREQ_CELL_INFO_SI_LIST_ECNO_NEW_INTER_FREQ_CELL_SI_LIST_ECNO_PRESENCE_FLAG   0x02
typedef struct
{
    U16                                 presence_bitmask;
    removed_inter_freq_cell_list_t      removed_inter_freq_cell_list;
    new_inter_freq_cell_si_list_ecno_t  new_inter_freq_cell_si_list_ecno;
}inter_freq_cell_info_si_list_ecno_t;

#define UMTS_SIB_INTER_FREQ_CELL_INFO_SI_LIST_ECNO_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                 presence_bitmask;
    inter_freq_cell_info_si_list_ecno_t inter_freq_cell_info_si_list_ecno;
}inter_freq_measurement_sysinfo_ecno_t;

#define UMTS_SIB_TYPE_11_MEAS_CTRL_SYSINFO_USE_OF_HCS_NOT_USED_CELL_SELECT_QLTY_MEAS_CPICH_EC_NO_INTRA_FREQ_PRESENCE_FLAG     0x01
#define UMTS_SIB_TYPE_11_MEAS_CTRL_SYSINFO_USE_OF_HCS_NOT_USED_CELL_SELECT_QLTY_MEAS_CPICH_EC_NO_INTER_FREQ_PRESENCE_FLAG     0x02
typedef struct
{
    U16                                     presence_bitmask;
    intra_freq_measurement_sysinfo_ecno_t   intra_freq_measurement_sysinfo_ecno;
    inter_freq_measurement_sysinfo_ecno_t   inter_freq_measurement_sysinfo_ecno; 
}measurement_control_sys_info_use_of_hcs_hcs_not_used_cell_select_quality_measure_cpich_ec_no_t;

typedef struct
{
    U32             n;
    U8              elem[32];
}removed_inter_rat_cell_list_remove_some_inter_rat_cells_t;

#define RRC_UMTS_REMOVEDINTERRATCELLLIST_REMOVEALLINTERRATCELLS 1
#define RRC_UMTS_REMOVEDINTERRATCELLLIST_REMOVESOMEINTERRATCELLS 2
#define RRC_UMTS_REMOVEDINTERRATCELLLIST_REMOVENOINTERRATCELLS 3
typedef struct
{
    U32                     t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        removed_inter_rat_cell_list_remove_some_inter_rat_cells_t   remove_some_inter_rat_cells;
        /* t = 3 */
    }u;
}removed_inter_rat_cell_list_t;

#define RRC_UMTS_PENALTYTIME_RSCP_NOTUSED 1
#define RRC_UMTS_PENALTYTIME_RSCP_PT10    2
#define RRC_UMTS_PENALTYTIME_RSCP_PT20    3
#define RRC_UMTS_PENALTYTIME_RSCP_PT30    4
#define RRC_UMTS_PENALTYTIME_RSCP_PT40    5
#define RRC_UMTS_PENALTYTIME_RSCP_PT50    6
#define RRC_UMTS_PENALTYTIME_RSCP_PT60    7
typedef struct
{
    U32                         t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        U32         pt10;
        /* t = 3 */
        U32         pt20;
        /* t = 4 */
        U32         pt30;
        /* t = 5 */
        U32         pt40;
        /* t = 6 */
        U32         pt50;
        /* t = 7 */
        U32         pt60;
    }u;
}penalty_time_rscp_t;

typedef struct
{
    penalty_time_rscp_t     penalty_time_rscp;
}hcs_cell_reselect_info_rscp_t;

typedef struct
{
    U8                              hcs_prio;
    U8                              q_hcs;
    hcs_cell_reselect_info_rscp_t   hcs_cell_reselect_info_rscp;
}hcs_neighboring_cell_info_rscp_t;

#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_SIB_11_12_MODE_SPECIFIC_INFO_FDD_Q_QUAL_MIN_PRESENCE_FLAG    0x01
#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_SIB_11_12_MODE_SPECIFIC_INFO_FDD_Q_RX_LEV_MIN_PRESENCE_FLAG  0x02
typedef struct
{
    U16             presence_bitmask;
    S8              q_qual_min;
    S8              q_rx_lev_min;
}cell_select_reselect_info_sib_11_12_mode_specific_info_fdd_t;

#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_SIB_11_12_MODE_SPECIFIC_INFO_TDD_Q_RX_LEV_MIN_PRESENCE_FLAG  0x01
typedef struct
{
    U16             presence_bitmask;
    S8              q_rx_lev_min;
}cell_select_reselect_info_sib_11_12_mode_specific_info_tdd_t;

#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_SIB_11_12_MODE_SPECIFIC_INFO_GSM_Q_RX_LEV_MIN_PRESENCE_FLAG  0x01
typedef struct
{
    U16             presence_bitmask;
    S8              q_rx_lev_min;
}cell_select_reselect_info_sib_11_12_mode_specific_info_gsm_t;

#define RRC_UMTS_CELLSELECTRESELECTINFOSIB_11_12_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_CELLSELECTRESELECTINFOSIB_11_12_MODESPECIFICINFO_TDD 2
#define RRC_UMTS_CELLSELECTRESELECTINFOSIB_11_12_MODESPECIFICINFO_GSM 3
typedef struct
{
    U32                     t;
    union
    {
        cell_select_reselect_info_sib_11_12_mode_specific_info_fdd_t    fdd;
        cell_select_reselect_info_sib_11_12_mode_specific_info_tdd_t    tdd;
        cell_select_reselect_info_sib_11_12_mode_specific_info_gsm_t    gsm;
    }u;
}cell_select_reselect_info_sib_11_12_mode_specific_info_t;

#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_11_12_Q_OFFSET_2S_N_PRESENCE_FLAG                    0x01
#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_11_12_MAX_ALLOWED_UL_TX_PWR_PRESENCE_FLAG            0x02
#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_11_12_HCS_NEIGHBOURING_CELL_INFO_RSCP_PRESENCE_FLAG  0x04
typedef struct
{
    U16                                 presence_bitmask;
    S8                                  q_offset_1s_n;
    S8                                  q_offset_2s_n;
    S8                                  max_allowed_ul_tx_power;
    hcs_neighboring_cell_info_rscp_t    hcs_neighboring_cell_info_rscp;
    cell_select_reselect_info_sib_11_12_mode_specific_info_t    mode_specific_info;
}cell_select_reselect_info_sib_11_12_t;

typedef struct
{
    U8          ncc;
    U8          bcc;
}bsic_t;

#define UMTS_SIB_NEW_INTER_RAT_CELL_LIST_B_TECH_SPECIFIC_INFO_GSM_CELL_SELECT_RESELECT_INFO_PRESENCE_FLAG   0x01
#define UMTS_SIB_NEW_INTER_RAT_CELL_LIST_B_TECH_SPECIFIC_INFO_GSM_DUMMY_PRESENCE_FLAG                       0x02
typedef struct
{
    U16                                     presence_bitmask;
    cell_select_reselect_info_sib_11_12_t   cell_select_reselect_info_sib_11_12;
    S8                                      inter_rat_cell_individual_offset;
    bsic_t                                  bsic;
    U32                                     freq_band;
    U16                                     bcch_arfcn;
}new_inter_rat_cell_list_b_technology_specific_info_gsm_t;

typedef struct
{
    U32     is_2000_specific_meas_info;
}new_inter_rat_cell_list_b_technology_specific_info_is_2000_t;

#define RRC_UMTS_NEWINTERRATCELL_B_TECHNOLOGYSPECIFICINFO_GSM 1
#define RRC_UMTS_NEWINTERRATCELL_B_TECHNOLOGYSPECIFICINFO_IS_2000 2
#define RRC_UMTS_NEWINTERRATCELL_B_TECHNOLOGYSPECIFICINFO_ABSENT 3
#define RRC_UMTS_NEWINTERRATCELL_B_TECHNOLOGYSPECIFICINFO_SPARE1 4
typedef struct
{
    U32                         t;
    union
    {
        /* t = 1 */
        new_inter_rat_cell_list_b_technology_specific_info_gsm_t        gsm;
        /* t = 2 */
        new_inter_rat_cell_list_b_technology_specific_info_is_2000_t    is_2000;
        /* t = 3 */
        /* t = 4 */
    }u;
}new_inter_rat_cell_list_b_technology_specific_info_t;

#define UMTS_SIB_NEW_INTER_RAT_CELL_LIST_B_INTER_RAT_CELL_ID_PRESENCE_FLAG      0x01
typedef struct
{
    U16                 presence_bitmask;
    U8                  inter_rat_cell_id;
    new_inter_rat_cell_list_b_technology_specific_info_t    technology_specific_info; 
}new_inter_rat_cell_b_t;

typedef struct
{
    U8                         count;
    new_inter_rat_cell_b_t     new_inter_rat_cell_b[MAX_CELL_MEAS];
}new_inter_rat_cell_list_b_t;

typedef struct
{
    removed_inter_rat_cell_list_t   removed_inter_rat_cell_list;
    new_inter_rat_cell_list_b_t     new_inter_rat_cell_list_b;    
}inter_rat_cell_info_list_b_t;

#define UMTS_SIB_INTER_RAT_CELL_INFO_LIST_B_CELL_LIST_PRESENCE_FLAG   0x01
typedef struct
{
    U16                             presence_bitmask;
    inter_rat_cell_info_list_b_t    inter_rat_cell_info_list_b; 
}inter_rat_measurement_sysinfo_b_t;

#define RRC_UMTS_MEASUREMENTCONTROLSYSINFO_USE_OF_HCS_HCS_NOT_USED_CELLSELECTQUALITYMEASURE_CPICH_RSCP 1
#define RRC_UMTS_MEASUREMENTCONTROLSYSINFO_USE_OF_HCS_HCS_NOT_USED_CELLSELECTQUALITYMEASURE_CPICH_EC_N0 2
typedef struct
{
    U32                 t;
    union
    {
        /* t = 1 */
        measurement_control_sys_info_use_of_hcs_hcs_not_used_cell_select_quality_measure_cpich_rscp_t   cpich_rscp;
        /* t = 2 */
        measurement_control_sys_info_use_of_hcs_hcs_not_used_cell_select_quality_measure_cpich_ec_no_t  cpich_ec_no;
    }u;
}measurement_control_sys_info_use_of_hcs_hcs_not_used_cell_select_quality_measure_t;

#define UMTS_SIB_TYPE_11_MEAS_CTRL_SYSINFO_USE_OF_HCS_NOT_USED_INTER_RAT_MEAS_SYSINFO_PRESENCE_FLAG     0x01 
typedef struct
{
    U16                                 presence_bitmask;
    measurement_control_sys_info_use_of_hcs_hcs_not_used_cell_select_quality_measure_t  cell_select_quality_measure;
    inter_rat_measurement_sysinfo_b_t   inter_rat_measurement_sysinfo_b;
}measurement_control_sys_info_use_of_hcs_hcs_not_used_t;

#define UMTS_SIB_CELL_INFO_SI_HCS_RSCP_MODE_SPECIFIC_INFO_FDD_PRIMARY_CPICH_INFO_PRESENCE_FLAG      0x01
#define UMTS_SIB_CELL_INFO_SI_HCS_RSCP_MODE_SPECIFIC_INFO_FDD_PRIMARY_CPICH_TX_PWR_PRESENCE_FLAG    0x02
typedef struct
{
    U16                         presence_bitmask;
    primary_cpich_info_t        primary_cpich_info;
    S8                          primary_cpich_tx_power;
    U8                          read_sfn_indicator;
    U8                          tx_diversity_indicator;
}cell_info_si_hcs_rscp_mode_specific_info_fdd_t;

#define UMTS_SIB_CELL_INFO_SI_HCS_RSCP_MODE_SPECIFIC_INFO_TDD_PRIMARY_CCPCH_TX_PWR_PRESENCE_FLAG    0x01
#define UMTS_SIB_CELL_INFO_SI_HCS_RSCP_MODE_SPECIFIC_INFO_TDD_TIMESLOT_INFO_LIST_PRESENCE_FLAG      0x02
typedef struct
{
    U16                         presence_bitmask;
    primary_ccpch_info_t        primary_ccpch_info;
    U8                          primary_ccpch_tx_power;
    timeslot_info_list_t        timeslot_info_list;
    U8                          read_sfn_indicator;
}cell_info_si_hcs_rscp_mode_specific_info_tdd_t;

#define RRC_UMTS_CELLINFOSI_HCS_RSCP_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_CELLINFOSI_HCS_RSCP_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32                     t;
    union
    {
        cell_info_si_hcs_rscp_mode_specific_info_fdd_t  fdd;
        cell_info_si_hcs_rscp_mode_specific_info_tdd_t  tdd;
    }u;
}cell_info_si_hcs_rscp_mode_specific_info_t;

typedef struct
{
    U8                              hcs_prio;
    U8                              q_hcs;
    hcs_cell_reselect_info_rscp_t   hcs_cell_reselect_info_rscp;
}hcs_neighbouring_cell_info_rscp_t;

#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_SIB_11_12_HCS_RSCP_MODE_SPECIFIC_INFO_FDD_Q_QUAL_MIN_PRESENCE_FLAG       0x01
#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_SIB_11_12_HCS_RSCP_MODE_SPECIFIC_INFO_FDD_Q_RX_LEV_MIN_PRESENCE_FLAG     0x02
typedef struct
{
    U16             presence_bitmask;
    S8              q_qual_min;
    S8              q_rxlev_min;
}cell_select_reselect_info_sib_11_12_hcs_rscp_mode_specific_info_fdd_t;

#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_SIB_11_12_HCS_RSCP_MODE_SPECIFIC_INFO_TDD_Q_RX_LEV_MIN_PRESENCE_FLAG     0x01
typedef struct
{
    U16             presence_bitmask;
    S8              q_rxlev_min;
}cell_select_reselect_info_sib_11_12_hcs_rscp_mode_specific_info_tdd_t;

#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_SIB_11_12_HCS_RSCP_MODE_SPECIFIC_INFO_GSM_Q_RX_LEV_MIN_PRESENCE_FLAG     0x01
typedef struct
{
    U16             presence_bitmask;
    S8              q_rxlev_min;
}cell_select_reselect_info_sib_11_12_hcs_rscp_mode_specific_info_gsm_t;

#define RRC_UMTS_CELLSELECTRESELECTINFOSIB_11_12_HCS_RSCP_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_CELLSELECTRESELECTINFOSIB_11_12_HCS_RSCP_MODESPECIFICINFO_TDD 2
#define RRC_UMTS_CELLSELECTRESELECTINFOSIB_11_12_HCS_RSCP_MODESPECIFICINFO_GSM 3
typedef struct
{
    U32                     t;
    union
    {
        cell_select_reselect_info_sib_11_12_hcs_rscp_mode_specific_info_fdd_t   fdd;
        cell_select_reselect_info_sib_11_12_hcs_rscp_mode_specific_info_tdd_t   tdd;
        cell_select_reselect_info_sib_11_12_hcs_rscp_mode_specific_info_gsm_t   gsm;
    }u;
}cell_select_reselect_info_sib_11_12_hcs_rscp_mode_specific_info_t;

#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_SIB_11_12_HCS_RSCP_MAX_ALLOWED_UL_TX_PWR_PRESENCE_FLAG           0x01
#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_SIB_11_12_HCS_RSCP_HCS_NEIGHBOURING_CELL_INFO_RSCP_PRESENCE_FLAG 0x02
typedef struct
{
    U16                                 presence_bitmask;
    S8                                  q_offset_s_n;
    S8                                  max_allowed_ul_tx_power;
    hcs_neighbouring_cell_info_rscp_t   hcs_neighbouring_cell_info_rscp;
    cell_select_reselect_info_sib_11_12_hcs_rscp_mode_specific_info_t   mode_specific_info;
}cell_select_reselect_info_sib_11_12_hcs_rscp_t;

#define UMTS_SIB_CELL_INFO_SI_HCS_RSCP_REF_TIME_DIFF_TO_CELL_PRESENCE_FLAG      0x01
#define UMTS_SIB_CELL_INFO_SI_HCS_RSCP_CELL_SELECT_RELECT_INFO_PRESENCE_FLAG    0x02
typedef struct
{
    U16                                             presence_bitmask;
    S8                                              cell_individual_offset;
    reference_time_difference_to_cell_t             reference_time_difference_to_cell;
    cell_info_si_hcs_rscp_mode_specific_info_t      mode_specific_info;
    cell_select_reselect_info_sib_11_12_hcs_rscp_t  cell_select_reselect_info_sib_11_12_hcs_rscp;
}cell_info_si_hcs_rscp_t;

#define UMTS_SIB_NEW_INTRA_FREQ_CELL_SI_HCS_RSCP_INTRA_FREQ_CELL_ID_PRESENCE_FLAG   0x01
typedef struct
{
    U16                     presence_bitmask;
    U8                      intra_freq_cell_id;
    cell_info_si_hcs_rscp_t cell_info_si_hcs_rscp;
}new_intra_freq_cell_si_hcs_rscp_t;

typedef struct
{
    U8                                  count;
    new_intra_freq_cell_si_hcs_rscp_t   new_intra_freq_cell_si_hcs_rscp[MAX_CELL_MEAS];
}new_intra_freq_cell_si_list_hcs_rscp_t;

#define UMTS_SIB_INTRA_FREQ_CELL_INFO_SI_LIST_HCS_RSCP_REMOVED_INTRA_FREQ_CELL_LIST_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                     presence_bitmask;
    removed_intra_freq_cell_list_t          removed_intra_freq_cell_list;
    new_intra_freq_cell_si_list_hcs_rscp_t  new_intra_freq_cell_si_list_hcs_rscp;
}intra_freq_cell_info_si_list_hcs_rscp_t;

#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_HCS_RSCP_INTRA_FREQ_CELL_INFO_SI_LIST_PRESENCE_FLAG        0x01
#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_HCS_RSCP_INTRA_FREQ_MEAS_QTY_PRESENCE_FLAG                 0x02
#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_HCS_RSCP_INTRA_FREQ_REPORTING_QTY_FOR_RACH_PRESENCE_FLAG   0x04
#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_HCS_RSCP_MAX_REPORTED_CELLS_ON_RACH_PRESENCE_FLAG          0x08
#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_HCS_RSCP_REPORTING_INFO_FOR_CELL_DCH_PRESENCE_FLAG         0x10
typedef struct
{
    U16                                         presence_bitmask;
    U8                                          intra_freq_meas_id;
    intra_freq_cell_info_si_list_hcs_rscp_t     intra_freq_cell_info_si_list_hcs_rscp;
    intra_freq_meas_quantity_t                  intra_freq_meas_quantity;
    intra_freq_reporting_quantity_for_rach_t    intra_freq_reporting_quantity_for_rach;
    U32                                         max_reported_cells_on_rach;
    reporting_info_for_cell_dch_t               reporting_info_for_cell_dch;
}intra_freq_measurement_sys_info_hcs_rscp_t;

#define UMTS_SIB_NEW_INTER_FREQ_CELL_SI_HCS_RSCP_INTER_FREQ_CELL_ID_PRESENCE_FLAG       0x01
#define UMTS_SIB_NEW_INTER_FREQ_CELL_SI_HCS_RSCP_FREQ_INFO_PRESENCE_FLAG                0x02
typedef struct
{
    U16                     presence_bitmask;
    U8                      inter_freq_cell_id;
    frequency_info_t        frequency_info;
    cell_info_si_hcs_rscp_t cell_info_si_hcs_rscp;
}new_inter_freq_cell_si_hcs_rscp_t;

typedef struct
{
    U8                                  count;
    new_inter_freq_cell_si_hcs_rscp_t   new_inter_freq_cell_si_hcs_rscp[MAX_CELL_MEAS];
}new_inter_freq_cell_si_list_hcs_rscp_t;

#define UMTS_SIB_INTER_FREQ_CELL_INFO_SI_LIST_HCS_RSCP_REMOVED_INTER_FREQ_CELL_LIST_PRESENCE_FLAG   0x01
#define UMTS_SIB_INTER_FREQ_CELL_INFO_SI_LIST_HCS_RSCP_NEW_INTER_FREQ_CELL_LIST_PRESENCE_FLAG       0x02
typedef struct
{
    U16                                     presence_bitmask;
    removed_inter_freq_cell_list_t          removed_inter_freq_cell_list;
    new_inter_freq_cell_si_list_hcs_rscp_t  new_inter_freq_cell_si_list_hcs_rscp;
}inter_freq_cell_info_si_list_hcs_rscp_t;

#define UMTS_SIB_INTER_FREQ_MEAS_SYSINFO_HCS_RSCP_INTER_FREQ_CELL_INFO_SI_LIST_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                     presence_bitmask;
    inter_freq_cell_info_si_list_hcs_rscp_t inter_freq_cell_info_si_list_hcs_rscp;
}inter_freq_measurement_sys_info_hcs_rscp_t;

#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_HCS_RSCP_PRESENCE_FLAG     0x01
#define UMTS_SIB_INTER_FREQ_MEAS_SYSINFO_HCS_RSCP_PRESENCE_FLAG     0x02
typedef struct
{
    U16                 presence_bitmask;
    intra_freq_measurement_sys_info_hcs_rscp_t  intra_freq_measurement_sys_info_hcs_rscp;
    inter_freq_measurement_sys_info_hcs_rscp_t  inter_freq_measurement_sys_info_hcs_rscp; 
}measurement_control_sys_info_use_of_hcs_hcs_used_cell_select_quality_measure_cpich_rscp_t;

#define UMTS_SIB_CELL_INFO_SI_HCS_ECNO_MODE_SPECIFIC_INFO_FDD_PRIMARY_CPICH_INFO_PRESENCE_FLAG      0x01
#define UMTS_SIB_CELL_INFO_SI_HCS_ECNO_MODE_SPECIFIC_INFO_FDD_PRIMARY_CPICH_TX_PWR_PRESENCE_FLAG    0x02
typedef struct
{
    U16                     presence_bitmask;
    primary_cpich_info_t    primary_cpich_info;
    S8                      primary_cpich_tx_power;
    U8                      read_sfn_indicator;
    U8                      tx_diversity_indicator;
}cell_info_si_hcs_ecno_mode_specific_info_fdd_t;

#define UMTS_SIB_CELL_INFO_SI_HCS_ECNO_MODE_SPECIFIC_INFO_TDD_PRIMARY_CCPCH_TX_PWR_PRESENCE_FLAG    0x01
#define UMTS_SIB_CELL_INFO_SI_HCS_ECNO_MODE_SPECIFIC_INFO_TDD_TIME_SLOT_INFO_LIST_PRESENCE_FLAG     0x02
typedef struct
{
    U16                     presence_bitmask;
    primary_ccpch_info_t    primary_ccpch_info;
    U8                      primary_ccpch_tx_power;
    timeslot_info_list_t    timeslot_info_list;
    U8                      read_sfn_indicator;
}cell_info_si_hcs_ecno_mode_specific_info_tdd_t;

#define RRC_UMTS_CELLINFOSI_HCS_ECN0_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_CELLINFOSI_HCS_ECN0_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32                         t;
    union
    {
        cell_info_si_hcs_ecno_mode_specific_info_fdd_t  fdd;
        cell_info_si_hcs_ecno_mode_specific_info_tdd_t  tdd;
    }u;
}cell_info_si_hcs_ecno_mode_specific_info_t;

typedef struct
{
    U32         temporary_offset_1;
    U32         temporary_offset_2;
}temporary_offset_list_t;

#define RRC_UMTS_PENALTYTIME_ECN0_NOTUSED 1
#define RRC_UMTS_PENALTYTIME_ECN0_PT10    2
#define RRC_UMTS_PENALTYTIME_ECN0_PT20    3
#define RRC_UMTS_PENALTYTIME_ECN0_PT30    4
#define RRC_UMTS_PENALTYTIME_ECN0_PT40    5
#define RRC_UMTS_PENALTYTIME_ECN0_PT50    6
#define RRC_UMTS_PENALTYTIME_ECN0_PT60    7
typedef struct
{
    U32                         t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        temporary_offset_list_t     pt10;
        /* t = 3 */
        temporary_offset_list_t     pt20;
        /* t = 4 */
        temporary_offset_list_t     pt30;
        /* t = 5 */
        temporary_offset_list_t     pt40;
        /* t = 6 */
        temporary_offset_list_t     pt50;
        /* t = 7 */
        temporary_offset_list_t     pt60;
    }u;
}penalty_time_ecno_t;

typedef struct
{
    penalty_time_ecno_t penalty_time_ecno;
}hcs_cell_reselect_info_ecno_t;

typedef struct
{
    U8          hcs_prio;
    U8          q_hcs;
    hcs_cell_reselect_info_ecno_t   hcs_cell_reselect_info_ecno;
}hcs_neighbouring_cell_info_ecno_t;

#define CELL_SELECT_RESELECT_INFO_SIB_11_12_HCS_ECNO_MODE_SPECIFIC_INFO_FDD_Q_QUAL_MIN_PRESENCE_FLAG    0x01
#define CELL_SELECT_RESELECT_INFO_SIB_11_12_HCS_ECNO_MODE_SPECIFIC_INFO_FDD_Q_RX_LEV_MIN_PRESENCE_FLAG  0x02
typedef struct
{
    U16             presence_bitmask;
    S8              q_qual_min;
    S8              q_rxlev_min;
}cell_select_reselect_info_sib_11_12_hcs_ecno_mode_specific_info_fdd_t;

#define CELL_SELECT_RESELECT_INFO_SIB_11_12_HCS_ECNO_MODE_SPECIFIC_INFO_TDD_Q_RX_LEV_MIN_PRESENCE_FLAG  0x01
typedef struct
{
    U16             presence_bitmask;
    S8              q_rxlev_min;
}cell_select_reselect_info_sib_11_12_hcs_ecno_mode_specific_info_tdd_t;

#define CELL_SELECT_RESELECT_INFO_SIB_11_12_HCS_ECNO_MODE_SPECIFIC_INFO_GSM_Q_RX_LEV_MIN_PRESENCE_FLAG  0x01
typedef struct
{
    U16             presence_bitmask;
    S8              q_rxlev_min;
}cell_select_reselect_info_sib_11_12_hcs_ecno_mode_specific_info_gsm_t;

#define RRC_UMTS_CELLSELECTRESELECTINFOSIB_11_12_HCS_ECN0_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_CELLSELECTRESELECTINFOSIB_11_12_HCS_ECN0_MODESPECIFICINFO_TDD 2
#define RRC_UMTS_CELLSELECTRESELECTINFOSIB_11_12_HCS_ECN0_MODESPECIFICINFO_GSM 3
typedef struct
{
    U32                         t;
    union
    {
        cell_select_reselect_info_sib_11_12_hcs_ecno_mode_specific_info_fdd_t   fdd;
        cell_select_reselect_info_sib_11_12_hcs_ecno_mode_specific_info_tdd_t   tdd;
        cell_select_reselect_info_sib_11_12_hcs_ecno_mode_specific_info_gsm_t   gsm;
    }u;
}cell_select_reselect_info_sib_11_12_hcs_ecno_mode_specific_info_t;

#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_SIB_11_12_HCS_ECNO_MAX_ALLOWED_UL_TX_PWR_PRESENCE_FLAG           0x01
#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_SIB_11_12_HCS_ECNO_HCS_NEIGHBOURING_CELL_INFO_ECNO_PRESENCE_FLAG 0x02
typedef struct
{
    U16                     presence_bitmask;
    S8                      q_offset_1s_n;
    S8                      q_offset_2s_n;
    S8                      max_allowed_ul_tx_power;
    hcs_neighbouring_cell_info_ecno_t   hcs_neighbouring_cell_info_ecno;
    cell_select_reselect_info_sib_11_12_hcs_ecno_mode_specific_info_t   mode_specific_info;
}cell_select_reselect_info_sib_11_12_hcs_ecno_t;

#define UMTS_SIB_CELL_INFO_SI_HCS_ECNO_REF_TIME_DIFF_TO_CELL_PRESENCE_FLAG      0x01
#define UMTS_SIB_CELL_INFO_SI_HCS_ECNO_CELL_SELECT_RESELECT_INFO_PRESENCE_FLAG  0x02
typedef struct
{
    U16                                             presence_bitmask;
    S8                                              cell_individual_offset;
    reference_time_difference_to_cell_t             reference_time_difference_to_cell;
    cell_info_si_hcs_ecno_mode_specific_info_t      mode_specific_info;
    cell_select_reselect_info_sib_11_12_hcs_ecno_t  cell_select_reselect_info_sib_11_12_hcs_ecno;
}cell_info_si_hcs_ecno_t;

#define UMTS_SIB_NEW_INTRA_FREQ_CELL_SI_HCS_ECNO_INTRA_FREQ_CELL_ID_PRESENCE_FLAG   0x01
typedef struct
{
    U16                     presence_bitmask;
    U8                      intra_freq_cell_id;
    cell_info_si_hcs_ecno_t cell_info_si_hcs_ecno;
}new_intra_freq_cell_si_hcs_ecno_t;

typedef struct
{
    U8                                  count;
    new_intra_freq_cell_si_hcs_ecno_t   new_intra_freq_cell_si_hcs_ecno[MAX_CELL_MEAS];
}new_intra_freq_cell_si_list_hcs_ecno_t;

#define UMTS_SIB_INTRA_FREQ_CELL_INFO_SI_LIST_HCS_ECNO_REMOVED_INTRA_FREQ_CELL_LIST_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                     presence_bitmask;
    removed_intra_freq_cell_list_t          removed_intra_freq_cell_list;
    new_intra_freq_cell_si_list_hcs_ecno_t  new_intra_freq_cell_si_list_hcs_ecno;
}intra_freq_cell_info_si_list_hcs_ecno_t;

#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_HCS_ECNO_INTRA_FREQ_CELL_INFO_SI_LIST_PRESENCE_FLAG        0x01
#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_HCS_ECNO_INTRA_FREQ_MEAS_QTY_PRESENCE_FLAG                 0x02
#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_HCS_ECNO_INTRA_FREQ_REPORTING_QTY_FOR_RACH_PRESENCE_FLAG   0x04
#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_HCS_ECNO_MAX_REPORTED_CELLS_ON_RACH_PRESENCE_FLAG          0x08
#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_HCS_ECNO_REPORTING_INFO_FOR_CELL_DCH_PRESENCE_FLAG         0x10
typedef struct
{
    U16                                         presence_bitmask;
    U8                                          intra_freq_meas_id;
    intra_freq_cell_info_si_list_hcs_ecno_t     intra_freq_cell_info_si_list_hcs_ecno;
    intra_freq_meas_quantity_t                  intra_freq_meas_quantity;
    intra_freq_reporting_quantity_for_rach_t    intra_freq_reporting_quantity_for_rach;
    U32                                         max_reported_cells_on_rach;
    reporting_info_for_cell_dch_t               reporting_info_for_cell_dch;
}intra_freq_measurement_sys_info_hcs_ecno_t;

#define UMTS_SIB_NEW_INTER_FREQ_CELL_SI_HCS_ECNO_INTER_FREQ_CELL_ID_PRESENCE_FLAG       0x01
#define UMTS_SIB_NEW_INTER_FREQ_CELL_SI_HCS_ECNO_FREQ_INFO_PRESENCE_FLAG                0x02
typedef struct
{
    U16                     presence_bitmask;
    U8                      inter_freq_cell_id;
    frequency_info_t        frequency_info;
    cell_info_si_hcs_ecno_t cell_info_si_hcs_ecno;
}new_inter_freq_cell_si_hcs_ecno_t;

typedef struct
{
    U8                                  count;
    new_inter_freq_cell_si_hcs_ecno_t   new_inter_freq_cell_si_hcs_ecno[MAX_CELL_MEAS];
}new_inter_freq_cell_si_list_hcs_ecno_t;

#define UMTS_SIB_INTER_FREQ_CELL_INFO_SI_LIST_HCS_ECNO_REMOVED_INTER_FREQ_CELL_LIST_PRESENCE_FLAG   0x01
#define UMTS_SIB_INTER_FREQ_CELL_INFO_SI_LIST_HCS_ECNO_NEW_INTER_FREQ_CELL_LIST_PRESENCE_FLAG       0x02
typedef struct
{
    U16                                     presence_bitmask;
    removed_inter_freq_cell_list_t          removed_inter_freq_cell_list;
    new_inter_freq_cell_si_list_hcs_ecno_t  new_inter_freq_cell_si_list_hcs_ecno;
}inter_freq_cell_info_si_list_hcs_ecno_t;

#define UMTS_SIB_INTER_FREQ_MEAS_SYSINFO_HCS_ECNO_INTER_FREQ_CELL_INFO_SI_LIST_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                     presence_bitmask;
    inter_freq_cell_info_si_list_hcs_ecno_t inter_freq_cell_info_si_list_hcs_ecno;
}inter_freq_measurement_sys_info_hcs_ecno_t;

#define UMTS_SIB_INTRA_FREQ_MEAS_SYSINFO_HCS_ECNO_PRESENCE_FLAG     0x01
#define UMTS_SIB_INTER_FREQ_MEAS_SYSINFO_HCS_ECNO_PRESENCE_FLAG     0x02
typedef struct
{
    U16                                         presence_bitmask;
    intra_freq_measurement_sys_info_hcs_ecno_t  intra_freq_measurement_sys_info_hcs_ecno;
    inter_freq_measurement_sys_info_hcs_ecno_t  inter_freq_measurement_sys_info_hcs_ecno;
}measurement_control_sys_info_use_of_hcs_hcs_used_cell_select_quality_measure_cpich_ec_no_t;

#define RRC_UMTS_MEASUREMENTCONTROLSYSINFO_USE_OF_HCS_HCS_USED_CELLSELECTQUALITYMEASURE_CPICH_RSCP 1
#define RRC_UMTS_MEASUREMENTCONTROLSYSINFO_USE_OF_HCS_HCS_USED_CELLSELECTQUALITYMEASURE_CPICH_EC_N0 2
typedef struct
{
    U32                     t;
    union
    {
        measurement_control_sys_info_use_of_hcs_hcs_used_cell_select_quality_measure_cpich_rscp_t   cpich_rscp;
        measurement_control_sys_info_use_of_hcs_hcs_used_cell_select_quality_measure_cpich_ec_no_t  cpich_ec_no; 
    }u;
}measurement_control_sys_info_use_of_hcs_hcs_used_cell_select_quality_measure_t;

#define UMTS_SIB_NEW_INTER_RAT_CELL_TECH_SPECIFIC_INFO_GSM_CELL_SELECT_RELECT_INFO_PRESENCE_FLAG    0x01
#define UMTS_SIB_NEW_INTER_RAT_CELL_TECH_SPECIFIC_INFO_GSM_DUMMY_PRESENCE_FLAG                      0x02
typedef struct
{
    U16                                     presence_bitmask;
    cell_select_reselect_info_sib_11_12_t   cell_select_reselect_info_sib_11_12;
    S8                                      inter_rat_cell_individual_offset;
    bsic_t                                  bsic;
    U32                                     freq_band;
    U16                                     bcch_arfcn;
}new_inter_rat_cell_technology_specific_info_gsm_t;

typedef struct
{
    U32     is_2000_specific_meas_info;
}new_inter_rat_cell_technology_specific_info_is_2000_t;

#define RRC_UMTS_NEWINTERRATCELL_TECHNOLOGYSPECIFICINFO_GSM 1
#define RRC_UMTS_NEWINTERRATCELL_TECHNOLOGYSPECIFICINFO_IS_2000 2
#define RRC_UMTS_NEWINTERRATCELL_TECHNOLOGYSPECIFICINFO_ABSENT 3
#define RRC_UMTS_NEWINTERRATCELL_TECHNOLOGYSPECIFICINFO_SPARE1 4
typedef struct
{
    U32                         t;
    union
    {
        /* t = 1 */
        new_inter_rat_cell_technology_specific_info_gsm_t       gsm;
        /* t = 2 */
        new_inter_rat_cell_technology_specific_info_is_2000_t   is_2000;
        /* t = 3 */
        /* t = 4 */
    }u;
}new_inter_rat_cell_technology_specific_info_t;

#define UMTS_SIB_NEW_INTER_RAT_CELL_INTER_RAT_CELL_ID_PRESENCE_FLAG     0x01
typedef struct
{
    U16                         presence_bitmask;
    U8                          inter_rat_cell_id;
    new_inter_rat_cell_technology_specific_info_t   new_inter_rat_cell_technology_specific_info;
}new_inter_rat_cell_t;

typedef struct
{
    U8                          count;
    new_inter_rat_cell_t        new_inter_rat_cell[MAX_CELL_MEAS];
}new_inter_rat_cell_list_t;

typedef struct
{
    U32             n;
    U8              elem[MAX_CELL_MEAS];
}cells_for_inter_rat_meas_list_t;

#define UMTS_SIB_INTER_RAT_CELL_INFO_LIST_CELLS_FOR_INTER_RAT_MEAS_LIST_PRESENCE_FLAG   0x01
typedef struct
{
    U16                             presence_bitmask;
    removed_inter_rat_cell_list_t   removed_inter_rat_cell_list;
    new_inter_rat_cell_list_t       new_inter_rat_cell_list;
    cells_for_inter_rat_meas_list_t cells_for_inter_rat_meas_list;
}inter_rat_cell_info_list_t;

#define UMTS_SIB_INTER_RAT_MEAS_SYSINFO_INTER_RAT_CELL_INFO_LIST_PRESENCE_FLAG  0x01
typedef struct
{
    U16                         presence_bitmask;
    inter_rat_cell_info_list_t  inter_rat_cell_info_list;
}inter_rat_measurement_sys_info_t;

#define UMTS_SIB_MEAS_CTRL_SYSINFO_USE_OF_HCS_HCS_USED_INTER_RAT_MEAS_SYSINFO_PRESENCE_FLAG     0x01
typedef struct
{
    U16                                 presence_bitmask;
    measurement_control_sys_info_use_of_hcs_hcs_used_cell_select_quality_measure_t  cell_select_quality_measure;
    inter_rat_measurement_sys_info_t    inter_rat_measurement_sys_info; 
}measurement_control_sys_info_use_of_hcs_hcs_used_t;

#define RRC_UMTS_MEASUREMENTCONTROLSYSINFO_USE_OF_HCS_HCS_NOT_USED 1
#define RRC_UMTS_MEASUREMENTCONTROLSYSINFO_USE_OF_HCS_HCS_USED 2
typedef struct
{
    U32         t;
    union
    {
        /* t = 1 */
        measurement_control_sys_info_use_of_hcs_hcs_not_used_t  hcs_not_used;
        /* t = 2 */
        measurement_control_sys_info_use_of_hcs_hcs_used_t      hcs_used; 
    }u;
}measurement_control_sys_info_use_of_hcs_t;

#define RRC_UMTS_UL_TRCH_IDENTITY_DCH     1
#define RRC_UMTS_UL_TRCH_IDENTITY_RACHORCPCH 2
#define RRC_UMTS_UL_TRCH_IDENTITY_USCH    3
typedef struct
{
    U32         t;
    union
    {   
        /* t = 1 */
        U8      dch;
        /* t = 2 */
        /* t = 3 */
        U8      usch;
    }u;
}ul_tr_ch_identity_t;

typedef struct
{
    U8                      count;
    ul_tr_ch_identity_t     ul_tr_ch_identity[MAX_TR_CH]; 
}traffic_volume_measurement_object_list_t;

#define RRC_UMTS_TRAFFICVOLUMEMEASQUANTITY_RLC_BUFFERPAYLOAD 1
#define RRC_UMTS_TRAFFICVOLUMEMEASQUANTITY_AVERAGERLC_BUFFERPAYLOAD 2
#define RRC_UMTS_TRAFFICVOLUMEMEASQUANTITY_VARIANCEOFRLC_BUFFERPAYLOAD 3
typedef struct
{
    U32                     t;
    union
    {
        /* t = 1 */
        /* t = 2 */
        U8          average_rlc_buffer_payload;
        /* t = 3 */
        U8          variance_of_rlc_buffer_payload;
    }u;
}traffic_volume_meas_quantity_t;

typedef struct
{
    U8          rlc_rb_buffer_payload;
    U8          rlc_rb_buffer_payload_average;
    U8          rlc_rb_buffer_payload_variance;
}traffic_volume_reporting_quantity_t;

#define UMTS_SIB_TRAFFIC_VOL_EVENT_PARAM_TIME_TO_TRIGGET_PRESENCE_FLAG              0x01
#define UMTS_SIB_TRAFFIC_VOL_EVENT_PARAM_PENDING_TIME_AFTER_TRIGGER_PRESENCE_FLAG   0x02
#define UMTS_SIB_TRAFFIC_VOL_EVENT_PARAM_TX_INTERRUPT_AFTER_TRIGGER_PRESENCE_FLAG   0x04
typedef struct
{
    U16     presence_bitmask;
    U32     event_id;
    U32     reporting_threshold;
    U32     time_to_trigger;
    U32     pending_time_after_trigger;
    U32     tx_interrupt_after_trigger;
}traffic_volume_event_param_t;

typedef struct
{
    U8                              count;
    traffic_volume_event_param_t    traffic_volume_event_param[MAX_MEAS_PAR_EVENT];
}trans_ch_criteria_event_specific_parameters_t;

#define UMTS_SIB_TRANS_CH_CRITERIA_UL_TR_CH_IDENTITY_PRESENCE_FLAG      0x01
#define UMTS_SIB_TRANS_CH_CRITERIA_EVENT_SPECIFIC_PARAMS_PRESENCE_FLAG  0x02
typedef struct
{
    U16                         presence_bitmask;
    ul_tr_ch_identity_t         ul_tr_ch_identity;
    trans_ch_criteria_event_specific_parameters_t   trans_ch_criteria_event_specific_parameters;
}trans_ch_criteria_t;

typedef struct
{
    U8                      count;
    trans_ch_criteria_t     trans_ch_criteria[MAX_TR_CH];
}trans_ch_criteria_list_t;

#define UMTS_SIB_TRAFFIC_VOL_REPORTING_CRITERIA_TRANS_CH_CRITERIA_LIST_PRESENCE_FLAG    0x01
typedef struct
{
    U16                         presence_bitmask;
    trans_ch_criteria_list_t    trans_ch_criteria_list;
}traffic_volume_reporting_criteria_t;

typedef struct
{
    U32             ue_state;
}measurement_validity_t;

#define RRC_UMTS_TRAFFICVOLUMEREPORTCRITERIASYSINFO_TRAFFICVOLUMEREPORTINGCRITERIA 1
#define RRC_UMTS_TRAFFICVOLUMEREPORTCRITERIASYSINFO_PERIODICALREPORTINGCRITERIA 2
typedef struct
{
    U32                         t;
    union
    {
        traffic_volume_reporting_criteria_t traffic_volume_reporting_criteria;
        periodical_reporting_criteria_t     periodical_reporting_criteria;
    }u;
}traffic_volume_report_criteria_sys_info_t;

#define UMTS_SIB_TRAFFIC_VOL_MEAS_SYSINFO_MEAS_OBJ_LIST_PRESENCE_FLAG       0x01
#define UMTS_SIB_TRAFFIC_VOL_MEAS_SYSINFO_MEAS_QTY_PRESENCE_FLAG            0x02
#define UMTS_SIB_TRAFFIC_VOL_MEAS_SYSINFO_REPORTING_QTY_PRESENCE_FLAG       0x04
#define UMTS_SIB_TRAFFIC_VOL_MEAS_SYSINFO_DUMMY_PRESENCE_FLAG               0x08
#define UMTS_SIB_TRAFFIC_VOL_MEAS_SYSINFO_MEAS_VALIDITY_PRESENCE_FLAG       0x10
typedef struct
{
    U16                                         presence_bitmask;
    U8                                          traffic_vol_meas_id;
    traffic_volume_measurement_object_list_t    traffic_volume_measurement_object_list;
    traffic_volume_meas_quantity_t              traffic_volume_meas_quantity;
    traffic_volume_reporting_quantity_t         traffic_volume_reporting_quantity;
    traffic_volume_reporting_criteria_t         dummy;
    measurement_validity_t                      measurement_validity;
    measurement_reporting_mode_t                measurement_reporting_mode;
    traffic_volume_report_criteria_sys_info_t   traffic_volume_report_criteria_sys_info;
}traffic_vol_meas_sys_info_t;

typedef struct
{
    U32                 ue_meas_quantity;
    U32                 filter_coefficient;
}ue_internal_meas_quantity_t;

typedef struct
{
    U8                          ue_internal_meas_id;
    ue_internal_meas_quantity_t ue_internal_meas_quantity;
}ue_internal_meas_sys_info_t;

#define UMTS_SIB_TYPE_11_MEAS_CTRL_SYSINFO_TRAFFIC_VOL_MEAS_SYSINFO_PRESENCE_FLAG   0x01
#define UMTS_SIB_TYPE_11_MEAS_CTRL_SYSINFO_DUMMY_PRESENCE_FLAG                      0x02
typedef struct
{
    U16                                         presence_bitmask;
    measurement_control_sys_info_use_of_hcs_t   use_of_hcs;
    traffic_vol_meas_sys_info_t                 traffic_vol_meas_sys_info;
    ue_internal_meas_sys_info_t                 dummy;
}measurement_control_sys_info_t;

typedef struct
{
    U32     n;
    U32     elem[15];
}fach_measurement_occasion_info_inter_rat_meas_ind_t;

#define UMTS_SIB_TYPE_11_FACH_MEAS_OCCASION_INFO_MEAS_OCCASION_COEFF_PRESENCE_FLAG  0x01
#define UMTS_SIB_TYPE_11_FACH_MEAS_OCCASION_INFO_INTER_RAT_MEAS_IND_PRESENCE_FLAG   0x02
typedef struct
{
    U16                         presence_bitmask;
    U8                          fach_meas_occasion_coeff;
    U8                          inter_freq_fdd_meas_ind;
    U8                          inter_freq_tdd_meas_ind;
    fach_measurement_occasion_info_inter_rat_meas_ind_t     inter_rat_meas_ind;
}fach_measurement_occasion_info_t;

typedef struct
{
    U32 n;
    U8 elem[32];
}removedintrafreqcelllist_removesomeintrafreqcells_t;

#define RRC_UMTS_RemovedIntraFreqCellList_removeAllIntraFreqCells 1
#define RRC_UMTS_RemovedIntraFreqCellList_removeSomeIntraFreqCells 2
#define RRC_UMTS_RemovedIntraFreqCellList_removeNoIntraFreqCells 3
typedef struct
{
    U32 t;
    union {
        /* t = 1 */
        /* t = 2 */
        removedintrafreqcelllist_removesomeintrafreqcells_t remove_someIntra_freq_cells;
        /* t = 3 */
    } u;
}removedintrafreqcelllist_t;


#define RRC_UMTS_ReferenceTimeDifferenceToCell_accuracy40 1
#define RRC_UMTS_ReferenceTimeDifferenceToCell_accuracy256 2
#define RRC_UMTS_ReferenceTimeDifferenceToCell_accuracy2560 3

typedef struct
{
    U32 t;
    union {
        /* t = 1 */
        U16 accuracy40;
        /* t = 2 */
        U8 accuracy256;
        /* t = 3 */
        U8 accuracy2560;
    } u;
}referencetimedifferencetocell_t;

#define UMTS_SIB_PRIMARYCCPCH_INFO_LCR_R4_CELLPARAMETERSID_PRESENCE_FLAG    0x01
typedef struct 
{
    U16		presence_bitmask;
    U8 tstd_Indicator;
    U8 cellParametersID;
    U8 sctd_Indicator;
}primaryccpch_Info_lcr_r4_t;

typedef struct
{
    U8 timeslot_number;
}timeslotinfo_lcr_r4_t;

typedef struct
{
    U8 count;
    timeslotinfo_lcr_r4_t timeslotinfo_lcr_r4[MAX_TS_LCR];
}timeslotinfolist_lcr_r4_t;

#define RRC_UMTS_PenaltyTime_ECN0_notUsed 1
#define RRC_UMTS_PenaltyTime_ECN0_pt10    2
#define RRC_UMTS_PenaltyTime_ECN0_pt20    3
#define RRC_UMTS_PenaltyTime_ECN0_pt30    4
#define RRC_UMTS_PenaltyTime_ECN0_pt40    5
#define RRC_UMTS_PenaltyTime_ECN0_pt50    6
#define RRC_UMTS_PenaltyTime_ECN0_pt60    7

typedef struct
{
    U32 temporaryOffset1;
    U32 temporaryOffset2;
} temporaryoffsetlist_t;

typedef struct
{
    U32 t;
    union {
        /* t = 1 */
        /* t = 2 */
        temporaryoffsetlist_t *pt10;
        /* t = 3 */
        temporaryoffsetlist_t *pt20;
        /* t = 4 */
        temporaryoffsetlist_t *pt30;
        /* t = 5 */
        temporaryoffsetlist_t *pt40;
        /* t = 6 */
        temporaryoffsetlist_t *pt50;
        /* t = 7 */
        temporaryoffsetlist_t *pt60;
    } u;
} penaltytime_ecn0_t;

typedef struct
{
    penaltytime_ecn0_t penaltyTime;
}umts_hcs_cellreselectinformation_ecn0_t;

typedef struct
{
    U8 hcs_prio;
    U8 q_hcs;
    umts_hcs_cellreselectinformation_ecn0_t hcs_cellreselectinformation;
}hcs_neighbouringcellinformation_ecn0_t;

typedef struct
{
    penaltytime_ecn0_t penaltytime;
} hcs_cellreselectinformation_ecn0_t;

#define RRC_UMTS_CellSelectReselectInfoSIB_11_12_HCS_ECN0_modeSpecificInfo_fdd 1
#define RRC_UMTS_CellSelectReselectInfoSIB_11_12_HCS_ECN0_modeSpecificInfo_tdd 2
#define RRC_UMTS_CellSelectReselectInfoSIB_11_12_HCS_ECN0_modeSpecificInfo_gsm 3

#define UMTS_SIB_CELLSELECTRESELECTINFOSIB_11_12_HCS_ECN0_MODESPECIFICiNFO_FDD_Q_QUALMIN_PRESENCE_FLAG		0x01
#define UMTS_SIB_CELLSELECTRESELECTINFOSIB_11_12_HCS_ECN0_MODESPECIFICiNFO_FDD_Q_RXLEVMIN_PRESENCE_FLAG		0x02
typedef struct
{
    U16		presence_bitmask;
    S8 q_qualmin;
    S8 q_RxlevMin;
}cellselectreselectinfosib_11_12_hcs_ecn0_modespecificInfo_fdd_t;

#define UMTS_SIB_CELLSELECTRESELECTINFOSIB_11_12_HCS_ECN0_MODESPECIFICINFO_TDD_Q_RXLEVMIN_PRESENCE_FLAG      0x01
typedef struct
{
    U16		presence_bitmask;
    S8 q_rxlevmin;   
}cellselectreselectinfosib_11_12_hcs_ecn0_modespecificInfo_tdd_t;


#define UMTS_SIB_CELLSELECTRESELECTINFOSIB_11_12_HCS_ECN0_MODESPECIFICINFO_Q_RXLEVMIN_PRESENCE_FLAG      0x01
typedef struct
{
    U16		presence_bitmask;
    S8 q_RxlevMin;
} cellselectreselectinfosib_11_12_hcs_ecn0_modespecificInfo_gsm_t;

typedef struct
{
    U32 t;
    union {
        /* t = 1 */
        cellselectreselectinfosib_11_12_hcs_ecn0_modespecificInfo_fdd_t *fdd;
        /* t = 2 */
        cellselectreselectinfosib_11_12_hcs_ecn0_modespecificInfo_tdd_t *tdd;
        /* t = 3 */
        cellselectreselectinfosib_11_12_hcs_ecn0_modespecificInfo_gsm_t *gsm;
    } u;
}cellselectreselectinfosib_11_12_hcs_ecn0_modespecificinfo_t;

#define UMTS_SIB_CELLSELECTRESELECTINFOSIB_11_12_HCS_ECNO_MAXALLOWEDUL_TX_POWER_PRESENCE_FLAG 		0x01
#define UMTS_SIB_CELLSELECTRESELECTINFOSIB_11_12_HCS_ECNO_HCS_NEIGHBOURINGCELLINFORMATION_ECN0pRESENT     0x02
typedef struct
{
    U16		presence_bitmask;
    S8 q_offset1s_n;
    S8 q_offset2s_n;
    S8 maxallowedul_tx_power;
    hcs_neighbouringcellinformation_ecn0_t hcs_neighbouringcellinformation_ecn0;
    cellselectreselectinfosib_11_12_hcs_ecn0_modespecificinfo_t modeSpecificInfo;
}cellselectreselectinfosib_11_12_hcs_ecno_t;

#define UMTS_SIB_PRIMARY_CCPCH_INFO_LCR_R4_CELL_PARAMS_ID_PRESENCE_FLAG     0x01
typedef struct
{
    U16             presence_bitmask;
    U8              tstd_indicator;
    U8              cell_parameters_id;
    U8              sctd_indicator;
}primary_ccpch_info_lcr_r4_t;


typedef struct
{
    U8          timeslot_number;
}timeslot_info_lcr_r4_t;

typedef struct
{
    U8                      count;
    timeslot_info_lcr_r4_t  timeslot_info_lcr_r4[MAX_NO_OF_TIMESLOTS_LCR];
}timeslot_info_list_lcr_r4_t;


#define UMTS_SIB_CELL_INFO_SI_HCS_ECNO_LCR_R4_REF_TIME_DIFF_TO_CELL_PRESENCE_FLAG       0x01
#define UMTS_SIB_CELL_INFO_SI_HCS_ECNO_LCR_R4_PRIMARY_CCPCH_TX_POWER_PRESENCE_FLAG      0x02
#define UMTS_SIB_CELL_INFO_SI_HCS_ECNO_LCR_R4_TIMESLOT_INFO_LIST_PRESENCE_FLAG          0x04
#define UMTS_SIB_CELL_INFO_SI_HCS_ECNO_LCR_R4_CELL_SELECT_RESELECT_INFO_PRESENCE_FLAG   0x08
typedef struct
{
    U16                                 presence_bitmask;
    S8                                  cell_individual_offset;
    reference_time_difference_to_cell_t reference_time_difference_to_cell;
    primary_ccpch_info_lcr_r4_t         primary_ccpch_info_lcr_r4;
    U8                                  primary_ccpch_tx_power;
    timeslot_info_list_lcr_r4_t         timeslot_info_list_lcr_r4;
    U8                                  read_sfn_indicator;
    cell_select_reselect_info_sib_11_12_hcs_ecno_t  cell_select_reselect_info_sib_11_12_hcs_ecno;
}cell_info_si_hcs_ecno_lcr_r4_t;

#define UMTS_SIB_NEWINTRAFREQCELLSI_HCS_ECNO_LCR_R4_INTRAFREQCELLID_PRESENCE_FLAG     0x01
typedef struct
{
    U16				presence_bitmask;
    U8 intrafreqcellid;
    cell_info_si_hcs_ecno_lcr_r4_t cellInfo;
}newintrafreqcellsi_hcs_ecn0_lcr_r4_t;

typedef struct
{
    U8 count;
    newintrafreqcellsi_hcs_ecn0_lcr_r4_t newintrafreqcellsi_hcs_ecn0_lcr_r4[MAX_CELL_MEAS];
} newintrafreqcellsi_list_hcs_ecn0_lcr_r4_t;

#define UMTS_SIB_INTRAFREQCELLINFOSI_LIST_HCS_ECN0_LCR_R4_REMOVEDINTRAFREQCELLLIST_PRESENCE_FLAG       0x01
typedef struct
{
    U16                                 presence_bitmask;
    removedintrafreqcelllist_t removed_intra_freq_cell_list;
    newintrafreqcellsi_list_hcs_ecn0_lcr_r4_t new_intra_freq_cell_list;
}intrafreqcellinfosi_list_hcs_ecn0_lcr_r4_t;

typedef struct
{
    U32 intra_freq_meas_quantity_fdd;
}intrafreqmeasquantity_modespecificinfo_fdd_t;


typedef struct
{
    U32 n;
    U32 elem[4];
}intrafreqmeasquantity_tddlist_t;

typedef struct
{
    intrafreqmeasquantity_tddlist_t	intra_freq_meas_quantity_tddlist;
}intrafreqmeasquantity_modespecificinfo_tdd_t;

#define RRC_UMTS_IntraFreqMeasQuantity_modeSpecificInfo_fdd 1
#define RRC_UMTS_IntraFreqMeasQuantity_modeSpecificInfo_tdd 2
typedef struct
{
    U32 t;
    union {
        intrafreqmeasquantity_modespecificinfo_fdd_t fdd;
        intrafreqmeasquantity_modespecificinfo_tdd_t tdd;
    } u;
}intrafreqmeasquantity_modespecificinfo_t;

typedef struct
{
    U32				filter_coefficient;
    intrafreqmeasquantity_modespecificinfo_t	mode_specific_info;
}intrafreqmeasquantity_t;


typedef struct
{
    U8	cpich_ec_no_reportingindicator;
    U8	cpich_rscp_reportingindicator;
    U8	pathloss_reportingIndicator;
}cellreportingquantities_modespecificInfo_fdd_t;

typedef struct
{
    U8	timeslotiscp_reporting_indicator;
    U8	proposedtgsn_reporting_required;
    U8	primaryccpch_rscp_reporting_indicator;
    U8	pathloss_reporting_indicator;
}cellreportingquantities_modespecificInfo_tdd_t;


#define RRC_UMTS_CellReportingQuantities_modeSpecificInfo_fdd 1
#define RRC_UMTS_CellReportingQuantities_modeSpecificInfo_tdd 2
typedef struct
{
    U32 t;
    union {
        cellreportingquantities_modespecificInfo_fdd_t fdd;
        cellreportingquantities_modespecificInfo_tdd_t tdd;
    } u;
}cellreportingquantities_modespecificinfo_t;


typedef struct
{
    U32				dummy;
    U8						cell_identity_reporting_indicator;
    U8						cell_synchronisation_info_reporting_indicator;
    cellreportingquantities_modespecificinfo_t	mode_specific_info;
}cellreportingquantities_t;

#define UMTS_SIB_INTRAFREQREPORTINGQUANTITY_DETECTEDSETREPORTINGQUANTITIES_PRESENCE_FLAG    0x01
typedef struct
{
    U16				presence_bitmask;
    cellreportingquantities_t	active_set_reporting_quantities;
    cellreportingquantities_t	monitored_set_reporting_quantities;
    cellreportingquantities_t	detected_set_reporting_quantities;
}intrafreqreportingquantity_t;


typedef struct
{
    U32		measurement_report_transfer_mode;
    U32	periodical_or_event_trigger;
}measurementreportingmode_t;

typedef struct
{
    U32 n;
    U32 elem[2];
}intrafreqrepquantityrach_tddlist_t;

typedef struct
{
    intrafreqrepquantityrach_tddlist_t	intrafreqrepquantityrach_tddlist;
}intrafreqreportingquantityforrach_modespecificinfo_tdd_t;


typedef struct
{
    U32	intrafreqrepquantityrach_fdd;
} intrafreqreportingquantityforrach_modespecificinfo_fdd_t;


#define RRC_UMTS_IntraFreqReportingQuantityForRACH_modeSpecificInfo_fdd 1
#define RRC_UMTS_IntraFreqReportingQuantityForRACH_modeSpecificInfo_tdd 2

typedef struct
{
    U32 t;
    union {
        /* t = 1 */
        intrafreqreportingquantityforrach_modespecificinfo_fdd_t fdd;
        /* t = 2 */
        intrafreqreportingquantityforrach_modespecificinfo_tdd_t tdd;
    } u;
}intrafreqreportingquantityforrach_modeSpecificInfo_t;


typedef struct
{
    U32						sfn_sfn_otd_type;
    intrafreqreportingquantityforrach_modeSpecificInfo_t		mode_specific_info;
}intrafreqreportingquantityforrach_t;


#define RRC_UMTS_FrequencyInfo_modeSpecificInfo_fdd 1
#define RRC_UMTS_FrequencyInfo_modeSpecificInfo_tdd 2
typedef struct
{
    U32 t;
    union {
        frequency_info_fdd_t fdd;
        frequency_info_tdd_t tdd;
    } u;
}frequencyinfo_modespecificinfo_t;

#define UMTS_SIB_PRIMARYCCPCH_INFO_LCR_R4_CELLPARAMETERSID_PRESENCE_FLAG       0x01
typedef struct
{
    U16						presence_bitmask;
    U8 tstd_Indicator;
    U8 cellparameters_id;
    U8 sctd_Indicator;
}primaryccpch_info_lcr_r4_t;

#define UMTS_SIB_NEWINTERFREQCELLSI_HCS_ECN0_LCR_R4_INTERFREQCELLID_PRESENCE_FLAG    0x01
#define UMTS_SIB_NEWINTERFREQCELLSI_HCS_ECN0_LCR_R4_FREQUENCYINFO_PRESENCE_FLAG     0x02
typedef struct
{
    U16						presence_bitmask;
    U8 inter_freq_cell_id;
    frequency_info_t frequency_info;
    cell_info_si_hcs_ecno_lcr_r4_t cell_info;
}newinterfreqcellsi_hcs_ecn0_lcr_r4_t;

typedef struct
{
    U8 count;
    newinterfreqcellsi_hcs_ecn0_lcr_r4_t newinterfreqcellsi_hcs_ecn0_lcr_r4[MAX_CELL_MEAS];
} newinterfreqcellsi_list_hcs_ecn0_lcr_r4_t;

typedef struct
{
    U32 n;
    U8 elem[32];
}removedinterfreqcelllist_removesomeinterfreqcells_t;


#define RRC_UMTS_RemovedInterFreqCellList_removeAllInterFreqCells 1
#define RRC_UMTS_RemovedInterFreqCellList_removeSomeInterFreqCells 2
#define RRC_UMTS_RemovedInterFreqCellList_removeNoInterFreqCells 3
typedef struct
{
    U32 t;
    union {
        /* t = 1 */
        /* t = 2 */
        removedinterfreqcelllist_removesomeinterfreqcells_t removeSomeInterFreqCells;
        /* t = 3 */
    } u;
}removedinterfreqcelllist_t;


#define UMTS_SIB_INTERFREQCELLINFOSI_LIST_HCS_ECN0_LCR_REMOVEDINTERFREQCELLLIST_PRESENCE_FLAG		0x01
#define UMTS_SIB_INTERFREQCELLINFOSI_LIST_HCS_ECN0_LCR_NEWINTERFREQcELLLIST_PRESENCE_FLAG			0x02
typedef struct
{
    U16						presence_bitmask;
    removedinterfreqcelllist_t removed_inter_freq_cell_list;
    newinterfreqcellsi_list_hcs_ecn0_lcr_r4_t new_inter_freq_cell_list;
}interfreqcellinfosi_list_hcs_ecn0_lcr_t;

#define UMTS_SIB_INTERFREQMEASUREMENTSYSINFO_HCS_ECN0_LCR_R4_INTERFREQCELLINFOSI_LIST_PRESENCE_FLAG        0x01
typedef struct
{
    U16                                 presence_bitmask;
    interfreqcellinfosi_list_hcs_ecn0_lcr_t inter_freq_cell_info_si_list;
}interfreqmeasurementsysinfo_hcs_ecn0_lcr_r4_t;

typedef struct
{
    U8	inter_freq_tdd128_meas_ind;
}fach_measurementoccasioninfo_lcr_r4_ext_t;

#define UMTS_SIB_CELLSELECTRESELECTINFO_V590EXT_DELTAQRXLEVMIN_PRESENCE_FLAG 		0x01
#define UMTS_SIB_CELLSELECTRESELECTINFO_V590EXT_DELTAQHCS_PRESENCE_FLAG 			0x02
typedef struct
{
    U16	presence_bitmask;
    S8	deltaqrxlevmin;
    S8  deltaqhcs;
} cellselectreselectinfo_v590ext_t;



typedef struct
{
    U8 count;
    cellselectreselectinfo_v590ext_t  cellselectreselectinfo_v590ext[MAX_CELL_MEAS];
} sysinfotype11_v590ext_ies_newintrafrequencycellinfolist_v590ext_t;

typedef struct
{
    U8 count;
    cellselectreselectinfo_v590ext_t cellselectreselectinfo_v590ext[MAX_CELL_MEAS];
} sysinfotype11_v590ext_ies_newinterfrequencycellinfolist_v590ext_t;

typedef struct 
{
    U8 count;
    cellselectreselectinfo_v590ext_t cellselectreselectinfo_v590ext[MAX_CELL_MEAS];
}sysinfotype11_v590ext_ies_newinterratcellinfolist_v590ext_t;

typedef S8 deltarscp_t;
typedef struct
{
    U32 n;
    deltarscp_t elem[8];
} intra_freqeventcriterialist_v590ext_t;


typedef struct
{
    U32		reporting_amount;
    U32		reporting_interval;
}periodicreportinginfo_1b_t;

typedef struct
{
    periodicreportinginfo_1b_t	periodic_reporting_info_1b;
}intrafreqreportingcriteria_1b_r5_t;


#define UMTS_SIB_INTRAFREQEVENT_1D_R5_TRIGGERINGCONDITION_PRESENCE_FLAG     0x01
#define UMTS_SIB_INTRAFREQEVENT_1D_R5_USECIO_PRESENCE_FLAG                  0x02
typedef struct
{
    U16                                 presence_bitmask;
    U32 triggering_condition;
    U8 use_cio;
} intrafreqevent_1d_r5_t;

#define UMTS_SIB_SYSINFOTYPE11_V590EXT_IES_NEWINTRAFREQUENCYCELLINFOLIST_V590EXT_PRESENCE_FLAG		0x01
#define UMTS_SIB_SYSINFOTYPE11_V590EXT_IES_NEWINTERFREQUENCYCELLINFOLIST_V590EXT_PRESENCE_FLAG		0x02
#define UMTS_SIB_SYSINFOTYPE11_V590EXT_IES_NEWINTERRATCELLINFOLIST_V590EXT_PRESENCE_FLAG		0x04
#define UMTS_SIB_SYSINFOTYPE11_V590EXT_IES_INTRAFREQEVENTCRITERIALIST_V590EXT_PRESENCE_FLAG		0x08
#define UMTS_SIB_SYSINFOTYPE11_V590EXT_IES_INTRAFREQREPORTINGcRITERIA_1B_R5_PRESENCE_FLAG		0x10
#define UMTS_SIB_SYSINFOTYPE11_V590EXT_IES_INTRAFREQEVENT_1D_R5_PRESENCE_FLAG				0x20
typedef struct
{
    U16		presence_bitmask;
    sysinfotype11_v590ext_ies_newintrafrequencycellinfolist_v590ext_t	new_intra_frequency_cell_info_list_v590ext;
    sysinfotype11_v590ext_ies_newinterfrequencycellinfolist_v590ext_t	new_inter_frequency_cell_info_list_v590ext;
    sysinfotype11_v590ext_ies_newinterratcellinfolist_v590ext_t		newinterratcell_info_list_v590ext;
    intra_freqeventcriterialist_v590ext_t intra_freq_event_criteria_list_v590ext;
    intrafreqreportingcriteria_1b_r5_t intra_freq_reporting_criteria_1b_r5;
    intrafreqevent_1d_r5_t intra_freq_event_1d_r5;
} sysinfotype11_v590ext_ies_t;

typedef S8 threshold_t;

typedef struct
{
    U32 inter_freq_rep_quantity_rach_fdd;
}dummy_interfreqrachreportinginfo_modespecificinfo_fdd_t;

typedef struct
{
    U32 n;
    U32 elem[2];
}interfreqrepquantityrach_tddlist_t;


typedef struct
{
    interfreqrepquantityrach_tddlist_t	inter_freq_rep_quantity_rach_tdd_list; 
}dummy_interfreqrachreportingInfo_modespecificinfo_tdd_t;


#define RRC_UMTS_Dummy_InterFreqRACHReportingInfo_modeSpecificInfo_fdd 1
#define RRC_UMTS_Dummy_InterFreqRACHReportingInfo_modeSpecificInfo_tdd 2
#define RRC_UMTS_Dummy_InterFreqRACHReportingInfo_modeSpecificInfo_interFreqRACHReportingThreshold 3
#define RRC_UMTS_Dummy_InterFreqRACHReportingInfo_modeSpecificInfo_maxReportedCellsOnRACHinterFreq 4

typedef struct
{
    U32 t;
    union {
        /* t = 1 */
        dummy_interfreqrachreportinginfo_modespecificinfo_fdd_t fdd;
        /* t = 2 */
        dummy_interfreqrachreportingInfo_modespecificinfo_tdd_t tdd;
        /* t = 3 */
        threshold_t			inter_freqrachreporting_threshold;
        /* t = 4 */
        U8	max_reported_cells_onrachinter_freq;
    } u;
}dummy_interfreqrachreportinginfo_modespecificinfo_t;

typedef struct
{
    dummy_interfreqrachreportinginfo_modespecificinfo_t	mode_specific_info;
}dummy_interfreqrachreportinginfo_t;

#define UMTS_SIB_SYSINFOTYPE11_V690EXT_IES_DUMMY_PRESENCE_FLAG      0x01
typedef struct
{
    U16					presence_bitmask;
    dummy_interfreqrachreportinginfo_t	dummy;
}sysinfotype11_v690ext_ies_t;


typedef struct
{
    U32	inter_freq_rep_quantityrach_fdd;
}interfreqrachreportinginfo_modespecificinfo_fdd_t;

typedef struct
{
    interfreqrepquantityrach_tddlist_t	inter_freq_repquantityrach_tdd_list;
}interfreqrachreportinginfo_modespecificinfo_tdd_t;

#define RRC_UMTS_InterFreqRACHReportingInfo_modeSpecificInfo_fdd 1
#define RRC_UMTS_InterFreqRACHReportingInfo_modeSpecificInfo_tdd 2
typedef struct
{
    U32 t;
    union {
        /* t = 1 */
        interfreqrachreportinginfo_modespecificinfo_fdd_t fdd;
        /* t = 2 */
        interfreqrachreportinginfo_modespecificinfo_tdd_t tdd;
    } u;
}interfreqrachreportinginfo_modespecificinfo_t;

typedef struct
{
    interfreqrachreportinginfo_modespecificinfo_t	mode_specific_info;
    threshold_t						inter_freqrachreporting_threshold;
    U8			max_reported_cells_on_rach_inter_freq;
}interfreqrachreportinginfo_t;

#define UMTS_SIB_SYSINFOTYPE11_V6B0EXT_IES_INTERFREQRACHREPORTINGINFO_PRESENCE_FLAG     0x01
typedef struct
{
    U16                          presence_bitmask;
    interfreqrachreportinginfo_t	inter_freq_rachreporting_info;
}sysinfotype11_v6b0ext_ies_t;


#define UMTS_SIB_MBSFNFREQUENCY_CELLPARAMETERSID_PRESENCE_FLAG 	0x01
typedef struct
{
    U16                          presence_bitmask;
    frequency_info_t frequencyinfo;
    U8 cellparametersid;
}mbsfnfrequency_t;

typedef struct
{
    U8 count;
    mbsfnfrequency_t mbsfnfrequency[MAX_MBSFN_CLUSTERS];
} mbsfnfrequencylist_t;

#define UMTS_SIB_SYSINFOTYPE11_V770EXT_IES_MBSFNFREQUENCYLIST_PRESENCE_FLAG        0x01
typedef struct
{
    U16                          presence_bitmask;
    mbsfnfrequencylist_t		mbsfn_frequency_list;
}sysinfotype11_v770ext_ies_t;

typedef struct
{
    U8 intrasecondaryfreqindicator;
}newinterfreqcell_v7b0ext_t;

typedef struct
{
    U8 count;
    newinterfreqcell_v7b0ext_t   newinterfreqcell_v7b0ext[MAX_CELL_MEAS];
}newinterfreqcelllist_v7b0ext_t;

#define UMTS_SIB_SYSINFOTYPE11_V7B0EXT_IES_NEWINTERFREQCELLLIST_PRESENCE_FLAG     0x01
typedef struct
{
    U16					presence_bitmask;
    newinterfreqcelllist_v7b0ext_t	new_inter_freq_cell_list;
}sysinfotype11_v7b0ext_ies_t;



#define UMTS_SIB_MBSFNFREQUENCY_V860EXT_IMB_INDICATION_PRESENCE_FLAG 	0x01
typedef struct
{
    U16 presence_bitmask;
    U32 imb_indication;
}mbsfnfrequency_v860ext_t;

typedef struct
{
    U8 count;
    mbsfnfrequency_v860ext_t mbsfnfrequency_v860ext[MAX_MBSFN_CLUSTERS];
} mbsfnfrequencylist_v860ext_t;

#define UMTS_SIB_SYSINFOTYPE11_V860EXT_IES_MBSFNFREQUENCYLIST_PRESENCE_FLAG      0x01
typedef struct
{
    U16							presence_bitmask;
    mbsfnfrequencylist_v860ext_t		mbsfn_frequency_list;
}sysinfotype11_v860ext_ies_t;

typedef struct
{
    U8 __dummy__;
}sysinfotype11_v4b0nces_v590nce_v690nces_v6b0nces_v770nces_v7b0nces_v860nces_nces_t;

#define UMTS_SIB_SYSINFOTYPE11_V4B0NCES_V590NCE_V690NCES_V6B0NCES_V770NCES_V7B0NCES_V860NCES_NONCRITICALEXTENSIONS_PRESENCE_FLAG      0x01
typedef struct
{
    U16							presence_bitmask;
    sysinfotype11_v860ext_ies_t				sys_info_type11_v860ext;
    sysinfotype11_v4b0nces_v590nce_v690nces_v6b0nces_v770nces_v7b0nces_v860nces_nces_t	non_critical_extensions;
}sysinfotype11_v4b0nces_v590nce_v690nces_v6b0nces_v770nces_v7b0nces_v860nces_t;


#define UMTS_SIB_SYSINFOTYPE11_V4B0NCES_V590NCE_V690NCES_V6B0NCES_V770NCES_V7B0NCES_V860NONCRITICALEXTENSIONS_PRESENCE_FLAG      0x01
typedef struct
{
    U16						presence_bitmask;
    sysinfotype11_v7b0ext_ies_t			sys_info_type11_v7b0ext;
    sysinfotype11_v4b0nces_v590nce_v690nces_v6b0nces_v770nces_v7b0nces_v860nces_t v860_non_critical_extensions;
}sysinfotype11_v4b0nces_v590nce_v690nces_v6b0nces_v770nces_v7b0nces_t;

#define UMTS_SIB_SYSINFOTYPE11_V4B0NCES_V590NCE_V690NCES_V6B0NCES_V770NCES_V7B0NONCRITICALEXTENSIONS_PRESENCE_FLAG     0x01
typedef struct
{
    U16									presence_bitmask;
    sysinfotype11_v770ext_ies_t						sys_info_type11_v770ext;
    sysinfotype11_v4b0nces_v590nce_v690nces_v6b0nces_v770nces_v7b0nces_t	v7b0_non_critical_extensions;
}sysinfotype11_v4b0nces_v590nce_v690nces_v6b0nces_v770nces_t;

#define UMTS_SIB_SYSINFOTYPE11_V4B0NCES_V590NCES_V690NCES_V6B0NCES_V770NONCRITICALEXTENSIONS_PRESENCE_FLAG     0x01
typedef struct
{
    U16								presence_bitmask;
    sysinfotype11_v6b0ext_ies_t					sys_info_type11_v6b0ext;
    sysinfotype11_v4b0nces_v590nce_v690nces_v6b0nces_v770nces_t	v770noncritical_extensions;
}sysinfotype11_v4b0nces_v590nces_v690nces_v6b0nces_t;


#define UMTS_SIB_SYSINFOTYPE11_V4B0NONCRITICALEXTENSIONS_V590NONCRITICALEXTENSION_V690NONCRITICALEXTENSIONS_V6B0NONCRITICALEXTENSIONS_PRESENCE_FLAG      0x01
typedef struct
{
    U16							presence_bitmask;
    sysinfotype11_v690ext_ies_t				sys_info_type11_v690ext;
    sysinfotype11_v4b0nces_v590nces_v690nces_v6b0nces_t	v6b0_non_critical_extensions;
}sysinfotype11_v4b0noncriticalextensions_v590noncriticalextension_v690noncriticalextensions_t;


#define UMTS_SIB_SYSINFOTYPE11_V4B0_NC_EXTENSIONS_V590_NC_EXTENSION_V690NONCRITICALEXTENSIONS_PRESENCE_FLAG       0x01
typedef struct
{
    U16					presence_bitmask;
    sysinfotype11_v590ext_ies_t		sys_info_type11_v590ext;
    sysinfotype11_v4b0noncriticalextensions_v590noncriticalextension_v690noncriticalextensions_t v690_non_critical_extensions;
}sysinfotype11_v4b0_nc_extensions_v590_nc_extension_t;

#define UMTS_SIB_CELLSELECTRESELECTINFOSIB_11_12_RSCP_MODESPECIFICINFO_FDD_Q_QUALMIN_PRESENCE_FLAG         0x01
#define UMTS_SIB_CELLSELECTRESELECTINFOSIB_11_12_RSCP_MODESPECIFICINFO_FDD_Q_RXLEVMIN_PRESENCE_FLAG         0x02
typedef struct
{
    U16	presence_bitmask;
    S8 q_QualMin;
    S8 q_RxlevMin;
}cellselectreselectinfosib_11_12_rscp_modespecificinfo_fdd_t;

#define UMTS_SIB_CELLSELECTRESELECTINFOSIB_11_12_RSCP_MODESPECIFICINFO_TDD_Q_RXLEVMIN_PRESENCE_FLAG   0x01
typedef struct
{
    U16						presence_bitmask;
    S8 q_rx_lev_min;
}cellselectreselectinfosib_11_12_rscp_modespecificinfo_tdd_t;

#define UMTS_SIB_CELLSELECTRESELECTINFOSIB_11_12_RSCP_MODESPECIFICINFO_GSM_Q_RXLEVMIN_PRESENCE_FLAG        0x01
typedef struct
{
    U16	presence_bitmask;
    S8 q_RxlevMin;
}cellselectreselectinfosib_11_12_rscp_modespecificinfo_gsm_t;

#define RRC_UMTS_CellSelectReselectInfoSIB_11_12_RSCP_modeSpecificInfo_fdd 1
#define RRC_UMTS_CellSelectReselectInfoSIB_11_12_RSCP_modeSpecificInfo_tdd 2
#define RRC_UMTS_CellSelectReselectInfoSIB_11_12_RSCP_modeSpecificInfo_gsm 3
typedef struct
{
    U32 t;
    union {
        /* t = 1 */
        cellselectreselectinfosib_11_12_rscp_modespecificinfo_fdd_t fdd;
        /* t = 2 */
        cellselectreselectinfosib_11_12_rscp_modespecificinfo_tdd_t tdd;
        /* t = 3 */
        cellselectreselectinfosib_11_12_rscp_modespecificinfo_gsm_t gsm;
    } u;
} cellselectreselectinfosib_11_12_rscp_modespecificinfo_t;

#define UMTS_SIB_CELLSELECTRESELECTINFOSIB_11_12_RSCP_MAXALLOWEDUL_TX_POWER_PRESENCE_FLAG      0x01
typedef struct 
{
    U16						presence_bitmask;
    S8  q_OffsetS_N;
    S8 maxAllowedUL_TX_Power;
    cellselectreselectinfosib_11_12_rscp_modespecificinfo_t mode_specific_info;
}cellselectreselectinfosib_11_12_rscp_t;

#define UMTS_SIB_CELLINFOSI_RSCP_LCR_R4_REFERENCETIMEDIFFERENCETO_CELL_PRESENCE_FLAG     0x01
#define UMTS_SIB_CELLINFOSI_RSCP_LCR_R4_PRIMARYCCPCH_TX_POWER_PRESENCE_FLAG     0x02
#define UMTS_SIB_CELLINFOSI_RSCP_LCR_R4_TIMESLOTINFOLIST_PRESENCE_FLAG       0x04
#define UMTS_SIB_CELLINFOSI_RSCP_LCR_R4_CELLSELECTIONRESELECTIONINFO_PRESENCE_FLAG   0x08
typedef struct
{
    U16						presence_bitmask;
    S8 cell_individual_offset;
    referencetimedifferencetocell_t reference_time_difference_to_cell;
    primaryccpch_info_lcr_r4_t primaryCCPCH_Info;
    U8 primaryccpch_tx_power;
    timeslotinfolist_lcr_r4_t timeslotinfo_list;
    U8 readsfn_indicator;
    cellselectreselectinfosib_11_12_rscp_t cellselection_reselection_info;
}cellinfosi_rscp_lcr_r4_t;

#define UMTS_SIB_NEWINTRAFREQCELLSI_RSCP_LCR_R4_INTRAFREQCELL_ID_PRESENCE_FLAG      0x01
typedef struct
{
    U16		presence_bitmask;
    U8 intra_freq_cell_id;
    cellinfosi_rscp_lcr_r4_t cell_info;
}newintrafreqcellsi_rscp_lcr_r4_t;

typedef struct
{
    U8 count;
    newintrafreqcellsi_rscp_lcr_r4_t newintrafreqcellsi_rscp_lcr_r4[MAX_CELL_MEAS];
}newintrafreqcellsi_list_rscp_lcr_r4_t;

#define UMTS_SIB_INTRAFREQCELLINFOSI_LIST_RSCP_LCR_R4_REMOVEDINTRAFREQCELLLIST_PRESENCE_FLAG  0x01
typedef struct
{
    U16						presence_bitmask;
    removedintrafreqcelllist_t			removed_intra_freq_cell_list;
    newintrafreqcellsi_list_rscp_lcr_r4_t	newintra_freq_cell_list;
}intrafreqcellinfosi_list_rscp_lcr_r4_t;

typedef struct
{
    U32 t;
    union {
        intrafreqreportingquantityforrach_modespecificinfo_fdd_t *fdd;
        intrafreqreportingquantityforrach_modespecificinfo_tdd_t *tdd;
    } u;
} intrafreqreportingquantityforrach_t_modeSpecificInfo;

typedef struct
{
    primaryccpch_info_lcr_r4_t tdd;
} forbiddenaffectcell_lcr_r4_t;

typedef struct
{
    U8 count;
    forbiddenaffectcell_lcr_r4_t   forbiddenaffectcell_lcr_r4[MAX_CELL_MEAS];
}forbiddenaffectcelllist_lcr_r4_t;

#define UMTS_SIB_EVENT1A_LCR_R4_FORBIDDENAFFECTCELLLIST_PRESENCE_FLAG     0x01
typedef struct
{
    U16 presence_bitmask;
    U32 triggering_condition;
    U8 reporting_range;
    forbiddenaffectcelllist_lcr_r4_t forbidden_affect_cell_list;
    U8 w;
    U32 report_deactivation_threshold;
    U32 reporting_amount;
    U32 reporting_interval;
}event1a_lcr_r4_t;


#define UMTS_SIB_EVENT1B_LCR_R4_FORBIDDENAFFECTCELLLIST_PRESENCE_FLAG     0x01
typedef struct
{
    U16 presence_bitmask;
    U32 triggering_condition;
    U8 reporting_range;
    forbiddenaffectcelllist_lcr_r4_t forbiddenAffectCellList;
    U8 w;
}event1b_lcr_r4_t;

typedef struct
{
    U32 replacementActivationThreshold;
    U32 reportingAmount;
    U32 reportingInterval;
}event1c_t;

typedef struct
{
    U32 triggeringCondition;
    S16 thresholdUsedFrequency;
}event1e_t;

typedef struct
{
    U32 triggeringCondition;
    S16 thresholdUsedFrequency;
}event1f_t;

#define RRC_UMTS_IntraFreqEvent_LCR_r4_e1a 1
#define RRC_UMTS_IntraFreqEvent_LCR_r4_e1b 2
#define RRC_UMTS_IntraFreqEvent_LCR_r4_e1c 3
#define RRC_UMTS_IntraFreqEvent_LCR_r4_e1d 4
#define RRC_UMTS_IntraFreqEvent_LCR_r4_e1e 5
#define RRC_UMTS_IntraFreqEvent_LCR_r4_e1f 6
#define RRC_UMTS_IntraFreqEvent_LCR_r4_e1g 7
#define RRC_UMTS_IntraFreqEvent_LCR_r4_e1h 8
#define RRC_UMTS_IntraFreqEvent_LCR_r4_e1i 9

typedef struct
{
    U32 t;
    union {
        /* t = 1 */
        event1a_lcr_r4_t e1a;
        /* t = 2 */
        event1b_lcr_r4_t e1b;
        /* t = 3 */
        event1c_t e1c;
        /* t = 4 */
        /* t = 5 */
        event1e_t e1e;
        /* t = 6 */
        event1f_t e1f; 
        /* t = 7 */
        /* t = 8 */
        S16 e1h;
        /* t = 9 */
        S16 e1i;
    } u;
}intrafreqevent_lcr_r4_t;

#define RRC_UMTS_ReportingCellStatus_withinActiveSet 1
#define RRC_UMTS_ReportingCellStatus_withinMonitoredSetUsedFreq 2
#define RRC_UMTS_ReportingCellStatus_withinActiveAndOrMonitoredUsedFreq 3
#define RRC_UMTS_ReportingCellStatus_withinDetectedSetUsedFreq 4
#define RRC_UMTS_ReportingCellStatus_withinMonitoredAndOrDetectedUsedFreq 5
#define RRC_UMTS_ReportingCellStatus_allActiveplusMonitoredSet 6
#define RRC_UMTS_ReportingCellStatus_allActivePlusDetectedSet 7
#define RRC_UMTS_ReportingCellStatus_allActivePlusMonitoredAndOrDetectedSet 8
#define RRC_UMTS_ReportingCellStatus_withinVirtualActSet 9
#define RRC_UMTS_ReportingCellStatus_withinMonitoredSetNonUsedFreq 10
#define RRC_UMTS_ReportingCellStatus_withinMonitoredAndOrVirtualActiveSetNonUsedFreq 11
#define RRC_UMTS_ReportingCellStatus_allVirtualActSetplusMonitoredSetNonUsedFreq 12
#define RRC_UMTS_ReportingCellStatus_withinActSetOrVirtualActSet_InterRATcells 13
#define RRC_UMTS_ReportingCellStatus_withinActSetAndOrMonitoredUsedFreqOrVirtualActSetAndOrMonitoredNonUsedFreq 14

typedef struct
{
    U32 t;
    union {

        /* t = 1 */
        U32 withinActiveSet;
        /* t = 2 */
        U32 withinMonitoredSetUsedFreq;
        /* t = 3 */
        U32 withinActiveAndOrMonitoredUsedFreq;
        /* t = 4 */
        U32 withinDetectedSetUsedFreq;
        /* t = 5 */
        U32 withinMonitoredAndOrDetectedUsedFreq;
        /* t = 6 */
        U32 allActiveplusMonitoredSet;
        /* t = 7 */
        U32 allActivePlusDetectedSet;
        /* t = 8 */
        U32 allActivePlusMonitoredAndOrDetectedSet;
        /* t = 9 */
        U32 withinVirtualActSet;
        /* t = 10 */
        U32 withinMonitoredSetNonUsedFreq;
        /* t = 11 */
        U32 withinMonitoredAndOrVirtualActiveSetNonUsedFreq;
        /* t = 12 */
        U32 allVirtualActSetplusMonitoredSetNonUsedFreq;
        /* t = 13 */
        U32 withinActSetOrVirtualActSet_InterRATcells;
        /* t = 14 */
        U32 withinActSetAndOrMonitoredUsedFreqOrVirtualActSetAndOrMonitoredNonUsedFreq;

    } u;
}inreportingcellstatus_t;


#define UMTS_SIB_INTRAFREQEVENTCRITERIA_LCR_R4_REPORTINGCELLSTATUS_PRESENCE_FLAG      0x01
typedef struct
{
    U16                                 presence_bitmask;
    intrafreqevent_lcr_r4_t event;
    U8 hysteresis;
    U32 timeToTrigger;
    inreportingcellstatus_t reporting_cell_status;
}intrafreqeventcriteria_lcr_r4_t;

typedef struct
{
    U8 count;
    intrafreqeventcriteria_lcr_r4_t intrafreqeventcriteria_lcr_r4[MAX_MEAS_EVENT];
} intrafreqeventcriterialist_lcr_r4_t;

#define UMTS_SIB_INTRAFREQREPORTINGCRITERIA_LCR_R4_EVENTCRITERIALIST_PRESENCE_FLAG      0x01
typedef struct
{
    U16                                 presence_bitmask;
    intrafreqeventcriterialist_lcr_r4_t event_criteria_list;
} intrafreqreportingcriteria_lcr_r4_t;


typedef struct
{
    U32		reporting_amount;
    U32	reporting_interval;
}periodicalreportingcriteria_t;

#define RRC_UMTS_CellDCH_ReportCriteria_LCR_r4_intraFreqReportingCriteria 1
#define RRC_UMTS_CellDCH_ReportCriteria_LCR_r4_periodicalReportingCriteria 2

typedef struct
{
    U32 t;
    union {
        intrafreqreportingcriteria_lcr_r4_t	intra_freq_reporting_criteria;
        periodicalreportingcriteria_t		periodical_reporting_criteria;
    } u;
}celldch_reportcriteria_lcr_r4_t;


typedef struct
{
    intrafreqreportingquantity_t		intra_freq_reporting_quantity;
    measurementreportingmode_t		measurement_reporting_mode;
    celldch_reportcriteria_lcr_r4_t	report_criteria;
}reportinginfoforcelldch_lcr_r4_t;

#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_RSCP_LCR_R4_INTRAFREQCELLINFOSI_LIST_PRESENCE_FLAG              0x01
#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_RSCP_LCR_R4_INTRAFREQMEASQUANTITY_PRESENCE_FLAG                 0x02
#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_RSCP_LCR_R4_INTRAFREQREPORTINGQUANTITYFORRACH_PRESENCE_FLAG     0x04
#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_RSCP_LCR_R4_MAXREPORTEDCELLSONRACH_PRESENCE_FLAG                0x08
#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_RSCP_LCR_R4_REPORTINGINFOFORCELLDCH_PRESENCE_FLAG               0x10
typedef struct
{
    U16						presence_bitmask;
    U8			intra_freq_measurement_id;
    intrafreqcellinfosi_list_rscp_lcr_r4_t	intra_freq_cellinfosi_list;
    intrafreqmeasquantity_t			intra_freq_meas_quantity;
    intrafreqreportingquantityforrach_t		intra_freq_reporting_quantity_for_rach;
    U32			max_reported_cell_son_rach;
    reportinginfoforcelldch_lcr_r4_t		reporting_info_for_celldch;
}intrafreqmeasurementsysinfo_rscp_lcr_r4_t;

#define UMTS_SIB_NEWINTERFREQCELLSI_RSCP_LCR_R4_INTERFREQCELLID_PRESENCE_FLAG     0x01
#define UMTS_SIB_NEWINTERFREQCELLSI_RSCP_LCR_R4_FREQUENCYINFO_PRESENCE_FLAG       0x02
typedef struct
{
    U16                                 presence_bitmask;
    U8 inter_freq_cell_id;
    frequency_info_t frequency_info;
    cellinfosi_rscp_lcr_r4_t cell_info;
}newinterfreqcellsi_rscp_lcr_r4_t;

typedef struct
{
    U8 count;
    newinterfreqcellsi_rscp_lcr_r4_t newinterfreqcellsi_rscp_lcr_r4[MAX_CELL_MEAS];
} newinterfreqcellsi_list_rscp_lcr_r4_t;

#define UMTS_SIB_INTERFREQCELLINFOSI_LIST_RSCP_LCR_REMOVEDINTERFREQCELLLIST_PRESENCE_FLAG        0x01
#define UMTS_SIB_INTERFREQCELLINFOSI_LIST_RSCP_LCR_NEWiNTERFREQCELLLIST_PRESENCE_FLAG            0x02
typedef struct
{
    U16                                 presence_bitmask;
    removedinterfreqcelllist_t removedInterFreqCellList;
    newinterfreqcellsi_list_rscp_lcr_r4_t new_inter_freq_cell_list;
} interfreqcellinfosi_list_rscp_lcr_t;

#define UMTS_SIB_INTERFREQMEASUREMENTSYSINFO_RSCP_LCR_R4_INTERFREQCELLINFOSI_LIST_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                 presence_bitmask;
    interfreqcellinfosi_list_rscp_lcr_t inter_freq_cell_info_si_list;
}interfreqmeasurementsysinfo_rscp_lcr_r4_t;

#define UMTS_SIB_MEASUREMENTCONTROLSYSINFO_LCR_R4_EXT_USE_OF_HCS_HCS_NOT_USED_CELLSELECTQUALITYMEASURE_CPICH_RSCP_INTRAFREQMEASUREMENTSYSINFO_PRESENCE_FLAG     0x01
#define UMTS_SIB_MEASUREMENTCONTROLSYSINFO_LCR_R4_EXT_USE_OF_HCS_HCS_NOT_USED_CELLSELECTQUALITYMEASURE_CPICH_RSCP_INTERFREQMEASUREMENTSYSINFO_PRESENCE_FLAG     0x02
typedef struct
{
    U16                                 presence_bitmask;
    intrafreqmeasurementsysinfo_rscp_lcr_r4_t intra_freq_measurement_sysInfo;
    interfreqmeasurementsysinfo_rscp_lcr_r4_t inter_freq_measurement_sysInfo;
}measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_not_used_cellselectqualitymeasure_cpich_rscp_t;


#define UMTS_SIB_CELLSELECTRESELECTINFOSIB_11_12_ECN0_MODESPECIFICINFO_FDD_Q_QUALMIN_PRESENCE_FLAG   0x01
#define UMTS_SIB_CELLSELECTRESELECTINFOSIB_11_12_ECN0_MODESPECIFICINFO_FDD_Q_RXLEVMIN_PRESENCE_FLAG  0x02
typedef struct
{
    U16                                 presence_bitmask;
    S8 q_QualMin;
    S8 q_RxlevMin;
}cellselectreselectinfosib_11_12_ecn0_modespecificinfo_fdd_t;


#define UMTS_SIB_CELLSELECTRESELECTINFOSIB_11_12_ECN0_MODESPECIFICINFO_TDD_Q_RXLEVMIN_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                 presence_bitmask;
    S8 q_RxlevMin;
}cellselectreselectinfosib_11_12_ecn0_modespecificinfo_tdd_t;

#define UMTS_SIB_CELLSELECTRESELECTINFOSIB_11_12_ECN0_MODESPECIFICINFO_GSM_Q_RXLEVMIN_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                 presence_bitmask;
    S8 q_RxlevMin;
}cellselectreselectinfosib_11_12_ecn0_modespecificinfo_gsm_t;

#define RRC_UMTS_CellSelectReselectInfoSIB_11_12_ECN0_modeSpecificInfo_fdd 1
#define RRC_UMTS_CellSelectReselectInfoSIB_11_12_ECN0_modeSpecificInfo_tdd 2
#define RRC_UMTS_CellSelectReselectInfoSIB_11_12_ECN0_modeSpecificInfo_gsm 3
typedef struct
{
    U32 t;
    union {
        /* t = 1 */
        cellselectreselectinfosib_11_12_ecn0_modespecificinfo_fdd_t fdd;
        /* t = 2 */
        cellselectreselectinfosib_11_12_ecn0_modespecificinfo_tdd_t tdd;
        /* t = 3 */
        cellselectreselectinfosib_11_12_ecn0_modespecificinfo_gsm_t gsm;
    } u;
}cellselectreselectinfosib_11_12_ecn0_modespecificinfo_t;

#define UMTS_SIB_CELLSELECTRESELECTINFOSIB_11_12_ECN0_MAXALLOWEDUL_TX_POWER_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                 presence_bitmask;
    S8 q_Offset1S_N;
    S8 q_Offset2S_N;
    S8 maxAllowedUL_TX_Power;
    cellselectreselectinfosib_11_12_ecn0_modespecificinfo_t mode_specific_info;
}cellselectreselectinfosib_11_12_ecn0_t;

#define UMTS_SIB_INCELLINFOSI_ECN0_LCR_R4_REFERENCETIMEDIFFERENCETOCELL_PRESENCE_FLAG    0x01
#define UMTS_SIB_INCELLINFOSI_ECN0_LCR_R4_PRIMARYCCPCH_TX_POWER_PRESENCE_FLAG            0x02
#define UMTS_SIB_INCELLINFOSI_ECN0_LCR_R4_TIMESLOTINFOLIST_PRESENCE_FLAG                 0x04
#define UMTS_SIB_INCELLINFOSI_ECN0_LCR_R4_CELLSELECTIONRESELECTIONINFO_PRESENCE_FLAG            0x08
typedef struct
{
    U16                                 presence_bitmask;
    S8 cellIndividualOffset;
    referencetimedifferencetocell_t reference_time_difference_to_cell;
    primaryccpch_info_lcr_r4_t primaryccpch_info;
    U8 primaryccpch_tx_power;
    timeslotinfolist_lcr_r4_t timeslot_info_list;
    U8 readsfn_indicator;
    cellselectreselectinfosib_11_12_ecn0_t cell_selection_reselection_info;
} cellinfosi_ecn0_lcr_r4_t;

#define UMTS_SIB_NEWINTRAFREQCELLSI_ECN0_LCR_R4_INTRAFREQCELLID_PRESENCE_FLAG      0x01
typedef struct
{
    U16                                 presence_bitmask;
    U8 intrafreqcell_id;
    cellinfosi_ecn0_lcr_r4_t cell_info;
}newintrafreqcellsi_ecn0_lcr_r4_t;

typedef struct
{
    U8 count;
    newintrafreqcellsi_ecn0_lcr_r4_t newintrafreqcellsi_ecn0_lcr_r4[MAX_CELL_MEAS]; 
}newintrafreqcellsi_list_ecn0_lcr_r4_t;

#define UMTS_SIB_INTRAFREQCELLINFOSI_LIST_ECN0_LCR_R4_REMOVEDINTRAFREQCELLLIST_PRESENCE_FLAG      0x01
typedef struct
{
    U16                                 presence_bitmask;
    removedintrafreqcelllist_t removed_intra_freq_cell_list;
    newintrafreqcellsi_list_ecn0_lcr_r4_t new_intra_freq_cell_list;
} intrafreqcellinfosi_list_ecn0_lcr_r4_t;

#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_ECN0_LCR_R4_INTRAFREQCELLINFOSI_LIST_PRESENCE_FLAG            0x01
#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_ECN0_LCR_R4_INTRAFREQMEASQUANTITY_PRESENCE_FLAG               0x02
#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_ECN0_LCR_R4_INTRAFREQREPORTINGQUANTITYFORRACHPRESEN_FLAG	  0x04
#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_ECN0_LCR_R4_MAXREPORTEDCELLSONRACH_PRESENCE_FLAG              0x08
#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_ECN0_LCR_R4_REPORTINGINFOFORCELLDCH_PRESENCE_FLAG             0x10
typedef struct
{
    U16						presence_bitmask;
    U8			intra_freq_measurement_id;
    intrafreqcellinfosi_list_ecn0_lcr_r4_t	intra_freq_cell_info_si_list;
    intrafreqmeasquantity_t			intra_freq_meas_quantity;
    intrafreqreportingquantityforrach_t		intra_freq_reporting_quantity_for_rach;
    U32			max_reported_cell_sonrach;
    reportinginfoforcelldch_lcr_r4_t		reporting_info_for_celldch;
} intrafreqmeasurementsysinfo_ecn0_lcr_r4_t;

#define UMTS_SIB_NEWINTERFREQCELLSI_ECN0_LCR_R4_INTERFREQCELLID_PRESENCE_FLAG    	0x01
#define UMTS_SIB_NEWINTERFREQCELLSI_ECN0_LCR_R4_INTERFREQCFREQUENCYINFO_PRESENCE_FLAG    0x02
typedef struct
{
    U16						presence_bitmask;
    U8 inter_freq_cell_id;
    frequency_info_t frequency_info;
    cellinfosi_ecn0_lcr_r4_t cell_info;
} newinterfreqcellsi_ecn0_lcr_r4_t;

typedef struct
{
    U8 count;
    newinterfreqcellsi_ecn0_lcr_r4_t newinterfreqcellsi_ecn0_lcr_r4[MAX_CELL_MEAS];
} newinterfreqcellsi_list_ecn0_lcr_r4_t;

#define UMTS_SIB_INTERFREQCELLINFOSI_LIST_ECN0_LCR_REMOVEDiNTERfREQcELLlISTpRESENT_FLAG           0x01
#define UMTS_SIB_INTERFREQCELLINFOSI_LIST_ECN0_LCR_NEWiNTERfREQcELLlISTpRESENT_FLAG               0x02
typedef struct
{
    U16						presence_bitmask;
    removedinterfreqcelllist_t			removed_inter_freq_cell_list;
    newinterfreqcellsi_list_ecn0_lcr_r4_t	new_inter_freq_cell_list;
}interfreqcellinfosi_list_ecn0_lcr_t;


#define UMTS_SIB_INTERFREQMEASUREMENTSYSINFO_ECN0_LCR_R4_INTERFREQCELLINFOSI_LIST_PRESENCE_FLAG       0x01
typedef struct
{
    U16                                 presence_bitmask;
    interfreqcellinfosi_list_ecn0_lcr_t inter_freq_cell_info_si_list;
}interfreqmeasurementsysinfo_ecn0_lcr_r4_t;

#define UMTS_SIB_MEASUREMENTCONTROLSYSiNFO_LCR_R4_EXT_USE_OF_HCS_HCS_NOT_USED_CELLSELECTQUALITYMEASURE_CPICH_EC_NO_INTRAFREQMEASUREMENTSYSINFO_PRESENCE_FLAG         0x01
#define UMTS_SIB_MEASUREMENTCONTROLSYSiNFO_LCR_R4_EXT_USE_OF_HCS_HCS_NOT_USED_CELLSELECTQUALITYMEASURE_CPICH_EC_NO_INTERFREQMEASUREMENTSYSINFO_PRESENCE_FLAG         0x02
typedef struct
{
    U16							presence_bitmask;
    intrafreqmeasurementsysinfo_ecn0_lcr_r4_t		intra_freq_measurement_sys_info;
    interfreqmeasurementsysinfo_ecn0_lcr_r4_t		inter_freq_measurement_sys_info;
}measurementcontrolsysInfo_lcr_r4_ext_use_of_hcs_hcs_not_used_cellselectqualitymeasure_cpich_ec_n0_t;

#define RRC_UMTS_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_not_used_cellSelectQualityMeasure_cpich_RSCP 1
#define RRC_UMTS_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_not_used_cellSelectQualityMeasure_cpich_Ec_N0 2
typedef struct
{
    U32 t;
    union {
        /* t = 1 */
        measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_not_used_cellselectqualitymeasure_cpich_rscp_t	cpich_rscp;
        /* t = 2 */
        measurementcontrolsysInfo_lcr_r4_ext_use_of_hcs_hcs_not_used_cellselectqualitymeasure_cpich_ec_n0_t	cpich_ec_n0;
    } u;
}measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_not_used_cellselectqualitymeasure_t;

typedef struct
{
    measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_not_used_cellselectqualitymeasure_t cell_select_quality_measure;
}measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_not_used_t;

#define RRC_UMTS_PenaltyTime_RSCP_notUsed 1
#define RRC_UMTS_PenaltyTime_RSCP_pt10    2
#define RRC_UMTS_PenaltyTime_RSCP_pt20    3
#define RRC_UMTS_PenaltyTime_RSCP_pt30    4
#define RRC_UMTS_PenaltyTime_RSCP_pt40    5
#define RRC_UMTS_PenaltyTime_RSCP_pt50    6
#define RRC_UMTS_PenaltyTime_RSCP_pt60    7

typedef struct
{
    U32 t;
    union {
        /* t = 1 */
        /* t = 2 */
        U32 pt10;
        /* t = 3 */
        U32 pt20;
        /* t = 4 */
        U32 pt30;
        /* t = 5 */
        U32 pt40;
        /* t = 6 */
        U32 pt50;
        /* t = 7 */
        U32 pt60;
    } u;
}penaltytime_rscp_t;

typedef struct
{
    penaltytime_rscp_t penalty_time;
}hcs_cellreselectinformation_rscp_t;

typedef struct
{
    U8 hcs_prio;
    U8 q_hcs;
    hcs_cellreselectinformation_rscp_t hcs_cellreselectinformation;
}hcs_neighbouringcellinformation_rscp_t;

#define UMTS_SIB_CELLSELECTRESELECTINFOSIB_11_12_HCS_RSCP_MAXALLOWEDUL_TX_POWER_PRESENCE_FLAG                0x01
#define UMTS_SIB_CELLSELECTRESELECTINFOSIB_11_12_HCS_RSCP_HCS_NEIGHBOURINGCELLINFORMATION_RSCP_PRESENCE_FLAG 0x02
typedef struct
{
    U16                    presence_bitmask;
    S8 q_OffsetS_N;
    S8 max_allowed_ul_tx_power;
    hcs_neighbouringcellinformation_rscp_t hcs_neighbouring_cell_information_rscp;
    cell_select_reselect_info_sib_11_12_hcs_rscp_mode_specific_info_t mode_specific_info;
}cellselectreselectinfosib_11_12_hcs_rscp_t;

#define UMTS_SIB_CELLINFOSI_HCS_RSCP_LCR_R4_REFERENCETIMEDIFFERENCETOCELL_PRESENCE_FLAG     0x01
#define UMTS_SIB_CELLINFOSI_HCS_RSCP_LCR_R4_PRIMARYCCPCH_TX_POWER_PRESENCE_FLAG             0x02
#define UMTS_SIB_CELLINFOSI_HCS_RSCP_LCR_R4_TIMESLOTINFOLIST_PRESENCE_FLAG                  0x04
#define UMTS_SIB_CELLINFOSI_HCS_RSCP_LCR_R4_CELLSELECTIONRESELECTIONINFO_PRESENCE_FLAG      0x08

typedef struct
{
    U16                    presence_bitmask;
    S8 cellIndividualOffset;
    referencetimedifferencetocell_t referencetime_differencetocell;
    primaryccpch_info_lcr_r4_t primaryccpch_info;
    U8 primaryccpch_tx_power;
    timeslotinfolist_lcr_r4_t timeslot_info_list;
    U8 readsfn_indicator;
    cellselectreselectinfosib_11_12_hcs_rscp_t cell_selection_reselection_info;
}cellinfosi_hcs_rscp_lcr_r4_t;


#define UMTS_SIB_NEWINTRAFREQCELLSI_HCS_RSCP_LCR_R4_INTRAFREQCELLIDPRESENCE_FLAG       0x01
typedef struct
{
    U16	presence_bitmask;
    U8 intra_freq_cell_id;
    cellinfosi_hcs_rscp_lcr_r4_t cell_info;
} newintrafreqcellsi_hcs_rscp_lcr_r4_t;

typedef struct 
{
    U8 count;
    newintrafreqcellsi_hcs_rscp_lcr_r4_t newintrafreqcellsi_hcs_rscp_lcr_r4[MAX_CELL_MEAS];
}newintrafreqcellsi_list_hcs_rscp_lcr_r4_t;


#define UMTS_SIB_INTRAFREQCELLINFOSI_LIST_HCS_RSCP_LCR_R4_REMOVEDiNTRAfREQCELLlIST_PRESENCE_FLAG  0x01
typedef struct
{
    U16                                 presence_bitmask;
    removedintrafreqcelllist_t removed_intra_freq_cell_list;
    newintrafreqcellsi_list_hcs_rscp_lcr_r4_t new_intra_freq_cell_list;
}intrafreqcellinfosi_list_hcs_rscp_lcr_r4_t;



#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_HCS_RSCP_LCR_R4_INTRAFREQCELLINFOSI_LIST_PRESENCE_FLAG		0x01
#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_HCS_RSCP_LCR_R4_INTRAFREQMEASQUANTITYPRESEN_FLAG			0x02
#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_HCS_RSCP_LCR_R4_INTRAFREQREPORTINGQUANTITYFORRACHPRESEN_FLAG	0x04
#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_HCS_RSCP_LCR_R4_MAXREPORTEDCELLSONRACHPRESEN_FLAG			0x08
#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_HCS_RSCP_LCR_R4_REPORTINGINFOFORCELLDCHPRESEN_FLAG			0x10
typedef struct
{
    U16						presence_bitmask;
    U8			intra_freq_measurement_id;
    intrafreqcellinfosi_list_hcs_rscp_lcr_r4_t	intra_freq_cell_info_si_list;
    intrafreqmeasquantity_t			intra_freq_meas_quantity;
    intrafreqreportingquantityforrach_t		intra_freq_reporting_quantity_forrach;
    U32			max_reported_cells_onrach;
    reportinginfoforcelldch_lcr_r4_t		reporting_info_forellDCH;
}intrafreqmeasurementsysinfo_hcs_rscp_lcr_r4_t;

#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_HCS_ECN0_LCR_R4_INTRAFREQCELLINFOSI_LIST_PRESENCE_FLAG		0x01
#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_HCS_ECN0_LCR_R4_INTRAFREQMEASQUANTITY_PRESENCE_FLAG			0x02
#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_HCS_ECN0_LCR_R4_INTRAFREQREPORTINGQUANTITYFORRACH_PRESENCE_FLAG	0x04
#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_HCS_ECN0_LCR_R4_MAXREPORTEDCELLSONRACH_PRESENCE_FLAG			0x08
#define UMTS_SIB_INTRAFREQMEASUREMENTSYSINFO_HCS_ECN0_LCR_R4_REPORTINGINFOFORCELLDCH_PRESENCE_FLAG			0x10
typedef struct
{
    U16						presence_bitmask;
    U8			intra_freq_measurement_id;
    intrafreqcellinfosi_list_hcs_ecn0_lcr_r4_t	intra_freq_cell_info_si_list;
    intrafreqmeasquantity_t			intra_freq_meas_quantity;
    intrafreqreportingquantityforrach_t		intra_freq_reporting_quantity_for_rach;
    U32			max_reported_cells_on_rach;
    reportinginfoforcelldch_lcr_r4_t		reporting_info_for_celldch;
}intrafreqmeasurementsysinfo_hcs_ecn0_lcr_r4_t;

#define  UMTS_SIB_MEASUREMENTCONTROLSYSINFO_LCR_R4_EXT_USE_OF_HCS_HCS_USED_CELLSELECTQUALITYMEASURE_CPICH_EC_NO_INTRAFREQMEASUREMENTSYSINFO_PRESENCE_FLAG 0x01
#define  UMTS_SIB_MEASUREMENTCONTROLSYSINFO_LCR_R4_EXT_USE_OF_HCS_HCS_USED_CELLSELECTQUALITYMEASURE_CPICH_EC_NO_INTERFREQMEASUREMENTSYSINFO_PRESENCE_FLAG  0x02
typedef struct
{
    U16                                 presence_bitmask;
    intrafreqmeasurementsysinfo_hcs_ecn0_lcr_r4_t intra_freq_measurement_sys_info;
    interfreqmeasurementsysinfo_hcs_ecn0_lcr_r4_t inter_freq_measurement_sys_info;
}measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_used_cellselectqualitymeasure_cpich_ec_no_t;

#define UMTS_SIB_NEWINTERFREQCELLSI_HCS_RSCP_LCR_R4_INTERFREQCELLID_PRESENCE_FLAG         0x01
#define UMTS_SIB_NEWINTERFREQCELLSI_HCS_RSCP_LCR_R4_FREQUENCYINFO_PRESENCE_FLAG           0x02
typedef struct
{
    U16                    presence_bitmask;
    U8 inter_freq_cell_id;
    frequency_info_t frequencyInfo;
    cellinfosi_hcs_rscp_lcr_r4_t cellInfo;
}newinterfreqcellsi_hcs_rscp_lcr_r4_t;

typedef struct
{
    U8 count;
    newinterfreqcellsi_hcs_rscp_lcr_r4_t newinterfreqcellsi_hcs_rscp_lcr_r4[MAX_CELL_MEAS];
} newinterfreqcellsi_list_hcs_rscp_lcr_r4_t;

#define UMTS_SIB_INTERFREQCELLINFOSI_LIST_HCS_RSCP_LCR_REMOVEDINTERFREQCELLLIST_PRESENCE_FLAG     0x01
#define UMTS_SIB_INTERFREQCELLINFOSI_LIST_HCS_RSCP_LCR_NEWINTERFREQCELLLIST_PRESENCE_FLAG         0x02
typedef struct
{
    U16                                 presence_bitmask;
    removedinterfreqcelllist_t removed_inter_freq_cell_list;
    newinterfreqcellsi_list_hcs_rscp_lcr_r4_t new_inter_freq_cell_list;
}interfreqcellinfosi_list_hcs_rscp_lcr_t;


#define UMTS_SIB_INTERFREQMEASUREMENTSYSINFO_HCS_RSCP_LCR_R4_INTERFREQCELLINFOSI_LIST_PRESENCE_FLAG          0x01
typedef struct
{
    U16						presence_bitmask;
    interfreqcellinfosi_list_hcs_rscp_lcr_t inter_freq_cell_info_si_list;
}interfreqmeasurementsysinfo_hcs_rscp_lcr_r4_t;


#define UMTS_SIB_MEASUREMENTCONTROLSYSINFO_LCR_R4_EXT_USE_OF_HCS_HCS_USED_CELLSELECTQUALITYMEASURE_CPICH_RSCP_INTRAFREQMEASUREMENTSYSINFO_PRESENCE_FLAG          0x01
#define UMTS_SIB_MEASUREMENTCONTROLSYSINFO_LCR_R4_EXT_USE_OF_HCS_HCS_USED_CELLSELECTQUALITYMEASURE_CPICH_RSCP_INTERFREQMEASUREMENTSYSiNFO_PRESENCE_FLAG          0x02
typedef struct
{
    U16							presence_bitmask;
    intrafreqmeasurementsysinfo_hcs_rscp_lcr_r4_t	intra_freq_measurement_sys_info;
    interfreqmeasurementsysinfo_hcs_rscp_lcr_r4_t	inter_freq_measurement_sys_info;
}measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_used_cellselectqualitymeasure_cpich_rscp_t;


#define RRC_UMTS_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_used_cellSelectQualityMeasure_cpich_RSCP 1
#define RRC_UMTS_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_used_cellSelectQualityMeasure_cpich_Ec_N0 2
typedef struct{
    U32 t;
    union {
        /* t = 1 */
        measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_used_cellselectqualitymeasure_cpich_rscp_t cpich_rscp;
        /* t = 2 */
        measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_used_cellselectqualitymeasure_cpich_ec_no_t cpich_ec_n0;
    } u;
}measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_used_cellselectqualitymeasure_t;


typedef struct
{
    measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_used_cellselectqualitymeasure_t cell_select_quality_measure;
}measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_used_t;


#define RRC_UMTS_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_not_used 1
#define RRC_UMTS_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_used 2
typedef struct
{
    U32 t;
    union {
        /* t = 1 */
        measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_not_used_t	hcs_not_used;
        /* t = 2 */
        measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_used_t	hcs_used;
    } u;
}measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_t;


typedef struct
{
    measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_t	use_of_hcs;
}measurementcontrolsysinfo_lcr_r4_ext_t;


#define UMTS_SIB_SYSINFOTYPE11_V4B0EXT_IES_FACH_MEASUREMENTOCCASIONINFO_LCR_EXT_PRESENCE_FLAG     0x01
typedef struct
{
    U16                                 presence_bitmask;
    fach_measurementoccasioninfo_lcr_r4_ext_t	fach_measurement_occasioninfo_lcr_ext;
    measurementcontrolsysinfo_lcr_r4_ext_t	measurement_control_sysinfo_lcr;
}sysinfotype11_v4b0ext_ies_t;


#define UMTS_SIB_SYSINFOTYPE11_V4B0_NC_EXTENSIONS_SYSINFOTYPE11_V4B0EXT_PRESENCE_FLAG          0x01
#define UMTS_SIB_SYSINFOTYPE11_V4B0_NC_EXTENSIONS_V590NONCRITICALEXTENSION_PRESENCE_FLAG       0x02
typedef struct 
{
    U16					presence_bitmask;
    sysinfotype11_v4b0ext_ies_t		sys_info_type11_v4b0ext;
    sysinfotype11_v4b0_nc_extensions_v590_nc_extension_t v590_nc_extension;
} sysinfotype11_v4b0_nc_extensions_t;

/********************************************************************/
/*                                                                  */
/*  umts_sib_type_11_info_t - System Information Block type 11      */
/*                                                                  */
/********************************************************************/
#define UMTS_SIB_TYPE_11_FACH_MEAS_OCCASION_INFO_PRESENCE_FLAG  0x01
#define UMTS_SIB_TYPE_11_V4B0_NC_EXTNS_PRESENCE_FLAG            0x02
typedef struct
{
    U16                                 presence_bitmask;
    U8                                  sib12indicator;
    fach_measurement_occasion_info_t    fach_measurement_occasion_info;
    measurement_control_sys_info_t      measurement_control_sys_info;
    sysinfotype11_v4b0_nc_extensions_t  sysinfotype11_v4b0_nc_extensions;
}umts_sib_type_11_info_t;

#define UMTS_SIB_MEAS_CTRL_SYSINFO_EXTN_USE_OF_HCS_HCS_NOT_USED_CELL_SELECT_QUALITY_MEAS_CPICH_RSCP_NEW_INTRA_FREQ_CELL_SI_LIST_RSCP_PRESENCE_FLAG   0x01
#define UMTS_SIB_MEAS_CTRL_SYSINFO_EXTN_USE_OF_HCS_HCS_NOT_USED_CELL_SELECT_QUALITY_MEAS_CPICH_RSCP_NEW_INTER_FREQ_CELL_SI_LIST_RSCP_PRESENCE_FLAG   0x02
typedef struct
{
    U16                                 presence_bitmask;
    new_intra_freq_cell_si_list_rscp_t  new_intra_freq_cell_si_list_rscp;
    new_inter_freq_cell_si_list_rscp_t  new_inter_freq_cell_si_list_rscp;
}measurement_control_sys_info_extension_use_of_hcs_hcs_not_used_cell_select_quality_measure_cpich_rscp_t;

#define UMTS_SIB_MEAS_CTRL_SYSINFO_EXTN_USE_OF_HCS_HCS_NOT_USED_CELL_SELECT_QUALITY_MEAS_CPICH_ECNO_NEW_INTRA_FREQ_CELL_SI_LIST_ECNO_PRESENCE_FLAG 0x01
#define UMTS_SIB_MEAS_CTRL_SYSINFO_EXTN_USE_OF_HCS_HCS_NOT_USED_CELL_SELECT_QUALITY_MEAS_CPICH_ECNO_NEW_INTER_FREQ_CELL_SI_LIST_ECNO_PRESENCE_FLAG 0x02
typedef struct
{
    U16                                 presence_bitmask;
    new_intra_freq_cell_si_list_ecno_t  new_intra_freq_cell_si_list_ecno;
    new_inter_freq_cell_si_list_ecno_t  new_inter_freq_cell_si_list_ecno;
}measurement_control_sys_info_extension_use_of_hcs_hcs_not_used_cell_select_quality_measure_cpich_ec_no_t;

#define RRC_UMTS_MEASUREMENTCONTROLSYSINFOEXTENSION_USE_OF_HCS_HCS_NOT_USED_CELLSELECTQUALITYMEASURE_CPICH_RSCP 1
#define RRC_UMTS_MEASUREMENTCONTROLSYSINFOEXTENSION_USE_OF_HCS_HCS_NOT_USED_CELLSELECTQUALITYMEASURE_CPICH_EC_N0 2
typedef struct
{
    U32                 t;
    union
    {
        measurement_control_sys_info_extension_use_of_hcs_hcs_not_used_cell_select_quality_measure_cpich_rscp_t   cpich_rscp;
        measurement_control_sys_info_extension_use_of_hcs_hcs_not_used_cell_select_quality_measure_cpich_ec_no_t  cpich_ec_no;
    }u;
}measurement_control_sys_info_extension_use_of_hcs_hcs_not_used_cell_select_quality_measure_t;

#define UMTS_SIB_MEAS_CTRL_SYSINFO_EXTN_USE_OF_HCS_HCS_NOT_USED_NEW_INTER_RAT_CELL_LIST_PRESENCE_FLAG   0x01
typedef struct
{
    U16                         presence_bitmask;
    measurement_control_sys_info_extension_use_of_hcs_hcs_not_used_cell_select_quality_measure_t  cell_select_quality_measure;
    new_inter_rat_cell_list_t   new_inter_rat_cell_list;
}measurement_control_sys_info_extension_use_of_hcs_hcs_not_used_t;

#define UMTS_SIB_MEAS_CTRL_SYSINFO_EXTN_USE_OF_HCS_HCS_USED_CELL_SELECT_QUALITY_MEAS_CPICH_RSCP_NEW_INTRA_FREQ_CELL_SI_LIST_HCS_RSCP_PRESENCE_FLAG   0x01
#define UMTS_SIB_MEAS_CTRL_SYSINFO_EXTN_USE_OF_HCS_HCS_USED_CELL_SELECT_QUALITY_MEAS_CPICH_RSCP_NEW_INTER_FREQ_CELL_SI_LIST_HCS_RSCP_PRESENCE_FLAG   0x02
typedef struct
{
    U16                                     presence_bitmask;
    new_intra_freq_cell_si_list_hcs_rscp_t  new_intra_freq_cell_si_list_hcs_rscp;
    new_inter_freq_cell_si_list_hcs_rscp_t  new_inter_freq_cell_si_list_hcs_rscp;
}measurement_control_sys_info_extension_use_of_hcs_hcs_used_cell_select_quality_measure_cpich_rscp_t;

#define UMTS_SIB_MEAS_CTRL_SYSINFO_EXTN_USE_OF_HCS_HCS_USED_CELL_SELECT_QUALITY_MEAS_CPICH_RSCP_NEW_INTRA_FREQ_CELL_SI_LIST_HCS_ECNO_PRESENCE_FLAG   0x01
#define UMTS_SIB_MEAS_CTRL_SYSINFO_EXTN_USE_OF_HCS_HCS_USED_CELL_SELECT_QUALITY_MEAS_CPICH_RSCP_NEW_INTER_FREQ_CELL_SI_LIST_HCS_ECNO_PRESENCE_FLAG   0x02
typedef struct
{
    U16                                     presence_bitmask;
    new_intra_freq_cell_si_list_hcs_ecno_t  new_intra_freq_cell_si_list_hcs_ecno;
    new_inter_freq_cell_si_list_hcs_ecno_t  new_inter_freq_cell_si_list_hcs_ecno;
}measurement_control_sys_info_extension_use_of_hcs_hcs_used_cell_select_quality_measure_cpich_ec_no_t;

#define RRC_UMTS_MEASUREMENTCONTROLSYSINFOEXTENSION_USE_OF_HCS_HCS_USED_CELLSELECTQUALITYMEASURE_CPICH_RSCP 1
#define RRC_UMTS_MEASUREMENTCONTROLSYSINFOEXTENSION_USE_OF_HCS_HCS_USED_CELLSELECTQUALITYMEASURE_CPICH_EC_N0 2
typedef struct
{
    U32                     t;
    union
    {
        measurement_control_sys_info_extension_use_of_hcs_hcs_used_cell_select_quality_measure_cpich_rscp_t  cpich_rscp;
        measurement_control_sys_info_extension_use_of_hcs_hcs_used_cell_select_quality_measure_cpich_ec_no_t cpich_ec_no;
    }u;
}measurement_control_sys_info_extension_use_of_hcs_hcs_used_cell_select_quality_measure_t;

#define UMTS_SIB_MEAS_CTRL_SYSINFO_EXTN_USE_OF_HCS_HCS_USED_NEW_INTER_RAT_CELL_LIST_PRESENCE_FLAG   0x01
typedef struct
{
    U16                                 presence_bitmask;
    measurement_control_sys_info_extension_use_of_hcs_hcs_used_cell_select_quality_measure_t  cell_select_quality_measure;
    new_inter_rat_cell_list_t           new_inter_rat_cell_list;
}measurement_control_sys_info_extension_use_of_hcs_hcs_used_t;

#define RRC_UMTS_MEASUREMENTCONTROLSYSINFOEXTENSION_USE_OF_HCS_HCS_NOT_USED 1
#define RRC_UMTS_MEASUREMENTCONTROLSYSINFOEXTENSION_USE_OF_HCS_HCS_USED 2
typedef struct
{
    U32         t;
    union
    {
        measurement_control_sys_info_extension_use_of_hcs_hcs_not_used_t  hcs_not_used;
        measurement_control_sys_info_extension_use_of_hcs_hcs_used_t      hcs_used;
    }u;
}measurement_control_sys_info_extension_use_of_hcs_t;

typedef struct
{
    measurement_control_sys_info_extension_use_of_hcs_t use_of_hcs;
}measurement_control_sys_info_extension_t;

#define UMTS_SIB_CELL_INFO_SI_RSCP_LCR_R4_REF_TIME_DIFF_TO_CELL_PRESENCE_FLAG       0x01
#define UMTS_SIB_CELL_INFO_SI_RSCP_LCR_R4_PRIMARY_CCPCH_TX_PWR_PRESENCE_FLAG        0x02
#define UMTS_SIB_CELL_INFO_SI_RSCP_LCR_R4_TIMESLOT_INFO_LIST_PRESENCE_FLAG          0x04
#define UMTS_SIB_CELL_INFO_SI_RSCP_LCR_R4_CELL_SELECT_RESELECT_INFO_PRESENCE_FLAG   0x08
typedef struct
{
    U16                                 presence_bitmask;
    S8                                  cell_individual_offset;
    reference_time_difference_to_cell_t reference_time_difference_to_cell;
    primary_ccpch_info_lcr_r4_t         primary_ccpch_info_lcr_r4;
    U8                                  primary_ccpch_tx_power;
    timeslot_info_list_lcr_r4_t         timeslot_info_list_lcr_r4;
    U8                                  read_sfn_indicator;
    cell_select_reselect_info_sib_11_12_rscp_t  cell_select_reselect_info_sib_11_12_rscp;
}cell_info_si_rscp_lcr_r4_t;

#define UMTS_SIB_NEW_INTRA_FREQ_CELL_SI_RSCP_LCR_R4_INTRA_FREQ_CELL_ID_PRESENCE_FLAG    0x01
typedef struct
{
    U16                         presence_bitmask;
    U8                          intra_freq_cell_id;
    cell_info_si_rscp_lcr_r4_t  cell_info_si_rscp_lcr_r4;
}new_intra_freq_cell_si_rscp_lcr_r4_t;

typedef struct
{
    U8                                      count;
    new_intra_freq_cell_si_rscp_lcr_r4_t    new_intra_freq_cell_si_rscp_lcr_r4[MAX_CELL_MEAS];
}new_intra_freq_cell_si_list_rscp_lcr_r4_t;

#define UMTS_SIB_NEW_INTER_FREQ_CELL_SI_RSCP_LCR_R4_INTER_FREQ_CELL_ID_PRESENCE_FLAG    0x01
#define UMTS_SIB_NEW_INTER_FREQ_CELL_SI_RSCP_LCR_R4_FREQUENCY_INFO_PRESENCE_FLAG        0x02
typedef struct
{
    U16                         presence_bitmask;
    U8                          inter_freq_cell_id;
    frequency_info_t            frequency_info;
    cell_info_si_rscp_lcr_r4_t  cell_info_si_rscp_lcr_r4;
}new_inter_freq_cell_si_rscp_lcr_r4_t;

typedef struct
{
    U8                                      count;
    new_inter_freq_cell_si_rscp_lcr_r4_t    new_inter_freq_cell_si_rscp_lcr_r4[MAX_CELL_MEAS];
}new_inter_freq_cell_si_list_rscp_lcr_r4_t;

#define UMTS_SIB_NEW_INTRA_FREQ_CELL_SI_LIST_RSCP_LCR_R4_PRESENCE_FLAG  0x01
#define UMTS_SIB_NEW_INTER_FREQ_CELL_SI_LIST_RSCP_LCR_R4_PRESENCE_FLAG  0x02
typedef struct
{
    U16                                         presence_bitmask;
    new_intra_freq_cell_si_list_rscp_lcr_r4_t   new_intra_freq_cell_si_list_rscp_lcr_r4;
    new_inter_freq_cell_si_list_rscp_lcr_r4_t   new_inter_freq_cell_si_list_rscp_lcr_r4;
}measurement_control_sys_info_extension_lcr_r4_use_of_hcs_hcs_not_used_cell_select_quality_measure_cpich_rscp_t;

#define UMTS_SIB_CELL_INFO_SI_ECNO_LCR_R4_REF_TIME_DIFF_TO_CELL_PRESENCE_FLAG       0x01
#define UMTS_SIB_CELL_INFO_SI_ECNO_LCR_R4_PRIMARY_CCPCH_TX_PWR_PRESENCE_FLAG        0x02
#define UMTS_SIB_CELL_INFO_SI_ECNO_LCR_R4_TIMESLOT_INFO_LIST_PRESENCE_FLAG          0x04
#define UMTS_SIB_CELL_INFO_SI_ECNO_LCR_R4_CELL_SELECT_RESELECT_INFO_PRESENCE_FLAG   0x08
typedef struct
{
    U16                                 presence_bitmask;
    S8                                  cell_individual_offset;
    reference_time_difference_to_cell_t reference_time_difference_to_cell;
    primary_ccpch_info_lcr_r4_t         primary_ccpch_info_lcr_r4;
    U8                                  primary_ccpch_tx_power;
    timeslot_info_list_lcr_r4_t         timeslot_info_list_lcr_r4;
    U8                                  read_sfn_indicator;
    cell_select_reselect_info_sib_11_12_ecno_t   cell_select_reselect_info_sib_11_12_ecno;
}cell_info_si_ecno_lcr_r4_t;

#define UMTS_SIB_NEW_INTRA_FREQ_CELL_SI_ECNO_LCR_R4_INTRA_FREQ_CELL_ID_PRESENCE_FLAG    0x01
typedef struct
{
    U16                         presence_bitmask;
    U8                          intra_freq_cell_id;
    cell_info_si_ecno_lcr_r4_t  cell_info_si_ecno_lcr_r4;
}new_intra_freq_cell_si_ecno_lcr_r4_t;

typedef struct
{
    U8                                      count;
    new_intra_freq_cell_si_ecno_lcr_r4_t    new_intra_freq_cell_si_ecno_lcr_r4[MAX_CELL_MEAS];
}new_intra_freq_cell_si_list_ecno_lcr_r4_t;

#define UMTS_SIB_NEW_INTER_FREQ_CELL_SI_ECNO_LCR_R4_INTER_FREQ_CELL_ID_PRESENCE_FLAG    0x01
#define UMTS_SIB_NEW_INTER_FREQ_CELL_SI_ECNO_LCR_R4_FREQ_INFO_PRESENCE_FLAG             0x02
typedef struct
{
    U16                         presence_bitmask;
    U8                          inter_freq_cell_id;
    frequency_info_t            frequency_info;
    cell_info_si_ecno_lcr_r4_t  cell_info_si_ecno_lcr_r4;
}new_inter_freq_cell_si_ecno_lcr_r4_t;

typedef struct
{
    U8                                      count;
    new_inter_freq_cell_si_ecno_lcr_r4_t    new_inter_freq_cell_si_ecno_lcr_r4[MAX_CELL_MEAS];
}new_inter_freq_cell_si_list_ecno_lcr_r4_t;

#define UMTS_SIB_MEAS_CTRL_SYSINFO_EXTN_LCR_R4_NEW_INTRA_FREQ_CELL_SI_LIST_ECNO_LCR_R4_PRESENCE_FLAG    0x01
#define UMTS_SIB_MEAS_CTRL_SYSINFO_EXTN_LCR_R4_NEW_INTER_FREQ_CELL_SI_LIST_ECNO_LCR_R4_PRESENCE_FLAG    0x02
typedef struct
{
    U16                                         presence_bitmask;
    new_intra_freq_cell_si_list_ecno_lcr_r4_t   new_intra_freq_cell_si_list_ecno_lcr_r4;
    new_inter_freq_cell_si_list_ecno_lcr_r4_t   new_inter_freq_cell_si_list_ecno_lcr_r4;
}measurement_control_sys_info_extension_lcr_r4_use_of_hcs_hcs_not_used_cell_select_quality_measure_cpich_ec_no_t;

#define RRC_UMTS_MEASUREMENTCONTROLSYSINFOEXTENSION_LCR_R4_USE_OF_HCS_HCS_NOT_USED_CELLSELECTQUALITYMEASURE_CPICH_RSCP 1
#define RRC_UMTS_MEASUREMENTCONTROLSYSINFOEXTENSION_LCR_R4_USE_OF_HCS_HCS_NOT_USED_CELLSELECTQUALITYMEASURE_CPICH_EC_N0 2
typedef struct
{
    U32                 t;
    union
    {
        measurement_control_sys_info_extension_lcr_r4_use_of_hcs_hcs_not_used_cell_select_quality_measure_cpich_rscp_t   cpich_rscp;
        measurement_control_sys_info_extension_lcr_r4_use_of_hcs_hcs_not_used_cell_select_quality_measure_cpich_ec_no_t  cpich_ec_no;
    }u;
}measurement_control_sys_info_extension_lcr_r4_use_of_hcs_hcs_not_used_cell_select_quality_measure_t;

#define UMTS_SIB_MEAS_CTRL_SYSINFO_EXTN_LCR_R4_USE_OF_HCS_HCS_NOT_USED_NEW_INTER_RAT_CELL_LIST_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                 presence_bitmask;
    measurement_control_sys_info_extension_lcr_r4_use_of_hcs_hcs_not_used_cell_select_quality_measure_t  cell_select_quality_measure;
    new_inter_rat_cell_list_t           new_inter_rat_cell_list;
}measurement_control_sys_info_extension_lcr_r4_use_of_hcs_hcs_not_used_t;

#define UMTS_SIB_CELL_INFO_SI_HCS_RSCP_LCR_R4_REF_TIME_DIFF_TO_CELL_PRESENCE_FLAG       0x01
#define UMTS_SIB_CELL_INFO_SI_HCS_RSCP_LCR_R4_PRIMARY_CCPCH_TX_PWR_PRESENCE_FLAG        0x02
#define UMTS_SIB_CELL_INFO_SI_HCS_RSCP_LCR_R4_TIMESLOT_INFO_LIST_PRESENCE_FLAG          0x04
#define UMTS_SIB_CELL_INFO_SI_HCS_RSCP_LCR_R4_CELL_SELECT_RESELECT_INFO_PRESENCE_FLAG   0x08
typedef struct
{
    U16                                 presence_bitmask;
    S8                                  cell_individual_offset;
    reference_time_difference_to_cell_t reference_time_difference_to_cell;
    primary_ccpch_info_lcr_r4_t         primary_ccpch_info_lcr_r4;
    U8                                  primary_ccpch_tx_power;
    timeslot_info_list_lcr_r4_t         timeslot_info_list_lcr_r4;
    U8                                  read_sfn_indicator;
    cell_select_reselect_info_sib_11_12_hcs_rscp_t  cell_select_reselect_info_sib_11_12_hcs_rscp;
}cell_info_si_hcs_rscp_lcr_r4_t;

#define UMTS_SIB_INTRA_FREQ_CELL_SI_HCS_RSCP_LCR_R4_INTRA_FREQ_CELL_ID_PRESENCE_FLAG    0x01
typedef struct
{
    U16                             presence_bitmask;
    U8                              intra_freq_cell_id;
    cell_info_si_hcs_rscp_lcr_r4_t  cell_info_si_hcs_rscp_lcr_r4;
}new_intra_freq_cell_si_hcs_rscp_lcr_r4_t;

typedef struct
{
    U8                                          count;
    new_intra_freq_cell_si_hcs_rscp_lcr_r4_t    new_intra_freq_cell_si_list_hcs_rscp_lcr_r4[MAX_CELL_MEAS];
}new_intra_freq_cell_si_list_hcs_rscp_lcr_r4_t;

#define UMTS_SIB_NEW_INTER_FREQ_CELL_SI_HCS_RSCP_LCR_R4_INTER_FREQ_CELL_ID_PRESENCE_FLAG    0x01
#define UMTS_SIB_NEW_INTER_FREQ_CELL_SI_HCS_RSCP_LCR_R4_FREQ_INFO_PRESENCE_FLAG             0x02
typedef struct
{
    U16                             presence_bitmask;
    U8                              inter_freq_cell_id;
    frequency_info_t                frequency_info;
    cell_info_si_hcs_rscp_lcr_r4_t  cell_info_si_hcs_rscp_lcr_r4;
}new_inter_freq_cell_si_hcs_rscp_lcr_r4_t;

typedef struct
{
    U8                                          count;
    new_inter_freq_cell_si_hcs_rscp_lcr_r4_t    new_inter_freq_cell_si_hcs_rscp_lcr_r4[MAX_CELL_MEAS];
}new_inter_freq_cell_si_list_hcs_rscp_lcr_r4_t;

#define UMTS_SIB_HCS_USED_CELL_SELECT_QUALITY_MEAS_CPICH_RSCP_NEW_INTRA_FREQ_CELL_SI_LIST_HCS_RSCP_LCR_R4_PRESENCE_FLAG  0x01
#define UMTS_SIB_HCS_USED_CELL_SELECT_QUALITY_MEAS_CPICH_RSCP_NEW_INTER_FREQ_CELL_SI_LIST_HCS_RSCP_LCR_R4_PRESENCE_FLAG  0x02
typedef struct
{
    U16                                             presence_bitmask;
    new_intra_freq_cell_si_list_hcs_rscp_lcr_r4_t   new_intra_freq_cell_si_list_hcs_rscp_lcr_r4;
    new_inter_freq_cell_si_list_hcs_rscp_lcr_r4_t   new_inter_freq_cell_si_list_hcs_rscp_lcr_r4;
}measurement_control_sys_info_extension_lcr_r4_use_of_hcs_hcs_used_cell_select_quality_measure_cpich_rscp_t;


#define UMTS_SIB_NEW_INTRA_FREQ_CELL_SI_HCS_ECNO_LCR_R4_CELL_INFO_SI_HCS_ECNO_LCR_R4_PRESENCE_FLAG  0x01
typedef struct
{
    U16                             presence_bitmask;
    U8                              intra_freq_cell_id;
    cell_info_si_hcs_ecno_lcr_r4_t  cell_info_si_hcs_ecno_lcr_r4;
}new_intra_freq_cell_si_hcs_ecno_lcr_r4_t;

typedef struct
{
    U8                                          count;
    new_intra_freq_cell_si_hcs_ecno_lcr_r4_t    new_intra_freq_cell_si_hcs_ecno_lcr_r4[MAX_CELL_MEAS];
}new_intra_freq_cell_si_list_hcs_ecno_lcr_r4_t;

#define UMTS_SIB_NEW_INTER_FREQ_CELL_SI_HCS_ECNO_LCR_R4_INTER_FREQ_CELL_ID_PRESENCE_FLAG            0x01
#define UMTS_SIB_NEW_INTER_FREQ_CELL_SI_HCS_ECNO_LCR_R4_CELL_INFO_SI_HCS_ECNO_LCR_R4_PRESENCE_FLAG  0x02
typedef struct
{
    U16                             presence_bitmask;
    U8                              inter_freq_cell_id;
    frequency_info_t                frequency_info;
    cell_info_si_hcs_ecno_lcr_r4_t  cell_info_si_hcs_ecno_lcr_r4;
}new_inter_freq_cell_si_hcs_ecno_lcr_r4_t;

typedef struct
{
    U8                                          count;
    new_inter_freq_cell_si_hcs_ecno_lcr_r4_t    new_inter_freq_cell_si_hcs_ecno_lcr_r4[MAX_CELL_MEAS];
}new_inter_freq_cell_si_list_hcs_ecno_lcr_r4_t;

#define UMTS_SIB_HCS_USED_CELL_SELECT_QUALITY_MEAS_CPICH_ECNO_NEW_INTRA_FREQ_CELL_SI_LIST_HCS_ECNO_LCR_R4_PRESENCE_FLAG  0x01
#define UMTS_SIB_HCS_USED_CELL_SELECT_QUALITY_MEAS_CPICH_ECNO_NEW_INTER_FREQ_CELL_SI_LIST_HCS_ECNO_LCR_R4_PRESENCE_FLAG  0x02
typedef struct
{
    U16                                             presence_bitmask;
    new_intra_freq_cell_si_list_hcs_ecno_lcr_r4_t   new_intra_freq_cell_si_list_hcs_ecno_lcr_r4;
    new_inter_freq_cell_si_list_hcs_ecno_lcr_r4_t   new_inter_freq_cell_si_list_hcs_ecno_lcr_r4;
}measurement_control_sys_info_extension_lcr_r4_use_of_hcs_hcs_used_cell_select_quality_measure_cpich_ecno_t;

#define RRC_UMTS_MEASUREMENTCONTROLSYSINFOEXTENSION_LCR_R4_USE_OF_HCS_HCS_USED_CELLSELECTQUALITYMEASURE_CPICH_RSCP 1
#define RRC_UMTS_MEASUREMENTCONTROLSYSINFOEXTENSION_LCR_R4_USE_OF_HCS_HCS_USED_CELLSELECTQUALITYMEASURE_CPICH_EC_N0 2
typedef struct
{
    U32                 t;
    union
    {
        measurement_control_sys_info_extension_lcr_r4_use_of_hcs_hcs_used_cell_select_quality_measure_cpich_rscp_t  cpich_rscp;
        measurement_control_sys_info_extension_lcr_r4_use_of_hcs_hcs_used_cell_select_quality_measure_cpich_ecno_t  cpich_ecno; 
    }u;
}measurement_control_sys_info_extension_lcr_r4_use_of_hcs_hcs_used_cell_select_quality_measure_t;

#define UMTS_SIB_MEAS_CTRL_SYSINFO_EXTN_LCR_R4_USE_OF_HCS_HCS_USED_NEW_INTER_RAT_CELL_LIST_PRESENCE_FLAG    0x01
typedef struct
{
    U16                         presence_bitmask;
    measurement_control_sys_info_extension_lcr_r4_use_of_hcs_hcs_used_cell_select_quality_measure_t cell_select_quality_measure;
    new_inter_rat_cell_list_t   new_inter_rat_cell_list;
}measurement_control_sys_info_extension_lcr_r4_use_of_hcs_hcs_used_t;

#define RRC_UMTS_MEASUREMENTCONTROLSYSINFOEXTENSION_LCR_R4_USE_OF_HCS_HCS_NOT_USED 1
#define RRC_UMTS_MEASUREMENTCONTROLSYSINFOEXTENSION_LCR_R4_USE_OF_HCS_HCS_USED 2
typedef struct
{
    U32         t;
    union
    {
        measurement_control_sys_info_extension_lcr_r4_use_of_hcs_hcs_not_used_t  hcs_not_used;
        measurement_control_sys_info_extension_lcr_r4_use_of_hcs_hcs_used_t      hcs_used;
    }u;
}measurement_control_sys_info_extension_lcr_r4_use_of_hcs_t;

typedef struct
{
    measurement_control_sys_info_extension_lcr_r4_use_of_hcs_t  use_of_hcs;
}measurement_control_sys_info_extension_lcr_r4_t;

typedef struct
{
    U8                                      count;
    cell_select_reselect_info_v590_ext_t    cell_select_reselect_info_v590_ext[MAX_CELL_MEAS];
}measurement_control_sys_info_extension_addon_r5_new_intra_frequency_cell_info_list_addon_r5_t;

typedef struct
{
    U8                                      count;
    cell_select_reselect_info_v590_ext_t    cell_select_reselect_info_v590_ext[MAX_CELL_MEAS];
}measurement_control_sys_info_extension_addon_r5_new_inter_frequency_cell_info_list_addon_r5_t;

typedef struct
{
    U8                                      count;
    cell_select_reselect_info_v590_ext_t    cell_select_reselect_info_v590_ext[MAX_CELL_MEAS];
}measurement_control_sys_info_extension_addon_r5_new_inter_rat_cell_info_list_addon_r5_t;

#define UMTS_SIB_MEAS_CTRL_SYSINFO_EXTN_ADDON_R5_NEW_INTRA_FREQ_CELL_INFO_LIST_ADDON_R5_PRESENCE_FLAG   0x01
#define UMTS_SIB_MEAS_CTRL_SYSINFO_EXTN_ADDON_R5_NEW_INTER_FREQ_CELL_INFO_LIST_ADDON_R5_PRESENCE_FLAG   0x02
#define UMTS_SIB_MEAS_CTRL_SYSINFO_EXTN_ADDON_R5_NEW_INTER_RAT_CELL_INFO_LIST_ADDON_R5_PRESENCE_FLAG    0x04
typedef struct
{
    U16                     presence_bitmask;
    measurement_control_sys_info_extension_addon_r5_new_intra_frequency_cell_info_list_addon_r5_t   new_intra_frequency_cell_info_list_addon_r5;
    measurement_control_sys_info_extension_addon_r5_new_inter_frequency_cell_info_list_addon_r5_t   new_inter_frequency_cell_info_list_addon_r5;
    measurement_control_sys_info_extension_addon_r5_new_inter_rat_cell_info_list_addon_r5_t         new_inter_rat_cell_info_list_addon_r5;
}measurement_control_sys_info_extension_addon_r5_t;

typedef struct
{
    U8          intra_secondary_freq_indicator;
}new_inter_freq_cell_v7b0ext_t;

typedef struct
{
    U8                              count;
    new_inter_freq_cell_v7b0ext_t   new_inter_freq_cell_v7b0ext[MAX_CELL_MEAS];
}new_inter_freq_cell_list_v7b0ext_t;

#define UMTS_SIB_TYPE_11BIS_V7B0_EXT_IES_NEW_INTER_FREQ_CELL_LIST_PRESENCE_FLAG 0x01
typedef struct
{
    U16                                 presence_bitmask;
    new_inter_freq_cell_list_v7b0ext_t  new_inter_freq_cell_list;
}sysinfotype11bis_v7b0ext_ies_t;

typedef struct
{
    U8               count;
    frequency_info_t frequency_info[MAX_DEDICATED_CSG_FREQ];
}csg_dedicated_frequency_info_list_t;

#define UMTS_SIB_TYPE_11BIS_V7B0_V860_IES_CSG_PSC_SPLIT_INFO_PRESENCE_FLAG                  0x01
#define UMTS_SIB_TYPE_11BIS_V7B0_V860_IES_CSG_CSG_DEDICATED_FREQ_INFO_LIST_PRESENCE_FLAG    0x02
typedef struct
{
    U16                     presence_bitmask;
    csg_psc_split_info_t    csg_pscsplit_info;
    csg_dedicated_frequency_info_list_t csg_dedicated_frequency_info_list;
}sysinfotype11bis_v860ext_ies_t;

typedef struct
{
    U8      __dummy__;
}sysinfotype11bis_v7b0nces_v860nces_nces_t;

#define UMTS_SIB_TYPE_11BIS_V7B0_V860_NCE_NCE_PRESENCE_FLAG     0x01
typedef struct
{
    U16                                         presence_bitmask;
    sysinfotype11bis_v860ext_ies_t              sys_info_type11bis_v860ext;
    sysinfotype11bis_v7b0nces_v860nces_nces_t   non_critical_extensions;
}sysinfotype11bis_v7b0nces_v860nces_t;

#define UMTS_SIB_TYPE_11BIS_V7B0_V860_NCE_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                     presence_bitmask;
    sysinfotype11bis_v7b0ext_ies_t          sys_info_type11bis_v7b0ext;
    sysinfotype11bis_v7b0nces_v860nces_t    v860non_critical_extensions;
}sysinfotype11bis_v7b0_nc_extensions_t;

/********************************************************************/
/*                                                                  */
/* umts_sib_type_11bis_info_t - System Information Block type 11BIS */
/*                                                                  */
/********************************************************************/
#define UMTS_SIB_TYPE11_BIS_MEAS_CTRL_SYSINFO_PRESENCE_FLAG                 0x01
#define UMTS_SIB_TYPE11_BIS_MEAS_CTRL_SYSINFO_LCR_PRESENCE_FLAG             0x02
#define UMTS_SIB_TYPE11_BIS_MEAS_CTRL_SYSINFO_EXTN_ADDON_R5_PRESENCE_FLAG   0x04
#define UMTS_SIB_TYPE11_BIS_V7B0_NCE_PRESENCE_FLAG                          0x08
typedef struct
{
    U16                                                 presence_bitmask;
    measurement_control_sys_info_extension_t            measurement_control_sys_info_extension;
    measurement_control_sys_info_extension_lcr_r4_t     measurement_control_sys_info_extension_lcr_r4;
    measurement_control_sys_info_extension_addon_r5_t   measurement_control_sys_info_extension_addon_r5;
    sysinfotype11bis_v7b0_nc_extensions_t               sysinfotype11bis_v7b0_nc_extensions;
}umts_sib_type_11bis_info_t;

typedef struct
{
    U8      inter_freq_tdd128_meas_ind;
}fach_measurement_occasion_info_lcr_r4_ext_t;

#define UMTS_SIB_INTRA_FREQ_CELL_INFO_SI_LIST_RSCP_LCR_R4_REMOVED_INTRA_FREQ_CELL_LIST_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                         presence_bitmask;
    removed_intra_freq_cell_list_t              removed_intra_freq_cell_list;
    new_intra_freq_cell_si_list_rscp_lcr_r4_t   new_intra_freq_cell_list;
}intra_freq_cell_info_si_list_rscp_lcr_r4_t;

typedef struct
{
    primary_ccpch_info_lcr_r4_t tdd;
}forbidden_affect_cell_lcr_r4_t;

typedef struct
{
    U8                              count;
    forbidden_affect_cell_lcr_r4_t  forbidden_affect_cell_lcr_r4[MAX_CELL_MEAS];
}forbidden_affect_cell_list_lcr_r4_t;

#define UMTS_SIB_EVENT_1A_LCR_R4_FORBIDDEN_AFFECT_CELL_LIST_PRESENCE_FLAG   0x01
typedef struct
{
    U16     presence_bitmask;
    U32     triggering_condition;
    U8      reporting_range;
    forbidden_affect_cell_list_lcr_r4_t forbidden_affect_cell_list;
    U8      w;
    U32     report_deactivation_threshold;
    U32     reporting_amount;
    U32     reporting_interval;
}event_1a_lcr_r4_t;

#define UMTS_SIB_EVENT_1B_LCR_R4_FORBIDDEN_AFFECT_CELL_LIST_PRESENCE_FLAG   0x01
typedef struct
{
    U16     presence_bitmask;
    U32     triggering_condition;
    U8      reporting_range;
    forbidden_affect_cell_list_lcr_r4_t forbidden_affect_cell_list;
    U8      w;
}event_1b_lcr_r4_t;

#define RRC_UMTS_INTRAFREQEVENT_LCR_R4_E1A 1
#define RRC_UMTS_INTRAFREQEVENT_LCR_R4_E1B 2
#define RRC_UMTS_INTRAFREQEVENT_LCR_R4_E1C 3
#define RRC_UMTS_INTRAFREQEVENT_LCR_R4_E1D 4
#define RRC_UMTS_INTRAFREQEVENT_LCR_R4_E1E 5
#define RRC_UMTS_INTRAFREQEVENT_LCR_R4_E1F 6
#define RRC_UMTS_INTRAFREQEVENT_LCR_R4_E1G 7
#define RRC_UMTS_INTRAFREQEVENT_LCR_R4_E1H 8
#define RRC_UMTS_INTRAFREQEVENT_LCR_R4_E1I 9
typedef struct
{
    U32 t;
    union {
        /* t = 1 */
        event_1a_lcr_r4_t    e1a;
        /* t = 2 */
        event_1b_lcr_r4_t    e1b;
        /* t = 3 */
        event_1c_t           e1c;
        /* t = 4 */
        /* t = 5 */
        event_1e_t           e1e;
        /* t = 6 */
        event_1f_t           e1f;
        /* t = 7 */
        /* t = 8 */
        S16                 e1h;
        /* t = 9 */
        S16                 e1i;
    } u;
}intra_freq_event_lcr_r4_t;

#define UMTS_SIB_INTRA_FREQ_EVENT_CRITERIA_LCR_R4_REPORTING_CELL_STATUS_PRESENCE_FLAG    0x01
typedef struct
{
    U16                         presence_bitmask;
    intra_freq_event_lcr_r4_t   event;
    U8                          hysteresis;
    U32                         timeToTrigger;
    reporting_cell_status_t     reporting_cell_status;
}intra_freq_event_criteria_lcr_r4_t;

typedef struct
{
    U8                                  count;
    intra_freq_event_criteria_lcr_r4_t  intra_freq_event_criteria_lcr_r4[MAX_MEAS_EVENT];
}intra_freq_event_criteria_list_lcr_r4_t;

#define UMTS_SIB_INTRA_FREQ_REPORTING_CRITERIA_LCR_R4_EVENT_CRITERIA_LIST_PRESENCE_FLAG 0x01
typedef struct
{
    U16                                     presence_bitmask;
    intra_freq_event_criteria_list_lcr_r4_t event_criteria_list;
}intra_freq_reporting_criteria_lcr_r4_t;

#define RRC_UMTS_CELLDCH_REPORTCRITERIA_LCR_R4_INTRAFREQREPORTINGCRITERIA 1
#define RRC_UMTS_CELLDCH_REPORTCRITERIA_LCR_R4_PERIODICALREPORTINGCRITERIA 2
typedef struct
{
    U32 t;
    union 
    {
        /* t = 1 */
        intra_freq_reporting_criteria_lcr_r4_t      intra_freq_reporting_criteria;
        /* t = 2 */
        periodical_reporting_criteria_t             periodical_reporting_criteria;
    }u;
}cell_dch_report_criteria_lcr_r4_t;

typedef struct
{
    intra_freq_reporting_quantity_t     intra_freq_reporting_quantity;
    measurement_reporting_mode_t        measurement_reporting_mode;
    cell_dch_report_criteria_lcr_r4_t   report_criteria;
}reporting_info_for_cell_dch_lcr_r4_t;

#define UMTS_SIB_INTRA_FREQ_MEASUREMENT_SYS_INFO_RSCP_LCR_R4_INTRA_FREQ_CELL_INFO_SI_LIST_PRESENCE_FLAG 0x01
#define UMTS_SIB_INTRA_FREQ_MEASUREMENT_SYS_INFO_RSCP_LCR_R4_INTRA_FREQ_MEAS_QUANTITY_PRESENCE_FLAG     0x02
#define UMTS_SIB_INTRA_FREQ_MEASUREMENT_SYS_INFO_RSCP_LCR_R4_INTRA_FREQ_REPORTING_QUANTITY_FOR_RACH_PRESENCE_FLAG   0x04
#define UMTS_SIB_INTRA_FREQ_MEASUREMENT_SYS_INFO_RSCP_LCR_R4_MAX_REPORTED_CELL_SON_RACH_PRESENCE_FLAG   0x08
#define UMTS_SIB_INTRA_FREQ_MEASUREMENT_SYS_INFO_RSCP_LCR_R4_REPORTING_INFO_FOR_CELL_DCH_PRESENCE_FLAG  0x10
typedef struct
{
    U16                                         presence_bitmask;
    U8                                          intra_freq_measurement_id;
    intra_freq_cell_info_si_list_rscp_lcr_r4_t  intra_freq_cell_info_si_list;
    intra_freq_meas_quantity_t                  intra_freq_meas_quantity;
    intra_freq_reporting_quantity_for_rach_t    intra_freq_reporting_quantity_for_rach;
    U32                                         max_reported_cells_on_rach;
    reporting_info_for_cell_dch_lcr_r4_t        reporting_info_for_cell_dch;
}intra_freq_measurement_sys_info_rscp_lcr_r4_t;

#define UMTS_SIB_INTER_FREQ_CELL_INFO_SI_LIST_RSCP_LCR_REMOVED_INTER_FREQ_CELL_LIST_PRESENCE_FLAG               0x01
#define UMTS_SIB_INTER_FREQ_CELL_INFO_SI_LIST_RSCP_LCR_NEW_INTER_FREQ_CELL_SI_LIST_RSCP_LCR_R4_PRESENCE_FLAG    0x02
typedef struct
{
    U16                                 presence_bitmask;
    removed_inter_freq_cell_list_t      removed_inter_freq_cell_list;
    new_inter_freq_cell_si_list_rscp_lcr_r4_t   new_inter_freq_cell_si_list_rscp_lcr_r4;
}inter_freq_cell_info_si_list_rscp_lcr_t;

#define UMTS_SIB_INTER_FREQ_MEASUREMENT_SYS_INFO_RSCP_LCR_R4_CELL_INFO_SI_LIST_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                     presence_bitmask;
    inter_freq_cell_info_si_list_rscp_lcr_t inter_freq_cell_info_si_list_rscp_lcr;
}inter_freq_measurement_sys_info_rscp_lcr_r4_t;

#define UMTS_SIB_INTRA_FREQ_MEASUREMENT_SYS_INFO_RSCP_LCR_R4_PRESENCE_FLAG  0x01
#define UMTS_SIB_INTER_FREQ_MEASUREMENT_SYS_INFO_RSCP_LCR_R4_PRESENCE_FLAG  0x02
typedef struct
{
    U16                 presence_bitmask;
    intra_freq_measurement_sys_info_rscp_lcr_r4_t intra_freq_measurement_sysInfo;
    inter_freq_measurement_sys_info_rscp_lcr_r4_t inter_freq_measurement_sysInfo;
}measurement_control_sys_info_lcr_r4_ext_use_of_hcs_hcs_not_used_cell_select_quality_measure_cpich_rscp_t;

#define UMTS_SIB_INTRA_FREQ_CELL_INFO_SI_LIST_ECNO_LCR_R4_REMOVED_INTRA_FREQ_CELL_LIST_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                 presence_bitmask;
    removed_intra_freq_cell_list_t      removed_intra_freq_cell_list;
    new_intra_freq_cell_si_list_ecno_lcr_r4_t   new_intra_freq_cell_si_list_ecno_lcr_r4;
}intra_freq_cell_info_si_list_ecno_lcr_r4_t;

#define UMTS_SIB_INTRA_FREQ_MEASUREMENT_SYS_INFO_ECNO_LCR_R4_INTRA_FREQ_CELL_INFO_SI_LIST_PRESENCE_FLAG 0x01
#define UMTS_SIB_INTRA_FREQ_MEASUREMENT_SYS_INFO_ECNO_LCR_R4_INTRA_FREQ_MEAS_QUANTITY_PRESENCE_FLAG     0x02
#define UMTS_SIB_INTRA_FREQ_MEASUREMENT_SYS_INFO_ECNO_LCR_R4_INTRA_FREQ_REPORTING_QUANTITY_FOR_RACH_PRESENCE_FLAG   0x04
#define UMTS_SIB_INTRA_FREQ_MEASUREMENT_SYS_INFO_ECNO_LCR_R4_MAX_REPORTED_CELLS_ON_RACH_PRESENCE_FLAG   0x08
#define UMTS_SIB_INTRA_FREQ_MEASUREMENT_SYS_INFO_ECNO_LCR_R4_REPORTING_INFO_FOR_CELL_DCH_PRESENCE_FLAG  0x10
typedef struct
{
    U16                                         presence_bitmask;
    U8                                          intra_freq_measurement_id;
    intra_freq_cell_info_si_list_ecno_lcr_r4_t  intra_freq_cell_info_si_list;
    intra_freq_meas_quantity_t                  intra_freq_meas_quantity;
    intra_freq_reporting_quantity_for_rach_t    intra_freq_reporting_quantity_for_rach;
    U32                                         max_reported_cells_on_rach;
    reporting_info_for_cell_dch_lcr_r4_t        reporting_info_for_cell_dch;
}intra_freq_measurement_sys_info_ecno_lcr_r4_t;

#define UMTS_SIB_INTER_FREQ_CELL_INFO_SI_LIST_ECNO_LCR_REMOVED_INTER_FREQ_CELL_LIST_PRESENCE_FLAG   0x01
#define UMTS_SIB_INTER_FREQ_CELL_INFO_SI_LIST_ECNO_LCR_NEW_INTER_FREQ_CELL_LIST_PRESENCE_FLAG       0x02
typedef struct
{
    U16                             presence_bitmask;
    removed_inter_freq_cell_list_t  removed_inter_freq_cell_list;
    new_inter_freq_cell_si_list_ecno_lcr_r4_t   new_inter_freq_cell_list;
}inter_freq_cell_info_si_list_ecno_lcr_t;

#define UMTS_SIB_INTER_FREQ_CELL_INFO_SI_LIST_ECNO_LCR_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                     presence_bitmask;
    inter_freq_cell_info_si_list_ecno_lcr_t inter_freq_cell_info_si_list;
}inter_freq_measurement_sys_info_ecno_lcr_r4_t;

#define UMTS_SIB_INTRA_FREQ_MEASUREMENT_SYS_INFO_ECNO_LCR_R4_PRESENCE_FLAG  0x01
#define UMTS_SIB_INTER_FREQ_MEASUREMENT_SYS_INFO_ECNO_LCR_R4_PRESENCE_FLAG  0x02
typedef struct
{
    U16                 presence_bitmask;
    intra_freq_measurement_sys_info_ecno_lcr_r4_t   intra_freq_measurement_sys_info;
    inter_freq_measurement_sys_info_ecno_lcr_r4_t   inter_freq_measurement_sys_info;
}measurement_control_sys_info_lcr_r4_ext_use_of_hcs_hcs_not_used_cell_select_quality_measure_cpich_ec_no_t;

#define RRC_UMTS_MEASUREMENTCONTROLSYSINFO_LCR_R4_EXT_USE_OF_HCS_HCS_NOT_USED_CELLSELECTQUALITYMEASURE_CPICH_RSCP 1
#define RRC_UMTS_MEASUREMENTCONTROLSYSINFO_LCR_R4_EXT_USE_OF_HCS_HCS_NOT_USED_CELLSELECTQUALITYMEASURE_CPICH_EC_N0 2
typedef struct
{
    U32                 t;
    union
    {
        measurement_control_sys_info_lcr_r4_ext_use_of_hcs_hcs_not_used_cell_select_quality_measure_cpich_rscp_t   cpich_rscp;
        measurement_control_sys_info_lcr_r4_ext_use_of_hcs_hcs_not_used_cell_select_quality_measure_cpich_ec_no_t  cpich_ec_no;
    }u;
}measurement_control_sys_info_lcr_r4_ext_use_of_hcs_hcs_not_used_cell_select_quality_measure_t;

typedef struct
{
    measurement_control_sys_info_lcr_r4_ext_use_of_hcs_hcs_not_used_cell_select_quality_measure_t   cell_select_quality_measure;
}measurement_control_sys_info_lcr_r4_ext_use_of_hcs_hcs_not_used_t;

#define UMTS_SIB_INTRA_FREQ_CELL_INFO_SI_LIST_HCS_RSCP_LCR_R4_REMOVED_INTRA_FREQ_CELL_LIST_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                 presence_bitmask;
    removed_intra_freq_cell_list_t      removed_intra_freq_cell_list;
    new_intra_freq_cell_si_list_hcs_rscp_lcr_r4_t   new_intra_freq_cell_list;
}intra_freq_cell_info_si_list_hcs_rscp_lcr_r4_t;

#define INTRA_FREQ_MEASUREMENT_SYS_INFO_HCS_RSCP_LCR_R4_INTRA_FREQ_CELL_INFO_SI_LIST_PRESENCE_FLAG  0x01
#define INTRA_FREQ_MEASUREMENT_SYS_INFO_HCS_RSCP_LCR_R4_INTRA_FREQ_MEAS_QUANTITY_PRESENCE_FLAG      0x02
#define INTRA_FREQ_MEASUREMENT_SYS_INFO_HCS_RSCP_LCR_R4_INTRA_FREQ_REPORTING_QUANTITY_FOR_RACH_PRESENCE_FLAG    0x04
#define INTRA_FREQ_MEASUREMENT_SYS_INFO_HCS_RSCP_LCR_R4_MAX_REPORTED_CELLS_ON_RACH_PRESENCE_FLAG    0x08
#define INTRA_FREQ_MEASUREMENT_SYS_INFO_HCS_RSCP_LCR_R4_REPORTING_INFO_FOR_CELL_DCH_PRESENCE_FLAG   0x10
typedef struct
{
    U16                                             presence_bitmask;
    U8                                              intra_freq_measurement_id;
    intra_freq_cell_info_si_list_hcs_rscp_lcr_r4_t  intra_freq_cell_info_si_list;
    intra_freq_meas_quantity_t                      intra_freq_meas_quantity;
    intra_freq_reporting_quantity_for_rach_t        intra_freq_reporting_quantity_for_rach;
    U32                                             max_reported_cells_on_rach;
    reporting_info_for_cell_dch_lcr_r4_t            reporting_info_for_cell_DCH;
}intra_freq_measurement_sys_info_hcs_rscp_lcr_r4_t;

#define UMTS_SIB_INTER_FREQ_CELL_INFO_SI_LIST_HCS_RSCP_LCR_REMOVED_INTER_FREQ_CELL_LIST_PRESENCE_FLAG  0x01
#define UMTS_SIB_INTER_FREQ_CELL_INFO_SI_LIST_HCS_RSCP_LCR_NEW_INTER_FREQ_CELL_SI_LIST_PRESENCE_FLAG  0x02
typedef struct
{
    U16                                             presence_bitmask;
    removed_inter_freq_cell_list_t                  removed_inter_freq_cell_list;
    new_inter_freq_cell_si_list_hcs_rscp_lcr_r4_t   new_inter_freq_cell_si_list;
}inter_freq_cell_info_si_list_hcs_rscp_lcr_t;

#define INTER_FREQ_MEASUREMENT_SYS_INFO_HCS_RSCP_LCR_R4_INTER_FREQ_CELL_INFO_SI_LIST_PRESENCE_FLAG  0x01
typedef struct
{
    U16                                         presence_bitmask;
    inter_freq_cell_info_si_list_hcs_rscp_lcr_t inter_freq_cell_info_si_list;
}inter_freq_measurement_sys_info_hcs_rscp_lcr_r4_t;

#define UMTS_SIB_INTRA_FREQ_MEASUREMENT_SYS_INFO_HCS_RSCP_LCR_R4_PRESENCE_FLAG  0x01
#define UMTS_SIB_INTER_FREQ_MEASUREMENT_SYS_INFO_HCS_RSCP_LCR_R4_PRESENCE_FLAG  0x02
typedef struct
{
    U16                          presence_bitmask;
    intra_freq_measurement_sys_info_hcs_rscp_lcr_r4_t    intra_freq_measurement_sys_info;
    inter_freq_measurement_sys_info_hcs_rscp_lcr_r4_t    inter_freq_measurement_sys_info;
}measurement_control_sys_info_lcr_r4_ext_use_of_hcs_hcs_used_cell_select_quality_measure_cpich_rscp_t;

#define UMTS_SIB_INTRA_FREQ_CELL_INFO_SI_LIST_HCS_ECNO_LCR_R4_REMOVED_INTRA_FREQ_CELL_LIST_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                 presence_bitmask;
    removed_intra_freq_cell_list_t      removed_intra_freq_cell_list;
    new_intra_freq_cell_si_list_hcs_ecno_lcr_r4_t   new_intra_freq_cell_si_list;
}intra_freq_cell_info_si_list_hcs_ecno_lcr_r4_t;

#define UMTS_SIB_INTRA_FREQ_MEASUREMENT_SYS_INFO_HCS_ECNO_LCR_R4_INTRA_FREQ_CELL_INFO_SI_LIST_PRESENCE_FLAG 0x01
#define UMTS_SIB_INTRA_FREQ_MEASUREMENT_SYS_INFO_HCS_ECNO_LCR_R4_INTRA_FREQ_MEAS_QUANTITY_PRESENCE_FLAG     0x02
#define UMTS_SIB_INTRA_FREQ_MEASUREMENT_SYS_INFO_HCS_ECNO_LCR_R4_INTRA_FREQ_REPORTING_QUANTITY_FOR_RACH_PRESENCE_FLAG   0x04
#define UMTS_SIB_INTRA_FREQ_MEASUREMENT_SYS_INFO_HCS_ECNO_LCR_R4_MAX_REPORTED_CELLS_ON_RACH_PRESENCE_FLAG   0x08
#define UMTS_SIB_INTRA_FREQ_MEASUREMENT_SYS_INFO_HCS_ECNO_LCR_R4_REPORTING_INFO_FOR_CELL_DCH_PRESENCE_FLAG  0x10
typedef struct
{
    U16                                             presence_bitmask;
    U8                                              intra_freq_measurement_id;
    intra_freq_cell_info_si_list_hcs_ecno_lcr_r4_t  intra_freq_cell_info_si_list;
    intra_freq_meas_quantity_t                      intra_freq_meas_quantity;
    intra_freq_reporting_quantity_for_rach_t        intra_freq_reporting_quantity_for_rach;
    U32                                             max_reported_cells_on_rach;
    reporting_info_for_cell_dch_lcr_r4_t            reporting_info_for_cell_dch;
}intra_freq_measurement_sys_info_hcs_ecno_lcr_r4_t;

#define UMTS_SIB_INTER_FREQ_CELL_INFO_SI_LIST_HCS_ECNO_LCR_REMOVED_INTER_FREQ_CELL_LIST_PRESENCE_FLAG   0x01
#define UMTS_SIB_INTER_FREQ_CELL_INFO_SI_LIST_HCS_ECNO_LCR_NEW_INTER_FREQ_CELL_SI_LIST_PRESENCE_FLAG   0x02
typedef struct
{
    U16                                     presence_bitmask;
    removed_inter_freq_cell_list_t          removed_inter_freq_cell_list;
    new_inter_freq_cell_si_list_hcs_ecno_lcr_r4_t   new_inter_freq_cell_si_list;
}inter_freq_cell_info_si_list_hcs_ecno_lcr_t;

#define UMTS_SIB_INTER_FREQ_CELL_INFO_SI_LIST_HCS_ECNO_LCR_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                         presence_bitmask;
    inter_freq_cell_info_si_list_hcs_ecno_lcr_t inter_freq_cell_info_si_list;
}inter_freq_measurement_sys_info_hcs_ecno_lcr_r4_t;

#define UMTS_SIB_INTRA_FREQ_MEASUREMENT_SYS_INFO_HCS_ECNO_LCR_R4_PRESENCE_FLAG   0x01
#define UMTS_SIB_INTER_FREQ_MEASUREMENT_SYS_INFO_HCS_ECNO_LCR_R4_PRESENCE_FLAG   0x02
typedef struct
{
    U16                                 presence_bitmask;
    intra_freq_measurement_sys_info_hcs_ecno_lcr_r4_t   intra_freq_measurement_sys_info;
    inter_freq_measurement_sys_info_hcs_ecno_lcr_r4_t   inter_freq_measurement_sys_info;
}measurement_control_sys_info_lcr_r4_ext_use_of_hcs_hcs_used_cell_select_quality_measure_cpich_ecno_t;

#define RRC_UMTS_MEASUREMENTCONTROLSYSINFO_LCR_R4_EXT_USE_OF_HCS_HCS_USED_CELLSELECTQUALITYMEASURE_CPICH_RSCP 1
#define RRC_UMTS_MEASUREMENTCONTROLSYSINFO_LCR_R4_EXT_USE_OF_HCS_HCS_USED_CELLSELECTQUALITYMEASURE_CPICH_EC_N0 2
typedef struct
{
    U32 t;
    union {
        /* t = 1 */
        measurement_control_sys_info_lcr_r4_ext_use_of_hcs_hcs_used_cell_select_quality_measure_cpich_rscp_t    cpich_rscp;
        /* t = 2 */
        measurement_control_sys_info_lcr_r4_ext_use_of_hcs_hcs_used_cell_select_quality_measure_cpich_ecno_t    cpich_ec_no;
    } u;
}measurement_control_sys_info_lcr_r4_ext_use_of_hcs_hcs_used_cell_select_quality_measure_t;

typedef struct
{
    measurement_control_sys_info_lcr_r4_ext_use_of_hcs_hcs_used_cell_select_quality_measure_t   cell_select_quality_measure;
}measurement_control_sys_info_lcr_r4_ext_use_of_hcs_hcs_used_t;

#define RRC_UMTS_MEASUREMENTCONTROLSYSINFO_LCR_R4_EXT_USE_OF_HCS_HCS_NOT_USED 1
#define RRC_UMTS_MEASUREMENTCONTROLSYSINFO_LCR_R4_EXT_USE_OF_HCS_HCS_USED 2
typedef struct
{
    U32         t;
    union
    {
        /* t = 1 */
        measurement_control_sys_info_lcr_r4_ext_use_of_hcs_hcs_not_used_t  hcs_not_used;
        /* t = 2 */
        measurement_control_sys_info_lcr_r4_ext_use_of_hcs_hcs_used_t      hcs_used;
    }u;
}measurement_control_sys_info_lcr_r4_ext_use_of_hcs_t;

typedef struct
{
    measurement_control_sys_info_lcr_r4_ext_use_of_hcs_t    use_of_hcs;
}measurement_control_sys_info_lcr_r4_ext_t;

#define UMTS_SIB_TYPE12_V4B0EXT_FACH_MEASUREMENT_OCCASION_INFO_LCR_R4_EXT_PRESENCE_FLAG 0x01
typedef struct
{
    U16                                             presence_bitmask;
    fach_measurement_occasion_info_lcr_r4_ext_t     fach_measurement_occasion_info_lcr_r4_ext;
    measurement_control_sys_info_lcr_r4_ext_t       measurement_control_sys_info_lcr_r4_ext;
}sysinfotype12_v4b0ext_ies_t;

#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_V590EXT_DELTA_Q_RX_LEV_MIN_PRESENCE_FLAG 0x01
#define UMTS_SIB_CELL_SELECT_RESELECT_INFO_V590EXT_DELTA_Q_HCS_PRESENCE_FLAG        0x02
typedef struct
{
    U16             presence_bitmask;
    S8              delta_q_rx_lev_min;
    S8              delta_q_hcs;
}cell_select_reselect_info_v590ext_t;

typedef struct
{
    U8                  count;
    cell_select_reselect_info_v590ext_t cell_select_reselect_info_v590ext[MAX_CELL_MEAS];
}sysinfotype12_v590ext_ies_new_intra_frequency_cell_info_list_v590ext_t;

typedef struct
{
    U8                  count;
    cell_select_reselect_info_v590ext_t cell_select_reselect_info_v590ext[MAX_CELL_MEAS];
}sysinfotype12_v590ext_ies_new_inter_rat_cell_info_list_v590ext_t;

typedef struct
{
    U8                  count;
    cell_select_reselect_info_v590ext_t cell_select_reselect_info_v590ext[MAX_CELL_MEAS];
}sysinfotype12_v590ext_ies_new_inter_frequency_cell_info_list_v590ext_t;

typedef struct
{
    U32     n;
    S8      elem[8];
}intra_freq_event_criteria_list_v590ext_t;

typedef struct
{
    U32      reporting_amount;
    U32      reporting_interval;
}periodic_reporting_info_1b_t;

typedef struct
{
    periodic_reporting_info_1b_t    periodic_reporting_info_1b;
}intra_freq_reporting_criteria_1b_r5_t;

#define UMTS_SIB_INTRA_FREQ_EVENT_1D_R5_TRIGGERING_CONDITION_PRESENCE_FLAG  0x01
#define UMTS_SIB_INTRA_FREQ_EVENT_1D_R5_USE_CIO_PRESENCE_FLAG               0x02
typedef struct
{
    U16     presence_bitmask;
    U32     triggering_condition;
    U8      use_cio;
}intra_freq_event_1d_r5_t;

#define UMTS_SIB_TYPE12_NEW_INTRA_FREQUENCY_CELL_INFO_LIST_V590EXT_PRESENCE_FLAG    0x01
#define UMTS_SIB_TYPE12_NEW_INTER_FREQUENCY_CELL_INFO_LIST_V590EXT_PRESENCE_FLAG    0x02
#define UMTS_SIB_TYPE12_NEW_INTER_RAT_CELL_INFO_LIST_V590EXT_PRESENCE_FLAG          0x04
#define UMTS_SIB_TYPE12_INTRA_FREQ_EVENT_CRITERIA_LIST_V590EXT_PRESENCE_FLAG        0x08
#define UMTS_SIB_TYPE12_INTRA_FREQ_REPORTING_CRITERIA_1B_R5_PRESENCE_FLAG           0x10
#define UMTS_SIB_TYPE12_INTRA_FREQ_EVENT_1D_R5_PRESENCE_FLAG                        0x20
typedef struct
{
    U16                                 presence_bitmask;
    sysinfotype12_v590ext_ies_new_intra_frequency_cell_info_list_v590ext_t  new_intra_frequency_cell_info_list_v590ext;
    sysinfotype12_v590ext_ies_new_inter_frequency_cell_info_list_v590ext_t  new_inter_frequency_cell_info_list_v590ext;
    sysinfotype12_v590ext_ies_new_inter_rat_cell_info_list_v590ext_t        new_inter_rat_cell_info_list_v590ext;
    intra_freq_event_criteria_list_v590ext_t                                intra_freq_event_criteria_list_v590ext;
    intra_freq_reporting_criteria_1b_r5_t                                   intra_freq_reporting_criteria_1b_r5;
    intra_freq_event_1d_r5_t                                                intra_freq_event_1d_r5;
}sysinfotype12_v590ext_ies_t;

typedef struct
{
    U32         n;
    U32         elem[2];
}inter_freq_rep_quantity_rach_tdd_list_t;

#define RRC_UMTS_DUMMY_INTERFREQRACHREPORTINGINFO_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_DUMMY_INTERFREQRACHREPORTINGINFO_MODESPECIFICINFO_TDD 2
#define RRC_UMTS_DUMMY_INTERFREQRACHREPORTINGINFO_MODESPECIFICINFO_INTERFREQRACHREPORTINGTHRESHOLD 3
#define RRC_UMTS_DUMMY_INTERFREQRACHREPORTINGINFO_MODESPECIFICINFO_MAXREPORTEDCELLSONRACHINTERFREQ 4
typedef struct
{
    U32 t;
    union {
        /* t = 1 */
        dummy_interfreqrachreportinginfo_modespecificinfo_fdd_t fdd;
        /* t = 2 */
        dummy_interfreqrachreportingInfo_modespecificinfo_tdd_t tdd;
        /* t = 3 */
        S8              inter_freq_rach_reporting_threshold;
        /* t = 4 */
        U8              max_reported_cells_on_rach_inter_freq;
    } u;
}dummy_inter_freq_rach_reporting_info_mode_specific_info_t;

typedef struct
{
    dummy_inter_freq_rach_reporting_info_mode_specific_info_t  mode_specific_info;
}dummy_inter_freq_rach_reporting_info_t;

#define UMTS_SIB_DUMMY_INTER_FREQ_RACH_REPORTING_INFO_PRESENCE_FLAG 0x01
typedef struct
{
    U16                                     presence_bitmask;
    dummy_inter_freq_rach_reporting_info_t  dummy;
}sysinfotype12_v690ext_ies_t;

typedef struct
{
    U32         inter_freq_rep_quantity_rach_fdd;
}inter_freq_rach_reporting_info_mode_specific_info_fdd_t;

typedef struct
{
    inter_freq_rep_quantity_rach_tdd_list_t inter_freq_rep_quantity_rach_tdd_list;
}inter_freq_rach_reporting_info_mode_specific_info_tdd_t;

#define RRC_UMTS_INTERFREQRACHREPORTINGINFO_MODESPECIFICINFO_FDD 1
#define RRC_UMTS_INTERFREQRACHREPORTINGINFO_MODESPECIFICINFO_TDD 2
typedef struct
{
    U32 t;
    union {
        /* t = 1 */
        inter_freq_rach_reporting_info_mode_specific_info_fdd_t fdd;
        /* t = 2 */
        inter_freq_rach_reporting_info_mode_specific_info_tdd_t tdd;
    } u;
}inter_freq_rach_reporting_info_mode_specific_info_t;

typedef struct
{
    inter_freq_rach_reporting_info_mode_specific_info_t mode_specific_info;
    S8                  inter_freq_rach_reporting_threshold;
    U8                  max_reported_cells_on_rach_inter_freq;
}inter_freq_rach_reporting_info_t;

#define UMTS_SIB_TYPE12_INTER_FREQ_RACH_REPORTING_INFO_PRESENCE_FLAG 0x01
typedef struct
{
    U16                                 presence_bitmask;
    inter_freq_rach_reporting_info_t    inter_freq_rach_reporting_info;
}sysinfotype12_v6b0ext_ies_t;

#define UMTS_SIB_TYPE12_NEW_INTER_FREQ_CELL_LIST_PRESENCE_FLAG 0x01
typedef struct
{
    U16                                 presence_bitmask;
    new_inter_freq_cell_list_v7b0ext_t  new_inter_freq_cell_list;
}sysinfotype12_v7b0ext_ies_t;

typedef struct
{
    U8          __dummy__;
}sysinfotype12_v4b0nces_v590nce_v690nces_v6b0nces_v7b0nces_nces_t;

#define UMTS_SIB_TYPE12_V4B0_V590_V690_V6B0_7B0_NCE_NCE_PRESENCE_FLAG 0x01
typedef struct
{
    U16                             presence_bitmask;
    sysinfotype12_v7b0ext_ies_t     sys_info_type12_v7b0ext;
    sysinfotype12_v4b0nces_v590nce_v690nces_v6b0nces_v7b0nces_nces_t    nonCriticalExtensions;
}sysinfotype12_v4b0nces_v590nce_v690nces_v6b0nces_v7b0nces_t;

#define UMTS_SIB_TYPE12_V4B0_V590_V690_V6B0_7B0_NCE_PRESENCE_FLAG    0x01
typedef struct
{
    U16                             presence_bitmask;
    sysinfotype12_v6b0ext_ies_t     sys_info_type12_v6b0ext;
    sysinfotype12_v4b0nces_v590nce_v690nces_v6b0nces_v7b0nces_t v7b0NonCriticalExtensions;
}sysinfotype12_v4b0nces_v590nce_v690nces_v6b0nces_t;

#define UMTS_SIB_TYPE12_V4B0_V590_V690_V6B0_NCE_PRESENCE_FLAG    0x01
typedef struct
{
    U16                             presence_bitmask;
    sysinfotype12_v690ext_ies_t     sys_info_type12_v690ext;
    sysinfotype12_v4b0nces_v590nce_v690nces_v6b0nces_t  v6b0_non_critical_extensions;
}sysinfotype12_v4b0nces_v590nce_v690nces_t;

#define UMTS_SIB_TYPE12_V4B0_V590_V690_NCE_PRESENCE_FLAG    0x01
typedef struct
{
    U16                                         presence_bitmask;
    sysinfotype12_v590ext_ies_t                 sys_info_type12_v590ext;
    sysinfotype12_v4b0nces_v590nce_v690nces_t   v690_non_critical_extensions;
}sysinfotype12_v4b0nces_v590nce_t;

#define UMTS_SIB_TYPE12_V4B0EXT_IES_PRESENCE_FLAG   0x01
#define UMTS_SIB_TYPE12_V4B0_V590_NCE_PRESENCE_FLAG 0x02
typedef struct
{
    U16                                 presence_bitmask;
    sysinfotype12_v4b0ext_ies_t         sys_info_type12_v4b0ext;
    sysinfotype12_v4b0nces_v590nce_t    v590non_critical_extension;
}sysinfotype12_v4b0_nc_extensions_t;

/********************************************************************/
/*                                                                  */
/*  umts_sib_type_12_info_t - System Information Block type 12      */
/*                                                                  */
/********************************************************************/
#define UMTS_SIB_TYPE12_FACH_MEAS_OCCASION_INFO_PRESENCE_FLAG   0x01
#define UMTS_SIB_TYPE12_V4B0_NCE_PRESENCE_FLAG                  0x02
typedef struct
{
    U16                                 presence_bitmask;
    fach_measurement_occasion_info_t    fach_measurement_occasion_info;
    measurement_control_sys_info_t      measurement_control_sys_info;
    sysinfotype12_v4b0_nc_extensions_t  sysinfotype12_v4b0_nc_extensions;
}umts_sib_type_12_info_t;

/********************************************************************/
/*                                                                  */
/*  umts_sib_message_t                                              */
/*                                                                  */
/********************************************************************/
#define UMTS_MIB_INFO_DECODE_COMPLETE                              0x0001
#define UMTS_SIB_TYPE_1_INFO_DECODE_COMPLETE                       0x0002
#define UMTS_SIB_TYPE_2_INFO_DECODE_COMPLETE                       0x0004
#define UMTS_SIB_TYPE_3_INFO_DECODE_COMPLETE                       0x0008
#define UMTS_SIB_TYPE_5_INFO_DECODE_COMPLETE                       0x0010
#define UMTS_SIB_TYPE_7_INFO_DECODE_COMPLETE                       0x0020
#define UMTS_SIB_TYPE_11_INFO_DECODE_COMPLETE                      0x0040
#define UMTS_SIB_TYPE_11_BIS_INFO_DECODE_COMPLETE                  0x0080
#define UMTS_SIB_TYPE_12_INFO_DECODE_COMPLETE                      0x0100
#define UMTS_SIB_TYPE_18_INFO_DECODE_COMPLETE                      0x0200
#define UMTS_SB_BLOCK_1_INFO_DECODE_COMPLETE                       0x0400
#define UMTS_SB_BLOCK_2_INFO_DECODE_COMPLETE                       0x0800
typedef struct 
{
    U16                             presence_bitmask;
    umts_mib_info_t                 umts_mib_info;                  /* Master Information Block */ 
    umts_sib_type_1_info_t          umts_sib_type_1_info;           /* System Information Block type 1 */
    umts_sib_type_2_info_t          umts_sib_type_2_info;           /* System Information Block type 2 */
    umts_sib_type_3_info_t          umts_sib_type_3_info;           /* System Information Block type 3 */
    umts_sib_type_5_info_t          umts_sib_type_5_info;           /* System Information Block type 5 */
    umts_sib_type_7_info_t          umts_sib_type_7_info;           /* System Information Block type 7 */
    umts_sib_type_11_info_t         umts_sib_type_11_info;          /* System Information Block type 11 */
    umts_sib_type_11bis_info_t      umts_sib_type_11bis_info;       /* System Information Block type 11bis */
    umts_sib_type_12_info_t         umts_sib_type_12_info;          /* System Information Block type 12 */
    umts_sib_type_18_info_t         umts_sib_type_18_info;          /* System Information Block type 18 */
    umts_scheduling_block_1_info_t  umts_scheduling_block_1_info;   /* Scheduling Block 1 */
    umts_scheduling_block_2_info_t  umts_scheduling_block_2_info;   /* Scheduling Block 2 */
}umts_sib_message_t;

/*******************************************************************
 *                                                                  *
 *        UMTS_SIB_DECODER_GLOBAL_CTXT_T                            *
 *                                                                  *
 ********************************************************************/
#define FIRST_SEGMENT_DATA_BYTES            28
#define COMPLETE_SIB_SEGMENT_DATA_BYTES     29    
#define OCTET_SIZE                          8
#define MAX_ASN_ENCODED_MSG_SIZE            1000

typedef struct
{
    void              *p_start_bch_data;
    void              *p_bch_data;
    U8                 segment_count;
    U8                 segment_remaining;
}umts_bch_info_t;

#define UMTS_SYSTEM_INFORMATION_MESSAGES_SUPPORTED          12

typedef enum
{
    UMTS_MIB_INFO,
    UMTS_SIB_TYPE_1_INFO,
    UMTS_SIB_TYPE_2_INFO,
    UMTS_SIB_TYPE_3_INFO,
    UMTS_SIB_TYPE_5_INFO,
    UMTS_SIB_TYPE_7_INFO,
    UMTS_SIB_TYPE_11_INFO,
    UMTS_SIB_TYPE_11_bis_INFO,
    UMTS_SIB_TYPE_12_INFO,
    UMTS_SIB_TYPE_18_INFO,
    UMTS_SB_BLOCK_1_INFO,
    UMTS_SB_BLOCK_2_INFO,
    INVALID_SIB_TYPE
}umts_sib_type_et;

#define UMTS_MIB_INFO_COMPLETE                              0x0001
#define UMTS_SIB_TYPE_1_INFO_COMPLETE                       0x0002
#define UMTS_SIB_TYPE_2_INFO_COMPLETE                       0x0004
#define UMTS_SIB_TYPE_3_INFO_COMPLETE                       0x0008
#define UMTS_SIB_TYPE_5_INFO_COMPLETE                       0x0010
#define UMTS_SIB_TYPE_7_INFO_COMPLETE                       0x0020
#define UMTS_SIB_TYPE_11_INFO_COMPLETE                      0x0040
#define UMTS_SIB_TYPE_11_BIS_INFO_COMPLETE                  0x0080
#define UMTS_SIB_TYPE_12_INFO_COMPLETE                      0x0100
#define UMTS_SIB_TYPE_18_INFO_COMPLETE                      0x0200
#define UMTS_SB_BLOCK_1_INFO_COMPLETE                       0x0400
#define UMTS_SB_BLOCK_2_INFO_COMPLETE                       0x0800
typedef struct
{
    U32                              complete_si_msg_filled_bitmask;
    umts_bch_info_t                  bch_info[UMTS_SYSTEM_INFORMATION_MESSAGES_SUPPORTED];
}umts_sib_decoder_global_ctxt_t;

typedef struct
{
    U32                              numbits;
    U8                               data[27];
}umts_sib_data_variable_t;

typedef struct
{
    U32                              sib_type;
    umts_sib_data_variable_t         umts_sib_data_variable;
}umts_completesib_info_t;    
#define UMTS_MAX_SIB_PER_MSG         16
typedef struct
{
    U8                               count;
    umts_completesib_info_t          umts_completesib_info[UMTS_MAX_SIB_PER_MSG];
}umts_completesib_info_list_t;    

extern void fill_plmn_identity
(
 UMTS_PLMN_Identity       *asn_plmn_identity,
 umts_plmn_identity_t     *umts_plmn_idenity
 );

extern void fill_plmn_type_gsm_map
( 
 UMTS_PLMN_Type_gsm_MAP           *asn_plmn_type_gsm,
 plmn_type_gsm_map_t         *umts_plmn_type_gsm
 );


extern void fill_plmn_type_ansi_41
(
 UMTS_PLMN_Type_ansi_41           *asn_plmn_type_ansi_41,
 plmn_type_ansi_41_t         *umts_plmn_type_ansi_41
 );


extern void fill_plmn_type_gsm_map_ansi_41
(   
 UMTS_PLMN_Type_gsm_MAP_and_ANSI_41   *asn_plmn_type_gsm_map_ansi_41,
 plmn_type_gsm_map_and_ansi_41_t *umts_plmn_type_gsm_map_ansi_41
 );


extern void convert_plmn_type
(
 UMTS_PLMN_Type       *asn_plmn_type,
 plmn_type_t     *umts_plmn_type
 );

extern void fill_sib_sb_type
(
 UMTS_SIBSb_TypeAndTag        *asn_sib_type_tag,
 sib_sb_type_and_tag_t   *umts_sib_type_tag
 );

extern void fill_scheduling_pos
(
 UMTS_SchedulingInformation_scheduling_sib_Pos    *asn_scheduling_sib_pos,
 schedulingInformation_scheduling_sib_pos_t  *umts_scheduling_sib_pos
 );

extern void fill_scheduling_info
(
 UMTS_SchedulingInformation       *asn_scheduling,
 scheduling_information_t    *umts_scheduling
 );

extern void fill_scheduling_info_sib_sb
(  
 UMTS_SchedulingInformationSIBSb          *asn_scheduling_info_sibsb,
 scheduling_information_sib_sb_t     *umts_scheduling_info_sibsb
 );

extern void convert_sibsb_reference_list
(
 UMTS_SIBSb_ReferenceList     *asn_sibsb_reference_list,
 sibsb_reference_list_t  *umts_sibsb_reference_list
 );

extern return_code decode_plmn_identity_with_optional_mcc_r6
(
 UMTS_PLMN_IdentityWithOptionalMCC_r6         *asn_plmn_id_with_optional_mcc_r6,
 plmn_identity_with_optional_mcc_r6_t    *umts_plmn_id_with_optional_mcc_r6
 );


extern void decode_multiple_plmn_list_r6_multiple_plmns
(
 UMTS_MultiplePLMN_List_r6_multiplePLMNs      *asn_multiple_plmns,
 multiple_plmn_list_r6_multiple_plmns_t  *umts_multiple_plmns
 );

extern return_code decoce_multiple_plmn_list_r6
(
 UMTS_MultiplePLMN_List_r6            *asn_multiple_plmn_list_r6,
 multiple_plmn_list_r6_t         *umts_multiple_plmn_list_r6
 );

extern void decode_mib_v690ext
(
 UMTS_MasterInformationBlock_v690ext          *asn_mib_v690ext,
 mib_v690ext_t                           *umts_mib_v690ext
 );

extern void decode_mib_v860ext_ies
(
 UMTS_MasterInformationBlock_v860ext_IEs  *asn_mib_v860ext_ies,
 mib_v860ext_ies_t                   *umts_mib_v860ext_ies
 );

extern void decode_mib_v690_nce_v6b0_nce_v860_nce_nce
(
 UMTS_MasterInformationBlock_v690NonCriticalExtensions_v6b0NonCriticalExtensions_v860NonCriticalExtensions                                    *asn_mib_v690_nce_v6b0_nce_v860_nce_nce,
 mib_v690_nce_v6b0_nce_v860_nce_nce_t  *umts_mib_v690_nce_v6b0_nce_v860_nce_nce
 );


extern void decode_mib_v690_nce_v6b0_nce
(
 UMTS_MasterInformationBlock_v690NonCriticalExtensions_v6b0NonCriticalExtensions
 *asn_mib_v690_nce_v6b0_nce,
 mib_v690_nce_v6b0_nce_t             *umts_mib_v690_nce_v6b0_nce
 );

extern void decode_mib_v690_nce
(
 UMTS_MasterInformationBlock_v690NonCriticalExtensions  *asn_mib_v690_nce,
 mib_v690_nce_t                                         *umts_mib_v690_nce
 );

extern return_code convert_mib
(
 OSCTXT          *pctxt,
 UMTS_CompleteSIB     *pcomplete_sib_segment,
 umts_mib_info_t *umts_mib_info
 );

extern void fill_sib_type
(
 UMTS_SIB_TypeAndTag          *asn_sib_type_tag,
 sib_type_and_tag_t      *umts_sib_type_tag
 );

extern void fill_scheduling_info_sib
(  
 UMTS_SchedulingInformationSIB            *asn_scheduling_info_sib,
 scheduling_information_sib_t        *umts_scheduling_info_sib
 );

extern void convert_sib_reference_list
(
 UMTS_SIB_ReferenceList     *asn_sib_reference_list,
 sib_reference_list_t  *umts_sib_reference_list
 );

extern return_code convert_scheduling_block1
(
 umts_scheduling_block_1_info_t *umts_sb_1_info
 );

extern return_code convert_scheduling_block2
(
 umts_scheduling_block_2_info_t *umts_sb_2_info
 );

extern void decode_explicit_discard
(
 UMTS_ExplicitDiscard         *asn_explicit_discard,
 explicit_discard_t      *umts_explicit_discard
 );

extern void decode_max_dat_retransmissions
(
 UMTS_MaxDAT_Retransmissions      *asn_max_dat_retransmissions,
 max_dat_retransmissions_t   *umts_max_dat_retransmissions
 );

extern void decode_transmission_rlc_discard
(
 UMTS_TransmissionRLC_Discard     *asn_transmission_rlc_discard,
 transmission_rlc_discard_t  *umts_transmission_rlc_discard
 );


extern void decode_polling_info
(
 UMTS_PollingInfo             *asn_polling_info,
 polling_info_t          *umts_polling_info
 );

extern void decode_ul_am_rlc_mode
(
 UMTS_UL_AM_RLC_Mode      *asn_ul_am_rlc_mode,
 ul_am_rlc_mode_t    *umts_ul_am_rlc_mode
 );

extern void decode_ul_um_rlc_mode
(
 UMTS_UL_UM_RLC_Mode      *asn_ul_um_rlc_mode,
 ul_um_rlc_mode_t    *umts_ul_um_rlc_mode
 );

extern void decode_ul_tm_rlc_mode
(
 UMTS_UL_TM_RLC_Mode      *asn_ul_tm_rlc_mode,
 ul_tm_rlc_mode_t    *umts_ul_tm_rlc_mode
 );

extern void decode_ul_rlc_mode
(
 UMTS_UL_RLC_Mode         *asn_ul_rlc_mode,
 ul_rlc_mode_t       *umts_ul_rlc_mode
 );

extern void decode_dl_rlc_status_info
(
 UMTS_DL_RLC_StatusInfo       *asn_dl_rlc_status_info,
 dl_rlc_status_info_t    *umts_dl_rlc_status_info
 );

extern void decode_dl_am_rlc_mode
(
 UMTS_DL_AM_RLC_Mode     *asn_dl_am_rlc_mode,
 dl_am_rlc_mode_t   *umts_dl_am_rlc_mode
 );

extern void decode_dl_tm_rlc_mode
(
 UMTS_DL_TM_RLC_Mode      *asn_dl_tm_rlc_mode,
 dl_tm_rlc_mode_t    *umts_dl_tm_rlc_mode
 );

extern void decode_dl_rlc_mode
(
 UMTS_DL_RLC_Mode     *asn_dl_rlc_mode,
 dl_rlc_mode_t   *umts_dl_rlc_mode
 );

extern void decode_rlc_info
(
 UMTS_RLC_Info        *asn_rlc_Info,
 rlc_info_t      *umts_rlc_Info
 );

extern void decode_rlc_info_choice
(
 UMTS_RLC_InfoChoice      *asn_rlc_info_choice,
 rlc_info_choice_t   *umts_rlc_info_choice
 );

extern void decode_ul_transport_channel_type
(
 UMTS_UL_TransportChannelType         *asn_ul_transport_channel_type,
 ul_transport_channel_type_t     *umts_ul_transport_channel_type
 );

extern void fill_rlc_size_explicit_list
(
 UMTS_RLC_SizeExplicitList        *asn_explicit_list,
 rlc_size_explicit_list_t    *umts_explicit_list
 );

extern void fill_ul_logical_channel_mapping_rlc_size_list
(
 UMTS_UL_LogicalChannelMapping_rlc_SizeList       *asn_rlc_size_list,
 ul_logical_channel_mapping_rlc_size_list_t  *umts_rlc_size_list
 );

extern void decode_one_ul_logical_channel_mapping
(
 UMTS_UL_LogicalChannelMapping         *asn_one_ul_logical_channel_mapping,
 ul_logical_channel_mapping_t     *umts_one_ul_logical_channel_mapping
 );

extern void fill_ul_logicalchannelmappinglist_ul_logicalchannelmapping
(
 UMTS_UL_LogicalChannelMappingList_ul_LogicalChannelMapping   *asn_mapping_list,
 ul_logicalchannelmappinglist_ul_logicalchannelmapping_t *umts_mapping_list
 );

extern void fill_ul_logical_channel_mapping_list
(
 UMTS_UL_LogicalChannelMappingList        *asn_ul_logical_channel_mapping_list,
 ul_logical_channel_mapping_list_t   *umts_ul_logical_channel_mapping_list
 );

extern void decode_ul_logical_channel_mappings_s
(
 UMTS_UL_LogicalChannelMappings       *asn_ul_logical_channel_mappings,
 ul_logical_channel_mappings_t   *umts_ul_logical_channel_mappings
 );

extern void decode_dl_transport_channel_type
(
 UMTS_DL_TransportChannelType         *asn_dl_transport_channel_type,
 dl_transport_channel_type_t     *umts_dl_transport_channel_type
 );

extern void decode_dl_logical_channel_mapping
(
 UMTS_DL_LogicalChannelMapping        *asn_dl_logical_channel_mapping,
 dl_logical_channel_mapping_t    *umts_dl_logical_channel_mapping
 );

extern void fill_dl_logical_channel_mappings
(
 UMTS_DL_LogicalChannelMappingList        *asn_dl_logical_channel_mapping_list,
 dl_logical_channel_mapping_list_t   *umts_dl_logical_channel_mapping_list
 );

extern void decode_rb_mapping_option
(
 UMTS_RB_MappingOption        *asn_rb_mapping_option,
 rb_mapping_option_t     *umts_rb_mapping_option
 );

extern void fill_rb_mapping_info_list
(
 UMTS_RB_MappingInfo              *asn_rb_mapping_info_list,
 rb_mapping_info_list_t      *umts_rb_mapping_info_list
 );

extern void decode_srb_information_setup
(
 UMTS_SRB_InformationSetup        *asn_srb_info_setup,
 srb_information_setup_t     *umts_srb_info_setup
 );

extern void fill_srb_information_setup_list
(
 UMTS_SRB_InformationSetupList        *asn_srb_info_setup_list,
 srb_information_setup_list_t    *umts_srb_info_setup_list
 );

extern void decode_lossless_srns_reloc_support
(
 UMTS_LosslessSRNS_RelocSupport       *asn_lossless_srns_reloc_support,
 lossless_srns_reloc_support_t   *umts_lossless_srns_reloc_support
 );

extern void decode_rfc2507_info
(
 UMTS_RFC2507_Info        *asn_rfc2507_info,
 rfc2507_info_t      *umts_rfc2507_info
 );

extern void decode_algorithm_specific_info
(
 UMTS_AlgorithmSpecificInfo       *asn_algorithm_specific_info,
 algorithm_specific_info_t   *umts_algorithm_specific_info
 );

extern void decode_header_compression_info
(
 UMTS_HeaderCompressionInfo       *asn_header_compression_info,
 header_compression_info_t   *umts_header_compression_info
 );

extern void decode_header_compression_info_list
(
 UMTS_HeaderCompressionInfoList           *asn_header_compression_info_list,
 header_compression_info_list_t      *umts_header_compression_info_list
 );

extern void decode_pdcp_info
(
 UMTS_PDCP_Info       *asn_pdcp_info,
 pdcp_info_t     *umts_pdcp_info
 );

extern void decode_rb_information_setup
(
 UMTS_RB_InformationSetup        *asn_rb_info_setup,
 rb_information_setup_t     *umts_rb_info_setup
 );

extern void fill_rb_information_setup_list
(
 UMTS_RB_InformationSetupList    *asn_rb_information_setup_list,
 rb_information_setup_list_t     *umts_rb_information_setup_list
 );

extern void decode_predefined_rb_configuration
(  
 UMTS_PredefinedRB_Configuration      *asn_predefined_rb_configuration,
 predefined_rb_configuration_t   *umts_predefined_rb_configuration
 );

extern void decode_pre_def_radio_configuration
(  
 UMTS_PreDefRadioConfiguration        *asn_pre_def_radio_configuration,
 pre_def_radio_configuration_t   *umts_pre_def_radio_configuration
 );

extern void convert_plmn_intra_freq_list
(
 UMTS_PLMNsOfIntraFreqCellsList              *asn_plmn_intra_freq_list,
 plmns_of_intra_freq_cells_list_t       *plmn_intra_freq_list
 );

extern void convert_plmn_inter_freq_list
(
 UMTS_PLMNsOfInterFreqCellsList              *asn_plmn_inter_freq_list,
 plmns_of_inter_freq_cells_list_t       *plmn_inter_freq_list
 );

extern void convert_plmn_inter_rat_list
(
 UMTS_PLMNsOfInterRATCellsList              *asn_plmn_inter_rat_list,
 plmns_of_inter_rat_cells_list_t       *plmn_inter_rat_list
 );

extern void convert_cn_domain_sysinfo_list
(
 UMTS_CN_DomainSysInfoList               *asn_DomainSysInfoList,
 cn_domainsys_info_list_t           *cn_domainsys_info_list
 );
extern void convert_mappingFunctionParameterList
(
 UMTS_MappingFunctionParameterList           *pvalue,
 mapping_function_parameter_list_t      *p_mappingFunction_Parameter_list
 );

extern void fill_sysinfotype1_v3a0_nce
(
 UMTS_SysInfoType1_v3a0NonCriticalExtensions  *v3a0NonCriticalExtensions,
 sysinfotype1_v3a0_nce_t                 *sysinfotype1_v3a0_nce
 );

extern return_code umts_convert_sib16
(
 OSCTXT                 *pctxt,
 UMTS_CompleteSIB *pcomplete_sib_segment,
 umts_sib_type_16_info_t  *umts_sib16
 );

extern return_code umts_convert_sib1
(
 umts_sib_type_1_info_t  *umts_sib1
 );

extern return_code umts_convert_sib2
(
 umts_sib_type_2_info_t  *umts_sib_type_2_info
 );


extern void convert_dynamic_persistencelevel_list
(
 UMTS_DynamicPersistenceLevelList            *p_asn_list,
 dynamic_persistencelevel_list_t        *p_dynamic_persistencelevel_list
 );

extern return_code umts_convert_sib7
(
 umts_sib_type_7_info_t  *umts_sib7
 );

extern return_code    fill_sysinfotype18_v3a0_nce
(
 UMTS_SysInfoType18_v6b0NonCriticalExtensions *v6b0NonCriticalExtensions,
 sysinfotype18_v6b0_nce_t                *sysinfotype18_v6b0_nce
 );

extern void convert_plmn_identities_v860ext
(
 UMTS_PLMNIdentitiesOfNeighbourCells_v860ext         *asn_plmn_id_v860,
 plmn_identities_of_neighbour_cells_v860ext_t   *plmn_id_v860
 );

extern void convert_multiple_plmn_intra_freq_list
(
 UMTS_MultiplePLMNsOfIntraFreqCellsList          *asn_multiplePLMNSIntraFreqList,
 multiple_plmns_of_intra_freq_cells_list_t  *multiple_plmns_list
 );

extern void convert_multiple_plmn_inter_freq_list
(
 UMTS_MultiplePLMNsOfInterFreqCellsList          *asn_multiplePLMNSInterFreqList,
 multiple_plmns_of_inter_freq_cells_list_t  *multiple_plmns_list
 );

extern void convert_multiple_plmn_intra_list_element
(
 UMTS_MultiplePLMNsOfIntraFreqCellsList_element_multiplePLMN_list            *asn_multiplePLMNSIntraFreqList,
 multiple_plmns_of_intra_freq_cells_list_element_multiple_plmn_list_t   *multiple_plmns_list
 );

extern void convert_multiple_plmn_inter_list_element
(
 UMTS_MultiplePLMNsOfInterFreqCellsList_element_multiplePLMN_list            *asn_multiplePLMNSInterFreqList,
 multiple_plmns_of_inter_freq_cells_list_element_multiple_plmn_list_t   *multiple_plmns_list
 );

extern return_code umts_convert_sib18
(
 umts_sib_type_18_info_t  *umts_sib18
 );

extern return_code umts_check_and_decode_sibs
(
 OSCTXT                  *pctxt,
 UMTS_BCCH_BCH_Message        *asn_decoded_sibs,
 umts_sib_message_t      *decoded_buf
 );

extern U8 umts_sib_decoder
(
 U8                 *encoded_sib_msg,
 umts_sib_message_t *decoded_sibs_msg,
 U16                encoded_sib_len
 );

extern void convert_mappingInfo_list
(
 UMTS_MappingInfo                        *pvalue,
 mapping_info_t                     *p_mapping_Info_list
 );

extern void convert_rat_fdd_info_list
(
 UMTS_RAT_FDD_InfoList                       *pvalue,
 rat_fdd_info_list_t                    *p_rat_fdd_info_list
 );

extern void convert_rat_tdd_info_list
(
 UMTS_RAT_TDD_InfoList                       *pvalue,
 rat_tdd_info_list_t                    *p_rat_fdd_info_list
 );

extern return_code umts_convert_sib3
(
 umts_sib_type_3_info_t  *umts_sib_type_3_info
 );

extern void convert_secondary_ccpch_info
(
 UMTS_SecondaryCCPCH_Info        *pvalue,
 secondary_ccpch_info_t     *secondary_ccpch_info
 );

extern void convert_sccpch_system_information_list
(
 UMTS_SCCPCH_SystemInformationList       *pvalue,
 sccpch_system_information_list_t   *sccpch_system_info_list
 );

extern void convert_number_of_tb_size_n_tti_list
(
 UMTS_NumberOfTbSizeAndTTIList            *pvalue,
 number_of_tb_size_n_tti_list_t      *numOf_tb_sizeAnd_tti_list
 );

extern void convert_common_dynamic_tf_info_list_dynamic_tti
(
 UMTS_CommonDynamicTF_InfoList_DynamicTTI        *pvalue,
 common_dynamic_tf_info_list_dynamic_tti_t  *tf_info_list_dynamic_tti
 );

extern void convert_logical_channel_list_explicitList
(
 UMTS_LogicalChannelList_explicitList        *pvalue,
 logical_channel_list_explicitList_t    *logical_channel_list_explicit_List
 );

extern void convert_dedicated_dynamic_tf_info_list_dynamic_tti
(
 UMTS_DedicatedDynamicTF_InfoList_DynamicTTI         *pvalue,
 dedicated_dynamic_tf_info_list_dynamic_tti_t   *dedicated_dynamic_tti_list
 );

extern void convert_dedicated_dynamic_tf_info_num_of_tb_size_list
(
 UMTS_DedicatedDynamicTF_Info_numberOfTbSizeList             *pvalue,
 dedicated_dynamic_tf_info_num_of_tb_size_list_t        *dedicated_dynamic_tf_info_num_of_tb_size_list
 );

extern void convert_dedicated_dynamic_tf_info_list
(
 UMTS_DedicatedDynamicTF_InfoList        *pvalue,
 dedicated_dynamic_tf_info_list_t   *dedicated_dynamic_tf_info_list
 );

extern void convert_dedicated_trans_ch_tfs_tti
(
 UMTS_DedicatedTransChTFS_tti            *pvalue,
 dedicated_trans_ch_tfs_tti_t       *dedicated_trans_ch_tfs_tti
 );

extern void convert_common_dynamic_tf_info_num_of_tb_size_list
(
 UMTS_CommonDynamicTF_Info_numberOfTbSizeList        *pvalue,
 common_dynamic_tf_info_num_of_tb_size_list_t   *num_of_tb_size_list
 );

extern void convert_common_dynamic_tf_info_list
(
 UMTS_CommonDynamicTF_InfoList       *pvalue,
 common_dynamic_tf_info_list_t  *common_dynamic_tf_info_list
 );

extern void convert_common_trans_ch_tfs
(
 UMTS_CommonTransChTFS      *pvalue,
 common_trans_ch_tfs_t      *p_common_trans_ch_tfs
 );

extern void convert_transport_format_set
(
 UMTS_TransportFormatSet         *pvalue,
 transport_format_set_t     *transport_format_set
 );

extern void convert_prach_rach_info
(
 UMTS_PRACH_RACH_Info        *pvalue,
 prach_rach_info_t      *p_prach_rach_info
 );

extern void convert_prach_partitioning_fdd
(
 UMTS_PRACH_Partitioning_fdd     *pvalue,
 prach_partitioning_fdd_t   *prach_partition_fdd
 );

extern void convert_prach_partitioning
(
 UMTS_PRACH_Partitioning         *pvalue,
 prach_partitioning_t       *prach_partition
 );

extern void convert_prach_system_information_modespecific_info
(
 UMTS_PRACH_SystemInformation_modeSpecificInfo       *pvalue,
 prach_system_information_modespecific_info_t   *prach_modeSpecificInfo
 );

extern void convert_prach_system_information
(
 UMTS_PRACH_SystemInformation        *pvalue,
 prach_system_information_t     *prach_system_information
 );

extern void convert_prach_system_information_list
(
 UMTS_PRACH_SystemInformationList        *pvalue,
 prach_system_information_list_t    *p_prach_system_information_list
 );

extern void convert_pusch_sysinfo
(
 UMTS_PUSCH_SysInfo      *pvalue,
 pusch_sysinfo_t    *p_pusch_sysinfo
 );

extern void convert_pusch_SysInfoList_SFN_element
(
 UMTS_PUSCH_SysInfoList_SFN_element      *pvalue,
 pusch_SysInfoList_SFN_element_t    *p_pusch_sysinfo_list_sfn_element
 );

extern void convert_pusch_sysinfo_list_sfn
(
 UMTS_PUSCH_SysInfoList_SFN              *pvalue,
 pusch_sysinfo_list_sfn_t           *p_pusch_sysinfo_list_sfn
 );

extern return_code umts_convert_sib5
(
 umts_sib_type_5_info_t *umts_sib_type_5_info
 );

extern void convert_plmn_identities_of_neighbour_cells_v860ext
(
 UMTS_PLMNIdentitiesOfNeighbourCells_v860ext    *pvalue,
 plmn_identities_of_neighbour_cells_v860ext_t   *p_plmn_identities_of_neighbour_cells_v860ext
 );

extern void convert_multiple_plmns_of_intra_freq_cells_list
(
 UMTS_MultiplePLMNsOfIntraFreqCellsList     *asn_multiplePLMNSIntraFreqList,
 multiple_plmns_of_intra_freq_cells_list_t  *multiple_plmns_list
 );

extern void convert_multiple_plmns_of_inter_freq_cells_list
(
 UMTS_MultiplePLMNsOfInterFreqCellsList     *p_asn_multiplePLMNSInterFreqList,
 multiple_plmns_of_inter_freq_cells_list_t  *p_multiple_plmns_list
 );

void convert_multiple_plmns_of_intra_freq_cells_list_element_multiple_plmn_list
(
 UMTS_MultiplePLMNsOfIntraFreqCellsList_element_multiplePLMN_list       *p_asn_multiplePLMNSIntraFreqList,
 multiple_plmns_of_intra_freq_cells_list_element_multiple_plmn_list_t   *p_multiple_plmns_list
 );

void convert_multiple_plmns_of_inter_freq_cells_list_element_multiple_plmn_list
(
 UMTS_MultiplePLMNsOfInterFreqCellsList_element_multiplePLMN_list       *p_asn_multiplePLMNSInterFreqList,
 multiple_plmns_of_inter_freq_cells_list_element_multiple_plmn_list_t   *p_multiple_plmns_list
 );

void convert_plmn_identities_of_neighbour_cells
(
 UMTS_PLMNIdentitiesOfNeighbourCells    *pvalue,
 plmn_identities_of_neighbour_cells_t   *p_plmn_identities_of_neighbour_cells
 );

void convert_ext_sib_type_info_scheduling_info_list
(
 UMTS_ExtSIBTypeInfoSchedulingInfo_List     *pvalue,
 ext_sib_type_info_scheduling_info_list_t   *p_ext_sib_type_info_scheduling_info_list
 );

void convert_ext_sib_type_info_scheduling_info_list2
(
 UMTS_ExtSIBTypeInfoSchedulingInfo_List2    *pvalue,
 ext_sib_type_info_scheduling_info_list2_t  *p_list2
 );

void convert_ext_ganss_sib_type_info_scheduling_info_list
(
 UMTS_ExtGANSS_SIBTypeInfoSchedulingInfoList    *pvalue,
 ext_ganss_sib_type_info_scheduling_info_list_t *p_scheduling_info_list
 );

void convert_measurement_control_sys_info_use_of_hcs_hcs_not_used
(
 UMTS_MeasurementControlSysInfo_use_of_HCS_hcs_not_used *pvalue,
 measurement_control_sys_info_use_of_hcs_hcs_not_used_t *p_hcs_not_used
 );

void convert_measurement_control_sys_info_use_of_hcs_hcs_used
(
 UMTS_MeasurementControlSysInfo_use_of_HCS_hcs_used      *pvalue,
 measurement_control_sys_info_use_of_hcs_hcs_used_t      *p_hcs_used
 );

void convert_primary_ccpch_info_lcr_r4
(
 UMTS_PrimaryCCPCH_Info_LCR_r4  *pvalue,
 primary_ccpch_info_lcr_r4_t    *p_primary_ccpch_info_lcr_r4
 );

void convert_new_intra_freq_cell_si_list_rscp_lcr_r4
(
 UMTS_NewIntraFreqCellSI_List_RSCP_LCR_r4   *pvalue,
 new_intra_freq_cell_si_list_rscp_lcr_r4_t  *p_lcr_r4
 );

void convert_new_inter_freq_cell_si_list_rscp_lcr_r4
(
 UMTS_NewInterFreqCellSI_List_RSCP_LCR_r4   *pvalue,
 new_inter_freq_cell_si_list_rscp_lcr_r4_t  *p_lcr_r4
 );

void convert_new_intra_freq_cell_si_list_ecno_lcr_r4
(
 UMTS_NewIntraFreqCellSI_List_ECN0_LCR_r4   *pvalue,
 new_intra_freq_cell_si_list_ecno_lcr_r4_t  *p_lcr_r4
 );

void convert_cell_info_si_hcs_ecno_lcr_r4
(
 UMTS_CellInfoSI_HCS_ECN0_LCR_r4    *pvalue,
 cell_info_si_hcs_ecno_lcr_r4_t     *p_lcr_r4 
 );

void convert_new_inter_freq_cell_si_list_ecno_lcr_r4
(
 UMTS_NewInterFreqCellSI_List_ECN0_LCR_r4   *pvalue,
 new_inter_freq_cell_si_list_ecno_lcr_r4_t  *p_lcr_r4
 );

void convert_new_intra_freq_cell_si_list_hcs_rscp_lcr_r4
(
 UMTS_NewIntraFreqCellSI_List_HCS_RSCP_LCR_r4   *pvalue,
 new_intra_freq_cell_si_list_hcs_rscp_lcr_r4_t  *p_lcr_r4
 );

void convert_new_inter_freq_cell_si_list_hcs_rscp_lcr_r4
(
 UMTS_NewInterFreqCellSI_List_HCS_RSCP_LCR_r4   *pvalue,
 new_inter_freq_cell_si_list_hcs_rscp_lcr_r4_t  *p_lcr_r4
 );

void convert_new_intra_freq_cell_si_list_hcs_ecno_lcr_r4
(
 UMTS_NewIntraFreqCellSI_List_HCS_ECN0_LCR_r4   *pvalue,
 new_intra_freq_cell_si_list_hcs_ecno_lcr_r4_t  *p_ecno_lcr_r4
 );

void convert_new_inter_freq_cell_si_list_hcs_ecno_lcr_r4
(
 UMTS_NewInterFreqCellSI_List_HCS_ECN0_LCR_r4   *pvalue,
 new_inter_freq_cell_si_list_hcs_ecno_lcr_r4_t  *p_ecno_lcr_r4
 );

void convert_removed_intra_freq_cell_list
(
 UMTS_RemovedIntraFreqCellList  *pvalue,
 removed_intra_freq_cell_list_t *p_removed_intra_freq_cell_list
 );

void convert_intra_freq_meas_quantity
(
 UMTS_IntraFreqMeasQuantity *pvalue,
 intra_freq_meas_quantity_t *p_intra_freq_meas_quantity
 );

void convert_intra_freq_reporting_quantity_for_rach
(
 UMTS_IntraFreqReportingQuantityForRACH     *pvalue,
 intra_freq_reporting_quantity_for_rach_t   *p_intra_freq_reporting_quantity_for_rach
 );

void convert_intra_freq_reporting_quantity
(
 UMTS_IntraFreqReportingQuantity *pvalue,
 intra_freq_reporting_quantity_t *p_intra_freq_reporting_quantity
 );

void convert_event_1c
(
 UMTS_Event1c   *pvalue,
 event_1c_t     *p_event_1c
 );

void convert_event_1f
(
 UMTS_Event1f   *pvalue,
 event_1f_t     *p_event_1f
 );

void convert_reporting_cell_status
(
 UMTS_ReportingCellStatus   *pvalue,
 reporting_cell_status_t    *p_reporting_cell_status
 );

void convert_removed_inter_freq_cell_list
(
 UMTS_RemovedInterFreqCellList       *pvalue,
 removed_inter_freq_cell_list_t      *p_removed_inter_freq_cell_list
 );

void convert_event_1e
(
 UMTS_Event1e   *pvalue,
 event_1e_t     *p_event_1e
 );

void convert_individual_timeslot_info_lcr_r4
(
 UMTS_IndividualTimeslotInfo_LCR_r4      *pvalue,
 individual_timeslot_info_lcr_r4_t       *p_lcr_r4
 );

void convert_downlink_timeslots_codes_lcr_r4
(
 UMTS_DownlinkTimeslotsCodes_LCR_r4     *pvalue,
 downlink_timeslots_codes_lcr_r4_t      *p_info
 );

void convert_mbms_fach_carrying_mtch_list
(
 UMTS_MBMS_FACHCarryingMTCH_List     *pvalue,
 mbms_fach_carrying_mtch_list_t      *p_fachCarryingMTCH_List
 );

void convert_usch_transport_channels_info
(
 UMTS_USCH_TransportChannelsInfo    *pvalue,
 usch_transport_channels_info_t     *p_info
 );

void convert_dsch_transport_channels_info
(
 UMTS_DSCH_TransportChannelsInfo    *pvalue,
 dsch_transport_channels_info_t     *p_info
 );

void convert_sysinfotype5_v4b0nces_v590nces_v650nces
(
 UMTS_SysInfoType5_v4b0NonCriticalExtensions_v590NonCriticalExtensions_v650NonCriticalExtensions    *pvalue,
 sysinfotype5_v4b0nces_v590nces_v650nces_t      *p_nce
 );

void convert_removeSomeIntraFreqCells
(
 UMTS_RemovedIntraFreqCellList_removeSomeIntraFreqCells *removeSomeIntraFreqCells,
 removedintrafreqcelllist_removesomeintrafreqcells_t *remove_someIntra_freq_cells
 );

void convert_RemovedIntraFreqCellList
(
 UMTS_RemovedIntraFreqCellList *removedIntraFreqCellList,
 removedintrafreqcelllist_t *removed_intra_freq_cell_list
 );

void convert_ReferenceTimeDifferenceToCell
(
 UMTS_ReferenceTimeDifferenceToCell *referenceTimeDifferenceToCell,
 referencetimedifferencetocell_t *referencetime_differencetocell
 );

void convert_PrimaryCCPCH_Info_LCR_r4
(
 UMTS_PrimaryCCPCH_Info_LCR_r4 *primaryCCPCH_Info,
 primaryccpch_info_lcr_r4_t *primaryccpch_info
 );

void convert_TimeslotInfo_LCR_r4
(
 UMTS_TimeslotInfo_LCR_r4  *TimeslotInfo_LCR_r4,
 timeslotinfo_lcr_r4_t *timeslotinfo_lcr_r4
 );

void convert_TimeslotInfoList_LCR_r4
(
 UMTS_TimeslotInfoList_LCR_r4 *timeslotInfoList,
 timeslotinfolist_lcr_r4_t *timeslotinfo_list
 );

void convert_CellSelectReselectInfoSIB_11_12_RSCP_modeSpecificInfo_fdd
(
 UMTS_CellSelectReselectInfoSIB_11_12_RSCP_modeSpecificInfo_fdd *asn_fdd,
 cellselectreselectinfosib_11_12_rscp_modespecificinfo_fdd_t *umts_fdd
 );

void convert_CellSelectReselectInfoSIB_11_12_RSCP_modeSpecificInfo_tdd
(
 UMTS_CellSelectReselectInfoSIB_11_12_RSCP_modeSpecificInfo_tdd *asn_tdd,
 cellselectreselectinfosib_11_12_rscp_modespecificinfo_tdd_t *umts_tdd
 );

void convert_CellSelectReselectInfoSIB_11_12_RSCP_modeSpecificInfo_gsm
(
 UMTS_CellSelectReselectInfoSIB_11_12_RSCP_modeSpecificInfo_gsm *asn_gsm,
 cellselectreselectinfosib_11_12_rscp_modespecificinfo_gsm_t *umts_gsm
 );

void convert_CellSelectReselectInfoSIB_11_12_HCS_RSCP_modeSpecificInfo
(
 UMTS_CellSelectReselectInfoSIB_11_12_RSCP_modeSpecificInfo *modeSpecificInfo,
 cellselectreselectinfosib_11_12_rscp_modespecificinfo_t *mode_specific_info
 );

void convert_CellSelectReselectInfoSIB_11_12_RSCP
(
 UMTS_CellSelectReselectInfoSIB_11_12_RSCP *cellSelectionReselectionInfo,
 cellselectreselectinfosib_11_12_rscp_t *cellselection_reselection_info
 );

void convert_CellInfoSI_RSCP_LCR_r4
(
 UMTS_CellInfoSI_RSCP_LCR_r4 *cellInfo,
 cellinfosi_rscp_lcr_r4_t *umts_cell_info
 );

void convert_NewIntraFreqCellSI_RSCP_LCR_r4
(
 UMTS_NewIntraFreqCellSI_RSCP_LCR_r4 *NewIntraFreqCellSI_HCS_RSCP_LCR_r4,
 newintrafreqcellsi_rscp_lcr_r4_t *newintrafreqcellsi_rscp_lcr_r4
 );

void convert_NewIntraFreqCellSI_List_RSCP_LCR_r4
(
 UMTS_NewIntraFreqCellSI_List_RSCP_LCR_r4 *newIntraFreqCellList,
 newintrafreqcellsi_list_rscp_lcr_r4_t	*newintra_freq_cell_list
 );

void convert_IntraFreqCellInfoSI_List_RSCP_LCR_r4
(
 UMTS_IntraFreqCellInfoSI_List_RSCP_LCR_r4 *intraFreqCellInfoSI_List,
 intrafreqcellinfosi_list_rscp_lcr_r4_t	*intra_freq_cellinfosi_list
 );

void convert_IntraFreqMeasQuantity_modeSpecificInfo_fdd
(
 UMTS_IntraFreqMeasQuantity_modeSpecificInfo_fdd *asn_fdd,
 intrafreqmeasquantity_modespecificinfo_fdd_t *umts_fdd
 );

void convert_intraFreqMeasQuantity_TDDList
(
 UMTS_IntraFreqMeasQuantity_TDDList *intraFreqMeasQuantity_TDDList,
 intrafreqmeasquantity_tddlist_t  *intra_freq_meas_quantity_tddlist
 );

void convert_IntraFreqMeasQuantity_modeSpecificInfo_tdd
(
 UMTS_IntraFreqMeasQuantity_modeSpecificInfo_tdd *asn_tdd,
 intrafreqmeasquantity_modespecificinfo_tdd_t *umts_tdd
 );

void convert_IntraFreqMeasQuantity_modeSpecificInfo
(
 UMTS_IntraFreqMeasQuantity_modeSpecificInfo  *modeSpecificInfo,
 intrafreqmeasquantity_modespecificinfo_t  *mode_specific_info
 );

void convert_IntraFreqMeasQuantity
(
 UMTS_IntraFreqMeasQuantity *intraFreqMeasQuantity,
 intrafreqmeasquantity_t *intra_freq_meas_quantity
 );

void convert_IntraFreqReportingQuantityForRACH_modeSpecificInfo_fdd
(
 UMTS_IntraFreqReportingQuantityForRACH_modeSpecificInfo_fdd *asn_fdd,
 intrafreqreportingquantityforrach_modespecificinfo_fdd_t *umts_fdd
 );

void convert_IntraFreqReportingQuantityForRACH_modeSpecificInfo_tdd
(
 UMTS_IntraFreqReportingQuantityForRACH_modeSpecificInfo_tdd *asn_tdd,
 intrafreqreportingquantityforrach_modespecificinfo_tdd_t *umts_tdd
 );

void convert_IntraFreqReportingQuantityForRACH_modeSpecificInfo
(
 UMTS_IntraFreqReportingQuantityForRACH_modeSpecificInfo *modeSpecificInfo,
 intrafreqreportingquantityforrach_modeSpecificInfo_t	*mode_specific_info
 );

void convert_IntraFreqReportingQuantityForRACH
(
 UMTS_IntraFreqReportingQuantityForRACH  *intraFreqReportingQuantityForRACH,
 intrafreqreportingquantityforrach_t	*intra_freq_reporting_quantity_for_rach
 );

void convert_CellReportingQuantities_modeSpecificInfo_fdd
(
 UMTS_CellReportingQuantities_modeSpecificInfo_fdd *asn_fdd,
 cellreportingquantities_modespecificInfo_fdd_t *umts_fdd
 );

void convert_CellReportingQuantities_modeSpecificInfo_tdd
(
 UMTS_CellReportingQuantities_modeSpecificInfo_tdd *asn_tdd,
 cellreportingquantities_modespecificInfo_tdd_t *umts_tdd
 );

void convert_CellReportingQuantities_modeSpecificInfo
(
 UMTS_CellReportingQuantities_modeSpecificInfo *modeSpecificInfo,
 cellreportingquantities_modespecificinfo_t  *mode_specific_info
 );

void convert_detectedSetReportingQuantities
(
 UMTS_CellReportingQuantities *detectedSetReportingQuantities,
 cellreportingquantities_t  *detected_set_reporting_quantities
 );

void convert_intraFreqReportingQuantity
(
 UMTS_IntraFreqReportingQuantity *intraFreqReportingQuantity,
 intrafreqreportingquantity_t	*intra_freq_reporting_quantity
 );

void convert_measurementReportingMode
(
 UMTS_MeasurementReportingMode *measurementReportingMode,
 measurementreportingmode_t  *measurement_reporting_mode
 );

void convert_ForbiddenAffectCell_LCR_r4
(
 UMTS_ForbiddenAffectCell_LCR_r4 *ForbiddenAffectCell_LCR_r4,
 forbiddenaffectcell_lcr_r4_t  *forbiddenaffectcell_lcr_r4
 );

void convert_ForbiddenAffectCellList_LCR_r4
(
 UMTS_ForbiddenAffectCellList_LCR_r4 *forbiddenAffectCellList,
 forbiddenaffectcelllist_lcr_r4_t *forbidden_affect_cell_list
 );

void convert_Event1a_LCR_r4
(
 UMTS_Event1a_LCR_r4 *asn_e1a,
 event1a_lcr_r4_t *umts_e1a
 );

void convert_Event1b_LCR_r4
(
 UMTS_Event1b_LCR_r4 *asn_e1b,
 event1b_lcr_r4_t *umts_e1b
 );

void convert_Event1c
(
 UMTS_Event1c *asn_e1c,
 event1c_t *umts_e1c
 );

void convert_Event1e
(
 UMTS_Event1e *asn_e1e,
 event1e_t *umts_e1e
 );

void convert_Event1f
(
 UMTS_Event1f *asn_e1f,
 event1f_t *umts_e1f
 ); 

void convert_IntraFreqEvent_LCR_r4
(
 UMTS_IntraFreqEvent_LCR_r4 *asn_event, 
 intrafreqevent_lcr_r4_t *umts_event
 );

void convert_ReportingCellStatus
(
 UMTS_ReportingCellStatus *reportingCellStatus,
 inreportingcellstatus_t *reporting_cell_status
 );

void convert_IntraFreqEventCriteria_LCR_r4
(
 UMTS_IntraFreqEventCriteria_LCR_r4 *IntraFreqEventCriteria_LCR_r4,
 intrafreqeventcriteria_lcr_r4_t *intrafreqeventcriteria_lcr_r4
 );

void convert_IntraFreqEventCriteriaList_LCR_r4
(
 UMTS_IntraFreqEventCriteriaList_LCR_r4 *eventCriteriaList,
 intrafreqeventcriterialist_lcr_r4_t *event_criteria_list
 );

void convert_intraFreqReportingCriteria
(
 UMTS_IntraFreqReportingCriteria_LCR_r4 *intraFreqReportingCriteria,
 intrafreqreportingcriteria_lcr_r4_t	*intra_freq_reporting_criteria
 );

void convert_periodicalReportingCriteria
(
 UMTS_PeriodicalReportingCriteria *periodicalReportingCriteria,
 periodicalreportingcriteria_t		*periodical_reporting_criteria
 );

void convert_CellDCH_ReportCriteria_LCR_r4
(
 UMTS_CellDCH_ReportCriteria_LCR_r4 *reportCriteria,
 celldch_reportcriteria_lcr_r4_t	*report_criteria
 );

void convert_ReportingInfoForCellDCH_LCR_r4
(
 UMTS_ReportingInfoForCellDCH_LCR_r4 *reportingInfoForCellDCH,
 reportinginfoforcelldch_lcr_r4_t   *reporting_info_for_celldch
 );

void convert_intraFreqMeasurementSysInfo
(
 UMTS_IntraFreqMeasurementSysInfo_RSCP_LCR_r4 *intraFreqMeasurementSysInfo,
 intrafreqmeasurementsysinfo_rscp_lcr_r4_t *intra_freq_measurement_sysInfo
 );

void convert_RemovedInterFreqCellList_removeSomeInterFreqCells
(
 UMTS_RemovedInterFreqCellList_removeSomeInterFreqCells *removeSomeInterFreqCells,
 removedinterfreqcelllist_removesomeinterfreqcells_t *remove_some_inter_freqCells
 );

void convert_RemovedInterFreqCellList
(
 UMTS_RemovedInterFreqCellList *removedInterFreqCellList,
 removedinterfreqcelllist_t *removed_inter_freq_cell_list
 );

void convert_FrequencyInfoFDD
( 
 UMTS_FrequencyInfoFDD *asn_fdd,
 frequency_info_fdd_t *umts_fdd
 );

void convert_FrequencyInfo_modeSpecificInfo 
(
 UMTS_FrequencyInfo_modeSpecificInfo *modeSpecificInfo,
 frequency_info_mode_specific_info_t  *mode_specific_info
 );

void convert_FrequencyInfo 
( 
 UMTS_FrequencyInfo *frequencyInfo,
 frequency_info_t *frequency_info
 );

void convert_NewInterFreqCellSI_RSCP_LCR_r4
(
 UMTS_NewInterFreqCellSI_RSCP_LCR_r4 *NewInterFreqCellSI_RSCP_LCR_r4,
 newinterfreqcellsi_rscp_lcr_r4_t *newinterfreqcellsi_rscp_lcr_r4
 );

void convert_NewInterFreqCellSI_List_RSCP_LCR_r4
(
 UMTS_NewInterFreqCellSI_List_RSCP_LCR_r4 *newInterFreqCellList,
 newinterfreqcellsi_list_rscp_lcr_r4_t *new_inter_freq_cell_list
 );

void convert_InterFreqCellInfoSI_List_RSCP_LCR
(
 UMTS_InterFreqCellInfoSI_List_RSCP_LCR *interFreqCellInfoSI_List,
 interfreqcellinfosi_list_rscp_lcr_t *inter_freq_cell_info_si_list
 );

void convert_InterFreqMeasurementSysInfo_RSCP_LCR_r4
(
 UMTS_InterFreqMeasurementSysInfo_RSCP_LCR_r4 *interFreqMeasurementSysInfo,
 interfreqmeasurementsysinfo_rscp_lcr_r4_t *inter_freq_measurement_sysInfo
 );

void convert_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_not_used_cellSelectQualityMeasure_cpich_RSCP
(
 UMTS_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_not_used_cellSelectQualityMeasure_cpich_RSCP *cpich_RSCP,
 measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_not_used_cellselectqualitymeasure_cpich_rscp_t	*cpich_rscp
 );

void convert_CellSelectReselectInfoSIB_11_12_ECN0_modeSpecificInfo_fdd
(
 UMTS_CellSelectReselectInfoSIB_11_12_ECN0_modeSpecificInfo_fdd *asn_fdd,
 cellselectreselectinfosib_11_12_ecn0_modespecificinfo_fdd_t *umts_fdd
 );

void convert_CellSelectReselectInfoSIB_11_12_ECN0_modeSpecificInfo_tdd
(
 UMTS_CellSelectReselectInfoSIB_11_12_ECN0_modeSpecificInfo_tdd *asn_tdd,
 cellselectreselectinfosib_11_12_ecn0_modespecificinfo_tdd_t *umts_tdd
 );

void convert_CellSelectReselectInfoSIB_11_12_ECN0_modeSpecificInfo_gsm
(
 UMTS_CellSelectReselectInfoSIB_11_12_ECN0_modeSpecificInfo_gsm *asn_gsm,
 cellselectreselectinfosib_11_12_ecn0_modespecificinfo_gsm_t *umts_gsm
 );

void convert_CellSelectReselectInfoSIB_11_12_ECN0_modeSpecificInfo
(
 UMTS_CellSelectReselectInfoSIB_11_12_ECN0_modeSpecificInfo *modeSpecificInfo,
 cellselectreselectinfosib_11_12_ecn0_modespecificinfo_t *mode_specific_info
 );

void convert_CellSelectReselectInfoSIB_11_12_ECN0
(
 UMTS_CellSelectReselectInfoSIB_11_12_ECN0 *cellSelectionReselectionInfo,
 cellselectreselectinfosib_11_12_ecn0_t *cell_selection_reselection_info
 );

void convert_NewIntraFreqCellSI_ECN0_LCR_r4
(
 UMTS_NewIntraFreqCellSI_ECN0_LCR_r4 *NewIntraFreqCellSI_ECN0_LCR_r4,
 newintrafreqcellsi_ecn0_lcr_r4_t *newintrafreqcellsi_ecn0_lcr_r4
 ); 

void convert_NewIntraFreqCellSI_List_ECN0_LCR_r4
(
 UMTS_NewIntraFreqCellSI_List_ECN0_LCR_r4 *newIntraFreqCellList,
 newintrafreqcellsi_list_ecn0_lcr_r4_t *new_intra_freq_cell_list
 );

void convert_IntraFreqCellInfoSI_List_ECN0_LCR_r4
(
 UMTS_IntraFreqCellInfoSI_List_ECN0_LCR_r4 *intraFreqCellInfoSI_List,
 intrafreqcellinfosi_list_ecn0_lcr_r4_t	*intra_freq_cell_info_si_list
 );

void convert_IntraFreqMeasurementSysInfo_ECN0_LCR_r4
(
 UMTS_IntraFreqMeasurementSysInfo_ECN0_LCR_r4 *intraFreqMeasurementSysInfo,
 intrafreqmeasurementsysinfo_ecn0_lcr_r4_t  *intra_freq_measurement_sys_info
 );

void convert_CellInfoSI_ECN0_LCR_r4
(
 UMTS_CellInfoSI_ECN0_LCR_r4 *cellInfo,
 cellinfosi_ecn0_lcr_r4_t *cell_info
 );

void convert_NewInterFreqCellSI_ECN0_LCR_r4
(
 UMTS_NewInterFreqCellSI_ECN0_LCR_r4 *NewInterFreqCellSI_ECN0_LCR_r4,
 newinterfreqcellsi_ecn0_lcr_r4_t *newinterfreqcellsi_ecn0_lcr_r4
 );

void convert_NewInterFreqCellSI_List_ECN0_LCR_r4
(
 UMTS_NewInterFreqCellSI_List_ECN0_LCR_r4 *newInterFreqCellList,
 newinterfreqcellsi_list_ecn0_lcr_r4_t	*new_inter_freq_cell_list
 );

void convert_InterFreqCellInfoSI_List_ECN0_LCR
(
 UMTS_InterFreqCellInfoSI_List_ECN0_LCR *interFreqCellInfoSI_List,
 interfreqcellinfosi_list_ecn0_lcr_t *inter_freq_cell_info_si_list
 );

void convert_InterFreqMeasurementSysInfo_ECN0_LCR_r4
(
 UMTS_InterFreqMeasurementSysInfo_ECN0_LCR_r4 *interFreqMeasurementSysInfo,
 interfreqmeasurementsysinfo_ecn0_lcr_r4_t  *inter_freq_measurement_sys_info
 );

void convert_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_not_used_cellSelectQualityMeasure_cpich_Ec_N0
(
 UMTS_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_not_used_cellSelectQualityMeasure_cpich_Ec_N0 *cpich_Ec_N0,
 measurementcontrolsysInfo_lcr_r4_ext_use_of_hcs_hcs_not_used_cellselectqualitymeasure_cpich_ec_n0_t	*cpich_ec_n0
 );

void convert_PenaltyTime_RSCP(
        UMTS_PenaltyTime_RSCP *penaltyTime,
        penaltytime_rscp_t *penalty_time);

void convert_HCS_CellReselectInformation_RSCP(
        UMTS_HCS_CellReselectInformation_RSCP *hcs_CellReselectInformation,
        hcs_cellreselectinformation_rscp_t *hcs_cellreselectinformation);

void convert_HCS_NeighbouringCellInformation_RSCP(
        UMTS_HCS_NeighbouringCellInformation_RSCP *hcs_NeighbouringCellInformation_RSCP,
        hcs_neighbouringcellinformation_rscp_t *hcs_neighbouring_cell_information_rscp);

void convert_cell_select_reselect_info_sib_11_12_hcs_rscp_mode_specific_info
(
 UMTS_CellSelectReselectInfoSIB_11_12_HCS_RSCP_modeSpecificInfo     *pvalue,
 cell_select_reselect_info_sib_11_12_hcs_rscp_mode_specific_info_t  *p_mode_specific_info 
 );

void convert_CellSelectReselectInfoSIB_11_12_HCS_RSCP(
        UMTS_CellSelectReselectInfoSIB_11_12_HCS_RSCP *cellSelectionReselectionInfo,
        cellselectreselectinfosib_11_12_hcs_rscp_t *cell_selection_reselection_info);

void convert_CellInfoSI_HCS_RSCP_LCR_r4(
        UMTS_CellInfoSI_HCS_RSCP_LCR_r4 *cellInfo,
        cellinfosi_hcs_rscp_lcr_r4_t *cell_info);

void convert_NewIntraFreqCellSI_HCS_RSCP_LCR_r4(
        UMTS_NewIntraFreqCellSI_HCS_RSCP_LCR_r4 *NewIntraFreqCellSI_HCS_RSCP_LCR_r4,
        newintrafreqcellsi_hcs_rscp_lcr_r4_t *newintrafreqcellsi_hcs_rscp_lcr_r4);

void convert_NewIntraFreqCellSI_List_HCS_RSCP_LCR_r4(
        UMTS_NewIntraFreqCellSI_List_HCS_RSCP_LCR_r4 *newIntraFreqCellList,
        newintrafreqcellsi_list_hcs_rscp_lcr_r4_t *new_intra_freq_cell_list);

void convert_IntraFreqCellInfoSI_List_HCS_RSCP_LCR_r4(
        UMTS_IntraFreqCellInfoSI_List_HCS_RSCP_LCR_r4 *intraFreqCellInfoSI_List,
        intrafreqcellinfosi_list_hcs_rscp_lcr_r4_t   *intra_freq_cell_info_si_list);

void convert_IntraFreqMeasurementSysInfo_HCS_RSCP_LCR_r4(
        UMTS_IntraFreqMeasurementSysInfo_HCS_RSCP_LCR_r4 *intraFreqMeasurementSysInfo,
        intrafreqmeasurementsysinfo_hcs_rscp_lcr_r4_t *intra_freq_measurement_sys_info);

void convert_NewInterFreqCellSI_HCS_RSCP_LCR_r4(
        UMTS_NewInterFreqCellSI_HCS_RSCP_LCR_r4 *NewInterFreqCellSI_HCS_RSCP_LCR_r4,
        newinterfreqcellsi_hcs_rscp_lcr_r4_t *newinterfreqcellsi_hcs_rscp_lcr_r4);

void convert_NewInterFreqCellSI_List_HCS_RSCP_LCR_r4(
        UMTS_NewInterFreqCellSI_List_HCS_RSCP_LCR_r4 *newInterFreqCellList,
        newinterfreqcellsi_list_hcs_rscp_lcr_r4_t *new_inter_freq_cell_list);

void convert_InterFreqCellInfoSI_List_HCS_RSCP_LCR(
        UMTS_InterFreqCellInfoSI_List_HCS_RSCP_LCR *interFreqCellInfoSI_List,
        interfreqcellinfosi_list_hcs_rscp_lcr_t *inter_freq_cell_info_si_list);

void convert_InterFreqMeasurementSysInfo_HCS_RSCP_LCR_r4(
        UMTS_InterFreqMeasurementSysInfo_HCS_RSCP_LCR_r4 *interFreqMeasurementSysInfo,
        interfreqmeasurementsysinfo_hcs_rscp_lcr_r4_t    *inter_freq_measurement_sys_info);

void convert_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_used_cellSelectQualityMeasure_cpich_RSCP(
        UMTS_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_used_cellSelectQualityMeasure_cpich_RSCP *cpich_RSCP,
        measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_used_cellselectqualitymeasure_cpich_rscp_t *cpich_rscp);

void convert_NewIntraFreqCellSI_HCS_ECN0_LCR_r4(
        UMTS_NewIntraFreqCellSI_HCS_ECN0_LCR_r4 *NewIntraFreqCellSI_HCS_ECN0_LCR_r4,
        newintrafreqcellsi_hcs_ecn0_lcr_r4_t *newintrafreqcellsi_hcs_ecno_lcr_r4);

void convert_NewIntraFreqCellSI_List_HCS_ECN0_LCR_r4(
        UMTS_NewIntraFreqCellSI_List_HCS_ECN0_LCR_r4 *newIntraFreqCellList,
        newintrafreqcellsi_list_hcs_ecn0_lcr_r4_t *new_intra_freq_cell_list);

void convert_IntraFreqCellInfoSI_List_HCS_ECN0_LCR_r4(
        UMTS_IntraFreqCellInfoSI_List_HCS_ECN0_LCR_r4 *intraFreqCellInfoSI_List,
        intrafreqcellinfosi_list_hcs_ecn0_lcr_r4_t  *intra_freq_cell_info_si_list);

void convert_IntraFreqMeasurementSysInfo_HCS_ECN0_LCR_r4(
        UMTS_IntraFreqMeasurementSysInfo_HCS_ECN0_LCR_r4 *intraFreqMeasurementSysInfo,
        intrafreqmeasurementsysinfo_hcs_ecn0_lcr_r4_t *intra_freq_measurement_sys_info);

void convert_NewInterFreqCellSI_HCS_ECN0_LCR_r4(
        UMTS_NewInterFreqCellSI_HCS_ECN0_LCR_r4 *NewInterFreqCellSI_HCS_ECN0_LCR_r4,
        newinterfreqcellsi_hcs_ecn0_lcr_r4_t *newinterfreqcellsi_hcs_ecn0_lcr_r4);

void convert_NewInterFreqCellSI_List_HCS_ECN0_LCR_r4(
        UMTS_NewInterFreqCellSI_List_HCS_ECN0_LCR_r4 *newInterFreqCellList,
        newinterfreqcellsi_list_hcs_ecn0_lcr_r4_t *new_inter_freq_cell_list);

void convert_InterFreqCellInfoSI_List_HCS_ECN0_LCR(
        UMTS_InterFreqCellInfoSI_List_HCS_ECN0_LCR *interFreqCellInfoSI_List,
        interfreqcellinfosi_list_hcs_ecn0_lcr_t *inter_freq_cell_info_si_list);

void convert_InterFreqMeasurementSysInfo_HCS_ECN0_LCR_r4(
        UMTS_InterFreqMeasurementSysInfo_HCS_ECN0_LCR_r4 *interFreqMeasurementSysInfo,
        interfreqmeasurementsysinfo_hcs_ecn0_lcr_r4_t *inter_freq_measurement_sys_info);

void convert_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_used_cellSelectQualityMeasure_cpich_Ec_N0(
        UMTS_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_used_cellSelectQualityMeasure_cpich_Ec_N0 *cpich_Ec_N0,
        measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_used_cellselectqualitymeasure_cpich_ec_no_t *cpich_ec_n0);

void convert_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_used_cellSelectQualityMeasure(
        UMTS_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_used_cellSelectQualityMeasure *cellSelectQualityMeasure,
        measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_used_cellselectqualitymeasure_t *cell_select_quality_measure);

void convert_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_not_used_cellSelectQualityMeasure(
        UMTS_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_not_used_cellSelectQualityMeasure *cellSelectQualityMeasure,
        measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_not_used_cellselectqualitymeasure_t *cell_select_quality_measure);

void convert_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_not_used(
        UMTS_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_not_used *asn_hcs_not_used,
        measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_not_used_t	*hcs_not_used);

void convert_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_used(
        UMTS_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS_hcs_used *asn_hcs_used,
        measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_hcs_used_t  *umts_hcs_used);

void conver_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS
(
 UMTS_MeasurementControlSysInfo_LCR_r4_ext_use_of_HCS *use_of_HCS,
 measurementcontrolsysinfo_lcr_r4_ext_use_of_hcs_t    *use_of_hcs
 );

void convert_MeasurementControlSysInfo_LCR_r4(
        UMTS_MeasurementControlSysInfo_LCR_r4_ext *measurementControlSysInfo_LCR,
        measurementcontrolsysinfo_lcr_r4_ext_t	*measurement_control_sysinfo_lcr);
void convert_fach_MeasurementOccasionInfo_LCR_Ext(
        UMTS_FACH_MeasurementOccasionInfo_LCR_r4_ext *fach_MeasurementOccasionInfo_LCR_Ext,
        fach_measurementoccasioninfo_lcr_r4_ext_t    *fach_measurement_occasioninfo_lcr_ext);

void convert_SysInfoType11_v590ext_IEs_newIntraFrequencyCellInfoList_v590ext(
        UMTS_SysInfoType11_v590ext_IEs_newIntraFrequencyCellInfoList_v590ext *newIntraFrequencyCellInfoList_v590ext,
        sysinfotype11_v590ext_ies_newintrafrequencycellinfolist_v590ext_t    *new_intra_frequency_cell_info_list_v590ext);

void convert_SysInfoType11_v590ext_IEs_newInterFrequencyCellInfoList_v590ext(
        UMTS_SysInfoType11_v590ext_IEs_newInterFrequencyCellInfoList_v590ext *newInterFrequencyCellInfoList_v590ext,
        sysinfotype11_v590ext_ies_newinterfrequencycellinfolist_v590ext_t    *new_inter_frequency_cell_info_list_v590ext);

void convert_CellSelectReselectInfo_v590ext(
        UMTS_CellSelectReselectInfo_v590ext *CellSelectReselectInfo_v590ext,
        cellselectreselectinfo_v590ext_t *cellselectreselectinfo_v590ext);

void convert_SysInfoType11_v590ext_IEs_newInterRATCellInfoList_v590ext(
        UMTS_SysInfoType11_v590ext_IEs_newInterRATCellInfoList_v590ext *newInterRATCellInfoList_v590ext,
        sysinfotype11_v590ext_ies_newinterratcellinfolist_v590ext_t    *newinterratcell_info_list_v590ext);

void convert_Intra_FreqEventCriteriaList_v590ext(
        UMTS_Intra_FreqEventCriteriaList_v590ext *intraFreqEventCriteriaList_v590ext,
        intra_freqeventcriterialist_v590ext_t *intra_freq_event_criteria_list_v590ext);

void convert_PeriodicReportingInfo_1b(
        UMTS_PeriodicReportingInfo_1b *periodicReportingInfo_1b,
        periodicreportinginfo_1b_t    *periodic_reporting_info_1b);

void convert_IntraFreqReportingCriteria_1b_r5(
        UMTS_IntraFreqReportingCriteria_1b_r5 *intraFreqReportingCriteria_1b_r5,
        intrafreqreportingcriteria_1b_r5_t *intra_freq_reporting_criteria_1b_r5);

void convert_IntraFreqEvent_1d_r5(
        UMTS_IntraFreqEvent_1d_r5 *intraFreqEvent_1d_r5,
        intrafreqevent_1d_r5_t *intra_freq_event_1d_r5);

void convert_SysInfoType11_v590ext_IEs(
        UMTS_SysInfoType11_v590ext_IEs *sysInfoType11_v590ext,
        sysinfotype11_v590ext_ies_t    *sys_info_type11_v590ext);

void convert_InterFreqRACHReportingInfo_modeSpecificInfo_fdd(
        UMTS_InterFreqRACHReportingInfo_modeSpecificInfo_fdd *asn_fdd,
        interfreqrachreportinginfo_modespecificinfo_fdd_t *umts_fdd);

void convert_InterFreqRepQuantityRACH_TDDList(
        UMTS_InterFreqRepQuantityRACH_TDDList *interFreqRepQuantityRACH_TDDList,
        interfreqrepquantityrach_tddlist_t   *inter_freq_repquantityrach_tdd_list);

void convert_InterFreqRACHReportingInfo_modeSpecificInfo_tdd(
        UMTS_InterFreqRACHReportingInfo_modeSpecificInfo_tdd *asn_tdd,
        interfreqrachreportinginfo_modespecificinfo_tdd_t *umts_tdd);

void convert_InterFreqRACHReportingInfo_modeSpecificInfo(
        UMTS_InterFreqRACHReportingInfo_modeSpecificInfo *modeSpecificInfo,
        interfreqrachreportinginfo_modespecificinfo_t    *mode_specific_info);

void convert_InterFreqRACHReportingInfo(
        UMTS_InterFreqRACHReportingInfo *interFreqRACHReportingInfo,
        interfreqrachreportinginfo_t *inter_freq_rachreporting_info);

void convert_SysInfoType11_v6b0ext_IEs(
        UMTS_SysInfoType11_v6b0ext_IEs *sysInfoType11_v6b0ext,
        sysinfotype11_v6b0ext_ies_t *sys_info_type11_v6b0ext);

void convert_MBSFNFrequency(
        UMTS_MBSFNFrequency *MBSFNFrequency,
        mbsfnfrequency_t *mbsfnfrequency);

void convert_MBSFNFrequencyList(
        UMTS_MBSFNFrequencyList *mbsfnFrequencyList,
        mbsfnfrequencylist_t  *mbsfn_frequency_list);

void convert_SysInfoType11_v770ext_IEs(
        UMTS_SysInfoType11_v770ext_IEs *sysInfoType11_v770ext,
        sysinfotype11_v770ext_ies_t *sys_info_type11_v770ext);

void convert_NewInterFreqCell_v7b0ext(
        UMTS_NewInterFreqCell_v7b0ext *NewInterFreqCell_v7b0ext,
        newinterfreqcell_v7b0ext_t   *newinterfreqcell_v7b0ext);

void convert_NewInterFreqCellList_v7b0ext(
        UMTS_NewInterFreqCellList_v7b0ext *newInterFreqCellList,
        newinterfreqcelllist_v7b0ext_t   *new_inter_freq_cell_list);

void convert_SysInfoType11_v7b0ext_IEs(
        UMTS_SysInfoType11_v7b0ext_IEs *sysInfoType11_v7b0ext,
        sysinfotype11_v7b0ext_ies_t *sys_info_type11_v7b0ext);

void convert_MBSFNFrequency_v860ext(
        UMTS_MBSFNFrequency_v860ext *MBSFNFrequency_v860ext,
        mbsfnfrequency_v860ext_t *mbsfnfrequency_v860ext);

void convert_MBSFNFrequencyList_v860ext(
        UMTS_MBSFNFrequencyList_v860ext *mbsfnFrequencyList,
        mbsfnfrequencylist_v860ext_t  *mbsfn_frequency_list);

void convert_SysInfoType11_v860ext_IEs(
        UMTS_SysInfoType11_v860ext_IEs *sysInfoType11_v860ext,
        sysinfotype11_v860ext_ies_t  *sys_info_type11_v860ext);

void convert_SysInfoType11_v4b0NonCriticalExtensions_v590NonCriticalExtension_v690NonCriticalExtensions_v6b0NonCriticalExtensions_v770NonCriticalExtensions_v7b0NonCriticalExtensions_v860NonCriticalExtensions_nonCriticalExtensions(
        UMTS_SysInfoType11_v4b0NonCriticalExtensions_v590NonCriticalExtension_v690NonCriticalExtensions_v6b0NonCriticalExtensions_v770NonCriticalExtensions_v7b0NonCriticalExtensions_v860NonCriticalExtensions_nonCriticalExtensions *nonCriticalExtensions,
        sysinfotype11_v4b0nces_v590nce_v690nces_v6b0nces_v770nces_v7b0nces_v860nces_nces_t   *non_critical_extensions);

void convert_SysInfoType11_v4b0NonCriticalExtensions_v590NonCriticalExtension_v690NonCriticalExtensions_v6b0NonCriticalExtensions_v770NonCriticalExtensions_v7b0NonCriticalExtensions_v860NonCriticalExtensions(
        UMTS_SysInfoType11_v4b0NonCriticalExtensions_v590NonCriticalExtension_v690NonCriticalExtensions_v6b0NonCriticalExtensions_v770NonCriticalExtensions_v7b0NonCriticalExtensions_v860NonCriticalExtensions *v860NonCriticalExtensions,
        sysinfotype11_v4b0nces_v590nce_v690nces_v6b0nces_v770nces_v7b0nces_v860nces_t *v860_non_critical_extensions);

void convert_SysInfoType11_v4b0NonCriticalExtensions_v590NonCriticalExtension_v690NonCriticalExtensions_v6b0NonCriticalExtensions_v770NonCriticalExtensions_v7b0NonCriticalExtensions(
        UMTS_SysInfoType11_v4b0NonCriticalExtensions_v590NonCriticalExtension_v690NonCriticalExtensions_v6b0NonCriticalExtensions_v770NonCriticalExtensions_v7b0NonCriticalExtensions *v7b0NonCriticalExtensions,
        sysinfotype11_v4b0nces_v590nce_v690nces_v6b0nces_v770nces_v7b0nces_t *v7b0_non_critical_extensions);

void convert_v4b0NonCriticalExtensions_v590NonCriticalExtension_v690NonCriticalExtensions_v6b0NonCriticalExtensions_v770NonCriticalExtensions(
        UMTS_SysInfoType11_v4b0NonCriticalExtensions_v590NonCriticalExtension_v690NonCriticalExtensions_v6b0NonCriticalExtensions_v770NonCriticalExtensions *v770NonCriticalExtensions,
        sysinfotype11_v4b0nces_v590nce_v690nces_v6b0nces_v770nces_t  *v770noncritical_extensions);

void convert_SysInfoType11_v4b0NonCriticalExtensions_v590NonCriticalExtension_v690NonCriticalExtensions_v6b0NonCriticalExtensions(
        UMTS_SysInfoType11_v4b0NonCriticalExtensions_v590NonCriticalExtension_v690NonCriticalExtensions_v6b0NonCriticalExtensions *v6b0NonCriticalExtensions,
        sysinfotype11_v4b0nces_v590nces_v690nces_v6b0nces_t  *v6b0_non_critical_extensions);

void convert_Dummy_InterFreqRACHReportingInfo_modeSpecificInfo_fdd(
        UMTS_Dummy_InterFreqRACHReportingInfo_modeSpecificInfo_fdd *asn_fdd,
        dummy_interfreqrachreportinginfo_modespecificinfo_fdd_t *umts_fdd);

void convert_Dummy_InterFreqRACHReportingInfo_modeSpecificInfo_tdd(
        UMTS_Dummy_InterFreqRACHReportingInfo_modeSpecificInfo_tdd *asn_tdd,
        dummy_interfreqrachreportingInfo_modespecificinfo_tdd_t *umts_tdd);

void convert_Dummy_InterFreqRACHReportingInfo_modeSpecificInfo(
        UMTS_Dummy_InterFreqRACHReportingInfo_modeSpecificInfo *modeSpecificInfo,
        dummy_interfreqrachreportinginfo_modespecificinfo_t  *mode_specific_info);

void convert_Dummy_InterFreqRACHReportingInfo(
        UMTS_Dummy_InterFreqRACHReportingInfo *asn_dummy,
        dummy_interfreqrachreportinginfo_t   *umts_dummy);

void convert_SysInfoType11_v690ext_IEs(
        UMTS_SysInfoType11_v690ext_IEs *sysInfoType11_v690ext,
        sysinfotype11_v690ext_ies_t *sys_info_type11_v690ext);

void convert_SysInfoType11_v4b0NonCriticalExtensions_v590NonCriticalExtension_v690NonCriticalExtensions
(
 UMTS_SysInfoType11_v4b0NonCriticalExtensions_v590NonCriticalExtension_v690NonCriticalExtensions *v690NonCriticalExtensions,
 sysinfotype11_v4b0noncriticalextensions_v590noncriticalextension_v690noncriticalextensions_t *v690_non_critical_extensions
 );

void convert_SysInfoType11_v4b0NonCriticalExtensions_v590NonCriticalExtension
(
 UMTS_SysInfoType11_v4b0NonCriticalExtensions_v590NonCriticalExtension *v590NonCriticalExtension,
 sysinfotype11_v4b0_nc_extensions_v590_nc_extension_t *v590_nc_extension
 );

void convert_sysInfoType11_v4b0ext
(
 UMTS_SysInfoType11_v4b0ext_IEs *sysInfoType11_v4b0ext,
 sysinfotype11_v4b0ext_ies_t    *sys_info_type11_v4b0ext
 );

void convert_sysinfotype11_v4b0_nc_extensions
(
 UMTS_SysInfoType11_v4b0NonCriticalExtensions *v4b0NonCriticalExtensions,
 sysinfotype11_v4b0_nc_extensions_t  *sysinfotype11_v4b0_nc_extensions
 );

#endif
