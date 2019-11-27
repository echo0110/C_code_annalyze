/******************************************************************************
*
*   FILE NAME:
*       rrc_phy_intf.h
*
*   DESCRIPTION:
*       This file contains types used for representation of PHY API inside RRC
*       Based on LTE_RRC_API_0_31.doc.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   28 Apr 2009     VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_PHY_INTF_H_
#define _RRC_PHY_INTF_H_

#include "rrc_defines.h"

typedef enum
{
    PHY_FAILURE,
    PHY_SUCCESS,
    PHY_PARTIAL_SUCCESS
} rrc_phy_return_et;
#ifndef _MSC_VER
typedef enum
{
    UL_TX_BANDWIDTH_6RB = 0,
    UL_TX_BANDWIDTH_16RB = 1,
    UL_TX_BANDWIDTH_25RB = 2,
    UL_TX_BANDWIDTH_50RB = 3,
    UL_TX_BANDWIDTH_75RB = 4,
    UL_TX_BANDWIDTH_100RB = 5
} ul_tx_bandwidth_et;

typedef enum
{
    DL_TX_BANDWIDTH_6RB = 0,
    DL_TX_BANDWIDTH_16RB = 1,
    DL_TX_BANDWIDTH_25RB = 2,
    DL_TX_BANDWIDTH_50RB = 3,
    DL_TX_BANDWIDTH_75RB = 4,
    DL_TX_BANDWIDTH_100RB = 5
} dl_tx_bandwidth_et;

typedef enum
{
    SUBCARRIER_SPACING_FREQ15 = 0,
    SUBCARRIER_SPACING_FREQ7DOT15 = 1
} subcarrier_spacing_et;

typedef enum
{
    CYCLIC_PREFIX_NORMAL = 0,
    CYCLIC_PREFIX_EXTENDED = 1
} cyclic_prefix_et;

typedef enum
{
    RB_SIZE_12 = 0,
    RB_SIZE_24 = 1
} rb_size_et;

typedef enum
{
    SRS_BW_CONFIG_BW0,
    SRS_BW_CONFIG_BW1,
    SRS_BW_CONFIG_BW2,
    SRS_BW_CONFIG_BW3,
    SRS_BW_CONFIG_BW4,
    SRS_BW_CONFIG_BW5,
    SRS_BW_CONFIG_BW6,
    SRS_BW_CONFIG_BW7
} srs_bw_config_et;

typedef enum
{
    RS_NO_HOPPING = 0,
    RS_GROUP_HOPPING = 1,
    RS_SEQUENCE_HOPPING = 2
} uplink_rs_hopping_et;

typedef enum
{
    DS_1 = 1,
    DS_2 = 2,
    DS_3 = 3
} pucch_delta_shift_et;

typedef enum
{
    PHICH_R_ONE_SIXTH = 0,
    PHICH_R_HALF = 1,
    PHICH_R_ONE = 2,
    PHICH_R_TWO = 3
} phich_resource_et;

typedef enum
{
    PHICH_D_NORMAL = 0,
    PHICH_D_EXTENDED = 1
} phich_duration_et;

typedef enum
{
    PDSCH_CONFIGURATION_DEDICATED_P_A_DB_M_6 = 0,
    PDSCH_CONFIGURATION_DEDICATED_P_A_DB_M_4DOT77 = 1,
    PDSCH_CONFIGURATION_DEDICATED_P_A_DB_M_3 = 2,
    PDSCH_CONFIGURATION_DEDICATED_P_A_DB_M_1DOT77 = 3,
    PDSCH_CONFIGURATION_DEDICATED_P_A_DB0 = 4,
    PDSCH_CONFIGURATION_DEDICATED_P_A_DB1 = 5,
    PDSCH_CONFIGURATION_DEDICATED_P_A_DB2 = 6,
    PDSCH_CONFIGURATION_DEDICATED_P_A_DB3 = 7
} pdsch_configuration_dedicated_p_a_et;

typedef enum
{
    ACK_NACK_REPETITION_FACTOR_N2 = 0,
    ACK_NACK_REPETITION_FACTOR_N4 = 1,
    ACK_NACK_REPETITION_FACTOR_N6 = 2
} ack_nack_repetition_factor_et;

typedef enum
{
    DELTA_MCS_EN0 = 0,
    DELTA_MCS_EN1 = 1
} delta_mcs_enabled_et;

typedef enum
{
    CQI_REPORTING_MODE_APERIODIC_RM12 = 0,
    CQI_REPORTING_MODE_APERIODIC_RM20 = 1,
    CQI_REPORTING_MODE_APERIODIC_RM22 = 2,
    CQI_REPORTING_MODE_APERIODIC_RM30 = 3,
    CQI_REPORTING_MODE_APERIODIC_RM31 = 4
} cqi_reporting_mode_aperiodic_et;

typedef enum
{
    SRS_BANDWIDTH_BW0 = 0,
    SRS_BANDWIDTH_BW1 = 1,
    SRS_BANDWIDTH_BW2 = 2,
    SRS_BANDWIDTH_BW3 = 3
} srs_bandwidth_et;

typedef enum
{
    SRS_HOPPING_BANDWIDTH_HBW0 = 0,
    SRS_HOPPING_BANDWIDTH_HBW1 = 1,
    SRS_HOPPING_BANDWIDTH_HBW2 = 2,
    SRS_HOPPING_BANDWIDTH_HBW3 = 3
} srs_hopping_bandwidth_et;

typedef enum
{
    CYCLIC_SHIFT_CS0 = 0,
    CYCLIC_SHIFT_CS1 = 1,
    CYCLIC_SHIFT_CS2 = 2,
    CYCLIC_SHIFT_CS3 = 3,
    CYCLIC_SHIFT_CS4 = 4,
    CYCLIC_SHIFT_CS5 = 5,
    CYCLIC_SHIFT_CS6 = 6,
    CYCLIC_SHIFT_CS7 = 7
} cyclic_shift_et;

typedef enum
{
    TRANSMISSION_MODE_TM1 = 0,
    TRANSMISSION_MODE_TM2 = 1,
    TRANSMISSION_MODE_TM3 = 2,
    TRANSMISSION_MODE_TM4 = 3,
    TRANSMISSION_MODE_TM5 = 4,
    TRANSMISSION_MODE_TM6 = 5,
    TRANSMISSION_MODE_TM7 = 6,
    TRANSMISSION_MODE_TM8_V920 = 7  /* CR_316And247_START */
} transmission_mode_et;

