/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: rrm_cell_context.h $
 *
 *******************************************************************************
 *
 * File Description:This file contains the struture related with cell.
 *
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   29 Feb,2012                 Initial
 *   28 Apr,2014     gur27356    SPR-10397       Updated for PDCP KPI stat.
 *   16 May,2014     gur13083    SPR 10869       Modified ECN function to
 *                                               calculate UL/DL status separately
 *   27 May,2014     gur29831    SPR-11293       NRT FIX
 *   27 May 2014    gur29831  Valgrind Crash fix ue index was being limited to 255   
 *   16 May,2014     gur13083    SPR 10869       Modified ECN function to
 *                                               calculate UL/DL status separately
 *   28 May,2014     gur26515    SPR-10749       Updated for Fromat2/2a/2b Multiplexing.
 *   28 May,2014     gur27356    SPR-11383       Cell Unavailability KPI compliance.
 *   08 May,2014     gur27356    SPR-10813       End Marker Indication support in
 *                                               the Stats APIs from L2.
 *   19 Jun,2014     gur27356    SPR-12035       Measurement report A4 in cell Block Scenario.
 *   30 Jun,2014     gur30784    SPR-12233       Handling for dequeing the message
 *   8  July,2014    gur29831    SPR-12240      CA to Non - CA issue 
 *   8  July,2014    gur35095    SPR 12416       Broadcast cell delete req to all eNb cells
 *                                               because of reconfiguration
 *   14 Jul,2014     gur27356    SPR-12550       Delete the timer id "x2ap_icic_info_rpt_tmr.
 *   05 Nov,2014     gur31264    SPR-13575       support of 3 Concurrent CMAS warnings
 ******************************************************************************/

#ifndef _RRM_CELL_CONTEXT_H_
#define _RRM_CELL_CONTEXT_H_

/*! \headerfile rrm_l2_intf.h <>
 */
#include "rrm_l2_intf.h"

/*! \headerfile rrm_oam_intf.h <>
 */
#include "rrm_oam_intf.h"

/*! \headerfile rrm_cell_rrmim_intf.h <>
 */
#include "rrm_cell_rrmim_intf.h"

/*! \headerfile rrm_cell_fsm.h <>
 */

#include "rrm_cell_fsm.h"

/*! \headerfile rrm_utils.h <>
 */
#include "rrm_utils.h"

/*! \headerfile rrc_ext_api.h <>
 */
#include "rrc_ext_api.h"

/*! \headerfile rrm_uem_cellm_intf.h <>
 */
#include "rrm_uem_cellm_intf.h"

/*! \headerfile rrm_api_types.h <>
 */
#include "rrm_api_types.h"

#include "x2ap_api.h"
#include "rrc_x2apRrm_intf.h"
#include "rrc_s1apRrm_intf.h"

/*! \headerfile rrm_defines.h <>
 */
#include "rrm_defines.h"
#ifdef LTE_EMBMS_SUPPORTED
#include "rrm_cell_embms.h"
#endif

/*! \def   p_rrm_cellm_facility_name
 *  \brief  Pointer variable storing the CellM name for logging
 */
extern S8 *p_g_rrm_cellm_facility_name;

/*! \def g_cellm_log_on_off
 *  \brief  Global variable storing the status (Enabled/Disabled) of
 *          CEllM logging
 */
extern rrm_bool_et g_cellm_log_on_off;

/* DYNAMIC ICIC CHANGES START */

/*! \def MAX_NUM_CELLSIN_MACRO_ENB
 *  \brief possible max num of cells in Macro eNB  
 */
#define MAX_NUM_CELLS_IN_MACRO_ENB   24

#define RRM_BW_N6 6
#define RRM_BW_N15 15
#define RRM_BW_N25 25
#define RRM_BW_N50 50
#define RRM_BW_N75 75
#define RRM_BW_N100 100

#define PRB_COUNT_6      6
#define PRB_COUNT_15    15
#define PRB_COUNT_25    25
#define PRB_COUNT_50    50
#define PRB_COUNT_75    75
#define PRB_COUNT_100  100

#define C_PRB_STRT_BW_1_4   0 
#define C_PRB_END_BW_1_4    5 
#define C_PRB_STRT_BW_3     4  
#define C_PRB_END_BW_3     10  
#define C_PRB_STRT_BW_5     9  
#define C_PRB_END_BW_5     15  
#define C_PRB_STRT_BW_10   22  
#define C_PRB_END_BW_10    27  
#define C_PRB_STRT_BW_15   34  
#define C_PRB_END_BW_15    40  
#define C_PRB_STRT_BW_20   47  
#define C_PRB_END_BW_20    52  
/* DYNAMIC ICIC CHANGES END */

/*! \def MAX_NUM_GBR
 *  \brief possible max num of GBR  
 */
#define MAX_NUM_GBR 4

/*! \def MAX_NUM_NGBR
 *  \brief possible max num of Non GBR
 */
#define MAX_NUM_NGBR 5

/*! \def CELL_ALREADY_REGISTERD_FOR_ES 
 *  \brief bitmask to chechk cell is registered with ES
 */
#define CELL_ALREADY_REGISTERD_FOR_ES 0x01

/*! \def CELL_ALREADY_REGISTERD_FOR_ANR 
 *  \brief bitmask to check cell is registered with ANR
 */
#define CELL_ALREADY_REGISTERD_FOR_ANR 0x01

/*! \def CELL_ALREADY_REGISTERD_FOR_MLB
 *  \brief bitmask to check cell is registered with MLB
 */
#define CELL_ALREADY_REGISTERD_FOR_MLB 0x04

/* MRO code changes start */
/*! \def CELL_ALREADY_REGISTERD_FOR_MRO
 *  \brief bitmask to check cell is registered with MRO
 */
#define CELL_ALREADY_REGISTERD_FOR_MRO 0x08
/* MRO code changes end */

/*! \def RRM_RECONFIG_ONGOING 
 *  \brief bitmask to check cell reconfig procedure ongoing
 */
#define RRM_RECONFIG_ONGOING  0x02

/*! \def RRM_RECONFIG_FAIL 
 *  \brief bitmsk to check cell reconfig failed
 */
#define RRM_RECONFIG_FAIL  0x01

/*! \def MAX_NUM_UE_PER_CELLM
 *  \brief max number of possible UE in a cell 
 */

/*! \def RRM_UPDATE_ONGOING
 *  \brief bitmask to check cell update procedure ongoing
 */
#define RRM_UPDATE_ONGOING  0x04

#define RRM_UPDATE_FAIL 0x08
/* CSR_00040799_CHANGES_START */
/* #define MAX_NUM_UE_PER_CELLM 32 */
/* CSR_00040799_CHANGES_END */

/*! \def BW_PER_PRB
 *  \brief bandwith per prb
 */
#define BW_PER_PRB 180000

/* __HK__ BEGIN */
/*! \def MAX_NUM_FRAMES
 *  \brief  max number of frames
 */
#define MAX_NUM_FRAMES 8

/*! \def MAX_DURATION_FOR_KPI_REQ
 *  \brief RRM will request data from L2 in this duration
 */
#define MAX_DURATION_FOR_KPI_REQ 120

/*! \def REDUCE_DURATION_FOR_KPI_REQ
 *  \brief RRM will request data from L2 for reduce duration
 */
#define REDUCE_DURATION_FOR_KPI_REQ 60

/*! \def MAC_PRB_KPI_STATS_REQ_BIT 
 *  \brief CELLM will set the bitmask for PRB totalPRBUsage for DL,totalPRBUsage for UL, 
 *   totalPRBUsage per QCI for DL, totalPRBUsage per QCI for UL and packetDelay per QCI for DL from MAC.
 *   packetDelay bit will contain totalPktDelayInDLPerQCI and totalPktAckedInDLPerQCI
 */
#define MAC_PRB_KPI_STATS_REQ_BIT   0x00F1

/* UPDATED NRT INFO CHANGES START */

/* SPR_14067_Fix_Start */
#define RRM_GERAN_NCL 1
#define RRM_GERAN_DAHO 0
/* SPR_14067_Fix_End */

#define RRM_INTRA_FREQ_BROADCAST_NBRS_UPDATED           0x01
#define RRM_INTRA_FREQ_NON_BROADCAST_NBRS_UPDATED       0x02
#define RRM_INTER_FREQ_BROADCAST_NBRS_UPDATED           0x04
#define RRM_INTER_FREQ_NON_BROADCAST_NBRS_UPDATED       0x08
#define RRM_UTRAN_NBRS_UPDATED                          0x10
#define RRM_GERAN_NBRS_UPDATED                          0x20
#define RRM_CDMA_NBRS_UPDATED                           0x40
/*SPR 14215 start*/
#define RRM_INTRA_FREQ_X2_STATUS_UPDATED                0x80
#define RRM_INTRA_FREQ_X2_NOT_UP                        0X100
/*SPR 14215 end*/
/* UPDATED NRT INFO CHANGES END */
typedef YLIST rrm_res_list_t;

/* __HK__ END */

/*! \def MAX_NUM_QCI
 *  \brief masimu number of QCI
 */
#define MAX_NUM_QCI 9

/*! \def MAX_ENB_IDS
 *  \brief
 */
#define MAX_ENB_IDS 4

/* MEAS CONFIG REL1.2: START*/

/*! \def FREQ_NOT_CONFIGURED 
 *  \brief To represent that a freq is not configured at RRM Cell Ctxt
 */
#define FREQ_NOT_CONFIGURED  -1

#define BAND_NOT_CONFIGURED -1
/*! \def RRM_SERV_CELL_FREQ
 *  \brief To represent that the freq is serving cell freq configured 
 at RRM Cell Ctxt
 */
#define RRM_SERV_CELL_FREQ   -2

/* MEAS CONFIG REL1.2: END*/

#define SR_PERIODICITY_5MS             5
#define SR_PERIODICITY_0               0
#define SR_PERIODICITY_5MS_START_INDEX    0

#define SR_PERIODICITY_10MS            10
#define SR_PERIODICITY_1               1
#define SR_PERIODICITY_10MS_START_INDEX   5

#define SR_PERIODICITY_20MS            20
#define SR_PERIODICITY_2               2
#define SR_PERIODICITY_20MS_START_INDEX   15

#define SR_PERIODICITY_40MS            40
#define SR_PERIODICITY_3               3
#define SR_PERIODICITY_40MS_START_INDEX   35

#define SR_PERIODICITY_80MS            80
#define SR_PERIODICITY_4               4
#define SR_PERIODICITY_80MS_START_INDEX   75
/* SPR 18871 Fix Start */
#define SR_PERIODICITY_2MS             2
#define SR_PERIODICITY_5               5
#define SR_PERIODICITY_2MS_START_INDEX    155
#define SR_PERIODICITY_1MS             1
#define SR_PERIODICITY_6               6
#define SR_PERIODICITY_1MS_START_INDEX    157
/* SPR 18871 Fix End */


/*
 ** CQI periodicities and start indexws
 */

#define CQI_PERIODICITY_1MS             1
#define CQI_PERIODICITY_2MS             2
#define CQI_PERIODICITY_5MS             5
#define CQI_PERIODICITY_10MS            10
#define CQI_PERIODICITY_20MS            20
#define CQI_PERIODICITY_40MS            40
#define CQI_PERIODICITY_80MS            80
#define CQI_PERIODICITY_160MS           160

#define CQI_PERIODICITY_0               0
#define CQI_PERIODICITY_1               1
#define CQI_PERIODICITY_2               2
#define CQI_PERIODICITY_3               3
#define CQI_PERIODICITY_4               4
#define CQI_PERIODICITY_5               5
#define CQI_PERIODICITY_6               6

#define MAX_PERIODICTY_CQI_N_P              160 /* Max Np Value Check 36.213 */
#define MAX_PERIODIC_REPTITION_SR           80 /* Max Value Check 36.213 */
#define N2_PUCCH_CYCLIC_SHIFT_IN_ONE_RB                  12

/* SPR 10749 Fix Start */
// The below macro RRM_MAX_NUM_OF_UE_IN_SINGLE_RB_FOR_FORMAT2_2A_2B can only have the value form 1-12. No other value is permitted.
#define RRM_MAX_NUM_OF_UE_IN_SINGLE_RB_FOR_FORMAT2_2A_2B      12
/* SPR 10749 Fix End */
#define RI_PERIODICITY_FOR_CQI_MULTIPLE_VAL_1_START_INDEX     0 
#define RI_PERIODICITY_FOR_CQI_MULTIPLE_VAL_1_START_INDEX     0 
#define RI_PERIODICITY_FOR_CQI_MULTIPLE_VAL_2_START_INDEX     161
#define RI_PERIODICITY_FOR_CQI_MULTIPLE_VAL_4_START_INDEX     322
#define RI_PERIODICITY_FOR_CQI_MULTIPLE_VAL_8_START_INDEX     483
#define RI_PERIODICITY_FOR_CQI_MULTIPLE_VAL_16_START_INDEX    644
#define RI_PERIODICITY_FOR_CQI_MULTIPLE_VAL_32_START_INDEX    805
#define RI_PERIODICITY_1                1
#define RI_PERIODICITY_2                2
#define RI_PERIODICITY_4                4
#define RI_PERIODICITY_8                8
#define RI_PERIODICITY_16              16
#define RI_PERIODICITY_32               32

#define MRI_VAL_1                1
#define MRI_VAL_2                2
#define MRI_VAL_4                4
#define MRI_VAL_8                8
#define MRI_VAL_16               16
#define MRI_VAL_32               32

#define RI_PERIODICITY_2                2
#define RI_PERIODICITY_4                4
#define RI_PERIODICITY_8                8
#define RI_PERIODICITY_16              16
#define RI_PERIODICITY_32               32

#define RI_PERIODICITY_10MS             10
#define RI_PERIODICITY_20MS             20
#define RI_PERIODICITY_40MS             40
#define RI_PERIODICITY_80MS             80
#define RI_PERIODICITY_160MS            160
#define RI_PERIODICITY_320MS            320

#define MAX_RI_INDEX_ARRAY_SIZE             RI_PERIODICITY_320MS + 1
#define CQI_INDEX_ARRAY_SIZE            160

#define MAX_N1_PUCCH_AN                 2054
#define MAX_N2_PUCCH_AN                 2057

/* SRS Start */
#define MAX_SRS_CONFIG_INDEX            4000
#define MAX_TRANS_COMB                  2
#define MAX_PERIODIC_REPITITION_SRS     320

/* SRS_FREQ_HOPPING_START */

#define WB_SRS_CONFIG_PERCENT           0.25
/*SPR 21223 START*/
#define SB_SRS_CONFIG_PERCENT           0.75
/*SPR 21223 END*/
/* BUG_876_CHANGES_START */
#define CELL_RECONFIG_TIMER_FOR_ICIC_VALUE      7200
/* BUG_876_CHANGES_END*/

/*
** Maximum number of possible wideband and subband SRS
** configurations can be calculated in the following manner:
** 
** Maximum value for srs-SubframeConfig as per
** 3GPP specs 36.211, Table 5.5.3.3-1 = 14 
** For srs-SubframeConfig=14 , 8 SRS SFs are possible  
** -- SRS_SF_COUNT = 8
** Maximum SRS periodicity currently allocated = 80 ms
** -- SRS_RF_COUNT = 8
** Maximum possible transmissionComb  = 2
** -- TC = 2
** Maximum possible Cyclic Shifts = 8
** -- CS = 8
** Maximum possible hopping offsets as per
** 3GPP specs 36.211,Table 5.5.3.2-4 = 24
** -- N_RRC_COUNT = 24
**
**        WB_SRS_CONFIG_PERCENT * SRS_RF_COUNT * SRS_SF_COUNT * TC * CS
**
*SPR 21223 START*
**Code Deleted**
*SPR 21223 END*
 */
/*SPR 21223 START*/
/* Code Deleted */
/*SPR 21223 END*/
#define MAX_SRS_BW_CONFIG               8
#define MAX_SRS_BW                      4

/* SRS_FREQ_HOPPING_END */

/*! \def  SRS_PERIODICITY_2MS 
 *  \brief srs periodicity for 2ms
 */
#define SRS_PERIODICITY_2MS             2
/*! \def  SRS_PERIODICITY_5MS 
 *  \brief srs periodicity for 5ms
 */
#define SRS_PERIODICITY_5MS             5
/*! \def  SRS_PERIODICITY_10MS 
 *  \brief srs periodicity for 10ms
 */
#define SRS_PERIODICITY_10MS            10
/*! \def  SRS_PERIODICITY_20MS 
 *  \brief srs periodicity for 20ms
 */
#define SRS_PERIODICITY_20MS            20
/*! \def  SRS_PERIODICITY_40MS 
 *  \brief srs periodicity for 40ms
 */
#define SRS_PERIODICITY_40MS            40
/*! \def  SRS_PERIODICITY_80MS 
 *  \brief srs periodicity for 80ms
 */
#define SRS_PERIODICITY_80MS            80
/*! \def  SRS_PERIODICITY_160MS 
 *  \brief srs periodicity for 160ms
 */
#define SRS_PERIODICITY_160MS           160
/*! \def  SRS_PERIODICITY_320MS 
 *  \brief srs periodicity for 320ms
 */
#define SRS_PERIODICITY_320MS           320

#define SRS_PERIODICITY_0               0
#define SRS_PERIODICITY_1               1
#define SRS_PERIODICITY_2               2
#define SRS_PERIODICITY_3               3
#define SRS_PERIODICITY_4               4
#define SRS_PERIODICITY_5               5
#define SRS_PERIODICITY_6               6
#define SRS_PERIODICITY_7               7

#define SRS_TRANS_COMB_INDEX_0                0
#define SRS_TRANS_COMB_INDEX_1                1

/* SRS End */

/*SPR_11293_FIX_START */
/* ETWS/CMAS Data structures*/
/*Bug_12233_changes_start*/
/*SPR 22445 Fix Start*/
#define CELL_M_FSM_SET_STATE(p_cell_ctx, new_state)\
		p_cell_ctx->cell_state = new_state;
/*SPR 22445 Fix Stop*/
/*SPR_11293_FIX_END */
#define SIB_SIZE                        150
#define RRM_RRC_WINDOW_LENGTH          5
#define RRM_FRAME_PERIOD               10
/* SPR 13575 Fix Start */
//Value is changed from 2 to 3
#define MAX_CMAS_WARNING_COUNT         3
/* SPR 13575 Fix End */ 

/*SPR 5676 Fix Start*/
#define RRM_SIB_TYPE_3_PRESENT  0x01
#define RRM_SIB_TYPE_4_PRESENT  0x02
#define RRM_SIB_TYPE_5_PRESENT  0x04
#define RRM_SIB_TYPE_6_PRESENT  0x08
#define RRM_SIB_TYPE_7_PRESENT  0x10

/* Spr 18401 Changes Start */
#define RRM_SIB_TYPE_8_PRESENT  0x20
/* Spr 18401 Changes End */

/*SPR 5676 Fix End*/

/* MLB Changes start */
#define RRM_MAX_CELL_IN_ENB         256
#define RRM_MAX_X2_MEAS_ID          4095
/* MLB Changes end */

/* DYNAMIC ICIC CHANGES START */
#define MIN_CE_INTF_LVL_PRB_REPORTED_PERCENTAGE             10 
#define MAX_CE_INTF_LVL_PRB_REPORTED_PERCENTAGE             50 
#define MIN_CE_PRB_USAGE_REPORTED_PERCENTAGE                10 
#define MIN_THRESHHOLD_CE_PRB_USAGE_REPORTED_PERCENTAGE     50 
#define MIN_PRBS_LEFT_FOR_NEW_CE_UES                        4 

#define     RRM_MAX_PRBS_COUNT                             110
#define     MAX_NUM_OF_CE_PRB_SAMPLES                      10
/* DYNAMIC ICIC CHANGES END */

/** Dynamic UE Scheduling Start **/
#define RRM_DEFAULT_HYSTERESIS_DELTA             2
/** Dynamic UE Scheduling End **/

/* BUG_11848_CHANGES_START */
#define CELL_RECONFIG_TIMER_VALUE_FOR_ICIC      3600
/* BUG_11848_CHANGES_END */

/* SPR 12102 Fix Start */
#define PRACH_PREAMBLE_FMT_4_CONFIG_IDX_START   48 
#define PRACH_PREAMBLE_FMT_4_CONFIG_IDX_END     57
/* SPR 12102 Fix End */

/* eICIC_PHASE_1_2_CHANGES_START */
#define RRM_INVALID_TARGET_NCL                        0xFF
#define RRM_ONE_TWENTY_EIGHT                           128
#define RRM_MAX_ABS_PATTERN_BYTE_FDD                    5
//NEED TO CHECK
#define RRM_MAX_ABS_PATTERN_BYTE_TDD_1_5                3
#define RRM_MAX_ABS_PATTERN_BYTE_TDD_0                  9
#define RRM_MAX_ABS_PATTERN_BYTE_TDD_6                  8

#define RRM_MAX_ABS_PATTERN_BITS_FDD                   40
#define RRM_MAX_ABS_PATTERN_BITS_TDD_1_5               20
#define RRM_MAX_ABS_PATTERN_BITS_TDD_0                 70
#define RRM_MAX_ABS_PATTERN_BITS_TDD_6                 60

     /* SPR14123start*/
typedef enum
{
  RRM_EICIC_ABS_NULL,
  RRM_EICIC_ABS_PRESENT,
  RRM_EICIC_ABS_NOT_PRESENT,
}rrm_abs_type_et;

     /* SPR14123end*/
/* eICIC_PHASE_1_2_CHANGES_END */

/*SPR 7892 FIX START*/
typedef enum
{
    MAC_UE_RECONF_NOT_ONGOING = 0,
    W_FOR_MAC_UE_RECONF_CNF_FOR_UL_MIMO,
    W_FOR_MAC_UE_RECONF_CNF_FOR_ICIC,
    W_FOR_MAC_UE_RECONF_CNF_FOR_BEAMFRMG,
    /*CA Stage3:start*/
    W_FOR_MAC_UE_RECONF_CNF_FOR_CA,
    /*CA Stage3:end*/
    /* eICIC_PHASE_1_2_CHANGES_START */
    W_FOR_MAC_UE_RECONF_CNF_FOR_EICIC,
    /* eICIC_PHASE_1_2_CHANGES_END */
}rrm_mac_ue_reconfig_mutex_val_et;
/*SPR 7892 FIX END*/

typedef enum
{
    RRM_PRS_N2,
    RRM_PRS_N4,
    RRM_PRS_N8,
    RRM_PRS_N16
}rrm_prs_mutting_bits_et;

/*! \enum rrm_pws_si_index_et
 * \brief si index
 */
typedef enum
{
    RRM_SI_CURR_START_IDX = 0,
    RRM_SI_CURR_END_IDX,
    RRM_SI_PREV_START_IDX,
    RRM_SI_PREV_END_IDX
}rrm_pws_si_index_et;
/*! \enum rrm_pws_sib_type_scheduled_et
 *  \brief sib type scheduled
 */
typedef enum 
{
    RRM_PWS_SIB_TYPE_10,
    RRM_PWS_SIB_TYPE_11,
    RRM_PWS_SIB_TYPE_10_11,
    RRM_PWS_SIB_TYPE_12,
    RRM_PWS_SIB_TYPE_UNDEF
}rrm_pws_sib_type_scheduled_et;
/*! \enum rrm_cellm_pws_substate_et
 * \brief cellm substate
 */
typedef enum
{
    PWS_IDLE,
    PWS_ONGOING_ACTIVE,
    PWS_W_FOR_PWS_CNF,
    PWS_W_FOR_KILL_CNF,
    PWS_W_FOR_UPDATE_CNF,
}rrm_cellm_pws_substate_et;
/*! \enum pws_warning_status_et
 *  \brief warning status
 */
typedef enum
{
    PWS_BROADCAST_UNDEFINED,
    PWS_BROADCAST_ONGOING,
    PWS_BROADCAST_COMPLETE,
    PWS_BROADCAST_KILLED
}pws_warning_status_et;

/*Cell update change start*/
typedef enum
{
    CELL_RECONFIG_PROC = 0,
    CELL_UPDATE_PROC = 1,
    CELL_MLB_RECONFIG_PROC,
    /*DYNAMIC ICIC START*/
    CELL_RECONFIG_FOR_ICIC_PROC
        /*DYNAMIC ICIC END*/
} rrm_cell_procedure_et;
/*Cell update change end*/

/* DYNAMIC ICIC CHANGES START */                                              

typedef enum
{
    RRM_OI_HIGH_INTERFERENCE= 0,
    RRM_OI_MEDIUM_INTERFERENCE,
    RRM_OI_LOW_INTERFERENCE
} rrm_oi_interference_level_et;

typedef struct _rrm_icic_info_stored_t
{
#define   RRM_CTXT_DL_PARTIONING_INFO_PRESENT            0x0001
#define   RRM_CTXT_UL_PARTIONING_INFO_PRESENT            0x0002
#define   RRM_CTXT_UEM_UPDATE_INFO_PRESENT               0x0004
    rrm_bitmask_t bitmask;

    rrm_resource_partition_info_t      dl_resource_partition_info_new;
    rrm_resource_partition_info_t      ul_resource_partition_info_new;
    rrmuem_cm_update_ue_info_req_t     cellm_uem_ue_info_req;
}rrm_icic_info_stored_t;

typedef struct
{
    U32    prb_count;
    S32    ul_intf_overload_indication[RRM_MAX_PRBS_COUNT];
}rrm_ul_intf_overload_ind_t;

typedef struct
{
    U32  prb_count; 
    U32  hii[RRM_MAX_PRBS_COUNT]; 
}rrm_ul_high_interference_info_t;

typedef struct
{
    U32    prb_count;
    U8    rntp_per_prb[RRM_MAX_PRBS_COUNT]; 
}rrm_relative_narrowband_tx_power_t;

/*! \typedef struct rrm_l2_icic_l1_l2_report_info_t 
 *  \brief  struct for L1/L2 measurnment reporting
 *  \param bitmask
 *  \param num_of_sib_segments num_of_sib_segments
 *  \param  
 */
typedef struct _rrm_l2_icic_l1_l2_report_info_t
{
    rrm_bitmask_t bitmask;
#define RRM_CTXT_UL_INTF_OVERLOAD_IND_PRESENT       	0x0001
#define RRM_CTXT_HIGH_INTF_IND_PRESENT              	0x0002
#define RRM_CTXT_RELATIVE_NARROWBAND_TX_POWER_PRESENT 	0x0004
#define RRM_CTXT_THERMAL_NOISE_PRESENT                  0x0008
#define RRM_CTXT_DL_RS_TX_POWER_PRESENT                 0x0010
#define RRM_CTXT_DL_CE_PRB_USAGE_PRESENT                0x0020
#define RRM_CTXT_UL_CE_PRB_USAGE_PRESENT                0x0040
#define RRM_CTXT_L1_L2_CONFIGURED_REPORT_BITMASK_PRESENT        0x0080
/* SPR 21843 Fix Start */ 
#define RRM_CTXT_LOAD_REPORT_IND_PRESENT                0x0100
/* SPR 21843 Fix End */ 
    rrm_ul_intf_overload_ind_t              ul_intf_overload_ind;
    rrm_ul_high_interference_info_t         ul_high_interference_info;
    S32                                     thermal_noise;
    S32                                     dl_rs_tx_power;
    U32                                     dl_ce_prb_usage;
    U32                                     ul_ce_prb_usage;
    rrm_icic_info_stored_t                  decision_info_stored;
    U8                                      bitmask_for_l1_l2_reports_from_l2;
    /* Below written is a temporary data struct
       will be replaced when BSR will be reported from L2
     */
    rrm_relative_narrowband_tx_power_t      relative_narrowband_tx_power;
    S32                                     ul_ce_prb_usage_array[MAX_NUM_OF_CE_PRB_SAMPLES];
    U8                                      num_ce_samples;
    /* SPR 20867 Fix Start */
    rrm_bool_et                             is_sample_valid_for_partioning; 
    /* SPR 20867 Fix End */

}rrm_l2_icic_l1_l2_report_info_t;

/* DYNAMIC ICIC CHANGES END */


/*! \  sib_sheduling_periodicity_data_t
 *  \brief  Parameters required to calculate the si periodicity
 *  \param bitmask
 *  \param num_of_sib_segments num_of_sib_segments
 *  \param rrm_pws_sib_type_scheduled_et rrm_pws_sib_type_scheduled_et
 *  \param  repetition_period repetition_period
 *  \param  number_of_broadcast_requested number_of_broadcast_requested
 */
typedef struct _sib_sheduling_periodicity_data_t
{
    rrm_bitmask_t bitmask;
#define NUM_OF_SIB_SEGMENTS_PRESENT 	0x01
#define REPETITION_PERIOD_PRESENT 	0x02
#define NUMBER_OF_BROADCAST_REQ_PRESENT 0x04

    U16 num_of_sib_segments;
    rrm_pws_sib_type_scheduled_et  sib_type;
    U32 repetition_period;
    U16 number_of_broadcast_requested;
}sib_sheduling_periodicity_data_t;


/*! \  scheduling_data_key_t
 *  \brief  scheduled pws data
 *  \param U16              msg_id
 *  \param U16              serial_number
 */
typedef struct _scheduling_data_key_t
{
    U8 	msg_id[MSG_ID_OCTET_SIZE];
    U8	serial_number[SERIAL_NUMBER_OCTET_SIZE];
}scheduling_data_key_t;

/*! \   scheduled_pws_data_t
 *  \brief  scheduled pws data 
 *  \param scheduling_data_key_t   sched_data_key
 *  \param U8      	    si_index
 *  \param U8                      si_periodicity
 *  \param U8                      sib_type
 */
typedef struct _scheduled_pws_data_t
{
    scheduling_data_key_t 	sched_data_key;
    U8			si_index[4];
    U8			si_periodicity;
    /*DYNAMIC SIB SCHEDULING START*/
    U8      si_periodicity_second_sib;
    /*DYNAMIC SIB SCHEDULING END*/
    rrm_pws_sib_type_scheduled_et			sib_type;
    U8 			num_of_sib_segments;
    U8                      broadcast_status;
    U8      num_of_sibs_scheduled;
}scheduled_pws_data_t;

