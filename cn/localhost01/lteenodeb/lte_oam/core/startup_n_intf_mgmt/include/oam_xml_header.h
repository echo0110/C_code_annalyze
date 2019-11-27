/********************************************************************************
 * File Name   : oam_xml_header.h
 * Description : This is a header file which contains xml functions
 *               
 * Revision History:
 * Date         Author          Reference             Comments
 * -----        ------          ---------             --------
 * May,2012    KAPIL TAWAR      OAM Design Document   Initial Version
 * September,2013 Neeraj Kumar  OAM Design Doc        Changes related to HM
 * Jan, 2014        Ravi Gupta              CSR 00058940 Fix
 * April, 2014      Dinesh Dua  CSR 00070293 Fix
 * Jan, 2014        Ravi Gupta  CSR 00058940 Fix
 * April, 2014      Dinesh Dua  CSR 00070293 Fix
 * April, 2014      Ravi Gupta  SPR 10551 Fix
 * June, 2014        Vivek K Gupta  Release 3.0         Changes for xml 
 *                                                      add/delete frwk code
 * June, 2014       Avinash Kumar  SPR 12228 Fix
 * Aug 2014       Shalu Sagar           SPR 12987 Fix
 * Aug 2014        Aditi Singh      SPR 13065 
 * Sep 2014       Vinod Vyas        Changes Related to eICIC 
 * Copyright (c) 2014, Aricent Inc. All Rights Reserved
 ******************************************************************************/

/*******************************************************************************
 Private Types
 ******************************************************************************/
 
#ifndef _xml_header_h_
#define _xml_header_h_

/*******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <oam_types.h>
#include <rrm_oam_defines.h>
#ifdef LTE_EMBMS_SUPPORTED
#include <oam_embms_defines.h>
#endif

/****************************************************************************
 **  ENUMERATIONS / STRUCTURES
 ****************************************************************************/
 /* SPR 20653 Fix Start */
typedef struct _xml_struct_freq_tbl_st
{
    xmlNode * freq_priority;
}xml_struct_freq_tbl_st;

typedef struct _xml_struct_rat_tbl_st
{
    xmlNode *rat_priority;
    xml_struct_freq_tbl_st freq_priority[RRM_MAX_FREQ_PRIORITY];
}xml_struct_rat_tbl_st;

typedef struct _xml_struct_spid_tbl_st
{
    xmlNode * p_spid_config;
    xml_struct_rat_tbl_st rat_priority[RRM_OAM_IRAT_TYPE];
}xml_struct_spid_tbl_st;
 /* SPR 20653 Fix End */
typedef struct _XML_struct_cell_config
{
    xmlNode * LTE_4;
    xmlNode * Tunnel_2;
    xmlNode * EPC;
    xmlNode * PLMNList[RRM_OAM_MAX_NUM_PLMNS];
    xmlNode * QoS[9];
    xmlNode * RAN;
    xmlNode * Common_1;
    xmlNode * X_VENDOR_ADDITIONAL_OPERATOR_INFO;
    xmlNode * SCHEDULING_INFO[RRM_OAM_MAX_NUM_SI];
    xmlNode * X_VENDOR_EMERGENCY_SERVICE;
    xmlNode * S1AP;
    xmlNode * S1U;
    xmlNode * GTPUEchoInterval;
    xmlNode * RF_1;
    xmlNode * PHY;
    xmlNode * AntennaInfo;
    xmlNode * PDSCH;
    xmlNode * SRS;
    xmlNode * PRACH;
    xmlNode * PUCCH;
    xmlNode * PUSCH;
    xmlNode * ULRS;	
    xmlNode * ULPowerControl;
    /* EMBMS Changes Start */
    xmlNode * MBSFN;
    xmlNode * MBSFN_SERVICE_AREA_CONFIG;
#ifdef LTE_EMBMS_SUPPORTED
    xmlNode * SFConfigList[MAX_SFCONFIG_NUM_ENTRIES];
#endif    
    /* EMBMS Changes End */
    xmlNode * PRS;
    xmlNode * TDDFrame;
    xmlNode * MAC;
    xmlNode * RACH;
    xmlNode * DRX;
    xmlNode * ULSCH;
/* CSR 59124 Change Start*/
    xmlNode * CDRX;
/* CSR 59124 Change End*/
    xmlNode * RLC;
    xmlNode * SRB1;	
    xmlNode * SRB2;
    xmlNode * RRCTimers;
    xmlNode * CellRestriction;
    xmlNode * X_VENDOR_ACCESS_BARRING_INFO;
    xmlNode * AC_BARRING_FOR_MO_SIGNALLING;
    xmlNode * AC_BARRING_FOR_MO_DATA;
    xmlNode * SSAC_BARRING_FOR_MMTEL_VOICE_R9;
    xmlNode * SSAC_BARRING_FOR_MMTEL_VIDEO_R9;
    xmlNode * AC_BARRING_FOR_CSFB_R10;
    xmlNode * Mobility;
    xmlNode * IdleMode;
    xmlNode * Common_2;
    xmlNode * IntraFreq;
    xmlNode * InterFreq;
    xmlNode * Carrier[RRM_OAM_MAX_NUM_INTER_FREQ_CARRIERS];
    xmlNode * IRAT_1;
    xmlNode * UTRA;
    xmlNode * UTRANFDDFreq[RRM_MAX_NUM_UTRA_FDD_CARRIERS];
    xmlNode * UTRANTDDFreq[RRM_MAX_NUM_UTRA_TDD_CARRIERS];
    xmlNode * GERAN;
    xmlNode * GERANFreqGroup[MAX_GERAN_FREQ];
    xmlNode * CDMA2000;
    xmlNode * CDMA2000Band[RRM_MAX_CDMA_BAND_CLASS];
    xmlNode * X_VENDOR_PRE_REG_INFO_HRPD;
    xmlNode * X_VENDOR_SECONDARY_LIST;
    xmlNode * X_VENDOR_MOBILITY_SIB_8_PARAMS;
    xmlNode * X_VENDOR_CDMA2000_CELL_PARAM;
    xmlNode * CDMA2000_RAND;
    xmlNode * CELL_ID_1XRTT;
    xmlNode * CELL_ID_HRPD;
    /* Start: SPR 10551 */
    xmlNode * REFERENCE_CELL_ID_1XRTT;
    xmlNode * REFERENCE_CELL_ID_HRPD;
    /*End: SPR 10551 */
    xmlNode * ConnMode;
    xmlNode * EUTRA;
    xmlNode * IRAT_2;
    xmlNode * NeighborList;
    xmlNode * LTECell[RRM_OAM_MAX_NUM_INTRA_FREQ_CELLS];
    /*SPR 15604 START */
    xmlNode * LTECell_NLIU[OAM_MAX_LTE_NBR_CELL_NUM_ENTRIES];
    /*SPR 15604 END*/

    xmlNode * InterRATCell_1;
    xmlNode * UMTS[RRM_OAM_MAX_NUM_INTER_FREQ_CELLS];
    /*SPR 15604 START*/
    xmlNode * UMTS_NLIU[OAM_MAX_UMTS_NBR_CELL_NUM_ENTRIES];
//    xmlNode * CellConfig[OAM_MAX_CELL_SUPPORTED];
    /*SPR 15604 END*/

/*  struct member fot NeighborListInUse */
    xmlNode * GSM[RRM_OAM_MAX_NUM_GERAN_FREQ_CELLS];
    xmlNode * CDMA2000_2[RRM_OAM_NUM_CDMA2000_FREQ_CELLS];
    xmlNode * NeighborListInUse;

#ifdef ENDC_ENABLED
    xmlNode * endc_params;
#endif

    /*SPR 23209 FIXED START */
    xmlNode * mocn_params;
    xmlNode * operator_list;
    xmlNode * operators_info[MAX_MOCN_OPERATOR];
    /*SPR 23209 FIXED END */


    xmlNode * InterRATCell_2;

    xmlNode * X_VENDOR_ICIC_PARAMS;
    xmlNode * atb_config;
    xmlNode * path_loss_to_target_sinr_map_info[RRM_OAM_MAX_NUM_PATHLOSS_TO_SINR_COUNT];
    xmlNode * path_loss_target_sinr_map_sps[RRM_OAM_MAX_NUM_ALPHA_COUNT];
    xmlNode * alpha_pathloss_target_sinr_map_sps;
    xmlNode * path_loss_to_target_sinr_map_info_sps[RRM_OAM_MAX_NUM_PATHLOSS_TO_SINR_COUNT];
    xmlNode * dynamic_icic_info;
    xmlNode * ul_cell_center_region[RRM_OAM_MAX_CC_REGION];
    xmlNode * dl_cell_center_region[RRM_OAM_MAX_CC_REGION];
    xmlNode * dl_resource_partition_info;
    xmlNode * dl_cell_edge_region;
    xmlNode * path_loss_target_sinr_map;
    xmlNode * alpha_pathloss_target_sinr_map;
    xmlNode * rntp_report_config_info;
    xmlNode * ul_cell_edge_region;
    xmlNode * ul_resource_partition_info;
    xmlNode * ul_power_mask;
    xmlNode * ca_params;
    xmlNode * ca_mac_sched_params;
    /* eICIC Feature Changes Start */
    xmlNode * eicic_params;
    xmlNode * AbsInfo;
    xmlNode * AbsPattern;
    xmlNode * eICICTimerInfo;
    /* eICIC Feature Changes End */
    /* SPR 20653 Fix Start */
    xmlNode * dci_params;
    xmlNode * cooloff_period;
    xmlNode * delta_sinr_tpc_map_pusch_info;
    xmlNode * delta_sinr_to_tpc_map_pusch_info[RRM_OAM_MAX_DELTA_SINR_TO_TCP_MAP_PUSCH];
    xmlNode * bler_to_tpc_map_for_pucch;
    xmlNode * delta_bler_tpc_map_pucch_info[RRM_OAM_MAX_NUM_TPC_FOR_PUCCH];
    xmlNode * delta_sinr_tpc_map_pucch_info;
    xmlNode * delta_sinr_to_tpc_map_pucch_info[RRM_OAM_MAX_NUM_TPC_FOR_PUCCH];
    xmlNode * report_config_info;
    xmlNode * l1_meas_report_config_info;
    xmlNode * l2_meas_report_config_info;
    xmlNode * X_VENDOR_OLPC_CONFIG_PARAMS;
    xmlNode * olpc_pusch_supported_category;
    xmlNode * olpc_pucch_supported_category;
    xmlNode * olpc_tupple_pusch[RRM_OLPC_CATEGORY_TABLE_SIZE];
    xmlNode * olpc_tupple_pucch[RRM_OLPC_CATEGORY_TABLE_SIZE];
    /* SPR 20653 Fix End */
}XML_struct_cell_config; 

/* spr_21013_fix_start */
/* SPR 21013 contains changes for support of multiple FAPService instances.
 * xmlNode * FAPService has been changed to XML_struct_fap_service 
 * FAPService[OAM_MAX_CELL_SUPPORTED] type in XML_struct_tr196 structure.
 * CellConfig structure has been changed to CellConfig single element and
 * now it is a part of XML_struct_fap_service structure. New
 * structure of XML_struct_fap_service has been added */
typedef struct _XML_struct_fap_service 
{
    xmlNode * FapService;
    xmlNode * Capabilities;
    xmlNode * LTE_1;
    xmlNode * FAPControl;
    xmlNode * LTE_2;
    xmlNode * Gateway;
    xmlNode * AccessMgmt;
    xmlNode * LTE_3;
/* SPR 16195 Fix Start*/	    
    xmlNode * DRX;
    /* SPR 16195 Fix End */
    /* SPR 17730 Fix Start */
     xmlNode * UTRA;
     /* SPR 17730 Fix End */


    XML_struct_cell_config CellConfig;

/*  struct member fot NeighborListInUse */
    xmlNode * ULSCH;
/* CSR 59124 Change Start*/
    xmlNode * CDRX;
/* CSR 59124 Change End*/
    xmlNode * cell_specific_params;
    xmlNode * pz_hyst_parameters_included;
    xmlNode * fpc_fch_included;
    xmlNode * NeighborListInUse;
    xmlNode * InterRATCell_2;
/*  struct member fot NeighborListInUse ends */
    xmlNode * REM;
    xmlNode * LTE_5;
    /*SPR_16048 Fix Start*/
    xmlNode * Cell[OAM_MAX_CELL_OBJECTS];
    /*SPR_16048 Fix End*/
    xmlNode * RF_2;
    xmlNode * BCCH;
    xmlNode * PLMNList_2;
    xmlNode * CarrierMeas;
    xmlNode * Transport;
    xmlNode * SCTP;
    /* SPR 16045 Fix Start */
    /* Code Removed */
    /* SPR 16045 Fix End */
}XML_struct_fap_service;

struct XML_struct_tr196 
{
    xmlNode * InternetGatewayDevice;
    xmlNode * nms_ftp_server_params;
    xmlNode * Time;
    xmlNode * ManagementServer;
    xmlNode * DeviceInfo;
    xmlNode * LANDevice;
    xmlNode * LAN_HostConfigManagement;
    xmlNode * IPInterface[OAM_MAX_NUM_IP_INTRFACES];
    xmlNode * WANDevice;
    xmlNode * WANEthernetInterfaceConfig;
    xmlNode * FAP;
    xmlNode * PerfMgmt;
    xmlNode * Config;
    xmlNode * Tunnel;
    xmlNode * CryptoProfile;
    xmlNode * IKESA;
    xmlNode * VirtualInterface;
    xmlNode * Services;
    XML_struct_fap_service FAPService[OAM_MAX_CELL_SUPPORTED];
    xmlNode * FaultMgmt;
    xmlNode * SupportedAlarm[200];
};
/* spr_21013_fix_end */

typedef struct XML_struct_tr196 xml_struct_tr196;

