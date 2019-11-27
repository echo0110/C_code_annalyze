#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: Makefile,v 1.1.1.1.16.1 2010/10/27 14:08:45 gur20056 Exp $
# *
# ****************************************************************************
# *
# *  File Description : Makefile
# *
# ****************************************************************************
# *
# * Revision Details
# * ----------------
# *
# * $Log: Makefile,v $
# * Revision 1.1.1.1.16.1  2010/10/27 14:08:45  gur20056
# * removed -g flag
# *
# * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
# * eNB framework for intel
# *
# * Revision 1.5.4.4.2.2  2009/09/30 10:37:15  gur20439
# * MAC1.2 related integration changes.
# *
# * Revision 1.5.4.4.2.1  2009/09/11 12:47:46  gur12140
# * modified to build with MAC 1.2
# *
# * Revision 1.5.4.4  2009/08/27 11:25:46  gur19140
# * *** empty log message ***
# *
# * Revision 1.5.4.3  2009/08/27 11:22:32  gur19140
# * *** empty log message ***
# *
# * Revision 1.5.4.2  2009/07/03 14:04:10  gur19479
# * updated for pdcp adapter
# *
# * Revision 1.5.4.1  2009/06/09 12:02:47  gur19140
# * first wave of Integration
# *
# * Revision 1.10  2009/05/28 17:03:40  gur19140
# * *** empty log message ***
# *
# * Revision 1.9  2009/05/15 09:11:52  gur19140
# * fixes
# *
# * Revision 1.8  2009/05/13 09:01:51  gur19140
# * *** empty log message ***
# *
# * Revision 1.7  2009/05/12 13:36:13  gur19479
# * pdcp changes
# *
# * Revision 1.6  2009/04/15 15:56:10  gur19140
# * rlc latency related changes
# *
# * Revision 1.5  2009/04/06 07:28:46  gur19140
# * integration changes
# *
# * Revision 1.4  2009/04/03 15:46:09  gur19140
# * mac sim as lib
# *
# * Revision 1.3  2009/03/23 08:37:17  gur19140
# * Integration changes first waves
# *
# * Revision 1.2  2009/01/21 17:38:50  gur18569
# * replaced gcc with $(CC)
# *
# * Revision 1.1  2009/01/21 10:19:40  gur19836
# * Directory structure changed
# *
# * Revision 1.6  2009/01/20 16:26:18  gur11912
# * Updated for Porting
# *
# * Revision 1.5  2009/01/20 08:00:01  gur19836
# * Updated for RRC Interface changes
# *
# * Revision 1.4  2009/01/15 10:44:28  gur18569
# * removed -lpthread option
# *
# * Revision 1.3  2008/12/09 12:31:10  gur03939
# * Updated during linking
# *
# * Revision 1.2  2008/12/09 11:24:06  gur19836
# * Updated during linking
# *
# * Revision 1.1  2008/12/08 13:07:48  gur12905
# * Initial Revision phase - 2
# *
# *
# *
# *
# ****************************************************************************/
##############################################################################
#
#		MAIN MAKE FILE						 
#	Make file to be used for building OAM. 
#						
##############################################################################
include ${LTE_ROOT}/make.inc
#Variables for  MAC 
TARGET = lteLayer2


