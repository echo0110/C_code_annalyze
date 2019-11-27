include ../make.inc
include make.in
SHELL=/bin/bash
MAKE = make
CC = $(LTE_CC)
LTE_ROOT = $(PWD)/..
TOP_LVL_DIR = $(LTE_ROOT)/lte_oam
CUNIT_INCL = /root/local/include
CUNIT_LIBS = /root/local/lib
ADVLOG_SRC = $(LTE_ROOT)/lte_rrc/tools/advlogger/src/advlogging.c

ifeq (1,$(OAM_UT_FRAMEWORK))
CFLAGS += -UOAM_DEBUG -DOAM_IPR -UOAM_UT_FRAMEWORK -UOAM_CLEANUP_FLAG
else
	# SPR 20430 Changes Start#
CFLAGS += -UOAM_DEBUG -DOAM_IPR -UOAM_UT_FRAMEWORK -UOAM_CLEANUP_FLAG -DOAM_SHARED_MEM_LOGGER
# SPR 20430 Changes End#
endif
#SPR_18897 Fix Start 
ifeq ($(HOST_COMPILATION),0)
SUBDIRS = import common core/config_mgmt core/perf_mgmt core/startup_n_intf_mgmt core/control_mgmt interface port qcli_intf_hdlr core/fault_mgmt core/health_monitoring soft_dwnld_mgr qcli oam_agnt
else
SUBDIRS = import common core/config_mgmt core/perf_mgmt core/startup_n_intf_mgmt core/control_mgmt interface port qcli_intf_hdlr core/fault_mgmt core/health_monitoring soft_dwnld_mgr qcli snmp oam_agnt
endif

#SPR_18897 Fix End 
SUBDIRS += $(OAM_APPS)

ifeq (1,$(OAM_UT_FRAMEWORK))
SUBDIRS += aut
endif
ifeq (1,$(RRH_SUPPORT))
SUBDIRS +=  rrh_intf
CFLAGS += -DOAM_RADIO_HEAD_SUPPORT -DOAM_RRH_TYPE_1_SUPPORT 
IDR_RRH = $(TOP_LVL_DIR)/rrh_intf/include
endif

ifeq (1,$(OAM_SON_ENABLE))
CFLAGS+= -DOAM_SON_ENABLE -DSON_COMPILE_X2 
endif
oft_dwnld_mgr="soft_dwnld_mgr"

