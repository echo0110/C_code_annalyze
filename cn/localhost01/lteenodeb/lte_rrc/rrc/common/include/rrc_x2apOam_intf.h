/******************************************************************************
*
*   FILE NAME   : x2apOam_intf.h
*
*   DESCRIPTION : This file Interface API for X2AP/OAM
*
*   DATE            AUTHOR         REFERENCE       REASON
*   12-JULY-2010    Vaibhav Singh  ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _X2AP_OAM_INFT_H_ 
#define _X2AP_OAM_INFT_H_
#include "rrc_defines.h"
#include "x2ap_api.h"


#define RRC_X2APOAM_MODULE_ID    RRC_OAM_MODULE_ID

typedef struct 
{
	/* SPR_17664_start*/
    U8                           num_srvd_cell;                  /*^ M,0,H,1,MAX_SERVED_CELLS ^*/
	/* SPR_17664_end*/
 x2apOam_srvd_cell_arry_t     srvd_cell_arr[MAX_SERVED_CELLS];/*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2apOam_srvd_cells_t;


typedef struct
{
    x2_enb_comm_info_t       local_enb_comm_info; /*^ M,0,N,0,0 ^*/
    /* number of Peer ENBs supported */
	/* SPR_17664_start*/
    U8                       num_peer_enb;     /*^ M,0,H,0,MAX_PEER_ENB ^*/
	/* SPR_17664_end*/
	x2_enb_comm_info_t       peer_enb_comm_info[MAX_PEER_ENB];/*^ M,0,OCTET_STRING,VARIABLE ^*/
    /*sctp communcation parameters for all peers/assocaiations*/
    x2ap_conf_param_t        conf_param;         /*^ M,0,N,0,0 ^*/ 
    x2_gb_enb_id_t           global_enb_id;      /*^ M,0,N,0,0^*/
    x2apOam_srvd_cells_t     srvd_cell;       /*^ M,0,N,0,0^*/  
    x2ap_gu_grp_ids_t        x2ap_gu_grp_id;     /*^ M,0,N,0,0^*/
    U8                          log_level;    /*^ M,0,H,0,6 ^*/  

    /* MENB CHANGES - START */
    U8                       num_peer_gnb;
    /*^ M, 0, H, 0, MAX_PEER_GNB ^*/

	x2_enb_comm_info_t       peer_gnb_comm_info[MAX_PEER_GNB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    /* MENB CHANGES - END */
}x2ap_info_t;            


typedef struct 
{
 #define RRC_X2AP_PROVISION_INFO_PRESENT 0x01
 #define SON_MODULE_INFO_PRESENT 0x02
   U16 bitmask; /*^ BITMASK ^*/
   x2ap_info_t x2ap_info;              /*^ O,RRC_X2AP_PROVISION_INFO_PRESENT,N,0,0 ^*/
   son_comm_info_t son_comm_info;      /*^ O,SON_MODULE_INFO_PRESENT,N,0,0 ^*/
}x2ap_oam_provision_req_t;             /*^ API, X2AP_OAM_PROVISION_REQ ^*/

/******************************************************************************
                        X2_AP_OAM_PROVISION_RESP
******************************************************************************/
typedef struct
{
  U8            response;       /*^ M, 0, H, 0, 1 ^*/  
}x2ap_oam_provision_resp_t; /*^ API, X2AP_OAM_PROVISION_RESP ^*/

/******************************************************************************
*  X2AP_OAM_GET_LOG_LEVEL_RESP
******************************************************************************/
typedef struct _x2ap_oam_get_log_level_resp_t
{
#define X2AP_OAM_LOG_LEVEL_PRESENT 0x01
    U16                     bitmask;    /*^ BITMASK ^*/
    U8                  log_on_off;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_oam_log_on_off_et */
    U8                  log_level;
/*^ O, X2AP_OAM_LOG_LEVEL_PRESENT, H, 0, 6 ^*/ /* rrc_oam_log_level_et */

} x2ap_oam_get_log_level_resp_t; /*^ API, X2AP_OAM_GET_LOG_LEVEL_RESP ^*/

