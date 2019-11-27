#/**************************************************************************
#** FILE NAME  :
#**               Makefile for GW
#**# description :
#**            Makefile for GATEWAY shAREDF mODULE
#**  DATE     NAME              REFERENCE         REASON
#**  ----     ----              ---------         ------
#**  13Oct2002  Manish S      -----            Create Original
#**
#**   Copyright (C) 2006 Aricent Inc . All Rights Reserved
#******************************************************************************/
GW_DIR = $(SIGTRAN_DIR)/common/gw

all         : setup objs

all_hash    : setup objs objs_hash combine

setup       :
		if [ -d $(GW_DIR)/obj ] ; \
                then \
                   echo "Dir Exists" ; \
                else \
                   mkdir -p $(GW_DIR)/obj ; \
                fi
		if [ -d $(GW_DIR)/obj/$(CPU) ] ; \
                then \
                   echo "Dir Exists" ; \
                else \
                   mkdir -p $(GW_DIR)/obj/$(CPU) ; \
                fi
objs         :
		@cd $(GW_DIR)/genlib/make/alpha; \
 		CFLAGS_CMN=-DGL_TOTAL_MT_SAFE;\
		export CFLAGS_CMN;\
		make -f makegl_os gl_tm_mt; \
		make -f makegl_os gl_gscim_mt; \
                make -f makegl_os gl_mm_mt; \
		make -f makegl_os gl_tmport; \
	        make -f makegl_cmn gl_ulm
#	        cd $(GW_DIR)/obj/$(CPU);\
#	        ar -xv $(GW_DIR)/lib/alpha/libgl_tm_mtl.a;\
#		ar -xv $(GW_DIR)/lib/alpha/libgl_mm_mtl.a; \
#		ar -xv $(GW_DIR)/lib/alpha/libgl_gscim_mtl.a ;\
#		ar -xv $(GW_DIR)/lib/alpha/libgl_tmportl.a ;\
#		ar -xv $(GW_DIR)/lib/alpha/libgl_ulml.a  ;\
#		ar -rv $(GW_DIR)/lib/alpha/libgl.a $(GW_DIR)/obj/$(CPU)/*.o;

objs_hash : 
		@cd $(GW_DIR)/genlib/make/alpha; \
 		CFLAGS_CMN=-DGL_TOTAL_MT_SAFE;\
		export CFLAGS_CMN;\
		make -f makegl_db gl_htm_mt
#	        cd $(GW_DIR)/obj/$(CPU);\
#		ar -x $(GW_DIR)/lib/alpha/libgl_htm_mtl.a  ;\
#		ar -r $(GW_DIR)/lib/alpha/libgl.a $(GW_DIR)/obj/$(CPU)/*.o;
		
combine  :
		 $(LTE_LD) -r $(GW_DIR)/genlib/obj/*.o -o $(GW_DIR)/obj/alpha/genlib.o
clean          :
		cd $(GW_DIR)/lib/alpha; \
		rm -f libgl*.a; \
		cd $(GW_DIR)/obj/$(CPU); \
                rm -f *.o; \
		cd $(GW_DIR)/genlib/obj/ ; \
		rm -f *.o;