/*! \  scheduled_pws_data_node_t
 *  \brief Node of the List of scheduled ETWS/CMAS warnings
 *  \param YLNODE pNode
 *  \param scheduled_pws_data_t        scheduled_pws_data
 */
typedef struct _scheduled_pws_data_node_t
{
    YLNODE                      pNode;
    scheduled_pws_data_t        scheduled_pws_data;
}scheduled_pws_data_node_t;

/* ETWS/CMAS Data structures*/

#ifndef TDD_MODE_FLAG

/*CQI configuration indexes for the FDD mode */
#define CQI_PERIODICITY_2MS_FDD_START_INDEX    0
#define CQI_PERIODICITY_5MS_FDD_START_INDEX    2
#define CQI_PERIODICITY_10MS_FDD_START_INDEX   7
#define CQI_PERIODICITY_20MS_FDD_START_INDEX   17
#define CQI_PERIODICITY_40MS_FDD_START_INDEX   37
#define CQI_PERIODICITY_80MS_FDD_START_INDEX   77
#define CQI_PERIODICITY_160MS_FDD_START_INDEX   157

#define SRS_START_INDEX_FOR_2_MS        0
#define SRS_START_INDEX_FOR_5_MS        2
#define SRS_START_INDEX_FOR_10_MS       7
#define SRS_START_INDEX_FOR_20_MS       17
#define SRS_START_INDEX_FOR_40_MS       37
#define SRS_START_INDEX_FOR_80_MS       77
#define SRS_START_INDEX_FOR_160_MS      157
#define SRS_START_INDEX_FOR_320_MS      317

#define SRS_OFFSET_CALCULATION_VAL_FOR_2_MS                   0
#define SRS_OFFSET_CALCULATION_VAL_FOR_5_MS                   2
#define SRS_OFFSET_CALCULATION_VAL_FOR_10_MS                  7
#define SRS_OFFSET_CALCULATION_VAL_FOR_20_MS                  17
#define SRS_OFFSET_CALCULATION_VAL_FOR_40_MS                  37
#define SRS_OFFSET_CALCULATION_VAL_FOR_80_MS                  77
#define SRS_OFFSET_CALCULATION_VAL_FOR_160_MS                 157
#define SRS_OFFSET_CALCULATION_VAL_FOR_320_MS                 317

/* BUG_11848_CHANGES_START */
/* Code Deleted */
/* BUG_11848_CHANGES_END */
#define RRM_L2_PERIODIC_REPORT_DURATION         100
#define NUM_OF_SRS_MAX_UE_VALIDATION            128

/* ! \UE Specific SRS Periodicity & Subframe Offset Configuration table
   \for details refer table 8.2-2 of 3gpp spec 36.213
 */
#define T_OFFSET_SF_0          {0 , 1}
#define T_OFFSET_SF_1          {0 , 2}
#define T_OFFSET_SF_2          {1 , 2}
#define T_OFFSET_SF_3          {0 , 3}
#define T_OFFSET_SF_4          {1 , 3}
#define T_OFFSET_SF_5          {0 , 4}
#define T_OFFSET_SF_6          {1 , 4}
#define T_OFFSET_SF_7          {2 , 3}
#define T_OFFSET_SF_8          {2 , 4}
#define T_OFFSET_SF_9          {3 , 4}

/* ! \Frame structure type 2 sounding reference signal subframe configuration
   \for details refer table 5.5.3.3-2 of 3gpp spec 36.211
 */
#define DELTA_SFC_SF_0         {1}
#define DELTA_SFC_SF_1         {1, 2}
#define DELTA_SFC_SF_2         {1, 3}
#define DELTA_SFC_SF_3         {1, 4}
#define DELTA_SFC_SF_4         {1, 2, 3}
#define DELTA_SFC_SF_5         {1, 2, 4}
#define DELTA_SFC_SF_6         {1, 3, 4}
#define DELTA_SFC_SF_7         {1, 2, 3, 4}
#define DELTA_SFC_SF_8         {1, 2, 6}
#define DELTA_SFC_SF_9         {1, 3, 6}



#else
/* TDD related Data structures */

/*CQI configuration indexes for the TDD mode */
#define CQI_PERIODICITY_1MS_TDD_START_INDEX    0
#define CQI_PERIODICITY_5MS_TDD_START_INDEX    1
#define CQI_PERIODICITY_10MS_TDD_START_INDEX   6
#define CQI_PERIODICITY_20MS_TDD_START_INDEX   16
#define CQI_PERIODICITY_40MS_TDD_START_INDEX   36
#define CQI_PERIODICITY_80MS_TDD_START_INDEX   76
#define CQI_PERIODICITY_160MS_TDD_START_INDEX   156

#define MAX_TDD_UL_DL_CONFIG 7
#define SUBFRAMES_IN_ONE_FRAME 10
#define MAX_SRS_SF_CONFIG 14
#define MAX_OFFSETS_IN_SRS_SF_CONFIG 4
#define MAX_SRS_OFFSETS_FOR_2MS 2

#define SRS_START_INDEX_FOR_2_MS_TDD        0
#define SRS_START_INDEX_FOR_5_MS_TDD        10 
#define SRS_START_INDEX_FOR_10_MS_TDD       15
#define SRS_START_INDEX_FOR_20_MS_TDD       25  
#define SRS_START_INDEX_FOR_40_MS_TDD       45
#define SRS_START_INDEX_FOR_80_MS_TDD       85
#define SRS_START_INDEX_FOR_160_MS_TDD      165
#define SRS_START_INDEX_FOR_320_MS_TDD      325

#define SRS_OFFSET_CALCULATION_VAL_FOR_2_MS_TDD                   0
#define SRS_OFFSET_CALCULATION_VAL_FOR_5_MS_TDD                   10
#define SRS_OFFSET_CALCULATION_VAL_FOR_10_MS_TDD                  15
#define SRS_OFFSET_CALCULATION_VAL_FOR_20_MS_TDD                  25
#define SRS_OFFSET_CALCULATION_VAL_FOR_40_MS_TDD                  45
#define SRS_OFFSET_CALCULATION_VAL_FOR_80_MS_TDD                  85
#define SRS_OFFSET_CALCULATION_VAL_FOR_160_MS_TDD                 165
#define SRS_OFFSET_CALCULATION_VAL_FOR_320_MS_TDD                 325

#define SRS_INDEX_ARRAY_SIZE 320
/* BUG_11848_CHANGES_START */
/* Code Deleted */
/* BUG_11848_CHANGES_END */
#define RRM_L2_PERIODIC_REPORT_DURATION         100
#define NUM_OF_SRS_MAX_UE_VALIDATION            128

/* ! \UE Specific SRS Periodicity & Subframe Offset Configuration table
   \for details refer table 8.2-2 of 3gpp spec 36.213
 */
#define T_OFFSET_SF_0          {0 , 1}
#define T_OFFSET_SF_1          {0 , 2}
#define T_OFFSET_SF_2          {1 , 2}
#define T_OFFSET_SF_3          {0 , 3}
#define T_OFFSET_SF_4          {1 , 3}
#define T_OFFSET_SF_5          {0 , 4}
#define T_OFFSET_SF_6          {1 , 4}
#define T_OFFSET_SF_7          {2 , 3}
#define T_OFFSET_SF_8          {2 , 4}
#define T_OFFSET_SF_9          {3 , 4}

/* ! \Frame structure type 2 sounding reference signal subframe configuration
   \for details refer table 5.5.3.3-2 of 3gpp spec 36.211
 */
#define DELTA_SFC_SF_0         {1}
#define DELTA_SFC_SF_1         {1, 2}
#define DELTA_SFC_SF_2         {1, 3}
#define DELTA_SFC_SF_3         {1, 4}
#define DELTA_SFC_SF_4         {1, 2, 3}
#define DELTA_SFC_SF_5         {1, 2, 4}
#define DELTA_SFC_SF_6         {1, 3, 4}
#define DELTA_SFC_SF_7         {1, 2, 3, 4}
#define DELTA_SFC_SF_8         {1, 2, 6}
#define DELTA_SFC_SF_9         {1, 3, 6}

/* ! \enum tdd_ul_dl_sbframe_pattern_et
 * \brief ul dl subfram patterm
 */
typedef enum
{
    UL_SF,
    DL_SF,
    SPCL_SF
}tdd_ul_dl_sbframe_pattern_et;

/* TDD related Data structures */
#endif


/*! \   cellm_tcb_t
 *  \brief  Transaction Control Block of CELL FSM 
 *  \param U16              transaction_id 
 *  \param U16              api originating source(MIF/RRC)
 *  \param p_next           cellm_tcb_t
 */
typedef struct cellm_tcb_t
{
    U16 trans_id;
    U16 corresponding_api_id;
    struct cellm_tcb_t *p_next;
}cellm_tcb;
typedef EcnFeatureStatus rrm_ecn_status_et;
/*! \  rrm_cellm_ue_qci_list_ecn_t
 *  \brief This struct is used for UE QCI list ECN information 
 *  \param qci QCI value
 *  \param dir ECN status
 *  \param congestion_status Congestion status
 */
typedef struct _rrm_cellm_ue_qci_list_ecn_t
{
    U8 qci;
    rrm_ecn_status_et dir; 
    rrm_bool_et congestion_status;
}rrm_cellm_ue_qci_list_ecn_t;
/*! \ rrm_cell_ue_ecn_list_t
 *  \brief This struct is used for Cell UE ECN list
 *  \param ue_index UE index
 *  \param ue_qci_ecn[RRM_MAX_QCI] UE QCI ECN array
 */
typedef struct _rrm_cell_ue_ecn_list_t
{
    /* +- SPR 18268 */
    U16 ue_index;
    /* +- SPR 18268 */
    rrm_cellm_ue_qci_list_ecn_t ue_qci_ecn[RRM_MAX_QCI];
}rrm_cell_ue_ecn_list_t;
#define RRM_CELL_TIME_DURATION_PDCP_KPI_THP 5 /*in milisecond*/
#define RRM_CELL_PDCP_KPI_THP_BITSET 0x18  /*Fourth and Fifth bit is on*/

/* SPR-10397-fix start */

typedef rrmcm_qci_bitrate_config_t rrm_cell_qci_bitrate_config_t;

/* SPR 10813 changes start */
/* Structure definition for API : PDCP_KPI_THP_STATS_IND
 * Structure name               : rrm_cell_pdcp_kpi_thp_stat */
/*! \ rrm_cell_pdcp_kpi_thp_stat
 *  \brief This struct is used for PDCP KPI THP Stats Indication Params
 *  \param avgDurationT average duration time
 *  \param ueIndex[MAX_KPI_INDEX] array of UE index
 *  \param qci[MAX_KPI_INDEX] array of QCI
 *  \param dlIPThp[ MAX_KPI_INDEX]; array of downlink IP thp
 *  \param ulIPThp[MAX_KPI_INDEX];  array of uplink IP thp
 *  \param dlIPLatency[MAX_KPI_INDEX]; array of lategency
 *  \param PdcpSduBitrateDl[MAX_KPI_INDEX]; array of pdcp bit rate for DL
 *  \param PdcpSduBitrateUl[MAX_KPI_INDEX]; array of pdcp bit rate for UL
 *  \param endIndicator; Informs if this is the last message in sequence or not. 
 */

typedef struct _rrm_cell_pdcp_kpi_thp_stat 
{
    U32 avgDurationT;
    U16 ueIndex[ MAX_KPI_INDEX ];
    U8  qci[ MAX_KPI_INDEX ];
/*spr 19711 fix start*/ 
	/*code deleted*/
/*spr 19711 fix end*/ 
    /* 32.425 Events code start*/

    U64 dlIPThp[ MAX_KPI_INDEX ];
    U64 ulIPThp[ MAX_KPI_INDEX ];
    U32 dlIPLatency[ MAX_KPI_INDEX ];
    /* Bug_12050_Fix: Start */
    U16 cell_index[ MAX_KPI_INDEX ];
    U8 cell_index_traversed[ RRM_MAX_NUM_CELLS ];
    U64 PdcpSduBitrateDl[ MAX_KPI_INDEX ];
    U64 PdcpSduBitrateUl[ MAX_KPI_INDEX ];
    /* Bug_12050_Fix: End */
    U8 endIndicator;

    /* padding is added for byte alignment,this is reserved and can
     * be used for new fields in future.Its value should be ignored */
    U8 padding[3];
    /* SPR 10813 changes end */

}rrm_cell_pdcp_kpi_thp_stat;

/* SPR-10397-fix end */

typedef struct _rrm_cell_ecn_config_t
{
    rrm_bitmask_t bitmask;
    rrm_bool_et ecn_config_recvd;
    S8 num_of_ue;
    rrm_cell_qci_bitrate_config_t bitrate_for_cell;
    /*List will be updated on the basis of throughput received from PDCP for a particular Ue*/
    rrm_cell_ue_ecn_list_t p_ue_ecn_set_list[32];
    U8 num_active_ecn_ue;
}rrm_cell_ecn_config_t;
/*! \ rrm_cell_statistics_t
 *  \brief Struct that specifies the cell statistics.
 *  \param num_rrm_rrc_ue_admission_failure  Number of UE for which admission failed from RRM. 
 *  \param num_rrm_rrc_ue_admission_success  Number of UE for which admission succeed from RRM.
 *  \param num_rrc_rrm_ue_admission_failure  Number of UE for which admission failed from RRC.
 *  \param num_rrc_rrm_ue_admission_success  Number of UE for which admission succeed from RRC.
 *  \param num_rrm_rrc_erb_setup_failure     Number of erb setup failed at RRM. 
 *  \param num_rrm_rrc_erb_setup_success     Number of erb setup successful at RRM.
 *  \param num_rrc_rrm_erb_setup_failure     Number of erb setup failed at RRC.
 *  \param num_rrc_rrm_erb_setup_success     Number of erb setup successful at RRC.
 *  \param num_ue_admission_failure          Total number of Ue admission failure.
 *  \param num_ue_admission_success          Total number of UE admission successful.
 *  \param num_erb_setup_failure             Total number of erb setup failure.
 *  \param num_erb_setup_success             Total number of erb setup successful.
 *  \param num_ue_delete_success             Number of UE deleted successfully.
 *  \param num_ue_delete_failure             Number of UE deletion failed.
 *  \param num_erb_delete_success            Number of erb deleted successfully.
 *  \param num_erb_delete_failure            Number of erb deletion failed.
 *  \param num_rrm_rrc_erb_modify_success    Number of erb modified successfully at RRM.
 *  \param num_rrm_rrc_erb_modify_failure    Number of erb modification failed at RRM.
 *  \param num_rrc_rrm_erb_modify_success    Number of erb modified successfully at RRC.
 *  \param num_rrc_rrm_erb_modify_failure    Number of erb modification failed at RRC.
 *  \param num_erb_modify_failure            Number of erb modification failed. 
 *  \param num_erb_modify_success            Number of erb modified successfully.
 */

typedef struct _rrm_cell_statistics_t
{
    U32    num_rrm_rrc_ue_admission_failure;
    U32    num_rrm_rrc_ue_admission_success;
    U32    num_rrc_rrm_ue_admission_failure;
    U32    num_rrc_rrm_ue_admission_success;
    U32    num_rrm_rrc_erb_setup_failure;
    U32    num_rrm_rrc_erb_setup_success;
    U32    num_rrc_rrm_erb_setup_failure;
    U32    num_rrc_rrm_erb_setup_success;
    U32    num_ue_admission_failure;
    U32    num_ue_admission_success;
    U32    num_erb_setup_failure;
    U32    num_erb_setup_success;
    U32    num_ue_delete_success;
    U32    num_ue_delete_failure;
    U32    num_erb_delete_success;
    U32    num_erb_delete_failure;
    U32    num_rrm_rrc_erb_modify_success;
    U32    num_rrm_rrc_erb_modify_failure;
    U32    num_rrc_rrm_erb_modify_success;
    U32    num_rrc_rrm_erb_modify_failure;
    U32    num_erb_modify_failure;
    U32    num_erb_modify_success;
}rrm_cell_statistics_t;

/*GBR LIMIT START*/
/*! \  used_gbr_limit_t 
 *  \brief Struct that specifies the used uplink and downlink GBR limit.
 *  \param used_dl_gbr_limit  Total used downlink GBR limit. 
 *  \param used_ul_gbr_limit  Total used uplink GBR limit.
 */
typedef struct _used_gbr_limit
{

    U64 used_dl_gbr_limit;
    U64 used_ul_gbr_limit;
}used_gbr_limit_t;
/*GBR LIMIT END*/
/*Bug_776_changes_start*/
typedef struct _rrm_drb_allocated_gbr_info_t
{
    U8                dl_allocated_gbr_prb;
    U8                ul_allocated_gbr_prb;
}rrm_drb_allocated_gbr_info_t;

/*! \  rrm_cell_info_for_ue_mgr_t
 *  \brief Struct that specifies the cell information needed for UE.
 *  \param num_active_ue                   Total number of active UE in a cell. 
 *  \param dl_usable_bw_prb_for_bearers    Available bandwidth PRB for downlink
 *  \param ul_usable_bw_prb_for_bearers    Available bandwidth PRB for uplink
 *  \param dl_srb_budget_prb               Available SRB budget for downlink
 *  \param ul_srb_budget_prb               Available SRB budget for uplink
 *  \param dl_prb_budget_gbr_ngnr_combined Available PRB budget in downilnk for GBR and NGBR.
 *  \param ul_prb_budget_gbr_ngnr_combined Available PRB budget in uplink for GBR and NGBR.
 *  \param dl_gbr_prb_in_use               GBR PRB in used for downlink.
 *  \param ul_gbr_prb_in_use               GBR PRB in used for uplink.
 *  \param dl_ngbr_prb_in_use              NGBR PRB in used for downlink.
 *  \param ul_ngbr_prb_in_use              NGBR PRB in used for uplink.
    *SPR 18654 START*
 *  \param dl_allocated_gbr_prb            Allocated GBR factored PRB's (multiplied by rb_factor) for downlink.
 *  \param ul_allocated_gbr_prb            Allocated GBR factored PRB's(multiplied by rb_factor) for uplink.
 *  \param dl_allocated_ngbr_prb           Allocated NGBR factored PRB's(multiplied by rb_factor) for downlink.
 *  \param ul_allocated_ngbr_prb           Allocated NGBR factored PRB's(multiplied by rb_factor) for uplink.
 *  \param used_gbr_limit                  Struct for used GBR limit in uplink and downlink.
 */
    /*SPR 18654 END*/

typedef struct _rrm_cell_info_for_ue_mgr_t
{
    rrm_ue_index_t    num_active_ue;
    U8                dl_usable_bw_prb_for_bearers;
    U8                ul_usable_bw_prb_for_bearers;
    U8                dl_srb_budget_prb;
    U8                ul_srb_budget_prb;
    U8                dl_prb_budget_gbr_ngnr_combined;
    U8                ul_prb_budget_gbr_ngnr_combined;
    U8                dl_gbr_prb_in_use;
    /* SPR 14509 start */
    U8                dl_gbr_absprb_in_use;
    U8                dl_ngbr_absprb_in_use;
    /* SPR 14509 end */
    U8                ul_gbr_prb_in_use;
    U8                dl_ngbr_prb_in_use;
    U8                ul_ngbr_prb_in_use;
    /*SPR 18654 START*/
    /*csg start*/
    U8                 admitted_csg_user;
    U8                 admitted_non_csg_user;
    /*csg end*/
    U64                dl_allocated_gbr_prb;
    U64                ul_allocated_gbr_prb;
    U64                dl_allocated_ngbr_prb;
    U64                ul_allocated_ngbr_prb;
    /*SPR 18654 END*/
    /*GBR LIMIT START*/
    used_gbr_limit_t           used_gbr_limit;
    /*GBR LIMIT END */
    /*SPR 18654 START*/
    /*code moved*/
    /*SPR 18654 END*/
    /*start tgt_cell_ranking*/
    U64 total_cell_thp_ul;
    U64 total_cell_thp_dl;
    /*end tgt_cell_ranking*/
}rrm_cell_info_for_ue_mgr_t;


/*! \typedef enum rrm_source_value 
 *  \brief Possible Source type received from Platform 
 */
typedef enum _rrm_source_value 
{
    TNL,
    MEMORY,
    CPU,
    RCC,
    INVALID_SRC
}rrm_source_value;
/*! \ rcc_param_t 
 *  \brief  Contain the parameters for detecting RCC(Radio Congestion Control)  
 *  \param total_pkt_delay_per_qci_dl        Total packet delay per QCI.
 *  \param total_pkt_acked_dl_per_qci        Total packet Acked in downlink per QCI.
 *  \param duration duartion for which KPI request is send to L2.
 *  \param kpi_cnf Flag will be true in case failure is received from L2 for KPI stat.
 */
typedef struct _rcc_param_t
{
    U64 total_pkt_delay_per_qci_dl[RRM_MAX_QCI];
    U64 total_pkt_acked_dl_per_qci[RRM_MAX_QCI];
    U64 total_prb_usage_dl;
    U64 total_prb_usage_ul;
    /* SPR 13492 changes start */
    U64 totalSessionTimeUE;
    U64 totalSessionTimePerQci[RRM_MAX_QCI];
    /* SPR 13492 changes end */
    /* SPR 22420 Fix +- */
    rrm_bool_et kpi_cnf_err;
}rcc_param_t;

/*! \typedef enum cell_timer_et 
 *  \brief timer type in CELLM for periodic reporting of UE measurment to ANR and ES.
 */

typedef enum 
{
    CELL_PERIODIC_TIMER_FOR_ANR = 0,
    CELL_PERIODIC_TIMER_FOR_ES ,
    CELL_PERIODIC_TIMER_FOR_UE_MEAS,
    /* MLB Changes start */
    CELL_PERIODIC_TIMER_FOR_NEIGH_LOAD_INFO, /* This timer is used for reporting load of peer eNodeB */
    CELL_PERIODIC_TIMER_FOR_SELF_LOAD_INFO, /* This time is used for sending serving cell load and its neighbour load to SON/OAM */
    CELL_PERIODIC_TIMER_FOR_LD_PROCESS, /* This time type is used for cell load processing periodically */
    /* MLB Changes end */
    /* DYNAMIC ICIC CHANGES START */
    CELL_RECONFIG_TIMER_FOR_ICIC,
    /* DYNAMIC ICIC CHANGES END */
    CELL_PERIODIC_TIMER_FOR_KPI,
    /* RRM ES Start */
    CELL_PERIODIC_TIMER_FOR_ES_SELF_LOAD_INFO,/* This time is used for sending serving cell load and its neighbour load to SON_ES */
    /* RRM ES End */
   /* Carrier Aggregation start*/
   CELL_PERIODIC_TIMER_FOR_CELL_LOAD_MULTICASTING_INFO,
   /* Carrier Aggregation end */
   /* eICIC_PHASE_1_2_CHANGES_START */
   CELL_PERIODIC_TIMER_FOR_AGGRESSOR_SEL,
   CELL_GUARD_TIMER_FOR_ABS_PATTERN,
   CELL_ABS_PATTERN_UPDATE_COLLATION_TIMER,
   /* eICIC_PHASE_1_2_CHANGES_END */
   CELL_MAX_TIMER_TYPE
}cell_timer_et;

/*! \ cell_timer_buf_t
 *  \brief  Structure to store Timer Buffer in CELLM
 *  \param  cell_timer_type Timer for ANR, ES or UE_MEAS
 *  \param  cell_index      Timer started for Cell index. 
 *  \param  event_id        Event id for which timer is started. 
 */
typedef struct _cell_timer_buf_t
{
    /* BUG_825_Fix Start */
    U32  timer_type;   /*Type of timer*/
    /* BUG_825_Fix End */
    rrm_cell_index_t cell_index;
    U16 event_id;
    U32 rsu_meas_id;
}cell_timer_buf_t;

/*! \ cell_state_t 
 *  \brief Struct that contains flag for different trigger type of cell state
 *  \      Flag shall be updated on the basis of trigger type received from SON.
 *  \param  flag_periodic_cell_state  Cell state indication shall be send periodically if falg is true. 
 *  \param  flag_threshold_cell_state Cell state indication shall be send on basis of threshold if flag is true. 
 *  \param  flag_trgr_cell_state      Cell state indication shall be send on basis on trigger if flag is true. 
 */
typedef struct _cell_state_t 
{
    rrm_bool_et flag_periodic_cell_state;
    rrm_bool_et flag_threshold_cell_state;
    rrm_bool_et flag_trgr_cell_state;
}cell_state_t;


/*! \ cell_active_ue_t 
 *  \brief Struct that contains flag for different trigger type of active UE in a cell
 *        Flag shall be updated on the basis of trigger type received from SON.
 *        Active UE is periodic indication sent to SON
 *  \param  cell_timer Timer need to be start for periodic indication of Active ue in a cell. 
 *  \param  flag_periodic_active_ue  Active UE indication shall be send periodically if flag is true. 
 *  \param  flag_threshold_active_ue Active UE indication shall be send on basis of threshold if flag is true. 
 *  \param  flag_trgr_active_ue      Active UE indication shall be send on basis of trigger if flag is true. 
 */
typedef struct _cell_active_ue_t
{
    cell_timer_buf_t cell_timer; 
    rrm_bool_et flag_periodic_active_ue;
    rrm_bool_et flag_threshold_active_ue;
    rrm_bool_et flag_trgr_active_ue;
}cell_active_ue_t;


/*! \  cell_active_ue_threshold_t 
 *  \brief Struct that contains flag for different trigger type of UE threshold in a cell
 *        Flag shall be updated on the basis of trigger type received from SON.
 *  \param  trigger_type  Trigger type for active ue threshold. 
 *  \param  threshold_val Threshold value for threshold trigger type. 
 *  \param  threshold_flag Threshold falg shall be true if trigger type is threshold type. 
 *  \param  flag_periodic_ue_threshold  periodic based indication shall be send for threshold UE. 
 *  \param  flag_threshold_ue_threshold threshold based indication shall be send for threshold UE. 
 *  \param  flag_trgr_ue_threshold      trigger based indication shall be send for threshold UE. 
 */
typedef struct _cell_active_ue_threshold_t 
{
    rrm_cellm_trigger_type_t trigger_type;
    U32 threshold_val;
    rrm_bool_et   threshold_flag;
    rrm_bool_et threshold_value_down_flag;
    rrm_bool_et flag_periodic_ue_threshold;
    rrm_bool_et flag_threshold_ue_threshold;
    rrm_bool_et flag_trgr_ue_threshold;
}cell_active_ue_threshold_t;

/*! \ cell_s1_msg_notification_t
 *  \brief Struct that specifies flag for cell S1 messasge notification and trigger type.
 *  \param  trigger_type  Trigger type received from MIF for S1 message notification. 
 *  \param  flag_periodic_s1_msg_notification  periodic based indication shall be send for S1 message. 
 *  \param  flag_trgr_s1_msg_notification      trigger based indication shall be send for S1 message. 
 *  \param  flag_threshold_s1_msg_notification threshold based indication shall be send for S1 message. 
 */
typedef struct _cell_s1_msg_notification_t 
{
    rrm_cellm_trigger_type_t trigger_type;
    rrm_bool_et flag_periodic_s1_msg_notification;
    rrm_bool_et flag_trgr_s1_msg_notification;
    rrm_bool_et flag_threshold_s1_msg_notification;
}cell_s1_msg_notification_t;

/* UE MEAS CHANGES : STARTS */

/*! \ cell_ue_meas_result_notification_t
 *  \brief Struct that specifies flag for UE meas result notification and trigger type.
 *  \param  trigger_type  trigger type for UE_measurement notification. 
 *  \param  flag_periodic_ue_meas_notification Periodic based indication shall be send for Ue measurementif flag is true. 
 *  \param  flag_trgr_ue_meas_notification     trigger based indication shall be send for ue measurement if flag is true.
 *  \param  flag_threshold_ue_meas_notification Threshold based indication shall be send for Ue measurement if flag is true.  
 */
typedef struct _cell_ue_meas_result_notification_t 
{
    rrm_cellm_trigger_type_t trigger_type;
    rrm_bool_et flag_periodic_ue_meas_notification;
    rrm_bool_et flag_trgr_ue_meas_notification;
    rrm_bool_et flag_threshold_ue_meas_notification;
}cell_ue_meas_result_notification_t;

/*! \typedef enum timer_type_et 
 *  \brief  timer type for UE measurement report and active UE count 
 */
typedef enum
{
    SON_ANR_ES_ACTIVE_UE_COUNT_TIMER = 0,
    SON_UE_MEAS_CONFIG_TIMER,
    /* RRM ES Start */
    SON_ES_LD_SELF_TIMER,
    /* RRM ES End */
    SON_MAX_TIMER
}timer_type_et;

/*! \typedef enum cell_timer_status_et 
 *  \brief  Timer status for UE measurment report send to SON.
 */
typedef enum 
{
    RUNNING,
    STOPPED,
    EXPIRED
}cell_timer_status_et;

/*!  \ cell_son_attr_info_list_t
 *  \brief Struct that specifies flag for cell state, active UE, threshold base report
 *        of a cell.
 *  \param  p_cell_state Pointer for trigger type of cell state. 
 *  \param  p_active_ue  Pointer for trigger type of active UE. 
 *  \param  p_ue_threshold Pointer for trigger type of UE threshold. 
 *  \param  p_s1_msg_notification Pointer for trigger type of S1 message notification. 
 *  \param  p_ue_meas_result_notification Pointer for trigger type of UE measurement notification. 
 *  \param  cell_timer_id  timer id for periodic reporting of the attributes. 
 *  \param  timer_status   Timer status for timer is running or stopped. 
 *  \param  is_active      Flag to know that cell is registered or not.
 *  \param  is_ho_report_active Flag to know whether to send ho report to Son or not
 */
