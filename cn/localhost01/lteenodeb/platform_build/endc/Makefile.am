lib_LTLIBRARIES = libe-utran-enodeb-rrc-stack.la

libe_utran_enodeb_rrc_stack_la_CFLAGS = -DSIGTRAN_PORT_LINUX -DLINUX -DNGN \
	-DNGN_ERROR_CODE -DSIG_COMMON_WITH_CSPL -DSCTP_APPL_CSPL_PORTED \
	-DSCTP_ERROR_ENABLED -DSCTP_UI_SOCK_API $(AM_CFLAGS)
#SPR 19713 change start
#code for 19288 removed
#SPR 19713 change end

ifeq '$(NBIOT_FEAT)' 'enable'
#SPR 21660 changes start_mokshi
libe_utran_enodeb_rrc_stack_la_SOURCES = \
	common/src/rrc_msg_mgmt.c \
	common/src/rrc_common_utils.c \
	common/src/rrc_cp_common.c \
	common/rrmim/src/rrc_rrm_il_composer.c \
	common/rrmim/src/rrc_rrm_il_parser.c \
	common/rrmim/src/rrc_rrmNb_il_composer.c \
	common/rrmim/src/rrc_rrmNb_il_parser.c \
	common/oamim/src/rrc_oam_il_composer.c \
	common/oamim/src/rrc_oam_il_parser.c \
	common/s1apim/src/rrc_s1ap_asn_intrl_enc.c \
	common/s1apim/src/rrc_s1ap_asn_intrl_dec.c \
	common/s1apim/src/rrc_s1ap_asn_intrl_enc_3gpp.c \
	common/s1apim/src/rrc_s1ap_asn_intrl_dec_3gpp.c \
	common/s1ap_oamim/src/rrc_s1apOam_il_composer.c \
	common/s1ap_oamim/src/rrc_s1apOam_il_parser.c \
	common/s1ap_rrmim/src/rrc_s1apRrm_il_composer.c \
	common/s1ap_rrmim/src/rrc_s1apRrm_il_parser.c \
	common/s1ap_wrap/src/sctp_wrappers.c \
	common/s1ap_wrap/src/search_tree_wrap.c \
	common/s1ap_wrap/src/list_wrap.c \
	common/src/rrm_rrc_asn1_convertor.c \
	llim/common/src/rrc_llim_utils.c \
	llim/main/src/rrc_llim_init.c \
	llim/main/src/rrc_llmd.c \
	llim/intf_lib/src/rrc_s1u_il_composer.c \
	llim/intf_lib/src/rrc_s1u_il_parser.c \
	llim/intf_lib/src/rrc_pdcp_il_composer.c \
	llim/intf_lib/src/rrc_pdcp_il_parser.c \
	llim/intf_lib/src/rrc_rlc_il_composer.c \
	llim/intf_lib/src/rrc_rlc_il_parser.c \
	llim/intf_lib/src/rrc_mac_il_composer.c \
	llim/intf_lib/src/rrc_mac_il_parser.c \
	llim/intf_lib/src/rrc_phy_il_composer.c \
	llim/intf_lib/src/rrc_phy_il_parser.c \
	llim/intf_lib/src/rrc_macNb_il_composer.c \
	llim/intf_lib/src/rrc_macNb_il_parser.c \
	llim/intf_lib/src/rrc_phyNb_il_composer.c \
	llim/intf_lib/src/rrc_phyNb_il_parser.c \
	llim/intf_lib/src/rrc_rlcNb_il_composer.c \
	llim/intf_lib/src/rrc_rlcNb_il_parser.c \
	llim/main/src/rrc_llim_oamh.c \
	llim/main/src/rrc_cemh.c \
	llim/main/src/rrc_cemh_fsm.c \
	llim/main/src/rrc_uemh.c \
	llim/main/src/rrc_uemh_fsm.c \
	csc/src/csc_paging_m.c \
	csc/src/csc_bch_m.c \
	csc/src/csc_cell_ctx.c \
	csc/src/csc_cell_m.c \
	csc/src/csc_cell_m_fsm.c \
	csc/src/csc_init.c \
	csc/src/csc_md.c \
	csc/src/csc_oamh.c \
	csc/src/csc_sfn_m.c \
	csc/src/csc_utils.c	\
	oamh/src/oamh_ed.c \
	oamh/src/oamh_fsm.c \
	asn/src/rrc_asn_enc_dec.c \
	asn/src/s1ap_asn_enc_dec_3gpp.c \
	asn/src/s1ap_3gpp.c \
	uecc/interface/src/uecc_csc.c \
	uecc/fsm/src/uecc_ed.c \
	uecc/utils/src/uecc_event_queue.c \
	uecc/fsm/src/uecc_init.c \
	uecc/interface/src/uecc_llim.c \
	uecc/fsm/src/uecc_mainfsm.c \
	uecc/interface/src/uecc_oamh.c \
	uecc/interface/src/uecc_rrm.c \
	uecc/interface/src/uecc_s1ap.c \
	uecc/interface/src/uecc_defaults.c \
	uecc/interface/src/uecc_llim_utils.c \
	uecc/fsm/src/uecc_rcrfsm.c \
	uecc/fsm/src/uecc_rcsfsm.c \
	uecc/fsm/src/uecc_rmfsm.c \
	uecc/fsm/src/uecc_fsm_engine.c \
	uecc/fsm/src/uecc_fsm_nb_engine.c \
	uecc/fsm/src/uecc_icsp.c \
	uecc/fsm/src/uecc_ct_a.c \
	uecc/fsm/src/uecc_isa_a.c \
	uecc/utils/src/uecc_ue_ctx.c \
	uecc/utils/src/uecc_utils.c \
    uecc/utils/src/uecc_ue_timer_utils.c \
	uecc/utils/src/hmac_sha256.c \
	uecc/utils/src/uecc_statistic.c \
	../s1ap/src/s1ap_asn_codec.c \
	../s1ap/src/s1ap_csc.c \
	../s1ap/src/s1ap_init.c \
	../s1ap/src/s1ap_md.c \
	../s1ap/src/s1ap_mme.c \
	../s1ap/src/s1ap_error_indication.c \
	../s1ap/src/s1ap_mme_fsm.c \
	../s1ap/src/s1ap_enb_config_fsm.c \
	../s1ap/src/s1ap_oam.c \
	../s1ap/src/s1ap_uecc.c \
	../s1ap/src/s1ap_utils.c \
	../s1ap/src/s1ap_rrm.c \
	../s1ap/src/s1ap_logger.c
