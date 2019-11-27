include ${LTE_ROOT}/make.inc
#SPR 3643 Fix 
MFLAGS = -UMCS_PRB_MAP_DEBUG
# these flags are now derived directly from make.inc
#MFLAGS = -DCYCLE_COUNT_MAC_RLC_PDCP -UCSPL_STATS -URLC_QUEUE_STATS -UACK_NACK_COUNT_STATS -DTDD_CONFIG -DMAC_TDD_WITH_FRAMEWORK -UMCS_PRB_MAP_DEBUG -UMAC_ENABLE_TTI_STATS -UUTFWK_SIMULATION -UMAC_DEBUG_STATS -UKPI_STATS -DDL_UL_SPLIT
INCLUDE_LTE+= -I$(LTE_ROOT)/common/include/lte_port \
          -I${MACCORE_ROOT}/include/common -I${MACCORE_ROOT}/include/scheduler/strategy \
          -I${MACCORE_ROOT}/include/scheduler/dlSchManager \
          -I${MACCORE_ROOT}/include/scheduler/ulSchManager -I${MACCORE_ROOT}/include/scheduler/bcchManager \
          -I${MACCORE_ROOT}/include/scheduler/ccchManager -I${MACCORE_ROOT}/include/scheduler/rachManager \
          -I${MACCORE_ROOT}/include/scheduler/DRXManager \
          -I${MACCORE_ROOT}/include/scheduler/MeasGapHandler \
          -I${MACCORE_ROOT}/include/scheduler/pcchManager  -I${MACCORE_ROOT}/include/scheduler/resourceManager \
	  -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/multiplexer \
          -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/encoder \
          -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/demultiplexer \
          -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/dlHarq \
          -I${MACCORE_ROOT}/include/assemblyDeassemblyUnit/ulHarq \
          -I${MACCORE_ROOT}/include/interfaces/rrc \
          -I${MACCORE_ROOT}/include/interfaces/rlc -I${MACCORE_ROOT}/include/interfaces/phy \
          -I${MACCORE_ROOT}/include/interfaces/oam \
          -I${MACCORE_ROOT}/include/executionScheduler

GETSTATS_SRC = lteMacGetStats.c
GETSTATS_OBJ = $(patsubst %.c,%.o,$(GETSTATS_SRC))

ifeq ($(LINUX_PC_TEST),true)
COMMON_LIB =  -lgcov
endif

all: getStats
clean: 
	@rm -rf getStats $(GETSTATS_OBJ)
getStats: $(GETSTATS_OBJ)
	${MAKE_BIN} -o $@ $<  $(COMMON_LIB)
%.o: %.c
	${CC} $(CFLAGS) ${MFLAGS} ${INCLUDE_LTE} -o $@ $<

