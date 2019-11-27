#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: 
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
# *
# *
# ****************************************************************************/
##############################################################################
#
#		MAIN MAKE FILE						 
#	Make file to be used for building mac simulator for RLC. 
#						
##############################################################################

include ${LTE_ROOT}/make.inc
SIMULATORS_ROOT=$(LTE_ROOT)/simulators/

OAMRRC_BIN_DIR= $(SIMULATORS_ROOT)/oamrrc/bin/
OAMRRC_SRC_DIR= $(SIMULATORS_ROOT)/oamrrc/src/
OAMRRC_LIB_DIR= $(SIMULATORS_ROOT)/oamrrc/lib/

TARGET = $(OAMRRC_BIN_DIR)/oamSim
OAMRRC_ARCHIVE = liboamrrcsim.a

OAMRRC_INC_DIR= -I${SIMULATORS_ROOT}/oamrrc/include/ 
#				   -I$(LTE_ROOT)/pdcp/pdcpCore/include/\
				   -I$(LTE_ROOT)/pdcp/pdcpCommon/include/

INC_COM = -I $(LTE_ROOT)/common/include
INC_COM += -I $(LTE_ROOT)/common/include/lte_port/
COMM_INET_INCLUDE = -I${LTE_ROOT}/commonInterface/interfaces/include/

CFLAGS += -DUT_TESTING $(CTAGS) $(FLGS) 

LIBS_DIR+= -L$(COMMON_LIB_UESIM_DIR) -lframework -lpthread  -lrt
LFLAGS += $(LIBS_DIR) -lm  
INCLUDE_LTE += $(OAMRRC_INC_DIR) $(INC_COM) $(COMM_INET_INCLUDE) 

OAMRRC_OBJS = $(OAMRRC_LIB_DIR)/lteSimPdcpCallbacks.o\
					  $(OAMRRC_LIB_DIR)/lteSimPdcpCallbacksSockets.o\
					  $(OAMRRC_LIB_DIR)/lteSimPdcpCallFunc.o\
					  $(OAMRRC_LIB_DIR)/lteSimPdcpOamRrcSimulatorMain.o\
					  $(OAMRRC_LIB_DIR)/lteSimPdcpSimuScheduler.o\
					  $(OAMRRC_LIB_DIR)/lteSimPdcpSockTools.o\
					  $(OAMRRC_LIB_DIR)/lteSimPdcpTestStatistic.o\
					  $(OAMRRC_LIB_DIR)/ltePdcpCmnTools.o\
					  $(OAMRRC_LIB_DIR)/lteSimPdcpUserUtils.o


OBJ_LOC= $(OAMRRC_OBJS) 

create_directory : 
	${MKD} $(OAMRRC_LIB_DIR) 

oamRrcLib: ${OBJ_LOC}
	for i in ${OBJ_LOC}; \
		do \
			$(AR) -ur $(OAMRRC_LIB_DIR)/$(OAMRRC_ARCHIVE) $$i ; \
			done

oamRrcExe: create_directory $(OBJ_LOC)
	$(MAKE_BIN) $(OBJ_LOC) -o $(TARGET) $(LFLAGS) $(CTAGS) $(FLGS) 

oamClientExe:
	gcc $(OAMRRC_SRC_DIR)/lteSimOamClientApp.c -o $(OAMRRC_BIN_DIR)/clientApp -Wall -O2


$(OAMRRC_LIB_DIR)/%.o: $(OAMRRC_SRC_DIR)/%.c
	$(COMPILE) $(CFLAGS) $(CTAGS) $(FLGS) $(INCLUDE_LTE) -c $< -o $@


clean:
	rm -f $(OAMRRC_LIB_DIR)/*.o $(OAMRRC_LIB_DIR)/*.a
	${RMD} $(OAMRRC_LIB_DIR)
	rm -f $(TARGET)
	rm -f $(OAMRRC_BIN_DIR)/clientApp

$(warning LTE_ROOT = $(LTE_ROOT))
$(warning SIMULATORS_ROOT = $(SIMULATORS_ROOT))
$(warning OAMRRC_LIB_DIR = $(OAMRRC_LIB_DIR))
$(warning OAMRRC_ARCHIVE = $(OAMRRC_ARCHIVE))
$(warning COMPILE = $(COMPILE))
$(warning LFLAGS = $(LFLAGS))