SYNC_DIR            = ${LTE_ROOT}/sync/bin/
MAC_COMMON_LIB_DIR            = ${LTE_ROOT}/mac/common/lib/
MAC_LAYER_LIB_DIR            = ${LTE_ROOT}/mac/maccore/lib/
ifeq ($(INTEL_T3300),intel_t3300_ca)
MSPD_ICPU_LIB_DIR            = ${LTE_ROOT}/bin/
MSPD_ICPU_LIBS               = $(MSPD_ICPU_LIB_DIR)libshm.so
endif
MAC_LAYER_INCLUDE_DIR        = ${LTE_ROOT}/mac/maccore/include
SCHSIM_LIB_DIR               = ${LTE_ROOT}/mac/scheduler/schSimulator/lib
#distributor include path
DISTRIBUTER_INCLUDE_DIR        = ${LTE_ROOT}/distributer/include
MACCORE_ROOT = ${LTE_ROOT}/mac/maccore
#MAC_LAYER_INC_PATH			= $(MAC_LAYER_INCLUDE_DIR)/common \
#								-I$(MAC_LAYER_INCLUDE_DIR)/scheduler \
#								-I$(MAC_LAYER_INCLUDE_DIR)/harq \
#								-I$(MAC_LAYER_INCLUDE_DIR)/demux \
#								-I$(MAC_LAYER_INCLUDE_DIR)/interfaces 
ifeq ($(INTEL_T3300),intel_t3300_ca)
MAC_LAYER_INC_PATH                      = $(MAC_LAYER_INCLUDE_DIR)/common \
                                                               -I$(MAC_LAYER_INCLUDE_DIR)/scheduler \
                                                                -I$(MAC_LAYER_INCLUDE_DIR)/harq \
                                                                -I$(MAC_LAYER_INCLUDE_DIR)/demux \
                                                                -I$(MAC_LAYER_INCLUDE_DIR)/interfaces \
                                                                -I$(MAC_LAYER_INCLUDE_DIR)/interfaces/oam \
                                                                -I$(MAC_LAYER_INCLUDE_DIR)/interfaces/phy \
								-I$(MAC_LAYER_INCLUDE_DIR)/interfaces/phy/customer/mindspeed \
                                                                -I$(MAC_LAYER_INCLUDE_DIR)/interfaces/rlc \
                                                                -I$(MAC_LAYER_INCLUDE_DIR)/interfaces/rrc \
                                                                -I$(MAC_LAYER_INCLUDE_DIR)/executionScheduler \
                                                                -I${MAC_LAYER_INCLUDE_DIR}/scheduler/ScellManager \
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/ulSchManager \
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/bcchManager \
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/ccchManager \
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/rachManager \
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/pcchManager \
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/resourceManager\
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/DRXManager\
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/MeasGapHandler\
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/dlSchManager \
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/ECIDManager \
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/PRSManager \
															   -I${MAC_LAYER_INCLUDE_DIR}/interfaces/rrm/\
															   -I${MAC_LAYER_INCLUDE_DIR}/porting/\
															   -I${MAC_LAYER_INCLUDE_DIR}/interfaces/sync/\
                                                               -I${DISTRIBUTER_INCLUDE_DIR}
else
MAC_LAYER_INC_PATH                      = $(MAC_LAYER_INCLUDE_DIR)/common \
                                                               -I$(MAC_LAYER_INCLUDE_DIR)/scheduler \
                                                                -I$(MAC_LAYER_INCLUDE_DIR)/harq \
                                                                -I$(MAC_LAYER_INCLUDE_DIR)/demux \
                                                                -I$(MAC_LAYER_INCLUDE_DIR)/interfaces \
                                                                -I$(MAC_LAYER_INCLUDE_DIR)/interfaces/oam \
                                                                -I$(MAC_LAYER_INCLUDE_DIR)/interfaces/phy \
                                                                -I$(MAC_LAYER_INCLUDE_DIR)/interfaces/rlc \
                                                                -I$(MAC_LAYER_INCLUDE_DIR)/interfaces/rrc \
                                                                -I$(MAC_LAYER_INCLUDE_DIR)/executionScheduler \
                                                                -I${MAC_LAYER_INCLUDE_DIR}/scheduler/ScellManager \
                                                                -I${MAC_LAYER_INCLUDE_DIR}/scheduler/LaaCellManager \
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/ulSchManager \
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/bcchManager \
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/ccchManager \
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/rachManager \
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/pcchManager \
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/resourceManager\
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/DRXManager\
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/MeasGapHandler\
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/dlSchManager \
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/ECIDManager \
                                                               -I${MAC_LAYER_INCLUDE_DIR}/scheduler/PRSManager \
															   -I${MAC_LAYER_INCLUDE_DIR}/interfaces/rrm/\
															   -I${MAC_LAYER_INCLUDE_DIR}/porting/\
                                                               -I${DISTRIBUTER_INCLUDE_DIR}
endif

#Variables for RLC 
RLC_LAYER_LIB_DIR        = ${LTE_ROOT}/rlc/rlccore/lib/
RLC_LAYER_INCLUDE_DIR    = ${LTE_ROOT}/rlc/rlccore/include
RLC_LAYER_INC_PATH		 = $(RLC_LAYER_INCLUDE_DIR)/common 	\
								-I$(RLC_LAYER_INCLUDE_DIR)/ammode	\
								-I$(RLC_LAYER_INCLUDE_DIR)/interfaces	\
								-I$(RLC_LAYER_INCLUDE_DIR)/tmmode	\
								-I$(RLC_LAYER_INCLUDE_DIR)/ummode	

