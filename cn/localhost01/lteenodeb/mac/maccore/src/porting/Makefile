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
# *  File Description : Makefile for building Execution Scheduler 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.2  2009/06/05 07:15:39  gur18569
# * updated make
# *
# * Revision 1.1  2009/05/06 03:46:37  gur11912
# * Initial Version
# *
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

PORTING_BIN_DIR=${MACCORE_BIN_DIR}/porting

SRCDIR= ${PORTING_ROOT}

OBJS= ${PORTING_BIN_DIR}/lteMacPorting.o

ifeq ($(LTE_EMBMS_FLAG),1)
OBJS += ${PORTING_BIN_DIR}/lteMacEmbmsPorting.o	
endif	
      
target: create_dirs OBJECTS 

create_dirs:
	${MKDIR} ${PORTING_BIN_DIR}		

OBJECTS: ${OBJS}

clean:
	${RM} ${OBJS}

#-------------------- Some rules. -------------#
${PORTING_BIN_DIR}/%.o : ${SRCDIR}/%.c
	${COMPILE} $< -c -o $@
