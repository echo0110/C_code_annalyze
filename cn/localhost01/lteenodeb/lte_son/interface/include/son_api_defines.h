/*! \file   son_api_defines.h
 *  \brief  This file contains Macro definitions, type-defines, Enums for 
 *          SON-API.
 *  \date   January 13, 2012
 *  \author gur25767
 */

/*!
 *  \ copyright {Copyright (c) 2009, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_API_DEFINES_H_ 
#define _SON_API_DEFINES_H_

#include "lteTypes.h"
/* Type defines */

typedef unsigned long long  son_u64;
typedef unsigned int        son_u32;
typedef unsigned short      son_u16;
typedef unsigned char       son_u8;
typedef unsigned long       son_ul32;
typedef signed int          son_s32;
typedef signed short        son_s16;
typedef signed long long    son_s64;
typedef signed char         son_s8;
typedef  char               son_8;
typedef float               son_float;
typedef son_u16             son_timer_duration_t;
typedef double              son_double;


/* SPR 20636 Changes Start */
#ifdef __x86_64__
#define son_u32_u64_Bit son_u64
#else
#define son_u32_u64_Bit son_u32
#endif
/* SPR 20636 Changes End */

/*
 * Macro definitions
 */

/*! \def SON_MAX_CELL_SUPPORTED_AT_ENB
 *  \brief Maximum number of Cells supported at eNodeB
 */
/*SPR 21702 Start*/
#define SON_MAX_CELL_SUPPORTED_AT_ENB         6    

/*! \def OAM_SON_MAX_CELL_IN_INFO_IND 
 *  \brief OAM can send maximun Cells info in SON_OAM_CELL_INFO_IND
 */
#define OAM_SON_MAX_CELL_IN_INFO_IND         1    
/*SPR 21702 End*/



/*! \def SON_MAX_NO_NEIGHBOUR_CELLS
 *  \brief Maximum number of neighbour Cells
 */
#define SON_MAX_NO_NEIGHBOUR_CELLS          256

/*! \def SON_MAX_NO_INTRA_NBR_CELLS
 *  \brief Maximum number of INTRA RAT neighbor cells
 */
#define SON_MAX_NO_INTRA_NBR_CELLS  32

/*! \def SON_MAX_NO_INTRA_RAT_NEIGHBOUR_CELLS
 *  \brief Maximum number of intra RAT neighbour Cells
 */
 /*CID 81807 START*/
#define SON_MAX_NO_INTRA_RAT_NEIGHBOUR_CELLS         32
 /* CID 81807 END*/

/*! \def SON_MAX_NO_INTER_RAT_NEIGHBOUR_CELLS
 *  \brief Maximum number of inter RAT neighbour Cells
 */
#define SON_MAX_NO_INTER_RAT_NEIGHBOUR_CELLS          128
/* CID-81785 START*/
#define SON_MAX_NO_UMTS_NEIGHBOUR_CELLS          32
/* CID-81785 END*/
/* CID-81786 START*/
#define SON_MAX_NO_GERAN_NEIGHBOUR_CELLS         32
/* CID-81786 END*/
/*! \def SON_MAX_NO_CELLS_PER_EARFCN
 *  \brief Maximum number of Cells PER EARFCN
 */
#define SON_MAX_NO_CELLS_PER_EARFCN          50
#define SON_MAX_NO_CDMA_XRTT_CELLS_PER_EARFCN   40

/*! \def SON_MAX_NO_EARFCN
 *  \brief A macro that store a integer value 100.
 */
#define SON_MAX_NO_EARFCN     100
/* SPR-12875 Fix Starts */
#define MAX_SON_MEAS_CONFIG_OBJECT  16
/* SPR-12875 Fix Ends */

/*! \def SON_MAX_NO_PCI
 *  \brief A macro that store a integer value 50.
 */
#define SON_MAX_NO_PCI        50

/*! \def SON_MAX_NUM_PLMN
 *  \brief A macro that store a integer value 6.
 */
#define SON_MAX_NUM_PLMN    6

/*! \def SON_NR_MAX_NUM_PLMN
 *  \brief A macro that store a integer value 5.
 */
#define SON_NR_MAX_NUM_PLMN    5

/*! \def SON_MCC_OCTET_SIZE
 *  \brief A macro that store a integer value 3.
 */
#define SON_MCC_OCTET_SIZE      3

/*! \def SON_MAX_MNC_OCTET_SIZE
 *  \brief A macro that store a integer value 3.
 */
#define SON_MAX_MNC_OCTET_SIZE         3

/*! \def SON_CELL_ID_OCTET_SIZE
 *  \brief A macro that store a integer value 4.
 */
#define SON_CELL_ID_OCTET_SIZE         4

/*! \def SON_CSG_ID_OCTET_SIZE
 *  \brief A macro that store a integer value 4.
 */
#define SON_CSG_ID_OCTET_SIZE          4

/*! \def SON_TAC_OCTET_SIZE
 *  \brief A macro that store a integer value 2.
 */
#define SON_TAC_OCTET_SIZE              2

/*! \def SON_RNC_ID_SIZE
 *  \brief A macro that store a integer value 4.
 */
#define SON_RNC_ID_SIZE                 4


/* REL1.1 CHANGES : STARTS */

/*! \def SON_MAX_PCI
 *  \brief A macro that store a integer value 504.
 */
#define SON_MAX_PCI                     504

/*! \def SON_MAX_PCI
 *  \brief A macro that store a integer value 504.
 */
#define SON_MAX_MEAS_BW                 6
/* REL1.1 CHANGES : ENDS */

/*! \def SON_LAC_OCTET_SIZE
 *  \brief A macro that store the size of location area code 
 *  which is a BIT STRING (SIZE (16))
 */
#define SON_LAC_OCTET_SIZE              2 

/*! \def SON_MAX_PCI_RANGE
 *  \brief A macro that store the size PCI Range
 */
#define SON_MAX_PCI_RANGE               4

/*! \def SON_MAX_INTRA_FREQ_NEIGHBORS_IN_SIB4
 *  \brief A macro that stores number of Intra Frequency Neighbors reported in SIB4
 */
#define SON_MAX_INTRA_FREQ_NEIGHBORS_IN_SIB4    16

/*! \def SON_MAX_CONFIG_OBJECT
 *  \brief A macro for maximum number of configuration objects
 */
/* SPR-12875 Fix Starts */
#define SON_MAX_CONFIG_OBJECT           16
/* SPR-12875 Fix Ends */

 /*! \def SON_MAX_CELLS_REPORTED
  *  \brief
  */
#define SON_MAX_CELLS_REPORTED  8

/*! \def SON_MAX_GERAN_EXP_ARFCN_LIST_SIZE
 *  \brief A macro that stores value for the max number of explicit arfcns
 */
#define SON_MAX_GERAN_EXP_ARFCN_LIST_SIZE   31

/*! \def SON_MAX_NUM_OF_OCTETS_IN_VAR_BITMAP
 *  \brief A macro that stores the value for max number of octets in geran freq bitmap
 */
#define SON_MAX_NUM_OF_OCTETS_IN_VAR_BITMAP 16

/*REL 1.2 CHANGES:START*/

/*
 * Macro definitions
 */

/*! \def TNL_DISCOVERY_TIMEOUT
 *  \brief A macro that stores the value for TNL Discovery timeout in seconds
 */
#define TNL_DISCOVERY_TIMEOUT    15

/* SPR 11340 Fix Starts */
/* Lines deleted */
/* SPR 11340 Fix Ends */

/*! \def SON_ENB_ID_OCTET_SIZE
 *  \brief Maximum number of octets in the eNB Id
 */
#define SON_ENB_ID_OCTET_SIZE         4

/*! \def SON_IPV4_SIZE
 *  \brief Maximum number of octets in IPV4 Address
 */

#define SON_IPV4_SIZE                       4