typedef enum
{
    N2_TX_ANTENNA_TM3 = 0,
    N4_TX_ANTENNA_TM3 = 1,
    N2_TX_ANTENNA_TM4 = 2,
    N4_TX_ANTENNA_TM4 = 3,
    N2_TX_ANTENNA_TM5 = 4,
    N4_TX_ANTENNA_TM5 = 5,
    N2_TX_ANTENNA_TM6 = 6,
    N4_TX_ANTENNA_TM6 = 7
} codebook_subset_restriction_type_et;

/* CR_316And247_START */
typedef enum
{
    N2_TX_ANTENNA_TM8_R9 = 0,
    N4_TX_ANTENNA_TM8_R9 = 1
} codebook_subset_restriction_V920_type_et;
/* CR_316And247_STOP */

typedef enum
{
    CLOSE_LOOP = 0,
    OPEN_LOOP = 1
} ue_transmit_antenna_selection_type_et;

typedef enum
{
    DSR_TRANS_MAX_N4 = 0,
    DSR_TRANS_MAX_N8 = 1,
    DSR_TRANS_MAX_N16 = 2,
    DSR_TRANS_MAX_N32 = 3,
    DSR_TRANS_MAX_N64 = 4
} dsr_trans_max_et;
#endif
typedef enum
{
    SUBMODE_1 = 0,
    SUBMODE_2
} cqi_periodic_format_ind_widebandCQI_csi_reportMode_r10_et;

#pragma pack(push, 1)

/******************************************************************************
*   PHY Cell messages
******************************************************************************/


