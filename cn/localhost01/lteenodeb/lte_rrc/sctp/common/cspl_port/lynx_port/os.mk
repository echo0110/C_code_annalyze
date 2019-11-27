##
## Default cross-build configuration for LynxOS
##

ifndef CSPL_FLAGS
	CSPL_FLAGS = -mpreferred-stack-boundary=2 -pedantic -Wall -W -Wimplicit -Wreturn-type -Wunused -Wmissing-prototypes -Wswitch -Wcomment -Wchar-subscripts -Wid-clash-32 -Wcast-align -Waggregate-return -Wstrict-prototypes -Wmissing-declarations -Wno-format -Wuninitialized -Wno-unused -Wpointer-arith -D__NO_INCLUDE_WARN__
endif

ifndef CSPL_COMPILE
	CSPL_COMPILE = gcc
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
	CSPL_LIBRARIES = 
endif

