include ../make.in
ifeq ($(NBIOT), 0)
ifeq (1,$(OAM_SON_ENABLE))
OBJS = oam_rrm_composer.o oam_rrm_parser.o oam_son_composer.o oam_son_parser.o oam_rrc_il_composer.o oam_rrc_il_parser.o oam_s1ap_il_composer.o oam_s1ap_il_parser.o oam_x2ap_il_composer.o oam_x2ap_il_parser.o oam_s1apCommon_il_composer.o oam_s1apCommon_il_parser.o oam_x2apCommon_il_composer.o oam_x2apCommon_il_parser.o

else
OBJS = oam_rrm_composer.o oam_rrm_parser.o oam_rrc_il_composer.o oam_rrc_il_parser.o oam_s1ap_il_composer.o oam_s1ap_il_parser.o oam_x2ap_il_composer.o oam_x2ap_il_parser.o oam_s1apCommon_il_composer.o oam_s1apCommon_il_parser.o oam_x2apCommon_il_composer.o oam_x2apCommon_il_parser.o
endif
else
ifeq (1,$(OAM_SON_ENABLE))
OBJS = oam_rrm_composer.o oam_rrm_parser.o oam_rrmNb_composer.o oam_rrmNb_parser.o oam_son_composer.o oam_son_parser.o oam_rrc_il_composer.o oam_rrc_il_parser.o oam_s1ap_il_composer.o oam_s1ap_il_parser.o oam_x2ap_il_composer.o oam_x2ap_il_parser.o oam_s1apCommon_il_composer.o oam_s1apCommon_il_parser.o oam_x2apCommon_il_composer.o oam_x2apCommon_il_parser.o 
else
OBJS = oam_rrm_composer.o oam_rrm_parser.o oam_rrmNb_composer.o oam_rrmNb_parser.o oam_rrc_il_composer.o oam_rrc_il_parser.o oam_s1ap_il_composer.o oam_s1ap_il_parser.o oam_x2ap_il_composer.o oam_x2ap_il_parser.o oam_s1apCommon_il_composer.o oam_s1apCommon_il_parser.o oam_x2apCommon_il_composer.o oam_x2apCommon_il_parser.o
endif
endif


# EMBMS_CHANGES_START
ifneq '$(findstring -DLTE_EMBMS_SUPPORTED,$(CFLAGS))' ''
OBJS += oam_m2ap_il_composer.o oam_m2ap_il_parser.o
endif
# EMBMS_CHANGES_END

OBJS += oam_agnt_il_composer.o oam_agnt_il_parser.o  

all: $(OBJS)

$(OBJS): %.o: $(SRC)%.c
	$(CC) $(CFLAGS) -c $< -o $(DST)$@ $(INCL)

clean:
	@for i in $(OBJS); do \
        (rm -rf ../build/obj/$$i); done

