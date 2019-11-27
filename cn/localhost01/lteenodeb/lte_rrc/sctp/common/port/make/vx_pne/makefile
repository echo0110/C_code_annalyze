##############################################################################
# Changes correspoding to FSS SCTP 6.1
# - A new flag has been added.
#    Name: SIG_VX_CMN_RECV_POLL_THREAD
#    Description:
#       The flag has to be used by FSS SCTP customers on Vx_PNE platform if FSS
#       SCTP stack is compiled with SCTP_CMN_RECV_POLL_THREAD.
##############################################################################

TGT_DIR 			=	$(WIND_BASE)/target

ifeq ($(WIND_HOST_TYPE),sun4-solaris2)
CP				=	cp
MV 				=	mv
RM				=	\rm -fr
MK_SLASH			=	/
endif

ifeq ($(WIND_HOST_TYPE),x86-win32)
CPY				=	cp
MV				= 	mv
RM				=	\rm -rf
MK_SLASH			=	\\
endif

MAKE				=	make
CP				    =	copy
TOOL   				=	gnu
COF  	  			=	objcopy -lpz
OBJ_SUFFIX			=	o

ifeq ($(VXWORKS_RELEASE),5.5.1)

CC              =       ccpentium
CPU             =       PENTIUM
LIB             =       arpentium cru
RANLIB          =       ranlibpentium
LD              =       ldpentium   -N -r
CPU_CFLAGS      =         -c -ansi  -fvolatile -nostdlib -fno-builtin -fno-defer-pop -Wall -DVX_PNE

COPTIONS	=   -c -Wall -DVXW_EXPLICIT_TEMPLATES \
                -fstrength-reduce \
                -Wimplicit -Wmissing-prototypes\
                -Wshadow -Wno-comment -Wno-reorder -Wno-unused -Waggregate-return\
                -Wmissing-declarations -Wreturn-type \
                -Wparentheses -Wpointer-arith \
                -fno-inline-functions \
                -fsigned-char -pipe \
                -Wno-format -W -fcond-mismatch -Wnested-externs\
                -Wswitch

else

CC              =       cc386
CPU             =       PENTIUM
LIB             =       ar386 cru
RANLIB          =       ranlib386
LD              =       ld386   -N -r
CPU_CFLAGS      =       -DVX_IGNORE_GNU_LIBS -DVXWORKS\
                         -mpentium  -nostdinc\
                        -fvolatile -nostdlib -D_REENTRANT\
                        -fno-builtin -fno-defer-pop

COPTIONS	=	-c  -ansi -nostdinc -fno-implicit-templates -DVXW_EXPLICIT_TEMPLATES \
			-fstrength-reduce -fno-builtin -fstrict-prototype -fno-for-scope \
			-Wall -Wno-comment -Wno-reorder  \
			-Wno-unused -Wimplicit -Wcomment\
			-Waggregate-return -Wmissing-declarations\
			-Wreturn-type -Wparentheses -Wpointer-arith\
			-Wnested-externs -Wswitch -Wno-format

endif

VOB_BASE_COMMON	=	$(VOB_BASE_SIGTRAN)$(MK_SLASH)common

COMMON_INCLUDES	=	-I$(TGT_DIR)$(MK_SLASH)h \
			-I$(TGT_DIR)$(MK_SLASH)h$(MK_SLASH)net \
			-I$(TGT_DIR)$(MK_SLASH)h$(MK_SLASH)wrn$(MK_SLASH)coreip\
			-I$(TGT_DIR)$(MK_SLASH)h$(MK_SLASH)wrn$(MK_SLASH)coreip$(MK_SLASH)net\
			-I$(TGT_DIR)$(MK_SLASH)h$(MK_SLASH)sys

SIG_INCLUDES        =   -I$(VOB_BASE_COMMON)$(MK_SLASH)port$(MK_SLASH)h \
						-I$(VOB_BASE_COMMON)$(MK_SLASH)port$(MK_SLASH)vx_pne$(MK_SLASH)h \
						-I$(VOB_BASE_COMMON)$(MK_SLASH)gw$(MK_SLASH)genlib$(MK_SLASH)common$(MK_SLASH)h \
						-I$(VOB_BASE_COMMON)$(MK_SLASH)gw$(MK_SLASH)genlib$(MK_SLASH)os$(MK_SLASH)h \
						-I$(VOB_BASE_COMMON)$(MK_SLASH)gw$(MK_SLASH)common$(MK_SLASH)h\
						-I$(VOB_CSPL_DIR)$(MK_SLASH)h\
						-I$(VOB_BASE_SIGTRAN)$(MK_SLASH)common$(MK_SLASH)cspl_port$(MK_SLASH)vxpne_port

CFLAGS		=	-DSIG_COMMON_WITH_CSPL -D_ANSI_C_ -DSIGTRAN_PORT_VX_PNE -DCPU=$(CPU) -DGW_VX_PNE -DGL_VX_PNE -DGEN_LIB -DGEN_LIB_VX $(CPU_CFLAGS) $(COPTIONS)

# Flag required when FSS SCTP stack compiled with "SCTP_CMN_RECV_POLL_THREAD"
#-DSIG_VX_CMN_RECV_POLL_THREAD

#-DCPU -DGW_VXWORKS
 
INCLUDES	=	$(COMMON_INCLUDES) $(SIG_INCLUDES)

 
OBJS_PATH	= ..$(MK_SLASH)..$(MK_SLASH)obj$(MK_SLASH)vx_pne

