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
#       Make file to be used for building libsonimport.a.
#
##############################################################################

include $(LTE_ROOT)/make.inc
ifeq '$(COMPILE_SON_AUT)' 'yes'
include $(LTE_ROOT)/lte_son_temp/Makefile.in
else
include $(LTE_SON)/Makefile.in
endif

SRCDIR = $(SON_IMPORT_ROOT)/src
INCDIR = $(SON_IMPORT_ROOT)/include

SON_IMPORT_INC  = -I$(INCDIR)
SON_IMPORT_INC += -I$(INCLUDE_COMM)
SON_IMPORT_INC += -I$(INCLUDE_INTERFACE)
SON_IMPORT_INC += -I$(CSPL_INC)
SON_IMPORT_INC += -I$(CSPL_POSIX)
SON_IMPORT_INC += -I$(INCLUDE_RRM)
################################
# MLBRRC: Below line to be removed after X2AP support is there in RRC
#############################
SON_IMPORT_INC += -I$(SON_X2_ROOT)/include
SON_IMPORT_INC += -I$(INCLUDE_RRC)
SON_IMPORT_INC += -I$(INCLUDE_X2AP)
SON_IMPORT_INC += -I$(INCLUDE_LGR)
SON_IMPORT_INC += -I$(ADVLOG_INC)
SON_IMPORT_OBJ =   $(OBJDIR)/rrm_son_parser.o	$(OBJDIR)/rrm_son_composer.o	$(OBJDIR)/rrm_pack_unpack_utils.o $(OBJDIR)/rrc_x2apCommon_composer.o $(OBJDIR)/rrc_x2apCommon_parser.o $(OBJDIR)/rrc_pack_unpack_utils.o

target : $(LIBDIR)/libsonimport.a

$(LIBDIR)/libsonimport.a : $(SON_IMPORT_OBJ)
	ar rv $@ $^

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(COMPILE)  $< $(SON_FLAG) $(SON_IMPORT_INC) -o $@

clean:
	rm  $(SON_IMPORT_OBJ)
	rm  $(LIBDIR)/libsonimport.a

