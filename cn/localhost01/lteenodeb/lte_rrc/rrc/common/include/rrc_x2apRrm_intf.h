/******************************************************************************
*
*   FILE NAME   : rrc_x2apRrm_intf.h
*
*   DESCRIPTION : This file contains basic X2AP RRM interface definitions.
*
*   DATE            AUTHOR         REFERENCE       REASON
*   12-JULY-2010    Vaibhav Singh  ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _X2APRRM_INTF_H_ 
#define _X2APRRM_INTF_H_
#include "rrc_defines.h"
#include "x2ap_api.h"
#include "rrc_rrm_intf.h"

#define RRC_X2APRRM_MODULE_ID    RRC_RRM_MODULE_ID
#define X2AP_LI_MAX_PRBS_COUNT 110
#define X2AP_RRM_LI_MAX_HII_PRB_OCTETS 110
/* eICIC changes start */
#define X2AP_ABS_PATTERN_SIZE_FDD 5
#define X2AP_ABS_PATTERN_SIZE_TDD 9
#define X2AP_MEASUREMENT_SUBSET_SIZE_FDD 5
#define X2AP_MEASUREMENT_SUBSET_SIZE_TDD 9
/* eICIC changes stop */

typedef struct _mbms_cell_info_t
{
#define RRM_X2AP_MBMS_CONFIG_APPLY_PRESENT      0x01
    rrc_bitmask_t              bitmask;/*^ BITMASK ^*/
    x2ap_ecgi_t                cell_id;/*^ M,0,N,0,0 ^*/
    x2ap_mbsfn_subframe_info_t mbsfn_subframe_info;/*^ O, RRM_X2AP_MBMS_CONFIG_APPLY_PRESENT  , N, 0, 0 ^*/
}mbms_cell_info_t;

typedef struct _rrm_updated_mbms_sf_info_t
{
    U8                num_cells; /*^ M,0,B,1,8 ^*/
    mbms_cell_info_t  cell_info [MAX_SERVED_CELLS];
    /*^ M,0,OCTET_STRING,VARIABLE ^*/
}rrm_updated_mbms_sf_info_t; /*^ API, X2AP_RRM_UPDATED_MBMS_SF_INFO_IND  ^*/


typedef struct
{
    U8   type;/*^ M, 0, N, 0, 0 ^*/
    U16  value;/*^ M, 0, N, 0, 0 ^*/
} x2ap_rrm_cause_t;

/* MENB CHANGES - START */
#define X2AP_RRM_LINK_DOWN_GB_ENB_ID_PRESENT    0x01
#define X2AP_RRM_LINK_DOWN_GB_GNB_ID_PRESENT    0x02
typedef struct
{
    rrc_bitmask_t              bitmask;
    /*^ BITMASK ^*/

    x2_gb_enb_id_t        peer_gb_enb_id;
    /*^ O,X2AP_RRM_LINK_DOWN_GB_ENB_ID_PRESENT,N,0,0 ^*/

    x2_gb_gnb_id_t        peer_gb_gnb_id;
    /*^ O,X2AP_RRM_LINK_DOWN_GB_GNB_ID_PRESENT,N,0,0 ^*/
}x2ap_rrm_x2_link_down_ind_t;/*^ API, X2AP_RRM_LINK_DOWN_IND ^*/
/* MENB CHANGES - END */

typedef struct
{
    rrc_cell_index_t         served_cell_id;/*^ M,0,H,0,MAX_CELL_INDEX ^*/ /* MAX_NUM_CELLS - 1 */
    x2ap_srvd_cell_arry_t   ngh_dir_cell_arr;/*^ M,0,N,0,0 ^*/
}x2ap_rrm_cell_info_t;

