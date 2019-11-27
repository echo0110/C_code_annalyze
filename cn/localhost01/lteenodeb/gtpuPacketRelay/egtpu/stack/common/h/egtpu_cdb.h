/***********************************************************************
**  FUNCTION:
**	Basic structures for the database
************************************************************************
**
**  FILE NAME:
**	egtpu_cdb.h
**
**  DESCRIPTION:
**	Contains basic structures for the database
**
**  DATE	    NAME			REFERENCE	REASON
**  ----	    ----			---------	------
**  Oct 2009    Vipul Aggarwal        		Initial
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/
#ifndef __EGTPU_CDB_DECL__
#define __EGTPU_CDB_DECL__



/* macro to obtain structure pointer from element ptr */
#define EGTPU_GET_STR_FROM_ELMT(str_type, elmt_ptr, elmt)               \
    ((str_type *)((unsigned long)(elmt_ptr) -                                 \
                  (unsigned long)&(((str_type *)0)->elmt)))

#define EGTPU_PATH_STATUS_UNKNOWN 0    
#define EGTPU_PATH_STATUS_OK      1    
#define EGTPU_PATH_STATUS_NOK     2    

typedef void (* list_walker_t)(YLNODE *, void *);

/* Path Structure */
typedef struct {
    /* Following variable shall store the timer duration waiting for echo 
     * response */
    egtpu_timer_id_t   tmr_id;
    egtpu_path_cntr_t  path_cntr;
    egtpu_timer_flag_t tmr_flag;
    UInt8              path_status;
} egtpu_path_struct_t;

/* TRANSPORT ADDRESS Context */
typedef struct {
    U8bit               entity_addr[EGTPU_MAX_IP6_ADDR_SIZE];
	egtpu_boolean_t 	is_alarm_raised; /* SPR 13765 fix */
    egtpu_ext_hdr_map_t ext_hdr;
    egtpu_ref_cntr_t    ref_cntr;
    egtpu_path_struct_t path_struct;
} egtpu_peer_ctxt_t;

/* SPR 10787 fix start */
typedef struct {
    egtpu_teid_t       teid_peer;
    RrcCellIndex  cellIndex;
} egtpu_peer_teid_cell_ctxt_t;
/* SPR 10787 fix end */

/* RAB Context */
typedef struct {
    egtpu_transaction_id_t transaction_id;

    /* SPR 10787 fix start */
    egtpu_peer_teid_cell_ctxt_t   teid_cell;
    /* SPR 10787 fix end */
    egtpu_teid_t           teid_peer_fwd;

    egtpu_peer_ctxt_t      *p_transport_addr;
    /* LIPA changes start */
    egtpu_peer_ctxt_t      *p_secondary_transport_addr; /* Secondary Transport Address for 
                                                           Secondary Tunnel for Supporting LIPA */
    /* LIPA changes end */
    egtpu_peer_ctxt_t      *p_forward_addr;

    egtpu_seq_num_t        seq_num_uplk;
    egtpu_seq_num_t        seq_num_dnlk;

    egtpu_sap_flag_t       sap_flag;
    egtpu_seq_flag_t       seq_disable_flag;
    U32bit                qos_id;

    egtpu_timer_drtn       reorder_timer_drtn;
    U8bit				  *p_tid;
    egtpu_seq_num_t        nxt_seq;
    egtpu_window_size      max_window_size;
    YHASH                 *H;
    S32bit                guard_arr[4];
    S32bit                max_seq_num;
    U8bit                 ip_traffic;
    U8bit                 reordering_reqd;
    U8bit                 *p_buf;
	U8bit              	  src_addr[EGTPU_MAX_IP6_ADDR_SIZE];
    U8bit                 end_marker_flag;
    /* INTRA_ENB_HO Fix Start */
    U8bit                 pdcpext_flag;
    /* INTRA_ENB_HO Fix End */
} egtpu_rab_ctxt_t;

/*structure containing pointer to GPDU, sequence no. etc
that is used in reordering*/
  typedef struct egtpu_hash_tbl_t
{
    YHNODE lnode;
    egtpu_seq_num_t seq_num;/*this would be of egtpu_seq_num_t*/
    U8bit *p_buff;
}egtpu_hash_tbl;
/*SPR_20405_START*/
struct __rab_ctxt_rec__ {
	YTNODE          __node__;
	egtpu_rab_ctxt_t ctxt;
};
/*SPR_20405_END*/
/* Interface to RAB Context table */
/* constructor rab tbl*/
egtpu_return_t egtpu_db_init_rab_ctxt_tbl (egtpu_global_t *);

/* destructor rab tbl*/
egtpu_void_t egtpu_db_destroy_rab_ctxt_tbl (egtpu_global_t *);

/*   rab context operations */
egtpu_rab_ctxt_t *egtpu_db_get_rab_ctxt (egtpu_global_t *);
egtpu_void_t egtpu_db_free_rab_ctxt (egtpu_rab_ctxt_t *);

/* Get Teid from RAB ctxt */
egtpu_teid_t egtpu_db_get_teid_key_rab_ctxt (egtpu_global_t *,
											egtpu_rab_ctxt_t *);

