###################################################################
# 
#  FILE NAME   : lterrc_oamh.mk 
# 
#  DESCRIPTION : The RRC OAMH module make file 
# 
#  REVISION HISTORY : 
# 
#  DATE          Name        Reference        Comments 
#  ----          ----        ---------        -------- 
#
#  Copyright (c) 2009, Aricent Inc. 
# 
###################################################################

OAMH_INCLUDES=	-I${RRC_ROOT}/oamh/include \
				-I$(RRC_ROOT)/common/oamim/include \
				-I$(RRC_ROOT)/../../common/in_memory_logging/include

OAMH_SRCDIR = ${RRC_ROOT}/oamh/src

OAMH_SRCFILES = oamh_ed.c		\
			   	oamh_fsm.c

OAMH_OBJFILES:=$(OAMH_SRCFILES)
OAMH_OBJFILES:=$(patsubst %.c,$(OBJDIR)/%.o,$(OAMH_OBJFILES))

OAMH_DEPFILES:=$(OAMH_SRCFILES)
OAMH_DEPFILES:=$(patsubst %.c,$(OBJDIR)/%.d,$(OAMH_DEPFILES))

OAMH_LIB=$(LIBDIR)/librrcoamh.a

oamh:  $(MODE) $(OAMH_LIB)

$(CSC_OBJFILES) : lterrc_oamh.mk

$(OAMH_LIB): $(OAMH_OBJFILES)
	@echo '[$(OAMH_LIB)]'
	$(RRC_AR) $(RRC_ARFLAGS) $(OAMH_LIB) $(OAMH_OBJFILES)
	$(RRC_RANLIB) $(OAMH_LIB)
	@echo

ifneq ($(strip $(OAMH_DEPFILES)),)
-include $(OAMH_DEPFILES)
endif

.PHONY : oamh 

#add unit data to global scope variables
VPATH += $(OAMH_SRCDIR)
RRC_OBJFILES += $(OAMH_OBJFILES)
RRC_INCLUDES += $(OAMH_INCLUDES)
RRC_LIBS += $(OAMH_LIB)
