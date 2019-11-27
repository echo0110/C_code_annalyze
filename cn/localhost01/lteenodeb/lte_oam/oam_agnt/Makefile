SRC = src/
OBJS = oam_agnt_main.o  oam_agnt_msg_handler.o  oam_agnt_port.o  oam_agnt_utils.o oam_agnt_parser.o oam_agnt_composer.o

all: $(OBJS)

CFLAGS = 

$(OBJS): %.o: $(SRC)%.c
	$(CC) $(CFLAGS) -c $< -o $(DST)$@ $(INCL)

clean:
	@for i in $(OBJS); do \
        (rm -rf ../build/obj/$$i); done

