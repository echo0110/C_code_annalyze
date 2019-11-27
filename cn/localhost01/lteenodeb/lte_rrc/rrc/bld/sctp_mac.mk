##############################################################################
#  sctp_mac.mk 
#  Included by sublevel makefiles for Linux Build
# 
#  Flags			Description
# --------------------------------------------
# LINUX      				        Required for Linux Compilation
# SCTP_STATS_ENABLED		        Enable Statistics Collection
# SCTP_ERROR_ENABLED		        Enable Error/Exception Handling
# SCTP_DEBUG_ENABLED		        Enable Debug Traces
# SCTP_NAME_RESOLV_THREAD	        Run a separate thread for Resolving Hostname
# SIGTRAN_LOCAL_TIME_THREAD	    	Run a separate Thread for Maintaining local time
# SCTP_DYNAMIC_IP_SUPPORT	        Enable support for Dynamic IP Configuration
# SCTP_CLI_RECV_COMMON_THREAD	    Have common thread for Client and TL
# SCTP_UI_SU_MESSAGEBASED	        For Message Based Interface with SU
# SCTP_UI_SM_MESSAGEBASED	        For Message Based Interface with SM
# SCTP_RED_SUPPORT		            Redundancy Support
# SCTP_IPC_UDP
# SCTP_ASSOC_STATS_RECOVERY_SUPPORT To provide support for returning 
#                                   Association level statistics of an 
#                                   association before its deletion.
# SIGTRAN_CLOCK_MONOTONIC              Enable using clock_gettime() 
##############################################################################

SCTP_BASE_DIR=$(SIGTRAN_DIR)/sctp
SIGTRAN_COMMON_DIR=$(SIGTRAN_DIR)/common
COM_BGTD_PATH= $(SCTP_BASE_DIR)/basic
#VPATH=$(COM_BGTD_PATH)/stack/obj

#########################################
# Macros for CSPL
#########################################
ROOTDIR		=	$(CSPL_DIR)/build/

LIBS= -lpthread

##########
#Compiler
##########
GCC= $(LTE_CC) 

#################################################
#Compile Time Options/Flags
################################################

SIG_COMMON_FLAGS = -DSIGTRAN_PORT_LINUX 
SCTP_BASIC_FLAGS = -DLINUX
CFLAGS = $(SCTP_BASIC_FLAGS)

#For using CSPL
SCTP_CSPL_CFLAGS = -DNGN -DNGN_ERROR_CODE

SCTP_CSPL_CFLAGS += -DSIG_COMMON_WITH_CSPL
#For if the above application is CSPL based
SCTP_CSPL_CFLAGS += -DSCTP_APPL_CSPL_PORTED

#For IPv6 support, these flags must be enabled 
# SPR 20633 Fix Start
CFLAGS += -DSCTP_INET6_SUPPORT -DSCTP_OS_SOLARIS10
# SPR 20633 Fix End 
SIG_COMMON_FLAGS += -DSIGTRAN_INET6_SUPPORT

CFLAGS += -DSCTP_HEALTH_MONITOR


CFLAGS +=$(SIG_COMMON_FLAGS)

#For DEBUG SUPPORT
CFLAGS += -DSCTP_DEBUG_ENABLED

#For ERROR SUPPORT
CFLAGS += -DSCTP_ERROR_ENABLED

#For STATS SUPPORT
CFLAGS += -DSCTP_STATS_ENABLED

#For ASSOC_LEVEL_STATS FOR ASB PATCH RELEASE
#This flag MUST be enabled only when there
#is function  based interface with su/sm and
#SCTP_STATS_ENABLED flag is also enabled  
#CFLAGS += -DSCTP_ASSOC_STATS_RECOVERY_SUPPORT

#For separate name resolver thread
#CFLAGS += -DSCTP_NAME_RESOLV_THREAD

#For a separate time thread, these flags must be enabled.
#CFLAGS += -DSIGTRAN_LOCAL_TIME_THREAD
#SIG_COMMON_FLAGS += -DSIGTRAN_LOCAL_TIME_THREAD
    
#For using clock_gettime with clock_id = CLOCK_MONOTONIC,
# these flags should be enabled.
# Before enabling these, verify that System supports
# clock_gettime with clock_id = CLOCK_MONOTONIC
#CFLAGS += -DSIGTRAN_CLOCK_MONOTONIC    
#SIG_COMMON_FLAGS += -DSIGTRAN_CLOCK_MONOTONIC    
#LIBS += -lrt   

#For socket interface
CFLAGS += -DSCTP_UI_SOCK_API
CPPFLAGS += -DSCTP_UI_SOCK_API

#For message based interface with su
#CFLAGS += -DSCTP_UI_SU_MESSAGEBASED