else
#MENB_CHANGES_START
ifeq '$(ENDC_FEAT)' 'enable'
libe_utran_enodeb_rrc_stack_la_SOURCES = \
	common/src/rrc_msg_mgmt.c \
	common/src/rrc_common_utils.c \
	common/src/rrc_cp_common.c \
	common/rrmim/src/rrc_rrm_il_composer.c \
	common/rrmim/src/rrc_rrm_il_parser.c \
	common/oamim/src/rrc_oam_il_composer.c \
	common/oamim/src/rrc_oam_il_parser.c \
	common/s1apim/src/rrc_s1ap_asn_intrl_enc.c \
	common/s1apim/src/rrc_s1ap_asn_intrl_dec.c \
	common/s1apim/src/rrc_s1ap_asn_intrl_enc_3gpp.c \
	common/s1apim/src/rrc_s1ap_asn_intrl_dec_3gpp.c \
	common/s1ap_oamim/src/rrc_s1apOam_il_composer.c \
	common/s1ap_oamim/src/rrc_s1apOam_il_parser.c \
	common/s1ap_rrmim/src/rrc_s1apRrm_il_composer.c \
	common/s1ap_rrmim/src/rrc_s1apRrm_il_parser.c \
	common/s1ap_wrap/src/sctp_wrappers.c \
	common/s1ap_wrap/src/search_tree_wrap.c \
	common/s1ap_wrap/src/list_wrap.c \
	common/src/rrm_rrc_asn1_convertor.c \
	llim/common/src/rrc_llim_utils.c \
	llim/main/src/rrc_llim_init.c \
	llim/main/src/rrc_llmd.c \
	llim/intf_lib/src/rrc_s1u_il_composer.c \
	llim/intf_lib/src/rrc_s1u_il_parser.c \
	llim/intf_lib/src/rrc_pdcp_il_composer.c \
	llim/intf_lib/src/rrc_pdcp_il_parser.c \
	llim/intf_lib/src/rrc_rlc_il_composer.c \
	llim/intf_lib/src/rrc_rlc_il_parser.c \
	llim/intf_lib/src/rrc_mac_il_composer.c \
	llim/intf_lib/src/rrc_mac_il_parser.c \
	llim/intf_lib/src/rrc_phy_il_composer.c \
	llim/intf_lib/src/rrc_phy_il_parser.c \
	llim/main/src/rrc_llim_oamh.c \
	llim/main/src/rrc_cemh.c \
	llim/main/src/rrc_cemh_fsm.c \
	llim/main/src/rrc_uemh.c \
	llim/main/src/rrc_uemh_fsm.c \
	csc/src/csc_paging_m.c \
	csc/src/csc_bch_m.c \
	csc/src/csc_cell_ctx.c \
	csc/src/csc_cell_m.c \
	csc/src/csc_cell_m_fsm.c \
	csc/src/csc_init.c \
	csc/src/csc_md.c \
	csc/src/csc_oamh.c \
	csc/src/csc_sfn_m.c \
	csc/src/csc_utils.c	\
	oamh/src/oamh_ed.c \
	oamh/src/oamh_fsm.c \
	asn/src/rrc_asn_enc_dec.c \
	asn/src/rrc_asn_enc_dec_nr.c \
	asn/src/s1ap_asn_enc_dec_3gpp.c \
	asn/src/s1ap_3gpp.c \
	uecc/interface/src/uecc_csc.c \
	uecc/fsm/src/uecc_ed.c \
	uecc/utils/src/uecc_event_queue.c \
	uecc/fsm/src/uecc_init.c \
	uecc/interface/src/uecc_llim.c \
	uecc/fsm/src/uecc_mainfsm.c \
	uecc/interface/src/uecc_oamh.c \
	uecc/interface/src/uecc_rrm.c \
	uecc/interface/src/uecc_s1ap.c \
	uecc/interface/src/uecc_defaults.c \
	uecc/interface/src/uecc_llim_utils.c \
	uecc/fsm/src/uecc_rcrfsm.c \
	uecc/fsm/src/uecc_rcsfsm.c \
	uecc/fsm/src/uecc_rmfsm.c \
	uecc/fsm/src/uecc_fsm_engine.c \
	uecc/fsm/src/uecc_icsp.c \
	uecc/fsm/src/uecc_ct_a.c \
	uecc/fsm/src/uecc_isa_a.c \
	uecc/utils/src/uecc_ue_ctx.c \
	uecc/utils/src/uecc_utils.c \
    uecc/utils/src/uecc_ue_timer_utils.c \
	uecc/utils/src/hmac_sha256.c \
	uecc/utils/src/uecc_statistic.c \
	../s1ap/src/s1ap_asn_codec.c \
	../s1ap/src/s1ap_csc.c \
	../s1ap/src/s1ap_init.c \
	../s1ap/src/s1ap_md.c \
	../s1ap/src/s1ap_mme.c \
	../s1ap/src/s1ap_error_indication.c \
	../s1ap/src/s1ap_mme_fsm.c \
	../s1ap/src/s1ap_enb_config_fsm.c \
	../s1ap/src/s1ap_oam.c \
	../s1ap/src/s1ap_uecc.c \
	../s1ap/src/s1ap_utils.c \
	../s1ap/src/s1ap_rrm.c \
	../s1ap/src/s1ap_logger.c
