/*******************************************************************************
* File Name   : oam_qcli_proto.h
*
* Description : This file contains User Defined Macros and enums used in
*                other files(oam_qcli_port.c).

* Revision History:
*
* Date         Author           Reference          Comments
* -----        ------           ---------          --------
* June,2012    Vivek Kumar Dixit    OAM Design Document    Initial Version
* April 2014   Vinod Vyas         CSR 57877 Fix  
* June 2014     Shalu SAgar         SPR 11266 FIX
* Aug 2014      Aditi Singh         SPR 13116
* Copyright (c) 2012, Aricent Inc. All Rights Reserved
*******************************************************************************/
/********************************************************************************
Exported Types
******************************************************************************/
#ifndef __OAM_QCLI_PROTO_H__
#define __OAM_QCLI_PROTO_H__

#include<oam_types.h>
/* CSR 57877 Fix START */
#include<oam_mgmt_intf_api.h>
/* CSR 57877 Fix End*/
#include "qmacro.h"
#include "qutil.h"
#include "lteLogger.h"
/* SPR Fix 17928 Start */
#define qcliHandleUnusedParameter(parameter) \
                (void)parameter
/* SPR Fix 17928 End */
/*******************************************************************************
* Includes
*******************************************************************************/
#define MAX_WARNING_PARAMETER          34

#define CELLMGMT_DELETE                1
#define CELLMGMT_RECONFIG              2
#define CELLMGMT_SETUP                 3
#define CELLMGMT_START                 4
#define CELLMGMT_STOP                  5
#define RESET_STATS_ALL                13
#define RESET_STATS_L2_ALL             14
#define RESET_STATS_L2_MAC             15
#define RESET_STATS_L2_RLC             16
#define RESET_STATS_L2_PDCP            17
#define RESET_STATS_L3_ALL             18
#define RESET_STATS_L3_RRC             19
#define RESET_STATS_L3_S1AP            20
#define SET_LOG_LEVEL_ALL              25
#define SET_LOG_LEVEL_L2_ALL           26
#define SET_SON_ANR_ENABLE             33
#define SET_SON_ANR_DISABLE            34
#define SET_SON_ES_ENABLE              35
#define SET_SON_ES_DISABLE             36
#define SET_SON_ES_CELL_SWITCH_ON      37
#define SET_SON_ES_CELL_SWITCH_OFF     38
#define SET_SON_ES_UE_COUNT_INTERVAL   39
#define SET_SON_ES_UE_COUNT_THRESHOLD  40
#define SET_SON_SCAN_NW                41
#define SET_SON_SCAN_NR                42
#define SET_SON_NR_UPDATE              43
#define SET_SON_NR_ADD                 44
#define SET_SON_NR_DELETE              45
#define SET_SON_NR_RETRIEVE            46
#define SHOW_BUILDVER                  47
#define SHOW_SON_SWITCHEDOFFCELLINFO   59
#define SHOW_STATS_ALL                 60
#define SHOW_STATS_L2_ALL              61
#define SHOW_STATS_L2_RLC              63
#define SHOW_STATUS_ALL                72
#define SHOW_STATUS_L2_ALL             73
#define SHOW_STATUS_L2_MAC             74
#define SHOW_STATUS_L2_PDCP            75
#define SHOW_STATUS_L2_RLC             76
#define SHOW_STATUS_L3_ALL             77
#define SHOW_STATUS_L3_RRC_CELL        78
#define SHOW_STATUS_L3_RRC_UE          79
#define SHOW_UESINRTA_ALL              80
#define SHOW_UESINRTA                  81
#define SET_LOG_CAT_DISABLE_MAC        89
#define SET_LOG_LEVEL_L3_RRC           94
#define OAM_MAX_NUMBER_ELEMENT         500     
#define MAX_CACHE_LIMIT                 20
#define OAM_CLI_DEFAULT_PARAM_VALUE "-"
#define OAM_BIT_ZERO                   0
#define OAM_BIT_ONE                    1
#define OAM_VERSION                  1
#define OAM_CONST_ZERO                 0
#define OAM_CONST_ONE                  1
#define OAM_CONST_TWO                  2
#define OAM_CONST_THREE                3
#define OAM_CONST_FOUR                 4
#define OAM_CONST_FIVE                 5
#define OAM_CONST_SIX                  6
#define OAM_CONST_SEVEN                7
#define OAM_CONST_EIGHT                8
#define OAM_CONST_NINE                 9
#define OAM_CONST_TEN                  10
#define OAM_CONST_ELEVEN               11
#define OAM_CONST_TWELVE               12
#define OAM_CONST_THIRTEEN             13
#define OAM_CONST_FOURTEEN             14
#define OAM_CONST_FIFTEEN              15