typedef struct __prop_cellconfig_xml_st {
    /* SPR-13976 START */
    /* AccessMgmt hierarchy pointers removed */
    /* SPR-13976 END */
    xmlNode * ADDITIONAL_ICIC_params;
    xmlNode * pdcch_aggregation_power_offset;
/* SPR 18641_18843 fix start */
    xmlNode * tti_bundling_info_params;
/* SPR 18641_18843 fix end */
    xmlNode * aggregation_power_offset_cc_user_dci[RRM_OAM_NUM_DCI_SIZE_CATEGORY];
    xmlNode * aggregation_power_offset_cc_user_cqi[RRM_OAM_NUM_DCI_SIZE_CATEGORY]\
              [RRM_OAM_MAX_CQI_VALUE]; /*  [3][15]*/
    xmlNode * aggr_pwr_offset_tuples_cc_user[RRM_OAM_NUM_DCI_SIZE_CATEGORY]\
              [RRM_OAM_MAX_CQI_VALUE][RRM_OAM_MAX_AGGREGATION_LEVEL_POWER_OFFSET_COUNT]; /* 15 * 3 ( 15 qci  * 3 dci * count )cqi 0 [3][15][4]*/
    xmlNode * aggregation_power_offset_ce_user_dci[RRM_OAM_NUM_DCI_SIZE_CATEGORY];
    xmlNode * aggregation_power_offset_ce_user_cqi[RRM_OAM_NUM_DCI_SIZE_CATEGORY][RRM_OAM_MAX_CQI_VALUE];
    xmlNode * aggr_pwr_offset_tuples_ce_user[RRM_OAM_NUM_DCI_SIZE_CATEGORY]\
              [RRM_OAM_MAX_CQI_VALUE][RRM_OAM_MAX_AGGREGATION_LEVEL_POWER_OFFSET_COUNT];
    xmlNode * cqi_to_phich_power;
    xmlNode * dynamic_cfi_extension_params;
    xmlNode * sps_params;
    xmlNode * spsCommIntervalProfile[SPS_INTERVAL_LIST];
    xmlNode * maxPdcpPduSizeIpv4[SPS_INTERVAL_LIST];
    xmlNode * maxPdcpPduSizeIpv6[SPS_INTERVAL_LIST];
    xmlNode * supportedDlIntervalList;
    xmlNode * supportedUlIntervalList;
    xmlNode * spsDLIntervalProfile[SPS_INTERVAL_LIST];
    xmlNode * spsULIntervalProfile[SPS_INTERVAL_LIST];
    xmlNode * sps_dl_scheduling_Info_per_tti;
    xmlNode * sps_ul_scheduling_Info_per_tti;
    xmlNode * mimo_mode_params;
    xmlNode * mimo_codebook_subset_restriction[RRM_OAM_MAX_CODEBOOK_SUBSET];
    xmlNode * mimo_transmission_mode_table;
    xmlNode * rrm_oam_tm_mode_table_row[RRM_OAM_MAX_TM_MODE_TABLE_ROW];
    xmlNode * scheduler_Params;
    xmlNode * bucket_management_params;
    xmlNode * Scheduling_Weights_Params;
    xmlNode * spid_table;
    /* SPR 20653 Fix Start */
    xml_struct_spid_tbl_st spid_config[RRM_OAM_MAX_SPID];
    /* SPR 20653 Fix End */
    xmlNode * power_control;
    xmlNode * power_control_enable;
    xmlNode * ue_generic_cdma2000_params;
    xmlNode * admission_control_info;
    xmlNode * available_gbr_limit;
    xmlNode * ho_configuration;
    xmlNode * ho_retry_params;
    xmlNode * ho_algo_params;
    xmlNode * target_cell_selection_params;
    xmlNode * measurement_configuration;
    xmlNode * csfb_tgt_selection;
    xmlNode * dimension_params;
    xmlNode * RNTI;
    xmlNode * RArnti;
    xmlNode * sps_crnti;
    xmlNode * tpc_rnti;
    xmlNode * rrm_ue_scheduling_info;
    xmlNode * additional_packet_scheduling_params;
    xmlNode * Scheduling_Weights_Params_DL;
    xmlNode * Scheduling_Weights_Params_UL;
    xmlNode * Scheduling_Bit_Rate_Factor;
    xmlNode * No_Of_UEs;
    xmlNode * ControlChennelConfiguration;
    xmlNode * preferredPrimaryDCIFormatForSingleTB;
/* SPR 11001 FIX START */
    xmlNode * load_config;
    xmlNode * load_level_info[MAX_LOAD_LEVEL_NUM_ENTRIES];
    xmlNode * load_access_class_barring_config[MAX_LOAD_LEVEL_NUM_ENTRIES];
    /* eICIC Feature Changes Start */ 
    xmlNode * eicic_load_config[MAX_LOAD_LEVEL_NUM_ENTRIES];
    /* eICIC Feature Changes End */ 
    xmlNode * acBarringForMoSignalling[MAX_LOAD_LEVEL_NUM_ENTRIES];
    xmlNode * acBarringForMoData[MAX_LOAD_LEVEL_NUM_ENTRIES];
    xmlNode * ssac_barring_for_mmtel_voice_r9[MAX_LOAD_LEVEL_NUM_ENTRIES];
    xmlNode * ssac_barring_for_mmtel_video_r9[MAX_LOAD_LEVEL_NUM_ENTRIES];
    /*SPR 12228 FIX START*/
            /*Redundant variables removed */
    /*SPR 12228 FIX END*/
/* SPR 11001 FIX END */
    /* SPR 13768 Changes Start */
    xmlNode * MAC_Params;
    /* SPR 13768 Changes End */
    /* SPR 20653 Fix Start */
    xmlNode * ho_oscillation_params;
    xmlNode * ho_block_cell_params;
    xmlNode * ul_mimo_mode_params;
    xmlNode * tm_mode_additional_info;
    xmlNode * beam_forming_vector;
    xmlNode * mac_periodic;
    xmlNode * default_rat_priority_info;
    xmlNode * prb_estimation_factor;
    xmlNode * uplink_power_control_dedicated;
    /* SPR 20653 Fix End */
} prop_cellconfig_xml_st;

typedef struct __agent_inst_info_xml_st {
    UInt8   instance_id;
    UInt8   num_process;
    xmlNode **process_info; /* Array of xmlNode pointers */
}agent_inst_info_xml_st;

/*SPR 21889 Start*/
typedef struct __agent_info_xml_st {
    UInt8 num_of_instance;
    agent_inst_info_xml_st  *p_inst_xml_info;
}agent_info_xml_st;

/*SPR 21889 End */
struct XML_struct
{
    xmlNode * eNodeBParams;
#ifdef OAM_SHARED_MEM_LOGGER
    xmlNode * oam_shared_mem_config;
#endif
    prop_cellconfig_xml_st CellConfig[OAM_MAX_CELL_SUPPORTED];
    /* spr 10918 Fix start*/
    xmlNode * traffic_generator_params[OAM_MAX_NUM_L2_INSTANCE];
    /* spr 10918 Fix end*/
    xmlNode * load_configuration;
/* SPR 11001 FIX START */
    /* CODE MOVED */
/* SPR 11001 FIX END */
    xmlNode * health_monitoring;
    xmlNode * rach_opt_params;
    xmlNode * stack_config;
    xmlNode * L1params; 
    xmlNode * L1_Power_Control_param;
    xmlNode * L2params;
    xmlNode * RLCParams;
    xmlNode * RLC_Q_Management;
    xmlNode * MACParams;
    /* SPR21708 Changes Start */
    xmlNode * GTPUParams;
    /* SPR21708 Changes end */
    xmlNode * L3Params;
    xmlNode * shared_mem_logging_config;
    xmlNode * RRCparams;
    xmlNode * Cell_Traffic_Trace_feature;
    xmlNode * RRCparams_guard_timers;
    xmlNode * RRCparams_procedural_timers;
    xmlNode * S1AP;
    xmlNode * eNB_CommInfo;
    xmlNode * S1AP_procedural_timers;
    xmlNode * X2AP;
    xmlNode * local_eNB_CommInfo;
#ifdef OAM_SON_ENABLE
    xmlNode * son_comm_info;
//#else //rm for support x2ap
    xmlNode * peer_eNB_CommInfo[MAX_PEER_ENB];
#endif
    xmlNode * X2AP_guard_timers;
    xmlNode * X2AP_procedural_timers;
    xmlNode * RRMparams;
    xmlNode * shm_logger_cfg;
    xmlNode * operator_info;
#ifdef ENDC_ENABLED
    xmlNode * peer_gNB_CommInfo[MAX_PEER_GNB];
    xmlNode * EN_DC_Params;
#if 1
    xmlNode * peer_pci_info[MAX_PEER_GNB];
#endif
#endif
    xmlNode * SONParams;
    xmlNode * son_es_params;
    xmlNode * son_oam_nr_enb_add_req;
    xmlNode * nbr_enb_list;
    xmlNode * nbr_enb_info[3];
    /*eICIC_PHASE_1_2_CHANGES_START*/
    xmlNode * son_oam_anr_neighbor_default_config_req;
    /*SPR 14554 start*/
    xmlNode *son_info_ind;
    /*SPR 14554 end*/
    /*eICIC_PHASE_1_2_CHANGES_END*/
   /* EMBMS Changes Start */
    /*SPR 21889 +- */
#ifdef LTE_EMBMS_SUPPORTED
    xmlNode * M2AP;
    xmlNode * EGTPUParams;
#endif
    /* EMBMS Changes End */
    /* SPR_19619 start */
    xmlNode *son_oam_peer_cell_activation_req;
    /* SPR_19619 stop */
/* LWA changes start*/
/* LWA changes end*/



};

typedef struct XML_struct xml_struct;

typedef struct xml_struct_nw_scan_st
{
    xmlNode* InternetGatewayDevice;
    xmlNode* Services;
    xmlNode* FAPService;
    xmlNode* REM;
    xmlNode* LTE;
}xml_struct_nw_scan_t;


extern struct XML_struct_tr196 xml_struct_tr196_g;
extern struct XML_struct xml_struct_g;

/****************************************************************************
 **  MACROS
 ****************************************************************************/
#define OAM_DL_CP 1
#define OAM_UL_CP 1
#define MIN_NUMANTENNA 0
#define MAX_NUMANTENNA 2 
#define MIN_DL_SCHEDULER_STRATEGY               0
#define MAX_DL_SCHEDULER_STRATEGY               2
#define MIN_UL_SCHEDULER_STRATEGY               0
#define MAX_UL_SCHEDULER_STRATEGY               2
#define MIN_ULCONFIGADVANCESUBFRAME 0
#define MAX_ULCONFIGADVANCESUBFRAME 2
#define MIN_CQI_THRESHOLD                        0
#define MAX_CQI_THRESHOLD                       15
#define MIN_SISCHEDULINGRATPOLICY 1
#define MAX_SISCHEDULINGRATPOLICY 2
#define MIN_GBR_BUCKET_FACTOR                    1
#define MAX_GBR_BUCKET_FACTOR                    5
#define MIN_AMBR_BUCKET_FACTOR                   1
 /* COMPILATION_FIX_20AUG */ 
#define MAX_AMBR_BUCKET_FACTOR                   5
 /* COMPILATION_FIX_20AUG */ 

#define MIN_DL_PDB_WEIGHT                        0
#define MAX_DL_PDB_WEIGHT                       10 
#define MIN_DL_QCI_WEIGHT                        0
#define MAX_DL_QCI_WEIGHT                       10 
#define MIN_DL_TOKEN_WEIGHT                      0
#define MAX_DL_TOKEN_WEIGHT                     10
#define MIN_DL_QLOAD_WEIGHT                      0
#define MAX_DL_QLOAD_WEIGHT                     10 
#define MIN_DL_MCS_WEIGHT                        0
#define MAX_DL_MCS_WEIGHT                       10 

#define MIN_UL_PDB_WEIGHT                        0
#define MAX_UL_PDB_WEIGHT                       10 
#define MIN_UL_QCI_WEIGHT                        0
#define MAX_UL_QCI_WEIGHT                       10 
#define MIN_UL_TOKEN_WEIGHT                      0
#define MAX_UL_TOKEN_WEIGHT                     10 
#define MIN_UL_QLOAD_WEIGHT                      0
#define MAX_UL_QLOAD_WEIGHT                     10 
#define MIN_UL_MCS_WEIGHT                        0
#define MAX_UL_MCS_WEIGHT                       10 

#define MIN_QOSQCIINFO_2_QCI 1
#define MAX_QOSQCIINFO_2_QCI 9 
#define MIN_QOSQCIINFO_2_RESTYPE 0
#define MAX_QOSQCIINFO_2_RESTYPE 1 
#define MIN_QOSQCIINFO_2_PRIORITY 1
#define MAX_QOSQCIINFO_2_PRIORITY 9 
#define MIN_QOSQCIINFO_2_PKTDLYBDGT 50
#define MAX_QOSQCIINFO_2_PKTDLYBDGT 300 
#define MIN_QOSQCIINFO_2_PKTERRLOSSRATE 0
#define MAX_QOSQCIINFO_2_PKTERRLOSSRATE 6 

#define MIN_ENABLE_ALLOC_AFTER_ALLOWED_BITRATE                      0
/* SPR 22539 Fix + */
#define MAX_ENABLE_ALLOC_AFTER_ALLOWED_BITRATE                      2 
/* SPR 22539 Fix - */
#define MIN_DL_RB_RESTRICTION_FACTOR                                1
#define MAX_DL_RB_RESTRICTION_FACTOR                                3 
#define MIN_ULRBRESTRICTIONFACTOR 1
#define MAX_ULRBRESTRICTIONFACTOR 3 
/* SPR 12975 Changes Start */
#define MIN_DL_AMBRTOKENLIMITFACTOR                 -1
#define MAX_DL_AMBRTOKENLIMITFACTOR                  1
#define MIN_DL_GBRTOKENLIMITFACTOR                  -1
#define MAX_DL_GBRTOKENLIMITFACTOR                   1 
#define MIN_UL_AMBRTOKENLIMITFACTOR                 -1
#define MAX_UL_AMBRTOKENLIMITFACTOR                  1 
#define MIN_UL_GBRTOKENLIMITFACTOR                  -1
#define MAX_UL_GBRTOKENLIMITFACTOR                   1
/* SPR 12975 Changes End */

