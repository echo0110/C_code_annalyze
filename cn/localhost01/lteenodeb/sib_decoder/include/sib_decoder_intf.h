/*********************************************************************
 *
 *  FILE NAME   : sib_intf.h
 *
 *  DESCRIPTION : File contains the interface API structures.
 *
 *  REVISION HISTORY :
 *
 *  DATE              Name           Reference               Comments
 *  Feb 1, 2011      Geetanjali       --------               Initial
 *
 *  Copyright (c) 2011, Aricent Inc.
 *
 *******************************************************************/
#ifndef __SIB_INTF__
#define __SIB_INTF__

#define RT_OK            0

typedef enum
{
    FAILURE,
    SUCCESS,
}return_code;

typedef enum
{
    BCCH_BCH_MSG,
    BCCH_DL_SCH
}MsgType;

/* MIB Message Structure start */

#define FRAME_NO_OCTET_SIZE             1
#define SPARE_OCTET_SIZE                2

typedef struct
{
    U8    phich_resource;
    U8    phich_duration;
}phich_config_t;


typedef struct
{
    U8                dl_band_width;
    phich_config_t    phich_config;
    U8                systemFrameNo[FRAME_NO_OCTET_SIZE];
    U8                spare[SPARE_OCTET_SIZE];
}mib_info_t;

/* MIB Message Structure End */

#define MIB_PRESENCE_FLAG                      0x01
#define SIB_TYPE_1_PRESENCE_FLAG               0x02
#define SIB_TYPE_2_PRESENCE_FLAG               0x04
#define SIB_TYPE_3_PRESENCE_FLAG               0x08
#define SIB_TYPE_4_PRESENCE_FLAG               0x10
#define SIB_TYPE_5_PRESENCE_FLAG               0x20
#define SIB_TYPE_6_PRESENCE_FLAG               0x40
#define SIB_TYPE_9_PRESENCE_FLAG               0x200

/* SIB1 Message structure Start */

#define MAX_MNC_OCTET_SIZE                  3
#define MCC_OCTET_SIZE                      3
#define MAX_PLMN_ID_INFO_SIZE               6
#define TAC_OCTET_SIZE                      2
#define CELL_ID_OCTET_SIZE                  4
#define CSG_ID_OCTET_SIZE                   4
#define MAX_SIB_MESSAGE                     32
#define MAX_SIB_MESSAGE_1                   MAX_SIB_MESSAGE - 1
#define MAX_SI_MESSAGE                      32

typedef struct
{
    U8        count;                   
    U8        mnc[MAX_MNC_OCTET_SIZE];
}mnc_t;

#define PLMN_IDENTITY_MCC_PRESENCE_FLAG     0x01

typedef struct
{
    U16       presence_bitmask;    
    U8        mcc[MCC_OCTET_SIZE]; /* O */
    mnc_t     mnc;                
}plmn_identity_t;

typedef struct
{
    plmn_identity_t   plmn_identity;        
    U8                cell_res_for_operator_use;
}plmn_identity_info_t;

typedef struct
{
    U8                    count;     
    plmn_identity_info_t  plmn_identity_info[MAX_PLMN_ID_INFO_SIZE];
}plmn_id_info_list_t;

#define CELL_ACCESS_INFO_CSG_ID_PRESENCE_FLAG   0x01

typedef struct
{
    U16      presence_bitmask;                
    U8       tac[TAC_OCTET_SIZE];             
    U8       cell_Id[CELL_ID_OCTET_SIZE];    
    U8       cell_barred;
    U8       intra_freq_reselection;
    U8       csg_indication;
    U8       csg_identity[CSG_ID_OCTET_SIZE]; /* O */
    plmn_id_info_list_t   plmn_Id_info_list;  
}cell_access_related_info_t;

#define CELL_SELECT_INFO_Q_RX_LEV_MIN_OFFSET_PRESENCE_FLAG  0x01