else
#MENB_CHANGES_END
libe_utran_enodeb_rrc_stack_la_SOURCES = \
	common/src/rrc_msg_mgmt.c \
	common/src/rrc_common_utils.c \
	common/src/rrc_cp_common.c \
	common/rrmim/src/rrc_rrm_il_composer.c \
	common/rrmim/src/rrc_rrm_il_parser.c \
	common/oamim/src/rrc_oam_il_composer.c \
	common/oamim/src/rrc_oam_il_parser.c \
	common/s1apim/src/rrc_s1ap_asn_intrl_enc.c \
	common/s1apim/src/rrc_s1ap_asn_intrl_dec.c \
	common/s1apim/src/rrc_s1ap_asn_intrl_enc_3gpp.c \
	common/s1apim/src/rrc_s1ap_asn_intrl_dec_3gpp.c \
	common/s1ap_oamim/src/rrc_s1apOam_il_composer.c \
	common/s1ap_oamim/src/rrc_s1apOam_il_parser.c \
	common/s1ap_rrmim/src/rrc_s1apRrm_il_composer.c \
	common/s1ap_rrmim/src/rrc_s1apRrm_il_parser.c \
	common/s1ap_wrap/src/sctp_wrappers.c \
	common/s1ap_wrap/src/search_tree_wrap.c \
	common/s1ap_wrap/src/list_wrap.c \
	common/src/rrm_rrc_asn1_convertor.c \
	llim/common/src/rrc_llim_utils.c \
	llim/main/src/rrc_llim_init.c \
	llim/main/src/rrc_llmd.c \
	llim/intf_lib/src/rrc_s1u_il_composer.c \
	llim/intf_lib/src/rrc_s1u_il_parser.c \
	llim/intf_lib/src/rrc_pdcp_il_composer.c \
	llim/intf_lib/src/rrc_pdcp_il_parser.c \
	llim/intf_lib/src/rrc_rlc_il_composer.c \
	llim/intf_lib/src/rrc_rlc_il_parser.c \
	llim/intf_lib/src/rrc_mac_il_composer.c \
	llim/intf_lib/src/rrc_mac_il_parser.c \
	llim/intf_lib/src/rrc_phy_il_composer.c \
	llim/intf_lib/src/rrc_phy_il_parser.c \
	llim/main/src/rrc_llim_oamh.c \
	llim/main/src/rrc_cemh.c \
	llim/main/src/rrc_cemh_fsm.c \
	llim/main/src/rrc_uemh.c \
	llim/main/src/rrc_uemh_fsm.c \
	csc/src/csc_paging_m.c \
	csc/src/csc_bch_m.c \
	csc/src/csc_cell_ctx.c \
	csc/src/csc_cell_m.c \
	csc/src/csc_cell_m_fsm.c \
	csc/src/csc_init.c \
	csc/src/csc_md.c \
	csc/src/csc_oamh.c \
	csc/src/csc_sfn_m.c \
	csc/src/csc_utils.c	\
	oamh/src/oamh_ed.c \
	oamh/src/oamh_fsm.c \
	asn/src/rrc_asn_enc_dec.c \
	asn/src/s1ap_asn_enc_dec_3gpp.c \
	asn/src/s1ap_3gpp.c \
	uecc/interface/src/uecc_csc.c \
	uecc/fsm/src/uecc_ed.c \
	uecc/utils/src/uecc_event_queue.c \
	uecc/fsm/src/uecc_init.c \
	uecc/interface/src/uecc_llim.c \
	uecc/fsm/src/uecc_mainfsm.c \
	uecc/interface/src/uecc_oamh.c \
	uecc/interface/src/uecc_rrm.c \
	uecc/interface/src/uecc_s1ap.c \
	uecc/interface/src/uecc_defaults.c \
	uecc/interface/src/uecc_llim_utils.c \
	uecc/fsm/src/uecc_rcrfsm.c \
	uecc/fsm/src/uecc_rcsfsm.c \
	uecc/fsm/src/uecc_rmfsm.c \
	uecc/fsm/src/uecc_fsm_engine.c \
	uecc/fsm/src/uecc_icsp.c \
	uecc/fsm/src/uecc_ct_a.c \
	uecc/fsm/src/uecc_isa_a.c \
	uecc/utils/src/uecc_ue_ctx.c \
	uecc/utils/src/uecc_utils.c \
    uecc/utils/src/uecc_ue_timer_utils.c \
	uecc/utils/src/hmac_sha256.c \
	uecc/utils/src/uecc_statistic.c \
	../s1ap/src/s1ap_asn_codec.c \
	../s1ap/src/s1ap_csc.c \
	../s1ap/src/s1ap_init.c \
	../s1ap/src/s1ap_md.c \
	../s1ap/src/s1ap_mme.c \
	../s1ap/src/s1ap_error_indication.c \
	../s1ap/src/s1ap_mme_fsm.c \
	../s1ap/src/s1ap_enb_config_fsm.c \
	../s1ap/src/s1ap_oam.c \
	../s1ap/src/s1ap_uecc.c \
	../s1ap/src/s1ap_utils.c \
	../s1ap/src/s1ap_rrm.c \
	../s1ap/src/s1ap_logger.c
endif
endif


libe_utran_enodeb_rrc_stack_la_LDFLAGS = -version-info 0:1:0

INCLUDES = \
	$(sctp_includedir) \
	$(cspl_includedir) \
	-I$(top_rrcstack_srcdir)/common/include \
	-I$(top_rrcstack_srcdir)/common/rrmim/include \
	-I$(top_rrcstack_srcdir)/common/oamim/include \
	-I$(top_rrcstack_srcdir)/common/s1apim/include \
	-I$(top_rrcstack_srcdir)/common/s1ap_oamim/include \
	-I$(top_rrcstack_srcdir)/common/s1ap_wrap/include \
	-I$(top_rrcstack_srcdir)/llim/common/include \
	-I$(top_rrcstack_srcdir)/llim/main/include \
	-I$(top_rrcstack_srcdir)/llim/intf_lib/include \
	-I$(top_rrcstack_srcdir)/csc/include \
	-I$(top_rrcstack_srcdir)/oamh/include \
	-I$(top_rrcstack_srcdir)/asn/include \
	-I$(top_rrcstack_srcdir)/uecc/fsm/include \
	-I$(top_rrcstack_srcdir)/uecc/interface/include \
	-I$(top_rrcstack_srcdir)/uecc/utils/include \
	-I$(top_rrcstack_srcdir)/../s1ap/include

