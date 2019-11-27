

/*******************************************************************************
 * File Name   : oam_qcli_cmd_tree.c
 *
 * Description : This file contains QCLI command action functions and parameters
 *               if any.
 *
 * Revision History:
 *
 * Date             Author             Reference             Comments
 * -----            ------             ---------             --------
 * May,2012         Vivek K Dixit      OAM Design Document   Initial Version
 * September,2012   Rajesh Kumar                             param_mapping,mapping cache 
 *                                                           structures added
 * October,2012     Harishankar Singh                        structures and tables added
 * November,2012    Vishwa Pandey                            Changes according to review comment. 
 * September,2013   Vinod Vyas                               Changes Related To IPV6
 * December 14,2013 Ekta Jain                                Changes to abort CLI in 
 * April,2014       Ankita Jain                              Cell Traffic Trace                                                               
 *                                                           case memory allocation occurs 
 * April 2014       Vinod Vyas         CSR 57877 Fix                                                          
 * April, 2014      Ankita Jain        CSR 00058157 Fix 
 * June,2014        Shalu Sagar         SPR 11467 Fix
 * June, 2014       Ashish Sharma      SPR 11249 FIX 
 * June 2014        Shalu SAgar         SPR 11266 FIX
 * June 2014        Shalu Sagar         COVERITY FIX
 * June 2014        Aditi Singh         SPR 11519 FIX
 * May, 2014        Ankita Jain        SPR 8158 Fix
 * June, 2014       Ankita Jain        SPR 11590 Fix
 * June, 2014       Ankita Jain        SPR 11757 Fix
 * June, 2014       Ankita Jain        SPR 11792 Fix
 * June, 2014       Mayank Kumar       SPR 11780 Fix: Renamed X2_STATUS to X2_HO_STATUS
 * July, 2014       Ekta Jain          SPR 12091 Fix
 * July 2014        Shalu SAgar         SPR 12397 Fix
 * Aug 2014         Aditi Singh         SPR 13116 Fix
 * Oct, 2014        Ankita Jain        SPR 13586 Fix
 * Sep 2014         Mohit Anand        EICIC Changes
 * Nov 2014         Karuna Bhardwaj    EICIC Enhancement
 * * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 ******************************************************************************/
/*******************************************************************************
 * Includes
 *******************************************************************************/
#include <qcli.h>
#include <stdio.h>
#include<oam_qcli_proto.h>
#include <oam_qcli_cmd_tree.h>
#include<oam_defines.h>
#include<oam_proto.h>
#include "qval.h"
#include "qutil.h"
#include <qval.h>
#include <qmacro.h>

/* CSR 57877 FIx Start */
#include <oam_mgmt_intf_api.h>
/* CSR 57877 FIx End*/

#ifndef OAM_QCLI_RRM_1_2
#define OAM_QCLI_RRM_1_2 0
#endif
#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
#include<oam_rcm_if.h>
extern const struct command_t rrh_mode_handler[];
#endif
#endif
extern const struct command_t mbms_mode_handler[];


/* + SPR 17439 */
oam_return_et qcli_send_rrc_config_req (void);
/* - SPR 17439 */
extern prov_request_t prov_req;
int cache_track_index=OAM_ZERO;
int is_cache_full=OAM_ZERO;
mapping_cache_t mapping_cache[MAX_CACHE_LIMIT];
/* - SPR 17439 */
void oam_cli_abort(void);
/* + SPR 17439 */
oam_cli_pm_config_req_t cli_pm_config_req;

/* EMBMS Changes Start */
//Macro CLI_QSLALLOC moved to oam_qcli_proto.h
/* EMBMS Changes End */

param_mapping_t g_oam_qcli_param_mapping_table[]=
{
	{
		"RRM UE RELEASE",
		"RRM.UE.RELEASE",
		"X_VENDOR_COMMAND_UE_RELEASE_UE_INDEX",
		OAM_SET_PARAMETER_VALUES
	},
	/* CSR_00057386*/
	{
		"Show UE Status MAC",
		"UeStatus.MAC",
		"UEID",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show UE Status RRC",
		"Show.UeStatus.RRC",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show UeStatus PDCP",
		"Show.UeStatus.PDCP",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show UeStatus RLC",
		"Show.UeStatus.RLC",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show UeSINR MAC",
		"Show.UeSINR.MAC",
		"UEID",
		OAM_GET_PARAMETER_VALUES
	},	
	{
		"Show KPI Mac",
		"Show.KPI.MAC",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show KPI PDCP",
		"Show.KPI.PDCP",
		"",
		OAM_GET_PARAMETER_VALUES
	},

	{
		"Show KPI RRM",
		"Show.KPI.RRM",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show PerfStats PDCP",
		"Show.PerfStats.PDCP",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Reset stats L2 RLC",
		"Reset.stats.L2.RLC",
		"",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Reset KPI L2 MAC",
		"Reset.KPI.L2.MAC",
		"",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Reset KPI L2 PDCP",
		"Reset.KPI.L2.PDCP",
		"",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Reset KPI L3 RRC",
		"Reset.KPI.L3.RRC",
		"",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"Reset KPI RRM All",
		"Reset.KPI.RRM.All",
		"",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Reset stats L3 S1AP",
		"Reset.stats.L3.S1AP",
		"",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"Reset stats L2 MAC",
		"Reset.stats.L2.MAC",
		"",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Reset stats L2 PDCP",
		"Reset.stats.L2.PDCP",
		"",
		OAM_SET_PARAMETER_VALUES
	},
    {
        "Show CellStatus",
        "Show.CellStatus.RRC",
        "",
        OAM_GET_PARAMETER_VALUES
    },
    {
        "Show CellContext",
        "Cell_Context.Context",
        "",
        OAM_GET_PARAMETER_VALUES
    },
	{
		"Show Stats L2 MAC",
		"Show.Stats.L2.MAC",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Stats L2 RLC",
		"Show.Stats.L2.RLC",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Stats L2 PDCP",
		"Show.Stats.L2.PDCP",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Stats L2 EGTPU",
		"Show.Stats.L2.EGTPU",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Stats L3 S1AP",
		"Show.Stats.L3.S1AP",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show KPI L2 MAC",
		"Show.KPi.L2.MAC",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show KPI L2 PDCP",
		"Show.KPI.L2.PDCP",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show KPI L3 RRC",
		"Show.KPI.L3.RRC",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Config Kpi Periodic-Start",
		"InternetGatewayDevice.FAP.PerfMgmt.Config.Start",
		"X_VENDOR_SamplingInterval,PeriodicUploadTime,X_VENDOR_FileWriteTimer,PeriodicUploadInterval", /* SPR 8942 PM_FIX */
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config Kpi Periodic-Stop",
		"InternetGatewayDevice.FAP.PerfMgmt.Config.Stop",
		"",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config Kpi On-demand",
		"InternetGatewayDevice.FAP.PerfMgmt.Ondemand",
		"",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"LGW ADD",
		"LGW.Add",
		"Ip address",
		OAM_SET_PARAMETER_VALUES
	},	
	{
		"LGW DELETE",
		"LGW.Delete",
		"",
		OAM_SET_PARAMETER_VALUES
	},	
	{
		"Show KPI RRC",
		"Show.KPI.RRC",
		"",
		OAM_GET_PARAMETER_VALUES
	},

	{
		"Modify MACQosParams",
		"Modify.MACQosParams",
/*SPR 12091 FIX START*/
		"dlQosStrategyWeights.pdbWeight,dlQosStrategyWeights.qciWeight,dlQosStrategyWeights.tokenWeight,dlQosStrategyWeights.qLoadWeight,dlQosStrategyWeights.mcsWeight,ulQosStrategyWeights.pdbWeight,ulQosStrategyWeights.qciWeight,ulQosStrategyWeights.tokenWeight,ulQosStrategyWeights.qLoadWeight,ulQosStrategyWeights.mcsWeight,gbrBktDepthFactor,ambrBktDepthFactor,tokenPeriodicity,qosStrategyOverallocFactor,enableAllocAfterAllowedBitrate",
/*SPR 12091 FIX END*/
		OAM_SET_PARAMETER_VALUES
	},
	{
		"CellMgmt Start",
		"Cell.Start",
		"Cell_Identity",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"CellMgmt Stop",
		"Cell.Stop",
		"Cell_Identity",
		OAM_SET_PARAMETER_VALUES
	},
	/*CELL_DELETE_2.0*/
	{
		"CellMgmt Delete",
		"Cell.Delete",
		"Cell_Identity",
		OAM_SET_PARAMETER_VALUES

	},
	/*CELL_SETUP_2.0*/
	{
		"CellMgmt Setup",
		"Cell.Setup",
		"Cell_Identity",
		OAM_SET_PARAMETER_VALUES

	},

    /*SPR 13518 START*/
    {
        "CellMgmt Block",
        "Cell.Block",
        "Cell_Identity,Cell_Block_Priority,Resource_Cleanup_Timer",
        OAM_SET_PARAMETER_VALUES
    },
    /*SPR 13518 END*/
	{
		"CellMgmt Unblock",
		"Cell.Unblock",
		"Cell_Identity",
		OAM_SET_PARAMETER_VALUES
	},
	/**CSR 00057534 FIX start**/
	{
		"Enable RRM",
		"Enable.RRM",
		"Radio_admission_control",
		OAM_SET_PARAMETER_VALUES
	},

    {
        "Enable RRM",
        "Enable.RRM",
        "LoadReporting",
        OAM_SET_PARAMETER_VALUES
    },
	/**CSR 00057534 FIX end**/
	{
		"Enable ECNCapacityEnhancement",
		"Enable.RRM.ECNCapacityEnhancement",
		"NumberOfUe,QCI_Number,UL_Bitrate_Max,UL_Bitrate_Min,DL_Bitrate_Max,DL_Bitrate_Min",
		OAM_SET_PARAMETER_VALUES
	},


	{
		"Disable RRM",
		"Disable.RRM",
		"Radio_admission_control",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config LoadReporting ServingCellReport",
		"LoadReporting.serving_cell",
		"ReportingInterval",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config LoadReporting NeighborCellReport",
		"LoadReporting.neighbor_cell",
		"ReportingInterval",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"Disable ECNCapacityEnhancement",
		"Disable.RRM.ECNCapacityEnhancement",
		"number_of_ue",
		OAM_SET_PARAMETER_VALUES
	},
#ifdef OAM_SON_ENABLE

	{
		"SON RACH-OPT ENABLE",
		"SON_RACH-OPT_ENABLE",
		"serving_cell_identity",
		OAM_SET_PARAMETER_VALUES
	},	
	{
		"SON RACH-OPT DISABLE",
		"SON_RACH-OPT_DISABLE",
		"serving_cell_identity",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON RACH-OPT MODIFY_MODE",
		"SON_RACH-OPT_MODIFY_MODE",
		"rach_opt_mode",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON RACH-OPT CONFIG",
		"SON_RACH-OPT_CONFIG",
		"RACHOPT_REPORT_PERIODICITY,RACHOPT_MIN_UE_RACH_REPORT_COUNT,RACHOPT_UE_RACH_ATTEMPTS_THRESHOLD,RACHOPT_START_THRESHOLD,RACHOPT_STOP_THRESHOLD,RACHOPT_DEGRADE_STOP_OFFSET,RACHOPT_MAX_ITERATIONS_TO_REACH_STOP_THRESHOLD,RACHOPT_CONTENTION_THRESHOLD,RACHOPT_POWER_RAMPUP_THRESHOLD,RACHOPT_MIN_CFRA_PREAMBLES",
		OAM_SET_PARAMETER_VALUES
	},	

     /* eICIC_PHASE_1_2_CHANGES_START */
    /*bug_13878_start */
    {
        "SON ANR NEIGHBOR-DEFAULT-CONFIG SET",
        "SON.ANR.NEIGHBOR-DEFAULT-CONFIG",
        "nr_status,ho_status,x2_status,csg_cell_intf_scheme,"
        "pico_cell_intf_scheme,macro_cell_intf_scheme",
        OAM_SET_PARAMETER_VALUES
    },

    /*bug_13878_end */

    /*bug_13893_start*/
    {
        "SON ANR NEIGHBOR-DEFAULT-CONFIG RETREIVE",
        "SON.ANR.NEIGHBOR-DEFAULT-CONFIG",
        "nr_status,ho_status,x2_status,csg_cell_intf_scheme,"
        "pico_cell_intf_scheme,macro_cell_intf_scheme",
        OAM_GET_PARAMETER_VALUES
    },

    /*bug_13893_end*/
    /*SPR 14554 start*/
    {
        "SON CELL-INFO_eICICParams",
        "SON.CELL-INFO_eICICParams",
        "min_abs_usage_threshold,max_abs_usage_threshold",
        OAM_SET_PARAMETER_VALUES 
    },
    /*SPR 14554 end*/

     /* eICIC_PHASE_1_2_CHANGES_END */
    /* SPR FIX 11266 + */
	/****** SON EUTRAN Prameters ******/
      /* spr_21013_fix_start */
      /* As a part of SPR 21013 changes, InternetGatewayDevice.Services.FAPService.[1].
       * has been changed to InternetGatewayDevice.Services.FAPService. hierarchy */
	{
		"SON ANR MEAS-CONFIG ADD EUTRAN",
		"InternetGatewayDevice.Services.FAPService.CellConfig."
			"LTE.RAN.Mobility.IdleMode.InterFreq.Carrier",
        /* SPR 14651 START */
        "Enable,EUTRACarrierARFCN,X_VENDOR_MEAS_BANDWIDTH_FOR_EARFCN,"
        /* SPR 14651 END */
		"X_VENDOR_PRESENCE_ANTENNA_PORT1,"
		"X_VENDOR_NEIGHBOR_CELL_CONFIG,QOffsetFreq,QRxLevMinSIB5,"
		"PMax,TReselectionEUTRA,TReselectionEUTRASFMedium,"
		"TReselectionEUTRASFHigh,ThreshXHigh,ThreshXLow,"
        /* SPR 14651 START */
		"CellReselectionPriority,"
        /* SPR 14651 END */
        "X_VENDOR_THRESHX_HIGHQ_R9,X_VENDOR_THRESHX_LOWQ_R9,"
        "X_VENDOR_Q_QUAL_MIN_R9,X_VENDOR_OPEN_PHY_CELLID_START,"
		"X_VENDOR_OPEN_PHY_CELLID_RANGE,X_VENDOR_CSG_PHY_CELLID_START,"
		"X_VENDOR_CSG_PHY_CELLID_RANGE,X_VENDOR_HYBRID_PHY_CELLID_START,"
		"X_VENDOR_HYBRID_PHY_CELLID_RANGE",
		OAM_SET_PARAMETER_VALUES | OAM_ADD_OBJECT_REQ
	},
	{
		"SON ANR MEAS-CONFIG UPDATE EUTRAN",
		"InternetGatewayDevice.Services.FAPService.CellConfig."
			"LTE.RAN.Mobility.IdleMode.InterFreq.Carrier",
        /* SPR 14651 START */
		"EUTRACarrierARFCN,Enable,X_VENDOR_MEAS_BANDWIDTH_FOR_EARFCN,"
        /* SPR 14651 END */
		"X_VENDOR_PRESENCE_ANTENNA_PORT1,"
		"X_VENDOR_NEIGHBOR_CELL_CONFIG,QOffsetFreq,QRxLevMinSIB5,"
		"PMax,TReselectionEUTRA,TReselectionEUTRASFMedium,"
		"TReselectionEUTRASFHigh,ThreshXHigh,ThreshXLow,"
        /* SPR 14651 START */
		"CellReselectionPriority,"
        /* SPR 14651 END */
        "X_VENDOR_Q_QUAL_MIN_R9,X_VENDOR_THRESHX_HIGHQ_R9,"
        "X_VENDOR_THRESHX_LOWQ_R9,X_VENDOR_OPEN_PHY_CELLID_START,"
		"X_VENDOR_OPEN_PHY_CELLID_RANGE,X_VENDOR_CSG_PHY_CELLID_START,"
		"X_VENDOR_CSG_PHY_CELLID_RANGE,X_VENDOR_HYBRID_PHY_CELLID_START,"
		"X_VENDOR_HYBRID_PHY_CELLID_RANGE",
		OAM_SET_PARAMETER_VALUES
	},

	/****** SON GERAN Prameters ******/
	{
		"SON ANR MEAS-CONFIG ADD GERANFreqGroup",
		"InternetGatewayDevice.Services.FAPService.CellConfig."
			"LTE.RAN.Mobility.IdleMode.IRAT.GERAN.GERANFreqGroup",
        /* SPR 14651 START */
		"Enable,BCCHARFCN,BandIndicator,X_VENDOR_NCC_PERMITTED,ThreshXHigh,"
        "ThreshXLow,QRxLevMin,"
        /* SPR 14651 END */
        "X_VENDOR_P_MAX_GERAN,CellReselectionPriority,"
        "X_VENDOR_ARFCN_SPACING,X_VENDOR_NUM_OF_FOLLOWING_ARFCNS,"
        "X_VENDOR_EXPLICIT_ARFCN_LIST,X_VENDOR_ARFCN_BITMAP",
		OAM_SET_PARAMETER_VALUES | OAM_ADD_OBJECT_REQ
	},
	{
		"SON ANR MEAS-CONFIG UPDATE GERANFreqGroup",
		"InternetGatewayDevice.Services.FAPService.CellConfig."
			"LTE.RAN.Mobility.IdleMode.IRAT.GERAN.GERANFreqGroup",
        /* SPR 14651 START */
		"BCCHARFCN,BandIndicator,Enable,X_VENDOR_NCC_PERMITTED,ThreshXHigh,"
        "ThreshXLow,QRxLevMin,X_VENDOR_P_MAX_GERAN,"
        /* SPR 14651 END */
        "CellReselectionPriority,X_VENDOR_ARFCN_SPACING,"
        "X_VENDOR_NUM_OF_FOLLOWING_ARFCNS,"
        "X_VENDOR_EXPLICIT_ARFCN_LIST,X_VENDOR_ARFCN_BITMAP",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON ANR MEAS-CONFIG UPDATE GERAN",
		"InternetGatewayDevice.Services.FAPService.CellConfig."
			"LTE.RAN.Mobility.IdleMode.IRAT.GERAN",
		"TReselectionGERAN,TReselectionGERANSFMedium,TReselectionGERANSFHigh",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON ANR MEAS-CONFIG ADD UTRANFDDFreq",
		"InternetGatewayDevice.Services.FAPService.CellConfig."
			"LTE.RAN.Mobility.IdleMode.IRAT.UTRA.UTRANFDDFreq",
        /* SPR 14651 START */
		"Enable,UTRACarrierARFCN,CellReselectionPriority,ThreshXHigh,ThreshXLow,"
        /* SPR 14651 END */
        "QRxLevMin,PMaxUTRA,QQualMin,X_VENDOR_THRESHX_HIGHQ_R9,"
        "X_VENDOR_THRESHX_LOWQ_R9,X_VENDOR_OFFSET_FREQUENCY",
		OAM_SET_PARAMETER_VALUES | OAM_ADD_OBJECT_REQ
	},
	{
		"SON ANR MEAS-CONFIG UPDATE UTRANFDDFreq",
		"InternetGatewayDevice.Services.FAPService.CellConfig."
			"LTE.RAN.Mobility.IdleMode.IRAT.UTRA.UTRANFDDFreq",
        /* SPR 14651 START */
		"UTRACarrierARFCN,Enable,CellReselectionPriority,ThreshXHigh,ThreshXLow,"
        "QRxLevMin,PMaxUTRA,QQualMin,"
        /* SPR 14651 END */
        "X_VENDOR_THRESHX_HIGHQ_R9,X_VENDOR_THRESHX_LOWQ_R9",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON ANR MEAS-CONFIG UPDATE UTRA",
		"InternetGatewayDevice.Services.FAPService.CellConfig."
			"LTE.RAN.Mobility.IdleMode.IRAT.UTRA",
		"TReselectionUTRA,TReselectionUTRASFMedium,TReselectionUTRASFHigh",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON ANR MEAS-CONFIG DELETE EUTRAN",
		"InternetGatewayDevice.Services.FAPService.CellConfig."
			"LTE.RAN.Mobility.IdleMode.InterFreq.Carrier",
		"TReselectionUTRA,TReselectionUTRASFMedium,TReselectionUTRASFHigh",
		OAM_DELETE_OBJECT_REQ
	},
	{
		"SON ANR MEAS-CONFIG DELETE UTRANFDDFreq",
		"InternetGatewayDevice.Services.FAPService.CellConfig."
			"LTE.RAN.Mobility.IdleMode.IRAT.UTRA.UTRANFDDFreq",
		"TReselectionUTRA,TReselectionUTRASFMedium,TReselectionUTRASFHigh",
		OAM_DELETE_OBJECT_REQ
	},
	{
		"SON ANR MEAS-CONFIG DELETE GERANFreqGroup",
		"InternetGatewayDevice.Services.FAPService.CellConfig."
			"LTE.RAN.Mobility.IdleMode.IRAT.GERAN.GERANFreqGroup",
		"",
		OAM_DELETE_OBJECT_REQ
	},
    /* SPR FIX 11266 - */
/* BUG_1171_START */
   {
        /* SPR 11467 START */
        "SON PCI-CONFLICT PCI-MODE",
        "SON.PCI-CONFLICT.PCI-MODE",
        "ENABLE",
        OAM_SET_PARAMETER_VALUES
        /* SPR 11467 END */
    },

/* BUG_1171_END */

	
	{
		"SON ANR MODIFY UE_COUNT",
		"SON.ANR.MODIFY.UE_COUNT",
		"UE_COUNT",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"SON ANR MODIFY MEAS_REPORTING_INTERVAL",
		"SON.ANR.MODIFY.MEAS_REPORTING_INTERVAL",
		"Meas_Reporting_Interval",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"SON ANR MODIFY REMOVAL_ATTRIBUTES",
		"SON.ANR.MODIFY.REMOVAL_ATTRIBUTES",
		"No_Activity_Interval,Handover_Failure_Threshold,Timeout_No_NR_Neighbors",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"SON ANR MODIFY PCI_CONFUSION_CFG",
		"SON.ANR.MODIFY.PCI_CONFUSION_CFG",
		"Suspect_PCI_Threshold,Clear_Suspect_PCI_Threshold",
		OAM_SET_PARAMETER_VALUES
	},
    /* SPR 20653 Fix Start */
	{
		"SON ANR MODIFY ATTRIBUTE",
		"SON.ANR.MODIFY.ATTRIBUTE",
		"UE_Throughput_DL_ANR_Threshold,UE_Throughput_UL_ANR_Threshold",
		OAM_SET_PARAMETER_VALUES
	},
    /* SPR 20653 Fix End */


	/****** SON NEIGHBOR CELL Prameters ******/

/* SPR-11792 Start */
	{
		"SON NR ADD NEIGHBOR_CELL EUTRAN",
		"InternetGatewayDevice.Services.FAPService.CellConfig."
			"LTE.RAN.NeighborList.LTECell",
/* Spr 15438 Fix Start */
		"Enable,MustInclude,PLMNID,CID,PhyCellID,EUTRACarrierARFCN,Blacklisted,"
/* Spr 15438 Fix End */
    /* SPR 11156 FIX START */
            "X_VENDOR_ACCESS_MODE,X_VENDOR_NR_STATUS,QOffset,X_VENDOR_TAC,"
            "X_VENDOR_CSGID,X_VENDOR_RSRP,X_VENDOR_RSRQ,CIO,"
            "X_VENDOR_BROADCAST_STATUS,X_VENDOR_PRACH_ROOT_SEQUENCE_INDEX,"
	    "X_VENDOR_PRACH_ZERO_CORRELATION_ZONE_CONFIG,"
	    "X_VENDOR_PRACH_HIGH_SPEED_FLAG,"
	    "X_VENDOR_PRACH_FREQUENCY_OFFSET,"
	    "X_VENDOR_PRACH_CONFIGURATION_INDEX,"
        "X_VENDOR_INTERFERENCE_SCHEME,X_VENDOR_MIN_ABS_USAGE_THRESHOLD,X_VENDOR_MAX_ABS_USAGE_THRESHOLD",
    /* SPR 11156 FIX END */
		(OAM_SET_PARAMETER_VALUES)  | (OAM_ADD_OBJECT_REQ)
	},
/* SPR-11792 End */
	{
		"SON NR UPDATE NEIGHBOR_CELL EUTRAN",
		"InternetGatewayDevice.Services.FAPService.CellConfig."
			"LTE.RAN.NeighborList.LTECell",
/* Spr 15438 Fix Start */
		"PLMNID,CID,Enable,MustInclude,PhyCellID,QOffset,EUTRACarrierARFCN,"
/* Spr 15438 Fix End */
            /* OAM BCOM Code Comment Changes Start */
            "Blacklisted,X_VENDOR_NR_STATUS,"
            /* OAM BCOM Code Comment Changes End */
			"X_VENDOR_TAC,X_VENDOR_CSGID,X_VENDOR_RSRP,"
			"X_VENDOR_RSRQ,CIO,"
			"X_VENDOR_BROADCAST_STATUS,X_VENDOR_ACCESS_MODE,"
			"X_VENDOR_PRACH_ROOT_SEQUENCE_INDEX,"
			"X_VENDOR_PRACH_ZERO_CORRELATION_ZONE_CONFIG,X_VENDOR_PRACH_HIGH_SPEED_FLAG,"
			"X_VENDOR_PRACH_FREQUENCY_OFFSET,X_VENDOR_PRACH_CONFIGURATION_INDEX,"
            "X_VENDOR_INTERFERENCE_SCHEME,X_VENDOR_MIN_ABS_USAGE_THRESHOLD,X_VENDOR_MAX_ABS_USAGE_THRESHOLD",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON NR DELETE NEIGHBOR_CELL EUTRAN",
		"InternetGatewayDevice.Services.FAPService.CellConfig."
			"LTE.RAN.NeighborList.LTECell",
		"PLMNID,CID",
		OAM_DELETE_OBJECT_REQ

	},


	/****** SON NEIGHBOR ENB Prameters ******/

	{
		"SON NR ADD NEIGHBOR_ENB",
		"eNodeBParams.oam_son_req.oam_son_nr_enb_add_req.nbr_enb_list[255]",
		"action,enb_type,plmn_id,enb_id,tac,enb_tnl_address0,enb_tnl_address1,x2_status,x2_connection_status",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON NR UPDATE NEIGHBOR_ENB",
		"eNodeBParams.oam_son_req.oam_son_nr_enb_add_req.nbr_enb_list[255]",
		"action,enb_type,plmn_id,enb_id,tac,enb_tnl_address0,enb_tnl_address1,x2_status,x2_connection_status",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON NR DELETE NEIGHBOR_ENB",
		"eNodeBParams.oam_son_req.oam_son_nr_enb_add_req.nbr_enb_list[255]",
		"action,enb_type,plmn_id,enb_id",
		OAM_SET_PARAMETER_VALUES
	},

/* SPR-16334 START */
#else
    {
        "X2 NR ADD/MODIFY NEIGHBOR_ENB",
        "L3Params.X2AP.peer_eNB_CommInfo",
        "Action,IPAddr,Port,IPv6Addr",
        OAM_SET_PARAMETER_VALUES
    },
    {
        "X2 NR DELETE NEIGHBOR_ENB",
        "L3Params.X2AP.ENB_Delete",
        "enb_id,plmn_identity",
        OAM_SET_PARAMETER_VALUES
    },   
#endif
/* SPR-16334 END */


	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY PRS",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PRS",
		"NumConsecutivePRSSubframes,NumPRSResourceBlocks,PRSConfigurationIndex,X_VENDOR_PRS_ENABLE,X_VENDOR_MUTING_CONFIG_TWO,X_VENDOR_MUTING_CONFIG_FOUR,X_VENDOR_MUTING_CONFIG_EIGHT,X_VENDOR_MUTING_CONFIG_SIXTEEN,X_VENDOR_PRS_TRANSMISSION_POWER,X_VENDOR_LATITUDE_SIGN,X_VENDOR_DEG_OF_LATITUDE,X_VENDOR_DEG_OF_LONGITUDE,X_VENDOR_DIR_OF_ALTITUDE,X_VENDOR_ALTITUDE,X_VENDOR_UNCERTAINTY_SEMI_MAJOR,X_VENDOR_UNCERTAINTY_SEMI_MINOR,X_VENDOR_ORIENTATION_OF_MAJOR_AXIS,X_VENDOR_UNCERTAINTY_ALTITUDE,X_VENDOR_CONFIDENCE",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"Config InternetGatewayDevice Services FAPService AccessMgmt LTE",
		"InternetGatewayDevice.Services.FAPService.AccessMgmt.LTE",
		"AccessMode,CSGID,HNBName,MaxCSGMembers,MaxNonCSGMembers",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config SaveConfig",
		"Config.SaveConfig",
		"XML_TYPE",
		OAM_SET_PARAMETER_VALUES
	},

#ifdef OAM_SON_ENABLE /* FLAG_CHANGES */
	/****** SON UTRAN Prameters ******/

	/* RT SPR 8627 FIX START */
	{
        "SON NR ADD NEIGHBOR_CELL UTRAN",
		"InternetGatewayDevice.Services.FAPService.CellConfig."
			"LTE.RAN.NeighborList.InterRATCell.UMTS",
/* Spr 15612  Fix Start*/ 
        "Enable,MustInclude,PLMNID,CID,RNCID,UARFCNDL,X_VENDOR_NR_STATUS,"
/* Spr 15612  Fix End*/ 
			"X_VENDOR_HO_STATUS,PCPICHScramblingCode,LAC,RAC,"
			"X_VENDOR_CELL_SPECIFIC_OFFSET,X_VENDOR_CSGID,"
			"X_VENDOR_RSCP,X_VENDOR_ECNO,"
            "X_VENDOR_ACCESS_MODE",
		(OAM_SET_PARAMETER_VALUES) | (OAM_ADD_OBJECT_REQ)
	},
    {
        "SON NR UPDATE NEIGHBOR_CELL UTRAN",
        "InternetGatewayDevice.Services.FAPService.CellConfig."
            "LTE.RAN.NeighborList.InterRATCell.UMTS",
/* Spr 15612  Fix Start*/ 
        "PLMNID,CID,RNCID,Enable,MustInclude,LAC,RAC,PCPICHScramblingCode,UARFCNDL,"
/* Spr 15612  Fix End*/ 
            "X_VENDOR_CELL_SPECIFIC_OFFSET,X_VENDOR_CSGID,"
            "X_VENDOR_RSCP,X_VENDOR_ECNO,X_VENDOR_NR_STATUS,"
            "X_VENDOR_HO_STATUS,X_VENDOR_ACCESS_MODE",
		OAM_SET_PARAMETER_VALUES
	},
	/* RT SPR 8627 FIX END */
	{
		"SON NR DELETE NEIGHBOR_CELL UTRAN",
		"InternetGatewayDevice.Services.FAPService.CellConfig."
			"LTE.RAN.NeighborList.InterRATCell.UMTS",
		"PLMNID,CID",
		OAM_DELETE_OBJECT_REQ
	},
/* SPR-13586 START */
   {
        "SON NR ADD NEIGHBOR_CELL GERAN",
        "InternetGatewayDevice.Services.FAPService.CellConfig."
            "LTE.RAN.NeighborList.InterRATCell.GSM",
        "PLMNID,LAC,CI,BandIndicator,BCCHARFCN,BSIC,X_VENDOR_NR_STATUS,"
            "X_VENDOR_HO_STATUS,X_VENDOR_RAC,X_VENDOR_CELL_SPECIFIC_OFFSET,"
            "X_VENDOR_RSSI",
        (OAM_SET_PARAMETER_VALUES) | (OAM_ADD_OBJECT_REQ)
    },
    {
        "SON NR UPDATE NEIGHBOR_CELL GERAN",
        "InternetGatewayDevice.Services.FAPService.CellConfig."
            "LTE.RAN.NeighborList.InterRATCell.GSM",
        "PLMNID,LAC,CI,BandIndicator,BCCHARFCN,BSIC,X_VENDOR_NR_STATUS,"
            "X_VENDOR_HO_STATUS,X_VENDOR_RAC,X_VENDOR_CELL_SPECIFIC_OFFSET,"
            "X_VENDOR_RSSI",
        OAM_SET_PARAMETER_VALUES
    },
    {
        "SON NR DELETE NEIGHBOR_CELL GERAN",
        "InternetGatewayDevice.Services.FAPService.CellConfig."
            "LTE.RAN.NeighborList.InterRATCell.GSM",
        "PLMNID,LAC,CI",
        OAM_DELETE_OBJECT_REQ
    },
/* SPR-13586 END */
    /* SPR 20653 Fix Start */
	{
		"SON ANR ENABLE",
		"SON_ANR_ENABLE",
		"ue_count,reporting_interval,limited_mode,no_activity_interval,ho_failure_threshold,timeout_no_nr_neighbors,suspect_pci_threshold,clear_suspect_pci_threshold,ue_throughput_dl_anr_threshold,ue_throughput_ul_anr_threshold",
		OAM_SET_PARAMETER_VALUES
	},
    /* SPR 20653 Fix End */
	{
		"SON ANR DISABLE",
		"SON_ANR_DISABLE",
		"DUMMY_PARAM",
		OAM_SET_PARAMETER_VALUES
	},
	/* RT SPR 8746 FIX START */
	{
		"SON X2 ENABLE",
		"SON_NR_CONFIG_X2_ENABLE",
		"SON_NR_CONFIG_X2_ENABLE",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON X2 DISABLE",
		"SON_NR_CONFIG_X2_DISABLE",
		"SON_NR_CONFIG_X2_DISABLE",
		OAM_SET_PARAMETER_VALUES
	},
	/* RT SPR 8746 FIX END */
	{
		"SON NR DELETE_FROM_REMOVE_LIST INTRA_RAT",
		"SON_NR_DELETE_FROM_REMOVE_LIST_INTRA_RAT",
		"PLMNID,CID", 
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON NR DELETE_FROM_REMOVE_LIST INTER_RAT UTRAN",
		"SON_NR_DELETE_FROM_REMOVE_LIST_INTER_RAT_UTRAN",
		/* RT SPR 8627 FIX START */
		"PLMNID,RNCID,CID", 
		/* RT SPR 8627 FIX END */
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON NW SCAN EUTRAN",
		"InternetGatewayDevice.Services.FAPService.REM.LTE",
/* SPR_12373_CHANGES */
		"X_VENDOR_MEASUREMENT_PERIOD,X_VENDOR_RETRY_COUNT,REMBandList,EUTRACarrierARFCNDLList,X_VENDOR_PCI_LIST,X_VENDOR_MEAS_BANDWIDTH,REMPLMNList",
/* SPR_12373_CHANGES */
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON NW PERIODIC_SCAN EUTRAN",
		"InternetGatewayDevice.Services.FAPService.REM.LTE",
		"ScanPeriodically,PeriodicInterval,X_VENDOR_MEASUREMENT_PERIOD,X_VENDOR_RETRY_COUNT,"
			"REMBandList,EUTRACarrierARFCNDLList,X_VENDOR_PCI_LIST,X_VENDOR_MEAS_BANDWIDTH,REMPLMNList",
/* SPR_12373_CHANGES */
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON NW PERIODIC_SCAN STOP",
		"InternetGatewayDevice.Services.FAPService.REM.LTE",
		"ScanPeriodically",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON NW SCAN_CONFIG",
		"InternetGatewayDevice.Services.FAPService.REM.LTE",
		"X_VENDOR_FORCEFUL_SCAN_FLAG,X_VENDOR_SCAN_DATA_TIMEOUT",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config InternetGatewayDevice Services FAPService FAPControl LTE eNBState",
		"InternetGatewayDevice.Services.FAPService.FAPControl.LTE",
		"AdminState",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"SON NW SCAN_STATUS",
		"InternetGatewayDevice.Services.FAPService.REM.LTE",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"SON NW SCAN_RESULTS RF",
		"InternetGatewayDevice.Services.FAPService.REM.LTE.Cell",
		"Index",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"SON NW SCAN_RESULTS BCCH",
		"InternetGatewayDevice.Services.FAPService.REM.LTE.Cell",
		"Index",
		OAM_GET_PARAMETER_VALUES
	},

#endif
/* eICIC_PHASE_1_2_CHANGES_START */
	{
		"Config LoadReporting OverLoadLevel",
		"LoadReporting.OverLoadLevel",
		"Percentage,Action,eligible_ue,abs_pattern",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config LoadReporting HighLoadLevel",
		"LoadReporting.HighLoadLevel",
		"Percentage,Action,eligible_ue,abs_pattern",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config LoadReporting MidLoadLevel",
		"LoadReporting.MidLoadLevel",
		"Percentage,Action,eligible_ue,abs_pattern",
		OAM_SET_PARAMETER_VALUES
	},
/* eICIC_PHASE_1_2_CHANGES_END */
	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY PRS",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PRS",
		"NumConsecutivePRSSubframes,NumPRSResourceBlocks,PRSConfigurationIndex",
		OAM_SET_PARAMETER_VALUES
	}, 
    /* EMBMS Changes Start */
	{
        "Config InternetGatewayDevice Services FAPService CellConfig LTE RAN"
            " PHY MBSFN",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN."
            "PHY.MBSFN",
		"NeighCellConfig",
		OAM_SET_PARAMETER_VALUES
	}, 
#ifdef LTE_EMBMS_SUPPORTED
    {
        /* 7.0 OAM_SSI Fix Start*/
        "Set Log Level M2AP_LOG_LEVEL",
        "eNodeBParams.stack_config.L3Params.M2AP",
        /* 7.0 OAM_SSI Fix End*/
        "M2AP_LOG_LEVEL",
        OAM_SET_PARAMETER_VALUES

    },
    {
        "MBMS Config X_VENDOR_ENB_NAME",
        "InternetGatewayDevice.Services.FAPService.AccessMgmt.LTE",
        "X_VENDOR_ENB_NAME",
        OAM_SET_PARAMETER_VALUES
    },
    {
        "MBMS Config M2AP",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY."
            /* 7.0 OAM_SSI Fix Start*/
            "MBSFN.MBSFN_SERVICE_AREA_CONFIG",
        /* 7.0 OAM_SSI Fix End*/
        "X_VENDOR_MBSFN_SYNC_AREA_ID,X_VENDOR_MBMS_SERVICE_AREA_ID,"
            "X_VENDOR_IS_EMBMS_ENABLED",
        OAM_SET_PARAMETER_VALUES
    },
    {
        "MBMS Config RRM",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY."
            "MBSFN",
        "X_VENDOR_MBMS_TRANSMISSION_POWER,"
            "X_VENDOR_RESERVED_CELL_POWER_OFFSET",
        OAM_SET_PARAMETER_VALUES
    },
    {
        "MBMS Config FDD SIB2",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY."
            "MBSFN.SFConfigList",
        "RadioFrameAllocationPeriod,RadioframeAllocationOffset,"
            "RadioframeAllocationSize,SubFrameAllocations",
        OAM_SET_PARAMETER_VALUES
    },
    {
        "MBMS Config TDD SIB2",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY."
            "MBSFN.SFConfigList",
        "SyncStratumID,RadioFrameAllocationPeriod,"
            "RadioframeAllocationOffset,RadioframeAllocationSize,SubFrameAllocations",
        OAM_SET_PARAMETER_VALUES
    },
    /* 7.0 OAM_SSI Fix Start*/
    {
        "MBMS Reset Session All",
        "MBMS.Reset.Session",
        "",
        OAM_SET_PARAMETER_VALUES
    },
    {
        "MBMS Reset Session Partial",
        "MBMS.Reset.Session",
        "Session_ID",
        OAM_SET_PARAMETER_VALUES
    },
    /* 7.0 OAM_SSI Fix End*/
#endif
    /* EMBMS Changes End*/

	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN RLC SRB1",

		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RLC.SRB1",
		"DefaultConfiguration,MaxRetxThreshold,PollByte,PollPDU,TPollRetransmit,TReordering,TStatusProhibit",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN RLC SRB2",

		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RLC.SRB2",
		"DefaultConfiguration,MaxRetxThreshold,PollByte,PollPDU,TPollRetransmit,TReordering,TStatusProhibit",
		OAM_SET_PARAMETER_VALUES
	},	
	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility IdleMode Common",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.Common",
		"NCellChangeHigh,NCellChangeMedium,Qhyst,QHystSFHigh,QHystSFMedium,TEvaluation,THystNormal",
		OAM_SET_PARAMETER_VALUES
	},


	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN CellRestriction",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.CellRestriction",
		"CellBarred,BarringForEmergency",
		OAM_SET_PARAMETER_VALUES
	},

    /* SPR 13970 FIX Start */
    {
        "Config InternetGatewayDevice Services FAPService CellConfig LTE RAN CellRestriction-AC_BARRING_FOR_MO_SIGNALLING",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.CellRestriction.X_VENDOR_ACCESS_BARRING_INFO.AC_BARRING_FOR_MO_SIGNALLING",
        "AC_BARRING_FACTOR,AC_BARRING_TIME,AC_BARRING_FOR_SPECIAL_AC",
        OAM_SET_PARAMETER_VALUES
    },

    {
        "Config InternetGatewayDevice Services FAPService CellConfig LTE RAN CellRestriction-AC_BARRING_FOR_MO_DATA",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.CellRestriction.X_VENDOR_ACCESS_BARRING_INFO.AC_BARRING_FOR_MO_DATA",
        "AC_BARRING_FACTOR,AC_BARRING_TIME,AC_BARRING_FOR_SPECIAL_AC",
        OAM_SET_PARAMETER_VALUES
    },

    {
        "Config InternetGatewayDevice Services FAPService CellConfig LTE RAN CellRestriction-SSAC_BARRING_FOR_MMTEL_VOICE_R9",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.CellRestriction.X_VENDOR_ACCESS_BARRING_INFO.SSAC_BARRING_FOR_MMTEL_VOICE_R9",
        "AC_BARRING_FACTOR,AC_BARRING_TIME,AC_BARRING_FOR_SPECIAL_AC",
        OAM_SET_PARAMETER_VALUES
    },

    {
        "Config InternetGatewayDevice Services FAPService CellConfig LTE RAN CellRestriction-SSAC_BARRING_FOR_MMTEL_VIDEO_R9",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.CellRestriction.X_VENDOR_ACCESS_BARRING_INFO.SSAC_BARRING_FOR_MMTEL_VIDEO_R9",
        "AC_BARRING_FACTOR,AC_BARRING_TIME,AC_BARRING_FOR_SPECIAL_AC",
        OAM_SET_PARAMETER_VALUES
    },

    {
        "Config InternetGatewayDevice Services FAPService CellConfig LTE RAN CellRestriction-AC_BARRING_FOR_CSFB_R10",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.CellRestriction.X_VENDOR_ACCESS_BARRING_INFO.AC_BARRING_FOR_CSFB_R10",
        "AC_BARRING_FACTOR,AC_BARRING_TIME,AC_BARRING_FOR_SPECIAL_AC",
        OAM_SET_PARAMETER_VALUES
    },
    /* SPR 13970 FIX End */


	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Common",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Common",
		"CellIdentity",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility IdleMode IntraFreq",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IntraFreq",
		"QRxLevMinSIB1,QRxLevMinSIB3,QRxLevMinOffset,SIntraSearch,TReselectionEUTRA,SNonIntraSearch,CellReselectionPriority,ThreshServingLow,TReselectionEUTRASFHigh,TReselectionEUTRASFMedium",/*BUG_FIX_ID:817*/
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility IdleMode InterFreq InterFreq",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.InterFreq",
		"MaxCarrierEntries,CarrierNumberOfEntries",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility IdleMode IRAT UTRA TReselectionUTRA",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IRAT.UTRA",
		"TReselectionUTRA",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility IdleMode IRAT UTRA TReselectionUTRASFMedium",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IRAT.UTRA",
		"TReselectionUTRASFMedium",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility IdleMode IRAT UTRA TReselectionUTRASFHigh",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IRAT.UTRA",
		"TReselectionUTRASFHigh",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY PRACH",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PRACH",
		"ConfigurationIndex,FreqOffset,HighSpeedFlag,RootSequenceIndex,ZeroCorrelationZoneConfig",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY PDSCH",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PDSCH",
		"Pa,Pb",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY PUSCH",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PUSCH",
		"HoppingMode,HoppingOffset,NSB",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY PUSCH-ULRS",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PUSCH.ULRS",
		"GroupHoppingEnabled,SequenceHoppingEnabled",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY PUCCH",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PUCCH",
		/* SPR 22311 Changes - Obselete Parameter Deleted */
        "DeltaPUCCHShift,N1PUCCHAN,NRBCQI",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY SRS",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.SRS",
		"AckNackSRSSimultaneousTransmission,SRSBandwidthConfig,SRSEnabled,SRSMaxUpPTS",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY ULPowerControl",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.ULPowerControl",
		"Alpha,P0NominalPUCCH,P0NominalPUSCH",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN MAC RACH",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.RACH",
		"MaxHARQMsg3Tx,MessagePowerOffsetGroupB,PowerRampingStep,PreambleInitialReceivedTargetPower,PreambleTransMax,SizeOfRaGroupA,MessageSizeGroupA,ContentionResolutionTimer,NumberOfRaPreambles,ResponseWindowSize", 
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN RRCTimers",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RRCTimers",
		"N310,N311,T300,T301,T302,T310,T311,T320,T304EUTRA,T304IRAT",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN RF",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RF",
        /* Spr 13033 Fix Start */
		"FreqBandIndicator,DLBandwidth,ULBandwidth,ReferenceSignalPower,PhyCellID,PSCHPowerOffset,SSCHPowerOffset,PBCHPowerOffset,EARFCNDL,EARFCNUL",
        /* Spr 13033 Fix End */
		OAM_SET_PARAMETER_VALUES
	},
    /*SPR 21159 Fixed Start*/
	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE EPC EAID_PARAM",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC",
		"EAID",
		OAM_SET_PARAMETER_VALUES
	},

    /*SPR 21159 Fixed End*/

    /*SPR 21280 FIXED START*/
    /*SPR 21158 Fixed Start*/
	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE EPC TAC_PARAM",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC",
		"TAC",
		OAM_SET_PARAMETER_VALUES
	},

    /*SPR 21158 Fixed End*/
    /*SPR 21280 FIXED END*/

/*LTE_OAM_INHANCEMENT_CA_CHANGED */
    /* SPR 14295 FIX START */
    {
        "Config InternetGatewayDevice Services FAPService CellConfig "
            "CarrierAggregation GeneralParams",
        "InternetGatewayDevice.Services.FAPService.CellConfig."
            "X_VENDOR_CARRIER_AGGREGATION_PARAMS",
        "isCAEligible,minNumUEThruputReport,applicableBitrateDL,isMASA,"
            "ScellDeactTimer",
        OAM_SET_PARAMETER_VALUES
    },
    {
        "Config InternetGatewayDevice Services FAPService CellConfig "
            "CarrierAggregation MACSchedParams",
        "InternetGatewayDevice.Services.FAPService.CellConfig."
            "X_VENDOR_CARRIER_AGGREGATION_PARAMS.MACSchedParams",
        "loadCalcWindowSize,loadDisparityThresh,MCSDiffThresh,"
        "PRBUsageWatermark,DLDeactCMDThresh,DLDeactMCSLowThresh,"
        "DLQLoadUPThresh,DLQLoadLowThresh,ActDeactTriggerCountThresh",
        OAM_SET_PARAMETER_VALUES
    },
    /* SPR 14295 FIX END */

/*LTE_OAM_INHANCEMENT_CA_CHANGED */

     /* eICIC_PHASE_1_2_CHANGES_START */
     /*SPR 15028 start*/
     {
         "Config InternetGatewayDevice Services FAPService CellConfig X_VENDOR_eICIC_PARAMS eICICParams",
         "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.eICICParams",
         "eICICProvisionType,eligible_ue,DlSinrThreshLowMark,DlSinrThreshHighMark",
         OAM_SET_PARAMETER_VALUES
     },

     {
         "Config InternetGatewayDevice Services FAPService CellConfig X_VENDOR_eICIC_PARAMS AbsInfoParams ABS_INFO",
         "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.AbsInfoParams.ABS_INFO",
         "NumAbsReport,AbsUsageLowThreshold,AbsUsageHighThreshold",
         OAM_SET_PARAMETER_VALUES
     },

     {
         "Config InternetGatewayDevice Services FAPService CellConfig X_VENDOR_eICIC_PARAMS AbsInfoParams ABS_PATTERN",
         "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.AbsInfoParams.ABS_PATTERN",
         "LowLoadAbsPattern,VictimAbsPattern,VictimMeasSubset",
         OAM_SET_PARAMETER_VALUES
     },


     {
         "Config InternetGatewayDevice Services FAPService CellConfig X_VENDOR_eICIC_PARAMS eICICTimers",
         "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.eICICTimers",
         "MacEicicMeasReportPeriodicity,AggressorSelTimer,LoadInformationGuardTimer,LoadInformationCollationTimer",
         OAM_SET_PARAMETER_VALUES
     },

     /* eICIC_PHASE_1_2_CHANGES_END */
     /*SPR 15028 end*/
	{
		"Config InternetGatewayDevice FAP Tunnel CryptoProfile",
		"InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1]",
		"Enable,IKERekeyLifetime,IPsecRekeyLifetimeTime,AuthMethod,DPDTimer,NATTKeepaliveTimer",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config InternetGatewayDevice FAP Tunnel VirtualInterface",
		"InternetGatewayDevice.FAP.Tunnel.VirtualInterface",
		"Enable,DSCPMarkPolicy",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE EPC-QoS[1]",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS.[1]",
		"QCI,Priority,Type,PacketDelayBudget",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE EPC-QoS[2]",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS.[2]",
		"QCI,Priority,Type,PacketDelayBudget",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE EPC-QoS[3]",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS.[3]",
		"QCI,Priority,Type,PacketDelayBudget",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE EPC-QoS[4]",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS.[4]",
		"QCI,Priority,Type,PacketDelayBudget",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE EPC-QoS[5]",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS.[5]",
		"QCI,Priority,Type,PacketDelayBudget",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE EPC-QoS[6]",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS.[6]",
		"QCI,Priority,Type,PacketDelayBudget",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE EPC-QoS[7]",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS.[7]",
		"QCI,Priority,Type,PacketDelayBudget",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE EPC-QoS[8]",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS.[8]",
		"QCI,Priority,Type,PacketDelayBudget",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE EPC-QoS[9]",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS.[9]",
		"QCI,Priority,Type,PacketDelayBudget",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"Config InternetGatewayDevice Services FAPService FAPControl LTE Gateway",
		"InternetGatewayDevice.Services.FAPService.FAPControl.LTE.Gateway",
		"SecGWServer1,SecGWServer2,SecGWServer3",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config InternetGatewayDevice FaultMgmt",
		"InternetGatewayDevice.FaultMgmt",
		"SupportedAlarmNumberOfEntries",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config InternetGatewayDevice ManagementServer",
		"InternetGatewayDevice.ManagementServer",
		"URL",
		OAM_SET_PARAMETER_VALUES
	},
/* SPR_12373_CHANGES */
/* Redundant Code Removed */
/* SPR_12373_CHANGES */
	{
		"Config InternetGatewayDevice Services FAPService FAPControl FAPControl",
		"InternetGatewayDevice.Services.FAPService.FAPControl",
		"SelfConfigEvents,EnclosureTamperingDetected",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility ConnMode EUTRA",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",
		"FilterCoefficientRSRP,FilterCoefficientRSRQ,A1ThresholdRSRP,A1ThresholdRSRQ,A2ThresholdRSRP,A2ThresholdRSRQ,A3Offset,ReportOnLeave,A4ThresholdRSRP,A4ThresholdRSRQ,A5Threshold1RSRP,A5Threshold1RSRQ,A5Threshold2RSRP,A5Threshold2RSRQ,Hysteresis,TimeToTrigger,TriggerQuantity,ReportQuantity,ReportInterval,ReportAmount",
		OAM_SET_PARAMETER_VALUES
	},

#ifdef OAM_SON_ENABLE /* FLAG_CHANGES */
	{
		"SON NR RETRIEVE NEIGHBOR_CELL EUTRAN",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborList.LTECell",
		"Retrieve_All",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"SON NR RETRIEVE NEIGHBOR_CELL UTRAN",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborList.InterRATCell.UMTS",
		"Retrieve_All",
		OAM_GET_PARAMETER_VALUES
	},
/* SPR-13586 START */
	{
		"SON NR RETRIEVE NEIGHBOR_CELL GERAN",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborList.InterRATCell.GSM",
		"Retrieve_All",
		OAM_GET_PARAMETER_VALUES
	},
/* SPR-13586 END */
	{
		"SON NR RETRIEVE NEIGHBOR_ENB",
		"eNodeBParams.oam_son_req.nbr_enb_list",
		"Retrieve_All",
		OAM_GET_PARAMETER_VALUES
	},
/* Start: SPR 10567 */
    {
        "SON NLIU RETRIEVE EUTRAN NEIGHBOR_CELL",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborListInUse.LTECell",
        "Retrieve_All",
        OAM_GET_PARAMETER_VALUES
    },
    {
        "SON NLIU RETRIEVE UTRAN NEIGHBOR_CELL",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborListInUse.InterRATCell.UMTS",
        "Retrieve_All",
        OAM_GET_PARAMETER_VALUES
    },
/*End: SPR 10567 */
/* SPR-13586 START */
    {
        "SON NLIU RETRIEVE GERAN NEIGHBOR_CELL",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborListInUse.InterRATCell.GSM",
        "Retrieve_All",
        OAM_GET_PARAMETER_VALUES
    },
/* SPR-13586 END */

#endif
	{
		"Show Config LoadReporting ServingCellReport",
		"LoadReporting.serving_cell",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config LoadReporting NeighborCellReport",
		"LoadReporting.neighbor_cell",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config LoadReporting OverLoadLevel",
		"LoadReporting.OverLoadLevel",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config LoadReporting HighLoadLevel",
		"LoadReporting.HighLoadLevel",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config LoadReporting MidLoadLevel",
		"LoadReporting.MidLoadLevel",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Version",
		"Show.Version",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE-TunnelNumberOfEntries",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE Tunnel",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.Tunnel",
		"",
		OAM_GET_PARAMETER_VALUES
	},

	{
		"Show Config InternetGatewayDevice DeviceSummary",
		"InternetGatewayDevice",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE eNBState",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService FAPControl FAPControl",
		"InternetGatewayDevice.Services.FAPService.FAPControl",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService FAPControl LTE eNBState",
		"InternetGatewayDevice.Services.FAPService.FAPControl.LTE",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService FAPService",
		"InternetGatewayDevice.Services.FAPService",
		"",
		OAM_GET_PARAMETER_VALUES
	},

	{
		"Show Config InternetGatewayDevice ManagementServer",
		"InternetGatewayDevice.ManagementServer",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE EPC-QoS[1]",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS.[1]",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE EPC-QoS[2]",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS.[2]",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE EPC-QoS[3]",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS.[3]",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE EPC-QoS[4]",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS.[4]",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE EPC-QoS[5]",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS.[5]",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE EPC-QoS[6]",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS.[6]",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE EPC-QoS[7]",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS.[7]",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE EPC-QoS[8]",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS.[8]",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE EPC-QoS[9]",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS.[9]",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService AccessMgmt LTE",
		"InternetGatewayDevice.Services.FAPService.AccessMgmt.LTE",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice DeviceInfo",
		"InternetGatewayDevice.DeviceInfo",
		"",
		OAM_GET_PARAMETER_VALUES
	},

/*LTE_OAM_ENHANCEMENT_CA_CHANGES */
    /* SPR 14295 FIX START */
    {
        "Show Config InternetGatewayDevice Services FAPService CellConfig "
            "CarrierAggregation GeneralParams",
        "InternetGatewayDevice.Services.FAPService.CellConfig."
            "X_VENDOR_CARRIER_AGGREGATION_PARAMS",
        "",
        OAM_GET_PARAMETER_VALUES
    },
    {
        "Show Config InternetGatewayDevice Services FAPService CellConfig "
            "CarrierAggregation MACSchedParams",
        "InternetGatewayDevice.Services.FAPService.CellConfig."
            "X_VENDOR_CARRIER_AGGREGATION_PARAMS.MACSchedParams",
        "",
        OAM_GET_PARAMETER_VALUES
    },
    /* SPR 14295 FIX END */
/*LTE_OAM_ENHANCEMENT_CA_CHANGES */

    /* eICIC_PHASE_1_2_CHANGES_START */
    /*SPR 15028 start*/
    {   
        "Show Config InternetGatewayDevice Services FAPService CellConfig X_VENDOR_eICIC_PARAMS eICICParams",
        "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.eICICParams",
        "",
        OAM_GET_PARAMETER_VALUES
    },
    {
        "Show Config InternetGatewayDevice Services FAPService CellConfig X_VENDOR_eICIC_PARAMS AbsInfoParams ABS_INFO",
        "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.AbsInfoParams.ABS_INFO",
        "",
        OAM_GET_PARAMETER_VALUES
    },
    {
        "Show Config InternetGatewayDevice Services FAPService CellConfig X_VENDOR_eICIC_PARAMS AbsInfoParams ABS_PATTERN",
        "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.AbsInfoParams.ABS_PATTERN",
        "",
        OAM_GET_PARAMETER_VALUES
    },

    {
        "Show Config InternetGatewayDevice Services FAPService CellConfig X_VENDOR_eICIC_PARAMS eICICTimers",
        "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.eICICTimers",
        "",
        OAM_GET_PARAMETER_VALUES
    },

    /* SPR 15028 end*/

    /* eICIC_PHASE_1_2_CHANGES_END */
	{
		"Show Config InternetGatewayDevice FAP PerfMgmt ConfigNumberOfEntries",
		"InternetGatewayDevice.FAP.PerfMgmt",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice FAP PerfMgmt Config",
		"InternetGatewayDevice.FAP.PerfMgmt.Config",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice FaultMgmt SupportedAlarmNumberOfEntries",
		"InternetGatewayDevice.FaultMgmt",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY SRS",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.SRS",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN S1AP",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.S1AP",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY ULPowerControl",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.ULPowerControl",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY AntennaInfo",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.AntennaInfo",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN CellRestriction",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.CellRestriction",
		"",
		OAM_GET_PARAMETER_VALUES
	},

    /* SPR 13970 FIX Start */
    {
        "Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN CellRestriction-AC_BARRING_FOR_MO_SIGNALLING",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.CellRestriction.X_VENDOR_ACCESS_BARRING_INFO.AC_BARRING_FOR_MO_SIGNALLING",
        "",
        OAM_GET_PARAMETER_VALUES
    },
    {
        "Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN CellRestriction-AC_BARRING_FOR_MO_DATA",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.CellRestriction.X_VENDOR_ACCESS_BARRING_INFO.AC_BARRING_FOR_MO_DATA",
        "",
        OAM_GET_PARAMETER_VALUES
    },
    {
        "Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN CellRestriction-SSAC_BARRING_FOR_MMTEL_VOICE_R9",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.CellRestriction.X_VENDOR_ACCESS_BARRING_INFO.SSAC_BARRING_FOR_MMTEL_VOICE_R9",
        "",
        OAM_GET_PARAMETER_VALUES
    },
    {
        "Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN CellRestriction-SSAC_BARRING_FOR_MMTEL_VIDEO_R9",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.CellRestriction.X_VENDOR_ACCESS_BARRING_INFO.SSAC_BARRING_FOR_MMTEL_VIDEO_R9",
        "",
        OAM_GET_PARAMETER_VALUES
    },
    {
        "Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN CellRestriction-AC_BARRING_FOR_CSFB_R10",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.CellRestriction.X_VENDOR_ACCESS_BARRING_INFO.AC_BARRING_FOR_CSFB_R10",
        "",
        OAM_GET_PARAMETER_VALUES
    },
    /* SPR 13970 FIX End */

	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY PUSCH-ULRS",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PUSCH.ULRS",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY PRACH",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PRACH",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY PUCCH",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PUCCH",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN RRCTimers",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RRCTimers",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY PUSCH",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PUSCH",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN MAC RACH",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.RACH",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN MAC DRX",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.DRX",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN S1AP",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.S1AP",
		"TRelocPrep,TRelocOverall",
		OAM_SET_PARAMETER_VALUES
	},
	{

		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY PDSCH",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PDSCH",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{

		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY PRS",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PRS",
		"",
		OAM_GET_PARAMETER_VALUES
	},
    /* EMBMS Changes Start */
    /* 7.0 OAM_SSI Fix Start*/
#ifdef LTE_EMBMS_SUPPORTED
    {
        "Show Config InternetGatewayDevice Services FAPService CellConfig LTE"
            " RAN PHY MBSFN",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE."
            "RAN.PHY.MBSFN",
        "",
        OAM_GET_PARAMETER_VALUES
    },
    {
        "MBMS Show Config SFConfigList",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE."
            "RAN.PHY.MBSFN.SFConfigList",
        "",
        OAM_GET_PARAMETER_VALUES
    },
    {
        "MBMS Show Config MBSFN_SERVICE_AREA_CONFIG",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE."
            "RAN.PHY.MBSFN.MBSFN_SERVICE_AREA_CONFIG",
        "",
        OAM_GET_PARAMETER_VALUES
    },
    {
        "MBMS Show Config MBSFN",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE."
            "RAN.PHY.MBSFN",
        "",
        OAM_GET_PARAMETER_VALUES
    },
    {
        "MBMS Show Config M2AP",
        "eNodeBParams.stack_config.L3Params.M2AP",
        "",
        OAM_GET_PARAMETER_VALUES
    },
    {
        "MBMS Show SessionList",
        "MBMS.Show.SessionList",
        "",
        OAM_GET_PARAMETER_VALUES
    },
    {
        "Show Log Level M2AP",
        "Show.Log.Level",
        "",
        OAM_GET_PARAMETER_VALUES
    },
#endif
    /* 7.0 OAM_SSI Fix End*/
    /* EMBMS Changes End */

	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN RF",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RF",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN RRCTimers",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RRCTimers",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Common",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Common",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility IdleMode Common",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.Common",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	/* START SPR_6719_FIX */
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig "
            "LTE RAN Mobility IdleMode IRAT GERAN",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.GERAN",
		"",
		OAM_GET_PARAMETER_VALUES
	},

	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig "
            "LTE RAN Mobility IdleMode IRAT GERAN-GERANFreqGroup",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.GERAN.GERANFreqGroup",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	/* END SPR_6719_FIX */
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility IdleMode IRAT UTRA TReselectionUTRA",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IRAT.UTRA",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility IdleMode IRAT UTRA TReselectionUTRASFHigh",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IRAT.UTRA",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility IdleMode IRAT UTRA TReselectionUTRASFMedium",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IRAT.UTRA",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility IdleMode IRAT UTRA UTRANFDDFreq",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IRAT.UTRA.UTRANFDDFreq",
		"",
		OAM_GET_PARAMETER_VALUES
	}, 
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility IdleMode IntraFreq",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IntraFreq",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility IdleMode InterFreq Carrier",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.InterFreq.Carrier",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility IdleMode InterFreq InterFreq",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.InterFreq",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE EPC",

		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN NeighborList LTECell",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborList.LTECell.[255]",
		"",
		OAM_GET_PARAMETER_VALUES
	}, 
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE EPC-PLMNList",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.PLMNList.[1]",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN RLC SRB1",

		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RLC.SRB1",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN RLC SRB2",

		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RLC.SRB2",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN IdleMode IRAT",

		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IRAT",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService AccessMgmt LTE",

		"InternetGatewayDevice.Services.FAPService.AccessMgmt.LTE",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{

		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN MAC DRX",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.DRX",
		"DRXEnabled,OnDurationTimer,DRXInactivityTimer,DRXRetransmissionTimer,LongDRXCycle,DRXStartOffset,ShortDRXCycle,DRXShortCycleTimer",
		OAM_SET_PARAMETER_VALUES
	},
	{ 
		"Config Kpi Periodic-Start",
		"InternetGatewayDevice.FAP.PerfMgmt.Config.Start",
		"X_VENDOR_SamplingInterval,PeriodicUploadTime,X_VENDOR_FileWriteTimer,PeriodicUploadInterval",  /* SPR 8942 PM_FIX */
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config Kpi Periodic-Stop",
		"InternetGatewayDevice.FAP.PerfMgmt.Config.Stop",
		"",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config Kpi On-demand",
		"InternetGatewayDevice.FAP.PerfMgmt.Ondemand",
		"",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"Config nmsFTPServerConfig",
		"InternetGatewayDevice.FTPParams",
		"enable,FTPServerurl,FTPServerUserName,FTPServerpassword,FilePath",
		OAM_SET_PARAMETER_VALUES
	},

	{
		"eNB Download" ,
		"eNB.Download",
		"sw_package_name",
		OAM_SET_PARAMETER_VALUES
	}, 
	{
		"eNB Switch" ,
		"eNB.Switch",
		"sw_package_name",
		OAM_SET_PARAMETER_VALUES
	},  
	{ 
		"eNB Archive",
		"Logs.Archive",
		"LayerLogs",
		OAM_SET_PARAMETER_VALUES
	},

#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
	{
		"RRH System ResetRRH",
		"RRH.ControlCommand.Reset",
		"Unit",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"RRH System ResetCarrier",
		"RRH.ControlCommand.Reset",
		"Unit",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"RRH System Upgrade",
		"RRH.ControlCommand.Upgrade",
		"FTPAddr,FTPUsrName,FTPPasswd,ImageToDwnld,ActivationFlag",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"RRH System ActivatePassiveSW",
		"RRH.ControlCommand.ActivatePassiveSW",
		"ActivationFlag",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"RRH System VSWRMonitoring Disable",
		"RRH.ControlCommand.VSWRAlarms",
		"EnableFlag,AntennaConnIndex,RFPath",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"RRH System VSWRMonitoring Enable",
		"RRH.ControlCommand.VSWRAlarms",
		"EnableFlag,AntennaConnIndex,RFPath,Thresh1Value,Thresh2Value,TxPwrFloor",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"RRH System UnlatchAlarms",
		"RRH.ControlCommand.UnlatchAlarms",
		"AlarmID",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"RRH System PowerReports Enable",
		"RRH.ControlCommand.PowerReports",
		"EnableFlag,RecordsCount",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"RRH System PowerReports Disable",
		"RRH.ControlCommand.PowerReports",
		"EnableFlag",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"RRH Reconfig ChangeCarrState",
		"RRH.Config",
		"state",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"RRH Reconfig Params",
		"RRH.Config",
		"tx_Frequency,rx_Frequency,carrier_Transmit_Power",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"RRH Show Config",
		"RRH.Config",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"RRH Show RRHStatus",
		"RRH.Status",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"RRH Show Inventory",
		"RRH.Inventory",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"RRH Show Capabilities",
		"RRH.Capabilities",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"RRH Show Attributes",
		"RRH.Attributes",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"RRH Show RRHStatus",
		"RRH.RRHStatus",
		"",
		OAM_GET_PARAMETER_VALUES
	},
#endif
#endif

	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility ConnMode EUTRA",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility ConnMode IRAT",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",
		"",
		OAM_GET_PARAMETER_VALUES
	},
    /* SPR 13020 START */
	{
		"Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility ConnMode IRAT",
		"InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",
		"QoffsettUTRA,FilterCoefficientUTRA,MeasQuantityUTRAFDD,B1ThresholdUTRARSCP,B1ThresholdUTRAEcN0,QoffsetGERAN,FilterCoefficientGERAN,B1ThresholdGERAN,QoffsetCDMA2000,MeasQuantityCDMA2000,B1ThresholdCDMA2000,B2Threshold2UTRARSCP,B2Threshold2UTRAEcN0,B2Threshold2GERAN,B2Threshold2CDMA2000,Hysteresis,TimeToTrigger,ReportInterval,ReportAmount,B2Threshold1RSRP,B2Threshold1RSRQ",
		OAM_SET_PARAMETER_VALUES
	},
    /* SPR 13020 END */
	{
		"Show Alarm CurrentAlarm",
		"InternetGatewayDevice.FaultMgmt.CurrentAlarm.",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Alarm HistoryEvent",
		"InternetGatewayDevice.FaultMgmt.HistoryEvent.",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Alarm ExpeditedEvent",
		"InternetGatewayDevice.FaultMgmt.ExpeditedEvent.",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Alarm QueuedEvent",
		"InternetGatewayDevice.FaultMgmt.QueuedEvent.",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Clean AlarmHistory",
		"Clean",
		"AlarmHistory",
		OAM_SET_PARAMETER_VALUES
	},
#ifdef LTE_EMBMS_SUPPORTED
    {
        "Show Stats L3 M2AP",
        "Show.Stats.L3.M2AP",
        "",
        OAM_GET_PARAMETER_VALUES
    },
#endif
	{
		"Show Stats L2 MAC",
		"Show.Stats.L2.MAC",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Stats L2 RLC",
		"Show.Stats.L2.RLC",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Stats L2 PDCP",
		"Show.Stats.L2.PDCP",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Stats L2 EGTPU",
		"Show.Stats.L2.EGTPU",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Stats L3 S1AP",
		"Show.Stats.L3.S1AP",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show KPI L2 MAC",
		"Show.KPi.L2.MAC",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show KPI L2 PDCP",
		"Show.KPI.L2.PDCP",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show KPI L3 RRC",
		"Show.KPI.L3.RRC",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	/*TR069 Changes*/
	{
		"Show Config InternetGatewayDevice Services FAPService FAPControl LTE Gateway",
		"InternetGatewayDevice.Services.FAPService.FAPControl.LTE.Gateway",
		"",
		OAM_GET_PARAMETER_VALUES
	},

	{
		"Show Config InternetGatewayDevice Services FAPService Capabilities SupportedSystems",
		"InternetGatewayDevice.Services.FAPService.Capabilities",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService Capabilities Beacon",
		"InternetGatewayDevice.Services.FAPService.Capabilities",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService Capabilities GPSEquipped",
		"InternetGatewayDevice.Services.FAPService.Capabilities",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"Show Config InternetGatewayDevice Services FAPService Capabilities MaxTxPower",
		"InternetGatewayDevice.Services.FAPService.Capabilities",
		"",
		OAM_GET_PARAMETER_VALUES
	},  
	{
		"Show Config InternetGatewayDevice Services FAPService Capabilities LTE",
		"InternetGatewayDevice.Services.FAPService.Capabilities.LTE",
		"",
		OAM_GET_PARAMETER_VALUES
	},

	/*SON_ENERGY SAVING*/
	{
		"SON ES Enable",
		"Set.SON.ES.Enable",
		"EnergySavingControl,ActiveUECountIntervalSecond,ActiveUECountThreshold",
		OAM_SET_PARAMETER_VALUES 
	},
	{
		"SON ES Disable",
		"Set.SON.ES.Disable",
		"EnergySavingControl",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON ES Modify UE_Count_interval",
		"Set.SON.ES.Modify.UE_Count_interval",
		"ActiveUECountIntervalSecond",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON ES Modify UE_Count_threshold",
		"Set.SON.ES.Modify.UE_Count_threshold",
		"ActiveUECountThreshold",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON ES Cell SwitchOn",
		"Set.SON.ES.Cell.SwitchOn",
		"CellSwitchStatus",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON ES Cell SwitchOff",
		"Set.SON.ES.Cell.SwitchOff",
		"CellSwitchStatus,CellSwitchOffMode",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON ES Cell Autonomous_SwitchOff TriggerCriteria LoadLevel",
		"Set.SON.ES.Cell.Autonomous_SwitchOff.TriggerCriteria.LoadLevel",
		"TimerDuration,LoadLevelThreshold",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON ES Cell Autonomous_SwitchOff TriggerCriteria ActiveUECount",
		"Set.SON.ES.Cell.Autonomous_SwitchOff.TriggerCriteria.ActiveUECount",
		"TimerDuration",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON ES Cell Autonomous_SwitchOff TriggerCriteria Both",
		"Set.SON.ES.Cell.Autonomous_SwitchOff.TriggerCriteria.Both",
		"TimerDuration,LoadLevelThreshold",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON ES Retrieve Switched_off_cells",
		"SON.ES.Retrieve.Switched_off_cells",
		"Retrieve_All",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"SON MRO Enable",
		SON_MRO_ENABLE,
		"",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON MRO Disable",
		SON_MRO_DISABLE,
		"",
		OAM_SET_PARAMETER_VALUES
	},
	{
	"SON MRO MODIFY CONFIG_PARAMS",
	SON_MRO_MODIFY_CONFIG_PARAM,
	"Mro_Mode,Mro_Param,Mro_Ho_Failure_Accumulation_Duration,Delta_Cio,Mro_Ho_Attempts_Threshold,Mro_Ho_Optimization_Start_Threshold,Mro_Ho_Optimization_Stop_Threshold,Mro_Ho_Fail_Rate_For_Optimization,TTT,Delta_ttt",
	OAM_SET_PARAMETER_VALUES
    },
    {
		"SON MLB Enable",
		SON_MLB_ENABLE,
		"",
		OAM_SET_PARAMETER_VALUES
	},
	{
		"SON MLB Disable",
		SON_MLB_DISABLE,
		"",
		OAM_SET_PARAMETER_VALUES
    },
    {
        "SON MLB MODIFY PARAMS",
        SON_MLB_MODIFY_ATTR,
        /* SPR 11519 CHANGE START */
        "Mlb_Mode,Q-Offset-Min,Q-Offset-Max",
        /* SPR 11519 CHANGE END */
        OAM_SET_PARAMETER_VALUES
    },
    {
        "Set Cell_Context Context",
        "Cell_Context.Context",
        /* SPR 11249 FIX START */
        /* Changed from ActiveCellIndex to CurrentCellIndex */
        "CurrentCellIndex",
        /* SPR 11249 FIX END */
        OAM_SET_PARAMETER_VALUES
    },
    /* SPR 13501 FIX START */
    {
        "Config InternetGatewayDevice Services FAPService CellConfig LTE OperatorInfo DynamicICICParams DLResourcePartitionInfo",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.OperatorInfo.DynamicICICParams.DLResourcePartitionInfo",
        "NumOfCellEdgeRegion,NumOfCellCenterRegion,start_rb_cell_center,end_rb_cell_center,start_rb_cell_edge,end_rb_cell_edge",
        OAM_SET_PARAMETER_VALUES
    },
    {
        "Config InternetGatewayDevice Services FAPService CellConfig LTE OperatorInfo DynamicICICParams ULResourcePartitionInfo",
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.OperatorInfo.DynamicICICParams.ULResourcePartitionInfo",
        "NumOfCellEdgeRegion,NumOfCellCenterRegion,start_rb_cell_center,end_rb_cell_center,start_rb_cell_edge,end_rb_cell_edge",
        OAM_SET_PARAMETER_VALUES
    /* SPR 13501 FIX END */
	/* SPR_19619 start*/
    },

	/* FLAG_CHANGES - Code Removed Here */
	{
		"SON PEER_CELL_ACTIVATION",
		"SON.PEER_CELL_ACTIVATION",
		"PLMNID,EUTRANCID",
		OAM_SET_PARAMETER_VALUES

/* SPR_19279_START */
    },
	/* SPR_19619 stop*/
    /* SPR 13501 FIX END */
    {
        "Config InternetGatewayDevice Services FAPService CellConfig LTE EPC PLMNList ADD INFO",
        "InternetGatewayDevice.Services.FAPService.CellConfig."
            "LTE.EPC.PLMNList",
        "Enable,IsPrimary,PLMNID,CellReservedForOperatorUse",
        (OAM_SET_PARAMETER_VALUES) | (OAM_ADD_OBJECT_REQ)
    },
    {
        "Config InternetGatewayDevice Services FAPService CellConfig LTE EPC PLMNList UPDATE INFO",
        "InternetGatewayDevice.Services.FAPService.CellConfig."
            "LTE.EPC.PLMNList",
        "Enable,IsPrimary,PLMNID,CellReservedForOperatorUse",
        OAM_SET_PARAMETER_VALUES
    },
    {
        "Config InternetGatewayDevice Services FAPService CellConfig LTE EPC PLMNList DELETE INFO",
        "InternetGatewayDevice.Services.FAPService.CellConfig."
            "LTE.EPC.PLMNList",
        "PLMNID",
        OAM_DELETE_OBJECT_REQ
    },
    {
        "Config InternetGatewayDevice Services FAPService CellConfig LTE EPC PLMNList RETRIEVE INFO",
        "InternetGatewayDevice.Services.FAPService.CellConfig."
            "LTE.EPC.PLMNList",
         /* spr_21013_fix_end */
        "Retrieve_All",
        OAM_GET_PARAMETER_VALUES
    },
/* SPR_19279_END */
/* spr 22474 start*/
	{
		"DEBUG GET L2 MAC",
		"Debug.Get.L2.MAC",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"DEBUG GET L2 RLC",
		"Debug.Get.L2.RLC",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"DEBUG GET L2 PDCP",
		"Debug.Get.L2.PDCP",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"DEBUG GET L2 EGTPU",
		"Debug.Get.L2.EGTPU",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"DEBUG GET L3 S1AP",
		"Debug.Get.L3.S1AP",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"DEBUG GET L3 X2AP",
		"Debug.Get.L3.X2AP",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"DEBUG GET L3 RRC",
		"Debug.Get.L3.RRC",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"DEBUG GET RRM",
		"Debug.Get.RRM",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"DEBUG GET SON",
		"Debug.Get.SON",
		"",
		OAM_GET_PARAMETER_VALUES
	},
	{
		"DEBUG GET OAM OAM",
		"Debug.Get.OAM.OAM",
		"",
		OAM_GET_PARAMETER_VALUES
    },
	{
		"DEBUG GET OAM TR069",
		"Debug.Get.OAM.TR069",
		"",
		OAM_GET_PARAMETER_VALUES
    }
/* spr 22474 end*/


};
mess_send_t message_send;

#ifdef OAM_SON_ENABLE
Char8* oam_qcli_send_nr_enb_cmd(int C, char *const *V,
		const char *const *E,
		const char *const *T, void *arg)
{
	UInt8 size = OAM_ZERO;
	UInt8 loop = OAM_ZERO;
	oam_error_code_et error_code = NO_ERROR;
	char *v[16] = {OAM_NULL};

    ++C;

	/* Copy command in first index */
	size = oam_strlen(V[0]) + 1;
	if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[0], &error_code)) {
		oam_fprintf(stderr, "Memory allocation failed!!!");
		return OAM_NULL;
	}
	oam_strcpy(v[0], V[0]);

	/* Copy action parameter in second index */ 
	size = 2;
	if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[1], &error_code)) {
		oam_fprintf(stderr, "Memory allocation failed!!!");
		return OAM_NULL;
	}

	if(strstr(V[0], "ADD")) {
		oam_strcpy(v[1], "1");
		if(!oam_strcmp(V[4], OAM_CLI_DEFAULT_PARAM_VALUE) &&
				!oam_strcmp(V[5], OAM_CLI_DEFAULT_PARAM_VALUE) &&
				!oam_strcmp(V[6], OAM_CLI_DEFAULT_PARAM_VALUE)) {
			oam_fprintf(stderr, "---------------------------------------------------------------");
			oam_fprintf(stderr, "\nValues for at least one of TAC, ENB_TNL_ADDRESSes must be given");
			oam_fprintf(stderr, "\n---------------------------------------------------------------\n");
			return NULL;
		}
		else if(!oam_strcmp(V[7], OAM_CLI_DEFAULT_PARAM_VALUE) ||
				!oam_strcmp(V[8], OAM_CLI_DEFAULT_PARAM_VALUE)) {
            /* SPR_11780_FIX Start */
			oam_fprintf(stderr, "-----------------------------------------------------------");
			oam_fprintf(stderr, "\nValues for X2_HO_STATUS and X2_CONNECTION_STATUS must be given");
			oam_fprintf(stderr, "\n-----------------------------------------------------------\n");
            /* SPR_11780_FIX End */
			return NULL;
		}
	}
	else if(strstr(V[0], "UPDATE")) {
		oam_strcpy(v[1], "2");
	}
	else if(strstr(V[0], "DELETE")) {
		oam_strcpy(v[1], "3");
	}

	/* Copy all other parameters as such */ 
	for(loop = OAM_TWO; loop < C; ++loop) {
		size = oam_strlen(V[loop - OAM_ONE]) + 1;
		if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[loop], &error_code)) {
			oam_fprintf(stderr, "Memory allocation failed!!!");
			return OAM_NULL;
		}
		oam_strcpy(v[loop], V[loop - OAM_ONE]);
	}

	return oam_qcli_send_command(C, v, E, T, arg);
}


/*******************************************************************************
 * Function Name  : oam_qcli_send_son_nr_add_cell_command
 * Description    : Function to send add object req
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : Char8*
 *******************************************************************************/
Char8* oam_qcli_send_son_nr_add_cell_command(int C, char *const *V,
		const char *const *E,
		const char *const *T,
		void *arg)
{
    oam_return_et retVal = OAM_FAILURE;

	/* RT SPR 8627 FIX START */
	if(strstr(V[0], "ADD UTRAN")) {
		/* Check whether the value of CID and value of RNCID coform to the
		 * restriction.
		 * V[3]: value of CID
		 * V[4]: value of RNCID
		 */
		if(oam_atoi(V[2]) > 4095 && oam_atoi(V[3]) > 4095) {
            oam_fprintf(stderr, "-------------------------------------------"
                    "-----------------------------------\n");
            oam_fprintf(stderr, "ERROR: The values of CID and RNCID put "
                    "together must not excceed 28-bit in size\n");
            oam_fprintf(stderr, "-------------------------------------------"
                    "------------------------------------\n");
            return OAM_NULL;
        }
    }
    /* SPR-13586 START */
    else if(oam_strstr(V[0], "ADD GERAN")) {
        if(0xFFFE == oam_atoi(V[2])) {
            oam_fprintf(stderr, "----------------------------------");
            oam_fprintf(stderr, "ERROR: LAC value 65534 not allowed");
            oam_fprintf(stderr, "----------------------------------");
            return NULL;
        }
    /* SPR-13586 END */
    }


	/* RT SPR 8627 FIX END */
    retVal = oam_qcli_prepare_add_object_cmd_buffer(C, V);

    if (OAM_FAILURE == retVal) {
        return OAM_NULL;
    }

	return oam_qcli_send_command(C, V, E, T, arg);
}
#endif

/* SPR_19279_START */
/*******************************************************************************
 * Function Name  : oam_qcli_send_epc_plmnlist_add_cell_command
 * Description    : Function to send add object req
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : Char8*
 *******************************************************************************/
Char8* oam_qcli_send_epc_plmnlist_add_cell_command(int C, char *const *V,
		const char *const *E,
		const char *const *T,
		void *arg)
{
    oam_return_et retVal = OAM_FAILURE;

    retVal = oam_qcli_prepare_add_object_cmd_buffer(C, V);

    if (OAM_FAILURE == retVal) {
        return OAM_NULL;
    }

	return oam_qcli_send_command(C, V, E, T, arg);
}
/* SPR_19279_END */

/*******************************************************************************
 * Function Name  : oam_set_log_cat_l2_mac_all
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_set_log_cat_l2_mac_all
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_set_log_cat_l2_mac_all_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_l2_sys_fail
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_l2_sys_fail
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_l2_sys_fail_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_mac_l1_inf
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_l1_inf
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_l1_inf_pkt(C,V);
	return OAM_ZERO;
}


/*******************************************************************************
 * Function Name  : oam_mac_ul_harq
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_ul_harq
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_ul_harq_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_mac_dl_harq
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_dl_harq
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_dl_harq_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_mac_rach
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_rach
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_rach_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_mac_rrc_inf
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_rrc_inf
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_rrc_inf_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_mac_oam_inf
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_oam_inf
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_oam_inf_pkt(C,V);
	return OAM_ZERO;
}


/*CSR:00055317_FIX_START*/

/*******************************************************************************
 * Function Name  : oam_mac_rrm_inf
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 ********************************************************************************/
static
char *oam_mac_rrm_inf
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);
    /* SPR 17777 FIX END */

    message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
    message_send.message_send = 1;
    oam_mac_rrm_inf_pkt(C,V);
    return OAM_ZERO;
}
/*CSR:00055317_FIX_END*/


/*******************************************************************************
 * Function Name  : oam_mac_dl_pc
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_dl_pc
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_dl_pc_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_mac_ul_pc
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_ul_pc
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_ul_pc_pkt(C,V);
	return OAM_ZERO;
}

/* SPR 55317 START */
/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_dl_strategy
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_dl_strategy_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_ul_strategy
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_ul_strategy_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_pdsch
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_pdsch_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_pdcch
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_pdcch_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_pusch
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_pusch_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_pucch
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_pucch_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_bcch
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_bcch_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_pcch
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_pcch_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_ccch
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_ccch_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_cqi
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_cqi_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_srs
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_srs_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_tb_ul
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_tb_ul_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_tb_dl
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_tb_dl_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_drx
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_drx_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_meas_gap
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_meas_gap_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_meas_handler
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_meas_handler_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_ul_csi
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_ul_csi_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_mux
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_mux_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_dl_sps_strategy
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_dl_sps_strategy_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_ul_sps_strategy
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_ul_sps_strategy_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_scheduler
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_scheduler_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_atb
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_atb_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_mac_ta
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_mac_ta_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_rlc_log_cat_enable_all
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_rlc_log_cat_enable_all_pkt(C,V);
	return OAM_ZERO;
}
/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_rlc_am
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_rlc_am_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_rlc_um
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_rlc_um_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_rlc_tm
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_rlc_tm_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_rlc_tx
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_rlc_tx_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_rlc_rx
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_rlc_rx_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_rlc_mac
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_rlc_mac_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_rlc_pdcp
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_rlc_pdcp_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_rlc_rrc
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_rlc_rrc_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_rlc_oam
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_rlc_oam_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_rlc_timer
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_rlc_timer_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_pdcp_enable_log_cat_all
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_pdcp_enable_log_cat_all_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_pdcp_rrcoam
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_pdcp_rrcoam_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_pdcp_tx
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_pdcp_tx_pkt(C,V);
	return OAM_ZERO;
}


/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_pdcp_rx
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_pdcp_rx_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : 
 * Description    : To set the desired log category.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_pdcp_rrm_int
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_pdcp_rrm_int_pkt(C,V);
	return OAM_ZERO;
}
/* SPR 55317 End */

char *oam_rlc_errorlog_all
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	oam_rlc_errorlog_all_pkt(C,V);
	return OAM_ZERO;
}


static
char *oam_l2_log_lvl
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;

	/* coverity 41138 */
	setloglevell2maccommands oam_mac_et   = MAC_CRITICALLOG;
	setloglevell2rlccommands oam_rlc_et   = RLC_CRITICALLOG;
	setloglevell2pdcpcommands oam_pdcp_et = PDCP_CRITICALLOG;
	setloglevell2gtpucommands oam_gtpu_et = GTPU_CRITICALLOG;

	if(oam_strstr(*V,"Critical"))
	{
		oam_mac_et   = MAC_CRITICALLOG;
        oam_rlc_et   = RLC_CRITICALLOG;
        oam_pdcp_et  = PDCP_CRITICALLOG;
        oam_gtpu_et  = GTPU_CRITICALLOG;
	}
	else if(oam_strstr(*V,"Error"))
	{
		oam_mac_et   = MAC_ERRORLOG;
        oam_rlc_et   = RLC_ERRORLOG;
        oam_pdcp_et  = PDCP_ERRORLOG;
        oam_gtpu_et  = GTPU_ERRORLOG;
	}
	else if(oam_strstr(*V,"Warning"))
	{
		oam_mac_et   = MAC_WARNINGLOG;
        oam_rlc_et   = RLC_WARNINGLOG;
        oam_pdcp_et  = PDCP_WARNINGLOG;
        oam_gtpu_et  = GTPU_WARNINGLOG;
	}
	else if(oam_strstr(*V,"Info"))
	{
		oam_mac_et   = MAC_INFOLOG;
        oam_rlc_et   = RLC_INFOLOG;
        oam_pdcp_et  = PDCP_INFOLOG;
        oam_gtpu_et  = GTPU_INFOLOG;
	}
	else if(oam_strstr(*V,"Brief"))
	{
		oam_mac_et   = MAC_BRIEFLOG;
        oam_rlc_et   = RLC_BRIEFLOG;
        oam_pdcp_et  = PDCP_BRIEFLOG;
        oam_gtpu_et  = GTPU_BRIEFLOG;
	}
    else if(oam_strstr(*V,"DetailedAll"))
    {
		oam_mac_et   = MAC_DETAILEDTTILOG;
        oam_rlc_et   = RLC_DETAILEDALLLOG;
        oam_pdcp_et  = PDCP_DETAILEDALLLOG;
        oam_gtpu_et  = GTPU_DETAILEDALLLOG;
    }
	else if(oam_strstr(*V,"Detailed"))
	{
		oam_mac_et   = MAC_DETAILEDLOG;
        oam_rlc_et   = RLC_DETAILEDLOG;
        oam_pdcp_et  = PDCP_DETAILEDLOG;
        oam_gtpu_et  = GTPU_DETAILEDLOG;
	}
	oam_mac_log_level_pkt(C,oam_mac_et);
	oam_rlc_log_level_pkt(C,oam_rlc_et);
	oam_pdcp_log_level_pkt(C,oam_pdcp_et);
	oam_egtpu_log_level_pkt(C,oam_gtpu_et);
	return OAM_ZERO;
}


static
char *oam_mac_log_lvl
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;

	setloglevell2maccommands oam_l2_et = MAC_CRITICALLOG;

	if(oam_strstr(*V,"Critical"))
	{
		oam_l2_et = MAC_CRITICALLOG;
	}
	else if(oam_strstr(*V,"Error"))
	{
		oam_l2_et = MAC_ERRORLOG;
	}
	else if(oam_strstr(*V,"Warning"))
	{
		oam_l2_et = MAC_WARNINGLOG;
	}
	else if(oam_strstr(*V,"Info"))
	{
		oam_l2_et = MAC_INFOLOG;
	}
	else if(oam_strstr(*V,"Brief"))
	{
		oam_l2_et = MAC_BRIEFLOG;
	}
    else if(oam_strstr(*V,"DetailedAll"))
    {
		oam_l2_et = MAC_DETAILEDTTILOG;
    }
	else if(oam_strstr(*V,"Detailed"))
	{
		oam_l2_et = MAC_DETAILEDLOG;
	}
	oam_mac_log_level_pkt(C,oam_l2_et);
	return OAM_ZERO;
}


static
char *oam_rlc_log_lvl
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;

	setloglevell2rlccommands oam_l2_et = RLC_CRITICALLOG;

	if(oam_strstr(*V,"Critical"))
	{
		oam_l2_et = RLC_CRITICALLOG;
	}
	else if(oam_strstr(*V,"Error"))
	{
		oam_l2_et = RLC_ERRORLOG;
	}
	else if(oam_strstr(*V,"Warning"))
	{
		oam_l2_et = RLC_WARNINGLOG;
	}
	else if(oam_strstr(*V,"Info"))
	{
		oam_l2_et = RLC_INFOLOG;
	}
	else if(oam_strstr(*V,"Brief"))
	{
		oam_l2_et = RLC_BRIEFLOG;
	}
    else if(oam_strstr(*V,"DetailedAll"))
    {
		oam_l2_et = RLC_DETAILEDALLLOG;
    }
	else if(oam_strstr(*V,"Detailed"))
	{
		oam_l2_et = RLC_DETAILEDLOG;
	}
	oam_rlc_log_level_pkt(C,oam_l2_et);
	return OAM_ZERO;
}


static
char *oam_pdcp_log_lvl
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;

	setloglevell2pdcpcommands oam_l2_et = PDCP_CRITICALLOG;

	if(oam_strstr(*V,"Critical"))
	{
		oam_l2_et = PDCP_CRITICALLOG;
	}
	else if(oam_strstr(*V,"Error"))
	{
		oam_l2_et = PDCP_ERRORLOG;
	}
	else if(oam_strstr(*V,"Warning"))
	{
		oam_l2_et = PDCP_WARNINGLOG;
	}
	else if(oam_strstr(*V,"Info"))
	{
		oam_l2_et = PDCP_INFOLOG;
	}
	else if(oam_strstr(*V,"Brief"))
	{
		oam_l2_et = PDCP_BRIEFLOG;
	}
    else if(oam_strstr(*V,"DetailedAll"))
    {
		oam_l2_et = PDCP_DETAILEDALLLOG;
    }
	else if(oam_strstr(*V,"Detailed"))
	{
		oam_l2_et = PDCP_DETAILEDLOG;
	}
	oam_pdcp_log_level_pkt(C,oam_l2_et);
	return OAM_ZERO;
}


static
char *oam_egtpu_log_lvl
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;

	setloglevell2gtpucommands oam_l2_et = GTPU_CRITICALLOG;

	if(oam_strstr(*V,"Critical"))
	{
		oam_l2_et = GTPU_CRITICALLOG;
	}
	else if(oam_strstr(*V,"Error"))
	{
		oam_l2_et = GTPU_ERRORLOG;
	}
	else if(oam_strstr(*V,"Warning"))
	{
		oam_l2_et = GTPU_WARNINGLOG;
	}
	else if(oam_strstr(*V,"Info"))
	{
		oam_l2_et = GTPU_INFOLOG;
	}
	else if(oam_strstr(*V,"Brief"))
	{
		oam_l2_et = GTPU_BRIEFLOG;
	}
    else if(oam_strstr(*V,"DetailedAll"))
    {
		oam_l2_et = GTPU_DETAILEDALLLOG;
    }
	else if(oam_strstr(*V,"Detailed"))
	{
		oam_l2_et = GTPU_DETAILEDLOG;
	}
	oam_egtpu_log_level_pkt(C,oam_l2_et);
	return OAM_ZERO;
}


#ifdef OAM_SON_ENABLE
/*******************************************************************************
 * Function Name  : oam_es_enable
 * Description    : To Enable ES Request of SON.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_es_enable
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */
	UInt8 size = oam_strlen(V[0]) + 1;
	oam_error_code_et error_code = NO_ERROR;
	UInt32 count = OAM_ZERO;
	char *v[5] = {OAM_NULL};

	if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[0], &error_code)) {
		oam_fprintf(stderr,"Memory allocation failed!!!");
		return " ";
	}

	/*Coverty fix*/
	oam_strncpy(v[0], V[0],size);
	count++;

	if(OAM_FAILURE == oam_mem_alloc(2, (void**)&v[1], &error_code)) {
		oam_fprintf(stderr, "Memory allocation failed!!!");
		return " ";
	}
	/*Coverty fix*/
	oam_strncpy(v[1],"1",sizeof("1") + OAM_ONE);/*value 1 for energy saving off*/
	count++;

	size = oam_strlen(V[1]) + 1;   
	if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[2], &error_code)) {
		oam_fprintf(stderr, "Memory allocation failed!!!");
		return " ";
	}

	/*Coverty fix*/
	oam_strncpy(v[2],V[1],size);/*UE active count interval sec*/
	count++;

	size = oam_strlen(V[2]) + 1;
	if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[3], &error_code)) {
		oam_fprintf(stderr, "Memory allocation failed!!!");
		return " ";
	}

	/*Coverty fix*/

	oam_strncpy(v[3],V[2],size);/*threshold value*/
	count++;

	return oam_qcli_send_command(count, v, E, T, arg);
}

/*******************************************************************************
 * Function Name  : oam_es_disable
 * Description    : To Disable ES Request of SON.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_es_disable
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */
	UInt8 size = oam_strlen(V[0]) + 1;
	oam_error_code_et error_code = NO_ERROR;
	UInt32 count = OAM_ZERO;
	char *v[3] = {OAM_NULL};

	if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[0], &error_code)) {
		oam_fprintf(stderr,"Memory allocation failed!!!");
		return " ";
	}

	/*Coverty fix*/
	oam_strncpy(v[0], V[0],size);
	count++;

	if(OAM_FAILURE == oam_mem_alloc(2, (void**)&v[1], &error_code)) {
		oam_fprintf(stderr, "Memory allocation failed!!!");
		return " ";
	}

	/*Coverty fix*/
	oam_strncpy(v[1],"0",sizeof("0") + OAM_ONE);/*value 0 for energy saving off*/
	/*value 0 for energy saving off*/
	count++;

	return oam_qcli_send_command(count, v, E, T, arg);
}

/*******************************************************************************
 * Function Name  : oam_switch_on
 * Description    : To Enable cell switch on request of ES of SON.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_switch_on
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */
	UInt8 size = oam_strlen(V[0]) + 1;
	oam_error_code_et error_code = NO_ERROR;
	UInt32 count = OAM_ZERO;
	char *v[3] = {OAM_NULL};

	if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[0], &error_code)) {
		oam_fprintf(stderr,"Memory allocation failed!!!");
		return " ";
	}
	/*Coverty fix*/
	oam_strncpy(v[0], V[0], size);
	count++;

	if(OAM_FAILURE == oam_mem_alloc(2, (void**)&v[1], &error_code)) {
		oam_fprintf(stderr, "Memory allocation failed!!!");
		return " ";
	}

	/*Coverty fix*/
	oam_strncpy(v[1],"1",sizeof("1") + OAM_ONE);/*value 1 for energy saving cell switch on*/
	count++;

	return oam_qcli_send_command(count, v, E, T, arg);

}
#endif
/*******************************************************************************
 * Function Name  : oam_rrm_disable_rac_ecn
 * Description    : To Enable cell switch on request of ES of SON.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_rrm_disable_rac_ecn
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */
	UInt8 size = oam_strlen(V[0]) + 1;
	oam_error_code_et error_code = NO_ERROR;
	UInt32 count = OAM_ZERO;
	char *v[3] = {OAM_NULL};


	if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[0], &error_code)) {
		oam_fprintf(stderr,"Memory allocation failed!!!");
		return " ";
	}

	oam_strncpy(v[0], V[0], size);
	count++;



	if(OAM_FAILURE == oam_mem_alloc(3, (void**)&v[1], &error_code)) {
		oam_fprintf(stderr, "Memory allocation failed!!!");
		return " ";
	}
	oam_strncpy(v[1],"-1",sizeof("-1") + OAM_ONE);
	count++;

	return oam_qcli_send_command(count, v, E, T, arg);

}



#ifdef OAM_SON_ENABLE
/*******************************************************************************
 * Function Name  : oam_switch_off
 * Description    : To serve cell switch off request of ES of SON.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
Char8 *oam_switch_off
(
 SInt32 C,
 Char8 *const *V,
 const Char8 *const *E,
 const Char8 *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */
	UInt8 size = oam_strlen(V[0]) + 1;
	oam_error_code_et error_code = NO_ERROR;
	UInt32 count = OAM_ZERO;
	UInt32 i = OAM_ZERO;
    /* SPR 12727 REVIEW COMMENT FIX START */
    Char8* retVal = OAM_NULL;
    /* SPR 12727 FIX START */
	char *v[4] = {OAM_NULL};
    /* SPR 12727 FIX End */
	if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[0], &error_code)) {
		oam_fprintf(stderr,"Memory allocation failed!!!");
		return " ";
	}

	/*Coverty fix*/
	oam_strncpy(v[0], V[0], size);

	count++;

	if(OAM_FAILURE == oam_mem_alloc(2, (void**)&v[1], &error_code)) {
		oam_fprintf(stderr, "Memory allocation failed!!!");
		return " ";
	}

    /*value 0 for energy saving cell switch off*/
	oam_strncpy(v[1],"0",sizeof("0") + OAM_ONE);
	count++;

	size = oam_strlen(V[1]) + 1;
	if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[2], &error_code)) {
		oam_fprintf(stderr, "Memory allocation failed!!!");
		return " ";
	}

	/* value 0 for energy saving cell switch off
      mode(either graceful or forceful) */
	oam_strncpy(v[2],V[1],size);
	count++;

	retVal = oam_qcli_send_command(count, v, E, T, arg);

    for (i = 0; i < count; ++i) {
        oam_mem_free(v[i], &error_code);
    }
    
    return retVal;
    /* SPR 12727 REVIEW COMMENT FIX END */
}
#endif


/*******************************************************************************
 * Function Name  : oam_show_log_cat_l2_mac
 * Description    : To serve the request for displaying the cat log of Mac.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_show_log_cat_l2_mac
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_GET_LOG_CAT_MAC_RESP;
	message_send.message_send = 1;
	oam_show_log_cat_l2_mac_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_show_log_cat_l2_pdcp
 * Description    : To serve the request for displaying the cat log of Pdcp.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_show_log_cat_l2_pdcp
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_GET_LOG_CAT_PDCP_RESP;
	message_send.message_send = 1;
	oam_show_log_cat_l2_pdcp_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_show_log_cat_l2_rlc
 * Description    : To serve the request for displaying the cat log of Rlc.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_show_log_cat_l2_rlc
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_GET_LOG_CAT_RLC_RESP;
	message_send.message_send = 1;
	oam_show_log_cat_l2_rlc_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_show_log_level_l2
 * Description    : To serve the request for displaying the log level of Mac.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_show_log_level_l2_all
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
    /* SPR 17842 START */
	message_send.apiid_expected = OAM_GET_LOG_LEVEL_L2_RESP;
    /* SPR 17842  END */
	message_send.message_send = 1;
	oam_show_log_level_l2_all_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_show_log_level_mac
 * Description    : To serve the request for displaying the log level of Mac.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_show_log_level_mac
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_GET_LOG_LEVEL_MAC_RESP;
	message_send.message_send = 1;
	oam_show_log_level_l2_mac_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_show_log_level_rlc
 * Description    : To serve the request for displaying the log level of RLC.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_show_log_level_rlc
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_GET_LOG_LEVEL_RLC_RESP;
	message_send.message_send = 1;
	oam_show_log_level_l2_rlc_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_show_log_level_pdcp
 * Description    : To serve the request for displaying the log level of PDCP
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_show_log_level_pdcp
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_GET_LOG_LEVEL_PDCP_RESP;
	message_send.message_send = 1;
	oam_show_log_level_l2_pdcp_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_show_log_level_egtpu
 * Description    : To serve the request for displaying the log level of EGTPU
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
/*SPR 19748 FIXED START*/
static
char *oam_show_log_level_gtpu
(
/*SPR 19748 FIXED END*/
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_GET_LOG_LEVEL_GTPU_RESP;
	message_send.message_send = 1;
    /*SPR 19748 FIXED START*/
	oam_show_log_level_l2_gtpu_pkt(C,V);
    /*SPR 19748 FIXED END*/
	return OAM_ZERO;
}

/*SPR 13617 START*/
/*******************************************************************************
 * Function Name  : oam_show_log_level_l3
 * Description    : To serve the request for displaying the log level of OAM.
 * Inputs         : C : Number of parameters passed to the command
 * V : The array of arguments passed to the command
 * E : This is the .environment. available to the command
 * T : This is the list of tags for the parameters passed
 * arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 ********************************************************************************/
static
char *oam_show_log_level_l3
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
        message_send.apiid_expected = OAM_GET_LOG_LEVEL_L3_RESP;
        message_send.message_send = 1;
        oam_show_log_level_l3_pkt(C,V);
        return OAM_ZERO;
}
/* SPR 13617 END */

/*******************************************************************************
 * Function Name  : oam_show_log_level_l3_s1ap
 * Description    : To serve the request for displaying the log level of OAM.
 * Inputs         : C : Number of parameters passed to the command
 * V : The array of arguments passed to the command
 * E : This is the .environment. available to the command
 * T : This is the list of tags for the parameters passed
 * arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 ********************************************************************************/
static
char *oam_show_log_level_l3_s1ap
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
        message_send.apiid_expected = OAM_GET_LOG_LEVEL_L3_S1AP_RESP;
        message_send.message_send = 1;
        oam_show_log_level_l3_s1ap_pkt(C,V);
        return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_show_log_level_l3_x2ap
 * Description    : To serve the request for displaying the log level of OAM.
 * Inputs         : C : Number of parameters passed to the command
 * V : The array of arguments passed to the command
 * E : This is the .environment. available to the command
 * T : This is the list of tags for the parameters passed
 * arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 ********************************************************************************/
static
char *oam_show_log_level_l3_x2ap
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
        message_send.apiid_expected = OAM_GET_LOG_LEVEL_L3_X2AP_RESP;
        message_send.message_send = 1;
        oam_show_log_level_l3_x2ap_pkt(C,V);
        return OAM_ZERO;
}


/*******************************************************************************
 * Function Name  : oam_show_log_level_rrm
 * Description    : To serve the request for displaying the log level of RRM.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_show_log_level_rrm
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
        message_send.apiid_expected = OAM_GET_LOG_LEVEL_RRM_RESP;
        message_send.message_send = 1;
        oam_qcli_send_rrm_get_log_level_cmd(C,V);
        return OAM_ZERO;
}
#ifdef OAM_SON_ENABLE
/*******************************************************************************
 * Function Name  : oam_show_log_level_son
 * Description    : To serve the request for displaying the log level of SON.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_show_log_level_son
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
        message_send.apiid_expected = OAM_GET_LOG_LEVEL_SON_RESP;
        message_send.message_send = 1;
        oam_son_get_log_lvl_pkt(C,V);
        return OAM_ZERO;
}
#endif
/*******************************************************************************
 * Function Name  : oam_show_log_level_oam
 * Description    : To serve the request for displaying the log level of OAM.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_show_log_level_oam
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
        message_send.apiid_expected = OAM_GET_LOG_LEVEL_RESP;
        message_send.message_send = 1;
        oam_show_log_level_oam_pkt(C,V);
        return OAM_ZERO;
}



/* SPR_13116_CHANGE_END */




#ifdef CLI_UNUSED

/*******************************************************************************
 * Function Name  : oam_show_stats_l2_mac
 * Description    : To get stats request of Mac L2 layer.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_show_stats_l2_mac
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{

	/********************************PM_CODE**************************************/





	message_send.apiid_expected = MAC_GET_STATS_CNF;
	message_send.message_send = 1;
	/********************************PM_CODE**************************************/
	oam_show_stats_l2_mac_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_show_stats_l2_rlc
 * Description    : To get stats request of Rlc of L2 layer.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_show_stats_l2_rlc
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
	/********************************PM_CODE**************************************/
	message_send.apiid_expected = RLC_GET_STATS_CNF;
	message_send.message_send = 1;
	/********************************PM_CODE**************************************/

	oam_show_stats_l2_rlc_pkt(C,V);

	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_show_stats_l2_pdcp
 * Description    : To get stats request of Pdcp of L2 layer.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_show_stats_l2_pdcp
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
	/********************************PM_CODE**************************************/
	message_send.apiid_expected = SHOW_STATS_L2_PDCP;
	message_send.message_send = 1;
	/********************************PM_CODE**************************************/
	oam_show_stats_l2_pdcp_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_show_stats_l2_egtpu
 * Description    : To get stats request of Egtpu of L2 layer.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_show_stats_l2_egtpu
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
	/********************************PM_CODE**************************************/
	message_send.apiid_expected = EGTPU_LM_INITDB_CNF; 
	message_send.message_send = 1; 
	/********************************PM_CODE**************************************/
	oam_show_stats_l2_egtpu_pkt(C,V);
	return OAM_ZERO;
}
# endif

#ifdef OAM_SON_ENABLE
/*******************************************************************************
 * Function Name  : oam_son_enable_warninglog
 * Description    : To Enable warning logs of son layer
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_son_enable_warninglog
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	switch(oam_atoi(V[1]))
	{
		case 0:
		case 25701 :
		case 25702 :
		case 25703 :
		case 25704 :
		case 25705 :
            message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
			message_send.message_send = 1;
			oam_son_enable_warning_pkt(C,V);
			return OAM_ZERO;
	}
	oam_fprintf(stdout,"Invalid values\n");
	return OAM_ZERO;
}
#endif

/*******************************************************************************
 * Function Name  : oam_son_enable_brieflog
 * Description    : To Enable brief logs of son layer
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

#ifdef OAM_SON_ENABLE
static
char *oam_son_enable_brieflog
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	switch(oam_atoi(V[1]))
	{
		case 0:
		case 25701 :
		case 25702 :
		case 25703 :
		case 25704 :
		case 25705 :
            message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
			message_send.message_send = 1;
			oam_son_enable_brief_pkt(C,V);
			return OAM_ZERO;
	}
	oam_fprintf(stdout,"Invalid values\n");
	return OAM_ZERO;
}

#endif
/*******************************************************************************
 * Function Name  : oam_son_enable_detailedlog
 * Description    : To Enable detailed logs of son layer
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
#ifdef OAM_SON_ENABLE
static
char *oam_son_enable_detailedlog
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	switch(oam_atoi(V[1]))
	{
		case 0:
		case 25701 :
		case 25702 :
		case 25703 :
		case 25704 :
		case 25705 :
            message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
			message_send.message_send = 1;
			oam_son_enable_detailed_pkt(C,V);
			return OAM_ZERO;
	}
	oam_fprintf(stdout,"Invalid values\n");
	return OAM_ZERO;
}
#endif

/*******************************************************************************
 * Function Name  : oam_rrm_disable_errorlog
 * Description    : To Disable error logs of rrm layer
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_rrm_disable_errorlog
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
    message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
	message_send.message_send = 1;
	oam_rrm_disable_error_pkt(C,V);
	return OAM_ZERO;
}

#ifdef OAM_SON_ENABLE
/*******************************************************************************
 * Function Name  : oam_son_disable_errorlog
 * Description    : To Disable error logs of son layer
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_son_disable_errorlog
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	switch(oam_atoi(V[1]))
	{
		case 0:
		case 25701 :
		case 25702 :
		case 25703 :
		case 25704 :
		case 25705 :
            message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
			message_send.message_send = 1;
			oam_son_disable_error_pkt(C,V);
			return OAM_ZERO;
	}
	oam_fprintf(stdout,"Invalid values\n");
	return OAM_ZERO;
}
#endif
/*******************************************************************************
 * Function Name  : oam_call_trace_start
 * Description    : To start call trace
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_call_trace_start
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	/* CSR_59405_FIX_START */
	message_send.apiid_expected = OAM_SET_CALL_TRACE_START_RESP;
	/* CSR_59405_FIX_END */
	message_send.message_send = 1;
	oam_call_trace_start_pkt(C,V);
	return OAM_ZERO;
}
/*******************************************************************************
 * Function Name  : oam_call_trace_stop
 * Description    : To stop call trace
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_call_trace_stop
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	/* CSR_59405_FIX_START */
	message_send.apiid_expected = OAM_SET_CALL_TRACE_STOP_RESP;
	/* CSR_59405_FIX_END */
	message_send.message_send = 1;
	oam_call_trace_stop_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_set_log_lvl_all
 * Description    : To set the desired log level to l2 layer.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_set_log_lvl_all
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
	message_send.message_send = 1;

	/* coverity 52810 */	
	oam_sys_log_level_et oam_all_et = SYS_CRITICAL;
	if(oam_strstr(*V,"Critical"))
	{
		oam_all_et = SYS_CRITICAL;
	}
	else if(oam_strstr(*V,"Error"))
	{
		oam_all_et = SYS_ERROR;
	}
	else if(oam_strstr(*V,"Warning"))
	{
		oam_all_et = SYS_WARNING;
	}
	else if(oam_strstr(*V,"Info"))
	{
		oam_all_et = SYS_INFO;
	}
	else if(oam_strstr(*V,"Brief"))
	{
		oam_all_et = SYS_BRIEF;
	}
	else if(oam_strstr(*V,"DetailedAll"))
	{
		oam_all_et = SYS_DETAILEDALL;
	}
	else if(oam_strstr(*V,"Detailed"))
	{
		oam_all_et = SYS_DETAILED;
	}

	oam_All_log_level_pkt(C,oam_all_et,OAM_SET_LOG_LEVEL_REQ);
	return OAM_ZERO;
}


/*******************************************************************************
 * Function Name  : oam_gtpu_incoming_trace_l2all
 * Description    : To view incoming trace logs of gtpu of L2.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

/*static
  char *oam_gtpu_incoming_trace_l2all
  (
  int C,
  char *const *V,
  const char *const *E,
  const char *const *T,
  void *arg
  )
  {
  oam_gtpu_incoming_l2all_pkt(C,V);
  return OAM_ZERO;
  }
  */
/*******************************************************************************
 * Function Name  : oam_gtpu_outgoing_trace_l2all
 * Description    : To view outgoing trace logs of gtpu of L2.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
/*
   static
   char *oam_gtpu_outgoing_trace_l2all
   (
   int C,
   char *const *V,
   const char *const *E,
   const char *const *T,
   void *arg
   )
   {
   oam_gtpu_outgoing_l2all_pkt(C,V);
   return OAM_ZERO;
   }*/
/*******************************************************************************
 * Function Name  : oam_gtpu_fsm_trace_l2all
 * Description    : To view fsm trace logs of gtpu of L2.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
/*
   static
   char *oam_gtpu_fsm_trace_l2all
   (
   int C,
   char *const *V,
   const char *const *E,
   const char *const *T,
   void *arg
   )
   {
   oam_gtpu_fsm_l2all_pkt(C,V);
   return OAM_ZERO;
   }
   */
/*******************************************************************************
 * Function Name  : oam_gtpu_error_trace_l2all
 * Description    : To view error trace logs of gtpu of L2.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
/*
   static
   char *oam_gtpu_error_trace_l2all
   (
   int C,
   char *const *V,
   const char *const *E,
   const char *const *T,
   void *arg
   )
   {
   oam_gtpu_error_l2all_pkt(C,V);
   return OAM_ZERO;
   }
   */
/*******************************************************************************
 * Function Name  : oam_gtpu_incoming_trace_all
 * Description    : To view all incoming trace logs of gtpu .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
/*
   static
   char *oam_gtpu_incoming_trace_all
   (
   int C,
   char *const *V,
   const char *const *E,
   const char *const *T,
   void *arg
   )
   {
   oam_gtpu_incoming_all_pkt(C,V);
   return OAM_ZERO;
   }
   */
/*******************************************************************************
 * Function Name  : oam_gtpu_outgoing_trace_all
 * Description    : To view all outgoing trace logs of gtpu .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
/*
   static
   char *oam_gtpu_outgoing_trace_all
   (
   int C,
   char *const *V,
   const char *const *E,
   const char *const *T,
   void *arg
   )
   {
   oam_gtpu_outgoing_all_pkt(C,V);
   return OAM_ZERO;
   }*/
/*******************************************************************************
 * Function Name  : oam_gtpu_fsm_trace_all
 * Description    : To view all fsm trace logs of gtpu .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
/*
   static
   char *oam_gtpu_fsm_trace_all
   (
   int C,
   char *const *V,
   const char *const *E,
   const char *const *T,
   void *arg
   )
   {
   oam_gtpu_fsm_all_pkt(C,V);
   return OAM_ZERO;
   }*/
/*******************************************************************************
 * Function Name  : oam_gtpu_error_trace_all
 * Description    : To view all error trace logs of gtpu .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the commandoam_son_disable_detailedlog
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
/*
   static
   char *oam_gtpu_error_trace_all
   (
   int C,
   char *const *V,
   const char *const *E,
   const char *const *T,
   void *arg
   )
   {
   oam_gtpu_error_all_pkt(C,V);
   return OAM_ZERO;
   }
   */
/*******************************************************************************
 * Function Name  : oam_l2_disable_sys_fail
 * Description    : To disable system failure .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_l2_disable_sys_fail
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_l2_disable_sys_fail_pkt(C,V);
	return OAM_ZERO;
}
/*******************************************************************************
 * Function Name  : oam_disable_lmac_l1_inf
 * Description    : To disable mac information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_disable_lmac_l1_inf
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_lmac_l1_inf_pkt(C,V);
	return OAM_ZERO;
}
/*******************************************************************************
 * Function Name  : oam_disable_mac_ul_harq
 * Description    : To disable mac uplink information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_disable_mac_ul_harq
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_ul_harq_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_dl_harq
 * Description    : To disable mac downlink information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_disable_mac_dl_harq
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_dl_harq_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_rach
 * Description    : To disable mac rach information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_disable_mac_rach
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_rach_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_rrc_inf
 * Description    : To disable mac rrc information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_disable_mac_rrc_inf
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_rrc_inf_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_oam_inf
 * Description    : To disable mac oam information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_disable_mac_oam_inf
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_oam_inf_pkt(C,V);
	return OAM_ZERO;
}

/*CSR:00055317_FIX_START*/

/*******************************************************************************
 * Function Name  : oam_disable_mac_rrm_inf
 * Description    : To disable mac rrm information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                arg : This is the argument that was passed to qclOpen
 *Outputs        : NONE
 *Returns        : NONE
 ********************************************************************************/

static
char *oam_disable_mac_rrm_inf
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

    message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
    message_send.message_send = 1;
    oam_disable_mac_rrm_inf_pkt(C,V);
    return OAM_ZERO;
}
/*CSR:00055317_FIX_END*/


/* SPR 55317 START */

/*******************************************************************************
 * Function Name  : oam_disable_mac_dl_strategy 
 * Description    : To disable mac oam information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_dl_strategy
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_dl_strategy_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_ul_strategy 
 * Description    : To disable mac oam information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_ul_strategy
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_ul_strategy_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_pdsch 
 * Description    : To disable 
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_pdsch
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_pdsch_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_pdcch 
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_pdcch
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_pdcch_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_pusch 
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_pusch
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_pusch_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_pucch 
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_pucch
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_pucch_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_bcch 
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_bcch
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_bcch_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_pcch 
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_pcch
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_pcch_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_ccch 
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_ccch
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_ccch_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_cqi 
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_cqi
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_cqi_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_srs 
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_srs
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_srs_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_tb_ul 
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_tb_ul
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_tb_ul_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_tb_dl 
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_tb_dl
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_tb_dl_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_drx 
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_drx
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_drx_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_meas_gap 
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_meas_gap
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_meas_gap_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_meas_handler 
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_meas_handler
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_meas_handler_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_ul_csi
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_ul_csi
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_ul_csi_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_mux
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_mux
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_mux_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_dl_sps_strategy
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_dl_sps_strategy
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{   /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_dl_sps_strategy_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_ul_sps_strategy
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_ul_sps_strategy
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
    message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_ul_sps_strategy_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_scheduler 
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_scheduler
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
    message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_scheduler_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_atb
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_atb
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
    message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_atb_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_ta
 * Description    :  
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char  *oam_disable_mac_ta
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */
    message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_ta_pkt(C,V);
	return OAM_ZERO;
}

/* SPR 55317 END */

/*******************************************************************************
 * Function Name  : oam_disble_mac_dl_pc
 * Description    : To disable mac downlink pc information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_disble_mac_dl_pc
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

    message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disble_mac_dl_pc_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_ul_pc
 * Description    : To disable mac uplink pc information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_disable_mac_ul_pc
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_mac_ul_pc_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_set_log_cat_disable_rlc_all
 * Description    : To disable rlc logs information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_set_log_cat_disable_rlc_all
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_set_log_cat_disable_rlc_all_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_rlc_am 
 * Description    : To disable rlc_am log information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_disable_rlc_am
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_rlc_am_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_rlc_um 
 * Description    : To disable rlc_um log information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_disable_rlc_um
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_rlc_um_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_rlc_tm 
 * Description    : To disable rlc_tm log information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_disable_rlc_tm
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_rlc_tm_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_rlc_tx 
 * Description    : To disable rlc_tx log information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_disable_rlc_tx
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_rlc_tx_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_rlc_rx 
 * Description    : To disable rlc_rx log information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_disable_rlc_rx
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_rlc_rx_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_rlc_mac 
 * Description    : To disable rlc_mac log information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_disable_rlc_mac
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_rlc_mac_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_rlc_pdcp 
 * Description    : To disable rlc_pdcp log information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_disable_rlc_pdcp
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_rlc_pdcp_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_rlc_rrc 
 * Description    : To disable rlc_rrc log information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_disable_rlc_rrc
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_rlc_rrc_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_rlc_oam 
 * Description    : To disable rlc_oam log information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_disable_rlc_oam
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_rlc_oam_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_disable_rlc_timer 
 * Description    : To disable rlc_timer log information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_disable_rlc_timer
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_disable_rlc_timer_pkt(C,V);
	return OAM_ZERO;
}

/* SPR 55317 END */

/*SPR 5317 START */

/*******************************************************************************
 * Function Name  : oam_set_log_cat_disable_pdcp_all
 * Description    : To disable pdcp logs information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_set_log_cat_disable_pdcp_all
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_set_log_cat_disable_pdcp_all_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_set_log_cat_disable_pdcp_rrcoam 
 * Description    : To disable pdcp logs information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_set_log_cat_disable_pdcp_rrcoam
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_set_log_cat_disable_pdcp_rrcoam_pkt(C,V);
	return OAM_ZERO;
}
/*******************************************************************************
 * Function Name  : oam_set_log_cat_disable_pdcp_tx
 * Description    : To disable pdcp logs information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_set_log_cat_disable_pdcp_tx
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_set_log_cat_disable_pdcp_tx_pkt(C,V);
	return OAM_ZERO;
}
/*******************************************************************************
 * Function Name  : oam_set_log_cat_disable_pdcp_rx
 * Description    : To disable pdcp logs information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_set_log_cat_disable_pdcp_rx
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_set_log_cat_disable_pdcp_rx_pkt(C,V);
	return OAM_ZERO;
}
/*******************************************************************************
 * Function Name  : oam_set_log_cat_disable_pdcp_rrm_int
 * Description    : To disable pdcp logs information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_set_log_cat_disable_pdcp_rrm_int
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_set_log_cat_disable_pdcp_rrm_int_pkt(C,V);
	return OAM_ZERO;
}

/* SPR 55317 END */


/*******************************************************************************
 * Function Name  : oam_set_log_cat_l2_disable_mac_all
 * Description    : To disable mac logs information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_set_log_cat_l2_disable_mac_all
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	oam_set_log_cat_disable_mac_all_pkt(C,V);
	return OAM_ZERO;
}
/*******************************************************************************
 * Function Name  : oam_lvl
 * Description    : To disable mac logs information .
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_lvl
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	message_send.apiid_expected = OAM_SET_LOG_LEVEL_RESP;
	message_send.message_send = 1;
	UInt32 oam_val = OAM_ZERO;
    if(oam_strstr(*V,"Critical"))
	{
		oam_val = OAM_CRITICAL;
	}
    else if(oam_strstr(*V,"Error"))
	{
		oam_val = OAM_ERROR;
	}
    else if(oam_strstr(*V,"Warning"))
	{
		oam_val = OAM_WARNING;
	}
    else if(oam_strstr(*V,"Info"))
	{
		oam_val = OAM_INFO;
	}
    else if(oam_strstr(*V,"Brief"))
	{
		oam_val = OAM_BRIEF;
	}
    else if(oam_strstr(*V,"DetailedAll"))
	{
		oam_val = OAM_DETAILEDALL;
	}
    else if(oam_strstr(*V,"Detailed"))
	{
		oam_val = OAM_DETAILED;
	}
	oam_lvl_pkt(C,oam_val);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_rrc_fatallog
 * Description    : To view rrc fatal log information of oam.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

static
char *oam_rrc_log_lvl
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

    message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
	message_send.message_send = 1;
	/*  CSR 57877 Fix Start */
	setloglevell3rrccommands oam_rrc_et  = OAM_RRC_ERRORLOG;

	if(oam_strstr(*V,"Critical"))
	{
		oam_rrc_et = OAM_RRC_CRITICALLOG;
	}
	else if(oam_strstr(*V,"Error"))
	{
		oam_rrc_et = OAM_RRC_ERRORLOG;
	}
	else if(oam_strstr(*V,"Warning"))
	{
		oam_rrc_et = OAM_RRC_WARNINGLOG;
	}
	else if(oam_strstr(*V,"Info"))
	{
		oam_rrc_et = OAM_RRC_INFOLOG;
	}
	else if(oam_strstr(*V,"Brief"))
	{
		oam_rrc_et = OAM_RRC_BRIEFLOG;
	}
	else if(oam_strstr(*V,"DetailedAll"))
	{
		oam_rrc_et = OAM_RRC_DETAILEDALLLOG;
	}
	else if(oam_strstr(*V,"Detailed"))
	{
		oam_rrc_et = OAM_RRC_DETAILEDLOG;
	}
	/*  CSR 57877 Fix End */

	oam_rrc_log_lvl_pkt(C,oam_rrc_et);
	return OAM_ZERO;
}

/*******************************************************************************
** Function Name  : oam_X2AP_fatallog
** Description    : To view X2AP fatal log information of oam.
** Inputs         : C : Number of parameters passed to the command
*                   V : The array of arguments passed to the command
**                  E : This is the .environment. available to the command
**                  T : This is the list of tags for the parameters passed
**                  arg : This is the argument that was passed to qclOpen
** Outputs        : NONE
** Returns        : NONE
********************************************************************************/

static
char *oam_X2AP_log_lvl
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

   message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
   message_send.message_send = 1;
   /*  CSR 57877 Fix Start */
   setloglevelX2APcommands oam_X2AP_et  = OAM_X2AP_ERRORLOG;

   if(oam_strstr(*V,"Critical"))
   {
       oam_X2AP_et = OAM_X2AP_CRITICALLOG;
   }
   else if(oam_strstr(*V,"Error"))
   {
       oam_X2AP_et = OAM_X2AP_ERRORLOG;
   }
   else if(oam_strstr(*V,"Warning"))
   {
       oam_X2AP_et = OAM_X2AP_WARNINGLOG;
   }
   else if(oam_strstr(*V,"Info"))
   {
       oam_X2AP_et = OAM_X2AP_INFOLOG;
   }
   else if(oam_strstr(*V,"Brief"))
   {
       oam_X2AP_et = OAM_X2AP_BRIEFLOG;
   }
   else if(oam_strstr(*V,"Detailed"))
   {
       oam_X2AP_et = OAM_X2AP_DETAILEDLOG;
   }
   /*  CSR 57877 Fix End */

   oam_X2AP_log_lvl_pkt(C,oam_X2AP_et);
   return OAM_ZERO;
}
              

/*******************************************************************************
** Function Name  : oam_S1AP_fatallog
** Description    : To view S1AP fatal log information of oam.
** Inputs         : C : Number of parameters passed to the command
*                   V : The array of arguments passed to the command
**                  E : This is the .environment. available to the command
**                  T : This is the list of tags for the parameters passed
**                  arg : This is the argument that was passed to qclOpen
** Outputs        : NONE
** Returns        : NONE
********************************************************************************/

static
char *oam_S1AP_log_lvl
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

   message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
   message_send.message_send = 1;
   /*  CSR 57877 Fix Start */
   setloglevelS1APcommands oam_S1AP_et  = OAM_S1AP_ERRORLOG;

   if(oam_strstr(*V,"Critical"))
   {
       oam_S1AP_et = OAM_S1AP_CRITICALLOG;
   }
   else if(oam_strstr(*V,"Error"))
   {
       oam_S1AP_et = OAM_S1AP_ERRORLOG;
   }
   else if(oam_strstr(*V,"Warning"))
   {
       oam_S1AP_et = OAM_S1AP_WARNINGLOG;
   }
   else if(oam_strstr(*V,"Info"))
   {
       oam_S1AP_et = OAM_S1AP_INFOLOG;
   }
   else if(oam_strstr(*V,"Brief"))
   {
       oam_S1AP_et = OAM_S1AP_BRIEFLOG;
   }
   else if(oam_strstr(*V,"Detailed"))
   {
       oam_S1AP_et = OAM_S1AP_DETAILEDLOG;

   }
   /*  CSR 57877 Fix End */

   oam_S1AP_log_lvl_pkt(C,oam_S1AP_et);
   return OAM_ZERO;
}


#ifdef OAM_SON_ENABLE
/*******************************************************************************
 * Function Name  : oam_son_enable_log
 * Description    : To Enable log information of oam.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_son_enable_log
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

    setson oam_son_log = OAM_SON_ERRORLOG;
    switch(oam_atoi(V[1]))
    {
        case 0:
        case 25701 :
        case 25702 :
        case 25703 :
        case 25704 :
        case 25705 :
        {
            message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
            message_send.message_send = 1;
            if(oam_strstr(*V,"Critical"))
            {
                oam_son_log = OAM_SON_CRITICALLOG;
            }
            else if(oam_strstr(*V,"Error"))
            {
                oam_son_log = OAM_SON_ERRORLOG;
            }
            else if(oam_strstr(*V,"Warning"))
            {
                oam_son_log = OAM_SON_WARNINGLOG;
            }
            else if(oam_strstr(*V,"Info"))
            {
                oam_son_log = OAM_SON_INFOLOG;
            }
            else if(oam_strstr(*V,"Brief"))
            {
                oam_son_log = OAM_SON_BRIEFLOG;
            }
            else if(oam_strstr(*V,"DetailedAll"))
            {
                oam_son_log = OAM_SON_DETAILEDALLLOG;
            }
            else if(oam_strstr(*V,"Detailed"))
            {
                oam_son_log = OAM_SON_DETAILEDLOG;
            }
            oam_son_enable_pkt(C,V,oam_son_log);
            break;
        }
        default :
        {
            oam_fprintf(stdout,"Invalid values\n");
            break;
        }
    }
    return OAM_ZERO;
}
/*******************************************************************************
 * Function Name  : oam_son_enable_errorlog
 * Description    : To Enable error log information of oam.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_son_enable_errorlog
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	switch(oam_atoi(V[1]))
	{
		case 0:
		case 25701 :
		case 25702 :
		case 25703 :
		case 25704 :
		case 25705 :
            message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
			message_send.message_send = 1;
			oam_son_enable_error_pkt(C,V);
			return OAM_ZERO;
	}
	oam_fprintf(stdout,"Invalid values\n");
	return OAM_ZERO;
}
#endif

/*******************************************************************************
 * Function Name  : oam_log_enable_rrc
 * Description    : To Enable error log information of oam.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_log_enable_rrc
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

    message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
	message_send.message_send = 1;
	oam_log_enable_rrc_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_log_enable_x2ap
 * Description    : To Enable error log information of oam.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_log_enable_x2ap
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);
    /* SPR 17777 FIX END */

    message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
    message_send.message_send = 1;
    oam_log_enable_x2ap_pkt(C,V);
    return OAM_ZERO;
}


/*******************************************************************************
 * Function Name  : oam_log_enable_s1ap
 * Description    : To Enable error log information of oam.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_log_enable_s1ap
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);
    /* SPR 17777 FIX END */

    message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
    message_send.message_send = 1;
    oam_log_enable_s1ap_pkt(C,V);
    return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_log_enable_rrm
 * Description    : To Enable RRM log information requested through OAM CLI.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
Char8* oam_log_enable_rrm
(
 SInt32 C,
 Char8* const *V,
 const Char8* const *E,
 const Char8* const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

    message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
    message_send.message_send = 1;
    rrm_oam_log_level_et log_level = RRM_OAM_ERROR;

    if(oam_strstr(*V, "Critical")) {
        log_level = RRM_OAM_CRITICAL;
    }
    else if(oam_strstr(*V, "Error")) {
        log_level = RRM_OAM_ERROR;
    }
    else if(oam_strstr(*V, "Warning")) {
        log_level = RRM_OAM_WARNING;
    }
    else if(oam_strstr(*V, "Info")) {
        log_level = RRM_OAM_INFO;
    }
    else if(oam_strstr(*V, "Brief")) {
        log_level = RRM_OAM_BRIEF;
    }
    else if(oam_strstr(*V, "DetailedAll")) {
        log_level = RRM_OAM_DETAILEDALL;
    }
    else if(oam_strstr(*V, "Detailed")) {
        log_level = RRM_OAM_DETAILED;
    }
	oam_enable_rrm_pkt(C,V,log_level);
    return OAM_NULL;
}

/*******************************************************************************
 * Function Name  : oam_log_enable_rrm_error
 * Description    : To Enable error log information of oam.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_log_enable_rrm_error
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

    message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
	message_send.message_send = 1;
	oam_enable_rrm_error_pkt(C,V);
	return OAM_ZERO;
}
/*******************************************************************************
 * Function Name  : oam_log_enable_rrm_warning
 * Description    : To Enable error log information of oam.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_log_enable_rrm_warning
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

    message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
	message_send.message_send = 1;
	oam_log_enable_rrm_warn_pkt(C,V);
	return OAM_ZERO;
}
/*******************************************************************************
 * Function Name  : oam_log_enable_rrm_brief
 * Description    : To Enable error log information of oam.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_log_enable_rrm_brief
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

    message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
	message_send.message_send = 1;
	oam_log_enable_rrm_brief_pkt(C,V);
	return OAM_ZERO;
}
/*******************************************************************************
 * Function Name  : oam_log_enable_rrm_detailed
 * Description    : To Enable error log information of oam.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_log_enable_rrm_detailed
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

    message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
	message_send.message_send = 1;
	oam_log_enable_rrm_detail_pkt(C,V);
	return OAM_ZERO;
}
/*******************************************************************************
 * Function Name  : oam_log_disable_rrc
 * Description    : To Enable error log information of oam.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_log_disable_rrc
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

    message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
	message_send.message_send = 1;
	oam_log_disable_rrc_pkt(C,V);
	return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_log_disable_x2ap
 * Description    : To Enable error log information of oam.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_log_disable_x2ap
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);
    /* SPR 17777 FIX END */

    message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
    message_send.message_send = 1;
    oam_log_disable_x2ap_pkt(C,V);
    return OAM_ZERO;
}


/*******************************************************************************
 * Function Name  : oam_log_disable_s1ap
 * Description    : To Enable error log information of oam.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_log_disable_s1ap
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);
    /* SPR 17777 FIX END */

    message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
    message_send.message_send = 1;
    oam_log_disable_s1ap_pkt(C,V);
    return OAM_ZERO;
}

/*******************************************************************************
 * Function Name  : oam_qcli_rrm_set_log_level_handler
 * Description    : To Enable error log information of oam.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
Char8* oam_qcli_rrm_set_log_level_handler
(
 SInt32 C,
 Char8* const *V,
 const Char8* const *E,
 const Char8* const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

    message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
	message_send.message_send = 1;
	rrm_oam_log_level_et log_level = RRM_OAM_ERROR;

	if(oam_strstr(*V, "Critical")) {
		log_level = RRM_OAM_CRITICAL;
	}
	else if(oam_strstr(*V, "Warning")) {
		log_level = RRM_OAM_WARNING;
	}
	else if(oam_strstr(*V, "Info")) {
		log_level = RRM_OAM_INFO;
	}
	else if(oam_strstr(*V, "Brief")) {
		/* coverity 52809 */
		log_level = RRM_OAM_BRIEF;
	}
	else if(oam_strstr(*V, "DetailedAll")) {
		log_level = RRM_OAM_DETAILEDALL;
	}
	else if(oam_strstr(*V, "Detailed")) {
		log_level = RRM_OAM_DETAILED;
	}

	oam_qcli_send_rrm_set_log_level_cmd(C, log_level, V);
	return OAM_NULL;
}

/*******************************************************************************
 * Function Name  : oam_son_log_lvl
 * Description    : To Enable error log information of oam.
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/

#ifdef OAM_SON_ENABLE
static
char *oam_son_log_lvl
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

    message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
	message_send.message_send = 1;
	/*  CSR 57877 Fix Start*/
/* Cov CID 62588/62589 fix*/    
    setson oam_son_log = OAM_SON_ERRORLOG;
	if(oam_strstr(*V,"Critical"))
	{
        oam_son_log = OAM_SON_CRITICALLOG;
	}
	else if(oam_strstr(*V,"Error"))
	{
        oam_son_log = OAM_SON_ERRORLOG;
	}
	else if(oam_strstr(*V,"Warning"))
	{
        oam_son_log = OAM_SON_WARNINGLOG;
	}
	else if(oam_strstr(*V,"Info"))
	{
        oam_son_log = OAM_SON_INFOLOG;
	}
	else if(oam_strstr(*V,"Brief"))
	{
        oam_son_log = OAM_SON_BRIEFLOG;
	}
	else if(oam_strstr(*V,"DetailedAll"))
	{
        oam_son_log = OAM_SON_DETAILEDALLLOG;
	}
	else if(oam_strstr(*V,"Detailed"))
	{
        oam_son_log = OAM_SON_DETAILEDLOG;
	}
	/*  CSR 57877 Fix End */
    oam_son_log_lvl_pkt(C,oam_son_log,V);
	return OAM_ZERO;
}
#endif

/*******************************************************************************
 * Function Name  : oam_shutdown
 * Description    : To Shutdown OAM LTE
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
static
char *oam_shutdown
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	oam_send_shutdown_request(C,V);
	return OAM_ZERO;
}
/*******************************************************************************
 * Function Name  : oam_restart
 * Description    : To Restart LTE OAM 
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : NONE
 ********************************************************************************/
static
char *oam_restart
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);    
    /* SPR 17777 FIX END */

	oam_send_shutdown_request(C,V);
	return OAM_ZERO;
}
/*******************************************************************************
 * Includes
 *******************************************************************************/
/* Public Command Tables */
extern const struct command_t commands[];
/*******************************************************************************
 * Function Name  : current
 * Description    : To Restart LTE OAM
 * Inputs         : unused 
 * Outputs        : NONE
 * Returns        : NONE
 ********************************************************************************/
static char **current(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */

	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("Current"));
	return L;
}

static const struct parameter_t __0806DDB8[] = {
	qmaMandatory("Cell_Identity", qmaDynamic(" ",current),"Accepted Value : Current"),
	qmaEndParams()
};

static const struct parameter_t __0806DEB8[] = {
	qmaMandatory("Cell_Identity", qmaDynamic(" ",current),"Accepted Value : Current"),
	qmaEndParams()
};
static const struct parameter_t __0806SD01[] = {
	qmaMandatory("Cell_Identity", qmaDynamic(" ",current),"Accepted Value : Current"),
	qmaEndParams()
};
static const struct parameter_t __0806SD02[] = {
	qmaMandatory("Cell_Identity", qmaDynamic(" ",current),"Accepted Value : Current"),
	qmaEndParams()
};

/*Cell Block and Cell unblock*/
static const struct parameter_t __0806SSA8[] = {
    /*SPR 13518 START*/
    qmaKeyword("Cell_Identity"),
    qmaMandatory("Cell_Identity", qmaDynamic(" ",current),"Accepted Value : Current"),
    qmaKeyword("Cell_Block_Priority"),
    /* SPR 15291 Fix start */
    qmaMandatory("Cell_Block_Priority", qmaStandard(INTEGER, "0:1"),"Priority : 1:LOW -:Resource_Cleanup_Timer Mandatory for LOW Priority Cell Block   0:HIGH"),
    /* SPR 15291 Fix end */
    qmaOptional( "Resource_Cleanup_Timer", "-", qmaStandard(INTEGER, "-1:32767"), "Accepted Value: -1 to 32767" ),
    /*SPR 13518 START*/

	qmaEndParams()
};
static const struct parameter_t __0806SSB8[] = {
	qmaMandatory("Cell_Identity", qmaDynamic(" ",current),"Accepted Value : Current"),
	qmaEndParams()
};

static const struct parameter_t __0806NSB8[] = {
	qmaMandatory( 0, qmaStandard(STRING, 0), "Software Package Name For Download." ), 
	qmaEndParams()
};

static const struct parameter_t __0806NSB9[] = {
	qmaMandatory( 0, qmaStandard(STRING, 0), "Software Package Name For Upgrade." ), 
	qmaEndParams()
};
/*******************************************************************************
 * Function Name  : log_archival 
 * Description    : To find tag log_archival (LayerLogs) 
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **log_archival(const void *unused)
{
    /* SPR 17777 FIX START */
     qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("LayerLogs"));

	return L;
}
static const struct parameter_t __0807SSQ2[] = {
	qmaMandatory(0,qmaDynamic("",log_archival),"LogsType -- It can possibly have two possible values: LayerLogs and AlarmLogs (this is planned for later release)"),
	qmaEndParams()
}; 


static const struct command_t __0806NED8[] = {
	qmaCommand("Download", __0806NSB8, oam_qcli_send_command, "To Download a Software Package" ),
	qmaCommand("Switch", __0806NSB9, oam_qcli_send_command, "To Upgrade a Software Package" ),
	qmaCommand("Archive", __0807SSQ2, oam_qcli_send_command, "To Archive Layer Logs" ),
	qmaEndCommands()
};


static const struct command_t __0806DED8[] = {
	/*CELL_DELETE_2.0*/
	qmaCommand( "Delete", __0806SD01, oam_qcli_send_command, "To Delete Current Cell" ),
	qmaCommand( "Setup", __0806SD02, oam_qcli_send_command, "To Setup Current Cell" ),
	qmaCommand( "Start", __0806DDB8, oam_qcli_send_command, "To Start Current Cell" ),
	qmaCommand( "Stop", __0806DEB8, oam_qcli_send_command, "To Stop Current Cell" ),
	qmaCommand( "Block", __0806SSA8, oam_qcli_send_command, "To Block Current Cell" ),
	qmaCommand( "Unblock", __0806SSB8, oam_qcli_send_command, "To Unblock Current Cell" ),

	qmaEndCommands()
};

static const struct parameter_t __0806E0C0[] = {
	qmaMandatory( 0, qmaStandard(INTEGER, "1:60"), "Sampling Interval Time(In minutes) Time interval for configuring the lower layers for periodic reporting of KPIs : 1 to 60 for L2, L3 and RRM  " ), /* PM_REVIEW_CHANGES */
	qmaMandatory( 0, qmaStandard(STRING, 0), "Periodic Upload Time  : (Format 2013-12-01T09:09:00Z)(Time when the PM XML file is to be uploaded at server : Enter Valid Future Time)" ),
	qmaMandatory( 0, qmaStandard(INTEGER, "5:1000"), "File Write Interval (In minutes)  : 5 to 1000 (Time interval to instruct OAM for performing write operation to PM XML file)" ),
	qmaMandatory( 0, qmaStandard(CHOICE, "3600|86400"), "Periodic Upload Interval (In Seconds) : 3600 or 86400 (Time interval for which KPIs should be fetched in single PM XML File, post expiry data shall be dumped in new file)" ),
	qmaEndParams()
};

static const struct parameter_t __0706E0C0[] = {
	qmaMandatory( 0, qmaStandard(INTEGER, "0:100"), "Number of UE, 0: For All UE" ),
	qmaMandatory( 0, qmaStandard(INTEGER, "1:9"), "QCI Number" ),
	qmaOptional( "UL_Bitrate_Max", "-",qmaStandard(INTEGER,"1:4294967295"), "UL Bitrate Max bits/s" ),
	qmaOptional( "UL_Bitrate_Min", "-",qmaStandard(INTEGER,"1:4294967295"), "UL Bitrate Min bits/s" ),
	qmaOptional( "DL_Bitrate_Max", "-",qmaStandard(INTEGER,"1:4294967295"), "DL Bitrate Max bits/s" ),
	qmaOptional( "DL_Bitrate_Min", "-",qmaStandard(INTEGER,"1:4294967295"), "DL Bitrate Min bits/s" ),
	qmaEndParams()
};


static const struct parameter_t __0806F0C0[] = {
    qmaMandatory( 0, qmaStandard(INTEGER, "0:65535"), "UE Index" ),
	qmaEndParams()
};

static const struct command_t __0806E2D0[] = {
	qmaCommand( "Periodic-Start", __0806E0C0, oam_qcli_send_command, "To Configure The Periodic KPIs maintained at L2, L3 and RRM" ),
	qmaCommandNoParam( "Periodic-Stop",oam_qcli_send_command_no_param, "To stop the periodic KPIs maintained at L2, L3 and RRM" ),
	qmaCommandNoParam( "On-demand",oam_qcli_send_command_no_param, "To start Ondemand KPIs maintained at L2, L3 and RRM, if ondemand is enabled then periodic KPIs will not be maintained" ),
	qmaEndCommands()
};
static const struct command_t __0816E2D0[] = {
	qmaCommand( "RELEASE", __0806F0C0, oam_qcli_send_command, "To Release RRM UE" ),
	qmaEndCommands()
};

static const struct command_t __0806N5C8[] = {
	qmaCommandNoParam( "MAC", oam_qcli_send_command_no_param, "Request to reset KPI for MAC layer of L2" ),
	qmaCommandNoParam( "PDCP",oam_qcli_send_command_no_param, "Request to reset KPI for PDCP layer of L2" ),
	qmaEndCommands()
};
static const struct command_t __0806N790[] = {
	qmaCommandNoParam( "RRC",oam_qcli_send_command_no_param, "Request to reset stats for RRC layer of L3" ),
	qmaEndCommands()
};


static const struct command_t __0806E5C8[] = {
	qmaCommandNoParam( "MAC", oam_qcli_send_command_no_param, "Request to reset stats for MAC layer of L2" ),
	qmaCommandNoParam( "PDCP",oam_qcli_send_command_no_param, "Request to reset stats for PDCP layer of L2" ),
	qmaCommandNoParam( "RLC", oam_qcli_send_command_no_param, "Request to reset stats for RLC layer of L2" ),
	qmaEndCommands()
};


static const struct command_t __0806E918[] = {
	qmaMode( "L2", "Reset stats L2 ", __0806E5C8, "To reset L2 stats " ),
	qmaEndCommands()
};

static const struct command_t __0806N792[] = {
	qmaCommandNoParam( "All",oam_qcli_send_command_no_param, "Reset Request for RRM KPIs" ),
	qmaEndCommands()
};

static const struct command_t __0806N918[] = {
	qmaMode( "L2", "Reset KPI L2 ", __0806N5C8, "To reset L2 KPI " ),
	qmaMode( "L3", "Reset KPI L3 ", __0806N790, "To reset L3 KPI " ),
	qmaMode( "RRM", "Reset KPI RRM ", __0806N792, "To reset RRM KPI " ),

	qmaEndCommands()
};

static const struct command_t __0806E968[] = {
	qmaMode( "stats", "Reset stats ", __0806E918, "To Reset Statistics" ),
	qmaMode( "KPI", "Reset KPI ", __0806N918, "To Reset KPI" ), 
	qmaEndCommands()
};


static const struct parameter_t __08076208[] = {
	qmaOptional( "QRxLevMinSIB1", "-", qmaStandard(INTEGER, "-70:-22"), "Minimum Required Received RSRP Level Of E-UTRA Cell" ),
	qmaOptional( "QRxLevMinSIB3", "-", qmaStandard(INTEGER, "-70:-22"), "Minimum Required Received RSRP Level For Intra-Frequency E-UTRA Cell" ),
	qmaOptional( "QRxLevMinOffset", "-",qmaStandard(INTEGER, "1:8"), "Set Offset With Respect To QrxLevMin" ),
	qmaOptional( "SIntraSearch", "-", qmaStandard(INTEGER, "0:31"), "Threshold For Intra-Frequency Measurements" ),
	qmaOptional( "TReselectionEUTRA", "-", qmaStandard(INTEGER, "0:7"), "Cell Reselection Timer For Intra Frequency E-UTRA Cell Reselection" ),
	qmaOptional( "SNonIntraSearch", "-", qmaStandard(INTEGER, "0:31"), "Used To Indicate An Reccieve Level Threshold For Cell Reselection" ),
	qmaOptional( "CellReselectionPriority", "-", qmaStandard(INTEGER, "0:7"), "Absolute Priority Of The E-UTRAN Serving Carrier Frequency" ),
	qmaOptional( "ThreshServingLow", "-", qmaStandard(INTEGER, "0:31"), "Threshold For Serving Frequency Used In Evaluation Of Reselection" ),
	qmaOptional( "TReselectionEUTRASFHigh", "-", qmaStandard(CHOICE,"25|50|75|100"), "Specifies the scaling factor for intra-frequency TreselectionEUTRA in High-mobility state : Accepted Input 25,50,75,100" ),
	qmaOptional( "TReselectionEUTRASFMedium", "-", qmaStandard(CHOICE,"25|50|75|100"), "Specifies the scaling factor for intra-frequency TreselectionEUTRA in Medium-mobility state : Accepted Input 25,50,75,100" ),
	qmaEndParams()
};

static const struct parameter_t __0806EDD8[] = {
	qmaOptional( "NCellChangeHigh", "-", qmaStandard(INTEGER, "1:16"), "Number of cell re-selections within TEvaluation to enter High-mobility state, Valid Range: (1 to 16)"),
	qmaOptional( "NCellChangeMedium", "-", qmaStandard(INTEGER, "1:16"), "Number of cell re-selections within TEvaluation to enter Medium-mobility state, Valid Range: (1 to 16)"),
	qmaOptional( "Qhyst", "-",qmaStandard(CHOICE,"0|1|2|3|4|5|6|8|10|12|14|16|18|20|22|24"), "Hysteresis value applied to serving cell for evaluating cell ranking criteria in dB. Accepted Input 0/ 1/ 2/ 3/ 4/ 5/ 6/ 8/ 10/ 12/ 14/ 16/ 18/ 20/ 22/ 24" ),
	qmaOptional( "QHystSFHigh", "-",qmaStandard(CHOICE,"-6|-4|-2|0"), "Speed-dependent scaling factor for Qhyst in High-mobility state in dBn. Accepted Input -6/ -4/ -2/ 0" ),
	qmaOptional( "QHystSFMedium", "-",qmaStandard(CHOICE,"-6|-4|-2|0"), "Speed-dependent scaling factor for Qhyst in Medium-mobility state in dBn. Accepted Input -6/ -4/ -2/ 0" ),
	qmaOptional( "TEvaluation", "-",qmaStandard(CHOICE,"30|60|120|180|240"), "Specifies the duration for evaluating allowed amount of cell reselection(s) required to enter mobility states. Accepted Input 30/ 60/ 120/ 180/ 240(in sec) " ),
	qmaOptional( "THystNormal", "-",qmaStandard(CHOICE,"30|60|120|180|240"), "Specifies the additional time period for evaluating criteria to enter Normal-mobility state. Accepted Input 30/ 60/ 120/ 180/ 240(in sec)" ),
	qmaEndParams()
};

static const struct command_t __08076238[] = {
	qmaCommand( "IntraFreq", __08076208, oam_qcli_send_command, "To configure intraFreq parameters" ),
	qmaCommand( "Common", __0806EDD8, oam_qcli_send_command, "To configure common parameters" ),
	qmaEndCommands()
};

/*******************************************************************************
 * Function Name  : TQ
 * Description    : To find tag TriggerQuantity (RSRP,RSRQ)
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **TQ (const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
    char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("RSRP"));
	L=qsladdstr(L,strdup("RSRQ"));
	return L;
}
/*******************************************************************************
 * Function Name  : RQ
 * Description    : To find tag ReportQuantity (SameAsTriggerQuantity,Both) 
 * Inputs         : pointer to void 
 * Outputs        : None
 * Returns        : pointer to pointer to Char8 
 ********************************************************************************/
static char **RQ (const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);    
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("SameAsTriggerQuantity"));
	L=qsladdstr(L,strdup("Both"));
	return L;
}

static const struct parameter_t __0807PC55[] = {
	qmaOptional( "FilterCoefficientRSRP", "-",qmaStandard(CHOICE,"0|1|2|3|4|5|6|7|8|9|11|13|15|17|19"), "Filtering coefficient used for RSRP measurements. Accepted Input 0/ 1/ 2/ 3/ 4/ 5/ 6/ 7/ 8/ 9/ 11/ 13/ 15/ 17/ 19" ),
	qmaOptional( "FilterCoefficientRSRQ", "-",qmaStandard(CHOICE,"0|1|2|3|4|5|6|7|8|9|11|13|15|17|19") , "Filtering coefficient used for RSRQ measurements. Accepted Input 0/ 1/ 2/ 3/ 4/ 5/ 6/ 7/ 8/ 9/ 11/ 13/ 15/ 17/ 19" ),
	qmaOptional( "A1ThresholdRSRP", "-", qmaStandard(INTEGER, "0:97"), "This is threshold to be used in EUTRA measurement report triggering condition for event a1." ),
	qmaOptional( "A1ThresholdRSRQ", "-", qmaStandard(INTEGER, "0:34"), "This is threshold to be used in EUTRA measurement report triggering condition for event a1." ),
	qmaOptional( "A2ThresholdRSRP", "-", qmaStandard(INTEGER, "0:97"), "This is threshold to be used in EUTRA measurement report triggering condition for event a2" ),
	qmaOptional( "A2ThresholdRSRQ", "-", qmaStandard(INTEGER, "0:34"), "This is threshold to be used in EUTRA measurement report triggering condition for event a2" ),
	qmaOptional( "A3Offset", "-", qmaStandard(INTEGER, "-30:30"), "This is offset to be used in evaluation of EUTRA measurement report triggering condition for event a3" ),
	qmaOptional( "ReportOnLeave", "-", qmaStandard(INTEGER, "0:1"), "This indicates whether or not the UE shall initiate the measurement reporting procedure when the leaving condition is met" ),
	qmaOptional( "A4ThresholdRSRP", "-", qmaStandard(INTEGER, "0:97"), "This is threshold to be used in EUTRA measurement report triggering condition for event a4." ),
	qmaOptional( "A4ThresholdRSRQ", "-", qmaStandard(INTEGER, "0:34"), "This is threshold to be used in EUTRA measurement report triggering condition for event a4." ),
	qmaOptional( "A5Threshold1RSRP", "-", qmaStandard(INTEGER, "0:97"), "This is threshold to be used in EUTRA measurement report triggering condition for event a5" ),
	qmaOptional( "A5Threshold1RSRQ", "-", qmaStandard(INTEGER, "0:34"), "This is threshold to be used in EUTRA measurement report triggering condition for event a5." ),
	qmaOptional( "A5Threshold2RSRP", "-", qmaStandard(INTEGER, "0:97"), "This is threshold to be used in EUTRA measurement report triggering condition for event a5." ),
	qmaOptional( "A5Threshold2RSRQ", "-", qmaStandard(INTEGER, "0:34"), "This is threshold to be used in EUTRA measurement report triggering condition for event a5." ),
	qmaOptional( "Hysteresis", "-", qmaStandard(INTEGER, "0:30"), "The hysteresis applied to entry and leave condition of a report triggering event." ),
	qmaOptional( "TimeToTrigger", "-",qmaStandard(CHOICE,"0|40|64|80|100|128|160|256|320|480|512|640|1024|1280|2560|5120"), "Time during which measurement report triggering condition needs to be met in order to trigger a measurement report. Accepted Input 0/ 40/ 64/ 80/ 100/ 128/ 160/ 256/ 320/ 480/ 512/ 640/ 1024/ 1280/ 2560/ 5120" ),
	qmaOptional( "TriggerQuantity", "-",qmaDynamic(" ",TQ) , "Quantities used to evaluate a measurement report triggering condition. Accepted Input RSRP,RSRQ" ),
	qmaOptional( "ReportQuantity", "-", qmaDynamic(" ",RQ), "Measurement quantities to be included in the measurement report. Accepted Input SameAsTriggerQuantity,Both" ),
	qmaOptional( "ReportInterval", "-",qmaStandard(CHOICE,"120|240|480|640|1024|2048|5120|10240|60000|360000|720000|1800000|3600000"), "Interval between successive measurement reports. Accepted Input 120/ 240/ 480/ 640/ 1024/ 2048/ 5120/ 10240/ 60000/ 360000/ 720000/ 1800000/ 3600000 " ),
	qmaOptional( "ReportAmount", "-",qmaStandard(CHOICE,"0|1|2|4|8|16|32|64"), "Number of times a measurement report is sent. Accepted Input 0/ 1/ 2/ 4/ 8/ 16/ 32/ 64" ),

	qmaEndParams()
};


/*******************************************************************************
 * Function Name  : MQU
 * Description    : find tag MeasQuantityUTRAFDD type (CPICH-RSCP,CPICH-EcN0)  
 * Inputs         : pointer to void 
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **MQU (const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);    
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("CPICH-RSCP"));
	L=qsladdstr(L,strdup("CPICH-EcN0"));
	return L;
}

static const struct parameter_t __0807PC56[] = {
	qmaOptional( "QoffsettUTRA", "-", qmaStandard(INTEGER, "-15:15"), "It indicates a UTRA-specific offset to be applied when evaluating triggering conditions for measurement reporting in connected mode" ),
	qmaOptional( "FilterCoefficientUTRA", "-",qmaStandard(CHOICE,"0|1|2|3|4|5|6|7|8|9|11|13|15|17|19") , "Filtering coefficient used for UTRA measurements. Accepted Input 0/ 1/ 2/ 3/ 4/ 5/ 6/ 7/ 8/ 9/ 11/ 13/ 15/ 17/ 19" ),
	qmaOptional( "MeasQuantityUTRAFDD", "-",qmaDynamic(" ",MQU) , "Measurement quantity used for UTRA measurements. Accepted Input CPICH-RSCP,CPICH-EcN0" ),
	qmaOptional( "B1ThresholdUTRARSCP", "-", qmaStandard(INTEGER, "-5:91"), "This is RSCP threshold to be used in UTRA measurement report triggering condition for event b1." ),
	qmaOptional( "B1ThresholdUTRAEcN0", "-", qmaStandard(INTEGER, "0:49"), "This is EcN0 threshold to be used in UTRA measurement report triggering condition for event b1." ),
	qmaOptional( "QoffsetGERAN", "-", qmaStandard(INTEGER, "-15:15"), "This indicates a GERAN-specific offset to be applied when evaluating triggering conditions for measurement reporting in connected mode" ),
	qmaOptional( "FilterCoefficientGERAN", "-",qmaStandard(CHOICE,"0|1|2|3|4|5|6|7|8|9|11|13|15|17|19") , "Filtering coefficient used for GERAN measurements. Accepted Input0/ 1/ 2/ 3/ 4/ 5/ 6/ 7/ 8/ 9/ 11/ 13/ 15/ 17/ 19 " ),
	qmaOptional( "B1ThresholdGERAN", "-", qmaStandard(INTEGER, "0:63"), "This is threshold to be used in GERAN measurement report triggering condition for event b1" ),
	qmaOptional( "QoffsetCDMA2000", "-", qmaStandard(INTEGER, "-15:15"), "This indicates a CDMA2000-specific offset to be applied when evaluating triggering conditions for measurement reporting in connected mode" ),
	qmaOptional( "MeasQuantityCDMA2000", "-", qmaStandard(INTEGER, "0:1"), "This is measurement quantity used for CDMA2000 measurements" ),
	qmaOptional( "B1ThresholdCDMA2000", "-", qmaStandard(INTEGER, "0:63"), "This is threshold to be used in CDMA2000 measurement report triggering condition for event b1." ),
	qmaOptional( "B2Threshold2UTRARSCP", "-", qmaStandard(INTEGER, "-5:91"), "This is RSCP threshold to be used in UTRA measurement report triggering condition for event b2. " ),
	qmaOptional( "B2Threshold2UTRAEcN0", "-", qmaStandard(INTEGER, "0:49"), "This is EcN0 threshold to be used in UTRA measurement report triggering condition for event b2." ),
	qmaOptional( "B2Threshold2GERAN", "-", qmaStandard(INTEGER, "0:63"), "This is threshold to be used in GERAN measurement report triggering condition for event b2." ),
	qmaOptional( "B2Threshold2CDMA2000", "-", qmaStandard(INTEGER, "0:63"), "This is threshold to be used in CDMA2000 measurement report triggering condition for event b2." ),
	qmaOptional( "Hysteresis", "-", qmaStandard(INTEGER, "0:30"), "This is hysteresis applied to entry and leave condition of an IRAT report triggering event." ),
	qmaOptional( "TimeToTrigger", "-", qmaStandard(CHOICE,"0|40|64|80|100|128|160|256|320|480|512|640|1024|1280|2560|5120"), "Time during which IRAT measurement report triggering condition needs to be met in order to trigger IRAT measurement report. Accepted Input 0/ 40/ 64/ 80/ 100/ 128/ 160/ 256/ 320/ 480/ 512/ 640/ 1024/ 1280/ 2560/ 5120" ),
	qmaOptional( "ReportInterval", "-",qmaStandard(CHOICE,"120|240|480|640|1024|2048|5120|10240|60000|360000|720000|1800000|3600000") , "Interval between successive IRAT measurement reports. Accepted Input 120/ 240/ 480/ 640/ 1024/ 2048/ 5120/ 10240/ 60000/ 360000/ 720000/ 1800000/ 3600000 " ),
	qmaOptional( "ReportAmount", "-", qmaStandard(CHOICE,"0|1|2|4|8|16|32|64"), "Number of times an IRAT measurement report is sent. Accepted Input 0/ 1/ 2/ 4/ 8/ 16/ 32/ 64" ),
    /* SPR 13020 START */
    qmaOptional( "B2Threshold1RSRP", "-", qmaStandard(INTEGER, "0:97"), "This is RSRP threshold to be used in UTRA measurement report triggering condition for event b2. " ),
    qmaOptional( "B2Threshold1RSRQ", "-", qmaStandard(INTEGER, "0:34"), "This is RSRQ threshold to be used in UTRA measurement report triggering condition for event b2. " ),
    /* SPR 13020 END */
	qmaEndParams()
};

static const struct command_t __0807PC57[] = {
	qmaCommand( "EUTRA", __0807PC55, oam_qcli_send_command, "To configure EUTRA Mode parameters" ),
	qmaCommand( "IRAT", __0807PC56,oam_qcli_send_command, "To configure IRAT Mode parameters" ),
	qmaEndCommands() 
};


static const struct command_t __080762F0[] = {
	qmaMode( "IdleMode", "Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility IdleMode ", __08076238, "To Configure IdleMode" ),
	qmaMode( "ConnMode", "Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility ConnMode ", __0807PC57, "To configure Connected MODE" ),

	qmaEndCommands()
};

static const struct parameter_t __08076776[] = {
	qmaOptional( "ConfigurationIndex", "-", qmaStandard(INTEGER, "0:63"), "Set index into the table defining PRACH resources within the frame" ),
	qmaOptional( "FreqOffset", "-", qmaStandard(INTEGER, "0:94"), "First physical resource block available" ),
	qmaOptional( "HighSpeedFlag", "-", qmaStandard(INTEGER, "0:1"), "Unrestricted set 0:FALSE or restricted set 1:TRUE of preambles" ),
	qmaOptional( "RootSequenceIndex", "-", qmaStandard(INTEGER, "0:837"), "Set Logical root sequence index" ),
	qmaOptional( "ZeroCorrelationZoneConfig", "-", qmaStandard(INTEGER, "0:15"), "parameter used for preamble sequence generation" ),
	qmaEndParams()
};

static const struct parameter_t __080768D0[] = {
	qmaOptional( "Pa", "-", qmaStandard(CHOICE,"-600|-477|-300|-177|0|100|200|300"), "Accepted Input -600/ -477/ -300/ -177/ 0/ 100/ 200/ 300" ),
	qmaOptional( "Pb", "-", qmaStandard(INTEGER, "0:3"), "Set PDSCH Pb parameter" ),
	qmaEndParams()
};

static const struct parameter_t __08076DA0[] = {
    /* SPR 22311 Changes - Obselete Parameter Deleted */
	qmaOptional( "DeltaPUCCHShift", "-", qmaStandard(CHOICE,"ds1|ds2|ds3"), "Accepted Input ds1,ds2,ds3" ),
	qmaOptional( "N1PUCCHAN", "-", qmaStandard(INTEGER, "0:2047"), "Used to determine resources used in transmission of PUCCH format" ),
	qmaOptional( "NRBCQI", "-", qmaStandard(INTEGER, "0:98"), "Used to determine blocked resource " ),
	qmaEndParams()
};

static const struct parameter_t __08077050[] = {
	qmaOptional( "AckNackSRSSimultaneousTransmission", "-", qmaStandard(INTEGER, "0:1"), "Defines whether a UE can simultaneously transmit SRS and HARQ ACK/NACK or not"),
	qmaOptional( "SRSBandwidthConfig", "-",qmaStandard(CHOICE,"bw0|bw1|bw2|bw3|bw4|bw5|bw6|bw7"), "Cell specific SRS Bandwidth Configuration: Accepted Input bw0,bw1,bw2,bw3,bw4,bw5,bw6,bw7" ),
	qmaOptional( "SRSEnabled", "-", qmaStandard(INTEGER, "0:1"), "Indicates if Sounding Reference Signal is Enabled(1)/Disabled(0)" ),
	qmaOptional( "SRSMaxUpPTS", "-", qmaStandard(INTEGER, "0:1"), "Defines whether reconfiguration of SRS applies to UpPTS or not"),
	qmaEndParams()
};

static const struct parameter_t __080772B8[] = {
	qmaOptional( "Alpha", "-", qmaStandard(CHOICE,"0|40|50|60|70|80|90|100"), "Specifies the value used to compute the UL UE transmit power for transmission on PUSCH, Valid Values: 0/ 40/ 50/ 60/ 70/ 80/ 90/ 100" ),
	qmaOptional( "P0NominalPUCCH", "-", qmaStandard(INTEGER, "-127:-96"), "To compute the UL UE transmit power in transmission on PUCCH, Valid Range: (-127 to -96)"),
	qmaOptional( "P0NominalPUSCH", "-", qmaStandard(INTEGER, "-126:24"), "To compute the UL UE transmit power in transmission on PUSCH in semi-persistent grants, Valid Range: (-126 to 24)" ),

	qmaEndParams()
};

static const struct parameter_t __0807VP09[] = {
	qmaOptional( "NumConsecutivePRSSubframes", "-", qmaStandard(CHOICE,"1|2|4|6"), "To configure the number of consecutive positioning sub-frames, Accepted Input 1/ 2/ 4/ 6 "),
	qmaOptional( "NumPRSResourceBlocks", "-", qmaStandard(INTEGER, "0:5"), "To configure the numeric PRS resource blocks related to PHY." ),
	qmaOptional( "PRSConfigurationIndex", "-", qmaStandard(INTEGER, "0:2399"), "To configure PRS configuration index related to PHY." ),
	qmaOptional( "X_VENDOR_PRS_ENABLE", "-", qmaStandard(INTEGER, "0:1"), "To Enable Disable PRS." ),
	qmaOptional( "X_VENDOR_MUTING_CONFIG_TWO", "-", qmaStandard(INTEGER, "0:3"), "To configure PRS Muting Config Two." ),
	qmaOptional( "X_VENDOR_MUTING_CONFIG_FOUR", "-", qmaStandard(INTEGER, "0:15"), "To configure PRS Muting Config Four." ),
	qmaOptional( "X_VENDOR_MUTING_CONFIG_EIGHT", "-", qmaStandard(INTEGER, "0:255"), "To configure PRS Muting Config Eight." ),
	qmaOptional( "X_VENDOR_MUTING_CONFIG_SIXTEEN", "-", qmaStandard(INTEGER, "0:65535"), "To configure PRS Muting Config Sixteen." ),
	qmaOptional( "X_VENDOR_PRS_TRANSMISSION_POWER", "-", qmaStandard(INTEGER, "-60:40"), "To configure PRS Transmission Power." ),
	qmaOptional( "X_VENDOR_LATITUDE_SIGN", "-", qmaStandard(INTEGER, "0:1"), "To configure Latitude Sign." ),
	qmaOptional( "X_VENDOR_DEG_OF_LATITUDE", "-", qmaStandard(INTEGER, "0:90"), "To configure Deg of Latitude ." ),
	qmaOptional( "X_VENDOR_DEG_OF_LONGITUDE", "-", qmaStandard(INTEGER, "-180:180"), "To configure Deg of Longitude ." ),
	qmaOptional( "X_VENDOR_DIR_OF_ALTITUDE", "-", qmaStandard(INTEGER, "0:1"), "To configure Dir of Altitude ." ),
	qmaOptional( "X_VENDOR_ALTITUDE", "-", qmaStandard(INTEGER, "0:32767"), "To configure Altitude ." ),
	qmaOptional( "X_VENDOR_UNCERTAINTY_SEMI_MAJOR", "-", qmaStandard(INTEGER, "0:1800000"), "To configure Uncertainity Major." ),
    qmaOptional( "X_VENDOR_UNCERTAINTY_SEMI_MINOR", "-", qmaStandard(INTEGER, "0:1800000"), "To configure Uncertainity Minor." ),
	qmaOptional( "X_VENDOR_ORIENTATION_OF_MAJOR_AXIS", "-", qmaStandard(INTEGER, "0:179"), "To configure Orientation of Major axis ." ),
	qmaOptional( "X_VENDOR_UNCERTAINTY_ALTITUDE", "-", qmaStandard(INTEGER, "0:990"), "To configure Orientaion Uncertainity Altitude." ),
	qmaOptional( "X_VENDOR_CONFIDENCE", "-", qmaStandard(INTEGER, "0:100"), "To configure Confidence ." ),

	qmaEndParams()
};

static const struct parameter_t __0807VP05[] = {
	qmaOptional( "NeighCellConfig", "-", qmaStandard(INTEGER, "0:3"), "To configure Neighbouring cell configuration"),

	qmaEndParams()
};
/*******************************************************************************
 * Function Name  : HM
 * Description    : To find tag HoppingMode(InterSubFrame,IntraAndInterSubFrame)
 * Inputs         : pointer to void 
 * Outputs        : None
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **HM (const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);    
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("InterSubFrame"));
	L=qsladdstr(L,strdup("IntraAndInterSubFrame"));
	return L;
}

static const struct parameter_t __0806VH07[] = {
	qmaOptional( "HoppingMode", "-",qmaDynamic(" ",HM) , "Indicates if hopping is 'inter-subframe' or 'intra and inter-subframe'" ), 
	qmaOptional( "HoppingOffset", "-", qmaStandard(INTEGER, "0:98"), "Specify value to compute physical resource blocks for transmission on PUSCH if uplink frequency hopping mode is enabled (0-98)" ),
	qmaOptional( "NSB", "-", qmaStandard(INTEGER, "1:4"), "Defines the number of sub-bands (1-4)" ),
	qmaEndParams()
};

static const struct parameter_t __0806VH06[] = {
	qmaOptional( "GroupHoppingEnabled", "-", qmaStandard(INTEGER, "0:1"), "Specifies whether group hopping is enabled or not(1-enabled,0-disabled)"),
	qmaOptional( "SequenceHoppingEnabled", "-", qmaStandard(INTEGER, "0:1"), "Specifies whether sequence hopping is enabled or not(1-enabled,0-disabled)" ),

	qmaEndParams()
};

static const struct command_t __080767A0[] = {
	qmaCommand( "PRACH", __08076776, oam_qcli_send_command, "To configure physical layer parameters related to PRACH" ),
	qmaCommand( "PDSCH", __080768D0, oam_qcli_send_command, "To configure physical layer parameters related to PDSCH" ),
	qmaCommand( "PUSCH", __0806VH07, oam_qcli_send_command, "To configure physical layer parameters related to PUSCH" ),
	qmaCommand( "PUSCH-ULRS", __0806VH06, oam_qcli_send_command, "To configure physical layer parameters related to ULRS." ),
	qmaCommand( "PUCCH", __08076DA0, oam_qcli_send_command, "To configure physical layer parameters related to PUCCH" ),
	qmaCommand( "SRS", __08077050, oam_qcli_send_command, "To configure physical layer parameters related to SRS" ),
	qmaCommand( "ULPowerControl", __080772B8, oam_qcli_send_command, "To configure physical layer parameters related to ULPowerControl"),
	qmaCommand( "PRS", __0807VP09, oam_qcli_send_command, "To configure physical layer parameters related to PRS" ),
    /* EMBMS Changes Start */
    qmaCommand( "MBSFN", __0807VP05, oam_qcli_send_command,
            "To configure physical layer parameters related to MBSFN." ),
    /* EMBMS Changes End */
	qmaEndCommands()
};

static const struct parameter_t __08070008[] = {
	qmaOptional( "DRXEnabled", "-", qmaStandard(INTEGER, "0:1"), "Specifies whether DRX operation is enabled or not"),
	qmaOptional( "OnDurationTimer", "-", qmaStandard(CHOICE,"1|2|3|4|5|6|8|10|20|30|40|50|60|80|100|200"), "Specifies DRX cycle during which UE actively monitors PDCCH" ),
	qmaOptional( "DRXInactivityTimer", "-", qmaStandard(CHOICE,"1|2|3|4|5|6|8|10|20|30|40|50|60|80|100|200|300|500|750|1280|1920|2560"), "Denotes the number of inactive consecutive PDCCH-subframe(s) that will activate short DRX cycle" ),
	qmaOptional( "DRXRetransmissionTimer", "-", qmaStandard(CHOICE,"1|2|4|6|8|16|24|33"), "Specifies the maximum number of consecutive PDCCH-subframe(s) UE must remain active expecting a DL retransmission" ),
	qmaOptional( "LongDRXCycle", "-", qmaStandard(CHOICE,"10|20|32|40|64|80|128|160|256|320|512|640|1024|1280|2048|2560"), "Specifies long DRX Cycle value, specified in number of sub-frames" ),
	qmaOptional( "DRXStartOffset", "-", qmaStandard(INTEGER, "0:2559"), "This is used to compute the starting frame of the DRX cycle" ),
	qmaOptional( "ShortDRXCycle", "-",qmaStandard(CHOICE,"2|5|8|10|16|20|32|40|64|80|128|160|256|320|512|640"), "Specifies short DRX Cycle value, specified in number of sub-frames" ),
	qmaOptional( "DRXShortCycleTimer", "-", qmaStandard(INTEGER, "1:16"), "This Duration of the Short DRX cycle in multiples of shortDRXCycle" ),
	qmaEndParams()

};		

static const struct parameter_t __080760A8[] = {
	qmaOptional( "MaxHARQMsg3Tx", "-", qmaStandard(INTEGER, "1:8"), "Specifies the maximum number of Msg3 HARQ transmissions by RRC" ),
	qmaOptional("MessagePowerOffsetGroupB", "-", qmaStandard(CHOICE, "-1|0|5|8|10|12|15|18"), "Specifies the threshold for preamble selection, specified in dB"),
	qmaOptional("PowerRampingStep", "-", qmaStandard(CHOICE, "0|2|4|6"), "Specifies the power increase factor between subsequent random access preamble transmissions, specified in dB"),
	qmaOptional("PreambleInitialReceivedTargetPower", "-", qmaStandard(CHOICE, "-120|-118|-116|-114|-112|-110|-108|-106|-104|-102|-100|-98|-96|-94|-92|-90"), "Specifies the value used as the baseline for computation for random access transmission power, specified in dBm"),
	qmaOptional("PreambleTransMax", "-", qmaStandard(CHOICE, "3|4|5|6|7|8|10|20|50|100|200"), "Specifies the maximum number of random access preamble transmissions"),
	qmaOptional("SizeOfRaGroupA", "-", qmaStandard(CHOICE, "4|8|12|16|20|24|28|32|36|40|44|48|52|56|60"), "Specifies the number of non-dedicated random access preambles in Random Acces Preambles group A"),
	qmaOptional("MessageSizeGroupA", "-", qmaStandard(CHOICE, "56|144|208|256"), "Specifies the threshold for preamble selection, specified in bits"),
	qmaOptional("ContentionResolutionTimer", "-", qmaStandard(CHOICE, "8|16|24|32|40|48|56|64"), "Specifies the contention resolution timer, specified in sub-frames"),
	qmaOptional("NumberOfRaPreambles", "-", qmaStandard(CHOICE, "4|8|12|16|20|24|28|32|36|40|44|48|52|56|60|64"), "Specifies the number of non-dedicated random access preambles"),
	qmaOptional("ResponseWindowSize", "-", qmaStandard(CHOICE, "2|3|4|5|6|7|8|10"), "Specifies the duration of the random access response window, specified in sub-frames"),
	qmaEndParams()  

};

static const struct command_t __08076140[] = { 
	qmaCommand( "RACH", __080760A8, oam_qcli_send_command, "To Configure RACH parameters" ),
	qmaCommand( "DRX", __08070008, oam_qcli_send_command, "To Configure DRX parameters" ),
	qmaEndCommands()
};

static const struct parameter_t __08076770[] = {
	qmaOptional("N310", "-", qmaStandard(CHOICE, "1|2|3|4|6|8|10|20"), "Number of consecutive out-of-sync indications: Accepted Input 1/2/3/4/6/8/10/20"),
	qmaOptional("N311", "-", qmaStandard(CHOICE, "1|2|3|4|5|6|8|10"), "Number of consecutive in-sync indications: Accepted Input 1/2/3/4/5/6/8/10"),
	qmaOptional("T300", "-", qmaStandard(CHOICE, "100|200|300|400|600|1000|1500|2000"), "Accepted Input 100/200/300/400/600/1000/1500/2000(in ms)"),
	qmaOptional("T301", "-", qmaStandard(CHOICE, "100|200|300|400|600|1000|1500|2000"), "Accepted Input 100/200/300/400/600/1000/1500/2000(in ms)"),
	qmaOptional("T302", "-", qmaStandard(CHOICE, "100|200|300|400|600|1000|1500|2000"), "Accepted Input 100/200/300/400/600/1000/1500/2000(in ms)"),
	qmaOptional("T310", "-", qmaStandard(CHOICE, "0|50|100|200|500|1000|2000"), "Accepted Input 0/50/100/200/500/1000/2000(in ms)"),
	qmaOptional("T311", "-", qmaStandard(CHOICE, "1000|3000|5000|10000|15000|20000|30000"), "Accepted Input 1000/3000/5000/10000/15000/20000/30000(in ms)"),
	qmaOptional("T320", "-", qmaStandard(CHOICE, "5|10|20|30|60|120|180"), "Accepted Input 5/10/20/30/60/120/180(in sec)"),
	qmaOptional("T304EUTRA", "-", qmaStandard(CHOICE, "50|100|150|200|500|1000|2000"), "Accepted Input 50/100/200/500/1000/2000(in ms)"),
	qmaOptional("T304IRAT", "-", qmaStandard(CHOICE, "100|200|500|1000|2000|4000|8000"), "Accepted Input 100/200/500/1000/2000/4000/8000(in ms)"),
	qmaEndParams()
};

static const struct parameter_t __08070B18[] = {
	qmaOptional( "DefaultConfiguration", "-", qmaStandard(INTEGER, "0:1"), "0 - Use the user provided RLC configuration for SRB, 1- use the default RLC configuration for the SRB"),
	qmaOptional("MaxRetxThreshold", "-", qmaStandard(CHOICE, "1|2|3|4|6|8|16|32"), "This parameter is used by the transmitting side of each RLC entity to limit the number of retransmissions of an AMD PDU. Accepted Input 1,2,3,4,6,8,16,32"),
	qmaOptional("PollByte", "-", qmaStandard(CHOICE, "0|25|50|75|100|125|250|375|500|750|1000|1250|1500|2000|3000"), "Counter of PDU bytes transmitted between successive poll transmissions. Accepted Input 0, 25, 50, 75, 100, 125, 250, 375,500, 750, 1000, 1250, 1500, 2000, 3000(in kBytes)"),
	qmaOptional( "PollPDU", "-", qmaStandard(CHOICE, "0|4|8|16|32|64|128|256"), "Counter of PDUs between successive poll transmissions. Accepted Input 0,4,8,16,32,64,128,256(specified in number of PDUs)"),
	qmaOptional( "TPollRetransmit", "-", qmaStandard(CHOICE, "5|10|15|20|25|30|35|40|45|50|55|60|65|70|75|80|85|90|95|100|105|110|115|120|125|130|135|140|145|150|155|160|165|170|175|180|185|190|195|200|205|210|215|220|225|230|235|240|245|250|300|350|400|450|500"), "Timer between successive poll transmissions. Accepted Input 5 to 250(multiple of 5),300, 350, 400, 450, 500(in ms)"),
	qmaOptional( "TReordering", "-", qmaStandard(CHOICE, "0|5|10|15|20|25|30|35|40|45|50|55|60|65|70|75|80|85|90|95|100|110|120|130|140|150|160|170|180|190|200"), "This timer is used by the receiving side of an RLC entity in order to detect loss of RLC PDUs at lower layer. Accepted Input: 0 to 100(multiple of 5),  110, 120, 130, 140, 150, 160, 170, 180, 190, 200(in ms)"),
	qmaOptional( "TStatusProhibit", "-", qmaStandard(CHOICE, "0|5|10|15|20|25|30|35|40|45|50|55|60|65|70|75|80|85|90|95|100|105|110|115|120|125|130|135|140|145|150|155|160|165|170|175|180|185|190|195|200|205|210|215|220|225|230|235|240|245|250|300|350|400|450|500"), "This timer is used by the receiving side of an AM RLC entity in order to prohibit transmission of a STATUS PDU. Accepted Input: 0 to 250(multiple of 5), 300, 350, 400, 450, 500(in ms)"),
	qmaEndParams()

};		


static const struct command_t __08070B48[] = {
	qmaCommand( "SRB1", __08070B18, oam_qcli_send_command, "To configure SRB1 parameters" ),
	qmaCommand( "SRB2", __08070B18, oam_qcli_send_command, "To configure SRB2 parameters" ),
	qmaEndCommands()
};		

static const struct parameter_t __08076868[] = {
	/* SPR 16407 START */
	qmaOptional( "CellBarred", "-", qmaStandard(INTEGER, "0:1"), "Set 0 - Cell-UnBarred or 1 - Cell-Barred" ),
	/* SPR 16407 END */

	qmaOptional( "BarringForEmergency", "-", qmaStandard(INTEGER, "0:1"), "Set whether the HeNB/eNB is barred (1-Barred,0-NotBarred)" ),
	qmaEndParams()
};

/* SPR 13970 FIX Start */
static const struct parameter_t  __0807PC68[] = {
	qmaKeyword("AC_BARRING_FACTOR"),
    qmaMandatory( "AC_BARRING_FACTOR", qmaStandard(INTEGER, "0:15"), "Set value between 0 to 15" ),
	qmaKeyword("AC_BARRING_TIME"),
    qmaMandatory( "AC_BARRING_TIME", qmaStandard(INTEGER, "0:7"), "Set value between 0 to 7"),
	qmaKeyword("AC_BARRING_FOR_SPECIAL_AC"),
    qmaMandatory( "AC_BARRING_FOR_SPECIAL_AC", qmaStandard(INTEGER, "0:31"), "Set value between 0 to 31" ),
    qmaEndParams()
};
/* SPR 13970 FIX End */


static const struct parameter_t __08077FB0[] = {
	qmaOptional( "FreqBandIndicator", "-", qmaStandard(INTEGER, "1:43"), "Frequency band indicator"), /* Modified from 40 to 43 for CSR 00056169 */
	qmaOptional("DLBandwidth", "-", qmaStandard(CHOICE, "n6|n15|n25|n50|n75|n100"), "Downlink transmission bandwidth, specified in number of Resource Blocks: Accepted Input n6/n15/n25/n50/n75/n100"),
	qmaOptional("ULBandwidth", "-", qmaStandard(CHOICE, "n6|n15|n25|n50|n75|n100"), "Uplink transmission bandwidth, specified in number of Resource Blocks: Accepted Input n6/n15/n25/n50/n75/n100"),
	qmaOptional( "ReferenceSignalPower", "-", qmaStandard(INTEGER, "-60:50"), "Downlink reference-signal transmit power, specified in dBm" ),
/* SPR 13024 FIX START */
	qmaOptional( "PhyCellID", "-", qmaStandard(STRING, "MAXLEN 512"),
                 "To set PhyCellID parameter" ),
/* SPR 13024 FIX END */
	qmaOptional( "PSCHPowerOffset", "-", qmaStandard(INTEGER, "-60:40"), "Power offset, specified in 0.1 dB of the Primary Synchronization Channel with respect to the Reference Signal Power" ),
	qmaOptional( "SSCHPowerOffset", "-", qmaStandard(INTEGER, "-60:40"), "Power offset, specified in 0.1 dB of the Secondary Synchronization Channel with respect to the Reference Signal Power" ),
	qmaOptional( "PBCHPowerOffset", "-", qmaStandard(INTEGER, "-60:40"), "Power offset, specified in 0.1 dB of the Physical Broadcast Channel with respect to the Reference Signal Power" ),
	qmaOptional( "EARFCNDL", "-", qmaStandard(STRING, 0), "To set EARFCNDL parameter, comma-seperated list"),
	qmaOptional( "EARFCNUL", "-", qmaStandard(STRING, 0), "To set EARFCNUL parameter, comma seperated list NOTE: To set EARFCNUL, EARFCNDL must also be set for carrier frequency selection"),

	qmaEndParams()

};

static const struct parameter_t __0807PC30[] = {
    qmaOptional( "TRelocPrep", "-", qmaStandard(INTEGER, "0:60"), "Maximum amount of time in seconds for the handover preparation procedure" ),
    qmaOptional( "TRelocOverall", "-", qmaStandard(INTEGER, "0:60"), "Maximum amount of time in seconds for protection of the overall Relocation procedure" ),
	qmaEndParams()
};

static const struct command_t __08076958[] = {
	qmaCommand( "CellRestriction", __08076868,oam_qcli_send_command, "To configure CellRestriction parameters" ),
    /* SPR 13970 FIX Start */
    qmaCommand( "CellRestriction-AC_BARRING_FOR_MO_SIGNALLING", __0807PC68, oam_qcli_send_command, "To configure CellRestriction-           AC_BARRING_FOR_MO_SIGNALLING parameters"),
    qmaCommand( "CellRestriction-AC_BARRING_FOR_MO_DATA", __0807PC68, oam_qcli_send_command, "To configure CellRestriction-                 AC_BARRING_FOR_MO_DATA parameters"),
    qmaCommand( "CellRestriction-SSAC_BARRING_FOR_MMTEL_VOICE_R9", __0807PC68, oam_qcli_send_command, "To configure CellRestriction-        SSAC_BARRING_FOR_MMTEL_VOICE_R9 parameters"),
    qmaCommand( "CellRestriction-SSAC_BARRING_FOR_MMTEL_VIDEO_R9", __0807PC68, oam_qcli_send_command, "To configure CellRestriction-        SSAC_BARRING_FOR_MMTEL_VIDEO_R9 parameters"),
    qmaCommand( "CellRestriction-AC_BARRING_FOR_CSFB_R10", __0807PC68, oam_qcli_send_command, "To configure CellRestriction-                AC_BARRING_FOR_CSFB_R10 parameters"),
    /* SPR 13970 FIX End */

	qmaCommand( "S1AP", __0807PC30,oam_qcli_send_command , "To configure S1AP parameters" ),
	qmaMode( "Mobility", "Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility ", __080762F0, "To configure UE's Idle Mode mobility parameters" ),
	qmaMode( "PHY", "Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY ", __080767A0, "To configure physical layer level configuration parameters" ),
	qmaMode( "MAC", "Config InternetGatewayDevice Services FAPService CellConfig LTE RAN MAC ", __08076140, "To Configure MAC layer parameters" ),
	qmaCommand( "RRCTimers", __08076770, oam_qcli_send_command, "To configure RRC timers and constants" ),
	qmaCommand( "RF", __08077FB0, oam_qcli_send_command, "To configure RF parameters" ),
	qmaMode( "RLC", "Config InternetGatewayDevice Services FAPService CellConfig LTE RAN RLC ", __08070B48, "To configure RLC parameters" ),
	qmaEndCommands()
};

/* SPR_19279_START */
static const struct parameter_t __0807VH04[] = {
	qmaOptional( "EAID", "-", qmaStandard(INTEGER, "000:16777216"), "Set EAID value for EPC(Accepted Value: 000-16777216)." ),
	qmaEndParams()
};

static const struct parameter_t __0807VH05[] = {
	qmaOptional( "TAC", "-", qmaStandard(INTEGER, "0:65535"), "Set TAC value for EPC(Accepted Value: 0-65535)." ),
	qmaEndParams()
};

static const struct parameter_t __add_plmnlist_info[] = {
    qmaKeyword("Enable"),
    qmaMandatory("Enable", qmaStandard(INTEGER, "0:1"),
                              "Possible values: 0/1"),
    qmaKeyword("IsPrimary"),
    qmaMandatory("IsPrimary", qmaStandard(STRING, 0),
                           "Possible values: 0/1"),

    qmaKeyword("PLMNID"),
    qmaMandatory("PLMNID", qmaStandard(STRING, "MAXLEN 6"),
                            "Possible values: 5/6 digit value"),

    qmaKeyword("CellReservedForOperatorUse"),
    qmaMandatory("CellReservedForOperatorUse", qmaStandard(STRING, 0),
                           "Possible values: 0/1"),

    qmaEndParams()
};

static const struct parameter_t __update_plmnlist_info[] = {
    qmaKeyword("InstanceId"),
    qmaMandatory("InstanceId", qmaStandard(INTEGER, "1:6"),
            "Instance Number: Mandatory: 1-6"),

    qmaOptional("Enable", "-",qmaStandard(INTEGER, "0:1"),
                              "Possible values: 0/1"),
    qmaOptional("IsPrimary", "-",qmaStandard(STRING, 0),
                           "Possible values: 0/1"),

    qmaOptional("PLMNID", "-",qmaStandard(STRING, "MAXLEN 6"),
                            "Possible values: 5/6 digit value"),

    qmaOptional("CellReservedForOperatorUse", "-",qmaStandard(STRING, 0),
                           "Possible values: 0/1"),

    qmaEndParams()
};

static const struct parameter_t __delete_plmnlist_info[] = {
    qmaMandatory( "InstanceId", qmaStandard(INTEGER, "1:6"), 
                  "InstanceId : Possible values : 1/6 - Instance to be deleted"),
    qmaEndParams()
};

static const struct parameter_t __retrieve_plmnlist_info[] = {
    qmaOptional( "", "1",qmaStandard(INTEGER, "0"), ""),
    qmaEndParams()
};

static const struct command_t __plmnlist_add[] = {
    qmaCommand("INFO", __add_plmnlist_info,
                      oam_qcli_send_epc_plmnlist_add_cell_command, "EPC PLMNList Parameters"),
    qmaEndCommands()
};

static const struct command_t __plmnlist_update[] = {
    qmaCommand("INFO", __update_plmnlist_info, 
                      qcli_oam_epc_plmnlist_update_cmd_handler, "EPC PLMNList Parameters"),
    qmaEndCommands()
};

static const struct command_t __plmnlist_delete[] = {
    qmaCommand("INFO",__delete_plmnlist_info, 
                       qcli_oam_epc_plmnlist_update_cmd_handler, "EPC PLMNList Parameters"),
    qmaEndCommands()
};

static const struct command_t _plmnlist_retrieve[] = {
    qmaCommand("INFO", __retrieve_plmnlist_info, oam_qcli_send_command, "EPC PLMNList Parameters"),
    qmaEndCommands()
};

static const struct command_t epc_plmnlist_mode[] = {
    qmaMode("ADD", "Config InternetGatewayDevice Services FAPService CellConfig LTE EPC PLMNList ADD", __plmnlist_add,
            "Add plmnlist"),
    qmaMode("UPDATE", "Config InternetGatewayDevice Services FAPService CellConfig LTE EPC PLMNList UPDATE", __plmnlist_update,
            "Modify plmnlist"),
    qmaMode("DELETE", "Config InternetGatewayDevice Services FAPService CellConfig LTE EPC PLMNList DELETE" , __plmnlist_delete,
            "Delete plmnlist"),
    qmaMode("RETRIEVE", "Config InternetGatewayDevice Services FAPService CellConfig LTE EPC PLMNList RETRIEVE", _plmnlist_retrieve,
            "Retrieve plmnlist"),
    qmaEndCommands()
};

static const struct command_t __0807VH03[] = {
    qmaMode( "PLMNList", "Config InternetGatewayDevice Services FAPService CellConfig LTE EPC PLMNList", epc_plmnlist_mode, "Get/Set EPC PLMNList parameters"),
	qmaCommand( "EAID_PARAM", __0807VH04, oam_qcli_send_command, "To configure EAID" ),
	qmaCommand( "TAC_PARAM", __0807VH05, oam_qcli_send_command, "To configure TAC" ),
    qmaEndCommands()
};
/* SPR_19279_END */

/* SPR 13501 FIX START */
static const struct parameter_t __0807VD03[] = {
        qmaMandatory( 0, qmaStandard(INTEGER, "0:1"), "NumofCellEdgeRegion" ),
        qmaMandatory( 0, qmaStandard(INTEGER, "1:2"), "NumofCellCenterRegion" ),
     qmaMandatory(0,  qmaStandard(STRING, "MAXLEN 10"), "StartRBCellCenter (0:99): Comma seperated list of values"
                                                         "in case NumofCellCenterRegion is more than 1"),
     qmaMandatory(0,  qmaStandard(STRING, "MAXLEN 10"), "NumOfRBCellCenter (0:100): Comma seperated list of values"
                                                        "in case NumofCellCenterRegion is more than 1"),
        qmaOptional( "StartRBCellEdge", "-",qmaStandard(INTEGER,"0:99"),"Mandatory in case NumofCellEdgeRegion is non-zero" ),
        qmaOptional( "NumOfRBCellEdge", "-",qmaStandard(INTEGER,"0:100"), "Mandatory in case NumofCellEdgeRegion is non-zero" ),
        qmaEndParams()
};
static const struct command_t __0807VD02[] = {

    qmaCommand( "DLResourcePartitionInfo", __0807VD03, oam_qcli_send_command, "To set DLResourcePartitionInfo parameters"),
    qmaCommand( "ULResourcePartitionInfo", __0807VD03, oam_qcli_send_command, "To set ULResourcePartitionInfo  parameters" ),

    qmaEndCommands()
};

static const struct command_t __0807VD01[] = {
    qmaMode( "DynamicICICParams", "Config InternetGatewayDevice Services FAPService CellConfig LTE OperatorInfo DynamicICICParams",
                                   __0807VD02, "To configure DynamicICICParams Parameters" ),
    qmaEndCommands()
};
/* SPR 13501 FIX END */

    /* SPR 14295 FIX START */
/* LTE_OAM_ENHANCEMENT_CA_CHANGES*/

static const struct parameter_t config_ca_general_params_cmd[] = {
    qmaOptional("isCAEligible", "-", qmaStandard(INTEGER, "0:1"),
                "It tells whether Cell is CA capable or not."),

    qmaOptional("minNumUEThruputReport", "-", qmaStandard(INTEGER, "1:20"),
                "The number of throughput reports to be considered before "
                "considering UE for CA."),

    qmaOptional("applicableBitrateDL", "-",
                qmaStandard(INTEGER, "5:18446744073709551615"),
                "Indicate at which rate the UE should be eligible for CA."
                "Value is in bps."),

    qmaOptional("isMASA", "-", qmaStandard(INTEGER, "0:1"),
                "Indicates whether scell addition is done based on UE "
               "measurements/only on the OAM configured eligibility as Scell."),

    qmaOptional("ScellDeactTimer", "-",
                qmaStandard(CHOICE, "RF2|RF4|RF8|RF16|RF32|RF64|RF128|RFinf"),
                "Indicate the deactivation time for the Scell. This value "
              "should be greater than the value choosen for DLDeactCMDThresh."),

    qmaEndParams()
};

static const struct parameter_t config_ca_mac_sched_params_cmd[] = {
    qmaOptional("loadCalcWindowSize", "-", qmaStandard(INTEGER, "100:5000"),
                "Indicates the size of the sliding window to be used to "
                "maintain the DL PRB usage statistics for using the load "
                "balancing criterion."),

    qmaOptional("loadDisparityThresh", "-", qmaStandard(INTEGER, "0:100"),
                "Indicates whether the load difference is above threshold "
                "value before using the load balacing criterion in "
                "distributing the queueLoad between different serving cells "
                "of the UE."),

    qmaOptional("MCSDiffThresh", "-", qmaStandard(INTEGER, "0:28"),
                "Indicates the minimum mcs index difference between the mcs "
                "index values of the serving cells of the UE after which "
                "preference to one cell (cell with higher mcs) will be given."),
    qmaOptional("PRBUsageWatermark", "-", qmaStandard(INTEGER, "0:100"),
                "Indicates maximum value to which a cell is to be loaded, "
                "when scheduling extra RBs on a less loaded cell for load "
                "balancing."),

    qmaOptional("DLDeactCMDThresh", "-", qmaStandard(INTEGER, "0:255"),
                "Indicates the number of TTIs.This value should be less "
                "than the value choosen for field ScellDeactTimer."),


    qmaOptional("DLDeactMCSLowThresh", "-", qmaStandard(INTEGER, "0:28"),
                "The trigger to deactivate the scell if the wideband MCS is "
                "below the threashold value."),
    qmaOptional("DLQLoadUPThresh", "-", qmaStandard(INTEGER, "0:255"),
                "This is used to activate the Scell if the queueLoad is "
                "above upThreshold."),

    qmaOptional("DLQLoadLowThresh", "-", qmaStandard(INTEGER, "0:255"),
                "This is used to deactivate the Scell if the queueLoad is "
                "below lowThreshold.." ),

    qmaOptional("ActDeactTriggerCountThresh", "-", qmaStandard(INTEGER, "0:255"),
                "Indicates the number of times the threshold limits has hit."),

    qmaEndParams()
};
/*LTE_OAM_ENHANCEMENT_CA_CHANGES */
    /* SPR 14295 FIX END */

/* eICIC_PHASE_1_2_CHANGES_START */

/*SPR 15028 start*/
static const struct parameter_t __abs_info[] = {
    qmaOptional( "NumAbsReport", "-", qmaStandard(INTEGER, "0:1440"), "Number of ABS status report received from victim eNB after which aggressor will take action to increase or decrease the ABS pattern." ),
    qmaOptional( "AbsUsageLowThreshold", "-", qmaStandard(INTEGER, "1:100"), "It indicates if the abs usage reported in resource status update message from victim is less than this value then ABS pattern needs to be decreased by one subframe" ),
    qmaOptional( "AbsUsageHighThreshold", "-", qmaStandard(INTEGER, "1:100"), "It indicates if the abs usage reported in resource status update message from victim is greater than this value then ABS pattern needs to be increased by one subframe" ),

    qmaEndParams()

};

static const struct parameter_t __abs_pattern[] = {
    qmaOptional( "LowLoadAbsPattern", "-", qmaStandard(STRING,"MAXLEN 70"), "Used when cell is configured as aggressor" ),  
    qmaOptional( "VictimAbsPattern", "-", qmaStandard(STRING,"MAXLEN 70"), "Used when cell is configured as victim" ),  

    qmaOptional( "VictimMeasSubset", "-", qmaStandard(STRING,"MAXLEN 70"), "This pattern is used for Measurement configuration" ),

    qmaEndParams()

};

static const struct command_t __abs_info_params[] = {

    qmaCommand( "ABS_INFO", __abs_info,oam_qcli_send_command, "To configure abs_info" ),
    qmaCommand( "ABS_PATTERN", __abs_pattern,oam_qcli_send_command, "To configure abs pattern" ),

    qmaEndCommands()
};

static const struct parameter_t __eicic_timer_info[] = {
    qmaOptional( "MacEicicMeasReportPeriodicity", "-", qmaStandard(INTEGER, "20:10000"), "Periodicity at which MAC will send periodic eICIC measurement to RRM" ),
    qmaOptional( "AggressorSelTimer", "-", qmaStandard(INTEGER, "30:7200"), "Periodic timer that activate eICIC algorithm on the victim UE" ),
    qmaOptional( "LoadInformationGuardTimer", "-", qmaStandard(INTEGER, "1:15"), "Guard timer to consolidate load information sent from different aggresssors" ),
    qmaOptional( "LoadInformationCollationTimer", "-", qmaStandard(INTEGER, "0:60"), "timer interval for which RRM shall wait for collation of subsequent load information message in which ABS Information update is provided by aggressor without sending invoke by victim" ),

    qmaEndParams()

};
static const struct parameter_t __eicic_params[] = {

    qmaOptional( "eICICProvisionType", "-", qmaStandard(INTEGER, "0:3"), "Parameter is used for Activation/Deactivation of eICIC feature" ),
    qmaOptional( "eligible_ue", "-", qmaStandard(INTEGER, "1:100"), "Percentage of UEs to be selected for the eICIC out of total UEs experiencing interference"),
    qmaOptional( "DlSinrThreshLowMark", "-", qmaStandard(INTEGER, "0:255"), "Value below a UE is being identified as victim   UE" ),
    qmaOptional( "DlSinrThreshHighMark", "-", qmaStandard(INTEGER, "0:255"), "DL SINR value above which a UE is being         identified as non victim UE" ),

    qmaEndParams()

};
/*SPR 15028 end*/

static const struct command_t __eicic_fill[] = {
    /*SPR 15028 start*/

    qmaCommand( "eICICParams", __eicic_params, oam_qcli_send_command, "To configure eicic_provision type,eligible_ue,dl_sinr values" ),
    qmaMode( "AbsInfoParams", "Config InternetGatewayDevice Services FAPService CellConfig X_VENDOR_eICIC_PARAMS ABS INFO PARAMS", __abs_info_params, "To configure abs thresholds and abs patterns"),
    qmaCommand( "eICICTimers", __eicic_timer_info, oam_qcli_send_command, "To Configure eICIC timers" ),
    qmaEndCommands()
        /*SPR 15028 end*/

};
/* eICIC_PHASE_1_2_CHANGES_END */
static const struct command_t __080769E0[] = {
	qmaMode( "RAN", "Config InternetGatewayDevice Services FAPService CellConfig LTE RAN ", __08076958, "To configure RAN parameters" ),
    /* SPR_19279_START */
	qmaMode( "EPC", "Config InternetGatewayDevice Services FAPService CellConfig LTE EPC", __0807VH03, "To configure EPC parameters" ),
    /* SPR_19279_END */
    /* SPR 13501 FIX START */
   qmaMode( "OperatorInfo", "Config InternetGatewayDevice Services FAPService CellConfig LTE OperatorInfo ", __0807VD01, "To configure OperatorInfoParameters" ),
    /* SPR 13501 FIX END */
	qmaEndCommands()
};


    /* SPR 14295 FIX START */
static const struct command_t config_ca_mode[] = {
	qmaCommand("GeneralParams", config_ca_general_params_cmd,
               oam_qcli_send_command,
               "Configure CA general parameters"),

	qmaCommand("MACSchedParams", config_ca_mac_sched_params_cmd,
               oam_qcli_send_command,
               "Configure CA MAC scheduler specific parameters" ),
	qmaEndCommands()
};
    /* SPR 14295 FIX END */


static const struct command_t __08076A78[] = {
	qmaMode( "LTE", "Config InternetGatewayDevice Services FAPService CellConfig LTE ", __080769E0, "To configure LTE parameters" ),

    /* SPR 14295 FIX START */
   /*LTE_OAM_ENHANCEMENT_CA_CHANGES*/
	qmaMode("CarrierAggregation", "Config InternetGatewayDevice Services "
                                  "FAPService CellConfig CarrierAggregation ",
                                  config_ca_mode, "Configure CA parameters" ),
   /*LTE_OAM_ENHANCEMENT_CA_CHANGES */
    /* SPR 14295 FIX END */
    /* eICIC_PHASE_1_2_CHANGES_START */

    /*SPR 15028 start*/
        qmaMode( "X_VENDOR_eICIC_PARAMS", "Config InternetGatewayDevice Services FAPService CellConfig X_VENDOR_eICIC_PARAMS",    __eicic_fill,"To configure eICIC  params" ),

    /*SPR 15028 end*/
    /* eICIC_PHASE_1_2_CHANGES_END */

    qmaEndCommands()
};

/*******************************************************************************
 * Function Name  : AM
 * Description    : To find tag AccessMode (OpenAccess,ClosedAccess,HybridAccess) 
 * Inputs         : pointer to void 
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **AM (const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);    
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("OpenAccess"));
	L=qsladdstr(L,strdup("ClosedAccess"));
	L=qsladdstr(L,strdup("HybridAccess"));
	return L;
}

static const struct parameter_t __0807VP08[] = {
	qmaOptional( "AccessMode", "-", qmaDynamic(" ",AM), "Set the type of access mode the FAP operates in" ),
	qmaOptional( "CSGID", "-", qmaStandard(INTEGER, "0:134217727"), "Set the Closed Subscriber Group of the Access Control List" ),
	qmaOptional( "HNBName", "-", qmaStandard(STRING, 0), "Set the Home NodeB name" ),
	qmaOptional( "MaxCSGMembers", "-", qmaStandard(INTEGER, "0:255"), "Set the Maximum number of concurrent CSG member UEs allowed at a hybrid or closed cell" ),
	qmaOptional( "MaxNonCSGMembers", "-", qmaStandard(INTEGER, "0:255"), "Set the Maximum number of concurerent non-CSG member UEs allowed at a hybrid cell" ),
	qmaEndParams()
};


static const struct command_t __0807VP07[] = {
	qmaCommand( "LTE", __0807VP08, oam_qcli_send_command, "To configure LTE Access management parameters" ),
	qmaEndCommands()
};

static const struct parameter_t __0807AJGW[] = {
	qmaOptional( "SecGWServer1", "-", qmaStandard(STRING, 0), "First SecGW the FAP attempts to establish connection with. Either hostname or IPaddress. Possible values : max 64 characters"),
	qmaOptional( "SecGWServer2", "-", qmaStandard(STRING, 0), "Second SecGW the FAP attempts to establish connection with. Either hostname or IPaddress. Possible values : max 64 characters"),
	qmaOptional( "SecGWServer3", "-", qmaStandard(STRING, 0), "Third SecGW the FAP attempts to establish connection with. Either hostname or IPaddress. Possible values : max 64 characters"),
	qmaEndParams()
};

static const struct parameter_t __0808FALT[] = {
	qmaOptional( "AdminState", "-", qmaStandard(INTEGER, "0:1"), "Lock or unlock the LTE FAP. Possible values : 0:to lock cell/ 1:to unlock cell"),
	qmaEndParams()
};

static const struct command_t __0807AJLT[] = {
	qmaCommand( "Gateway", __0807AJGW, oam_qcli_send_command, "To configure LTE Gateway parameters" ),
	qmaCommand( "eNBState", __0808FALT, oam_qcli_send_command, "To configure eNB Admin State" ),
	qmaEndCommands()
};



static const struct command_t __0807AJFC[] = {
	qmaMode( "LTE", "Config InternetGatewayDevice Services FAPService FAPControl LTE ", __0807AJLT, "To configure the FAP Control LTE parameters" ),
	qmaEndCommands()
};

static const struct command_t __08076B18[] = {
	qmaMode( "FAPControl", "Config InternetGatewayDevice Services FAPService FAPControl ", __0807AJFC, "To configure FAP Control parameters for LTE" ),
	qmaMode( "AccessMgmt", "Config InternetGatewayDevice Services FAPService AccessMgmt ", __0807VP07, "To configure Access Management parameters for LTE" ),
	qmaMode( "CellConfig", "Config InternetGatewayDevice Services FAPService CellConfig ", __08076A78, "To configure the FAP" ),
	qmaEndCommands()
};

static const struct command_t qcli_clean_mode[] = {
	qmaCommandNoParam("AlarmHistory", oam_qcli_send_command_no_param,
			"To clear alarm history"),
	qmaEndCommands()
};

#ifdef CLI_UNUSED
static const struct parameter_t __0807SS81[] = {
	qmaOptional( "Enable", "-", qmaStandard(INTEGER, "0:1"), "To enable/disable Cryptoprofile " ),
/* SPR_12373_CHANGES */
/* Redundant Code Removed */
/* SPR_12373_CHANGES */
	qmaOptional( "IPsecRekeyLifetimeTime", "-", qmaStandard(INTEGER, "0:65535"), "IPsec SA rekey timeout in Kilobytes" ),
	qmaOptional( "AuthMethod", "-", qmaStandard(INTEGER, "0:256"), "Specifies the Security mechanism and set of credentials used by the device to authenticate itself " ),
	qmaOptional( "DPDTimer", "-", qmaStandard(INTEGER, "0:256"), "DPD timeout in seconds" ),
	qmaOptional( "NATTKeepaliveTimer", "-", qmaStandard(INTEGER, "0:256"), "NAT-T keepalive timeout in seconds" ),
	qmaEndParams()
};

static const struct parameter_t __0807DK81[] = {
	qmaOptional( "Enable", "-", qmaStandard(INTEGER, "0:1"), "To enable/disable VirtualInterface" ),
	qmaOptional( "DSCPMarkPolicy", "-", qmaStandard(INTEGER, "-2:65535"), "DSCP to mark the outer IP header for traffic that is associated with this virtual interface" ),
	qmaEndParams()
};
static const struct command_t __0806E2Z3[] = {
	qmaCommand( "CryptoProfile", __0807SS81, oam_qcli_send_command, "CryptoProfile parameters" ),
	qmaCommand( "VirtualInterface", __0807DK81, oam_qcli_send_command, "VirtualInterface parameters" ),
	qmaEndCommands()
};
static const struct parameter_t __0808MGM[] = {
	qmaOptional( "URL", "-", qmaStandard(STRING, 0), "URL for the CPE to connect to the ACS using the CPE WAN Management Protocol. Possible values : max 64 characters"),
	qmaEndParams()
};
/*******************************************************************************
 * Function Name  : time_enable
 * Description    : To find tag time_enable (0,1) 
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **time_enable(const void *unused)
{
	char **L = NULL; CLI_QSLALLOC(L);
    qcliHandleUnusedParameter(unused);
	*L = 0;
	L=qsladdstr(L,strdup("0"));
	L=qsladdstr(L,strdup("1"));
	return L;
}

/* SPR_12373_CHANGES */
/* Redundant Code Removed */
/* SPR_12373_CHANGES */
#endif
/*******************************************************************************
 * Function Name  : type
 * Description    : To find tag type (eNodeB_Data_Model_TR_196_based.xml,Proprietary_eNodeB_Data_Model.xml)
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/

static char **type(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("eNodeB_Data_Model_TR_196_based.xml"));
	L=qsladdstr(L,strdup("Proprietary_eNodeB_Data_Model.xml"));
	L=qsladdstr(L,strdup("BOTH"));
	return L;
}

static const struct parameter_t __0807SS01[] = {
	qmaMandatory("0", qmaDynamic("",type), "Persistance of XML Configuration Files (Created at path : cfg folder)" ),
	qmaEndParams()
};

static const struct parameter_t __08076BLD[] = {
	qmaMandatory( "Percentage", qmaStandard(INTEGER, "1:100"), "accepted input: 1-100" ),
	qmaMandatory("Action",qmaStandard(INTEGER,"1:1024") , "\nAction taken on reaching the load level, possible values can be one of the combination of the following options:\n\t1\tNo Action;\n\t2\tHandOver;\n\t4\tAccess Class Barring;\n\t8\tStop Admission;\n\t16\tRelease UE with Carrier Redirect;\n\t32\tRelease UE;\n\t64\tActive Queue Management;\n\t128\tExplicit Congestion Control-UL;\n\t256\tExplicit Congestion Control-DL;\n\t512\tExplicit Congestion Control-Bidirectional;"),
    /* eICIC_PHASE_1_2_CHANGES_START */
	qmaOptional( "eligible_ue", "-",qmaStandard(INTEGER,"1:100"), "eligible_ue Percentage of UEs to be selected for the eICIC out of UEs experiencing interference" ),
	qmaOptional( "abs_pattern", "-",qmaStandard(STRING,"MAXLEN 70"), "abs_pattern in eICIC Used for providing ABS pattern to the victim node in case node is acting as aggressor node." ),
    /* eICIC_PHASE_1_2_CHANGES_END */
	qmaEndParams()
};

static const struct parameter_t __08077ULD[] = {
	qmaKeyword("ReportingInterval"),
	qmaMandatory( "ReportingInterval", qmaStandard(CHOICE, "0|1|2|5|10"), "Reporting interval in seconds, accepted values: 0, 1, 2, 5, 10; 0 to DISABLE" ),
	qmaEndParams()
};

static const struct parameter_t __08078ZLD[] = {
	qmaKeyword("ReportingInterval"),
	qmaMandatory( "ReportingInterval", qmaStandard(INTEGER, "0:300"), "Reporting interval in seconds, accepted values: 0-300, 0 to DISABLE" ),
	qmaEndParams()
};

static const struct parameter_t __08076BLO[] = {
    qmaMandatory( "Percentage", qmaStandard(INTEGER, "1:100"), "Percentage for load level, Accepted Range: 1-100" ),
    qmaMandatory("Action",qmaStandard(INTEGER,"1:1024") , "\nAction taken on reaching the load level, possible values can be one of the combination of the following options:\n\t2\tHandOver;\n\t4\tAccess Class Barring;\n\t16\tRelease UE with Carrier Redirect;\n\t64\tActive Queue Management;\n\t128\tExplicit Congestion Control-UL;\n\t256\tExplicit Congestion Control-DL;\n\t512\tExplicit Congestion Control-Bidirectional;\n. For OverLoad Level, 'Stop Admission' and 'Release UE' actions are mandatory and will be set automatically."),
    /* eICIC_PHASE_1_2_CHANGES_START */
	qmaOptional( "eligible_ue", "-",qmaStandard(INTEGER,"1:100"), "eligible_ue Percentage of UEs to be selected for the eICIC out of UEs experiencing interference" ),
	qmaOptional( "abs_pattern", "-",qmaStandard(STRING,"MAXLEN 70"), "abs_pattern in eICIC Used for providing ABS pattern to the victim node in case node is acting as aggressor node." ),
    /* eICIC_PHASE_1_2_CHANGES_END */
    qmaEndParams()
};
static const struct command_t __08076BLR[] = {

	qmaCommand( "ServingCellReport", __08078ZLD, oam_qcli_send_command, "To enable / disable serving cell load reporting and set the reporting interval" ),
	qmaCommand( "NeighborCellReport", __08077ULD, oam_qcli_send_command, "To enable / disable neighbor cell load reporting and set the reporting interval" ),
    qmaCommand( "OverLoadLevel", __08076BLO, oam_qcli_send_command, "Configure load percentage for Overload Level" ),
	qmaCommand( "HighLoadLevel", __08076BLD, oam_qcli_send_command, "Configure load percentage for Highload Level" ),
	qmaCommand( "MidLoadLevel", __08076BLD, oam_qcli_send_command, "Configure load percentage for Midload Level" ),
	qmaEndCommands() 

};

static const struct command_t __0806E372[] = {
	qmaMode( "FAPService", "Config InternetGatewayDevice Services FAPService ", __08076B18, "To configure Femto Access Point (FAP) Service Object"),
	qmaEndCommands()
};


static const struct command_t __0806E371[] = {
	qmaMode( "Services", "Config InternetGatewayDevice Services", __0806E372, "Services Configuration" ),
#ifdef CLI_UNUSED
	qmaCommand( "ManagementServer", __0808MGM, oam_qcli_send_command, "To set Management server URL" ),
/* SPR_12373_CHANGES */
/* Redundant Code Removed */
/* SPR_12373_CHANGES */
	qmaMode( "FAP", "Config InternetGatewayDevice FAP", __0806EFAP, "FAP Configuration" ),
#endif

	qmaEndCommands()
};

static const struct parameter_t __0806EYT2[] = {
	qmaOptional( "Enable", "-",qmaStandard(INTEGER,"0:1"), "To Enable/Disable FTP, 1: Enable, 0:Disable" ),
    qmaOptional( "FTPServerURL", "-", qmaStandard(STRING, 0), "To update the FTP URL(ftp:IPAddress)" ),
	qmaOptional( "FTPServerUserName", "-", qmaStandard(STRING, 0), "To update the FTP Username" ),
	qmaOptional( "FTPServerPassword", "-", qmaStandard(STRING, 0), "To update the FTP Password" ),
	qmaOptional( "FilePath", "-", qmaStandard(STRING, 0), "To update the local download/upload path" ),

	qmaEndParams()
};

static const struct command_t __0806E370[] = {
	qmaMode( "InternetGatewayDevice", "Config InternetGatewayDevice ", __0806E371, "To show InternetGatewayDevice parameters" ),
	qmaMode( "Kpi", "Config Kpi ", __0806E2D0, "To configure KPI" ),
	qmaCommand( "SaveConfig", __0807SS01, oam_qcli_send_command, "Save Current System configuration" ),
	qmaMode( "LoadReporting", "Config LoadReporting", __08076BLR, "To Configure Load Reporting"),
	qmaCommand( "nmsFTPServerConfig", __0806EYT2, oam_qcli_send_command, "To update FTP Parameters" ),
	qmaEndCommands()
};

static const struct command_t __0805E370[] = {
	qmaMode( "UE", "RRM UE", __0816E2D0, "To Release UE " ),
	qmaEndCommands()
};


static const struct command_t __0806EE68[] = {
	qmaCommandNoParam( "All", oam_set_log_cat_l2_mac_all, "To set the desired log category" ),
	qmaCommandNoParam( "l2_sys_fail", oam_l2_sys_fail, "L2 layer system Failure" ),
	qmaCommandNoParam( "mac_l1_inf", oam_mac_l1_inf, "Mac L1 Information" ),
	qmaCommandNoParam( "mac_ul_harq", oam_mac_ul_harq, "Mac ul Harq" ),

	qmaCommandNoParam( "mac_rach", oam_mac_rach, "Mac Rach" ),
	/* 55317 Start */
	qmaCommandNoParam( "mac_rrc_inf", oam_mac_rrc_inf, "Mac RRC Information" ),
	qmaCommandNoParam( "mac_oam_inf", oam_mac_oam_inf, "Mac OAM Information" ),
/*CSR:00055315_FIX_START*/
    qmaCommandNoParam( "mac_rrm_inf", oam_mac_rrm_inf, "Mac RRM Information" ),
/*CSR:00055315_FIX_END*/    
	qmaCommandNoParam( "mac_dl_strategy", oam_mac_dl_strategy, "Mac dl strategy" ),
	qmaCommandNoParam( "mac_ul_strategy", oam_mac_ul_strategy, "Mac ul strategy" ),
	qmaCommandNoParam( "mac_pdsch", oam_mac_pdsch, "Mac pdsch" ),
	qmaCommandNoParam( "mac_pdcch", oam_mac_pdcch, "Mac pdcch" ),
	qmaCommandNoParam( "mac_pusch", oam_mac_pusch, "Mac pusch" ),
	qmaCommandNoParam( "mac_pucch", oam_mac_pucch, "Mac pucch" ),
	qmaCommandNoParam( "mac_bcch", oam_mac_bcch, "Mac bcch" ),
	qmaCommandNoParam( "mac_pcch", oam_mac_pcch, "Mac pcch" ),
	qmaCommandNoParam( "mac_ccch", oam_mac_ccch, "Mac ccch" ),
	/* 55317 End */
	qmaCommandNoParam( "mac_dl_harq", oam_mac_dl_harq, "Mac dl harq" ),
	/* 55317 Start */
	qmaCommandNoParam( "mac_cqi", oam_mac_cqi, "Mac cqi" ),
	qmaCommandNoParam( "mac_srs", oam_mac_srs, "Mac srs" ),
	qmaCommandNoParam( "mac_tb_ul", oam_mac_tb_ul, "Mac tb ul" ),
	qmaCommandNoParam( "mac_tb_dl", oam_mac_tb_dl, "Mac tb dl" ),
	qmaCommandNoParam( "mac_dl_pc", oam_mac_dl_pc, "Mac dl pc" ),
	qmaCommandNoParam( "mac_ul_pc", oam_mac_ul_pc, "Mac ul pc" ),
	qmaCommandNoParam( "mac_drx", oam_mac_drx, "Mac drx" ),
	qmaCommandNoParam( "mac_meas_gap", oam_mac_meas_gap, "Mac meas gap" ),
	qmaCommandNoParam( "mac_meas_handler", oam_mac_meas_handler, "Mac meas handler" ),
	qmaCommandNoParam( "mac_ul_csi", oam_mac_ul_csi, "Mac ul csi" ),
	qmaCommandNoParam( "mac_mux", oam_mac_mux, "Mac mux" ),
	qmaCommandNoParam( "mac_dl_sps_strategy", oam_mac_dl_sps_strategy, "Mac dl sps strategy" ),
	qmaCommandNoParam( "mac_ul_sps_strategy", oam_mac_ul_sps_strategy, "Mac ul sps strategy" ),
	qmaCommandNoParam( "mac_scheduler", oam_mac_scheduler, "Mac scheduler" ),
	qmaCommandNoParam( "mac_atb", oam_mac_atb, "Mac atb" ),
	qmaCommandNoParam( "mac_ta", oam_mac_ta, "Mac ta" ),
	/* 55317 End */
	qmaEndCommands()
};

/* 55317 Start */
static const struct command_t __0806EE69[] = {
	qmaCommandNoParam( "All", oam_rlc_log_cat_enable_all, "Rlc all" ),
	qmaCommandNoParam( "rlc_am", oam_rlc_am, "Rlc am" ),
	qmaCommandNoParam( "rlc_um", oam_rlc_um, "Rlc um" ),
	qmaCommandNoParam( "rlc_tm", oam_rlc_tm, "Rlc tm" ),
	qmaCommandNoParam( "rlc_tx", oam_rlc_tx, "Rlc tx" ),
	qmaCommandNoParam( "rlc_rx", oam_rlc_rx, "Rlc rx" ),
	qmaCommandNoParam( "rlc_mac", oam_rlc_mac, "Rlc mac" ),
	qmaCommandNoParam( "rlc_pdcp", oam_rlc_pdcp, "Rlc pdcp" ),
	qmaCommandNoParam( "rlc_rrc", oam_rlc_rrc, "Rlc rrc" ),
	qmaCommandNoParam( "rlc_oam", oam_rlc_oam, "Rlc oam" ),
	qmaCommandNoParam( "rlc_timer", oam_rlc_timer, "Rlc timer" ),	
	qmaEndCommands()
};

static const struct command_t __0806EE70[] = {
	qmaCommandNoParam( "All", oam_pdcp_enable_log_cat_all, "Pdcp rrcoam" ),
	qmaCommandNoParam( "pdcp_rrcoam", oam_pdcp_rrcoam, "Pdcp rrcoam" ),
	qmaCommandNoParam( "pdcp_tx", oam_pdcp_tx, "Pdcp tx" ),
	qmaCommandNoParam( "pdcp_rx", oam_pdcp_rx, "Pdcp rx" ),
	qmaCommandNoParam( "pdcp_rrm_int", oam_pdcp_rrm_int, "Pdcp rrm int" ),
	qmaEndCommands()
};
/* 55317 End */


#ifdef GTPU_DISABLE
static const struct parameter_t __setlogcatenablegtpu[] ={
	qmaMandatory( 0, qmaStandard(INTEGER, "21:25"), "log category" ),
	qmaEndParams()
};
#endif

static const struct command_t __0806F028[] = {
	qmaMode( "MAC", "Set Log Category Enable MAC ", __0806EE68, "To enable MAC Log Category" ),
	/* 55317 Start */
	qmaMode( "RLC", "Set Log Category Enable RLC ", __0806EE69, "To enable RLC Log Category" ),
	qmaMode( "PDCP", "Set Log Category Enable PDCP ", __0806EE70, "To enable PDCP Log Category" ),
	/* 55317 End */
#ifdef GTPU_DISABLE
	qmaCommand( "Gtpu", __setlogcatenablegtpu,oam_set_log_cat_l2_egtpu, "To enable gtpu Log Category" ),
#endif
	qmaEndCommands()
};

/* SPR 55317 START */
static const struct command_t __0806F541[] = {
	qmaCommandNoParam( "All", oam_set_log_cat_disable_rlc_all, "To disable all log categories of RLC" ),
	qmaCommandNoParam( "Rlc_am", oam_disable_rlc_am, "rlc_am " ),
	qmaCommandNoParam( "Rlc_um", oam_disable_rlc_um, "rlc_um " ),
	qmaCommandNoParam( "Rlc_tm", oam_disable_rlc_tm, "rlc_tm " ),
	qmaCommandNoParam( "Rlc_tx", oam_disable_rlc_tx, "rlc_tx " ),
	qmaCommandNoParam( "Rlc_rx", oam_disable_rlc_rx, "rlc_rx " ),
	qmaCommandNoParam( "Rlc_mac", oam_disable_rlc_mac, "rlc_mac " ),
	qmaCommandNoParam( "Rlc_pdcp", oam_disable_rlc_pdcp, "rlc_pdcp " ),
	qmaCommandNoParam( "Rlc_rrc", oam_disable_rlc_rrc, "rlc_rrc " ),
	qmaCommandNoParam( "Rlc_oam", oam_disable_rlc_oam, "rlc_oam " ),
	qmaCommandNoParam( "Rlc_timer", oam_disable_rlc_timer, "rlc_timer " ),
	qmaEndCommands()
};

static const struct command_t __0806F542[] = {
    qmaCommandNoParam( "All", oam_set_log_cat_disable_pdcp_all,
                              "To disable all log categories of PDCP" ),

    qmaCommandNoParam( "Pdcp_rrcoam", oam_set_log_cat_disable_pdcp_rrcoam,
                                      "pdcp_rrcoam" ),

    qmaCommandNoParam( "Pdcp_tx", oam_set_log_cat_disable_pdcp_tx,
                                  "pdcp_tx" ),

    qmaCommandNoParam( "Pdcp_rx", oam_set_log_cat_disable_pdcp_rx,
                                  "pdcp_rx" ),

    qmaCommandNoParam( "Pdcp_rrm_int", oam_set_log_cat_disable_pdcp_rrm_int,
                                       "pdcp_rrm_int" ),

    qmaEndCommands()
};

/* SPR 55317 END */

static const struct command_t __0806F540[] = {
	qmaCommandNoParam( "All", oam_set_log_cat_l2_disable_mac_all, "To set the desired log category" ),
	qmaCommandNoParam( "l2_sys_fail", oam_l2_disable_sys_fail, "L2 layer system Failure" ),
	qmaCommandNoParam( "mac_l1_inf", oam_disable_lmac_l1_inf, "Mac L1 Information" ),
	qmaCommandNoParam( "mac_ul_harq", oam_disable_mac_ul_harq, "Mac ul Harq" ),

	qmaCommandNoParam( "mac_rach", oam_disable_mac_rach, "Mac Rach" ),
	qmaCommandNoParam( "mac_rrc_inf", oam_disable_mac_rrc_inf, "Mac RRC Information" ),
	qmaCommandNoParam( "mac_oam_inf", oam_disable_mac_oam_inf, "Mac OAM Information" ),
/*CSR:00055315_FIX_START*/
    qmaCommandNoParam( "mac_rrm_inf", oam_disable_mac_rrm_inf, "Mac RRM Information" ),
/*CSR:00055315_FIX_END*/
	qmaCommandNoParam( "mac_dl_strategy", oam_disable_mac_dl_strategy, "Mac dl strategy" ),
	qmaCommandNoParam( "mac_ul_strategy", oam_disable_mac_ul_strategy, "Mac ul strategy" ),
	qmaCommandNoParam( "mac_pdsch", oam_disable_mac_pdsch, "Mac pdsch" ),
	qmaCommandNoParam( "mac_pdcch", oam_disable_mac_pdcch, "Mac pdcch" ),
	qmaCommandNoParam( "mac_pusch", oam_disable_mac_pusch, "Mac pusch" ),
	qmaCommandNoParam( "mac_pucch", oam_disable_mac_pucch, "Mac pucch" ),
	qmaCommandNoParam( "mac_bcch", oam_disable_mac_bcch, "Mac bcch" ),
	qmaCommandNoParam( "mac_pcch", oam_disable_mac_pcch, "Mac pcch" ),
	qmaCommandNoParam( "mac_ccch", oam_disable_mac_ccch, "Mac ccch" ),
	qmaCommandNoParam( "mac_dl_harq", oam_disable_mac_dl_harq, "Mac dl harq" ),
	qmaCommandNoParam( "mac_cqi", oam_disable_mac_cqi, "Mac cqi" ),
	qmaCommandNoParam( "mac_srs", oam_disable_mac_srs, "Mac srs" ),
	qmaCommandNoParam( "mac_tb_ul", oam_disable_mac_tb_ul, "Mac TB UL" ),
	qmaCommandNoParam( "mac_tb_dl", oam_disable_mac_tb_dl, "Mac TB DL" ),
	qmaCommandNoParam( "mac_dl_pc", oam_disble_mac_dl_pc, "Mac dl pc" ),
	qmaCommandNoParam( "mac_ul_pc", oam_disable_mac_ul_pc, "Mac ul pc" ),
	qmaCommandNoParam( "mac_drx", oam_disable_mac_drx, "Mac drx" ),
	qmaCommandNoParam( "mac_meas_gap", oam_disable_mac_meas_gap, "Mac meas gap" ),
	qmaCommandNoParam( "mac_meas_handler", oam_disable_mac_meas_handler, "Mac meas handler" ),
	qmaCommandNoParam( "mac_ul_csi", oam_disable_mac_ul_csi, "Mac ul csi" ),
	qmaCommandNoParam( "mac_mux", oam_disable_mac_mux, "Mac mux" ),
	qmaCommandNoParam( "mac_dl_sps_strategy", oam_disable_mac_dl_sps_strategy, "Mac dl sps strategy" ),
	qmaCommandNoParam( "mac_ul_sps_strategy", oam_disable_mac_ul_sps_strategy, "Mac ul sps strategy" ),
	qmaCommandNoParam( "mac_scheduler", oam_disable_mac_scheduler, "Mac scheduler" ),
	qmaCommandNoParam( "mac_atb", oam_disable_mac_atb, "Mac atb" ),
	qmaCommandNoParam( "mac_ta", oam_disable_mac_ta, "Mac ta" ),
	qmaEndCommands()
};



static const struct command_t __0806F708[] = {
	qmaMode( "MAC", "Set Log Category Disable MAC ", __0806F540, "To disable MAC Log Category" ),
	/* SPR 55317 */ qmaMode( "Rlc", "Set Log Category Disable RLC", __0806F541, "To disable RLC Log Category" ),
	qmaMode( "Pdcp","Set Log Category Disable PDCP ", __0806F542, "To disable pdcp Log Category" ),
#ifdef GTPU_DISABLE
	qmaCommand( "Gtpu", __setlogcatenablegtpu,oam_set_log_cat_disable_gtpu, "To disable gtpu Log Category" ),
#endif
	qmaEndCommands()
};

static const struct command_t __0806F800[] = {
	qmaMode( "Enable", "Set Log Category Enable ", __0806F028, "To Enable log category " ),
	qmaMode( "Disable", "Set Log Category Disable ", __0806F708, "To Disable log category" ),
	qmaEndCommands()
};

static const struct command_t __08070618[] = {
	qmaCommandNoParam( "Critical", oam_lvl, "To Enable Critical log level"),
	qmaCommandNoParam( "Error", oam_lvl, "To Enable Error log level"),
	qmaCommandNoParam( "Warning", oam_lvl, "To Enable Warning log level"),
	qmaCommandNoParam( "Info", oam_lvl, "To Enable Info log level"),
	qmaCommandNoParam( "Brief", oam_lvl, "To Enable Brief log level"),
	qmaCommandNoParam( "Detailed", oam_lvl, "To Enable Detailed log level"),
	qmaCommandNoParam( "DetailedAll", oam_lvl, "To Enable DetailedAll log level"),
	qmaEndCommands()
};


static const struct command_t __08071308[] = {
    qmaCommandNoParam( "Critical", oam_l2_log_lvl, "To Set L2 log level as Critical" ),
    qmaCommandNoParam( "Error", oam_l2_log_lvl, "To Set L2 log level as Error " ),
    qmaCommandNoParam( "Warning", oam_l2_log_lvl, "To Set L2 log level as Warning" ),
    qmaCommandNoParam( "Info", oam_l2_log_lvl, "To Set L2 log level as Info" ),
    qmaCommandNoParam( "Brief", oam_l2_log_lvl, "To Set L2 log level as Brief" ),
    qmaCommandNoParam( "Detailed", oam_l2_log_lvl, "To Set L2 log level as Detailed" ),
    qmaCommandNoParam( "DetailedAll", oam_l2_log_lvl, "To Set L2 log level as DetailedAll"),
    qmaEndCommands()
};


static const struct command_t __08071310[] = {
    qmaCommandNoParam( "Critical", oam_mac_log_lvl, "To Set MAC log level as Critical" ),
    qmaCommandNoParam( "Error", oam_mac_log_lvl, "To Set MAC log level as Error " ),
    qmaCommandNoParam( "Warning", oam_mac_log_lvl, "To Set MAC log level as Warning" ),
    qmaCommandNoParam( "Info", oam_mac_log_lvl, "To Set MAC log level as Info" ),
    qmaCommandNoParam( "Brief", oam_mac_log_lvl, "To Set MAC log level as Brief" ),
    qmaCommandNoParam( "Detailed", oam_mac_log_lvl, "To Set MAC log level as Detailed" ),
    qmaCommandNoParam( "DetailedAll", oam_mac_log_lvl, "To Set MAC log level as DetailedAll (per TTI logs)"),
    qmaEndCommands()
};


static const struct command_t __08071312[] = {
    qmaCommandNoParam( "Critical", oam_rlc_log_lvl, "To Set RLC log level as Critical" ),
    qmaCommandNoParam( "Error", oam_rlc_log_lvl, "To Set RLC log level as Error " ),
    qmaCommandNoParam( "Warning", oam_rlc_log_lvl, "To Set RLC log level as Warning" ),
    qmaCommandNoParam( "Info", oam_rlc_log_lvl, "To Set RLC log level as Info" ),
    qmaCommandNoParam( "Brief", oam_rlc_log_lvl, "To Set RLC log level as Brief" ),
    qmaCommandNoParam( "Detailed", oam_rlc_log_lvl, "To Set RLC log level as Detailed" ),
    qmaCommandNoParam( "DetailedAll", oam_rlc_log_lvl, "To Set RLC log level as DetailedAll"),
    qmaEndCommands()
};


static const struct command_t __08071314[] = {
    qmaCommandNoParam( "Critical", oam_pdcp_log_lvl, "To Set PDCP log level as Critical" ),
    qmaCommandNoParam( "Error", oam_pdcp_log_lvl, "To Set PDCP log level as Error " ),
    qmaCommandNoParam( "Warning", oam_pdcp_log_lvl, "To Set PDCP log level as Warning" ),
    qmaCommandNoParam( "Info", oam_pdcp_log_lvl, "To Set PDCP log level as Info" ),
    qmaCommandNoParam( "Brief", oam_pdcp_log_lvl, "To Set PDCP log level as Brief" ),
    qmaCommandNoParam( "Detailed", oam_pdcp_log_lvl, "To Set PDCP log level as Detailed" ),
    qmaCommandNoParam( "DetailedAll", oam_pdcp_log_lvl, "To Set PDCP log level as DetailedAll"),
    qmaEndCommands()
};


static const struct command_t __08071316[] = {
    qmaCommandNoParam( "Critical", oam_egtpu_log_lvl, "To Set EGTPU log level as Critical" ),
    qmaCommandNoParam( "Error", oam_egtpu_log_lvl, "To Set EGTPU log level as Error " ),
    qmaCommandNoParam( "Warning", oam_egtpu_log_lvl, "To Set EGTPU log level as Warning" ),
    qmaCommandNoParam( "Info", oam_egtpu_log_lvl, "To Set EGTPU log level as Info" ),
    qmaCommandNoParam( "Brief", oam_egtpu_log_lvl, "To Set EGTPU log level as Brief" ),
    qmaCommandNoParam( "Detailed", oam_egtpu_log_lvl, "To Set EGTPU log level as Detailed" ),
    qmaCommandNoParam( "DetailedAll", oam_egtpu_log_lvl, "To Set EGTPU log level as DetailedAll"),
    qmaEndCommands()
};


static const struct command_t __08071CF8[] = {
	qmaCommandNoParam( "Critical", oam_rrc_log_lvl, "To set RRC log level as Critical" ),
	qmaCommandNoParam( "Error", oam_rrc_log_lvl, "To set RRC log level as Error" ),
	qmaCommandNoParam( "Warning", oam_rrc_log_lvl, "To set RRC log level as Warning" ),
	qmaCommandNoParam( "Info", oam_rrc_log_lvl, "To set RRC log level as Info" ),
	qmaCommandNoParam( "Brief", oam_rrc_log_lvl, "To set RRC log level as Brief" ),
	qmaCommandNoParam( "Detailed", oam_rrc_log_lvl, "To set RRC log level as Detailed" ),
	qmaCommandNoParam( "DetailedAll", oam_rrc_log_lvl, "To set RRC log level as DetailedAll (composer-parser logs)" ),
	qmaEndCommands()
};

static const struct command_t __08071CE8[] = {
    qmaCommandNoParam( "Critical", oam_X2AP_log_lvl, "To set X2Ap log level as Fatal" ),
    qmaCommandNoParam( "Error", oam_X2AP_log_lvl, "To set X2Ap log level as Error" ),
    qmaCommandNoParam( "Warning", oam_X2AP_log_lvl, "To set X2Ap log level as Warning" ),
    qmaCommandNoParam( "Info", oam_X2AP_log_lvl, "To set X2Ap log level as Info" ),
    qmaCommandNoParam( "Brief", oam_X2AP_log_lvl, "To set X2Ap log level as Brief" ),
    qmaCommandNoParam( "Detailed", oam_X2AP_log_lvl, "To set X2Ap log level as Detailed" ),
    qmaEndCommands()
};


static const struct command_t __08071EF8[] = {
    qmaCommandNoParam( "Critical", oam_S1AP_log_lvl, "To set S1AP log level as Fatal" ),
    qmaCommandNoParam( "Error", oam_S1AP_log_lvl, "To set S1AP log level as Error" ),
    qmaCommandNoParam( "Warning", oam_S1AP_log_lvl, "To set S1AP log level as Warning" ),
    qmaCommandNoParam( "Info", oam_S1AP_log_lvl, "To set S1AP log level as Info" ),
    qmaCommandNoParam( "Brief", oam_S1AP_log_lvl, "To set S1AP log level as Brief" ),
    qmaCommandNoParam( "Detailed", oam_S1AP_log_lvl, "To set S1AP log level as Detailed" ),
    qmaEndCommands()
};

static const struct parameter_t __setloglevelrrmparam1[] ={
	qmaMandatory( 0, qmaStandard(CHOICE, "0|21|22|23"), "RRM Module id 0:,21:RRM Management Interface Handler, 22:CELL Manager ,23:UE Manager" ),
	qmaEndParams()
};

static const struct parameter_t __setloglevelrrmparam2[] ={
	qmaMandatory( 0, qmaStandard(CHOICE, "0|21|22|23"), "RRM Module id 0:,21:RRM Management Interface Handler, 22:CELL Manager ,23:UE Manager" ),
	qmaEndParams()
};
static const struct parameter_t __setloglevelrrmparam3[] ={
	qmaMandatory( 0, qmaStandard(CHOICE, "0|21|22|23"), "RRM Module id 0:,21:RRM Management Interface Handler, 22:CELL Manager ,23:UE Manager" ),
	qmaEndParams()
};
static const struct parameter_t __setloglevelrrmparam4[] ={
	qmaMandatory( 0, qmaStandard(CHOICE, "0|21|22|23"), "RRM Module id 0:,21:RRM Management Interface Handler, 22:CELL Manager ,23:UE Manager" ),
	qmaEndParams()
};
static const struct command_t __08071EC8[] = {
	qmaCommand( "Critical", __setloglevelrrmparam1,oam_qcli_rrm_set_log_level_handler, "To set RRM log level as Critical" ),
	qmaCommand( "Error", __setloglevelrrmparam1,oam_qcli_rrm_set_log_level_handler, "To set RRM log level as Error" ),
	qmaCommand( "Warning", __setloglevelrrmparam2,oam_qcli_rrm_set_log_level_handler, "To set RRM log level as Warning" ),
	qmaCommand( "Info", __setloglevelrrmparam2,oam_qcli_rrm_set_log_level_handler, "To set RRM log level as Info" ),
	qmaCommand( "Brief", __setloglevelrrmparam3,oam_qcli_rrm_set_log_level_handler, "To set RRM log level as Brief" ),
	qmaCommand( "Detailed",__setloglevelrrmparam4, oam_qcli_rrm_set_log_level_handler, "To set RRM log level as Detailed" ),
	qmaCommand( "DetailedAll",__setloglevelrrmparam4, oam_qcli_rrm_set_log_level_handler, "To set RRM log level as DetailedAll (composer-parser logs)" ),
	qmaEndCommands()
};


#ifdef OAM_SON_ENABLE
static const struct parameter_t __setloglevelsonparam1[] ={
	qmaMandatory( 0, qmaStandard(CHOICE, "0|25701|25702|25703|25704|25705"), "SON Module ID, possible values:\n 0 - SON_ALL_INT_MODULES \n 25701 - SON_MIF_MODULE_ID  \n 25702 - SON_APPS_MODULE_ID  \n 25703 - SON_ANR_MODULE_ID  \n 25704 - SON_ES_MODULE_ID  \n 25705 - SON_NMM_MODULE_ID" ),			 
	qmaEndParams()
};
static const struct parameter_t __setloglevelsonparam2[] ={
	qmaMandatory( 0, qmaStandard(CHOICE, "0|25701|25702|25703|25704|25705"), "SON Module ID, possible values:\n 0 - SON_ALL_INT_MODULES \n 25701 - SON_MIF_MODULE_ID  \n 25702 - SON_APPS_MODULE_ID  \n 25703 - SON_ANR_MODULE_ID  \n 25704 - SON_ES_MODULE_ID  \n 25705 - SON_NMM_MODULE_ID" ),			 
	qmaEndParams()
};
static const struct parameter_t __setloglevelsonparam3[] ={
	qmaMandatory( 0, qmaStandard(CHOICE, "0|25701|25702|25703|25704|25705"), "SON Module ID, possible values:\n 0 - SON_ALL_INT_MODULES \n 25701 - SON_MIF_MODULE_ID  \n 25702 - SON_APPS_MODULE_ID  \n 25703 - SON_ANR_MODULE_ID  \n 25704 - SON_ES_MODULE_ID  \n 25705 - SON_NMM_MODULE_ID" ),			 
	qmaEndParams()
};
static const struct parameter_t __setloglevelsonparam4[] ={
	qmaMandatory( 0, qmaStandard(CHOICE, "0|25701|25702|25703|25704|25705"), "SON Module ID, possible values:\n 0 - SON_ALL_INT_MODULES \n 25701 - SON_MIF_MODULE_ID  \n 25702 - SON_APPS_MODULE_ID  \n 25703 - SON_ANR_MODULE_ID  \n 25704 - SON_ES_MODULE_ID  \n 25705 - SON_NMM_MODULE_ID" ),			 
	qmaEndParams()
};

static const struct command_t __08072098[] = {
	qmaCommand( "Critical",__setloglevelsonparam1, oam_son_log_lvl, "To set SON Module log level as Critical" ),
	qmaCommand( "Error",__setloglevelsonparam1, oam_son_log_lvl, "To set SON Module log level as Error" ),
	qmaCommand( "Warning",__setloglevelsonparam2, oam_son_log_lvl, "To set SON Module log level as Warning" ),
	qmaCommand( "Info", __setloglevelsonparam3,oam_son_log_lvl, "To set SON Module log level as Info" ),
	qmaCommand( "Brief", __setloglevelsonparam3,oam_son_log_lvl, "To set SON Module log level as Brief" ),
	qmaCommand( "Detailed",__setloglevelsonparam4, oam_son_log_lvl, "To set SON Module log level as Detailed" ),
	qmaCommand( "DetailedAll",__setloglevelsonparam4, oam_son_log_lvl, "To set SON Module log level as DetailedAll (composer-parser logs)" ),
	qmaEndCommands()
};
#endif

static const struct command_t __08070620[] = {
	qmaCommandNoParam( "Critical", oam_set_log_lvl_all, "To Enable Critical log level of system"),
	qmaCommandNoParam( "Error", oam_set_log_lvl_all, "To Enable Error log level of system"),
	qmaCommandNoParam( "Warning", oam_set_log_lvl_all, "To Enable Warning log level of system"),
	qmaCommandNoParam( "Info", oam_set_log_lvl_all, "To Enable Info log level of system"),
	qmaCommandNoParam( "Brief", oam_set_log_lvl_all, "To Enable Brief log level of system"),
	qmaCommandNoParam( "Detailed", oam_set_log_lvl_all, "To Enable Detailed log level of system"),
	qmaCommandNoParam( "DetailedAll", oam_set_log_lvl_all, "To Enable Detailed All log level of system"),
	qmaEndCommands()
};


/* EMBMS Changes Start */
static const struct parameter_t m2ap_log_level_params[] = {
    /* 7.0 OAM_SSI Fix Start*/
    qmaMandatory("M2AP_LOG_LEVEL", 
            qmaStandard(CHOICE,"0|1|2|3|4|5|6|7"),
            "Accepted Values to set M2AP log level are: "
            "Fatal|Error|Warning|Info|Detailed|ASN|Detailed_ALL|NONE" ),			 
    /* 7.0 OAM_SSI Fix End*/
    qmaEndParams()
};
/* EMBMS Changes End */


static const struct command_t __08072330[] = {
	qmaMode( "System", "Set Log Level System ", __08070620, "To Set System Logs" ),
	qmaMode( "OAM", "Set Log Level OAM ", __08070618, "To Set OAM Logs" ),
	qmaMode( "L2", "Set Log Level L2 ", __08071308, "To set L2 Layers(MAC/RLC/PDCP/EGPTU) Log level" ),
	qmaMode( "MAC", "Set Log Level MAC ", __08071310, "To set MAC Log level" ),
	qmaMode( "RLC", "Set Log Level RLC ", __08071312, "To set RLC Log level" ),
	qmaMode( "PDCP", "Set Log Level PDCP ", __08071314, "To set PDCP Log level" ),
    /*SPR 19748 FIXED START */
	qmaMode( "GTPU", "Set Log Level GTPU ", __08071316, "To set GTPU Log level" ),
    /*SPR 19748 FIXED END */
   /*SPR FIX 17842 START    */
        qmaMode( "RRC", "Set Log Level L3 ", __08071CF8, "To set L3 log level" ),
        qmaMode( "S1AP", "Set Log Level S1AP ", __08071EF8, "To set S1AP log level" ),
        qmaMode( "X2AP", "Set Log Level X2AP ", __08071CE8, "To set X2AP log level" ),
   /*SPR FIX 17842 END    */
	qmaMode( "RRM", "Set Log Level RRM ", __08071EC8, "To set RRM log level" ),
#ifdef OAM_SON_ENABLE
	qmaMode( "SON", "Set Log Level SON ", __08072098, "To set SON Log level" ),
#endif
    /* EMBMS Changes Start */
    qmaCommand( "M2AP_LOG_LEVEL", m2ap_log_level_params, oam_qcli_send_command,
            "To set M2AP Log level" ),
    /* EMBMS Changes End */
	qmaEndCommands()
};





static const struct parameter_t __setlogenablerrmparam1[] ={
	qmaMandatory( 0, qmaStandard(CHOICE, "0|21|22|23"), "RRM Module id 0:,21:RRM Management Interface Handler, 22:CELL Manager ,23:UE Manager" ),
	qmaEndParams()
};

static const struct parameter_t __setlogenablerrmparam2[] ={
	qmaMandatory( 0, qmaStandard(CHOICE, "0|21|22|23"), "RRM Module id 0:,21:RRM Management Interface Handler, 22:CELL Manager ,23:UE Manager" ),
	qmaEndParams()
};

static const struct parameter_t __setlogenablerrmparam3[] ={
	qmaMandatory( 0, qmaStandard(CHOICE, "0|21|22|23"), "RRM Module id 0:,21:RRM Management Interface Handler, 22:CELL Manager ,23:UE Manager" ),
	qmaEndParams()
};
static const struct parameter_t __setlogenablerrmparam4[] ={
	qmaMandatory( 0, qmaStandard(CHOICE, "0|21|22|23"), "RRM Module id 0:,21:RRM Management Interface Handler, 22:CELL Manager ,23:UE Manager" ),
	qmaEndParams()
};
static const struct command_t __setlogenablerrm[] = {
	qmaCommand("Critical", __setlogenablerrmparam1,oam_log_enable_rrm,"To Enable RRM logging with Critical log"),
	qmaCommand("Error", __setlogenablerrmparam1,oam_log_enable_rrm_error,"To Enable RRM logging with Error log"),
	qmaCommand("Warning", __setlogenablerrmparam2,oam_log_enable_rrm_warning,"To Enable RRM logging with Warning log"),
	qmaCommand("Info", __setlogenablerrmparam1,oam_log_enable_rrm,"To Enable RRM logging with Infp log"),
	qmaCommand("Brief", __setlogenablerrmparam3,oam_log_enable_rrm_brief,"To Enable RRM logging with Brief log"),
	qmaCommand("Detailed", __setlogenablerrmparam4,oam_log_enable_rrm_detailed,"To Enable RRM logging with Detailed log"),
	qmaCommand("DetailedAll", __setlogenablerrmparam1,oam_log_enable_rrm,"To Enable RRM logging with DetailedAll log (composer parser logs)"),
	qmaEndCommands()
};
#ifdef OAM_SON_ENABLE
static const struct parameter_t __setlogenablesonparam1[] ={
	qmaMandatory( 0, qmaStandard(CHOICE, "0|25701|25702|25703|25704|25705"), "SON Module ID, possible values:\n 0 - SON_ALL_INT_MODULES \n 25701 - SON_MIF_MODULE_ID  \n 25702 - SON_APPS_MODULE_ID  \n 25703 - SON_ANR_MODULE_ID  \n 25704 - SON_ES_MODULE_ID  \n 25705 - SON_NMM_MODULE_ID" ),			 
	qmaEndParams()
};
static const struct parameter_t __setlogenablesonparam2[] ={
	qmaMandatory( 0, qmaStandard(CHOICE, "0|25701|25702|25703|25704|25705"), "SON Module ID, possible values:\n 0 - SON_ALL_INT_MODULES \n 25701 - SON_MIF_MODULE_ID  \n 25702 - SON_APPS_MODULE_ID  \n 25703 - SON_ANR_MODULE_ID  \n 25704 - SON_ES_MODULE_ID  \n 25705 - SON_NMM_MODULE_ID" ),			 
	qmaEndParams()
};

static const struct parameter_t __setlogenablesonparam3[] ={
	qmaMandatory( 0, qmaStandard(CHOICE, "0|25701|25702|25703|25704|25705"), "SON Module ID, possible values:\n 0 - SON_ALL_INT_MODULES \n 25701 - SON_MIF_MODULE_ID  \n 25702 - SON_APPS_MODULE_ID  \n 25703 - SON_ANR_MODULE_ID  \n 25704 - SON_ES_MODULE_ID  \n 25705 - SON_NMM_MODULE_ID" ),			 
	qmaEndParams()
};

static const struct parameter_t __setlogenablesonparam4[] ={
	qmaMandatory( 0, qmaStandard(CHOICE, "0|25701|25702|25703|25704|25705"), "SON Module ID, possible values:\n 0 - SON_ALL_INT_MODULES \n 25701 - SON_MIF_MODULE_ID  \n 25702 - SON_APPS_MODULE_ID  \n 25703 - SON_ANR_MODULE_ID  \n 25704 - SON_ES_MODULE_ID  \n 25705 - SON_NMM_MODULE_ID" ),			 
	qmaEndParams()
};

static const struct command_t  __setlogenableson[] ={
	qmaCommand("Critical", __setlogenablesonparam1,oam_son_enable_log,"To Enable SON logging with Critical log"),
	qmaCommand("Error", __setlogenablesonparam1,oam_son_enable_errorlog,"To Enable SON logging with Error log"),
	qmaCommand("Warning", __setlogenablesonparam2,oam_son_enable_warninglog,"To Enable SON logging with Warning log"),
	qmaCommand("Info", __setlogenablesonparam1,oam_son_enable_log,"To Enable SON logging with Info log"),
	qmaCommand("Brief", __setlogenablesonparam3,oam_son_enable_brieflog,"To Enable SON logging with Brief log"),
	qmaCommand("Detailed", __setlogenablesonparam4,oam_son_enable_detailedlog,"To Enable SON logging with Detailed log"),
	qmaCommand("DetailedAll", __setlogenablesonparam1,oam_son_enable_log,"To Enable SON logging with DetailedAll log (composer parser logs)"),
	qmaEndCommands()
};

#endif
static const struct command_t __setlogenable[] = {
	qmaMode("RRM", "Set Log Enable RRM ", __setlogenablerrm,"To Enable RRM Logs" ),
#ifdef OAM_SON_ENABLE
	qmaMode("SON" ,"Set Log Enable SON ",__setlogenableson,"To Enable SON Logs"),
#endif
       
qmaCommandNoParam("RRC",oam_log_enable_rrc,"To Enable RRC Logs"),


    qmaCommandNoParam("X2AP",oam_log_enable_x2ap,"To Enable X2AP Logs"),
    qmaCommandNoParam("S1AP",oam_log_enable_s1ap,"To Enable S1AP Logs"),
qmaEndCommands()

};

static const struct parameter_t __setlogdisablerrmparam1[] ={
	qmaMandatory( 0, qmaStandard(CHOICE, "0|21|22|23"), "RRM Module id 0:,21:RRM Management Interface Handler, 22:CELL Manager ,23:UE Manager" ),
	qmaEndParams()
};


#ifdef OAM_SON_ENABLE
static const struct parameter_t __setlogdisablesonparam1[] ={
	qmaMandatory( 0, qmaStandard(CHOICE, "0|25701|25702|25703|25704|25705"), "SON Module ID, possible values:\n 0 - SON_ALL_INT_MODULES \n 25701 - SON_MIF_MODULE_ID  \n 25702 - SON_APPS_MODULE_ID  \n 25703 - SON_ANR_MODULE_ID  \n 25704 - SON_ES_MODULE_ID  \n 25705 - SON_NMM_MODULE_ID" ),			 
	qmaEndParams()
};
#endif
static const struct command_t __setlogdisable[] = {
	qmaCommand("RRM",__setlogdisablerrmparam1,oam_rrm_disable_errorlog,"To Disable RRM Logs"),
#ifdef OAM_SON_ENABLE
	qmaCommand("SON",__setlogdisablesonparam1,oam_son_disable_errorlog,"To Disable SON Logs"),
#endif
qmaCommandNoParam("RRC",oam_log_disable_rrc,"To Disable RRC Logs"),

   qmaCommandNoParam("X2AP",oam_log_disable_x2ap,"To Disable X2AP Logs"),
    qmaCommandNoParam("S1AP",oam_log_disable_s1ap,"To Disable S1AP Logs"),
    qmaEndCommands()
};

static const struct command_t __080730C0[] = {
	qmaMode( "Category", "Set Log Category ", __0806F800, "To Set Log Category [ENABLE / DISABLE]" ),
	qmaMode( "Level", "Set Log Level ", __08072330, "To Set Log Level [OAM / L2 / MAC / RLC / PDCP / GTPU / L3 / RRM / SON]" ),
#ifdef OAM_SON_ENABLE
	qmaMode( "Enable", "Set Log Enable ", __setlogenable, "To Enable Logs [RRM / SON / RRC]" ),
	qmaMode( "Disable", "Set Log Disable ", __setlogdisable, "To Disable Logs [RRM / SON / RRC]" ),
#else
	qmaMode( "Enable", "Set Log Enable ", __setlogenable, "To Enable Logs [RRM / RRC / X2AP /S1AP]" ),
	qmaMode( "Disable", "Set Log Disable ", __setlogdisable, "To Disable Logs [RRM / RRC / X2AP / S1AP]" ),
#endif

	qmaEndCommands()
};


static const struct parameter_t __080733C8[] = {
/*SPR 11323 FIX START*/
    qmaMandatory( 0, qmaStandard(INTEGER, "0:255"), "Value of Cell index which is active" ),
/*SPR 11323 FIX END*/
	qmaMandatory( 0, qmaStandard(INTEGER, "1:7"), "Interface to trace value - \n\t\t\tBit1 : S1-MME\n\t\t\tBit2 : X2\n\t\t\tBit3 : UU\n" ),
	qmaMandatory( 0, qmaStandard(INTEGER, "0:5"), "Trace depth value  - \n\t\t\t0 : Minimum\n\t\t\t1 : Medium\n\t\t\t2 : Maximum\n\t\t\t3 : Minimum without vendor specific extension\n\t\t\t4 : Medium without vendor specific extension\n\t\t\t5 : Maximum without Vendor specific extension\n" ),
	qmaMandatory( 0, qmaStandard(STRING, 0), "Trace collection entity IPv4/IPv6 address\n" ),
	qmaMandatory( 0, qmaStandard(INTEGER, "1:16777215"), "Trace id for Trace reference\n" ),/*SPR FIX 7851*/ /*SPR FIX 16727*/

	qmaEndParams()
};
static const struct parameter_t context_args[] = {
    qmaMandatory( 0, qmaStandard(INTEGER, "0:7"), "Cell index value" ),
    qmaEndParams()
};

static const struct parameter_t __080734C0[] = {
/*SPR 11323 FIX START*/
    qmaMandatory( 0, qmaStandard(INTEGER, "0:255"), "Value of Cell index which is active" ),
/*SPR 11323 FIX END*/
	qmaEndParams()
};

static const struct command_t __080734E0[] = {
	qmaCommand( "Start", __080733C8, oam_call_trace_start, "To Start Cell trace" ),
	qmaCommand( "Stop", __080734C0, oam_call_trace_stop, "To Stop Cell trace" ),
	qmaEndCommands()
};

static const struct command_t set_cell_context[] = {
    qmaCommand( "Context", context_args, oam_qcli_send_command, "To Start Cell trace" ),
    qmaEndCommands()
};

#ifdef OAM_SON_ENABLE
/*SON ENERGY SAVING*/
static const struct parameter_t energy_saving_enable[] = {
	qmaMandatory(0, qmaStandard(INTEGER, "0:65535"), "Active UE count report interval - The value 0 means it will not be reported periodically. (Interval unit is in Secs)" ),
	qmaMandatory( 0, qmaStandard(INTEGER, "0:255"), " Threshold for number of active UEs - When threshold is reached or the no of UE exceeds the threshold value,report will be send to the OAM" ),
	qmaEndParams()
};

static const struct parameter_t es_modify_ue_count_interval[] = {
	qmaMandatory( 0, qmaStandard(INTEGER, "0:65535"), "Active UE count report interval - The value 0 means it will not be reported periodically (Interval unit is in Secs)" ),
	qmaEndParams()
};

static const struct parameter_t es_modify_ue_count_threshold[] = {
	qmaMandatory( 0, qmaStandard(INTEGER, "0:255"), "Threshold value for no. of active UEs" ),
	qmaEndParams()
};

static const struct parameter_t switch_off_mode[] = {
	qmaMandatory( 0, qmaStandard(INTEGER, "1:2"), "value 1 for Graceful switch off/value 2 for Forceful switch off" ),
	qmaEndParams()
};

static const struct parameter_t auto_both[] = {
	qmaKeyword("TimerDuration "),
    qmaMandatory(0, qmaStandard(INTEGER, "1:65535"),
                    "Timer Duration to monitor Load and/or Active UE count"
                   " before triggering Autonomous Cell Switch Off" ),

	qmaKeyword("LoadLevelThreshold "),
    qmaMandatory(0, qmaStandard(INTEGER, "0:100"),
                    "Threshold value of composite capacity to trigger "
                    "cell switch off"),

	qmaEndParams()
};

static const struct parameter_t auto_active_ue[] = {
	qmaKeyword("TimerDuration "),
    qmaMandatory(0, qmaStandard(INTEGER, "1:65535"),
                    "Timer Duration to monitor Active UE count before "
                    "triggering Autonomous Cell Switch Off" ),
	qmaEndParams()
};
static const struct parameter_t auto_load_level[] = {
	qmaKeyword("TimerDuration "),
    qmaMandatory(0, qmaStandard(INTEGER, "1:65535"),
                    "Timer Duration to monitor Load before triggering "
                    "Autonomous Cell Switch Off" ),
	qmaKeyword("LoadLevelThreshold "),
    qmaMandatory(0, qmaStandard(INTEGER, "0:100"),
                    "Threshold value of composite capacity to trigger "
                    "cell switch off"),
    qmaEndParams()
};
static const struct command_t trigger_crit_mode[] = {
    qmaCommand("LoadLevel", auto_load_level, oam_qcli_send_command,
                "To trigger load level criteria" ),

    qmaCommand("ActiveUECount", auto_active_ue, oam_qcli_send_command,
                                "To trigger active ue count criteria"),
    qmaCommand("Both", auto_both, oam_qcli_send_command,
                       "To trigger both active ue count and load "
                       "level criteria" ),

	qmaEndCommands()
};

static const struct command_t aut_switch_off_mode[] = {
    qmaMode("TriggerCriteria",
            "SON ES Cell Autonomous_SwitchOff TriggerCriteria ",
            trigger_crit_mode,
            "Triggering criterion for Autonomous_SwitchOff" ),
    qmaEndCommands()
};

static const struct command_t son_es_cell_mode[] = {
	qmaCommandNoParam( "SwitchOn", oam_switch_on, "To switch ON ES cell" ),
	qmaCommand( "SwitchOff",switch_off_mode, oam_switch_off, "To switch OFF ES Cell" ),
    qmaMode( "Autonomous_SwitchOff", "Set SON ES Cell Autonomous_SwitchOff ",
                        aut_switch_off_mode, "To enable autonomous cell switch off"),
	qmaEndCommands()
};

static const struct command_t oam_es_modify[] = {
	qmaCommand( "UE_Count_interval", es_modify_ue_count_interval, oam_qcli_send_command, "To modify the interval for receiving the periodic active UE count report" ),
	qmaCommand( "UE_Count_threshold", es_modify_ue_count_threshold, oam_qcli_send_command, "To modify the threshold value for the no. of Active UEs for the specified cell" ),
	qmaEndCommands()
};
static const struct parameter_t es_retrieve_sw_off_cell[] = {
	qmaOptional( "", "1",qmaStandard(INTEGER, "0"), ""),
	qmaEndParams()
};

static const struct command_t oam_es_retrieve[] = {
	qmaCommand( "Switched_off_cells", es_retrieve_sw_off_cell, oam_qcli_send_command, "To retrieve info about Switched_off_cells" ), /*  SPR 14114 Fix */ 
	qmaEndCommands()
};
static const struct command_t son_energy_saving[] = {
	qmaCommand( "Enable", energy_saving_enable, oam_es_enable, "Enable ES" ),
	qmaCommandNoParam( "Disable", oam_es_disable, "Disable ES" ),
    qmaMode( "Cell", "Set SON ES Cell ", son_es_cell_mode, "To Set Cell parameters [SwitchOn / SwitchOff]" ),
	qmaMode( "Modify", "SON ES Modify ", oam_es_modify, "To modify UE count interval/UE count threshold" ),
	qmaMode( "Retrieve", "SON ES Retrieve ", oam_es_retrieve, "To Retrieve info about cells" ),  /* SPR 14114 Fix */
	qmaEndCommands()
};
	/* SPR_19619 start*/
static const struct parameter_t __cell_activation[] = {
	qmaKeyword("PLMNID"),
	qmaMandatory("PLMNID", qmaStandard(STRING, "MAXLEN 6"),
			"Possible values: 5/6 digit value"),

	qmaKeyword("EUTRANCID"),
	qmaMandatory("EUTRANCID", qmaStandard(INTEGER, "1:268435455"),
			"Range : 1:268435455"),
	qmaEndParams()

};
	/* SPR_19619 stop*/

static const struct parameter_t son_rach_optmode_modify[] = {
	qmaMandatory( 0, qmaStandard(INTEGER, "0:1")," : Possible values : \n \t\t\t  0: Distributed Mode \n \t\t\t  1: Centralised Mode" ),
	qmaEndParams()
};

static const struct parameter_t son_rach_opt_config_params[] = {
	qmaOptional( "RACHOPT_REPORT_PERIODICITY", "-", qmaStandard(INTEGER, "1:600")," Possible values : 1:600 " ),
	qmaOptional( "RACHOPT_MIN_UE_RACH_REPORT_COUNT","-", qmaStandard(INTEGER, "10:1000"),"Possible values : 10:1000 " ),
	qmaOptional( "RACHOPT_UE_RACH_ATTEMPTS_THRESHOLD","-", qmaStandard(INTEGER, "2:10"),"Possible values : 2:10 " ),
	qmaOptional( "RACHOPT_START_THRESHOLD", "-",qmaStandard(INTEGER, "1:1000"),"Possible values : 1:1000 " ),
	qmaOptional( "RACHOPT_STOP_THRESHOLD", "-",qmaStandard(INTEGER, "1:1000")," Possible values : 1:1000 " ),
	qmaOptional( "RACHOPT_DEGRADE_STOP_OFFSET","-", qmaStandard(INTEGER, "1:1000"),"Possible values : 1:1000 " ),
	qmaOptional( "RACHOPT_MAX_ITERATIONS_TO_REACH_STOP_THRESHOLD","-", qmaStandard(INTEGER, "1:10"),"Possible values : 1:10 " ),
	qmaOptional( "RACHOPT_CONTENTION_THRESHOLD", "-",qmaStandard(INTEGER, "1:100")," Possible values : 1:100 " ),
	qmaOptional( "RACHOPT_POWER_RAMPUP_THRESHOLD","-", qmaStandard(INTEGER, "1:100"),"Possible values : 1:100 " ),
	qmaOptional( "RACHOPT_MIN_CFRA_PREAMBLES", "-",qmaStandard(INTEGER, "4:60"),"Possible values : 4:60 " ),
	qmaEndParams()
};

static const struct command_t son_rach_opt[] = {
    /*BUG_FIXED_11742_FIXED_START */
    qmaCommandNoParam( "ENABLE", oam_qcli_send_command_no_param, "To Enable rach-opt " ),
    qmaCommandNoParam( "DISABLE", oam_qcli_send_command_no_param, "To Disable rach-opt " ),
    /*BUG_FIXED_11742_FIXED_END */
	qmaCommand("MODIFY_MODE",son_rach_optmode_modify, oam_qcli_send_command, "To modify rach-opt mode "),
	qmaCommand("CONFIG",son_rach_opt_config_params, qcli_validate_send_optional_param_cmd, "To Config rach-opt params"),
	qmaEndCommands()	
};


static const struct parameter_t mro_modify_config_params[] = {
	qmaOptional( "Mro_Mode", "-" ,qmaStandard(INTEGER, "0:1"), "Mro_Mode:Possible values : 0:1"),
	qmaOptional( "Mro_Param", "-" ,qmaStandard(INTEGER, "0:2"), "Mro_Param:Possible values : 0:2"),
	qmaOptional( "Mro_Ho_Failure_Accumulation_Duration", "-" ,qmaStandard(INTEGER, "5:60"),"Mro_Ho_Failure_Accumulation_Duration: Possible values : 5:60"),
	qmaOptional( "Delta_Cio", "-" ,qmaStandard(INTEGER, "1:5"), "Delta_Cio: Possible values : 1:5"),
	qmaOptional( "Mro_Ho_Attempts_Threshold", "-" ,qmaStandard(INTEGER, "1:1"), "Mro_Ho_Attempts_Threshold: Possible values : 1:0"),
	qmaOptional( "Mro_Ho_Optimization_Start_Threshold", "-" ,qmaStandard(INTEGER, "1:100"), "Mro_Ho_Optimization_Start_Threshold: Possible values : 1:100"),
	qmaOptional( "Mro_Ho_Optimization_Stop_Threshold", "-" ,qmaStandard(INTEGER, "1:100"), "Mro_Ho_Optimization_Stop_Threshold: Possible values : 1:100"),
	qmaOptional( "Mro_Ho_Fail_Rate_For_Optimization", "-" ,qmaStandard(INTEGER, "1:1000"), "Mro_Ho_Fail_Rate_For_Optimization: Possible values : 1:1000"),
	qmaOptional( "TTT", "-" ,qmaStandard(INTEGER, "1:15"), "TTT: Possible values : 1:15"),
	qmaOptional( "Delta_ttt", "-" ,qmaStandard(INTEGER, "1:5"), "Delta_ttt: Possible values : 1:5"),
	qmaEndParams()
};

static const struct command_t __mro_modify[] = {
          qmaCommand("CONFIG_PARAMS",mro_modify_config_params, (void *)oam_qcli_send_command, "Enter Config Param Values"),
          qmaEndCommands()
};

static const struct command_t son_mro[] = {
	qmaCommandNoParam( "Enable", oam_qcli_send_command_no_param, "Enable MRO" ),
	qmaCommandNoParam( "Disable", oam_qcli_send_command_no_param, "Disable MRO" ),
    qmaMode("MODIFY","SON MRO MODIFY",__mro_modify, "Modify MRO"),
	qmaEndCommands()
};

static const struct parameter_t mlb_modify_params[] = {
    /* SPR 11519 CHANGE START */
    qmaOptional( "Mlb_Mode", "-" ,qmaStandard(INTEGER, "0:1"), "Mlb_Mode: Possible values : \n \t\t\t  0: Distributed Mode \n \t\t\t  1: Centralised Mode"),
     /* SPR 11519 CHANGE END */
    qmaOptional( "Q-Offset-Min", "-" ,qmaStandard(INTEGER, "0:30"), "Q-Offset-Min: Possible values : 0-30 Optional for Modify Attribut Request"),
    qmaOptional( "Q-Offset-Max", "-", qmaStandard(INTEGER, "0:30"), "Q-Offset-Max: Possible values : 0-30 - Optional for Modify Attribut Request"),
    qmaEndParams()
};

static const struct command_t _mlb_modify_attribute_req[] = {
        qmaCommand("PARAMS",mlb_modify_params, (void *)oam_qcli_send_command, "Enter values"),
        qmaEndCommands()
};

static const struct command_t son_mlb[] = {
	qmaCommandNoParam( "Enable", oam_qcli_send_command_no_param, "Enable MLB" ),
	qmaCommandNoParam( "Disable", oam_qcli_send_command_no_param, "Disable MLB" ),
    qmaMode("MODIFY","SON MLB MODIFY",_mlb_modify_attribute_req, "Modify MLB"),
	qmaEndCommands()
};
#endif

/* FLAG_CHANGES - Code Removed Here */
static const struct command_t __080740A8[] = {
	qmaMode( "Log", "Set Log ", __080730C0, "To set [Log level / Category, Enable / Disable Logs]" ),
	qmaMode( "Cell_Trace", "Set Cell_Trace ", __080734E0, "To Set Cell Trace [Start / Stop]" ),
    qmaMode( "Cell_Context", "Set Cell_Context ", set_cell_context, "To Set Cell context" ),
	/* FLAG_CHANGES - Code Removed Here */
	qmaEndCommands()
};
/*******************************************************************************
 * Function Name  : all
 * Description    : To find tag all  
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **all(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);    
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("All"));
	return L;
}

static const struct parameter_t __08074F41[] = {

	qmaMandatory(0,qmaDynamic("",all),"select your option"),
	qmaEndParams()
};

/*CSR 00053551 FIX END*/

/*******************************************************************************
 * Function Name  : status_type
 * Description    : To Restart LTE OAM
 * Inputs         : unused
 * Outputs        : NONE
 * Returns        : NONE
 ********************************************************************************/
static char **status_type(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */

	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("EGTPU_STATS_PROTO_MSG"));
	L=qsladdstr(L,strdup("EGTPU_STATS_API"));
	L=qsladdstr(L,strdup("EGTPU_STATS_ERROR"));
	L=qsladdstr(L,strdup("EGTPU_STATS_INTERNAL"));
	L=qsladdstr(L,strdup("EGTPU_STATS_ERR_PROTO_MSG"));
	return L;
}

/*******************************************************************************
 * Function Name  : reset_type
 * Description    : To find tag reset_type (RESET,NOT_RESET) 
 * Inputs         : pointer to void 
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **reset_type(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("RESET"));
	L=qsladdstr(L,strdup("NOT_RESET"));
	return L;
}

static const struct parameter_t __08074F42[] = {

	qmaMandatory(0,qmaDynamic("",status_type),"select your option"),
	qmaMandatory(0,qmaDynamic("",reset_type),"select your option"),
	qmaEndParams()
};



/*CSR 00053551 FIX END*/ 
static const struct command_t __080743F8[] = {
	qmaCommand( "Mac", __08074F41,oam_qcli_send_command_no_param , "To Get the KPIs maintained at MAC" ),
	qmaCommand( "PDCP", __08074F41,oam_qcli_send_command_no_param , "To Get the KPIs maintained at PDCP" ),
	qmaCommand( "RRC", __08074F41, oam_qcli_send_command, "To Get the KPIs maintained at RRC" ),
	qmaCommand( "RRM", __08074F41, oam_qcli_send_command_no_param, "To Get the KPIs maintained at RRM" ),
	qmaEndCommands()
};

static const struct command_t __080743K8[] = {
	qmaCommand( "PDCP", __08074F41,oam_qcli_send_command_no_param , "To Get the Perf Stats maintained at PDCP" ),
	qmaEndCommands()
};

static const struct command_t __080746A8[] = {
	qmaCommandNoParam( "MAC", oam_show_log_cat_l2_mac, "To show MAC category logs" ),
	qmaCommandNoParam( "PDCP", oam_show_log_cat_l2_pdcp, "To show PDCP category logs" ),
	qmaCommandNoParam( "RLC", oam_show_log_cat_l2_rlc, "To show RLC category logs" ),
#ifdef GTPU_DISABLE
	qmaCommandNoParam( "GTPU", oam_show_log_cat_l2_gtpu, "To show GTPU category logs" ),
#endif
	qmaEndCommands()
};

static const struct command_t __08074708[] = {
	qmaMode( "L2", "Show Log Category L2 ", __080746A8, "To show L2 log category" ),
	qmaEndCommands()
};


static const struct command_t __080749C0[] = {
    qmaCommandNoParam( "L2", oam_show_log_level_l2_all, "To show L2 Layers(MAC/RLC/PDCP/EGTPU) Log Level" ),
    qmaCommandNoParam( "MAC", oam_show_log_level_mac, "To show MAC Log Level" ),
    qmaCommandNoParam( "RLC", oam_show_log_level_rlc, "To show RLC Log Level" ),
    qmaCommandNoParam( "PDCP", oam_show_log_level_pdcp, "To show PDCP Log Level" ),
    /*SPR 19748 FIXED START*/
    qmaCommandNoParam( "GTPU", oam_show_log_level_gtpu, "To show GTPU Log Level" ),
    /*SPR 19748 FIXED END*/
    /* SPR_13116_CHANGE_START */
    qmaCommandNoParam( "OAM", oam_show_log_level_oam, "To show OAM Log Level" ),
    qmaCommandNoParam( "RRM", oam_show_log_level_rrm, "To show RRM Log Level" ),
    /*SPR 13617 START*/
   /*SPR 17842 START  */
    qmaCommandNoParam( "RRC",oam_show_log_level_l3, "To show L3 Log Level" ),
    qmaCommandNoParam( "S1AP",oam_show_log_level_l3_s1ap, "To show L3 s1ap Log Level" ),
    qmaCommandNoParam( "X2AP",oam_show_log_level_l3_x2ap, "To show L3 x2ap Log Level" ),
   /*SPR 17842 END  */
   /*SPR 13617 END*/
#ifdef OAM_SON_ENABLE
    qmaCommandNoParam( "SON",oam_show_log_level_son, "To show SON Log Level" ),
#endif
    /* SPR_13116_CHANGE_END */
    /* 7.0 OAM_SSI Fix Start*/
    /* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
    qmaCommandNoParam( "M2AP", oam_qcli_send_command_no_param, "To show M2AP Log Level" ),
#endif
    /* EMBMS Changes End */
    /* 7.0 OAM_SSI Fix End*/
    qmaEndCommands()
};
static const struct command_t __08074AE0[] = {
	qmaMode( "Category", "Show Log Category ", __08074708, "To show Log Category" ),
	qmaMode( "Level", "Show Log Level ", __080749C0, "To show Log Level" ),
	qmaEndCommands()
};


static const struct command_t __08074DB0[] = {
	qmaCommand( "MAC", __08074F41, oam_qcli_send_command, "To show MAC stats " ),
	qmaCommand( "RLC", __08074F41, oam_qcli_send_command, "To show RLC stats " ),
	qmaCommand( "PDCP",__08074F41, oam_qcli_send_command, "To show PDCP stats" ),
	/*CSR 00053551 FIX START*/ 
	qmaCommand( "EGTPU",__08074F42, oam_qcli_send_command, "To show EGTPU stats" ),
	/*CSR 00053551 FIX END*/ 
	qmaEndCommands()
};

static const struct command_t __08075080[] = {
	qmaCommand( "S1AP", __08074F41, oam_qcli_send_command, "To show SIAP stats " ),
    qmaCommand( "M2AP", __08074F41, oam_qcli_send_command_no_param, "To Get the KPIs maintained at M2AP" ),
	qmaEndCommands()
};


static const struct command_t __080751B8[] = {
	qmaMode( "L2", "Show Stats L2 ", __08074DB0, "To Show L2 Stats [MAC / RLC / PDCP /EGTPU]" ),
	qmaMode( "L3", "Show Stats L3 ", __08075080, "To Show L3 Stats [S1AP]" ),
	qmaEndCommands()
};

/*******************************************************************************
 * Function Name  : showcellreserved
 * Description    : To find tag showcellreserved (BarringForEmergency,CellBarred,IntraFreqCellReselectionIndicator)
 * Inputs         : pointer to void 
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **showcellreserved(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("BarringForEmergency"));
	L=qsladdstr(L,strdup("CellBarred"));
	L=qsladdstr(L,strdup("IntraFreqCellReselectionIndicator"));
	return L;
}

static const struct parameter_t __0807PC04[] = {

	qmaMandatory(0,qmaDynamic(" ",showcellreserved),"Cell Restriction Parameters"),
	qmaEndParams()
};


/* SPR 13970 FIX Start */

/*******************************************************************************
 * Function Name  : showcellreserved_ac
 * Description    : To find tag showcellreserved (AC_BARRING_FACTOR,AC_BARRING_TIME,AC_BARRING_FOR_SPECIAL_AC)
 * Inputs         : pointer to void 
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/

static char **showcellreserved_ac(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
    char **L = NULL; CLI_QSLALLOC(L);
    *L = 0;
    L=qsladdstr(L,strdup("AC_BARRING_FACTOR"));
    L=qsladdstr(L,strdup("AC_BARRING_TIME"));
    L=qsladdstr(L,strdup("AC_BARRING_FOR_SPECIAL_AC"));
    return L;
}

static const struct parameter_t __0807PC08[] = {

    qmaMandatory(0,qmaDynamic(" ",showcellreserved_ac),"Cell Restriction Ac Barring For Mo Signalling Parameters"),
    qmaEndParams()
};

static const struct parameter_t __0807PC09[] = {

    qmaMandatory(0,qmaDynamic(" ",showcellreserved_ac),"Cell Restriction Ac Barring For Mo Data Parameters"),
    qmaEndParams()
};

static const struct parameter_t __0807PC0A[] = {

    qmaMandatory(0,qmaDynamic(" ",showcellreserved_ac),"Cell Restriction SSAC Barring For MMtel Voice R9 Parameters"),
    qmaEndParams()
};

static const struct parameter_t __0807PC0B[] = {

    qmaMandatory(0,qmaDynamic(" ",showcellreserved_ac),"Cell Restriction SSAC Barring For MMtel Video R9 Parameters"),
    qmaEndParams()
};

static const struct parameter_t __0807PC0C[] = {

    qmaMandatory(0,qmaDynamic(" ",showcellreserved_ac),"Cell Restriction Ac Barring For CSFB R10 Parameters"),
    qmaEndParams()
};


/* SPR 13970 FIX End */



/*******************************************************************************
 * Function Name  : showPDSCH
 * Description    : To find tag showPDSCH (Pa,Pb)
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **showPDSCH(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("Pa"));
	L=qsladdstr(L,strdup("Pb"));
	return L;
}

static const struct parameter_t __0807PC12[] = {

	qmaMandatory(0,qmaDynamic(" ",showPDSCH),"To fetch value of PDSCH parameter"),
	qmaEndParams()
};


/*******************************************************************************
 * Function Name  : showSRS
 * Description    : To find tag showSRS (SRSBandwidthConfig,SRSMaxUpPTS,SRSEnabled,AckNackSRSSimultaneousTransmission)
 * Inputs         : pointer to void 
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **showSRS(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("SRSBandwidthConfig"));
	L=qsladdstr(L,strdup("SRSMaxUpPTS"));
	L=qsladdstr(L,strdup("SRSEnabled"));
	L=qsladdstr(L,strdup("AckNackSRSSimultaneousTransmission"));
	return L;
}

static const struct parameter_t __0807PC19[] = {

	qmaMandatory(0,qmaDynamic("",showSRS),"To fetch value of SRS parameters"),
	qmaEndParams()
};


/* EMBMS Changes Start */
/*******************************************************************************
 * Function Name  : show_mbsfn_params
 * Description    : To fetch MBSFN parameters
 * Inputs         : pointer to void 
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **show_mbsfn_params(const void *unused)
{
    qcliHandleUnusedParameter(unused);
    char **L = NULL; CLI_QSLALLOC(L);
    *L = 0;
    L=qsladdstr(L,strdup("NeighCellConfig"));
    L=qsladdstr(L,strdup("MaxSFConfigListEntries"));
    L=qsladdstr(L,strdup("SFConfigListNumberOfEntries"));
    return L;
}

static const struct parameter_t mbsfn_params[] = {

    qmaMandatory(0,qmaDynamic("",show_mbsfn_params),
            "To fetch value of MBSFN parameters. "),
    qmaEndParams()
};
/* EMBMS Changes End */

/*******************************************************************************
 * Function Name  : ShowDRX
 * Description    : To find tag ShowDRX 
 * Inputs         : pointer to void 
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **ShowDRX(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("DRXShortCycleTimer"));
	L=qsladdstr(L,strdup("DRXStartOffset"));
	L=qsladdstr(L,strdup("ShortDRXCycle"));
	L=qsladdstr(L,strdup("LongDRXCycle"));
	L=qsladdstr(L,strdup("DRXEnabled"));
	L=qsladdstr(L,strdup("DRXInactivityTimer"));
	L=qsladdstr(L,strdup("DRXRetransmissionTimer"));
	L=qsladdstr(L,strdup("OnDurationTimer"));
	return L;
}
static const struct parameter_t __0807PC23[] = {

	qmaMandatory(0,qmaDynamic("",ShowDRX),"DRX Configuration Parameters"),
	qmaEndParams()
};

/*******************************************************************************
 * Function Name  : showQRxLevMinOffset
 * Description    : To find tag showQRxLevMinOffset
 * Inputs         : pointer to void 
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **showQRxLevMinOffset(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("QRxLevMinOffset"));
	L=qsladdstr(L,strdup("QRxLevMinSIB1"));
	L=qsladdstr(L,strdup("QRxLevMinSIB3"));
	L=qsladdstr(L,strdup("SIntraSearch"));
	L=qsladdstr(L,strdup("SNonIntraSearch"));
	L=qsladdstr(L,strdup("TReselectionEUTRA"));
	L=qsladdstr(L,strdup("TReselectionEUTRASFHigh"));
	L=qsladdstr(L,strdup("TReselectionEUTRASFMedium"));
	L=qsladdstr(L,strdup("ThreshServingLow"));
	L=qsladdstr(L,strdup("CellReselectionPriority"));
	return L;
}

static const struct parameter_t __0807PC18[] = {

	qmaMandatory(0,qmaDynamic("",showQRxLevMinOffset),"To fetch value of intrafreq parameter "),
	qmaEndParams()
};


/*******************************************************************************
 * Function Name  : ulpower 
 * Description    : To find tag ulpower
 * Inputs         : pointer to void 
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **ulpower(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("P0NominalPUCCH"));
	L=qsladdstr(L,strdup("P0NominalPUSCH"));
	L=qsladdstr(L,strdup("Alpha"));
	return L;
}

static const struct parameter_t __0807HS04[] = {

	qmaMandatory(0,qmaDynamic("",ulpower),"To fetch value of ULPowerControl parameter"),
	qmaEndParams()
};

/*******************************************************************************
 * Function Name  : prachparams
 * Description    : To find tag prachparams
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **prachparams(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */

	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("FreqOffset"));
	L=qsladdstr(L,strdup("ZeroCorrelationZoneConfig"));
	L=qsladdstr(L,strdup("RootSequenceIndex"));
	L=qsladdstr(L,strdup("HighSpeedFlag"));
	L=qsladdstr(L,strdup("ConfigurationIndex"));

	return L;
}

static const struct parameter_t __0807HS09[] = {

	qmaMandatory(0,qmaDynamic("FOR PRACH",prachparams),"To fetch value of PRACH Parameters"),
	qmaEndParams()
};

/*******************************************************************************
 * Function Name  : ulrparams
 * Description    : To find tag ulrparams
 * Inputs         : pointer to void 
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **ulrparams(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("GroupHoppingEnabled"));
	L=qsladdstr(L,strdup("SequenceHoppingEnabled"));

	return L;
}

static const struct parameter_t __0807RK10[] = {

	qmaMandatory(0,qmaDynamic(" ",ulrparams),"To fetch value of ULRS Parameters"),
	qmaEndParams()
};

/*******************************************************************************
 * Function Name  : fun_pusch
 * Description    : To find tag fun_pusch
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **fun_pusch(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("HoppingMode"));
	L=qsladdstr(L,strdup("HoppingOffset"));
	L=qsladdstr(L,strdup("NSB"));

	return L;
}

static const struct parameter_t __0807RK12[] = {

	qmaMandatory(0,qmaDynamic(" ",fun_pusch),"To fetch value of PUSCH Parameters"),
	qmaEndParams()
};
/*******************************************************************************
 * Function Name  : prsparams
 * Description    : To find tag prsparams
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **prsparams(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("NumConsecutivePRSSubframes"));
	L=qsladdstr(L,strdup("NumPRSResourceBlocks"));
	L=qsladdstr(L,strdup("PRSConfigurationIndex"));
	L=qsladdstr(L,strdup("X_VENDOR_PRS_ENABLE"));
	L=qsladdstr(L,strdup("X_VENDOR_MUTING_CONFIG_TWO"));
	L=qsladdstr(L,strdup("X_VENDOR_MUTING_CONFIG_FOUR"));
	L=qsladdstr(L,strdup("X_VENDOR_MUTING_CONFIG_EIGHT"));
	L=qsladdstr(L,strdup("X_VENDOR_MUTING_CONFIG_SIXTEEN"));
	L=qsladdstr(L,strdup("X_VENDOR_PRS_TRANSMISSION_POWER"));
	L=qsladdstr(L,strdup("X_VENDOR_LATITUDE_SIGN"));
	L=qsladdstr(L,strdup("X_VENDOR_DEG_OF_LATITUDE"));
	L=qsladdstr(L,strdup("X_VENDOR_DEG_OF_LONGITUDE"));
	L=qsladdstr(L,strdup("X_VENDOR_DIR_OF_ALTITUDE"));
	L=qsladdstr(L,strdup("X_VENDOR_ALTITUDE"));
	L=qsladdstr(L,strdup("X_VENDOR_UNCERTAINTY_SEMI_MAJOR"));
	L=qsladdstr(L,strdup("X_VENDOR_UNCERTAINTY_SEMI_MINOR"));
	L=qsladdstr(L,strdup("X_VENDOR_ORIENTATION_OF_MAJOR_AXIS"));
	L=qsladdstr(L,strdup("X_VENDOR_UNCERTAINTY_ALTITUDE"));
	L=qsladdstr(L,strdup("X_VENDOR_CONFIDENCE"));
	return L;
}

static const struct parameter_t __0807HS22[] = {

	qmaMandatory(0,qmaDynamic(" ",prsparams),"To fetch value of PRS Parameters"),
	qmaEndParams()
};

/*******************************************************************************
 * Function Name  : pucchparams
 * Description    : To find tag pucchparams
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **pucchparams(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("N1PUCCHAN"));
	L=qsladdstr(L,strdup("NCSAN"));
	L=qsladdstr(L,strdup("NRBCQI"));
	L=qsladdstr(L,strdup("DeltaPUCCHShift"));
    /* SPR 22311 Changes - Obselete Parameter Deleted */
	return L;
}
static const struct parameter_t __0807HS505[] = {

	qmaMandatory(0,qmaDynamic(" ",pucchparams),"To fetch value of PUCCH Parameters"),
	qmaEndParams()
};
/*******************************************************************************
 * Function Name  : ant_info_param
 * Description    : To find tag ant_info_param
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **ant_info_param(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("AntennaPortsCount"));
	return L;
}
static const struct parameter_t __0807HS34[] = {

	qmaMandatory(0,qmaDynamic(" ",ant_info_param),"To fetch value of AntennaPortsCount"),
	qmaEndParams()
};

static const struct command_t __0807HS03[] = {

	qmaCommand( "AntennaInfo", __0807HS34, oam_qcli_send_command, "Physical layer parameters related to AntennaInfo " ),
	qmaCommand( "ULPowerControl", __0807HS04, oam_qcli_send_command, "Physical layer parameters related to ULPowerControl " ),
	qmaCommand( "PRACH", __0807HS09,oam_qcli_send_command , "Physical layer parameters related to PRACH"),  
	qmaCommand( "PUSCH", __0807RK12,oam_qcli_send_command , "To get physical layer parameters related to PUSCH"),
	qmaCommand( "PUSCH-ULRS",__0807RK10 ,oam_qcli_send_command , "To get physical layer parameters related to ULRS"),
	qmaCommand( "PUCCH", __0807HS505,oam_qcli_send_command , "Physical layer parameters related to PUCCH"),
	qmaCommand( "PRS", __0807HS22,oam_qcli_send_command , "Physical layer parameters related to PRS"),
	qmaCommand( "PDSCH", __0807PC12, oam_qcli_send_command, "Physical layer parameters related to PDSCH" ),
	qmaCommand( "SRS", __0807PC19, oam_qcli_send_command, "Physical layer parameters related to SRS" ),
    /* EMBMS Changes Start */
    qmaCommand("MBSFN", mbsfn_params, oam_qcli_send_command,
            "Physical layer parameters related to MBSFN."),
    /* EMBMS Changes End */
	qmaEndCommands()
};

/*******************************************************************************
 * Function Name  : rachparams
 * Description    : To find tag rachparams
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **rachparams(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */

	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("NumberOfRaPreambles"));
	L=qsladdstr(L,strdup("PreambleInitialReceivedTargetPower"));
	L=qsladdstr(L,strdup("ResponseWindowSize"));
    L=qsladdstr(L,strdup("MaxHARQMsg3Tx"));                                
	L=qsladdstr(L,strdup("ContentionResolutionTimer"));
	L=qsladdstr(L,strdup("MessagePowerOffsetGroupB"));
	L=qsladdstr(L,strdup("MessageSizeGroupA"));
	L=qsladdstr(L,strdup("PowerRampingStep"));
	L=qsladdstr(L,strdup("PreambleTransMax"));
	L=qsladdstr(L,strdup("SizeOfRaGroupA"));
	return L;
}


static const struct parameter_t __0807HS06[] = {
	qmaMandatory(0,qmaDynamic("RACH ",rachparams),"To fetch value of RACH Parameters"),
	qmaEndParams()
};

static const struct command_t __0807HS05[] = {
	qmaCommand( "RACH", __0807HS06, oam_qcli_send_command, "To get  MAC layer parameters " ),
	qmaCommand( "DRX", __0807PC23, oam_qcli_send_command, "[To show parameters in DRX]" ),
	qmaEndCommands()
};

/*******************************************************************************
 * Function Name  : rfparams
 * Description    : To find tag rfparams
 * Inputs         : pointer to void 
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **rfparams(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("DLBandwidth"));
	L=qsladdstr(L,strdup("EARFCNDL"));
	L=qsladdstr(L,strdup("ULBandwidth"));
	L=qsladdstr(L,strdup("EARFCNUL"));
	L=qsladdstr(L,strdup("FreqBandIndicator"));
	L=qsladdstr(L,strdup("ReferenceSignalPower"));
	L=qsladdstr(L,strdup("PBCHPowerOffset"));
	L=qsladdstr(L,strdup("PSCHPowerOffset"));
	L=qsladdstr(L,strdup("SSCHPowerOffset"));
	L=qsladdstr(L,strdup("PhyCellID"));
	return L;


}
static const struct parameter_t __0807HS07[] = {

	qmaMandatory(0,qmaDynamic(" ",rfparams),"To fetch value of RF parameter"),
	qmaEndParams()
};

/*******************************************************************************
 * Function Name  : rrcparams
 * Description    : To find tag rrcparams
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **rrcparams(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("N311"));
	L=qsladdstr(L,strdup("T310"));
	L=qsladdstr(L,strdup("T311"));
	L=qsladdstr(L,strdup("N310"));
	L=qsladdstr(L,strdup("T300"));
	L=qsladdstr(L,strdup("T301"));
	L=qsladdstr(L,strdup("T302"));
	L=qsladdstr(L,strdup("T304EUTRA"));
	L=qsladdstr(L,strdup("T304IRAT"));
	L=qsladdstr(L,strdup("T320"));
	return L;
}

static const struct parameter_t __0807HS08[] = {

	qmaMandatory(0,qmaDynamic("RRCTimers ",rrcparams),"To fetch value of RRCTimers"),
	qmaEndParams()
};

/*******************************************************************************
 * Function Name  : commonparams
 * Description    : To find tag commonparams
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **commonparams(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("NCellChangeHigh"));
	L=qsladdstr(L,strdup("NCellChangeMedium"));
	L=qsladdstr(L,strdup("Qhyst"));
	L=qsladdstr(L,strdup("QHystSFHigh"));
	L=qsladdstr(L,strdup("QHystSFMedium"));
	L=qsladdstr(L,strdup("TEvaluation"));
	L=qsladdstr(L,strdup("THystNormal"));
	return L;
}


static const struct parameter_t __0807HS13[] = {

	qmaMandatory(0,qmaDynamic(" ",commonparams),"To get values of Parameters of Common"),
	qmaEndParams()
};


/* START SPR_6719_FIX */

/*******************************************************************************
 * Function Name  : grpparams
 * Description    : To find tag grpparams
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **grpparams(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("QRxLevMin"));
	L=qsladdstr(L,strdup("CellReselectionPriority"));
	L=qsladdstr(L,strdup("ThreshXHigh"));
	L=qsladdstr(L,strdup("ThreshXLow"));
	L=qsladdstr(L,strdup("BandIndicator"));
	L=qsladdstr(L,strdup("BCCHARFCN"));

	return L;
}


static const struct parameter_t __0807HS21[] = {

	qmaMandatory(0,qmaDynamic(" ",grpparams),"Mandatory parameter for GERANFreqGroup"),
	qmaEndParams()
};
/*******************************************************************************
 * Function Name  : grparams
 * Description    : To find tag grparams 
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **grparams(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	/* COV Fix 55818*/ 
	*L = 0;
	L=qsladdstr(L,strdup("TReselectionGERAN"));
	L=qsladdstr(L,strdup("TReselectionGERANSFMedium"));
	L=qsladdstr(L,strdup("TReselectionGERANSFHigh"));
	L=qsladdstr(L,strdup("GERANFreqGroupNumberOfEntries"));

	return L;
}


static const struct parameter_t __0807HS20[] = {

	qmaMandatory(0,qmaDynamic(" ",grparams),"Mandatory parameter for GERAN"),
	qmaEndParams()
};

/* END SPR_6719_FIX */

/*******************************************************************************
 * Function Name  : fn_UTRANFDDFreq
 * Description    : To find tag fn_UTRANFDDFreq
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **fn_UTRANFDDFreq(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("PMaxUTRA"));

	return L;
}

static const struct parameter_t __0807RK24[] = {

	qmaMandatory(0,qmaDynamic(" ",fn_UTRANFDDFreq),"Mandatory parameter for UTRANFDDFreq"),
	qmaEndParams()
};



/* SPR 13805 START */
/* Code Removed */
/* SPR 13805 END */

static const struct command_t __0807RK20[] = {
/* SPR 13805 START */
    qmaCommandNoParam( "TReselectionUTRA", oam_qcli_send_command_no_param, "To get parameters related to TReselectionUTRA" ),
    qmaCommandNoParam( "TReselectionUTRASFHigh", oam_qcli_send_command_no_param, "To get parameter related to TReselectionUTRASFHigh" ),
    qmaCommandNoParam( "TReselectionUTRASFMedium", oam_qcli_send_command_no_param, "To get parameter related to TReselectionUTRASFMedium" ),
/* SPR 13805 END*/
	qmaCommand( "UTRANFDDFreq", __0807RK24, oam_qcli_send_command, "To get parameters related to UTRANFDDFreq" ),

	qmaEndCommands()

};


static const struct command_t __0807HS18[] = {
	qmaMode( "UTRA","Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility IdleMode IRAT UTRA  ", __0807RK20, "TO Show UTRA Parameter" ),
	/* coverity 52786 52787*/	
	qmaCommand( "GERAN", __0807HS20,oam_qcli_send_command, "To Show GERAN Parameter" ),
	qmaCommand( "GERAN-GERANFreqGroup", __0807HS21, oam_qcli_send_command, "To Show parameters related to GERANFreqGroup" ),
	qmaEndCommands()

};

static const struct command_t __0807HS12[] = {

	qmaCommand( "Common", __0807HS13, oam_qcli_send_command, "\tTo show parameters related to Common" ),
	qmaMode( "IRAT","Show Config FAPService CellConfig LTE RAN Mobility IdleMode IRAT  ", __0807HS18, "\tTo show IRAT Parameter" ),
	qmaCommand( "IntraFreq", __0807PC18, oam_qcli_send_command, "\tTo show IntraFreq parameters" ),        
	qmaEndCommands()

};              

/*******************************************************************************
 * Function Name  : ShowIRAT
 * Description    : To find tag ShowIRAT
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **ShowIRAT(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("QoffsettUTRA"));
	L=qsladdstr(L,strdup("FilterCoefficientUTRA"));
	L=qsladdstr(L,strdup("MeasQuantityUTRAFDD"));
	L=qsladdstr(L,strdup("B1ThresholdUTRARSCP"));
	L=qsladdstr(L,strdup("B1ThresholdUTRAEcN0"));
	L=qsladdstr(L,strdup("QoffsetGERAN"));
	L=qsladdstr(L,strdup("FilterCoefficientGERAN"));
	L=qsladdstr(L,strdup("B1ThresholdGERAN"));
	L=qsladdstr(L,strdup("QoffsetCDMA2000"));
	L=qsladdstr(L,strdup("MeasQuantityCDMA2000"));
	L=qsladdstr(L,strdup("B1ThresholdCDMA2000"));
	L=qsladdstr(L,strdup("B2Threshold2UTRARSCP"));
	L=qsladdstr(L,strdup("B2Threshold2UTRAEcN0"));
	L=qsladdstr(L,strdup("B2Threshold2GERAN"));
	L=qsladdstr(L,strdup("B2Threshold2CDMA2000"));
	L=qsladdstr(L,strdup("Hysteresis"));
	L=qsladdstr(L,strdup("TimeToTrigger"));
	L=qsladdstr(L,strdup("MaxReportCells"));
	L=qsladdstr(L,strdup("ReportInterval"));
	L=qsladdstr(L,strdup("ReportAmount"));
    /* SPR 13020 START */
    L=qsladdstr(L,strdup("B2Threshold1RSRP"));
    L=qsladdstr(L,strdup("B2Threshold1RSRQ"));
    /* SPR 13020 END */

	return L;
}

static const struct parameter_t __0807PC52[] = {

	qmaMandatory(0,qmaDynamic(" ",ShowIRAT),"Connected mode IRAT Parameters"),
	qmaEndParams()
};

/*******************************************************************************
 * Function Name  : ShowEutra
 * Description    : To find tag ShowEutra 
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **ShowEutra(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("FilterCoefficientRSRP"));
	L=qsladdstr(L,strdup("FilterCoefficientRSRQ"));
	L=qsladdstr(L,strdup("A1ThresholdRSRP"));
	L=qsladdstr(L,strdup("A1ThresholdRSRQ"));
	L=qsladdstr(L,strdup("A2ThresholdRSRP"));
	L=qsladdstr(L,strdup("A2ThresholdRSRQ"));
	L=qsladdstr(L,strdup("A3Offset"));
	L=qsladdstr(L,strdup("ReportOnLeave"));
	L=qsladdstr(L,strdup("A4ThresholdRSRP"));
	L=qsladdstr(L,strdup("A4ThresholdRSRQ"));
	L=qsladdstr(L,strdup("A5Threshold1RSRP"));
	L=qsladdstr(L,strdup("A5Threshold1RSRQ"));
	L=qsladdstr(L,strdup("A5Threshold2RSRP"));
	L=qsladdstr(L,strdup("A5Threshold2RSRQ"));
	L=qsladdstr(L,strdup("Hysteresis"));
	L=qsladdstr(L,strdup("TimeToTrigger"));
	L=qsladdstr(L,strdup("TriggerQuantity"));
	L=qsladdstr(L,strdup("ReportQuantity"));
	L=qsladdstr(L,strdup("ReportInterval"));
	L=qsladdstr(L,strdup("ReportAmount"));

	return L;
}

static const struct parameter_t __0807PC51[] = {

	qmaMandatory(0,qmaDynamic(" ",ShowEutra),"Connected mode EUTRA Parameters"),
	qmaEndParams()
};

static const struct command_t __0807PC50[] = {

	qmaCommand( "EUTRA", __0807PC51, oam_qcli_send_command, "To get parameters in EUTRA" ),
	qmaCommand( "IRAT", __0807PC52, oam_qcli_send_command, "To get parameters in IRAT" ),
	qmaEndCommands()

};

static const struct command_t __0807HS11[] = {
	qmaMode( "IdleMode","Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility IdleMode ", __0807HS12, "To show IdleMode parameters" ),
	qmaMode( "ConnMode","Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility ConnMode  ", __0807PC50, "To show Connected parameters" ),
	qmaEndCommands()
};

/*******************************************************************************
 * Function Name  : qosparams
 * Description    : To find tag qosparams
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **qosparams(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("PacketDelayBudget"));
	L=qsladdstr(L,strdup("Priority"));
	L=qsladdstr(L,strdup("QCI"));
	L=qsladdstr(L,strdup("Type"));
	L=qsladdstr(L,strdup("X_VENDOR_PACKETERRORLOSSRATE"));
	return L;
}


static const struct parameter_t __0807HS15[] = {

	qmaMandatory(0,qmaDynamic(" ",qosparams),"Mandatory_param for QoS"),
	qmaEndParams()
};
/*******************************************************************************
 * Function Name  : srbparams
 * Description    : To find tag srbparams
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **srbparams(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("DefaultConfiguration"));
	L=qsladdstr(L,strdup("MaxRetxThreshold"));
	L=qsladdstr(L,strdup("PollByte"));
	L=qsladdstr(L,strdup("PollPDU"));
	L=qsladdstr(L,strdup("TPollRetransmit"));
	L=qsladdstr(L,strdup("TReordering"));
	L=qsladdstr(L,strdup("TStatusProhibit"));
	return L;
}

static const struct parameter_t __0807HS17[] = {

	qmaMandatory(0,qmaDynamic(" ",srbparams),"Mandatory parameters for SRB1"),
	qmaEndParams()
};

static const struct parameter_t __0807HS27[] = {

	qmaMandatory(0,qmaDynamic(" ",srbparams),"Mandatory parameters for SRB2"),
	qmaEndParams()
};

static const struct command_t __0807HS16[] = {

	qmaCommand( "SRB1", __0807HS17, oam_qcli_send_command, "To get parameters related to SRB1" ),
	qmaCommand( "SRB2", __0807HS27, oam_qcli_send_command, "To get parameters related to SRB2" ),
	qmaEndCommands()

};
/*******************************************************************************
 * Function Name  : fun_PLMNLI
 * Description    : To find tag fun_PLMNLI
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/

static char  **fun_PLMNLI(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("PLMNID"));
	L=qsladdstr(L,strdup("IsPrimary"));
	L=qsladdstr(L,strdup("Enable"));
	L=qsladdstr(L,strdup("CellReservedForOperatorUse"));

	return L;
}
static const struct parameter_t  __0807AJPLI[] = {

	qmaMandatory(0,qmaDynamic(" ",fun_PLMNLI),"Mandatory_param for PLMNList"),
	qmaEndParams()
};
/*******************************************************************************
 * Function Name  : cell_config_epc_params
 * Description    : To find tag cell_config_epc_params
 * Inputs         : pointer to void 
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **cell_config_epc_params(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("EAID"));
	L=qsladdstr(L,strdup("TAC"));
	L=qsladdstr(L,strdup("PLMNListNumberOfEntries"));
	L=qsladdstr(L,strdup("QoSNumberOfEntries"));
	L=qsladdstr(L,strdup("MaxPLMNListEntries"));
	L=qsladdstr(L,strdup("AllowedCipheringAlgorithmList"));
	L=qsladdstr(L,strdup("AllowedIntegrityProtectionAlgorithmList"));
	return L;
}

static const struct parameter_t display_cell_config_epc[] = {

	qmaMandatory(0,qmaDynamic(" ",cell_config_epc_params),"Tunnel parameters"),
	qmaEndParams()
};
/*******************************************************************************
 * Function Name  : common1params
 * Description    : To find tag common1params
 * Inputs         : pointer to void 
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **common1params(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */

	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("CellIdentity"));
	return L;
}

static const struct parameter_t __0807HS500[] = {

	qmaMandatory(0,qmaDynamic("Common",common1params),"Mandatory_param"),
	qmaEndParams()
};


/*******************************************************************************
 * Function Name  : s1apparams
 * Description    : To find tag s1apparams
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **s1apparams(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("TRelocPrep"));
	L=qsladdstr(L,strdup("TRelocOverall"));
	return L;
}
static const struct parameter_t __0807HS506[] = {
	qmaMandatory(0,qmaDynamic("S1AP",s1apparams),"Mandatory_param"),
	qmaEndParams()
};

static const struct command_t __0807HS02[] = {
	qmaMode( "PHY", "Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN PHY ",__0807HS03 , "To show PHY parameters" ),
	qmaMode( "MAC", "Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN MAC ",__0807HS05 , "To show MAC parameters" ),
	qmaMode( "Mobility", "Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN Mobility ",__0807HS11 , "To show Mobility parameters" ), 
	qmaCommand( "RF", __0807HS07, oam_qcli_send_command, "To show RF parameters" ),
	qmaCommand( "S1AP", __0807HS506, oam_qcli_send_command, "To show S1AP parameters" ),
	qmaCommand( "RRCTimers", __0807HS08, oam_qcli_send_command, "To show RRCTimers parameters" ),
	qmaCommand( "Common", __0807HS500, oam_qcli_send_command, "To Show Common Parameter" ),
	qmaMode( "RLC","Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN RLC ", __0807HS16 , "To show RLC parameters" ),
	qmaCommand( "CellRestriction", __0807PC04, oam_qcli_send_command, "Indicates whether the cell is reserved for operator's use or not"),  
    /* SPR 13970 FIX Start */
    qmaCommand( "CellRestriction-AC_BARRING_FOR_MO_SIGNALLING", __0807PC08, oam_qcli_send_command, "To show CellRestriction-                AC_BARRING_FOR_MO_SIGNALLING parameters"),
    qmaCommand( "CellRestriction-AC_BARRING_FOR_MO_DATA", __0807PC09, oam_qcli_send_command, "To show CellRestriction-                      AC_BARRING_FOR_MO_DATA parameters"),
    qmaCommand( "CellRestriction-SSAC_BARRING_FOR_MMTEL_VOICE_R9", __0807PC0A, oam_qcli_send_command, "To show CellRestriction-             SSAC_BARRING_FOR_MMTEL_VOICE_R9 paramenters"),
    qmaCommand( "CellRestriction-SSAC_BARRING_FOR_MMTEL_VIDEO_R9", __0807PC0B, oam_qcli_send_command, "To show CellRestriction-             SSAC_BARRING_FOR_MMTEL_VIDEO_R9 paramenters"),
    qmaCommand( "CellRestriction-AC_BARRING_FOR_CSFB_R10", __0807PC0C, oam_qcli_send_command, "To show CellRestriction-                     AC_BARRING_FOR_CSFB_R10 paramenters"),
    /* SPR 13970 FIX End */

    qmaEndCommands()
};

/*******************************************************************************
 * Function Name  : cell_config_tunnel_params
 * Description    : To find tag cell_config_tunnel_params
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **cell_config_tunnel_params(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("Enable"));
	L=qsladdstr(L,strdup("TunnelRef"));
	L=qsladdstr(L,strdup("PLMNID"));

	return L;
}

static const struct parameter_t display_cell_config_tunnel[] = {

	qmaMandatory(0,qmaDynamic(" ",cell_config_tunnel_params),"Tunnel parameters"),
	qmaEndParams()
};

/*******************************************************************************
 * Function Name  : cell_config_lte_params
 * Description    : To find tag cell_config_lte_params
 * Inputs         : pointer to void 
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **cell_config_lte_params(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("TunnelNumberOfEntries"));

	return L;
}

/*LTE_OAM_INHANCEMENT_CA_CHANGED */
    /* SPR 14295 FIX START */
/****************************************************************************
 * Function Name  : list_ca_genral_params
 * Description    : To list all the CA general params
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ***************************************************************************/

static char  **list_ca_genral_params(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
    char **L = OAM_NULL;
    
    CLI_QSLALLOC(L);
    *L = 0;
    L = qsladdstr(L, strdup("isCAEligible"));
    L = qsladdstr(L, strdup("minNumUEThruputReport"));
    L = qsladdstr(L, strdup("applicableBitrateDL"));
    L = qsladdstr(L, strdup("isMASA"));
    L = qsladdstr(L, strdup("ScellDeactTimer"));

    return L;
}
/****************************************************************************
 * Function Name  : list_ca_mac_sched_params
 * Description    : To list all the CA MAC scheduler specific params
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ***************************************************************************/

static char  **list_ca_mac_sched_params(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
    char **L = OAM_NULL;

    CLI_QSLALLOC(L);

    *L = 0;
    L = qsladdstr(L, strdup("loadCalcWindowSize"));
    L = qsladdstr(L, strdup("loadDisparityThresh"));
    L = qsladdstr(L, strdup("MCSDiffThresh"));
    L = qsladdstr(L, strdup("PRBUsageWatermark"));
    L = qsladdstr(L, strdup("DLDeactCMDThresh"));
    L = qsladdstr(L, strdup("DLDeactMCSLowThresh"));
    L = qsladdstr(L, strdup("DLQLoadUPThresh"));
    L = qsladdstr(L, strdup("DLQLoadLowThresh"));
    L = qsladdstr(L, strdup("ActDeactTriggerCountThresh"));

    return L;
}
    /* SPR 14295 FIX END */
/*LTE_OAM_INHANCEMENT_CA_CHANGED */

/* eICIC_PHASE_1_2_CHANGES_START */

/*******************************************************************************
 * Function Name  : display_cell_config_eICIC_PARAMS 
 * Description    : To find tag display_cell_config_eICIC_PARAMS 
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
/*SPR 15028 start*/
static char  **abs_info(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
    char **L = NULL; CLI_QSLALLOC(L);
    *L = 0;
    L=qsladdstr(L,strdup("NumAbsReport"));
    L=qsladdstr(L,strdup("AbsUsageLowThreshold"));
    L=qsladdstr(L,strdup("AbsUsageHighThreshold"));

    return L;
}

static char  **abs_pattern(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
    char **L = NULL; CLI_QSLALLOC(L);
    *L = 0;
    L=qsladdstr(L,strdup("LowLoadAbsPattern"));
    L=qsladdstr(L,strdup("VictimAbsPattern"));
    L=qsladdstr(L,strdup("VictimMeasSubset"));

    return L;
}

static char  **display_eICIC_PARAMS(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
    char **L = NULL; CLI_QSLALLOC(L);
    *L = 0;
    L=qsladdstr(L,strdup("eICICProvisionType"));
    L=qsladdstr(L,strdup("eligible_ue"));
    L=qsladdstr(L,strdup("DlSinrThreshLowMark"));
    L=qsladdstr(L,strdup("DlSinrThreshHighMark"));

    return L;
}

static char  **display_eICIC_Timers(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
    char **L = NULL; CLI_QSLALLOC(L);
    *L = 0;
    L=qsladdstr(L,strdup("MacEicicMeasReportPeriodicity"));
    L=qsladdstr(L,strdup("AggressorSelTimer"));
    L=qsladdstr(L,strdup("LoadInformationGuardTimer"));
    L=qsladdstr(L,strdup("LoadInformationCollationTimer"));

    return L;
}
/*SPR 15028 end*/

/* eICIC_PHASE_1_2_CHANGES_END */
static const struct parameter_t display_cell_config_lte[] = {

	qmaMandatory(0,qmaDynamic(" ",cell_config_lte_params),"FAPService Parameters"),
	qmaEndParams()
};

/*LTE_OAM_INHANCEMENT_CA_CHANGED*/ 
    /* SPR 14295 FIX START */
static const struct parameter_t show_ca_mac_sched_params_cmd[] = {
    qmaMandatory(0, qmaDynamic(" ", list_ca_mac_sched_params),
                 "CarrierAggregation MAC scheduler parameters"),

    qmaEndParams()
};


static const struct parameter_t show_ca_general_params_cmd[] = {
    qmaMandatory(0, qmaDynamic(" ", list_ca_genral_params),
                 "CarrierAggregation Parameters"),

    qmaEndParams()
};
    /* SPR 14295 FIX END */

/*LTE_OAM_INHANCEMENT_CA_CHANGED */ 

/* eICIC_PHASE_1_2_CHANGES_START */

/*SPR 15028 start*/
static const struct parameter_t display_abs_info[] = {


    qmaMandatory(0,qmaDynamic(" ",abs_info),"ABS INFO"),
    qmaEndParams()
};

static const struct parameter_t display_abs_pattern[] = {


    qmaMandatory(0,qmaDynamic(" ",abs_pattern),"ABS Pattern"),
    qmaEndParams()
};

static const struct command_t display_abs_info_params[] = {

    qmaCommand( "ABS_INFO", display_abs_info,oam_qcli_send_command, "To get abs_info" ),
    qmaCommand( "ABS_PATTERN",display_abs_pattern,oam_qcli_send_command, "To get abs pattern" ),

    qmaEndCommands()
};
static const struct parameter_t display_cell_config_Params[] = {


    qmaMandatory(0,qmaDynamic(" ",display_eICIC_PARAMS),"eICIC Parameters"),
    qmaEndParams()
};
static const struct parameter_t display_cell_config_eICIC_Timers[] = {


    qmaMandatory(0,qmaDynamic(" ",display_eICIC_Timers),"eICIC Timers"),
    qmaEndParams()
};

static const struct command_t display_cell_config_eICIC_PARAMS[]={
    qmaCommand( "eICICParams", display_cell_config_Params, oam_qcli_send_command, "To get eICICParams" ),
    qmaMode( "AbsInfoParams","Show Config InternetGatewayDevice Services FAPService CellConfig X_VENDOR_eICIC_PARAMS ABS INFO ", display_abs_info_params, "To get abs thresholds and abs patterns" ),
    qmaCommand( "eICICTimers", display_cell_config_eICIC_Timers, oam_qcli_send_command, "To get eICICTimers" ),

    qmaEndCommands()
};

/*SPR 15028 end*/
/* eICIC_PHASE_1_2_CHANGES_END */


static const struct command_t __0807HS99[] = {
	qmaCommand( "Tunnel", display_cell_config_tunnel, oam_qcli_send_command, "To get Tunnel parameters" ),
	qmaMode( "RAN","Show Config InternetGatewayDevice Services FAPService CellConfig LTE RAN ", __0807HS02, "To get RAN parameters" ),
	qmaCommand( "EPC", display_cell_config_epc, oam_qcli_send_command, "To get EPC parameters" ),
	qmaCommand( "EPC-PLMNList", __0807AJPLI, oam_qcli_send_command, "To get parameters related to PLMNList" ),
	qmaCommand( "EPC-QoS[1]", __0807HS15, oam_qcli_send_command, "To get parameters related to QoS[1]" ),
	qmaCommand( "EPC-QoS[2]", __0807HS15, oam_qcli_send_command, "To get parameters related to QoS[2]" ),
	qmaCommand( "EPC-QoS[3]", __0807HS15, oam_qcli_send_command, "To get parameters related to QoS[3]" ),
	qmaCommand( "EPC-QoS[4]", __0807HS15, oam_qcli_send_command, "To get parameters related to QoS[4]" ),
	qmaCommand( "EPC-QoS[5]", __0807HS15, oam_qcli_send_command, "To get parameters related to QoS[5]" ),
	qmaCommand( "EPC-QoS[6]", __0807HS15, oam_qcli_send_command, "To get parameters related to QoS[6]" ),
	qmaCommand( "EPC-QoS[7]", __0807HS15, oam_qcli_send_command, "To get parameters related to QoS[7]" ),
	qmaCommand( "EPC-QoS[8]", __0807HS15, oam_qcli_send_command, "To get parameters related to QoS[8]" ),
	qmaCommand( "EPC-QoS[9]", __0807HS15, oam_qcli_send_command, "To get parameters related to QoS[9]" ),
	qmaEndCommands()
};


    /* SPR 14295 FIX START */
static const struct command_t show_ca_mode[] = {
	qmaCommand("GeneralParams", show_ca_general_params_cmd,
               oam_qcli_send_command,
               "Show CA general parameters"),

	qmaCommand("MACSchedParams", show_ca_mac_sched_params_cmd,
               oam_qcli_send_command,
               "Show CA MAC scheduler specific parameters" ),
	qmaEndCommands()
};
    /* SPR 14295 FIX END */

static const struct command_t __0807HS01[] = {
	qmaMode( "LTE","Show Config InternetGatewayDevice Services FAPService CellConfig LTE ", __0807HS99, "To show LTE parameters" ),
	qmaCommand( "LTE-TunnelNumberOfEntries", display_cell_config_lte, oam_qcli_send_command, "To get LTE TunnelNumberOfEntries Parameter" ),
    /* SPR 14295 FIX START */
	qmaMode("CarrierAggregation", "Show Config InternetGatewayDevice Services "
                                  "FAPService CellConfig CarrierAggregation ",
                                  show_ca_mode, "Show CA parameters" ),
    /* SPR 14295 FIX END */

  /* eICIC_PHASE_1_2_CHANGES_START */
  /*SPR 15028 start*/
  qmaMode( "X_VENDOR_eICIC_PARAMS", "Show Config InternetGatewayDevice Services FAPService CellConfig X_VENDOR_eICIC_PARAMS", display_cell_config_eICIC_PARAMS, "To get X_VENDOR_eICIC_PARAMS" ),
  /*SPR 15028 end*/
  /* eICIC_PHASE_1_2_CHANGES_END */
    qmaEndCommands()
};
/*******************************************************************************
 * Function Name  : lteparams
 * Description    : To find tag lteparams
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **lteparams(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("AccessMode"));
	L=qsladdstr(L,strdup("CSGID"));
	L=qsladdstr(L,strdup("HNBName"));
	L=qsladdstr(L,strdup("MaxCSGMembers"));
	L=qsladdstr(L,strdup("MaxNonCSGMembers"));

	return L;
}

static const struct parameter_t __0807HS101[] = {

	qmaMandatory(0,qmaDynamic(" ",lteparams),"Mandatory_param for LTE"),
	qmaEndParams()
};

static const struct command_t __0807HS100[] = {
	qmaCommand( "LTE", __0807HS101, oam_qcli_send_command, "To get parameters related to LTE" ),       
	qmaEndCommands()
};

/*******************************************************************************
 * Function Name  : fap_control_params
 * Description    : To find tag fap_control_params
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **fap_control_params(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("SelfConfigEvents"));
	L=qsladdstr(L,strdup("EnclosureTamperingDetected"));

	return L;
}

static const struct parameter_t display_fap_control[] = {

	qmaMandatory(0,qmaDynamic(" ",fap_control_params),"FAPControl parameters"),
	qmaEndParams()
};
/*******************************************************************************
 * Function Name  : fap_service_params
 * Description    : To find tag fap_service_params
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **fap_service_params(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("DeviceType"));

	return L;
}

static const struct parameter_t display_fap_service[] = {

	qmaMandatory(0,qmaDynamic(" ",fap_service_params),"FAPService parameters"),
	qmaEndParams()
};

/*******************************************************************************
 * Function Name  : gateway_params
 * Description    : To find tag gateway_params
 * Inputs         : pointer to void 
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **gateway_params(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("SecGWServer1"));
	L=qsladdstr(L,strdup("SecGWServer2"));
	L=qsladdstr(L,strdup("SecGWServer3"));
	L=qsladdstr(L,strdup("S1SigLinkServerList"));
	L=qsladdstr(L,strdup("S1SigLinkPort"));

	return L;
}

static const struct parameter_t display_gateway[] = {

	qmaMandatory(0,qmaDynamic(" ",gateway_params),"Gateway parameters"),
	qmaEndParams()
};


/* SPR 13805 START */
/* Code Removed */
/* SPR 13805 END */

/*******************************************************************************
 * Function Name  : cap_LTE_params
 * Description    : To find tag cap_LTE_params
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **cap_LTE_params(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("BandsSupported"));
	L=qsladdstr(L,strdup("UMTSRxSupported"));
	L=qsladdstr(L,strdup("GSMRxSupported"));
	L=qsladdstr(L,strdup("CDMA2000RxSupported"));
	L=qsladdstr(L,strdup("CDMA2000RxBandsSupported"));
	L=qsladdstr(L,strdup("GSMRxBandsSupported"));
	L=qsladdstr(L,strdup("UMTSRxBandsSupported"));
	L=qsladdstr(L,strdup("DuplexMode"));
	L=qsladdstr(L,strdup("NNSFSupported"));


	return L;
}

static const struct parameter_t display_capabilities_LTE[] = {

	qmaMandatory(0,qmaDynamic(" ",cap_LTE_params),"LTE parameters"),
	qmaEndParams()
};
static const struct command_t __0807HCAP[] = {
/* SPR 13805 START */
    qmaCommandNoParam( "SupportedSystems", oam_qcli_send_command_no_param, "To get Capabilities parameters" ),
    qmaCommandNoParam( "GPSEquipped", oam_qcli_send_command_no_param, "To get Capabilities parameters" ),
    qmaCommandNoParam( "MaxTxPower", oam_qcli_send_command_no_param, "To get Capabilities parameters" ),
    qmaCommandNoParam( "Beacon", oam_qcli_send_command_no_param, "To get Capabilities parameters" ),
/* SPR 13805 END */
	qmaCommand( "LTE", display_capabilities_LTE, oam_qcli_send_command, "To get LTE parameters" ),

	qmaEndCommands()

};

/* SPR 13805 START */
/*SPR 13805 : struct FM_params removed*/
/* SPR 13805 END*/

/*******************************************************************************
 * Function Name  : mgmtsrvr_params
 * Description    : To find tag mgmtsrvr_params
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **mgmtsrvr_params(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("ConnectionRequestURL"));
	L=qsladdstr(L,strdup("URL"));

	return L;
}


static const struct parameter_t __0807VIS[] = {
	qmaMandatory(0,qmaDynamic(" ",mgmtsrvr_params),"Management Server Parameters"),
	qmaEndParams()
};

static const struct command_t __0807AJFM[] = {
/* SPR 13805 START */
    qmaCommandNoParam( "SupportedAlarmNumberOfEntries", oam_qcli_send_command_no_param, "To get Fault Mgmt Parameter" ),
/* SPR 13805 END*/

	qmaEndCommands()

};

/*******************************************************************************
 * Function Name  : lte_2_params
 * Description    : To find tag lte_2_params
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **lte_2_params(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("AdminState"));
	L=qsladdstr(L,strdup("OpState"));
	L=qsladdstr(L,strdup("RFTxStatus"));
	L=qsladdstr(L,strdup("AvailabilityStatus"));

	return L;
}

static const struct parameter_t display_lte_2[] = {
	qmaMandatory(0,qmaDynamic(" ",lte_2_params),"LTE Parameters"),
	qmaEndParams()
};



static const struct command_t __0807FPLT[] = {
	qmaCommand( "Gateway", display_gateway, oam_qcli_send_command, "Gateway Related Parameters" ),
	qmaCommand( "eNBState", display_lte_2, oam_qcli_send_command, "LTE Related Parameters" ),
	qmaEndCommands()
};

static const struct command_t __0807AFP[] = {
	qmaMode( "LTE", "Show Config InternetGatewayDevice Services FAPService FAPControl LTE ", __0807FPLT, "LTE for FAPControl" ),
	qmaCommand( "FAPControl", display_fap_control, oam_qcli_send_command, "FAP Control Related parameters" ),       
	qmaEndCommands()
};


static const struct command_t __0807ADE8[] = {
	qmaMode( "Capabilities", "Show Config InternetGatewayDevice Services FAPService Capabilities ", __0807HCAP, "To show Capabilities parameters" ),
	qmaMode( "FAPControl", "Show Config InternetGatewayDevice Services FAPService FAPControl ", __0807AFP, "To show FAPControl parameters" ),
	qmaMode( "CellConfig", "Show Config InternetGatewayDevice Services FAPService CellConfig ", __0807HS01, "To show CellConfig parameter" ),	
	qmaMode( "AccessMgmt", "Show Config InternetGatewayDevice Services FAPService AccessMgmt ", __0807HS100, "To show AccessMgmt parameter" ),
	qmaCommand( "FAPService", display_fap_service, oam_qcli_send_command, "FAP Control Related Parameters" ),
	qmaEndCommands()
};

/*******************************************************************************
 * Function Name  : device_info_params
 * Description    : To find tag device_info_params
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char  **device_info_params(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("Manufacturer"));
	L=qsladdstr(L,strdup("ManufacturerOUI"));
	L=qsladdstr(L,strdup("ModelName"));
	L=qsladdstr(L,strdup("Description"));
	L=qsladdstr(L,strdup("ProductClass"));
	L=qsladdstr(L,strdup("SerialNumber"));
	L=qsladdstr(L,strdup("HardwareVersion"));
	L=qsladdstr(L,strdup("SoftwareVersion"));
	L=qsladdstr(L,strdup("ModemFirmwareVersion"));
	L=qsladdstr(L,strdup("AdditionalSoftwareVersion"));
	L=qsladdstr(L,strdup("UpTime"));
	L=qsladdstr(L,strdup("FirstUseDate"));

	return L;
}

static const struct parameter_t display_device_info[] = {

	qmaMandatory(0,qmaDynamic(" ",device_info_params),"Device Info parameters"),
	qmaEndParams()
};

/*******************************************************************************
 * Function Name  : configparams
 * Description    : To find tag configparams
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/

static char  **configparams(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("URL"));
	L=qsladdstr(L,strdup("Username"));
	L=qsladdstr(L,strdup("Password"));
	L=qsladdstr(L,strdup("Enable"));
	L=qsladdstr(L,strdup("PeriodicUploadInterval"));
	L=qsladdstr(L,strdup("PeriodicUploadTime"));


	return L;
}

static const struct parameter_t  display_config[] = {

	qmaMandatory(0,qmaDynamic(" ",configparams),"Mandatory_param for Config"),
	qmaEndParams()
};


/* SPR 13805 START */
/*SPR 13805 : struct perfparams removed*/
/* SPR 13805 END*/
static const struct command_t __0807HSPM[] = {
/* SPR 13805 START */
    qmaCommandNoParam( "ConfigNumberOfEntries", oam_qcli_send_command_no_param, "To get config number of entries parameter" ),
/* SPR 13805 END */
	qmaCommand( "Config", display_config, oam_qcli_send_command, "To get Config Parameter" ),

	qmaEndCommands()
};


/* SPR 13805 START */
/*SPR 13805 : struct igd_params removed*/
/* SPR 13805 END*/

static const struct parameter_t __0807NP014[] ={
	qmaMandatory( "UEID", qmaStandard(INTEGER, "0:65535"), "UEID" ),
	qmaEndParams()
};

static const struct command_t __0807NP013[] = {
	qmaCommand("MAC", __0807NP014, oam_qcli_send_command, "To display average SINR and TA of a connected UE"),/* Cov CID 56664 Fix */
	qmaEndCommands()
};

static const struct command_t show_alarm_mode[] = {
	qmaCommandNoParam("CurrentAlarm",
			oam_qcli_send_command_no_param,
			"Display all the raised alarms"),

	qmaCommandNoParam("ExpeditedEvent",
			oam_qcli_send_command_no_param,
			"Display all the expedited alarms raised"),

	qmaCommandNoParam("HistoryEvent",
			oam_qcli_send_command_no_param,
			"Display history of raised alarms"),

	qmaCommandNoParam("QueuedEvent",
			oam_qcli_send_command_no_param,
			"Display all the queued alarms raised"),

	qmaEndCommands()
};


static const struct command_t __0807AFAP[] = {
	qmaMode( "PerfMgmt", "Show Config InternetGatewayDevice FAP PerfMgmt ", __0807HSPM, "To display Performance Management parameters" ),
	qmaEndCommands()
};


static const struct command_t __0807B088[] = {
	qmaMode( "FAPService", "Show Config InternetGatewayDevice Services FAPService ", __0807ADE8, "To display FAPService parameters" ),
	qmaEndCommands()
};
/*******************************************************************************
 * Function Name  : show_vid
 * Description    : To find tag  show_vid
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **show_vid(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("VersionID"));
	return L;
}

/*******************************************************************************
 * Function Name  : show_ctx
 * Description    : To find tag show_ctx
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8 
 ********************************************************************************/
static char **show_ctx(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
    char **L = NULL; CLI_QSLALLOC(L);
    *L = 0;
    L=qsladdstr(L,strdup("CellIndexList"));
    /* SPR 11249 FIX START */
    /* Changed from ActiveCellIndex to CurrentCellIndex */
    L=qsladdstr(L,strdup("CurrentCellIndex"));
    /* SPR 11249 FIX END */
    return L;
}


static char **show_cells(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
    char **L = NULL; CLI_QSLALLOC(L);
    *L = 0;
    L=qsladdstr(L,strdup("RRC"));
    return L;
}
static const struct parameter_t __0806DAVI[] = {
	qmaMandatory(0,qmaDynamic("",show_vid),"to get version ID"),
	qmaEndParams()
};

static const struct parameter_t  CellStatus[] = {
    qmaMandatory(0,qmaDynamic("",show_cells),"to get cell status"),
    qmaEndParams()
};
static const struct parameter_t CellContext[] = {
    qmaMandatory(0,qmaDynamic("",show_ctx),"to get Cell Context Set"),
    qmaEndParams()
};

static const struct command_t __0807B090[] = {
/* SPR 13805 START */
    qmaCommandNoParam( "DeviceSummary", oam_qcli_send_command_no_param, "To get Management server parameters" ),
/* SPR 13805 END */
	qmaCommand( "ManagementServer", __0807VIS, oam_qcli_send_command, "To get Management server" ),
	qmaCommand( "DeviceInfo", display_device_info, oam_qcli_send_command, "To get Device Info" ),
	qmaMode( "Services", "Show Config InternetGatewayDevice Services", __0807B088 , "To Show Services Parameters" ),
	qmaMode( "FAP", "Show Config InternetGatewayDevice FAP ",  __0807AFAP, "To Show FAP Parameter" ),
	qmaMode( "FaultMgmt", "Show Config InternetGatewayDevice FaultMgmt ", __0807AJFM, "To Show FaultMgmt Parameter" ),

	qmaEndCommands()
};

/*******************************************************************************
 * Function Name  : show_midload
 * Description    : To find tag show_midload
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **show_midload(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("Percentage"));
	L=qsladdstr(L,strdup("Action"));
    /* eICIC_PHASE_1_2_CHANGES_START */
    L=qsladdstr(L,strdup("eligible_ue"));
    L=qsladdstr(L,strdup("abs_pattern"));
    /* eICIC_PHASE_1_2_CHANGES_END */
	return L;
}

static const struct parameter_t display_midload[] = {
	qmaMandatory(0,qmaDynamic("",show_midload),"to get mid load level configuration"),
	qmaEndParams()
};

/*******************************************************************************
 * Function Name  : show_highload
 * Description    : To find tagshow_highload
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **show_highload(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("Percentage"));
	L=qsladdstr(L,strdup("Action"));
    /* eICIC_PHASE_1_2_CHANGES_START */
    L=qsladdstr(L,strdup("eligible_ue"));
    L=qsladdstr(L,strdup("abs_pattern"));
    /* eICIC_PHASE_1_2_CHANGES_END */
	return L;
}

static const struct parameter_t display_highload[] = {
	qmaMandatory(0,qmaDynamic("",show_highload),"to get high load level configuration"),
	qmaEndParams()
};

/*******************************************************************************
 * Function Name  : show_overload
 * Description    : To find tag show_overload
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **show_overload(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("Percentage"));
	L=qsladdstr(L,strdup("Action"));
    /* eICIC_PHASE_1_2_CHANGES_START */
    L=qsladdstr(L,strdup("eligible_ue"));
    L=qsladdstr(L,strdup("abs_pattern"));
    /* eICIC_PHASE_1_2_CHANGES_END */
	return L;
}

static const struct parameter_t display_overload[] = {
	qmaMandatory(0,qmaDynamic("",show_overload),"to get high over load level configuration"),
	qmaEndParams()
};

/*******************************************************************************
 * Function Name  : show_NCR
 * Description    : To find tag show_NCR
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **show_NCR(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("ReportingInterval"));
	return L;
}       

static const struct parameter_t display_NCR[] = {
	qmaMandatory(0,qmaDynamic("",show_NCR),"to get neighbor cell report"),
	qmaEndParams()
};

/*******************************************************************************
 * Function Name  : show_SCR
 * Description    : To find tag  show_SCR
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **show_SCR(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("ReportingInterval"));
	return L;
}

static const struct parameter_t display_SCR[] = {
	qmaMandatory(0,qmaDynamic("",show_SCR),"to get serving cell report"),
	qmaEndParams()
};


static const struct command_t __0807B0LD[] = {
	qmaCommand( "ServingCellReport", display_SCR, oam_qcli_send_command, "To get serving cell report value" ),
	qmaCommand( "NeighborCellReport", display_NCR, oam_qcli_send_command, "To get neighbor cell report value" ),
	qmaCommand( "OverLoadLevel", display_overload, oam_qcli_send_command, "To get overLoad level value" ),
	qmaCommand( "HighLoadLevel", display_highload, oam_qcli_send_command, "To get highLoad level value" ),
	qmaCommand( "MidLoadLevel", display_midload, oam_qcli_send_command, "To get midLoad level value" ),
	qmaEndCommands()
};

static const struct command_t __0807B089[] = {
	qmaMode( "InternetGatewayDevice", "Show Config InternetGatewayDevice ", __0807B090 , "To show InternetGatewayDevice parameters" ),
	qmaMode( "LoadReporting", "Show Config LoadReporting", __0807B0LD , "To show LoadReporting parameters" ),

	qmaEndCommands()
};

/*******************************************************************************
 * Function Name  : show_rrc_ue
 * Description    : To find tag show_rrc_ue
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 ********************************************************************************/
static char **show_rrc_ue(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0; /* Cov CID 56679 Fix */ 
	L=qsladdstr(L,strdup("All"));
	return L;
}

static const struct parameter_t display_rrc[] = {
	qmaMandatory(0,qmaDynamic("",show_rrc_ue),"To show RRC UE Status"),
	qmaEndParams()
};

static const struct parameter_t display_mac[] ={
	qmaMandatory( "UEID", qmaStandard(INTEGER, "0:255"), "UEID" ),
	qmaEndParams()
};


/*CSR_00057386*/
/* Cov CID 56665 Fix removed unused code */ 
static const struct command_t __08074FQ2[] = {
	qmaCommand( "MAC", display_mac, oam_qcli_send_command, "To get status of UE" ),
	qmaCommand( "RRC", display_rrc, oam_qcli_send_command, "To get status of UE" ), /* Cov CID 56663 Fix */
	qmaEndCommands()
};

static const struct command_t __080743E4[] = {
	qmaMode( "Status", "Show UE Status ", __08074FQ2, "To Show UE Status[MAC/RRC]" ),
	qmaEndCommands()
};

static const struct command_t __0806DA98[] = {
    qmaMode( "KPI", "Show KPI ", __080743F8, "To show Kpi's [MAC / PDCP/ RRC/ RRM/ M2AP]" ),
	qmaMode( "PerfStats", "Show PerfStats ", __080743K8, "To show PerfStats [PDCP]" ),
	qmaMode( "Log", "Show Log ", __08074AE0, "To show logs [Level / Category]" ),
	qmaMode("Alarm", "Show Alarm ", show_alarm_mode,
			"Display FaultMgmt parameters"),   
	qmaMode( "UE", "Show UE ", __080743E4, "To display the connected UEs status in RRC/MAC" ),
	qmaMode( "Stats", "Show Stats ", __080751B8, "To show Stats [ L3 / L2]" ),
	qmaMode( "Config", "Show Config ", __0807B089, "To show Config Parameter" ),
	qmaMode( "UeSINR", "Show UeSINR ", __0807NP013, "To show UeSINR [MAC]" ),
	qmaCommand( "Version", __0806DAVI, oam_qcli_send_command, "To get Version ID" ),   
    qmaCommand( "CellContext", CellContext, oam_qcli_send_command, "To get Cell Context" ),   
    qmaCommand( "CellStatus", CellStatus, oam_qcli_send_command, "To get Cell Status" ),   
    qmaEndCommands()
};

/* spr 22474 start*/
/*******************************************************************************
 * Function Name  : SET Options
 * Description    : To find different tags/options 
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 *************************************************************************************/
static const struct parameter_t oam_debug_set_option_cmd[] = {
    qmaMandatory(0, qmaStandard(INTEGER, "0:100"), "Option (0 to 100)"),
    qmaEndParams()
};

static const struct command_t oam_debug_set_l2_cmd[] = {
    qmaCommand( "MAC", oam_debug_set_option_cmd, oam_qcli_send_command, "To set MAC Debug Info " ),
    qmaCommand( "RLC", oam_debug_set_option_cmd, oam_qcli_send_command, "To set RLC Debug Info " ),
    qmaCommand( "PDCP", oam_debug_set_option_cmd, oam_qcli_send_command, "To set PDCP Debug Info" ),
    qmaCommand( "EGTPU", oam_debug_set_option_cmd, oam_qcli_send_command, "To set EGTPU Debug Info" ),
    qmaEndCommands()
};

static const struct command_t oam_debug_set_l3_cmd[] = {
    qmaCommand( "RRC", oam_debug_set_option_cmd, oam_qcli_send_command, "To set RRC Debug Info " ),
    qmaCommand( "X2AP", oam_debug_set_option_cmd, oam_qcli_send_command, "To set X2AP Debug Info " ),
    qmaCommand( "S1AP", oam_debug_set_option_cmd, oam_qcli_send_command, "To set S1AP Debug Info" ),
    qmaEndCommands()
};

static const struct command_t oam_debug_set_oam_cmd[] = {
    qmaCommand( "OAM", oam_debug_set_option_cmd, oam_qcli_send_command, "To set OAM Debug Info " ),
    qmaCommand( "TR069", oam_debug_set_option_cmd, oam_qcli_send_command, "To set TR069 Debug Info" ),
    qmaEndCommands()
};

static const struct command_t oam_debug_set_cmd[] = {
	qmaMode( "L2", "DEBUG SET L2 ", oam_debug_set_l2_cmd, "To Set L2 Debug Info [MAC / RLC / PDCP /EGTPU]" ),
    qmaMode( "L3", "DEBUG SET L3 ", oam_debug_set_l3_cmd, "To Set L3 Debug Info [S1AP]" ),
    qmaCommand( "RRM", oam_debug_set_option_cmd, oam_qcli_send_command, "To Set RRM Debug Info [RRM]" ),
    qmaCommand( "SON", oam_debug_set_option_cmd, oam_qcli_send_command, "To Set SON Debug Info [SON]" ),
    qmaMode( "OAM", "DEBUG SET OAM ", oam_debug_set_oam_cmd, "To Set OAM Debug Info [OAM]" ),
	qmaEndCommands()
};

/*******************************************************************************
 * Function Name  : GET Options
 * Description    : To find different tags/options 
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 *************************************************************************************/
static const struct parameter_t oam_debug_get_option_cmd[] = {
    qmaMandatory(0, qmaStandard(INTEGER, "0:100"), "0:MemPool Stats/1:MsgPool Stats(for L2)/0: both MemPool and MsgPool CSPL Stats(For L3/RRM/SON/OAM)"),
    qmaEndParams()
};

static const struct command_t oam_debug_get_l2_cmd[] = {
    qmaCommand( "MAC", oam_debug_get_option_cmd, oam_qcli_send_command, "To show MAC Debug Info" ),
    qmaCommand( "RLC", oam_debug_get_option_cmd, oam_qcli_send_command, "To show RLC Debug Info " ),
    qmaCommand( "PDCP", oam_debug_get_option_cmd, oam_qcli_send_command, "To show PDCP Debug Info" ),
    qmaCommand( "EGTPU", oam_debug_get_option_cmd, oam_qcli_send_command, "To show EGTPU Debug Info" ),
    qmaEndCommands()
};

static const struct command_t oam_debug_get_l3_cmd[] = {
    qmaCommand( "RRC", oam_debug_get_option_cmd, oam_qcli_send_command, "To show RRC Debug Info " ),
    //qmaCommand( "X2AP", oam_debug_get_option_cmd, oam_qcli_send_command, "To show X2AP Debug Info " ),
    //qmaCommand( "S1AP", oam_debug_get_option_cmd, oam_qcli_send_command, "To show S1AP Debug Info" ),
    qmaEndCommands()
};

static const struct command_t oam_debug_get_oam_cmd[] = {
    qmaCommand( "OAM", oam_debug_get_option_cmd, oam_qcli_send_command, "To show OAM Debug Info " ),
    //qmaCommand( "TR069", oam_debug_get_option_cmd, oam_qcli_send_command, "To show TR069 Debug Info" ),
    qmaEndCommands()
};

static const struct command_t oam_debug_get_cmd[] = {
	qmaMode( "L2", "DEBUG GET L2 ", oam_debug_get_l2_cmd, "To Show L2 Debug Info [MAC / RLC / PDCP /EGTPU]" ),
    qmaMode( "L3", "DEBUG GET L3 ", oam_debug_get_l3_cmd, "To Show L3 Debug Info [RRC / X2AP / S1AP]" ),
    qmaCommand( "RRM", oam_debug_get_option_cmd, oam_qcli_send_command, "To Show RRM Debug Info [RRM]" ),
    qmaCommand( "SON", oam_debug_get_option_cmd, oam_qcli_send_command, "To Show SON Debug Info [SON]" ),
    qmaMode( "OAM", "DEBUG GET OAM ", oam_debug_get_oam_cmd, "To Show OAM Debug Info [OAM]" ),
	qmaEndCommands()
};

/*******************************************************************************
 * Function Name  : oam_debug_cmd
 * Description    : To find different tags/options 
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 *************************************************************************************/
static const struct command_t oam_debug_cmd[] = {
    qmaMode( "GET", "DEBUG GET ", oam_debug_get_cmd, "To show Debug info [L2/ L3/ RRC/ RRM/ SON]" ),
    qmaMode( "SET", "DEBUG SET ", oam_debug_set_cmd, "To set Debug info [L2/ L3/ RRC/ RRM/ SON]" ),
    qmaEndCommands()
};
/* spr 22474 end*/

/*******************************************************************************
 * Function Name  : disable_rrm
 * Description    : To find tag disable_rrm
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 *************************************************************************************/

static char  **disable_rrm(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("Radio_Admission_Control"));

	return L;
}
static const struct parameter_t __0806DFB8[] = {
	qmaMandatory(0,qmaDynamic("",disable_rrm),""),
	qmaEndParams()
};


static const struct command_t __0806DC97[] = {
	qmaCommand( "RRM",__0806DFB8, oam_qcli_send_command, "To disable RRM for radio admission control" ),
	qmaCommandNoParam( "ECNCapacityEnhancement",oam_rrm_disable_rac_ecn, "To disable RRM for ECNCapacityEnhancement" ),
	qmaEndCommands()
};

/**CSR 00057534 FIX start**/
/*******************************************************************************
 * Function Name  : enable_rrm
 * Description    : To find tag enable_rrm
 * Inputs         : pointer to void
 * Outputs        : NONE
 * Returns        : pointer to pointer to Char8
 *
 ********************************************************************************/
static char  **enable_rrm(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
	char **L = NULL; CLI_QSLALLOC(L);
	*L = 0;
	L=qsladdstr(L,strdup("Radio_Admission_Control"));
    L=qsladdstr(L,strdup("LoadReporting"));    
	return L;
}
static const struct parameter_t __0806DEB1[] = {
	qmaMandatory(0,qmaDynamic("",enable_rrm),"To enable RRM for RadioAdmissionControl/LoadReporting"),
	qmaEndParams()
};

static const struct command_t __0806DB99[] = {
	qmaCommand( "RRM",__0806DEB1, oam_qcli_send_command, "To enable RRM for functionalities" ),
	/**CSR 00057534 FIX end**/
	qmaCommand( "ECNCapacityEnhancement",__0706E0C0, oam_qcli_send_command, "To enable RRM ECNCapacityEnhancement" ),

	qmaEndCommands()
};

#ifdef OAM_SON_ENABLE
static const struct parameter_t __anr_enable_req[] = {

	qmaOptional( "UECount", "-", qmaStandard(INTEGER, "1:10"), 
			"Optional: Possible values: 1:10"),
	qmaOptional("ReportingInterval",  "-", qmaStandard(INTEGER, "60:1440"),
			"Optional: Possible values(in minutes): 60:1440"),
	qmaOptional("LimitedMode",  "-", qmaStandard(INTEGER, "0:1"),
			"Optional: Possible Values: 0(FALSE)/1(TRUE)"),
	qmaOptional("NoActivityInterval", "-", qmaStandard(INTEGER, "0:65535"),
			"Optional: Possible Values(in minutes): 0:65535"),
	qmaOptional("HOFailureThreshold", "-", qmaStandard(INTEGER, "0:255"),
			"Optional: Possible Values: 0:255"),
	qmaOptional("TimeoutNONRNeighbors", "-", qmaStandard(INTEGER, "0:65535"),
			"Optional: Possible Values(in minutes): 0:65535"),
	qmaOptional("SuspectPCIThreshold", "-", qmaStandard(INTEGER, "0:255"),
			"Optional: Possible Values 0 to 255"),
	qmaOptional("ClearSuspectPCIThreshold", "-", qmaStandard(INTEGER, "0:255"),
			"Optional: Possible Values: 0:255"),
    /* SPR 20653 Fix Start */
	qmaOptional("UEThroughputDLANRThreshold", "-", qmaStandard(INTEGER, "1:255"),
			"Optional: Possible Values: 1:255"),
	qmaOptional("UEThroughputULANRThreshold", "-", qmaStandard(INTEGER, "1:255"),
			"Optional: Possible Values: 1:255"),
    /* SPR 20653 Fix End */
	qmaEndParams()
};
/*eICIC_PHASE_1_2_CHANGES_START*/
static const struct parameter_t __anr_neighbor_default_config_set[] = {

	qmaOptional( "nr_status", "-", qmaStandard(INTEGER, "1:255"), 
			"Optional: Possible values: 1:255 \n\t\t\t"
            "Values Range 1-2 and 255"),
	qmaOptional( "ho_status", "-", qmaStandard(INTEGER, "1:255"), 
			"Optional: Possible values: 1:255\n\t\t\t"
            "Values Range 1-2 and 255"),
	qmaOptional( "x2_status", "-", qmaStandard(INTEGER, "1:255"), 
			"Optional: Possible values: 1:255\n\t\t\t"
            "Values Range 1-7 and 255"),
	qmaOptional( "csg_cell_intf_scheme", "-", qmaStandard(INTEGER, "0:2"), 
			"Optional: Possible values: 0:2\n\t\t\t"
            "0 --> ICIC 1--> EICIC 2--> NO SCHEME "),
	qmaOptional( "pico_cell_intf_scheme", "-", qmaStandard(INTEGER, "0:2"), 
			"Optional: Possible values: 0:2\n\t\t\t"
            "0 --> ICIC 1--> EICIC 2--> NO SCHEME "),
	qmaOptional( "macro_cell_intf_scheme", "-", qmaStandard(INTEGER, "0:2"), 
			"Optional: Possible values: 0:2\n\t\t\t"
            "0 --> ICIC 1--> EICIC 2--> NO SCHEME "),
    qmaEndParams()
};

/*bug_13893_start*/
static char  **nbor_default_config(const void *unused)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(unused);
    /* SPR 17777 FIX END */
   char **L = NULL; CLI_QSLALLOC(L);
    *L = 0;
    L=qsladdstr(L,strdup("nr_status"));
    L=qsladdstr(L,strdup("ho_status"));
    L=qsladdstr(L,strdup("x2_status"));
    L=qsladdstr(L,strdup("csg_cell_intf_scheme"));
    L=qsladdstr(L,strdup("pico_cell_intf_scheme"));
    L=qsladdstr(L,strdup("macro_cell_intf_scheme"));

    return L;
}

static const struct parameter_t _oam_get_anr_neighbor_default_config_params[]={

    qmaMandatory(0,qmaDynamic(" ",nbor_default_config),"default_config"),
    qmaEndParams()
};

/*bug_13893_end*/

static const struct command_t __anr_neighbor_default_config_req[] = {
    
    /*bug_13893_start */
    qmaCommand("RETREIVE", _oam_get_anr_neighbor_default_config_params,
            oam_qcli_send_command,"GET anr_neighbor_default_config_req parameters"),

    /*bug_13893_end */
    qmaCommand("SET", __anr_neighbor_default_config_set,
                        qcli_validate_send_optional_param_cmd,
                         "SET anr_neighbor_default_config_req parameters"),
    qmaEndCommands()
};
/*eICIC_PHASE_1_2_CHANGES_END */

static const struct parameter_t __anr_modify_ue_count[] = {
	qmaMandatory("UECount", qmaStandard(INTEGER, "1:10"),
			"UECount: Possible values: 1:10"),
	qmaEndParams()
};

static const struct parameter_t __anr_modify_meas_reporting_interval[] = {
	qmaMandatory("MeasReportingInterval", qmaStandard(INTEGER, "60:1440"),
			"Reporting_Interval: Possible values(in minutes): 60:1440"),
	qmaEndParams()

};

static const struct parameter_t __anr_modify_removal_attributes[] = {
	qmaOptional("NoActivityInterval", "-" , qmaStandard(INTEGER, "0:65535"),
			"Optional: Possible values(in minutes): 0:65535.\n\t\t\t "
			"(0 means the neighbor shall never be autonomously deleted by SON)"),

	qmaOptional("HandoverFailureThreshold", "-", qmaStandard(INTEGER, "0:255"),
			"Optional: Possible values: 0:255.\n\t\t\t"
			"(0 means the neighbor shall never be autonomously deleted by SON)"),

	qmaOptional("TimeoutNoNRNeighbors", "-", qmaStandard(INTEGER, "0:65535"),
			"Optional: Possible values(in minutes) : 0:65535.\n\t\t\t "
			"(0 means the neighbor shall never be autonomously deleted by SON)"),

	qmaEndParams()
};

static const struct parameter_t __anr_modify_pci_confusion_cfg[] = {
	qmaOptional("SuspectPCIThreshold", "-", qmaStandard(INTEGER, "0:255"),
			"Optional: Possible values: 0:255.\n\t\t\t "
			"(0 means the PCI will never be considered for suspicion)"),

	qmaOptional("ClearSuspectPCIThreshold", "-", qmaStandard(INTEGER, "0:255"),
			"Optional: Possible values: 0:255.\n\t\t\t "
			"(0 means the UE will not be asked for ECGI for the PCI "
			"under suspicion)"),

	qmaEndParams()
};

/* SPR 20653 Fix Start */
static const struct parameter_t __anr_modify_attribute[] = {
	qmaOptional("UEThroughputDLANRThreshold", "-", qmaStandard(INTEGER, "1:255"),
			"Optional: Possible values: 1:255.\n\t\t\t "
			"(Value is in kbps)"),

	qmaOptional("UEThroughputULANRThreshold", "-", qmaStandard(INTEGER, "1:255"),
			"Optional: Possible values: 1:255.\n\t\t\t "
			"(Value is in kbps)"),

	qmaEndParams()
};
/* SPR 20653 Fix End */
static const struct command_t __anr_modify[] = {
	qmaCommand("UE_COUNT", __anr_modify_ue_count,
			oam_qcli_send_command, "UE Count for ANR measurements"),

	qmaCommand("MEAS_REPORTING_INTERVAL", __anr_modify_meas_reporting_interval,
			oam_qcli_send_command,
			"Mmodify the ANR preparation interval"),

	qmaCommand("REMOVAL_ATTRIBUTES", __anr_modify_removal_attributes,
			qcli_validate_send_optional_param_cmd,
			"Modify neighbor relation removal attributes"),

	qmaCommand("PCI_CONFUSION_CFG", __anr_modify_pci_confusion_cfg,
			qcli_validate_send_optional_param_cmd,
			"Modify PCI Confusion/Conflict configuration"), 
    /* SPR 20653 Fix Start */
	qmaCommand("ATTRIBUTE", __anr_modify_attribute,
			oam_qcli_send_command,
			"Modify the ANR attribute"),
    /* SPR 20653 Fix End */
	qmaEndCommands()
};
/* SPR 11467 START */
static const struct parameter_t _aps_enable_par[] = {
        qmaMandatory(0, qmaStandard(INTEGER, "0:1"), "ENABLE :1 /DISABLE :0"),
        qmaEndParams()
};

static const struct command_t son_aps_mode[] = {
    qmaCommand("PCI-MODE", _aps_enable_par, oam_qcli_send_command,"To Enable/Disable son aps "),
    
    qmaEndCommands()
};
/* SPR FIX 11266 + */

static const struct parameter_t __anr_add_meas_config_eutran[] = {
    /* OAM BCOM Code Comment Changes Start */
    /* SPR 14651 START */
/* SPR_18782 Fix Start */
    qmaKeyword("Enable"),
    qmaMandatory("Enable",qmaStandard(INTEGER, "0:1"), "Enable: 0-1"),
/* SPR_18782 Fix End */
    /* SPR 14651 END */
    /* OAM BCOM Code Comment Changes End */
	qmaKeyword("EUTRACarrierARFCN"),
	qmaMandatory("EUTRACarrierARFCN", qmaStandard(INTEGER, "0:65535"),
			     "Possible values: 0:65535"),

	qmaKeyword("X_VENDOR_MEAS_BANDWIDTH_FOR_EARFCN"),
	qmaMandatory("X_VENDOR_MEAS_BANDWIDTH_FOR_EARFCN",
                 qmaStandard(CHOICE, "BW6|BW15|BW25|BW50|BW75|BW100"),
                 "Possible values: BW6, BW15, BW25, BW50, BW75, BW100"),

	qmaKeyword("X_VENDOR_PRESENCE_ANTENNA_PORT1"),
	qmaMandatory("X_VENDOR_PRESENCE_ANTENNA_PORT1",
			    qmaStandard(INTEGER, "0:1"), "Possible values: 0:1"),

	qmaKeyword("X_VENDOR_NEIGHBOR_CELL_CONFIG"),
	qmaMandatory("X_VENDOR_NEIGHBOR_CELL_CONFIG",
			    qmaStandard(INTEGER, "0:3"), "Possible values: 0:3"),

	qmaKeyword("QOffsetFreq"),
	qmaMandatory("QOffsetFreq",
                 qmaStandard(CHOICE, "-24|-22|-20|-18|-16|-14|-12|-10|-8|"
                                     "-6|-5|-4|-3|-2|-1|0|1|2|3|4|5|6|8|10|"
                                     "12|14|16|18|20|22|24"),
			     "Accepted Input(dBm): -24, -22, -20, -18,"
			     "\n\t\t\t -16, -14, -12, -10, -8, -6, "
			     "\n\t\t\t -5, -4, -3, -2, -1, 0, 1, 2, 3, 4,"
			     "\n\t\t\t 5, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24"),

	qmaKeyword("QRxLevMinSIB5"),
	qmaMandatory("QRxLevMinSIB5", qmaStandard(INTEGER, "-70:-22"),
			    "Possible values: -70:-22"),

    /* + SRR_17523_FIX */
	qmaKeyword("PMax"),
	qmaMandatory("PMax", qmaStandard(INTEGER, "-30:33"),
			    "Possible values: -30:33"),
    /* - SRR_17523_FIX */

	qmaKeyword("TReselectionEUTRA"),
	qmaMandatory("TReselectionEUTRA", qmaStandard(INTEGER, "0:7"),
			    "Possible values: 0:7"),

	qmaKeyword("TReselectionEUTRASFMedium"),
	qmaMandatory("TReselectionEUTRASFMedium",
                qmaStandard(CHOICE,"25|50|75|100"),
			    "Accepted Input: 25, 50, 75, 100"),

	qmaKeyword("TReselectionEUTRASFHigh"),
	qmaMandatory("TReselectionEUTRASFHigh",
                qmaStandard(CHOICE,"25|50|75|100"),
			    "Accepted Input: 25, 50, 75, 100"),

	qmaKeyword("ThreshXHigh"),
	qmaMandatory("ThreshXHigh", qmaStandard(INTEGER, "0:31"),
			    "Possible values: 0:31"),

	qmaKeyword("ThreshXLow"),
	qmaMandatory("ThreshXLow", qmaStandard(INTEGER, "0:31"),
			    "Possible values: 0:31"),

	qmaKeyword("CellReselectionPriority"),
	qmaMandatory("CellReselectionPriority", qmaStandard(INTEGER, "0:7"),
			    "Possible values: 0:7"),

    /* SPR 14651 START/END Code Removed */

	qmaKeyword("X_VENDOR_THRESHX_HIGHQ_R9"),
	qmaMandatory("X_VENDOR_THRESHX_HIGHQ_R9",
			    qmaStandard(INTEGER, "0:31"), "Possible values: 0:31"),

	qmaKeyword("X_VENDOR_THRESHX_LOWQ_R9"),
	qmaMandatory("X_VENDOR_THRESHX_LOWQ_R9",
			    qmaStandard(INTEGER, "0:31"), "Possible values: 0:31"),

	qmaOptional("X_VENDOR_Q_QUAL_MIN_R9", "-",
                qmaStandard(INTEGER, "-34:-3"),
			    "Possible values: -34:-3"),

	qmaOptional("X_VENDOR_OPEN_PHY_CELLID_START", "-",
			qmaStandard(INTEGER, "0:503"), "Possible values: 0:503"),

    /* SPR 12920 START */
	qmaOptional("X_VENDOR_OPEN_PHY_CELLID_RANGE", "-",
			qmaStandard(CHOICE, "n4|n8|n12|n16|n24|n32|n48|n64|n84|n96|n128|n168|n252|n504"),
            "Possible values: n4(0),n8(1),n12(2),n16(3),n24(4),n32(5),n48(6),n64(7),n84(8),n96(9),n128(10),n168(11),n252(12),n504(13)"),
    /* SPR 12920 END */

	qmaOptional("X_VENDOR_CSG_PHY_CELLID_START", "-",
			qmaStandard(INTEGER, "0:503"), "Possible values: 0:503"),

    /* SPR 12920 START */
	qmaOptional("X_VENDOR_CSG_PHY_CELLID_RANGE", "-",
			qmaStandard(CHOICE, "n4|n8|n12|n16|n24|n32|n48|n64|n84|n96|n128|n168|n252|n504"), 
            "Possible values: n4(0),n8(1),n12(2),n16(3),n24(4),n32(5),n48(6),n64(7),n84(8),n96(9),n128(10),n168(11),n252(12),n504(13)"),
    /* SPR 12920 END */

	qmaOptional("X_VENDOR_HYBRID_PHY_CELLID_START", "-",
			qmaStandard(INTEGER, "0:503"), "Possible values: 0:503"),

    /* SPR 12920 START */
	qmaOptional("X_VENDOR_HYBRID_PHY_CELLID_RANGE", "-",
			qmaStandard(CHOICE, "n4|n8|n12|n16|n24|n32|n48|n64|n84|n96|n128|n168|n252|n504"), 
            "Possible values: n4(0),n8(1),n12(2),n16(3),n24(4),n32(5),n48(6),n64(7),n84(8),n96(9),n128(10),n168(11),n252(12),n504(13)"),
    /* SPR 12920 END */

	qmaEndParams()
};

static const struct parameter_t __anr_upd_meas_config_eutran[] = {
	qmaKeyword("InstanceID"),
	qmaMandatory("InstanceID", qmaStandard(INTEGER, "1:12"),
			     "Accepted Input: 1:32"),

	qmaKeyword("EUTRACarrierARFCN"),
	qmaMandatory("EUTRACarrierARFCN", qmaStandard(INTEGER, "0:65535"),
			     "Possible values: 0:65535"),

    /* SPR 14651 START */
    qmaOptional("Enable","-",qmaStandard(INTEGER, "0:1"), "Enable: 0-1"),
    /* SPR 14651 END */
	qmaOptional("X_VENDOR_MEAS_BANDWIDTH_FOR_EARFCN", "-",
                 qmaStandard(CHOICE, "BW6|BW15|BW25|BW50|BW75|BW100"),
                 "Possible values: BW6, BW15, BW25, BW50, BW75, BW100"),

	qmaOptional("X_VENDOR_PRESENCE_ANTENNA_PORT1", "-",
			    qmaStandard(INTEGER, "0:1"), "Possible values: 0:1"),

	qmaOptional("X_VENDOR_NEIGHBOR_CELL_CONFIG", "-",
			    qmaStandard(INTEGER, "0:3"), "Possible values: 0:3"),

	qmaOptional("QOffsetFreq", "-",
                 qmaStandard(CHOICE, "-24|-22|-20|-18|-16|-14|-12|-10|-8|"
                                     "-6|-5|-4|-3|-2|-1|0|1|2|3|4|5|6|8|10|"
                                     "12|14|16|18|20|22|24"),
			     "Accepted Input(dBm): -24, -22, -20, -18,"
			     "\n\t\t\t -16, -14, -12, -10, -8, -6, "
			     "\n\t\t\t -5, -4, -3, -2, -1, 0, 1, 2, 3, 4,"
			     "\n\t\t\t 5, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24"),

	qmaOptional("QRxLevMinSIB5", "-", qmaStandard(INTEGER, "-70:-22"),
			    "Possible values: -70:-22"),

    /* + SRR_17523_FIX */
	qmaOptional("PMax", "-", qmaStandard(INTEGER, "-30:33"),
			    "Possible values: -30:33"),
    /* - SRR_17523_FIX */

	qmaOptional("TReselectionEUTRA", "-", qmaStandard(INTEGER, "0:7"),
			    "Possible values: 0:7"),

	qmaOptional("TReselectionEUTRASFMedium", "-",
                qmaStandard(CHOICE,"25|50|75|100"),
			    "Accepted Input: 25, 50, 75, 100"),

	qmaOptional("TReselectionEUTRASFHigh", "-",
                qmaStandard(CHOICE,"25|50|75|100"),
			    "Accepted Input: 25, 50, 75, 100"),

	qmaOptional("ThreshXHigh", "-", qmaStandard(INTEGER, "0:31"),
			    "Possible values: 0:31"),

	qmaOptional("ThreshXLow", "-", qmaStandard(INTEGER, "0:31"),
			    "Possible values: 0:31"),

	qmaOptional("CellReselectionPriority", "-", qmaStandard(INTEGER, "0:7"),
			    "Possible values: 0:7"),

    /* SPR 14651 START/END Code Removed */

	qmaOptional("X_VENDOR_Q_QUAL_MIN_R9", "-",
                qmaStandard(INTEGER, "-34:-3"),
			    "Possible values: -34:-3"),

	qmaOptional("X_VENDOR_THRESHX_HIGHQ_R9", "-",
			    qmaStandard(INTEGER, "0:31"), "Possible values: 0:31"),

	qmaOptional("X_VENDOR_THRESHX_LOWQ_R9", "-",
			    qmaStandard(INTEGER, "0:31"), "Possible values: 0:31"),

	qmaOptional("X_VENDOR_OPEN_PHY_CELLID_START", "-",
			qmaStandard(INTEGER, "0:503"), "Possible values: 0:503"),

    /* SPR 12920 START */
	qmaOptional("X_VENDOR_OPEN_PHY_CELLID_RANGE", "-",
			qmaStandard(CHOICE,"n4|n8|n12|n16|n24|n32|n48|n64|n84|n96|n128|n168|n252|n504"),
            "Possible values: n4(0),n8(1),n12(2),n16(3),n24(4),n32(5),n48(6),n64(7),n84(8),n96(9),n128(10),n168(11),n252(12),n504(13)"),
    /* SPR 12920 END */

	qmaOptional("X_VENDOR_CSG_PHY_CELLID_START", "-",
			qmaStandard(INTEGER, "0:503"), "Possible values: 0:503"),

    /* SPR 12920 START */
	qmaOptional("X_VENDOR_CSG_PHY_CELLID_RANGE", "-",
			qmaStandard(CHOICE,"n4|n8|n12|n16|n24|n32|n48|n64|n84|n96|n128|n168|n252|n504"), 
            "Possible values: n4(0),n8(1),n12(2),n16(3),n24(4),n32(5),n48(6),n64(7),n84(8),n96(9),n128(10),n168(11),n252(12),n504(13)"),
    /* SPR 12920 END */

	qmaOptional("X_VENDOR_HYBRID_PHY_CELLID_START", "-",
			qmaStandard(INTEGER, "0:503"), "Possible values: 0:503"),

    /* SPR 12920 START */
	qmaOptional("X_VENDOR_HYBRID_PHY_CELLID_RANGE", "-",
			qmaStandard(CHOICE,"n4|n8|n12|n16|n24|n32|n48|n64|n84|n96|n128|n168|n252|n504"), 
            "Possible values: n4(0),n8(1),n12(2),n16(3),n24(4),n32(5),n48(6),n64(7),n84(8),n96(9),n128(10),n168(11),n252(12),n504(13)"),
    /* SPR 12920 END */

	qmaEndParams()
};

static const struct parameter_t meas_config_geran_reselection_params[] = {

	qmaOptional("TReselectionGERAN", "-", qmaStandard(INTEGER, "0:7"),
			    "Possible values: 0:7"),

	qmaOptional("TReselectionGERANSFMedium", "-",
                qmaStandard(CHOICE, "25|50|75|100"),
			    "Accepted Input: 25, 50, 75, 100"),

	qmaOptional("TReselectionGERANSFHigh", "-",
                qmaStandard(CHOICE, "25|50|75|100"),
			    "Accepted Input: 25, 50, 75, 100"),

	qmaEndParams()
};

static const struct parameter_t meas_config_utran_reselection_params[] = {

	qmaOptional("TReselectionUTRA", "-", qmaStandard(INTEGER, "0:7"),
			"Possible values: 0:7"),

	qmaOptional("TReselectionUTRASFMedium", "-",
                qmaStandard(CHOICE, "25|50|75|100"),
			    "Accepted Input: 25, 50, 75, 100"),

	qmaOptional("TReselectionUTRASFHigh", "-",
                qmaStandard(CHOICE, "25|50|75|100"),
			    "Accepted Input: 25, 50, 75, 100"),

	qmaEndParams()
};

static const struct parameter_t __anr_add_meas_config_geran_freq_group[] = {
    /* SPR 14651 START */
   	qmaKeyword("Enable"),
    	qmaMandatory("Enable",qmaStandard(INTEGER, "0:1"), "Enable: 0-1"),
    /* SPR 14651 END */
	qmaKeyword("BCCHARFCN"),
	qmaMandatory("BCCHARFCN", qmaStandard(INTEGER, "0:1023"),
			     "Possible values: 0:1023"),

	qmaKeyword("BandIndicator"),
	qmaMandatory("BandIndicator", qmaStandard(CHOICE, "DCS1800|PCS1900"),
			     "Possible values: DCS1800 or PCS1900"),

	qmaKeyword("X_VENDOR_NCC_PERMITTED"),
	qmaMandatory("X_VENDOR_NCC_PERMITTED", qmaStandard(INTEGER, "0:255"),
			     "A bitmap, where bit N is set to 0 if a BCCH carrier with "
			     "\n\t\t\t NCC = N-1 is not permitted for monitoring and "
			     "\n\t\t\t vice versa"),

	qmaKeyword("ThreshXHigh"),
	qmaMandatory("ThreshXHigh", qmaStandard(INTEGER, "0:31"),
			     "Cell selection RX levelthreshold (in dB) used by the UE "
                 "when reselecting towards a higher priority RAT/ frequency "
                 "than the current serving frequency"),

	qmaKeyword("ThreshXLow"),
	qmaMandatory("ThreshXLow", qmaStandard(INTEGER, "0:31"),
			     "Cell selection RX level threshold (in dB) used by the UE "
                 "when reselecting towards a lower priority RAT/ frequency "
                 "than the current serving frequency"),

	qmaKeyword("QRxLevMin"),
	qmaMandatory("QRxLevMin", qmaStandard(INTEGER, "0:45"),
			    "Minimum required RX level in the GSM cell"),

    /* SPR 14651 START/END Code Removed */

	qmaOptional("X_VENDOR_P_MAX_GERAN", "-", qmaStandard(INTEGER, "0:39"),
			     "Maximum allowed transmission power for GERAN on an "
			     "\n\t\t\t uplink carrier frequency"),

	qmaOptional("CellReselectionPriority", "-", qmaStandard(INTEGER, "0:7"),
			     "Possible values : 0:7"),

	qmaOptional("X_VENDOR_ARFCN_SPACING", "-", qmaStandard(INTEGER, "1:8"),
           "Mandatory if none of explicit arfcns and arfcn bitmap is given"),

	qmaOptional("X_VENDOR_NUM_OF_FOLLOWING_ARFCNS", "-",
           qmaStandard(INTEGER, "0:31"),
           "Mandatory if none of explicit arfcns and arfcn bitmap is given"),

	qmaOptional("X_VENDOR_EXPLICIT_ARFCN_LIST", "-",
                qmaStandard(STRING, "MAXLEN 255"),
                "Comma separated list of arfcns, maximum arfcns should not "
                "\n\t\t\t be more than 31. Mandatory if none of equally "
                "\n\t\t\t spaced arfcns and arfcn bitmap is given"),

	qmaOptional("X_VENDOR_ARFCN_BITMAP", "-",
			   qmaStandard(STRING, "MAXLEN 255"),
			   "Comma separated list of bitmap octets, maximum octets should"
			   "\n\t\t\t not be more than 16. Mandatory if none of equally "
			   "\n\t\t\t spaced arfcns and explicit arfcn list is given"),

	qmaEndParams()
};


static const struct parameter_t __anr_upd_meas_config_geran_freq_group[] = {
	qmaKeyword("InstanceNumber"),
	qmaMandatory("InstanceNumber", qmaStandard(INTEGER, "1:12"),
                 "Instance Number: Mandatory: 1:12"),

	qmaKeyword("BCCHARFCN"),
	qmaMandatory("BCCHARFCN", qmaStandard(INTEGER, "0:1023"),
                 "Possible values: 0:1023"),

	qmaKeyword("BandIndicator"),
	qmaMandatory("BandIndicator", qmaStandard(CHOICE, "DCS1800|PCS1900"),
                 "Possible values: DCS1800 or PCS1900"),

    /* SPR 14651 START */
    qmaOptional("Enable","-",qmaStandard(INTEGER, "0:1"), "Enable: 0-1"),
    /* SPR 14651 END */
	qmaOptional("X_VENDOR_NCC_PERMITTED", "-", qmaStandard(INTEGER, "0:255"),
                "A bitmap, where bit N is set to 0 if a BCCH carrier with "
                "\n\t\t\t NCC = N-1 is not permitted for monitoring and "
                "\n\t\t\t vice versa. Mandatory if if the frequency is not "
                "\n\t\t\t already configured on RRM"),

	qmaOptional("ThreshXHigh", "-", qmaStandard(INTEGER, "0:31"),
			     "Cell selection RX levelthreshold (in dB) used by the UE "
                 "when reselecting towards a higher priority RAT/ frequency "
                 "than the current serving frequency"),

	qmaOptional("ThreshXLow", "-", qmaStandard(INTEGER, "0:31"),
                "Possible values : 0:31"),

	qmaOptional("QRxLevMin", "-", qmaStandard(INTEGER, "0:45"),
			     "Cell selection RX level threshold (in dB) used by the UE "
                 "when reselecting towards a lower priority RAT/ frequency "
                 "than the current serving frequency"),

    /* SPR 14651 START/END Code Removed */

	qmaOptional("X_VENDOR_P_MAX_GERAN", "-", qmaStandard(INTEGER, "0:39"),
                "Maximum allowed transmission power for GERAN on an "
                "\n\t\t\t uplink carrier frequency"),

	qmaOptional("CellReselectionPriority", "-", qmaStandard(INTEGER, "0:7"),
                "Possible values : 0:7"),

	qmaOptional("X_VENDOR_ARFCN_SPACING", "-", qmaStandard(INTEGER, "1:8"),
                "Possible values : 1:8"),

	qmaOptional("X_VENDOR_NUM_OF_FOLLOWING_ARFCNS", "-",
                qmaStandard(INTEGER, "0:31"),
                "Possible values : 0:31"),

	qmaOptional("X_VENDOR_EXPLICIT_ARFCN_LIST", "-",
                qmaStandard(STRING, "MAXLEN 255"),
                "Comma separated list of arfcns, maximum arfcns should not "
                "\n\t\t\t be more than 31"),

	qmaOptional("X_VENDOR_ARFCN_BITMAP", "-",
                qmaStandard(STRING, "MAXLEN 255"),
               "Comma separated list of bitmap octets, maximum octets should"
               "\n\t\t\t not be more than 16"),

	qmaEndParams()
};

static const struct parameter_t __anr_upd_meas_config_utran[] = {
	qmaKeyword("InstanceID"),
	qmaMandatory("InstanceID", qmaStandard(INTEGER, "1:12"),
			     "Accepted Input: 1:32"),

	qmaKeyword("UTRACarrierARFCN"),
	qmaMandatory("UTRACarrierARFCN", qmaStandard(INTEGER, "0:16383"),
			"UTRACarrierARFCN: Possible values: 0:16383"),

    /* OAM BCOM Code Comment Changes Start */
    qmaOptional("Enable","-",qmaStandard(INTEGER, "0:1"), "Enable: 0-1"),
    /* OAM BCOM Code Comment Changes End */ 

	qmaOptional("CellReselectionPriority", "-", qmaStandard(INTEGER, "0:7"),
			     "Possible values: 0:7"),

	qmaOptional("ThreshXHigh", "-", qmaStandard(INTEGER, "0:31"),
			     "Possible values(dB): 0:31"),

	qmaOptional("ThreshXLow", "-", qmaStandard(INTEGER, "0:31"),
			     "Possible values(dB): 0:31"),

	qmaOptional("QRxLevMin", "-", qmaStandard(INTEGER, "-60:-13"),
			     "Possible values(dBm): -60:-13"),

	qmaOptional("PMaxUTRA", "-", qmaStandard(INTEGER, "-50:33"),
			     "Possible values : -50:33"),

	qmaOptional("QQualMin", "-", qmaStandard(INTEGER, "-24:0"),
			     "Possible values(dB): -24:0"),

    /* SPR 14651 START/END Code Removed */

	qmaOptional("X_VENDOR_THRESHX_HIGHQ_R9", "-",
			    qmaStandard(INTEGER, "0:31"), "Possible values: 0:31"),

	qmaOptional("X_VENDOR_THRESHX_LOWQ_R9", "-",
			    qmaStandard(INTEGER, "0:31"), "Possible values: 0:31"),

	qmaEndParams()
};


static const struct parameter_t __anr_add_meas_config_utran[] = {
/* OAM BCOM Code Comment Changes Start */
/* OAM BCOM Code Comment Changes End */
    /* SPR 14651 START */
/* SPR_19303 Fix Start */
   	qmaKeyword("Enable"),
    qmaMandatory("Enable",qmaStandard(INTEGER, "0:1"), "Enable: 0-1"),
/* SPR_19303 Fix End */
    /* SPR 14651 END */
	qmaKeyword("UTRACarrierARFCN"),
	qmaMandatory("UTRACarrierARFCN", qmaStandard(INTEGER, "0:16383"),
			"UTRACarrierARFCN: Possible values: 0:16383"),

	qmaKeyword("CellReselectionPriority"),
	qmaMandatory("CellReselectionPriority", qmaStandard(INTEGER, "0:7"),
			     "Possible values: 0:7"),

	qmaKeyword("ThreshXHigh"),
	qmaMandatory("ThreshXHigh", qmaStandard(INTEGER, "0:31"),
			     "Possible values(dB): 0:31"),

	qmaKeyword("ThreshXLow"),
	qmaMandatory("ThreshXLow", qmaStandard(INTEGER, "0:31"),
			     "Possible values(dB): 0:31"),

	qmaKeyword("QRxLevMin"),
	qmaMandatory("QRxLevMin", qmaStandard(INTEGER, "-60:-13"),
			     "Possible values(dBm): -60:-13"),

	qmaKeyword("PMaxUTRA"),
	qmaMandatory("PMaxUTRA", qmaStandard(INTEGER, "-50:33"),
			     "Possible values : -50:33"),

	qmaKeyword("QQualMin"),
	qmaMandatory("QQualMin", qmaStandard(INTEGER, "-24:0"),
			     "Possible values(dB): -24:0"),

	qmaKeyword("X_VENDOR_THRESHX_HIGHQ_R9"),
	qmaMandatory("X_VENDOR_THRESHX_HIGHQ_R9",
			    qmaStandard(INTEGER, "0:31"), "Possible values: 0:31"),

	qmaKeyword("X_VENDOR_THRESHX_LOWQ_R9"),
	qmaMandatory("X_VENDOR_THRESHX_LOWQ_R9",
			    qmaStandard(INTEGER, "0:31"), "Possible values: 0:31"),

	qmaOptional("X_VENDOR_OFFSET_FREQUENCY", "-",
			    qmaStandard(INTEGER, "-15:15"),
                "Possible values(dB): -15:15" ),

	qmaEndParams()
};

/* OAM BCOM Code Comment Changes Start */
static const struct parameter_t meas_config_utran_tdd_params[] = {
    qmaKeyword("NeighborIndex"),
    qmaMandatory("NeighborIndex", qmaStandard(INTEGER, "1:12"),
            "Instance Number: Mandatory: 1:12"),

    qmaKeyword("UTRACarrierARFCN"),
    qmaMandatory("UTRACarrierARFCN", qmaStandard(INTEGER, "0:16383"),
            "UTRACarrierARFCN: Possible values: 0:16383"),

    qmaOptional("X_VENDOR_OFFSET_FREQUENCY", "-",
            qmaStandard(INTEGER, "-15:15"), "Possible values: -15:15" ),

    qmaOptional("CellReselectionPriority", "-", qmaStandard(INTEGER, "0:7"),
            "Possible values: 0:7"
            "\n\t\t\t Mandatory, if the UTRACarrierARFCN is not already "
            "configured on RRM"),

    qmaOptional("ThreshXHigh", "-", qmaStandard(INTEGER, "0:31"),
            "Possible values : 0:31"
            "\n\t\t\t Mandatory, if the UTRACarrierARFCN is not already "
            "configured on RRM"),

    qmaOptional("ThreshXLow", "-", qmaStandard(INTEGER, "0:31"),
            "Possible values : 0:31"
            "\n\t\t\t Mandatory, if the UTRACarrierARFCN is not already "
            "configured on RRM"),

    qmaOptional("QRxLevMin", "-", qmaStandard(INTEGER, "-60:-13"),
            "Possible values : -60:-13"
            "\n\t\t\t Mandatory, if the UTRACarrierARFCN is not already "
            "configured on RRM"),

    qmaOptional("PMaxUTRA", "-", qmaStandard(INTEGER, "-50:33"),
            "Possible values : -50:33"
            "\n\t\t\t Mandatory, if the UTRACarrierARFCN is not already "
            "configured on RRM"),

    qmaEndParams()
};
/* OAM BCOM Code Comment Changes End */

/* Coverity 64258 Start */ 
  /* Unused Code Removed */
/* Coverity 64258 End */

static const struct command_t __anr_meas_config_add[] = {
    qmaCommand("EUTRAN", __anr_add_meas_config_eutran,
                         qcli_handle_anr_add_meas_config_cmd,
                         "EUTRAN meas config parmeters"),

    qmaCommand("UTRANFDDFreq", __anr_add_meas_config_utran,
               qcli_handle_anr_add_meas_config_cmd,
               "UTRANFDDFreq parameters"),

	qmaCommand("GERANFreqGroup", __anr_add_meas_config_geran_freq_group,
			   qcli_handle_anr_add_meas_config_cmd,
               "GERANFreqGroup parameters"),

	qmaEndCommands()
};


static const struct parameter_t __anr_delete_meas_config_params[] = {
	qmaKeyword("InstanceID"),
	qmaMandatory("InstanceID", qmaStandard(INTEGER, "1:12"),
			     "Accepted Input: 1:32"),

	qmaEndParams()
};

static const struct command_t __anr_meas_config_delete[] = {
    qmaCommand("EUTRAN", __anr_delete_meas_config_params,
                         qcli_send_multi_instance_obj_command,
                         "EUTRAN meas config parmeters"),

    qmaCommand("UTRANFDDFreq", __anr_delete_meas_config_params,
               qcli_send_multi_instance_obj_command,
               "UTRANFDDFreq parameters"),

    qmaCommand("GERANFreqGroup", __anr_delete_meas_config_params,
               qcli_send_multi_instance_obj_command,
               "GERANFreqGroup parameters"),

	qmaEndCommands()
};

static const struct command_t __anr_meas_config_update[] = {
    qmaCommand("EUTRAN", __anr_upd_meas_config_eutran,
                         qcli_send_multi_instance_obj_command,
                         "EUTRAN meas config parmeters"),

    qmaCommand("UTRANFDDFreq", __anr_upd_meas_config_utran,
               qcli_send_multi_instance_obj_command,
               "UTRANFDDFreq parameters"),

    /* OAM BCOM Code Comment Changes Start */
    qmaCommand( "UTRANTDDFreq", meas_config_utran_tdd_params,
            qcli_send_multi_instance_obj_command,
            "UTRANTDDFreq parameters"),
    /* OAM BCOM Code Comment Changes End */

	qmaCommand("UTRA", meas_config_utran_reselection_params,
			   qcli_validate_send_optional_param_cmd,
               "UTRAN Reselection parameters"),

	qmaCommand("GERANFreqGroup", __anr_upd_meas_config_geran_freq_group,
			   qcli_send_multi_instance_obj_command,
               "GERANFreqGroup parameters"),

	qmaCommand("GERAN", meas_config_geran_reselection_params,
			   qcli_validate_send_optional_param_cmd,
               "GERAN Reselection parameters"),

	qmaEndCommands()
};

static const struct command_t anr_measurement_mode[] = {
	qmaMode("ADD", "SON ANR MEAS-CONFIG ADD ", __anr_meas_config_add,
			"Add new measurement configuration to SON"),

	qmaMode("UPDATE", "SON ANR MEAS-CONFIG UPDATE ", __anr_meas_config_update,
            /* OAM BCOM Code Comment Changes Start */
			"Modify Measurement Configuration at eNB"),
            /* OAM BCOM Code Comment Changes End */

	qmaMode("DELETE", "SON ANR MEAS-CONFIG DELETE " , __anr_meas_config_delete,
            /* OAM BCOM Code Comment Changes Start */
			"Delete Measurement Configuration from eNB"),
            /* OAM BCOM Code Comment Changes End */

	qmaEndCommands()
};
/* SPR FIX 11266 - */
/* SPR 11467 END */
static const struct command_t son_anr_mode[] = {
    /*eICIC_PHASE_1_2_CHANGES_START */
	qmaMode("NEIGHBOR-DEFAULT-CONFIG", "NEIGHBOR-DEFAULT-CONFIG",
            __anr_neighbor_default_config_req,
            "SET-GET SON ANR NEIGHBOR DEFAULT CONFIG"),
    /*eICIC_PHASE_1_2_CHANGES_END */
    /* SPR FIX 11266 + */
	qmaMode("MEAS-CONFIG", "SON ANR MEAS-CONFIG ", anr_measurement_mode,
			"Measurement configuration parameters"),
   /* SPR FIX 11266 - */
	qmaCommand("ENABLE", __anr_enable_req,
			qcli_validate_send_optional_param_cmd,
			"Enable ANR Feature"),

	qmaCommandNoParam("DISABLE", oam_qcli_send_no_param_get_set_commands,
			"Disable ANR Feature"),

	qmaMode("MODIFY","SON ANR MODIFY ", __anr_modify,
			"Modify ANR parameters."),

	qmaEndCommands()
};


static const struct parameter_t __nr_add_update_nbr_enb[] = {
	qmaKeyword("ENB_TYPE"),
	qmaMandatory("ENB_TYPE", qmaStandard(INTEGER, "1:2"),
			"Possible values: 1(Home Enb)/2(Macro Enb)"),
	qmaKeyword("PLMNID"),
	qmaMandatory("PLMNID", qmaStandard(STRING, "MAXLEN 6"),
			"Possible values: 5/6 digit value"),

	qmaKeyword("ENBID"),
	qmaMandatory("EN_ID", qmaStandard(STRING, 0),
			"Possible values:  0:268435455"),

	qmaOptional("TAC", "-", qmaStandard(INTEGER, "1:65535"),
			"Possible values: 1:65535 - "
			"Mandatory for ADD if ENB_TNL_ADDRESS is not given"),

	qmaOptional("ENB_TNL_ADDRESS[0]", "-", qmaStandard(STRING, 0),
			"Possible values: IPV6/IPV4 address "
			"- Mandatory for ADD if TAC is not given"),

	qmaOptional("ENB_TNL_ADDRESS[1]", "-", qmaStandard(STRING, 0),
			"Possible values: IPV6/IPV4 address "
			"- Mandatory for ADD if TAC is not given"),

    /* SPR_11780_FIX Start */
	qmaOptional("X2_HO_STATUS", "-", qmaStandard(INTEGER, "1:2"),
			"Possible values: 1(X2_Allowed)/2(X2_Prohibited)"
			" - Mandatory for ADD"),
    /* SPR_11780_FIX End */

	qmaOptional("X2_CONNECTION_STATUS", "-", qmaStandard(INTEGER, "7:10"),
			"Possible values: 7:10 - Mandatory for ADD"),

	qmaEndParams()
};

static const struct parameter_t __nr_delete_enb[] = {
	qmaMandatory( "enb_type", qmaStandard(INTEGER, "1:2"), "enb_type : Possible values : 1(Home Enb)/2(Macro Enb) - Mandatory for DELETE"),
	qmaMandatory( "plmn_id", qmaStandard(STRING, 0), "plmn_id : Possible values : 5/6 digit value - Mandatory for DELETE"),
	qmaMandatory( "enb_id", qmaStandard(STRING, 0), "enb_id : Possible values : 0:268435455 - Mandatory for DELETE"),
	qmaEndParams()
};

static const struct parameter_t __nr_update_eutran_nbr_cell[] = {
	qmaKeyword("NeighborIndex"), 
	qmaMandatory("NeighborIndex", qmaStandard(INTEGER, "1:32"),
			"Neighbor Instance Number: Mandatory: 1-32"),
	qmaKeyword("PLMNID"),
	qmaMandatory("PLMNID", qmaStandard(STRING, "MAXLEN 6"),
			"PLMNID: Mandatory: 6-digit number"),
	qmaKeyword("CID"),
  /* SPR 15266 Fix Start */
	qmaMandatory("CID", qmaStandard(INTEGER, "0:268435455"),
			"CID: Mandatory: 0:268435455"),
    /* SPR 15266 Fix End */
   
/* Spr 15438 Fix Start */
    qmaOptional("Enable","-",qmaStandard(INTEGER, "0:1"), "Enable: 0-1"),
    qmaOptional("MustInclude","-",qmaStandard(INTEGER, "0:1"), "MustInclude: 0-1"),
/* Spr 15438 Fix End */
	qmaOptional("PhyCellID", "-", qmaStandard(INTEGER, "0:503"),
			"0:503"),
	qmaOptional("QOffset", "-",  qmaStandard(CHOICE, "-24|-22|-20|-18|-16|-14|-12|-10|-8|-6|-5|-4|-3|-2|-1|0|1|2|3|4|5|6|8|10|12|14|16|18|20|22|24"),
			"Accepted Input: -24, -22, -20, -18,"
			"\n\t\t\t -16, -14, -12, -10, -8, -6, "
			"\n\t\t\t -5, -4, -3, -2, -1, 0, 1, 2, 3, 4,"
			"\n\t\t\t 5, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24"),

	qmaOptional("EUTRACarrierARFCN", "-",
			qmaStandard(INTEGER, "0:65535"), "0:65535"),

	qmaOptional("Blacklisted", "-", qmaStandard(INTEGER, "0:1"),
			"0(False)/1(True)"),

    /* OAM BCOM Code Comment Changes Start */
    qmaOptional("X_VENDOR_NR_STATUS", "-", qmaStandard(INTEGER, "1:2"),
                 "1(NR Locked)/2(NR Unlocked)"),
    /* OAM BCOM Code Comment Changes End */

	qmaOptional("X_VENDOR_TAC", "-", qmaStandard(INTEGER, "1:65535") ,
			"1:65535"),

	qmaOptional("X_VENDOR_CSGID", "-",
			qmaStandard(INTEGER, "0:134217727"), "0-134217727"),

        /* SPR-13828 Start */
	qmaOptional("X_VENDOR_RSRP", "-", qmaStandard(INTEGER, "0:97"),
			"0:97"),
        /* SPR-13828 End */

	qmaOptional("X_VENDOR_RSRQ", "-", qmaStandard(INTEGER, "0:34"),
			"0:34"),
	qmaOptional("CIO", "-", qmaStandard(INTEGER, "0:3"), "0:3"),
	qmaOptional("X_VENDOR_BROADCAST_STATUS", "-",
			qmaStandard(INTEGER, "0:3"), "0(False)/1(True)"),
	qmaOptional("X_VENDOR_ACCESS_MODE", "-",
			qmaStandard(INTEGER, "0:3"),
        /* SPR 17408 Fix Start */
			"0(Closed)/1(Hybrid)/2(Open-Macro)/3(Open-Femto)"),
        /* SPR 17408 Fix End */
	qmaOptional("X_VENDOR_PRACH_ROOT_SEQUENCE_INDEX", "-", 
			qmaStandard(INTEGER, "0:837"), "Set Logical root sequence index 0:837 "),
	qmaOptional("X_VENDOR_PRACH_ZERO_CORRELATION_ZONE_CONFIG", "-", 
			qmaStandard(INTEGER, "0:15"), "parameter used for preamble sequence generation :0:15"),
	qmaOptional("X_VENDOR_PRACH_HIGH_SPEED_FLAG", "-", 
			qmaStandard(INTEGER, "0:1"), "Unrestricted set 0:FALSE or restricted set 1:TRUE of preambles : 0:1 "),
	qmaOptional("X_VENDOR_PRACH_FREQUENCY_OFFSET", "-", 
			qmaStandard(INTEGER, "0:94"), "First physical resource block available: 0:94"),
	qmaOptional("X_VENDOR_PRACH_CONFIGURATION_INDEX", "-", 
			qmaStandard(INTEGER, "0:63"), "Set index into the table defining PRACH resources within the frame: 0:63"),
    /*eICIC_PHASE_1_2_CHANGES_START */
    qmaOptional("X_VENDOR_INTERFERENCE_SCHEME", "-", qmaStandard(INTEGER, "0:2"), "Type of interference scheme to be used : 0:2"),
    qmaOptional("X_VENDOR_MIN_ABS_USAGE_THRESHOLD", "-", qmaStandard(INTEGER, "0:100"), " : MIN_ABS_USAGE_THRESHOLD 0:100"),
    qmaOptional("X_VENDOR_MAX_ABS_USAGE_THRESHOLD", "-", qmaStandard(INTEGER, "0:100"), " : MAX_ABS_USAGE_THRESHOLD 0:100"),
    /*eICIC_PHASE_1_2_CHANGES_END */

	qmaEndParams()
};

static const struct parameter_t __nr_add_eutran_nbr_cell[] = {
/* Spr 15438 Fix Start */
    qmaKeyword("Enable"),
    qmaMandatory("Enable",qmaStandard(INTEGER, "0:1"), "Enable: 0-1"),
    qmaKeyword("MustInclude"),
    qmaMandatory("MustInclude",qmaStandard(INTEGER, "0:1"), "MustInclude: 0-1"),
/* Spr 15438 Fix End */
	qmaKeyword("PLMNID"),
	qmaMandatory("PLMNID", qmaStandard(STRING, "MAXLEN 6"),
			"PLMNID: Mandatory: 6-digit number"),
	qmaKeyword("CID"),
/* SPR 15266 Fix Start */
	qmaMandatory("CID", qmaStandard(INTEGER, "0:268435455"), "CID: Mandatory: 0-268435455"),
    /* SPR 15266 Fix End */

    /* SPR 11156 FIX START */
    qmaKeyword("PhyCellID"),
	qmaMandatory("PhyCellID",qmaStandard(INTEGER, "0:503"), "PhyCellID: Mandatory for ADD: 0-503"),
    qmaKeyword("EUTRACarrierARFCN"),
	qmaMandatory("EUTRACarrierARFCN", qmaStandard(INTEGER, "0:65535"), "EUTRACarrierARFCN: Mandatory for ADD: 0-65535"),
    qmaKeyword("Blacklisted"),
	qmaMandatory("Blacklisted", qmaStandard(INTEGER, "0:1"),"Blacklisted: Mandatory for ADD: 0(False)/1(True)"),
    qmaKeyword("X_VENDOR_ACCESS_MODE"),
    /* SPR 17408 Fix Start */
    qmaMandatory("X_VENDOR_ACCESS_MODE", qmaStandard(INTEGER, "0:3"),
                                         "0(Closed)/1(Hybrid)/2(Open Macro)/3(Open Femto)"),
	/* SPR 17408 Fix End */
    /* SPR 11156 FIX END */

    qmaKeyword("X_VENDOR_NR_STATUS"),
    qmaMandatory("X_VENDOR_NR_STATUS", qmaStandard(INTEGER, "1:2"),
                                         "1(NR locked) / 2(NR unlocked)"),

	qmaOptional("QOffset", "-",  qmaStandard(CHOICE, "-24|-22|-20|-18|-16|-14|-12|-10|-8|-6|-5|-4|-3|-2|-1|0|1|2|3|4|5|6|8|10|12|14|16|18|20|22|24"),
			"Accepted Input(in dBm): -24, -22, -20, -18,"
			"\n\t\t\t -16, -14, -12, -10, -8, -6, "
			"\n\t\t\t -5, -4, -3, -2, -1, 0, 1, 2, 3, 4,"
			"\n\t\t\t 5, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24"),

	qmaOptional("X_VENDOR_TAC", "-",qmaStandard(INTEGER, "1:65535") ,"1:65535"),
	qmaOptional("X_VENDOR_CSGID", "-", qmaStandard(INTEGER, "0:134217727"),"0:134217727"),
        /* SPR-13828 Start */
	qmaOptional("X_VENDOR_RSRP", "-", qmaStandard(INTEGER, "0:97"), "0:97"),
        /* SPR-13828 End */
	qmaOptional("X_VENDOR_RSRQ", "-", qmaStandard(INTEGER, "0:34"), "0:34"),
        /* BUG_FIXED_11753 */
        qmaOptional("CIO", "-",  qmaStandard(CHOICE, "-24|-22|-20|-18|-16|-14|-12|-10|-8|-6|-5|-4|-3|-2|-1|0|1|2|3|4|5|6|8|10|12|14|16|18|20|22|24"),
            "Accepted Input(in dBm): -24, -22, -20, -18,"
            "\n\t\t\t -16, -14, -12, -10, -8, -6, "
            "\n\t\t\t -5, -4, -3, -2, -1, 0, 1, 2, 3, 4,"
            "\n\t\t\t 5, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24"),
        /* BUG_FIXED_11753 */

	qmaOptional("X_VENDOR_BROADCAST_STATUS", "-", qmaStandard(INTEGER, "0:1"), "0(False)/1(True)"),
    /* SPR 11156 FIX START */
    /* CODE MOVED */
    /* SPR 11156 FIX END */
	qmaOptional("X_VENDOR_PRACH_ROOT_SEQUENCE_INDEX", "-", qmaStandard(INTEGER, "0:837"), "Set Logical root sequence index : 0:837"),
	qmaOptional("X_VENDOR_PRACH_ZERO_CORRELATION_ZONE_CONFIG", "-", qmaStandard(INTEGER, "0:15"), "parameter used for preamble sequence generation :0:15"),
	qmaOptional("X_VENDOR_PRACH_HIGH_SPEED_FLAG", "-", qmaStandard(INTEGER, "0:1"), "Unrestricted set 0:FALSE or restricted set 1:TRUE of preambles : 0/1"),
	qmaOptional("X_VENDOR_PRACH_FREQUENCY_OFFSET", "-", qmaStandard(INTEGER, "0:94"), "First physical resource block available : 0:94"),
	qmaOptional("X_VENDOR_PRACH_CONFIGURATION_INDEX", "-", qmaStandard(INTEGER, "0:63"), "Set index into the table defining PRACH resources within the frame : 0:63"),
    /* SPR 15238 START */
    /* SPR 17408 Fix +- */
    /* SPR 15238 END */

    /*eICIC_PHASE_1_2_CHANGES_START */
	qmaOptional("X_VENDOR_INTERFERENCE_SCHEME", "-", qmaStandard(INTEGER, "0:2"), "Type of interference scheme to be used : 0:2"),
    qmaOptional("X_VENDOR_MIN_ABS_USAGE_THRESHOLD", "-", qmaStandard(INTEGER, "0:100"), " : MIN_ABS_USAGE_THRESHOLD 0:100"),
    qmaOptional("X_VENDOR_MAX_ABS_USAGE_THRESHOLD", "-", qmaStandard(INTEGER, "0:100"), " : MAX_ABS_USAGE_THRESHOLD 0:100"),
    /*eICIC_PHASE_1_2_CHANGES_END */
	qmaEndParams()
};

/* RT SPR 8627 FIX START */
static const struct parameter_t __nr_add_utran_nbr_cell[] = {
/* Spr 15612  Fix Start*/ 
    qmaKeyword("Enable"),
    qmaMandatory("Enable",qmaStandard(INTEGER, "0:1"), "Enable: 0-1"),
    qmaKeyword("MustInclude"),
    qmaMandatory("MustInclude",qmaStandard(INTEGER, "0:1"), "MustInclude: 0-1"),
/* Spr 15612  Fix End*/ 
	qmaKeyword("PLMNID"),
	qmaMandatory("PLMNID", qmaStandard(STRING, "MAXLEN 6"), "6-digit number"),

	qmaKeyword("CID"),
	qmaMandatory("CID", qmaStandard(INTEGER, "1:65535"),
			"Range: 1:65535; In case the input CID is greater "
			"\n\t\t than 4095(more than 12 bits in size), the "
			"\n\t\t user must ensure that the RNCID value must "
			"\n\t\t be in the range 0:4095"),

	qmaKeyword("RNCID"),
	qmaMandatory("RNCID", qmaStandard(INTEGER, "1:65535"),
			"Range: 1:65535; In case the input CID is greater "
			"\n\t\t than 4095(more than 12 bits in size), the "
			"\n\t\t user must ensure that the RNCID value must "
			"\n\t\t be in the range 0:4095"),

	qmaKeyword("UARFCNDL"),
	qmaMandatory("UARFCNDL", qmaStandard(INTEGER, "0:16383"), "0:16383"),

	qmaKeyword("X_VENDOR_NR_STATUS"),
	qmaMandatory("X_VENDOR_NR_STATUS", qmaStandard(INTEGER, "1:2"),
			"1(NR Locked)/2(NR Unlocked)"),

        /* SPR-11757 Start */
	qmaKeyword("X_VENDOR_HO_STATUS"),
	qmaMandatory("X_VENDOR_HO_STATUS", qmaStandard(INTEGER, "1:2"),
			"1(HO Allowed)/2(HO Prohibited)"),
        /* SPR-11757 End */

	qmaKeyword("PCPICHScramblingCode"),
	qmaMandatory("PCPICHScramblingCode", qmaStandard(INTEGER, "0:511"),
			"0:511"),

	qmaOptional("LAC", "-", qmaStandard(INTEGER, "0:65535"), "0:65535"),

	qmaOptional("RAC", "-", qmaStandard(INTEGER, "0:255"), "0:255"),

	qmaOptional("X_VENDOR_CELL_SPECIFIC_OFFSET", "-",
			qmaStandard(INTEGER, "-15:15"), "-15:15"),

	qmaOptional("X_VENDOR_CSGID", "-", qmaStandard(INTEGER, "0:134217727"),
			"0:134217727"),

	qmaOptional("X_VENDOR_RSCP", "-", qmaStandard(INTEGER, "-5:91") , "-5:91"),

	qmaOptional("X_VENDOR_ECNO", "-", qmaStandard(INTEGER, "0:49"), "0:49"),

	/* SPR 17408 Fix Start */
	qmaOptional("X_VENDOR_ACCESS_MODE", "-", qmaStandard(INTEGER, "0:3"),
			"0(Closed)/1(Hybrid)/2(Open Macro)/3(Open Femto)"),
	/* SPR 17408 Fix End */

	qmaEndParams()
};


static const struct parameter_t __nr_update_utran_nbr_cell[] = {
	qmaKeyword("NeighborIndex"),
	qmaMandatory("NeighborIndex", qmaStandard(INTEGER, "1:32"),
			"Neighbor Instance Number: 1-32"),

	qmaKeyword("PLMNID"),
	qmaMandatory( "PLMNID", qmaStandard(STRING, "MAXLEN 6"), "6-digit number"),

	qmaKeyword("CID"),
    /* SPR 15266 Fix Start */
    qmaMandatory("CID", qmaStandard(INTEGER, "0:65535"),
            "Range: 0:65535; In case the input CID is greater "
            /* SPR 15266 Fix End */        

			"\n\t\t than 4095(more than 12 bits in size), the "
			"\n\t\t user must ensure that the RNCID value must "
			"\n\t\t be in the range 0:4095"),

	qmaKeyword("RNCID"),
	qmaMandatory("RNCID", qmaStandard(INTEGER, "1:65535"),
			"Range: 1:65535; In case the input RNCID is greater"
			"\n\t\t than 4095(more than 12 bits in size), the "
			"\n\t\t user must ensure that the CID value must be"
			"\n\t\t  in the range 0:4095"),

    /* OAM BCOM Code Comment Changes Start */
    qmaOptional("Enable","-",qmaStandard(INTEGER, "0:1"), "Enable: 0-1"),
    qmaOptional("MustInclude","-",qmaStandard(INTEGER, "0:1"), "MustInclude: 0-1"),
    /* OAM BCOM Code Comment Changes End */

	qmaOptional("LAC", "-",qmaStandard(INTEGER, "0:65535"), "0:65535"),

	qmaOptional("RAC", "-", qmaStandard(INTEGER, "0:255"), "0:255"),

	qmaOptional("PCPICHScramblingCode", "-", qmaStandard(INTEGER, "0:511"),
			"0:511"),

	qmaOptional("UARFCNDL", "-", qmaStandard(INTEGER, "0:16383"), "0:16383"),

	qmaOptional("X_VENDOR_CELL_SPECIFIC_OFFSET", "-",
			qmaStandard(INTEGER, "-15:15"), "-15:15"),

	qmaOptional("X_VENDOR_CSGID", "-", qmaStandard(INTEGER, "0:134217727"),
			"0:134217727"),

	qmaOptional("X_VENDOR_RSCP", "-", qmaStandard(INTEGER, "-5:91"), "-5:91"),

	qmaOptional( "X_VENDOR_ECNO", "-", qmaStandard(INTEGER, "0:49"), "0:49"),

	qmaOptional("X_VENDOR_NR_STATUS", "-", qmaStandard(INTEGER, "1:2"),
			"1(NR Locked)/2(NR Unlocked)"),

        /* SPR-11757 Start */
	qmaOptional("X_VENDOR_HO_STATUS", "-", qmaStandard(INTEGER, "1:2"),
			"1(HO Allowed)/2(HO Prohibited)"),
        /* SPR-11757 End */

	qmaOptional("X_VENDOR_ACCESS_MODE", "-", qmaStandard(INTEGER, "0:3"),
            /* SPR 17408 Fix Start */
			"0(Closed)/1(Hybrid)/2(Open Macro)/3(Open Femto)"),
	/* SPR 17408 Fix End */

    /* OAM BCOM Code Comment Changes Start */
    qmaOptional("X_VENDOR_PS_HO_SUPPORTED", "-", qmaStandard(INTEGER, "0:1"), "0 for PS HandOver not Supported ;1 for PS HandOver Supported"),
    qmaOptional("X_VENDOR_VOIP_CAPABLE", "-", qmaStandard(INTEGER, "0:1"), "0 for neighbor not VOIP Capable; 1 for neighbor VOIP Capable"),
    /* OAM BCOM Code Comment Changes End */

	qmaEndParams()
};

/* SPR-13586 START */
static const struct parameter_t __nr_update_geran_nbr_cell[] = {
    qmaKeyword("NeighborIndex"),
    qmaMandatory("NeighborIndex", qmaStandard(INTEGER, "1:32"),
                            "Neighbor Instance Number: 1-32"),

    qmaKeyword("PLMNID"),
    qmaMandatory("PLMNID", qmaStandard(STRING, "MAXLEN 6"), "6-digit number"),

    qmaKeyword("LAC"),
    qmaMandatory("LAC", qmaStandard(INTEGER, "1:65535"),
                        "Location Area Code: Range: 1 to 65535, except the"
                        "\n\t\t reserved value 65534(0xFFFE)"),

    qmaKeyword("CI"),
    qmaMandatory("CI", qmaStandard(INTEGER, "0:65535"),
                          "Cell Identity: Range: 0:65535"),

    qmaOptional("BandIndicator", "-", qmaStandard(CHOICE, "DCS1800|PCS1900"),
			     "Possible values: DCS1800 or PCS1900"),

    qmaOptional("BCCHARFCN", "-", qmaStandard(INTEGER, "0:1023"),
                                        "1(NR Locked)/2(NR Unlocked)"),

    qmaOptional("BSIC", "-", qmaStandard(INTEGER, "0:63"),
                        "BSIC(3-bit NCC + 3-bit BSCC) of the neighbor cell"),

    qmaOptional("X_VENDOR_NR_STATUS", "-", qmaStandard(CHOICE, "1|2"),
                                       "1(NR Locked)/2(NR Unlocked)"),

    qmaOptional("X_VENDOR_HO_STATUS", "-", qmaStandard(CHOICE, "1|2"),
                                       "1(HO Allowed)/2(HO Prohibited)"),
    qmaOptional("X_VENDOR_RAC", "-", qmaStandard(INTEGER, "0:255"),
                            "Routing Area Code"),

    qmaOptional("X_VENDOR_CELL_SPECIFIC_OFFSET", "-",
                qmaStandard(INTEGER, "-15:15"), "-15:15"),

    qmaOptional("X_VENDOR_RSSI", "-", qmaStandard(INTEGER, "0:63") ,
                                      "GERAN carrier RSSI"),

    qmaEndParams()
};
/* SPR-13586 END */


static const struct parameter_t __nr_delete_nbr_cell_params[] = {
    /* SPR 11156 FIX START */
    qmaKeyword("NeighborIndex"),
    /* SPR 11156 FIX END */
	qmaMandatory("NeighborIndex", qmaStandard(INTEGER, "1:32"),
			"Neighbor Instance Number: Mandatory: 1:32"),
	qmaEndParams()
};


/* SPR-13586 START */
static const struct parameter_t __nr_add_geran_nbr_cell[] = {
    qmaKeyword("PLMNID"),
    qmaMandatory("PLMNID", qmaStandard(STRING, "MAXLEN 6"), "6-digit number"),

    qmaKeyword("LAC"),
    qmaMandatory("LAC", qmaStandard(INTEGER, "1:65535"),
                        "Location Area Code: Range: 1 to 65535, except the"
                        "\n\t\t reserved value 65534(0xFFFE)"),

    qmaKeyword("CI"),
    qmaMandatory("CI", qmaStandard(INTEGER, "0:65535"),
                          "Cell Identity: Range: 0:65535"),

    qmaKeyword("BandIndicator"),
	qmaMandatory("BandIndicator", qmaStandard(CHOICE, "DCS1800|PCS1900"),
			     "Possible values: DCS1800 or PCS1900"),
    qmaKeyword("BCCHARFCN"),
    qmaMandatory("BCCHARFCN", qmaStandard(INTEGER, "0:1023"),
                                  "BCCHARFCN of the neighbor cell"),

    qmaKeyword("BSIC"),
    qmaMandatory("BSIC", qmaStandard(INTEGER, "0:63"),
                                              "BSIC(3-bit NCC + 3-bit BSCC) of the neighbor cell"),


    qmaKeyword("X_VENDOR_NR_STATUS"),
    qmaMandatory("X_VENDOR_NR_STATUS", qmaStandard(CHOICE, "1|2"),
                                       "1(NR Locked)/2(NR Unlocked)"),

    qmaKeyword("X_VENDOR_HO_STATUS"),
    qmaMandatory("X_VENDOR_HO_STATUS", qmaStandard(CHOICE, "1|2"),
                                       "1(HO Allowed)/2(HO Prohibited)"),
    qmaOptional("X_VENDOR_RAC", "-", qmaStandard(INTEGER, "0:255"),
                            "Routing Area Code"),

    qmaOptional("X_VENDOR_CELL_SPECIFIC_OFFSET", "-",
                qmaStandard(INTEGER, "-15:15"), "-15:15"),

    qmaOptional("X_VENDOR_RSSI", "-", qmaStandard(INTEGER, "0:63") ,
                                      "GERAN carrier RSSI"),

    qmaEndParams()
};
/* SPR-13586 END */


static const struct command_t __nr_add_nbr_cell[] = {
    qmaCommand("EUTRAN", __nr_add_eutran_nbr_cell,
                         oam_qcli_send_son_nr_add_cell_command,
                         "EUTRAN neighbor cell parmeters"),

    qmaCommand("UTRAN", __nr_add_utran_nbr_cell,
                        oam_qcli_send_son_nr_add_cell_command,
                        "UTRAN neighbor cell parmeters"),
/* SPR-13586 START */
    qmaCommand("GERAN", __nr_add_geran_nbr_cell,
                        oam_qcli_send_son_nr_add_cell_command,
                        "GERAN neighbor cell parmeters"),
/* SPR-13586 END */
	qmaEndCommands()
};

static const struct command_t __nr_add[] = {
    qmaMode("NEIGHBOR_CELL", "SON NR ADD NEIGHBOR_CELL ",
            __nr_add_nbr_cell, "Add neighbor cells"),

    qmaCommand("NEIGHBOR_ENB", __nr_add_update_nbr_enb,
                oam_qcli_send_nr_enb_cmd, "Neighbor eNB Parameters"),

    qmaEndCommands()
};


static const struct command_t __nr_update_nbr_cell[] = {
    qmaCommand("EUTRAN", __nr_update_eutran_nbr_cell,
                         qcli_oam_son_nbr_cell_update_cmd_handler,
                         "EUTRAN neighbor cell parmeters"),

    qmaCommand("UTRAN", __nr_update_utran_nbr_cell,
                        qcli_oam_son_nbr_cell_update_cmd_handler,
                        "UTRAN neighbor cell parmeters"),
/* SPR-13586 START */
    qmaCommand("GERAN", __nr_update_geran_nbr_cell,
                        qcli_oam_son_nbr_cell_update_cmd_handler,
                        "GERAN neighbor cell parmeters"),
/* SPR-13586 END */

	qmaEndCommands()
};

static const struct command_t __nr_update[] = {
    qmaMode("NEIGHBOR_CELL", "SON NR UPDATE NEIGHBOR_CELL ",
            __nr_update_nbr_cell, "Update neighbor cells"),

	qmaCommand("NEIGHBOR_ENB", __nr_add_update_nbr_enb,
               oam_qcli_send_nr_enb_cmd, "Neighbor eNB Parameters"),

	qmaEndCommands()
};
/* RT SPR 8627 FIX END */

static const struct command_t __nr_delete_nbr_cell[] = {
	qmaCommand("EUTRAN", __nr_delete_nbr_cell_params,
              qcli_oam_son_nbr_cell_update_cmd_handler, "EUTRAN Parameters"),

	qmaCommand("UTRAN", __nr_delete_nbr_cell_params,
                qcli_oam_son_nbr_cell_update_cmd_handler, "UTRAN Parmeters"),

/* SPR-13586 START */
	qmaCommand("GERAN", __nr_delete_nbr_cell_params,
                qcli_oam_son_nbr_cell_update_cmd_handler, "GERAN Parmeters"),
/* SPR-13586 END */

	qmaEndCommands()
};

static const struct command_t __nr_delete[] = {
	qmaMode("NEIGHBOR_CELL", "SON NR DELETE NEIGHBOR_CELL ",
            __nr_delete_nbr_cell, "Neighbor cell parameters"),

	qmaCommand("NEIGHBOR_ENB", __nr_delete_enb,
               oam_qcli_send_nr_enb_cmd, "Neighbor eNB Parameters"),

	qmaEndCommands()
};


static const struct parameter_t __nr_retrieve_cmd[] = {
	qmaOptional( "", "1",qmaStandard(INTEGER, "0"), ""),
	qmaEndParams()
};
static const struct command_t __nr_retrieve_nbr_cell[] = {
	qmaCommand("EUTRAN", __nr_retrieve_cmd,
               oam_qcli_send_command, "EUTRAN Parameters"),

	qmaCommand("UTRAN", __nr_retrieve_cmd,
            oam_qcli_send_command, "UTRAN Parmeters"),

/* SPR-13586 START */
	qmaCommand("GERAN", __nr_retrieve_cmd,
            oam_qcli_send_command, "GERAN Parmeters"),
/* SPR-13586 END */

	qmaEndCommands()
};

static const struct command_t __nr_retrieve[] = {
    qmaMode("NEIGHBOR_CELL","SON NR RETRIEVE NEIGHBOR_CELL ",
            __nr_retrieve_nbr_cell, "Neighbor cell parameters"),
    qmaCommand("NEIGHBOR_ENB", __nr_retrieve_cmd,
               oam_qcli_send_command, "Neighbor eNB Parameters"),

	qmaEndCommands()
};
/* Start: SPR 10567 */
static const struct parameter_t __nliu_retrieve_cmd[] = {
    qmaOptional( "", "1",qmaStandard(INTEGER, "0"), ""),
    qmaEndParams()
};
static const struct command_t __nliu_retrieve_eutran[] = {
    qmaCommand( "NEIGHBOR_CELL", __nliu_retrieve_cmd, oam_qcli_send_command, "Neighbor Cell Parameters"),
    qmaEndCommands()
};
static const struct command_t __nliu_retrieve_utran[] = {
    qmaCommand( "NEIGHBOR_CELL", __nliu_retrieve_cmd, oam_qcli_send_command, "Neighbor Cell Parameters"),
    qmaEndCommands()
};
/* SPR-13586 START */
static const struct command_t __nliu_retrieve_geran[] = {
    qmaCommand( "NEIGHBOR_CELL", __nliu_retrieve_cmd, oam_qcli_send_command, "Neighbor Cell Parameters"),
    qmaEndCommands()
};
/* SPR-13586 END */
static const struct command_t __nliu_retrieve[] = {
    qmaMode("EUTRAN","EUTRAN", __nliu_retrieve_eutran, "EUTRAN Parameters"),
    qmaMode("UTRAN","UTRAN", __nliu_retrieve_utran, "UTRAN Parameters"),
/* SPR-13586 START */
    qmaMode("GERAN","GERAN", __nliu_retrieve_geran, "GERAN Parameters"),
/* SPR-13586 END */

    qmaEndCommands()
};
/*End: SPR 10567 */


/* RT SPR 8627 FIX START */
static const struct parameter_t __nr_del_from_rem_list_eutran[] = {
	/* RT SPR 8627 FIX END */
	qmaKeyword("PLMNID"),
	qmaMandatory("PLMNID", qmaStandard(STRING, "MAXLEN 6"), "6-digit number"),

	qmaKeyword("CID"),
	qmaMandatory("CID", qmaStandard(INTEGER, "1:268435455"), "1:268435455"),

	qmaEndParams()
};


/* RT SPR 8627 FIX START */
static const struct parameter_t __nr_del_from_rem_list_utran[] = {
	qmaKeyword("PLMNID"),
	qmaMandatory("PLMNID", qmaStandard(STRING, "MAXLEN 6"), "6-digit number"),

	qmaKeyword("RNCID"),
	qmaMandatory("RNCID", qmaStandard(INTEGER, "1:65535"),
			"Range: 1:65535; In case the input CID is greater "
			"\n\t\t than 4095(more than 12 bits in size), the "
			"\n\t\t user must ensure that the RNCID value must "
			"\n\t\t be in the range 0:4095"),

	qmaKeyword("CID"),
	qmaMandatory("CID", qmaStandard(INTEGER, "1:65535"),
			"Range: 1:65535; In case the input CID is greater "
			"\n\t\t than 4095(more than 12 bits in size), the "
			"\n\t\t user must ensure that the RNCID value must "
			"\n\t\t be in the range 0:4095"),
	qmaEndParams()
};

static const struct command_t __nr_del_inter_rat_from_rem_list[] =
{
	qmaCommand("UTRAN", __nr_del_from_rem_list_utran, oam_qcli_send_command, "Delete UTRAN  neighbor from remove list"),
	qmaEndCommands()
};
static const struct command_t __del_from_rem_list_req[] = 
{
	qmaCommand("INTRA_RAT",__nr_del_from_rem_list_eutran,oam_qcli_send_command, "Delete intra rat neighbor from remove list"),
	qmaMode("INTER_RAT","SON NR DELETE_FROM_REMOVE_LIST INTER_RAT ",
			__nr_del_inter_rat_from_rem_list, "Delete inter rat neighbor from remove list"),
	qmaEndCommands()
};
/* RT SPR 8627 FIX END */

/* RT SPR 8746 FIX START */
static const struct command_t __x2_mode[] =
{
	qmaCommandNoParam("ENABLE", oam_qcli_send_no_param_get_set_commands,
			"Enable X2 interface"),

	qmaCommandNoParam("DISABLE", oam_qcli_send_no_param_get_set_commands,
			"Disable X2 interface"),

	qmaEndCommands()
};
/* RT SPR 8746 FIX END */

static const struct parameter_t __nw_scan_req[] =
{
	qmaMandatory("measurement_period", qmaStandard(INTEGER, "1:255"), "Measurement Period: Possible Values: 1 to 255"),
	qmaMandatory("retry_count", qmaStandard(INTEGER, "1:8"), "Retry Count: Possible Values: 1 to 8"),
	qmaMandatory("REMBandList", qmaStandard(INTEGER, "1:64"), "Frequency Band ID: Possible Values: 1 to 64"),
	qmaOptional("EUTRACarrierARFCNDLList", "-", qmaStandard(STRING, "MAXLEN 700"), "Comma seperated "
			"list of EARFCN values, Max 100 Values, Range: 0 to 65535"),
	qmaOptional("pci_list","-", qmaStandard(STRING, "MAXLEN 250"), "List of PCI values(Comma separated) corresponding to each EARFCN separated by colon,"
			" \n\t\t\t For eg - pci_list for 3 EARFCNs shall be given as  - 2,3:4,5,6:2,3"
			" \n\t\t\t Max 50 Values, Range 0 to 503"),
	qmaOptional("meas_bandwidth", "-",qmaStandard(INTEGER, "6:100"), "Possible values : 6 to 100"),
	qmaOptional("REMPLMNList", "-",  qmaStandard(STRING, "MAXLEN 48"), "Comma seperated list of PLMNID values,"
			" Max 6 Values"),
    /* OAM BCOM Code Comment Changes Start */
    qmaOptional("CarrierARFCNDL", "-", qmaStandard(INTEGER, "0:65535"), "Value of lower limit of EARFCN"
            " to be scanned: Possible Values: 0 to 65535)"),
    qmaOptional("CarrierChWidth", "-", qmaStandard(INTEGER, "1:100"), "Number of EARFCN values beyond the CarrierARFCNDL"
            " to be scanned: Possible Values: 1 to 100, CarrierChWidth should be less than (65535 - CarrierARFCNDL),"
            " note*** CarrierARFCNDL is mandatory with CarrierChWidth, if EUTRACarrierARFCNDLList is also specified "
            " CarrierChWidth & CarrierARFCNDL are ignored"),
    /* OAM BCOM Code Comment Changes End */
	qmaEndParams()
};

static const struct command_t __nw_scan[] =
{
	qmaCommand("EUTRAN", __nw_scan_req, oam_qcli_send_command, "Sent NW Scan Request"),
	qmaEndCommands()
};



static const struct command_t son_nr_mode[] = {
	qmaMode("ADD", "SON NR ADD ", __nr_add,
			"Add Neighbor to SON layer"),

	qmaMode("UPDATE", "SON NR UPDATE ", __nr_update,
			"Modify Neighbor details at SON layer"),

	qmaMode("DELETE", "SON NR DELETE " , __nr_delete,
			"Delete Neighbor from SON layer"),
	qmaMode("RETRIEVE", "SON NR RETRIEVE ", __nr_retrieve,
			"Retrieve neighbor info from SON layer"),
	/* RT SPR 8746 FIX START */
	/* LINES DELETED */
	/* RT SPR 8746 FIX END */
	qmaMode("DELETE_FROM_REMOVE_LIST","Delete NR from Remove List",
			__del_from_rem_list_req, "Delete neighbor from remove list"),
	qmaEndCommands()
};

/* Start: SPR 10567 */
static const struct command_t son_nliu_mode[] = {
    qmaMode("RETRIEVE", "SON NLIU RETRIEVE ", __nliu_retrieve,
            "Retrieve Neighbor List In Use info from SON layer"),
    qmaEndCommands()
};
/*End: SPR 10567 */

static const struct parameter_t __nw_periodic_scan_req[] =
{
	qmaMandatory("ScanPeriodically", qmaStandard(INTEGER, "0:1"), "1(PERIODIC_SCAN_ON)/0(PERIODIC_SCAN_OFF)"),
	qmaMandatory("PeriodicInterval", qmaStandard(INTEGER, "0:4294967295"), "Interval between two "
			"periodic scans: Possible Values: 0 to 4294967295"),
	qmaMandatory("measurement_period", qmaStandard(INTEGER, "1:255"), "Measurement Period: Possible Values: 1 to 255"),
	qmaMandatory("retry_count", qmaStandard(INTEGER, "1:8"), "Retry Count: Possible Values: 1 to 8"),
	qmaMandatory("REMBandList", qmaStandard(INTEGER, "1:64"), "Frequency Band ID: Possible Values: 1 to 64"),
         /* Max length of earfcn_list string = (5(max length of single earfcn)+1(for comma)+1(for space))*100(max values) */
	qmaOptional("EUTRACarrierARFCNDLList", "-", qmaStandard(STRING, "MAXLEN 700"), "Comma seperated list of EARFCN values,"
			" Max 100 Values, Range: 0 to 65535)"),
	/* Max length of pci_list string = (3(max length of single pci)+1(for comma)+1(for space))*50(max values) */
	qmaOptional("pci_list","-", qmaStandard(STRING, "MAXLEN 250"), "List of PCI values(Comma separated) corresponding to each EARFCN separated by colon,"
                        " \n\t\t\t For eg - pci_list for 3 EARFCNs shall be given as  - 2,3:4,5,6:2,3 "
                        " \n\t\t\t Max 50 Values, Range 0 to 503"),
	qmaOptional("meas_bandwidth", "-",qmaStandard(INTEGER, "6:100"), "Possible values : 6 to 100"),
	 qmaOptional("REMPLMNList", "-",  qmaStandard(STRING, "MAXLEN 48"), "Comma seperated list of PLMNID values,"
                        " Max 6 Values"),
	qmaEndParams()
};

static const struct parameter_t __nw_periodic_scan_stop_req[] =
{
	qmaEndParams()
};


static const struct command_t __nw_periodic_scan[] =
{
	qmaCommand("EUTRAN", __nw_periodic_scan_req, oam_qcli_send_command, "Send Periodic Scan Request"),
	qmaCommand("STOP",__nw_periodic_scan_stop_req, oam_send_periodic_scan_stop, "Sent Periodic Scan Stop Request"),
	qmaEndCommands()
};

static const struct parameter_t __son_nw_scan_results_rf_bcch[] =
{
	qmaMandatory("Index", qmaStandard(INTEGER, "1:50"), "Index of the Cell for which records"
			" are to be fetched (Currently Supported Range 1 to 50). "),
	qmaEndParams()
};

static const struct command_t __son_nw_scan_results[] =
{
	qmaCommand("RF", __son_nw_scan_results_rf_bcch,oam_son_get_last_nw_scan_results_rf, "Get Last Scan Results RF"),
	qmaCommand("BCCH", __son_nw_scan_results_rf_bcch,oam_son_get_last_nw_scan_results_bcch, "Get Last Scan Results BCCH"),
	qmaEndCommands()
};

static const struct parameter_t __son_nw_scan_config[] =
{
	qmaMandatory("FORCEFUL_SCAN_FLAG", qmaStandard(INTEGER, "0:1"),
                 "Flag to specify if any network scan command is forceful"),
	qmaOptional("SCAN_DATA_TIMEOUT", "-", qmaStandard(INTEGER, "0:4294967295"),
                "Timeout value in seconds for scan result data after "
                "which it'll be marked stale"),
	qmaEndParams()
};

static const struct command_t __son_nw[] =
{
	qmaMode("SCAN", "SON NW SCAN ", __nw_scan, "Send NW Scan Request"),
	qmaMode("PERIODIC_SCAN", "SON NW PERIODIC_SCAN", __nw_periodic_scan, "Configure Periodic Scan"),
	qmaCommandNoParam("SCAN_STATUS",oam_son_get_last_nw_scan_status, "Get Last Scan Status"),
	qmaMode("SCAN_RESULTS", "SON NW SCAN_RESULTS", __son_nw_scan_results, "Get Last Scan Results"),
	qmaCommand("SCAN_CONFIG", __son_nw_scan_config, oam_qcli_send_command,
               "Configure behaviour of network scan"),
	qmaEndCommands()
};

/*SPR 14554 start*/
static const struct parameter_t __cell_info_eicic_params[] =
{
    qmaOptional("min_abs_usage_threshold", "-", qmaStandard(INTEGER, "0:100"),
        "Below this threshold SON will start increasing cell range expansion"),
    qmaOptional("max_abs_usage_threshold", "-", qmaStandard(INTEGER, "0:100"),
        "Above this threshold SON will stop increasing Cell range expansion"),
	qmaEndParams()
};
/*SPR 14554 end*/
static const struct command_t son_cmds[] = {
	qmaMode("ANR", "SON ANR ", son_anr_mode,
            "Configure ANR feature parameters"),
	/* SPR_19619 start*/
	qmaCommand( "PEER_CELL_ACTIVATION", __cell_activation, oam_qcli_send_command, "Perform Peer Cell Activation" ),
	/* SPR_19619 stop*/

	qmaMode( "ES", "SON ES ", son_energy_saving, "To Set ES [ENABLE / DISABLE/MODIFY]" ),
	qmaMode( "NR", "SON NR ", son_nr_mode, "Get/Set neighbor cell / eNodeB info" ),
/* Start: SPR 10567 */
    qmaMode( "NLIU", "SON NLIU ", son_nliu_mode, "Get/Set Neighbor List In Use / eNodeB info" ),
/*End: SPR 10567 */
	qmaMode("NW", "SON NW ", __son_nw, "SON network scan commands"),
	qmaMode( "MRO", "SON MRO ", son_mro, "To Set MRO [ENABLE / DISABLE]" ),
	qmaMode( "MLB", "SON MLB ", son_mlb, "To Set MLB [ENABLE / DISABLE]" ),
	qmaMode("RACH-OPT", "SON RACH-OPT ", son_rach_opt, "To Set rach-opt [ENABLE / DISABLE / MODIFY ]"),
	/* RT SPR 8746 FIX START */
	qmaMode("X2", "SON X2 ", __x2_mode, "Enable/Disable X2 interace"),
	/* RT SPR 8746 FIX END */

/*BUG_1171_START */
   /* SPR 11467 START */
   qmaMode( "PCI-CONFLICT", "SON PCI-CONFLICT", son_aps_mode, "To Set ENABLE/DISABLE for PCI Conflict Detection" ),
   /* SPR 11467 END */				
/*BUG_1171_END */  

    /*SPR 14554 start*/
    qmaCommand("CELL-INFO_eICICParams", __cell_info_eicic_params, oam_qcli_send_command,
        "Configure abs usage thresholds for cell range expansion"),
    /*SPR 14554 end*/
        qmaEndCommands()
};




/* SPR-16334 START */
#else

static const struct parameter_t __x2_nr_enb_del[] = {
    qmaKeyword("EnodeB_ID"),
    qmaMandatory("enb_id", qmaStandard(STRING, "MAXLEN 4"), 
                              "Possible values: macro_enb_id/ home_enb_id"),
    qmaKeyword("PLMNID"),
    qmaMandatory("plmn_identity", qmaStandard(STRING, "MAXLEN 6"), 
            "6-digit number"),

    qmaEndParams()
};

static const struct parameter_t __x2_nr_enb_add_mod[] = {
    qmaKeyword("Action"),
    qmaMandatory("Action", qmaStandard(INTEGER, "1:2"),
                              "Possible values: 1(Add Enb)/2(Modify Enb)"),
    qmaKeyword("IPAddr"),
    qmaMandatory("IPAddr", qmaStandard(STRING, 0),
                            "Possible values: IPV4 Address"),
    qmaKeyword("Port"),
    qmaMandatory("Port", qmaStandard(INTEGER, "0:65535"),
                              "Possible values: Port Number"),
    qmaOptional("IPv6Addr", "-", qmaStandard(STRING, 0),
            "Possible values: IPV4 address "),
    qmaEndParams()
};

static const struct command_t x2_nr_mode[] = {
    qmaCommand("NEIGHBOR_ENB", __x2_nr_enb_add_mod, oam_qcli_send_command, "Add/Modify neighbor eNodeB info"),
    qmaEndCommands()
};

static const struct command_t x2_nr_mode_del[] = {
    qmaCommand("NEIGHBOR_ENB", __x2_nr_enb_del, oam_qcli_send_command, "Delete neighbor eNodeB"),
    qmaEndCommands()
};

static const struct command_t __0806DR12[] = {
    qmaMode( "ADD/MODIFY", "X2 NR ADD/MODIFY", x2_nr_mode, "Add/Modify neighbor eNodeB info" ),
    qmaMode( "DELETE", "X2 NR DELETE", x2_nr_mode_del, "Delete neighbor eNodeB" ),
    qmaEndCommands()
};

static const struct command_t __0806DR11[] = {
    qmaMode( "NR", "X2 NR ", __0806DR12, "Set neighbor eNodeB info" ),
    qmaEndCommands()
};
/* SPR-16334 END */

#endif


static const struct parameter_t __add_lgw[] = {
	qmaMandatory("IP Address(ipV4/ipV6)", qmaStandard(STRING, 0), "To Provide ip address for adding LIPA Gateway"),
	qmaEndParams()
};


static const struct command_t __0806KR01[] = {
	qmaCommand("ADD",  __add_lgw, oam_qcli_send_command , "To send add LGW Request to RRC"),
	qmaCommandNoParam("DELETE", oam_qcli_send_command_no_param ,  "To send delete LGW Request to RRC"),
	qmaEndCommands()
};

static const struct parameter_t __0806NP71[] = {
/*SPR 12091 FIX START*/
/* SPR_18872 Fix Start */
	qmaKeyword("dlQosStrategyWeights.pdbWeight"),
        qmaMandatory("dlQosStrategyWeights.pdbWeight",qmaStandard(INTEGER,"0:10"), "Indicates the PDB weight for dlQosStrategy : Default 10, Range 0-10"),

	qmaKeyword("dlQosStrategyWeights.qciWeight"),
	qmaMandatory("dlQosStrategyWeights.qciWeight",qmaStandard(INTEGER,"0:10"), "Indicates the QCI weight for dlQosStrategy : Default 10, Range 0-10" ),

	qmaKeyword("dlQosStrategyWeights.tokenWeight"),
	qmaMandatory("dlQosStrategyWeights.tokenWeight",qmaStandard(INTEGER,"0:10"), "Indicates the Token weight for dlQosStrategy : Default 10, Range 0-10" ),

	qmaKeyword("dlQosStrategyWeights.qLoadWeight"),
	qmaMandatory("dlQosStrategyWeights.qLoadWeight",qmaStandard(INTEGER,"0:10"), "Indicates the QLOAD weight for dlQosStrategy: Default 10, Range 0-10" ),

	qmaKeyword("dlQosStrategyWeights.mcsWeight"),
	qmaMandatory("dlQosStrategyWeights.mcsWeight",qmaStandard(INTEGER,"0:10"), "Indicates the MCS weight for dlQosStrategy: Default 10, Range 0-10" ),

	qmaKeyword("ulQosStrategyWeights.pdbWeight"),
	qmaMandatory("ulQosStrategyWeights.pdbWeight",qmaStandard(INTEGER,"0:10"), "Indicates the PDB weight for ulQosStrategy : Default 10, Range 0-10" ),

	qmaKeyword("dlQosStrategyWeights.qciWeight"),
	qmaMandatory("ulQosStrategyWeights.qciWeight",qmaStandard(INTEGER,"0:10"), "Indicates the QCI weight for ulQosStrategy: Default 10, Range 0-10" ),

	qmaKeyword("ulQosStrategyWeights.tokenWeight"),
	qmaMandatory("ulQosStrategyWeights.tokenWeight",qmaStandard(INTEGER,"0:10"), "Indicates the Token weight for ulQosStrategy: Default 10, Range 0-10" ),

	qmaKeyword("ulQosStrategyWeights.qLoadWeight"),
	qmaMandatory("ulQosStrategyWeights.qLoadWeight",qmaStandard(INTEGER,"0:10"), "Indicates the qLoad weight for ulQosStrategy: Default 10, Range 0-10" ),

	qmaKeyword("ulQosStrategyWeights.mcsWeight"),
	qmaMandatory("ulQosStrategyWeights.mcsWeight",qmaStandard(INTEGER,"0:10"), "Indicates the MCS weight for ulQosStrategy: Default 10, Range 0-10" ),

/* SPR_18872 Fix End */
/*SPR 12091 FIX END*/
	qmaOptional( "gbrBktDepthFactor", "-",qmaStandard(INTEGER,"1:5"), "Factor for calculating the bucket depth for GBR LC's : Default 2, Range 1..5" ),
	qmaOptional( "ambrBktDepthFactor", "-",qmaStandard(INTEGER,"1:5"), "factor for calculating the bucket depth for AMBR : Default 2, Range 1..5" ),
	qmaOptional( "tokenPeriodicity", "-",qmaStandard(INTEGER, "10:100"), "token update periodicity : Default 20ms, Range 10-100ms" ),
	qmaOptional( "qosStrategyOverallocFactor", "-",qmaStandard(INTEGER, "10:100"), "Excess RB's to condsidered for QOS strategy : Default 20%, Range 10-100%" ),
    /* SPR 22539 Fix + */
	qmaOptional("enableAllocAfterAllowedBitrate","-",qmaStandard(INTEGER,"0:2"), "Used to enable or disable retraversal : Default 1 Range 0-2" ),
    /* SPR 22539 Fix - */

	qmaEndParams()
};

static const struct command_t __0806NP70[] = {
	qmaCommand( "MACQosParams",  __0806NP71,oam_qcli_send_command, "MAC Qos Params modification" ),
	qmaEndCommands()
};
const struct command_t commands[] = {
	qmaMode( "CellMgmt", "CellMgmt ", __0806DED8, "To perform cell level operations[Start / Stop / Setup / Delete / Block / Unblock] etc" ),
	qmaMode("Clean", "Clean ", qcli_clean_mode,
			"To clear the alarm history"),
	qmaMode( "eNB",  "eNB", __0806NED8, "eNB system level commands [Package Download/ Package Upgrade/ Log Archival]" ),
	qmaMode( "Config", "Config ", __0806E370, "To configure/modify system parameters" ),
	qmaMode( "Disable", "Disable ", __0806DC97, "To disable features" ),
	qmaMode( "Enable", "Enable ", __0806DB99, "To enable features" ),
	qmaMode( "Modify", "Modify ", __0806NP70, "To modify MAC layer QoS parameters" ),
	qmaMode( "Reset", "Reset ", __0806E968, "To reset stats/KPI" ),
#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
	qmaMode( "RRH", "RRH ", rrh_mode_handler, "RRH Management Commands" ),
#endif
#endif
	qmaMode( "RRM", "RRM ", __0805E370, "To use RRM provided features" ),
	qmaMode( "Set", "Set ", __080740A8, "To set log level / cell trace [start/stop] / radio usage threshold" ),
	qmaMode( "Show", "Show  ", __0806DA98, "To show running system information" ),
#ifdef OAM_SON_ENABLE
	qmaMode( "SON", "SON ", son_cmds, "To use SON provided features"),
/* SPR-16334 START */
#else
    qmaMode( "X2", "X2 ", __0806DR11, "To use X2 provided features"),
/* SPR-16334 END */
#endif
	/*Bug_Fix 574-210*/
	qmaCommandNoParam( "shutdown", oam_shutdown, "To shutdown eNodeB System" ),
	/*Bug_Fix 494*/
	qmaCommandNoParam( "Restart", oam_restart, "To restart eNodeB System" ),
	qmaMode("LGW", "LGW ", __0806KR01, "To add/delete lippa gateway(LGW)"),	
    /*spr 22474 start*/
    /* A generic command to debug the internal information of modules like L2, L3, RRM, SON and OAM*/
	qmaMode("DEBUG", "DEBUG ", oam_debug_cmd, "To display debug information of eNB stack modules"),	
    /*spr 22474 end*/
    /* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
    qmaMode("MBMS", "MBMS ", mbms_mode_handler,
            "To Configure/Show MBMS Parameters."),	
#endif
    /* EMBMS Changes End */
	qmaEndCommands()
};