typedef struct _cell_son_attr_info_list_t
{
    cell_state_t *p_cell_state;
    cell_active_ue_t *p_active_ue;
    cell_active_ue_threshold_t *p_ue_threshold;
    cell_s1_msg_notification_t *p_s1_msg_notification;
    cell_ue_meas_result_notification_t *p_ue_meas_result_notification;
    QTIMER                   cell_timer_id[SON_MAX_TIMER];
    cell_timer_status_et     timer_status[SON_MAX_TIMER];
    rrm_bool_et              is_active;
    rrm_bool_et              is_ho_report_active;
    rrm_bool_et              is_ho_attempt_ind_active;
    rrm_bool_et              is_ho_failure_report_active;
}cell_son_attr_info_list_t;

/* UE MEAS CHANGES : ENDS */

/*! \ cell_son_info_t 
 *  \brief Struct that contains pointer for ANR and ES type of attribute info 
 *        Different type of attribute info list of cell notification for SON.
 *  \param  p_anr_info Pointer for ANR attribute information.
 *  \param  p_es_info  Pointer for ES attribute information.
 */
typedef struct _cell_son_info_t 
{
    cell_son_attr_info_list_t  *p_anr_info;
    cell_son_attr_info_list_t  *p_es_info;
    cell_son_attr_info_list_t  *p_mlb_info;
    /* MRO code changes start */
    /* Added to store MRO related information */
    cell_son_attr_info_list_t   *p_mro_info;
    /* MRO code changes end */
}cell_son_info_t;

/* UE MEAS CHANGES : STARTS */

/* MLB Changes start */
/*! \ rrm_x2ap_plmn_identity_t
 *  \brief A struct that holds x2ap plmn identity
 *  \param plmn_id PLMN id for X2 interface.
 */
typedef struct _rrm_x2ap_plmn_identity_t
{
    U8 plmn_id[MAX_PLMN_ID_BYTES];
}rrm_x2ap_plmn_identity_t;
/*! \ rrm_x2_gb_enb_id_t
 *  \brief A struct that holds Global enb id 
 *  \param plmn_identity plmn id
 *  \param enb_id enb id
 */
typedef struct _rrm_x2_gb_enb_id_t
{
    rrm_x2ap_plmn_identity_t plmn_identity;
    rrm_enb_id_info_t        enb_id; 
}rrm_x2_gb_enb_id_t;
/*! \ rrm_rsu_meas_id_node_t
 *  \brief This struct contains rrm rsu measurement id 
 *  \param node node
 *  \param rsu_meas_id rsu meas id
 *  \param g_enb_id    g enb id
 */
typedef struct _rrm_rsu_meas_id_node_t
{
    YLNODE node;
    U32    rsu_meas_id;
    rrm_x2_gb_enb_id_t g_enb_id;
}rrm_rsu_meas_id_node_t;

typedef YLIST rrm_rsu_meas_id_list_t; /* rrm_rsu_meas_id_node_t */
/*! \enum rrm_calls_allowed_et
 *  \brief This enum gives all the calls allowed
 */
typedef enum
{
    RRM_ALL_CALLS,
    RRM_EMRGCY_CALLS,
    RRM_PRIVILEGE_CALLS,
    RRM_NO_CALLS
}rrm_calls_allowed_et;

/* MLB Changes end */
typedef YLIST meas_eutran_context_list_t;
typedef YLIST meas_utran_context_list_t;
/* TDD Support SON Start */
typedef YLIST meas_utran_tdd_context_list_t;
/* TDD Support SON End */
typedef YLIST meas_geran_context_list_t;
typedef YLIST meas_cdma_context_list_t ;

/* X2AP START */
typedef YLIST rrm_x2ap_enb_list_t;
/* X2AP END */

/* MLB X2AP RSU START */
typedef YLIST rrm_rsu_trans_info_list_t;
/* MLB X2AP RSU END */

/*! \typedef enum meas_type_et 
 *  \brief  enum for measurement type HO , ANR and ECID
 */
typedef enum _meas_type_et
{
    MEAS_TYPE_ANR = 0,
    MEAS_TYPE_HO,
    MEAS_TYPE_ECID,
    MEAS_TYPE_RACH,
    MEAS_TYPE_CA, 
    /* SPR-12035-fix start */
    MEAS_TYPE_REDIRECTION,
    /* SPR-12035-fix end */


    MEAS_TYPE_INVALID,
    /* eICIC_PHASE_1_2_CHANGES_START */
    MEAS_TYPE_FOR_VICTIM
    /* eICIC_PHASE_1_2_CHANGES_END */
/* ENDC_MEAS_CHANGES_START */
#ifdef ENDC_ENABLED
   ,MEAS_TYPE_EUTRA_NR
#endif
/* ENDC_MEAS_CHANGES_END */
}meas_type_et;

/*! \ rrmcm_meas_config_utran_fdd_t
 *  \brief  Struct that contains fdd utran meas params that are 
 *          not part of SIB
 *  \param  prev_offset_frequency Offset Frequency stored before meas
 *                                config req is recvd                                       
 *  \param  offset_frequency      Offset Frequency value recvd in meas config
 *                                req
 */
typedef struct _rrmcm_meas_config_utran_fdd_t
{
    U16        uarfcn;
    S8         prev_offset_frequency; 
    S8         offset_frequency; 
}rrmcm_meas_config_utran_fdd_t;

/* TDD Support SON Start */
/*! \ rrmcm_meas_config_utran_tdd_t
 *  \brief  Struct that contains tdd utran meas params that are 
 *          not part of SIB
 *  \param  prev_offset_frequency Offset Frequency stored before meas
 *                                config req is recvd                                       
 *  \param  offset_frequency      Offset Frequency value recvd in meas config
 *                                req
 */
typedef struct _rrmcm_meas_config_utran_tdd_t
{
    U16        uarfcn_tdd;
    S8         prev_offset_frequency; 
    S8         offset_frequency; 
}rrmcm_meas_config_utran_tdd_t;
/* TDD Support SON End */

/*! \ meas_utran_node_t 
 *  \brief Struct that contains utran meas node.
 *  \param  sNode Node created.
 *  \param  measobj_id measurement object id of the node.
 *  \param  meas_type Measurement type of the node.
 *  \param  utran_config Structure for UTRAN config.
 */
typedef struct _meas_utran_node_t 
{
    YLNODE                                sNode;    
    meas_type_et                          meas_type;
    S8					                  cell_ctx_index;
    rrmcm_meas_config_utran_fdd_t         utran_config;
}meas_utran_node_t;


/* TDD Support SON Start */
/*! \ meas_utran_tdd_node_t 
 *  \brief Struct that contains utran_tdd meas node.
 *  \param  sNode Node created.
 *  \param  measobj_id measurement object id of the node.
 *  \param  meas_type Measurement type of the node.
 *  \param  utran_tdd_config Structure for UTRAN TDD config.
 */
typedef struct _meas_utran_tdd_node_t 
{
    YLNODE                                sNode;    
    meas_type_et                          meas_type;
    S8					                  cell_ctx_index;
    rrmcm_meas_config_utran_tdd_t     utran_tdd_config;
}meas_utran_tdd_node_t;
/* TDD Support SON End */

/*! \ rrmcm_meas_config_eutran_t
 *  \brief  Struct that contains eutran meas params that are 
 *          not part of SIB
 *  \param  offset_frequency Offset Frequency
 */
typedef struct _rrmcm_meas_config_eutran_t
{
    U32                         earfcn;
    S8                          prev_offset_frequency; 
    rrm_q_offsetrange_et        offset_frequency;
}rrmcm_meas_config_eutran_t;

/*! \ meas_eutran_node_t 
 *  \brief Struct that contains eutran meas node.
 *  \param  sNode Node created.
 *  \param  measobj_id measurement object id of the node.
 *  \param  meas_type Measurement type of the node.
 *  \param  eutran_config structure for EUTRAN config .
 */
typedef struct _meas_eutran_node_t 
{
    YLNODE                         sNode;    
    meas_type_et                   meas_type;
    S8	  			   cell_ctx_index;
    rrmcm_meas_config_eutran_t     eutran_config;
}meas_eutran_node_t;

/*! \ rrmcm_geran_carrier_freq_info_t
 *  \brief  Struct that contains geran carrier frequency info
 *  \param  starting_arfcn
 *  \param  prev_offset_frequency
 *  \param  offset_frequency
 */
typedef struct _rrmcm_geran_carrier_freq_info_t
{
    U16                                 starting_arfcn;
    rrm_son_geran_band_indicator_et     band_ind;
    S8                                  prev_offset_frequency; 
    S8                                  offset_frequency;
    /* Spr 18401 Changes Start */
    rrm_son_geran_following_arfcns_t    following_arfcns;
    /* Spr 18401 Changes Start */
}rrmcm_geran_carrier_freq_info_t;

/*! \ meas_geran_node_t 
 *  \brief Struct that contains geran meas node.
 *  \param  sNode Node created.
 *  \param  measobj_id measurement object id of the node.
 *  \param  meas_type Measurement type of the node.
 *  \param  geran_config structure for GERAN config.
 */
typedef struct _meas_geran_node_t 
{
    YLNODE                             sNode;    
    meas_type_et                       meas_type;
    S8	    			               cell_ctx_index;
    rrmcm_geran_carrier_freq_info_t    geran_config;
}meas_geran_node_t;



/*! \struct _rrmcm_cdma2000_carrier_freq_info_t
 *  \brief  Struct that contains CDMA meas params that are
 *          not part of SIB
 *  \param  offset_frequency Offset Frequency
 */
typedef struct _rrmcm_cdma_carrier_freq_info_t
{
    //rrm_son_cdma2000_type_et                cdma_type;
    U8                cdma_type;
    /*^ M, 0, B, 0, 1 ^*/
    //rrm_son_cdma_band_class_et              band_class;
    U8              band_class;
    U16                                 arfcn;
    S8                                  prev_offset_frequency;
    S8                                  offset_frequency;
}rrmcm_cdma_carrier_freq_info_t;

/*! \struct meas_cdma2000_node_t
 *  \brief Struct that contains geran meas node.
 *  \param  sNode Node created.
 *  \param  measobj_id measurement object id of the node.
 *  \param  meas_type Measurement type of the node.
 *  \param  cdma_config structure for CDMA config.
 */
typedef struct _meas_cdma_node_t
{
    YLNODE                             sNode;
    U8                                 measobj_id;
    meas_type_et                       meas_type;
    S8                                 cell_ctx_index;
    rrmcm_cdma_carrier_freq_info_t     cdma_config;
}meas_cdma_node_t;

/* UE MEAS CHANGES : ENDS */

/**CLPC_MR_START*/
/*! \ rrm_ue_tpc_rnti_row_pucch_t
 *  \brief Struct that contains parameter for needed for CLPC.
 *  \param  drx_prof_indx DRX profile index.
 *  \param  dci_format Dci format 3/3A.
 *  \param  sps_enabled for SPS active UE.
 *  \param  busy_field Profile index is busy or not.
 *  \param  p_tpc_id  Pointer for tpc id.
 */

typedef struct _rrm_ue_tpc_rnti_row_pucch_t
{
    U8 drx_prof_indx;
    rrm_dci_format_et dci_format;
    /* SPS related changes start */
    rrm_bool_et sps_enabled;
    /* SPS related changes end */
    rrm_tpc_rnti_status_et busy_field;
    U8 occupied_tpc_id_count;
    U32 *p_tpc_id;
}rrm_ue_tpc_rnti_row_pucch_t;

/*! \ rrm_ue_tpc_rnti_tbl_pucch_t
 *  \brief Struct that contains parameter for needed for CLPC.
 *  \param  num_of_row Number of row of TPC rnti table.
 *  \param  p_p_rrm_ue_tpc_rnti_row double pointer of struct rrm_ue_tpc_rnti_roww
 _pucch_t.
 */
typedef struct _rrm_ue_tpc_rnti_tbl_pucch_t
{
    U8 num_of_row;
    rrm_ue_tpc_rnti_row_pucch_t **p_p_rrm_ue_tpc_rnti_row_pucch;
}rrm_ue_tpc_rnti_tbl_pucch_t;

/*! \ rrm_ue_tpc_rnti_row_t
 *  \brief Struct that contains parameter for needed for CLPC.
 *  \param  drx_prof_indx DRX profile index.
 *  \param  sps_enabled for SPS active UE.
 *  \param  busy_field Profile index is busy or not.
 *  \param  p_tpc_id  Pointer for tpc id.
 */

typedef struct _rrm_ue_tpc_rnti_row_pusch_t
{
    /* SPR 18621 Changes Start */
    rrm_dci_format_et dci_format;
    /* SPR 18621 Changes Stop */
    /* SPS related changes start */
    /* SPS related changes end */
    rrm_tpc_rnti_status_et busy_field;
    U32 *p_tpc_id;
    U8 sps_enabled;
    U8 occupied_tpc_id_count;
    U8 drx_prof_indx;
}rrm_ue_tpc_rnti_row_pusch_t;

/*! \ rrm_ue_tpc_rnti_tbl_pusch_t
 *  \brief Struct that contains parameter for needed for CLPC.
 *  \param  num_of_row Number of row of TPC rnti pusch table.
 *  \param  p_p_rrm_ue_tpc_rnti_row_pusch double pointer of struct rrm_ue_tpc_rnti_row_pusch_t.
 */
typedef struct _rrm_ue_tpc_rnti_tbl_pusch_t
{
    U8 num_of_row;
    rrm_ue_tpc_rnti_row_pusch_t **p_p_rrm_ue_tpc_rnti_row_pusch;
}rrm_ue_tpc_rnti_tbl_pusch_t;

/**CLPC_MR_END*/

/*! \ rrm_cqi_ri_res_list_t
 *  \brief Structure for Channel quality indicator.
 *  \param row_num
 *  \param col_num
 *  \param cqi_config_index
 *  \param ri_config_index
 *  \param cqi_pucch_res_index
 *  \param cqi_ri_marked_status
 */
typedef struct _rrm_cqi_ri_res_list_t
{
    U8                    row_num;
    U16                   col_num;
    U16                   cqi_config_index;
    U16                   ri_config_index;
    U8                    cqi_pucch_res_index;
    rrm_res_status_et     cqi_ri_marked_status;
}rrm_cqi_ri_res_list_t;


/*! \ rrm_sr_res_list_t
 *  \brief This struct contains sr result list
 *  \param row_num row number
 *  \param col_num column number
 *  \param sr_config_index sr config index
 *  \param sr_pucch_res_index sr puchh res index
 *  \param sr_marked_status   sr marked index
 */
typedef struct _rrm_sr_res_list_t
{
    U8                 row_num;
    U16                col_num;
    U8                 sr_config_index;
    U8                 sr_pucch_res_index;
    rrm_res_status_et  sr_marked_status;
    /*SPR 15713 Fix Start*/
    rrm_bool_et        sr_conflict_resolve;
    /*SPR 15713 Fix End*/
}rrm_sr_res_list_t;

/* TDD_SRS_FREQ_HOPPING_START */
/* SRS End */
/** SRS table to be used in FDD duplexing mode **/
/*! \ rrm_wb_srs_table_t
 *  \brief  This struct contains the wb srs table
 *  \param  srs_index         srs index
 *  \param  comb              comb
 *  \param  c_shift           c shift
 *  \param  srs_marked_status srs marked status
 *  \param  anchor            anchor
 */
typedef struct _rrm_wb_srs_table_t
{
    U16 srs_index;
    U8 comb;
    U8 c_shift;
    rrm_res_status_et srs_marked_status;
    U16 anchor; 
    /* SPR_8391_CHANGES_START*/
    U8 srs_bw;
    U8 srs_hopping_bw;
    /* SPR_8391_CHANGES_END*/
}rrm_wb_srs_table_t;
/*! \ rrm_sb_srs_table_t
 *  \brief This struct contains the srs sb table
 *  \param srs_index srs_index
 *  \param n_rrc n_rrc
 *  \param comb comb
 *  \param c_shift c_shift
 *  \param rrm_res_status_et rrm_res_status_et
 *  \param anchor anchor
 */
typedef struct _rrm_sb_srs_table_t
{
    U16 srs_index;
    U8 n_rrc;
    U8 comb;
    U8 c_shift;
    rrm_res_status_et srs_marked_status;
    U16 anchor; 
    /* SPR_8391_CHANGES_START*/
    U8 srs_bw;
    U8 srs_hopping_bw;
    /* SPR_8391_CHANGES_END*/
}rrm_sb_srs_table_t;

/* SPR 21223 Start */
/*! \ rrm_srs_table_t
 *  \brief This struct contains info about srs table
 *  \param res_anchor_for_switch_wideband_to_subband_srs res anchor for switch wideband to subband srs
 *  \param *p_wideband_srs_table wideband_srs_table
 *  \param *p_subband_srs_table  subband_srs_table
 */
typedef struct _rrm_srs_table_t
{
    U16 res_anchor_for_switch_wideband_to_subband_srs;
    rrm_wb_srs_table_t     *p_wideband_srs_table;
    rrm_sb_srs_table_t     *p_subband_srs_table;
    /* SPR 21223 End */
}rrm_srs_table_t;
/*! \enum srs_bandwidth_et
  \brief This enum stores info srs bw
 */
typedef enum
{
    SRS_BW0,
    SRS_BW1,
    SRS_BW2,
    SRS_BW3
}srs_bandwidth_et;

/*! \enum rrm_srs_action_on_reconfig_et
 *  \brief This enum stores info about action on reconfig
 */
typedef enum
{
    RRM_SRS_ACT_FREE_RES,
    RRM_SRS_ACT_ALLOC_RES,
    RRM_SRS_ACT_FREE_AND_ALLOC_RES,
    RRM_SRS_ACT_NO_CHANGE 
}rrm_srs_action_on_reconfig_et;
/*TDD_SRS_FREQ_HOPPING_END */


/* MC,MR & HO Start */
/*! \ rrm_cell_ho_params_t
 *  \brief A struct that holds cfg values for Meas config & HO
 *  \param ho_blind_timer_duration ho blind timer duration
 *  \param a3_a4_event_trigger_quantity event trigger quantity
 *  \param a5_event_trigger_quantity    a5 event trigger quantity
 *  \param delayed_a3_enabled           delayed a3 enabled
 *  \param a4_meas_cfg_enabled          a4 measurement enabled  
 *  \param b1_event_trigger_quantity    b1 event trigger quantity
 *  \param b2_event_utran_trigger_quantity b2 event utran trigger quantity
 *  \param csfb_blind_timer_duration        csfb blind timer duration
 *  \param b2_event_eutran_thresh_rsrx      b2 event eutran thresh rsrx
 *  \param irat_offset                      irat offset
 *  \param s1_direct_fwd_path_available     s1 direct fwd path available
 *  \param spid_for_cdma                    spid_for_cdma 

 */
typedef struct _rrm_cell_ho_params_t 
{
#define RRM_CELLM_CSFB_RELEASE		1
#define RRM_CELLM_CSFB_PS_HANDOVER	2
    U16   ho_blind_timer_duration;
    U8    a3_a4_event_trigger_quantity;
    U8    a5_event_trigger_quantity;
    U8    delayed_a3_enabled;
    U8    a4_meas_cfg_enabled;
    U8    b1_event_trigger_quantity;
    U8    b1_threshold_cdma2000;
    U8    b2_event_utran_trigger_quantity;
    U16   csfb_blind_timer_duration;
    U8   b2_event_eutran_thresh_rsrx;
    S8  irat_offset;
    U8    s1_direct_fwd_path_available;
    U8   spid_for_cdma;
    rrm_enable_ue_measurement_et rrm_enable_ue_measurement;
    /*SPR 21365 fix start*/
    /*Lines Deleted*/
    /*SPR 21365 fix stop*/
    /* bug 866 start */
    U8    s_measure;
    /* bug 866 end */
    /* Bug-10188 Fix: Start */
    U8  utra_rnc_direct_fwd_path_available;
    /* Bug-10188 Fix: End */
    /* SPR 17784 Start */
    U16 cgi_timer_duration;
    /* SPR 17784 End */
}rrm_cell_ho_params_t;

/*! \ rrm_x2ap_srvd_cell_info_st
 *  \brief A struct that holds X2 served Physical cell ID
 *  \param pci
 */
typedef struct _rrm_x2ap_srvd_cell_info_st
{
    U32                               pci; 
}rrm_x2ap_srvd_cell_info_st;

/*! \ rrm_x2ap_neighbour_info_st
 *  \brief A struct that holds X2 neighbour Physical cell ID
 *  \param pci
 */
typedef struct _rrm_x2ap_neighbour_info_st
{
    U32                               pci;
}rrm_x2ap_neighbour_info_st;

/*! \def MAX_NEIGHBOURS
 *  \brief maximum possible neighbours  
 */
#define MAX_NEIGHBOURS 256

/*! \ rrm_x2ap_nbour_cell_arry_st
 *  \brief A struct that holds X2 neighbour cell information
 *  \param num_neighbour_cell U16
 *  \param neighbour_info_arr[MAX_NEIGHBOURS] rrm_x2ap_neighbour_info_st
 */
typedef struct _rrm_x2ap_nbour_cell_arry_st
{
    U16                         num_neighbour_cell;
    rrm_x2ap_neighbour_info_st      neighbour_info_arr[MAX_NEIGHBOURS];
}rrm_x2ap_nbour_cell_arry_st;

/*! \ rrm_x2ap_cell_info_st
 *  \brief A struct that holds X2 served & neighbour cell information
 *  \param served_cell_info  served cell info
 *  \param nbour_cell_info   neighbour cell info
 */
typedef struct _rrm_x2ap_cell_info_st
{
    rrm_x2ap_srvd_cell_info_st      served_cell_info;     /*^ M,0,N,0,0 ^*/
    rrm_x2ap_nbour_cell_arry_st     nbour_cell_info;      /*^ M,0,N,0,0 ^*/
}rrm_x2ap_cell_info_st;

/*! \ rrm_x2_cell_info_t
 *  \brief A struct that holds X2 neighbour cell information
 *  \param num_served_cell  number of served cell
 *  \param cell_info[MAX_SERVED_CELLS] cell_info 
 */
typedef struct _rrm_x2_cell_info_t
{
    U16                      num_served_cell; /*^ M,0,N,0,0 ^*/
    rrm_x2ap_cell_info_st    cell_info[MAX_SERVED_CELLS];  /*^ M,0,OCTET_STRING,VARIABLE ^*/   
}rrm_x2_cell_info_t;
/* MC,MR & HO end */
/*! \  cell_radio_usage_params_t
 *  \brief A struct that holds congestion param received from OAM.
 *  \param threshold_present field for THRESHOLD request from OAM or not.
 *  \param threshold_overload field for threshold overload.
 *  \param threshold_high field for threshold high.
 *  \param threshold_normal field for threshold normal.
 *  \param max_for_num_of_sample field for maximum number of sample.
 */
typedef struct _cell_radio_usage_params_t
{
    rrm_bool_et radio_usage_flag; /*It will indeicate that radio usage param is recieved from OAM*/
    U64 threshold_overload;
    U64 threshold_high;
    U64 threshold_normal;
    U64 threshold_overload_for_cell;
    U64 threshold_high_for_cell;
    U64 threshold_normal_for_cell;
    /*Remainig value are in operator_info as RRM is reading these from File:rrm_oam.cfg*/
}cell_radio_usage_params_t;

#define MAX_SRC_TYPE 4
#define CELL_LOAD_TYPE 2
#define RRM_LOAD_TYPE_COL 1 
/*! \ load_state_t 
 *  \brief A struct that holds Load state of a cell  
 *  \param load_state load state of a cell.
 *  \param load_src_and_type 2D array containg src and load type.
 */
/* load_src_and_type is 2D array containing 
 *  --------------------------
 *   SRC    | LOAD_TYPE
 *  -------------------------
 *   TNL    | LOW
 *  -------------------------
 *   MEMORY | LOW
 *  -------------------------
 *   CPU    | LOW
 *  -------------------------
 *   RCC    | LOW
 *  -------------------------
 */
typedef struct _load_state_t
{
    rrm_load_type load_state;
    U8 load_src_and_type[MAX_SRC_TYPE][CELL_LOAD_TYPE];
}load_state_t;

/* MEAS CONFIG REL1.2: START*/
#define INVALID_OFFSET_FREQ -127

#define RRMCM_SERV_CELL_Q_QUALMIN_R9_PRESENT     0x01
#define RRMCM_SERV_CELL_THRESHX_LOWQ_R9_PRESENT  0x02
typedef struct _serv_cell_params_t
{
    rrm_bitmask_t                   bitmask;
    intra_freq_params_t            idle_mode_mobility_intra_freq_params;
    rrm_intra_freq_cell_reselection_info_t  cell_reselection_info;
    S8                              q_qualmin_r9;
    U8                              threshx_lowq_r9;
}serv_cell_params_t;

typedef struct _rrmcm_eutran_params_t
{
    U8                             num_valid_inter_freq_list;
    inter_freq_params_t            idle_mode_mobility_inter_freq_params[MAX_NO_INTER_FREQ];
    S8                             offset_freq[MAX_NO_INTER_FREQ];
}rrmcm_eutran_params_t;

typedef struct _rrmcm_utran_params_t
{
    idle_mode_mobility_irat_utra_params_t  idle_mode_mobility_irat_utra_params;
    S8                             offset_freq[RRM_MAX_NUM_UTRA_FDD_CARRIERS];
}rrmcm_utran_params_t;

typedef struct _rrmcm_geran_params_t
{
    idle_mode_mobility_irat_geran_params_t  idle_mode_mobility_irat_geran_params;
    S8                             offset_freq[MAX_NO_GERAN_FREQ_GROUUP];
}rrmcm_geran_params_t;


typedef struct _rrmcm_cdma_params_t
{
    rrmcm_idle_mode_mobility_irat_cdma_params_t  idle_mode_mobility_irat_cdma_params;
    S8                                           offset_freq[MAX_NO_CDMA_FREQ_GROUUP];
    U8                                           num_valid_cdma2000_freq_cells;
    rrmcm_rmif_cdma2000_freq_cells_t             cdma2000_freq_cells[RRMCM_RMIF_NUM_CDMA2000_FREQ_CELLS];

}rrmcm_cdma_params_t;

#define RRMCM_SERV_CELL_PARAMS_PRESENT  0x01
#define RRMCM_INTER_FREQ_PARAMS_PRESENT 0x02
#define RRMCM_UTRA_FREQ_PARAMS_PRESENT  0x04
#define RRMCM_GERAN_FREQ_PARAMS_PRESENT 0x08

#define RRMCM_CDMA_FREQ_PARAMS_PRESENT 0x010


typedef struct _rrmcm_meas_config_sib_info_for_reconfig_t
{
    rrm_bitmask_t                   bitmask;
    rrm_bool_et                     req_for_obj_remove;
    serv_cell_params_t              serv_cell_params;
    rrmcm_eutran_params_t           eutran_params;
    rrmcm_utran_params_t            utran_params;
    rrmcm_geran_params_t            geran_params;
    rrmcm_cdma_params_t            cdma_params;

}rrmcm_meas_config_sib_info_for_reconfig_t;

/* MEAS CONFIG REL1.2: END*/

/* BUG_11317_FIX_START */
/* Uplink power control starts */
typedef struct _rrm_olpc_pusch_params_t
{
    U8 olpc_pusch_supported_table[RRM_OLPC_CATEGORY_TABLE_SIZE];
    U8 olpc_pusch_snr_threshold_for_cell_center_ue;
    U8 olpc_pusch_snr_threshold_for_cell_edge_ue;
    S8 olpc_p0_ue_pusch_range_start;
    S8 olpc_p0_ue_pusch_range_end;
} rrm_olpc_pusch_params_t;
/*! \ rrm_olpc_pucch_params_t
 * \brief This struct contains olpc params
 * \param olpc_pucch_supported_table[RRM_OLPC_CATEGORY_TABLE_SIZE] olpc_pucch_supported_table
 * \param olpc_pucch_snr_threshold olpc pucch snr threshold
 * \param olpc_p0_ue_pucch_range_start
 * \param olpc_p0_ue_pucch_range_end
*/
typedef struct _rrm_olpc_pucch_params_t
{
    U8 olpc_pucch_supported_table[RRM_OLPC_CATEGORY_TABLE_SIZE];
    U8 olpc_pucch_snr_threshold;
    S8 olpc_p0_ue_pucch_range_start;
    S8 olpc_p0_ue_pucch_range_end;
} rrm_olpc_pucch_params_t;

/* BUG_11317_FIX_END */

/*! \ rrm_olpc_params_t
 * \brief rrm olpc params info
 * \param rrm_olpc_pusch_config_params
 * \param rrm_olpc_pucch_config_params
 * \param ul_pathloss_threshold

 */
typedef struct _rrm_olpc_params_t
{
    rrm_olpc_pusch_params_t rrm_olpc_pusch_config_params;
    rrm_olpc_pucch_params_t rrm_olpc_pucch_config_params;
    U8 ul_pathloss_threshold;
    /*DYNAMIC ICIC START*/
    U8 ue_handover_count_threshold;
    S8 min_ue_power;
    /*DYNAMIC ICIC END*/
} rrm_olpc_params_t;
/* Uplink power control end */
/*! \enum rrm_action_et
 * \brief rrm action information

 */
typedef enum
{
    RRM_ACTN_NONE,
    RRM_ACTN_HANDOVER,
    RRM_ACTN_SSAC_BARRING,
    RRM_ACTN_STOP_ADM,
    RRM_ACTN_REL_UE_CARR_REDIRECT,
    RRM_ACTN_REL_UE,
    RRM_ACTN_AQM,
    RRM_ACTN_RRM_ECN_UL_DIR,
    RRM_ACTN_RRM_ECN_DL_DIR,
    RRM_ACTN_RRM_ECN_BI_DIR,
    /* Carrier_Aggregation_Start */
    RRM_ACTN_RRM_SCELL_DEACTIVATE,
    /* Carrier_Aggregation_End */
    /* eICIC_PHASE_1_2_CHANGES_START */
    RRM_ACTN_RRM_SEND_ABS_TO_L2,
    /* eICIC_PHASE_1_2_CHANGES_END */
    RRM_ACTN_MAX
}rrm_action_et;

