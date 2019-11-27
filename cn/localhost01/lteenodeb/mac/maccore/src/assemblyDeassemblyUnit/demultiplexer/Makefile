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
# *  File Description : Makefile for building Demultiplexer. 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.1  2009/05/06 03:42:43  gur11912
# * Initial Version
# *
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

DEMULTIPLEXER_BIN_DIR=${MACCORE_BIN_DIR}/assemblyDeassemblyUnit/demultiplexer

SRCDIR= ${ASSEMBLY_DEASSEMBLY_ROOT}/demultiplexer

OBJS= ${DEMULTIPLEXER_BIN_DIR}/lteMacDemux.o

target: create_dirs OBJECTS 

create_dirs:
	${MKDIR} ${DEMULTIPLEXER_BIN_DIR}	

OBJECTS: ${OBJS}

clean:
	${RM} ${OBJS}

#-------------------- Some rules. -------------#
${DEMULTIPLEXER_BIN_DIR}/%.o : ${SRCDIR}/%.c
	${COMPILE} $< -c -o $@
