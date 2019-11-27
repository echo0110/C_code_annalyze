include ../make.in

ifeq (1,$(RRH_SUPPORT))
OBJS = oam_qcli_main.o oam_qcli_port.o oam_qcli_utils.o oam_qcli_cmd_tree.o oam_qcli_cmd_tree_rrh.o
else
OBJS = oam_qcli_main.o oam_qcli_port.o oam_qcli_utils.o oam_qcli_cmd_tree.o
ifneq '$(findstring -DLTE_EMBMS_SUPPORTED,$(CFLAGS))' ''
OBJS +=oam_qcli_cmd_tree_embms.o
endif
endif

all: $(OBJS)

$(OBJS): %.o: $(SRC)%.c
	$(CC) $(CFLAGS) -c $< -o $(DST)$@ $(INCL)

clean:
	@for i in $(OBJS); do \
        (rm -rf ../build/obj/$$i); done

