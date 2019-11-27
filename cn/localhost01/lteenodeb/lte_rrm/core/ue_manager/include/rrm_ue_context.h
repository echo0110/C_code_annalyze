/*! \file rrm_ue_context.h 
 *  \brief This file contains the the UE context information.
 *         and contains the information about procedural data.
 *  \date January 30, 2012
 *  \author gur11059
 */
/*! 
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _RRM_UE_CONTEXT_H_
#define _RRM_UE_CONTEXT_H_

/*! \headerfile rrm_defines.h <>
 */

#include "rrm_defines.h"
/*! \headerfile rrm_son_intf.h <>
 */
#include "rrm_son_intf.h"

/*! \headerfile rrm_cell_context.h <>
 */
#include "rrm_cell_context.h"

/*! \headerfile rrm_oam_defines.h <>
 */
#include "rrm_oam_defines.h"
#include "rrm_uem_rrmim_intf.h"


/*! \def   p_rrm_uem_facility_name
 *  \brief  Pointer variable storing the UEM name for logging
 */
extern S8 *p_g_rrm_uem_facility_name;

/*! \def g_uem_log_on_off
 *  \brief  Global variable storing the status (Enabled/Disabled) of
 *          UEM logging
 */
extern rrm_bool_et g_uem_log_on_off;

/*! \def meas_report_info_list_t
 *  \brief  list to store the meas report info
 */
typedef YLIST meas_report_info_list_t;

/*! \def RRM_MAX_NUM_SRB_PER_UE
 *  \brief 
 */
#define RRM_MAX_NUM_SRB_PER_UE   	2
/*! \def RRM_MAX_NUM_DRB_PER_UE
 *  \brief 
 */
#define RRM_MAX_NUM_DRB_PER_UE   	8
/*! \def RRM_UE_MAX_ERB_COUNT
 *  \brief
 */
#define RRM_UE_MAX_ERB_COUNT                8
/*! \def RRM_UE_MME_OCTET_SIZE
 *  \brief
 */
#define RRM_UE_MME_OCTET_SIZE                      1
/*! \def RRM_UE_M_TMSI_OCTET_SIZE
 *  \brief
 */
#define RRM_UE_M_TMSI_OCTET_SIZE                   4
/*! \def RRM_UE_STMSI_RANDOM_VALUE_OCTET_SIZE
 *  \brief
 */
#define RRM_UE_STMSI_RANDOM_VALUE_OCTET_SIZE       5
/*! \def RRM_UEM_S_TMSI_SIZE
 *  \brief
 */
#define RRM_UEM_S_TMSI_SIZE         RRM_UE_MME_OCTET_SIZE + RRM_UE_M_TMSI_OCTET_SIZE +2
/*! \def RRM_UEM_MAX_LAC_NUMOCTS
 *  \brief
 */
#define RRM_UEM_MAX_LAC_NUMOCTS       2
/*! \def RRM_UEM_MAX_PLMN_ID_NUMOCTS
 *  \brief
 */
#define RRM_UEM_MAX_PLMN_ID_NUMOCTS   3
/*! \def RRM_CODEBOOK_SUBSET_VAL
 *  \brief
 */
#define RRM_CODEBOOK_SUBSET_VAL		63

/* UE MEAS CHANGES :STARTS */

/*! \def REPORT_CONFIG_ID
 *  \brief
 */
#define REPORT_CONFIG_ID            1
/* eICIC_PHASE_1_2_CHANGES_START */
#define MAX_AGGRESSOR_COUNT         3
#define EUTRAN_MEAS_CELL_CNT        6  
/* eICIC_PHASE_1_2_CHANGES_END */
/* SPR 7514 Fix Start - report config 924 */
#define REPORT_CONFIG_INTERRAT_ID   2
#define REPORT_CONFIG_INTERRAT_UTRAN_CDMA_ID   3
/* SPR 12351 Start */
#define REPORT_CONFIG_EUTRA_CGI_ID                  4
#define REPORT_CONFIG_INTERRAT_GERAN_CGI_ID         5
#define REPORT_CONFIG_INTERRAT_UTRAN_CDMA_CGI_ID    6
#define NUM_REPORT_CONFIG          REPORT_CONFIG_INTERRAT_UTRAN_CDMA_CGI_ID 
/* SPR 12351 End */

/* SPR_16053_Fix: Start */
#define RADIO_CONNECTION_WITH_UE_LOST    21
/* SPR_16053_Fix: End */

/* SPR 7514 Fix End - report config 924 */

/*! \def RRM_MEAS_OBJ_ID_OFFSET
 *  \brief
 */
#define RRM_MEAS_OBJ_ID_OFFSET	    1  
/*! \def FGI_BIT_17
 *  \brief
 */
#define FGI_BIT_5                   0x8000000
#define FGI_BIT_17                  0x8000
#define FGI_BIT_18                  0x4000
#define FGI_BIT_19                  0x2000
/* SIT_CHANGES_START */
/* eICIC_PHASE_1_2_CHANGES_START */
#define FGI_BIT_115                  0x0020000
/* eICIC_PHASE_1_2_CHANGES_START */
/* SIT_CHANGES_END */
#define RRM_MEAS_GAP_1              70
#define RRM_MEAS_GAP_TYPE_0         0
#define RRM_MEAS_GAP_TYPE_1         1


/* ENDC_MEAS_CHANGES_START */
#ifdef ENDC_ENABLED

#define RRM_MEAS_GAP_TYPE_4         4
#define RRM_MEAS_GAP_TYPE_5         5
#define RRM_MEAS_GAP_TYPE_6         6
#define RRM_MEAS_GAP_TYPE_7         7
#define RRM_MEAS_GAP_TYPE_8         8
#define RRM_MEAS_GAP_TYPE_9         9
#define RRM_MEAS_GAP_TYPE_10        10
#define RRM_MEAS_GAP_TYPE_11        11


#define MAX_USABLE_GP4_OFFSET       7
#define MAX_USABLE_GP5_OFFSET       54

#endif
/* ENDC_MEAS_CHANGES_END */

/* Fix for CSR: 00059124 */
#define UE_SUPPORT_INTRAFREQ_RAT         0x001
#define UE_SUPPORT_INTERFREQ_RAT         0x002
#define UE_SUPPORT_UTRAN_RAT             0x004
#define UE_SUPPORT_GERAN_RAT             0x008
#define UE_SUPPORT_HRPD_RAT              0x010
#define UE_SUPPORT_1XRTT                 0x020
/* Fix for CSR: 00059124 */
/*Bug 497 Fix Start */
#define MAX_USABLE_GP0_OFFSET       14
#define MAX_USABLE_GP1_OFFSET       27
/*Bug 497 Fix End */

/*! \def RRM_MAX_ERAB_PRIORITY
 *  \brief
 */
#define RRM_MAX_ERAB_PRIORITY       30

/*! \def RRM_MAX_UE_PRIORITY
 *  \brief
 */
#define RRM_MAX_UE_PRIORITY         30

/*! \def QCI_GBR_NGBR_CLASSIFICATION
 *  \brief
 */
#define RRM_GBR_QCI_1                    1             
#define RRM_GBR_QCI_2                    2
#define RRM_GBR_QCI_3                    3
#define RRM_GBR_QCI_4                    4            /* Highest value of QCI for GBR ERAB */
#define RRM_NGBR_QCI_5                   5            
#define RRM_NGBR_QCI_6                   6
#define RRM_NGBR_QCI_7                   7
#define RRM_NGBR_QCI_8                   8
#define RRM_NGBR_QCI_9                   9            /*  Highest value of QCI for Non-GBR ERAB */
#define RRM_MAX_QCI_LIMIT                10           /* QCI value not to exceed 9: Valid Range for Non-GBR ERAB (>4 AND <10) */

/*! \def RRM_ERB_REQ_TYPE_SETUP
 *  \brief
 */
#define RRM_ERB_REQ_TYPE_SETUP           0

/*! \def RRM_ERB_REQ_TYPE_MODIFY
 *  \brief
 */
#define RRM_ERB_REQ_TYPE_MODIFY           1

/*! \def RRM_1
 *  \brief
 */
#define RRM_1			    1
/*! \def RRM_ALL_NODES_PROCESSED
 *  \brief
 */
#define RRM_ALL_NODES_PROCESSED     -1
/*! \def RRM_ANR_MAX_MEAS_OBJ
 *  \brief
 */
#define RRM_ANR_MAX_MEAS_OBJ        MAX_SON_MEAS_CONFIG_OBJECT

#define SON_ANR_MEAS_OFFSET_FREQ        RRM_OAM_Q_OFFESET_RANGE_DB0

/* UE MEAS CHANGES :ENDS */

/* UE MEAS CHANGES :ENDS */
/* MC, MR & HO Starts */
/*! \def MAX_NEIGH_CELL
 *  \brief
 */
#define MAX_NEIGH_CELL 32
/*! \def INVALID_PHYSICAL_CELL_ID
 *  \brief
 */
#define INVALID_PHYSICAL_CELL_ID 0xFFFF
/*! \def UERRM_MAX_TIME_FOR_UE_IN_CELL
 *  \brief
 */
#define UERRM_MAX_TIME_FOR_UE_IN_CELL  4095
/* SPR 20652 Fix Start */
#define BITMASK_FOR_UE_ID       0x1FFF
/* SPR 20652 Fix End */

/* CSR_00040799_CHANGES_START */
/* CSR_00040799_CHANGES_END */
#define MAX_ERAB 16
#define RRM_MAX_REPORT_STAT_UE		   20

#define RRM_MAX_CELL_MEAS              32
/* SPR 15232 Start */
#define RRM_ZERO_POINT_FIVE_MULTIPLIER       0.5 
/* SPR 15232 end */


#define RRM_DELTA_SINR_COUNT_PERCENT       70
#define RRM_PERCENTAGE                     100

#define RRM_UE_MEAS_OBJECT_COUNT(x)\
    (x == RRM_EIGHT)? RRM_TRUE:RRM_FALSE

/* ROHC Context Session values */
#define RRM_UE_CS_2      2
#define RRM_UE_CS_4      4
#define RRM_UE_CS_8      8
#define RRM_UE_CS_12     12
#define RRM_UE_CS_16     16
#define RRM_UE_CS_24     24
#define RRM_UE_CS_32     32
#define RRM_UE_CS_48     48
#define RRM_UE_CS_64     64
#define RRM_UE_CS_128    128
#define RRM_UE_CS_256    256
#define RRM_UE_CS_512    512
#define RRM_UE_CS_1024   1024
#define RRM_UE_CS_16384  16383

/* ON Duration Timer PSF */
#define RRM_ON_DURATION_TIMER_PSF_1    1
#define RRM_ON_DURATION_TIMER_PSF_2    2
#define RRM_ON_DURATION_TIMER_PSF_3    3
#define RRM_ON_DURATION_TIMER_PSF_4    4
#define RRM_ON_DURATION_TIMER_PSF_5    5
#define RRM_ON_DURATION_TIMER_PSF_6    6
#define RRM_ON_DURATION_TIMER_PSF_8    8
#define RRM_ON_DURATION_TIMER_PSF_10   10
#define RRM_ON_DURATION_TIMER_PSF_20   20
#define RRM_ON_DURATION_TIMER_PSF_30   30
#define RRM_ON_DURATION_TIMER_PSF_40   40
#define RRM_ON_DURATION_TIMER_PSF_50   50
#define RRM_ON_DURATION_TIMER_PSF_60   60
#define RRM_ON_DURATION_TIMER_PSF_80   80
#define RRM_ON_DURATION_TIMER_PSF_100  100
#define RRM_ON_DURATION_TIMER_PSF_200  200

#define RRM_BYTE_BITMASK       0xFF

#define RRM_N4_TX_UE_CAT_5_CBSR_TM3_MASK         0xF0
#define RRM_N4_TX_UE_CAT_NOT_5_CBSR_TM3_MASK     0x30
#define RRM_N2_TX_UE_CAT_ANY_CBSR_TM3_MASK       0xC0
#define RRM_N4_TX_UE_CAT_NOT_5_CBSR_TM4_MASK_LAST_4_BYTES 0xFF   /* first 4 bytes to be masked with 0x00 & last 4 bytes to be masked with 0xFF of 8 byte field */
#define RRM_N4_TX_UE_CAT_NOT_5_CBSR_TM4_MASK_FIRST_4_BYTES 0x00
#define RRM_N4_TX_UE_CAT_5_CBSR_TM4_MASK   0xFF
#define RRM_N2_TX_UE_CAT_ANY_CBSR_TM4_MASK   0xFC
#define RRM_N4_TX_UE_CAT_NOT_5_CBSR_TM5_MASK    0xF0
#define RRM_N4_TX_UE_CAT_5_CBSR_TM5_MASK       0xFF
#define RRM_N2_TX_UE_CAT_ANY_CBSR_TM5_MASK    0xF0
#define RRM_N2_TX_UE_CAT_ANY_CBSR_TM8_MASK    0xFC
#define RRM_N4_TX_UE_CAT_ANY_CBSR_TM8_MASK      0xFF
#define RRM_N4_TX_UE_CAT_ANY_CBSR_TM6_MASK      0xFF
#define RRM_N2_TX_UE_CAT_ANY_CBSR_TM6_MASK      0xF0

#define N2_TX_ANTENNA_TM8_R9 0
#define N4_TX_ANTENNA_TM8_R9 1
/*  cdma2000 start */
#define RRM_MAX_MEAS_OBJECT_ID     32

/* MAX value for uplinkPathlossValue & uplinkBlerValue */
#define RRM_MAX_UPLINK_PATHLOSS_VALUE     100
#define RRM_MAX_UPLINK_BLER_VALUE         100

/* MIN value for uplinkSINRValue */
#define RRM_MIN_UPLINK_SINR_VALUE         0

/* uplink mu mimo change start*/
#define    RRM_MU_MIMO_ENABLE_CNF                    0x0001
#define    RRM_MU_MIMO_DISABLE_CNF                   0x0002
/* uplink mu mimo change end*/

/* cdma2000 end */

/* LIPA SIPTO start */
#define RRM_MAX_LOCAL_ERROR_IMPACTED_ITEMS      256
/* Macros used for various flags to be set */

/* SPR 10795 Fix Start */
#define RRM_MAX_SUPPORTED_RLC_QUEUE_SIZE 28311552
/* SPR 10795 Fix End */

#define BAND_1_SUPPORTED          0x01
#define BAND_5_SUPPORTED          0x02
#define BAND_25_SUPPORTED         0x04
#define BAND_CLASS_A_SUPPORTED    0x08
#define BAND_CLASS_C_SUPPORTED    0x10
#define SCELL_FREQ_SUPPORTED      0x20
/*! \enum rrm_ue_lipa_release_et
 *  \brief An enumeration for lipa ue/rab release
 */
/*SPR 15713 Fix Start*/
#define INVALID_FRAME 0xFF
#define INVALID_SLOT  0xFF
#define INVALID_RI_INDEX 0xFFFF
#define INVALID_RES_INDEX 0xFF
#define INVALID_SR_CFG_INDEX 0xFF
#define INVALID_CQI_CFG_INDEX 0xFFFF
/*SPR 15713 Fix End*/


/* SPR 17529 Fix Start */
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_24_DB      -24
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_22_DB      -22
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_20_DB      -20
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_18_DB      -18
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_16_DB      -16
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_14_DB      -14
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_12_DB      -12
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_10_DB      -10
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_8_DB       -8
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_6_DB       -6
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_5_DB       -5
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_4_DB       -4
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_3_DB       -3
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_2_DB       -2
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_1_DB       -1
#define RRM_EUTRAN_FREQ_OFFSET_0_DB             0
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_1_DB        1
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_2_DB        2
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_3_DB        3
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_4_DB        4
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_5_DB        5
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_6_DB        6
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_8_DB        8
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_10_DB       10
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_12_DB       12 
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_14_DB       14
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_16_DB       16
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_18_DB       18
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_20_DB       20
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_22_DB       22
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_24_DB       24
/* SPR 17529 Fix End */
typedef enum
{
    UE_LIPA_RELEASE_NONE = 0,
    UE_LIPA_UE_RELEASE,
    UE_LIPA_RAB_RELEASE
}rrm_ue_lipa_release_et;
/* LIPA SIPTO end */



/*! \enum rrm_ue_category_et
 *  \brief An enumeration for UE Category
 */
typedef enum
{
    UE_CATEGORY_NONE = 0,
    UE_CATEGORY_1,
    UE_CATEGROY_2,
    UE_CATEGORY_3,
    UE_CATEGORY_4,
    UE_CATEGORY_5,
    UE_CATEGORY_6,
    UE_CATEGORY_7,
    UE_CATEGORY_8
}rrm_ue_category_et;

/*! \enum rrm_ue_response_et
 *  \brief Enumeration for UE response
 */
typedef enum
{
    RRM_UE_INDEX_ERR = -1,
    RRM_UE_FAILURE,
    RRM_UE_SUCCESS,
    RRM_UE_PARTIAL_SUCCESS
}rrm_ue_response_et;

/*! \enum rrm_ue_establishment_cause_et
 *  \brief Enumeration for categorizing the UE establishment Cause
 */
typedef enum
{
    RRM_UE_ESTAB_CAUSE_EMERGENCY,
    RRM_UE_ESTAB_CAUSE_HIGHPRIORITY_ACCESS,
    RRM_UE_ESTAB_CAUSE_MT_ACCESS,
    RRM_UE_ESTAB_CAUSE_MO_SIGNALLING,
    RRM_UE_ESTAB_CAUSE_MO_DATA,
    /*Rel10 CR changes start*/
    RRM_UE_ESTAB_CAUSE_DELAYTOLERANTACCESS_V1020,
    /*Rel10 CR changes end*/
    /* coverity : CID 16404,16403*/
    RRM_UE_INVALID_ESTABLISHMENT_CAUSE
} rrm_ue_establishment_cause_et;

/* SRVCC SUPPORT START */
/*! \enum rrm_srvcc_ue_criteria_et
 *  \brief indicates the SRVCC criteria as per UE and MME capabilities
 */
typedef enum
{
    RRM_UE_SRVCC_IND_NOT_TO_BE_INCLUDED,
    /* SRVCC HO not required */
    RRM_UE_SRVCC_CS_PS_TYPE,
    /* Voice bearer is established, SRVCC may be included with PS + CS type */
    RRM_UE_SRVCC_OP_NOT_POSSIBLE
        /* srvcc_op_possible = false or FGI bit 8/27 not set:Target cell can't be voice-incapable */
}rrm_srvcc_ue_criteria_et;

/*! \enum rrm_srvcc_ho_type_et
 *  \brief indicates the SRVCC criteria as target cell capabilities
 *          as well as UE/MME capabilities
 */
typedef enum
{
    RRM_SRVCC_CS_PS_TYPE,
    /* Target Cell supports PS HO, SRVCC to be included with PS + CS type */
    RRM_SRVCC_CS_ONLY_TYPE,
    /* Target Cell does not support PS HO, SRVCC to be included with CS only type */
}rrm_srvcc_ho_type_et;

/* SRVCC SUPPORT END */

/*! \enum rrm_ue_access_stratum_release_et
 *  \brief
 */
typedef enum
{
    RRM_REL8,
    RRM_REL9,
    RRM_REL10,
    /* contella fix start */
    /* eMTC changes start */
    RRM_REL11,
    RRM_REL12,
    RRM_REL13,
    /* eMTC changes stop */
    /* contella fix start */
    RRM_RELSPARE2,
    RRM_RELSPARE1
}rrm_ue_access_stratum_release_et;

/* UE MEAS CHANGES :STARTS */

/*! \enum rrm_ue_meas_config_status_et
 *  \brief status of measurement configuration sent
 */
typedef enum
{
    RRM_MEAS_CONFIG_NULL,
    RRM_MEAS_CONFIG_REQ_SENT,
    RRM_MEAS_CONFIG_RESP_SUCCESS,
    RRM_MEAS_CONFIG_RESP_FAILED,
    RRM_MEAS_CONFIG_TO_DELETE
}rrm_ue_meas_config_status_et;

/*! \enum rrm_ue_cell_type_et
 *  \brief An enum for UE cell type information
 */
typedef enum
{
    RRM_EUTRAN,
    RRM_UTRAN,
    RRM_GERAN,
    RRM_CDMA2000,
    /* TDD Support SON Start */
    RRM_UTRAN_TDD,
    /* TDD Support SON End */
    RRM_MAX_CELL_TYPES
}rrm_ue_cell_type_et;

typedef enum
{
    RRM_HO_STATE_NULL,
    RRM_HO_STATE_HO_TRIGGERED,
    RRM_HO_STATE_WAIT_FOR_REL
} rrm_ue_ho_state_et;
/* Force Lock / Grace lock changes Ends*/

/*! \enum rrm_ue_release_cause_et
 *  \brief Enumeration for categorizing the UE release Cause
 */
typedef enum
{
    /*SPR_15323 start*/
    RRM_UE_REL_CAUSE_CONGESTION,
    RRM_UE_REL_CAUSE_UNDEFINED
    /*SPR_15323 end*/
} rrm_ue_release_cause_et;

/* SPR 12512 Fix Start */
#define RRM_UE_OUT_OF_SYNC_INITIAL         0x00
#define RRM_UE_IN_SYNC_AFTER_OUT_OF_SYNC   0x01
#define RRM_UE_OUT_OF_SYNC_MAX_RETRIES     0x02
typedef struct _rrm_ue_sync_staus_ind_data_t
{
        U32   ue_sync_status;
}rrm_ue_sync_staus_ind_data_t;
/* SPR 12512 Fix End */

/*LTE_RRM_TM_SWITCH_START*/
/*! \ rrm_ue_mac_report_t
 *  \brief This struct is used for UE MAC report
 *  \param report_index
 *  \param ue_stats[RRM_MAX_REPORT_STAT_UE]
 */
typedef struct _rrm_ue_mac_report_t
{
    U16               report_index;
    U16               report_count;
    /* SPR 14286 start */
    U16               eicic_report_count;
    U16               eicic_set2_report_count;
    /* SPR 14286 end */
    /* + SPR 20078 */
    PeriodicReportInfo   ue_stats[RRM_MAX_REPORT_STAT_UE];
    /* - SPR 20078 */
}rrm_ue_mac_report_t;

/*ue position start*/
typedef enum
{
    RRM_ECID_MEAS_CONTINUE,
    RRM_ECID_MEAS_DELETE,
    RRM_ECID_MEAS_DELETE_FAILED,
    RRM_ECID_MEAS_FAILED
}rrm_meas_ecid_resp_et;
typedef enum
{
    RRM_LPPA_ONDEMAND,
    RRM_LPPA_PERIODIC
}rrm_lppa_report_char_et;

typedef enum
{
    RRM_LPPA_MS120,
    RRM_LPPA_MS240,
    RRM_LPPA_MS480,
    RRM_LPPA_MS640,
    RRM_LPPA_MS1024,
    RRM_LPPA_MS2048,
    RRM_LPPA_MS5120,
    RRM_LPPA_MS10240,
    RRM_LPPA_MSMIN1,
    RRM_LPPA_MSMIN6,
    RRM_LPPA_MSMIN12,
    RRM_LPPA_MSMIN30,
    RRM_LPPA_MSMIN60
}rrm_lppa_meas_peridicity_et;

typedef enum
{
    RRM_LPPA_CELL_ID,
    RRM_LPPA_ANGLEOFARRIVAL,
    RRM_LPPA_TIMINGADVANCETYPE1,
    RRM_LPPA_TIMINGADVANCETYPE2,
    RRM_LPPA_RSRP,
    RRM_LPPA_RSRQ
}rrm_lppa_meas_quanify_item_et;

#define    RRM_MAX_ESMLC_MEAS_ID                     15
#define    RRM_ECID_MEAS_QUANTIFIERS                 63

#define    RRM_ECID_RADIO_NW_LAYER_CAUSE             1
#define    RRM_ECID_PROTOCOL_CAUSE                   2
#define    RRM_ECID_MISCELLANEOUS_CAUSE              3

typedef enum {
    RRM_PC_ABSTRACT_SYNTAX_ERROR_REJECT= RRM_ZERO,
    RRM_PC_ABSTRACT_SYNTAX_ERROR_IGNORE_AND_NOTIFY ,
    RRM_PC_ABSTRACT_SYNTAX_ERROR_FALESLY_CONSTRUCTED_MESSAGE,
    RRM_PC_MESSAGE_NOT_COMPATIBLE_WITH_RECEIVER_STATE ,
    RRM_PC_SEMANTIC_ERROR,
    RRM_PC_TRANSFER_SYNTAX_ERROR,
    RRM_PC_UNSPECIDIED
} rrm_ecid_protocol_cause;

typedef enum {
    RRM_RNL_UNSPECIFIED = RRM_ZERO,
    RRM_RNL_REQUESTED_ITEM_NOT_SUPPORTED ,
    RRM_RNL_REQUESTED_ITEM_TEMPORARILLY_NOT_AVAILABLE ,
} rrm_ecid_radio_network_layer_cause;

typedef enum {
    RRM_MISC_UNSPECIFIED = RRM_ZERO,
} rrm_ecid_miscellaneous_cause;

/* Fix for CSR: 00059124 start */
typedef enum
{
    NO_RAT_CONFIGURED,
    EUTRAN_RAT,
    UTRAN_FDD_RAT,
    UTRAN_TDD_RAT,
    GERAN_RAT,
    CDMA_RAT,
 /*spr 17848 fix start*/
 RRM_MAX_RAT
 /*spr 17848 fix stop*/
}rrm_ue_rat_et;
/* Fix for CSR: 00059124 end */

/* SPR 14326 Fix Start */
typedef enum
{
  INTER_FREQ_BAND_LIST =0,
  INTER_RAT_BAND_LIST
}rrm_meas_gap_band_type_et;

typedef enum
{
   EUTRA_FREQ = 0 ,
   UTRAN_FDD_FREQ,
   UTRAN_TDD_FREQ,
   GERAN_FREQ,
   CDMA_FREQ
}rrm_meas_gap_freq_type_et;

typedef struct _rrm_cdma_meas_gap_config_t
{
    U8 band_class;
    U8 cdma_type;
}rrm_cdma_meas_gap_config_t;
/* SPR 14326 Fix End */







typedef struct
{
    rrm_lppa_meas_quanify_item_et meas_quantity_item;
}rrm_lppa_meas_quantifier_t;

typedef struct
{
    U16 bitmask;
#define RRM_LPPA_MEAS_INIT_REQ_MEAS_PERIODICITY_PRESENT    0x01
    U8                       mme_id;           
    U8                       routing_id;         
    U32                      lppa_trans_id;      
    U32                      e_smlc_meas_id;     
    rrm_lppa_report_char_et  report_char;
    rrm_lppa_meas_peridicity_et  meas_periodicity;
    /*^ O, RRM_LPPA_MEAS_INIT_REQ_MEAS_PERIODICITY_PRESENT, H, 0, 12 ^*/
    U8             num_meas_quantifier;/*^ M, 0, B, 1, 63 ^*/
    rrm_lppa_meas_quantifier_t  meas_quantifier[MAX_NO_OF_MEAS];
}rrm_ue_lppa_ecid_meas_init_req_t;

typedef struct
{
    U16  bitmask;            /*^ BITMASK ^*/
    U8             mme_id;             /*^ M, 0, H, 0, 15^*/
    U8             routing_id;         /*^ M, 0, H, 0, 255 ^*/
    U32            lppa_trans_id;      /*^ M, 0, H, 0, 32767 ^*/
    U32            e_smlc_ue_meas_id;  /*^ M, 0, B, 1, 15 ^*/
    U32            enb_ue_meas_id;     /*^ M, 0, B, 1, 15 ^*/
}rrm_ue_lppa_ecid_meas_termination_cmd_t;

typedef struct{
    rrc_bitmask_t              bitmask; /*^ BITMASK ^*/
#define RRM_LPPA_CRIT_DIAGNO_PROCEDURE_CODE_PRESENT    0x01
#define RRM_LPPA_CRIT_DIAGNO_TRIGERRING_MSG_PRESENT    0x02
#define RRM_LPPA_CRIT_DIAGNO_PROC_CRITICALITY_PRESENT  0x04
#define RRM_LPPA_CRIT_DIAGNO_TRANS_ID_PRESENT          0x08
    U8                         procedure_code;
    /*^ O, RRM_LPPA_CRIT_DIAGNO_PROCEDURE_CODE_PRESENT, H, 0, 255 ^*/
    U32                        triggering_msg;
    /*^ O, RRM_LPPA_CRIT_DIAGNO_TRIGERRING_MSG_PRESENT, H, 0, 2 ^*//*lppa_triggering_msg_et*/
    U32                        proc_critic;
    /*^O, RRM_LPPA_CRIT_DIAGNO_PROC_CRITICALITY_PRESENT, H, 0, 2 ^*//*lppa_proc_crit_et*/
    U32                        lppa_trans_id;      
    /*^ O, RRM_LPPA_CRIT_DIAGNO_TRANS_ID_PRESENT, H, 0, 32767 ^*/
}rrm_lppa_criticality_diagno_t;

typedef struct
{
    rrc_bitmask_t  bitmask;      /*^ BITMASK ^*/
#define RRM_LPPA_ERROR_IND_UE_INDEX_PRESENT       0x01
#define RRM_LPPA_ERROR_IND_CAUSE_PRESENT          0x02
#define RRM_LPPA_ERROR_IND_CRITIC_DIAG_PRESENT    0x04
    U8             mme_id;         /*^ M, 0, H, 0, 15 ^*/
    U16            ue_index;       /*^ O, RRM_LPPA_ERROR_IND_UE_INDEX_PRESENT, N, 0, 0 ^*/
    U8             routing_id;      /*^ M, 0, H, 0, 255 ^*/
    U32            lppa_trans_id;  /*^ M, 0, H, 0, 32767 ^*/
    lppa_cause_t   cause;          /*^ O, RRM_LPPA_ERROR_IND_CAUSE_PRESENT, N, 0, 0 ^*/ 
    lppa_criticality_diagno_t  criticality_diagnostics;
    /*^ O, RRM_LPPA_ERROR_IND_CRITIC_DIAG_PRESENT, N, 0, 0 ^*/
}rrm_ue_lppa_error_ind_t;

/*ue position end */

/*csg start*/
typedef enum
{
    RRM_ENTERING,
    RRM_LEAVING
}rrm_proximity_ind_type_et;
/*! \ rrm_ProximityIndication_r9_IEs_carrierFreq_r9_t
 *  \brief This struct is used for Proximity indication r9 IEs carrier frequency information
 *  \param bitmask
 *  \param carrier_freq_eutra
 *  \param carrier_freq_utra
 */
typedef struct _rrm_ProximityIndication_r9_IEs_carrierFreq_r9_t
{
    U16              bitmask;        /*^ BITMASK ^*/

#define RRM_UE_PROXIMITY_ARFCN_VALUE_EUTRA_PRESENT   0x01
#define RRM_UE_PROXIMITY_ARFCN_VALUE_UTRA_PRESENT    0x02

    U32                       carrier_freq_eutra;
    /*^ O, RRM_UE_PROXIMITY_ARFCN_VALUE_EUTRA_PRESENT ^*/
    U16                       carrier_freq_utra;
    /*^ O, RRM_UE_PROXIMITY_ARFCN_VALUE_UTRA_PRESENT ^*/
}rrm_ProximityIndication_r9_IEs_carrierFreq_r9_t;
/*! \ rrm_proximity_ind_t
 *  \brief This struct is used for Proximity indication
 *  \param bitmask
 *  \param proximity_ind_type
 *  \param carrier_Freq
 */
typedef struct _rrm_proximity_ind_t
{
    U16                  bitmask;        /*^ BITMASK ^*/
    U8                             proximity_ind_type;
    /*^ M, 0, H, 0, 1 ^*/ /* rrm_proximity_ind_type_et */
    rrm_ProximityIndication_r9_IEs_carrierFreq_r9_t   carrier_Freq;
}rrm_proximity_ind_t;
/*csg end*/
/*!
 * \ rrm_ue_phy_antenna_information_t
 * \brief Structure for antenna configurations of ue
 */
typedef struct _rrm_ue_reconfig_response_t
{
    U16       bitmask;
#define RRM_UE_RECONFIG_RESP_CAUSE_PRESENT       0x01
    rrm_cell_index_t    cell_index;
    U16                 ue_index;    
    U8                  response;   /*!< Success/Failure */
    U8                  fail_cause;      /*!< cause */	
}rrm_ue_reconfig_response_t;

typedef rrc_phy_antenna_information_dedicated_t rrm_ue_phy_antenna_information_t;
typedef rrc_antenna_info_dedicated_r10_t        rrm_ue_phy_antenna_info_r10_t;
typedef rrc_phy_cqi_reporting_t			rrm_ue_phy_cqi_reporting_t;
typedef rrc_cqi_report_config_r10_t             rrm_ue_cqi_report_config_r10_t;
typedef rrc_phy_antenna_info_v920_t             rrm_ue_phy_antenna_info_v920_t;
/* SPR 10450 Fix Start */
typedef struct _rrm_ue_applied_drx_config_t
{
    U32         applied_drx_profile_index;
    U8          applied_drx_offset_val;
    rrm_oam_long_drx_cycle_start_offset_et  
                            applied_long_drx_cycle; 
    #ifdef ENDC_ENABLED
    rrm_oam_long_drx_cycle_start_offset_r15_et  
                            applied_long_drx_cycle_r15; 
    #endif
}rrm_ue_applied_drx_config_t;

typedef struct _rrm_ue_applied_drx_config_data_t
{
    rrm_ue_applied_drx_config_t curr_applied_drx_config; 
    U8         num_of_released_erab;
    U8         released_erab_info[RRM_MAX_NUM_DRB_PER_UE];
}rrm_ue_applied_drx_config_data_t;
/* SPR 10450 Fix End */
/*SPR 15663 start*/
typedef struct _rrm_ue_temp_sr_config_data_t
{
 U8    sr_params_bitmask;
#define RRM_UE_RECONFIG_SR_PARAMS_PRESENT 0X001   
 U8    sr_res_row;
 U16   sr_res_col;
 U8    sr_pucch_res_index;
 U8    sr_configuration_index;
}rrm_ue_temp_sr_config_data_t;
/*SPR 15663 end*/
/*!
 * \ rrm_ue_phy_config_information_t
 * \brief Structure for physical configurations of ue
 */
typedef struct _rrm_ue_phy_config_information_t
{

    U16                                 ue_reconfig_rollback_bitmask;/*!< To specify the ue reconfig nitmask for rollback>*/
#define RRM_UE_RECONFIG_PHY_ANTENNA_CONFIG_ROLLBACK_PRESENT           0x001
#define RRM_UE_RECONFIG_PHY_CQI_REPORTING_ROLLBACK_PRESENT            0x002
#define RRM_UE_RECONFIG_PHY_TPC_PDCCH_CONFIG_PUSCH_ROLLBACK_PRESENT   0x004
#define RRM_UE_RECONFIG_PHY_TPC_PDCCH_CONFIG_PUCCH_ROLLBACK_PRESENT   0x008
#define RRM_UE_RECONFIG_PHY_UPLINK_POWER_CONTROL_ROLLBACK_PRESENT     0x010
#define RRM_UE_RECONFIG_PHY_SOUNDING_RS_UL_CONFIG_ROLLBACK_PRESENT    0x020
#define RRM_UE_RECONFIG_PHY_ANTENNA_INFO_V920_PRESENT                 0x040
#define RRM_UE_RECONFIG_PHY_ANTENNA_INFO_R10_ROLLBACK_PRESENT         0x080
#define RRM_UE_RECONFIG_PHY_CQI_REPORTING_R10_ROLLBACK_PRESENT        0x100
/* SPR 10450 Fix Start */
#define RRM_UE_RECONFIG_PHY_DRX_CONFIG_ROLLBACK_PRESENT               0x200
/* SPR 10450 Fix End */
    rrm_ue_phy_antenna_information_t	ue_antenna_information;
    rrm_ue_phy_antenna_information_t    ue_backup_antenna_information;
    rrm_ue_phy_antenna_info_r10_t       ue_antenna_info_r10;
    rrm_ue_phy_antenna_info_r10_t       ue_backup_antenna_info_r10;
    rrm_ue_phy_cqi_reporting_t		ue_cqi_present_configured_info;
    rrm_ue_phy_cqi_reporting_t		ue_backup_cqi_configured_info;
    rrm_ue_cqi_report_config_r10_t      ue_cqi_present_configured_info_r10;
    rrm_ue_cqi_report_config_r10_t      ue_backup_cqi_configured_info_r10;
    rrm_ue_phy_antenna_info_v920_t      ue_antenna_info_v920;
    rrm_ue_phy_antenna_info_v920_t      ue_backup_antenna_info_v920;
    /* SPR 10450 Fix Start */
    rrm_ue_applied_drx_config_t         ue_backup_applied_drx_config; 
    /* SPR 10450 Fix End */
}rrm_ue_phy_config_information_t;

/*LTE_RRM_TM_SWITCH_END*/

/*!
 * \ rrm_ue_meas_obj_fill_info_t 
 * \brief Structure for Measuement object fill information
 * \param obj_to_be_filled_cnt No. of objects to be filled for current UE
 * \param obj_filled_cnt No. of objects already filled
 */
typedef struct _rrm_ue_meas_obj_fill_info_t
{
    U8	obj_to_be_filled_cnt; /*!< No. of objects to be filled for current UE */
    S8	obj_filled_cnt;           /*!< No. of objects already filled */
}rrm_ue_meas_obj_fill_info_t;


/* spr_12273_fix_start*/ 
/*!
 * \ rrm_meas_obj_cell_info_t 
 * \brief Structure for Measuement object cellid and pci information
 * \param phy_cell_id phy cell id of the cell
 * \param cell_index cell index 
 */
typedef struct _rrm_meas_obj_cell_to_add_info_t
{
   /* SPR:12849 Start */ 
    YLNODE node;
    /* SPR:12849 End */ 
    U16     phy_cell_id;
    U8      cell_index;
}rrm_meas_obj_cell_to_add_info_t;
/* spr_12273_fix_end*/ 


/*! \  meas_report_info_t
 *  \brief Structure storing the meas report info
 *  \param meas_id meas_id measurement id of the corresponding measurement results object
 *  \param meas_object_id mesurement object id of the corresponding meas object
 *  \param report_config_id report config id of the report config object
 *  \param trigger_quantity configured trigger quantity of the event
 *  \param reporting_event configured reporting event
 *  \param thresh1 configured threshold value of the event
 *  \param thresh2 configured threshold value of the event
 *  \param meas_config_status
 */
typedef struct meas_report_info_t
{
    U8                           meas_id;   /*!<  Measurement id */ 
    U8                           meas_object_id; /*!< meas_object_id */
    U8                           report_config_id;/*!< report_config_id  */
    rrmc_trigger_quantity_et     trigger_quantity; /*!< trigger_quantity (RSRP/RSRQ)   */
    rrmc_meas_report_event_et    reporting_event;
    /*!< EVENT_NULL, EVENT_A1, A2, A3, A4, A5, B1, B2 */
    U8                           thresh1; /*!<  Threshold for A1,A2 & thresh1 for A5 Calculation */
    U8                           thresh2; /*!<  Threshold2 for A5 Calculation */
    rrm_ue_meas_config_status_et meas_config_status;/*!< config status (SUCCESS/FAILURE) */
    /*!< NULL/Meas Req Sent/Resp Rcvd*/
}meas_report_info_t; 

/*! \ rrm_ue_ho_drb_sub_to_fwd_info_t
 *  \brief Structure for storing the HO DRB forward list
 *  \param forward_direction
 *  \param erab_id
 */
typedef struct _rrm_ue_ho_drb_sub_to_fwd_info_t
{
    U8 forward_direction; /*!<  forward_direction        */
    U8 erab_id;  /*!<  erab_id        */
}rrm_ue_ho_drb_sub_to_fwd_info_t;

/*! \  rrm_ue_ho_erab_info_list_t
 *  \brief Stucture for storing the UE erab handover List
 *  \param erab_count
 *  \param drb_sub_to_fwd[MAX_ERAB]
 */
typedef struct _rrm_ue_ho_erab_info_list_t
{
    U8         erab_count;/*!<  erab_count        */
    rrm_ue_ho_drb_sub_to_fwd_info_t        drb_sub_to_fwd[MAX_ERAB]; /*!<  drb_sub_to_fwd        */
}rrm_ue_ho_erab_info_list_t;

/*! \  rrm_ue_ho_nas_security_param_t
 *  \brief Sructure storing the UE handover NAS security params
 *  \param nas_msg_length
 *  \param nas_security_param[2]
 */
typedef struct _rrm_ue_ho_nas_security_param_t
{
    U8            nas_msg_length;/*!<  nas_msg_length        */
    U8            nas_security_param[2]; /*!<  nas_security_param        */
}rrm_ue_ho_nas_security_param_t;

/*! \ rrm_ue_ho_erab_to_release_t
 *  \brief Structure for Handover erab to release
 *  \param erab_id
 *  \param rrm_cause
 */
typedef struct _rrm_ue_ho_erab_to_release_t
{
    U16            erab_id; /*!<  erab_id        */
    rrm_cause_t    rrm_cause;/*!<  rrm_cause        */
}rrm_ue_ho_erab_to_release_t;

/*! \  rrm_ue_ho_erab_to_release_list_t
 *  \brief List of Handover erab to releas
 *  \param erab_count
 *  \param erab_to_release[MAX_ERAB]
 */
typedef struct _rrm_ue_ho_erab_to_release_list_t
{
    U8    erab_count;/*!<  erab_count       */ 
    rrm_ue_ho_erab_to_release_t   erab_to_release[MAX_ERAB]; /*!<  erab_to_release        */
}rrm_ue_ho_erab_to_release_list_t;

/* SPR_15259_fix: start */
typedef enum
{
    RRM_ACQUSITION_NONE,
    RRM_ACQUSITION_INTRA_FREQ,
    RRM_ACQUSITION_INTER_FREQ,
    RRM_ACQUSITION_UTRA_FDD,
    RRM_ACQUSITION_UTRA_TDD
}rrm_si_acqusition_et;
/* SPR_15259_fix: end */
typedef enum
{
    RRM_CDMA2000_HO_SUCCESS,
    RRM_CDMA2000_HO_FAILURE,
    RRM_CDMA2000_HO_NO_REPORT
}rrm_ue_cdma2000_ho_status_et;

typedef enum
{
    RRM_CDMA2000_1XRTT = 0,
    RRM_CDMA2000_HRPD,
    RRM_MAX_CDMA2000_NETWORK_TYPES
}rrm_ue_cdma2000_rat_type_et;

/*! \ rrm_ue_proc_ho_command_request_t
 *  \brief Struct for storing the handover command request
 *  \param bitmask
 *  \param ue_index
 *  \param nas_security_param
 *  \param erab_sub_to_fwd_list
 *  \param erab_to_release_list
 */
typedef struct _rrm_ue_proc_ho_command_request_t
{
    U16                 bitmask; /*!<bitmask                */
#define RRC_RRM_UE_HO_CMD_REQ_NAS_SECURITY_PARAM_PRESENT         0x01
#define RRC_RRM_UE_HO_CMD_REQ_ERAB_SUB_TO_FORWARD_LIST_PRESENT   0x02
#define RRC_RRM_UE_HO_CMD_REQ_ERAB_TO_RELEASE_LIST_PRESENT       0x04
    U16                     ue_index;/*!<ue_index               */
    rrm_ue_ho_nas_security_param_t    nas_security_param;/*!< nas_security_param            */
    rrm_ue_ho_erab_info_list_t    erab_sub_to_fwd_list;/*!< erab_sub_to_fwd_list               */
    rrm_ue_ho_erab_to_release_list_t  erab_to_release_list;/*!< erab_to_release_list               */
    U8                      cdma2000_ho_status[RRM_MAX_CDMA2000_NETWORK_TYPES];
    /* rrm_ue_cdma2000_ho_status_et */
}rrm_ue_proc_ho_command_request_t;

/*! \ rrm_ue_proc_ho_failure_t
 *  \brief Structure for Hand Over Failure
 *  \param ue_index
 *  \param rrm_cause
 */
typedef struct _rrm_ue_proc_ho_failure_t
{
    U16            ue_index;/*!<ue_index               */
    rrm_cause_t    rrm_cause;/*!<  rrm_cause        */
}rrm_ue_proc_ho_failure_t;


/* MRO changes start */
/*! \ rrm_ue_proc_rlf_ind_t
 *  \brief Structure for Hand Over RLF ind
 *  \param bitmask
 *  \param enb1_gb_id
 *  \param enb2_gb_id
 *  \param failure_cell_pci
 *  \param re_est_cell_ecgi
 *  \param crnti
 *  \param rlf_report
 *  \param ue_index
 *  \param ue_context_released
 */
#define RRM_X2AP_RLF_IND_RLF_REPORT_PRESENT          0x0001
#define RRM_X2AP_RLF_IND_UE_INDEX_PRESENT            0x0002
#define RRM_X2AP_RLF_IND_UE_CONTEXT_RELEASED_PRESENT 0x0004
typedef struct _rrm_ue_proc_rlf_ind_t
{
    U16                bitmask; 
    rrm_x2_gb_enb_id_t enb1_gb_id; 
    rrm_x2_gb_enb_id_t enb2_gb_id;
    U16                failure_cell_pci; 
    rrm_x2ap_ecgi_t    re_est_cell_ecgi;
    U16                crnti;
    rrc_rlf_report_t   rlf_report;          /*^ O,RRM_X2AP_RLF_IND_RLF_REPORT_PRESENT,N,0,0 ^*/
    U16                ue_index;            /*^ O,RRM_X2AP_RLF_IND_UE_INDEX_PRESENT,N,0,0 ^*/
    U8                 ue_context_released; /*^ O,RRM_X2AP_RLF_IND_UE_CONTEXT_RELEASED_PRESENT,H,0,1 ^*/
}rrm_ue_proc_rlf_ind_t; 
/* MRO changes end */


/*! \ rrm_ue_rrm_rstdInterFreqInfoList_r10_t 
  s  \brief Struct rrm_rstdInterFreqInfoList_r10_t 
 */
typedef struct _rrm_ue_rrm_rstdInterFreqInfoList_r10_t
{
    U32                   carrier_freq;
    U8                    meas_prs_offset;
}rrm_ue_rrm_rstdInterFreqInfoList_r10_t;

/*! \ rrm_ue_inter_freq_meas_ind_t 
 *  \brief Struct for inter freq meas ind request 
 */
#define RRM_UE_INTER_FREQ_RSTD_START                      0x01
#define RRM_UE_INTER_FREQ_RSTD_STOP                       0x02
#define RRM_UE_INTER_FREQ_RSTD_INFO_LIST_PRESENT          0x04
typedef struct _rrm_ue_inter_freq_meas_ind_t
{
    U32           bitmask;
    U16   		 ue_index;
    U8			 num_rstd_intfreq_list;
    rrm_ue_rrm_rstdInterFreqInfoList_r10_t  rstd_interfreq_list[MAXRSTD_FREQ];
}rrm_ue_inter_freq_meas_ind_t;


/*! \  rrm_ue_proc_ho_cancel_resp_t
 *  \brief Struct for Hand Over cancel response
 *  \param ue_index
 *  \param response 
 */
typedef struct _rrm_ue_proc_ho_cancel_resp_t
{
    U16            ue_index;/*!<ue_index               */
    U8             response;/*!< Success/Failure */
}rrm_ue_proc_ho_cancel_resp_t;

/* SPR:15970 Fix Start - 6/8/15 */
#define  MEAS_EVENT_NONE_PRESENT    0x00
#define  MEAS_EVENT_A3_PRESENT      0x01
#define  MEAS_EVENT_A4_PRESENT      0x02
#define  MEAS_EVENT_A5_PRESENT      0x04
/* SPR:15970 Fix End - 6/8/15 */


/* SPR 15232 start */
/*! \ rrm_rpt_cell_info_t
 *  \brief structure storing the reported neighbour cell information 
 *  \param phy_cell_id 
 *  \param is_pci_confusion_detected 
 *  \param rsrx_value 
 *  \param priority 
 *  \param csg_membership_status 
 *  \param csg_membership_status_presence 
 *  \param csg_id_presence 
 *  \param cell_rank 
 */
typedef struct _rrm_rpt_cell_info_t
{
    U16                             phy_cell_id;            /*!< This indicate the physical cell Id */ 
    /* SPR 15674 Start */
    U32                             target_earfcn;          /*!< this indicate the earfcn of target cell */
    /* SPR 15674 End */
    U8                              reason_event_type;      /*!< Store bitwise information of event types. Valid values are MEAS_EVENT_A3_PRESENT,
                                                 MEAS_EVENT_A4_PRESENT,MEAS_EVENT_A5_PRESENT, MEAS_EVENT_NONE_PRESENT */
    /* SPR:15970 Fix End - 6/8/15 */
    /* SPR 16406 Start */
    /* Code Removed */
    /* SPR 16406 End */
    U8                              rsrx_value;             /*!< This is the RSRP or RSRQ value corresponding to physical cell id */
    U8                              priority;               /*!< This is the priority to the frequncy to physical cell id */
    /*csg start*/
    /* SPR 18008 Fix Start */ 
    rrm_bool_t                     csg_membership_status_presence; 
                                                            /*this the csg membership status presence flag for the physical cell id */
    rrm_bool_t                     csg_id_presence;        /*this the csg membership status presence flag for the physical cell id */
    /*csg end*/
    /*start tgt cell ranking*/
    /* SPR 17529 Fix Start */
    S8 cell_rank;
    /* SPR 17529 Fix End */
    /*end tgt cell ranking*/
    /* SPR 16406 18sep Start */
    U8 reported_count;
    /* SPR 16406 18sep End */
    rrm_csg_membership_status_et    csg_membership_status;  /*this the csg membership status of the ue corresponding to the physical cell id */
    /* SPR 18008 Fix End */ 
}rrm_rpt_cell_info_t;
/* SPR 15232 end */

/*! \ cell_selection_priority_list_t 
 *  \brief structure storing the cell selection priority list
 *  \param count
 *  \param bitmask
 *  \param next_target_cell_idx
 *  \param phy_cell_id[MAX_NEIGH_CELL]
 *  \param rsrx_value[MAX_NEIGH_CELL]
 *  \param priority[MAX_NEIGH_CELL]
 *  \param csg_membership_status[MAX_NEIGH_CELL]
 *  \param csg_membership_status_presence[MAX_NEIGH_CELL]
 *  \param csg_id_presence[MAX_NEIGH_CELL]
 */
typedef struct _cell_selection_priority_list_t
{
#define	RRM_UE_BLIND_CELL_HO_TRIGGERED	0x01
    U16	bitmask;
    U8    count; /*!< Number of PHY cell id present in the list */
    U8    next_target_cell_idx; /*!< This will tell the index position of the next target cell to which HO should be Triggered */
  /* SPR 16406 8SEP start */
  rrm_bool_t ranking_done;
  /* SPR 16406 8SEP end */
  /* SPR 17529 Fix Start */
  rrm_rat_type_et   rat_type;
  /* SPR 17529 Fix End */
/* SPR 15232 start */
  rrm_rpt_cell_info_t reported_neighbor_cell_info[MAX_NEIGH_CELL];
/* SPR 15232 end */
} cell_selection_priority_list_t;

/*! \  struct RRM_TIME_T
 *  \brief  Time structure
 *  \param  sec
 *  \param  msec
 */

typedef struct  {
    unsigned long   sec;/*!<  Time in sec */
    unsigned short  msec;/*!<  Time in milli sec */
} RRM_TIME_T;

/*! \  struct rrm_ho_params_t
 *  \brief structure storing all  Hand Over related parameters
 *  \param blind_timer value of blind timer id
 *  \param blind_timer_expired_flag  specifies whether blind timer expired
 *  \param a3_event_data stores a3 event data
 *  \param a3_event_ts stores a3 event time stamp
 *  \param a4_event_data value of a4 event data
 *  \param a4_event_ts stores a4 event time stamp
 *  \param a5_event_data stores 53 event time stamp
 *  \param a5_event_ts stores a5 event time stamp
 *  \param last_reported_meas_event stores last reported event
 *  \param cell_selection_list value of cell list
 *  \param target_phys_cell_id value of target cell physical cell id
 *  \param target_cell_identity[4] value of target cell identity
 */
typedef struct _rrm_ho_params_t
{
#define		RRM_UE_PRIMARY_MEAS_OBJ_CONFIGURED		0x01
#define		RRM_UE_SECONDARY_MEAS_OBJ_CONFIGURED	0x02
#define 	RRM_UE_B1_MEAS_OBJ_CONFIGURED			0x04
#define		RRM_UE_CSFB_RECD      			        0x08
#define		RRM_UE_CSFB_BLIND_TIMER_EXPIRED			0x10
#define		RRM_UE_B2_MEAS_OBJ_CONFIGURED			0x20
    /* Inter RAT changes start */
    U16   					bitmask;
    /* Inter RAT changes end */
    /* SPR 18008 Fix Start */ 
    U8                                blind_timer_expired_flag; /*!<  Blind timer Expiry Flag */
    /*SPR 22571 Fix Start*/
    /*Delay timer Removed*/
    /*SPR 22571 Fix Stop*/
    rrm_timer_t                       blind_timer;              /*!<  Blind timer id*/
    /* SPR 17784 Start */
    rrm_timer_t                       cgi_timer;              /*!<  CGI timer id*/
    /* SPR 17784 End */
    /* Inter RAT changes start */
    /* CSF blind timer will be started when UE_CONTEXT_MOD_REQ is received 
     * with CSFB indicator */
    rrm_timer_t							csfb_blind_timer;
    /* Inter RAT changes end */

    rrmc_meas_report_event_et         last_reported_meas_event; /*!< Last reported Event */
/* SPR 16406 Start */
    cell_selection_priority_list_t    eutra_known_cell_selection_list;  /*!< Cell Selection list for EUTRA Known cell */
    cell_selection_priority_list_t    eutra_unknown_cell_selection_list;  /*!< Cell Selection list for EUTRA Unknown cell*/
    cell_selection_priority_list_t    eutra_confused_cell_selection_list;  /*!< Cell Selection list for EUTRA confused cell*/

    cell_selection_priority_list_t    utra_known_cell_selection_list;  /*!< Cell Selection list for EUTRA Known cell */
    cell_selection_priority_list_t    utra_unknown_cell_selection_list;  /*!< Cell Selection list for EUTRA Unknown cell*/
    cell_selection_priority_list_t    utra_confused_cell_selection_list;  /*!< Cell Selection list for EUTRA confused cell*/

    cell_selection_priority_list_t    geran_known_cell_selection_list;  /*!< Cell Selection list for EUTRA Known cell */
    cell_selection_priority_list_t    geran_unknown_cell_selection_list;  /*!< Cell Selection list for EUTRA Unknown cell*/
    cell_selection_priority_list_t    geran_confused_cell_selection_list;  /*!< Cell Selection list for EUTRA confused cell*/
/* SPR 16406 End */
    U16                               target_phys_cell_id;      /*!< Target Cell selected for HO */
    U8                                target_cell_identity[4];
    /* MRO changes start */
    rrm_bool_t                        ho_ue_attempt_ind_sent;           /*!<  HO ue attempt ind sent to son */
    /* MRO changes end */
    /* SPR 15232 start */
    /* Removed code */
    /* SPR 15232 end */
    rrm_bool_t                        osc_loop_detected;
    U8                                osc_loop_count;
    U8                                ho_retry_count;
    /* SPR 15232 start */
    rrm_bool_t                        a2_event_received;
    /* SPR 15232 end */
    /* SPR:15970 Fix Start - 6/4/15 */
    U8                                last_ho_trigger_event; /*!<  Store reason for HO triggered last time. Possible values are MEAS_REP_EVENT_A3, MEAS_REP_EVENT_A4, MEAS_REP_EVENT_A5 */
                                                             /* And set invalid value as RRM_FALSE */   
    /* SPR:15970 Fix End - 6/4/15 */
    /* SPR 15674 Start */
    U16                                target_earfcn;
    /* SPR 18008 Fix End */ 
    /* SPR 15674 End */
    /* SPR 20635 Fix - SRVCC to GERAN - Starts */
    U8                                target_geran_cell_rssi;      /*!< Target GERAN Cell RSSI received in MEas Result */
    /* SPR 20635 Fix - SRVCC to GERAN - Ends */
}rrm_ho_params_t;

/* SPR 16053 start */
/* SPR_15282_Fix_Start */
typedef enum
{
  RRM_CGI_REP_CAUSE_MAX,
  /* SPR 16406 8SEP start */
  RRM_CGI_REP_CAUSE_KNOWN_PCI,
  /* SPR 16406 8SEP end */
  RRM_CGI_REP_CAUSE_NEW_PCI,
  RRM_CGI_REP_CAUSE_PCI_CONFUSION
}rrm_cgi_trigger_cause_et;
/* SPR_15282_Fix_End */ 
/* SPR 16053 end */


typedef enum
{
  RRM_CSFB_PRESENT,
  RRM_CSFB_HIGH_PRIORITY
}rrm_ue_csfb_present_et;
/* SPR_14216_stop */
/*! \  struct rrm_cell_meas_data_t
 *  \brief Structure storing the Cell Measurement data
 *  \param meas_resultmeas_result value of meas result of the cell
 *  \param freq_specific_offset value of frequency specific offset
 *  \param cell_specific_offset value of cell specific offset
 *  \param hysteresis value of hysteresis
 *  \param thresh1 value of threshold of the received event
 *  \param thresh2 value of threhold2 of the event A5
 *  \param offset value of offset
 */
typedef struct _rrm_cell_meas_data_t
{
    U8      meas_result;              /*!< measurement result of the cell, not taking into account any offsets */
    /*Cov_fix_86439/86437/86432/86427/86425/86424*/
    rrm_q_offsetrange_et 	   freq_specific_offset; /*!< frequency specific offset */
    rrm_q_offsetrange_et      cell_specific_offset;  /*!< cell specific offset */
    /*Cov_fix_86439/86437/86432/86427/86425/86424*/
    U8	   hysteresis;                /*!< hysteresis of the cell*/
    U8      thresh1;                       /*!<threshold parameter for this event  */
    U8      thresh2;                       /*!< threshold parameter for this event  */
    S8      offset;                        /*!< offset parameter for this event */
    /* SPR 10498 Fix Start */
    U16     earfcn;                        /*!< carrier freq for which report is received */
    /* SPR 10498 Fix End */
    /* SPR 15674 Start */
    U16     carr_earfcn;                     /*!< carr_freq configured for this event */
    /* SPR 15674 End */
} rrm_cell_meas_data_t;

/*! \enum  rrm_ue_cond_entry_exit_et
 *  \brief Entry/Exit condition of the UE
 */
typedef enum 
{
    RRM_UE_COND_EXIT = 0,
    RRM_UE_COND_ENTRY,
    RRM_UE_COND_UNKNOWN
} rrm_ue_cond_entry_exit_et;

/*! \enum  rrm_ue_timer_type_et
 *  \brief Timer expiry Events for HO
 */
typedef enum /* RRC_RRM_UE_TIMER_EXPIRY_EVENT */
{
    RRM_UE_DELAY_HO_TIMER,
    RRM_UE_BLIND_HO_TIMER,
    RRM_UE_CELL_BLOCKED_W_FOR_HO_TIMER, /*! On this timer expiry UEM send HO_RESP to mif */
    RRM_UE_CELL_BLOCKED_W_FOR_A4_REPORT_TIMER, /*! Wait Timer for A4 report for all UEs */
    RRM_UE_CSF_BLIND_TIMER,
    RRM_UE_W_FOR_MLB_REDIRECT_MEAS_IND,   
    RRM_UE_ECID_MEAS_TIMER_1,         /*  E-CID meas related timers */
    RRM_UE_ECID_MEAS_TIMER_2,
    RRM_UE_ECID_MEAS_TIMER_3,
    RRM_UE_ECID_MEAS_TIMER_4,
    RRM_UE_ECID_MEAS_TIMER_5,
    RRM_UE_ECID_MEAS_TIMER_6,
    RRM_UE_ECID_MEAS_TIMER_7,
    RRM_UE_ECID_MEAS_TIMER_8,
    RRM_UE_ECID_MEAS_TIMER_9,
    RRM_UE_ECID_MEAS_TIMER_10,
    RRM_UE_ECID_MEAS_TIMER_11,
    RRM_UE_ECID_MEAS_TIMER_12,
    RRM_UE_ECID_MEAS_TIMER_13,
    RRM_UE_ECID_MEAS_TIMER_14,
    RRM_UE_ECID_MEAS_TIMER_15,
    /* RACH_OPTIMIZATION_CHANGES_START */
    RRM_UE_RACH_REPORT_TIMER,
    /* RACH_OPTIMIZATION_CHANGES_END */
    RRM_UE_CSG_GUARD_TIMER,
    /* SPR 17784 Start */
    RRM_UE_CGI_TIMER,
    /* SPR 17784 End */
    RRM_UE_MAX_TIMER_ID
} rrm_ue_timer_type_et;

/*! \ rrm_ue_timer_buf_t
 *  \brief Structure to store Timer Buffer
 *  \param ue_timer_type specifies the timer as blind/delay timer
 *  \param ue_index value of ue index
 *  \param timer_data value of timer data
 *  \param timer_data_length value of timer data length
 */
typedef struct _rrm_ue_timer_buf_t
{
    rrm_ue_timer_type_et  timer_type;   /*!< Type of timer*/
    rrm_ue_index_t           ue_index;  /*!< UE Index */
    rrm_cell_index_t           cell_index; /*!< Cell Index */
    void*                          p_timer_data; /*!< Timer Data */
    U16                            timer_data_len; /*!< Timer Data length */
    U16                     trans_id; /*!< Transaction Id */
  /* SPR 17784 Start */
  U16                   phy_cell_id;
  U16                   earfcn;
  /* SPR 17784 End */
}rrm_ue_timer_buf_t;
/* MC,MR & HO END  */ 
/* UE MEAS CHANGES :ENDS */

/*! \enum rrm_ue_max_num_rohc_context_session_et
 *  \brief UE SUPPORTED ROHC context information
 */
typedef enum
{
    RRM_UE_CS2,
    RRM_UE_CS4,
    RRM_UE_CS8,
    RRM_UE_CS12,
    RRM_UE_CS16,
    RRM_UE_CS24,
    RRM_UE_CS32,
    RRM_UE_CS48,
    RRM_UE_CS64,
    RRM_UE_CS128,
    RRM_UE_CS256,
    RRM_UE_CS512,
    RRM_UE_CS1024,
    RRM_UE_CS16384,
    RRM_UE_SPARE2,
    RRM_UE_SPARE1
}rrm_ue_max_num_rohc_context_session_et;

/*! \enum rrm_ue_supported_band_utra_fdd_et
 *  \brief UE Supported BAND UTRA FDD info
 */
typedef enum
{
    RRM_UE_UTRA_FDD_BANDI,
    RRM_UE_UTRA_FDD_BANDII,
    RRM_UE_UTRA_FDD_BANDIII,
    RRM_UE_UTRA_FDD_BANDIV,
    RRM_UE_UTRA_FDD_BANDV,
    RRM_UE_UTRA_FDD_BANDVI,
    RRM_UE_UTRA_FDD_BANDVII,
    RRM_UE_UTRA_FDD_BANDVIII,
    RRM_UE_UTRA_FDD_BANDIX,
    RRM_UE_UTRA_FDD_BANDX,
    RRM_UE_UTRA_FDD_BANDXI,
    RRM_UE_UTRA_FDD_BANDXII,
    RRM_UE_UTRA_FDD_BANDXIII,
    RRM_UE_UTRA_FDD_BANDXIV,
    RRM_UE_UTRA_FDD_BANDXV,
    RRM_UE_UTRA_FDD_BANDXVI,
    RRM_UE_UTRA_FDD_BANDXVII_8A0,
    RRM_UE_UTRA_FDD_BANDXVIII_8A0,
    RRM_UE_UTRA_FDD_BANDXIX_8A0,
    RRM_UE_UTRA_FDD_BANDXX_8A0,
    RRM_UE_UTRA_FDD_BANDXXI_8A0,
    RRM_UE_UTRA_FDD_BANDXXII_8A0,
    RRM_UE_UTRA_FDD_BANDXXIII_8A0,
    RRM_UE_UTRA_FDD_BANDXXIV_8A0,
    RRM_UE_UTRA_FDD_BANDXXV_8A0,
    RRM_UE_UTRA_FDD_BANDXXVI_8A0,
    RRM_UE_UTRA_FDD_BANDXXVII_8A0,
    RRM_UE_UTRA_FDD_BANDXXVIII_8A0,
    RRM_UE_UTRA_FDD_BANDXXIX_8A0,
    RRM_UE_UTRA_FDD_BANDXXX_8A0,
    RRM_UE_UTRA_FDD_BANDXXXI_8A0,
    RRM_UE_UTRA_FDD_BANDXXXII_8A0
}rrm_ue_supported_band_utra_fdd_et;

/*! \enum rrm_ue_supported_band_utra_tdd128_et 
 *  \brief UE Supported BAND UTRA TDD info
 */
typedef enum
{
    RRM_UE_UTRA_TDD_128_A,
    RRM_UE_UTRA_TDD_128_B,
    RRM_UE_UTRA_TDD_128_C,
    RRM_UE_UTRA_TDD_128_D,
    RRM_UE_UTRA_TDD_128_E,
    RRM_UE_UTRA_TDD_128_F,
    RRM_UE_UTRA_TDD_128_G,
    RRM_UE_UTRA_TDD_128_H,
    RRM_UE_UTRA_TDD_128_I,
    RRM_UE_UTRA_TDD_128_J,
    RRM_UE_UTRA_TDD_128_K,
    RRM_UE_UTRA_TDD_128_L,
    RRM_UE_UTRA_TDD_128_M,
    RRM_UE_UTRA_TDD_128_N,
    RRM_UE_UTRA_TDD_128_O,
    RRM_UE_UTRA_TDD_128_P
}rrm_ue_supported_band_utra_tdd128_et;

/*! \enum rrm_ue_supported_band_utra_tdd384_et 
 *  \brief UE Supported BAND UTRA TDD info
 */
typedef enum
{
    RRM_UE_UTRA_TDD_384_A,
    RRM_UE_UTRA_TDD_384_B,
    RRM_UE_UTRA_TDD_384_C,
    RRM_UE_UTRA_TDD_384_D,
    RRM_UE_UTRA_TDD_384_E,
    RRM_UE_UTRA_TDD_384_F,
    RRM_UE_UTRA_TDD_384_G,
    RRM_UE_UTRA_TDD_384_H,
    RRM_UE_UTRA_TDD_384_I,
    RRM_UE_UTRA_TDD_384_J,
    RRM_UE_UTRA_TDD_384_K,
    RRM_UE_UTRA_TDD_384_L,
    RRM_UE_UTRA_TDD_384_M,
    RRM_UE_UTRA_TDD_384_N,
    RRM_UE_UTRA_TDD_384_O,
    RRM_UE_UTRA_TDD_384_P
}rrm_ue_supported_band_utra_tdd384_et;

/*! \enum rrm_ue_supported_band_utra_tdd768_et 
 *  \brief UE Supported BAND UTRA TDD info
 */
typedef enum
{
    RRM_UE_UTRA_TDD_768_A,
    RRM_UE_UTRA_TDD_768_B,
    RRM_UE_UTRA_TDD_768_C,
    RRM_UE_UTRA_TDD_768_D,
    RRM_UE_UTRA_TDD_768_E,
    RRM_UE_UTRA_TDD_768_F,
    RRM_UE_UTRA_TDD_768_G,
    RRM_UE_UTRA_TDD_768_H,
    RRM_UE_UTRA_TDD_768_I,
    RRM_UE_UTRA_TDD_768_J,
    RRM_UE_UTRA_TDD_768_K,
    RRM_UE_UTRA_TDD_768_L,
    RRM_UE_UTRA_TDD_768_M,
    RRM_UE_UTRA_TDD_768_N,
    RRM_UE_UTRA_TDD_768_O,
    RRM_UE_UTRA_TDD_768_P
}rrm_ue_supported_band_utra_tdd768_et;


typedef enum
{
    RRM_UE_GERAN_GSM_450,
    RRM_UE_GERAN_GSM_480,
    RRM_UE_GERAN_GSM_710,
    RRM_UE_GERAN_GSM_750,
    RRM_UE_GERAN_GSM_810,
    RRM_UE_GERAN_GSM_850,
    RRM_UE_GERAN_GSM_900_P,
    RRM_UE_GERAN_GSM_900_E,
    RRM_UE_GERAN_GSM_900_R,
    RRM_UE_GERAN_GSM_1800,
    RRM_UE_GERAN_GSM_1900,
    RRM_UE_GERAN_SPARE5,
    RRM_UE_GERAN_SPARE4,
    RRM_UE_GERAN_SPARE3,
    RRM_UE_GERAN_SPARE2,
    RRM_UE_GERAN_SPARE1
}rrm_ue_supported_band_geran_et;

/*! \Structures used for UE Capability information*/

/*! \  struct rrm_ue_pdcp_rohc_profile_t
 *  \brief rohc profiles
 *  \param U8 profile0x0001
 *  \param U8 profile0x0002
 *  \param U8 profile0x0003
 *  \param U8 profile0x0004
 *  \param U8 profile0x0006
 *  \param U8 profile0x0101
 *  \param U8 profile0x0102
 *  \param U8 profile0x0103
 *  \param U8 profile0x0104
 */
typedef struct _rrm_ue_pdcp_rohc_profile_t
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
}rrm_ue_pdcp_rohc_profile_t;

/*! \  struct rrm_ue_pdcp_parameters_t
 *  \brief PDCP parameters
 *  \param rrm_ue_pdcp_rohc_profile_t rohc_profile
 *  \param rrm_ue_max_num_rohc_context_session_et max_num_rohc_context_session
 */
typedef  struct _rrm_ue_pdcp_parameters_t
{
    rrm_ue_pdcp_rohc_profile_t					rohc_profile;					/*^ M, 0, N, 0, 0 ^*/
    rrm_ue_max_num_rohc_context_session_et		max_num_rohc_context_session;   /*^ M, 0, H, 1, 15 ^*/ /* rrm_ue_max_num_rohc_context_session_et */
}rrm_ue_pdcp_parameters_t;

/*! \  struct rrm_ue_supported_band_eutra_t
 *  \brief Structure for UE supported eutra band
 *  \param U8 band_eutra
 *  \param U8 half_duplex
 */
typedef struct _rrm_ue_supported_band_eutra_t
{
    U8      band_eutra;     /*^ M, 0, B, 1, MAX_FREQ_BAND_INDICATOR ^*/
    //    U8      half_duplex;    /*^ M, 0, H, 1, 1 ^*/   /* rrm_bool_et */ //Not used in rrm_son Rel-1.1
}rrm_ue_supported_band_eutra_t;

/*! \  struct rrm_ue_supported_band_list_eutra_t
 *  \brief List of UE supported eutra bands
 *  \param U8 count
 *  \param rrm_ue_supported_band_eutra_t supported_band_eutra[MAX_BAND_EUTRA]
 */
typedef struct _rrm_ue_supported_band_list_eutra_t
{
    U8								count;									/*^ M, 0, B, 1, 64 ^*/     /* MAX_BAND_EUTRA  */
    rrm_ue_supported_band_eutra_t	supported_band_eutra[MAX_BAND_EUTRA];	/*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_ue_supported_band_list_eutra_t;

/*! \  struct rrm_ue_rf_parameters_t
 *  \brief UE radio frequency parameters
 *  \param rrm_ue_supported_band_list_eutra_t supported_band_list_eutra
 */
typedef struct _rrm_ue_rf_parameters_t
{
    rrm_ue_supported_band_list_eutra_t     supported_band_list_eutra;  /*^ M, 0, H, 1, 64 ^*/
}rrm_ue_rf_parameters_t;

/*! \  struct rrm_ue_inter_freq_band_info_t
 *  \brief Structure for UE inter frequency band information
 *  \param U8 inter_freq_need_for_gaps
 */
typedef struct _rrm_ue_inter_freq_band_info_t
{
    U8      inter_freq_need_for_gaps;   /*^ M, 0, H, 0, 1 ^*/   /* rrc_bool_et */
}rrm_ue_inter_freq_band_info_t;

/*! \  struct rrm_ue_inter_freq_band_list_t
 *  \brief List of UE inter frequency band information
 *  \param U8 count
 *  \param rrm_ue_inter_freq_band_info_t inter_freq_band_info[MAX_BAND_EUTRA]
 */
typedef struct _rrm_ue_inter_freq_band_list_t
{
    U8							count;									/*^ M, 0, B, 1, 64 ^*/     /* MAX_BAND_EUTRA  */
    rrm_ue_inter_freq_band_info_t  inter_freq_band_info[MAX_BAND_EUTRA];	/*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_ue_inter_freq_band_list_t;

/*! \  struct rrm_ue_inter_rat_band_info_t
 *  \brief Structure for inter RAT band information
 *  \param U8 inter_rat_need_for_gaps
 */
typedef struct _rrm_ue_inter_rat_band_info_t
{
    U8      inter_rat_need_for_gaps;    /*^ M, 0, H, 0, 1 ^*/   /* rrc_bool_et */
}rrm_ue_inter_rat_band_info_t;

/*! \  struct rrm_ue_inter_rat_band_list_t
 *  \brief List of inter RAT band information
 *  \param U8 count
 *  \param rrm_ue_inter_rat_band_info_t inter_rat_band_info[MAX_BAND_EUTRA]
 */
typedef struct _rrm_ue_inter_rat_band_list_t
{
    U8							count;									/*^ M, 0, B, 1, 64 ^*/     /* MAX_BAND_EUTRA  */
    rrm_ue_inter_rat_band_info_t   inter_rat_band_info[MAX_BAND_EUTRA];	/*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_ue_inter_rat_band_list_t;

/*! \  struct rrm_ue_band_info_eutra_t
 *  \brief Structure for eutra band information
 *  \param U16 bitmask
 *  \param rrm_ue_inter_freq_band_list_t inter_freq_band_list
 *  \param rrm_ue_inter_rat_band_list_t inter_rat_band_list
 */
typedef struct _rrm_ue_band_info_eutra_t
{
    U16                  bitmask;                    /*^ BITMASK ^*/
#define RRM_UE_EUTRA_INTER_RAT_BAND_LIST_PRESENT       0x01
    rrm_ue_inter_freq_band_list_t  inter_freq_band_list;
    rrm_ue_inter_rat_band_list_t   inter_rat_band_list;	/*^ O, RRM_UE_EUTRA_INTER_RAT_BAND_LIST_PRESENT,N,0,0 ^*/

}rrm_ue_band_info_eutra_t;

/*! \  struct rrm_ue_band_list_eutra_t
 *  \brief List of eutra band information
 *  \param U8 count
 *  \param rrm_ue_band_info_eutra_t band_info_eutra[MAX_BAND_EUTRA]
 */
typedef struct _rrm_ue_band_list_eutra_t
{
    U8							count;      /*^ M, 0, B, 1, 64 ^*/     /* MAX_BAND_EUTRA  */
    /* SPR 21496 Fix Start */ 
    rrm_ue_band_info_eutra_t   	*p_band_info_eutra; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    /* SPR 21496 Fix End */ 
}rrm_ue_band_list_eutra_t;

/*! \  struct rrm_ue_meas_parameters_t
 *  \brief Measurement Parameters
 *  \param rrm_ue_band_list_eutra_t band_list_eutra
 */
typedef struct _rrm_ue_meas_parameters_t
{
    rrm_ue_band_list_eutra_t       band_list_eutra;
}rrm_ue_meas_parameters_t;

/*! \  struct rrm_ue_utra_fdd_t
 *  \brief structure for UE UTRA FDD information
 *  \param U8 count
 *  \param rrm_ue_supported_band_utra_fdd_et supported_band_utra_fdd[MAX_BAND_EUTRA]
 */
typedef struct _rrm_ue_utra_fdd_t
{
    U8      								count;										/*^ M, 0, B, 1, 64 ^*/     /* MAX_BAND_EUTRA  */
    rrm_ue_supported_band_utra_fdd_et		supported_band_utra_fdd[MAX_BAND_EUTRA];	/*^ M,0,OCTET_STRING,VARIABLE ^*/
    /* rrm_supported_band_utra_fdd_et */
}rrm_ue_utra_fdd_t;

/*! \  struct rrm_ue_utra_tdd128_t
 *  \brief structure for UE UTRA FDD information
 *  \param U8 count
 *  \param rrm_ue_supported_band_utra_fdd_et supported_band_utra_tdd128[MAX_BAND_EUTRA]
 */
typedef struct _rrm_ue_utra_tdd128_t
{
    U8      								count;										/*^ M, 0, B, 1, 64 ^*/     /* MAX_BAND_EUTRA  */
    rrm_ue_supported_band_utra_tdd128_et    supported_band_utra_tdd128[MAX_BAND_EUTRA];	/*^ M,0,OCTET_STRING,VARIABLE ^*/
    /* rrm_supported_band_utra_tdd128_et */
}rrm_ue_utra_tdd128_t;

/*! \  struct rrm_ue_utra_tdd384_t
 *  \brief structure for UE UTRA TDD information
 *  \param U8 count
 *  \param rrm_ue_supported_band_utra_tdd384_et supported_band_utra_tdd384[MAX_BAND_EUTRA]
 */
typedef struct _rrm_ue_utra_tdd384_t
{
    U8      								count;										/*^ M, 0, B, 1, 64 ^*/     /* MAX_BAND_EUTRA  */
    rrm_ue_supported_band_utra_tdd384_et    supported_band_utra_tdd384[MAX_BAND_EUTRA];	/*^ M,0,OCTET_STRING,VARIABLE ^*/
    /* rrm_supported_band_utra_tdd384_et */
}rrm_ue_utra_tdd384_t;

/*! \  struct rrm_ue_utra_tdd768_t
 *  \brief structure for UE UTRA TDD information
 *  \param U8 count
 *  \param rrm_ue_supported_band_utra_tdd768_et supported_band_utra_tdd768[MAX_BAND_EUTRA]
 */
typedef struct _rrm_ue_utra_tdd768_t
{
    U8      								count;										/*^ M, 0, B, 1, 64 ^*/     /* MAX_BAND_EUTRA  */
    rrm_ue_supported_band_utra_tdd768_et    supported_band_utra_tdd768[MAX_BAND_EUTRA];	/*^ M,0,OCTET_STRING,VARIABLE ^*/
    /* rrm_supported_band_utra_tdd768_et */
}rrm_ue_utra_tdd768_t;



typedef struct _rrm_ue_geran_t
{
    U8                          count;      /*^ M, 0, B, 1, 64 ^*/     /* MAX_BAND_EUTRA  */
    rrm_ue_supported_band_geran_et  supported_band_geran[MAX_BAND_EUTRA];
    /*^ M,0,OCTET_STRING,VARIABLE ^*/   /* supported_band_geran_et */    
}rrm_ue_geran_t;


/**********Start CDMA2000_UE_CAPABILITY_INFO*******************************************/

typedef struct _rrm_ue_supported_band_list_hrpd_t
{
    U8      count;      /*^ M, 0, B, 1, 32 ^*/     /* MAX_CDMA_BAND_CLASS  */
    U8      band_class_cdma2000[MAX_CDMA_BAND_CLASS];
    /*^ M,0,OCTET_STRING,VARIABLE ^*/   /* rrm_bandclass_cdma2000_et */
}rrm_ue_supported_band_list_hrpd_t;

typedef enum _rrm_ue_tx_rx_config_hrpd_1xrtt
{
    rrm_ue_tx_rx_single,
    rrm_ue_tx_rx_dual
}rrm_ue_tx_rx_config_hrpd_1xrtt;

typedef struct _rrm_ue_cdma2000_hrpd_t
{
    rrm_ue_supported_band_list_hrpd_t      supported_band_list_hrpd;
    U8          tx_confiig_hrpd;    /*^ M, 0, H, 0, 16 ^*/   /* rrm_ue_tx_rx_config_hrpd_1xrtt */
    U8          rx_confiig_hrpd;    /*^ M, 0, H, 0, 16 ^*/   /* rrm_ue_tx_rx_config_hrpd_1xrtt */
}rrm_ue_cdma2000_hrpd_t;

typedef struct _rrm_ue_supported_band_list_1xrtt_t
{
    U8               count;     /*^ M, 0, B, 1, 32 ^*/     /* MAX_CDMA_BAND_CLASS  */
    U8  band_class_cdma2000[MAX_CDMA_BAND_CLASS];
    /*^ M,0,OCTET_STRING,VARIABLE ^*/   /* rrm_bandclass_cdma2000_et */
}rrm_ue_supported_band_list_1xrtt_t;

typedef struct _rrm_ue_cdma2000_1x_rtt_t
{
    rrm_ue_supported_band_list_1xrtt_t     supported_band_list_1xrtt;
    U8          tx_confiig_1xrtt;   /*^ M, 0, H, 0, 16 ^*/   /* rrm_ue_tx_rx_config_hrpd_1xrtt */
    U8          rx_confiig_1xrtt;   /*^ M, 0, H, 0, 16 ^*/   /* rrm_ue_tx_rx_config_hrpd_1xrtt */
}rrm_ue_cdma2000_1x_rtt_t;

/**********End CDMA2000_UE_CAPABILITY_INFO*******************************************/
/*! \  struct rrm_ue_inter_rat_Parameters_t
 *  \brief Structure for storing Inter RAT Parameters
 *  \param U16 bitmask
 *  \param rrm_ue_utra_fdd_t utra_fdd
 *  \param rrm_ue_utra_tdd128_t utra_tdd128
 *  \param rrm_ue_utra_tdd384_t utra_tdd384
 *  \param rrm_ue_utra_tdd768_t utra_tdd768
 */
typedef struct _rrm_ue_inter_rat_Parameters_t
{
    U16  				bitmask;						/*^ BITMASK ^*/
#define RRM_INTER_RAT_UTRA_FDD_PRESENT	     				0x01
#define RRM_INTER_RAT_UTRA_TDD_128_PRESENT					0x02
#define RRM_INTER_RAT_UTRA_TDD_384_PRESENT					0x04
#define RRM_INTER_RAT_UTRA_TDD_768_PRESENT					0x08
#define RRM_INTER_RAT_GERAN_PRESENT					0x10
#define RRM_INTER_RAT_CDMA2000_HRPD_PRESENT                 0x20
#define RRM_INTER_RAT_CDMA2000_1XRTT_PRESENT                0x40 
    rrm_ue_utra_fdd_t				utra_fdd;						/*^ O, RRM_INTER_RAT_UTRA_FDD_PRESENT,N,0,0 ^*/
    rrm_ue_utra_tdd128_t			utra_tdd128;						/*^ O, RRM_INTER_RAT_UTRA_TDD_128_PRESENT,N,0,0 ^*/
    rrm_ue_utra_tdd384_t			utra_tdd384;						/*^ O, RRM_INTER_RAT_UTRA_TDD_384_PRESENT,N,0,0 ^*/
    rrm_ue_utra_tdd768_t			utra_tdd768;						/*^ O, RRM_INTER_RAT_UTRA_TDD_768_PRESENT,N,0,0 ^*/
    rrm_ue_geran_t			        geran;						/*^ O, RRM_INTER_RAT_GERAN_PRESENT,N,0,0 ^*/
    rrm_ue_cdma2000_hrpd_t          cdma2000_hrpd;                  /*^ O, RRM_INTER_RAT_UTRA_CDMA2000_HRPD_PRESENT,N,0,0 ^*/
    rrm_ue_cdma2000_1x_rtt_t        cdma2000_1x_rtt;                /*^ O, RRM_INTER_RAT_UTRA_CDMA2000_1XRTT_PRESENT,N,0,0 ^*/
}rrm_ue_inter_rat_Parameters_t;

/*! \  struct rrm_ue_eutra_radio_capability_info_def_t
 *  \brief Parameters for EUTRA radio Capability Info
 *  \param U16  bitmask
 *  \param rrm_ue_access_stratum_release_et access_stratum_release
 *  \param U8ue_category
 *  \param rrm_ue_pdcp_parameters_t pdcp_parameters
 *  \param rrm_ue_rf_parameters_t rf_parameters
 *  \param rrm_ue_meas_parameters_t meas_parameters
 *  \param U32 feature_group_indicators
 *  \param rrm_ue_inter_rat_Parameters_t inter_rat_parameters
 */
typedef struct _rrm_ue_eutra_radio_capability_info_def_t
{
    U16     bitmask; /*^ BITMASK ^*/
#define RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT 0x01

    rrm_ue_access_stratum_release_et	access_stratum_release;
    U8                          		ue_category;
    rrm_ue_pdcp_parameters_t			pdcp_parameters;
    rrm_ue_rf_parameters_t         		rf_parameters;
    rrm_ue_meas_parameters_t       		meas_parameters;
    /* Bit 3, 7, 20 - As an input to the radio bearer control algorithm and when selecting radio parameters to the admitted bearers.
       Bit 14, 15, 22, 25 - For measurement configuration */
    U32									feature_group_indicators; /*^ O, RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT, N, 0, 0 ^*/
    rrm_ue_inter_rat_Parameters_t  		inter_rat_parameters;

}rrm_ue_eutra_radio_capability_info_def_t;

/*! \  struct rrm_ue_eutra_radio_capability_info_t
 *  \brief Parameters for EUTRA radio Capability Info
 *  \param rrm_intra_freq_proximity_indication_supported_r9
 *  \param rrm_inter_freq_proximity_indication_supported_r9
 *  \param rrm_utran_proximity_indication_supported_r9
 */
/*csg start*/ 
typedef struct _rrm_csg_proximity_indication_parameters_r9_t
{
    U8 rrm_intra_freq_proximity_indication_supported_r9;
    /*^ M, 0, H, 1, 1 ^*/ /* rrc_bool_et */

    U8 rrm_inter_freq_proximity_indication_supported_r9;
    /*^ M, 0, H, 1, 1 ^*/ /* rrc_bool_et */

    U8 rrm_utran_proximity_indication_supported_r9;
    /*^ M, 0, H, 1, 1 ^*/ /* rrc_bool_et */
}rrm_csg_proximity_indication_parameters_r9_t;
/*! \ rrm_ue_eutra_capability_v920_ies_t
 *  \brief This struct is used for UE EUTRAN capability v920 ies information
 *  \param  csg_proximity_indication_parameters_r9 
 */
typedef struct _rrm_ue_irat_parameters_cdma2000_v920_t
{
    U16                   bitmask;                    /*^ BITMASK ^*/
#define RRM_UE_IRAT_PARAMS_CDMA_V920_E_CSFB_CONC_PS_MOB_1XRTT_R9_PRESENT                 0x01
    rrm_oam_eCSFB_1xrtt_r9_et     eCSFB_1xrtt_r9;/*^ M, 0, N, 0, 0 ^*/

    rrm_oam_eCSFB_conc_ps_mobility_1xrtt_r9_et    eCSFB_conc_ps_mobility_1xrtt_r9;
    /*^ O, RRM_UE_IRAT_PARAMS_CDMA_V920_E_CSFB_CONC_PS_MOB_1XRTT_R9_PRESENT, N, 0, 0 ^*/
}rrm_ue_irat_parameters_cdma2000_v920_t;

/*Rel10 CR changes start*/
/* CR 597 changes */
typedef enum
{
    RRM_ECSFB_1XRTT_DUAL_R10_SUPPORTED
}rrm_eCSFB_1xrtt_r10_et;
typedef struct
{
    U8                   eCSFB_dual_1xrtt_r10;/*^ M, 0, H, 0, 0 ^*/
    /*rrm_eCSFB_1xrtt_r10_et*/
}rrm_irat_parameters_cdma2000_v1020_t;
/* CR 597 changes */
/*CR 543 changes */
typedef enum
{
    rrm_bw_a,
    rrm_bw_b,
    rrm_bw_c,
    rrm_bw_d,
    rrm_bw_e,
    rrm_bw_f
}rrm_ca_band_width_class_r10_et;

typedef enum
{
    RRM_UL_TWO_LAYERS,
    RRM_UL_FOUR_LAYERS
}rrm_mimo_capability_ul_r10_et;

typedef enum
{
    RRM_DL_TWO_LAYERS,
    RRM_DL_FOUR_LAYERS,
    RRM_DL_EIGHT_LAYERS
}rrm_mimo_capability_dl_r10_et;

typedef struct _rrm_ca_mimo_parameters_ul_r10_t
{
    U8                              bitmask;    /*^ BITMASK ^*/
#define RRM_SUPPORTED_MIMO_CAPABILITY_UL_R10_IE_PRESENT 0x01
    rrm_ca_band_width_class_r10_et       ca_band_width_class_ul_r10;
    /*^ M, 0, N, 0, 0 ^*/    
    rrm_mimo_capability_ul_r10_et        supported_mimo_capability_ul_r10;
    /*^ O, RRM_SUPPORTED_MIMO_CAPABILITY_UL_R10_IE_PRESENT, N, 0, 0 ^*/
}rrm_ca_mimo_parameters_ul_r10_t;

typedef struct _rrm_band_parameters_ul_r10_t
{
    U8                              count;      /*^ M, 0, B, 1, 16 ^*/   /* MAX_BANDWIDTH_CLASS_R10 */

    rrm_ca_mimo_parameters_ul_r10_t     ca_mimo_parameters_ul_r10[MAX_BANDWIDTH_CLASS_R10];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/    
}rrm_band_parameters_ul_r10_t;

typedef struct _rrm_ca_mimo_parameters_dl_r10_t
{
    U8                              bitmask;    /*^ BITMASK ^*/
#define RRM_SUPPORTED_MIMO_CAPABILITY_DL_R10_IE_PRESENT 0x01
    rrm_ca_band_width_class_r10_et       ca_band_width_class_dl_r10;
    /*^ M, 0, N, 0, 0 ^*/
    rrm_mimo_capability_dl_r10_et        supported_mimo_capability_dl_r10;
    /*^ O, RRM_SUPPORTED_MIMO_CAPABILITY_DL_R10_IE_PRESENT, N, 0, 0 ^*/
}rrm_ca_mimo_parameters_dl_r10_t;

typedef struct _rrm_band_parameters_dl_r10_t
{
    U8                              count;      /*^ M, 0, B, 1, 16 ^*/   /* MAX_BANDWIDTH_CLASS_R10 */
    rrm_ca_mimo_parameters_dl_r10_t     ca_mimo_parameters_dl_r10[MAX_BANDWIDTH_CLASS_R10];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/    
}rrm_band_parameters_dl_r10_t;

typedef struct _rrm_band_parameters_r10_t
{
    U8                          bitmask;        /*^ BITMASK ^*/
#define RRM_BAND_PARAMETERS_UL_R10_IES_PRESENT      0x01
#define RRM_BAND_PARAMETERS_DL_R10_IES_PRESENT      0x02
    U8                          band_eutra_r10;     /*^ M, 0, B, 1, MAX_FREQ_BAND_INDICATOR ^*/
    rrm_band_parameters_ul_r10_t    band_parameters_ul_r10; 
    /*^ O, RRM_BAND_PARAMETERS_UL_R10_IES_PRESENT ^*/
    rrm_band_parameters_dl_r10_t    band_parameters_dl_r10;
    /*^ O, RRM_BAND_PARAMETERS_DL_R10_IES_PRESENT ^*/
}rrm_band_parameters_r10_t;

typedef struct _rrm_band_combination_parameters_r10_t
{
    U8              count;  /*^ M, 0, B, 1, 64 ^*/   
    /* SPR 21496 Fix Start */ 
    rrm_band_parameters_r10_t       *p_band_parameters_info;
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/    
    /* SPR 21496 Fix End */ 
}rrm_band_combination_parameters_r10_t;

typedef struct _rrm_supported_band_combination_r10_ies_t
{
    U8              count;     /*^ M, 0, B, 1, 128 ^*/   /* MAX_BAND_COMB_R10 */ 
    /* SPR 21496 Fix Start */ 
    rrm_band_combination_parameters_r10_t       *p_band_combination_parameters_r10; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/    
    /* SPR 21496 Fix End */ 
}rrm_supported_band_combination_r10_ies_t;

typedef struct _rrm_rf_parameters_v1020_ies_t
{
    rrm_supported_band_combination_r10_ies_t    supported_band_combination_r10;
}rrm_rf_parameters_v1020_ies_t;

typedef struct _rrm_band_combination_list_eutra_r10_ies_t
{
    U8                  count;     /*^ M, 0, B, 1, 128 ^*/   /* MAX_BAND_COMB_R10 */ 
    /* SPR 21496 Fix Start */ 
    rrm_ue_band_info_eutra_t   *p_band_info_eutra; /*^ M, 0, OCTET_STRING, VARIABLE ^*/   
    /* SPR 21496 Fix End */ 
}rrm_band_combination_list_eutra_r10_ies_t;

typedef struct _rrm_measparameters_v1020_ies_t
{
    rrm_band_combination_list_eutra_r10_ies_t band_combination_list_eutra_r10;
}rrm_measparameters_v1020_ies_t;
/*CR 543 changes */
/*CR 530,543 changes */
/*CR 710 changes */
typedef enum
{
    RRM_TWO_ANTENNA_SUPPORTED
}rrm_two_antenna_ports_for_pucch_r10_et;
typedef enum
{
    RRM_TM9_SUPPORTED
}rrm_tm9_with_8tx_fdd_r10_et;
typedef enum
{
    RRM_PMI_SUPPORTED
}rrm_pmi_disabling_r10_et;
typedef enum
{
    RRM_CROSS_CARRIER_SUPPORTED
}rrm_cross_carrier_scheduling_r10_et;
typedef enum
{
    RRM_SIMULTANEOUS_SUPPORTED
}rrm_simultaneous_pucch_pusch_r10_et;
typedef enum
{
    RRM_MULTI_CLUSTER_SUPPORTED
}rrm_multi_cluster_pusch_withincc_r10_et;
typedef enum
{
    RRM_NONCONTIGUOUSUL_SUPPORTED
}rrm_noncontiguousul_ra_withincc_info_r10_et;
typedef struct
{
    U16                   bitmask;                    /*^ BITMASK ^*/
#define RRM_NONCONTIGUOUSUL_RA_WITHINCC_INFO_R10    0x01
    rrm_noncontiguousul_ra_withincc_info_r10_et             noncontiguousul_ra_withincc_info_r10;
    /*^ O, RRM_NONCONTIGUOUSUL_RA_WITHINCC_INFO_R10 , N, 0, 0 ^*/
}rrm_noncontiguousul_ra_withincc_r10_ies_t;
typedef struct _rrm_noncontiguousul_ra_withincc_list_r10_ies_t
{
    U8                  count;     /*^ M, 0, B, 1, 64 ^*/   /* MAX_BAND_EUTRA */ 
    rrm_noncontiguousul_ra_withincc_r10_ies_t       noncontiguousul_ra_withincc_r10[MAX_BAND_EUTRA];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_noncontiguousul_ra_withincc_list_r10_ies_t;
typedef struct _rrm_phylayer_parameters_v1020_ies_t
{
    U16                   bitmask;                    /*^ BITMASK ^*/
#define RRM_TWO_ANTENNA_PORTS_FOR_PUCCH_R10_PRESENT                    0x01
#define RRM_TM9_WITH_8TX_FDD_R10_PRESENT                               0x02
#define RRM_PMI_DISABLING_R10_PRESENT                                  0x04
#define RRM_CROSS_CARRIER_SCHEDULING_R10_PRESENT                       0x08
#define RRM_SIMULTANEOUS_PUCCH_PUSCH_R10_PRESENT                       0x10
#define RRM_MULTI_CLUSTER_PUSCH_WITHINCC_R10_PRESENT                   0x20
#define RRM_MULTI_NONCONTIGUOUSUL_RA_WITHINCC_LIST_R10_PRESENT         0x40
    rrm_two_antenna_ports_for_pucch_r10_et              two_antenna_ports_for_pucch_r10;             
    /*^ O, RRM_TWO_ANTENNA_PORTS_FOR_PUCCH_R10_PRESENT , N, 0, 0 ^*/
    rrm_tm9_with_8tx_fdd_r10_et                         tm9_with_8tx_fdd_r10; 
    /*^ O, RRM_TM9_WITH_8TX_FDD_R10_PRESENT , N, 0, 0 ^*/
    rrm_pmi_disabling_r10_et                            pmi_disabling_r10;
    /*^ O, RRM_PMI_DISABLING_R10_PRESENT , N, 0, 0 ^*/
    rrm_cross_carrier_scheduling_r10_et                 cross_carrier_scheduling_r10;
    /*^ O, RRM_CROSS_CARRIER_SCHEDULING_R10_PRESENT , N, 0, 0 ^*/
    rrm_simultaneous_pucch_pusch_r10_et                 simultaneous_pucch_pusch_r10;
    /*^ O, RRM_SIMULTANEOUS_PUCCH_PUSCH_R10_PRESENT , N, 0, 0 ^*/
    rrm_multi_cluster_pusch_withincc_r10_et             multi_cluster_pusch_withincc_r10;
    /*^ O, RRM_MULTI_CLUSTER_PUSCH_WITHINCC_R10_PRESENT , N, 0, 0 ^*/
    rrm_noncontiguousul_ra_withincc_list_r10_ies_t      noncontiguousul_ra_withincc_list_r10;
    /*^ O, RRM_MULTI_NONCONTIGUOUSUL_RA_WITHINCC_LIST_R10_PRESENT , N, 0, 0 ^*/
}rrm_phylayer_parameters_v1020_ies_t;
/*CR 710 changes */
/* CR 624 changes */
typedef enum
{
    RRM_LOGGED_MEAS_IDLE_R10_TRUE
}rrm_logged_measurements_idle_r10_et;

typedef enum
{
    RRM_STANDALONE_GNSS_LOCATION_R10_TRUE
}rrm_standalone_gnss_location_r10_et;

typedef struct _rrm_ue_based_nw_perf_meas_params_t
{
    U16                   bitmask;                    /*^ BITMASK ^*/
#define RRM_LOGGED_MEAS_IDLE_UE_EUTRA_CAPABILITY_R10_PRESENT            0x01
#define RRM_STANDALONE_GNSS_LOC_UE_EUTRA_CAPABILITY_R10_PRESENT         0x02
    U8                  logged_measurements_idle_r10;
    /*^ O,RRM_LOGGED_MEAS_IDLE_UE_EUTRA_CAPABILITY_R10_PRESENT, H, 0, 0 ^*/
    /* rrm_logged_measurements_idle_r10_et */
    U8                  standalone_gnss_location_r10;
    /*^ O,RRM_STANDALONE_GNSS_LOC_UE_EUTRA_CAPABILITY_R10_PRESENT, H, 0, 0 ^*/
    /* rrm_standalone_gnss_location_r10_et */
}rrm_ue_based_nw_perf_meas_params_t;
/* CR 624 changes */
typedef enum
{
    RRM_E_REDIRECTION_UTRA_TDD_R10_SUPPORTED
}rrm_irat_parameters_utra_tdd_v1020_e_redirection_utra_tdd_r10_et;

typedef struct _rrm_inter_rat_parameters_utra_tdd_v1060_t
{
    U8    e_redirection_utra_tdd_r10;
    /* rrm_irat_parameters_utra_tdd_v1020_e_redirection_utra_tdd_r10_et */
}rrm_inter_rat_parameters_utra_tdd_v1060_t;

typedef struct _rrm_tdd_add_ue_eutra_capability_v1060_t
{
#define RRM_UE_EUTRA_CAPABILITIES_v1060_IRAT_PARAMS_UTRA_TDD_PRESENT    0x01
    U16                 bitmask;
    rrm_inter_rat_parameters_utra_tdd_v1060_t   inter_rat_parameters_utra_tdd_v1060;
    /*^ O,RRM_UE_EUTRA_CAPABILITIES_v1060_IRAT_PARAMS_UTRA_TDD_PRESENT, H, 0, 0 ^*/
}rrm_tdd_add_ue_eutra_capability_v1060_t;

/* SPR 13383 changes start */
typedef struct _rrm_supported_band_width_combination_set_r10_t
{
    U8      count;
#define MAX_BAND_WIDTH_COMB_R10 4
    U8      data[MAX_BAND_WIDTH_COMB_R10];
}rrm_supported_band_width_combination_set_r10_t;

typedef struct _rrm_band_combination_parameters_ext_r10_t
{
#define RRM_SUPPORTED_BAND_WIDTH_COMB_SET_R10 0x01
    U16                                               bitmask;
    rrm_supported_band_width_combination_set_r10_t    supported_band_width_combination_set_r10;

}rrm_band_combination_parameters_ext_r10_t;

typedef struct _rrm_supported_band_combination_ext_r10_ies_t
{
    U8                                            count;
    /* SPR 21496 Fix Start */ 
    rrm_band_combination_parameters_ext_r10_t     *p_band_combination_parameters_ext_r10;
    /* SPR 21496 Fix End */ 
}rrm_supported_band_combination_ext_r10_ies_t;

typedef struct _rrm_rf_parameters_v1060_ies_t
{
    rrm_supported_band_combination_ext_r10_ies_t    supported_band_combination_ext_r10;
}rrm_rf_parameters_v1060_ies_t;

typedef struct _rrm_ue_eutra_capability_v1060_ies_t
{
    U16			bitmask;
#define RRM_TDD_ADD_UE_EUTRA_CAPABILITIES_v1060_PRESENT     0x02    
#define RRM_UE_EUTRA_CAPABILITY_V1060_RF_PARAMETERS_PRESENT 0x04
   rrm_tdd_add_ue_eutra_capability_v1060_t        tdd_add_ue_eutra_capability_v1060;  
   rrm_rf_parameters_v1060_ies_t                  rf_parameters_v1060;
                                                  /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1060_RF_PARAMETERS_PRESENT , N, 0, 0 ^*/
}rrm_ue_eutra_capability_v1060_ies_t;
/* SPR 13383 changes end */

/* SPR_11104_FIX_START */
typedef struct 
{
    U8  e_redirection_utra_tdd_r10;/*^ M, 0, H, 0, 0 ^*/
    /*rrm_irat_parameters_utra_tdd_v1020_e_redirection_utra_tdd_r10_et */

}rrm_irat_parameters_utra_tdd_v1020_t;
/* SPR_11104_FIX_END */





#ifdef ENDC_ENABLED
/* MENB_CHANGES_START */
#define RRM_UE_EUTRA_CAPABILITY_EN_DC_R15_PRESENT    0x01
typedef struct _RrmUeEutraCapabilityIratParametersNrR15_t
{
    rrm_bitmask_t   bitmask;
    /*^ BITMASK ^*/

    U8              enDcR15Supported;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_EN_DC_R15_PRSENT, H, 0, 1 ^*/
}RrmUeEutraCapabilityIratParametersNrR15_t;

#define RRM_UE_EUTRA_CAPABILITY_IRAT_PARAMETERS_NR_R15_PRESENT    0x01
typedef struct _RrmUeEutraCapabilityEndcParamaters_t
{
    rrm_bitmask_t                               bitmask;
    /*^ BITMASK ^*/

    RrmUeEutraCapabilityIratParametersNrR15_t   iratParametersNrR15;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_IRAT_PARAMETERS_NR_R15_PRESENT, N, 0, 0 ^*/
}RrmUeEutraCapabilityEndcParamaters_t;

#define RRM_UE_EUTRA_CAPABILITY_ENDC_PARAMETERS_R15_PRESENT           0x01
typedef struct _rrm_ue_eutra_capability_v1510_ies_t
{
    rrm_bitmask_t                          bitmask;
    /*^ BITMASK ^*/

    RrmUeEutraCapabilityEndcParamaters_t   endcParameters;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_ENDC_PARAMETERS_R15_PRESENT, N, 0, 0 ^*/
}rrm_ue_eutra_capability_v1510_ies_t;

/* ENDC_MEAS_CHANGES_START */
typedef struct _RrmUeEutraCapabilityMeasParameters_t
{
  U8  measGapPatterns_v1520;
}RrmUeEutraCapabilityMeasParameters_t;

typedef struct _rrm_ue_eutra_capability_v1520_ies_t
{
    RrmUeEutraCapabilityMeasParameters_t measParameters;
}rrm_ue_eutra_capability_v1520_ies_t;
/* ENDC_MEAS_CHANGES_END */


#define RRM_UE_EUTRA_CAPABILITY_v1510_IES_PRESENT           0x01
/* ENDC_MEAS_CHANGES_START */
#define RRM_UE_EUTRA_CAPABILITY_v1520_IES_PRESENT           0x02
/* ENDC_MEAS_CHANGES_END */
typedef struct _rrm_ue_eutra_capability_v1460_ies_t
{
    rrm_bitmask_t                          bitmask;
    /*^ BITMASK ^*/

    rrm_ue_eutra_capability_v1510_ies_t    ue_eutra_capability_v1510_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_v1510_IES_PRESENT , N, 0, 0 ^*/

/* ENDC_MEAS_CHANGES_START */
    rrm_ue_eutra_capability_v1520_ies_t    ue_eutra_capability_v1520_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_v1520_IES_PRESENT , N, 0, 0 ^*/
/* ENDC_MEAS_CHANGES_END */
}rrm_ue_eutra_capability_v1460_ies_t;

#define RRM_UE_EUTRA_CAPABILITY_v1460_IES_PRESENT           0x01
typedef struct _rrm_ue_eutra_capability_v1450_ies_t
{
    rrm_bitmask_t                          bitmask;
    /*^ BITMASK ^*/

    rrm_ue_eutra_capability_v1460_ies_t    ue_eutra_capability_v1460_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_v1460_IES_PRESENT , N, 0, 0 ^*/
}rrm_ue_eutra_capability_v1450_ies_t;

#define RRM_UE_EUTRA_CAPABILITY_v1450_IES_PRESENT           0x01
typedef struct _rrm_ue_eutra_capability_v1440_ies_t
{
    rrm_bitmask_t                          bitmask;
    /*^ BITMASK ^*/

    rrm_ue_eutra_capability_v1450_ies_t    ue_eutra_capability_v1450_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_v1450_IES_PRESENT , N, 0, 0 ^*/
}rrm_ue_eutra_capability_v1440_ies_t;

#define RRM_UE_EUTRA_CAPABILITY_v1440_IES_PRESENT           0x01
typedef struct _rrm_ue_eutra_capability_v1430_ies_t
{
    rrm_bitmask_t                          bitmask;
    /*^ BITMASK ^*/

    rrm_ue_eutra_capability_v1440_ies_t    ue_eutra_capability_v1440_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_v1440_IES_PRESENT , N, 0, 0 ^*/
}rrm_ue_eutra_capability_v1430_ies_t;

#define RRM_UE_EUTRA_CAPABILITY_v1430_IES_PRESENT           0x01
typedef struct _rrm_ue_eutra_capability_v1360_ies_t
{
    rrm_bitmask_t                          bitmask;
    /*^ BITMASK ^*/

    rrm_ue_eutra_capability_v1430_ies_t    ue_eutra_capability_v1430_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_v1430_IES_PRESENT , N, 0, 0 ^*/
}rrm_ue_eutra_capability_v1360_ies_t;

#define RRM_UE_EUTRA_CAPABILITY_v1360_IES_PRESENT           0x01
typedef struct _rrm_ue_eutra_capability_v1350_ies_t
{
    rrm_bitmask_t                          bitmask;
    /*^ BITMASK ^*/

    rrm_ue_eutra_capability_v1360_ies_t    ue_eutra_capability_v1360_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_v1360_IES_PRESENT , N, 0, 0 ^*/
}rrm_ue_eutra_capability_v1350_ies_t;

#define RRM_UE_EUTRA_CAPABILITY_v1350_IES_PRESENT           0x01
typedef struct _rrm_ue_eutra_capability_v1340_ies_t
{
    rrm_bitmask_t                          bitmask;
    /*^ BITMASK ^*/

    rrm_ue_eutra_capability_v1350_ies_t    ue_eutra_capability_v1350_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_v1350_IES_PRESENT , N, 0, 0 ^*/
}rrm_ue_eutra_capability_v1340_ies_t;

#define RRM_UE_EUTRA_CAPABILITY_v1340_IES_PRESENT           0x01
typedef struct _rrm_ue_eutra_capability_v1330_ies_t
{
    rrm_bitmask_t                          bitmask;
    /*^ BITMASK ^*/

    rrm_ue_eutra_capability_v1340_ies_t    ue_eutra_capability_v1340_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_v1340_IES_PRESENT , N, 0, 0 ^*/
}rrm_ue_eutra_capability_v1330_ies_t;

#define RRM_UE_EUTRA_CAPABILITY_v1330_IES_PRESENT           0x01
typedef struct _rrm_ue_eutra_capability_v1320_ies_t
{
    rrm_bitmask_t                          bitmask;
    /*^ BITMASK ^*/

    rrm_ue_eutra_capability_v1330_ies_t    ue_eutra_capability_v1330_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_v1330_IES_PRESENT , N, 0, 0 ^*/
}rrm_ue_eutra_capability_v1320_ies_t;
/* MENB_CHANGES_END */
#endif

/* MENB_CHANGES_START */
#if  defined(ENDC_ENABLED)
typedef struct _rrm_ue_eutra_capability_v1310_ies_t
{
	U16                   bitmask;                    /*^ BITMASK ^*/

#define RRM_UE_EUTRA_CAPABILITY_V1310_RF_PARAMETERS_PRESENT 		0x01
#define RRM_UE_LAA_PARAMETERS_PRESENT                               0x02
#define RRM_UE_EUTRA_CAPABILITY_LWA_PARAMETERS_R13_PRESENT          0x04


#ifdef ENDC_ENABLED
/* MENB_CHANGES_START */
#define RRM_UE_EUTRA_CAPABILITY_v1320_IES_PRESENT                   0x010
/* MENB_CHANGES_END */
#endif





#ifdef ENDC_ENABLED
    /* MENB_CHANGES_START */
    rrm_ue_eutra_capability_v1320_ies_t       ue_eutra_capability_v1320_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_v1320_IES_PRESENT , N, 0, 0 ^*/
    /* MENB_CHANGES_END */
#endif
}rrm_ue_eutra_capability_v1310_ies_t;

typedef struct _rrm_ue_eutra_capability_v1280_ies_t
{
	U16                   bitmask;                    /*^ BITMASK ^*/
#define RRM_UE_EUTRA_CAPABILITY_v1310_IES_PRESENT			0x01

   rrm_ue_eutra_capability_v1310_ies_t             ue_eutra_capability_v1310_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_v1310_IES_PRESENT , N, 0, 0 ^*/

}rrm_ue_eutra_capability_v1280_ies_t;

typedef struct _rrm_ue_eutra_capability_v1270_ies_t
{
	U16                   bitmask;                    /*^ BITMASK ^*/
#define RRM_UE_EUTRA_CAPABILITY_v1280_IES_PRESENT			0x01

    rrm_ue_eutra_capability_v1280_ies_t             ue_eutra_capability_v1280_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_v1280_IES_PRESENT , N, 0, 0 ^*/

}rrm_ue_eutra_capability_v1270_ies_t;

typedef struct _rrm_ue_eutra_capability_v1260_ies_t
{
	U16                   bitmask;                    /*^ BITMASK ^*/
#define RRM_UE_EUTRA_CAPABILITY_v1270_IES_PRESENT			0x02

    rrm_ue_eutra_capability_v1270_ies_t             ue_eutra_capability_v1270_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_v1270_IES_PRESENT , N, 0, 0 ^*/

}rrm_ue_eutra_capability_v1260_ies_t;

typedef struct _rrm_ue_eutra_capability_v1250_ies_t
{
	U16                   bitmask;                    /*^ BITMASK ^*/
#define RRM_UE_EUTRA_CAPABILITY_v1260_IES_PRESENT			0x01

    rrm_ue_eutra_capability_v1260_ies_t             ue_eutra_capability_v1260_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_v1260_IES_PRESENT , N, 0, 0 ^*/

}rrm_ue_eutra_capability_v1250_ies_t;

typedef struct _rrm_ue_eutra_capability_v11a0_ies_t
{
	U16                   bitmask;                    /*^ BITMASK ^*/
#define RRM_UE_EUTRA_CAPABILITY_v1250_IES_PRESENT			0x01

    rrm_ue_eutra_capability_v1250_ies_t             ue_eutra_capability_v1250_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_v1250_IES_PRESENT , N, 0, 0 ^*/
}rrm_ue_eutra_capability_v11a0_ies_t;
#endif
/* LAA Rel13 Code - */

typedef struct _rrm_ue_eutra_capability_v1180_ies_t
{
    U16                                             bitmask;         /*^BITMASK^*/
#define RRM_UE_EUTRA_CAPABILITY_V1180_RF_PARAMETERS_PRESENT     0x01
/* LAA Rel13 Code + */

/* MENB_CHANGES_START */
#if  defined(ENDC_ENABLED)
    rrm_ue_eutra_capability_v11a0_ies_t             ue_eutra_capability_v11a0_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_v11a0_IES_PRESENT , N, 0, 0 ^*/
#endif
/* MENB_CHANGES_END */
/* LAA Rel13 Code - */
}rrm_ue_eutra_capability_v1180_ies_t;

typedef struct _rrm_ue_eutra_capability_v1090_ies_t
{
    U16                                             bitmask;         /*^BITMASK^*/
#define RRM_UE_EUTRA_CAPABILITY_v1180_IES_PRESENT               0x02
    

    rrm_ue_eutra_capability_v1180_ies_t             ue_eutra_capability_v1180_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_v1180_IES_PRESENT ^*/
}rrm_ue_eutra_capability_v1090_ies_t;
//#endif

typedef struct _rrm_ue_eutra_capability_v1020_ies_t
{
    U16                   bitmask;                    /*^ BITMASK ^*/
#define RRM_UE_EUTRA_CAPABILITY_V1020_UE_CATEGORY_PRESENT               0x01 
#define RRM_UE_EUTRA_CAPABILITY_V1020_RF_PARAMETERS_PRESENT             0x02 
#define RRM_UE_EUTRA_CAPABILITY_V1020_MEAS_PARAMETERS_PRESENT           0x04 
    /* CR 597 changes */
#define RRM_UE_EUTRA_CAPABILITY_INTER_RAT_PARAMS_CDMA2000_V1020_PRESENT 0x08 
    /* CR 597 changes */
    /* CR 710 changes */
#define RRM_UE_EUTRA_CAPABILITY_V1020_PHYLAYERS_PARAMETERS_PRESENT      0x10 
#define RRM_UE_EUTRA_CAPABILITY_V1020_FEATUREGROUP_IND_R10_PRESENT      0x20 
    /* CR 710 changes */
/* SPR_11104_FIX_START */
#define RRM_UE_EUTRA_CAPABILITY_INTER_RAT_PARAMS_UTRA_TDD_V1020_PRESENT 0x40     
#define RRM_UE_EUTRA_CAPABILITY_v1060_IES_PRESENT                       0x80
    /* CR 624 changes */
#define RRM_UE_EUTRA_CAPABILITY_NW_PERF_MEAS_PARAMS_PRESENT             0x100
//#ifdef IPR_LAA 
    /* CR 624 changes */
/* SPR_11104_FIX_END */

    U8                                  ue_category_v1020;    //
    /*^ O, RRM_UE_EUTRA_CAPABILITY_V1020_UE_CATEGORY_PRESENT, B, 6, 8 ^*/
    /* CR 710 changes */
    rrm_phylayer_parameters_v1020_ies_t     phylayer_parameter_v1020;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_V1020_PHYLAYERS_PARAMETERS_PRESENT , N, 0, 0 ^*/
    /* CR 710 changes */
    rrm_rf_parameters_v1020_ies_t           rf_parameters_v1020;//
    /*^ O, RRM_UE_EUTRA_CAPABILITY_V1020_RF_PARAMETERS_PRESENT , N, 0, 0 ^*/
    rrm_measparameters_v1020_ies_t          meas_parameters_v1020; //
    /*^ O, RRM_UE_EUTRA_CAPABILITY_V1020_MEAS_PARAMETERS_PRESENT , N, 0, 0 ^*/
    /* CR 710 changes */
    U32                                  featuregroup_ind_r10;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_V1020_FEATUREGROUP_IND_R10_PRESENT , OCTET_STRING, FIXED ^*/
    /* CR 710 changes */
    /* CR 597 changes */
    rrm_irat_parameters_cdma2000_v1020_t    inter_rat_parameters_cdma2000_v1020; //
    /*^ O, RRM_UE_EUTRA_CAPABILITY_INTER_RAT_PARAMS_CDMA2000_V1020_PRESENT , N, 0, 0 ^*/
    /* CR 597 changes */

/* SPR_11104_FIX_START */
    rrm_irat_parameters_utra_tdd_v1020_t    inter_rat_parameters_utra_tdd_v1020;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_INTER_RAT_PARAMS_UTRA_TDD_V1020_PRESENT, N, 0, 0 ^*/
/* SPR_11104_FIX_END */

    /* CR 624 changes */
    rrm_ue_based_nw_perf_meas_params_t      ue_based_nw_perf_meas_params;//
    /*^ O, RRM_UE_EUTRA_CAPABILITY_NW_PERF_MEAS_PARAMS_PRESENT , N, 0, 0 ^*/
    /* CR 624 changes */
    /*CR 713 changes*/
    rrm_ue_eutra_capability_v1060_ies_t     ue_eutra_capability_v1060;//
    /*^ O, RRM_UE_EUTRA_CAPABILITY_v1060_IES_PRESENT , N, 0, 0 ^*/
    /*CR 713 changes*/
//#ifdef IPR_LAA 
}rrm_ue_eutra_capability_v1020_ies_t;


/* SPR_11104_FIX_START */



typedef struct
{
    U8      ue_TxAntennaSelectionSupported; /*^ M, 0, H, 1, 1 ^*/      
    U8      ue_SpecificRefSigsSupported;    /*^ M, 0, H, 1, 1 ^*/      
}rrm_rrc_ue_capab_physical_layer_params_t;

typedef enum
{
    rrm_geran_gsm450,
    rrm_geran_gsm480,
    rrm_geran_gsm710,
    rrm_geran_gsm750, 
    rrm_geran_gsm810, 
    rrm_geran_gsm850,
    rrm_geran_gsm900P,
    rrm_geran_gsm900E, 
    rrm_geran_gsm900R, 
    rrm_geran_gsm1800, 
    rrm_geran_gsm1900,
    rrm_geran_spare5, 
    rrm_geran_spare4, 
    rrm_geran_spare3, 
    rrm_geran_spare2, 
    rrm_geran_spare1
}rrm_supported_band_geran_et;

typedef struct
{
    U8                           count;      /*^ M, 0, B, 1, 64 ^*/     /* MAX_BAND_EUTRA  */
    rrm_supported_band_geran_et  rrm_supported_band_geran[MAX_BAND_EUTRA];
        /*^ M,0,OCTET_STRING,VARIABLE ^*/   /* rrm_supported_band_geran_et */
}rrm_supported_band_list_geran_t;

typedef struct
{
    rrm_supported_band_list_geran_t     supported_band_list_geran;
    U8                              inter_rat_ps_ho_to_geran;   /*^ M, 0, H, 1, 1 ^*/ 
}rrm_geran_t;



typedef enum
{
   RRM_RRC_E_REDIRECTION_UTRA_SUPPORTED_DEFINED
}rrm_irat_params_utra_v920_e_redirection_utra_r9_et;

typedef struct
{
    rrm_irat_params_utra_v920_e_redirection_utra_r9_et       e_redirection_utra_r9;/*^ M, 0, N, 0, 0 ^*/   
    /*rrm_irat_params_utra_v920_e_redirection_utra_r9_et*/
}rrm_irat_parameters_utra_v920_t;


typedef enum
{
   RRM_ECSFB_1XRTT_R9_SUPPORTED
}rrm_eCSFB_1xrtt_r9_et;

typedef enum
{
   RRM_ECSFB_CONC_PS_MOB_1XRTT_R9_SUPPORTED
}rrm_eCSFB_conc_ps_mobility_1xrtt_r9_et;


typedef struct
{
    U16                   bitmask;                    /*^ BITMASK ^*/
#define RRM_IRAT_PARAMS_CDMA_V920_E_CSFB_CONC_PS_MOB_1XRTT_R9_PRESENT                 0x01
    rrm_eCSFB_1xrtt_r9_et           eCSFB_1xrtt_r9;/*^ M, 0, N, 0, 0 ^*/
 /*rrm_eCSFB_1xrtt_r9_et*/

    rrm_eCSFB_conc_ps_mobility_1xrtt_r9_et     eCSFB_conc_ps_mobility_1xrtt_r9;
/*^ O, RRM_IRAT_PARAMS_CDMA_V920_E_CSFB_CONC_PS_MOB_1XRTT_R9_PRESENT, N, 0, 0 ^*/
 /*rrm_eCSFB_conc_ps_mobility_1xrtt_r9_et*/

}rrm_irat_parameters_cdma2000_v920_t;


typedef struct
{
    U8 intra_freq_si_acquisition_for_ho_supported_r9;    /*^ M, 0, H, 1, 1 ^*/ 
    U8 inter_freq_si_acquisition_for_ho_supported_r9;    /*^ M, 0, H, 1, 1 ^*/ 
    U8 utran_si_acquisition_for_ho_supported_r9;    /*^ M, 0, H, 1, 1 ^*/ 
}rrm_neigh_cell_si_acquisition_parameters_r9_t;




/*! \rrm_ue_eutra_capability_v9a0_ies_t
 *  \brief This struct is used for UE EUTRAN capability v9 ies information
 *  \param phy_layer_parameters_r9
 *  \param feature_group_indicators_r9
 *  \param feature_group_ind_rel9add_r9
 *  \param inter_rat_parameters_geran_r9
 *  \param inter_rat_parameters_utra_r9
 *  \param inter_rat_parameters_cdma2000_r9
 *  \param neigh_cellsi_acquisition_parameters_r9
 */
typedef struct 
{
    rrc_bitmask_t                      bitmask; /*^ BITMASK ^*/
#define RRM_UE_EUTRA_CAPABILITY_PHY_LAYER_PARAMETERS_R9_PRESENT     0x01
#define RRM_UE_EUTRA_CAPABILITY_FEATURE_GRP_INDICATORS_R9_PRESENT   0x02
#define RRM_UE_EUTRA_CAPABILITY_FEATURE_GRP_IND_R9ADD_PRESENT       0x04
#define RRM_UE_EUTRA_CAPABILITY_IRAT_PARAMETERS_GERAN_R9_PRESENT    0x08
#define RRM_UE_EUTRA_CAPABILITY_IRAT_PARAMETERS_UTRA_R9_PRESENT     0x10
#define RRM_UE_EUTRA_CAPABILITY_IRAT_PARAMETERS_CDMA2000_R9_PRESENT 0x20
#define RRM_UE_EUTRA_CAPABILITY_NEIGH_CELLSI_ACQUISITION_PARAMS_R9_PRESENT 0x40
    
    rrm_rrc_ue_capab_physical_layer_params_t                 phy_layer_parameters_r9;
/*^ O, RRM_UE_EUTRA_CAPABILITY_PHY_LAYER_PARAMETERS_R9_PRESENT ,N, 0, 0 ^*/

    U32                                     feature_group_indicators_r9;
/*^ O, RRM_UE_EUTRA_CAPABILITY_FEATURE_GRP_INDICATORS_R9_PRESENT ,N, 0, 0 ^*/

    U32                                     feature_group_ind_rel9add_r9;
/*^ O, RRM_UE_EUTRA_CAPABILITY_FEATURE_GRP_IND_R9ADD_PRESENT ,N, 0, 0 ^*/

    rrm_geran_t                                 inter_rat_parameters_geran_r9;
/*^ O, RRM_UE_EUTRA_CAPABILITY_IRAT_PARAMETERS_GERAN_R9_PRESENT ,N, 0, 0 ^*/

    rrm_irat_parameters_utra_v920_t             inter_rat_parameters_utra_r9;
/*^ O, RRM_UE_EUTRA_CAPABILITY_IRAT_PARAMETERS_UTRA_R9_PRESENT ,N, 0, 0 ^*/

    rrm_irat_parameters_cdma2000_v920_t         inter_rat_parameters_cdma2000_r9;
/*^ O, RRM_UE_EUTRA_CAPABILITY_IRAT_PARAMETERS_CDMA2000_R9_PRESENT, N, 0, 0 ^*/

    rrm_neigh_cell_si_acquisition_parameters_r9_t   neigh_cellsi_acquisition_parameters_r9;
/*^ O, RRM_UE_EUTRA_CAPABILITY_NEIGH_CELLSI_ACQUISITION_PARAMS_R9_PRESENT, N, 0, 0 ^*/

}rrm_ue_eutra_capability_addxdd_mode_r9_t;

/*SPR_15241_Start*/

/*! \enum rrm_voice_over_ps_hs_utra_fdd_r9_et 
 *  \brief
 */
typedef enum
{
    RRM_VOICEOVERPS_HS_UTRA_FDD_R9_SUPPORTED
}rrm_voice_over_ps_hs_utra_fdd_r9_et;

/*! \enum rrm_voice_over_ps_hs_utra_tdd128_r9_et 
 *  \brief
 */
typedef enum
{
    RRM_VOICEOVERPS_HS_UTRA_TDD_R9_SUPPORTED
}rrm_voice_over_ps_hs_utra_tdd128_r9_et;


/*SPR_15241_End*/

/*! \enum rrm_srvcc_from_utra_fdd_to_utra_fdd_r9_et 
 *  \brief
 */
typedef enum
{
    RRM_SRVCC_FROM_UTRA_FDD_TO_UTRA_FDD_r9_SUPPORTED
}rrm_srvcc_from_utra_fdd_to_utra_fdd_r9_et;

/*! \enum rrm_srvcc_from_utra_fdd_to_geran_r9_et 
 *  \brief
 */
typedef enum
{
    RRM_SRVCC_FROM_UTRA_FDD_TO_GERAN_R9_SUPPORTED
}rrm_srvcc_from_utra_fdd_to_geran_r9_et;

/*! \enum rrm_srvcc_from_utra_tdd128_to_utra_tdd128_r9_et 
 *  \brief
 */
typedef enum 
{
    RRM_SRVCC_FROM_UTRA_TDD128_TO_UTRA_TDD128_R9_SUPPORTED
}rrm_srvcc_from_utra_tdd128_to_utra_tdd128_r9_et;

/*! \enum rrm_srvcc_from_utra_tdd128_to_geran_r9_et 
 *  \brief
 */
typedef enum
{
    RRM_SRVCC_FROM_UTRA_TDD128_TO_GERAN_R9_SUPPORTED
}rrm_srvcc_from_utra_tdd128_to_geran_r9_et;

/*! \rrm_irat_parameters_utra_v9c0_t
 *  \brief This struct is used for IRAT capability v9c0 ies information
 *  \param bitmask
 *  \param voice_over_ps_hs_utra_fdd_r9
 *  \param voice_over_ps_hs_utra_tdd128_r9
 *  \param srvcc_from_utra_fdd_to_utra_fdd_r9
 *  \param srvcc_from_utra_fdd_to_geran_r9
 *  \param srvcc_from_utra_tdd128_to_utra_tdd128_r9
 *  \param srvcc_from_utra_tdd128_to_geran_r9
 */
typedef struct
{
    rrc_bitmask_t                      bitmask; /*^ BITMASK ^*/
#define RRM_VOICEOVERPS_HS_UTRA_FDD_R9_PRESENT                  0x01
#define RRM_VOICEOVERPS_HS_UTRA_TDD_R9_PRESENT                  0x02
#define RRM_SRVCC_FROM_UTRA_FDD_TO_UTRA_FDD_r9_PRESENT          0x04    
#define RRM_SRVCC_FROM_UTRA_FDD_TO_GERAN_R9_PRESENT             0x08
#define RRM_SRVCC_FROM_UTRA_TDD128_TO_UTRA_TDD128_R9_PRESENT    0x10
#define RRM_SRVCC_FROM_UTRA_TDD128_TO_GERAN_R9_PRESENT          0x20

    rrm_voice_over_ps_hs_utra_fdd_r9_et          voice_over_ps_hs_utra_fdd_r9;
/*^ O, RRM_VOICEOVERPS_HS_UTRA_FDD_R9_PRESENT, N, 0, 0 ^*/
    rrm_voice_over_ps_hs_utra_tdd128_r9_et       voice_over_ps_hs_utra_tdd128_r9;
/*^ O, RRM_VOICEOVERPS_HS_UTRA_TDD_R9_PRESENT, N, 0, 0 ^*/
    rrm_srvcc_from_utra_fdd_to_utra_fdd_r9_et    srvcc_from_utra_fdd_to_utra_fdd_r9;
/*^ O, RRM_SRVCC_FROM_UTRA_FDD_TO_UTRA_FDD_r9_PRESENT, N, 0, 0 ^*/
    rrm_srvcc_from_utra_fdd_to_geran_r9_et       srvcc_from_utra_fdd_to_geran_r9;
/*^ O, RRM_SRVCC_FROM_UTRA_FDD_TO_GERAN_R9_PRESENT, N, 0, 0 ^*/
    rrm_srvcc_from_utra_tdd128_to_utra_tdd128_r9_et srvcc_from_utra_tdd128_to_utra_tdd128_r9;
/*^ O, RRM_SRVCC_FROM_UTRA_TDD128_TO_UTRA_TDD128_R9_PRESENT, N, 0, 0 ^*/
    rrm_srvcc_from_utra_tdd128_to_geran_r9_et    srvcc_from_utra_tdd128_to_geran_r9;
/*^ O, RRM_SRVCC_FROM_UTRA_TDD128_TO_GERAN_R9_PRESENT, N, 0, 0 ^*/    

}rrm_irat_parameters_utra_v9c0_t;

/*! \ue_eutra_capability_v9c0_ies_t
 *  \brief This struct is used for UE EUTRAN capability v9c0 ies information
 *  \param bitmask
 *  \param inter_rat_parameters_utra_v9c0
 */
typedef struct
{
    rrm_bitmask_t                      bitmask; /*^ BITMASK ^*/
#define RRM_UE_EUTRA_CAPABILITY_IRAT_PARAMS_UTRA_v9c0_PRESENT   0x01

    rrm_irat_parameters_utra_v9c0_t        inter_rat_parameters_utra_v9c0;
/*^ O, RRM_UE_EUTRA_CAPABILITY_IRAT_PARAMS_UTRA_v9c0_PRESENT, N, 0, 0 ^*/    
    
}rrm_ue_eutra_capability_v9c0_ies_t;


/*! \rrm_ue_eutra_capability_v9a0_ies_t
 *  \brief This struct is used for UE EUTRAN capability v9a0 ies information
 *  \param feature_group_ind_rel9add_r9 
 *  \param fdd_add_ue_eutra_capabilities_r9 
 *  \param tdd_add_ue_eutra_capabilities_r9
 *  \param ue_eutra_capability_v9c0_ies
 */
typedef struct 
{
    rrm_bitmask_t                      bitmask; /*^ BITMASK ^*/
#define RRM_UE_EUTRA_CAPABILITY_FEATURE_GRP_IND_REL9_ADD_PRESENT 0x01
#define RRM_UE_EUTRA_CAPABILITY_ADDFDD_MODE_R9_PRESENT           0x02
#define RRM_UE_EUTRA_CAPABILITY_ADDTDD_MODE_R9_PRESENT           0x04
/*SPR_15241_Start*/

#define RRM_UE_EUTRA_CAPABILITY_V9c0_IES_PRESENT                 0x08    
/*SPR_15241_End*/

    U32      feature_group_ind_rel9add_r9;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_FEATURE_GRP_IND_REL9_ADD_PRESENT ^*/
    
    rrm_ue_eutra_capability_addxdd_mode_r9_t    fdd_add_ue_eutra_capabilities_r9;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_ADDFDD_MODE_R9_PRESENT ^*/

    rrm_ue_eutra_capability_addxdd_mode_r9_t    tdd_add_ue_eutra_capabilities_r9;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_ADDTDD_MODE_R9_PRESENT ^*/
    
    /*SPR_15241_Start*/
    rrm_ue_eutra_capability_v9c0_ies_t          ue_eutra_capability_v9c0_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_V9c0_IES_PRESENT ^*/
    /*SPR_15241_End*/

}rrm_ue_eutra_capability_v9a0_ies_t;
/* SPR_11104_FIX_END */






/* CR 530 changes start*/
/*! \rrm_ue_eutra_capability_v940_ies_t 
 *  \brief This struct is used for UE EUTRAN capability v920 ies information
 *  \param  csg_proximity_indication_parameters_r9
 */
typedef struct _rrm_ue_eutra_capability_v940_ies_t
{
    U16                   bitmask;                    /*^ BITMASK ^*/
#define RRM_UE_EUTRA_CAPABILITY_V1020_IES_PRESENT     0x01 

/* SPR_11104_FIX_START */
#define RRC_UE_EUTRA_CAPABILITY_V9a0_IES_PRESENT      0x02
    rrm_ue_eutra_capability_v9a0_ies_t         ue_eutra_capability_v9a0_ies;
    /*^ O, RRC_UE_EUTRA_CAPABILITY_V9a0_IES_PRESENT, N, 0, 0 ^*/
/* SPR_11104_FIX_END */

    rrm_ue_eutra_capability_v1020_ies_t        ue_eutra_capability_v1020_ies;
    /*^ O, RRC_RRM_UE_EUTRA_CAPABILITY_V1020_IES_PRESENT , N, 0, 0 ^*/

}rrm_ue_eutra_capability_v940_ies_t;

typedef struct _rrm_phy_layer_parameters_v920_t
{
    U8 enhanced_dual_layer_fdd_supported_r9;
    /*^ M, 0, H, 1, 1 ^*/ /* rrm_bool_et */

    U8 enhanced_dual_layer_tdd_supported_r9;
    /*^ M, 0, H, 1, 1 ^*/ /* rrm_bool_et */
}rrm_phy_layer_parameters_v920_t;
/* CR 530 changes end*/
/*Rel10 CR changes end*/
/* SPR-10489-fix start */
/*! \ rrm_ue_irat_parameters_geran_v920_t
 *  \brief This struct is used for UE GERAN capability v920 ies information
 *  \param  bitmask 
 *  \param  dtm_r9
 */
typedef struct _rrm_ue_irat_parameters_geran_v920_t
{
    U16                   bitmask;                    /*^ BITMASK ^*/
#define RRM_UE_IRAT_PARAMS_GERAN_V920_DTM_R9_PRESENT    0x01
/*SPR 20571 START*/
#define RRM_UE_IRAT_PARAMS_GERAN_V920_E_REDIRECTIONGERAN_R9_PRESENT 0x02
/*SPR 20571 END*/

    U8                    dtm_r9;
    /*^ O, RRM_UE_IRAT_PARAMS_GERAN_V920_DTM_R9_PRESENT, H, 0, 0 ^*/
    /*SPR 20571 START*/
    U8                    e_redirectiongeran_r9;
    /*^ O, RRM_UE_IRAT_PARAMS_GERAN_V920_E_REDIRECTIONGERAN_R9_PRESENT, H, 0, 0 ^*/
    /*SPR 20571 END*/

} rrm_ue_irat_parameters_geran_v920_t;


/*! \ rrm_ue_eutra_capability_v920_ies_t
 *  \brief This struct is used for UE EUTRAN capability v920 ies information
 *  \param  csg_proximity_indication_parameters_r9
 *  \param  inter_rat_parameters_geran_v920
 *  \param  inter_rat_parameters_cdma2000_v920
 *  \param  ue_eutra_capability_v940_ies 
 */
 /* SPR_11104_FIX_START */
typedef struct _rrm_ue_eutra_capability_v920_ies_t
{
    rrm_bitmask_t   bitmask;
#define RRM_UE_IRAT_CDMA2000_PARAMS_V920_PRESENT    0x01
    /*Rel10 CR 530 changes start*/
#define RRM_UE_EUTRA_CAPABILITY_V940_IES_PRESENT    0x02
    /*Rel10 CR 530 changes end*/

    rrm_ue_irat_parameters_geran_v920_t     inter_rat_parameters_geran_v920;
    /*^ M, 0, N, 0, 0 ^*/
    /* SPR-10489-fix end */
    /*TM 7/8 start*/
    rrm_phy_layer_parameters_v920_t phy_layer_parameters_v920; /*^ M, 0, N, 0, 0 ^*/
    /*TM 7/8 end*/
    rrm_csg_proximity_indication_parameters_r9_t csg_proximity_indication_parameters_r9; /*^ M, 0, N, 0, 0 ^*/

    /* SPR-10489-fix end */

    /* SPR 12897 Start */
    rrm_neigh_cell_si_acquisition_parameters_r9_t neigh_cell_si_acquisition_parameters_r9;
    /*^ M, 0, N, 0, 0 ^*/
    /* SPR 12897 End */

    rrm_ue_irat_parameters_cdma2000_v920_t  inter_rat_parameters_cdma2000_v920;
    /*^ O, RRM_UE_IRAT_CDMA2000_PARAMS_V920_PRESENT,N,0,0 ^*/
    
    /*Rel10 CR 530 changes start*/
    rrm_ue_eutra_capability_v940_ies_t        ue_eutra_capability_v940_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_V940_IES_PRESENT,N,0,0 ^*/
    /*Rel10 CR 530 changes end*/
}rrm_ue_eutra_capability_v920_ies_t;
/*csg end*/ 
/* SPR_11104_FIX_END */


/*! \ rrm_ue_eutra_radio_capability_info_t
 *  \brief This struct is used for UE EUTRAN RADIO CAPABILITY INFO
 *  \param bitmask
 *  \param eutra_radio_capability_info_def
 *  \param ue_eutra_capability_v920_ies
 */
typedef struct _rrm_ue_eutra_radio_capability_info_t
{
    /*csg start*/
#define RRM_UE_EUTRA_CAPABILITY_V920_IES_PRESENT          0x01

/* MENB_CHANGES_START */
#if  defined(ENDC_ENABLED)
#define RRM_UE_EUTRA_CAPABILITY_V1310_IES_PRESENT          0x02
#endif
/* MENB_CHANGES_END */
    U16                   bitmask;                    /*^ BITMASK ^*/
    /*csg end*/
    rrm_ue_eutra_radio_capability_info_def_t   eutra_radio_capability_info_def;	/*^ M, 0, N, 0, 0 ^*/
    /*csg start*/
    rrm_ue_eutra_capability_v920_ies_t        ue_eutra_capability_v920_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_V920_IES_PRESENT,N,0,0 ^*/
    /*csg end*/

/* MENB_CHANGES_START */
#if  defined(ENDC_ENABLED)
    rrm_ue_eutra_capability_v1310_ies_t        ue_eutra_capability_v1310_ies;
    /*^ O, RRM_UE_EUTRA_CAPABILITY_V1310_IES_PRESENT, N, 0, 0 ^*/
#endif  // LWA_ENABLED
/* MENB_CHANGES_END */
}rrm_ue_eutra_radio_capability_info_t;

#ifdef ENDC_ENABLED
/* MENB_CHANGES_START */
typedef struct _rrm_band_parameters_eutra_t
{
    rrm_bitmask_t     bitmask;
    /*^ BITMASK ^*/
#define RRM_BAND_PARAMETERS_CA_BANDWIDTH_CLASS_DL_PRESENT  0x01
#define RRM_BAND_PARAMETERS_CA_BANDWIDTH_CLASS_UL_PRESENT  0x02

    U16               band_eutra;
    /*^ M, 0, N, 0, 0 ^*/

    U32               ca_bandwidth_class_dl_eutra;
    /*^ O, RRM_BAND_PARAMETERS_CA_BANDWIDTH_CLASS_DL_PRESENT,N,0,0 ^*/

    U32               ca_bandwidth_class_ul_eutra;
    /*^ O, RRM_BAND_PARAMETERS_CA_BANDWIDTH_CLASS_UL_PRESENT,N,0,0 ^*/
}rrm_band_parameters_eutra_t;

typedef struct _rrm_band_parameters_nr_t
{
    rrm_bitmask_t     bitmask;
    /*^ BITMASK ^*/
#define RRM_BAND_PARAMETERS_CA_BANDWIDTH_CLASS_DL_NR_PRESENT  0x01
#define RRM_BAND_PARAMETERS_CA_BANDWIDTH_CLASS_UL_NR_PRESENT  0x02

    U16               band_nr;
    /*^ M, 0, N, 0, 0 ^*/

    U32               ca_bandwidth_class_dl_nr;
    /*^ O, RRM_BAND_PARAMETERS_CA_BANDWIDTH_CLASS_DL_NR_PRESENT,N,0,0 ^*/

    U32               ca_bandwidth_class_ul_nr;
    /*^ O, RRM_BAND_PARAMETERS_CA_BANDWIDTH_CLASS_UL_NR_PRESENT,N,0,0 ^*/
}rrm_band_parameters_nr_t;

typedef struct _rrm_band_parameters_t
{
    rrm_bitmask_t               bitmask;
    /*^ BITMASK ^*/
#define RRM_BAND_PARAMETERS_EUTRA_PRESENT       0x01
#define RRM_BAND_PARAMETERS_NR_PRESENT          0x02

    rrm_band_parameters_eutra_t eutra;
    /*^ O, RRM_BAND_PARAMETERS_EUTRA_PRESENT,N,0,0 ^*/

    rrm_band_parameters_nr_t    nr;
    /*^ O, RRM_BAND_PARAMETERS_NR_PRESENT,N,0,0 ^*/
}rrm_band_parameters_t;

typedef struct _rrm_band_combination_band_list_t
{
    U16                    count;
    /*^ M, 0, H, 0, MAX_SIMULTANEOUS_BANDS_EUTRA_NR ^*/

    rrm_band_parameters_t  band_list[MAX_SIMULTANEOUS_BANDS_EUTRA_NR];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_band_combination_band_list_t;

typedef struct _rrm_band_combination_t
{
    rrm_bitmask_t                    bitmask;
    /*^ BITMASK ^*/

    rrm_band_combination_band_list_t band_combination_band_list;
    /*^ M, 0, N, 0, 0 ^*/

    U16                              feature_set_combination;
    /*^ M, 0, N, 0, 0 ^*/
}rrm_band_combination_t;

typedef struct _rrm_supported_band_comb_list_t
{
    U16                    count;
    /*^ M, 0, H, 0, MAX_BAND_COMB_EUTRA_NR ^*/

    rrm_band_combination_t band_combination_list[MAX_BAND_COMB_EUTRA_NR];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_supported_band_comb_list_t;

typedef struct _rrm_rf_parameters_mrdc_t
{
    rrm_bitmask_t                  bitmask;
    /*^ BITMASK ^*/
#define RRM_RF_PARAMETERS_MRDC_SUPPORTED_BAND_COMB_LIST_PRESENT      0x01

    rrm_supported_band_comb_list_t supported_band_comb_list;
    /*^ O, RRM_RF_PARAMETERS_MRDC_SUPPORTED_BAND_COMB_LIST_PRESENT,N,0,0 ^*/
}rrm_rf_parameters_mrdc_t;

/* ENDC_MEAS_CHANGES_START */

typedef struct _rrm_nr_meas_parameters_t
{
    rrm_bitmask_t              bitmask;
    /*^ BITMASK ^*/

    U32                          supported_gap_pattern;
    /*^ M, 0, N, 0, 0 ^*/
}rrm_nr_meas_parameters_t;

typedef struct _rrm_eutra_nr_radio_capability_info_t
{
    rrm_rf_parameters_mrdc_t   rf_parameters_mrdc;
    /*^ M, 0, N, 0, 0 ^*/
}rrm_ue_eutra_nr_radio_capability_info_t;
/* MENB_CHANGES_END */

typedef struct _rrm_nr_radio_capability_info_t
{
    rrm_bitmask_t              bitmask;
    /*^ BITMASK ^*/

#define RRM_NR_RADIO_CAPABILITY_NR_MEAS_PRESENT 0x01

    rrm_nr_meas_parameters_t   rrm_nr_meas_parameters;
    /*^ O, RRM_NR_RADIO_CAPABILITY_NR_MEAS_PRESENT, N, 0, 0 ^*/
}rrm_ue_nr_radio_capability_info_t;
#endif
/* ENDC_MEAS_CHANGES_END */

/*! \  struct rrm_ue_capability_params_t
 *  \brief UE Capability related parameters
 *  \param U16 bitmask
 *  \param rrm_ue_eutra_radio_capability_info_t rrm_eutra_radio_capability_info 
 */
typedef struct _rrm_ue_capability_params_t
{
    U16									bitmask; 
#define RRM_UE_EUTRA_RADIO_CAPABILITY_PRESENT      0x01
/*SPR_17893_START*/
#define RRM_UE_UTRA_RADIO_CAPABILITY_PRESENT       0x02
/*SPR_17893_END*/
/*SPR 20459 FIX START*/
#define RRM_UE_CDMA2000_RADIO_CAPABILITY_PRESENT   0x04
/*SPR 20459 FIX END*/

#ifdef ENDC_ENABLED
/* MENB_CHANGES_START */
#define RRM_UE_EUTRA_NR_RADIO_CAPABILITY_PRESENT   0x08
#define RRM_UE_NR_RADIO_CAPABILITY_PRESENT         0x10
/* MENB_CHANGES_END */
#endif

    rrm_ue_eutra_radio_capability_info_t			rrm_eutra_radio_capability_info; /*^ O, RRM_UE_EUTRA_RADIO_CAPABILITY_PRESENT,N,0,0 ^*/

#ifdef ENDC_ENABLED
    /* MENB_CHANGES_START */
    rrm_ue_eutra_nr_radio_capability_info_t			rrm_eutra_nr_radio_capability_info;
    /*^ O, RRM_UE_EUTRA_NR_RADIO_CAPABILITY_PRESENT,N,0,0 ^*/

    rrm_ue_nr_radio_capability_info_t			    rrm_nr_radio_capability_info;
    /*^ O, RRM_UE_NR_RADIO_CAPABILITY_PRESENT, N, 0, 0 ^*/
    /* MENB_CHANGES_END */
#endif
}rrm_ue_capability_params_t;

/*SPR_17893_START*/

/*! \  struct rrm_utran_capability_enq_info_t
 *  \brief This structure contains the info of Utran Cap transfer procedure
 *  \param is_utran_cap_enq_req_sent   If Utran Capability Enq Req sent
 *  \param is_utran_cap_enq_resp_recd  If Utran Capability Enq Resp received
 */
typedef struct _rrm_utran_capability_enq_info_t
{
  rrm_bool_t is_utran_cap_enq_req_sent;
  rrm_bool_t is_utran_cap_enq_resp_recd;

}rrm_utran_capability_enq_info_t;

/*! \  struct rrm_ue_capability_enq_res_t
 *  \brief UE Capability Enquiry Respone
 *  \param is_utran_cap_present Utran capability presence
 */
typedef struct _rrm_ue_capability_enq_res_t
{
   rrm_bool_et                 is_utran_cap_present;
}rrm_ue_capability_enq_res_t;
/*SPR_17893_END*/


/* UE MEAS CHANGES :STARTS */
/*! \  rrm_ue_meas_config_t
 *  \brief Structure for storing the Measurement configuration
 *  \param U16 carr_freq
 *  \param rrm_ue_meas_config_status_et meas_config_status
 *  \param U8 cgi_req_cnt
 *  \param U8 cgi_success_cnt
 *  \param rrm_return_et cgi_resp_status
 */
typedef struct _rrm_ue_meas_config_t
{
    U8                              meas_id;
    U8                              meas_obj_id;
    U8                              report_id;
    /* SPR 18008 Fix Start */ 
    /* SPR 15314 Start*/
    U8 report_counter;
    /* SPR 15314 End */
    rrm_rat_type_et                 rat_type;
    U32                             carr_freq;  
    /*!< EARFCN/UARFCN value */
    U8				    cgi_req_cnt;
    /*!< No. of Report CGI requests sent*/
    U8				    cgi_success_cnt; 
    /*!< No. of Success responses received for Report CGI request*/
    rrm_bool_t obj_created_at_ue;
    rrm_son_geran_band_indicator_et band_ind;
    rrm_son_cdma_band_class_et              band_class;
    rrm_ue_meas_config_status_et    meas_config_status; 
    /*!< NULL/Meas Req Sent/Resp Rcvd*/
    rrm_return_et		    cgi_resp_status; 
    /* SPR 18008 Fix End */ 

    /*!< Resp Status for Report CGI Request */
    /* Spr 18401 Changes Start */
    rrm_son_geran_following_arfcns_t  following_arfcns;
    /* Spr 18401 Changes End */

}rrm_ue_meas_config_t;

/* UE MEAS CHANGES :STARTS */
/*! \  struct rrm_ue_meas_result_serv_cell_t
 *  \brief structure for storing the Measurement serving cell result info
 *  \param U8 rsrp_result
 *  \param U8 rsrq_result
 */
typedef struct _rrm_ue_meas_result_serv_cell_t
{
    U8    rsrp_result;        /*!< RSRP of serving cell */
    U8    rsrq_result;        /*!< RSRQ of serving cell */
}rrm_ue_meas_result_serv_cell_t;

/*! \  struct rrm_ue_meas_config_response_t
 *  \brief Structure for storing the Measurement config Response
 *  \param U16  bitmask
 *  \param U16 ue_index
 *  \param U8 response
 *  \param rrm_cause_t cause
 */
typedef struct _rrm_ue_meas_config_response_t
{
    U16                 bitmask; 
#define RRM_UE_MEAS_CONFIG_RESP_CAUSE_PRESENT       0x01

    U16                 ue_index;   /*!< UE to which resp belongs*/
    U8                  response;   /*!< Success/Failure */
    rrm_cause_t         cause;      /*!< cause */
}rrm_ue_meas_config_response_t;

/*CDMA2000 Changes start*/
typedef struct _rrm_ue_cell_global_id_cdma2000_t
{
    U16   bitmask;

#define RRM_UEM_CELL_GLOBAL_ID_CDMA2000_1XRTT_PRESENT       0x01
#define RRM_UEM_CELL_GLOBAL_ID_CDMA2000_HRPD_PRESENT        0x02

    U8    cell_global_id_1xrtt[6];
    /*^ O, RRM_UEM_CELL_GLOBAL_ID_CDMA2000_1XRTT_PRESENT, OCTET_STRING, FIXED ^*/
    /*BIT STRING (SIZE (47))*/

    U8    cell_global_id_hrpd[16];
    /*^ O, RRM_UEM_CELL_GLOBAL_ID_CDMA2000_HRPD_PRESENT, OCTET_STRING, FIXED ^*/
    /*BIT STRING (SIZE (128))*/

}rrm_ue_cell_global_id_cdma2000_t;

typedef struct _rrm_ue_meas_result_cdma2000_meas_result_t
{
    U16    bitmask;
#define RRM_UEM_MEAS_RESULT_CDMA2000_PILOT_PN_PHASE_PRESENT     0x01
    U16           pilot_pn_phase;
    /*^ O, RRM_UEM_MEAS_RESULT_CDMA2000_PILOT_PN_PHASE_PRESENT, H, 0, 32767 ^*/

    U8            pilot_strength;                   
}rrm_ue_meas_result_cdma2000_meas_result_t;

typedef struct _rrm_ue_meas_result_cdma2000_t
{
    U16    bitmask;
#define RRM_UEM_MEAS_RESULT_CDMA2000_CGI_INFO_PRESENT   0x01
    U16                                   phy_cell_id;    /*^ M, 0, H, 0, 511 ^*/
    rrm_ue_cell_global_id_cdma2000_t             cgi_info;
    /*^ O, RRM_UEM_MEAS_RESULT_CDMA2000_CGI_INFO_PRESENT ^*/

    rrm_ue_meas_result_cdma2000_meas_result_t    meas_result;
}rrm_ue_meas_result_cdma2000_t;

typedef struct _rrm_ue_meas_result_list_cdma2000_t
{
    U8                  cdma2000_count;
    rrm_ue_meas_result_cdma2000_t  meas_result_cdma2000[RRM_MAX_CELLS_REPORTED];
}rrm_ue_meas_result_list_cdma2000_t;

typedef struct _rrm_ue_cdma2000_meas_results_t
{
    U8                  pre_registration_status_hrpd;
    rrm_ue_meas_result_list_cdma2000_t   meas_result_list_cdma2000;
}rrm_ue_cdma2000_meas_results_t;
/*CDMA2000 Changes ends*/

/* UE Positioning start */
typedef struct _rrm_meas_result_ecid_r9_t
{
    U16    ue_rxtx_time_diff;
    U8     current_sfn_r9[ECID_CURRENT_SFN_OCTET_SIZE];
}rrm_meas_result_ecid_r9_t;
/* UE Positioning end */

/* ENDC_MEAS_CHANGES_START */
#ifdef ENDC_ENABLED

typedef struct _meas_result_nr_r15_t
{
    U16                   bitmask;
#define MEAS_RESULT_NR_RSRP_PRESENT                  0x01
#define MEAS_RESULT_NR_RSRQ_PRESENT                  0x02
#define MEAS_RESULT_NR_RS_SINR_PRESENT               0x04
    rsrp_result_nr_r15_t   rsrp_result_nr_r15;
    /*^ O, MEAS_RESULT_NR_RSRP_PRESENT, H, 0, 127 ^*/
    rsrq_result_nr_r15_t   rsrq_result_nr_r15;   
    /*^ O, MEAS_RESULT_NR_RSRQ_PRESENT, H, 0, 127 ^*/
    rs_sinr_result_r15_t   rs_sinr_result_r15;   
    /*^ O, MEAS_RESULT_NR_RS_SINR_PRESENT, H, 0, 127 ^*/
}meas_result_nr_r15_t;

typedef struct _rrm_nr_meas_results_t
{
    rrm_x2_gb_gnb_id_t      selected_gnb_id ;
    phy_scell_id_nr_r15_t   pci_r15;
    meas_result_nr_r15_t    meas_result_nr_r15;
}rrm_nr_meas_results_t;

typedef struct _rrm_nr_meas_results_list_t
{
  U8 count;
  rrm_nr_meas_results_t rrm_nr_meas_results[8];
}rrm_nr_meas_results_list_t;

typedef struct _rrm_meas_result_ssb_index_r15_t
{
    U16                   bitmask;
#define RRM_MEAS_RESULT_SSB_INDEX_PRESENT                  0x01
 rs_index_nr_r15_t    ssb_index;
 meas_result_nr_r15_t meas_result_ssb_index; /*^ M, 0, H, 0, 63 ^*/
}rrm_meas_result_ssb_index_r15_t;

typedef struct _rrm_meas_result_ssb_indexlist_r15_t
{
   U8      count; /* ^ M, 0, B, 1, 32 ^*/
   rrm_meas_result_ssb_index_r15_t  meas_result_ssb_index_r15[MAX_SSB_IDX_R15];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_meas_result_ssb_indexlist_r15_t;


typedef struct _rrm_meas_result_neighbour_nr_cell_r15_t
{
    U16                   bitmask;
#define RRM_MEAS_RESULT_SSB_INDEX_LIST_PRESENT                  0x01
    phy_scell_id_nr_r15_t              pci_r15; /*^ M, 0, H, 0, 1007 ^*/ 
    meas_result_nr_r15_t               meas_result_nr_r15;
    rrm_meas_result_ssb_indexlist_r15_t    meas_result_ssb_indexlist_r15;
}rrm_meas_result_neighbour_nr_cell_r15_t;

typedef struct _rrm_meas_result_nr_neighbour_list_r15_t
{
    U8                          count;
    rrm_meas_result_neighbour_nr_cell_r15_t    meas_result_neighbour_nr_cell_r15[RRM_MAX_CELLS_REPORTED];
}rrm_meas_result_nr_neighbour_list_r15_t;

#endif
/* ENDC_MEAS_CHANGES_END */


/*! \  struct rrm_ue_measurment_results_ind_t
 *  \brief Structure for UE measurement release indication 
 *  \param rrm_ue_index_t ue_index
 *  \param U8 meas_id
 *  \param rrm_ue_meas_result_serv_cell_t meas_result_serv_cell
 *  \param U8 eutran_count
 *  \param rrm_son_eutran_neighbor_cell_t 
 eutran_neighbor_list[RRM_MAX_CELLS_REPORTED]
 *  \param U8 utran_count
 *  \param rrm_son_utran_neighbor_cell_t
 utran_neighbor_list[RRM_MAX_CELLS_REPORTED]
 *  \param rrm_meas_result_ecid_r9_t
 *  \param rrm_meas_result_wlan_list_r13_t 
 *  \param rrm_meas_result_nr_neighbour_list_r15_t 
 meas_result_ecid_r9
 */
typedef struct _rrm_ue_measurment_results_ind_t
{
    rrm_ue_index_t                  ue_index;   /*!< UE to result ind belongs */
    U8                              meas_id;    /*!< Meas Id of Meas results */
    rrm_ue_meas_result_serv_cell_t  meas_result_serv_cell; /*!< Meas results of
                                                             serving cell */
    U8                              eutran_count; /*!< No. of EUTRAN nbrs 
                                                    detected */
    rrm_son_eutran_neighbor_cell_t  eutran_neighbor_list[RRM_MAX_CELLS_REPORTED];
    /*!< EUTRAN nbr info list */
    U8                              utran_count; /*!< No. of UTRAN nbrs 
                                                   detected */
    rrm_son_utran_neighbor_cell_t   utran_neighbor_list[RRM_MAX_CELLS_REPORTED];
    /*!< UTRAN nbr info list */
    U8                              geran_count; /*!< No. of GERAN nbrs
                                                   detected */
    rrm_son_geran_neighbor_cell_t   geran_neighbor_list[RRM_MAX_CELLS_REPORTED];
    /*!< GERAN nbr info list */
    rrm_ue_cdma2000_meas_results_t  cdma2000_meas_results;

    rrm_meas_result_ecid_r9_t       meas_result_ecid_r9;  /*!< Meas results for ECID */

/* ENDC_MEAS_CHANGES_START */
#ifdef ENDC_ENABLED
    rrm_meas_result_nr_neighbour_list_r15_t rrm_meas_result_nr_neighbour_list_r15; /*Meas results for EUTRA-NR*/
#endif
/* ENDC_MEAS_CHANGES_END */
}rrm_ue_measurment_results_ind_t;

/* UE MEAS CHANGES :ENDS */

/*! \enum rrm_ue_states_et
 *  \brief An enum for States of UE FSM
 */
typedef enum
{
    RRM_UE_STATE_IDLE,				/*!< UEM FSM state 0 */ 
    RRM_UE_WAIT_FOR_CAP_IND,		/*!< UEM FSM state 1 */
    RRM_UE_STATE_ACTIVE,			/*!< UEM FSM state 2 */
    RRM_UE_STATE_HO_ONGOING,		/*!< UEM FSM state 3 */
    /*SPR_17893_START*/
    RRM_UE_STATE_WAIT_FOR_CAP_ENQ_RES,/*!< UEM FSM state 4 */
    RRM_UE_MAX_STATE				/*!< UEM FSM state 5 */
    /*SPR_17893_END*/
}rrm_ue_states_et;

/* SPR 15232 start */
typedef enum
{
    RRM_UE_MEAS_EVENT_NO_COND,
    RRM_UE_MEAS_EVENT_ENTER_COND,
    RRM_UE_MEAS_EVENT_LEAVE_COND
}rrm_meas_event_cond_et;
/* SPR 15232 end */
/*! \enum rrm_ue_events_et
 *  \brief An enum for RRM UE Events
 */
typedef enum
{
    RRC_RRM_UE_ADMISSION_REQ_EVENT,	/*!< FSM event 0 	*/
    RRC_RRM_UE_ADMISSION_CNF_EVENT, /*!< FSM event 1 	*/
    RRC_RRM_UE_CAPABILITY_IND_EVENT,/*!< FSM event 2 	*/
    RRC_RRM_UE_CAPABILITY_ENQUIRY_RESP_EVENT,
    /*!< FSM event 3 	*/
    RRC_RRM_ERB_SETUP_REQUEST_EVENT,/*!< FSM event 4 	*/
    RRC_RRM_ERB_SETUP_CNF_EVENT,	/*!< FSM event 5 	*/
    RRC_RRM_MEASURMENT_RESULTS_IND_EVENT,
    /*!< FSM event 6 	*/
    RRC_RRM_ERB_MODIFY_REQUEST_EVENT,
    /*!< FSM event 7 	*/
    RRC_RRM_ERB_RELEASE_REQUEST_EVENT,
    /*!< FSM event 8 	*/
    RRC_RRM_ERB_MODIFY_CNF_EVENT,
    /*!< FSM event 9 	*/
    RRC_RRM_ERB_RELEASE_CNF_EVENT,
    /*!< FSM event 10 	*/
    RRC_RRM_UE_RELEASE_REQ_EVENT,   /*!< FSM event 11 	*/
    RRC_RRM_UE_HO_ADM_REQ_EVENT,	/*!< FSM event 12 	*/
    RRC_RRM_UE_HO_ADM_CNF_EVENT,	/*!< FSM event 13 	*/
    RRC_RRM_UE_HO_CMD_REQ_EVENT,	/*!< FSM event 14 	*/
    RRC_RRM_HO_FAILURE_EVENT,		/*!< FSM event 15 	*/
    RRC_RRM_HO_CANCEL_RESP_EVENT,	/*!< FSM event 16 	*/
    RRC_RRM_UE_CONTEXT_MOD_REQ_EVENT,   /*!< FSM event 17       */
    RRC_RRM_UE_CONTEXT_MOD_CNF_EVENT,   /*!< FSM event 18       */
    RRC_RRM_MEAS_CONFIG_RESP_EVENT,     /*!< FSM event 19       */
    /* MC, MR & HO Start */
    RRC_RRM_HO_RESTRICTION_LIST_IND_EVENT,/*!< FSM event 20       */
    RRM_UE_TIMER_EXPIRY_EVENT,          /*!< FSM event 21       */
    /* MC, MR & HO End */
    /*LTE_RRM_TM_SWITCH_START*/
    RRC_RRM_UE_RECONFIG_RESP_EVENT,		/*!< FSM event 22	*/
    RRM_MAC_PERIODIC_REPORTS_IND_EVENT,		/*!< FSM event 23*/
    /*csg start*/
    /*PROXIMITY IND START*/
    RRC_RRM_PROXIMITY_IND_EVENT,         /*!< FSM event 24*/
    /*PROXIMITY IND END*/
    /*csg end*/
    /*RRC_RRM RLF IND start*/
    RRC_RRM_RLF_IND_EVENT,         /*!< FSM event 25*/
    /*RRC_RRM RLF IND end*/
    /*LTE_RRM_TM_SWITCH_END*/
    RRC_RRM_UE_CDMA2000_MOB_INFO_REQ_EVENT,  /*!<FSM event 26*/
    RRC_RRM_DL_NAS_TRANSPORT_IND_EVENT,              /*!<FSM event 27*/ 
    RRC_RRM_INTRA_ENB_HO_IND_EVENT,          /*!<FSM event 28 */
    /* UE positioning start */
    S1AP_RRM_LPPA_ECID_MEAS_INIT_REQ_EVENT,            /*!<FSM event 29 */
    S1AP_RRM_LPPA_ECID_MEAS_TERMINATION_COMMAND_EVENT, /*!<FSM event 30 */
    S1AP_RRM_LPPA_ERROR_IND_EVENT,                     /*!<FSM event 31 */
    /* UE positioning end */
    RRC_RRM_INTER_FREQ_MEAS_IND_EVENT,           /*!<FSM event 32 */
    RRC_RRM_LOCAL_ERROR_IND_EVENT,                     /*!<FSM event 33 */
    RRC_RRM_LIPA_HO_REQUIRED_EVENT,                    /*!<FSM event 34 */
    /*SPR-695 Fix Starts*/
    RRM_RELEASE_LOW_PRIORITY_UE_REL_EVENT,                /*!<FSM event 35 */
    /*SPR-695 Fix Ends*/
    /* SPR 12512 Fix Start */
    RRC_RRM_UE_SYNC_STATUS_IND_EVENT,       /*!<FSM event 36 */
    /* SPR 12512 Fix End */
    /* SPR 9216:MRO enhancement start */
    /* SPR_16053_Fix: Start */
    RRC_RRM_UE_REESTAB_CONFIG_CNF_EVENT,          /*!<FSM event 37 */
    /* SPR_16053_Fix: End */
    /* SPR 17784 Start */
    RRM_UE_CGI_TIMER_EXPIRY_EVENT,         /*!<FSM event 38 */
    /* SPR 17784 End */

    /* SPR 9216:MRO enhancement end */
    /*SPR 11535:start*/
    RRC_RRM_UE_INACTIVE_EVENT,
    /*SPR 11535:end*/
/* OPTION3X Changes Start */ 
#ifdef ENDC_ENABLED
    RRC_RRM_UE_DC_BEARER_CHANGE_CNF_EVENT,
#endif
/* OPTION3X Changes End */

    RRM_UE_MAX_EVENT				/*!< FSM MAX event	*/
}rrm_ue_events_et;

/*! \enum rrm_ue_erb_states_et
 *  \brief An enum for RRM UE ERAB STATES
 */
typedef enum
{
    RRM_UE_ERB_NULL,				                    /*!< Procedure 0	*/
    
    /* SPR 15462 Fix Code Deleted */
    /*SPR 18241 START*/
    RRM_UE_ERB_SETUP_ONGOING,       /*!< Procedure 1    */
    RRM_UE_ERB_MODIFY_ONGOING,      /*!< Procedure 3    */
        /* TC_45 test fix */
    RRM_UE_ERB_RELEASE_ONGOING,      /*!< Procedure 3    */
        /* TC_45 test fix */
        /* TC_46 test fix */
    RRM_UE_ERB_RECONFIG_ONGOING,      /*!< Procedure 3    */
        /* TC_46 test fix */
    RRM_UE_HO_ADM_ONGOING,
    /*SPR 18241 END*/
    RRM_UE_CONTEXT_MOD_ONGOING,                         /*!< Procedure 8    */
    RRM_UE_RECONFIG_ONGOING,                            /*!< Procedure 9    */
    /*  DYNAMIC ICIC CHANGES START  */
    RRM_UE_ICIC_UPDATE_INFO_ONGOING,                    /*!< Procedure 10   */
    /*  DYNAMIC ICIC CHANGES END  */
    /*  LIPA SIPTO START */
    RRM_UE_LIPA_SIPTO_HO_ONGOING,                       /*< Procedure 11    */
    /*  LIPA SIPTO START */
    /* CSR00069389 Start */
    RRM_UE_MEAS_CONFIG_REQ_ONGOING,                     /*< Procedure 12    */
    RRM_UE_MEAS_RESULTS_AWAITED,                        /*< Procedure 13    */
    /* CSR00069389 End */
    RRM_UE_SCELL_PROCEDURE_ONGOING,	            /*!< Procedure 14   */
    /* eICIC_PHASE_1_2_CHANGES_START */
    RRM_UE_RECONFIG_GOING_FOR_EICIC,		            /*!< Procedure 15   */
    /* eICIC_PHASE_1_2_CHANGES_END */
    /* SPR_16053_Fix: Start */
    RRM_UE_REESTABLISH_PROCEDURE_ONGOING,               /*!< Procedure 16   */
    /* SPR_16053_Fix: End */
    /*SPR 18241 START*/
    RRM_UE_SRC_HO_ONGOING,                /*!< Procedure 17 */
    /*SPR 18241 END*/
/* OPTION3X Changes Start */
#ifdef ENDC_ENABLED
    RRM_UE_DC_BEARER_CHANGE_PROCEDURE_ONGOING,
#endif
/* OPTION3X Changes End */
 RRM_UE_ERB_MAX_STATE			
}rrm_ue_erb_states_et;


/* CSR00069389 Start */
typedef struct _rrm_ue_meas_config_cgi_queue_node
{
    rrm_rat_type_et     rat_type;
    U16                 arfcn;
    U8                  band_ind;
    U8                  band_class;
    U16                 cgi;
}rrm_ue_meas_config_cgi_queue_node_t;
/* CSR00069389 End */

/*! \  struct rrm_uem_s_tmsi_t
 *  \brief TMSI info
 */
typedef char rrm_uem_s_tmsi_t[RRM_UEM_S_TMSI_SIZE]; 

/*! \  struct rrm_ue_cause_t
 *  \brief UE Cause
 *  \param U8   type
 *  \param U16  value
 */
typedef struct
{
    U8   type;
    U16  value;
} rrm_ue_cause_t;

/*! \  struct rrm_ue_procedure_queue_t
 *  \brief Internal procedure queue for messages coming from RRC
 *  \param U16 api_id
 *  \param void *p_proc_data
 */
typedef struct _rrm_ue_procedure_queue_t
{
    U16							api_id;				/*!< API ID for message from RRC 	*/
    /*BUG 13332:start*/
    U16                         trans_id;           /*!< Transaction ID                 */
    /*BUG 13332:end*/
    void						*p_proc_data;		/*!< Pointer to procedure data		*/
}rrm_ue_procedure_queue_t;

/*! \  struct rrm_ue_procedure_record_t
 *  \brief A queue node for the incoming messages 
 *  \param YLNODE s_node
 *  \param rrm_ue_procedure_queue_t proc_queue
 */
typedef struct  _rrm_ue_procedure_record_t
{
    YLNODE						s_node;				/*!< CSPL node for queue			*/
    rrm_ue_procedure_queue_t	proc_queue;			/*!< Object for procedure queue 	*/
}rrm_ue_procedure_record_t;

/*! \  procedure queue list 
 *  \brief Procedure queue Head
 */ 
typedef YLIST rrm_ue_proc_queue_t;

/*! \  struct rrm_ue_erab_failed_item_t
 *  \brief structure storing the details od failed ERAB
 *  \param U8 erab_id
 *  \param rrm_ue_cause_t cause
 */
typedef struct _rrm_ue_erab_failed_item_t
{
    U8                 		erab_id;			/*!< ERAB ID for failed RABs		*/	
    rrm_ue_cause_t        		cause;				/*!< Cause for failed RABs			*/
}rrm_ue_erab_failed_item_t;

/*! \  struct rrm_ue_erab_failed_item_list_t
 *  \brief List containg all the the failed ERABs during setup/modify
 *  \param U16 num_of_list
 *  \param rrm_ue_erab_failed_item_t erab_to_be_release_item[RRM_MAX_NUM_DRB_PER_UE]
 */
typedef struct _rrm_ue_erab_failed_item_list_t
{
    U16                    	num_of_list;		/*!< Number of failed item			*/
    rrm_ue_erab_failed_item_t	erab_to_be_release_item[RRM_MAX_NUM_DRB_PER_UE];
    /*!< ERAB to be release item 		*/
} rrm_ue_erab_failed_item_list_t;

/*! \  struct rrm_ue_allocation_and_retention_priority_t
 *  \brief Allocation and Retention priority parameters
 *  \param U8 priority_level
 *  \param U8 preemption_capability
 *  \param U8 preemption_vulnerability
 */
typedef struct _rrm_ue_allocation_and_retention_priority_t
{
    U8        					priority_level;		/*!< Prioriry level set 			*/           
    U8        					preemption_capability;
    /*!< Preemption capability 			*/
    U8        					preemption_vulnerability;
    /*!< preemption vulnerability		*/
} rrm_ue_allocation_and_retention_priority_t;

/*! \  struct rrm_ue_gbr_qos_info_t
 *  \brief Struct for GBR QOS related parameters
 *  \param U64 erab_max_bit_rate_dl
 *  \param U64 erab_max_bit_rate_ul
 *  \param U64 erab_guar_bit_rate_dl
 *  \param U64 erab_guar_bit_rate_ul
 */
typedef struct  _rrm_ue_gbr_qos_info_t
{
    U64      						erab_max_bit_rate_dl;
    /*!< Erab NGBR DL bit rate			*/   
    U64      						erab_max_bit_rate_ul;   
    /*!< Erab NGBR UL bit rate			*/
    U64      						erab_guar_bit_rate_dl;  
    /*!< Erab GBR DL bit rate			*/
    U64      						erab_guar_bit_rate_ul;
    /*!< Erab GBR UL bit rate			*/  
} rrm_ue_gbr_qos_info_t;


/*! \  struct rrm_ue_erab_level_qos_params_t
 *  \brief Struct for Quality of Service related parameters for a DRB 
 *  \param U16 bitmask
 *  \param U8 qci
 *  \param rrm_ue_allocation_and_retention_priority_t alloc_and_reten_prior
 *  \param rrm_ue_gbr_qos_info_t gbr_qos_info
 */
typedef struct _rrm_ue_erab_level_qos_params_t
{
    U16               bitmask;   			/*!< Bitmask QoS param stucture		*/   
#define ERAB_LEVEL_QOS_PARAM_GBR_QOS_INFO_PRESENT     0x01
    U8                          qci;          		/*!< QCI parameter for profiling	*/
    rrm_ue_allocation_and_retention_priority_t       alloc_and_reten_prior;
    /*!< Parameter for alloc and prio   */
    rrm_ue_gbr_qos_info_t            gbr_qos_info;		/*!< GBR QoS information			*/
} rrm_ue_erab_level_qos_params_t;

/*! \  struct rrm_ue_erab_setup_item_t
 *  \brief Strcut storing the information of ERB 
 *  \param U8 erab_id
 *  \param U8 drb_id
 *  \param U8 lc_id
 *  \param U8 dl_fwding
 *  \param rrm_ue_erab_level_qos_params_t erab_level_qos_params
 */
/* SPR 16500 Fix Start */
typedef enum {
    QCI_5 = 1,
    QCI_7,
    QCI_9,
    QCI_8,
    QCI_6
}rrm_ue_max_priority_qci_for_emergency_et;
/* SPR 16500 Fix Stop */
#define ECN_DL_BIT_PRESENT 0x01
#define ECN_UL_BIT_PRESENT 0x02
/* SPR 10729 start */
#define RRM_UEM_ERAB_TO_BE_SETUP_LIPA_RAB_IND_PRESENT_FLAG 0x04
/* SPR 10729 end */
#ifdef ENDC_ENABLED
#define RRM_UEM_ERAB_TO_BE_SETUP_DRB_TYPE_PRESENT_FLAG 0x08
#endif
typedef struct _rrm_ue_erab_setup_item_t
{
    U16                           bitmask;
    U8                            erab_id;			/*!< ERAB ID for setup item list	*/
    U8                            drb_id;			/*!< DRB ID for erab	*/
    U8                            lc_id;
    U8                            dl_fwding;		/*!< DL forwarding 					*/
    rrm_ue_erab_level_qos_params_t   erab_level_qos_params;
    /*!< ERAB level QoS parameters		*/
    U8                            data_fwding_not_possible;
    /*SPR_17817_Fix_Start*/
    /*!< data_fwding_not_possible is 0 when  ERAB_TO_BE_SETUP_DATA_FWDING_NOT_POSSIBLE_INFO_PRESENT_FLAG
     * bitmask is set, otherwise its 1 */
    /*SPR_17817_Fix_End*/
    U8                            lipa_rab_ind;
    U8                              data_transfer_mode;
    /*ECN start*/
    U8 direction_of_congestion;
    /*ECN end*/
    /* SPR 16500 Fix Start */
    rrm_ue_max_priority_qci_for_emergency_et 
				  emergency_arp_ngbr_priority;
    /* SPR 16500 Fix Stop */
#ifdef ENDC_ENABLED
    rrm_drb_type_et               drb_type; /* rrm_drb_type_et */
/* OPTION3X Changes Start */
    /* new_lc_id is used during DC_BEARER_CHANGE for OPTION3X for temporarily storing
     * the LD ID during transition. Once the DC_BEARER_CHANGE_CNF is received new _lc_id
     * is copied to lc_id and new_lc_id is set to zero.
     */
    U8                            new_lc_id;
    /* new_drb_id is used during DC_BEARER_CHANGE for OPTION3X for temporarily storing
     * the DRB ID during transition. Once the DC_BEARER_CHANGE_CNF is received new_drb_id
     * is copied to drb_id and new_drb_id is set to zero.
     */
    U8                            new_drb_id;
/* OPTION3X Changes End */
#endif
} rrm_ue_erab_setup_item_t;

/*! \  struct  rrm_ue_erab_setup_item_list_t
 *  \brief List of ERBs for a UE
 *  \param U16 num_of_list 
 *  \param rrm_ue_erab_setup_item_t erab_item[RRM_MAX_NUM_DRB_PER_UE]
 *  \param U8 num_um_drb
 *  \param U8 num_am_drb
 */
typedef struct	_rrm_ue_erab_setup_item_list_t
{
    /* SPR 18008 Fix Start */ 
    U8                              num_um_drb;
    U8                              num_am_drb;
    U16                      		num_of_list;  		/*!< Number of list in setup		*/ 
    rrm_ue_erab_setup_item_t        erab_item[RRM_MAX_NUM_DRB_PER_UE];
    /*!< List of erab item in setup list*/ 
    /* SPR 18008 Fix End */ 
} rrm_ue_erab_setup_item_list_t;

/*SPR 20523 START*/
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_24_DB      -24
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_22_DB      -22
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_20_DB      -20
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_18_DB      -18
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_16_DB      -16
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_14_DB      -14
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_12_DB      -12
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_10_DB      -10
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_8_DB       -8
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_6_DB       -6
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_5_DB       -5
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_4_DB       -4
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_3_DB       -3
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_2_DB       -2
#define RRM_EUTRAN_FREQ_OFFSET_MINUS_1_DB       -1
#define RRM_EUTRAN_FREQ_OFFSET_0_DB             0
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_1_DB        1
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_2_DB        2
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_3_DB        3
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_4_DB        4
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_5_DB        5
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_6_DB        6
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_8_DB        8
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_10_DB       10
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_12_DB       12
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_14_DB       14
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_16_DB       16
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_18_DB       18
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_20_DB       20
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_22_DB       22
#define RRM_EUTRAN_FREQ_OFFSET_PLUS_24_DB       24
/*SPR 20523 END*/


/* CSR-79374[SPR-11578]-fix start */
#define RRM_UE_RELEASE_CAUSE_PRESENCE_FLAG  0x01

/*! \  struct  rrm_ue_release_req_t
 *  \brief Cause for the UE release API. 
 *  \param rrc_bitmask_t bitsmask
 *  \param rrm_cause_t   cause
 */

typedef struct
{
    rrm_bitmask_t               bitmask;       /*!< BITMASK */
    rrm_cause_t                 cause ; 
    /*^ O, RRM_UE_RELEASE_CAUSE_PRESENCE_FLAG, N, 0, 0 ^*/
} rrm_ue_release_req_t;
/* CSR-79374[SPR-11578]-fix end */

typedef struct _rrm_ue_ambr_ext_t
{
    U64       						ue_agg_max_bit_rate_ul_ext;  
    /*!< AMBR for in UL					*/
    U64       						ue_agg_max_bit_rate_dl_ext;
    /*!< AMBR for in DL 				*/  
}rrm_ue_ambr_ext_t;

/*! \  struct rrm_ue_ambr_t
 *  \brief Struct storing the aggregate maximum bit rate
 *  \param U64 ue_agg_max_bit_rate_ul
 *  \param U64 ue_agg_max_bit_rate_dl
 */
#define RRM_EXT_UE_AGG_MAX_BITRATE_EXT_PRESENT    0x01
typedef struct _rrm_ue_ambr_t
{
    rrm_bitmask_t                   bitmask;
    /*!< BITMASK */

    U64       						ue_agg_max_bit_rate_ul;  
    /*!< AMBR for in UL					*/
    U64       						ue_agg_max_bit_rate_dl;
    /*!< AMBR for in DL 				*/  

    rrm_ue_ambr_ext_t               ue_ambr_ext;
} rrm_ue_ambr_t;

/* CS-Fallback changes starts */

/*! \  struct rrm_uem_lac_t
 *  \brief Structure for storing the Location area code info
 *  \param U32 num
 *  \param U8 data[RRM_UEM_MAX_LAC_NUMOCTS]
 */
typedef struct _rrm_uem_lac_t
{
    U32 num;    /*^ M, 0, H, 0, 2 ^*/
    U8  data[RRM_UEM_MAX_LAC_NUMOCTS];/*^ M, 0, OCTET_STRING, FIXED ^*/
}rrm_uem_lac_t;

/*! \  struct rrm_uem_plmn_identity_lai_t
 *  \brief Structure for storing the Location Area PLMN ID
 *  \param U32 num
 *  \param U8 data[RRM_UEM_MAX_PLMN_ID_NUMOCTS]
 */
typedef struct _rrm_uem_plmn_identity_lai_t
{
    U32 num;    /*^ M, 0, H, 0, 3 ^*/
    U8  data[RRM_UEM_MAX_PLMN_ID_NUMOCTS];/*^ M, 0, OCTET_STRING, FIXED ^*/
}rrm_uem_plmn_identity_lai_t;

/*! \  struct rrm_uem_registered_lai_t
 *  \brief Structure for storing the Registered  Location Area ID information
 *  \param rrm_uem_plmn_identity_lai_t plmn_identity
 *  \param rrm_uem_lac_t lac
 */
typedef struct _rrm_uem_registered_lai_t
{
    rrm_uem_plmn_identity_lai_t plmn_identity;  /*^ M, 0, N, 0, 0 ^*/
    rrm_uem_lac_t           lac;                /*^ M, 0, N, 0, 0 ^*/
}rrm_uem_registered_lai_t;

/*csg start*/
/*! \ rrm_temp_cell_info_t
 *  \brief This struct is used for temp cell information
 *  \param cell_access_mode Cell Access Mode
 *  \param csg_id[NUM_CSG_OCTETS] CSG identity
 */
typedef struct
{
    U16 bitmask;
#define RRM_UEM_TEMP_CELL_ACCESS_MODE_PRESENT   0x01
#define RRM_UEM_TEMP_CSG_ID_PRESENT   0x02
    /* Fix 10029 Start */
#define RRM_UEM_TEMP_CELL_ID_PRESENT 0x04
    /* Fix 10029 End */
    rrm_oam_access_mode_et cell_access_mode; 
    /*O,RRM_UEM_TEMP_CELL_ACCESS_MODE_PRESENT*/
    U8                           csg_id[NUM_CSG_OCTETS];
    /*O,RRM_UEM_TEMP_CSG_ID_PRESENT*/
    /* Fix 10029 Start */
    /* SPR 10137 Start */
    U32       cell_id;
    /* SPR 10137 End */
    /* O, RRM_UEM_TEMP_CELL_ID_PRESENT */
    /* Fix 10029 End */

}rrm_temp_cell_info_t;
/*csg end*/


#define  RRM_UEM_CTX_SPID_PRESENT             0x01
#define  RRM_UEM_CTX_CSF_INDICATOR_PRESENT    0x02
#define  RRM_UEM_CTX_REGISTERED_LAI_PRESENT   0x04
/*csg start*/
#define  RRM_UEM_CONTXT_MOD_CSG_MEMBERSHIP_STATUS_PRESENT 0x08
/*csg end*/
/*! \  struct rrm_ue_context_mod_req_t
 *  \brief structure storing the ue context modification request
 *  \param U16 bitmask
 *  \param U16 spid
 *  \param U8 csf_indicator
 *  \param rrm_uem_registered_lai_t registered_lai
 */
typedef struct _rrm_ue_context_mod_req_t
{
    U16                   bitmask;
    U16                             spid;
    U8                              csf_indicator;
    rrm_uem_registered_lai_t        registered_lai;
    /*csg start*/
    rrm_csg_membership_status_et    csg_membership_status;
    /*csg end*/
}rrm_ue_context_mod_req_t;

/* SPR 22553 FIXED START*/

typedef struct _rrm_ue_ctxt_mod_info
{

    U16                   bitmask;
    U8                              csf_indicator;
    rrm_uem_registered_lai_t        registered_lai;
    /*csg start*/
    rrm_csg_membership_status_et    csg_membership_status;
    /*csg end*/
}rrm_ue_ctxt_mod_info_t;
/* SPR 22553 FIXED END*/

/* SPR 22553 FIXED START*/
 #define  RRM_UEM_CTX_TRANSE_PID_PRESENT             0x01
typedef struct _rrm_sub_profile_id_info
{

    U16                   bitmask;
    U16                          sub_profile_id;/*!<  sub_profile_id          */
    U16                          trans_sub_profile_id;/*!< trans_sub_profile_id  RRM_UEM_CTX_TRANSE_PID_PRESENT */

}rrm_sub_profile_id_info_t;
/* SPR 22553 FIXED END*/

/*! \  struct rrm_ue_context_mod_cnf_t
 *  \brief Structure for UE context modification Confirmation
 *  \param U16 ue_index
 *  \param U8 response
 */
typedef struct _rrm_ue_contxt_mod_cnf_t
{
    U16                                     ue_index;
    U8                                      response;
}rrm_ue_contxt_mod_cnf_t;
/* CS-Fallback changes ends */

/*! \  struct rrm_ue_erab_to_be_release_item_t
 *  \brief Structure storing the info if erab to be released with its cause
 *  \param U8 erab_id
 *  \param rrm_ue_cause_t cause
 */
typedef struct _rrm_ue_erab_to_be_release_item_t
{
    U8                         	erab_id;			/*!< ERAB ID 						*/
    rrm_ue_cause_t        			cause;				/*!< ERAB release cause				*/
} rrm_ue_erab_to_be_release_item_t;

/*! \  struct rrm_ue_erab_to_be_released_item_list_t
 *  \brief List of ERABs to be released
 *  \param U16 num_of_list
 *  \param rrm_ue_erab_to_be_release_item_t
 erab_to_be_release_item[RRM_UE_MAX_ERB_COUNT]
 */
typedef struct _rrm_ue_erab_to_be_released_item_list_t
{
    U16                      		num_of_list;		/*!< Number of list in released list */
    rrm_ue_erab_to_be_release_item_t  erab_to_be_release_item[RRM_UE_MAX_ERB_COUNT];
    /*!< ERAB to release item			 */
} rrm_ue_erab_to_be_released_item_list_t;

/*! \  struct rrm_ue_proc_erab_release_data_t
 *  \brief Structure for holding the data required for releasing a ERAB
 *  \param rrm_ue_ambr_t ambr
 *  \param rrm_ue_erab_to_be_released_item_list_t erab_setup_list
 */
typedef struct _rrm_ue_proc_erab_release_data_t
{
    rrm_ue_ambr_t  					ambr;				/*!< AMBR for erab release data 	*/
    rrm_ue_erab_to_be_released_item_list_t  erab_setup_list;
    /*!< ERAB setup released list		*/
} rrm_ue_proc_erab_release_data_t;

/*! \  struct rrm_ue_erab_item_t
 *  \brief ERAB ID
 *  \param U8 erab_id
 */
typedef struct _rrm_ue_erab_item_t
{
    U8      						erab_id;   			/*!< Erab ID ERAB item				*/
}rrm_ue_erab_item_t;

/*! \  struct rrm_ue_erab_cnf_list_t
 *  \brief  List of all the Confirmed ERABs
 *  \param U8 erab_count
 *  \param rrm_ue_erab_item_t erab_cnf_info[RRM_UE_MAX_ERB_COUNT]
 */
typedef struct _rrm_ue_erab_cnf_list_t
{

    U8       			   			erab_count; 		/*!< Erab count						*/
    rrm_ue_erab_item_t     			erab_cnf_info[RRM_UE_MAX_ERB_COUNT];
    /*!< Erab conf information			*/
} rrm_ue_erab_cnf_list_t;

/*! \  struct rrm_ue_erab_error_info_t
 *  \brief Struct for all ERAB related error
 *  \param U8 erab_id
 *  \param U32 error_code
 */
typedef struct
{
    U8       						erab_id;			/*!< ERAB id for error code			*/
    U32      						error_code;			/*!< error code for error info		*/
} rrm_ue_erab_error_info_t;

/*! \  struct rrm_ue_erab_error_list_t
 *  \brief The List of erab errors
 *  \param U8 erab_count
 *  \param rrm_ue_erab_error_info_t
 erab_error_info[RRM_UE_MAX_ERB_COUNT]
 */
typedef struct _rrm_ue_erab_error_list_t
{
    U8                        		erab_count; 		/*!< ERAB count for error list		*/
    rrm_ue_erab_error_info_t  		erab_error_info[RRM_UE_MAX_ERB_COUNT];
    /*!< Error info list				*/
} rrm_ue_erab_error_list_t;


/*SPR 10329:start*/
typedef struct 
{
       plmn_identity_t    plmn_identity;
       U8                 mme_group_id[MAX_MME_GRP_BYTES]; /*!< MME GROUP LIST      */ 
       U8                 mmec;  
}rrm_mme_sel_gummei_info_t;

#define RRM_UEM_ERAB_SETUP_CNF_GUMMEI_INFO_PRESENT  0x01
/*SPR 10329:end*/

/* SPR_21459 Fix - Start */
#define RRM_UEM_ERAB_SETUP_CNF_MME_ID_PRESENT       0x02
/* SPR_21459 Fix - End */

/*! \  struct rrm_ue_proc_erab_cnf_data_t
 *  \brief Structure for confirmed erab related data 
 *  \param rrm_ue_erab_cnf_list_t erab_cnf_list
 *  \param rrm_ue_erab_error_list_t erab_error_list
 *  \param rrm_bool_et sps_error
 */
typedef struct _rrm_ue_proc_erab_cnf_data_t
{
    /*SPR 10329:start*/
    rrm_bitmask_t                   bitmask; /*!< BITMASK  */ 
    /*SPR 10329:end*/
    /* SPS related changes start */
    rrm_bool_et                     sps_error;          /*!< SPS ERAB error                 */ 
    /* SPS related changes end */
    rrm_ue_erab_cnf_list_t       	erab_cnf_list;		/*!< ERAB conf list					*/
    rrm_ue_erab_error_list_t     	erab_error_list;	/*!< ERAB error list 				*/
    /* SPR 19968 Start */ 
    rrm_erab_release_cnf_list_t erab_rel_cnf_list;      /*!< ERAB rel list                   */
    /* SPR 19968 End */
    /*SPR 10329:start*/
    rrm_mme_sel_gummei_info_t       gummei_info;        /*!< GUMMEI info                    */ 
    /*SPR 10329:end*/
    /* SPR_21459 Fix - Start */
    U8                          mme_id;                 /*!< MME_ID */
    /* SPR_21459 Fix - End */
} rrm_ue_proc_erab_cnf_data_t;

/*! \  struct rrm_ue_ho_erab_item_list_t
 *  \brief Structure for hand over ERAB item list 
 *  \param U16 count
 *  \param U16 erab_id[MAX_ERAB_COUNT]
 */
typedef struct _rrm_ue_ho_erab_item_list_t
{
    U16        count; /*!< COUNT          */
    U16        erab_id[MAX_ERAB_COUNT]; /*!<  erab id        */
}rrm_ue_ho_erab_item_list_t;


/* OPTION3X Changes Start */
#ifdef ENDC_ENABLED
/*! \  struct rrm_ue_proc_dc_bearer_change_cnf_data_t
 *  \brief Structure for confirmed erab change related data 
 *  \param rrm_ue_erab_cnf_list_t erab_cnf_list
 *  \param rrm_ue_erab_error_list_t erab_error_list
 */
typedef struct _rrm_ue_proc_dc_bearer_change_cnf_data_t
{
    U16                                bitmask; /*!<   BITMASK */
#define RRM_UEM_DC_BEARER_CHANGE_CNF_ERAB_CNF_LIST_PRESENT             0x01
#define RRM_UEM_DC_BEARER_CHANGE_CNF_ERAB_ERROR_LIST_PRESENT           0x02
    U16                         ue_index;
    U8                         response; 
    /*^ M, 0, H, 0, 2 ^*/
    rrm_ue_erab_cnf_list_t       	erab_cnf_list;		/*!< ERAB conf list					*/
    rrm_ue_erab_error_list_t     	erab_error_list;	/*!< ERAB error list 				*/
} rrm_ue_proc_dc_bearer_change_cnf_data_t;

#endif
/* OPTION3X Changes End */


/* LIPA start */
typedef enum
{
    RRM_GTP_ERROR_IND = 1,
    RRM_GTP_PATH_FAIL,
    RRM_OAM_INITIATED_LGW_DEL
}rrm_local_error_ind_cause_et;

/*! \  struct rrm_ue_local_error_erab_id_list_t
 *  \brief Structure for impacted rab
 *  \param U8   erab_id
 */
typedef struct _rrm_ue_local_error_erab_id_list_t
{
    U8                         count;                      /*!< total count of rab id    */
    U8                         erab_id[RRM_MAX_NUM_DRB_PER_UE];       /*!< rab id list    */
}rrm_ue_local_error_erab_id_list_t;

/*! \  struct rrm_ue_local_error_ind_t
 *  \brief Structure for Local error indication
 *  \param U16 bitmask
 *  \param U8  cause
 *  \param rrm_ue_impacted_items_t impacted_items
 */
typedef struct _rrm_ue_proc_local_error_ind_t
{
    U8                                    cause;             /*!< rrm_local_error_ind_cause_et   */
   /* SPR 10729 start */
    rrm_ue_index_t                        ue_index;          /*!< ue index    */
   /* SPR 10729 end */
    rrm_ue_local_error_erab_id_list_t     erab_id_list;      /*!< list of rab ids    */
}rrm_ue_proc_local_error_ind_t; 
/* LIPA end */


/*! \  struct rrm_ue_proc_ho_adm_cnf_data_t
 *  \brief Structure for Handover admission confirm Data
 *  \param U8 response
 *  \param rrm_ue_ho_erab_item_list_t erab_item_list
 *  \param rrm_ue_ho_erab_item_list_t erab_failed_item_list
 */
typedef struct _rrm_ue_proc_ho_adm_cnf_data_t
{
    U8      response; /*!< Response    Success/failure  */
    rrm_ue_ho_erab_item_list_t  erab_item_list; /*!< erab_item_list */
    /* SPS related changes start */
    rrm_bool_et                 sps_error; /*!sps_error true/false */
    rrm_ue_ho_erab_item_list_t  erab_failed_item_list; /*!< erab_failed_item_list */
    /* SPS related changes end */
}rrm_ue_proc_ho_adm_cnf_data_t;

/*! \  struct rrm_mnc_t
 *  \brief  Structure for storing the MNC information
 *  \param U8 count
 *  \param U8 mnc[MAX_MNC_OCTET_SIZE]
 */
typedef struct _rrm_mnc_t
{
    U8        count;   /*!< count               */
    U8        mnc[MAX_MNC_OCTET_SIZE];  /*!< mnc         */
}rrm_mnc_t;

/*! \  struct rrm_plmn_identity_t
 *  \brief PLMN related information
 *  \param U16 presence_bitmask
 *  \param U8 mcc[MCC_OCTET_SIZE]
 *  \param rrm_mnc_t mnc
 */
typedef struct _rrm_plmn_identity_t
{
    U16       presence_bitmask; /*!< presence_bitmask  */
    U8        mcc[MCC_OCTET_SIZE];  /*!<mcc           */
    rrm_mnc_t     mnc; /*!<  mcc          */
}rrm_plmn_identity_t;

/*! \  struct rrm_ho_cell_global_id_eutra_t
 *  \brief Structure for storing the HO ECGI
 *  \param rrm_plmn_identity_t plmn_identity
 *  \param  U8 cell_identity[4]
 */
typedef struct _rrm_ho_cell_global_id_eutra_t
{
    rrm_plmn_identity_t plmn_identity;  /*!< plmn_identity    */
    U8    cell_identity[4]; /*!<   cell_identity    */
}rrm_ho_cell_global_id_eutra_t;

/*! \  struct rrm_ho_cell_global_id_utra_t
 *  \brief structure for storing the HO CGI for UTRA
 *  \param rrm_plmn_identity_t plmn_identity
 *  \param U8 cell_identity[4]
 */
typedef struct _rrm_ho_cell_global_id_utra_t
{
    rrm_plmn_identity_t plmn_identity;  /*!< plmn_identity    */
    U8    cell_identity[4]; /*!<   cell_identity    */
}rrm_ho_cell_global_id_utra_t;

/*! \  struct rrm_eutran_type_t
 *  \brief Cell size information
 *  \param U8 cell_size
 */
typedef struct _rrm_eutran_type_t
{
    U8 cell_size; /*!<   cell_size    */
}rrm_eutran_type_t;

/*! \  struct rrm_last_visited_utran_cell_information_t
 *  \brief Structure for storing the Last visited UTRAN cell information
 *  \param rrm_ho_cell_global_id_utra_t global_cell_id
 *  \param rrm_eutran_type_t cell_type
 *  \param U16 time_ue_stayed_in_cell
 */
typedef struct _rrm_last_visited_utran_cell_information_t
{
    rrm_ho_cell_global_id_utra_t global_cell_id; /*!<global_cell_id     */
    rrm_eutran_type_t          cell_type;/*!< cell_type              */
    U16                        time_ue_stayed_in_cell; /*!<  time_ue_stayed_in_cell    */
}rrm_last_visited_utran_cell_information_t;

/*! \  struct rrm_last_visited_e_utran_cell_information_t
 *  \brief Structure for storing the Last visited EUTRAN cell information 
 *  \param rrm_ho_cell_global_id_eutra_t global_cell_id
 *  \param rrm_eutran_type_t cell_type
 *  \param U16 time_ue_stayed_in_cell
 */
typedef struct _rrm_last_visited_e_utran_cell_information_t
{
    rrm_ho_cell_global_id_eutra_t global_cell_id; /*!<global_cell_id     */
    rrm_eutran_type_t          cell_type;/*!< cell_type              */
    U16                        time_ue_stayed_in_cell; /*!<  time_ue_stayed_in_cell    */
}rrm_last_visited_e_utran_cell_information_t;

/*! \  struct rrm_last_visited_cell_info_t
 *  \brief Structure for storing the Last visited Cell information
 *  \param U16 bitmask
 *  \param rrm_last_visited_e_utran_cell_information_t 
 last_visited_e_utran_cell_information
 *  \param rrm_last_visited_utran_cell_information_t
 last_visited_utran_cell_information
 */
typedef struct _rrm_last_visited_cell_info_t
{
    U16                                bitmask; /*!< BITMASK */
#define LAST_VISITED_CELL_INFO_E_UTRAN_CELL_INFORMATION_PRESENT 0X01
#define LAST_VISITED_CELL_INFO_UTRAN_CELL_INFORMATION_PRESENT 0X02
    rrm_last_visited_e_utran_cell_information_t last_visited_e_utran_cell_information;/*!< last_visited_e_utran_cell_information  */
    rrm_last_visited_utran_cell_information_t   last_visited_utran_cell_information;/*!< last_visited_utran_cell_information   */
}rrm_last_visited_cell_info_t;

/*! \  struct rrm_last_visited_cell_list_t
 *  \brief Structure for storing the list of last visited cell information
 *  \param U8 count
 *  \param rrm_last_visited_cell_info_t
 last_visited_cell_info[MAX_NO_OF_CELLS_UE_HISTORY]
 */
typedef struct _rrm_last_visited_cell_list_t
{
    U8 count; /*!<  Count        */
    rrm_last_visited_cell_info_t last_visited_cell_info[MAX_NO_OF_CELLS_UE_HISTORY]; /*!< last_visited_cell_info   */
}rrm_last_visited_cell_list_t;

/*! \  struct rrm_ue_history_t
 *  \brief structure for storing the UE history
 *  \param rrm_last_visited_cell_list_t last_visited_cell_list
 */
typedef struct _rrm_ue_history_t
{
    rrm_last_visited_cell_list_t last_visited_cell_list; /*!< last_visited_cell_list      */
}rrm_ue_history_t;

/*! \  struct rrm_forbidden_lac_t
 *  \brief structure for storing th forbidden LAC 
 *  \param U8 lac[2]
 */
typedef struct _rrm_forbidden_lac_t
{
    U8             lac[2];/*!<   lac    */
}rrm_forbidden_lac_t;

/*! \  struct rrm_forbidden_lac_list_t
 *  \brief structure for storing list of Forbidden LAC
 *  \param U16 count
 *  \param rrm_forbidden_lac_t forbidden_lac[MAX_NO_OF_FORBIDDEN_LACS]
 */
typedef struct _rrm_forbidden_lac_list_t
{
    U16               count; /*!<    count */
 /*SPR 20589 FIX START*/  
    rrm_forbidden_lac_t   *p_forbidden_lac; /*!< forbidden_lac   */
 /*SPR 20589 FIX END*/  
}rrm_forbidden_lac_list_t;

/*! \  struct rrm_forbidden_la_t
 *  \brief structure for storing forbidden LA 
 *  \param rrm_plmn_identity_t plmn_identity
 *  \param rrm_forbidden_lac_list_t forbidden_lacs_list
 */
typedef struct _rrm_forbidden_la_t
{
    rrm_plmn_identity_t         plmn_identity; /*!<    plmn_identity    */
    rrm_forbidden_lac_list_t    forbidden_lacs_list;/*!<   forbidden_lacs_list   */
}rrm_forbidden_la_t;

/*! \  struct rrm_forbidden_la_list_t
 *  \brief structure for storing list of forbidden LA
 *  \param U16 count
 *  \param rrm_forbidden_la_t forbidden_la[MAX_NO_OF_EPLMNS_PlUS_ONE]
 */
typedef struct _rrm_forbidden_la_list_t
{
    U16             count; /*!< count          */
 /*SPR 20589 FIX START*/  
    rrm_forbidden_la_t  *p_forbidden_la;/*!<  forbidden_la        */
 /*SPR 20589 FIX END*/  
}rrm_forbidden_la_list_t;

/*! \  struct rrm_forbidden_tac_list_t
 *  \brief structure for storing forbidden TAC
 *  \param U8 tac[2]
 */
typedef struct _rrm_forbidden_tac_t
{
    U8   tac[2]; /*!< tac         */
}rrm_forbidden_tac_t;

/*! \  struct rrm_forbidden_tac_list_t
 *  \brief structure for storing list of forbidden TAC
 *  \param U16 count
 *  \param rrm_forbidden_tac_t forbidden_tac[MAX_NO_OF_FORBIDDEN_TACS]
 */
typedef struct _rrm_forbidden_tac_list_t
{
    U16               count; /*!<    count   */
 /*SPR 20589 FIX START*/  
    rrm_forbidden_tac_t   *p_forbidden_tac;/*!<  forbidden_tac   */
 /*SPR 20589 FIX END*/  
}rrm_forbidden_tac_list_t;

/*! \  struct rrm_forbidden_ta_t
 *  \brief structure for storing forbidden TA
 *  \param rrm_plmn_identity_t plmn_identity
 *  \param rrm_forbidden_tac_list_t forbidden_tac
 */
typedef struct _rrm_forbidden_ta_t
{
    rrm_plmn_identity_t         plmn_identity; /*!<    plmn_identity    */
    rrm_forbidden_tac_list_t    forbidden_tac;/*!< forbidden_tac        */
}rrm_forbidden_ta_t;

/*! \  struct _rrm_forbidden_ta_list_t
 *  \brief structure for storing list of forbidden TA
 *  \param U16 count
 *  \param rrm_forbidden_ta_t forbidden_ta[MAX_NO_OF_EPLMNS_PlUS_ONE]
 */
typedef struct _rrm_forbidden_ta_list_t
{
    U16              count; /*!< count        */
 /*SPR 20589 FIX START*/  
    rrm_forbidden_ta_t   *p_forbidden_ta;/*!<   forbidden_ta   */
 /*SPR 20589 FIX END*/  
}rrm_forbidden_ta_list_t;

/*! \  struct rrm_equiv_plmn_t
 *  \brief structure for storing equivalent PLMN information
 *  \param rrm_plmn_identity_t plmn_identity
 */
typedef struct _rrm_equiv_plmn_t
{
    rrm_plmn_identity_t plmn_identity; /*!< plmn_identity     */
}rrm_equiv_plmn_t;

/*! \  struct rrm_equiv_plmn_list_t
 *  \brief structure for storing list equivalent PLMN
 *  \param U8 count
 *  \param rrm_equiv_plmn_t equiv_plmn[MAX_NO_OF_EQUIVALENT_PLMN_IDS]
 */
typedef struct _rrm_equiv_plmn_list_t
{
    U8            count; /*!< count         */
 /*SPR 20589 FIX START*/  
  rrm_equiv_plmn_t  *p_equiv_plmn;/*!< equiv_plmn     */
 /*SPR 20589 FIX END*/  
}rrm_equiv_plmn_list_t;

/*! \  struct rrm_ho_restriction_list_t
 *  \brief structure for storing HO restriction list
 *  \param U16 bitmask
 *  \param rrm_plmn_identity_t serving_plmn_identity
 *  \param rrm_equiv_plmn_list_t equiv_plmn_list
 *  \param rrm_forbidden_ta_list_t forbidden_ta_list
 *  \param rrm_forbidden_la_list_t forbidden_la_list
 *  \param U8 forbidden_inter_rat
 */
typedef struct _rrm_ho_restriction_list_t
{
    U16                                bitmask; /*!<   BITMASK */
#define HO_RESTRICTION_EQUIV_PLMN_LIST_PRESENT      0x01
#define HO_RESTRICTION_FORBIDDEN_TA_LIST_PRESENT    0x02
#define HO_RESTRICTION_FORBIDDEN_LA_LIST_PRESENT    0x04
#define HO_RESTRICTION_FORBIDDEN_INTER_RAT_PRESENT  0x08

    rrm_plmn_identity_t       serving_plmn_identity;/*!<  serving_plmn_identity   */
    rrm_equiv_plmn_list_t     equiv_plmn_list;/*!< equiv_plmn_list    */
    rrm_forbidden_ta_list_t   forbidden_ta_list;/*!< forbidden_ta_list  */
    rrm_forbidden_la_list_t   forbidden_la_list;/*!< forbidden_la_list   */
    U8                        forbidden_inter_rat;/*!< forbidden_inter_rat   */
}rrm_ho_restriction_list_t;

/*! \  struct rrm_ue_proc_ho_restriction_list_t 
 *  \brief structure for storing Procedure for HO restriction list
 *  \param rrm_ho_restriction_list_t ho_restriction_list
 */
typedef struct _rrm_ue_proc_ho_restriction_list_t
{
    rrm_ho_restriction_list_t ho_restriction_list; /*!< ho_restriction_list */
}rrm_ue_proc_ho_restriction_list_t;

#define  RRM_UEM_ERAB_SETUP_REQ_SPID_PRESENT             0x01
#define  RRM_UEM_ERAB_SETUP_REQ_CSF_INDICATOR_PRESENT    0x02
#define  RRM_UEM_ERAB_SETUP_REQ_REGISTERED_LAI_PRESENT   0x04
#define  RRM_UEM_ERAB_SETUP_REQ_AMBR_PRESENT             0x08
/*csg start*/
#define  RRM_UEM_ERAB_SETUP_REQ_CSG_MEM_STATUS_PRESENT   0x10
/*csg end*/
#define  RRM_UEM_ERAB_SETUP_REQ_HO_RESTRICTION_LIST_PRESENT 0x20
/* SRVCC SUPPORT START */
#define  RRM_UEM_ERAB_SETUP_REQ_SRVCC_OP_POSSIBLE_PRESENT   0x40
/* SRVCC SUPPORT END */

/*! \  struct rrm_ue_proc_erab_setup_data_t
 *  \brief Struct for ERB related data at setup time
 *  \param U16 bitmask
 *  \param rrm_ue_ambr_t ambr
 *  \param U16 spid
 *  \param U8 csf_indicator
 *  \param rrm_uem_registered_lai_t registered_lai
 *  \param rrm_ue_erab_setup_item_list_t erab_setup_list
 *  \param rrm_ho_restriction_list_t     ho_restriction_list
 */
typedef struct rrm_ue_proc_erab_setup_data_t
{
    U16                   bitmask;
    rrm_ue_ambr_t  					ambr;               /*!< AMBR parameters				*/
    U16                             spid;
    U8                              csf_indicator;
    rrm_uem_registered_lai_t        registered_lai;
    rrm_ue_erab_setup_item_list_t 	erab_setup_list;	/*!< ERAB UE setup item list		*/
    /*csg start*/
    rrm_csg_membership_status_et    csg_membership_status;
    /*csg end*/
    rrm_ho_restriction_list_t       ho_restriction_list;/*!<  ho_restriction_list    */
    /* SRVCC SUPPORT START */
    rrm_bool_et                     srvcc_op_possible;
    /* SRVCC SUPPORT END */
} rrm_ue_proc_erab_setup_data_t;


typedef struct rrm_dl_nas_transport_ind_t
{
    U16                        bitmask; /*^ BITMASK ^*/
#define RRM_DL_NAS_TRANSPORT_IND_SUBS_PROF_ID_PRESENT            0x01

    U16                        ue_index;
    U16                        subscriber_profile_id_for_rfp;
}rrm_dl_nas_transport_ind_t; 

typedef struct rrm_intra_enodeb_ho_ind_t
{
    U16                        bitmask; /*^ BITMASK ^*/
    U16                        ue_index;
}rrm_intra_enodeb_ho_ind_t;




/* SPS related changes start */

/*! \struct sps_config_dl_t
 *  \brief structure for storing the SPS downlink configuration
 *  \param U8 semi_persist_sched_interval_dl
 *  \param U8 number_of_conf_sps_processes
 *  \param U8 max_sps_harq_retx
 *  \param U8 expilcit_release_after
 *  \param U8 pucch_an_persist_list_count
 *  \param U16 n1_pucch_an_persist
 */
typedef struct _sps_config_dl_t
{
    U8    semi_persist_sched_interval_dl;
    U8    number_of_conf_sps_processes;
    U8    max_sps_harq_retx;
    U8    expilcit_release_after;
}sps_config_dl_t;

/*! \struct sps_config_ul_t
 *  \brief  structure for storing the SPS uplink configuration
 *  \param  U8 semi_persist_sched_interval_ul
 *  \param  U8 implicit_release_after
 *  \param  S8 p_zero_nominal_pusch_persistent
 *  \param  S8 p_zero_ue_pusch_persistent
 */

typedef struct _sps_config_ul_t
{
    U8    semi_persist_sched_interval_ul;
    U8    implicit_release_after;
    S8    p_zero_nominal_pusch_persistent;
    S8    p_zero_ue_pusch_persistent;
/* + SPS_TDD_Changes */
#ifdef TDD_MODE_FLAG
    U8   two_intervals_config; 
#endif
/* - SPS_TDD_Changes */
}sps_config_ul_t;

/*! \struct rrm_sps_config_t
 *  \brief  structure for storing the SPS uplink and downlink structures
 *  \param  sps_config_dl_t sps_config_dl
 *  \param  sps_config_ul_t sps_config_ul
 */
typedef struct _rrm_sps_config_t
{
    sps_config_dl_t   sps_config_dl;
    sps_config_ul_t   sps_config_ul;
} rrm_sps_config_t;

/*! \struct rrm_drx_config_t
 *  \brief  structure for storing the DRX parameters
 *  \param  U8 on_duration_timer
 *  \param  U8 drx_inactivity_timer
 */
typedef struct _rrm_drx_config_t
{
    U8    on_duration_timer;
    U8    drx_inactivity_timer;
}rrm_drx_config_t;

/* SPS related changes end */

/* Carrier_Aggregation_Start */
/*! \struct rrm_scell_Identification_t
 *  \brief  structure for storing Scell ID and DL carrier frequency 
 *  \param  U16 phy_cell_id 
 *  \param  U16 dl_carrier_freq
 */
typedef struct _rrm_scell_Identification_t
{
    U16   phy_cell_id; /*!< Physical cell ID */
    U16   dl_carrier_freq; /*!< DL Carrier frequency */            
}rrm_scell_Identification_t;

/*! \struct rrm_scell_to_add_mod_t
 *  \brief  structure for storing Scell info for addition modification 
 *  \param  rrm_scell_Identification_t   scell_id 
 */
#define RRM_SCELL_ADD_MOD_CELL_ID_PRESENT               0x01
typedef struct _rrm_scell_to_add_mod_t
{
    rrm_bitmask_t         bitmask;
    rrm_cell_index_t     scell_index; /*!< SCell index */
    rrm_scell_Identification_t   scell_id; /*!< Scell ID */
    /*^ O, RRM_SCELL_ADD_MOD_CELL_ID_PRESENT ^*/
}rrm_scell_to_add_mod_t; 

/*! \struct rrm_scell_to_add_mod_list_t
 *  \brief  structure for storing Scell list for addition modification 
 *  \param  U8 count
 *  \param  rrm_scell_to_add_mod_t rrm_scell_to_add_mod
 */
typedef struct _rrm_scell_to_add_mod_list_t
{
    U8                     count;   /*!< Scell count */ 
    rrm_scell_to_add_mod_t  rrm_scell_to_add_mod[RRM_MAX_SCELL]; /*!< Scell to add mod  */
}rrm_scell_to_add_mod_list_t;

/* Carrier_Aggregation_End */

/* CA: Stage3 HO code start */
#define CANDIDATE_SCELL_RSRP_PRESENT   0x01
#define CANDIDATE_SCELL_RSRQ_PRESENT   0x02
typedef struct _rrm_candidate_cell_info_t
{
    U8      bitmask;
    U16     pci;
    U16     carrier_freq;
    U8      rsrp;
    U8      rsrq;
}rrm_candidate_cell_info_t;
/* CA: Stage3 HO code end */

/*! \  struct rrm_ue_proc_ho_adm_data_t
 *  \brief structure for storing HO Adm procedure dara
 *  \param U8 ho_type
 *  \param rrm_ue_ambr_t ambr
 *  \param rrm_ue_erab_setup_item_list_t erab_setup_list
 *  \param U16 sub_profile_id
 *  \param rrm_ue_history_t ue_history
 *  \param U8 ue_inactivity_time
 *  \param rrm_ho_restriction_list_t ho_restriction_list
 *  \param rrm_ue_capability_params_t ue_capability_params
 *  \param rrm_meas_config_t meas_config
 *  \param rrm_csg_membership_status_et csg_membership_status
 *  \param rrm_sps_config_t sps_config
 *  \param rrm_drx_config_t drx_config
 *  \param U8 csg_identity[CSG_ID_OCTET_SIZE]
 *  \param rrm_scell_to_add_mod_list_t scell_add_mod_list
 */

/*csg start*/
#define RRM_HO_ADM_REQ_CSG_MEMBERSHIP_STATUS_PRESENT    0x01
#define RRM_HO_ADM_REQ_CSG_ID_PRESENCE_FLAG             0x02
#define RRM_AS_CONFIG_EXT_SCELL_ADD_MOD_LIST_PRESENT    0x04
#define RRM_SCELL_CANDIDATE_INFO_PRESENT                0x08
/*SPR 10329:start*/
#define RRM_UE_HO_ADM_REQ_GUMMEI_INFO_PRESENT           0x10
/*SPR 10329:end*/
/* SPR 14537 start */
#define RRM_HO_ADM_REQ_MEAS_SUBFRAME_PATTERN_PRESENT    0x20
/* SPR 14537 end */
/* SPR 20632 21459 START */
#define RRM_HO_ADM_REQ_MME_ID_PRESENT                   0x40

/* SPR 22553 FIXED START*/
#define RRM_HO_ADM_REQ_SUB_PROFILE_ID_PRESENT                   0x80
/* SPR 22553 FIXED END*/

/* SPR 20632 21459 END */
typedef struct _rrm_ue_proc_ho_adm_data_t
{
    /*csg start*/
    U16                             bitmask;
    /*csg end */
    U8                              ho_type;     /*!< Handover type           */
    rrm_ue_ambr_t                   ambr; /*!< AMBR parameters                              */
    rrm_ue_erab_setup_item_list_t   erab_setup_list;        /*!< ERAB UE setup item list            */
    U16                             sub_profile_id; /*!< subscriber Profile id        */
    ue_history_t                    ue_history;/*!< UE History      */
    /*SPR_19066_START*/
    /* Code deleted */
    /*SPR_19066_END*/
    rrm_ho_restriction_list_t       ho_restriction_list;/*!<  ho_restriction_list    */
    rrm_ue_capability_params_t      ue_capability_params;/*!< ue_capability_params   */
    rrm_meas_config_t               meas_config; /*!< ue_meas_config_parameters   */
    /* SRVCC SUPPORT START */
    rrm_bool_et                     srvcc_op_possible;
    /* SRVCC SUPPORT END */
    rrm_csg_membership_status_et    csg_membership_status;
    /* SPS related changes start */
    rrm_sps_config_t                sps_config;  /*!< sps_config data */
    rrm_drx_config_t                drx_config;  /*!< drx_config data */
    /* SPS related changes end */
    /* Rel10_CRs 420 start */
    U8                              csg_identity[CSG_ID_OCTET_SIZE]; /*!< CSG ID */
    /*^ O, RRM_HO_ADM_REQ_CSG_ID_PRESENCE_FLAG, N, 0, 0 ^*/
    /* Rel10_CRs 420 end */
    /* Carrier_Aggregation_Start */
    rrm_scell_to_add_mod_list_t     scell_add_mod_list; /*!< Scell addition modification list */
    /*^ O, RRM_AS_CONFIG_EXT_SCELL_ADD_MOD_LIST_PRESENT ^*/
    /* Carrier_Aggregation_End */
    /* CA: Stage3 HO code start */
    U8                              scell_candidate_count;
    rrm_candidate_cell_info_t       candidate_scell_info[RRM_EIGHT];
    /* CA: Stage3 HO code end */
    /*SPR 10329:start*/
    rrm_mme_sel_gummei_info_t       gummei_info; /*!< GUMMEI info     */
    /*SPR 10329:end*/
    /* SPR 20632 21459 START */
    U8                              mme_id;  /*^ O, RRM_HO_ADM_REQ_MME_ID_PRESENT  ^*/
    /* SPR 20632 21459 END */
} rrm_ue_proc_ho_adm_data_t;
/*csg end*/

/*! \  struct rrm_ue_procedure_t
 *  \brief An internal structure for storing the ERAB related data to be processed
 *         along with success and failure list
 *  \param rrm_ue_erb_states_et state
 *  \param void *p_proc_data
 *  \param void *p_erb_success_list
 *  \param void *p_erb_failed_list
 */
typedef struct _rrm_ue_procedure_t
{
    rrm_ue_erb_states_et    	state;					/*!< Procedural state				*/
    void                    	*p_proc_data;			/*!< Void pointer to proc data		*/
    /*CID 60105 Start*/
    rrm_ue_erab_setup_item_list_t                    	*p_erb_success_list;	/*!< Void pointer to Success list	*/
    /*CID 60105 End*/
    void                    	*p_erb_failed_list;		/*!< Void pointer to failed list	*/
} rrm_ue_procedure_t;

/*! \  struct rrc_ue_ul_sch_config_t
 *  \brief Structure for UL SCH config 
 *  \param U8 max_harq_tx
 *  \param U8 periodic_bsr_timer
 *  \param U8 retx_bsr_timer
 *  \param U8 tti_bundling
 */
typedef struct _rrc_ue_ul_sch_config_t
{
    U8                    		max_harq_tx;			/*!< MAX HARQ transmission			*/
    U8                    		periodic_bsr_timer;		/*!< Periodic BSR timer				*/
    U8                    		retx_bsr_timer_present;			/*!< Retransmission	BSR timer latest updated */
    U8                    		retx_bsr_timer_past;			/*!< Retransmission	BSR timer last updated	*/
    U8                    		tti_bundling;			/*!< TTI bundling informantion		*/
}rrc_ue_ul_sch_config_t;

/*! \  struct rrm_ue_cqi_paramas_t
 *  \brief Structure for cqi parameters
 *  \param U8 cqi_res_frame
 *  \param U8 cqi_res_slot
 *  \param U16 cqi_pucch_res_index
 *  \param U16 cqi_pmi_config_index
 */
typedef struct _rrm_ue_cqi_paramas_t
{
    U8							cqi_res_row;			/*!< CQI resolution frame			*/
    /* SPR 18008 Fix Start */ 
    U8                         cqi_pucch_res_index;	/*!< CQI PUCCH res index			*/
    U16							cqi_res_col;			/*!< CQI resolution	slot			*/
    /* SPR 18008 Fix End */ 
    /* SPR14123start */
    U16                         cqi_pmi_config_index;	/*!< CQI PMI config index			*/
    U16                         ri_index;               /*!< CQI RI index                   */
    U16                         cqi_pmi_config_index_2;	/*!< CQI PMI config index			*/
    U16                         ri_index_2;               /*!< CQI RI index                   */
    /* SPR14123end*/
}rrm_ue_cqi_paramas_t;

/*! \  struct rrm_ue_sr_paramas_t
 *  \brief Structure for SR Parameters
 *  \param U8 sr_res_frame
 *  \param U8 sr_res_slot
 *  \param U16 sr_pucch_res_index
 *  \param U8 sr_configuration_index
 */
typedef struct _rrm_ue_sr_paramas_t
{
    U8							sr_res_row;			/*!< SR resolution frame			*/
    /* SPR 18008 Fix Start */ 
    U8                         sr_pucch_res_index;		/*!< SR PUCCH res index				*/
    U8                          sr_configuration_index;	/*!< SR configuration index			*/
    U16							sr_res_col;			/*!< SR resolution slot				*/
    /* SPR 18008 Fix End */ 
}rrm_ue_sr_paramas_t;

/* SPR_8315_PART2_FIX_START */
typedef enum
{
    RRM_UE_SRS_TYPE_UNDEF,
    RRM_UE_SRS_TYPE_WIDEBAND ,
    RRM_UE_SRS_TYPE_SUBBAND
}rrm_ue_srs_res_type_et;
/* SPR_8315_PART2_FIX_END */

/* SRS Start */
/*! \struct rrm_ue_srs_params_t
 *  \brief  Structure for UE dedicated SRS parameters
 *  \param  U8  srs_bandwidth
 *  \param  U8  srs_hopping_bandwidth
 *  \param  U8  frequency_domain_position
 *  \param  U8  duration
 *  \param  U16 srs_configuration_index
 *  \param  U8  transmission_comb
 *  \param  U8  cyclic_shif
 */
typedef struct _rrm_ue_srs_params_t
{
    U8  srs_anchor;
    U8  srs_bandwidth;
    U8  srs_hopping_bandwidth;
    U8  frequency_domain_position;
    U8  duration;
    /* SPR 18008 Fix Start */ 
    U8  transmission_comb;
    U8  cyclic_shift;
    U16 srs_configuration_index;
    /* SPR 18008 Fix End */ 
    /* SPR_8315_PART2_FIX_START */
    rrm_ue_srs_res_type_et srs_res_type;
    /* SPR_8315_PART2_FIX_END */
}rrm_ue_srs_params_t;
/* SRS End */

/*! \ rrm_ue_block_params_t
 *  \brief Structure storing parameters for cell blocking
 *  \param rrm_ue_meas_config_req_sent U8
 */
typedef struct _rrm_ue_block_params_t
{
    /* SPR 18008 Fix Start */ 
    rrm_bool_t rrm_ue_meas_config_req_sent;
    /* SPR 18008 Fix End */ 
}rrm_ue_block_params_t;

/* SPS related changes start */

/*! \  rrm_ue_sps_data_t
 *  \brief structure for storing the SPS related parameters
 *  \param is_sps_erab_established rrm_bool_et
 *  \param sps_erab_highest_arp_priority U8
 *  \param sps_erab_highest_qci_priority U8
 *  \param sps_erab_id U8
 *  \param sps_config rrm_sps_config_t
 *  \param drx_config rrm_drx_config_t
 *  \param pucch_an_list_resource_index U16
 */
typedef struct _rrm_ue_sps_data_t
{
    /* SPR 18008 Fix Start */ 
    rrm_bool_t            is_sps_erab_established;        /* Checks whether an SPS RAB is already established for a particular UE or not */
    /* SPR 18008 Fix End */ 
    U8                    sps_erab_highest_arp_priority;  /* Contains the highest arp priority amongst the competitor RABs for SPS */
    U8                    sps_erab_highest_qci_priority;  /* Contains the highest qci priority amongst the competitor RABs for SPS */
    U8                    sps_erab_id;                    /* Contains the erab id of the RAB which will be given SPS configuration */
    rrm_sps_config_t      sps_config;                     /* sps_config data */
    rrm_drx_config_t      drx_config;                     /* drx_config data */
    U16                   pucch_an_list_resource_index;   /* contains the resource index which has been allocated to the a/n list in sps DL config*/
}rrm_ue_sps_data_t;
/* SPS related changes end */

/* UE Positioing changes start */
/*! \rrm_ue_ecid_meas_mac_data_t
 *  \brief structure for storing the meas data coming from MAC in UE context
 *  \param U16 sfn_for_enb_rxtx_diff_type1
 *  \param U16 enb_rxtx_diff_type1
 *  \param U16 sfn_for_enb_rxtx_diff_type2
 *  \param U16 enb_rxtx_diff_type2
 *  \param U16 sfn_for_AoA
 *  \param U16 AoA
 */
typedef struct _rrm_ue_ecid_meas_mac_data_t
{
    U16   sfn_for_enb_rxtx_diff_type1;
    U16   enb_rxtx_diff_type1;
    U16   sfn_for_enb_rxtx_diff_type2;
    U16   enb_rxtx_diff_type2;
    U16   sfn_for_AoA;
    U16   AoA[MAX_NUM_ANTENNA];
}rrm_ue_ecid_meas_mac_data_t;

typedef struct _rrm_ue_ecid_meas_eutran_nbr_data_t
{
    U16   pci;
    U8    rsrp;
    U8    rsrq;
}rrm_ue_ecid_meas_eutran_nbr_data_t;

/*! \rrm_ue_ecid_meas_ue_data_t
 *  \brief structure for storing the meas data coming from meas results ind in UE context
 *  \param meas_result_serv_cell
 *  \param eutran_count
 *  \param eutran_neighbor_list[8]
 *  \param meas_result_ecid_r9
 */
typedef struct _rrm_ue_ecid_meas_ue_data_t
{
    rrm_ue_meas_result_serv_cell_t      meas_result_serv_cell;                         /*!< Meas results of serving cell */
    U8                                  eutran_count;                                  /*!< No. of EUTRAN nbrs detected */
    rrm_ue_ecid_meas_eutran_nbr_data_t  eutran_neighbor_list[RRM_MAX_CELLS_REPORTED];  /*!< EUTRAN nbr info list */
    rrm_meas_result_ecid_r9_t           meas_result_ecid_r9;                           /*!< Meas results for ECID */
}rrm_ue_ecid_meas_ue_data_t;


/* SPR_8004:fix_start */
#define MAX_ALLOCATED_ID_FOR_ECID   20
/*! \rrm_ecid_meas_id_and_meas_obj_id_t
 *  \brief structure for storing meas_id, meas_object_id and report_config_id for ue_positioning
 *  \param meas_id 
 *  \param meas_object_id 
 *  \report_config_id
 */
typedef struct 
{
    U8                      meas_id;
    U8                      meas_object_id;
    U8                      report_config_id;

}rrm_ecid_meas_id_and_meas_obj_id_t;
/* SPR_8004:fix_end */

/*! \  rrm_ue_positioing_data
 *  \brief structure for storing the UE Positioing related data
 *  \param meas id          U8
 *  \param meas_object_id   U8
 *  \param report_config_id U8
 */
#define           RRM_ECID_UE_MEAS_REQD     0x01
#define           RRM_ECID_UE_MEAS_RECVD    0x02
#define           RRM_ECID_MAC_MEAS_REQD    0x04
#define           RRM_ECID_MAC_MEAS_RECVD   0x08
#define           RRM_ECID_MAC_TA1_REQD     0x01
#define           RRM_ECID_MAC_TA2_REQD     0x02
#define           RRM_ECID_MAC_AOA_REQD     0x04

typedef struct _rrm_ue_positioning_data
{
    /* SPR 18008 Fix Start */ 
    /* SPR_8004:fix_start */
    /* Contains meas_id meas_object_id and report_config_id of the measurement request for UE Positioing*/
    rrm_ecid_meas_id_and_meas_obj_id_t meas_id_and_meas_obj_id[MAX_ALLOCATED_ID_FOR_ECID];
    /* SPR_8004:fix_end */
    rrm_bool_t             ecid_meas_req_received;       /* Set True if ECID meas request is received */
    U8                     mme_id;                       /* mme_id received in E-CID meas request */
    U8                     routing_id;                   /* routing id of the E-CID meas request */
    U8                     ue_mac_meas_status;           /* Bitmask that contains UE and MAC meas status */
    U8                     e_smlc_ue_meas_id;            /* e_smlc_meas_id of E-CID meas request */
    U8                     num_meas_quantifier;
    U8                     ta1_ta2_aoa_req;     /* Which MAC qunatifiers is required */
    rrm_bool_t             ecid_meas_report_sent; /* Set true if ECID meas report is sent */
    rrm_bool_t             ue_meas_id_allocate;
    rrm_bool_t             ue_meas_send_for_rem;/* Whether meas config removal req has been sent to UE*/
    rrm_lppa_meas_quanify_item_et   meas_quantifier[RRM_ECID_MEAS_QUANTIFIERS];  /* contains the meas_quantifier received in E-CID meas request */
    U32                    lppa_trans_id;                /* lppa_trans_id of E-CID meas request */
    U32                    enb_ue_meas_id;               /* e_smlc_meas_id of E-CID meas request */
    U32                    report_type;                  /* report_typei(on demand or periodic) of E-CID meas request*/
    rrm_lppa_meas_peridicity_et report_interval;         /* Contains the periodicity of E-CID meas */
    rrm_timer_t            ue_pos_timer;                 /* contains timer info for ue positioning*/
    rrm_ue_ecid_meas_mac_data_t     ecid_meas_mac_data;  /* Data coming from MAC stored in UE context */
    rrm_ue_ecid_meas_ue_data_t      ecid_meas_ue_data;   /* Data coming from UE meas_results_ind stored in UE context */
    U32                             timer_expire_count;    /* Counter for timer_expire  */
    /* SPR 18008 Fix End */ 
    RRM_TIME_T                      ue_meas_config_sent_time;
}rrm_ue_positioing_data;
/* UE Positioing changes end */

/*CLPC START*/
/*! \ rrm_ue_clpc_params_t
 *  \brief This struct is used for UE CLPC params information
 *  \param tpc_rnti_pucch
 *  \param tpc_rnti_pusch
 *  \param tpc_rnti_index_pucch
 *  \param tpc_rnti_index_pusch
 *  \param dci_format_pucch
 *  \param dci_format_pusch
 *  \param sps_enabled_pucch
 *  \param sps_enabled_pusch
 */
typedef struct _rrm_ue_clpc_params_t
{
    U16                         tpc_rnti_pucch;
    U16                         tpc_rnti_pusch;
    U8                          tpc_rnti_index_pucch;
    U8                          tpc_rnti_index_pusch;
    /* SPS related changes start */
    rrm_bool_t                 sps_enabled_pucch;
    rrm_bool_t                 sps_enabled_pusch;
    /* SPS related changes end */
    rrm_dci_format_et           dci_format_pucch;
    rrm_dci_format_et           dci_format_pusch;
}rrm_ue_clpc_params_t;
/*CLPC END*/
/*! \ rrm_phy_uplink_power_control_dedicated_t
 *  \brief This struct is used for Uplink Power Control Dedicated
 *  \param p0_ue_pusch
 *  \param delta_mcs_enabled
 *  \param accumulation_enabled
 *  \param p0_ue_pucch
 *  \param p_srs_offset
 *  \param filter_coefficient
 */
typedef struct _rrm_phy_uplink_power_control_dedicated_t
{
    S8  p0_ue_pusch;           
    U8  delta_mcs_enabled;     
    U8  accumulation_enabled;  
    S8  p0_ue_pucch;           
    U8  p_srs_offset;          
    U8  filter_coefficient;    
} rrm_phy_uplink_power_control_dedicated_t;

/*! \ rrm_ue_olpc_stats_t
 *  \brief Structure for olpc stats parameters
 *  \param ue_index
 *  \param uplinkSINRValue
 *  \param downlinkSINRValueCW0
 *  \param downlinkSINRValueCW1
 *  \param uplinkBlerValue
 *  \param downlinkBlerValueCW0
 *  \param downlinkBlerValueCW1
 *  \param uplinkPathlossValue
 *  \param timingAdvanceValue
 */
typedef struct _rrm_ue_olpc_stats_t
{
    U32    ue_index;
    U32    uplinkSINRValue;       /*(0-255)*/
    U32    downlinkSINRValueCW0;  /*(0-255)*/
    U32    downlinkSINRValueCW1;  /*(0-255)*/
    U32    uplinkBlerValue;   /*(0-100)*/
    U32    downlinkBlerValueCW0;  /*(0-100)*/
    U32    downlinkBlerValueCW1;  /*(0-100)*/
    U32    uplinkPathlossValue;
    U32    timingAdvanceValue;    /*(0-1282)*/

}rrm_ue_olpc_stats_t;

/* Uplink power control end */
/*start csg membership*/


/*start csg membership*/
/*! \ rrm_csg_membership_t
 *  \brief This struct is used for CSG membership information
 *  \param rrm_csg_membership_status Membership status info
 */
typedef struct _rrm_csg_membership_t
{
    rrm_csg_membership_status_et rrm_csg_membership_status; 
}rrm_csg_membership_t;
/*end csg membership*/
#define RRM_ACTION_ECN_DL_SET 0x01
#define RRM_ACTION_ECN_UL_SET 0x02
#define RRM_ACTION_ECN_DL_UL_SET 0x04
/*! \ rrm_ue_ecn_list_t
 *  \brief This struct is used for UE ECN list info
 *  \param total_downlinkSINRValue
 *  \param max_dl_thp
 *  \param max_ul_thp
 *  \param max_thp
 *  \param ul_value_for_ecn_set
 *  \param dl_value_for_ecn_set
 *  \param bidir_value_for_ecn_set
 *  \param ecn_applied_dir
 */
typedef struct _rrm_ue_ecn_list_t
{
    /* SPR 18008 Fix Start */ 
    U64 max_dl_thp;
    U64 max_ul_thp;
    U64 max_thp;
    U32 total_downlinkSINRValue;
    /* SPR 18008 Fix End */ 
    U32 ul_value_for_ecn_set;
    U32 dl_value_for_ecn_set;
    U32 bidir_value_for_ecn_set;
    U8 ecn_applied_dir; /*Bitmask for each bit*/
}rrm_ue_ecn_list_t;
typedef enum
{
    RRM_NO_ACTN_ON_UE,
    RRM_HO_ACTN_ON_UE,
    RRM_REL_ACTN_ON_UE,
    RRM_CARRIER_REDIRECT_ON_UE,
    RRM_ECN_UL_DL_ON_UE,
    RRM_ECN_DL_ON_UE,
    RRM_ECN_UL_ON_UE
}rrm_ue_actn_et;

typedef enum
{
    RRM_INTER_FREQ,
    RRM_INTRA_FREQ,
    RRM_INTRA_INTER_BOTH,
    /*KPI:start*/
    RRM_HANDOVER_TYPE_INTRA_LTE_S1,
    RRM_HANDOVER_TYPE_LTE_TO_UTRAN,
    RRM_HANDOVER_TYPE_LTE_TO_GERAN,
    RRM_HANDOVER_TYPE_UTRAN_TO_LTE,
    RRM_HANDOVER_TYPE_GERAN_TO_LTE,
    RRM_HANDOVER_TYPE_INTRA_LTE_X2,
    RRM_HANDOVER_TYPE_INTRA_CELL,
    RRM_HANDOVER_TYPE_LTE_TO_CDMA2000_1XRTT,
    RRM_HANDOVER_TYPE_LTE_TO_CDMA2000_HRPD,
    RRM_HANDOVER_TYPE_LTE_TO_CDMA2000_CONC_1XRTT_HRPD,
    RRM_HANDOVER_TYPE_CCO,
    RRM_HANDOVER_TYPE_INTER_CELL
        /*KPI:end*/
}rrm_lte_ho_type_et;

/*! \typedef meas_config_sent_bitmask
 * \brief Bitmasks to indicate the sent status of Quantity config 
 *          and Report Config in Meas Config Req(ANR) to RRC
 *          for the UE
 */
#define QUANTITY_CONFIG_EUTRA_SENT           0x01
#define QUANTITY_CONFIG_UTRA_SENT            0x02
#define QUANTITY_CONFIG_GERAN_SENT           0x04
#define QUANTITY_CONFIG_CDMA2000_SENT        0x08
#define REPORT_CONFIG_EUTRA_SENT             0x10
#define REPORT_CONFIG_INTERRAT_SENT          0x20
/* SPR 7514 Fix Start - report config 924 */
#define REPORT_CONFIG_INTERRAT_UTRAN_CDMA_SENT 0x40
/* SPR 12351 Start */
#define REPORT_CONFIG_EUTRA_CGI_SENT                0x80
#define REPORT_CONFIG_INTERRAT_GERAN_CGI_SENT       0x100
#define REPORT_CONFIG_INTERRAT_UTRAN_CDMA_CGI_SENT  0x200
/* SPR 12351 End */

/* SPR 7514 Fix End - report config 924 */


typedef struct _rrm_ue_cdma2000_ncl_info_list_t
{
    rrm_oam_ncl_type_et  type;
    U16  arfcn;
    U16  pn_offset;
    U8   band_class;
    U8   cell_reselection_priority;
    U8   CID[16];
}rrm_ue_cdma2000_ncl_info_list_t;

typedef struct _rrm_ue_meas_obj_cdma2000_cell_info_t
{
    U8   meas_obj_id;
    U8   meas_id;
    U8   ncl_count;
    rrm_ue_cdma2000_ncl_info_list_t  cdma2000_ncl_info_list[RRMCM_RMIF_NUM_CDMA2000_FREQ_CELLS];
}rrm_ue_meas_obj_cdma2000_cell_info_t;

typedef struct _rrm_ue_meas_obj_cdma2000_info_t
{
    U8                                      count_meas_object_hrpd;
    /* SPR 18008 Fix Start */ 
    U8                                      count_meas_object_1xrtt;
    rrm_ue_meas_obj_cdma2000_cell_info_t    meas_obj_cdma2000_cell_info_hrpd[RRM_MAX_MEAS_OBJECT_ID];
    /* SPR 18008 Fix End */ 
    rrm_ue_meas_obj_cdma2000_cell_info_t    meas_obj_cdma2000_cell_info_1xrtt[RRM_MAX_MEAS_OBJECT_ID];
}rrm_ue_meas_obj_cdma2000_info_t;

typedef enum
{
    RRM_HO_REQUIRED_SENT_FOR_DAHO_CELL,
    RRM_HO_REQUIRED_SENT_FOR_MAHO_CELL
}rrm_ue_ho_req_identifier_et;

typedef struct _rrm_ue_cdma2000_1xrtt_params_t
{
    U16  arfcn;
    U8   band_class;
    U8   CID[6];
    rrm_bool_et  ho_required_sent_status;
    rrm_ue_ho_req_identifier_et  ho_req_identifier;
}rrm_ue_cdma2000_1xrtt_params_t;

typedef struct _rrm_ue_cdma2000_hrpd_params_t
{
    U16  arfcn;
    U8   band_class;
    U8   CID[16];
    rrm_bool_et  ho_required_sent_status;
    rrm_ue_ho_req_identifier_et  ho_req_identifier;
}rrm_ue_cdma2000_hrpd_params_t;

typedef struct _rrm_ue_cdma2000_ho_cmd_resp_info_t
{
    rrm_ue_cdma2000_1xrtt_params_t  cdma2000_1xrtt_params;
    rrm_ue_cdma2000_hrpd_params_t   cdma2000_hrpd_params;
}rrm_ue_cdma2000_ho_cmd_resp_info_t;

typedef struct _rrm_ue_meas_results_info_t
{
    U8  meas_id;
    /* SPR 18008 Fix Start */ 
    rrm_bool_t  cdma2000_cell_type;
    /* SPR 18008 Fix End */ 
    U8  pci_count;
    U16  pci[RRM_MAX_CELLS_REPORTED];
    U8  pilot_strength[RRM_MAX_CELLS_REPORTED];
}rrm_ue_meas_results_info_t;

typedef struct _rrm_ue_cdma2000_capabilities_t
{
    /* SPR 18008 Fix Start */ 
    rrm_bool_t  is_cdma2000_present;
    rrm_bool_t  is_ecsfb_conc_ho_present;
    rrm_bool_t  is_only_ecsfb_present;
    rrm_bool_t  is_ecsfb_present;
    rrm_bool_t  is_utran_geran_present;
    rrm_bool_t  is_dual_rx_mode_present;
    /* SPR 18008 Fix End */ 
}rrm_ue_cdma2000_capabilities_t;

typedef struct _rrm_ue_irat_spid_priority_t
{
    rrm_irat_priority_et irat_priority_list[RRM_IRAT_TYPE];
}rrm_ue_irat_priority_t;


/* MAX_CID Changes Starts */
typedef struct _rrm_map_drb_rohc_t
{
    U8            rab_id;
    /* SPR 18008 Fix Start */ 
    rrm_bool_t   is_rohc_enable;
    /* SPR 18008 Fix End */ 
}rrm_map_drb_rohc_t;

typedef struct _rrm_map_drb_rohc_info_t
{
    /* SPR 18008 Fix Start */ 
    U8    max_count_drbs_rohc_enabled;
    U8    count_drbs_cid_allocated;    
    U8                  total_drb_count;
    /* SPR 18912 Fix Start */
    U8            cid_from_config;
    U8            max_cid_allocated;
    rrm_bool_t    from_ue;
    /* SPR 18912 Fix End */ 
    rrm_map_drb_rohc_t  map_data_rohc[RRM_MAX_NUM_DRB_PER_UE];
    U32   max_cid_allocation_per_drb;
    /* SPR 18008 Fix End */
 
}rrm_map_drb_rohc_info_t;
/* MAX_CID Changes End */


typedef struct _rrm_ue_fgi_bits_presence_t
{
    /* SPR 18008 Fix Start */ 
    rrm_bool_t  is_ue_utran_capable;
    rrm_bool_t  is_ue_geran_capable;
    rrm_bool_t  is_ue_cdma2000_capable;
    /* SPR 18008 Fix End */ 
}rrm_ue_fgi_bits_presence_t;



/*  DYNAMIC ICIC CHANGES START  */

typedef struct _rrm_ue_list_for_icic_action_t
{
    rrm_cell_index_t                 cell_index;				/*!< Cell index for UE context		*/
    rrm_ue_index_t                   ue_id;
    U32                              avg_ul_path_loss_value;
    struct _rrm_ue_list_for_icic_action_t    *p_next;
    struct _rrm_ue_context_t                 *p_ue_context;
}rrm_ue_list_for_icic_action_t;

typedef struct _rrm_ue_loc_update_action_info_t
{
    rrm_cell_index_t     cell_index;				/*!< Cell index for UE context		*/
    U16                  transaction_id;
    U16                  num_of_requests_triggered;
    rrm_return_et        confirmation_status; 
    rrm_location_update_action_et       ue_location_update_action; /*!<  Location action  */
    rrm_olpc_reduce_power_action_et     ue_reduce_power_action; /*!<  Reduce UL Power action  */
    U8                                  location_update_percentage;
}rrm_ue_loc_update_action_info_t;

typedef struct _rrm_cellm_uem_ue_update_data_t
{
#define     UE_LOCATION_UPDATE_ACTION_PRESENT           0x01
#define     UE_OLPC_REDUCE_POWER_ACTION_PRESENT         0x02
    U8                                     action_requested;
    U8                                     action_performed;
    rrm_return_et                          ue_loc_update_action_resp; 
    rrm_return_et                          ue_olpc_reduce_power_action_resp;
    rrm_ue_loc_update_action_info_t        rrm_ue_loc_update_action_info;
}rrm_cellm_uem_ue_update_data_t;

/*DYNAMIC ICIC START*/
#define COOL_OFF_PERIOD_FOR_UE_RECONFIG 20
typedef enum
{
    /* FIX UE_RECONFIG_NO_POSITION_CHANGE START */
    RRM_UE_P_A_NOT_UPDATED = 0,
    RRM_UE_P_A_INCREMENTED,
    RRM_UE_P_A_DECREMENTED
        /* FIX UE_RECONFIG_NO_POSITION_CHANGE END */
}rrm_ue_pa_update_type_et;


typedef enum
{
    RRM_UE_POSITION_AT_CELL_CENTER = 0,
    RRM_UE_POSITION_AT_CELL_EDGE,
    RRM_UE_POSITION_FRM_CE_to_CC,
    RRM_UE_POSITION_FRM_CC_to_CE
}rrm_ue_position_et;

typedef struct _rrm_ue_dynamic_icic_data_t
{
    U8                  p_a_configured;
    rrm_ue_position_et  ue_past_position;			
    rrm_ue_position_et  ue_present_position;		
    rrm_ue_pa_update_type_et last_p_a_updated_type;
    //timestamp value when last reconfigured
    RRM_TIME_T           last_time_ue_reconfigured;
}rrm_ue_dynamic_icic_data_t;

typedef struct _rrm_ue_dynamic_icic_info_t_
{
    /* SPR 18008 Fix Start */ 
    rrm_bool_t         ue_reconfig_rollback_flag;
    /*Location update fix in UE RECONFIG*/
    rrm_bool_t         location_update_req_for_ue_reconfig;
    /* SPR 18008 Fix End */ 
    /*Location update fix in UE RECONFIG*/
    rrm_ue_dynamic_icic_data_t rrm_ue_dynamic_icic_present_data;
    rrm_ue_dynamic_icic_data_t rrm_ue_dynamic_icic_past_data;
}rrm_ue_dynamic_icic_info_t;

/*  DYNAMIC ICIC CHANGES END  */

/* CA_Stage_3_Scell_TM_change: Start */
typedef struct _ue_speed_related_info_for_scell_t
{
    U8                                 ue_speed_calculation_count;
    /* SPR 18008 Fix Start */ 
    rrm_bool_t                        ue_speed_category_change_flag;
    /* SPR 18008 Fix End */ 
    rrm_ue_speed_category_et           ue_speed_category;
}ue_speed_related_info_for_scell_t;
/* CA_Stage_3_Scell_TM_change: End */

/* Code changes end for LA feature*/
typedef struct _ue_speed_related_info_t
{
    U8                          ue_speed_calculation_count;
    /* SPR 18008 Fix Start */ 
    rrm_bool_t                 ue_speed_catagory_change_flag;
    /* SPR 18008 Fix End */ 
    rrm_ue_speed_category_et    ue_speed_catagory;
    /* CA_Stage_3_Scell_TM_change: Start */
    /* array indexes to be accessed by the Cell index of the scell */
    ue_speed_related_info_for_scell_t  ue_speed_param_scell[RRM_MAX_SCELL];
    /* CA_Stage_3_Scell_TM_change: End */
}ue_speed_related_info_t;
/* Code changes end for LA feature*/

/* RACH_OPTIMIZATION_CHANGES_START */
typedef struct _rrm_ue_info_resp_t
{
#define RRM_UE_RACH_INFO_PRESENT  0x01
    rrm_bitmask_t         bitmask;
    U16                   number_of_preambles_sent;
    rrm_bool_et           contention_detected;
}rrm_ue_info_resp_t;

typedef struct _rrm_ue_temp_rach_data_t
{
    prach_cause_et        cause;
    /* SPR 18008 Fix Start */ 
    strong_cell_info_t    strong_cell_info[STRONG_CELL_INFO_COUNT];
    U16                   number_of_preambles_sent;
	rrm_bool_t            contention_detected;
    rrm_bool_t            is_ue_report_requested;
    /* SPR 18008 Fix End */ 
    U8                    meas_id;
/* SPR-18789 START */
    U8                    report_config_id;
    U8                    is_rach_report_config_to_remove;
/* SPR-18789 END */
}rrm_ue_temp_rach_data_t;

typedef struct _rrm_lipa_ho_required_t
{
    U8   transaction_id;
}rrm_lipa_ho_required_t;

/* CA_Stage_3_Scell_TM_change: Start */
typedef struct _rrm_scell_periodic_stats_t
{
    U16               scell_report_index;
    U16               scell_report_count;
}rrm_scell_periodic_stats_t;

typedef rrc_antenna_info_dedicated_r10_t rrm_scell_antenna_info_dedicated_r10_t;
/* CA_Stage_3_Scell_TM_change: End */


/* CA structures : Start */
typedef struct _rrm_ue_format1b_t
{
     U16 n1_pucch_an_cs_first_index;   /* contains the resource index which has been allocated to the a/n list in sps DL config*/
     U16 n1_pucch_an_cs_second_index;   /* contains the resource index which has been allocated to the a/n list in sps DL config*/
}rrm_ue_format1b_t;
typedef struct _rrm_ue_scell_sps_data_t
{
    U16 pucch_an_list_resource_first_index;   /* contains the resource index which has been allocated to the a/n list in sps DL config*/
    U16 pucch_an_list_resource_second_index;   /* contains the resource index which has been allocated to the a/n list in sps DL config*/

}rrm_ue_scell_sps_data_t;
/*CA Stage3 start*/
typedef enum
{
    SCELL_DEACTIVE,
    SCELL_DEACTIVE_FOR_MEASUREMENT,
    SCELL_READY_FOR_ACTIVATION,
    SCELL_ACTIVE
}rrm_ue_scell_state_et;
/*CA Stage3 end*/
typedef struct _rrm_scell_info_t
{
    /* CA_Stage_3_Scell_TM_change: Start */
    U16                           bitmask;
#define RRM_UE_RECONFIG_SCELL_TM_INFO_R10_ROLLBACK_PRESENT      0x01
    /* SPR 18008 Fix Start */ 
    U8              scell_index;      /* To be used at Scell release time */
    U8              scell_cell_index; /* Scell Cell Index on current eNB */
    /* CA_Stage_3_Scell_TM_change: End */
    rrm_ue_scell_state_et  scell_state;      /* 0 -> De-active ; 1 -> De-Active for Measurement 2->Ready for activation 3-> Active */
    /* SPR 18008 Fix End */ 
    rrm_ue_cqi_paramas_t        cqi_params;             /*!< CQI Params for Ue Context      */
    rrm_ue_scell_sps_data_t  ue_scell_sps_resc;
    rrm_ue_format1b_t     ue_format1b_resc;
    /*CA Stage3 start*/
    /*meas_id will be array of two.
    As two report config id will be generated for A1 and A2 RSRQ*/
    U8  meas_id[RRM_TWO];
    U8 meas_bitmask;
    /*CA Stage3 end*/
    /* CA_Stage_3_Scell_TM_change: Start */
    rrm_scell_periodic_stats_t    scell_periodic_stats;     /* Scell Mac periodic stats for runtime Scell TM change */
    rrm_scell_antenna_info_dedicated_r10_t   scell_antenna_info_dedicated_r10;
    rrm_scell_antenna_info_dedicated_r10_t   scell_antenna_info_dedic_r10_backup;
    /* CA_Stage_3_Scell_TM_change: End */
    U64 bitrate_req_from_scell;
}rrm_scell_info_for_ue_t;

typedef struct _ue_scell_add_params
{
    U8                          bitmask;
#define RRM_SCELL_INDEX_BACKUP_PRESENT 0x01        
#define RRM_SCELL_RADIO_RESOURCE_CONFIG_COMMON_PARAM_CHANGE 0x02
#define RRM_SCELL_DELETE_SCENARIO 0x04
    U8                          count;
    U8                          scell_bkp_count;
    rrm_scell_info_for_ue_t     rrm_scell_info_for_ue[RRM_MAX_SCELL];
    U8                          scell_cell_index_add_bkp[RRM_MAX_SCELL];
    U8                          scell_cell_index;
    /* Carrier Aggregation start*/
    rrm_scell_info_for_ue_t     rrm_scell_state_backup[RRM_MAX_SCELL];
    /* Carrier Aggregation end */
}rrm_ue_scell_add_params_t;

rrm_return_et
rrm_find_scell_index_position(rrm_cell_index_t scell_index,
     rrm_ue_scell_add_params_t *p_scell_param, rrm_cell_index_t *p_rrm_cell_index);
/* CA structures : End */

typedef struct _rrm_configured_meas_gap_info_t
{
#define         MEAS_GAP_ADDED_FOR_PRS      0x01
#define         MEAS_GAP_ADDED_FOR_NON_PRS  0x02
    U8            meas_gap_added;
    U8            meas_gap_type;
    U8            meas_gap_offset;/* meas gap offset value */
    /*SPR 17061 Start*/
    /* SPR 15397 Start */
    U8            meas_gap_config_type;
    /* SPR 15397 End */
    /*SPR 17061 End*/
}rrm_configured_meas_gap_info_t;
/* CA: SCell Selection start */
typedef struct _rrm_ca_data_t
{
    U8  num_of_times_threshold_exceeded;      /* Indicates the count for which the OAM configured bitrate-threshold gets exceeded */
    U8  num_of_times_threshold_not_exceeded;  /* Indicates the count for which the OAM configured bitrate-threshold doesn't get exceeded */
    /* SPR 18008 Fix Start */ 
    rrm_bool_t       a4_recv_for_ca;
    U8  count;                                /* Total number of SCells present in the final Scell list */
    rrm_cell_index_t  final_scell_list[RRM_MAX_SCELL]; /* Array of final Scells shortlisted after all the checks */
    /* SPR 18008 Fix End */ 
    /*spr_22361_changes_start*/
    rrm_bool_et      is_meas_gap_enabled_for_ca;
    /*spr_22361_changes_end*/
}rrm_ca_data_t;
/* CA: SCell Selection end */

/* RACH_OPTIMIZATION_CHANGES_END */

/* SRVCC SUPPORT START */
#define RRM_UEM_SRVCC_HO_INDICATION_PRESENT 0x01
/*! \  struct rrm_srvcc_config_info_t
 *  \brief Structure storing the SRVCC configuration for the UE
 *  \param
 */
typedef struct _rrm_srvcc_config_info_t
{
    U8                      bitmask;
    /* SPR 18008 Fix Start */ 
    rrm_bool_t             srvcc_ho_op_possible;
    /* SPR 18008 Fix End */ 
    rrm_srvcc_ho_type_et    srvcc_ho_indication;
    rrm_srvcc_ue_criteria_et ue_srvcc_criteria;
}rrm_srvcc_config_info_t;
/* SRVCC SUPPORT END */

/*! \  struct _rrm_ue_allocated_info_t 
 *  \brief Structure storing the UE Allocated info
 *  \param allocated_gbr_info_per_drb DRB Allocated gbr info 
 *  \param dl_allocated_ngbr_prb dl allocated ngbr prb
 *  \param ul_allocated_ngbr_prb ue allocated ngbr prb 
 */
typedef struct _rrm_ue_allocated_info_t
{
    rrm_drb_allocated_gbr_info_t    allocated_gbr_info_per_drb[RRM_MAX_NUM_RAB_IDS];
    U8                dl_allocated_ngbr_prb;
    U8                ul_allocated_ngbr_prb;
}rrm_ue_allocated_info_t;


/* SPR 12897 Start */
typedef enum
{
    RRM_UE_CDRX_DISABLED,
    RRM_UE_CDRX_TO_BE_APPLIED,
    RRM_UE_CDRX_APPLIED,
    RRM_UE_CDRX_TO_BE_REMOVED
}rrm_ue_cdrx_status_et;
/* SPR 12897 End */
/* SPR 16406 Start */
typedef enum _rrm_cell_list_type_et
{
  RRM_INVALID_CELL_LIST_TYPE,
  RRM_EUTRAN_KNOWN_CELL_LIST, 
  RRM_EUTRAN_UNKNOWN_CELL_LIST, 
  RRM_EUTRAN_CONFUSED_CELL_LIST, 
  RRM_UTRAN_KNOWN_CELL_LIST,
  RRM_UTRAN_UNKNOWN_CELL_LIST,
  RRM_UTRAN_CONFUSED_CELL_LIST,
  RRM_GERAN_KNOWN_CELL_LIST,
  RRM_GERAN_UNKNOWN_CELL_LIST,
  RRM_GERAN_CONFUSED_CELL_LIST 
}rrm_cell_list_type_et;

typedef struct _rrm_cell_selection_info_t
{
    rrm_cell_list_type_et current_cell_list;
}rrm_cell_selection_info_t;

/* SPR 16406 End */

/*spr 17437 fix start*/
typedef struct _rrm_ue_rab_remove_data_t
{
   rrm_ue_index_t       ue_idx;
   U8           erabs_to_be_released_count;
   U8           erabs_to_be_released[RRM_MAX_NUM_DRB_PER_UE];
}rrm_ue_rab_remove_data_t;

typedef struct _rrm_ue_rab_remove_record_t
{
  YLNODE          pNode;
  rrm_ue_rab_remove_data_t  ue_rab_info;
}rrm_ue_rab_remove_record_t;

typedef struct _rrm_ue_remove_node_t
{
  YLNODE            pNode;
  rrm_ue_index_t    ue_idx;

} rrm_ue_remove_node_t;

typedef struct _rrm_rab_remove_node_t
{
  YLNODE            pNode;
  U8                    erab_id;
} rrm_rab_remove_node_t;

typedef struct _rrm_rab_removal_container_t
{
   YLIST ue_to_remove_list;  /* List of UEs to be removed,list of nodes of type rrm_ue_remove_node_t */
   YLIST ue_rab_list;        /* List of RABs to be deleted for other UEs,nodes of type rrm_ue_rab_remove_record_t  */
}rrm_rab_removal_container_t;

typedef struct _rrm_rab_removal_ctxt_t
{
   YLIST            rabs_to_rem_list;  /* List of self rabs to be deleted,nodes of type rrm_rab_remove_node_t */
   rrm_rab_removal_container_t  rab_rem_container;
}rrm_rab_removal_ctxt_t;


typedef struct _rrm_adm_ctrl_record_t
{
   rrm_bool_et                   is_ue_already_selected;
   rrm_rab_removal_container_t   rab_removal_container;

}rrm_adm_ctrl_record_t;
/*spr 17437 fix stop*/



/* eICIC_PHASE_1_2_CHANGES_START */
/* SPR 14285 start */
typedef enum
{
    RRM_UE_NO_VICTIM_STATE,
    RRM_UE_VICTIM_CANDIDATE,
    RRM_UE_VICTIM_STATE,
}rrm_victim_state_et;
/* SPR 14285 end */
typedef struct _victim_ue_node_data_t
{
    rrm_ue_index_t        ue_index;
    rrm_victim_state_et   victim_state;  
    U16       		  pci[MAX_AGGRESSOR_COUNT];
    U32                   dl_sinr;  
}victim_ue_node_data_t;

typedef struct _victim_ue_node_t
{
    YLNODE                    node;
    victim_ue_node_data_t     victim_ue_node_data;
}victim_ue_node_t;

typedef struct _rrm_victim_ue_on_cell_t
{
    YLIST                    victim_ue_list;
}rrm_victim_ue_on_cell_t;

typedef struct _rrm_victim_ue_info_t
{
   U8                      count;
   rrm_victim_ue_on_cell_t rrm_victim_ue_on_cell[RRM_MAX_NUM_CELLS];
}rrm_victim_ue_info_t;

typedef struct _aggressor_node_data_t
{
    U16       pci;
    U16       occurence_count;      
}aggressor_node_data_t;

typedef struct _aggressor_node_t
{
    YLNODE                    node;
    aggressor_node_data_t     aggressor_node_data;
}aggressor_node_t;

typedef struct _rrm_aggressor_on_cell_t
{
   YLIST                    aggressor_node_list;
}rrm_aggressor_on_cell_t;

typedef struct _rrm_aggressor_node_info_t
{
   U8                      count;
   rrm_aggressor_on_cell_t rrm_aggressor_on_cell[RRM_MAX_NUM_CELLS];
}rrm_aggressor_node_info_t;

/* SPR 14471 start */
typedef enum 
{
    RRM_UE_RECONFIG_FOR_VICTIM,
    RRM_UE_RECONFIG_FOR_NON_VICTIM
}rrm_victim_reconfig_opt_et;
/* SPR 14471 end */

typedef struct _rrm_ue_eicic_info_t
{
    /* SPR 14285 start */
    rrm_victim_state_et victim_ue;
    /* SPR 14285 end */
    rrm_bool_t  scheduled_on_abs;
    U8          num_of_eligible_ue;
    /* SPR14165start */
    rrm_bool_t  rrm_ue_eicic_reconfig_ongoing; 
    /* SPR14165end */
    U8	        meas_report_id;
    /* SPR 18008 Fix Start */ 
    /*SPR 15663 start*/
    rrm_bool_t non_vitim_reconfig_flag;
    /*SPR 15663 end*/
    /* SPR 14471 start */
    rrm_victim_reconfig_opt_et victim_reconfig_flag;
    /* SPR 14471 end */
    /* SPR 18008 Fix End */ 
}rrm_ue_eicic_info_t;


typedef struct _eicic_meas_data_t
{
    rrm_son_eutran_neighbor_cell_t eutran_ncl;
    U16       occurence_count;      
}eicic_meas_data_t;

typedef struct _rrm_ue_eicic_meas_ind_t
{

    U8                     num_meas_ind_rcvd;
    U8                     num_cell_reported;
    /* SPR 18008 Fix Start */ 
    U8                     valid_aggressor_count;
    eicic_meas_data_t      eutran_cell[EUTRAN_MEAS_CELL_CNT];
    /* SPR 18008 Fix End */ 
    U16                    valid_aggressor_list[RRM_THREE];
}rrm_ue_eicic_meas_ind_t;

rrm_void_t rrm_uem_sort_eicic_meas_array
(rrm_ue_eicic_meas_ind_t * p_eicic_meas_result);

/* eICIC_PHASE_1_2_CHANGES_END */

#ifdef LTE_EMBMS_SUPPORTED

#define RRM_UE_MBMS_RECONFIG_DRX_ROLLBACK_PRESENT                  0x001 
#define RRM_UE_MBMS_RECONFIG_REL10_VICTIM_CQI_ROLLBACK_PRESENT     0x002 
#define RRM_UE_MBMS_RECONFIG_REL10_NON_VICTIM_CQI_ROLLBACK_PRESENT 0x004 
#define RRM_UE_MBMS_RECONFIG_REL9_CQI_ROLLBACK_PRESENT             0x008 
#define RRM_UE_MBMS_RECONFIG_SR_ROLLBACK_PRESENT                   0x010 
#define RRM_UE_MBMS_RECONFIG_MEASGAP_ROLLBACK_PRESENT              0x020 
#define RRM_UE_MBMS_RECONFIG_DRX_REL_ROLLBACK_PRESENT              0x040 
#define RRM_UE_MBMS_RECONFIG_CQI_REL_ROLLBACK_PRESENT              0x080 
typedef struct  _rrm_ue_mbms_data_t
{
    rrm_bool_et    mbms_reconfig_triggered;
    rrm_bitmask_t  reconfig_bitmask;
    U8             mbms_reconfig_count;
    U8             is_cqi_disabled;
    U8             is_drx_disabled;
}rrm_ue_mbms_data_t;
#endif

/* ENDC_MEAS_CHANGES_START */
#ifdef ENDC_ENABLED

typedef enum
{
  RRM_ENDC_ENABLE_NOT_INITIATED = RRM_ZERO,
  RRM_ENDC_ENABLE_INITIATED,
  RRM_ENDC_ENABLE_SUCCESSFUL,
  RRM_ENDC_ENABLE_FAILED
}rrm_endc_enable_status_et;

typedef enum
{
  TRIGGER_TYPE_QCI,
  TRIGGER_TYPE_ERAB_THROUGHPUT,
  TRIGGER_TYPE_CHANNEL_QUALITY
}rrm_nr_meas_trigger_type_et;
typedef enum
{
  RRM_NR_MEAS_REQ_NOT_INITIATED = RRM_ZERO,
  RRM_NR_MEAS_REQ_FILLED,
  RRM_NR_MEAS_REQ_INITIATED,
  RRM_NR_MEAS_REQ_COMPLETED
}rrm_nr_meas_req_status_et;

/* NR_REP_CONFIG_FIX_START */
typedef struct _rrm_endc_nr_sgnb_meas_info_t
{
    U8        meas_id;
    U8        meas_object_id;
    U8        report_config_id;
}rrm_endc_nr_sgnb_meas_info_t;

typedef struct _rrm_endc_nr_cell_meas_info_t
{
    U16       phy_cell_id;
    U8        meas_object_id;
}rrm_endc_nr_cell_meas_info_t;

typedef struct _rrm_endc_nr_sgnb_meas_data_t
{
    rrm_x2_gb_gnb_id_t       global_gnb_id;
    U8                       cell_count;
    rrm_endc_nr_cell_meas_info_t  rrm_endc_nr_cell_meas_info[MAX_SERVED_CELLS];
    U8                       meas_count;
    rrm_endc_nr_sgnb_meas_info_t  rrm_endc_nr_sgnb_meas_info[24];
}rrm_endc_nr_sgnb_meas_data_t;

typedef struct _rrm_endc_nr_meas_data_t
{
    U8  sgnb_count;
    rrm_endc_nr_sgnb_meas_data_t rrm_endc_nr_sgnb_meas_data[RRM_TWO];
    U8        report_config_id_count;
    U8        report_config_id_list[3];
}rrm_endc_nr_meas_data_t;
/* NR_REP_CONFIG_FIX_END */

typedef struct _rrm_endc_nr_meas_stats_t
{
    U8                           nr_meas_req_counter;
    rrm_nr_meas_req_status_et    nr_meas_req_status;
//    rrm_nr_meas_result_status_et nr_meas_result_status;
}rrm_endc_nr_meas_stats_t;

typedef struct _rrm_ue_channel_quality_reports_t
{
  U32 sinr_ul;
  U32 sinr_dl_cw0;
  U32 sinr_dl_cw1;
  U32 bler_ul;
  U32 bler_dl_cw0;
  U32 bler_dl_cw1;
}rrm_ue_channel_quality_reports_t;

typedef struct _rrm_ue_qci_reports_t
{
   U8  qci;
   U8  lc_id;
   U64 dl_bitrate;
   U64 ul_bitrate;
}rrm_ue_qci_reports_t;

typedef struct _rrm_ue_throughput_reports_t
{
   U8  count;
   rrm_ue_qci_reports_t  rrm_ue_qci_reports[RRM_MAX_NUM_DRB_PER_UE];
}rrm_ue_throughput_reports_t;

typedef struct _rrm_ue_reports_t
{
  rrm_ue_channel_quality_reports_t rrm_ue_channel_quality_reports;
  rrm_ue_throughput_reports_t      rrm_ue_throughput_reports;
}rrm_ue_reports_t;

typedef struct _rrm_endc_nr_data_t
{
    rrm_endc_nr_meas_data_t      rrm_endc_nr_meas_data[RRM_TWO];
    rrm_endc_nr_meas_stats_t     rrm_endc_nr_meas_stats;
    rrm_endc_enable_status_et    endc_enable_status;
    U8                           report_sample_count;
    U8                           bearer_thp_report_count;
    rrm_ue_reports_t             rrm_ue_temp_reports;
    rrm_ue_reports_t             rrm_ue_final_reports;
    rrm_bool_t                   reports_resfreshed;
    U8                           lte_meas_gap_offset;
    U8                           lte_meas_gap_type;
    rrm_bool_et                  mrdc_capability_valid;
}rrm_endc_nr_data_t;

/* NR_BAND_FIX_START */
typedef enum
{
    RRM_NR_FREQ_BAND_FDD = 1,
    RRM_NR_FREQ_BAND_TDD,
    RRM_NR_FREQ_BAND_INVALID
}rrm_nr_freq_band_mode_et;

/* FDD */
#define RRM_NR_BAND_N1      1
#define RRM_NR_BAND_N2      2
#define RRM_NR_BAND_N3      3
#define RRM_NR_BAND_N5      5
#define RRM_NR_BAND_N7      7
#define RRM_NR_BAND_N8      8
#define RRM_NR_BAND_N20    20
#define RRM_NR_BAND_N28    28
#define RRM_NR_BAND_N66    66
#define RRM_NR_BAND_N70    70
#define RRM_NR_BAND_N71    71
#define RRM_NR_BAND_N74    74

/* TDD */
#define RRM_NR_BAND_N38    38
#define RRM_NR_BAND_N41    41
#define RRM_NR_BAND_N50    50
#define RRM_NR_BAND_N51    51
#define RRM_NR_BAND_N77    77
#define RRM_NR_BAND_N78    78
#define RRM_NR_BAND_N79    79
/* NR_BAND_FIX_END */

#endif
/* ENDC_MEAS_CHANGES_END */


/*! \  struct rrm_ue_context_t
 *  \brief Structure storing the all UE related information
 *  \param rrm_ue_index_t ue_index
 *  \param rrm_cell_index_t cell_index
 *  \param rrm_ue_establishment_cause_et establishment_cause
 *  \param rrm_ue_ambr_t ue_agg_max_bit_rate
 *  \param rrm_ue_erab_setup_item_list_t drb_configured
 *  \param rrm_ue_states_et ue_state
 *  \param rrm_ue_events_et ue_event
 *  \param rrm_ue_procedure_t current_procedure
 *  \param rrm_ue_cqi_paramas_t cqi_params
 *  \param rrm_ue_sr_paramas_t sr_params
 *  \param rrm_ue_srs_params_t srs_params
 *  \param S8 average_ul_sinr
 *  \param S8 average_dl_snr
 *  \param rrc_ue_ul_sch_config_t ul_sch_config
 *  \param rrm_ue_proc_queue_t procedure_queue
 *  \param rrm_uem_s_tmsi_t ue_s_tmsi
 *  \param rrm_ue_context_mod_req_t ue_context_mod_req
 *  \param U8 time_alignment_timer_dedicated
 *  \param U8 sinr_adm_req
 *  \param rrm_ue_capability_params_t ue_capability_params
 *  \param U16 tpc_rnti_pucch
 *  \param U16 tpc_rnti_pusch
 *  \param U8 tpc_rnti_index_pucch
 *  \param U8 tpc_rnti_index_pusch
 *  \param U32 applied_drx_profile_index
 *  \param U16 sub_profile_id
 *  \param U16 trans_sub_profile_id
 *  \param rrm_ue_history_t ue_history
 *  \param U8 ue_inactivity_time
 *  \param rrm_ho_restriction_list_t ho_restriction_list
 *  \param U8 ue_priority
 *  \param rrm_ue_meas_config_t meas_config[MAX_SON_MEAS_CONFIG_OBJECT]
 *  \param rrm_ue_meas_result_serv_cell_t meas_result_serv_cell;
 *  \param U16 cgi_req_trans_id
 *  \param meas_status value of received measurement config response
 *  \param meas_report_list list of measurement reports
 *  \param meas_object_id_list list of measurement object ids used/unused
 *  \param report_config_id_list list of report config ids used/free
 *  \param meas_report_id_list list of measurement report ids used/free
 *  \param ho_params ho parameters
 *  \param ue_start_time start time of the UE
 *  \param report_event value of the latest measurement event type recevied
 *  \param trigger_quantity of the latest event received
 *  \param is_a3_configured specifies if a3 is default/delayed configured
 *  \param a2_trigger_quantity_bitmask specifies is a2 RSRP/RSRQ received
 *  \param a1_trigger_quantity_bitmask specifies is a1 RSRP/RSRQ received
 *  \param ue_handover_report stores the handover report to be sent to SON
 *  \param ue_handover_failure_report stores the handover failure report to be sent to SON
 *  \param meas_config_of_ho_adm the meas config received in ho adm req
 *  \param rrm_son_meas_results_for_missing_pci stores the missing pci info to be sent to SON
 *  \param carrier_earfcn configured in measurement object
 *  \param band_indicator corresponding to the carrier frequency
 *  \param srvcc_config_info SRVCC Configuration info
 */
/* SPR 21496 Fix Start */ 
typedef struct _rrm_ue_context_t
{
    rrm_ue_index_t              ue_index;				/*!< UE index for UE context		*/
    rrm_cell_index_t            cell_index;				/*!< Cell index for UE context		*/
    /* MLB Changes start */
    rrm_ue_actn_et                  action_triggered;       /*!< Indicate that UE is already marked for an action */
    rrm_timer_t                     carrier_redirect_ue;
    YLIST                           best_cell_for_ue;
    /* MLB Changes end */
    /* SPR 18008 Fix Start */ 
    rrm_ue_establishment_cause_et establishment_cause;	/*!< UE AMBR for UE context			*/
    rrm_ue_ambr_t               ue_agg_max_bit_rate;
    rrm_ue_erab_setup_item_list_t   drb_configured;			/*!< Number of DRB configured		*/
    rrm_ue_states_et            ue_state;				/*!< FSM states for UE				*/
    rrm_ue_events_et            ue_event;			    /*!< current event for FSM			*/
    rrm_ue_procedure_t          current_procedure;		/*!< Current procedure for context	*/
    /* CSR00069389 Start */
    rrm_ue_erb_states_et    	procedure_awaited_state;	/*!< Procedural state for awaited event	*/
    rrm_ue_procedure_record_t   *p_proc_queue_first_meas_node; /*!< First meas node in proc queue */
    /* CSR00069389 End */
    rrm_ue_cqi_paramas_t        cqi_params;             /*!< CQI Params for Ue Context      */
    rrm_ue_sr_paramas_t         sr_params;              /*!< SR Params for Ue COntext       */
    rrm_ue_srs_params_t         srs_params;             /*!< SRS Params for Ue Context      */
    /* SRS_FREQ_HOPPING_START */
    rrm_ue_srs_params_t         srs_params_last_used;             /*!< SRS Params for Ue Context      */
    /* SRS_FREQ_HOPPING_END */
    /*<  To store the missing PCI info to be sent to SON */
    U32                         carrier_earfcn; /* Stores the carrier frequency configured */
    U8                          band_indicator; /*Stores the band indicator for GERAN / CDMA */
    U8                          meas_gap_type;
    U8                          meas_gap_offset;/* meas gap offset value */
    U8                          rrm_ue_in_hold;
    U8                          time_alignment_timer_dedicated; /*Time Alignment Timer Dedicated  */
    U8                          ue_inactivity_time;/*!<  ue_inactivity_time          */
    U8				            ue_priority; /* UE priority */
	U8                          is_ho_reqd_on_recv_utan_cap;
    /*!<   meas_config. Index implies the meas ID for which the config is applicable */
    U8                          config_id;
    U8      			        meas_req_send_for_olpc;
    U8                          is_a3_configured; /*!< To validate a3 delayed or default configuration */
    /*ECN start*/
    U8                          ecn_supported;
    U8                          ecn_applied;
    /*ECN end*/
    rrm_bool_t           cdma2000_meas_id_received;
    rrm_bool_t           trying_ho_for_cdma2000;
    /*SPR 5169 start*/
    U8    sr_timer_r9_bkp;
    U8    sr_timer_r9_new;
    U8    meas_rep_event;
    U8    add_mod_list_cell_count;
    rrm_bool_t  spid_found;
    rrm_bool_t  is_mac_req_initiated;
    /*Start changes for TTI bundling feature*/
    rrm_bool_t support_tti_bundling; /* store result on the basis of QCI and FGI */
    rrm_bool_t tti_bundling_enabled; /* store current status of TTI bundling feature */
    U8          num_tti_bundling_erb; /* store total number of TTI bundling capable erbs */
    /*End changes for TTI bundling feature*/
    /* SPR 14302 start */
    rrm_bool_t			      rrm_eicic_meas_event_ongoing;		
    /* SPR 14302 end */
    /* eICIC_PHASE_1_2_CHANGES_END */

    /* SPR-13322-fix start */
    rrm_bool_t    is_cgi_process_in_progress;
    /* SPR-13322-fix end */
    /*SPR 15301 Fix Start */
    rrm_bool_t    is_csi_pattern_to_release;
    /*SPR 15301 Fix end */
    /*SPR_15323 start*/
    rrm_bool_t srb_allocated;
    /*SPR_15323 end*/
    /* SPR 15314 Start */
    U8 report_amount_ue;
    /* SPR 15314 End*/
    /*SPR Fix 15664 Start*/
    rrm_bool_t     is_meas_gap_configured;
    /*SPR Fix 15664 End*/
    /*SPR 17061 Start*/
    /*SPR 15835:Start*/
    /*Initially the report amount of UE will get stored and 
     * after meas config response will be updated in report_amount_ue*/
    U8   temp_report_amt_ue; 
    /*SPR 15835:End*/
    /*SPR 17061 End*/
    /* SPR 15674 End */
    /* SPR 15674 End */
	/* SPR 14326 Fix Start */
	rrm_bool_t    send_measgap_in_meas_config;
    /* SPR 18008 Fix End */ 
    /* SPR 14326 Fix End */
    /*TM mode 7/8 start*/
    rrm_bool_t beam_forming_algo_configd_for_TM7_8;
    /*TM mode 7/8 end*/
    /*spr 8391 start*/
    rrm_bool_t proximity_ind;
    /*spr 8391 end*/
    /* Start CSR: 00058590 */
    rrm_bool_t    interfreq_interrat_meas_configured;
    /* End CSR: 00058590 */
    /* Fix for CSR: 00059124 */
    U8 supported_rats;
    /* SPR-12570-fix start */
    rrm_bool_t                   handover_on_daho_cell_skip_ho_status_report;
    /* SPR-12570-fix end */
    /* SPR 9216:MRO enhancement start */
    rrm_bool_t          is_ho_failed;
    /* SPR 9216:MRO enhancement end */
    /*SPR 10329:start*/
    rrm_bool_t                  is_gu_grp_info_present; /*bitmask if gu_group info is present*/
    /* CSR-86373[SPR-12963]-fix start */
    U8                          stored_meas_id_for_cgi;
    /* CSR-86737[SPR-12963]-fix end */
    U8                   is_num_of_ca_ue_incremented; 
    /* CA: SCell Selection end */
   /*SPR 20137 Start*/
   rrm_bool_t  ue_sync_status_ind;
   /*SPR 20137 Stop*/
    rrc_ue_ul_sch_config_t      ul_sch_config;			/*!< UL SCH config 					*/
    rrm_ue_proc_queue_t			procedure_queue;		/*!< procedure QUEUE				*/
    rrm_uem_s_tmsi_t            ue_s_tmsi;              /*!< M TMSI                         */
    /* CS-Fallback related changes start */
    /*SPR 22553 FIXED START */
    rrm_ue_ctxt_mod_info_t    ue_context_mod_req;
    /*SPR 22553 FIXED END */
    /* CS-Fallback related changes end */
    rrm_ue_capability_params_t	ue_capability_params;
	/*SPR_17893_START*/
	rrm_utran_capability_enq_info_t    utran_capability_enq_info;
	/*SPR_17893_END*/
    rrm_ue_clpc_params_t        rrm_ue_clpc_params;
    /* SPR 10450 Fix Start */
    rrm_ue_applied_drx_config_data_t   applied_drx_config;
    /* SPR 10450 Fix Start */
    /*SPR 15663 start*/
    rrm_ue_temp_sr_config_data_t  temp_sr_config;
    /*SPR 15663 end*/
    /* SPR 22553 FIXED START*/
    /* Code Remove */
    rrm_sub_profile_id_info_t    rrm_sub_profile_id_info;

    /* SPR 22553 FIXED END*/
    ue_history_t                ue_history;/*!< UE History      */
    /*SPR_19066_START*/
    /* Code deleted */
    /*SPR_19066_END*/
    rrm_ho_restriction_list_t   ho_restriction_list;/*!<   ho_restriction_list       */

    /* UE MEAS CHANGES :STARTS */
    rrm_ue_meas_config_t           *p_meas_config; 

    /* SPR 12351 Start */
    U16                            meas_config_sent_bitmask;
    /* SPR 12351 End */
    rrm_ue_meas_result_serv_cell_t meas_result_serv_cell;
    U16 			   cgi_req_trans_id;
    /* UE MEAS CHANGES :ENDS */
    /* MC,MR & HO START  */
    rrm_ue_meas_config_status_et meas_status;/*!< Measurement configuration Status Success/failure*/
    meas_report_info_list_t     meas_report_list;/*!< Measurement Report List */
    U8                          meas_object_id_list[MAX_MEAS_OBJECT_ID - HO_MEAS_START_ID + 1];/*!< Measurement Object Id list */
    U8                          report_config_id_list[MAX_MEAS_OBJECT_ID - HO_MEAS_START_ID + 1];/*!<Report Config Id List */
    U8                          meas_report_id_list[MAX_MEAS_OBJECT_ID - HO_MEAS_START_ID + 1];/*!<Measurement Report Id list */
    rrm_ho_params_t             ho_params;  /*!< ho related params       */
    RRM_TIME_T                  ue_start_time; /*!< ue started time */ 
    /* SPR 16406 8SEP start */
    /* Code removed */
    /* SPR 16406 8SEP end */
    rrm_trigger_quantity_et trigger_quantity; /*!< To store the trigger quantity of the latest Meas report */
    rrm_ue_block_params_t   ue_block_params;
    U16           mu_mimo_bitmask;
    U16           a2_trigger_quantity_bitmask; /*!< To store RSRP & RSRQ received status */
    U16           a1_trigger_quantity_bitmask; /*!< To store RSRP & RSRQ received status */
    U16           a3_trigger_quantity_bitmask; /*!< To cancel A3 event received before A2 event */
    rrm_meas_config_t        *p_meas_config_of_ho_adm; /*!< To store the meas config of ho adm req msg */
    /* MC,MR & HO END  */
    /*LTE_RRM_TM_SWITCH_START*/
    rrm_ue_mac_report_t			*p_ue_mac_report;
    rrm_ue_phy_config_information_t	ue_phy_config_information;    
    /*LTE_RRM_TM_SWITCH_END*/ 
    /* Uplink power control starts */
    /* + SPR 20078 */
    PeriodicReportInfo    ue_avg_stats; 
    /* - SPR 20078 */
    rrm_phy_uplink_power_control_dedicated_t ue_uplink_power_control_dedicated; 
    /* Uplink power control end */	 
    /* BLR Team HO status report to SON Changes Start */
    rrmuem_rmif_ho_report_t ue_handover_report;    /*<  To store handover report to be sent to SON */
    /* BLR Team HO status report to SON Changes End */
    
    /* Force Lock / Grace lock changes*/
    rrm_ue_ho_state_et      ue_ho_current_state;
    rrm_ue_release_cause_et ue_release_cause;	
    /* handling of the ERB_SETUP_REQ after A2 report */

    /* SPS related changes start */
    rrm_ue_sps_data_t             ue_sps_data;      /* contains data related to SPS */
    /* SPS related changes end */

    /* SPR 16406 Start */
    /* Code Removed */
    /* SPR 16406 End */
    /*start csg membership*/
    rrm_csg_membership_t rrm_csg_membership;
    /*end csg membership*/

    /* SPR 314 start */
    rrm_cell_type_t   target_cell_type;
    /* SPR 314 end */
    
    /*ECN load start*/
    rrm_ue_ecn_list_t ue_ecn_list;
    /*ECN load end*/
    /*CDMA2000 changes*/
    rrm_ue_meas_obj_cdma2000_info_t    *p_meas_obj_cdma2000_info;
#define RRM_CDMA2000_CELL_TYPE 2
    /* 0-1XRTT, 1-HRPD */
    rrm_ue_meas_results_info_t         meas_results_info[RRM_CDMA2000_CELL_TYPE];
    rrm_ue_cdma2000_ho_cmd_resp_info_t      cdma2000_ho_cmd_resp;
    /*CDMA2000 changes*/
    rrc_handover_type_et ho_type;
    rrm_lte_ho_type_et ho_typ;
    /*SPR 5169 end*/
    rrm_ue_cdma2000_capabilities_t  cdma2000_capabilities;
    rrm_ue_irat_priority_t  irat_priority;

    /* MAX_CID Changes Starts */
    rrm_map_drb_rohc_info_t   map_data_drb_rohc;
    /* MAX_CID Changes End */
    /* SPR 17850 Fix Start */
    /* Code Deleted */
    /* SPR 17850 Fix End */
    /* UE positioing */
    rrm_ue_positioing_data        *p_ue_positioing_data;
    rrm_ue_proc_queue_t           esmlc_meas_req_queue;

    /* UE Positioning end */
    /*  DYNAMIC ICIC CHANGES START  */
    rrm_ue_dynamic_icic_info_t  dynamic_icic_info;
    /*  DYNAMIC ICIC CHANGES END  */
    /* SPR 21446 start */
    /* Code removed */
    /* SPR 21446 end */
    /*Link Adaptation changes start*/
    ue_speed_related_info_t     ue_speed_algo_param;
    /*Link Adaptation changes start*/
    /* RACH_OPTIMIZATION_CHANGES_START */
    rrm_ue_temp_rach_data_t      temp_rach_data;
    /* RACH_OPTIMIZATION_CHANGES_END */
    rrm_configured_meas_gap_info_t       meas_gap_config;
    /*spr 7466 start*/
    rrm_ue_erab_to_be_released_item_list_t  erab_to_be_released_item_list;
    rrm_lipa_ho_required_t      lipa_ho_required;
    /*spr 7466 end*/
    rrm_timer_t csg_subscription_expiry_timer;
    /* SPR 12897 Start */
    rrm_ue_cdrx_status_et    cdrx_status;
    /* SPR 12897 End */
    /* Fix for CSR: 00059124 */
    /* SRVCC SUPPORT START */
    rrm_srvcc_config_info_t       srvcc_config_info;
    /* SRVCC SUPPORT END */
    /* SPR 16406 8SEP start */
    /* Code removed */
    /* SPR 16406 8SEP end */
    /* Fix for CSR 58972 start*/
    U16   pci_for_which_cgi_required;
    /* SPR_15282_Fix_Start */
    /* CSR_97730_fix: start */
    rrm_cgi_trigger_cause_et    cgi_reporting_triggered_for_meas_event;
    /* CSR_97730_fix: end */
    /* SPR_15282_Fix_End */
    /* SPR 16406 18sep Start */
    /* Code Removed */
    /* SPR 16406 18sep End */
    /* Fix for CSR 58972 end*/

    /* SPR_14564_fix: start */
    rrm_ue_rat_et rrm_cgi_triggered_for_rat_type;
    U32           inter_freq_earfcn_for_cgi;
    /* SPR_14564_fix: end */
    /*CA HARDENING CHANGES 6.3.0*/
    rrm_ue_rat_et rat_configured;
    /*SPR-604 Fix Starts*/
    /* SPR 22553 FIXED START*/
    rrm_ue_ctxt_mod_info_t    ue_context_mod_old_params;
    /* SPR 22553 FIXED END*/
   
    /*SPR-604 Fix Ends*/
    /* SPR-12035-fix start */
    meas_type_et                   meas_type;
    /* SPR-12035-fix end */

    rrm_ue_scell_add_params_t  ue_scell_add_params;
    rrm_ca_data_t        ca_data;          /* Structure for storing the CA related data items */
    /* CA: Stage3 HO code start */ 
    U8                              scell_candidate_count;
    rrm_candidate_cell_info_t       candidate_scell_info[RRM_EIGHT];
    /* CA: Stage3 HO code end */
    rrm_ue_allocated_info_t     allocated_info_per_ue;

    rrm_gu_group_id_t           gu_group_id;
    /*SPR 10329:end*/

    /* eICIC_PHASE_1_2_CHANGES_START */
    rrm_ue_eicic_info_t         rrm_ue_eicic_info;
    rrm_ue_eicic_meas_ind_t     eicic_meas_result;
    /* SPR_17047_START*/
    U16  cgi_trigerred_for_earfcn;
    rrm_meas_gap_status_et  meas_gap_sent_status;
    /* SPR_17047_END*/
    /*spr 17437 fix start*/
   rrm_adm_ctrl_record_t       adm_ctrl_rec;
   /*spr 17437 fix stop*/
    /* SPR 16406 Start */
 	rrm_cell_selection_info_t rrm_cell_selection_info;
    /* SPR 16406 8SEP start */
    /*SPR 18602 START*/
    U64     rrm_additional_bitrate_setup_mod;
    U64     rrm_reduced_bitrate_setup_mod;
    /*SPR 18602 END*/
    /* Code removed */
    /* SPR 16406 8SEP end */
    /* SPR 16406 End */
    /* SPR_21459 Fix - Start */
    U8          mme_id;
    /* SPR_21459 Fix - End */
#ifdef LTE_EMBMS_SUPPORTED
    rrm_ue_mbms_data_t    ue_mbms_data;
#endif


#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    rrm_bool_et               is_sgnb_param_reqd; 
/*NR_DC Code Change Stop*/
    /* MENB_CHANGES_START */
    band_combination_index_list_t   allowed_bc_list_mrdc;

    U8                        split_bearer_threshold_exceed_count;
    U8                        split_bearer_threshold_not_exceed_count;
    rrm_bool_t                            is_drx_configured;
    rrm_bool_t                            is_short_drx_configured;
    rrc_long_drx_cycle_start_offset_r15_t long_drx_cycle_start_offset_r15;
    rrc_short_drx_t                       short_drx;
/* NR_BAND_FIX_START */
    U64                                   nr_band_bitmap_fdd[MAX_NR_FREQ_BANDS_OCT];
    U64                                   nr_band_bitmap_tdd[MAX_NR_FREQ_BANDS_OCT];
/* NR_BAND_FIX_END */
    /* MENB_CHANGES_END */
/* ENDC_MEAS_CHANGES_START */
    rrm_endc_nr_data_t   rrm_endc_nr_data;
/* ENDC_MEAS_CHANGES_END */
#endif
}rrm_ue_context_t;
/* SPR 21496 Fix End */ 

/* SPR 15674 Start */
/* SPR 16406 18sep Start */
#define RRM_PHY_CELL_ID_INFO_CGI_PRESENT 0x01
/* SPR 19364 start */
#define RRM_CELL_DTM_HO_SUPPORTED 		    0x02
#define RRM_CELL_DTM_SUPPORTED 	            0x04
/* SPR 19364 end */
typedef struct _phy_cell_id_info_t
{
    U8 bitmask;
        U16 phy_cell_id;
        U32 carr_earfcn;
    /* SPR 16406 8SEP start */
    rrm_bool_t is_cell_blk_list;
    rrm_bool_t is_cell_confused;
    /* SPR 16406 8SEP end */
    rrm_oam_eutran_global_cell_id_t cgi;
    /* SPR 16406 18sep End */
}phy_cell_id_info_t;
/* SPR 15674 End */

/* for meas gap config start*/
/*! \  rrm_gap_offset_repository_t
 *  \brief This struct is used for GAP offset repository 
 *  \param offset
 *  \param offset_count
 */
typedef struct
{
    U16 offset;
    U16 offset_count; /*^ M, 0, B, 1, 6 ^*/
}rrm_gap_offset_repository_t;

/* for meas gap config end */

/*! \  rrm_ue_erb_t
 *  \brief Sructure to identify a ERAB uniquely among all ERABs for all UEs 
 *  \param rrm_ue_context_t *p_rrm_ue_context
 *  \param U8 erab_id
 */
typedef struct _rrm_ue_erb_t
{
    rrm_ue_context_t 		*p_rrm_ue_context;
    U8 				erab_id;
    /*Bug_721_changes_start*/
    rrm_bool_et      is_erb_selected_for_rel;
    /*Bug_721_changes_end*/
}rrm_ue_erb_t;

/*! \ rrm_ue_erb_list_t
 *  \brief Node of the List conataining ERABs
 *  \param pNode YLNODE
 *  \param ue_erb rrm_ue_erb_t
 */
typedef struct _rrm_ue_list_t
{
    YLNODE 			pNode;
    rrm_ue_erb_t		ue_erb;
}rrm_ue_erb_list_t;

/*! \  struct rrm_ue_priority_node_t
 *  \brief A node of a list conating UEs
 *  \param pNode YLNODE
 *  \param p_rrm_ue_context rrm_ue_context_t
 */
typedef struct _ue_priority_node_t
{
    YLNODE                      pNode;
    rrm_ue_context_t		*p_rrm_ue_context;
}rrm_ue_priority_node_t;

typedef enum
{
    RRM_PE_INDEX,
    RRM_PE_CELL_SPECIFIC_INDEX,
    RRM_PE_UE_SPECIFIC_INDEX,
    RRM_PE_ERAB_SPECIFIC_INDEX,
    RRM_PE_MEAS_INFO_INDEX,
    RRM_PE_HO_OUT_INDEX,
    RRM_PE_HO_IN_INDEX,
    RRM_PE_OTHERS_INDEX,
    RRM_EE_INDEX,
    RRM_EE_MEMORY_SPECIFIC_INDEX,
    RRM_EE_TIMER_SPECIFIC_INDEX,
    RRM_EE_SEND_RECD_SPECIFIC_INDEX,
    RRM_EVENT_MAX_INDEX
}rrm_event_class_et;

typedef enum
{
    EVENT_ACTION_LOG,
    EVENT_ACTION_NOTIFY,
    EVENT_ACTION_NOTIFY_LOG,
    EVENT_ACTION_NONE
}rrm_action_prfmd_et;


typedef struct _rrm_event_config_t
{
    rrm_bool_et           ee_subcls;
    rrm_bool_et           pe_subcls;  
    rrm_action_prfmd_et   action_prfmd[RRM_EVENT_MAX_INDEX];
}rrm_event_config_t;
/* RACH_OPTIMIZATION_CHANGES_START */

#define STRONG_CELL_INFO_COUNT 3
typedef struct _rrm_ue_rach_info_t
{
    prach_cause_et        cause;
    U16                   number_of_preambles_sent;
    rrm_bool_et           contention_detected;
    rrm_ue_position_et    ue_location;
    U8                     strong_cell_count;
    strong_cell_info_t    strong_cell_info[STRONG_CELL_INFO_COUNT];
}rrm_ue_rach_info_t;

typedef struct _rrm_ue_rach_info_list_t
{
    YLNODE          pNode;
    rrm_ue_rach_info_t              ue_rach_info;
}rrm_ue_rach_info_list_t;

typedef YLIST rach_report_list_t;

typedef struct _rrm_uem_rach_info_data_t
{
    rrm_cell_index_t           cell_index;
    rrm_bool_et                is_ue_rach_report_active;
    U16                        ue_reports_periodicity;
    U16                         rach_sample_count;
    U8                          strong_cell_info_count;
    rach_report_list_t                      rach_report_list;
    rrm_ue_rach_info_list_t              ue_rach_info_list;
}rrm_uem_rach_info_data_t;
/* RACH_OPTIMIZATION_CHANGES_END */
/* SPR 8315 FIX START*/
typedef struct _mac_ue_reconf_seq_node_data_t
{
    rrm_ue_context_t                   *p_ue_context;          /*!< UE context data for list       */
}mac_ue_reconf_seq_node_data_t;

typedef struct _mac_ue_reconfig_sequence_node_t
{
    YLNODE                          pNode;
    mac_ue_reconf_seq_node_data_t   mac_ue_reconf_seq_node_data;
}mac_ue_reconfig_sequence_node_t;
/* SPR 8315 FIX END*/

/* Carrier Aggregation start*/

/* BUG_11907_FIX_START */
typedef rrm_return_et (*register_handler)( rrm_cell_index_t ,rrm_void_t *p_api, rrm_void_t *p_gb_ctxt, rrm_void_t *p_data );
/* BUG_11907_FIX_END */

typedef struct _rrm_transaction_table_t
{
    S16               transaction_id;
    register_handler  handler;
    rrm_void_t          *p_data;
		
}rrm_transaction_table_t;

/* Carrier Aggregation end */

/*! \  rrm_ue_global_context_t
 *  \brief Global UE context
 *  \param p_out  FILE out put file 
 *  \param log_level Log level for UEM
 *  \param log_enabled Log Enable/Disable
 *  \param p_ue_context UE contex in global context
 *  \param trans_id Transaction ID
 *  \param p_data API data to store in global
 *  \param p_api_data pointer to the API data buffer
 *  \param data_length Data length for API 
 *  \param api_id API ID
 *  \param clpc_ue_counter CLPC counter
 */
typedef struct _rrm_ue_global_context_t
{
    FILE                     	*p_out;			/*!< FILE out put file			*/
    U8                       	log_level;		/*!< Log level for UEM			*/
    U8                          log_enabled;            /*!< Log Enable/Disable         	*/
    rrm_ue_context_t         	*p_ue_context;		/*!< UE contex in global context	*/
    U16                       	trans_id;		/*!< Transaction ID			*/
    void                     	*p_data;		/*!< API data to store in global	*/
    void                        *p_api_data;		/*!< pointer to the API data buffer	*/
    U16                       	data_length;		/*!< Data length for API 		*/
    U16 			api_id;			/*!< API ID				*/
    U16                         clpc_ue_counter;
    rrm_event_config_t          event_config;           /*!< Event Configuration Params         */
    /*  DYNAMIC ICIC CHANGES START  */
    rrm_cellm_uem_ue_update_data_t rrm_cellm_uem_ue_update_data;   
    /*  DYNAMIC ICIC CHANGES END  */
    /* RACH_OPTIMIZATION_CHANGES_START */
    rrm_uem_rach_info_data_t       rrm_uem_rach_info_data;
    /* RACH_OPTIMIZATION_CHANGES_END */
    /* SPR 8315 FIX START*/
    YLIST                        mac_ue_reconfig_sequence_list;
    /* SPR 8315 FIX END*/
   /* Carrier Aggregation start */
    rrm_transaction_table_t      tb_block[50]; 
   /* Carrier Aggregation end*/
   /*CA Stage3:start*/
   rrm_ue_proc_queue_t      procedure_queue;		/*!< procedure QUEUE				*/
   rrm_void_t             	*p_proc_data;			/*!< Void pointer to proc data		*/
   /*CA Stage3:end*/
   /* eICIC_PHASE_1_2_CHANGES_START */
   rrm_victim_ue_info_t           victim_ue_info;
   rrm_aggressor_node_info_t      aggressor_node_info;
   /* eICIC_PHASE_1_2_CHANGES_END */

}rrm_ue_global_context_t;

/*! \  ue_erab_priority_list[30]
 *  \brief A list containing ERABs priority wise
 *         Fist Half contains Preemptable ERBs and Second Half contains NON Preemptable ERbs
 */
extern YLIST			ue_erab_priority_list[30]; 

/*! \  ue_priority_list[30]
 *  \brief A list containing UEs priority wise
 */
extern YLIST			ue_priority_list[30];
/* SPR 22248 MBMS Changes Start */
typedef struct _ue_context_list_st
{
    U16 						ue_context_id;			/*!< UE context ID for a UE			*/
    rrm_ue_context_t 			*p_ue_context;			/*!< UE context data for list		*/
    struct _ue_context_list_st   *next;					/*!< Next node for the UE list		*/
}ue_context_list_t;
/* SPR 22248 MBMS Changes Stop */
/* SPR 21426 +- */
/*! \ ue_ho_lst_node_t
 *  \brief This struct is used for HO list Node information
 *  \param node Node
 *  \param p_ue_context UE context data for list
 */
typedef struct _ue_ho_lst_node
{
    YLNODE            node;
    rrm_ue_context_t  *p_ue_context;          /*!< UE context data for list       */
}ue_ho_lst_node_t;

/**TNL feature */

/*! \fn         void rrm_ue_calculate_total_bit_rate(rrm_ue_context_t  *p_ue_context,U64  *ul_total_gbr_ue,U64  *dl_total_gbr_ue)
 *  \brief      To calculate the total bit rate for.
 *  \param  p_ue_context
 *  \param  ul_total_gbr_ue
 *  \param  dl_total_gbr_ue 
 */
void
rrm_ue_calculate_total_bit_rate
(
 rrm_ue_context_t         *p_ue_context,
 U64                      *ul_total_gbr_ue,
 U64                      *dl_total_gbr_ue 
 );
/*! \fn         U64 rrm_calculate_required_bit_rate()
 *  \brief      calculate required bit rate
 */
/* + SPR 17439 */
U64
rrm_calculate_required_bit_rate(void);
/* - SPR 17439 */

/**TNL feature*/

/*! \fn rrm_return_et rrm_ue_get_low_priority_erb(rrm_ue_erb_t *p_ue_erb,rrm_ue_context_t *p_ue_context,rrm_ue_erab_setup_item_t *p_erab_to_be_setup)
 *  \brief      To get low priority erb
 *  \param[in]  p_ue_erb
 *  \param[in]  p_ue_context
 *  \param[in]  p_erab_to_be_setup
 */
rrm_return_et
rrm_ue_get_low_priority_erb(
        rrm_ue_erb_t *p_ue_erb,
        rrm_ue_context_t *p_ue_context,
        rrm_ue_erab_setup_item_t *p_erab_to_be_setup
        );
/* SPR 21426 Start */
/*! \fn  rrm_ue_context_t* rrm_ue_find_context(rrm_ue_index_t ue_index,rrm_cell_index_t cell_index)
 *  \brief      To find context
 *  \param[in]   ue_index
 *  \param[in]  cell_index
 */
/* SPR 21426 End */
/* SPR 20652 Fix Start */
rrm_ue_context_t *
rrm_ue_find_context(
        rrm_ue_index_t   ue_index,
        rrm_cell_index_t cell_index
        );
/* SPR 20652 Fix End */


/*emergency support */
/*! \  rrm_ue_erb_release_queue_t
 *  \brief
 */
typedef YLIST rrm_ue_erb_release_queue_t;

/*LTE_RRM_TM_SWITCH_START*/
/*! \fn rrm_return_et rrm_ue_set_phy_antenna_info(rrm_ue_index_t ue_index,rrm_cell_index_t cell_index,const rrm_ue_phy_antenna_information_t *p_ue_antenna_info)
 *  \brief      To set the UE anetnna information
 *  \param[in]     ue_index
 *  \param[in]   cell_index
 *  \param[in]  p_ue_antenna_info
 */
rrm_return_et
rrm_ue_set_phy_antenna_info(rrm_ue_index_t      ue_index,
        rrm_cell_index_t    cell_index,
        const rrm_ue_phy_antenna_information_t *p_ue_antenna_info);


/*! \fn       const rrm_ue_phy_antenna_information_t*  rrm_ue_get_phy_antenna_info(rrm_ue_index_t ue_index,rrm_cell_index_t cell_index)
 *  \brief      To set the UE anetnna information
 *  \param[in]  ue_index
 *  \param[in]  cell_index

 */
const rrm_ue_phy_antenna_information_t*
rrm_ue_get_phy_antenna_info(rrm_ue_index_t       ue_index,
        rrm_cell_index_t     cell_index);


/*! \fn       rrm_bool_et rrm_compare_codebook_subset_restriction(const U8 *codebook_subset,const U8 *codebook_subset_stored,U16 size);

 *  \brief      To compare the codebook subset restriction value
 *  \param[in]  codebook_subset
 *  \param[in]  codebook_subset_stored
 *  \param[in]  size
 */
rrm_bool_et
rrm_compare_codebook_subset_restriction(const U8 *codebook_subset,
        const U8 *codebook_subset_stored,
        U16 size);


/*! \fn rrm_return_et rrm_compare_ue_antenna_info(rrm_ue_index_t ue_index, rrm_cell_index_t cell_index, const rrm_ue_phy_antenna_information_t *p_antenna_info, rrm_bool_et *p_bool)
 *  \brief      To compare the UE anetnna information
 *  \param[in]  ue_index
 *  \param[in]  cell_index
 *  \param[in]  p_antenna_info 
 *  \param[out] p_bool
 */
rrm_return_et
rrm_compare_ue_antenna_info(rrm_ue_index_t       ue_index,
        rrm_cell_index_t     cell_index,
        const rrm_ue_phy_antenna_information_t *p_antenna_info,
        rrm_bool_et           *p_bool);

/*! \fn rrm_return_et rrm_backup_ue_antenna_info(rrm_ue_context_t *p_ue_context)
 *  \brief      To reset the UE's last anetnna information
 *  \param[in]   p_ue_context
 */
rrm_return_et
rrm_backup_ue_antenna_info(rrm_ue_context_t   *p_ue_context);


/*! \fn  rrm_return_et rrm_rollback_ue_antenna_info(rrm_ue_context_t   *p_ue_context)
 *  \brief      To reset the UE's last anetnna information
 *  \param[in]   p_ue_context
 */
rrm_return_et
rrm_rollback_ue_antenna_info(rrm_ue_context_t   *p_ue_context);

rrm_return_et
rrm_rollback_ue_antenna_info_v920( rrm_ue_context_t *);

rrm_return_et
rrm_backup_ue_antenna_info_v920( rrm_ue_context_t   *p_ue_context );

rrm_return_et
rrm_compare_ue_antenna_info_v920(const rrm_ue_context_t     *p_ue_context,
        const rrm_ue_phy_antenna_info_v920_t  *p_antenna_info_v920,
        rrm_bool_et           *p_antenna_info_match);

const rrm_ue_phy_antenna_info_v920_t*
rrm_ue_get_antenna_info_v920( const rrm_ue_context_t   *p_ue_context );

rrm_return_et
rrm_set_phy_antenna_info_v920( rrm_ue_context_t  *p_ue_context,
        const rrm_ue_phy_antenna_info_v920_t
        *p_ue_antenna_info_v920 );


/*LTE_RRM_TM_SWITCH_END*/
/*ECN LOAD start*/
rrm_void_t
rrm_sorting_for_bidir_ecn(U8 num_of_usr, rrm_cell_index_t cell_index);

/*! \fn  rrm_return_et rrm_send_pdcp_ecn_bidir_req(rrm_ue_context_t *p_ue_ctxt, U8 dir, U8 cong_status)
 *  \brief This function sends pdcp ecn bidir request
 *  \param p_ue_ctxt ue context
 *  \param dir directory
 *  \param cong_status cong status
 */
rrm_return_et
rrm_send_pdcp_ecn_bidir_req(rrm_ue_context_t *p_ue_ctxt, U8 dir, U8 cong_status);

/* SPR 10869 changes */

/*! \fn rrm_cellm_update_thp_of_ue(rrm_cell_index_t cell_index, rrm_ue_index_t ue_index,
  U64 max_dl_thp,U64 max_ul_thp)
 *  \brief this function updates the thp of ue
 *  \param cell_index cell index
 *  \param ue_index ue index
 *  \param max_dl_thp maximum downlink thp
 *  \param max_ul_thp maximum uplink thp
 */ 
void
rrm_cellm_update_thp_of_ue (rrm_cell_index_t cell_index,
        rrm_ue_index_t ue_index,
        U64 max_dl_thp,
        U64 max_ul_thp);

/*! \fn void rrm_sorting_ue_for_ecn_dl(U8 num_of_user)
 *  \brief Sorting ue for ecn dl
 *  \param num_of_user number of user
 */
rrm_void_t
rrm_sorting_ue_for_ecn_dl(U8 num_of_user, rrm_cell_index_t cell_index);

/*! \fn rrm_sorting_ue_for_ecn_ul(U8 num_of_usr)
 *  \brief Sorting ue for ecn ul
 *  \param num_of_usr number of user
 */ 
rrm_void_t
rrm_sorting_ue_for_ecn_ul(U8 num_of_usr, rrm_cell_index_t cell_index);

/*ECN Load end*/

/* MRO changes start */
/*! \fn rrm_return_et rrm_ue_handle_rlf_ind(rrm_ue_global_context_t *p_rrm_glb_ctxt)
 *  \brief This function handle ue rlf index
 *  \param p_rrm_glb_ctxt rrm global context
 */
rrm_return_et
rrm_ue_handle_rlf_ind (
        rrm_ue_global_context_t *p_rrm_glb_ctxt
        );
/* MRO changes end */

rrm_return_et
rrm_ue_intra_enb_ho_ind_event (
        rrm_ue_global_context_t *p_rrm_glb_ctxt
        );

rrm_return_et
rrm_ue_send_drb_ecn_ce_req(
        RrmPdcpDrbEcnCeReq          *p_pdcp_drb_ecn_req,
        U16 src_module_id,
        U16 transaction_id,
        U8  cell_index);
rrm_return_et 
rrm_verify_ue_for_inter_freq_meas(
rrm_ue_context_t *p_ue_ctxt, rrm_bool_et is_meas_for_masa);
typedef struct _rrm_best_cell_for_ue_node_t
{
    YLNODE p_node;
    U8 tgt_cell_capacity;
    U16 earfcn;
    rrm_bool_et meas_ind_rcvd; 
    rrm_bool_et pci_found;
}rrm_best_cell_for_ue_node_t;

rrm_bool_et
rrm_uem_is_cell_capable(rrm_ue_context_t *p_rrm_ue_ctxt, inter_freq_cells_t *p_int_freq_cell);

rrm_bool_et
rrm_get_cell_specific_info_carrier_redirect(
        U8  meas_id,         /* IN  MeasID obtained in meas res ind */
        U16 phy_cell_id,    /* IN  physical cell ID obtained in meas results ind */
        rrm_cell_meas_data_t*  p_meas_data, /* OUT Parameters that are configured for this PCI  */
        rrm_ue_context_t*      p_rrm_ue_context,
        /* Fix for CSR 58972 start*/
        rrm_bool_et *p_pci_confusion_detected
        /* Fix for CSR 58972 end*/
        );

rrm_return_et
rrm_ue_handle_lppa_ecid_meas_init_req(
        rrm_ue_global_context_t *p_rrm_glb_ctxt
        );
rrm_return_et
rrm_ue_handle_lppa_ecid_meas_termination_cmd(
        rrm_ue_global_context_t *p_rrm_glb_ctxt
        );
rrm_return_et
rrm_ue_handle_lppa_error_ind(
        rrm_ue_global_context_t *p_rrm_glb_ctxt
        );

rrm_return_et
rrm_ue_handle_inter_freq_meas_ind(
        rrm_ue_global_context_t *p_rrm_glb_ctxt
        );


rrm_return_et 
rrm_map_rrm_rrc_registered_lai_t(
        rrm_uem_registered_lai_t *p_ue_register_lai,
        registered_lai_t *p_rrc_ue_erb_registered_lai
        );

rrm_return_et
rrm_map_rrm_rrc_plmn_identity(
        rrm_uem_plmn_identity_lai_t *p_ue_plmn_id,
        plmn_identity_lai_t *p_rrc_ue_erb_plmn_id
        );

rrm_return_et
rrm_map_rrm_rrc_lac(
        rrm_uem_lac_t *p_ue_register_lac,
        lac_t *p_rrc_ue_erb_lac
        );
/* LIPA SIPTO start */
rrm_return_et
rrm_ue_handle_local_error_ind(
        rrm_ue_global_context_t *p_rrm_glb_ctxt
        );

rrm_return_et
rrm_ue_handle_lipa_ho_required
(
 rrm_ue_global_context_t *p_rrm_glb_ctxt
 );
/* LIPA SIPTO end */

/* Review_comment_for_SPR_7996_nd_8345_start */
rrm_void_t
rrm_decrement_num_of_enb_init_ho_csg_and_non_csg_usr(
        rrm_ue_context_t    *p_ue_ctxt
        );

rrm_void_t
rrm_increment_num_of_enb_init_ho_csg_and_non_csg_usr(
        rrm_ue_context_t    *p_ue_ctxt
        );
/* Review_comment_for_SPR_7996_nd_8345_end */

/* CSR-58333-fix start*/
/******************************************************************************
 *   Timer management functions
 ******************************************************************************/

rrm_timer_t
rrm_ue_start_timer (
        U32         timer_duration,
        void        *p_timer_data,
        U16         timer_data_size,
        rrm_bool_t  is_repeated  
        ); 

void
rrm_ue_stop_timer (
        rrm_timer_t timer
        );

void*
rrm_ue_get_timer_data (
        rrm_timer_t timer
        );

/* CSR-58333-fix end*/

/* Fix for CSR 58972 start*/
rrm_bool_et
rrm_is_ue_supporting_autonomous_gap
(
 rrm_ue_context_t *p_ue_context
 );
/* SPR_15259_fix: start */
rrm_bool_et
rrm_is_ue_supporting_autonomous_gap_for_ho
(
    rrm_ue_context_t *p_ue_context,
    rrm_ue_rat_et    rrm_cell_rat_type
);
/* SPR_15259_fix: end */
/* Fix for CSR 58972 end*/
/*Bug:9923*/
rrm_return_et
rrm_ho_to_utra_daho(
        rrm_ue_context_t  *p_ue_context,
        U16   trans_id
        );


/*26_March_Fix:Start */
rrm_return_et
rrm_ho_to_geran_daho(
        rrm_ue_context_t  *p_ue_context,
        U16   trans_id
        );

/* Start fix for SPR 14987 */
rrm_bool_et
is_ue_supported_fgi_for_cdrx
(
    rrm_ue_context_t *p_ue_context
);
/* End fix for SPR 14987 */
/*SPR 15713 Fix Start*/
rrm_return_et
rrm_ue_rac_release_sr_res_index
(
 rrm_ue_context_t *p_rrm_ue_ctxt
 );

/*SPR 15713 Fix End*/

/*26_March_Fix:End */
/*Bug:9923*/
/*CA Stage3: start*/
typedef rrm_return_et (*rrm_meas_func_ptr)(rrm_ue_global_context_t *);
/*CA Stage3: end*/
/* SPR:12849 Start */ 
typedef YLIST rrm_cell_to_add_list_t;
/* SPR:12849 End */ 



/* MC,MR & HO START  */
/*! \  meas_report_info_node_t
 *  \brief Structure storing the meas report info
 *  \param sNode 
 *  \param meas_object_id mesurement object id of the corresponding meas object
 *  \param meas_id measurement id of the corresponding measurement results object
 *  \param report_config_id report config id of the report config object
 *  \param trigger_quantity configured trigger quantity of the event
 *  \param reporting_event configured reporting event
 *  \param meas_type whether measurement type as HO or ANR
 *  \param thresh1 configured threshold value of the event
 *  \param thresh2 configure threhold value of A5 event
 *  \param meas_status result of measurement status of the event
 *  \param offset
 *  \param thresh_rscp configured the threshold for B1/B2 rscp 
 *  \param thresh_ecn0 configured the threshold for B1/B2 ecn0
 *  \param carrier_earfcn configured in measurement object
 *  \param band_indicator corresponding to the carrier frequency
 */
typedef struct _meas_report_info_node_t 
{
    YLNODE                        sNode;    /*!< Node for the YLIST */
    U8                           meas_object_id; /*!< Meas Object ID */
    U8                           meas_id;        /*!< Meas ID */
    U8                           report_config_id;/*!< Report Config ID */
    rrmc_trigger_quantity_et     trigger_quantity; /*!< NULL/BOTH/RSRP/RSRQ */
    rrmc_meas_report_event_et    reporting_event; /*!< EVENT_NULL, EVENT_A1, A2, A3, A4, A5, B1, B2 */
    meas_type_et                   meas_type; /*!< Measurement Type ANR/HO */
    U8                           thresh1;    /*!<  Threshold for A1,A2 & thresh1 for A5 Calculation */
    U8                           thresh2;    /*!<  Threshold2 for A5 Calculation */
    rrm_ue_meas_config_status_et   meas_status; /*!< config status (SUCCESS/FAILURE) */
    S8                           offset; /*!< Offset */
    S8                           thresh_rscp; /* Threshold for B1/B2 rscp */
    U8                           thresh_ecn0; /* Threshold for B1/B2 ecn0 */
    U8                           thresh_rssi;/* Threshold for B1/B2 rssi for GERAN */
    U32                          carrier_earfcn; /* Stores the carrier frequency configured */
    U8                           band_indicator; /*Stores the band indicator for GERAN / CDMA */
    U32                          utra_type;   /* UTRA type FDD or TDDD */ 
/*CA stage3:start*/
    rrm_bool_et			 eicic_a4_meas_flag; /* boolean flag to check a4 trigger of victim UE */	
/* eICIC_PHASE_1_2_CHANGES_END */
/*CA stage3:start*/
    rrm_meas_func_ptr            register_meas_func_type;
/*CA stage3:end*/
/* SPR:12849 Start */ 
    rrm_cell_to_add_list_t      cells_to_add_mod_list;
/* SPR:12849 End */ 
    /*SPR_17893_START*/
    /*utran_type is changed to rat_type. Currenly it is populated for the UTRAN rat,
     * and remains unpopulated for other rats*/
    rrm_irat_priority_et         rat_type; 
    /*SPR_17893_END*/
    /* SPR 15860 Start */
    /* SPR 15861 Start */
    /* SPR 15862 Start */

    /// This flag is added to track the measurement report status. we shouldn't release the 
    /// DRX configuration if at least one report is pending
    rrm_bool_et                  is_meas_report_pending; /// RRM_TRUE - if mesurement report is pending
                                                         /// RRM_FALSE - otherwise


    /* SPR 15860 End */
    /* SPR 15861 End */
    /* SPR 15862 End */
    /*SPR 16292 Fix Start*/
    rrm_bool_et                 is_ask_cgi_configured_via_intra_a3;
    /*SPR 16292 Fix Stop*/


/* ENDC_MEAS_CHANGES_START */
#ifdef ENDC_ENABLED
    U8                           b1_nr_threshold_rsrp_r15;
    U8                           b1_nr_threshold_rsrq_r15;
    U8                           b1_nr_threshold_sinr_r15;
#endif
/* ENDC_MEAS_CHANGES_END */

}meas_report_info_node_t;
/*CA stage3:start*/
typedef struct _rrm_scell_active_deactive_t
{
    U8 scell_active_count;
    U8 scell_deactive_count;/*number of deactived cell which can be activated on need basis*/
    U8 scell_deactive_index[RRM_MAX_SCELL];
}rrm_scell_active_deactive_t;

rrm_return_et
rrm_scell_check_add_meas_config_req(rrm_ue_context_t *p_ue_context);

rrm_void_t
rrm_check_active_scell(rrm_ue_scell_add_params_t *p_scell_add_param,
    rrm_scell_active_deactive_t *p_scell_active_deactive_list);

rrm_return_et
rrm_fill_a1_a2_meas_config_object(
        rrm_meas_config_t *p_meas_config,
        rrm_ue_context_t  *p_rrm_ue_ctx,
        rrm_cell_index_t cell_index);
rrm_return_et
rrm_uem_masa_meas_config_ind(rrm_ue_global_context_t *p_rrm_glb_ctxt);
rrm_return_et
/*SPR 16764 START*/
rrm_uem_meas_results_ind(rrm_ue_global_context_t *p_rrm_glb_ctxt);
/*SPR 16764 END*/
rrm_return_et
rrm_ue_chk_is_meas_config_for_masa( U16 meas_id,rrm_ue_global_context_t *p_rrm_glb_ctxt);

rrm_return_et
rrm_ue_glb_enqueue_req(
        rrm_void_t *p_api_buf,
        rrm_ue_global_context_t *p_gl_ctx
        );

/*SPR 21479 Fix start*/
rrm_ue_procedure_record_t * 
rrm_ue_glb_dequeue_req(
        rrm_ue_proc_queue_t * procedure_queue
        );
/*SPR 21479 Fix end*/

rrm_void_t
rrm_process_mac_ue_reconfig_for_ca(
        rrm_cell_index_t          cell_index,
        RrmMacUEReConfigCnf *p_mac_ue_reconf_cnf);
rrm_bool_et
rrm_check_freq_already_configured_for_ca(inter_freq_cells_t *p_inter_freq, 
                                         rrm_ue_context_t *p_ue_ctxt);

/*CA stage3:end*/

rrm_void_t rrm_ue_scell_state_backp( rrm_ue_context_t   *p_ue_context);

/* BUG_11907_FIX_START */
typedef enum
{
    RRM_UE_RECONFIG_FOR_CELL_STOP,
    RRM_UE_RECONFIG_FOR_OTHER
}rrm_ue_reconfig_type_et;

typedef struct _rrm_ue_reconfig_cell_stop_params_t
{
    rrm_ue_reconfig_type_et    ue_reconfig_reason;
    rrm_bool_et                subsequent_resp;
    rrm_cell_index_t           pcell_index;
    rrm_cell_index_t           scell_index;
    /*SPR 13316:start*/
    U16                        trans_id;
    /*SPR 13316:end*/
}rrm_ue_reconfig_cell_stop_params_t;
/* BUG_11907_FIX_END */

/*SPR 11535:start*/
typedef struct _rrm_ue_inactive_t
{
    rrm_ue_index_t ue_index;
    rrm_cell_index_t cell_index;
}rrm_ue_inactive_t;
rrm_return_et rrm_ue_handle_rrc_ue_inactivity(
  rrm_ue_global_context_t *p_rrm_glb_ctxt );
/*SPR 11535:end*/

/* Bug_12882_fix: Start */
rrm_void_t rrm_fill_ca_lc_bandwidth_for_scell(rrm_ue_context_t *p_rrm_ue_context,
                                              rrm_scell_config_t *p_scell_config);
/* Bug_12882_fix: End*/
#endif
