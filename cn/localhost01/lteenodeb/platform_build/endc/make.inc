#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: make.inc,v 1.6.2.2.4.2.2.7 2010/10/29 15:54:08 gur20056 Exp $
# *
# ****************************************************************************
# *
# *  File Description : LTE enviornment settings file
# *
# ****************************************************************************
# *
# * Revision Details
# * ----------------
# *
# * Release 6.3.1  2016/06/23 17:25:08  gur28898
# * SPR 20078 RRM_TARGET_ENV flag removed 
# *
# * $Log: make.inc,v $
# * Revision 1.6.2.2.4.2.2.7  2010/10/29 15:54:08  gur20056
# * rupesh - multi attach detach fix in pdcpadapter ULTG DLTG pdcp and mac
# *
# * Revision 1.6.2.2.4.2.2.6  2010/10/27 10:45:29  gur20056
# * changes for compiling default with O2 flag
# *
# * Revision 1.6.2.2.4.2.2.5  2010/10/15 11:03:08  gur20491
# * Added new macro RRC_2_0_1_DEFINED for enabling RRC 2.0.1 support in MAC
# *
# * Revision 1.6.2.2.4.2.2.4  2010/09/29 16:00:55  gur20491
# * updated to remove gmake
# *
# * Revision 1.6.2.2.4.2.2.3  2010/09/29 05:11:49  gur04640
# * undefined logging flag
# *
# * Revision 1.6.2.2.4.2.2.2  2010/09/27 16:49:11  gur10121
# * SPR 662 Fix
# *
# * Revision 1.6.2.2.4.2.2.1  2010/09/21 15:45:37  gur20491
# * FAPI changes
# *
# * Revision 1.6.2.2  2010/08/02 08:57:13  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.5  2010/03/11 07:30:33  gur18550
# * Forced_MIMO
# *
# * Revision 1.4  2010/03/04 04:54:13  gur18550
# * Forced_Mimo
# *
# * Revision 1.3  2010/02/18 14:42:07  gur20619
# * Defined flag BUILD_TRAFFIC_GEN
# *
# * Revision 1.2  2010/02/18 08:02:31  gur15697
# * -g compilation flag removed
# *
# * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
# * eNB framework for intel
# *
# * Revision 1.14.4.10.2.5  2009/12/05 13:32:38  gur19479
# * updated for compiling differet platform
# *
# * Revision 1.14.4.10.2.4  2009/12/04 06:34:48  gur20439
# * TEMP_FIX flag inserted for temp fix code.
# *
# * Revision 1.14.4.10.2.3  2009/11/13 13:17:05  gur19140
# * added pdcp msb flag
# *
# * Revision 1.14.4.10.2.2  2009/10/09 05:53:57  gur11318
# * New flag added with name OAM_WITH_RRC.
# *
# * Revision 1.14.4.10.2.1  2009/09/11 12:34:41  gur12140
# * Modified the flag related to intel.
# *
# * Revision 1.14.4.10  2009/09/03 06:15:11  gur19140
# * adding cycle count code
# *
# * Revision 1.14.4.9  2009/08/27 11:22:10  gur19140
# * *** empty log message ***
# *
# * Revision 1.14.4.7  2009/07/28 15:36:08  gur19836
# * Downlink Issues Resolved, Latency Measurement code added
# *
# * Revision 1.14.4.6  2009/07/21 08:04:55  gur19140
# * Bug Fixes
# *
# * Revision 1.14.4.5  2009/07/14 14:07:05  gur20435
# * Removed flag INTEL_EXEC_SCHEDULER_OP1
# *
# * Revision 1.14.4.4  2009/07/08 08:42:24  gur19836
# * Tx flow Optimization changes
# *
# * Revision 1.14.4.3  2009/07/07 08:07:19  gur19836
# * Compilation Issues Resolved
# *
# * Revision 1.14.4.2  2009/06/19 14:30:13  gur19836
# * Flags added for execution scheduler
# *
# * Revision 1.14.4.1  2009/06/09 12:21:15  gur19140
# * first wave of Integration
# *
# * Revision 1.18  2009/05/28 17:03:40  gur19140
# * *** empty log message ***
# *
# * Revision 1.17  2009/05/20 15:16:45  gur20548
# * *** empty log message ***
# *
# * Revision 1.16  2009/05/12 14:53:02  gur19479
# * adding pdcp root path
# *
# * Revision 1.15  2009/05/06 03:56:58  gur11912
# * Added MACCORE_ROOT
# *
# * Revision 1.14  2009/04/08 08:49:18  gur20435
# *
# * 	Removed the m32 option from CFLAGS
# *
# * Revision 1.13  2009/04/03 13:24:54  gur20439
# * speling correction.
# *
# * Revision 1.12  2009/04/02 11:04:12  gur20439
# * CFLAGS modifed to get compilation flags from command line.
# *
# * Revision 1.11  2009/03/23 09:20:39  gur19140
# * Integration changes first waves
# *
# * Revision 1.10  2009/03/23 08:37:17  gur19140
# * Integration changes first waves
# *
# * Revision 1.9  2009/03/13 08:39:32  gur19836
# * ZERO_BUFFER_COPY flag commented as it will be used only in case of integrated layer2
# *
# * Revision 1.8  2009/03/12 10:48:39  gur19140
# * Regarding Zero Buffer copy changes in DL
# *
# * Revision 1.7  2009/01/29 17:43:23  gur03939
# * Removed default UT_TESTING flag
# *
# * Revision 1.6  2009/01/27 17:20:26  gur19413
# * UT_TESTING SHM_INTF PHYSIM_TESTING Flags added
# *
# * Revision 1.5  2009/01/20 16:11:19  gur11912
# * Updated SIM_ROOT
# *
# * Revision 1.4  2009/01/19 14:18:09  gur18569
# * removed testing flags
# *
# * Revision 1.3  2009/01/19 14:16:17  gur18569
# * mac/simulator/phy/src/phySimulator.c
# *
# * Revision 1.2  2008/12/30 05:15:21  gur20294
# * RLC_ROOT is added to (Makefile make.inc) to make LTE RLC layer.
# *
# * Revision 1.1  2008/09/05 08:16:24  ssinghal
# * Makefile changes
# *
# *
# *
# *
# ****************************************************************************/
GMAKE=make

