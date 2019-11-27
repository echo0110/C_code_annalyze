include ${LTE_ROOT}/make.inc

INCLUDE_LTE+= -I$(LTE_ROOT)/rlc/rlccore/include/common \
                  -I${LTE_ROOT}/common/include \
                  -I${LTE_ROOT}/common/loggingFrameWork/logging/inc/ \
                  -I${LTE_ROOT}/common/include/lte_port \
				  -I$(LTE_ROOT)/commonInterface/interfaces/include

RLCSTATS_LIB = -L${LTE_ROOT}/common/lib/
RLCSTATS_SRC = lteRlcGetStats.c
RLCSTATS_OBJ = $(patsubst %.c,%.o,$(RLCSTATS_SRC))
all: getRlcStats
clean: 
	@rm -rf getRlcStats $(RLCSTATS_OBJ)
getRlcStats: $(RLCSTATS_OBJ)
	${MAKE_BIN} $(INCLUDE_LTE) $(RLCSTATS_LIB) -g -o $@ $< -lframework -lm -lrt
%.o: %.c
	${CC} $(CFLAGS) $(INCLUDE_LTE) -o $@ $<
