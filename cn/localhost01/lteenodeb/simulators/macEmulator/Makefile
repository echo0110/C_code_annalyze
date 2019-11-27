#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: Makefile,v 1.1.1.1.16.1 2010/09/21 15:53:14 gur20491 Exp $
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
# * Revision 1.1.1.1.16.1  2010/09/21 15:53:14  gur20491
# * FAPI changes
# *
# * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
# * eNB framework for intel
# *
# * Revision 1.1.4.3  2009/08/27 11:24:04  gur19140
# * *** empty log message ***
# *
# * Revision 1.1.4.2  2009/06/10 12:18:04  gur19836
# * Functional Interface changes with PHY Sim
# *
# * Revision 1.1.4.1  2009/06/09 12:07:13  gur19140
# * first wave of Integration
# *
# * Revision 1.4  2009/06/01 11:34:37  gur20439
# * modified for 1.2 release.
# *
# * Revision 1.3  2009/06/01 10:51:30  gur20439
# * makefile changed to make macemulator+physimulator as combined exe.
# *
# * Revision 1.11  2009/05/26 07:25:48  gur20439
# * Makefile
# *
# * Revision 1.9  2009/02/17 13:44:31  gur10445
# * Ul Traffic Gen
# *
# * Revision 1.8  2009/01/20 16:25:09  gur11912
# * Updated for Porting
# *
# * Revision 1.7  2009/01/19 15:45:33  gur18569
# * name changes for RLC interfaces
# *
# * Revision 1.6  2009/01/19 14:19:33  gur18569
# * removed testing flags
# *
# * Revision 1.5  2009/01/19 14:16:18  gur18569
# * mac/simulator/phy/src/phySimulator.c
# *
# * Revision 1.4  2009/01/15 10:44:57  gur18569
# * removed -static and -lpthread option
# *
# * Revision 1.3  2008/12/12 05:12:39  gur19413
# * Updated for Phase 2
# *
# * Revision 1.2  2008/09/19 11:15:24  gur11912
# * Modified for Phy Simulator
# *
# * Revision 1.1  2008/09/05 08:21:54  ssinghal
# * Makefile added
# *
# *
# *
# *
# ****************************************************************************/
##############################################################################
#
#		MAIN MAKE FILE						 
#	Make file to be used for building PHY. 
#						
##############################################################################
include ${LTE_ROOT}/make.inc

#Variables for  RLC Sim
RLC_SIM_HOME           = ${LTE_ROOT}/mac/simulator/rlc
RLC_SIM_LIB_DIR        = ${RLC_SIM_HOME}/lib
RLC_SIM_INCLUDE_DIR    = ${RLC_SIM_HOME}/include

#Variables for  MAC Emulator
MAC_EMU_HOME           = ${LTE_ROOT}/simulators/macEmulator
MAC_EMU_SRC_DIR    =  ${MAC_EMU_HOME}/src
MAC_EMU_LIB_DIR        = ${MAC_EMU_HOME}/lib
MAC_EMU_INCLUDE_DIR    = ${MAC_EMU_HOME}/include
MAC_EMU_LAYER_INC_PATH	= $(MAC_EMU_INCLUDE_DIR)/common \
								-I$(MAC_EMU_INCLUDE_DIR)/scheduler \
								-I$(MAC_EMU_INCLUDE_DIR)/harq \
								-I$(MAC_EMU_INCLUDE_DIR)/demux \
								-I$(MAC_EMU_INCLUDE_DIR)/interfaces
COMM_FRAMEWORK_LIB_DIR       = ${LTE_ROOT}/common/lib_uesim
HARQ_LIB_DIR= ${LTE_ROOT}/simulators/macEmulator/lib/harq
INTF_LIB_DIR= ${LTE_ROOT}/simulators/macEmulator/lib/interfaces
SCHED_LIB_DIR= ${LTE_ROOT}/simulators/macEmulator/lib/scheduler
COMM_LIB_DIR= ${LTE_ROOT}/simulators/macEmulator/lib/common
DEMUX_LIB_DIR=${LTE_ROOT}/simulators/macEmulator/lib/demux