#Variables for eGTPU
eGTPU_LIB_DIR            = ${LTE_ROOT}/gtpuPacketRelay/egtpu/stack/
eGTPU_INCLUDE_DIR        = -I ${LTE_ROOT}/gtpuPacketRelay/egtpu/stack/fsm/h/
eGTPU_INCLUDE_DIR        += -I ${LTE_ROOT}/gtpuPacketRelay/egtpu/shared/h/

#Variables for Packet Relay
PACKET_RELAY_LIB_DIR            = ${LTE_ROOT}/gtpuPacketRelay/packetRelay/bin
PACKET_RELAY_INCLUDE_DIR        = ${LTE_ROOT}/gtpuPacketRelay/packetRelay/include

#Variables for PDCP
PDCP_LAYER_LIB_DIR            = ${LTE_ROOT}/pdcp/pdcpcore/lib/
PDCP_LAYER_INCLUDE_DIR        = ${LTE_ROOT}/pdcp/pdcpcore/include

#Variables for  MAC Sim
MAC_SIM_HOME           = ${LTE_ROOT}/rlc/simulator/mac/
MAC_SIM_LIB_DIR        = ${MAC_SIM_HOME}/lib
MAC_SIM_INCLUDE_DIR    = ${MAC_SIM_HOME}/include

#Variables for  RLC Sim
RLC_SIM_HOME           = ${LTE_ROOT}/mac/simulator/rlc
RLC_SIM_LIB_DIR        = ${RLC_SIM_HOME}/lib
RLC_SIM_INCLUDE_DIR    = ${RLC_SIM_HOME}/include

#Variables for  PDCP Sim
PDCP_SIM_HOME           = ${LTE_ROOT}/rlc/simulator/pdcp/
PDCP_SIM_LIB_DIR        = ${PDCP_SIM_HOME}/lib
PDCP_SIM_INCLUDE_DIR    = ${PDCP_SIM_HOME}/include


COMM_LIB_DIR       = ${LTE_ROOT}/common/lib
COMM_LOG_LIB_DIR       = ${LTE_ROOT}/common/loggingFrameWork/lib/
LAYER2_OAM_HOME    = ${LTE_ROOT}/commonInterface/OAMApp
LAYER2_OAM_BIN_DIR = ${LAYER2_OAM_HOME}/bin
LAYER2_OAM_SRC_DIR     = ${LAYER2_OAM_HOME}/src
LAYER2_OAM_INCLUDE_DIR = ${LAYER2_OAM_HOME}/include
LAYER2_OAM_EXEC_NAME   = lteLayer2


#Variables for MAC RLC PDCP integration
INTERFACES_SRC_DIR = ${LTE_ROOT}/commonInterface/interfaces/src/
INTERFACES_INC_DIR = ${LTE_ROOT}/commonInterface/interfaces/include/
PHYSIM_OBJ = #${LTE_ROOT}/simulators/enbPhySim/bin/lteEnbPhySim.o

INCLUDE_PATH_MAC = -I$(COMMON_ROOT)/include -I$(COMMON_ROOT)/include/lte_port -I$(INTERFACES_INC_DIR) -I$(RLC_SIM_INCLUDE_DIR) -I$(MAC_LAYER_INC_PATH) -I$(LAYER2_OAM_INCLUDE_DIR)

INCLUDE_PATH_DISTRIBUTOR = -I$(DISTRIBUTER_INCLUDE_DIR)

INCLUDE_PATH_MAC_RLC = -I$(COMMON_ROOT)/include -I$(COMMON_ROOT)/include/lte_port -I$(INTERFACES_INC_DIR) -I$(RLC_LAYER_INC_PATH) -I$(MAC_LAYER_INC_PATH) -I$(LAYER2_OAM_INCLUDE_DIR) -I$(PDCP_SIM_INCLUDE_DIR) $(eGTPU_INCLUDE_DIR) -I$(PACKET_RELAY_INCLUDE_DIR)


