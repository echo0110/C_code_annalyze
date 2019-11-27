###################################################################
# 
#  FILE NAME   : lterunner.mk 
# 
#  DESCRIPTION : This file contains general rules for make. 
# 
#  REVISION HISTORY : 
# 
#  DATE          Name        Reference        Comments 
#  ----          ----        ---------        -------- 
#
#  Copyright (c) 2009, Aricent Inc. 
# 
###################################################################


# The RRC Runner Root folder
RUNNER_ROOT=$(subst \,/,.)

export RRC_UT_FRAMEWORK = 0


ifeq ($(RRC_UT_FRAMEWORK), 1)
   CUNIT_DIR = /home/CUnit-2.1-2
   CUNIT_LIB_DIR=${CUNIT_DIR}/CUnit/Sources/.libs/
   L3_SUIT_DIR=${RUNNER_ROOT}/../l3_aut_suite/lib
   L3_SUIT_SRC_DIR=${RUNNER_ROOT}/../l3_aut_suite/src
   CUNIT_LIBS=/home/CUnit-2.1-2/CUnit/Sources/.libs/libcunit.a
   L3_SUIT_OBJ_DIR=${RUNNER_ROOT}/../l3_aut_suite/obj
   L3_SUIT_INCLUDES=-I${RUNNER_ROOT}/../l3_aut_suite/inc
   L3_SUIT_LIBS=${RUNNER_ROOT}/../l3_aut_suite/lib/librrcAut.a
endif

#RRC_NEW_UT_FRAMEWORK
export RRC_UNIT_TEST_FRAMEWORK = 0

ifeq ($(RRC_UNIT_TEST_FRAMEWORK), 1)
RRC_STUB_PATH=../../../../TESTING/lte_rrc_ut_frwk
SCTP_STACK=lksctp
else
include ${SIGTRAN_DIR}/sctp.mk
endif

ifeq '$(SCTP_STACK)' 'lksctp'
    SCTP_DEFINES+=	-DLKSCTP
    SCTP_DEFINES+=  -DLKSCTP_1_0_11
    ifeq ($(LTE_CC), arm-linux-gnueabi-gcc)
       $(warning "sctp_libs is $(SCTP_LIBS)")
       SCTP_LIBS = /home/zhanghh/arm_tools/sysroot/lib/libsctp.so.1 -lpthread
    else
        ifeq ($(LTE_CC), aarch64-linux-gnu-gcc)
            SCTP_LIBS = /home/zhanghh/sysroot/lib/libsctp.so.1 -lpthread
        else
            ifeq ($(BIT_64_SYSTEM),1)
                SCTP_LIBS += /usr/lib64/libsctp.so.1
            else
                SCTP_LIBS += /usr/lib/libsctp.so.1
            endif
        endif
        SCTP_LIBS += -lpthread
    endif
endif

ifeq ($(RRC_UNIT_TEST_FRAMEWORK), 1)
   CUNIT_DIR = /home/CUnit-2.1-2
   CUNIT_LIB_DIR=${CUNIT_DIR}/CUnit/Sources/.libs/
   L3_SUIT_DIR=${RUNNER_ROOT}/../l3_aut_suite/lib
   L3_SUIT_SRC_DIR=${RUNNER_ROOT}/../l3_aut_suite/src
   CUNIT_LIBS=/home/CUnit-2.1-2/CUnit/Sources/.libs/libcunit.a
   L3_SUIT_OBJ_DIR=${RUNNER_ROOT}/../l3_aut_suite/obj
   L3_SUIT_INCLUDES=-I${RUNNER_ROOT}/../l3_aut_suite/inc
   INCLUDE_CUNIT=/usr/local/include/CUnit
   INCLUDE_OAM_STUB=${RRC_STUB_PATH}/oam_stub/include
   INCLUDE_MAC_STUB=${RRC_STUB_PATH}/mac_stub/include
   INCLUDE_PHY_STUB=${RRC_STUB_PATH}/phy_stub/include
   INCLUDE_RLC_STUB=${RRC_STUB_PATH}/rlc_stub/include
   INCLUDE_PDCP_STUB=${RRC_STUB_PATH}/pdcp_stub/include
   INCLUDE_S1U_STUB=${RRC_STUB_PATH}/s1u_stub/include
   INCLUDE_RRM_STUB=${RRC_STUB_PATH}/rrm_stub/include
   INCLUDE_MENB_STUB=${RRC_STUB_PATH}/menb_stub/include
   INCLUDE_SGNB_STUB=${RRC_STUB_PATH}/sgnb_stub/include
   INCLUDE_MME_STUB=${RRC_STUB_PATH}/mme_stub/include
   INCLUDE_COMM_STUB=${RRC_STUB_PATH}/common/include
   UT_FILES=$(RRC_STUB_PATH)common/src/
