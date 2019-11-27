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
#       Make file to be used for building libnmm.a.
#
##############################################################################
include $(LTE_ROOT)/make.inc
ifeq '$(COMPILE_SON_AUT)' 'yes'
include $(LTE_ROOT)/lte_son_temp/Makefile.in
else
include $(LTE_SON)/Makefile.in
endif

SRCDIR = $(SON_NMM_ROOT)/src
INCDIR = $(SON_NMM_ROOT)/include



SON_NMM_INC += -I$(INCDIR)
SON_NMM_INC += -I$(INCLUDE_COMM)
SON_NMM_INC += -I$(CSPL_INC)
SON_NMM_INC += -I$(CSPL_POSIX)
SON_NMM_INC += -I$(INCLUDE_SIB)
SON_NMM_INC += -I$(INCLUDE_UMTS_SIB)
SON_NMM_INC += -I$(INCLUDE_IMPORT)
SON_NMM_INC += -I$(INCLUDE_RRM)
SON_NMM_INC += -I$(INCLUDE_INTERFACE)
SON_NMM_INC += -I$(ADVLOG_INC)
SON_NMM_INC += -I$(INCLUDE_LGR)

ifeq '$(LTE_SON_PLATFORM)' 'fapi'
SRCDIR_L1 = $(LTE_SON)/port/L1/fapi/src
SON_NMM_INC += -I$(LTE_SON)/port/L1/fapi/include/
endif

ifeq '$(LTE_SON_PLATFORM)' 'mspd'
SRCDIR_L1 = $(LTE_SON)/port/L1/mspd/src
SON_NMM_INC += -I$(LTE_SON)/port/L1/mspd/include/
endif

ifeq '$(LTE_SON_PLATFORM)' 'astri'
SRCDIR_L1 = $(LTE_SON)/port/L1/astri/src
SON_NMM_INC += -I$(LTE_SON)/port/L1/astri/include/
endif


SON_NMM_INC += -I$(INCLUDE_RRM)
SON_NMM_INC += -I$(INCLUDE_IMPORT)


NMM_OBJ = \
          $(OBJDIR)/son_nmm_fsm.o \
          $(OBJDIR)/son_nmm_phy_api.o\
          $(OBJDIR)/son_nmm.o \
          $(OBJDIR)/son_nmmctl_fsm.o 

ifeq '$(BROADCOM_FAPI)' 'yes'
NMM_OBJ += $(OBJDIR)/son_brcm_sniffer_phy_if.o
endif
          
ifeq '$(LTE_SON_PLATFORM)' 'fapi'
NMM_OBJ += $(OBJDIR)/son_nmm_fapi.o 
NMM_OBJ += $(OBJDIR)/son_fapi_composer.o 
NMM_OBJ += $(OBJDIR)/son_fapi_parser.o 
endif

ifeq '$(LTE_SON_PLATFORM)' 'mspd'
NMM_OBJ += $(OBJDIR)/son_nmm_mspd.o
#SPR 22925 Fix +
ifeq '$(MSPD_NMM_INTG_L1)' 'yes'
NMM_OBJ += $(OBJDIR)/son_nmm_compose_mspd.o
endif
#SPR 22925 Fix -
NMM_OBJ += $(OBJDIR)/son_mspd_composer.o 
NMM_OBJ += $(OBJDIR)/son_mspd_parser.o 
NMM_OBJ += $(OBJDIR)/son_mspd_pack_unpack_utils.o 
endif

ifeq '$(LTE_SON_PLATFORM)' 'astri'
NMM_OBJ += $(OBJDIR)/son_nmm_astri.o 
NMM_OBJ += $(OBJDIR)/son_astri_composer.o 
NMM_OBJ += $(OBJDIR)/son_astri_parser.o 
endif


target : $(LIBDIR)/libnmm.a

$(LIBDIR)/libnmm.a : $(NMM_OBJ)
	ar rv $@ $^

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(COMPILE)   $< $(SON_FLAG) $(SON_NMM_INC)  -o $@
	${COMPILE}   $< ${SON_FLAG} ${SON_NMM_INC} -o $@

$(OBJDIR)/%.o : $(SRCDIR_L1)/%.c
	$(COMPILE)   $<  $(SON_FLAG)  $(SON_NMM_INC)  -o $@

clean:
	rm -rf $(NMM_OBJ)
	rm -rf $(LIBDIR)/libnmm.a

            