#For message based interface with sm
#CFLAGS += -DSCTP_UI_SM_MESSAGEBASED

#Send data with the data pending indication in msg i/f with SU
#CFLAGS += -DSCTP_SEND_DATA_WITH_IND

#For C Structure based message interface
#CFLAGS += -DSCTP_STRUCT_BASED

#For redundancy support, message based should also be enabled
#CFLAGS += -DSCTP_RED_SUPPORT 

#CFLAGS += -DSCTP_CLI_RECV_COM_THREAD 

#For enabling dynamic ip reconfiguration (ASCONF)
#CFLAGS += -DSCTP_DYNAMIC_IP_SUPPORT

#For Configuration of TTL and DSCP
CFLAGS += -DSCTP_IP_HEADER_PARAMS

#For common polling & receiving thread 
#CFLAGS += -DSCTP_CMN_RECV_POLL_THREAD

#For preserving the byte order of ppid as provided by the host 
#CFLAGS += -DSCTP_HOST_PPID

#CFLAGS used by Stack
CFLAGS_STK  =   $(CFLAGS)
CFLAGS_STK  +=  $(SCTP_CSPL_CFLAGS)

#CFLAGS used by Client
CFLAGS_CLI  =   $(CFLAGS)
CFLAGS_CLI  +=  $(SCTP_CSPL_CFLAGS)

######################################
#Compiler Options
######################################


#following for acc/cc
#COPTIONS= -c   -xCC -vc

#for gcc
COPTIONS = -c -g -Wall -Wimplicit -Wmissing-prototypes\
                -Wshadow -Wcomment -Waggregate-return\
                -Wmissing-declarations -Wreturn-type \
                -Wparentheses -Wpointer-arith \
                -fno-builtin -fno-inline-functions \
                -fsigned-char -pipe -Wstrict-prototypes \
                -Wno-format -W -fcond-mismatch -Wnested-externs\
                -Wswitch -fno-strict-aliasing 

#CC = /home1/SUNWspro/SC5.0/bin/CC 
#CC= CC +w +w2

 
CC_STK=  @printf "building :$@ [Flags: $(CFLAGS_STK)] using $(GCC) [options:$(COPTIONS)]\n\n";$(GCC)
CC_CLI=  @printf "building :$@ [Flags: $(CFLAGS_CLI)] using $(GCC) [options:$(COPTIONS)]\n\n";$(GCC)

LD = $(LTE_LD) -r
RM = rm -f 
AR = $(LTE_AR) 

#########################################################
#INCLUDE SERACH PATHS
#########################################################
INCLUDES=       -I$(COM_BGTD_PATH)/stack/co/h\
                -I$(COM_BGTD_PATH)/stack/err/h\
                -I$(COM_BGTD_PATH)/stack/mh/h\
                -I$(COM_BGTD_PATH)/stack/in/h\
                -I$(COM_BGTD_PATH)/stack/ui/mib/h\
                -I$(COM_BGTD_PATH)/stack/ui/sm/h\
                -I$(COM_BGTD_PATH)/stack/ui/su/h\
                -I$(COM_BGTD_PATH)/stack/ui/co/h\
                -I$(COM_BGTD_PATH)/stack/port/os_if_linux/h\
                -I$(COM_BGTD_PATH)/stack/port/tl_if/h\
                -I$(COM_BGTD_PATH)/client/su/h\
                -I$(COM_BGTD_PATH)/stack/ui/port/h\
                -I$(COM_BGTD_PATH)/stack/db/h\
                -I$(COM_BGTD_PATH)/stack/fsm/h\
                -I$(COM_BGTD_PATH)/stack/mm/h\
                -I$(COM_BGTD_PATH)/stack/red/h\
                -I$(COM_BGTD_PATH)/stack/st/h\
                -I$(COM_BGTD_PATH)/stack/dbg/h\
                -I$(COM_BGTD_PATH)/stack/err/h\
                -I$(COM_BGTD_PATH)/stack/err/port/h\
                -I$(COM_BGTD_PATH)/stack/port/co/h \
                -I$(COM_BGTD_PATH)/client/co/h\
                -I$(COM_BGTD_PATH)/stack/port/e_shell/h \
                -I$(COM_BGTD_PATH)/stack/port/res/h \
                -I$(COM_BGTD_PATH)/client/su/h \
                -I$(COM_BGTD_PATH)/client/sm/h \
                -I$(COM_BGTD_PATH)/client/su/port/h\
                -I$(COM_BGTD_PATH)/client/sm/port/h\
		-I$(COM_BGTD_PATH)/stack/port/cspl/h\
		-I$(SIGTRAN_DIR)/common/port/h\
		-I$(SIGTRAN_DIR)/common/port/linux/h\
		-I$(SIGTRAN_DIR)/common/util/h

