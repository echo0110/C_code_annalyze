/* vim:ts=4:et:ai:ci:sw=4
*/
/*****************************************************************************
 * File Name   : oam_defines.h
 *
 * Description : This file contains common defines required by OAM modules
 *
 * Revision History:
 *
 * Date             Author              Reference          Comments
 * -----            ------               ---------          --------
 * April,2012      Nitin Agarwal     OAM Design Document   Initial Version
 * June,2012       Brijesh Madolia                         updated wrapper functions
 * September 2012  Shivam Garg                             QCLI Architecture Change    
 * October 2012    Vivek/Harsh                             Changes related to RRH
 * September 2012  Shivam Garg			                   QCLI Architecture Change 	
 * January 2013    Vinod Vyas       OAM Design Doc         Changes for SDM
 *                                  Release 2.0
 * Nov, 2013        Vinod Vyas                             CSR 00051709 Fix    
 * April 2014       Nitin/Vinod                            Cell State handling for multicell
 * April, 2014        Vinod Vyas                           CSR 57877 FIx       
 * June, 2014        Vivek K Gupta  Release 3.0            Changes for xml 
 *                                                         add/delete
 *                                                         framwork code
 * July,2014     Aditi Singh           SPR 9620 Fix                  Guard timer Added
 *  Aug 2014         Shalu Sagar         SPR 12929 FIX
 *  Oct 2014         Ankita Jain         SPR 13586 FIX
 * Sep 2014       Vinod Vyas        Changes Related to eICIC 
 * Nov 2014      Karuna Bhardwaj    EICIC Enhancement
 * Copyright (c) 2014, Aricent Inc. All Rights Reserved
 ****************************************************************************/

#ifndef OAM_DEFINES_H
#define OAM_DEFINES_H
#ifndef _MSC_VER
#include<lteLogger.h>
#endif
/****************************************************************************
 **  MACROS
 ****************************************************************************/
/* SPR 14383 START */
#define ENUM_KPI_CAUSE_FLAG                                           1
/* SPR 14383 END */
#define OAM_CELL_INFO_PARAM_MOD_REQ                                   1
#define OAM_CELL_INFO_PARAM_MOD_RESP                                  2
#define OAM_SIB2_PARAM_MOD_REQ                                        3
#define OAM_SIB2_PARAM_MOD_RESP                                       4
#define OAM_CELL_REST_PARAM_MOD_REQ                                   5
#define OAM_CELL_REST_PARAM_MOD_RESP                                  6
#define OAM_RACH_PARAM_MOD_REQ                                        7
#define OAM_RRC_STATS_ALL                                             8
#define OAM_RRC_TIMER_PARAM_MOD_REQ                                   9
#define OAM_RRC_TIMER_PARAM_MOD_RESP                                 10
#define OAM_GET_LOG_CAT_EGTPU_REQ                                    26
#define OAM_GET_LOG_CAT_EGTPU_RESP                                   27
#define OAM_GET_LOG_CAT_L2_REQ                                       28
#define OAM_GET_LOG_CAT_L2_RESP                                      29
#define OAM_MAX_GSM_INST_ID_VAL                                      32
#define OAM_GET_LOG_LEVEL_L2_REQ                                     37
#define OAM_GET_LOG_LEVEL_L2_RESP                                    38
#define OAM_GET_LOG_LEVEL_RRC_REQ                                    39
#define OAM_GET_LOG_LEVEL_RRC_RESP                                   40
#define OAM_RESET_LOG_CAT_MAC_REQ                                    41
#define OAM_RESET_LOG_CAT_RLC_REQ                                    42
#define OAM_RESET_LOG_CAT_PDCP_REQ                                   43
#define OAM_RESET_LOG_CAT_EGTPU_REQ                                  44
#define OAM_RESET_LOG_CAT_L2_REQ                                     45
#define OAM_SET_ENABLE_LOG_CAT_MAC_REQ                               46
#define OAM_SET_ENABLE_LOG_CAT_RLC_REQ                               47
#define OAM_SET_ENABLE_LOG_CAT_PDCP_REQ                              48
#define OAM_SET_ENABLE_LOG_CAT_EGTPU_REQ                             49
#define OAM_SET_LOG_LEVEL_MAC_REQ                                    50
#define OAM_SET_LOG_LEVEL_RLC_REQ                                    51
#define OAM_SET_LOG_LEVEL_PDCP_REQ                                   52
#define OAM_SET_LOG_LEVEL_L2_REQ                                     53
#define OAM_SET_LOG_LEVEL_REQ                                        54

#define OAM_SET_DISABLE_LOG_CAT_MAC_REQ                              61
#define OAM_SET_DISABLE_LOG_CAT_RLC_REQ                              62
#define OAM_SET_DISABLE_LOG_CAT_PDCP_REQ                             63
#define OAM_SET_DISABLE_LOG_CAT_EGTPU_REQ                            64
#define OAM_SET_LOG_LEVEL_GTPU_REQ                                   65
#define OAM_SET_LOG_LEVEL_RRM_ENABLE_REQ                             66
#define OAM_SET_LOG_LEVEL_SON_ENABLE_REQ                             67
#define OAM_SET_LOG_LEVEL_L3_ENABLE_REQ                              68
#define OAM_SET_LOG_LEVEL_OAM_REQ                                    69
#define OAM_RRC_GET_CELL_STATS_REQ                                   72
#define OAM_RRC_GET_CELL_STATS_RESP                                  73
#define OAM_SET_ENABLE_RRM_LOG_REQ                                   74
#define OAM_SET_ENABLE_DISABLE_RRM_LOG_REQ                           75
#define OAM_SET_LOG_ENABLE_SON_REQ                                   76
#define OAM_SET_LOG_DISABLE_SON_REQ                                  77
#define OAM_SET_ENABLE_DISABLE_RRM_LOG_RESP                          78
#define OAM_SET_ENABLE_DISABLE_SON_REQ                               79
#define OAM_SET_ENABLE_DISABLE_SON_RESP                              80
#define OAM_SET_LOG_ENABLE_RRC_REQ                                   81
#define OAM_SET_LOG_ENABLE_RRC_RESP                                  82

#define OAM_CELLMGT_RECONFIG_SIB1_INTRA_FREQ_REQ                     83
#define OAM_CELLMGT_RECONFIG_SIB1_MNC_REQ                            84
#define OAM_CELLMGT_RECONFIG_SIB1_MCC_REQ                            85
#define OAM_CELLMGT_RECONFIG_SIB2_ADD_SPEC_EMI_REQ                   86
#define OAM_CELLMGT_RECONFIG_SIB2_DEF_PAG_CYL_REQ                    87
#define OAM_CELLMGT_RECONFIG_SIB2_DEF_MOD_PRD_COEFF_REQ              88
#define OAM_CELLMGT_RECONFIG_SIB2_NB_REQ                             89
#define OAM_CELLMGT_RECONFIG_SIB2_TIME_ALGN_TIMER_REQ                90
#define OAM_CELLMGT_RECONFIG_SIB1_INTRA_FREQ_RESP                    91
#define OAM_CELLMGT_RECONFIG_SIB1_MNC_RESP                           92
#define OAM_CELLMGT_RECONFIG_SIB1_MCC_RESP                           93
#define OAM_CELLMGT_RECONFIG_SIB2_ADD_SPEC_EMI_RESP                  94
#define OAM_CELLMGT_RECONFIG_SIB2_DEF_PAG_CYL_RESP                   95
#define OAM_CELLMGT_RECONFIG_SIB2_DEF_MOD_PRD_COEFF_RESP             96
#define OAM_CELLMGT_RECONFIG_SIB2_NB_RESP                            97
#define OAM_CELLMGT_RECONFIG_SIB2_TIME_ALGN_TIMER_RESP               98
#define OAM_FAP_CONFIG_RAN_BARR_EMERGENCY_REQ                        99
#define OAM_FAP_CONFIG_RAN_BARR_EMERGENCY_RESP                      100
#define OAM_FAP_CONFIG_RAN_MAC_RACH_MAXHARQ_REQ                     101
#define OAM_FAP_CONFIG_RAN_MAC_RACH_MAXHARQ_RESP                    102
#define OAM_FAP_CONFIG_RAN_MAC_RACH_MSG_PWR_OFFSET_GRP_B_REQ        103
#define OAM_FAP_CONFIG_RAN_MAC_RACH_MSG_PWR_OFFSET_GRP_B_RESP       104
#define OAM_FAP_CONFIG_RAN_MAC_RACH_PREAMBLE_INIT_RX_TGT_PWR_REQ    105
#define OAM_FAP_CONFIG_RAN_MAC_RACH_PREAMBLE_INIT_RX_TGT_PWR_RESP   106
#define OAM_FAP_CONFIG_RAN_MAC_RACH_PREAMBLE_TX_MAX_REQ             107
#define OAM_FAP_CONFIG_RAN_MAC_RACH_PREAMBLE_TX_MAX_RESP            108
#define OAM_FAP_CONFIG_RAN_RRC_TIMER_N310_REQ                       109
#define OAM_FAP_CONFIG_RAN_RRC_TIMER_N310_RESP                      110
#define OAM_FAP_CONFIG_RAN_RRC_TIMER_N311_REQ                       111
#define OAM_FAP_CONFIG_RAN_RRC_TIMER_N311_RESP                      112
#define OAM_FAP_CONFIG_RAN_RRC_TIMER_T300_REQ                       113
#define OAM_FAP_CONFIG_RAN_RRC_TIMER_T300_RESP                      114
#define OAM_FAP_CONFIG_RAN_RRC_TIMER_T301_REQ                       115
#define OAM_FAP_CONFIG_RAN_RRC_TIMER_T301_RESP                      116
#define OAM_FAP_CONFIG_RAN_RRC_TIMER_T310_REQ                       117
#define OAM_FAP_CONFIG_RAN_RRC_TIMER_T310_RESP                      118
#define OAM_FAP_CONFIG_RAN_RRC_TIMER_T311_REQ                       119
#define OAM_FAP_CONFIG_RAN_RRC_TIMER_t311_RESP                      120
#define OAM_FAP_CONFIG_RAN_MAC_RACH_PWR_RAMP_STEP_REQ               121
#define OAM_FAP_CONFIG_RAN_MAC_RACH_PWR_RAMP_STEP_RESP              122