IDR = $(TOP_LVL_DIR)/common/include \
      -I$(LTE_ROOT)/../common/include/ \
      -I$(TOP_LVL_DIR)/import/include/ \
      -I$(LTE_ROOT)/lte_rrc/rrc/common/include \
      -I$(TOP_LVL_DIR)/core/fault_mgmt/include \
      -I$(TOP_LVL_DIR)/core/config_mgmt/include \
      -I$(TOP_LVL_DIR)/core/perf_mgmt/include \
      -I$(TOP_LVL_DIR)/core/startup_n_intf_mgmt/include \
     -I$(TOP_LVL_DIR)/core/control_mgmt/include \
     -I$(TOP_LVL_DIR)/core/health_monitoring/include \
      -I$(TOP_LVL_DIR)/interface/include \
      -I$(TOP_LVL_DIR)/port/include \
      -I$(TOP_LVL_DIR)/qcli_intf_hdlr/include \
      -I$(TOP_LVL_DIR)/third_party/libxml2/include/ \
      -I$(TOP_LVL_DIR)/third_party/curl \
      -I$(TOP_LVL_DIR)/third_party/curl/lib \
      -I$(TOP_LVL_DIR)/third_party/curl/include \
      -I$(TOP_LVL_DIR)/third_party/netsnmp/include \
      -I$(LTE_ROOT)/common/include \
      -I$(LTE_ROOT)/common/in_memory_logging/include/ \
      -I$(LTE_ROOT)/common/include/lte_port \
      -I$(LTE_ROOT)/lte_rrm/common/include \
      -I$(LTE_ROOT)/lte_rrm/interface/include \
      -I$(LTE_ROOT)/lte_son/common/include \
      -I$(LTE_ROOT)/lte_son/interface/include \
      -I$(LTE_ROOT)/lte_rrc/cspl/h \
      -I$(LTE_ROOT)/lte_rrc/s1ap/include \
      -I$(LTE_ROOT)/lte_rrc/x2ap/common/inc \
      -I$(LTE_ROOT)/lte_rrc/cspl/build \
      -I$(LTE_ROOT)/lte_rrc/cspl/ports/linux \
      -I$(LTE_ROOT)/lte_rrc/rrc/common/include/multi \
      -I$(LTE_ROOT)/lte_rrc/x2ap/init/inc \
      -I$(LTE_ROOT)/lte_rrc/rrc/common/oamim/include \
      -I$(LTE_ROOT)/lte_rrc/rrc/common/rrmim/include \
      -I$(LTE_ROOT)/lte_rrc/tools/advlogger/src/ \
      -I$(LTE_ROOT)/lte_rrc/s1ap/include/ \
      -I$(LTE_ROOT)/lte_rrc/m2ap/inc/ \
      -I$(LTE_ROOT)/lte_rrc/m2ap/interface/oam/inc/ \
      -I$(LTE_ROOT)/lte_rrc/m2ap/interface/rrm/inc/ \
      -I$(LTE_ROOT)/lte_rrc/m2ap/utils/inc \
      -I$(LTE_ROOT)/lte_rrc/m2ap/interface/lower_layers/inc \
      -I$(LTE_ROOT)/lte_rrc/rrc/common/s1ap_oamim/include \
      -I$(LTE_ROOT)/common/loggingFrameWork/common/inc \
      -I$(LTE_ROOT)/common/loggingFrameWork/alarm/inc \
      -I$(LTE_ROOT)/common/loggingFrameWork/logging/inc/ \
      -I$(LTE_ROOT)/commonInterface/interfaces/include \
      -I$(LTE_ROOT)/cspl/ports/linux \
      -I$(LTE_ROOT)/lte_oam/qcli/include \
      -I$(LTE_ROOT)/lte_oam/soft_dwnld_mgr/include \
      -I$(LTE_ROOT)/lte_oam/soft_dwnld_mgr/include \
      -I$(TOP_LVL_DIR)/snmp/include \
      -I$(TOP_LVL_DIR)/aut/include 

IDR += -I$(LTE_ROOT)/lte_oam/oam_agnt/include

IDR += $(OAM_EXTERNAL_INCLUDES)

ifeq (1,$(OAM_UT_FRAMEWORK))
INCL = -I$(IDR) -I$(CUNIT_INCL)
else
INCL = -I$(IDR)
endif

LDR = $(TOP_LVL_DIR)/library

ifeq (1,$(OAM_UT_FRAMEWORK))
LIB = -L$(CSPL_BUILD) -L$(LTE_ROOT)/common/lib -L$(CUNIT_LIBS) -L$(LDR)
else
LIB = -L$(CSPL_BUILD) -L$(LTE_ROOT)/common/lib -L$(LDR)
endif

# if using arm64 compiler, using static library for link
ifeq ($(LTE_CC), aarch64-linux-gnu-gcc)
LIBS_OAM = -lcspl -l:libxml2.a -lm -lpthread -linMemLgr -l:libcurl.a -lrt -lz
else
LIBS_OAM = -lcspl -lxml2 -lm -lpthread -linMemLgr -lcurl -lrt
endif

ifeq (1,$(OAM_UT_FRAMEWORK))
LIBS_OAM += -lcunit
endif

ifeq ($(LTE_CC), aarch64-linux-gnu-gcc)
LIBS_CLI = -lcspl -lqcli -l:libncurses.a -lm -lpthread -linMemLgr -lrt
else
LIBS_CLI = -lcspl -lqcli -lncurses -lm -lpthread -linMemLgr -lrt
endif
LIBS_SDM = -lcspl -lm -lpthread
LIBS_AGNT = -lcspl 

