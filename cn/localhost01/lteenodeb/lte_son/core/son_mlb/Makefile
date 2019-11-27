#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: Makefile,v 1.0 2012/12/17 12:13:41 Sajida Exp $
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
#       Make file to be used for building libmlb.a.
#
##############################################################################
include $(LTE_ROOT)/make.inc
ifeq '$(COMPILE_SON_AUT)' 'yes'
include $(LTE_ROOT)/lte_son_temp/Makefile.in
else
include $(LTE_SON)/Makefile.in
endif

SRCDIR = $(SON_MLB_ROOT)/src
INCDIR = $(SON_MLB_ROOT)/include


SON_MLB_INC  = -I$(INCDIR)
SON_MLB_INC += -I$(INCLUDE_COMM)
SON_MLB_INC += -I$(CSPL_INC)
SON_MLB_INC += -I$(CSPL_POSIX)
SON_MLB_INC += -I$(SON_ANR_ROOT)/include
SON_MLB_INC += -I$(SON_MIF_ROOT)/include
SON_MLB_INC += -I$(SON_ES_ROOT)/include
SON_MLB_INC += -I$(INCLUDE_IMPORT)
SON_MLB_INC += -I$(INCLUDE_RRC)
SON_MLB_INC += -I$(INCLUDE_X2AP)
SON_MLB_INC += -I$(SON_X2_ROOT)/include
SON_MLB_INC += -I$(INCLUDE_INTERFACE)
SON_MLB_INC += -I$(INCLUDE_IMPORT)
SON_MLB_INC += -I$(INCLUDE_RRM)
SON_MLB_INC += -I$(INCLUDE_LGR)
SON_MLB_INC += -I$(ADVLOG_INC)

MLB_OBJS = $(OBJDIR)/son_mlb_fsm.o\
    $(OBJDIR)/son_mlb_cell_fsm.o\
    $(OBJDIR)/son_mlb_msg_handler.o\
    $(OBJDIR)/son_mlb_ctxt_mgr.o

target : $(LIBDIR)/libmlb.a

$(LIBDIR)/libmlb.a : $(MLB_OBJS)
	ar rv $@ $^

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(COMPILE)   $< $(SON_FLAG) $(SON_MLB_INC)  -o $@

clean:
	rm -rf $(MLB_OBJS)
	rm -rf $(LIBDIR)/libmlb.a
