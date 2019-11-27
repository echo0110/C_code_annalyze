#####################################################
# Starting Path of X2AP software
#####################################################

# Specify X2AP Root
#X2AP_DIR:=$(subst \,/,.)

include ${X2AP_DIR}/../runner/lterunner.mk

# The build Mode
MODE:=release
#X2AP_KNOWN_MODE := debug release profiler debug_with_license release_with_license

# Logger type
LOGGER_TYPE=internal
#LOGGER_KNOWN_TYPE := internal advanced none
#

# Logging and Alarm Framework integration
LOG_ALM_FRWK=enable
#LOG_ALM_FRWK_KNOWN_TYPE := disable enable
#

# Endian type
ENDIAN_TYPE=little
#ENDIAN_KNOWN_TYPE := little big
#

#MENB_CHANGES_START
ENDC_FEAT=disable
#ENDC_FEAT_KNOWN_TYPE := enable disable
#MENB_CHANGES_END

#####################################################
# X2AP VARIABLES 
#####################################################
X2AP_CSPL_LIBS=	$(CSPL_DIR)/build/cspl.lib.o
X2AP_ASN_LIBS= -lasn1per -lasn1rt
X2AP_LIB = libx2ap.a 
X2AP_EXE = x2ap.exe

#####################################################
# DEBUG and LOG flags - to be set during debugging 
# if needed
#####################################################
X2AP_DEFINES=$(RUNNER_DEFINES_$(MODE))
X2AP_CFLAGS = $(RUNNER_CFLAGS_$(MODE)) 
ARICENT_SCTP_IPR_FLAG=$(SCTP_STACK)
X2AP_DEBUG=0
O2_FLAG=0
X2AP_AR_FLAGS=rv
X2AP_SSIT_SETUP = 0
X2AP_ENB_CFG_UPDATE = 0

#####################################################
# X2AP INCLUDE AND LIBRARY PATHS
#####################################################
INC = -I$(X2AP_DIR)/../rrc/common/include/multi

#MENB_CHANGES_START  
ifeq '$(ENDC_FEAT)' 'enable' 
X2AP_CFLAGS += -DENDC_ENABLED
endif
#MENB_CHANGES_END

ifeq ($(RLC_PDCP_SPLIT_FLAG), 1)
X2AP_CFLAGS += -UCRAN_RLC_PDCP_SPLIT
endif
X2AP_CFLAGS += -DHIGH_SCALABILITY_L3_MAX_UE


ifeq ($(RRC_UNIT_TEST_FRAMEWORK), 1)
X2AP_CFLAGS+= -DRRC_UNIT_TEST_FRAMEWORK -lgcov -fprofile-arcs -ftest-coverage --coverage
#X2AP_CFLAGS+= -DRRC_UNIT_TEST_FRAMEWORK 

INC += -I${X2AP_DIR}/../rrc/llim/intf_lib/include
INC += -I${X2AP_DIR}/../rrc/common/oamim/include/multi/
INC += -I${X2AP_DIR}/../rrc/common/s1ap_oamim/include/
INC += -I${X2AP_DIR}/../rrc/common/rrmim/include/
INC += -I${X2AP_DIR}/../rrc/uecc/fsm/include/
INC += -I${X2AP_DIR}/../rrc/uecc/utils/include/
INC += -I${X2AP_DIR}/../rrc/llim/common/include/
INC += -I${X2AP_DIR}/../rrc/llim/main/include/
INC += -I${X2AP_DIR}/../rrc/common/include/
INC += -I${X2AP_DIR}/../rrc/oamh/include/
INC += -I${X2AP_DIR}/../../../../TESTING/lte_rrc_ut_frwk/common/include/
INC += -I${X2AP_DIR}/../../../../TESTING/lte_rrc_ut_frwk/mac_stub/include/
INC += -I${X2AP_DIR}/../../../../TESTING/lte_rrc_ut_frwk/menb_stub/include/
INC += -I${X2AP_DIR}/../../../../TESTING/lte_rrc_ut_frwk/phy_stub/include/
INC += -I${X2AP_DIR}/../../../../TESTING/lte_rrc_ut_frwk/pdcp_stub/include/
INC += -I${X2AP_DIR}/../../../../TESTING/lte_rrc_ut_frwk/s1u_stub/include/
INC += -I${X2AP_DIR}/../../../../TESTING/lte_rrc_ut_frwk/rlc_stub/include/
INC += -I${X2AP_DIR}/../../../../TESTING/lte_rrc_ut_frwk/rrm_stub/include/
INC += -I${X2AP_DIR}/../../../../TESTING/lte_rrc_ut_frwk/oam_stub/include/
INC += -I${X2AP_DIR}/../../../../TESTING/lte_rrc_ut_frwk/sgnb_stub/include/
INC += -I${X2AP_DIR}/../../../../TESTING/lte_rrc_ut_frwk/mme_stub/include/
endif

#SPR 19713 change start
#change for 19288 removed
#SPR 19713 change end

	INC += \
	-I$(X2AP_DIR)/../rrc/common/include/\
	-I$(X2AP_DIR)/../../common/include/\
	-I$(X2AP_DIR)/../cspl/h/ \
    -I$(X2AP_DIR)/../cspl/ports/${RUNNER_PORT}\
	-I$(X2AP_DIR)/../rrc/asn/include/ \
	-I$(X2AP_DIR)/common/inc/ \
	-I$(X2AP_DIR)/db/inc/ \
	-I$(X2AP_DIR)/encdec/inc/ \
	-I$(X2AP_DIR)/init/inc/ \
	-I$(X2AP_DIR)/msgDist/inc/ \
	-I$(X2AP_DIR)/sctp_intf/inc/ \
	-I$(X2AP_DIR)/x2apsm/inc/ \
	-I$(X2AP_DIR)/common/utils/inc/ \
	-I$(X2AP_DIR)/../rrc/common/s1ap_wrap/include/ \
	-I$(X2AP_DIR)/../s1ap/include/ \
	-I$(X2AP_DIR)/../rrc/common/include/ \
	-I$(X2AP_DIR)/../rrc/uecc/utils/include/ \
	-I$(X2AP_DIR)/../runner/include/ \
	-I$(X2AP_DIR)/../../common/in_memory_logging/include/ 
    
