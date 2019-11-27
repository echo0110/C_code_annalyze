MODE_BUILD := udp
SCTP_LIB :=

ifeq '$(SCTP_STACK)' 'aricent_sctp_ipr'
	MODE_BUILD = sctp
#### adding SCTP LIB ####
	SCTP_LIB = cspl cpset mpset porting
#### adding SCTP LIB ####
endif
ifeq '$(SCTP_STACK)' 'lksctp'
	MODE_BUILD = sctp
#### adding SCTP LIB ####
#	SCTP_LIBS = libsctp.so.1
#### adding SCTP LIB ####
endif

TARGETS := protocolSimulator
protocolSimulator.TYPE := executable
protocolSimulator.SRCS := protocolSimulatorMain.c\
                         call_func.c\
                         callbacks.c\
                         operations.c\
                         callbacksVars.c\
                         callbacksSockets.c\
                         simuScheduler.c\
                         simLogger.c\
                         socket_udp_tools.c\
                         simu_table_func.c\
                         userUtils.c\
                         testStatistic.c\
                         sock_tools.c
##                       socket_cmd_tools.c

ifeq '$(SCTP_STACK)' 'aricent_sctp_ipr' 
protocolSimulator.SRCS += socket_sctp_tools.c
endif
ifeq '${SCTP_STACK}' 'lksctp'
protocolSimulator.SRCS += socket_sctp_tools.c
endif
protocolSimulator.XLIBS := pthread  $(SCTP_LIB)
protocolSimulator.LIBS :=