/******************************************************************************
*   X2AP_OAM_SET_LOG_LEVEL_REQ
******************************************************************************/
typedef struct _x2ap_oam_set_log_level_req_t
{
    U8                  log_level;
/*^ M, 0, H, 0, 6 ^*/ /* rrc_oam_log_level_et */

} x2ap_oam_set_log_level_req_t; /*^ API, X2AP_OAM_SET_LOG_LEVEL_REQ ^*/

/******************************************************************************
*   X2AP_OAM_SET_LOG_LEVEL_RESP
******************************************************************************/
typedef struct _x2ap_oam_set_log_level_resp_t
{
    U8                  response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

} x2ap_oam_set_log_level_resp_t; /*^ API, X2AP_OAM_SET_LOG_LEVEL_RESP ^*/

/******************************************************************************
 * *   X2AP_OAM_LOG_ENABLE_REQ
 * ******************************************************************************/
typedef struct _x2ap_oam_log_enable_req_t
{
        U8                  log_on_off;
        /*^ M, 0, H, 0, 1 ^*/ /* rrc_oam_log_on_off_et */

} x2ap_oam_log_enable_req_t; /*^ API, X2AP_OAM_LOG_ENABLE_REQ ^*/

/******************************************************************************
 * *   X2AP_OAM_LOG_ENABLE_RESP
 * ******************************************************************************/
