OBJS = oam_config_mgmt_fsm.o oam_config_mgmt_if.o oam_config_tcb.o

all: $(OBJS)

$(OBJS): %.o: $(SRC)%.c
	$(CC) $(CFLAGS) -c $< -o ../$(DST)$@ $(INCL)

clean:
	@for i in $(OBJS); do \
	(rm -rf ../../build/obj/$$i); done

