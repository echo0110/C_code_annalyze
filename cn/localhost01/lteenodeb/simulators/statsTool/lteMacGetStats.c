#include <stddef.h>
#include "lteFapiMacPhyInterface.h"
//#include "lteMacStatsManager.h"
#include "lteCommonStatsManager.h"
#include "ltePerfStats.h"
#include "lteMacTypes.h"
/*spr 22474 start*/
    /*CSPL Code removed*/
/*spr 22474 end*/

#ifdef UTFWK_SIMULATION
#define MIN_LC_VALUE 3
#define MAX_LC_VALUE 5


#endif

#if (defined (DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
/*#ifdef DL_UL_SPLIT*/
#include "lteMacExecutionLegs.h"
#endif

/*++coverity 47825*/
/* +- SPR 17777 */
static void printEcidPdcchOrderStats(void);
/* +- SPR 17777 */
/*--coverity 47825*/
/* +- SPR 18490 */
UInt16 spsIntervalMap_g[TOTAL_SPS_INTERVAL]= { 10, 20, 32, 40, 64, 80, 128, 160, 320, 640 };
#define SIZEOF_L1_HDR (offsetof(FAPI_l1ApiMsg_st, msgBody))

LteMacStatsInfo gMacStats;
#ifdef PERF_STATS

#define MAX_PERF_STATS 2
/* + PERF_CA */
LteCellPerfStats *gMacCellPerfStats_p[MAX_NUM_CELL] ;
LteUePerfStats *gMacUePerfStats_p ;
LteCellPerfStats *gPdcpCellPerfStats_p[MAX_NUM_CELL] ;
LteUePerfStats *gPdcpUePerfStats_p;
LteCellPerfStats *gRlcCellPerfStats_p[MAX_NUM_CELL] ;
/* - PERF_CA */
LteUePerfStats *gRlcUePerfStats_p ;
LtePerfStatInfo gPerfStats;
LtePerfStatsControlInfo *gPerfStatsControlInfo_p;
PerfStatsScanner g_perfStatsScannerObj;


#endif
/*SPR 21545 Start*/
UInt8 instanceL2_g;
/*SPR 21545 End*/
#if (defined (DL_UL_SPLIT) || defined (DL_UL_SPLIT_TDD))
UChar8 perfStatArr[MAX_OVERSHOOT_STATS][50]=
                            {
                                "UL",
                                "DL"
};
#endif
#ifdef CYCLE_COUNT_MAC_RLC_PDCP

#ifdef TDD_CONFIG
#ifdef DL_UL_SPLIT_TDD

UChar8 MAC_CC_FUNC_STRING[MAX_EXECUTION_LEGS][100] =
{
   /*  MAC EL*/
    {"PROCESS_DL_HARQ_ACK_NACK_QUEUE"},
    {"PROCESS_DL_HARQ_TIMER_EXPIRY"},
    {"PROCESS_DL_SPS_STRATEGY"},
    {"PROCESS_DL_SCH_STRATEGY"},
    {"PROCESS_PDCCH_PDSCH_WITH_MUX"},
    {"PROCESS_ENCODER_QUEUE_WITHOUT_TX_UNTIL_LAST_DATA"},
    {"TX_DL_CONTROL_MSG"},
    {"TX_DL_DATA_MSG"},
    {"PROCESS_DRX_FOR_ALL_UE"},
    {"PROCESS_MEAS_GAP_FOR_ALL_UE"},
    {"PROCESS_DL_OAM_AND_RRC_MSG"},
    {"PROCESS_PENDING_DELETE_UE"},
    {"PROCESS_PENDING_RESET_UE"},
    {"PROCESS_MISC_UPDATES"},
    {"PROCESS_RX_UNTIL_SUBFRAME_IND"},
    {"PROCESS_UL_HARQ_TIMER_EXPIRY"},
    {"PROCESS_UL_SPS_STRATEGY"},
    {"PROCESS_UL_SCH_STRATEGY"},
    {"PROCESS_PUSCH_FOR_DELAY_ONE"},
    {"PROCESS_DEMUX_QUEUE"},
    {"PROCESS_SRS_REPORTS"},
    {"PROCESS_UL_CSI_SINR_REPORTS"},
    {"PROCESS_SPS_PDSCH"},
    {"PROCESS_CQI_REPORTS"},
    {"PROCESS_SPS_PUSCH"},
    /* + E_CID_5.3*/
    {"PROCESS_E_CID_REPORTS"},
    /* - E_CID_5.3*/
    {"PROCESS_PRS_TRANSMISSION"}, /* PRS_CHANGE */
    {"PROCESS_UL_RA"},
    {"PROCESS_UL_OAM_AND_RRC_MSG"},
    {"PROCESS_UL_PENDING_DELETE_UE"},
    {"PROCESS_UL_PENDING_RESET_UE"},
    {"PROCESS_DL_PERFORMANCE_STATS_UPDATE"},
    {"PROCESS_UL_PERFORMANCE_STATS_UPDATE"},
    {"PROCESS_SUB_FRAME_IND_UL"},
    {"WAIT_FOR_TDD_SUBFRAME_IND"},
    {"PROCESS_HARQ_TIMER_EXPIRY"},
    {"PROCESS_ENCODER_QUEUE_WITHOUT_TX"},
    {"PROCESS_UL_CONFIG_MSG"},
    {"TX_UL_CONFIG_MSG"},
    {"PROCESS_SRS_FOR_U_SUBFRAME"},
    /*Rel 5.4.0 Getstats Fix Start */
    ("PROCESS_MERGE_UL_STARTEGY_NODES"),
    /*Rel 5.4.0 Getstats Fix End */
    {"PROCESS_PRACH_FOR_S_SUBFRAME"},
    {"PROCESS_SRS_FOR_S_SUBFRAME"},
    {"RLC_HANDLE_TX_OPPORTUNITY"},
    {"RLC_HANDLE_HARQ_FAIL_IND"},
    {"PROCESS_MEM_CLEANUP"},
    {"TX_HI_DCI0_MSG"},
    {"PROCESS_UL_PDCCH_WITH_MUX"},
    {"PROCESS_DL_PDCCH_SYNC"},
#ifdef FAPI_CONTROL_FLOW
    {"FAPI_DL_FLOW_SYNC"},
    {"FAPI_UL_FLOW_SYNC"},
#endif
    {"SEND_AUDIT_REPORT_FOR_INACTIVE_UE"},
    /*enum for MAC*/
    /*enum from RLC*/
    {"PROCESS_RLC_HANDLE_HARQ_FAILURE_IND"},
    {"PROCESS_RLC_UL_CHECK_TIMER_EXPIRY"},
    {"PROCESS_RLC_UL_HDLR_REESTABLISH_Q"},
    {"PROCESS_RLC_UL_HDLR_PEN_DEL_Q"},
    /*Rel 5.4.0 Getstats Fix Start */
    {"PROCESS_RLC_HANDLE_STATUS_REPORT"},
    /*Rel 5.4.0 Getstats Fix End */
    {"PROCESS_RLC_HANDLE_DATA_FROM_MAC"},
    {"PROCESS_RLC_DL_CHECK_TIMER_EXPIRY"},
    {"PROCESS_RLC_DL_HDLR_REESTABLISH_Q"},
    {"PROCESS_RLC_DL_HDLR_PEN_DEL_Q"},
    {"PROCESS_RLC_HANDLE_TX_OPPORTUNITY"},
    /*enum for RLC*/
    /*enum for Others*/
    {"PROCESS_START_PHY"},
    {"PROCESS_OAM_AND_RRC_MSG"},
    {"WAIT_FOR_SUBFRAME_IND"},
    {"DEFAULT_STARTUP_EXEC_LEG"},
    {"REGISTER_WITH_PHY_FOR_SUBFRAME_IND_AFTER_INIT_MAC"},
    {"INIT_DL_THREAD_PARAMS"},
    {"INIT_UL_THREAD_PARAMS"},
    {"INIT_RLC_PARAMS"},
#ifdef DL_DATA_SEPARATION
    {"PROCESS_RLC_HANDLE_TX_OPPORTUNITY_WITH_ENCODER_PER_UE"},
    {"RLC_UPDATE_BUFF_OCC_TO_MAC"},
    {"PROCESS_DL_SYNC_TX_DATA"},
#endif

};



#else
                                                                                       
UChar8 LEG_CC_FUNC_STRING[MAX_LEG_CC_FUNC][100] =
{
    {"UNKNOWN"},
    {"WAIT_FOR_SUBFRAME_IND"},
    {"PROCESS_HARQ_TIMER_EXPIRY_IF_TIME_LEFT"},
    {"PROCESS_CQI_REPORTS_IF_TIME_LEFT"},
    {"PROCESS_DEMUX_QUEUE"},
    {"PROCESS_DEMUX_QUEUE_IF_TIME_LEFT"},
    {"PROCESS_MUX_QUEUE"},
    {"PROCESS_MUX_QUEUE_UNTIL_LAST_NODE"},
    {"PROCESS_N_RRC_OAM_MSG_IF_TIME_LEFT"},
    {"PROCESS_PDCCH_PDSCH_WITH_MUX"},
    {"PROCESS_PDCCH_PDSCH_WITHOUT_MUX"},
    {"PROCESS_PDCCH_PDSCH_WITH_MUX_AND_TRIGGER_PUSCH"},
    {"PROCESS_PDCCH_PDSCH_WITHOUT_MUX_AND_TRIGGER_PUSCH"},
    {"PROCESS_PUSCH_FOR_DELAY_ONE"},
    {"TX_DL_DATA_MSG"},
    {"PROCESS_TX_QUEUE_UNTIL_LAST_NODE"},
    {"PROCESS_TX_QUEUE"},
    {"TX_COMMON_CHANNEL_DATA"},
    {"TX_DL_CONTROL_MSG"},
    {"TX_DL_DATA_MSG_IF_DATA_READY"},
    {"TX_DL_CONTROL_MSG_AND_TRIGGER"},
    {"PROCESS_ENCODER_QUEUE_WITH_TX_UNTIL_LAST_DATA"},
    {"PROCESS_DL_HARQ_ACK_NACK_QUEUE"},
    /*SPR 16855 +-*/
    {"PROCESS_DL_OAM_AND_RRC_MSG"},
    {"PROCESS_UL_OAM_AND_RRC_MSG"},
    {" PROCESS_UL_PENDING_DELETE_UE"},
    {"PROCESS_START_PHY"},
    {"PROCESS_OAM_AND_RRC_MSG"},
    /*SPR 16855 +-*/
   {"PROCESS_PENDING_DELETE_UE"},
    {"PROCESS_PENDING_RESET_UE"},
    {"SEND_AUDIT_REPORT_FOR_INACTIVE_UE"},
    {"PROCESS_UL_RA"},
    {"PROCESS_DL_SPS_STRATEGY"},
    {"PROCESS_DL_SCH_STRATEGY"},
    {"PROCESS_UL_SPS_STRATEGY"},
    {"PROCESS_UL_SCH_STRATEGY"},
    {"PROCESS_RX_CONTROL_DATA_WITH_DEMUX"},
    {"PROCESS_HARQ_TIMER_EXPIRY"},
    {"PROCESS_DL_HARQ_TIMER_EXPIRY"},
    {"PROCESS_UL_HARQ_TIMER_EXPIRY"},
    {"RLC_HANDLE_TX_OPPORTUNITY"},
    {"PROCESS_CQI_REPORTS"},
    {"PROCESS_N_CQI_REPORTS"},
    {"PROCESS_N_RRC_OAM_MSG"},
    {"REGISTER_WITH_PHY_FOR_SUBFRAME_IND"},
    {"REGISTER_WITH_PHY_FOR_SUBFRAME_IND_AFTER_INIT_MAC"},
    {"WAIT_FOR_CELL_CONFIG"},
    {"WAIT_FOR_PDCCH_READY_TRIGGER"},
    {"PROCESS_N_DEMUX_QUEUE"},
    {"WAIT_FOR_CONTROL_MSG_SENT_TRIGGER"},
    {"PROCESS_RX_UNTIL_SUBFRAME_IND"},
    {"PROCESS_RX_CONTROL_DATA_WITHOUT_DEMUX"},
    {"PROCESS_ENCODER_QUEUE_WITH_TX"},
    {"PROCESS_ENCODER_QUEUE_WITHOUT_TX"},
    {"PROCESS_ENCODER_QUEUE_WITHOUT_TX_UNTIL_LAST_DATA"},
    {"PROCESS_PUSCH_FOR_DELAY_TWO"},
    {"PROCESS_PUSCH_FOR_DELAY_THREE"},
    {"PROCESS_UL_SCH_STRATEGY_IF_NOT_ALREADY_DONE"},
    {"SEND_CONTROL_TRIGGER"},
    {"RX_CONTROL_TRIGGER"},
    {"DEFAULT_STARTUP_EXEC_LEG"},
    {"PROCESS_UL_CONFIG_MSG"},
    {"TX_UL_CONFIG_MSG"},
    {"PROCESS_SPS_PDSCH"},
    {"PROCESS_UL_CSI_SINR_REPORTS"},
    {"PROCESS_SPS_PUSCH"},
    /* + E_CID_PRS_TO_5.3*/
    {"PROCESS_E_CID_REPORTS"}, 
    {"PROCESS_PRS_TRANSMISSION"},
    /* - E_CID_PRS_TO_5.3*/
#ifdef TDD_CONFIG
	{"WAIT_FOR_TDD_SUBFRAME_IND"},
/*SPR 20200 fix start*/
    {"PROCESS_PRACH_FOR_S_SUBFRAME"},
    {"PROCESS_SRS_FOR_S_SUBFRAME"},
    /*Rel 5.4.0 Getstats Fix Start */
/*SPR 20200 fix start*/
    {"PROCESS_SRS_FOR_U_SUBFRAME"},
    /*Rel 5.4.0 Getstats Fix End */
    /* TDD Config 0 changes start */
    {"PROCESS_MERGE_UL_STARTEGY_NODES"},
    /* TDD Config 0 changes end */
#endif
    {"PROCESS_SRS_REPORTS"},
    {"MAC_PROCESS_DRX_FOR_ALL_UE"}, /* DRX_CHG */
    {"MAC_PROCESS_MEAS_GAP_FOR_ALL_UE"}, /* MEAS_GAP_CHG */
    {"PROCESS_DELETE_SCHEDULE_LC_LIST"},
#ifdef TDD_CONFIG
    {"PDCP_TX_PROCESS_MESSAGE"},
    {"RLC_HANDLE_CTRL_PDU_RLC_AM"},
    {"RLC_HANDLE_HARQ_FAIL_IND"},
    {"RLC_PDCP_HANDLE_DATA_FRM_MAC"},
    {"RLC_TIMER_CHK_FOR_EXPIRY"},
    {"RLC_TIMER_CHK_FOR_TX_EXPIRY"},
    {"PDCP_TX_PROCESS_DELIVERY_IND"},
    {"RLC_HANDLE_COMMON_CHANNEL_AND_PENDING_QUEUE"},
    /* SPR 13373 fix start */
    {"RLC_HANDLE_PENDING_DELETE_UE"},
    /* SPR 13373 fix end */
    {"PROCESS_RLC_PDCP_MSG"},
#endif
    {"PROCESS_MISC_UPDATES"},
#ifdef UTFWK_SIMULATION
        {"UPDATE_TICK_RX_MSQ"},
        {"RLC_UPDATE_QLOAD"},
        {"RLC_HANDLE_TX_OPPORTUNITY_QOS"},
#endif

#ifdef PERF_STATS
        {"PROCESS_MAC_PERF_STATS"},
#endif
#ifdef TDD_CONFIG
        {"PROCESS_MEM_CLEANUP"},
#endif // TDD_CONFIG

        {"UPDATE_DL_TOP_SCH_LIST_SCELL_STATE"},
        {"PROCESS_DL_CA_TIMER_EXPIRY"},
        {"PROCESS_PENDING_EVENT_FD"},
#ifdef DL_DATA_SEPARATION
        {"PROCESS_RLC_HANDLE_TX_OPPORTUNITY_WITH_ENCODER_PER_UE"},
        {"RLC_UPDATE_BUFF_OCC_TO_MAC"},
        {"PROCESS_DL_SYNC_TX_DATA"},
#endif
#ifndef DL_UL_SPLIT
        {"PROCESS_ENCODER_QUEUE_OF_OTHER_CELLS"},
        {"PROCESS_SET_STRATEGY_FD"},
        {"PROCESS_CHECK_STRATEGY_FD"},
        {"PROCESS_SET_PDCCH_PDSCH_FD"},
        {"PROCESS_CHECK_PDCCH_PDSCH_FD"},
        {"PROCESS_SET_MUX_FD"},
        {"PROCESS_CHECK_MUX_FD"},
        {"PROCESS_SET_ENCODER_FD"},
        {"PROCESS_CHECK_ENCODER_FD"}
#endif
        ,{"MAX_LAST_EL"},

};
#endif
#else

#ifdef DL_UL_SPLIT

UChar8 excecutionFlowVar[NUM_OF_EXEC_FLOWS][100]=
                            {
/* + SPR 9460 Changes*/
//#ifdef KPI_STATS   
                                /*SPR 16855 +-*/
                                "MAC-DL",
                                "MAC-UL + RLC-DL",
                                "RLC-UL"
 };

ExecutionFlowCreationParam executionFlowCreationParamArr_g[NUM_OF_EXEC_FLOWS]
                = {
/* + SPR 9460 Changes*/
//#ifdef KPI_STATS 
                                /*SPR 16855 +-*/
                    {
                        1,     /*Thread id*/
                        99,     /*Thread Priority */
                        {
                            INIT_DL_THREAD_PARAMS,
                            0, /* Thread bind mask */
                            1,
                            PHY_DL_DELAY, /* delay to phy */
/* + SPR 9460 Changes*/
                            /*SPR 16855 +-*/
                            16, /* execution leg count */
                            /*SPR 16855 +-*/
#ifdef OVERSHOOT_CORRECTION
                            12, /*mandatory ELs count*/
#endif
                            2, /* core id */
                            {
                             {PROCESS_DL_HARQ_ACK_NACK_QUEUE,0,0,0},
                             {PROCESS_DL_HARQ_TIMER_EXPIRY,1,0,0},
/* + SPR 9460 Changes*/
                             {PROCESS_DL_SPS_STRATEGY,0,0,0},
                             {PROCESS_DL_SCH_STRATEGY,0,0,0},
                             {PROCESS_PRS_TRANSMISSION,0,0,0}, /* PRS_CHANGES */
                             {PROCESS_PDCCH_PDSCH_WITH_MUX,0,0,0},
#ifndef DL_DATA_SEPARATION
                             {PROCESS_ENCODER_QUEUE_WITHOUT_TX_UNTIL_LAST_DATA,0,0,0},
#endif
                             {TX_DL_CONTROL_MSG,0,0,0},
#ifndef DL_DATA_SEPARATION
                             {TX_DL_DATA_MSG,0,0,0},
#endif
                             {PROCESS_DRX_FOR_ALL_UE,0,0,0},
                             {PROCESS_MEAS_GAP_FOR_ALL_UE,0,0,0},
                             {PROCESS_CQI_REPORTS,0,0,0},
                             {PROCESS_SPS_PDSCH,0,0,0},
                             {PROCESS_DL_OAM_AND_RRC_MSG,0,0,0},
#ifdef DL_DATA_SEPARATION
                            { PROCESS_DL_SYNC_TX_DATA,0,0,0},
                            { RLC_UPDATE_BUFF_OCC_TO_MAC,0,0,0},
#endif
/* - SPR 9460 Changes*/
                            /*SPR 16855 +-*/
                             {PROCESS_PENDING_DELETE_UE,0,0,0},
                            /*SPR 16855 +-*/
                             {PROCESS_DL_PERFORMANCE_STATS_UPDATE,DL_OVERSHOOT_STATS,0,0}
                            }
                        }
                     },
                     {  /* 2nd */
                        2, /*Thread id*/
                        99, /* thread priority */
                        {
                            INIT_UL_THREAD_PARAMS,
                            0, /* Thread bind mask */
                            2,
                            PHY_DL_DELAY, /* delay to phy */
/* + SPR 9460 Changes*/
                            /*SPR 16855 +-*/
			    /* + SPR 22494 */
#ifndef DL_DATA_SEPARATION
#ifdef OBSELETE_REL_8
                            20, /* execution leg count */
#else                            
                            19, /* execution leg count */ 
#endif                            
#else
#ifdef OBSELETE_REL_8
                            18, /* execution leg count */
#else                            
                            17, /* execution leg count */
#endif                            
#endif
/* - SPR 9460 Changes*/
			     /* - SPR 22494 */
                            /*SPR 16855 +-*/
#ifdef OVERSHOOT_CORRECTION
                            6,  /* mandatory execution leg count */
#endif
                            3, /* core id */
                            {
                             {PROCESS_RX_UNTIL_SUBFRAME_IND,0,0,0},
                             {PROCESS_UL_HARQ_TIMER_EXPIRY,1,0,0},
/* + SPR 9460 Changes*/
                             {PROCESS_UL_SPS_STRATEGY,0,0,0},
                             {PROCESS_UL_SCH_STRATEGY,0,0,0},
                             {PROCESS_PUSCH_FOR_DELAY_ONE,0,0,0},
                             {PROCESS_SRS_REPORTS,0,0,0},
#ifndef DL_DATA_SEPARATION                             
                             {PROCESS_RLC_HANDLE_TX_OPPORTUNITY,0,0,0},
#endif
                             {PROCESS_DEMUX_QUEUE,0,0,0},
                             {PROCESS_UL_CSI_SINR_REPORTS,0,0,0},
                             {PROCESS_SPS_PUSCH,0,0,0},
                             {PROCESS_E_CID_REPORTS,0,0,0}, /*E-CID REPORTS*/
                             {PROCESS_UL_RA,0,0,0},
#ifdef OBSELETE_REL_8
                             {PROCESS_RLC_HANDLE_HARQ_FAILURE_IND,0,0,0},
#endif
#ifndef DL_DATA_SEPARATION                             
                             {PROCESS_RLC_DL_CHECK_TIMER_EXPIRY,0,0,0},
#endif
                             {PROCESS_RLC_DL_HDLR_REESTABLISH_Q,0,0,0},
                             {PROCESS_RLC_DL_HDLR_PEN_DEL_Q,0,0,0},
                             /*Change DROP1*/
/* - SPR 9460 Changes*/
                             {PROCESS_UL_OAM_AND_RRC_MSG,0,0,0},
                             {PROCESS_UL_PENDING_DELETE_UE,0,0,0},
                             /*SPR 16855 +-*/
                             {PROCESS_MISC_UPDATES,0,0,0},
                             /*Change DROP1*/
			     /* +- SPR 22494 */
                             {PROCESS_UL_PERFORMANCE_STATS_UPDATE,UL_OVERSHOOT_STATS,0,0}
                            }
                        }
                     },
                     {
                        3, /* thread id */
                        50, /* thread priority */
                        {
                             /*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
                            DEFAULT_STARTUP_EXEC_LEG,
#else
                            INIT_RLC_PARAMS,
#endif
                            0,
                            3,
                            PHY_DL_DELAY, /* delay to phy */
                            6,
#ifdef OVERSHOOT_CORRECTION                            
                            6,  /* mandatory execution leg count */
#endif /*OVERSHOOT_CORRECTION*/                              
                            2, /* core id */
                            {
                             {WAIT_FOR_SUBFRAME_IND,0,0,0},
                             /* SPR 9911 fix start */
                             {PROCESS_RLC_HANDLE_STATUS_REPORT,0,0,0},
                             /* SPR 9911 fix end */
                             {PROCESS_RLC_HANDLE_DATA_FROM_MAC,0,0,0},
                             {PROCESS_RLC_UL_CHECK_TIMER_EXPIRY,0,0,0},
                             {PROCESS_RLC_UL_HDLR_REESTABLISH_Q,0,0,0},
                             {PROCESS_RLC_UL_HDLR_PEN_DEL_Q,0,0,0}
                            }
                        }
                     }
                  };

UChar8 MAC_CC_FUNC_STRING[MAX_EXECUTION_LEGS][100] =
{
   /*  MAC EL*/
    {"PROCESS_DL_HARQ_ACK_NACK_QUEUE"},
    {"PROCESS_DL_HARQ_TIMER_EXPIRY"},
    {"PROCESS_DL_SPS_STRATEGY"},
    {"PROCESS_DL_SCH_STRATEGY"},
    {"PROCESS_PDCCH_PDSCH_WITH_MUX"},
    {"PROCESS_ENCODER_QUEUE_WITHOUT_TX_UNTIL_LAST_DATA"},
    {"TX_DL_CONTROL_MSG"},
    {"TX_DL_DATA_MSG"},
    {"PROCESS_DRX_FOR_ALL_UE"},
    {"PROCESS_MEAS_GAP_FOR_ALL_UE"},
    {"PROCESS_DL_OAM_AND_RRC_MSG"},
    {"PROCESS_PENDING_DELETE_UE"},
    {"PROCESS_PENDING_RESET_UE"},
    {"PROCESS_MISC_UPDATES"},
    {"PROCESS_RX_UNTIL_SUBFRAME_IND"},
    {"PROCESS_UL_HARQ_TIMER_EXPIRY"},
    {"PROCESS_UL_SPS_STRATEGY"},
    {"PROCESS_UL_SCH_STRATEGY"},
    {"PROCESS_PUSCH_FOR_DELAY_ONE"},
    {"PROCESS_DEMUX_QUEUE"},
    {"PROCESS_SRS_REPORTS"},
    {"PROCESS_UL_CSI_SINR_REPORTS"},
    {"PROCESS_SPS_PDSCH"},
/* + SPR 9460 Changes*/
    {"PROCESS_CQI_REPORTS"},
    {"PROCESS_SPS_PUSCH"},
    /* + E_CID_5.3*/
    {"PROCESS_E_CID_REPORTS"}, 
    /* - E_CID_5.3*/
    {"PROCESS_PRS_TRANSMISSION"}, /* PRS_CHANGE */
/* - SPR 9460 Changes*/
    {"PROCESS_UL_RA"},
    {"PROCESS_UL_OAM_AND_RRC_MSG"},
    {"PROCESS_UL_PENDING_DELETE_UE"},
    {"PROCESS_UL_PENDING_RESET_UE"},
    {"PROCESS_DL_PERFORMANCE_STATS_UPDATE"},
    {"PROCESS_UL_PERFORMANCE_STATS_UPDATE"},
/* + SPR 9460 Changes*/
   {"SEND_AUDIT_REPORT_FOR_INACTIVE_UE"},
/* - SPR 9460 Changes*/
    /*enum for MAC*/
    /*enum for Others*/
    {"PROCESS_START_PHY"},
    {"PROCESS_OAM_AND_RRC_MSG"},
    {"WAIT_FOR_SUBFRAME_IND"},
    {"DEFAULT_STARTUP_EXEC_LEG"},
    {"REGISTER_WITH_PHY_FOR_SUBFRAME_IND_AFTER_INIT_MAC"},
    {"INIT_DL_THREAD_PARAMS"},
    {"INIT_UL_THREAD_PARAMS"},
    {"INIT_RLC_PARAMS"},

};
UChar8 RLC_CC_FUNC_STRING[MAX_RLC_CC_FUNC][100] =
{
    /*enum from RLC*/
    {"PROCESS_RLC_HANDLE_HARQ_FAILURE_IND"},
    {"PROCESS_RLC_UL_CHECK_TIMER_EXPIRY"},
    {"PROCESS_RLC_UL_HDLR_REESTABLISH_Q"},
    {"PROCESS_RLC_UL_HDLR_PEN_DEL_Q"},
    /* SPR 9911 fix start */
    {"PROCESS_RLC_HANDLE_STATUS_REPORT"},
    /* SPR 9911 fix end */
    {"PROCESS_RLC_HANDLE_DATA_FROM_MAC"},
    {"PROCESS_RLC_DL_CHECK_TIMER_EXPIRY"},
    {"PROCESS_RLC_DL_HDLR_REESTABLISH_Q"},
    {"PROCESS_RLC_DL_HDLR_PEN_DEL_Q"},
    {"PROCESS_RLC_HANDLE_TX_OPPORTUNITY"}
    /*enum for RLC*/
};

#else
UChar8 MAC_CC_FUNC_STRING[MAX_MAC_CC_FUNC][100] =
{
 {"MAC_PROCESS_RX_UNTIL_SUBFRAME_IND"},
#ifdef UTFWK_SIMULATION
 {"MAC_UTFW_RLC_UPDATE_QLOAD"},
#endif
 {"MAC_PROCESS_DL_HARQ_ACK_NACK_QUEUE"},
 {"MAC_PROCESS_HARQ_TIMER_EXPIRY"},
 {"MAC_PROCESS_DL_CA_TIMER_EXPIRY"},
 {"MAC_PROCESS_DL_SPS_STRATEGY"},
 /* SPR 17912 Fix Start*/
 {"MAC_PROCESS_PENDING_EVENT_FD"},
 /* SPR 17912 Fix End*/
 {"MAC_PROCESS_SET_STRATEGY_FD"},
 {"MAC_PROCESS_CHECK_STRATEGY_FD"},
 {"MAC_PROCESS_DL_SCH_STRATEGY"},
 {"MAC_PROCESS_SET_PDCCH_PDSCH_FD"},
#ifndef OVERSHOOT_CORRECTION
 {"MAC_PROCESS_DEMUX_QUEUE"},
#endif
 {"MAC_PROCESS_UL_HARQ_TIMER_EXPIRY"},
 {"MAC_PROCESS_UL_SPS_STRATEGY"},
 {"MAC_PROCESS_UL_SCH_STRATEGY"},
 /*+ E_CIS_PRS_TO_5.3*/
 {"MAC_PROCESS_PRS_TRANSMISSION"},
 /*- E_CIS_PRS_TO_5.3*/
 {"MAC_PROCESS_CHECK_PDCCH_PDSCH_FD"},
 {"MAC_PROCESS_PDCCH_PDSCH_WITH_MUX"},
 {"MAC_PROCESS_SET_MUX_FD"},
 {"MAC_PROCESS_CHECK_MUX_FD"},
 {"MAC_PROCESS_MUX_QUEUE"},
 {"MAC_RLC_HANDLE_TX_OPPORTUNITY"},
 /* SPR 17912 Fix Start*/
 {"MAC_PROCESS_SET_ENCODER_FD"},
 {"MAC_PROCESS_PUSCH_FOR_DELAY_ONE"},
 {"MAC_PROCESS_SRS_REPORTS"},
 /* SPR 17912 Fix End*/
#ifndef DL_DATA_SEPARATION 
 {"MAC_PROCESS_ENCODER_QUEUE_WITHOUT_TX_UNTIL_LAST_DATA"},
#endif
 {"MAC_PROCESS_CHECK_ENCODER_FD"},
 {"MAC_PROCESS_ENCODER_QUEUE_OF_OTHER_CELLS"},
 {"MAC_TX_DL_CONTROL_MSG"},
#ifndef DL_DATA_SEPARATION 
 {"MAC_TX_DL_DATA_MSG"},
#endif
 {"MAC_PROCESS_DRX_FOR_ALL_UE"}, /* DRX_CHG */
 {"MAC_PROCESS_MEAS_GAP_FOR_ALL_UE"}, /* MEAS_GAP_CHG */
 {"MAC_PROCESS_DELETE_SCHEDULE_LC_LIST"},
#ifdef OVERSHOOT_CORRECTION
 {"MAC_PROCESS_DEMUX_QUEUE"},
#endif
 {"MAC_PROCESS_UL_RA"},
 {"MAC_PROCESS_CQI_REPORTS"},
 {"MAC_PROCESS_SPS_PDSCH"},
 {"MAC_PROCESS_UL_CSI_SINR_REPORTS"},
 {"MAC_PROCESS_SPS_PUSCH"},
 /*+ E_CIS_PRS_TO_5.3*/
 {"MAC_PROCESS_E_CID_REPORTS"},
 /*- E_CIS_PRS_TO_5.3*/
 {"MAC_UPDATE_DL_TOP_SCH_LIST_SCELL_STATE"},
#ifdef UTFWK_SIMULATION
   {"MAC_UTFW_RLC_HANDLE_TX_OPPORTUNITY_QOS"},
#endif
#ifdef DL_DATA_SEPARATION
 {"PROCESS_MAC_DL_SYNC_TX_DATA"},
 {"RLC_UPDATE_BUFFER_OCC_TO_MAC"},
#endif
 /*SPR 16855 +-*/
 {"MAC_PROCESS_UL_OAM_AND_RRC_MSG"},
 {"MAC_PROCESS_DL_OAM_AND_RRC_MSG"},
 {"MAC_PROCESS_UL_PENDING_DELETE_UE"},
 /*SPR 16855 +-*/
 {"MAC_PROCESS_PENDING_DELETE_UE"},
 {"MAC_PROCESS_PENDING_RESET_UE"},
 {"MAC_SEND_AUDIT_REPORT_FOR_INACTIVE_UE"},
 {"MAC_PROCESS_MISC_UPDATES"},
 /*SPR 16855 +-*/
 {"MAC_PROCESS_START_PHY"},
 {"NORMAL_MODE_FOR_MAC_EXEC_SCH"}
};

UChar8 RLC_CC_FUNC_STRING[MAX_RLC_CC_FUNC][100] =
{
 {"RLC_HANDLE_TRX_OPPORTUNITY"},
 {"RLC_HANDLE_CONTROL_PDU_RLC_AM"},
 {"RLC_HANDLE_HARQ_FAILURE_IND"},
 {"RLC_HANDLE_DATA_FROM_MAC"},
 {"RLC_TIMER_UPD_TICKCHECK_FOR_TIMER_EXE"},
 {"RLC_HANDLE_TX_EXPIRED_TIMER"},
 {"RLC_HANDLE_RX_EXPIRED_TIMER"},
 {"RLC_HANDLE_COMCH_DATA_FROM_MAC"},
 {"RLC_HANDLE_COMCH_TX_OPPORTUNITY"},
 {"RLC_HDLR_PEN_DEL_Q"},
 {"RLC_HDLR_REESTABLISH_Q"}
};

#endif
#endif

#if (defined(DL_UL_SPLIT_TDD) || defined(FDD_CONFIG))
UChar8 PDCP_CC_FUNC_STRING[MAX_PDCP_CC_FUNC][100] =
{
 {"PDCP_PROCESS_SRB_TX"},
 {"PDCP_PROCESS_DRB_TX"},
 {"PDCP_PROCESS_RX"},
 {"PDCP_PROCESS_TX_DELIVERY_STATUS_IND"},
 {"PDCP_ROHC_PROCESS_TIMERS"},
 {"PDCP_PROCESS_DISCARD_TIMERS"},
 {"PDCP_PROCESS_TX_CNTRL_PDU_IND"}

/* SPR 8704 changes start */
#ifdef PDCP_ASYNC_INTERFACE
/* SPR 8704 changes end */
 {"PDCP_SECURITY_PROCESS_TX"},
 {"PDCP_SECURITY_PROCESS_RX"},
 {"PDCP_RX_PROCESS_DECIPHERED_PDU"},
 {"PDCP_POLL_SECURITY_ENGINE"}
#endif
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
 ,{"NORMAL_MODE_FOR_MAC_EXEC_SCH"}
#endif
};
UChar8 SOCKET_CC_FUNC_STRING[9][100] =
{
#ifdef DL_UL_SPLIT
 {"PDCP_SOCKET_PROCESS_RX"},
 {"MAC_SOCKET_TX_DL_CONTROL_MSG"},
 {"MAC_SOCKET_TX_DL_DATA_MSG"},
 {"MAC_SOCKET_OAM_AND_RRC_MSG"},
 {"MAC_SOCKET_NORMAL_MODE_RECV_FROM"},
 {"MAC_SOCKET_RX_UNTIL_SUBFRAME_IND"},
 {"MAC_SOCKET_START_PHY"},
#else
#ifdef DL_UL_SPLIT_TDD
 {"PDCP_SOCKET_PROCESS_RX"},
 {"MAC_SOCKET_TX_DL_CONTROL_MSG"},
 {"MAC_SOCKET_TX_DL_DATA_MSG"},
 {"MAC_SOCKET_OAM_AND_RRC_MSG"},
 {"MAC_SOCKET_RX_UNTIL_SUBFRAME_IND"},
 {"MAC_SOCKET_SUB_FRAME_IND_UL"},
 {"MAC_SOCKET_WAIT_FOR_TDD_SUBFRAME_IND"},
 {"MAC_SOCKET_START_PHY"},
 {"MAC_SOCKET_NORMAL_MODE_RECV_FROM"}
#else
 {"MAC_SOCKET_RX_UNTIL_SUBFRAME_IND"},
 {"PDCP_SOCKET_PROCESS_RX"},
 {"MAC_SOCKET_TX_DL_CONTROL_MSG"},
 {"MAC_SOCKET_TX_DL_DATA_MSG"},
 {"MAC_SOCKET_START_PHY"},
 {"MAC_SOCKET_NORMAL_MODE_RECV_FROM"}
#endif
#endif
};
#endif

#ifdef DL_DATA_SEPARATION
UChar8 ENCODER_CC_FUNC_STRING[MAX_ENC_CC_FUNC][100] =
{
 {"RLC_HANDLE_TX_OPPORTUNITY_WITH_ENCODER_PER_UE"},
 {"PROCESS_MAC_TX_DL_DATA_MSG"}
};
#endif
#endif
#ifdef RLC_QUEUE_STATS
UChar8 PER_UE_PER_LC_QUEUE_STRING[MAX_UE_LC_QUEUE_COUNT][100] =
{
{"UMD_RAW_SDU_Q"},
{"AMD_RAW_SDU_Q"}
};

UChar8 QUEUE_STRING[MAX_QUEUE_COUNT][100] =
{
{"MAC_UEDATA_IND_Q"}
};

#endif

/* +- SPR 17777 */
UInt32 initStats(void)
/* +- SPR 17777 */
{
    /* Get the shared memory ID */
    gMacStats.shmId = shmget(STATS_SHM_KEY + instanceL2_g , sizeof(LteMacStats),
            STATS_SHM_FLAGS);

    if (0 > gMacStats.shmId)
    {
        perror("SHM get failed");
        return 1;
    }

    /* Map Shared memory */
    gMacStats.pStats = shmat(gMacStats.shmId, NULL, 0);
    if ((void *)-1 == gMacStats.pStats)
    {
        perror("SHM attach failed");
        return 1;
    }

    return 0;
}

#ifdef PERF_STATS
/****************************************************************************
* Function Name  :   initPerfStats 
* * Inputs         : none
* * Outputs        : none
* * Returns        : none
* * Description    : It will initialize the shared memory for the PERF stats  
* *                  
* ********************************************************************/
/* +- SPR 17777 */
UInt32 initPerfStats(void)
/* +- SPR 17777 */
{
	/* + PERF_CA */
	InternalCellIndex cellIndex = 0;
	/* + PERF_CA */
    /* Get the shared memory ID */
	gPerfStats.shmId = shmget(PERF_STATS_SHM_KEY+ instanceL2_g ,
			(MAX_PERF_STATS * sizeof(LtePerfStats) + sizeof(LtePerfStatsControlInfo)) ,
			PERF_STATS_SHM_FLAGS);

    if (0 > gPerfStats.shmId)
    {
        perror("SHM get failed");
        return PERF_STATS_FAILURE;
    }

    /* Map Shared memory */
    gPerfStatsControlInfo_p = shmat(gPerfStats.shmId, NULL, 0);

    gPerfStats.perfStats = (LtePerfStats*)(gPerfStatsControlInfo_p + 1);

    if ((void *)-1 == gPerfStatsControlInfo_p)
    {
        perror("SHM attach failed");
        return PERF_STATS_FAILURE;
    }
    if ((void *)-1 == gPerfStats.perfStats)
    {
        perror("SHM attach failed");
        return PERF_STATS_FAILURE;
    }
	/* + PERF_CA */
	/* loop runs upto MAX_NUM_CELL beacuse MAC_INIT_LAYER_REQ API not received when initPerfStats func is called */
	for (cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
	{
    	gMacCellPerfStats_p[cellIndex] = &gPerfStats.perfStats->lteCellPerfStats[cellIndex];
    	gPdcpCellPerfStats_p[cellIndex] = &gPerfStats.perfStats->lteCellPerfStats[cellIndex];
    	gRlcCellPerfStats_p[cellIndex] = &gPerfStats.perfStats->lteCellPerfStats[cellIndex];
	}
    gMacUePerfStats_p = gPerfStats.perfStats->lteUePerfStats;
    gPdcpUePerfStats_p = gPerfStats.perfStats->lteUePerfStats;
	/* - PERF_CA */
    gRlcUePerfStats_p = gPerfStats.perfStats->lteUePerfStats;

    return PERF_STATS_SUCCESS;
}

#endif

#ifdef RLC_QUEUE_STATS

void printQueueStats()
{
    UInt32 queueCounter = 0, ueCounter = 0, lcCounter = 0;
    UInt32 currentSize = 0;
    UInt32 peakSize = 0;
    printf("\n================QUEUE STATS per UE per LC=====================\n");
    printf("\nQueue Name,	Ue Id,		Lc Id, 		CurrentQueueSize,    PeakQueueSize");
    for(queueCounter = 0; queueCounter < MAX_UE_LC_QUEUE_COUNT; queueCounter ++)
    {
       // printf("\n %s :",PER_UE_PER_LC_QUEUE_STRING[queueCounter]);
        for(ueCounter =0; ueCounter < MAX_UE_SUPPORTED; ueCounter ++)
        {
             for(lcCounter =0; lcCounter < MAX_LOGICAL_CHANNEL; lcCounter ++)
             {
                 currentSize = gMacStats.pStats->queuePerUePerLcStats.qPerUePerLcStats[queueCounter].currentOccupancyPerUePerLc[ueCounter][lcCounter];
                 peakSize = gMacStats.pStats->queuePerUePerLcStats.qPerUePerLcStats[queueCounter].peakOccupancyPerUePerLc[ueCounter][lcCounter];
	         if(peakSize != 0)
                 {
	             printf("\n %s,    %d, 	%d,   	 %d,  	  %d",PER_UE_PER_LC_QUEUE_STRING[queueCounter],ueCounter,lcCounter,currentSize,peakSize);
	         }
	     }

         }  
    }
    printf("\n================QUEUE STATS =====================\n");
    printf("\nQueue Name,	Current Queue Occupancy, Peak Queue Occupancy, ");
    for(queueCounter = 0; queueCounter < MAX_UE_LC_QUEUE_COUNT; queueCounter ++)
     {
 	 currentSize = gMacStats.pStats->queueStats.qStats[queueCounter].currentOccupancy; 
	 peakSize = gMacStats.pStats->queueStats.qStats[queueCounter].peakOccupancy;
	 if(peakSize != 0)
	 {
    	     printf("\n %s , 	%d,   		%d,",QUEUE_STRING[queueCounter],currentSize,peakSize);
         } 
     }
}
#endif


#ifdef CYCLE_COUNT_MAC_RLC_PDCP
/* CA Stats Changes Start */
void printCycleCountStats(InternalCellIndex cellIndex, SInt8 ccIterator)
{
#ifdef DL_UL_SPLIT
    UInt64 totalMacRlcCycleCount = 0;
    UInt64 totalMacRlcClkTime = 0;
    UInt64 totalMacRlcCycles = 0;

#ifdef KPI_STATS
    UInt8 threadCounter = 0;
#else
    UInt8 threadCounter = 1;
#endif
#endif
    UInt8 funcCounter = 0;
    UInt64 totalCycleCount = 0, cycleCount = 0;
    UInt64 numCycles = 0,totalClkTimeInMicroSec = 0;
    UInt64 macCycleCount = 0, pdcpCycleCount = 0;
#ifndef DL_UL_SPLIT
    UInt64  rlcCycleCount = 0; 
#endif
    UInt64 totalL2Cycles = 0,totalL2CycleCount = 0, totalL2ClkTime = 0;
#ifdef TDD_CONFIG
    UInt64 totalLegCycleCount = 0, totalLegCycles = 0, totalLegClkTime = 0;
#else
    UInt64 totalMacCycleCount = 0, totalMacCycles = 0, totalMacClkTime = 0;
#ifndef DL_UL_SPLIT
    UInt64 totalRlcCycleCount = 0, totalRlcCycles = 0, totalRlcClkTime = 0;
#endif
    UInt64 totalPdcpCycleCount = 0, totalPdcpCycles = 0, totalPdcpClkTime = 0;
#endif
#ifdef DL_DATA_SEPARATION
    UInt64  totalEncoderCycleCount = 0, totalEncoderClkTime = 0, totalEncoderCycles = 0;
    UInt64  encoderClkTime = 0;
    UInt64  encoderCycleCount = 0;
#endif
    UInt64 macClkTime = 0,  pdcpClkTime = 0;
#ifndef DL_UL_SPLIT
    UInt64  rlcClkTime = 0; 
#endif
#ifdef DL_UL_SPLIT_TDD
UInt64 totalMacRlcCycleCount = 0, totalMacRlcClkTime = 0, totalMacRlcCycles = 0;
UInt64 totalMacCycleCount = 0,totalMacCycles = 0,totalMacClkTime = 0;
UInt64 totalPdcpCycleCount = 0, totalPdcpCycles = 0, totalPdcpClkTime = 0;
#endif

    UInt8 i = 0;
/*SPR 20200 fix start*/
    printf("\n total cycles is %d",gMacStats.pStats->ccStats[cellIndex][ccIterator].totalL2Cycles);
/*SPR 20200 fix end*/
    if(0 == gMacStats.pStats->ccStats[cellIndex][ccIterator].totalL2Cycles)
        return;

    printf("\n****************************** Cell [%d] *********************************\n",cellIndex);
#if(!defined(DL_UL_SPLIT)&& !defined(DL_UL_SPLIT_TDD))
    printf("\n************************************* ITERATION %d ****************************************\n",ccIterator);
#else
#ifndef GETSTATS_EL_CLOCKTIME
      printf("\n*************,**************** Iteration%02d,****************************,*********************** \n",ccIterator);
      printf("Function Name                                           , NumCycles  , CycleCount , TotalCycleCount ");
#else      
      printf("\n*************,**************** Iteration%02d,*************************,**********************,*******************,******************\n",ccIterator);
      printf("Function Name                                           , NumCycles  , CycleCount , ClkTime    , TotalCycleCount      , TotalClkTime");
#endif      
#endif
    totalL2Cycles = gMacStats.pStats->ccStats[cellIndex][ccIterator].totalL2Cycles;

    #ifdef TDD_CONFIG
    UInt64 clkTime = 0;
    #ifdef DL_DATA_SEPARATION
         UInt64 clkTime = 0;
    #endif
    #else
        #ifdef DL_UL_SPLIT
        #ifdef GETSTATS_EL_CLOCKTIME
            UInt64 clkTime = 0;
        #endif
        #else
            UInt64 clkTime = 0;/*SPR 19428 +-*/
        #endif
#endif


#ifdef TDD_CONFIG
#ifdef DL_UL_SPLIT_TDD

    totalMacCycleCount = 0;
    totalMacClkTime = 0;
    totalMacCycles = 0;
    for(funcCounter = 0; funcCounter < MAX_MAC_RLC_CC_FUNC; funcCounter++)
    {
        if(funcCounter == PROCESS_RX_UNTIL_SUBFRAME_IND ||
           funcCounter == TX_DL_CONTROL_MSG ||
           funcCounter == TX_DL_DATA_MSG ||
           funcCounter == PROCESS_SUB_FRAME_IND_UL ||
           funcCounter == WAIT_FOR_TDD_SUBFRAME_IND ||
           funcCounter == PROCESS_START_PHY
	   //funcCounter == NORMAL_MODE_FOR_MAC_EXEC_SCH
	   )
        {
	    totalCycleCount = gMacStats.pStats->ccStats[ccIterator].cycleCountStats[funcCounter].totalFuncCycleCount - gMacStats.pStats->ccStats[ccIterator].socketCCStats[funcCounter].totalFuncCycleCount;
#ifdef GETSTATS_EL_CLOCKTIME
            totalClkTimeInMicroSec = gMacStats.pStats->ccStats[ccIterator].cycleCountStats[funcCounter].totalFuncClkTimeInMicroSec - gMacStats.pStats->ccStats[ccIterator].socketCCStats[funcCounter].totalFuncClkTimeInMicroSec;
#endif
	}
        else
        {
	    totalCycleCount = gMacStats.pStats->ccStats[ccIterator].cycleCountStats[funcCounter].totalFuncCycleCount;
#ifdef GETSTATS_EL_CLOCKTIME
	    totalClkTimeInMicroSec = gMacStats.pStats->ccStats[ccIterator].cycleCountStats[funcCounter].totalFuncClkTimeInMicroSec;
#endif
        }
	numCycles = gMacStats.pStats->ccStats[ccIterator].cycleCountStats[funcCounter].numCycles;
        if(numCycles != 0)
        {
	    cycleCount = totalCycleCount/numCycles;
#ifdef GETSTATS_EL_CLOCKTIME
	    clkTime = totalClkTimeInMicroSec/numCycles;
#endif
        }
        else
	    cycleCount = 0;

        totalMacCycleCount+= totalCycleCount;
#ifdef GETSTATS_EL_CLOCKTIME
        totalMacClkTime+= totalClkTimeInMicroSec;
#endif
        totalMacCycles+= numCycles;
#ifndef GETSTATS_EL_CLOCKTIME
        printf("\n%-55s , %-10llu , %-10llu , %-20llu  ",MAC_CC_FUNC_STRING[funcCounter], numCycles, cycleCount ,totalCycleCount);
#else
        printf("\n%-55s , %-10llu , %-10llu , %-10llu , %-20llu , %llu  ",MAC_CC_FUNC_STRING[funcCounter], numCycles, cycleCount , clkTime,totalCycleCount,totalClkTimeInMicroSec);
#endif


     }
     macCycleCount = totalMacCycleCount/totalL2Cycles;
#ifdef GETSTATS_EL_CLOCKTIME
     macClkTime = totalMacClkTime/totalL2Cycles;
#endif
     totalMacRlcCycleCount += totalMacCycleCount;
#ifdef GETSTATS_EL_CLOCKTIME
     totalMacRlcClkTime += totalMacClkTime ;
#endif
     totalMacRlcCycles += totalMacCycles;

#else


    printf("\nFunction Name,                                      NumCycles,    CycleCount,    ClkTime,     TotalCycleCount,   TotalClkTime\n");
    for(funcCounter = 0; funcCounter < MAX_LEG_CC_FUNC; funcCounter++)
    {
       printf("\n"); 
        totalCycleCount = gMacStats.pStats->ccStats[cellIndex][ccIterator].legCCStats[funcCounter].
            totalFuncCycleCount;

        totalClkTimeInMicroSec = gMacStats.pStats->ccStats[cellIndex][ccIterator].legCCStats[funcCounter].
            totalFuncClkTimeInMicroSec;

        numCycles = gMacStats.pStats->ccStats[cellIndex][ccIterator].legCCStats[funcCounter].
            numCycles;
        if(numCycles != 0)
        {
            cycleCount = totalCycleCount/numCycles;
            clkTime = totalClkTimeInMicroSec/numCycles; 
        }
        else
       	/* SPR 10852 fix start */
        {
            cycleCount = 0;
            clkTime = 0;
        }    
    	/* SPR 10852 fix end */

        totalLegCycleCount+= totalCycleCount;
        totalLegClkTime+= totalClkTimeInMicroSec;
        totalLegCycles+= numCycles;
        printf("\n%-55s , %-10llu , %-10llu , %-10llu , %-20llu , %llu ",
                LEG_CC_FUNC_STRING[funcCounter], numCycles, cycleCount, clkTime, 
                totalCycleCount,totalClkTimeInMicroSec);
    }
    macCycleCount = totalLegCycleCount/totalL2Cycles;
    macClkTime = totalLegClkTime/totalL2Cycles;
    printf("\n##################################################################################\n");     
    printf("\n%-55s , %-10llu , %-10llu , %-10llu , %-20llu , %llu \n","MAC",totalLegCycles,
            macCycleCount, macClkTime, totalLegCycleCount, totalLegClkTime);      
    printf("\n##################################################################################\n");    
#endif


#ifdef DL_DATA_SEPARATION
    for(funcCounter = 0; funcCounter < MAX_ENC_CC_FUNC; funcCounter++)
    { 
        totalCycleCount = gMacStats.pStats->ccStats[cellIndex][ccIterator].encoderCCStats[funcCounter].
            totalFuncCycleCount;
        totalClkTimeInMicroSec = gMacStats.pStats->ccStats[cellIndex][ccIterator].encoderCCStats[funcCounter].
            totalFuncClkTimeInMicroSec;
        numCycles = gMacStats.pStats->ccStats[cellIndex][ccIterator].encoderCCStats[funcCounter].
            numCycles;
        if(numCycles != 0)
        {
            cycleCount = totalCycleCount/numCycles;
            clkTime = totalClkTimeInMicroSec/numCycles; 
        }
        else
       	/* SPR 10852 fix start */
        {
            cycleCount = 0;
            clkTime = 0;
        }    
    	/* SPR 10852 fix end */

        totalEncoderCycleCount+= totalCycleCount;
        totalEncoderClkTime+= totalClkTimeInMicroSec;
        totalEncoderCycles+= numCycles;

        printf("\n%-55s , %-10llu , %-10llu , %-10llu , %-20llu , %llu ",
                ENCODER_CC_FUNC_STRING[funcCounter], numCycles, cycleCount, clkTime, 
                totalCycleCount,totalClkTimeInMicroSec);
    }

    encoderCycleCount = totalEncoderCycleCount/totalL2Cycles;
    encoderClkTime = totalEncoderClkTime/totalL2Cycles;
    printf("\n##################################################################################\n");     
    printf("\n%-55s , %-10llu , %-10llu , %-10llu , %-20llu , %llu  \n","ENCODER",totalEncoderCycles,
            encoderCycleCount, encoderClkTime, totalEncoderCycleCount, totalEncoderClkTime);      
    printf("\n##################################################################################\n");     


    printf("\nFunction Name,                                      NumCycles,    CycleCount,    ClkTime,     TotalCycleCount,   TotalClkTime\n");

    totalCycleCount = 0;
    totalClkTimeInMicroSec = 0;
    cycleCount = 0;
    numCycles = 0;
#endif

#else
#ifdef DL_UL_SPLIT
    for(threadCounter = 0; threadCounter < NUM_OF_EXEC_FLOWS ; threadCounter++)
    {
#ifndef GETSTATS_EL_CLOCKTIME
      printf("\n\n********************, THREAD NAME: %-20s ,************,*************\n",excecutionFlowVar[threadCounter]);
#else
      printf("\n\n********************, THREAD NAME: %-20s ,************,************,**********************,***********************\n",excecutionFlowVar[threadCounter]);
#endif
        totalMacCycleCount = 0;
        totalMacClkTime = 0;
        totalMacCycles = 0;

        for(funcCounter = 0; funcCounter < executionFlowCreationParamArr_g[threadCounter].threadArguments.executionLegCount; funcCounter++)
        {
            if(funcCounter == PROCESS_RX_UNTIL_SUBFRAME_IND ||
                    funcCounter == TX_DL_CONTROL_MSG ||
                    funcCounter == TX_DL_DATA_MSG ||
                    funcCounter == PROCESS_START_PHY
//                    funcCounter == NORMAL_MODE_FOR_MAC_EXEC_SCH
                    )
            {
                totalCycleCount = gMacStats.pStats->ccStats[cellIndex][ccIterator].cycleCountStats[executionFlowCreationParamArr_g[threadCounter].threadArguments.executionLegArr[funcCounter].executionLeg].totalFuncCycleCount - gMacStats.pStats->ccStats[cellIndex][ccIterator].socketCCStats[executionFlowCreationParamArr_g[threadCounter].threadArguments.executionLegArr[funcCounter].executionLeg].
                    totalFuncCycleCount;
#ifdef GETSTATS_EL_CLOCKTIME
                totalClkTimeInMicroSec = gMacStats.pStats->ccStats[cellIndex][ccIterator].cycleCountStats[executionFlowCreationParamArr_g[threadCounter].threadArguments.executionLegArr[funcCounter].executionLeg].
                    totalFuncClkTimeInMicroSec - gMacStats.pStats->ccStats[cellIndex][ccIterator].socketCCStats[executionFlowCreationParamArr_g[threadCounter].threadArguments.executionLegArr[funcCounter].executionLeg].
                    totalFuncClkTimeInMicroSec;
#endif
            }
            else
            {
                totalCycleCount = gMacStats.pStats->ccStats[cellIndex][ccIterator].cycleCountStats[executionFlowCreationParamArr_g[threadCounter].threadArguments.executionLegArr[funcCounter].executionLeg].
                    totalFuncCycleCount;

#ifdef GETSTATS_EL_CLOCKTIME
                totalClkTimeInMicroSec = gMacStats.pStats->ccStats[cellIndex][ccIterator].cycleCountStats[executionFlowCreationParamArr_g[threadCounter].threadArguments.executionLegArr[funcCounter].executionLeg].
                    totalFuncClkTimeInMicroSec;

#endif
            }

            numCycles = gMacStats.pStats->ccStats[cellIndex][ccIterator].cycleCountStats[executionFlowCreationParamArr_g[threadCounter].threadArguments.executionLegArr[funcCounter].executionLeg].
                numCycles;
            if(numCycles != 0)
            {
                cycleCount = totalCycleCount/numCycles;
#ifdef GETSTATS_EL_CLOCKTIME
                clkTime = totalClkTimeInMicroSec/numCycles;
#endif
            }
            else
       	/* SPR 10852 fix start */
        {
            cycleCount = 0;
#ifdef GETSTATS_EL_CLOCKTIME
            clkTime = 0;
#endif
        }    
    	/* SPR 10852 fix end */

            totalMacCycleCount+= totalCycleCount;
#ifdef GETSTATS_EL_CLOCKTIME

            totalMacClkTime+= totalClkTimeInMicroSec;
#endif
            totalMacCycles+= numCycles;
#ifndef GETSTATS_EL_CLOCKTIME
            printf("\n%-55s , %-10llu , %-10llu , %-20llu  ",
                    MAC_CC_FUNC_STRING[executionFlowCreationParamArr_g[threadCounter].threadArguments.executionLegArr[funcCounter].executionLeg], numCycles, cycleCount ,
                    totalCycleCount);
#else            
            printf("\n%-55s , %-10llu , %-10llu , %-10llu , %-20llu , %llu  ",
                    MAC_CC_FUNC_STRING[executionFlowCreationParamArr_g[threadCounter].threadArguments.executionLegArr[funcCounter].executionLeg], numCycles, cycleCount , clkTime,
                    totalCycleCount,totalClkTimeInMicroSec);
#endif


        }
        macCycleCount = totalMacCycleCount/totalL2Cycles;
        macClkTime = totalMacClkTime/totalL2Cycles;
        totalMacRlcCycleCount += totalMacCycleCount;
        totalMacRlcClkTime += totalMacClkTime ;
        totalMacRlcCycles += totalMacCycles;
    }
#else
   	/* SPR 10852 fix start */
    printf("\n%-55s , %-10s , %-10s , %-10s , %-20s , %s\n",
    	   "Function Name", "NumCycles", "CycleCount", "ClkTime", "TotalCycleCount", "TotalClkTime");
   	/* SPR 10852 fix end */

    for(funcCounter = 0; funcCounter < MAX_MAC_CC_FUNC; funcCounter++)
    { 
        if(funcCounter == MAC_PROCESS_RX_UNTIL_SUBFRAME_IND ||
           funcCounter == MAC_TX_DL_CONTROL_MSG ||
#ifndef DL_DATA_SEPARATION 
           funcCounter == MAC_TX_DL_DATA_MSG ||
#else
           funcCounter == PROCESS_MAC_TX_DL_DATA_MSG ||
#endif
           funcCounter == MAC_PROCESS_START_PHY ||
           funcCounter == NORMAL_MODE_FOR_MAC_EXEC_SCH)
        {
          totalCycleCount = gMacStats.pStats->ccStats[cellIndex][ccIterator].macCCStats[funcCounter].
            totalFuncCycleCount - gMacStats.pStats->ccStats[cellIndex][ccIterator].socketCCStats[funcCounter].
            totalFuncCycleCount;

          totalClkTimeInMicroSec = gMacStats.pStats->ccStats[cellIndex][ccIterator].macCCStats[funcCounter].
            totalFuncClkTimeInMicroSec - gMacStats.pStats->ccStats[cellIndex][ccIterator].socketCCStats[funcCounter].
            totalFuncClkTimeInMicroSec;
        }
        else
        {
        totalCycleCount = gMacStats.pStats->ccStats[cellIndex][ccIterator].macCCStats[funcCounter].
            totalFuncCycleCount;

        totalClkTimeInMicroSec = gMacStats.pStats->ccStats[cellIndex][ccIterator].macCCStats[funcCounter].
            totalFuncClkTimeInMicroSec;

        }

        numCycles = gMacStats.pStats->ccStats[cellIndex][ccIterator].macCCStats[funcCounter].
            numCycles;
        if(numCycles != 0)
        {
            cycleCount = totalCycleCount/numCycles;
            clkTime = totalClkTimeInMicroSec/numCycles; 
        }
        else
       	/* SPR 10852 fix start */
        {
            cycleCount = 0;
            clkTime = 0;
        }    
    	/* SPR 10852 fix end */

        totalMacCycleCount+= totalCycleCount;
        totalMacClkTime+= totalClkTimeInMicroSec;
        totalMacCycles+= numCycles;
             
        printf("\n%-55s , %-10llu , %-10llu , %-10llu , %-20llu , %llu  ",
                MAC_CC_FUNC_STRING[funcCounter], numCycles, cycleCount , clkTime, 
                totalCycleCount,totalClkTimeInMicroSec);
       
    }
    macCycleCount = totalMacCycleCount/totalL2Cycles;
    macClkTime = totalMacClkTime/totalL2Cycles;
    printf("\n##################################################################################\n");     
    printf("\n%-55s , %-10llu , %-10llu , %-10llu , %-20llu , %llu  \n","MAC",totalMacCycles,
            macCycleCount, macClkTime, totalMacCycleCount, totalMacClkTime);      
    printf("\n##################################################################################\n");     

   	/* SPR 10852 fix start */
    printf("\n%-55s , %-10s , %-10s , %-10s , %-20s , %s\n",
    	   "Function Name", "NumCycles", "CycleCount", "ClkTime", "TotalCycleCount", "TotalClkTime");
   	/* SPR 10852 fix end */
    totalCycleCount = 0;
    totalClkTimeInMicroSec = 0;
    cycleCount = 0;
    numCycles = 0;

#ifdef DL_DATA_SEPARATION
    for(funcCounter = 0; funcCounter < MAX_ENC_CC_FUNC; funcCounter++)
    { 
        totalCycleCount = gMacStats.pStats->ccStats[cellIndex][ccIterator].encoderCCStats[funcCounter].
            totalFuncCycleCount;
        totalClkTimeInMicroSec = gMacStats.pStats->ccStats[cellIndex][ccIterator].encoderCCStats[funcCounter].
            totalFuncClkTimeInMicroSec;
        numCycles = gMacStats.pStats->ccStats[cellIndex][ccIterator].encoderCCStats[funcCounter].
            numCycles;
        if(numCycles != 0)
        {
            cycleCount = totalCycleCount/numCycles;
            clkTime = totalClkTimeInMicroSec/numCycles; 
        }
        else
       	/* SPR 10852 fix start */
        {
            cycleCount = 0;
            clkTime = 0;
        }    
    	/* SPR 10852 fix end */

        totalEncoderCycleCount+= totalCycleCount;
        totalEncoderClkTime+= totalClkTimeInMicroSec;
        totalEncoderCycles+= numCycles;

        printf("\n%-55s , %-10llu , %-10llu , %-10llu , %-20llu , %llu ",
                ENCODER_CC_FUNC_STRING[funcCounter], numCycles, cycleCount, clkTime, 
                totalCycleCount,totalClkTimeInMicroSec);
    }

    encoderCycleCount = totalEncoderCycleCount/totalL2Cycles;
    encoderClkTime = totalEncoderClkTime/totalL2Cycles;
    printf("\n##################################################################################\n");     
    printf("\n%-55s , %-10llu , %-10llu , %-10llu , %-20llu , %llu  \n","ENCODER",totalEncoderCycles,
            encoderCycleCount, encoderClkTime, totalEncoderCycleCount, totalEncoderClkTime);      
    printf("\n##################################################################################\n");     


   	/* SPR 10852 fix start */
    printf("\n%-55s , %-10s , %-10s , %-10s , %-20s , %s\n",
    	   "Function Name", "NumCycles", "CycleCount", "ClkTime", "TotalCycleCount", "TotalClkTime");
   	/* SPR 10852 fix end */
    totalCycleCount = 0;
    totalClkTimeInMicroSec = 0;
    cycleCount = 0;
    numCycles = 0;
#endif

    for(funcCounter = 0; funcCounter < MAX_RLC_CC_FUNC; funcCounter++)
    { 
        totalCycleCount = gMacStats.pStats->ccStats[cellIndex][ccIterator].rlcCCStats[funcCounter].
            totalFuncCycleCount;
        totalClkTimeInMicroSec = gMacStats.pStats->ccStats[cellIndex][ccIterator].rlcCCStats[funcCounter].
            totalFuncClkTimeInMicroSec;
        numCycles = gMacStats.pStats->ccStats[cellIndex][ccIterator].rlcCCStats[funcCounter].
            numCycles;
        if(numCycles != 0)
        {
            cycleCount = totalCycleCount/numCycles;
            clkTime = totalClkTimeInMicroSec/numCycles; 
        }
        else
       	/* SPR 10852 fix start */
        {
            cycleCount = 0;
            clkTime = 0;
        }    
    	/* SPR 10852 fix end */

        totalRlcCycleCount+= totalCycleCount;
        totalRlcClkTime+= totalClkTimeInMicroSec;
        totalRlcCycles+= numCycles;

        printf("\n%-55s , %-10llu , %-10llu , %-10llu , %-20llu , %llu ",
                RLC_CC_FUNC_STRING[funcCounter], numCycles, cycleCount, clkTime, 
                totalCycleCount,totalClkTimeInMicroSec);
    }
    rlcCycleCount = totalRlcCycleCount/totalL2Cycles;
    rlcClkTime = totalRlcClkTime/totalL2Cycles;
    printf("\n##################################################################################\n");     
    printf("\n%-55s , %-10llu , %-10llu , %-10llu , %-20llu , %llu  \n","RLC",totalRlcCycles,
            rlcCycleCount, rlcClkTime, totalRlcCycleCount, totalRlcClkTime);      
    printf("\n##################################################################################\n");     


   	/* SPR 10852 fix start */
    printf("\n%-55s , %-10s , %-10s , %-10s , %-20s , %s\n",
    	   "Function Name", "NumCycles", "CycleCount", "ClkTime", "TotalCycleCount", "TotalClkTime");
   	/* SPR 10852 fix end */
#endif
    totalCycleCount = 0;
    totalClkTimeInMicroSec = 0;
    cycleCount = 0;
    numCycles = 0;
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))

#ifndef GETSTATS_EL_CLOCKTIME
      printf("\n\n********************, THREAD NAME: %-20s ,************,*************\n","PDCP + GTPU");
#else
      printf("\n\n********************, THREAD NAME: %-20s ,************,************,**********************,***********************\n","PDCP + GTPU");
#endif
#endif
    for(funcCounter = 0; funcCounter < MAX_PDCP_CC_FUNC; funcCounter++)
    {
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
        if(funcCounter == PDCP_PROCESS_RX || funcCounter == NORMAL_MODE_FOR_MAC_EXEC_SCH )
#else
            if(funcCounter == PDCP_PROCESS_RX )
#endif
            {
#ifdef DL_UL_SPLIT_TDD
                if (funcCounter == NORMAL_MODE_FOR_MAC_EXEC_SCH)
                { 
                    totalCycleCount = gMacStats.pStats->ccStats[cellIndex][ccIterator].pdcpCCStats[funcCounter].
                        totalFuncCycleCount - gMacStats.pStats->ccStats[cellIndex][ccIterator].socketCCStats[MAC_SOCKET_NORMAL_MODE_RECV_FROM].
                        totalFuncCycleCount;
#ifndef OVERSHOOT_DEBUG_STATS
                    totalClkTimeInMicroSec = gMacStats.pStats->ccStats[cellIndex][ccIterator].pdcpCCStats[funcCounter].
                        totalFuncClkTimeInMicroSec - gMacStats.pStats->ccStats[cellIndex][ccIterator].socketCCStats[MAC_SOCKET_NORMAL_MODE_RECV_FROM].
                        totalFuncClkTimeInMicroSec;
#endif
                }
                else
#endif
                {   
                    totalCycleCount = gMacStats.pStats->ccStats[cellIndex][ccIterator].pdcpCCStats[funcCounter].
                        totalFuncCycleCount - gMacStats.pStats->ccStats[cellIndex][ccIterator].socketCCStats[funcCounter].
                        totalFuncCycleCount;
#ifndef OVERSHOOT_DEBUG_STATS
                    totalClkTimeInMicroSec = gMacStats.pStats->ccStats[cellIndex][ccIterator].pdcpCCStats[funcCounter].
                        totalFuncClkTimeInMicroSec - gMacStats.pStats->ccStats[cellIndex][ccIterator].socketCCStats[funcCounter].
                        totalFuncClkTimeInMicroSec;
#endif
                }
            }
            else
            {
                totalCycleCount = gMacStats.pStats->ccStats[cellIndex][ccIterator].pdcpCCStats[funcCounter].
                    totalFuncCycleCount;
#ifndef OVERSHOOT_DEBUG_STATS
                totalClkTimeInMicroSec = gMacStats.pStats->ccStats[cellIndex][ccIterator].pdcpCCStats[funcCounter].
                    totalFuncClkTimeInMicroSec;
#endif

            }
        numCycles = gMacStats.pStats->ccStats[cellIndex][ccIterator].pdcpCCStats[funcCounter].
            numCycles;
        if(numCycles != 0)
        {
            cycleCount = totalCycleCount/numCycles;
#if (!defined(DL_UL_SPLIT)|| defined(GETSTATS_EL_CLOCKTIME))
            clkTime = totalClkTimeInMicroSec/numCycles; 
#endif
        }
        else
            /* SPR 10852 fix start */
        {
            cycleCount = 0;
            clkTime = 0;
        }    
        /* SPR 10852 fix end */

        totalPdcpCycleCount+= totalCycleCount;
        totalPdcpClkTime+= totalClkTimeInMicroSec;
        totalPdcpCycles+= numCycles;
#if (!defined(DL_UL_SPLIT)|| defined(GETSTATS_EL_CLOCKTIME))
        printf("\n%-55s , %-10llu , %-10llu , %-10llu , %-20llu , %llu  ",
                PDCP_CC_FUNC_STRING[funcCounter], numCycles, cycleCount, clkTime, 
                totalCycleCount,totalClkTimeInMicroSec);
#else
        printf("\n%-55s , %-10llu , %-10llu , %-20llu  ",
                PDCP_CC_FUNC_STRING[funcCounter], numCycles, cycleCount, 
                totalCycleCount);
#endif

    }

    pdcpCycleCount = totalPdcpCycleCount/totalL2Cycles;
    pdcpClkTime = totalPdcpClkTime/totalL2Cycles;
    /* SPR 10852 fix start */
    printf("\n##################################################################################");
    printf("\n%-55s , %-10llu , %-10llu , %-10llu , %-20llu , %llu", "    PDCP    ", totalPdcpCycles,
            pdcpCycleCount, pdcpClkTime, totalPdcpCycleCount, totalPdcpClkTime);
    printf("\n##################################################################################\n");
    /* SPR 10852 fix end */
#endif
/* Coverity # 62456, 62457 */
#ifdef TDD_CONFIG
#ifdef DL_UL_SPLIT_TDD
    totalL2CycleCount = totalMacRlcCycleCount + totalPdcpCycleCount;
    totalL2ClkTime = totalMacRlcClkTime + totalPdcpClkTime;

#else
    totalL2CycleCount = totalLegCycleCount;
    totalL2ClkTime = totalLegClkTime;  
#endif 
#else
#ifdef DL_UL_SPLIT
    totalL2CycleCount = totalMacRlcCycleCount + totalPdcpCycleCount;
    totalL2ClkTime = totalMacRlcClkTime + totalPdcpClkTime;   
#endif
#ifndef DL_UL_SPLIT
    totalL2CycleCount = totalMacCycleCount + totalRlcCycleCount + 
        totalPdcpCycleCount;
    totalL2ClkTime = totalMacClkTime + totalRlcClkTime + totalPdcpClkTime;   
#endif
#endif

#if (defined(DL_UL_SPLIT_TDD) || defined(FDD_CONFIG))
    totalCycleCount = 0;
    totalClkTimeInMicroSec = 0;
    cycleCount = 0;
    numCycles = 0;
#ifndef GETSTATS_EL_CLOCKTIME
      printf("\n\n********************, THREAD NAME: %-20s ,************,*************\n","SOCKET");
#else
      printf("\n\n********************, THREAD NAME: %-20s ,************,************,**********************,***********************\n","SOCKET");
#endif
    //Socket Functions Cycle Count printing
    for(funcCounter = 0; funcCounter < MAX_SOCKET_CC_FUNC; funcCounter++)
    { 
        if(funcCounter == MAC_SOCKET_RX_UNTIL_SUBFRAME_IND ||
           funcCounter == PDCP_SOCKET_PROCESS_RX ||
           funcCounter == MAC_SOCKET_TX_DL_CONTROL_MSG ||
           funcCounter == MAC_SOCKET_TX_DL_DATA_MSG ||
           funcCounter == MAC_SOCKET_START_PHY || 
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
           funcCounter == MAC_SOCKET_OAM_AND_RRC_MSG || 
#endif
#ifdef DL_UL_SPLIT_TDD
           funcCounter == MAC_SOCKET_SUB_FRAME_IND_UL || 
           funcCounter == MAC_SOCKET_WAIT_FOR_TDD_SUBFRAME_IND || 
#endif
           funcCounter == MAC_SOCKET_NORMAL_MODE_RECV_FROM)
        {
        totalCycleCount = gMacStats.pStats->ccStats[cellIndex][ccIterator].socketCCStats[funcCounter].
            totalFuncCycleCount; 

#ifndef OVERSHOOT_DEBUG_STATS
        totalClkTimeInMicroSec = gMacStats.pStats->ccStats[cellIndex][ccIterator].socketCCStats[funcCounter].
            totalFuncClkTimeInMicroSec;
#endif
         i++;
        }
        else
            continue;

        numCycles = gMacStats.pStats->ccStats[cellIndex][ccIterator].socketCCStats[funcCounter].
            numCycles;
        if(numCycles != 0)
        {
            cycleCount = totalCycleCount/numCycles;
#if (!defined(DL_UL_SPLIT)|| defined(GETSTATS_EL_CLOCKTIME))
            clkTime = totalClkTimeInMicroSec/numCycles; 
#endif
        }
        else
            cycleCount = 0;

        totalMacCycleCount+= totalCycleCount;
        totalMacClkTime+= totalClkTimeInMicroSec;
        totalMacCycles+= numCycles;
             
#if (!defined(DL_UL_SPLIT)|| defined(GETSTATS_EL_CLOCKTIME))
        printf("%-55s , %-10llu , %-10llu , %-10llu , %-20llu , %llu \n",
                SOCKET_CC_FUNC_STRING[i-1], numCycles, cycleCount , clkTime, 
                totalCycleCount,totalClkTimeInMicroSec);
#else
        printf("%-55s , %-10llu , %-10llu , %-20llu \n",
                SOCKET_CC_FUNC_STRING[i-1], numCycles, cycleCount ,totalCycleCount);
#endif
       
    }
#endif
}
/* CA Stats Changes End */
#endif

/*spr 22474 start*/
    /*CSPL Code removed*/
/*spr 22474 end*/

#ifdef ACK_NACK_COUNT_STATS
void printDebugStats()
{
 UInt32 acks =0 , nacks =0, ueIndex = 0;
 printf("=====HARQ====\n");
 if(gMacStats.pStats->debugStats.Mimo_NonMimo_Flag == 2)
 {
     printf("***MIMO****\n");

     for( ueIndex =0; ueIndex < MAX_UE_SUPPORTED; ueIndex ++)
     {
        acks = gMacStats.pStats->debugStats.TBOneAckNack[ueIndex].ackCount;
        nacks = gMacStats.pStats->debugStats.TBOneAckNack[ueIndex].nackCount;
        if( (acks != 0)||(nacks != 0))
        printf("TB_ONE: UeIndex:%d  ,  No of ACK: %d   ,  No of NACK: %d  \n",ueIndex,acks,nacks);
      } 
     for( ueIndex =0; ueIndex < MAX_UE_SUPPORTED; ueIndex ++)
     {
        acks = gMacStats.pStats->debugStats.TBTwoAckNack[ueIndex].ackCount;
        nacks = gMacStats.pStats->debugStats.TBTwoAckNack[ueIndex].nackCount;
        if( (acks != 0)||(nacks != 0))
        printf("TB_TWO: UeIndex:%d  ,  No of ACK: %d   ,  No of NACK: %d  \n",ueIndex,acks,nacks);
    
      }

   }
   else 
    if(gMacStats.pStats->debugStats.Mimo_NonMimo_Flag == 0)
    {
      printf("***NON_MIMO****\n");
      for( ueIndex =0; ueIndex < MAX_UE_SUPPORTED; ueIndex ++)
      {
         acks = gMacStats.pStats->debugStats.TBOneAckNack[ueIndex].ackCount;
         nacks = gMacStats.pStats->debugStats.TBOneAckNack[ueIndex].nackCount;
         if( (acks != 0)||(nacks != 0))
         printf("UeIndex:%d  ,  No of ACK: %d   ,  No of NACK: %d  \n",ueIndex,acks,nacks);
       } 
     }
 }    


#endif

/* CA Stats Changes Start */
/* +- SPR 17777 */
void printPerformanceStats(void)
/* +- SPR 17777 */
{
    InternalCellIndex cellIndex = 0;
    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
    printf("###################### Cell [%d] #####################\n", cellIndex);
#if (defined (DL_UL_SPLIT) || defined (DL_UL_SPLIT_TDD))
    UInt32 maxSplitCount = 0;
    for (maxSplitCount = 0; maxSplitCount < MAX_OVERSHOOT_STATS; maxSplitCount++)
    {
        unsigned long long  totalTime = 0;
        unsigned long long  secs = 0;
        unsigned long long  msecs = 0;
        unsigned long long  usecs = 0;
        unsigned long long  nsecs = 0;
        unsigned long long  highestTime = 0;
        //  unsigned int countBw0_499 = 0;
        //   unsigned int countBw500_799 = 0;
        //  unsigned int countBw800_999 = 0;
        //unsigned int countGrtrThan1000 = 0;
        int                 i = 0;
        printf("\n********Performance Stats for %s for Cell %d************ \n",perfStatArr[maxSplitCount], cellIndex );
        for (i = 0; i < SFI_PROFILE_COUNT; i++)
        {
            totalTime += gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].tickTime[i];
            if (gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].tickTime[i] > highestTime)
            {
                highestTime = gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].tickTime[i];
            }
        }
        totalTime = totalTime/SFI_PROFILE_COUNT;
        usecs = totalTime / 1000;
        nsecs = totalTime % 1000;
        msecs = usecs / 1000;
        usecs = usecs % 1000;
        secs = msecs / 1000;
        msecs = msecs % 1000;
        printf("Average tick time               : %llu.%03llu.%03llu.%03llu\n",
                secs, msecs, usecs, nsecs);
        printf("Number of overshoots            : %u/%llu\n",
                gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].numTickOvershoots,
                gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].numTicks);
        usecs = highestTime / 1000;
        nsecs = highestTime % 1000;
        msecs = usecs / 1000;
        usecs = usecs % 1000;
        secs = msecs / 1000;
        msecs = msecs % 1000;
        printf("Highest tick time               : %llu.%03llu.%03llu.%03llu\n",
                secs, msecs, usecs, nsecs);
        highestTime = 0;
        totalTime = 0;
        for (i = 0; i < PERF_STATS_TIME; i++)
        {
            totalTime += gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].totalProcessingTime[i];
            //   countGrtrThan1000+=gMacStats.pStats->perfStatsSplit[maxSplitCount].numProcessingOvershoots[i];
            // countBw800_999+=gMacStats.pStats->perfStatsSplit[maxSplitCount].countBw800_999[i];
            //  countBw500_799+=gMacStats.pStats->perfStatsSplit[maxSplitCount].countBw500_799[i];
            //  countBw0_499+=gMacStats.pStats->perfStatsSplit[maxSplitCount].countBw0_499[i];
            if (gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].highestTime[i] > highestTime)
            {
                highestTime = gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].highestTime[i];
            }
        }

        if(gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].numProcessings > 
                (SFI_PROFILE_COUNT*PERF_STATS_TIME))
            totalTime = totalTime/(SFI_PROFILE_COUNT*PERF_STATS_TIME);
        else
            if(gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].numProcessings != 0)
            {
                totalTime = totalTime/gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].numProcessings;
            }
        usecs = totalTime / 1000;
        nsecs = totalTime % 1000;
        msecs = usecs / 1000;
        usecs = usecs % 1000;
        secs = msecs / 1000;
        msecs = msecs % 1000;
        printf("Average processing time (Last 1000 sec): %llu.%03llu.%03llu.%03llu\n",
                secs, msecs, usecs, nsecs);
        usecs = highestTime / 1000;
        nsecs = highestTime % 1000;
        msecs = usecs / 1000;
        usecs = usecs % 1000;
        secs = msecs / 1000;
        msecs = msecs % 1000;
        printf("Highest processing time(Last 1000 sec)         : %llu.%03llu.%03llu.%03llu\n",
                secs, msecs, usecs, nsecs);


        /*   printf("Number of processing overshoots : %u/%llu\n",
             countGrtrThan1000,
             gMacStats.pStats->perfStatsSplit[maxSplitCount].numProcessings);*/
        printf("Number of processing overshoots : %llu/%llu\n",
                gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].numProcessingOvershoots,
                gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].numProcessings);

        /*    printf("800-999 us : %u/%llu\n",
              countBw800_999,
              gMacStats.pStats->perfStatsSplit[maxSplitCount].numProcessings);*/
        printf("800-999 us : %llu/%llu\n",
                gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].countBw800_999,
                gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].numProcessings);

        /*  printf("500-799 us : %u/%llu\n",
            countBw500_799,
            gMacStats.pStats->perfStatsSplit[maxSplitCount].numProcessings);*/
        printf("500-799 us : %llu/%llu\n",
                gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].countBw500_799,
                gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].numProcessings);

        /*  printf("0-499 us   : %u/%llu\n",
            countBw0_499,
            gMacStats.pStats->perfStatsSplit[maxSplitCount].numProcessings);*/
        printf("0-499 us   : %llu/%llu\n",
                gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].countBw0_499,
                gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].numProcessings);

        highestTime = 0;
        totalTime = 0;
        for (i = 0; i < SFI_PROFILE_COUNT; i++)
        {
            //        printf("Processing time[%d] : %llu\n",i,
            //                gMacStats.pStats->perfStatsSplit[maxSplitCount].processingTime[i]);
            totalTime += gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].processingTime[i];
            if (gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].processingTime[i] > highestTime)
            {
                highestTime = gMacStats.pStats->perfStatsSplit[cellIndex][maxSplitCount].processingTime[i];
            }
        }

        totalTime = totalTime/SFI_PROFILE_COUNT;
        usecs = totalTime / 1000;
        nsecs = totalTime % 1000;
        msecs = usecs / 1000;
        usecs = usecs % 1000;
        secs = msecs / 1000;
        msecs = msecs % 1000;
        printf("Average processing time(in last 1000 ticks) : %llu.%03llu.%03llu.%03llu\n",
                secs, msecs, usecs, nsecs);

        usecs = highestTime / 1000;
        nsecs = highestTime % 1000;
        msecs = usecs / 1000;
        usecs = usecs % 1000;
        secs = msecs / 1000;
        msecs = msecs % 1000;
        printf("Highest processing time(in Last 1000 ticks) : %llu.%03llu.%03llu.%03llu\n",
                secs, msecs, usecs, nsecs);
    }
#else
    unsigned long long  totalTime = 0;
    unsigned long long  secs = 0;
    unsigned long long  msecs = 0;
    unsigned long long  usecs = 0;
    unsigned long long  nsecs = 0;
    unsigned long long  highestTime = 0;/* +- SPR 18490 */
  //  unsigned int countBw0_499 = 0;
  //   unsigned int countBw500_799 = 0;
  //  unsigned int countBw800_999 = 0;
    //unsigned int countGrtrThan1000 = 0;
    int                 i = 0;
    for (i = 0; i < SFI_PROFILE_COUNT; i++)
    {
        totalTime += gMacStats.pStats->perfStats[cellIndex].tickTime[i];
        if (gMacStats.pStats->perfStats[cellIndex].tickTime[i] > highestTime)
        {
            highestTime = gMacStats.pStats->perfStats[cellIndex].tickTime[i];
        }
    }
    totalTime = totalTime/SFI_PROFILE_COUNT;
    usecs = totalTime / 1000;
    nsecs = totalTime % 1000;
    msecs = usecs / 1000;
    usecs = usecs % 1000;
    secs = msecs / 1000;
    msecs = msecs % 1000;
    printf("Average tick time               : %llu.%03llu.%03llu.%03llu\n",
            secs, msecs, usecs, nsecs);
    printf("Number of overshoots            : %u/%llu\n",
            gMacStats.pStats->perfStats[cellIndex].numTickOvershoots,
            gMacStats.pStats->perfStats[cellIndex].numTicks);
    usecs = highestTime / 1000;
    nsecs = highestTime % 1000;
    msecs = usecs / 1000;
    usecs = usecs % 1000;
    secs = msecs / 1000;
    msecs = msecs % 1000;
    printf("Highest tick time               : %llu.%03llu.%03llu.%03llu\n",
            secs, msecs, usecs, nsecs);
    highestTime = 0;
    totalTime = 0;
    for (i = 0; i < PERF_STATS_TIME; i++)
    {
        totalTime += gMacStats.pStats->perfStats[cellIndex].totalProcessingTime[i];
     //   countGrtrThan1000+=gMacStats.pStats->perfStats.numProcessingOvershoots[i];
       // countBw800_999+=gMacStats.pStats->perfStats.countBw800_999[i];
      //  countBw500_799+=gMacStats.pStats->perfStats.countBw500_799[i];
      //  countBw0_499+=gMacStats.pStats->perfStats.countBw0_499[i];
        if (gMacStats.pStats->perfStats[cellIndex].highestTime[i] > highestTime)
        {
            highestTime = gMacStats.pStats->perfStats[cellIndex].highestTime[i];
        }
    }

    if(gMacStats.pStats->perfStats[cellIndex].numProcessings > 
            (SFI_PROFILE_COUNT*PERF_STATS_TIME))
        totalTime = totalTime/(SFI_PROFILE_COUNT*PERF_STATS_TIME);
    else
        if(gMacStats.pStats->perfStats[cellIndex].numProcessings != 0)
	{
          totalTime = totalTime/gMacStats.pStats->perfStats[cellIndex].numProcessings;
        }
    usecs = totalTime / 1000;
    nsecs = totalTime % 1000;
    msecs = usecs / 1000;
    usecs = usecs % 1000;
    secs = msecs / 1000;
    msecs = msecs % 1000;
    printf("Average processing time (Last 1000 sec): %llu.%03llu.%03llu.%03llu\n",
            secs, msecs, usecs, nsecs);
    usecs = highestTime / 1000;
    nsecs = highestTime % 1000;
    msecs = usecs / 1000;
    usecs = usecs % 1000;
    secs = msecs / 1000;
    msecs = msecs % 1000;
    printf("Highest processing time(Last 1000 sec)         : %llu.%03llu.%03llu.%03llu\n",
            secs, msecs, usecs, nsecs);


 /*   printf("Number of processing overshoots : %u/%llu\n",
            countGrtrThan1000,
            gMacStats.pStats->perfStats.numProcessings);*/
    printf("Number of processing overshoots : %llu/%llu\n",
            gMacStats.pStats->perfStats[cellIndex].numProcessingOvershoots,
            gMacStats.pStats->perfStats[cellIndex].numProcessings);

/*    printf("800-999 us : %u/%llu\n",
            countBw800_999,
            gMacStats.pStats->perfStats.numProcessings);*/
   printf("800-999 us : %llu/%llu\n",
            gMacStats.pStats->perfStats[cellIndex].countBw800_999,
            gMacStats.pStats->perfStats[cellIndex].numProcessings);

  /*  printf("500-799 us : %u/%llu\n",
            countBw500_799,
            gMacStats.pStats->perfStats.numProcessings);*/
    printf("500-799 us : %llu/%llu\n",
            gMacStats.pStats->perfStats[cellIndex].countBw500_799,
            gMacStats.pStats->perfStats[cellIndex].numProcessings);

  /*  printf("0-499 us   : %u/%llu\n",
            countBw0_499,
            gMacStats.pStats->perfStats.numProcessings);*/
    printf("0-499 us   : %llu/%llu\n",
            gMacStats.pStats->perfStats[cellIndex].countBw0_499,
            gMacStats.pStats->perfStats[cellIndex].numProcessings);

    highestTime = 0;
    totalTime = 0;
    for (i = 0; i < SFI_PROFILE_COUNT; i++)
    {
        //        printf("Processing time[%d] : %llu\n",i,
        //                gMacStats.pStats->perfStats.processingTime[i]);
        totalTime += gMacStats.pStats->perfStats[cellIndex].processingTime[i];
        if (gMacStats.pStats->perfStats[cellIndex].processingTime[i] > highestTime)
        {
            highestTime = gMacStats.pStats->perfStats[cellIndex].processingTime[i];
        }
    }

    totalTime = totalTime/SFI_PROFILE_COUNT;
    usecs = totalTime / 1000;
    nsecs = totalTime % 1000;
    msecs = usecs / 1000;
    usecs = usecs % 1000;
    secs = msecs / 1000;
    msecs = msecs % 1000;
    printf("Average processing time(in last 1000 ticks) : %llu.%03llu.%03llu.%03llu\n",
            secs, msecs, usecs, nsecs);

    usecs = highestTime / 1000;
    nsecs = highestTime % 1000;
    msecs = usecs / 1000;
    usecs = usecs % 1000;
    secs = msecs / 1000;
    msecs = msecs % 1000;
    printf("Highest processing time(in Last 1000 ticks) : %llu.%03llu.%03llu.%03llu\n",
            secs, msecs, usecs, nsecs);
#endif
#ifdef UTFWK_SIMULATION
    UInt8 qciCounter = 0;
    UInt8 pdbCounter = 0;

    printf("\n\n\n");
    printf("###################### DL Packet Delay profile #####################\n");
    if (gMacStats.pStats->qoSUEAndPerLCTTIStat.numTotDLPkts)
    {    
    printf("Average DL packet delay : %llu\n",
            (gMacStats.pStats->perfStats.aggrgtPdbDelay / gMacStats.pStats->qoSUEAndPerLCTTIStat.numTotDLPkts));
    printf("Highest DL packet delay : %u\n",
            gMacStats.pStats->perfStats.highestDelay);
    printf("Number of packets beyond PDB DL: %llu/%u\n",
            gMacStats.pStats->perfStats.pdbDelayFail, gMacStats.pStats->qoSUEAndPerLCTTIStat.numTotDLPkts);
    printf("DL PdbDelay 0-25ms : %llu/%u\n",
            gMacStats.pStats->perfStats.pdbDelay0_25, gMacStats.pStats->qoSUEAndPerLCTTIStat.numTotDLPkts);
    printf("DL PdbDelay 26-75ms : %llu/%u\n",
            gMacStats.pStats->perfStats.pdbDelay26_75, gMacStats.pStats->qoSUEAndPerLCTTIStat.numTotDLPkts);
    printf("DL PdbDelay 76-125ms : %llu/%u\n",
            gMacStats.pStats->perfStats.pdbDelay76_125, gMacStats.pStats->qoSUEAndPerLCTTIStat.numTotDLPkts);
    printf("DL PdbDelay 126-275ms : %llu/%u\n",
            gMacStats.pStats->perfStats.pdbDelay126_275, gMacStats.pStats->qoSUEAndPerLCTTIStat.numTotDLPkts);
    printf("DL PdbDelay above 275ms : %llu/%u\n",
            gMacStats.pStats->perfStats.pdbDelayAbove275, gMacStats.pStats->qoSUEAndPerLCTTIStat.numTotDLPkts);
    }
    else
    {
        printf("Total no. of DL packet transmission = 0\n\n");
    }    
    
    /* For UL */
    printf("###################### UL Packet Delay profile #####################\n");
    if (gMacStats.pStats->qoSUEAndPerLCTTIStat.numTotULPkts)
    {    
    printf("Average UL packet delay : %llu\n",
            (gMacStats.pStats->ulPerfStats.aggrgtPdbDelay / gMacStats.pStats->qoSUEAndPerLCTTIStat.numTotULPkts));
    printf("Highest UL packet delay : %u\n",
            gMacStats.pStats->ulPerfStats.highestDelay);
    printf("Number of packets beyond PDB UL: %llu/%u\n",
            gMacStats.pStats->ulPerfStats.pdbDelayFail, gMacStats.pStats->qoSUEAndPerLCTTIStat.numTotULPkts);
    printf("UL PdbDelay 0-25ms : %llu/%u\n",
            gMacStats.pStats->ulPerfStats.pdbDelay0_25, gMacStats.pStats->qoSUEAndPerLCTTIStat.numTotULPkts);
    printf("UL PdbDelay 26-75ms : %llu/%u\n",
            gMacStats.pStats->ulPerfStats.pdbDelay26_75, gMacStats.pStats->qoSUEAndPerLCTTIStat.numTotULPkts);
    printf("UL PdbDelay 76-125ms : %llu/%u\n",
            gMacStats.pStats->ulPerfStats.pdbDelay76_125, gMacStats.pStats->qoSUEAndPerLCTTIStat.numTotULPkts);
    printf("UL PdbDelay 126-275ms : %llu/%u\n",
            gMacStats.pStats->ulPerfStats.pdbDelay126_275, gMacStats.pStats->qoSUEAndPerLCTTIStat.numTotULPkts);
    printf("UL PdbDelay above 275ms : %llu/%u\n",
            gMacStats.pStats->ulPerfStats.pdbDelayAbove275, gMacStats.pStats->qoSUEAndPerLCTTIStat.numTotULPkts);
    }
    else
    {
        printf("Total no. of UL packet transmission = 0\n\n");
    }    
    
    printf("\n###################### Packet Delay Per QCI profile #####################\n");
    for (qciCounter = 1; qciCounter < 10; qciCounter++)
    {
        for (pdbCounter = 0; pdbCounter < 5; pdbCounter++)
        {
            if (0 == pdbCounter)
            {
                if (gMacStats.pStats->perfStats.pdbDelayPerQci[qciCounter][pdbCounter] != 0)
                printf ("\n DL PdbDelay 0-25ms for QCI[%u] : %llu\n", qciCounter,gMacStats.pStats->perfStats.pdbDelayPerQci[qciCounter][pdbCounter]);
            }
            else if (1 == pdbCounter)
            {
                if (gMacStats.pStats->perfStats.pdbDelayPerQci[qciCounter][pdbCounter] != 0)
                printf ("\n DL PdbDelay 26-75ms for QCI[%u] : %llu\n", qciCounter,gMacStats.pStats->perfStats.pdbDelayPerQci[qciCounter][pdbCounter]);
            }
            else if (2 == pdbCounter)
            {
                if (gMacStats.pStats->perfStats.pdbDelayPerQci[qciCounter][pdbCounter] != 0)
                printf ("\n DL PdbDelay 76-125ms for QCI[%u] : %llu\n", qciCounter,gMacStats.pStats->perfStats.pdbDelayPerQci[qciCounter][pdbCounter]);
            }
            else if (3 == pdbCounter)
            {
                if (gMacStats.pStats->perfStats.pdbDelayPerQci[qciCounter][pdbCounter] != 0)
                printf ("\n DL PdbDelay 126-275ms for QCI[%u] : %llu\n", qciCounter,gMacStats.pStats->perfStats.pdbDelayPerQci[qciCounter][pdbCounter]);
            }
            else if (4 == pdbCounter)
            {
                if (gMacStats.pStats->perfStats.pdbDelayPerQci[qciCounter][pdbCounter] != 0)
                printf ("\n DL PdbDelay above 275ms for QCI[%u] : %llu\n", qciCounter,gMacStats.pStats->perfStats.pdbDelayPerQci[qciCounter][pdbCounter]);
            }
        }
    }
 /* QOS_PDB_CHANGES*/
    printf("\n###################### UL Packet Delay Per QCI profile #####################\n");
    for (qciCounter = 1; qciCounter < 10; qciCounter++)
    {
        for (pdbCounter = 0; pdbCounter < 5; pdbCounter++)
        {              
             if (gMacStats.pStats->perfStats.pdbUlDelayPerQci[qciCounter][pdbCounter] != 0)
             {
                 switch(pdbCounter)
                 {    
                     case 0:
                         printf ("\n UL PdbDelay 0-25ms for QCI[%u] : %llu\n", qciCounter,gMacStats.pStats->perfStats.pdbUlDelayPerQci[qciCounter][pdbCounter]);
                         break;

                     case 1: 
                         printf ("\n UL PdbDelay 26-75ms for QCI[%u] : %llu\n", qciCounter,gMacStats.pStats->perfStats.pdbUlDelayPerQci[qciCounter][pdbCounter]);
                         break;

                     case 2: 
                         printf ("\n UL PdbDelay 76-125ms for QCI[%u] : %llu\n", qciCounter,gMacStats.pStats->perfStats.pdbUlDelayPerQci[qciCounter][pdbCounter]);
                         break;

                     case 3:                
                         printf ("\n UL PdbDelay 126-275ms for QCI[%u] : %llu\n", qciCounter,gMacStats.pStats->perfStats.pdbUlDelayPerQci[qciCounter][pdbCounter]);
                         break;

                     case 4:                
                         printf ("\n UL PdbDelay above 275ms for QCI[%u] : %llu\n", qciCounter,gMacStats.pStats->perfStats.pdbUlDelayPerQci[qciCounter][pdbCounter]);
                         break;
                     
                    default: 
                         printf("Incorrect value of pdbCounter \n");
                         break; 
                }    
             }         
        }
    }
 /* QOS_PDB_CHANGES*/
    printf("\n\n\n");
    printf("##########################DL GBR THROUGHPUT FACTOR DISTRIBUTION###################\n");

    printf("Total Token period samples : %u\n",gMacStats.pStats->perfStats.totalNumberOfSamples);
    printf("GBR tput factor 0-75% : %llu/%u\n",
            gMacStats.pStats->perfStats.gbrTputFactor_0_75, gMacStats.pStats->perfStats.totalNumberOfSamples);
    printf("GBR tput factor 75-90% : %llu/%u\n",
            gMacStats.pStats->perfStats.gbrTputFactor_75_90,gMacStats.pStats->perfStats.totalNumberOfSamples);
    printf("GBR tput factor 90-100% : %llu/%u\n",
            gMacStats.pStats->perfStats.gbrTputFactor_90_100,gMacStats.pStats->perfStats.totalNumberOfSamples);
    printf("GBR tput factor 100-125% : %llu/%u\n",
            gMacStats.pStats->perfStats.gbrTputFactor_100_125,gMacStats.pStats->perfStats.totalNumberOfSamples);
    printf("GBR tput factor above 125% : %llu/%u\n",
            gMacStats.pStats->perfStats.gbrTputFactor_125,gMacStats.pStats->perfStats.totalNumberOfSamples);
    printf("\n");

    printf("########################## UL GBR THROUGHPUT FACTOR DISTRIBUTION ###################\n");

    printf("Total Token period samples :      %u\n",gMacStats.pStats->ulPerfStats.totalNumberOfSamples);
    printf("GBR tput factor 0-75% : %llu/%u\n",
            gMacStats.pStats->ulPerfStats.gbrTputFactor_0_75,
            gMacStats.pStats->ulPerfStats.totalNumberOfSamples);
    printf("GBR tput factor 75-90% : %llu/%u\n",
            gMacStats.pStats->ulPerfStats.gbrTputFactor_75_90,gMacStats.pStats->ulPerfStats.totalNumberOfSamples);
    printf("GBR tput factor 90-100% : %llu/%u\n",
            gMacStats.pStats->ulPerfStats.gbrTputFactor_90_100,gMacStats.pStats->ulPerfStats.totalNumberOfSamples);
    printf("GBR tput factor 100-125% : %llu/%u\n",
            gMacStats.pStats->ulPerfStats.gbrTputFactor_100_125,gMacStats.pStats->ulPerfStats.totalNumberOfSamples);
    printf("GBR tput factor above 125% : %llu/%u\n",
            gMacStats.pStats->ulPerfStats.gbrTputFactor_125,gMacStats.pStats->ulPerfStats.totalNumberOfSamples);
    printf("\n");

#endif
#ifdef DL_DATA_SEPARATION

/* SPR 8712 changes */    
    unsigned long long  totalTimeEnc = 0;
    unsigned long long  secsEnc = 0;
    unsigned long long  msecsEnc = 0;
    unsigned long long  usecsEnc = 0;
    unsigned long long  nsecsEnc = 0;
    unsigned long long  highestTimeEnc = 0;
    int                 idx = 0;

    printf("\n******** Performance Stats for ENCODER Thread Cell [%d]************ \n", cellIndex);
    for (idx = 0; idx < ENCODER_PERF_STATS_TIME; idx++)
    {
        totalTimeEnc += gMacStats.pStats->encoderPerfStats[cellIndex].totalProcessingTime[idx];
        if (gMacStats.pStats->encoderPerfStats[cellIndex].highestTime[idx] > highestTimeEnc)
        {
            highestTimeEnc = gMacStats.pStats->encoderPerfStats[cellIndex].highestTime[idx];
        }
    }

    if(gMacStats.pStats->encoderPerfStats[cellIndex].numProcessings > 
            (ENCODER_PROFILE_COUNT * ENCODER_PERF_STATS_TIME))
        totalTimeEnc = totalTimeEnc/(ENCODER_PROFILE_COUNT * ENCODER_PERF_STATS_TIME);
    else
        if(gMacStats.pStats->encoderPerfStats[cellIndex].numProcessings != 0)
	{
          totalTimeEnc = totalTimeEnc/gMacStats.pStats->encoderPerfStats[cellIndex].numProcessings;
    }
    usecsEnc = totalTimeEnc / 1000;
    nsecsEnc = totalTimeEnc % 1000;
    msecsEnc = usecsEnc / 1000;
    usecsEnc = usecsEnc % 1000;
    secsEnc = msecsEnc / 1000;
    msecsEnc = msecsEnc % 1000;
    printf("Average processing time (Last 1000000 iteration): %llu.%03llu.%03llu.%03llu\n",
            secsEnc, msecsEnc, usecsEnc, nsecsEnc);
    usecsEnc = highestTimeEnc / 1000;
    nsecsEnc = highestTimeEnc % 1000;
    msecsEnc = usecsEnc / 1000;
    usecsEnc = usecsEnc % 1000;
    secsEnc = msecsEnc / 1000;
    msecsEnc = msecsEnc % 1000;
    printf("Highest processing time(Last 1000000 iteration)         : %llu.%03llu.%03llu.%03llu\n",
            secsEnc, msecsEnc, usecsEnc, nsecsEnc);


    printf("Number of processing overshoots : %llu/%llu\n",
            gMacStats.pStats->encoderPerfStats[cellIndex].numProcessingOvershoots,
            gMacStats.pStats->encoderPerfStats[cellIndex].numProcessings);

   printf("800-999 us : %llu/%llu\n",
            gMacStats.pStats->encoderPerfStats[cellIndex].countBw800_999,
            gMacStats.pStats->encoderPerfStats[cellIndex].numProcessings);

    printf("500-799 us : %llu/%llu\n",
            gMacStats.pStats->encoderPerfStats[cellIndex].countBw500_799,
            gMacStats.pStats->encoderPerfStats[cellIndex].numProcessings);

    printf("0-499 us   : %llu/%llu\n",
            gMacStats.pStats->encoderPerfStats[cellIndex].countBw0_499,
            gMacStats.pStats->encoderPerfStats[cellIndex].numProcessings);

    highestTimeEnc = 0;
    totalTimeEnc = 0;
    for (idx = 0; idx < ENCODER_PROFILE_COUNT; idx++)
    {
        totalTimeEnc += gMacStats.pStats->encoderPerfStats[cellIndex].processingTime[idx];
        if (gMacStats.pStats->encoderPerfStats[cellIndex].processingTime[idx] > highestTimeEnc)
        {
            highestTimeEnc = gMacStats.pStats->encoderPerfStats[cellIndex].processingTime[idx];
        }
    }

    totalTimeEnc = totalTimeEnc/ENCODER_PROFILE_COUNT;
    usecsEnc = totalTimeEnc / 1000;
    nsecsEnc = totalTimeEnc % 1000;
    msecsEnc = usecsEnc / 1000;
    usecsEnc = usecsEnc % 1000;
    secsEnc = msecsEnc / 1000;
    msecsEnc = msecsEnc % 1000;
    printf("Average processing time(in last 1000 iterations) : %llu.%03llu.%03llu.%03llu\n",
            secsEnc, msecsEnc, usecsEnc, nsecsEnc);

    usecsEnc = highestTimeEnc / 1000;
    nsecsEnc = highestTimeEnc % 1000;
    msecsEnc = usecsEnc / 1000;
    usecsEnc = usecsEnc % 1000;
    secsEnc = msecsEnc / 1000;
    msecsEnc = msecsEnc % 1000;
    printf("Highest processing time(in Last 1000 iterations) : %llu.%03llu.%03llu.%03llu\n",
            secsEnc, msecsEnc, usecsEnc, nsecsEnc);
/* SPR 8712 changes */    

#endif
    printf("\n");
    }
}

void printFapiTxParReqStats(InternalCellIndex cellIndex)
{
    printf("Num of messages : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.parReqStats.numMsgs);
    printf("Num of bytes    : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.parReqStats.numMsgs *
            SIZEOF_L1_HDR);
}

void printFapiTxCellConfReqStats(InternalCellIndex cellIndex)
{
    printf("Num of messages : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.cellConfReqStats.numMsgs);
    printf("Num of TLVs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.cellConfReqStats.numTlvs);
    printf("Num of bytes    : %d\n",
            (gMacStats.pStats->phyStats[cellIndex].fapiTxStats.cellConfReqStats.numMsgs *
                (SIZEOF_L1_HDR +
                offsetof(FAPI_phyCellConfigRequest_st, configtlvs)) +
            (gMacStats.pStats->phyStats[cellIndex].fapiTxStats.cellConfReqStats.numTlvs *
                sizeof(FAPI_cellConfig_st))));
}

void printFapiTxStartReqStats(InternalCellIndex cellIndex)
{
    printf("Num of messages : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.startReqStats.numMsgs);
    printf("Num of bytes    : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.startReqStats.numMsgs *
            SIZEOF_L1_HDR);
}

void printFapiTxStopReqStats(InternalCellIndex cellIndex)
{
    printf("Num of messages : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.stopReqStats.numMsgs);
    printf("Num of bytes    : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.stopReqStats.numMsgs *
            SIZEOF_L1_HDR);
}

void printFapiTxUeConfReqStats(InternalCellIndex cellIndex)
{
    printf("Num of messages : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ueConfReqStats.numMsgs);
    printf("Num of TLVs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ueConfReqStats.numTlvs);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ueConfReqStats.numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxDlConfDciStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.dlConfReqStats.dciStats.
            numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.dlConfReqStats.dciStats.
            numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxDlConfBchStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.dlConfReqStats.bchStats.
            numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.dlConfReqStats.bchStats.
            numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxDlConfMchStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.dlConfReqStats.mchStats.
            numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.dlConfReqStats.mchStats.
            numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxDlConfDlschStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.dlConfReqStats.dlschStats.
            numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.dlConfReqStats.dlschStats.
            numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxDlConfPchStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.dlConfReqStats.pchStats.
            numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.dlConfReqStats.pchStats.
            numBytes);
    /* spr 3643 Changes end*/
}
void printFapiTxDlConfReqStats(InternalCellIndex cellIndex)
{
    printf("Num of messages : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.dlConfReqStats.numMsgs);
    printf("Num of DCI      : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.dlConfReqStats.numDci);
    printf("Num of PDU      : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.dlConfReqStats.numPdu);
    /* spr 3643 Changes start*/
    printf("Total bytes     : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.dlConfReqStats.numBytes);
    /* spr 3643 Changes end*/
    printf("----------------------- DCI stats ----------------------\n");
    printFapiTxDlConfDciStats(cellIndex);
    printf("----------------------- BCH stats ----------------------\n");
    printFapiTxDlConfBchStats(cellIndex);
    printf("----------------------- MCH stats ----------------------\n");
    printFapiTxDlConfMchStats(cellIndex);
    printf("----------------------- DLSCH stats ----------------------\n");
    printFapiTxDlConfDlschStats(cellIndex);
    printf("----------------------- PCH stats ----------------------\n");
    printFapiTxDlConfPchStats(cellIndex);
}

void printFapiTxUlConfUlschStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.ulschStats.
            numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.ulschStats.
            numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxUlConfUlschCqiRiStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.
            ulschCqiRiStats.numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.
            ulschCqiRiStats.numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxUlConfUlschHarqStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.
            ulschHarqStats.numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.
            ulschHarqStats.numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxUlConfUlschCqiHarqRiStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.
            ulschCqiHarqRiStats.numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.
            ulschCqiHarqRiStats.numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxUlConfUciCqiStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.uciCqiStats.
            numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.uciCqiStats.
            numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxUlConfUciSrStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.uciSrStats.
            numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.uciSrStats.
            numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxUlConfUciHarqStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.uciHarqStats.
            numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.uciHarqStats.
            numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxUlConfUciSrHarqStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.
            uciSrHarqStats.numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.
            uciSrHarqStats.numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxUlConfUciCqiHarqStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.
            uciCqiHarqStats.numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.
            uciCqiHarqStats.numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxUlConfUciCqiSrStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.
            uciCqiSrStats.numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.
            uciCqiSrStats.numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxUlConfUciCqiSrHarqStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.
            uciCqiSrHarqStats.numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.
            uciCqiSrHarqStats.numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxUlConfSrsStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.srsStats.
            numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.srsStats.
            numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxUlConfReqStats(InternalCellIndex cellIndex)
{
    printf("Num of messages : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.numMsgs);
    printf("Num of PDU      : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.numPdu);
    /* spr 3643 Changes start*/
    printf("Total bytes     : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.ulConfReqStats.numBytes);
    /* spr 3643 Changes end*/
    printf("----------------------- ULSCH stats ----------------------\n");
    printFapiTxUlConfUlschStats(cellIndex);
    printf("--------------------- ULSCH CQI RI stats --------------------\n");
    printFapiTxUlConfUlschCqiRiStats(cellIndex);
    printf("--------------------- ULSCH HARQ stats --------------------\n");
    printFapiTxUlConfUlschHarqStats(cellIndex);
    printf("------------------- ULSCH CQI HARQ RI stats ------------------\n");
    printFapiTxUlConfUlschCqiHarqRiStats(cellIndex);
    printf("----------------------- UCI CQI stats ----------------------\n");
    printFapiTxUlConfUciCqiStats(cellIndex);
    printf("----------------------- UCI SR stats ----------------------\n");
    printFapiTxUlConfUciSrStats(cellIndex);
    printf("----------------------- UCI HARQ stats ----------------------\n");
    printFapiTxUlConfUciHarqStats(cellIndex);
    printf("--------------------- UCI SR HARQ stats --------------------\n");
    printFapiTxUlConfUciSrHarqStats(cellIndex);
    printf("--------------------- UCI CQI HARQ stats --------------------\n");
    printFapiTxUlConfUciCqiHarqStats(cellIndex);
    printf("--------------------- UCI CQI SR stats --------------------\n");
    printFapiTxUlConfUciCqiSrStats(cellIndex);
    printf("------------------- UCI CQI SR HARQ stats ------------------\n");
    printFapiTxUlConfUciCqiSrHarqStats(cellIndex);
    printf("----------------------- SRS stats ----------------------\n");
    printFapiTxUlConfSrsStats(cellIndex);
}

void printFapiTxHiDci0ReqStats(InternalCellIndex cellIndex)
{
    printf("Num of messages : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.hiDci0ReqStats.numMsgs);
    printf("Num of HI       : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.hiDci0ReqStats.numHi);
    printf("Num of DCI0     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.hiDci0ReqStats.numDci0);
    printf("Num of bytes    : %d\n",
            ((gMacStats.pStats->phyStats[cellIndex].fapiTxStats.hiDci0ReqStats.numHi *
                sizeof(FAPI_dlHiPduInfo_st)) +
              (gMacStats.pStats->phyStats[cellIndex].fapiTxStats.hiDci0ReqStats.numDci0 *
                sizeof(FAPI_dlDCIPduInfo_st))));
}

void printFapiTxTxReqDlschStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.txReqStats.dlschStats.
            numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.txReqStats.dlschStats.
            numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxTxReqBchStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.txReqStats.bchStats.
            numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.txReqStats.bchStats.
            numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxTxReqMchStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.txReqStats.mchStats.
            numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.txReqStats.mchStats.
            numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxTxReqPchStats(InternalCellIndex cellIndex)
{
    printf("Num of PDUs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.txReqStats.pchStats.
            numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.txReqStats.pchStats.
            numBytes);
    /* spr 3643 Changes end*/
}

void printFapiTxTxReqStats(InternalCellIndex cellIndex)
{
    printf("Num of messages : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.txReqStats.numMsgs);
    printf("Num of PDU      : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiTxStats.txReqStats.numPdu);
    printf("----------------------- DLSCH stats ----------------------\n");
    printFapiTxTxReqDlschStats(cellIndex);
    printf("----------------------- BCH stats ----------------------\n");
    printFapiTxTxReqBchStats(cellIndex);
    printf("----------------------- MCH stats ----------------------\n");
    printFapiTxTxReqMchStats(cellIndex);
    printf("----------------------- PCH stats ----------------------\n");
    printFapiTxTxReqPchStats(cellIndex);
}

    /* spr 3643 Changes start*/
void printFapiTxStats(InternalCellIndex cellIndex)
    /* spr 3643 Changes end*/
{
    printf("======================= PAR REQ stats ======================\n");
    printFapiTxParReqStats(cellIndex);
    printf("===================== CELL CONF REQ stats ====================\n");
    printFapiTxCellConfReqStats(cellIndex);
    printf("======================= START REQ stats ======================\n");
    printFapiTxStartReqStats(cellIndex);
    printf("======================= STOP REQ stats ======================\n");
    printFapiTxStopReqStats(cellIndex);
    printf("===================== UE CONF REQ stats ====================\n");
    printFapiTxUeConfReqStats(cellIndex);
    printf("===================== DL CONFIG REQ stats ====================\n");
    printFapiTxDlConfReqStats(cellIndex);
    printf("===================== UL CONFIG REQ stats ====================\n");
    printFapiTxUlConfReqStats(cellIndex);
    printf("===================== HI DCI0 REQ stats ====================\n");
    printFapiTxHiDci0ReqStats(cellIndex);
    printf("======================= TX REQ stats ======================\n");
    printFapiTxTxReqStats(cellIndex);
}

void printFapiRxParRespStats(InternalCellIndex cellIndex)
{
    printf("Num of messages : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.parRespStats.numMsgs);
    printf("Num of TLVs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.parRespStats.numTlvs);
    printf("Num of bytes    : %d\n",
            (gMacStats.pStats->phyStats[cellIndex].fapiRxStats.parRespStats.numMsgs *
                (SIZEOF_L1_HDR +
                offsetof(FAPI_paramResponse_st, tlvs)) +
            (gMacStats.pStats->phyStats[cellIndex].fapiRxStats.parRespStats.numTlvs *
                sizeof(FAPI_paramResponseTLV_st))));
}

void printFapiRxCellConfRespStats(InternalCellIndex cellIndex)
{
    printf("Num of Ok       : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.cellConfRespStats.numOk);
    printf("Num of Failures : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.cellConfRespStats.numFail);
    printf("Num of Inc TLVs : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.cellConfRespStats.
            numInvalidTlvs);
    printf("Num of Miss TLV : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.cellConfRespStats.
            numMissingTlvs);
    printf("Num of bytes    : %d\n",
            ((gMacStats.pStats->phyStats[cellIndex].fapiRxStats.cellConfRespStats.numOk +
              gMacStats.pStats->phyStats[cellIndex].fapiRxStats.cellConfRespStats.numFail)
                * (SIZEOF_L1_HDR +
                offsetof(FAPI_phyCellConfigResp_st, listOfTLV)) +
            ((gMacStats.pStats->phyStats[cellIndex].fapiRxStats.cellConfRespStats.
                numInvalidTlvs + gMacStats.pStats->phyStats[cellIndex].fapiRxStats.
                cellConfRespStats.numMissingTlvs) *
                sizeof(FAPI_paramResponseTLV_st))));
}

void printFapiRxStopIndStats(InternalCellIndex cellIndex)
{
    printf("Num of messages : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.stopIndStats.numMsgs);
    printf("Num of bytes    : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.stopIndStats.numMsgs *
            SIZEOF_L1_HDR);
}

void printFapiRxUeConfRespStats(InternalCellIndex cellIndex)
{
    printf("Num of Ok       : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.ueConfRespStats.numOk);
    printf("Num of Failures : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.ueConfRespStats.numFail);
    printf("Num of Inc TLVs : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.ueConfRespStats.
            numInvalidTlvs);
    printf("Num of Miss TLV : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.ueConfRespStats.
            numMissingTlvs);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.ueConfRespStats.numBytes);
    /* spr 3643 Changes end*/
}

void printFapiRxErrIndStats(InternalCellIndex cellIndex)
{
    printf("Num of Ok       : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.errIndStats.numMsgs);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.errIndStats.numBytes);
    /* spr 3643 Changes end*/
}

void printFapiRxSfIndStats(InternalCellIndex cellIndex)
{
    /* spr 3643 Changes start*/
    printf("Num of messages : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.sfIndStats.numMsgs);
    /* spr 3643 Changes end*/
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.sfIndStats.numMsgs *
            (SIZEOF_L1_HDR + sizeof(FAPI_subFrameIndication_st)));
    /* spr 3643 Changes end*/
}

void printFapiRxHarqIndStats(InternalCellIndex cellIndex)
{
    printf("Num of messages : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.harqIndStats.numMsgs);
    printf("Num of HARQs    : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.harqIndStats.numHarqs);
    printf("Num of ACKs     : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.harqIndStats.numAck);
    printf("Num of NACKs    : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.harqIndStats.numNack);
    printf("Num of bytes    : %d\n",
            (gMacStats.pStats->phyStats[cellIndex].fapiRxStats.harqIndStats.numMsgs *
                (SIZEOF_L1_HDR +
                offsetof(FAPI_harqIndication_st, harqPduInfo)) +
            (gMacStats.pStats->phyStats[cellIndex].fapiRxStats.harqIndStats.numHarqs *
                sizeof(FAPI_fddHarqPduIndication_st))));
}

void printFapiRxCrcIndStats(InternalCellIndex cellIndex)
{
    printf("Num of messages : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.crcIndStats.numMsgs);
    printf("Num of OK       : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.crcIndStats.numOk);
    printf("Num of ERR      : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.crcIndStats.numErr);
    printf("Num of bytes    : %d\n",
            (gMacStats.pStats->phyStats[cellIndex].fapiRxStats.crcIndStats.numMsgs *
                (SIZEOF_L1_HDR +
                offsetof(FAPI_crcIndication_st, crcPduInfo)) +
            (gMacStats.pStats->phyStats[cellIndex].fapiRxStats.crcIndStats.numMsgs *
                sizeof(FAPI_crcPduIndication_st))));
}

void printFapiRxRxUlschStats(InternalCellIndex cellIndex)
{
    printf("Num of messages : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.rxUlschStats.numMsgs);
    printf("Num of PDU      : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.rxUlschStats.numPdu);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.rxUlschStats.numBytes);
    /* spr 3643 Changes end*/
}

void printFapiRxRachIndStats(InternalCellIndex cellIndex)
{
    printf("Num of messages : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.rachIndStats.numMsgs);
    printf("Num of preambles: %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.rachIndStats.numPreambles);
    printf("Num of bytes    : %d\n",
            (gMacStats.pStats->phyStats[cellIndex].fapiRxStats.rachIndStats.numMsgs *
                (SIZEOF_L1_HDR +
                offsetof(FAPI_rachIndication_st, rachPduInfo)) +
            (gMacStats.pStats->phyStats[cellIndex].fapiRxStats.rachIndStats.numMsgs *
                sizeof(FAPI_rachPduIndication_st))));
}

void printFapiRxSrsIndStats(InternalCellIndex cellIndex)
{
    printf("Num of messages : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.srsIndStats.numMsgs);
     /* SPR 2066 ----- */
    printf("Num of bytes    : %d\n",
          (gMacStats.pStats->phyStats[cellIndex].fapiRxStats.srsIndStats.numMsgs *
            (SIZEOF_L1_HDR +
             offsetof(FAPI_srsIndication_st, srsPduInfo))+
            (gMacStats.pStats->phyStats[cellIndex].fapiRxStats.srsIndStats.numMsgs *
              sizeof(FAPI_srsIndication_st))));
   /*-------SPR 2066-------*/

        
}

void printFapiRxRxSrIndStats(InternalCellIndex cellIndex)
{
    printf("Num of messages : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.rxSrIndStats.numMsgs);
    printf("Num of SRs      : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.rxSrIndStats.numSr);
    printf("Num of bytes    : %d\n",
            (gMacStats.pStats->phyStats[cellIndex].fapiRxStats.rxSrIndStats.numMsgs *
                (SIZEOF_L1_HDR +
                offsetof(FAPI_rxSRIndication_st, srPduInfo)) +
            (gMacStats.pStats->phyStats[cellIndex].fapiRxStats.rxSrIndStats.numMsgs *
                sizeof(FAPI_srPduIndication_st))));
}

void printFapiRxCqiIndStats(InternalCellIndex cellIndex)
{
    printf("Num of messages : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.cqiIndStats.numMsgs);
    printf("Num of CQI      : %d\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.cqiIndStats.numCqi);
    /* spr 3643 Changes start*/
    printf("Num of bytes    : %lld\n",
            gMacStats.pStats->phyStats[cellIndex].fapiRxStats.cqiIndStats.numBytes);
    /* spr 3643 Changes end*/
}

void printFapiRxStats(InternalCellIndex cellIndex)
{
    printf("======================= PAR RESP stats ======================\n");
    printFapiRxParRespStats(cellIndex);
    printf("===================== CELL CONF RESP stats ====================\n");
    printFapiRxCellConfRespStats(cellIndex);
    printf("======================= STOP INd stats ======================\n");
    printFapiRxStopIndStats(cellIndex);
    printf("===================== UE CONF RESP stats ====================\n");
    printFapiRxUeConfRespStats(cellIndex);
    printf("======================= ERR IND stats ======================\n");
    printFapiRxErrIndStats(cellIndex);
    printf("======================= SF IND stats ======================\n");
    printFapiRxSfIndStats(cellIndex);
    printf("======================= HARQ IND stats ======================\n");
    printFapiRxHarqIndStats(cellIndex);
    printf("======================= CRC IND stats ======================\n");
    printFapiRxCrcIndStats(cellIndex);
    printf("======================= RX ULSCH stats ======================\n");
    printFapiRxRxUlschStats(cellIndex);
    printf("======================= RACH IND stats ======================\n");
    printFapiRxRachIndStats(cellIndex);
    printf("======================= SRS IND stats ======================\n");
    printFapiRxSrsIndStats(cellIndex);
    printf("======================= RX SR IND stats ======================\n");
    printFapiRxRxSrIndStats(cellIndex);
    printf("======================= RX CQI stats ======================\n");
    printFapiRxCqiIndStats(cellIndex);
}

/* +- SPR 17777 */
void printFapiStats(void)
/* +- SPR 17777 */
{
    InternalCellIndex cellIndex = 0;
    for(cellIndex=0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf("@@@@@@@@@@@@@@@@@@@@@@@@@ TX stats for Cell [%d]@@@@@@@@@@@@@@@@@@@@@@@@\n", cellIndex);
        printFapiTxStats(cellIndex);
        printf("@@@@@@@@@@@@@@@@@@@@@@@@@ RX stats for Cell [%d]@@@@@@@@@@@@@@@@@@@@@@@@\n", cellIndex);
        printFapiRxStats(cellIndex);
    }
}
void printRfStats(InternalCellIndex cellIndex)
{
#define TTI_STATS(cellIndex) gMacStats.pStats->phyStats[(cellIndex)].radioFrameStats.\
	ttiStats[sfnIterator][sfIterator]
	int sfnIterator = 0;
	int sfIterator = 0;
    /*spr 3643 changes start */
    /*++coverity 47784*/
#ifdef CYCLE_COUNT_MAC_RLC_PDCP                                               
#ifdef TDD_CONFIG
    /*spr 3643 Changes Ends*/
	int legCCIterator =0;
    /*spr 3643 changes start */
#endif
#endif
    /*++coverity 47784*/
    /*spr 3643 Changes Ends*/
    /*++coverity 47785*/
#ifdef CYCLE_COUNT_MAC_RLC_PDCP                                               
	int macCCIterator =0;
	int rlcCCIterator =0;
	int pdcpCCIterator =0;
#endif    
    /*--coverity 47785*/
	printf( "SFN     ,SF      ,"
			"CQI_IND ,SR_IND  ,SRS_IND ,RACH_IND,ULS_IND ,CRC_IND ,HARQ_IND,"
			"SF_IND  ,ERROR   ,UE_CONF ,STOP_IND,CELL_CNF,PARAM   ,  ||||  ,"
			"DCI0    ,HI      ,HI_DCI0 ,UE_CONF ,STOP_REQ,START   ,CELL_CNF,"
			"PARAM   ,TX_PCH  ,TX_DLSCH,TX_MCH  ,TX_BCH  ,TX_REQ  ,UL_SRS  ,"
			"UL_UcCRH,UL_UcCS ,UL_UcCH ,UL_UcSH ,UL_UcH  ,Ul_UcS  ,UL_UcC  ,"
			"UL_UCHR ,UL_UH   ,UL_UCR  ,UL_U    ,ULSCH   ,DL_PCH  ,DL_DLSCH,"
			"DL_MCH  ,DL_BCH  ,DL_DCI  ,DL_CFG  ,NUM_TB  ,MCS     ,LEN     ,"
			"N_TXPDU ,UL_SIZE ,");                                           
#ifdef CYCLE_COUNT_MAC_RLC_PDCP                                               
	printf( "||*Cycles*||,");
#ifdef TDD_CONFIG                                                                

	printf( "SF_IND  ,HQ_T_EXPifTM_LFT ,CQI_RPTifTM_LFT ,DMX_Q,DMX_Q_TM, MX_Q,"
			"MX_LN   ,RRC_OAM_MSG      ,PDCCH_PDSCHwMX   ,PDCCH_PDSCHwoMX,"
			"PDCCH_PDSCHwMX_TRG_PSCH ,PDCCH_PDSCHwoMX_TRG_PSCH, PUSCHfrDLY_ONE,"
			"TX_DL_DATA_MSG   ,TX_Q_LN ,TX_Q    ,TX_CMN_CH_D   ,TX_DL_CNT_MSG,"
			"TX_DL_DATA_MSG_IF_DATA_RDY, TX_DL_CNT_MSG_AND_TRIGGER ,ENCODER_QwTX_UNTIL_LST_DATA,"
			"DL_HQ_ACK_NACK_Q ,PENDING_DEL_UE   ,PENDING_RESET_UE  ,AUDIT_ACTIVE_UE,"
            "UL_RA   , DL_SCH_STRATEGY  ,UL_SCH_STRATEGY  ,RX_CNT_DATAwDMX  ,HQ_TIMER_EXPIRY  ,"
            "DL_SCHDL_STATS, UL_SCHDL_STATS   ,DL_SCHDL_STATSifTM_LFT    ,UL_SCHDL_STATSifTM_LFT ,"
			"CQI_RPT          ,N_CQI_RPT        ,N_RRC_OAM_MSG    ,REGwPHYfrSF_IND,"
			"REGwPHYfrSF_IND_AFTER_INIT_MAC     ,WTfrCELL_CONFIG  ,WTforPDCCH_RDY_TRG,"
			"N_DMX_Q ,WTfrCNT_MSG_SNT_TRG       ,RX_UNTIL_SUBFRAME_IND     ,"
			"RX_CNT_DATAwDMX  ,ENCODER_QwTX     ,ENCODER_QwoTX    ,ENCODER_QwoTX_UNTIL_LAST_DATA,"
			"PUSCHfrDLY_TWO   ,PUSCHfrDLY_THREE ,UL_SCH_STRATEGYifPDCCH_TRG_NOT_RDY,"
			"UL_SCH_STRATEGYif_NOT_ALRDY_DONE   ,SEND_CNT_TRG     ,RX_CNT_TRG,"
			"DEFAULT_STARTUP_EXEC_LEG           ,UL_CONFIG_MSG    ,TX_UL_CONFIG_MSG, UL_CSI_SINR_RPRTS,"
			"UL_HQ_TM_EXP     ,WTfrTDD_SF_IND   ,SRSfrS_SF        ,SRSfrU_SF ,"
			"SRS_RPT ,DRX_FOR_ALL_UE, MEAS_GAP_FOR_ALL_UE,");                                    
#ifdef TDD_CONFIG              
	printf( "PDCP_TX_MSG      ,RLC_HNDL_TX_OPPORTUNITY            ,RLC_HNDL_CTRL_PDU_RLC_AM,"
			"RLC_HNDL_HQ_FAIL_IND      ,RLC_PDCP_HNDLE_DATAfrmMAC ,RLC_TMR_CHKforEXP, RLC_TMR_CHK_FOR_TX_EXP,"
			"PDCP_TX_PRCSS_DELIVERY_IND,RLC_HNDL_CMN_CH&PENDING_Q ,RLC_PDCP_MSG,");
#endif
	printf( "||*ClkTime*||,");
	printf( "SF_IND  ,HQ_T_EXPifTM_LFT ,CQI_RPTifTM_LFT  ,DMX_Q   ,DMX_Q_TM,MX_Q,"
			"MX_LN   ,RRC_OAM_MSG      ,PDCCH_PDSCHwMX   ,PDCCH_PDSCHwoMX,"
			"PDCCH_PDSCHwMX_TRG_PSCH   ,PDCCH_PDSCHwoMX_TRG_PSCH  ,PUSCHfrDLY_ONE,"
			"TX_DL_DATA_MSG   ,TX_Q_LN ,TX_Q    ,TX_CMN_CH_D      ,TX_DL_CNT_MSG,"
			"TX_DL_DATA_MSG_IF_DATA_RDY,TX_DL_CNT_MSG_AND_TRIGGER ,ENCODER_QwTX_UNTIL_LST_DATA,"
			"DL_HQ_ACK_NACK_Q ,PENDING_DEL_UE   ,PENDING_RESET_UE  ,AUDIT_ACTIVE_UE,"
            "UL_RA   ,DL_SCH_STRATEGY  ,UL_SCH_STRATEGY  ,RX_CNT_DATAwDMX  ,HQ_TIMER_EXPIRY,"
            "DL_SCHDL_STATS, UL_SCHDL_STATS   ,DL_SCHDL_STATSifTM_LFT    ,UL_SCHDL_STATSifTM_LFT ,"
			"CQI_RPT          ,N_CQI_RPT        ,N_RRC_OAM_MSG    ,REGwPHYfrSF_IND,"
			"REGwPHYfrSF_IND_AFTER_INIT_MAC     ,WTfrCELL_CONFIG  ,WTforPDCCH_RDY_TRG,"
			"N_DMX_Q ,WTfrCNT_MSG_SNT_TRG       ,RX_UNTIL_SUBFRAME_IND     ,"
			"RX_CNT_DATAwDMX  ,ENCODER_QwTX     ,ENCODER_QwoTX    ,ENCODER_QwoTX_UNTIL_LAST_DATA,"
			"PUSCHfrDLY_TWO   ,PUSCHfrDLY_THREE ,UL_SCH_STRATEGYifPDCCH_TRG_NOT_RDY,"
			"UL_SCH_STRATEGYif_NOT_ALRDY_DONE   ,SEND_CNT_TRG     ,RX_CNT_TRG,"
			"DEFAULT_STARTUP_EXEC_LEG           ,UL_CONFIG_MSG    ,TX_UL_CONFIG_MSG, UL_CSI_SINR_RPRTS,"
			"UL_HQ_TM_EXP     ,WTfrTDD_SF_IND   ,SRSfrS_SF        ,SRSfrU_SF ,"
			"SRS_RPT ,DRX_FOR_ALL_UE, MEAS_GAP_FOR_ALL_UE,");
#ifdef TDD_CONFIG
	printf( "PDCP_TX_MSG      ,RLC_HNDL_TX_OPPORTUNITY            ,RLC_HNDL_CTRL_PDU_RLC_AM,"
			"RLC_HNDL_HQ_FAIL_IND      ,RLC_PDCP_HNDLE_DATAfrmMAC ,RLC_TMR_CHKforEXP, RLC_TMR_CHK_FOR_TX_EXP,"
			"PDCP_TX_PRCSS_DELIVERY_IND,RLC_HNDL_CMN_CH&PENDING_Q ,RLC_PDCP_MSG,");
#endif
#else



        /* SPR 8704 changes start */
	for ( macCCIterator = 0; macCCIterator < MAX_MAC_CC_FUNC; macCCIterator ++)
	{
		printf( "%-55s,",MAC_CC_FUNC_STRING[macCCIterator] );
	}
#ifndef DL_UL_SPLIT
	for (rlcCCIterator = 0; rlcCCIterator < MAX_RLC_CC_FUNC; rlcCCIterator ++)
	{
		printf( "%-55s,",RLC_CC_FUNC_STRING[rlcCCIterator] );
	}
#endif
	for (pdcpCCIterator = 0; pdcpCCIterator < MAX_PDCP_CC_FUNC; pdcpCCIterator ++)
	{
		printf( "%-55s,",PDCP_CC_FUNC_STRING[pdcpCCIterator] );
	}
        /* SPR 8704 changes end */
	printf( ",||*ClkTime*||,");

        /* SPR 8704 changes start */
	for ( macCCIterator = 0; macCCIterator < MAX_MAC_CC_FUNC; macCCIterator ++)
	{
		printf( "%-55s,",MAC_CC_FUNC_STRING[macCCIterator] );
	}
#ifndef DL_UL_SPLIT
	for (rlcCCIterator = 0; rlcCCIterator < MAX_RLC_CC_FUNC; rlcCCIterator ++)
	{
		printf( "%-55s,",RLC_CC_FUNC_STRING[rlcCCIterator] );
	}
#endif
	for (pdcpCCIterator = 0; pdcpCCIterator < MAX_PDCP_CC_FUNC; pdcpCCIterator ++)
	{
		printf( "%-55s,",PDCP_CC_FUNC_STRING[pdcpCCIterator] );
	}
        /* SPR 8704 changes end */

#endif
#endif
        printf(",TOTAL_PRC_TIME");
	printf("\n");

	for (sfnIterator = 0; sfnIterator < LTE_MAC_STATS_MAX_SFN; sfnIterator++)
	{
		for (sfIterator = 0; sfIterator < MAX_SUB_FRAME; sfIterator++)
		{
			printf("%4d    ,%d       ," 
					"    %d   ,    %d   ,   %d    ,    %d   ,    %d   ,    %d   ,"
					"    %d   ,"
					"    %d   ,    %d   ,    %d   ,    %d   ,    %d   ,    %d   ,"
					"  ||||  ,"
					"    %d   ,    %d   ,    %d   ,    %d   ,    %d   ,    %d   ,"
					"    %d   ,"
					"    %d   ,    %d   ,    %d   ,    %d   ,    %d   ,    %d   ,"
					"    %d   ,"
					"    %d   ,    %d   ,    %d   ,    %d   ,    %d   ,    %d   ,"
					"    %d   ,"
					"    %d   ,    %d   ,    %d   ,    %d   ,    %d   ,    %d   ,"
					"    %d   ,"
					"    %d   ,    %d   ,    %d   ,    %d   ,    %d   ,    %d   ,"
					"%5d      ,"
					"%2d      ,%4d     ,",
					sfnIterator, sfIterator,
					(TTI_STATS(cellIndex).msgRx & LTE_MAC_STATS_RF_RX_RX_CQI_IND)?1:0,
					(TTI_STATS(cellIndex).msgRx & LTE_MAC_STATS_RF_RX_RX_SR_IND)?1:0,
					(TTI_STATS(cellIndex).msgRx & LTE_MAC_STATS_RF_RX_SRS_IND)?1:0,
					(TTI_STATS(cellIndex).msgRx & LTE_MAC_STATS_RF_RX_RACH_IND)?1:0,
					(TTI_STATS(cellIndex).msgRx & LTE_MAC_STATS_RF_RX_RX_ULSCH_IND)?1:0,
					(TTI_STATS(cellIndex).msgRx & LTE_MAC_STATS_RF_RX_CRC_IND)?1:0,
					(TTI_STATS(cellIndex).msgRx & LTE_MAC_STATS_RF_RX_HARQ_IND)?1:0,
					(TTI_STATS(cellIndex).msgRx & LTE_MAC_STATS_RF_RX_SF_IND)?1:0,
					(TTI_STATS(cellIndex).msgRx & LTE_MAC_STATS_RF_RX_ERROR_IND)?1:0,
					(TTI_STATS(cellIndex).msgRx & LTE_MAC_STATS_RF_RX_UE_CONFIG_RESP)?1:0,
					(TTI_STATS(cellIndex).msgRx & LTE_MAC_STATS_RF_RX_STOP_IND)?1:0,
					(TTI_STATS(cellIndex).msgRx & LTE_MAC_STATS_RF_RX_CELL_CONFIG_RESP)?1:0,
					(TTI_STATS(cellIndex).msgRx & LTE_MAC_STATS_RF_RX_PARAM_RESP)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_HI_DCI0_DCI0)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_HI_DCI0_HI)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_HI_DCI0_REQ)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_UE_CONFIG_REQ)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_STOP_REQ)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_START_REQ)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_CELL_CONFIG_REQ)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_PARAM_REQ)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_TX_REQ_PCH)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_TX_REQ_DLSCH)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_TX_REQ_MCH)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_TX_REQ_BCH)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_TX_REQ)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_UL_CONFIG_SRS)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_CQI_SR_HARQ)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_CQI_SR)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_CQI_HARQ)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_SR_HARQ)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_HARQ)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_SR)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_CQI)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_UL_CONFIG_ULSCH_CQI_HARQ_RI)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_UL_CONFIG_ULSCH_HARQ)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_UL_CONFIG_ULSCH_CQI_RI)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_UL_CONFIG_ULSCH)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_UL_CONFIG)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_DL_CONFIG_PCH)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_DL_CONFIG_DLSCH)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_DL_CONFIG_MCH)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_DL_CONFIG_BCH)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_DL_CONFIG_DCI)?1:0,
					(TTI_STATS(cellIndex).msgTx & LTE_MAC_STATS_RF_TX_DL_CONFIG)?1:0,
					TTI_STATS(cellIndex).numTb,
					TTI_STATS(cellIndex).mcs,
					TTI_STATS(cellIndex).dlSize,
					TTI_STATS(cellIndex).numTxPdu,
					TTI_STATS(cellIndex).ulSize
						);
#ifdef CYCLE_COUNT_MAC_RLC_PDCP
			printf( "||||||||,");
#ifdef TDD_CONFIG
#ifdef DL_UL_SPLIT_TDD
#ifdef GETSTATS_EL_CLOCKTIME
            for (macCCIterator = 1; macCCIterator < MAX_EXECUTION_LEGS; macCCIterator ++)
            {
                printf( "    %d   ,",TTI_STATS.macFuncClkTimeInMicroSecPerTick[macCCIterator] );
            }
#endif
            for (macCCIterator = 1; macCCIterator < MAX_EXECUTION_LEGS; macCCIterator ++)
            {
                printf( "    %d   ,",TTI_STATS.macCyclesPerTick[macCCIterator] );
            }
#if 0
#ifdef GETSTATS_EL_CLOCKTIME
            for (rlcCCIterator = 1; rlcCCIterator < MAX_EXECUTION_LEGS; rlcCCIterator ++)
            {
                printf( "    %d   ,",TTI_STATS.ulRlcFuncClkTimeInMicroSecPerTick[rlcCCIterator] );
            }
#endif
            for (rlcCCIterator = 1; rlcCCIterator < MAX_EXECUTION_LEGS; rlcCCIterator ++)
            {
                printf( "    %d   ,",TTI_STATS.ulRlcCyclesPerTick[rlcCCIterator] );
            }
#endif
#ifdef GETSTATS_EL_CLOCKTIME
			for (pdcpCCIterator = 0; pdcpCCIterator < MAX_PDCP_CC_FUNC; pdcpCCIterator ++)
			{
				printf( "    %d   ,",TTI_STATS.pdcpFuncClkTimeInMicroSecPerTick[pdcpCCIterator] );
			}
#endif
			for (pdcpCCIterator = 0; pdcpCCIterator < MAX_PDCP_CC_FUNC; pdcpCCIterator ++)
			{
				printf( "    %d   ,",TTI_STATS.pdcpCyclesPerTick[pdcpCCIterator] );
			}
#else
		    for (legCCIterator = 1; legCCIterator < MAX_LEG_CC_FUNC; legCCIterator ++)
			{ 
				printf( "    %d   ,",TTI_STATS(cellIndex).legCyclesPerTick[legCCIterator] );
			}
#endif
#else
			for ( macCCIterator = 0; macCCIterator < MAX_MAC_CC_FUNC; macCCIterator ++)
			{
				printf( "    %d   ,",TTI_STATS(cellIndex).macCyclesPerTick[macCCIterator] );
			}
			for (rlcCCIterator = 0; rlcCCIterator < MAX_RLC_CC_FUNC; rlcCCIterator ++)
			{
				printf( "    %d   ,",TTI_STATS(cellIndex).rlcCyclesPerTick[rlcCCIterator] );
			}
			for (pdcpCCIterator = 0; pdcpCCIterator < MAX_PDCP_CC_FUNC; pdcpCCIterator ++)
			{
				printf( "    %d   ,",TTI_STATS(cellIndex).pdcpCyclesPerTick[pdcpCCIterator] );
			}
#endif
			printf( "||||||||,");
                        /* SPR 8704 changes start */
                        printf( "||||||||,");
                        /* SPR 8704 changes end */
#ifdef TDD_CONFIG
#ifndef DL_UL_SPLIT_TDD
				for (legCCIterator = 1; legCCIterator < MAX_LEG_CC_FUNC; legCCIterator ++)
			{
				printf( "    %d   ,",TTI_STATS(cellIndex).legFuncClkTimeInMicroSecPerTick[legCCIterator] );
			}
#endif
#else
			for(macCCIterator = 0; macCCIterator < MAX_MAC_CC_FUNC; macCCIterator ++)
			{
				printf( "    %d   ,",TTI_STATS(cellIndex).macFuncClkTimeInMicroSecPerTick[macCCIterator] );
			}
			for(rlcCCIterator = 0; rlcCCIterator < MAX_RLC_CC_FUNC; rlcCCIterator ++)
			{
				printf( "    %d   ,",TTI_STATS(cellIndex).rlcFuncClkTimeInMicroSecPerTick[rlcCCIterator] );
			}
			for(pdcpCCIterator = 0; pdcpCCIterator < MAX_PDCP_CC_FUNC; pdcpCCIterator ++)
			{
				printf( "    %d   ,",TTI_STATS(cellIndex).pdcpFuncClkTimeInMicroSecPerTick[pdcpCCIterator] );
			}
#endif

#endif
                        /* SPR 8704 changes start */
			printf( "||||||||,");
                        /* SPR 8704 changes end */
                        printf("    %lld   ,", TTI_STATS(cellIndex).processingTime);
			printf("\n");

		} 
	}
}
/*CA Stats Changes End */

#ifdef SSI_DEBUG_STATS
    /* +- SPR 18490 */
void printPuschPerTTIPerUeStats(void) 
    /* +- SPR 18490 */
{
    SInt32 ttiIndex = 0;
    UInt32 idx       = 0 ;
#ifdef MCS_PRB_MAP_DEBUG
    UInt32 i = 0, j = 0;
#endif
    UInt32 numTTIPrinted = 0;
    LteMacPuschTTIStats *puschTTIStats_p = NULL;
    LteMacPuschPerTTIPerUEStats *puschPerTTIPerUEStats_p = NULL;
    /*CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;

//#define gMacStats.pStats->puschTTIStats gMacStats.pStats->puschTTIStats
    if (gMacStats.pStats)
    {
        for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
        {
            printf(" #################### Cell [%d] #####################\n", cellIndex);
            for(numTTIPrinted = 0, ttiIndex = gMacStats.pStats->writeTTIIndex[cellIndex] - 1; 
            numTTIPrinted < MAX_TTI_STATS ; 
            ttiIndex--, numTTIPrinted++)
        {
                puschTTIStats_p = &gMacStats.pStats->puschTTIStats[cellIndex][ttiIndex];
            printf(" ABSOLUTE_TTI , NUM_OF_SCHEDULED_UE_RETX , NUM_OF_SCHEDULED_UE_NEWTX ,"
                " NUM_OF_SCHEDULED_UE_TOTALTX, TOTAL_RB_SHARED, TOTAL_BUDDY_PAIR_SUCCESS, TOTAL_BUDDY_PAIR_UE_RETX\n \n");
            printf("       %llu      ,        %d            ,             %d          ,"
            "             %d             ,          %d           ,          %d        ,          %d        \n",puschTTIStats_p->absoluteTTI,
            puschTTIStats_p->numScheduledUERetx,
            puschTTIStats_p->numScheduledUENewTx,
            puschTTIStats_p->numScheduledUETotal,
            /* UL_MU_MIMO_CHG_START */ 
            puschTTIStats_p->totalRbShared,
            puschTTIStats_p->totalBuddyPairSuccess,
            puschTTIStats_p->ulMuMimoTotalUeRetx);
            printf("FAILURE_COUNT_PER_CASES \n"
                    "      STRICT_ALLOC_FAILURE_COUNT        %d\n"
                    "      NDMRS_PAIR_FAILURE                %d\n"
                    "      POWER_CHECK_FAILURE               %d\n"
                    "      AVGMCS_DIFF_THRESOLD_FAILURE      %d\n"
                    "      FSS_TO_NONFSS_TRANSITION          %d\n"
                    "      SHARED_RB_EXCEED_TOTAL_RB_REQ     %d\n",
                    puschTTIStats_p->buddyPairFailurePerCases[STRICT_ALLOC_FAILURE],
                    puschTTIStats_p->buddyPairFailurePerCases[NDMRS_PAIR_FAILURE],
                    puschTTIStats_p->buddyPairFailurePerCases[POWER_CHECK_FAILURE],
                    puschTTIStats_p->buddyPairFailurePerCases[AVGMCS_DIFF_THRESOLD_FAILURE],
                    puschTTIStats_p->buddyPairFailurePerCases[FSS_TO_NONFSS_TRANSITION],
                    puschTTIStats_p->buddyPairFailurePerCases[SHARED_RB_EXCEED_TOTAL_RB_REQ]);
            /*UL_MU_MIMO_CHG_END*/
    
#ifdef MCS_PRB_MAP_DEBUG
            printf(", MCS_PRB_MAP ");
#endif
            printf("\n");
            /* for loop , UE 1 < UE 2.... UE N , print for all schecduled ues per tick */
            for (idx = 0;idx < puschTTIStats_p->numScheduledUETotal;idx++)
            {
                    puschPerTTIPerUEStats_p = &gMacStats.pStats->puschTTIStats[cellIndex][ttiIndex].puschStatsPerScheduledUE[idx];
    
                printf("SCH_UE, UEINDEX, TX_TYPE, HARQ_ID, ALLOC_PRBs,"
                   " START_RB_POS, UL_GRANT_TYPE, OPP_GRANTED, FREQ_SELECTIVE,"
                   " MCS, UL_MUMIMO_STATUS, BUDDY_UE_INDEX, USER_LOCATION_TYPE, NDMRS, MODULATION_SCHEME, ALLOC_FAIL_CAUSE \n");
                printf(" UE%3d,%5d,%5d,%5d,"
                        "%5d,%5d,%5d,"
                        "%8d,%8d,%6d,"
                        "%8d,%5d,%5d,%5d,%5d,%8d",idx,
                        puschPerTTIPerUEStats_p->ueIndex,
                        puschPerTTIPerUEStats_p->transmissionType,
                        puschPerTTIPerUEStats_p->harqProcessId,
                        puschPerTTIPerUEStats_p->allocatedPRBs,
                        puschPerTTIPerUEStats_p->startRBPosition,
                        puschPerTTIPerUEStats_p->ulGrantType,
                        puschPerTTIPerUEStats_p->oppGranted,
                        puschPerTTIPerUEStats_p->frequencySeletiveScheduling,
                        puschPerTTIPerUEStats_p->selectedMCS,
                        /* UL_MU_MIMO_CHG_START */
                        puschPerTTIPerUEStats_p->ulMUMIMOStatus,
                        puschPerTTIPerUEStats_p->buddyUeIndex,
                        puschPerTTIPerUEStats_p->userLocationType,
                        puschPerTTIPerUEStats_p->nDmrs,
                        puschPerTTIPerUEStats_p->modulationScheme,
                        /* UL_MU_MIMO_CHG_END */
                        puschPerTTIPerUEStats_p->allocationFailureCause);
                 printf("\nCURRENT_UE_POWER,  MAX_QCI_DELTA_POWER_MASK, MAX_PERMISSIBLE_POWER, INPUT_MCS_ATB, INPUT_RB_ATB, TPC, PUSCH_POWER_FACTOR,  PATH_LOSS \n");
                 printf("%7d %24d %30d %15d %12d  %5d %9d % 14d",
                        puschPerTTIPerUEStats_p->currentUePower,
                        puschPerTTIPerUEStats_p->maxQciDeltaPowerMask,
                        puschPerTTIPerUEStats_p->maxPower,
                        puschPerTTIPerUEStats_p->inputMCSForATB,
                        puschPerTTIPerUEStats_p->inputRBForATB,
                        puschPerTTIPerUEStats_p->tpc,
                        puschPerTTIPerUEStats_p->currPowerCorrectionForPusch,
                        puschPerTTIPerUEStats_p->pathLoss);

#ifdef MCS_PRB_MAP_DEBUG
                for(i=0;i<MAX_MCS_INDEX;i++)
                {
                    for(j=0;j<MAX_RESOURCE_BLOCK;j++)
                    {
                        printf("  %d  ,",puschPerTTIPerUEStats_p->mcsToPrbMap[i].prbMap[j]);
                    }
                    printf("\n");
                }
#endif
                printf("\n");
            }
            if (ttiIndex < 0)
            {
                ttiIndex = MAX_TTI_STATS - 1;
            }
        }/*for()*/
        }/* for(cellIndex) */
        /* CA Stats Changes End */
    }/*if(gMacStats.pStats) */
}

    /* +- SPR 18490 */
void printUeCommonInfoStats(void)
    /* +- SPR 18490 */
{
    UInt32 ueIdx;
    UInt16 puschIdx;
    UInt16 ttiPosIdx;
    UInt16 idx;
    if(gMacStats.pStats)
    {
        printf("         UE_INDEX_POS , ABSOLUTE_TTI \n");
        for(ueIdx = 0;ueIdx<MAX_UE_SUPPORTED;ueIdx++)
        {
            puschIdx = gMacStats.pStats->ueCommonInfoStats[ueIdx].puschCurWriteIndex;
            ttiPosIdx = gMacStats.pStats->ueCommonInfoStats[ueIdx].
                puschScheduledTTIIndex[puschIdx-1].absoluteTTI % MAX_TTI_STATS;
            for(idx=puschIdx-1; idx<puschIdx;puschIdx--)
            {
                //printf(" PUSCH_TTI_INDEX ,");
                //printf(" %d ,",idx);
                printf("UE ID %d ",ueIdx);
                if((gMacStats.pStats->ueCommonInfoStats[ueIdx].puschScheduledTTIIndex
                        [idx].absoluteTTI % MAX_TTI_STATS) - ttiPosIdx < MAX_TTI_STATS)
                {
                    printf("      %d      ,      %llu      \n",gMacStats.pStats->ueCommonInfoStats[ueIdx]
                            .puschScheduledTTIIndex[idx].ueIndexPos, gMacStats.pStats->
                            ueCommonInfoStats[ueIdx].puschScheduledTTIIndex[idx].absoluteTTI);  
                }
                if(idx == MAX_TTI_STATS)
                {
                    idx = 0;
                }
            }
        }        
    }/*if(gMacStats.pStats)*/
}

/*****************************************************************************
 * Function Name:  printSpsOamStats 
 * Input        :  None
 * Output       :  None
 * Returns      :  None
 * Description  :  Prints OAM SPS statistics
 ****************************************************************************/
    /* +- SPR 18490 */
void printSpsOamStats(void)
    /* +- SPR 18490 */
{
    UInt16  intCtr = 0;
    UInt16  index = 0;
    QoSUEAndPerLCTTIStat *spsSysWideStats_p = PNULL;
    SpsDlIntProfileStats *spsDLStats_p = PNULL;
    SpsUlIntProfileStats *spsULStats_p = PNULL;
    SpsCommIntProfileStats *spsCommStats_p =PNULL;
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;

    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf("\n###############SPS SYSTEM WIDE Stats CELL [%d]###################\n", cellIndex);
        spsSysWideStats_p = &gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex];
    printf( "enableSps = %d \nmaxSpsDciAttempt = %d \n",
            spsSysWideStats_p->enableSps,
            spsSysWideStats_p->maxSpsDciAttempt);
    printf("\nQCI SPS List = \n");
    for(intCtr =0; intCtr < MAX_QCI; intCtr++)
    {
        printf("QCI [%d], isSpsEnable [%d] \n",
                intCtr + 1, spsSysWideStats_p->qciSpslist[intCtr]);
    }
    printf("\nSPS DL Interval List = \n");
    for(intCtr =0; intCtr < TOTAL_SPS_INTERVAL; intCtr++)
    {
        if(spsSysWideStats_p->supportedSpsDlIntervalList[intCtr])
        {
            printf("Interval [%d] : Supported [%d]\n",
                    spsIntervalMap_g[intCtr],
                    spsSysWideStats_p->supportedSpsDlIntervalList[intCtr]);
        }
        else
        {
            printf("Interval [%d] : Not Supported [%d]\n",
                    spsIntervalMap_g[intCtr],
                    spsSysWideStats_p->supportedSpsDlIntervalList[intCtr]);

        }
    }
    printf("\nSPS UL Interval List = \n");
    for(intCtr =0; intCtr < TOTAL_SPS_INTERVAL; intCtr++)
    {
        if(spsSysWideStats_p->supportedSpsUlIntervalList[intCtr])
        {

            printf("Interval [%d] : Supported [%d]\n",
                    spsIntervalMap_g[intCtr],
                    spsSysWideStats_p->supportedSpsUlIntervalList[intCtr]);
        }
        else
        {
            printf("Interval [%d] : Not Supported [%d]\n",
                    spsIntervalMap_g[intCtr],
                    spsSysWideStats_p->supportedSpsUlIntervalList[intCtr]);
        }

    }
    for (intCtr = 0; intCtr < TOTAL_SPS_INTERVAL; intCtr++)
    {
        if(spsSysWideStats_p->supportedSpsDlIntervalList[intCtr])
        {
                spsDLStats_p = &gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].spsDlIntProf[intCtr];
            printf("\n\nSPS DL OAM Stats for Interval = %d \n", spsIntervalMap_g[intCtr]);

            printf ( "dlMinMcsSps = %d, dlMcsMargin = %d, "
                    "dlTbsizeMarginSps = %d, rlcSduThresForDra = %d,"
                    "dlJitterHandlingOption = %d",
                    spsDLStats_p->dlMinMcsSps,
                    spsDLStats_p->dlMcsMargin,
                    spsDLStats_p->dlTbsizeMarginSps,
                    spsDLStats_p->rlcSduThresForDra,
                    spsDLStats_p->dlJitterHandlingOption);
        }
        if( spsSysWideStats_p->supportedSpsUlIntervalList[intCtr] )
        {
                spsULStats_p = &gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].spsUlIntProf[intCtr];
            printf("\n\nSPS UL OAM Stats for Interval = %d \n", spsIntervalMap_g[intCtr]);
            printf ( "ulMinMcsSps = %d, ulMcsMargin = %d, "
                    "ulTbsizeMarginSps = %d ",
                    spsULStats_p->ulMinMcsSps,
                    spsULStats_p->ulMcsMargin,
                    spsULStats_p->ulTbSizeMarginSps);
        }
        if( spsSysWideStats_p->supportedSpsDlIntervalList[intCtr] || 
                spsSysWideStats_p->supportedSpsUlIntervalList[intCtr] )
        {
                spsCommStats_p = &gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].spsCommIntProf[intCtr];
            printf("\n\nSPS Common OAM Stats for Interval = %d \n", spsIntervalMap_g[intCtr]);
            printf ( "overrideSpsOccassionForOtherUe = %d \nstrictSpsResourceUsageForSpsLc = %d \n"
                    "interPktDelayForSilenceDetection = %d \n"
                    "enableSilenceDetection = %d \n"
                    "minRbSavedReactivationThres = %d \n"
                    "spsBlerThres = %d \nauditInterval = %d \nspsResAllocTtiAdv =%d \n",
                    spsCommStats_p->overrideSpsOccassionForOtherUe,
                    spsCommStats_p->strictSpsResourceUsageForSpsLc,
                    spsCommStats_p->interPktDelayForSilenceDetection,
                    spsCommStats_p->enableSilenceDetection,
                    spsCommStats_p->minRbSavedReactivationThres,
                    spsCommStats_p->spsBlerThres,
                    spsCommStats_p->auditInterval,
                    spsCommStats_p->spsResAllocTtiAdv);
            printf("maxPdcpPduSizeIpv4 = ");
            for(index =0; index < 4; index++)
            {
                printf("%d,",spsCommStats_p->maxPdcpPduSizeIpv4[index]);
            }
            printf("\nmaxPdcpPduSizeIpv6 = ");
            for(index =0; index < 4; index++)
            {
                printf("%d,",spsCommStats_p->maxPdcpPduSizeIpv6[index]);
            }
        }
    }
}
    /* CA Stats Changes End */
}
/*****************************************************************************
 * Function Name:  printSpsDlSchInfoPerTtiStats
 * Input        :  None
 * Output       :  None
 * Returns      :  None
 * Description  :  Prints SPS DL Scheduling Info statistics
 ****************************************************************************/
    /* +- SPR 18490 */
void printSpsDlSchInfoPerTtiStats(void)
    /* +- SPR 18490 */
{
    UInt32 index = 0;
    SpsDlSchInfoPerTtiStats *spsDlSchInfoPerTtiStats_p = PNULL;
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;
    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf("\n###############CELL [%d]###################\n", cellIndex);
        spsDlSchInfoPerTtiStats_p = &gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].spsDlSchInfoPerTtiStats;
    printf("\n###############SPS DL Scheduling Info Per TTI  Stats###################\n");
    printf("\nmaxDlSpsDciPerTti = %d \nmaxDlSpsOccasionPerTti = %d \nmaxDlSpsRbsPerTti = %d\n",
             spsDlSchInfoPerTtiStats_p->maxDlSpsDciPerTti,
             spsDlSchInfoPerTtiStats_p->maxDlSpsOccasionPerTti,
             spsDlSchInfoPerTtiStats_p->maxDlSpsRbsPerTti);
    for(index = 0; index < MAX_SPS_INTERVALS; index++)
    {
        printf("\nmaxDlSpsRbsPerTtiPerInterval = %d for interval = %d \n",
                  spsDlSchInfoPerTtiStats_p->maxDlSpsRbsPerTtiPerInterval[index],spsIntervalMap_g[index]);
    }
}
    /* CA Stats Changes End */
}
/*****************************************************************************
 * Function Name:  printSpsUlSchInfoPerTtiStats
 * Input        :  None
 * Output       :  None
 * Returns      :  None
 * Description  :  Prints SPS UL Scheduling Info statistics
 ****************************************************************************/
    /* +- SPR 18490 */
void printSpsUlSchInfoPerTtiStats(void)
    /* +- SPR 18490 */
{
    UInt32 index = 0;
    SpsUlSchInfoPerTtiStats *spsUlSchInfoPerTtiStats_p = PNULL;
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;
    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf("\n*************************CELL [%d]***********************\n", cellIndex);
        spsUlSchInfoPerTtiStats_p = &gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].spsUlSchInfoPerTtiStats;
    printf("\n###############SPS UL Scheduling Info Per TTI  Stats###################\n");
    printf("\nmaxUlSpsDciPerTti = %d \nmaxUlSpsOccasionPerTti = %d \nmaxUlSpsRbsPerTti = %d\n",
             spsUlSchInfoPerTtiStats_p->maxUlSpsDciPerTti,
             spsUlSchInfoPerTtiStats_p->maxUlSpsOccasionPerTti,
             spsUlSchInfoPerTtiStats_p->maxUlSpsRbsPerTti);
    for(index = 0; index < MAX_SPS_INTERVALS; index++)
    {
        printf("\nmaxUlSpsRbsPerTtiPerInterval = %d for interval = %d \n",
                  spsUlSchInfoPerTtiStats_p->maxUlSpsRbsPerTtiPerInterval[index],spsIntervalMap_g[index]);
    }
}
    /* CA Stats Changes End */
}

/*****************************************************************************
 * Function Name:  printSpsDlStrategyStats
 * Input        :  None
 * Output       :  None
 * Returns      :  None
 * Description  :  Prints SPS strategy statistics
 ****************************************************************************/
    /* +- SPR 18490 */
void printSpsDlStrategyStats(void)
    /* +- SPR 18490 */
{
    UInt16  ueId = 0;
    UInt16  count = 0;
    LP_SpsPerUePerTTIStats spsTtiStats_p = PNULL; 
    /* CA Stats Changes Start */
    UInt16  numActiveUe = 0;
    LP_LteSpsQueuesUsed spsQueueUsed_p = PNULL;
    UInt16 writeIndex = 0;
    InternalCellIndex cellIndex = 0;
    
    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf("\n###############CELL [%d]###################\n", cellIndex);
        numActiveUe = gMacStats.pStats->numActiveUe[cellIndex];
        writeIndex = gMacStats.pStats->writeTTIIndex[cellIndex];
    printf("\n###############DL SPS Strategy Stats###################\n");
    printf("\nFollowing enums for SPS state and SPS cause used:\n"
           "enum SPS state { SPS_IDLE = 0, SPS_CONFIGURED = 1, "
           "SPS_RES_RSV_REQ_INITIATED = 2, SPS_ACTIVATION_INITIATED = 3,"
           " SPS_ACTIVATED = 4, SPS_DEACTIVATION_INITIATED = 5, "
           "SPS_DEACTIVATED = 6, SPS_REACTIVATION_INITIATED = 7, "
           "UE_NOT_READY = 8 }\n\n enum SPS cause{ UE_SYNC_LOSS_ON = 0, "
           "UE_SYNC_LOSS_OFF = 1, SPS_CONFIG_RECVD = 2, "
           "FIRST_NON_ZERO_QUEUE_LOAD = 3, VOICE_PACKET = 4, "
           "SPS_PDSCH_SUCCESS = 5, SPS_PDSCH_FAILURE = 6, FIRST_ACK_NACK = 7, "
           "PDCCH_CCE_ALLOC_FAILURE = 8, HARQ_TIMER_EXPIRED = 9, "
           "SILENCE_DETECTED = 10, MCS_CHANGED = 11, BLER_THRESHOLD = 12, "
           "PDU_SEG_IND = 13, MAX_DCI_ATTEMPT = 14, MAX_EMPTY_TX = 15, "
           "MAX_SPS_HARQ_RETX = 16, SDL_PROCESSED = 17, OCC_PROCESSED = 18}\n"
           );
    printf( "enum SpsQName{ DL_SPS_STATUS_Q = 0, DL_SPS_DEACT_Q = 1," 
            " DL_SPS_ACT_Q = 2, DL_SPS_REACT_Q = 3, DL_SPS_UE_EXCLUDE = 4, "
            "DL_SPS_RES_RSV_REL_Q = 5 }") ;
    for (ueId = 0; ueId < numActiveUe; ueId++)
    {
        printf ( "\nUE Index = %d", ueId );
        count = writeIndex;
        do
        {
                spsTtiStats_p = &(gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].\
                            qoSUELCStat[ueId].spsTTIStats[count]);
                spsQueueUsed_p = &gMacStats.pStats->spsQueueUsed[cellIndex][count];
            if( 0 != spsTtiStats_p->isSpsPerUePerTTIStatsValid )
            {
                printf("\nCurrent TTI : %d\n", count);
                printf( "Absolute TTI [%llu] lcId = %d SPS State = %d, "
                    "SPS cause = %d, SPS Q Load = %d, SchType = %d, "
                    "currMcs = %d, spsMcs = %d, ttiIndex = %d, "
                    "numOfSpsEmptyTx = %d numRBs = %d HARQ Process Id = %d, "
                    "numDciAttempts = %d", 
                    spsTtiStats_p->absoluteTTI,
                    spsTtiStats_p->spsLcId,
                    spsTtiStats_p->spsState,
                    spsTtiStats_p->cause,
                    spsTtiStats_p->spsQload,
                    spsTtiStats_p->schType, 
                    spsTtiStats_p->currMcs,
                    spsTtiStats_p->spsMcs,
                    spsTtiStats_p->ttiIndex,
                    spsTtiStats_p->numOfSpsEmptyTx,
                    spsTtiStats_p->numRBs,
                    spsTtiStats_p->harqProcessId,
                    spsTtiStats_p->numDciAttempts );
                printf( " Queue Name [%d] , Queue Count [%d]",
                     spsQueueUsed_p->qName,
                     spsQueueUsed_p->qCount );
            }
            if (count == 0)
            {
                count = MAX_TTI_STATS - 1;
            }
            else
            {
                count--;
            }
        } while( count != writeIndex  );
    }
}
    /* CA Stats Changes End */
}
/*****************************************************************************
 * Function Name:  printSpsUlStrategyStats
 * Input        :  None
 * Output       :  None
 * Returns      :  None
 * Description  :  Prints SPS strategy statistics
 ****************************************************************************/
    /* +- SPR 18490 */
void printSpsUlStrategyStats(void)
    /* +- SPR 18490 */
{
    UInt16  ueId = 0;
    UInt16  count = 0;
    LP_SpsPerUePerTTIStatsUL spsUlTtiStats_p = PNULL; 
    LP_LteSpsQueuesUsed spsQueueUsed_p = PNULL;
    /* CA Stats Changes Start */
    UInt16  numActiveUe = 0;
    UInt16 writeIndex = 0;
    InternalCellIndex cellIndex = 0;
    
    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        numActiveUe = gMacStats.pStats->numActiveUe[cellIndex];
        writeIndex = gMacStats.pStats->writeTTIIndex[cellIndex];
        printf("\n*************************CELL [%d]***********************\n", cellIndex);
    printf("\n###############UL SPS Strategy Stats###################\n");
    printf("\nFollowing enums for SPS state and SPS cause used:\n"
           "enum SPS state { SPS_IDLE = 0, SPS_CONFIGURED = 1, "
           "SPS_RES_RSV_REQ_INITIATED = 2, SPS_ACTIVATION_INITIATED = 3,"
           " SPS_ACTIVATED = 4, SPS_DEACTIVATION_INITIATED = 5, "
           "SPS_DEACTIVATED = 6, SPS_REACTIVATION_INITIATED = 7, "
           "UE_NOT_READY = 8 }\n\n enum SPS cause{ UE_SYNC_LOSS_ON = 0, "
           "UE_SYNC_LOSS_OFF = 1, SPS_CONFIG_RECVD = 2, "
           "FIRST_NON_ZERO_QUEUE_LOAD = 3, VOICE_PACKET = 4, "
           "SPS_PDSCH_PUSCH_SUCCESS = 5, SPS_PDSCH_PUSCH_FAILURE = 6, FIRST_ACK_NACK = 7, "
           "PDCCH_CCE_ALLOC_FAILURE = 8, HARQ_TIMER_EXPIRED = 9, "
           "SILENCE_DETECTED = 10, MCS_CHANGED = 11, BLER_THRESHOLD = 12, "
           "PDU_SEG_IND = 13, MAX_DCI_ATTEMPT = 14, MAX_EMPTY_TX = 15, "
           "MAX_SPS_HARQ_RETX = 16, SDL_PROCESSED = 17, OCC_PROCESSED = 18}\n"
           );
    printf( "enum SpsQName{ UL_SPS_STATUS_Q = 0, UL_SPS_DEACT_Q = 1," 
            " UL_SPS_ACT_Q = 2, UL_SPS_REACT_Q = 3, UL_SPS_UE_EXCLUDE = 4, "
            "UL_SPS_RES_RSV_REL_Q = 5 }") ;
    for (ueId = 0; ueId < numActiveUe; ueId++)
    {
        printf ( "\nUE Index = %d", ueId );
        count = writeIndex;
        do
        {
                spsUlTtiStats_p = &(gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].\
                            qoSUELCStat[ueId].spsTTIStatsUl[count]);
                spsQueueUsed_p = &gMacStats.pStats->spsQueueUsed[cellIndex][count];
            if( 0 != spsUlTtiStats_p->isSpsPerUePerTTIStatsULValid )
            {
                printf("\nCurrent TTI : %d\n", count);
                printf( "Absolute TTI [%llu] lcId = %d SPS State = %d, "
                    "SPS cause = %d, avgRlcSduSize = %d, SchType = %d, "
                    "currMcs = %d, spsMcs = %d, ttiIndex = %d, "
                    "numOfSpsEmptyTxSure = %d numOfSpsEmptyTxUnsure = %d numRBs = %d HARQ Process Id = %d, "
                    "numDciAttempts = %d", 
                    spsUlTtiStats_p->absoluteTTI,
                    spsUlTtiStats_p->spsLcId,
                    spsUlTtiStats_p->spsState,
                    spsUlTtiStats_p->cause,
                    spsUlTtiStats_p->avgRlcSduSize,
                    spsUlTtiStats_p->schType, 
                    spsUlTtiStats_p->currMcs,
                    spsUlTtiStats_p->spsMcs,
                    spsUlTtiStats_p->ttiIndex,
                    spsUlTtiStats_p->numOfSureSpsEmptyTx,
                    spsUlTtiStats_p->numOfUnsureSpsEmptyTx,
                    spsUlTtiStats_p->numRBs,
                    spsUlTtiStats_p->harqProcessId,
                    spsUlTtiStats_p->numDciAttempts );
                printf( " Queue Name [%d] , Queue Count [%d]",
                     spsQueueUsed_p->qName,
                     spsQueueUsed_p->qCount );
            }
            if (count == 0)
            {
                count = MAX_TTI_STATS - 1;
            }
            else
            {
                count--;
            }
        } while( count != writeIndex  );
    }
}
    /* CA Stats Changes End */
}

    /* +- SPR 18490 */
void  printDebugUlUeHarqStats(void)
    /* +- SPR 18490 */
{
    UInt16  ueId = 0;
    UInt16  count = 0;
    LP_SpsDebugPerUePerTTIStatsUL spsDebugUlPerUePerTTIStats_p = PNULL; 
    /* CA Stats Changes Start */
    UInt16  numActiveUe = 0;
    UInt16 writeIndex = 0;
    InternalCellIndex cellIndex = 0;

    for(cellIndex = 0;cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf(" #################### Cell [%d] #####################\n", cellIndex);
        numActiveUe = gMacStats.pStats->numActiveUe[cellIndex];
        writeIndex = gMacStats.pStats->writeTTIIndex[cellIndex];
        printf("\n###############UL Harq Debug Stats for Cell %d###################\n", cellIndex);
        for (ueId = 0; ueId < numActiveUe; ueId++)
        {
            printf("\n[UE Index : %d] ", ueId);
            count = writeIndex;
            do
            {
                spsDebugUlPerUePerTTIStats_p = &(gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].\
                    qoSUELCStat[ueId].spsDebugPerUePerTTIStatsUl[count]);
            if(spsDebugUlPerUePerTTIStats_p->isSpsDebugPerUePerTTIStatsULValid)
            {
                printf ( "[Current TTI = %d]", count );
                printf("AbsoluteTTI[%d] harqProcessId[%d] schUsed[%d] crcReceived[%d] current_TX_NB[%d]\n",
                        spsDebugUlPerUePerTTIStats_p->absoluteTTI, 
                        spsDebugUlPerUePerTTIStats_p->harqProcessId,
                        spsDebugUlPerUePerTTIStats_p->schUsed,
                        spsDebugUlPerUePerTTIStats_p->crcReceived,
                        spsDebugUlPerUePerTTIStats_p->current_TX_NB
                      );
            }
            if (count == 0)
            {
                count = MAX_TTI_STATS - 1;
            }
            else
            {
                count--;
            }
        } while( count != writeIndex );
    }
}
    /* CA Stats Changes End */
}
/*****************************************************************************
 * Function Name:  printSpsUlUeSpecificStats
 * Input        :  None
 * Output       :  None
 * Returns      :  None
 * Description  :  Prints SPS Uplink UE statistics
 ****************************************************************************/
    /* +- SPR 18490 */
void printSpsUlUeSpecificStats(void)
    /* +- SPR 18490 */
{
    UInt16  ueId = 0;
    LteMacUlSpsUeSpecificStats *spsUlUeInfoStats_p   = PNULL;
    LteMacSpsConfigRespStats   *spsConfigRespStats_p = PNULL;
    printf("\n###############UL SPS UE Specifig Stats###################\n");
    for (ueId = 0; ueId < MAX_UE_SUPPORTED; ueId++)
    {
        spsUlUeInfoStats_p = &gMacStats.pStats->\
                           ueCommonInfoStats[ueId].ulSpsUeInfoStats;
        spsConfigRespStats_p = &gMacStats.pStats->\
                               ueCommonInfoStats[ueId].spsConfigRespStats;
        if ((spsUlUeInfoStats_p != PNULL) &&
              (spsUlUeInfoStats_p->absoluteTTI)
           /* (LTE_TRUE == spsUlUeInfoStats_p->isUlUeActive)*/)
        {
            printf("\n[UeIndex : %d] TTI[%d] isUlUeActive[%d] [spsCrnti : %d] [spsLc : %d]"
                   "[ isSpsConfigured = %d ] ",
                   spsUlUeInfoStats_p->ueIndex,spsUlUeInfoStats_p->absoluteTTI,spsUlUeInfoStats_p->isUlUeActive, spsUlUeInfoStats_p->spsCrnti, 
                   spsUlUeInfoStats_p->spsLc, spsUlUeInfoStats_p->isSpsConfigured);
            if(LTE_TRUE == spsUlUeInfoStats_p->isSpsConfigured)
            {
                printf("[SPS Interval : %d] [implicitReleaseAfter : %d]",
                spsUlUeInfoStats_p->semiPersistentSchedIntervalUL,
                spsUlUeInfoStats_p->implicitReleaseAfter);
                printf("[p0NominalPuschPersistent : %d ] [p0UePuschPersistent : %d]",
                            spsUlUeInfoStats_p->p0NominalPuschPersistent,
                            spsUlUeInfoStats_p->p0UePuschPersistent   );
            }
            printf("\n");
        }
        if (spsConfigRespStats_p != PNULL && 
            LTE_TRUE == spsConfigRespStats_p->isUeActive)
        {
            printf("\n[UeIndex : %d] ",spsConfigRespStats_p->ueIndex);
            if (spsConfigRespStats_p->spsCrnti)
            {
                printf("[Sps Config Response with spsCrnti : %d ]\n",spsConfigRespStats_p->spsCrnti);
            }
            else
            {
                printf("[Sps Config Response without spsCrnti ]\n");
            }
        }
    }
}

/*****************************************************************************
 * Function Name:  printSpsDlUeSpecificStats
 * Input        :  None
 * Output       :  None
 * Returns      :  None
 * Description  :  Prints SPS Downlink UE statistics
 ****************************************************************************/
    /* +- SPR 18490 */
void printSpsDlUeSpecificStats(void)
    /* +- SPR 18490 */
{
    UInt16  ueId = 0;
    UInt16  count = 0;
    LteMacDlSpsUeSpecificStats *spsUeInfoStats_p     = PNULL;
    LteMacSpsConfigRespStats   *spsConfigRespStats_p = PNULL;
    printf("\n###############DL SPS UE Specifig Stats###################\n");
    for (ueId = 0; ueId < MAX_UE_SUPPORTED; ueId++)
    {
        spsUeInfoStats_p = &gMacStats.pStats->\
                           ueCommonInfoStats[ueId].dlSpsUeInfoStats;
        spsConfigRespStats_p = &gMacStats.pStats->\
                               ueCommonInfoStats[ueId].spsConfigRespStats;
        if ((spsUeInfoStats_p != PNULL) && 
            (LTE_TRUE == spsUeInfoStats_p->isUeActive))
        {
            printf("\n[UeIndex : %d] [spsCrnti : %d] [spsLc : %d]"
                   " [rohcProfileUsed : %d] [ ipPktType = %d ] "
                   "[ isSpsConfigured = %d ] ",
                   spsUeInfoStats_p->ueIndex, spsUeInfoStats_p->spsCrnti, 
                   spsUeInfoStats_p->spsLc, spsUeInfoStats_p->rohcProfileUsed,
                   spsUeInfoStats_p->ipPktType, 
                   spsUeInfoStats_p->isSpsConfigured);
            if(LTE_TRUE == spsUeInfoStats_p->isSpsConfigured)
            {
                printf("[SPS Interval : %d] [numberOfConfSpsProcesses : %d]"
                       " [maxSpsHarqRetx : %d] [explicitReleaseAfter : %d]",
                spsUeInfoStats_p->semiPersistentSchedIntervalDL,
                spsUeInfoStats_p->numberOfConfSpsProcesses,
                spsUeInfoStats_p->maxSpsHarqRetx,
                spsUeInfoStats_p->explicitReleaseAfter);
                for (count = 0; 
                     count < spsUeInfoStats_p->numOfN1PucchAnPersistent; 
                     count++)
                {
                    printf("[N1PucchAnPersistent[%d] : %d] ", count,
                           spsUeInfoStats_p->N1PucchAnPersistent[count]);
                }
                printf( "[DRX OFF TTI : ");
                for( count = 0; count < MAX_DL_SPS_INTERVAL; count++ )
                {
                    if( DRX_STATE_OFF == spsUeInfoStats_p->ueDrxTTI[count] )
                    {
                        printf( "%d ", count);
                    }
                }
                printf("] ");
                printf( "[MEAS GAP ON TTI : ");
                for( count = 0; count < MAX_DL_SPS_INTERVAL; count++ )
                {
                    if( MEAS_GAP_ON == spsUeInfoStats_p->ueMeasGapTTI[count] )
                    {
                        printf( "%d ", count);
                    }
                }
                printf("] ");
            }
            printf("\n");
        }
        if (spsConfigRespStats_p != PNULL && 
            LTE_TRUE == spsConfigRespStats_p->isUeActive)
        {
            printf("\n[UeIndex : %d] ",spsConfigRespStats_p->ueIndex);
            if (spsConfigRespStats_p->spsCrnti)
            {
                printf("[Sps Config Response with spsCrnti : %d ]\n",spsConfigRespStats_p->spsCrnti);
            }
            else
            {
                printf("[Sps Config Response without spsCrnti ]\n");
            }
        }
    }
}

/*****************************************************************************
 * Function Name:  printHarqPerUeStats
 * Input        :  None
 * Output       :  None
 * Returns      :  None
 * Description  :  Prints SPS/Non SPS Downlink UE Harq statistics
 ****************************************************************************/
    /* +- SPR 18490 */
void printHarqPerUeStats(void)
    /* +- SPR 18490 */
{
    UInt16  ueId = 0;
    UInt16  count = 0;
    LP_SpsPerUeHarqPerTTIStats spsPerUeHarqPerTTIStats_p = PNULL; 
    /* CA Stats Changes Start */
    UInt16  numActiveUe = 0;
    UInt16 writeIndex = 0;
    InternalCellIndex cellIndex = 0;

    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf("\n###############DL Harq Stats foe CELL [%d]###################\n", cellIndex);
        numActiveUe = gMacStats.pStats->numActiveUe[cellIndex];
        writeIndex = gMacStats.pStats->writeTTIIndex[cellIndex];
        for (ueId = 0; ueId < numActiveUe; ueId++)
        {
            printf("\n[UE Index : %d] ", ueId);
            count = writeIndex;
            do
            {
                spsPerUeHarqPerTTIStats_p = &(gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].\
                    qoSUELCStat[ueId].spsPerUeHarqPerTTIStats[count]);
            if( spsPerUeHarqPerTTIStats_p->isSpsPerUeHarqPerTTIStatsValid )
            {
                printf ( "[Current TTI = %d, absoluteTTI = %llu]", count,
                         spsPerUeHarqPerTTIStats_p->absoluteTTI );
                printf("[numOfBusySpsHarqProcessForNewTx: %d] "
                       "[numOfBusySpsHarqProcessForReTx: %d] "
                       "[numOfBusyDRAHarqProcessForNewTx: %d] "
                       "[numOfBusyDRAHarqProcessForReTx: %d]\n",
                    spsPerUeHarqPerTTIStats_p->numOfBusySpsHarqProcessForNewTx,
                    spsPerUeHarqPerTTIStats_p->numOfBusySpsHarqProcessForReTx,
                    spsPerUeHarqPerTTIStats_p->numOfBusyDRAHarqProcessForNewTx,
                    spsPerUeHarqPerTTIStats_p->numOfBusyDRAHarqProcessForReTx);
            }
            if (count == 0)
            {
                count = MAX_TTI_STATS - 1;
            }
            else
            {
                count--;
            }
        } while( count != writeIndex );
    }
}
    /* CA Stats Changes End */
}

    /* +- SPR 18490 */
void  printDebugUeHarqStats(void)
    /* +- SPR 18490 */
{
    UInt16  ueId = 0;
    UInt16  count = 0;
    LP_SpsDebugPerUePerTTIStats spsDebugPerUePerTTIStats_p = PNULL; 
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;
    UInt16  numActiveUe = 0;
    UInt16 writeIndex = 0;

    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf("\n###############DL Harq Debug Stats for CELL[%d]###################\n", cellIndex);
        numActiveUe = gMacStats.pStats->numActiveUe[cellIndex];
        writeIndex = gMacStats.pStats->writeTTIIndex[cellIndex];
        for (ueId = 0; ueId < numActiveUe; ueId++)
        {
            printf("\n[UE Index : %d] ", ueId);
            count = writeIndex;
            do
            {
                spsDebugPerUePerTTIStats_p = &(gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].\
                    qoSUELCStat[ueId].spsDebugPerUePerTTIStats[count]);
            if(spsDebugPerUePerTTIStats_p->isSpsDebugPerUePerTTIStatsValid)
            {
                printf ( "[Current TTI = %d]", count );
                printf("AbsoluteTTI[%d] harqProcessId[%d] ackNackReceived[%d] current_TX_NB[%d]\n",
                        spsDebugPerUePerTTIStats_p->absoluteTTI, 
                        spsDebugPerUePerTTIStats_p->harqProcessId,
                        spsDebugPerUePerTTIStats_p->ackNackReceived,
                        spsDebugPerUePerTTIStats_p->current_TX_NB
                      );
            }
            if (count == 0)
            {
                count = MAX_TTI_STATS - 1;
            }
            else
            {
                count--;
            }
        } while( count != writeIndex );
    }
}
    /* CA Stats Changes End */
}
#endif /* SSI_DEBUG_STATS*/

#ifdef KPI_STATS
/* +- SPR 17777 */
void printULSpectralEfficiency(void)
/* +- SPR 17777 */
{
    /* + KPI_CA */
    InternalCellIndex cellIndex = 0;
    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
    if(gMacStats.pStats && \
           gMacStats.pStats->lteCommonKPIStats[cellIndex].lteMacKPIStats[0].uplinkCellBandwidth && \
           gMacStats.pStats->lteCommonKPIStats[cellIndex].lteMacKPIStats[0].totalUplinkTTI)
    {
        /*SPR 15909 Fix Start*/
            printf("Average Uplink Spectral Efficiency for Cell[%d] in the last %llu TTI = %f\n",cellIndex, gMacStats.pStats->lteCommonKPIStats[cellIndex].lteMacKPIStats[0].totalUplinkTTI ,(UDouble32)((gMacStats.pStats->lteCommonKPIStats[cellIndex].lteMacKPIStats[0].totalUplinkBytesSent<<3)/gMacStats.pStats->lteCommonKPIStats[cellIndex].lteMacKPIStats[0].uplinkCellBandwidth)*1000/(gMacStats.pStats->lteCommonKPIStats[cellIndex].lteMacKPIStats[0].totalUplinkTTI));

            gMacStats.pStats->lteCommonKPIStats[cellIndex].lteMacKPIStats[0].totalUplinkBytesSent = 0;
        /*SPR 15909 Fix end*/
        }
    }
}

/* +- SPR 17777 */
void printUplinkPRBUsageStats(void)
/* +- SPR 17777 */
{
    InternalCellIndex cellIndex = 0;
    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        if(gMacStats.pStats && gMacStats.pStats->lteCommonKPIStats[cellIndex].lteMacKPIStats[0].totalUplinkTTI)
        {
        /* spr 3643 Changes start*/
        /*SPR 15909 Fix Start*/
            printf("Average Uplink PRB usage in Cell[%d] per TTI in the last %llu TTIs = %lld\n", \
                cellIndex, gMacStats.pStats->lteCommonKPIStats[cellIndex].lteMacKPIStats[0].totalUplinkTTI, 
                gMacStats.pStats->lteCommonKPIStats[cellIndex].lteMacKPIStats[0].totalUplinkPRBUsage/gMacStats.pStats->lteCommonKPIStats[cellIndex].lteMacKPIStats[0].totalUplinkTTI);   
        /*SPR 15909 Fix end*/
        /* spr 3643 Changes end*/
            gMacStats.pStats->lteCommonKPIStats[cellIndex].lteMacKPIStats[0].totalUplinkTTI = 1;
            gMacStats.pStats->lteCommonKPIStats[cellIndex].lteMacKPIStats[0].totalUplinkPRBUsage = 0;
        }
    }    
}
    /* - KPI_CA */
#endif

#ifdef UTFWK_SIMULATION
/* +- SPR 18490 */
void printQosStats(void)
{
    UInt64 tempTick = 0;
    UInt64 timeIntval = 0;
    UInt64 totalSchLoad = 0;
    UInt64 thruPut = 0;
    UInt64 LCthruput_temp = 0;
    UInt64 padThruput_temp = 0;
    UInt64 UEthruput = 0;
    UInt64 UEthruput2 = 0;
    UInt64 UEthruput_temp = 0;
    UInt32 ueId = 0;
    UInt32 lcId = 0;
    float ueThruput = 0.0;
    UInt16 count = 0;
    float LCthruput = 0.0;
    float dl_Lcthruput_ticks = 0.0;
    float dl_Padthruput_ticks = 0.0;
    UInt64 UEthruputDl[MAX_UE_SUPPORTED] = {0};
    float ulError[MAX_UE_SUPPORTED][MAX_LC_VALUE] = {{0.0}};
    float dlError[MAX_UE_SUPPORTED][MAX_LC_VALUE] = {{0.0}};

    UInt64 currentTick = gMacStats.pStats->qoSUEAndPerLCTTIStat.currentTick;
    UInt16  numActiveUe = gMacStats.pStats->numActiveUe;

    tempTick = gMacStats.pStats->qoSUEAndPerLCTTIStat.lastTick;
    timeIntval = currentTick - tempTick;
    gMacStats.pStats->qoSUEAndPerLCTTIStat.lastTick = 0;

    printf("\n###################DL Stats##############################\n");
    printf("\nCurrent TTI : %llu ",currentTick);
    printf("\n#################################################");
    for (ueId = 0; ueId < numActiveUe; ueId++)
    {
        printf("\nUe Index : %6u ",ueId);
        printf("     LC Throughput  ");
        for (lcId = MIN_LC_VALUE; lcId < MAX_LC_VALUE; lcId++)
        {
            dlError[ueId][lcId] = 0.0;
            LCthruput_temp = 0;
            padThruput_temp = 0;
            for( count = 0; count < MAX_UTFWK_TICK; count++)
            {
                LCthruput_temp += gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueId].qoSLCPerTTIStat[lcId].STX[count%MAX_UTFWK_TICK].OppGranted;
                if (gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueId].qoSLCPerTTIStat[lcId].STX[count%MAX_UTFWK_TICK].OppGranted != 0)
                {
                    padThruput_temp += (gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueId].dlTBSize[count%MAX_UTFWK_TICK] - gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueId].qoSLCPerTTIStat[lcId].STX[count%MAX_UTFWK_TICK].OppGranted);
                }
            }
            dl_Lcthruput_ticks = (float)(LCthruput_temp * 8)/(1024*1024);
            dl_Padthruput_ticks = (float)(padThruput_temp * 8)/(1024*1024);

            printf(" LCID[%d] : %0.2f",lcId,dl_Lcthruput_ticks);
            printf(" PAD[%d] : %0.2f",lcId,dl_Padthruput_ticks);
            if(gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueId].qoSLCPerTTIStat[lcId].numDLMiss)
            {
                dlError[ueId][lcId] = ((float)(gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueId].qoSLCPerTTIStat[lcId].numDLMiss)/(float)(gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueId].qoSLCPerTTIStat[lcId].numDLPkts) * 100.00);
            }
            printf(" DL Err[%d] :  %0.2f%% ",lcId,dlError[ueId][lcId]);
            UEthruputDl[ueId] += LCthruput_temp;

            gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueId].qoSLCPerTTIStat[lcId].TotalLCOppGranted = 0;
        }
        gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueId].TotalUeOPPGranted = 0;
	ueThruput = (float)(UEthruputDl[ueId] * 8)/(1024 * 1024);
        printf("Ue Throughput        : %0.2f ",ueThruput);
        UEthruputDl[ueId] = 0;
    }
    //for UL
    printf("\n###################UL Stats##############################\n");
    printf("\nCurrent TTI : %llu ",currentTick);
    printf("\n#################################################");
    for (ueId = 0; ueId < numActiveUe; ueId++)
    {
        UEthruput2 = 0.0;
        UEthruput = 0.0;

        printf("\nUe Index : %6u ",ueId);
        printf("     LC Throughput  ");
        for (lcId = MIN_LC_VALUE; lcId < MAX_LC_VALUE; lcId++)
        {
            ulError[ueId][lcId] = 0.0;
            LCthruput_temp = 0;
            for( count = 0; count < MAX_UTFWK_TICK; count++)
            {
                LCthruput_temp += gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcId].STX[count].ulLcGrant;
            }
            LCthruput = LCthruput_temp/timeIntval;
            LCthruput= (float)(LCthruput_temp * 8)/(1024 * 1024);
            printf("  LCID[%d] : %0.2f",lcId,LCthruput);
            if(gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcId].numULMiss)
            {
                ulError[ueId][lcId] = (((float)gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcId].numULMiss)/(float)(gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcId].numULPkts) * 100.00);
            }
            printf(" UL Err[%d] :  %0.2f%% ",lcId,ulError[ueId][lcId]);
        }
        for( count = 0; count < MAX_UTFWK_TICK; count++)
        {
            UEthruput += gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueId].grant[count];
        }
        UEthruput2 = UEthruput *8;
        ueThruput = (float)(UEthruput2)/(1024 * 1024);
        printf("Ue Throughput        : %0.2f",ueThruput);
    }
}
#endif

/* CA Stats Changes Start */
#ifdef SSI_DEBUG_STATS
/* +- SPR 18490 */
void printSchConfigStats(void)
{
    InternalCellIndex cellIndex = 0;
    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf(" #################### Cell [%d] #####################\n", cellIndex);
    QoSUEAndPerLCTTIStat *qoSUEAndPerLCTTIStat_p = 
            &(gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex]) ;
    printf("###################### SCH CONFIG STATS #####################\n");
    /* For DL */
    printf("DL TknPeriodicity : %2u\n", qoSUEAndPerLCTTIStat_p->tknPeriodicity);
    printf("DL SCH UE : %2u\n", qoSUEAndPerLCTTIStat_p->noOfDLSchUE);
    printf("DLPdbWt : %2u\n", qoSUEAndPerLCTTIStat_p->pdbWt);
    printf("DLQciWt : %2u\n", qoSUEAndPerLCTTIStat_p->qciWt);
    printf("DLtknWt : %2u\n", qoSUEAndPerLCTTIStat_p->tknWt);
    printf("DLqLoadWt : %2u\n",qoSUEAndPerLCTTIStat_p->qLoadWt);
    printf("DlMcsWt : %2u\n", qoSUEAndPerLCTTIStat_p->mcsWt);
    printf("\n\n");
    /* For UL */
    printf("UL TknPeriodicity : %2u\n",qoSUEAndPerLCTTIStat_p->ulTknPeriodicity);
    printf("UL SCH UE : %2u\n", qoSUEAndPerLCTTIStat_p->noOfULSchUE);
    printf("ULPdbWt : %2u\n", qoSUEAndPerLCTTIStat_p->ulPdbWt);
    printf("ULQciWt : %2u\n", qoSUEAndPerLCTTIStat_p->ulQciWt);
    printf("ULtknWt : %2u\n", qoSUEAndPerLCTTIStat_p->ulTknWt);
    printf("ULqLoadWt : %2u\n", qoSUEAndPerLCTTIStat_p->ulQLoadWt);
    printf("UlMcsWt : %2u\n", qoSUEAndPerLCTTIStat_p->ulMcsWt);
    printf("\n");
}
}

/* +- SPR 18490 */
void printQoSUELCStats(void)
{
    UInt16 ueId = 0;
    UInt8 lcId = 0;
    UInt16 count = 0;
    float thruput = 0.0;
    UInt64 dl_thruput_tick = 0;
    UInt64 pad_thruput_tick = 0;
    float pad_thruput = 0;
    InternalCellIndex cellIndex = 0;
    UInt16  numActiveUe = 0;

    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf(" #################### Cell [%d] #####################\n", cellIndex);
        numActiveUe = gMacStats.pStats->numActiveUe[cellIndex];
        printf ("Num of Active UE in Cell[%d] =%d\n", cellIndex, numActiveUe);
    printf("\n######################### PER TTI stats ########################\n");
    for (count = 0; count < MAX_TTI_STATS; count++)
    {
            printf("\n######################TTI  %d global Tick %d ###########################\n",
                    count,gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].currentTickValue[count]);
        for (ueId = 0; ueId < numActiveUe; ueId++)
        {
            QoSUELCStat *qosUeStat_p =
                    &(gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].qoSUELCStat[ueId]);
            for (lcId = MIN_LC_VALUE; lcId < MAX_LCID; lcId++)
            {
                QoSLCPerTTIStat *qosLcPerTTIStat_p =
                    &(qosUeStat_p->qoSLCPerTTIStat[lcId]);
                printf("UeId[%u],lcId[%u],",ueId,lcId);
                printf("MCS[%u],QciPri[%u],QLPri[%d],PDBPri[%d],TknPri[%d],"
                       "AllcGrntT[%u],ExpGrntT[%u],PktDly[%u],Qload[%u],"
                       "OppInd[%u],Tkn[%u],TBSize[%u],",
                       qosUeStat_p->mcsIndexCodeWordOne[count],
                       qosLcPerTTIStat_p->STX[count].qciPriority,
                       qosLcPerTTIStat_p->STX[count].qLoadPriority,
                       qosLcPerTTIStat_p->STX[count].pdbPriority,
                       qosLcPerTTIStat_p->STX[count].tokenPriority,
                       qosLcPerTTIStat_p->STX[count].totSchdTknInWindow,
                       qosLcPerTTIStat_p->expGrntT,
                       qosLcPerTTIStat_p->STX[count].headOfLinePktDelay,
                       qosLcPerTTIStat_p->STX[count].qLoad,
                       qosLcPerTTIStat_p->STX[count].OppGranted,
                       qosLcPerTTIStat_p->STX[count].avaiableToken,
                       qosUeStat_p->dlTBSize[count]);
                if (qosLcPerTTIStat_p->STX[count].OppGranted != 0)
                {
                    printf("Pad[%u],",qosUeStat_p->dlTBSize[count] - \
                              qosLcPerTTIStat_p->STX[count].OppGranted);
                }
                else
                {
                    printf("Pad[%u],",qosLcPerTTIStat_p->STX[count].OppGranted);
                }
                    printf( "Wp[%u],PktDrp[%llu]\n",
                        qosLcPerTTIStat_p->STX[count].LCWeight,
                        qosLcPerTTIStat_p->STX[count].numDLMissPerTick);
                dl_thruput_tick += qosLcPerTTIStat_p->STX[count].OppGranted;
                if (qosLcPerTTIStat_p->STX[count].OppGranted != 0)
                {
                    pad_thruput_tick += (qosUeStat_p->dlTBSize[count] - qosLcPerTTIStat_p->STX[count].OppGranted);
                }
            }
        }
    }
    }
    printf("\n DL Total OppGranted for last %d Ticks : %lld ",MAX_TTI_STATS, dl_thruput_tick);
    printf("\n DL Total Padding for last %d Ticks : %lld ",MAX_TTI_STATS, pad_thruput_tick);
    thruput = (float)(dl_thruput_tick * 8)/(1024*1024);
    pad_thruput = (float)(pad_thruput_tick * 8)/(1024*1024);
    printf("\n DL Throughput is :::  %0.2f ",thruput);
    printf("\n Pad Throughput is :::  %0.2f ",pad_thruput);
#ifdef UTFWK_SIMULATION
    printf("\n Total DL Pkts in System : %u ",gMacStats.pStats->qoSUEAndPerLCTTIStat.numTotDLPktsInSys);
    printf("\n Total DL Pkts Dropped in System : %u ",gMacStats.pStats->qoSUEAndPerLCTTIStat.numDLMissInSys);
#endif
}
/* CA Stats Changes End */

/****************************************************************************
* Function Name  : printDLStartegyPerTTIPerLCStats
* Inputs         : None
* Outputs        : none
* Returns        : none
* Description    : This function prints the PDSCH debug stats per TTI per UE
****************************************************************************/
/* +- SPR 18490 */
void printDLStartegyPerTTIPerLCStats(void)
{
    UInt16 ttiIndex = 0;
    LteQosDLStrategyLCTTIStat *pLteMacDLStrategyLCTTIStats = PNULL;
    LteQosDLStrategyLCTTIPerLcStat *pDlStrategyPerLCPerTTI = PNULL;
    UInt16 numScheduledLC = 0;
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;

    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf(" #################### Cell [%d] #####################\n", cellIndex);
        for (ttiIndex = gMacStats.pStats->writeTTIIndex[cellIndex] - 2;
                ttiIndex < gMacStats.pStats->writeTTIIndex[cellIndex];ttiIndex--) 
        {
            pLteMacDLStrategyLCTTIStats = &gMacStats.pStats->dlStrategyLcTTIStats[cellIndex][ttiIndex];

       printf("absoluteTTI [%lld]   numScheduledLC [%d] \n",
                pLteMacDLStrategyLCTTIStats->absoluteTTI,
                pLteMacDLStrategyLCTTIStats->numScheduledLC);

       printf("ueIndex, lcId, qLoad, scheduledBytes, resheduledBytes, headOfLineDelay,"
                    "drxOnDurationDelay, availToken, totalLcWeight, totTknSchedWndw\n");

        for ( numScheduledLC = 0; 
              numScheduledLC < pLteMacDLStrategyLCTTIStats->numScheduledLC;
              numScheduledLC++ )
        {
            pDlStrategyPerLCPerTTI = &pLteMacDLStrategyLCTTIStats->
                                dlStrategyPerLC[numScheduledLC];

            printf("[%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t \n",
                    pDlStrategyPerLCPerTTI->ueIndex,
                    pDlStrategyPerLCPerTTI->lcId,
                    pDlStrategyPerLCPerTTI->qLoad,
                    pDlStrategyPerLCPerTTI->scheduledBytes,
                    pDlStrategyPerLCPerTTI->resheduledBytes,
                    pDlStrategyPerLCPerTTI->headOfLineDelay,
                    pDlStrategyPerLCPerTTI->drxOnDurationDelay,
                    pDlStrategyPerLCPerTTI->availToken,
                    pDlStrategyPerLCPerTTI->totalLcWeight,
                    pDlStrategyPerLCPerTTI->totTknSchedWndw);
        }
        if (ttiIndex == 0)
        {
            ttiIndex = MAX_TTI_STATS - 1;
        }
    }
}
    /* CA Stats Changes End */
}

/****************************************************************************
* Function Name  : printfDLStartegyLcCommonInfoStats
* Inputs         : None
* Outputs        : none
* Returns        : none
* Description    : This function prints the PDSCH stats for common UE Info
****************************************************************************/
/* +- SPR 18490 */
void printfDLStartegyLcCommonInfoStats(void)
{
    UInt32 ueIdx;
    UInt64 lcAbsTTI;
    UInt16 lcPosIndex;
    UInt16 lcIndex;
    UInt16 lcCount;
    UInt16 ttiIndex;
    UInt16 dlStrategyLcIndex = 0;
	UInt16 dlstrategyLcTTIIndex = 0;
    LteUECommonInfoStats *pUECommonInfoStats = PNULL;
    LteQosDLStrategyLCTTIStat *pLteMacDLStrategyLCTTIStats = PNULL;
    LteQosDLStrategyLCTTIPerLcStat *pDlStrategyPerLCPerTTI = PNULL;
    /* CA Stats Changes Start */
    UInt16  numActiveUe = 0;
    InternalCellIndex cellIndex = 0;

    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf(" #################### Cell [%d] #####################\n", cellIndex);
        numActiveUe = gMacStats.pStats->numActiveUe[cellIndex];

    for(ueIdx = 0; ueIdx < numActiveUe; ueIdx++)
    {
        pUECommonInfoStats = &gMacStats.pStats->ueCommonInfoStats[ueIdx];

        dlStrategyLcIndex = pUECommonInfoStats->dlStrategyLcCurWriteIndex;

        while(dlstrategyLcTTIIndex < MAX_TTI_STATS)
		{
		    if(!dlStrategyLcIndex)
			{
				dlStrategyLcIndex = MAX_TTI_STATS;
			}

			lcAbsTTI = (pUECommonInfoStats->dlstrategyLcTTIIndex[dlStrategyLcIndex - 1].
								absoluteTTI);
			ttiIndex = (lcAbsTTI -1) % MAX_TTI_STATS;

			lcCount = pUECommonInfoStats->dlstrategyLcTTIIndex[dlStrategyLcIndex - 1].
						lcCurWriteIndex;

                pLteMacDLStrategyLCTTIStats = &gMacStats.pStats->dlStrategyLcTTIStats[cellIndex][ttiIndex];

			if(lcAbsTTI == pLteMacDLStrategyLCTTIStats->absoluteTTI)
            {
                printf("ueIndex, lcId, qLoad, scheduledBytes, resheduledBytes, headOfLineDelay,"
                        "drxOnDurationDelay, availToken, totalLcWeight, totTknSchedWndw\n");

                lcIndex = 0;
                while(lcIndex < lcCount)
                {
                    lcPosIndex = pUECommonInfoStats->dlstrategyLcTTIIndex[dlStrategyLcIndex - 1].
                        lcIndexPos[lcIndex]; 
                    pDlStrategyPerLCPerTTI = &pLteMacDLStrategyLCTTIStats->
                        dlStrategyPerLC[lcPosIndex];

                    printf("[%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t \n",
                            pDlStrategyPerLCPerTTI->ueIndex,
                            pDlStrategyPerLCPerTTI->lcId,
                            pDlStrategyPerLCPerTTI->qLoad,
                            pDlStrategyPerLCPerTTI->scheduledBytes,
                            pDlStrategyPerLCPerTTI->resheduledBytes,
                            pDlStrategyPerLCPerTTI->headOfLineDelay,
                            pDlStrategyPerLCPerTTI->drxOnDurationDelay,
                            pDlStrategyPerLCPerTTI->availToken,
                            pDlStrategyPerLCPerTTI->totalLcWeight,
                            pDlStrategyPerLCPerTTI->totTknSchedWndw);
                    lcIndex++;
                }
                dlStrategyLcIndex--;
            }
			else
			{
				break;
			}
            dlstrategyLcTTIIndex++;
		}
    }
}
    /* CA Stats Changes End */
}

/****************************************************************************
* Function Name  : printfDLStartegyUeCommonInfoStats
* Inputs         : None
* Outputs        : none
* Returns        : none
* Description    : This function prints the PDSCH stats for common UE Info
****************************************************************************/
/* +- SPR 18490 */
void printfDLStartegyUeCommonInfoStats(void)
{
    UInt32 ueIdx;
    UInt64 ueAbsTTI;
    UInt16 uePosIndex;
    UInt16 ttiIndex;
    UInt16 dlStrategyIndex = 0;
	UInt16 dlstartegyScheduledTTIIndex = 0;
    LteUECommonInfoStats *pUECommonInfoStats = PNULL;
    LteMacDlStrategyTTIStats *pLteMacDLStrategyTTIStats = PNULL;
    LteMacDlStrategyPerTTIPerUEStats *pDlStrategyPerUEPerTTI = PNULL;
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;
    UInt16  numActiveUe = 0;

    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf(" #################### Cell [%d] #####################\n", cellIndex);
        numActiveUe = gMacStats.pStats->numActiveUe[cellIndex];
        for(ueIdx = 0; ueIdx < numActiveUe; ueIdx++)
        {
            pUECommonInfoStats = &gMacStats.pStats->ueCommonInfoStats[ueIdx];

            dlStrategyIndex = pUECommonInfoStats->dlStrategyCurWriteIndex[cellIndex];

        printf("ueIndex, minTBSize, maxTBSize, scheduledRB, ueWbMCS, recvdRI,"
                "transmissionMode, transmissionType, scheduleLcCount, eligibleLcCount,"
                "harqProcessId, aperodicCQIFlag\n");
        while(dlstartegyScheduledTTIIndex < MAX_TTI_STATS)
        {
            if(!dlStrategyIndex)
            {
                dlStrategyIndex = MAX_TTI_STATS;
            }

                ueAbsTTI = (pUECommonInfoStats->dlstartegyScheduledTTIIndex[cellIndex][dlStrategyIndex - 1].
                        absoluteTTI);
                ttiIndex = (ueAbsTTI - 1) % MAX_TTI_STATS;

                uePosIndex = pUECommonInfoStats->dlstartegyScheduledTTIIndex[cellIndex][dlStrategyIndex - 1].
                ueIndexPos;

                pLteMacDLStrategyTTIStats = &gMacStats.pStats->dlStrategyTTIStats[cellIndex][ttiIndex];

            if(ueAbsTTI == pLteMacDLStrategyTTIStats->absoluteTTI)
            {
                pDlStrategyPerUEPerTTI = &pLteMacDLStrategyTTIStats->dlStrategyPerUE[uePosIndex];

                printf("[%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t \n",
                        pDlStrategyPerUEPerTTI->ueIndex,
                        pDlStrategyPerUEPerTTI->minTBSize,
                        pDlStrategyPerUEPerTTI->maxTBSize,
                        pDlStrategyPerUEPerTTI->scheduledRB,
                        pDlStrategyPerUEPerTTI->ueWbMCS,
                        pDlStrategyPerUEPerTTI->recvdRI,
                        pDlStrategyPerUEPerTTI->transmissionMode,
                        pDlStrategyPerUEPerTTI->transmissionType,
                        pDlStrategyPerUEPerTTI->scheduleLcCount,
                        pDlStrategyPerUEPerTTI->eligibleLcCount,
                        pDlStrategyPerUEPerTTI->harqProcessId,
                        pDlStrategyPerUEPerTTI->aperodicCQIFlag);

                dlStrategyIndex--;
            }
            else
            {
                break;
            }
            dlstartegyScheduledTTIIndex++;
        }
    }
}
    /* CA Stats Changes End */
}

/****************************************************************************
* Function Name  : printDLStartegyPerTTIPerUEStats
* Inputs         : None
* Outputs        : none
* Returns        : none
* Description    : This function prints the PDSCH debug stats per TTI per UE
****************************************************************************/
/* +- SPR 18490 */
void printDLStartegyPerTTIPerUEStats(void)
{
    UInt16 ttiIndex = 0;
    LteMacDlStrategyTTIStats *pLteMacDLStrategyTTIStats = PNULL;
    LteMacDlStrategyPerTTIPerUEStats *pDlStrategyPerUEPerTTI = PNULL;
    UInt16 numScheduledUE = 0;
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;

    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf(" #################### Cell [%d] #####################\n", cellIndex);
        for (ttiIndex = gMacStats.pStats->writeTTIIndex[cellIndex] - 2;
                ttiIndex < gMacStats.pStats->writeTTIIndex[cellIndex];ttiIndex--) 
        {
            pLteMacDLStrategyTTIStats = &gMacStats.pStats->dlStrategyTTIStats[cellIndex][ttiIndex];

        printf("absoluteTTI [%lld]  numUeRetxnFailureQueue [%d], numUeRetxnQueue[%d]," 
                "numTAPendingQueue[%d], numTAFreshQueue[%d], numTAWithData[%d]\n, scheduledReTXUE[%d]  "
                "scheduledTAUE [%d], scheduledTxUE [%d] totalUESch [%d]\n",
                pLteMacDLStrategyTTIStats->absoluteTTI,
                pLteMacDLStrategyTTIStats->numUeRetxnFailureQueue,
                pLteMacDLStrategyTTIStats->numUeRetxnQueue,
                pLteMacDLStrategyTTIStats->numTAPendingQueue,
                pLteMacDLStrategyTTIStats->numTAFreshQueue,
                pLteMacDLStrategyTTIStats->numTAWithData,
                pLteMacDLStrategyTTIStats->scheduledReTXUE,
                pLteMacDLStrategyTTIStats->scheduledTAUE,
                pLteMacDLStrategyTTIStats->scheduledTxUE,
                pLteMacDLStrategyTTIStats->scheduledUETotal);

            printf("ueIndex, minTBSize, maxTBSize, scheduledRB, ueWbMCS, recvdRI,"
                   "transmissionMode, transmissionType, scheduleLcCount, eligibleLcCount,"
                   "harqProcessId, aperodicCQIFlag, isTBOneValid, isTBTwoValid, primaryDCI, secondaryDCI\n");
        for ( numScheduledUE = 0; 
              numScheduledUE < pLteMacDLStrategyTTIStats->scheduledUETotal;
              numScheduledUE++ )
        {
            pDlStrategyPerUEPerTTI = &pLteMacDLStrategyTTIStats->
                                dlStrategyPerUE[numScheduledUE];

            printf("[%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t \n",
                   pDlStrategyPerUEPerTTI->ueIndex,
                   pDlStrategyPerUEPerTTI->minTBSize,
                   pDlStrategyPerUEPerTTI->maxTBSize,
                   pDlStrategyPerUEPerTTI->scheduledRB,
                   pDlStrategyPerUEPerTTI->ueWbMCS,
                   pDlStrategyPerUEPerTTI->recvdRI,
                   pDlStrategyPerUEPerTTI->transmissionMode,
                   pDlStrategyPerUEPerTTI->transmissionType,
                   pDlStrategyPerUEPerTTI->scheduleLcCount,
                   pDlStrategyPerUEPerTTI->eligibleLcCount,
                   pDlStrategyPerUEPerTTI->harqProcessId,
                   pDlStrategyPerUEPerTTI->aperodicCQIFlag,
                   pDlStrategyPerUEPerTTI->isTBOneValid,
                   pDlStrategyPerUEPerTTI->isTBTwoValid,
                   pDlStrategyPerUEPerTTI->primaryDCIFormat,
                   pDlStrategyPerUEPerTTI->secondaryDCIFormat);

        }
        if (ttiIndex == 0)
        {
            ttiIndex = MAX_TTI_STATS - 1;
        }
    }
}
    /* CA Stats Changes End */
}

/****************************************************************************
* Function Name  : printfUeCommonInfoStats
* Inputs         : None
* Outputs        : none
* Returns        : none
* Description    : This function prints the PDSCH stats for common UE Info
****************************************************************************/
/* +- SPR 18490 */
void printfUeCommonInfoStats(void)
/* +- SPR 18490 */
{
    UInt32 ueIdx;
    UInt16 lcIndex;
    LteUEInfoStat *pUeInfoStat= PNULL;
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;
    UInt16  numActiveUe = 0;

    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf(" #################### Cell [%d] #####################\n", cellIndex);
        numActiveUe = gMacStats.pStats->numActiveUe[cellIndex];
    for(ueIdx = 0; ueIdx < numActiveUe; ueIdx++)
    {
        pUeInfoStat = &gMacStats.pStats->ueCommonInfoStats[ueIdx].ueInfoStat;

        if(TRUE == pUeInfoStat->isUeActive)
        {

            printf("\n-------------------- UE ID [%d] START ------------------\n",ueIdx);

            printf(
                    "rnti = [%d] \n"
                    "ueTxnMode = [%d] \n"
                    "ueCategory = [%d] \n"
                    "srbCount = [%d] \n"
                    "dlGbrLcCount = [%d] \n"
                    "ulGbrLcCount = [%d] \n"
                    "dlNonGbrLcCount = [%d] \n"
                    "ulNonGbrLcCount = [%d] \n"
                    "dlEligibleLcCount = [%d] \n"
                    "ulEligibleLcCount = [%d] \n"
                        "ulAmbr = [%d] \n"
                        "dlAmbr = [%u] \n"
                        "ulScheduledCount = [%u] \n"
                    "dlScheduledCount = [%d] \n"
                    "ulTreeLcCount = [%d] \n"
                    "dlTreeLcCount = [%d] \n"
                    "zeroNonZeroCount = [%d] \n"
                    "nonZeroZeroCount = [%d] \n"
                    "srbTokenCount = [%d] \n"
                    "dlDrxOnCount = [%d] \n"
                    "dlMeasGapOnCount = [%d] \n"
                    "dlSyncLossOnCount = [%d] \n"
                    "dlDrxOffCount = [%d] \n"
                    "dlMeasGapOffCount = [%d] \n"
                    "dlSyncLossOffCount = [%d] \n"
                    "ulDrxOnCount = [%d] \n"
                    "ulMeasGapOnCount = [%d] \n"
                    "ulSyncLossOnCount = [%d] \n"
                    "ulDrxOffCount = [%d] \n"
                    "ulMeasGapOffCount = [%d] \n"
                    "ulSyncLossOffCount = [%d] \n"
                    "dlNewTxnCount = [%d] \n"
                    "dlReTxnCount = [%d] \n"
                    "ulNewTxnCount = [%d] \n"
                    "ulReTxnCount = [%d] \n"
                    "dlStrategyFailCount = [%d] \n"
                    "ulStrategyFailCount = [%d] \n"
                    "pdcchFailCount = [%d] \n"
                    "pdschFailCount = [%d] \n"
                    "puschFailCount = [%d] \n"
                    "dlAmbrTokenOverflow = [%d] \n"
                    "ulAmbrTokenOverflow = [%d] \n"
                    "dlScheduleFlag = [%d] \n"
                    "ulScheduleFlag = [%d] \n",

                pUeInfoStat->rnti,               
                pUeInfoStat->ueTxnMode,          
                pUeInfoStat->ueCategory,         
                pUeInfoStat->srbCount,           
                pUeInfoStat->dlGbrLcCount,       
                pUeInfoStat->ulGbrLcCount,       
                pUeInfoStat->dlNonGbrLcCount,    
                pUeInfoStat->ulNonGbrLcCount,    
                pUeInfoStat->dlEligibleLcCount,  
                pUeInfoStat->ulEligibleLcCount,  
                pUeInfoStat->ulAmbr,             
                pUeInfoStat->dlAmbr,             
                pUeInfoStat->ulScheduledCount,   
                pUeInfoStat->dlScheduledCount,   
                pUeInfoStat->ulTreeLcCount,      
                pUeInfoStat->dlTreeLcCount,      
                pUeInfoStat->dlZeroNonZeroCount,   
                pUeInfoStat->dlNonZeroZeroCount,   
                pUeInfoStat->srbTokenCount,      
                pUeInfoStat->dlDrxOnCount,       
                pUeInfoStat->dlMeasGapOnCount,   
                pUeInfoStat->dlSyncLossOnCount,  
                pUeInfoStat->dlDrxOffCount,      
                pUeInfoStat->dlMeasGapOffCount,  
                pUeInfoStat->dlSyncLossOffCount, 
                pUeInfoStat->ulDrxOnCount,       
                pUeInfoStat->ulMeasGapOnCount,   
                pUeInfoStat->ulSyncLossOnCount,  
                pUeInfoStat->ulDrxOffCount,      
                pUeInfoStat->ulMeasGapOffCount,  
                pUeInfoStat->ulSyncLossOffCount, 
                pUeInfoStat->dlNewTxnCount,      
                pUeInfoStat->dlReTxnCount,       
                pUeInfoStat->ulNewTxnCount,      
                pUeInfoStat->ulReTxnCount,       
                pUeInfoStat->dlStrategyFailCount,
                pUeInfoStat->ulStrategyFailCount,
                pUeInfoStat->pdcchFailCount,     
                pUeInfoStat->pdschFailCount,     
                pUeInfoStat->puschFailCount,     
                pUeInfoStat->dlAmbrTokenOverflow,
                pUeInfoStat->ulAmbrTokenOverflow,
                pUeInfoStat->dlScheduleFlag,
                pUeInfoStat->ulScheduleFlag
                    );

            printf(" \n ******* DL LC INFO *******\n");
            for(lcIndex = 0; lcIndex < MAX_LCID; lcIndex++)
            {
                if(TRUE ==  pUeInfoStat->dlLCInfoStat[lcIndex].isValid)
                {
                printf("\n------------- LC ID [%d] --------------\n",lcIndex);

                printf(
                        "qci = [%d] "
                        "lcMode = [%d] "
                        "lcId = [%d] "
                        "tokenPriority = [%d] "
                        "bitRate = [%lld] "
                        "tokenOverflow = [%d] "
                        "tokenDepth = [%d] "
                        "tokenRate = [%d] "
                        "totalLCWeight = [%d] "
                        "ctrlFlag = [%d] \n",
                        pUeInfoStat->dlLCInfoStat[lcIndex].qci,
                        pUeInfoStat->dlLCInfoStat[lcIndex].lcMode,
                        pUeInfoStat->dlLCInfoStat[lcIndex].lcId,
                        pUeInfoStat->dlLCInfoStat[lcIndex].tokenPriority,
                        pUeInfoStat->dlLCInfoStat[lcIndex].bitRate,
                        pUeInfoStat->dlLCInfoStat[lcIndex].tokenOverflow,
                        pUeInfoStat->dlLCInfoStat[lcIndex].tokenDepth,
                        pUeInfoStat->dlLCInfoStat[lcIndex].tokenRate,
                        pUeInfoStat->dlLCInfoStat[lcIndex].totalLCWeight,
                        pUeInfoStat->dlLCInfoStat[lcIndex].ctrlFlag
                            );
                }
            }
            printf(" \n ******* UL LC INFO *******\n");
            for(lcIndex = 0; lcIndex < MAX_LCID; lcIndex++)
            {

                if(TRUE ==  pUeInfoStat->ulLCInfoStat[lcIndex].isValid)
                {
                printf("\n------------- LC ID [%d] --------------\n",lcIndex);
                printf(
                        "qci = [%d] "
                        "lcMode = [%d] "
                        "lcId = [%d] "
                        "lcgId = [%d] "
                                "bitRate = [%llu] ",
                        pUeInfoStat->ulLCInfoStat[lcIndex].qci,
                        pUeInfoStat->ulLCInfoStat[lcIndex].lcMode,
			pUeInfoStat->ulLCInfoStat[lcIndex].lcId,
			pUeInfoStat->ulLCInfoStat[lcIndex].lcgId,
			pUeInfoStat->ulLCInfoStat[lcIndex].bitRate);

		printf(
			"tokenOverflow = [%d] "
			"tokenDepth = [%d] "
			"tokenRate = [%d] "
			"tokenPriority = [%d] "
			"totalLCWeight = [%d] ",
			pUeInfoStat->ulLCInfoStat[lcIndex].tokenOverflow,
			pUeInfoStat->ulLCInfoStat[lcIndex].tokenDepth,
			pUeInfoStat->ulLCInfoStat[lcIndex].tokenRate,
                        pUeInfoStat->ulLCInfoStat[lcIndex].tokenPriority,
                        pUeInfoStat->ulLCInfoStat[lcIndex].totalLCWeight
                      );
                }
            }
            printf("\n-------------------- UE ID [%d] END ------------------\n",ueIdx);
        }
    }
}
    /* CA Stats Changes End */
}

/****************************************************************************
* Function Name  : printPdschPerTTIPerUEStats
* Inputs         : None
* Outputs        : none
* Returns        : none
* Description    : This function prints the PDSCH debug stats per TTI per UE
****************************************************************************/
/* +- SPR 18490 */
void printPdschPerTTIPerUEStats(void)
{
    UInt16 ttiIndex = 0;
    LteMacPdschTTIStats *pLteMacPDSCHTTIStats = PNULL;
    LteMacPdschPerTTIPerUEStats *pPdschPerUEPerTTI = PNULL;
    UInt16 numScheduledUE = 0;
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;
    UInt16 writeIndex = 0;

    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf( "\n############# PDSCH Stats for Cell[%d]#################\n", cellIndex);
        writeIndex = gMacStats.pStats->writeTTIIndex[cellIndex];
        ttiIndex = writeIndex;
        do
        {
            pLteMacPDSCHTTIStats = &gMacStats.pStats->pdschTTIStats[cellIndex][ttiIndex];
        if (pLteMacPDSCHTTIStats->isPdschTTIStatsValid)
        {
            if( pLteMacPDSCHTTIStats->numScheduledUETotal )
            {
                printf("absoluteTTI [%lld]   numOfScheduledUERext[%d]  "
                        "numOfScheduledUENewTx [%d] numFailedUeTx [%d] totalUESch [%d]\n",
                        pLteMacPDSCHTTIStats->absoluteTTI,
                        pLteMacPDSCHTTIStats->numScheduledUERetx,
                        pLteMacPDSCHTTIStats->numScheduledUENewTx,
                        pLteMacPDSCHTTIStats->numFailedUeTx,
                        pLteMacPDSCHTTIStats->numScheduledUETotal);
            }
            for ( numScheduledUE = 0; 
                    numScheduledUE < pLteMacPDSCHTTIStats->numScheduledUETotal;
                    numScheduledUE++ )
            {
                pPdschPerUEPerTTI = &pLteMacPDSCHTTIStats->
                    pdschStatsPerScheduledUE[numScheduledUE];

                if ( pPdschPerUEPerTTI->allocationFailureCause > 0 )
                {
                    printf("\nueIndex  [%u]  allocationFailureCause [%u] \n",
                            pPdschPerUEPerTTI->ueIndex,
                            pPdschPerUEPerTTI->allocationFailureCause );
                }
                else
                {
                    printf("\nueIndex  [%u] transmissionType  [%u]"
                            "harqProcessId [%u] tbOneTBSize  [%u]"
                            "tbTwoTBSize  [%u]  codeWordOneMcs [%u]"
                            "allocatedSPSRBs  [%u]  allocatedSPSMcs [%u] "
                            "startSPSVRBIndex [%u]",
                            pPdschPerUEPerTTI->ueIndex,
                            pPdschPerUEPerTTI->transmissionType,
                            pPdschPerUEPerTTI->harqProcessId,
                            pPdschPerUEPerTTI->tbOneTBSize,
                            pPdschPerUEPerTTI->tbTwoTBSize,
                            pPdschPerUEPerTTI->codeWordOneMcs,
                            pPdschPerUEPerTTI->allocatedSPSRBs,
                            pPdschPerUEPerTTI->allocatedSPSMcs,
                            pPdschPerUEPerTTI->startSPSVRBIndex);

                    printf("codeWordTwoMcs [%u] allocatedPRBs [%u]"
                            "isSubBandScheduling [%u] "
                            "dciFormat [%u] ratPoilicy [%u] allocationFailureCause [%u]"
                            "excessPoolRB [%d] resourceAllocationBitMap [%u] " 
                            "SPSResourceAllocationBitMap [%u]\n",
                            pPdschPerUEPerTTI->codeWordTwoMcs,
                            pPdschPerUEPerTTI->allocatedPRBs,
                            pPdschPerUEPerTTI->isSubBandScheduling,
                            pPdschPerUEPerTTI->dciFormat,
                            pPdschPerUEPerTTI->ratPoilicy,
                            pPdschPerUEPerTTI->allocationFailureCause,
                            pPdschPerUEPerTTI->excessPoolRB,
                            pPdschPerUEPerTTI->resourceAllocationBitMap, 
                            pPdschPerUEPerTTI->spsResourceAllocationBitMap);
                }
            }
        }
        if (ttiIndex == 0)
        {
            ttiIndex = MAX_TTI_STATS - 1;
        }
        else
        {
            ttiIndex--;
        }
    } while( ttiIndex != writeIndex );
}
    /* CA Stats Changes End */
}
/****************************************************************************
* Function Name  : printPdschPerTTIPerUEStats
* Inputs         : None
* Outputs        : none
* Returns        : none
* Description    : This function prints the PDSCH debug stats per TTI per UE
****************************************************************************/
    /* +- SPR 18490 */
void printPuschPerTTIPerUEStats(void)
    /* +- SPR 18490 */
{
    UInt16 ttiIndex = 0;
    LteMacPuschTTIStats *pLteMacPUSCHTTIStats = PNULL;
    LteMacPuschPerTTIPerUEStats *pPuschPerUEPerTTI = PNULL;
    UInt16 numScheduledUE = 0;
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;
    UInt16 writeIndex = 0;

    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf( "\n############# PUSCH Stats cell[%d] #################\n", cellIndex );
        writeIndex = gMacStats.pStats->writeTTIIndex[cellIndex];
        ttiIndex = writeIndex;
        do
        {
            pLteMacPUSCHTTIStats = &gMacStats.pStats->puschTTIStats[cellIndex][ttiIndex];
        if( pLteMacPUSCHTTIStats->numScheduledUETotal )
        {
            printf("\nabsoluteTTI [%lld]   numOfScheduledUERext[%d]  "
                    "numOfScheduledUENewTx [%d]  totalUESch [%d]\n",
                    pLteMacPUSCHTTIStats->absoluteTTI,
                    pLteMacPUSCHTTIStats->numScheduledUERetx,
                    pLteMacPUSCHTTIStats->numScheduledUENewTx,
                    pLteMacPUSCHTTIStats->numScheduledUETotal);
        }
        for ( numScheduledUE = 0; 
              numScheduledUE < pLteMacPUSCHTTIStats->numScheduledUETotal;
              numScheduledUE++ )
        {
            pPuschPerUEPerTTI = &pLteMacPUSCHTTIStats->
                                puschStatsPerScheduledUE[numScheduledUE];

            if ( pPuschPerUEPerTTI->allocationFailureCause > 0 )
            {
                printf("\nueIndex  [%u]  allocationFailureCause [%u] \n",
                        pPuschPerUEPerTTI->ueIndex,
                        pPuschPerUEPerTTI->allocationFailureCause );
            }
            else
            {
                printf("ueIndex  [%u] transmissionType  [%u]"
                   "harqProcessId [%u] oppGranted  [%u]"
                   "selectedMCS [%u]"
                   "allocatedSPSRBs  [%u]  allocatedSPSMcs [%u] "
                   "startSPSVRBIndex [%u]",
                    pPuschPerUEPerTTI->ueIndex,
                    pPuschPerUEPerTTI->transmissionType,
                    pPuschPerUEPerTTI->harqProcessId,
                    pPuschPerUEPerTTI->oppGranted,
                    pPuschPerUEPerTTI->selectedMCS,
                    pPuschPerUEPerTTI->allocatedSPSRBs,
                    pPuschPerUEPerTTI->allocatedSPSMcs,
                    pPuschPerUEPerTTI->startSPSVRBIndex);

                printf(" allocatedPRBs [%u]"
                   "frequencySeletiveScheduling [%d]"
                   " allocationFailureCause [%u]"
                   "SPSResourceAllocationBitMap [%u]\n",
                    pPuschPerUEPerTTI->allocatedPRBs,
                    pPuschPerUEPerTTI->frequencySeletiveScheduling,
                    pPuschPerUEPerTTI->allocationFailureCause,
                    pPuschPerUEPerTTI->spsResourceAllocationBitMap);
            }
        }
        if (ttiIndex == 0)
        {
            ttiIndex = MAX_TTI_STATS - 1;
        }
        else
        {
            ttiIndex--;
        }
    } while( ttiIndex != writeIndex );
}
    /* CA Stats Changes End */
}

/****************************************************************************
* Function Name  : printPdschRMStats
* Inputs         : None
* Outputs        : none
* Returns        : none
* Description    : This function prints the PDSCH stats for common UE Info
****************************************************************************/
/* +- SPR 18490 */
void printPdschRMStats(void)
{
    UInt8  idx, idx1 = 0;
    UInt16 ttiIndex = 0;
    LP_LteSpsDlRmStats  spsDlRmStats_p = PNULL;
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;
    UInt16 writeIndex = 0;
    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf(" #################### Cell [%d] #####################\n", cellIndex);
        writeIndex = gMacStats.pStats->writeTTIIndex[cellIndex];
        QoSUEAndPerLCTTIStat *spsSysWideStats_p = 
            &gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex];

        ttiIndex = writeIndex;
        do
        {
            spsDlRmStats_p = &gMacStats.pStats->dlSpsRmStats[cellIndex][ttiIndex];
        if( spsDlRmStats_p->isDlSpsRmStatsValid )
        {
            printf( "\n##### Absolute TTI [%llu] #####\n", 
                    spsDlRmStats_p->absoluteTTI );
            for( idx = 0 ;idx < MAX_DL_SPS_INTERVAL; idx++ )

            {
                printf("\nSPS ttiIndex [%d] \ndlSpsRbsPerTTI [%d] "
                       "numDlSpsDciPerTti [%d] numDlSpsOccasionPerTti [%d]",
                       idx, spsDlRmStats_p->dlSpsRbsPerTTI[idx],
                       spsDlRmStats_p->dlSpsDciPerTTI[idx],
                       spsDlRmStats_p->dlSpsOccasionPerTTI[idx] );
                for( idx1 = 0; idx1 < MAX_SPS_INTERVALS; idx1++ )
                {
                    if( spsSysWideStats_p->supportedSpsDlIntervalList[idx1] )
                    {
                        printf( "dlSpsRbsPerTTIPerInterval [%d] for %d "
                                "perioducity;",
                                spsDlRmStats_p->dlSpsRbsPerTTIPerInterval
                                                               [idx][idx1],
                                spsIntervalMap_g[idx1] ); 
                    }
                }
            }
        }
        if (ttiIndex == 0)
        {
            ttiIndex = MAX_TTI_STATS - 1;
        }
        else
        {
            ttiIndex--;
        }
    } while( ttiIndex != writeIndex );
}
    /* CA Stats Changes End */
}

/****************************************************************************
* Function Name  : printPuschRMStats
* Inputs         : None
* Outputs        : none
* Returns        : none
* Description    : This function prints the PDSCH stats for common UE Info
****************************************************************************/
/* +- SPR 18490 */
void printPuschRMStats(void)
{
    UInt8  idx, idx1 = 0;
    UInt16 ttiIndex = 0;
    LP_LteSpsUlRmStats  spsUlRmStats_p = PNULL;
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;
    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf("\n*************************CELL [%d]***********************\n", cellIndex);
        UInt16 writeIndex = gMacStats.pStats->writeTTIIndex[cellIndex];
        QoSUEAndPerLCTTIStat *spsSysWideStats_p = 
            &gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex];

        ttiIndex = writeIndex;
        do
        {
            spsUlRmStats_p = &gMacStats.pStats->ulSpsRmStats[cellIndex][ttiIndex];
        if( spsUlRmStats_p->isUlSpsRmStatsValid )
        {
            printf( "\n##### Absolute TTI [%llu] #####\n", 
                    spsUlRmStats_p->absoluteTTI );
            for( idx = 0 ;idx < MAX_UL_SPS_INTERVAL; idx++ )
            {
                printf("\nSPS ttiIndex [%d] \nulSpsRbsPerTTI [%d] "
                       "numUlSpsDciPerTti [%d] numUlSpsOccasionPerTti [%d]",
                       idx, spsUlRmStats_p->ulSpsRbsPerTTI[idx],
                       spsUlRmStats_p->ulSpsDciPerTTI[idx],
                       spsUlRmStats_p->ulSpsOccasionPerTTI[idx] );
                for( idx1 = 0; idx1 < MAX_SPS_INTERVALS; idx1++ )
                {
                    if( spsSysWideStats_p->supportedSpsUlIntervalList[idx1] )
                    {
                        printf( "ulSpsRbsPerTTIPerInterval [%d] for %d "
                                "perioducity;",
                                spsUlRmStats_p->ulSpsRbsPerTTIPerInterval
                                                               [idx][idx1],
                                spsIntervalMap_g[idx1] ); 
                    }
                }
            }
        }
        if (ttiIndex == 0)
        {
            ttiIndex = MAX_TTI_STATS - 1;
        }
        else
        {
            ttiIndex--;
        }
    } while( ttiIndex != writeIndex );
}
    /* CA Stats Changes End */
}
/****************************************************************************
* Function Name  : printfPdschUeCommonInfoStats
* Inputs         : None
* Outputs        : none
* Returns        : none
* Description    : This function prints the PDSCH stats for common UE Info
****************************************************************************/
/* +- SPR 18490 */
void printfPdschUeCommonInfoStats(void)
{
    UInt32 ueIdx;
    UInt16 pdschIdx = 0;
    UInt16 ttiPosIdx;
    UInt16 uePosIndex;
    UInt64 ueAbsTTI;
	UInt16 pdschScheduledTTIIndex = 0;
    LteUECommonInfoStats *pUECommonInfoStats = PNULL;
    LteMacPdschPerTTIPerUEStats *pPdschPerUEPerTTI = PNULL;
    LteMacPdschTTIStats *pLteMacPDSCHTTIStats = PNULL;
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;
    UInt16  numActiveUe = 0;

    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf(" #################### Cell [%d] #####################\n", cellIndex);
        numActiveUe = gMacStats.pStats->numActiveUe[cellIndex];

        for(ueIdx = 0; ueIdx < numActiveUe; ueIdx++)
        {
            pUECommonInfoStats = &gMacStats.pStats->ueCommonInfoStats[ueIdx];

            pdschIdx = pUECommonInfoStats->pdschCurWriteIndex[cellIndex];

        while(pdschScheduledTTIIndex < MAX_TTI_STATS)
        {
        	if(!pdschIdx)
        	{
            	pdschIdx = MAX_TTI_STATS;
        	}

                ueAbsTTI = pUECommonInfoStats->pdschScheduledTTIIndex[cellIndex]
                                                  [pdschIdx-1].absoluteTTI;

                ttiPosIdx = (ueAbsTTI - 1)%MAX_TTI_STATS;

                pLteMacPDSCHTTIStats = &gMacStats.pStats->pdschTTIStats[cellIndex][ttiPosIdx];
            
                uePosIndex = pUECommonInfoStats->pdschScheduledTTIIndex[cellIndex]
                                                    [pdschIdx-1].ueIndexPos;
             
            if ( ueAbsTTI == pLteMacPDSCHTTIStats->absoluteTTI )
            {
                pPdschPerUEPerTTI = &pLteMacPDSCHTTIStats->
                                      pdschStatsPerScheduledUE[uePosIndex];

                printf("ueIndex  [%d] transmissionType  [%d]"
                   "harqProcessId [%d] tbOneTBSize  [%d]"
                   "tbTwoTBSize  [%d]  codeWordOneMcs [%d]"
                   "codeWordTwoMcs [%d] allocatedPRBs [%d]"
                   "isSubBandScheduling [%d] resourceAllocationBitMap [%d]"
                   "dciFormat [%d] ratPoilicy [%d] excessPoolRB [%d]"
                   "allocationFailureCause [%d]\n",
                    pPdschPerUEPerTTI->ueIndex,
                    pPdschPerUEPerTTI->transmissionType,
                    pPdschPerUEPerTTI->harqProcessId,
                    pPdschPerUEPerTTI->tbOneTBSize,
                    pPdschPerUEPerTTI->tbTwoTBSize,
                    pPdschPerUEPerTTI->codeWordOneMcs,
                    pPdschPerUEPerTTI->codeWordTwoMcs,
                    pPdschPerUEPerTTI->allocatedPRBs,
                    pPdschPerUEPerTTI->isSubBandScheduling,
                    pPdschPerUEPerTTI->resourceAllocationBitMap,
                    pPdschPerUEPerTTI->dciFormat,
                    pPdschPerUEPerTTI->ratPoilicy,
                    pPdschPerUEPerTTI->excessPoolRB,
                    pPdschPerUEPerTTI->allocationFailureCause);

                pdschIdx--;
            }
            else
            {
                break;
            }
			pdschScheduledTTIIndex++;
        }
    }
}
    /* CA Stats Changes End */
}
/*UL strategy stats start*/
/****************************************************************************
 * * Function Name  : printUlStrategyPerUE
 * * Inputs         : None
 * * Outputs        : none
 * * Returns        : none
 * * Description    : This function prints the UL Strategy Per UE debug stats 
 * ****************************************************************************/
void printUlStrategyPerUE(LteMacUlStrategyPerTTIPerUEStats *pLteMacUlStrategyPerTTIPerUEStats)
{
    printf("\nueIndex  [%d] minTBSize  [%d]"
            "maxTBSize [%d] ulGrantType  [%d]"
            "ueAvgMCS  [%d] scheduleTBSize [%d]"  
#ifdef TDD_CONFIG
            "harqProcessId [%d]"
#endif
            "scheduledRB [%d] scheduleLcCount [%d]"
            "eligibleLcCount [%d]\n\n ",
            pLteMacUlStrategyPerTTIPerUEStats->ueIndex,
            pLteMacUlStrategyPerTTIPerUEStats->minTBSize,
            pLteMacUlStrategyPerTTIPerUEStats->maxTBSize,
            pLteMacUlStrategyPerTTIPerUEStats->ulGrantType,
            pLteMacUlStrategyPerTTIPerUEStats->ueAvgMCS,
            pLteMacUlStrategyPerTTIPerUEStats->tbSize,
#ifdef TDD_CONFIG
            pLteMacUlStrategyPerTTIPerUEStats->harqProcessId,
#endif
            pLteMacUlStrategyPerTTIPerUEStats->scheduledRB,
            pLteMacUlStrategyPerTTIPerUEStats->scheduleLcCount,
            pLteMacUlStrategyPerTTIPerUEStats->eligibleLcCount);

}
 
/****************************************************************************
 * * Function Name  : printULStrategyPerTTIPerUEStats
 * * Inputs         : None
 * * Outputs        : none
 * * Returns        : none
 * * Description    : This function prints the UL Strategy debug stats per 
 * *                  TTI per UE
 * ****************************************************************************/
void printULStrategyPerTTIPerUEStats()
{
    UInt16 ttiIndex = 0;
    LteMacUlStrategyTTIStats *pLteMacUlStrategyTTIStats = PNULL;
    LteMacUlStrategyPerTTIPerUEStats *pLteMacUlStrategyPerTTIPerUEStats = PNULL;
    UInt16 numScheduledUE = 0;
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;

    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf(" #################### Cell [%d] #####################\n", cellIndex);
        for (ttiIndex = gMacStats.pStats->writeTTIIndex[cellIndex] - 2;
                ttiIndex < gMacStats.pStats->writeTTIIndex[cellIndex]; ttiIndex--)
        {
            pLteMacUlStrategyTTIStats = &gMacStats.pStats->ulStrategyTTIStats[cellIndex][ttiIndex];

            printf("\n##################AbsoluteTTI [%llu] #################\n",
                pLteMacUlStrategyTTIStats->absoluteTTI);

        printf("  failedULGrants :[%d]\n failedAndPendingSR :[%d]\n failedAndPendingCR :[%d]\n",
                pLteMacUlStrategyTTIStats->failedULGrants,
                pLteMacUlStrategyTTIStats->failedAndPendingSR,
                pLteMacUlStrategyTTIStats->failedAndPendingCR);

        printf(" failedAndPendingApCQI :[%d]\n pendingULGrants :[%d]\n scheduledReTXUE :[%d]\n scheduledSR :[%d]\n"
                " scheduledCR :[%d]\n scheduledApCQI :[%d]\n scheduledDataApCQI :[%d]\n scheduledTxUE :[%d]\n scheduledTotalUE :[%d] \n",
                pLteMacUlStrategyTTIStats->failedAndPendingApCQI,
                pLteMacUlStrategyTTIStats->pendingULGrants,
                pLteMacUlStrategyTTIStats->scheduledReTXUE,
                pLteMacUlStrategyTTIStats->scheduledSR,
                pLteMacUlStrategyTTIStats->scheduledCR,
                pLteMacUlStrategyTTIStats->scheduledApCQI,
                pLteMacUlStrategyTTIStats->scheduledDataApCQI,
                pLteMacUlStrategyTTIStats->scheduledTxUE,
                pLteMacUlStrategyTTIStats->scheduledTotalUE);

        for ( numScheduledUE = 0;
                numScheduledUE < pLteMacUlStrategyTTIStats->scheduledTotalUE;
                numScheduledUE++ )
        {
            pLteMacUlStrategyPerTTIPerUEStats = &pLteMacUlStrategyTTIStats->
                ulStrategyPerUE[numScheduledUE];
            printUlStrategyPerUE(pLteMacUlStrategyPerTTIPerUEStats);
        }
        if (ttiIndex == 0)
        {
            ttiIndex = MAX_TTI_STATS - 1;
        }
    }
}
    /* CA Stats Changes End */
}

/****************************************************************************
 * * Function Name  : printULStrategyPerTTIPerLCStats
 * * Inputs         : None
 * * Outputs        : none
 * * Returns        : none
 * * Description    : This function prints the UL strategy debug stats per TTI 
 * * per LC
 * ****************************************************************************/
/* +- SPR 18490 */
void printULStrategyPerTTIPerLCStats(void)
{
    UInt16 ttiIndex = 0;
    UInt16 counter = 0;
    LteQosULStrategyLCTTIPerLcStat *pLteQosULStrategyLCTTIPerLcStat = PNULL;
    LteQosULStrategyLCTTIStat      *pLteQosULStrategyLCTTIStat = PNULL;
    UInt16 lcIndex = 0;
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;

    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf(" #################### Cell [%d] #####################\n", cellIndex);
        for (ttiIndex = gMacStats.pStats->writeTTIIndex[cellIndex] ,counter = 0;
            counter < MAX_TTI_STATS;++ttiIndex, ++counter)
    {
            pLteQosULStrategyLCTTIStat = &gMacStats.pStats->ulStrategyLcTTIStats[cellIndex][ttiIndex];

            printf("########### Absolute TTI [%llu] #################\n",pLteQosULStrategyLCTTIStat->absoluteTTI);
        printf("########### numScheduledLC [%d] #################\n",pLteQosULStrategyLCTTIStat->numScheduledLC);

        for (lcIndex = 0; lcIndex < pLteQosULStrategyLCTTIStat->numScheduledLC; ++lcIndex)
        {
            pLteQosULStrategyLCTTIPerLcStat = &pLteQosULStrategyLCTTIStat->ulStrategyPerLC[lcIndex];

            printf("ueId [%d] lcId [%d] BSRLoad[%d]  Qload[%d] "
                    "scheduledBytes [%d] resheduledBytes [%d]\n"
                    "headOfLineDelay [%d] drxOnDurationDelay [%d] availToken [%d] "
                    "totalLcWeight [%d] totTknSchedWndw [%d]\n\n",
                    pLteQosULStrategyLCTTIPerLcStat->ueIndex,
                    pLteQosULStrategyLCTTIPerLcStat->lcId,
                    pLteQosULStrategyLCTTIPerLcStat->BSRLoad,
                    pLteQosULStrategyLCTTIPerLcStat->qLoad,
                    pLteQosULStrategyLCTTIPerLcStat->scheduledBytes,
                    pLteQosULStrategyLCTTIPerLcStat->resheduledBytes,
                    pLteQosULStrategyLCTTIPerLcStat->headOfLineDelay,
                    pLteQosULStrategyLCTTIPerLcStat->drxOnDurationDelay,
                    pLteQosULStrategyLCTTIPerLcStat->availToken,
                    pLteQosULStrategyLCTTIPerLcStat->totalLcWeight,
                    pLteQosULStrategyLCTTIPerLcStat->totTknSchedWndw);
        }  

        if (ttiIndex == MAX_TTI_STATS - 1)
        {
            ttiIndex = 0;
        }
    }
}
    /* CA Stats Changes End */
}

void printulStrategyLcStats(LCScheduledTTIInfo *pLCScheduledTTIInfo)
{
    UInt8  lcCurWriteIndex = 0;

    printf("Absolute TTI : %llu\n",pLCScheduledTTIInfo->absoluteTTI);
    for(lcCurWriteIndex = 0; lcCurWriteIndex < pLCScheduledTTIInfo->lcCurWriteIndex; lcCurWriteIndex++)
    {
        printf("LC Index pos : %d\n",pLCScheduledTTIInfo->lcIndexPos[lcCurWriteIndex]);
    }

}

/****************************************************************************
 * Function Name  : printfULStartegyLcCommonInfoStats
 * Inputs         : None
 * Outputs        : none
 * Returns        : none
 * Description    : This function prints the PDSCH stats for common UE Info
 ****************************************************************************/
/* +- SPR 18490 */
void printfULStartegyLcCommonInfoStats(void)
{
    UInt32 ueIdx;
    UInt64 lcAbsTTI;
    UInt16 lcPosIndex;
    UInt16 lcIndex;
    UInt16 lcCount;
    UInt16 ttiIndex;
    UInt16 ulStrategyLcIndex = 0;
    LteUECommonInfoStats *pUECommonInfoStats = PNULL;
    LteQosULStrategyLCTTIStat *pLteMacULStrategyLCTTIStats = PNULL;
    LteQosULStrategyLCTTIPerLcStat *pUlStrategyPerLCPerTTI = PNULL;
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;
    UInt16  numActiveUe = 0;

    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf(" #################### Cell [%d] #####################\n", cellIndex);
        numActiveUe = gMacStats.pStats->numActiveUe[cellIndex];
    printf("\n\n################## UE Common Info stats ###################\n\n");
    for(ueIdx = 0; ueIdx < numActiveUe; ueIdx++)
    {
        pUECommonInfoStats = &gMacStats.pStats->ueCommonInfoStats[ueIdx];

        ulStrategyLcIndex = pUECommonInfoStats->ulStrategyLcCurWriteIndex;
        if(!ulStrategyLcIndex)
        {
            ulStrategyLcIndex = MAX_TTI_STATS;
        }

        while(1)
        {
            lcAbsTTI = (pUECommonInfoStats->ulstrategyLcTTIIndex[ulStrategyLcIndex - 1].
                    absoluteTTI);
                ttiIndex = (lcAbsTTI-1) % MAX_TTI_STATS;

            lcCount = pUECommonInfoStats->ulstrategyLcTTIIndex[ulStrategyLcIndex - 1].
                lcCurWriteIndex;

                pLteMacULStrategyLCTTIStats = &gMacStats.pStats->ulStrategyLcTTIStats[cellIndex][ttiIndex];

            if(lcAbsTTI == pLteMacULStrategyLCTTIStats->absoluteTTI)
            {
                lcIndex = 0;
                while(lcIndex < lcCount)
                {
                    lcPosIndex = pUECommonInfoStats->ulstrategyLcTTIIndex[ulStrategyLcIndex - 1].
                        lcIndexPos[lcIndex]; 

                    pUlStrategyPerLCPerTTI = &pLteMacULStrategyLCTTIStats->
                        ulStrategyPerLC[lcPosIndex];

                    printf("ueIndex,     lcId,    qLoad, scheduledBytes, resheduledBytes, headOfLineDelay,"
                            "drxOnDurationDelay, availToken, totalLcWeight, totTknSchedWndw\n");

                    printf("[%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t [%d]\t \n",
                            pUlStrategyPerLCPerTTI->ueIndex,
                            pUlStrategyPerLCPerTTI->lcId,
                            pUlStrategyPerLCPerTTI->BSRLoad,
                            pUlStrategyPerLCPerTTI->scheduledBytes,
                            pUlStrategyPerLCPerTTI->resheduledBytes,
                            pUlStrategyPerLCPerTTI->headOfLineDelay,
                            pUlStrategyPerLCPerTTI->drxOnDurationDelay,
                            pUlStrategyPerLCPerTTI->availToken,
                            pUlStrategyPerLCPerTTI->totalLcWeight,
                            pUlStrategyPerLCPerTTI->totTknSchedWndw);

                }
            }
            else
            {
                break;
            }
        }
        ulStrategyLcIndex--;
    }
}
    /* CA Stats Changes End */
}

/****************************************************************************
 * Function Name  : printfULStrategyUeCommonInfoStats
 * Inputs         : None
 * Outputs        : none
 * Returns        : none
 * Description    : This function prints the PDSCH stats for common UE Info
 ****************************************************************************/
/* +- SPR 18490 */
void printfULStrategyUeCommonInfoStats(void)
{
    UInt64 tti = 0;
    UInt16 ttiIndex = 0;
    UInt16 uePos = 0;
    UInt16 ueId = 0;
    UInt16 ulWriteIndex = 0;
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;

    //for (ueId = 0; ueId < MAX_UE_SUPPORTED; ueId++)
    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf(" #################### Cell [%d] #####################\n", cellIndex);
    for (ueId = 0; ueId < 1; ueId++)
    {
        ulWriteIndex = gMacStats.pStats->ueCommonInfoStats[ueId].ulStrategyCurWriteIndex;

        if(!ulWriteIndex)
        {
            ulWriteIndex = MAX_TTI_STATS;
        }
    


        while(1)
        {
            tti = gMacStats.pStats->ueCommonInfoStats[ueId].ulstrategyScheduledTTIIndex[ulWriteIndex - 1].absoluteTTI;
            ttiIndex = (tti-1) % MAX_TTI_STATS;

            uePos = gMacStats.pStats->ueCommonInfoStats[ueId].ulstrategyScheduledTTIIndex[ulWriteIndex].ueIndexPos;

                if (tti == gMacStats.pStats->ulStrategyTTIStats[cellIndex][ttiIndex].absoluteTTI)
            {
                /*Print UL TTI stats*/

                /*Print UL TTI per UE stat*/
                    printUlStrategyPerUE(&gMacStats.pStats->ulStrategyTTIStats[cellIndex][ttiIndex].ulStrategyPerUE[uePos]);
            }
            else
            {
                break;
            }
        }
        ulWriteIndex--; 
    }
}
    /* CA Stats Changes End */
}
/* KPI_COMPILATION_BREAK */
#ifdef UTFWK_SIMULATION
#ifdef KPI_STATS
/* +- SPR 18490 */
void printKPIStats(void)
{

    UInt8 kpiBitmap = 0;
    UInt8 kpiBit = 0;
    LteMacKPIStats *lteMacKPIStatsForPrinting_p = &(gMacStats.pStats->lteCommonKPIStats.lteMacKPIStatsForPrinting);
    LteMacKPIStats *lteMacKPIStats =  &(gMacStats.pStats->lteCommonKPIStats.lteMacKPIStats[0]);

    printf("################KPI STATS######################## \n");

    printf("Duration for Kpi Stats [%u]seconds ",gMacStats.pStats->lteCommonKPIStats.lteMacKPIConfig.durationTTI/1000);

    printf("Start TTI[%u]  ",
            gMacStats.pStats->lteCommonKPIStats.finalTTI - gMacStats.pStats->lteCommonKPIStats.lteMacKPIConfig.durationTTI);
    printf("End TTI[%u] \n",gMacStats.pStats->lteCommonKPIStats.finalTTI);

    printf("\n");

    kpiBitmap = gMacStats.pStats->lteCommonKPIStats.lteMacKPIConfig.kpiBitmap;
    for (kpiBit =0 ; kpiBit < 7; kpiBit++)
    {
        if (kpiBitmap & (1<<kpiBit))
        {
            switch (kpiBit)
            {
                case KPI_TOTAL_PRB_USAGE_DL:
                printf("Total DL PRB Usage[%u%]\n",lteMacKPIStatsForPrinting_p->totalDLPRBUsage);
                printf("\n");
                break;

                case KPI_TOTAL_PRB_USAGE_UL:
                printf("Total UL PRB Usage[%u%]\n",lteMacKPIStatsForPrinting_p->totalUplinkPRBUsage);
                printf("\n");
                break;

                case KPI_TOTAL_PRB_USAGE_PER_QCI_DL:
                printf("DL PRB Usage Per QCI 1 [%u%]\n",lteMacKPIStatsForPrinting_p->totalDLPRBUsagePerQCI[0]);
                printf("DL PRB Usage Per QCI 2 [%u%]\n",lteMacKPIStatsForPrinting_p->totalDLPRBUsagePerQCI[1]);
                printf("DL PRB Usage Per QCI 3 [%u%]\n",lteMacKPIStatsForPrinting_p->totalDLPRBUsagePerQCI[2]);
                printf("DL PRB Usage Per QCI 4 [%u%]\n",lteMacKPIStatsForPrinting_p->totalDLPRBUsagePerQCI[3]);
                printf("DL PRB Usage Per QCI 5 [%u%]\n",lteMacKPIStatsForPrinting_p->totalDLPRBUsagePerQCI[4]);
                printf("DL PRB Usage Per QCI 6 [%u%]\n",lteMacKPIStatsForPrinting_p->totalDLPRBUsagePerQCI[5]);
                printf("DL PRB Usage Per QCI 7 [%u%]\n",lteMacKPIStatsForPrinting_p->totalDLPRBUsagePerQCI[6]);
                printf("DL PRB Usage Per QCI 8 [%u%]\n",lteMacKPIStatsForPrinting_p->totalDLPRBUsagePerQCI[7]);
                printf("DL PRB Usage Per QCI 9 [%u%]\n",lteMacKPIStatsForPrinting_p->totalDLPRBUsagePerQCI[8]);
                printf("\n");
                break;

                case KPI_TOTAL_PRB_USAGE_PER_QCI_UL:
                printf("UL PRB Usage Per QCI 1 [%u%]\n",lteMacKPIStatsForPrinting_p->totalULPRBUsagePerQCI[0]);
                printf("UL PRB Usage Per QCI 2 [%u%]\n",lteMacKPIStatsForPrinting_p->totalULPRBUsagePerQCI[1]);
                printf("UL PRB Usage Per QCI 3 [%u%]\n",lteMacKPIStatsForPrinting_p->totalULPRBUsagePerQCI[2]);
                printf("UL PRB Usage Per QCI 4 [%u%]\n",lteMacKPIStatsForPrinting_p->totalULPRBUsagePerQCI[3]);
                printf("UL PRB Usage Per QCI 5 [%u%]\n",lteMacKPIStatsForPrinting_p->totalULPRBUsagePerQCI[4]);
                printf("UL PRB Usage Per QCI 6 [%u%]\n",lteMacKPIStatsForPrinting_p->totalULPRBUsagePerQCI[5]);
                printf("UL PRB Usage Per QCI 7 [%u%]\n",lteMacKPIStatsForPrinting_p->totalULPRBUsagePerQCI[6]);
                printf("UL PRB Usage Per QCI 8 [%u%]\n",lteMacKPIStatsForPrinting_p->totalULPRBUsagePerQCI[7]);
                printf("UL PRB Usage Per QCI 9 [%u%]\n",lteMacKPIStatsForPrinting_p->totalULPRBUsagePerQCI[8]);
                printf("\n");
                break;

                case KPI_NUMBER_OF_ACTIVE_UE_PER_QCI_DL:
                printf("DL Active Users Per QCI 1 [%u]\n",lteMacKPIStatsForPrinting_p->totalActiveUsersPerQCIDLCumulative[0]);
                printf("DL Active Users Per QCI 2 [%u]\n",lteMacKPIStatsForPrinting_p->totalActiveUsersPerQCIDLCumulative[1]);
                printf("DL Active Users Per QCI 3 [%u]\n",lteMacKPIStatsForPrinting_p->totalActiveUsersPerQCIDLCumulative[2]);
                printf("DL Active Users Per QCI 4 [%u]\n",lteMacKPIStatsForPrinting_p->totalActiveUsersPerQCIDLCumulative[3]);
                printf("DL Active Users Per QCI 5 [%u]\n",lteMacKPIStatsForPrinting_p->totalActiveUsersPerQCIDLCumulative[4]);
                printf("DL Active Users Per QCI 6 [%u]\n",lteMacKPIStatsForPrinting_p->totalActiveUsersPerQCIDLCumulative[5]);
                printf("DL Active Users Per QCI 7 [%u]\n",lteMacKPIStatsForPrinting_p->totalActiveUsersPerQCIDLCumulative[6]);
                printf("DL Active Users Per QCI 8 [%u]\n",lteMacKPIStatsForPrinting_p->totalActiveUsersPerQCIDLCumulative[7]);
                printf("DL Active Users Per QCI 9 [%u]\n",lteMacKPIStatsForPrinting_p->totalActiveUsersPerQCIDLCumulative[8]);
                printf("\n");
                break;

                case KPI_NUMBER_OF_ACTIVE_UE_PER_QCI_UL:
                printf("UL Active Users Per QCI 1 [%u]\n",lteMacKPIStatsForPrinting_p->totalActiveUsersPerQCIULCumulative[0]);
                printf("UL Active Users Per QCI 2 [%u]\n",lteMacKPIStatsForPrinting_p->totalActiveUsersPerQCIULCumulative[1]);
                printf("UL Active Users Per QCI 3 [%u]\n",lteMacKPIStatsForPrinting_p->totalActiveUsersPerQCIULCumulative[2]);
                printf("UL Active Users Per QCI 4 [%u]\n",lteMacKPIStatsForPrinting_p->totalActiveUsersPerQCIULCumulative[3]);
                printf("UL Active Users Per QCI 5 [%u]\n",lteMacKPIStatsForPrinting_p->totalActiveUsersPerQCIULCumulative[4]);
                printf("UL Active Users Per QCI 6 [%u]\n",lteMacKPIStatsForPrinting_p->totalActiveUsersPerQCIULCumulative[5]);
                printf("UL Active Users Per QCI 7 [%u]\n",lteMacKPIStatsForPrinting_p->totalActiveUsersPerQCIULCumulative[6]);
                printf("UL Active Users Per QCI 8 [%u]\n",lteMacKPIStatsForPrinting_p->totalActiveUsersPerQCIULCumulative[7]);
                printf("UL Active Users Per QCI 9 [%u]\n",lteMacKPIStatsForPrinting_p->totalActiveUsersPerQCIULCumulative[8]);
                printf("\n\n\n");
                break;

                case KPI_PKT_DELAY_PER_QCI_DL:
                for(qci = 0; qci < MAX_QCI ; qci++)
                {
                 if(lteMacKPIStats->totalPktAckedInDLPerQCI[qci] != 0)
                 printf("PACKET DELAY FOR QCI [%d] [%u]\n",qci+1,(lteMacKPIStats->totalPktDelayInDLPerQCI[qci]/
                                                                     lteMacKPIStats->totalPktAckedInDLPerQCI[qci]));
                 printf("\n\n\n");

                } 
                
                break;
            }
        }
    }
}

#endif
#endif

/* +- SPR 18490 */
void printQoSULUELCStats(void)
{
    UInt32 ueId = 0;
    UInt32 lcId = 0;
    UInt32 count = 0;
    UInt32 newCount = 0;
    float thruput = 0.0;
    UInt64 ul_thruput_tick = 0;
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;
    UInt16  numActiveUe = 0;

    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        printf(" #################### Cell [%d] #####################\n", cellIndex);
        numActiveUe = gMacStats.pStats->numActiveUe[cellIndex];
        printf ("Num of Active UE in Cell[%d]=%d\n", cellIndex, numActiveUe);
    
    printf("\n######################### PER TTI stats UL Started########################\n");
    for (count = 0; count < MAX_TTI_STATS; count++)
    {
            printf("\n######################TTI  %d global Tick %d ###########################\n",count,
                    gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].currentTickValue[count]);
        for (ueId = 0; ueId < numActiveUe; ueId++)
        {
            printf("\n");
            for (lcId = MIN_LC_VALUE; lcId < MAX_LCID; lcId++)
            {
                printf("\n");
                printf("Ue[%d],",ueId);
                printf("lc[%d],",lcId);
                printf("MCS[%d],QLPri[%d],PDBPri[%d],TknPri[%d],",
                            gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].qoSUELCStat[ueId].mcs,
                            gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcId].STX[count%MAX_TTI_STATS].qLoadPriority,
                            gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcId].STX[count%MAX_TTI_STATS].pdbPriority,
                            gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcId].STX[count%MAX_TTI_STATS].tokenPriority);
                printf("AllcGrntT[%d],",
                            gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcId].STX[count%MAX_TTI_STATS].totSchdTknInWindow);

                printf("ExpGrntT[%d],Dly[%d],Ql[%d],Tkn[%d]",
                            gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcId].STX[count%MAX_TTI_STATS].expectedGrantT,
                            gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcId].STX[count%MAX_TTI_STATS].headOfLinePktDelay,
                            gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcId].STX[count%MAX_TTI_STATS].qLoad,
                            gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcId].STX[count%MAX_TTI_STATS].availLcToken);
                    printf(",LcGrnt[%d],TBSize[%llu],Pad[%d],",
                            gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcId].STX[count%MAX_TTI_STATS].ulLcGrant,
                            gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].qoSUELCStat[ueId].grant[count%MAX_TTI_STATS],
                            gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcId].STX[count%MAX_TTI_STATS].pad);
                printf("Wp[%d]",
                            gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcId].STX[count%MAX_TTI_STATS].ulLcWt);
/*QOS_PKT_DROP_CHANGES*/
                    printf("PktDrp[%llu]\n",gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcId].STX[count%MAX_TTI_STATS].numULMissPerTick);
/*QOS_PKT_DROP_CHANGES*/

            }
        }
    }
    for (ueId = 0; ueId < 1; ueId++)
    {
        for (newCount = 0; newCount< MAX_TTI_STATS ; newCount++)
        {
                ul_thruput_tick += gMacStats.pStats->qoSUEAndPerLCTTIStat[cellIndex].qoSUELCStat[ueId].grant[newCount % MAX_TTI_STATS ];
        }
    }
    }
    /* CA Stats Changes End */
    printf("\n UL Total Grant for last 1000 Ticks : %llu ",ul_thruput_tick);
    //printf("\n UL Throughput in last 1000 Ticks is  :::  %lld ",(ul_thruput_tick * 8));
    thruput = (float)(ul_thruput_tick * 8)/(1024*1024);
    printf("\n UL Throughput is :::  %0.2f ",thruput);
#ifdef UTFWK_SIMULATION
    printf("\n Total UL Pkts in System : %u ",gMacStats.pStats->qoSUEAndPerLCTTIStat.numTotULPktsInSys);
    printf("\n Total UL Pkts Dropped in System : %u ",gMacStats.pStats->qoSUEAndPerLCTTIStat.numULMissInSys);
#endif
}
#endif /* SSI_DEBUG_STATS */

#ifdef UTFWK_SIMULATION
#ifdef KPI_STATS
/* +- SPR 18490 */
void printPDCPKPIStats(void)
{
    UInt8 qciIndex=0;
    UInt64 dlDiscardRate=0;
    UInt64 uuLossRate=0;
    UInt64 ulDrbLossRate=0;
    UInt8 kpiBitmap = 0;
    UInt8 kpiBit = 0;

    printf("################PDCP KPI STATS######################## \n");

    printf("Duration for Kpi Stats [%u]seconds ",gMacStats.pStats->lteCommonKPIStats.ltePdcpKPIConfig.durationTTI/1000);

    printf("\n");

    kpiBitmap = gMacStats.pStats->lteCommonKPIStats.ltePdcpKPIConfig.kpiBitmap;
    for (kpiBit =0 ; kpiBit < 3; kpiBit++)
    {
        if (kpiBitmap & (1<<kpiBit))
        {
            switch (kpiBit)
            {
                case KPI_TOTAL_DL_DRB_DISCARD_RATE:
                   for (qciIndex = 0; qciIndex < MAX_QCI; qciIndex++)
                   {
                     if(gMacStats.pStats->lteCommonKPIStats.ltePdcpKPIStats.totalDlDrbSduRcvd[qciIndex] != 0)
                     { 
                        dlDiscardRate = 
/* SPR 4497 Changes Start */
                           ((UInt64)(gMacStats.pStats->lteCommonKPIStats.ltePdcpKPIStats.totalDlDrbDiscardRatePerQci[qciIndex]

                            /* SPR 3947 Changes Start */
                            
                            )
                            /* SPR 3947 Changes Stop */
/* SPR 4497 Changes End */
                             *1000000)/
                             (gMacStats.pStats->lteCommonKPIStats.ltePdcpKPIStats.totalDlDrbSduRcvd[qciIndex]);
                         /* spr 3643 Changes start*/
                   	 printf("Total DL DRB Discard Rate per QCI [%d] [%lld]\n",qciIndex+1, dlDiscardRate);
                         /* spr 3643 Changes end*/
                     }
                     printf("\n");
                   }
                   printf("=======================================================================================");
                break;

                case KPI_TOTAL_DL_DRB_UU_LOSS_RATE:
                   for (qciIndex = 0; qciIndex < MAX_QCI; qciIndex++)
                   { 
		     if(gMacStats.pStats->lteCommonKPIStats.ltePdcpKPIStats.totalDlDrbSduRcvd[qciIndex] != 0)
                     	{
                           uuLossRate=
                             ((UInt64)(gMacStats.pStats->lteCommonKPIStats.ltePdcpKPIStats.totalDlDrbUuLossRatePerQci[qciIndex])
			     *1000000)/
                              (gMacStats.pStats->lteCommonKPIStats.ltePdcpKPIStats.totalDlDrbSduRcvd[qciIndex]);
    /* spr 3643 Changes start*/
                    	   printf("Total DL DRB Uu Loss Rate per QCI [%d] [%lld]\n",qciIndex+1, uuLossRate);
    /* spr 3643 Changes end*/
                        }
                     printf("\n");
                   }
                   printf("=======================================================================================");
                break;

                case KPI_TOTAL_UL_DRB_LOSS_RATE:
                   for (qciIndex = 0; qciIndex < MAX_QCI; qciIndex++)
                   { 
		     if(gMacStats.pStats->lteCommonKPIStats.ltePdcpKPIStats.totalUlDrbSduRcvd[qciIndex] != 0)
                     {
                         ulDrbLossRate=
                           ((UInt64)(gMacStats.pStats->lteCommonKPIStats.ltePdcpKPIStats.totalUlLossRatePerQci[qciIndex])
			   *1000000)/
                            (gMacStats.pStats->lteCommonKPIStats.ltePdcpKPIStats.totalUlDrbSduRcvd[qciIndex]);
    /* spr 3643 Changes start*/
                         printf("Total UL Packet Loss Rate per QCI [%d] [%lld]\n",qciIndex+1, ulDrbLossRate);
    /* spr 3643 Changes end*/
                     }
                     printf("\n");
                   }
                       
                   printf("=======================================================================================");
            }
        } 
    }    

}
#endif
#endif

#ifdef PERF_STATS
/* +- SPR 17777 */
void printPerfStats(void)
/* +- SPR 17777 */
{
    InternalCellIndex cellIndex = 0;
	UInt8 perfStatsBitmap = 0;
    UInt8 perfStatsBit = 0;
    LteUeMacULPerfStats  *lteUeMacULPerfStats_p =&gMacUePerfStats_p->lteUeMacULPerfStats;
    LteUeMacDLPerfStats  *lteUeMacDLPerfStats_p =&gMacUePerfStats_p->lteUeMacDLPerfStats;
    LteCellMacDLPerfStats  *lteCellMacDLPerfStats_p = PNULL; 
    LteCellMacULPerfStats  *lteCellMacULPerfStats_p = PNULL;  
    LteUePdcpDLPerfStats  *lteUePdcpDLPerfStats_p =&gPdcpUePerfStats_p->lteUePdcpDLPerfStats;
    LteUePdcpULPerfStats  *lteUePdcpULPerfStats_p =&gPdcpUePerfStats_p->lteUePdcpULPerfStats;
    LteCellPdcpDLPerfStats  *lteCellPdcpDLPerfStats_p = PNULL;
    LteCellPdcpULPerfStats  *lteCellPdcpULPerfStats_p = PNULL;
    LteUeRlcDLPerfStats  *lteUeRlcDLPerfStats_p =&gRlcUePerfStats_p->lteUeRlcDLPerfStats;
    LteUeRlcULPerfStats *lteUeRlcULPerfStats_p =&gRlcUePerfStats_p->lteUeRlcULPerfStats;

    LteUePdcpRadioBearerInfoStats *lteUePdcpRadioBearerInfoStats_p = &(gPdcpUePerfStats_p->lteUePdcpRadioBearerInfoStats[0]);
    LteCellMacSchedULPerfStats *lteCellMacSchedULPerfStats_p= PNULL;
    LteCellMacSchedDLPerfStats *lteCellMacSchedDLPerfStats_p= PNULL;
    LteUeMacSchedULPerfStats *lteUeMacSchedULPerfStats_p=&gMacUePerfStats_p->lteUeMacSchedULPerfStats;
    LteUeMacSchedDLPerfStats *lteUeMacSchedDLPerfStats_p=&gMacUePerfStats_p->lteUeMacSchedDLPerfStats;

   /*++coverity-56630*/ 
    int ueIndex = 0;
    int lcIndex=0;
   /*--coverity-56630*/ 
    printf("################--MAC PERF STATS--######################## \n");
    perfStatsBitmap = g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_MAC].
		perfConfig.lteMacPerfStatsConfig.uePerfStatsToReportBitMap;
    for (perfStatsBit =0 ; perfStatsBit < 15; perfStatsBit++)
    {
        if (perfStatsBitmap & (1<<perfStatsBit))
        {
            switch (perfStatsBit)
            {
                /*++coverity-56630 - code removed*/
                ueIndex = 0;
                case 0:
                printf("################--CQI STATS--######################## \n");
                for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
                {
                    lteUeMacULPerfStats_p =&gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats;
                    if (lteUeMacULPerfStats_p)
                    {
                        printf("Total CQI Reported for UE Index[%u] per CQI 0  [%u]\n",ueIndex, 
                                                                                       (lteUeMacULPerfStats_p)->totalCQICW1Reported[0]);
                        printf("Total CQI Reported for UE Index[%u] per CQI 1  [%u]\n",ueIndex, 
                                                                                       (lteUeMacULPerfStats_p)->totalCQICW1Reported[1]);
                        printf("Total CQI Reported for UE Index[%u] per CQI 2  [%u]\n",ueIndex, 
                                                                                       (lteUeMacULPerfStats_p)->totalCQICW1Reported[2]);
                        printf("Total CQI Reported for UE Index[%u] per CQI 3  [%u]\n",ueIndex, 
                                                                                       (lteUeMacULPerfStats_p)->totalCQICW1Reported[3]);
                        printf("Total CQI Reported for UE Index[%u] per CQI 4  [%u]\n",ueIndex, 
                                                                                       (lteUeMacULPerfStats_p)->totalCQICW1Reported[4]);
                        printf("Total CQI Reported for UE Index[%u] per CQI 5  [%u]\n",ueIndex, 
                                                                                       (lteUeMacULPerfStats_p)->totalCQICW1Reported[5]);
                        printf("Total CQI Reported for UE Index[%u] per CQI 6  [%u]\n",ueIndex, 
                                                                                       (lteUeMacULPerfStats_p)->totalCQICW1Reported[6]);
                        printf("Total CQI Reported for UE Index[%u] per CQI 7  [%u]\n",ueIndex, 
                                                                                       (lteUeMacULPerfStats_p)->totalCQICW1Reported[7]);
                        printf("Total CQI Reported for UE Index[%u] per CQI 8  [%u]\n",ueIndex, 
                                                                                       (lteUeMacULPerfStats_p)->totalCQICW1Reported[8]);
                        printf("Total CQI Reported for UE Index[%u] per CQI 9  [%u]\n",ueIndex, 
                                                                                       (lteUeMacULPerfStats_p)->totalCQICW1Reported[9]);
                        printf("Total CQI Reported for UE Index[%u] per CQI 10 [%u]\n",ueIndex, 
                                                                                        (lteUeMacULPerfStats_p)->totalCQICW1Reported[10]);
                        printf("Total CQI Reported for UE Index[%u] per CQI 11 [%u]\n",ueIndex, 
                                                                                        (lteUeMacULPerfStats_p)->totalCQICW1Reported[11]);
                        printf("Total CQI Reported for UE Index[%u] per CQI 12 [%u]\n",ueIndex, 
                                                                                        (lteUeMacULPerfStats_p)->totalCQICW1Reported[12]);
                        printf("Total CQI Reported for UE Index[%u] per CQI 13 [%u]\n",ueIndex, 
                                                                                        (lteUeMacULPerfStats_p)->totalCQICW1Reported[13]);
                        printf("Total CQI Reported for UE Index[%u] per CQI 14 [%u]\n",ueIndex, 
                                                                                        (lteUeMacULPerfStats_p)->totalCQICW1Reported[14]);
                        printf("Total CQI Reported for UE Index[%u] per CQI 15 [%u]\n",ueIndex, 
                                                                                        (lteUeMacULPerfStats_p)->totalCQICW1Reported[15]);
                        printf("\n");
                    }
        }
                for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
                {
  
                    lteUeMacULPerfStats_p =&gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats;
                    if (lteUeMacULPerfStats_p)
                    {
                        printf("Total CQI2 Reported for UE Index[%u] per CQI 0  [%u]\n",ueIndex, 
                                                                               (lteUeMacULPerfStats_p)->totalCQICW2Reported[0]);
                        printf("Total CQI2 Reported for UE Index[%u] per CQI 1  [%u]\n",ueIndex, 
                                                                               (lteUeMacULPerfStats_p)->totalCQICW2Reported[1]);
                        printf("Total CQI2 Reported for UE Index[%u] per CQI 2  [%u]\n",ueIndex, 
                                                                               (lteUeMacULPerfStats_p)->totalCQICW2Reported[2]);
                        printf("Total CQI2 Reported for UE Index[%u] per CQI 3  [%u]\n",ueIndex, 
                                                                               (lteUeMacULPerfStats_p)->totalCQICW2Reported[3]);
                        printf("Total CQI2 Reported for UE Index[%u] per CQI 4  [%u]\n",ueIndex, 
                                                                               (lteUeMacULPerfStats_p)->totalCQICW2Reported[4]);
                        printf("Total CQI2 Reported for UE Index[%u] per CQI 5  [%u]\n",ueIndex, 
                                                                               (lteUeMacULPerfStats_p)->totalCQICW2Reported[5]);
                        printf("Total CQI2 Reported for UE Index[%u] per CQI 6  [%u]\n",ueIndex, 
                                                                               (lteUeMacULPerfStats_p)->totalCQICW2Reported[6]);
                        printf("Total CQI2 Reported for UE Index[%u] per CQI 7  [%u]\n",ueIndex, 
                                                                               (lteUeMacULPerfStats_p)->totalCQICW2Reported[7]);
                        printf("Total CQI2 Reported for UE Index[%u] per CQI 8  [%u]\n",ueIndex, 
                                                                               (lteUeMacULPerfStats_p)->totalCQICW2Reported[8]);
                        printf("Total CQI2 Reported for UE Index[%u] per CQI 9  [%u]\n",ueIndex, 
                                                                               (lteUeMacULPerfStats_p)->totalCQICW2Reported[9]);
                        printf("Total CQI2 Reported for UE Index[%u] per CQI 10 [%u]\n",ueIndex, 
                                                                               (lteUeMacULPerfStats_p)->totalCQICW2Reported[10]);
                        printf("Total CQI2 Reported for UE Index[%u] per CQI 11 [%u]\n",ueIndex, 
                                                                               (lteUeMacULPerfStats_p)->totalCQICW2Reported[11]);
                        printf("Total CQI2 Reported for UE Index[%u] per CQI 12 [%u]\n",ueIndex, 
                                                                               (lteUeMacULPerfStats_p)->totalCQICW2Reported[12]);
                        printf("Total CQI2 Reported for UE Index[%u] per CQI 13 [%u]\n",ueIndex, 
                                                                               (lteUeMacULPerfStats_p)->totalCQICW2Reported[13]);
                        printf("Total CQI2 Reported for UE Index[%u] per CQI 14 [%u]\n",ueIndex, 
                                                                               (lteUeMacULPerfStats_p)->totalCQICW2Reported[14]);
                        printf("Total CQI2 Reported for UE Index[%u] per CQI 15 [%u]\n",ueIndex, 
                                                                               (lteUeMacULPerfStats_p)->totalCQICW2Reported[15]);
                        printf("\n");
                    }
                }
                for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
                {
                    lteUeMacULPerfStats_p =&gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats;
                    if (lteUeMacULPerfStats_p)
                    {
                        printf("Total Rank Reported for UE Index[%u] per Rank 0 [%u]\n",ueIndex, 
                                                                              (lteUeMacULPerfStats_p)->totalRankReported[0]);
                        printf("Total Rank Reported for UE Index[%u] per Rank 1 [%u]\n",ueIndex, 
                                                                              (lteUeMacULPerfStats_p)->totalRankReported[1]);
                        printf("\n");
                    }
                }
                break;

		case 1:
                printf("################--HARQ STATS--######################## \n");
                for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
                {
                    lteUeMacDLPerfStats_p =&gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats;
                    if (lteUeMacDLPerfStats_p )
                    {
                        printf("Total DL HARQ ACK for UE Index[%u] per modScheme 0 [%u]\n",ueIndex, 
                                                                                 (lteUeMacDLPerfStats_p)->totalDLHarqAck[0]);
                        printf("Total DL HARQ ACK for UE Index[%u] per modScheme 1 [%u]\n",ueIndex, 
                                                                                 (lteUeMacDLPerfStats_p)->totalDLHarqAck[1]);
                        printf("Total DL HARQ ACK for UE Index[%u] per modScheme 2 [%u]\n",ueIndex, 
                                                                                 (lteUeMacDLPerfStats_p)->totalDLHarqAck[2]);
                        printf("\n");
                    }
                }

                for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
                {

                    lteUeMacDLPerfStats_p =&gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats;
                    if (lteUeMacDLPerfStats_p )
                    {
                        printf("Total DL HARQ NACK for UE Index[%u] per modScheme 0 [%u]\n",ueIndex, 
                                                                                  (lteUeMacDLPerfStats_p)->totalDLHarqNack[0]);
                        printf("Total DL HARQ NACK for UE Index[%u] per modScheme 1 [%u]\n",ueIndex, 
                                                                                  (lteUeMacDLPerfStats_p)->totalDLHarqNack[1]);
                        printf("Total DL HARQ NACK for UE Index[%u] per modScheme 2 [%u]\n",ueIndex, 
                                                                                  (lteUeMacDLPerfStats_p)->totalDLHarqNack[2]);
                        printf("\n");
                    }
		}

		for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
		{
                    lteUeMacULPerfStats_p =&gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats;
                    if (lteUeMacULPerfStats_p)
                    {
                        printf("Total UL HARQ SUCCESS for UE Index[%u] per modScheme 0 [%u]\n",ueIndex, 
                                                                                     (lteUeMacULPerfStats_p)->totalULHarqSucc[0]);
                        printf("Total UL HARQ SUCCESS for UE Index[%u] per modScheme 1 [%u]\n",ueIndex, 
                                                                                     (lteUeMacULPerfStats_p)->totalULHarqSucc[1]);
                        printf("Total UL HARQ SUCCESS for UE Index[%u] per modScheme 2 [%u]\n",ueIndex, 
                                                                                     (lteUeMacULPerfStats_p)->totalULHarqSucc[2]);
                        printf("\n");
                    }
                }
                for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
                {
                    lteUeMacULPerfStats_p =&gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats;
                    if (lteUeMacULPerfStats_p)
                    {
                        printf("Total UL HARQ FAIL for UE Index[%u] per modScheme 0 [%u]\n",ueIndex, 
                                                                                  (lteUeMacULPerfStats_p)->totalULHarqFail[0]);
                        printf("Total UL HARQ FAIL for UE Index[%u] per modScheme 1 [%u]\n",ueIndex, 
                                                                                  (lteUeMacULPerfStats_p)->totalULHarqFail[1]);
                        printf("Total UL HARQ FAIL for UE Index[%u] per modScheme 2 [%u]\n",ueIndex, 
                                                                                  (lteUeMacULPerfStats_p)->totalULHarqFail[2]);
                        printf("\n");
                    }
                }
                for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
                {
                    lteUeMacDLPerfStats_p =&gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats;
                    if (lteUeMacDLPerfStats_p)
		    {
			    printf("Total HARQ FAIL(No More ReTransmission) for UE Index[%u] : [%u]\n",ueIndex, 
					    (lteUeMacDLPerfStats_p)->totalHarqFail);
		    }
		}

		break;

		case 2:
		printf("################--UE CAPABILITY STATS--######################## \n");
		for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
		{

                     lteUeMacSchedDLPerfStats_p=&gMacUePerfStats_p[ueIndex].lteUeMacSchedDLPerfStats;
                    if(lteUeMacSchedDLPerfStats_p)
		    {
			    printf("Total Time Restrict UE in DL by UE Capability for UE Index [%u] : [%u]\n", ueIndex,
					    (lteUeMacSchedDLPerfStats_p)->totalTimeRestrictUeInDlbyUeCap);
		    }
		}
		for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
		{
                        lteUeMacSchedULPerfStats_p=&gMacUePerfStats_p[ueIndex].lteUeMacSchedULPerfStats;
                        if(lteUeMacSchedULPerfStats_p)
			{
				printf("Total Time Restrict UE in UL by UE Capability for UE Index [%u] : [%u]\n", ueIndex,
						(lteUeMacSchedULPerfStats_p)->totalTimeRestrictUeInUlbyUeCap);

			}
		}
		break;

		case 3:
		printf("################--UE POWER LIMITED STATS--######################## \n");
		for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
		{
                        lteUeMacSchedULPerfStats_p=&gMacUePerfStats_p[ueIndex].lteUeMacSchedULPerfStats;
                        if(lteUeMacSchedULPerfStats_p)
			{
				printf("Total TB Scheduled on Power Restriction for UE Index [%u] : [%u]\n", ueIndex,
						(lteUeMacSchedULPerfStats_p)->totalTbScheduledOnPowerRestriction);
				printf("TotalTB Scheduled in UL on No Power Restriction for UE Index [%u] : [%u]\n", ueIndex,
						(lteUeMacSchedULPerfStats_p)->totalTbScheduledInUlOnNoPowerRestriction);
			} 
		}
		break;

		case 4:
		printf("################--UE TRANSMISSION MODE RI STATS--######################## \n");
        	for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
		{
                        lteUeMacDLPerfStats_p =&gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats;
                        if (lteUeMacDLPerfStats_p )
			{
				printf("Total DL Transmission With TX2 RI1 for UE Index [%u]  : [%u]\n", ueIndex,
						(lteUeMacDLPerfStats_p)->totalDLTransmissionsWithTX2RI1);
			}
		}
		for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
		{
                        lteUeMacDLPerfStats_p =&gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats;
                        if (lteUeMacDLPerfStats_p )
			{
				printf("Total DL Transmission With TX3 RI1 for UE Index [%u]  : [%u]\n", ueIndex,(lteUeMacDLPerfStats_p)->totalDLTransmissionsWithTX3RI[0]);
				printf("Total DL Transmission With TX3 RI2 for UE Index [%u]  : [%u]\n", ueIndex,(lteUeMacDLPerfStats_p)->totalDLTransmissionsWithTX3RI[1]);
			}
		}

                for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
                {
                        lteUeMacDLPerfStats_p =&gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats;
                        if (lteUeMacDLPerfStats_p)
                        {
                                printf("Total DL Transmission With TX4 RI1 for UE Index [%u]  : [%u]\n",ueIndex,(lteUeMacDLPerfStats_p)->totalDLTransmissionsWithTX4RI[0]);
                                printf("Total DL Transmission With TX4 RI2 for UE Index [%u]  : [%u]\n", ueIndex,(lteUeMacDLPerfStats_p)->totalDLTransmissionsWithTX4RI[1]);
                        }
                }

		break;

		case 5:
		printf("################--UE PUCCH and PUSCH SINR STATS--######################## \n");
		for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
		{
                         lteUeMacULPerfStats_p =&gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats;
                        if (lteUeMacULPerfStats_p)
			{
			    printf("Total PUCCH SINR Reported for UE Index [%u] for MAX_SINF_DIFF_INDEX 0 : [%u]\n", ueIndex,
						(lteUeMacULPerfStats_p)->totalPUCCHSINRReported[0]);
			    printf("Total PUCCH SINR Reported for UE Index [%u] for MAX_SINF_DIFF_INDEX 1 : [%u]\n", ueIndex,
						(lteUeMacULPerfStats_p)->totalPUCCHSINRReported[1]);
			    printf("Total PUCCH SINR Reported for UE Index [%u] for MAX_SINF_DIFF_INDEX 2 : [%u]\n", ueIndex,
						(lteUeMacULPerfStats_p)->totalPUCCHSINRReported[2]);
			    printf("Total PUCCH SINR Reported for UE Index [%u] for MAX_SINF_DIFF_INDEX 3 : [%u]\n", ueIndex,
						(lteUeMacULPerfStats_p)->totalPUCCHSINRReported[3]);
			    printf("Total PUCCH SINR Reported for UE Index [%u] for MAX_SINF_DIFF_INDEX 4 : [%u]\n", ueIndex,
						(lteUeMacULPerfStats_p)->totalPUCCHSINRReported[4]);
			    printf("Total PUCCH SINR Reported for UE Index [%u] for MAX_SINF_DIFF_INDEX 5 : [%u]\n", ueIndex,
						(lteUeMacULPerfStats_p)->totalPUCCHSINRReported[5]);
			    printf("Total PUCCH SINR Reported for UE Index [%u] for MAX_SINF_DIFF_INDEX 6 : [%u]\n", ueIndex,
						(lteUeMacULPerfStats_p)->totalPUCCHSINRReported[6]);
			    printf("Total PUCCH SINR Reported for UE Index [%u] for MAX_SINF_DIFF_INDEX 7 : [%u]\n", ueIndex,
						(lteUeMacULPerfStats_p)->totalPUCCHSINRReported[7]);
                        }
                  }

		for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
		{

                         lteUeMacULPerfStats_p =&gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats;
                        if (lteUeMacULPerfStats_p)
			{
                             printf("Total PUSCH SINR Reported for UE Index [%u] for MAX_SINF_DIFF_INDEX 0 : [%u]\n", ueIndex,
                                                (lteUeMacULPerfStats_p)->totalPUSCHSINRReported[0]);
                             printf("Total PUSCH SINR Reported for UE Index [%u] for MAX_SINF_DIFF_INDEX 1 : [%u]\n", ueIndex,
                                                (lteUeMacULPerfStats_p)->totalPUSCHSINRReported[1]);
                             printf("Total PUSCH SINR Reported for UE Index [%u] for MAX_SINF_DIFF_INDEX 2 : [%u]\n", ueIndex,
                                                (lteUeMacULPerfStats_p)->totalPUSCHSINRReported[2]);
                             printf("Total PUSCH SINR Reported for UE Index [%u] for MAX_SINF_DIFF_INDEX 3 : [%u]\n", ueIndex,
                                                (lteUeMacULPerfStats_p)->totalPUSCHSINRReported[3]);
                             printf("Total PUSCH SINR Reported for UE Index [%u] for MAX_SINF_DIFF_INDEX 4 : [%u]\n", ueIndex,
                                                (lteUeMacULPerfStats_p)->totalPUSCHSINRReported[4]);
                             printf("Total PUSCH SINR Reported for UE Index [%u] for MAX_SINF_DIFF_INDEX 5 : [%u]\n", ueIndex,
                                                (lteUeMacULPerfStats_p)->totalPUSCHSINRReported[5]);
                             printf("Total PUSCH SINR Reported for UE Index [%u] for MAX_SINF_DIFF_INDEX 6 : [%u]\n", ueIndex,
                                                (lteUeMacULPerfStats_p)->totalPUSCHSINRReported[6]);
                             printf("Total PUSCH SINR Reported for UE Index [%u] for MAX_SINF_DIFF_INDEX 7 : [%u]\n", ueIndex,
                                                (lteUeMacULPerfStats_p)->totalPUSCHSINRReported[7]);
  

			}
		}				
		break;

		case 6:
		printf("################--DATA VOLUME STATS--######################## \n");
		for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
		{
                     lteUeMacULPerfStats_p =&gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats;
                     if (lteUeMacULPerfStats_p)
		     {
			     printf("Total UL Bytes received for UE Index [%u] : [%u]\n", ueIndex, 
					     (lteUeMacULPerfStats_p)->totalUlBytesReceived);
		     }

		}  

		for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
		{
		
                        lteUeMacDLPerfStats_p =&gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats;
                        if (lteUeMacDLPerfStats_p)
			{
				printf("Total DL Bytes received for UE Index [%u] : [%u]\n", ueIndex,
						(lteUeMacDLPerfStats_p)->totalDlBytesSent);
			}

		}
		for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
		{
                       lteUeMacULPerfStats_p =&gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats;
                        if (lteUeMacULPerfStats_p)
			{
				printf("Total UL volume used for UL UE Throughput for UE Index [%u] : [%u]\n", ueIndex,
						(lteUeMacULPerfStats_p)->totalUlThpPerUeTrunk);

			}
		}
		break;

		case 7:
		printf("################--UE PRB USAGE STATS--######################## \n");
		for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
		{
                        lteUeMacDLPerfStats_p =&gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats;
                        if (lteUeMacDLPerfStats_p)
			{
				printf("Total DL RE Usage for UE Index [%u]  : [%u]\n", ueIndex, 
						(lteUeMacDLPerfStats_p)->totalDLREsUsage);
			}
		}

		for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
		{
                        lteUeMacULPerfStats_p =&gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats;
                        if (lteUeMacULPerfStats_p)
			{
				printf("Total UL REs Usage For UE Index [%u] : [%u]\n", ueIndex,
						(lteUeMacULPerfStats_p)->totalREsUsedInUL);
			}
		}
		break;
	    }
	}
    }

    perfStatsBitmap = g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_MAC].
		perfConfig.lteMacPerfStatsConfig.cellPerfStatsToReportBitMap;
    for (perfStatsBit =0 ; perfStatsBit < 15; perfStatsBit++)
    {
        if (perfStatsBitmap & (1<<perfStatsBit))
        {
            switch (perfStatsBit)
            {
                case 0:
				for (cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++ )
				{
    				lteCellMacDLPerfStats_p =&gMacCellPerfStats_p[cellIndex]->lteCellMacDLPerfStats;
    				lteCellMacSchedDLPerfStats_p=&gMacCellPerfStats_p[cellIndex]->lteCellMacSchedDLPerfStats;
                	
					printf("################--DL PRB HARQ USAGE STATS for Cell:[%u]--######################## \n",cellIndex);
                	printf("Total DL PRB Usage for HARQ PerQCI 1 [%u]\n",lteCellMacDLPerfStats_p->totalDLPRBUsageforHarq[0]);
                	printf("Total DL PRB Usage for HARQ PerQCI 2 [%u]\n",lteCellMacDLPerfStats_p->totalDLPRBUsageforHarq[1]);
               	 	printf("Total DL PRB Usage for HARQ PerQCI 3 [%u]\n",lteCellMacDLPerfStats_p->totalDLPRBUsageforHarq[2]);
                	printf("Total DL PRB Usage for HARQ PerQCI 4 [%u]\n",lteCellMacDLPerfStats_p->totalDLPRBUsageforHarq[3]);
                	printf("Total DL PRB Usage for HARQ PerQCI 5 [%u]\n",lteCellMacDLPerfStats_p->totalDLPRBUsageforHarq[4]);
                	printf("Total DL PRB Usage for HARQ PerQCI 6 [%u]\n",lteCellMacDLPerfStats_p->totalDLPRBUsageforHarq[5]);
                	printf("Total DL PRB Usage for HARQ PerQCI 7 [%u]\n",lteCellMacDLPerfStats_p->totalDLPRBUsageforHarq[6]);
                	printf("Total DL PRB Usage for HARQ PerQCI 8 [%u]\n",lteCellMacDLPerfStats_p->totalDLPRBUsageforHarq[7]);
                	printf("Total DL PRB Usage for HARQ PerQCI 9 [%u]\n",lteCellMacDLPerfStats_p->totalDLPRBUsageforHarq[8]);
					printf("Total DL PRB Usage for HARQ Retransmission : [%u]\n", 
						lteCellMacSchedDLPerfStats_p->totalDLPRBUsageforHarqRetx);
				}
		break;

		case 1:
		for (cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++ )
		{
    		lteCellMacULPerfStats_p =&gMacCellPerfStats_p[cellIndex]->lteCellMacULPerfStats; 
		
			printf("################--RACH STATS For Cell: [%u]--######################## \n",cellIndex);
			printf("Total MSG3 CBRA RECVD [%u]\n",lteCellMacULPerfStats_p->totalRcvdMsg3RandomAccessPreamblesCBRA);
			printf("Total MSG3 CFRA RECVD [%u]\n",lteCellMacULPerfStats_p->totalRcvdMsg3RandomAccessPreamblesCFRA);
			printf("Total Failed MSG2 CBRA RECVD [%u]\n",lteCellMacULPerfStats_p->totalFailedRandomAccessPreamblesMsg2CBRA);
			printf("Total Failed MSG2 CFRA RECVD [%u]\n",lteCellMacULPerfStats_p->totalFailedRandomAccessPreamblesMsg2CFRA);
			printf("Total Allocated preambles [%u]\n",lteCellMacULPerfStats_p->totalAllocatedRandomAccessPreambles);

			printf("Total Successful Random Access TA for MAX_TA_RANGE 1 : [%u]\n",
							lteCellMacULPerfStats_p->totalSuccRandomAccessTA[0]);
			printf("Total Successful Random Access TA for MAX_TA_RANGE 2 : [%u]\n",
							lteCellMacULPerfStats_p->totalSuccRandomAccessTA[1]);
			printf("Total Successful Random Access TA for MAX_TA_RANGE 3 : [%u]\n",
							lteCellMacULPerfStats_p->totalSuccRandomAccessTA[2]);
			printf("Total Successful Random Access TA for MAX_TA_RANGE 4 : [%u]\n",
							lteCellMacULPerfStats_p->totalSuccRandomAccessTA[3]);
			printf("Total Successful Random Access TA for MAX_TA_RANGE 5 : [%u]\n",
							lteCellMacULPerfStats_p->totalSuccRandomAccessTA[4]);
			printf("Total Successful Random Access TA for MAX_TA_RANGE 6 : [%u]\n",
							lteCellMacULPerfStats_p->totalSuccRandomAccessTA[5]);
			printf("Total Successful Random Access TA for MAX_TA_RANGE 7 : [%u]\n",
							lteCellMacULPerfStats_p->totalSuccRandomAccessTA[6]);
			printf("Total Successful Random Access TA for MAX_TA_RANGE 8 : [%u]\n",
							lteCellMacULPerfStats_p->totalSuccRandomAccessTA[7]);
			printf("Total Successful Random Access TA for MAX_TA_RANGE 9 : [%u]\n",
							lteCellMacULPerfStats_p->totalSuccRandomAccessTA[8]);
			printf("Total Successful Random Access TA for MAX_TA_RANGE 10: [%u]\n",
							lteCellMacULPerfStats_p->totalSuccRandomAccessTA[9]);
			printf("Total Successful Random Access TA for MAX_TA_RANGE 11: [%u]\n",
							lteCellMacULPerfStats_p->totalSuccRandomAccessTA[10]);
			printf("Total Successful Random Access TA for MAX_TA_RANGE 12: [%u]\n",
							lteCellMacULPerfStats_p->totalSuccRandomAccessTA[11]);
			printf("\nTotal number of CFRA Symbols that are not allocated to any UE : [%u]\n", 
							lteCellMacULPerfStats_p->totalUnassignedFalseCFRA);
			printf("Total number of unassigned CFRA preambles : [%u]\n",
							lteCellMacULPerfStats_p->totalUnassignedSumCFRA);
			printf("\n");
		}
		break;

		case 2:
		for (cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++ )
		{
    		lteCellMacDLPerfStats_p =&gMacCellPerfStats_p[cellIndex]->lteCellMacDLPerfStats;
    		lteCellMacSchedDLPerfStats_p=&gMacCellPerfStats_p[cellIndex]->lteCellMacSchedDLPerfStats;
		
			printf("################--PDCCH UTILIZATION STATS For Cell: [%u]--######################## \n",cellIndex);
			printf("No Of Subframe with PDCCH CFI for cfi 1 : [%u]\n",
			lteCellMacDLPerfStats_p->noOfSubFrameWithPdcchCfi[0]);
			printf("No Of Subframe with PDCCH CFI for cfi 2 : [%u]\n", 
				lteCellMacDLPerfStats_p->noOfSubFrameWithPdcchCfi[1]);
			printf("No Of Subframe with PDCCH CFI for cfi 3 : [%u]\n",
				lteCellMacDLPerfStats_p->noOfSubFrameWithPdcchCfi[2]);


			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 0 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[0]);
			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 1 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[1]);
			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 2 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[2]);
			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 3 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[3]);
			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 4 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[4]);
			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 5 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[5]);
			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 6 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[6]);
			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 7 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[7]);
			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 8 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[8]);
			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 9 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[9]);
			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 10 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[10]);
			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 11 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[11]);
			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 12 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[12]);
			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 13 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[13]);
			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 14 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[14]);
			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 15 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[15]);
			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 16 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[16]);
			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 17 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[17]);
			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 18 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[18]);
			printf("Total PDCCH CCE Util for  MAX_UTIL_INDEX 19 : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalPdcchCceUtil[19]);

			printf("Total CCE Available in a cell : [%u]\n", lteCellMacSchedDLPerfStats_p->totalCceAvail);
		}
		break;

		case 3:	
		for (cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++ )
		{
    	    lteCellMacSchedULPerfStats_p=&gMacCellPerfStats_p[cellIndex]->lteCellMacSchedULPerfStats;
		
			printf("################--PUCCH UTILIZATION STATS for Cell :[%u]--######################## \n",cellIndex);
			printf("Total PUCCH Resource Allocation Attempts : [%u]\n",
					lteCellMacSchedULPerfStats_p->totalPUCCHResAllocationAttempts);
		}
		break;

		case 4:
		for (cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++ )
		{
    		lteCellMacULPerfStats_p =&gMacCellPerfStats_p[cellIndex]->lteCellMacULPerfStats; 
    		lteCellMacDLPerfStats_p =&gMacCellPerfStats_p[cellIndex]->lteCellMacDLPerfStats;
    		lteCellMacSchedDLPerfStats_p=&gMacCellPerfStats_p[cellIndex]->lteCellMacSchedDLPerfStats;
    	    lteCellMacSchedULPerfStats_p=&gMacCellPerfStats_p[cellIndex]->lteCellMacSchedULPerfStats;
			
			printf("################--CELL PRB USAGE STATS--######################## \n");
			printf("Total UL PRB Usage SRB for MAX_SRB 0 : [%u] \n", 
				lteCellMacULPerfStats_p->totalULPRBUsageSRB[0]);
			printf("Total UL PRB Usage SRB for and MAX_SRB 1 : [%u] \n",
				lteCellMacULPerfStats_p->totalULPRBUsageSRB[1]);
			printf("Total UL PRB Usage SRB and MAX_SRB 2 : [%u] \n",
				lteCellMacULPerfStats_p->totalULPRBUsageSRB[2]);

			printf("Total DL PRB Usage SRB and MAX_SRB 0 : [%u] \n",
				lteCellMacDLPerfStats_p->totalDLPRBUsageSRB[0]);
			printf("Total DL PRB Usage SRB and MAX_SRB 1 : [%u] \n",
				lteCellMacDLPerfStats_p->totalDLPRBUsageSRB[1]);
			printf("Total DL PRB Usage SRB and MAX_SRB 2 : [%u] \n",
				lteCellMacDLPerfStats_p->totalDLPRBUsageSRB[2]);

			printf("Total DL PRB Usage BCCH  : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalDLPRBUsageBCCH);
			printf("Total DL PRB Usage PCCH  : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalDLPRBUsagePCCH);
			printf("Total DL PRB Available  : [%u]\n",
				lteCellMacSchedDLPerfStats_p->totalDLPRBAvailable);
			printf("Total UL PRB Available  : [%u]\n",
				lteCellMacSchedULPerfStats_p->totalULPRBAvailable);
		}
	    }
	}
    }

    printf("################--PDCP PERF STATS--######################## \n");
    perfStatsBitmap = g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].
			perfConfig.ltePdcpPerfStatsConfig.uePerfStatsToReportBitMap;
    for (perfStatsBit =0 ; perfStatsBit < 15; perfStatsBit++)
    {
        if (perfStatsBitmap & (1<<perfStatsBit))
        {
            switch (perfStatsBit)
            {
                ueIndex=0;
                lcIndex=0;
                /*++coverity-56630 -code removed*/ 
                case 0:
                printf("################--PDCP RB STATS--################# \n");
                for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
                {
                    lteUePdcpDLPerfStats_p =&gPdcpUePerfStats_p[ueIndex].lteUePdcpDLPerfStats;
                    lteUePdcpULPerfStats_p =&gPdcpUePerfStats_p[ueIndex].lteUePdcpULPerfStats;
                    if ((lteUePdcpDLPerfStats_p) || (lteUePdcpULPerfStats_p))
                    {
                        printf("Total DL SRB Bytes Sent  for UE Index[%u] : [%u]\n", ueIndex, 
                                                                         (lteUePdcpDLPerfStats_p)->totalDlSrbBytesSent);
                        /*coverity 56629*/
                        printf("Total UL SRB Bytes Sent for UE Index[%u] : [%u]\n", ueIndex, 
                                                                         (lteUePdcpULPerfStats_p)->totalUlSrbBytesRcvd);
                        /*coverity 56629*/
                        printf("Total DL DRB Bytes Sent for UE Index[%u] per LCID 3 [%u]\n",ueIndex, 
                                                                         (lteUePdcpDLPerfStats_p)->totalDlDrbBytesSent[0]);
                        printf("Total DL DRB Bytes Sent for UE Index[%u] per LCID 4 [%u]\n",ueIndex, 
                                                                         (lteUePdcpDLPerfStats_p)->totalDlDrbBytesSent[1]);
                        printf("Total DL DRB Bytes Sent for UE Index[%u] per LCID 5 [%u]\n",ueIndex, 
                                                                         (lteUePdcpDLPerfStats_p)->totalDlDrbBytesSent[2]);
                        printf("Total DL DRB Bytes Sent for UE Index[%u] per LCID 6 [%u]\n",ueIndex, 
                                                                         (lteUePdcpDLPerfStats_p)->totalDlDrbBytesSent[3]);
                        printf("Total UL DRB Bytes Sent to Upper Layer for UE Index[%u] per LCID 3 [%u]\n",ueIndex, 
                                                                         (lteUePdcpULPerfStats_p)->totalUlDrbBytesSent[0]);
                        printf("Total UL DRB Bytes Sent to Upper Layer for UE Index[%u] per LCID 4 [%u]\n",ueIndex, 
                                                                         (lteUePdcpULPerfStats_p)->totalUlDrbBytesSent[1]);
                        printf("Total UL DRB Bytes Sent to Upper Layer for UE Index[%u] per LCID 5 [%u]\n",ueIndex, 
                                                                         (lteUePdcpULPerfStats_p)->totalUlDrbBytesSent[2]);
                        printf("Total UL DRB Bytes Sent to Upper Layer for UE Index[%u] per LCID 6 [%u]\n",ueIndex, 
                                                                         (lteUePdcpULPerfStats_p)->totalUlDrbBytesSent[3]);
                        printf("Total UL DRB SDU  Sent to Upper Layer for UE Index[%u] per LCID 3 [%u]\n",ueIndex, 
                                                                         (lteUePdcpULPerfStats_p)->numberOfUlDrbSduReceived[0]);
                        printf("Total UL DRB SDU RECVD for UE Index[%u] per LCID 4 [%u]\n",ueIndex, 
                                                                         (lteUePdcpULPerfStats_p)->numberOfUlDrbSduReceived[1]);
                        printf("Total UL DRB SDU RECVD for UE Index[%u] per LCID 5 [%u]\n",ueIndex, 
                                                                         (lteUePdcpULPerfStats_p)->numberOfUlDrbSduReceived[2]);
                        printf("Total UL DRB SDU RECVD for UE Index[%u] per LCID 6 [%u]\n",ueIndex, 
                                                                         (lteUePdcpULPerfStats_p)->numberOfUlDrbSduReceived[3]);
                        printf("Total DL DRB SDU RECVD from TG  for UE Index[%u] per LCID 3 [%u]\n",ueIndex, 
                                                                         (lteUePdcpDLPerfStats_p)->numberOfDlDrbSduReceived[0]);
                        printf("Total DL DRB SDU RECVD from TG for UE Index[%u] per LCID 4 [%u]\n",ueIndex, 
                                                                         (lteUePdcpDLPerfStats_p)->numberOfDlDrbSduReceived[1]);
                        printf("Total DL DRB SDU  RECVD from TG for UE Index[%u] per LCID 5 [%u]\n",ueIndex, 
                                                                         (lteUePdcpDLPerfStats_p)->numberOfDlDrbSduReceived[2]);
                        printf("Total DL DRB SDU RECVD from TG for UE Index[%u] per LCID 6 [%u]\n",ueIndex, 
                                                                         (lteUePdcpDLPerfStats_p)->numberOfDlDrbSduReceived[3]);

			printf("Total RB Volume in UL for UE Index [%u] : [%u]\n", ueIndex,
					                               (lteUePdcpULPerfStats_p)->totalUlRbBytesSent);
 			printf("Total RB Volume in DL for UE Index [%u] : [%u]\n", ueIndex,
                                                                       (lteUePdcpDLPerfStats_p)->totalDlRbBytesSent);

			printf("\n");
		    }
                }
		break;

		case 1:
		printf("##############--PDCP-UE RADIO BEARER STATS--################ \n");
                for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED;ueIndex++)
		{
                       
			lteUePdcpRadioBearerInfoStats_p = &(gPdcpUePerfStats_p[ueIndex].lteUePdcpRadioBearerInfoStats[0]);
                        if (lteUePdcpRadioBearerInfoStats_p)
			{
                           printf("UeIndex = %u \n",ueIndex);
				for( lcIndex = 1; lcIndex < MAX_PDCP_LC_ID; lcIndex++ )
				{
					printf("Radio Bearer Stats for LC INDEX [%u] \n", lcIndex);
					printf("Bearer Id : [%u]\n", (lteUePdcpRadioBearerInfoStats_p[lcIndex]).bearerId);
					printf("Bearer Type : [%u] \n", (lteUePdcpRadioBearerInfoStats_p[lcIndex]).bearerType);
					printf("Radio Bearer QCI : [%u]\n", (lteUePdcpRadioBearerInfoStats_p[lcIndex]).qci);
					printf("Rlc Mode : [%u]\n", (lteUePdcpRadioBearerInfoStats_p[lcIndex]).rlcMode);
					printf("\n");
				}
			}
		}
		break;
		case 2:
		printf("################--PDCP_UE_VOLUME_STATS--############### \n");
		for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
                {
                    lteUePdcpDLPerfStats_p =&gPdcpUePerfStats_p[ueIndex].lteUePdcpDLPerfStats;
                    if(lteUePdcpDLPerfStats_p)
 		    {
			printf("Total DL SRB Bytes Sent And Acknowledged for UE Index [%u] : [%u]\n", ueIndex,
										(lteUePdcpDLPerfStats_p)->totalDLSrbBytesSentAndAck);
            printf("Total DL DRB SDU Discarded due to HO for UE Index [%u] per LCID 3 [%u]\n", ueIndex,
                    (lteUePdcpDLPerfStats_p)->totalDlDrbSduDiscardedHO[0]);
            printf("Total DL DRB SDU Discarded due to HO for UE Index [%u] per LCID 4 [%u]\n", ueIndex,
                    (lteUePdcpDLPerfStats_p)->totalDlDrbSduDiscardedHO[1]);
            printf("Total DL DRB SDU Discarded due to HO for UE Index [%u] per LCID 5 [%u]\n", ueIndex,
                    (lteUePdcpDLPerfStats_p)->totalDlDrbSduDiscardedHO[2]);

            printf("Total DL transmitted PDCP volume for the last piece of data in large data bursts for UE Index[%u] per LCID 3 [%u]\n",ueIndex, (lteUePdcpDLPerfStats_p)->pdcpLastTtiTransVolDl[0]);
            printf("Total DL transmitted PDCP volume for the last piece of data in large data bursts for UE Index[%u] per LCID 4 [%u]\n",ueIndex, (lteUePdcpDLPerfStats_p)->pdcpLastTtiTransVolDl[1]);
            printf("Total DL transmitted PDCP volume for the last piece of data in large data bursts for UE Index[%u] per LCID 5 [%u]\n",ueIndex, (lteUePdcpDLPerfStats_p)->pdcpLastTtiTransVolDl[2]);
            printf("\n");
            }
                }
				
	    }
	}
    }

    perfStatsBitmap = g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].
			perfConfig.ltePdcpPerfStatsConfig.cellPerfStatsToReportBitMap;
    for (perfStatsBit =0 ; perfStatsBit < 7; perfStatsBit++)
    {
        if (perfStatsBitmap & (1<<perfStatsBit))
        {
            switch (perfStatsBit)
            {
		    case 0:
				for (cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++ )
				{
    				lteCellPdcpDLPerfStats_p =&gPdcpCellPerfStats_p[cellIndex]->lteCellPdcpDLPerfStats;
    				lteCellPdcpULPerfStats_p =&gPdcpCellPerfStats_p[cellIndex]->lteCellPdcpULPerfStats;
					printf("##############--PDCP ROHC STATS--################ for Cell :[%u]\n",cellIndex);
					printf("Total DL ROHC UNCMP HDR VOL per QCI 1  [%u]\n", lteCellPdcpDLPerfStats_p->totalRohcUnCmpHdrVolDL[0]);
		        	printf("Total DL ROHC UNCMP HDR VOL per QCI 2  [%u]\n", lteCellPdcpDLPerfStats_p->totalRohcUnCmpHdrVolDL[1]);
					printf("Total DL ROHC UNCMP HDR VOL per QCI 3  [%u]\n", lteCellPdcpDLPerfStats_p->totalRohcUnCmpHdrVolDL[2]);
					printf("Total DL ROHC UNCMP HDR VOL per QCI 4  [%u]\n", lteCellPdcpDLPerfStats_p->totalRohcUnCmpHdrVolDL[3]);
                        printf("Total DL ROHC UNCMP HDR VOL per QCI 5  [%u]\n", lteCellPdcpDLPerfStats_p->totalRohcUnCmpHdrVolDL[4]);
                        printf("Total DL ROHC UNCMP HDR VOL per QCI 6  [%u]\n", lteCellPdcpDLPerfStats_p->totalRohcUnCmpHdrVolDL[5]);
                        printf("Total DL ROHC UNCMP HDR VOL per QCI 7  [%u]\n", lteCellPdcpDLPerfStats_p->totalRohcUnCmpHdrVolDL[6]);
                        printf("Total DL ROHC UNCMP HDR VOL per QCI 8  [%u]\n", lteCellPdcpDLPerfStats_p->totalRohcUnCmpHdrVolDL[7]);
                        printf("Total DL ROHC UNCMP HDR VOL per QCI 9  [%u]\n", lteCellPdcpDLPerfStats_p->totalRohcUnCmpHdrVolDL[8]);
                        printf("Total DL ROHC CMP HDR VOL per QCI 1  [%u]\n", lteCellPdcpDLPerfStats_p->totalRohcCmpHdrVolDL[0]);
                        printf("Total DL ROHC CMP HDR VOL per QCI 2  [%u]\n", lteCellPdcpDLPerfStats_p->totalRohcCmpHdrVolDL[1]);
                        printf("Total DL ROHC CMP HDR VOL per QCI 3  [%u]\n", lteCellPdcpDLPerfStats_p->totalRohcCmpHdrVolDL[2]);
                        printf("Total DL ROHC CMP HDR VOL per QCI 4  [%u]\n", lteCellPdcpDLPerfStats_p->totalRohcCmpHdrVolDL[3]);
                        printf("Total DL ROHC CMP HDR VOL per QCI 5  [%u]\n", lteCellPdcpDLPerfStats_p->totalRohcCmpHdrVolDL[4]);
                        printf("Total DL ROHC CMP HDR VOL per QCI 6  [%u]\n", lteCellPdcpDLPerfStats_p->totalRohcCmpHdrVolDL[5]);
                        printf("Total DL ROHC CMP HDR VOL per QCI 7  [%u]\n", lteCellPdcpDLPerfStats_p->totalRohcCmpHdrVolDL[6]);
                        printf("Total DL ROHC CMP HDR VOL per QCI 8  [%u]\n", lteCellPdcpDLPerfStats_p->totalRohcCmpHdrVolDL[7]);
                        printf("Total DL ROHC CMP HDR VOL per QCI 9  [%u]\n", lteCellPdcpDLPerfStats_p->totalRohcCmpHdrVolDL[8]);
                        printf("Total UL ROHC UNCMP HDR VOL per QCI 1  [%u]\n", lteCellPdcpULPerfStats_p->totalRohcUnCmpHdrVolUL[0]);
                        printf("Total UL ROHC UNCMP HDR VOL per QCI 2  [%u]\n", lteCellPdcpULPerfStats_p->totalRohcUnCmpHdrVolUL[1]);
                        printf("Total UL ROHC UNCMP HDR VOL per QCI 3  [%u]\n", lteCellPdcpULPerfStats_p->totalRohcUnCmpHdrVolUL[2]);
                        printf("Total UL ROHC UNCMP HDR VOL per QCI 4  [%u]\n", lteCellPdcpULPerfStats_p->totalRohcUnCmpHdrVolUL[3]);
                        printf("Total UL ROHC UNCMP HDR VOL per QCI 5  [%u]\n", lteCellPdcpULPerfStats_p->totalRohcUnCmpHdrVolUL[4]);
                        printf("Total UL ROHC UNCMP HDR VOL per QCI 6  [%u]\n", lteCellPdcpULPerfStats_p->totalRohcUnCmpHdrVolUL[5]);
                        printf("Total UL ROHC UNCMP HDR VOL per QCI 7  [%u]\n", lteCellPdcpULPerfStats_p->totalRohcUnCmpHdrVolUL[6]);
                        printf("Total UL ROHC UNCMP HDR VOL per QCI 8  [%u]\n", lteCellPdcpULPerfStats_p->totalRohcUnCmpHdrVolUL[7]);
                        printf("Total UL ROHC UNCMP HDR VOL per QCI 9  [%u]\n", lteCellPdcpULPerfStats_p->totalRohcUnCmpHdrVolUL[8]);
                        printf("Total UL ROHC CMP HDR VOL per QCI 1  [%u]\n", lteCellPdcpULPerfStats_p->totalRohcCmpHdrVolUL[0]);
                        printf("Total UL ROHC CMP HDR VOL per QCI 2  [%u]\n", lteCellPdcpULPerfStats_p->totalRohcCmpHdrVolUL[1]);
                        printf("Total UL ROHC CMP HDR VOL per QCI 3  [%u]\n", lteCellPdcpULPerfStats_p->totalRohcCmpHdrVolUL[2]);
                        printf("Total UL ROHC CMP HDR VOL per QCI 4  [%u]\n", lteCellPdcpULPerfStats_p->totalRohcCmpHdrVolUL[3]);
                        printf("Total UL ROHC CMP HDR VOL per QCI 5  [%u]\n", lteCellPdcpULPerfStats_p->totalRohcCmpHdrVolUL[4]);
                        printf("Total UL ROHC CMP HDR VOL per QCI 6  [%u]\n", lteCellPdcpULPerfStats_p->totalRohcCmpHdrVolUL[5]);
                        printf("Total UL ROHC CMP HDR VOL per QCI 7  [%u]\n", lteCellPdcpULPerfStats_p->totalRohcCmpHdrVolUL[6]);
                        printf("Total UL ROHC CMP HDR VOL per QCI 8  [%u]\n", lteCellPdcpULPerfStats_p->totalRohcCmpHdrVolUL[7]);
                        printf("Total UL ROHC CMP HDR VOL per QCI 9  [%u]\n", lteCellPdcpULPerfStats_p->totalRohcCmpHdrVolUL[8]);
                        printf("\n");
				}

            }
        }
    }

    printf("################--RLC PERF STATS--######################## \n");
    perfStatsBitmap = g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_RLC].
			perfConfig.lteRlcPerfStatsConfig.uePerfStatsToReportBitMap;
    for (perfStatsBit =0 ; perfStatsBit < 7; perfStatsBit++)
    {
        if (perfStatsBitmap & (1<<perfStatsBit))
        {
            switch (perfStatsBit)
            {
                int ueIndex;
                case 0:
                printf("################--RLC RB STATS--######################## \n");
                for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
                {   lteUeRlcDLPerfStats_p =&gRlcUePerfStats_p[ueIndex].lteUeRlcDLPerfStats;
                    lteUeRlcULPerfStats_p =&gRlcUePerfStats_p[ueIndex].lteUeRlcULPerfStats;
                    if ((lteUeRlcDLPerfStats_p) || (lteUeRlcULPerfStats_p))
                    {
                        printf("Total DL RLC UM SDU Transmitted for UE Index[%u] Per LCID 3 [%u]\n",ueIndex, 
                                                                    (lteUeRlcDLPerfStats_p)->totalDlRLCUMSduTransmitted[0]);
                        printf("Total DL RLC UM SDU Transmitted for UE Index[%u] Per LCID 4 [%u]\n",ueIndex, 
                                                                    (lteUeRlcDLPerfStats_p)->totalDlRLCUMSduTransmitted[1]);
                        printf("Total DL RLC UM SDU Transmitted for UE Index[%u] Per LCID 5 [%u]\n",ueIndex, 
                                                                    (lteUeRlcDLPerfStats_p)->totalDlRLCUMSduTransmitted[2]);
                        printf("Total DL RLC UM SDU Transmitted for UE Index[%u] Per LCID 6  [%u]\n",ueIndex, 
                                                                    (lteUeRlcDLPerfStats_p)->totalDlRLCUMSduTransmitted[3]);
			printf("Total DL RLC PDU's Acknowledged for UE Index [%u] : [%u]\n", ueIndex,
                                           		             (lteUeRlcDLPerfStats_p)->totalDLRLCAcks);

			printf("Total UL RLC PDU's Acknowledged for UE Index [%u] : [%u]\n", ueIndex,
                                                                     (lteUeRlcULPerfStats_p)->totalULRLCAcks);

			printf("Total UL RLC PDU's Nack's for UE Index [%u] : [%u]\n", ueIndex,
                                                                     (lteUeRlcULPerfStats_p)->totalULRLCNacks);

                        printf("\n");
                    }
		}
		break;

		case 1:
		printf("################--ReTx EXCEEDED STATS--######################## \n");
		for (ueIndex = 0;  ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
		{
                         lteUeRlcDLPerfStats_p =&gRlcUePerfStats_p[ueIndex].lteUeRlcDLPerfStats;
			if( lteUeRlcDLPerfStats_p )
			{
				printf("Total Maximum Retransmission Exceeded for UE Index [%u] : [%u]\n", ueIndex,
						(lteUeRlcDLPerfStats_p)->totalMaxRetxExceeded);
			}
		}
		break;
	    }
	}
    }
}
/* - PERF_CA */
#endif

int main(int argc, char *argv[]) 
{
    UInt8 choice = 0;
    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
#ifdef HDFDD_STATS
    UInt32 hdFddCnt = 0;
#endif
#endif
    /*HD FDD Changes End*/
#ifdef CYCLE_COUNT_MAC_RLC_PDCP
    SInt8 iterator = 0;
#endif
/* SPR21807 Changes Start */
#ifdef PDCP_GTPU_INTF
    if (argc < 2){
	fprintf(stderr,"Usage %s <instanceL2_g>\n",argv[0]);
	return 1;
    }
    instanceL2_g = atoi(argv[1]);
#else
    instanceL2_g = 0;
#endif
/* SPR21807 Changes End */
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;
    /* CA Stats Changes End */
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/
    initStats();

    printf("SHM ID: %d\n", gMacStats.shmId);
    printf("Size of shared memory is %d\n",sizeof(LteMacStats));
#ifdef PERF_STATS
    initPerfStats();
    printf("PERF stats SHM ID: %d\n", gPerfStats.shmId);
    /*coverity 56638*/
    printf("Size of shared memory is %d \n",(MAX_PERF_STATS * (sizeof(LtePerfStats) + sizeof(LtePerfStatsControlInfo))));
    /*coverity 56638*/

#endif

/*SPR 21653 changes start*/
/* SPR21807 Changes Start */
#ifdef PDCP_GTPU_INTF
    if(argc >= 3)
    {
        choice = atoi(argv[2]);
    }    
#else
    if(argc >= 2)
    {
        choice = atoi(argv[1]);
    }
#endif
/* SPR21807 Changes End */
/*SPR 21653 changes end*/

    switch(choice)
    {
        case 0:
        /* CA Stats Changes Start */
        for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
        {
            printf("######################### PHY stats CELL [%d] ########################\n", cellIndex);
            printf("Number of messages sent         : %u\n", 
                    gMacStats.pStats->phyStats[cellIndex].txStats.numMsgs);
            printf("Number of bytes sent            : %llu\n",
                    gMacStats.pStats->phyStats[cellIndex].txStats.numBytes);
            printf("Number of messages received     : %u\n",
                    gMacStats.pStats->phyStats[cellIndex].rxStats.numMsgs
#ifdef DL_UL_SPLIT_TDD
                    + gMacStats.pStats->phyStats[cellIndex].rxStats.numUlMsgs
#endif
                  );
            printf("Number of bytes received        : %llu\n",
                    gMacStats.pStats->phyStats[cellIndex].rxStats.numBytes
#ifdef DL_UL_SPLIT_TDD
                    + gMacStats.pStats->phyStats[cellIndex].rxStats.numUlBytes
#endif
                  );
            printf("\n");
        }
        /* CA Stats Changes End */
        printf("######################### RLC stats ########################\n");
        printf("Number of messages sent         : %u\n",
                gMacStats.pStats->rlcStats.txStats.numMsgs);
        printf("Number of bytes sent            : %llu\n",
                gMacStats.pStats->rlcStats.txStats.numBytes);
        printf("Number of messages received     : %u\n",
                gMacStats.pStats->rlcStats.rxStats.numMsgs);
        printf("Number of bytes received        : %llu\n",
                gMacStats.pStats->rlcStats.rxStats.numBytes);
        printf("Number of HARQ failure          : %u\n",
                gMacStats.pStats->rlcStats.numHarqFailures);
        printf("\n");
        printf("###################### Performance stats #####################\n");
        /* CA Stats Changes Start */
        printPerformanceStats();
        /* CA Stats Changes End */
        printf("\n");
#ifdef HD_FDD_CONFIG
#ifdef HDFDD_STATS
        /*HD FDD Changes Start*/
        printf("###################### HD FDD stats ##########################\n");
        printf("Number of HD UE Attached        : %u\n",
                gMacStats.pStats->lteMacHdFddStats.numOfHdFddUE);
        if(gMacStats.pStats->lteMacHdFddStats.numOfHdFddUE > 0)
        {
            for(hdFddCnt = 0; hdFddCnt<MAX_UE_SUPPORTED; hdFddCnt++)
            {
                printf("UE ID[%u] Number of time DL Resource Denied      : %u\n",
                        hdFddCnt,gMacStats.pStats->lteMacHdFddStats.totalNumTimesUEdeniedDL[hdFddCnt]);
                printf("UE ID[%u] Number of time UL Resource Denied      : %u\n",
                        hdFddCnt,gMacStats.pStats->lteMacHdFddStats.totalNumTimesUEdeniedUL[hdFddCnt]);
            }
        }
        printf("\n");
        /*HD FDD Changes End*/
#endif
#endif
/*spr 22474 start*/
    /*CSPL Code removed*/
/*spr 22474 end*/
        printf("###################### Fapi stats #####################\n");
        printFapiStats();
        printf("\n");
#ifdef CYCLE_COUNT_MAC_RLC_PDCP
        printf("####################### Cycle Count stats ############################\n");
        /* CA Stats Changes Start */
        for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
        {
            for(iterator = 0; iterator < NUM_L2_CC_ITERATIONS; iterator++)
            {
                printCycleCountStats(cellIndex, iterator);
            }
        }
        /* CA Stats Changes End */
#endif
        /* + E_CID_5.3 */
        printf("##########################PDCCH ORDER STATS ############################\n");
        printEcidPdcchOrderStats();
        /*  - E_CID_5.3 */    
        printf("\n\n###################### TTI stats #####################\n");
        /* CA Stats Changes Start */
        for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
        {
            printRfStats(cellIndex);
        }
        /* CA Stats Changes End */
        break;
#ifdef SSI_DEBUG_STATS
        case 1:
        printf("######### DL STARTEGY per UE per LC stats ##############\n");
        printfDLStartegyLcCommonInfoStats();
        break;

        case 2:
        printf("######### DL STRATEGY per TTI per LC stats ##############\n");
        printDLStartegyPerTTIPerLCStats();
        break;

        case 3:
        printf("######### DL STARTEGY per UE ##############\n");
        printfDLStartegyUeCommonInfoStats();
        break;

        case 4:
        printf("######### DL STRATEGY per TTI per UE stats ##############\n");
        printDLStartegyPerTTIPerUEStats();
        break;

        case 5:
        printf("######### PDSCH per TTI per UE stats ##############\n");
        printPdschPerTTIPerUEStats();
        break;

        case 6:
        printf("######### PDSCH UE common Info Stats ##############\n");
        printfPdschUeCommonInfoStats();
        break;

        case 7:
        /*UL strategy changes start*/
        printf("######### UL Strategy LC Common Info stats ##############\n");
        printfULStartegyLcCommonInfoStats();
        break;

        case 8:
        printf("######### UL Strategy per TTI per LC stats ##############\n");
        printULStrategyPerTTIPerLCStats();
        break;

        case 9:
        printf("######### UL Strategy UE common Info Stats ##############\n");
        printfULStrategyUeCommonInfoStats();
        break;

        case 10:
        printf("######### UL Strategy per TTI per UE stats ##############\n");
        printULStrategyPerTTIPerUEStats();
        break;
        case 11:
        printf("######### UE Coomon Info stats and counters ##############\n");
        printfUeCommonInfoStats();
        break;
        case 12:
        printf("###################### QOS stats #####################\n");
#ifdef UTFWK_SIMULATION
        printPerformanceStats();
#endif
        printSchConfigStats();
        printQoSUELCStats();
        printQoSULUELCStats();
#ifdef UTFWK_SIMULATION
        printQosStats();
#endif
        break;
#endif
        case 13:
#ifdef RLC_QUEUE_STATS
        printf("\n");
        printf("###################### QUEUE stats #####################\n");
        printQueueStats();
        printf("\n");
#endif
        case 14:
#ifdef ACK_NACK_COUNT_STATS
        printf("\n");
        printf("###################### DEBUG stats #####################\n");
        printDebugStats();
        printf("\n");
#endif
        case 15:
#ifdef SSI_DEBUG_STATS
        printf("###################### PUSCH per TTI per UE stats #####################\n");
        printPuschPerTTIPerUeStats();
        case 16:
        printf("###################### UE Common Info stats #####################\n");
        printUeCommonInfoStats();
#endif
#ifdef KPI_STATS
        printf("###################### Uplink Spectral Efficiency stats #####################\n");
        printULSpectralEfficiency();
        printf("###################### Uplink PRB Usage KPI stats #####################\n");
        printUplinkPRBUsageStats();
#endif
        break;
        case 17:
        /* KPI_COMPILATION_BREAK */
#ifdef UTFWK_SIMULATION
#ifdef KPI_STATS
        printKPIStats();
        printPDCPKPIStats();
#endif
#endif
        /* KPI_COMPILATION_BREAK */
        break;
#ifdef SSI_DEBUG_STATS
        case 18:
        {
            printf("###################### SPS stats #####################\n");
            printSpsOamStats();
            printSpsDlSchInfoPerTtiStats();
            printSpsDlUeSpecificStats();
            printSpsDlStrategyStats();
            printPdschPerTTIPerUEStats();
            printHarqPerUeStats();
            printDebugUeHarqStats();
            break;
        }
        case 19:
        {
            printf("###################### SPS RM stats #####################\n");
            printPdschRMStats();
            break;
        }
        case 20:
        {
            printf("###################### SPS UL  stats #####################\n");
            printSpsUlSchInfoPerTtiStats();
            printSpsUlUeSpecificStats();
            printSpsUlStrategyStats();
            printPuschPerTTIPerUEStats();
            printDebugUlUeHarqStats();
            break;
        }
        case 21:
        {
            printf("###################### UL SPS RM stats #####################\n");
            printPuschRMStats();
            break;
        }
#endif
        case 22:
        {
#ifdef PERF_STATS
            printPerfStats();
#endif
            /*++coverity 56625*/
            break;
            /*--coverity 56625*/
        }
        default:
        printf("wrong input enter(0-21)\n");            
    }
    /*--coverity 56627*/
    return 0;
}
/*  + E_CID_5.3 */
/*++coverity 47825*/
/* +- SPR 17777 */
static void printEcidPdcchOrderStats(void)
/* +- SPR 17777 */
/*--coverity 47825*/
{
    /* +- SPR 18268 */
    UInt16 ueIdx;
    /* +- SPR 18268 */
    printf("Num Of Pdcch Orders due to Ecid Meas request\n");
    for(ueIdx = 0; ueIdx < MAX_UE_SUPPORTED; ueIdx++)
    {
        if(gMacStats.pStats->lteMacEcidMeasStats[ueIdx].numOfPdcchOrders)
        {
            printf("UE INDEX:%d\tPDCCH ORDERS:%d\n",ueIdx,\
                    gMacStats.pStats->lteMacEcidMeasStats[ueIdx].numOfPdcchOrders);
        }
    }
}
/*  - E_CID_5.3 */