/* MLB Ph2 Start */
/*! \rrmcm_rmif_ac_barring_config_t
 *  \brief  Struct contains AC Barring Param configuration.
 *  \param  ac_barring_factor presents the factor value.
 *  \param  ac_barring_time presents the time till barred
 *  \param  ac_barring_spec_ac presents the special AC, only 5 bits used
 *          to indicate AC 11-15 presents.
 */
typedef struct _rrm_cellm_ac_barring_ld_info_t
{
    U8 ac_barring_factor;   /*^ M, 0, H, 0, 16 ^*/ /* ENUM */
    U8 ac_barring_time;     /*^ M, 0, H, 0, 7 ^*/
    U8 ac_barring_spec_ac;  /*^ M, 0, H, 0, 31 ^*/
}rrm_cellm_ac_barring_ld_info_t;

/*! \rrmcm_rmif_load_ac_barring_t
 *  \brief  Struct contains AC Barring Action that needs to be taken under various load
 *          condition.
 *  \param  mmtel_video indicates that the action should be applied for video if present.
 *  \param  mmtel_voice indicates that the action should be applied for voice if present.
 *  \param  mo_data indicates that that the action should be applied for MO Data if present.
 *  \param  mo_sigl indicates that that the action should be applied for MO Sigl if present.
 */

typedef struct _rrm_cellm_load_ac_barring_t
{
#define RRM_CELLM_LDACB_MMTEL_VIDEO_PRESENT 0x01
#define RRM_CELLM_LDACB_MMTEL_VOICE_PRESENT 0x02
#define RRM_CELLM_LDACB_MMTEL_MODATA_PRESENT 0x04
#define RRM_CELLM_LDACB_MMTEL_MOSIGL_PRESENT 0x08
/*SPR 17786 Fix Start */
#define RRM_CELLM_LDACB_CSFB_R10_PRESENT 0x10
    rrm_bitmask_t                   bitmask;            /*^ BITMASK ^*/
    rrm_cellm_ac_barring_ld_info_t  mmtel_video;        /*^ O, RRM_CELLM_LDACB_MMTEL_VIDEO_PRESENT, N, 0, 0 ^*/
    rrm_cellm_ac_barring_ld_info_t  mmtel_voice;        /*^ O, RRM_CELLM_LDACB_MMTEL_VOICE_PRESENT, N, 0, 0 ^*/
    rrm_cellm_ac_barring_ld_info_t  mo_data;            /*^ O, RRM_CELLM_LDACB_MMTEL_MODATA_PRESENT, N, 0, 0 ^*/
    rrm_cellm_ac_barring_ld_info_t  mo_sigl;            /*^ O, RRM_CELLM_LDACB_MMTEL_MOSIGL_PRESENT, N, 0, 0 ^*/
    rrm_cellm_ac_barring_ld_info_t  csfb_r10;           /*^ O, RRM_CELLM_LDACB_CSFB_R10_PRESENT, N, 0, 0 ^*/
}rrm_cellm_load_ac_barring_t;
/*SPR 17786 Fix End */
/* Bug_944_start */
/*! \rrm_cellm_redirect_info_t
 *  \brief This Struct contains MARD information that needs to be taken
 *  redirection of UE on load based scenario.
 *  \param bitmask to check optional field is present or not
 *  \param mard_present to check mard is present or not
 *  \param rwr_timer indicates measurement based rwr timer value 
 *  the duration for which RRM will wait for the measurement report 
 *  to select the suitable frequency.
 */
#define RRM_CELLM_MARD_PRESENT 0x01
#define RRM_CELLM_MEAS_BASED_RWR_TIMER_PRESENT 0x02

typedef struct _rrm_cellm_redirect_info_t
{
    rrm_bitmask_t   bitmask;        /*^ BITMASK ^*/
    rrm_bool_t      mard_present;   /*^ O, RRM_CELLM_MARD_PRESENT, B, 0,1 ^*/
    U8              rwr_timer;      /*^ O, RRM_CELLM_MEAS_BASED_RWR_TIMER_PRESENT, B, 5,120 ^*/
}rrm_cellm_redirect_info_t;
/* Bug_944_end */

/* MLB Ph2 End */

/** eICIC_PHASE_1_2_CHANGES_START */
#define RRM_CELLM_ELIGIBLE_UE_PRESENT   0x01
#define RRM_CELLM_ABS_PATTERN_PRESENT   0x02
/*! \rrm_cellm_eicic_load_config_info_t
 *  \brief Structure contains eCICI load
 *         related information.
 *  \param  bitmask
 *  \param  eligible_ue
 *  \param  abs_pattern
 */
typedef struct rrm_cellm_eicic_load_config_info_t
{
    rrm_bitmask_t   bitmask;            
    U8              eligible_ue;        
    U8              abs_pattern[RRM_MAX_PATTERN_BYTE];
}rrm_cellm_eicic_load_config_info_t;
/** eICIC_PHASE_1_2_CHANGES_END */

/*! \ rrm_cell_load_def_t
 *  \brief  Struct contains RRM action
 *  to be taken under various load condition
 *  \param bitmask
 *  \param percent
 *  \param action
 *  \param q_watermark
 *  \param num_of_usr
 *  \param mard_info
 *  \param eicic_load_config
 */

/* Bug_944_start */
#define RRM_CELLM_MARD_INFO_PRESENT         0x01
#define RRM_CELLM_EICIC_LOAD_CONFIG_PRESENT 0x02
/* Bug_944_end */

typedef struct _rrm_cell_action_def_t
{
    rrm_bitmask_t                           bitmask;
    U8                                      percent;
    U16                                     action;
    rrm_oam_watermark_t                     q_watermark;
    U8                                      num_of_usr;
    rrm_cellm_load_ac_barring_t             ld_ac_bar;
    /* Bug_944_start */
    rrm_cellm_redirect_info_t               mard_info;     /*^ O, RRM_CELLM_MARD_INFO_PRESENT, N, 0, 0, ^*/
    /* Bug_944_end */
    /* Bug_8718_start */
    U8                                      soft_lmt; /* Indicate the soft limit to be given over stop admission */
    /* Bug_8718_end */
    /** eICIC_PHASE_1_2_CHANGES_START */
    rrm_cellm_eicic_load_config_info_t      eicic_load_config;
    /** eICIC_PHASE_1_2_CHANGES_END */
}rrm_cell_load_def_t;

/*! \ rrm_cell_load_action_t
 *  \brief  Struct contains composite load action information that needs
 *  to be taken under various load condition
 *  \param bitmask
 *  \param over_load Contain information about the overload
 *  \param high_load Contain information about the high load
 *  \param mid_load Contain information about the mid load
 *  \param actn_status
 *  \param curr_load_lvl
 *  \param curr_load_val
 */

typedef struct _rrm_cell_comp_load_action_t
{
#define RRM_CELLM_OVERLOAD_PRESENT 0x01
#define RRM_CELLM_HIGHLOAD_PRESENT 0x02
#define RRM_CELLM_MIDLOAD_PRESENT 0x04
    rrm_bitmask_t                 bitmask;
    rrm_cell_load_def_t           over_load;
    rrm_cell_load_def_t           high_load;
    rrm_cell_load_def_t           mid_load;
    /*+ Coverity CID : 28805, 28804, 28803, 28802, 28801, 28800 +*/
    U16                           actn_status;  
    /*- Coverity CID : 28805, 28804, 28803, 28802, 28801, 28800 -*/
    rrm_load_type                 curr_load_lvl;  
    U8                            curr_load_val; /* Current load value in % */
}rrm_cell_load_action_t;

/*! \ rrm_cell_resrc_spec_info_t
 *  \brief This struct contains cell resource spec info
 *  \param resrc_type resource type
 *  \param ld_actn load action
 */
typedef struct _rrm_cell_resrc_spec_info_t
{
    rrm_cellm_resrc_type_et resrc_type;
    rrm_cell_load_action_t  ld_actn;
}rrm_cell_resrc_spec_info_t;

/*! \ rrm_cell_resrc_spec_load_action_t
 *  \brief  Struct contains resource load action information that needs
 *  to be taken under various load condition
 *  \param  count
 *  \param  resrc_spec_info
 */

typedef struct _rrm_cell_resrc_spec_load_action_t
{
    U8      count;
    rrm_cell_resrc_spec_info_t  resrc_spec_info[RRM_CELLM_MAX_RESRC];
}rrm_cell_resrc_spec_load_action_t;

#define RRM_CELL_LOAD_COMPOSITE_PRESENT     0x01
#define RRM_CELL_LOAD_RESRC_SPEC_PRESENT    0x02
/*! \ rrm_cell_load_action_info_t
 *  \brief This Struct contains load action info
 *  \param choice  choice
 *  \param soft_lmt soft limit
 *  \param comp_load_info contains composite info about load condindition
 *  \param resrc_spec_info contains resource specific information about load condition
 */
typedef struct _rrm_cell_load_action_info_t
{
    U8      choice; /* 1 = comp_load_info, 2 = resrc_spec_info */
    U8      high_actn_applied_lmt; /* Indicate that action applied stop for Hi load when hiload % fall to this*/
    /* Load Processing timer */
    U16     ld_process_timer;
    QTIMER  ld_process_timer_id;
    union
    {
        rrm_cell_load_action_t              comp_load_info;
        rrm_cell_resrc_spec_load_action_t   resrc_spec_info;
    }u;
    /* SPR 20653 Fix Start */
    U8      tnl_load_support_flag;
    U8      hw_load_support_flag;
    /* SPR 20653 Fix End */
}rrm_cell_load_action_info_t;


/* +- SPR 18268 */
/* Bug_12050_Fix: Start */
/*! \ rrm_temp_max_thp_t
 *  \brief This Struct contains the temporary data for per API
 *  \and will helpful to calculate the total cell thp.
 *  \param max_dl_thp; Maximum downlink thp.
 *  \param max_ul_thp; Maximum uplink thp.
 *  \param ue_index; UEs index.
 */
typedef struct _rrm_temp_max_thp_t
{
    U64  max_dl_thp[ MAX_KPI_INDEX ];
    U64  max_ul_thp[ MAX_KPI_INDEX ];
    U16  ue_index[ MAX_KPI_INDEX ];
    U16  cell_index[ MAX_KPI_INDEX ];
}rrm_temp_max_thp_t;
/* Bug_12050_Fix: End */

/*! \ rrm_max_thp_t
 *  \brief This Struct contains the temporary data for per API
 *  \and will helpful to calculate the total cell thp.
 *  \param max_dl_thp; Maximum downlink thp.
 *  \param max_ul_thp; Maximum uplink thp.
 *  \param ue_index; UEs index. 
 */
typedef struct _rrm_max_thp_t
{
    U64  temp_cell_max_dl_thp;
    U64  temp_cell_max_ul_thp;
    U16  temp_num_of_active_UE;

    U16  num_of_active_UE;
    U16  ue_index[ MAX_UE_SUPPORTED ];
}rrm_max_thp_t;

/* SPR-10397-fix end */

/*DYNAMIC SIB SCHEDULING START*/

#define RRM_MAX_SI_SEGMENTS         11 

#define RRM_MAX_SFN_VALUE 1024
#define RRM_MAX_SF_VALUE 10240

#define RRM_SI_PERIODICITY_RF_8     8
#define RRM_SI_PERIODICITY_RF_16    16
#define RRM_SI_PERIODICITY_RF_32    32
#define RRM_SI_PERIODICITY_RF_64    64
#define RRM_SI_PERIODICITY_RF_128   128
#define RRM_SI_PERIODICITY_RF_256   256  
#define RRM_SI_PERIODICITY_RF_512   512

#define RRM_SI_WINDOW_LEN_1_MS      1
#define RRM_SI_WINDOW_LEN_2_MS      2
#define RRM_SI_WINDOW_LEN_5_MS      5
#define RRM_SI_WINDOW_LEN_10_MS     10
#define RRM_SI_WINDOW_LEN_15_MS     15
#define RRM_SI_WINDOW_LEN_20_MS     20
#define RRM_SI_WINDOW_LEN_40_MS     40

#define RRM_MAX_SUBFRAME 10
#define RRM_MAX_UL_DL_CONFIG_ENTERIES  7
typedef struct _rrm_si_segment_data_t
{
    U16  si_segment_periodicity;
    U8   is_periodicity_changed;
    U8   si_segment_id;
}rrm_si_segment_data_t;

typedef struct _sib_msg_info_t
{
    U8     segment_id;
    U8     occupied;
#ifdef TDD_MODE_FLAG
    U32   sub_frame_type;
#endif

}sib_msg_info_t;

typedef enum{
    UL_SUBFRAME = 0,
    DL_SUBFRAME,
    SPECIAL_SUBFRAME
}frame_type_et;

typedef struct _ul_dl_config_data_t{

    U8    switch_point_periodicity;
    U8    sub_frame_structure[RRM_MAX_SUBFRAME];
}ul_dl_config_data_t;


typedef struct _ul_dl_config_data_array_t{
    ul_dl_config_data_t    ul_dl_config_data[RRM_MAX_UL_DL_CONFIG_ENTERIES];
}ul_dl_config_data_array_t;

typedef enum
{
    RRM_SIB_TYPE_3 = 0,
    RRM_SIB_TYPE_4 ,
    RRM_SIB_TYPE_5 ,
    RRM_SIB_TYPE_6 ,
    RRM_SIB_TYPE_7 ,
    RRM_SIB_TYPE_8 ,
    RRM_SIB_TYPE_9,
    RRM_SIB_TYPE_UNDEF 
}rrm_sib_merge_id_et;

/* RACH_OPTIMIZATION_CHANGES_START */

typedef struct _rrmcm_rach_data_t
{
    U16                             num_of_l2_perf_accumulated_reports;
    U64                             total_rcvd_msg3_random_access_preambles_cbra;  
    U64                             total_rcvd_msg3_random_access_preambles_cfra; 
    U64                             total_failed_random_access_preambles_msg2_cbra; 
    U64                             total_failed_random_access_preambles_msg2_cfra;  
    U64                             total_allocated_random_access_preambles;    
    U64                             total_succ_random_access_ta[MAX_TA_RANGE];  
    U64                             total_unassigned_false_cfra;  
    U64                             total_unassigned_sum_cfra;  
    /* SPR-18438 START */
    U64                             num_random_access_preambles_group_a;
    U64                             num_random_access_preambles_group_b;
    /* SPR-18438 END */
}rrmcm_rach_data_t;

typedef struct _rrm_cellm_rach_info_t
{
    rrmcm_rach_data_t   cell_rach_data;
    U16                 rach_l2_periodic_timer;
}rrm_cellm_rach_info_t;
/* RACH_OPTIMIZATION_CHANGES_END */
typedef struct _rrm_cell_kpi_stat_t
{
    QTIMER kpi_periodic_timer;
    /* CSR-81563[SPR-11383]-fix start */
    U32     accumulative_duration;
    /* CSR-81563[SPR-11383]-fix end */
    /* BUG_857 Start*/
    /* cell unavailable time */
    struct timeval cell_stop_time;
    struct timeval cell_start_time;
    rrmcm_kpi_data_t kpi;
    U16     duration;
    /* BUG_857 end*/
}rrm_cell_kpi_stat_t;

/*DYNAMIC SIB SCHEDULING END*/
/*DYNAMIC ICIC START*/

#define THRESHOLD_PERCENT_FOR_UES_VD_MAX_P_A 50
#define MAX_REFERENCE_SIGNAL_POWER 50
#define MIN_REFERENCE_SIGNAL_POWER -60
#define MAX_VALUE_FOR_P_A 7 

typedef struct _rrm_cellm_cell_reconfig_params_for_icic_t
{
    S8 refrence_signal_power;    
}rrm_cellm_cell_reconfig_params_for_icic_t;

typedef enum
{
    RRM_CELLM_DYNAMIC_ICIC_CELL_EDGE_UES = 0,
    RRM_CELLM_DYNAMIC_ICIC_CELL_CENTER_UES,
    RRM_CELLM_DYNAMIC_ICIC_HANDOUT_UES,
    RRM_CELLM_DYNAMIC_ICIC_P_MAX_UES
}rrm_cellm_dynamic_icic_counters_et;

typedef enum
{
    RRM_CELLM_REF_SIG_PWR_DECREMENTED = 0,
    RRM_CELLM_REF_SIG_PWR_INCREMENTED,
    RRM_CELLM_REF_SIG_PWR_NOT_UPDATED
}rrm_cellm_ref_sig_pwr_update_type_et;
typedef struct _rrm_cellm_dynamic_icic_data_t
{
    U16  num_ues_in_cell_edge; 
    U16  num_ues_in_cell_center; 
    U16  num_ues_handout_in_olpc;
    U16  num_ues_vd_max_p_a_val;
    QTIMER cell_reconfig_time_for_icic;
    rrm_cellm_ref_sig_pwr_update_type_et cell_reconfigured_for_icic;
    /*Location update fix in UE RECONFIG*/
    rrm_bool_et mac_loc_update_triggrd_aftr_ue_reconfig;
    /*Location update fix in UE RECONFIG*/
    rrm_pdsch_config_params_t pdsch_config_params;
}rrm_cellm_dynamic_icic_data_t;
/*DYNAMIC ICIC END*/

/** Dynamic UE Scheduling Start **/

typedef struct _rrm_ue_scheduling_info_perfs_t
{
    rrm_bool_et        ue_scheduling;
    rrm_cell_perf_active_ue_dl_ul_stats_t    /* reports came from mac for active ue in dl,ul*/
        active_ue_ul_dl_stats; 
}rrm_ue_scheduling_info_perfs_t;

typedef struct _rrm_mac_config_perfs_stats_bitmap_t
{

    U32     uePerfStatsToReportBitMap; /* persistent bitmap for enbaling the ue perfs stats report */
    U32     cellPerfStatsToReportBitMap; /* persistent bitmap for enabling the cell perfs stats */
}rrm_mac_config_perfs_stats_bitmap_t;
/** Dynamic UE Scheduling End **/

typedef enum _rrm_antenna_mxn_et
{
    Anetnna_1x1,
    Anetnna_2x2,
    Anetnna_4x2,
    Anetnna_4x4
}rrm_antenna_mxn_et;

/* Fix for CSR: 00059124 */
typedef struct _rrm_anr_meas_config_ue_category_list_t
{
    rrm_ue_index_t ue_index_list[MAX_NUM_OF_UE];
    U8             count;
}rrm_anr_meas_config_ue_category_list_t;
/* Fix for CSR: 00059124 */
/*Bug:9237 start*/
/*! \  struct rrm_cell_procedure_queue_t
 *  \brief Internal procedure queue for messages coming from RRC
 *  \param U16 api_id
 *  \param void *p_proc_data
 */
typedef struct _rrm_cell_procedure_queue_t
{
    U32                         event_id;             /*!< Event ID for message from RRC    */
	/*TRANS ID FIX */
	U16                         api_id;
    U16                         trans_id;
/* SPR_17763_FIX_START */
    struct timeval              enqueue_time;
/* SPR_17763_FIX_END */
    void                        *p_proc_data;       /*!< Pointer to procedure data      */
}rrm_cell_procedure_queue_t;

/*! \  struct rrm_cell_procedure_record_t
 *  \brief A queue node for the incoming messages
 *  \param YLNODE s_node
 *  \param rrm_cell_procedure_queue_t proc_queue
 */
typedef struct  _rrm_cell_procedure_record_t
{
    YLNODE                      s_node;             /*!< CSPL node for queue            */
    rrm_cell_procedure_queue_t    proc_queue;         /*!< Object for procedure queue     */
}rrm_cell_procedure_record_t;

/*! \  procedure queue list
 *  \brief Procedure queue Head
 */
typedef YLIST rrm_cell_proc_queue_t;

 rrm_void_t
rrm_cell_init_queue
(
 rrm_cell_proc_queue_t *p_procedure_queue
 );


/*Bug:9237 end*/

/* Carrier Aggregation start*/

typedef struct _rrm_scell_node_t
{
	YLNODE               node;
        void                 *p_ue_context;

}rrm_scell_node_t;

typedef struct rrm_scell_info_t
{
    YLIST                           list;
    rrm_cell_index_t                scell_index;/* some times we are inside context and want to what cell is it has */

}rrm_scell_info_t;


/* Carrier Aggregation end */
/* SPR 10450 Fix Start */

typedef struct _drx_res_alloc_tree_per_ue_t
{
     rrm_res_status_et                       res_status;
     U16                                     drx_start_offset;
}drx_res_alloc_tree_per_ue_t;

#define RRMCM_SHORT_DRX_CONFIG_PRESENT        0x01
#ifdef ENDC_ENABLED
#define RRMCM_LONG_DRX_CYCLE_R15_PRESENT        0x02
#endif
typedef struct _rrm_cell_drx_res_per_profile_t
{
     rrm_bitmask_t           bitmask;   
     rrm_bool_et                             drx_enabled;
     U8                                      qci;
     rrm_oam_on_duration_timer_et            on_duration_timer;
     rrm_oam_drx_inactivity_timer_et         drx_inactivity_timer;
     rrm_oam_drx_retransmission_timer_et     drx_retransmission_timer;
     rrm_oam_long_drx_cycle_start_offset_et  long_drx_cycle; 
     rrm_oam_short_drx_cycle_config_t        short_drx_cycle_config; 
     U8                                      total_drx_offset;
     drx_res_alloc_tree_per_ue_t             drx_res_tree[255];
     #ifdef ENDC_ENABLED
     rrm_oam_long_drx_cycle_start_offset_r15_et  long_drx_cycle_r15; 
     #endif
}rrm_cell_drx_res_per_profile_t;


typedef struct _rrm_cell_drx_config_resources_t
{
    rrm_cell_drx_res_per_profile_t       drx_res_arr[RRM_OAM_MAX_NUM_QCI_CLASSES];
}rrm_cell_drx_config_resources_t;
/* SPR 16012 Fix Start */
typedef enum 
{
    NO_BLOCK_UNBLOCK, 	/* Neither Cell block nor Cell Unblock is ongoing */ 
    CELL_BLOCK_ONGOING,	
    CELL_UNBLOCK_ONGOING
}rrm_cell_block_unblock_ongoing_proc_et;
/* SPR 16012 Fix Stop */

/* SPR 10450 Fix End */
/* SPR 16406 Start */
/* SPR 16406 18sep Start */
#define RRM_HO_PENDING_NODE_CGI_PRESENT 0x01 
typedef struct _ho_pending_cgi_node_data_t
{
    U8 bitmask;
    U16               ue_idx; /* ue_index for which CGI triggered with this pci+earfcn */
    U16               pci;    /* pci for which CGI is pending for HO */
    U16               earfcn; /* earfcn for the pci at which CGI is pending for HO */
    rrm_oam_eutran_global_cell_id_t cgi;   /* CGI info for confused cells otherwise invalid */                             
/* SPR 16406 18sep End */
}ho_pending_cgi_node_data_t;

typedef struct _ho_pending_cgi_node_t
{
    YLNODE                    node;
    ho_pending_cgi_node_data_t   ho_pending_cgi_node_data;
}ho_pending_cgi_node_t;

typedef struct _rrm_ho_pending_cgi_list_t
{
    YLIST                    ho_pending_cgi_list;
}rrm_ho_pending_cgi_list_t;

typedef struct _rrm_ho_cgi_pending_list_t
{
   rrm_ho_pending_cgi_list_t rrm_ho_pending_cgi_list[RRM_MAX_NUM_CELLS];
}rrm_ho_cgi_pending_list_t;
/* SPR 16406 End */
/* SPR 12387 Fix Start */
typedef struct _rrm_sr_data_t
{
  U16  allocated_sr_perodicity_max_val;
  U8   sr_periodicity; 
}
rrm_sr_data_t;
/* SPR 12387 Fix End */

/* eICIC_PHASE_1_2_CHANGES_START */
typedef enum
{
  LOAD_INFO_EICIC_INVOKE,
  LOAD_INFO_EICIC_ABS_PATTERN,
  LOAD_INFO_EICIC_ABS_INACTIVE,
  LOAD_INFO_EICIC_INACTIVE
}rrm_eicic_load_info_type_et;

/* bug_13898_start */ 
typedef enum
{
  EICIC_LOAD_INFO_SEND_NOT_REQUIRED,
  EICIC_LOAD_INFO_SEND_SUCCESS,
  EICIC_LOAD_INFO_SEND_FAILURE
}rrm_eicic_load_info_send_status_et;
/* bug_13898_end */ 

typedef struct _mac_config_data_t
{
  rrm_oam_eicic_node_type_et          eicic_provisioned_type; 
  U8 serving_abs_pattern[RRM_MAX_PATTERN_BYTE];
  U8 usable_abs_pattern[RRM_MAX_PATTERN_BYTE];
  U32 mac_eicic_meas_report_periodicity;
}mac_config_data_t;
/** eICIC_PHASE_1_2_CHANGES_END */ 

#define RRM_CELLM_USABLE_ABS_PATTERN_PRESENT                      0x00001
#define RRM_CELLM_SERVING_ABS_PATTERN_PRESENT                     0x00002
    /* SPR 14465 start */
    /* code deleted */
    /* SPR 14465 end */
/** eICIC_PHASE_1_2_CHANGES_START */
/* MAC meas periodicity change */
#define   RRM_CTXT_REQUIRED_PER_RECONFIG_TO_MAC_PRESENT           0x00008
/* ABS PATTERN CHANGED */
#define   RRM_CTXT_ABS_PATTERN_FLAG_PRESENT                       0x00010
/* USABLE pattern CHANGE */
#define   RRM_CTXT_USABLE_ABS_RECONFIG_INFO_PRESENT               0x00020
/* SERVING PATTERN CHANGE */
#define   RRM_CTXT_SRV_ABS_RECONFIG_INFO_PRESENT                  0x00040
/* EICIC PROV MODE CHANGE */
#define   RRM_CTXT_EICIC_PROV_MOD_INFO_PRESENT                    0x00080


/* RECONFIG TRIGGER DECISION */
/* During reconfig, Send Reconfig to MAC */
#define   RRM_CTXT_SENDING_EICIC_RECONFIG_TO_MAC_PRESENT          0x00100
/* During reconfig, Send Reconfig to UEM */
#define   RRM_CTXT_SENDING_EICIC_RECONFIG_TO_UEM_PRESENT          0x00200
#define   RRM_CTXT_SENDING_EICIC_LOAD_ABS_RECONFIG_TO_MAC_PRESENT 0x00400
#define   RRM_CTXT_COMP_OVERLOAD_ABS_RECONFIG_PRESENT             0x00800
#define   RRM_CTXT_COMP_MIDLOAD_ABS_RECONFIG_PRESENT              0x01000
#define   RRM_CTXT_COMP_HIGHLOAD_ABS_RECONFIG_PRESENT             0x02000
#define   RRM_CTXT_RSRC_SP_OVERLOAD_ABS_RECONFIG_PRESENT          0x04000
#define   RRM_CTXT_RSRC_SP_MIDLOAD_ABS_RECONFIG_PRESENT           0x08000
#define   RRM_CTXT_RSRC_SP_HIGHLOAD_ABS_RECONFIG_PRESENT          0x10000
#define   RRM_CTXT_ABS_RECONFIG_LOW_LOAD_ABS_PRESENT              0x20000
#define   RRM_CTXT_ABS_RECONFIG_VICTIM_ABS_PRESENT                0x40000
#define   RRM_CTXT_ABS_RECONFIG_INVALID_PARAM                     0x80000
/*eicic_subset_pattern_fix_start*/
#define   RRM_CTXT_RECONFIG_VICTIM_MEAS_PATTERN_PRESENT           0x100000
/* SPR 14215 start*/
#define   RRM_CTXT_RECONFIG_X2_LINK_UP_RECONFIG                   0x200000
#define   RRM_CTXT_RECONFIG_X2_LINK_DOWN_RECONFIG                 0x400000
/*eICIC_Changes_Start*/
/*Bug 831 Changes Start*/
#define   RRM_CTXT_RECONFIG_FIRST_INTRA_CELL			  0x800000
/*Bug 831 Changes End*/
/*eICIC_Changes_End*/
/* SPR 14215 en*/
/*eicic_subset_pattern_fix_end*/

/** eICIC_PHASE_1_2_CHANGES_END */
typedef struct _rrm_eicic_cell_info_t
{
	rrm_bitmask_t                   bitmask;
	/** eICIC_PHASE_1_2_CHANGES_START */
	rrm_bitmask_t                   reconfig_param_bitmask;
	rrm_bitmask_t                   reconfig_api_trigger_bitmask;
	U8                      temp_load_config_abs_pattern[RRM_MAX_PATTERN_BYTE];
	mac_config_data_t       mac_config_data;    
	/** eICIC_PHASE_1_2_CHANGES_END */
	U8     serving_abs_pattern_applied[RRM_MAX_PATTERN_BYTE];
	U8     usable_abs_pattern_applied[RRM_MAX_PATTERN_BYTE];
	U8     serving_meas_abs_pattern_applied[RRM_MAX_PATTERN_BYTE];
	U8     temp_srv_abs_pattern[RRM_MAX_PATTERN_BYTE];
	U8     temp_meas_abs_pattern[RRM_MAX_PATTERN_BYTE];
	U8     temp_usable_abs_pattern[RRM_MAX_PATTERN_BYTE];
	U8     num_x2ap_abs_ind_recived ;
	U8     num_x2ap_invoke_ind_sent;
	rrm_bool_et  rsu_is_required;  
    /* SPR 14465 start */
    /* code deleted */
    /* SPR 14465 end */
	QTIMER cell_aggressor_sel_timer;
	QTIMER cell_guard_timer;
	QTIMER cell_collation_timer;
}rrm_eicic_cell_info_t;

rrm_return_et rrm_cellm_l2_eicic_meas_report 
(
 void *p_api,
 /*SPR 17777 +-*/
 U8  cell_index
 );
/* eICIC_PHASE_1_2_CHANGES_END */
#ifdef LTE_EMBMS_SUPPORTED
#define RRMCM_EMBMS_RESERVED_SF_CHANGED     0x01
typedef struct _rrm_sf_invalid_for_cqi_t
{
    rrm_bitmask_t       bitmask;
    U8                  changed_reserved_sf_list;
}rrm_sf_invalid_for_cqi_t;
#endif