#define MIN_DL_SCH_WITH_MIB_ALLOWED                                 0
#define MAX_DL_SCH_WITH_MIB_ALLOWED                                 1
#define MIN_COMM_CH_WITH_MIB_ALLOWED                                0
#define MAX_COMM_CH_WITH_MIB_ALLOWED                                1
/* EMBMS Changes Start */
#define MIN_EMBMS_MAX_SYNC_PERIOD                                   0
#define MAX_EMBMS_MAX_SYNC_PERIOD                               59999
#define MIN_SYNC_SEQ_DURATION                                       0
#define MAX_SYNC_SEQ_DURATION                                      10
#define MIN_SYNC_MARGIN_TIMER                                       0
#define MAX_SYNC_MARGIN_TIMER                                      10
/* EMBMS Changes End */
#define MIN_QOSQCIINFO_1_QCI 1
#define MAX_QOSQCIINFO_1_QCI 9 
#define MIN_QOSQCIINFO_1_RESTYPE 0
#define MAX_QOSQCIINFO_1_RESTYPE 1 
#define MIN_QOSQCIINFO_1_PRIORITY 1
#define MAX_QOSQCIINFO_1_PRIORITY 9 
#define MIN_QOSQCIINFO_1_PKTDLYBDGT 50
#define MAX_QOSQCIINFO_1_PKTDLYBDGT 300 
#define MIN_QOSQCIINFO_1_PKTERRLOSSRATE 0
#define MAX_QOSQCIINFO_1_PKTERRLOSSRATE 6 

#define MIN_QUEUESIZEFACTORGBR 1
#define MAX_QUEUESIZEFACTORGBR 6 
#define MIN_QUEUESIZEFACTORNGBR 1
#define MAX_QUEUESIZEFACTORNGBR 6 

#define MIN_RRM_INIT_LOG_ON_OFF 0
#define MAX_RRM_INIT_LOG_ON_OFF 1 
#define MIN_RRM_INIT_LOG_LEVEL 0 
#define MAX_RRM_INIT_LOG_LEVEL 6 

#define MIN_NUM_MNC_DIGIT 2
#define MAX_NUM_MNC_DIGIT 3 
#define MIN_CELL_BARRED 0
#define MAX_CELL_BARRED 1 
#define MIN_BARRING_FOR_EMERGENCY 0
#define MAX_BARRING_FOR_EMERGENCY 1 
#define MIN_INTRA_FREQ_RESELECTION 0
#define MAX_INTRA_FREQ_RESELECTION 1 
#define MIN_PDSCH_PB 0
#define MAX_PDSCH_PB 3 
#define MIN_PDSCH_PA 0
#define MAX_PDSCH_PA 7 
#define MIN_SRSENABLED 0
#define MAX_SRSENABLED 1 
#define MIN_SRSBANDWIDTHCONFIG 0
#define MAX_SRSBANDWIDTHCONFIG 7 
#define MIN_SRS_SUBFRAME_CONFIG 0
#define MAX_SRS_SUBFRAME_CONFIG 15 
#define MIN_SRSMAXUPPTS 0
#define MAX_SRSMAXUPPTS 1 
#define MIN_ACKNACKSRSSIMULTANEOUSTRANSMISSION 0
#define MAX_ACKNACKSRSSIMULTANEOUSTRANSMISSION 7 
#define MIN_ROOTSEQUENCEINDEX 0
#define MAX_ROOTSEQUENCEINDEX 837 
#define MIN_CONFIGURATIONINDEX 0
#define MAX_CONFIGURATIONINDEX 63 
#define MIN_HIGHSPEEDFLAG 0
#define MAX_HIGHSPEEDFLAG 1 
#define MIN_ZEROCORRELATIONZONECONFIG 0
#define MAX_ZEROCORRELATIONZONECONFIG 15 
#define MIN_FREQOFFSET 0
#define MAX_FREQOFFSET 94 
#define MIN_DELTAPUCCHSHIFT 0
#define MAX_DELTAPUCCHSHIFT 2 
#define MIN_NRBCQI 0
#define MAX_NRBCQI 98 
#define MIN_N1PUCCHAN 0
#define MAX_N1PUCCHAN 2047 
/* SPR 22311 Changes - Obselete Parameter Deleted */
#define MIN_NSB 1
#define MAX_NSB 4
#define MIN_HOPPINGMODE 0
#define MAX_HOPPINGMODE 1 
#define MIN_HOPPINGOFFSET 0
#define MAX_HOPPINGOFFSET 98 
#define MIN_GROUPHOPPINGENABLED 0
#define MAX_GROUPHOPPINGENABLED 1 
#define MIN_SEQUENCEHOPPINGENABLED 0
#define MAX_SEQUENCEHOPPINGENABLED 1 
#define MIN_P0NOMINALPUSCH -126
#define MAX_P0NOMINALPUSCH 24 

/* SPR 7497 CHANGES START */
#define MIN_SI_COUNT 1
#define MAX_SI_COUNT 32
#define MIN_SI_OAM_PERIODICITY 0  
#define MAX_SI_OAM_PERIODICITY 6
/* SPR 9938 START */
#define MIN_NUM_SIB_TYPE 0
/* SPR 9938 END */
#define MAX_NUM_SIB_TYPE 16
#define MIN_SIB_TYPE 0
#define MAX_SIB_TYPE 15  
/* SPR 7497 CHANGES END */
#define MIN_ALPHA 0
#define MAX_ALPHA 7 

#define MIN_P0NOMINALPUCCH -127
#define MAX_P0NOMINALPUCCH -96 
#define MIN_SPS_DCI_ATTEMPT 1
#define MAX_SPS_DCI_ATTEMPT 8
#define MIN_NUMPRSRESOURCEBLOCKS 0
#define MAX_NUMPRSRESOURCEBLOCKS 5 
#define MIN_PRS_CONF_INDEX 0
#define MAX_PRS_CONF_INDEX 2399
#define MIN_NUMCONSECUTIVEPRSSUBFAMES 0
#define MAX_NUMCONSECUTIVEPRSSUBFAMES 3 
#define MIN_NUMBEROFRAPREAMBLES 0
#define MAX_NUMBEROFRAPREAMBLES 15 
#define MIN_SIZEOFRAGROUPA 0
#define MAX_SIZEOFRAGROUPA 14 
#define MIN_MESSAGESIZEGROUPA 0
#define MAX_MESSAGESIZEGROUPA 3 
#define MIN_MESSAGEPOWEROFFSETGROUPB 0
#define MAX_MESSAGEPOWEROFFSETGROUPB 7 
#define MIN_POWERRAMPINGSTEP 0
#define MAX_POWERRAMPINGSTEP 3 
#define MIN_PREAMBLEINITIALRECEIVEDTARGETPOWER 0
#define MAX_PREAMBLEINITIALRECEIVEDTARGETPOWER 15 
#define MIN_PREAMBLETRANSMAX 0
#define MAX_PREAMBLETRANSMAX 10 
#define MIN_RESPONSEWINDOWSIZE 0
#define MAX_RESPONSEWINDOWSIZE 7 
#define MIN_CONTENTIONRESOLUTIONTIMER 0
#define MAX_CONTENTIONRESOLUTIONTIMER 7 
#define MIN_MAXHARQMSG3TX 1
#define MAX_MAXHARQMSG3TX 8 
#define MIN_DRX_ENABLED 0
#define MAX_DRX_ENABLED 1 
#define MIN_NUM_VALID_DRX_PROFILES 1
#define MAX_NUM_VALID_DRX_PROFILES 9 
#define MIN_NUM_APPLICABLE_QCI_LIST 1
#define MAX_NUM_APPLICABLE_QCI_LIST 9 
#define MIN_ON_DURATION_TIMER 0
#define MAX_ON_DURATION_TIMER 15 
#define MIN_DRX_INACTIVITY_TIMER 0
#define MAX_DRX_INACTIVITY_TIMER 22

#define MIN_AVGNUMAMLCPERUE 0
#define MAX_AVGNUMAMLCPERUE 8
#define MIN_AVGNUMUMLCPERUE 0
#define MAX_AVGNUMUMLCPERUE 8


#define MIN_DRX_RETRANSMISSION_TIMER 0
#define MAX_DRX_RETRANSMISSION_TIMER 7
#define MIN_LONG_DRX_CYCLE 0	
#define MAX_LONG_DRX_CYCLE 15
#ifdef ENDC_ENABLED
#define MIN_LONG_DRX_CYCLE_R15 0	
#define MAX_LONG_DRX_CYCLE_R15 17
#endif

#define MIN_DRX_START_OFFSETT  0
#define MAX_DRX_START_OFFSETT  2559
#define MIN_SHORT_DRX_CYCLE	  0
#define MAX_SHORT_DRX_CYCLE	 15
#define MIN_DRX_SHORT_CYCLE_TIMER 1
#define MAX_DRX_SHORT_CYCLE_TIMER 16
#define MIN_SRB1_DEFAULT_CONFIGURATION 0
#define MAX_SRB1_DEFAULT_CONFIGURATION 1
#define MIN_SRB1_T_POLL_RETRANSMIT 0
#define MAX_SRB1_T_POLL_RETRANSMIT 54
#define MIN_SRB1_POLL_PDU 0
#define MAX_SRB1_POLL_PDU 7
#define MIN_SRB1_POLL_BYTE 0
#define MAX_SRB1_POLL_BYTE 14
#define MIN_SRB1_MAX_RETX_THRESHOLD 0
#define MAX_SRB1_MAX_RETX_THRESHOLD 7
#define MIN_SRB1_T_REORDERING 0
#define MAX_SRB1_T_REORDERING 30
#define MIN_SRB1_T_STATUS_PROHIBIT 0
#define MAX_SRB1_T_STATUS_PROHIBIT 55
#define MIN_SRB2_DEFAULT_CONFIGURATION 0
#define MAX_SRB2_DEFAULT_CONFIGURATION 1
#define MIN_SRB2_T_POLL_RETRANSMIT 0
#define MAX_SRB2_T_POLL_RETRANSMIT 54
#define MIN_SRB2_POLL_PDU 0
#define MAX_SRB2_POLL_PDU 7
#define MIN_SRB2_POLL_BYTE 0
#define MAX_SRB2_POLL_BYTE 14
#define MIN_SRB2_MAX_RETX_THRESHOLD 0
#define MAX_SRB2_MAX_RETX_THRESHOLD 7
#define MIN_SRB2_T_REORDERING 0
#define MAX_SRB2_T_REORDERING 30
#define MIN_SRB2_T_STATUS_PROHIBIT 0
#define MAX_SRB2_T_STATUS_PROHIBIT 55
 
#define MIN_RRM_CELL_COMMON_2_Q_HYST 0
#define MAX_RRM_CELL_COMMON_2_Q_HYST 15
#define MIN_RRM_CELL_COMMON_2_Q_HYST_SF_MEDIUM 0
#define MAX_RRM_CELL_COMMON_2_Q_HYST_SF_MEDIUM 3
#define MIN_RRM_CELL_Q_HYST_SF_HIGH 0
#define MAX_RRM_CELL_Q_HYST_SF_HIGH 3
#define MIN_RRM_CELL_T_EVALUATION 0
#define MAX_RRM_CELL_T_EVALUATION 4
#define MIN_RRM_CELL_T_HYST_NORMAL 0
#define MAX_RRM_CELL_T_HYST_NORMAL 4
#define MIN_RRM_CELL_N_CELL_CHANGE_MEDIUM 1
#define MAX_RRM_CELL_N_CELL_CHANGE_MEDIUM 16
#define MIN_RRM_CELL_N_CELL_CHANGE_HIGH 1
#define MAX_RRM_CELL_N_CELL_CHANGE_HIGH 16
#define MIN_RRM_CELL_Q_RX_LEV_MIN_SIB_1 -70 
#define MAX_RRM_CELL_Q_RX_LEV_MIN_SIB_1 -22
#define MIN_RRM_CELL_Q_RX_LEV_MIN_OFFSET 1
#define MAX_RRM_CELL_Q_RX_LEV_MIN_OFFSET 8
#define MIN_RRM_CELL_P_MAX_SIB_1 -30
#define MAX_RRM_CELL_P_MAX_SIB_1 33
#define MIN_RRM_CELL_Q_RX_LEV_MIN_SIB_3 -70
#define MAX_RRM_CELL_Q_RX_LEV_MIN_SIB_3 -22
#define MIN_RRM_CELL_P_MAX_SIB_3 -30
#define MAX_RRM_CELL_P_MAX_SIB_3  33
#define MIN_RRM_CELL_S_INTRA_SEARCH 0
#define MAX_RRM_CELL_S_INTRA_SEARCH 31
#define MIN_RRM_CELL_TRESELECTIONEUTRASFMEDIUM 0
#define MAX_RRM_CELL_TRESELECTIONEUTRASFMEDIUM 3
#define MIN_RRM_CELL_TRESELECTIONEUTRASFHIGH 0
#define MAX_RRM_CELL_TRESELECTIONEUTRASFHIGH 3
#define MIN_RRM_CELL_S_NON_INTRA_SEARCH 0
#define MAX_RRM_CELL_S_NON_INTRA_SEARCH 31

#define MIN_RRM_CELL_CELL_RESELECTION_PRIORITY 0
#define MAX_RRM_CELL_CELL_RESELECTION_PRIORITY 7
#define MIN_RRM_CELL_THRESH_SERVING_LOW 0
#define MAX_RRM_CELL_THRESH_SERVING_LOW 31
#define MIN_RRM_CELL_EUTRA_CARRIER_ARFCN 0
#define MAX_RRM_CELL_EUTRA_CARRIER_ARFCN 65535

#define MIN_RRM_CELL_Q_RX_LEV_MIN_SIB_5 -70
#define MAX_RRM_CELL_Q_RX_LEV_MIN_SIB_5 -22
#define MIN_RRM_CELL_Q_OFFSET_FREQ 0
#define MAX_RRM_CELL_Q_OFFSET_FREQ 30
#define MIN_RRM_CELL_T_RESELECTION_EUTRA 0
#define MAX_RRM_CELL_T_RESELECTION_EUTRA 7
#define MIN_RRM_CELL_CELL_RESELECTION_PRIORITY 0
#define MAX_RRM_CELL_CELL_RESELECTION_PRIORITY 7

#define MIN_RRM_CELL_THRESH_X_HIGH 0
#define MAX_RRM_CELL_THRESH_X_HIGH 31
#define MIN_RRM_CELL_THRESH_X_LOW 0
#define MAX_RRM_CELL_THRESH_X_LOW 31
#define MIN_RRM_CELL_P_MAX -30
#define MAX_RRM_CELL_P_MAX 33
#define MIN_RRM_CELL_ALLOWED_MEASUREMENT_BW 0
#define MAX_RRM_CELL_ALLOWED_MEASUREMENT_BW 5
#define MIN_RRM_CELL_PRESENCE_ANTENNA_PORT1 0
#define MAX_RRM_CELL_PRESENCE_ANTENNA_PORT1 1
#define MIN_RRM_CELL_CARR_TRESELECTIONEUTRASFMEDIUM 0
#define MAX_RRM_CELL_CARR_TRESELECTIONEUTRASFMEDIUM 3


