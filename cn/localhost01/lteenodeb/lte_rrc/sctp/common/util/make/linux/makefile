##############################################################################
#############	Sigtran Common UTIL makefile
##############################################################################

CC = $(LTE_CC) -Wall
INCLUDES = -I ../../h  -I $(SIGTRAN_DIR)/common/gw/common/h -I $(SIGTRAN_DIR)/common/gw/genlib/common/h -I ../../../port/h/ 
DEFINES = -DSIGTRAN_PORT_LINUX 

SRC = ../../src
OBJS_PATH = ../../obj/linux

UTIL_SRC= $(SRC)/tcpiplib.c 

UTIL_OBJS= $(OBJS_PATH)/tcpiplib.o 

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
	$(LTE_CC) -c $(DEFINES) $(INCLUDES) -DSIGTRAN_PORT_LINUX $(HASH_UTIL_SRC)
	mv *.o $(OBJS_PATH)
	$(LTE_LD)  -r -o $(OBJS_PATH)/sig_util_hash.o $(HASH_UTIL_OBJ)

UTIL_ALL  :
	$(LTE_CC) -c $(DEFINES) $(INCLUDES) -DSIGTRAN_PORT_LINUX $(UTIL_SRC)
	mv *.o $(OBJS_PATH)
	$(LTE_LD) -r -o $(OBJS_PATH)/sig_util.o $(UTIL_OBJS)

clean:
	$(RM) -f $(UTIL_OBJS) $(OBJS_PATH)/sig_util.o

cleanhash:
	 $(RM) -f $(HASH_UTIL_OBJ) $(OBJS_PATH)/sig_util_hash.o

.c.o:
	$(LTE_CC) $(DEFINES) $(INCLUDES) -c $< 
	mv *.o $(OBJS_PATH)