/* SPR 20652 Fix Start */
typedef YLIST rrm_ue_info_t;  /* CSPL List of rrm_ue_shelf_node_t */

/*! \  _rrm_ue_shelf_node_t
 *  \brief  Parameters required to store the UE context in rrm_ue_info_t 
 *  \param node CSPL node
 *  \param *p_ue_context void pointer for UE context
 */
typedef struct _rrm_ue_shelf_node_t
{
    YLNODE              node;
    rrm_void_t         *p_ue_context;
}rrm_ue_shelf_node_t;

typedef struct _rrm_ues_shelf_t
{
    rrm_ue_info_t   rrm_ue_info_list; /* List of rrm_ue_shelf_node_t */
    rrm_ue_index_t  next_idx;
    rrm_ue_index_t  prev_idx;
}rrm_ues_shelf_t;

/*! \  _rrm_ue_db_t
 *  \brief  Parameters required to store the UE Info list per uecc instance 
 *  \param *p_ue_idx_list
 *  \param head_idx
 *  \param tail_idx
 */
typedef struct _rrm_ue_db_t
{
    rrm_ues_shelf_t   *p_ue_idx_list;
    rrm_ue_index_t    head_idx;
    rrm_ue_index_t    tail_idx;
}rrm_ue_db_t;

#define RRM_NEXT_NODE_FOR_SAME_UE_INDEX_PRESENT     0x01
#define RRM_NEXT_UE_INDEX_PRESENT                   0x02
#define RRM_INVALID_UE_NODE_PRESENT                 0x03
typedef struct _rrm_seq_data_info_t
{
    U8 choice;  /* Choice = 1, means p_nxt_node present and Choice = 2 means nxt_idx present */
    union
    {
        rrm_ue_shelf_node_t *p_nxt_node;
        rrm_ue_index_t      nxt_idx;
    }u;
}rrm_seq_data_info_t;
/* SPR 20652 Fix End */

/* SPR 21367 Fix End */
typedef struct _rrm_ue_erab_bitrate_t
{
    U64  ul_bitrate;
    U64  dl_bitrate;
}rrm_ue_erab_bitrate_t;

typedef struct _rrm_ue_admitted_bitrate_t
{
    rrm_ue_erab_bitrate_t  ue_gbr_bitrate;
    rrm_ue_erab_bitrate_t  ue_ngbr_bitrate;
}rrm_ue_admitted_bitrate_t;
/* SPR 21367 Fix End */

#ifdef ENDC_ENABLED
typedef struct _rrm_qci_pos_in_ue_t
{
  U8 qci;
  U8 pos;
}rrm_qci_pos_in_ue_t;

typedef struct _rrm_qci_pos_t
{
  rrm_ue_index_t ue_index;
  rrm_qci_pos_in_ue_t rrm_qci_pos_in_ue[9];
}rrm_qci_pos_t;

typedef struct _rrm_qci_data_in_stats_t
{
  U16  ue_count;
  rrm_qci_pos_t  rrm_qci_pos[MAX_NUM_UE];
}rrm_qci_data_in_stats_t;

typedef struct _rrm_temp_qci_thp_t
{
    U64  pdcpSduBitUl[ MAX_KPI_INDEX ];
    U64  pdcpSduBitDl[ MAX_KPI_INDEX ];
    U8   qci[ MAX_KPI_INDEX ];
    U16  ue_index[ MAX_KPI_INDEX ];
    U16  cell_index[ MAX_KPI_INDEX ];
}rrm_temp_qci_thp_t;
typedef struct _rrm_endc_into_t
{
	U8        sgnb_ambr_share;
	U8        min_num_ue_thp_report;
	U64       split_bearer_applicable_bitrate_dl;
}rrm_endc_info_t;
#endif

#define RRM_OAM_INSTANCE_RESET_FLAG_PRESENT			0x01

/*! \ rrm_cell_context_t
 *  \brief A struct that holds all the required information of a cell 
 *  \param bitmask Bitmask for Cell context Access management parameters is present or not.
 *  \param cell_index  Cell index for the cell.
 *  \param cell_state  Cell state of the cell.
 *  SPR 21322 START
 *  \param transition_cell_state When another procedure is invoked when one is
 *  already ongoing, this will be used to store the state in which FSM should
 *  return after completing the new procedure.
 *  SPR 21322 END
 *  \param global_cell_id Global cell id for the cell.
 *  \param ongoing_procedure_fail_cause Fail cause for the ongoing procedure.
 *  \param immediate_cell_start Paramter to check immediate cell start is needed or not.
 *  \param cell_resource_info structure for resource information of cell.
 *  \param ran_info   Structure for ran_info of a cell.
 *  \param epc_info   Structure for epc_info of a cell.
 *  \param operator_info Structure for operator info of a cell.
 *  \param access_mgmt_params Structure for access management information of a cell.
 *  \param ongoing_trans_id parameter for ongoing transaction id of API.
 *  \param stop_adm_flag Flag for UE admission is stopped or not.
 *  \param p_incoming_api_info Pointer to incoming API.
 *  \param statistics  Structure for cell statistics.
 *  \param stop_reconfig_fail parameter for reconfig procedure and cell is stop.
 *  \param p_tcb Pointer for TCB of cell context.
 *  \param p_son_info  Pointer for Son information of cell context.
 *  \param p_new_reconfig_data Pointer to store the reconfig structure.
 *  \param p_ncl_built_from_updated_nrt_info Pointer to store NCl build and updated NRT information.
 *  \param event_id Ongoing Event id .
 *  \param enable_rac_flag RAC falg is enabled and not.
 *  \param deletion_flag Flag for cell deleteion in case of reconfig.
 *  \param cell_already_register_es Flag to check cell is registered for ES or not.
 *  \param cell_already_register_anr Flag to check cell is registered for ANR or not.
 *  \param api_id Ongoing API id.
 *  \param cell_reconfig_deletion_required Cell need to be deleted for reconfig procedure.
 *  \param is_rf_param_changed_in_reconf Flag to check Rf param is changed in reconfig procedure.
 *  \param cell_rcc_states Congestion states of a cell.
 *  \param rcc_param structure for parameter needed to check congestion.
 *  \param rrm_ue_tpc_rnti_tbl Structure to store RNTI table.
 *  \param ue_meas_cnt   Ue measurement count.
 *  \param meas_eutran_list  Measurement EUTRAN list.
 *  \param meas_utran_list   Measurement UTRAN FDD list.
 *  \param meas_utran_tdd_list Measurement UTRAN TDD list.
 *  \param meas_geran_list   Measurement for GERAN list.
 *  \param allocated_sr_perodicity_max_val parameter for allocated SR periodicity maximum value.
 *  \param allocated_cqi_perodicity_max_val Parameter for allocated CQI periodicity maximum value.
 *  \param cqi_ri_res   two dimensional array for CQI RI list.
 *  \param sr_res       two dimensional array for SR list.
 *  \param ho_parameters  structure to store Handover paramteters of cell.
 *  \param x2_neighbours structure to store X2 cell information.
 */
typedef struct _rrm_cell_context_t
{
    rrm_bitmask_t                   bitmask;
    U32                             timer_val[CELL_MAX_TIMER_TYPE];
    rrm_cell_index_t                cell_index;
    U16                             ongoing_trans_id;
    rrm_cell_states_et        	    cell_state;
    /*SPR 21322 START*/
    rrm_cell_states_et              transition_cell_state;
    /*SPR 21322 END*/
    rrm_global_cell_id_t            global_cell_id;
    rrm_bool_t			            immediate_cell_start;
    rrm_cell_info_for_ue_mgr_t      cell_resource_info;
    ran_t                           ran_info;/* defined in rrm_cell_rrmim_intf.h */
    epc_t                           epc_info;/* defined in rrm_cell_rrmim_intf.h */
    operator_info_t                 operator_info;
    access_mgmt_params_t            access_mgmt_params;        
    /* flag for RRMCM_RMIF_STOP_ADMISSION_REQ*/
    void                            *p_incoming_api_info;
    U8                              ongoing_procedure_fail_cause;
    U8                              stop_reconfig_fail;
    U16                             event_id;
    U16                             api_id;
    cellm_tcb                       *p_tcb;
   /* SPR 11681 Fix Start */
   /* Code deleted */
   /* SPR 11681 Fix End */
    cell_son_info_t                 *p_son_info;                     
    rrmcm_rmif_cell_reconfig_req_t  *p_new_reconfig_data;
    ncl_params_t                    *p_ncl_built_from_updated_nrt_info;
    /* MEAS CONFIG REL1.2: START*/
    rrmcm_meas_config_sib_info_for_reconfig_t
        *p_meas_config_sib_info_for_reconfig;
    /* MEAS CONFIG REL1.2: END*/                                    
    rrm_bool_t			            stop_adm_flag;
    rrm_bool_t                      deletion_flag;
    rrm_bool_t                      enable_rac_flag;
    rrm_bool_t                     cell_reconfig_deletion_required;
    rrm_bool_t                     is_rf_param_changed_in_reconf;
    rrm_bool_t                     administrative_procedure_ongoing;
    rrm_bool_t                     cell_stop_ongoing_del_ue_ctxt;
    rrm_bool_t                       apply_anr_to_all_ue;
	rrm_bool_t                       apply_anr_on_ue_attach_in_all_cycles;
	rrm_bool_t                       is_anr_first_cycle;
	U8                              ue_meas_cnt;
    U8                              cell_already_register_es;
    U8                              cell_already_register_anr;
    U8                              registered_mod_by_son;
    /* RCC CHANGES */
    rcc_param_t                     rcc_param;
    /**CLPC_MR_START*/
    rrm_ue_tpc_rnti_tbl_pusch_t     rrm_ue_tpc_rnti_tbl_pusch;
    rrm_ue_tpc_rnti_tbl_pucch_t     rrm_ue_tpc_rnti_tbl_pucch; 
    /**CLPC_MR_END*/

    /* UE MEAS CHANGES : STARTS */
    meas_eutran_context_list_t      meas_eutran_list;
    meas_utran_context_list_t       meas_utran_list;
    /* TDD Support SON Start */
    meas_utran_tdd_context_list_t   meas_utran_tdd_list;
    /* TDD Support SON End */
    meas_geran_context_list_t       meas_geran_list;
    meas_cdma_context_list_t       meas_cdma_list;
    /* UE MEAS CHANGES : ENDS */
    /* SPR 12387 Fix Start */
     rrm_sr_data_t                 sr_data;
    /* SPR 12387 Fix End */
    /* List of SR resources index */
#ifdef LTE_EMBMS_SUPPORTED
/* SPR 22248 MBMS Changes Start */
    U16                             allocated_n2_pucch_res_idx_val;
/* SPR 22248 MBMS Changes Stop */
    rrm_sf_invalid_for_cqi_t       sf_invalid_for_cqi;
#endif
    /*MEMORY_PROFILING*/
    rrm_cqi_ri_res_list_t           **cqi_ri_res;
    /* List of CQI-RI resources index */
    rrm_sr_res_list_t               **sr_res;
    /*MEMORY_PROFILING*/
    /* SRS Start */
    /* List of SRS resource index */
    U16                             allocated_srs_max_anchor_val;
    U16                             allocated_cqi_perodicity_max_val;
    /* TDD_SRS_FREQ_HOPPING_START */
    rrm_srs_table_t                 srs_table;
    /* TDD_SRS_FREQ_HOPPING_END */
    /* SRS End */
    /* MC,MR & HO Start */
    rrm_cell_ho_params_t            ho_parameters;
    rrm_x2_cell_info_t              x2_neighbours;
    /* MC,MR & HO End */
    /* Soft Lock Changes Start */
    rrm_block_cell_params_ho_t      block_cell_params_ho;
    /* Soft Lock Changes End */
    /*BUG:102 */
    /*BUG:102 */
    /* ETWS/CMAS start */ 
    YLIST                           scheduled_etws_warning_list;
    YLIST                           scheduled_cmas_warning_list;
    scheduled_pws_data_t            *to_be_scheduled_pws_warning;
    rrm_cellm_pws_substate_et       pws_substate;
    rrm_scheduling_info_list_t      *scheduling_info_list_for_pws;
    U8                              filter_tx_table[RRM_TX_MODE_TABLE_SIZE]; 
    U8                              dl_sinr_quality_threshold;
    /* uplink mu mimo change start  */
    U8                              uplink_sinr_threshold;
    U8                              uplink_prb_budget_threshold;
    U8                              uplink_prb_budget_down_threshold;
    U8                              stop_update_fail;
    /* uplink mu mimo change end*/
    /* Uplink power control starts */
    rrm_olpc_params_t                     rrm_olpc_config_params;
    /* Uplink power control end */
    /* ETWS/CMAS End*/ 
    cell_ongoing_procedure_et       ongoing_proc_triggered;
    rrm_void_t                      *p_new_update_data;
    /* MLB Changes start */
    rrm_rsu_meas_id_list_t          rsu_meas_id_list;
    rrm_rs_srv_cell_ld_info_t       svr_cell_load_info;
    rrm_cell_load_action_info_t     cell_load_action;
    QTIMER                          self_load_rpt_tmr;
    /* MLB Changes end */
    rrm_cell_ecn_config_t            *p_cell_ecn_config;
    rrm_bool_t                     cell_stop_not_send_active_ue_count;
    /* SPS related changes start */
    rrm_bool_t                     sps_on;                          /* Checks whether SPS is enabled at Cell */
    U8                              sps_ues_count;                   /* Number of SPS UEs in the cell currently */
    U8                              sps_long_drx_cycle_start_offset; /* Start offset for DRX long cycle for SPS UEs*/
    /* SPR_8597_FIX_START*/
    //    U16                             sr_n1_pucch_an_last_indx;        /* last n1_pucch resource index for SR */ 
    /* SPR_8597_FIX_END*/
    U16                             sps_n1_pucch_an_start_indx;      /* start n1_pucch resource index for SPS */
    U8                             *sps_n1_pucch_an_marked_st;       /* Mark the sps_n1_pucch resource index on the corresponding bit */ 
    /* SPS related changes end */
    /*ECN load start*/
    rrm_max_thp_t                     max_thp;
    /*ECN load end*/
    /* MLB Ph2 SSAC Start */
    /* Contains the value for SIB2 SSAC param configured from oam
       Whenever ac-barring action hit cellm allocate memory to store SSAC param
       in this variable and trigger reconfig req to rrc. This memory will be freed when
       ever reconfig resp received form rrc.
     */
    rrm_cellm_load_ac_barring_t    *p_cell_ld_reconfig_data;
    /* MLB Ph2 SSAC End */

    /*DYNAMIC SIB SCHEDULING START*/
    rrm_si_segment_data_t           si_segment_data_sib_scheduling[RRM_MAX_SI_SEGMENTS];
    /*DYNAMIC SIB SCHEDULING END*/
    /* CSR 00051178 CHANGES START */
    U16  sib_present_at_cell_config_time_bitmask;
    U16  sib_present_at_cell_reconfig_time_bitmask;   
    /* CSR 00051178 CHANGES END */

    /*RIM changes start*/
    U32                             cur_rsn;/*Current seq no maitained
                                              at RRM for RIM Info request*/
    /* DYNAMIC ICIC CHANGES START */                                             
    /* SPR-12550-fix start */
    /* code deleted */
    /* SPR-12550-fix end */ 
    QTIMER                            x2ap_icic_info_rpt_tmr;
    rrm_l2_icic_l1_l2_report_info_t   icic_l1_l2_report_info;
    rrm_cellm_dynamic_icic_data_t   dynamic_icic_data;     
    /* DYNAMIC ICIC CHANGES END */
    /*uplink mu mimo chnage start*/
    /*SPR 7892 FIX START*/
    rrm_mac_ue_reconfig_mutex_val_et    ue_mac_reconfig_mutex;
    /*SPR 7892 FIX END*/
    /* uplink mu mimo change end */
    rrm_cell_kpi_stat_t               kpi_stat;
    /*Rel10 CR 393 changes start*/
    /* SPR 20653 Fix Start */
    /* Code Removed */
    /* SPR 20653 Fix End */
    /*Rel10 CR 393 changes end*/
    /* RACH_OPTIMIZATION_CHANGES_START */
    rrm_cellm_rach_info_t            cell_rach_info;
    /* RACH_OPTIMIZATION_CHANGES_END */
    /** Dynamic UE Scheduling Start **/
    rrm_mac_config_perfs_stats_bitmap_t  mac_perfs_stats_bitmap; 
    rrm_ue_scheduling_info_perfs_t       ue_scheduling_perfs;
    rrm_antenna_mxn_et                   mxn_antenna_support;
    /* SPR 5676 Fix Start*/
    U8                              system_info_tag_counter;
    /* SPR 5676 Fix End*/    
    U8                                dynamic_pdcch;
    U8                               mac_ecid_meas_start_sfn; 
    /* Start: Enh 317 */
    U8 updated_nrt_bitmask;
    /* End: Enh 317 */
    /* ative ue reports comes from mac. this report is configrable from 
       rrm.cfg. Timer values is considered in seconds as per mac api doc.
       This Timer only is configured for active ue reports in mac perf stats req api. 
     */
    U8                                   active_ue_reports_timer;
    /** Dynamic UE Scheduling End **/
    /* BUG_11317_FIX_START */
    U8               pusch_bler_threshold;
    /*Start:SPR 8715*/
    U8 num_tpc_id_dci_3;
    U8 num_tpc_id_dci_3a;
    /*End:SPR 8715*/


    /* Fix for CSR: 00059124 */
    rrm_anr_meas_config_ue_category_list_t 
        anr_meas_config_ue_category_list[RRM_MAX_ANR_MEAS_CONFIG_UE_CATEGORY];
    /* Fix for CSR: 00059124 */

    /*Bug:9237 start*/
    rrm_cell_proc_queue_t procedure_queue;
    /*Bug:9237 end*/
    U8   ue_schedule_to_dl;
    U8   ue_schedule_to_ul;
    /* Fix for CSR: 00059124 */
    U8 rrm_ue_throughput_dl_anr_threshold;
    U8 rrm_ue_throughput_ul_anr_threshold;
    /* Fix for CSR: 00059124 */
    /* BUG_11317_FIX_END */

    /* Carrier Aggregation start*/
    /* CID : 62114 */
    rrm_scell_info_t          scell_info[RRM_MAX_NUM_CELLS];
    rrm_cell_change_object    cell_modify_notification_obj;
    QTIMER                    intra_eNB_cell_load_broadcast_timer; 
    /* Carrier Aggregation end*/
    /* SPR 10450 Fix Start */
    rrm_cell_drx_config_resources_t   rrm_cell_drx_config_resources ;
    /* SPR 10450 Fix End */
    /* eICIC_PHASE_1_2_CHANGES_START */
    rrm_eicic_cell_info_t             rrm_eicic_cell_info;
    /* eICIC_PHASE_1_2_CHANGES_END */
#ifdef LTE_EMBMS_SUPPORTED
    rrm_cell_embms_data_t             rrm_cell_embms_data;             
#endif
/* SPR 16406 Start */
    rrm_ho_cgi_pending_list_t            rrm_cgi_pending_list;
/* SPR 16406 End */
/* SPR 16012 Fix Start */
    /* Used in order to discriminate the whether cell block/unblock is ongoing
     * while composing rrc_rrm_cell_reconfig_req during both these procedures */
    rrm_cell_block_unblock_ongoing_proc_et cell_block_unblock_ongoing; 
    /* SPR 16012 Fix Stop */

    /*SPR 20908 Start*/
	rrm_oam_lower_layer_comm_info_t   l2_comm_info_params; 
	/*SPR 20908 Start*/

	/* SPR 20652 Fix Start */
	rrm_ue_db_t rrm_ue_db;
	rrm_ue_admitted_bitrate_t    rrm_ue_admitted_bitrate;
	/* SPR 20652 Fix End */
	/*SPR 20908 Start*/

    /* SPR 19309 + */
    /* Start fix for SPR 14987*/
    U8                                num_anr_strong_cell_req_sent;
    /* End fix for SPR 14987*/
    U8                   cfi_value;         
    /* SPR 19309 - */
    /* SPR 19310 + */
    U16                   num_of_eul;        
    U16                   size_of_eul;       
    /* SPR 19310 - */
    /*SPR21886 Start*/
    /* SPR 12971 Start */
    U8                                starting_rat_type_for_anr;
    /*SPR21886 End*/
    /* SPR 12971 End */
}rrm_cell_context_t;
/*Bug:9237 start*/
rrm_return_et
rrm_cell_mgr_enqueue_req
(
  rrm_cell_procedure_queue_t * p_cell_proc_queue,
/* SPR_17763_FIX_START */
  rrm_cell_context_t *p_cell_ctxt,
  rrm_bool_et         push_at_tail
/* SPR_17763_FIX_END */
);

rrm_void_t
rrmcm_dequeue_message(
        rrm_cell_context_t *p_cell_ctxt
        );

rrm_void_t
rrm_clean_cell_queue
(
 rrm_cell_context_t *p_cell_ctxt
 );

/*Bug:9237 end*/

/*X2AP START */
/*! \typedef enum rrm_x2ap_num_antenna_port_et 
 *  \brief  enum for antenna port
 */

typedef enum  
{
    RRM_AN1 = 1,
    RRM_AN2 = 2,
    RRM_AN4=  4
}rrm_x2ap_num_antenna_port_et;


/*! \def X2AP_OAM_MACRO_ENB_ID_PRESENT 
 *  \brief oam macro for enb present
 */
#define X2AP_OAM_MACRO_ENB_ID_PRESENT        0x01

/*! \def X2AP_OAM_HOME_ENB_ID_PRESENT 
 *  \brief enb id present
 */
#define X2AP_OAM_HOME_ENB_ID_PRESENT         0x02

/*! \ cqi_ri_map_info_per_ue_t 
 *  \brief Structre for mapping of CQI and RI information per UE 
 *  \param cqi_index cqi index
 *  \param ri_index ri index
 *  \param sf_ri_scheduling sf ri scheduling
 *  \param allocated allocated
 */
typedef struct cqi_ri_map_info_per_ue_t
{
    U16              cqi_index;
    U16              ri_index;
    U16              sf_ri_scheduling;
    rrm_bool_et      allocated;
}cqi_ri_map_info_per_ue;

/*! \ rrm_x2ap_tac_t
 *  \brief A struct that holds x2ap tracking area code 
 *  \param tac[MAX_TAC_BYTES] array of tracking area code
 */
typedef struct _rrm_x2ap_tac_t
{
    U8 tac[MAX_TAC_BYTES];
}rrm_x2ap_tac_t;

/*! \ rrm_x2ap_broadcast_plmn_t
 *  \brief A struct that holds x2ap broadcast plmn identity
 *  \param num_bcast_plmn_id
 *  \param plmn_identity[MAX_NUM_BPLMN]
 */
typedef struct _rrm_x2ap_broadcast_plmn_t
{
    U8                     num_bcast_plmn_id;    
    rrm_x2ap_plmn_identity_t  plmn_identity[MAX_NUM_BPLMN]; 
}rrm_x2ap_broadcast_plmn_t;

/*! \ rrm_x2ap_ecgi_t
 *  \brief A struct that holds x2ap ecgi
 *  \param plmn_identity plmn identity
 *  \param eutran_cell_id[HOME_ENB_ID_OCTET_SIZE] eutran_cell_id
 */
typedef struct _rrm_x2ap_ecgi_t
{
    rrm_x2ap_plmn_identity_t  plmn_identity; 
    U8                     eutran_cell_id[HOME_ENB_ID_OCTET_SIZE]; 
}rrm_x2ap_ecgi_t;

/*! \ rrm_x2ap_special_subframe_info_t
 *  \brief A struct that holds x2ap special subframe info
 *  \param special_subframe_patterns special subframe patterns
 *  \param dl_cp dl cp
 *  \param  ul_cp ul_cp
 */
typedef struct _rrm_x2ap_special_subframe_info_t
{
    U8 special_subframe_patterns; 
    U8 dl_cp; 
    U8 ul_cp;
}rrm_x2ap_special_subframe_info_t;

/*! \ rrm_x2ap_earfcn_t
 *  \brief A struct that holds x2ap earfcn value
 *  \param earfcn earfcn 
 */
typedef struct _rrm_x2ap_earfcn_t
{
    U32 earfcn; 
}rrm_x2ap_earfcn_t;

/*! \ rrm_x2ap_fdd_info_t
 *  \brief A struct that holds x2ap fdd information
 *  \param ul_earfcn
 *  \param dl_earfcn
 *  \param ul_trnsmission_bdwidth
 *  \param dl_trnsmission_bdwidth
 */
typedef struct _rrm_x2ap_fdd_info_t
{
    rrm_x2ap_earfcn_t ul_earfcn; 
    rrm_x2ap_earfcn_t dl_earfcn;
    U8                          ul_trnsmission_bdwidth; 
    U8                          dl_trnsmission_bdwidth;
}rrm_x2ap_fdd_info_t;

/*! \ rrm_x2ap_tdd_info_t
 *  \brief A struct that holds x2ap tdd information
 *  \param earfcn earfcn
 *  \param trnsmission_bdwidth txn bw
 *  \param subframe_assn subframe assn
 *  \param special_subframe_info
 */
typedef struct _rrm_x2ap_tdd_info_t
{
    rrm_x2ap_earfcn_t                earfcn;   
    U8                            trnsmission_bdwidth;
    U8                            subframe_assn; 
    rrm_x2ap_special_subframe_info_t special_subframe_info; 
}rrm_x2ap_tdd_info_t;

/*! \ rrm_x2ap_choice_eutra_mode_t
 *  \brief A struct that holds x2ap choice eutra mode
 * \param bitmask bitmask
 * \param fdd_info fdd info
 * \param tdd_info tdd info
 */
typedef struct _rrm_x2ap_choice_eutra_mode_t
{
    rrm_bitmask_t           bitmask;   
/*SPR 15752 Fix Start */ 
#define    RRM_X2AP_EUTRA_MODE_FDD   0x01
#define    RRM_X2AP_EUTRA_MODE_TDD   0x02
/*SPR 15752 Fix Stop */
    rrm_x2ap_fdd_info_t     fdd_info;  
    rrm_x2ap_tdd_info_t     tdd_info; 
}rrm_x2ap_choice_eutra_mode_t;

/*! \ rrm_x2ap_choice_eutra_mode_info_t
 *  \brief A struct that holds x2ap choice eutra mode info
 *  \param choice_eutra_mode
 */
typedef struct _rrm_x2ap_choice_eutra_mode_info_t
{
    rrm_x2ap_choice_eutra_mode_t    choice_eutra_mode;
}rrm_x2ap_choice_eutra_mode_info_t;

/*! \ rrm_x2ap_srvd_cell_info_t
 *  \brief A struct that holds x2ap server cell information
 *  \param bitmask bitmask
 *  \param pci pci
 *  \param cell_id cell id
 *  \param track_area_code track area code
 *  \param broadcast_plmn_info broadcast plmn info
 *  \param choice_eutra_mode choice eutra mode
 *  \param num_antenna_port number of antenna port
 */
typedef struct _rrm_x2ap_srvd_cell_info_t
{
    rrm_bitmask_t                       bitmask; /*^ BITMASK ^*/
    U32                               pci; /*^ M,0,N,0,0 ^*/
    rrm_x2ap_ecgi_t                      cell_id; /*^ M,0,N,0,0 ^*/
    rrm_x2ap_tac_t                       track_area_code; /*^ M,0,N,0,0 ^*/
    rrm_x2ap_broadcast_plmn_t            broadcast_plmn_info; /*^ M,0,N,0,0 ^*/
    rrm_x2ap_choice_eutra_mode_info_t    choice_eutra_mode; /*^ M,0,N,0,0 ^*/
    rrm_x2ap_num_antenna_port_et          num_antenna_port;  /*^ O,NUM_ANTENNA_PORT_BITMASK,B,1,4 ^*/
}rrm_x2ap_srvd_cell_info_t;

/*! \ rrm_x2ap_neighbour_info_t
 *  \brief A struct that holds x2ap neighbour info
 *  \param bitmask bitmask
 *  \param ecgi ecgi
 *  \param pci pci
 *  \param earfcn earfcn
 */
typedef struct _rrm_x2ap_neighbour_info_t
{
    rrm_bitmask_t         bitmask;
    rrm_x2ap_ecgi_t      ecgi;   /*^ M,0,N,0,0 ^*/
    U32               pci;    /*^ M,0,N,0,0 ^*/
    rrm_x2ap_earfcn_t    earfcn; /*^ M,0,N,0,0 ^*/
}rrm_x2ap_neighbour_info_t;

/*! \ rrm_x2ap_nbour_cell_arry_t
 *  \brief A struct that holds x2ap neighbour cell list
 *  \param num_neighbour_cell number of neighbour cell
 *  \param neighbour_info_arr[MAX_NEIGHBOURS] neighbour info
 */
typedef struct _rrm_x2ap_nbour_cell_arry_t
{
    U16                         num_neighbour_cell; /*^ M,0,B,1,5 ^*/
    rrm_x2ap_neighbour_info_t      neighbour_info_arr[MAX_NEIGHBOURS]; /*^ M,0,OCTET_STRING,VARIABLE ^*/
}rrm_x2ap_nbour_cell_arry_t;


/*! \ rrm_x2ap_srvd_cell_arry_t
 *  \brief A struct that holds x2ap served cell array 
 *  \param served_cell_info server cell info
 *  \param nbour_cell_info neighbour cell info
 */
typedef struct _rrm_x2ap_srvd_cell_arry_t
{
    rrm_x2ap_srvd_cell_info_t      served_cell_info;  
    rrm_x2ap_nbour_cell_arry_t     nbour_cell_info;  
}rrm_x2ap_srvd_cell_arry_t;

/*! \ rrm_x2ap_cell_info_t
 *  \brief A struct that holds x2ap cell information
 *  \param cell_id cell id
 *  \param ngh_dir_cell_arr ngh_dir_cell_arr
 */
typedef struct _rrm_x2ap_cell_info_t
{
    U16                      cell_id;
    rrm_x2ap_srvd_cell_arry_t   ngh_dir_cell_arr;
}rrm_x2ap_cell_info_t;

