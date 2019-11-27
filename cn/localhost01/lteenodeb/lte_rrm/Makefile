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
#       Make file to be used for building rrm_cell.
#
##############################################################################
include ${LTE_ROOT}/make.inc
include ${LTE_RRM}/Makefile.in

# EMBMS_CHANGES_START
ifneq '$(findstring -DLTE_EMBMS_SUPPORTED,$(CFLAGS))' ''
INCLUDE_RRC+=-I$(LTE_ROOT)/lte_rrc/m2ap/utils/inc
INCLUDE_RRC+=-I$(LTE_ROOT)/lte_rrc/m2ap/inc
INCLUDE_RRC+=-I$(LTE_ROOT)/lte_rrc/m2ap/interface/rrm/inc
INCLUDE_RRC+=-I$(LTE_ROOT)/lte_rrc/m2ap/interface/lower_layers/inc/
INCLUDE_RRC_STUB+=-I$(LTE_ROOT)/lte_rrc/rrc/common/include
endif
# EMBMS_CHANGES_END
ifeq ($(RRM_UT_FRAMEWORK), 1)
ifeq ($(NBIOT), 1)
COMM_OBJ += $(OBJDIR)/rrm_main.o 
COMM_OBJ += $(OBJDIR)/rrm_utils.o 
COMM_OBJ += $(OBJDIR)/rrm_shell.o 
COMM_OBJ += $(OBJDIR)/l3_api_compose_parse_utils.o
COMM_OBJ += $(OBJDIR)/l3_api_composer.o
COMM_OBJ += $(OBJDIR)/l3_api_parser.o
COMM_OBJ += $(OBJDIR)/l3_nb_api_parser.o
COMM_OBJ += $(OBJDIR)/l3_nb_api_composer.o
COMM_OBJ += $(OBJDIR)/x2ap_rrm_parser.o
COMM_OBJ += $(OBJDIR)/s1ap_rrm_parser.o
COMM_OBJ += $(OBJDIR)/s1ap_rrm_composer.o
COMM_OBJ += $(OBJDIR)/rrm_rrc_ue_intf.o
COMM_OBJ += $(OBJDIR)/rrm_rrc_cell_intf.o
COMM_OBJ += $(OBJDIR)/rrm_l2_intf.o
COMM_OBJ += $(OBJDIR)/son_rrm_parser.o
COMM_OBJ += $(OBJDIR)/son_rrm_composer.o
COMM_OBJ += $(OBJDIR)/rrm_rrc_meas_config.o
COMM_OBJ += $(OBJDIR)/x2ap_rrm_composer.o
COMM_OBJ += $(OBJDIR)/advlogging.o
COMM_OBJ += $(OBJDIR)/rrm_ut_oam_stub.o
COMM_OBJ += $(OBJDIR)/rrm_ut_rrc_stub.o
COMM_OBJ += $(OBJDIR)/rrm_ut_mac_stub.o
COMM_OBJ += $(OBJDIR)/rrm_ut_son_stub.o
COMM_OBJ += $(OBJDIR)/rrm_ut_pdcp_stub.o
COMM_OBJ += $(OBJDIR)/rrm_ut_stub_db.o
COMM_OBJ += $(OBJDIR)/rrm_ut_suite_db.o
# SPR 20653 Fix Start #
COMM_OBJ += $(OBJDIR)/lteMisc.o
# SPR 20653 Fix Stop #
else
COMM_OBJ += $(OBJDIR)/rrm_main.o 
COMM_OBJ += $(OBJDIR)/rrm_utils.o 
COMM_OBJ += $(OBJDIR)/rrm_shell.o 
COMM_OBJ += $(OBJDIR)/l3_api_compose_parse_utils.o
COMM_OBJ += $(OBJDIR)/l3_api_composer.o
COMM_OBJ += $(OBJDIR)/l3_api_parser.o
COMM_OBJ += $(OBJDIR)/x2ap_rrm_parser.o
COMM_OBJ += $(OBJDIR)/s1ap_rrm_parser.o
COMM_OBJ += $(OBJDIR)/s1ap_rrm_composer.o
COMM_OBJ += $(OBJDIR)/rrm_rrc_ue_intf.o
COMM_OBJ += $(OBJDIR)/rrm_rrc_cell_intf.o
COMM_OBJ += $(OBJDIR)/rrm_l2_intf.o
COMM_OBJ += $(OBJDIR)/son_rrm_parser.o
COMM_OBJ += $(OBJDIR)/son_rrm_composer.o
COMM_OBJ += $(OBJDIR)/rrm_rrc_meas_config.o
COMM_OBJ += $(OBJDIR)/x2ap_rrm_composer.o
COMM_OBJ += $(OBJDIR)/advlogging.o
COMM_OBJ += $(OBJDIR)/rrm_ut_oam_stub.o
COMM_OBJ += $(OBJDIR)/rrm_ut_rrc_stub.o
COMM_OBJ += $(OBJDIR)/rrm_ut_mac_stub.o
COMM_OBJ += $(OBJDIR)/rrm_ut_son_stub.o
COMM_OBJ += $(OBJDIR)/rrm_ut_pdcp_stub.o
COMM_OBJ += $(OBJDIR)/rrm_ut_stub_db.o
COMM_OBJ += $(OBJDIR)/rrm_ut_suite_db.o
# SPR 20653 Fix Start #
COMM_OBJ += $(OBJDIR)/lteMisc.o
# SPR 20653 Fix Stop #
endif
else
ifeq ($(NBIOT), 1)
COMM_OBJ += $(OBJDIR)/rrm_main.o 
COMM_OBJ += $(OBJDIR)/rrm_utils.o 
COMM_OBJ += $(OBJDIR)/rrm_shell.o 
COMM_OBJ += $(OBJDIR)/l3_api_compose_parse_utils.o
COMM_OBJ += $(OBJDIR)/l3_api_composer.o
COMM_OBJ += $(OBJDIR)/l3_api_parser.o
COMM_OBJ += $(OBJDIR)/l3_nb_api_parser.o
COMM_OBJ += $(OBJDIR)/l3_nb_api_composer.o
COMM_OBJ += $(OBJDIR)/x2ap_rrm_parser.o
COMM_OBJ += $(OBJDIR)/s1ap_rrm_parser.o
COMM_OBJ += $(OBJDIR)/s1ap_rrm_composer.o
COMM_OBJ += $(OBJDIR)/rrm_rrc_ue_intf.o
COMM_OBJ += $(OBJDIR)/rrm_rrc_cell_intf.o
COMM_OBJ += $(OBJDIR)/rrm_l2_intf.o
COMM_OBJ += $(OBJDIR)/son_rrm_parser.o
COMM_OBJ += $(OBJDIR)/son_rrm_composer.o
COMM_OBJ += $(OBJDIR)/rrm_rrc_meas_config.o
COMM_OBJ += $(OBJDIR)/x2ap_rrm_composer.o
COMM_OBJ += $(OBJDIR)/advlogging.o
COMM_OBJ += $(OBJDIR)/lteMisc.o
else
COMM_OBJ += $(OBJDIR)/rrm_main.o 
COMM_OBJ += $(OBJDIR)/rrm_utils.o 
COMM_OBJ += $(OBJDIR)/rrm_shell.o 
COMM_OBJ += $(OBJDIR)/l3_api_compose_parse_utils.o
COMM_OBJ += $(OBJDIR)/l3_api_composer.o
COMM_OBJ += $(OBJDIR)/l3_api_parser.o
COMM_OBJ += $(OBJDIR)/x2ap_rrm_parser.o
COMM_OBJ += $(OBJDIR)/s1ap_rrm_parser.o
COMM_OBJ += $(OBJDIR)/s1ap_rrm_composer.o
COMM_OBJ += $(OBJDIR)/rrm_rrc_ue_intf.o
COMM_OBJ += $(OBJDIR)/rrm_rrc_cell_intf.o
COMM_OBJ += $(OBJDIR)/rrm_l2_intf.o
COMM_OBJ += $(OBJDIR)/son_rrm_parser.o
COMM_OBJ += $(OBJDIR)/son_rrm_composer.o
COMM_OBJ += $(OBJDIR)/rrm_rrc_meas_config.o
COMM_OBJ += $(OBJDIR)/x2ap_rrm_composer.o
COMM_OBJ += $(OBJDIR)/advlogging.o
COMM_OBJ += $(OBJDIR)/lteMisc.o
endif
endif
# EMBMS_CHANGES_START
ifneq '$(findstring -DLTE_EMBMS_SUPPORTED,$(CFLAGS))' ''
COMM_OBJ += $(OBJDIR)/rrm_rrc_cell_embms.o
COMM_OBJ += $(OBJDIR)/m2ap_rrm_composer.o
COMM_OBJ += $(OBJDIR)/m2ap_rrm_parser.o
endif
# EMBMS_CHANGES_END

