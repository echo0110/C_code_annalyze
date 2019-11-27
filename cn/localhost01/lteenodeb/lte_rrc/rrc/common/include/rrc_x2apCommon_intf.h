/******************************************************************************
 * *
 * *   FILE NAME   : rrc_x2apCommon_intf.h
 * *
 * *   DESCRIPTION : This file contains basic X2AP Common interface definitions.
 * *
 * *   DATE            AUTHOR         REFERENCE       REASON
 * *   24-SEPT-2012    Rajneesh Walia  ---------       Initial
 * *
 * *   Copyright (c) 2012, Aricent Inc. All Rights Reserved
 * *
 * ******************************************************************************/

#ifndef RRC_X2APCOMMON_INTF_H_
#define RRC_X2APCOMMON_INTF_H_

#include "rrc_defines.h"
#include "x2ap_api.h"

/* spr_12819_fix_start */
#define X2AP_CAUSE_INTERNAL 6 
#define ENB_CONFIG_UPDATE_TIMER_EXPIRY 0
/* spr_12819_fix_stop */
typedef enum
{
    HO_TOO_EARLY,
    HO_TO_WRONG_CELL
}x2ap_ho_report_type_et;

typedef struct
{
    x2ap_ecgi_t      ecgi;   /*^ M,0,N,0,0 ^*/
    U32               pci;    /*^ M,0,H,0,503 ^*/
    x2ap_earfcn_t    earfcn; /*^ M,0,N,0,0 ^*/
}x2ap_nbour_info_t;

typedef struct
{

	/* SPR_17664_start*/
    U16	num_neighbour_cell; /*^ M,0,H,0,MAX_NEIGHBOURS ^*/
	/* SPR_17664_end*/
	
 	x2ap_nbour_info_t neighbour_info_arr[MAX_NEIGHBOURS]; 	/*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_nbour_cell_info_t;

typedef struct
{
	x2ap_served_cell_info_t cell_info;				/*^ M,0,N,0,0 ^*/
	x2ap_nbour_cell_info_t nbour_cell_info;		/*^ M,0,N,0,0 ^*/
}x2ap_served_cell_info_arr_t;

typedef struct
{

	/* SPR_17664_start*/
	U16 num_served_cell; /*^ M,0,B,1,MAX_SERVED_CELLS ^*/
	/* SPR_17664_end*/
	
    x2ap_served_cell_info_arr_t cell_list[MAX_SERVED_CELLS];		/*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_served_cells_to_add_list_t;

typedef struct
{
#define X2AP_DEACTIVATION_IND_PRESENT 0x01
	U16 bitmask;												/*^ BITMASK ^*/
	U8 old_cell_id;												/*^ M,0,N,0,0^*/ /* This is  Cellid in case of X2AP_ENB_CONFIG_UPDATE_IND otherwise it is use as Cell Index *//*SPR 21554 ++*/ 
	x2ap_served_cell_info_arr_t served_cell_info;	/*^ M,0,N,0,0 ^*/
	U8 deactivation_ind;									/*^ O,X2AP_DEACTIVATION_IND_PRESENT,H,0,1 ^*/
}x2ap_served_cells_to_modify_info_t;

typedef struct
{
		
	/* SPR_17664_start*/
	U16 num_served_cell; /*^ M,0,B,1,MAX_SERVED_CELLS ^*/
	
	/* SPR_17664_end*/
x2ap_served_cells_to_modify_info_t cell_list[MAX_SERVED_CELLS];		/*^ M,0,OCTET_STRING,VARIABLE^*/
}x2ap_served_cells_to_modify_list_t;

typedef struct
{

	/* SPR_17664_start*/
	U16 num_served_cell; 								/*^ M,0,B,1,MAX_SERVED_CELLS ^*/
	
	/* SPR_17664_end*/
U8 cell_id_list[MAX_SERVED_CELLS];		/*^ M,0,OCTET_STRING,VARIABLE^*/
}x2ap_served_cells_to_delete_list_t;

