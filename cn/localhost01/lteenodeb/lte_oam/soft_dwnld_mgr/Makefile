OBJS = oam_sdm_main.o oam_sdm_utils.o oam_sdm_port.o 

SRC=src/
CC=$(LTE_CC)

$(warning "$(CC) $(CFLAGS) -c $< -o $(DST)$@ $(INCL)")

CFLAGS = $(CFLAGS)
all: $(OBJS)

$(OBJS): %.o: $(SRC)%.c
	$(CC) $(CFLAGS) -c $< -o $(DST)$@ $(INCL)
#	@cp $(OBJS) ../build/obj/
clean:
	@for i in $(OBJS); do \
        (rm -rf ../build/obj/$$i); done

