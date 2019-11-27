###################################################################
# 
#  FILE NAME   : lterrc_csc.mk  
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

CSC_INCLUDES =  -I${RRC_ROOT}/csc/include		\
				-I$(RRC_ROOT)/asn/include
				
CSC_INCLUDES += -I${RRC_ROOT}/common/rrmim/include/multi


CSC_SRCDIR = ${RRC_ROOT}/csc/src

CSC_SRCFILES =	csc_bch_m.c		\
				csc_cell_m_fsm.c	\
				csc_md.c			\
				csc_sfn_m.c		\
				csc_cell_m.c		\
				csc_init.c		\
				csc_oamh.c		\
				csc_utils.c \
				csc_paging_m.c \
				csc_cell_ctx.c

CSC_OBJFILES:=$(CSC_SRCFILES)
CSC_OBJFILES:=$(patsubst %.c,$(OBJDIR)/%.o,$(CSC_OBJFILES))

CSC_DEPFILES:=$(CSC_SRCFILES)
CSC_DEPFILES:=$(patsubst %.c,$(OBJDIR)/%.d,$(CSC_DEPFILES))

CSC_LIB=$(LIBDIR)/librrccsc.a

csc:  $(MODE) $(CSC_LIB)

$(CSC_OBJFILES) : lterrc_csc.mk 

$(CSC_LIB): $(CSC_OBJFILES)
	@echo '[$(CSC_LIB)]'
	$(RRC_AR) $(RRC_ARFLAGS) $(CSC_LIB) $(CSC_OBJFILES)
	$(RRC_RANLIB) $(CSC_LIB)
	@echo

ifneq ($(strip $(CSC_DEPFILES)),)
-include $(CSC_DEPFILES)
endif

.PHONY : csc clean_csc

#add unit data to global scope variables
VPATH += $(CSC_SRCDIR)
RRC_OBJFILES += $(CSC_OBJFILES)
RRC_INCLUDES += $(CSC_INCLUDES)
RRC_LIBS += $(CSC_LIB)
