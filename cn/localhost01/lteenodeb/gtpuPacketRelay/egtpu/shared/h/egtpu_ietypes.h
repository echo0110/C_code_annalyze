/***********************************************************************
**  FUNCTION:
**	GTP-U IE type definitions
************************************************************************
**
**  FILE NAME:
**	egtpu_ietpyes.h
**
**  DESCRIPTION:
**	Contains IE type definitions for GTP-U stack. IEs for APIs are
**      assigned identifiers in a way that they dont clash with GTP-U
**      protocol IE identifiers.
**      Also defined are presence flags, sizes, data definitions
**      for IEs.
**
**  DATE	    NAME			REFERENCE		REASON
**  ----	    ----			---------		------
**  Oct 2009    Vipul Aggarwal        			Initial
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/
#ifndef _EGTPU_IE_DECL_
#define _EGTPU_IE_DECL_

/* IE Type definitions */
#define EGTPU_IE_CAUSE                           1
#define EGTPU_IE_TRANSACTION_ID                  2
#define EGTPU_IE_SAP_FLAG                        3
#define EGTPU_IE_SEQ_NUM                         4
#define EGTPU_IE_NPDU_NUM                        5
#define EGTPU_IE_PDCP_NUM                        6
#define EGTPU_IE_PORT_NUM                        7
#define EGTPU_IE_NEXT_IDX                        32
#define EGTPU_IE_REC_COUNT                       9
#define EGTPU_IE_SEQ_DISABLE                     10
#define EGTPU_IE_RECOVERY                        14
#define EGTPU_IE_TEID_DATA_I                     16
#define EGTPU_IE_GSN_ADDR                        133
#define EGTPU_IE_GSN_ADDR_FLAG                   134  /* SPR 13765 fix */
#define EGTPU_IE_QOS_PROFILE                     135
#define EGTPU_IE_EXT_HDR_TYPE_LIST               141
/* SPR 10787 fix start */
#define EGTPU_IE_CELL_INDEX_I                    142
/* SPR 10787 fix end */
#define EGTPU_IE_PRIVATE_EXTENSION               255

/*reordering*/
#define EGTPU_IE_REORDERING_REQD                 8
/* record types for QI */

/* INTRA_ENB_HO Fix Start */
#define EGTPU_IE_PDCP_EXT_FLAG                   17
/* INTRA_ENB_HO Fix End */

/* ordering of PEER-CTXT after RAB-CTXT is an assumption */
/* take care */
#define EGTPU_RAB_CTXT_INFO                      150
#define EGTPU_PEER_CTXT_INFO                     151
#define EGTPU_INIT_HASH_CTXT_POOL	        	1000
#define EGTPU_MAX_HASH_CTXT_POOL	            	20000
#define EGTPU_TIMER_MSG_SIZE						32
#define EGTPU_INIT_TIMER_POOL_SIZE				1000
#define EGTPU_MAX_TIMER_POOL_SIZE				20000
/*+SPR #3456*/
/* SPR_10908_fix Start */
#define EGTPU_MAX_PEER_CTXT                     48                 
/* SPR_10908_fix End */
/*-SPR #3456*/
/* IE Type presence flags definitions */
#define EGTPU_IE_CAUSE_FLAG                      0x00000001
#define EGTPU_IE_SEQ_NUM_UPLK_FLAG               0x00000002
#define EGTPU_IE_SEQ_NUM_DNLK_FLAG               0x00000004
#define EGTPU_IE_NPDU_NUM_FLAG                   0x00000008
#define EGTPU_IE_PDCP_NUM_FLAG                   0x00000010
#define EGTPU_IE_TEID_SELF_FLAG                  0x00000020
#define EGTPU_IE_TEID_PEER_FLAG                  0x00000040
#define EGTPU_IE_TRANSACTION_ID_FLAG             0x00000080
#define EGTPU_IE_TRANSPORT_ADDR_FLAG             0x00000100
#define EGTPU_IE_SEQ_NUM_FLAG                    0x00000200
#define EGTPU_IE_SAP_FLAG_FLAG                   0x00000400
#define EGTPU_IE_NEXT_IDX_FLAG                   0x00000800
#define EGTPU_IE_REC_COUNT_FLAG                  0x00001000
#define EGTPU_IE_SEQ_FLAG                        0x00002000
/*reordering*/
#define EGTPU_IE_REORDERING_FLAG                 0x00004000    
#define EGTPU_IE_SOURCE_TRANSPORT_ADDR_FLAG      0x00008000
#define EGTPU_IE_PVT_EXTN_FLAG      	         0x00010000
/* INTRA_ENB_HO Fix Start */
#define EGTPU_IE_PDCP_EXT_FLAG_P                 0x00020000    
/* INTRA_ENB_HO Fix End */
/* SPR 10787 fix start */
#define EGTPU_IE_CELL_ID_FLAG                    0x00040000
/* SPR 10787 fix end */
#define EGTPU_EXT_HDR_COMPREHENSION_REQD         0x80
/* Ext Hdr Bit Map definition */
#define EGTPU_EXT_HDR_PDCP                       0x01
#define EGTPU_EXT_HDR_NR_RAN                       0x84
/* + SPR 17435 */
/* UDP extension header definition, but currently not supported */
#define EGTPU_EXT_HDR_UDP                        0x02
#define EGTPU_UDP_EXT_HDR                        0x40
/* - SPR 17435 */
/* extension header - PDCP number Id */
#define EGTPU_PDCP_PDU_EXT_HDR                   0xc0
#define EGTPU_LEN_OF_PDCP_PDU_EXT_HDR            1

