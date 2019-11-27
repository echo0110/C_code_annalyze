SIMROOT = $(firstword $(subst $(SIM_ROOT)/,$(SIM_ROOT) ,$(CURDIR)))
ifneq ($(findstring $(SIM_ROOT),$(SIMROOT)),$(SIM_ROOT))
  SIMROOT = $(SIM_ROOT)
endif
export SIMROOT

# IMPORTANT: keep the following two in sync !!!
MAKE_ROOT := $(SIMROOT)
SIM_REL_MAKE_ROOT := .

include $(MAKE_ROOT)/Makefile.inc
