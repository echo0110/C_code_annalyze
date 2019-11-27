###################################################################
# 
#  FILE NAME   : lterrc_s1apOamim.mk 
# 
#  DESCRIPTION : The RRC OAMIM module make file 
# 
#  REVISION HISTORY : 
# 
#  DATE          Name        Reference        Comments 
#  ----          ----        ---------        -------- 
#
#  Copyright (c) 2009, Aricent Inc. 
# 
###################################################################

S1AP_OAMIM_INCLUDES = -I${RRC_ROOT}/common/s1ap_oamim/include -I$(RRC_ROOT)/s1ap/include/ 

S1AP_OAMIM_SRCDIR = ${RRC_ROOT}/common/s1ap_oamim/src 

S1AP_OAMIM_SRCFILES = rrc_s1apOam_il_composer.c \
		      rrc_s1apOam_il_parser.c \
		      rrc_s1apCommon_il_composer.c \
		      rrc_s1apCommon_il_parser.c

S1AP_OAMIM_OBJFILES:=$(S1AP_OAMIM_SRCFILES)
S1AP_OAMIM_OBJFILES:=$(patsubst %.c,$(OBJDIR)/%.o,$(S1AP_OAMIM_OBJFILES))

S1AP_OAMIM_DEPFILES:=$(S1AP_OAMIM_SRCFILES)
S1AP_OAMIM_DEPFILES:=$(patsubst %.c,$(OBJDIR)/%.d,$(S1AP_OAMIM_DEPFILES))

S1AP_OAMIM_LIB=$(LIBDIR)/librrcS1apOamim.a

s1apOamim: $(MODE) $(S1AP_OAMIM_LIB) 

$(S1AP_OAMIM_OBJFILES) : lterrc_s1apOamim.mk

$(S1AP_OAMIM_LIB): $(S1AP_OAMIM_OBJFILES)
	@echo '[$(S1AP_OAMIM_LIB)]'
	$(RRC_AR) $(RRC_ARFLAGS) $(S1AP_OAMIM_LIB) $(S1AP_OAMIM_OBJFILES)
	$(RRC_RANLIB) $(S1AP_OAMIM_LIB)
	@echo

ifneq ($(strip $(S1AP_OAMIM_DEPFILES)),)
-include $(S1AP_OAMIM_DEPFILES)
endif

.PHONY : s1apOamim 

#add unit data to global scope variables
VPATH += $(S1AP_OAMIM_SRCDIR)
RRC_OBJFILES += $(S1AP_OAMIM_OBJFILES)
RRC_INCLUDES += $(S1AP_OAMIM_INCLUDES)
RRC_LIBS += $(S1AP_OAMIM_LIB)