#define MIN_RRM_CELL_CELLRESERVEDFOROPERATORUSE 0
#define MAX_RRM_CELL_CELLRESERVEDFOROPERATORUSE 1
#define MIN_RRM_CELL_QCI 1
#define MAX_RRM_CELL_QCI 9
#define MIN_RRM_CELL_TYPE 0
#define MAX_RRM_CELL_TYPE 1
#define MIN_RRM_CELL_PRIORITY 1
#define MAX_RRM_CELL_PRIORITY 20
#define MIN_SGNB_AMBR_SHARE 1
#define MAX_SGNB_AMBR_SHARE 100
#define MAX_NUM_THROUGHPUT_REPORT 20
#define MIN_BEARER_THROUGHPUT_THRESHOLD_UL 5
#define MIN_BEARER_THROUGHPUT_THRESHOLD_DL 5
#define MAX_NUM_THROUGHPUT_THRESHOLD_DL 1000
#define MAX_ENDC_SINR_THRESHOLD_UL      255
#define MAX_ENDC_SINR_THRESHOLD_DL      255
#define MAX_BLER_THRESHOLD_UL           100
#define MAX_BLER_THRESHOLD_DL           100
#define MIN_SPLIT_BEARER_BITRATE_DL 5

#define MIN_RRM_CELL_PACKETDELAYBUDGET 0
#define MAX_RRM_CELL_PACKETDELAYBUDGET 13
#define MIN_RRM_MAC_S_RARNTI 1
#define MAX_RRM_MAC_S_RARNTI 60 
#define MIN_RRM_MAC_E_RARNTI 1
#define MAX_RRM_MAC_E_RARNTI 60
#define MIN_UE_INACTIVITY_TIMER 0
#define MIN_RRM_CELL_PHICH_RESOURCE 0
#define MAX_RRM_CELL_PHICH_RESOURCE 3
#define MIN_RRM_CELL_PHICH_DURATION 0
#define MAX_RRM_CELL_PHICH_DURATION 1
#define MIN_RRM_CELL_MODIFICATION_PERIOD_COEFF 0
#define MAX_RRM_CELL_MODIFICATION_PERIOD_COEFF 3

#define MIN_RRM_CELL_DEFAULT_PAGING_CYCLE 0
#define MAX_RRM_CELL_DEFAULT_PAGING_CYCLE 3
#define MIN_RRM_CELL_NB 0
#define MAX_RRM_CELL_NB 7
#define MIN_RRM_CELL_UL_CYCLIC_PREFIX_LENGTH 0
#define MAX_RRM_CELL_UL_CYCLIC_PREFIX_LENGTH 1

#define MIN_RRM_CELL_ADDITIONAL_SPECTRUM_EMISSION 1
#define MAX_RRM_CELL_ADDITIONAL_SPECTRUM_EMISSION 32
#define MIN_RRM_CELL_TIME_ALIGNMENT_TIMER_COMMON 0
#define MAX_RRM_CELL_TIME_ALIGNMENT_TIMER_COMMON 7
#define MIN_RRM_CELL_ALLOWED_MEASUREMENT_BW 0
#define MAX_RRM_CELL_ALLOWED_MEASUREMENT_BW 5
#define MIN_RRM_CELL_PRESENCE_ANTENNA_PORT1 0
#define MAX_RRM_CELL_PRESENCE_ANTENNA_PORT1 1
#define MIN_RRM_CELL_S_INTRA_SEARCH_P_R9 0
#define MAX_RRM_CELL_S_INTRA_SEARCH_P_R9 31

#define MIN_RRM_CELL_S_INTRA_SEARCH_Q_R9 0
#define MAX_RRM_CELL_S_INTRA_SEARCH_Q_R9 31
#define MIN_RRM_CELL_S_NON_INTRA_SEARCH_P_R9 0
#define MAX_RRM_CELL_S_NON_INTRA_SEARCH_P_R9 31
#define MIN_RRM_CELL_S_NON_INTRA_SEARCH_Q_R9 0
#define MAX_RRM_CELL_S_NON_INTRA_SEARCH_Q_R9 31
#define MIN_RRM_CELL_Q_QUAL_MIN_R9 -34 
#define MAX_RRM_CELL_Q_QUAL_MIN_R9 -3
#define MIN_RRM_CELL_THRESH_SERVING_LOWQ_R9 0
#define MAX_RRM_CELL_THRESH_SERVING_LOWQ_R9 31
#define MIN_RRM_CELL_PHY_CELL_ID_START 0
#define MAX_RRM_CELL_PHY_CELL_ID_START 503

#define MIN_RRM_CELL_PHY_CELL_ID_RANGE 0
#define MAX_RRM_CELL_PHY_CELL_ID_RANGE 13
#define MIN_RRM_CELL_MAX_NUM_GBR_DRBS_PER_UE 1
#define MAX_RRM_CELL_MAX_NUM_GBR_DRBS_PER_UE 8
#define MIN_RRM_CELL_MAX_NUM_NON_GBR_DRBS_PER_UE 1
#define MAX_RRM_CELL_MAX_NUM_NON_GBR_DRBS_PER_UE 8

#define MIN_RRM_CELL_DL_PRB_BUDGET 0
#define MAX_RRM_CELL_DL_PRB_BUDGET 100
#define MIN_RRM_CELL_UL_PRB_BUDGET 0
#define MAX_RRM_CELL_UL_PRB_BUDGET 100
#define MIN_RRM_CELL_DL_PRB_BUDGET_GBR 0
#define MAX_RRM_CELL_DL_PRB_BUDGET_GBR 100
#define MIN_RRM_CELL_UL_PRB_BUDGET_GBR 0
#define MAX_RRM_CELL_UL_PRB_BUDGET_GBR 100

#define MIN_RRM_CELL_DL_PRB_BUDGET_NGBR 0
#define MAX_RRM_CELL_DL_PRB_BUDGET_NGBR 100
#define MIN_RRM_CELL_UL_PRB_BUDGET_NGBR 0
#define MAX_RRM_CELL_UL_PRB_BUDGET_NGBR 100

/*SPR 18654 START*/
#define MIN_RRM_CELL_DL_BW_FOR_CONTROL_PRBS 0
#define MAX_RRM_CELL_DL_BW_FOR_CONTROL_PRBS 100
#define MIN_RRM_CELL_RB_FACTOR_FOR_BITRATE 1
#define MAX_RRM_CELL_RB_FACTOR_FOR_BITRATE 100
/*SPR 18654 END*/
/* SPR-18198 START */
#define MAX_RRM_CELL_UL_PRB_BUDGET_NGBR_GBR 100 
#define MAX_RRM_CELL_DL_PRB_BUDGET_NGBR_GBR 100
/* SPR-18198 END */

#define MIN_RRM_CELL_ACCESSMODE 0
#define MAX_RRM_CELL_ACCESSMODE 2

#define OAM_MAX_UES_SERVED_MIN_VAL                            1
#define OAM_MAX_UES_SERVED_MAX_VAL                          255
#define OAM_CSGID_MIN_VAL                                     0
#define OAM_CSGID_MAX_VAL                             134217727
#define MIN_RRM_CELL_MAXCSGMEMBERS 0
#define MAX_RRM_CELL_MAXCSGMEMBERS 255
#define MIN_RRM_CELL_MAXNONCSGMEMBERS 0
#define MAX_RRM_CELL_MAXNONCSGMEMBERS 255
#define MIN_RRM_CELL_IMMEDIATE_START_NEEDED 0
#define MAX_RRM_CELL_IMMEDIATE_START_NEEDED 1
#define MIN_RRM_LOG_ENABLE_DISABLE_LOG_ON_OFF 0
#define MAX_RRM_LOG_ENABLE_DISABLE_LOG_ON_OFF 1
#define MIN_RRM_LOG_ENABLE_DISABLE_LOG_LEVEL 0
#define MAX_RRM_LOG_ENABLE_DISABLE_LOG_LEVEL 6 

#define MIN_RRM_SET_LOG_LEVEL_REQ_INT_MODULE_ID 21
#define MAX_RRM_SET_LOG_LEVEL_REQ_INT_MODULE_ID 24
#define MIN_RRM_SET_LOG_LEVEL_REQ_LOG_LEVEL 0 
#define MAX_RRM_SET_LOG_LEVEL_REQ_LOG_LEVEL 6

#define MIN_RRM_OAM_MAX_NUM_PATHLOSS_TO_SINR_COUNT 1 
#define MAX_RRM_OAM_MAX_NUM_PATHLOSS_TO_SINR_COUNT 2
#define MIN_RRM_OAM_NUM_DCI_SIZE_CATEGORY  1
#define MAX_RRM_OAM_NUM_DCI_SIZE_CATEGORY  3
#define MIN_RRM_OAM_MAX_CQI_VALUE 1
#define MAX_RRM_OAM_MAX_CQI_VALUE 15
#define MIN_RRM_OAM_AGGREGATION_LEVEL_POWER_OFFSET_COUNT 1
#define MAX_RRM_OAM_AGGREGATION_LEVEL_POWER_OFFSET_COUNT 2
#define MIN_RRM_OAM_MAX_CC_REGION 1
#define MAX_RRM_OAM_MAX_CC_REGION 2

#define MIN_RESELECT_UTRA 0
#define MAX_RESELECT_UTRA 7 

#define MIN_TRESELECTIONUTRASFHIGH 0
#define MAX_TRESELECTIONUTRASFHIGH 3

#define MIN_TRESELECTIONUTRASFMEDIUM 0
#define MAX_TRESELECTIONUTRASFMEDIUM 3

#define MIN_UTRACARRIERARFCN 0
#define MAX_UTRACARRIERARFCN 16383

#define MIN_QRXLEVMIN -60 
#define MAX_QRXLEVMIN -13

#define MIN_QQUALMIN -24 
#define MAX_QQUALMIN  0

#define MIN_CELLRESELECTIONPRIORITY 0
#define MAX_CELLRESELECTIONPRIORITY 7

#define MIN_THRESHXHIGH 0
#define MAX_THRESHXHIGH 31

#define MIN_THRESHXLOW 0
#define MAX_THRESHXLOW 31

#define MIN_PMAXUTRA -50
#define MAX_PMAXUTRA 33
#define MIN_THRESH_X_HIGHQ_R9 0
#define MAX_THRESH_X_HIGHQ_R9 31
#define MIN_THRESH_X_LOWQ_R9 0
#define MAX_THRESH_X_LOWQ_R9 31

#define MIN_RRC_LOGLEVEL 0
#define MAX_RRC_LOGLEVEL 6 
#define MIN_RRC_NUMALGORITHMS 1
#define MAX_RRC_NUMALGORITHMS 3
#define MIN_RRC_ALGORITHMS 0
#define MAX_RRC_ALGORITHMS 2

#define MIN_RRC1_NUMALGORITHMS 1
#define MAX_RRC1_NUMALGORITHMS 3
#define MIN_RRC1_ALGORITHMS 0
#define MAX_RRC1_ALGORITHMS 2


