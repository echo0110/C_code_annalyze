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
# *  File Description : Makefile for building OAM Interfaces Modules. 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.4  2009/06/09 07:21:39  gur20470
# * updated for mac1.2
# *
# * Revision 1.3  2009/06/07 08:46:34  gur20470
# * updated for mac 1.2
# *
# * Revision 1.2  2009/06/01 07:42:15  gur18569
# * added files to compile
# *
# * Revision 1.1  2009/05/06 03:48:23  gur11912
# * Initial Version
# *
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

OAM_BIN_DIR=${MACCORE_BIN_DIR}/interfaces/oam

SRCDIR= ${INTERFACES_ROOT}/oam

OBJS= ${OAM_BIN_DIR}/lteMacOAMInterface.o  \
      ${OAM_BIN_DIR}/lteMacParseOAMMsg.o  \
      $(OAM_BIN_DIR)/lteMacStatsManager.o

target: create_dirs OBJECTS 

create_dirs:
	${MKDIR} ${OAM_BIN_DIR}		

OBJECTS: ${OBJS}

clean:
	${RM} ${OBJS}

#-------------------- Some rules. -------------#
${OAM_BIN_DIR}/%.o : ${SRCDIR}/%.c
	${COMPILE} $< -c -o $@