OS_OBJS= sig_tmr.o \
	  sig_ipc.o \
	  sig_nw_util.o \
	  sig_cspl_wrap.o \
	  sig_dbg.o \
	  sig_buff.o \
	  sig_mem.o \
	  sig_mutx.o \
	  sig_thr.o \
	  sig_time.o \
      sig_util.o\
      sig_hand.o\
	  sig_ipc_os.o \
      sig_os_mutx.o\
      sig_os_time.o\
      sig_os_thr.o


OS_CLI_OBJS= sig_mutx.o \
	  sig_mem.o \
	  sig_buff.o \
	  sig_dbg.o \
	  sig_util.o \
	  sig_ipc.o \
      sig_nw_util.o\
	  sig_ipc_os.o \
      sig_os_mutx.o\
      sig_os_thr.o\
      sig_os_time.o\
	  sig_thr.o\
	sig_time.o

HASH_OBJS= sig_hash.o 

sig_os_time.o	:	$(VOB_BASE_COMMON)$(MK_SLASH)port$(MK_SLASH)vx_pne$(MK_SLASH)src$(MK_SLASH)sig_os_time.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES)  -c $< 
			@echo "\tDone Compiling $<." 

sig_os_thr.o	:	$(VOB_BASE_COMMON)$(MK_SLASH)port$(MK_SLASH)vx_pne$(MK_SLASH)src$(MK_SLASH)sig_os_thr.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES)  -c $< 
			@echo "\tDone Compiling $<." 

sig_os_mutx.o	:	$(VOB_BASE_COMMON)$(MK_SLASH)port$(MK_SLASH)vx_pne$(MK_SLASH)src$(MK_SLASH)sig_os_mutx.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES)  -c $< 
			@echo "\tDone Compiling $<." 

sig_ipc_os.o	:	$(VOB_BASE_COMMON)$(MK_SLASH)port$(MK_SLASH)vx_pne$(MK_SLASH)src$(MK_SLASH)sig_ipc_os.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES)  -c $< 
			@echo "\tDone Compiling $<." 

sig_util.o	:	$(VOB_BASE_COMMON)$(MK_SLASH)port$(MK_SLASH)src$(MK_SLASH)sig_util.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES)  -c $< 
			@echo "\tDone Compiling $<." 

sig_nw_util.o	:	$(VOB_BASE_COMMON)$(MK_SLASH)port$(MK_SLASH)src$(MK_SLASH)sig_nw_util.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES)  -c $< 
			@echo "\tDone Compiling $<." 

sig_hand.o	:	$(VOB_BASE_COMMON)$(MK_SLASH)port$(MK_SLASH)src$(MK_SLASH)sig_hand.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES)  -c $< 
			@echo "\tDone Compiling $<." 

sig_ipc.o	:	$(VOB_BASE_COMMON)$(MK_SLASH)port$(MK_SLASH)src$(MK_SLASH)sig_ipc.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES)  -c $< 
			@echo "\tDone Compiling $<." 

sig_hash.o	:	$(VOB_BASE_COMMON)$(MK_SLASH)port$(MK_SLASH)src$(MK_SLASH)sig_hash.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES)  -c $< 
			@echo "\tDone Compiling $<." 

sig_cspl_wrap.o	:	$(VOB_BASE_COMMON)$(MK_SLASH)port$(MK_SLASH)src$(MK_SLASH)sig_cspl_wrap.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES)  -c $< 
			@echo "\tDone Compiling $<." 

sig_buff.o	:	$(VOB_BASE_COMMON)$(MK_SLASH)port$(MK_SLASH)src$(MK_SLASH)sig_buff.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES)  -c $< 
			@echo "\tDone Compiling $<." 

sig_dbg.o	:	$(VOB_BASE_COMMON)$(MK_SLASH)port$(MK_SLASH)src$(MK_SLASH)sig_dbg.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES)  -c $< 
			@echo "\tDone Compiling $<." 
 
sig_mem.o	:	$(VOB_BASE_COMMON)$(MK_SLASH)port$(MK_SLASH)src$(MK_SLASH)sig_mem.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES) -c $< 
			@echo "\tDone Compiling $<." 

sig_mutx.o	:	$(VOB_BASE_COMMON)$(MK_SLASH)port$(MK_SLASH)src$(MK_SLASH)sig_mutx.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES) -c $< 
			@echo "\tDone Compiling $<." 
 
sig_thr.o	:	$(VOB_BASE_COMMON)$(MK_SLASH)port$(MK_SLASH)src$(MK_SLASH)sig_thr.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES) -c $< 
			@echo "\tDone Compiling $<." 
 
sig_time.o	:	$(VOB_BASE_COMMON)$(MK_SLASH)port$(MK_SLASH)src$(MK_SLASH)sig_time.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES) -c $< 
			@echo "\tDone Compiling $<." 
 
sig_tmr.o	:	$(VOB_BASE_COMMON)$(MK_SLASH)port$(MK_SLASH)src$(MK_SLASH)sig_tmr.c 
			@echo "\tCompiling $<."	 
			$(CC) $(CFLAGS) $(INCLUDES) -c $< 
			@echo "\tDone Compiling $<." 

os:		$(OS_OBJS)
		$(LD) -r $(OS_OBJS)  -o sig_os.o 
		$(MV) *.o $(OBJS_PATH)$(MK_SLASH)

os_hash:	$(HASH_OBJS)
			$(LD) -r $(HASH_OBJS)  -o sig_os_util.o 
			$(MV) *.o $(OBJS_PATH)$(MK_SLASH)

os_cli:		$(OS_CLI_OBJS)
		$(LD) -r $(OS_CLI_OBJS) -o sig_os_cli.o
		$(MV) *.o $(OBJS_PATH)$(MK_SLASH)

clean:	
		cd $(OBJS_PATH)
		$(RM) *.o
