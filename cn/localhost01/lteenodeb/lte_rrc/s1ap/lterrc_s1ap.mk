GMAKE=gmake

MAKE=${GMAKE}

CC = gcc
AR = ar
RM=rm -f

MAKE_CLEAN=${GMAKE} -i

CFLAGS =  -c  -g -Wall 

LFLAGS = $(OC_FLAGS)

COMPILE = $(CC) $(CFLAGS) ${INCLUDE}

MAKE_BIN = $(CC) $(LFLAGS) $(LIBS_DIR)

    MAKE_LIB = ${AR}

    S1AP_ROOT=${RRC_ROOT}/s1ap

    COMMON_ROOT=${RRC_ROOT}/rrc/common

    COMMON_LIB_DIR=${COMMON_ROOT}/lib

    COMMON_LIB_PATH=-L${COMMON_ROOT}/lib

    INCLUDE+=   -I${RRC_ROOT}/rrc/common/include \
    -I${RRC_ROOT}/rrc/common/oamim/include \
    -I${RRC_ROOT}/rrc/common/rrmim/include \
    -I${RRC_ROOT}/rrc/common/s1apim/include \
    -I${RRC_ROOT}/rrc/llim/common/include \
    -I${RRC_ROOT}/rrc/llim/main/include \
    -I${RRC_ROOT}/rrc/llim/intf_lib/include \
    -I${RRC_ROOT}/rrc/llim/pup/include \
    -I${RRC_ROOT}/rrc/rrc/asn/src \
    -I${RRC_ROOT}/rrc/rrc/asn/include \
    -I${RRC_ROOT}/rrc/csc/include \
    -I${RRC_ROOT}/rrc/asn/include \
    -I${RRC_ROOT}/rrc/common/rrmim/include \
    -I${RRC_ROOT}/rrc/oamh/include \
    -I${RRC_ROOT}/rrc/common/oamim/include \
    -I${RRC_ROOT}/rrc/uecc/fsm/include \
    -I${RRC_ROOT}/rrc/uecc/interface/include \
    -I${RRC_ROOT}/rrc/uecc/utils/include \
    -I${RRC_ROOT}/rrc/asn/include \
    -I${RRC_ROOT}/rrc/common/rrmim/include \
    -I${RRC_ROOT}/rrc/common/s1apim/include \
    -I${RRC_ROOT}/rrc/common/s1ap_oamim/include \
    -I${RRC_ROOT}/rrc/common/s1ap_wrap/include \
    -I${RRC_ROOT}/rrc/asn/include/s1ap \
    -I${RRC_ROOT}/runner/include 

    MKDIR=mkdir -p

    S1AP_BIN_DIR=${RRC_ROOT}/s1ap/bld

    INCLUDE+= -I$(RRC_ROOT)/rrc/common/include/ \
              -I${S1AP_ROOT}/include/ 

ifeq '$(LINUX_PC_TEST)' 'true'
	CFLAGS   += -fprofile-arcs -ftest-coverage
endif