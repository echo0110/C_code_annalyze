#/****************************************************************************** # *
# *  ARICENT -
# *
# *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
# *
# ******************************************************************************
# *
# *  $Id: Makefile,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
# *
# ******************************************************************************
# *
# *  File Description : Makefile for building RRC Interfaces Modules. 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.2  2009/06/05 15:16:19  gur18569
# * removed sfn obj
# *
# * Revision 1.1  2009/05/06 03:47:32  gur11912
# * Initial Version
# *
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

RRC_BIN_DIR=${MACCORE_BIN_DIR}/interfaces/rrc

SRCDIR= ${INTERFACES_ROOT}/rrc

OBJS= ${RRC_BIN_DIR}/lteMacCellConfMgr.o  \
      ${RRC_BIN_DIR}/lteMacUEConfMgr.o ${RRC_BIN_DIR}/lteMacParseUtil.o 

ifeq ($(LTE_EMBMS_FLAG),1)
OBJS += $(RRC_BIN_DIR)/lteMacEmbmsUtil.o
endif

target: create_dirs OBJECTS 

create_dirs:
	${MKDIR} ${RRC_BIN_DIR}		

OBJECTS: ${OBJS}

clean:
	${RM} ${OBJS}

#-------------------- Some rules. -------------#
${RRC_BIN_DIR}/%.o : ${SRCDIR}/%.c
	${COMPILE} $< -c -o $@
