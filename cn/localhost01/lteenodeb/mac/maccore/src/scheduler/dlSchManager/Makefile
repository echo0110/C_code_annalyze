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
# *  File Description : Makefile for building DL-SCH Modules. 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.5  2009/06/05 11:10:27  gur12140
# * Removed Linking Erros
# *
# * Revision 1.4  2009/06/01 06:43:31  gur18569
# *  lteMacPDCCHMgr.o lteMacDLQueueLoadMgr.o compilation was commented , un commenting them
# *
# * Revision 1.3  2009/05/11 09:26:08  gur20052
# * Reveiw Comments are incorporated
# *
# * Revision 1.2  2009/05/06 09:40:08  gur12140
# * File Updated
# *
# * Revision 1.1  2009/05/06 03:50:09  gur11912
# * Initial Version
# *
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

DLSCH_BIN_DIR=${MACCORE_BIN_DIR}/scheduler/dlSchManager

SRCDIR= ${SCHEDULER_ROOT}/dlSchManager
OBJS= ${DLSCH_BIN_DIR}/lteMacPDCCHMgr.o  ${DLSCH_BIN_DIR}/lteMacDLQueueLoadMgr.o \
     ${DLSCH_BIN_DIR}/lteMacPDSCHMgr.o ${DLSCH_BIN_DIR}/lteMacCQI.o \
	  ${DLSCH_BIN_DIR}/lteMacPucchPowerControl.o ${DLSCH_BIN_DIR}/lteMacSpsPDSCHMgr.o

target: create_dirs OBJECTS 

create_dirs:
	${MKDIR} ${DLSCH_BIN_DIR}	

OBJECTS: ${OBJS}

clean:
	${RM} ${OBJS}

#-------------------- Some rules. -------------#
${DLSCH_BIN_DIR}/%.o : ${SRCDIR}/%.c
	${COMPILE} $< -c -o $@
