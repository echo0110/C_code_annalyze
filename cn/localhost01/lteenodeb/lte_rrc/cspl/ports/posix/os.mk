##
## Default build configuration for Posix on Unix
##

include ../../runner/lterunner.mk

ifndef CSPL_FLAGS
	CSPL_FLAGS = -g -O3 -D_REENTRANT -pedantic -std=c99  -D_XOPEN_SOURCE -Wall -W -Wimplicit -Wreturn-type -Wunused -Wmissing-prototypes -Wswitch -Wcomment -Wchar-subscripts -Wcast-align -Waggregate-return -Wstrict-prototypes -Wmissing-declarations -Wno-format -Wuninitialized -Wno-unused -Wpointer-arith
endif

ifndef CSPL_COMPILE
	CSPL_COMPILE=$(LTE_CC) 
endif

ifndef CSPL_LINKOBJ
	CSPL_LINKOBJ=$(LTE_LD)  -r
endif

ifndef CSPL_LINKEXE
	CSPL_LINKEXE=$(CSPL_COMPILE)
endif

ifndef CSPL_AR
	CSPL_AR=$(LTE_AR) rv
endif

ifndef CSPL_RANLIB
	CSPL_RANLIB=$(LTE_RANLIB) 
endif

ifndef CSPL_LIBRARIES
	CSPL_LIBRARIES = -lpthread -lposix4 -lsocket
endif