typedef struct
{
    U16       presence_bitmask;       
    S8        q_rx_lev_min;  
    U8        q_rx_lev_min_offset; /* O */
}cell_selection_Info_t;

typedef struct
{
    U8            count;
    /* SPR 12568 START */
    U32            sib_type[MAX_SIB_MESSAGE_1];
    /* SPR 12568 STOP */
}sib_mapping_info_t;

typedef struct
{
    sib_mapping_info_t   sib_mapping_info;
    U8                   si_periodicity;
}sheduling_info_t;

typedef struct
{
    U8                count;
    sheduling_info_t  sheduling_info[MAX_SI_MESSAGE];
}scheduling_info_list_t;

typedef struct
{
    U8    sub_frame_assignment;
    U8    special_sub_frame_pattern;
}tdd_config_t;

typedef struct
{
    U8    no_octet;
    U8    *data;
}dynamic_str_t;

#define SIB_TYPE_1_QUAL_MIN_OFFSET_R9_PRESENT_FLAG       0x01

typedef struct
{
    U16              presence_bitmask;
    S8               q_qual_min_r9;
    U8               q_qual_min_offset_r9; /* O */
}cell_selection_info_v920_t;

#define SIB_TYPE_1_IMS_EMERGENCYSUPPORT_R9_PRESENT_FLAG  0x01
#define SIB_TYPE_1_CELL_SELECTION_INFO_PRESENT_FLAG      0x02
#define SIB_TYPE_1_NONCRITICAL_EXT_PRESENT_FLAG          0x04

typedef struct
{
    U16                          presence_bitmask;
    U8                           ims_emergencysupport_r9; /* O */
    cell_selection_info_v920_t   cell_selection_info_v920 ; /* O */
    U8                           noncritical_ext; /* O */
}sib_type_1_v920_ie_t;

#define SIB_TYPE_1_LATE_NON_CRITICAL_EXT_PRESENT_FLAG   0x01
#define SIB_TYPE_1_NON_CRITICAL_EXT_V890_PRESENT_FLAG   0x02

typedef struct
{
    U16                          presence_bitmask;
    dynamic_str_t                late_noncritical_ext; /* O */
    sib_type_1_v920_ie_t          non_critical_ext_v920; /* O */
}sib_type_1_v890_ie_t;

#define SIB_TYPE_1_P_MAX_PRESENCE_FLAG               0x01
#define SIB_TYPE_1_TDD_CONFIG_PRESENT_FLAG           0x02
#define SIB_TYPE_1_NON_CRITICAL_EXT_PRESENT_FLAG     0x04

typedef struct
{
    U16                                    presence_bitmask;
    cell_access_related_info_t             cell_access_related_info;
    cell_selection_Info_t                  cell_selection_Info;
    S8                                     p_max; /* O */
    U8                                     freq_band_indicator;   
    U8                                     si_window_length;
    U8                                     si_value_tag;         
    scheduling_info_list_t                 scheduling_info_list; 
    tdd_config_t                           tdd_config; /* O */
    sib_type_1_v890_ie_t                   non_critical_ext_v890; /* O */
}sib_type_1_Info_t;

/* SIB1 Message structure End */

/* SIB2 Message structure Start */

#define SUB_FRAME_ALLOC_ONE_FRAME_OCTET_SIZE    1
#define SUB_FRAME_ALLOC_FOUR_FRAME_OCTET_SIZE   3
#define MAX_MBSFN_SUBFRAME_CONFIG           8

typedef struct
{
    U8        ac_barring_factor;
    U8        ac_barring_time;
    U8        ac_barring_for_special_ac; /* size 5 bit */
}access_class_barring_Info_t;

#define AC_BARR_INFO_MO_SIG_PRESENCE_FLAG       0x01
#define AC_BARR_INFO_MO_DATA_PRESENCE_FLAG      0x02