/******************************************************************************
*   RRC_PHY_CONFIG_CELL_REQ
******************************************************************************/
typedef struct _rrc_phy_config_cell_req_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_EXPLICIT_START_REQUIRED_PRESENT          0x01
/* eMTC changes start */
#define RRC_PHY_PRACH_CONFIG_V1310_PRESENT              0x02
#define RRC_PHY_FREQUENCY_HOPPING_PARAMS_R13_PRESENT    0x04
/* eMTC changes stop */

    rrc_cell_index_t                cell_index;
    /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
    rrc_config_phy_cell_parameters_t cell_parameters;
    rrc_phy_sync_signals_t          sync_signals;
    rrc_phy_prach_configuration_t   prach_configuration;
    rrc_phy_pusch_configuration_t   pusch_configuration;
    rrc_phy_pucch_configuration_t   pucch_configuration;
    rrc_phy_phich_configuration_t   phich_configuration;
    pdsch_config_common_t           pdsch_configuration;
    U8                              explicit_start_required;  /*^ O, RRC_PHY_EXPLICIT_START_REQUIRED_PRESENT , H, 0, 0 ^*/
    /* eMTC changes start */
    RrcPhyPrachConfigV1310_t              prachConfigurationV1310;
    /*^ O, RRC_PHY_PRACH_CONFIG_V1310_PRESENT , N, 0, 0 ^*/

    RrcFrequancyHoppingParametersR13_t    frequancyHoppingParametersR13;
    /*^ O, RRC_PHY_FREQUENCY_HOPPING_PARAMS_R13_PRESENT , N, 0, 0 ^*/
    /* eMTC changes stop */

} rrc_phy_config_cell_req_t; /*^ API, RRC_PHY_CONFIG_CELL_REQ ^*/



/******************************************************************************
*   RRC_PHY_CONFIG_CELL_CNF
******************************************************************************/
typedef struct _rrc_phy_config_cell_cnf_t
{
    rrc_cell_index_t    cell_index;
    /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
    U8                  response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

} rrc_phy_config_cell_cnf_t; /*^ API, RRC_PHY_CONFIG_CELL_CNF ^*/

/***************************************************************************
 * RRC_PHY_RECONFIG_CELL_PARAMETERS
 * ***********************************************************************/

