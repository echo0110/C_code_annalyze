/*! \file son_defines.h
 *  \brief This file contains the enums, struct, constants definitions
 * common to all modules of SON binary
 *  \date January 20, 2012
 *  \author Shilpi Jain/Gautam Shah
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved
 */

#ifndef _SON_DEFINES_H_
#define _SON_DEFINES_H_

/*! \headerfile stacklayer.h <>
 */
#include <stacklayer.h>

/*! \headerfile son_api_defines.h <>
 */
#include <son_api_defines.h>

/*! \  void son_void_t */
typedef void son_void_t;
/*! \  size_t son_size_t */
typedef size_t son_size_t;
/*! \  QTIMER son_timer_t */
typedef QTIMER son_timer_t;
/*! \  QTIME son_time_t */
typedef QTIME son_time_t;
/*! \  U16 son_module_id_t */
typedef U16 son_module_id_t;
/*! \  U16 son_length_t */
typedef U16 son_length_t;
/*! \  U16 son_counter_t */
typedef U16 son_counter_t;
/*! \  U16 son_tag_t */
typedef U16 son_tag_t;
/*! \  U16 son_transaction_id_t */
typedef U16         son_transaction_id_t;
/*! \  U8 son_cell_index_t */
typedef U8 son_cell_index_t;

/*! \def SON_MAX_LOG_PATH_STR_LEN
 *  \brief Maximum size of log path read from configuration for logging 
 */
#define SON_MAX_LOG_PATH_STR_LEN    256

/*! \def SON_NULL
 *  \brief A macro that stores the value zero
 */
#define SON_NULL                        0

/*! \def SON_NULL
 *  \brief A macro that stores the value one 
 */
#define SON_ONE                         1
/*! \def SON_PNULL
 *  \brief A macro that defines the intialization value for Pointers.
 */
#define SON_PNULL                       ((void *)0)

#define SON_ONE            1
#define SON_TWO            2
#define SON_THREE          3
#define SON_FOUR           4
#define SON_FIVE           5
#define SON_SIX            6
#define SON_SEVEN          7
#define SON_EIGHT          8
#define SON_NINE           9
#define SON_TEN            10
#define SON_ELEVEN         11
#define SON_TWELVE         12
#define SON_THIRTEEN       13
#define SON_FOURTEEN       14
#define SON_FIFTEEN        15
/*spr 22474 start*/
#define NVARPOOL    27
#define MINBUF      8
#define NVARMSGPOOL    27
#define NVARMEMPOOL    27

/* This is used in debug GET Command */
typedef enum son_GetDebugTypeE
{
    SON_GET_DEBUG_INFO_MEM_STATS, /* Mem Pool Statistics */
    SON_GET_DEBUG_INFO_MSG_STATS  /* Message Pool Statistics */
}son_DebugType;
typedef enum      
{ 
    SON_GET_MEM_POOL_STATS=0,
    SON_GET_MSG_POOL_STATS
}son_oam_debug_info_req_et;
typedef enum
{
    SON_WRONG_DEBUG_TYPE
}son_oam_debug_info_fail_cause_et;

/*spr 22474 end*/

/*! \def SON_VERSION_ID
 *  \brief A macro that stores the version
 * number for the SON module messages
 */
#define SON_VERSION_ID                  01

/*! \def MAX_BUFF_SIZE
 * \brief A macro that defines the Maximum buffer size
 */
#define MAX_BUFF_SIZE                                                  256

/*! \def SON_API_PRIORITY
 *  \brief A macro that stores the priority
 * value for the SON module messages
 */
#define SON_API_PRIORITY                0

/*! \def SON_API_HEADER_SIZE
 *  \brief A macro that stores the CSPL header size
 */
#define SON_API_HEADER_SIZE             16

/*! \def SON_EXT_MAX_MSG_API_HDR_SIZE
 *  \brief A macro that stores the message External API header size
 */
#define SON_EXT_MAX_MSG_API_HDR_SIZE            16

/*! \def EXT_MSG_API_HDR_SIZE
 *  \brief A macro that stores the message External API header size
 */
#define EXT_MSG_API_HDR_SIZE           16 

#ifdef __SON_FAPI__

/*! \def SON_FAPI_MSG_API_HDR_SIZE
 *  \brief A macro that stores the message External API header size
 */
#define SON_FAPI_MSG_API_HDR_SIZE            4
#endif


/*! \def SON_MAX_NO_OF_LOG_LEVELS
 *  \brief A macro that stores the maximum count of the log levels supported.
 *  There are 7 log levels CRITICAL, ERROR, WARNING, INFO, BRIEF, DETAILED and DetailedAll
 */
#define SON_MAX_NO_OF_LOG_LEVELS            7

/*! \def SON_FULL_INTERFACE_HEADERS_SIZE
 *  \brief A macro that stores the total of CSPL header size
 *         and external API header size
 */
#define SON_FULL_INTERFACE_HEADERS_SIZE \
    (CV_HDRSIZE + EXT_MSG_API_HDR_SIZE)

/*! \def IP_PORT_STR_LEN
 *  \brief IP address and port number string(IP address in dot
 *        decimal notation:Port Number) maximum length.
 *        In this string: IP address can maximum be xxx.xxx.xxx.xxx(15 digits)
 *        and port number can be maximum 65535 (5 digits), a column
 *        (1 digit) so total 21 digits + 1 byte for null terminating
 *        the string.
 */
#define IP_PORT_STR_LEN             23

/* SPR-12708 Fix Starts */
/*! \def SON_SECONDS_IN_A_MIN
 *  \brief Macro storing number of seconds in a minute value i.e. 60
 */
#define SON_SECONDS_IN_A_MIN   60
/* SPR-12708 Fix Ends */

/*! \def SON_MILLISECONDS_IN_A_SECOND
 *  \brief Macro storing number of milliseconds in a second value i.e. 1000
 */
