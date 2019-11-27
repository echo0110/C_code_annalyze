lib_LTLIBRARIES = librrc_driver.la
librrc_driver_la_SOURCES = $(cspl_targetdriver_srcdir)/shell.c

librrc_driver_la_CFLAGS = -DSIGTRAN_PORT_LINUX -DLINUX -DNGN \
	-DNGN_ERROR_CODE -DSIG_COMMON_WITH_CSPL -DSCTP_APPL_CSPL_PORTED \
	-DSCTP_ERROR_ENABLED -DSCTP_UI_SOCK_API $(AM_CFLAGS)

INCLUDES = $(cspl_includedir) $(sctp_includedir) -I$(top_rrcstack_srcdir)/../s1ap/include -I$(top_rrcstack_srcdir)/common/s1ap_wrap/include/ -I$(top_rrcstack_srcdir)/common/include -I$(top_rrcstack_srcdir)/asn/asn1c-v613 -I$(top_rrcstack_srcdir)/../rrc/asn/include/

noinst_HEADERS = \
	$(top_srcdir)/runner/drivers/rrc-cmes-udp/shell.c \
	$(top_srcdir)/runner/drivers/rrc-generic-tcp-ipv4/shell.c \
	$(top_srcdir)/runner/drivers/rrc-generic-udp/shell.c