MAKE=${GMAKE}

CC = $(LTE_CC) 
AR = $(LTE_AR)
LD = $(LTE_LD)
RANLIB = $(LTE_RANLIB)
RM=rm -f
RMD=rm -fr
#We create the directory only once
MKD=mkdir -p


#ifeq ($(CC),gcc)
#  AR = ar
#  OPTIM_FLAGS = 
#else
#  AR = xiar
#  OPTIM_FLAGS = -xHost -O3 -ipo -no-prec-div -vec-guard-write -opt-multi-version-aggressive -opt-calloc -opt-subscript-in-range -fno-alias -fomit-frame-pointer -funroll-loops -complex-limited-range -vec-report2 -fp-model fast=2 -no-prec-sqrt -fast-transcendentals -rcd -ftz -Zp16 -align -restrict -falign-functions=16
#endif
#6.3.0 compiler options
#SPR 19713 change start
#OPTIM_FLAGS = -O3 -Wstrict-prototypes -Wformat -Wpointer-sign -Wsign-compare -Wunused-but-set-variable -Wmissing-field-initializers -Wformat-extra-args -Wmissing-field-initializers -Wunused-parameter
#SPR 19713 change end
#OPTIM_FLAGS = -O3

MAKE_CLEAN=${GMAKE} -i

#OPTIM_FLAGS += -fstack-protector-all

#FAPI_2_4_COMPLIANCE compilation flag below supports Small Cell Forum Release 4, LTE eNB L1 API definition, Version: 082.4.1, Date:25-Feb-2014

