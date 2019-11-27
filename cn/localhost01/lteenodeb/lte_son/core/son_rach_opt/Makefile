#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: Makefile,v 1.0 2013/8/13 12:16:41 SON Team Exp $
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
#       Make file to be used for building librach_opt.a.
#
##############################################################################
include $(LTE_ROOT)/make.inc
ifeq '$(COMPILE_SON_AUT)' 'yes'
include $(LTE_ROOT)/lte_son_temp/Makefile.in
LTE_SON=$(LTE_ROOT)/lte_son_temp
else
include $(LTE_SON)/Makefile.in
endif

SRCDIR = $(SON_RACH_OPT_ROOT)/src
INCDIR = $(SON_RACH_OPT_ROOT)/include

SON_RACH_OPT_INC  = -I$(INCDIR)
SON_RACH_OPT_INC += -I$(INCLUDE_COMM)
SON_RACH_OPT_INC += -I$(CSPL_INC)
SON_RACH_OPT_INC += -I$(CSPL_POSIX)
SON_RACH_OPT_INC += -I$(INCLUDE_INTERFACE)
SON_RACH_OPT_INC += -I$(INCLUDE_IMPORT)
SON_RACH_OPT_INC += -I$(INCLUDE_RRM)
SON_RACH_OPT_INC += -I$(ADVLOG_INC)
SON_RACH_OPT_INC += -I$(INCLUDE_LGR)
SON_RACH_OPT_INC += -I$(INCLUDE_X2AP)
SON_RACH_OPT_INC += -I$(INCLUDE_ANR)

RACH_OPT_OBJS = $(OBJDIR)/son_rach_opt_fsm.o\
    $(OBJDIR)/son_rach_opt_ctxt_mgr.o\
    $(OBJDIR)/son_rach_opt_msg_handler.o\
    $(OBJDIR)/son_rach_opt_cell_fsm.o

target : $(LIBDIR)/librach_opt.a

$(LIBDIR)/librach_opt.a : $(RACH_OPT_OBJS)
	ar rv $@ $^

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(COMPILE)   $< $(SON_FLAG) $(SON_RACH_OPT_INC)  -o $@

clean:
	rm -rf $(RACH_OPT_OBJS)
	rm -rf $(LIBDIR)/librach_opt.a