#SPR 21660 changes start_mokshi
ifeq '$(NBIOT_FEAT)' 'enable'
rrc_headers = \
	common/include/rrc_defines.h \
	common/include/rrc_defines_nb.h \
	common/include/rrc_common_utils.h \
	common/include/rrc_ext_api.h \
	common/include/rrc_intrl_api.h \
	common/include/rrc_handlers.h \
	common/include/rrc_msg_mgmt.h \
	common/include/rrc_oam_intf.h \
	common/include/rrc_oamh_intf.h \
	common/include/rrc_rrm_intf.h \
	common/include/rrc_rrmNb_intf.h \
	common/include/rrc_s1u_intf.h \
	common/include/rrc_s1ap_csc_intf.h \
	common/include/rrc_s1ap_uecc_intf.h \
	common/include/rrc_s1ap_asn_intrl.h \
	common/include/rrc_s1ap_asn_intrl_3gpp.h \
	common/include/rrc_csc_llim_intf.h \
	common/include/rrc_uecc_llim_intf.h \
	common/include/rrc_csc_uecc_intf.h \
	common/include/rrc_pdcp_intf.h \
	common/include/rrc_rlc_intf.h \
	common/include/rrc_mac_intf.h  \
	common/include/rrc_phy_intf.h \
	common/include/rrc_macNb_intf.h  \
	common/include/rrc_phyNb_intf.h \
	common/include/rrc_rlcNb_intf.h \
	common/include/rrc_cp_common.h \
	common/include/rrc_s1apOam_intf.h \
	common/include/rrc_s1apRrm_intf.h \
	common/rrmim/include/rrc_rrm_il_composer.h \
	common/rrmim/include/rrc_rrm_il_parser.h \
	common/rrmim/include/rrc_rrmNb_il_composer.h \
	common/rrmim/include/rrc_rrmNb_il_parser.h \
	common/oamim/include/rrc_oam_il_composer.h \
	common/oamim/include/rrc_oam_il_parser.h \
	common/s1apim/include/rrc_s1ap_asn_intrl_enc.h \
	common/s1apim/include/rrc_s1ap_asn_intrl_dec.h \
	common/s1apim/include/rrc_s1ap_asn_intrl_enc_3gpp.h \
	common/s1apim/include/rrc_s1ap_asn_intrl_dec_3gpp.h \
	common/s1ap_oamim/include/rrc_s1apOam_il_composer.h \
	common/s1ap_oamim/include/rrc_s1apOam_il_parser.h \
	common/s1ap_rrmim/include/rrc_s1apRrm_il_parser.h \
	common/s1ap_rrmim/include/rrc_s1apRrm_il_composer.h \
	common/s1ap_wrap/include/sctp_wrappers.h \
	common/s1ap_wrap/include/search_tree_wrap.h \
	common/s1ap_wrap/include/list_wrap.h \
	common/include/rrm_rrc_asn1_convertor.h \
	llim/common/include/rrc_llim_timers.h \
	llim/common/include/rrc_llim_global_ctx.h \
	llim/common/include/rrc_llim_logger.h \
	llim/common/include/rrc_llim_utils.h \
	llim/intf_lib/include/rrc_s1u_il_composer.h \
	llim/intf_lib/include/rrc_s1u_il_parser.h \
	llim/intf_lib/include/rrc_pdcp_il_composer.h \
	llim/intf_lib/include/rrc_pdcp_il_parser.h \
	llim/intf_lib/include/rrc_rlc_il_composer.h \
	llim/intf_lib/include/rrc_rlc_il_parser.h \
	llim/intf_lib/include/rrc_mac_il_composer.h \
	llim/intf_lib/include/rrc_mac_il_parser.h \
	llim/intf_lib/include/rrc_phy_il_composer.h \
	llim/intf_lib/include/rrc_phy_il_parser.h \
	llim/intf_lib/include/rrc_macNb_il_composer.h \
	llim/intf_lib/include/rrc_macNb_il_parser.h \
	llim/intf_lib/include/rrc_phyNb_il_composer.h \
	llim/intf_lib/include/rrc_phyNb_il_parser.h \
	llim/intf_lib/include/rrc_rlcNb_il_composer.h \
	llim/intf_lib/include/rrc_rlcNb_il_parser.h \
	llim/main/include/rrc_llim_oamh.h \
	llim/main/include/rrc_cemh.h \
	llim/main/include/rrc_llim_cell_ctx.h \
	llim/main/include/rrc_cemh_timers.h \
	llim/main/include/rrc_cemh_fsm.h \
	llim/main/include/rrc_cemh_fsm_states.h \
	llim/main/include/rrc_uemh.h \
	llim/main/include/rrc_llim_ue_ctx.h \
	llim/main/include/rrc_uemh_timers.h \
	llim/main/include/rrc_uemh_fsm.h \
	llim/main/include/rrc_uemh_fsm_states.h \
	csc/include/csc_bch_m.h \
	csc/include/csc_bch_m_sibs.h \
	csc/include/csc_cell_ctx.h \
	csc/include/csc_paging_m.h \
	csc/include/csc_cell_m.h \
	csc/include/csc_cell_m_fsm.h \
	csc/include/csc_cell_m_fsm_states.h \
	csc/include/csc_cell_m_timers.h \
	csc/include/csc_global_ctx.h \
	csc/include/csc_logger.h \
	csc/include/csc_oamh.h \
	csc/include/csc_sfn_m.h \
	csc/include/csc_sfn_m_timers.h \
	csc/include/csc_timers.h \
	csc/include/csc_utils.h \
	oamh/include/oamh_logger.h \
	oamh/include/oamh_ed.h \
	oamh/include/oamh_fsm.h \
	asn/include/rrc_asn_enc_dec.h \
	asn/include/s1ap_asn_enc_dec_3gpp.h \
	asn/include/rtkey.h \
    uecc/utils/include/uecc_ue_timer_utils.h \
	uecc/interface/include/uecc_csc.h \
	uecc/interface/include/uecc_llim.h \
	uecc/interface/include/uecc_oamh.h \
	uecc/interface/include/uecc_rrm.h \
	uecc/interface/include/uecc_s1ap.h \
	uecc/interface/include/uecc_defaults.h \
	uecc/interface/include/uecc_llim_utils.h \
	uecc/utils/include/uecc_event_queue.h \
	uecc/utils/include/uecc_ue_ctx.h \
	uecc/utils/include/uecc_utils.h \
	uecc/utils/include/hmac_sha256.h \
	uecc/utils/include/uecc_statistic.h \
	uecc/fsm/include/uecc_common.h \
	uecc/fsm/include/uecc_fsm_engine.h \
	uecc/fsm/include/uecc_fsm_nb_engine.h \
	uecc/fsm/include/uecc_fsm_common.h \
	uecc/fsm/include/uecc_ed.h \
	uecc/fsm/include/uecc_global_ctx.h \
	uecc/fsm/include/uecc_mainfsm.h \
	uecc/fsm/include/uecc_rcrfsm.h \
	uecc/fsm/include/uecc_rcsfsm.h \
	uecc/fsm/include/uecc_rmfsm.h \
	uecc/fsm/include/uecc_icsp.h \
	uecc/fsm/include/uecc_ct_a.h \
	uecc/fsm/include/uecc_isa_a.h \
	../s1ap/include/s1ap_api.h \
	../s1ap/include/s1ap_asn_codec.h \
	../s1ap/include/s1ap_csc.h \
	../s1ap/include/s1ap_global_ctx.h \
	../s1ap/include/s1ap_handlers.h \
	../s1ap/include/s1ap_logger.h \
	../s1ap/include/s1ap_mme.h \
	../s1ap/include/s1ap_rrm.h \
	../s1ap/include/s1ap_mme_fsm.h \
	../s1ap/include/s1ap_enb_config_fsm.h \
	../s1ap/include/s1ap_oam.h \
	../s1ap/include/s1ap_timers.h \
	../s1ap/include/s1ap_uecc.h \
	../s1ap/include/s1ap_utils.h
