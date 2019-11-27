#/******************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
# *
# ******************************************************************************
# *
# *  $Id: Makefile,v 1.1.1.1.6.1.6.1 2010/09/21 15:46:06 gur20491 Exp $
# *
# ******************************************************************************
# *
# *  File Description : Makefile for building PHY Interfaces Modules. 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Revision 1.1.1.1.6.1.6.1  2010/09/21 15:46:06  gur20491
# * FAPI changes
# *
# * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.8  2009/06/26 05:09:26  gur19413
# * updated
# *
# * Revision 1.7  2009/06/26 04:21:43  gur19413
# * remove unneeded files
# *
# * Revision 1.6  2009/06/05 12:26:26  gur19413
# * updated
# *
# * Revision 1.5  2009/06/05 09:52:39  gur19413
# * added files
# *
# * Revision 1.4  2009/05/28 08:46:21  gur19413
# * Initial Version
# *
# * Revision 1.3  2009/05/19 06:23:04  gur18550
# * File Updated
# *
# * Revision 1.2  2009/05/10 09:07:40  gur19413
# * Initial Version
# *
# * Revision 1.1  2009/05/06 03:48:02  gur11912
# * Initial Version
# *
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

PHY_BIN_DIR=${MACCORE_BIN_DIR}/interfaces/phy

SRCDIR= ${INTERFACES_ROOT}/phy

OBJS= ${PHY_BIN_DIR}/lteMacPhyInterface.o ${PHY_BIN_DIR}/lteMacReceiver.o ${PHY_BIN_DIR}/lteMacSFNMgr.o ${PHY_BIN_DIR}/lteMacTimingAdvanceMgr.o\
    ${PHY_BIN_DIR}/lteMacTransmitter.o ${PHY_BIN_DIR}/lteMacFAPIApi.o

target: create_dirs OBJECTS 

create_dirs:
	${MKDIR} ${PHY_BIN_DIR}	

OBJECTS: ${OBJS}

clean:
	${RM} ${OBJS}

#-------------------- Some rules. -------------#
${PHY_BIN_DIR}/%.o : ${SRCDIR}/%.c
	${COMPILE} $< -c -o $@