#define MIN_TIMERINFO0_TIMERVAL 50
#define MAX_TIMERINFO0_TIMERVAL 65535
#define MIN_TIMERINFO1_TIMERID 0
#define MAX_TIMERINFO1_TIMERID 41
#define MIN_TIMERINFO1_TIMERVAL 50
#define MAX_TIMERINFO1_TIMERVAL 65535
#define MIN_TIMERINFO2_TIMERID 0
#define MAX_TIMERINFO2_TIMERID 41
#define MIN_TIMERINFO2_TIMERVAL 50
#define MAX_TIMERINFO2_TIMERVAL 65535
#define MIN_TIMERINFO3_TIMERID 0
#define MAX_TIMERINFO3_TIMERID 41
#define MIN_TIMERINFO3_TIMERVAL 50
#define MAX_TIMERINFO3_TIMERVAL 65535
#define MIN_TIMERINFO4_TIMERID 0
#define MAX_TIMERINFO4_TIMERID 41
#define MIN_TIMERINFO4_TIMERVAL 50
#define MAX_TIMERINFO4_TIMERVAL 65535
#define MIN_TIMERINFO5_TIMERID 0
#define MAX_TIMERINFO5_TIMERID 41
#define MIN_TIMERINFO5_TIMERVAL 50
#define MAX_TIMERINFO5_TIMERVAL 65535
#define MIN_TIMERINFO6_TIMERID 0
#define MAX_TIMERINFO6_TIMERID 41
#define MIN_TIMERINFO6_TIMERVAL 50
#define MAX_TIMERINFO6_TIMERVAL 65535
#define MIN_TIMERINFO7_TIMERID 0
#define MAX_TIMERINFO7_TIMERID 41
#define MIN_TIMERINFO7_TIMERVAL 50
#define MAX_TIMERINFO7_TIMERVAL 65535
#define MIN_TIMERINFO8_TIMERID 0
#define MAX_TIMERINFO8_TIMERID 41
#define MIN_TIMERINFO8_TIMERVAL 50
#define MAX_TIMERINFO8_TIMERVAL 65535
#define MIN_TIMERINFO9_TIMERID 0
#define MAX_TIMERINFO9_TIMERID 41
#define MIN_TIMERINFO9_TIMERVAL 50
#define MAX_TIMERINFO9_TIMERVAL 65535
#define MIN_TIMERINFO10_TIMERID 0
#define MAX_TIMERINFO10_TIMERID 41
#define MIN_TIMERINFO10_TIMERVAL 50
#define MAX_TIMERINFO10_TIMERVAL 65535
#define MIN_TIMERINFO11_TIMERID 0
#define MAX_TIMERINFO11_TIMERID 41
#define MIN_TIMERINFO11_TIMERVAL 50
#define MAX_TIMERINFO11_TIMERVAL 65535
#define MIN_TIMERINFO12_TIMERID 0
#define MAX_TIMERINFO12_TIMERID 41
#define MIN_TIMERINFO12_TIMERVAL 50
#define MAX_TIMERINFO12_TIMERVAL 65535
#define MIN_TIMERINFO13_TIMERID 0
#define MAX_TIMERINFO13_TIMERID 41
#define MIN_TIMERINFO13_TIMERVAL 50
#define MAX_TIMERINFO13_TIMERVAL 65535
#define MIN_TIMERINFO14_TIMERID 0
#define MAX_TIMERINFO14_TIMERID 41
#define MIN_TIMERINFO14_TIMERVAL 50
#define MAX_TIMERINFO14_TIMERVAL 65535
#define MIN_TIMERINFO15_TIMERID 0
#define MAX_TIMERINFO15_TIMERID 41
#define MIN_TIMERINFO15_TIMERVAL 50
#define MAX_TIMERINFO15_TIMERVAL 65535
#define MIN_TIMERINFO16_TIMERID 0
#define MAX_TIMERINFO16_TIMERID 41
#define MIN_TIMERINFO16_TIMERVAL 50
#define MAX_TIMERINFO16_TIMERVAL 65535
#define MIN_TIMERINFO17_TIMERID 0
#define MAX_TIMERINFO17_TIMERID 41
#define MIN_TIMERINFO17_TIMERVAL 50
#define MAX_TIMERINFO17_TIMERVAL 65535
#define MIN_TIMERINFO18_TIMERID 0
#define MAX_TIMERINFO18_TIMERID 41
#define MIN_TIMERINFO18_TIMERVAL 50
#define MAX_TIMERINFO18_TIMERVAL 65535
#define MIN_TIMERINFO19_TIMERID 0
#define MAX_TIMERINFO19_TIMERID 41
#define MIN_TIMERINFO19_TIMERVAL 50
#define MAX_TIMERINFO19_TIMERVAL 65535
#define MIN_TIMERINFO20_TIMERID 0
#define MAX_TIMERINFO20_TIMERID 41
#define MIN_TIMERINFO20_TIMERVAL 50
#define MAX_TIMERINFO20_TIMERVAL 65535
#define MIN_TIMERINFO21_TIMERID 0
#define MAX_TIMERINFO21_TIMERID 41
#define MIN_TIMERINFO21_TIMERVAL 50
#define MAX_TIMERINFO21_TIMERVAL 65535
#define MIN_TIMERINFO22_TIMERID 0
#define MAX_TIMERINFO22_TIMERID 41
#define MIN_TIMERINFO22_TIMERVAL 50
#define MAX_TIMERINFO22_TIMERVAL 65535
#define MIN_TIMERINFO23_TIMERID 0
#define MAX_TIMERINFO23_TIMERID 41
#define MIN_TIMERINFO23_TIMERVAL 50
#define MAX_TIMERINFO23_TIMERVAL 65535
#define MIN_TIMERINFO24_TIMERID 0
#define MAX_TIMERINFO24_TIMERID 41
#define MIN_TIMERINFO24_TIMERVAL 50
#define MAX_TIMERINFO24_TIMERVAL 65535
#define MIN_TIMERINFO25_TIMERID 0
#define MAX_TIMERINFO25_TIMERID 41
#define MIN_TIMERINFO25_TIMERVAL 50
#define MAX_TIMERINFO25_TIMERVAL 65535
#define MIN_TIMERINFO26_TIMERID 0
#define MAX_TIMERINFO26_TIMERID 41
#define MIN_TIMERINFO26_TIMERVAL 50
#define MAX_TIMERINFO26_TIMERVAL 65535
#define MIN_TIMERINFO27_TIMERID 0
#define MAX_TIMERINFO27_TIMERID 41
#define MIN_TIMERINFO27_TIMERVAL 50
#define MAX_TIMERINFO27_TIMERVAL 65535
#define MIN_TIMERINFO28_TIMERID 0
#define MAX_TIMERINFO28_TIMERID 41
#define MIN_TIMERINFO28_TIMERVAL 50
#define MAX_TIMERINFO28_TIMERVAL 65535
#define MIN_TIMERINFO29_TIMERID 0
#define MAX_TIMERINFO29_TIMERID 41
#define MIN_TIMERINFO29_TIMERVAL 50
#define MAX_TIMERINFO29_TIMERVAL 65535
#define MIN_TIMERINFO30_TIMERID 0
#define MAX_TIMERINFO30_TIMERID 41
#define MIN_TIMERINFO30_TIMERVAL 50
#define MAX_TIMERINFO30_TIMERVAL 65535
#define MIN_TIMERINFO31_TIMERID 0
#define MAX_TIMERINFO31_TIMERID 41
#define MIN_TIMERINFO31_TIMERVAL 50
#define MAX_TIMERINFO31_TIMERVAL 65535
#define MIN_TIMERINFO32_TIMERID 0
#define MAX_TIMERINFO32_TIMERID 41
#define MIN_TIMERINFO32_TIMERVAL 50
#define MAX_TIMERINFO32_TIMERVAL 65535

#define MIN_TIMERINFO33_TIMERID 0
#define MAX_TIMERINFO33_TIMERID 41
#define MIN_TIMERINFO33_TIMERVAL 50
#define MAX_TIMERINFO33_TIMERVAL 65535
#define MIN_TIMERINFO34_TIMERID 0
#define MAX_TIMERINFO34_TIMERID 41
#define MIN_TIMERINFO34_TIMERVAL 50
#define MAX_TIMERINFO34_TIMERVAL 65535
 
#define MIN_TIMERINFO35_TIMERID 0
#define MAX_TIMERINFO35_TIMERID 41
#define MIN_TIMERINFO35_TIMERVAL 50
#define MAX_TIMERINFO35_TIMERVAL 65535
#define MIN_TIMERINFO36_TIMERID 0
#define MAX_TIMERINFO36_TIMERID 41
#define MIN_TIMERINFO36_TIMERVAL 50
#define MAX_TIMERINFO36_TIMERVAL 65535
#define MIN_TIMERINFO37_TIMERID 0
#define MAX_TIMERINFO37_TIMERID 41
#define MIN_TIMERINFO37_TIMERVAL 50
#define MAX_TIMERINFO37_TIMERVAL 65535
#define MIN_TIMERINFO38_TIMERID 0
#define MAX_TIMERINFO38_TIMERID 41
#define MIN_TIMERINFO38_TIMERVAL 50
#define MAX_TIMERINFO38_TIMERVAL 65535
#define MIN_TIMERINFO39_TIMERID 0
#define MAX_TIMERINFO39_TIMERID 41
#define MIN_TIMERINFO39_TIMERVAL 50
#define MAX_TIMERINFO39_TIMERVAL 65535
#define MIN_TIMERINFO40_TIMERID 0
#define MAX_TIMERINFO40_TIMERID 41
#define MIN_TIMERINFO40_TIMERVAL 50
#define MAX_TIMERINFO40_TIMERVAL 65535
#define MIN_TIMERINFO41_TIMERID 0
#define MAX_TIMERINFO41_TIMERID 41
#define MIN_TIMERINFO41_TIMERVAL 50
#define MAX_TIMERINFO41_TIMERVAL 65535
#define MIN_TIMERINFO42_TIMERID 0
#define MAX_TIMERINFO42_TIMERID 41
#define MIN_TIMERINFO42_TIMERVAL 50
#define MAX_TIMERINFO42_TIMERVAL 65535



#define MIN_RRC_NONBROADCASTSFNOFFSET 0
#define MAX_RRC_NONBROADCASTSFNOFFSET 1023

#define MIN_RRC_MAXCELLRECONFIGRETRYCOUNT 0
#define MAX_RRC_MAXCELLRECONFIGRETRYCOUNT 255
#define MIN_RRC_LENGTHVENDORNAME 0
#define MAX_RRC_LENGTHVENDORNAME 9
#define MIN_RRC_VENDORNAME 0 
#define MAX_RRC_VENDORNAME 255  
#define MIN_ADVANCE_LOGGING_CONFIG_OPTION 0
/* SPR 20585 Fix Start */
#define MAX_ADVANCE_LOGGING_CONFIG_OPTION 3
/* SPR 20585 Fix End */
/* SHM ADD */
#define MIN_SHARED_MEM_SIZE 3
#define MAX_SHARED_MEM_SIZE 255

#define MIN_LOG_FILE_SIZE 3
#define MAX_LOG_FILE_SIZE 255

 
#define MIN_S1AP_LOGLEVEL 0
#define MAX_S1AP_LOGLEVEL 6
#define MIN_S1AP_NUMMME 1
#define MAX_S1AP_NUMMME 3
#define MIN_S1AP_MME_NUMIPADDR 1
#define MAX_S1AP_MME_NUMIPADDR 3
#define MIN_S1AP_NUMSTREAM 2
#define MAX_S1AP_NUMSTREAM 10
#define MIN_S1AP_DRX 0
#define MAX_S1AP_DRX 3
#define MIN_S1AP_NUMCSG 1
#define MAX_S1AP_NUMCSG 256


#define MIN_S1AP_HEARTBEATTIMER 0
#define MAX_S1AP_HEARTBEATTIMER 0 
#define MIN_S1AP_RELOFMME 0
#define MAX_S1AP_RELOFMME 2
#define MIN_S1AP_NUMIPADDR 1
#define MAX_S1AP_NUMIPADDR 3

#define MIN_S1AP_PORT 0 
#define MAX_S1AP_PORT 65535
#define MIN_X2AP_LOGLEVEL 0
#define MAX_X2AP_LOGLEVEL 6
#define MIN_X2AP_NUMIPADDR 1
#define MAX_X2AP_NUMIPADDR 3
#define MIN_X2AP_IPADDR 1
#define MAX_X2AP_IPADDR 3


#define MIN_X2AP_PORT 0
#define MAX_X2AP_PORT 65535

/* LWA changes start*/
/* LWA changes end*/

#define MIN_SON_INIT_LOG_ON_OFF 0
#define MAX_SON_INIT_LOG_ON_OFF 1
#define MIN_SON_INIT_LOG_LEVEL 0
#define MAX_SON_INIT_LOG_LEVEL 6 

#define MIN_EGTPU_NUMMYADDR4 1
#define MIN_EGTPU_NUMMYADDR6 1

/* SPR 10597 Changes Start */
#define MIN_EGTPU_ECHO_REQ_TIMER 60
#define MAX_EGTPU_ECHO_REQ_TIMER 0xFFFFFFFF
/* SPR 10597 Changes End */

#define MIN_MAX_RETRY 1
#define MAX_MAX_RETRY 5

/* CSR 00070293 FIX START */
#define OAM_INVALID_STR "-500"
#define OAM_INVALID_VALUE -500
/* CSR 00070293 FIX END */

#define MIN_RRM_CELL_INDIV_OFFSET 0
#define MAX_RRM_CELL_INDIV_OFFSET 30
#define MIN_RRM_CELL_SIG_TX -60
#define MAX_RRM_CELL_SIG_TX 50
#define MIN_RRM_Q_QUAL_R9 -34
#define MAX_RRM_Q_QUAL_R9 -3
#define MIN_NUM_MNC_DIGIT 2
#define MAX_NUM_MNX_DIGIT 3
#define MIN_INT_MODULE_ID 21
#define MAX_INT_MODULE_ID 24
#define MIN_TRELOCOVERALL 0
#define MAX_TRELOCOVERALL 300
#define MIN_DEFAULT_PAGING_DRX 0
#define MAX_DEFAULT_PAGING_DRX 3
#define MIN_NEIGH_CELL_CONFIG 0
#define MAX_NEIGH_CELL_CONFIG 3
#define MIN_NUM_UTRANFDDFREQENTRIES 1
#define MAX_NUM_UTRANFDDFREQENTRIES 16

#define MIN_LOWQ_R9 0
#define MAX_LOWQ_R9 31
#define MIN_HIGHQ_R9 0
#define MAX_HIGHQ_R9 31
#define MIN_Q_QUAL_MIN_R9 -34
#define MAX_Q_QUAL_MIN_R9 -3
#define MIN_RESELECTION_EUTRASFHIGH 0
#define MAX_RESELECTION_EUTRASFHIGH 3
#define MIN_PHY_CELL_ID 0
#define MAX_PHY_CELL_ID 503
#define MIN_Q_OFFSET_CELL 0
#define MAX_Q_OFFSET_CELL 30
#define MIN_BLACKLISTED 0
#define MAX_BLACKLISTED 1
#define MIN_NUM_VALID_INTER_FREQ 0
#define MAX_NUM_VALID_INTER_FREQ 32
#define MIN_EUTRA_CARRIER_ARFCN 0
#define MAX_EUTRA_CARRIER_ARFCN 65535


#define MIN_T300 0
#define MAX_T300 7
#define MIN_T301 0
#define MAX_T301 7
#define MIN_T302 0
#define MAX_T302 7
#define MIN_T304EUTRA 0
#define MAX_T304EUTRA 6
#define MIN_T304IRAT 0
#define MAX_T304IRAT 6
#define MIN_T310 0
#define MAX_T310 6
#define MIN_T311 0
#define MAX_T311 6
#define MIN_T320 0
#define MAX_T320 6
#define MIN_N310 0
#define MAX_N310 7
#define MIN_N311 0
#define MAX_N311 7
#define MIN_FREQBANDINDICATOR 1
/* LAA Rel13 Code + */ 
/* LAA Rel13 Code - */ 
#define MAX_FREQBANDINDICATOR 43 /* Modified from 40 to 43 for CSR 00056169 */
/* LAA Rel13 Code + */ 
/* LAA Rel13 Code - */ 

/* SPR 14091 FIX START*/
#define MAX_FDD_BAND 29
#define MIN_TDD_BAND 33
/* SPR 14091 FIX END*/
/* SPR-10651 FIX Start */
#define MIN_TDD_FREQBANDINDICATOR 33
#define MAX_FDD_FREQBANDINDICATOR 25
/* SPR-10651 FIX End */
#define MIN_DLEARFCN 0
#define MAX_DLEARFCN 65535
#define MIN_DLBANDWIDTH 0
#define MAX_DLBANDWIDTH 5
#define MIN_REFERENCESIGNALPOWER -60
#define MAX_REFERENCESIGNALPOWER 50
#define MIN_PHYCELLID 0
#define MAX_PHYCELLID 503
  /*SPR 19329 change start*/
#define MIN_PSCHPOWEROFFSET -1280
#define MAX_PSCHPOWEROFFSET 60
#define MIN_SSCHPOWEROFFSET -1280
#define MAX_SSCHPOWEROFFSET 60
  /*SPR 19329 change end*/
#define MIN_PBCHPOWEROFFSET -60
#define MAX_PBCHPOWEROFFSET 40