/*! \ rrm_x2ap_gu_group_id_t
 *  \brief A struct that holds x2ap gu group id 
 *  \param plmn_id
 *  \param  mme_group_id[MAX_MME_GRP_BYTES]
 */
typedef struct _rrm_x2ap_gu_group_id_t
{
    rrm_x2ap_plmn_identity_t    plmn_id; /*^ M,0,N,0,0 ^*/
    U8                       mme_group_id[MAX_MME_GRP_BYTES]; 
}rrm_x2ap_gu_group_id_t;

#ifdef ENDC_ENABLED
/* MENB CHANGES - START */
#define NR_CELL_ID_OCTET_SIZE    5
#define NR_GNB_ID_OCTET_SIZE     4

typedef struct
{
    U8    nrscs;
    /*^ M, 0, H, 0, MAX_NRCS ^*/
    /* x2ap_nrcs_et */

    U8    nrnrb;
    /*^ M, 0, H, 0, X2AP_NRB_MAX ^*/
    /* x2ap_nrnb_et */
}rrm_nr_tx_bandwidth_t;

typedef struct
{
    U32    freq_band_indicator_nr;
    /*^ M, 0, B, 1, NR_MAX_FREQ_BAND_INDICATOR  ^*/

    U8     num_supported_sul_band;
    /*^ M, 0, H, 0, NR_MAX_FREQ_BANDS ^*/

    U32    supported_sul_freq_band_indicator_nr[NR_MAX_FREQ_BANDS];
    /*^ M, 0, B, 1, NR_MAX_FREQ_BAND_INDICATOR ^*/
}rrm_freq_band_nr_info_t;

typedef struct
{
    U32                         nr_earfcn;
    /*^ M, 0, H, 0, NR_MAX_EARFCN ^*/

    U8                          num_freq_nr;
    /*^ M, 0, B, 1, NR_MAX_FREQ_BANDS ^*/

    rrm_freq_band_nr_info_t    freq_band_nr_info[NR_MAX_FREQ_BANDS];
    /*^ M, 0, N, 0, 0 ^*/
}rrm_nr_freq_info_t;


typedef struct _rrm_x2ap_nr_fdd_info_t
{
    rrm_nr_freq_info_t       nr_ul_freq_info;
    /*^ M, 0, N, 0, 0 ^*/

    rrm_nr_freq_info_t       nr_dl_freq_info;
    /*^ M, 0, N, 0, 0 ^*/

    rrm_nr_tx_bandwidth_t    nr_ul_tx_bdwidth;
    /*^ M, 0, N, 0, 0 ^*/

    rrm_nr_tx_bandwidth_t    nr_dl_tx_bdwidth;
    /*^ M, 0, N, 0, 0 ^*/

}rrm_x2ap_nr_fdd_info_t;

/*! \ rrm_x2ap_choice_nr_mode_info_t
 *  \brief A struct that holds x2ap choice nr mode
 * \param bitmask bitmask
 * \param fdd_info fdd info
 * \param tdd_info tdd info
 */
#define    RRM_X2AP_NR_MODE_FDD_PRESENT   0x01
#define    RRM_X2AP_NR_MODE_TDD_PRESENT   0x02
typedef struct _rrm_x2ap_choice_nr_mode_info_t
{
    rrm_bitmask_t           bitmask;   
    rrm_x2ap_nr_fdd_info_t     fdd_info;  
//    rrm_x2ap_tdd_info_t     tdd_info; 
}rrm_x2ap_choice_nr_mode_info_t;

/*! \ rrm_x2ap_nr_ecgi_t
 *  \brief A struct that holds x2ap ecgi
 *  \param plmn_identity plmn identity
 *  \param eutran_cell_id[HOME_ENB_ID_OCTET_SIZE] eutran_cell_id
 */
typedef struct _rrm_x2ap_nr_ecgi_t
{
    rrm_x2ap_plmn_identity_t  plmn_identity; 
    U8                        nr_cell_id[NR_CELL_ID_OCTET_SIZE]; 
}rrm_x2ap_nr_ecgi_t;

/* ENDC_MEAS_CHANGES_START */

typedef struct _rrm_x2ap_measurement_slots_t
{
  U8    numbits;
  /*^ M, 0, B, 1, 80 ^*/
  U8    meas_slots[10];
  /*^ M, 0, OCTET_STRING, VARIABLE, H, 0, 1 ^*/
}rrm_x2ap_measurement_slots_t;

typedef struct _rrm_x2ap_ss_rssi_measurement_t
{
   rrm_x2ap_measurement_slots_t measurement_slots;
   /*^ M, 0, N, 0, 0 ^*/
   U8   end_synmbol;
   /*^ M, 0, H, 0, 3 ^*/
}rrm_x2ap_ss_rssi_measurement_t;

typedef enum 
{
    RRM_X2AP_KHZ_15, 
    RRM_X2AP_KHZ_30, 
    RRM_X2AP_KHZ_60, 
    RRM_X2AP_KHZ_120, 
    RRM_X2AP_KHZ_240, 
    RRM_X2AP_KHZ_SPARE3, 
    RRM_X2AP_KHZ_SPARE2, 
    RRM_X2AP_KHZ_SPARE1 
}rrm_x2ap_subcarrier_spacing_et;

typedef enum
{
  RRM_X2AP_DURATION_SF1,
  RRM_X2AP_DURATION_SF2,
  RRM_X2AP_DURATION_SF3,
  RRM_X2AP_DURATION_SF4,
  RRM_X2AP_DURATION_SF5
}rrm_x2ap_mtc_duration_et;

#define RRM_X2AP_PERIODICTY_OFFSET_SF5_PRESENT    0x01
#define RRM_X2AP_PERIODICTY_OFFSET_SF10_PRESENT   0x02
#define RRM_X2AP_PERIODICTY_OFFSET_SF20_PRESENT   0x04
#define RRM_X2AP_PERIODICTY_OFFSET_SF40_PRESENT   0x08
#define RRM_X2AP_PERIODICTY_OFFSET_SF80_PRESENT   0x10
#define RRM_X2AP_PERIODICTY_OFFSET_SF160_PRESENT  0x20

typedef struct _rrm_x2ap_periodicity_and_offset_t
{
      U8                    bitmask;
      /*^ BITMASK ^*/
      U8                    sf5;
      /*^ O, RRM_X2AP_PERIODICTY_OFFSET_SF5_PRESENT , H, 0, 4 ^*/
      U8                    sf10;
      /*^ O, RRM_X2AP_PERIODICTY_OFFSET_SF10_PRESENT , H, 0, 9 ^*/
      U8                    sf20;
      /*^ O, RRM_X2AP_PERIODICTY_OFFSET_SF20_PRESENT , H, 0, 19 ^*/
      U8                    sf40;
      /*^ O, RRM_X2AP_PERIODICTY_OFFSET_SF40_PRESENT , H, 0, 39 ^*/
      U8                    sf80;
      /*^ O, RRM_X2AP_PERIODICTY_OFFSET_SF80_PRESENT , H, 0, 79 ^*/
      U8                    sf160;
      /*^ O, RRM_X2AP_PERIODICTY_OFFSET_SF160_PRESENT , H, 0, 159 ^*/
}rrm_x2ap_periodicity_and_offset_t;

typedef struct _rrm_x2ap_ssb_mtc_t
{
  rrm_x2ap_periodicity_and_offset_t periodicity_and_offset;
  /*^ M, 0, N, 0, 0 ^*/
  U8    mtc_duration; /* rrm_x2ap_mtc_duration_et */
  /*^ M, 0, H, 0, RRM_X2AP_DURATION_SF5 ^*/
}rrm_x2ap_ssb_mtc_t;

typedef struct _rrm_x2ap_meas_timing_config_t
{
      U8                    bitmask;
      /*^ BITMASK ^*/
      U32   carrier_freq;
      /*^ M, 0, H, 0, NR_MAX_EARFCN ^*/
      U8 ssb_subcarrier_spacing; /* rrm_x2ap_subcarrier_spacing_et */
      /*^ M, 0, H, 0, RRM_X2AP_KHZ_240 ^*/
      rrm_x2ap_ssb_mtc_t  ssb_meas_timimg_configuration;
      /*^ M, 0, N, 0, 0 ^*/
      rrm_x2ap_ss_rssi_measurement_t ss_rssi_measurement;
      /*^ O, 0, N, 0, 0 ^*/
}rrm_x2ap_meas_timing_config_t;
/* ENDC_MEAS_CHANGES_END */

/*! \ rrm_x2ap_srvd_nr_cell_info_t
 *  \brief A struct that holds x2ap server cell information
 *  \param bitmask bitmask
 *  \param pci pci
 *  \param cell_id cell id
 *  \param track_area_code track area code
 *  \param broadcast_plmn_info broadcast plmn info
 *  \param choice_eutra_mode choice eutra mode
 *  \param num_antenna_port number of antenna port
 */
typedef struct _rrm_x2ap_srvd_nr_cell_info_t
{
/* ENDC_MEAS_CHANGES_START */
    U16                               nr_pci; /*^ M,0,N,0,0 ^*/
/* ENDC_MEAS_CHANGES_END */
    rrm_x2ap_nr_ecgi_t                nr_cell_id; /*^ M,0,N,0,0 ^*/
    rrm_x2ap_tac_t                    track_area_code; /*^ M,0,N,0,0 ^*/
    rrm_x2ap_broadcast_plmn_t         broadcast_plmn_info; /*^ M,0,N,0,0 ^*/
    rrm_x2ap_choice_nr_mode_info_t    choice_nr_mode; /*^ M,0,N,0,0 ^*/
/* ENDC_MEAS_CHANGES_START */
    U8                                meas_timing_config_len;
    rrm_x2ap_meas_timing_config_t     meas_timing_config[32];
/* ENDC_MEAS_CHANGES_END */
}rrm_x2ap_srvd_nr_cell_info_t;


typedef struct _rrm_x2ap_nr_cell_info_t
{
    U16                               cell_id;
    rrm_x2ap_srvd_nr_cell_info_t      served_cell_info;  
}rrm_x2ap_nr_cell_info_t;

/*! \ rrm_x2_gb_gnb_id_t
 *  \brief A struct that holds Global gnb id 
 *  \param plmn_identity plmn id
 *  \param gnb_id gnb id
 */
typedef struct _rrm_x2_gb_gnb_id_t
{
    rrm_x2ap_plmn_identity_t plmn_identity;
    U8                       gNB_id[NR_GNB_ID_OCTET_SIZE]; 
}rrm_x2_gb_gnb_id_t;

typedef struct _rrm_nr_cell_info_t
{
    rrm_x2_gb_gnb_id_t          global_gnb_id;
    rrm_bool_et                 default_sgnb;
    U16                         num_served_nr_cell;
    rrm_x2ap_nr_cell_info_t     cell_info[MAX_SERVED_CELLS]; 
}rrm_nr_cell_info_t;
/* MENB CHANGES - END */
#endif

typedef struct _rrm_eutra_cell_info_t
{
    rrm_x2_gb_enb_id_t             global_enb_id;
    U16                         num_served_cell;
    rrm_x2ap_cell_info_t        cell_info[MAX_SERVED_CELLS]; 
    U8                          num_gu_group_id;
    rrm_x2ap_gu_group_id_t      group_id_arr[MAX_ENB_POOLS];
}rrm_eutra_cell_info_t;

/*! \ rrm_x2ap_link_up_ind_t
 *  \brief A struct that holds x2ap linkup ind message
 *  \param global_enb_id global enb id
 *  \param num_served_cell num of served cell
 *  \param cell_info[MAX_SERVED_CELLS] cell info
 *  \param num_gu_group_id num of group id
 *  \param group_id_arr[MAX_ENB_POOLS] group id
 */

#ifdef ENDC_ENABLED
/* MENB CHANGES - START */
#define RRM_EUTRA_CELL_INFO_PRESENT    0x01
#define RRM_NR_CELL_INFO_PRESENT       0x02
#endif

typedef struct _rrm_x2ap_link_up_ind_t
{
#ifdef ENDC_ENABLED
    rrm_bitmask_t bitmask;
    /*^ BITMASK ^*/
#endif

    rrm_eutra_cell_info_t   eutra_cell_info;
 
#ifdef ENDC_ENABLED
    rrm_nr_cell_info_t      nr_cell_info; 
#endif
}rrm_x2ap_link_up_ind_t;

/*! \ rrm_x2ap_enb_node_t 
 *  \brief Struct that contains X2 Enb information.
 *  \param sNode snode
 *  \param enb_id  enb id
 *  \param link_up_data link up data
 */
typedef struct _rrm_x2ap_enb_node_t 
{
    YLNODE                  sNode;    
    U32                      enb_id;
    rrm_x2ap_link_up_ind_t  link_up_data;
}rrm_x2ap_enb_node_t;
/*X2AP END */

/* Fix 960 Start */
typedef struct _rrm_tnl_discovery_trans_id_node_t
{
    YLNODE                  sNode;
    U16                     trans_id;
    rrm_global_enb_id_t     enb_id;
}rrm_tnl_discovery_trans_id_node_t;
/* Fix 960 End */

/* MLB RSU START */
/*! \ rrm_rsu_trans_info_node_t
 *  \brief rsu transfer info node
 *  \param sNode snode
 *  \param trans_id transaction id
 *  \param cell_index cell index
 *  \param bitmask bitmask
 *  \param intra_freq_cell[MAX_INTRA_FREQ_CELLS/RRM_EIGHT]
 *  \param inter_freq_cell[MAX_INTER_FREQ_CELLS/RRM_EIGHT]
 */
typedef struct _rrm_rsu_trans_info_node_t
{
    YLNODE                  sNode;
    U16                     trans_id;
    U32                     cell_index;
}rrm_rsu_trans_info_node_t;
/*! \ rrm_cell_meas_gb_t
 * \brief cell meas gb
 * \param meas_id meas id
 * \param g_enb_id enb id
 */
typedef struct _rrm_cell_meas_gb_t
{
    U32                     meas_id;
    rrm_x2_gb_enb_id_t      g_enb_id;
}rrm_cell_meas_gb_t;
/*! \ rrm_rsu_meas_info_node_t
 *  \brief rsu meas info node
 *  \param rsu_controlling_node controlling node
 *  \param rsu_id rsu id
 *  \param cell_index cell index
 */
typedef struct _rrm_rsu_meas_info_node_t
{
    YHNODE                  rsu_controlling_node;
    rrm_cell_meas_gb_t      rsu_id;
    U32                     cell_index;
}rrm_rsu_meas_info_node_t;
/*! \ rrm_rsu_meas_srv_cell_info_node_t
 *  \brief measure serving cell info
 *  \param rsu_serving_node rsu serving node
 *  \param rsu_id rsu id
 *  \param serving_cell[(RRM_MAX_NUM_CELLS/RRM_EIGHT) + RRM_ONE] serving cell
 *  \param report_characteristics report characterstics
 *  \param timer_id timer id
 */
typedef struct _rrm_rsu_meas_srv_cell_info_node_t
{
    YHNODE                  rsu_serving_node;
    rrm_cell_meas_gb_t      rsu_id;
    U8                      serving_cell[(RRM_MAX_NUM_CELLS/RRM_EIGHT) + RRM_ONE];
    U32                     report_characteristics;     /*^ M,0,H,0,15 ^*/
    rrm_timer_t             timer_id;
}rrm_rsu_meas_srv_cell_info_node_t;
/*! \ rrm_meas_cell_config_t
 *  \brief measure cell config
 *  \param cell_type cell type
 *  \param global_cell_id global cell id
 */
typedef struct _rrm_meas_cell_config_t
{
    rrm_oam_eutran_global_cell_id_t  global_cell_id;
}rrm_meas_cell_config_t;
/*! \ rrm_meas_rpt_config_info_t
 *  \brief meas rpt config info
 *  \param cell_index cell index
 *  \param num_cell num of cell
 *  \param cell_info[RRM_MAX_CELL_IN_ENB] cell info
 */
typedef struct _rrm_meas_rpt_config_info_t
{
    U16 cell_index;
    U16 num_cell;
    rrm_meas_cell_config_t  cell_info[RRM_MAX_CELL_IN_ENB];
}rrm_meas_rpt_config_info_t;

/* MLB RSU END */

/* Carrier_Aggregation_Start */
/*! \ rrm_load_actn_scell_deact_ind_t
 *  \param cell_index_of_deact_scell cell index of cell wich deactivated as scell
 *  \param num_of_user  number of impacted user
 *  \param global_cell_id global cell identity of cell
*/
typedef struct _rrm_load_actn_scell_deact_ind_t
{
    rrm_cell_index_t cell_index_of_deact_scell;
    U8 num_of_user;                 
    rrm_global_cell_id_t global_cell_id;
}rrm_load_actn_scell_deact_ind_t;
/* Carrier_Aggregation_End */

/*! \fn rrm_return_et rmcm_rmif_cell_del_req_reconfig_fail (rrm_cell_context_t *p_cell_ctx)
 *  \brief Cell deletion is expected due to reconfiguration failure
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_rmif_cell_del_req_reconfig_fail
(
 rrm_cell_context_t *p_cell_ctx
 );

/*! \fn rrm_cell_log_enable_disable_cmd(rrm_oam_log_on_off_et log_on_off,rrm_oam_log_level_et log_level) 
 *  \brief Cell log enable disable command as received from MIF
 *  \param log_on_off 
 *  \param log_level 
 */
void rrm_cell_log_enable_disable_cmd (
        rrm_oam_log_on_off_et log_on_off,
        rrm_oam_log_level_et log_level);

/*! \fn rrm_return_et rrm_decision_cell_delete(rrm_cell_context_t *p_cell_ctx) 
 *  \brief Cell need to be deleted for reconfig 
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rrm_decision_cell_delete
(
 rrm_cell_context_t *p_cell_ctx
 );

/*! \fn rrm_return_et cell_state_change_ind(rrm_cell_context_t *p_rrm_cell_ctx, rrm_cell_index_t cell_state) 
 *  \brief send periodic active ue  chnage INDICATION to MIF
 *  \param p_rrm_cell_ctx 
 *  \param cell_state 
 */
rrm_return_et
cell_state_change_ind
(
 rrm_cell_context_t *p_rrm_cell_ctx,
 rrm_cell_index_t cell_state
 );

/*! \fn rrm_return_et send_periodic_ue_active_ind(rrm_cell_context_t *p_rrm_cell_ctx, cell_son_attr_info_list_t *p_attribute_info, rrm_mif_cellm_message_resp_e resp, cell_timer_et timer_type)
 *  \brief send periodic active ue  chnage INDICATION to MIF
 *  \param p_rrm_cell_ctx 
 *  \param p_attribute_info  
 *  \param resp
 *  \param timer_type 
 */
rrm_return_et
send_periodic_ue_active_ind
(
 rrm_cell_context_t *p_rrm_cell_ctx,
 rrm_mif_cellm_message_resp_e resp
            /*SPR 17777 +-*/
 );
/*SPR-695 Fix Starts*/
/*! \fn rrm_return_et rrm_cellm_search_and_release_lowest_priority_arp(rrm_cell_index_t cell_index)
 *  \brief search and release lowest priority ARP
 *  \param  cell_index, count indicating number of UEs to release
 */
rrm_return_et 
rrm_cellm_search_and_release_lowest_priority_arp(rrm_cell_index_t cell_index , U8 count);
/*SPR-695 Fix Ends*/



/*! \fn rrm_return_et rrm_cellm_congestion_control_algo(rrm_cell_index_t cell_index) 
 *  \brief congestion control alogrithm
 *  \param  cell_index
 */

rrm_return_et 
rrm_cellm_congestion_control_algo(rrm_cell_index_t cell_index);

/*! \fn rrm_return_et rrm_request_kpi_from_l2(U16 duration, rrm_bool_et periodic_reporting) 
 *  \brief Send request to L2 for KPI
 *  \param duration 
 *  \param periodic_reporting 
 */
rrm_return_et
rrm_request_kpi_from_l2
(
 U16 duration,
 rrm_bool_et periodic_reporting,
 U8 cell_index     /* Multiple cell support */
 );

/*! \fn rrm_return_et send_cell_state_ind_to_mif(rrm_cell_context_t *p_cell_ctx, rrm_mif_cellm_message_resp_e resp, rrm_cell_index_t cell_state)
 *  \brief send state chnage INDICATION to MIF
 *  \param p_cell_ctx cell context 
 *  \param resp
 *  \param cell_state
 */
rrm_return_et
send_cell_state_ind_to_mif
(
 rrm_cell_context_t *p_cell_ctx,
 rrm_mif_cellm_message_resp_e resp,
 rrm_cell_index_t cell_state
 );
/*! \fn rrm_return_et send_threshold_to_ind(rrm_cell_context_t *p_cell_ctx,cell_son_attr_info_list_t  *p_attribute_info,  rrm_mif_cellm_message_resp_e resp)
 *  \brief send threshold INDICATION to MIF
 *  \param p_cell_ctx cell context 
 *  \param p_attribute_info
 *  \param resp
 */
rrm_return_et
send_threshold_to_ind
(
 rrm_cell_context_t *p_cell_ctx,
 cell_son_attr_info_list_t  *p_attribute_info,
 rrm_mif_cellm_message_resp_e resp
 );

/* __HK__ BEGIN */

/*! \ prb_usage 
 *  \brief  Struct contain information about PRB usage 
 *  \param  QCI 
 *  \param  prb_usage
 */
typedef struct prb_usage{
    U8 QCI;
    U8 prb_usage;
}prb_usage_t;

/*! \ update_prb_usage
 *  \brief Struct contains information about PRB usage in uplionk and downlink
 *  \param  prb_usage_ul[MAX_NUM_QCI] 
 *  \param  prb_usage_dl[MAX_NUM_QCI]
 */

typedef struct update_prb_usage{
    prb_usage_t prb_usage_ul[MAX_NUM_QCI];
    prb_usage_t prb_usage_dl[MAX_NUM_QCI];
}update_prb_usage;

/* __HK__ END */

typedef struct _rrm_cell_x2ap_rsu_meas_t
{
    U32           meas_id;
    rrm_bool_et   x2ap_load_req_send;
}rrm_cell_x2ap_rsu_meas_t;
/*! \ rrm_global_cell_index_t 
 *  \brief RRM GLOBAL CELL ID
 *  \param global_cell_id     
 *  \param cell_index    cell index
 *  \param p_cell_meas_rsu_info
 */

typedef struct _rrm_global_cell_index_t
{
    rrm_global_cell_id_t  global_cell_id;
    U8	   		  cell_index;
    rrm_cell_x2ap_rsu_meas_t p_cell_meas_rsu_info;
}rrm_global_cell_index_t;

typedef struct  _rrm_enb_tai_t
{
    rrm_oam_cell_plmn_info_t   plmn_id;
    U8    tac[TAC_OCTET_SIZE];     /* TAC_OCTET_SIZE = 2 */
}rrm_enb_tai_t;

/*spr_12440_fix Code removed*/

#define RRM_GLOBAL_ENB_ID_PRESENT                   0x0001
/* Carrier_Aggregation_End */


/*SPR 16477 Fix Start*/
typedef YLIST rrm_connected_mme_list_t;
/*SPR 16477 Fix Stop*/
typedef struct _rrm_enb_context_t
{
/* Carrier_Aggregation_Start */
    rrm_bitmask_t bitmask;          /*^ BITMASK ^*/
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
#define RRM_QCI_FOR_SGNB_ADDITION_PRESENT           0x0002
/*NR_DC Code Change Stop*/
#endif
/* Carrier_Aggregation_End */
    /* SPR_17367_Start */
    /* Code deleted */
    /* SPR_17367_End */
    rrm_global_enb_id_t            enb_id; /* RRM_GLOBAL_ENB_ID_PRESENT */
    U8                             num_ip_addr;
    rrm_enb_tnl_address_t          ip_addr[MAX_NO_ENB_X2_TLAS];
    rrm_enb_tai_t                  enb_tai;
    /*SPR 16477 Fix Start*/
    rrm_connected_mme_list_t       connected_mme_list; /*rrm_connected_mme_node_t*/
    /*SPR 16477 Fix Stop*/
    /* Spr 16211 Changes Start*/
    rrm_bool_t                     rim_feature_enable;
    /* Spr 16211 Changes End*/


#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    U8        qci_for_sgnb_addition;    /* RRM_QCI_FOR_SGNB_ADDITION_PRESENT */
/*NR_DC Code Change Stop*/
#endif
} rrm_enb_context_t;

/* BUG_825_Fix Start */
/*! \typedef enum rrm_global_timer_et
 *  \brief Global timer type in CELLM for periodic reporting of load ind.
 */
typedef enum
{
    CELL_PERIODIC_TIMER_FOR_NEIGH_LOAD_IND = CELL_MAX_TIMER_TYPE + RRM_ONE, /* This timer is used for sending self load to peer eNodeB */
    RRM_GLOBAL_MAX_TIMER_TYPE
}rrm_global_timer_et;

/* BUG_825_Fix End */

/* SPR 13383 changes start */
typedef struct _rrm_intra_non_conti_band_t
{
    U8    band;
    U8    bandwidth_class[RRM_TWO];
    U8    bandwidth_combination_set;
    U8    *bandwidth_combination_tbl;

}rrm_intra_non_conti_band_t;

typedef struct _rrm_intra_non_contigious_table_t
{
    U8                             number_of_combination;
    rrm_intra_non_conti_band_t    *p_intra_non_cont_band;

}rrm_intra_non_contigious_table_t;

/*! \ rrm_intra_band_t 
 *  \brief  Struct contain information about intra band table 
 *  \param  band 
 *  \param  bandwidth_class
 *  \param  num_of_bw_combination
 *  \param  bandwidth_combination_tbl
 */
typedef struct _rrm_intra_band_t
{
    U8    band;
    U8    bandwidth_class;
    U8    bandwidth_comb_set;
    U8    num_of_bw_combination;
    U8    *bandwidth_combination_tbl;

}rrm_intra_band_t;


/*! \ rrm_intra_band_contigious_table_t
 *  \brief  Struct contain information about intra band table 
 *  \param  num_of_band
 *  \param  ptr_band
 */
typedef struct _rrm_intra_band_contigious_table_t
{
    U8                    num_of_band;
    rrm_intra_band_t      *ptr_band;

}rrm_intra_band_contigious_table_t;


/*! \ rrm_sband_table_t
 *  \brief  Struct contain information sub table of band 
 *          with other bands relational combination 
 *  \param  band
 *  \param  num_of_bands_sub_table
 *  \param  band_comb_sub_table
 */
typedef struct _rrm_sband_table_t
{
    U8    *band;            
    U8	   num_of_bands_sub_table;
    U8	  *band_comb_sub_table;

}rrm_sband_table_t;


/*! \ rrm_interband_table_t
 *  \brief  Struct contain information Inter band table 
 *  \param  num
 *  \param  ptr_band
 */
typedef struct _rrm_interband_table_t
{
    U8	                  num;
    rrm_sband_table_t    *ptr_band;

}rrm_interband_table_t;


/*! \ rrm_ca_band_table_t
 *  \brief  Struct contain carrier aggregation band table information 
 *  \param  rrm_inter_band_tb
 *  \param  rrm_inta_band_cont_tb
 */
typedef struct _rrm_ca_band_table_t
{
    rrm_interband_table_t               rrm_inter_band_tb;
    rrm_intra_band_contigious_table_t   rrm_inta_band_cont_tb;
    rrm_intra_non_contigious_table_t    rrm_intra_band_non_cont_tb;
}rrm_ca_band_table_t;


/* SPR 13383 changes end  */

typedef YLIST rrm_tnl_discovery_trans_id_list_t;
/*SPR 10329:start*/
typedef YLIST rrm_enb_gu_group_id_list_t;  /*rrm_enb_gu_group_id_node_t*/
/*SPR 10329:end*/
/*! \ rrm_global_context_t
 *  \brief Struct that specifies the global cell context. 
 *  \param  timer_val[CELL_MAX_TIMER_TYPE]
 *  \param p_rrm_cell_ctx[RRM_MAX_NUM_CELLS]
 *  \param  rrm_x2_enb_list
 *  \param  arr_rrm_global_cell_index[RRM_MAX_NUM_CELLS] 
 *  \param  log_level
 *  \param  log_enabled
 *  \param  enb_context  
 *  \param  rrm_tnl_discovery_trans_id_list    
 *  \param  rsu_measid_hash_tbl  
 *  \param  rsu_measid_serving_hash_tbl  
 *  \param  rsu_trans_info_list  
 */
#define RRM_MAX_BUCKET_SIZE 15
typedef struct _rrm_global_context_t
{
    /* CA_Stage3_Change: Start */
    U8                 enb_cell_count;
    /* CA_Stage3_Change: End */
    rrm_cell_context_t *p_rrm_cell_ctx[RRM_MAX_NUM_CELLS];
    /*MC,MR and HO START */
    rrm_x2ap_enb_list_t     rrm_x2_enb_list;
    /*MC,MR and HO END */
    rrm_global_cell_index_t   arr_rrm_global_cell_index[RRM_MAX_NUM_CELLS];
    rrm_oam_log_level_et               log_level;/*value of log level*/
    rrm_oam_log_on_off_et         log_enabled; /*log enable or disable*/
    rrm_enb_context_t             enb_context;
    /* Fix 960 Start */
    rrm_tnl_discovery_trans_id_list_t    rrm_tnl_discovery_trans_id_list;
    /* Fix 960 End */
    /* MLB Changes start */
    YHASH rsu_measid_hash_tbl;
    YHASH rsu_measid_serving_hash_tbl;
    /* Store transaction information rrm_rsu_trans_info_node_t */
    rrm_rsu_trans_info_list_t           rsu_trans_info_list;
    /* MLB Changes end */
    /*SPR 10329:start*/
    rrm_enb_gu_group_id_list_t    rrm_enb_gu_group_id_list;  /*rrm_enb_gu_group_id_node_t*/
    /*SPR 10329:end*/
    /* SPR 13383 changes start */
    rrm_ca_band_table_t           rrm_ca_band_table;
    /* SPR 13383 changes end  */
#ifdef LTE_EMBMS_SUPPORTED
    /* SPR 18705:start */
    rrm_enqueued_sf_info_req_list_t enque_sf_info_list;  /*rrm_enqueued_sf_info_req_node_t*/
    /* SPR 18705:end */
#endif
}rrm_global_context_t;