typedef struct
{
	
	/* SPR_17664_start*/
	U8 num_gu_group_id; /*^ M,0,B,1,MAX_ENB_POOLS ^*/
	
	/* SPR_17664_end*/
x2ap_gu_group_id_t gu_id_list[MAX_ENB_POOLS];	/*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_gu_group_id_list_t;

typedef struct
{
    x2ap_ecgi_t      ecgi;   /*^ M,0,N,0,0 ^*/
    x2_gb_enb_id_t   enb_id; /*^ M,0,N,0,0 ^*/
}x2ap_neigh_ecgi_enb_id_map_info_t;

typedef struct
{

	/* SPR_17664_start*/
    U16  neigh_info_count; /*^ M,0,B,1,MAX_NEIGHBOURS ^*/
    
	/* SPR_17664_end*/
x2ap_neigh_ecgi_enb_id_map_info_t neigh_ecgi_enb_id_info[MAX_NEIGHBOURS]; 
    /*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_neigh_ecgi_enb_id_map_list_t;

typedef struct
{
#define X2AP_SERVED_CELLS_TO_ADD_LIST_PRESENT			0x01
#define X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT		0x02
#define X2AP_SERVED_CELLS_TO_DELETE_LIST_PRESENT		0x04
#define X2AP_GU_GROUP_ID_TO_ADD_LIST_PRESENT		    0x08
#define X2AP_GU_GROUP_ID_TO_DELETE_LIST_PRESENT		    0x10
#define X2AP_NEIGH_ECGI_ENB_ID_MAPPING_PRESENT          0x20
	U16 bitmask; /*^ BITMASK ^*/

	/* SPR_17664_start*/
	U8 num_peer_enb; /*^ M,0,H,0, MAX_PEER_ENB ^*/
	/* SPR_17664_end*/
	
	x2_gb_enb_id_t enb_info[MAX_PEER_ENB]; 			/*^ M,0,OCTET_STRING,VARIABLE ^*/
	x2ap_served_cells_to_add_list_t add_served_cell;		/*^ O,X2AP_SERVED_CELLS_TO_ADD_LIST_PRESENT,N,0,0 ^*/
	x2ap_served_cells_to_modify_list_t mod_served_cell;	/*^ O,X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT,N,0,0 ^*/
	x2ap_served_cells_to_delete_list_t del_served_cell;	/*^ O,X2AP_SERVED_CELLS_TO_DELETE_LIST_PRESENT,N,0,0 ^*/
	x2ap_gu_group_id_list_t add_gu_id;							/*^ O,X2AP_GU_GROUP_ID_TO_ADD_LIST_PRESENT,N,0,0 ^*/
	x2ap_gu_group_id_list_t delete_gu_id;						/*^ O,X2AP_GU_GROUP_ID_TO_DELETE_LIST_PRESENT,N,0,0 ^*/
    x2ap_neigh_ecgi_enb_id_map_list_t  neigh_ecgi_enb_id_map;   /*^ O, X2AP_NEIGH_ECGI_ENB_ID_MAPPING_PRESENT,N,0,0 ^*/
}x2ap_enb_config_update_req_t; /*^ API, X2AP_ENB_CONFIG_UPDATE_REQ ^*/

typedef struct
{
#define X2AP_CAUSE_PRESENT 			0x01
	U16 bitmask;									/*^ BITMASK ^*/
	x2_gb_enb_id_t enb1_gb_id;			/*^ M,0,N,0,0 ^*/
	x2_gb_enb_id_t enb2_gb_id;			/*^ M,0,N,0,0 ^*/
	U8 response;									/*^ M,0,H,0,1 ^*/
	x2ap_cause_t cause;							/*^ O,X2AP_CAUSE_PRESENT,N,0,0 ^*/
}x2ap_enb_config_update_resp_t; /*^ API, X2AP_ENB_CONFIG_UPDATE_RES ^*/