ifeq ($(NBIOT), 1)
COMM_OBJ += $(OBJDIR)/rrm_main.o
COMM_OBJ += $(OBJDIR)/rrm_utils.o
COMM_OBJ += $(OBJDIR)/rrm_shell.o
COMM_OBJ += $(OBJDIR)/l3_api_compose_parse_utils.o
COMM_OBJ += $(OBJDIR)/l3_api_composer.o
COMM_OBJ += $(OBJDIR)/l3_api_parser.o
COMM_OBJ += $(OBJDIR)/l3_nb_api_parser.o
COMM_OBJ += $(OBJDIR)/l3_nb_api_composer.o
COMM_OBJ += $(OBJDIR)/x2ap_rrm_parser.o
COMM_OBJ += $(OBJDIR)/s1ap_rrm_parser.o
COMM_OBJ += $(OBJDIR)/s1ap_rrm_composer.o
COMM_OBJ += $(OBJDIR)/rrm_rrc_ue_intf.o
COMM_OBJ += $(OBJDIR)/rrm_rrc_cell_intf.o
COMM_OBJ += $(OBJDIR)/rrm_l2_intf.o
COMM_OBJ += $(OBJDIR)/son_rrm_parser.o
COMM_OBJ += $(OBJDIR)/son_rrm_composer.o
COMM_OBJ += $(OBJDIR)/rrm_rrc_meas_config.o
COMM_OBJ += $(OBJDIR)/x2ap_rrm_composer.o
COMM_OBJ += $(OBJDIR)/advlogging.o
COMM_OBJ += $(OBJDIR)/lteMisc.o
else
COMM_OBJ += $(OBJDIR)/rrm_main.o 
COMM_OBJ += $(OBJDIR)/rrm_utils.o 
COMM_OBJ += $(OBJDIR)/rrm_shell.o 
COMM_OBJ += $(OBJDIR)/l3_api_compose_parse_utils.o
COMM_OBJ += $(OBJDIR)/l3_api_composer.o
COMM_OBJ += $(OBJDIR)/l3_api_parser.o
COMM_OBJ += $(OBJDIR)/x2ap_rrm_parser.o
COMM_OBJ += $(OBJDIR)/s1ap_rrm_parser.o
COMM_OBJ += $(OBJDIR)/s1ap_rrm_composer.o
COMM_OBJ += $(OBJDIR)/rrm_rrc_ue_intf.o
COMM_OBJ += $(OBJDIR)/rrm_rrc_cell_intf.o
COMM_OBJ += $(OBJDIR)/rrm_l2_intf.o
COMM_OBJ += $(OBJDIR)/son_rrm_parser.o
COMM_OBJ += $(OBJDIR)/son_rrm_composer.o
COMM_OBJ += $(OBJDIR)/rrm_rrc_meas_config.o
COMM_OBJ += $(OBJDIR)/x2ap_rrm_composer.o
COMM_OBJ += $(OBJDIR)/advlogging.o
COMM_OBJ += $(OBJDIR)/lteMisc.o
endif