#define MIN_TOTAL_BACKHAUL_CAPACITY 1
#define MAX_TOTAL_BACKHAUL_CAPACITY 20000
#define MIN_CAPACITY_THRESHOLD 0
#define MAX_CAPACITY_THRESHOLD 100
#define MIN_PACKET_ERROR_LOSS_RATE 0
#define MAX_PACKET_ERROR_LOSS_RATE 6
#define MIN_FREQ_PRIORITY_COUNT 1
#define MAX_FREQ_PRIORITY_COUNT 8
#define MIN_EUTRAN_CARRIER_FREQ 0
#define MAX_EUTRAN_CARRIER_FREQ 65535
#define MIN_Cell_Reselection_Priority 0
#define MAX_Cell_Reselection_Priority 7
#define MIN_UTRAN_CARRIER_FREQ 0
#define MAX_UTRAN_CARRIER_FREQ 16383
#define MIN_PRIORITY_CFG_COUNT 1
#define MAX_PRIORITY_CFG_COUNT 3
#define MIN_BUCKET 0
#define MAX_BUCKET 7
#define MIN_CountWraparoundThresholdValue 0
#define MAX_CountWraparoundThresholdValue 4294967295LL
#define MIN_GBR_LIMIT 1
#define MAX_GBR_LIMIT 20000
#define MIN_DSCP 0
#define MAX_DSCP 63
#define MIN_UES_TO_BE_SCHEDULED_DL                      0
#define MAX_UES_TO_BE_SCHEDULED_DL                      8
#define MIN_UES_TO_BE_SCHEDULED_UL                      0
#define MAX_UES_TO_BE_SCHEDULED_UL                      8

/* 4x4 MIMO SUPPORT ADD Start */
#define MIN_CQI_THRESHOLD_LAYER3                    0
#define MAX_CQI_THRESHOLD_LAYER3                    15
#define MIN_CQI_THRESHOLD_CELL_EDGE_LAYER3          0
#define MAX_CQI_THRESHOLD_CELL_EDGE_LAYER3          15
#define MIN_CQI_THRESHOLD_LAYER4                    0
#define MAX_CQI_THRESHOLD_LAYER4                    15
#define MIN_CQI_THRESHOLD_CELL_EDGE_LAYER4          0
#define MAX_CQI_THRESHOLD_CELL_EDGE_LAYER4          15
/* 4x4 MIMO SUPPORT ADD End */

#define MIN_FILTER_COEFFICIENT_RSRP 0
#define MAX_FILTER_COEFFICIENT_RSRP 15
#define MIN_FILTER_COEFFICIENT_RSRQ 0
#define MAX_FILTER_COEFFICIENT_RSRQ 15
#define MIN_A1_THRESHOLD_RSRP 0
#define MAX_A1_THRESHOLD_RSRP 97
#define MIN_A1_THRESHOLD_RSRQ 0
#define MAX_A1_THRESHOLD_RSRQ 34
#define MIN_A2_THRESHOLD_RSRP 0
#define MAX_A2_THRESHOLD_RSRP 97
#define MIN_A2_THRESHOLD_RSRQ 0
#define MAX_A2_THRESHOLD_RSRQ 34
#define MIN_A3_OFFSET -30
#define MAX_A3_OFFSET 30
#define MIN_REPORT_ON_LEAVE 0
#define MAX_REPORT_ON_LEAVE 1
#define MIN_A4_THRESHOLD_RSRP 0
#define MAX_A4_THRESHOLD_RSRP 97
#define MIN_A4_THRESHOLD_RSRQ 0
#define MAX_A4_THRESHOLD_RSRQ 34
#define MIN_A5_THRESHOLD_1RSRP 0
#define MAX_A5_THRESHOLD_1RSRP 97
#define MIN_A5_THRESHOLD_1RSRQ 0
#define MAX_A5_THRESHOLD_1RSRQ 34
#define MIN_A5_THRESHOLD_2RSRP 0
#define MAX_A5_THRESHOLD_2RSRP 97

#define MIN_A5_THRESHOLD_2RSRQ 0
#define MAX_A5_THRESHOLD_2RSRQ 34
#define MIN_HYSTERESIS 0
#define MAX_HYSTERESIS 30
#define MIN_TIME_TO_TRIGGER 0
#define MAX_TIME_TO_TRIGGER 15
#define MIN_TRIGGER_QUANTITY 0
#define MAX_TRIGGER_QUANTITY 1
#define MIN_REPORT_QUANTITY 0
#define MAX_REPORT_QUANTITY 2
#define MIN_REPORT_INTERVAL 0
#define MAX_REPORT_INTERVAL 12
#define MIN_REPORT_AMOUNT 0
#define MAX_REPORT_AMOUNT 7
#define DELTAFPUCCHFORMAT1_1 -2
#define DELTAFPUCCHFORMAT1_2  0 
#define DELTAFPUCCHFORMAT1_3  2
#define DELTAFPUCCHFORMAT1b_1 1
#define DELTAFPUCCHFORMAT1b_2  3
#define DELTAFPUCCHFORMAT1b_3  5
#define DELTAFPUCCHFORMAT2_1 -2
#define DELTAFPUCCHFORMAT2_2  0
#define DELTAFPUCCHFORMAT2_3  2
#define DELTAFPUCCHFORMAT2a_1 -2
#define DELTAFPUCCHFORMAT2a_2  0
#define DELTAFPUCCHFORMAT2a_3  2
#define DELTAFPUCCHFORMAT2b_1 -2
#define DELTAFPUCCHFORMAT2b_2  0
#define DELTAFPUCCHFORMAT2b_3  2
#define DELTAPREAMBLEMSG3_1  -1
#define DELTAPREAMBLEMSG3_2  6
#define MIN_GROUPASSIGNMENTPUSCH 0
#define MAX_GROUPASSIGNMENTPUSCH 29
#define MIN_CYCLICSHIFT 0
#define MAX_CYCLICSHIFT 7

#define MIN_RRM_CELL_GERANFREQ_BANDINDICATOR   0
#define MAX_RRM_CELL_GERANFREQ_BANDINDICATOR   1
#define MIN_RRM_CELL_TRESELECTIONGERANSFMEDIUM 0
#define MAX_RRM_CELL_TRESELECTIONGERANSFMEDIUM 3
#define MIN_RRM_CELL_TRESELECTIONGERANSFHIGH 0
#define MAX_RRM_CELL_TRESELECTIONGERANSFHIGH 3
#define MIN_TRESELECTIONCDMA2000SFHIGH 0
#define MAX_TRESELECTIONCDMA2000SFHIGH 3
#define MIN_TRESELECTIONCDMA2000SFMEDIUM 0
#define MAX_TRESELECTIONCDMA2000SFMEDIUM 3
#define MIN_PS_HO_ENABLED 0
#define MAX_PS_HO_ENABLED 1
#define LIBXML_SCHEMAS_ENABLED
#define MIN_MAXUESSERVED 1
#define MAX_MAXUESSERVED 255


#define GERANFREQ_BANDINDICATOR_COUNT 2
/* CSR 00070293 FIX START */
#define GSM_BANDINDICATOR_COUNT 2
#define TRESELECTIONSF_COUNT    4
/* SPR 13179 FIX START */
#define OAM_NR_BLACKLISTED_VAL_COUNT                    2
/* SPR 13179 FIX END */
/* CSR 00070293 FIX END */
#define NUM_ANTENNA_COUNT            3
#define MAX_STRING_LENGTH_TR196 128
#define HOPPING_MODE_COUNT            2
#define QOS_LOGICALCHANNELTYPE_COUNT            2
/* SPR 12987 START */
#define MAX_SUPPORTED_RAT	5
/* SPR 12987 END */
#define DL_BANDWIDTH_COUNT            6
#define UL_BANDWIDTH_COUNT            6
#define SRS_BANDWIDTH_CONFIG_COUNT    8
#define DELTA_PUCCH_SHIFT_COUNT       3
#define ACCESS_MODE_COUNT             3
#define ALPHA_COUNT                   8
#define NUMCONSECUTIVEPRSSUBFAMES_COUNT 4
#define NUMBEROFRAPREAMBLES_COUNT     16
#define SIZEOFRAGROUPA_COUNT          15
#define MESSAGESIZEGROUPA_COUNT             4
#define MESSAGEPOWEROFFSETGROUPB_COUNT      8
#define POWERRAMPINGSTEP_COUNT              4
#define PREAMBLEINITIALRECEIVEDTARGETPOWER_COUNT  16
#define PREAMBLETRANSMAX_COUNT              11
#define RESPONSEWINDOWSIZE_COUNT       8 
#define CONTENTIONRESOLUTIONTIMER_COUNT      8
/*SPR 21643 Start*/
/* SPR 21737 Start */
#define ONDURATIONTIMER_COUNT               17
#define DRXINACTIVITYTIMER_COUNT            24
#define DRXRETRANSMISSIONTIMER_COUNT         9
#define LONGDRXCYCLE_COUNT                  17

#ifdef ENDC_ENABLED
#define LONGDRXCYCLE_COUNT_R15              19
#define MIN_MAX_MEAS_FREQ_SCG_NR 1
#define MAX_MAX_MEAS_FREQ_SCG_NR 32
#define MIN_MAX_MEAS_ID_SCG_NR   1
#define MAX_MAX_MEAS_ID_SCG_NR   62
#endif

/*SPR 21643 Stop*/
/* SPR 21737 End */
#define SHORTDRXCYCLE_COUNT                 16
#define TPOLLRETRANSMIT_COUNT               55
#define POLLPDU_COUNT                        8
#define POLLBYTE_COUNT                      15
#define MAXRETXTHRESHOLD_COUNT               8
#define TREORDERING_COUNT                   31
#define TSTATUSPROHIBIT_COUNT               56
/*spr_22058_changes_start*/
#define PERIODIC_PHR_COUNT                  8
#define PROHIBIT_PHR_COUNT                  8
#define DL_PATHLOSS_COUNT                   4
/*spr_22058_changes_end*/
#define INTERFREQ_CARRIER_QOFFSETFREQ_COUNT 31
#define INTRAFREQ_TRESELECTIONEUTRASFMEDIUM_COUNT 4
#define INTRAFREQ_TRESELECTIONEUTRASFHIGH_COUNT   4
#define RRCTIMERS_T300                       8
#define RRCTIMERS_T301                       8
#define RRCTIMERS_T302                       8
#define RRCTIMERS_T304EUTRA                  7
#define RRCTimers_T304IRAT                   7
#define RRCTimers_T310                       7
#define RRCTimers_T311                       7
#define RRCTimers_T320                       7


#define RRCTimers_N310                       8
#define RRCTimers_N311                       8
#define EPC_QOS_PACKETDELAYBUDGET_COUNT     14
#define PDSCH_PA_COUNT                       8
/*SPR 21643 Start*/
#define DRXSHORTCYCLE_COUNT                 17
/*SPR 21643 Stop*/
#define SUPP_SYSTEM_COUNT                    4
#define MIN_MAX_UES_TO_BE_SCHEDULED_DL	     1
/*SPR 19119 Fix Start*/
#define MAX_MAX_UES_TO_BE_SCHEDULED_DL	    16
#define MIN_MAX_UES_TO_BE_SCHEDULED_UL	     1
#define MAX_MAX_UES_TO_BE_SCHEDULED_UL	    16
#define MIN_MAX_UE_SCHEDULED_UL		1	
#define MAX_MAX_UE_SCHEDULED_UL		16
#define MIN_UE_SCHEDULED_DL		1
#define MAX_UE_SCHEDULED_DL		16
/*SPR 19119 Fix end*/
#define MIN_UE_SCHEDULING_HYSTERESIS	1
#define MAX_UE_SCHEDULING_HYSTERESIS	14
/* SPR 13768 Changes Start */
#define MIN_DL_NUM_HARQ_PROCESS		         1
#define MAX_DL_NUM_HARQ_PROCESS		         15
/* SPR 13768 Changes End */
/* SPR 19309 + */
#define MIN_POSSIBLE_CFI_VALUE              1
#define MAX_POSSIBLE_CFI_VALUE              4
/* SPR 19309 - */
/* SPR 19310 + */
#define MIN_POSSIBLE_EUL_VALUE              0
#define MAX_POSSIBLE_EUL_VALUE              65535
/* SPR 19310 - */
#define NR_LIST oam_prov_req.oam_son_req.oam_son_nr_add_req.nr_list
#define NR_LIST_IN_USE(i) oam_prov_req.oam_son_req.neigh_list_in_use[i]
#define OAM_NMM_SCAN_DATA_FILE          "../cfg/networkScanDataModel.xml"
#define OAM_MAX_PCI_TDD_VALUE               127
/* Start: CSR 00058940 */
#define OAM_P_MAX_DIFF                  10
#define OAM_REFERENCE_POWER_DIFF        10
#define OAM_MIN_DL_TX_P_MAX             -30
/* + SRR_17523_FIX */
#define OAM_MAX_DL_TX_P_MAX             33
/* - SRR_17523_FIX */
/*End: CSR 00058940 */


#define MAX_SPS_UES                    255
#define MIN_SPS_UES			1   
#define MIN_SUPPORTED_UL_VALUE          0
#define MAX_SUPPORTED_UL_VALUE          1
#define MIN_UL_MIN_MCS_SPS              0
#define MAX_UL_MIN_MCS_SPS              15
#define MIN_UL_MCS_MARGIN		0
#define MAX_UL_MCS_MARGIN              8
#define MIN_UL_TB_SIZE_MARGIN_SPS      0  
#define MAX_UL_TB_SIZE_MARGIN_SPS      20
#define MIN_SPS_UL_DCI_PER_TTI		1
#define MAX_SPS_UL_DCI_PER_TTI		10	
#define MIN_UL_SPS_OCCASSION_PER_TTI     0
#define MAX_UL_SPS_OCCASSION_PER_TTI	20
#define MIN_UL_SPS_RBS_PER_TTI		1
#define MAX_UL_SPS_RBS_PER_TTI 		100
#define MIN_SUPPORTED_DL_VALUE		0
#define MAX_SUPPORTED_DL_VALUE		1
#define MIN_SPS_DL_DCI_PER_TTI		1
#define MAX_SPS_DL_DCI_PER_TTI		10
#define MIN_DL_SPS_OCCASSION_PER_TTI	0
#define MAX_DL_SPS_OCCASSION_PER_TTI	20
#define MIN_DL_SPS_RBS_PER_TTI		1
#define MAX_DL_SPS_RBS_PER_TTI		100
#define MIN_DL_SPS_RBS_TTI_PER_INTERVAL	1
#define MAX_DL_SPS_RBS_TTI_PER_INTERVAL	100
#define MIN_UL_SPS_RBS_TTI_PER_INTERVAL	1
#define MAX_UL_SPS_RBS_TTI_PER_INTERVAL	100
#define MIN_DL_MCS_MARGIN		0
#define MAX_DL_MCS_MARGIN		8
#define MIN_DL_TB_SIZE_MARGIN_SPS	0
#define MAX_DL_TB_SIZE_MARGIN_SPS	20
#define MIN_DL_JITTER_HANDLING_OPTION	0
#define MAX_DL_JITTER_HANDLING_OPTION	2