typedef struct
{
	U16 bitmask; /*^ BITMASK ^*/
	x2_gb_enb_id_t enb1_gb_id;			/*^ M,0,N,0,0 ^*/
	x2_gb_enb_id_t enb2_gb_id;			/*^ M,0,N,0,0 ^*/
	x2ap_served_cells_to_add_list_t add_served_cell;		/*^ O,X2AP_SERVED_CELLS_TO_ADD_LIST_PRESENT,N,0,0 ^*/
	x2ap_served_cells_to_modify_list_t mod_served_cell;	/*^ O,X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT,N,0,0 ^*/
	x2ap_served_cells_to_delete_list_t del_served_cell;	/*^ O,X2AP_SERVED_CELLS_TO_DELETE_LIST_PRESENT,N,0,0 ^*/
	x2ap_gu_group_id_list_t add_gu_id;							/*^ O,X2AP_GU_GROUP_ID_TO_ADD_LIST_PRESENT,N,0,0 ^*/
	x2ap_gu_group_id_list_t delete_gu_id;						/*^ O,X2AP_GU_GROUP_ID_TO_DELETE_LIST_PRESENT,N,0,0 ^*/
}x2ap_enb_config_update_ind_t; /*^ API, X2AP_ENB_CONFIG_UPDATE_IND ^*/

typedef struct
{
	U16	bitmask;							/*^ BITMASK ^*/
	x2_gb_enb_id_t gb_enb_id;	/*^ M,0,N,0,0 ^*/
	x2ap_cause_t cause;					/*^ M,0,N,0,0 ^*/
}x2ap_reset_req_t; /*^ API, X2AP_RESET_REQ ^*/

typedef struct
{
	U16 bitmask;							/*^ BITMASK ^*/
	x2_gb_enb_id_t enb1_gb_id;	/*^ M,0,N,0,0 ^*/
	x2_gb_enb_id_t enb2_gb_id;	/*^ M,0,N,0,0 ^*/
	U8 response;							/*^ M,0,H,0,1 ^*/
}x2ap_reset_resp_t; /*^ API, X2AP_RESET_RES ^*/

typedef struct
{
	U16	bitmask;							/*^ BITMASK ^*/
	x2_gb_enb_id_t enb1_gb_id;	/*^ M,0,N,0,0 ^*/
	x2_gb_enb_id_t enb2_gb_id;	/*^ M,0,N,0,0 ^*/
	x2ap_cause_t cause;					/*^ M,0,N,0,0 ^*/
}x2ap_reset_ind_t; /*^ API, X2AP_RESET_IND ^*/


