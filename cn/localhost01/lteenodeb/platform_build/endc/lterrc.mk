###################################################################
# 
#  FILE NAME   : lterrc.mk
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

# Specify RRC Root
RRC_ROOT:=$(subst \,/,..)

#Include Files   
include $(RRC_ROOT)/../runner/lterunner.mk

# CSPL parameters 
RRC_PORT=posix

# The build Mode
MODE:=release
RRC_KNOWN_MODE := debug release profiler debug_with_license release_with_license

# Logger type
LOGGER_TYPE=internal
LOGGER_KNOWN_TYPE := internal advanced none
#

#MENB_CHANGES_START
ENDC_FEAT=disable
ENDC_FEAT_KNOWN_TYPE := enable disable
#MENB_CHANGES_END

EMTC_FEAT=disable
EMTC_FEAT_KNOWN_TYPE := enable disable

NBIOT_FEAT=disable
NBIOT_FEAT_KNOWN_TYPE := enable disable

# M2AP EMBMS feature 
EMBMS_FEAT=disable
EMBMS_FEAT_KNOWN_TYPE := enable disable

# Logging and Alarm Framework integration
LOG_ALM_FRWK=disable
LOG_ALM_FRWK_KNOWN_TYPE := disable enable


# Endian type
ENDIAN_TYPE=little
ENDIAN_KNOWN_TYPE := little big

#Set it to 1 for enabling lst-files generation
RRC_LST_ON := 0

#Set it to 1 when CSC is compiled for FT and SSIT
ifndef RRC_UNIT_TEST_FRAMEWORK
RRC_CSC_FT := 0
else
RRC_CSC_FT := 1
endif

#Set it to 1 when UECC is compiled for SSIT or FT
# for using constant transaction id values in messages 
RRC_UECC_TR_ID_CONST := 1

# Common include folders
RRC_COMMON_INCLUDES = -I${RRC_ROOT}/common/include/multi


RRC_COMMON_INCLUDES += 	-I${RRC_ROOT}/common/include \
		  	-I${RRC_ROOT}/../cspl/h \
 			-I${RRC_ROOT}/../cspl/ports/${RRC_PORT} \
		  	-I${RRC_ROOT}/../runner/include \

ifeq '$(LOG_ALM_FRWK)' 'enable'            
# Logging FrameWork include folders
LOGGING_FRWK_INCLUDES= \
                  -I${RRC_ROOT}/../../common/loggingFrameWork/alarm/inc \
                  -I${RRC_ROOT}/../../common/loggingFrameWork/common/inc \
                  -I${RRC_ROOT}/../../common/loggingFrameWork/logging/inc \
                  -I${RRC_ROOT}/../../common/include

RRC_COMMON_INCLUDES += ${LOGGING_FRWK_INCLUDES}
endif

ifeq '$(LOGGER_TYPE)' 'advanced'
ADV_LOGGING_INCLUDES= -I${RRC_ROOT}/../tools/advlogger/src/include
IN_MEM_LGR_INCLUDES= -I${RUNNER_ROOT}/../../common/in_memory_logging/include/

RRC_COMMON_INCLUDES += ${ADV_LOGGING_INCLUDES} $(IN_MEM_LGR_INCLUDES)
endif

ifeq ($(RLC_PDCP_SPLIT_FLAG), 1)
RRC_DEFINES_debug+= -DCRAN_RLC_PDCP_SPLIT 
RRC_DEFINES_release+= -DCRAN_RLC_PDCP_SPLIT
RRC_DEFINES_profiler+= -DCRAN_RLC_PDCP_SPLIT
RRC_DEFINES_debug_with_license+= -DCRAN_RLC_PDCP_SPLIT
RRC_DEFINES_release_with_license+= -DCRAN_RLC_PDCP_SPLIT
endif
# Output folders
OBJDIR=$(RRC_ROOT)/bld/$(MODE)/obj
LIBDIR=$(RRC_ROOT)/bld/$(MODE)/lib

# Global defines 
RRC_DEFINES_debug+=	-DRRC_ENABLE_TRACE \
	  				-DRRC_DEBUG \
                    -DMEM_PROFILE_ON\
					-US1AP_UT_TESTING\
					-UL3_SSIT_UDP_SETUP -UHEADER_ALIGNMENT -ULOCKLESS_IMPL
RRC_DEFINES_debug+= ${SCTP_DEFINES} -UHEADER_ALIGNMENT
RRC_DEFINES_profiler+=	-DRRC_ENABLE_TRACE \
			  			-DRRC_DEBUG  -UHEADER_ALIGNMENT -DRRC_PROFILER -ULOCKLESS_IMPL
RRC_DEFINES_debug_with_license+= -DRRC_ENABLE_TRACE \
                                 -DRRC_DEBUG \
								 -US1AP_UT_TESTING\
								 -UL3_SSIT_UDP_SETUP -UHEADER_ALIGNMENT -ULOCKLESS_IMPL
RRC_DEFINES_debug_with_license+= ${SCTP_DEFINES} -UHEADER_ALIGNMENT


RRC_DEFINES_profiler+= ${SCTP_DEFINES} -UHEADER_ALIGNMENT

