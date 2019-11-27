/***********************************************************************
**  FUNCTION:
**      Definitions for Unitdata Indication API prototypes
************************************************************************
**
**  FILE NAME:
**      egtpu_udi.h
**
**  DATE           NAME               REFERENCE        REASON
**  ----           ----               ---------        ------
**  Oct, 2009     Vipul Aggarwal                    	Original
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/
#ifndef __EGTPU_DATA_DECL__
#define __EGTPU_DATA_DECL__

 egtpu_return_t egtpu_unitdata_ind(egtpu_global_t *,egtpu_info_t *);
 extern egtpu_return_t egtpu_chk_starvation(egtpu_rab_ctxt_t*,egtpu_global_t*);
 extern egtpu_return_t egtpu_reordering(egtpu_rab_ctxt_t*, egtpu_global_t*,
 															egtpu_info_t*);
extern egtpu_void_t egtpu_set_max_seq_num_and_min_seq_num_in_hash_tbl
									(egtpu_rab_ctxt_t *, egtpu_hash_tbl *);
extern egtpu_seq_num_t egtpu_find_min_in_hash_table(egtpu_rab_ctxt_t*,YHNODE*);
#endif