#define SON_MILLISECONDS_IN_A_SECOND   1000

/*! \def SON_MAX_NUMBER_OF_THREADS
 *  \brief Macro storing maximum number of threads in SON
 */
#define SON_MAX_NUMBER_OF_THREADS  2

/*! \def LOG_FILENAME_LEN
 *  \brief Macro storing maximum length of SON log file name
 */
#define LOG_FILENAME_LEN            64

/*! \def SON_MAX_RBS
 *  \brief Macro storing maximum number of RBs possible for given bandwidth
 */
#define SON_MAX_RBS                 6

/*! \decl SON_DEFAULT_LOG_FILE
 *  \brief Pointer to array storing default log file name for SON
 */
/* SPR 21622 start */
#define SON_DEFAULT_LOG_FILE        "son_main.log"
/* SPR 21622 end */

/*! \def EXT_MSG_SRC_MODULE_ID_OFFSET
 *  \brief  Offset of Source module Id in the external API header
 */
#define EXT_MSG_SRC_MODULE_ID_OFFSET   2

/*! \def EXT_MSG_DEST_MODULE_ID_OFFSET
 *  \brief  Offset of Destination module Id in the external API header
 */
#define EXT_MSG_DEST_MODULE_ID_OFFSET   4

/*! \def EXT_MSG_API_OFFSET
 *  \brief  Offset of API field in the external API header
 */
#define EXT_MSG_API_OFFSET              6

/*! \def EXT_MSG_BUF_LEN_OFFSET
 *  \brief  Offset of Buffer length field in the external API header
 */
#define EXT_MSG_BUF_LEN_OFFSET          8

/*! \def EXT_MSG_TRANSACTION_ID_OFFSET
 *  \brief Offset of Transaction Id in the external API header
 */
#define EXT_MSG_TRANSACTION_ID_OFFSET   0

/*! \def SON_NTOHS
    /brief macro to convert byte order from network to host of short int.
 */
#define SON_NTOHS                   ntohs

/*! \def EARFCN_SCAN_LIST_DEQUEUE(p_earfcn_scan_res_list, p_earfcn_data)
 *  \brief  To Dequeue a node from the earfcn scan res list received from
 *      SONapps
 */
#define EARFCN_SCAN_LIST_DEQUEUE(p_earfcn_scan_res_list, p_earfcn_data) { \
        YLNODE *snode = ylPopHead(p_earfcn_scan_res_list); \
        p_earfcn_data = YMEMBEROF(earfcn_scan_res_data_t, earfcn_node, snode); \
}

/*! \def CELL_INFO_LIST_DEQUEUE(p_cell_info_list, p_cell_info_data)
 *  \brief  To Dequeue a node from the cell info list received from SONapps
 */
#define CELL_INFO_LIST_DEQUEUE(p_cell_info_list, p_cell_info_data) { \
        YLNODE *snode = ylPopHead(p_cell_info_list); \
        p_cell_info_data = YMEMBEROF(cell_info_data_t, cell_node, snode); \
}

/*! \def CONVERT_RSRQ_DB_TO_REPORTED_VALUE(rsrq_in_db, rsrq)
 *  \brief Convert RSRQ value from DB to reported value as per 3GPP
 */
#define CONVERT_RSRQ_DB_TO_REPORTED_VALUE(rsrq_in_db, rsrq) \
    { \
        if (-19.5 > rsrq_in_db) \
        { \
            rsrq = 0; \
        } \
        else if (-3 <= rsrq_in_db) \
        { \
            rsrq = 34; \
        } \
        else \
        { \
            rsrq = ((son_u8)(rsrq_in_db + 19.5) / 0.5) + 1; \
        } \
    }

/*! \def CONVERT_RSRP_TO_3GPP_REPORTED_VALUE(rsrp_fapi, rsrp_3gpp)
 *  \brief Map reported RSRP value from FAPI to 3GPP.
 *     The conversion can be done by first coverting the reported
 *     value to its dBm equivalent as per FAPI(subtract 144 from reported
 *     value) and then coverting this dbM value to reported value as per 3GPP.
 */
#define CONVERT_RSRP_TO_3GPP_REPORTED_VALUE(rsrp_fapi, rsrp_3gpp) \
    { \
        son_s16 rsrp_in_db = 0; \
        rsrp_in_db = rsrp_fapi - 144; \
        if (-44 <= rsrp_in_db) \
        { \
            rsrp_3gpp = 97; \
        } \
        else if (-140 > rsrp_in_db) \
        { \
            rsrp_3gpp = 0; \
        } \
        else \
        { \
            rsrp_3gpp = rsrp_in_db + 141; \
        } \
    }

/*! \def CONVERT_RSSI_TO_3GPP_REPORTED_VALUE(rssi_fapi, rssi_3gpp)
 *  \brief Map reported RSSI value from FAPI to 3GPP.
 *     The conversion can be done by first coverting the reported
 *     value to its dBm equivalent as per FAPI(subtract 144 from reported
 *     value) and then coverting this dbM value to reported value as per 3GPP.
 */
#define CONVERT_RSSI_TO_3GPP_REPORTED_VALUE(rssi_fapi, rssi_3gpp) \
    { \
        son_s16 rssi_in_db = 0; \
        rssi_in_db = rssi_fapi - 144; \
        if (-44 <= rssi_in_db) \
        { \
            rssi_3gpp = 97; \
        } \
        else if (-140 > rssi_in_db) \
        { \
            rssi_3gpp = 0; \
        } \
        else \
        { \
            rssi_3gpp = rssi_in_db + 141; \
        } \
    }

/*! \def SON_MAX_PRACH_CONFIG_IDX
 *  \brief A macro that stores the maximum number of PRACH configuration index values possible 
 */
#define SON_MAX_PRACH_CONFIG_IDX 64

