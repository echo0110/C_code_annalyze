/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
#ifndef _RLC_APITYPES_H
#define _RLC_APITYPES_H

/* Incoming upper layer control API types */
#define RLC_TM_CONFIG_REQ           0    
#define RLC_UM_CONFIG_REQ           1   
#define RLC_AM_CONFIG_REQ           2   
#define RLC_STOP_REQ                3   
#define RLC_CONTINUE_REQ            4   
#define RLC_SUSPEND_REQ             5   
#define RLC_RESUME_REQ              6   
#define RLC_UM_CIPHER_CONFIG_REQ    7
#define RLC_AM_CIPHER_CONFIG_REQ    8
#define RLC_UM_GET_COUNTC_REQ       9
#define RLC_AM_GET_COUNTC_REQ       10
#define RLC_DELETE_REQ              11   

/* Incoming upper layer data API types */
#define RLC_TM_DATA_REQ             12
#define RLC_UM_DATA_REQ             13
#define RLC_AM_DATA_REQ             14 

/* MAC API types */
#define MAC_TX_NOTE                 15      
#define MAC_RX_NOTE                 16

/* Incoming LME API types */       
#define RLC_INITDB_REQ              17            
#define RLC_SET_DEFAULT_PARAM_REQ   18
#define RLC_GET_DEFAULT_PARAM_REQ   19
#define RLC_INIT_STATS_REQ          20
#define RLC_GET_STATS_MODE_REQ      21
#define RLC_SET_STATS_MODE_REQ      22
#define RLC_GET_STATS_REQ           23
#define RLC_INIT_TRACE_LEVEL_REQ    24
#define RLC_GET_TRACE_LEVEL_REQ     25
#define RLC_SET_TRACE_LEVEL_REQ     26

/* Outgoing upper layer control API types */
#define RLC_TM_CONFIG_CNF           27     
#define RLC_UM_CONFIG_CNF           28    
#define RLC_AM_CONFIG_CNF           29    
#define RLC_STOP_CNF                30       
#define RLC_CONTINUE_CNF            31    
#define RLC_SUSPEND_CNF             32    
#define RLC_RESUME_CNF              33 
#define RLC_UM_CIPHER_CONFIG_CNF    34
#define RLC_AM_CIPHER_CONFIG_CNF    35
#define RLC_UM_GET_COUNTC_CNF       36
#define RLC_AM_GET_COUNTC_CNF       37
#define RLC_DELETE_CNF              38
#define RLC_STATUS_IND              39

/* Outgoing upper layer data API types */
#define RLC_TM_DATA_IND             40       
#define RLC_UM_DATA_IND             41      
#define RLC_AM_DATA_IND             42      
#define RLC_AM_DATA_CNF             43

/* Outgoing LME API types */      
#define RLC_INITDB_CNF              44
#define RLC_SET_DEFAULT_PARAM_CNF   45
#define RLC_GET_DEFAULT_PARAM_CNF   46
#define RLC_INIT_STATS_CNF          47
#define RLC_GET_STATS_MODE_CNF      48
#define RLC_SET_STATS_MODE_CNF      49
#define RLC_GET_STATS_CNF           50
#define RLC_INIT_TRACE_LEVEL_CNF    51
#define RLC_GET_TRACE_LEVEL_CNF     52
#define RLC_SET_TRACE_LEVEL_CNF     53

#define RLC_REQ_CNF_DIFF    (RLC_TM_CONFIG_CNF - RLC_TM_CONFIG_REQ)

#ifdef RLC_MAC_SIM

#define RLC_STACK_API_IDS_MAX       100

#define RLC_SKIP_PDU_REQ            101
#define RLC_SKIP_PDU_CNF            102
#define RLC_SKIP_ACK_REQ            103
#define RLC_SKIP_ACK_CNF            104
#define RLC_DUP_ACKS_REQ            105
#define RLC_DUP_ACKS_CNF            106

#endif /* RLC_MAC_SIM */

#endif /* _RLC_APITYPES_H */