typedef struct
{
    U16                           presence_bitmask;              
    U8                            acBarringForEmergency;
    access_class_barring_Info_t   ac_barring_for_mo_signalling; /* O */
    access_class_barring_Info_t   ac_barring_for_mo_data; /* O */
}access_barring_info_t;

typedef struct
{
    U8        ra_preambles_group_a_size;
    U8        group_a_msg_size;
    U8        group_b_msg_power_offset;
}preambles_group_a_config_t;

#define PREAMBLE_INFO_GROUP_A_CONFIG_PRESENCE_FLAG      0x01

typedef struct
{
    U16                         presence_bitmask;       
    U8                          ra_preamble_count;
    preambles_group_a_config_t  preambles_group_a_config; /* O */
}preamble_info_t;

typedef struct
{
    U8       power_ramping_step;
    U8       preamble_init_rec_target_pow;
}power_ramping_params_t;

typedef struct
{
    U8        preamble_trans_max;
    U8        ra_resp_win_size;
    U8        mac_cont_resol_timer;
}ra_supervision_info_t;

typedef struct
{
    preamble_info_t             preamble_info;    
    power_ramping_params_t      power_ramping_params; 
    ra_supervision_info_t       ra_supervision_info; 
    U8                          max_harq_msg_3_tx;  
}rach_config_common_t;

typedef struct
{
    U8        mod_period_coeff;
}bcch_config_t;

typedef struct
{
    U8    default_paging_cycle;
    U8    nb; 
}pcch_config_t;

typedef struct
{
    U8        prach_config_index;
    U8        high_speed_flag; 
    U8        zero_cor_zone_config;
    U8        prach_freq_offset; 
}prach_config_info_t;

typedef struct
{
    U16                   root_seq_index; 
    prach_config_info_t   prach_config_info;
}prach_config_sib_t;

typedef struct
{
    S8        ref_signal_power;
    U8        pb;  
}pdsch_config_common_t;

typedef struct
{
    U8        nsb;               
    U8        hopping_mode;
    U8        pusch_hopping_offset;
    U8        enable_64_qam;      
}pusch_config_basic_t;

typedef struct
{
    U8      group_hopping_enabled;
    U8      group_assign_pusch; 
    U8      seq_hopping_enabled; 
    U8      cyclic_shift;       
}ul_ref_signals_pusch_t;

typedef struct
{
    pusch_config_basic_t      pusch_config_basic; 
    ul_ref_signals_pusch_t    ul_ref_signals_pusch;
}pusch_config_common_t;

typedef struct
{
    U8        delta_pucch_shift;
    U8        n_rb_cqi;  
    U8        n_cs_an;  
    U16       n_one_pucch_an;  
}pucch_config_common_t;

#define SOUNDING_RS_UL_CONFIG_COMMON_SETUP_SRS_MAX_UP_PTS_PRESENCE_FLAG     0x01

typedef struct
{
    U16       presence_bitmask;     
    U8        srs_bw_config;
    U8        srs_subframe_config;
    U8        ack_nack_srs_simul_trans;
    U8        srs_max_up_pts; /* O */
}sounding_rs_ul_config_common_setup_t;

#define SOUNDING_RS_UL_CONFIG_COMMON_SETUP_PRESENCE_FLAG      0x01
#define SOUNDING_RS_UL_CONFIG_COMMON_RELEASE_PRESENCE_FLAG    0x02

typedef struct
{
    U16                                  presence_bitmask;  
    sounding_rs_ul_config_common_setup_t setup; /* O */
}sounding_rs_ul_config_common_t;

typedef struct
{
    U8    delta_f_pucch_format_one;
    U8    delta_f_pucch_format_one_b;
    U8    delta_f_pucch_format_two;
    U8    delta_f_pucch_format_two_a;
    U8    delta_f_pucch_format_two_b;
}delta_f_list_pucch_t;

typedef struct
{
    S8                           p_zero_nominal_Pusch;
    U8                           alpha;
    S8                           p_zero_nominal_pucch;
    delta_f_list_pucch_t         delta_f_list_pucch;   
    S8                           delta_preamble_msg_three;
}uplink_power_control_common_t;

