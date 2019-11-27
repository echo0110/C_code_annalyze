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
# *  File Description : Makefile for building Resource Manager Modules. 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.4  2009/06/05 07:01:07  gur19413
# * Removed Compilation Errors
# *
# * Revision 1.3  2009/06/01 07:11:36  gur18569
# * added lteMacULResourceAllocator.c
# *
# * Revision 1.2  2009/05/25 10:53:08  gur12140
# * RAT 2 Localized and RAT 2 Distributed Code Updated
# *
# * Revision 1.1  2009/05/06 03:51:48  gur11912
# * Initial Version
# *
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

RESOURCE_MANAGER_BIN_DIR=${MACCORE_BIN_DIR}/scheduler/resourceManager

SRCDIR= ${SCHEDULER_ROOT}/resourceManager

OBJS= ${RESOURCE_MANAGER_BIN_DIR}/lteMacDLResourceAllocator.o  \
	  ${RESOURCE_MANAGER_BIN_DIR}/lteMacULResourceAllocator.o \
	  ${RESOURCE_MANAGER_BIN_DIR}/lteMacDLResourceInfoMgr.o \
	  ${RESOURCE_MANAGER_BIN_DIR}/lteMacULResourceInfoMgr.o


target: create_dirs OBJECTS 

create_dirs:
	${MKDIR} ${RESOURCE_MANAGER_BIN_DIR}	

OBJECTS: ${OBJS}

clean:
	${RM} ${OBJS}

#-------------------- Some rules. -------------#
${RESOURCE_MANAGER_BIN_DIR}/%.o : ${SRCDIR}/%.c
	${COMPILE} $< -c -o $@
