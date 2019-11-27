###################################################################
# 
#  FILE NAME   : lterrc_advlogger.mk  
# 
#  DESCRIPTION : The Advance Logger make file 
# 
#  REVISION HISTORY : 
# 
#  DATE          Name        Reference        Comments 
#  ----          ----        ---------        -------- 
#
#  Copyright (c) 2009, Aricent Inc. 
# 
###################################################################

ADVLOGGER_INCLUDES =  -I${RRC_ROOT}/../tools/advlogger/src \
				
ADVLOGGER_SRCDIR = ${RRC_ROOT}/../tools/advlogger/src

ADVLOGGER_SRCFILES =	advlogging.c

ADVLOGGER_OBJFILES:=$(ADVLOGGER_SRCFILES)
ADVLOGGER_OBJFILES:=$(patsubst %.c,$(OBJDIR)/%.o,$(ADVLOGGER_OBJFILES))

ADVLOGGER_DEPFILES:=$(ADVLOGGER_SRCFILES)
ADVLOGGER_DEPFILES:=$(patsubst %.c,$(OBJDIR)/%.d,$(ADVLOGGER_DEPFILES))

ADVLOGGER_LIB=$(LIBDIR)/libadvlogging.a

advlogger:  $(MODE) $(ADVLOGGER_LIB)

$(ADVLOGGER_OBJFILES) : lterrc_advlogger.mk 

$(ADVLOGGER_LIB): $(ADVLOGGER_OBJFILES)
	@echo '[$(ADVLOGGER_LIB)]'
	$(RRC_AR) $(RRC_ARFLAGS) $(ADVLOGGER_LIB) $(ADVLOGGER_OBJFILES)
	$(RRC_RANLIB) $(ADVLOGGER_LIB)
	@echo

ifneq ($(strip $(ADVLOGGER_DEPFILES)),)
-include $(ADVLOGGER_DEPFILES)
endif

.PHONY : advlogger clean_advlogger

#add unit data to global scope variables
VPATH += $(ADVLOGGER_SRCDIR)
RRC_OBJFILES += $(ADVLOGGER_OBJFILES)
RRC_INCLUDES += $(ADVLOGGER_INCLUDES)
RRC_LIBS += $(ADVLOGGER_LIB)
