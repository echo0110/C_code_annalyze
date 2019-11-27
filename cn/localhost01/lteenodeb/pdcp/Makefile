#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: Makefile,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
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
# * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
# * eNB framework for intel
# *
# * Revision 1.2.2.2  2009/06/19 13:47:05  gur19140
# * *** empty log message ***
# *
# * Revision 1.2.2.1  2009/06/09 12:01:32  gur19140
# * first wave of Integration
# *
# * Revision 1.2  2009/05/12 13:45:03  gur19479
# * *** empty log message ***
# *
# * Revision 1.4  2008/12/30 05:15:21  gur20294
# * RLC_ROOT is added to (Makefile make.inc) to make LTE RLC layer.
# *
# * Revision 1.3  2008/09/09 06:22:23  gur11974
# * Updated changes for Integration
# *
# * Revision 1.2  2008/09/05 08:16:24  ssinghal
# * Makefile changes
# *
# *
# *
# *
# ****************************************************************************/
##############################################################################
#
#		MAIN MAKE FILE						 
#	Make file to be used for building Layer 2. 
#						
##############################################################################
include ${LTE_ROOT}/make.inc

ROHC_ROOT= $(PDCP_ROOT)/rohc

COMMON_SUBDIRS = \
		${PDCP_ROOT}/executionScheduler \
		${PDCP_ROOT}/pdcpCore \
		${PDCP_ROOT}/pdcpCommon \
		${PDCP_ROOT}/integrity \
        ${PDCP_ROOT}/ciphering \
		${PDCP_ROOT}/rohc \
		${PDCP_ROOT}/openSource \
		${PDCP_ROOT}/securityEngine
#		${PDCP_ROOT}/simulator/pdcpuser \


#		${PDCP_ROOT}/simulator/rlc \


ifneq ($(HOST_COMPILATION), 1)
export RLC_PDCP_SPLIT_FLAG=0
endif

ifeq ($(RLC_PDCP_SPLIT_FLAG), 0)

.DEFAULT_GOAL:= debug

all debug release local cross tests common: COMMON_MODULES

CFLAGS += $(FLAGS) 
COMMON_MODULES:
	${MKD} ${PDCP_ROOT}/lib
	set -e;
	for i in $(COMMON_SUBDIRS);\
	do \
	echo "Building in $$i"; \
	if ! $(MAKE) target -C $$i; \
	then exit 1; \
	fi; \
	done

else

PDCP_BINARY_NAME     = lte_pdcp
PDCP_BIN_DIR         = ${LTE_ROOT}/../bin
COMMON_INTERFACE     = ${LTE_ROOT}/commonInterface/OAMApp
COMMON_INTERFACE_BIN = ${COMMON_INTERFACE}/bin
COMMON_INTERFACE_SRC = ${COMMON_INTERFACE}/src

COMM_LIB_DIR         = ${LTE_ROOT}/common/lib
COMM_LOG_LIB_DIR     = ${LTE_ROOT}/common/loggingFrameWork/lib/
INTERFACES_SRC_DIR   = ${LTE_ROOT}/commonInterface/interfaces/src/
INTERFACES_INC_DIR   = -I${LTE_ROOT}/commonInterface/interfaces/include/

INCLUDE_LTE += -I${LTE_ROOT}/commonInterface/interfaces/include/ -I${LTE_ROOT}/commonInterface/OAMApp/include -I${LTE_ROOT}/common/include/lte_port

ifeq ($(LTE_CC), arm-linux-gnueabi-gcc)
INCLUDE_LTE += -I$(ARM_INCLUDE)
endif

PDCP_BIN_DIR = ${LTE_ROOT}/../bin/

COMMON_LIB = -lm -lrt -lpthread

#Variables for Packet Relay
PDCP_LIB_DIR = ${LTE_ROOT}/pdcp/lib

DPDK_FRAMEWORK_PATH = ${LTE_ROOT}/dpdk_framework/
DPDK_PDCP_LIB_DIR   = ${LTE_ROOT}/dpdk_framework/common/mainlink/enode_lib/
DPDK_FRAMEWORK_BIN  = ${LTE_ROOT}/dpdk_framework/common/mainlink/build/

.DEFAULT_GOAL:= pdcp 
pdcp:  COMMON_MODULES

ifeq ($(LINUX_PC_TEST),true)
TEST_DEP_MAKE +=  -L$(TEST_LIB_DIR) -lTest
endif