INTERFACES_INC_DIR = ${LTE_ROOT}/commonInterface/interfaces/include/
PHY_MACEMU_SIM_EXEC_PATH=${LTE_ROOT}/simulators/macEmulator/bin
EXE_FILES_C = $(MAC_EMU_SRC_DIR)/interfaces/lteLayer2RLCInterface.c \
	      $(MAC_EMU_SRC_DIR)/interfaces/lteLayer2RRCInterface.c \
	      $(MAC_EMU_SRC_DIR)/interfaces/lteLayer2InitMac.c \
	      $(MAC_EMU_SRC_DIR)/interfaces/lteLayer2ParseUtil.c \
	      $(MAC_EMU_SRC_DIR)/interfaces/lteLayer2Uplink.c \
	      $(MAC_EMU_SRC_DIR)/interfaces/lteLayer2OAM.c \
	      $(MAC_EMU_SRC_DIR)/common/lteLayer2UEContext.c \
              $(MAC_EMU_SRC_DIR)/common/lteLayer2TBSize.c \
              $(MAC_EMU_SRC_DIR)/demux/lteLayer2ControllerThread.c \
              $(MAC_EMU_SRC_DIR)/interfaces/phySimulatorApiHandler.c

ifneq '$(findstring -DLTE_EMBMS_SUPPORTED,$(CFLAGS))' ''
EXE_FILES_C+=$(MAC_EMU_SRC_DIR)/interfaces//lteLayer2EmbmsInterface.c
endif			  

PHY_MACEMU_SIM_EXEC_NAME=phySim

MACEMU_EXE_FILES_C = $(MAC_EMU_SRC_DIR)/interfaces/lteLayer2RLCInterface.c \
	      $(MAC_EMU_SRC_DIR)/interfaces/lteLayer2RRCInterface.c \
	      $(MAC_EMU_SRC_DIR)/interfaces/lteLayer2InitMac.c \
	      $(MAC_EMU_SRC_DIR)/interfaces/lteLayer2ParseUtil.c \
	      $(MAC_EMU_SRC_DIR)/interfaces/lteLayer2Uplink.c \
	      $(MAC_EMU_SRC_DIR)/common/lteLayer2UEContext.c \
              $(MAC_EMU_SRC_DIR)/common/lteLayer2TBSize.c \
              $(MAC_EMU_SRC_DIR)/demux/lteLayer2ControllerThread.c \
              $(MAC_EMU_SRC_DIR)/interfaces/lteLayer2TDDMgr.c \
              $(MAC_EMU_SRC_DIR)/interfaces/lteFapiControlMsgHandler.c \
              $(MAC_EMU_SRC_DIR)/interfaces/lteFapiPhySimulatorApiHandler.c \
              $(MAC_EMU_SRC_DIR)/interfaces/phySimulatorApiHandler.c

ifneq '$(findstring -DLTE_EMBMS_SUPPORTED,$(CFLAGS))' ''
MACEMU_EXE_FILES_C+=$(MAC_EMU_SRC_DIR)/interfaces/lteLayer2EmbmsInterface.c
endif
MACEMU_SIM_LIB_NAME=libmacEmulator.a

OBJS= \
    $(MAC_EMU_LIB_DIR)/lteLayer2UEContext.o $(MAC_EMU_LIB_DIR)/lteLayer2UplinkTransmitter.o\
    $(MAC_EMU_LIB_DIR)/lteLayer2RRCInterface.o $(MAC_EMU_LIB_DIR)/lteLayer2RLCInterface.o \
    $(MAC_EMU_LIB_DIR)/lteLayer2InitMac.o $(MAC_EMU_LIB_DIR)/lteLayer2ParseUtil.o \
    $(MAC_EMU_LIB_DIR)/lteLayer2Uplink.o $(MAC_EMU_LIB_DIR)/lteLayer2TBSize.o \
    $(MAC_EMU_LIB_DIR)/lteLayer2PhysInterface.o $(MAC_EMU_LIB_DIR)/lteLayer2OAM.o \
    $(MAC_EMU_LIB_DIR)/lteLayer2ControllerThread.o \
    $(MAC_EMU_LIB_DIR)/phySimulatorApiHandler.o 