endif

# CSPL parameters 
RUNNER_PORT=posix
#RUNNER_DRIVER=rrc-generic-udp
RUNNER_DRIVER=rrc-cmes-udp


# The build Mode
MODE=debug
#MODE=release
RUNNER_KNOWN_MODE := debug release profiler debug_with_license release_with_license

# Logger type
LOGGER_TYPE=internal
LOGGER_KNOWN_TYPE := internal advanced none

# Logging and Alarm Framework integration
LOG_ALM_FRWK=disable
LOG_ALM_FRWK_KNOWN_TYPE := disable enable

# Sector Type

#MENB_CHANGES_START
ENDC_FEAT=disable
ENDC_FEAT_KNOWN_TYPE := enable disable
#MENB_CHANGES_END

NBIOT_FEAT=disable
NBIOT_FEAT_KNOWN_TYPE := enable disable

# M2AP Changes - Start
EMBMS_FEAT=disable
EMBMS_FEAT_KNOWN_TYPE := enable disable
# M2AP Changes - End

# Endian type
ENDIAN_TYPE=little
ENDIAN_KNOWN_TYPE := little big

# Enabling Core Binding
CORE_BINDING =disable
CORE_BINDING_TYPE := disable enable

# Enabling Core Binding for threads
THREAD_CORE_BINDING =disable
THREAD_CORE_BINDING_TYPE := disable enable

#Set it to 1 when CSC is compiled for FT and SSIT
RUNNER_RRC_CSC_FT := 0
ifeq ($(RRC_UNIT_TEST_FRAMEWORK), 1)
   RUNNER_RRC_CSC_FT := 1
endif

#Set it to 1 when UECC is compiled for SSIT or FT
# for using constant transaction id values in messages 
RUNNER_RRC_UECC_TR_ID_CONST := 1

#We create the directory only once
MKD=mkdir -p

############################################################################
# INCLUDE folders for RUNNER
###########################################################################
RUNNER_COMMON_INCLUDES += -I${RUNNER_ROOT}/../rrc/common/include/multi