CFLAGS += $(FLAGS) 
COMMON_MODULES:
	${MKD} ${PDCP_ROOT}/lib
	set -e;
	for i in $(COMMON_SUBDIRS);\
	do \
	echo "Building in $$i"; \
	if ! $(MAKE) target -C $$i; \
	then exit 1; \
	fi; \
	done
ifneq ($(DPDK_ENABLED), 1)
	cd ${PDCP_BIN_DIR} && $(CC) $(CFLAGS) $(INCLUDE_LTE) ${INTERFACES_INC_DIR} -o $(COMMON_INTERFACE_BIN)/lteLayer2MacRlcPdcpInterface.o $(INTERFACES_SRC_DIR)/lteLayer2MacRlcPdcpInterface.c
	cd ${PDCP_BIN_DIR} && $(LTE_MAKE_BIN) -O2 -Wall $(OPTIM_FLAGS) \
-L$(PDCP_LIB_DIR) -L$(COMM_LIB_DIR) -L$(COMM_LOG_LIB_DIR) $(PL_LIB_PATH) $(PDCP_LIB_DIR)/ltePdcpCmnTools.o $(COMMON_INTERFACE_BIN)/lteLayer2MacRlcPdcpInterface.o -o lte_pdcp -lpdcp -lciphering -lrohc -lintegrity -lsecurity  -lframework -llogging -lalarm $(COMMON_LIB) $(OAMAPP_PL_LIB) $(TEST_DEP_MAKE)
else
	cd ${PDCP_BIN_DIR} && $(CC) $(CFLAGS) $(INCLUDE_LTE) ${INTERFACES_INC_DIR} -o $(COMMON_INTERFACE_BIN)/lteLayer2MacRlcPdcpInterface.o $(INTERFACES_SRC_DIR)/lteLayer2MacRlcPdcpInterface.c;
	echo "****************** Now making Single PDCP Application Library ****************"
	cp ${LTE_ROOT}/common/loggingFrameWork/lib/libalarm.a $(PDCP_LIB_DIR)
	cp ${LTE_ROOT}/common/loggingFrameWork/lib/liblogging.a $(PDCP_LIB_DIR)
	cp ${LTE_ROOT}/common/lib/libframework.a $(PDCP_LIB_DIR)
	rm -rf $(PDCP_LIB_DIR)/tempDIR
	mkdir $(PDCP_LIB_DIR)/tempDIR 
	cp $(COMMON_INTERFACE_BIN)/lteLayer2MacRlcPdcpInterface.o  $(PDCP_LIB_DIR)/tempDIR
	cp  $(PDCP_LIB_DIR)/*.a $(PDCP_LIB_DIR)/tempDIR/

	cd $(PDCP_LIB_DIR)/tempDIR/ && ar xv libpdcp.a
	cd $(PDCP_LIB_DIR)/tempDIR/ && ar xv libciphering.a
	cd $(PDCP_LIB_DIR)/tempDIR/ && ar xv librohc.a
	cd $(PDCP_LIB_DIR)/tempDIR/ && ar xv libintegrity.a
	cd $(PDCP_LIB_DIR)/tempDIR/ && ar xv libsecurity.a
	cd $(PDCP_LIB_DIR)/tempDIR/ && ar xv libframework.a
	cd $(PDCP_LIB_DIR)/tempDIR/ && ar xv liblogging.a
	cd $(PDCP_LIB_DIR)/tempDIR/ && ar xv libalarm.a
	cd $(PDCP_LIB_DIR)/tempDIR/ && ar rv libPdcp.a *.o  
	cp $(PDCP_LIB_DIR)/tempDIR/libPdcp.a $(DPDK_PDCP_LIB_DIR)
	echo "***************************** libPdcp.a created ******************************"
endif

endif
    
##################################################
# Rules for Cleaning the object/library archives #    
##################################################


clean:
	${RMD} $(PDCP_ROOT)/lib
	for i in $(COMMON_SUBDIRS);\
	do \
	echo "Cleaning in $$i"; \
	if ! $(MAKE_CLEAN) clean -C $$i; \
	then exit 1; \
	fi; \
	done

# Remove PDCP binary, if PDCP is compiled as independent binary
ifeq ($(RLC_PDCP_SPLIT_FLAG), 1)
	${RM} $(PDCP_BIN_DIR)/$(PDCP_BINARY_NAME)
endif