# SPR_18897 Fix Start 
#LIBS_SNMP= -lcspl -ladvlogger -lpthread -lnetsnmpmibs -lnetsnmpagent -lnetsnmp -lrt
ifeq ($(LTE_CC), aarch64-linux-gnu-gcc)
LIBS_SNMP= -lcspl -lpthread -l:libnetsnmpmibs.a -l:libnetsnmpagent.a -l:libnetsnmp.a -lrt
else
LIBS_SNMP= -lcspl -lpthread -lnetsnmpmibs -lnetsnmpagent -lnetsnmp -lrt
endif
# SPR_18897 Fix End 
ifeq ($(PLATFORM),intel)
# SPR 20430 Changes Start#
#LIBS_OAM += -m32
#LIBS_CLI += -m32
#LIBS_SDM += -m32
#spr 20430 Changes End#
endif

ifeq (1,$(OAM_UT_FRAMEWORK))
UT_SRC = ./suites/ 
endif


SRC = ./src/

DST = ../build/obj/
BIN = ../../bin/

DST1= $(TOP_LVL_DIR)/build/obj/

ifeq ($(LTE_CC), aarch64-linux-gnu-gcc)
CFLAGS += -L/home/zhanghh/sysroot/lib
LIB += -L/home/zhanghh/sysroot/lib
endif

ifeq ($(HOST_COMPILATION),0)
ifeq ($(LTE_CC), aarch64-linux-gnu-gcc)
MYMAKEFLAGS = 'CFLAGS=$(CFLAGS)' 'INCL=$(INCL)' 'LIB=$(LIB)' 'LIBS_OAM=$(LIBS_OAM)' 'LIBS_CLI=$(LIBS_CLI)' 'LIBS_SDM=$(LIBS_SDM)' 'SRC=$(SRC)' 'DST=$(DST)' '-L/home/zhanghh/sysroot/lib'
else
MYMAKEFLAGS = 'CFLAGS=$(CFLAGS)' 'INCL=$(INCL)' 'LIB=$(LIB)' 'LIBS_OAM=$(LIBS_OAM)' 'LIBS_CLI=$(LIBS_CLI)' 'LIBS_SDM=$(LIBS_SDM)' 'SRC=$(SRC)' 'DST=$(DST)'
endif
else
MYMAKEFLAGS = 'CFLAGS=$(CFLAGS)' 'INCL=$(INCL)' 'SRC=$(SRC)' 'DST=$(DST)'
endif
ifeq (1,$(OAM_UT_FRAMEWORK))
UT_MAKEFLAGS = 'CFLAGS=$(CFLAGS)' 'INCL=$(INCL)' 'SRC=$(UT_SRC)' 'DST=$(DST)'
endif

help:
	@echo ""
	@echo "Makefile for LTE eNodeB OAM"
	@echo "=================================="
	@echo "Usage:     make [target]"
	@echo ""
	@echo "Avialable Targets "
	@echo "----------------"
	@echo "all"
	@echo "common"
	@echo "config-mgmt"
	@echo "fault-mgmt"
	@echo "perf-mgmt"
	@echo "startup-n-intf-mgmt"
	@echo "Control-mgmt"
	@echo "soft_dwnld_mgr"
	@echo "health-monitoring"
	@echo "hw-sw-mgmt"
	@echo "interface"
	@echo "qcli_intf_hdlr"
	@echo "port"
	@echo "tp-libxml2"
	@echo "tp-curl"
	@echo "tp-net-snmp"
	@echo "qcli"
	@echo "rrh"
	@echo "import"
	@echo "snmp"
	@echo "clean-common"
	@echo "clean-config-mgmt"
	@echo "clean-control-mgmt"
	@echo "clean-soft_dwnld_mgr"
	@echo "clean-health-monitoring"
	@echo "clean-fault-mgmt"
	@echo "clean-perf-mgmt"
	@echo "clean-start-n-intf-mgmt"
	@echo "clean-hw-sw-mgmt"
	@echo "clean-interface"
	@echo "clean-qcli_intf_hdlr"
	@echo "clean-port"
	@echo "clean-tp-libxml2"
	@echo "clean-tp-curl"
	@echo "clean-tp-net-snmp"
	@echo "clean-qcli"
	@echo "clean-all"
	@echo "clean-rrh"
	@echo "clean-import"
	@echo "clean-snmp"
	@echo ""