/* SPR 12920 FIX START */
#define RRM_OAM_PDCP_DISCARD_TIMER_COUNT 8
#define RRM_OAM_SR_PERIODICITY_COUNT 7
#define RRM_OAM_SR_MAX_TRANSMISSION_COUNT 8
#define RRM_OAM_INTRA_FREQ_RESELECTION_COUNT 2
#define SON_OAM_PHY_CELLID_RANGE_COUNT 16
#define RRM_OAM_BW_MEAS_COUNT 6
#define RRM_OAM_SYN_SYS_TIME_COUNT 2
#define CELL_ACCESS_MODE_COUNT  4
#define CELL_HO_STATUS_COUNT  3
#define PCKT_ERR_LOSS_RATE_COUNT 7
#define ALT_BAND_CLASS_COUNT 22
/* SPR 12920 FIX END */
/* SPR 16200 FIX START */
#define OAM_FDD_MODE_ABS_PATTERN_MAX_LEN                                   40
#define OAM_TDD_MODE_SF_CONFIG0_ABS_PATTERN_MAX_LEN                        70
#define OAM_TDD_MODE_SF_CONFIG1_5_ABS_PATTERN_MAX_LEN                      20
#define OAM_TDD_MODE_SF_CONFIG6_ABS_PATTERN_MAX_LEN                        60
/* SPR 16200 FIX END */

/* SPR 13501 FIX START */
#define OAM_MIN_NUM_RBS                                       0
#define OAM_MAX_NUM_RBS                                     100

#define OAM_MIN_NUM_CE_REGION                                 0
#define OAM_MAX_NUM_CE_REGION                                 1

#define OAM_MIN_NUM_CC_REGION                                 1
#define OAM_MAX_NUM_CC_REGION                                 2

#define OAM_MIN_START_RB                                      0
#define OAM_MAX_START_RB                                     99
/*SPR 23209 FIXED START */

#define MIN_FORGETTING_FACTOR                                 1
#define MAX_FORGETTING_FACTOR                                10
#define MIN_RESETPERIOD                                       1
#define MAX_RESETPERIOD                                   65535
#define MIN_MOCN_OPERATOR                                     1
/*MOCN SPR 21985 Fix Start*/
#define MIN_RB_PERCENTAGE                                     0
/*MOCN SPR 21985 Fix End*/
#define MAX_RB_PERCENTAGE                                   100
/*SPR 23209 FIXED END */


/* SPR 13501 FIX END */
/* SPR 20653 Fix Start */
#define MIN_SRSHOPPINGENABLED                                 0
#define MAX_SRSHOPPINGENABLED                                 1
#define MIN_SRSDURATION                                       0
#define MAX_SRSDURATION                                       1
#define MIN_SRSPERIODICITY                                    0
#define MAX_SRSPERIODICITY                                    7
#define MIN_BETAOFFSETACKINDEX                                0
#define MAX_BETAOFFSETACKINDEX                               15
#define MIN_BETAOFFSETRIINDEX                                 0
#define MAX_BETAOFFSETRIINDEX                                15
#define MIN_BETAOFFSETCQIINDEX                                0
#define MAX_BETAOFFSETCQIINDEX                               15
#define MIN_PUSCHBLERTHRESHOLD                                1
#define MAX_PUSCHBLERTHRESHOLD                              100
#define MIN_SRB1_TX_BUFFERFACTOR_GBR                          1
#define MAX_SRB1_TX_BUFFERFACTOR_GBR                         4
#define MIN_SRB1_TX_BUFFERFACTOR_NGBR                          1
#define MAX_SRB1_TX_BUFFERFACTOR_NGBR                         4
#define MIN_SRB2_TX_BUFFERFACTOR_GBR                          1
#define MAX_SRB2_TX_BUFFERFACTOR_GBR                         4
#define MIN_SRB2_TX_BUFFERFACTOR_NGBR                          1
#define MAX_SRB2_TX_BUFFERFACTOR_NGBR                         4
#define MIN_DELAYED_A3_ENABLED                                0
#define MAX_DELAYED_A3_ENABLED                                1
#define MIN_A4_MEAS_CFG_ENABLED                               0
#define MAX_A4_MEAS_CFG_ENABLED                               1
#define MIN_SMEASURE                                          0
#define MAX_SMEASURE                                         97
#define MIN_ACTIVE_UE_REPORTS_TIMER                           1
#define MAX_ACTIVE_UE_REPORTS_TIMER                         255
#define MIN_PRB_PER_BPS                                       1
#define MAX_PRB_PER_BPS                                     300
#define MIN_RRM_RB_SIZE                                       0
#define MAX_RRM_RB_SIZE                                       1
#define RB_SIZE_COUNT                                         2 
#define MAX_AOA_BEAMFORMING_VECTORS                         359
#define MAX_SINR_BEAMFORMING_VECTORS                        255
#define MIN_BUCKET_SIZE_DURATION                              0
#define MAX_BUCKET_SIZE_DURATION                              5
#define MIN_PRIORITIZED_BIT_RATE                              0
#define MAX_PRIORITIZED_BIT_RATE                              7
#define BUCKET_SIZE_DURATION_COUNT                            6
#define PRIORITIZED_BIT_RATE_COUNT                            8
#define MIN_MAC_ECID_MEAS_START_SFN                           0
#define MAX_MAC_ECID_MEAS_START_SFN                         255
#define MIN_CONTENTIONFREERACHTIMER                          25
#define MAX_CONTENTIONFREERACHTIMER                         200 
/* SPR 20653 Fix End */
#ifdef ENDC_ENABLED
#define MIN_DC_PREP_TIMER                                    50
#define MAX_DC_PREP_TIMER                                 65535
/* OPTION3X Changes Start */
#define MIN_DC_BEARER_CHANGE_GUARD_TIMER                     50
#define MAX_DC_BEARER_CHANGE_GUARD_TIMER                  65535
#define MIN_GTPU_DATA_FORWARDING_GUARD_TIMER                 10
#define MAX_GTPU_DATA_FORWARDING_GUARD_TIMER              65535
#define MIN_X2U_DELIVERY_STATUS_TRIGGER_TIMER                10
#define MAX_X2U_DELIVERY_STATUS_TRIGGER_TIMER             65535
#define MIN_X2U_DELIVERY_STATUS_PKT_COUNT                    10
#define MAX_X2U_DELIVERY_STATUS_PKT_COUNT                 65535
#define MIN_X2U_DELIVERY_STATUS_PKT_SIZE                     10
#define MAX_X2U_DELIVERY_STATUS_PKT_SIZE                  65535
#define DEFAULT_GTPU_DATA_FORWARDING_GUARD_TIMER          10000
#define DEFAULT_X2U_DELIVERY_STATUS_TRIGGER_TIMER         10000
#define DEFAULT_X2U_DELIVERY_STATUS_PKT_COUNT             10000
#define DEFAULT_X2U_DELIVERY_STATUS_PKT_SIZE               5000
/* OPTION3X Changes End */
#endif
/* SPR 22420 Fix Start */
#define MAX_SAMPLING_NORMAL_TIME                           3600
/* SPR 22420 Fix End */
/****************************************************************************
 **  FUNCTION PROTOTYPES
 ****************************************************************************/
/*function to print name of elements in xml*/
void print_element_names
(
    xmlNode *a_node
);

/*function to find tags in xml*/
xmlNode*
find_tag
(
    xmlNode *a_node,
    xmlChar *tagname
);

    /* SPR 20430 Changes Start*/
void 
oam_xml_find_ptr
(
    xmlNode *p_a_node,
    xmlNode **p_b_Node,
    char    *p_str
);

    /* SPR 20430 Changes End*/
xmlChar*
oam_xmlNodeGetContent
(
    xmlNode *p_parent,
    char    *p_node_tag
);

/*to find pointer of element by the name of str and to be stored in xmlNode 
pointer*/
void 
find_ptr
(
    xmlNode      *a_node,
    unsigned int *b_Node,
    char         *str
);

void 
populate
(
    xmlNode *node
);

unsigned int 
oam_atoh
(
    char *str
);
xmlNode *
oam_xml_find_tag
(
    xmlNode *p_a_node,
    xmlChar *p_tagname
);

#ifdef OAM_SON_ENABLE
oam_return_et
oam_populate_son_cell_info_ind
(
    XML_struct_cell_config *p_cell_spec_params,
    son_cell_t *p_serving_cell, 
    oam_rrm_cell_config_req_t *p_rrm_cellconfig
);

/*eICIC_PHASE_1_2_CHANGES_START*/
/* + SPR 17439 */
oam_return_et
oam_populate_anr_neighbor_default_config_params(void);
/*eICIC_PHASE_1_2_CHANGES_END*/
oam_return_et
oam_populate_son_nr_enb_add_req(void);
oam_return_et
oam_populate_son_nr_add_req
(
 XML_struct_cell_config        *p_cell_spec_params,
 oam_rrm_cell_config_req_t     *p_rrm_cellconfig,
 oam_cell_neighbourlist_info_t *p_nr_info
 );
/* - SPR 17439 */
oam_return_et
oam_populate_son_pci_selection_params
(
    XML_struct_cell_config *p_cell_spec_params,
    oam_son_pci_selection_req_t  *p_pci_selection_req,
    /* SPR 15267 START */
    oam_son_pci_reselection_req_t  *p_pci_reselection_req,
 /* Spr 17753 Changes Start */
 oam_cell_neighbourlist_info_t *p_nr_info,
    /* SPR 15267 END */
 son_cell_t *p_serving_cell
 /* Spr 17753 Changes End */
 );
/* + SPR 17439 */
/*+ SPR 18390*/
oam_return_et
oam_populate_son_career_freq_and_dl_tx_power_params
(
 XML_struct_cell_config *p_cell_spec_params,
 oam_son_carrier_freq_and_dl_tx_power_req_t *p_earfcn_selection_req,
 oam_cell_carrier_params_t *p_cell_carr_params
 );
/*- SPR 18390*/
#endif

oam_return_et 
oam_populate_s1ap_enb_cfg_update_params(void);


oam_return_et
oam_populate_son_mro_params(void);

oam_return_et
oam_populate_son_mlb_params(void);

oam_return_et
oam_populate_son_rach_opt_params(void);

oam_return_et oam_populate_son_nw_scan_params(void);
/*SPR_15770 Fix start*/
UInt8 oam_fill_known_earfcn(UInt16 *p_earfcn_list);
oam_return_et oam_populate_son_nr_scan_params(void);
/*SPR_15770 Fix end*/
oam_return_et oam_populate_son_nw_scan_cfg(void);
oam_return_et oam_populate_son_periodic_nw_scan_req(void);

/* OAM BCOM Code Comment Changes Start */
oam_return_et oam_populate_son_meas_config_req_params(void);
/* OAM BCOM Code Comment Changes End */

oam_return_et 
oam_populate_fault_mgmt_param(void);

#ifdef OAM_SON_ENABLE
oam_return_et
oam_populate_son_es_params(EnergySavingConfig_t *p_es_config);
oam_return_et
oam_populate_nw_scan_results(void);
/* - SPR 17439 */

oam_return_et
oam_populate_son_neigh_list_in_use
(
    XML_struct_cell_config *p_cell_spec_params,
    oam_son_neigh_list_in_use_t  *p_neigh_list_in_use,
    oam_rrm_cell_config_req_t *p_rrm_cellconfig
);
#endif

/* + SPR 17439 */
oam_return_et oam_populate_rrm_inter_rat_ncl_geran(void);

oam_return_et oam_populate_nms_ftp_server_params(void);

oam_return_et
oam_populate_dynamic_icic_param
(
 oam_rrm_cell_config_req_t  *p_rrm_cellconfig,
 XML_struct_cell_config *p_cell_spec_params,
 prop_cellconfig_xml_st *p_prop_cellconfig,
 /* SPR 21958 PUSCH RAT1 Support Start */
  UInt8 cell_cnt
 /* SPR 21958 PUSCH RAT1 Support End */
 );
/* - SPR 17439 */

oam_return_et
oam_populate_dynamic_icic_param_cell_based
(
    oam_rrm_cell_config_req_t  *cell_config_ptr,
    XML_struct_cell_config *p_cell_spec_params,
    prop_cellconfig_xml_st *p_prop_cellconfig
);

#ifdef OAM_SHARED_MEM_LOGGER
oam_return_et
oam_populate_shared_memory_conf(void);
#endif

/* + SPR 17439 */
oam_return_et
oam_populate_configrable_core_num(void);

oam_return_et
oam_populate_support_for_log_archival(void);
/* - SPR 17439 */

typedef oam_return_et (*p_validateFunc)(UInt8*) ;
/* This function used to populate string parameters without validation */
void  
oam_populate_string_params
( xmlNode *, 
   Char8 *, 
   UInt8 *,
   UInt32
);
/* This function used to populate string parameters with validation */
oam_return_et 
oam_populate_string_param_with_validation
( 
 xmlNode * , 
 Char8 * , 
 UInt8 * , 
 UInt32,
 p_validateFunc
);

/* + SPR 17439 */
	oam_return_et
oam_populate_lan_device(oam_l2_req_t *p_l2_req);
/* - SPR 17439 */
/*SPR 21096 Start*/
//TEMP FIX FOR MULTIPLE INSTANCES
oam_return_et
oam_populate_lan_device_1(oam_l2_req_t *p_l2_req);
/*SPR 21096 Start*/
char *
oam_xml_str_to_ptr
(
char * p_src
);

/* + SPR 17439 */
oam_return_et
oam_populate_health_mgmt_param(void);
/* - SPR 17439 */

