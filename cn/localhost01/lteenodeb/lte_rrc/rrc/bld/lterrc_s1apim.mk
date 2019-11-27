###################################################################
# 
#  FILE NAME   : lterrc_s1apim.mk 
# 
#  DESCRIPTION : The RRC make file 
# 
#  REVISION HISTORY : 
# 
#  DATE          Name        Reference        Comments 
#  ----          ----        ---------        -------- 
#
#  Copyright (c) 2009, Aricent Inc. 
# 
###################################################################

S1APIM_INCLUDES = -I${RRC_ROOT}/common/s1apim/include

S1APIM_SRCDIR = ${RRC_ROOT}/common/s1apim/src

S1APIM_SRCFILES = rrc_uecc_error_indication.c \
                  rrc_s1ap_asn_intrl_enc.c \
				  rrc_s1ap_asn_intrl_dec.c \
				  rrc_s1ap_asn_intrl_enc_3gpp.c \
				  rrc_s1ap_asn_intrl_dec_3gpp.c

S1APIM_OBJFILES:=$(S1APIM_SRCFILES)
S1APIM_OBJFILES:=$(patsubst %.c,$(OBJDIR)/%.o,$(S1APIM_OBJFILES))

S1APIM_DEPFILES:=$(S1APIM_SRCFILES)
S1APIM_DEPFILES:=$(patsubst %.c,$(OBJDIR)/%.d,$(S1APIM_DEPFILES))

S1APIM_LIB=$(LIBDIR)/librrcs1apim.a

s1apim: $(MODE) $(S1APIM_LIB) 

$(S1APIM_OBJFILES) : lterrc_s1apim.mk

$(S1APIM_LIB): $(S1APIM_OBJFILES)
	@echo '[$(S1APIM_LIB)]'
	$(RRC_AR) $(RRC_ARFLAGS) $(S1APIM_LIB) $(S1APIM_OBJFILES)
	$(RRC_RANLIB) $(S1APIM_LIB)
	@echo

ifneq ($(strip $(S1APIM_DEPFILES)),)
-include $(S1APIM_DEPFILES)
endif

.PHONY : s1apim 

#add unit data to global scope variables
VPATH += $(S1APIM_SRCDIR)
RRC_OBJFILES += $(S1APIM_OBJFILES)
RRC_INCLUDES += $(S1APIM_INCLUDES)
RRC_LIBS += $(S1APIM_LIB)
