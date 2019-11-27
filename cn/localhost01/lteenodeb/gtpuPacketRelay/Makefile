#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: Makefile $
# *
# ****************************************************************************
# *
# *  File Description : Makefile for GTP process
# *
# ****************************************************************************
# *
# * Revision Details
# * ----------------
# *
# ****************************************************************************/
##############################################################################
#
#		MAIN MAKE FILE						 
#	Make file to be used for building GTP. 
#						
##############################################################################
include ${LTE_ROOT}/make.inc

ifeq ($(GTPU_SPLIT),1)
INCLUDE_LTE += -I$(COMMON_ROOT)/include/lte_port \
			  -I${LTE_ROOT}/gtpuPacketRelay/egtpu/stack/fsm/h \
			  -I${LTE_ROOT}/gtpuPacketRelay/egtpu/shared/h \
			  -I${LTE_ROOT}/gtpuPacketRelay/packetRelay/include 

INTERFACES_SRC_DIR = ${LTE_ROOT}/commonInterface/interfaces/src/
INTERFACES_INC_DIR = -I${LTE_ROOT}/commonInterface/interfaces/include/

GTPU_SRC_DIR = ${LTE_ROOT}/gtpuPacketRelay
GTP_BIN_DIR = ${LTE_ROOT}/../bin/
COMMON_INTERFACE_BIN = ${LTE_ROOT}/commonInterface/OAMApp/bin/

#Variables for Packet Relay
PACKET_RELAY_LIB_DIR = ${LTE_ROOT}/gtpuPacketRelay/packetRelay/bin
#Variables for eGTPU
eGTPU_LIB_DIR            = ${LTE_ROOT}/gtpuPacketRelay/egtpu/stack/
#Variables for Common
COMM_LIB_DIR         = ${LTE_ROOT}/common/lib
COMM_LOG_LIB_DIR     = ${LTE_ROOT}/common/loggingFrameWork/lib/

CFLAGS += -DGTP_SPLIT

SEARCH_LIB_PATH = -L$(PACKET_RELAY_LIB_DIR) -L$(eGTPU_LIB_DIR) -L$(COMM_LIB_DIR) -L$(COMM_LOG_LIB_DIR)

COMMON_LIB = -lm -lrt -lpthread
BRDCOM_LIB = -lprcl1 -lprcl1_ns -lprc_commons

ifeq ($(LINUX_PC_TEST),true)
INCLUDE_LTE += -I${TEST_INC_DIR}
COMMON_LIB +=  -L$(TEST_LIB_DIR) -lTest
CFLAGS += $(FLAGS) -DTEST_L2_BUILD
endif

gtpu:
	cd ${GTP_BIN_DIR} && $(CC) $(CFLAGS) $(INCLUDE_LTE) ${INTERFACES_INC_DIR} -o $(COMMON_INTERFACE_BIN)/lteLayer2MacRlcPdcpInterface.o $(INTERFACES_SRC_DIR)/lteLayer2MacRlcPdcpInterface.c
	cd ${GTP_BIN_DIR} && $(CC) $(CFLAGS) $(INCLUDE_LTE) -o ${LTE_ROOT}/gtpuPacketRelay/lte_gtpu.o $(GTPU_SRC_DIR)/gtpuMain.c
	cd ${GTP_BIN_DIR} && $(LTE_MAKE_BIN) -O2 -Wall $(OPTIM_FLAGS) \
$(SEARCH_LIB_PATH) $(PL_LIB_PATH) $(COMMON_INTERFACE_BIN)/lteLayer2MacRlcPdcpInterface.o ${LTE_ROOT}/gtpuPacketRelay/lte_gtpu.o -o lte_gtpu -lrelay -legtpu -lframework -llogging -lalarm $(COMMON_LIB) $(OAMAPP_PL_LIB)

clean:
	${RM} ${LTE_ROOT}/gtpuPacketRelay/*.o
	${RM} ${GTP_BIN_DIR}/lte_gtpu
endif	
