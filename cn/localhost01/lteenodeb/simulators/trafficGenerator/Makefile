#/****************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (c) Aricent.
# *
# ****************************************************************************
# *
# *  $Id: Makefile,v 1.6.16.1 2010/11/01 15:34:21 gur20056 Exp $
# *
# ****************************************************************************
# *
# *  File Description : Makefile
# *
# ****************************************************************************
# *
# * Revision Details
# * ----------------
# *
# * $Log: Makefile,v $
# * Revision 1.6.16.1  2010/11/01 15:34:21  gur20056
# * data pumping for different UE indexes
# *
# * Revision 1.6  2010/03/15 10:42:07  gur19479
# * updated for UL Traffic gen
# *
# * Revision 1.5  2010/02/22 11:47:31  gur19479
# * updated to compile dltraffic gen
# *
# * Revision 1.4  2010/02/18 12:09:38  gur19479
# * updated for ul and dl traffic gen
# *
# * Revision 1.3  2010/02/18 11:15:11  gur20619
# * updated to remove executables
# *
# * Revision 1.2  2010/02/18 09:03:22  gur19479
# * updated for ul and dl tg
# *
# * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
# * eNB framework for intel
# *
# * Revision 1.1.2.2  2009/08/27 11:24:38  gur19140
# * *** empty log message ***
# *
# * Revision 1.1.2.1  2009/07/12 09:46:21  gur19479
# * updated for traffic gen
# *
# *
# ****************************************************************************/
##############################################################################
#
#       MAIN MAKE FILE
#   Make file to be used for building Traffic Generator.
#
##############################################################################

include ${LTE_ROOT}/make.inc
TRAFFIC_GENERATOR_ROOT=$(LTE_ROOT)/simulators/trafficGenerator

TRAFFIC_GENERATOR_BIN_DIR=$(TRAFFIC_GENERATOR_ROOT)/bin
TRAFFIC_GENERATOR_SRC_DIR=$(TRAFFIC_GENERATOR_ROOT)/src
TRAFFIC_GENERATOR_LIB_DIR=$(TRAFFIC_GENERATOR_ROOT)/lib

TARGET = $(TRAFFIC_GENERATOR_BIN_DIR)/ulTrafficGen
DL_TARGET = $(TRAFFIC_GENERATOR_BIN_DIR)/dlTrafficGen
DL_TARGET_REAL = $(TRAFFIC_GENERATOR_BIN_DIR)/dlTgGen
LGW_DL_TARGET = $(TRAFFIC_GENERATOR_BIN_DIR)/lgwDlTrafficGen
UL_TARGET = $(TRAFFIC_GENERATOR_BIN_DIR)/ulTrafficGen

TRAFFIC_GENERATOR_INC_DIR= -I${TRAFFIC_GENERATOR_ROOT}/include/
#SPR 20526 Changes Start#
INC_COM = -I$(LTE_ROOT)/common/include
INC_COM += -I$(LTE_ROOT)/common/include/lte_port/
#SPR 20526 Changes End#

CFLAGS += -DSIM_MODE_PDCP $(CTAGS)
LIBS_DIR+= -L$(COMMON_LIB_UESIM_DIR) -lframework -lpthread 
LIBS_DIR_REAL+= -L$(COMMON_LIB_UESIM_DIR) -lpthread -lm -lrt
LFLAGS += $(LIBS_DIR) -lm -lrt
INCLUDE_LTE += $(TRAFFIC_GENERATOR_INC_DIR) $(INC_COM) 
INCLUDE_LTE_REAL += $(TRAFFIC_GENERATOR_INC_DIR) 

TRAFFIC_GENERATOR_OBJS = $(TRAFFIC_GENERATOR_BIN_DIR)/lteTrafficGenEntities.o\
					     $(TRAFFIC_GENERATOR_BIN_DIR)/lteTrafficGenExtInterface.o\
					     $(TRAFFIC_GENERATOR_BIN_DIR)/lteTrafficGenRrcInterface.o\
					     $(TRAFFIC_GENERATOR_BIN_DIR)/lteTrafficGenMain.o

OBJ_LOC= $(TRAFFIC_GENERATOR_OBJS) 

trafficGenExe: create_dir make_genExe

make_genExe: $(OBJ_LOC)
	$(MAKE_BIN) $(OBJ_LOC) -o $(TARGET) $(LFLAGS) $(CTAGS) 

