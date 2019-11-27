###################################################################
# 
#  FILE NAME   : lterrc_emtcvalidatoinfrwrk.mk  
# 
#  DESCRIPTION : The EMTC Validation Frwrk Makefile
# 
#  REVISION HISTORY : 
# 
#  DATE          Name        Reference        Comments 
#  ----          ----        ---------        -------- 
#
#  Copyright (c) 2017, Aricent Inc. 
# 
###################################################################

				
EMTCVALIDATIONFRWRK_INCLUDES =  -I${RRC_ROOT}/../../common/include/lte_port/ 
EMTCVALIDATIONFRWRK_SRCDIR = ${RRC_ROOT}/../../common/src

EMTCVALIDATIONFRWRK_SRCFILES = emtcCommonValidation.c

EMTCVALIDATIONFRWRK_OBJFILES:=$(EMTCVALIDATIONFRWRK_SRCFILES)
EMTCVALIDATIONFRWRK_OBJFILES:=$(patsubst %.c,$(OBJDIR)/%.o,$(EMTCVALIDATIONFRWRK_OBJFILES))

EMTCVALIDATIONFRWRK_DEPFILES:=$(EMTCVALIDATIONFRWRK_SRCFILES)
EMTCVALIDATIONFRWRK_DEPFILES:=$(patsubst %.c,$(OBJDIR)/%.d,$(EMTCVALIDATIONFRWRK_DEPFILES))

EMTCVALIDATIONFRWRK_LIB=${RRC_ROOT}/../../common/lib/emtcValidationFrwk.a

emtcvalidationfrwrk:  $(MODE) $(EMTCVALIDATIONFRWRK_LIB)

$(EMTCVALIDATIONFRWRK_OBJFILES) : lterrc_emtcvalidatoinfrwrk.mk 

$(EMTCVALIDATIONFRWRK_LIB): $(EMTCVALIDATIONFRWRK_OBJFILES)
	@echo '[$(EMTCVALIDATIONFRWRK_LIB)]'
	$(RRC_AR) $(RRC_ARFLAGS) $(EMTCVALIDATIONFRWRK_LIB) $(EMTCVALIDATIONFRWRK_OBJFILES)
	$(RRC_RANLIB) $(EMTCVALIDATIONFRWRK_LIB)
	@echo

ifneq ($(strip $(EMTCVALIDATIONFRWRK_DEPFILES)),)
-include $(EMTCVALIDATIONFRWRK_DEPFILES)
endif

.PHONY : emtcvalidationfrwrk clean_emtcvalidationfrwrk

#add unit data to global scope variables
VPATH += $(EMTCVALIDATIONFRWRK_SRCDIR)
RRC_OBJFILES += $(EMTCVALIDATIONFRWRK_OBJFILES)
RRC_INCLUDES += $(EMTCVALIDATIONFRWRK_INCLUDES)
RRC_LIBS += $(EMTCVALIDATIONFRWRK_LIB)