#define OAM_SET_LOG_DISABLE_RRC_REQ                                 123
#define OAM_GET_LOG_LEVEL_GTPU_REQ                                  124
#define OAM_GET_LOG_LEVEL_GTPU_RESP                                 125
#define OAM_RRM_RECONFIG_RESP                                       126
                                                                    
#define OAM_SET_LOG_LEVEL_RESP                                      127
#define OAM_SET_LOG_LEVEL_L2_RESP                                   128

#define OAM_SET_LOG_LEVEL_OAM_RESP                                  129

#define OAM_SET_LOG_LEVEL_GTPU_RESP                                 130
/* CSR_59405_FIX_START */
#define OAM_SET_CALL_TRACE_START_RESP                               138
#define OAM_SET_CALL_TRACE_STOP_RESP                                139
/* CSR_59405_FIX_END */
#define OAM_SET_LOG_ENABLE_S1AP_REQ                                 140 
#define OAM_SET_LOG_ENABLE_S1AP_RESP                                141
#define OAM_SET_LOG_DISABLE_S1AP_REQ                                142
#define OAM_SET_LOG_ENABLE_X2AP_REQ                                 143 
#define OAM_SET_LOG_ENABLE_X2AP_RESP                                144
#define OAM_SET_LOG_DISABLE_X2AP_REQ                                145
#define OAM_SET_LOG_ENABLE_XWAP_REQ                                 146 
#define OAM_SET_LOG_ENABLE_XWAP_RESP                                147
#define OAM_SET_LOG_DISABLE_XWAP_REQ                                148


#define OAM_TIMEOUT                                                 132

#define OAM_API_VERSION_1                                             1


#define RRM_MESSAGE_API_START                   6000
#define OAM_RRM_GET_KPI_REQ                     RRM_MESSAGE_API_START+58
#define OAM_RRM_GET_KPI_RESP                    RRM_MESSAGE_API_START+59

/****************************ERROR CODES*****************************************/
#define OAM_ERR_METHOD_NOT_SUPPORTED                               9000
#define OAM_ERR_REQUEST_DENIED                                     9001
#define OAM_ERR_INTERNAL_ERROR                                     9002
#define OAM_ERR_RESOURCES_EXCEEDED                                 9004
#define OAM_ERR_INVALID_PARAM_NAME                                 9005
#define OAM_ERR_INVALID_PARAM_VALUE                                9007
/*SPR 14045 Fixed Start */
#define OAM_ERR_INVALID_PARAM_TYPE                                 9006
/*SPR 14045 Fixed End */

#define OAM_ERR_FILE_ACCESS_FAILURE                                9016
#define OAM_ERR_FILE_CORRUPTED                                     9018
#define OAM_ERR_ATTEMPT_SET_FOR_NON_WRITABLE_PARAM                 9008 
/****************************END OF ERROR CODED**********************************/

#define UINT8_TYPE                                                    1
#define UINT16_TYPE                                                   2
#define UINT32_TYPE                                                   3
#define STRING_TYPE                                                   4
#define ASCII_STRING_TYPE                                             6
#define PLMN_TYPE                                                     7
#define IPADDR_TYPE                                                   8
#define SINT8_TYPE                                                    9
#define TWO_BYTE_TYPE                                                10
#define THREE_BYTE_TYPE                                              11
#define FOUR_BYTE_TYPE                                               12
#define SINT16_TYPE                                                  13 
#define TIMESTAMP_TYPE  14
#define OAM_ENUM_TYPE  15
/* RT SPR 8154 FIX START */
#define OAM_UINT16_TOKENIZED_LIST_TYPE                               16
#define OAM_BITMAP_TOKENIZED_LIST_TYPE                               17
/* RT SPR 8154 FIX END */
/* RT SPR 9019 FIX START */
#define STRUCTURE_TYPE                                               18
/* RT SPR 9019 FIX END */
#define UINT64_TYPE                                                  19
#define EUTRAN_CELL_IDENTITY_TYPE                                    20

 /* EMBMS Changes Start */
#define OAM_BIT_STRING_TOKENIZE_TO_UINT8_ARRAY_TYPE                   21
 /* EMBMS Changes End */
#ifndef OAM_UT_FRAMEWORK 
#define LOCAL                                                        static
#else
#define LOCAL

/* SPR 15527 START */                                             
#define SINT32_TYPE  												19
/* SPR 15527 END */                                                  
#endif
/* These macros are used for sending the result field to Management Interface */
/* CSR 57877 FIx moved Code to oam_mgmt_intf_api.h */
#define OAM_API_VERSION                                               1
#define OAM_SPV_COMMITTED_NOT_APPLIED                                 1

/*TR069*/
#define TR069_MODULE_ID                                              20
#define INVALID_ARGUMENT                                             -1
#define OAM_INVALID_RETURN                                           -1
#define OAM_TR069_INTF_HEADER_SIZE                                   12
#define OAM_VERSION                                                   1
#define OAM_RRC_ENABLE_LOG                                            1
#define OAM_RRC_DISABLE_LOG                                           0
#define OAM_X2AP_ENABLE_LOG                                            1
#define OAM_S1AP_ENABLE_LOG                                            1
#define OAM_X2AP_DISABLE_LOG                                           0

#define OAM_S1AP_DISABLE_LOG                                           0
#define OAM_HEADER_SIZE                                              12
#define OAM_NULL                                                   NULL
#define OAM_ZERO                                                      0

/* SPR 23209 FIXED START */
#define OAM_MAX_PLMNLIST_OBJECTS                                      6
#define MOCN_OPERATOR_ID_START                                        0
#define MOCN_OPERATOR_ID_MAX                                          5
#define MOCN_PARTITION_LIMIT                                        100
#define QOS_SCHEDULER_STRATEGY                                        2
#define MAX_MOCN_OPERATOR                                             6
/* SPR 23209 FIXED END */


#define STD_ERROR                                                    -1
#define OAM_SIG_KILL                                            SIGKILL
/* SPR 13937 Fixed Start */
#define OAM_SIG_CHILD                                           SIGCHLD
/* SPR 13937 Fixed End */
#define OAM_TIME_SIZE_LEN                                            30
#define OAM_INVALID_POPULATE_VAL 				     -500
/*SPR 15604 START */
#define OAM_MINUS_FIVE_HUNDRED                                     -500
/*SPR 15604 END */

/*SDM*/
#define OAM_SDM_MODULE_ID                                            26
/*SPR 21889 +- */
#define OAM_SNMP_MODULE_ID					     27
#define OAM_SNMP_INTF_MODULE_ID					     28
#define OAM_AGNT_MODULE_ID                                           30

#define SDM_STOP_TIMER                                                6
#define FIRMWARE_UPGRADE_IMAGE                                        1
#define MAX_RETRY_SEND_UPGRADE_REQ                                    5

/*SPR 21889 End */
#define FORK_ERROR                                                   -1
#define FORK_CHILD                                                    0

#define PDCP_INDEX                                                    0
#define RLC_INDEX                                                     1
#define MAC_INDEX                                                     2
#define EGTPU_INDEX                                                   3
#define PHY_INDEX                                                     4
#define RRM_INDEX                                                     0
#define OAM_INDEX                                                     1
#define RRC_INDEX                                                     2
#define SON_INDEX                                                     6 /* Coverity */
#define NMM_INDEX                                                     9 
#define LTE_RRM_INDEX                                                10
#define OAM_MIN_FILE_WRITE_INTERVAL                                  5
#define OAM_MAX_FILE_WRITE_INTERVAL                                  60
#define OAM_MIN_SAMPLING_INTERVAL                                    5
#define OAM_MAX_SAMPLING_INTERVAL                                    60
#define OAM_PERIODIC_INTERVAL_FOR_HOUR                               3600
#define OAM_PERIODIC_INTERVAL_FOR_DAY                                86400
#define OAM_MAX_NUM_IP_INTRFACES                                     10
/* START RT SPR 8709 FIX */
#define OAM_PLMNID_MIN_LENGTH                                            5
#define OAM_PLMNID_MAX_LENGTH                                            6
/* END RT SPR 8709 FIX */
/*Coverity fix start : 78991 */
#define OAM_INSTANCE_ID_MAX_VAL                                      32
/*Coverity fix End : 78991 */