typedef struct
{
    rach_config_common_t            rach_config_common; 
    bcch_config_t                   bcch_config;       
    pcch_config_t                   pcch_config;      
    prach_config_sib_t              prach_config_sib;
    U8                              ul_cyc_prefix_len;
    pdsch_config_common_t           pdsch_config_common;
    pusch_config_common_t           pusch_config_common;
    pucch_config_common_t           pucch_config_common;
    sounding_rs_ul_config_common_t  sounding_rs_ul_config_common;
    uplink_power_control_common_t   uplink_power_control_common;
}radio_resource_config_common_sib_t;

typedef struct
{
    U8      timer_300;
    U8      timer_301;
    U8      timer_310;
    U8      timer_n310;
    U8      timer_311;
    U8      timer_n311;
}ue_timers_and_constants_t;


#define FREQ_INFO_UL_CARRER_FREQ_PRESENCE_FLAG      0x01
#define FREQ_INFO_UL_BW_PRESENCE_FLAG               0x02

typedef struct
{
    U16       presence_bitmask;      
    U16       ul_carrier_freq; /* O */
    U8        ul_bandwidth; /* O */
    U8        add_spectrum_emission;  
}freq_info_t;


#define SUBFRAME_ALLOCATION_ONE_FRAME                0X01
#define SUBFRAME_ALLOCATION_FOUR_FRAMES              0X02

typedef struct
{
    U16       presence_bitmask;                             
    U8        one_frame[SUB_FRAME_ALLOC_ONE_FRAME_OCTET_SIZE]; /* O */
    U8        four_frames[SUB_FRAME_ALLOC_FOUR_FRAME_OCTET_SIZE]; /* O */
}subframe_allocation_t;

typedef struct
{
    U8      radio_frame_alloc_period;
    U8      radio_frame_alloc_offset;   
    subframe_allocation_t      subframe_allocation;
}mbsfn_subframe_config_t;

typedef struct
{
    U8                        count;  
    mbsfn_subframe_config_t   mbsfn_subframe_config[MAX_MBSFN_SUBFRAME_CONFIG];
}mbsfn_subframe_config_list_t;

#define SIB2_ACCESS_BARR_INFO_PRESENCE_FLAG             0x01
#define SIB2_MBSFN_SUBFRAME_CONF_LIST_PRESENCE_FLAG     0x02
#define SIB_TYPE_2_LATE_NON_CRITICAL_EXT_PRESENT_FLAG   0x04
#define SIB_TYPE_2_SSAC_BARRING_VOICE_R9_PRESENT_FLAG   0x08
#define SIB_TYPE_2_SSAC_BARRING_VIDEO_R9_PRESENT_FLAG   0x10

typedef struct
{
    U16                                 presence_bitmask;
    access_barring_info_t               access_barring_info; /* O */
    radio_resource_config_common_sib_t  radio_resource_config_common_sib;
    ue_timers_and_constants_t           ue_timers_and_constants;  
    freq_info_t                         freq_info;
    mbsfn_subframe_config_list_t        mbsfn_subframe_config_list; /* O */
    U8                                  time_align_timer;
    dynamic_str_t                       late_noncritical_ext; /* O */
    access_class_barring_Info_t         ssac_barring_for_mmtel_voice_r9; /* O */
    access_class_barring_Info_t         ssac_barring_for_mmtel_video_r9; /* O */

}sib_type_2_Info_t;


/* SIB2 Message structure End */

/* SIB3 Message structure Start */

typedef struct
{
    U8  sf_medium;
    U8  sf_high; 
} speed_state_scale_factors_t;

#define INTRA_FREQ_CELL_RESELECT_P_MAX_PRESENCE_FLAG            0x01
#define INTRA_FREQ_CELL_RESELECT_INTRA_SEARCH_PRESENCE_FLAG     0x02
#define INTRA_FREQ_CELL_RESELECT_MEAS_BW_PRESENCE_FLAG          0x04
#define INTRA_FREQ_CELL_RESELECT_EUTRAN_SF_PRESENCE_FLAG        0x08

