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
# *  File Description : Makefile for building Uplink HARQ. 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# *
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

ULHARQ_BIN_DIR=${MACCORE_BIN_DIR}/assemblyDeassemblyUnit/ulHarq

SRCDIR= ${ASSEMBLY_DEASSEMBLY_ROOT}/ulHarq

OBJS= ${ULHARQ_BIN_DIR}/lteMacULHarqMgr.o 

target: create_dirs OBJECTS 

create_dirs:
	${MKDIR} ${ULHARQ_BIN_DIR}	

OBJECTS: ${OBJS}

clean:
	${RM} ${OBJS}

#-------------------- Some rules. -------------#
${ULHARQ_BIN_DIR}/%.o : ${SRCDIR}/%.c
	${COMPILE} $< -c -o $@