ifneq '$(findstring -DLTE_EMBMS_SUPPORTED,$(CFLAGS))' ''
OBJS+=$(MAC_EMU_LIB_DIR)/lteLayer2EmbmsInterface.o
endif

OBJS_MACEMU= \
    $(MAC_EMU_LIB_DIR)/lteLayer2UEContext.o \
    $(MAC_EMU_LIB_DIR)/lteLayer2RRCInterface.o $(MAC_EMU_LIB_DIR)/lteLayer2RLCInterface.o \
    $(MAC_EMU_LIB_DIR)/lteLayer2InitMac.o $(MAC_EMU_LIB_DIR)/lteLayer2ParseUtil.o \
    $(MAC_EMU_LIB_DIR)/lteLayer2Uplink.o $(MAC_EMU_LIB_DIR)/lteLayer2TBSize.o \
    $(MAC_EMU_LIB_DIR)/lteLayer2ControllerThread.o \
    $(MAC_EMU_LIB_DIR)/lteLayer2TDDMgr.o \
    $(MAC_EMU_LIB_DIR)/lteFapiControlMsgHandler.o \
    $(MAC_EMU_LIB_DIR)/lteFapiPhySimulatorApiHandler.o \
    $(MAC_EMU_LIB_DIR)/phySimulatorApiHandler.o

ifneq '$(findstring -DLTE_EMBMS_SUPPORTED,$(CFLAGS))' ''
OBJS_MACEMU+=$(MAC_EMU_LIB_DIR)/lteLayer2EmbmsInterface.o
endif
INCLUDE_PATH_PHY = -I$(COMMON_ROOT)/include -I$(COMMON_ROOT)/include/lte_port -I$(INTERFACES_INC_DIR) -I$(RLC_SIM_INCLUDE_DIR) -I$(MAC_EMU_LAYER_INC_PATH) 

SEARCH_PATH_MAC_EMU_RLC     = -L/usr/lib -L$(COMM_FRAMEWORK_LIB_DIR) -L$(RLC_SIM_LIB_DIR)     
target: PHY_SIM_EXEC

CFLAGS += $(FLAGS) -DHO_SETUP_CHANGES -DUE_SIM_TESTING -UUE_SIM_SPLIT
      
PHY_SIM_EXEC:
	cd ${MAC_EMU_LIB_DIR} && gcc -c -g -Wall -xHost -O3 -ipo -no-prec-div -vec-guard-write -opt-multi-version-aggressive \
             -opt-calloc -opt-subscript-in-range -fno-alias -fomit-frame-pointer \
                   -funroll-loops -complex-limited-range -vec-report2 -fp-model fast=2 \
                         -no-prec-sqrt -fast-transcendentals -rcd -ftz -Zp16 -align -restrict -falign-functions=16 -UMSGQ_INTF -UFUNC_INTF ${INCLUDE_PATH_PHY} \
        ${EXE_FILES_C} 
	cd ${MAC_EMU_LIB_DIR} && gcc ${SEARCH_PATH_MAC_EMU_RLC} ${OBJS} -o ${PHY_MACEMU_SIM_EXEC_PATH}/${PHY_MACEMU_SIM_EXEC_NAME} \
        -lrlc -lframework -lm -lrt

macEmuLib:
	cd ${MAC_EMU_LIB_DIR} && $(CC) $(CFLAGS) ${INCLUDE_PATH_PHY} ${MACEMU_EXE_FILES_C}
	cd ${MAC_EMU_LIB_DIR} && ${AR} -ur ${MACEMU_SIM_LIB_NAME} ${OBJS_MACEMU} 
        
clean:
	${RM} ${OBJS}
	${RM} $(PHY_MACEMU_SIM_EXEC_PATH)/$(PHY_MACEMU_SIM_EXEC_NAME)
	${RM} $(MAC_EMU_LIB_DIR)/*.o
	$(RM) $(MAC_EMU_LIB_DIR)/$(MACEMU_SIM_LIB_NAME)

