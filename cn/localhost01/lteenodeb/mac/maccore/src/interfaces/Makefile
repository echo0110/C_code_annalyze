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
# *  File Description : Makefile for building Interfaces Modules. 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.1  2009/05/06 03:47:09  gur11912
# * Initial Version
# *
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

INTERFACES_ROOT=${MACCORE_ROOT}/src/interfaces

INTERFACES_SUBDIRS = \
		${INTERFACES_ROOT}/rrc \
		${INTERFACES_ROOT}/phy \
		${INTERFACES_ROOT}/rlc \
		${INTERFACES_ROOT}/oam \
		${INTERFACES_ROOT}/rrm \
		${INTERFACES_ROOT}/pdcp

ifeq ($(LTE_EMBMS_FLAG),1)
INTERFACES_SUBDIRS +=  ${INTERFACES_ROOT}/sync
endif

.DEFAULT_GOAL:= debug

debug release target: INTERFACES_MODULES

INTERFACES_MODULES:
	${MKDIR} ${MACCORE_BIN_DIR}/interfaces
	set -e;
	for i in $(INTERFACES_SUBDIRS);\
	do \
	echo "Building in $$i"; \
	if ! $(MAKE) target -C $$i; \
	then exit 1; \
	fi; \
	done
##################################################
# Rules for Cleaning the object/library archives #    
##################################################


clean:
	for i in $(INTERFACES_SUBDIRS);\
	do \
	echo "Cleaning in $$i"; \
	if ! $(MAKE_CLEAN) clean -C $$i; \
	then exit 1; \
	fi; \
	done

