#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: Makefile,v 1.1.1.1.16.1 2012/01/05 15:53:14 gur22059 Exp $
# *
# ****************************************************************************
# *
# *  File Description : Makefile
# *
# ****************************************************************************
# *
# * Revision Details
# * ----------------
# ****************************************************************************/
##############################################################################
#
#       MAIN MAKE FILE
#   Make file to be used for building PHY Sim.
#
##############################################################################
include ${LTE_ROOT}/make.inc

COMMON_ROOT = ${LTE_ROOT}/common
INCLUDE_PATH_PHY = -I$(COMMON_ROOT)/include -I$(COMMON_ROOT)/include/lte_port\
                   -I${LTE_ROOT}/commonInterface/interfaces/include\
                   -I${LTE_ROOT}/simulators/macEmulator/include/interfaces\
                   -I${LTE_ROOT}/simulators/macEmulator/include/common\
				   -I${LTE_ROOT}/common/include \
				   -I${LTE_ROOT}/common/loggingFrameWork/logging/inc

ENB_PHY_SEARCH_PATH = -L${LTE_ROOT}/common/lib
ENB_PHY_SIM_BIN_DIR = ${LTE_ROOT}/simulators/enbPhySim/bin
ENB_PHY_SIM_SRC_DIR = ${LTE_ROOT}/simulators/enbPhySim/src
ENB_PHY_SIM_INCLUDE_DIR = ${LTE_ROOT}/simulators/enbPhySim/include

l2PhySim :
	cd ${ENB_PHY_SIM_BIN_DIR} && $(CC) -c -O2 ${INCLUDE_PATH_PHY} -I$(ENB_PHY_SIM_INCLUDE_DIR) -D_GNU_SOURCE -DLAYER2_PHYSIM_THREAD -o lteEnbPhySim.o $(ENB_PHY_SIM_SRC_DIR)/lteEnbPhySim.c
#	cd ${ENB_PHY_SIM_BIN_DIR} && $(CC) -g3 -Wall $(ENB_PHY_SEARCH_PATH) lteEnbPhySim.o -o l2PhySim -lframework -lpthread -lrt



clean:
	rm -rf bin/*.o
	rm -rf bin/l2PhySim