SEARCH_PATH_RLC     = -L$(COMM_LIB_DIR) -L$(COMM_LOG_LIB_DIR) -L$(RLC_LAYER_LIB_DIR)  -L$(MAC_SIM_LIB_DIR) -L$(PDCP_SIM_LIB_DIR)
ifeq ($(INTEL_T3300),intel_t3300_ca)
SEARCH_PATH_MAC     = -L$(COMM_LIB_DIR) -L$(COMM_LOG_LIB_DIR) -L$(MAC_COMMON_LIB_DIR) -L$(SCHSIM_LIB_DIR) -L$(MAC_LAYER_LIB_DIR) -L$(MSPD_ICPU_LIB_DIR) -L$(RLC_SIM_LIB_DIR)
SEARCH_PATH_MAC_RLC     = -L$(COMM_LIB_DIR) -L$(COMM_LOG_LIB_DIR) -L$(MAC_COMMON_LIB_DIR) -L$(SCHSIM_LIB_DIR) -L$(MAC_LAYER_LIB_DIR) -L$(MSPD_ICPU_LIB_DIR) -L$(RLC_LAYER_LIB_DIR) -L$(PDCP_LAYER_LIB_DIR) -L$(eGTPU_LIB_DIR) -L$(PACKET_RELAY_LIB_DIR)
else
SEARCH_PATH_MAC     = -L$(COMM_LIB_DIR) -L$(COMM_LOG_LIB_DIR) -L$(MAC_COMMON_LIB_DIR) -L$(SCHSIM_LIB_DIR) -L$(MAC_LAYER_LIB_DIR)  -L$(RLC_SIM_LIB_DIR)
SEARCH_PATH_MAC_RLC     = -L$(COMM_LIB_DIR) -L$(COMM_LOG_LIB_DIR) -L$(MAC_COMMON_LIB_DIR) -L$(SCHSIM_LIB_DIR) -L$(MAC_LAYER_LIB_DIR) -L$(RLC_LAYER_LIB_DIR) -L$(PDCP_LAYER_LIB_DIR) -L$(eGTPU_LIB_DIR) -L$(PACKET_RELAY_LIB_DIR)
SEARCH_PATH_MAC_RLC_GTPU     = -L$(COMM_LIB_DIR) -L$(COMM_LOG_LIB_DIR) -L$(MAC_COMMON_LIB_DIR) -L$(SCHSIM_LIB_DIR) -L$(MAC_LAYER_LIB_DIR) -L$(RLC_LAYER_LIB_DIR) -L$(PDCP_LAYER_LIB_DIR)
endif

CFLAGS += $(FLAGS) 
PDCP_ROOT= $(LTE_ROOT)/pdcp
PDCP_LIB_PATH= $(PDCP_ROOT)/lib
PDCP_ADAPTER_LIB_PATH= #$(LTE_ROOT)/simulators/adapter/pdcp/lib

ifeq ($(INTEL_T3300),intel_t3300_ca)
INCLUDE_PATH_PDCP = -I$(COMMON_ROOT)/include -I$(COMMON_ROOT)/include/lte_port -I$(INTERFACES_INC_DIR) -I$(LAYER2_OAM_INCLUDE_DIR) -I$(PDCP_ROOT)/simulator/pdcpuser/include -I$(PDCP_ROOT)/pdcpCommon/include -I${MAC_LAYER_INC_PATH}
else
INCLUDE_PATH_PDCP = -I$(COMMON_ROOT)/include -I$(COMMON_ROOT)/include/lte_port -I$(INTERFACES_INC_DIR) -I$(LAYER2_OAM_INCLUDE_DIR) -I$(PDCP_ROOT)/simulator/pdcpuser/include -I$(PDCP_ROOT)/pdcpCommon/include
endif

INCLUDE_PATH_PDCP_ADAPTER = #-I$(LTE_ROOT)/simulators/adapter/pdcp/include

LD_FLAGS= $(PDCP_LIB_PATH)/librlcsim.a $(PDCP_LIB_PATH)/libpdcpuser.a $(PDCP_LIB_PATH)/libpdcp.a $(PDCP_LIB_PATH)/libintegrity.a $(PDCP_LIB_PATH)/libciphering.a $(PDCP_LIB_PATH)/libsecurity.a $(PDCP_LIB_PATH)/librohc.a $(COMM_LIB_DIR)/libframework.a $(COMM_LOG_LIB_DIR)/liblogging.a $(COMM_LOG_LIB_DIR)/libalarm.a

