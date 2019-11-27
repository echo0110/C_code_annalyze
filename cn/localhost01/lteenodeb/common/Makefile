#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: Makefile,v 1.2 2010/02/13 10:32:02 gur15697 Exp $
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
# * Revision 1.2  2010/02/13 10:32:02  gur15697
# * changed for disabling the locking at enodeb side
# *
# * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
# * eNB framework for intel
# *
# * Revision 1.6.10.2.2.1  2009/09/11 12:35:23  gur12140
# * modified to integrate MAC 1.2 with existing stacks.
# *
# * Revision 1.6.10.2  2009/08/27 11:22:23  gur19140
# * *** empty log message ***
# *
# * Revision 1.6.10.1  2009/06/09 11:24:43  gur19140
# * first wave of Integration
# *
# * Revision 1.8  2009/05/12 06:05:57  gur19479
# * pdcp merging
# *
# * Revision 1.7  2009/05/06 05:40:20  gur20548
# * pdcp common merged with lte common
# *
# * Revision 1.6  2008/09/18 05:27:13  gur19836
# * New File added lteMisc.o
# *
# * Revision 1.5  2008/09/10 03:41:28  gur11974
# * Updated for integration
# *
# * Revision 1.4  2008/09/08 10:32:01  gur19836
# * lteSQueue library Added
# *
# * Revision 1.3  2008/09/05 08:18:09  ssinghal
# * Makefile changes
# *
# * Revision 1.2  2008/09/04 08:40:00  gur19836
# * lteSQueue.o Added for compilation
# *
# * Revision 1.1  2008/08/25 11:38:50  ssinghal
# * Makefile added
# *
# *
# *
# *
# ****************************************************************************/
##############################################################################
#
#		MAIN MAKE FILE						 
#	Make file to be used for building COMMON. 
#						
##############################################################################
include ${LTE_ROOT}/make.inc

FRAMEWORK_ARCHIVE=libframework.a
SRC_DIR= ${COMMON_ROOT}/src
PORT_LIB= ${COMMON_LIB_DIR}/lte_port
INCLUDE_LTE+= -I$(COMMON_ROOT)/include/lte_port


OBJS=${COMMON_LIB_DIR}/lteCommonStatsManager.o ${COMMON_LIB_DIR}/lteComOAMIntegration.o ${COMMON_LIB_DIR}/lteConfigFileHdlr.o ${COMMON_LIB_DIR}/lteMacComPorting.o ${COMMON_LIB_DIR}/lteCircQueue.o ${COMMON_LIB_DIR}/lteShmLogger.o ${COMMON_LIB_DIR}/lteTree.o ${COMMON_LIB_DIR}/lteHash.o ${COMMON_LIB_DIR}/lteList.o ${COMMON_LIB_DIR}/lteMemPool.o ${COMMON_LIB_DIR}/lteMsgPool.o ${COMMON_LIB_DIR}/lteLog.o ${COMMON_LIB_DIR}/ltePQueue.o ${COMMON_LIB_DIR}/lteQueue.o ${COMMON_LIB_DIR}/lteSQueue.o ${COMMON_LIB_DIR}/lteMisc.o ${COMMON_LIB_DIR}/lteSync.o ${COMMON_LIB_DIR}/lteThread.o ${COMMON_LIB_DIR}/lteTime.o ${PORT_LIB}/mempool.o ${PORT_LIB}/msgpool.o ${PORT_LIB}/port.o ${PORT_LIB}/ylib-hash.o ${PORT_LIB}/ylib-list.o ${PORT_LIB}/ylib-pool.o ${PORT_LIB}/ylib-squeue.o ${PORT_LIB}/ylib-time.o ${PORT_LIB}/ylib-tree.o ${PORT_LIB}/atomic-ops.o ${COMMON_LIB_DIR}/lteMessageQIntf.o ${PORT_LIB}/shmpool.o ${COMMON_LIB_DIR}/ltePerfStats.o
#SPR 19288 change start
#SPR 19713 change start
#remove 19288 if condition
#SPR 19713 change end
CFLAGS += $(FLAGS)
ifeq ($(LTE_CC), arm-linux-gnueabi-gcc)
CFLAGS += $(ARM_CFLAGS)
INCLUDE_LTE+=-I$(ARM_INCLUDE)
endif
#SPR 19288 change end
target:${OBJS}
	for i in ${OBJS}; \
		do \
			$(AR) -ur $(COMMON_LIB_DIR)/$(FRAMEWORK_ARCHIVE) $$i ; \
		done


clean:
	${RM} $(OBJS)
	${RM} $(COMMON_LIB_DIR)/$(FRAMEWORK_ARCHIVE)
	${RMD} $(PORT_LIB)
#SPR 19713 change start
#code for 19288 removed
#SPR 192713 change end

#-------------------- Some rules. -------------#
${COMMON_LIB_DIR}/%.o : ${SRC_DIR}/%.c 
	${MKD} $(COMMON_LIB_DIR)
	${MKD} $(PORT_LIB)
	${COMPILE} $< -c -o $@