/*! \def SON_IPV6_SIZE
 *  \brief Maximum number of octets in IPV6 Address
 */
#define SON_IPV6_SIZE                       16
 
/*! \def SON_MAX_NO_ENB_X2_TLAS
 *  \brief Maximum number of TNL Addresses
 */
#define  SON_MAX_NO_ENB_X2_TLAS             2

/*! \def SON_MAX_NO_NEIGHBOUR_ENBS
*  \brief Maximum number of neighbor eNB supported
 */

 /*SPR-989-Start-Number of eNB's increased to 32 from 3*/
#define  SON_MAX_NO_NEIGHBOUR_ENBS          32
 /*SPR-989-End*/
/*REL 1.2 CHANGES STOP*/

#define SON_CELL_GLOBAL_ID_1X_RTT_SIZE    6
#define SON_CELL_GLOBAL_ID_HRPD_SIZE      16
#define SON_RRM_MAX_CDMA_FREQ_ALLOWED     16
#define SON_RRM_MAX_CDMA_BAND_ALLOWED     16
#define SON_MAX_NO_CDMA_HRPD_CELLS_PER_EARFCN   32
#define SON_MAX_NO_CDMA_XRTT_CELLS_PER_EARFCN   40

/*! \def SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED
*  \brief Maximum number of EARFCNs allowed
*/
#define SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED        9 

/*! \def SON_RRM_MAX_INTRA_RAT_BROADCAST_HO_ALLOWED_NBRS
*  \brief Maximum number of EARFCNs allowed
*/
#define SON_RRM_MAX_INTRA_RAT_BROADCAST_HO_ALLOWED_NBRS         16

/*! \def SON_RRM_MAX_INTRA_RAT_NON_BROADCAST_HO_ALLOWED_NBRS
*  \brief Maximum number of EARFCNs allowed
*/
#define SON_RRM_MAX_INTRA_RAT_NON_BROADCAST_HO_ALLOWED_NBRS     16

/*! \def SON_RRM_MAX_INTRA_RAT_BROADCAST_HO_BLKLSTED_NBRS
*  \brief Maximum number of EARFCNs allowed
*/
#define SON_RRM_MAX_INTRA_RAT_BROADCAST_HO_BLKLSTED_NBRS        16

/*! \def SON_RRM_MAX_INTRA_RAT_NON_BROADCAST_HO_BLKLSTED_NBRS
*  \brief Maximum number of EARFCNs allowed
*/
#define SON_RRM_MAX_INTRA_RAT_NON_BROADCAST_HO_BLKLSTED_NBRS    16

/*! \def SON_RRM_MAX_INTRA_RAT_HO_ALLWED_BLKLISTED_NBRS_PER_EARFCN
*  \brief Maximum number of ho allowed or ho blacklisted
 *        intra rat neighbors allowed
*/
#define SON_RRM_MAX_INTRA_RAT_HO_ALLWED_BLKLISTED_NBRS_PER_EARFCN 32

/*! \def SON_RRM_MAX_UTRAN_FREQ_ALLOWED
*  \brief Maximum number of EARFCNs allowed
*/
#define SON_RRM_MAX_UTRAN_FREQ_ALLOWED      32

/*! \def SON_RRM_MAX_UTRAN_FDD_FREQ_ALLOWED
*  \brief Maximum number of EARFCNs allowed
*/
#define SON_RRM_MAX_UTRAN_FDD_FREQ_ALLOWED      16

/*! \def SON_RRM_MAX_UTRAN_TDD_FREQ_ALLOWED
*  \brief Maximum number of EARFCNs allowed
*/
#define SON_RRM_MAX_UTRAN_TDD_FREQ_ALLOWED      16

/*! \def SON_RRM_MAX_GERAN_FREQ_ALLOWED
*  \brief Maximum number of EARFCNs allowed
*/
#define SON_RRM_MAX_GERAN_FREQ_ALLOWED      16

/*! \def SON_RRM_MAX_GERAN_UTRAN_FREQ_ALLOWED
*  \brief Maximum number of EARFCNs allowed
*/
#define SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN   128

/*! \def SON_MAX_ENB_POOLS
*  \brief Maximum number of eNB pools allowed
*/
#define SON_MAX_ENB_POOLS       16

/*! \def SON_MAX_MME_GRP_BYTES
*  \brief Maximum number of MME group bytes allowed
*/
#define SON_MAX_MME_GRP_BYTES 2

/*! \def SON_MAX_SERVED_CELLS_AT_NBR_ENB
*  \brief Maximum number of served cells allowed at neighbor eNB
*/
#define SON_MAX_SERVED_CELLS_AT_NBR_ENB 8

/*! \def SON_OAM_MAX_ASSOCIATED_DATA_LEN
*  \brief length of additional information in event notification
*/
#define SON_OAM_MAX_ASSOCIATED_DATA_LEN 20

/*! \def SON_STRONG_CELL_INFO_COUNT
 *  \brief 
 */
#define SON_STRONG_CELL_INFO_COUNT        3

/*! \def SON_MAX_NUMBER_OF_PRACH_SAMPLES
 *  \brief 
 */
#define SON_MAX_NUMBER_OF_PRACH_SAMPLES   1000



/*! \def SON_MIN_VALUE_TDD_EARFCN
 *  \brief 
 */
#define SON_MIN_VALUE_TDD_EARFCN   36000


/*! \def SON_MAX_VALUE_TDD_EARFCN
 *  \brief 
 */
#define SON_MAX_VALUE_TDD_EARFCN   45589


/*! \def SON_MAX_EARFCN_LIMIT
 *  \brief 
 */
#define SON_MAX_EARFCN_LIMIT   47535

/*! \def SON_DIFF_IN_UL_DL_EARFCN
 *  \brief 
 */
#define SON_DIFF_IN_UL_DL_EARFCN  18000

/*! \def SON_LEFT_NIBBLE_MASK
 *  \brief 
 */
#define SON_LEFT_NIBBLE_MASK  0xF0

/*! \def SON_RIGHT_NIBBLE_MASK
 *  \brief 
 */
#define SON_RIGHT_NIBBLE_MASK  0x0F

/*
 * Enumeration Definitions
 */

/*! \enum son_module_id_et
 *  \brief An enum that specifies the SON module IDs
 *  The value of internal module identifiers is taken such that their first
 *  byte contains SON Module Id (0x64 or 100)
 */
typedef enum 
{
    /* Internal Module IDs */
    SON_ALL_INT_MODULES = 0,
#ifndef OAM_IPR 
    SON_MODULE_ID = 100,
#endif    

    /* First byte of SON Internal Module's IDs contain the SON Module ID 
     * (25701 = 0x6465, i.e. 100 * 256 + 101) */
    SON_MIN_MODULE_ID = 25701,  
    SON_MIF_MODULE_ID  = SON_MIN_MODULE_ID, 
    SON_APPS_MODULE_ID,
    SON_ANR_MODULE_ID,
    SON_ES_MODULE_ID,
    SON_NMM_MODULE_ID,
    SON_MRO_MODULE_ID,
    SON_MLB_MODULE_ID,
    SON_RACH_OPT_MODULE_ID,
    SON_LOGGER_MODULE_ID,
    SON_X2_MODULE_ID,
    SON_MAX_MODULE_ID = SON_X2_MODULE_ID,

    /* External Module IDs */
    SON_OAM_MODULE_ID = 1,
    SON_RRM_MODULE_ID = 2,
    SON_RRC_MODULE_ID = 3,
    SON_PHY_MODULE_ID = 8   
}son_module_id_et;

/* If above enumeration is changed, the following module ID specific macro definitions need to be updated accordingly */

/*! \def SON_MAX_NO_INT_MODULES
 *  \brief Maximum no of SON Internal Modules from OAM perspective.
 */