/*! \enum son_buf_retain_status_et
 *  \brief An enum that specifies if SON buffer
 *        is to be freed or not
 */
typedef enum _son_buf_retain_status_e
{
    RETAIN_SON_BUFFER = 0,
    RELEASE_SON_BUFFER
}son_buf_retain_status_et;

/*! \enum son_procedure_code_et
 *  \brief An enum that specifies the Procedure Codes for the messages
 */
typedef enum _son_procedure_code_e
{
    /*NMM proc ID*/
    SONNMM_INIT_IND = 50,
    /* NMMCTL <= => NMM */
    NMM_FSM_EX_MSG_BASE = 100,
    SONNMM_START_SCAN_REQ = NMM_FSM_EX_MSG_BASE,
    SONNMM_PERIODIC_SCAN_REQ,
    SONNMM_STOP_SCAN_REQ,
    SONNMM_START_SCAN_RES,
    SONNMM_STOP_SCAN_RES,
    SONNMM_LAST_SCAN_TIMESTAMP_IND,
    NMM_NMMCTL_UNDEFINED,

    /* SONANR REQ APIS */
    SONANR_REQ_BASE_API_ID = 1000,
    SONANR_ENABLE_REQ = SONANR_REQ_BASE_API_ID,
    SONANR_DISABLE_REQ,
    SONANR_NR_ADD_REQ,
    SONANR_NR_DELETE_REQ,
    SONANR_NR_UPDATE_REQ,
    SONANR_NR_RETRIEVE_REQ,
    SONANR_NR_SCAN_REQ,
    SONANR_MEAS_CONFIG_REQ,
    SONANR_PCI_REG_DEREG_REQ,
    SONANR_PCI_IN_USE_LIST_REQ,
    SONANR_MODIFY_UE_COUNT_REQ,
    SONANR_MODIFY_MEAS_REPORTING_INTERVAL_REQ,
    SONANR_NR_DELETE_FROM_REMOVE_LIST_REQ,
    SONANR_MODIFY_REMOVAL_ATTRIBUTES_REQ,
    SONANR_MODIFY_PCI_CONFUSION_CFG_REQ,
    SONANR_ENABLE_DISABLE_X2_REQ,
    SONANR_ENABLE_DISABLE_LIMITED_MODE_REQ,
    SONANR_NR_ENB_ADD_REQ,
    SONANR_NR_ENB_UPDATE_REQ,
    SONANR_NR_ENB_DELETE_REQ,
    SONANR_NR_ENB_RETRIEVE_REQ,
    SONANR_NEIGHBOR_DEFAULT_CONFIG_REQ,
    /* SPR-10331 Fix Starts */
    /* Line Deleted */
    /* SPR-10331 Fix Ends */
    SONANR_MEAS_CONFIG_OBJ_REMOVE_REQ,
    /* SPR 20653 Fix Start */
    SONANR_MODIFY_ATTRIBUTE_REQ,
    /* This should always be the last value of enum */
    SONANR_REQ_MAX_API = SONANR_MODIFY_ATTRIBUTE_REQ,
    /* SPR 20653 Fix End */

    /* SONANR RES/IND APIS */
    SONANR_RES_BASE_API_ID = 1500,
    SONANR_ENABLE_RES = SONANR_RES_BASE_API_ID,
    SONANR_DISABLE_RES,
    SONANR_NR_ADD_RES,
    SONANR_NR_DELETE_RES,
    SONANR_NR_UPDATE_RES,
    SONANR_NR_RETRIEVE_RES,
    SONANR_NR_SCAN_RES,
    SONANR_MEAS_CONFIG_RES,
    SONANR_PCI_REG_DEREG_RES,
    SONANR_PCI_IN_USE_LIST_RES,
    SONANR_PCI_CONFLICT_CONFUSION_IND,
    SONANR_UPDATED_NRT_INFO,
    SONANR_MODIFY_UE_COUNT_RES,
    SONANR_MODIFY_MEAS_REPORTING_INTERVAL_RES,
    SONANR_NR_DELETE_FROM_REMOVE_LIST_RES,
    SONANR_MODIFY_REMOVAL_ATTRIBUTES_RES,
    SONANR_MODIFY_PCI_CONFUSION_CFG_RES,
    SONANR_ENABLE_DISABLE_X2_RES,
    SONANR_ENABLE_DISABLE_LIMITED_MODE_RES,
    SONANR_NR_ENB_ADD_RESP,
    SONANR_NR_ENB_UPDATE_RESP,
    SONANR_NR_ENB_DELETE_RESP,
    SONANR_NR_ENB_RETRIEVE_RESP,
    SONANR_LIMITED_MODE_PCI_INFO,
    SONANR_FEATURE_STATE_CHANGE_IND,
    SONANR_DISABLE_IND,
    SONANR_NEIGHBOR_DEFAULT_CONFIG_RESP,
    /* SPR-10331 Fix Starts */
    /* Line Deleted */
    /* SPR-10331 Fix Ends */
    SONANR_MEAS_CONFIG_OBJ_REMOVE_RES,
    /* SPR 20653 Fix Start */
    SONANR_MODIFY_ATTRIBUTE_RES,

    /* This should always be the last value of enum */
    SONANR_RES_MAX_API = SONANR_MODIFY_ATTRIBUTE_RES,
    /* SPR 20653 Fix End */

    /* SONES <= => SONMgmtIfH */
    SONES_REQ_BASE_API_ID = 2000,
    SONES_ENABLE_REQ = SONES_REQ_BASE_API_ID,
    SONES_DISABLE_REQ,
    SONES_MODIFY_UE_COUNT_INTERVAL_REQ,
    SONES_MODIFY_UE_COUNT_THRESHOLD_REQ,
    SONES_CELL_SWITCH_ON_REQ,
    SONES_CELL_SWITCH_OFF_REQ,
    SONES_SWITCHED_OFF_CELL_INFO_REQ,
    /* AUTONOMOUS SWITCH OFF START */
    SONES_AUTONOMOUS_SWITCH_OFF_CONFIG_REQ,
    /* AUTONOMOUS SWITCH OFF END */
    SONES_FEATURE_STATE_CHANGE_IND,
    /* SPR_19619 start */
    SONES_PEER_CELL_ACTIVATION_REQ,
    /* SPR_19619 stop */

    /* SPR_19619 start */
    /* This should always be the last value of enum */
    SONES_REQ_MAX_API = SONES_PEER_CELL_ACTIVATION_REQ,
    /* SPR_19619 stop */

    SONES_RES_BASE_API_ID = 2500,
    SONES_ENABLE_RES = SONES_RES_BASE_API_ID,
    SONES_DISABLE_RES,
    SONES_MODIFY_UE_COUNT_INTERVAL_RES,
    SONES_MODIFY_UE_COUNT_THRESHOLD_RES,
    SONES_CELL_SWITCH_ON_RES,
    SONES_CELL_SWITCH_OFF_RES,
    SONES_SWITCHED_OFF_CELL_INFO_RES,
    /* AUTONOMOUS SWITCH OFF START */
    SONES_AUTONOMOUS_SWITCH_OFF_CONFIG_RESP,
    /* AUTONOMOUS SWITCH OFF END */
    SONES_ACTIVE_UE_COUNT_REPORT,
    SONES_THRESHOLD_HIT_IND,
    SONES_REQUEST_TO_SWITCH_ON_CELL,
    /* -------------------ES 1.2 Changes Start ------------------------*/
    SONES_CELL_SWITCH_ON_OFF_IND,
    /* -------------------ES 1.2 Changes Finish ------------------------
    * This should always be the last value of enum */
    SONES_RES_MAX_API = SONES_CELL_SWITCH_ON_OFF_IND,

    /* SONAPPS <==> SONMgmtIfH */
    SONAPPS_REQ_BASE_API_ID = 3000,
    SONAPPS_NW_SCAN_REQ = SONAPPS_REQ_BASE_API_ID,
    SONAPPS_CARRIER_FREQ_SELECTION_REQ,
    SONAPPS_PCI_CONFLICT_ENABLE_DISABLE_REQ,
    SONAPPS_PCI_SELECTION_REQ,
    SONAPPS_PCI_RESELECTION_REQ,
    SONAPPS_PERIODIC_NW_SCAN_REQ,
    /*REL 1.4:START*/
    SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_REQ,
    /*REL 1.4:STOP*/

    /* This should always be the last value of enum */
    SONAPPS_REQ_MAX_API,

    /* SONAPPS <==> SONMgmtIfH */
    SONAPPS_RES_BASE_API_ID = 3500,
    SONAPPS_NW_SCAN_RES = SONAPPS_RES_BASE_API_ID,
    SONAPPS_CARRIER_FREQ_SELECTION_RES,
    SONAPPS_PCI_CONFLICT_ENABLE_DISABLE_RES,
    SONAPPS_PCI_SELECTION_RES,
    SONAPPS_PCI_RESELECTION_RES,
    SONAPPS_PCI_CONFLICT_CONFUSION_IND,
    /*REL 1.4:START*/
    SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_RESP,
    /*REL 1.4:STOP*/

    /*This should always be the last value of enum*/
    SONAPPS_RES_MAX_API,

    /* SONAPPS <==> SONANR */

    /* MRO */
    SONMRO_REQ_BASE_API_ID = 4000,
    SONMRO_ENABLE_REQ = SONMRO_REQ_BASE_API_ID,
    SONMRO_DISABLE_REQ,
    SONMRO_MODIFY_CONFIG_PARAM_REQ,

    SONMRO_REQ_MAX_API,

    SONMRO_RES_BASE_API_ID = 4500,
    SONMRO_ENABLE_RES = SONMRO_RES_BASE_API_ID,
    SONMRO_DISABLE_RES,
    SONMRO_MODIFY_CONFIG_PARAM_RES,
    SONMRO_EUTRAN_TTT_UPDATE_IND,
    SONMRO_FEATURE_STATE_CHANGE_IND,
    SONMRO_HO_COUNTER_REPORT,
    SONMRO_OPTIMIZATION_NOTIFICATION,
    SONMRO_RES_MAX_API,

    /* MLB */
    SONMLB_REQ_BASE_API_ID = 5000,
    SONMLB_ENABLE_REQ = SONMLB_REQ_BASE_API_ID,
    SONMLB_DISABLE_REQ,
    SONMLB_MODIFY_ATTRIBUTES_REQ,
    SONMLB_MOBILITY_CHANGE_REQ,
    /*SONMLB-- Centralized support start*/
    SONMLB_OFFSET_CHANGE_REQ,
    SONMLB_PEER_OFFSET_CHANGE_RESP,
    /*SONMLB-- Centralized support end*/
    /* Rel 3.0 Multi Sector code changes Start */
    SONMLB_INTERNAL_MOBILITY_CHANGE_REQ,  
    /* Rel 3.0 Multi Sector code changes Stop */
    SONMLB_REQ_MAX_API,

    SONMLB_RES_BASE_API_ID = 5500,
    SONMLB_ENABLE_RES = SONMLB_RES_BASE_API_ID,
    SONMLB_DISABLE_RES,
    SONMLB_MODIFY_ATTRIBUTES_RES,
    SONMLB_MOBILITY_CHANGE_ACK,
    SONMLB_MOBILITY_CHANGE_FAILURE,
    SONMLB_FEATURE_STATE_CHANGE_IND,
    /*SONMLB-- Centralized support start*/
    SONMLB_HO_LOAD_PRARAMS_INFO,
    SONMLB_OFFSET_CHANGE_RESP,
    SONMLB_PEER_OFFSET_CHANGE_REQ,
    /*SONMLB-- Centralized support end*/
    SONMLB_NBR_CELL_STATUS_IND,
    /* Rel 3.0 Multi Sector code changes Start */
    SONMLB_INTERNAL_MOBILITY_CHANGE_ACK,
    SONMLB_INTERNAL_MOBILITY_CHANGE_FAILURE,
    /* Rel 3.0 Multi Sector code changes Stop */
    SONMLB_RES_MAX_API,

    /* RACH_OPT */
    SONRACH_OPT_REQ_BASE_API_ID = 6000,
    SONRACH_OPT_ENABLE_REQ = SONRACH_OPT_REQ_BASE_API_ID,
    SONRACH_OPT_DISABLE_REQ,
    SONRACH_OPT_MODIFY_CONFIG_PARAMS_REQ,
    SONRACH_OPT_MODIFY_RACH_OPTMODE_REQ,
    SONRACH_OPT_REQ_MAX_API,

    SONRACH_OPT_RES_BASE_API_ID = 6500,
    SONRACH_OPT_ENABLE_RES = SONRACH_OPT_RES_BASE_API_ID,
    SONRACH_OPT_DISABLE_RES,
    SONRACH_OPT_MODIFY_CONFIG_PARAMS_RES,
    SONRACH_OPT_MODIFY_RACH_OPTMODE_RES,
    SONRACH_OPT_PARAMETER_UPDATE_REQ,
    SONRACH_OPT_FAILURE_NOTIFICATION,
    SONRACH_OPT_UE_INFO_REPORT,
    SONRACH_OPT_L2_PERF_REPORT,
    SONRACH_OPT_FEATURE_STATE_CHANGE_IND,
    SONRACH_OPT_RES_MAX_API,

    /* SONMgmtIfH <= => OAM */
    SON_OAM_INIT_IND = 12000,
    SON_OAM_INIT_CONFIG_REQ,
    SON_OAM_INIT_CONFIG_RESP,
    SON_OAM_SHUTDOWN_REQ,
    SON_OAM_SHUTDOWN_RESP,
    SON_OAM_SET_LOG_LEVEL_REQ,
    SON_OAM_SET_LOG_LEVEL_RESP,
    SON_OAM_LOG_ENABLE_DISABLE_REQ,
    SON_OAM_LOG_ENABLE_DISABLE_RESP,
    SON_OAM_CELL_INFO_IND,
    SON_OAM_ENABLE_DISABLE_X2_REQ,
    SON_OAM_ENABLE_DISABLE_X2_RESP,
    SON_OAM_X2_LINK_UP_IND,
    SON_OAM_X2_LINK_DOWN_IND,
    SON_OAM_SET_NW_SCAN_CONFIG_CMD,
    SON_OAM_PERIODIC_NW_SCAN_REQ,
    SON_OAM_NW_SCAN_REQ,
    SON_OAM_BULK_NW_SCAN_RESP,
    SON_OAM_PCI_CONFLICT_IND,
    SON_OAM_NR_SCAN_REQ,
    SON_OAM_NR_SCAN_RESP,
    SON_OAM_NR_ADD_REQ,
    SON_OAM_NR_ADD_RESP,
    SON_OAM_NR_UPDATE_REQ,
    SON_OAM_NR_UPDATE_RESP,
    SON_OAM_NR_DELETE_REQ,
    SON_OAM_NR_DELETE_RESP,
    SON_OAM_NR_RETRIEVE_REQ,
    SON_OAM_NR_RETRIEVE_RESP,
    SON_OAM_ANR_UPDATED_NRT_INFO,
    SON_OAM_ANR_ENABLE_REQ,
    SON_OAM_ANR_ENABLE_RESP,
    SON_OAM_ANR_DISABLE_REQ,
    SON_OAM_ANR_DISABLE_RESP,
    SON_OAM_ANR_MEAS_CONFIG_REQ,
    SON_OAM_ANR_MEAS_CONFIG_RESP,
    SON_OAM_ANR_MODIFY_UE_COUNT_REQ,
    SON_OAM_ANR_MODIFY_UE_COUNT_RESP,
    SON_OAM_ANR_MODIFY_MEAS_REPORTING_INTERVAL_REQ,
    SON_OAM_ANR_MODIFY_MEAS_REPORTING_INTERVAL_RESP,
    SON_OAM_NR_DELETE_FROM_REMOVE_LIST_REQ,
    SON_OAM_NR_DELETE_FROM_REMOVE_LIST_RESP,
    SON_OAM_ANR_MODIFY_REMOVAL_ATTRIBUTES_REQ,
    SON_OAM_ANR_MODIFY_REMOVAL_ATTRIBUTES_RESP,
    SON_OAM_ANR_MODIFY_PCI_CONFUSION_CFG_REQ,
    SON_OAM_ANR_MODIFY_PCI_CONFUSION_CFG_RESP,
    SON_OAM_ANR_ENABLE_DISABLE_LIMITED_MODE_REQ,
    SON_OAM_ANR_ENABLE_DISABLE_LIMITED_MODE_RESP,
    SON_OAM_NR_ENB_ADD_REQ,
    SON_OAM_NR_ENB_ADD_RESP,
    SON_OAM_NR_ENB_UPDATE_REQ,
    SON_OAM_NR_ENB_UPDATE_RESP,
    SON_OAM_NR_ENB_DELETE_REQ,
    SON_OAM_NR_ENB_DELETE_RESP,
    SON_OAM_NR_ENB_RETRIEVE_REQ,
    SON_OAM_NR_ENB_RETRIEVE_RESP,
    SON_OAM_ANR_LIMITED_MODE_PCI_INFO,
    SON_OAM_ANR_NEIGHBOR_DEFAULT_CONFIG_REQ,
    SON_OAM_ANR_NEIGHBOR_DEFAULT_CONFIG_RESP,
    SON_OAM_ANR_GU_GROUP_ID_CONFIG_REQ,
    SON_OAM_ANR_GU_GROUP_ID_CONFIG_RESP,
    SON_OAM_ES_ENABLE_REQ,
    SON_OAM_ES_ENABLE_RESP,
    SON_OAM_ES_DISABLE_REQ,
    SON_OAM_ES_DISABLE_RESP,
    SON_OAM_ES_MODIFY_UE_COUNT_INTERVAL_REQ,
    SON_OAM_ES_MODIFY_UE_COUNT_INTERVAL_RESP,
    SON_OAM_ES_ACTIVE_UE_COUNT_REPORT,
    SON_OAM_ES_CELL_SWITCH_OFF_REQ,
    SON_OAM_ES_CELL_SWITCH_OFF_RESP,
    SON_OAM_ES_CELL_SWITCH_ON_REQ,
    SON_OAM_ES_CELL_SWITCH_ON_RESP,
    SON_OAM_ES_SWITCHED_OFF_CELL_INFO_REQ,
    SON_OAM_ES_SWITCHED_OFF_CELL_INFO_RESP,
    SON_OAM_ES_MODIFY_UE_COUNT_THRESHOLD_REQ,
    SON_OAM_ES_MODIFY_UE_COUNT_THRESHOLD_RESP,
    SON_OAM_ES_UE_COUNT_THRESHOLD_HIT_IND,
    SON_OAM_ES_REQUEST_TO_SWITCH_ON_CELL,
    SON_OAM_ES_CELL_SWITCH_ON_OFF_IND,
    SON_OAM_PCI_CONFLICT_ENABLE_DISABLE_REQ,
    SON_OAM_PCI_CONFLICT_ENABLE_DISABLE_RESP,
    SON_OAM_PCI_SELECTION_REQ,
    SON_OAM_PCI_SELECTION_RESP,
    SON_OAM_PCI_RESELECTION_REQ,
    SON_OAM_PCI_RESELECTION_RESP,
    SON_OAM_PCI_CONFLICT_CONFUSION_IND,
    SON_OAM_CARRIER_FREQ_SELECTION_REQ,
    SON_OAM_CARRIER_FREQ_SELECTION_RESP,
    SON_OAM_PROC_SUP_REQ,
    SON_OAM_PROC_SUP_RESP,
    SON_OAM_FEATURE_STATE_CHANGE_IND,
    SON_OAM_MRO_ENABLE_REQ,
    SON_OAM_MRO_ENABLE_RESP,
    SON_OAM_MRO_DISABLE_REQ,
    SON_OAM_MRO_DISABLE_RESP,
    SON_OAM_MRO_MODIFY_CONFIG_PARAM_REQ,
    SON_OAM_MRO_MODIFY_CONFIG_PARAM_RESP,
    SON_OAM_MRO_EUTRAN_TTT_UPDATE_IND,
    SON_OAM_MRO_HO_COUNTER_REPORT,
    SON_OAM_MRO_OPTIMIZATION_NOTIFICATION,
    SON_OAM_MLB_ENABLE_REQ,
    SON_OAM_MLB_ENABLE_RESP,
    SON_OAM_MLB_DISABLE_REQ,
    SON_OAM_MLB_DISABLE_RESP,
    SON_OAM_MLB_MODIFY_ATTRIBUTES_REQ,
    SON_OAM_MLB_MODIFY_ATTRIBUTES_RESP,
    SON_OAM_EVENT_NOTIFICATION,
    /*REL 1.4:START*/
    SON_OAM_CARRIER_FREQ_AND_DL_UL_POWER_SELECTION_REQ,
    SON_OAM_CARRIER_FREQ_AND_DL_UL_POWER_SELECTION_RES,
    /*REL 1.4:STOP*/
    /* SPR 20653 Fix Start */
    SON_OAM_ANR_MODIFY_ATTRIBUTE_REQ,
    SON_OAM_ANR_MODIFY_ATTRIBUTE_RESP,
    /* SPR 20653 Fix End */

    SON_OAM_LAST_SCAN_TIMESTAMP_IND,

    /* SONApps/ANR/ES/Nmm <= => SONMIF
     *SONMLB-- Centralized support start*/
    SON_OAM_MLB_PRARAMS_INFO,
    SON_OAM_MLB_OFFSET_CHANGE_REQ,
    SON_OAM_MLB_OFFSET_CHANGE_RESP,
    SON_OAM_MLB_PEER_OFFSET_CHANGE_REQ,
    SON_OAM_MLB_PEER_OFFSET_CHANGE_RESP,
    /*SONMLB-- Centralized support end
     * AUTONOMOUS SWITCH OFF START */
    SON_OAM_ES_AUTONOMOUS_SWITCH_OFF_CONFIG_REQ,
    SON_OAM_ES_AUTONOMOUS_SWITCH_OFF_CONFIG_RESP,
    SON_OAM_CHK_HEALTH_REQ,
    SON_OAM_CHK_HEALTH_RESP,
    /* AUTONOMOUS SWITCH OFF END */
    SON_OAM_ANR_MEAS_CONFIG_OBJ_REMOVE_REQ,
    SON_OAM_ANR_MEAS_CONFIG_OBJ_REMOVE_RESP,
    SON_OAM_RACH_OPT_ENABLE_REQ,
    SON_OAM_RACH_OPT_ENABLE_RESP,
    SON_OAM_RACH_OPT_DISABLE_REQ,
    SON_OAM_RACH_OPT_DISABLE_RESP,
    SON_OAM_RACH_OPT_MODIFY_RACH_OPTMODE_REQ,
    SON_OAM_RACH_OPT_MODIFY_RACH_OPTMODE_RESP,
    SON_OAM_RACH_OPT_MODIFY_CONFIG_PARAMS_REQ,
    SON_OAM_RACH_OPT_MODIFY_CONFIG_PARAMS_RESP,
    SON_OAM_RACH_OPT_PARAMETER_UPDATE_REQ,
    SON_OAM_RACH_OPT_FAILURE_NOTIFICATION,
    SON_OAM_RACH_OPT_UE_INFO_REPORT,
/* Bug 8036 Fix start */
    SON_OAM_RACH_OPT_L2_PERF_REPORT,
    /* Rel 3.0 Cell delete support changes Start */
    SON_OAM_CELL_DELETE_REQ,
    SON_OAM_CELL_DELETE_RESP,
    /* Rel 3.0 Cell delete support changes Stop */
    /* SPR-13251 Fix Starts */
    SON_OAM_GET_LOG_LEVEL_REQ,
    SON_OAM_GET_LOG_LEVEL_RESP,
    /* SPR-13251 Fix End */
 /* SPR_19619 start */
    SON_OAM_PEER_CELL_ACTIVATION_REQ,
    SON_OAM_PEER_CELL_ACTIVATION_RESP,
    /* SPR_19619 stop */
/*spr 22474 start*/    
    SON_OAM_GET_DEBUG_INFO_REQ, 
    SON_OAM_GET_DEBUG_INFO_RESP,
/*spr 22474 end*/    
/* SPR-13251 Fix Ends */
    /*This should always be the last value of API Id with OAM */
    SON_OAM_MAX_API_ID,
/* Bug 8036 Fix end */

    SMIF_REQ_BASE_API_ID = 13000,
    SMIF_INIT_CONFIG_REQ = SMIF_REQ_BASE_API_ID,
    SMIF_SHUTDOWN_REQ,
    /* Rel 3.0 Cell delete support changes Start */
    SMIF_CELL_DELETE_REQ,
    /* Rel 3.0 Cell delete support changes Stop */
    /* SPR-13251 Fix Starts */
    SMIF_GET_LOG_LEVEL_REQ,
    /* SPR-13251 Fix Ends */
    /*This should always be the last value of enum*/
    SMIF_REQ_MAX_API,

    SMIF_RES_BASE_API_ID = 13500,
    SMIF_INIT_CONFIG_RES = SMIF_RES_BASE_API_ID,
    SMIF_SHUTDOWN_RES,
    SMIF_GENERIC_RES,
    SMIF_LOG_ENABLE_DISABLE_CMD,
    SMIF_SET_LOG_LEVEL_CMD,
    SMIF_CELL_INFO_IND,
    SMIF_INIT_IND,
    SMIF_SET_SCAN_CONFIG_CMD,
    /* Rel 3.0 Cell delete support changes Start */
    SMIF_CELL_DELETE_RESP,
    /* Rel 3.0 Cell delete support changes Stop */
    /* SPR-13251 Fix Starts */
    SMIF_GET_LOG_LEVEL_RESP,
    /* SPR-13251 Fix Ends */
    /* SPR_19619 start */
    SMIF_PEER_CELL_ACTIVATION_RESP,
    /* SPR_19619 stop */
  /*This should always be the last value of enum*/
    SMIF_RES_MAX_API,

    /* SPR-10331 Fix Starts */
    /* ANR/ES/SMIF <==> X2 */
    /* SPR-10331 Fix Ends */
    SONX2_REQ_BASE_API_ID = 14000,
    SONX2_ADD_ENB_REQ = SONX2_REQ_BASE_API_ID,
    SONX2_DELETE_ENB_REQ,
    SONX2_NRT_UPDATE_IND,
    SONX2_ENB_RESET_REQ,
    SONX2_SWITCH_ON_OFF_IND,
    SONX2_MOBILITY_CHANGE_REQ,
    SONX2_MOBILITY_CHANGE_ACK,
    SONX2_MOBILITY_CHANGE_FAILURE,
    /* SPR-10331 Fix Starts */
    SONX2_GU_GROUP_ID_CONFIG_REQ,
   
/* SPR_19619 start */
    SONX2_PEER_CELL_ACTIVATION_REQ,

   /*This should always be the last value of enum*/
    SONX2_REQ_MAX_API = SONX2_PEER_CELL_ACTIVATION_REQ,
/* SPR_19619 stop */
    /* SPR-10331 Fix Ends */

    SONX2_RES_BASE_API_ID = 14500,
    SONX2_ADD_ENB_RES = SONX2_RES_BASE_API_ID,
    SONX2_LINK_UP_IND,
    SONX2_LINK_DOWN_IND,
    SONX2_DELETE_ENB_RES,
    SONX2_ENB_RESET_RES,
    SONX2_HO_REPORT_IND,
    SONX2AP_HO_REPORT_IND,
    /* SPR-10331 Fix Starts */
    SONX2_GU_GROUP_ID_CONFIG_RES,
    
    SONX2_ENB_CONFIG_UPDATE_IND,
    /*This should always be the last value of enum*/
    SONX2_RES_MAX_API = SONX2_GU_GROUP_ID_CONFIG_RES,
    /* SPR-10331 Fix Ends */

    /* SPR 11692 changes start */
    /* ANR <=> ES */
    SONES_CELL_SWITCH_ON_FOR_X2_SETUP_REQ = 15000,
    /*SPR11689 changes start */
    SONES_CELL_ACTIVATION_REQ ,
    /*SPR11689 changes end */
    /*This should always be the last value of enum*/
    SONES_X2_REQ_MAX_API = SONES_CELL_ACTIVATION_REQ,

    SONES_CELL_SWITCH_ON_FOR_X2_SETUP_RES = 15500,
    /*SPR11689 changes start */
    SONES_CELL_ACTIVATION_RESP ,
    /*SPR11689 changes end */
    /* SPR_19619 start */ 
    SONX2_PEER_CELL_ACTIVATION_RESP,
    /*This should always be the last value of enum*/
    SONES_X2_RES_MAX_API = SONX2_PEER_CELL_ACTIVATION_RESP,
    /* SPR_19619 stop */ 
    /* SPR 11692 changes end */


    /* Rel 3.0 Multi Sector code changes Start */
    /* ES <=> ANR */
    SONANR_CELL_SWITCH_ON_OFF_IND = 16000,
    /*This should always be the last value of enum*/
    SONANR_ES_REQ_MAX_API = SONANR_CELL_SWITCH_ON_OFF_IND,
    /* Rel 3.0 Multi Sector code changes Stop */



    SON_PROCEDURE_UNDEFINED = 0xFFFF
}son_procedure_code_et;