PDCP_LD_FLAG= $(PDCP_LIB_PATH)/libpdcpuser.a $(PDCP_LIB_PATH)/libpdcp.a $(PDCP_LIB_PATH)/libintegrity.a $(PDCP_LIB_PATH)/libciphering.a $(PDCP_LIB_PATH)/libsecurity.a $(PDCP_LIB_PATH)/librohc.a 
ifeq ($(INTEL_T3300),intel_t3300_ca)
PDCP_LIBD_FLAG= $(PDCP_LIB_PATH)/libpdcp.a $(PDCP_LIB_PATH)/libsecurity.a $(PDCP_LIB_PATH)/libintegrity.a  $(PDCP_LIB_PATH)/librohc.a $(PDCP_LIB_PATH)/libciphering.a /usr/lib/libspaccdev.so
else
#SPR 19288 change start
#SPR 19713 change start
#change for 19288 condition removed
#SPR 19713 change end

PDCP_LIBD_FLAG= $(PDCP_LIB_PATH)/libpdcp.a $(PDCP_LIB_PATH)/libsecurity.a $(PDCP_LIB_PATH)/libintegrity.a  $(PDCP_LIB_PATH)/librohc.a $(PDCP_LIB_PATH)/libciphering.a
#SPR 19288 change end
endif
SYNC_LIB=
ifeq ($(LTE_EMBMS_FLAG),1)
    # SYNC is required only for eMBMS.
    SYNC_LIB += $(SYNC_DIR)/libsync.a
endif
#SPR 19288 change start
#SPR 19713 change start
#change for 19288 condition removed
#SPR 19713 change end

COMMON_LIB = -lm -lrt -lpthread -ldl
#SPR 19288 change end

ifeq ($(INTEL_T3300),intel_t3300_ca)
INCLUDE_LTE+= -I$(LTE_ROOT)/common/include/lte_port \
			  -I${MACCORE_ROOT}/include/common -I${MACCORE_ROOT}/include/scheduler/strategy \
			  -I${MACCORE_ROOT}/include/scheduler/dlSchManager \
			  -I${MACCORE_ROOT}/include/scheduler/ulSchManager -I${MACCORE_ROOT}/include/scheduler/bcchManager \
			  -I${MACCORE_ROOT}/include/scheduler/ccchManager -I${MACCORE_ROOT}/include/scheduler/rachManager \
			  -I${MACCORE_ROOT}/include/scheduler/pcchManager  -I${MACCORE_ROOT}/include/scheduler/resourceManager \
			  -I${MACCORE_ROOT}/include/scheduler/DRXManager \
			  -I${MACCORE_ROOT}/include/scheduler/MeasGapHandler \
			  -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/multiplexer \
			  -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/encoder \
			  -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/demultiplexer \
			  -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/dlHarq \
			  -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/ulHarq \
			  -I${MACCORE_ROOT}/include/interfaces/rrc \
			  -I${MACCORE_ROOT}/include/interfaces/rlc \
			  -I${MACCORE_ROOT}/include/interfaces/pdcp \
			  -I${MACCORE_ROOT}/include/interfaces/phy \
			  -I$(MACCORE_ROOT)/include/interfaces/phy/customer/mindspeed \
			  -I${MACCORE_ROOT}/include/interfaces/oam \
			  -I${MACCORE_ROOT}/include/executionScheduler \
			  -I${MACCORE_ROOT}/include/porting \
			  -I${LTE_ROOT}/gtpuPacketRelay/egtpu/stack/fsm/h \
			  -I${LTE_ROOT}/gtpuPacketRelay/egtpu/shared/h \
			  -I${LTE_ROOT}/gtpuPacketRelay/packetRelay/include 
else
INCLUDE_LTE+= -I$(LTE_ROOT)/common/include/lte_port \
			  -I${MACCORE_ROOT}/include/common -I${MACCORE_ROOT}/include/scheduler/strategy \
			  -I${MACCORE_ROOT}/include/scheduler/dlSchManager \
			  -I${MACCORE_ROOT}/include/scheduler/ulSchManager -I${MACCORE_ROOT}/include/scheduler/bcchManager \
			  -I${MACCORE_ROOT}/include/scheduler/ccchManager -I${MACCORE_ROOT}/include/scheduler/rachManager \
			  -I${MACCORE_ROOT}/include/scheduler/pcchManager  -I${MACCORE_ROOT}/include/scheduler/resourceManager \
			  -I${MACCORE_ROOT}/include/scheduler/DRXManager \
			  -I${MACCORE_ROOT}/include/scheduler/MeasGapHandler \
			  -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/multiplexer \
			  -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/encoder \
			  -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/demultiplexer \
			  -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/dlHarq \
			  -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/ulHarq \
			  -I${MACCORE_ROOT}/include/interfaces/rrc \
			  -I${MACCORE_ROOT}/include/interfaces/rlc \
			  -I${MACCORE_ROOT}/include/interfaces/pdcp \
              -I${MACCORE_ROOT}/include/interfaces/phy \
			  -I${MACCORE_ROOT}/include/interfaces/oam \
			  -I${MACCORE_ROOT}/include/interfaces/sync \
			  -I${MACCORE_ROOT}/include/executionScheduler \
			  -I${MACCORE_ROOT}/include/porting \
			  -I${LTE_ROOT}/gtpuPacketRelay/egtpu/stack/fsm/h \
			  -I${LTE_ROOT}/gtpuPacketRelay/egtpu/shared/h \
			  -I${LTE_ROOT}/gtpuPacketRelay/packetRelay/include 
