
/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rrc_s1apOam_intf.h,v 1.3 2010/03/24 09:50:40 gur18569 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the declarations of data types for
 *                     S1AP-OAM interface file.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rrc_s1apOam_intf.h,v $
 * Revision 1.3  2010/03/24 09:50:40  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2.2.3  2010/03/05 08:27:46  gur18569
 * removed guard timer
 *
 * Revision 1.2.2.2  2010/02/02 13:18:16  ukr15916
 * tabs->spaces expanded
 *
 * Revision 1.2.2.1  2010/01/25 09:04:50  gur18569
 * added reset_timer field
 *
 * Revision 1.2  2010/01/04 16:10:03  ukr15916
 * no message
 *
 * Revision 1.1.2.6  2009/11/26 18:33:42  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.2.5  2009/11/19 05:34:37  gur20470
 * Modified MAX_IP_ADDRESS_LENGTH
 *
 * Revision 1.1.2.4  2009/11/11 04:36:39  gur18569
 * added max_reset_retries
 *
 * Revision 1.1.2.3  2009/10/29 13:19:17  gur18569
 * fixed errors
 *
 * Revision 1.1.2.2  2009/10/28 07:26:59  gur18569
 * changed TAC_OCTET_SIZE from 6 to 2
 *
 * Revision 1.1.2.1  2009/10/23 16:15:31  gur18569
 * Initial version
 *
 * Revision 1.1.2.1  2009/10/15 08:42:38  gur18569
 * Initial version
 *
 *
 *
 ****************************************************************************/

#ifndef _S1AP_OAM_INFT_H_
#define _S1AP_OAM_INFT_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "rrc_defines.h"
#include "s1ap_api.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/* Hash Defined*/
#define MAX_NUM_MME                  16
#define MAX_NUM_DYNAMIC_MME             5
#define MAX_NUM_IP_ADDR                     3
#define MAX_NUM_TAC                     256
#define MAX_CSG_ID_LIST                 256
#define MAX_PLMN_ID_BYTES               3
#define MAX_BC_PLMN                     6
#define TAC_OCTET_SIZE                  2
/*#define MACRO_ENB_ID_OCTET_SIZE         3
#define HOME_ENB_ID_OCTET_SIZE          4*/
#define CSG_ID_OCTET_SIZE               4
#define MAX_ENB_NAME_STR_SIZE           150

#define RRC_S1APOAM_MODULE_ID    RRC_OAM_MODULE_ID
/****************************************************************************
 * Exported Types
 ****************************************************************************/

#pragma pack(push, 1)


/******************************************************************************
  S1AP_OAM_PROVISION_REQ
 ******************************************************************************/
typedef enum
{
        MME_REL8,
        MME_REL9,
        MME_REL10
}s1ap_oam_rel_of_mme_et;

/* IPV6 Start */
#define ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT    0x01
#define ENB_COMM_INFO_IPV6_ADDR_PRESENT        0x02
/*SPR 20633 Fix START*/
#define ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT    0x04
#define ENB_COMM_INFO_IPV4_ADDR_PRESENT        0x08

/* IPV6 Stop */
typedef struct
{
    /* IPV6 Start */
    U16           bitmask;     /*^ BITMASK ^*/
    /* IPV6 Stop */
    /* number of ip addr */
	/* SPR_17664_start*/
    U8           num_ip_addr; /*^ O,ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT,B,1,MAX_NUM_IP_ADDR ^*/
	/* SPR_17664_end*/
    /* ip addresses of MMEs */
    ip_addr_t ip_addr[MAX_NUM_IP_ADDR];  /*^ O,ENB_COMM_INFO_IPV4_ADDR_PRESENT,OCTET_STRING,VARIABLE ^*/

    /* port of MME with which SCTP association needs to be initiated */
    U16       port;  /*^ M,0,N,0,0 ^*/

/*SPR 20633 Fix End*/
    /* number of ipv6 addr */

    U8           num_ipv6_addr; 
	/* SPR_17664_start*/
    /*^ O,ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT,B,1,MAX_NUM_IP_ADDR ^*/
	/* SPR_17664_end*/
    /* IPV6 addresses of eNB */

    ipv6_addr_t ipv6_addr[MAX_NUM_IP_ADDR];
    /*^ O,ENB_COMM_INFO_IPV6_ADDR_PRESENT,OCTET_STRING,VARIABLE ^*/
    /* IPV6 Stop */
}enb_comm_info_t;


