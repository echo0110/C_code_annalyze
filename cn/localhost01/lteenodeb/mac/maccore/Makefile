#/******************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
# *
# ******************************************************************************
# *
# *  $Id: Makefile,v 1.1.1.1.6.1 2010/08/02 08:13:41 gur20491 Exp $
# *
# ******************************************************************************
# *
# *  File Description : Makefile for building MACCORE 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Revision 1.1.1.1.6.1  2010/08/02 08:13:41  gur20491
# * FrameworkSetupWith1.0_MAC1.2.4
# *
# * Revision 1.20  2009/09/16 10:11:59  gur03939
# * Re-Added
# *
# * Revision 1.18  2009/06/05 07:16:29  gur18569
# * updated make file
# *
# * Revision 1.17  2009/05/06 03:56:01  gur11912
# * Makefile for MAC 1.2
# *
# *
# *****************************************************************************/
##############################################################################
#
#		MAIN MAKE FILE						 
#	Makefile to be used for building MAC. 
#						
##############################################################################
include ${LTE_ROOT}/make.inc
include ${MACCORE_ROOT}/mac.inc

MACCORE_SUBDIRS = \
		${MACCORE_COMMON_ROOT} \
		${SCHEDULER_ROOT} \
		${ASSEMBLY_DEASSEMBLY_ROOT} \
		${INTERFACES_ROOT} \
        ${EXECUTION_SCHEDULER_ROOT} \
		${PORTING_ROOT}


.DEFAULT_GOAL:= debug

debug release: MACCORE_MODULES

MACCORE_MODULES:
	${MKDIR} lib
	${MKDIR} bin
	${MKDIR} ${MACCORE_ROOT}/lib
	set -e;
	for i in $(MACCORE_SUBDIRS);\
	do \
	echo "Building in $$i"; \
	if ! $(MAKE) target -C $$i; \
	then exit 1; \
	fi; \
	done

#Building MAC library once all objects files are created
# SPR 23261 Changes start */
ifeq ($(INTEL_GPS_SYNC_MODE),1)
	cp -f ${LTE_ROOT}/../lib/mindspeed/libminmea.a ${LTE_ROOT}/mac/maccore/lib/
	${AR} -x ${MACCORE_LIB_DIR}/libminmea.a && mv *.o ${MACCORE_BIN_DIR}/interfaces/gps/
endif
# SPR 23261 Changes end */
	$(AR) -ur $(MACCORE_LIB_DIR)/$(MACCORE_ARCHIVE) ${MACCORE_BIN_DIR}/*/*.o $(MACCORE_BIN_DIR)/*/*/*.o 
#$(LTE_ROOT)/mac/schtest/bin/lteRrcSimulator.o $(LTE_ROOT)/mac/schtest/bin/lteRlcSimulator.o


##################################################
# Rules for Cleaning the object/library archives #    
##################################################

clean:
	for i in $(MACCORE_SUBDIRS);\
	do \
	echo "Cleaning in $$i"; \
	if ! $(MAKE_CLEAN) clean -C $$i; \
	then exit 1; \
	fi; \
	done
	${RM} $(MACCORE_LIB_DIR)/$(MACCORE_ARCHIVE)
	${RMD} $(MACCORE_LIB_DIR)
