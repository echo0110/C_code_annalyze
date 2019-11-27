export WIND_HOST_TYPE=x86-win32

#macro for base dir
TGT_DIR		=	$(WIND_BASE)\target
MAKE		=	make
CP		=	copy
RM		=	rm -f
TOOL		=	gnu
COF		=	objcopy -lpz

# setting for specific CPU
CC		=	cc386
CPU		=	PENTIUM
LIB		=	ar386 cru
RANLIB		=	ranlib386
LD		=	ld386	-N -r
CPU_CFLAGS	=	-DVX_IGNORE_GNU_LIBS -DVXWORKS\
			-mpentium  -nostdinc\
			-fvolatile -nostdlib -D_REENTRANT\
			-fno-builtin -fno-defer-pop

#VOB_BASE_COMMON	=	d:\views\test\gateway_product\sg\sigtran\common
#common c flags
COMMON_INCLUDES	=	-I$(TGT_DIR)\h -I$(TGT_DIR)\h\net
COPTIONS	=	-c -Wall -Wimplicit -Wcomment\
			-Waggregate-return -Wmissing-declarations\
			-Wreturn-type -Wparentheses -Wpointer-arith\
			-Wnested-externs -Wswitch

SIG_INCLUDES        =   -I$(VOB_BASE_COMMON)\util\h \
			-I$(VOB_BASE_COMMON)\gw\genlib\common\h \
			-I$(VOB_BASE_COMMON)\gw\genlib\os\h \
			-I$(VOB_BASE_COMMON)\gw\common\h \
			-I$(VOB_BASE_COMMON)\port\h

CFLAGS		=	-D_ANSI_C_ -DSIGTRAN_PORT_VXWORKS -DCPU=$(CPU) -DGW_VXWORKS -DGL_VXWORKS -DGEN_LIB -DGEN_LIB_VX $(CPU_CFLAGS) $(COPTIONS)
#-DCPU -DGW_VXWORKS
 
INCLUDES	=	$(COMMON_INCLUDES) $(SIG_INCLUDES)

 
OBJS_PATH	= ..\..\obj\vxworks

HASH_UTIL_OBJS	=	sig_hash.o tcpiplib.o sig_hand.o sig_util.o

UTIL_OBJS	=	sig_hand.o tcpiplib.o

			
sig_hash.o	:	$(VOB_BASE_COMMON)\util\src\sig_hash.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES)  -c $< 
			@echo "\tDone Compiling $<." 

sig_hand.o	:	$(VOB_BASE_COMMON)\util\src\sig_hand.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES)  -c $< 
			@echo "\tDone Compiling $<." 

tcpiplib.o	:	$(VOB_BASE_COMMON)\util\src\tcpiplib.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES)  -c $< 
			@echo "\tDone Compiling $<." 
 
sig_util.o	:	$(VOB_BASE_COMMON)\util\src\sig_util.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES)  -c $< 
			@echo "\tDone Compiling $<." 

all:		$(UTIL_OBJS)
		move *.o $(OBJS_PATH)\ &\
		cd $(OBJS_PATH)\ &\
		ld386 -r  $(UTIL_OBJS)  -o sig_util.o

hash_all:	$(HASH_UTIL_OBJS)
		move *.o $(OBJS_PATH)\ &\
		cd $(OBJS_PATH)\ &\
		ld386 -r  $(HASH_UTIL_OBJS) -o sig_util_hash.o

clean:	
		cd $(OBJS_PATH) &\
		$(RM) *.o
