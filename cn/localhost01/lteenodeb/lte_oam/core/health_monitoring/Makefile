OBJS = oam_hm.o 

all: $(OBJS)

$(OBJS): %.o: $(SRC)%.c
	$(CC) $(CFLAGS) -c $< -o ../$(DST)$@ $(INCL)

clean:
	@for i in $(OBJS); do \
	(rm -rf ../../build/obj/$$i); done