#define S1AP_OAM_MACRO_ENB_ID_PRESENT        0x01
#define S1AP_OAM_HOME_ENB_ID_PRESENT         0x02

typedef struct
{
    U8 presence_bitmask;    /*^ BITMASK ^*/

    macro_enb_id_t  macro_enb_id; /*^ O,1,N,0,0 ^*/

    home_enb_id_t   home_enb_id; /*^ O,2,N,0,0 ^*/
}enb_id_info_t;

typedef struct
{
    /*PLMN id */
    s1ap_plmn_identity_t plmn_identity; /*^ M,0,N,0,0 ^*/

    /* eNB id info */
    enb_id_info_t       enb_id; /*^ M,0,N,0,0 ^*/
}gb_enb_id_t;


typedef struct
{
    /* no of broadcast PLMN */

	/* SPR_17664_start*/
    U8           num_bplmn; /*^ M,0,B,1,MAX_BC_PLMN ^*/
	/* SPR_17664_end*/
    s1ap_plmn_identity_t plmn_identity[MAX_BC_PLMN]; /*^ M,0,OCTET_STRING,VARIABLE ^*/
}bc_plmn_list_t;

/* SPR 21660 changes start */
#define S1AP_OAM_NB_RAT_TYPE_PRESENT        0x01
/* SPR 21660 changes end */
typedef struct
{
/* SPR 21660 changes start */
    U8           bitmask;     /*^ BITMASK ^*/
/* SPR 21660 changes end */

    U8      tac[TAC_OCTET_SIZE]; /*^ M,0,OCTET_STRING,FIXED ^*/

    /* broadcast PLMN list info*/
    bc_plmn_list_t  bc_plmn_list; /*^ M,0,N,0,0 ^*/
}supp_ta_t;

typedef struct
{
    /* num of TAI supported */

	/* SPR_17664_start*/
    U16    num_supported_tais; /*^ M,0,B,1,MAX_NUM_TAC ^*/
	/* SPR_17664_end*/
    supp_ta_t supp_tais[MAX_NUM_TAC]; /*^ M,0,OCTET_STRING,VARIABLE ^*/

}supp_ta_list_t;

typedef struct
{
    U8         csg_id[CSG_ID_OCTET_SIZE];   /*^ M,0,OCTET_STRING,FIXED ^*/
}csg_id_info_t;

typedef struct
{
    /* number of CSG Ids */

	/* SPR_17664_start*/
    U16   num_csg_ids; /*^ M,0,B,1,MAX_CSG_ID_LIST ^*/
	/* SPR_17664_end*/
    csg_id_info_t   csg_ids[MAX_CSG_ID_LIST]; /*^ M,0,OCTET_STRING,VARIABLE ^*/
}csg_id_list_t;

#define S1AP_OAM_ENB_NAME_PRESENT        0x01
#define S1AP_OAM_CSG_ID_LIST_PRESENT     0x02

typedef struct
{
    U16           bitmask;     /*^ BITMASK ^*/

    /* global enb id info */
    gb_enb_id_t     gb_enb_id; /*^ M,0,N,0,0 ^*/

    U8          enb_name[MAX_ENB_NAME_STR_SIZE]; /*^ O,1,OCTET_STRING,FIXED ^*/

    /* supported TAI info */
    supp_ta_list_t       supp_ta_list; /*^ M,0,N,0,0 ^*/

    /* CSG list info */
    csg_id_list_t      csg_id_list; 
    /*^ O, S1AP_OAM_CSG_ID_LIST_PRESENT, N, 0, 0 ^*/

    U32          default_paging_drx;    /*^ M,0,H,0,3 ^*/

} s1_setup_req_t;

