OBJS = oam_port_main.o oam_port_utils.o

all: $(OBJS)

$(OBJS): %.o: $(SRC)%.c
	$(CC) $(CFLAGS) -c $< -o $(DST)$@ $(INCL)

clean:
	@for i in $(OBJS); do \
	(rm -rf ../build/obj/$$i); done