obj =  $(DST1)oam_populate_l3_timers.o $(DST1)oam_populate_rrm_geran_cdma200_params.o $(DST1)oam_xml_function.o $(DST1)oam_populate_func.o $(DST1)oam_utils.o $(DST1)oam_curl_utils.o $(DST1)oam_l2_if.o $(DST1)oam_config_mgmt_fsm.o $(DST1)oam_config_mgmt_if.o $(DST1)oam_config_tcb.o $(DST1)oam_perf_mgmt_utils.o $(DST1)oam_sim.o $(DST1)oam_sim_proc_msg.o $(DST1)oam_tr69_interface_handler.o $(DST1)oam_sim_utils.o $(DST1)oam_port_main.o $(DST1)oam_port_utils.o $(DST1)oam_control_mgmt.o $(DST1)oam_fault_mgmt_fsm.o $(DST1)oam_fault_mgmt_utils.o $(DST1)oam_hm.o $(DST1)oam_rrm_composer.o $(DST1)oam_rrm_parser.o  $(DST1)oam_rrc_il_composer.o $(DST1)oam_rrc_il_parser.o $(DST1)oam_s1ap_il_parser.o $(DST1)oam_s1ap_il_composer.o $(DST1)oam_x2ap_il_parser.o $(DST1)oam_x2ap_il_composer.o $(DST1)oam_s1apCommon_il_composer.o $(DST1)oam_s1apCommon_il_parser.o $(DST1)oam_x2apCommon_il_composer.o $(DST1)oam_x2apCommon_il_parser.o $(DST1)advlogging.o $(DST1)oam_agnt_il_composer.o $(DST1)oam_agnt_il_parser.o  $(DST1)lteMisc.o

# EMBMS_CHANGES_START
ifneq '$(findstring -DLTE_EMBMS_SUPPORTED,$(CFLAGS))' ''
obj += $(DST1)oam_sim_proc_msg_embms.o $(DST1)oam_populate_embms_params.o $(DST1)oam_m2ap_il_composer.o $(DST1)oam_m2ap_il_parser.o
endif
# EMBMS_CHANGES_END
ifeq ($(NBIOT), 0)
#obj =  $(DST1)oam_populate_l3_timers.o $(DST1)oam_populate_rrm_geran_cdma200_params.o $(DST1)oam_xml_function.o $(DST1)oam_populate_func.o $(DST1)oam_utils.o $(DST1)oam_curl_utils.o $(DST1)oam_l2_if.o $(DST1)oam_config_mgmt_fsm.o $(DST1)oam_config_mgmt_if.o $(DST1)oam_config_tcb.o $(DST1)oam_perf_mgmt_utils.o $(DST1)oam_sim.o $(DST1)oam_sim_proc_msg.o $(DST1)oam_tr69_interface_handler.o $(DST1)oam_sim_utils.o $(DST1)oam_port_main.o $(DST1)oam_port_utils.o $(DST1)oam_control_mgmt.o $(DST1)oam_fault_mgmt_fsm.o $(DST1)oam_fault_mgmt_utils.o $(DST1)oam_hm.o $(DST1)oam_rrm_composer.o $(DST1)oam_rrm_parser.o $(DST1)oam_rrc_il_composer.o $(DST1)oam_rrc_il_parser.o $(DST1)oam_s1ap_il_parser.o $(DST1)oam_s1ap_il_composer.o $(DST1)oam_x2ap_il_parser.o $(DST1)oam_x2ap_il_composer.o $(DST1)oam_s1apCommon_il_composer.o $(DST1)oam_s1apCommon_il_parser.o $(DST1)oam_x2apCommon_il_composer.o $(DST1)oam_x2apCommon_il_parser.o $(DST1)advlogging.o 
else
obj += $(DST1)oam_rrmNb_parser.o $(DST1)oam_rrmNb_composer.o  
endif

obj1 = $(DST1)oam_qcli_main.o $(DST1)oam_qcli_port.o $(DST1)oam_qcli_utils.o $(DST1)oam_qcli_cmd_tree.o $(DST1)oam_utils.o $(DST1)oam_port_utils.o $(DST1)advlogging.o $(DST1)lteMisc.o

obj2 = $(DST1)oam_sdm_main.o $(DST1)oam_sdm_utils.o $(DST1)oam_sdm_port.o

