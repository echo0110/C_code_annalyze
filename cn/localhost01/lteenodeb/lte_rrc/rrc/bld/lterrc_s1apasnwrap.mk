###################################################################
# 
#  FILE NAME   : lterrc_s1apasnwrap.mk 
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

S1AP_ASN_WRAP_INCLUDES = -I${RRC_ROOT}/common/include/multi \
							  -I${RRC_ROOT}/common/rrmim/include/multi \
							  -I${RRC_ROOT}/llim/intf_lib/include/multi


S1AP_ASN_WRAP_INCLUDES += -I${RRC_ROOT}/../s1ap/s1ap_enc_dec/include \
-I${RRC_ROOT}/common/include \
-I${RRC_ROOT}/common/oamim/include \
-I${RRC_ROOT}/common/s1apim/include \
-I${RRC_ROOT}/llim/common/include \
-I${RRC_ROOT}/llim/main/include \
-I${RRC_ROOT}/llim/pup/include \
-I${RRC_ROOT}/asn/src \
-I${RRC_ROOT}/asn/include \
-I${RRC_ROOT}/csc/include \
-I${RRC_ROOT}/../s1ap/include \
-I${RRC_ROOT}/asn/include \
-I${RRC_ROOT}/oamh/include \
-I${RRC_ROOT}/common/oamim/include \
-I${RRC_ROOT}/uecc/fsm/include \
-I${RRC_ROOT}/uecc/interface/include \
-I${RRC_ROOT}/uecc/utils/include \
-I${RRC_ROOT}/asn/include \
-I${RRC_ROOT}/common/s1apim/include \
-I${RRC_ROOT}/common/s1ap_oamim/include \
-I${RRC_ROOT}/common/s1ap_wrap/include \
-I${RRC_ROOT}/asn/include/s1ap \
-I${RRC_ROOT}/../s1ap/s1ap_enc_dec/include \
-I${RRC_ROOT}/../s1ap/s1ap_mgr/include

S1AP_ASN_WRAP_SRCDIR= ${RRC_ROOT}/../s1ap/s1ap_enc_dec/src

S1AP_ASN_WRAP_SRCFILES=	s1ap_wrapper_asn.c \
                        rrc_s1ap_asn_intrl_enc.c \
                        rrc_s1ap_asn_intrl_enc_3gpp.c \
                        rrc_s1ap_asn_intrl_dec.c \
                        rrc_s1ap_asn_intrl_dec_3gpp.c

S1AP_ASN_WRAP_OBJFILES:=$(S1AP_ASN_WRAP_SRCFILES)
S1AP_ASN_WRAP_OBJFILES:=$(patsubst %.c,$(OBJDIR)/%.o,$(S1AP_ASN_WRAP_OBJFILES))

S1AP_ASN_WRAP_DEPFILES:=$(S1AP_ASN_WRAP_SRCFILES)
S1AP_ASN_WRAP_DEPFILES:=$(patsubst %.c,$(OBJDIR)/%.d,$(S1AP_ASN_WRAP_DEPFILES))

S1AP_ASN_WRAP_LIB=$(LIBDIR)/librrcs1apasnwrap.a

s1apasn_wrap: $(MODE) $(S1AP_ASN_WRAP_LIB) 

$(S1AP_ASN_WRAP_OBJFILES) : lterrc_s1apasnwrap.mk

$(S1AP_ASN_WRAP_LIB): $(S1AP_ASN_WRAP_OBJFILES)
	@echo '[$(S1AP_ASN_WRAP_LIB)]'
	$(RRC_AR) $(RRC_ARFLAGS) $(S1AP_ASN_WRAP_LIB) $(S1AP_ASN_WRAP_OBJFILES)
	$(RRC_RANLIB) $(S1AP_ASN_WRAP_LIB)
	@echo

ifneq ($(strip $(S1AP_ASN_WRAP_DEPFILES)),)
-include $(S1AP_ASN_WRAP_DEPFILES)
endif

.PHONY : s1apasn_wrap 

#add unit data to global scope variables
VPATH += $(S1AP_ASN_WRAP_SRCDIR)
RRC_OBJFILES += $(S1AP_ASN_WRAP_OBJFILES)
RRC_INCLUDES += $(S1AP_ASN_WRAP_INCLUDES)
RRC_LIBS += $(S1AP_ASN_WRAP_LIB)