typedef struct _rrc_phy_reconfig_cell_parameters_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_RECONFIG_PHY_UL_EARFCN                               0x01
#define RRC_RECONFIG_PHY_DL_EARFCN                               0x02
#define RRC_RECONFIG_PHY_NUM_OF_ANTENNAS                         0x04
#define RRC_RECONFIG_PHY_UL_TX_BANDWIDTH_PRESENT                 0x08
#define RRC_RECONFIG_PHY_DL_TX_BANDWIDTH_PRESENT                 0x10
#define RRC_RECONFIG_PHY_UL_CYCLIC_PREFIX                        0x20
#define RRC_RECONFIG_PHY_DL_CYCLIC_PREFIX                        0x40
#define RRC_RECONFIG_PHY_SRS_BANDWIDTH_CONFIGURATION_PRESENT     0x80
#define RRC_RECONFIG_PHY_DUPLEX_MODE                             0x100   
#define RRC_RECONFIG_PHY_CELL_ID                                 0x200
/* CLPC Start */
#define RRC_PCFICH_POWER_OFFSET_PRESENT                          0x400
#define RRC_PHICH_POWER_OFFSET_PRESENT                           0x800
#define RRC_PRIMARY_SYNC_SIGNAL_EPRE_EPRERS                      0x1000
#define RRC_SECONDARY_SYNC_SIGNAL_EPRE_EPRERS                    0x2000
/* CLPC Stop */
/* SPR 3802 Start */
#define RRC_RECONFIG_PHY_MAX_RS_EPRE_PRESENT                     0x4000
/* SPR 3802 Stop */
/* PRS +*/
#define RRC_RECONFIG_PHY_PRS_PRESENT                       	 0x8000
/* PRS -*/
    U8  duplexing_mode;  /*^ O, RRC_RECONFIG_PHY_DUPLEX_MODE, H, 0, 1 ^*/ 
            /* duplexing_mode_et */

    U16 ul_earfcn;                          
    /*^ O,RRC_RECONFIG_PHY_UL_EARFCN ^*/    

    U32 dl_earfcn;
    /*^ O,RRC_RECONFIG_PHY_DL_EARFCN ^*/    

    U8  num_of_antennas;                    
    /*^ O,RRC_RECONFIG_PHY_NUM_OF_ANTENNAS , B, 1, 4 ^*/
    
    U8  ul_tx_bandwidth;
    /*^ O, RRC_RECONFIG_PHY_UL_TX_BANDWIDTH_PRESENT, H, 0, 5 ^*/  /* ul_tx_bandwidth_et */
    
    U8  dl_tx_bandwidth;
    /*^ O, RRC_RECONFIG_PHY_DL_TX_BANDWIDTH_PRESENT, H, 0, 5 ^*/  /* dl_tx_bandwidth_et */

    U8  ul_cyclic_prefix;
    /*^ O, RRC_RECONFIG_PHY_UL_CYCLIC_PREFIX, H, 0, 1 ^*/ 
    /* cyclic_prefix_et */

    U8  dl_cyclic_prefix;
    /*^ O, RRC_RECONFIG_PHY_DL_CYCLIC_PREFIX, H, 0, 1 ^*/     
    /* cyclic_prefix_et */

    sounding_rs_ul_config_common_t        srs_bandwidth_configuration;
    /*^ O, RRC_RECONFIG_PHY_SRS_BANDWIDTH_CONFIGURATION_PRESENT, H, 0, 7 ^*/

    rrc_phys_cell_id_t  phys_cell_id;
    /*^ O, RRC_RECONFIG_PHY_CELL_ID ^*/


    /* CLPC Start */

    U16 pcfich_power_offset;               
    /*^ O, RRC_PCFICH_POWER_OFFSET_PRESENT, H, 0, 10000 ^*/

    U16 phich_power_offset;                
    /*^ O, RRC_PHICH_POWER_OFFSET_PRESENT, H, 0, 10000 ^*/

    U16 pri_sync_sig_epre_eprers;
    /*^ O, RRC_PRIMARY_SYNC_SIGNAL_EPRE_EPRERS, H, 0, 10000 ^*/

    U16 sec_sync_sig_epre_eprers;
    /*^ O, RRC_SECONDARY_SYNC_SIGNAL_EPRE_EPRERS, H, 0, 10000 ^*/


    /* CLPC Stop */

    /* SPR 3802 Start */
    U8    max_rs_epre;
    /*^ O, RRC_RECONFIG_PHY_MAX_RS_EPRE_PRESENT, H, 0, 160 ^*/
    /* SPR 3802 Stop */
    /* PRS + */
    rrc_phy_prs_config_t            prs_reconfig;
    /*^ O, RRC_RECONFIG_PHY_PRS_PRESENT, N, 0, 0 ^*/
    /* PRS - */

} rrc_phy_reconfig_cell_parameters_t;
/******************************************************************************
*   RRC_PHY_RECONFIG_CELL_REQ
******************************************************************************/
typedef struct _rrc_phy_reconfig_cell_req_t
{

    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_RECONFIG_PHY_CELL_PARAMETERS_PRESENT                 0x01
#define RRC_RECONFIG_PHY_SYNC_SIGNAL_PRESENT                     0x02
#define RRC_RECONFIG_PHY_PRACH_CONFIGURATION_PRESENT             0x04
#define RRC_RECONFIG_PHY_PUSCH_CONFIGURATION_PRESENT             0x08
#define RRC_RECONFIG_PHY_PUCCH_CONFIGURATION_PRESENT             0x10
#define RRC_RECONFIG_PHY_PHICH_CONFIGURATION_PRESENT             0x20
#define RRC_RECONFIG_PHY_PDSCH_CONFIGURATION_PRESENT             0x40

/* eMTC changes start */
#define RRC_RECONFIG_PHY_PRACH_CONFIG_V1310_PRESENT              0x80
#define RRC_RECONFIG_PHY_FREQUANCY_HOPPING_PARAMS_R13_PRESENT    0x100
/* eMTC changes stop */

    rrc_cell_index_t                            cell_index;
    /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
    U16                                         sfn;
    U8                      sf; 
    rrc_phy_reconfig_cell_parameters_t          phy_recfg_cell_parameters;
    /*^ O,RRC_RECONFIG_PHY_CELL_PARAMETERS_PRESENT  ^*/
    rrc_phy_sync_signals_t                      sync_signals;
    /*^ O,RRC_RECONFIG_PHY_SYNC_SIGNAL_PRESENT^*/
    rrc_phy_prach_configuration_t               prach_configuration;
    /*^ O,RRC_RECONFIG_PHY_PRACH_CONFIGURATION_PRESENT^*/
    rrc_phy_pusch_configuration_t               pusch_configuration;
    /*^ O,RRC_RECONFIG_PHY_PUSCH_CONFIGURATION_PRESENT^*/
    rrc_phy_pucch_configuration_t               pucch_configuration;
    /*^ O,RRC_RECONFIG_PHY_PUCCH_CONFIGURATION_PRESENT^*/
    rrc_phy_phich_configuration_t               phich_configuration;
    /*^ O,RRC_RECONFIG_PHY_PHICH_CONFIGURATION_PRESENT^*/
    pdsch_config_common_t                       pdsch_configuration;
    /*^ O,RRC_RECONFIG_PHY_PDSCH_CONFIGURATION_PRESENT^*/

    /* eMTC changes start */
    RrcPhyPrachConfigV1310_t              prachConfigurationV1310;
    /*^ O, RRC_RECONFIG_PHY_PRACH_CONFIG_V1310_PRESENT , N, 0, 0 ^*/

    RrcFrequancyHoppingParametersR13_t    frequancyHoppingParametersR13;
    /*^ O, RRC_RECONFIG_PHY_FREQUANCY_HOPPING_PARAMS_R13_PRESENT , N, 0, 0 ^*/
    /* eMTC changes stop */

} rrc_phy_reconfig_cell_req_t; /*^ API, RRC_PHY_RECONFIG_CELL_REQ ^*/