/*! \  struct ext_api_hdr_t
 *  \brief Structure for External API Header
 *  \param transaction_id       Transaction Identifier
 *  \param source_id            Source Module Id
 *  \param dest_id              Destination Module Identifier
 *  \param api_id               API Identifier
 *  \param buf_len              Message Buffer Length
 */
typedef struct _ext_api_hdr_t
{
    son_u16 transaction_id;
    son_u16 source_id;
    son_u16 dest_id;
    son_u16 api_id;
    son_u16 buf_len;
}ext_api_hdr_t;

/*! \enum phy_nmm_error_code_et specific Error Codes
 *  \brief An enum that specifies the common error code.
 */

typedef enum
{
    /*FAPI ERROR CODE
     *    if any
     *END OF FAPI ERROR CODE*/

    MSPD_PHY_OK = 0,                    /*Should start from Zero*/
    MSPD_PHY_NOT_PARAM,                 /* PHY ERR 1*/
    MSPD_PHY_CTX_ALLOC_ERROR,           /* PHY ERR 2*/
    MSPD_PHY_PART_ALLOC_ERROR,          /* PHY ERR 3*/
    MSPD_PHY_UNKNOWN_API,               /* PHY ERR 4*/
    MSPD_PHY_UNKNOWN_TYPE,              /* PHY ERR 5*/
    MSPD_PHY_UNKNOWN_PARAM,             /* PHY ERR 6*/
    MSPD_PHY_CONTEX_ALLOC_ERROR,        /* PHY ERR 7*/
    MSPD_PHY_INCORRECT_PARAM,           /* PHY ERR 8*/
    MSPD_PHY_NO_VALID_DATA_ERROR,       /* PHY ERR 9*/
    MSPD_PHY_HEAP_ALLOC_ERROR,          /* PHY ERR 10*/
    MSPD_NMM_NOT_ENABLED = 0x81,      /* PHY ERR 0x81*/
    MSPD_NMM_UNEXPECTED_CMD,            /* PHY ERR 0x82*/
    MSPD_NMM_INVALID_FREQ_BAND,         /* PHY ERR 0x83*/
    MSPD_NMM_INVALID_EARFCN,            /* PHY ERR 0x84*/
    MSPD_NMM_FREQ_BAND_NOT_SUPPORTED_BY_HW, /* PHY ERR 0x85*/
    MSPD_NMM_INVALID_NR_CARRIER_LIST,   /* PHY ERR 0x86*/
    MSPD_NMM_INVALID_CARRIER_LIST,      /* PHY ERR 0x87*/
    MSPD_NMM_LTE_IN_USE,                /* PHY ERR 0x88*/
    MSPD_NMM_LTE_OUT_OF_MEMORY,          /* PHY ERR 0x89*/
    MSPD_NMM_SIB_INCOMPLETE              /* PHY ERR 0x8a*/
}phy_nmm_error_code_et;