obj3 = $(DST1)oam_snmp_port_main.o $(DST1)oam_snmp_main.o $(DST1)oam_enb_mib_traps.o $(DST1)oam_enb_mib_scalars.o $(DST1)oam_snmp_agt.o $(DST1)oam_snmp_interface.o $(DST1)oam_enb_mib_tables.o

ifeq (1,$(OAM_UT_FRAMEWORK))
obj4 =  $(DST1)oam_populate_l3_timers.o $(DST1)oam_populate_rrm_geran_cdma200_params.o $(DST1)oam_xml_function.o $(DST1)oam_populate_func.o $(DST1)oam_utils.o $(DST1)oam_curl_utils.o $(DST1)oam_l2_if.o $(DST1)oam_config_mgmt_fsm.o $(DST1)oam_config_mgmt_if.o $(DST1)oam_config_tcb.o $(DST1)oam_perf_mgmt_utils.o $(DST1)oam_sim.o $(DST1)oam_sim_proc_msg.o $(DST1)oam_tr69_interface_handler.o $(DST1)oam_sim_utils.o $(DST1)oam_port_main.o $(DST1)oam_port_utils.o $(DST1)oam_control_mgmt.o $(DST1)oam_fault_mgmt_fsm.o $(DST1)oam_fault_mgmt_utils.o $(DST1)oam_hm.o $(DST1)oam_rrm_composer.o $(DST1)oam_rrm_parser.o  $(DST1)oam_rrc_il_composer.o $(DST1)oam_rrc_il_parser.o $(DST1)oam_s1ap_il_parser.o $(DST1)oam_s1ap_il_composer.o $(DST1)oam_x2ap_il_parser.o $(DST1)oam_x2ap_il_composer.o $(DST1)oam_s1apCommon_il_composer.o $(DST1)oam_s1apCommon_il_parser.o $(DST1)oam_x2apCommon_il_composer.o $(DST1)oam_x2apCommon_il_parser.o $(DST1)oam_aut_main.o $(DST1)oam_aut_rrm.o $(DST1)oam_aut_fm.o $(DST1)oam_aut_son.o $(DST1)oam_aut_l2.o $(DST1)oam_aut_l3.o $(DST1)oam_aut_cli.o $(DST1)oam_aut_pm.o $(DST1)oam_aut_common.o $(DST1)advlogging.o  $(DST1)lteMisc.o
endif
# EMBMS_CHANGES_START
ifneq '$(findstring -DLTE_EMBMS_SUPPORTED,$(CFLAGS))' ''
obj4 += $(DST1)oam_sim_proc_msg_embms.o $(DST1)oam_populate_embms_params.o $(DST1)oam_m2ap_il_composer.o $(DST1)oam_m2ap_il_parser.o
endif