ifeq ($(RRM_UT_FLAG),1)
STAT_OBJ += $(OBJDIR)/rrm_stat_init.o
STAT_OBJ += $(OBJDIR)/rrmstat_util.o
STAT_OBJ += $(OBJDIR)/rrm_utils.o 
STAT_OBJ += $(OBJDIR)/rrm_rrc_ue_intf.o
STAT_OBJ += $(OBJDIR)/rrm_rrc_cell_intf.o
STAT_OBJ += $(OBJDIR)/rrm_rrc_meas_config.o
STAT_OBJ += $(OBJDIR)/rrm_l2_intf.o
STAT_OBJ += $(OBJDIR)/son_rrm_parser.o
STAT_OBJ += $(OBJDIR)/son_rrm_composer.o
STAT_OBJ += $(OBJDIR)/advlogging.o
STAT_OBJ += $(OBJDIR)/lteMisc.o
endif

COMMON_SUBDIRS = \
		$(RRM_CELL_MGR_ROOT)\
		$(RRM_UE_MGR_ROOT)\
		$(RRM_MIF_ROOT)
       
.DEFAULT_GOAL:= debug

ifeq ($(RRM_UT_FLAG),1)
all debug release local cross tests common target : create_dir comm_obj COMMON_MODULES $(RELEASE)/lte_rrm $(RELEASE)/rrm_stat_util
else
all debug release local cross tests common target : create_dir comm_obj COMMON_MODULES $(RELEASE)/lte_rrm 
endif

