/*!
 * \file        rrm_api_defines.h
 * \author      gur20435
 * \date        07 Mar 2012
 * \brief       This file contains the rrm definitions for external modules.
 */
 /*!
  *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
  */

#ifndef _RRM_API_DEFINES_H_
#define _RRM_API_DEFINES_H_
/*! \  headerfile stacklayer.h <> */
#include "stacklayer.h"


#define RRM_MAX_QCI                                             9
#define MAX_S1U_QOS_PROFILE_DATA_OCTET_SIZE 	                254
#define MAX_MCC_DIGITS 						3 
#define MAX_MNC_DIGITS 						3    
#define NUM_CSG_OCTETS 						4
#define RRMCM_MAX_HNB_ID 				    	48
#define RRM_MAX_NO_INT_MODULES 					3
#define RRM_API_HEADER_SIZE 					16
/* SPR 22248 MBMS Changes Start */
#define MAX_SCHEDULING_INFO_LIST 				12 /* RRC accepts only upto 12 */
/* SPR 22248 MBMS Changes Stop */
#define MAX_SI_MSG_NUM 						    31 /* RRC accepts only upto 31 */
#define MAX_SUBFRAME_ALLOCATIONS 				4
#define MAX_MB_SFN_SUBFRAME_LIST 				8
#define MAX_UTRA_CARRIER_FDD 					16
#define MAX_UTRA_CARRIER_TDD                    16
#define MAX_TAC_SIZE 						2
#define MAX_NO_INTER_FREQ 					8
#define MAX_NO_EUTRAN_TO_UTRAN_FDD 				16
#define MAX_NO_GERAN_FREQ_GROUUP 				16

#define MAX_NO_CDMA_FREQ_GROUUP 				16

#define MAX_INTRA_FREQ_CELLS 					64
/** FIX for Buffer overflow start */
//Changing 128 to 64
#define MAX_INTER_FREQ_CELLS                    64
/** FIX for Buffer overflow end */
#define MAX_PLMN_LIST 						6
#define MAX_EAID						3
#define MAX_CID_BYTES 						16
#define MAX_NUM_UMTS_NCL					256
/*bug_9464_changes_start*/
#define MAX_NO_DRX_PROFILE 					9  /* Assuming DRX for nine profiles only - per qci */
/*bug_9464_changes_end*/
#define	MAX_NO_SRB 						2 /* Choice of default configuration or else parameters	*/
#define MAX_CELL_IDENTITY_OCTETS 				4 
#define MAX_NUM_UTF					        4
#define RRM_MAX_M1_VALUES                                       168
#define RRM_MAX_M2_VALUES                                       4 
#define RRM_MAX_N1                              		3
#define RRM_MAX_N2                              		168
#define MAX_NUM_SNR_VALUES                      		20
#define RRM_MIN_EXT_MODULE_ID                   		1
#define RRM_OAM_MODULE_ID                       		RRM_MIN_EXT_MODULE_ID + 0
#define RRM_MAX_EXT_MODULE_ID                   		RRM_OAM_MODULE_ID
#define RRM_MIN_INT_MODULE_ID                   		RRM_MAX_EXT_MODULE_ID + 20

#define RRM_MAX_NUM_UTRA_FDD_CARRIERS          			16
#define RRM_MAX_NUM_UTRA_TDD_CARRIERS                   16
#define RRM_MAX_NUM_UTRA_CARRIERS                       32
#define MAX_GERAN_FREQ                         			16
#define MAX_VAR_BITMAP_OF_ARFCNS               			16
#define MAX_EXPL_ARFCNS                        			31
#define NCC_PERMITTED_OCTET_SIZE               			1
#define MAX_NO_EARFCN                          			100
#define RRM_MAX_NO_CELLS_PER_EARFCN            			32
#define RRM_SON_MAX_GERAN_UTRAN_NBRS_PER_EARFCN         128
#define RRM_MAX_NUM_CELLS                               8       /* Bug 10058 Fix */
/* MLB Changes start */
#define RRM_OAM_MAX_RESOURCE                            4 
#define RRM_MAX_NUM_NGBR_CELLS                          32
/* MLB Changes end */
#define RRM_MAX_KPI                                      8
/* CDMA2000 changes start*/