/* START RT SPR 8627 FIX */
#define OAM_12BIT_MAX_VALUE                                           4095
#define OAM_BYTE_SIZE_IN_BITS                                            8
#define OAM_NIBBLE_SIZE_IN_BITS                                          4
#define OAM_EXTENDED_RNCID_BITMASK                              0x80000000
#define OAM_UMTS_CID_MIN_NUM_BITS                                       12
/* END RT SPR 8627 FIX */
/* SPR 12880 Fix Start */
#define OAM_INVALID_CELL_ID                                     0xFF
/* SPR 12880 Fix End */
/* KLOCWORK warning fix */
#define OAM_OBJ_INSTANCE_NUM_STR_MAX_SIZE                            4
#define OAM_OBJ_NAME_MAX_LEN                                       255
#define OAM_ARCHIVE_FILE_NAME_MAX_SIZE                             255
#define OAM_CONFIG_FILE_LINE_MAX_SIZE                              100
#define OAM_NOTIFICATION_API_EVENT_INFO_MAX_LEN                    255

#ifdef OAM_RADIO_HEAD_SUPPORT
    #define RRH_INDEX                                                11
#endif

#define OAM                                                         200

/* Log file name and path */
#define OAM_LOG_FILE_NAME                                 "oam_logs.log"
#define OAM_LOG_FILE_PATH                                           "./"
/* SPR 21415 Fix Start */
#define OAM_RLC_STATS_LOG_FILE_NAME                       "L2_RLC_Stats.log"
/* SPR 21415 Fix End */
/* SPR 24124/SES-507 Fix Start */
#define OAM_MAC_KPI_LOG_FILE_NAME                         "MAC_KPI.log"
/* SPR 24124/SES-507 Fix End */

#ifndef OAM_EVM_TARGET
    #define OAM_LOG_MAX_SIZE                                      229587950
#else
    #define OAM_LOG_MAX_SIZE                                      52428800
#endif

#define OAM_MAX_LOG_MSG_SIZE                                       1024
#define OAM_THREAD_STACK_SIZE                                      2097152
/*SPR 21889 Fix Start */
#define OAM_THREAD_NUM                                             6
/*SPR 21889 Fix End */

/* Shared Memory Default Configuration */
#define SHM_LOG_INTERVAL                                            2
#define SHM_SIZE                                                    5
#define SHM_LOG_FILE_SIZE                                           10
#define SHM_LOG_FILE_COUNT                                          10
#define SHM_LOG_CORE                                                1
#define SHM_LOG_FILE_PATH                                           "."
/* SPR 22071 Logging Changes Start */
#define SHM_LOG_CHUNK_COUNT                                         100
/* SPR 22071 Logging Changes End */
/* SPR_17377_Warning_fix + */
/* SPR 22304 Fix Start */
    void oam_trace_dbg(
            unsigned int source,signed int log_level,
            const char* format, ...);
/* SPR 22304 Fix End */

#define OAM_TRACE_DBG(source,log_level,format, ...)   oam_trace_dbg(source,log_level, format, ## __VA_ARGS__);
#ifdef ATTRIBUTE_COMP
#define OAM_LOG OAM_TRACE_DBG
#else
/* SPR_17377_Warning_fix - */

