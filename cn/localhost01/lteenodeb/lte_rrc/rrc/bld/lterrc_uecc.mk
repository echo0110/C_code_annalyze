###################################################################
# 
#  FILE NAME   : lterrc_uecc.mk
# 
#  DESCRIPTION : The RRC UECC module make file 
# 
#  REVISION HISTORY : 
# 
#  DATE          Name        Reference        Comments 
#  ----          ----        ---------        -------- 
#
#  Copyright (c) 2009, Aricent Inc. 
# 
###################################################################

UECC_DIR = ${RRC_ROOT}/uecc

UECC_INCLUDES =	-I$(UECC_DIR)/fsm/include				\
				-I$(UECC_DIR)/interface/include			\
				-I$(UECC_DIR)/utils/include				\
				-I$(RRC_ROOT)/asn/include				\
				-I$(RRC_ROOT)/asn/include/rtxsrc    	\
				-I$(RRC_ROOT)/common/s1apim/include
				
UECC_INCLUDES += -I${RRC_ROOT}/common/rrmim/include/multi

UECC_FSM_SRCDIR = $(UECC_DIR)/fsm/src
					
ifeq '$(NBIOT_FEAT)' 'enable'
#SPR 21660 changes start_mokshi					
UECC_FSM_SRCFILES =	uecc_ed.c		\
					uecc_init.c		\
					uecc_mainfsm.c	\
					uecc_rrm_ue_reconfig_fsm.c	\
					uecc_rcsfsm.c	\
					uecc_rcrfsm.c	\
					uecc_rmfsm.c	\
					uecc_fsm_engine.c \
					uecc_fsm_nb_engine.c \
					uecc_icsp.c \
					uecc_ct_a.c \
					uecc_isa_a.c \
					uecc_erabsp.c \
					uecc_erabsp_rmfsm.c \
					uecc_erab_delfsm.c \
					uecc_rrm_erab_relfsm.c \
					uecc_erab_modfsm.c \
                    uecc_rlf.c\
                    uecc_ue_context_modfsm.c \
                    uecc_trg_hofsm.c\
                    uecc_src_hofsm.c\
                    uecc_crefsm.c

#SPR 21660 changes end_mokshi					
endif
ifeq '$(NBIOT_FEAT)' 'disable'
UECC_FSM_SRCFILES =	uecc_ed.c		\
					uecc_init.c		\
					uecc_mainfsm.c	\
					uecc_rrm_ue_reconfig_fsm.c	\
					uecc_rcsfsm.c	\
					uecc_rcrfsm.c	\
					uecc_rmfsm.c	\
					uecc_fsm_engine.c \
					uecc_icsp.c \
					uecc_ct_a.c \
					uecc_isa_a.c \
					uecc_rrm_dc_bearer_reconfig_fsm.c \
					uecc_erabsp.c \
					uecc_erabsp_rmfsm.c \
					uecc_erab_delfsm.c \
					uecc_rrm_erab_relfsm.c \
					uecc_erab_modfsm.c \
                    uecc_rlf.c\
                    uecc_ue_context_modfsm.c \
                    uecc_trg_hofsm.c\
                    uecc_src_hofsm.c\
                    uecc_crefsm.c
endif

UECC_INTF_SRCDIR = ${UECC_DIR}/interface/src
					 
UECC_INTF_SRCFILES =	uecc_llim.c		\
		      		 	uecc_oamh.c		\
		      			uecc_csc.c		\
		      			uecc_rrm.c		\
		      			uecc_s1ap.c		\
		      			uecc_defaults.c	\
		      			uecc_llim_utils.c \
                        uecc_x2ap.c

UECC_UTILS_SRCDIR = ${UECC_DIR}/utils/src
						
UECC_UTILS_SRCFILES =	uecc_utils.c		\
						uecc_ue_ctx.c		\
						uecc_event_queue.c	\
						hmac_sha256.c \
						uecc_statistic.c \
						uecc_ue_timer_utils.c
						

UECC_SRC_FILES:= $(UECC_FSM_SRCFILES) $(UECC_INTF_SRCFILES) $(UECC_UTILS_SRCFILES)

UECC_OBJFILES:=$(UECC_SRC_FILES)
UECC_OBJFILES:=$(patsubst %.c,$(OBJDIR)/%.o,$(UECC_OBJFILES))

UECC_DEPFILES:=$(UECC_SRC_FILES)
UECC_DEPFILES:=$(patsubst %.c,$(OBJDIR)/%.d,$(UECC_DEPFILES))

UECC_LIB=$(LIBDIR)/librrcuecc.a

uecc:  $(MODE) $(UECC_LIB)

$(UECC_OBJFILES) : lterrc_uecc.mk	 

$(UECC_LIB): $(UECC_OBJFILES) 
	@echo '[$(UECC_LIB)]'
	$(RRC_AR) $(RRC_ARFLAGS) $(UECC_LIB) $(UECC_OBJFILES)
	$(RRC_RANLIB) $(S1APIM_LIB)	
	@echo

ifneq ($(strip $(UECC_DEPFILES)),)
-include $(UECC_DEPFILES)
endif

.PHONY : uecc 

#add unit data to global scope variables
VPATH += $(UECC_FSM_SRCDIR) $(UECC_INTF_SRCDIR) $(UECC_UTILS_SRCDIR)
RRC_OBJFILES += $(UECC_OBJFILES)
RRC_INCLUDES += $(UECC_INCLUDES)
RRC_LIBS += $(UECC_LIB)