#define OAM_BYTESIZE                   8
#define OAM_MAX_TRANS_ID         0xFFFF
#define OAM_V_FPRINTF                 vfprintf
#define OAM_FREE                      free
#define OAM_GET_TIME_OF_DAY           gettimeofday
#define OAM_ABORT                     abort
#define OAM_BIND                      bind
#define OAM_SOCKET                    socket
#define OAM_CLOSE                     close
#define OAM_SELECT                    select
#define OAM_RECV_FROM                 recvfrom
#define OAM_SEND_TO                   sendto
#define OAM_NULL                       NULL
#define OAM_ATOI		       atoi
#define ID                             1
#define SRC_ID                         2
#define OAM_ID                         6
#define TRANS_ID                       1
#define OAM_NEG_ONE                   -1
#define OAM_ZERO                       0
#define PNULL                         NULL
#define OAM_QCLI_HDR_SIZE    12
#define SH_HDRSIZE 16
#define MAX_RX_DATA_BUFF_SIZE 11000
#define MAX_IP_STR_SIZE 16
#define MAX_PORT_LENGTH            6
#define E_NOXMLPARSE    0x26      /* Error(s) during parsing xml message */
#define XML_STRUCT_INIT 0
#define OAM_STRCMP                   strcmp
/* SPR 11266 FIX START */
#define OAM_SON_ANR_MEAS_CONFIG_MAX_PARAM    25
#define ARFCN_SPACING_INDEX                  10
#define NUM_FOLLOWING_ARFCNS_INDEX           11
#define EXPLICIT_ARFNC_LIST_INDEX            12
#define ARFCN_BITMAP_INDEX                   13
/* SPR 11266 FIX END */
#define MAX_PARAM_VALUE_SIZE   100 

/* EMBMS Changes Start 
Macro moved from oam_qcli_cmd_tree.c */
/* SPR 1251 FIX START */
#define CLI_QSLALLOC(L){\
	L = qslalloc(); \
	if(L == 0) { \
		oam_fprintf(stderr, "\n---------------------------------------------------------------"); \
		oam_fprintf(stderr, "\nMemory allocation failed. CLI will be aborted!"); \
		oam_fprintf(stderr, "\n---------------------------------------------------------------\n"); \
		oam_cli_abort();\
	}\
}
/* SPR 1251 FIX END */
/* EMBMS Changes Start */

typedef struct sockaddr_in oam_sockaddr_t;
typedef struct in_addr oam_in_addr_t;
typedef struct timeval oam_timeval;

typedef enum 
{
 OAM_CLI_SET = 0,
 OAM_CLI_GET
}cmd_type_et;

typedef enum
{
  RRMC_POWER_RAMP_STEP_DB0,
  RRMC_POWER_RAMP_STEP_DB2,
  RRMC_POWER_RAMP_STEP_DB4,
  RRMC_POWER_RAMP_STEP_DB6
}rrmc_power_ramping_step_et;

typedef enum
{
  RRMC_GROUP_B_MSG_POWER_OFFSET_MINUSINFINITY,
  RRMC_GROUP_B_MSG_POWER_OFFSET_DB0,
  RRMC_GROUP_B_MSG_POWER_OFFSET_DB5,
  RRMC_GROUP_B_MSG_POWER_OFFSET_DB8,
  RRMC_GROUP_B_MSG_POWER_OFFSET_DB10,
  RRMC_GROUP_B_MSG_POWER_OFFSET_DB12,
  RRMC_GROUP_B_MSG_POWER_OFFSET_DB15,
  RRMC_GROUP_B_MSG_POWER_OFFSET_DB18
}rrmc_group_b_msg_power_offset_et;

typedef enum
{
  RRMC_PREAMBLE_POWER_DBM_120,
  RRMC_PREAMBLE_POWER_DBM_118,
  RRMC_PREAMBLE_POWER_DBM_116,
  RRMC_PREAMBLE_POWER_DBM_114,
  RRMC_PREAMBLE_POWER_DBM_112,
  RRMC_PREAMBLE_POWER_DBM_110,
  RRMC_PREAMBLE_POWER_DBM_108,
  RRMC_PREAMBLE_POWER_DBM_106,
  RRMC_PREAMBLE_POWER_DBM_104,
  RRMC_PREAMBLE_POWER_DBM_102,
  RRMC_PREAMBLE_POWER_DBM_100,
  RRMC_PREAMBLE_POWER_DBM_98,
  RRMC_PREAMBLE_POWER_DBM_96,
  RRMC_PREAMBLE_POWER_DBM_94,
  RRMC_PREAMBLE_POWER_DBM_92,
  RRMC_PREAMBLE_POWER_DBM_90
}rrmc_preamble_init_rec_target_pow_et;

typedef enum
{
  RRMC_PREAMBLE_TRANS_MAX_N3,
  RRMC_PREAMBLE_TRANS_MAX_N4,
  RRMC_PREAMBLE_TRANS_MAX_N5,
  RRMC_PREAMBLE_TRANS_MAX_N6,
  RRMC_PREAMBLE_TRANS_MAX_N7,
  RRMC_PREAMBLE_TRANS_MAX_N8,
  RRMC_PREAMBLE_TRANS_MAX_N10,
  RRMC_PREAMBLE_TRANS_MAX_N20,
  RRMC_PREAMBLE_TRANS_MAX_N50,
  RRMC_PREAMBLE_TRANS_MAX_N100,
  RRMC_PREAMBLE_TRANS_MAX_N200
}rrmc_preamble_trans_max_et;

typedef enum
{
  RRMC_RRMC_CELL_BARRED,
  RRMC_RRMC_CELL_NOT_BARRED
}rrmc_cell_barred_et;

typedef enum
{
  RRMC_TIMER_300_301_MS100,
  RRMC_TIMER_300_301_MS200,
  RRMC_TIMER_300_301_MS300,
  RRMC_TIMER_300_301_MS400,
  RRMC_TIMER_300_301_MS600,
  RRMC_TIMER_300_301_MS1000,
  RRMC_TIMER_300_301_MS1500,
  RRMC_TIMER_300_301_MS2000
}rrmc_timer_300_301_et;

