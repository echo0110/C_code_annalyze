#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: Makefile,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
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
# * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
# * eNB framework for intel
# *
# * Revision 1.2  2009/01/30 14:00:32  gur19140
# * minnor changes
# *
# * Revision 1.1  2008/12/30 06:24:49  gur20294
# * RLC Layer added. INITIAL DRAFT
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
		${RLC_ROOT}/rlccore \
        ${RLC_ROOT}/simulator/mac



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