ifeq ($(RRC_UNIT_TEST_FRAMEWORK), 1)
RUNNER_COMMON_INCLUDES +=	-I${RUNNER_ROOT}/../rrc/common/include \
                            -I${RUNNER_ROOT}/../cspl/h \
                            -I${RUNNER_ROOT}/../cspl/ports/${RUNNER_PORT}\
                            -I${RUNNER_ROOT}/../rrc/asn/include \
                            -I${RUNNER_ROOT}/../rrc/llim/intf_lib/include \
                            -I${RUNNER_ROOT}/../rrc/uecc/fsm/include/ \
                            -I${RUNNER_ROOT}/../rrc/common/include \
                            -I${RUNNER_ROOT}/../rrc/uecc/utils/include/ \
                            -I${RUNNER_ROOT}/../rrc/llim/common/include \
                            -I${RUNNER_ROOT}/../rrc/llim/main/include \
                            -I${RUNNER_ROOT}/../rrc/common/oamim/include/multi/ \
                            -I${RUNNER_ROOT}/../rrc/common/oamim/include/single/ \
                            -I${RUNNER_ROOT}/../rrc/common/rrmim/include/ \
                            -I${RUNNER_ROOT}/../rrc/oamh/include/ \
                            -I${RUNNER_ROOT}/../rrc/common/s1ap_oamim/include/ \
							-I${RUNNER_ROOT}/../s1ap/include \
                            -I${RUNNER_ROOT}/../rrc/common/s1ap_wrap/include \
							-I${RUNNER_ROOT}/../runner/include \
			    -I${RUNNER_ROOT}/../l3_aut_suite/inc \
			    -I${RRC_STUB_PATH}/oam_stub/include \
			    -I${RRC_STUB_PATH}/mac_stub/include \
			    -I${RRC_STUB_PATH}/phy_stub/include \
			    -I${RRC_STUB_PATH}/rlc_stub/include \
			    -I${RRC_STUB_PATH}/pdcp_stub/include \
			    -I${RRC_STUB_PATH}/s1u_stub/include \
			    -I${RRC_STUB_PATH}/rrm_stub/include \
			    -I${RRC_STUB_PATH}/menb_stub/include \
			    -I${RRC_STUB_PATH}/sgnb_stub/include \
			    -I${RRC_STUB_PATH}/mme_stub/include \
			    -I${RRC_STUB_PATH}/common/include
else
RUNNER_COMMON_INCLUDES +=	-I${RUNNER_ROOT}/../rrc/common/include \
                            -I${RUNNER_ROOT}/../cspl/h \
                            -I${RUNNER_ROOT}/../cspl/ports/${RUNNER_PORT}\
                            -I${RUNNER_ROOT}/../rrc/asn/include \
							-I${RUNNER_ROOT}/../s1ap/include \
                            -I${RUNNER_ROOT}/../rrc/common/s1ap_wrap/include \
							-I${RUNNER_ROOT}/../runner/include 
endif
#Including sctp directories
RUNNER_COMMON_INCLUDES += ${SCTP_INCLUDES}

#Set X2AP common includes files having declarations for X2AP init entity
X2AP_COMMON_INCLUDES   =    -I${RUNNER_ROOT}/../x2ap/common/inc \
                            -I${RUNNER_ROOT}/../x2ap/db/inc \
                            -I${RUNNER_ROOT}/../x2ap/encdec/inc \
                            -I${RUNNER_ROOT}/../x2ap/init/inc \
                            -I${RUNNER_ROOT}/../x2ap/msgDist/inc \
                            -I${RUNNER_ROOT}/../x2ap/sctp_intf/inc \
                            -I${RUNNER_ROOT}/../x2ap/x2apsm/inc \
                            -I${RUNNER_ROOT}/../x2ap/common/utils/inc 

RUNNER_COMMON_INCLUDES +=   $(X2AP_COMMON_INCLUDES)

# M2AP Changes - Start
#Set M2AP common includes files having declarations for X2AP init entity
M2AP_COMMON_INCLUDES   =    -I${RUNNER_ROOT}/../m2ap/msg_dist/inc \
                            -I${RUNNER_ROOT}/../m2ap/m2ap_sm/inc \
                            -I${RUNNER_ROOT}/../m2ap/utils/inc \
                            -I${RUNNER_ROOT}/../m2ap/inc \
                            -I${RUNNER_ROOT}/../m2ap/interface/lower_layers/inc \
                            -I${RUNNER_ROOT}/../m2ap/interface/mce/inc \
                            -I${RUNNER_ROOT}/../m2ap/interface/oam/inc \
                            -I${RUNNER_ROOT}/../m2ap/interface/rrm/inc \
                            -I${RUNNER_ROOT}/../m2ap/interface/rrc/inc \
                            -I${RUNNER_ROOT}/../m2ap/interface/sctp/inc \