/*! \fn load_fsm(rrm_source_value source, rrm_load_type type,rrm_cell_index_t cell_index) 
 *  \brief Function for handling load from different source.
 *  \param  source
 *  \param  type 
 *  \param  cell_index
 */
void
load_fsm(rrm_source_value source,
        rrm_load_type type,
        rrm_cell_index_t cell_index);

/*! \fn rrm_return_et rrm_cellm_ecn_capacity_enhance_req_process_msg(void *p_api,U16 api_id,U16 data_len)  
 *  \brief Function for processing congestion threshold message .
 *  \param  p_api 
 *  \param  api_id
 *  \param  data_len
 */
rrm_return_et
rrm_cellm_ecn_capacity_enhance_req_process_msg(

        void *p_api/*pointer to api buffer*/
            /*SPR 17777 +-*/
        );

/*! \fn rrm_return_et de_allocate_tcb_from_cell_ctxt(rrm_cell_context_t *p_cell_context, U16 rcvd_trans_id) 
 *  \brief  A function that deallocate TCB.
 *  \param  p_cell_context
 *  \param  rcvd_trans_id
 */
rrm_return_et 
de_allocate_tcb_from_cell_ctxt(
        rrm_cell_context_t *p_cell_context,
        U16                 rcvd_trans_id);

/*! \fn rrm_return_et rrm_cellm_cell_nmm_complete_process_msg(void *p_api,U16 api_id, U16 data_length) 
 *  \brief A function that process NMM complete message received from MIF.
 *  \param  p_api  
 *  \param api_id
 *  \param data_length
 */
rrm_return_et
rrm_cellm_cell_nmm_complete_process_msg(void *p_api);/*SPR 17777 +-*/


/*! \fn rrm_return_et rrm_cellm_update_prb_process_msg(U8 *p_api,U16 api_id, U16 data_len) 
 *  \brief A function that update prb usage as received from MIF.
 *  \param  p_api
 *  \param  api_id
 *  \param  data_len 
 *  \param  cell_index
 */
rrm_return_et
rrm_cellm_update_prb_process_msg
(
 U8 *p_api, /*pointer to input API buffer*/
            /*SPR 17777 +-*/
 U8  cell_index /* cell index */
 );
/*  DYNAMCI ICIC CHANGES START  */

rrm_return_et rrm_cellm_l2_scheduler_reconf_cnf
(
 void *p_api
 /*SPR 17777 +-*/
 );

rrm_return_et rrm_cellm_l1_meas_report_ind
(
 void *p_api,
 /*SPR 17777 +-*/
 U8  cell_index
 );
rrm_return_et rrm_cellm_l2_icic_meas_report
(
 void *p_api,
 /*SPR 17777 +-*/
 U8  cell_index
 );

rrm_return_et
rrm_cellm_cell_ue_update_ue_info_process_msg(
        rrm_void_t *p_api,/*pointer pt api buffer*/
        U16 api_id /*ADI id of incoming message*/
 /*SPR 17777 +-*/
        );



/*  DYNAMIC ICIC CHANGES END    */
/* CA_Stage3_Change: Start */
rrm_return_et rrm_cellm_cell_info_change_ind_resp(rrm_void_t *p_data,
                                     U16 api_id);
 /*SPR 17777 +-*/
/* CA_Stage3_Change: End */

/* BUG_11648_FIX_START */
rrm_return_et
rrm_cellm_cell_stop_broadcast_resp(
        rrm_void_t *p_api,/*pointer pt api buffer*/
        U16 api_id /*ADI id of incoming message*/
 /*SPR 17777 +-*/
        );
/* BUG_11648_FIX_END */

/*! \fn rrm_return_et rrm_cellm_cell_nmm_prepare_process_msg(void *p_api, U16 api_id, U16 data_length) 
 *  \brief A function that process NMM prepare message received from MIF.
 *  \param  p_api
 *  \param  api_id
 *  \param  data_length 
 */

rrm_return_et
rrm_cellm_cell_nmm_prepare_process_msg(void *p_api);/*SPR 17777 +-*/


/*! \fn rrm_calls_allowed_et rrm_cellm_ue_admission_allowed(rrm_cell_index_t cell_index) 
 *  \brief A function that allowed UE admission.
 *  \param cell_index
 */

rrm_calls_allowed_et
rrm_cellm_ue_admission_allowed(rrm_cell_index_t cell_index);

/*! \fn rrm_calls_allowed_et rrm_calls_allowed_et rrm_cellm_erb_setup_and_modification_allowed(rrm_cell_index_t cell_index) 
 *  \brief A function that allowed ERB modification.
 *  \param cell_index
 */
rrm_calls_allowed_et
rrm_cellm_erb_setup_and_modification_allowed(rrm_cell_index_t cell_index);

/*! \fn rrm_return_et build_and_send_generic_resp_to_mif(rrmcm_rmif_generic_resp_t  *p_generic_resp_to_mif,U16 api_id)
 *  \brief A function that send response to MIF.
 *  \param p_generic_resp_to_mif
 *  \param api_id
 */
rrm_return_et build_and_send_generic_resp_to_mif(
        rrmcm_rmif_generic_resp_t  *p_generic_resp_to_mif,
        U16                         api_id);
/* SPR 11681 Fix Start */
/* Code Deleted */
/* SPR 11681 Fix End */

/*! \fn rrm_cellm_add_ue(  rrm_cell_index_t cell_index, U8 ue_index)

 *  \brief function that specifies the addition of UE.
 *  \param cell_index    cell index
 *  \param ue_index      ue index
 */

void 
rrm_cellm_add_ue        				
(
 rrm_cell_index_t cell_index, 
 /* +- SPR 18268 */
 U16 ue_index 
 /* +- SPR 18268 */
 );

/*! \fn rrm_cellm_delete_ue(rrm_cell_index_t cell_index,U8 ue_index)
 *  \brief function that specifies the deleteion of UE.
 *  \param cell_index    cell index
 *  \param ue_index      ue index
 */

void 
rrm_cellm_delete_ue                                
(
 rrm_cell_index_t cell_index, 
 /* +- SPR 18268 */
 U16 ue_index 
 /* +- SPR 18268 */
 );

/*! \fn rrm_cellm_add_gbr_prb(rrm_cell_index_t cell_index,U8 dl_gbr_prb,U8 ul_gbr_prb)
 *  \brief function that specifies the addition of gbr prb.
 *  \param cell_index    cell index
 *  \param dl_gbr_prb    downlink gbr prb
 *  \param ul_gbr_prb    uplink gbr prb
 */
void 
rrm_cellm_add_gbr_prb                            	
(
 rrm_cell_index_t cell_index, 
 U8 dl_gbr_prb, 
 U8 ul_gbr_prb
 );

/*! \fn rrm_cellm_delete_gbr_prb( rrm_cell_index_t cell_index, U8 dl_gbr_prb, U8 ul_gbr_prb)
 *  \brief function that specifies the deleteion of gbr prb.
 *  \param   cell_index
 *  \param   dl_gbr_prb   
 *  \param  ul_gbr_prb
 */

void 
rrm_cellm_delete_gbr_prb                         	
(
 rrm_cell_index_t cell_index, 
 U8 dl_gbr_prb, 
 U8 ul_gbr_prb
 );

/*! \fn rrm_cellm_add_non_gbr_prb(rrm_cell_index_t cell_index,U8 dl_non_gbr_prb, U8 ul_non_gbr_prb)
 *  \brief function that specifies the addition of non gbr prb.
 *  \param   cell_index
 *  \param   dl_non_gbr_prb
 *  \param   ul_non_gbr_prb
 */

void 
rrm_cellm_add_non_gbr_prb                    	
(
 rrm_cell_index_t cell_index, 
 U8 dl_non_gbr_prb, 
 U8 ul_non_gbr_prb
 );

/*! \fn rrm_cellm_delete_non_gbr_prb(rrm_cell_index_t cell_index, U8 dl_non_gbr_prb, U8 ul_non_gbr_prb)
 *  \brief function that specifies the deleteion of non gbr prb.
 *  \param cell_index    
 *  \param dl_non_gbr_prb    
 *  \param ul_non_gbr_prb    
 */

void 
rrm_cellm_delete_non_gbr_prb                	
(
 rrm_cell_index_t cell_index, 
 U8 dl_non_gbr_prb,     
 U8 ul_non_gbr_prb
 );

/*! \fn U8 rrm_cellm_get_num_connected_ue(rrm_cell_index_t cell_index)

 *  \brief function that specifies the number of connected UE.
 *  \param cell_index
 */

U8  
rrm_cellm_get_num_connected_ue           		
(
 rrm_cell_index_t cell_index
 );

/*! \fn U16 rrm_cellm_get_total_cell_dl_gbr_prb(rrm_cell_index_t cell_index)
 *  \brief function that specifies the total cell downlink gbr prb.
 *  \param  cell_index
 */

U16 rrm_cellm_get_total_cell_dl_gbr_prb              	
(
 rrm_cell_index_t cell_index
 );

/*! \fn U16 rrm_cellm_get_total_cell_dl_gbr_prb( rrm_cell_index_t cell_index)
 *  \brief function that specifies the total cell downlink non gbr prb.
 *  \param cell_index
 */
U16 rrm_cellm_get_total_cell_dl_non_gbr_prb       	
(
 rrm_cell_index_t cell_index
 );

/*! \fn U16 rrm_cellm_get_used_cell_dl_gbr_prb( rrm_cell_index_t cell_index)
 *  \brief function that specifies the used cell downlink gbr prb.
 *  \param cell_index
 */
U16 
rrm_cellm_get_used_cell_dl_gbr_prb             	
(
 rrm_cell_index_t cell_index
 );

/*!\fn U16 rrm_cellm_get_used_cell_dl_non_gbr_prb(rrm_cell_index_t cell_index)
 * \brief function that specifies the used cell downlink non gbr prb.
 * \param cell_index
 */
U16 
rrm_cellm_get_used_cell_dl_non_gbr_prb      	
(
 rrm_cell_index_t cell_index
 );

/*! \fn U16 rrm_cellm_get_total_cell_ul_gbr_prb(rrm_cell_index_t cell_index)
 *  \brief function that specifies the used cell downlink non gbr prb.
 *  \param cell_index
 */

U16 
rrm_cellm_get_total_cell_ul_gbr_prb              	
(
 rrm_cell_index_t cell_index
 );

/*! \fn U16 rrm_cellm_get_total_cell_ul_non_gbr_prb( rrm_cell_index_t cell_index)
 *  \brief function that specifies total the used cell uplink non gbr prb.
 *  \param cell_index
 */
U16 
rrm_cellm_get_total_cell_ul_non_gbr_prb       	
(
 rrm_cell_index_t cell_index
 );

/*! \fn U16 rrm_cellm_get_used_cell_ul_gbr_prb( rrm_cell_index_t cell_index)
 *  \brief function that specifies the used cell uplink gbr prb.
 *  \param cell_index
 */
U16 
rrm_cellm_get_used_cell_ul_gbr_prb             	
(
 rrm_cell_index_t cell_index
 );

/*! \fn U16 rrm_cellm_get_used_cell_ul_gbr_prb(rrm_cell_index_t cell_index)
 *  \brief function that specifies the used cell uplink non gbr prb.
 *  \param cell_index
 */
U16 
rrm_cellm_get_used_cell_ul_non_gbr_prb      	
(
 rrm_cell_index_t cell_index
 );

/*Statistic functions*/

/*! \fn rrm_cellm_update_num_ue_admission_success(rrm_cell_index_t cell_index)
 *  \brief function that specifies number of UE admission success.
 *  \param  cell_index
 */
void 
rrm_cellm_update_num_ue_admission_success
(
 rrm_cell_index_t cell_index
 );

/*! \fn rrm_cellm_update_num_ue_admission_failure(rrm_cell_index_t cell_index)
 *  \brief function that specifies number of UE admission failure.
 *  \param cell_index
 */
void 
rrm_cellm_update_num_ue_admission_failure
(
 rrm_cell_index_t cell_index
 );

/*! \fn rrm_cellm_update_num_erb_admission_success(rrm_cell_index_t cell_index)
 *  \brief function that specifies ERB admission success.
 *  \param cell_index
 */
void 
rrm_cellm_update_num_erb_admission_success
(
 rrm_cell_index_t cell_index
 );

/*! \fn rrm_cellm_update_num_erb_admission_failure(rrm_cell_index_t cell_index)
 *  \brief function that specifies ERB admission failure.
 *  \param cell_index
 */
void 
rrm_cellm_update_num_erb_admission_failure
(
 rrm_cell_index_t cell_index
 );

/*! \fn rrm_cellm_update_num_ue_delete_success(rrm_cell_index_t cell_index)
 *  \brief function that specifies number of UE deleted success.
 *  \param cell_index
 */
void 
rrm_cellm_update_num_ue_delete_success
(
 rrm_cell_index_t cell_index
 );

/*! \fn rrm_cellm_update_num_ue_delete_failure(rrm_cell_index_t cell_index)
 *  \brief function that specifies number of UE deleted failure.
 *  \param cell_index
 */
void 
rrm_cellm_update_num_ue_delete_failure
(
 rrm_cell_index_t cell_index
 );

/*! \fn rrm_cellm_update_num_erb_delete_success(rrm_cell_index_t cell_index)
 *  \brief function that specifies number of ERB deleted success.
 *  \param cell_index
 */
void 
rrm_cellm_update_num_erb_delete_success
(
 rrm_cell_index_t cell_index
 );

/*! \fn rrm_cellm_update_num_erb_delete_failure(rrm_cell_index_t cell_index)
 *  \brief function that specifies number of ERB deleted failure.
 *  \param cell_index
 */
void 
rrm_cellm_update_num_erb_delete_failure
(
 rrm_cell_index_t cell_index
 );



/*! \fn rrm_return_et rmcm_rmif_cell_init_config(rrm_cell_context_t *p_cell_ctx)
 *  \brief handle cell init config request.
 *  \param p_cell_ctx cell context 
 */

rrm_return_et
rmcm_rmif_cell_init_config
(
 rrm_cell_context_t *p_cell_ctx /*cell context*/
 );

/*! \fn rrm_return_et rmcm_rmif_cell_stop_req(rrm_cell_context_t *p_cell_ctx)
 *  \brief handle cell stop request.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_rmif_cell_stop_req
(
 rrm_cell_context_t *p_cell_ctx /*cell context*/
 );
/*! \fn cell_stop_timer(QTIMER    p_timerId) 
 *  \brief A function for stopping the timer.
 *  \param p_timerId 
 */
void 
cell_stop_timer (QTIMER    p_timerId); 

/*! \fn cell_start_timer(cell_timer_et timerType, void *p_data, U16 size, rrm_cell_context_t *p_cell_ctx) 
 *  \brief for starting the timer.
 *  \param timerType 
 *  \param p_data 
 *  \param size 
 *  \param p_cell_ctx cell context 
 */

QTIMER cell_start_timer(cell_timer_et timerType, 
        void *p_data, 
        U16 size,
        rrm_cell_context_t *p_cell_ctx); 
/*! \fn rrm_return_et rmcm_rmif_cell_start_req(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle cell start request.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_rmif_cell_start_req
(
 rrm_cell_context_t *p_cell_ctx /*cell context*/
 );

/*! \fn rrm_return_et rmcm_rmif_cell_config_req(rrm_cell_context_t *p_cell_ctx)  
 *  \brief Handle cell config request.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_rmif_cell_config_req
(
 rrm_cell_context_t *p_cell_ctx /*cell context*/
 );

/*! \fn rrm_return_et rmcm_rmif_send_stop_resp(rrm_cell_context_t *p_cell_ctx)  
 *  \brief Send cell stop response To mif.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_rmif_send_stop_resp(U16 transaction_id,
    rrc_rrm_cell_stop_resp_t *rrc_rrm_cell_stop_resp
);

/*! \fn rrm_return_et rmcm_rmif_send_config_resp(rrm_cell_context_t *p_cell_ctx, U8 response, U16 fail_cause)  
 *  \brief send config fail response to MIF
 *  \param p_cell_ctx  
 *  \param response 
 *  \param fail_cause 
 */
rrm_return_et
rmcm_rmif_send_config_resp
(
 rrm_cell_context_t *p_cell_ctx, /*cell context*/
 U8 response,
 rrm_error_et fail_cause
 );

/*! \fn rrm_error_et fail_cause_mapping(U8 fail_cause) 
 *  \brief Fail cause mapping .
 *  \param  fail_cause 
 */
rrm_error_et fail_cause_mapping(U8 fail_cause);

/*! \fn rrm_return_et rmcm_rrc_cell_setup_resp(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle cell setup response.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_rrc_cell_setup_resp
(
 rrm_cell_context_t *p_cell_ctx /*cell context*/
 );
/* SPR 21660 changes start */
/*! \fn rrm_return_et rmcm_rmif_send_start_resp( 
    rrc_rrm_cell_start_resp_t  *rrc_rrm_cell_start_resp, 
    U16   transaction_id  )  
 *  \brief A Function that Send cell start response.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_rmif_send_start_resp
(
 rrc_rrm_cell_start_resp_t  *rrc_rrm_cell_start_resp, /* cell_start_resp */
 U16   transaction_id   /* transaction id */
);
/* SPR 21660 changes end */

/*! \fn rrm_return_et rrm_rrc_cell_start_resp(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle cell start response.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rrm_rrc_cell_start_resp
(
 rrm_cell_context_t *p_cell_ctx /* cell context */
 );

/*! \fn rrm_return_et rmcm_rmif_send_reconfig_ongoing_start_stop_fail(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle cell start fail in case of reconfig procedure.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_rmif_send_reconfig_ongoing_start_stop_fail
(
 rrm_cell_context_t *p_cell_ctx /*cell context */  

 );

/*! \fn rrm_return_et rmcm_rmif_send_reconfig_ongoing_start_succ(rrm_cell_context_t *p_cell_ctx)  
 *  \brief send successful start for reconfig.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_rmif_send_reconfig_ongoing_start_succ
(
 rrm_cell_context_t *p_cell_ctx /*cell context */ 

 );

/*! \fn rrm_return_et rmcm_rmif_reconfig_ongoing_start_resp(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle cell start response for reconfig.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_rmif_reconfig_ongoing_start_resp
(
 rrm_cell_context_t *p_cell_ctx /*cell context */ 

 );

/*! \fn rrm_bool_et rrm_decision_cell_stop(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle cell decision to stop or not for reconfig.
 *  \param p_cell_ctx cell context 
 */
rrm_bool_et
rrm_decision_cell_stop
(
 rrm_cell_context_t *p_cell_ctx /*cell context */

 );

/*SPR-9631 Fix Starts*/
/*! \fn rrm_return_et rmcm_mif_reconfig_resp_during_cell_stop(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle cell reconfig response.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_mif_reconfig_resp_during_cell_stop
(
 rrm_cell_context_t *p_cell_ctx /*cell context */ 

 );
/*SPR-9631 Fix Stop*/


/*! \fn rrm_return_et rmcm_mif_reconfig_req(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle cell reconfig request.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_mif_reconfig_req
(
 rrm_cell_context_t *p_cell_ctx /*cell context */ 

 );


/*! \fn rrm_return_et rmcm_cell_stop_send_reconfig_req(rrm_cell_context_t *p_cell_ctx)  
 *  \brief send cell stop in reconfigure procedure.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_cell_stop_send_reconfig_req
(
 rrm_cell_context_t *p_cell_ctx /*cell context */ 

 );

/*! \fn rrm_return_et cell_stop_for_reconfig(rrm_cell_context_t *p_cell_ctx)  
 *  \brief Struct that handle cell start request.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
cell_stop_for_reconfig
(
 rrm_cell_context_t *p_cell_ctx /*cell context */ 

 );

/*! \fn rrm_return_et rmcm_rmif_reconfig_ongoing_stopped_resp(rrm_cell_context_t *p_cell_ctx)  
 *  \brief stop success for reconfig procedure.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_rmif_reconfig_ongoing_stopped_resp
(
 rrm_cell_context_t *p_cell_ctx /*cell context */ 

 );

/*! \fn rrm_return_et rmcm_rmif_send_stop_succ_resp(rrm_cell_context_t *p_cell_ctx)  
 *  \brief cell stop success response.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_rmif_send_stop_succ_resp
(
 rrm_cell_context_t *p_cell_ctx
 );

/*! \fn rrm_return_et rmcm_rmif_send_stop_fail_resp(rrm_cell_context_t *p_cell_ctx)  
 *  \brief send stop fail response.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_rmif_send_stop_fail_resp
(
 rrm_cell_context_t *p_cell_ctx /*cell context */
 );

/*! \fn rrm_return_et rmcm_rmif_cell_del_req(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle cell delete request.
 *  \param p_cell_ctx cell context

 */
rrm_return_et
rmcm_rmif_cell_del_req
(
 rrm_cell_context_t *p_cell_ctx
 );

/*! \fn rrm_return_et rmcm_rmif_cell_del_resp(rrm_cell_context_t *p_cell_ctx)  
 *  \brief cell delete response.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_rmif_cell_del_resp
(
 rrm_cell_context_t *p_cell_ctx        /* cell context */
 );

/*! \fn rrm_return_et rmcm_mif_update_req(rrm_cell_context_t *p_cell_ctx)
 *  \brief handle cell update request.
 *  \param p_cell_ctx cell context
 */
rrm_return_et
rmcm_mif_update_req
(
 rrm_cell_context_t *p_cell_ctx
 );

/*! \fn rrm_return_et rmcm_rrc_update_resp(rrm_cell_context_t *p_cell_ctx)
 *  \brief handle update response.
 *  \param p_cell_ctx cell context
 */
rrm_return_et
rmcm_rrc_update_resp
(
 rrm_cell_context_t *p_cell_ctx        /* cell context */

 );

/*! \fn rrm_return_et rmcm_cell_send_del_to_ue(rrm_cell_context_t *p_cell_ctx)  
 *  \brief send delete request to UE.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_cell_send_del_to_ue
(
 rrm_cell_context_t  *p_cell_ctx  /* cell context */

 );

/*! \fn rrm_return_et rmcm_mif_send_reconfig_resp(rrm_cell_context_t *p_cell_ctx)  
 *  \brief send reconfig response.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_mif_send_reconfig_resp
(
 rrm_cell_context_t *p_cell_ctx  /* cell context */

 );

/*! \fn  rrm_return_et rmcm_rrc_reconfig_resp(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle reconfig response.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_rrc_reconfig_resp
(
 rrm_cell_context_t *p_cell_ctx        /* cell context */

 );


/*! \fn rrm_void_t commit_cell_reconfig_params(rrm_cell_context_t *p_cell_ctx)
 *  \brief update cell context accoring to reconfig parameters.
 *  \param p_cell_ctx 
 */
rrm_void_t
commit_cell_reconfig_params
(
 rrm_cell_context_t *p_cell_ctx  /* cell context */

 );


/*! \fn rrm_void_t commit_reconfig_param(rrm_cell_context_t *p_cell_ctx)  
 *  \brief update cell context accoring to reconfig parameters.
 *  \param p_cell_ctx cell context 
 */
rrm_void_t
commit_reconfig_param
(
 rrm_cell_context_t *p_cell_ctx  /* cell context */

 );

/*! \fn rrm_return_et rmcm_rmif_reconfig_ongoing_reconfig_resp(rrm_cell_context_t *p_cell_ctx)  
 *  \brief reconfifg fail cell was stopped.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_rmif_reconfig_ongoing_reconfig_resp
(
 rrm_cell_context_t *p_cell_ctx        /* cell context */

 );

/*! \fn rrm_return_et rmcm_start_for_reconfig(rrm_cell_context_t *p_cell_ctx)  
 *  \brief cell start in case of reconfig.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_start_for_reconfig(
        rrm_cell_context_t *p_cell_ctx   /* cell context */ 

        );

/*! \fn rrm_return_et rmcm_mif_send_del_resp(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle delete response.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_mif_send_del_resp
(
 rrm_cell_context_t *p_cell_ctx  /* cell context */ 

 );

/*! \fn rrm_return_et rmcm_ue_del_resp(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle ue delete response from UEM.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_ue_del_resp
(
 rrm_cell_context_t *p_cell_ctx        /* cell context */
 );

/*! \fn rrm_return_et rmcm_mif_send_start_adm_resp(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle cell start admission response.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_mif_send_start_adm_resp
(
 rrm_cell_context_t *p_cell_ctx  /* cell context */

 );

/*! \fn rrm_return_et rmcm_mif_start_adm_req(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle cell start admission request.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_mif_start_adm_req
(
 rrm_cell_context_t *p_cell_ctx  /* cell context */

 );

/*! \fn rrm_return_et rmcm_mif_stop_adm_resp(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle cell stop admission request response.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_mif_stop_adm_resp
(
 rrm_cell_context_t *p_cell_ctx  /* cell context */ 

 );

/*! \fn rrm_return_et rmcm_mif_stop_adm_req(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle cell stop admission request.
 *  \param *p_cell_ctx 
 */
rrm_return_et
rmcm_mif_stop_adm_req
(
 rrm_cell_context_t *p_cell_ctx /*cell context*/
 );

/*! \fn rrm_return_et rmcm_mif_failed_event(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle cell failed event.
 *  \param *p_cell_ctx 
 */
rrm_return_et
rmcm_mif_failed_event
(
 rrm_cell_context_t *p_cell_ctx  /*cell context*/      
 );

/*! \fn rrm_void_t rmcm_mif_set_log_level_resp(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle set log level response for cell.
 *  \param *p_cell_ctx 
 */
rrm_void_t
rmcm_mif_set_log_level_resp
(
 rrm_cell_context_t *p_cell_ctx    /* cell context */ 

 );

/*! \fn rrm_return_et rmcm_mif_register_req(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle cell register request.
 *  \param *p_cell_ctx 
 */
rrm_return_et
rmcm_mif_register_req
(
 rrm_cell_context_t *p_cell_ctx  /* cell context */ 

 );

/*! \fn rrm_return_t rmcm_mif_register_resp(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle cell register request.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_mif_register_resp
(
 rrm_cell_context_t *p_cell_ctx  /* cell context */ 

 );
/*! \fn rrm_return_et rmcm_mif_deregister_req(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle cell deregister request.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_mif_deregister_req
(
 rrm_cell_context_t *p_cell_ctx  /* cell context */ 

 );

/*! \fn rrm_return_et rmcm_mif_deregister_resp(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle cell deregister response.
 *  \param p_cell_ctx cell context 
 */
rrm_return_et
rmcm_mif_deregister_resp
(
 rrm_cell_context_t *p_cell_ctx  /* cell context */ 

 );
/*! \fn rrm_void_t rmcm_mif_init_config_resp(rrm_cell_context_t *p_cell_ctx)  
 *  \brief handle cell config response.
 *  \param p_cell_ctx cell context 
 */
rrm_void_t
rmcm_mif_init_config_resp
(
 rrm_cell_context_t *p_cell_ctx  /* cell context */ 

 );

/******CELL UPDATE**************/
/*! \fn rrm_return_et rmcm_rmif_send_update_ongoing_start_stop_fail(rrm_cell_context_t *p_cell_ctx)
 *  \brief handle cell start fail in case of update procedure.
 *  \param p_cell_ctx cell context cell context
 */
rrm_return_et
rmcm_rmif_send_update_ongoing_start_stop_fail
(
 rrm_cell_context_t *p_cell_ctx  /* cell context */ 

 );

/*! \fn rrm_return_et rmcm_rmif_send_update_ongoing_start_succ(rrm_cell_context_t *p_cell_ctx)
 *  \brief send successful start for update.
 *  \param *p_cell_ctx
 */
rrm_return_et
rmcm_rmif_send_update_ongoing_start_succ
(
 rrm_cell_context_t *p_cell_ctx /*cell context*/
 );

/*! \fn rrm_return_et rmcm_rmif_update_ongoing_start_resp(rrm_cell_context_t *p_cell_ctx)
 *  \brief handle cell start response for update.
 *  \param *p_cell_ctx
 */
rrm_return_et
rmcm_rmif_update_ongoing_start_resp
(
 rrm_cell_context_t *p_cell_ctx /* cell context */
 );

/*! \fn rrm_return_et rmcm_mif_update_req(rrm_cell_context_t *p_cell_ctx)
 *  \brief handle cell update request.
 *  \param *p_cell_ctx
 */
rrm_return_et
rmcm_mif_update_req
(
 rrm_cell_context_t *p_cell_ctx /* cell context */ 
 );

/*! \fn rrm_return_et rmcm_cell_stop_send_update_req(rrm_cell_context_t *p_cell_ctx)
 *  \brief send cell stop in update procedure.
 *  \param *p_cell_ctx
 */
rrm_return_et
rmcm_cell_stop_send_update_req
(
 rrm_cell_context_t *p_cell_ctx   /* cell context */ 

 );

/*! \fn rrm_return_et cell_stop_for_update(rrm_cell_context_t *p_cell_ctx)
 *  \brief Struct that handle cell start request.
 *  \param *p_cell_ctx
 */
rrm_return_et
cell_stop_for_update
(
 rrm_cell_context_t *p_cell_ctx  /* cell context */ 

 );

/*! \fn rrm_return_et rmcm_rmif_update_ongoing_stopped_resp(rrm_cell_context_t *p_cell_ctx)
 *  \brief stop success for update procedure.
 *  \param *p_cell_ctx
 */
rrm_return_et
rmcm_rmif_update_ongoing_stopped_resp
(
 rrm_cell_context_t *p_cell_ctx  /* cell context */ 

 );

/*! \fn rrm_return_et rmcm_mif_send_update_resp(rrm_cell_context_t *p_cell_ctx)
 *  \brief send update response.
 *  \param *p_cell_ctx
 */
rrm_return_et
rmcm_mif_send_update_resp
(
 rrm_cell_context_t *p_cell_ctx  /* cell context */ 

 );

