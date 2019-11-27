/******************************************************************************
*   FILE NAME      : oam_snmp_interface.c
*
*   DESCRIPTION    : SNMP Agent & OAM Interface handler
*
*        DATE            AUTHOR          REFERENCE       REASON
*    25 July   2013  Vivek Kumer Gupta   ---------       Draft Version
*    02 August 2013  Purshottam Gupta    ---------       Initial
 *   23 August 2013  Vivek Kumer Gupta   ---------       Added trap handling
*    April 2014      Vinod vyas          CSR 57877 Fix
*	 28 May 2014     Avnish				 SPR FIX 10257   Code change
*    17 Feb 2015     Karuna              SPR 15028       Added eICIC params
 *   Copyright (c) 2013, Aricent Inc. All Rights Reserved
******************************************************************************/

/*******************************************************************************
*                        Header Files
*******************************************************************************/
#include <stacklayer.h>
#include <oam_snmp_interface.h>
#include <oam_snmp_main.h>
#include <oam_enb_mib_traps.h>
/* CSR 57877 FIx Start */
#include <oam_mgmt_intf_api.h>
/* CSR 57877 FIx End */
/*******************************************************************************
*                        GLOBEL VARIBLES
*******************************************************************************/

extern void *channel;
extern const QWAIT* waitstruct;
extern int wakeup_flag;
extern trans_cntrl_ptr p_cntrl_ptr;
extern int expected_trans_id;
extern int expected_api_id;
typedef QTIMER oam_timer_t;
extern prov_request_t prov_req;
trans_cntrl_ptr p_cntrl_ptr={0};

/*******************************************************************************
*                        GLOBEL DATABASE
*******************************************************************************/

