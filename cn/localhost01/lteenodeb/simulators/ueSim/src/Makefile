#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: Makefile,v 1.1.1.1.12.2.2.2 2010/10/27 10:47:26 gur20056 Exp $
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
# * Revision 1.1.1.1.12.2.2.2  2010/10/27 10:47:26  gur20056
# * changes for compiling default with O2 flag
# *
# * Revision 1.1.1.1.12.2.2.1  2010/09/21 16:09:21  gur20491
# * FAPI changes
# *
# * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
# * eNB framework for intel
# *
# ****************************************************************************/
##############################################################################
#
#		MAIN MAKE FILE						 
#	Make file to be used for building UESim. 
#						
##############################################################################
include ${LTE_ROOT}/make.inc
UE_SIM_BIN=ueSim
#SPR 21653 changes start#
MYFLAGS =  $(OPTIM_FLAGS) -UTHREAD_BIND_INTEL_CYC_CNT -UUT_TESTING -DTHREAD_BIND_ENABLE1 -D_GNU_SOURCE  -UTEMP_FIX -DMSG_BYTE_ORDER_MSB -UEXECUTION_LEG_PERFORMANCE -DTIME_PERF -DUE_SIM_TESTING -UNON_FAPI_LEGACY_INF -DHO_SETUP_CHANGES -D_FORTIFY_SOURCE=0 -UUE_SIM_SPLIT -UUESIM_PDCP_THREAD -UDL_UL_SPLIT_TDD
#SPR 21653 changes end#
ifeq ($(LTE_EMBMS_FLAG),1)
MYFLAGS += -DLTE_EMBMS_SUPPORTED
endif
UESIM_SRC_DIR= ${LTE_ROOT}/simulators/ueSim/src
UESIM_BIN_DIR=${LTE_ROOT}/simulators/ueSim/bin
UESIM_C_FILES = $(UESIM_SRC_DIR)/lteUeExecutionScheduler.c
                

UESIM_OBJS= ${UESIM_BIN_DIR}/lteUeExecutionScheduler.o

UESIM_INCLUDE= ${INCLUDE_LTE} -I$(COMMON_ROOT)/include/lte_port \
        -I${LTE_ROOT}/simulators/ueSim/include/ -I${LTE_ROOT}/simulators/macEmulator/include/common/  \
        -I${LTE_ROOT}/simulators/macEmulator/include/interfaces/ \
        -I${LTE_ROOT}/commonInterface/interfaces/include/ \
        -I${LTE_ROOT}/simulators/adapter/pdcp/include \
#SPR 2030
UESIM_LIBS_WITH_ADPT=-L/${LTE_ROOT}/common/lib_uesim -L${LTE_ROOT}/simulators/macEmulator/lib/ -L${LTE_ROOT}/rlc/rlccore/lib_uesim/ -L${LTE_ROOT}/pdcpUesim/lib/ -L${LTE_ROOT}/simulators/adapter/pdcp/lib_uesim/ -L$(LTE_ROOT)/common/loggingFrameWork/lib/ #-L$(LTE_ROOT)/gtpuPacketRelay/packetRelay/bin/  -L$(LTE_ROOT)/gtpuPacketRelay/egtpu/stack/

UESIM_INC_LIBS_WITH_ADPT= -lrlc -lmacEmulator -lpdcpAdapter -lpdcp -lintegrity -lrohc -lframework -lrt -lm  -lpthread -lciphering -llogging
target: UE_SIM_BIN 

uesimWithAdapter:
	$(CC) -O2 -Wall $(MYFLAGS) $(UESIM_INCLUDE) $(UESIM_C_FILES) $(UESIM_LIBS_WITH_ADPT) $(UESIM_INC_LIBS_WITH_ADPT) ${LTE_ROOT}/commonInterface/OAMApp/bin/lteLayer2MacRlcPdcpInterface.o -o ${UESIM_BIN_DIR}/${UE_SIM_BIN}
#$(CC)  -g -Wall $(MYFLAGS) $(UESIM_INCLUDE) $(UESIM_C_FILES) $(UESIM_LIBS_WITH_ADPT) $(UESIM_INC_LIBS_WITH_ADPT) ${LTE_ROOT}/commonInterface/OAMApp/bin/lteLayer2MacRlcPdcpInterface.o -o ${UESIM_BIN_DIR}/${UE_SIM_BIN}

clean:
	${RM} $(UESIM_OBJS)
	${RM} $(UESIM_BIN_DIR)/$(UE_SIM_BIN)

#-------------------- Some rules. -------------#
${UESIM_BIN_DIR}/%.o : ${UESIM_SRC_DIR}/%.c
	${COMPILE} $< -c -O2 -o $@