oam_return_et
oam_populate_l2_sps_param
(
    XML_struct_cell_config    *p_cell_spec_params,
    prop_cellconfig_xml_st *p_prop_cellconfig,
    oam_rrm_cell_config_req_t *p_rrm_cellconfig,
    oam_l2_req_t           *p_l2_init_req
);


oam_return_et
oam_populate_rrm_cell_config_param
(
    oam_rrm_cell_config_req_t *p_rrm_cellconfig,
    oam_rrm_cell_plmnlist_data_t *p_plmnlist_data,
    XML_struct_cell_config *p_cell_spec_params,
/* SPR 22248 MBMS Changes Start */
     XML_struct_fap_service *p_cell_specFAPService,
/* SPR 22248 MBMS Changes Stop */
  /* SPR 17730 Fix Start */
#ifdef OAM_SON_ENABLE
   oam_son_inter_freq_carrier_t *p_son_inter_freq_carr_params,
#endif
  /* SPR 17730 Fix End */
    /* spr_21013_fix_start */
    prop_cellconfig_xml_st *p_prop_cellconfig,
    /* SPR 15527 START */
    UInt8 cell_cnt,
    /* spr_21013_fix_end */
    oam_rrm_qos_data_t *p_qos_data
    /* SPR 15527 END */
);


/* + SPR 17439 */
oam_return_et
oam_populate_rrm_inter_rat_ncl_utran
(
 oam_rrm_cell_config_req_t  *p_rrm_cellconfig,
 XML_struct_cell_config *p_cell_spec_params
 );
/* - SPR 17439 */

    oam_return_et
oam_populate_rrm_inter_rat_ncl_utran_cell_based
(
    oam_rrm_cell_config_req_t  *cell_config_ptr,
    XML_struct_cell_config *p_cell_spec_params
);
            /* RT SPR 9019 FIX END */

/*SPR 15604 START */
oam_return_et oam_populate_rrm_inter_rat_ncl_utran_nliu(oam_rrm_cell_config_req_t *p_rrm_cellconfig ,XML_struct_cell_config *p_cell_spec_params);
/*SPR 15604 END */
oam_return_et
oam_populate_rrm_geran_freq_cells_cell_based
(
    oam_rrm_cell_config_req_t  *cell_config_ptr,
    XML_struct_cell_config *p_cell_spec_params
);

oam_return_et
oam_populate_rrm_cdma2000_cell_based
(
    oam_rrm_cell_config_req_t  *cell_config_ptr,
    XML_struct_cell_config *p_cell_spec_params
);

oam_return_et
oam_populate_rrm_meas_gap_config_cell_based
(
    oam_rrm_cell_config_req_t  *cell_config_ptr,
    XML_struct_cell_config *p_cell_spec_params
);


oam_return_et
oam_populate_rrm_utran_freq_cells_cell_based
(
    oam_rrm_cell_config_req_t  *cell_config_ptr,
    XML_struct_cell_config *p_cell_spec_params
);

        /* SPR 11952 FIX START */
oam_return_et
oam_populate_device_specific_rrc_params
(
     XML_struct_cell_config *p_cell_spec_params
);
        /* SPR 11952 FIX END */

oam_return_et
oam_populate_cell_specific_rrc_params
(
    XML_struct_cell_config *p_cell_spec_params,
    prop_cellconfig_xml_st *p_prop_cellconfig,
    oam_rrm_cell_config_req_t *p_rrm_cellconfig,
    rrc_oam_sector_info_t *p_cell_info
);

/* + SPR 17439 */
oam_return_et
oam_populate_fap_control_param(void);
/* - SPR 17439 */

oam_return_et
oam_populate_l3_s1ap_param(XML_struct_cell_config *p_cell_spec_params);

/* SPR 11001 FIX START */
/* + SPR 17439 */
oam_return_et
oam_populate_system_spec_load_config_params(void);
/* - SPR 17439 */

oam_return_et
oam_populate_cell_spec_load_config_params
(
    rrm_oam_serving_enb_cell_info_t *p_srv_enb_cell_info,
    prop_cellconfig_xml_st *p_prop_cellconfig,
    oam_rrm_cell_config_req_t *p_rrm_cell_config
);
/*SPR 12228 FIX START*/
            /*Redundant variables removed */
/*SPR 12228 FIX END*/
/* SPR 11001 FIX END */

oam_return_et
oam_populate_l3_rrc_params(XML_struct_cell_config *p_cell_spec_params,
                           rrc_oam_sector_info_t  *p_cell_info);
oam_return_et
oam_populate_l3_x2ap_param(XML_struct_cell_config *p_cell_spec_params,
                           x2apOam_srvd_cell_arry_t  *p_cell_info);

/* + SPR 17439 */
oam_return_et
oam_populate_l3_rrc_module_timer_info(void);

oam_return_et
oam_populate_l3_x2ap_timer(void);

oam_return_et
oam_populuate_l2_rlc_sps_params(void);

oam_return_et
oam_populate_rrm_geran_freq_cells(void);

oam_return_et
oam_populate_tr069_params(void);

oam_return_et
oam_populate_tr069_tr196_params(XML_struct_cell_config *p_cell_spec_params);

oam_return_et 
oam_populate_device_info_param(void);

oam_return_et
oam_xml_struct_tr196_populate(xmlNode *p_node);

oam_return_et
oam_populate_rrm_meas_gap_config(void);

oam_return_et
oam_populate_rrm_utran_freq_cells(void);

oam_return_et
oam_populate_rrm_cdma2000(void);

oam_return_et
oam_populate_l3_s1ap_s1apInfo(void);
/* - SPR 17439 */

oam_return_et
oam_populate_l2_mac_init_param(XML_struct_cell_config *p_cell_spec_params,
                               prop_cellconfig_xml_st *p_prop_cellconfig,
                               oam_l2_req_t         *p_l2_req,
                               oam_rrm_cell_config_req_t *p_rrm_cellconfig);

oam_return_et
oam_populate_l2_mac_log_level_param(oam_l2_req_t  *p_l2_req);

void
oam_populate_l2_qos_params(oam_l2_req_t *p_l2_init,
                            oam_rrm_cell_config_req_t *p_rrm_cellconfig);

oam_return_et
oam_populate_l2_pdcp_init_param
(
    XML_struct_cell_config *p_cell_spec_params,
    prop_cellconfig_xml_st *p_prop_cellconfig,
    oam_l2_req_t * p_l2_init
);

oam_return_et
oam_populate_ca_params(oam_rrm_cell_config_req_t  *p_cellconfig,
                       XML_struct_cell_config     *p_cell_spec_params);

/* + SPR 17439 */
oam_return_et
oam_populate_device_specific_s1ap_params(void);

oam_return_et
oam_populate_device_specific_x2ap_params(void);
/* - SPR 17439 */

/* LWA changes start*/
/* LWA changes end*/

oam_return_et
oam_populate_cell_specific_x2ap_params
(
    XML_struct_cell_config         *p_cell_spec_params,
    rrc_oam_sector_info_t          *p_rrc_cell_info,
    oam_rrm_cell_config_req_t      *p_rrm_cellconfig,
    x2apOam_srvd_cell_arry_t       *p_cell_info
);

oam_return_et
oam_populate_cell_specific_s1ap_params
(
    XML_struct_cell_config *p_cell_spec_params,
    oam_rrm_cell_config_req_t *p_rrm_cellconfig
);

oam_return_et
oam_populate_l2_rlc_init_param
(
    prop_cellconfig_xml_st *p_prop_cellconfig,
    oam_rlc_init_layer_req_t *p_rlc_init_req
);
/* SPR 17777 fix */
oam_return_et
oam_populate_l2_icic_params(XML_struct_cell_config    *p_cell_spec_params,
                            oam_l2_req_t              *p_l2_req,
                            UInt8                      cell_cnt_index);
/* SPR 17777 fix */
oam_return_et
oam_populate_l2_ul_qos_strategy_wt_params
(
    prop_cellconfig_xml_st    *p_prop_cellconfig,
    oam_l2_req_t              *p_l2_req,
    UInt8                      cell_cnt_index
);

oam_return_et
oam_populate_l2_ul_qos_params
(
    XML_struct_cell_config    *p_cell_spec_params,
    prop_cellconfig_xml_st    *p_prop_cellconfig,
    oam_l2_req_t              *p_l2_req,
    UInt8                      cell_cnt_index
);
/* SPR 17777 fix */
oam_return_et
oam_populate_l2_dl_qos_strategy_wt_params
(
    prop_cellconfig_xml_st    *p_prop_cellconfig,
    oam_l2_req_t              *p_l2_req,
    UInt8                      cell_cnt_index
);

oam_return_et
oam_populate_l2_dl_qos_params
(
    XML_struct_cell_config    *p_cell_spec_params,
    prop_cellconfig_xml_st    *p_prop_cellconfig,
    oam_l2_req_t              *p_l2_req,
    UInt8                      cell_cnt_index
);
/* SPR 17777 fix */
oam_return_et
oam_populate_l2_dl_pfs_params
(
    prop_cellconfig_xml_st    *p_prop_cellconfig,
    oam_l2_req_t              *p_l2_req,
    UInt8                      cell_cnt_index
);
/* SPR 17777 fix */
oam_return_et
oam_populate_l2_ul_pfs_params
(
    prop_cellconfig_xml_st    *p_prop_cellconfig,
    oam_l2_req_t              *p_l2_req,
    UInt8                      cell_cnt_index
);
/* SPR 17777 fix */
oam_return_et
oam_populate_l2_sps_comm_interval_profile
(
    prop_cellconfig_xml_st    *p_prop_cellconfig,
    oam_l2_req_t              *p_l2_req,
    UInt8                      cell_cnt_index
);
/* SPR 17777 fix */
oam_return_et
oam_populate_l2_sps_ul_interval_profile
(
    prop_cellconfig_xml_st    *p_prop_cellconfig,
    oam_l2_req_t              *p_l2_req,
    UInt8                      cell_cnt_index
);
/* SPR 17777 fix */
oam_return_et
oam_populate_l2_sps_dl_interval_profile
(
    prop_cellconfig_xml_st    *p_prop_cellconfig,
    oam_l2_req_t              *p_l2_req,
    UInt8                      cell_cnt_index
);

oam_return_et
oam_populate_l2_cell_specific_params
(
    XML_struct_cell_config     *p_cell_spec_params,
    prop_cellconfig_xml_st     *p_prop_cellconfig,
    oam_rrm_cell_config_req_t  *p_rrm_cellconfig,
    oam_l2_req_t               *p_l2_req,
    UInt8                      cell_cnt_index,
    /*SPR 21096 Start*/
    UInt8 			    l2_instance
    /*SPR 21096 End*/
);

oam_return_et
oam_populate_l2_layer_specific_params
(
    XML_struct_cell_config     *p_cell_spec_params,
    prop_cellconfig_xml_st     *p_prop_cellconfig,
    oam_l2_req_t               *p_l2_req,
    oam_rrm_cell_config_req_t *p_rrm_cellconfig,
    /*SPR 21321 Start*/
    UInt8 			cell_cnt,
    UInt8			l2_instance
    /*SPR 21321 End*/
);


oam_return_et
oam_populate_egtpu_init_req(prop_cellconfig_xml_st  *p_prop_cellconfig,
                            oam_l2_req_t            *p_l2_req);

/* + SPR 17439 */
oam_return_et oam_fill_version_id_req(void);
/* - SPR 17439 */

oam_return_et
oam_populate_l2_sps_syswide_params
(
    XML_struct_cell_config    *p_cell_spec_params,
    prop_cellconfig_xml_st    *p_prop_cellconfig,
    oam_rrm_cell_config_req_t *p_rrm_cellconfig,
    oam_l2_req_t              *p_l2_req,
    UInt8                      cell_cnt_index
);
/* SPR 20653 Fix Start */
oam_return_et
oam_populate_olpc_config_params
(
    XML_struct_cell_config *p_cell_spec_params,
    oam_rrm_cell_config_req_t *p_rrm_cellconfig
);
/* SPR 20653 Fix End */

/*SPR 21889 +- */

oam_return_et
oam_populate_rrm_enb_config_params(void);
/* - SPR 17439 */

oam_return_et
oam_libxml_init(SInt32 argc, Char8* arg[]);

#ifdef OAM_SON_ENABLE
oam_return_et
oam_populate_son_anr_meas_params
(
    XML_struct_cell_config *p_cell_spec_params,
    oam_son_inter_freq_carrier_t *p_son_inter_freq_carr_params,
    oam_rrm_cell_config_req_t *p_rrm_cell_config
);

oam_return_et
oam_populate_son_utra_tdd_list
(
    oam_son_inter_freq_carrier_t *p_son_inter_freq_carr_params,
    oam_rrm_cell_config_req_t *p_rrm_cell_config
);

oam_return_et
oam_populate_son_utra_fdd_list
(
    XML_struct_cell_config *p_cell_spec_params,
    oam_son_inter_freq_carrier_t *p_son_inter_freq_carr_params,
    oam_rrm_cell_config_req_t *p_rrm_cell_config
);

oam_return_et
oam_populate_son_geran_config_list
(
    XML_struct_cell_config *p_cell_spec_params,
    oam_son_inter_freq_carrier_t *p_son_inter_freq_carr_params,
    oam_rrm_cell_config_req_t *p_rrm_cell_config
);
#endif


oam_return_et   
oam_populate_eicic_params
(   oam_rrm_cell_config_req_t  *p_cellconfig,
    XML_struct_cell_config     *p_cell_spec_params
);

xmlChar *
oam_get_xml_text_node_content(xmlNode *p_parent,
                              Char8 *tag_name,
                              Char8 *out_buf,
                              UInt16 buf_size);

XML_struct_cell_config* oam_get_xml_cellconfig_node(UInt8 cid);

xmlNode* oam_get_xml_leaf_child_node(xmlNode * p_parent, Char8* p_name);

xmlNode* oam_get_xml_nonleaf_child_node(xmlNode * p_parent, Char8* p_name);

xmlNode* oam_xmlCopyNode(xmlNode *node, SInt32 opt);
/* SPR 21889 Start */ 
oam_return_et oam_populate_process_tbl(xmlNode *p_process_tbl_node);
/* SPR 21889 End */ 

#ifdef ENDC_ENABLED
oam_return_et       
    oam_populate_en_dc_config_params(void);
#endif


#endif /* End of _xml_header_h_ */
