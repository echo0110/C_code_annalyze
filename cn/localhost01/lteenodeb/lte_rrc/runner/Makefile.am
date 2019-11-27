SUBDIRS = drivers

bin_PROGRAMS = rrc_entity

rrc_entity_SOURCES = rrc_entity.c \
					src/rrc_porting.c
rrc_entity_LDADD = 	$(top_builddir)/runner/drivers/librrc_driver.la \
					$(top_builddir)/rrc/libe-utran-enodeb-rrc-stack.la \
					$(top_rrcstack_srcdir)/asn/lib/libasn1per.a \
					$(top_rrcstack_srcdir)/asn/lib/libasn1rt.a 
if SCTP_IS_ARICENT_SCTP_IPR
rrc_entity_LDADD += $(top_builddir)/sctp/libsctp.la \
					$(top_builddir)/sctp/libsig_os.la 
else
ifeq ($(LTE_CC), arm-linux-gnueabi-gcc)
rrc_entity_LDADD += /home/zhanghh/arm_tools/sysroot/lib/libsctp.so.1
else
ifeq ($(BIT_64_SYSTEM),1)
rrc_entity_LDADD += /usr/lib64/libsctp.so.1
else
rrc_entity_LDADD += /usr/lib/libsctp.so.1
endif
endif
endif
rrc_entity_LDADD += $(top_builddir)/cspl/libcspl_porting.la \
					$(top_builddir)/cspl/libcspl_cpset.la \
					$(top_builddir)/cspl/libcspl_mpset.la \
					$(top_builddir)/cspl/libcspl.la \
					-lpthread

if ADV_LOGGER
rrc_entity_LDADD += -ladvlogging
endif

noinst_HEADERS = simulation_environment.h

INCLUDES = $(cspl_includedir) \
			-I$(top_rrcstack_srcdir)/common/include \
			-I$(top_rrcstack_srcdir)/../s1ap/include \
			-I$(top_rrcstack_srcdir)/asn/asn1c-v613 \
			-I$(top_rrcstack_srcdir)/../runner/include \
