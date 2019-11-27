/***********************************************************************
**  FUNCTION:
**      Definitions for Path Management Messages prototypes
************************************************************************
**
**  FILE NAME:
**      gtpu_pmm.h
**
**  DATE           NAME               REFERENCE        REASON
**  ----           ----               ---------        ------
**  Oct, 2009     Vipul Aggarwal                    	Original
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/
#ifndef __EGTPU_PMM_DECL__
#define __EGTPU_PMM_DECL__
/*+SPR #3456*/
egtpu_return_t egtpu_echo_timeout (egtpu_global_t *,egtpu_peer_ctxt_t *);
/*-SPR #3456*/
egtpu_return_t egtpu_echo_response (egtpu_global_t *,egtpu_info_t *);
egtpu_return_t egtpu_echo_request (egtpu_global_t *,egtpu_info_t *);
egtpu_return_t egtpu_send_echo_response (egtpu_info_t *);
egtpu_return_t egtpu_send_echo_req (egtpu_global_t *,U8bit *);
egtpu_return_t __send_path_success_ind_to_cp__(egtpu_global_t *p_global,U8bit *p_addr, U8bit alarm_flag); /* SPR 13765 fix */

extern egtpu_return_t egtpu_reorder_timeout(egtpu_global_t * ,egtpu_info_t *);
extern egtpu_void_t egtpu_send_hash_node_relay(egtpu_global_t*, egtpu_info_t*);
extern egtpu_void_t egtpu_send_gpdu_relay(egtpu_global_t * ,egtpu_info_t *);
#endif
