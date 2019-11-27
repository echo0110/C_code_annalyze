#/****************************************************************************
#*
#* ARICENT -
#*
#* Copyright (C) 2012 Aricent Inc. All Rights Reserved.
#*
#****************************************************************************
#*
#* SPR-612 Coverity Fix Starts
#* $Id: Makefile $
#* SPR-612 Coverity Fix Ends
#*
#****************************************************************************
#*
#* File Description: Make file to be used for building libapps.a(Static library
#*                   for SONApps moduel).
#*
#****************************************************************************
#* Revision Details
#* ----------------
#*
#* SPR-612 Coverity Fix Starts
#* $Log: Makefile $
#* SPR-612 Coverity Fix Ends
#* Revision 0.1  2012/23/02 10:00:00  Shilpi Jain
#* Initial code
#*
#****************************************************************************/
include $(LTE_ROOT)/make.inc
ifeq '$(COMPILE_SON_AUT)' 'yes'
include $(LTE_ROOT)/lte_son_temp/Makefile.in
else
include $(LTE_SON)/Makefile.in
endif

SRCDIR = $(SON_APPS_ROOT)/src
INCDIR = $(SON_APPS_ROOT)/include

SON_APPS_INC  = -I$(INCDIR)
SON_APPS_INC += -I$(INCLUDE_COMM)
SON_APPS_INC += -I$(CSPL_INC)
SON_APPS_INC += -I$(CSPL_POSIX)
SON_APPS_INC += -I$(SON_NMM_ROOT)/include
SON_APPS_INC += -I$(INCLUDE_INTERFACE)
SON_APPS_INC += -I$(INCLUDE_IMPORT)
SON_APPS_INC += -I$(INCLUDE_RRM)
SON_APPS_INC += -I$(INCLUDE_LGR)
SON_APPS_INC += -I$(ADVLOG_INC)
SON_APPS_INC += -I$(SON_ANR_ROOT)/include
    
APPS_OBJS = $(OBJDIR)/son_apps_acf.o \
    $(OBJDIR)/son_apps_common_intf.o \
    $(OBJDIR)/son_apps_pci.o \
    $(OBJDIR)/son_apps_fsm.o \
    $(OBJDIR)/son_apps_msg_handler.o

target : $(LIBDIR)/libapps.a

$(LIBDIR)/libapps.a : $(APPS_OBJS)
	ar rv $@ $^

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(COMPILE) $< $(SON_FLAG) $(SON_APPS_INC)  -o $@

clean:
	rm -rf $(APPS_OBJS)
	rm -rf $(LIBDIR)/libapps.a
