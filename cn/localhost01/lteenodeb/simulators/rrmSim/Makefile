#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: Makefile
# *
# ****************************************************************************
# *
# *  File Description : Makefile
# *
##############################################################################
#
#		MAIN MAKE FILE						 
#	Make file to be used for building RRM simulator for MAC.
#
#
##############################################################################

include ${LTE_ROOT}/make.inc
RRMSIM_INC_DIR =  -I$(LTE_ROOT)/simulators/rrmSim/include/ \
                  -I$(LTE_ROOT)/common/include/ \
				  -I$(LTE_ROOT)/mac/maccore/include/interfaces/rrm/ \
				  -I$(LTE_ROOT)/mac/maccore/include/common/ \
				  -I$(LTE_ROOT)/pdcp/pdcpCore/include/ \
				  -I$(LTE_ROOT)/common/loggingFrameWork/logging/inc/ \
				  -I$(LTE_ROOT)/common/loggingFrameWork/alarm/inc/ \
				  -I$(LTE_ROOT)/common/loggingFrameWork/common/inc/ \
				  -I$(LTE_ROOT)/common/include/lte_port/ \
				  -I$(LTE_ROOT)/commonInterface/interfaces/include/\
				  -I$(LTE_ROOT)/mac/maccore/include/scheduler/ulSchManager/\
				  -I$(LTE_ROOT)/mac/maccore/include/interfaces/phy/\
				  -I$(LTE_ROOT)/mac/maccore/include/interfaces/rlc/\
				  -I$(LTE_ROOT)/mac/maccore/include/scheduler/dlSchManager/\
				  -I$(LTE_ROOT)/mac/maccore/include/interfaces/rrc/\
				  -I$(LTE_ROOT)/mac/maccore/include/scheduler/DRXManager/\
				  -I$(LTE_ROOT)/mac/maccore/include/scheduler/MeasGapHandler/\
				  -I$(LTE_ROOT)/mac/maccore/include/scheduler/resourceManager/\
                  -I$(LTE_ROOT)/mac/maccore/include/porting/\
                  -I$(LTE_ROOT)/mac/maccore/include/scheduler/ECIDManager/

RRM: lteRrmSim.o
	gcc -g -o bin/RRM lteRrmSim.o  -lrt
lteRrmSim.o: src/lteRrmSim.c
	gcc -g -c -Wall src/lteRrmSim.c $(RRMSIM_INC_DIR) $(CFLAGS)
clean:
	rm -f *.o include/*.a
	rm -f bin/RRM
