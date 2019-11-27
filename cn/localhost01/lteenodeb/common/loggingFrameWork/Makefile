###################################################################
#
#  FILE NAME   : lterrc_rrmim.mk
#
#  DESCRIPTION : The RRC RRMIM module make file
#
#  REVISION HISTORY :
#
#  DATE          Name        Reference        Comments
#  ----          ----        ---------        --------
#
#  Copyright (c) 2009, Aricent Inc.
#
###################################################################
export LOG_FRWK_DIR=$(shell pwd)
include ${LTE_ROOT}/make.inc

#SPR 17884 changes - CFLAGS line deleted
COMPILE= $(LTE_CC) -c 
LINK= $(LTE_CC)
AR= $(LTE_AR) rv

# To debug sample application, enable the following macros.
# These macros need to be enabled in respective makefiles of stacks
# while integrating the log and alarm framework

#CFLAGS+= -DLOG_PRINT_ENABLED -DSHM_PRINT_ENABLED -DALM_FRWK

#################################################################
# Logging FrameWork Include Directories
##################################################################    
LOG_FRWK_INCLUDES = -I$(LOG_FRWK_DIR)/logging/inc \
                    -I$(LOG_FRWK_DIR)/sample/inc \
                    -I$(LOG_FRWK_DIR)/signalHandler/inc \
                    -I$(LOG_FRWK_DIR)/reader/inc \
                    -I$(LOG_FRWK_DIR)/common/inc \
                    -I$(LOG_FRWK_DIR)/alarm/inc \
                    -I${LTE_ROOT}/common/include \
                    -I${LTE_ROOT}/common/include/lte_port \
                    -I${LTE_ROOT}/commonInterface/interfaces/include \
                    -I${LTE_ROOT}/mac/maccore/include/common \
                    -I$(LTE_ROOT)/mac/maccore/include/interfaces/phy
#SPR 17884 changes LOG_FRWK_INCLUDES added
LOG_FRWK_INCLUDES += ${INCLUDE_LTE}
#################################################################
# Logging FrameWork Source Directories
##################################################################    
LOGGER_SOURCE=$(LOG_FRWK_DIR)/logging/src
READER_SOURCE=${LOG_FRWK_DIR}/reader/src
SIGNAL_HANDLER_SOURCE=${LOG_FRWK_DIR}/signalHandler/src
SAMPLE_SOURCE=${LOG_FRWK_DIR}/sample/src
COMMON_SOURCE=${LOG_FRWK_DIR}/common/src
 =${LOG_FRWK_DIR}/alarm/src

#################################################################
# Logging FrameWork Libraries
##################################################################    
ALARM_LIBS= libalarm.a
LOGGING_LIBS= liblogging.a
READER_LIBS= libreader.a


ifeq ($(LINUX_PC_TEST),true)
INCLUDE_LTE += -I${TEST_INC_DIR}
COMMON_LIB +=  -L$(TEST_LIB_DIR) -lTest -lgcov
CFLAGS += $(FLAGS) -DTEST_L2_BUILD
endif

ifeq ($(LTE_CC), arm-linux-gnueabi-gcc)
COMMON_LIB += $(ARM_LIB)
endif
#################################################################
#  Makefile: RULES
##################################################################    
all: create_dir log_make alarm_make signal_handler_make ltebinread
		mv *.o $(LOG_FRWK_DIR)/bin
		mv *.a $(LOG_FRWK_DIR)/lib

utils: create_dir reader_make ltecat ltetail logArrayVerify sample_make ltebinread
		mv *.o $(LOG_FRWK_DIR)/bin
		mv *.a $(LOG_FRWK_DIR)/lib

clean:  bin_clean lib_clean

#################################################################
#  all: sub rules
##################################################################    
create_dir:
	${MKD} $(LOG_FRWK_DIR)/bin
	${MKD} $(LOG_FRWK_DIR)/lib
	
log_make: $(LOG_FRWK_DIR)/../lib/lteMisc.o
	@echo Building liblogging.a
	$(COMPILE) $(COMMON_LIB) $(CFLAGS) $(LOG_FRWK_INCLUDES) $(LOGGER_SOURCE)/logging.c
	$(COMPILE) $(COMMON_LIB) $(CFLAGS) $(LOG_FRWK_INCLUDES) $(LOGGER_SOURCE)/logarray.c
	$(COMPILE) $(COMMON_LIB) $(CFLAGS) $(LOG_FRWK_INCLUDES) $(READER_SOURCE)/log_fapi_msg_decoder.c
	$(AR) $(LOGGING_LIBS) logging.o logarray.o log_fapi_msg_decoder.o $(LOG_FRWK_DIR)/../lib/lteMisc.o
	@echo