typedef struct
{
    U16       presence_bitmask; 
    S8        q_rx_lev_min;    
    S8        p_max; /* O */
    U8        s_intra_search; /* O */
    U8        measure_bw; /* O */ 
    U8        presence_antenna_port_1;  
    U8        neigh_cell_config;       
    U8        t_reselec_eutra;        
    speed_state_scale_factors_t  t_reselect_eutra_sf; /* O */
}intra_freq_cell_reselect_info_t;

#define CELL_RESELECT_NON_INTRA_SEARCH_PRESENCE_FLAG            0x01

typedef struct
{
    U16       presence_bitmask;     
    U8        s_non_intra_search; /* O */
    U8        thresh_serving_low;  
    U8        cell_reselect_priority; 
}cell_reselect_serv_freq_info_t;

typedef struct
{
    U8        t_evaluation;
    U8        t_hyst_normal;
    U8        n_cell_change_medium;   
    U8        n_cell_change_high;    
}mobility_state_params_t;

typedef struct
{
    U8        q_hyst_sf_medium;
    U8        q_hyst_sf_high; 
}speed_depend_scaling_param_hyst_t;

typedef struct
{
    mobility_state_params_t           mobility_state_params;
    speed_depend_scaling_param_hyst_t speed_depend_scaling_param_hyst;
}speed_depend_reselect_t;

#define CELL_RESELECT_SPEED_DEPEND_RESELECT_PRESENCE_FLAG       0x01

typedef struct
{
    U16                      presence_bitmask;      
    U8                       q_hyst;
    speed_depend_reselect_t  speed_depend_reselect; /* O */
}cell_reselect_info_comm_t;

typedef struct
{
    U8         s_intra_search_p_r9;
    U8         s_intra_search_q_r9;
}s_intra_search_v920_t;

typedef struct
{
    U8         s_non_intra_search_p_r9;
    U8         s_non_intra_search_q_r9;
}s_non_intra_search_v920_t;

#define SIB_TYPE_3_LATE_NON_CRITICAL_EXT_PRESENT_FLAG   0x01
#define INTRA_SEARCH_V920_PRESENCE_FLAG                 0x02
#define NON_INTRA_SEARCH_V920_PRESENCE_FLAG             0x04
#define QUALMIN_R9_PRESENCE_FLAG                        0x08
#define RESELECTION_THRESHOLDQ_R9_PRESENCE_FLAG         0x10

typedef struct
{
    U16                             presence_bitmask; 
    cell_reselect_info_comm_t       timecell_reselect_info_comm;
    cell_reselect_serv_freq_info_t  cell_reselect_serv_freq_info;
    intra_freq_cell_reselect_info_t intra_freq_cell_reselect_info;
    dynamic_str_t                   late_noncritical_ext; /* O */
    s_intra_search_v920_t           s_intra_search_v920; /* O */
    s_non_intra_search_v920_t       s_non_intra_search_v920; /* O */
    S8                              q_qualmin_r9; /* O */
    U8                              reselection_thresholdq_r9; /* O */ 
}sib_type_3_Info_t;

/* SIB3 Message structure End */

/* SIB4 Message structure Start */

typedef U16             phys_cell_id_t;

#define MAX_CELL_INTRA            16
#define MAX_CELL_BLACK_LIST       16

#define PHY_CELL_ID_RANGE_PRESENCE_FLAG         0x01

typedef struct
{
    U16                 presence_bitmask;
    phys_cell_id_t  start;          
    U8                  range; /* O */ 
}phy_cell_id_range_t;

typedef struct
{
    U8                    count;   
    phy_cell_id_range_t   black_listed_cell[MAX_CELL_BLACK_LIST];
}black_listed_cell_list_t;