#SPR 21660 changes end_mokshi
else
#MENB_CHANGES_START
ifeq '$(ENDC_FEAT)' 'enable'
rrc_headers = \
	common/include/rrc_defines.h \
	common/include/rrc_common_utils.h \
	common/include/rrc_ext_api.h \
	common/include/rrc_intrl_api.h \
	common/include/rrc_handlers.h \
	common/include/rrc_msg_mgmt.h \
	common/include/rrc_oam_intf.h \
	common/include/rrc_oamh_intf.h \
	common/include/rrc_rrm_intf.h \
	common/include/rrc_s1u_intf.h \
	common/include/rrc_s1ap_csc_intf.h \
	common/include/rrc_s1ap_uecc_intf.h \
	common/include/rrc_s1ap_asn_intrl.h \
	common/include/rrc_s1ap_asn_intrl_3gpp.h \
	common/include/rrc_csc_llim_intf.h \
	common/include/rrc_uecc_llim_intf.h \
	common/include/rrc_csc_uecc_intf.h \
	common/include/rrc_pdcp_intf.h \
	common/include/rrc_rlc_intf.h \
	common/include/rrc_mac_intf.h  \
	common/include/rrc_phy_intf.h \
	common/include/rrc_cp_common.h \
	common/include/rrc_s1apOam_intf.h \
	common/include/rrc_s1apRrm_intf.h \
	common/rrmim/include/rrc_rrm_il_composer.h \
	common/rrmim/include/rrc_rrm_il_parser.h \
	common/oamim/include/rrc_oam_il_composer.h \
	common/oamim/include/rrc_oam_il_parser.h \
	common/s1apim/include/rrc_s1ap_asn_intrl_enc.h \
	common/s1apim/include/rrc_s1ap_asn_intrl_dec.h \
	common/s1apim/include/rrc_s1ap_asn_intrl_enc_3gpp.h \
	common/s1apim/include/rrc_s1ap_asn_intrl_dec_3gpp.h \
	common/s1ap_oamim/include/rrc_s1apOam_il_composer.h \
	common/s1ap_oamim/include/rrc_s1apOam_il_parser.h \
	common/s1ap_rrmim/include/rrc_s1apRrm_il_parser.h \
	common/s1ap_rrmim/include/rrc_s1apRrm_il_composer.h \
	common/s1ap_wrap/include/sctp_wrappers.h \
	common/s1ap_wrap/include/search_tree_wrap.h \
	common/s1ap_wrap/include/list_wrap.h \
	common/include/rrm_rrc_asn1_convertor.h \
	llim/common/include/rrc_llim_timers.h \
	llim/common/include/rrc_llim_global_ctx.h \
	llim/common/include/rrc_llim_logger.h \
	llim/common/include/rrc_llim_utils.h \
	llim/intf_lib/include/rrc_s1u_il_composer.h \
	llim/intf_lib/include/rrc_s1u_il_parser.h \
	llim/intf_lib/include/rrc_pdcp_il_composer.h \
	llim/intf_lib/include/rrc_pdcp_il_parser.h \
	llim/intf_lib/include/rrc_rlc_il_composer.h \
	llim/intf_lib/include/rrc_rlc_il_parser.h \
	llim/intf_lib/include/rrc_mac_il_composer.h \
	llim/intf_lib/include/rrc_mac_il_parser.h \
	llim/intf_lib/include/rrc_phy_il_composer.h \
	llim/intf_lib/include/rrc_phy_il_parser.h \
	llim/main/include/rrc_llim_oamh.h \
	llim/main/include/rrc_cemh.h \
	llim/main/include/rrc_llim_cell_ctx.h \
	llim/main/include/rrc_cemh_timers.h \
	llim/main/include/rrc_cemh_fsm.h \
	llim/main/include/rrc_cemh_fsm_states.h \
	llim/main/include/rrc_uemh.h \
	llim/main/include/rrc_llim_ue_ctx.h \
	llim/main/include/rrc_uemh_timers.h \
	llim/main/include/rrc_uemh_fsm.h \
	llim/main/include/rrc_uemh_fsm_states.h \
	csc/include/csc_bch_m.h \
	csc/include/csc_bch_m_sibs.h \
	csc/include/csc_cell_ctx.h \
	csc/include/csc_paging_m.h \
	csc/include/csc_cell_m.h \
	csc/include/csc_cell_m_fsm.h \
	csc/include/csc_cell_m_fsm_states.h \
	csc/include/csc_cell_m_timers.h \
	csc/include/csc_global_ctx.h \
	csc/include/csc_logger.h \
	csc/include/csc_oamh.h \
	csc/include/csc_sfn_m.h \
	csc/include/csc_sfn_m_timers.h \
	csc/include/csc_timers.h \
	csc/include/csc_utils.h \
	oamh/include/oamh_logger.h \
	oamh/include/oamh_ed.h \
	oamh/include/oamh_fsm.h \
	asn/include/rrc_asn_enc_dec.h \
	asn/include/rrc_asn_enc_dec_nr.h \
	asn/include/s1ap_asn_enc_dec_3gpp.h \
	asn/include/rtkey.h \
    uecc/utils/include/uecc_ue_timer_utils.h \
	uecc/interface/include/uecc_csc.h \
	uecc/interface/include/uecc_llim.h \
	uecc/interface/include/uecc_oamh.h \
	uecc/interface/include/uecc_rrm.h \
	uecc/interface/include/uecc_s1ap.h \
	uecc/interface/include/uecc_defaults.h \
	uecc/interface/include/uecc_llim_utils.h \
	uecc/utils/include/uecc_event_queue.h \
	uecc/utils/include/uecc_ue_ctx.h \
	uecc/utils/include/uecc_utils.h \
	uecc/utils/include/hmac_sha256.h \
	uecc/utils/include/uecc_statistic.h \
	uecc/fsm/include/uecc_common.h \
	uecc/fsm/include/uecc_fsm_engine.h \
	uecc/fsm/include/uecc_fsm_common.h \
	uecc/fsm/include/uecc_ed.h \
	uecc/fsm/include/uecc_global_ctx.h \
	uecc/fsm/include/uecc_mainfsm.h \
	uecc/fsm/include/uecc_rcrfsm.h \
	uecc/fsm/include/uecc_rcsfsm.h \
	uecc/fsm/include/uecc_rmfsm.h \
	uecc/fsm/include/uecc_icsp.h \
	uecc/fsm/include/uecc_ct_a.h \
	uecc/fsm/include/uecc_isa_a.h \
	../s1ap/include/s1ap_api.h \
	../s1ap/include/s1ap_asn_codec.h \
	../s1ap/include/s1ap_csc.h \
	../s1ap/include/s1ap_global_ctx.h \
	../s1ap/include/s1ap_handlers.h \
	../s1ap/include/s1ap_logger.h \
	../s1ap/include/s1ap_mme.h \
	../s1ap/include/s1ap_rrm.h \
	../s1ap/include/s1ap_mme_fsm.h \
	../s1ap/include/s1ap_enb_config_fsm.h \
	../s1ap/include/s1ap_oam.h \
	../s1ap/include/s1ap_timers.h \
	../s1ap/include/s1ap_uecc.h \
	../s1ap/include/s1ap_utils.h
