
##
## Default build configuration for Windows/Cygwin
##

ifndef CSPL_FLAGS
	CSPL_FLAGS = -O3 -DNDEBUG -pedantic -Wall -W -Wimplicit -Wreturn-type -Wunused -Wmissing-prototypes -Wswitch -Wcomment -Wchar-subscripts -Wid-clash-32 -Wcast-align -Waggregate-return -Wstrict-prototypes -Wmissing-declarations -Wno-format -Wuninitialized -Wno-unused -Wpointer-arith
endif

ifndef CSPL_COMPILE
	CSPL_COMPILE = i586-cygwin32-gcc
endif

ifndef CSPL_LINKOBJ
	CSPL_LINKOBJ = i586-cygwin32-ld -r
endif

ifndef CSPL_LINKEXE
	CSPL_LINKEXE = $(CSPL_COMPILE)
endif

ifndef CSPL_AR
	CSPL_AR = i586-cygwin32-ar rv
endif

ifndef CSPL_RANLIB
	CSPL_RANLIB = i586-cygwin32-ranlib
endif

ifndef CSPL_LIBRARIES
	CSPL_LIBRARIES = 
endif

