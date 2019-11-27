# default mode
MODE = release

SSIT_SETUP=1

SCTP_STACK=lksctp
#  SPR 20430 Changes Start #
KNOWN_ARCH := i686
#  SPR 20430 Changes End #
KNOWN_MODE := debug release profiler

CPPFLAGS := -I$(SIMROOT)/protocolSimulator/include

C_ADD_DEF :=

LIB_PATH := $(SIMROOT)/lib


ifeq '$(SCTP_STACK)' 'aricent_sctp_ipr'
include $(SIMROOT)/sctp_mac.mk
	
	C_ADD_DEF += -DSCTP -DARICENT_SCTP_IPR $(SCTP_DEFINES) -DSSIT_SETUP
    
    CPPFLAGS += $(SCTP_INCLUDES) 
    CPPFLAGS += -I$(RRC_ROOT)/rrc/common/s1ap_wrap/include \
                -I$(RRC_ROOT)/s1ap/include \
                -I$(RRC_ROOT)/rrc/common/include \
                -I$(CSPL_DIR)/ports/linux\
        		-I$(CSPL_DIR)/h 
        		
    SCTPOBJFLAGS := $(SIGTRAN_DIR)/common/port/obj/linux/sig_os.o $(SIGTRAN_DIR)/sctp/basic/stack/obj/sctp.o
    
    LIB_PATH += $(SIGTRAN_DIR)/sctp/basic/stack/obj \
                $(CSPL_DIR)/build \
                $(RRC_ROOT)/rrc/bld/debug/lib \
                $(RRC_ROOT)/rrc/bld/release/lib \
                $(RRC_ROOT)/rrc/asn/lib

      SCTP_LIBS := 
else
    SCTPOBJFLAGS :=
endif

ifeq '$(SCTP_STACK)' 'lksctp'
	
	C_ADD_DEF += -DSCTP -DLKSCTP -DSSIT_SETUP
    
    CPPFLAGS += -I$(RRC_ROOT)/rrc/common/s1ap_wrap/include \
                -I$(RRC_ROOT)/s1ap/include \
#  -I$(RRC_ROOT)/rrc/common/include \
#               -I$(CSPL_DIR)/ports/linux\
#       		-I$(CSPL_DIR)/h 
        		
#   SCTPOBJFLAGS := $(SIGTRAN_DIR)/common/port/obj/linux/sig_os.o $(SIGTRAN_DIR)/sctp/basic/stack/obj/sctp.o
    
     LIB_PATH +=$(RRC_ROOT)/rrc/bld/debug/lib \
                $(RRC_ROOT)/rrc/bld/release/lib \
                $(RRC_ROOT)/rrc/asn/lib\
		        /usr/lib       
ifeq ($(BIT_64_SYSTEM),1)
      SCTP_LIBS :=/usr/lib64/libsctp.so.1
else   
      SCTP_LIBS :=/usr/lib/libsctp.so.1 
endif

endif

##C_ADD_DEF := -DSKIP_BYTE_ORDER_PROCESS
# Define SKIP_BYTE_ORDER_PROCESS to not process byte order in messages
# between modules in case when all modules run on platforms with same
# byte order. It can improve performance.
# Affects only on messages between modules, all other information
# (like Network packets in ROHC atc.) are processed taking care of byte order.

##C_ADD_DEF := -DMSG_BYTE_ORDER_MSB
# Define MSG_BYTE_ORDER_MSB to use MSB (Network) BYTE ORDER in messages between modules.
# When it is not defined (default) LSB (Intel) BYTE ORDER is used in messages between modules.
# This flag affects only when SKIP_BYTE_ORDER_PROCESS is NOT defined.

C_FLAGS_i386 := -march=i686
C_FLAGS_release := -O2 -fno-strength-reduce
C_FLAGS_debug := -Wall -g -DUT_TESTING 
C_FLAGS_profiler := -Wall -g -O0 -fprofile-arcs -ftest-coverage -DUT_TESTING 

# Possible places to place C,CXX,CPP flags are
# <x>FLAGS
# <x>FLAGS_<arch>
# <x>FLAGS_<mode>
# <x>FLAGS_<arch>_<mode>
# where x is on of 
#   "CPP" - preprocessor flags
#   "C_" - common flags for C and C++
#   "C" - C specific flags
#   "CXX" - C++ specific flags
#
# compilers essentially get the flags in the following order 
# ("x" is "C" or "CXX", "y" refers to target specific flags)
#
# C_FLAGS C_FLAGS_<arch> C_FLAGS_<mode> C_FLAGS_<arch>_<mode> 
# <x>FLAGS <x>FLAGS_<arch> <x>FLAGS_<mode> <x>FLAGS_<arch>_<mode>
# <y>.<x>FLAGS
# CPPFLAGS CPPFLAGS_<arch> CPPFLAGS_<mode> CPPFLAGS_<arch>_<mode>
# <y>.CPPFLAGS
#
# linker (executed via g++ regardless of source file types) 
# gets the set of flags as for C++ compilation followed by
# LDFLAGS LDFLAGS_<arch> LDFLAGS_<mode> LDFLAGS_<arch>_<mode>
# <y>.LDFLAGS

LIB_INSTALL_DIR := $(SIMROOT)/lib

CONFIG_DIR := $(MAKE_ROOT)/config

POD2MAN_FLAGS = --center "Aaa" --release '' 

### 
# default destination paths (relative to build dir) for various target types.

executable_DESTDIR = ../../bin
lib_DESTDIR = ../../lib
so_DESTDIR = ../../lib
podman_DESTDIR = usr/share/man

###
# default source paths (relative to user.mk defining the target) for various target types

podman_SRCDIR = /man

###
# default permissions per target type

CHMOD_TYPE_executable = 755
CHMOD_TYPE_so = 755
CHMOD_TYPE_podman = 644

###
# default permissions per directory

CHMOD_DIR_etc = 644
CHMOD_DIR_etc/init.d = 755
CHMOD_DIR_lib = 755
CHMOD_DIR_sbin = 755
###
