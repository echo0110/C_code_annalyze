##############################################################################
#############	Sigtran Common OS Port makefile
##############################################################################

WARNINGS = -Wall -Wstrict-prototypes -Wimplicit -Wmissing-prototypes -Wmissing-declarations -Wreturn-type -Wcomment -Waggregate-return -Wparentheses  -fno-builtin -fsigned-char -pipe -Wstrict-prototypes -Wno-format -W -fcond-mismatch -Wnested-externs -Wswitch -fsigned-char -Wshadow 
#-Wid-clash-32 

#-Wredundant-decls flag is not used ,using this warnings start coming in
# system header files

CPU = sparc
RM  = \rm
CFLAGS	=	-DSIGTRAN_PORT_SOLARIS -DSIGTRAN_SOLARIS_SUN_CC_SUPPORT
#-DSIGTRAN_INET6_SUPPORT
#-DSIGTRAN_LOCAL_TIME_THREAD

GENLIB_FLAGS = -DSIGTRAN_PORT_SOLARIS -DGW_SOLARIS -DGL_SOLARIS
#CC = insure gcc 
CC =  gcc
INCLUDES = -I ../../h \
	   -I ../../../gw/common/h \
	   -I ../../../gw/genlib/common/h \
       -I ../../solaris/h\
	   -I $(CSPL_DIR)/h\
	   -I $(CSPL_DIR)/ports/solaris

SRC = ../../src
OBJS_PATH = ../../obj/$(CPU)
SIG_OS      = $(SIGTRAN_DIR)/common/port/obj/$(CPU)/sig_os.o

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
      $(SRC)/../solaris/src/sig_ipc_os.c\
      $(SRC)/../solaris/src/sig_os_mutx.c\
      $(SRC)/../solaris/src/sig_os_thr.c\
      $(SRC)/../solaris/src/sig_os_time.c


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
      $(OBJS_PATH)/sig_os_thr.o\
      $(OBJS_PATH)/sig_os_time.o


OS_CLI_SRC= $(SRC)/sig_mutx.c \
	  $(SRC)/sig_mem.c \
	  $(SRC)/sig_buff.c \
	  $(SRC)/sig_dbg.c \
	  $(SRC)/sig_util.c \
	  $(SRC)/sig_ipc.c \
	  $(SRC)/sig_time.c \
      $(SRC)/sig_nw_util.c\
      $(SRC)/../solaris/src/sig_ipc_os.c\
      $(SRC)/../solaris/src/sig_os_mutx.c\
      $(SRC)/../solaris/src/sig_os_thr.c\
      $(SRC)/../solaris/src/sig_os_time.c\
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
	$(CC) $(INCLUDES) $(CFLAGS) $(CFLAGS_GCC) -DSIG_COMMON_WITH_CSPL $(WARNINGS) -c $(OS_SRC)
	mv *.o $(OBJS_PATH)
	$(LD) -r -o $(OBJS_PATH)/sig_os.o $(OS_OBJS)

OS_HASH  :
	$(RM) -f $(HASH_OBJS)
	$(CC) $(INCLUDES) $(GENLIB_FLAGS) -c $(HASH_SRC)
	mv *.o $(OBJS_PATH)
	$(LD) -r -o $(OBJS_PATH)/sig_util.o $(HASH_OBJS)

OS_CLI:
	$(RM) -f $(OS_OBJS)
	$(CC) $(INCLUDES) $(CFLAGS) $(CFLAGS_GCC) $(WARNINGS) -c $(OS_CLI_SRC)
	mv *.o $(OBJS_PATH)
	$(LD) -r -o $(OBJS_PATH)/sig_os_cli.o $(OS_CLI_OBJS)

clean:
	$(RM) -f $(OS_OBJS) $(OBJS_PATH)/sig_os.o $(OBJS_PATH)/sig_os_cli.o 
	$(RM) -f $(OBJS_PATH)/sig_hash.o 
	$(RM) -f $(OBJS_PATH)/sig_util.o 

.c.o:
	$(CC) $(INCLUDES) -c $< 
	mv *.o $(OBJS_PATH)