endif

ifeq ($(LINUX_PC_TEST),true)
INCLUDE_LTE += -I${TEST_INC_DIR}
COMMON_LIB +=  -L$(TEST_LIB_DIR) -lTest -lgcov
CFLAGS += $(FLAGS) -DTEST_L2_BUILD
endif

pdcplayer:
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) -fpack-struct ${INCLUDE_PATH_PDCP} -o lteLayer2OAM.o $(LAYER2_OAM_SRC_DIR)/lteOAM.c
ifeq ($(INTEL_T3300),intel_t3300_ca)
	cd ${LAYER2_OAM_BIN_DIR} && $(CC)  ${MAC_LAYER_INC_PATH} -O2 -Wall  lteLayer2OAM.o -o $(TARGET) $(LD_FLAGS)  $(COMMON_LIB)
else
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) -O2 -Wall  lteLayer2OAM.o -o $(TARGET) $(LD_FLAGS)  $(COMMON_LIB)
endif

rlclayer:
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} -o lteLayer2OAM.o $(LAYER2_OAM_SRC_DIR)/lteOAM.c
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) -O2 -Wall $(SEARCH_PATH_RLC) lteLayer2OAM.o -o lteLayer2 -lrlc -lmacsim -lpdcp -lframework -llogging  -lalarm $(COMMON_LIB)

maclayer:
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} -o lteLayer2MacRlcPdcpInterface.o $(INTERFACES_SRC_DIR)/lteLayer2MacRlcPdcpInterface.c
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC} -o lteLayer2OAM.o $(LAYER2_OAM_SRC_DIR)/lteOAM.c
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) -O2 -Wall $(SEARCH_PATH_MAC) lteLayer2OAM.o lteLayer2MacRlcPdcpInterface.o -o lteLayer2 -lmaccore  $(COMMON_LIB) 

layer2:
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} -o lteLayer2MacRlcPdcpInterface.o $(INTERFACES_SRC_DIR)/lteLayer2MacRlcPdcpInterface.c
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} -o lteOAMIntegration.o $(LAYER2_OAM_SRC_DIR)/lteOAMIntegration.c 
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} $(INCLUDE_LTE) -o lteOAMKpiContainer.o $(LAYER2_OAM_SRC_DIR)/lteOAMKpiContainer.c
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) -O2 -Wall -xHost -ipo $(SEARCH_PATH_MAC_RLC) lteOAMIntegration.o lteOAMKpiContainer.o lteLayer2MacRlcPdcpInterface.o -o lteLayer2 -lmaccore $(PLUGSCH_LIB) -lrlc $(PDCP_LD_FLAG) -lframework -llogging -lalarm $(COMMON_LIB) $(SYNC_LIB)

			
ifeq ($(HOST_COMPILATION),0)
l2WithAdpt:
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} -o lteLayer2MacRlcPdcpInterface.o $(INTERFACES_SRC_DIR)/lteLayer2MacRlcPdcpInterface.c
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} ${INCLUDE_PATH_PDCP_ADAPTER} $(INCLUDE_LTE) -o lteOAMIntegration.o $(LAYER2_OAM_SRC_DIR)/lteOAMIntegration.c 
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} ${INCLUDE_PATH_PDCP_ADAPTER} $(INCLUDE_LTE) -o lteOAMKpiContainer.o $(LAYER2_OAM_SRC_DIR)/lteOAMKpiContainer.c
ifeq ($(INTEL_T3300),intel_t3300_ca)
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} $(INCLUDE_PATH_PDCP_ADAPTER) $(INCLUDE_LTE) -o  lteMacMspdApi.o $(LAYER2_OAM_SRC_DIR)/lteMacMspdApi.c
endif
#SPR 19288 change start
#SPR 19713 change start
#change for 19288 condition removed

	cd ${LAYER2_OAM_BIN_DIR} && $(LTE_MAKE_BIN) -O2 -Wall $(OPTIM_FLAGS) $(SEARCH_PATH_MAC_RLC) $(PL_LIB_PATH) $(PHYSIM_OBJ) lteOAMKpiContainer.o lteOAMIntegration.o lteLayer2MacRlcPdcpInterface.o -o lteLayer2 -lrlc $(PDCP_LIB_PATH)/libpdcp.a -lmaccore  $(PDCP_LIBD_FLAG) $(PDCP_ADAPTER_LIBD_FLAG) -lframework -llogging -lalarm $(COMMON_LIB) $(OAMAPP_PL_LIB)  -L$(RLC_LAYER_LIB_DIR) -lrlc 

