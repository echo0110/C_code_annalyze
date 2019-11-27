#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (C) 2011 Aricent Inc . All Rights Reserved.
# *
# ****************************************************************************
# *
# ****************************************************************************
# *
# *  File Description : Makefile
# *
# ****************************************************************************
# *
# ****************************************************************************/
##############################################################################
#
#		MAIN MAKE FILE						 
#	Make file to be used for building security engine. 
#						
##############################################################################

include ${LTE_ROOT}/make.inc

PDCP_ARCHIVE=libsecurity.a

PDCP_INC_DIR= ${PDCP_ROOT}/securityEngine/include

PDCP_SRC_DIR= ${PDCP_ROOT}/securityEngine/src

PDCP_LIB_DIR= ${PDCP_ROOT}/lib

PDCP_INCLUDE=-I$(PDCP_INC_DIR) -I${PDCP_ROOT}/openSource/include -I$(PDCP_ROOT)/pdcpCommon/include -I$(PDCP_ROOT)/pdcpCore/include -I${PDCP_ROOT}/integrity/include -I${PDCP_ROOT}/ciphering/include 

INCLUDE_LTE += -I$(COMMON_ROOT)/include/lte_port $(COMMON_INER_INCLUDE) $(PDCP_INCLUDE)
ifeq ($(LTE_CC), arm-linux-gnueabi-gcc)
INCLUDE_LTE += -I$(ARM_INCLUDE)
endif

PDCP_SRCS= $(wildcard $(PDCP_SRC_DIR)/*.c)
PDCP_OBJS=$(patsubst %.c,%.o,$(PDCP_SRCS))
PDCP_LIB_OBJS=$(patsubst %.c,$(PDCP_LIB_DIR)/%.o,$(notdir $(PDCP_SRCS)))

CFLAGS += $(FLAGS)

target: $(PDCP_LIB_OBJS) 
			$(AR) ur $(PDCP_LIB_DIR)/$(PDCP_ARCHIVE) $?  
$(PDCP_LIB_DIR)/%.o : $(PDCP_SRC_DIR)/%.c 
			${COMPILE} -c $< -o $@  
clean:
	${RM} $(PDCP_LIB_DIR)/*.o
	${RM} $(PDCP_LIB_DIR)/$(PDCP_ARCHIVE)

$(warning LTE_ROOT = $(LTE_ROOT))
$(warning PDCP_ROOT = $(PDCP_ROOT))
$(warning PDCP_LIB_DIR = $(PDCP_LIB_DIR))
$(warning PDCP_ARCHIVE = $(PDCP_ARCHIVE))
$(warning COMPILE = $(COMPILE))