/* request codes for QI */
#define EGTPU_ALL_CTXT                           0
#define EGTPU_FLTR_CTXT_BY_PARM                  1
#define EGTPU_NUM_CTXT                           2

/* Size */
#define EGTPU_SIZE_OF_LENGTH                     2
#define EGTPU_SIZE_OF_TRANSACTION_ID             4
#define EGTPU_SIZE_OF_TEID                       4
#define EGTPU_SIZE_OF_SEQ_NUM                    2
#define EGTPU_SIZE_OF_PDCP_NUM                   2
#define EGTPU_SIZE_OF_NPDU_NUM                   1
#define EGTPU_SIZE_OF_TYPE                       1
#define EGTPU_SIZE_OF_RECOVERY                   1
#define EGTPU_SIZE_OF_PORT_NUM                   2
#define	EGTPU_SIZE_OF_QOS_ID                     4
#define EGTPU_SIZE_OF_NEXT_IDX_FLAG              4
#define EGTPU_SIZE_OF_REC_COUNT_FLAG             4
#define EGTPU_SIZE_OF_IPV4_ADDR                  4
#define EGTPU_SIZE_OF_IPV6_ADDR                  16
#define EGTPU_SIZE_OF_RESULT                     1
#define EGTPU_SIZE_OF_STAT_ENTRY                 4
#define EGTPU_SIZE_OF_TIMER_DURATION             4
#define EGTPU_SIZE_OF_RETRY_COUNT                2
#define EGTPU_SIZE_OF_TRACE_FLAG                 1
#define EGTPU_MIN_SIZE_OF_EXT_HDR_TYPE_LIST		 3
#define EGTPU_MAX_IP_ADDR_SIZE                   6
#define EGTPU_MAX_IP6_ADDR_SIZE                  18
#define EGTPU_MIN_SRC_IP_ADDR					 1
#define EGTPU_MAX_SRC_IP_ADDR					10
/* SPR 10787 fix start */
#define EGTPU_SIZE_OF_CELL_ID                    1
/* SPR 10787 fix end */
/*reordering*/
#define EGTPU_SIZE_OF_WINDOW                     2
#define EGTPU_SIZE_OF_REORDER_TIMER              4

/* different ip traffic classes */
#define EGTPU_SUBSCRIPTION_TRAFFIC_CLASS		0
#define EGTPU_CONVERSATIONAL_TRAFFIC_CLASS		1
#define EGTPU_STREAMING_TRAFFIC_CLASS			2
#define EGTPU_INTERACTIVE_TRAFFIC_CLASS			3
#define EGTPU_BKGROUND_TRAFFIC_CLASS			4

/* Macros of SAP flags */
#define EGTPU_RX_SAP                             0x01
#define EGTPU_TX_SAP                             0x02
#define EGTPU_FROM_RELAY_SAP                     0x04
#define EGTPU_TO_RELAY_SAP                       0x08
#define EGTPU_RELOC_DATA_SAP                     0x10
#define EGTPU_FWD_SAP                            0x20
#define EGTPU_ALL_SAP                            0x0f

/* Min, Max Values */
#define EGTPU_MIN_SEQ_NUM						0
#define EGTPU_MAX_SEQ_NUM						65535
#define EGTPU_INV_SEQ_NUM						-1
#define EGTPU_INV_TEID_VALUE					0
#define EGTPU_RECOVERY_VALUE					0

/* Macros for Seq disable flag */
#define EGTPU_SEQ_ENABLE  0
#define EGTPU_SEQ_DISABLE 1