ifeq ($(RRC_UNIT_TEST_FRAMEWORK), 1)
RRC_DEFINES_release+= -DRRC_UNIT_TEST_FRAMEWORK
RRC_COMMON_INCLUDES += -I${RRC_ROOT}/../../../../TESTING/lte_rrc_ut_frwk/common/include
RRC_COMMON_INCLUDES += -I${RRC_ROOT}/../../../../TESTING/lte_rrc_ut_frwk/mac_stub/include
RRC_COMMON_INCLUDES += -I${RRC_ROOT}/../../../../TESTING/lte_rrc_ut_frwk/oam_stub/include
RRC_COMMON_INCLUDES += -I${RRC_ROOT}/../../../../TESTING/lte_rrc_ut_frwk/phy_stub/include
RRC_COMMON_INCLUDES += -I${RRC_ROOT}/../../../../TESTING/lte_rrc_ut_frwk/rrm_stub/include
RRC_COMMON_INCLUDES += -I${RRC_ROOT}/../../../../TESTING/lte_rrc_ut_frwk/rlc_stub/include
RRC_COMMON_INCLUDES += -I${RRC_ROOT}/../../../../TESTING/lte_rrc_ut_frwk/pdcp_stub/include
RRC_COMMON_INCLUDES += -I${RRC_ROOT}/../../../../TESTING/lte_rrc_ut_frwk/s1u_stub/include
RRC_COMMON_INCLUDES += -I${RRC_ROOT}/../../../../TESTING/lte_rrc_ut_frwk/rrm_stub/include
RRC_COMMON_INCLUDES += -I${RRC_ROOT}/../../../../TESTING/lte_rrc_ut_frwk/menb_stub/include
RRC_COMMON_INCLUDES += -I${RRC_ROOT}/../../../../TESTING/lte_rrc_ut_frwk/mme_stub/include
RRC_COMMON_INCLUDES += -I${RRC_ROOT}/../../../../TESTING/lte_rrc_ut_frwk/sgnb_stub/include
endif

RRC_DEFINES_release+=
RRC_DEFINES_release+= ${SCTP_DEFINES} -UHEADER_ALIGNMENT -ULOCKLESS_IMPL

RRC_DEFINES_release_with_license+= ${SCTP_DEFINES} -UHEADER_ALIGNMENT -ULOCKLESS_IMPL

RRC_DEFINES_intlogger := -DLOGGER_TYPE=1
RRC_DEFINES_advlogger := -DLOGGER_TYPE=2

# Compile time flags
RRC_LST_CFLAGS=
ifeq ($(RRC_LST_ON), 1)
	#Use it for lst generation : -Wa,-ahls=$(OBJDIR)/$*.lst -g
	RRC_LST_CFLAGS = -Wa,-ahls=$(subst .o,.lst,$@)
endif



RRC_CFLAGS_debug = -Wall -O0 -g03 $(RRC_LST_CFLAGS)  -Wextra -Wdeclaration-after-statement -Wimplicit -Wmissing-prototypes  -Wcomment -Waggregate-return -Wmissing-declarations -Wreturn-type -Wparentheses -Wpointer-arith -fno-builtin -fno-inline-functions -fsigned-char -pipe -Wstrict-prototypes -Wno-format -W -fcond-mismatch -Wnested-externs -Wswitch -fno-strict-aliasing -Wconversion -Wundef -Wpointer-arith -Wcast-qual -Wcast-align -Wsign-compare -Wendif-labels $(CFLAGS)
RRC_CFLAGS_debug_with_license = -Wall -O0 -g03 $(RRC_LST_CFLAGS) -Wextra -Wdeclaration-after-statement -Wimplicit -Wmissing-prototypes  -Wcomment -Waggregate-return -Wmissing-declarations -Wreturn-type -Wparentheses -Wpointer-arith -fno-builtin -fno-inline-functions -fsigned-char -pipe -Wstrict-prototypes -Wno-format -W -fcond-mismatch -Wnested-externs -Wswitch -fno-strict-aliasing -Wconversion -Wundef -Wpointer-arith -Wcast-qual -Wcast-align -Wsign-compare -Wendif-labels -DLICENSE_FLAG $(CFLAGS)

ifeq ($(RRC_UNIT_TEST_FRAMEWORK), 0)
RRC_CFLAGS_release = -O2 -Wall -Wstrict-prototypes $(CFLAGS) $(EXTCFLAGS)
else
RRC_CFLAGS_release = -g -Wall -Wstrict-prototypes -lgcov -fprofile-arcs -ftest-coverage --coverage $(CFLAGS) $(EXTCFLAGS)
#RRC_CFLAGS_release = -g -Wall -Wstrict-prototypes $(CFLAGS) $(EXTCFLAGS)
endif
RRC_CFLAGS_release_with_license = -O2 -DLICENSE_FLAG $(CFLAGS)
RRC_CFLAGS_profiler = -Wall -g -O0 -fprofile-arcs -ftest-coverage $(RRC_LST_CFLAGS)  -Wextra -Wdeclaration-after-statement $(CFLAGS)


# Compiler to be used
RRC_CC = $(LTE_CC) 

# Archiving command and options
RRC_AR = $(LTE_AR)
RRC_ARFLAGS = rvs

#Ranlib tool command
RRC_RANLIB= $(LTE_RANLIB)

# Delete command
RRC_RM = rm -f

# Object linker
RRC_LINKOBJ = $(LTE_LD) -r
