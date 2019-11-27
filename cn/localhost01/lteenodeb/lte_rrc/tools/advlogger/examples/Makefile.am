noinst_PROGRAMS = console_log syslog_log file_log net_log multi_log

console_log_SOURCES = console_log.c
console_log_LDADD = $(top_builddir)/src/libadvlogging.la

syslog_log_SOURCES = syslog_log.c
syslog_log_LDADD = $(top_builddir)/src/libadvlogging.la

file_log_SOURCES = file_log.c
file_log_LDADD = $(top_builddir)/src/libadvlogging.la

net_log_SOURCES = net_log.c
net_log_LDADD = $(top_builddir)/src/libadvlogging.la

multi_log_SOURCES = multi_log.c
multi_log_LDADD = $(top_builddir)/src/libadvlogging.la

INCLUDES = -I$(top_srcdir)/src