/* SPR 13280 fix start */
/* Following denotes number of bits in the tunnel ID */
#define NUM_BITS_IN_TEID  32
/* SPR 13280 fix end */

/* Data type for IE definitions */
typedef U8bit  egtpu_cause_t;
typedef U8bit  egtpu_sap_flag_t;
typedef U8bit  egtpu_seq_flag_t;
typedef U8bit  egtpu_ext_hdr_map_t;
typedef U8bit  egtpu_path_cntr_t;
typedef U8bit  egtpu_timer_flag_t;
typedef U8bit  egtpu_npdu_num_t;
typedef U16bit egtpu_seq_num_t;
typedef U16bit egtpu_length_t;
typedef U16bit egtpu_pdcp_num_t;
typedef U16bit egtpu_port_num_t;
typedef U32bit egtpu_ref_cntr_t;
typedef U32bit egtpu_transaction_id_t;
typedef U32bit egtpu_teid_t;
typedef U32bit egtpu_timer_id_t;

/* SPR 13765 fix START */
typedef enum EgtpuAlarmTypeTag
{
    EGTPU_INVALID,
    EGTPU_SEND_ALARM_WITH_PATH_SUCCESS,
    EGTPU_SEND_ALARM_ONLY
}EgtpuAlarmTypeT;
/* SPR 13765 fix END*/

/*invalid timer id*/
#define EGTPU_INVALID_TIMER_ID                    0x00
typedef struct {
    U32bit curr_size;
    U32bit step_size;
    U32bit max_size;
} egtpu_size_struct_t;

/* structure to hold decoded initialization information */
typedef struct {
    egtpu_size_struct_t num_rab_rec;
    U16bit             num_peer_rec;
    U8bit			   num_my_addr4;
	U8bit			   num_my_addr6;
    U8bit              my_addr[EGTPU_MAX_SRC_IP_ADDR][EGTPU_MAX_IP_ADDR_SIZE];
	U8bit              my_addr6[EGTPU_MAX_SRC_IP_ADDR][EGTPU_MAX_IP6_ADDR_SIZE];
    /*+SPR #3456*/
    U32bit            *peer_ctxt_addr[EGTPU_MAX_PEER_CTXT]; /*Memory Address of different Peer Ctxt*/
    U8bit              num_active_peer_rec;
    /*-SPR #3456*/
} egtpu_initdb_info_t;

/* structure to hold decoded timer information */
typedef struct {
    U16bit max_retries;
    U32bit tmr_dur;
} egtpu_timer_info_t;

/* structure to hold decoded QI information */
typedef struct {
    U8bit rec_type;
    U8bit req_code;

    struct {
        U8bit type;
        union {
            U8bit  *addr;
            U32bit teid;
            U32bit xid;
        } val;
    } fltr;

    U32bit next_idx;
    U32bit rec_count;
} egtpu_qi_info_t;

/* super structure to keep decoded information for every API */
typedef struct {
    U8bit                 *p_buff;
    U32bit                ie_presence;
    U8bit                 *p_peer_addr; /* peer's address */
    U8bit                 *p_src_addr; /* own address */
    U8bit                 *p_pvt_extn; /* private extension */
    U8bit				   mem_flag;   /*to free memory*/
    /* SPR 10787 fix start */
    InternalCellIndex      cellIndex;
    /* SPR 10787 fix end */
    egtpu_teid_t           teid_self;
    egtpu_teid_t           teid_peer;
    egtpu_transaction_id_t transaction_id;
    egtpu_length_t         hdr_len;      /* gtp hdr-len inc min */
    egtpu_length_t         pdu_len;      /* pdu len */
    egtpu_seq_num_t        seq_num_uplk;
    egtpu_seq_num_t        seq_num_dnlk;
    egtpu_pdcp_num_t       pdcp_num;

    egtpu_port_num_t       port_num;     /* peer's port number */

    U8bit                 pdu_type;

    egtpu_cause_t          cause;
    egtpu_sap_flag_t       sap_flag;
    egtpu_npdu_num_t       n_pdu_num;

    egtpu_qi_info_t        qi_info;      /* decoded QI information */
    U8bit                 seq_disable;
    /* INTRA_ENB_HO Fix Start */   
    U8bit                 pdcpext_flag;  /* To Enable or Disable the flag for SN in case of INTRA eNB HO*/ 
    /* INTRA_ENB_HO Fix End */   
    U32bit                qos_id;
    /*reordering*/ 
    U8bit                 ip_traffic;
    U8bit                 reordering_reqd;    
	egtpu_window_size	  max_window_size;
	egtpu_timer_drtn	 	  reorder_timer_drtn;
} egtpu_info_t;

#endif