ifeq ($(LINUX_PC_TEST), true)
TEST_DEP_MAKE = -L${TEST_LIB_DIR} -lTest -lgcov
endif


ifeq ($(RRM_UT_FRAMEWORK), 1)
$(RELEASE)/lte_rrm : $(COMM_OBJ)    
	${LTE_MAKE_BIN} $^  -L$(ADVLOG_LIB) -ladvlogging -L$(CSPL_BUILD) -L $(LIB_IN_MEM_LGR) -lrt -L$(LIBDIR) -luem -lcellm -lmif -lcspl -lm -lpthread -linMemLgr -lcunit -lgcov -o $@
else
$(RELEASE)/lte_rrm : $(COMM_OBJ)    
	${LTE_MAKE_BIN} $^  -L$(ADVLOG_LIB) -ladvlogging -L$(CSPL_BUILD) -L $(LIB_IN_MEM_LGR) -lrt -L$(LIBDIR) -luem -lcellm -lmif -lcspl -lm -lpthread -linMemLgr -o $@  $(TEST_DEP_MAKE) 
endif

ifeq ($(RRM_UT_FLAG),1)
$(RELEASE)/rrm_stat_util : $(STAT_OBJ)    
	${LTE_MAKE_BIN} $^  -L$(ADVLOG_LIB) -ladvlogging -L$(CSPL_BUILD) -lrt -L$(LIBDIR) -L $(LIB_IN_MEM_LGR) -luem -lcellm -lmif -lcspl -lm -lpthread -linMemLgr -o $@ 
endif

create_dir:
	${MKD} $(OBJDIR)
	${MKD} $(LIBDIR)
	${MKD} $(RELEASE)

comm_obj : $(COMM_OBJ)

$(OBJDIR)/%.o : $(SRC_COMM)/%.c 
	${COMPILE} $^ -I$(INCLUDE_COMM)\
				 -I$(INCLUDE_INTERFACE)\
				 -I$(COMMON_ROOT)/include\
                                 -I$(INCLUDE_UEM)\
 		                 -I$(ADVLOG_INC)\
				 -I$(CSPL_INC)\
				 -I$(CSPL_POSIX)\
				 -I$(INCLUDE_RRC) \
                 -I$(INCLUDE_LLIF)\
                 -I$(INCLUDE_UEM)\
                 -I$(INCLUDE_LGR)\
                 -I$(INCLUDE_CELLM) -I$(LTE_RRM) -o  $@