$(LOG_FRWK_DIR)/../lib/lteMisc.o:
	make -C $(LOG_FRWK_DIR)/../  

alarm_make:
	@echo Building libalarm.a
	$(COMPILE) $(COMMON_LIB) $(CFLAGS) $(LOG_FRWK_INCLUDES) $(COMMON_SOURCE)/socket_utils.c
	$(COMPILE) $(COMMON_LIB) $(CFLAGS) $(LOG_FRWK_INCLUDES) $(COMMON_SOURCE)/shm_utils.c
	$(COMPILE) $(COMMON_LIB) $(CFLAGS) $(LOG_FRWK_INCLUDES) $(ALARM_SOURCE)/alarm.c
	$(AR) $(ALARM_LIBS) socket_utils.o shm_utils.o alarm.o
	@echo

signal_handler_make:
	$(COMPILE) $(CFLAGS) $(LOG_FRWK_INCLUDES) $(SIGNAL_HANDLER_SOURCE)/signal_handler.c
	$(AR) libsignalhand.a signal_handler.o
	@echo


#################################################################
#  utils: sub rules
##################################################################    
reader_make:log_make
	@echo Building libreader.a
	$(COMPILE) $(COMMON_LIB) $(CFLAGS) $(LOG_FRWK_INCLUDES) $(READER_SOURCE)/reader.c
	$(COMPILE) $(COMMON_LIB) $(CFLAGS) $(LOG_FRWK_INCLUDES) $(LOGGER_SOURCE)/logarray.c
	$(COMPILE) $(COMMON_LIB) $(CFLAGS) $(LOG_FRWK_INCLUDES) $(READER_SOURCE)/log_fapi_msg_decoder.c
	$(AR) $(READER_LIBS) reader.o logarray.o log_fapi_msg_decoder.o $(LOG_FRWK_DIR)/../lib/lteMisc.o
#SPR 3576: CFLAGS removed while linking
ltecat:	reader_make
	$(LINK) $(LOG_FRWK_INCLUDES) $(COMMON_LIB) $(READER_SOURCE)/ltecat.c $(READER_LIBS) -lrt -lpthread -lm $(COMMON_LIB) -o $(LOG_FRWK_DIR)/bin/ltecat

ltetail: reader_make
	$(LINK) $(LOG_FRWK_INCLUDES) $(READER_SOURCE)/ltetail.c $(READER_LIBS) -lrt -lpthread -lm $(COMMON_LIB) -o $(LOG_FRWK_DIR)/bin/ltetail

logArrayVerify: reader_make
	$(LINK) $(LOG_FRWK_INCLUDES) $(READER_SOURCE)/logArrayVerify.c $(READER_LIBS) -lrt -lpthread -lm $(COMMON_LIB) -o $(LOG_FRWK_DIR)/bin/logArrayVerify

ltebinread: reader_make
	$(LINK) $(LOG_FRWK_INCLUDES) $(READER_SOURCE)/ltebinread.c $(READER_LIBS) -lrt  -lpthread -lm  $(COMMON_LIB) -o $(LOG_FRWK_DIR)/bin/ltebinread

sample_make: log_make alarm_make
	$(COMPILE) $(CFLAGS) $(LOG_FRWK_INCLUDES) $(SAMPLE_SOURCE)/sample.c
	$(LINK) $(LOG_FRWK_INCLUDES) sample.o $(ALARM_LIBS) $(LOGGING_LIBS) -lrt -lpthread -lm $(COMMON_LIB) -o $(LOG_FRWK_DIR)/bin/sample.exe
#################################################################
#  clean: sub rules
##################################################################    
bin_clean:
	rm -rf $(LOG_FRWK_DIR)/bin/*;
	${RMD} $(LOG_FRWK_DIR)/bin

lib_clean:
	rm -rf $(LOG_FRWK_DIR)/lib/*;
	rm -rf $(LOG_FRWK_DIR)/../lib/lteMisc.o
	${RMD} $(LOG_FRWK_DIR)/lib
	
