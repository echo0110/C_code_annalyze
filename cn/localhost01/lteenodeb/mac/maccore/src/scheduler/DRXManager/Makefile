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
# *  File Description : Makefile for building Strategy Modules. 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.5  2009/05/26 10:16:04  gur20052
# * Updated to incorporated UE Context Changes
# *
# * Revision 1.4  2009/05/25 16:20:01  gur20052
# * Updated the version to support the UL Fair and Round Robin Scheduler
# *
# * Revision 1.3  2009/05/21 17:22:46  gur20052
# * Updated the Basic Framework of UL Strategy
# *
# * Revision 1.2  2009/05/11 06:18:32  gur20052
# * Removed compilation errors
# *
# * Revision 1.1  2009/05/06 03:52:10  gur11912
# * Initial Version
# *
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

STRATEGY_BIN_DIR=${MACCORE_BIN_DIR}/scheduler/DRXManager

SRCDIR= ${SCHEDULER_ROOT}/DRXManager

OBJS= ${STRATEGY_BIN_DIR}/lteMacDRXMgr.o

target: create_dirs OBJECTS

create_dirs:
	${MKDIR} ${STRATEGY_BIN_DIR}

OBJECTS:${OBJS}

clean:
	${RM} ${OBJS}

#-------------------- Some rules. -------------#
${STRATEGY_BIN_DIR}/%.o : ${SRCDIR}/%.c
	${COMPILE} $< -c -o $@