RUNNER_COMMON_INCLUDES +=   $(M2AP_COMMON_INCLUDES)
# M2AP Changes - End

ifeq '$(LOG_ALM_FRWK)' 'enable'
    #LOGGING FRAMEWORK INCLUDES
    LOGGING_FRWK_INCLUDES= -I${RUNNER_ROOT}/../../common/loggingFrameWork/alarm/inc \
                           -I${RUNNER_ROOT}/../../common/loggingFrameWork/common/inc \
                           -I${RUNNER_ROOT}/../../common/loggingFrameWork/logging/inc \
                           -I${RUNNER_ROOT}/../../common/include

    #LOGGING FRAMEWORK  LIBRARIES
    LOGGING_FRWK_LIBS=${RUNNER_ROOT}/../../common/loggingFrameWork/lib/libalarm.a \
                      ${RUNNER_ROOT}/../../common/loggingFrameWork/lib/liblogging.a

    RUNNER_COMMON_INCLUDES += ${LOGGING_FRWK_INCLUDES}
endif    

ifeq '$(LOGGER_TYPE)' 'advanced'
    ADV_LOGGING_INCLUDES= -I${RUNNER_ROOT}/../tools/advlogger/src/
    IN_MEM_LGR_INCLUDES= -I${RUNNER_ROOT}/../../common/in_memory_logging/include/
    RUNNER_COMMON_INCLUDES += ${ADV_LOGGING_INCLUDES} $(IN_MEM_LGR_INCLUDES)

    ADV_LOGGING_LIBS = $(RUNNER_ROOT)/../rrc/bld/$(MODE)/lib/libadvlogging.a
    IN_MEM_LGR_LIBS  = $(RUNNER_ROOT)/../../common/lib/libinMemLgr.a
endif

################################################################################
# OUTPUT folders for RUNNER
################################################################################
OBJDIR=${RUNNER_ROOT}/$(MODE)/obj
BINDIR=${RUNNER_ROOT}/$(MODE)/bin
LIBDIR=$(RUNNER_ROOT)/$(MODE)/lib

# Global defines 
RUNNER_DEFINES_debug += -DRUNNER_ENABLE_TRACE \
			     	   -DRUNNER_DEBUG -UHEADER_ALIGNMENT

RUNNER_DEFINES_debug += ${SCTP_DEFINES} -UHEADER_ALIGNMENT

RUNNER_DEFINES_debug_with_license += -DRUNNER_ENABLE_TRACE \
                                     -DRUNNER_DEBUG -UHEADER_ALIGNMENT

RUNNER_DEFINES_debug_with_license += ${SCTP_DEFINES} -UHEADER_ALIGNMENT
RUNNER_DEFINES_profiler += -DRUNNER_ENABLE_TRACE \
			     	     -DRUNNER_DEBUG \
			     	     -DMODE_PROFILER_DEFINED -UHEADER_ALIGNMENT -DRRC_PROFILER

RUNNER_DEFINES_profiler += ${SCTP_DEFINES} -UHEADER_ALIGNMENT

RUNNER_DEFINES_release += ${SCTP_DEFINES} -UHEADER_ALIGNMENT

RUNNER_DEFINES_release_with_license += ${SCTP_DEFINES} -UHEADER_ALIGNMENT

RUNNER_DEFINES_intlogger := -DLOGGER_TYPE=1
RUNNER_DEFINES_advlogger := -DLOGGER_TYPE=2

