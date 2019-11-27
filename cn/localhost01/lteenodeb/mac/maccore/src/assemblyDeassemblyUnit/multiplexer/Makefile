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
# *  File Description : Makefile for building Multiplexer. 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.2  2009/06/08 15:44:45  gur12140
# * Removed Linking Errors
# *
# * Revision 1.1  2009/05/06 03:44:43  gur11912
# * Initial Version
# *
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

MULTIPLEXER_BIN_DIR=${MACCORE_BIN_DIR}/assemblyDeassemblyUnit/multiplexer

SRCDIR= ${ASSEMBLY_DEASSEMBLY_ROOT}/multiplexer

OBJS= ${MULTIPLEXER_BIN_DIR}/lteMacMultiplexer.o

ifeq ($(LTE_EMBMS_FLAG),1)
OBJS += ${MULTIPLEXER_BIN_DIR}/lteMacEmbmsMultiplexer.o	
endif	

target: create_dirs OBJECTS 

create_dirs:
	${MKDIR} ${MULTIPLEXER_BIN_DIR}	

OBJECTS: ${OBJS}

clean:
	${RM} ${OBJS}

#-------------------- Some rules. -------------#
${MULTIPLEXER_BIN_DIR}/%.o : ${SRCDIR}/%.c
	${COMPILE} $< -c -o $@ -I${LTE_ROOT}/simulators/trafficGenerator/include/