oam_mgmt_if_param_tab_t snmp_param_table[] = {
    { OID_PARAM_0,    "InternetGatewayDevice.DeviceInfo",                               "Manufacturer" },
    { OID_PARAM_1,    "InternetGatewayDevice.DeviceInfo",                           "ManufacturerOUI" },
    { OID_PARAM_2,    "InternetGatewayDevice.DeviceInfo",                           "ModelName" },
    { OID_PARAM_3,    "InternetGatewayDevice.DeviceInfo",                       "Description" },
    { OID_PARAM_4,    "InternetGatewayDevice.DeviceInfo",                       "ProductClass" },
    { OID_PARAM_5,    "InternetGatewayDevice.DeviceInfo",                       "SerialNumber" },
    { OID_PARAM_6,    "InternetGatewayDevice.DeviceInfo",                       "HardwareVersion" },
    { OID_PARAM_7,    "InternetGatewayDevice.DeviceInfo",                       "SoftwareVersion" },
    { OID_PARAM_8,    "InternetGatewayDevice.DeviceInfo",                       "ModemFirmwareVersion" },
    { OID_PARAM_9,    "InternetGatewayDevice.DeviceInfo",                       "AdditionalSoftwareVersion" },
    { OID_PARAM_10,   "InternetGatewayDevice.DeviceInfo",                       "UpTime" },
    { OID_PARAM_11,   "InternetGatewayDevice.DeviceInfo",                       "FirstUseDate" },
    /* spr_21013_fix_start */
    { OID_PARAM_12,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IntraFreq",   "TReselectionEUTRASFMedium" },
    { OID_PARAM_13,   "InternetGatewayDevice.FAP.PerfMgmt",                             "ConfigNumberOfEntries" },
    { OID_PARAM_14,   "InternetGatewayDevice.FAP.PerfMgmt.Config.[1]",                      "URL" },
    { OID_PARAM_15,   "InternetGatewayDevice.FAP.PerfMgmt.Config.[1]",                  "Username" },
    { OID_PARAM_16,   "InternetGatewayDevice.FAP.PerfMgmt.Config.[1]",                              "Password" },
    { OID_PARAM_17,   "InternetGatewayDevice.FAP.PerfMgmt.Config.[1]",                          "PeriodicUploadInterval" },
    { OID_PARAM_18,   "InternetGatewayDevice.FAP.PerfMgmt.Config.[1]",                              "PeriodicUploadTime" },
    { OID_PARAM_19,   "InternetGatewayDevice.FAP.PerfMgmt.Config.[1]",                              "X_VENDOR_FileWriteTimer" },
    { OID_PARAM_20,   "InternetGatewayDevice.FAP.PerfMgmt.Config.[1]",                          "X_VENDOR_SamplingInterval" },
    { OID_PARAM_21,   "InternetGatewayDevice.Services",                                     "FAPServiceNumberOfEntries" },
    { OID_PARAM_22,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IntraFreq",   "TReselectionEUTRASFHigh" },
    { OID_PARAM_23,   "InternetGatewayDevice.Services.FAPService",                          "DeviceType" },
    { OID_PARAM_24,   "InternetGatewayDevice.Services.FAPService",                      "DNPrefix" },
    { OID_PARAM_25,   "InternetGatewayDevice.Services.FAPService.Capabilities",             "GPSEquipped" },
    { OID_PARAM_26,   "InternetGatewayDevice.Services.FAPService.Capabilities",             "MaxTxPower" },
    { OID_PARAM_27,   "InternetGatewayDevice.Services.FAPService.Capabilities",             "SupportedSystems" },
    { OID_PARAM_28,   "InternetGatewayDevice.Services.FAPService.Capabilities",             "Beacon" },
    { OID_PARAM_29,   "InternetGatewayDevice.Services.FAPService.Capabilities.LTE",         "DuplexMode" },
    { OID_PARAM_30,   "InternetGatewayDevice.Services.FAPService.Capabilities.LTE",         "BandsSupported" },
    { OID_PARAM_31,   "InternetGatewayDevice.Services.FAPService.Capabilities.LTE",         "NNSFSupported" },
    { OID_PARAM_32,   "InternetGatewayDevice.Services.FAPService.Capabilities.LTE",         "UMTSRxSupported" },
    { OID_PARAM_33,   "InternetGatewayDevice.Services.FAPService.Capabilities.LTE",         "UMTSRxBandsSupported" },
    { OID_PARAM_34,   "InternetGatewayDevice.Services.FAPService.Capabilities.LTE",         "GSMRxSupported" },
    { OID_PARAM_35,   "InternetGatewayDevice.Services.FAPService.Capabilities.LTE",         "GSMRxBandsSupported" },
    { OID_PARAM_36,   "InternetGatewayDevice.Services.FAPService.Capabilities.LTE",         "CDMA2000RxSupported" },
    { OID_PARAM_37,   "InternetGatewayDevice.Services.FAPService.Capabilities.LTE",         "CDMA2000RxBandsSupported" },
    { OID_PARAM_38,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IRAT.UTRA",  "TReselectionUTRASFMedium" },
    { OID_PARAM_39,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.InterFreq",  "MaxCarrierEntries" },
    { OID_PARAM_40,   "InternetGatewayDevice.Services.FAPService.FAPControl.LTE",           "OpState" },
    { OID_PARAM_41,   "InternetGatewayDevice.Services.FAPService.FAPControl.LTE",           "AdminState" },
    { OID_PARAM_42,   "InternetGatewayDevice.Services.FAPService.FAPControl.LTE",           "RFTxStatus" },
    { OID_PARAM_43,   "InternetGatewayDevice.Services.FAPService.FAPControl.LTE",           "AvailabilityStatus" },
    /*SPR 10257 FIX start*/
    { OID_PARAM_44,   "InternetGatewayDevice.Services.FAPService.FAPControl.LTE.Gateway",   "SecGWServer3" },
    { OID_PARAM_45,   "InternetGatewayDevice.Services.FAPService.FAPControl.LTE.Gateway",   "SecGWServer2" },
    /*SPR 10257 FIX end*/
    { OID_PARAM_46,   "InternetGatewayDevice.Services.FAPService.FAPControl.LTE.Gateway",   "S1SigLinkServerList" },
    /*SPR 10257 FIX start*/
    { OID_PARAM_47,   "InternetGatewayDevice.Services.FAPService.FAPControl.LTE.Gateway",   "SecGWServer1" },
    /*SPR 10257 FIX end*/
    { OID_PARAM_48,   "InternetGatewayDevice.Services.FAPService.FAPControl.LTE.Gateway",   "S1SigLinkPort" },
    { OID_PARAM_49,   "InternetGatewayDevice.Services.FAPService.AccessMgmt.LTE",               "AccessMode" },
    { OID_PARAM_50,   "InternetGatewayDevice.Services.FAPService.AccessMgmt.LTE",               "MaxUEsServed" },
    { OID_PARAM_51,   "InternetGatewayDevice.Services.FAPService.AccessMgmt.LTE",               "MaxCSGMembers" },
    { OID_PARAM_52,   "InternetGatewayDevice.Services.FAPService.AccessMgmt.LTE",               "MaxNonCSGMembers" },
    { OID_PARAM_53,   "Not found",                                                                  "Not used" },
    { OID_PARAM_54,   "InternetGatewayDevice.Services.FAPService.AccessMgmt.LTE",               "CSGID" },
    { OID_PARAM_55,   "InternetGatewayDevice.Services.FAPService.AccessMgmt.LTE",               "HNBName" },
    { OID_PARAM_56,   "Not used",               "Not used" },
    { OID_PARAM_57,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC",           "AllowedCipheringAlgorithmList" },
    { OID_PARAM_58,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC",           "AllowedIntegrityProtectionAlgorithmList" },
    { OID_PARAM_59,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC",            "TAC" },
    { OID_PARAM_60,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC",            "EAID" },
    { OID_PARAM_61,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC",             "MaxPLMNListEntries" },
    { OID_PARAM_62,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC",            "PLMNListNumberOfEntries" },
    { OID_PARAM_63,   "Not found",                      "Not used" },
    { OID_PARAM_64,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC",                      "QoSNumberOfEntries" },
    { OID_PARAM_65,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Common",               "CellIdentity" },
    { OID_PARAM_66,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.S1AP",                 "TRelocPrep" },
    { OID_PARAM_67,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.S1AP",                 "TRelocOverall" },
    { OID_PARAM_68,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RF",                   "EARFCNDL" },
    { OID_PARAM_69,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RF",                   "EARFCNUL" },
    { OID_PARAM_70,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RF",                   "FreqBandIndicator" },
    { OID_PARAM_71,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RF",                                 "DLBandwidth" },
    { OID_PARAM_72,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RF",                                 "ULBandwidth" },
    { OID_PARAM_73,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RF",                          "ReferenceSignalPower" },
    { OID_PARAM_74,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RF",                                 "PhyCellID" },
    { OID_PARAM_75,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RF",                                 "PSCHPowerOffset" },
    { OID_PARAM_76,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RF",                                 "SSCHPowerOffset" },
    { OID_PARAM_77,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RF",                                 "PBCHPowerOffset" },
    { OID_PARAM_78,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.AntennaInfo",          "AntennaPortsCount" },
    { OID_PARAM_79,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PDSCH",                              "Pb" },
    { OID_PARAM_80,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PDSCH",                              "Pa" },
    { OID_PARAM_81,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.SRS",                                "SRSEnabled" },
    { OID_PARAM_82,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.SRS",                                "SRSBandwidthConfig" },
    { OID_PARAM_83,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.SRS",                                "SRSMaxUpPTS" },
    { OID_PARAM_84,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.SRS",                                "AckNackSRSSimultaneousTransmission" },
    { OID_PARAM_85,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PRACH",                              "RootSequenceIndex" },
    { OID_PARAM_86,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PRACH",                              "ConfigurationIndex" },
    { OID_PARAM_87,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PRACH",                              "HighSpeedFlag" },
    { OID_PARAM_88,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PRACH",                              "ZeroCorrelationZoneConfig" },
    { OID_PARAM_89,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PRACH",                              "FreqOffset" },
    { OID_PARAM_90,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PUCCH",                              "DeltaPUCCHShift" },
    { OID_PARAM_91,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PUCCH",                              "NRBCQI" },
    { OID_PARAM_92,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PUCCH",                              "NCSAN" },
    { OID_PARAM_93,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PUCCH",                              "N1PUCCHAN" },
    /* SPR 22311 Changes - Obselete Parameter Deleted */
    { OID_PARAM_94,   "Not found",                      "Not used" },
    { OID_PARAM_95,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PUSCH",                                        "NSB" },
    { OID_PARAM_96,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PUSCH",                              "HoppingMode" },
    { OID_PARAM_97,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PUSCH",                              "HoppingOffset" },
    { OID_PARAM_98,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PUSCH",                              "Enable64QAM" },
    { OID_PARAM_99,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PUSCH.ULRS",                         "GroupHoppingEnabled" },
    { OID_PARAM_100,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PUSCH.ULRS",                         "GroupAssignmentPUSCH" },
    { OID_PARAM_101,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PUSCH.ULRS",                         "SequenceHoppingEnabled" },
    { OID_PARAM_102,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PUSCH.ULRS",                         "CyclicShift" },
    { OID_PARAM_103,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.ULPowerControl",             "P0NominalPUSCH" },
    { OID_PARAM_104,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.ULPowerControl",                     "Alpha" },
    { OID_PARAM_105,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.ULPowerControl",                     "P0NominalPUCCH" },
    { OID_PARAM_106,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.ULPowerControl",                      "DeltaFPUCCHFormat1" },
    { OID_PARAM_107,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.ULPowerControl",                      "DeltaFPUCCHFormat1b" },
    { OID_PARAM_108,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.ULPowerControl",                      "DeltaFPUCCHFormat2" },
    { OID_PARAM_109,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.ULPowerControl",                      "DeltaFPUCCHFormat2a" },
    { OID_PARAM_110,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.ULPowerControl",                      "DeltaFPUCCHFormat2b" },
    { OID_PARAM_111,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.ULPowerControl",                      "DeltaPreambleMsg3" },
    { OID_PARAM_112,  "not found",                                                    "Not used" },
    { OID_PARAM_113, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PRS",                                  "NumPRSResourceBlocks" },
    { OID_PARAM_114, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PRS",                               "PRSConfigurationIndex" },
    { OID_PARAM_115, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.PRS",                              "NumConsecutivePRSSubframes" },
    { OID_PARAM_116, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.TDDFrame",                         "SubFrameAssignment" },
    { OID_PARAM_117, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.PHY.TDDFrame",                         "SpecialSubframePatterns" },
    { OID_PARAM_118, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.RACH",                             "NumberOfRaPreambles" },
    { OID_PARAM_119, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.RACH",                             "SizeOfRaGroupA" },
    { OID_PARAM_120, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.RACH",                             "MessageSizeGroupA" },
    { OID_PARAM_121, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.RACH",                             "MessagePowerOffsetGroupB" },
    { OID_PARAM_122, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.RACH",                             "PowerRampingStep" },
    { OID_PARAM_123, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.RACH",                             "PreambleInitialReceivedTargetPower" },
    { OID_PARAM_124, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.RACH",                    "PreambleTransMax" },
    { OID_PARAM_125, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.RACH",               "ResponseWindowSize" },
    { OID_PARAM_126, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.RACH",                "ContentionResolutionTimer" },
    { OID_PARAM_127, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.RACH",               "MaxHARQMsg3Tx" },
    { OID_PARAM_128, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.DRX",               "DRXEnabled" },
    { OID_PARAM_129, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.DRX",               "OnDurationTimer" },
    { OID_PARAM_130, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.DRX",               "DRXInactivityTimer" },
    { OID_PARAM_131, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.DRX",               "DRXRetransmissionTimer" },
    { OID_PARAM_132, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.DRX",               "LongDRXCycle" },
    { OID_PARAM_133, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.DRX",               "DRXStartOffset" },
    { OID_PARAM_134, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.DRX",                "ShortDRXCycle" },
    { OID_PARAM_135, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.MAC.DRX",                "DRXShortCycleTimer" },
    { OID_PARAM_136, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RLC.SRB1",               "DefaultConfiguration" },
    { OID_PARAM_137, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RLC.SRB1",               "TPollRetransmit" },
    { OID_PARAM_138, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RLC.SRB1",               "PollPDU" },
    { OID_PARAM_139, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RLC.SRB1",              "PollByte" },
    { OID_PARAM_140, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RLC.SRB1",              "MaxRetxThreshold" },
    { OID_PARAM_141, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RLC.SRB1",               "TReordering" },
    { OID_PARAM_142, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RLC.SRB1",              "TStatusProhibit" }, 
    { OID_PARAM_143, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RLC.SRB2",               "DefaultConfiguration" },
    { OID_PARAM_144, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RLC.SRB2",               "TPollRetransmit" },
    { OID_PARAM_145, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RLC.SRB2",               "PollPDU" },
    { OID_PARAM_146, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RLC.SRB2",              "PollByte" },
    { OID_PARAM_147, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RLC.SRB2",              "MaxRetxThreshold" },
    { OID_PARAM_148, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RLC.SRB2",               "TReordering" },
    { OID_PARAM_149, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RLC.SRB2",              "TStatusProhibit" },
    { OID_PARAM_150, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RRCTimers",             "T300" },
    { OID_PARAM_151, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RRCTimers",             "T301" },
    { OID_PARAM_152, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RRCTimers",             "T302" },
    { OID_PARAM_153, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RRCTimers",             "T304EUTRA" },
    { OID_PARAM_154, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RRCTimers",             "T304IRAT" },
    { OID_PARAM_155, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RRCTimers",             "T310" },
    { OID_PARAM_156, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RRCTimers",             "T311" },
    { OID_PARAM_157, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RRCTimers",             "T320" },
    { OID_PARAM_158, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RRCTimers",             "N310" },
    { OID_PARAM_159, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.RRCTimers",             "N311" },
    { OID_PARAM_160, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.CellRestriction",      "CellBarred" },
    { OID_PARAM_161, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.PLMNList",          "CellReservedForOperatorUse" },
    { OID_PARAM_162, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.CellRestriction",      "BarringForEmergency" },
    { OID_PARAM_163, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.Common", "Qhyst" },
    { OID_PARAM_164,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.Common",  "QHystSFMedium" },
    { OID_PARAM_165,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.Common",   "QHystSFHigh" },
    { OID_PARAM_166,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.Common",    "TEvaluation" },
    { OID_PARAM_167, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.Common",     "THystNormal" },
    { OID_PARAM_168, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.Common",      "NCellChangeMedium" },
    { OID_PARAM_169, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.Common",       "NCellChangeHigh" },
    { OID_PARAM_170, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IntraFreq",    "QRxLevMinSIB1" },
    { OID_PARAM_171, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IntraFreq",   "QRxLevMinSIB3" },
    { OID_PARAM_172, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IntraFreq",  "QRxLevMinOffset" },
    { OID_PARAM_173, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IntraFreq",  "SIntraSearch" },
    { OID_PARAM_174, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IntraFreq",   "TReselectionEUTRA" },
    { OID_PARAM_175, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IntraFreq",   "SNonIntraSearch" },
    { OID_PARAM_176, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IntraFreq",   "CellReselectionPriority" },
    { OID_PARAM_177, "Not used",  "Not used" },
    { OID_PARAM_178, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IntraFreq",         "ThreshServingLow" },
    { OID_PARAM_179, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.InterFreq",         "CarrierNumberOfEntries" },
    { OID_PARAM_180, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IRAT.UTRA",         "TReselectionUTRA" },
    { OID_PARAM_181, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IRAT.UTRA",         "TReselectionUTRASFHigh" },
    { OID_PARAM_182, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IRAT.UTRA",         "UTRANFDDFreqNumberOfEntries" },
    { OID_PARAM_183, "Not used", "Not used" },
    { OID_PARAM_184, "Not used", "Not used" },
    { OID_PARAM_185, "Not used", "Not used" },
    { OID_PARAM_186, "Not used", "Not used" },
    { OID_PARAM_187, "Not used", "Not used" },
    { OID_PARAM_188, "Not used", "Not used" },
    { OID_PARAM_189, "Not used", "Not used" },
    { OID_PARAM_190, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",             "FilterCoefficientRSRP" },
    { OID_PARAM_191, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",              "FilterCoefficientRSRQ" },
    { OID_PARAM_192,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",             "A1ThresholdRSRP" },
    { OID_PARAM_193, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",               "A1ThresholdRSRQ" },
    { OID_PARAM_194,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",              "A2ThresholdRSRP" },
    { OID_PARAM_195,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",                "A2ThresholdRSRQ" },
    { OID_PARAM_196, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",                "A3Offset" },
    { OID_PARAM_197,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",              "ReportOnLeave" },
    { OID_PARAM_198,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",              "A4ThresholdRSRP" },
    { OID_PARAM_199, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",               "A4ThresholdRSRQ" },
    { OID_PARAM_200,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",              "A5Threshold1RSRP" },
    { OID_PARAM_201,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",              "A5Threshold1RSRQ" },
    { OID_PARAM_202,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",              "A5Threshold2RSRP" },
    { OID_PARAM_203,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",             "A5Threshold2RSRQ" },
    { OID_PARAM_204,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",             "Hysteresis" },
    { OID_PARAM_205, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",              "TimeToTrigger" },
    { OID_PARAM_206,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",             "TriggerQuantity" },
    { OID_PARAM_207, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",              "ReportQuantity" },
    { OID_PARAM_208, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",               "MaxReportCells" },
    { OID_PARAM_209, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",              "ReportInterval" },
    { OID_PARAM_210,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA",              "ReportAmount" },
    { OID_PARAM_211,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",               "QoffsettUTRA" },
    { OID_PARAM_212,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",               "FilterCoefficientUTRA" },
    { OID_PARAM_213,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",               "MeasQuantityUTRAFDD" },
    { OID_PARAM_214,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",               "B1ThresholdUTRARSCP" },
    { OID_PARAM_215, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",                 "B1ThresholdUTRAEcN0" },
    { OID_PARAM_216, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",                 "QoffsetGERAN" },
    { OID_PARAM_217,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",               "FilterCoefficientGERAN" },
    { OID_PARAM_218, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",                "B1ThresholdGERAN" },
    { OID_PARAM_219, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",              "QoffsetCDMA2000" },
    { OID_PARAM_220,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",           "MeasQuantityCDMA2000" },
    { OID_PARAM_221,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",           "B1ThresholdCDMA2000" },
    { OID_PARAM_222,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",           "B2Threshold2UTRARSCP" },
    { OID_PARAM_223,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",           "B2Threshold2UTRAEcN0" },
    { OID_PARAM_224, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",            "B2Threshold2GERAN" },
    { OID_PARAM_225,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",          "B2Threshold2CDMA2000" },
    { OID_PARAM_226,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",          "Hysteresis" },
    { OID_PARAM_227,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",          "TimeToTrigger" },
    { OID_PARAM_228,  "Not used",          "Not used" },
    { OID_PARAM_229,   "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",         "ReportInterval" },
    { OID_PARAM_230,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.ConnMode.IRAT",           "ReportAmount" },
    { OID_PARAM_231,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborList",                  "MaxLTECellEntries" },
    { OID_PARAM_232,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborList",                  "LTECellNumberOfEntries" },
    { OID_PARAM_233,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborList.InterRATCell",     "MaxUMTSEntries" },
    { OID_PARAM_234,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborList.InterRATCell",     "UMTSNumberOfEntries" },
    { OID_PARAM_235,  "Not used",    "Not used" },
    { OID_PARAM_236,  "Not used",    "Not used" },
    { OID_PARAM_237,  "Not used",    "Not used" },
    { OID_PARAM_238,  "Not used",    "Not used" },
    { OID_PARAM_239,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborListInUse",              "MaxLTECellEntries" },
    { OID_PARAM_240,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborListInUse",              "LTECellNumberOfEntries" },
    { OID_PARAM_241,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborListInUse.InterRATCell", "MaxUMTSEntries" },
    { OID_PARAM_242,  "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborListInUse.InterRATCell", "UMTSNumberOfEntries" },  
    { OID_PARAM_243,  "InternetGatewayDevice.Services.FAPService.Transport.SCTP",                   "Enable" },
    { OID_PARAM_244,  "InternetGatewayDevice.Services.FAPService.Transport.SCTP",                   "HBInterval" },
    { OID_PARAM_245,  "InternetGatewayDevice.Services.FAPService.Transport.SCTP",                   "MaxAssociationRetransmits" },
    { OID_PARAM_246,  "InternetGatewayDevice.Services.FAPService.Transport.SCTP",                   "MaxPathRetransmits" },
    { OID_PARAM_247,  "InternetGatewayDevice.Services.FAPService.Transport.SCTP",                   "RTOInitial" },
    { OID_PARAM_248,  "InternetGatewayDevice.Services.FAPService.Transport.SCTP",                   "RTOMax" },
    { OID_PARAM_249,  "InternetGatewayDevice.Services.FAPService.Transport.SCTP",                   "RTOMin" },
    { OID_PARAM_250,  "InternetGatewayDevice.Services.FAPService.Transport.SCTP",                   "ValCookieLife" },
    /* spr_21013_fix_end */
    { OID_PARAM_251, "InternetGatewayDevice.FaultMgmt", "SupportedAlarmNumberOfEntries" },
    { OID_PARAM_252, "InternetGatewayDevice.FaultMgmt", "MaxCurrentAlarmEntries" },
    { OID_PARAM_253, "InternetGatewayDevice.FaultMgmt", "CurrentAlarmNumberOfEntries" },
    { OID_PARAM_254, "InternetGatewayDevice.FaultMgmt", "HistoryEventNumberOfEntries" },
    { OID_PARAM_255,  "InternetGatewayDevice.FaultMgmt", "ExpeditedEventNumberOfEntries" },
    { OID_PARAM_256,  "InternetGatewayDevice.FaultMgmt", "QueuedEventNumberOfEntries" },
    /*
    the following are used for control commands 
    OID_PARAM_257, OID_PARAM_258, OID_PARAM_259
    */
    { OID_PARAM_257,  "Not used", "Not used" },
    { OID_PARAM_258,  "Not used", "Not used" },
    { OID_PARAM_259,  "Not used", "Not used" },
    /* spr_21013_fix_start */
    { OID_PARAM_260, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.PLMNList", NULL },
    { OID_PARAM_261, NULL, "Enable" },
    { OID_PARAM_262, NULL, "IsPrimary" },
    { OID_PARAM_263, NULL, "PLMNID" },
    { OID_PARAM_264, NULL, "CellReservedForOperatorUse" },
    { OID_PARAM_265, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS", NULL },
    { OID_PARAM_266, NULL, "QCI" },
    { OID_PARAM_267, NULL, "Type" },
    { OID_PARAM_268, NULL, "Priority" },
    { OID_PARAM_269, NULL, "PacketDelayBudget" },
    { OID_PARAM_270, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.InterFreq.Carrier", NULL },
    { OID_PARAM_271, NULL, "EUTRACarrierARFCN" },
    { OID_PARAM_272, NULL, "QOffsetFreq" },
    { OID_PARAM_273, NULL, "QRxLevMinSIB5" },
    { OID_PARAM_274, NULL, "PMax" },
    { OID_PARAM_275, NULL, "TReselectionEUTRA" },
    { OID_PARAM_276, NULL, "TReselectionEUTRASFMedium" },
    { OID_PARAM_277, NULL, "TReselectionEUTRASFHigh" },
    { OID_PARAM_278, NULL, "ThreshXHigh" },
    { OID_PARAM_279, NULL, "ThreshXLow" },
    { OID_PARAM_280, NULL, "CellReselectionPriority" },
    { OID_PARAM_281, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IRAT.UTRA.UTRANFDDFreq", NULL },
    { OID_PARAM_282, NULL, "UTRACarrierARFCN" },
    { OID_PARAM_283, NULL, "QRxLevMin" },
    { OID_PARAM_284, NULL, "QQualMin" },
    { OID_PARAM_285, NULL, "CellReselectionPriority" },
    { OID_PARAM_286, NULL, "ThreshXHigh" },
    { OID_PARAM_287, NULL, "ThreshXLow" },
    { OID_PARAM_288, NULL, "PMaxUTRA" },
    { OID_PARAM_289, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.Mobility.IdleMode.IRAT.GERAN.GERANFreqGroup", NULL },
    { OID_PARAM_290, NULL, "QRxLevMin" },
    { OID_PARAM_291, NULL, "CellReselectionPriority" },
    { OID_PARAM_292, NULL, "ThreshXHigh" },
    { OID_PARAM_293, NULL, "ThreshXLow" },
    { OID_PARAM_294, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborList.LTECell", NULL },
    { OID_PARAM_295, NULL, "PLMNID" },
    { OID_PARAM_296, NULL, "CID" },
    { OID_PARAM_297, NULL, "EUTRACarrierARFCN" },
    { OID_PARAM_298, NULL, "PhyCellID" },
    { OID_PARAM_299, NULL, "QOffset" },
    { OID_PARAM_300, NULL, "CIO" },
    { OID_PARAM_301, NULL, "Blacklisted" },
    { OID_PARAM_302, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborList.InterRATCell.UMTS", NULL },
    { OID_PARAM_303, NULL, "PLMNID" },
    { OID_PARAM_304, NULL, "RNCID" },
    { OID_PARAM_305, NULL, "CID" },
    { OID_PARAM_306, NULL, "LAC" },
    { OID_PARAM_307, NULL, "RAC" },
    { OID_PARAM_308, NULL, "UARFCNDL" },
    { OID_PARAM_309, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborListInUse.LTECell", NULL },
    { OID_PARAM_310, NULL, "PLMNID" },
    { OID_PARAM_311, NULL, "CID" },
    { OID_PARAM_312, NULL, "EUTRACarrierARFCN" },
    { OID_PARAM_313, NULL, "PhyCellID" },
    { OID_PARAM_314, NULL, "QOffset" },
    { OID_PARAM_315, NULL, "Blacklisted" },
    { OID_PARAM_316, "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborListInUse.InterRATCell.UMTS", NULL },
    /* spr_21013_fix_end */
    { OID_PARAM_317, NULL, "PLMNID" },
    { OID_PARAM_318, NULL, "RNCID" },
    { OID_PARAM_319, NULL, "CID" },
    { OID_PARAM_320, NULL, "LAC" },
    { OID_PARAM_321, NULL, "RAC" },
    { OID_PARAM_322, NULL, "UARFCNDL" },
    { OID_PARAM_323, "InternetGatewayDevice.FaultMgmt.CurrentAlarm", NULL },
    { OID_PARAM_324, NULL, "AlarmIdentifier" },
    { OID_PARAM_325, NULL, "AlarmRaisedTime" },
    { OID_PARAM_326, NULL, "AlarmChangedTime" },
    { OID_PARAM_327, NULL, "ManagedObjectInstance" },
    { OID_PARAM_328, NULL, "EventType" },
    { OID_PARAM_329, NULL, "ProbableCause" },
    { OID_PARAM_330, NULL, "SpecificProblem" },
    { OID_PARAM_331, NULL, "PerceivedSeverity" },
    { OID_PARAM_332, NULL, "AdditionalText" },
    { OID_PARAM_333, NULL, "AdditionalInformation" },
    { OID_PARAM_334, "InternetGatewayDevice.FaultMgmt.SupportedAlarm", NULL },
    { OID_PARAM_335, NULL, "EventType" },
    { OID_PARAM_336, NULL, "ProbableCause" },
    { OID_PARAM_337, NULL, "SpecificProblem" },
    { OID_PARAM_338, NULL, "PerceivedSeverity" },
    { OID_PARAM_339, NULL, "ReportingMechanism" },
    { OID_PARAM_340, "InternetGatewayDevice.FaultMgmt.HistoryEvent", NULL },
    { OID_PARAM_341, NULL, "EventTime" },
    { OID_PARAM_342, NULL, "AlarmIdentifier" },
    { OID_PARAM_343, NULL, "NotificationType" },
    { OID_PARAM_344, NULL, "ManagedObjectInstance" },
    { OID_PARAM_345, NULL, "EventType" },
    { OID_PARAM_346, NULL, "ProbableCause" },
    { OID_PARAM_347, NULL, "SpecificProblem" },
    { OID_PARAM_348, NULL, "PerceivedSeverity" },
    { OID_PARAM_349, NULL, "AdditionalText" },
    { OID_PARAM_350, NULL, "AdditionalInformation" },
    { OID_PARAM_351, "InternetGatewayDevice.FaultMgmt.ExpeditedEvent", NULL },
    { OID_PARAM_352, NULL, "EventTime" },
    { OID_PARAM_353, NULL, "AlarmIdentifier" },
    { OID_PARAM_354, NULL, "NotificationType" },
    { OID_PARAM_355, NULL, "ManagedObjectInstance" },
    { OID_PARAM_356, NULL, "EventType" },
    { OID_PARAM_357, NULL, "ProbableCause" },
    { OID_PARAM_358, NULL, "SpecificProblem" },
    { OID_PARAM_359, NULL, "PerceivedSeverity" },
    { OID_PARAM_360, NULL, "AdditionalText" },
    { OID_PARAM_361, NULL, "AdditionalInformation" },
    { OID_PARAM_362, "InternetGatewayDevice.FaultMgmt.QueuedEvent", NULL },
    { OID_PARAM_363, NULL, "EventTime" },
    { OID_PARAM_364, NULL, "AlarmIdentifier" },
    { OID_PARAM_365, NULL, "NotificationType" },
    { OID_PARAM_366, NULL, "ManagedObjectInstance" },
    { OID_PARAM_367, NULL, "EventType" },
    { OID_PARAM_368, NULL, "ProbableCause" },
    { OID_PARAM_369, NULL, "SpecificProblem" },
    { OID_PARAM_370, NULL, "PerceivedSeverity" },
    { OID_PARAM_371, NULL, "AdditionalText" },
        { OID_PARAM_372, NULL, "AdditionalInformation" },
        { OID_PARAM_373, "Cell_Context.Context.CellIndexList", "Not used" },
        /* SPR 11249 FIX START */
        /* Changed from ActiveCellIndex to CurrentCellIndex */
        { OID_PARAM_374, "Cell_Context.Context", "CurrentCellIndex" },
        /* SPR 11249 FIX END */
    /*SPR 14225 start*/
    /*SPR 15028 start*/
        /* spr_21013_fix_start */
        { OID_PARAM_375, "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.eICICParams" , "eICICProvisionType" },
        { OID_PARAM_376, "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.AbsInfoParams.ABS_INFO", "NumAbsReport" },
        { OID_PARAM_377, "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.AbsInfoParams.ABS_INFO", "AbsUsageLowThreshold" },
        { OID_PARAM_378, "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.AbsInfoParams.ABS_INFO", "AbsUsageHighThreshold" },
        { OID_PARAM_379, "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.AbsInfoParams.ABS_PATTERN", "LowLoadAbsPattern" },
        { OID_PARAM_380, "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.AbsInfoParams.ABS_PATTERN", "VictimAbsPattern" },
        { OID_PARAM_381, "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.AbsInfoParams.ABS_PATTERN", "VictimMeasSubset" },
        { OID_PARAM_382, "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.eICICParams", "eligible_ue" },
        { OID_PARAM_383, "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.eICICTimers", "MacEicicMeasReportPeriodicity" },
        { OID_PARAM_384, "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.eICICTimers", "AggressorSelTimer" },
        { OID_PARAM_385, "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.eICICTimers", "LoadInformationGuardTimer" },
        { OID_PARAM_386, "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.eICICTimers", "LoadInformationCollationTimer" },
        { OID_PARAM_387, "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.eICICParams", "DlSinrThreshLowMark" },
        { OID_PARAM_388, "InternetGatewayDevice.Services.FAPService.CellConfig.X_VENDOR_eICIC_PARAMS.eICICParams", "DlSinrThreshHighMark" },
    /* spr_21013_fix_end */
    /*SPR 15028 end*/
    /*SPR 14225 end*/
    /* EMBMS Changes Start */
    { OID_PARAM_389, "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.MBSFN", "NeighCellConfig" },
    { OID_PARAM_390, "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.MBSFN", "MaxSFConfigListEntries" },
    { OID_PARAM_391, "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.MBSFN", "SFConfigListNumberOfEntries" },
    { OID_PARAM_392, "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.MBSFN.SFConfigList", NULL },
    { OID_PARAM_393, NULL, "SyncStratumID" },
    { OID_PARAM_394, NULL, "RadioFrameAllocationPeriod" },
    { OID_PARAM_395, NULL, "RadioframeAllocationOffset" },
    { OID_PARAM_396, NULL, "RadioframeAllocationSize" },
    { OID_PARAM_397, NULL, "SubFrameAllocations" }
    /* EMBMS Changes End*/
};


UInt32 snmp_param_table_max_size = sizeof(snmp_param_table) /
                                   sizeof(oam_mgmt_if_param_tab_t);

/****************************************************************************
 * Function Name  : oam_mgmt_mem_alloc
 * Inputs         : size                        
 * Outputs        : none
 * Returns        : Pointer to memory buffer or NULL in case of failure
 * Description    : This function returns memory buffer from memory pool.
 ****************************************************************************/
mgmt_return_et                           
oam_mgmt_mem_alloc(size_t size, void **p_msg)
{
    void *p_buf = OAM_NULL;

    p_buf = qvMsgAlloc(OAM_NULL, OAM_ZERO, OAM_ZERO, size);

    if(!p_buf)
    {
        OAM_SNMP_LOG (SNMP, OAM_SNMP_ERROR, "Memory allocation to "
                "p_buf failed");

        return OAM_SNMP_RESOURCEUNAVAILABLE;
    }

    *p_msg = p_buf;
    return OAM_SNMP_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_mgmt_mem_free
 * Inputs         : buffer                    
 * Outputs        : none
 * Returns        : Pointer to memory buffer or NULL in case of failure
 * Description    : This function free memory buffer from memory pool.
 ****************************************************************************/
mgmt_return_et
oam_mgmt_mem_free
(
    void *p_buffer /* Pointer to buffer which will be freed */
)
{
    mgmt_return_et retVal = OAM_SNMP_SUCCESS;
    if (p_buffer != OAM_NULL)
    {
        qvMsgFree(p_buffer);
    }
    else {
        OAM_SNMP_LOG (SNMP, OAM_SNMP_WARNING, "Null pointer passed");
        retVal = OAM_SNMP_GEN_ERROR;
    }

    return retVal;
}

/****************************************************************************
 * Function Name  : oam_mgmt_construct_api_header
 * Inputs         : This function constructs api header from given
 *                     parameters.
 * Outputs        : none
 * Returns        : void
 * Description    : constructs CSPL api header
 ****************************************************************************/
void oam_mgmt_construct_api_header
(
 UInt8          *p_header,      /* CSPL header */
 UInt8          version_id,     /* API version Id */
 UInt16         src_module_id,  /* Source module Id */
 UInt16         dst_module_id,  /* Destination module Id */
 UInt16         api_id,         /* API Id */
 UInt16         api_buf_size    /* API buffer size */
 )
{
    int curr_pos = 0;
        /* version */
        p_header[curr_pos++] = version_id;
        /* from (HI) */
        p_header[curr_pos++] = (src_module_id & 0xFF00) >> 8;
        /* from (LOW) */
        p_header[curr_pos++] = src_module_id & 0x00FF;
        /* to (HI) */
        p_header[curr_pos++] = (dst_module_id & 0xFF00) >> 8;
        /* to (LOW) */
        p_header[curr_pos++] = dst_module_id & 0x00FF;
        /* api id (HI) */
        p_header[curr_pos++] = (api_id & 0xFF00) >> 8;
        /* api id (LOW) */
        p_header[curr_pos++] = api_id & 0x00FF;
        /* api size (HI) */
        p_header[curr_pos++] = (api_buf_size & 0xFF00) >> 8;
        /* api size (LOW) */
        p_header[curr_pos++] = api_buf_size & 0x00FF;
        /* spare bytes */
        p_header[curr_pos++]    = 0x00;
        p_header[curr_pos++]    = 0x00;
        p_header[curr_pos++]    = 0x00;
        p_header[curr_pos++]    = 0x00;
        p_header[curr_pos++]    = 0x00;
        p_header[curr_pos++]    = 0x00;
        p_header[curr_pos++]    = 0x00;
}

/****************************************************************************
 * Function Name  : oam_construct_mgmt_intf_header
 * Inputs         : This function constructs mgmt intf header from given
 *                  parameters.
 * Outputs        : none
 * Returns        : void
 * Description    : constructs CSPL api header
 ****************************************************************************/
void
oam_construct_mgmt_intf_header
(       
unsigned char *p_header,
UInt16 transaction_id,  
UInt16 src_module_id,
UInt16 dst_module_id,
UInt16 api_id,
UInt16 api_buf_size
)
{
    int curr_pos = 0;
    /* transaction identifier (HI) */
    p_header[curr_pos++] = ((transaction_id & 0xFF00) >> 8);
    /* transaction identifier (LOW) */
    p_header[curr_pos++] = (transaction_id & 0x00FF);
    /* from (HI) */
    p_header[curr_pos++] = ((src_module_id & 0xFF00) >> 8);
    /* from (LOW) */
    p_header[curr_pos++] = (src_module_id & 0x00FF);
    /* to (HI) */
    p_header[curr_pos++] = ((dst_module_id & 0xFF00) >> 8);
    /* to (LOW) */
    p_header[curr_pos++] = (dst_module_id & 0x00FF);
    /* api id (HI) */
    p_header[curr_pos++] = ((api_id & 0xFF00) >> 8);
    /* api id (LOW) */
    p_header[curr_pos++] = (api_id & 0x00FF);
    /* api size (HI) */
    p_header[curr_pos++] = ((api_buf_size & 0xFF00) >> 8);
    /* api size (LOW) */
    p_header[curr_pos++] = api_buf_size & 0x00FF;

    p_header[curr_pos++] = 0x00;
    p_header[curr_pos++] = 0x00;
    p_header[curr_pos++] = 0x00;
    p_header[curr_pos++] = 0x00;
    p_header[curr_pos++] = 0x00;
    p_header[curr_pos++] = 0x00;
} 

/****************************************************************************
 * Function Name  : oam_mgmt_cp_pack_UInt16
 * Inputs         : p_dest, p_src
 * Outputs        : none
 * Returns        : OAM_SNMP_SUCCESS
 * Description    : This is used for pack UInt16 buffer
 ****************************************************************************/
mgmt_return_et
oam_mgmt_cp_pack_UInt16
(
    void    *p_dest,
    void    *p_src
)
{
    /* Packing UInt16 */
    ((UInt8*)p_dest)[0]    = (UInt8)((*(UInt16 *)p_src) >> 8);
    ((UInt8*)p_dest)[1]    = (UInt8)((*(UInt16 *)p_src));
    return(OAM_SNMP_SUCCESS);
}

/****************************************************************************
 * Function Name  : oam_mgmt_cp_unpack_UInt16
 * Inputs         : p_dest, p_src
 * Outputs        : none
 * Returns        : OAM_SNMP_SUCCESS
 * Description    : This is used for unpack UInt16 buffer
 ****************************************************************************/
mgmt_return_et
oam_mgmt_cp_unpack_UInt16
(
    void    *p_dest,
    void    *p_src
)
{
    /* Unpacking UInt16 */
    *(UInt16 *)p_dest  = 0;
    *(UInt16 *)p_dest = (UInt16)(*(UInt16 *)p_dest  | (UInt16)(((UInt8*)p_src)[0] << 8));
    *(UInt16 *)p_dest = (UInt16)(*(UInt16 *)p_dest  | (UInt16)((UInt8*)p_src)[1]);
    return(OAM_SNMP_SUCCESS);
}

/****************************************************************************
 * Function Name  : oam_mgmt_send_message
 * Inputs         : p_msg, dst_module
 * Outputs        : none
 * Returns        : void
 * Description    : This is used for sending message to external modules
 ****************************************************************************/
mgmt_return_et 
oam_mgmt_send_message(void          *p_msg,  /* Message that will be passed */
                      unsigned long dst_module)  /* Module id for which message will be passed */
{
    mgmt_return_et retVal = OAM_SNMP_SUCCESS;
    QMODULE service = qvGetService(dst_module);
    int error_code = 0;
    UInt16 src_id;
    UInt16 dst_id;
    UInt16 api_id;
    UInt16 length;

    oam_mgmt_cp_unpack_UInt16(&src_id, (UInt8 *)p_msg + 1); 
    oam_mgmt_cp_unpack_UInt16(&dst_id, (UInt8 *)p_msg + 3); 
    oam_mgmt_cp_unpack_UInt16(&api_id, (UInt8 *)p_msg + 5); 
    oam_mgmt_cp_unpack_UInt16(&length, (UInt8 *)p_msg + 7); 

    OAM_SNMP_LOG(SNMP, OAM_SNMP_INFO, "Sending API[%u] of length[%u]"
        " from Module[%u] to Module[%u]", api_id, length, src_id, dst_id);

    qvSendEx(service, OAM_ZERO, p_msg, &error_code);
    if(error_code != QVERROR_NONE) {
        retVal = OAM_SNMP_INT_ERROR;
    }

    return retVal;
}


/******************************************************************************
 * Function Name: oam_snmp_get_req_handler
 * Description  : This function creates and sends API for GPV req to OAM.
 * Inputs       : p_req             : Pointer to oam_mgmt_if_req_t structure 
 * OUTPUTS      : p_resp_info : response message 
 * Returns      : OAM_SNMP_SUCCESS or some error code
 *******************************************************************************/
mgmt_return_et 
oam_snmp_get_req_handler(oam_mgmt_if_req_t *p_req,
                         void *p_resp_info)
{
    UInt16 param_info_len = 0;
    UInt8 count = 0;
    UInt16 rem_len = OAM_ZERO;

    mgmt_return_et retVal = OAM_SNMP_SUCCESS;
    oam_mgmt_if_param_info_t *param = NULL;

    UInt8 *p_msg = NULL;
    UInt8 *ptr_msg = NULL;
    UInt16 trans_id = oam_snmp_create_new_trans_id();
/* CSR 57877 FIx Start */
    UInt16 api_id = OAM_GET_PARAMETER_VALUES;
/* CSR 57877 FIx End */


    param = p_req->param_list;
    expected_trans_id = trans_id;
/* CSR 57877 FIx Start */
    expected_api_id   = OAM_GET_PARAMETER_VALUES_RESP;
/* CSR 57877 FIx End */

    /* msg length with header size */
    UInt16 msg_size = OAM_MGMT_CSPL_HDR_SIZE + OAM_MGMT_INTF_HEADER_SIZE;

    /* Parent Node Info Length */
    UInt16 parent_info_len = OAM_MGMT_IF_MIN_PARENT_INFO_LEN
                             + oam_snmp_strlen(p_req->parent_name);   

    /* length of leaf level parameters */
    for(count = OAM_ZERO; param && count < p_req->num_params; ++count) {
        param_info_len += OAM_MGMT_IF_MIN_PARAM_INFO_LEN
                          + oam_snmp_strlen(param->param_name);
        param = param->next;
    }

    /* Total msg_size */
    msg_size += parent_info_len + param_info_len;

    retVal = oam_mgmt_mem_alloc(msg_size, (void**)&p_msg);
    if(OAM_SNMP_RESOURCEUNAVAILABLE == retVal) {
        OAM_SNMP_LOG(SNMP, OAM_SNMP_ERROR,
                  "Memory allocation to p_msg failed with error code: %d", retVal);
        return retVal;
    }

    oam_mgmt_memset(p_msg, 0, msg_size);

    ptr_msg = p_msg;
    rem_len = msg_size;

    /* Fill api header to buffer */
    oam_mgmt_construct_api_header(ptr_msg, OAM_VERSION,
                             INTERFACE_MODULE_ID, OAM_MODULE_ID,
                             api_id, rem_len);
    ptr_msg += OAM_MGMT_CSPL_HDR_SIZE;
    rem_len -= OAM_MGMT_CSPL_HDR_SIZE;

    /* Fill oam_mgmt_intf header to buffer */
    oam_construct_mgmt_intf_header(ptr_msg, trans_id,
                                   INTERFACE_MODULE_ID, OAM_MODULE_ID,
                                   api_id, rem_len);
    ptr_msg   += OAM_MGMT_INTF_HEADER_SIZE;
    rem_len   -= OAM_MGMT_INTF_HEADER_SIZE;

    /* Fill payload to buffer */
    oam_snmp_fill_mgmt_if_req_payload(ptr_msg, rem_len, p_req);

    /* send message to OAM */
    retVal = oam_mgmt_send_message(p_msg, OAM_MODULE_ID);

    if(OAM_SNMP_SUCCESS != retVal) {
        OAM_SNMP_LOG(SNMP, OAM_SNMP_ERROR, "Send failed...");
        return retVal;
    }

    /*** WAIT FOR RESPONSE ***/
    QTIME timer_timeout;
    timer_timeout.s = SNMP_OAM_IF_RESP_TIMEOUT_S;
    timer_timeout.us = SNMP_OAM_IF_RESP_TIMEOUT_US;
    OAM_SNMP_LOG (SNMP, OAM_SNMP_DETAILED," waiting for response from OAM ");
    waitstruct->sleep(channel ,&timer_timeout);

    if(SNMP_OAM_IF_MESSAGE_RECEIVED == wakeup_flag) {
        OAM_SNMP_LOG (SNMP, OAM_SNMP_INFO, "Response received");
        wakeup_flag = SNMP_OAM_IF_WAKEUP_TIMEDOUT; /* reset with default */

	    retVal = oam_snmp_decode_mgmt_if_resp_msg(p_cntrl_ptr.msg_ptr,
                                              OAM_MGMT_IF_GET_REQ,
                                              p_resp_info);
    }
    else if(SNMP_OAM_IF_WAKEUP_TIMEDOUT == wakeup_flag) {
        /* coverity 41353 */
        retVal = (mgmt_return_et)OAM_SNMP_TIMEOUT;	
        expected_trans_id = 0;
        expected_api_id = 0;
        OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING,
               "Timeout occured, message not received from OAM.");
    }
    else {
       /* coverity 41352 */
	   retVal = (mgmt_return_et)OAM_SNMP_INT_ERROR;
       OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING,
                    "Error, shouldn't happen !!!");
    }

    return retVal;
}

/******************************************************************************
 * Function Name: oam_snmp_decode_mgmt_if_resp_msg
 * Description  : This function decode response of GPV req from OAM.
 * Inputs       : p_mgmt_if_resp    : Pointer to response buffer 
 *                mode              : mode
 * OUTPUTS      : p_resp_info       : response message 
 * Returns      : OAM_SNMP_SUCCESS or some error code
 *******************************************************************************/
mgmt_return_et 
oam_snmp_decode_mgmt_if_resp_msg(UInt8 *p_mgmt_if_resp,
                                 oam_mgmt_if_mode_et mode,
                                 void *p_resp_info)
{
    mgmt_return_et retVal = OAM_SNMP_SUCCESS;
    SInt32 msg_size = 0;
    UInt16 api_id = 0;
    oam_mgmt_if_get_resp_t *p_get_resp = OAM_NULL;

    api_id = get_word_from_header(p_mgmt_if_resp + SNMP_OAM_IF_API_ID_OFFSET);

/* CSR 57877 FIx Start*/
    if(((OAM_MGMT_IF_SET_REQ == mode) && (api_id != OAM_SET_PARAMETER_VALUES_RESP))
       ||
       ((OAM_MGMT_IF_GET_REQ == mode) && (api_id != OAM_GET_PARAMETER_VALUES_RESP))) 
/* CSR 57877 FIx End */
    {
        OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING, "Unexpected API ID[%u] or "
            "mode[%u] received",
            api_id, mode);
        retVal = OAM_SNMP_UNEXPECTED_MSG;
        return retVal;
    }

    msg_size = get_word_from_header(p_mgmt_if_resp + SNMP_OAM_IF_MSG_LEN_OFFSET);

    p_mgmt_if_resp += OAM_MGMT_INTF_HEADER_SIZE;

    retVal = oam_snmp_decode_mgmt_if_resp_status(p_mgmt_if_resp,
                                             msg_size,
                                             p_resp_info);



    /* Check for Failure Response */
    if(OAM_SNMP_SUCCESS != retVal) {
/*CSR 57877 Fix Start */
		/* coverity 62583 */       
		if(OAM_SNMP_UNEXPECTED_MSG == retVal) {
/*CSR 57877 Fix End*/
            OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING,
                      "Decoding response message failed...");
        }
        else {
            OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING,
                                  "Failure reponse received...");
        }
        return retVal;
    }
    else if(mode != OAM_MGMT_IF_SET_REQ) { // Response success
        /* Get response recieved, decode for the parameter values */
        p_get_resp = (oam_mgmt_if_get_resp_t *)p_resp_info;

        /* Parent Info Length */
        p_mgmt_if_resp += OAM_MGMT_IF_MIN_RESP_PAYLOAD_LEN;
        msg_size -= OAM_MGMT_IF_MIN_RESP_PAYLOAD_LEN;

        retVal = oam_snmp_decode_mgmt_if_get_resp_info(p_mgmt_if_resp, 
                                                       msg_size,
                                                       p_get_resp);

    }

    return retVal;
}

/******************************************************************************
 * Function Name: oam_snmp_decode_mgmt_if_get_resp_info
 * Description  : This function decode detail of GPV successful resp from OAM.
 * Inputs       : p_mgmt_if_resp    : Pointer to response buffer 
 *                len_left          : len_left
 * OUTPUTS      : p_resp            : response message 
 * Returns      : OAM_SNMP_SUCCESS or some error code
 *******************************************************************************/
mgmt_return_et 
oam_snmp_decode_mgmt_if_get_resp_info
(
    UInt8 *p_mgmt_if_resp,
    SInt32 len_left,
    oam_mgmt_if_get_resp_t *p_resp
)
{
    UInt8 count = 0;
    UInt16 parent_name_len = 0;
    UInt16 param_name_len = 0;
    mgmt_return_et retVal = OAM_SNMP_SUCCESS;
    oam_mgmt_if_param_info_t **p_p_param = NULL;


    retVal = oam_snmp_decode_mgmt_if_get_resp_parent_info(p_mgmt_if_resp, 
                                                          len_left,
                                                          &parent_name_len,
                                                          p_resp);

    if(OAM_SNMP_SUCCESS != retVal) {
        OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING, "Decoding failed...");
        return retVal;
    }


    p_mgmt_if_resp += OAM_MGMT_IF_MIN_PARENT_INFO_LEN + parent_name_len;
    len_left -= OAM_MGMT_IF_MIN_PARENT_INFO_LEN + parent_name_len;

    p_p_param = &p_resp->param_list;

    OAM_SNMP_LOG(SNMP, OAM_SNMP_DETAILED,
                 "Decoding parameter info block...");

    for(count = 0; count < p_resp->num_params; ++count) {
        retVal = oam_mgmt_mem_alloc(sizeof(oam_mgmt_if_param_info_t),
                		   (void**)p_p_param);

        if(OAM_SNMP_SUCCESS != retVal) {
            return retVal;
        }

        oam_mgmt_memset(*p_p_param, 0, sizeof(oam_mgmt_if_param_info_t));

        /* OAM_MGMT_IF_MIN_PARAM_INFO_LEN = 4 */
        if(len_left < OAM_MGMT_IF_MIN_PARAM_INFO_LEN) {
            OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING, "Incoming message damaged");
			retVal = OAM_SNMP_UNEXPECTED_MSG;
            return retVal;
        }

        param_name_len = (get_word_from_header(p_mgmt_if_resp)
                          - OAM_MGMT_IF_MIN_PARAM_INFO_LEN);

        p_mgmt_if_resp += OAM_MGMT_IF_MIN_PARAM_INFO_LEN;
        len_left       -= OAM_MGMT_IF_MIN_PARAM_INFO_LEN;

        if(len_left < param_name_len) {
            OAM_SNMP_LOG (SNMP, OAM_SNMP_WARNING, "Incoming message damaged");
			retVal = OAM_SNMP_UNEXPECTED_MSG;
            return retVal;
        }

        /* KLOCWORK warning fix start */
        if(param_name_len <= sizeof((*p_p_param)->param_name)) {
            snmp_memcpy((*p_p_param)->param_name, p_mgmt_if_resp, param_name_len);

            p_mgmt_if_resp += param_name_len;
            len_left -= param_name_len;

            (*p_p_param)->next = NULL;
            /* Move to the next node of Link List */
            p_p_param = &(*p_p_param)->next;
        }
        else {
            OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING, "Message damaged !!!");
            retVal = OAM_SNMP_UNEXPECTED_MSG;
            break;
        }
    }
        /* KLOCWORK warning fix end */

    return retVal;
}

/******************************************************************************
 * Function Name: oam_snmp_decode_mgmt_if_get_resp_parent_info
 * Description  : This function decode parent info detail of GPV successful resp from OAM.
 * Inputs       : p_mgmt_if_resp    : Pointer to response buffer 
 *                len_left          : len_left
 * OUTPUTS      : p_resp            : response message 
 * Returns      : OAM_SNMP_SUCCESS or some error code
 *******************************************************************************/
mgmt_return_et
oam_snmp_decode_mgmt_if_get_resp_parent_info
(
    UInt8 *p_mgmt_if_resp,
    SInt32 len_left,
	UInt16 *parent_len,
    oam_mgmt_if_get_resp_t *p_resp
)
{
    mgmt_return_et retVal = OAM_SNMP_SUCCESS;
    UInt16 parent_name_len = 0;
    OAM_SNMP_LOG(SNMP, OAM_SNMP_DETAILED,
                 "Decoding parent info of response...");


    if(len_left < OAM_MGMT_IF_MIN_PARENT_INFO_LEN) {
        OAM_SNMP_LOG (SNMP, OAM_SNMP_WARNING, "Incoming message damaged");
		retVal = OAM_SNMP_UNEXPECTED_MSG;
        return retVal;
    }

    oam_mgmt_memset(p_resp, 0, sizeof(oam_mgmt_if_get_resp_t));
    
    /* Total length of parent information */
    len_left = get_word_from_header(p_mgmt_if_resp);
    p_mgmt_if_resp += SNMP_OAM_IF_MSG_WORD_LEN;

    /* Number of parameters */
    /* coverity fix 41362 */
    p_resp->num_params = *p_mgmt_if_resp++;
    OAM_SNMP_LOG(SNMP, OAM_SNMP_DETAILED,
                 "Number of parameters received: %d", p_resp->num_params);

    /* Start Index + End Index + Reserved = 3 */
    p_mgmt_if_resp += 3; /* Skip reserved bytes */

    /* Length of Parent Node Name */
    *parent_len = parent_name_len = get_word_from_header(p_mgmt_if_resp);
    p_mgmt_if_resp += SNMP_OAM_IF_MSG_WORD_LEN;

    /* Length of Parent Node Info Header Length */
    len_left -= OAM_MGMT_IF_MIN_PARENT_INFO_LEN;

    if(len_left < parent_name_len) {
        OAM_SNMP_LOG (SNMP, OAM_SNMP_WARNING, "Incoming message damaged");
		retVal = OAM_SNMP_UNEXPECTED_MSG;
        return retVal;
    }

    /*Copy Parent name to p_resp->parent_name */
    /* KLOCWORK warning fix start */
    if(parent_name_len <= sizeof(p_resp->parent_name)) {
        snmp_memcpy(p_resp->parent_name, p_mgmt_if_resp, parent_name_len);
        OAM_SNMP_LOG(SNMP, OAM_SNMP_DETAILED,
                 "Parent name: %s", p_resp->parent_name);
    }
    else {
        OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING, "Message damaged !!!");
    /* KLOCWORK warning fix end */
        retVal = OAM_SNMP_UNEXPECTED_MSG;
    }

    return retVal;
}

/******************************************************************************
 * Function Name: oam_snmp_decode_mgmt_if_resp_status
 * Description  : This function decode response of GPV req from OAM.
 * Inputs       : p_resp_payload    : Pointer to response buffer 
 *                payload_len       : payload length
 * OUTPUTS      : p_resp        : response message 
 * Returns      : OAM_SNMP_SUCCESS or some error code
 *******************************************************************************/
mgmt_return_et
oam_snmp_decode_mgmt_if_resp_status
(
    UInt8 *p_resp_payload,
        SInt32 payload_len,
        oam_mgmt_if_resp_status_t *p_resp
)
{
    mgmt_return_et retVal = OAM_SNMP_SUCCESS;

    if(payload_len < OAM_MGMT_IF_MIN_RESP_PAYLOAD_LEN) {
        OAM_SNMP_LOG (SNMP, OAM_SNMP_WARNING, "Incoming message damaged");
		/* coverity 62593 */
		retVal = OAM_SNMP_UNEXPECTED_MSG;
        return retVal;
    }

    oam_mgmt_memset(p_resp, 0, sizeof(oam_mgmt_if_resp_status_t));

    /* Status Response of GPV*/
    p_resp->status = *p_resp_payload++;

    if(OAM_RESULT_FAILURE == p_resp->status) {
    /* Error Code of GPV Req Fail of UInt16 Type */
        oam_mgmt_cp_unpack_UInt16(&p_resp->err_code, p_resp_payload);
        p_resp_payload += SNMP_OAM_IF_MSG_WORD_LEN;
        OAM_SNMP_LOG(SNMP, OAM_SNMP_INFO,
                     "Failure response received with error code: %d",
                     p_resp->err_code);


        /* Skip 4 reserved byte undecoded */
        p_resp_payload += 4;

    /* Length of Error String */
        p_resp->err_str_len = *p_resp_payload++;


        if(payload_len < (OAM_MGMT_IF_MIN_RESP_PAYLOAD_LEN + p_resp->err_str_len)) 
        {
            OAM_SNMP_LOG (SNMP, OAM_SNMP_WARNING, "Incoming message damaged");
			/* coverity 62592 */
			retVal = OAM_SNMP_UNEXPECTED_MSG;
            return retVal;
        }

        if(p_resp->err_str_len) {
            snmp_memcpy(p_resp->err_string, p_resp_payload, p_resp->err_str_len);
            OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING,
                         "Error string returned: %s", p_resp->err_string);
        }
 
        /* coverity 52818 */
        retVal = (mgmt_return_et)p_resp->err_code;
    }
    else {
        OAM_SNMP_LOG(SNMP, OAM_SNMP_DETAILED, "Success response received");
    }

    return retVal;
}

/******************************************************************************
* Function Name  : oam_snmp_create_new_trans_id
* Description    : generate new transcation id
* Input          : NONE
* OUTPUT     : transaction id
******************************************************************************/
UInt16 
oam_snmp_create_new_trans_id()
{
    static UInt16 seed = 1;
    return (seed++ == 0 ? 1 : seed);
}

/******************************************************************************
* Function Name: oam_snmp_fill_mgmt_if_req_payload
* Description  : Payload copied to the buffer allocated.
* Inputs       : ptr_msg : Pointer to buffer that is allocated for payload
*                rem_len : msg_length
*            oam_mgmt_if_req_t : structure pointer
* OUTPUTS      : NONE
* Returns      : void
******************************************************************************/
void 
oam_snmp_fill_mgmt_if_req_payload(UInt8 *ptr_msg, 
                                  UInt16 rem_len,
                                  oam_mgmt_if_req_t *p_req) 
{
    UInt8 count = 0;
    UInt16 param_info_len = 0;
    oam_mgmt_if_param_info_t *param = NULL;

    /* Length of Total Msg */
    UInt16 parent_name_len = oam_snmp_strlen(p_req->parent_name);

    /* Pack Total Length Of Parent Information of UInt16 Type */
    oam_mgmt_cp_pack_UInt16(ptr_msg, &rem_len);
    ptr_msg += SNMP_OAM_IF_MSG_WORD_LEN;

    OAM_SNMP_LOG (SNMP, OAM_SNMP_DETAILED, "Encoding parent info...");


    /* Pack Number of Parameter of UInt8 Type */
    *ptr_msg++ = p_req->num_params;
    OAM_SNMP_LOG(SNMP, OAM_SNMP_DETAILED,
                 "Number of parameters: %d", p_req->num_params);

    /* Pack Reserved Field of UInt8 Type */
    *ptr_msg++ = 0;

    /* Pack Reserved Field of UInt16 Type */
    *ptr_msg++ = 0;
    *ptr_msg++ = 0;

    /* Pack Length of Parent Node Name of UInt16 Type */
    oam_mgmt_cp_pack_UInt16(ptr_msg, &parent_name_len);
    ptr_msg += SNMP_OAM_IF_MSG_WORD_LEN;

    /* Pack Parent Node Name */
    snmp_memcpy(ptr_msg, p_req->parent_name, parent_name_len);
    ptr_msg += parent_name_len;


    param = p_req->param_list;
    for(count = 0; param && count < p_req->num_params; ++count) {

        /* Length of Total Msg */
        param_info_len = OAM_MGMT_IF_MIN_PARAM_INFO_LEN
                         + oam_snmp_strlen(param->param_name);

        /* Pack Length of Parameter Info of UInt16 Type */
        oam_mgmt_cp_pack_UInt16(ptr_msg, &param_info_len);
        ptr_msg += SNMP_OAM_IF_MSG_WORD_LEN;

        /* Pack Reserved Field of UInt16 Type */
        *ptr_msg++ = 0;
        *ptr_msg++ = 0;

        /* Pack Name=Value Pair */
        snmp_memcpy(ptr_msg, param->param_name,
               param_info_len - OAM_MGMT_IF_MIN_PARAM_INFO_LEN);
        OAM_SNMP_LOG(SNMP, OAM_SNMP_INFO,
                     "Paramter: %s", param->param_name);
        ptr_msg += (param_info_len  - OAM_MGMT_IF_MIN_PARAM_INFO_LEN);
        param = param->next;
    }
}

/******************************************************************************
 * Function Name: oam_snmp_set_req_handler
 * Description  : This function creates and sends API for SPV req to OAM.
 * Inputs       : p_req             : Pointer to oam_mgmt_if_req_t structure 
 * OUTPUTS      : p_resp_info : response message 
 * Returns      : OAM_SNMP_SUCCESS or some error code
 *******************************************************************************/
mgmt_return_et 
oam_snmp_set_req_handler(oam_mgmt_if_req_t *p_req, 
                         void *p_resp_info)
{
    UInt16 param_info_len = 0;
    UInt8 count = 0;
    UInt16 rem_len = OAM_ZERO;

    mgmt_return_et retVal = OAM_SNMP_SUCCESS;
    oam_mgmt_if_param_info_t *param = NULL;

    UInt8 *p_msg = NULL;
    UInt8 *ptr_msg = NULL;
    UInt16 trans_id = oam_snmp_create_new_trans_id();
/* CSR 57877 FIx Start*/
    UInt16 api_id = OAM_SET_PARAMETER_VALUES;
/* CSR 57877 FIx End */

    expected_trans_id = trans_id;
/* CSR 57877 FIx Start*/
    expected_api_id   = OAM_SET_PARAMETER_VALUES_RESP;
/* CSR 57877 FIx End */

    param = p_req->param_list;

    /* msg length with header size */
    UInt16 msg_size = OAM_MGMT_CSPL_HDR_SIZE + OAM_MGMT_INTF_HEADER_SIZE;

    /* Parent Node Info Length */
    UInt16 parent_info_len = OAM_MGMT_IF_MIN_PARENT_INFO_LEN
                             + oam_snmp_strlen(p_req->parent_name);   

    /* length of leaf level parameters */
    for(count = OAM_ZERO; param && count < p_req->num_params; ++count) {
        param_info_len += OAM_MGMT_IF_MIN_PARAM_INFO_LEN
                          + oam_snmp_strlen(param->param_name);
        param = param->next;
    }

    /* Total msg_size */
    msg_size += parent_info_len + param_info_len;

    retVal = oam_mgmt_mem_alloc(msg_size, (void**)&p_msg);
    if(OAM_SNMP_SUCCESS != retVal) {
        OAM_SNMP_LOG (SNMP, OAM_SNMP_ERROR,
                "Memory allocation to p_msg failed with error code: %d",
                retVal);
        return retVal;
    }

    ptr_msg = p_msg;
    rem_len = msg_size;

    /* Fill api header to buffer */
    oam_mgmt_construct_api_header(ptr_msg, OAM_VERSION,
                                  INTERFACE_MODULE_ID, OAM_MODULE_ID,
                                  api_id, rem_len);
    ptr_msg += OAM_MGMT_CSPL_HDR_SIZE;
    rem_len -= OAM_MGMT_CSPL_HDR_SIZE;

    /* Fill oam_mgmt_intf header to buffer */
    oam_construct_mgmt_intf_header(ptr_msg, trans_id,
                                   INTERFACE_MODULE_ID, OAM_MODULE_ID,
                                   api_id, rem_len);
    ptr_msg   += OAM_MGMT_INTF_HEADER_SIZE;
    rem_len   -= OAM_MGMT_INTF_HEADER_SIZE;

    /* Fill payload to buffer */
    oam_snmp_fill_mgmt_if_req_payload(ptr_msg, rem_len, p_req);

    /* send message to OAM */
    retVal = oam_mgmt_send_message(p_msg, OAM_MODULE_ID);

    if(OAM_SNMP_SUCCESS != retVal) {
        OAM_SNMP_LOG (SNMP, OAM_SNMP_ERROR, "Send failed...");
        return retVal;
    }

    /*** WAIT FOR RESPONSE ***/
    QTIME timer_timeout;
    timer_timeout.s = SNMP_OAM_IF_RESP_TIMEOUT_S;
    timer_timeout.us = SNMP_OAM_IF_RESP_TIMEOUT_US;
    OAM_SNMP_LOG (SNMP, OAM_SNMP_DETAILED," waiting for response from OAM ");
    waitstruct->sleep(channel ,&timer_timeout);

    if(SNMP_OAM_IF_MESSAGE_RECEIVED == wakeup_flag) {
        OAM_SNMP_LOG (SNMP, OAM_SNMP_INFO, "Response received");
        wakeup_flag = SNMP_OAM_IF_WAKEUP_TIMEDOUT; /* reset with default */

        retVal = oam_snmp_decode_mgmt_if_resp_msg(p_cntrl_ptr.msg_ptr,
                                                  OAM_MGMT_IF_SET_REQ,
                                                  p_resp_info);
    }       
    else if(SNMP_OAM_IF_WAKEUP_TIMEDOUT == wakeup_flag) {       
        expected_trans_id = 0;
        expected_api_id = 0;
        retVal = OAM_SNMP_TIMEOUT;       
        OAM_SNMP_LOG (SNMP, OAM_SNMP_WARNING,
                    "Timeout occured, message not received from OAM.");
    }       
    else    {
        retVal = OAM_SNMP_INT_ERROR; 
        OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING, "Error, shouldn't happen !!!");
    }


    return retVal; 
}



/****************************************************************************
 * Function Name: snmp_oam_handle_oam_notif
 * Description  : This function handles the alarm received from OAM and  
 *                sends it as a SNMP trap to SNMP master agent
 * Inputs       : Pointer to buffer received from OAM
 * OUTPUTS      : NONE
 * Returns      : 0 if success or a non-zero number indicating the error code
 ***************************************************************************/
SInt32 snmp_oam_handle_oam_notif(UInt8 *p_api_buf)
{
    UInt16 curr_pos = OAM_MGMT_INTF_HEADER_SIZE;
    snmp_oam_trap_param_t  trap_obj;
    UInt8 event_code_len = OAM_ZERO;
    UInt8 num_parent_blocks = OAM_ZERO;
    UInt8 parent_cnt = OAM_ZERO;
    UInt8 param_cnt = OAM_ZERO;
    UInt8 num_params = OAM_ZERO;
    UInt16 parent_name_len = OAM_ZERO;
    UInt16 name_val_pair_len = OAM_ZERO;
    UInt16 param_info_len = OAM_ZERO;
    Char8 name_val_pair[OAM_MGMT_IF_MAX_PARAM_NAME_LEN] = {OAM_ZERO};
    Char8 *param_name = OAM_NULL;
    Char8 *param_val = OAM_NULL;
    SInt32 retVal = OAM_ZERO;

    /* curr_pos will have the offset to the start of the next field to decode
     * from p_api_buf, curr_pos has been initialized to have the offset of
     * total payload length
     */

    /* We don't need to store the total length of payload, just skip it */
    curr_pos += SNMP_OAM_IF_MSG_WORD_LEN;

    /* Skip Reserved Bytes */
    curr_pos += 6;

    /* Decode Event Info Length */
    event_code_len = p_api_buf[curr_pos++];

    /* Skip Reserved Bytes */
    curr_pos += 3;

    /* Skip event code */
    curr_pos += event_code_len;

    /* Skip Reserved Bytes */
    curr_pos += 4;

    /* Skip total length of parameter list information */
    curr_pos += SNMP_OAM_IF_MSG_WORD_LEN;

    /* Decode Total Number of parents */
    num_parent_blocks = p_api_buf[curr_pos++];

    /* Skip Reserved Bytes */
    curr_pos += 5;


    /* Extract Parent Node Information */
    for(parent_cnt = 0; parent_cnt < num_parent_blocks ; ++parent_cnt) {
        /* Skip total length of parent information */
        curr_pos += SNMP_OAM_IF_MSG_WORD_LEN;

        num_params = p_api_buf[curr_pos++];


        /* Skip Reserved Bytes */
        curr_pos += 3;

        /* Decode Length of Parent Node Name */
        parent_name_len = get_word_from_header(p_api_buf + curr_pos);

        curr_pos += SNMP_OAM_IF_MSG_WORD_LEN;

        /* Skip Parent Node Name */
        curr_pos += parent_name_len;


        oam_mgmt_memset(&trap_obj, OAM_ZERO, sizeof(snmp_oam_trap_param_t));

        for(param_cnt = 0; param_cnt < num_params; ++param_cnt) {
            param_info_len = get_word_from_header(p_api_buf + curr_pos);
            curr_pos += SNMP_OAM_IF_MSG_WORD_LEN;

            /* Skip Reserved Bytes */
            curr_pos += 2;

            name_val_pair_len = param_info_len
                                - OAM_MGMT_IF_MIN_PARAM_INFO_LEN;

            /* Decode Name Value Pair */
            /* KLOCWORK warning fix start */
            oam_snmp_strncpy(name_val_pair, (Char8*)p_api_buf + curr_pos,
                        OAM_MGMT_IF_MAX_PARAM_NAME_LEN - 1);
            OAM_SNMP_LOG(SNMP, OAM_SNMP_INFO,
                        "Parameter received: %s", name_val_pair);
            param_name = strtok(name_val_pair, "=");
            OAM_NULL_CHECK(NULL != param_name); /* Coverity FIX 41203 */
            param_val = strtok(NULL, "=");
            OAM_NULL_CHECK(NULL != param_val); /* Coverity FIX 41203 */

            if(strstr(param_name, "EventTime")) {
                oam_snmp_strncpy(trap_obj.event_time, param_val,
                        sizeof(trap_obj.event_time) - 1);
            }
            else if(strstr(param_name, "AlarmIdentifier")) {
                oam_snmp_strncpy(trap_obj.trap_id, param_val,
                        sizeof(trap_obj.trap_id) - 1);
            }
            else if(strstr(param_name, "EventType")) {
                oam_snmp_strncpy(trap_obj.event_type, param_val,
                        sizeof(trap_obj.event_type) - 1);
            }
            else if(strstr(param_name, "ManagedObjectInstance")) {
                oam_snmp_strncpy(trap_obj.managed_obj_inst,
                            param_val,
                        sizeof(trap_obj.managed_obj_inst) - 1);
            }
            else if(strstr(param_name, "ProbableCause")) {
                oam_snmp_strncpy(trap_obj.prob_cause, param_val,
                        sizeof(trap_obj.prob_cause) - 1);
            }
            else if(strstr(param_name, "SpecificProblem")) {
                oam_snmp_strncpy(trap_obj.spec_problem, param_val,
                        sizeof(trap_obj.spec_problem) - 1);
            }
            else if(strstr(param_name, "PerceivedSeverity")) {
                oam_snmp_strncpy(trap_obj.perceived_sev, param_val,
                        sizeof(trap_obj.perceived_sev) - 1);
            }
            else if(strstr(param_name, "AdditionalText")) {
                oam_snmp_strncpy(trap_obj.add_text, param_val,
                        sizeof(trap_obj.add_text) - 1);
            }
            else if(strstr(param_name, "NotificationType")) {
                oam_snmp_strncpy(trap_obj.notif_type, param_val,
                        sizeof(trap_obj.notif_type) - 1);
            }
            else if(strstr(param_name, "AdditionalInformation")) {
                oam_snmp_strncpy(trap_obj.add_info, param_val,
                        sizeof(trap_obj.add_info) - 1);
            }
    /* KLOCWORK warning fix end */

            curr_pos += name_val_pair_len;
        }

        retVal = snmp_oam_send_snmp_trap(&trap_obj);
    }

    return retVal;
}


/******************************************************************************
 *   FUNCTION NAME  : oam_snmp_trace_message
 *   RETURNS        : None
 *   INPUTS         : log_level :Log level
 *                    pFile :Pointer to file
 *                    pFun : Pointer to function
 *                    Line : line
 *                    format :format.
 *   DESCRIPTION    : function to print trace message.
 ******************************************************************************/
void
oam_snmp_trace_message (SInt32 log_level,	/* log level of this message */
        		SInt8 * pFile, SInt8 * pFun, UInt16 Line, SInt8 * format,	/* format string like in printf */
        		...	/* optional parameters */
        		)
{
    U8 tmp_fmt[OAM_SNMP_TRACE_FORMAT_SIZE];
    U8 tmp_fmt1[OAM_SNMP_TRACE_FORMAT_SIZE];
    oam_mgmt_memset (tmp_fmt, OAM_ZERO, sizeof (tmp_fmt));
    oam_mgmt_memset (tmp_fmt1, OAM_ZERO, sizeof (tmp_fmt1));
                    
    va_list arglist;
    OAM_SNMP_VA_START (arglist, format);

    /*SPR 15198 Fixed Start */
    /* Code Removed*/
    /*SPR 15198 Fixed Stop */

    /* Start Fix for actual timestamp in logs */
    time_t secs_time;
    UInt8 time_string[OAM_SNMP_TRACE_TEMP_STR_SIZE] = { OAM_ZERO };
    struct tm *lt, result;

    /* Current time as seconds elapsed since the Epoch. */
    secs_time = oam_snmp_time ( OAM_NULL );
    lt = oam_snmp_localtime_r (&secs_time, &result);
    /* Convert to local time format. */
    if(OAM_NULL != lt)
    {
    oam_snmp_sprintf ((char *) time_string, "%d %d %d %d:%d:%d",
                lt->tm_mday, lt->tm_mon + 1, lt->tm_year + OAM_SNMP_TRACE_YEAR_FMT,
            lt->tm_hour, lt->tm_min, lt->tm_sec);
    /* End Fix for actual timestamp in logs */
    }

    switch (log_level)
    {
	    /*SPR 15198 Fixed Start*/
       case OAM_SNMP_DETAILED:
	    {
		vsprintf ((char *) tmp_fmt1, (const char *) format, arglist);
		oam_snmp_sprintf ((char *) tmp_fmt,
		    "\n[%s] [OAM_SNMP_DETAILED] %s [API:%s] [From FILE:%s] [Line No.:%d]\n",
		    time_string, tmp_fmt1, pFun, pFile, Line);
		break;
	    }
	    /*SPR 15198 Fixed End */



        case OAM_SNMP_INFO:
            {
                oam_snmp_sprintf ((char *) tmp_fmt1, (const char *) format, arglist);
                oam_snmp_sprintf ((char *) tmp_fmt,
                        "\n[%s] [OAM_SNMP_INFO] %s [API:%s] [From FILE:%s] [Line No.:%d]\n",
                        time_string, tmp_fmt1, pFun, pFile, Line);
                break;
            }
        case OAM_SNMP_WARNING:
            {
                oam_snmp_sprintf ((char *) tmp_fmt1, (const char *) format, arglist);
                oam_snmp_sprintf ((char *) tmp_fmt,
                        "\n[%s] [OAM_SNMP_WARNING] %s [API:%s] [From FILE:%s] [Line No.:%d]\n",
                        time_string, tmp_fmt1, pFun, pFile, Line);
                break;
            }
        case OAM_SNMP_ERROR:
            {
                oam_snmp_sprintf ((char *) tmp_fmt1, (const char *) format, arglist);
                oam_snmp_sprintf ((char *) tmp_fmt,
                        "\n[%s] [OAM_SNMP_ERROR] %s [API:%s] [From FILE:%s] [Line No.:%d]\n",
                        time_string, tmp_fmt1, pFun, pFile, Line);
                break;
            }
        case OAM_SNMP_CRITICAL:
            {
                oam_snmp_sprintf ((char *) tmp_fmt1, (const char *) format, arglist);
                oam_snmp_sprintf ((char *) tmp_fmt,
                        "\n[%s] [OAM_SNMP_CRITICAL] %s [API:%s] [From FILE:%s] [Line No.:%d]\n",
                        time_string, tmp_fmt1, pFun, pFile, Line);
                break;

            }
        default:
            {
		    /*SPR 15198 Fixed Start*/
		    oam_snmp_sprintf ((char *) tmp_fmt,"\n Invalid Log level");
		    /*SPR 15198 Fixed End*/
            }
    }

    if (log_level >= prov_req.log_level)
    {
        /* coverity 52820 */
        oam_snmp_fprintf (prov_req.p_out, "%s", (const char *) tmp_fmt);
	/*SPR 15198 Fixed Start*/
	fflush(prov_req.p_out);
	/*SPR 15198 Fixed End*/
    }

    OAM_SNMP_VA_END (arglist);
}
