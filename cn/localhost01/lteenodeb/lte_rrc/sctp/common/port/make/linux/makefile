##############################################################################
#############	Sigtran Common OS Port makefile
##############################################################################
include $(LTE_ROOT)/make.inc


WARNINGS = -Wall -Wimplicit -Wmissing-prototypes\
           -Wshadow -Wcomment -Waggregate-return\
                -Wmissing-declarations -Wreturn-type \
                -Wparentheses -Wpointer-arith \
                -fno-builtin -fno-inline-functions \
                -fsigned-char -pipe -Wstrict-prototypes \
                -Wno-format -W -fcond-mismatch -Wnested-externs\
                -Wswitch 

#-Wredundant-decls flag is not used ,using this warnings start coming in
# system header files

CPU = linux
RM  = \rm
CFLAGS	=	$(OPTIM_FLAGS) -DSIGTRAN_PORT_LINUX -DSIGTRAN_SOLARIS_SUN_CC_SUPPORT 
#-DSIGTRAN_INET6_SUPPORT
#-DSIGTRAN_LOCAL_TIME_THREAD

GENLIB_FLAGS = -DSIGTRAN_PORT_LINUX -DGW_LINUX -DGL_LINUX
#CC = insure gcc 
ifndef GCC
GCC =  $(LTE_CC) 
endif
INCLUDES = -I ../../h \
	   -I $(CSPL_DIR)/h \
	   -I $(CSPL_DIR)/ports/linux \
	   -I ../../../gw/common/h \
	   -I ../../../gw/genlib/common/h \
       -I ../../linux/h
#Uncomment following flag to use aricent internal testool for single thread behaviour



CFLAGS	+=	-DSIGTRAN_PORT_LINUX

#Uncomment following flag to enable single thread behaviour 
#CFLAGS	+=	 -DSIGTRAN_SINGLE_THREAD 

SRC = ../../src
OBJS_PATH = ../../obj/$(CPU)
#SIGTRAN_DIR = /cm_data/gateway_product/sg/sigtran
SIG_OS      = $(SIGTRAN_BASE_DIR)/common/port/obj/$(CPU)/sig_os.o

OS_SRC= $(SRC)/sig_tmr.c \
      $(SRC)/sig_ipc.c\
      $(SRC)/sig_nw_util.c\
      $(SRC)/sig_cspl_wrap.c\
      $(SRC)/sig_dbg.c\
      $(SRC)/sig_buff.c\
      $(SRC)/sig_mem.c\
      $(SRC)/sig_mutx.c\
      $(SRC)/sig_thr.c\
      $(SRC)/sig_time.c\
      $(SRC)/sig_util.c\
      $(SRC)/sig_hand.c\
      $(SRC)/../linux/src/sig_ipc_os.c\
      $(SRC)/../linux/src/sig_os_mutx.c\
      $(SRC)/../linux/src/sig_os_thr.c \
      $(SRC)/../linux/src/sig_os_time.c

OS_OBJS= $(OBJS_PATH)/sig_tmr.o \
	  $(OBJS_PATH)/sig_ipc.o \
	  $(OBJS_PATH)/sig_nw_util.o \
	  $(OBJS_PATH)/sig_cspl_wrap.o \
	  $(OBJS_PATH)/sig_dbg.o \
	  $(OBJS_PATH)/sig_buff.o \
	  $(OBJS_PATH)/sig_mem.o \
	  $(OBJS_PATH)/sig_mutx.o \
	  $(OBJS_PATH)/sig_thr.o \
	  $(OBJS_PATH)/sig_time.o \
      $(OBJS_PATH)/sig_util.o\
      $(OBJS_PATH)/sig_hand.o\
	  $(OBJS_PATH)/sig_ipc_os.o \
      $(OBJS_PATH)/sig_os_mutx.o\
      $(OBJS_PATH)/sig_os_thr.o \
      $(OBJS_PATH)/sig_os_time.o

OS_CLI_SRC= $(SRC)/sig_mutx.c \
	  $(SRC)/sig_mem.c \
	  $(SRC)/sig_buff.c \
	  $(SRC)/sig_dbg.c \
	  $(SRC)/sig_util.c \
	  $(SRC)/sig_ipc.c \
	  $(SRC)/sig_time.c \
      $(SRC)/sig_nw_util.c\
      $(SRC)/../linux/src/sig_ipc_os.c\
      $(SRC)/../linux/src/sig_os_mutx.c\
      $(SRC)/../linux/src/sig_os_thr.c\
      $(SRC)/../linux/src/sig_os_time.c\
	  $(SRC)/sig_thr.c 

OS_CLI_OBJS= $(OBJS_PATH)/sig_mutx.o \
	  $(OBJS_PATH)/sig_mem.o \
	  $(OBJS_PATH)/sig_buff.o \
	  $(OBJS_PATH)/sig_dbg.o \
	  $(OBJS_PATH)/sig_util.o \
	  $(OBJS_PATH)/sig_ipc.o \
	  $(OBJS_PATH)/sig_time.o \
      $(OBJS_PATH)/sig_nw_util.o\
	  $(OBJS_PATH)/sig_ipc_os.o \
      $(OBJS_PATH)/sig_os_mutx.o\
      $(OBJS_PATH)/sig_os_thr.o\
      $(OBJS_PATH)/sig_os_time.o\
	  $(OBJS_PATH)/sig_thr.o 

HASH_SRC= $(SRC)/sig_hash.c 

HASH_OBJS= $(OBJS_PATH)/sig_hash.o 


all:   OS_ALL OS_CLI OS_HASH 

OS_ALL  :
	$(RM) -f $(OS_CLI_OBJS)
	$(GCC) $(INCLUDES) $(CFLAGS) $(CFLAGS_GCC) -DSIG_COMMON_WITH_CSPL $(WARNINGS) -c $(OS_SRC)
	mv *.o $(OBJS_PATH)
	$(LTE_LD)  -r -o $(OBJS_PATH)/sig_os.o $(OS_OBJS)

OS_HASH  :
	$(RM) -f $(HASH_OBJS)
	$(GCC) $(INCLUDES) $(GENLIB_FLAGS) -c $(HASH_SRC)
	mv *.o $(OBJS_PATH)
	$(LTE_LD) -r -o $(OBJS_PATH)/sig_util.o $(HASH_OBJS)

OS_CLI:
	$(RM) -f $(OS_OBJS)
	$(GCC) $(INCLUDES) $(CFLAGS) $(CFLAGS_GCC) $(WARNINGS) -c $(OS_CLI_SRC)
	mv *.o $(OBJS_PATH)
	$(LTE_LD)  -r -o $(OBJS_PATH)/sig_os_cli.o $(OS_CLI_OBJS)

clean:
	$(RM) -f $(OS_OBJS) $(OBJS_PATH)/sig_os.o $(OBJS_PATH)/sig_os_cli.o 
	$(RM) -f $(OBJS_PATH)/sig_hash.o 
	$(RM) -f $(OBJS_PATH)/sig_util.o 

.c.o:
	$(GCC) $(INCLUDES) -c $< 
	mv *.o $(OBJS_PATH)
