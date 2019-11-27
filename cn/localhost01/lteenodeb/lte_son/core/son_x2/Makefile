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
#       Make file to be used for building libx2.a.
#
##############################################################################
include $(LTE_ROOT)/make.inc
ifeq '$(COMPILE_SON_AUT)' 'yes'
include $(LTE_ROOT)/lte_son_temp/Makefile.in
else
include $(LTE_SON)/Makefile.in
endif

SRCDIR = $(SON_X2_ROOT)/src
INCDIR = $(SON_X2_ROOT)/include

SON_X2_INC  = -I$(INCDIR)
SON_X2_INC += -I$(INCLUDE_COMM)
SON_X2_INC += -I$(CSPL_INC)
SON_X2_INC += -I$(CSPL_POSIX)
SON_X2_INC += -I$(INCLUDE_INTERFACE)
SON_X2_INC += -I$(INCLUDE_IMPORT)
SON_X2_INC += -I$(INCLUDE_RRM)
SON_X2_INC += -I$(INCLUDE_RRC)
SON_X2_INC += -I$(INCLUDE_X2AP)
SON_X2_INC += -I$(ADVLOG_INC)
SON_X2_INC += -I$(INCLUDE_ANR)
SON_X2_INC += -I$(INCLUDE_MRO)
SON_X2_INC += -I$(INCLUDE_MLB)
SON_X2_INC += -I$(INCLUDE_LGR)
SON_X2_INC += -I$(INCLUDE_ES)


X2_OBJS = $(OBJDIR)/son_x2_msg_handler.o\
    $(OBJDIR)/son_x2_ctxt_mgr.o\
    $(OBJDIR)/son_x2_fsm.o\
    $(OBJDIR)/son_x2_enb_fsm.o\
    $(OBJDIR)/son_enb_table.o

target : $(LIBDIR)/libx2.a

$(LIBDIR)/libx2.a : $(X2_OBJS)
	ar rv $@ $^

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(COMPILE)   $< $(SON_FLAG) $(SON_X2_INC)  -o $@

clean:
	rm -rf $(X2_OBJS)
	rm -rf $(LIBDIR)/libx2.a