ifeq ($(NBIOT), 0)
#obj4 =  $(DST1)oam_populate_l3_timers.o $(DST1)oam_populate_rrm_geran_cdma200_params.o $(DST1)oam_xml_function.o $(DST1)oam_populate_func.o $(DST1)oam_utils.o $(DST1)oam_curl_utils.o $(DST1)oam_l2_if.o $(DST1)oam_config_mgmt_fsm.o $(DST1)oam_config_mgmt_if.o $(DST1)oam_config_tcb.o $(DST1)oam_perf_mgmt_utils.o $(DST1)oam_sim.o $(DST1)oam_sim_proc_msg.o $(DST1)oam_tr69_interface_handler.o $(DST1)oam_sim_utils.o $(DST1)oam_port_main.o $(DST1)oam_port_utils.o $(DST1)oam_control_mgmt.o $(DST1)oam_fault_mgmt_fsm.o $(DST1)oam_fault_mgmt_utils.o $(DST1)oam_hm.o $(DST1)oam_rrm_composer.o $(DST1)oam_rrm_parser.o $(DST1)oam_rrc_il_composer.o $(DST1)oam_rrc_il_parser.o $(DST1)oam_s1ap_il_parser.o $(DST1)oam_s1ap_il_composer.o $(DST1)oam_x2ap_il_parser.o $(DST1)oam_x2ap_il_composer.o $(DST1)oam_s1apCommon_il_composer.o $(DST1)oam_s1apCommon_il_parser.o $(DST1)oam_x2apCommon_il_composer.o $(DST1)oam_x2apCommon_il_parser.o $(DST1)oam_aut_main.o $(DST1)oam_aut_rrm.o $(DST1)oam_aut_fm.o $(DST1)oam_aut_son.o $(DST1)oam_aut_l2.o $(DST1)oam_aut_l3.o $(DST1)oam_aut_cli.o $(DST1)oam_aut_pm.o $(DST1)oam_aut_common.o $(DST1)advlogging.o
else
#obj4 =  $(DST1)oam_populate_l3_timers.o $(DST1)oam_populate_rrm_geran_cdma200_params.o $(DST1)oam_xml_function.o $(DST1)oam_populate_func.o $(DST1)oam_utils.o $(DST1)oam_curl_utils.o $(DST1)oam_l2_if.o $(DST1)oam_config_mgmt_fsm.o $(DST1)oam_config_mgmt_if.o $(DST1)oam_config_tcb.o $(DST1)oam_perf_mgmt_utils.o $(DST1)oam_sim.o $(DST1)oam_sim_proc_msg.o $(DST1)oam_tr69_interface_handler.o $(DST1)oam_sim_utils.o $(DST1)oam_port_main.o $(DST1)oam_port_utils.o $(DST1)oam_control_mgmt.o $(DST1)oam_fault_mgmt_fsm.o $(DST1)oam_fault_mgmt_utils.o $(DST1)oam_hm.o $(DST1)oam_rrm_composer.o $(DST1)oam_rrmNb_composer.o $(DST1)oam_rrm_parser.o  $(DST1)oam_rrmNb_parser.o $(DST1)oam_rrc_il_composer.o $(DST1)oam_rrc_il_parser.o $(DST1)oam_s1ap_il_parser.o $(DST1)oam_s1ap_il_composer.o $(DST1)oam_x2ap_il_parser.o $(DST1)oam_x2ap_il_composer.o $(DST1)oam_s1apCommon_il_composer.o $(DST1)oam_s1apCommon_il_parser.o $(DST1)oam_x2apCommon_il_composer.o $(DST1)oam_x2apCommon_il_parser.o $(DST1)oam_aut_main.o $(DST1)oam_aut_rrm.o $(DST1)oam_aut_fm.o $(DST1)oam_aut_son.o $(DST1)oam_aut_l2.o $(DST1)oam_aut_l3.o $(DST1)oam_aut_cli.o $(DST1)oam_aut_pm.o $(DST1)oam_aut_common.o $(DST1)advlogging.oi
obj4 += $(DST1)oam_rrmNb_parser.o $(DST1)oam_rrmNb_composer.o
endif

obj5 = $(DST1)oam_agnt_main.o $(DST1)oam_agnt_msg_handler.o $(DST1)oam_agnt_port.o $(DST1)oam_agnt_utils.o $(DST1)oam_agnt_parser.o $(DST1)oam_agnt_composer.o $(DST1)advlogging.o 


ifeq (1,$(RRH_SUPPORT))
obj +=  $(DST1)oam_rcm_if.o $(DST1)oam_rcm_core.o 
obj1 += $(DST1)oam_qcli_cmd_tree_rrh.o
else
ifneq '$(findstring -DLTE_EMBMS_SUPPORTED,$(CFLAGS))' ''
obj1 += $(DST1)oam_qcli_cmd_tree_embms.o
endif
endif
ifeq (1,$(OAM_SON_ENABLE))
obj += $(DST1)oam_son_composer.o $(DST1)oam_son_parser.o
obj4 += $(DST1)oam_son_composer.o $(DST1)oam_son_parser.o
endif


ifeq ($(LINUX_PC_TEST),true)
LIB +=  -L$(TEST_LIB_DIR) -lTest
endif
ifeq ($(LTE_CC), arm-linux-gnueabi-gcc)
LIB += -L/home/zhanghh/arm_tools/sysroot/lib
else
LIB += -L/home/zhanghh/sysroot/lib
endif