ifeq ($(RRM_UT_FRAMEWORK), 1)
$(OBJDIR)/%.o : $(SRC_PORT)/%.c 
	${COMPILE} $^ -I$(INCLUDE_COMM)\
        -I$(INCLUDE_LLIF)\
        -I$(INCLUDE_INTERFACE)\
        -I$(INCLUDE_PORT)\
        -I$(INCLUDE_UEM)\
        -I$(ADVLOG_INC)\
        -I$(CSPL_INC)\
        -I$(CSPL_POSIX)\
        -I$(INCLUDE_UEM)\
        -I$(INCLUDE_CELLM)\
        -I$(INCLUDE_LGR)\
        -I$(INCLUDE_COMM_STUB)\
        -I$(INCLUDE_OAM_STUB)\
        -I$(INCLUDE_RRC_STUB)\
        -I$(INCLUDE_L2_STUB)\
        -I$(INCLUDE_SON_STUB)\
        -I$(INCLUDE_PDCP_STUB)\
        -I$(INCLUDE_RRC) -o  $@
else
$(OBJDIR)/%.o : $(SRC_PORT)/%.c 
	${COMPILE} $^ -I$(INCLUDE_COMM)\
        -I$(INCLUDE_LLIF)\
        -I$(INCLUDE_INTERFACE)\
        -I$(INCLUDE_PORT)\
        -I$(INCLUDE_UEM)\
        -I$(ADVLOG_INC)\
        -I$(CSPL_INC)\
        -I$(CSPL_POSIX)\
        -I$(INCLUDE_UEM)\
        -I$(INCLUDE_CELLM)\
        -I$(INCLUDE_LGR)\
        -I$(INCLUDE_RRC) -o  $@
endif


$(OBJDIR)/%.o : $(SRC_INTF)/%.c 
	${COMPILE} $^ -I$(INCLUDE_COMM)\
        -I$(INCLUDE_INTERFACE)\
        -I$(INCLUDE_PORT)\
        -I$(INCLUDE_INTF)\
        -I$(INCLUDE_MULTISECTOR)\
        -I$(INCLUDE_SON)\
        -I$(ADVLOG_INC)\
        -I$(CSPL_INC)\
        -I$(CSPL_POSIX)\
        -I$(INCLUDE_RRC)\
        -I$(INCLUDE_LLIF)\
        -I$(INCLUDE_UEM)\
        -I$(INCLUDE_LGR)\
        -I$(INCLUDE_CELLM) -o $@


$(OBJDIR)/%.o : $(SRC_MULTISECTOR)/%.c
	${COMPILE} $^ -I$(INCLUDE_COMM)\
        -I$(INCLUDE_INTERFACE)\
        -I$(INCLUDE_PORT)\
        -I$(INCLUDE_MULTISECTOR)\
        -I$(INCLUDE_INTF)\
        -I$(INCLUDE_LLIF) \
        -I$(INCLUDE_SON)\
        -I$(ADVLOG_INC)\
        -I$(CSPL_INC)\
        -I$(CSPL_POSIX)\
        -I$(INCLUDE_LGR)\
        -I$(INCLUDE_RRC) -o $@

$(OBJDIR)/%.o : $(SRC_MULTISECTOR)/%.c
	${COMPILE} $^ -I$(INCLUDE_COMM)\
        -I$(INCLUDE_INTERFACE)\
        -I$(INCLUDE_PORT)\
        -I$(INCLUDE_MULTISECTOR)\
        -I$(INCLUDE_INTF)\
        -I$(INCLUDE_LLIF) \
        -I$(INCLUDE_SON)\
        -I$(ADVLOG_INC)\
        -I$(CSPL_INC)\
        -I$(CSPL_POSIX)\
        -I$(INCLUDE_LGR)\
        -I$(INCLUDE_RRC) -o $@

