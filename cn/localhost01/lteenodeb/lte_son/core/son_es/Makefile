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
#       Make file to be used for building libes.a.
#
##############################################################################
include $(LTE_ROOT)/make.inc
ifeq '$(COMPILE_SON_AUT)' 'yes'
include $(LTE_ROOT)/lte_son_temp/Makefile.in
else
include $(LTE_SON)/Makefile.in
endif

SRCDIR = $(SON_ES_ROOT)/src
INCDIR = $(SON_ES_ROOT)/include


SON_ES_INC  = -I$(INCDIR)
SON_ES_INC += -I$(INCLUDE_COMM)
SON_ES_INC += -I$(CSPL_INC)
SON_ES_INC += -I$(CSPL_POSIX)
SON_ES_INC += -I$(SON_NMM_ROOT)/include
SON_ES_INC += -I$(SON_MIF_ROOT)/include
SON_ES_INC += -I$(INCLUDE_IMPORT)
SON_ES_INC += -I$(INCLUDE_RRC)
SON_ES_INC += -I$(INCLUDE_X2AP)
SON_ES_INC += -I$(SON_X2_ROOT)/include
SON_ES_INC += -I$(INCLUDE_RRM)
SON_ES_INC += -I$(ADVLOG_INC)
SON_ES_INC += -I$(INCLUDE_LGR)
SON_ES_INC += -I$(INCLUDE_ANR)

ES_OBJS = $(OBJDIR)/son_es_fsm.o  \
	$(OBJDIR)/son_es_cell_fsm.o \
	$(OBJDIR)/son_es_msg_handler.o \
	$(OBJDIR)/son_es_ctxt_mgr.o 


target : $(LIBDIR)/libes.a

$(LIBDIR)/libes.a : $(ES_OBJS)
	ar rv $@ $^

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(COMPILE) $< $(SON_FLAG) $(SON_FLAG) $(SON_ES_INC)  -o $@

clean:
	rm -rf $(ES_OBJS)
	rm -rf $(LIBDIR)/libes.a
