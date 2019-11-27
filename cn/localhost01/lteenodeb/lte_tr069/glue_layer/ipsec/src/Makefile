##################################################################################
#
#   FILE NAME   : Makefile for IPSec Library 
#
#   DESCRIPTION: Makefile for IPSec library
#
#   REVISION HISTORY:
#
#   NAME         DATE                REFERENCE              REASON
#
#   Anant K Sharma    Nov 17, 2012   TR-069 Amendment 4   Initial Creation
#
#
#   All Rights Reserved. Copyright @ Aricent Group, 2012.
#
##################################################################################

COMPILE_DIR = /usr/bin/

AR              = $(COMPILE_DIR)ar
AS              = $(COMPILE_DIR)as
LD              = $(COMPILE_DIR)ld
CC              = $(COMPILE_DIR)gcc

CFLAGS = $(ALLOWED_INCLUDE_PATHS)
ARFLAGS = rcs

LIB = libipsec.a
LIBS_PATH = ../../../libs
all install: $(LIB) libs_install
OBJS = 	ipr_cwmp_policy_handler.o ipr_cwmp_state_handler.o ipr_cwmp_netlink.o
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(CURR_DIR)


all install: $(LIB) libs_install

clean: 
	rm -rf *.o *.d *.d.* core.* *.a output.txt $(LIB) $(LIBS_PATH)/$(LIB)

libs_install:
	install -m 755 $(LIB) $(LIBS_PATH)

$(LIB): $(OBJS)

	${AR} ${ARFLAGS} libipsec.a *.o

ALLOWED_INCLUDE_PATHS := -I../include -I../../../cwmp/common/include
