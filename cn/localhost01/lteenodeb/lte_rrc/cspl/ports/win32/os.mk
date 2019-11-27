##
## Default build configuration for Windows/Cygwin
##

ifndef CSPL_FLAGS
	CSPL_FLAGS = -c -Wall -Wimplicit -Wmissing-prototypes -Wshadow -Wcomment -Waggregate-return -Wmissing-declarations -Wreturn-type -Wparentheses -Wpointer-arith -fno-builtin -fno-inline-functions -fsigned-char -pipe -Wstrict-prototypes -Wno-format -W -fcond-mismatch -Wnested-externs -Wswitch -Wno-unused
endif

ifndef CSPL_COMPILE
	CSPL_COMPILE = i586-cygwin32-gcc -mno-cygwin -mwindows
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
	CSPL_LIBRARIES = -lwsock32
endif

