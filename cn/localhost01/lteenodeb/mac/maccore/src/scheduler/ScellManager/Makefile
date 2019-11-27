#/******************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (C) 2014 Aricent Inc. All Rights Reserved.
# *
# ******************************************************************************
# *
# *  $Id: Makefile,v 1.0 2014/02/24 19:12:02 gur29743 Exp $
# *
# ******************************************************************************
# *
# *  File Description : Makefile for building Scell state machine files. 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Revision v 1.0 2014/02/24 19:12:02 gur29743 
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

SCELL_MANAGER_BIN_DIR=${MACCORE_BIN_DIR}/scheduler/ScellManager

SRCDIR= ${SCHEDULER_ROOT}/ScellManager

OBJS= ${SCELL_MANAGER_BIN_DIR}/lteMacDLScellStateManager.o 

target: create_dirs OBJECTS

create_dirs:
	${MKDIR} ${SCELL_MANAGER_BIN_DIR}

OBJECTS:${OBJS}

clean:
	${RM} ${OBJS}

#-------------------- Some rules. -------------#
${SCELL_MANAGER_BIN_DIR}/%.o : ${SRCDIR}/%.c
	${COMPILE} $< -c -o $@
