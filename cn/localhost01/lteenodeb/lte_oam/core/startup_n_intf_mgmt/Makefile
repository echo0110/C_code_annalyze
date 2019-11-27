OBJS = oam_populate_l3_timers.o oam_sim.o oam_sim_proc_msg.o oam_sim_utils.o oam_xml_function.o oam_populate_func.o oam_tr69_interface_handler.o oam_populate_rrm_geran_cdma200_params.o 

# EMBMS_CHANGES_START
ifneq '$(findstring -DLTE_EMBMS_SUPPORTED,$(CFLAGS))' ''
OBJS += $(DST1)oam_sim_proc_msg_embms.o $(DST1)oam_populate_embms_params.o
endif
# EMBMS_CHANGES_END

all: $(OBJS)

$(OBJS): %.o: $(SRC)%.c
	$(CC) $(CFLAGS) -c $< -o ../$(DST)$@ $(INCL)

clean:
	@for i in $(OBJS); do \
	(rm -rf ../../build/obj/$$i); done