#ifdef OAM_SHARED_MEM_LOGGER
#define OAM_LOG(facility_name, log_level, format, ...) \
        {\
            AdvL_LogTarget logTarget = LogTarget_None; \
	        AdvL_ContextIndex log_index = OAM_ZERO; \
            if(LogSuccess == check_log_level(log_index , (AdvL_LogLevel)log_level, &logTarget)) {\
                {\
                    lgr_log_level_et logLevel = LGR_TRACE; \
                    logLevel = oam_map_log_level_to_lgr_log_level_type(log_level);\
                    print_log_message((U32)logLevel, __FILE__, __LINE__, __func__,\
                        (const char*)facility_name, 0xFF, 0xFFFF, LGR_TRUE, (const char*)format, ##__VA_ARGS__); \
                }\
            }\
        } 
#else
#define OAM_LOG(facility_name, log_level, format, ...) \
    	            oam_log_message(log_level,(SInt8 *) __FILE__,(SInt8 *)__FUNCTION__,__LINE__,(SInt8 *)format,##__VA_ARGS__)
#endif

/* SPR_17377_Warning_fix + */
#endif
/* SPR_17377_Warning_fix - */


#define OAM_LOGg(source, log_priority, formate, ...) \
    if ( log_priority >= prov_req.log_level) \
    {\
        if (NULL == prov_req.p_out)\
        {\
            prov_req.p_out = oam_fopen("oam_logs.log", "w+");\
        }\
        oam_fprintf(prov_req.p_out, formate,##__VA_ARGS__);\
        oam_fflush(prov_req.p_out);\
    }

#define OAM_LOGn(source, log_priority, formate, ...) \
    if ( log_priority >= prov_req.log_level) \
    {\
        if (NULL == prov_req.p_fp)\
        {\
            prov_req.p_fp = oam_fopen("rrcstats_logs.log", "w+");\
        }\
        oam_fprintf(prov_req.p_fp, formate,##__VA_ARGS__);\
        oam_fflush(prov_req.p_fp);\
    }

#define OAM_LOGk(source, log_priority, formate, ...) \
    if ( log_priority >= prov_req.log_level) \
    {\
        if (NULL == prov_req.p_fp)\
        {\
            prov_req.p_fp = oam_fopen("kpistats_logs.log", "w+");\
        }\
        oam_fprintf(prov_req.p_fp, formate,##__VA_ARGS__);\
        oam_fflush(prov_req.p_fp);\
    }

/*
 *   UT Trace
 */
/* This will be used when debugging flag is ON.
 *  \brief Macro to be used for debugging purpose traces.
 */
#ifdef OAM_DEBUG
    #define OAM_FUN_ENTRY_TRACE()\
       OAM_LOG(OAM, OAM_DETAILEDALL, "Entering function")

    #define OAM_FUN_EXIT_TRACE()\
       OAM_LOG(OAM, OAM_DETAILEDALL, "Exiting function")
#else
    #define OAM_FUN_ENTRY_TRACE()
    #define OAM_FUN_EXIT_TRACE()
#endif /* OAM_DEBUG */ 

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))

/* LTE stack module identifiers */
#define OAM_VERSION_ID                                             0x01
#define SON_API_HEADER_SIZE             16


#define LOCAL_FM_CNTXT      local_prov_req.fm_cntxt
#define FM_CNTXT            oam_prov_req.fm_cntxt

#define OAM_ONE                                                       1
#define OAM_TWO                                                       2
#define OAM_THREE                                                     3
#define OAM_FOUR                                                      4
#define OAM_FIVE                                                      5
#define OAM_SIX                                                       6
#define OAM_SEVEN                                                     7
#define OAM_EIGHT                                                     8
#define OAM_NINE                                                      9
#define OAM_THIRTEEN                                                 13
#define OAM_FIFTEEN                                                  15
#define OAM_NINETYNINE                                               99
#define LTE_RRM_MODULE_ID                                             2
#define MAC_MODULE_ID                                                 7
#define RLC_MODULE_ID                                                 6
#define SON_MODULE_ID                                               100
#define PDCP_MODULE_ID                                                5
#define EGTPU_MODULE_ID                                               4
#define    PM_MODULE_ID                                             101
#define    OAM_LOGGER_MODULE_ID                                     102
#define MAX_IP_INTERFACES					      4
#define MAX_IPV6_ADDR_LEN					     39
#ifdef OAM_RADIO_HEAD_SUPPORT
    #define RCM_MODULE_ID                                            25
    #define OAM_RRH_LISTEN_PORT                                   10146
#endif
#define OAM_SDM_MODULE_ID                                            26
#define OAM_AGNT_MODULE_ID                                           30
/** new timer functionality defines **/
/* Time Duration is in seconds */
#define OAM_LAYERS_CONFIG_TIMEOUT_DURATION                           45
#define OAM_CELL_OPERATION_TIMEOUT_DURATION                          90
#define OAM_TR069_INIT_TIMEOUT_DURATION                              60
#define OAM_TR069_INIT_NMM_TIMEOUT_DURATION                          60

#define OAM_SUB_MODULE_MIH                                            1 /* Management Interface Handler */
#define OAM_SUB_MODULE_CM                                             2 /* Configuration Management */
#define OAM_SUB_MODULE_HM                                             3 /* Health Monitoring */
#define OAM_SUB_MODULE_PM                                             4 /* Performance Management */
#define OAM_SUB_MODULE_SDM                                            5 /* Software Download Manager */
#define OAM_SUB_MODULE_FM                                             6 /* Fault Management */
#define OAM_SUB_MODULE_SM                                             7 /* State Management */
/* SPR 9620 START */
#define OAM_SUB_MODULE_SON                                            9
#define OAM_SUB_MODULE_RRM                                            10
#define OAM_SUB_MODULE_MAC                                            11
#define OAM_SUB_MODULE_RLC                                            12
#define OAM_SUB_MODULE_PDCP                                           13
#define OAM_SUB_MODULE_RRC                                            14  
#define OAM_SUB_MODULE_AGENT                                          15  
/* SPR 9620 END */

/* Support of TR069 ADD/Delete object */
#define OAM_ADD_OPERATION                                             1
#define OAM_DELETE_OPERATION                                          2

#define OAM_MAX_INTER_FREQ_CARRIER_OBJECTS                            8

/*SPR_16048 Fix Start*/
#define OAM_MAX_CELL_OBJECTS                                          32
/*SPR_16048 Fix End*/
/* SPR 15527 START */                                             
#define OAM_MAX_QOSLIST_OBJECTS                                       9
/* SPR 15527 END */                                                  

/** Timer Types */
#define OAM_GUARD_TIMER_TYPE                                          1
#define OAM_HM_TIMER_TYPE                                             2
#define OAM_HM_NON_RESPONSIVE_TIMER_TYPE                              20
#define OAM_LAYERS_CONFIG_TIMER_TYPE                                  3
#define OAM_CELL_SETUP_TIMER_TYPE                                     4
#define OAM_CELL_BLOCK_TIMER_TYPE                                     5
#define OAM_SDM_TIMER_TYPE                                            6
#define OAM_SDM_TIMEOUT                                              10
#define OAM_TR069_INIT_TIMER_TYPE                                     7
#define OAM_TR069_INIT_NMM_TIMER_TYPE                                16
#define OAM_NTP_TIMER_TYPE                                            17
#define OAM_SYSTEM_LOG_TIMER_TYPE                                     18       
/* SPR 16567 START */
#define OAM_ENB_CONFIG_UPDATE_TIMER                                   25
/* SPR 16567 START */
#define OAM_SHUTDOWN_TIMER                                              28 /* Timer type */
#define OAM_SHUTDOWN_TIME_OUT                                           4  /* Time out value in sec */

#define OAM_AGENT_RESP_PENDING_TIMER                                    29 /* Timer type */
#define OAM_AGENT_RESP_PENDING_TM_OUT                                   20 /* Time out value in sec */
/** new timer functionality defines end **/
/*NMM_AB+*/ 
#define NMM_MODULE_ID                                                 9
/*NMM_AB-*/                                                         
#define QCLI_MODULE_ID                                               10
#define OAM_QCLI_TRAP_MODULE_ID                                      15
#define S1_MODULE_ID                                                 11
#define TRAFFIC_GEN_MODULE_ID                                       100
#define USER_MODULE_ID                                              200
#define EGTPU_PACKET_RELAY_MODULE_ID                                  4
#define QCLI_INTERFACE_MODULE_ID                                     13
#define FM_MODULE_ID                                                 16
#define QCLI_HEADER_SIZE                                             12
#define OAM_TRAP_API_ID                                             999
#define MAX_PARAMETER_NAME_LENGTH                                   128
/* SPR 17408 Fix Start */
#define OAM_USR_INTF_ERR_STR_MAX_SIZE                               200
/* SPR 17408 Fix End */
/* START RT SPR 8627 FIX */
#define PARAM_VAL_PAIR_MAX_LEN                                      128
/* END RT SPR 8627 FIX */
/* SPR 15527 START */
#define MAX_NUM_PARAMS_ALLOWED_GPV_RESP     1000
/* SPR 15527 END */
#define OAM_INFORM_API_MAX_NUM_PARAMS                               255
#define OAM_INFORM_API_PARAM_NAME_MAX_LEN                           255

#define OAM_SON_MAX_NUM_NEIGHBOR            32
#define UINT16_TYPE_2                                                 5 /* It will cater only unsigned U16 bit */

#define OAM_MGMT_INTF_MODULE_ID QCLI_INTERFACE_MODULE_ID

/* Default Addresses of External Entitites to OAM */
/* These are required by CSPL Framework to register a module with itself. */
#define OAM_IP_ADDR                                          "127.0.0.1"
#define RRM_IP_ADDR                                          "127.0.0.1"
/*NMM_AB+*/                                                  
#define NMM_IP_ADDR                                          "127.0.0.1"
/*NMM_AB-*/

#define OAM_PORT                                                  10157
#define TRAFFIC_GEN_MODULE_PORT                                    3315
#define PACKET_RELAY_OAM_PORT                                      9908
#define PACKET_RELAY_S1AP_PORT                                     5656
#define RRM_PORT                                                   3222
#define PHY_MODULE_PORT                                            3320
/*NMM_AB+*/
#define NMM_PORT                                                   5555
/*NMM_AB-*/

/* Maximum UDP Payload Size that OAM can receive */
#define OAM_MAX_MTU_SIZE                                        64*1024

#define L2_INIT_TIMER_VALUE                                       20000
#define L3_INIT_TIMER_VALUE                                       20000
/* SPR 22948 fix start */
#define OAM_USER_INTF_RESP_TIMEOUT_S                                 400
/* SPR 22948 fix stop */
#define OAM_LAYER_INTF_RESP_TIMEOUT_S      (OAM_USER_INTF_RESP_TIMEOUT_S - 1)
/* SPR_9620_CHANGES_START */
#define OAM_GUARD_TIMER_DURATION                                50*1000
/* SPR_9620_CHANGES_END */
#define OAM_TRANS_ID_LOC                                              0
#define OAM_SRC_ID_LOC                                                2
#define OAM_DEST_ID_LOC                                               4
#define OAM_API_ID_LOC                                                6
#define OAM_MSG_SIZE_LOC                                              8
#define OAM_QCLI_ID_LOC                                              10
#define OAM_MAX_TRANS_ID                                         0xFFFF
#define SON_MRO_MODIFY_CONFIG_PARAM 				"SON.MRO.MODIFY.CONFIG.PARAM"

#define LOWER_LAYER_INTERFACE_HEADER_SIZE                          16

#define OAM_CSPL_HDR_SIZE                                            16
#define OAM_SDM_UPGRADE                                               3
#define OAM_SDM_SHUTDOWN                                              4
#define OAM_CSPL_HDR_SIZE                                            16
#define OAM_MAC_INIT_IND_FLAG                                      0x01
#define OAM_MAC_INIT_CNF_FLAG                                      0x02
#define OAM_RLC_INIT_IND_FLAG                                      0x04
#define OAM_RLC_INIT_CNF_FLAG                                      0x08
#define OAM_PDCP_INIT_IND_FLAG                                     0x10

#define OAM_RRM_INIT_IND_FLAG                                      0x20
#define OAM_MODULE_ID                                                 1
#define RRM_MAX_QCI                                                   9
#define MAX_UE_PER_BATCH                                             10
                                                                   
#define STATS_IE_SIZE                                                 4
#define TAG_SIZE                                                      2
#define LENGTH_SIZE                                                   2
#define TAG_LEN_SIZE                                                  4
#define TIME_SIZE 20
#define CASE_SET_LOG_LEVEL                                            1
#define CASE_ENABLE_DISABLE_LOG_CATEGORY                              2
#define CASE_GET_LOG_LEVEL                                            3
#define CASE_GET_LOG_CATEGORY                                         4
#define CASE_RRC_LOG_ENABLE                                           5
#define CASE_RRC_CELL_STATS                                           6
#define CASE_CELL_RECONFIG                                            7
#define OAM_CASE_PDCP_CONFIG_KPI                                      8
#define OAM_CASE_PDCP_GET_KPI_STATS                                   9
#define OAM_CASE_RLC_CONFIG_KPI                                      10
#define OAM_CASE_RLC_GET_KPI_STATS                                   11
#define OAM_CASE_MAC_CONFIG_KPI                                      12
#define OAM_CASE_MAC_GET_KPI_STATS                                   13
#define CASE_CELL_DELETE                                             14
#define CASE_CELL_SETUP                                              15
#define CASE_CELL_START                                              16
#define CASE_CELL_STOP                                               17
/* CLPC Start*/                                                      
#define CASE_GET_UE_SINR_TA                                          18
/* CLPC Stop*/                                                       
#define CASE_RESET_LOG_LEVEL                                         19
#define CASE_LOG_STATUS                                              20
#define CASE_SET_LOG_CATEGORY                                        21
#define CASE_RESET_LOG_CATEGORY                                      22
#define CASE_HO_TRIGGER                                              23
#define CASE_HO_TRIGGER_TYPE                                         24


/*EGTPU MACROS*/

#define EGTPU_LM_IN_API_BASE          400
#define EGTPU_INITDB_REQ              (EGTPU_LM_IN_API_BASE)
#define EGTPU_SET_TIMER_REQ           (EGTPU_LM_IN_API_BASE + 1)
#define EGTPU_GET_TIMER_REQ           (EGTPU_LM_IN_API_BASE + 2)
#define EGTPU_GET_MAX_RETRIES_REQ     (EGTPU_LM_IN_API_BASE + 3)
#define EGTPU_SET_MAX_RETRIES_REQ     (EGTPU_LM_IN_API_BASE + 4)
#define EGTPU_GET_STATS_REQ           (EGTPU_LM_IN_API_BASE + 5)
#define EGTPU_GET_INIT_STATS_REQ      (EGTPU_LM_IN_API_BASE + 6)
#define EGTPU_INIT_TRACE_LEVEL_REQ    (EGTPU_LM_IN_API_BASE + 7)
#define EGTPU_SET_TRACE_LEVEL_REQ     (EGTPU_LM_IN_API_BASE + 8)
#define EGTPU_GET_TRACE_LEVEL_REQ     (EGTPU_LM_IN_API_BASE + 9)
#define EGTPU_LM_OUT_API_BASE         450

#define EGTPU_LM_INITDB_CNF           (EGTPU_LM_OUT_API_BASE)
#define EGTPU_LM_SET_TIMER_CNF        (EGTPU_LM_OUT_API_BASE + 1)
#define EGTPU_LM_GET_TIMER_CNF        (EGTPU_LM_OUT_API_BASE + 2)
#define EGTPU_LM_GET_MAX_RETRIES_CNF  (EGTPU_LM_OUT_API_BASE + 3)
#define EGTPU_LM_SET_MAX_RETRIES_CNF  (EGTPU_LM_OUT_API_BASE + 4)
#define EGTPU_LM_GET_STATS_CNF        (EGTPU_LM_OUT_API_BASE + 5)
#define EGTPU_LM_GET_INIT_STATS_CNF   (EGTPU_LM_OUT_API_BASE + 6)
#define EGTPU_LM_INIT_TRACE_LEVEL_CNF (EGTPU_LM_OUT_API_BASE + 7)
#define EGTPU_LM_SET_TRACE_LEVEL_CNF  (EGTPU_LM_OUT_API_BASE + 8)
#define EGTPU_LM_GET_TRACE_LEVEL_CNF  (EGTPU_LM_OUT_API_BASE + 9)
#define EGTPU_LM_GET_CTXT_INFO_CNF    (EGTPU_LM_OUT_API_BASE + 10)
#define EGTPU_LM_ALARM_NOTIFICATION   (EGTPU_LM_OUT_API_BASE + 11)
#define EGTPU_LM_MODIFY_REORDERING_IE_CNF (EGTPU_LM_OUT_API_BASE + 12)
#define EGTPU_LM_LAST_OUT_API         (EGTPU_LM_MODIFY_REORDERING_IE_CNF)

#define OAM_PROC_SUP_REQ  (EGTPU_LM_LAST_OUT_API + 1)
#define OAM_PROC_SUP_RESP (EGTPU_LM_LAST_OUT_API + 2)

#define OAM_LTE_SON_BIN_NAME          "lte_son"
#define OAM_LTE_RRM_BIN_NAME          "lte_rrm"
#define SON_INTERFACE_API_HEADER_SIZE   12
#define OAM_INTF_SRC_ID_LOC             2   

#define SON_MRO_ENABLE "SON.MRO.Enable"
#define SON_MRO_DISABLE "SON.MRO.Disable"
#define  SON_MLB_MODIFY_ATTR "SON.MLB.MODIFY"
#define SON_MRO_MODIFY_CONFIG_PARAM "SON.MRO.MODIFY.CONFIG.PARAM"
#define SON_MLB_ENABLE "SON.MLB.Enable"
#define SON_MLB_DISABLE "SON.MLB.Disable"
#define SON_MLB_MODIFY "SON.MLB.Disable"
#define SON_MLB_MODIFY_ATTRIBUTE_REQUEST "SON.MLB.Modify.Attribute_Request"

#define MAX_IP_STR_SIZE 16
/*SPR 21889 Start*/
#define MAX_IP_V6_STR_SIZE 46
/*SPR 21889 End */
#define OAM_MAX_IPV4_DIGITS 4
#define OAM_CM_FSM_NAME "OAM Configuration Manager FSM"
#define OAM_CM_FSM_GET_CURRENT_STATE(p_cm_cntxt) \
        (p_cm_cntxt->state)
#define OAM_CM_FSM_SET_STATE(p_cm_cntxt, new_state)\
          p_cm_cntxt->state = new_state;
/* OAM BCOM Code Comment Changes Start */
#ifdef OAM_SON_ENABLE
/* SPR 15527 START */                                                  
/* SPR 14651 START */
#define OAM_MAX_SUPPORTED_OBJECTS 10
/* SPR 15527 END */                                                  

/* SPR-13586 END */

/*SPR_16048 Fix Start*/
/*SPR_16390 Fix start*/
#define MIN_3GPP_RSRP             -141
#define MIN_RSRP                  -140
#define MAX_RSRP                  -44
#define MIN_RSRQ                  -240
#define MAX_RSRQ                   0
#define FACTOR_FIVE                0.5
#define MIN_3GPP_RSRQ_RANGE       -20
#define MIN_3GPP_RSSI_RANGE       -141
#define MIN_RSSI                  -110
#define MAX_RSSI                  -19
/*SPR_16390 Fix end*/
/*SPR_16048 Fix End*/
#else
#define OAM_MAX_SUPPORTED_OBJECTS 2
/* SPR 14651 END */
#endif
/* OAM BCOM Code Comment Changes End */
#define LOW_COUNT 0
#define NOT_FOUND   -1

/****************************************************************************
 **  ENUMERATIONS / STRUCTURES
 ****************************************************************************/

#define OAM_NO_OF_BITS           8
#define OAM_MSEC_IN_ONE_SEC   1000
#define OAM_MAX_RECV_BUF_SIZE 65535 

#define OAM_INTF_HDR_SRC_ID_OFFSET                       2
#define OAM_INTF_HDR_DST_ID_OFFSET                       4
#define OAM_EXT_API_ID_FROM_HEADER 6
#define OAM_INTF_HDR_MSG_SIZE_OFFSET                     8
#define OAM_EXT_RESP_STATUS_FROM_HEADER 12
#define OAM_INTF_HDR_CELL_IDX_OFFSET                    10
#define OAM_INTF_HDR_TRANS_ID_OFFSET                     0

#define MAX_LOAD_LEVEL_NUM_ENTRIES      3
#define MIN_BITRATE_COUNT               2
#define OAM_NMM_MAX_STR_SIZE          256
#define STR_TO_INT_MAP_STR_SIZE        12
#define OAM_USR_REQ_CTXT_SIZE        2048
#define MAX_CELL_ID_SIZE                4
#define INTEGRITY_CHK_CMD_SZ          200 
#define INTEGRITY_CHK_BUF_SZ           50 
#define INTEGRITY_FILE_NAME_SZ        256
#define LOAD_CONFIG_INFOSTR_SIZE      250


#define OAM_CURL_ERROR_MSG_ZIZE       200 

#define OAM_RRM_ECN_UL_MAX   0x01
#define OAM_RRM_ECN_UL_MIN   0x02
#define OAM_RRM_ECN_DL_MAX   0x04
#define OAM_RRM_ECN_DL_MIN   0x08

#define RESET_REASON_COUNT              7
#define OAM_TEN                        10
#define OAM_HUNDRED                   100
#define OAM_THOUSAND                 1000
#define OAM_TWO_THOUSAND             2000
#define OAM_SIXTEEN                    16
#define OAM_CMD_SIZE                  200
#define OAM_CP_CMD_SIZE               500
#define BKP_FILE_NAME_SIZE            500 
#define TIMESTAMP_SIZE                128
#define STR_LEN_128                   128
#define STR_LEN_255                   255
#define STR_LEN_254                   254
#define STR_LEN_512                   512
#define STR_LEN_32                     32
#define STR_LEN_80                     80
#define OAM_FOURTEEN                   14
#define OAM_TWELVE                     12
#define OAM_UPPER_EIGHT_BITS_SET   0xFF00
#define OAM_LOWER_EIGHT_BITS_SET   0x00FF
#define OAM_LOWER_FOUR_BIT_SET       0x0F
#define OAM_ALL_8_BIT_SET            0xFF
#define OAM_ALL_8_BIT_ZERO           0x00
#define OAM_ELEVEN                     11
#define OAM_TWENTY_TWO                 22
#define OAM_TWENTY                     20
#define OAM_FIFTY                      50
#define OAM_FIFTY_FIVE                 55
#define OAM_THIRTY_ONE                 31
#define OAM_FIFTY_SIX                  56  
#define OAM_MAX_GPV_INSTANCES          19  
#define OAM_PM_XML_UPLOAD_ITRVL_TMR  86400  
#define OAM_PM_XML_SAMPLNG_ITRVL_TMR  3600  
#define OAM_SIXTY                      60
/* SPR 15527 START */
#define OAM_SIXTY_FOUR                 64
#define OAM_SIXTY_FIVE                 65
/* SPR 15527 END */
#define OAM_EIGHTEEN                   18
#define OAM_SEVENTEEN                  17
#define OAM_THIRTY_TWO                 32
#define OAM_NINTEEN                    19
#define OAM_TWENTY_FOUR                24
#define STR_LEN_100                   100
#define STR_LEN_264                   264
#define STR_LEN_16                     16
#define STR_LEN_256                   256
#define STR_LEN_64                     64
#define EXT_COMM_INFO_PORT           1024
#define OAM_TWENTY_ONE                 21
#define OAM_TWENTY_SEVEN               27
#define OAM_TWENTY_EIGHT               28
#define OAM_TWENTY_NINE                29
#define OAM_THIRTY                     30
#define OAM_THIRTY_SIX                 36
#define OAM_THIRTY_EIGHT               38
#define OAM_THIRTY_NINE                39
#define OAM_THIRTY_THREE               33
#define OAM_THIRTY_FOUR                34
#define OAM_THIRTY_FIVE                35
#define OAM_THIRTY_SEVEN               37
#define OAM_TWENTY_FIVE                25
#define OAM_TWENTY_SIX                 26
#define OAM_FOURTY                     40
#define OAM_FOURTY_ONE                 41
#define OAM_FOURTY_EIGHT               48
#define OAM_TWENTY_THREE               23

/* SPR 12698 FIX START */
#define TEMP_PTR_BUFFER_SIZE           1000
/* SPR 12698 FIX END */

 /* CSR 00051709 Change Start */
#define OAM_ADD_MME                    1
#define OAM_CLOSE_MME                  2 
 /* CSR 00051709 Change End */

/*SPR 16151 Fixed Start*/
#define MAX_PARAMETER_NAME_LENGTH_VALUE                             1024
/*SPR 16151 Fixed End*/
/* SPR 21737 Start */
#define OAM_GET_SIZEOF_ARRAY(X_ARG)        (sizeof(X_ARG)/sizeof(X_ARG[0]))
/* SPR 21737 End */
    /* RT SPR 9019 FIX START */
/* SPR 12929 FIX START */
#define OAM_MAX_NUM_UTRA_FDD_CARRIERS                                   16
#define OAM_MAX_NUM_UTRA_TDD_CARRIERS                                   16
#define OAM_MAX_NUM_GERAN_CARRIERS                                      16
/* SPR 12929 FIX END */
#ifdef OAM_SON_ENABLE
#define LOCAL_SON_PROV_REQ    local_prov_req.oam_son_req
#define SON_PROV_REQ          oam_prov_req.oam_son_req

#define LOCAL_SON_NEIGHBORLISTINUSE_INFO(i) \
                                LOCAL_SON_PROV_REQ.neigh_list_in_use[i]
                                

#define SON_NEIGHBORLISTINUSE_INFO(i)    \
                                SON_PROV_REQ.neigh_list_in_use[i]   


#define LOCAL_SON_NEIGHBORLIST_INFO(i)  \
                             LOCAL_SON_PROV_REQ.cell_neighbourlist_info[i]

#define SON_NEIGHBORLIST_INFO(i)   SON_PROV_REQ.cell_neighbourlist_info[i]

#define LOCAL_SON_NR_LIST(i)        LOCAL_SON_NEIGHBORLIST_INFO(i).\
                                       oam_son_nr_add_req.nr_list

#define SON_NR_LIST(i)    SON_NEIGHBORLIST_INFO(i).oam_son_nr_add_req.nr_list

#define NRT_INFO(i)              SON_PROV_REQ.neigh_list_in_use[i].nrt_info
#define LOCAL_NRT_INFO(i)        LOCAL_SON_PROV_REQ.neigh_list_in_use[i].nrt_info

#define LOCAL_SON_INTER_FREQ_CARR_PARAMS(i) LOCAL_SON_PROV_REQ.\
                                            oam_son_inter_freq_carrier_params[i]

#define SON_INTER_FREQ_CARR_PARAMS(i) SON_PROV_REQ.oam_son_inter_freq_carrier_params[i]

#define SON_ANR_MEAS_CONFIG_REQ(i) \
                    SON_INTER_FREQ_CARR_PARAMS(i).oam_son_anr_meas_config_req

#define LOCAL_SON_ANR_MEAS_CONFIG_REQ(i) \
                    LOCAL_SON_INTER_FREQ_CARR_PARAMS(i).oam_son_anr_meas_config_req

#define LOCAL_SON_NR_ADD_REQ(i)   \
                         LOCAL_SON_NEIGHBORLIST_INFO(i).oam_son_nr_add_req

#define SON_NR_ADD_REQ(i)        SON_NEIGHBORLIST_INFO(i).oam_son_nr_add_req

/*SPR 21702 Start*/
#define SON_CELL_INFO_IND_SERVING_CELL(i) SON_PROV_REQ.oam_son_cell_info_ind[i].\
						serving_cell[OAM_ZERO]
/*SPR 21702 End*/

#define TRGT_CGI_LIST oam_son_nr_delete_req->trgt_cgi_list[0]
#define SON_CELL_NEIGHLIST_INFO(i)  SON_PROV_REQ.cell_neighbourlist_info[i]

#define LOCAL_SON_CELL_NEIGHLIST_INFO(i)  \
                        LOCAL_SON_PROV_REQ.cell_neighbourlist_info[i]
#endif

#define RRM_PROV_REQ            oam_prov_req.oam_rrm_req
#define RRM_CELL_CONFIG_PARAMS  RRM_PROV_REQ.oam_rrm_cell_config_req

#define LOCAL_RRM_PROV_REQ  local_prov_req.oam_rrm_req
    /* RT SPR 9019 FIX END */

#define OAM_MAX_CELL_PRESENT_MULTICELL	    3

/* Carrier Aggregation changes start */
#define RRC_PROV_REQ   oam_prov_req.oam_l3_req.oam_rrc_req.rrc_oam_prov
#define LOCAL_RRC_PROV_REQ local_prov_req.oam_l3_req.oam_rrc_req.rrc_oam_prov
#define X2AP_PROV_REQ  oam_prov_req.oam_l3_req.oam_x2ap_req.x2ap_oam_prov

/* SPR-13586 START */
#define OAM_GSM_CELL_IDENTITY_LAC_START_OFFSET                 0
#define OAM_GSM_CELL_IDENTITY_CI_START_OFFSET                  2
#define OAM_GSM_CELL_BSIC_BSCC_MASK                          0x7
#define OAM_GSM_CELL_BSIC_NCC_MASK                          0x38
#define OAM_GSM_CELL_BSIC_NUM_BSCC_BITS                        3
#define OAM_GSM_CELL_BSIC_NUM_NCC_BITS                         3
/* SPR-13586 END */
/* Spr 15612 Fix Start */
#define OAM_PCPICH_TX_POWER_DEFAULT_VALUE                      1
#define OAM_URA_DEFAULT_VALUE                                  1
/* Spr 15612 Fix End */


#define OAM_MIN_COMMANDLINE_ARGS                3
#define OAM_MAX_COMMANDLINE_ARGS                4
#define NON_TR196_XML_ARG_INDEX                 1
#define MANDATORY_TR196_XML_ARG_INDEX           2
#define OAM_CFG_COMMENT_START_CHAR              '#'
#define OAM_CFG_NEW_LINE_CHAR                   '\n'
#define OAM_L3_MIN_UECC_INSTANCES               1
#define OAM_L3_MAX_UECC_INSTANCES               8
/* SPR 20908 Start */
/* code removed */
/* SPR 20908 End */                          
#define OAM_S1AP_INFO            oam_prov_req.oam_l3_req.oam_s1ap_req.\
                                 s1ap_oam_prov.s1apInfo

#define LOCAL_OAM_S1AP_INFO      local_prov_req.oam_l3_req.oam_s1ap_req.\
                                 s1ap_oam_prov.s1apInfo

#define L3_PROV_REQ              oam_prov_req.oam_l3_req
#define OAM_MAX_NUM_PEER_ENB                    16
#define OAM_LOCALHOST_IP4_ADDR                  "127.0.0.1" 
#define OAM_MACRO_ENB_TYPE                      0
#define OAM_HOME_ENB_TYPE                       1
#define OAM_S1AP_CONFIG_TIMER_MIN_VAL          50
#define OAM_S1AP_RESET_EP_TIMER_MIN_VAL        50
#define OAM_S1AP_PWS_TIMER_MIN_VAL             50
#define OAM_S1AP_KILL_TIMER_MIN_VAL            50
#define OAM_S1_CONN_RECOV_DURATION_MIN_VAL     50
#define OAM_S1CONNMODE_MAX_LEN                  8
/* SPR 20908 Start */
#define OAM_MAX_NUM_L2_INSTANCE                 MAX_NUM_L2_INSTANCE
/* SPR 20908 End */
#define OAM_MAX_NUM_CELLS_PER_L2_INSTANCE       2
#define OAM_SCELL_DEACT_TIMER_MAP_MAX_LEN       8
#define OAM_MEAS_CYCLE_SCELL_LEN                7 
/*SPR 21889 +- */
/*SPR 21321 Start*/
#define OAM_MAX_CELL_SUPPORTED                  6
/*SPR 21321 End*/
#define OAM_MAX_MAC_ROHC_PROFILE                4
/*SPR 21889 Start*/
#define L2_INSTANCE_INFO(i)                     oam_prov_req.l2_setup_info.\
                                                instance_info[i].l2_cell_info
#define L2_MAC_INIT_CELL_REQ(i, j)              oam_prov_req.l2_setup_info.instance_info[i].\
                                                oam_l2_req.oam_mac_req.\
                                                mac_init_cell_req[j]
#define L2_MAC_INIT_LAYER_REQ(i)                oam_prov_req.l2_setup_info.instance_info[i].\
                                                oam_l2_req.oam_mac_req.mac_init_req
#define L2_INSTANCE_CELL_INFO(i, j)             oam_prov_req.l2_setup_info.\
                                                instance_info[i].l2_cell_info.cell_info[j]
/*SPR 21889 End */

#define	OWN_CELL_CGI(i)             RRM_PROV_REQ.oam_rrm_cell_config_req[i].\
                                     global_cell_info.eutran_global_cell_id

#define	RRM_CELL_CONFIG(i)          RRM_PROV_REQ.oam_rrm_cell_config_req[i]
#define	LOCAL_RRM_CELL_CONFIG(i)    LOCAL_RRM_PROV_REQ.oam_rrm_cell_config_req[i]
/* Carrier Aggregation changes end */


#define OAM_XWAP_INFO                          oam_prov_req.oam_l3_req.oam_xwap_req.\
                                                   xwap_oam_prov.xwapInfo



#define LOCAL_OAM_XWAP_INFO                    local_prov_req.oam_l3_req.oam_xwap_req.\
                                                   xwap_oam_prov.xwapInfo

/*SPR 21889 Start*/
#define OAM_MAC_RLC_NICK_NAME                               "MAC_RLC"
#define OAM_L2_NICK_NAME                                    "L2"
#define OAM_PDCP_SPLIT_NICK_NAME                            "PDCP_SPLIT"
#define OAM_EGTPU_NICK_NAME                                 "eGTPU"
#define OAM_TR069_NICK_NAME                                 "TR069"
#define OAM_QCLI_NICK_NAME                                  "CLI"
#define OAM_RRM_NICK_NAME                                   "RRM"
#define OAM_SON_NICK_NAME                                   "SON"
#define OAM_L3_NICK_NAME                                    "L3"
#define OAM_AGENT_NICK_NAME                                 "AGENT"
/*SPR 21889 End */

#define OAM_L2_BIN_NAME                                     "lteLayer2"

#define OAM_PDCP_BIN_NAME                                   "lteLayer2_pdcp_split"
/*SPR 21321 Start*/
/*SPR 21889 Start*/
#define OAM_EGTPU_BIN_NAME                                  "lte_gtpu"
#define OAM_LIBBINREAD_BIN_NAME                             "ltebinread"
#define OAM_SDM_BIN_NAME                                    "lte_oamSDM"
/*SPR 21889 End */
/*SPR 21321 End*/
#define OAM_TR069_BIN_NAME                                  "lte_tr069"
#define OAM_QCLI_BIN_NAME                                  "lte_oamCli"
#define OAM_RRM_BIN_NAME                                      "lte_rrm"
#define OAM_SON_BIN_NAME                                      "lte_son"
#define OAM_L3_BIN_NAME                                    "rrc_entity"
#define OAM_RRH_BIN_NAME                                          "rrm"
/*SPR 21889 Start*/
#define OAM_AGENT_BIN_NAME                                 "lte_oamAgnt"
#define OAM_BIN_NAME                                       "lte_oam"
#define OAM_BIN_SOFT_UPGRADE_EXT_STR                       ".bin_updt_img"
#define OAM_BIN_FILE_PERMISSION                             (S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH|S_IXOTH)
#define OAM_BIN_FILE_TRFN_PERMISSION                        (0755)
/*SPR 21889 End */
/*SPR 21889 +- */

/*SPR 15604 START */
#define OAM_MAX_LTE_NBR_CELL_NUM_ENTRIES                         32
#define OAM_MAX_UMTS_NBR_CELL_NUM_ENTRIES                        32
/* spr_21013_fix_start */
#define NLIU_LTECell_PATH "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborListInUse.LTECell"
#define NLIU_UMTS_PATH "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborListInUse.InterRATCell.UMTS"
/* spr_21013_fix_end */
/*SPR 15604 END*/

/*SPR 21889 +- */

#define OAM_MAX_ALLOWED_CID_VALUE                                           7
#define OAM_ERROR_RETURN_VAL                                               -1
#define OAM_ENB_CONFIG_FILE_NAME            "../cfg/eNodeB_Configuration.cfg"
#define OAM_TR196_XML_FILENAME               "eNodeB_Data_Model_TR_196_based"
#define OAM_PROPRIETARY_XML_FILENAME          "Proprietary_eNodeB_Data_Model"
#define OAM_TR196_XML_FILENAME_WITH_EXT  "eNodeB_Data_Model_TR_196_based.xml"
#define OAM_PROPRIETARY_XML_FILENAME_WITH_EXT "Proprietary_eNodeB_Data_Model.xml"

#define EUTRAN_CID_BYTE_OFFSET                                      3
#define OAM_MAX_EUTRAN_CELL_IDENTITY_VALUE                  0xfffffff
#define OAM_SINGLE_CELL_SETUP                                       1
#define OAM_TR069_MODE_ON                                           1

#define OAM_CELL_EV_ONGOING(i)               oam_prov_req.cell_cntxt.\
                                       cell_ctxt_status[i].cell_event

#define OAM_CELL_CNTXT_STATUS(i)               oam_prov_req.cell_cntxt.\
                                               cell_ctxt_status[i]

#define TIME_UNIT_TO_MILLISEC          	1000
#define MAC_KPI_BUFFER_LEN             	 200
#define OAM_PERCENTAGE_CONVERSION_VAL           100

#define OAM_CELL_CURR_STATE(i)               oam_prov_req.cell_cntxt.\
                                       cell_ctxt_status[i].cell_state

#define OAM_SYS_EVENT_BIMASK           oam_prov_req.system_status.curr_event
#define OAM_SYS_STATE_BIMASK           oam_prov_req.system_status.curr_state


#define LAYER_INIT_FAILED_STR(name)      name " layer initialization failed"
#define OAM_INTF_MSG_PARSING_FAILED_STR(name)  name " Message parsing failed"

#define OAM_ALARM_PERCEIVED_SEV_MAX_LEN    256
#define OAM_ALARM_REPORTING_MECH_MAX_LEN   256
#define OAM_ALARM_NOTIF_TYPE_MAX_LEN       256
#define OAM_ALARM_ADD_TXT_MAX_LEN          256
#define MAX_PARENT_NAME_LEN                255
#define OAM_ALARM_ADD_INFO_MAX_LEN         256
#define OAM_ALARM_PROB_CAUSE_STR_MAX_LEN   256
#define OAM_ALARM_TIMESTAMP_STR_MAX_LEN     50
#define OAM_ALARM_EVENT_TYPE_MAX_LEN       256
#define SUPP_ALARM_LIST(i)      oam_prov_req.fm_cntxt.supported_alarm_list[i]
#define EXPD_ALARM_LIST(i)      oam_prov_req.fm_cntxt.expedited_alarm_list[i]
#define QUEUED_ALARM_LIST(i)    oam_prov_req.fm_cntxt.queued_alarm_list[i]
#define OAM_WORD_SIZE_IN_BYTE                2
#define OAM_DWORD_SIZE_IN_BYTE               4


/* SPR 11001 FIX START */
#define OAM_RRM_MIN_LOAD_PRCNT                                1
#define OAM_RRM_MAX_LOAD_PRCNT                              100
#define OAM_RRM_MIN_LOAD_NUM_USRS                             1
#define OAM_RRM_MAX_LOAD_NUM_USRS                           255
#define OAM_RRM_MIN_LOAD_Q_WATERMARK                          1
#define OAM_RRM_MAX_LOAD_Q_WATERMARK                        100
#define OAM_RRM_MIN_LOAD_AC_BARRING_FOR_SPECIAL_AC            0
#define OAM_RRM_MAX_LOAD_AC_BARRING_FOR_SPECIAL_AC          248
#define OAM_RRM_MIN_LOAD_REDIRECT_MEAS_BASED_RWR              0
#define OAM_RRM_MAX_LOAD_REDIRECT_MEAS_BASED_RWR              1
#define OAM_RRM_MIN_LOAD_REDIRECT_RWR_TIMER                   5
#define OAM_RRM_MAX_LOAD_REDIRECT_RWR_TIMER                 120
#define OAM_RRM_MIN_LOAD_STOP_ADM_INFO_SOFT_LIMIT             5
#define OAM_RRM_MAX_LOAD_STOP_ADM_INFO_SOFT_LIMIT            10
#define OAM_RRM_MIN_ECN_CAP_NUM_UE                           -1
#define OAM_RRM_MAX_ECN_CAP_NUM_UE                          100
/* SPR 11001 FIX END */

/*eICIC_PHASE_1_2_CHANGES_START */ 
#define OAM_RRM_eICIC_MIN_ELEGIBLE_UE                         1
#define OAM_RRM_eICIC_MAX_ELEGIBLE_UE                       100
#define OAM_RRM_MAX_ABS_PATTERN_BITS_FDD                     40
#define OAM_RRM_eICIC_MIN_ABS_PATTERN_LENGTH                  1 
#define OAM_RRM_eICIC_MAX_ABS_PATTERN_LENGTH                 70
#define OAM_RRM_MAX_ABS_PATTERN_SIZE_TDD                      9
#define OAM_RRM_MAX_ABS_PATTERN_SIZE_FDD                      5
/*eICIC_PHASE_1_2_CHANGES_END*/ 

#define OAM_TAG_VALUE_STR_MAX_LEN                          1024
#define OAM_RRM_GSM_BAND_IND_TBL_NUM_ROWS                     2
#define OAM_SON_GSM_BANDINDICATOR_TBL_NUM_ROWS                 2
#define OAM_MAX_USR_INTF_MSG_BUF_SIZE                      2048
#define OAM_MAX_CELL_SPEC_ACC_MGMT_PARAMS                     5


#define OAM_EUTRA_CARRIER_ARFCN_DL_LIST_MAX_LEN              64
#define OAM_PHY_CELL_ID_STR_MAX_LEN                         512
/* SPR 13024 FIX START */
#define OAM_MAX_PHY_CELL_ID_TOKEN                           504
/* SPR 13024 FIX END */

/* max length of any IPv4 or IPv6 address */
#define OAM_IP_ADDR_MAX_LEN                                  45
/* SPR 12377 FIX END */


/* CSR 58157 START */
#define EARFCN_LIST_MAX_LENGTH                              129
#define OAM_EARFCN_LIST_MAX_NUM_ITEMS                        64
/* CSR 58157 END */

#define OAM_CFG_DIR_PATH_RELATIVE_TO_BIN_DIR           "../cfg/"


#define OAM_PARAM_VAL_ENUM_MAP_TBL_MAX_COL                   2


#define OAM_ANR_ACTION_DELETE_INTER_FREQ_CARRIER_ONGOING              0x1
#define OAM_ANR_ACTION_DELETE_UTRAN_FDD_FREQ_ONGOING                  0x2
#define OAM_ANR_ACTION_DELETE_GERAN_FREQ_GROUP_ONGOING                0x4
/* OAM BCOM Code Comment Changes Start */
#define OAM_ANR_ACTION_DELETE_INTER_FREQ_CARRIER_ONGOING_AT_SON       0x8
#define OAM_ANR_ACTION_DELETE_UTRAN_FDD_CARRIER_ONGOING_AT_SON        0x10
#define OAM_ANR_ACTION_DELETE_GERAN_FREQ_CARRIER_ONGOING_AT_SON       0x20
/* OAM BCOM Code Comment Changes End */

/* SPR 13380 REIVEW COMMENTS FIX START */
#define OAM_NUM_SUPP_EUTRA_REPORT_AMT                                   8
#define OAM_2X2_TBL_NUM_COULMNS                                         2
/* SPR 13380 REIVEW COMMENTS FIX END */

/* SPR 14153 FIX START */
#define PLMNLIST_ENABLE_DEF_VAL_STR                                       "0"
#define PLMNLIST_PLMNID_DEF_VAL_STR                                  "111111"
#define PLMNLIST_IS_PRIMARY_DEF_VAL_STR                                   "0"
#define PLMNLIST_CELL_RESV_FOR_OP_USE_DEF_VAL_STR                         "0"

#define CARRIER_EUTRA_CARRIER_ARFCN_DEF_VAL_STR                           "2"
#define CARRIER_MEAS_BANDWIDTH_FOR_EARFCN_DEF_VAL_STR                   "BW6"
#define CARRIER_PRESENCE_ANTENNA_PORT1_DEF_VAL_STR                        "0"
#define CARRIER_NEIGHBOR_CELL_CONFIG_DEF_VAL_STR                          "0"
#define CARRIER_QOFFSET_FREQ_DEF_VAL_STR                                  "1"
#define CARRIER_QRXLEVMIN_SIB5_DEF_VAL_STR                              "-70"
#define CARRIER_PMAX_DEF_VAL_STR                                        "-30"
#define CARRIER_TRESELECTION_EUTRA_DEF_VAL_STR                            "0"
#define CARRIER_TRESELECTION_EUTRA_SFMEDIUM_DEF_VAL_STR                  "25"
#define CARRIER_TRESELECTION_EUTRA_SFHIGH_DEF_VAL_STR                    "25"
#define CARRIER_THRESHX_HIGH_DEF_VAL_STR                                  "0"
#define CARRIER_THRESHX_LOW_DEF_VAL_STR                                   "0"
#define CARRIER_CELL_RESELECTION_PRIORITY_DEF_VAL_STR                     "7"
#define CARRIER_THRESHX_HIGHQ_R9_DEF_VAL_STR                              "0"
#define CARRIER_CM_OFFSET_FREQ_DEF_VAL_STR                                "0"
#define CARRIER_THRESHX_LOWQ_R9_DEF_VAL_STR                               "0"

#define UTRAN_FDD_FREQ_UTRA_CARRIER_ARFCN_DEF_VAL_STR                     "1"
#define UTRAN_FDD_FREQ_QRXLEVMIN_DEF_VAL_STR                            "-20"
#define UTRAN_FDD_FREQ_QQUAL_MIN_DEF_VAL_STR                             "-5"
#define UTRAN_FDD_FREQ_CELL_RESELECTION_PRIORITY_DEF_VAL_STR              "7"
#define UTRAN_FDD_FREQ_THRESHX_HIGH_DEF_VAL_STR                           "0"
#define UTRAN_FDD_FREQ_THRESHX_LOW_DEF_VAL_STR                            "0"
#define UTRAN_FDD_FREQ_PMAX_UTRA_DEF_VAL_STR                              "1"
#define UTRAN_FDD_FREQ_THRESHX_HIGHQ_R9_DEF_VAL_STR                       "0"
#define UTRAN_FDD_FREQ_THRESHX_LOWQ_R9_DEF_VAL_STR                        "0"

#define GERAN_FREQ_GROUP_BAND_INDICATOR_DEF_VAL_STR                 "DCS1800"
#define GERAN_FREQ_GROUP_BCCH_ARFCN_DEF_VAL_STR                         "800"
#define GERAN_FREQ_GROUP_NCC_PERMITTED_DEF_VAL_STR                        "1"
#define GERAN_FREQ_GROUP_QRXLEV_MIN_DEF_VAL_STR                           "0"
#define GERAN_FREQ_GROUP_CELL_RESELECTION_PRIORITY_DEF_VAL_STR            "7"
#define GERAN_FREQ_GROUP_THRESHX_HIGH_DEF_VAL_STR                         "0"
#define GERAN_FREQ_GROUP_THRESHX_LOW_DEF_VAL_STR                          "0"
#define GERAN_FREQ_GROUP_OFFSET_FREQ_DEF_VAL_STR                          "0"

#define LTE_CELL_PLMNID_DEF_VAL_STR                                  "111111"
#define LTE_CELL_CID_DEF_VAL_STR                                          "3"
#define LTE_CELL_EUTRA_CARRIER_ARFCN_DEF_VAL_STR                          "2"
#define LTE_CELL_PHY_CELL_ID_DEF_VAL_STR                                  "1"
#define LTE_CELL_BLACKLISTED_DEF_VAL_STR                                  "0"
#define LTE_CELL_NR_STATUS_DEF_VAL_STR                                    "1"

#define UMTS_PLMNID_DEF_VAL_STR                                      "111111"
#define UMTS_RNCID_DEF_VAL_STR                                            "1"
#define UMTS_CID_DEF_VAL_STR                                              "1"
#define UMTS_NR_STATUS_DEF_VAL_STR                                        "1"
#define UMTS_HO_STATUS_DEF_VAL_STR                                        "1"
#define UMTS_UARFCNDL_DEF_VAL_STR                                         "1"
#define UMTS_ACCESS_MODE_DEF_VAL_STR                                      "2"

#define GSM_PLMNID_DEF_VAL_STR                                       "111111"
#define GSM_LAC_DEF_VAL_STR                                               "1"
#define GSM_CI_DEF_VAL_STR                                                "0"
#define GSM_BAND_INDICATOR_DEF_VAL_STR                              "DCS1800"
#define GSM_BCCH_ARFCN_DEF_VAL_STR                                      "800"
#define GSM_BSIC_DEF_VAL_STR                                              "0"
#define GSM_NR_STATUS_DEF_VAL_STR                                         "1"
#define GSM_HO_STATUS_DEF_VAL_STR                                         "1"
/* SPR 14153 FIX END */
/* Spr 15612 Fix Start */
#define OAM_MAX_LTE_NBR_CELL_NUM_ENTRIES                         32
#define OAM_MAX_UMTS_NBR_CELL_NUM_ENTRIES                        32
/* spr_21013_fix_start */
#define NLIU_LTECell_PATH "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborListInUse.LTECell"
#define NLIU_UMTS_PATH "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborListInUse.InterRATCell.UMTS"
/* spr_21013_fix_end */
/* Spr 15612 Fix End */
/* eMTC changes start */
#define OAM_DEFAULT_TWO_BITS_SET            0xFFC0
#define OAM_DEFAULT_SF_PATTERN_LSB_2BITS    0xC0
/* eMTC changes stop */

#ifdef ENDC_ENABLED
/* MENB CHANGES - START */
#define OAM_MAX_NUM_PEER_GNB                    16
/* MENB CHANGES - END */
#endif

#endif /* OAM_DEFINES_H */