typedef enum
{
  RRMC_TIMER_310_MS0,
  RRMC_TIMER_310_MS50,
  RRMC_TIMER_310_MS100,
  RRMC_TIMER_310_MS200,
  RRMC_TIMER_310_MS500,
  RRMC_TIMER_310_MS1000,
  RRMC_TIMER_310_MS2000
}rrmc_timer_310_et;

typedef enum
{
  RRMC_TIMER_311_MS1000,
  RRMC_TIMER_311_MS3000,
  RRMC_TIMER_311_MS5000,
  RRMC_TIMER_311_MS10000,
  RRMC_TIMER_311_MS15000,
  RRMC_TIMER_311_MS20000,
  RRMC_TIMER_311_MS30000
}rrmc_timer_311_et;

typedef enum
{
  RRMC_TIMER_N310_N1,
  RRMC_TIMER_N310_N2,
  RRMC_TIMER_N310_N3,
  RRMC_TIMER_N310_N4,
  RRMC_TIMER_N310_N6,
  RRMC_TIMER_N310_N8,
  RRMC_TIMER_N310_N10,
  RRMC_TIMER_N310_N20
}rrmc_timer_n310_et;

typedef enum
{
  RRMC_TIMER_N311_N1,
  RRMC_TIMER_N311_N2,
  RRMC_TIMER_N311_N3,
  RRMC_TIMER_N311_N4,
  RRMC_TIMER_N311_N5,
  RRMC_TIMER_N311_N6,
  RRMC_TIMER_N311_N8,
  RRMC_TIMER_N311_N10
}rrmc_timer_n311_et;

#define OAM_LOG_CAT_AL    0x000000000030803F
#define OAM_SYS_FAIL_L2   0x0000000000000001
#define OAM_MAC_INF_L1    0x0000000000000002
#define OAM_MAC_HARQ_UL   0x0000000000000004
#define OAM_MAC_HARQ_DL   0x0000000000000008
#define OAM_L2_MAC_RACH   0x0000000000000010
#define OAM_MAC_INF_RRC   0x0000000000000020
#define OAM_MAC_INF_OAM   0x0000000000008000
#define OAM_MAC_PC_DL     0x0000000000100000
#define OAM_MAC_PC_UL     0x0000000000200000

typedef enum _setlogcatl2maccommand
{
    LOG_CAT_AL,
    SYS_FAIL_L2,
    MAC_INF_L1,
    MAC_HARQ_UL,
    MAC_HARQ_DL,
    L2_MAC_RACH,
    MAC_INF_RRC,
    MAC_INF_OAM,
    MAC_PC_DL,
    MAC_PC_UL
} setlogcatl2maccommand;


typedef enum _setlogcatdisablemaccommand
{
    CAT_LOG_AL,
    FAIL_SYS_L2,
    INF_MAC_L1,
    HARQ_MAC_UL,
    HARQ_MAC_DL,
    RACH_MAC_L2,
    RRC_INF_MAC,
    OAM_MAC_INF,
    PC_MAC_DL,
    PC_MAC_UL
}setlogcatdisablecommand;
typedef struct _oam_qcli_header
{
    UInt16 trans_id;
    UInt16 src_id;
    UInt16 dest_id;
    UInt16 api_id;
    UInt16 size;
    UInt16 id;
}oam_qcli_header_t;

typedef struct celldeleteparam
{
    UInt8 cellId;
}oam_cell_mgmt_delete_t;

typedef struct _cellmgmtsib1
{
    UInt16 intra_freq_val;
    UInt16 mnc_val;
    UInt16 mcc_val;
}oam_cell_mgmt_sib1_t;

typedef struct _cellmgmtsib2
{
    UInt16 spect_emmision;
    UInt16 paging_cycle;
    UInt16 modf_period_coeff;
    UInt16 time_allign;
} oam_cell_mgmt_sib2_t;

typedef enum _cellreconfigparam
{
    DLPC_PARAMS = 1,
    SYNC_SIGNALS,
    MIB_INFO,
    SIB3,
    SIB4,
    SIB5
}cellreconfigparam;

typedef struct fapservicebarring
{
    UInt8 barrforemer;
}oam_fap_service_barr_t;

typedef struct preambletrans
{
    UInt8 trans_val;
} oam_trans_max_t;
typedef struct fapservicerach_maxharq
{
    UInt8 maxharq;
} oam_rach_maxharq_msg_t;

typedef struct msgpoweroffset
{
    UInt8 msg_pwr_val;
} oam_msg_power_t;

typedef struct t311
{
    UInt8 val;
}oam_t311_t;

typedef struct t310
{
    UInt8 val;
}oam_t310_t;

typedef struct t301
{
    UInt8 val;
}oam_t301_t;

typedef struct t300
{
    UInt8 val;
}oam_t300_t;

typedef struct n311
{
    UInt8 val;
}oam_n311_t;
typedef struct n310
{
    UInt8 val;
} oam_n310_t;

typedef struct initrecvtgt
{
    UInt8 init_tgt;
} oam_init_recv_t;

typedef struct pwrrampstep
{
    UInt8 pwr_rmp_val;
} oam_pwr_ramp_t;
typedef struct _calltracestart
{
    UInt16 transaction_id;
    UInt8 cell_index;
    UInt8 intf_to_trace;
    UInt8 trace_depth;
    UInt8 ipaddr_len;
    UInt8 ipaddress[20]; 
} oam_call_trace_start_t;