#define SON_MAX_NO_INT_MODULES    (SON_MAX_MODULE_ID - SON_MIN_MODULE_ID - 1)

/*! \def SON_MAX_INTERNAL_MODULES
 *  \brief Actual number of SON internal CSPL Modules using interanl or advance logger
 */
/*Coverity 81011 Fix Start*/
#define SON_MAX_INTERNAL_MODULES  (SON_MAX_MODULE_ID - SON_MIN_MODULE_ID +1)
/*Coverity 81011 Fix End*/

/*! \def SON_MIN_INT_MODULE_ID
 *  \brief Minimum value of SON internal module Ids
 */
#define SON_MIN_INT_MODULE_ID     SON_MIN_MODULE_ID

/* Maximum value of internal module Id from OAM perspective */

/*! \def SON_MAX_INT_MODULE_ID
 *  \brief Maximum value of SON internal module Ids
 */
#define SON_MAX_INT_MODULE_ID     (SON_MAX_MODULE_ID - 2)

/*! \def SON_MIN_EXT_MODULE_ID
 *  \brief Minimum value of SON external module Ids
 */
#define SON_MIN_EXT_MODULE_ID     SON_OAM_MODULE_ID

/*! \def SON_MAX_EXT_MODULE_ID
 *  \brief Maximum value of SON external module Ids
 */
#define SON_MAX_EXT_MODULE_ID     SON_PHY_MODULE_ID

/*! \def MAX_TA_RANGE
 *  \brief Maximum possible TA range 
 */
#define MAX_TA_RANGE              12

/*! \enum son_return_et
 *  \brief An enum that specifies the return value for SON APIs
 */
typedef enum
{
    SON_FAILURE = 0,       /*!< If result is Failure */
    SON_SUCCESS,           /*!< If result is SUCCESS */
    SON_PARTIAL_SUCCESS    /*!< If result is Partially successful */
}son_return_et;

/*! \enum son_shutdown_type_et
 *  \brief Shutdown Type
 */
typedef enum
{
    SON_FORCED_SHUTDOWN = 0,    /*!< Forced shutdown for SON */
    SON_GRACEFUL_SHUTDOWN    /*!< Graceful Shutdown for SON */
}son_shutdown_type_et;

/*! \enum   son_oam_log_level_et
 *  \brief  Log levels received from OAM
 */
typedef enum
{
    SON_OAM_LOG_LEVEL_CRITICAL = LTE_LOG_CRITICAL, /*!< To set log level for Critical */
    SON_OAM_LOG_LEVEL_ERROR = LTE_LOG_ERROR, /*!< To set log level for Error */
    SON_OAM_LOG_LEVEL_WARNING = LTE_LOG_WARNING, /*!< To set log level for Warning */
    SON_OAM_LOG_LEVEL_INFO = LTE_LOG_INFO,   /*!< To set log level for Info description */
    SON_OAM_LOG_LEVEL_BRIEF = LTE_LOG_BRIEF,   /*!< To set log level for Brief description */
    SON_OAM_LOG_LEVEL_DETAILED = LTE_LOG_DETAILED, /*!< To set log level for Detailed description */
    SON_OAM_LOG_LEVEL_DETAILEDALL = LTE_LOG_DETAILEDALL/*!< To set log level for Detailed ALL description */
}son_oam_log_level_et;

/*! \enum   son_oam_log_on_off_et
 *  \brief  Log Enable/Disable 
 */
typedef enum
{
    SON_OAM_LOG_OFF = 0,     /*!<Log Disable */
    SON_OAM_LOG_ON        /*!<Log Enable */
}son_oam_log_on_off_et;

/*! \enum son_cell_switch_on_cause_et
 *  \brief An enum that specifies cell switch on cause
 */
typedef enum
{
    RECEPTION_OF_S1_MSG = 0, /*!<S1 msg is received */
    CAUSE_UNDEFINED = 255
}son_cell_switch_on_cause_et;

/* NR Status enumeration */

/*! \enum son_nr_status_et
 *  \brief An enum that specifies the SON NR status.
 *         Possible Values:
 *         --- LOCKED
 *         --- UNLOCKED
 *         --- UNDEFINED
 */
typedef enum
{
    SON_NR_LOCKED = 1,              /*!< NR is locked */
    SON_NR_UNLOCKED,                /*!< NR is unlocked */
    SON_NR_UNDEFINED = 255          
}son_nr_status_et;

/* HO Status enumeration */

/*! \enum son_ho_status_et
 *  \brief An enum that specifies the SON HO status.
 *         Possible Values:
 *         --- ALLOWED
 *         --- PROHIBITED
 *         --- UNDEFINED
 */
typedef enum
{
    SON_HO_ALLOWED = 1, /*!< HO is allowed */
    SON_HO_PROHIBITED,  /*!< HO is prohibited */
    SON_HO_UNDEFINED = 255 
}son_ho_status_et;

/* X2 Status enumeration */

/*! \enum son_x2_status_et
 *  \brief An enum that specifies the SON X2 status.
 *         Possible Values:
 *         --- ALLOWED
 *         --- PROHIBITED
 *         --- ALLOWED_CONNECTED
 *         --- ALLOWED_DISCONNECTED
 *         --- PROHIBITED_CONNECTED
 *         --- PROHIBITED_DISCONNECTED
 *         --- LOCALLY_CONNECTED
 *         --- UNDEFINED
 */
typedef enum
{
    SON_X2_ALLOWED = 1,                    /*!< X2 allowed */
    SON_X2_PROHIBITED,                     /*!< X2 prohibited */
    SON_X2_ALLOWED_CONNECTED,              /*!< X2 allowed and link is connected*/ 
    SON_X2_ALLOWED_DISCONNECTED,           /*!< X2 allowed and link is not connected*/
    SON_X2_PROHIBITED_CONNECTED,           /*!< X2 prohibited and link is connected*/
    SON_X2_PROHIBITED_DISCONNECTED,        /*!< X2 prohibited and link is not connected*/
    /* Rel 3.0 Multi Sector code changes Start */
    SON_LOCALLY_CONNECTED_CELL,             /*!< The neighbor cell and the serving cell belong to same eNB. Hence there is no X2 connection. */
    /* Rel 3.0 Multi Sector code changes Stop */
    /*SPR 15799 Fix Start*/
    /*This value is to be used only in case of Updated NRT info 
     *for ADD, to notify OAM that nbr's eNB is X2 BLACKLISTED
     *so that OAM doesn't initiate the eNB ADD.
     */
    SON_X2_BLACKLISTED,                    /*!< X2 link is Blacklisted*/
    /*SPR 15799 Fix Stop*/ 
    SON_X2_UNDEFINED = 255                 /*!< X2 unavailable */
}son_x2_status_et;

/*! \enum son_cell_switch_off_mode_et
 *  \brief An enum that specifies type of cell switch off requested
 */
typedef enum
{
    SO_GRACEFUL = 1,             /*!< For Graceful switch off */
    SO_FORCEFUL,                 /*!< For Forceful switch off */
    SO_UNDEF = 255
}son_cell_switch_off_mode_et;

/*! \enum son_threshold_hit_cause_et
 *  \brief An enum that specifies type of threshold level
 */
typedef enum
{
    REACHED_THRESHOLD = 0, /*!< If value is equal to or below threshold level*/
    ABOVE_THRESHOLD        /*!< If value is crossed above threshold level*/     
}son_threshold_hit_cause_et;

/* Rat Type enumeration */

/*! \enum son_rat_type_et
 *  \brief An enum that specifies the SON RAT type.
 *         Possible Values:
 *         --- EUTRAN
 *         --- UTRAN
 *         --- GERAN
 *         --- CELL UNDEFINED
 */
