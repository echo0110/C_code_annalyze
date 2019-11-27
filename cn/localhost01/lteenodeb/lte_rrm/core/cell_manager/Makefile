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
#              CELLM Makefile 
#       Make file to be used for building rrm_uem library.
#*****************************************************************************

#*****************************************************************************
#	INCLUDE the root vob directory path                    
#   INCLUDE RRM releated PATH                              
#*****************************************************************************
include ${LTE_ROOT}/make.inc
include ${LTE_RRM}/Makefile.in

# EMBMS_CHANGES_START
ifneq '$(findstring -DLTE_EMBMS_SUPPORTED,$(CFLAGS))' ''
INCLUDE_RRC+=-I$(LTE_ROOT)/lte_rrc/m2ap/utils/inc
INCLUDE_RRC+=-I$(LTE_ROOT)/lte_rrc/m2ap/inc
INCLUDE_RRC+=-I$(LTE_ROOT)/lte_rrc/m2ap/interface/rrm/inc
endif
# EMBMS_CHANGES_END

SRCDIR = ${RRM_CELL_MGR_ROOT}/src
COMMON_SRCDIR = ${LTE_ROOT}/lte_rrm/common/src/
INCDIR = ${RRM_CELL_MGR_ROOT}/include
COMMON_INCDIR = ${LTE_ROOT}/lte_rrm/common/include
TMP_INCDIR = ${LTE_ROOT}/lte_rrm

RRM_CELLM_INC =  -I${TMP_INCDIR}
RRM_CELLM_INC += -I${INCDIR}
RRM_CELLM_INC += -I${INCLUDE_LLIF}
RRM_CELLM_INC += -I${INCLUDE_COMM}
RRM_CELLM_INC += -I${INCLUDE_INTF}
RRM_CELLM_INC += -I${INCLUDE_MULTISECTOR}
RRM_CELLM_INC += -I${INCLUDE_INTERFACE}
RRM_CELLM_INC += -I${CSPL_INC}
RRM_CELLM_INC += -I${CSPL_POSIX}
RRM_CELLM_INC += -I${COMMON_INCDIR}
RRM_CELLM_INC += -I${ADVLOG_INC}
RRM_CELLM_INC += -I${INCLUDE_UEM}
RRM_CELLM_INC += -I${INCLUDE_LGR}

ifeq ($(NBIOT), 0)
CELLM_OBJ = 	$(OBJDIR)/rrm_utils.o\
		${OBJDIR}/rrm_cell_context.o\
		${OBJDIR}/rrm_cell_mgr.o\
		${OBJDIR}/rrm_cell_fsm.o\
		${OBJDIR}/l3_api_composer.o\
		${OBJDIR}/l3_api_compose_parse_utils.o\
		${OBJDIR}/l3_api_parser.o\
		${OBJDIR}/x2ap_rrm_parser.o\
        $(OBJDIR)/s1ap_rrm_composer.o\
        $(OBJDIR)/s1ap_rrm_parser.o\
		${OBJDIR}/rrm_rrc_cell_intf.o\
        ${OBJDIR}/x2ap_rrm_composer.o
else
CELLM_OBJ = 	$(OBJDIR)/rrm_utils.o\
		${OBJDIR}/rrm_cell_context.o\
		${OBJDIR}/rrm_cell_mgr.o\
		${OBJDIR}/rrm_cell_fsm.o\
	${OBJDIR}/rrm_nb_cell_fsm.o\
		${OBJDIR}/l3_api_composer.o\
	${OBJDIR}/l3_nb_api_composer.o\
		${OBJDIR}/l3_api_compose_parse_utils.o\
	${OBJDIR}/l3_nb_api_parser.o\
		${OBJDIR}/l3_api_parser.o\
		${OBJDIR}/x2ap_rrm_parser.o\
        $(OBJDIR)/s1ap_rrm_composer.o\
        $(OBJDIR)/s1ap_rrm_parser.o\
		${OBJDIR}/rrm_rrc_cell_intf.o\
        ${OBJDIR}/x2ap_rrm_composer.o
endif

# EMBMS_CHANGES_START
ifneq '$(findstring -DLTE_EMBMS_SUPPORTED,$(CFLAGS))' ''

CELLM_OBJ +=    $(OBJDIR)/m2ap_rrm_composer.o
CELLM_OBJ +=    $(OBJDIR)/m2ap_rrm_parser.o

CELLM_OBJ +=    $(OBJDIR)/rrm_cell_embms.o
CELLM_OBJ +=    $(OBJDIR)/rrm_rrc_cell_embms.o
endif
# EMBMS_CHANGES_END
$(BIN_DIR)cellm : $(CELLM_OBJ)

${OBJDIR}/%.o :  ${SRCDIR}/%.c
	${COMPILE}   ${RRM_CELLM_INC} ${INCLUDE_RRC} -o $@ $<

target : ${LIBDIR}/libcellm.a $(BIN_DIR)cellm

${LIBDIR}/libcellm.a : ${CELLM_OBJ}
	ar rv $@ $^

#${OBJDIR}/%.o : ${SRCDIR}/%.c 
#	${COMPILE}   ${RRM_CELLM_INC} ${INCLUDE_RRC}  -o $@ $<


#$(BIN_DIR)cellm:
#	gcc -o $(BIN_DIR)cellm $(CELLM_OBJ) -L -lcspl

clean:
#rm  ${MIF_OBJ}
	rm  ${LIBDIR}/libcellm.a
	#rm  ${OBJDIR}/*.o