else
#MENB_CHANGES_END
rrc_headers = \
	common/include/rrc_defines.h \
	common/include/rrc_common_utils.h \
	common/include/rrc_ext_api.h \
	common/include/rrc_intrl_api.h \
	common/include/rrc_handlers.h \
	common/include/rrc_msg_mgmt.h \
	common/include/rrc_oam_intf.h \
	common/include/rrc_oamh_intf.h \
	common/include/rrc_rrm_intf.h \
	common/include/rrc_s1u_intf.h \
	common/include/rrc_s1ap_csc_intf.h \
	common/include/rrc_s1ap_uecc_intf.h \
	common/include/rrc_s1ap_asn_intrl.h \
	common/include/rrc_s1ap_asn_intrl_3gpp.h \
	common/include/rrc_csc_llim_intf.h \
	common/include/rrc_uecc_llim_intf.h \
	common/include/rrc_csc_uecc_intf.h \
	common/include/rrc_pdcp_intf.h \
	common/include/rrc_rlc_intf.h \
	common/include/rrc_mac_intf.h  \
	common/include/rrc_phy_intf.h \
	common/include/rrc_cp_common.h \
	common/include/rrc_s1apOam_intf.h \
	common/include/rrc_s1apRrm_intf.h \
	common/rrmim/include/rrc_rrm_il_composer.h \
	common/rrmim/include/rrc_rrm_il_parser.h \
	common/oamim/include/rrc_oam_il_composer.h \
	common/oamim/include/rrc_oam_il_parser.h \
	common/s1apim/include/rrc_s1ap_asn_intrl_enc.h \
	common/s1apim/include/rrc_s1ap_asn_intrl_dec.h \
	common/s1apim/include/rrc_s1ap_asn_intrl_enc_3gpp.h \
	common/s1apim/include/rrc_s1ap_asn_intrl_dec_3gpp.h \
	common/s1ap_oamim/include/rrc_s1apOam_il_composer.h \
	common/s1ap_oamim/include/rrc_s1apOam_il_parser.h \
	common/s1ap_rrmim/include/rrc_s1apRrm_il_parser.h \
	common/s1ap_rrmim/include/rrc_s1apRrm_il_composer.h \
	common/s1ap_wrap/include/sctp_wrappers.h \
	common/s1ap_wrap/include/search_tree_wrap.h \
	common/s1ap_wrap/include/list_wrap.h \
	common/include/rrm_rrc_asn1_convertor.h \
	llim/common/include/rrc_llim_timers.h \
	llim/common/include/rrc_llim_global_ctx.h \
	llim/common/include/rrc_llim_logger.h \
	llim/common/include/rrc_llim_utils.h \
	llim/intf_lib/include/rrc_s1u_il_composer.h \
	llim/intf_lib/include/rrc_s1u_il_parser.h \
	llim/intf_lib/include/rrc_pdcp_il_composer.h \
	llim/intf_lib/include/rrc_pdcp_il_parser.h \
	llim/intf_lib/include/rrc_rlc_il_composer.h \
	llim/intf_lib/include/rrc_rlc_il_parser.h \
	llim/intf_lib/include/rrc_mac_il_composer.h \
	llim/intf_lib/include/rrc_mac_il_parser.h \
	llim/intf_lib/include/rrc_phy_il_composer.h \
	llim/intf_lib/include/rrc_phy_il_parser.h \
	llim/main/include/rrc_llim_oamh.h \
	llim/main/include/rrc_cemh.h \
	llim/main/include/rrc_llim_cell_ctx.h \
	llim/main/include/rrc_cemh_timers.h \
	llim/main/include/rrc_cemh_fsm.h \
	llim/main/include/rrc_cemh_fsm_states.h \
	llim/main/include/rrc_uemh.h \
	llim/main/include/rrc_llim_ue_ctx.h \
	llim/main/include/rrc_uemh_timers.h \
	llim/main/include/rrc_uemh_fsm.h \
	llim/main/include/rrc_uemh_fsm_states.h \
	csc/include/csc_bch_m.h \
	csc/include/csc_bch_m_sibs.h \
	csc/include/csc_cell_ctx.h \
	csc/include/csc_paging_m.h \
	csc/include/csc_cell_m.h \
	csc/include/csc_cell_m_fsm.h \
	csc/include/csc_cell_m_fsm_states.h \
	csc/include/csc_cell_m_timers.h \
	csc/include/csc_global_ctx.h \
	csc/include/csc_logger.h \
	csc/include/csc_oamh.h \
	csc/include/csc_sfn_m.h \
	csc/include/csc_sfn_m_timers.h \
	csc/include/csc_timers.h \
	csc/include/csc_utils.h \
	oamh/include/oamh_logger.h \
	oamh/include/oamh_ed.h \
	oamh/include/oamh_fsm.h \
	asn/include/rrc_asn_enc_dec.h \
	asn/include/s1ap_asn_enc_dec_3gpp.h \
	asn/include/rtkey.h \
    uecc/utils/include/uecc_ue_timer_utils.h \
	uecc/interface/include/uecc_csc.h \
	uecc/interface/include/uecc_llim.h \
	uecc/interface/include/uecc_oamh.h \
	uecc/interface/include/uecc_rrm.h \
	uecc/interface/include/uecc_s1ap.h \
	uecc/interface/include/uecc_defaults.h \
	uecc/interface/include/uecc_llim_utils.h \
	uecc/utils/include/uecc_event_queue.h \
	uecc/utils/include/uecc_ue_ctx.h \
	uecc/utils/include/uecc_utils.h \
	uecc/utils/include/hmac_sha256.h \
	uecc/utils/include/uecc_statistic.h \
	uecc/fsm/include/uecc_common.h \
	uecc/fsm/include/uecc_fsm_engine.h \
	uecc/fsm/include/uecc_fsm_common.h \
	uecc/fsm/include/uecc_ed.h \
	uecc/fsm/include/uecc_global_ctx.h \
	uecc/fsm/include/uecc_mainfsm.h \
	uecc/fsm/include/uecc_rcrfsm.h \
	uecc/fsm/include/uecc_rcsfsm.h \
	uecc/fsm/include/uecc_rmfsm.h \
	uecc/fsm/include/uecc_icsp.h \
	uecc/fsm/include/uecc_ct_a.h \
	uecc/fsm/include/uecc_isa_a.h \
	../s1ap/include/s1ap_api.h \
	../s1ap/include/s1ap_asn_codec.h \
	../s1ap/include/s1ap_csc.h \
	../s1ap/include/s1ap_global_ctx.h \
	../s1ap/include/s1ap_handlers.h \
	../s1ap/include/s1ap_logger.h \
	../s1ap/include/s1ap_mme.h \
	../s1ap/include/s1ap_rrm.h \
	../s1ap/include/s1ap_mme_fsm.h \
	../s1ap/include/s1ap_enb_config_fsm.h \
	../s1ap/include/s1ap_oam.h \
	../s1ap/include/s1ap_timers.h \
	../s1ap/include/s1ap_uecc.h \
	../s1ap/include/s1ap_utils.h