typedef enum
{
        SON_EUTRAN_CELL = 1,  /*!< Cell type is EUTRAN */
        SON_UTRAN_CELL,       /*!< Cell type is UTRAN */
        SON_GERAN_CELL,       /*!< Cell type is GERAN */
        SON_CDMA_CELL,        /*!< Cell type is CDMA */
        SON_CELL_UNDEFINED = 255
}son_rat_type_et;

/*! \enum son_error_et
 *  \brief An enum that specifies type of causes
 */

typedef enum
{
    SON_NO_ERROR = 0,               /*!< No Errors (In case of Success)*/

    SON_ERR_SYS_CPU_OVERLOAD = 1,   /*!< System Error: CPU OVERLOAD*/
    SON_ERR_SYS_MEMORY_OVERLOAD,    /*!< System Error: MEMORY OVERLOAD*/
    SON_ERR_SYS_MEM_ALLOC_FAILURE,  /*!< System Error: MEMORY ALLOCATION 
                                     *   FAILURE*/
    SON_ERR_SYS_IPC_FAILURE,        /*!< System Error: IPC FAILURE*/
    SON_ERR_SYS_SYSCALL_FAILURE,    /*!< System Error: SYSTEM CALL FAILURE*/

    SON_ERR_NMM_INTERNAL_FAILURE = 1000, /*!< NMM Error: NMM INTERNAL FAILURE*/
    SON_ERR_NMM_TIMER_EXPIRED,         /*!< NMM Error: NMM TIMER EXPIRED*/
    SON_ERR_NMM_STOP_MSG_RECVD,        /*!< NMM Error: NMM STOP MSG RECVD*/
    SON_ERR_NMM_MSG_SEND_FAILED,       /*!< NMM Error: NMM MSG SEND FAILED*/
    SON_ERR_NMM_INVALID_MSG_REC,       /*!< NMM Error: NMM INVALID MSG REC*/
    SON_ERR_NMM_PHY_NULL_DATA,         /*!< NMM Error: NMM PHY NULL DATA*/
    SON_ERR_NMM_FREQ_BAND_NOT_SUPPORTED_BY_HW,     /*!< NMM Error: NMM FREQ_BAND_NOT_SUPPORTED_BY_HW*/
    SON_ERR_NMM_INVALID_EARFCN,        /*!< NMM Error: NMM INVALID EARFCN*/
    SON_ERR_NMM_INVALID_NR_CARRIER_LIST, /*!< NMM Error: NMM INVALID_NR_CARRIER_LIST*/
    SON_ERR_NMM_INVALID_CARRIER_LIST,  /*!< NMM Error: NMM INVALID_CARRIER_LIST*/
    SON_ERR_NMM_STOP_SCAN_RUNNING,     /*!< NMM Error: STOP SCAN RUNNING */
    SON_ERR_NMM_START_SCAN_NOT_FOUND,  /*!< NMM Error: START SCAN NOT FOUND*/
    SON_ERR_NMM_SCAN_PREPAIR_FAILURE,  /*!< NMM Error: SCAN PREPAIR FAILURE*/
    SON_ERR_NMM_MAX_ALLOWED_SCAN_REACHED, /*!< NMM Error: MAX_ALLOWED_SCAN_REACHED*/
    SON_ERR_NMM_UNKNOWN_FAILURE,       /*!< NMM Error: NMM UNKNOWN FAILURE*/

    SON_ERR_ANR_NR_NOT_FOUND = 2000,   /*!< ANR Error: NR NOT FOUND*/
    SON_ERR_ANR_DUPLICATE_NR,          /*!< ANR Error: DUPLICATE NR */
    SON_ERR_ANR_NR_PRESENT_IN_REMOVE_LIST, /*!< ANR Error: NR PRESENT IN REMOVE LIST, NR CANNOT BE ADDED */
    SON_ERR_ANR_MAX_NRT_SIZE_REACHED,       /*!< ANR Error:MAX NRT SIZE REACHED */
    SON_ERR_ANR_MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED,  /*!< ANR Error:MAX_ALLOWED_FREQS_NBRS_TO_RRM_REACHED */
    SON_ERR_ANR_MODIFICATION_NOT_ALLOWED,  /*!< ANR Error:ANR MODIFICATION NOT ALLOWED */
    SON_ERR_ANR_MAX_ALLOWED_BANDS_NBRS_TO_RRM_REACHED,  /*!< ANR Error:MAX_ALLOWED_BANDS_NBRS_TO_RRM_REACHED */
    SON_ERR_ANR_MAX_ALLOWED_XRTT_NBRS_PER_ARFCN_TO_RRM_REACHED,  /*!< ANR Error:MAX_ALLOWED_XRTT_NBRS_PER_ARFCN_TO_RRM_REACHED */
    SON_ERR_ANR_MAX_ALLOWED_HRPD_NBRS_PER_ARFCN_TO_RRM_REACHED,  /*!< ANR Error:MAX_ALLOWED_HRPD_NBRS_PER_ARFCN_TO_RRM_REACHED */
    SON_ERR_ANR_RNC_ID_NOT_FOUND,   /*!< ANR Error:SON_ERR_ANR_RNC_ID_NOT_FOUND */
   
    SON_ERR_ES_CELL_SWITCHED_ON = 3000, /*!< ES Error: CELL SWITCHED ON*/
    SON_ERR_ES_CELL_SWITCHED_ON_IN_PROGRESS, /*!< ES Error: CELL SWITCHED ON IN PROGRESS*/
    SON_ERR_ES_CELL_SWITCHED_OFF,      /*!< ES Error: CELL SWITCHED OFF*/
    SON_ERR_ES_CELL_SWITCH_OFF_IN_PROGRESS, /*!< ES Error: CELL SWITCH OFF IN PROGRESS*/
/* ES 1.2 Changes Start */
    SON_ERR_ES_CELL_SWITCHON_FOR_X2_IN_PROGRESS, /*!< ES Error: Switch On request is already in progress from ANR */
    SON_ERR_ES_GENERIC_ERROR, /*!< ES Error: GENERIC ERROR */
/* ES 1.2 Changes Ends */
    
    SON_ERR_REGISTERATION_FAILURE = 4000, /*!< Generic Error: REGISTERATION 
                                           *   FAILURE*/
    SON_ERR_ALREADY_REGISTERED,
    SON_ERR_DEREGISTERATION_FAILURE,      /*!< Generic Error: DEREGISTERATION 
                                           *   FAILURE*/
    SON_ERR_ALREADY_DEREGISTERED,
    SON_ERR_ENABLE_IN_PROGRESS,           /*!< Generic Error: ENABLE IN 
                                           *   PROGRESS*/
    SON_ERR_DISABLE_IN_PROGRESS,          /*!< Generic Error: DISABLE IN
                                           *    PROGRESS*/
    SON_ERR_ALREADY_ENABLED,              /*!< Generic Error: ALREADY ENABLED*/
    SON_ERR_ALREADY_DISABLED,             /*!< Generic Error: ALREADY DISABLED*/
    SON_ERR_SHUTDOWN_IN_PROGRESS,     /*!< Generic Error: SCAN IN PROGRESS*/
    SON_ERR_CELL_UNCONFIGURED,        /*!< Generic Error: CELL UNOPERATIONAL*/
    SON_ERR_INVALID_PARAMS,           /*!< Generic Error: INVALID PARAMS*/
    SON_ERR_UNEXPECTED_MSG,           /*!< Generic Error: UNEXPECTED MSG*/
    SON_ERR_UNEXPECTED_MSG_IN_INIT_STATE, /*!< Generic Error: UNEXPECTED MSG
                                           *   IN INIT STATE*/
    SON_ERR_UNEXPECTED_MSG_IN_ACTIVE_STATE, /*!< Generic Error: UNEXPECTED MSG
                                             *   IN ACTIVE STATE*/
    SON_ERR_EMERGENCY_CALL_ONGOING,
    SON_ERR_RRM_FAILURE,
    SON_ERR_TIMER_EXPIRED,            /*!< Generic Error: TIMER EXPIRED*/
    SON_INIT_CONFIG_FAIL,              /*!< Generic Error: INIT CONFIG FAIL*/
/* REL 1.2 Changes Start */
    SON_ERR_NO_CELL_CONFIGURED,       /*!< Generic Error: No cell is Configured */
    /* REL 1.2 Changes Ends
     * REL1.1 CHANGES : STARTS */
    SON_ERR_INVALID_EARFCN,            /*!< Generic Error: INVALID EARFCN */
    SON_ERR_INVALID_MEAS_BW,           /*!< Generic Errpr: INVALID MEAS BW */
/* REL 2.0 Changes Start */
    SON_ERR_CELL_UNCONFIGURED_AT_ANR, /*!< Generic Error: Cell not configured at ANR due to which its dependent modules(MRO/MLB) funcitonality can't be performed */
    SON_ERR_ANR_MODE_IS_DISABLED,    /*!< Generic Error: ANR mode is disabled for cell due to which dependent modules(MRO/MLB) functionality can't be performed */ 
    SON_ERR_COMPOSER_ERROR,             /*!< Generic Error: Failure in sending a message by composer */
    SON_ERR_FAILED_TO_START_TIMER,      /*!< Generic Error: Failure in starting timer*/
    /* Rel 3.0 Multi Sector code changes Start */
    SON_ERR_LOCAL_NBR_CELL_NOT_ALLOWED_IN_X2_SETUP, /*!< Generic Error: X2 setup not possible with the cell belonging to same eNobeB. */
    /* Rel 3.0 Multi Sector code changes Stop */
/* REL 2.0 Changes Ends */
    /* SPR-13251 Fix Starts */
    SON_ERR_LOGGING_DISABLED,       /*!< Generic Error: Logging is disabled */
    SON_ERR_REQ_ALREADY_IN_PROGRESS, /*!< Generic Error: The request is already in progress */ 
    /* SPR-13251 Fix Ends */
    SON_ERR_UNUSED_PCI_NOT_FOUND = 5000,      /*!< PCI Error: UNUSED PCI NOT FOUND*/
    SON_ERR_PCI_DISABLED,              /*!< PCI Error: PCI IN DISABLE STATE*/
    SON_ERR_PCI_SELECTION_IN_PROGRESS, /*!< PCI Error: PCI SELECTION is in Progress STATE*/
    SON_ERR_EMPTY_PCI_LIST,            /*!< PCI Error: PCI list received from OAM is empty */
    SON_ERR_ANR_IS_DISABLED,           /*!< PCI Error: ANR is disabled */
    /* Spr 17753 Changes Start */
    SON_ERR_PCI_RESELECTION_NOT_REQUIRED, /*!< PCI Error : pci reselection is not requited */ 
    /* Spr 17753 Changes End */
    SON_ERR_ACF_INTERNAL_FAILURE = 6000,      /*!< ACF Error: INTERNAL ERROR */
    SON_ERR_ACF_NO_NEIGHBOR_FOUND,      /*!< ACF Error: NO NEIGHBOUR FOUND */
    SON_ERR_ACF_SELECTION_IN_PROGRESS, /*!< ACF Error: ACF SELECTION IN PROGRESS */
    SON_ERR_EMPTY_CARRIER_FREQ_LIST,   /*!< ACF Error: CARRIER FREQ LIST EMPTY */
/* REL1.1 CHANGES : ENDS */

/* REL 1.2 Changes Start */
    SON_ERR_TNL_DISCOVERY_IN_PROGRESS = 7000, /*!< Generic Error: TNL Discovery
                                               * in Progress */
    SON_ERR_X2_CONNECTION_IN_PROGRESS,  /*!< Generic Error: X2 Setup in progresss
 */
    SON_ERR_X2_DISCONNECTION_IN_PROGRESS, /*!< Generic Error: X2 Termination in pp
                                           * rogress */
    SON_ERR_TNL_DISCOVERY_FAILURE, /*!< Generic Error: TNL Discovery Failure */
    SON_ERR_X2_CONNECT_FAILURE, /*!< Generic Error: X2 Setup Failure */
    SON_ERR_X2_DISCONNECT_FAILURE, /*!< Generic Error: X2 Disconnect Failure*/
    SON_ERR_X2_DISABLED,          /*!< Generic Error: X2 is Disabled*/
    /*SPR_17670_START*/
    SON_ERR_NO_ENB_REQUIRES_X2_SETUP, /*!< Generic Error: No X2 set up required */
/* REL 1.2 Changes Ends */
    /* SPR_19619 start */
    SON_ERR_DUPLICATE_IP_IN_LINK_UP, /*!< Generic Error: Duplicate IP recieved in LINK UP*/
    /*SPR_17670_END*/
    SON_ERR_CELL_ACTIVATION_FAILED /*!< Generic Error:Cell Activation Failed */
    /* SPR_19619 stop */
}son_error_et;

