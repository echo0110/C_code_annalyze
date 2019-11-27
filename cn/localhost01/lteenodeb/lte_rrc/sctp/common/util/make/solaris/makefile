##############################################################################
#############	Sigtran Common UTIL makefile
##############################################################################

CC = gcc -Wall
INCLUDES = -I ../../h  -I $(SIGTRAN_DIR)/common/gw/common/h -I $(SIGTRAN_DIR)/common/gw/genlib/common/h -I ../../../port/h/ -I ../../../port/solaris/h
DEFINES = -DSIGTRAN_PORT_SOLARIS -DGW_SOLARIS -DGL_SOLARIS

SRC = ../../src
OBJS_PATH = ../../obj/sparc

UTIL_SRC= $(SRC)/tcpiplib.c \
	  $(SRC)/sig_hand.c

UTIL_OBJS= $(OBJS_PATH)/tcpiplib.o \
	   $(OBJS_PATH)/sig_hand.o 

HASH_UTIL_SRC=$(SRC)/sig_hash.c \
	      $(SRC)/tcpiplib.c \
	      $(SRC)/sig_hand.c

HASH_UTIL_OBJ=$(OBJS_PATH)/sig_hash.o \
	      $(OBJS_PATH)/tcpiplib.o \
	      $(OBJS_PATH)/sig_hand.o 

all  :  HASH_UTIL_ALL

HASH_UTIL_ALL  :
	$(CC) -c $(DEFINES) $(INCLUDES) -DSIGTRAN_PORT_SOLARIS $(HASH_UTIL_SRC)
	mv *.o $(OBJS_PATH)
	$(LD) -r -o $(OBJS_PATH)/sig_util.o $(HASH_UTIL_OBJ)

UTIL_ALL  :
	$(CC) -c $(DEFINES) $(INCLUDES) -DSIGTRAN_PORT_SOLARIS $(UTIL_SRC)
	mv *.o $(OBJS_PATH)
	$(LD) -r -o $(OBJS_PATH)/sig_util.o $(UTIL_OBJS)

clean:
	$(RM) -f $(HASH_UTIL_OBJ) $(OBJS_PATH)/sig_util.o

cleanhash:
	 $(RM) -f $(HASH_UTIL_OBJ) $(OBJS_PATH)/sig_util_hash.o

.c.o:
	$(CC) $(DEFINES) $(INCLUDES) -c $< 
	mv *.o $(OBJS_PATH)
