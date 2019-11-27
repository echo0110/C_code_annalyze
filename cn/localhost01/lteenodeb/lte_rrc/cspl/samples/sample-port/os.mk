##
## Sample build configuration for UNIX-ish systems using GCC
##

ifndef CSPL_FLAGS
	CSPL_FLAGS = -g -O3 -pedantic -Wall -W -Waggregate-return -Wstrict-prototypes -Wno-unused -DMODULE_LOGGING 
endif

ifndef CSPL_COMPILE
	CSPL_COMPILE = gcc
	#CSPL_COMPILE = purecov -cache-dir=. gcc

endif

ifndef CSPL_LINKOBJ
	CSPL_LINKOBJ = ld -r
endif

ifndef CSPL_LINKEXE
	CSPL_LINKEXE = $(CSPL_COMPILE)
endif

ifndef CSPL_AR
	CSPL_AR = ar rv
endif

ifndef CSPL_RANLIB
	CSPL_RANLIB = ranlib
endif

ifndef CSPL_LIBRARIES
	CSPL_LIBRARIES = -lsocket -lxnet
endif

