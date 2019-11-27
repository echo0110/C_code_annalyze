##############################################################################
#############	Sigtran Common UTIL makefile
##############################################################################

CC = cc -std
INCLUDES =	-I../../h  \
		-I$(SIGTRAN_DIR)/common/gw/common/h \
		-I$(SIGTRAN_DIR)/common/gw/genlib/common/h \
		-I../../../port/h

DEFINES = -DSIGTRAN_PORT_TRU64 -DGW_SOLARIS -DGL_SOLARIS -DGL_TRU64

SRC = ../../src
OBJS_PATH = ../../obj/$(CPU)

UTIL_SRC= $(SRC)/tcpiplib.c \
          $(SRC)/sig_hand.c 

UTIL_OBJS= $(OBJS_PATH)/tcpiplib.o \
           $(OBJS_PATH)/sig_hand.o 

HASH_UTIL_SRC=$(SRC)/sig_hash.c \
	      $(SRC)/tcpiplib.c \
	      $(SRC)/sig_hand.c \
	      $(SRC)/sig_util.c

HASH_UTIL_OBJ=$(OBJS_PATH)/sig_hash.o \
	      $(OBJS_PATH)/tcpiplib.o \
	      $(OBJS_PATH)/sig_hand.o \
	      $(OBJS_PATH)/sig_util.o

all  :  UTIL_ALL

hash_all : HASH_UTIL_ALL 

HASH_UTIL_ALL  :
	$(CC) -c $(DEFINES) $(INCLUDES) -DSIGTRAN_PORT_TRU64 $(HASH_UTIL_SRC)
	mv *.o $(OBJS_PATH)
	$(LD) -r -o $(OBJS_PATH)/sig_util_hash.o $(HASH_UTIL_OBJ)

UTIL_ALL  :
	$(CC) -c $(DEFINES) $(INCLUDES) -DSIGTRAN_PORT_TRU64 $(UTIL_SRC)
	mv *.o $(OBJS_PATH)
	$(LD) -r -o $(OBJS_PATH)/sig_util.o $(UTIL_OBJS)

clean:
	$(RM) -f $(UTIL_OBJS) $(OBJS_PATH)/sig_util.o

cleanhash:
	 $(RM) -f $(HASH_UTIL_OBJ) $(OBJS_PATH)/sig_util_hash.o

.c.o:
	$(CC) $(DEFINES) $(INCLUDES) -c $< 
	mv *.o $(OBJS_PATH)
