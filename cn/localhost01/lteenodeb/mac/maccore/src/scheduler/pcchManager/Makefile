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
# *  File Description : Makefile for building PCCH Modules. 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.1  2009/05/06 03:50:30  gur11912
# * Initial Version
# *
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

PCCH_BIN_DIR=${MACCORE_BIN_DIR}/scheduler/pcchManager

SRCDIR= ${SCHEDULER_ROOT}/pcchManager

OBJS= ${PCCH_BIN_DIR}/lteMacPCCH.o

target: create_dirs OBJECTS 

create_dirs:
	${MKDIR} ${PCCH_BIN_DIR}

OBJECTS:${OBJS}

clean:
	${RM} ${OBJS}

#-------------------- Some rules. -------------#
${PCCH_BIN_DIR}/%.o : ${SRCDIR}/%.c
	${COMPILE} $< -c -o $@