/*! \enum son_error_et
 *  \brief An enum that specifies the Timer type
 */
typedef enum
{
    SON_INIT_IND_TIMER = 0, /*!<Timer is for SON_OAM_INIT_IND to be sent to OAM*/
    SON_SHUTDOWN_TIMER     /*!<Timer is for receiving Shutdown responses from 
                            *   SON internal modules */
}son_timer_id_et;

/*! \  son_q_offset_range_et
 *  \brief  An enum that define Cell Specific Offset
 *
 */
typedef enum
{
  SON_Q_OFFSET_RANGE_DB_24,
  SON_Q_OFFSET_RANGE_DB_22,
  SON_Q_OFFSET_RANGE_DB_20,
  SON_Q_OFFSET_RANGE_DB_18,
  SON_Q_OFFSET_RANGE_DB_16,
  SON_Q_OFFSET_RANGE_DB_14,
  SON_Q_OFFSET_RANGE_DB_12,
  SON_Q_OFFSET_RANGE_DB_10,
  SON_Q_OFFSET_RANGE_DB_8,
  SON_Q_OFFSET_RANGE_DB_6,
  SON_Q_OFFSET_RANGE_DB_5,
  SON_Q_OFFSET_RANGE_DB_4,
  SON_Q_OFFSET_RANGE_DB_3,
  SON_Q_OFFSET_RANGE_DB_2,
  SON_Q_OFFSET_RANGE_DB_1,
  SON_Q_OFFSET_RANGE_DB0,
  SON_Q_OFFSET_RANGE_DB1,
  SON_Q_OFFSET_RANGE_DB2,
  SON_Q_OFFSET_RANGE_DB3,
  SON_Q_OFFSET_RANGE_DB4,
  SON_Q_OFFSET_RANGE_DB5,
  SON_Q_OFFSET_RANGE_DB6,
  SON_Q_OFFSET_RANGE_DB8,
  SON_Q_OFFSET_RANGE_DB10,
  SON_Q_OFFSET_RANGE_DB12,
  SON_Q_OFFSET_RANGE_DB14,
  SON_Q_OFFSET_RANGE_DB16,
  SON_Q_OFFSET_RANGE_DB18,
  SON_Q_OFFSET_RANGE_DB20,
  SON_Q_OFFSET_RANGE_DB22,
  SON_Q_OFFSET_RANGE_DB24
}son_q_offset_range_et;

/*SPR 20523 START*/
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB_24 -24
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB_22 -22
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB_20 -20
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB_18 -18
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB_16 -16
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB_14 -14
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB_12 -12
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB_10 -10
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB_8   -8
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB_6   -6
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB_5   -5
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB_4   -4
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB_3   -3
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB_2   -2
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB_1   -1
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB0     0
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB1     1
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB2     2
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB3     3
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB4     4
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB5     5
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB6     6
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB8     8
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB10   10
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB12   12
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB14   14
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB16   16
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB18   18
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB20   20
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB22   22
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB24   24
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB30   30
#define SON_Q_OFFSET_ABSOLUTE_VAL_DB31   31
/*SPR 20523 END*/