/* CSR 57877 Fix moved Code to oam_mgmt_intf_api.h*/

typedef struct cellsetupparam
{
    UInt8 cellId;
}oam_cellmgmt_setup_t;

typedef struct cellstartparam
{
    UInt8 cellId;
}oam_cellmgmt_start_t;

typedef struct cellstopparam
{
    UInt8 cellId;
}oam_cellmgmt_stop_t;

typedef struct configkpimac
{
	UInt16 duration;
          UInt16 sample_duration;
	UInt8 periodic_Reporting;
	UInt16 kpitype;
}oam_config_kpi_mac_t;


typedef struct configkpirrc
{
    
	UInt16 duration;
        UInt16 sample_duration;
	UInt8 periodic_Reporting;
	UInt16 kpitype;
	UInt8 cell_index;
	UInt8 reset_stats;
}oam_config_kpi_rrc_t;

typedef struct configkpipdcp
{
	UInt16 duration;
        UInt16 sample_duration;
	UInt8 periodic_Reporting;
	UInt8 kpitype;
}oam_config_kpi_pdcp_t;



typedef struct resetlogcatl2macparam
{
    UInt64 log_category;
}oam_log_cat_l2_mac_t;

typedef struct resetlogcatl2rlcparam
{
    UInt64 log_category;
}oam_log_cat_l2_rlc_t;

typedef struct resetlogcatl2pdcpparam
{
    UInt64 log_category;
}oam_log_cat_l2_pdcp_t;

typedef struct resetlogcatl2egtpuparam
{
    UInt64 log_category;
}oam_log_cat_l2_egtpu_t;

/* CSR 57877 Fix moved Code to oam_mgmt_intf_api.h*/

typedef enum _setloglevell2allcommands
{
    MAC_FATALLOG_L2ALL = 1,
    MAC_ERRORLOG_L2ALL,
    MAC_WARNINGLOG_L2ALL,
    MAC_INFOLOG_L2ALL,
    MAC_DEBUGLOG_L2ALL,
    PDCP_INVALIDLOG_L2ALL,
    PDCP_FATALLOG_L2ALL,
    PDCP_ERRORLOG_L2ALL,
    PDCP_WARNINGLOG_L2ALL,
    PDCP_INFOLOG_L2ALL,
    PDCP_DEBUGLOG_L2ALL,
    RLC_DISABLED_L2ALL,
    RLC_FATALLOG_L2ALL,
    RLC_ERRORLOG_L2ALL,
    RLC_WARNINGLOG_L2ALL,
    RLC_INFOLOG_L2ALL,
    RLC_DEBUGLOG_L2ALL,
    GTPU_INCOMING_TRACE_L2ALL,
    GTPU_OUTGOING_TRACE_L2ALL,
    GTPU_FSM_TRACE_L2ALL,
    GTPU_ERROR_TRACE_L2ALL
}setloglevell2allcommands;

typedef enum _setlogenablecommands
{
    RRM_ENABLE_ERRORLOG,
    RRM_ENABLE_WARNINGLOG,
    RRM_ENABLE_BRIEFLOG,
    RRM_ENABLE_DETAILEDLOG,
    SON_ENABLE_ERRORLOG,
    SON_ENABLE_WARNINGLOG,
    SON_ENABLE_BRIEFLOG,
    SON_ENABLE_DETAILEDLOG,
    RRC_ENABLE_FATALLOG,
    RRC_ENABLE_ERRORLOG,
    RRC_ENABLE_WARNINGLOG,
    RRC_ENABLE_INFOLOG,
    RRC_ENABLE_BRIEFLOG,
    RRC_ENABLE_DETAILEDLOG
} setlogenaablecommands;

typedef enum _setlogdisablecommands
{
    RRM_DISABLE_ERRORLOG,
    RRM_DISABLE_WARNINGLOG,
    RRM_DISABLE_BRIEFLOG,
    RRM_DISABLE_DETAILEDLOG,
    SON_DISABLE_ERRORLOG,
    SON_DISABLE_WARNINGLOG,
    SON_DISABLE_BRIEFLOG,
    SON_DISABLE_DETAILEDLOG,
    RRC_DISABLE_FATALLOG,
    RRC_DISABLE_ERRORLOG,
    RRC_DISABLE_WARNINGLOG,
    RRC_DISABLE_INFOLOG,
    RRC_DISABLE_BRIEFLOG,
    RRC_DISABLE_DETAILEDLOG
} setlogdisablecommands;

typedef enum _setlogcatoamcommands
{
    CRITICALLOG,
    ERRORLOG,
    WARNINGLOG,
    BRIEFLOG,
    DETAILEDLOG
}setlogleveloamcommands;


typedef enum _setloglevell2gtpucommands
{
    GTPU_CRITICALLOG    = LTE_L2_LOG_CRITICAL,
    GTPU_ERRORLOG       = LTE_L2_LOG_ERROR,
    GTPU_WARNINGLOG     = LTE_L2_LOG_WARNING,
    GTPU_INFOLOG        = LTE_L2_LOG_INFO,
    GTPU_BRIEFLOG       = LTE_L2_LOG_BRIEF, 
    GTPU_DETAILEDLOG    = LTE_L2_LOG_DETAILED,
    GTPU_DETAILEDALLLOG = LTE_L2_LOG_DETAILEDALL
}setloglevell2gtpucommands;
#define GTPU_INCOMING_TRACE  0
#define GTPU_OUTGOING_TRACE  1
#define GTPU_FSM_TRACE       2
#define GTPU_ERROR_TRACE     3