/* AK Start */
typedef struct
{
/* SPR-18445 start */
    U8 num_mme_id;
    /*^ M,0,B,1,MAX_NUM_MME ^*/
/* SPR-18445 stop */
    

    U8 mme_id[MAX_NUM_MME]; 
    /*^ M,0,OCTET_STRING,VARIABLE ^*/

} s1ap_oam_mme_id_list_info_t;
/* AK Stop */

/* AK Start */
#define S1AP_OAM_ENB_CONFIG_UPDATE_ENB_NAME_PRESENT               0x01
#define S1AP_OAM_ENB_CONFIG_UPDATE_TA_LIST_PRESENT                0x02
#define S1AP_OAM_ENB_CONFIG_UPDATE_CSG_ID_LIST_PRESENT            0x04    
#define S1AP_OAM_ENB_CONFIG_UPDATE_PAGING_DRX_PRESENT             0x08
#define S1AP_OAM_ENB_CONFIG_UPDATE_MME_LIST_PRESENT               0x10

/* AK Stop */

typedef struct
{
    U16           bitmask;     /*^ BITMASK ^*/

    U8          enb_name[MAX_ENB_NAME_STR_SIZE]; /*^ O,S1AP_OAM_ENB_CONFIG_UPDATE_ENB_NAME_PRESENT,OCTET_STRING,FIXED ^*/

    /* supported TAI info */
    supp_ta_list_t       supp_ta_list; /*^ O, S1AP_OAM_ENB_CONFIG_UPDATE_TA_LIST_PRESENT,N,0,0 ^*/

    /* CSG list info */
    csg_id_list_t      csg_id_list; /*^ O, S1AP_OAM_ENB_CONFIG_UPDATE_CSG_ID_LIST_PRESENT,N,0,0 ^*/

    U32          default_paging_drx; /*^O, S1AP_OAM_ENB_CONFIG_UPDATE_PAGING_DRX_PRESENT,H,0,3^*/

    /* AK Start */
    s1ap_oam_mme_id_list_info_t    s1ap_oam_mme_id_list;
    /*^ O, S1AP_OAM_ENB_CONFIG_UPDATE_MME_LIST_PRESENT, N, 0, 0 ^*/

    /* AK Stop */

} s1ap_oam_enb_config_update_t; /*^ API, S1AP_OAM_ENB_CONFIG_UPDATE ^*/

typedef enum
{
    OAM_ENB_CONFIG_UPDATE_FAILURE,
    OAM_ENB_CONFIG_UPDATE_SUCCESS,
    OAM_ENB_CONFIG_MSG_DECODE_FAILURE,
    OAM_ENB_CONFIG_UPDATE_INTERNAL_ERROR,
    OAM_ENB_CONFIG_UPDATE_INVALID_RESP,
    OAM_ENB_CONFIG_UPDATE_FAILURE_TIMER_EXPIRY,
    OAM_ENB_CONFIG_UPDATE_FAILURE_INVALID_MME_ID
} s1ap_oam_enb_config_update_resp_cause_et;

typedef struct
{
    U8 mme_id; /*^ M,0,H,0,15 ^*/

    U8 response; /*^ M,0,H,0,4 ^*/
                 /* s1ap_oam_enb_config_update_resp_cause_et */

} s1ap_oam_mme_conn_info_t;

typedef struct
{
	/* SPR_17664_start*/
    U8 mme_count; /*^ M,0,N,1,MAX_NUM_MME ^*/
	/* SPR_17664_end*/
    s1ap_oam_mme_conn_info_t
        oam_mme_conn_info_list[MAX_NUM_MME]; /*^ M,0,OCTET_STRING,FIXED ^*/
} s1ap_oam_mme_conn_status_response_t; /*^ API, S1AP_OAM_MME_CONN_INFO_RESPONSE ^*/