$(OBJDIR)/%.o : $(SRC_LLIF)/%.c 
	${COMPILE} $^ -I$(INCLUDE_COMM)\
        -I$(INCLUDE_PORT)\
        -I$(INCLUDE_INTF)\
        -I$(INCLUDE_MULTISECTOR)\
        -I$(INCLUDE_INTERFACE)\
        -I$(ADVLOG_INC)\
        -I$(CSPL_INC)\
        -I$(CSPL_POSIX)\
        -I$(INCLUDE_RRC)\
        -I$(INCLUDE_LLIF)\
        -I$(INCLUDE_UEM)\
        -I$(INCLUDE_LGR)\
        -I$(INCLUDE_CELLM) -I$(LTE_RRM) -o  $@

$(OBJDIR)/%.o : $(ADVLOG_SRC)/%.c 
	${COMPILE} $^ -I$(INCLUDE_COMM)\
        -I$(INCLUDE_PORT)\
        -I$(INCLUDE_INTF)\
        -I$(INCLUDE_INTERFACE)\
        -I$(CSPL_INC)\
        -I$(CSPL_POSIX)\
        -I$(ADVLOG_INC)\
        -I$(INCLUDE_RRC)\
        -I$(INCLUDE_LLIF)\
        -I$(INCLUDE_UEM)\
        -I$(INCLUDE_LGR)\
        -I$(INCLUDE_CELLM) -I$(LTE_RRM) -o  $@

$(OBJDIR)/%.o : $(MISC_SRC)/%.c 
	${COMPILE} $^ -I$(INCLUDE_COMM)\
        -I$(INCLUDE_PORT)\
        -I$(INCLUDE_INTF)\
        -I$(INCLUDE_INTERFACE)\
        -I$(CSPL_INC)\
        -I$(CSPL_POSIX)\
        -I$(MISC_INC)\
        -I$(INCLUDE_RRC)\
        -I$(INCLUDE_LLIF)\
        -I$(INCLUDE_UEM)\
        -I$(INCLUDE_LGR)\
        -I$(INCLUDE_CELLM) -I$(LTE_RRM) -o  $@

########################################################################
#Compilation of All Stub code
#Oam Stub
#RRC Stub
#MAC Stub
#PDCP Stub
########################################################################
ifeq ($(RRM_UT_FRAMEWORK), 1) 
$(OBJDIR)/%.o : $(SRC_OAM_STUB)/%.c 
	${COMPILE} $^ -I$(INCLUDE_COMM)\
        -I$(INCLUDE_PORT)\
        -I$(INCLUDE_INTF)\
        -I$(INCLUDE_INTERFACE)\
        -I$(CSPL_INC)\
        -I$(CSPL_POSIX)\
        -I$(ADVLOG_INC)\
        -I$(INCLUDE_RRC)\
        -I$(INCLUDE_LLIF)\
        -I$(INCLUDE_UEM)\
        -I$(INCLUDE_LGR)\
        -I$(INCLUDE_CELLM)\
        -I$(LTE_RRM)\
        -I$(INCLUDE_RRC_STUB)\
        -I$(INCLUDE_RRC_STUB_1)\
        -I$(INCLUDE_COMM_STUB)\
        -I$(INCLUDE_OAM_STUB) -o $@
$(OBJDIR)/%.o : $(SRC_RRC_STUB)/%.c 
	${COMPILE} $^ -I$(INCLUDE_COMM)\
        -I$(INCLUDE_PORT)\
        -I$(INCLUDE_INTF)\
        -I$(INCLUDE_INTERFACE)\
        -I$(CSPL_INC)\
        -I$(CSPL_POSIX)\
        -I$(ADVLOG_INC)\
        -I$(INCLUDE_RRC)\
        -I$(INCLUDE_LLIF)\
        -I$(INCLUDE_UEM)\
        -I$(INCLUDE_LGR)\
        -I$(INCLUDE_CELLM)\
        -I$(LTE_RRM)\
        -I$(INCLUDE_SON)\
        -I$(INCLUDE_OAM_STUB)\
        -I$(INCLUDE_RRC_STUB)\
        -I$(INCLUDE_COMM_STUB)\
        -I$(INCLUDE_RRC_STUB_1)  -o $@