#SPR 19713 change end
#SPR 19288 change end
ifeq ($(INTEL_T3300),intel_t3300_ca)
l2_split:
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} -o lteLayer2MacRlcPdcpInterface.o $(INTERFACES_SRC_DIR)/lteLayer2MacRlcPdcpInterface.c
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} ${INCLUDE_PATH_PDCP_ADAPTER} $(INCLUDE_LTE) -o lteOAMIntegration.o $(LAYER2_OAM_SRC_DIR)/lteOAMIntegration.c 
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} ${INCLUDE_PATH_PDCP_ADAPTER} $(INCLUDE_LTE) -o lteOAMKpiContainer.o $(LAYER2_OAM_SRC_DIR)/lteOAMKpiContainer.c
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} ${INCLUDE_PATH_PDCP_ADAPTER} $(INCLUDE_LTE) -o lteMacMspdApi.o $(LAYER2_OAM_SRC_DIR)/lteMacMspdApi.c
	cd ${LAYER2_OAM_BIN_DIR} && $(LTE_MAKE_BIN) -g -O2 -Wall $(OPTIM_FLAGS) \
	$(SEARCH_PATH_MAC_RLC) $(PHYSIM_OBJ) lteOAMKpiContainer.o lteOAMIntegration.o lteLayer2MacRlcPdcpInterface.o -o lteLayer2_mac_rlc -lrlc $(PDCP_LIB_PATH)/libpdcp.a -lmaccore  $(PDCP_LIBD_FLAG) -lframework -llogging -lalarm -liccserv $(COMMON_LIB) /usr/lib/libshm.so $(MSPD_ICPU_LIB_DIR)libtl.a lteMacMspdApi.o -fPIC -Wl,-lntl -lshm -Wl,-L ../../../bin 

pdcp_split:
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} ${INCLUDE_PATH_PDCP_ADAPTER} $(INCLUDE_LTE) -o lteMacMspdApi.o $(LAYER2_OAM_SRC_DIR)/lteMacMspdApi.c
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} -o lteLayer2MacRlcPdcpInterface.o $(INTERFACES_SRC_DIR)/lteLayer2MacRlcPdcpInterface.c
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} ${INCLUDE_PATH_PDCP_ADAPTER} $(INCLUDE_LTE) -o lteOAMIntegration.o $(LAYER2_OAM_SRC_DIR)/lteOAMIntegration.c 
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} ${INCLUDE_PATH_PDCP_ADAPTER} $(INCLUDE_LTE) -o lteOAMKpiContainer.o $(LAYER2_OAM_SRC_DIR)/lteOAMKpiContainer.c
	cd ${LAYER2_OAM_BIN_DIR} && $(LTE_MAKE_BIN) -g -O2 -Wall $(OPTIM_FLAGS) \
	$(SEARCH_PATH_MAC_RLC) $(PHYSIM_OBJ) lteOAMKpiContainer.o lteOAMIntegration.o lteLayer2MacRlcPdcpInterface.o -o lteLayer2_pdcp -lrelay -legtpu -lrlc -lmaccore $(PDCP_LIBD_FLAG) -lframework -llogging -lalarm -liccserv $(COMMON_LIB) /usr/lib/libshm.so $(MSPD_ICPU_LIB_DIR)libtl.a lteMacMspdApi.o -fPIC -Wl,-lntl -lshm -Wl,-L ../../../bin 
endif