CFLAGS =  -c -g -O2 -Wall -fno-aggressive-loop-optimizations $(OPTIM_FLAGS) \
-USEM_SYNC \
-UL2_FAST_PATH \
-D_GNU_SOURCE \
-DMSG_BYTE_ORDER_MSB \
-UNANO_SLEEP_TIMER \
-UTDD_CONFIG \
-DFDD_CONFIG \
-DMULTI_UE_ENHANCEMENT \
-DMULTI_UE_ENHANCEMENT_FRWK \
-DULHARQ_TIMER_PROC \
-UCYCLE_COUNT_MAC_RLC_PDCP \
-ULOG_PRINT_ENABLED \
-UCONSOLE_PRINT_ENABLED \
-DSHM_PRINT_ENABLED \
-ULOCKLESS_IMPL \
-ULAYER2_DLTG_THREAD \
-UROHC_PROFILE_6 \
-UUE_IPV6_ADDR \
-URLC_QUEUE_STATS \
-UACK_NACK_COUNT_STATS \
-D_FORTIFY_SOURCE=0 \
-UMALLOC_HOOK \
-UEPC_SETUP \
-UDISABLE_UL_CQI_PROCESSING \
-UUTFWK_SIMULATION \
-UMEM_LEAK_DEBUG \
-USSI_DEBUG_STATS \
-DKPI_STATS \
-UQOS_SI_FWK_TEST \
-UPDCP_ASYNC_INTERFACE \
-DPOLL_JOB_RING \
-URLC_PDCP_IPC_INTF \
-URLC_STANDALONE_SETUP \
-ULAYER2_PHYSIM_THREAD \
-UUESIM_PDCP_THREAD \
-UCYCLE_COUNT_1MS_RDB \
-UGETSTATS_EL_CLOCKTIME \
-UTA_AVG \
-UOVERSHOOT_CORRECTION \
-UZBC_STRUCT_ALIGN_64BYTE \
-UOTA_DL_ADVANCE_2 \
-DPERF_STATS \
-UHDFDD_STATS \
-DHD_FDD_CONFIG \
-UCIRC_QUEUE_IMPL \
-UDLPKT_DROP_UE_PDCP \
-UUL_MU_MIMO_BUILD_ENABLED \
-UFAPI_1_1_COMPLIANCE \
-UINTER_CELL_HO \
-UPDCP_ASYNC_INTR_TESTING \
-UPDCP_ASYNC_INTR \
-UPDCP_ASYNC_TESTING_THR \
-UUESIM_ENHANCEMENTS \
-DPUCCH_RESERVE_ALWAYS \
-UMAC_AUT_TEST \
-UCSI_VALIDATION \
-UPREVENT_SR_DROP \
-UTDD_MODE_FLAG \
-URRM_DEBUG \
-DLAYER2_ALARM_REPORTING_ENABLED \
-UMAC_PHY_DIFF_ENDIAN \
-UDL_DATA_SEPARATION \
-UREAL_UE_ENVIORNMENT \
-UFAPI_L2_MASTER \
-DFAPI_4_0_COMPLIANCE \
-UFAPI_RELEASE8 \
-UFAPI_RELEASE9 \
-UIPR_LAA \
-DFAPI_RELEASE10 \
-UPDCP_GTPU_INTF \
-UHIGH_SCALABILITY_L2_MAX_UE \
-UHIGH_SCALABILITY_L3_MAX_UE \
-UCRAN_RLC_PDCP_SPLIT_FILE_WRITER \
-UCRAN_RLC_PDCP_SPLIT \
-ULTE_EMBMS_SUPPORTED \
-ULTE_EMBMS_UESIM_TESTING \
-UIPR_NBIOT \
-UIPR_EMTC \
-UFAPI_9_0_COMPLIANCE \
-UFLEXRAN \
-UFLEXRAN_FAPI_LOGGING \
-UPDCP_DPDK \
-UH3C_USECASE_2 \
-UH3C_USECASE_2_JUMBO \
-DENDC_ENABLED

# Layer2 Logging Flags
CFLAGS += -DLTE_MAC_LOG_FATAL_ENABLE \
-DLTE_MAC_LOG_ERROR_ENABLE \
-DLTE_MAC_LOG_WARNING_ENABLE \
-DLTE_MAC_LOG_INFO_ENABLE \
-DLTE_MAC_LOG_BRIEF_ENABLE \
-DLTE_MAC_LOG_DEBUG_ENABLE \
-DLTE_MAC_LOG_TTI_DEBUG_ENABLE \
-DLTE_RLC_LOG_FATAL_ENABLE \
-DLTE_RLC_LOG_ERROR_ENABLE \
-DLTE_RLC_LOG_WARNING_ENABLE \
-DLTE_RLC_LOG_INFO_ENABLE \
-DLTE_RLC_LOG_BRIEF_ENABLE \
-DLTE_RLC_LOG_DEBUG_ENABLE \
-DLTE_RLC_LOG_TTI_DEBUG_ENABLE \
-DLTE_PDCP_LOG_FATAL_ENABLE \
-DLTE_PDCP_LOG_ERROR_ENABLE \
-DLTE_PDCP_LOG_WARNING_ENABLE \
-DLTE_PDCP_LOG_INFO_ENABLE \
-DLTE_PDCP_LOG_BRIEF_ENABLE \
-DLTE_PDCP_LOG_DEBUG_ENABLE \
-DLTE_PDCP_LOG_TTI_DEBUG_ENABLE \
-DLTE_GTPUPR_LOG_FATAL_ENABLE \
-DLTE_GTPUPR_LOG_ERROR_ENABLE \
-DLTE_GTPUPR_LOG_WARNING_ENABLE \
-DLTE_GTPUPR_LOG_INFO_ENABLE \
-DLTE_GTPUPR_LOG_BRIEF_ENABLE \
-DLTE_GTPUPR_LOG_DEBUG_ENABLE \
-DLTE_GTPUPR_LOG_TTI_DEBUG_ENABLE \
-DLTE_SYNC_LOG_FATAL_ENABLE \
-DLTE_SYNC_LOG_ERROR_ENABLE \
-DLTE_SYNC_LOG_WARNING_ENABLE \
-DLTE_SYNC_LOG_INFO_ENABLE \
-DLTE_SYNC_LOG_BRIEF_ENABLE \
-DLTE_SYNC_LOG_DEBUG_ENABLE \
-DLTE_SYNC_LOG_TTI_DEBUG_ENABLE \
-DLTE_LR_LOG_FATAL_ENABLE \
-DLTE_LR_LOG_ERROR_ENABLE \
-DLTE_LR_LOG_WARNING_ENABLE \
-DLTE_LR_LOG_INFO_ENABLE \
-DLTE_LR_LOG_BRIEF_ENABLE \
-DLTE_LR_LOG_DEBUG_ENABLE \
-DLTE_LR_LOG_TTI_DEBUG_ENABLE


