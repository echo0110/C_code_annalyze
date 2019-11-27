#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: Makefile,v 1.1.1.1.6.1 2010/08/02 08:13:41 gur20491 Exp $
# *
# ****************************************************************************
# *
# *  File Description : Makefile
# *
# ****************************************************************************
# *
# * Revision Details
# * ----------------
# *
# * $Log: Makefile,v $
# * Revision 1.1.1.1.6.1  2010/08/02 08:13:41  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.1  2008/09/05 08:18:57  ssinghal
# * Makefile added
# *
# *
# *
# *
# ****************************************************************************/
##############################################################################
#
#		MAIN MAKE FILE						 
#	Make file to be used for building maccore and simulator.
#						
##############################################################################
include ${LTE_ROOT}/make.inc

COMMON_SUBDIRS = \
		${LTE_ROOT}/mac/maccore \
        ${LTE_ROOT}/mac/simulator



.DEFAULT_GOAL:= debug

all debug release local cross tests common target: COMMON_MODULES 


COMMON_MODULES:
	set -e;
	for i in $(COMMON_SUBDIRS);\
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
	for i in $(COMMON_SUBDIRS);\
	do \
	echo "Cleaning in $$i"; \
	if ! $(MAKE_CLEAN) clean -C $$i; \
	then exit 1; \
	fi; \
	done

