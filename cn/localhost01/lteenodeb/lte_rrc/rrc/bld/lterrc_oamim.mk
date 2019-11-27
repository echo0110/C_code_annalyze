###################################################################
# 
#  FILE NAME   : lterrc_oamim.mk 
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

OAMIM_INCLUDES = -I${RRC_ROOT}/common/oamim/include/multi
OAMIM_SRCDIR = ${RRC_ROOT}/common/oamim/src/multi

OAMIM_SRCFILES = rrc_oam_il_composer.c \
				 rrc_oam_il_parser.c

OAMIM_OBJFILES:=$(OAMIM_SRCFILES)
OAMIM_OBJFILES:=$(patsubst %.c,$(OBJDIR)/%.o,$(OAMIM_OBJFILES))

OAMIM_DEPFILES:=$(OAMIM_SRCFILES)
OAMIM_DEPFILES:=$(patsubst %.c,$(OBJDIR)/%.d,$(OAMIM_DEPFILES))

OAMIM_LIB=$(LIBDIR)/librrcoamim.a

oamim: $(MODE) $(OAMIM_LIB) 

$(OAMIM_OBJFILES) : lterrc_oamim.mk	

$(OAMIM_LIB): $(OAMIM_OBJFILES)
	@echo '[$(OAMIM_LIB)]'
	$(RRC_AR) $(RRC_ARFLAGS) $(OAMIM_LIB) $(OAMIM_OBJFILES)
	$(RRC_RANLIB) $(OAMIM_LIB)
	@echo

ifneq ($(strip $(OAMIM_DEPFILES)),)
-include $(OAMIM_DEPFILES)
endif

.PHONY : oamim 

#add unit data to global scope variables
VPATH += $(OAMIM_SRCDIR)
RRC_OBJFILES += $(OAMIM_OBJFILES)
RRC_INCLUDES += $(OAMIM_INCLUDES)
RRC_LIBS += $(OAMIM_LIB)