/* key operations , rab ctxt*/
egtpu_rab_ctxt_t *egtpu_db_sel_teid_key_rab_ctxt (egtpu_global_t *,
												egtpu_teid_t);

/* interface to iterate through RAB contexts */
egtpu_rab_ctxt_t *egtpu_db_get_rab_ctxt_by_idx (egtpu_global_t *,U32bit *);

/* Interface to PEER Context table */
/* constructor peer tbl*/
egtpu_return_t egtpu_db_init_peer_ctxt_tbl (egtpu_global_t *);

/* destructor peer tbl*/
egtpu_void_t egtpu_db_destroy_peer_ctxt_tbl (egtpu_global_t *);

/* peer context operations */
egtpu_peer_ctxt_t *egtpu_db_get_peer_ctxt (egtpu_global_t *);
egtpu_void_t egtpu_db_free_peer_ctxt (egtpu_peer_ctxt_t *, egtpu_global_t *);
/*+SPR #3456*/
egtpu_return_t egtpu_db_init_peer_ctxt_addr_tbl(egtpu_global_t *p_global);
egtpu_void_t egtpu_free_peer_ctxt_addr_tbl(egtpu_peer_ctxt_t *p_ctxt, egtpu_global_t *p_global);

/*-SPR #3456*/

/* key operation, peer context */
egtpu_peer_ctxt_t *egtpu_db_sel_addr_key_peer_ctxt (U8bit *);
egtpu_boolean_t egtpu_db_validate_peer_ctxt (egtpu_peer_ctxt_t *);

/* iteration support for QI */
/* + coverity 32072 */
egtpu_peer_ctxt_t *egtpu_db_get_first_peer_ctxt (void);
/* - coverity 32072 */
egtpu_peer_ctxt_t *egtpu_db_get_next_peer_ctxt (egtpu_peer_ctxt_t *);

/* teid-peer database interface */
egtpu_return_t egtpu_db_ins_teid_peer_key_peer_map (egtpu_peer_ctxt_t *,
													egtpu_rab_ctxt_t *);
egtpu_void_t egtpu_db_del_teid_peer_key_peer_map (egtpu_peer_ctxt_t *,
												egtpu_rab_ctxt_t *);
/*SPR_20405_START*/
YTNODE *egtpu_db_check_teid_peer_key_peer_map (egtpu_peer_ctxt_t *,
		egtpu_teid_t, YTNODE *);
/*SPR_20405_END*/
/* SPR 10787 fix start */
egtpu_rab_ctxt_t *egtpu_db_sel_teid_peer_key_peer_map (egtpu_peer_ctxt_t *,
        egtpu_peer_teid_cell_ctxt_t);

/* iteration interface to support query interface */
egtpu_rab_ctxt_t *egtpu_db_get_first_peer_map (U8bit *, egtpu_peer_teid_cell_ctxt_t, U32bit);
/* SPR 10787 fix end */
egtpu_rab_ctxt_t *egtpu_db_get_next_peer_map (egtpu_rab_ctxt_t *);

egtpu_void_t egtpu_db_start_echo_timers (egtpu_global_t *);
egtpu_index_t egtpu_db_get_index_from_addr (egtpu_peer_ctxt_t *);
egtpu_peer_ctxt_t *egtpu_db_reconstruct_peer_r (U8bit, U32bit,
										egtpu_error_t *, egtpu_global_t *);
egtpu_rab_ctxt_t *egtpu_db_reconstruct_rab_r (U8bit, U32bit,
										egtpu_error_t *, egtpu_global_t *);
egtpu_void_t egtpu_get_peer_rec_from_freelist (egtpu_peer_ctxt_t *);
egtpu_rab_ctxt_t *egtpu_sbdb_get_rab_rec_r(U32bit ,egtpu_global_t *);
egtpu_void_t egtpu_init_peer_ctxt_r(egtpu_peer_ctxt_t *);
egtpu_void_t egtpu_init_rab_ctxt_r(egtpu_rab_ctxt_t *);
egtpu_peer_ctxt_t	*egtpu_get_rec_addr_from_index(U32bit );
/*reordering*/
extern egtpu_return_t egtpu_db_init_hash_ctxt_tbl (egtpu_global_t * );
extern egtpu_return_t egtpu_db_init_hash_tbl (egtpu_global_t * );
extern egtpu_hash_tbl *egtpu_db_get_hash_ctxt (egtpu_global_t *);
extern egtpu_void_t egtpu_db_free_hash_ctxt (YHNODE *);
extern egtpu_return_t egtpu_init_hash_table(egtpu_global_t *p_global);
extern egtpu_return_t egtpu_init_hash(egtpu_global_t *p_global, YHASH *H );
egtpu_void_t egtpu_stop_reorder_timers (egtpu_global_t *p_global);
egtpu_return_t egtpu_db_init_timer_pool (egtpu_global_t *p_global);
egtpu_void_t egtpu_destroy_timer_pool(egtpu_global_t *p_global);
egtpu_void_t egtpu_db_destroy_hash_ctxt_tbl(egtpu_global_t *p_global);
egtpu_void_t egtpu_db_destroy_hash_tbl(egtpu_global_t *p_global);
egtpu_void_t egtpu_destroy_hash_table(egtpu_global_t *p_global);
#endif