#define OAM_MAC_FATALLOG    LTE_L2_LOG_CRITICAL    /*1*/
#define OAM_MAC_ERRORLOG    LTE_L2_LOG_ERROR       /*2*/
#define OAM_MAC_WARNINGLOG  LTE_L2_LOG_WARNING     /*4*/
#define OAM_MAC_INFOLOG     LTE_L2_LOG_INFO        /*8*/
#define OAM_MAC_BRIEFLOG    LTE_L2_LOG_BRIEF       /*16*/
#define OAM_MAC_DEBUGLOG    LTE_L2_LOG_DETAILED    /*32*/
#define OAM_MAC_DEBUGTTILOG LTE_L2_LOG_DETAILEDALL /*64*/


#define OAM_PDCP_INVALIDLOG  LTE_L2_LOG_INVALID
#define OAM_PDCP_FATALLOG    LTE_L2_LOG_CRITICAL
#define OAM_PDCP_ERRORLOG    LTE_L2_LOG_ERROR
#define OAM_PDCP_WARNINGLOG  LTE_L2_LOG_WARNING
#define OAM_PDCP_INFOLOG     LTE_L2_LOG_INFO
#define OAM_PDCP_BRIEFLOG    LTE_L2_LOG_BRIEF
#define OAM_PDCP_DEBUGLOG    LTE_L2_LOG_DETAILED
#define OAM_PDCP_DEBUGALLLOG LTE_L2_LOG_DETAILEDALL


#define OAM_RLC_DISABLED     LTE_L2_LOG_NONE
#define OAM_RLC_FATALLOG     LTE_L2_LOG_CRITICAL
#define OAM_RLC_ERRORLOG     LTE_L2_LOG_ERROR
#define OAM_RLC_WARNINGLOG   LTE_L2_LOG_WARNING
#define OAM_RLC_INFOLOG      LTE_L2_LOG_INFO
#define OAM_RLC_BRIEFLOG     LTE_L2_LOG_BRIEF
#define OAM_RLC_DEBUGLOG     LTE_L2_LOG_DETAILED
#define OAM_RLC_DEBUGALLLOG     LTE_L2_LOG_DETAILEDALL


typedef struct setloglevell3rrc
{
    UInt8 level;
}oam_set_log_level_l3_rrc_t;

#define OAM_RRM_CRITICALLOG   LTE_LOG_CRITICAL   /*0*/
#define OAM_RRM_ERRORLOG      LTE_LOG_ERROR      /*1*/
#define OAM_RRM_WARNINGLOG    LTE_LOG_WARNING    /*2*/
#define OAM_RRM_INFOLOG       LTE_LOG_INFO       /*3*/
#define OAM_RRM_BRIEFLOG      LTE_LOG_BRIEF      /*4*/
#define OAM_RRM_DETAILEDLOG   LTE_LOG_DETAILED   /*5*/
#define OAM_RRM_LOG_ENABLE        1
#define OAM_RRM_LOG_DISABLE       0

/* coverity 63011 */
#define OAM_SON_LOG_ENABLE       1
#define OAM_SON_LOG_DISABLE      0
#define OAM_SON_NW_SCAN_STATUS_PARAMS 5
#define OAM_SON_NW_SCAN_RESULT_RF_PARAMS 5
#define OAM_SON_NW_SCAN_RESULT_BCCH_PARAMS 10
#define OAM_QCLI_RRM_1_2 0

/*SPR 12727 FIX_2 START */
#define OAM_PERIODIC_SCAN_STOP_MAX_PARAMS 2
#define OAM_GET_SET_COMMANDS_MAX_PARAMS 3
/*SPR 12727 FIX_2 END */

/*eICIC_PHASE_1_2_CHANGES_START*/
#define OAM_SON_DEFAULT_CONFIG_PARAMS      6
/*eICIC_PHASE_1_2_CHANGES_END*/
typedef struct showkpimacparam
{
	UInt8 reset;
	UInt16 kpi_type;
}oam_show_kpi_mac_t;

typedef struct showkpipdcpparam
{
	UInt16 reset;
	UInt8 kpi_type;
}oam_show_kpi_pdcp_t;


typedef struct _showstatsl3rrc
{
    UInt16 bitmask;	
    UInt8 cell_index;
    UInt8 reset_stats;
}oam_show_rrc_cell_stats_t;

typedef struct mess_send
{
	UInt8 message_send;
	UInt16 apiid_expected;
}mess_send_t;



typedef struct param_map
{
 char *mode;
 char *parent;
 char *param_list;
 int cmd_type;
}param_mapping_t;

typedef struct map_cache
{
  char *mode;
  int index;
}mapping_cache_t;


typedef struct param_set_t
{
  char param[MAX_PARAM_VALUE_SIZE];
  char value[MAX_PARAM_VALUE_SIZE];
}param_set;

oam_return_et
oam_mac_log_level_pkt
(
    UInt16 C,
    setloglevell2maccommands mac_log_val
);

oam_return_et
oam_rlc_log_level_pkt
(
    UInt16 C,
    setloglevell2rlccommands log_val
);

oam_return_et
oam_pdcp_log_level_pkt
(
    UInt16 C,
    setloglevell2pdcpcommands log_val
);

