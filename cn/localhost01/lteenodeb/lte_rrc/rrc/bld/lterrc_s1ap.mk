###################################################################
# 
#  FILE NAME   : lterrc_s1ap.mk  
# 
#  DESCRIPTION : The RRC CSC module make file 
# 
#  REVISION HISTORY : 
# 
#  DATE          Name        Reference        Comments 
#  ----          ----        ---------        -------- 
#
#  Copyright (c) 2009, Aricent Inc. 
# 
###################################################################

S1AP_INCLUDES = -I${RRC_ROOT}/common/include/multi \
					 -I${RRC_ROOT}/common/rrmim/include/multi \
					 -I${RRC_ROOT}/llim/intf_lib/include/multi

S1AP_INCLUDES +=   -I${RRC_ROOT}/common/include \
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
-I${RRC_ROOT}/common/s1ap_common/inc \
-I${RRC_ROOT}/asn/include/s1ap \
-I$(RRC_ROOT)/common/include/ \
-I$(RRC_ROOT)/../runner/include/ 

S1AP_SRCDIR = ${RRC_ROOT}/../s1ap/src/

S1AP_SRCFILES =	s1ap_oam.c		\
				s1ap_init.c	\
				s1ap_md.c			\
				s1ap_asn_codec.c		\
				s1ap_lppa_asn_codec.c		\
				s1ap_mme_fsm.c		\
				s1ap_enb_config_fsm.c		\
				s1ap_mme.c		\
				s1ap_error_indication.c		\
				s1ap_csc.c		\
				s1ap_utils.c		\
				s1ap_uecc.c      \
				s1ap_rrm.c \
				s1ap_enb_mgr_fsm.c	\
				s1ap_common_intf.c  \
				s1ap_x2ap.c \
				s1ap_logger.c \
				s1ap_rrm_rim_intl.c
				
ifeq '$(LINUX_PC_TEST)' 'true'
S1AP_SRCFILES +=  s1ap_test.c 
endif


S1AP_OBJFILES:=$(S1AP_SRCFILES)
S1AP_OBJFILES:=$(patsubst %.c,$(OBJDIR)/%.o,$(S1AP_OBJFILES))

S1AP_DEPFILES:=$(S1AP_SRCFILES)
S1AP_DEPFILES:=$(patsubst %.c,$(OBJDIR)/%.d,$(S1AP_DEPFILES))

S1AP_LIB=$(LIBDIR)/librrcs1ap.a
S1APWRAP_LIB=$(LIBDIR)/librrcs1apwrap.a

s1ap:  $(MODE) $(S1AP_LIB)

$(S1AP_OBJFILES) : lterrc_s1ap.mk

$(S1AP_LIB): $(S1AP_OBJFILES)
	@echo '[$(S1AP_LIB)]'
	$(RRC_AR) $(RRC_ARFLAGS) $(S1AP_LIB) $(S1AP_OBJFILES) $(S1APWRAP_LIB)
	$(RRC_RANLIB) $(S1AP_LIB)
	@echo

ifneq ($(strip $(S1AP_DEPFILES)),)
-include $(S1AP_DEPFILES)
endif

.PHONY : s1ap clean_s1ap

#add unit data to global scope variables
VPATH += $(S1AP_SRCDIR)
RRC_OBJFILES += $(S1AP_OBJFILES)
RRC_INCLUDES += $(S1AP_INCLUDES)
RRC_LIBS += $(S1AP_LIB)