endif
endif


asn1_headers = \
	asn/include/rtpersrc/asn1per.h \
	asn/include/rtpersrc/asn1PerCppTypes.h \
	asn/include/rtsrc/asn1compat.h \
	asn/include/rtsrc/rtStream.h \
	asn/include/rtsrc/rtconv.h \
	asn/include/rtsrc/ASN1CUTCTime.h \
	asn/include/rtsrc/asn1CppEvtHndlr.h \
	asn/include/rtsrc/ASN1Context.h \
	asn/include/rtsrc/ASN1TTime.h \
	asn/include/rtsrc/rtSocketSelect.h \
	asn/include/rtsrc/rtBCD.h \
	asn/include/rtsrc/rtbench.h \
	asn/include/rtsrc/rtExternDefs.h \
	asn/include/rtsrc/rtCompare.h \
	asn/include/rtsrc/rtContext.h \
	asn/include/rtsrc/asn1ErrCodes.h \
	asn/include/rtsrc/rtPrintToString.h \
	asn/include/rtsrc/asn1intl.h \
	asn/include/rtsrc/rtPrint.h \
	asn/include/rtsrc/asn1tag.h \
	asn/include/rtsrc/ASN1CSeqOfList.h \
	asn/include/rtsrc/asn1CharSet.h \
	asn/include/rtsrc/ASN1CBitStr.h \
	asn/include/rtsrc/rtPrintToStream.h \
	asn/include/rtsrc/ASN1CTime.h \
	asn/include/rtsrc/ASN1TObjId.h \
	asn/include/rtsrc/rtTable.h \
	asn/include/rtsrc/rtPrintStream.h \
	asn/include/rtsrc/asn1CppTypes.h \
	asn/include/rtsrc/ASN1TOctStr.h \
	asn/include/rtsrc/asn1type.h \
	asn/include/rtsrc/rtToken.h \
	asn/include/rtsrc/rtclock.h \
	asn/include/rtsrc/asn1version.h \
	asn/include/rtsrc/asn1CEvtHndlr.h \
	asn/include/rtsrc/rtCopy.h \
	asn/include/rtsrc/ASN1CGeneralizedTime.h \
	asn/include/rtxsrc/rtxEnum.h \
	asn/include/rtxsrc/OSRTOutputStream.h \
	asn/include/rtxsrc/rtxBitString.h \
	asn/include/rtxsrc/rtxPattern.h \
	asn/include/rtxsrc/rtxErrCodes.h \
	asn/include/rtxsrc/rtxStreamFile.h \
	asn/include/rtxsrc/rtxBuffer.h \
	asn/include/rtxsrc/rtxSocket.h \
	asn/include/rtxsrc/rtxRegExp.h \
	asn/include/rtxsrc/rtxBigInt.h \
	asn/include/rtxsrc/rtxSList.h \
	asn/include/rtxsrc/OSRTOutputStreamIF.h \
	asn/include/rtxsrc/rtxHashMapUndef.h \
	asn/include/rtxsrc/rtxMemBuf.h \
	asn/include/rtxsrc/rtxError.h \
	asn/include/rtxsrc/OSRTCtxtHolderIF.h \
	asn/include/rtxsrc/OSRTMsgBuf.h \
	asn/include/rtxsrc/OSRTContext.h \
	asn/include/rtxsrc/OSRTString.h \
	asn/include/rtxsrc/osSysTypes.h \
	asn/include/rtxsrc/rtxScalarDList.h \
	asn/include/rtxsrc/rtxPrintToStream.h \
	asn/include/rtxsrc/rtxDiagBitTrace.h \
	asn/include/rtxsrc/OSRTBaseType.h \
	asn/include/rtxsrc/rtxStreamMemory.h \
	asn/include/rtxsrc/rtxExternDefs.h \
	asn/include/rtxsrc/rtxXmlQName.h \
	asn/include/rtxsrc/wceAddon.h \
	asn/include/rtxsrc/OSRTMemoryInputStream.h \
	asn/include/rtxsrc/rtxCharStr.h \
	asn/include/rtxsrc/rtxStream.h \
	asn/include/rtxsrc/OSRTSocket.h \
	asn/include/rtxsrc/rtxBase64.h \
	asn/include/rtxsrc/rtxClock.h \
	asn/include/rtxsrc/rtxDList.h \
	asn/include/rtxsrc/rtxBitEncode.h \
	asn/include/rtxsrc/rtxStack.h \
	asn/include/rtxsrc/rtxDiag.h \
	asn/include/rtxsrc/OSRTStreamIF.h \
	asn/include/rtxsrc/rtxUtil.h \
	asn/include/rtxsrc/rtxUnicode.h \
	asn/include/rtxsrc/rtxBitDecode.h \
	asn/include/rtxsrc/rtxHashMap.h \
	asn/include/rtxsrc/rtxCtype.h \
	asn/include/rtxsrc/rtxHashMapStr2Int.h \
	asn/include/rtxsrc/rtxToken.h \
	asn/include/rtxsrc/rtxTokenConst.h \
	asn/include/rtxsrc/rtxFile.h \
	asn/include/rtxsrc/rtxMemLeakCheck.h \
	asn/include/rtxsrc/OSRTInputStream.h \
	asn/include/rtxsrc/rtxUTF8.h \
	asn/include/rtxsrc/OSRTMsgBufIF.h \
	asn/include/rtxsrc/OSRTStringIF.h \
	asn/include/rtxsrc/rtxArrayList.h \
	asn/include/rtxsrc/OSRTMemBuf.h \
	asn/include/rtxsrc/OSRTFastString.h \
	asn/include/rtxsrc/OSRTFileOutputStream.h \
	asn/include/rtxsrc/OSRTMemoryOutputStream.h \
	asn/include/rtxsrc/OSRTSocketOutputStream.h \
	asn/include/rtxsrc/OSRTSocketInputStream.h \
	asn/include/rtxsrc/rtxFloat.h \
	asn/include/rtxsrc/rtxMemory.h \
	asn/include/rtxsrc/OSRTFileInputStream.h \
	asn/include/rtxsrc/rtxDynBitSet.h \
	asn/include/rtxsrc/rtxHashMapStr2UInt.h \
	asn/include/rtxsrc/rtxStreamSocket.h \
	asn/include/rtxsrc/rtxReal.h \
	asn/include/rtxsrc/rtxCommon.h \
	asn/include/rtxsrc/OSRTCtxtHolder.h \
	asn/include/rtxsrc/rtxXmlStr.h \
	asn/include/rtxsrc/OSRTInputStreamIF.h \
	asn/include/rtxsrc/rtxContext.h \
	asn/include/rtxsrc/rtxPrintStream.h \
	asn/include/rtxsrc/rtxPrint.h \
	asn/include/rtxsrc/rtxStreamBuffered.h \
	asn/include/rtxsrc/osMacros.h \
	asn/include/rtxsrc/rtxDecimal.h \
	asn/include/rtxsrc/OSRTUTF8String.h \
	asn/include/rtxsrc/rtxDynPtrArray.h \
	asn/include/rtxsrc/rtxDateTime.h \
	asn/include/rtxsrc/rtxStreamCtxtBuf.h \
	asn/include/rtxsrc/rtxCommonDefs.h \
	asn/include/rtxsrc/OSRTStream.h \
	asn/include/rtxsrc/rtxStreamZlib.h

asn1_libs = \
	asn/lib/libasn1per.a \
	asn/lib/libasn1rt.a

noinst_HEADERS = \
	$(rrc_headers) \
	$(asn1_headers) \
	$(asn1_libs)
