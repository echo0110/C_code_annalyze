###################################################################
# 
#  FILE NAME   : lterrc_llim.mk 
# 
#  DESCRIPTION : The RRC LLIM module make file 
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

LLIM_INCLUDES = -I${RRC_ROOT}/llim/common/include	\
				-I${RRC_ROOT}/llim/main/include		\
				-I${RRC_ROOT}/llim/pup/include

LLIM_SRCDIR = ${RRC_ROOT}/llim

LLIM_INCLUDES += -I${RRC_ROOT}/llim/intf_lib/include
LLIM_INTF_LIB_SRCDIR = $(LLIM_SRCDIR)/intf_lib/src

LLIM_COMMON_SRCDIR = $(LLIM_SRCDIR)/common/src

LLIM_COMMON_SRCFILES = rrc_llim_utils.c

# SPR 21660 changes start_mokshi
ifeq '$(NBIOT_FEAT)' 'enable'
LLIM_INTF_LIB_SRCFILES =rrc_mac_il_composer.c		\
						rrc_mac_il_parser.c		\
						rrc_pdcp_il_composer.c	\
						rrc_pdcp_il_parser.c		\
						rrc_phy_il_composer.c		\
						rrc_phy_il_parser.c		\
						rrc_rlc_il_composer.c		\
						rrc_rlc_il_parser.c         \
						rrc_macNb_il_parser.c		\
						rrc_macNb_il_composer.c		\
						rrc_rlcNb_il_composer.c		\
						rrc_rlcNb_il_parser.c		\
						rrc_phyNb_il_composer.c		\
						rrc_phyNb_il_parser.c		\
						rrc_s1u_il_composer.c		\
						rrc_s1u_il_parser.c
else
LLIM_INTF_LIB_SRCFILES =rrc_mac_il_composer.c		\
						rrc_mac_il_parser.c		\
						rrc_pdcp_il_composer.c	\
						rrc_pdcp_il_parser.c		\
						rrc_phy_il_composer.c		\
						rrc_phy_il_parser.c		\
						rrc_rlc_il_composer.c		\
						rrc_rlc_il_parser.c         \
						rrc_s1u_il_composer.c		\
						rrc_s1u_il_parser.c
endif

#SPR 21660 changes end_mokshi

LLIM_MAIN_SRCDIR = $(LLIM_SRCDIR)/main/src

LLIM_MAIN_SRCFILES =rrc_cemh.c			\
		 			rrc_cemh_fsm.c		\
					rrc_llim_init.c		\
					rrc_llim_oamh.c		\
					rrc_llmd.c			\
					rrc_uemh.c			\
					rrc_uemh_fsm.c
					
LLIM_SRC_FILES:= $(LLIM_COMMON_SRCFILES) $(LLIM_INTF_LIB_SRCFILES) $(LLIM_MAIN_SRCFILES)

LLIM_OBJFILES:=$(LLIM_SRC_FILES)
LLIM_OBJFILES:=$(patsubst %.c,$(OBJDIR)/%.o,$(LLIM_OBJFILES))

LLIM_DEPFILES:=$(LLIM_SRC_FILES)
LLIM_DEPFILES:=$(patsubst %.c,$(OBJDIR)/%.d,$(LLIM_DEPFILES))

LLIM_LIB=$(LIBDIR)/librrcllim.a

llim:  $(MODE) $(LLIM_LIB)

$(LLIM_OBJFILES) : lterrc_llim.mk	 

$(LLIM_LIB): $(LLIM_OBJFILES)
	@echo '[$(LLIM_LIB)]'
	$(RRC_AR) $(RRC_ARFLAGS) $(LLIM_LIB) $(LLIM_OBJFILES)
	$(RRC_RANLIB) $(LLIM_LIB)
	@echo

ifneq ($(strip $(LLIM_DEPFILES)),)
-include $(LLIM_DEPFILES)
endif

.PHONY : llim 

#add unit data to global scope variables
VPATH += $(LLIM_COMMON_SRCDIR) $(LLIM_INTF_LIB_SRCDIR) $(LLIM_MAIN_SRCDIR)
RRC_OBJFILES += $(LLIM_OBJFILES)
RRC_INCLUDES += $(LLIM_INCLUDES)
RRC_LIBS += $(LLIM_LIB)