/*! \enum son_duplex_mode_et
 *  \brief An enum that specifies the Duplex mode for the cell
 */
typedef enum
{
    SON_FDD_MODE = 0,
    SON_TDD_MODE,
    SON_MAX_NUM_OF_DUPLEX_MODES,
    SON_INVALID_MODE = 0xFF
}son_duplex_mode_et;


/*! \enum preamble_format_et
 *  \brief An enum that specifies the various possible preamble formats
 */
typedef enum
{
    PREAMBLE_FORMAT_0 = 0,
    PREAMBLE_FORMAT_1,
    PREAMBLE_FORMAT_2,
    PREAMBLE_FORMAT_3,
    PREAMBLE_FORMAT_4,
    PREAMBLE_FORMAT_NA
}preamble_format_et;

/*! \  struct frame_type_1_ra_config_data_for_preamble
 *  \brief Structure for storing Frame structure type 1 random access configurations for preamble formats 0-3 used in FDD mode
 *  \param preamble_format      Preamble Format
 *  \param density_per_10_ms    Density per 10 ms
 */
typedef struct _frame_type_1_ra_config_data_for_preamble
{
    preamble_format_et  preamble_format;
    son_float           density_per_10_ms;
}frame_type_1_ra_config_data_for_preamble;

/*! \  struct frame_type_2_ra_config_data_for_preamble
 *  \brief Structure for storing Frame structure type 2 random access configurations for preamble formats 0-4 used in TDD mode 
 *  \param preamble_format      Preamble Format
 *  \param density_per_10_ms    Density per 10 ms
 */
typedef struct _frame_type_2_ra_config_data_for_preamble
{
    preamble_format_et  preamble_format;
    son_float           density_per_10_ms;
}frame_type_2_ra_config_data_for_preamble;

#ifndef LOGGING_ENHANCEMENT_CELL_AND_UE_INDEX_STRUCT
#define LOGGING_ENHANCEMENT_CELL_AND_UE_INDEX_STRUCT
typedef struct _cell_and_ue_index
{
    U8  cell_index;
    U16 ue_index;
} cell_and_ue_index;
#endif

#endif /* _SON_DEFINES_H_ */