ifeq ($(ARICENT_SCTP_IPR_FLAG), aricent_sctp_ipr)
    INC += $(ARICENT_IPR_INCLUDES)
endif

X2AP_LIB_OBJ_PATH = $(X2AP_DIR)/build/obj
X2AP_LIB_PATH = $(X2AP_DIR)/build
ASNLIBSPATH = \
	-L$(X2AP_DIR)/../rrc/asn/lib/ \

    
#####################################################
# Command Options
#####################################################
ifeq '$(LOGGER_TYPE)' 'internal'
X2AP_DEFINES += $(RUNNER_DEFINES_intlogger)
endif

    ifeq '$(LOGGER_TYPE)' 'advanced'
X2AP_DEFINES += $(RUNNER_DEFINES_advlogger)
INC += -I${X2AP_DIR}/../tools/advlogger/src/
    endif

    ifeq '$(ENDIAN_TYPE)' 'little'
    X2AP_DEFINES += -DLTE_L3_LITTLE_ENDIAN
    endif

    ifeq '$(ENDIAN_TYPE)' 'big'
    X2AP_DEFINES += -DLTE_L3_BIG_ENDIAN
    endif
    
ifeq ($(O2_FLAG), 1)
    X2AP_CFLAGS += -O2
endif

ifeq ($(ARICENT_SCTP_IPR_FLAG), aricent_sctp_ipr)
    X2AP_CFLAGS += -DARICENT_SCTP_IPR
    X2AP_CFLAGS += $(SCTP_FLAGS) 
    X2AP_CFLAGS += -DSCTP_UI_SOCK_API 
    X2AP_CFLAGS += -DSCTP_IP_HEADER_PARAMS 
    X2AP_CFLAGS += -DSCTP_INET6_SUPPORT
ifeq ($(INTEL_T3300),intel_t3300_ca)
    X2AP_CFLAGS += -DINTEL_COMMON
endif
else
    X2AP_CFLAGS += -DLKSCTP
#SPR 23215 CHANGES START
ifeq ($(INTEL_T3300),intel_t3300_ca)
    X2AP_CFLAGS += -DINTEL_COMMON
endif
#SPR 23215 CHANGES END
endif

ifeq ($(X2AP_SSIT_SETUP),1)
    X2AP_CFLAGS += -DX2AP_SSIT_SETUP
endif

ifeq ($(X2AP_ENB_CFG_UPDATE),1)
    X2AP_CFLAGS += -DX2AP_ENB_CFG_UPDATE
endif
        
ifeq ($(X2AP_DEBUG), 1)
    X2AP_CFLAGS += -DX2AP_DEBUG
endif

ifeq ($(X2AP_UNIT_TESTING), 1)
    X2AP_CFLAGS += -DX2AP_UNIT_TESTING
endif

ifeq ($(X2AP_WITHOUT_ASN), 1)
    X2AP_CFLAGS += -DX2AP_WITHOUT_ASN
endif

ifeq ($(OAM_STUB), 1)
    X2AP_CFLAGS += -DOAM_STUB
endif

ifeq ($(X2AP_BOUNDRY_UT_TEST), 1)
    X2AP_CFLAGS += -DX2AP_BOUNDRY_UT_TEST
endif

ifeq ($(X2AP_WITHOUT_ASN), 1)
    X2AP_INC_LIB =  -lpthread
else
    X2AP_INC_LIB =  $(ASNLIBSPATH) $(X2AP_ASN_LIBS) -lpthread
endif


#####################################################
# Commands
#####################################################
DELETE = rm -rf
MAKE = make \
            MODE=$(MODE) \
            LOGGER_TYPE=$(LOGGER_TYPE) \
            LOG_ALM_FRWK=$(LOG_ALM_FRWK)\
            ENDIAN_TYPE=$(ENDIAN_TYPE)
MAKECLEAN = make clean \
            MODE=$(MODE) \
            LOGGER_TYPE=$(LOGGER_TYPE) \
            LOG_ALM_FRWK=$(LOG_ALM_FRWK)\
            ENDIAN_TYPE=$(ENDIAN_TYPE)
COMPILER = $(LTE_CC) $(X2AP_DEFINES) $(X2AP_CFLAGS) $(INC) -c  $^
ARCHIVE  = $(LTE_AR) $(X2AP_AR_FLAGS)
MOVE = mv
CP = cp
CHMOD = chmod
LINKER = $(LTE_CC) -Wl \
            -L$(X2AP_LIB_PATH) ${X2AP_LIB}\
            -L$(X2AP_DIR)/../rrc/bld/$(MODE)/lib -lrrccommon \
            $(SIGTRAN_DIR)sctp/basic/stack/obj/sctp.o \
            $(SIGTRAN_DIR)common/port/obj/linux/sig_os.o \
            -L$(CSPL_DIR)/build/ -lcspl -lporting -lcpset -lmpset \
            $(X2AP_INC_LIB) \
            -o ${X2AP_EXE}\

#####################################################
# Pattern Rules
#####################################################
OBJECTS = $(SRC:%.c=%.o)
SRC = $(shell ls *.c)

%.o : %.c
	$(COMPILER)