oam_return_et
oam_egtpu_log_level_pkt
(
    UInt16 C,
    setloglevell2gtpucommands log_val
);

oam_return_et
oam_rach_maxharq_msg_pkt
(
    UInt16 C,
    char * const *V
);

oam_return_et
oam_msg_power_offset_group_pkt
(
    UInt16 C,
    rrmc_group_b_msg_power_offset_et pwr_offset
);

oam_return_et
oam_power_ramping_step_pkt
(
    UInt16 C,
    rrmc_power_ramping_step_et pwr_ramp
);

oam_return_et
oam_init_recv_tgt_pkt
(
    UInt16 C,
    rrmc_preamble_init_rec_target_pow_et init_tgt
);


oam_return_et
oam_preamble_trans_max_pkt
(
    UInt16 C,
    rrmc_preamble_trans_max_et trans_max
);

oam_return_et
oam_n310_pkt
(
    UInt16 C,
    rrmc_timer_n310_et timer_n310
);

oam_return_et
oam_n311_pkt
(
    UInt16 C,
    rrmc_timer_n311_et timer_n311
);

oam_return_et
oam_t300_pkt
(
    UInt16 C,
    rrmc_timer_300_301_et timer_300_301
);

oam_return_et
oam_t301_pkt
(
    UInt16 C,
    rrmc_timer_300_301_et timer_300_301
);

oam_return_et
oam_t310_pkt
(
    UInt16 C,
    rrmc_timer_310_et timer_t310
);

oam_return_et
oam_t311_pkt
(
    UInt16 C,
    rrmc_timer_311_et timer_t311
);


oam_return_et
oam_son_enable_warning_pkt
(
    UInt16 C,
    char * const *V

);


oam_return_et
oam_son_enable_detailed_pkt
(
    UInt16 C,
    char * const *V
);

oam_return_et
oam_rrm_disable_error_pkt
(
    UInt16 C,
    char * const *V
);



oam_return_et
oam_rrm_disable_detailed_pkt
(
    UInt16 C,
    char * const *V
);

oam_return_et
oam_son_disable_error_pkt
(
    UInt16 C,
    char * const *V
);




oam_return_et
oam_call_trace_start_pkt
(
    UInt16 C,
    char * const *V
);


oam_return_et
oam_call_trace_stop_pkt
(
    UInt16 C,
    char * const *V
);

oam_return_et
oam_gtpu_incoming_l2all_pkt
(
    UInt16 C,
    char* const *V
);

oam_return_et
oam_gtpu_outgoing_l2all_pkt
(
    UInt16 C,
    char* const* V
);

oam_return_et
oam_gtpu_fsm_l2all_pkt
(
    UInt16 C,
    char* const *V
);

oam_return_et
oam_gtpu_error_l2all_pkt
(
    UInt16 C,
    char* const *V
);

oam_return_et
oam_qcli_mac_get_kpi_stat_conf
(
    void* p_api_buf
);

oam_return_et 
oam_qcli_pdcp_get_kpi_stat_conf
(
    void* p_api_buf,
    U16 msg_len
);

oam_return_et 
oam_qcli_mac_config_kpi_stat_conf
(
    void* p_api_buf,
    U16 msg_len
);

oam_return_et 
oam_qcli_pdcp_config_kpi_stat_conf
(
    void* p_api_buf,
    U16 msg_len
);

oam_return_et 
oam_qcli_get_log_cat_MAC_resp
(
    void* p_api_buf,
    U16 msg_len
);

oam_return_et 
oam_qcli_get_log_cat_RLC_resp
(
    void* p_api_buf,
    U16 msg_len
);

oam_return_et 
oam_qcli_get_log_cat_PDCP_resp
(
    void* p_api_buf,
    U16 msg_len
);

oam_return_et 
oam_qcli_get_log_cat_GTPU_resp
(
    void* p_api_buf,
    U16 msg_len
);

oam_return_et
oam_gtpu_incoming_all_pkt
(
    UInt16 C,
    char* const* V
);

oam_return_et
oam_gtpu_outgoing_all_pkt
(
    UInt16 C,
    char* const *V
);

oam_return_et
oam_gtpu_fsm_all_pkt
(
    UInt16 C,
    char* const *V
);

oam_return_et
oam_gtpu_error_all_pkt
(
    UInt16 C,
    char* const *V
);

oam_return_et
oam_l2_disable_sys_fail_pkt
(
    UInt16 C,
    char * const *V
);

oam_return_et
oam_disable_lmac_l1_inf_pkt
(
    UInt16 C,
    char * const *V
);

oam_return_et
oam_disable_mac_ul_harq_pkt
(
    UInt16 C,
    char* const *V
);

oam_return_et
oam_disable_mac_dl_harq_pkt
(
    UInt16 C,
    char* const *V
);

oam_return_et
oam_disable_mac_rach_pkt
(
    UInt16 C,
    char* const *V
);

oam_return_et
oam_disable_mac_rrc_inf_pkt
(
    UInt16 C,
    char* const *V
);

oam_return_et
oam_disable_mac_oam_inf_pkt
(
    UInt16 C,
    char* const *V
);

oam_return_et
oam_disble_mac_dl_pc_pkt
(
    UInt16 C,
    char* const *V
);

oam_return_et
oam_disable_mac_ul_pc_pkt
(
    UInt16 C,
    char* const *V
);

oam_return_et
oam_set_log_cat_disable_rlc_pkt
(
    UInt16 C,
    char * const *V
);

