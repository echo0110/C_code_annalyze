ifeq ($(OAM_SON_ENABLE),1)
OBJS = oam_l2_if.o 
else
OBJS = oam_l2_if.o
endif
all: $(OBJS)

$(OBJS): %.o: $(SRC)%.c
	$(CC) $(CFLAGS) -c $< -o $(DST)$@ $(INCL)

clean:
	@for i in $(OBJS); do \
	(rm -rf ../build/obj/$$i); done

