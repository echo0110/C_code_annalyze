#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (C) 2009 Aricent Inc . All Rights Reserved.
# *
# ****************************************************************************
# *
# *  $Id: Makefile,v 1.2 2010/02/13 10:37:42 gur15697 Exp $
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
# * Revision 1.2  2010/02/13 10:37:42  gur15697
# * PDCP_LATENCY_TEST is undefined
# *
# * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
# * eNB framework for intel
# *
# * Revision 1.6.2.2.2.1  2009/12/06 07:06:25  gur19479
# * updated to remove compilation issue on cavium
# *
# * Revision 1.6.2.2  2009/08/27 11:22:48  gur19140
# * *** empty log message ***
# *
# * Revision 1.6.2.1  2009/06/09 11:27:18  gur19140
# * first wave of Integration
# *
# * Revision 1.6  2009/05/27 04:51:19  gur19479
# * modified for rohc timers
# *
# * Revision 1.5  2009/05/15 09:05:19  gur19140
# * bug fixes
# *
# * Revision 1.4  2009/05/14 11:04:45  gur19836
# * CR Comments Incorporated
# *
# * Revision 1.3  2009/05/13 09:01:39  gur19140
# * *** empty log message ***
# *
# * Revision 1.2  2009/05/12 13:44:42  gur19479
# * pdcp merging
# *
# * Revision 1.3  2009/04/02 11:06:31  gur20439
# * Modified to get compilation flag from command line.
# *
# * Revision 1.2  2009/03/23 08:37:17  gur19140
# * Integration changes first waves
# *
# * Revision 1.1  2008/12/30 06:24:49  gur20294
# * PDCP Layer added. INITIAL DRAFT
# *
# *
# *
# ****************************************************************************/
##############################################################################
#
#		MAIN MAKE FILE						 
#	Make file to be used for building PDCP. 
#						
##############################################################################

include ${LTE_ROOT}/make.inc

PDCP_ARCHIVE=libpdcp.a

PDCP_INC_DIR= ${PDCP_ROOT}/executionScheduler/include

PDCP_SRC_DIR= ${PDCP_ROOT}/executionScheduler/src

PDCP_LIB_DIR= ${PDCP_ROOT}/lib

PDCP_INCLUDE=-I$(PDCP_INC_DIR) -I${PDCP_ROOT}/pdcpCommon/include -I$(PDCP_ROOT)/pdcpCore/include -I$(PDCP_ROOT)/simulator/rlc/include \
	-I$(PDCP_ROOT)/simulator/pdcpuser/include -I$(PDCP_ROOT)/rohc/include/common/ 

PDCP_INCLUDE += -I${PDCP_ROOT}/securityEngine/include/

INCLUDE_LTE+= -I$(COMMON_ROOT)/include/lte_port -I${LTE_ROOT}/commonInterface/interfaces/include $(COMMON_INER_INCLUDE) $(PDCP_INCLUDE)

ifeq ($(LTE_CC), arm-linux-gnueabi-gcc)
INCLUDE_LTE += -I$(ARM_INCLUDE)
endif

BIN_SRCS= $(PDCP_SRC_DIR)/pdcp_test.c
BIN_PROM= $(BIN_SRCS:.c=)
PDCP_SRCS= $(wildcard $(PDCP_SRC_DIR)/*.c)
PDCP_OBJS=$(patsubst %.c,%.o,$(PDCP_SRCS))
PDCP_LIB_OBJS=$(patsubst %.c,$(PDCP_LIB_DIR)/%.o,$(notdir $(PDCP_SRCS)))

CFLAGS += $(FLAGS)  
target: $(PDCP_LIB_OBJS) 
			$(AR) ur $(PDCP_LIB_DIR)/$(PDCP_ARCHIVE) $?  
$(PDCP_LIB_DIR)/%.o : $(PDCP_SRC_DIR)/%.c 
			${COMPILE} -c $< -o $@
$(BIN_PROM):$(BIN_SRCS)/%.c  
			${COMPILE} $< -o $@
clean:
	${RM} $(PDCP_LIB_DIR)/*.o
	${RM} $(PDCP_LIB_DIR)/$(PDCP_ARCHIVE)

$(warning LTE_ROOT = $(LTE_ROOT))
$(warning PDCP_ROOT = $(PDCP_ROOT))
$(warning PDCP_LIB_DIR = $(PDCP_LIB_DIR))
$(warning PDCP_ARCHIVE = $(PDCP_ARCHIVE))
$(warning COMPILE = $(COMPILE))
