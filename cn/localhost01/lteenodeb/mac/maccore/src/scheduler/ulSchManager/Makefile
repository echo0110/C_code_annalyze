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
# *  File Description : Makefile for building UL-SCH Modules. 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.1  2009/05/06 03:52:24  gur11912
# * Initial Version
# *
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

ULSCH_BIN_DIR=${MACCORE_BIN_DIR}/scheduler/ulSchManager

SRCDIR= ${SCHEDULER_ROOT}/ulSchManager

OBJS= ${ULSCH_BIN_DIR}/lteMacPHICH.o ${ULSCH_BIN_DIR}/lteMacPUCCH.o \
      ${ULSCH_BIN_DIR}/lteMacPUSCH.o ${ULSCH_BIN_DIR}/lteMacULQueueLoad.o \
      ${ULSCH_BIN_DIR}/lteMacULCSIManager.o ${ULSCH_BIN_DIR}/lteMacPuschPowerControl.o \
      ${ULSCH_BIN_DIR}/lteMacSRSMgr.o \
	  ${ULSCH_BIN_DIR}/lteMacSpsPUSCHMgr.o

target: create_dirs OBJECTS 

create_dirs:
	${MKDIR} ${ULSCH_BIN_DIR}	

OBJECTS: ${OBJS}

clean:
	${RM} ${OBJS}

#-------------------- Some rules. -------------#
${ULSCH_BIN_DIR}/%.o : ${SRCDIR}/%.c
	${COMPILE} $< -c -o $@