ifeq (1,$(OAM_UT_FRAMEWORK))
all : advlogger lteMisc
	@for i in $(SUBDIRS); do \
        echo "make all in $$i..."; \
        (cd $$i;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC)); \
        done
	echo "UT Build Called"; $(CC) -o $(BIN)lte_oam_ut $(obj4) $(LIB) $(LIBS_OAM) $(LIBS_CLI);
	echo "QCLI Build Called"; $(CC) -o $(BIN)lte_oamCli $(obj1) $(LIB) $(LIBS_CLI);
	echo "Software Download Manager called"; $(CC) -o $(BIN)lte_oamSDM $(obj2) $(LIB) $(LIBS_SDM);
	echo "OAM Agent Build called"; $(CC) -o $(BIN)lte_oamAgnt $(obj5) $(LIB) $(LIBS_AGNT);
				lte_oam/Makefile

# SPR 19713 change start
# change for 19288 removed
# SPR_18897 Fix Start
#	echo "SNMP Agent called"; $(CC) -o $(BIN)lte_oamSnmpAgt $(obj3) $(LIB) $(OAM_APPS_LIB);
# SPR_18897 Fix End
# SPR 19713 change end
 
	echo "SNMP Agent called"; $(CC) -o $(BIN)lte_oamSnmpAgt $(obj3) $(LIB) $(OAM_APPS_LIB);
else
all : advlogger lteMisc
	@for i in $(SUBDIRS); do \
        echo "make all in $$i..."; \
        (cd $$i;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC)); \
        done
	echo "Build Called"; $(CC) -o $(BIN)lte_oam $(obj) $(LIB) $(LIBS_OAM);
	echo "QCLI Build Called"; $(CC) $(INCL) -o $(BIN)lte_oamCli $(obj1) $(LIB) $(LIBS_CLI);
	echo "Software Download Manager called"; $(CC) -o $(BIN)lte_oamSDM $(obj2) $(LIB) $(LIBS_SDM);
	echo "OAM Agent Build called"; $(CC) -o $(BIN)lte_oamAgnt $(obj5) $(LIB) $(LIBS_AGNT);

# SPR 19713 change start
# change for 19288 removed
# SPR_18897 Fix Start
#	echo "SNMP Agent called"; $(CC) -o $(BIN)lte_oamSnmpAgt $(obj3) $(LIB) $(LIBS_SNMP);
# SPR_18897 Fix End
# SPR 19713 change end

ifeq ($(HOST_COMPILATION),1)
	echo "SNMP Agent called"; $(CC) -o $(BIN)lte_oamSnmpAgt $(obj3) $(LIB) $(LIBS_SNMP);
endif
endif

advlogger :
	${MKD} build/
	${MKD} build/obj
	${MKD} bin/
	$(COMPILE) $(INCL) $(INCLUDE_RRC) $(ADVLOG_SRC) -o $(DST1)advlogging.o

lteMisc :
	@echo "lteMisc Build Called";
	$(COMPILE) $(INCL) $(LTE_ROOT)/common/src/lteMisc.c -o $(DST1)lteMisc.o

clean-all:
	@for i in $(SUBDIRS); do \
        echo "Clean all..."; \
        (cd $$i; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean); done
	rm -rf $(BIN)lte_oam; \
        rm -rf $(BIN)lte_oamCli;
	rm -rf $(BIN)lte_oamSDM;
	rm -rf $(BIN)lte_oamAgnt;
	rm -rf $(BIN)lte_oamSnmpAgt;
	rm -rf $(BIN)lte_oam_ut;
	rm -rf $(LDR)/libqcli.a;
	${RMD} build/
	${RMD} build/obj/
	${RMD} bin/

ifeq (1,$(RRH_SUPPORT))
.PHONY: common config-mgmt control-mgmt health-monitoring fault-mgmt perf-mgmt startup-n-intf-mgmt\
        hw-sw-mgmt interface port rrh qcli_intf_hdlr snmp
else
.PHONY: common config-mgmt control-mgmt health-monitoring fault-mgmt perf-mgmt startup-n-intf-mgmt\
        hw-sw-mgmt interface port qcli_intf_hdlr snmp aut
endif


ifeq (1,$(OAM_UT_FRAMEWORK))
aut:
	@echo "make in aut"; \
        (cd aut;$(MAKE) $(UT_MAKEFLAGS) all));
endif

common:
	@echo "make in common"; \
        (cd common;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));