/*! \  enum son_nrt_operation_performed_et
 *  \brief This enum defines the operation performed on NRT
 */
typedef enum
{
    SON_NRT_OP_ADD = 1,  /*!< Add operation performed */
    SON_NRT_OP_UPDATE,   /*!< update operation performed */
    /*+SPR 18853 FIX*/
    SON_NRT_OP_DELETE,   /*!< delete operation performed */
    SON_NRT_NMM_OP_ADD   /* Add operation performed after NR scan using NMM*/
    /*-SPR 18853 FIX*/
}son_nrt_operation_performed_et;

/*! \enum son_allowed_meas_bandwidth_et
 *  \brief An enum that specifes allowed meas bandwidth values.
 */
typedef enum
{
    SON_ALWD_BW_MBW_6,   /*!< bandwidth of 6 RBs */
    SON_ALWD_BW_MBW_15,  /*!< bandwidth of 15 RBs*/
    SON_ALWD_BW_MBW_25,   /*!<bandwidth of 25 RBs*/
    SON_ALWD_BW_MBW_50,  /*!< bandwidth of 50 RBs*/
    SON_ALWD_BW_MBW_75,  /*!< bandwidth of 75 RBs*/
    SON_ALWD_BW_MBW_100  /*!< bandwidth of 100 RBs */
}son_allowed_meas_bandwidth_et;

/*! \enum son_speed_state_scale_factors_sf_medium_et
 *  \brief An enum that specifes speed state scale factors
 */
typedef enum {
    son_o_dot25 = 0, /*!< son_o_dot25 */
    son_o_dot5 = 1,  /*!< son_o_dot5 */
    son_o_dot75 = 2, /*!< son_o_dot75 */
    son_l_dot0 = 3   /*!< son_l_dot0 */
}son_speed_state_scale_factors_sf_medium_et;

/*! \enum son_speed_state_scale_factors_sf_high_et
 *  \brief An enum that specifes speed state scale factors
 */
typedef enum {
    son_o_dot25_1 = 0,   /*!< son_o_dot25_1 */
    son_o_dot5_1 = 1,    /*!< son_o_dot5_1 */
    son_o_dot75_1 = 2,   /*!< son_o_dot75_1 */
    son_l_dot0_1 = 3     /*!< son_l_dot0_1 */
}son_speed_state_scale_factors_sf_high_et;

/*! \ son_timer_buf_t
 *  \brief   Structure storing the Timer type
 *  \param   timer_type - Specifies whether timer is for Init Indication msg
 *           to be sent to OAM or Shutdown msg to be received from SON internal
 *           modules
 */
typedef struct _son_timer_buf_t
{
    son_timer_id_et    timer_type;
}son_timer_buf_t;

/*! \enum son_pci_mode_et
 *  \brief An enum that specifies mode of PCI request
 */
typedef enum
{
    SON_PCI_DISABLE = 0,  /*!< Disable PCI feature */
    SON_PCI_ENABLE        /*!< Enable PCI feature */
}son_pci_mode_et;

/*! \enum son_pci_cause_et
 *  \brief An enum that specifies PCI Conflict or Confusion
 */
typedef enum
{
    SON_PCI_CONFLICT = 0,  /*!< PCI Conflict */
    SON_PCI_CONFUSION,     /*!< PCI Confusion */
    /*SPR-12596-Start*/
    SON_PCI_CONFLICT_CLEARED,  /*!< PCI Conflict Cleared*/
    SON_PCI_CONFUSION_CLEARED, /*!< PCI Confusion Cleared*/
    /*SPR-12596-End*/
    SON_PCI_UNDEFINED = 255
}son_pci_cause_et;

/*! \enum son_feature_id_et
 *  \brief An enum that specifies the features of SON internal modules
 */
typedef enum
{
    SON_ALL_FEATURE = 0,  /*!< 1. In case of global Enable/Disable logging of all
                           * modules and feature 2. In case of individual APPS module, log enable/disable
                           * request is applicable to all its  internal features (PCI and ACF) 3. All other
                           * modules except  APPS will ignore this field */
    SON_PCI_FEATURE,           /*!< PCI feature identifier */
    SON_ACF_FEATURE,           /*!< ACF feature identifier */
    SON_NW_SCAN_FEATURE        /*!< ACF feature identifier */
}son_feature_id_et;

/*! \enum son_geran_band_indicator_et
 *  \brief An enum that specifies bands for GERAN carrier ARFCNs
 */
typedef enum 
{
  SON_GERAN_DCS_1800,              /*!< geran band dcs_1800*/
  SON_GERAN_PCS_1900               /*!< geran band dcs 1900*/
}son_geran_band_indicator_et;

/*REL 1.2 CHANGES:START*/

/*! \enum son_enb_type_et
 *  \brief An enum that specifies the type of eNB
 */

typedef enum
{
  SON_HOME_ENB = 1,        /*!< Home ENB Id */ 
  SON_MACRO_ENB,           /*!< macro eNB Id */
  SON_ENB_TYPE_UNDEFINED = 255    
}son_enb_type_et;

/*! \enum   son_x2_connection_status_et
 *  \brief  An enum that specifies the SON X2 Connection status
 */     
typedef enum
{
    SON_X2_CONNECTING = 1,             /*!< X2 Connection Setup is in progress*/
  SON_X2_CONNECTED,                 /*!< X2 Connection is established */
  SON_X2_CONNECTED_WHITELISTED,     /*!< X2 Connection is established and whitelisted */
  SON_X2_DISCONNECTING,             /*!< X2 Connection release is in progress*/
  SON_X2_DISCONNECTED,              /*!< X2 Connection is released */
  SON_X2_DISCONNECTED_BLACKLISTED,  /*!< X2 Connection is released and blacklisted */
  SON_X2_CONNECT,                   /*!< Request establishment of X2 Connection */
  SON_X2_CONNECT_WHITELIST,         /*!< Request establishment of X2 Connection */
  SON_X2_DISCONNECT,                /*!< Request establishment of X2 Connection and whitelist */
  SON_X2_DISCONNECT_BLACKLIST,      /*!< Request release  of X2 Connection and blacklist */
  SON_X2_UNDEF    = 255
}son_x2_connection_status_et;

/*! \enum son_bool_et
 *  \brief An enum is taken for False or True value
 */
typedef enum
{
    SON_FALSE,                 /*!< False*/
    SON_TRUE                   /*!< True*/
}son_bool_et;

/*! \enum  son_switch_on_off_cause_et
 *  \brief An enum that specifies the reason for switching on or off the cell
 */
typedef enum 
{
    SON_SWTICH_ON_FROM_PEER_ENB,
    SON_SWITCH_ON_DUE_TO_X2_SET_UP,
    SON_SWITCH_OFF_DUE_TO_LOW_LOAD_COND,
    /* AUTONOMOUS SWITCH OFF START */
    SON_SWITCH_OFF_DUE_TO_LOW_UE_CNT_COND,
    /* Fix 875 Start */
    SON_SWITCH_OFF_DUE_TO_LOW_LOAD_AND_LOW_UE_CNT_COND,
    /* Fix 875 End */
    SON_SWITCH_ON_OFF_CAUSE_UNDEFINED = 255
                                        /* AUTONOMOUS SWITCH OFF END */
}son_switch_on_off_cause_et;

/*REL 1.2 CHANGES:END*/
typedef enum {
    BC0 = 0,
    BC1,
    BC2,
    BC3,
    BC4,
    BC5,
    BC6,
    BC7,
    BC8,
    BC9,
    BC10 = 10,
    BC11,
    BC12,
    BC13,
    BC14,
    BC15,
    BC16,
    BC17,
    BC18_V9A0,
    BC19_V9A0,
    BC20_V9A0 = 20,
    BC21_V9A0
}son_cdma_band_class_et;

