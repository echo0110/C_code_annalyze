lib_LTLIBRARIES = libcspl.la libcspl_mpset.la libcspl_cpset.la libcspl_porting.la

libcspl_la_SOURCES = \
	$(cspl_srcdir)/src/es.c				\
	$(cspl_srcdir)/src/run.c			\
	$(cspl_srcdir)/src/manifest.c		\
	$(cspl_srcdir)/src/cspl.c			\
	$(cspl_srcdir)/src/cpool.c			\
	$(cspl_srcdir)/src/stacklayer.c		\
	$(cspl_srcdir)/src/ylib-bitmap.c	\
	$(cspl_srcdir)/src/ylib-bufop.c		\
	$(cspl_srcdir)/src/ylib-hash.c		\
	$(cspl_srcdir)/src/ylib-pool.c		\
	$(cspl_srcdir)/src/ylib-tree.c		\
	$(cspl_srcdir)/src/ylib-bitstring.c	\
	$(cspl_srcdir)/src/ylib-crc.c		\
	$(cspl_srcdir)/src/ylib-list.c		\
	$(cspl_srcdir)/src/ylib-squeue.c

libcspl_mpset_la_SOURCES = $(cspl_srcdir)/src/mpoolset.c
libcspl_cpset_la_SOURCES = $(cspl_srcdir)/src/cpoolset.c

libcspl_la_LDFLAGS = -version-info 6:1:5
libcspl_mpset_la_LDFLAGS = -version-info 6:1:5
libcspl_cpset_la_LDFLAGS = -version-info 6:1:5

noinst_HEADERS = $(cspl_srcdir)/h/cspl.h \
				 $(cspl_srcdir)/h/shell.h \
				 $(cspl_srcdir)/h/stacklayer.h \
				 $(cspl_srcdir)/h/ylib.h \
				 $(cspl_targetport_srcdir)/os.h

libcspl_porting_la_SOURCES = $(cspl_targetport_srcdir)/os.c
libcspl_porting_la_LDFLAGS = -version-info 1:0:0

INCLUDES = $(cspl_includedir) -I$(top_rrcstack_srcdir)/common/include

