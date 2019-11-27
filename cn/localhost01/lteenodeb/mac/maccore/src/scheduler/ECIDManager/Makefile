#/******************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (C) 2013 Aricent Inc. All Rights Reserved.
# *
# ******************************************************************************
# *
# *  $Id: Makefile, Exp $
# *
# ******************************************************************************
# *
# *  File Description : Makefile for building Strategy Modules.
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Initial Version
# *
# *
# *****************************************************************************/
 include ${LTE_ROOT}/make.inc
 include ${MACCORE_ROOT}/mac.inc

 STRATEGY_BIN_DIR=${MACCORE_BIN_DIR}/scheduler/ECIDManager

 SRCDIR= ${SCHEDULER_ROOT}/ECIDManager

 OBJS= ${STRATEGY_BIN_DIR}/lteMacECIDMgr.o

target: create_dirs OBJECTS

create_dirs:
	${MKDIR} ${STRATEGY_BIN_DIR}

OBJECTS:${OBJS}

clean:
	${RM} ${OBJS}

         #-------------------- Some rules. -------------#
${STRATEGY_BIN_DIR}/%.o : ${SRCDIR}/%.c
	    ${COMPILE} $< -c -o $@


