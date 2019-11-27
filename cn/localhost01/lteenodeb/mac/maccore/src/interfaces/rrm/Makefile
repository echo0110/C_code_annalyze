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
# *  File Description : Makefile for building RRM Interfaces Modules. 
# *
# ******************************************************************************
# *
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

RRM_BIN_DIR=${MACCORE_BIN_DIR}/interfaces/rrm

SRCDIR= ${INTERFACES_ROOT}/rrm

OBJS= ${RRM_BIN_DIR}/lteMacParseRRMMsg.o  \
      ${RRM_BIN_DIR}/lteMacRRMInterface.o ${RRM_BIN_DIR}/lteMacRRMIpcPort.o \
	  ${RRM_BIN_DIR}/lteMacRRMPeriodicMgr.o
     

target: create_dirs OBJECTS 

create_dirs:
	${MKDIR} ${RRM_BIN_DIR}		

OBJECTS: ${OBJS}

clean:
	${RM} ${OBJS}

#-------------------- Some rules. -------------#
${RRM_BIN_DIR}/%.o : ${SRCDIR}/%.c
	${COMPILE} $< -c -o $@