typedef struct
{
    phys_cell_id_t    phys_cell_id;    
    U8                    q_offset_cell;

}neigh_cell_t;

typedef struct
{
    U8            count;                      
    neigh_cell_t  neigh_cell[MAX_CELL_INTRA]; 
}neigh_cell_list_t;

#define SIB_4_NEIGH_CELL_LIST_PRESENCE_FLAG             0x01
#define SIB_4_BLACK_LIST_LIST_PRESENCE_FLAG             0x02
#define SIB_4_PHY_CELL_ID_PRESENCE_FLAG                 0x04
#define SIB_4_LATE_NON_CRITICAL_EXT_PRESENT_FLAG        0x08

typedef struct
{
    U16                       presence_bitmask;              
    neigh_cell_list_t         intra_freq_neigh_cell_list; /* O */
    black_listed_cell_list_t  intra_freq_black_listed_cell_list; /* O */
    phy_cell_id_range_t       csg_phy_cell_id_range; /* O */
    dynamic_str_t             late_noncritical_ext; /* O */
}sib_type_4_Info_t;

/* SIB4 Message structure End */

/* SIB5 Message structure start */

#define MAX_EUTRA_CARRIER_FREQ              8

typedef struct
{
    U8                        thresh_x_high_q_r9; 
    U8                        thresh_x_low_q_r9; 

}thresh_x_q_r9_t;

#define INTER_FREQ_CARR_P_MAX_PRESENCE_FLAG                     0x01
#define INTER_FREQ_CARR_SCALE_PARAM_PRESENCE_FLAG               0x02
#define INTER_FREQ_CARR_CELL_RESELCT_PRIOR_PRESENCE_FLAG        0x04
#define INTER_FREQ_CARR_NEIGH_CELL_LIST_PRESENCE_FLAG           0x08
#define INTER_FREQ_CARR_BLACK_LIST_CELL_LIST_PRESENCE_FLAG      0x10
#define INTER_FREQ_CARR_Q_QUAL_MIN_R9_PRESENCE_FLAG             0x20
#define INTER_FREQ_THRESH_X_Q_R9_PRESENCE_FLAG                  0x40

typedef struct
{
    U16                             presence_bitmask;        
    U32                             eutra_dl_carrier_freq;   
    S8                              qrx_lev_min;
    S8                              p_max; /* O */
    U8                              t_reselection_eutran;
    speed_state_scale_factors_t     speed_depend_scal_params; /* O */
    U8                              thresh_x_high; 
    U8                              thresh_x_low;
    U8                              measurement_bandwidth;
    U8                              presence_antenna_port_1;
    U8                              cell_reselect_priority; /* O */
    U8                              neigh_cell_config; /* 2 bit */
    U8                              q_offset_freq;
    neigh_cell_list_t               inter_freq_neigh_cell_list; /* O */
    black_listed_cell_list_t        inter_freq_black_listed_cell_list; /* O */
    S8                              q_qual_min_r9; /* O */
    thresh_x_q_r9_t                 thresh_x_q_r9; /* O */
}inter_freq_carrier_freq_t;

typedef struct
{
    U8                              count; 
    inter_freq_carrier_freq_t       inter_freq_carrier_freq_list[MAX_EUTRA_CARRIER_FREQ];

}inter_freq_carrier_freq_list_t;

#define SIB_5_LATE_NON_CRITICAL_EXT_PRESENT_FLAG        0x01

typedef struct
{
    U16                              presence_bitmask; 
    inter_freq_carrier_freq_list_t   inter_freq_carrier_freq_list;
    dynamic_str_t                    late_noncritical_ext; /* O */

}sib_type_5_Info_t;

#define MAX_UTRA_FDD_CARRIER  10
#define MAX_UTRA_TDD_CARRIER  10
#define MAX_SIZE_OF_STR_HNB   48
typedef struct 
{
    U8 threshX_highQ_r9;
    U8 threshX_LowQ_r9;
}carrier_freq_UTRA_FDD_threshX_Q_r9_t;