typedef struct
{
    U8 mme_id; /*^ M,0,H,0,15 ^*/

    U8 response; /*^ M,0,H,0,6 ^*/
                 /* s1ap_oam_enb_config_update_resp_cause_et */

} s1ap_oam_enb_config_update_response_t; /*^ API, S1AP_OAM_ENB_CONFIG_UPDATE_RESPONSE ^*/

typedef struct
{
/* SPR-18445 start */
    U8 mme_count;                       /*^ M,0,B,1,MAX_NUM_MME ^*/
/* SPR-18445 stop */

    U8 mme_id[MAX_NUM_MME];             /*^ M,0,OCTET_STRING,FIXED ^*/

} s1ap_oam_close_mme_conn_t;            /*^ API, S1AP_OAM_CLOSE_MME_CONN ^*/

typedef struct
{
/* SPR-18445 start */
    U8 mme_count;                       /*^ M,0,B,1,MAX_NUM_MME ^*/
/* SPR-18445 stop */

    U8 mme_id[MAX_NUM_MME];             /*^ M,0,OCTET_STRING,FIXED ^*/

} s1ap_oam_reestab_mme_conn_t;            /*^ API, S1AP_OAM_REESTAB_MME_CONN ^*/

#define S1AP_OAM_MME_SELECTION_ALGO_PRESENT        0x01
/* SPR_5240 Start */
#define S1AP_OAM_S1_CONN_RECOVERY_DURATION_PRESENT 0x02
/* SPR_5240 Stop */
/*sctp parameters start*/
#define S1AP_OAM_SCTP_CONF_INFO_PRESENT       0x04
/*sctp parameters stop */
#define S1AP_OAM_S1_HEALTH_MONITORING_TIMER_PRESENT 0x08
/* SPR 13432 Fix Start */ 
#define S1AP_OAM_MME_S1_CONN_MODE_ONE_PRESENT       0x10
/* SPR 13432 Fix Stop */ 
typedef struct
{
    /* number of MMEs supported */

	/* SPR_17664_start*/
    U8           num_mme;     /*^ M,0,B,1,MAX_NUM_MME ^*/
	/* SPR_17664_end*/
    /* Info needed to open an association with MME */
    mme_comm_info_t mme_comm_info[MAX_NUM_MME]; /*^ M,0,OCTET_STRING,VARIABLE ^*/

    /* SCTP connection info of enb */
    enb_comm_info_t enb_comm_info; /*^ M,0,N,0,0 ^*/

    /* Info needed to send an s1 setup request */
    s1_setup_req_t  s1_setup_req_parameters;  /*^ M,0,N,0,0 ^*/

    /* timer value that indicates waiting time after sending s1 setup request */
    U32          s1_config_timer;   /*^ M,0,L,50,0 ^*/

    /* timer value that indicates waiting time after sending RESET request */
    U32          reset_ep_timer;    /*^ M,0,L,50,0 ^*/

    /* no of retries to be made when s1 setup req fails */
    U8           max_s1_config_retries;    /*^ M,0,N,0,0 ^*/

    /* no of retries to be made when reset ack is not received */
    U8           max_reset_retries;    /*^ M,0,N,0,0 ^*/

    /*s1ap pws timer duration*/
    U32 s1ap_pws_timer; /*^ M,0,L,50,0 ^*/

    /*s1ap kill timer duration*/
    U32 s1ap_kill_timer; /*^ M,0,L,50,0 ^*/
    
    U16                   bitmask; /*^ BITMASK ^*/       
    
    /* Algorithm used to select the MME */
    U8                    mme_selection_algo; 
    /*^ O, S1AP_OAM_MME_SELECTION_ALGO_PRESENT, H, 0, 1 ^*/ /* s1ap_oam_mme_selection_algo_et */
    
/* SPR_5240 Start */
    U32                   s1_conn_recovery_duration; 
    /*^ O, S1AP_OAM_S1_CONN_RECOVERY_DURATION_PRESENT, L, 50, 0 ^*/ 
/* SPR_5240 Stop */

/*sctp parameters start*/
    /*sctp communcation parameters for all peers/assocaiations*/
    s1ap_sctp_conf_info_t  s1ap_sctp_conf_param;
    /*^ O, S1AP_OAM_SCTP_CONF_INFO_PRESENT, N, 0, 0 ^*/
/*sctp parameters stop */
    U16                   s1ap_health_mon_time_int;
/*^ O, S1AP_OAM_S1_HEALTH_MONITORING_TIMER_PRESENT, N, 0, 0  ^*/
    U8                          log_level;  /*^ M, 0, H, 0, 6 ^*/

}s1apInfo_t;