oam_return_et
oam_set_log_cat_disable_pdcp_pkt
(
    UInt16 C,
    char * const*V
);

oam_return_et
oam_set_log_cat_disable_gtpu_pkt
(
    UInt16 C,
    char* const * V
);

oam_return_et
oam_set_log_cat_disable_mac_all_pkt
(
    UInt16 C,
    char* const *V
);

oam_return_et
oam_lvl_pkt
(
    UInt16 C,
    UInt32 oam_log_val
);

oam_return_et
oam_rrc_log_lvl_pkt
(
    UInt16 C,
    setloglevell3rrccommands rrc_log_val
);
/* spr 17535 - start */
oam_return_et
oam_S1AP_log_lvl_pkt
(
    UInt16 C,
    setloglevelS1APcommands s1ap_log_val
);

oam_return_et
oam_X2AP_log_lvl_pkt
(
    UInt16 C,
    setloglevelX2APcommands x2ap_log_val
);


#ifdef OAM_SON_ENABLE
oam_return_et
oam_son_enable_pkt
(
    UInt16 C,
    char * const *V,
    setson oam_son_log
);
#endif
/* spr 17535 - end */
oam_return_et
oam_son_enable_error_pkt
(
    UInt16 C,
    char * const *V
);

oam_return_et
oam_log_enable_rrc_pkt
(
    UInt16 C,
    char * const *V
);

oam_return_et
oam_log_enable_s1ap_pkt
(
    UInt16 C,
    char * const *V
);

oam_return_et
oam_log_enable_x2ap_pkt
(
    UInt16 C,
    char * const *V
);


oam_return_et
oam_enable_rrm_pkt
(
    UInt16 C,
    char * const *V,
    rrm_oam_log_level_et log_level
);

oam_return_et
oam_enable_rrm_error_pkt
(
    UInt16 C,
    char * const *V
);

oam_return_et
oam_log_enable_rrm_warn_pkt
(
    UInt16 C,
    char * const *V
);

oam_return_et
oam_log_enable_rrm_brief_pkt
(
    UInt16 C,
    char * const *V
);

oam_return_et
oam_log_enable_rrm_detail_pkt
(
    UInt16 C,
    char * const*V
);

oam_return_et
oam_log_disable_rrc_pkt
(
    UInt16 C,
    char * const*V
);
oam_return_et
oam_log_disable_s1ap_pkt
(
    UInt16 C,
    char * const*V
);
oam_return_et
oam_log_disable_x2ap_pkt
(
    UInt16 C,
    char * const*V
);


/* SPR_13116_CHANGE_START */
oam_return_et
oam_qcli_send_rrm_get_log_level_cmd
(
    UInt16 C,
        Char8* const *V
        );
/* SPR_13116_CHANGE_END */

oam_return_et
oam_qcli_send_rrm_set_log_level_cmd
(
    UInt16 C,
    rrm_oam_log_level_et rrm_log_val,
    Char8* const *V
);
#ifdef OAM_SON_ENABLE
oam_return_et
oam_son_log_lvl_pkt
(
    UInt16 C,
    setson son_log_val,
    char * const *V
);

/* SPR_13116_CHANGE_START */
oam_return_et
oam_son_get_log_lvl_pkt
(
    UInt16 C,
        char * const *V
        );
/* SPR_13116_CHANGE_END */


#endif
oam_return_et
oam_cell_cfg_sib1_ifr
(
    UInt16 C,
    char *const*V
);

oam_return_et
oam_cell_cfg_sib1_mnc
(
    UInt16 C,
    char *const*V
);

oam_return_et
oam_cell_cfg_sib1_mcc
(
    UInt16 C,
    char *const*V
);

oam_return_et
oam_cell_cfg_sib2_add_spect_emi
(
    UInt16 C,
    char * const*V
);

oam_return_et
oam_cell_cfg_sib2_dflt_pag_cyl
(
    UInt16 C,
    char *const *V
);

oam_return_et
oam_cell_cfg_sib2_mod_per_coeff
(
    UInt16 C,
    char *const*V
);

oam_return_et
oam_cell_cfg_sib2_nb
(
    UInt16 C,
    char * const *V
);

oam_return_et
oam_cell_cfg_sib2_time_align_timer
(
    UInt16 C,
    char * const *V
);

oam_return_et
oam_incoming_lvl_pkt
(
    UInt16 C,
    UInt8 trace_flag,
    UInt8 trace_val
);

oam_return_et
oam_gtpu_lvl_pkt
(
    UInt16 C,
    UInt8 trace_flag
);

oam_return_et
oam_set_log_level_all_pkt
(
    UInt16 C,
    char* const *V
);

UInt16
oam_get_word_from_header (UChar8 * p_header);

oam_return_et
oam_send_shutdown_request
(
    UInt16 C,
    char* const *V
);

oam_return_et
oam_set_log_level_l2_all_pkt
(
    UInt16 C,
    char* const *V
);

oam_return_et
oam_son_enable_brief_pkt
(
    UInt16 C,
    char * const *V
);

int show_prompt(void);

void oam_print_header(void);
void oam_print_line(void);

oam_return_et
oam_All_log_level_pkt
(
    UInt16 C,
    oam_sys_log_level_et rlc_log_val,
    UInt16 api_id
);


UInt16 oam_qcli_fill_param(UInt16 C,char * const *V,UInt16 index,param_set **param);

void oam_qcli_update_cache(UInt16 index);