# Put all not used directly CSPL things into distribution
EXTRA_DIST = \
	$(cspl_srcdir)/samples/Makefile \
	$(cspl_srcdir)/samples/sample-port/port.h \
	$(cspl_srcdir)/samples/sample-port/os.h \
	$(cspl_srcdir)/samples/sample-port/port.c \
	$(cspl_srcdir)/samples/sample-port/os.mk \
	$(cspl_srcdir)/samples/sample-stack.c \
	$(cspl_srcdir)/samples/sample-stack.h \
	$(cspl_srcdir)/samples/sample-main.c \
	\
	$(cspl_srcdir)/build/Makefile_win_cspl \
	$(cspl_srcdir)/build/cspl.ncb \
	$(cspl_srcdir)/build/Makefile \
	$(cspl_srcdir)/build/Makefile_win_porting \
	$(cspl_srcdir)/build/libcpset.dsp \
	$(cspl_srcdir)/build/cspl.opt \
	$(cspl_srcdir)/build/libcspl.plg \
	$(cspl_srcdir)/build/libcspl.dsp \
	$(cspl_srcdir)/build/libporting.plg \
	$(cspl_srcdir)/build/cspl.mk \
	$(cspl_srcdir)/build/libmpset.plg \
	$(cspl_srcdir)/build/libmpset.dsp \
	$(cspl_srcdir)/build/libcpset.plg \
	$(cspl_srcdir)/build/cspl.dsw \
	$(cspl_srcdir)/build/libporting.dsp \
	\
	$(cspl_srcdir)/utils/logger/samples/rrc_api_defs.h \
	$(cspl_srcdir)/utils/logger/samples/input \
	$(cspl_srcdir)/utils/logger/samples/rn_defs.h \
	$(cspl_srcdir)/utils/logger/samples/nbuapidef.h \
	$(cspl_srcdir)/utils/logger/samples/rlc_apitypes.h \
	$(cspl_srcdir)/utils/logger/h/logger_flrd.h \
	$(cspl_srcdir)/utils/logger/h/common_nw_util.h \
	$(cspl_srcdir)/utils/logger/h/logger_utl.h \
	$(cspl_srcdir)/utils/logger/h/logger_print.h \
	$(cspl_srcdir)/utils/logger/h/logger_typ.h \
	$(cspl_srcdir)/utils/logger/h/logger.h \
	$(cspl_srcdir)/utils/logger/h/logger_init.h \
	$(cspl_srcdir)/utils/logger/h/logger_nw_recv.h \
	$(cspl_srcdir)/utils/logger/build/sample_log \
	$(cspl_srcdir)/utils/logger/build/sample_cfg \
	$(cspl_srcdir)/utils/logger/build/build_logger \
	$(cspl_srcdir)/utils/logger/src/logger_main.c \
	$(cspl_srcdir)/utils/logger/src/common_nw_util.c \
	$(cspl_srcdir)/utils/logger/src/logger_init.c \
	$(cspl_srcdir)/utils/logger/src/Makefile \
	$(cspl_srcdir)/utils/logger/src/logger_print.c \
	$(cspl_srcdir)/utils/logger/src/logger_flrd.c \
	$(cspl_srcdir)/utils/logger/src/logger_nw_recv.c \
	$(cspl_srcdir)/utils/logger/src/logger_utl.c \
	$(cspl_srcdir)/utils/logger/cfg/Readme \
	$(cspl_srcdir)/utils/logger/cfg/parser.awk \
	$(cspl_srcdir)/utils/logger/cfg/create_cfg \
	$(cspl_srcdir)/utils/logger/cfg/gen_api \
	\
	$(cspl_srcdir)/ui/posix-mq/Makefile \
	$(cspl_srcdir)/ui/posix-mq/main.c \
	$(cspl_srcdir)/ui/posix-mq/ui.h \
	$(cspl_srcdir)/ui/posix-mq/util.c \
	$(cspl_srcdir)/ui/posix-mq/util.h \
	$(cspl_srcdir)/ui/posix-mq/ui.c \
	$(cspl_srcdir)/ui/tcpc/Makefile \
	$(cspl_srcdir)/ui/tcpc/main.c \
	$(cspl_srcdir)/ui/tcpc/ui.h \
	$(cspl_srcdir)/ui/tcpc/util.c \
	$(cspl_srcdir)/ui/tcpc/util.h \
	$(cspl_srcdir)/ui/tcpc/ui.c \
	$(cspl_srcdir)/ui/sysv-ipc/Makefile \
	$(cspl_srcdir)/ui/sysv-ipc/main.c \
	$(cspl_srcdir)/ui/sysv-ipc/ui.h \
	$(cspl_srcdir)/ui/sysv-ipc/util.c \
	$(cspl_srcdir)/ui/sysv-ipc/util.h \
	$(cspl_srcdir)/ui/sysv-ipc/ui.c \
	$(cspl_srcdir)/ui/common/Makefile \
	$(cspl_srcdir)/ui/common/main.c \
	$(cspl_srcdir)/ui/common/ui.h \
	$(cspl_srcdir)/ui/common/util.c \
	$(cspl_srcdir)/ui/common/util.h \
	$(cspl_srcdir)/ui/tcp/Makefile \
	$(cspl_srcdir)/ui/tcp/main.c \
	$(cspl_srcdir)/ui/tcp/ui.h \
	$(cspl_srcdir)/ui/tcp/util.c \
	$(cspl_srcdir)/ui/tcp/util.h \
	$(cspl_srcdir)/ui/tcp/ui.c \
	$(cspl_srcdir)/ui/unix/Makefile \
	$(cspl_srcdir)/ui/unix/main.c \
	$(cspl_srcdir)/ui/unix/ui.h \
	$(cspl_srcdir)/ui/unix/util.c \
	$(cspl_srcdir)/ui/unix/util.h \
	$(cspl_srcdir)/ui/unix/ui.c \
	$(cspl_srcdir)/ui/udp/Makefile \
	$(cspl_srcdir)/ui/udp/main.c \
	$(cspl_srcdir)/ui/udp/ui.h \
	$(cspl_srcdir)/ui/udp/util.c \
	$(cspl_srcdir)/ui/udp/util.h \
	$(cspl_srcdir)/ui/udp/ui.c \
	\
	$(cspl_srcdir)/drivers/udp_ext/sockutil.h \
	$(cspl_srcdir)/drivers/udp_ext/shell.c \
	$(cspl_srcdir)/drivers/posix-mq/shell.c \
	$(cspl_srcdir)/drivers/vxworks/shell.c \
	$(cspl_srcdir)/drivers/simple/shell.c \
	$(cspl_srcdir)/drivers/winsock/sockutil.h \
	$(cspl_srcdir)/drivers/winsock/shell.c \
	$(cspl_srcdir)/drivers/multi/shell.c \
	$(cspl_srcdir)/drivers/sysv-ipc/shell.c \
	$(cspl_srcdir)/drivers/common/sockutil.h \
	$(cspl_srcdir)/drivers/tcp/sockutil.h \
	$(cspl_srcdir)/drivers/tcp/shell.c \
	$(cspl_srcdir)/drivers/unix/shell.c \
	$(cspl_srcdir)/drivers/udp/sockutil.h \
	$(cspl_srcdir)/drivers/udp/shell.c \
	\
	$(cspl_srcdir)/ports/win32/eflow.c \
	$(cspl_srcdir)/ports/win32/os.h \
	$(cspl_srcdir)/ports/win32/os.mk \
	$(cspl_srcdir)/ports/win32/os.c \
	$(cspl_srcdir)/ports/posix/eflow.c \
	$(cspl_srcdir)/ports/posix/os.h \
	$(cspl_srcdir)/ports/posix/os.mk \
	$(cspl_srcdir)/ports/posix/os.c \
	$(cspl_srcdir)/ports/vxworks/eflow.c \
	$(cspl_srcdir)/ports/vxworks/os.h \
	$(cspl_srcdir)/ports/vxworks/os.mk \
	$(cspl_srcdir)/ports/vxworks/os.c \
	$(cspl_srcdir)/ports/solaris/eflow.c \
	$(cspl_srcdir)/ports/solaris/os.h \
	$(cspl_srcdir)/ports/solaris/os.mk \
	$(cspl_srcdir)/ports/solaris/os.c \
	$(cspl_srcdir)/ports/simple/eflow.c \
	$(cspl_srcdir)/ports/simple/os.h \
	$(cspl_srcdir)/ports/simple/os.mk \
	$(cspl_srcdir)/ports/simple/os.c \
	$(cspl_srcdir)/ports/linux/eflow.c \
	$(cspl_srcdir)/ports/linux/os.mk \
	$(cspl_srcdir)/ports/win32-console/eflow.c \
	$(cspl_srcdir)/ports/win32-console/os.h \
	$(cspl_srcdir)/ports/win32-console/os.mk \
	$(cspl_srcdir)/ports/win32-console/os.c \
	$(cspl_srcdir)/ports/cygwin/eflow.c \
	$(cspl_srcdir)/ports/cygwin/os.h \
	$(cspl_srcdir)/ports/cygwin/os.mk \
	$(cspl_srcdir)/ports/cygwin/os.c
