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
# *  File Description : Makefile for building Scheduler Modules. 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.1  2009/05/06 03:49:02  gur11912
# * Initial Version
# *
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

SCHEDULER_SUBDIRS = \
		${SCHEDULER_ROOT}/strategy \
		${SCHEDULER_ROOT}/bcchManager \
		${SCHEDULER_ROOT}/ccchManager \
		${SCHEDULER_ROOT}/dlSchManager \
		${SCHEDULER_ROOT}/ulSchManager \
		${SCHEDULER_ROOT}/pcchManager \
		${SCHEDULER_ROOT}/rachManager \
		${SCHEDULER_ROOT}/resourceManager\
		${SCHEDULER_ROOT}/DRXManager\
		${SCHEDULER_ROOT}/MeasGapHandler\
		${SCHEDULER_ROOT}/ECIDManager\
		${SCHEDULER_ROOT}/PRSManager\
                ${SCHEDULER_ROOT}/ScellManager

.DEFAULT_GOAL:= debug

debug release target: SCHEDULER_MODULES

SCHEDULER_MODULES:
	${MKDIR} ${MACCORE_BIN_DIR}/scheduler
	set -e;
	for i in $(SCHEDULER_SUBDIRS);\
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
	for i in $(SCHEDULER_SUBDIRS);\
	do \
	echo "Cleaning in $$i"; \
	if ! $(MAKE_CLEAN) clean -C $$i; \
	then exit 1; \
	fi; \
	done

