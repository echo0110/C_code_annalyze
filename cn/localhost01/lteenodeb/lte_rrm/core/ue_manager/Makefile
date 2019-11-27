#/****************************************************************************
# *  ARICENT -
# *  Copyright (c) Aricent.
# ****************************************************************************
# *  $Id: Makefile,v 1.0 2012/01/17 12:13:41 gur26991 Exp $
# ****************************************************************************
# *  File Description : Makefile
# ****************************************************************************
# * Revision Details
# * ----------------
# *
# *
# ****************************************************************************/
#*****************************************************************************
#              UEM Makefile 
#       Make file to be used for building rrm_uem library.
#*****************************************************************************

#*****************************************************************************
#	INCLUDE the root vob directory path                    
#   INCLUDE RRM releated PATH                              
#*****************************************************************************
include ${LTE_ROOT}/make.inc
include ${LTE_RRM}/Makefile.in
#*****************************************************************************
#	UEM Source Directory path                              
#*****************************************************************************

# EMBMS_CHANGES_START
ifneq '$(findstring -DLTE_EMBMS_SUPPORTED,$(CFLAGS))' ''
INCLUDE_RRC+=-I$(LTE_ROOT)/lte_rrc/m2ap/utils/inc
INCLUDE_RRC+=-I$(LTE_ROOT)/lte_rrc/m2ap/inc
INCLUDE_RRC+=-I$(LTE_ROOT)/lte_rrc/m2ap/interface/rrm/inc
endif
# EMBMS_CHANGES_END

SRCDIR = ${RRM_UE_MGR_ROOT}/src
ifeq ($(NBIOT),0)
UEM_OBJ=${OBJDIR}/rrm_utils.o\
        ${OBJDIR}/rrm_event_handler.o\
	$(OBJDIR)/rrm_event_composer.o\
	${OBJDIR}/rrm_ue_fsm.o\
	${OBJDIR}/rrm_ue_mgr.o\
	${OBJDIR}/rrm_ue_ho_mgr.o\
	${OBJDIR}/rrm_ue_rac.o\
	${OBJDIR}/l3_api_compose_parse_utils.o\
	${OBJDIR}/l3_api_composer.o\
	${OBJDIR}/l3_api_parser.o\
	${OBJDIR}/rrm_ue_ho_utils.o
else
UEM_OBJ=${OBJDIR}/rrm_utils.o\
        ${OBJDIR}/rrm_event_handler.o\
	$(OBJDIR)/rrm_event_composer.o\
	${OBJDIR}/rrm_ue_fsm.o\
	${OBJDIR}/rrm_nb_ue_fsm.o\
	${OBJDIR}/rrm_ue_mgr.o\
	${OBJDIR}/rrm_ue_ho_mgr.o\
	${OBJDIR}/rrm_ue_rac.o\
	${OBJDIR}/l3_api_compose_parse_utils.o\
	${OBJDIR}/l3_api_composer.o\
	${OBJDIR}/l3_api_parser.o\
	${OBJDIR}/rrm_ue_ho_utils.o
endif

# EMBMS_CHANGES_START
ifneq '$(findstring -DLTE_EMBMS_SUPPORTED,$(CFLAGS))' ''
UEM_OBJ+=${OBJDIR}/rrm_ue_embms.o
endif
# EMBMS_CHANGES_END

target : ${LIBDIR}/libuem.a

${LIBDIR}/libuem.a : ${UEM_OBJ}
	ar rv $@ $^

${OBJDIR}/%.o : ${SRCDIR}/%.c 
	${COMPILE} $<	-I${INCLUDE_LLIF}\
					-I${INCLUDE_COMM}\
					-I${INCLUDE_INTF}\
					-I${INCLUDE_MULTISECTOR}\
					-I${INCLUDE_INTERFACE}\
					-I${ADVLOG_INC}\
					-I${CSPL_INC}\
					-I${CSPL_POSIX}\
					-I${INCLUDE_RRC}\
					-I${INCLUDE_CELLM}\
					-I${INCLUDE_UEM}\
					-I${INCLUDE_LGR}\
					-I${LTE_RRM} -o $@


clean:
	rm  ${LIBDIR}/libuem.a
	make -C $(LTE_ROOT)/lte_rrc/tools/advlogger distclean