typedef struct
{
    s1apInfo_t            s1apInfo;       /*^ M, 0, N, 0, 0 ^*/
}s1ap_oam_provision_req_t;              /*^ API, S1AP_OAM_PROVISION_REQ ^*/

/******************************************************************************
  S1AP_OAM_PROVISION_RESP
 ******************************************************************************/
typedef struct
{
    U8            response;       /*^ M, 0, H, 1, 1 ^*/  /* rrc_return_et */
}s1ap_oam_provision_resp_t; /*^ API, S1AP_OAM_PROVISION_RESP ^*/

/******************************************************************************
*   S1AP_OAM_GET_LOG_LEVEL_RESP
******************************************************************************/
typedef struct _s1ap_oam_get_log_level_resp_t
{
#define S1AP_OAM_LOG_LEVEL_PRESENT 0x01
    U16                     bitmask;    /*^ BITMASK ^*/
    U8                  log_on_off;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_oam_log_on_off_et */
    U8                  log_level;
/*^ O, S1AP_OAM_LOG_LEVEL_PRESENT, H, 0, 6 ^*/ /* rrc_oam_log_level_et */

} s1ap_oam_get_log_level_resp_t; /*^ API, S1AP_OAM_GET_LOG_LEVEL_RESP ^*/

/******************************************************************************
*   S1AP_OAM_SET_LOG_LEVEL_REQ
******************************************************************************/
typedef struct _s1ap_oam_set_log_level_req_t
{
    U8                  log_level;
/*^ M, 0, H, 0, 6 ^*/ /* rrc_oam_log_level_et */

} s1ap_oam_set_log_level_req_t; /*^ API, S1AP_OAM_SET_LOG_LEVEL_REQ ^*/

/******************************************************************************
*   S1AP_OAM_SET_LOG_LEVEL_RESP
******************************************************************************/
typedef struct _s1ap_oam_set_log_level_resp_t
{
    U8                  response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

} s1ap_oam_set_log_level_resp_t; /*^ API, S1AP_OAM_SET_LOG_LEVEL_RESP ^*/

/******************************************************************************
 * *   S1AP_OAM_LOG_ENABLE_REQ
 * ******************************************************************************/
typedef struct _s1ap_oam_log_enable_req_t
{
        U8                  log_on_off;
        /*^ M, 0, H, 0, 1 ^*/ /* rrc_oam_log_on_off_et */

} s1ap_oam_log_enable_req_t; /*^ API, S1AP_OAM_LOG_ENABLE_REQ ^*/

/******************************************************************************
*   S1AP_OAM_LOG_ENABLE_RESP
******************************************************************************/
typedef struct _s1ap_oam_log_enable_resp_t
{
    U8                  response;
/*^ M, 0, H, 0, 1 ^*/ /* rrc_return_et */

} s1ap_oam_log_enable_resp_t; /*^ API, S1AP_OAM_LOG_ENABLE_RESP ^*/

/******************************************************************************
  S1AP_OAM_RESET_RESP
 ******************************************************************************/
typedef struct
{
    U8            response;       /*^ M, 0, H, 1, 1 ^*/  /* rrc_return_et */
}s1ap_oam_reset_resp_t; /*^ API, S1AP_OAM_RESET_RESP ^*/

/******************************************************************************
  S1AP_OAM_S1AP_LINK_STATUS_IND
 ******************************************************************************/
typedef enum
{
    OAM_S1AP_LINK_UP,
    OAM_S1AP_LINK_DOWN
}s1ap_oam_s1ap_link_status_et;