# Compile time flags
RUNNER_CFLAGS_debug = -Wall -g03 -O0  -Wextra -Wdeclaration-after-statement -Wimplicit -Wmissing-prototypes -Wcomment -Waggregate-return -Wmissing-declarations -Wreturn-type -Wparentheses -Wpointer-arith -fno-builtin -fno-inline-functions -fsigned-char -pipe -Wstrict-prototypes -Wno-format -W -fcond-mismatch -Wnested-externs -Wswitch -fno-strict-aliasing -Wundef -Wpointer-arith -Wcast-qual -Wcast-align -Wsign-compare -Wendif-labels -DMEM_PROFILE_ON $(CFLAGS) $(EXTCFLAGS)
RUNNER_CFLAGS_debug_with_license = -Wall -g03 -O0  -Wextra -Wdeclaration-after-statement -Wimplicit -Wmissing-prototypes -Wcomment -Waggregate-return -Wmissing-declarations -Wreturn-type -Wparentheses -Wpointer-arith -fno-builtin -fno-inline-functions -fsigned-char -pipe -Wstrict-prototypes -Wno-format -W -fcond-mismatch -Wnested-externs -Wswitch -fno-strict-aliasing -Wundef -Wpointer-arith -Wcast-qual -Wcast-align -Wsign-compare -Wendif-labels -DLICENSE_FLAG -DMEM_PROFILE_ON $(CFLAGS) $(EXTCFLAGS)
RUNNER_CFLAGS_release = -O2 -Wall -Wstrict-prototypes $(CFLAGS) $(EXTCFLAGS)
ifeq ($(RRC_UNIT_TEST_FRAMEWORK), 0)
RUNNER_CFLAGS_release = -O2 -Wall -Werror -Wstrict-prototypes $(CFLAGS) $(EXTCFLAGS)
else
RUNNER_CFLAGS_release = -g -Wall -Wstrict-prototypes -lgcov -fprofile-arcs -ftest-coverage --coverage $(CFLAGS) $(EXTCFLAGS)
#RUNNER_CFLAGS_release = -g -Wall -Wstrict-prototypes  $(CFLAGS) $(EXTCFLAGS)
endif
RUNNER_CFLAGS_release_with_license = -O2 -DLICENSE_FLAG $(CFLAGS) $(EXTCFLAGS)
ifndef RRC_UNIT_TEST_FRAMEWORK
RUNNER_CFLAGS_profiler = -Wall -Werror -g -O0 -fprofile-arcs -ftest-coverage -Wextra -Wdeclaration-after-statement -Wimplicit -Wmissing-prototypes -Wshadow -Wcomment -Waggregate-return -Wmissing-declarations -Wreturn-type -Wparentheses -Wpointer-arith -fno-builtin -fno-inline-functions -fsigned-char -pipe -Wstrict-prototypes -Wno-format -W -fcond-mismatch -Wnested-externs -Wswitch -fno-strict-aliasing $(CFLAGS) $(EXTCFLAGS)
else
RUNNER_CFLAGS_profiler = -Wall -g -O0 -fprofile-arcs -ftest-coverage -Wextra -Wdeclaration-after-statement -Wimplicit -Wmissing-prototypes -Wshadow -Wcomment -Waggregate-return -Wmissing-declarations -Wreturn-type -Wparentheses -Wpointer-arith -fno-builtin -fno-inline-functions -fsigned-char -pipe -Wstrict-prototypes -Wno-format -W -fcond-mismatch -Wnested-externs -Wswitch -fno-strict-aliasing $(CFLAGS) $(EXTCFLAGS)
endif

################################################################################
# COMMANDS for RUNNER
################################################################################
# Compiler to be used
CC_STK=  @printf "building sctp:$@ [Flags: $(CFLAGS_STK)] using $(GCC) [options:$(COPTIONS)]\n\n";$(GCC)
RUNNER_CC =  $(LTE_CC) 

# Archiving command and options
RUNNER_AR =  $(LTE_AR) rv

# Delete command
RUNNER_RM = rm -f

#Configure command for autoconf used for advanced logger
CONFIGURE=configure --host=powerpc CC="$(LTE_CC)" AR="$(LTE_AR)" CFLAGS="$(RUNNER_CFLAGS_$(MODE))"