/*! \enum  son_access_mode_et
 *  \brief An enum that indicates ACCESS mode of the cell.
 */
typedef enum
{
    SON_CLOSED,
    SON_HYBRID,
    SON_OPEN,
    SON_OPEN_FEMTO  /*Enum value added for SPR-9574*/
}son_access_mode_et;

/*SPR-9574-Start*/

/*! \enum  son_oam_cell_id_range_et
 *  \brief An enum that indicates pci range.
 */
typedef enum
{
    SON_OAM_N4,
    SON_OAM_N8,
    SON_OAM_N12,
    SON_OAM_N16,
    SON_OAM_N24,
    SON_OAM_N32,
    SON_OAM_N48,
    SON_OAM_N64,
    SON_OAM_N84,
    SON_OAM_N96,
    SON_OAM_N128,
    SON_OAM_N168,
    SON_OAM_N252,
    SON_OAM_N504,
    SON_OAM_SPARE2,
    SON_OAM_SPARE1
}son_oam_cell_id_range_et;


/*SPR-9574-End*/

/*! \enum  son_disable_cause_et
 *  \brief An enum that indicates disable cause of the cell.
 */
typedef enum
{
    SON_ANR_DISABLED,
    SON_CELL_UNOPERATIONAL
}son_disable_cause_et;

/*! \enum  son_feature_state_et
 *  \brief An enum that specifies the SON feature state
 */
typedef enum
{
    SON_STATE_ENABLED = 0,
    SON_STATE_DISABLED,
    SON_STATE_DELETED,
    SON_STATE_UNDEFINED = 255
}son_feature_state_et;

/*! \enum  son_feature_state_change_cause_et
 *  \brief An enum that specifies the cause for SON feature state change
 */
typedef enum
{
    SON_RRM_REGISTRATION_SUCCESSFUL = 0,
    SON_RRM_REGISTRATION_UNSUCCESSFUL,
    SON_RRM_CELL_UNOPERATIONAL,
    SON_RRM_CELL_UNCONFIGURED,
    SON_ANR_CELL_UNCONFIGURED,
    SON_ANR_MODE_DISABLED,
    SON_MEM_ALLOC_FAILURE,
    SON_MAX_SUPPORTED_CELL_LIMIT_REACHED,
    SON_CELL_ALREADY_EXISTS,
    SON_INTERNAL_FAILURE,
    SON_MRO_MODE_DISABLED,
    SON_MLB_MODE_DISABLED,
    SON_ES_MODE_DISABLED,
    /* PR 551 Fix Start */
    SON_RRM_CELL_OPERATIONAL,
    /* PR 551 Fix End */
    SON_RACH_OPT_MODE_DISABLED,
    SON_STATE_CHANGE_CAUSE_UNDEFINED = 255
}son_feature_state_change_cause_et;

/*! \enum  son_anr_cell_mode_et
 *   \brief An enum that indicates anr mode for the cell.
 */
typedef enum
{
    ANR_MODE_DISABLED
}son_anr_cell_mode_et;

typedef enum
{
    TYPE_1X_RTT = 0,
    TYPE_HRPD
}son_cdma2000_type_et;

/* MRO Changes Start */

/*! \enum son_mro_param_et
 *   \brief An enum that stores the possible parameters that can be optimized by MRO module.
 */
typedef enum
{
    SON_MRO_CIO = 0,
    SON_MRO_TTT,
    SON_MRO_CIO_TTT
}son_mro_param_et;

/*! \enum son_time_to_trigger_range_et
 *   \brief This enum stores the value for the information element
 * TimeToTrigger.
 */
typedef enum
{
    SON_TTT_MS_0 = 0,
    SON_TTT_MS_40,
    SON_TTT_MS_64,
    SON_TTT_MS_80,
    SON_TTT_MS_100,
    SON_TTT_MS_128,
    SON_TTT_MS_160,
    SON_TTT_MS_256,
    SON_TTT_MS_320,
    SON_TTT_MS_480,
    SON_TTT_MS_512,
    SON_TTT_MS_640,
    SON_TTT_MS_1024,
    SON_TTT_MS_1280,
    SON_TTT_MS_2560,
    SON_TTT_MS_5120
}son_time_to_trigger_range_et;

/*! \enum son_mode_et
 *  \brief This enum specifies the SON running mode
 */

typedef enum
{
    SON_MODE_DISTRIBUTED = 0,
    SON_MODE_CENTRALIZED
}son_mode_et;        

/*! \enum son_mro_notification_cause_et
 *  \brief An enum that specifies the SON MRO Notification cause.
 */
typedef enum
{
    SON_MRO_OPTIMIZATION_NO_GAIN = 1,
    SON_MRO_OPTIMIZATION_LIMITED_GAIN,
    SON_MRO_CIO_MAX_LIMIT_REACHED,
    SON_MRO_CIO_MIN_LIMIT_REACHED,
    SON_MRO_TTT_MAX_LIMIT_REACHED,
    SON_MRO_TTT_MIN_LIMIT_REACHED,
    SON_MRO_NOTIFICATION_CAUSE_UNDEFINED = 0xFF
}son_mro_notification_cause_et;

/*! \enum son_mro_ho_failure_level_et
 *  \brief An enum that specifies the SON MRO HO Failure Level
 */
typedef enum
{
    SON_MRO_ABOVE_START_THRESHOLD = 1,
    SON_MRO_ABOVE_STOP_THRESHOLD
}son_mro_ho_failure_level_et;

/* MRO Changes End */

/*! \enum son_nbr_level_et
 *  \brief This enum specifies the level of the neighbors to be searched for the PCI Reselection
 */
typedef enum
{
    SON_LEVEL_1 = 0,
    SON_LEVEL_2,
    SON_LEVEL_UNDEFINED = 0xFF
}son_nbr_level_et;

/*! \enum son_preamble_init_rec_target_pow_et
 *  \brief This enum specifies the initial preamble power. Value in dBm. Value SON_OAM_PREAMBLE_POWER_DBM_120 corresponds to -120 dBm, SON_OAM_PREAMBLE_POWER_DBM_118 corresponds to -118 dBm and so on.
 */
typedef enum
{
    SON_OAM_PREAMBLE_POWER_DBM_120 = 0,
    SON_OAM_PREAMBLE_POWER_DBM_118,
    SON_OAM_PREAMBLE_POWER_DBM_116,
    SON_OAM_PREAMBLE_POWER_DBM_114,
    SON_OAM_PREAMBLE_POWER_DBM_112,
    SON_OAM_PREAMBLE_POWER_DBM_110,
    SON_OAM_PREAMBLE_POWER_DBM_108,
    SON_OAM_PREAMBLE_POWER_DBM_106,
    SON_OAM_PREAMBLE_POWER_DBM_104,
    SON_OAM_PREAMBLE_POWER_DBM_102,
    SON_OAM_PREAMBLE_POWER_DBM_100,
    SON_OAM_PREAMBLE_POWER_DBM_98,
    SON_OAM_PREAMBLE_POWER_DBM_96,
    SON_OAM_PREAMBLE_POWER_DBM_94,
    SON_OAM_PREAMBLE_POWER_DBM_92,
    SON_OAM_PREAMBLE_POWER_DBM_90,
    /* SPR 15748 Fixed Start */
    SON_OAM_PREAMBLE_POWER_UNDEFINED
    /* SPR 15748 Fixed End */
}son_preamble_init_rec_target_pow_et;

/*! \enum son_oam_ra_preamble_count_et
 *  \brief This enum specifies the Number of non-dedicated random access preambles. Value is an integer. Value SON_OAM_RA_PREAMBLE_COUNT_N4 corresponds to 4, SON_OAM_RA_PREAMBLE_COUNT_N8 corresponds to 8 and so on.
 */