/******************************************************************************
*   RRC_PHY_RECONFIG_CELL_CNF
******************************************************************************/
typedef struct _rrc_phy_reconfig_cell_cnf_t
{
    rrc_cell_index_t   cell_index;
    /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
    U8                 response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

} rrc_phy_reconfig_cell_cnf_t; /*^ API, RRC_PHY_RECONFIG_CELL_CNF ^*/

/******************************************************************************
*   RRC_PHY_DELETE_CELL_REQ
******************************************************************************/
typedef struct _rrc_phy_delete_cell_req_t
{
    rrc_cell_index_t   cell_index;
    /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
} rrc_phy_delete_cell_req_t; /*^ API, RRC_PHY_DELETE_CELL_REQ ^*/

/******************************************************************************
*   RRC_PHY_DELETE_CELL_CNF
******************************************************************************/
typedef struct _rrc_phy_delete_cell_cnf_t
{
    rrc_cell_index_t   cell_index;
    /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
} rrc_phy_delete_cell_cnf_t; /*^ API, RRC_PHY_DELETE_CELL_CNF ^*/

/******************************************************************************
*   PHY UE messages
******************************************************************************/

/******************************************************************************
*   RRC_PHY_CREATE_UE_ENTITY_REQ
******************************************************************************/
/* eMTC changes start */
#define RRC_PHY_CREATE_PHYSICAL_CONFIG_DEDICATED_R13_PRESENT    0x02
/* eMTC changes stop */
typedef struct _rrc_phy_create_ue_entity_req_t
{
#define RRC_PHY_CREATE_SCELL_CONFIG_PRESENT 0x01
    rrc_ue_index_t   ue_index;

    rrc_rnti_t       rnti;                          /*^ M, 0, B, 1, 65523 ^*/

    rrc_phy_physical_config_dedicated_t   physical_config_dedicated;

    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/

    rrc_phy_scell_config_list_t rrc_phy_scell_config_list;
    /*^ O, RRC_PHY_CREATE_SCELL_CONFIG_PRESENT ^*/

    /* eMTC changes start */
    RrcPhysicalConfigDedicatedR13_t    physicalConfigDedicatedR13;
    /*^ O, RRC_PHY_CREATE_PHYSICAL_CONFIG_DEDICATED_R13_PRESENT, N, 0, 0 ^*/
    /* eMTC changes stop */

} rrc_phy_create_ue_entity_req_t; /*^ API, RRC_PHY_CREATE_UE_ENTITY_REQ ^*/

/******************************************************************************
*   RRC_PHY_CREATE_UE_ENTITY_CNF
******************************************************************************/
typedef struct _rrc_phy_create_ue_entity_cnf_t
{
    
    rrc_ue_index_t    ue_index;
    U8                response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

} rrc_phy_create_ue_entity_cnf_t; /*^ API, RRC_PHY_CREATE_UE_ENTITY_CNF ^*/

/******************************************************************************
*   RRC_PHY_DELETE_UE_ENTITY_REQ
******************************************************************************/
typedef struct _rrc_phy_delete_ue_entity_req_t
{
    
    rrc_ue_index_t    ue_index;
} rrc_phy_delete_ue_entity_req_t; /*^ API, RRC_PHY_DELETE_UE_ENTITY_REQ ^*/

/******************************************************************************
*   RRC_PHY_DELETE_UE_ENTITY_CNF
******************************************************************************/
typedef struct _rrc_phy_delete_ue_entity_cnf_t
{
    
    rrc_ue_index_t   ue_index;
    U8               response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

} rrc_phy_delete_ue_entity_cnf_t; /*^ API, RRC_PHY_DELETE_UE_ENTITY_CNF ^*/