typedef struct _x2ap_oam_log_enable_resp_t
{
        U8                  response;
        /*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

} x2ap_oam_log_enable_resp_t; /*^ API, X2AP_OAM_LOG_ENABLE_RESP ^*/


/******************************************************************************
                         X2AP_OAM_LINK_DOWN_IND
******************************************************************************/
/*SPR 14503 Fix Start*/
#define X2AP_OAM_X2_LINK_DOWN_IPV4_ADDRESS_PRESENT        0x01
#define X2AP_OAM_X2_LINK_DOWN_IPV6_ADDRESS_PRESENT        0x02
#define X2AP_OAM_X2_LINK_DOWN_PORT_PRESENT                            0x04
/*SPR 14503 Fix Stop*/

/* MENB CHANGES - START */
#define X2AP_OAM_X2_LINK_DOWN_ENB_GB_ID_PRESENT          0x08
#define X2AP_OAM_X2_LINK_DOWN_GNB_GB_ID_PRESENT          0x10
typedef struct 
{
/*SPR 14503 Fix Start*/
  U8 bitmask;    /*^ BITMASK ^*/
/*SPR 14503 Fix Stop*/
    x2_gb_enb_id_t           peer_enb_gb_id;
    /*^ O, X2AP_OAM_X2_LINK_DOWN_ENB_GB_ID_PRESENT, H, 1, 1 ^*/    

/*SPR 14503 Fix Start*/
   U8 ip_addr[MAX_X2AP_IP_ADDRESS_LENGTH];        
    /*^ O, X2AP_OAM_X2_LINK_DOWN_IPV4_ADDRESS_PRESENT , OCTET_STRING, FIXED ^*/

    U8 ipv6_addr[MAX_X2AP_IPV6_ADDRESS_LENGTH]; 
/*^ O, X2AP_OAM_X2_LINK_DOWN_IPV6_ADDRESS_PRESENT , OCTET_STRING, FIXED ^*/

    U16       port; 
 /*^ O, X2AP_OAM_X2_LINK_DOWN_PORT_PRESENT,N,0,0 ^*/

/*SPR 14503 Fix Stop*/
    x2_gb_gnb_id_t           peer_gnb_gb_id;
    /*^ O, X2AP_OAM_X2_LINK_DOWN_GNB_GB_ID_PRESENT, N, 0, 0 ^*/    

}x2ap_oam_x2_link_down_t;              /*^ API, X2AP_OAM_LINK_DOWN_IND ^*/
/* MENB CHANGES - END */

/******************************************************************************
                         X2AP_OAM_LINK_UP_IND
******************************************************************************/
/*SPR 14503 Fix Start*/
/* MENB CHANGES - START */
#define X2AP_OAM_X2_LINK_UP_IPV4_ADDRESS_PRESENT        0x01
#define X2AP_OAM_X2_LINK_UP_IPV6_ADDRESS_PRESENT        0x02
#define X2AP_OAM_X2_LINK_UP_PORT_PRESENT                0x04
#define X2AP_OAM_X2_LINK_UP_ENB_ID_PRESENT              0x08
#define X2AP_OAM_X2_LINK_UP_GNB_ID_PRESENT              0x10
/*SPR 14503 Fix Stop*/
typedef struct 
{
/*SPR 14503 Fix Start*/
   U8 bitmask;    /*^ BITMASK ^*/
/*SPR 14503 Fix Stop*/
/* SPR-16334 START */
    x2_gb_enb_id_t           peer_enb_gb_id;
    /*^ O, X2AP_OAM_X2_LINK_UP_ENB_ID_PRESENT, N, 0, 0 ^*/    
    x2_enb_comm_info_t       enb_comm_info;        /*^ M,0,N,0,0 ^*/
/* SPR-16334 END */
/*SPR 14503 Fix Start*/
    U8 ip_addr[MAX_X2AP_IP_ADDRESS_LENGTH];        
    /*^ O, X2AP_OAM_X2_LINK_UP_IPV4_ADDRESS_PRESENT , OCTET_STRING, FIXED ^*/

    U8 ipv6_addr[MAX_X2AP_IPV6_ADDRESS_LENGTH]; 
/*^ O, X2AP_OAM_X2_LINK_UP_IPV6_ADDRESS_PRESENT , OCTET_STRING, FIXED ^*/

    U16       port;  
/*^ O, X2AP_OAM_X2_LINK_UP_PORT_PRESENT,N,0,0 ^*/

/*SPR 14503 Fix Stop*/
    x2_gb_gnb_id_t           peer_gnb_gb_id;
    /*^ O, X2AP_OAM_X2_LINK_UP_GNB_ID_PRESENT, N, 0, 0 ^*/    
}x2ap_oam_x2_link_up_t;              /*^ API, X2AP_OAM_LINK_UP_IND ^*/
/* MENB CHANGES - END */

/******************************************************************************
                         X2AP_OAM_LINK_SETUP_REQ
******************************************************************************/
typedef struct 
{
    x2_gb_enb_id_t           peer_enb_gb_id;       /*^ M, 0, H, 1, 1 ^*/    
}x2ap_oam_x2_link_setup_req_t;              /*^ API, X2AP_OAM_LINK_SETUP_REQ ^*/

/******************************************************************************
                        X2AP_OAM_LINK_SETUP_RESP
******************************************************************************/
typedef struct 
{
    U8            response;       /*^ M, 0, H, 1, 1 ^*/    
}x2ap_oam_x2_link_setup_resp_t;         /*^ API, X2AP_OAM_LINK_SETUP_RESP ^*/

/******************************************************************************
                         X2AP_OAM_CLEANUP_RESP
******************************************************************************/
typedef struct 
{
    U8            response;       /*^ M, 0, H, 1, 1 ^*/    
}x2ap_oam_cleanup_resp_t;              /*^ API, X2AP_OAM_CLEANUP_RESP ^*/

/*sctp parameters start*/
/******************************************************************************
                         X2AP_OAM_GET_SCTP_STATUS_REQ
******************************************************************************/
/* MENB CHNAGES - START */
typedef struct
{
    U8                          num_peer_gnb;
    /*^ M,0,H,0,MAX_PEER_GNB ^*/

	x2_gb_gnb_id_t              gb_gnb_id[MAX_PEER_GNB];
    /*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_oam_gb_gnb_id_list_info_t;
/* MENB CHNAGES - END */

typedef struct
{
	/* SPR_17664_start*/
    U8                          num_peer_enb;     /*^ M,0,H,0,MAX_PEER_ENB ^*/
	/* SPR_17664_end*/
	x2_gb_enb_id_t              gb_enb_id[MAX_PEER_ENB]; /*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_oam_gb_enb_id_list_info_t;

#define X2AP_OAM_GLOBAL_ENB_STATUS_LIST_PRESENT 0x01

/* MENB CHNAGES - START */
#define X2AP_OAM_GLOBAL_GNB_STATUS_LIST_PRESENT 0x02
/* MENB CHNAGES - END */
typedef struct 
{
    U16                            bitmask;          /*^ BITMASK ^*/
    
    x2ap_oam_gb_enb_id_list_info_t x2ap_oam_gb_enb_status_list; 
    /*^ O, X2AP_OAM_GLOBAL_ENB_STATUS_LIST_PRESENT, N, 0, 0 ^*/

    /* MENB CHNAGES - START */
    x2ap_oam_gb_gnb_id_list_info_t x2ap_oam_gb_gnb_status_list; 
    /*^ O, X2AP_OAM_GLOBAL_GNB_STATUS_LIST_PRESENT, N, 0, 0 ^*/
    /* MENB CHNAGES - END */
}x2ap_oam_get_sctp_status_req_t;              /*^ API, X2AP_OAM_GET_SCTP_STATUS_REQ ^*/

/******************************************************************************
                         X2AP_OAM_GET_SCTP_STATUS_RESP
******************************************************************************/

typedef enum
{
    X2AP_OAM_ENB_ID_NOT_FOUND,
    /* MENB CHANGES - START */
    X2AP_OAM_GNB_ID_NOT_FOUND,
    /* MENB CHANGES - END */
    X2AP_OAM_STATE_NOT_ACTIVE
}x2ap_oam_sctp_staus_resp_cause_et;

typedef struct
{
    x2_gb_enb_id_t  global_enb_id;      /*^ M,0,N,0,0^*/

    U32             status;  /*^ M,0,H,0,9 ^*/ /* s1ap_oam_sctp_status_et */
    
    U32             in_streams; /*^ M, 0, N, 0, 0 ^*/
    
    U32             out_streams; /*^ M, 0, N, 0, 0 ^*/
    
    U32             local_port; /*^ M, 0, N, 0, 0 ^*/

    rrc_timeval_t    start_time; /*^ M, 0, N, 0, 0 ^*/ 
    
    U8              primary_peer_addr[MAX_IP_ADDRESS_LENGTH]; /*^ M,0,OCTET_STRING,FIXED ^*/ 
}x2ap_oam_gb_enb_id_status_params_t;

typedef struct
{
	/* SPR_17664_start*/
    U8                num_peer_enb;     /*^ M,0,H,0,MAX_PEER_ENB ^*/
	/* SPR_17664_end*/
    x2ap_oam_gb_enb_id_status_params_t staus_params[MAX_PEER_ENB];/*^ M,0,OCTET_STRING,VARIABLE ^*/

}x2ap_oam_gb_enb_status_params_list_info_t; 

typedef struct
{
    U8              cause;   /*^ M,0,N,0,0^*/   
    
    x2_gb_enb_id_t  global_enb_id; /*^ M,0,N,0,0^*/ /* x2ap_oam_sctp_staus_resp_cause_et */


}x2ap_oam_failed_gb_enb_info_t; 

typedef struct
{
	/* SPR_17664_start*/
    U8   count;/*^ M,0,H,1,MAX_PEER_ENB ^*/
	/* SPR_17664_end*/
    x2ap_oam_failed_gb_enb_info_t   failed_global_enb_info[MAX_PEER_ENB];/*^ M,0,OCTET_STRING,VARIABLE ^*/

}x2ap_oam_gb_enb_failed_list_t; 

/* MENB CHANGES - START */ 
typedef struct
{
    U8              cause;
    /*^ M,0,N,0,0^*/   
    /* x2ap_oam_sctp_staus_resp_cause_et */

    x2_gb_gnb_id_t  global_gnb_id;
    /*^ M,0,N,0,0^*/

}x2ap_oam_failed_gb_gnb_info_t; 

typedef struct
{
    U8   count;
    /*^ M,0,H,1,MAX_PEER_GNB ^*/

    x2ap_oam_failed_gb_gnb_info_t   failed_global_gnb_info[MAX_PEER_GNB];
    /*^ M,0,OCTET_STRING,VARIABLE ^*/

}x2ap_oam_gb_gnb_failed_list_t; 

typedef struct
{
    x2_gb_gnb_id_t  global_gnb_id;
    /*^ M,0,N,0,0^*/

    U32             status;
    /*^ M,0,H,0,9 ^*/
    /* x2ap_oam_sctp_status_et */
    
    U32             in_streams;
    /*^ M, 0, N, 0, 0 ^*/
    
    U32             out_streams;
    /*^ M, 0, N, 0, 0 ^*/
    
    U32             local_port;
    /*^ M, 0, N, 0, 0 ^*/

    rrc_timeval_t    start_time;
    /*^ M, 0, N, 0, 0 ^*/ 
    
    U8              primary_peer_addr[MAX_IP_ADDRESS_LENGTH];
    /*^ M,0,OCTET_STRING,FIXED ^*/ 
}x2ap_oam_gb_gnb_id_status_params_t;

typedef struct
{
    U8                num_peer_gnb;
    /*^ M,0,H,0,MAX_PEER_GNB ^*/

    x2ap_oam_gb_gnb_id_status_params_t staus_params[MAX_PEER_GNB];
    /*^ M,0,OCTET_STRING,VARIABLE ^*/

}x2ap_oam_gb_gnb_status_params_list_info_t; 
/* MENB CHANGES - END */
 
#define X2AP_OAM_GLOBAL_ENB_STATUS_PARAMS_LIST_PRESENT   0x01 
#define X2AP_OAM_GLOBAL_ENB_FAILED_LIST_PRESENT          0x02

/* MENB CHANGES - START */ 
#define X2AP_OAM_GLOBAL_GNB_STATUS_PARAMS_LIST_PRESENT   0x04
#define X2AP_OAM_GLOBAL_GNB_FAILED_LIST_PRESENT          0x08
/* MENB CHANGES - END */ 
typedef struct
{
    rrc_bitmask_t     bitmask;   /*^ BITMASK ^*/
    
    U8                response;  /*^ M, 0, H, 0, 2 ^*/  /* rrc_return_et */
    
    x2ap_oam_gb_enb_status_params_list_info_t  x2ap_oam_gb_enb_status_params_list; 
    /*^ O, X2AP_OAM_GLOBAL_ENB_STATUS_PARAMS_LIST_PRESENT, N, 0, 0 ^*/

    x2ap_oam_gb_enb_failed_list_t              gb_enb_failed_list;
    /*^ O, X2AP_OAM_GLOBAL_ENB_FAILED_LIST_PRESENT, N, 0, 0 ^*/

    /* MENB CHANGES - START */ 
    x2ap_oam_gb_gnb_status_params_list_info_t  x2ap_oam_gb_gnb_status_params_list; 
    /*^ O, X2AP_OAM_GLOBAL_GNB_STATUS_PARAMS_LIST_PRESENT, N, 0, 0 ^*/

    x2ap_oam_gb_gnb_failed_list_t              gb_gnb_failed_list;
    /*^ O, X2AP_OAM_GLOBAL_GNB_FAILED_LIST_PRESENT, N, 0, 0 ^*/
    /* MENB CHANGES - END */ 

}x2ap_oam_get_sctp_status_resp_t; /*^ API, X2AP_OAM_GET_SCTP_STATUS_RESP ^*/
/*sctp parameters stop*/




/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/


#endif /*_X2AP_OAM_INFT_H_*/