#Enable flag UE_IPV6_ADDR to assign IPV6 header in UL/DL traffic buffer
#SPR 20526 Changes Start#
ulTrafficGenExe: create_dir
	$(LTE_CC) -DMULTI_UE_ENHANCEMENT_FRWK -DROHC_UT_TEMP_FLAG -DTG_MULTI_ATTACH_DETACH -DMULTI_UE_ENHANCEMENT -UROHC_PROFILE_1  -UCAVIUM_BOARD  -UUE_IPV6_ADDR -ULIPA_GW_SEQ_CHK -UROHC_PROFILE_6 -ULTE_EMBMS_SUPPORTED -ULTE_EMBMS_UESIM_TESTING $(INCLUDE_LTE) $(TRAFFIC_GENERATOR_SRC_DIR)/ulTrafficGen.c -o $(UL_TARGET) $(LFLAGS)

dlTrafficGenExe: create_dir
	$(LTE_CC) -DENDC_ENABLED -USUPPORT_64_UE -DHEADER_ALIGNMENT  -UAUTOMATION_DLTG -UNO_GTPU_STACK -DMULTI_UE_ENHANCEMENT_FRWK -DROHC_UT_TEMP_FLAG -DTG_MULTI_ATTACH_DETACH -DMULTI_UE_ENHANCEMENT -UROHC_PROFILE_1 -UCAVIUM_BOARD -UIPV6_ENABLE -UUE_IPV6_ADDR -ULIPA_GW_SEQ_CHK -UROHC_PROFILE_6 -ULTE_EMBMS_SUPPORTED -ULTE_EMBMS_UESIM_TESTING $(INCLUDE_LTE) $(TRAFFIC_GENERATOR_SRC_DIR)/gtpSimulator.c -o $(DL_TARGET) $(LFLAGS)

dlTgGenExe: create_dir
	$(LTE_CC) -g -USUPPORT_64_UE -DTG_REAL_EPC  -UAUTOMATION_DLTG -UNO_GTPU_STACK -DMULTI_UE_ENHANCEMENT_FRWK -DROHC_UT_TEMP_FLAG -DTG_MULTI_ATTACH_DETACH -DMULTI_UE_ENHANCEMENT -UROHC_PROFILE_1 -UCAVIUM_BOARD -UIPV6_ENABLE -UUE_IPV6_ADDR -ULIPA_GW_SEQ_CHK -UROHC_PROFILE_6 -ULTE_EMBMS_SUPPORTED -ULTE_EMBMS_UESIM_TESTING $(INCLUDE_LTE) $(TRAFFIC_GENERATOR_SRC_DIR)/gtpSimulator.c -o $(DL_TARGET_REAL) $(LIBS_DIR_REAL)


lgwDlTrafficGenExe: create_dir
	$(LTE_CC) -DLIPA_GW -USUPPORT_64_UE -DHEADER_ALIGNMENT  -UAUTOMATION_DLTG -UNO_GTPU_STACK -DMULTI_UE_ENHANCEMENT_FRWK -DROHC_UT_TEMP_FLAG -DTG_MULTI_ATTACH_DETACH -DMULTI_UE_ENHANCEMENT -UROHC_PROFILE_1 -UCAVIUM_BOARD -UIPV6_ENABLE -UUE_IPV6_ADDR -UROHC_PROFILE_6 -ULTE_EMBMS_SUPPORTED -ULTE_EMBMS_UESIM_TESTING $(INCLUDE_LTE) $(TRAFFIC_GENERATOR_SRC_DIR)/gtpSimulator.c -o $(LGW_DL_TARGET) $(LFLAGS)

#SPR 20526 Changes End#
create_dir:
	${MKD} $(TRAFFIC_GENERATOR_BIN_DIR)

$(TRAFFIC_GENERATOR_BIN_DIR)/%.o: $(TRAFFIC_GENERATOR_SRC_DIR)/%.c
	$(COMPILE) $(CFLAGS) $(CTAGS) $(INCLUDE_LTE) -c $< -o $@ 

clean:
	rm -f $(TRAFFIC_GENERATOR_BIN_DIR)/*.o
	${RMD} $(TRAFFIC_GENERATOR_BIN_DIR)

$(warning LTE_ROOT = $(LTE_ROOT))
$(warning TRAFFIC_GENERATOR_LIB_DIR = $(TRAFFIC_GENERATOR_LIB_DIR))
$(warning COMPILE = $(COMPILE))
$(warning LFLAGS = $(LFLAGS))