LFLAGS = $(OC_FLAGS)

COMPILE = $(CC) $(CFLAGS) ${INCLUDE_LTE}

MAKE_BIN = $(LTE_MAKE_BIN) $(LFLAGS) $(LIBS_DIR)

MAKE_LIB = ${AR}

MAC_ROOT=${LTE_ROOT}/mac
MACCORE_ROOT=${MAC_ROOT}/maccore
MACCOMMON_ROOT=${MAC_ROOT}/common
RLC_ROOT=${LTE_ROOT}/rlc
SYNC_ROOT=${LTE_ROOT}/sync

COMMON_ROOT=${LTE_ROOT}/common

COMMON_LIB_DIR=${COMMON_ROOT}/lib

COMMON_LIB_UESIM_DIR=${LTE_ROOT}/common/lib_uesim

COMMON_LIB_PATH=-L${COMMON_ROOT}/lib


#SPR 17558 - start : Added to manage platorm specicic compilation
##The blank macros are deliberate
PL_LIB_PATH =
OAMAPP_PL_LIB = 
OAM_APPS = snmp
OAM_APPS_LIB = -lcspl -ladvlogger -lpthread -lnetsnmpmibs -lnetsnmpagent -lnetsnmp -lrt
OAM_EXTERNAL_INCLUDES = 
#SPR 17558 - end
#SPR 19288 change start
#SPR 19713 changes start
#remove platform specific if condition 19288 spr
#SPR 19713 changes end
INCLUDE_LTE+=	-I${COMMON_ROOT}/include -I${LTE_ROOT}/commonInterface/interfaces/include/ -I$(COMMON_ROOT)/loggingFrameWork/logging/inc/ -I$(COMMON_ROOT)/loggingFrameWork/alarm/inc/ -I$(COMMON_ROOT)/loggingFrameWork/common/inc/ -I${LTE_ROOT}/simulators/enbPhySim/include/  -I${LINUX_SOURCE} -I${LTE_ROOT}/mac/maccore/include/interfaces/phy/ -I$(L1_SOURCE)/CPU_COMMONS/externals  -I$(L1_SOURCE)/CPUL/LTE_L1/externals -I$(L1_SOURCE)/CPUL/LTE_L1/includes -I$(RLC_ROOT)/rlccore/include/common
#SPR 19288 change end
PHY_SIM_EXEC_PATH=${LTE_ROOT}/mac/simulator/phy/bin

PHY_SIM_EXEC_NAME=phySim

RLC_SIM_EXEC_PATH=${LTE_ROOT}/mac/simulator/rlc/bin

RLC_SIM_EXEC_NAME=rlcSim

RRC_SIM_EXEC_PATH=${LTE_ROOT}/mac/simulator/rrc/bin

RRC_SIM_EXEC_NAME=rrcSim

PDCP_ROOT = $(LTE_ROOT)/pdcp

PACKET_RELAY_ROOT=$(LTE_ROOT)/gtpuPacketRelay/packetRelay

EGTPU_ROOT = $(LTE_ROOT)/gtpuPacketRelay/egtpu

LTE_RRM = $(LTE_ROOT)/lte_rrm

RRM_CELL_MGR_ROOT = $(LTE_ROOT)/lte_rrm/core/cell_manager

RRM_UE_MGR_ROOT = $(LTE_ROOT)/lte_rrm/core/ue_manager

RRM_MIF_ROOT = $(LTE_ROOT)/lte_rrm/interface

LTE_SON = $(LTE_ROOT)/lte_son

SON_NMM_ROOT = $(LTE_ROOT)/lte_son/core/son_nmm

SON_APPS_ROOT = $(LTE_ROOT)/lte_son/core/son_apps

SON_ANR_ROOT = $(LTE_ROOT)/lte_son/core/son_anr

SON_X2_ROOT = $(LTE_ROOT)/lte_son/core/son_x2

SON_ES_ROOT = $(LTE_ROOT)/lte_son/core/son_es

SON_MIF_ROOT = $(LTE_ROOT)/lte_son/interface
SON_IMPORT_ROOT = $(LTE_ROOT)/lte_son/import
SON_MRO_ROOT = $(LTE_ROOT)/lte_son/core/son_mro
SON_MLB_ROOT = $(LTE_ROOT)/lte_son/core/son_mlb
SON_RACH_OPT_ROOT = $(LTE_ROOT)/lte_son/core/son_rach_opt