#define RRMCM_RMIF_MAX_CDMA_BAND_CLASS  32
#define RRMCM_RMIF_XRTT_SID_OCTET_SIZE               2
#define RRMCM_RMIF_XRTT_NID_OCTET_SIZE               2
#define RRMCM_RMIF_XRTT_REG_ZONE_OCTET_SIZE          2
#define RRMCM_RMIF_XRTT_TOTAL_ZONE_OCTET_SIZE        1
#define RRMCM_RMIF_XRTT_ZONE_TIMER_OCTET_SIZE        1
#define RRMCM_RMIF_PRE_REG_ZONE_ID  2
#define RRMCM_RMIF_CDMA2000_1XRTT_CELL_ID_LENGTH 6
#define RRMCM_RMIF_MAX_CGI_CDMA_2000_HRPD   16
#define RRMCM_RMIF_MAX_CGI_CDMA_2000_1XRTT  6
#define RRMCM_RMIF_MAX_CGI_CDMA2000                 16
#define RRMCM_RMIF_NUM_CDMA2000_FREQ_CELLS  72

/* CDMA2000 changes ends */

/*Message API base*/
#define RRM_SON_API_BASE 			 	    	100
#define SON_RRM_API_BASE 				    	200

#define rrm_bitmask_t                          U32
/* SPR 22248 MBMS Changes Start */
#define rrm_bitmask64_t                        U64
/* SPR 22248 MBMS Changes Stop */
/** RRM types start*/
typedef U32             rrm_oam_action_t;
typedef U8              rrm_oam_bool_t;
typedef U16             rrm_error_t;
typedef U16             rrm_length_t;
typedef U16             rrm_counter_t;
typedef U16             rrm_transaction_id_t;
typedef void            rrm_void_t;
typedef size_t          rrm_size_t;
typedef QTIMER          rrm_timer_t;
typedef U16             rrm_timer_duration_t;
typedef U8              rrm_bool_t;
typedef U16             rrm_module_id_t;
typedef U8              rrm_retry_count_t;
typedef U16             rrm_tag_t;
typedef U16             rrm_thread_id_t;
/** RRM types end*/

/* UE MEAS CHANGES : STARTS */

/*! \def RRM_MEAS_RESULT_LATENCY_TIME 
 *  \brief A macro for latency time of UE MEAS REQUEST
 */
#define RRM_MEAS_RESULT_LATENCY_TIME    5

/*! \def MIN_CONFIG_OBJECT
 *  \brief A macro for minimum number of configuration objects
 */
#define MIN_CONFIG_OBJECT           	0

/*! \def MAX_CONFIG_OBJECT
 *  \brief A macro for maximum number of configuration objects
 */
#define MAX_CONFIG_OBJECT           	32

    /* SPR 12929 Fix Start */
#define MAX_SON_MEAS_CONFIG_OBJECT  	16
    /* SPR 12929 Fix End */

/*! \def MAX_PLMN_ID_LIST2
 *  \brief A macro for maximum number of PLMN identity list
 */
#define MAX_PLMN_ID_LIST2    		5

/*! \def RRM_MAX_CELLS_REPORTED
 *  \brief A macro for maximum number of cells reported
 */
#define RRM_MAX_CELLS_REPORTED  	8

/*! \def MAX_PCI_RANGE
 *  \brief A macro that store the size PCI Range
 */
#define MAX_PCI_RANGE               	4
 
/*! \def MAX_LAC_SIZE
 *  \brief A macro that store the size of location area code
 which is a BIT STRING (SIZE (16))
 */
#define MAX_LAC_SIZE 			2
/*! \def MCC_OCTET_SIZE
 *  \brief A macro for maximum MCC octet size
*/
#define MCC_OCTET_SIZE   		3
/*! \def MAX_MNC_OCTET_SIZE
 *  \brief A macro for maximum MNC octet size
*/
#define MAX_MNC_OCTET_SIZE 		3

/*! \def TAC_OCTET_SIZE
 *  \brief A macro for maximum TAC octet size
*/
#define TAC_OCTET_SIZE 			2

/* SPR 7514 Fix Start - report config 924 */
/*! \enum rrm_interrat_trigger_type_periodical_purpose_et
 *  \brief An enum that specifies the purpose for Inter-RAT
 *         periodical UE measurements
 */
