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
# *  File Description : Makefile for building Assembly Deassembly Unit. 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.2  2009/06/08 14:07:24  gur12140
# * Removed Linking Errors
# *
# * Revision 1.1  2009/05/06 03:42:14  gur11912
# * Initial Version
# *
# *
# *****************************************************************************/
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

ASSEMBLY_DEASSEMBLY_SUBDIRS = \
		${ASSEMBLY_DEASSEMBLY_ROOT}/multiplexer \
		${ASSEMBLY_DEASSEMBLY_ROOT}/encoder \
		${ASSEMBLY_DEASSEMBLY_ROOT}/dlHarq \
		${ASSEMBLY_DEASSEMBLY_ROOT}/demultiplexer \
		${ASSEMBLY_DEASSEMBLY_ROOT}/ulHarq

.DEFAULT_GOAL:= debug

debug release target: ASSEMBLY_DEASSEMBLY_MODULES

ASSEMBLY_DEASSEMBLY_MODULES:
	${MKDIR} ${MACCORE_BIN_DIR}/assemblyDeassemblyUnit
	set -e;
	for i in $(ASSEMBLY_DEASSEMBLY_SUBDIRS);\
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
	for i in $(ASSEMBLY_DEASSEMBLY_SUBDIRS);\
	do \
	echo "Cleaning in $$i"; \
	if ! $(MAKE_CLEAN) clean -C $$i; \
	then exit 1; \
	fi; \
	done