#define CARRIER_FREQ_UTRA_FDD_CELL_RESELECTION_PRIORITY 0x01
#define CARRIER_FREQ_UTRA_FDD_THRESHX_Q_R9              0x02

typedef struct 
{ 
    U16                                     presence_bitmask;
    U16                                     carrier_freq;
    U8                                      cell_reselection_priority;/* optional */
    U8                                      thresh_X_High;
    U8                                      thresh_X_Low;
    S8                                      q_Rx_Lev_Min;
    S8                                      p_Max_UTRA;
    S8                                      q_Qual_min;
    carrier_freq_UTRA_FDD_threshX_Q_r9_t    thresh_X_Q_r9; /* optional */
    /* optional  extElem1 Not supported */
}carrier_freq_UTRA_FDD_t;

#define CARRIER_FREQ_UTRA_TDD_CELL_RESELECTION_PRIORITY 0x01
typedef struct 
{ 
    U16                                 presence_bitmask;
    U16                                 carrier_freq;
    U8                                  cell_reselection_priority;/* optional */
    U8                                  thresh_X_High;
    U8                                  thresh_X_Low;
    S8                                  q_Rx_Lev_Min;
    S8                                  p_Max_UTRA;
    /* optional  extElem1 Not supported */
}carrier_freq_UTRA_TDD_t;

typedef struct 
{
    U8                       count; 
    carrier_freq_UTRA_FDD_t  carrier_freq_UTRA_FDD_data[MAX_UTRA_FDD_CARRIER];
}carrier_freq_list_UTRA_FDD_t;

typedef struct 
{
    U8                       count; 
    carrier_freq_UTRA_TDD_t  carrier_freq_UTRA_TDD_data[MAX_UTRA_TDD_CARRIER];
}carrier_freq_list_UTRA_TDD_t;

#define CARRIER_FREQ_LIST_UTRA_FDD 0x01
#define CARRIER_FREQ_LIST_UTRA_TDD 0x02
#define T_RESELECTION_UTRA_SF         0x04
typedef struct 
{
    U16                             presence_bitmask;
    carrier_freq_list_UTRA_FDD_t    carrier_freq_list_UTRA_FDD;
    carrier_freq_list_UTRA_TDD_t    carrier_freq_list_UTRA_TDD;
    U8                              t_reselection_UTRA;                               /* optional */
    speed_state_scale_factors_t     t_reselection_UTRA_SF;
    /* optional lateNonCriticalExtension and extElem1 Not supported */
}sib_type_6_Info_t;

typedef struct 
{
    U8                  count;
    U8                  hnb_Name[MAX_SIZE_OF_STR_HNB];
    /* optional lateNonCriticalExtension and extElem1 Not supported */
}sib_type_9_Info_t;


/* SIB5 Message structure End */

typedef struct
{
    U16                         presence_bitmask; 
    sib_type_1_Info_t           sib_type_1_Info;      
    sib_type_2_Info_t           sib_type_2_Info; 
    sib_type_3_Info_t           sib_type_3_Info; 
    sib_type_4_Info_t           sib_type_4_Info; 
    sib_type_5_Info_t           sib_type_5_Info; 
    sib_type_6_Info_t           sib_type_6_Info; 
    sib_type_9_Info_t           sib_type_9_Info; 
}sib_messages_t; 

typedef struct
{
    U16                         presence_bitmask;
    mib_info_t                  mib_info;
}mib_messages_t;

U8 sib_and_si_decoder (U8 *encoded_sib_or_si_msg, sib_messages_t *decoded_sibs_msg,  U16 encoded_sib_or_si_len);

U8 mib_decoder (U8 *encoded_mib_msg, mib_messages_t *decoded_mib_msg,  U16 encoded_mib_len);

U8 sib_and_si_mem_free (sib_messages_t *decoded_sibs_msg);

#endif
