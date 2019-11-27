#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: Makefile,v 1.0 2012/01/17 12:13:41 gur26210 Exp $
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
#       Make file to be used for building libmif.a.
#
##############################################################################
include $(LTE_ROOT)/make.inc
ifeq '$(COMPILE_SON_AUT)' 'yes'
include $(LTE_ROOT)/lte_son_temp/Makefile.in
else
include $(LTE_SON)/Makefile.in
endif

SRCDIR = $(SON_MIF_ROOT)/src
INCDIR = $(SON_MIF_ROOT)/include


SON_MIF_INC  = -I$(INCDIR)
SON_MIF_INC += -I$(INCLUDE_COMM)
SON_MIF_INC += -I$(SON_ANR_ROOT)/include
SON_MIF_INC += -I$(SON_ES_ROOT)/include
SON_MIF_INC += -I$(SON_APPS_ROOT)/include
SON_MIF_INC += -I$(SON_X2_ROOT)/include
SON_MIF_INC += -I$(SON_MRO_ROOT)/include
SON_MIF_INC += -I$(SON_MLB_ROOT)/include
SON_MIF_INC += -I$(SON_NMM_ROOT)/include
SON_MIF_INC += -I$(INCLUDE_RRC)
SON_MIF_INC += -I$(INCLUDE_X2AP)
SON_MIF_INC += -I$(CSPL_INC)
SON_MIF_INC += -I$(CSPL_POSIX)
SON_MIF_INC += -I$(INCLUDE_IMPORT)
SON_MIF_INC += -I$(INCLUDE_RRM)
SON_MIF_INC += -I$(ADVLOG_INC)
SON_MIF_INC += -I$(INCLUDE_LGR)
SON_MIF_INC += -I$(INCLUDE_RACH_OPT)

MIF_OBJ = $(OBJDIR)/son_mif_fsm.o \
                $(OBJDIR)/son_oam_composer.o \
                $(OBJDIR)/son_oam_parser.o \
                $(OBJDIR)/son_mif_msg_handler.o \
        		$(OBJDIR)/son_shell.o \
	            $(OBJDIR)/son_rrm_composer.o \
        	    $(OBJDIR)/son_rrm_parser.o \

target : $(LIBDIR)/libmif.a

$(LIBDIR)/libmif.a : $(MIF_OBJ)
	ar rv $@ $^

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(COMPILE)   $< $(SON_FLAG) $(SON_MIF_INC)  -o $@

clean:
	rm -rf $(MIF_OBJ)
	rm -rf $(LIBDIR)/libmif.a

            
