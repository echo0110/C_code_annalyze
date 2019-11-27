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
#               MAIN MAKE FILE
#       Make file to be used for building lte_son binary.
#
##############################################################################
include $(LTE_ROOT)/make.inc


ifeq '$(COMPILE_SON_AUT)' 'yes'
include $(LTE_ROOT)/lte_son_temp/Makefile.in
else
include $(LTE_SON)/Makefile.in
endif

SRC_COMM = $(LTE_SON)/common/src
ADVLOG_SRC = $(LTE_ROOT)/lte_rrc/tools/advlogger/src
lteMisc_SRC = $(LTE_ROOT)/common/src

COMM_OBJ += $(OBJDIR)/son_main.o
COMM_OBJ += $(OBJDIR)/son_utils.o 
COMM_OBJ += $(OBJDIR)/son_pack_unpack_utils.o
COMM_OBJ += $(OBJDIR)/son_list.o
COMM_OBJ += $(OBJDIR)/son_hash.o
COMM_OBJ += $(OBJDIR)/advlogging.o
COMM_OBJ += $(OBJDIR)/lteMisc.o
SON_COM_INCL +=  -I$(INCLUDE_IMPORT)
SON_COM_INCL +=  -I$(INCLUDE_RRM)
SON_COM_INCL +=  -I$(INCLUDE_LTE_COMMON)
SON_COM_INCL +=  -I$(INCLUDE_COMM)
SON_COM_INCL +=  -I$(INCLUDE_INTERFACE)
SON_COM_INCL +=  -I$(CSPL_INC)
SON_COM_INCL +=  -I$(CSPL_POSIX)
SON_COM_INCL +=  -I$(ADVLOG_INC)
SON_COM_INCL +=  -I$(INCLUDE_LGR)
SON_COM_INCL +=  -I$(INCLUDE_RACH_OPT)
SON_COM_INCL +=  -I$(INCLUDE_X2AP)
SON_COM_INCL +=  -I$(LTEMISC_INC)

ifeq '$(COMPILE_MRO)' 'yes'
LINKLIBS_MRO= $(LIBDIR)/libmro.a
COMMON_SUBDIR_MRO=$(SON_MRO_ROOT)
LIB_MRO=-lmro
SON_COM_INCL  += -I$(INCLUDE_MRO)
SON_COM_INCL  += -I$(INCLUDE_ANR)
endif

LINKLIBS :=  $(LIBDIR)/libapps.a $(LIBDIR)/libx2.a  $(LIBDIR)/libmif.a $(LIBDIR)/libes.a $(LIBDIR)/libnmm.a $(LIBDIR)/libsonimport.a $(LIBDIR)/libanr.a $(LINKLIBS_MRO)  $(LIBDIR)/libmlb.a $(CSPL_BUILD)/*.a $(LIB_SIB_DECODER)/*.a $(LIB_UMTS_SIB_DECODER)/*.a $(LIBDIR)/librach_opt.a
COMMON_SUBDIRS = $(SON_X2_ROOT)\
				$(SON_ANR_ROOT)\
				$(COMMON_SUBDIR_MRO)\
				$(SON_MLB_ROOT)\
				$(SON_ES_ROOT)\
				$(SON_MIF_ROOT)\
				$(SON_APPS_ROOT)\
				$(SON_NMM_ROOT)\
                $(SON_IMPORT_ROOT)\
                $(SON_RACH_OPT_ROOT)
.DEFAULT_GOAL:= debug

ifeq ($(LINUX_PC_TEST),true)
SON_COM_INCL += -I${TEST_INC_DIR}
COMMON_LIB +=  -L$(TEST_LIB_DIR) -lTest -lgcov
CFLAGS += $(FLAGS) -DTEST_L2_BUILD
endif

all debug release local cross tests common target : create_dir comm_obj COMMON_MODULES $(RELEASE)/lte_son 
$(RELEASE)/lte_son : $(COMM_OBJ) $(LINKLIBS)
ifeq '$(COMPILE_SON_AUT)' 'yes'
	$(CC) -Wall -g $(GCOV_FLAG) $^ -L$(LIBDIR)  -lapps -lnmm -lanr $(LIB_MRO) -lmlb -les -lmif -lx2 -lrach_opt -lsonimport -lrt -lm -lasn1per -lasn1rt -L$(CSPL_BUILD) -lcspl -L$(LIB_SIB_DECODER) -lasndecoder -L$(LIB_UMTS_SIB_DECODER) -lasndecoder_umts -lpthread  $(COMMON_LIB) -o $@ 
else
	$(CC) -Wall -O2  $^ -L$(LIBDIR)  -L$(LIB_IN_MEM_LGR) -lapps -lnmm -lanr $(LIB_MRO) -lmlb -les -lmif -lx2 -lrach_opt -lsonimport -lrt -lm -lasn1per -lasn1rt -L$(CSPL_BUILD) -lcspl -L$(LIB_SIB_DECODER) -lasndecoder -L$(LIB_UMTS_SIB_DECODER) -lasndecoder_umts -linMemLgr -lpthread $(COMMON_LIB) -o $@
endif
	cp $(LTE_SON)/build/release/lte_son $(LTE_ROOT)/../bin
	cp $(LTE_SON)/cfg/son_oam.cfg $(LTE_ROOT)/../cfg
create_dir:
	${MKD} $(SON_BUILD)
	${MKD} $(OBJDIR)
	${MKD} $(LIBDIR)
	${MKD} $(RELEASE)

comm_obj : $(COMM_OBJ)

$(OBJDIR)/%.o : $(SRC_COMM)/%.c 
	$(COMPILE) $^ $(SON_FLAG) $(SON_COM_INCL) -o  $@


$(OBJDIR)/%.o : $(ADVLOG_SRC)/%.c 
	$(COMPILE) $^ $(SON_FLAG) $(SON_COM_INCL) -I$(INCLUDE_RRC)  -o  $@
 

 $(OBJDIR)/%.o : $(lteMisc_SRC)/%.c 
	$(COMPILE) $^ $(SON_FLAG) $(SON_COM_INCL) -I$(INCLUDE_RRM)  -o  $@
       
COMMON_MODULES:
	set -e;
	for i in $(COMMON_SUBDIRS);\
        do \
        echo "Building in $$i"; \
        if ! $(MAKE) target -C $$i; \
        then exit 1; \
        fi; \
        done


##################################################
# Rules for Cleaning the object/library archives #
##################################################

clean : clean_common_module 
	rm -rf $(OBJDIR)/*.o
	rm -rf $(RELEASE)/lte_son
	rm -rf $(LIBDIR)/*.a
	${RMD} $(RELEASE)
	${RMD} $(OBJDIR)
	${RMD} $(LIBDIR)
	${RMD} $(SON_BUILD)
    

clean_common_module :
	for i in $(COMMON_SUBDIRS);\
        do \
        echo "Cleaning in $$i"; \
        if ! $(MAKE_CLEAN) clean -C $$i; \
        then exit 1; \
        fi; \
        done
