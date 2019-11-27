###################################################################
# 
#  FILE NAME   : lterrc_common.mk 
# 
#  DESCRIPTION : The RRC Common module make file 
# 
#  REVISION HISTORY : 
# 
#  DATE          Name        Reference        Comments 
#  ----          ----        ---------        -------- 
#
#  Copyright (c) 2009, Aricent Inc. 
# 
###################################################################

COMMON_INCLUDES = -I$(RRC_ROOT)/../../common/in_memory_logging/include/

COMMON_SRCDIR= ${RRC_ROOT}/common/src

COMMON_SRCFILES=	rrc_common_utils.c	\
					rrc_msg_mgmt.c		\
					rrm_rrc_asn1_convertor.c	\
					rrc_cp_common.c \
					rrc_shared_stats.c \
					rrc_logging.c

COMMON_OBJFILES:=$(COMMON_SRCFILES)
COMMON_OBJFILES:=$(patsubst %.c,$(OBJDIR)/%.o,$(COMMON_OBJFILES))

COMMON_DEPFILES:=$(COMMON_SRCFILES)
COMMON_DEPFILES:=$(patsubst %.c,$(OBJDIR)/%.d,$(COMMON_DEPFILES))

COMMON_LIB=$(LIBDIR)/librrccommon.a

common: $(MODE) $(COMMON_LIB) 

$(COMMON_OBJFILES) : lterrc_common.mk

$(COMMON_LIB): $(COMMON_OBJFILES)
	@echo '[$(COMMON_LIB)]'
	$(RRC_AR) $(RRC_ARFLAGS) $(COMMON_LIB) $(COMMON_OBJFILES)
	$(RRC_RANLIB) $(COMMON_LIB)
	@echo

ifneq ($(strip $(COMMON_DEPFILES)),)
-include $(COMMON_DEPFILES)
endif

.PHONY : common 

#add unit data to global scope variables
VPATH += $(COMMON_SRCDIR)
RRC_OBJFILES += $(COMMON_OBJFILES)
RRC_INCLUDES += $(COMMON_INCLUDES)
RRC_LIBS += $(COMMON_LIB)