typedef enum
{
    SON_OAM_RA_PREAMBLE_COUNT_N4 = 0,
    SON_OAM_RA_PREAMBLE_COUNT_N8,
    SON_OAM_RA_PREAMBLE_COUNT_N12,
    SON_OAM_RA_PREAMBLE_COUNT_N16,
    SON_OAM_RA_PREAMBLE_COUNT_N20,
    SON_OAM_RA_PREAMBLE_COUNT_N24,
    SON_OAM_RA_PREAMBLE_COUNT_N28,
    SON_OAM_RA_PREAMBLE_COUNT_N32,
    SON_OAM_RA_PREAMBLE_COUNT_N36,
    SON_OAM_RA_PREAMBLE_COUNT_N40,
    SON_OAM_RA_PREAMBLE_COUNT_N44,
    SON_OAM_RA_PREAMBLE_COUNT_N48,
    SON_OAM_RA_PREAMBLE_COUNT_N52,
    SON_OAM_RA_PREAMBLE_COUNT_N56,
    SON_OAM_RA_PREAMBLE_COUNT_N60,
    SON_OAM_RA_PREAMBLE_COUNT_N64,
  /* SPR 15748 Fixed Start */
    SON_OAM_RA_PREAMBLE_COUNT_UNDEFINED
  /* SPR 15748 Fixed End */ 
}son_oam_ra_preamble_count_et;

/*! \enum son_oam_ra_preambles_group_a_size_et
 *  \brief This enum specifies the size of the random access preambles group A. Value is an integer. Value SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N4 corresponds to 4, SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N8 corresponds to 8 and so on.
 */
typedef enum
{
  SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N4 = 0,
  SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N8,
  SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N12,
  SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N16, 
  SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N20,
  SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N24,
  SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N28,
  SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N32,
  SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N36,
  SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N40,
  SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N44,
  SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N48,
  SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N52,
  SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N56,
  SON_OAM_RA_PREAMBLE_GROUP_A_SIZE_N60,
  /* SPR 15748 Fixed Start */
  SON_OAM_RA_PREAMBLE_GROUP_A_UNDEFINED
  /* SPR 15748 Fixed End */

}son_oam_ra_preambles_group_a_size_et;

/*! \enum son_rach_opt_identified_optimization_type_et
 *  \brief An enum that specifies what type of RACH optimization was being tried by the SON RACH-OPT when RACH Algo was stopped abnormally.
 */
typedef enum
{
    SON_RACH_OPT_INITIAL_POWER_OPTIMIZATION = 0,
    SON_RACH_OPT_INTERFERENCE_OPTIMIZATION,
    SON_RACH_OPT_CB_OPTIMIZATION,
    SON_RACH_OPT_TYPE_UNDEFINED = 0xFF
}son_rach_opt_identified_optimization_type_et;

/*! \enum son_rach_opt_failure_cause_et
 *  \brief An enum that specifies the SON RACH OPT Notification cause.
 */
typedef enum
{
    SON_RACH_OPT_RACH_DEGRADED = 1,
    SON_RACH_OPT_MAX_ITERATION_RCHD,
    SON_RACH_OPT_COUNTERS_INSUFFICIENT_TO_START_POWER_ALGO,
    SON_RACH_OPT_COUNTERS_INSUFFICIENT_TO_START_INTERFERENCE_ALGO,
    SON_RACH_OPT_INSUFFICIENT_UNUSED_CFRA_COUNTERS,
    SON_RACH_OPT_COUNTERS_INSUFFICIENT_TO_START_GROUP_A_B_SPLIT_ALGO,
    SON_RACH_OPT_RA_PREAMBLE_GROUP_A_MAX_LIMIT_REACHED,
    SON_RACH_OPT_RA_PREAMBLE_GROUP_A_MIN_LIMIT_REACHED,
    SON_RACH_OPT_RA_PREAMBLE_MAX_LIMIT_REACHED,
    SON_RACH_OPT_RA_PREAMBLE_MIN_LIMIT_REACHED,
    SON_RACH_OPT_TOTAL_CFRA_PREAMBLE_CALCULATED_ZERO,
    SON_RACH_OPT_NUMBER_OF_CFRA_PREAMBLES_MIN_THRESHOLD_REACHED,
    SON_RACH_OPT_INTERFERENCE_OPTIMIZATION_FAILURE,
    SON_RACH_OPT_INITIAL_POWER_LIMIT_REACHED,
    SON_RACH_OPT_FAILURE_CAUSE_UNDEFINED = 0xFF
}son_rach_opt_failure_cause_et;

/*! \enum son_rach_opt_failure_level_et
 *  \brief An enum that specifies the SON RACH OPT Failure Level when RACH Algo was stopped abnormally.
 */
typedef enum
{
    SON_RACH_OPT_ABOVE_START_THRESHOLD = 0,
    SON_RACH_OPT_ABOVE_STOP_THRESHOLD
}son_rach_opt_failure_level_et;

typedef enum
{
    SON_CELL_CENTRE = 0,
    SON_CELL_EDGE
}son_ue_location_et;

typedef enum{
    SON_RRC_CONNECTED_UE = 1,
    SON_RRC_RE_ESTABLISHMENT_UE,
    SON_IN_SYNC_IND_UE
}son_prach_cause_et;

/* eICIC_PHASE_1_2_CHANGES_START */
/*! \enum son_intf_scheme_type_et
 *  \brief An enum that specifies the scheme type used for the cell.
 */
typedef enum
{
    SON_ACTIVATE_ICIC = 0,
    SON_ACTIVATE_EICIC,
    SON_NO_INTF_SCHEME
}son_intf_scheme_type_et;
/* eICIC_PHASE_1_2_CHANGES_END */
/* SPR 22248 MBMS Changes Start */
#define SON_SUBFRAME_ALLOC_ONE_FRAME_OCTET_SIZE   1
#define SON_SUBFRAME_ALLOC_FOUR_FRAME_OCTET_SIZE  3
#define MAX_NUM_MBSFN                             8
#define SON_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT 0x01
#define SON_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT 0x02
typedef enum
{
    SON_RADIO_ALLOC_FRAME_1 = 0,
    SON_RADIO_ALLOC_FRAME_2,
    SON_RADIO_ALLOC_FRAME_4,
    SON_RADIO_ALLOC_FRAME_8,
    SON_RADIO_ALLOC_FRAME_16,
    SON_RADIO_ALLOC_FRAME_32
}son_radioframe_allocation_period_et;

#define SON_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT 0x01
#define SON_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT 0x02
typedef struct _son_subframe_allocation_t
{
    son_u32 bitmask;  
    /*^ BITMASK ^*/
    son_u8  oneframe[SON_SUBFRAME_ALLOC_ONE_FRAME_OCTET_SIZE];
    /*^O,SON_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT,OCTET_STRING,FIXED^*/ 
    son_u8  fourframe[SON_SUBFRAME_ALLOC_FOUR_FRAME_OCTET_SIZE];
    /*^O,SON_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT,OCTET_STRING,FIXED^*/ 
}son_subframe_allocation_t;

typedef struct _son_mbsfn_info_t
{
    son_u8 radioframe_alloc_period; /* son_radioframe_allocation_period_et */
    /*^ M, 0, H, 0, 5 ^*/  
    son_u8 radioframe_alloc_offset;
    /*^ M, 0, H, 0, 7 ^*/  
    son_subframe_allocation_t subframe_alloc;
    /*^ M, 0, N, 0, 0 ^*/
}son_mbsfn_info_t;

typedef struct _son_mbms_configuration_t
{
    son_u8  num_mbsfn;
    /*^ M, 0, B, 1, 8 ^*/
    son_mbsfn_info_t mbsfn_info[MAX_NUM_MBSFN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_mbms_configuration_t;
/* SPR 22248 MBMS Changes Stop */
#endif /* _SON_API_DEFINES_H_ */
