###################################################################
# 
#  FILE NAME   : lterrc_s1apwrap.mk 
# 
#  DESCRIPTION : The RRC S1ap Wrappers Make file 
# 
#  REVISION HISTORY : 
# 
#  DATE          Name        Reference        Comments 
#  ----          ----        ---------        -------- 
#
#  Copyright (c) 2009, Aricent Inc. 
# 
###################################################################

include sctp_mac.mk
CPPFLAGS +=  -DSIG_COMMON_WITH_CSPL -DSIGTRAN_PORT_LINUX 

S1AP_WRAP_INCLUDES += -I${RRC_ROOT}/common/s1ap_wrap/include \
                      -I${RRC_ROOT}/asn/include 


S1AP_WRAP_SRCDIR= ${RRC_ROOT}/common/s1ap_wrap/src

S1AP_WRAP_SRCFILES=	sctp_wrappers.c	\
					search_tree_wrap.c \
 			list_wrap.c

S1AP_WRAP_OBJFILES:=$(S1AP_WRAP_SRCFILES)
S1AP_WRAP_OBJFILES:=$(patsubst %.c,$(OBJDIR)/%.o,$(S1AP_WRAP_OBJFILES))

S1AP_WRAP_DEPFILES:=$(S1AP_WRAP_SRCFILES)
S1AP_WRAP_DEPFILES:=$(patsubst %.c,$(OBJDIR)/%.d,$(S1AP_WRAP_DEPFILES))

S1AP_WRAP_LIB=$(LIBDIR)/librrcs1apwrap.a

s1apwrap: $(MODE) $(S1AP_WRAP_LIB) 

$(S1AP_WRAP_OBJFILES) : lterrc_s1apwrap.mk

$(S1AP_WRAP_LIB): $(S1AP_WRAP_OBJFILES)
	@echo '[$(S1AP_WRAP_LIB)]'
	$(RRC_AR) $(RRC_ARFLAGS) $(S1AP_WRAP_LIB) $(S1AP_WRAP_OBJFILES)
	$(RRC_RANLIB) $(S1AP_WRAP_LIB)
	@echo

ifneq ($(strip $(S1AP_WRAP_DEPFILES)),)
-include $(S1AP_WRAP_DEPFILES)
endif

.PHONY : s1apwrap 

#add unit data to global scope variables
VPATH += $(S1AP_WRAP_SRCDIR)
RRC_OBJFILES += $(S1AP_WRAP_OBJFILES)
RRC_INCLUDES += $(S1AP_WRAP_INCLUDES)
RRC_LIBS += $(S1AP_WRAP_LIB)
