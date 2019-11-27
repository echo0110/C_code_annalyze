#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: Makefile,v 1.0 2013/07/15 
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
# ****************************************************************************/
##############################################################################
#
#		MAIN MAKE FILE						 
#	Make file to be used for building IN_MEMORY_LOGGING 
#						
##############################################################################
include ${LTE_ROOT}/make.inc

LGR_ARCHIVE=libinMemLgr.a
SRC_DIR= ${COMMON_ROOT}/in_memory_logging/src
INCLUDE_LTE+= -I$(COMMON_ROOT)/in_memory_logging/include/
INCLUDE_LTE+= -I$(LTE_ROOT)/lte_rrc/cspl/h/
INCLUDE_LTE+= -I$(LTE_ROOT)/lte_rrc/cspl/ports/posix/

OBJS=${COMMON_LIB_DIR}/lteLogger.o
CFLAGS += $(FLAGS)
target:makelibdir ${OBJS}
	for i in ${OBJS}; \
		do \
			$(AR) -ur $(COMMON_LIB_DIR)/$(LGR_ARCHIVE) $$i ; \
		done

makelibdir:
	${MKD} $(COMMON_LIB_DIR)

clean:
	${RM} ${OBJS}
	${RM} $(COMMON_LIB_DIR)/$(LGR_ARCHIVE)

#-------------------- Some rules. -------------#
${COMMON_LIB_DIR}/%.o : ${SRC_DIR}/%.c 
	${COMPILE} $< -c -o $@
