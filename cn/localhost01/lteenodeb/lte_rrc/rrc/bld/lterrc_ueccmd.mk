###################################################################
# 
#  FILE NAME   : lterrc_ueccmd.mk  
# 
#  DESCRIPTION : The RRC UECC MD module make file 
# 
#  Copyright (c) 2012, Aricent Inc. 
# 
###################################################################

UECCMD_INCLUDES =  -I${RRC_ROOT}/ueccmd/include    \
				   -I$(RRC_ROOT)/asn/include
				
UECCMD_INCLUDES += -I${RRC_ROOT}/common/rrmim/include/multi


UECCMD_SRCDIR = ${RRC_ROOT}/ueccmd/src

UECCMD_SRCFILES =	ueccmd_md.c		\
	         		ueccmd_utils.c	\
			    	ueccmd_init.c	\
			    	ueccmd_uecc.c	\
			    	ueccmd_llim.c	\
			    	ueccmd_oamh.c

UECCMD_OBJFILES:=$(UECCMD_SRCFILES)
UECCMD_OBJFILES:=$(patsubst %.c,$(OBJDIR)/%.o,$(UECCMD_OBJFILES))

UECCMD_DEPFILES:=$(UECCMD_SRCFILES)
UECCMD_DEPFILES:=$(patsubst %.c,$(OBJDIR)/%.d,$(UECCMD_DEPFILES))

UECCMD_LIB=$(LIBDIR)/librrcueccmd.a

ueccmd:  $(MODE) $(UECCMD_LIB)

$(UECCMD_OBJFILES) : lterrc_ueccmd.mk 

$(UECCMD_LIB): $(UECCMD_OBJFILES)
	@echo '[$(UECCMD_LIB)]'
	$(RRC_AR) $(RRC_ARFLAGS) $(UECCMD_LIB) $(UECCMD_OBJFILES)
	$(RRC_RANLIB) $(UECCMD_LIB)
	@echo

ifneq ($(strip $(UECCMD_DEPFILES)),)
-include $(UECCMD_DEPFILES)
endif

.PHONY : ueccmd clean_ueccmd

#add unit data to global scope variables
VPATH += $(UECCMD_SRCDIR)
RRC_OBJFILES += $(UECCMD_OBJFILES)
RRC_INCLUDES += $(UECCMD_INCLUDES)
RRC_LIBS += $(UECCMD_LIB)