$(OBJDIR)/%.o : $(SRC_L2_STUB)/%.c 
	${COMPILE} $^ -I$(INCLUDE_COMM)\
		      -I$(INCLUDE_PORT)\
              -I$(INCLUDE_INTF)\
              -I$(INCLUDE_INTERFACE)\
              -I$(CSPL_INC)\
              -I$(CSPL_POSIX)\
              -I$(ADVLOG_INC)\
              -I$(INCLUDE_RRC)\
              -I$(INCLUDE_LLIF)\
              -I$(INCLUDE_UEM)\
              -I$(INCLUDE_LGR)\
              -I$(INCLUDE_CELLM)\
              -I$(LTE_RRM)\
              -I$(INCLUDE_COMM_STUB)\
              -I$(INCLUDE_L2_STUB)  -o $@
$(OBJDIR)/%.o : $(SRC_SON_STUB)/%.c 
	${COMPILE} $^ -I$(INCLUDE_COMM)\
        -I$(INCLUDE_PORT)\
        -I$(INCLUDE_INTF)\
        -I$(INCLUDE_INTERFACE)\
        -I$(CSPL_INC)\
        -I$(CSPL_POSIX)\
        -I$(ADVLOG_INC)\
        -I$(INCLUDE_RRC)\
        -I$(INCLUDE_LLIF)\
        -I$(INCLUDE_UEM)\
        -I$(INCLUDE_LGR)\
        -I$(INCLUDE_CELLM)\
        -I$(LTE_RRM)\
        -I$(INCLUDE_SON)\
        -I$(INCLUDE_COMM_STUB)\
        -I$(INCLUDE_SON_STUB)  -o $@
$(OBJDIR)/%.o : $(SRC_COMM_STUB)/%.c 
	${COMPILE} $^ -I$(INCLUDE_COMM)\
        -I$(INCLUDE_PORT)\
        -I$(INCLUDE_INTF)\
        -I$(INCLUDE_INTERFACE)\
        -I$(CSPL_INC)\
        -I$(CSPL_POSIX)\
        -I$(ADVLOG_INC)\
        -I$(INCLUDE_RRC)\
        -I$(INCLUDE_LLIF)\
        -I$(INCLUDE_UEM)\
        -I$(INCLUDE_LGR)\
        -I$(INCLUDE_CELLM)\
        -I$(LTE_RRM)\
        -I$(INCLUDE_SON)\
        -I$(INCLUDE_SON_STUB)\
        -I$(INCLUDE_L2_STUB)\
        -I$(INCLUDE_RRC_STUB)\
        -I$(INCLUDE_OAM_STUB)\
        -I$(INCLUDE_COMM_STUB)  -o $@

endif

COMMON_MODULES:
	set -e;
	for i in ${COMMON_SUBDIRS};\
        do \
        echo "Building in $$i"; \
        if ! ${MAKE} target -C $$i; \
        then exit 1; \
        fi; \
        done

##################################################
# Rules for Cleaning the object/library archives #
##################################################

ifeq ($(RRM_UT_FLAG),1)
clean : clean_common_module 
	rm $(OBJDIR)/*.o
	rm $(RELEASE)/lte_rrm
	rm $(RELEASE)/rrm_stat_util
else
clean : clean_common_module 
	rm $(OBJDIR)/*
	rm $(RELEASE)/lte_rrm
endif

clean_common_module :
	for i in ${COMMON_SUBDIRS};\
        do \
        echo "Cleaning in $$i"; \
        if ! ${MAKE_CLEAN} clean -C $$i; \
        then exit 1; \
        fi; \
        done
	${RMD} $(OBJDIR)
	${RMD} $(LIBDIR)
	${RMD} $(RELEASE)
