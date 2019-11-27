#/******************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
# *
# ******************************************************************************
# *
# *  $Id: mac.inc,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
# *
# ******************************************************************************
# *
# *  File Description : MAC Enviornment Settings file
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: mac.inc,v $
# * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.2  2009/05/14 12:02:38  gur18550
# * Added ulHarq in Include
# *
# * Revision 1.1  2009/05/06 03:41:21  gur11912
# * Initial Version
# *
# *
# *****************************************************************************/

MKDIR=mkdir -p

MACCORE_COMMON_ROOT=${MACCORE_ROOT}/src/common
SCHEDULER_ROOT=${MACCORE_ROOT}/src/scheduler
ASSEMBLY_DEASSEMBLY_ROOT=${MACCORE_ROOT}/src/assemblyDeassemblyUnit
INTERFACES_ROOT=${MACCORE_ROOT}/src/interfaces
EXECUTION_SCHEDULER_ROOT=${MACCORE_ROOT}/src/executionScheduler
PORTING_ROOT=${MACCORE_ROOT}/src/porting
MACCORE_BIN_DIR=${MACCORE_ROOT}/bin

INCLUDE_LTE+= -I$(LTE_ROOT)/common/include/lte_port \
          -I${MACCOMMON_ROOT}/include \
          -I${MACCORE_ROOT}/include/common -I${MACCORE_ROOT}/include/scheduler/ScellManager \
          -I${MACCORE_ROOT}/include/scheduler/strategy \
          -I${MACCORE_ROOT}/include/scheduler/dlSchManager \
          -I${MACCORE_ROOT}/include/scheduler/ulSchManager -I${MACCORE_ROOT}/include/scheduler/bcchManager \
          -I${MACCORE_ROOT}/include/scheduler/ccchManager -I${MACCORE_ROOT}/include/scheduler/rachManager \
          -I${MACCORE_ROOT}/include/scheduler/pcchManager  -I${MACCORE_ROOT}/include/scheduler/resourceManager \
          -I${MACCORE_ROOT}/include/scheduler/DRXManager \
          -I${MACCORE_ROOT}/include/scheduler/MeasGapHandler \
          -I${MACCORE_ROOT}/include/scheduler/ECIDManager \
          -I${MACCORE_ROOT}/include/scheduler/PRSManager \
    	  -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/multiplexer \
          -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/encoder \
          -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/demultiplexer \
          -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/dlHarq \
          -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/ulHarq \
          -I${MACCORE_ROOT}/include/interfaces/rrc \
          -I${MACCORE_ROOT}/include/interfaces/rlc \
          -I${MACCORE_ROOT}/include/interfaces/pdcp \
          -I${MACCORE_ROOT}/include/interfaces/phy \
          -I${MACCORE_ROOT}/include/interfaces/phy/customer/mindspeed \
          -I${MACCORE_ROOT}/include/interfaces/oam \
          -I${MACCORE_ROOT}/include/executionScheduler \
          -I${MACCORE_ROOT}/include/porting \
          -I${LTE_ROOT}/gtpuPacketRelay/egtpu/stack/fsm/h \
          -I${LTE_ROOT}/gtpuPacketRelay/egtpu/shared/h \
          -I${LTE_ROOT}/gtpuPacketRelay/packetRelay/include \
          -I${MACCORE_ROOT}/include/interfaces/rrm 
ifeq ($(LTE_EMBMS_FLAG),1)
INCLUDE_LTE += -I${MACCORE_ROOT}/include/interfaces/sync
endif

CFLAGS +=$(FLAGS)
MACCORE_LIB_DIR=${MACCORE_ROOT}/lib
MACCORE_ARCHIVE =libmaccore.a 
