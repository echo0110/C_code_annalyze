#/**************************************************************************
#** FILE NAME  :
#**               Makefile for GW
#**# description :
#**            Makefile for GATEWAY shAREDF mODULE
#**  DATE     NAME              REFERENCE         REASON
#**  ----     ----              ---------         ------
#**  13Oct2002  Anjali     -----            Create Original
#**
#**   Copyright (C) 2006 Aricent Inc . All Rights Reserved
#******************************************************************************/
include $(GW_DIR)/build/make/vxworks/makefile.mk

GENLIB_OBJ_PATH  =      $(GW_DIR)/genlib/obj
GENLIB_OBJS      =      $(GENLIB_OBJ_PATH)/gl_gscim_file_io_opr_wrppr_api.o \
			$(GENLIB_OBJ_PATH)/gl_gscim_mutex_oprns_wrppr_api.o \
			$(GENLIB_OBJ_PATH)/gl_gscim_semphore_op_wrppr_api.o \
			$(GENLIB_OBJ_PATH)/gl_gscim_thrd_oprns_wrppr_api.o \
			$(GENLIB_OBJ_PATH)/gl_mm_appl_best_fit_alloc_schm.o \
			$(GENLIB_OBJ_PATH)/gl_mm_appl_sp_slectv_aloc_schm.o \
			$(GENLIB_OBJ_PATH)/gl_mm_cngst_mgmt_cnfg_api.o \
			$(GENLIB_OBJ_PATH)/gl_mm_glb_best_fit_alloc_schm.o \
			$(GENLIB_OBJ_PATH)/gl_mm_glb_defs_n_intrnl_funcs.o \
			$(GENLIB_OBJ_PATH)/gl_mm_init_deint_api_funcs.o \
			$(GENLIB_OBJ_PATH)/gl_mm_usage_mgmt_n_debug_ctrl.o \
			$(GENLIB_OBJ_PATH)/gl_tmport.o \
			$(GENLIB_OBJ_PATH)/gl_tm_basic_timer_oprns_api.o \
			$(GENLIB_OBJ_PATH)/gl_tm_glb_defs_n_intrnl_funcs.o \
			$(GENLIB_OBJ_PATH)/gl_tm_init_deinit_api_funcs.o \
			$(GENLIB_OBJ_PATH)/gl_tm_timer_server_impl_api.o \
			$(GENLIB_OBJ_PATH)/gl_ulm_gnrl_utils_api.o \
			$(GENLIB_OBJ_PATH)/gl_ulm_stats_api.o \
			$(GENLIB_OBJ_PATH)/gl_ulm_str_manipultn_api.o

GENLIB_OBJS_HASH      =      $(GENLIB_OBJ_PATH)/gl_gscim_file_io_opr_wrppr_api.o \
			$(GENLIB_OBJ_PATH)/gl_gscim_mutex_oprns_wrppr_api.o \
			$(GENLIB_OBJ_PATH)/gl_gscim_semphore_op_wrppr_api.o \
			$(GENLIB_OBJ_PATH)/gl_gscim_thrd_oprns_wrppr_api.o \
			$(GENLIB_OBJ_PATH)/gl_mm_appl_best_fit_alloc_schm.o \
			$(GENLIB_OBJ_PATH)/gl_mm_appl_sp_slectv_aloc_schm.o \
			$(GENLIB_OBJ_PATH)/gl_mm_cngst_mgmt_cnfg_api.o \
			$(GENLIB_OBJ_PATH)/gl_mm_glb_best_fit_alloc_schm.o \
			$(GENLIB_OBJ_PATH)/gl_mm_glb_defs_n_intrnl_funcs.o \
			$(GENLIB_OBJ_PATH)/gl_mm_init_deint_api_funcs.o \
			$(GENLIB_OBJ_PATH)/gl_mm_usage_mgmt_n_debug_ctrl.o \
			$(GENLIB_OBJ_PATH)/gl_tmport.o \
			$(GENLIB_OBJ_PATH)/gl_tm_basic_timer_oprns_api.o \
			$(GENLIB_OBJ_PATH)/gl_tm_glb_defs_n_intrnl_funcs.o \
			$(GENLIB_OBJ_PATH)/gl_tm_init_deinit_api_funcs.o \
			$(GENLIB_OBJ_PATH)/gl_tm_timer_server_impl_api.o \
			$(GENLIB_OBJ_PATH)/gl_ulm_gnrl_utils_api.o \
			$(GENLIB_OBJ_PATH)/gl_ulm_stats_api.o \
			$(GENLIB_OBJ_PATH)/gl_ulm_str_manipultn_api.o \
			$(GENLIB_OBJ_PATH)/gl_htm_user_key_oprns_api.o \
			$(GENLIB_OBJ_PATH)/gl_htm_glb_defs_n_intrnl_funcs.o \
			$(GENLIB_OBJ_PATH)/gl_htm_init_deinit_api_funcs.o \
			$(GENLIB_OBJ_PATH)/gl_htm_port_api_funcs.o
