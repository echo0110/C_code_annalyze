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
# * Revision 1.2.2.2  2009/08/27 11:23:04  gur19140
# * *** empty log message ***
# *
# * Revision 1.2.2.1  2009/06/09 11:29:18  gur19140
# * first wave of Intergration
# *
# * Revision 1.2  2009/05/13 09:01:39  gur19140
# * *** empty log message ***
# *
# * Revision 1.1  2009/05/12 14:44:56  gur19479
# * *** empty log message ***
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

PDCP_ARCHIVE=libintegrity.a

PDCP_INC_DIR= ${PDCP_ROOT}/openSource/include

PDCP_SRC_DIR= ${PDCP_ROOT}/openSource/src

PDCP_LIB_DIR= ${PDCP_ROOT}/lib

PDCP_INCLUDE=-I$(PDCP_INC_DIR) -I${PDCP_ROOT}/integrity/include -I$(PDCP_ROOT)/pdcpCommon/include -I$(PDCP_ROOT)/pdcpCore/include 

INCLUDE_LTE+= -I$(COMMON_ROOT)/include/lte_port $(COMMON_INER_INCLUDE) $(PDCP_INCLUDE)
ifeq ($(LTE_CC), arm-linux-gnueabi-gcc)
INCLUDE_LTE += -I$(ARM_INCLUDE)
endif

PDCP_SRCS= $(wildcard $(PDCP_SRC_DIR)/*.c)
PDCP_OBJS=$(patsubst %.c,%.o,$(PDCP_SRCS))
PDCP_LIB_OBJS=$(patsubst %.c,$(PDCP_LIB_DIR)/%.o,$(notdir $(PDCP_SRCS)))

# changes for SPR 1757 start
CFLAGS += $(FLAGS)
# changes for SPR 1757 end

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
