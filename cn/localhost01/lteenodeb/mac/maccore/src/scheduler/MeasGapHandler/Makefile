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
# *  File Description : Makefile for building Meas Gap Handler. 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

STRATEGY_BIN_DIR=${MACCORE_BIN_DIR}/scheduler/MeasGapHandler

SRCDIR= ${SCHEDULER_ROOT}/MeasGapHandler

OBJS= ${STRATEGY_BIN_DIR}/lteMacMeasGapHandler.o

target: create_dirs OBJECTS

create_dirs:
	${MKDIR} ${STRATEGY_BIN_DIR}

OBJECTS:${OBJS}

clean:
	${RM} ${OBJS}

#-------------------- Some rules. -------------#
${STRATEGY_BIN_DIR}/%.o : ${SRCDIR}/%.c
	${COMPILE} $< -c -o $@
