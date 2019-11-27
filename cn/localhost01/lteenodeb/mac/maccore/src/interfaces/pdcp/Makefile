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
# *  File Description : Makefile for building PDCP Interfaces Modules. 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# *
# * Revision 1.1  2012/10/30 03:47:49  gur26313
# * Initial Version
# *
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

PDCP_BIN_DIR=${MACCORE_BIN_DIR}/interfaces/pdcp

SRCDIR= ${INTERFACES_ROOT}/pdcp

OBJS= ${PDCP_BIN_DIR}/lteMacPdcpInterface.o 

target: create_dirs OBJECTS 

create_dirs:
	${MKDIR} ${PDCP_BIN_DIR}		

OBJECTS: ${OBJS}

clean:
	${RM} ${OBJS}

#-------------------- Some rules. -------------#
${PDCP_BIN_DIR}/%.o : ${SRCDIR}/%.c
	${COMPILE} $< -c -o $@