/******************************************************************************
*   RRC_PHY_RECONFIG_UE_ENTITY_REQ
******************************************************************************/
/* eMTC changes start */
#define RRC_PHY_RECONFIG_PHYSICAL_CONFIG_DEDICATED_R13_PRESENT    0x02
/* eMTC changes stop */
typedef struct _rrc_phy_reconfig_ue_entity_req_t
{
    
#define RRC_PHY_SCELL_CONFIG_PRESENT 0x01
    rrc_ue_index_t  ue_index;

    rrc_phy_physical_config_dedicated_t   physical_config_dedicated;

    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/

    rrc_phy_scell_config_list_t rrc_phy_scell_config_list;
/*^ O, RRC_PHY_SCELL_CONFIG_PRESENT ^*/

    /* eMTC changes start */
    RrcPhysicalConfigDedicatedR13_t    physicalConfigDedicatedR13;
    /*^ O, RRC_PHY_RECONFIG_PHYSICAL_CONFIG_DEDICATED_R13_PRESENT, N, 0, 0 ^*/
    /* eMTC changes stop */

} rrc_phy_reconfig_ue_entity_req_t; /*^ API, RRC_PHY_RECONFIG_UE_ENTITY_REQ ^*/

/******************************************************************************
*   RRC_PHY_RECONFIG_UE_ENTITY_CNF
******************************************************************************/
typedef struct _rrc_phy_reconfig_ue_entity_cnf_t
{
    rrc_bitmask_t   bitmask;    /*^ BITMASK ^*/
#define RRC_PHY_FAIL_CAUSE_PRESENT          0x01

    
    rrc_ue_index_t  ue_index;
    U8              response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

    U8              fail_cause;
/*^ O, RRC_PHY_FAIL_CAUSE_PRESENT ^*/

} rrc_phy_reconfig_ue_entity_cnf_t; /*^ API, RRC_PHY_RECONFIG_UE_ENTITY_CNF ^*/

/******************************************************************************
*   RRC_PHY_CHANGE_CRNTI_REQ
******************************************************************************/
typedef struct _rrc_phy_change_crnti_req_t
{
    
    rrc_ue_index_t  ue_index;
    rrc_rnti_t      old_crnti; 
    rrc_rnti_t      new_crnti; 
} rrc_phy_change_crnti_req_t; /*^ API, RRC_PHY_CHANGE_CRNTI_REQ ^*/

/******************************************************************************
*   RRC_PHY_CHANGE_CRNTI_CNF
******************************************************************************/
typedef struct _rrc_phy_change_crnti_cnf_t
{
    
    rrc_ue_index_t  ue_index;

    U16             response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

} rrc_phy_change_crnti_cnf_t; /*^ API, RRC_PHY_CHANGE_CRNTI_CNF ^*/

/*****************************************************************************
 *   RRC_PHY_CELL_START_REQ
 ***************************************************************************/
typedef struct _rrc_phy_cell_start_req_t
{
    
    rrc_cell_index_t   cell_index;
    /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
}rrc_phy_cell_start_req_t;
/*^ API, RRC_PHY_CELL_START_REQ ^*/

/*****************************************************************************
 *    RRC_PHY_CELL_START_CNF
 **************************************************************************/
typedef struct _rrc_phy_cell_start_cnf_t
{
    rrc_cell_index_t   cell_index;
    /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
    rrc_response_t     response;     /*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */
}rrc_phy_cell_start_cnf_t;  /*^ API, RRC_PHY_CELL_START_CNF ^*/

/*****************************************************************************
 *    RRC_PHY_CELL_STOP_REQ
 **************************************************************************/
typedef struct _rrc_phy_cell_stop_req_t
{
    rrc_cell_index_t   cell_index;
    /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
}rrc_phy_cell_stop_req_t; /*^ API, RRC_PHY_CELL_STOP_REQ ^*/

/***************************************************************************
*    RRC_PHY_CELL_STOP_CNF
 **************************************************************************/ 
typedef struct _rrc_phy_cell_stop_cnf_t
{
    rrc_cell_index_t   cell_index;
    /*^ M, 0, H, 0, MAX_CELL_INDEX ^*/    /* MAX_NUM_CELLS - 1 */
}rrc_phy_cell_stop_cnf_t;  /*^ API, RRC_PHY_CELL_STOP_CNF ^*/

#pragma pack(pop)

#endif /* _RRC_PHY_INTF_H_ */