/* SPR 13432 Fix Start */
#define S1AP_OAM_LINK_STATUS_IND_IPv4_ADDRESS_PRESENT   0x01
#define S1AP_OAM_LINK_STATUS_IND_IPv6_ADDRESS_PRESENT   0x02
#define S1AP_OAM_LINK_STATUS_IND_PORT_PRESENT           0x04
/* SPR 13432 Fix Stop */
typedef struct
{
    /* SPR 13432 Fix Start */
    rrc_bitmask_t   bitmask;   /*^ BITMASK ^*/
    /* SPR 13432 Fix Stop */

    /* Index of MME */
    U8 mme_id;      /*^ M, 0, H, 0, 15 ^*/
    
    /* status of S1AP LINK */
    U8  status; /*^ M,0,H,0,1 ^*/ /* s1ap_oam_s1ap_link_status_et */

    /* SPR 13432 Fix Start */
    /* ipv4 address of MME with which SCTP association is established/terminated */
    U8  ip_addr[MAX_IP_ADDRESS_LENGTH];
    /*^ O, S1AP_OAM_LINK_STATUS_IND_IPv4_ADDRESS_PRESENT, OCTET_STRING, FIXED ^*/

    /* ipv6 address of MME with which SCTP association is established/terminated */
    U8  ipv6_addr[MAX_IPV6_ADDRESS_LENGTH];
    /*^ O, S1AP_OAM_LINK_STATUS_IND_IPv6_ADDRESS_PRESENT, OCTET_STRING, FIXED ^*/

    /* port of MME with which SCTP association is established/terminated */
    U16 port;  
    /*^ O, S1AP_OAM_LINK_STATUS_IND_PORT_PRESENT, N, 0, 0 ^*/
    /* SPR 13432 Fix Stop */

}s1ap_oam_s1ap_link_status_ind_t; /*^ API, S1AP_OAM_S1AP_LINK_STATUS_IND ^*/
                           

/******************************************************************************
  S1AP_OAM_STATS_RESP
 ******************************************************************************/

typedef struct
{
    /* Index of MME */
    U8 mme_id;      /*^ M, 0, H, 0, 15 ^*/

    /* Number of active UE associated logical S1
       connections handled by a MME */
    U16 count_of_ue;   /*^ M, 0, N, 0, 0 ^*/
}active_mme_ctx_t;

typedef struct
{
    /* Number of active MMEs */
	/* SPR_17664_start*/
    U8 active_mme_count;      /*^ M, 0, N, 1, MAX_NUM_MME^*/
	/* SPR_17664_end*/
    /* List of Stats information per MME*/
    active_mme_ctx_t active_mme_list[MAX_NUM_MME];  /*^ M,0,OCTET_STRING,VARIABLE ^*/
}s1ap_oam_status_resp_t; /*^ API, S1AP_OAM_STATUS_RESP ^*//*CSR 00058587 Fix*/


/******************************************************************************
 *                         S1AP_OAM_MME_INFO 
 *****************************************************************************/                          

typedef struct
{
    rrc_bitmask_t   bitmask;   /*^ BITMASK ^*/
    
    U8              mme_id; /*^ M,0,N,0,0 ^*/
    
    U8              ip_addr[MAX_IP_ADDRESS_LENGTH]; /*^ M,0,OCTET_STRING,FIXED ^*/
    
    U16             port; /*^ M,0,N,0,0 ^*/
    
    s1ap_mme_info_t      mme_info; /*^ M, 0, N, 0, 0 ^*/
}s1ap_oam_mme_info_t; /*^ API, S1AP_OAM_MME_INFO ^*/

/*sctp parameters start*/

/******************************************************************************
 *                         S1AP_OAM_GET_SCTP_STATUS_REQ 
 *****************************************************************************/                          

