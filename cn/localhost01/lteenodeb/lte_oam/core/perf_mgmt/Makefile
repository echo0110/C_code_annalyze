OBJS = oam_perf_mgmt_utils.o

ifeq ($(LTE_CC), aarch64-linux-gnu-gcc)
CFLAGS+=-UPDCP_GTPU_INTF
endif

all: $(OBJS)

$(OBJS): %.o: $(SRC)%.c
	$(CC) $(CFLAGS) -c $< -o ../$(DST)$@ $(INCL)

clean:
	@for i in $(OBJS); do \
	(rm -rf ../../build/obj/$$i); done