typedef enum
{
    RRM_INTERRAT_REPORT_STRONGEST_CELLS,
    RRM_INTERRAT_REPORT_STRONGEST_CELLS_FOR_SON,
    RRM_INTERRAT_REPORT_CGI
}rrm_interrat_trigger_type_periodical_purpose_et;
/* SPR 7514 Fix End - report config 924 */


/*! \enum rrm_error_et
 *  \brief An enum that specifies type allowed bandwidth
 */
typedef enum
{
    RRM_ALWD_BW_MBW_6,
    RRM_ALWD_BW_MBW_15,
    RRM_ALWD_BW_MBW_25,
    RRM_ALWD_BW_MBW_50,
    RRM_ALWD_BW_MBW_75,
    RRM_ALWD_BW_MBW_100
}rrm_allowed_meas_bandwidth_et;

/*! \enum rrm_error_et
 *  \brief An enum that specifies speed scale factors sf medium
 */
typedef enum {
    o_dot25 = 0,
    o_dot5 = 1,
    o_dot75 = 2,
    l_dot0 = 3
}rrm_speed_state_scale_factors_sf_medium_et;

/*! \enum rrm_error_et
 *  \brief An enum that specifies speed scale factors sf high
 */
typedef enum {
    o_dot25_1 = 0,
    o_dot5_1 = 1,
    o_dot75_1 = 2,
    l_dot0_1 = 3
}rrm_speed_state_scale_factors_sf_high_et;

/*! \enum rrm_son_cdma2000_type_et
 *  \brief An enum that specifies SON CDMA2000 type
 */
typedef enum
{
    R_TYPE_1X_RTT = 0,
    R_TYPE_HRPD
}rrm_son_cdma2000_type_et;

/*! \typedef rrm_q_offsetrange_et
 *  \brief  An enum that define Cell Specific Offset
 */
typedef enum
{
    RRM_Q_OFFSET_RANGE_DB_24,
    RRM_Q_OFFSET_RANGE_DB_22,
    RRM_Q_OFFSET_RANGE_DB_20,
    RRM_Q_OFFSET_RANGE_DB_18,
    RRM_Q_OFFSET_RANGE_DB_16,
    RRM_Q_OFFSET_RANGE_DB_14,
    RRM_Q_OFFSET_RANGE_DB_12,
    RRM_Q_OFFSET_RANGE_DB_10,
    RRM_Q_OFFSET_RANGE_DB_8,
    RRM_Q_OFFSET_RANGE_DB_6,
    RRM_Q_OFFSET_RANGE_DB_5,
    RRM_Q_OFFSET_RANGE_DB_4,
    RRM_Q_OFFSET_RANGE_DB_3,
    RRM_Q_OFFSET_RANGE_DB_2,
    RRM_Q_OFFSET_RANGE_DB_1,
    RRM_Q_OFFSET_RANGE_DB0,
    RRM_Q_OFFSET_RANGE_DB1,
    RRM_Q_OFFSET_RANGE_DB2,
    RRM_Q_OFFSET_RANGE_DB3,
    RRM_Q_OFFSET_RANGE_DB4,
    RRM_Q_OFFSET_RANGE_DB5,
    RRM_Q_OFFSET_RANGE_DB6,
    RRM_Q_OFFSET_RANGE_DB8,
    RRM_Q_OFFSET_RANGE_DB10,
    RRM_Q_OFFSET_RANGE_DB12,
    RRM_Q_OFFSET_RANGE_DB14,
    RRM_Q_OFFSET_RANGE_DB16,
    RRM_Q_OFFSET_RANGE_DB18,
    RRM_Q_OFFSET_RANGE_DB20,
    RRM_Q_OFFSET_RANGE_DB22,
    RRM_Q_OFFSET_RANGE_DB24
}rrm_q_offsetrange_et;

/* UE MEAS CHANGES : ENDS */

/* UE Positioning start */
#define ECID_CURRENT_SFN_OCTET_SIZE   2
/* UE Positioning end */
/** eICIC feature changes start */
/*! \def RRM_MAX_PATTERN_BYTE
 *  \brief A macro for ABS Pattern max size
*/
#define RRM_MAX_PATTERN_BYTE                              9
/** eICIC feature changes end */

#endif /*! _RRM_API_DEFINES_H_ */