all         : objs

all_hash	: objs_hash

objs         :
		make -f makegl_os gl_tm_mt CFLAGS_CMN='$(CFLAGS_CMN) -DGL_TOTAL_MT_SAFE' \
			GW_CSPL_DIR=$(CSPL_DIR) -C $(GW_DIR)/genlib/make/vxworks
		make -f makegl_os gl_gscim_mt CFLAGS_CMN='$(CFLAGS_CMN) -DGL_TOTAL_MT_SAFE' \
			GW_CSPL_DIR=$(CSPL_DIR) -C $(GW_DIR)/genlib/make/vxworks
		make -f makegl_os gl_mm_mt CFLAGS_CMN='$(CFLAGS_CMN) -DGL_TOTAL_MT_SAFE' \
			GW_CSPL_DIR=$(CSPL_DIR) -C $(GW_DIR)/genlib/make/vxworks
		make -f makegl_os gl_tmport CFLAGS_CMN='$(CFLAGS_CMN) -DGL_TOTAL_MT_SAFE' \
			GW_CSPL_DIR=$(CSPL_DIR) -C $(GW_DIR)/genlib/make/vxworks
		make -f makegl_cmn gl_ulm CFLAGS_CMN='$(CFLAGS_CMN) -DGL_TOTAL_MT_SAFE' \
			GW_CSPL_DIR=$(CSPL_DIR) -C $(GW_DIR)/genlib/make/vxworks
		ld386 -r -o $(GW_DIR)/obj/vxworks/libgl.o $(GENLIB_OBJS)

objs_hash	:
		make -f makegl_os gl_tm_mt CFLAGS_CMN='$(CFLAGS_CMN) -DGL_TOTAL_MT_SAFE' \
			GW_CSPL_DIR=$(CSPL_DIR) -C $(GW_DIR)/genlib/make/vxworks
		make -f makegl_os gl_gscim_mt CFLAGS_CMN='$(CFLAGS_CMN) -DGL_TOTAL_MT_SAFE' \
			GW_CSPL_DIR=$(CSPL_DIR) -C $(GW_DIR)/genlib/make/vxworks
		make -f makegl_os gl_mm_mt CFLAGS_CMN='$(CFLAGS_CMN) -DGL_TOTAL_MT_SAFE' \
			GW_CSPL_DIR=$(CSPL_DIR) -C $(GW_DIR)/genlib/make/vxworks
		make -f makegl_os gl_tmport CFLAGS_CMN='$(CFLAGS_CMN) -DGL_TOTAL_MT_SAFE' \
			GW_CSPL_DIR=$(CSPL_DIR) -C $(GW_DIR)/genlib/make/vxworks
		make -f makegl_cmn gl_ulm CFLAGS_CMN='$(CFLAGS_CMN) -DGL_TOTAL_MT_SAFE' \
			GW_CSPL_DIR=$(CSPL_DIR) -C $(GW_DIR)/genlib/make/vxworks
		make -f makegl_db gl_htm_mt CFLAGS_CMN='$(CFLAGS_CMN) -DGL_TOTAL_MT_SAFE' \
			GW_CSPL_DIR=$(CSPL_DIR) -C $(GW_DIR)/genlib/make/vxworks
		ld386 -r -o $(GW_DIR)/obj/vxworks/libgl.o $(GENLIB_OBJS_HASH)

clean          :
		cd $(GW_DIR)/lib/vxworks &\
		rm -f libgl*.a &\
		cd $(GW_DIR)/obj/vxworks &\
                rm -f *.o; \
		cd $(GW_DIR)/genlib/obj/ ; \
		rm -f *.o