typedef struct
{
	U16 bitmask; /*^ BITMASK ^*/

	/* SPR_17664_start*/
	U8 num_peer_enb; /*^ M,0,B,1,MAX_PEER_ENB ^*/
	
	/* SPR_17664_end*/
x2_enb_comm_info_t enb_info[MAX_PEER_ENB]; /*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_add_enb_req_t; /*^ API, X2AP_ADD_ENB_REQ ^*/

typedef struct
{

	/* SPR_17664_start*/
	U8 num_peer_enb; /*^ M,0,B,1,MAX_PEER_ENB ^*/
	
	/* SPR_17664_end*/
x2_enb_comm_info_t enb_comm_info[MAX_PEER_ENB];	/*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_enb_comm_info_list_t;

typedef struct
{
#define X2AP_SUCCESS_ENB_ADD_LIST_PRESENT 0x01
#define X2AP_FAILURE_ENB_ADD_LIST_PRESENT 0x02
	U16 bitmask;											/*^ BITMASK ^*/
	x2_gb_enb_id_t gb_enb_id;					/*^ M,0,N,0,0 ^*/
	x2ap_enb_comm_info_list_t success_enb_list; 	/*^ O,X2AP_SUCCESS_ENB_ADD_LIST_PRESENT,N,0,0 ^*/
	x2ap_enb_comm_info_list_t failure_enb_list; 		/*^ O,X2AP_FAILURE_ENB_ADD_LIST_PRESENT,N,0,0 ^*/
}x2ap_add_enb_res_t; /*^ API, X2AP_ADD_ENB_RES ^*/

typedef struct
{
	U16 bitmask;							/*^ BITMASK ^*/
	x2_gb_enb_id_t gb_enb_id;	/*^ M,0,N,0,0 ^*/
}x2ap_delete_enb_req_t; /*^ API, X2AP_DEL_ENB_REQ ^*/

typedef struct
{
	U16 bitmask;							/*^ BITMASK ^*/
	x2_gb_enb_id_t enb1_gb_id;	/*^ M,0,N,0,0 ^*/
	x2_gb_enb_id_t enb2_gb_id;	/*^ M,0,N,0,0 ^*/
	U8 response;							/*^ M,0,H,0,1 ^*/
}x2ap_delete_enb_res_t; /*^ API, X2AP_DEL_ENB_RES ^*/

typedef struct
{
#define X2AP_GU_GROUP_ID_LIST_PRESENT 0x01
    U16 bitmask;																		/*^ BITMASK ^*/
	x2_gb_enb_id_t enb1_gb_id;															/*^ M,0,N,0,0 ^*/
	x2_gb_enb_id_t enb2_gb_id;															/*^ M,0,N,0,0 ^*/
	x2_enb_comm_info_t enb_comm_info;   /*^ M,0,N,0,0 ^*/
   
 	U16 num_served_cell;
		
	/* SPR_17664_start*/
		/*^ M,0,B,1,MAX_SERVED_CELLS ^*/
	
	/* SPR_17664_end*/
	x2ap_served_cell_info_arr_t served_cell_info[MAX_SERVED_CELLS];	                    /*^ M,0,OCTET_STRING,VARIABLE ^*/
	x2ap_gu_group_id_list_t gu_group_id;												/*^ O,X2AP_GU_GROUP_ID_LIST_PRESENT,N,0,0 ^*/
}x2ap_enb_link_up_ind_t; /*^ API, X2AP_ENB_LINK_UP_IND ^*/

typedef struct
{
#define X2AP_PEER_ENB_ID_INFO_PRESENT 0x01
#define X2AP_PEER_ENB_COMM_INFO_PRESENT 0x02
    U16 bitmask; /*^ BITMASK ^*/
	x2_gb_enb_id_t enb1_gb_id;	/*^ M,0,N,0,0 ^*/
	x2_gb_enb_id_t enb2_gb_id;	/*^ O,X2AP_PEER_ENB_ID_INFO_PRESENT,N,0,0 ^*/
    x2_enb_comm_info_t enb_comm_info; /*^ O,X2AP_PEER_ENB_COMM_INFO_PRESENT,N,0,0 
                                         ^*/
}x2ap_enb_link_down_ind_t; /*^ API, X2AP_ENB_LINK_DOWN_IND ^*/

typedef struct
{
	U16 bitmask; /*^ BITMASK ^*/
	x2_gb_enb_id_t enb1_gb_id;	/*^ M,0,N,0,0 ^*/
	x2_gb_enb_id_t enb2_gb_id;	/*^ M,0,N,0,0 ^*/
	U8 time_to_wait;	/*^ M,0,H,0,2 ^*/
}x2ap_enb_config_update_wait_ind_t; /*^ API, X2AP_ENB_CONFIG_UPDATE_WAIT_IND ^*/

typedef struct
{
#define X2AP_HO_REPORT_REEST_CELL_ECGI_PRESENT  0x01
    U16 bitmask;  /*^ BITMASK ^*/
    x2_gb_enb_id_t enb1_gb_id;   /*^ M,0,N,0,0 ^*/
    x2_gb_enb_id_t enb2_gb_id;   /*^ M,0,N,0,0 ^*/
    /*SPR_6460_fix Start*/
    U32 ho_report_type; /*^ M,0,H,0,1 ^*/ /* x2ap_ho_report_type_et */
    /*SPR_6460_fix Stop*/
    x2ap_cause_t ho_cause;          /*^ M,0,N,0,0 ^*/
    x2ap_ecgi_t  src_ecgi;       /*^ M,0,N,0,0 ^*/
    x2ap_ecgi_t  fail_cell_ecgi;  /*^ M,0,N,0,0 ^*/
    x2ap_ecgi_t  re_est_cell_ecgi; 
        /*^ O,X2AP_HO_REPORT_REEST_CELL_ECGI_PRESENT,N,0,0 ^*/
}x2ap_ho_report_ind_t; /*^ API, X2AP_HO_REPORT_IND ^*/


typedef struct
{
    rrc_bitmask_t               bitmask; /*^ BITMASK ^*/

	/* SPR_17664_start*/
    U16                         num_served_cell;/*^ M, 0, B, 1, MAX_SERVED_CELLS ^*/
   
	/* SPR_17664_end*/
 x2ap_ecgi_t                 served_cell_list[MAX_SERVED_CELLS];
    /*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_cell_activation_info_t;

typedef struct
{
    rrc_bitmask_t               bitmask; /*^ BITMASK ^*/
    x2_gb_enb_id_t              gb_enb_id; /*^ M,0,N,0,0 ^*/
    x2ap_cell_activation_info_t peer_cell_activation_info;/*^ M,0,N,0,0 ^*/   
}x2ap_peer_cell_activation_req_t;/*^ API, X2AP_PEER_CELL_ACTIVATION_REQ ^*/

typedef struct
{
#define X2AP_CELL_ACTIVATION_SERVED_CELL_LIST_PRESENT 0x01
#define X2AP_CELL_ACTIVATION_CAUSE_PRESENT 0x02
    rrc_bitmask_t               bitmask; /*^ BITMASK ^*/
    x2_gb_enb_id_t              enb1_gb_id; /*^ M,0,N,0,0 ^*/
    x2_gb_enb_id_t              enb2_gb_id; /*^ M,0,N,0,0 ^*/
    U8                          response;/*^ M,0,H,0,1 ^*/

	/* SPR_17664_start*/
    U16                         num_served_cell;/*^ M, 0, H, 0, MAX_SERVED_CELLS ^*/
   
	/* SPR_17664_end*/
 x2ap_ecgi_t                 served_cell_list[MAX_SERVED_CELLS];
    /*^ O,X2AP_CELL_ACTIVATION_SERVED_CELL_LIST_PRESENT,OCTET_STRING,VARIABLE ^*/
    x2ap_cause_t                cause;    
    /*^ O,X2AP_CELL_ACTIVATION_CAUSE_PRESENT,N,0,0 ^*/
}x2ap_cell_activation_resp_t;

typedef struct
{
    x2ap_cell_activation_resp_t peer_cell_activation_resp;/*^ M,0,N,0,0 ^*/
}x2ap_peer_cell_activation_resp_t;/*^ API, X2AP_PEER_CELL_ACTIVATION_RESP ^*/

typedef struct
{
    rrc_bitmask_t               bitmask; /*^ BITMASK ^*/
    x2_gb_enb_id_t              enb1_gb_id; /*^ M,0,N,0,0 ^*/
    x2_gb_enb_id_t              enb2_gb_id; /*^ M,0,N,0,0 ^*/
    x2ap_cell_activation_info_t cell_activation_info;/*^ M,0,N,0,0 ^*/   
}x2ap_cell_activation_req_t;/*^ API, X2AP_CELL_ACTIVATION_REQ ^*/

typedef struct
{
    x2ap_cell_activation_resp_t cell_activation_resp;/*^ M,0,N,0,0 ^*/
}x2ap_cell_activation_response_t;/*^ API, X2AP_CELL_ACTIVATION_RESP ^*/







typedef struct
{
    S8  handover_trigger_change;/*^ M,0,B,-20,20 ^*/
}x2ap_mob_param_t;
typedef struct
{
#define X2AP_ENB1_MOBILITY_PARAMETERS_PRESENT 0x01
	U16     bitmask;                /*^ BITMASK ^*/
    x2ap_ecgi_t    enb1_cell_id;    /*^ M,0,N,0,0 ^*/
    x2ap_ecgi_t    enb2_cell_id;    /*^ M,0,N,0,0 ^*/
    x2ap_mob_param_t  enb1_mob_param; /*^ O,X2AP_ENB1_MOBILITY_PARAMETERS_PRESENT,N,0,0 ^*/
    x2ap_mob_param_t  enb2_mob_param; /*^ M,0,N,0,0 ^*/
    x2ap_cause_t   cause;  /*^ M,0,N,0,0 ^*/
}x2ap_mobility_change_info_t;
typedef struct
{
	U16 bitmask;		    	/*^ BITMASK ^*/
	x2_gb_enb_id_t gb_enb_id;	/*^ M,0,N,0,0 ^*/
    x2ap_mobility_change_info_t  mobility_change_info;/*^ M,0,N,0,0 ^*/
}x2ap_mobility_change_req_t; /*^ API, X2AP_MOBILITY_CHANGE_REQ ^*/

typedef struct
{
	U16 bitmask;		    	/*^ BITMASK ^*/
    S8  ho_trigger_change_lower_limit; /*^ M,0,B,-20,20^*/
    S8  ho_trigger_change_upper_limit; /*^ M,0,B,-20,20^*/
}x2ap_mob_range_t;

typedef struct
{
#define X2AP_MOBILITY_SETTING_CAUSE_PRESENT 0x01
#define X2AP_MOBILITY_PARAM_MOD_RANGE_PRESENT 0x02
	U16     bitmask;            /*^ BITMASK ^*/
	x2_gb_enb_id_t enb1_gb_id;	/*^ M,0,N,0,0 ^*/
	x2_gb_enb_id_t enb2_gb_id;	/*^ M,0,N,0,0 ^*/
    x2ap_ecgi_t    enb1_cell_id;    /*^ M,0,N,0,0 ^*/
    x2ap_ecgi_t    enb2_cell_id;    /*^ M,0,N,0,0 ^*/
    U8     response;  /*^ M,0,N,0,0 ^*/
    x2ap_cause_t   cause;           /*^ O,X2AP_MOBILITY_SETTING_CAUSE_PRESENT,N,0,0 ^*/
    x2ap_mob_range_t  enb2_mob_param_mod_range;/*^ O,X2AP_MOBILITY_PARAM_MOD_RANGE_PRESENT,N,0,0 ^*/ 
}x2ap_mob_ch_resp_t;

typedef struct
{
    x2ap_mob_ch_resp_t  mobility_change_resp;/*^ M,0,N,0,0 ^*/
}x2ap_mobility_change_resp_t; /*^ API, X2AP_MOBILITY_CHANGE_RESP ^*/

typedef struct
{
	U16 bitmask;		    	/*^ BITMASK ^*/
	x2_gb_enb_id_t enb1_gb_id;	/*^ M,0,N,0,0 ^*/
	x2_gb_enb_id_t enb2_gb_id;	/*^ M,0,N,0,0 ^*/
    x2ap_mobility_change_info_t  mobility_change_info;/*^ M,0,N,0,0 ^*/
}x2ap_mobility_setting_change_req_t; /*^ API, X2AP_MOBILITY_SETTING_CHANGE_REQ ^*/

typedef struct
{
    x2ap_mob_ch_resp_t  mobility_setting_change_resp;/*^ M,0,N,0,0 ^*/
}x2ap_mobility_setting_change_resp_t; /*^ API, X2AP_MOBILITY_SETTING_CHANGE_RESP ^*/

#endif /* RRC_X2APCOMMON_INTF_H_ */
