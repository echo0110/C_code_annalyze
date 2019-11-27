OBJS = oam_snmp_port_main.o oam_snmp_main.o oam_enb_mib_scalars.o oam_enb_mib_traps.o oam_snmp_agt.o oam_snmp_interface.o oam_enb_mib_tables.o
all: $(OBJS)

BUILDLIBS=`net-snmp-config --libs`
BUILDAGENTLIBS=`net-snmp-config --agent-libs`

$(OBJS): %.o: $(SRC)%.c
	$(CC) $(CFLAGS) $(BUILDAGENTLIBS) $(BUILDLIBS) -c $< -o $(DST)$@ $(INCL)

clean:
	@for i in $(OBJS); do \
	(rm -rf ../build/obj/$$i); done

