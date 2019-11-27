lib_LTLIBRARIES = libsctp.la libsig_os.la

libsctp_la_CFLAGS = -DSIGTRAN_PORT_LINUX -DLINUX -DNGN \
	-DNGN_ERROR_CODE -DSIG_COMMON_WITH_CSPL -DSCTP_APPL_CSPL_PORTED \
	-DSCTP_ERROR_ENABLED -DSCTP_UI_SOCK_API $(AM_CFLAGS)


libsctp_la_SOURCES = \
	$(sctp_bgtddir)/stack/port/os_if/src/s_ostmr.c \
	$(sctp_bgtddir)/stack/port/os_if/src/s_osutil.c \
	$(sctp_bgtddir)/stack/port/os_if/src/s_osmem.c \
	$(sctp_bgtddir)/stack/port/cspl/src/s_cspl_util.c \
	$(sctp_bgtddir)/stack/port/cspl/src/s_cspl_init.c \
	$(sctp_bgtddir)/stack/port/cspl/src/s_cspl_func_handlr.c \
	$(sctp_bgtddir)/stack/port/e_shell/src/s_es.c \
	$(sctp_bgtddir)/stack/port/res/src/s_resnam.c \
	$(sctp_bgtddir)/stack/port/tl_if/src/s_tlif.c \
	$(sctp_bgtddir)/stack/mh/src/s_mhmac.c \
	$(sctp_bgtddir)/stack/mh/src/s_mhbld.c \
	$(sctp_bgtddir)/stack/mh/src/s_mhtbl.c \
	$(sctp_bgtddir)/stack/mh/src/s_mhproc.c \
	$(sctp_bgtddir)/stack/mh/src/s_mhpars.c \
	$(sctp_bgtddir)/stack/db/src/s_dbtcb.c \
	$(sctp_bgtddir)/stack/db/src/s_dbbuf.c \
	$(sctp_bgtddir)/stack/db/src/s_dbutil.c \
	$(sctp_bgtddir)/stack/red/src/s_red.c \
	$(sctp_bgtddir)/stack/co/src/s_colst.c \
	$(sctp_bgtddir)/stack/co/src/s_coad32.c \
	$(sctp_bgtddir)/stack/co/src/s_comd5.c \
	$(sctp_bgtddir)/stack/co/src/s_cocrc32.c \
	$(sctp_bgtddir)/stack/ui/port/src/s_uiipc.c \
	$(sctp_bgtddir)/stack/ui/port/src/s_uiport.c \
	$(sctp_bgtddir)/stack/ui/mib/src/s_mibapi.c \
	$(sctp_bgtddir)/stack/ui/su/src/s_uisusk.c \
	$(sctp_bgtddir)/stack/ui/su/src/s_uiutil.c \
	$(sctp_bgtddir)/stack/ui/su/src/s_uisuif.c \
	$(sctp_bgtddir)/stack/ui/su/src/s_select.c \
	$(sctp_bgtddir)/stack/ui/su/src/s_uisuap.c \
	$(sctp_bgtddir)/stack/ui/co/src/s_uico.c \
	$(sctp_bgtddir)/stack/ui/sm/src/s_uismap.c \
	$(sctp_bgtddir)/stack/in/src/s_in.c \
	$(sctp_bgtddir)/stack/fsm/src/s_fsmdat.c \
	$(sctp_bgtddir)/stack/fsm/src/s_fsmcng.c \
	$(sctp_bgtddir)/stack/fsm/src/s_fsmmtu.c \
	$(sctp_bgtddir)/stack/fsm/src/s_fsmass.c \
	$(sctp_bgtddir)/stack/fsm/src/s_fsmtbl.c \
	$(sctp_bgtddir)/stack/fsm/src/s_fsmtx.c \
	$(sctp_bgtddir)/stack/fsm/src/s_fsmrx.c \
	$(sctp_bgtddir)/stack/dbg/src/s_dbg.c \
	$(sctp_bgtddir)/stack/st/src/s_stats.c \
	$(sctp_bgtddir)/stack/err/port/src/s_errhdl.c \
	$(sctp_bgtddir)/stack/err/src/s_errp.c \
	$(sctp_bgtddir)/stack/mm/src/s_mmmain.c \
	$(sctp_rootdir)/sctp/algos/arAdler/src/arAdler.c \
	$(sctp_rootdir)/sctp/algos/arCRC/src/arCRC.c \
	$(sctp_rootdir)/sctp/algos/Crypto/algo/md5/src/arMD5_hash.c \
	$(sctp_rootdir)/sctp/basic/shared/co/src/s_cobuf.c


####
INCLUDES = $(cspl_includedir) $(sctp_includedir)
####


libsig_os_la_CFLAGS = -DGW_LINUX -DGL_LINUX -DSIGTRAN_PORT_LINUX -DLINUX -DNGN \
	-DNGN_ERROR_CODE -DSIG_COMMON_WITH_CSPL -DSCTP_APPL_CSPL_PORTED \
	-DSCTP_ERROR_ENABLED -DSCTP_UI_SOCK_API $(AM_CFLAGS)

libsig_os_la_SOURCES = \
	$(sctp_commondir)/port/src/sig_tmr.c \
	$(sctp_commondir)/port/src/sig_ipc.c\
	$(sctp_commondir)/port/src/sig_nw_util.c\
	$(sctp_commondir)/port/src/sig_cspl_wrap.c\
	$(sctp_commondir)/port/src/sig_dbg.c\
	$(sctp_commondir)/port/src/sig_buff.c\
	$(sctp_commondir)/port/src/sig_mem.c\
	$(sctp_commondir)/port/src/sig_mutx.c\
	$(sctp_commondir)/port/src/sig_thr.c\
	$(sctp_commondir)/port/src/sig_time.c\
	$(sctp_commondir)/port/src/sig_util.c\
	$(sctp_commondir)/port/src/sig_hand.c\
	$(sctp_commondir)/port/src/../linux/src/sig_ipc_os.c\
	$(sctp_commondir)/port/src/../linux/src/sig_os_mutx.c\
	$(sctp_commondir)/port/src/../linux/src/sig_os_thr.c \
	$(sctp_commondir)/port/src/../linux/src/sig_os_time.c

