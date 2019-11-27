#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: Makefile,v 1.0 2012/12/21 12:13:41 Shilpi Exp $
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
#       Make file to be used for building libmro.a.
#
##############################################################################
include $(LTE_ROOT)/make.inc
ifeq '$(COMPILE_SON_AUT)' 'yes'
include $(LTE_ROOT)/lte_son_temp/Makefile.in
else
include $(LTE_SON)/Makefile.in
endif

SRCDIR = $(SON_MRO_ROOT)/src
INCDIR = $(SON_MRO_ROOT)/include

SON_MRO_INC  = -I$(INCDIR)
SON_MRO_INC += -I$(INCLUDE_COMM)
SON_MRO_INC += -I$(CSPL_INC)
SON_MRO_INC += -I$(CSPL_POSIX)
SON_MRO_INC += -I$(INCLUDE_ANR)
SON_MRO_INC += -I$(INCLUDE_INTERFACE)
SON_MRO_INC += -I$(INCLUDE_IMPORT)
SON_MRO_INC += -I$(INCLUDE_RRM)
SON_MRO_INC += -I$(ADVLOG_INC)
SON_MRO_INC += -I$(INCLUDE_X2AP)
SON_MRO_INC += -I$(INCLUDE_LGR)

MRO_OBJS = $(OBJDIR)/son_mro_fsm.o\
    $(OBJDIR)/son_mro_ctxt_mgr.o\
    $(OBJDIR)/son_mro_msg_handler.o\
    $(OBJDIR)/son_mro_cell_fsm.o

target : $(LIBDIR)/libmro.a

$(LIBDIR)/libmro.a : $(MRO_OBJS)
	ar rv $@ $^

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(COMPILE)   $< $(SON_FLAG) $(SON_MRO_INC)  -o $@

clean:
	rm -rf $(MRO_OBJS)
	rm -rf $(LIBDIR)/libmro.a
