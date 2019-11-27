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

include lterrc.mk

RRMIM_INCLUDES = -I${RRC_ROOT}/common/rrmim/include
RRMIM_SRCDIR = ${RRC_ROOT}/common/rrmim/src

#SPR 21660 changes start_mokshi
ifeq '$(NBIOT_FEAT)' 'enable'
RRMIM_SRCFILES = rrc_rrm_il_composer.c \
				 rrc_rrmNb_il_composer.c \
				 rrc_rrmNb_il_parser.c \
				 rrc_rrm_il_parser.c
else
RRMIM_SRCFILES = rrc_rrm_il_composer.c \
				 rrc_rrm_il_parser.c
endif
#SPR 21660 changes end_mokshi
RRMIM_OBJFILES:=$(RRMIM_SRCFILES)
RRMIM_OBJFILES:=$(patsubst %.c,$(OBJDIR)/%.o,$(RRMIM_OBJFILES))

RRMIM_DEPFILES:=$(RRMIM_SRCFILES)
RRMIM_DEPFILES:=$(patsubst %.c,$(OBJDIR)/%.d,$(RRMIM_DEPFILES))

RRMIM_LIB=$(LIBDIR)/librrcrrmim.a

rrmim: $(MODE) $(RRMIM_LIB) 

$(RRMIM_OBJFILES) : lterrc_rrmim.mk  

$(RRMIM_LIB): $(RRMIM_OBJFILES)
	@echo '[$(RRMIM_LIB)]'
	$(RRC_AR) $(RRC_ARFLAGS) $(RRMIM_LIB) $(RRMIM_OBJFILES)
	$(RRC_RANLIB) $(RRMIM_LIB)
	@echo

ifneq ($(strip $(RRMIM_DEPFILES)),)
-include $(RRMIM_DEPFILES)
endif

.PHONY : rrmim 

#add unit data to global scope variables
VPATH += $(RRMIM_SRCDIR)
RRC_OBJFILES += $(RRMIM_OBJFILES)
RRC_INCLUDES += $(RRMIM_INCLUDES)
RRC_LIBS += $(RRMIM_LIB)
