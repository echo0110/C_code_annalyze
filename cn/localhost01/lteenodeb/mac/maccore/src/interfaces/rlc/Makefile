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
# *  File Description : Makefile for building RLC Interfaces Modules. 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.6  2009/07/31 09:03:35  gur18569
# * replaced lteMacRlcPdcpInterface.o with  lteMacRlcInterface.o
# *
# * Revision 1.5  2009/07/15 06:45:27  gur18550
# * *** empty log message ***
# *
# * Revision 1.4  2009/07/15 06:15:37  gur18550
# * changes due to file name
# *
# * Revision 1.3  2009/06/09 04:09:03  gur18569
# * not compiling lteMacRlcInterface.c
# *
# * Revision 1.2  2009/06/01 06:53:18  gur18569
# * lteMacRlcInterface.c lteMacRlcPdcpInterface.c compiling these files
# *
# * Revision 1.1  2009/05/06 03:47:49  gur11912
# * Initial Version
# *
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

RLC_BIN_DIR=${MACCORE_BIN_DIR}/interfaces/rlc

SRCDIR= ${INTERFACES_ROOT}/rlc

OBJS= ${RLC_BIN_DIR}/lteMacRlcInterface.o 

ifeq ($(LTE_EMBMS_FLAG),1)
OBJS += $(RLC_BIN_DIR)/lteMacEmbmsRlcInterface.o
endif

target: create_dirs OBJECTS 

create_dirs:
	${MKDIR} ${RLC_BIN_DIR}		

OBJECTS: ${OBJS}

clean:
	${RM} ${OBJS}

#-------------------- Some rules. -------------#
${RLC_BIN_DIR}/%.o : ${SRCDIR}/%.c
	${COMPILE} $< -c -o $@
