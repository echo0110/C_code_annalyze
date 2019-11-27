#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: Makefile,v 1.0 2012/02/27 12:13:41 Komal Exp $
# *
# ****************************************************************************
# *
# *  File Description : Makefile
# *
# ****************************************************************************
# *
# * Revision Details
# * ----------------
# *
# *
# *
# *
# *
# *
# ****************************************************************************/
##############################################################################
#
#
#       Make file to be used for building libanr.a.
#
##############################################################################
include $(LTE_ROOT)/make.inc
ifeq '$(COMPILE_SON_AUT)' 'yes'
include $(LTE_ROOT)/lte_son_temp/Makefile.in
else
include $(LTE_SON)/Makefile.in
endif

SRCDIR = $(SON_ANR_ROOT)/src
INCDIR = $(SON_ANR_ROOT)/include


SON_ANR_INC  = -I$(INCDIR)
SON_ANR_INC += -I$(INCLUDE_COMM)
SON_ANR_INC += -I$(CSPL_INC)
SON_ANR_INC += -I$(CSPL_POSIX)
SON_ANR_INC += -I$(SON_NMM_ROOT)/include
SON_ANR_INC += -I$(SON_MIF_ROOT)/include
SON_ANR_INC += -I$(SON_ES_ROOT)/include
SON_ANR_INC += -I$(INCLUDE_IMPORT)
SON_ANR_INC += -I$(INCLUDE_RRC)
SON_ANR_INC += -I$(INCLUDE_X2AP)
SON_ANR_INC += -I$(SON_X2_ROOT)/include
SON_ANR_INC += -I$(INCLUDE_INTERFACE)
SON_ANR_INC += -I$(INCLUDE_IMPORT)
SON_ANR_INC += -I$(INCLUDE_RRM)
SON_ANR_INC += -I$(INCLUDE_MRO)
SON_ANR_INC += -I$(INCLUDE_MLB)
SON_ANR_INC += -I$(ADVLOG_INC)
SON_ANR_INC += -I$(INCLUDE_LGR)
SON_ANR_INC += -I$(INCLUDE_RACH_OPT)

ANR_OBJS = $(OBJDIR)/son_anr_fsm.o\
    $(OBJDIR)/son_anr_cell_fsm.o\
    $(OBJDIR)/son_anr_table.o \
    $(OBJDIR)/son_anr_utils.o \
    $(OBJDIR)/son_anr_ctxt_mgr.o\
    $(OBJDIR)/son_anr_enb_ctxt_mgr.o\
    $(OBJDIR)/son_anr_msg_handler.o

target : $(LIBDIR)/libanr.a

$(LIBDIR)/libanr.a : $(ANR_OBJS)
	ar rv $@ $^

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(COMPILE)   $< $(SON_FLAG) $(SON_ANR_INC)  -o $@

clean:
	rm -rf $(ANR_OBJS)
	rm -rf $(LIBDIR)/libanr.a