config-mgmt:
	@echo "make in config-mgmt"; \
        (cd core;cd config_mgmt;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));

fault-mgmt:
	@echo "make in fault-mgmt"; \
        (cd core;cd fault_mgmt;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));

perf-mgmt:
	@echo "make in perf-mgmt"; \
        (cd core;cd perf_mgmt;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));

control-mgmt:
	@echo "make in control-mgmt"; \
        (cd core;cd control_mgmt;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));

soft_dwnld_mgr:
	@echo "make in soft_dwnld_mgr"; \
	@echo "(cd soft_dwnld_mgr;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));"
		(cd soft_dwnld_mgr;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));

oam_agnt:
	@echo "make in oam_agnt"; \
		(cd oam_agnt;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));

health-monitoring:
	@echo "make in health-monitoring"; \
        (cd core;cd health_monitoring;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));


startup-n-intf-mgmt:
	@echo "make in start-n-intf-mgmt"; \
        (cd core;cd startup_n_intf_mgmt;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));

hw-sw-mgmt:
	@echo "make in hw-sw-mgmt"; \
        (cd core;cd hw_sw_mgmt;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));

interface:
	@echo "make in interface"; \
        (cd interface;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));

qcli_intf_hdlr:
	@echo "make in qcli_intf_hdlr"; \
        (cd qcli_intf_hdlr;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));

port:
	@echo "make in port"; \
        (cd port;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));

tp-libxml2:
	@echo "make in tp-libxml2"; 
	@echo "$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));"
	(cd third_party;cd libxml2;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));

tp-curl:
	@echo "make in tp-curl";
	@echo "$(MAKE) $(MYMAKEFLAGS) all CC=$(CC))"
	(cd third-party;cd curl;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));


qcli:
	@echo "make in qcli"; \
        (cd qcli;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));
rrh:
	@echo "make in rrh"; \
        (cd rrh_intf;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));
#adding import folder compilation start
import :
	@echo "make in import";\
	(cd import; $(MAKE) $(MYMAKEFLAGS) all CC=$(CC));
#adding import folder compilation end

snmp:
	@echo "make in snmp";\
    (cd snmp;$(MAKE) $(MYMAKEFLAGS) all CC=$(CC));

clean-soft_dwnld_mgr:
	@echo "Clean all..."; \
        (cd soft_dwnld_mgr; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean);

clean-oam_agnt:
	@echo "Clean all..."; \
        (cd oam_agnt; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean);

clean-common:
	@echo "Clean all..."; \
        (cd common; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean);

clean-config-mgmt:
	@echo "Clean all..."; \
        (cd core;cd config_mgmt; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean);

clean-fault-mgmt:
	@echo "Clean all..."; \
        (cd core;cd fault_mgmt; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean);

clean-perf-mgmt:
	@echo "Clean all..."; \
        (cd core;cd perf_mgmt; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean);

clean-control-mgmt:
	@echo "Clean all..."; \
        (cd core;cd control_mgmt; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean);

clean-health-monitoring:
	@echo "Clean all..."; \
        (cd core;cd health_monitoring; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean);

clean-startup-n-intf-mgmt:
	@echo "Clean all..."; \
        (cd core;cd startup_n_intf_mgmt; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean);

clean-hw-sw-mgmt:
	@echo "Clean all..."; \
        (cd core;cd hw_sw_mgmt; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean);

clean-interface:
	@echo "Clean all..."; \
        (cd interface; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean);

clean-qcli_intf_hdlr:
	@echo "Clean all..."; \
        (cd qcli_intf_hdlr; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean);

clean-port:
	@echo "Clean all..."; \
        (cd port; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean);

clean-tp-libxml2:
	@echo "Clean all..."; \
        (cd third-party;cd libxml2; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean);

clean-tp-curl:
	@echo "Clean all..."; \
        (cd third-party;cd curl; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean);
clean-qcli:
	@echo "Clean all..."; \
        (cd qcli; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean);

clean-rrh:
	@echo "Clean all..."; \
        (cd rrh_intf; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean);

clean-import:
	@echo "Clean all..."; \
	(cd import; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean);

clean-snmp:
	@echo "Clean all..."; \
	(cd snmp; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean);