#define S1AP_OAM_MME_STATUS_LIST_PRESENT    0x01
typedef struct
{
    rrc_bitmask_t   bitmask;   /*^ BITMASK ^*/
    
    s1ap_oam_mme_id_list_info_t s1ap_oam_mme_status_list; 
    /*^ O, S1AP_OAM_MME_STATUS_LIST_PRESENT, N, 0, 0 ^*/
    
}s1ap_oam_get_sctp_status_req_t; /*^ API, S1AP_OAM_GET_SCTP_STATUS_REQ ^*/

/******************************************************************************
 *                         S1AP_OAM_GET_SCTP_STATUS_RESP 
 *****************************************************************************/                          
typedef enum
{
    S1AP_OAM_MME_INVALID,
    S1AP_OAM_MME_NOT_ACTIVE
}s1ap_oam_sctp_staus_resp_cause_et;

typedef struct
{
    U8          mme_id; /*^ M,0,N,0,0 ^*/

    U32         status;  /*^ M,0,H,0,9 ^*/ /* s1ap_oam_sctp_status_et */
    
    U32         in_streams; /*^ M, 0, N, 0, 0 ^*/
    
    U32         out_streams; /*^ M, 0, N, 0, 0 ^*/
    
    U32         local_port; /*^ M, 0, N, 0, 0 ^*/

    rrc_timeval_t  start_time; /*^ M, 0, N, 0, 0 ^*/
    
    U8          primary_peer_addr[MAX_IP_ADDRESS_LENGTH]; /*^ M,0,OCTET_STRING,FIXED ^*/ 
}s1ap_oam_mme_id_status_params_t;

typedef struct
{

	/* SPR_17664_start*/
    U8                num_mme_id;/*^ M,0,B,1,MAX_NUM_MME ^*/
	/* SPR_17664_end*/
    s1ap_oam_mme_id_status_params_t staus_params[MAX_NUM_MME];/*^ M,0,OCTET_STRING,VARIABLE ^*/

}s1ap_oam_mme_status_params_list_info_t; 

typedef struct
{
    U8          mme_id; /*^ M,0,N,0,0 ^*/

    U8          cause; /*^ M,0,N,0,1 ^*/   /* s1ap_oam_sctp_staus_resp_cause_et */

}s1ap_oam_failed_mme_info_t; 

typedef struct
{

	/* SPR_17664_start*/
    U8   count;/*^ M,0,B,1,MAX_NUM_MME ^*/
	/* SPR_17664_end*/
    s1ap_oam_failed_mme_info_t   failed_mme_info[MAX_NUM_MME];/*^ M,0,OCTET_STRING,VARIABLE ^*/

}s1ap_oam_mme_failed_list_t; 

#define S1AP_OAM_MME_STATUS_PARAMS_LIST_PRESENT  0x01 
#define S1AP_OAM_MME_FAILED_LIST_PRESENT         0x02 
typedef struct
{
    rrc_bitmask_t     bitmask;   /*^ BITMASK ^*/
    
    U8                response;  /*^ M, 0, H, 0, 2 ^*/  /* rrc_return_et */
    
    s1ap_oam_mme_status_params_list_info_t  s1ap_oam_mme_status_params_list; 
    /*^ O, S1AP_OAM_MME_STATUS_PARAMS_LIST_PRESENT, N, 0, 0 ^*/

    s1ap_oam_mme_failed_list_t              mme_failed_list;
    /*^ O, S1AP_OAM_MME_FAILED_LIST_PRESENT, N, 0, 0 ^*/

}s1ap_oam_get_sctp_status_resp_t; /*^ API, S1AP_OAM_GET_SCTP_STATUS_RESP ^*/
/*sctp parameters stop*/

#ifdef LINUX_PC_TEST
typedef struct
{	
	U8 current_mme_state;
	U8 mme_id;
	U16 ue_count;
}pctest_mme_info_t;

typedef struct
{
	U8  mme_count;
	pctest_mme_info_t mme_info[MAX_NUM_MME];
	U8  cell_count;
	U8 s1ap_current_state;
	U8 current_enb_mgr_state;
}s1ap_gb_status_resp_t;
#endif
#pragma pack(pop)

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/




#endif /* _S1AP_OAM_INFT_H_ */
