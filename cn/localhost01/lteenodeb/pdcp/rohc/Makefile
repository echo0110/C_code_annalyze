
#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: Makefile,v 1.1.1.1.16.1 2010/11/03 06:02:46 gur22059 Exp $
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
# * Revision 1.1.1.1.16.1  2010/11/03 06:02:46  gur22059
# * ROHC Merge for Profile-0 and 2
# *
# * Revision 1.7.8.2  2010/10/13 05:28:00  gur22059
# * Updated for haeder files path
# *
# * Revision 1.7.8.1  2010/08/19 09:33:45  gur22059
# * SPR 575: Added ROHC_MODE_TRIGGER & ROHC_UT_TESTING flags
# *
# * Revision 1.7  2010/07/09 04:45:07  gur21145
# * Adding HIGH_PERFORMANCE flag
# *
# * Revision 1.6  2010/06/25 06:28:40  gur21145
# * .
# *
# * Revision 1.5  2010/06/24 11:21:07  gur21145
# * modified makefile
# *
# * Revision 1.4  2009/05/13 09:01:39  gur19140
# * *** empty log message ***
# *
# * Revision 1.3  2009/05/12 14:07:29  gur19479
# * *** empty log message ***
# *
# *
# ****************************************************************************/
##############################################################################
#
#		MAIN MAKE FILE						 
#	Make file to be used for building ROHC. 
#						
##############################################################################
#
include ${LTE_ROOT}/make.inc

ROHC_ARCHIVE=librohc.a

ROHC_ROOT= $(PDCP_ROOT)/rohc

ROHC_INC_DIR= $(ROHC_ROOT)/include

ROHC_SRC_DIR= $(ROHC_ROOT)/src

ROHC_OBJ_DIR= $(ROHC_ROOT)/obj

ROHC_MODULE_LIST = M(ed) M(led) M(common) M(ppm) M(prm) M(cpm) M(pbm) M(ext_if) M(crcm) M(db) M(main) M(pedcm) 

ROHC_INCLUDE=$(patsubst M(%),-I$(ROHC_INC_DIR)/%,$(ROHC_MODULE_LIST))

INCLUDE_LTE+= -I$(COMMON_ROOT)/include/lte_port -I$(LTE_ROOT)/pdcp/pdcpCore/include  -I$(LTE_ROOT)/pdcp/pdcpCommon/include $(COMMON_INER_INCLUDE) $(ROHC_INCLUDE)
ifeq ($(LTE_CC), arm-linux-gnueabi-gcc)
INCLUDE_LTE += -I$(ARM_INCLUDE)
endif

ROHC_SRCS= $(wildcard $(patsubst M(%),$(ROHC_SRC_DIR)/%/*.c,$(ROHC_MODULE_LIST)))
ROHC_OBJS=$(patsubst %.c,%.o,$(ROHC_SRCS))
ROHC_LIB_OBJS=$(patsubst %.c,$(ROHC_OBJ_DIR)/%.o,$(notdir $(ROHC_SRCS)))

# changes for SPR 1757 start
# Flag ROHC_DEBUG_TRACE_ENABLE used for enabling PDCP rohc traces on L2
CFLAGS += $(FLAGS) -UROHC_MODE_TRIGGER -UROHC_UT_FLAG
# changes for SPR 1757 end

target: create_directory make_objs

make_objs: $(ROHC_LIB_OBJS) 
			$(AR) -ur $(PDCP_ROOT)/lib/$(ROHC_ARCHIVE) $(ROHC_LIB_OBJS)

create_directory :
	${MKD} $(ROHC_OBJ_DIR)

$(ROHC_OBJ_DIR)/%.o : $(ROHC_SRC_DIR)/common/%.c 
			${COMPILE} -c $< -o $@  
$(ROHC_OBJ_DIR)/%.o : $(ROHC_SRC_DIR)/led/%.c 
			${COMPILE} -c $< -o $@  
$(ROHC_OBJ_DIR)/%.o : $(ROHC_SRC_DIR)/crcm/%.c 
			${COMPILE} -c $< -o $@  
$(ROHC_OBJ_DIR)/%.o : $(ROHC_SRC_DIR)/db/%.c 
			${COMPILE} -c $< -o $@  
$(ROHC_OBJ_DIR)/%.o : $(ROHC_SRC_DIR)/pedcm/%.c 
			${COMPILE} -c $< -o $@
$(ROHC_OBJ_DIR)/%.o : $(ROHC_SRC_DIR)/ed/%.c
			${COMPILE} -c $< -o $@  
$(ROHC_OBJ_DIR)/%.o : $(ROHC_SRC_DIR)/ext_if/%.c   
			${COMPILE} -c $< -o $@  
$(ROHC_OBJ_DIR)/%.o : $(ROHC_SRC_DIR)/ppm/%.c      
			${COMPILE} -c $< -o $@  
$(ROHC_OBJ_DIR)/%.o : $(ROHC_SRC_DIR)/pbm/%.c    
			${COMPILE} -c $< -o $@  
$(ROHC_OBJ_DIR)/%.o : $(ROHC_SRC_DIR)/cpm/%.c  
			${COMPILE} -c $< -o $@ 
$(ROHC_OBJ_DIR)/%.o : $(ROHC_SRC_DIR)/main/%.c
			${COMPILE} -c $< -o $@
$(ROHC_OBJ_DIR)/%.o : $(ROHC_SRC_DIR)/prm/%.c  
			${COMPILE} -c $< -o $@     
 
 
clean:
	${RM} $(ROHC_OBJ_DIR)/*.o
	${RMD} $(ROHC_OBJ_DIR)
	${RM} ${PDCP_ROOT}/lib/$(ROHC_ARCHIVE)

$(warning LTE_ROOT = $(LTE_ROOT))
$(warning ROHC_ROOT = $(ROHC_ROOT))
$(warning ROHC_OBJ_DIR = $(ROHC_OBJ_DIR))
$(warning ROHC_ARCHIVE = $(ROHC_ARCHIVE))
$(warning COMPILE = $(COMPILE))
