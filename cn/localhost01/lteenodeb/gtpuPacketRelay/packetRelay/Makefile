#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: Makefile,v 1.4.16.1 2010/11/01 15:30:02 gur20056 Exp $
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
# * Revision 1.4.16.1  2010/11/01 15:30:02  gur20056
# * mwc_demo renamed to layer3 related changes
# *
# * Revision 1.4  2010/03/10 09:27:36  gur19479
# * updated for cleanup
# *
# * Revision 1.3  2010/03/10 09:25:43  gur19479
# * updated for relay flag
# *
# * Revision 1.2  2010/03/08 14:31:06  gur19479
# * updated for RELAY STACK
# *
# * Revision 1.1.1.1  2010/02/11 04:51:36  cm_intel
# * eNB framework for intel
# *
# * Revision 1.1.2.3  2009/12/06 07:50:12  gur19479
# * updated for cross compilation
# *
# * Revision 1.1.2.2  2009/12/05 14:53:44  gur19479
# * updated for cross compilation
# *
# * Revision 1.1.2.1  2009/11/25 09:57:08  cm_lteenodeb
# * code checkin
# *
# *
# ****************************************************************************/
##############################################################################
#
#       MAIN MAKE FILE
#   Make file to be used for building Packet Relay.
#
##############################################################################

include ${LTE_ROOT}/make.inc
PACKET_RELAY_ROOT=$(LTE_ROOT)/gtpuPacketRelay/packetRelay

PACKET_RELAY_BIN_DIR=$(PACKET_RELAY_ROOT)/bin
PACKET_RELAY_SRC_DIR=$(PACKET_RELAY_ROOT)/src
PACKET_RELAY_LIB_DIR=$(PACKET_RELAY_ROOT)/lib

PACKET_RELAY_INC_DIR= -I${PACKET_RELAY_ROOT}/include/
SEC_ENGINE_INC_DIR= -I${LTE_ROOT}/pdcp/securityEngine/include/
TARGET = $(PACKET_RELAY_BIN_DIR)/packetRelay
INC_COM = -I $(EGTPU_ROOT)/stack/fsm/h/
INC_COM += -I $(EGTPU_ROOT)/stack/common/h/
INC_COM += -I $(EGTPU_ROOT)/shared/h/
INC_COM += -I $(LTE_ROOT)/common/include/
INC_COM += -I $(LTE_ROOT)/common/include/lte_port/
INC_COM += -I $(LTE_ROOT)/sync/include/
#INC_COM += -I $(LTE_ROOT)/simulators/adapter/pdcp/include/
INC_COM += -I $(LTE_ROOT)/pdcp/pdcpCore/include/
INC_COM += -I $(LTE_ROOT)/pdcp/pdcpCommon/include/
#SPR 2030
INC_COM += -I $(LTE_ROOT)/common/loggingFrameWork/logging/inc/
INC_COM += -I $(LTE_ROOT)/common/loggingFrameWork/alarm/inc/
INC_COM += -I $(LTE_ROOT)/common/loggingFrameWork/common/inc/
INC_COM += -I $(LTE_ROOT)/commonInterface/interfaces/include/
INC_COM += -I $(LTE_ROOT)/distributer/include/

LFLAGS += $(LIBS_DIR) -lm -lrt
INCLUDE += $(PACKET_RELAY_INC_DIR) $(INC_COM) $(SEC_ENGINE_INC_DIR) $(INCLUDE_LTE)

ifeq ($(ENDC_ENABLE),1)
PACKET_RELAY_OBJS = $(PACKET_RELAY_BIN_DIR)/lteRelayAdapter.o\
					     $(PACKET_RELAY_BIN_DIR)/lteRelay.o\
					     $(PACKET_RELAY_BIN_DIR)/lteRelayCommon.o\
					     $(PACKET_RELAY_BIN_DIR)/lteRelayGtpuSap.o\
					     $(PACKET_RELAY_BIN_DIR)/lteRelayOamSap.o\
					     $(PACKET_RELAY_BIN_DIR)/lteRelayS1Sap.o\
                         $(PACKET_RELAY_BIN_DIR)/lteX2uRelay.o\
                         $(PACKET_RELAY_BIN_DIR)/lteRelayX2uSap.o
else
PACKET_RELAY_OBJS = $(PACKET_RELAY_BIN_DIR)/lteRelayAdapter.o\
					     $(PACKET_RELAY_BIN_DIR)/lteRelay.o\
					     $(PACKET_RELAY_BIN_DIR)/lteRelayCommon.o\
					     $(PACKET_RELAY_BIN_DIR)/lteRelayGtpuSap.o\
					     $(PACKET_RELAY_BIN_DIR)/lteRelayOamSap.o\
					     $(PACKET_RELAY_BIN_DIR)/lteRelayS1Sap.o
endif
ifeq ($(LTE_EMBMS_FLAG),1)
PACKET_RELAY_OBJS += $(PACKET_RELAY_BIN_DIR)/lteRelayM1uTeidSearchTree.o
PACKET_RELAY_OBJS += $(PACKET_RELAY_BIN_DIR)/lteRelayM2Sap.o
endif	
OBJ_LOC= $(PACKET_RELAY_OBJS) 
#SPR 2298 - changed Archieve option from -q to -r
lib: $(OBJ_LOC)
	$(LTE_AR) -r $(PACKET_RELAY_BIN_DIR)/librelay.a $(OBJ_LOC) 

$(PACKET_RELAY_BIN_DIR)/%.o: $(PACKET_RELAY_SRC_DIR)/%.c
	#Make the directory only once
	${MKD} $(PACKET_RELAY_BIN_DIR)
	$(LTE_CC) $(CFLAGS) $(CTAGS) $(INCLUDE) -c $< -o $@ 

clean:
	${RM} $(PACKET_RELAY_BIN_DIR)/*.o
	${RM} $(TARGET)
	${RM} $(PACKET_RELAY_BIN_DIR)/librelay.a
	${RMD} $(PACKET_RELAY_BIN_DIR)

$(warning LTE_ROOT = $(LTE_ROOT))
$(warning PACKET_RELAY_LIB_DIR = $(PACKET_RELAY_LIB_DIR))
$(warning COMPILE = $(COMPILE))
$(warning LFLAGS = $(LFLAGS))
