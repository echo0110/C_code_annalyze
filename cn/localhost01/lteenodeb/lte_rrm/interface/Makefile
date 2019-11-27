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
include ${LTE_ROOT}/make.inc
include ${LTE_RRM}/Makefile.in

# EMBMS_CHANGES_START
ifneq '$(findstring -DLTE_EMBMS_SUPPORTED,$(CFLAGS))' ''
INCLUDE_RRC+=-I$(LTE_ROOT)/lte_rrc/m2ap/utils/inc
INCLUDE_RRC+=-I$(LTE_ROOT)/lte_rrc/m2ap/inc
INCLUDE_RRC+=-I$(LTE_ROOT)/lte_rrc/m2ap/interface/rrm/inc
endif
# EMBMS_CHANGES_END

SRCDIR = ${RRM_MIF_ROOT}/src
INCDIR = ${RRM_MIF_ROOT}/include


RRM_MIF_INC  = -I${INCDIR}
RRM_MIF_INC += -I${INCLUDE_COMM}
RRM_MIF_INC += -I${CSPL_INC}
RRM_MIF_INC += -I${CSPL_POSIX}
RRM_MIF_INC += -I${INCLUDE_INTF}

ifeq ($(RRM_UT_FLAG), 1)
ifeq ($(NBIOT), 1) 
MIF_OBJ = ${OBJDIR}/rrm_mif_fsm.o\
	${OBJDIR}/rrm_son_parser.o\
	${OBJDIR}/rrm_son_composer.o\
	${OBJDIR}/rrm_mif_msg_handler.o\
	${OBJDIR}/rrm_mif_interface.o\
	${OBJDIR}/rrm_mif_db.o\
	${OBJDIR}/rrm_oam_parser.o\
	${OBJDIR}/rrm_oamNb_parser.o\
	${OBJDIR}/rrm_oam_composer.o\
	${OBJDIR}/rrm_oamNb_composer.o\
	${OBJDIR}/rrm_pack_unpack_utils.o \
	${OBJDIR}/rrm_stat_init.o \
	${OBJDIR}/rrm_oam_platform_parser.o \
	${OBJDIR}/rrmstat_util.o 
else
MIF_OBJ = ${OBJDIR}/rrm_mif_fsm.o\
	${OBJDIR}/rrm_son_parser.o\
	${OBJDIR}/rrm_son_composer.o\
	${OBJDIR}/rrm_mif_msg_handler.o\
	${OBJDIR}/rrm_mif_interface.o\
	${OBJDIR}/rrm_mif_db.o\
	${OBJDIR}/rrm_oam_parser.o\
	${OBJDIR}/rrm_oam_composer.o\
	${OBJDIR}/rrm_pack_unpack_utils.o \
	${OBJDIR}/rrm_stat_init.o \
	${OBJDIR}/rrm_oam_platform_parser.o \
	${OBJDIR}/rrmstat_util.o 
endif
else
ifeq ($(NBIOT), 1) 
MIF_OBJ = ${OBJDIR}/rrm_mif_fsm.o\
	${OBJDIR}/rrm_son_parser.o\
	${OBJDIR}/rrm_son_composer.o\
	${OBJDIR}/rrm_mif_msg_handler.o\
	${OBJDIR}/rrm_mif_interface.o\
	${OBJDIR}/rrm_mif_db.o\
	${OBJDIR}/rrm_oam_parser.o\
	${OBJDIR}/rrm_oamNb_parser.o\
	${OBJDIR}/rrm_oam_composer.o\
	${OBJDIR}/rrm_oamNb_composer.o\
	${OBJDIR}/rrm_pack_unpack_utils.o \
	${OBJDIR}/rrm_oam_platform_parser.o 
else
MIF_OBJ = ${OBJDIR}/rrm_mif_fsm.o\
	${OBJDIR}/rrm_son_parser.o\
	${OBJDIR}/rrm_son_composer.o\
	${OBJDIR}/rrm_mif_msg_handler.o\
	${OBJDIR}/rrm_mif_interface.o\
	${OBJDIR}/rrm_mif_db.o\
	${OBJDIR}/rrm_oam_parser.o\
	${OBJDIR}/rrm_oam_composer.o\
	${OBJDIR}/rrm_pack_unpack_utils.o \
	${OBJDIR}/rrm_oam_platform_parser.o 
endif    
endif    

# EMBMS_CHANGES_START
ifneq '$(findstring -DLTE_EMBMS_SUPPORTED,$(CFLAGS))' ''
MIF_OBJ += ${OBJDIR}/rrm_mif_embms.o
endif
# EMBMS_CHANGES_END

target : ${LIBDIR}/libmif.a

${LIBDIR}/libmif.a : ${MIF_OBJ}
	ar rv $@ $^

ifeq ($(RRM_UT_FRAMEWORK), 1) 
${OBJDIR}/%.o : ${SRCDIR}/%.c
	${COMPILE}  $< -I${INCLUDE_INTERFACE}\
                       -I$(INCLUDE_INTF)\
                       -I$(INCLUDE_UEM)\
  	               -I${INCLUDE_COMM} \
  		       -I${ADVLOG_INC} \
		       -I${CSPL_INC}\
		       -I${CSPL_POSIX}\
                       -I$(INCLUDE_SON)\
		       -I${INCLUDE_RRC} \
		       -I${INCLUDE_LGR}\
                       -I$(INCLUDE_LLIF)\
                       -I$(INCLUDE_UEM)\
                       -I$(INCLUDE_SON_STUB)\
                       -I$(INCLUDE_CELLM) -I$(LTE_RRM) -o  $@ 
else
${OBJDIR}/%.o : ${SRCDIR}/%.c
	${COMPILE}  $< -I${INCLUDE_INTERFACE}\
                       -I$(INCLUDE_INTF)\
                       -I$(INCLUDE_UEM)\
  		       -I${INCLUDE_COMM} \
  		       -I${ADVLOG_INC} \
		       -I${CSPL_INC}\
		       -I${CSPL_POSIX}\
                       -I$(INCLUDE_SON)\
		       -I${INCLUDE_RRC} \
		       -I${INCLUDE_LGR}\
                       -I$(INCLUDE_LLIF)\
                       -I$(INCLUDE_UEM)\
                       -I$(INCLUDE_CELLM) -I$(LTE_RRM) -o  $@ 
endif

clean:
	rm  ${LIBDIR}/libmif.a

            
