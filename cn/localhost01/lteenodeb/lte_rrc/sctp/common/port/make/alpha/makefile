##############################################################################
#############	Sigtran Common OS Port makefile
##############################################################################

CC = cc -std 
INCLUDES = -I../../h \
	   -I../../../gw/genlib/common/h \
	   -I../../../gw/genlib/os/h \
	   -I../../../gw/common/h 

SRC = ../../src
OBJS_PATH = ../../obj/$(CPU)

OS_SRC= $(SRC)/sig_mutx.c \
	  $(SRC)/sig_tmr.c \
	  $(SRC)/sig_mem.c \
	  $(SRC)/sig_time.c \
	  $(SRC)/sig_buff.c \
	  $(SRC)/sig_dbg.c \
	  $(SRC)/sig_thr.c

OS_OBJS= $(OBJS_PATH)/sig_mutx.o \
	  $(OBJS_PATH)/sig_tmr.o \
	  $(OBJS_PATH)/sig_mem.o \
	  $(OBJS_PATH)/sig_time.o \
	  $(OBJS_PATH)/sig_buff.o \
	  $(OBJS_PATH)/sig_dbg.o \
	  $(OBJS_PATH)/sig_thr.o 

OS_CLI_SRC= $(SRC)/sig_mutx.c \
	  $(SRC)/sig_mem.c \
	  $(SRC)/sig_time.c \
	  $(SRC)/sig_dbg.c \
	  $(SRC)/sig_thr.c 

OS_CLI_OBJS= $(OBJS_PATH)/sig_mutx.o \
	  $(OBJS_PATH)/sig_mem.o \
	  $(OBJS_PATH)/sig_time.o \
	  $(OBJS_PATH)/sig_dbg.o \
	  $(OBJS_PATH)/sig_thr.o 

all:   OS_ALL  OS_CLI

OS_ALL  :
	$(CC) $(INCLUDES) -DSIGTRAN_PORT_TRU64 -DGL_TRU64  -c $(OS_SRC)
	mv *.o $(OBJS_PATH)
	$(LD) -r -o $(OBJS_PATH)/sig_os.o $(OS_OBJS)

OS_CLI:
	$(CC) $(INCLUDES) -DSIGTRAN_PORT_TRU64 -DGL_TRU64 -c $(OS_CLI_SRC)
	mv *.o $(OBJS_PATH)
	$(LD) -r -o $(OBJS_PATH)/sig_os_cli.o $(OS_CLI_OBJS)

clean:
	$(RM) -f $(OS_OBJS) $(OBJS_PATH)/sig_os.o $(OBJS_PATH)/sig_os_cli.o

.c.o:
	$(CC) $(INCLUDES) -c $< 
	mv *.o $(OBJS_PATH)
