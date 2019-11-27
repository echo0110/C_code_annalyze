PDCPROOT = $(firstword $(subst $(LTE_PDCP_ROOT)/,$(LTE_PDCP_ROOT) ,$(CURDIR)))
ifneq ($(findstring $(LTE_PDCP_ROOT),$(PDCPROOT)),$(LTE_PDCP_ROOT))
  PDCPROOT = $(LTE_PDCP_ROOT)
endif
export PDCPROOT

# IMPORTANT: keep the following two in sync !!!
MAKE_ROOT := $(PDCPROOT)
PDCPROOT_REL_MAKE_ROOT := .

include $(MAKE_ROOT)/Makefile.inc
