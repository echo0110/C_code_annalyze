###################################################################
# 
#  FILE NAME   : lterrc_s1apRrmim.mk 
# 
#  DESCRIPTION : The RRC RRMIM module make file 
# 
#  REVISION HISTORY : 
# 
#  DATE          Name        Reference        Comments 
#  ----          ----        ---------        -------- 
#
#  Copyright (c) 2011, Aricent Inc. 
# 
###################################################################

S1AP_RRMIM_INCLUDES = -I${RRC_ROOT}/common/include/multi

S1AP_RRMIM_INCLUDES += -I${RRC_ROOT}/common/s1ap_rrmim/include \
					  -I${RRC_ROOT}/rrc/common/include

S1AP_RRMIM_SRCDIR = ${RRC_ROOT}/common/s1ap_rrmim/src

S1AP_RRMIM_SRCFILES = rrc_s1apRrm_il_composer.c \
				 rrc_s1apRrm_il_parser.c

S1AP_RRMIM_OBJFILES:=$(S1AP_RRMIM_SRCFILES)
S1AP_RRMIM_OBJFILES:=$(patsubst %.c,$(OBJDIR)/%.o,$(S1AP_RRMIM_OBJFILES))

S1AP_RRMIM_DEPFILES:=$(S1AP_RRMIM_SRCFILES)
S1AP_RRMIM_DEPFILES:=$(patsubst %.c,$(OBJDIR)/%.d,$(S1AP_RRMIM_DEPFILES))

S1AP_RRMIM_LIB=$(LIBDIR)/librrcS1apRrmim.a

s1apRrmim: $(MODE) $(S1AP_RRMIM_LIB) 

$(S1AP_RRMIM_OBJFILES) : lterrc_s1apRrmim.mk

$(S1AP_RRMIM_LIB): $(S1AP_RRMIM_OBJFILES)
	@echo '[$(S1AP_RRMIM_LIB)]'
	$(RRC_AR) $(RRC_ARFLAGS) $(S1AP_RRMIM_LIB) $(S1AP_RRMIM_OBJFILES)
	$(RRC_RANLIB) $(S1AP_RRMIM_LIB)
	@echo

ifneq ($(strip $(S1AP_RRMIM_DEPFILES)),)
-include $(S1AP_RRMIM_DEPFILES)
endif

.PHONY : s1apRrmim 

#add unit data to global scope variables
VPATH += $(S1AP_RRMIM_SRCDIR)
RRC_OBJFILES += $(S1AP_RRMIM_OBJFILES)
RRC_INCLUDES += $(S1AP_RRMIM_INCLUDES)
RRC_LIBS += $(S1AP_RRMIM_LIB)