else
ifeq ($(RLC_PDCP_SPLIT_FLAG), 1)
l2WithAdpt:	
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} -o lteLayer2MacRlcPdcpInterface.o $(INTERFACES_SRC_DIR)/lteLayer2MacRlcPdcpInterface.c
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} ${INCLUDE_PATH_PDCP_ADAPTER} $(INCLUDE_LTE) -o lteOAMIntegration.o $(LAYER2_OAM_SRC_DIR)/lteOAMIntegration.c
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} ${INCLUDE_PATH_PDCP_ADAPTER} $(INCLUDE_LTE) -o lteOAMKpiContainer.o $(LAYER2_OAM_SRC_DIR)/lteOAMKpiContainer.c
	cd ${LAYER2_OAM_BIN_DIR} && $(LTE_MAKE_BIN) -Wall $(OPTIM_FLAGS) \
$(SEARCH_PATH_MAC_RLC_GTPU) $(PHYSIM_OBJ) lteOAMKpiContainer.o lteOAMIntegration.o lteLayer2MacRlcPdcpInterface.o -o lteLayer2 -lrlc -lmaccore  -lframework -llogging -lalarm $(COMMON_LIB) 
else
ifeq ($(GTPU_SPLIT),0)
l2WithAdpt:
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} -o lteLayer2MacRlcPdcpInterface.o $(INTERFACES_SRC_DIR)/lteLayer2MacRlcPdcpInterface.c
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} ${INCLUDE_PATH_PDCP_ADAPTER} $(INCLUDE_LTE) -o lteOAMIntegration.o $(LAYER2_OAM_SRC_DIR)/lteOAMIntegration.c
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} ${INCLUDE_PATH_PDCP_ADAPTER} $(INCLUDE_LTE) -o lteOAMKpiContainer.o $(LAYER2_OAM_SRC_DIR)/lteOAMKpiContainer.c
	cd ${LAYER2_OAM_BIN_DIR} && $(LTE_MAKE_BIN) -O2 -Wall $(OPTIM_FLAGS) \
$(SEARCH_PATH_MAC_RLC) $(PHYSIM_OBJ) lteOAMKpiContainer.o lteOAMIntegration.o lteLayer2MacRlcPdcpInterface.o -o lteLayer2 $(SYNC_LIB) -lrlc $(PDCP_LIB_PATH)/libpdcp.a -lmaccore  $(PDCP_LIBD_FLAG) -lrelay -legtpu $(PDCP_ADAPTER_LIBD_FLAG) -lframework -llogging -lalarm $(COMMON_LIB)
else
l2WithAdpt:
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} -o lteLayer2MacRlcPdcpInterface.o $(INTERFACES_SRC_DIR)/lteLayer2MacRlcPdcpInterface.c
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} ${INCLUDE_PATH_PDCP_ADAPTER} $(INCLUDE_LTE) -o lteOAMIntegration.o $(LAYER2_OAM_SRC_DIR)/lteOAMIntegration.c
	cd ${LAYER2_OAM_BIN_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_MAC_RLC} ${INCLUDE_PATH_PDCP_ADAPTER} $(INCLUDE_LTE) -o lteOAMKpiContainer.o $(LAYER2_OAM_SRC_DIR)/lteOAMKpiContainer.c
	cd ${LAYER2_OAM_BIN_DIR} && $(LTE_MAKE_BIN) -O2 -Wall $(OPTIM_FLAGS) \
$(SEARCH_PATH_MAC_RLC_GTPU) $(PHYSIM_OBJ) lteOAMKpiContainer.o lteOAMIntegration.o lteLayer2MacRlcPdcpInterface.o -o lteLayer2 -lrlc $(PDCP_LIB_PATH)/libpdcp.a -lmaccore  $(PDCP_LIBD_FLAG)  $(PDCP_ADAPTER_LIBD_FLAG) -lframework -llogging -lalarm $(COMMON_LIB) 
endif
endif

endif

clean:
	${RM} $(LAYER2_OAM_BIN_DIR)/$(LAYER2_OAM_EXEC_NAME)
	${RM} $(LAYER2_OAM_BIN_DIR)/*.o
	$(RM) $(LAYER2_OAM_BIN_DIR)/../src/daemon.o


#cd ${LAYER2_OAM_BIN_DIR} && $(CC) -O3 -xHost -ipo -O2 -Wall $(FLAGS) -I${    INCLUDE} ${INCLUDE_PATH_MAC_RL    C} $(SEARCH_PATH_RLC) -o lteLayer2 $(LAYER2_OAM_SRC_DIR)/lteOAM.c -lrlc -lma    csim -lpdcp -lframework -lm -lrt