/*! \fn rrm_return_et rmcm_rmif_update_ongoing_update_resp(rrm_cell_context_t *p_cell_ctx)
 *  \brief update fail cell was stopped.
 *  \param *p_cell_ctx
 */
rrm_return_et
rmcm_rmif_update_ongoing_update_resp
(
 rrm_cell_context_t *p_cell_ctx        /* cell context */

 );

/*! \fn rrm_return_et rmcm_start_for_update(rrm_cell_context_t *p_cell_ctx)
 *  \brief cell start in case of update.
 *  \param *p_cell_ctx
 */
rrm_return_et
rmcm_start_for_update(
        rrm_cell_context_t *p_cell_ctx  /* cell context */ 

        );

/*! \fn rrm_void_t commit_update_param(rrm_cell_context_t *p_cell_ctx)
 *  \brief update cell context accoring to update parameters.
 *  \param *p_cell_ctx 
 */
rrm_void_t
commit_update_param
(
 rrm_cell_context_t *p_cell_ctx  /* cell context */ 
 );

/******CELL UPDATE**************/
/* SPR 21527 Start */
/*! \fn U16 rrm_cellm_get_num_active_ue(rrm_cell_index_t cell_index)
 *  \brief Function that get number of active UE
 *  \param cell_index 
 */
U16 
/* SPR 21527 End */
rrm_cellm_get_num_active_ue
(
 rrm_cell_index_t cell_index
 );

/*! \fn rrm_bool_et rrm_cellm_is_rac_enabled(rrm_cell_index_t cell_index)
 *  \brief Function that checks if RAC is enabled  
 *  \param cell_index 
 */
rrm_bool_et
rrm_cellm_is_rac_enabled(rrm_cell_index_t cell_index);

/*! \fn rrm_return_et rrm_send_internal_msg(U16 dst_module_id,U16 api_id,U16 msg_size,void *p_msg)
 *  \brief Function for sending internal message to MIF and UEM.
 *  \param  dst_module_id 
 *  \param  api_id 
 *  \param  msg_size 
 *  \param  p_msg 
 */
rrm_return_et
rrm_send_internal_msg(
        U16     dst_module_id,
        U16     api_id,
        U16     msg_size,
        void    *p_msg);

/*! \fn  meas_eutran_node_t * rrmcm_find_eutran_meas_context( U16 arfcn, rrm_cell_context_t *p_cell_ctx) 
 *  \param  arfcn
 *  \param p_cell_ctx cell context
 *  \brief function for finding eutran measurement context. 
 */
meas_eutran_node_t *
rrmcm_find_eutran_meas_context
(
 U16                   arfcn,
 rrm_cell_context_t   *p_cell_ctx /*cell context*/
 );
/*! \fn  meas_geran_node_t * rrmcm_find_geran_meas_context(U16 arfcn,rrm_son_geran_band_indicator_et band_ind,rrm_cell_context_t *p_cell_ctx)
 *  \brief function for finding geran measurement context. 
 *  \param  arfcn
 *  \param  band_ind
 *  \param p_cell_ctx cell context
 */
meas_geran_node_t *
rrmcm_find_geran_meas_context
(
 U16 arfcn,
 rrm_son_geran_band_indicator_et     band_ind,
 rrm_cell_context_t *p_cell_ctx
 );
/*! \fn  meas_utran_node_t * rrmcm_find_utran_meas_context(U16 arfcn, rrm_cell_context_t *p_cell_ctx) 
 *  \param arfcn
 *  \param p_cell_ctx cell context
 *  \brief Function for finding utran measurement context. 
 */
meas_utran_node_t *
rrmcm_find_utran_meas_context
(
 U16                   arfcn,
 rrm_cell_context_t   *p_cell_ctx /*cell context*/
 );

/* TDD Support SON Start */
/*! \fn  meas_utran_tdd_node_t * rrmcm_find_utran_tdd_meas_context(U16 arfcn, rrm_cell_context_t *p_cell_ctx) 
 *  \param arfcn
 *  \param p_cell_ctx cell context
 *  \brief Function for finding utran tdd measurement context. 
 */
meas_utran_tdd_node_t *
rrmcm_find_utran_tdd_meas_context
(
 U16                   arfcn,
 rrm_cell_context_t   *p_cell_ctx /*cell context*/
 );
/* TDD Support SON End */


/*! \fn rrm_return_et rrmcm_free_meas_config_lists( rrm_cell_context_t *p_cell_ctx) 
 *  \param p_cell_ctx cell context
 *  \brief Function for finding free list of measurement config.
 */
rrm_return_et
rrmcm_free_meas_config_lists
(
 rrm_cell_context_t    *p_cell_ctx  /* cell context */ 

 );

/*! \fn meas_utran_node_t * rrm_utran_list_dequeue( meas_utran_context_list_t *p_meas_utran_list) 
 *  \param p_meas_utran_list 
 *  \brief Function for finding utran list. 
 */
meas_utran_node_t *
rrm_utran_list_dequeue
( 
 meas_utran_context_list_t    *p_meas_utran_list
 );

/* TDD Support SON Start */
/*! \fn meas_utran_tdd_node_t * rrm_utran_tdd_list_dequeue( meas_utran_tdd_context_list_t *p_meas_utran_tdd_list) 
 *  \param p_meas_utran_tdd_list 
 *  \brief Function for finding utran tdd list. 
 */
meas_utran_tdd_node_t *
rrm_utran_tdd_list_dequeue
( 
 meas_utran_tdd_context_list_t    *p_meas_utran_tdd_list
 );
/* TDD Support SON End */


/*! \fn meas_eutran_node_t *rrm_eutran_list_dequeue( meas_eutran_context_list_t *p_meas_eutran_list) 
 *  \param p_meas_eutran_list
 *  \brief Function for finding EUTRAN dequeue list. 
 */
meas_eutran_node_t *
rrm_eutran_list_dequeue
( 
 meas_eutran_context_list_t    *p_meas_eutran_list
 );
/*! \fn map_flag_periodic_s1_msg_notification_info(rrm_cellm_trigger_type_et trigger_id, cell_son_attr_info_list_t *p_cell_attrbte_info)
 *  \brief Function for mapping trigger type of S1 meassage.
 *  \param trigger_id
 *  \param p_cell_attrbte_info
 */

void 
map_flag_periodic_s1_msg_notification_info
(
 rrm_cellm_trigger_type_et  trigger_id,
 cell_son_attr_info_list_t  *p_cell_attrbte_info
 );
/* !\fn rrm_return_et rrm_cellm_cell_initialize_sr_res_list( rrm_cell_context_t *p_cell_context)
 * \param p_cell_context
 * \breif initialize the SR resource list
 */
rrm_return_et
rrm_cellm_cell_initialize_sr_res_list
(
 rrm_cell_context_t *p_cell_context  /* cell context */ 

 );
/* \fn rrm_void_t rrm_cellm_cell_initialize_cqi_res_list(rrm_cell_context_t *p_cell_context,cqi_ri_map_info_per_ue     *p_cqi_ri_table_map,U16 *p_alloc_cqi_ri_from_algo_val, U16 *p_cqi_periodicity)
 * \param   p_cell_context
 * \param   p_cqi_ri_table_map
 * \param   p_alloc_cqi_ri_from_algo_val
 * \param   p_cqi_periodicity
 * \brief initialize the CQI resource list
 */
rrm_return_et
rrm_cellm_cell_initialize_cqi_res_list
(
 rrm_cell_context_t       *p_cell_context,
 cqi_ri_map_info_per_ue   *p_cqi_ri_table_map,
 U16                      *p_alloc_cqi_ri_from_algo_val
 /*SPR 17777 +-*/

 );
/*! \fn  rrm_void_t rrm_cellm_cell_initialize_srs_res_list( rrm_cell_context_t *p_cell_context)
 * \param  p_cell_context  
 *  \brief initialize the SRS resource list */
rrm_void_t
rrm_cellm_cell_initialize_srs_res_list
(
 rrm_cell_context_t *p_cell_context
 );
/**ETWS/CMAS**/
const void * rrm_cellm_pws_request_sched_data_key
(
 const YLNODE *p_ylnode
 );
const void * rrm_cellm_pws_request_sched_data_sib_type_key
(
 const YLNODE *p_ylnode
 );
int rrm_cellm_shed_data_key_compare
(
 const void *p_key1,
 const void *p_key2
 );
int rrm_cellm_shed_data_sib_type_key_compare
(
 const void *p_key1,
 const void *p_key2
 );
rrm_return_et
build_and_send_tnl_discovery_resp_to_mif
(
 rrmcm_rmif_son_tnl_discovery_resp_t  *p_tnl_discovery_resp,
 U16                         api_id
 );

rrm_return_et
rrm_cellm_tnl_discovery_process_msg
(
 void *p_api/*pointer pt api buffer*/
 /*SPR 17777 +-*/
 );




rrm_void_t
rrm_list_delete_node
(
 YLIST        *p_list,
 YLNODE       *p_node
 );

rrm_void_t print_cell_informaton_for_pws
(
 rrm_cell_context_t *p_cell_context
 );

/* RRM_MAC_RECONF_SCHEDULER_CHANGES_START */
rrm_return_et
rrm_cellm_l2_reconfig_scheduler_process_msg
(
 rrm_void_t  *l2_reconf_scheduler_req/* Pointer to API specific information. */
 /*SPR 17777 +-*/
 );

/* RRM_MAC_RECONF_SCHEDULER_CHANGES_ENDS */

rrm_return_et chk_global_cell_id(
        rrm_global_cell_id_t     *p_rcvd,
        rrm_global_cell_id_t     *p_strd);

/* COMP_WARN_1_FEB:compilation warning removal */
rrm_return_et 
rrm_cellm_populate_nbr_rsu_in_cell_context
(
 x2ap_rrm_rsu_enb_update_ind_t *p_x2ap_rrm_rsu_enb_update_ind,
	/* eICIC_PHASE_1_2_CHANGES_START */
	rrm_cell_context_t *p_cell_ctx ,
    /* eICIC_PHASE_1_2_CHANGES_END */
 U16 report_cnt,
 rrm_eutran_cell_types_et cell_type,
 U16 nbr_cell_idx
 );

rrm_common_params_for_eutra_t*  
rrm_cellm_get_connected_mode_common_params
(
 rrm_cell_index_t cell_index
 );

rrm_return_et
rrm_send_pdcp_config_param_req
(
 RrmPdcpEcnConfigParamReq *rrm_pdcp_config_param_req,/* Pointer to API specific information. */
 U16 src_module_id,  /* Source module identifier */
 U16 transaction_id, /* Interface transaction identifier */
 U8  cell_index      /* cell index */
 );
rrm_return_et
rrm_send_pdcp_kpi_thp_req
(
 PdcpConfigureKpiStatsReqParams *p_pdcp_kpi_config,
 U16 source_module,
 U16 tran_id,
 U8  cell_index
 );
typedef rrm_void_t (*rrm_cell_bitmap_func_ptr)(rrmcm_kpi_data_t *, rrmcm_kpi_data_t *, rrm_bool_et *);
//extern rrm_cell_bitmap_func_ptr rrm_cellm_map_bit[RRM_MAX_KPI][RRM_EIGHT]; 
/* MLB Start */

typedef rrm_return_et   (*rrm_cell_load_actn_func_ptr) (rrm_cell_context_t *, rrm_cell_load_def_t *, U8, U8);
/* Function Pointer Variable declaration for cell load action handlar */
extern rrm_cell_load_actn_func_ptr rrm_cell_load_actn[RRM_ACTN_MAX];


/*! \fn rrm_return_et rrm_cell_load_action_none_hndl(rrm_cell_context_t *p_cell_ctxt, rrm_cell_load_def_t *p_cell_ld_def)
 *  \brief Function for handling cell load for action NONE.
 *  \param p_cell_ctxt
 *  \param p_cell_ld_def
 */
rrm_return_et
rrm_cell_load_action_none_hndl
(
 rrm_cell_context_t *p_cell_ctxt,
 rrm_cell_load_def_t *p_cell_ld_def,
 U8 ld_choice,
 U8 resrc_cnt
 );

/*! \fn rrm_return_et rrm_cell_load_action_ho_hndl(rrm_cell_context_t *p_cell_ctxt, rrm_cell_load_def_t *p_cell_ld_def)
 *  \param p_cell_ctxt 
 *  \param p_cell_ld_def
 *  \brief Function for handling cell load for action HO.
 */
rrm_return_et
rrm_cell_load_action_ho_hndl
(
 rrm_cell_context_t *p_cell_ctxt,
 rrm_cell_load_def_t *p_cell_ld_def,
 U8 ld_choice,
 U8 resrc_cnt
 );

/*! \fn rrm_return_et rrm_cell_load_action_stop_adm_hndl(rrm_cell_context_t *p_cell_ctxt, rrm_cell_load_def_t *p_cell_ld_def)
 *  \param p_cell_ctxt
 *  \param p_cell_ld_def
 *  \brief Function for handling cell load for action Stop ADM.
 */
rrm_return_et
rrm_cell_load_action_stop_adm_hndl
(
 rrm_cell_context_t *p_cell_ctxt,
 rrm_cell_load_def_t *p_cell_ld_def,
 U8 ld_choice,
 U8 resrc_cnt
 );

/*! \fn rrm_return_et rrm_cell_load_action_rel_ue_hndl(rrm_cell_context_t *p_cell_ctxt, rrm_cell_load_def_t *p_cell_ld_def)
 *  \param p_cell_ctxt 
 *  \param  p_cell_ld_def
 *  \brief Function for handling cell load for action Rel UE.
 */
rrm_return_et
rrm_cell_load_action_rel_ue_hndl
(
 rrm_cell_context_t *p_cell_ctxt,
 rrm_cell_load_def_t *p_cell_ld_def,
 U8 ld_choice,
 U8 resrc_cnt
 );

/*! \fn rrm_return_et rrm_cell_load_action_aqm_hndl(rrm_cell_context_t *p_cell_ctxt, rrm_cell_load_def_t *p_cell_ld_def)
 *  \param p_cell_ctxt 
 *  \param p_cell_ld_def
 *  \brief Function for handling cell load for action AQM
 */
rrm_return_et
rrm_cell_load_action_aqm_hndl
(
 rrm_cell_context_t *p_cell_ctxt,
 rrm_cell_load_def_t *p_cell_ld_def,
 U8 ld_choice,
 U8 resrc_cnt
 );

/*! \fn rrm_return_et rrm_cell_load_action_ecn_ul_dir_hndl(rrm_cell_context_t *p_cell_ctxt, rrm_cell_load_def_t *p_cell_ld_def)
 *  \param p_cell_ctxt
 *  \param p_cell_ld_def
 *  \brief Function for handling cell load for action ECN UL DIR.
 */
rrm_return_et
rrm_cell_load_action_ecn_ul_dir_hndl
(
 rrm_cell_context_t *p_cell_ctxt,
 rrm_cell_load_def_t *p_cell_ld_def,
 U8 ld_choice,
 U8 resrc_cnt
 );

/*! \fn rrm_return_et rrm_cell_load_action_ecn_dl_dir_hndl(rrm_cell_context_t *p_cell_ctxt, rrm_cell_load_def_t *p_cell_ld_def)
 *  \param p_cell_ctxt
 *  \param p_cell_ld_def
 *  \brief Function for handling cell load for action ECN DL DIR.
 */
rrm_return_et
rrm_cell_load_action_ecn_dl_dir_hndl
(
 rrm_cell_context_t *p_cell_ctxt,
 rrm_cell_load_def_t *p_cell_ld_def,
 U8 ld_choice,
 U8 resrc_cnt
 );

/*! \fn rrm_return_et rrm_cell_load_action_ecn_bi_dir_hndl(rrm_cell_context_t *p_cell_ctxt, rrm_cell_load_def_t *p_load_def)
 *  \param p_cell_ctxt 
 *  \param p_load_def
 *  \brief Function for handling cell load for action ECN BI DIR.
 */
rrm_return_et
rrm_cell_load_action_ecn_bi_dir_hndl
(
 rrm_cell_context_t *p_cell_ctxt,
 rrm_cell_load_def_t *p_load_def,
 U8 ld_choice,
 U8 resrc_cnt
 );

/*! \fn rrm_return_et rrm_cell_load_action_ssac_bar_hndl
 *  \param p_cell_ctxt
 *  \param p_load_def
 *  \brief Function for handling cell load for action SSAC barring.
 */
rrm_return_et
rrm_cell_load_action_ssac_bar_hndl
(
 rrm_cell_context_t *p_cell_ctxt,
 rrm_cell_load_def_t *p_load_def,
 U8 ld_choice,
 U8 resrc_cnt
 );
/*! \fn rrm_cell_load_action_rel_ue_carr_redir_hndl 
 *  \param p_cell_ctxt
 *  \param p_load_def
 *  \brief Function for handling cell load for action UE release with carrier redirect.
 */
rrm_return_et
rrm_cell_load_action_rel_ue_carr_redir_hndl
(
 rrm_cell_context_t *p_cell_ctxt,
 rrm_cell_load_def_t *p_load_def,
 U8 ld_choice,
 U8 resrc_cnt
 );

/* Carrier_Aggregation_Start */
/*! \fn rrm_cell_load_action_scell_deact_hndl 
 *  \param p_cell_ctxt
 *  \param p_load_def
 *  \brief Function for handling scell deactivation.
 */
rrm_return_et
rrm_cell_load_action_scell_deact_hndl
(
    rrm_cell_context_t *p_cell_ctxt,
    rrm_cell_load_def_t *p_load_def,
    U8 ld_choice,
    U8 resrc_cnt
);
/* Carrier_Aggregation_End */

/* eICIC_PHASE_1_2_CHANGES_START */
rrm_return_et
rrm_cell_load_abs_pattern_send_to_l2
(
    rrm_cell_context_t *p_cell_ctxt,
    rrm_cell_load_def_t *p_load_def
    /*SPR 14182 start*/
    //code deleted
    /*SPR 14182 end*/
);
/* eICIC_PHASE_1_2_CHANGES_END */
void rrm_fill_dl_comp_avl_cap_val (rrm_cell_context_t *p_cell_ctx);
void rrm_fill_ul_comp_avl_cap_val (rrm_cell_context_t *p_cell_ctx);

rrm_return_et rrm_reset_load_actn
(   rrm_cell_context_t *p_cell_ctxt,
    rrm_cell_load_action_t *p_ld_actn,
    rrm_load_type ld_lvl);

rrm_return_et rrm_get_load_percent
(   
 rrm_cell_load_action_t  *p_ld_actn,
 rrm_load_type           ld_lvl,
 /*SPR 17777 +-*/
 U8                      *p_ld_percnt);

rrm_return_et
rrm_ue_unset_ecn_bit (rrm_cell_index_t cell_index); /* SPR 20652 Fix +- */

S8
rrm_cellm_num_ue_to_apply_ecn(rrm_cell_index_t cell_index);

rrm_return_et
rrm_send_unset_aqm_ecn_bit(rrm_cell_context_t *p_cell_ctx);/*SPR 17777 +-*/

void
rrm_cellm_update_thp_of_ue (rrm_cell_index_t cell_index,
        rrm_ue_index_t ue_index,
        U64 max_dl_thp,
        U64 max_ul_thp);

rrm_return_et 
rrm_process_load_action (rrm_cell_context_t *p_cell_ctxt);
/* MLB End */
rrm_bool_et
rrm_cellm_get_ho_failure_report_attr_status
(
 rrm_cell_index_t cell_index
 );
rrm_bool_et
rrm_cellm_get_ho_attemt_ind_attr_status
(
 rrm_cell_index_t cell_index
 );


/* SPS related changes start */
/*! \fn  rrm_return_et rrm_set_n1_pucch_an_res_for_sps(rrm_cell_context_t  *p_cell_ctx)
 *  \brief This function sets n1 pucch an resource for SPS
 *  \param p_cell_ctx cell context
 */
rrm_return_et
rrm_set_n1_pucch_an_res_for_sps
(
 rrm_cell_context_t  *p_cell_ctx
 );
/*! \fn rrm_return_et rrm_allocate_mem_n1_pucch_an_sps_status(rrm_cell_context_t  *p_cell_ctx)
 *  \brief This function allocates the memory to keep the status of n1_pucch_an for sps
 *  \param p_cell_ctx cell context
 */
rrm_return_et
rrm_allocate_mem_n1_pucch_an_sps_status
(
 rrm_cell_context_t  *p_cell_ctx
 );

/*! \fn rrm_return_et rrm_reallocate_mem_n1_pucch_an_sps_status(rrm_cell_context_t  *p_cell_ctx)
 *  \brief This function reallocates the memory to keep the status of n1_pucch_an for sps
 *  \param p_cell_ctx cell context
 */
rrm_return_et
rrm_reallocate_mem_n1_pucch_an_sps_status
(
 rrm_cell_context_t  *p_cell_ctx
 );

/*! \fn    set_sps_on_off_for_cell_config(rrmcm_rmif_cell_config_req_t *p_cell_config_req, rrm_cell_context_t *p_cell_ctx)
 *  \brief This function sets the SPS feature ON/OFF for a Cell on the basis of SPS_CRNTI_RANGE
 *  \param p_cell_config_req
 *  \param p_cell_ctx
 */
rrm_return_et
set_sps_on_off_for_cell_config(
        rrmcm_rmif_cell_config_req_t    *p_cell_config_req,
        rrm_cell_context_t              *p_cell_ctx
        );

/*! \fn    set_sps_on_off_for_cell_reconfig(rrmcm_rmif_cell_config_req_t *p_reconfig_req, rrm_cell_context_t *p_cell_ctx)
 *  \brief This function sets the SPS feature ON/OFF for a Cell on Cell_reconfig
 *  \param p_reconfig_req
 *  \param p_cell_ctx
 */
rrm_return_et
set_sps_on_off_for_cell_reconfig(
        rrmcm_rmif_cell_reconfig_req_t  *p_reconfig_req,
        rrm_cell_context_t              *p_cell_ctx
        );

/* SPS related changes end */
U8 rrm_cellm_get_num_ue_on_ecn(rrm_cell_index_t cell_index);
void rrm_cellm_increment_ecn_ue(rrm_cell_index_t cell_index);
/*SRP 20856 +-*/
rrm_return_et
rrm_sort_ue_for_ho_send_meas_config(
        rrm_cell_index_t cell_index,
        rrm_cell_load_def_t *p_load_def
        );

/* SPR 10869 start */
void
rrm_cell_cal_need_for_ecn(rrm_cell_context_t *p_cell_ctx,
        rrm_cell_load_def_t *p_load_def,
        U8 dir);
/* SPR 10869 end */

rrm_return_et
rrm_cell_load_action_ue_rel_with_redirect_hndl
(
 rrm_cell_context_t *p_cell_ctxt,
 rrm_cell_load_def_t *p_load_def
 );

U16 rrm_inter_freq_count(rrm_cell_index_t cell_index);
rrm_void_t rrm_update_ue_actn(rrm_ue_index_t ue_index, rrm_cell_index_t cell_index); 
/*! \fn         rrm_return_et
 *              rrm_ue_get_low_priority_ue(rrm_ue_context_t *ue_context) 
 *  \brief      will provide the lowest priority ue to be released
 *  \param[out]  p_ue_index
 */
rrm_return_et
rrm_ue_get_low_priority_ue(
/*spr_22158_changes_start*/
	rrm_cell_index_t cell_index,
/*spr_22158_changes_end*/
        /*Bug_721_changes_start*/
        rrm_ue_index_t *p_ue_index, /* Outgoing  UE index*/
        rrm_ue_index_t ue_index,
        U8      current_ue_priority,/*In case of ARP, UE getting released should be of lower priority than for current UE priority*/
        rrm_bool_et    is_ue_check_reqd /*This checks if UE getting selected needs to be verified with ue_index being provided, so that both are not same (for arp)*/
        /*Bug_721_changes_end*/
        );

/*! \fn rrm_return_et rmcm_ignore_timer_event(rrm_cell_context_t *p_cell_ctx)
 *  \brief handle cell timer event.
 *  \param *p_cell_ctx
 */
rrm_return_et
rmcm_ignore_timer_event
(
 rrm_cell_context_t *p_cell_ctx  /*cell context*/
 );

rrm_return_et 
rrm_cell_ue_threshold_rchd_ind(
        rrm_cell_context_t* p_rrm_cell_ctx);

rrm_return_et
rmcm_rrc_ignored_event
(
 rrm_cell_context_t *p_cell_ctx
 );

#define MAX_BETWEEN_TWO(a, b) ((a) > (b) ? (a) : (b))

/* Start: Enh 317 */
/*! \fn meas_cdma_node_t * rrmcm_find_cdma_meas_context(U16 arfcn,
  rrm_son_cdma_band_class_et band_class,
  rrm_cell_context_t *p_cell_ctx
 * \brief function for finding cdma measurement context
 * \param arfcn
 * \param band_class
 * \param p_cell_ctx cell context
 */

meas_cdma_node_t *
rrmcm_find_cdma_meas_context
(
 U16 arfcn,
 rrm_son_cdma_band_class_et band_class,
 rrm_cell_context_t *p_cell_ctx
 );


/* End: Enh 317 */
U8 rrm_cellm_subband_cqi_param (rrm_cell_index_t cell_index);
U8 rrm_cellm_subband_periodic_reporting(rrm_cell_index_t cell_index);
/*SPR-695 Fix Starts*/
/*spr_22158_changes_start*/
/*code deleted*/
/*spr_22158_changes_end*/
/*SPR-695 Fix Ends*/
rrm_return_et
rrm_validate_ncl_cell_for_scell_selection(inter_freq_cells_t *p_inter_freq,
        U8 cell_index, rrm_bool_et *p_scell_deactivated_flag, rrm_cell_index_t *p_cell_index_of_scell);
rrm_void_t
rrm_update_scell_list_and_scell_cap_list(U8 *p_scell_index,
        rrm_cell_index_t *p_scell_list,
        rrm_cell_index_t *p_scell_cap_list,
        inter_freq_cells_t *p_inter_freq,
        rrm_cell_index_t *p_cell_index_of_scell);
/* BUG_12416_FIX_START */
rrm_return_et
rmcm_rmif_broadcast_cell_del_req(rrm_cell_context_t *p_cell_ctx);
/* BUG_12416_FIX_END */

/*SPR_15022_Start*/

/*! \fn U8 get_dl_res_block_from_dl_bandwidth(rrm_cell_context_t *cell_context)
 *  \brief This function send the res block dl bandwidth
 *  \param cell_context Cell Context
 */
U8
get_dl_res_block_from_dl_bandwidth
(
 rrm_cell_context_t *p_cell_context
);


/*! \fn U8 get_ul_res_block_from_ul_bandwidth(rrm_cell_context_t *cell_context)
 *  \brief This function send res block ul bandwidth
 *  \param cell_context Cell Context
 */
U8
get_ul_res_block_from_ul_bandwidth
(
 rrm_cell_context_t *p_cell_context
);

/*SPR_15022_End*/

/*SPR_12535_FIX_END*/

/*SPR 10329:start*/
rrm_gu_group_id_node_t *
find_gu_group_id_in_ncl_list
(
 rrm_gu_group_id_list_t *p_gu_group_id_list,
 rrm_gu_group_id_t  *p_gu_group_id

 );

/*SPR 10329:end*/

/* SPR_13117_FIX_START */
/*! \fn void rrm_get_cellm_log_level
 *  \brief get the log_level and log_level_on_off.
 *  \param *log_level_enable
 *  \param *log_level_for_uem
 */

void rrm_get_cellm_log_level(U8 *log_level_enable,
        U8 *log_level_for_uem);
/* SPR_13117_FIX_END */
/* + SPR 17665 +*/
rrm_return_et 
validation_mac_reconfig_req(rrmcm_rmif_cell_reconfig_req_t  *p_reconfig_req ,
rrm_cell_context_t *p_cell_ctx);

/* - SPR 17665 -*/

#ifdef LTE_EMBMS_SUPPORTED
rrm_return_et
rrmcm_process_mbms_sf_info_req
(
    rrm_cell_context_t *p_cell_ctx        /* cell context */
);

rrm_return_et
rmcm_rrc_reconfig_resp_for_mbms
(
    rrm_cell_context_t *p_cell_ctx
);

rrm_return_et
build_send_rrc_reconfig_req_for_mbms
(
 rrm_cell_context_t          *p_cell_context, 
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req,
 U8                          *p_si_index_offset
);

rrm_return_et
rrmcm_process_mbms_sf_info_req
(
    rrm_cell_context_t *p_cell_ctx        /* cell context */
);

rrm_return_et
rmcm_rrc_reconfig_resp_for_mbms
(
    rrm_cell_context_t *p_cell_ctx
    );
rrm_void_t
rrm_process_link_up_down_nrt_for_mbms
(
 rrm_cell_context_t *p_cell_ctxt
 );

rrm_void_t
rrm_store_neigh_cell_config_for_inter_cell
(
 U8                  *p_neigh_cell_config,
 U8                  inter_freq_cells,
 inter_freq_params_t *p_inter_freq_params
 );

rrm_return_et
rrm_process_mtch_info_with_broadcasted_info
(
 rrm_cell_context_t  *p_cell_ctx,
 S32                  flag,
 rrm_bool_t           updated_ncl_received_from_son,
 U8                  *new_neigh_cell_config
 );

rrm_bool_et
rrm_validate_inter_intra_mbsfn
(
 lte_ncl_t *p_ncl_info
 );




#endif
#ifdef ENDC_ENABLED
rrm_void_t
rrm_cellm_update_qci_thp_of_ue
(
    rrm_cell_index_t cell_index,
    rrm_ue_index_t ue_index,
    U8  qci,
    U16 stats_idx,
    rrm_qci_data_in_stats_t *p_rrm_qci_data_in_stats,
    U64 pdcpSduBitUl,
    U64 pdcpSduBitDl
);

rrm_void_t
rrm_update_qci_pos_in_stats
(
    rrm_ue_index_t ue_index,
    U8 qci,
    rrm_qci_data_in_stats_t *p_rrm_qci_data_in_stats,
    U16 *p_stats_idx
);

#endif
#endif /* _RRM_CELL_CONTEXT_H_ */

