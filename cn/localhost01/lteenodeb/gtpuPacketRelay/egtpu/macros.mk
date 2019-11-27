#
# standard macros
#
SHELL := /bin/sh
include $(LTE_ROOT)/make.inc
#
# default configuration
#
ifndef SYS
   SYS := linux
endif
DEBUG := no
OPTIMIZE := yes 
PROFILE := no
USEDEP := yes

ifneq ($(SYS),solaris)
ifneq ($(SYS),linux)
ifneq ($(SYS),vxppc)
ifneq ($(SYS),vx960)
ifneq ($(SYS),vxhppa)
$(error Unknown SYS, should be 'solaris', 'linux', 'vxppc', 'vx960' or 'vxhppa')
endif
endif
endif
endif
endif

ifeq ($(SYS),solaris)
TOOLSET := gcc
ifneq ($(TOOLSET),gcc)
ifneq ($(TOOLSET),sparcworks)
$(error For SYS=solaris, TOOLSET should be 'gcc' or 'sparcworks')
endif
endif
endif

ifeq ($(SYS),linux)
TOOLSET := $(LTE_CC)
ifneq ($(TOOLSET),$(LTE_CC))
$(error For SYS=linux, TOOLSET should be 'gcc')
endif
endif

ifeq ($(SYS),vxppc)
TOOLSET := tornado
ifneq ($(TOOLSET),tornado)
$(error For SYS=vxppc, TOOLSET should be 'tornado')
endif
endif

ifeq ($(SYS),vx960)
TOOLSET := tornado
ifneq ($(TOOLSET),tornado)
$(error For SYS=vx960, TOOLSET should be 'tornado')
endif
endif

ifeq ($(SYS),vxhppa)
TOOLSET := tornado
ifneq ($(TOOLSET),tornado)
$(error For SYS=vxhppa, TOOLSET should be 'tornado')
endif
endif

ifndef PORT
PORT = simple
endif

CC := $(LTE_CC)
AR := $(LTE_AR)

#
# something that seem common to all toolset ...
#
#SPR 2030 -Dependency added for loggingFrameWork
#-DLOG_PRINT_ENABLED -DSHM_PRINT_ENABLED
CPPFLAGS := -DEGTPU_ENABLE_STATS -DEGTPU_ENABLE_TRACE \
	-DSTACK_PRINT=egtpu_stack_printf \
	-I$(ccm_project_path)/shared/h   \
	-I$(ccm_project_path)/../packetRelay/include   \
	-I$(LTE_ROOT)/common/include     \
	-I$(LTE_ROOT)/common/include/lte_port \
	-I$(LTE_ROOT)/common/loggingFrameWork/logging/inc/ \
	-I$(LTE_ROOT)/common/loggingFrameWork/alarm/inc/ \
	-I$(LTE_ROOT)/common/loggingFrameWork/common/inc/ \
	-I$(LTE_ROOT)/commonInterface/interfaces/include/

#SPR17558
CPPFLAGS += ${OAM_EXTERNAL_INCLUDES}
	

ifeq ($(TOOLSET),$(LTE_CC))
#use make.inc cfg
ifeq ($(BUILD_USE_GLB_FLAG),USE_GLB)
CPP := $(LTE_CC) -E

CC := $(LTE_CC)
#SPR 21653 changes start# 
CFLAGS := -pedantic -W -Waggregate-return \
	-Wstrict-prototypes -Wno-unused -Wno-long-long -Wno-variadic-macros -ULOG_PRINT_ENABLED -DSHM_PRINT_ENABLED -UMAC_AUT_TEST

CFLAGS += -DENDC_ENABLED

#SPR 21653 changes end# 
#LOGGING FLAGS
CFLAGS += -DLTE_GTPUPR_LOG_FATAL_ENABLE \
 -DLTE_GTPUPR_LOG_ERROR_ENABLE \
 -ULTE_GTPUPR_LOG_WARNING_ENABLE \
 -ULTE_GTPUPR_LOG_INFO_ENABLE \
 -ULTE_GTPUPR_LOG_BRIEF_ENABLE \
 -ULTE_GTPUPR_LOG_DEBUG_ENABLE \
 -ULTE_GTPUPR_LOG_TTI_DEBUG_ENABLE 

CFLAGS+=$(OPTIM_FLAGS)

ifeq ($(PROFILE),yes)
CFLAGS += -pg
endif

ifeq ($(OPTIMIZE),yes)
CFLAGS += -O3
endif

MKDEP = $(LTE_CC) -M $(CPPFLAGS)
endif
endif

ifeq ($(TOOLSET),sparcworks)
CPP := cc -P

CC := $(LTE_CC)
CFLAGS := -Xc -v
ifeq ($(PROFILE),yes)
CFLAGS += -xpg
endif

ifeq ($(OPTIMIZE),yes)
CFLAGS += -O
endif

MKDEP = $(LTE_CC) -xM $(CPPFLAGS)
endif

ifeq ($(TOOLSET),tornado)

CFLAGS := -pedantic -Wall -W -Waggregate-return \
	-Wstrict-prototypes -Wno-unused


ifeq ($(SYS),vxppc)
CPP := ccppc -E
CC := ccppc -DCPU=PPC604
MKDEP = ccppc -M $(CPPFLAGS) -DCPU=PPC604
AR := arppc
LD := ldppc -r
CFLAGS += -mlongcall
endif

ifeq ($(SYS),vx960)
CPP := cc960 -E
CC := cc960
MKDEP = cc960 -M $(CPPFLAGS)
AR := ar960
LD := ld960
endif

ifeq ($(SYS),vxhppa)
CPP := cchppa -E
CC := cchppa
MKDEP = cchppa -M $(CPPFLAGS)
AR := ar
LD := ld
endif

ifeq ($(PROFILE),yes)
CFLAGS += -pg
endif

ifeq ($(OPTIMIZE),yes)
CFLAGS += -O2
endif

CPPFLAGS += -I$(WIND_BASE)/target/h

endif

ifeq ($(DEBUG),yes)
CFLAGS += -g
endif