/* MENB CHANGES - START */
typedef struct
{
    x2_gb_enb_id_t           global_enb_id; /*^ M,0,N,0,0 ^*/

	/* SPR_17664_start*/
    U16                      num_served_cell; /*^ M,0,H,0,MAX_SERVED_CELLS ^*/
    
	/* SPR_17664_end*/
    x2ap_rrm_cell_info_t     cell_info[MAX_SERVED_CELLS];  /*^ M,0,OCTET_STRING,VARIABLE ^*/

	/* SPR_17664_start*/
    U8                       num_gu_group_id; /*^ M,0,H,0,MAX_ENB_POOLS ^*/
   
	/* SPR_17664_end*/
    x2ap_gu_group_id_t       group_id_arr[MAX_ENB_POOLS]; /*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_rrm_eutra_cell_info_t;

#define    X2AP_NR_CELL_INFO_FIVE_GS_TAC_PRESENT    0x01
#define    X2AP_NR_CELL_INFO_CONFIGURED_TAC_PRESENT 0x02
typedef struct
{
    U8                                       bitmask;
    /*^ BITMASK ^*/

    U16                                      nr_pci;
    /*^ M, 0, H, 0, NR_MAX_PCI ^*/

    x2ap_nr_ecgi_t                           nr_cell_id;
    /*^ M, 0, N, 0, 0 ^*/

    x2ap_broadcast_plmn_t                    broadcast_plmn_info;
    /*^ M, 0, N, 0, 0 ^*/

    x2ap_choice_nr_mode_info_t               choice_nr_mode;
    /*^ M, 0, N, 0, 0 ^*/

#ifndef LINUX_PC_TEST
/* ENDC_MEAS_CHANGES_START */
    U8                                       meas_timing_config_count;
    /*^ M, 0, B, 1, RRC_MAX_MEAS_FREQ_MN ^*/

    x2ap_meas_timing_config_t                x2ap_meas_timing_config[RRC_MAX_MEAS_FREQ_MN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
/* ENDC_MEAS_CHANGES_END */
#endif

    x2ap_five_gs_tac_t                       fiveGS_track_area_code;
    /*^ O, X2AP_NR_CELL_INFO_FIVE_GS_TAC_PRESENT, N, 0, 0 ^*/

    x2ap_tac_t                               configured_tac;
    /*^ O, X2AP_NR_CELL_INFO_CONFIGURED_TAC_PRESENT, N, 0, 0 ^*/

#ifdef LINUX_PC_TEST
    U32                              meas_timing_config_buf_len;
    /*^ M, 0, H, 0, MAX_ASN_BUFFER ^*/

    U8                               measurement_timing_config_asn_buff[MAX_ASN_BUFFER];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
#endif

}x2ap_rrm_srvd_nr_cell_info_t;

#define NR_NEIGHBOUR_INFO_PRESENT    0x01
typedef struct
{
    rrc_bitmask_t                   bitmask;
    /*^ BITMASK ^*/

    U8                              default_sgnb;
    /*^ M, 0, H, 0, 1 ^*/

    x2_gb_gnb_id_t                  global_gnb_id;
    /*^ M, 0, N, 0, 0 ^*/

    U16                             num_served_cell;
    /*^ M, 0, H, 0, MAX_SERVED_CELLS ^*/

    x2ap_rrm_srvd_nr_cell_info_t    served_cell_info[MAX_SERVED_CELLS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    U16                             num_neigh_cell;
    /*^ M, 0, H, 0, MAX_SERVED_CELLS ^*/

    x2ap_nr_neigh_cell_arry_t       nr_neigh_cell_arr[MAX_SERVED_CELLS];
    /*^ O, NR_NEIGHBOUR_INFO_PRESENT, OCTET_STRING, VARIABLE ^*/
}x2ap_rrm_nr_cell_info_t;

#define RRM_X2AP_EUTRA_CELL_INFO_PRESENT      0x01
#define RRM_X2AP_NR_CELL_INFO_PRESENT         0x02
typedef struct
{
    rrc_bitmask_t              bitmask;
    /*^ BITMASK ^*/

    x2ap_rrm_eutra_cell_info_t eutra_cell_info;
    /*^ O,RRM_X2AP_EUTRA_CELL_INFO_PRESENT,N,0,0 ^*/

    x2ap_rrm_nr_cell_info_t    nr_cell_info;
    /*^ O,RRM_X2AP_NR_CELL_INFO_PRESENT,N,0,0 ^*/
}x2ap_rrm_x2_link_up_ind_st;/*^ API, X2AP_RRM_LINK_UP_IND ^*/
/* MENB CHANGES - END */

typedef enum 
{
X2AP_RRM_RSU_REPORTING_PERIODICITY_1000MS =0,
X2AP_RRM_RSU_REPORTING_PERIODICITY_2000MS, 
X2AP_RRM_RSU_REPORTING_PERIODICITY_5000MS,
X2AP_RRM_RSU_REPORTING_PERIODICITY_10000MS
}x2ap_rrm_rsu_reporting_periodicity_et;

/* eICIC changes start */
typedef struct
{
    U8      usable_abs_pattern_fdd_info[X2AP_ABS_PATTERN_SIZE_FDD]; /*^ M,0,OCTET_STRING,FIXED ^*/
}x2ap_usable_abs_information_fdd_t;

typedef struct
{
    U8      usable_abs_pattern_tdd_info[X2AP_ABS_PATTERN_SIZE_TDD]; /*^ M,0,OCTET_STRING,FIXED ^*/
}x2ap_usable_abs_information_tdd_t;


typedef struct
{
#define X2AP_USABLE_ABS_INFORMATION_FDD_PRESENT 0x01
#define X2AP_USABLE_ABS_INFORMATION_TDD_PRESENT 0x02

    rrc_bitmask_t                       bitmask;   /*^ BITMASK ^*/      
    x2ap_usable_abs_information_fdd_t        usable_abs_info_fdd;
    /*^ O, X2AP_USABLE_ABS_INFORMATION_FDD_PRESENT, N, 0, 0 ^*/
    x2ap_usable_abs_information_tdd_t        usable_abs_info_tdd;
    /*^ O, X2AP_USABLE_ABS_INFORMATION_TDD_PRESENT, N, 0, 0 ^*/
}x2ap_usable_abs_information_t;

typedef struct
{
    U8                                      dl_abs_status; /*^ M,0,H,0,100 ^*/
    x2ap_usable_abs_information_t           x2ap_usable_abs_information;  /*^ M,0,N,0,0 ^*/
}x2ap_rrm_abs_status_t;
/* eICIC changes stop */

typedef struct
{
#define RRM_RSU_START_PARTIAL_SUCC_IND_IS_PRESENT 0x01 
   /* SPR 18994 Fix Start */ 
    U32                          report_characteristics;/*^ M,0,N,0,0 ^*/
   /* SPR 18994 Fix End */ 
	/* SPR_17664_start*/
    U16                          num_cell;        /*^ M,0,B,1,MAX_SERVED_CELLS ^*/
	/* SPR_17664_end*/
    rrm_cell_global_id_eutra_t   cell_info[MAX_SERVED_CELLS];    /*^ M,0,OCTET_STRING,VARIABLE ^*/
    U8                           report_periodicity;  /*^ M,0,H,0,3 ^*/ 
    U16                          bitmask;      /*^ BITMASK ^*/
    U8                           partial_success_indicator;/*^ O, RRM_RSU_START_PARTIAL_SUCC_IND_IS_PRESENT,N,0,0 ^*/
}x2ap_rrm_rsu_rrm_start_req_t; /*^ API, X2AP_RRM_RSU_RRM_START_REQ ^*/

typedef enum
{
 X2AP_RRM_RSU_FAIL,
 X2AP_RRM_RSU_SUCCESS
}x2ap_rrm_rsu_response_et;

typedef struct
{
   /* SPR 18994 Fix Start */
    U32    report_characteristics;/*^ M,0,N,0,0 ^*/
   /* SPR 18994 Fix End */
    x2ap_rrm_cause_t      rrm_cause;
}measurement_failure_cause_item_t;

typedef struct
{
#define X2AP_RRM_FAILED_CAUSE_NUM_ITEM_PRESENT 0x01 
#define X2AP_RRM_FAILED_CAUSE_ITEM_PRESENT 0x02 
    U16              bitmask;      /*^ BITMASK ^*/
    rrm_cell_global_id_eutra_t     cell_info;

	/* SPR_17664_start*/
    U16                            num_measurement;        /*^ O,X2AP_RRM_FAILED_CAUSE_NUM_ITEM_PRESENT,B,1,MAX_ENB_MEASUREMENTS ^*/
    
	/* SPR_17664_end*/
    measurement_failure_cause_item_t measurement_failure_cause_item[MAX_ENB_MEASUREMENTS];/*^ O,X2AP_RRM_FAILED_CAUSE_ITEM_PRESENT,OCTET_STRING,VARIABLE ^*/
}rsu_measurement_res_item_t;

typedef struct
{
	/* SPR_17664_start*/
    U16                          num_cell;        /*^ M,0,B,1,MAX_SERVED_CELLS ^*/
	/* SPR_17664_end*/
    rsu_measurement_res_item_t   rsu_measurement_res_item[MAX_SERVED_CELLS]; /*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_rrm_rsu_measurement_res_t;

typedef struct
{
#define ENB_MEAS_ID_IS_PRESENT               0x01
#define ENB_START_RES_RRM_CAUSE_IS_PRESENT   0x02
#define ENB_ID_IS_PRESENT     0x04
#define ENB_START_PARTIAL_SUCCESS            0x08
    U16              bitmask;      /*^ BITMASK ^*/
    U8               response;      /*^ M, 0, H, 0, 1 ^*//* x2ap_return_et */ 
    U32              enb_meas_id;  /*^ O,ENB_MEAS_ID_IS_PRESENT,B,1,4095 ^*/
    x2ap_rrm_cause_t      rrm_cause;    /*^ O,ENB_START_RES_RRM_CAUSE_IS_PRESENT,N,0,0 ^*/
    x2_gb_enb_id_t        global_enb_id; /*^ O, ENB_ID_IS_PRESENT,N,0,0 ^*//*x2ap_rsu_gb_enb_id_fix*/
    x2ap_rrm_rsu_measurement_res_t rrm_rsu_measurement_res_list; /*^ O,ENB_START_PARTIAL_SUCCESS,N,0,0 ^*/
}x2ap_rrm_rsu_enb_start_res_t; /*^ API, X2AP_RRM_RSU_ENB_START_RES^*/

typedef struct
{
#define ENB_STOP_RES_RRM_CAUSE_IS_PRESENT   0x01
    U8              bitmask;          /*^ BITMASK ^*/
    U32             enb_meas_id;      /*^ M,0,B,1,4095 ^*/
    x2_gb_enb_id_t  peer_gb_enb_id;   /*^ M,0,N,0,0 ^*/    
    U8              response;          /*^ M, 0, H, 0, 1 ^*/    /* x2ap_return_et */
    x2ap_rrm_cause_t     rrm_cause;        /*^ O, ENB_STOP_RES_RRM_CAUSE_IS_PRESENT,N,0,0 ^*/
}x2ap_rrm_rsu_enb_stop_res_t; /*^ API, X2AP_RRM_RSU_ENB_STOP_RES ^*/

typedef struct 
{
#define RRM_START_RES_RRM_CAUSE_IS_PRESENT   0x01
    U16              bitmask;        /*^ BITMASK ^*/
    U32              enb_meas_id;    /*^ M,0,B,1,4095 ^*/
    x2_gb_enb_id_t   peer_gb_enb_id; /*^ M,0,N,0,0 ^*/
    U8               response;        /*^ M, 0, H, 0, 1 ^*/   /* x2ap_return_et */
    x2ap_rrm_cause_t      rrm_cause;      /*^ O, RRM_START_RES_RRM_CAUSE_IS_PRESENT,N,0,0 ^*/
    x2ap_rrm_rsu_measurement_res_t rrm_rsu_measurement_res_list; /*^ O,ENB_START_PARTIAL_SUCCESS,N,0,0 ^*/
}x2ap_rrm_rsu_rrm_start_res_t; /*^ API, X2AP_RRM_RSU_RRM_START_RES ^*/

typedef struct 
{
#define RRM_STOP_RES_RRM_CAUSE_IS_PRESENT   0x01
    U16              bitmask;        /*^ BITMASK ^*/
    U32              enb_meas_id;    /*^ M,0,B,1,4095 ^*/
    x2_gb_enb_id_t   peer_gb_enb_id; /*^ M,0,N,0,0 ^*/
    U8               response;        /*^ M,0,H,0,1 ^*/   /* x2ap_return_et */
    x2ap_rrm_cause_t      rrm_cause;      /*^ O, RRM_STOP_RES_RRM_CAUSE_IS_PRESENT,N,0,0 ^*/
}x2ap_rrm_rsu_rrm_stop_res_t ;/*^ API, X2AP_RRM_RSU_RRM_STOP_RES ^*/

typedef struct
{
   /* SPR 18994 Fix Start */
    U32                            report_characteristics; /*^ M,0,N,0,0 ^*/
   /* SPR 18994 Fix End */
    x2_gb_enb_id_t                 peer_gb_enb_id;         /*^ M,0,N,0,0 ^*/
    U32                            enb_meas_id;            /*^ M,0,B,1,4095 ^*/
	/* SPR_17664_start*/
    U16                            num_cell;        /*^ M,0,H,1,MAX_SERVED_CELLS ^*/
	/* SPR_17664_end*/
    rrm_cell_global_id_eutra_t     cell_info[MAX_SERVED_CELLS];    /*^ M,0,OCTET_STRING,VARIABLE ^*/
    U8                             report_periodicity; /*^ M,0,H,1,3 ^*/ /* x2ap_rrm_rsu_reporting_periodicity_et */
    U16                            bitmask;        /*^ BITMASK ^*/
    U8                             partial_success_indicator_present; /*^ O,ENB_START_PARTIAL_SUCCESS,N,0,0 ^*/
}x2ap_rrm_rsu_enb_start_req_t; /*^ API, X2AP_RRM_RSU_ENB_START_REQ ^*/

/*Load indicator */
typedef enum
{
    X2AP_RRM_RSU_LOAD_LOW    =   1,     /* Low load */
    X2AP_RRM_RSU_LOAD_MEDIUM,           /* Medium load */
    X2AP_RRM_RSU_LOAD_HIGH,             /* High load */
    X2AP_RRM_RSU_LOAD_OVER              /* Over load */
}x2ap_rrm_rsu_load_indicator_et;

/*Physical resource block usage */
typedef struct
{
    U8 dl_gbr_prb_usage;        /*DL GBR PRB usage:     M, 0, H, 0, 100 */
    U8 ul_gbr_prb_usage;        /*UL GBR PRB usage:     M, 0, H, 0, 100 */
    U8 dl_non_gbr_prb_usage;    /*DL non-GBR PRB usage: M, 0, H, 0, 100 */
    U8 ul_non_gbr_prb_usage;    /*UL non-GBR PRB usage: M, 0, H, 0, 100 */
    U8 dl_total_gbr_prb_usage;  /*DL Total PRB usage:   M, 0, H, 0, 100 */
    U8 ul_total_gbr_prb_usage;  /*UL Total PRB usage:   M, 0, H, 0, 100 */
}x2ap_rrm_rsu_prb_t;

/*S1 transport network layer load */
typedef struct
{
    U8 dl_s1_tnl_load_indicator;  /*^ M,0,H,0,3 ^*/ /* x2ap_rrm_rsu_load_indicator_et */
    U8 ul_s1_tnl_load_indicator;  /*^ M,0,H,0,3 ^*/ /* x2ap_rrm_rsu_load_indicator_et */
}x2ap_rrm_rsu_tnl_load_t;

/*Hardware load */
typedef struct
{
    U8 dl_hardware_load_indicator; /*^ M,0,H,0,3 ^*/ /* x2ap_rrm_rsu_load_indicator_et */
    U8 ul_hardware_load_indicator; /*^ M,0,H,0,3 ^*/ /* x2ap_rrm_rsu_load_indicator_et */
}x2ap_rrm_rsu_hw_load_t;

/*Composite available capacity*/
typedef struct
{
#define CELL_CAPACITY_CLASS_VALUE_PRESENT   0x01  
    U8 bitmask;                    /*^ BITMASK ^*/ 
    U8 cell_capacity_class_value;  /*^ O,CELL_CAPACITY_CLASS_VALUE_PRESENT,B,1,100 ^*/
    U8 cell_capacity_value;        /*^ M,0,H,0,100 ^*/
}x2ap_rrm_rsu_composite_available_capacity_t;

/*Composite available capacity group*/
typedef struct
{
    x2ap_rrm_rsu_composite_available_capacity_t dl_composite_available_capacity; /*^ M,0,N,0,0 ^*/
    x2ap_rrm_rsu_composite_available_capacity_t ul_composite_available_capacity; /*^ M,0,N,0,0 ^*/
}x2ap_rrm_rsu_cacg_t;

typedef struct _x2ap_rrm_rsu_info_t
{
#define PRB_PERIODIC_REPORT_PRESENT                          0x01
#define TNL_LOAD_IND_PERIODIC_REPORT_PRESENT                 0x02
#define LOAD_IND_PERIODIC_REPORT_PRESENT                     0x04
#define COMPOSITE_AVAILABLE_CAPACITY_PERIODIC_REPORT_PRESENT 0x08
/* eICIC changes start */
#define ABS_STATUS_PERIODIC_REPORT_PRESENT                   0x10
/* eICIC changes stop */
    U8                                       bitmask;  /*^ BITMASK ^*/
    x2ap_rrm_rsu_prb_t                       radio_resource_status; 
    /*^ O,PRB_PERIODIC_REPORT_PRESENT,N,0,0 ^*/
    x2ap_rrm_rsu_tnl_load_t                  s1_tnl_load_indicator; 
    /*^ O,TNL_LOAD_IND_PERIODIC_REPORT_PRESENT,N,0,0 ^*/
    x2ap_rrm_rsu_hw_load_t                   hardware_load_indicator; 
    /*^ O,LOAD_IND_PERIODIC_REPORT_PRESENT,N,0,0 ^*/
    x2ap_rrm_rsu_cacg_t                      composite_available_capacity_group;
    /*^ O,COMPOSITE_AVAILABLE_CAPACITY_PERIODIC_REPORT_PRESENT,N,0,0 ^*/
    /* eICIC changes start */
    x2ap_rrm_abs_status_t                    abs_status;
    /*^ O,ABS_STATUS_PERIODIC_REPORT_PRESENT,N,0,0 ^*/
    /* eICIC changes stop */
}x2ap_rrm_rsu_info_t; 

typedef struct _x2ap_rrm_rsu_report_info_t
{
    rrm_cell_global_id_eutra_t    cell_info; /*^ M,0,N,0,0 ^*/
    x2ap_rrm_rsu_info_t           rsu_info; /*^ M,0,N,0,0 ^*/
}x2ap_rrm_rsu_report_info_t;

typedef struct 
{
    U32                                   enb_meas_id;    /*^ M,0,B,1,4095 ^*/
    x2_gb_enb_id_t                        peer_gb_enb_id; /*^ M,0,N,0,0 ^*/
	/* SPR_17664_start*/
    U16                                   num_reports;    /*^ M,0,H,1,MAX_SERVED_CELLS ^*/
	/* SPR_17664_end*/
    x2ap_rrm_rsu_report_info_t            rsu_report[MAX_SERVED_CELLS]; /*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_rrm_rsu_rrm_update_t; /*^ API, X2AP_RRM_RSU_RRM_UPDATE ^*/

typedef struct
{
   U32             enb_meas_id; /*^ M,0,B,1,4095 ^*/
   x2_gb_enb_id_t  peer_gb_enb_id; /*^ M,0,N,0,0 ^*/
}x2ap_rrm_rsu_rrm_stop_req_t; /*^ API, X2AP_RRM_RSU_RRM_STOP_REQ ^*/


typedef struct
{
   U32             enb_meas_id; /*^ M,0,B,1,4095 ^*/
   x2_gb_enb_id_t  peer_gb_enb_id; /*^ M,0,N,0,0 ^*/
}x2ap_rrm_rsu_enb_stop_req_t; /*^ API, X2AP_RRM_RSU_ENB_STOP_REQ ^*/

typedef struct 
{
    
   U32                             enb_meas_id; /*^ M,0,B,1,4095 ^*/
   x2_gb_enb_id_t                  peer_gb_enb_id; /*^ M,0,N,0,0 ^*/
	/* SPR_17664_start*/
   U16                             num_reports; /*^ M,0,H,1,MAX_SERVED_CELLS ^*/
	/* SPR_17664_end*/
   x2ap_rrm_rsu_report_info_t      rsu_report[MAX_SERVED_CELLS]; /*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_rrm_rsu_enb_update_ind_t; /*^ API, X2AP_RRM_RSU_ENB_UPDATE_IND ^*/


typedef enum {
   X2AP_LI_ANTENA_PORT_ONE  = 0,
   X2AP_LI_ANTENA_PORT_TWO = 1,
   X2AP_LI_ANTENA_PORT_FOUR  = 2
}x2ap_rrm_li_num_cell_specific_antena_port_et;

typedef enum {
   X2AP_LI_MINUS_INFINITY = 0,
   X2AP_LI_MINUS_ELEVEN = 1,
   X2AP_LI_MINUS_TEN = 2,
   X2AP_LI_MINUS_NINE = 3,
   X2AP_LI_MINUS_EIGHT = 4,
   X2AP_LI_MINUS_SEVEN = 5,
   X2AP_LI_MINUS_SIX = 6,
   X2AP_LI_MINUS_FIVE = 7,
   X2AP_LI_MINUS_FOUR = 8,
   X2AP_LI_MINUS_THREE = 9,
   X2AP_LI_MINUS_TWO = 10,
   X2AP_LI_MINUS_ONE = 11,
   X2AP_LI_ZERO = 12,
   X2AP_LI_ONE = 13,
   X2AP_LI_TWO = 14,
   X2AP_LI_THREE = 15
}x2ap_rrm_li_rntp_threshold_et;

typedef struct
{
	/* SPR_17664_start*/
    U8    prb_count;/*^M, 0, B, 6, X2AP_RRM_LI_MAX_HII_PRB_OCTETS ^*/
	/* SPR_17664_end*/
    U8    rntp_per_prb[X2AP_RRM_LI_MAX_HII_PRB_OCTETS]; /*^ M, 0, BIT_STRING,VARIABLE ^*/ 
    U8    rntp_threshold; /*^ M, 0, H, 0, 15 ^*/ /* x2ap_rrm_li_rntp_threshold_et */
    /*number of antenna ports for cell-specific reference signals*/
    U8    num_cell_specific_antena_port;
    /*^ M, 0, H, 0, 2 ^*/ /* x2ap_rrm_li_num_cell_specific_antena_port_et */
    U32   p_b; /*^ M, 0, H, 0, 3 ^*/
    /*Measured by Predicted Number Of Occupied PDCCH OFDM Symbols*/
    U32   pdcch_interference_impact; /*^ M, 0, H, 0, 4 ^*/
}x2ap_rrm_li_relative_narrowband_tx_power_t;

typedef struct
{
	/* SPR_17664_start*/
    U8  prb_count; /*^M, 0, B, 1, X2AP_RRM_LI_MAX_HII_PRB_OCTETS ^*/
	/* SPR_17664_end*/
    U8  hii[X2AP_RRM_LI_MAX_HII_PRB_OCTETS]; /*^ M,0,BIT_STRING,VARIABLE ^*/
}x2ap_rrm_li_ul_high_intf_ind_t;

typedef struct
{
    /*Id of the cell for which the HII is meant*/
    rrm_cell_global_id_eutra_t target_cell_id; /*^ M, 0, N, 0, 0 ^*/
    x2ap_rrm_li_ul_high_intf_ind_t  ul_high_intf_ind; /*^ M, 0, N, 0, 0 ^*/
}x2ap_rrm_li_ul_high_intf_info_arr_t;    
typedef struct
{
	/* SPR_17664_start*/
    U16    ul_high_interference_info_count;/*^M, 0, H, 0, MAX_SERVED_CELLS ^*/
	/* SPR_17664_end*/
    x2ap_rrm_li_ul_high_intf_info_arr_t  ul_high_intf_info[MAX_SERVED_CELLS];
    /*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_rrm_li_ul_high_interference_info_t;
typedef enum
{
    X2AP_LI_HIGH_INTERFERENCE = 0,
    X2AP_LI_MEDIUM_INTERFERENCE = 1,
    X2AP_LI_LOW_INTERFERENCE = 2
}x2ap_rrm_li_ul_intf_overload_ind_et;

typedef struct
{
	/* SPR_17664_start*/
    U16    count;/*^M, 0, H, 0, X2AP_LI_MAX_PRBS_COUNT ^*/ 
	/* SPR_17664_end*/
    U8     ul_intf_overload_indication[X2AP_LI_MAX_PRBS_COUNT]; /*^ M,0,OCTET_STRING,VARIABLE,H,0,2^*/
}x2ap_rrm_li_ul_intf_overload_ind_t;


/* eICIC changes start */
typedef struct
{
    U8                   abs_pattern_info[X2AP_ABS_PATTERN_SIZE_TDD];             /*^ M,0,OCTET_STRING,FIXED ^*/
    U32                  no_cell_specific_antenna_port;                           /*^M, 0, H, 0, 2 ^*/ /* cell_specific_antenna_port_et */
    U8                   measurement_subset[X2AP_MEASUREMENT_SUBSET_SIZE_TDD];    /*^ M,0,OCTET_STRING,FIXED ^*/
    
}x2ap_abs_information_tdd_t;

typedef struct
{
    U8                   abs_pattern_info[X2AP_ABS_PATTERN_SIZE_FDD];             /*^ M,0,OCTET_STRING,FIXED ^*/
    U32                  no_cell_specific_antenna_port;                           /*^ M, 0, H, 0, 2 ^*/ /* cell_specific_antenna_port_et */
    U8                   measurement_subset[X2AP_MEASUREMENT_SUBSET_SIZE_FDD];    /*^ M,0,OCTET_STRING,FIXED ^*/
}x2ap_abs_information_fdd_t;

typedef struct
{
#define X2AP_LI_ABS_INFORMATION_FDD_PRESENT 0x01
#define X2AP_LI_ABS_INFORMATION_TDD_PRESENT 0x02
#define X2AP_LI_ABS_INFORMATION_ABS_INACTIVE_PRESENT 0x04

    rrc_bitmask_t bitmask; /*^ BITMASK ^*/
    x2ap_abs_information_fdd_t   abs_info_fdd; 
    /*^ O, X2AP_LI_ABS_INFORMATION_FDD_PRESENT, N, 0, 0 ^*/
    x2ap_abs_information_tdd_t   abs_info_tdd; 
    /*^ O, X2AP_LI_ABS_INFORMATION_TDD_PRESENT, N, 0, 0 ^*/
}x2ap_rrm_li_abs_information_t;


typedef struct
{
    U32             invoke_ind; /*^ M, 0, H, 0, 0 ^*/
    /*invoke_indication_et*/
}x2ap_rrm_li_invoke_indication_t;
/* eICIC changes stop */

typedef struct
{
#define X2AP_LI_CELL_INFO_ITEM_UL_INTF_OVERLOAD_IND_PRESENT 0x01
#define X2AP_LI_CELL_INFO_ITEM_UL_INTF_INFO_PRESENT 0x02
#define X2AP_LI_CELL_INFO_ITEM_RELATIVE_NARROWBAND_TX_POWER_PRESENT 0x04
/* eICIC changes start */
#define X2AP_LI_ABS_INFORMATION_PRESENT 0x08
#define X2AP_LI_INVOKE_INDICATION_PRESENT 0x10
/* eICIC changes stop */

    rrc_bitmask_t  bitmask; /*^ BITMASK ^*/
    /*Id of the source cell*/
    rrm_cell_global_id_eutra_t    source_cell_id;/*^ M, 0, N, 0, 0 ^*/
    x2ap_rrm_li_ul_intf_overload_ind_t ul_intf_overload_ind;
    /*^ O, X2AP_LI_CELL_INFO_ITEM_UL_INTF_OVERLOAD_IND_PRESENT, N, 0, 0 ^*/
    x2ap_rrm_li_ul_high_interference_info_t    ul_high_interference_info;
    /*^ O, X2AP_LI_CELL_INFO_ITEM_UL_INTF_INFO_PRESENT, N, 0, 0 ^*/
    x2ap_rrm_li_relative_narrowband_tx_power_t    relative_narrowband_tx_power;
    /*^ O, X2AP_LI_CELL_INFO_ITEM_RELATIVE_NARROWBAND_TX_POWER_PRESENT, N, 0, 0 ^*/
    /* eICIC changes start */
     x2ap_rrm_li_abs_information_t abs_infomation;
    /*^ O, X2AP_LI_ABS_INFORMATION_PRESENT, N, 0, 0 ^*/
     x2ap_rrm_li_invoke_indication_t invoke_indication;
    /*^ O, X2AP_LI_INVOKE_INDICATION_PRESENT, N, 0, 0 ^*/
    /* eICIC changes stop */
}x2ap_rrm_li_cell_info_item_arr_t;

typedef struct
{
    x2_gb_enb_id_t     gb_enb_id; /*^ M,0,N,0,0 ^*/
	/* SPR_17664_start*/
    U16  cell_information_count;/*^M, 0, H, 0, MAX_SERVED_CELLS^*/
	/* SPR_17664_end*/
    x2ap_rrm_li_cell_info_item_arr_t  cell_information[MAX_SERVED_CELLS];
    /*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_rrm_load_information_req_t;/*^ API, X2AP_RRM_LI_RRM_LOAD_INFORMATION_REQ ^*/


typedef struct
{
    U8    response;/*^ M, 0, N, 0, 0 ^*/
}x2ap_rrm_load_information_resp_t;/*^ API, X2AP_RRM_LI_ENB_LOAD_INFORMATION_RES ^*/


typedef struct
{
	/* SPR_17664_start*/
    U16    cell_information_count;/*^M, 0, H, 0, MAX_SERVED_CELLS^*/
	/* SPR_17664_end*/
    x2ap_rrm_li_cell_info_item_arr_t  cell_load_info [MAX_SERVED_CELLS];
    /*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_rrm_load_information_ind_t;/*^ API, X2AP_RRM_LI_ENB_LOAD_INFORMATION_IND ^*/
/*x2ap_li_end*/

typedef struct
{
#define X2AP_RLF_IND_RLF_REPORT_PRESENT  0x01
#define X2AP_RLF_IND_UE_INDEX_PRESENT    0x02
#define X2AP_RLF_IND_UE_CONTEXT_RELEASED_PRESENT 0x04
#define X2AP_RLF_IND_CONN_SETUP_IND_PRESENT      0x08

	U16	bitmask;							/*^ BITMASK ^*/
	x2_gb_enb_id_t enb1_gb_id;	/*^ M,0,N,0,0 ^*/
	x2_gb_enb_id_t enb2_gb_id;	/*^ M,0,N,0,0 ^*/
	U16 failure_cell_pci;				/*^ M,0,H,0,503 ^*/
	x2ap_ecgi_t re_est_cell_ecgi;	/*^ M,0,N,0,0 ^*/
	U16 crnti;									/*^ M,0,N,0,0 ^*/
    rrc_rlf_report_t rlf_report; /*^ O,X2AP_RLF_IND_RLF_REPORT_PRESENT,N,0,0 ^*/
    U16       ue_index; /*^ O,X2AP_RLF_IND_UE_INDEX_PRESENT,N,0,0 ^*/
    U8        ue_context_released; /*^ O,X2AP_RLF_IND_UE_CONTEXT_RELEASED_PRESENT,H,0,1 ^*/
    U8                   conn_setup_indicator;/*^ O,X2AP_RLF_IND_CONN_SETUP_IND_PRESENT,H,0,0 ^*/
}x2ap_rlf_ind_t; /*^ API, X2AP_RLF_IND ^*/

/* endc config update ind start */
#define NR_NEIGHBOUR_INFO_PRESENT    0x01
#define RRC_ENDC_X2_NR_DEACT_IND_PRESENT    0x02

typedef struct
{
    U16                   bitmask; /* NR_NEIGHBOUR_INFO_PRESENT */
    x2ap_rrm_srvd_nr_cell_info_t    served_cell_info;
    x2ap_nr_nbour_cell_info_t		neighbour_cell_info;
}x2ap_nr_srvd_cell_arr_t;

typedef struct
{
    rrc_bitmask_t              bitmask;	/* NR_NEIGHBOUR_INFO_PRESENT,RRC_ENDC_X2_NR_DEACT_IND_PRESENT */
    x2ap_nr_ecgi_t              old_nr_cgi;
    U8     			deact_ind;         /*^ O, RRC_ENDC_X2_NR_DEACT_IND_PRESENT,N,0,0,x2ap_deact_et ^*/
    x2ap_rrm_srvd_nr_cell_info_t    served_cell_info;
    x2ap_nr_nbour_cell_info_t		neighbour_cell_info;
}x2ap_nr_srvd_cell_modify_arr_t;

typedef struct
{
    U16			num_nr_srvd_cell_to_management;
    x2ap_nr_srvd_cell_arr_t	nr_served_cell_management_list[MAX_SERVED_CELLS];	
}x2ap_nr_srvd_cell_to_management_list_t;

typedef struct
{
    U16			num_nr_srvd_cell_to_modify;
    x2ap_nr_srvd_cell_modify_arr_t	nr_served_cell_modify_list[MAX_SERVED_CELLS];
}x2ap_nr_srvd_cell_to_modify_list_t;

typedef struct
{
    U16            num_nr_srvd_cell_delete;
    x2ap_nr_ecgi_t    nr_served_cell_delete_list[MAX_SERVED_CELLS];
}x2ap_nr_srvd_cell_to_delete_list_t;

#define X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_MANAGEMENTLIST_PRESENT     0x01
#define X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_MODIFYLIST_PRESENT         0x02
#define X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_DELETELIST_PRESENT         0x04
typedef struct
{
    U16              					bitmask;
    x2_gb_gnb_id_t					global_en_gnb_id;
    x2ap_nr_srvd_cell_to_management_list_t		eutra_nr_cell_management;
    x2ap_nr_srvd_cell_to_modify_list_t			eutra_nr_cell_modify;
    x2ap_nr_srvd_cell_to_delete_list_t			eutra_nr_cell_delete;
}x2ap_endc_config_update_en_gnb_ind_t;
/* endc config update ind end */

/* x2ap-rrm/son endc config update req/resp struct start */
#define X2AP_CELL_ASSISTANCE_LIMITED_LIST_PRESENT 0x01
#define X2AP_CELL_ASSISTANCE_FULL_LIST_PRESENT    0x02
typedef struct
{
	U16			bitmask;
	x2ap_partial_list_t     partial_nr_cells;
	U16			full_nr_cells_ind;	/* complete_info_req_ind_et */		
}x2ap_rrm_cell_assistance_info_t;

typedef struct
{
#define NR_NEIGHBOUR_INFO_PRESENT   0x01

    U16				bitmask;			/* NR_NEIGHBOUR_INFO_PRESENT */
    x2ap_served_cell_info_t   eutra_cell_info;		/*^ M,0,N,0,0 ^*/ 
    x2ap_nr_nbour_cell_info_t   nr_neighbour_cell_info;
}x2ap_served_eutra_nr_cell_info_arr_t;

typedef struct
{
    U8 		old_cell_id;				/*^ M,0,N,0,0^*/ 
    x2ap_served_eutra_nr_cell_info_arr_t    served_eutra_nr_cell_info;	/*^ M,0,N,0,0 ^*/
}x2ap_served_eutra_nr_cells_to_modify_info_t;

typedef struct
{
    U16 num_served_eutra_nr_cell; /*^ M,0,B,1,MAX_SERVED_CELLS ^*/	
    x2ap_served_eutra_nr_cell_info_arr_t	eutra_nr_cell_list[MAX_SERVED_CELLS];		/*^ M,0,OCTET_STRING,VARIABLE ^*/
}x2ap_served_eutra_nr_cells_to_add_list_t;

typedef struct
{
    U16 num_served_eutra_nr_cell; /*^ M,0,B,1,MAX_SERVED_CELLS ^*/
    x2ap_served_eutra_nr_cells_to_modify_info_t eutra_nr_cell_list[MAX_SERVED_CELLS];		/*^ M,0,OCTET_STRING,VARIABLE^*/
}x2ap_served_eutra_nr_cells_to_modify_list_t;

typedef struct
{
    /* SPR_17664_start*/
    U16 num_served_cell; 								/*^ M,0,B,1,MAX_SERVED_CELLS ^*/
	
    /* SPR_17664_end*/
    U8 cell_id_list[MAX_SERVED_CELLS];		/*^ M,0,OCTET_STRING,VARIABLE^*/
}x2ap_served_eutra_nr_cells_to_delete_list_t;

typedef struct
{
#define X2AP_ENDC_CONFIGUPDATE_ENB_CELLASSISTANCE_INFO_PRESENT  0x01
#define X2AP_SERVED_EUTRA_CELLS_TO_ADD_LIST_PRESENT		0x02
#define X2AP_SERVED_EUTRA_CELLS_TO_MODIFY_LIST_PRESENT		0x04
#define X2AP_SERVED_EUTRA_CELLS_TO_DELETE_LIST_PRESENT		0x08

	U16 bitmask; /*^ BITMASK ^*/
	U8 num_peer_gnb; /*^ M,0,H,0, MAX_PEER_GNB ^*/
	x2_gb_gnb_id_t gnb_info[MAX_PEER_GNB]; 			/*^ M,0,OCTET_STRING,VARIABLE ^*/
	
	x2ap_rrm_cell_assistance_info_t		cell_assistance_info;	/*^ O,X2AP_ENDC_CONFIGUPDATE_ENB_CELLASSISTANCE_INFO_PRESENT,N,0,0 ^*/
	x2ap_served_eutra_nr_cells_to_add_list_t	add_served_cell;		/*^ O,X2AP_SERVED_NR_CELLS_TO_ADD_LIST_PRESENT,N,0,0 ^*/
	x2ap_served_eutra_nr_cells_to_modify_list_t	mod_served_cell;	/*^ O,X2AP_SERVED_NR_CELLS_TO_MODIFY_LIST_PRESENT,N,0,0 ^*/
	x2ap_served_eutra_nr_cells_to_delete_list_t	del_served_cell;	/*^ O,X2AP_SERVED_NR_CELLS_TO_DELETE_LIST_PRESENT,N,0,0 ^*/
}x2ap_endc_config_update_req_t; /*^ API, X2AP_SON_ENDC_CONFIG_UPDATE_REQ ^*/

typedef struct
{
#define X2AP_CAUSE_PRESENT 			0x01
#define X2AP_NR_CELL_INFOMATION_PRESENT  0x02
    U16 bitmask;						/*^ BITMASK ^*/
    x2_gb_enb_id_t enb_gb_id;			/*^ M,0,N,0,0 ^*/
    x2_gb_gnb_id_t gnb_gb_id;			/*^ M,0,N,0,0 ^*/
    U8 response;						/*^ M,0,H,0,1 ^*/
    x2ap_cause_t cause;					/*^ O,X2AP_CAUSE_PRESENT,N,0,0 ^*/
    x2ap_nr_srvd_cell_to_management_list_t  nr_cell_information;
}x2ap_endc_config_update_resp_t; /*^ API, X2AP_SON_ENDC_CONFIG_UPDATE_RES ^*/

typedef struct
{
    U16 bitmask; /*^ BITMASK ^*/
    x2_gb_enb_id_t enb_gb_id;	/*^ M,0,N,0,0 ^*/
    x2_gb_gnb_id_t gnb_gb_id;	/*^ M,0,N,0,0 ^*/
    U8 time_to_wait;	/*^ M,0,H,0,2 ^*/
}x2ap_endc_config_update_wait_ind_t; /*^ API, X2AP_ENDC_CONFIG_UPDATE_WAIT_IND ^*/
/* x2ap-rrm/son endc config update req/resp struct end */

#endif
