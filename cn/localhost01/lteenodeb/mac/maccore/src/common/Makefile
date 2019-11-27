#/******************************************************************************
# *
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
# *  File Description : Makefile for building Common 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.4  2009/06/26 05:05:40  gur19413
# * remove lteMacUEContext.c
# *
# * Revision 1.3  2009/06/05 07:48:17  gur18569
# * added more files to compile
# *
# * Revision 1.2  2009/05/20 13:38:23  gur19413
# * Initial Version
# *
# * Revision 1.1  2009/05/06 03:45:17  gur11912
# * Initial Version
# *
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

COMMON_BIN_DIR=${MACCORE_BIN_DIR}/common

SRCDIR= ${MACCORE_COMMON_ROOT}

OBJS= ${COMMON_BIN_DIR}/lteMacTBSize.o  \
      ${COMMON_BIN_DIR}/lteMacUtilityAndSharedStructs.o\
	  ${COMMON_BIN_DIR}/lteMacTDDConfig.o\
	  ${COMMON_BIN_DIR}/lteMacPhyCellReconfig.o \
	  ${COMMON_BIN_DIR}/lteMacULPowerControlData.o \
	  ${COMMON_BIN_DIR}/lteMacDLPowerControlData.o

target: create_dirs OBJECTS

create_dirs:
	${MKDIR} ${COMMON_BIN_DIR}

OBJECTS: ${OBJS}

clean:
	${RM} ${OBJS}

#-------------------- Some rules. -------------#
${COMMON_BIN_DIR}/%.o : ${SRCDIR}/%.c
	${COMPILE} $< -c -o $@