SInt16 oam_qcli_find_index(void *current_mode);

char *
oam_qcli_send_command_no_param
(
    int C,
    char *const *V,
    const char *const *E,
    const char *const *T,
    void *arg
);

char *
oam_qcli_send_command
(
    int C,
    char *const *V,
    const char *const *E,
    const char *const *T,
    void *arg
);

UInt16
oam_qcli_message_length
(
  UInt8 number_of_param,
  param_set *param,
  char *parent,
  UInt16 api_id
);

void oam_qcli_compose_payload
(
    UInt8 *p_msg,
    UInt8 number_of_param,
    param_set *param,
    char *parent,
    UInt16 msg_length,
    UInt16 api_id
);

oam_return_et
oam_send_get_set_req
(
    UInt8 no_of_param,
    param_set *param,
    char *parent,
    UInt16 trans_id,
    UInt16 src_id,
    UInt16 dest_id,
    UInt16 api_id
);

oam_return_et
oam_send_delete_obj_req
(
 char *parent,
 UInt16 trans_id,
 UInt16 src_id,
 UInt16 dest_id,
 UInt16 api_id
 );

oam_return_et
oam_send_add_obj_req
(
    char *parent,
    UInt16 trans_id,
    UInt16 src_id,
    UInt16 dest_id,
    UInt16 api_id
);


void oam_qcli_compose_delete_req
(
    UInt8 *p_payload,
    char * parent
);

oam_return_et oam_interface_resp(UInt8 *);

/********************************PM_CODE************************/
oam_return_et
oam_qcli_handle_rlc_get_stats_cnf
(
    void* p_api_buf,
    UInt16 msg_len,
    oam_error_code_et *p_error_code

);

oam_return_et
oam_qcli_handle_mac_get_stats_cnf
(
    void* p_api_buf,
    UInt16 msg_len,
    oam_error_code_et *p_error_code

);

oam_return_et
oam_qcli_handle_s1ap_get_stats_cnf
(
    void* p_api_buf,
    UInt16 msg_len,
    oam_error_code_et *p_error_code

);

oam_return_et
oam_qcli_handle_pdcp_get_stats_cnf
(
    void* p_api_buf,
    UInt16 msg_len,
    oam_error_code_et *p_error_code

);

typedef struct
{
    UInt8 rrc_config_req_flag;
    UInt8 mac_config_req_flag;
    UInt8 pdcp_config_req_flag;

}oam_cli_pm_config_req_t;

char *
oam_qcli_send_no_param_get_set_commands
(
 int param_count,
 char *const *p_param_list_in,
 const char *const *E,
 const char *const *T,
 void *arg
);
/* CSR 57877 Fix moved Code to oam_mgmt_intf_api.h*/
void
oam_send_set_req_after_add_resp(UInt8 *p_api_buf);
unsigned short decode_word_from_buf(unsigned char* p_buf);
#ifdef OAM_SON_ENABLE
Char8 *
oam_son_get_last_nw_scan_status
(
    SInt32 C,
    Char8 *const *V,
    const Char8 *const *E,
    const Char8 *const *T,
    void *arg
);

Char8 *
oam_son_get_last_nw_scan_results_rf
(
    SInt32 C,
    Char8 *const *V,
    const Char8 *const *E,
    const Char8 *const *T,
    void *arg
);

Char8 *
oam_son_get_last_nw_scan_results_bcch
(
    SInt32 C,
    Char8 *const *V,
    const Char8 *const *E,
    const Char8 *const *T,
    void *arg
);

char *
oam_send_periodic_scan_stop
(
    int param_count,
    char *const *p_param_list_in,
    const char *const *E,
    const char *const *T,
    void *arg
);


Char8* qcli_oam_son_nbr_cell_update_cmd_handler(int C, char *const *V,
                                                 const char *const *E,
                                                 const char *const *T,
                                                 void *arg);

/* SPR_19279_START */
Char8* qcli_oam_epc_plmnlist_update_cmd_handler(int C, char *const *V,
                                                 const char *const *E,
                                                 const char *const *T,
                                                 void *arg);
/* SPR_19279_END */

Char8* qcli_handle_anr_meas_config_cmd(SInt32 param_count,
                                       Char8 *const *p_param_list_in,
                                       const Char8 *const *E,
                                       const Char8 *const *T,
                                       void *arg);


Char8* qcli_validate_send_gap_config_command(SInt32 param_count,
                                            Char8 *const *p_param_list_in,
                                            const Char8 *const *E,
                                            const Char8 *const *T,
                                            void *arg);


#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
    void format_rrh_parameters(Char8 *name, Char8 *value);
#endif
#endif


#endif

Char8* qcli_validate_send_optional_param_cmd(SInt32 param_count,
                                       Char8 *const *p_param_list_in,
                                       const Char8 *const *E,
                                       const Char8 *const *T,
                                       void *arg);

Char8*
qcli_send_multi_instance_obj_command(int C, char *const *V,
                                    const char *const *E,
                                    const char *const *T,
                                    void *arg);

oam_return_et
oam_qcli_prepare_add_object_cmd_buffer(SInt32 C, Char8 *const *V);

Char8* qcli_handle_anr_add_meas_config_cmd(int C, Char8 *const *V,
                                           const Char8 *const *E,
                                           const Char8 *const *T,
                                           void *arg);
void oam_cli_abort(void);
#endif  /* __OAM_QCLI_PROTO_H__ */
