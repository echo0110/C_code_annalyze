/*! \file son_oam_intf.h
 *  \brief This file contains the OAM interface API structures.
 *  \date January 13, 2012
 *  \author gur25767
 */

/*!
 *  \copyright {Copyright (c) 2009, Aricent Inc. All Rights Reserved}
 */

#ifndef __SON_OAM_INTF__
#define __SON_OAM_INTF__

/*
 * Header File Includes
 */

/*! \  headerfile son_api_defines.h <> */
#include "son_api_defines.h"

typedef enum
{
    SON_NOT_OK,
    SON_OK
}son_module_sup_et;

/*
 * SON_OAM_INIT_CONFIG_REQ
 */

/*
 * SON_OAM_ANR_ENABLE_REQ
 */

/*! \ son_neighbor_handover_stats_t
 *  \brief Neighbor Handover Statistics
 *  \param handovers_attempted  - Ho's Attempted
 *  \param handovers_succesful  - Ho's Successful 
 *  \param handovers_failed     - Ho's Failed
 */
typedef struct _son_neighbor_handover_stats_t
{
    son_u8  handovers_attempted;    /*^ M, 0, N, 0, 0 ^*/
    son_u8  handovers_succesful;    /*^ M, 0, N, 0, 0 ^*/
    son_u8  handovers_failed;       /*^ M, 0, N, 0, 0 ^*/
}son_neighbor_handover_stats_t;

#define SON_CELL_GLOBAL_ID_1X_RTT_SIZE    6
#define SON_CELL_GLOBAL_ID_HRPD_SIZE                       16

/*! \ son_cdma_global_cell_id_t
 *  \brief CDMA Global Cell ID
 *  \param bitmask                  - Bitmask value
 *  \param cell_global_id_1X_RTT    - 1X_RTT Global cell id
 *  \param cell_global_id_HRPD      - HRPD Global cell id
 */
#define SON_CGI_1X_RTT_PRESENT 0x01
#define SON_CGI_HRPD_PRESENT 0x02
typedef struct _son_cdma_global_cell_id_t
{
    son_u32                               bitmask; /*^ BITMASK ^*/
    son_u8                 cell_global_id_1X_RTT [SON_CELL_GLOBAL_ID_1X_RTT_SIZE];
    /*^ O, SON_CGI_1X_RTT_PRESENT, OCTET_STRING, FIXED ^*/
    son_u8                 cell_global_id_HRPD [SON_CELL_GLOBAL_ID_HRPD_SIZE];
    /*^ O, SON_CGI_HRPD_PRESENT, OCTET_STRING, FIXED ^*/
}son_cdma_global_cell_id_t;

/*! \ son_oam_pz_hyst_params_included_t
 *  \brief CDMA Hysteresis Feature
 *  \param bitmask                  - Bitmask value
 *  \param pz_hyst_enabled          - Hysteresis feature to be enabled
 *  \param pz_hyst_info_incl        - Hysteresis information included
 *  \param pz_hyst_act_timer        - Hysteresis activity timer 
 *  \param pz_hyst_timer_mul        - Hysteresis timer multiplier 
 *  \param pz_hyst_timer_exp        - Hysteresis timer exponent
 *  \param pz_hyst_list_len         - Hysteresis list length 
 */
#define SON_OAM_PZ_HYST_ENABLED_PRESENT     0x01
#define SON_OAM_PZ_HYST_INFO_INCL_PRESENT   0x02
#define SON_OAM_PZ_HYST_TIMER_PRESENT       0x04
#define SON_OAM_PZ_HYST_TIMER_MUL_PRESENT   0x08
#define SON_OAM_PZ_HYST_TIMER_EXP_PRESENT   0x10
#define SON_OAM_PZ_HYST_LIST_LEN_PRESENT    0x20
typedef struct _son_oam_pz_hyst_params_included_t
{
    son_u32 bitmask;    
    /*^BITMASK ^*/
    son_u8  pz_hyst_enabled;
    /*^ O, SON_OAM_PZ_HYST_ENABLED_PRESENT, H, 0, 1 ^*/
    son_u8  pz_hyst_info_incl;
    /*^ O, SON_OAM_PZ_HYST_INFO_INCL_PRESENT, H, 0, 1 ^*/
    son_u8  pz_hyst_act_timer;
    /*^ O, SON_OAM_PZ_HYST_TIMER_PRESENT, N, 0, 0 ^*/
    son_u8  pz_hyst_timer_mul;
    /*^ O, SON_OAM_PZ_HYST_TIMER_MUL_PRESENT, H, 0, 7 ^*/
    son_u8  pz_hyst_timer_exp;
    /*^ O, SON_OAM_PZ_HYST_TIMER_EXP_PRESENT, H, 0, 31 ^*/
    son_u8 pz_hyst_list_len;
    /*^ O, SON_OAM_PZ_HYST_LIST_LEN_PRESENT, H, 0, 1 ^*/
}son_oam_pz_hyst_params_included_t;

/*! \ son_oam_fpc_fch_included_t
 *  \brief CDMA Fundamental channels params
 *  \param bitmask                  - Bitmask value
 *  \param fpc_fch_init_setpt_rc3   - Initial Fundamental channel RC3 
 *  \param fpc_fch_init_setpt_rc4   - Initial Fundamental channel RC4
 *  \param fpc_fch_init_setpt_rc5   - Initial Fundamental channel RC5
 */
#define SON_OAM_FPC_FCH_INCL_RC3_PRESENT    0x01
#define SON_OAM_FPC_FCH_INCL_RC4_PRESENT    0x02
#define SON_OAM_FPC_FCH_INCL_RC5_PRESENT    0x04
typedef struct _son_oam_fpc_fch_included_t
{
    son_u32 bitmask;
    /*^ BITMASK ^*/
    son_u8  fpc_fch_init_setpt_rc3;
    /*^ O, SON_OAM_FPC_FCH_INCL_RC3_PRESENT, N, 0, 0 ^*/
    son_u8  fpc_fch_init_setpt_rc4;
    /*^ O, SON_OAM_FPC_FCH_INCL_RC4_PRESENT, N, 0, 0 ^*/
    son_u8  fpc_fch_init_setpt_rc5;
    /*^ O, SON_OAM_FPC_FCH_INCL_RC5_PRESENT, N, 0, 0 ^*/
}son_oam_fpc_fch_included_t;

/*! \ son_cdma_cell_specific_params_t 
 *  \brief CDMA Cell specific params
 *  \param bitmask                  - Bitmask value
 *  \param pz_hyst_params_included  - Hysteresis params included
 *  \param p_rev                    - Protocol revision level 
 *  \param min_p_rev                - Minimum mobile station protocol revision level
 *  \param neg_slot_cycle_index_sup - Set to 1, if it supports negative values of the preferred slot cycle index
 *  \param encrypt_mode             - Message encryption mode(00 - Encryption disabled,
 *                                    01 - Basic encryption of call control messages,
 *                                    10 - Enhanced encryption of call control messages,
 *                                    11 - Extended encryption of call control messages)
 *  \param encrypt_suported         - Set to 1 if the encryption related fields are included
 *  \param sig_encrypt_suported     - Indicates which signaling encryption algorithms are supported by the
 *                                    base station. If ENC_SUPPORTED is equal to 1, the base station shall
 *                                    include this field
 *  \param msg_integrity_sup        - If the base station supports message integrity, the base station
 *                                    shall set this field to 1 
 *  \param sig_integrity_sup_incl   - If the base station supports other integrity algorithm(s)
 *                                    in addition to the default integrity algorithm, the base station
 *                                    shall set this field to 1
 *  \param sig_integrity_sup        - Indicates the supported message integrity algorithms in
 *                                    addition to the default integrity algorithm 
 *  \param ms_init_pos_loc_sup_ind  - If the base station supports mobile station initiated position
 *                                    determination, the base station shall set this field to 1
 *  \param band_class_info_req      - The base station shall set this field to 1 if the ALT_BAND_CLASS
 *                                    field is included in this message 
 *  \param band_class               - CDMA2000 band class 
 *  \param alt_band_class           - The base station shall set this field to an alternate
 *                                    CDMA band class supported by the base station 
 *  \param tkz_mode_supported       - The base station shall set this field to 1 if the tracking
 *                                    zone mode is supported 
 *  \param tkz_id                   - If TKZ_MODE_SUPPORTED is set to 1, the base station shall
 *                                    set this field to its tracking zone identifier
 *  \param fpc_fch_included         - Fundamental channel included 
 *  \param t_add                    - This value is used by the mobile station to trigger the 
 *                                    transfer of a pilot from the Neighbor Set or Remaining Set 
 *                                    to the Candidate Set and to trigger the sending of the 
 *                                    Pilot Strength Measurement Message or Extended Pilot Strength 
 *                                    Measurement Message initiating the handoff process 
 *  \param pilot_inc                - A mobile station searches for Remaining-Set pilots at 
 *                                    pilot PN sequence index values that are multiples of this value 
 */
#define SON_CDMA_CSP_PZ_HYST_PARAMS_INCLUDED_PRESENT    0x01
#define SON_CDMA_CSP_P_REV_PRESENT                      0x02
#define SON_CDMA_CSP_MIN_P_REV_PRESENT                  0x04
#define SON_CDMA_CSP_NEG_SLOT_CYCLE_INDEX_PRESENT       0x08
#define SON_CDMA_CSP_ENCRYPT_MODE_PRESENT               0x10
#define SON_CDMA_CSP_ENCRYPT_SUPPORTED_PRESENT          0x20
#define SON_CDMA_CSP_SIG_ENCRYPT_SUPPORTED_PRESENT      0x40
#define SON_CDMA_CSP_MSG_INT_SUPPORTED_PRESENT          0x80
#define SON_CDMA_CSP_SIG_INT_SUPPORTED_INCL_PRESENT     0x100
#define SON_CDMA_CSP_SIG_INT_SUPPORTED_PRESENT          0x200
#define SON_CDMA_CSP_MS_INIT_POS_LOC_SUP_IND_PRESENT    0x400
#define SON_CDMA_CSP_BAND_CLASS_INFO_REQ_PRESENT        0x800
#define SON_CDMA_CSP_BAND_CLASS_PRESENT                 0x1000
#define SON_CDMA_CSP_ALT_BAND_CLASS_PRESENT             0x2000
#define SON_CDMA_CSP_TKZ_MODE_SUP_PRESENT               0x4000
#define SON_CDMA_CSP_TKZ_ID_PRESENT                     0x8000
#define SON_CDMA_CSP_FPC_FCH_INCLUDED                   0x10000
#define SON_CDMA_CSP_T_ADD                              0x20000
#define SON_CDMA_CSP_PILOT_INC_PRESENT                  0x40000
typedef struct _son_cdma_cell_specific_params_t
{
    son_u32     bitmask;
    /*^ BITMASK ^*/
    son_oam_pz_hyst_params_included_t   pz_hyst_params_included;
    /*^ O, SON_CDMA_CSP_PZ_HYST_PARAMS_INCLUDED_PRESENT, N, 0, 0 ^*/
    son_u8      p_rev;
    /*^ O, SON_CDMA_CSP_P_REV_PRESENT, N, 0, 0 ^*/
    son_u8    min_p_rev;
    /*^ O, SON_CDMA_CSP_MIN_P_REV_PRESENT, N, 0, 0 ^*/
    son_u8  neg_slot_cycle_index_sup;
    /*^ O, SON_CDMA_CSP_NEG_SLOT_CYCLE_INDEX_PRESENT, H, 0, 1 ^*/
    son_u8  encrypt_mode;
    /*^ O, SON_CDMA_CSP_ENCRYPT_MODE_PRESENT, N, 0, 0 ^*/
    son_u8  encrypt_suported;
    /*^ O, SON_CDMA_CSP_ENCRYPT_SUPPORTED_PRESENT, H, 0, 1 ^*/
    son_u8  sig_encrypt_suported;
    /*^ O, SON_CDMA_CSP_SIG_ENCRYPT_SUPPORTED_PRESENT, N, 0, 0 ^*/
    son_u8 msg_integrity_sup;
    /*^ O, SON_CDMA_CSP_MSG_INT_SUPPORTED_PRESENT, H, 0, 1 ^*/
    son_u8 sig_integrity_sup_incl;
    /*^ O, SON_CDMA_CSP_SIG_INT_SUPPORTED_INCL_PRESENT, H, 0, 1 ^*/
    son_u8  sig_integrity_sup;
    /*^ O, SON_CDMA_CSP_SIG_INT_SUPPORTED_PRESENT, N, 0, 0 ^*/
    son_u8  ms_init_pos_loc_sup_ind;
    /*^ O, SON_CDMA_CSP_MS_INIT_POS_LOC_SUP_IND_PRESENT, H, 0, 1 ^*/
    son_u8  band_class_info_req;
    /*^ O, SON_CDMA_CSP_BAND_CLASS_INFO_REQ_PRESENT, H, 0, 1 ^*/
    son_cdma_band_class_et  band_class;
    /*^ O, SON_CDMA_CSP_BAND_CLASS_PRESENT, H, 0, 31 ^*/
    son_cdma_band_class_et  alt_band_class;
    /*^ O, SON_CDMA_CSP_ALT_BAND_CLASS_PRESENT, H, 0, 31^*/
    son_u8  tkz_mode_supported;
    /*^ O, SON_CDMA_CSP_TKZ_MODE_SUP_PRESENT, H, 0, 1 ^*/
    son_u8  tkz_id;
    /*^ O, SON_CDMA_CSP_TKZ_ID_PRESENT, H, 0, 1 ^*/
    son_oam_fpc_fch_included_t  fpc_fch_included;
    /*^ O, SON_CDMA_CSP_FPC_FCH_INCLUDED, N, 0, 0 ^*/
    son_u8 t_add;
    /*^ O, SON_CDMA_CSP_T_ADD, H, 0, 63 ^*/
    son_u8  pilot_inc;
    /*^ O, SON_CDMA_CSP_PILOT_INC_PRESENT, H, 0, 15 ^*/
}son_cdma_cell_specific_params_t;

/*! \ son_cdma_neighbor_info_t
 *  \brief CDMA Neighbor Info
 *  \param bitmask              - Bitmask value
 *  \param nbr_cgi              - Neighbor Cell CGI 
 *  \param pci                  - Physical Cell Id 
 *  \param band_class           - This field indicates the CDMA2000 band in which the
 *                                CDMA2000 carrier frequency can be found  
 *  \param arfcn                - This field indicates the CDMA2000 carrier frequency within a CDMA2000 band
 *  \param cell_specific_offset - Cell Specific Offset
 *  \param pilot_pn_phase       - Indicates the arrival time of a CDMA2000 pilot, measured relative 
 *                                to the UEs time reference in units of PN chips
 *  \param pilot_strength       - CDMA2000 Pilot Strength, the ratio of pilot power to total power
 *                                in the signal bandwidth of a CDMA2000 Forward Channel
 *  \param nr_status            - Indicates whether the ANR function at eNB is allowed to 
 *                                autonomously remove this cell from the neighbor relation table
 *  \param ho_status            - Indicates whether HO is allowed or not to this target cell
 *  \param handover_stats       - Indicates the handover statistics
 *  \param broadcast_status     - Indicates if the NBR needs to be broadcasted
 *  \param cell_specific_params - Indicates CDMA cell specific params
 */
#define SON_CDMA_PCI_PRESENT                    0x01
#define SON_CDMA_BAND_CLASS_PRESENT             0x02
#define SON_CDMA_BAND_CLASS_ARFCN_PRESENT       0x04
#define SON_CDMA_CSO_PRESENT                    0x08
#define SON_CDMA_PILOT_PN_PHASE_PRESENT         0x10
#define SON_CDMA_PILOT_STRENGTH_PRESENT         0x20
#define SON_CDMA_ENABLE_NR_STATUS_PRESENT       0x40
#define SON_CDMA_ENABLE_HO_STATUS_PRESENT       0x80
#define SON_CDMA_NR_HO_STATS_PRESENT            0x100
#define SON_CDMA_BROADCAST_MODE_PRESENT         0x200
#define SON_CDMA_CELL_SPECIFIC_PARAMS_PRESENT   0x400
typedef struct _son_cdma_neighbor_info_t
{
    son_u32                             bitmask; /*^ BITMASK ^*/
    son_cdma_global_cell_id_t           nbr_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_u16                             pci;
    /*^ O, SON_CDMA_PCI_PRESENT, H, 0, 511 ^*/

    son_cdma_band_class_et              band_class;
    /*^ O, SON_CDMA_BAND_CLASS_PRESENT ,H, 0,31 ^*/

    son_u16                             arfcn;
    /*^ O, SON_CDMA_BAND_CLASS_ARFCN_PRESENT, H, 0, 2047 ^*/

    son_s8                              cell_specific_offset; 
    /*^ O, SON_CDMA_CSO_PRESENT, B, -15, 15 ^*/

    son_u16                             pilot_pn_phase;
    /*^ O, SON_CDMA_PILOT_PN_PHASE_PRESENT,H, 0,32767 ^*/

    son_u8                              pilot_strength;
    /*^ O, SON_CDMA_PILOT_STRENGTH_PRESENT,H, 0,63 ^*/

    son_nr_status_et                    nr_status;
    /*^ O, SON_CDMA_ENABLE_NR_STATUS_PRESENT, B, 1, 2 ^*/

    son_ho_status_et                    ho_status;
    /*^ O, SON_CDMA_ENABLE_HO_STATUS_PRESENT, B, 1, 2 ^*/

    son_neighbor_handover_stats_t       handover_stats;
    /*^ O, SON_CDMA_NR_HO_STATS_PRESENT, N, 0, 0 ^*/

    son_bool_et                         broadcast_status;
    /*^ O, SON_CDMA_BROADCAST_MODE_PRESENT, H, 0, 1 ^*/

    son_cdma_cell_specific_params_t     cell_specific_params;
    /*^ O, SON_CDMA_CELL_SPECIFIC_PARAMS_PRESENT, N, 0, 0 ^*/
}son_cdma_neighbor_info_t;

#define SON_OAM_MODULE_LOG_LEVEL_PRESENT 0x01

/*! \  son_oam_module_log_config_t 
 *  \brief   Contains initial log configuration for the SON internal modules
 *  \param   bitmask  -   It has primarily been added for future extensions 
 *  \param   log_on_off -  Specifies to Enable or Disable Logging of the 
 *           specified module
 *  \param   log_level    -  log level to be set for logging
 */
typedef struct _son_oam_module_log_config_t
{
    son_u32                    bitmask;      /*^ BITMASK ^*/
    son_oam_log_on_off_et      log_on_off;   /*^ M, 0, H, 0, 1 ^*/
/* coverity_17493_start */
    son_oam_log_level_et       log_level;    /*^ O, SON_OAM_MODULE_LOG_LEVEL_PRESENT, H, 0, 6 ^*/
/* coverity_17493_stop */
}son_oam_module_log_config_t;

/*! \  son_oam_module_init_config_t 
 *  \brief   contains initial configuration for the SON internal modules
 *  \param   bitmask  -   It has primarily been added for future extensions 
 *  \param   module_id  -  SON Internal Module ID 
 *  \param   log_configuration  -  specifies log_level and log enable/disable 
 */
typedef struct _son_oam_module_init_config_t
{
    son_u32                        bitmask;      /*^ BITMASK ^*/
    son_module_id_et               module_id;    /*^ M, 0, B, 25701, 25708 ^*/
    son_oam_module_log_config_t    log_configuration;  /*^ M, 0, N, 0, 0 ^*/
}son_oam_module_init_config_t;

#define SON_INIT_CONFIG_PRESENT                 0x01
#define SON_HEALTH_UPD_TMR_PERIOD_PRESENT       0x02
/*SPR-12944-Start*/
#define SON_NMM_SUPPORT_PRESENT                 0x04
/*SPR-12944-End*/

/*! \  son_oam_init_config_req_t
 *  \brief   Initial Configuration Request received from OAM for SON modules
 *  \param   bitmask  -  0x0001 to indicate if the individual module 
 *           configurations are present in the API message or not.
 *  \param   int_module_config -  contains initial configuration reqd by SON 
 *           internal modules
 *  \param   health_upd_tmr_period  - health update timer period 
 */
typedef struct _son_oam_init_config_req_t
{
    son_u32                      bitmask;  /*^ BITMASK ^*/
    son_oam_module_init_config_t int_module_config[SON_MAX_NO_INT_MODULES];
    /*^ O, SON_INIT_CONFIG_PRESENT, OCTET_STRING, FIXED ^*/
    son_u16                      health_upd_tmr_period;
    /*^ O, SON_HEALTH_UPD_TMR_PERIOD_PRESENT, B, 5, 900 ^*/

    /*SPR-12944-Start*/
    son_bool_et                  nmm_supported;
    /*^ O, SON_NMM_SUPPORT_PRESENT, H, 0, 1 ^*/
    /*SPR-12944-End*/
} son_oam_init_config_req_t;    /*^ API, SON_OAM_INIT_CONFIG_REQ ^*/
   
/* SPR-13251 Fix Starts */
/*! \ son_oam_get_log_level_req_t
 *  \brief  Request from OAM to get the log level of SON internal modules
 *  \param bitmask - Bitmask Value
 *  \param module_id - Internal Module ID (In case module Id is zero then
 *                     log level will be given for all the SON modules)
 */
typedef struct _son_oam_get_log_level_req_t
{
    son_u32             bitmask;  /*^ BITMASK ^*/
    son_module_id_et    module_id;    /*^ M, 0, N, 0, 0 ^*/
}son_oam_get_log_level_req_t;       /*^ API, SON_OAM_GET_LOG_LEVEL_REQ ^*/

#define SON_MODULE_LOG_LEVEL_PRESENT    0x01
/*! \ son_oam_module_log_level_t
 *  \brief Structure storing log level of SON module
 *  \param bitmask - Bitmask Value
 *  \param module_id - Internal Module ID 
 *  \param error_code - Error Cause. If it is set to no error then only log_level value is present
 *  \param log_level - Log level of the module
 */
typedef struct _son_oam_module_log_level_t
{
    son_u32                 bitmask;        /*^ BITMASK ^*/
    son_module_id_et        module_id;      /*^ M, 0, B, 25701, 25708 ^*/
    son_error_et            error_code;     /*^ M, 0, N, 0, 0 ^*/
/* coverity_94834_start */
    son_oam_log_level_et    log_level;    
    /*^ O, SON_MODULE_LOG_LEVEL_PRESENT, H, 0, 6 ^*/
/* coverity_94834_stop */
}son_oam_module_log_level_t;

/*! \ son_oam_get_log_level_resp_t
 *  \brief Response from SON to OAM which gives the log levels of SON internal modules
 *  \param bitmask - Bitmask Value
 *  \param result - SUCCESS/FAILURE
 *  \param error_code - Cause (valid in case of Failure)
 *  \param num_of_modules - Number of SON modules
 *  \param module_log_level - Structure storing log level of SON module
 */
typedef struct _son_oam_get_log_level_resp_t
{
    son_u32                     bitmask;  /*^ BITMASK ^*/
    son_return_et               result;     /*^ M, 0, H, 0, 2 ^*/ 
    son_error_et                error_code; /*^ M, 0, N, 0, 0 ^*/
    son_u8                      num_of_modules; /*^ M, 0, H, 0, 8 ^*/
    son_oam_module_log_level_t  module_log_level[SON_MAX_NO_INT_MODULES]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_get_log_level_resp_t;    /*^ API, SON_OAM_GET_LOG_LEVEL_RESP ^*/
/* SPR-13251 Fix Ends */

/*
 * SON_OAM_INIT_CONFIG_RESP
 */

/*! \  son_oam_init_config_resp_t
 *  \brief   Response to SON_OAM_INIT_CONFIG_REQ
 *  \param   result     -     Success/Failure
 *  \param   error_code -     Cause (valid in case of Failure)
 */
typedef struct _son_oam_init_config_resp_t
{
    son_return_et               result;     /*^ M, 0, H, 0, 1 ^*/ 
    son_error_et                error_code; /*^ M, 0, N, 0, 0 ^*/
}son_oam_init_config_resp_t;    /*^ API, SON_OAM_INIT_CONFIG_RESP ^*/

/*
 *   SON_OAM_SHUTDOWN_REQ
 */

/*! \  son_oam_shutdown_req_t
 *  \brief   Request from OAM to shutdown SON. In case of graceful shutdown, 
 *           SON modules free their contexts and in case of forced shutdown, 
 *           SON immediately shuts down.
 *  \param   shutdown_type    -  Shutdown type: Forced/Graceful 
 *  \param   time_to_shutdown -  Time duration for shutdown
 */
typedef struct _son_oam_shutdown_req_t
{
    son_shutdown_type_et    shutdown_type;       /*^ M, 0, H, 0, 1 ^*/
    son_timer_duration_t    time_to_shutdown;    /*^ M, 0, N, 0, 0 ^*/
}son_oam_shutdown_req_t;    /*^ API, SON_OAM_SHUTDOWN_REQ ^*/

/*
 *   SON_OAM_SHUTDOWN_RESP
 */

/*! \  son_oam_shutdown_resp_t
 *  \brief   Shutdown response send from SON to OAM in case of failure only.
 *  \param   result     -     Success/Failure
 *  \param   error_code -     Cause (valid in case of Failure)
 */
typedef struct _son_oam_shutdown_resp_t
{
    son_return_et               result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et                error_code; /*^ M, 0, N, 0, 0 ^*/
}son_oam_shutdown_resp_t;    /*^ API, SON_OAM_SHUTDOWN_RESP ^*/

/*
 *   SON_OAM_SET_LOG_LEVEL_REQ
 */

/*! \  son_oam_set_log_level_req_t
 *  \brief   Request from OAM to set the log level of SON internal module
 *  \param   module_id - Internal Module ID (In case module Id is zero then 
 *           log level will be applied to all the SON modules)
 *  \param   log_level - log level to be set
 */
typedef struct _son_oam_set_log_level_req_t
{
    son_module_id_et        module_id;    /*^ M, 0, N, 0, 0 ^*/
/* coverity_17523_start */
    son_oam_log_level_et    log_level;    /*^ M, 0, H, 0, 6 ^*/
/* coverity_17523_stop */
}son_oam_set_log_level_req_t;    /*^ API, SON_OAM_SET_LOG_LEVEL_REQ ^*/

/*
 *   SON_OAM_SET_LOG_LEVEL_RESP
 */

/*! \     son_oam_set_log_level_resp_t
 *  \brief      Response from SON to OAM for SON_OAM_SET_LOG_LEVEL_REQ request
 *  \param      result   -     SUCCESS/FAILURE
 *  \param      error_code -     Cause (valid in case of Failure)
 */
typedef struct _son_oam_set_log_level_resp_t
{
    son_return_et               result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et                error_code; /*^ M, 0, N, 0, 0 ^*/
}son_oam_set_log_level_resp_t;    /*^ API, SON_OAM_SET_LOG_LEVEL_RESP ^*/

/*
 *   SON_OAM_LOG_ENABLE_DISABLE_REQ
 */

/*! \  son_oam_log_enable_disable_req_t
 *  \brief   Request from OAM to enable/disable the SON logging at a specified 
 *           module or globally for all the SON modules.
 *  \param   module_id - Internal Module ID (In case module Id is zero then 
 *           logging will be enabled/disabled for all the SON modules)
 *  \param   log_configuration - Specifies to Enable or Disable Logging
 *  \param   feature_id         - SON internal feature ID. 
 */
typedef struct _son_oam_log_enable_disable_req_t
{
    son_module_id_et            module_id;          /*^ M, 0, N, 0, 0 ^*/    
    son_oam_module_log_config_t log_configuration;  /*^ M, 0, N, 0, 0 ^*/  
    son_feature_id_et           feature_id;         /*^ M, 0, H, 0, 3 ^*/    
} son_oam_log_enable_disable_req_t; /*^ API, SON_OAM_LOG_ENABLE_DISABLE_REQ ^*/

/*
 *   SON_OAM_LOG_ENABLE_DISABLE_RESP
 */

/*! \  son_oam_log_enable_disable_resp_t
 *  \brief   Response from SON to OAM for SON_OAM_LOG_ENABLE_DISABLE_REQ request
 *  \param   result     -   SUCCESS/FAILURE
 *  \param   error_code -   Cause (valid in case of Failure)
 */
typedef struct _son_oam_log_enable_disable_resp_t
{
    son_return_et               result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et                error_code; /*^ M, 0, N, 0, 0 ^*/
} son_oam_log_enable_disable_resp_t; /*^ API, SON_OAM_LOG_ENABLE_DISABLE_RESP ^*/

/*! \  son_cell_plmn_info_t
 *  \brief   This structure stores the PLMN info
 *  \param   mcc[SON_MCC_OCTET_SIZE]      Mobile Country Code
 *  \param   num_mnc_digit                This can be 2 or 3
 *  \param   mnc[SON_MAX_MNC_OCTET_SIZE]  Mobile Network Code
 */
typedef struct _son_cell_plmn_info_t
{
    son_u8         mcc[SON_MCC_OCTET_SIZE];    /*^ M, 0, OCTET_STRING, FIXED ^*/
/*SPR_17664_start*/
    son_u8         num_mnc_digit;              /*^ M, 0, B, 2, SON_MAX_MNC_OCTET_SIZE ^*/
/*SPR_17664_end*/
    son_u8         mnc[SON_MAX_MNC_OCTET_SIZE];
                   /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_cell_plmn_info_t;

/*! \  struct son_plmn_identity_info_t
 *  \brief Structure containing PLMN identity info
 *  \param plmn_id         MCC and MNCs
 *  \param cell_res_for_operator_use            Cell reserved/unreserved
 */

typedef struct _son_plmn_identity_info_t
{
    son_cell_plmn_info_t    plmn_id;
    /*^M,0,N,0,0^*/
    son_u8                  cell_res_for_operator_use;
    /*^ M, 0, N, 0, 0 ^*/
}son_plmn_identity_info_t;

/*! \  son_intra_rat_global_cell_id_t
 *  \brief Global Cell ID for EUTRAN 
 *  \param plmn_id       - PLMN Id 
 *  \param cell_identity - Cell Identity
 */
typedef struct _son_intra_rat_global_cell_id_t
{
    son_cell_plmn_info_t   plmn_id; /*^ M, 0, N, 0, 0 ^*/
    son_u8                 cell_identity[SON_CELL_ID_OCTET_SIZE];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
}son_intra_rat_global_cell_id_t;

/*! \  son_utran_geran_global_cell_id_t
 *  \brief Global Cell ID of UTRAN and GERAN
 *  \param rat_type      - RAT type
 *  \param plmn_id       - PLMN Id 
 *  \param cell_identity - Cell Identity
 */
typedef struct _son_utran_geran_global_cell_id_t
{
    son_rat_type_et        rat_type;       /*^ M, 0, N, 1, 3 ^*/
    son_cell_plmn_info_t   plmn_id;        /*^ M, 0, N, 0, 0 ^*/
    son_u8                 cell_identity[SON_CELL_ID_OCTET_SIZE];
    /*^ M, 0, OCTET_STRING, FIXED ^*/ 
}son_utran_geran_global_cell_id_t;

/*! \   son_others_global_cell_id_t
 *  \brief Global Cell ID for other RATs 
 *  \param bitmask - Bitmask Value
 */
typedef struct _son_others_global_cell_id_t
{
    son_u32             bitmask;  /* For Future use */
    /*  To Be Defined Later */
}son_others_global_cell_id_t;

#define SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT        0x01
#define SON_OTHERS_GLOBAL_CELL_ID_PRESENT             0x02
#define SON_CDMA_GLOBAL_CELL_ID_PRESENT               0x04

/*! \   son_inter_rat_global_cell_id_t
 *  \brief Inter RAT Global Cell ID
 *  \param bitmask 
 *  \param utran_geran_global_cell_id  - Global Cell ID for GERAN,UTRAN
 *  \param others_global_cell_id       - Global Cell ID for OTHERS (Future use)
 *  \param son_cdma_global_cell_id_t   - Global Cell ID for CDMA
 */
typedef struct _son_inter_rat_global_cell_id_t
{
    son_u32                            bitmask;    /*^ BITMASK ^*/
    
    son_utran_geran_global_cell_id_t         utran_geran_global_cell_id; 
    /*^ O, SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT, N, 0, 0 ^*/
    
    son_others_global_cell_id_t        others_global_cell_id;  
    /*^ O, SON_OTHERS_GLOBAL_CELL_ID_PRESENT, N, 0, 0 ^*/

    son_cdma_global_cell_id_t           cdma_global_cell_id;
    /*^ O, SON_CDMA_GLOBAL_CELL_ID_PRESENT, N, 0, 0 ^*/
}son_inter_rat_global_cell_id_t;

#define SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT            0x01
#define SON_INTER_RAT_GLOBAL_CELL_ID_PRESENT            0x02

/*! \   son_global_cell_id_t
 *  \brief  Global Cell ID of EUTRAN/UTRAN_GERAN/OTHERS cell types 
 *  \param bitmask           
 *  \param intra_rat_global_cell_id    -   Global cell id for eutran cell type
 *  \param inter_rat_global_cell_id    -   Global cell id for utran, geran and 
 *                                         others cell types
 */
typedef struct _son_global_cell_id_t
{
    son_u32                          bitmask;    /*^ BITMASK ^*/
    
    son_intra_rat_global_cell_id_t   intra_rat_global_cell_id;    
    /*^ O, SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT, N, 0, 0 ^*/
    
    son_inter_rat_global_cell_id_t   inter_rat_global_cell_id;    
    /*^ O, SON_INTER_RAT_GLOBAL_CELL_ID_PRESENT, N, 0, 0 ^*/
}son_global_cell_id_t;

/*! \ son_special_subframe_info_t
 *  \brief Special subframe configuration information
 *  \param  special_subframe_patterns   - Special subframe patterns
 *  \param  dl_cp                       - Downlink Cyclic Prefix
 *  \param  ul_cp                       - Uplink Cyclic Prefix
 */
typedef struct
{
    son_u8      special_subframe_patterns; /*^ M,0,H,0,8 ^*/
    son_u8      dl_cp; /*^ M,0,H,0,1 ^*/
    son_u8      ul_cp; /*^ M,0,H,0,1 ^*/
}son_special_subframe_info_t;

/*! \  son_fdd_info_t
 *  \brief  FDD Mode Information
 *  \param  ul_earfcn   - Uplink Carrier Frequency Channel identifier
 *  \param  dl_earfcn   - Downlink  Carrier Frequency Channel identifier
 *  \param  ul_trnsmission_bdwidth  - Uplink Bandwidth
 *  \param  dl_trnsmission_bdwidth  - Downlink Bandwidth
 */
typedef struct _son_fdd_info_t
{
    son_u16     ul_earfcn; /*^ M,0,N,0,0 ^*/
    son_u32     dl_earfcn; /*^ M,0,N,0,0 ^*/
    son_u8      ul_trnsmission_bdwidth; /*^ M,0,H,0,5 ^*/
    son_u8      dl_trnsmission_bdwidth; /*^ M,0,H,0,5 ^*/
}son_fdd_info_t;

/*! \  son_tdd_info_t
 *  \brief TDD Mode Information
 *  \param  earfcn                  - Carrier Frequency Channel identifier
 *  \param  trnsmission_bdwidth     - Tx Bandwidth
 *  \param  subframe_assn           - Special subframe Assignment
 *  \param  special_subframe_info   - Special subframe configuration information
 */
typedef struct _son_tdd_info_t
{
    son_u32                         earfcn;    /*^ M,0,N,0,0 ^*/
    son_u8                          trnsmission_bdwidth; /*^ M,0,H,0,5 ^*/
    son_u8                          subframe_assn; /*^ M,0,H,0,6 ^*/
    son_special_subframe_info_t     special_subframe_info;     /*^ M,0,N,0,0 ^*/
}son_tdd_info_t;

#define    SON_EUTRA_MODE_FDD   0x01
#define    SON_EUTRA_MODE_TDD   0x02

/*! \  son_choice_eutra_mode_t
 *  \brief   Son EUTRAN Mode Information
 *  \param  bitmask
 *  \param  fdd_info    - FDD Mode Specific information
 *  \param  tdd_info    - TDD Mode Specific information
 */ 
typedef struct _son_choice_eutra_mode_t
{
    son_u32            bitmask;   /*^ BITMASK ^*/
    son_fdd_info_t     fdd_info;  /*^ O,SON_EUTRA_MODE_FDD,N,0,0 ^*/
    son_tdd_info_t     tdd_info;  /*^ O,SON_EUTRA_MODE_TDD,N,0,0 ^*/
}son_choice_eutra_mode_t;

/*SPR-9574-Start*/

/*! \son_per_freq_pci_range_t
 *  \brief  Per frequency pci range Information
 *  \param  pci_start    - Starting pci value of the pci range
 *  \param  pci_range    - pci range
 */
typedef struct _son_per_freq_pci_range_t
{ 
    son_u16                      pci_start;
    /*^ M, 0, H, 0, 503 ^*/
    son_oam_cell_id_range_et     pci_range;  
    /*^ M, 0, H, 0, 13 ^*/
}son_per_freq_pci_range_t;

/* Spr 17753 Changes Start */
/*! \son_x2_second_degree_conflict_config_t
 *  \brief  Second degree conflict configuration  
 *  \param  second_degree_conflict_enable    - Enable/Disable conflict detection with second degree nbr 
 *  \param  nr_add_wait_timer   	     - Timer Value for NR add req from OAM
 *  \param  conflict_detection_backoff_timer - Timer Value for conflict back off timer
 */
typedef struct _son_x2_second_degree_conflict_config_t
{
    son_bool_et             second_degree_conflict_enable;
    /*^ M, 0, H, 0, 1 ^*/
    son_timer_duration_t    nr_add_wait_timer;
    /*^ M, 0, N, 0, 0 ^*/
    son_timer_duration_t    conflict_detection_backoff_timer;
    /*^ M, 0, N, 0, 0 ^*/
}son_x2_second_degree_conflict_config_t;
/* Spr 17753 Changes End */
/*SPR-9574-End*/

/** eICIC changes start */
/*! \son_oam_abs_threshold_usage_t
 * \brief SON ABS usage threshold minimum and maximum info.
 * \param bitmask 
 * \param min_abs_usage_threshold Below this threshold SON will start increasing cell range expansion
 * \param max_abs_usage_threshold Above this threshold SON will stop increasing Cell range expansion
 */
#define SON_CELL_MIN_ABS_USAGE_THRESHOLD_PRESENT    0x0001
#define SON_CELL_MAX_ABS_USAGE_THRESHOLD_PRESENT    0x0002
typedef struct _son_oam_abs_threshold_usage_t
{
    son_u32                     bitmask;                    /*^ BITMASK ^*/
    son_u8                      min_abs_usage_threshold;    /*^ O, SON_CELL_MIN_ABS_USAGE_THRESHOLD_PRESENT, H, 0, 100 ^*/
    son_u8                      max_abs_usage_threshold;    /*^ O, SON_CELL_MAX_ABS_USAGE_THRESHOLD_PRESENT, H, 0, 100 ^*/
}son_oam_abs_threshold_usage_t;
/* ! \son_oam_eicic_info_t
 *  \brief SON OAM eICIC information for ABS usage and interface scheme 
 *  \param  bitmask     bitmask for eicic information
 *  \param  interference_scheme interface scheme for eICIC from OAM.
 *  \param  abs_usage_threshold ABS usage threshold for CRE algo
 */
#define SON_CELL_INTF_SCHEME_TYPE_PRESENT           0x0001
#define SON_CELL_ABS_USAGE_THRESHOLD_PRESENT        0x0002
typedef struct _son_oam_eicic_info_t
{
    son_u32                         bitmask;                /*^ BITMASK ^*/
    son_intf_scheme_type_et         interference_scheme;    /*^ O, SON_CELL_INTF_SCHEME_TYPE_PRESENT, H, 0, 2 ^*/
    son_oam_abs_threshold_usage_t   abs_usage_threshold;    /*^ O, SON_CELL_ABS_USAGE_THRESHOLD_PRESENT, N, 0, 0 ^*/
}son_oam_eicic_info_t;
/** eICIC changes end */

#define SON_CELL_TAC_PRESENT                                0x0001
#define SON_CELL_PLMN_PRESENT                               0x0002
#define SON_CELL_EUTRA_MODE_PRESENT                         0x0004
#define SON_CELL_CSG_IDENTITY_PRESENT                       0x0008
#define SON_CELL_PREAMBLE_INITIAL_RCVD_TARGET_POWER_PRESENT 0x0010
#define SON_CELL_NUMBER_OF_RA_PREAMBLES_PRESENT             0x0020
#define SON_CELL_SIZE_OF_RA_GROUP_A_PRESENT                 0x0040
#define SON_CELL_PRACH_CONFIG_PRESENT                       0x0080
#define SON_CELL_PUSCH_HOPPING_OFFSET_PRESENT               0x0100
#define SON_CELL_INTRA_FREQ_OPEN_PCI_RANGE_PRESENT          0x0200
#define SON_CELL_INTRA_FREQ_CLOSED_PCI_RANGE_PRESENT        0x0400
#define SON_CELL_INTRA_FREQ_HYBRID_PCI_RANGE_PRESENT        0x0800
#define SON_CELL_ABS_USAGE_THRESHOLD_RANGE                  0x1000
/* Spr 17753 Changes Start */
#define SON_CELL_SECOND_DEGREE_CONFLICT_CONFIG_PRESENT      0x2000
/* Spr 17753 Changes End */
/*
 *  SON_OAM_CELL_INFO_IND    
 */

/*! \  son_prach_configuration_t
 *  \brief This structure contains lte cell physical layer PRACH parameters
 *  \param root_sequence_index          Logical root sequence index used to determine 64 physical RACH preamble sequences available in the cell.
 *  \param zero_correlation_zone_config This parameter is used for preamble sequence generation
 *  \param high_speed_flag              Determines whether unrestricted set (0:FALSE) or restricted set (1:TRUE) of preambles is used
 *  \param prach_frequency_offset       The first physical resource block available for PRACH expressed as a physical resource block number.
 *  \param prach_configuration_index    Provides index into the table defining PRACH resources within the frame. Mandatory for TDD, shall not be present for FDD.
 */
#define SON_PRACH_CONFIG_INDEX_PRESENT                  0x01
typedef struct _son_prach_configuration
{
    son_u32 bitmask;                        
    /*^ BITMASK ^*/
    son_u16 root_sequence_index;            
    /*^ M, 0, H, 0, 837 ^*/
    son_u8  zero_correlation_zone_config;   
    /*^ M, 0, H, 0, 15 ^*/
    son_u8  high_speed_flag;                
    /*^ M, 0, H, 0, 1 ^*/
    son_u8  prach_frequency_offset;         
    /*^ M, 0, H, 0, 94 ^*/
    son_u8  prach_configuration_index;      
    /*^ O, SON_PRACH_CONFIG_INDEX_PRESENT, H, 0, 63 ^*/
}son_prach_configuration_t;

/*! \  son_cell_t
 *  \brief   Configuration for cell
 *  \param   bitmask 
 *  \param   src_cgi                ECGI of the cell
 *  \param   pci                    PCI of the cell
 *  \param   tac                    TAC value
 *  \param   num_plmn_id            number of plmns
 *  \param   plmn_id                Plmn id list
 *  \param   choice_eutra_mode      eutra mode(FDD/TDD info)
 *  \param   csg_identity           Closed subscriber group id used to determine eNB type
 *  \param   preamble_initial_received_target_power Denotes baseline for computation of transmit power for random access transmission parameters
 *  \param   number_of_ra_preambles Number of non dedicated random access preambles (Contention based)
 *  \param   size_of_ra_group_a     Number of non dedicated random access preamble group A 
 *  \param   prach_config           LTE cell physical layer PRACH parameters     
 *  \param   pusch_hopping_offset   This is used to compute the set of physical resources blocks to be used for transmission on PUSCH if Uplink freq hopping mode is enabled
 *  \param   intra_freq_open_pci_range    Open Intra frequency pci range   //Added for SPR-9574
 *  \param   intra_freq_closed_pci_range  Closed Intra frequency pci range //Added for SPR-9574
 *  \param   intra_freq_hybrid_pci_range  Hybrid Intra frequency pci range //Added for SPR-9574
// *  \param   sub_frame_assignment   Defines the DL/UL subframe configuration
 *  \param   abs_usage_threshold    ABS usage threshold maximum and minimum
 */
typedef struct _son_cell_t
{
    son_u32                                 bitmask;    /*^ BITMASK ^*/
    
    son_intra_rat_global_cell_id_t          src_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_u16                                 pci;
    /*^ M, 0, H, 0, 503 ^*/
    son_u8                                  tac[SON_TAC_OCTET_SIZE];
    /*^ O, SON_CELL_TAC_PRESENT, OCTET_STRING, FIXED ^*/

    son_u8                                  num_plmn_id;
/*SPR_17664_start*/
    /*^ O, SON_CELL_PLMN_PRESENT, B, 1, SON_MAX_NUM_PLMN^*/
/*SPR_17664_end*/
    son_cell_plmn_info_t                    plmn_id[SON_MAX_NUM_PLMN];
    /*^ O, SON_CELL_PLMN_PRESENT, OCTET_STRING, VARIABLE ^*/

    son_choice_eutra_mode_t                 choice_eutra_mode;
    /*^ O,SON_CELL_EUTRA_MODE_PRESENT,N,0,0 ^*/

    son_u8                                  csg_identity[SON_CSG_ID_OCTET_SIZE];
    /*^ O,SON_CELL_CSG_IDENTITY_PRESENT,OCTET_STRING, FIXED ^*/

    son_preamble_init_rec_target_pow_et     preamble_initial_rcvd_target_power; 
    /*^ O, SON_CELL_PREAMBLE_INITIAL_RCVD_TARGET_POWER_PRESENT, H, 0, 15 ^*/
   
    son_oam_ra_preamble_count_et            number_of_ra_preambles;
    /*^ O, SON_CELL_NUMBER_OF_RA_PREAMBLES_PRESENT, H, 0, 15 ^*/
              
    son_oam_ra_preambles_group_a_size_et    size_of_ra_group_a;
    /*^ O, SON_CELL_SIZE_OF_RA_GROUP_A_PRESENT, H, 0, 14 ^*/

    son_prach_configuration_t               prach_config;
    /*^ O, SON_CELL_PRACH_CONFIG_PRESENT, N, 0, 0 ^*/
 
    son_u8                                  pusch_hopping_offset;
    /*^ O, SON_CELL_PUSCH_HOPPING_OFFSET_PRESENT, H, 0, 98 ^*/

    son_per_freq_pci_range_t                intra_freq_open_pci_range;
    /*^ O, SON_CELL_INTRA_FREQ_OPEN_PCI_RANGE_PRESENT, N, 0, 0 ^*/

    son_per_freq_pci_range_t                intra_freq_closed_pci_range;
    /*^ O, SON_CELL_INTRA_FREQ_CLOSED_PCI_RANGE_PRESENT, N, 0, 0 ^*/

    son_per_freq_pci_range_t                intra_freq_hybrid_pci_range;
    /*^ O, SON_CELL_INTRA_FREQ_HYBRID_PCI_RANGE_PRESENT, N, 0, 0 ^*/
/** eICIC changes start */
    son_oam_abs_threshold_usage_t           abs_usage_threshold;
    /*^O, SON_CELL_ABS_USAGE_THRESHOLD_RANGE, N, 0, 0 ^*/
/** eICIC changes end */
/* Spr 17753 Changes Start */
    son_x2_second_degree_conflict_config_t   son_x2_second_degree_conflict_config;
    /*^ O, SON_CELL_SECOND_DEGREE_CONFLICT_CONFIG_PRESENT, N, 0, 0 ^*/
/* Spr 17753 Changes End */
}son_cell_t;


/*! \  son_oam_cell_info_ind_t
 *  \brief   Cell configuration received from OAM for SON
 *  \param   cell_info_size         No. of cells for which the information is coming
 *  \param   serving_cell           List of CGIs of cells
 */
typedef struct _son_oam_cell_info_ind_t
{
/*SPR_17664_start*/
    son_u8                           cell_info_size;    /*^ M, 0, B, 1, SON_MAX_CELL_SUPPORTED_AT_ENB ^*/
/*SPR_17664_end*/
    son_cell_t                       serving_cell[SON_MAX_CELL_SUPPORTED_AT_ENB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} son_oam_cell_info_ind_t;    /*^ API, SON_OAM_CELL_INFO_IND ^*/

/* SPR_19619 start */
#define HOME_ENB_ID_OCTET_SIZE          	    4
#define SON_ES_PEER_CELL_ACTIVATION_PRESENT     0x01
    #define MAX_SERVED_CELLS                        8

/*
 * SON_OAM_PEER_CELL_ACTIVATION_REQ
 */
/*! \  son_oam_peer_cell_activation_req_t 
 *  \  brief Handle Request from OAM for Peer Cell Activation
 */
typedef struct _son_oam_peer_cell_activation_req_t
{
    son_u32     			                bitmask;        /*^ BITMASK ^*/	 
    U16                         	        num_served_cell;/*^ M, 0, H, 1, MAX_SERVED_CELLS ^*/
    son_intra_rat_global_cell_id_t          served_cell_list[MAX_SERVED_CELLS];/*^ M,0,OCTET_STRING,VARIABLE ^*/
}son_oam_peer_cell_activation_req_t;/*^ API, SON_OAM_PEER_CELL_ACTIVATION_REQ ^*/

typedef struct _son_oam_peer_cell_activation_resp_t
{
    son_return_et                   result;     /*^ M, 0, H, 0, 2 ^*/
    son_error_et                    error_code; /*^ M, 0, N, 0, 0 ^*/
}son_oam_peer_cell_activation_resp_t;/*^ API, SON_OAM_PEER_CELL_ACTIVATION_RESP ^*/


/* SPR_19619 stop */


/* Start: CSR 00053250 */
/*! \  son_meas_bandwidth_pci_per_earfcn_t
 *  \brief   This structure stores bandwidth corresponding to each EARFCN
 *  \param   earfcn                    - EARFCN Id
 *  \param   meas_bandwidth_for_earfcn - Measurement bandwidth for the EARFCN
 *  \param num_pci                     - The number of cells in the 
 *                                       pciList. If 0 all cells on the 
 *                                       carrier should  be found.
 *  \param pci                         - The list of cells which should be
 *                                       searched
 */
typedef struct _son_meas_bandwidth_pci_per_earfcn_t 
{
    son_u32   earfcn;                    /*^ M, 0, N, 0, 0 ^*/
    son_u8    meas_bandwidth_for_earfcn; /*^ M, 0, B, 6, 100 ^*/
    son_u16     num_pci;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_MAX_NO_CELLS_PER_EARFCN ^*/
/*SPR_17664_end*/
    son_u16     pci[SON_MAX_NO_CELLS_PER_EARFCN];    
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_meas_bandwidth_pci_per_earfcn_t;

/*! \  son_meas_bandwidth_earfcn_list_t
 *  \brief   This structure stores the Bandwidth information list for EARFCNs
 *  \param   num_earfcn                - Number of elements present in the 
 *                                       EARFCN bandwidth information list
 *  \param   meas_bandwidth_per_earfcn - List containing the measurement  
 *                                       bandwidth,pci for each EARFCN
 */
typedef struct _son_meas_bandwidth_earfcn_list_t
{
/*SPR_17664_start*/
   son_u16                            num_earfcn;    /*^ M, 0, B, 1, SON_MAX_NO_EARFCN ^*/
/*SPR_17664_end*/
   son_meas_bandwidth_pci_per_earfcn_t 
           meas_bandwidth_per_earfcn[SON_MAX_NO_EARFCN];
           /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_meas_bandwidth_earfcn_list_t;
/*End: CSR 00053250 end */
/*
 *  SON_OAM_NW_SCAN_REQ
 */

#define SON_MEAS_BANDWINDTH_PER_EARFCN_PRESENT     0x01
#define SON_MEAS_BANDWINDTH_PRESENT                0x02
#define SON_NW_SCAN_PLMN_ID_PRESENT                0x04
/*! \ son_start_intra_rat_scan_req_t
 *  \brief   This structure stores the Scan Request parameters                  
 *  \param bitmask - Only one of SON_MEAS_BANDWINDTH_PER_EARFCN_PRESENT and
 *                   SON_MEAS_BANDWINDTH_PRESENT is expected. Measurement BW 
 *                   when the OAM does not specify any EARFCN (all carriers in 
 *                   the band shall be measured), and Measurement BW per EARFCN 
 *                   when the EARFCNs are specified.
 *  \param measurement_period               - Time period. 1 unit is 10ms.
 *  \param retry_count                      - The number of SI windows for 
 *                                            which decoding of a SI should be 
 *                                            retried.The retries are counted 
 *                                            per siPeriodicity.
 *  \param freq_band_id                     - The E-UTRA band for which the 
 *                                            carrier List applies. 
 *  \param meas_bandwidth_per_earfcn        - list containing meas_bandwidth 
 *                                            for each earfcn to be measured
 *  \param meas_bandwidth                   - The number of resource blocks 
 *                                            which should be used for 
 *                                            measuring RSRP. This is global 
 *                                            meas_bandwidth, in this request
 *                                            either global meas_bandwidth or 
 *                                            per earfcn meas_bandwidth should 
 *                                            be present.
 *  \param num_plmn_id                 - The number of PLMNs
 *  \param plmn_id                     - The MCCs and MNCs
 */
typedef struct _son_start_intra_rat_scan_req_t
{           
    son_u32     bitmask;             /*^ BITMASK ^*/

    son_u8      measurement_period;  /*^ M, 0, L, 1, 255 ^*/
    
    son_u8      retry_count;         /*^ M, 0, B, 1, 8 ^*/
    
    son_u8      freq_band_id;        /*^ M, 0, B, 1, 64 ^*/
    
    son_meas_bandwidth_earfcn_list_t        meas_bandwidth_per_earfcn;
    /*^ O, SON_MEAS_BANDWINDTH_PER_EARFCN_PRESENT,  N, 0, 0 ^*/
    
    son_u8      meas_bandwidth;        
    /*^ O, SON_MEAS_BANDWINDTH_PRESENT, B, 6, 100 ^*/
    
    son_u8      num_plmn_id;
/*SPR_17664_start*/
    /*^ O, SON_NW_SCAN_PLMN_ID_PRESENT, B, 1, SON_MAX_NUM_PLMN ^*/    
/*SPR_17664_end*/
    son_cell_plmn_info_t plmn_id[SON_MAX_NUM_PLMN];
    /*^ O, SON_NW_SCAN_PLMN_ID_PRESENT, OCTET_STRING, VARIABLE ^*/    
    
}son_start_intra_rat_scan_req_t;

#define SON_MEAS_UTRAN_BANDWINDTH_PER_ARFCN_PRESENT   0x01
#define SON_MEAS_UTRAN_BANDWINDTH_PRESENT             0x02

/*! \ son_start_utra_scan_req_t
 *  \brief   This structure stores the Scan Request parameters
 *  \param bitmask - Only one of SON_MEAS_UTRAN_BANDWINDTH_PER_ARFCN_PRESENT and
 *                   SON_MEAS_UTRAN_BANDWINDTH_PRESENT is expected. Measurement BW
 *                   when the OAM does not specify any UARFCN (all carriers in
 *                   the band shall be measured), and Measurement BW per ARFCN
 *                   when the UARFCNs are specified.
 *  \param measurement_period               - Time period. 1 unit is 10ms.
 *  \param retry_count                      - The number of SI windows for
 *                                            which decoding of a SI should be
 *                                            retried.The retries are counted
 *                                            per siPeriodicity.
 *  \param freq_band_id                     - The UTRA band for which the
 *                                            carrier List applies.
 *  \param meas_bandwidth_per_arfcn        - list containing meas_bandwidth
 *                                            for each earfcn to be measured
 *  \param meas_bandwidth                   - The number of resource blocks
 *                                            which should be used for
 *                                            measuring RSCP. This is global
 *                                            meas_bandwidth, in this request
 *                                            either global meas_bandwidth or
 *                                            per earfcn meas_bandwidth should
 *                                            be present.
 *  \param num_pci                          - The number of cells in the
 *                                            pciList. If 0 all cells on the
 *                                            carrier should  be found.
 *  \param pci                              - The list of cells which should be
 *                                            searched
 */
typedef struct _son_start_utra_scan_req_t
{
    son_u32      bitmask;  /*^ BITMASK ^*/

    son_u8      measurement_period;  /*^ M, 0, L, 1, 255 ^*/

    son_u8      retry_count;         /*^ M, 0, B, 1, 8 ^*/

    son_u8      freq_band_id;        /*^ M, 0, B, 1, 64 ^*/

    son_meas_bandwidth_earfcn_list_t        meas_bandwidth_per_arfcn;
    /*^ O, SON_MEAS_UTRAN_BANDWINDTH_PER_ARFCN_PRESENT,  N, 0, 0 ^*/

    son_u8      meas_bandwidth;
    /*^ O, SON_MEAS_UTRAN_BANDWINDTH_PRESENT, B, 6, 100 ^*/

/*SPR_17664_start*/
    son_u16     num_pci;        /*^ M, 0, H, 0, SON_MAX_NO_CELLS_PER_EARFCN ^*/
/*SPR_17664_end*/
    son_u16     pci[SON_MAX_NO_CELLS_PER_EARFCN];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_start_utra_scan_req_t;

/*! \ son_start_geran_scan_req_t
 *  \brief   This structure stores the Scan Request parameters for GERAN
 *  \param bitmask - Bitmask Value 
 */
typedef struct _son_start_geran_scan_req_t
{
    son_u32  bitmask;  /* For Future Use */
        /* To Be Defined Later */
}son_start_geran_scan_req_t;

/*! \ son_start_cdma_scan_req_t
 *  \brief   This structure stores the Scan Request parameters for CDMA
 *  \param bitmask - Bitmask Value
 */
typedef struct _son_start_cdma_scan_req_t
{
    son_u32  bitmask;  /* For Future Use */
        /* To Be Defined Later */
}son_start_cdma_scan_req_t;

#define SON_UTRA_SCAN_REQ_PRESENT     0x01
#define SON_GERAN_SCAN_REQ_PRESENT    0x02
#define SON_CDMA_SCAN_REQ_PRESENT     0x04

/*! \ struct  son_start_inter_rat_scan_req_t
 *  \brief   For future Release 
 *  \param   bitmask
 *  \param   start_utra_scan_req
 *  \param   start_geran_scan_req
 *  \param   start_cdma_scan_req
 */
typedef struct _son_start_inter_rat_scan_req_t
{
    son_u32                               bitmask;     /*^ BITMASK ^*/
    son_start_utra_scan_req_t             start_utra_scan_req;
    /*^ O, SON_UTRA_SCAN_REQ_PRESENT, N, 0, 0 ^*/
    son_start_geran_scan_req_t            start_geran_scan_req;
    /*^ O, SON_GERAN_SCAN_REQ_PRESENT, N, 0, 0 ^*/
    son_start_cdma_scan_req_t             start_cdma_scan_req;
    /*^ O, SON_CDMA_SCAN_REQ_PRESENT, N, 0, 0 ^*/
}son_start_inter_rat_scan_req_t;

#define SON_INTRA_RAT_NW_SCAN_REQ_PRESENT    0x01
#define SON_INTER_RAT_NW_SCAN_REQ_PRESENT    0x02

/*! \ son_oam_nw_scan_req_t
 *  \brief    Network Scan Request for SON
 *  \param    bitmask
 *  \param    start_intra_rat_scan_req - For LTE
 *  \param    start_inter_rat_scan_req - FOR UTRAN, GERAN and CDMA
 */
typedef struct _son_oam_nw_scan_req_t 
{
    son_u32                          bitmask;       /*^ BITMASK ^*/
    son_start_intra_rat_scan_req_t   start_intra_rat_scan_req; 
    /*^ O, SON_INTRA_RAT_NW_SCAN_REQ_PRESENT, N, 0, 0 ^*/
    son_start_inter_rat_scan_req_t   start_inter_rat_scan_req; 
    /*^ O, SON_INTER_RAT_NW_SCAN_REQ_PRESENT, N, 0, 0 ^*/
}son_oam_nw_scan_req_t; /*^ API, SON_OAM_NW_SCAN_REQ ^*/

/*! \  son_rf_cell_num_t
 *  \brief   structure storing RF cell info (PCI, RSRP, RSRQ)
 *  \param   pci_id - This field contains the pci Id of cells which should be 
 *                    searched
 *  \param   rsrq   - The calculated .Reference Signal Received Quality. as 
 *                    per the spec 36.214
 *  \param   rsrp   - The measured .Reference signal received power. by the L1 
 *                    N/W Scan 
 */
typedef struct _son_rf_cell_num_t
{
    son_u16            pci_id;  /*^ M, 0, H, 0, 503 ^*/    
    son_u8             rsrq;    /*^ M, 0, H, 0, 34 ^*/
    son_u8             rsrp;    /*^ M, 0, H, 0, 128 ^*/
}son_rf_cell_num_t;

#define SON_CSG_ID_PRESENT         0x01
#define SON_BCCH_PLMN_ID_PRESENT   0x02

/*! \ son_bcch_cell_num_t
 *  \brief structure storing BCCH cell info
 *  \param bitmask
 *  \param cell_Id - Cell Identity read from SIB-1
 *  \param cell_barred
 *  \param csg_identity                   - CSG Identity from SIB-1
 *  \param num_plmn_id                    - number of plmns
 *  \param plmn_identity_info             - PLMN identity info
 *  \param tac                            - Tracking Area Code
 *  \param rsTxPower                      - Calculated as: meas_bw*RSRP/RSSI
 *  \param dl_sys_bw                      - Num of DL RBs(6,....,100)
 *  \param ul_sys_bw                      - Num of UL RBs
 */
typedef struct _son_bcch_cell_num_t
{
    son_u32        bitmask;    /*^ BITMASK ^*/

    son_u8        cell_Id[SON_CELL_ID_OCTET_SIZE];    
    /*^ M, 0, OCTET_STRING, FIXED ^*/

    son_u8        cell_barred;    
    /*^ M, 0, N, 0, 0 ^*/

    son_u8        csg_indication;    
    /*^ M, 0, N, 0, 0 ^*/

    son_u8        csg_identity[SON_CSG_ID_OCTET_SIZE];    
    /*^ O, SON_CSG_ID_PRESENT, OCTET_STRING, FIXED ^*/

    son_u8        num_plmn_id;
/*SPR_17664_start*/
    /*^ O, SON_BCCH_PLMN_ID_PRESENT, B, 1, SON_MAX_NUM_PLMN ^*/
/*SPR_17664_end*/
    son_plmn_identity_info_t        plmn_identity_info[SON_MAX_NUM_PLMN];
    /*^ O, SON_BCCH_PLMN_ID_PRESENT, OCTET_STRING, VARIABLE ^*/

    son_u8 tac[SON_TAC_OCTET_SIZE];   /*^ M, 0, OCTET_STRING, FIXED ^*/

    /* SPR 13018 changes start */
    son_s8        rsTxPower;
    /*^ M, 0, B, -60, 50 ^*/
    /*SPR 13018 changes end */

    son_u8        dl_sys_bw;
    /*^ M, 0, N, 0, 0 ^*/
    son_u8        ul_sys_bw;
    /*^ M, 0, N, 0, 0 ^*/
}son_bcch_cell_num_t;

#define SON_BCCH_LIST_PRESENT         0x01

/*! \  son_cell_info_t
 *  \brief     Cell information
 *  \param     bitmask
 *  \param     rf_list   - RF information
 *  \param     bcch_list - BCCH information
 */
typedef struct _son_cell_info_t
{
    son_u32              bitmask;    /*^ BITMASK ^*/
    son_rf_cell_num_t    rf_list;    /*^ M, 0, N, 0, 0 ^*/ 
    son_bcch_cell_num_t bcch_list; /*^ O, SON_BCCH_LIST_PRESENT, N, 0, 0 ^*/
}son_cell_info_t;

/*! \   son_rf_freq_channel_num_t
 *  \brief   Information corresponding to the EARFCN
 *  \param   earfcn - This field contains the value of the earfcn for which the
 *                    RSSI and the neighbouring cell information have been 
 *                    collected.
 *  \param   rssi   - The measured RSSI in dBm in steps of 1db (here 0 is -144 
 *                    and 128 is -16)
 *  \param   cell_list_size - No. of cells for which information is present in 
 *                            the list
 *  \param   cell_list      - List containing the information of cells
 */
typedef struct _son_rf_freq_channel_num_t
{
    son_u32            earfcn;         /*^ M, 0, N, 0, 0 ^*/
    son_u8             rssi;           /*^ M, 0, H, 0, 128 ^*/
/*SPR_17664_start*/
    son_u16            cell_list_size; /*^ M, 0, B, 1, SON_MAX_NO_CELLS_PER_EARFCN ^*/
/*SPR_17664_end*/
    son_cell_info_t    cell_list[SON_MAX_NO_CELLS_PER_EARFCN];    
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/ 
}son_rf_freq_channel_num_t;

/*! \   son_start_intra_rat_bulk_scan_resp_t
 *  \brief Cell data list containing the information for EARFCN that have been
 *         scanned
 *  \param rf_freq_channel_list_size - Number of EARFCNs that have been scanned
 *  \param rf_freq_channel_num       - Neighbors cell information. collected by 
 *                                     reading the Cell Broadcast Parameters
 */
typedef struct _son_start_intra_rat_bulk_scan_resp_t
{
   son_u16                      rf_freq_channel_list_size; 
   /* Coverity 99555 FIX START */
   /*^ M, 0, H, 0, 100 ^*/
   /* Coverity 99555 FIX END */
   son_rf_freq_channel_num_t    rf_freq_channel_num[SON_MAX_NO_EARFCN];
   /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_start_intra_rat_bulk_scan_resp_t;

/*! \ son_start_inter_rat_bulk_scan_resp_t 
 *  \brief to be defined later
 *  \param bitmask - Bitmask Value 
 */
typedef struct _son_start_inter_rat_bulk_scan_resp_t
{
    son_u32         bitmask;   /* For Future Reference */
    /* To Be Defined Later */
}son_start_inter_rat_bulk_scan_resp_t;

#define SON_INTRA_RAT_BULK_NW_SCAN_RESP_PRESENT                  0x01
#define SON_INTER_RAT_BULK_NW_SCAN_RESP_PRESENT                  0x02

/*! \  son_bulk_nw_scan_resp_t
 *  \brief Scan results for Inter/Intra Rat Cells
 *  \param bitmask
 *  \param intra_rat_scan_resp - The Scan Result from LTE(INTRA-RAT) scan
 *  \param inter_rat_scan_resp - To be supported in Future releases
 */
typedef struct _son_bulk_nw_scan_resp_t
{
    son_u32                                 bitmask;  /*^ BITMASK ^*/
    son_start_intra_rat_bulk_scan_resp_t    intra_rat_scan_resp; 
    /*^ O, SON_INTRA_RAT_BULK_NW_SCAN_RESP_PRESENT, N, 0, 0 ^*/
    son_start_inter_rat_bulk_scan_resp_t    inter_rat_scan_resp; 
    /*^ O, SON_INTER_RAT_BULK_NW_SCAN_RESP_PRESENT, N, 0, 0 ^*/
}son_bulk_nw_scan_resp_t;

/*
 *  SON_OAM_BULK_NW_SCAN_RESP
 */ 

/*! \ son_oam_bulk_nw_scan_resp_t
 *  \brief Response to SON_OAM_NW_SCAN_REQ request 
 *  \param result       - Success/Failure
 *  \param error_code   - If Failure, then Cause
 *  \param nw_scan_resp - Scan response for Intra-RAT and Inter-RAT
 */
typedef struct _son_oam_bulk_nw_scan_resp_t
{
    son_return_et                   result;     /*^ M, 0, H, 0, 2 ^*/
    son_error_et                    error_code; /*^ M, 0, N, 0, 0 ^*/
    son_bulk_nw_scan_resp_t nw_scan_resp; /*^ M, 0, N, 0, 0 ^*/
}son_oam_bulk_nw_scan_resp_t;     /*^ API, SON_OAM_BULK_NW_SCAN_RESP ^*/

/*
 *  SON_OAM_NR_SCAN_REQ
 */

#define SON_INTRA_RAT_NR_SCAN_REQ_PRESENT                    0x01
#define SON_INTER_RAT_NR_SCAN_REQ_PRESENT                    0x02

/*! \ son_oam_nr_scan_req_t
 *  \brief Request to start scanning for NR
 *  \param bitmask
 *  \param start_intra_rat_scan_req  - Scan Result from LTE(INTRA-RAT) scan
 *  \param start_inter_rat_scan_req  - Future Release
 */
typedef struct _son_oam_nr_scan_req_t 
{
    son_u32                               bitmask;        /*^ BITMASK ^*/
    son_start_intra_rat_scan_req_t        start_intra_rat_scan_req; 
    /*^ O, SON_INTRA_RAT_NR_SCAN_REQ_PRESENT, N, 0, 0 ^*/
    son_start_inter_rat_scan_req_t        start_inter_rat_scan_req; 
    /*^ O, SON_INTER_RAT_NR_SCAN_REQ_PRESENT, N, 0, 0 ^*/
}son_oam_nr_scan_req_t;    /*^ API, SON_OAM_NR_SCAN_REQ ^*/

/*
 *  SON_OAM_NR_SCAN_RESP
 */

/*! \ son_oam_nr_scan_resp_t
 *  \brief Response for SON_OAM_NR_SCAN_REQ request
 *  \param result   Success/Failure
 *  \param error_code   If Failure, then Cause
 */
typedef struct _son_oam_nr_scan_resp_t
{
    son_return_et               result;     /*^ M, 0, H, 0, 2 ^*/
    son_error_et                error_code; /*^ M, 0, N, 0, 0 ^*/
}son_oam_nr_scan_resp_t; /*^ API, SON_OAM_NR_SCAN_RESP ^*/

#define SON_PCI_PRESENT                        0x01
#define SON_CELL_OFFSET_PRESENT                0x02
#define SON_TAC_PRESENT                        0x04
#define SON_PLMN_ID_PRESENT                    0x08
#define SON_INTRA_CSG_ID_PRESENT               0x10
#define SON_EARFCN_PRESENT                     0x20
#define SON_RSRP_PRESENT                       0x40
#define SON_RSRQ_PRESENT                       0x80
#define SON_ENABLE_NR_STATUS_PRESENT           0x100
#define SON_ENABLE_HO_STATUS_PRESENT           0x200
#define SON_ENABLE_X2_STATUS_PRESENT           0x400
#define SON_EUTRAN_NR_HO_STATS_PRESENT         0x800
#define SON_SUSPECT_PCI_PRESENT                0x1000
#define SON_CM_CELL_OFFSET_PRESENT             0x2000
#define SON_BROADCAST_STATUS_PRESENT           0x4000
#define SON_ACCESS_MODE_PRESENT                0x8000
#define SON_UL_EARFCN_PRESENT                  0x10000
#define SON_PRACH_CONFIGURATION_PRESENT        0x20000
/** eICIC changes start */
#define SON_EICIC_INFO_PRESENT                 0x40000
/** eICIC changes end */
/* SPR 22248 MBMS Changes Start */
#define SON_EMBMS_MBMSFN_INFO_PRESENT          0x80000
/*SPR 18605:Fix Start*/
#define SON_EMBMS_TDD_SF_ASSN_PRESENT          0x100000 
/*SPR 18605:Fix End*/
/* SPR 22248 MBMS Changes Stop */

/*! \ son_intra_rat_neighbor_info_t
 *  \brief Inra-Rat neighbor information maintained in NRT
 *  \param bitmask
 *  \param nbr_cgi                Nbr's cell id
 *  \param pci                    Physical cell id  
 *  \param cell_specific_offset   Cell Specific Offset
 *  \param tac                    Tracking area code
 *  \param num_plmn_id            Number of PLMNs
 *  \param plmn_id                PLMN Ids of the PLMNs the cell serves
 *  \param csg_identity           Closed subscriber group id (Optional)
 *  \param earfcn                 This field contains the value of the earfcn 
 *                                for which the RSSI and the neighbouring cell 
 *                                information have been collected. 
 *  \param rsrp                   The measured "Reference signal received 
 *                                power" by the L1 N/W Scan
 *  \param rsrq                   The calculated "Reference Signal Received 
 *                                Quality" as per the spec 36.214
 *  \param nr_status              Indicates whether the ANR function at eNB is 
 *                                allowed to autonomously remove this cell from
 *                                the neighbour relation table.  
 *  \param ho_status              Indicates whether HO is allowed or not to 
 *                                this target cell (In the first release it 
 *                                would always be HO Prohibited
 *  \param x2_status              Indicates whether X2 connection can be used 
 *                                for HO or not with the corresponding eNB 
 *  \param handover_stats         Indicates handover statistics 
 *  \param suspected_pci          Indicates if pci is suspected 
 *  \param cm_cell_specific_offset Connected Mode Cell Specific Offset 
 *  \param broadcast_status       Indicates if the neighbor cell is to
 *                                be broadcasted in SIB4 or SIB5
 *  \param access_mode            Indicates access mode of the neighbor. 
 *  \param ul_earfcn              This field contains the value of the 
 *                                uplink earfcn
 *  \param prach_config           LTE cell physical layer PRACH parameters     
 *  \param eicic_info             eICIC information stores ABS max and min threshold and
 *                                scheme related information.
 */
typedef struct _son_intra_rat_neighbor_info_t
{
    son_u32                         bitmask;    /*^ BITMASK ^*/
    
    son_intra_rat_global_cell_id_t  nbr_cgi;        /*^ M, 0, N, 0, 0 ^*/
    
    son_u16                         pci;         
    /*^ O, SON_PCI_PRESENT, H, 0, 503 ^*/

    son_q_offset_range_et           cell_specific_offset;
    /*^ O, SON_CELL_OFFSET_PRESENT, H, 0, 30 ^*/ 

    son_u8                          tac[SON_TAC_OCTET_SIZE];
    /*^ O, SON_TAC_PRESENT, OCTET_STRING, FIXED ^*/

    son_u8                          num_plmn_id; 

/*SPR_17664_start*/
    /*^ O, SON_PLMN_ID_PRESENT, B, 1, SON_NR_MAX_NUM_PLMN ^*/
/*SPR_17664_end*/
    son_cell_plmn_info_t            plmn_id[SON_NR_MAX_NUM_PLMN];
    /*^ O, SON_PLMN_ID_PRESENT, OCTET_STRING, VARIABLE ^*/

    son_u8                          csg_identity[SON_CSG_ID_OCTET_SIZE];
    /*^ O, SON_INTRA_CSG_ID_PRESENT, OCTET_STRING, FIXED ^*/

    son_u32                         earfcn;
    /*^ O, SON_EARFCN_PRESENT, N, 0, 0 ^*/
    
    /* SPR-14015 START */
    son_u8                          rsrp;
    /*^ O, SON_RSRP_PRESENT, H, 0, 97 ^*/
    /* SPR-14015 END */

    son_u8                          rsrq;
    /*^ O, SON_RSRQ_PRESENT, H, 0, 34 ^*/
    
    son_nr_status_et                nr_status;
    /*^ O, SON_ENABLE_NR_STATUS_PRESENT, B, 1, 2 ^*/

    son_ho_status_et                ho_status;
    /*^ O, SON_ENABLE_HO_STATUS_PRESENT, B, 1, 2 ^*/

/* SPR-10937 Fix Starts */
    son_x2_status_et                x2_status;    
    /*^ O, SON_ENABLE_X2_STATUS_PRESENT, B, 3, 7 ^*/
/* SPR-10937 Fix Ends */

    son_neighbor_handover_stats_t   handover_stats;
    /*^ O, SON_EUTRAN_NR_HO_STATS_PRESENT, N, 0, 0 ^*/

    son_bool_et                     suspected_pci;
    /*^ O, SON_SUSPECT_PCI_PRESENT, H, 0, 1 ^*/
    son_q_offset_range_et           cm_cell_specific_offset;
    /*^ O, SON_CM_CELL_OFFSET_PRESENT, H, 0, 30 ^*/
    son_bool_et                     broadcast_status;
    /*^ O, SON_BROADCAST_STATUS_PRESENT, H, 0,1 ^*/
    son_access_mode_et              access_mode;
    /*^ O, SON_ACCESS_MODE_PRESENT, H, 0,3 ^*/
    
    son_u16                         ul_earfcn;
    /*^ O, SON_UL_EARFCN_PRESENT, N, 0, 0 ^*/

    son_prach_configuration_t               prach_config;
    /*^ O, SON_PRACH_CONFIGURATION_PRESENT, N, 0, 0 ^*/
/** eICIC changes start */
    son_oam_eicic_info_t            eicic_info;
    /*^ O, SON_EICIC_INFO_PRESENT, N, 0, 0 ^*/
/** eICIC changes end */
/* SPR 22248 MBMS Changes Start */
    son_mbms_configuration_t          mbms_config;
    /*^ O, SON_EMBMS_MBMSFN_INFO_PRESENT, N, 0, 0 ^*/
/*SPR 18605:Fix Start*/
    son_u8                          subframe_assn;
    /*^ O, SON_EMBMS_TDD_SF_ASSN_PRESENT, H, 0, 6 ^*/
/*SPR 18605:Fix End*/

/* SPR 22248 MBMS Changes Stop */
}son_intra_rat_neighbor_info_t;

#define SON_PCI_FDD_PRESENT                  0x01
#define SON_PCI_TDD_PRESENT                  0x02

/*! \  son_utran_physical_cell_id_t
 *  \brief Physical Cell ID
 *  \param bitmask - Bitmask Value
 *  \param pci_fdd - PCI (FDD Mode)
 *  \param pci_tdd - PCI (TDD Mode)
 */
typedef struct _son_utran_physical_cell_id_t
{
    son_u32            bitmask;        /*^ BITMASK ^*/
    son_u16                pci_fdd;    /*^ O, SON_PCI_FDD_PRESENT, H, 0, 511 ^*/
    son_u8                 pci_tdd;        /*^ O, SON_PCI_TDD_PRESENT, H, 0, 127 ^*/
}son_utran_physical_cell_id_t;

#define SON_UTRAN_PCI_PRESENT                        0x01
#define SON_UTRAN_CSO_PRESENT                        0x02
#define SON_UTRAN_LAC_PRESENT                        0x04
#define SON_UTRAN_RAC_PRESENT                        0x08
#define SON_UTRAN_PLMN_ID_PRESENT                    0x10
#define SON_UTRAN_CSG_ID_PRESENT                     0x20
#define SON_UTRAN_UARFCN_PRESENT                     0x40
#define SON_UTRAN_RSCP_PRESENT                       0x80
#define SON_UTRAN_ECNO_PRESENT                       0x100
#define SON_UTRAN_ENABLE_NR_STATUS_PRESENT           0x200
#define SON_UTRAN_ENABLE_HO_STATUS_PRESENT           0x400
#define SON_UTRAN_NR_HO_STATS_PRESENT                0x800
#define SON_UTRAN_ACCESS_MODE_PRESENT                0x1000
#define SON_UTRAN_PS_HO_SUPPORT_PRESENT              0x2000
#define SON_UTRAN_VOIP_CAPABLE_PRESENT               0x4000

/*! \ son_utran_neighbor_info_t
 *  \brief This is neighbour relation maintained in NRT
 *  \param bitmask
 *  \param nbr_cgi                Source cell id
 *  \param pci                    Physical cell id
 *  \param cell_specific_offset   Cell specific offset
 *  \param lac                    Location area code
 *  \param rac                    Routing area code
 *  \param num_plmn_id            Number of PLMNs
 *  \param plmn_id                PLMN Ids of the PLMNs the cell serves
 *  \param csg_identity           Closed subscriber group id (Optional)
 *  \param uarfcn                 This field contains the value of the earfcn
 *                                for which the RSSI and the neighbouring cell
 *                                information have been collected.
 *  \param rscp                   The measured "Reference signal received
 *                                power" by the L1 N/W Scan
 *  \param ecno                   The calculated "Reference Signal Received
 *                                Quality" as per the spec 36.214
 *  \param nr_status              Indicates whether the ANR function at eNB is
 *                                allowed to autonomously remove this cell from
 *                                the neighbour relation table.
 *  \param ho_status              Indicates whether HO is allowed or not to
 *                                this target cell (In the first release it
 *                                would always be HO Prohibited
 *  \param handover_stats         Indicates handover statistics 
 *  \param access_mode            Indicates access mode of the neighbor.
 *  \param ps_ho_supported        Indicates whether ps_ho is supported over
 *                                this cell or not.(Added for SRVCC)
 *  \param voip_capable           Indicates whether this cell is voip capable
 *                                or not.(Added for SRVCC)
 */
typedef struct _son_utran_neighbor_info_t
{
    son_u32                             bitmask;    /*^ BITMASK ^*/

    son_utran_geran_global_cell_id_t    nbr_cgi;        /*^ M, 0, N, 0, 0 ^*/

    son_utran_physical_cell_id_t        pci;        /*^ O, SON_UTRAN_PCI_PRESENT, N, 0, 0 ^*/

    son_s8                              cell_specific_offset;   /*^ O, SON_UTRAN_CSO_PRESENT, B, -15, 15 ^*/

    son_u8                              lac[SON_LAC_OCTET_SIZE];
    /*^ O, SON_UTRAN_LAC_PRESENT, OCTET_STRING, FIXED ^*/

    son_u8                              rac;
    /*^ O, SON_UTRAN_RAC_PRESENT, N, 0, 0 ^*/

    son_u8                          num_plmn_id;

/*SPR_17664_start*/
    /*^ O, SON_UTRAN_PLMN_ID_PRESENT, B, 1, SON_NR_MAX_NUM_PLMN ^*/
/*SPR_17664_end*/
    son_cell_plmn_info_t        plmn_id[SON_NR_MAX_NUM_PLMN];
    /*^ O, SON_UTRAN_PLMN_ID_PRESENT, OCTET_STRING, VARIABLE ^*/

    son_u8                          csg_identity[SON_CSG_ID_OCTET_SIZE];
    /*^ O, SON_UTRAN_CSG_ID_PRESENT, OCTET_STRING, FIXED ^*/

    son_u16                         uarfcn;
    /*^ O, SON_UTRAN_UARFCN_PRESENT, N, 0, 16383 ^*/

    son_s8                          rscp;
    /*^ O, SON_UTRAN_RSCP_PRESENT, B, -5, 91 ^*/

    son_u8                         ecno;
    /*^ O, SON_UTRAN_ECNO_PRESENT, H, 0, 49 ^*/

    son_nr_status_et             nr_status;
    /*^ O, SON_UTRAN_ENABLE_NR_STATUS_PRESENT, B, 1, 2 ^*/

    son_ho_status_et             ho_status;
    /*^ O, SON_UTRAN_ENABLE_HO_STATUS_PRESENT, B, 1, 2 ^*/

    son_neighbor_handover_stats_t   handover_stats;
    /*^ O, SON_UTRAN_NR_HO_STATS_PRESENT, N, 0, 0 ^*/
    son_access_mode_et            access_mode;
    /*^ O, SON_UTRAN_ACCESS_MODE_PRESENT,H,0,3  ^*/

    son_bool_et                 ps_ho_supported;
    /*^ O, SON_UTRAN_PS_HO_SUPPORT_PRESENT, H, 0, 1 ^*/

    son_bool_et                 voip_capable;
    /*^ O, SON_UTRAN_VOIP_CAPABLE_PRESENT, H, 0, 1 ^*/
}son_utran_neighbor_info_t;

/*! \  son_geran_physical_cell_id_t
 *  \brief This is GERAN pci inforamtion
 *  \param ncc                  Network Colour Code def in 23.003
 *  \param bscc                 Base station Colour Code def in 23.003
 */
typedef struct _son_geran_physical_cell_id_t
{
    son_u8          ncc;
    /*^ M, 0, H, 0, 7 ^*/
    son_u8          bscc;
    /*^ M, 0, H, 0, 7 ^*/
}son_geran_physical_cell_id_t;

#define SON_GERAN_PCI_PRESENT                        0x01
#define SON_GERAN_CSO_PRESENT                        0x02
#define SON_GERAN_RAC_PRESENT                        0x04
#define SON_GERAN_ARFCN_BAND_IND_PRESENT             0x08
#define SON_GERAN_RSSI_PRESENT                       0x10
#define SON_GERAN_ENABLE_NR_STATUS_PRESENT           0x20
#define SON_GERAN_ENABLE_HO_STATUS_PRESENT           0x40
#define SON_GERAN_NR_HO_STATS_PRESENT                0x80
#define SON_GERAN_DTM_SUPPORT_PRESENT                0x100

/*! \ son_geran_neighbor_info_t
 *  \brief GERAN neighbor information maintained in NRT
 *  \param bitmask
 *  \param nbr_cgi                Source cell id
 *  \param pci                    Physical cell id
 *  \param cell_specific_offset   Cell Specific offset
 *  \param rac                    Routing area code
 *  \param arfcn                  This field contains the value of the arfcn
 *                                info for GERAN cell
 *  \param band_ind               Band indicator of the given ARFCN value
 *  \param rssi                   rssi value
 *  \param nr_status              Indicates whether the ANR function at eNB is
 *                                allowed to autonomously remove this cell from
 *                                the neighbour relation table.
 *  \param ho_status              Indicates whether HO is allowed or not to
 *                                this target cell (In the first release it
 *                                would always be HO Prohibited)
 *  \param handover_stats         Indicates handover statistics
 *  \param dtm_supported          Indicates whether GERAN cell supports DTM or not(Added for SPR 11121)
 */
typedef struct _son_geran_neighbor_info_t
{
    son_u32                             bitmask;    /*^ BITMASK ^*/

    son_utran_geran_global_cell_id_t    nbr_cgi;
    /*^ M, 0, N, 0, 0 ^*/

    son_geran_physical_cell_id_t        pci;        
    /*^ O, SON_GERAN_PCI_PRESENT, N, 0, 0 ^*/

    son_s8                              cell_specific_offset;
    /*^ O, SON_GERAN_CSO_PRESENT, B, -15, 15 ^*/

    son_u8                              rac;
    /*^ O, SON_GERAN_RAC_PRESENT, N, 0, 0 ^*/

    son_u16                              arfcn;
    /*^ O, SON_GERAN_ARFCN_BAND_IND_PRESENT, H, 0, 1023 ^*/

    son_geran_band_indicator_et         band_ind;
    /*^ O, SON_GERAN_ARFCN_BAND_IND_PRESENT, H, 0, 1 ^*/

    son_u8                              rssi;
    /*^ O, SON_GERAN_RSSI_PRESENT, H, 0, 63 ^*/

    son_nr_status_et                     nr_status;
    /*^ O, SON_GERAN_ENABLE_NR_STATUS_PRESENT, B, 1, 2 ^*/

    son_ho_status_et                     ho_status;
    /*^ O, SON_GERAN_ENABLE_HO_STATUS_PRESENT, B, 1, 2 ^*/

    son_neighbor_handover_stats_t   handover_stats;
    /*^ O, SON_GERAN_NR_HO_STATS_PRESENT, N, 0, 0 ^*/

    son_u8                              dtm_supported;
    /*^ O, SON_GERAN_DTM_SUPPORT_PRESENT, H, 0, 1 ^*/            
}son_geran_neighbor_info_t;

#define SON_INTER_UTRAN_NEIGHBOR                        0x01
#define SON_INTER_GERAN_NEIGHBOR                        0x02
#define SON_INTER_CDMA_NEIGHBOR                         0x04

/*! \ son_inter_rat_neighbor_info_t
 *  \brief Structure storing Inter-RAT Neighboring cell information
 *  \param bitmask
 *  \param utran_neighbor - Information for Utran Neighbor Cell
 *  \param geran_neighbor - Information for Geran Neighbor Cell
 *  \param cdma_neighbor  - Information for CDMA Neighbor Cell
 */
typedef struct _son_inter_rat_neighbor_info_t
{
    son_u32                     bitmask;        /*^ BITMASK ^*/
    son_utran_neighbor_info_t   utran_neighbor;
    /*^ O, SON_INTER_UTRAN_NEIGHBOR, M, 0, 0 ^*/
    son_geran_neighbor_info_t   geran_neighbor;
    /*^ O, SON_INTER_GERAN_NEIGHBOR, M, 0, 0 ^*/
    son_cdma_neighbor_info_t    cdma_neighbor;
    /*^ O, SON_INTER_CDMA_NEIGHBOR, M, 0, 0 ^*/
    /* Other RAT Specific Structures To Be Defined Later */
}son_inter_rat_neighbor_info_t;

/*! \  son_neighbor_info_t
 *  \brief   Structure storing Intra-RAT and Inter-RAT Neighbouring cell
 *           information
 *  \param   intra_rat_nr_list_size - Number of intra rat neighbor cell 
 *  \param   intra_rat_nr_list      - Information of Intra-RAT Neighbouring 
 *                                    Cells
 *  \param   inter_rat_nr_list_size - Number of inter rat neighbor cells
 *  \param   inter_rat_nr_list      - Information of Intra-RAT Neighbouring 
 *                                    cells
 */
typedef struct _son_neighbor_info_t
{
    son_u16                intra_rat_nr_list_size;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_MAX_NO_INTRA_RAT_NEIGHBOUR_CELLS ^*/
/*SPR_17664_end*/
    son_intra_rat_neighbor_info_t      
        intra_rat_nr_list[SON_MAX_NO_INTRA_RAT_NEIGHBOUR_CELLS];
        /*^ M, 0, OCTET_STRING, VARIABLE ^*/

    son_u16                inter_rat_nr_list_size;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_MAX_NO_INTER_RAT_NEIGHBOUR_CELLS ^*/
/*SPR_17664_end*/
    son_inter_rat_neighbor_info_t      
        inter_rat_nr_list[SON_MAX_NO_INTER_RAT_NEIGHBOUR_CELLS];
        /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_neighbor_info_t;

#define SON_NO_ACTIVITY_INTERVAL_PRESENT    0x01
#define SON_FAILED_HO_THRESHOLD_PRESENT     0x02
#define SON_TIMEOUT_NO_NR_NEIGHBORS         0x04

/*! \  son_neighbor_removal_info_t
 *  \brief    Structure storing neighbor removal criteria info
 *  \param    bitmask
 *  \param    no_activity_interval          - Indicates the interval in minutes
 *  \param    handover_failure_threshold    - Indicates the max number of failed handovers 
 *  \param    timeout_no_nr_neighbors       - Indicates the length of time a neighbor cell 
 *                                            without any neighbor relations remain in the eNB
 */
typedef struct _son_neighbor_removal_info_t
{
    son_u32                     bitmask;                    /*^ BITMASK ^*/
    son_u16                     no_activity_interval; 
        /*^ O, SON_NO_ACTIVITY_INTERVAL_PRESENT, N, 0, 0 ^*/ 
    son_u8                      handover_failure_threshold;
        /*^ O, SON_FAILED_HO_THRESHOLD_PRESENT, N, 0, 0 ^*/
    son_u16                     timeout_no_nr_neighbors;
        /*^ O, SON_TIMEOUT_NO_NR_NEIGHBORS, N, 0, 0 ^*/ 
}son_neighbor_removal_info_t;

#define SON_SUSPECT_PCI_THRESHOLD_PRESENT            0x01
#define SON_CLEAR_SUSPECT_PCI_THRESHOLD_PRESENT      0x02

/*! \  son_neighbor_pci_confusion_info_t
 *  \brief    Structure storing neighbor pci confusion information in NRT
 *  \param    bitmask
 *  \param    suspect_pci_threshold          - Indicates the max number of failed handovers 
 *                                             due to PCI conflict 
 *  \param    clear_suspect_pci_threshold    - Indicates the number of measurement reports to 
 *                                             remove a PCI from the suspect list Indicates the 
 *                                             number of measurement reports to remove a PCI from 
 *                                             the suspect list 
 */
typedef struct _son_neighbor_pci_confusion_info_t
{
    son_u32                     bitmask;    /*^ BITMASK ^*/
    son_u8                      suspect_pci_threshold;
    /*^ O, SON_SUSPECT_PCI_THRESHOLD_PRESENT, N, 0, 0 ^*/
    son_u8                      clear_suspect_pci_threshold;
    /*^ O, SON_CLEAR_SUSPECT_PCI_THRESHOLD_PRESENT, N, 0, 0 ^*/
}son_neighbor_pci_confusion_info_t;

/* SPR 20653 Fix Start */
#define SON_MODIFY_DL_ANR_THRESHOLD_PRESENT  0x01
#define SON_MODIFY_UL_ANR_THRESHOLD_PRESENT  0x02

/*! \  son_oam_anr_modify_attribute_info_t
 *  \brief   Request to Modify ANR attributes 
 *  \param ue_throughput_dl_anr_threshold       DL Throughput for UE inactivity for ANR measurements
 *  \param ue_throughput_ul_anr_threshold       UL Throughput for UE inactivity for ANR measurements
 */
typedef struct _son_oam_anr_modify_attribute_info_t
{
    son_u32                     bitmask;    /*^ BITMASK ^*/
    son_u8 ue_throughput_dl_anr_threshold;    
    /*^ O, SON_MODIFY_DL_ANR_THRESHOLD_PRESENT, B, 1, 255 ^*/
    son_u8 ue_throughput_ul_anr_threshold;     
    /*^ O, SON_MODIFY_UL_ANR_THRESHOLD_PRESENT, B, 1, 255 ^*/
}son_oam_anr_modify_attribute_info_t;
/* SPR 20653 Fix End */

#define SON_SRC_CGI_PRESENT                 0x01
#define SON_UE_CNT_PRESENT                  0x02
#define SON_REPORTNG_INTERVAL_PRESENT       0x04
#define SON_LIMITED_MODE_PRESENT            0x08
#define SON_REMOVAL_CRITERIA_PRESENT        0x10
#define SON_PCI_CONFUSION_CFG_PRESENT       0x20
/* SPR_19619 start */
#define SON_PEER_CELL_ACTIVATION_PRESENT       0x40
/* SPR_19619 stop */
/* SPR 20653 Fix Start */
#define SON_UE_THROUGHPUT_DL_ANR_THRESHOLD_PRESENT  0x80
#define SON_UE_THROUGHPUT_UL_ANR_THRESHOLD_PRESENT  0x100
/* SPR 20653 Fix End */


/*! \  son_oam_anr_enable_req_t
 *  \brief It is used to enable ANR for the cell whose id is present in the 
 *         request or to globally enable ANR for all configured cells at 
 *         eNodeB, if the cell id is not present
 *  \param bitmask - If bitmask is set then request is for particular cell id 
 *                   else considered for all the configured cells
 *  \param src_cgi_list_size - Source cgi list
 *  \param src_cgi - Source cell id (Optional)
 *  \param ue_cnt    Number of UEs to be configured for UE Measurements The value 0 means 
 *                   no UE will be configured for measurements. //SPR 18268
 *  \param reporting_interval -  UE Measurement report interval - The value 0 means it
 *                   will not be reported periodically. (Interval unit is in
 *                   Minutes)
 *  \param limited_mode       - Indicates of limited mode is ON/OFF.
 *  \param removal_criteria    - Indicates of neighbor removal criteria.
 *  \param pci_confusion_cfg   -  PCI conflict confusion configuration.
 */
typedef struct _son_oam_anr_enable_req_t
{
    son_u32                             bitmask;    /*^ BITMASK ^*/
    son_u8                              src_cgi_list_size;
    /*^ O, SON_SRC_CGI_PRESENT, B, 1, 1 ^*/
    son_intra_rat_global_cell_id_t      src_cgi[1]; 
    /*^ O, SON_SRC_CGI_PRESENT, OCTET_STRING, VARIABLE ^*/
    son_u16                              ue_cnt;                                     
    /*^ O, SON_UE_CNT_PRESENT, B, 1, 10 ^*/
    son_u16                             reporting_interval;                         
    /*^ O, SON_REPORTNG_INTERVAL_PRESENT, B, 60, 1440 ^*/
    son_bool_et                         limited_mode;
    /*^ O, SON_LIMITED_MODE_PRESENT, H, 0, 1 ^*/
    son_neighbor_removal_info_t         removal_criteria;
    /*^ O, SON_REMOVAL_CRITERIA_PRESENT, N, 0, 0 ^*/
    son_neighbor_pci_confusion_info_t  pci_confusion_cfg;
    /*^ O, SON_PCI_CONFUSION_CFG_PRESENT, N, 0, 0 ^*/
    /* SPR 20653 Fix Start */
    son_u8                              ue_throughput_dl_anr_threshold;
    /*^ O, SON_UE_THROUGHPUT_DL_ANR_THRESHOLD_PRESENT, B, 1, 255 ^*/
    son_u8                              ue_throughput_ul_anr_threshold;
    /*^ O, SON_UE_THROUGHPUT_UL_ANR_THRESHOLD_PRESENT, B, 1, 255 ^*/
    /* SPR 20653 Fix End */
}son_oam_anr_enable_req_t; /*^ API, SON_OAM_ANR_ENABLE_REQ ^*/

/*
 * SON_OAM_ANR_ENABLE_RESP
 */

/*! \  son_cell_status_t 
 *  \brief Structure for storing cell status
 *  \param cgi    - CGI of cell
 *  \param error_code - indicates error type in case of Failure
 */
typedef struct _son_cell_status_t
{
    son_intra_rat_global_cell_id_t        cgi; /*^ M, 0, N, 0, 0 ^*/
    son_error_et                          error_code; /*^ M, 0, N, 0, 0 ^*/
}son_cell_status_t;

/*! \  son_oam_anr_enable_resp_t
 *  \brief Response to ANR Enable Request.
 *  \param result                 - Result
 *  \param error_code             - error code
 *  \param cell_status_list_size - Number of elememts in the cell 
 *                                 status list
 *  \param cell_status_list      - List containing the status of each 
 *                                 cell requested for Enable
 */
typedef struct _son_oam_anr_enable_resp_t
{
    son_return_et        result;     /*^ M, 0, H, 0, 2 ^*/
    son_error_et         error_code; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u16              cell_status_list_size; /*^ M, 0, H, 0, SON_MAX_CELL_SUPPORTED_AT_ENB ^*/
/*SPR_17664_end*/
    son_cell_status_t    cell_status_list[SON_MAX_CELL_SUPPORTED_AT_ENB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_anr_enable_resp_t;    /*^ API, SON_OAM_ANR_ENABLE_RESP ^*/

/*
 * SON_OAM_ANR_DISABLE_REQ
 */

#define SON_ANR_DISABLE_SRC_CGI_PRESENT 0x01

/*! \ son_oam_anr_disable_req_t
 *  \brief It is used to disable ANR for the cell whose id is present in the 
 *         request or to globally disable ANR for all configured cells at 
 *         eNodeB, if cell id is not present
 *  \param bitmask
 *  \param src_cgi_list_size    CGI list size
 *  \param src_cgi              CGI of cell(Optional)
 */
typedef struct _son_oam_anr_disable_req_t
{
    son_u32                            bitmask;    /*^ BITMASK ^*/
    son_u8                              src_cgi_list_size;
    /*^ O, SON_ANR_DISABLE_SRC_CGI_PRESENT, B, 1, 1 ^*/
    son_intra_rat_global_cell_id_t      src_cgi[1];
    /*^ O, SON_ANR_DISABLE_SRC_CGI_PRESENT, OCTET_STRING, VARIABLE ^*/
} son_oam_anr_disable_req_t;    /*^ API, SON_OAM_ANR_DISABLE_REQ ^*/

/*
 * SON_OAM_ANR_DISABLE_RESP
 */

/*! \  son_oam_anr_disable_resp_t
 *  \brief ANR Disable Response Structure.
 *  \param result                 - Result
 *  \param error_code             - error code
 *  \param cell_status_list_size - Number of elememts in the cell 
 *                                 status list
 *  \param cell_status_list      - List containing the status of each 
 *                                 cell requested for Enable
 */
typedef struct _son_oam_anr_disable_resp_t
{
    son_return_et        result;     /*^ M, 0, H, 0, 2 ^*/
    son_error_et         error_code; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u16              cell_status_list_size;    /*^ M, 0, H, 0, SON_MAX_CELL_SUPPORTED_AT_ENB ^*/
/*SPR_17664_end*/
    son_cell_status_t    cell_status_list[SON_MAX_CELL_SUPPORTED_AT_ENB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_anr_disable_resp_t;    /*^ API, SON_OAM_ANR_DISABLE_RESP ^*/

/*! \  son_nbr_cell_status_t
 *  \brief Structure for storing cell status
 *  \param cgi    - CGI of cell
 *  \param error_code - indicates error type in case of Failure
 */
typedef struct _son_nbr_cell_status_t
{
    son_global_cell_id_t        cgi; /*^ M, 0, N, 0, 0 ^*/
    son_error_et                error_code; /*^ M, 0, N, 0, 0 ^*/
}son_nbr_cell_status_t;

/*
 * SON_OAM_NR_ADD_REQ
 */

/*! \  son_oam_nr_add_req_t
 *  \brief This is the request structure to add NR entry in NRT 
 *  \param src_cgi - Source cell id
 *  \param nr_list - List of neighbours and their attribute. 
 *                   Max size of list 256.
 */
typedef struct _son_oam_nr_add_req_t
{
    son_intra_rat_global_cell_id_t     src_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_neighbor_info_t                nr_list;    /*^ M, 0, N, 0, 0 ^*/
} son_oam_nr_add_req_t;    /*^ API, SON_OAM_NR_ADD_REQ ^*/

/*
 * SON_OAM_NR_ADD_RESP
 */

/*! \  son_nr_add_resp_t
 *  \brief This is the response structure for add NR entry in NRT
 *  \param src_cgi                      Source cell id
 *  \param result                       Result of NR update request
 *  \param error_code                   Error code 
 *  \param trgt_cell_status_list_size   target cell status list.
 *  \param trgt_cell_status_list        list containing cell id and error code
 */
typedef struct _son_oam_nr_add_resp_t
{
    son_intra_rat_global_cell_id_t   src_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_return_et                    result;     /*^ M, 0, H, 0, 2 ^*/
    son_error_et                     error_code; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u16                           trgt_cell_status_list_size;    /*^ M, 0, H, 0, SON_MAX_NO_NEIGHBOUR_CELLS ^*/
/*SPR_17664_end*/
    son_nbr_cell_status_t               trgt_cell_status_list[SON_MAX_NO_NEIGHBOUR_CELLS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} son_oam_nr_add_resp_t;    /*^ API, SON_OAM_NR_ADD_RESP ^*/

/*
 * SON_OAM_NR_UPDATE_REQ
 */

/*! \  son_oam_nr_update_req_t
 *  \brief ANR Table Update Request Structure
 *  \param src_cgi                    Source CGI
 *  \param nr_list - List of neighbours and their attributes.
 */
typedef struct _son_oam_nr_update_req_t
{
    son_intra_rat_global_cell_id_t        src_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_neighbor_info_t                   nr_list;    /*^ M, 0, N, 0, 0 ^*/
} son_oam_nr_update_req_t;    /*^ API, SON_OAM_NR_UPDATE_REQ ^*/

/*
 * SON_OAM_NR_UPDATE_RESP
 */

/*! \  son_nr_update_resp_t
 *  \brief This is the response structure for add NR entry in NRT
 *  \param src_cgi                      Source cell id
 *  \param result                       Result of NR update request
 *  \param error_code                   Error code 
 *  \param trgt_cell_status_list_size   target cell status list.
 *  \param trgt_cell_status_list        list containing cell id and error code
 */
typedef struct _son_oam_nr_update_resp_t
{
    son_intra_rat_global_cell_id_t   src_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_return_et                    result;     /*^ M, 0, H, 0, 2 ^*/
    son_error_et                     error_code; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u16                           trgt_cell_status_list_size;    /*^ M, 0, H, 0, SON_MAX_NO_NEIGHBOUR_CELLS ^*/
/*SPR_17664_end*/
    son_nbr_cell_status_t               trgt_cell_status_list[SON_MAX_NO_NEIGHBOUR_CELLS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} son_oam_nr_update_resp_t;    /*^ API, SON_OAM_NR_UPDATE_RESP ^*/

/*
 * SON_OAM_NR_DELETE_REQ
 */

/*! \  son_oam_nr_delete_req_t
 *  \brief This is the request structure to delete NR entry in NRT
 *  \param src_cgi            Source cell id
 *  \param trgt_cgi_list_size To be deleted list size
 *  \param trgt_cgi_list      To be deleted list of neighbours. Max list size 256
 */
typedef struct _son_oam_nr_delete_req_t
{
    son_intra_rat_global_cell_id_t src_cgi;             /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u16                        trgt_cgi_list_size;  /*^ M, 0, H, 1, SON_MAX_NO_NEIGHBOUR_CELLS ^*/
/*SPR_17664_end*/
    son_global_cell_id_t           trgt_cgi_list[SON_MAX_NO_NEIGHBOUR_CELLS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} son_oam_nr_delete_req_t;    /*^ API, SON_OAM_NR_DELETE_REQ ^*/

/*
 * SON_OAM_NR_DELETE_RESP
 */

/*! \  son_oam_nr_delete_resp_t
 *  \brief This is the response structure to delete NR entry in NRT
 *  \param src_cgi           Source cell id
 *  \param result            Result of NR update request
 *  \param error_code        Error code (Optional)
 *  \param trgt_cell_status_list_size   target cell status list.
 *  \param trgt_cell_status_list        list containing cell id and error code
 */
typedef struct _son_oam_nr_delete_resp_t
{
    son_intra_rat_global_cell_id_t  src_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_return_et                     result;     /*^ M, 0, H, 0, 2 ^*/
    son_error_et                      error_code; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u16                          trgt_cell_status_list_size;    /*^ M, 0, H, 0, SON_MAX_NO_NEIGHBOUR_CELLS ^*/
/*SPR_17664_end*/
    son_nbr_cell_status_t              trgt_cell_status_list[SON_MAX_NO_NEIGHBOUR_CELLS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} son_oam_nr_delete_resp_t;    /*^ API, SON_OAM_NR_DELETE_RESP ^*/

/*
 * SON_OAM_NR_RETRIEVE_REQ
 */

#define SON_RETRIEVE_TRGT_CGI_PRESENT  0x01
#define SON_RETRIEVE_NR_STATUS_PRESENT 0x02
#define SON_RETRIEVE_HO_STATUS_PRESENT 0x04
#define SON_RETRIEVE_X2_STATUS_PRESENT 0x08

/*! \  son_oam_nr_retrieve_req_t
 *  \brief This is the request of Neighbour retrieval for a particular cell 
 *         identified as source cell id.
 *  \param bitmask
 *  \param src_cgi   Source cell id
 *  \param trgt_cgi  Target cell id
 *  \param nr_status Indicates whether the ANR function at eNB is allowed to 
 *                   autonomously remove this cell from the neighbour relation 
 *                   table. 
 *  \param ho_status Indicates whether HO is allowed or not to this target 
 *                   cell 
 *  \param x2_status Indicates whether X2 connection can be used for HO or not 
 *                   with the corresponding eNB 
 */
typedef struct _son_oam_nr_retrieve_req_t
{
    son_u32                         bitmask;    /*^ BITMASK ^*/

    son_intra_rat_global_cell_id_t  src_cgi;    
    /*^ M, 0, N, 0, 0 ^*/

    son_global_cell_id_t            trgt_cgi;   
    /*^ O, SON_RETRIEVE_TRGT_CGI_PRESENT, N, 0, 0 ^*/
    
    son_nr_status_et                nr_status;   
    /*^ O, SON_RETRIEVE_NR_STATUS_PRESENT, B, 1, 2 ^*/

    son_ho_status_et                ho_status;   
    /*^ O, SON_RETRIEVE_HO_STATUS_PRESENT, B, 1, 2 ^*/

    son_x2_status_et                x2_status;   
    /*^ O, SON_RETRIEVE_X2_STATUS_PRESENT, B, 1, 2 ^*/
} son_oam_nr_retrieve_req_t;    /*^ API, SON_OAM_NR_RETRIEVE_REQ ^*/

/*
 * SON_OAM_NR_RETRIEVE_RESP
 */

/*! \  son_oam_nr_retrieve_resp_t
 *  \brief This is the response of NR retrieval 
 *  \param src_cgi           Source cell id
 *  \param result            Result of neighbour reterival request
 *  \param error_code        Error code  (valid in case of FAILURE)  
 *  \param nr_list           List of neighbours and its attribute values for
 *                           the source cell id. Max list size 256 (Optional)
 */
typedef struct _son_oam_nr_retrieve_resp_t
{
    son_intra_rat_global_cell_id_t     src_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_return_et                      result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et                       error_code; /*^ M, 0, N, 0, 0 ^*/
    son_neighbor_info_t                nr_list;    /*^ M, 0, N, 0, 0 ^*/
}son_oam_nr_retrieve_resp_t; /*^ API, SON_OAM_NR_RETRIEVE_RESP ^*/

/*
 * SON_OAM_ANR_UPDATED_NRT_INFO
 */

/*! \  son_oam_anr_updated_nrt_info_t
 *  \brief It gives updated Neighbour Relation Table information.
 *  \param src_cgi             CGI of source cell
 *  \param nrt_op              NRT Operation performed on the neighbors contained in the message
 *  \param nr_list             Updated list of neighbours and its attribute
 *                             values of source cell id. 
 *                             Max list size is 256.
 */
typedef struct _son_oam_anr_updated_nrt_info_t
{
    son_intra_rat_global_cell_id_t     src_cgi;    /*^ M, 0, N, 0, 0 ^*/
    /*+SPR 18853 FIX*/
    son_nrt_operation_performed_et     nrt_op;     /*^ M, 0, B, 1, 4 ^*/
    /*-SPR 18853 FIX*/
    son_neighbor_info_t                nr_list;    /*^ M, 0, N, 0, 0 ^*/
}son_oam_anr_updated_nrt_info_t;    /*^ API, SON_OAM_ANR_UPDATED_NRT_INFO ^*/

/*
 * SON_OAM_ES_ENABLE_REQ
 */

#define SON_ES_ENABLE_SRV_CGI_PRESENT 0x01

/*! \  son_oam_es_init_config_req_t
 *  \brief ES Enable Request received from OAM
 *  \param bitmask
 *  \param srv_cgi   CGI of cell on which ES is to be enabled. In case no 
 *                   cellId is present in the request then ES has to be enabled
 *                   on all the cells of eNodeB
 *  \param interval  Active UE count report interval - The value 0 means it 
 *                   will not be reported periodically. (Interval unit is in 
 *                   Secs)
 *  \param threshold_value  Threshold for number of active UEs - When threshold
 *                          is reached or the no of UE exceeds the threshold 
 *                          value, report will be send to the OAM.  
 */
typedef struct _son_oam_es_enable_req_t
{
    son_u32                         bitmask;         /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t  srv_cgi;    
    /*^ O, SON_ES_ENABLE_SRV_CGI_PRESENT, N, 0, 0 ^*/
    son_u16                         interval;        /*^ M, 0, N, 0, 0 ^*/
    son_u8                          threshold_value; /*^ M, 0, N, 0, 0 ^*/
}son_oam_es_enable_req_t;    /*^ API, SON_OAM_ES_ENABLE_REQ  ^*/

/*
 * SON_OAM_ES_ENABLE_RESP
 */

/*! \  son_oam_es_enable_resp_t
 *  \brief Response to ES Enable Request.
 *  \param  result                Result value
 *  \param  error_code            error_code
 *  \param cell_status_list_size  Number of elememts in the cell 
 *                                status list
 *  \param cell_status_list       List containing the status of each 
 *                                cell requested for Enable
 */
typedef struct _son_oam_es_enable_resp_t
{
    son_return_et        result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et         error_code; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u16              cell_status_list_size; /*^ M, 0, H, 0, SON_MAX_CELL_SUPPORTED_AT_ENB ^*/
/*SPR_17664_end*/
    son_cell_status_t    cell_status_list[SON_MAX_CELL_SUPPORTED_AT_ENB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_es_enable_resp_t; /*^ API, SON_OAM_ES_ENABLE_RESP ^*/

/*
 * SON_OAM_ES_DISABLE_REQ
 */

#define SON_ES_DISABLE_SRV_CGI_PRESENT 0x01

/*! \  son_oam_es_disable_req_t 
 *  \brief ES Disable Request Structure     
 *  \param bitmask
 *  \param srv_cgi                          CGI of cell
 */
typedef struct _son_oam_es_disable_req_t
{
    son_u32                            bitmask;    /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t     srv_cgi;    
    /*^ O, SON_ES_DISABLE_SRV_CGI_PRESENT, N, 0, 0 ^*/
}son_oam_es_disable_req_t;    /*^ API, SON_OAM_ES_DISABLE_REQ  ^*/

/*
 * SON_OAM_ES_DISABLE_RESP
 */

/*! \  son_oam_es_disable_resp_t
 *  \brief   Response to ES DISABLE REQ.
 *  \param  result                  Result value
 *  \param  error_code              error_code
 *  \param cell_status_list_size    Number of elememets in the cell
 *                                  status list
 *  \param cell_status_list         List containing the status of each 
 *                                  cell requested for Enable
 */
typedef struct _son_oam_es_disable_resp_t
{
    son_return_et        result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et         error_code; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u16              cell_status_list_size; /*^ M, 0, H, 0, SON_MAX_CELL_SUPPORTED_AT_ENB ^*/
/*SPR_17664_end*/
    son_cell_status_t    cell_status_list[SON_MAX_CELL_SUPPORTED_AT_ENB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_es_disable_resp_t;    /*^ API, SON_OAM_ES_DISABLE_RESP  ^*/

/*
 * SON_OAM_ES_MODIFY_UE_COUNT_INTERVAL_REQ
 */

/*! \  son_oam_es_modify_ue_count_interval_req_t
 *  \brief   Request to Modify the Interval for receiving 
 *           Active UE Count Report
 *  \param   srv_cgi    CGI of Cell
 *  \param   interval   Time interval for receiving Active UE Count report
 */
typedef struct _son_oam_es_modify_ue_count_interval_req_t
{
    son_intra_rat_global_cell_id_t    srv_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_u16                           interval;   /*^ M, 0, N, 0, 0 ^*/
}son_oam_es_modify_ue_count_interval_req_t;
/*^ API, SON_OAM_ES_MODIFY_UE_COUNT_INTERVAL_REQ  ^*/

/*
 * SON_OAM_ES_MODIFY_UE_COUNT_INTERVAL_RESP
 */

/*! \  son_oam_es_modify_ue_count_interval_resp_t
 *  \brief   Response to Modify UE Count Report Interval Request
 *  \param   srv_cgi    CGI of Cell
 *  \param   result     Success/failure
 *  \param   error_code Cause ( valid in case of FAILURE )
 */
typedef struct _son_oam_es_modify_ue_count_interval_resp_t
{
    son_intra_rat_global_cell_id_t  srv_cgi;        /*^ M, 0, N, 0, 0 ^*/
    son_return_et                   result;         /*^ M, 0, H, 0, 1 ^*/
    son_error_et                    error_code;     /*^ M, 0, N, 0, 0 ^*/
}son_oam_es_modify_ue_count_interval_resp_t;    
/*^ API, SON_OAM_ES_MODIFY_UE_COUNT_INTERVAL_RESP ^*/

/*
 * SON_OAM_ES_CELL_SWITCH_OFF_REQ
 */

/*! \  son_oam_es_cell_switch_off_req_t 
 *  \brief   Request for Cell Switch OFF
 *  \param   srv_cgi            CGI of Cell
 *  \param   switch_off_mode    type of switch off requested
 *                              (Graceful/Forceful)
 */
typedef struct _son_oam_es_cell_switch_off_req_t
{
    son_intra_rat_global_cell_id_t     srv_cgi;        /*^ M, 0, N, 0, 0 ^*/
    son_cell_switch_off_mode_et switch_off_mode; /*^ M, 0, B, 1, 2 ^*/
}son_oam_es_cell_switch_off_req_t; /*^ API, SON_OAM_ES_CELL_SWITCH_OFF_REQ ^*/

/*
 * SON_OAM_ES_CELL_SWITCH_OFF_RESP
 */

/*! \  son_oam_es_cell_switch_off_resp_t
 *  \brief   Response to CELL SWITCH OFF Request
 *  \param   srv_cgi    CGI of Cell
 *  \param   result     Success/failure
 *  \param   error_code Cause ( valid in case of FAILURE )
 */
typedef struct _son_oam_es_cell_switch_off_resp_t
{
    son_intra_rat_global_cell_id_t  srv_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_return_et                   result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et                    error_code; /*^ M, 0, N, 0, 0 ^*/
}son_oam_es_cell_switch_off_resp_t; /*^ API, SON_OAM_ES_CELL_SWITCH_OFF_RESP  ^*/

/*
 * SON_OAM_ES_SWITCHED_OFF_CELL_INFO_RESP
 */

/*! \  son_oam_es_switched_off_cell_info_resp_t
 *  \brief Switched off Cell info response 
 *  \param   result                      Success/failure
 *  \param   error_code                  Cause ( valid in case of FAILURE )
 *  \param   num_switched_off_cells      No. of elements in switched off cell 
 *                                       list
 *  \param   switched_off_cell_info_list List comntaining switched off cells 
 *                                       info
 */

typedef struct _son_oam_es_switched_off_cell_info_resp_t
{
    son_return_et          result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et           error_code; /*^ M, 0, N, 0, 0 ^*/
    son_u16                num_switched_off_cells;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_MAX_CELL_SUPPORTED_AT_ENB ^*/
/*SPR_17664_end*/
    son_intra_rat_global_cell_id_t    switched_off_cell_info_list[SON_MAX_CELL_SUPPORTED_AT_ENB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_es_switched_off_cell_info_resp_t;    
/*^ API, SON_OAM_ES_SWITCHED_OFF_CELL_INFO_RESP  ^*/

/*
 * SON_OAM_ES_CELL_SWITCH_ON_REQ
 */

/*! \  son_oam_es_cell_switch_on_req_t 
 *  \brief   Request for Cell Switch ON
 *  \param   srv_cgi    CGI of Cell
 */
typedef struct _son_oam_es_cell_switch_on_req_t
{
    son_intra_rat_global_cell_id_t        srv_cgi;    /*^ M, 0, N, 0, 0 ^*/
}son_oam_es_cell_switch_on_req_t;    /*^ API, SON_OAM_ES_CELL_SWITCH_ON_REQ ^*/

/*
 * SON_OAM_ES_CELL_SWITCH_ON_RESP
 */

/*! \  son_oam_es_cell_switch_on_resp_t 
 *  \brief   Response to CELL SWITCH ON Request
 *  \param   srv_cgi     CGI of Cell
 *  \param   result    Success/failure
 *  \param   error_code Cause for Failure
 */
typedef struct _son_oam_es_cell_switch_on_resp_t
{
    son_intra_rat_global_cell_id_t  srv_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_return_et                   result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et                    error_code; /*^ M, 0, N, 0, 0 ^*/
}son_oam_es_cell_switch_on_resp_t;  /*^ API, SON_OAM_ES_CELL_SWITCH_ON_RESP  ^*/

/*
 * SON_OAM_ES_ACTIVE_UE_COUNT_REPORT
 */

/*! \  son_oam_es_active_ue_count_report_t
 *  \brief   Report having active ue count for the specified cell to be sent 
 *           to OAM
 *  \param srv_cgi     CGI of cell
 *  \param ue_cnt      Number of active UEs //SPR 18268
 */
typedef struct _son_oam_es_active_ue_count_report_t
{
    son_intra_rat_global_cell_id_t        srv_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_u16                                ue_cnt;     /*^ M, 0, N, 0, 0 ^*/
}son_oam_es_active_ue_count_report_t;
/*^ API, SON_OAM_ES_ACTIVE_UE_COUNT_REPORT  ^*/

/*
 * SON_OAM_ES_MODIFY_UE_COUNT_THRESHOLD_REQ 
 */

/*! \  son_oam_es_modify_ue_count_threshold_req_t
 *  \brief   Request to modify the threshold value for the no. of Active UEs 
 *           for the specified cell 
 *  \param   srv_cgi             CGI of cell
 *  \param   threshold_value     Threshold value to be set
 */
typedef struct _son_oam_es_modify_ue_count_threshold_req_t
{
    son_intra_rat_global_cell_id_t  srv_cgi;        /*^ M, 0, N, 0, 0 ^*/
    son_u8 threshold_value; /*^ M, 0, N, 0, 0 ^*/
}son_oam_es_modify_ue_count_threshold_req_t;
/*^ API, SON_OAM_ES_MODIFY_UE_COUNT_THRESHOLD_REQ ^*/

/*
 * SON_OAM_ES_MODIFY_UE_COUNT_THRESHOLD_RESP
 */

/*! \  son_oam_es_modify_ue_count_threshold_resp_t
 *  \brief Response to  to modify the threshold value for the no. of Active UEs
 *  \param   srv_cgi     CGI of Cell
 *  \param   result    Success/failure
 *  \param   error_code Cause (valid in case of FAILURE)
 */
typedef struct _son_oam_es_modify_ue_count_threshold_resp_t
{
    son_intra_rat_global_cell_id_t   srv_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_return_et                   result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et                    error_code; /*^ M, 0, N, 0, 0 ^*/
}son_oam_es_modify_ue_count_threshold_resp_t; 
/*^ API, SON_OAM_ES_MODIFY_UE_COUNT_THRESHOLD_RESP ^*/

/*
 * SON_OAM_ES_UE_COUNT_THRESHOLD_HIT_IND
 */

/*! \  son_oam_es_ue_count_threshold_hit_ind_t
 *  \brief To indicate the following conditions to OAM 
 *      when the number of active UEs becomes less than or equal to the 
 *      configured threshold value 
 *      when the number of active UEs becomes greater than the configured 
 *      threshold value 
 *  \param srv_cgi  CGI of cell
 *  \param cause    Cause indicating Threshold Level
 */
typedef struct _son_oam_es_ue_count_threshold_hit_ind_t
{
    son_intra_rat_global_cell_id_t    srv_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_threshold_hit_cause_et        cause;        /*^ M, 0, H, 0, 1 ^*/
}son_oam_es_ue_count_threshold_hit_ind_t; 
/*^ API, SON_OAM_ES_UE_COUNT_THRESHOLD_HIT_IND ^*/

/*
 *  SON_OAM_ES_REQUEST_TO_SWITCH_ON_CELL
 */

/*! \  son_oam_es_request_to_switch_on_cell_t
 *  \brief   Request to switch on a particular cell, 
 *         OAM may switch on an existing stopped cell, if required, for example 
 *         in case other cells are under high load or eNB has received an S1 
 *         message which may require a cell to be started again (e.g. ETWS 
 *         notification over S1 interface)
 *  \param   srv_cgi      CGI of cell
 *  \param   cause        Cause for switching on the cell
 */
typedef struct _son_oam_es_request_to_switch_on_cell_t
{
    son_intra_rat_global_cell_id_t    srv_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_cell_switch_on_cause_et       cause;      /*^ M, 0, N, 0, 0 ^*/
}son_oam_es_request_to_switch_on_cell_t; 
/*^ API, SON_OAM_ES_REQUEST_TO_SWITCH_ON_CELL ^*/

/*
 *  SON_OAM_PERIODIC_NW_SCAN_REQ
 */
#define SON_INTRA_RAT_PERIODIC_NW_SCAN_REQ_PRESENT    0x01
#define SON_INTER_RAT_PERIODIC_NW_SCAN_REQ_PRESENT    0x02

/*! \typedef struct son_oam_periodic_nw_scan_req_t
 *  \brief    Periodic Network Scan Request for SON
 *  \param    bitmask
 *  \param    scan_on_off Periodic network scan is ON/OFF
 *  \param    scan_time_interval Time interval between two periodic scan
 *  \param    start_intra_rat_scan_req - For LTE
 *  \param    start_inter_rat_scan_req - FOR UMTS, GERAN and CDMA
 */

/*! \  son_oam_periodic_nw_scan_req_t
 *  \brief Structure containing Periodic NMM Scan Request
 *  \param  scan_on_off         Periodic scan is switched on/off
 *  \param  scan_time_interval  Periodic interval after which NMM Scan should be initiated
 *  \param  periodic_nw_scan    NMM Scan Request
 */
typedef struct _son_oam_periodic_nw_scan_req_t
{
    son_u32     bitmask;       /*^ BITMASK ^*/
    son_u8      scan_on_off;        /*^ M, 0, H, 0, 1 ^*/     
    son_u32     scan_time_interval;   /*^ M, 0, N, 0, 0 ^*/
    son_start_intra_rat_scan_req_t   start_intra_rat_periodic_scan_req; 
    /*^ O, SON_INTRA_RAT_PERIODIC_NW_SCAN_REQ_PRESENT, N, 0, 0 ^*/
    son_start_inter_rat_scan_req_t   start_inter_rat_periodic_scan_req; 
    /*^ O, SON_INTER_RAT_PERIODIC_NW_SCAN_REQ_PRESENT, N, 0, 0 ^*/
}son_oam_periodic_nw_scan_req_t; /*^ API, SON_OAM_PERIODIC_NW_SCAN_REQ ^*/

/*
 * SON_OAM_ANR_MEAS_CONFIG_REQ
 */

/*! \  son_speed_state_scale_factors_t
 *  \brief Structure storing parameter "Speed dependent ScalingFactor"
 *  \param sf_medium Factors to be applied when UE is in medium speed state. 
 *  \param sf_high   Factors to be applied when UE is in medium speed state. 
 */
typedef struct _son_speed_state_scale_factors_t
{
    son_speed_state_scale_factors_sf_medium_et sf_medium;   /*^ M, 0, H, 0, 3 ^*/
    son_speed_state_scale_factors_sf_high_et   sf_high;     /*^ M, 0, H, 0, 3 ^*/
}son_speed_state_scale_factors_t;

#define SON_MC_EUTRAN_ALLOW_MEAS_BW_PRESENT                 0x01
#define SON_MC_EUTRAN_PRESENCE_ANTENNA_PORT_PRESENT         0x02
#define SON_MC_EUTRAN_NBR_CELL_CONFIG_PRESENT               0x04
#define SON_MC_EUTRAN_OFFSET_FREQ_PRESENT                   0x08
#define SON_MC_EUTRAN_Q_RX_LEV_MIN_PRESENT                  0x10
#define SON_MC_EUTRAN_P_MAX_PRESENT                         0x20
#define SON_MC_EUTRAN_RESELECTION_PRESENT                   0x40
#define SON_MC_EUTRAN_RESELECTION_SF_PRESENT                0x80
#define SON_MC_EUTRAN_CELL_THRESH_HIGH_PRESENT              0x100
#define SON_MC_EUTRAN_CELL_THRESH_LOW_PRESENT               0x200
#define SON_MC_EUTRAN_CELL_RESELECTION_PRIORITY_PRESENT     0x400
#define SON_MC_EUTRAN_Q_QUAL_MIN_R9                         0x800
#define SON_MC_EUTRAN_THRESHX_HIGH_Q_R9                     0x1000
#define SON_MC_EUTRAN_THRESHX_LOW_Q_R9                      0x2000
#define SON_MC_EUTRAN_CM_OFFSET_FREQ_PRESENT                0x4000
#define SON_MC_INTER_FREQ_OPEN_PCI_RANGE_PRESENT            0x8000
#define SON_MC_INTER_FREQ_CLOSED_PCI_RANGE_PRESENT          0x10000
#define SON_MC_INTER_FREQ_HYBRID_PCI_RANGE_PRESENT          0x20000
 
/*! \  son_meas_config_eutran_t
 *  \brief Structure containing UE Measurement Configuration for EUTRAN Frequencies.
 *  \param bitmask
 *  \param earfcn                       EARFCN Id to Identify E-UTRA carrier 
 *                                      frequency for which this configuration
 *                                      is valid.
 *  \param meas_bandwidth_for_earfcn    Maximum allowed measurement bandwidth
 *                                      on a carrier frequency.
 *  \param presence_antenna_port        Indicate whether all the neighbouring 
 *                                      cells use Antenna Port 1.
 *  \param neighbor_cell_config         Information related to MBSFN and TDD 
 *                                      UL/DL configuration of neighbour cells.
 *  \param offset_frequency             Offset value applicable to the carrier 
 *                                      frequency.
 *  \param q_rx_lev_min                 Required minimum received RSRP level 
 *                                      in the (E-UTRA) cell for cell
 *                                      selection/ re-selection.
 *  \param p_max                        Power value applicable for the 
 *                                      neighbouring E-UTRA cells on this
 *                                      carrier frequency
 *  \param t_reselection                Parameter "TreselectionEUTRA"
 *  \param t_reselection_sf             Parameter "Speed dependent 
 *                                      ScalingFactor for TreselectionEUTRA"
 *  \param threshX_high                 Parameter "ThreshX, HighP"
 *  \param threshX_low                  Parameter "ThreshX, LowP"
 *  \param cell_reselection_priority    Absolute priority of the concerned 
 *                                      carrier frequency/ set of frequencies.
 *  \param cm_offset_frequency          CM offset frequency
 *  \param inter_freq_open_pci_range    Open Inter frequency pci range   //Added for SPR-9574 
 *  \param inter_freq_closed_pci_range  Closed Inter frequency pci range   //Added for SPR-9574
 *  \param inter_freq_hybrid_pci_range  Hybrid Inter frequency pci range   //Added for SPR-9574
 *
 */
typedef struct _son_meas_config_eutran_t
{
    son_u32                             bitmask;                   /*^ BITMASK ^*/
    son_u32                             earfcn;                    
    /*^ M, 0, N, 0, 0 ^*/
    son_allowed_meas_bandwidth_et       meas_bandwidth_for_earfcn; 
    /*^ O,SON_MC_EUTRAN_ALLOW_MEAS_BW_PRESENT, H, 0, 5 ^*/
    son_u8                              presence_antenna_port;     
    /*^ O,SON_MC_EUTRAN_PRESENCE_ANTENNA_PORT_PRESENT, H, 0, 1 ^*/
    son_u8                              neighbor_cell_config;   
    /*^ O,SON_MC_EUTRAN_NBR_CELL_CONFIG_PRESENT, N, 0, 0 ^*/
    son_q_offset_range_et               offset_frequency;          
    /*^ O,SON_MC_EUTRAN_OFFSET_FREQ_PRESENT, H, 0, 30 ^*/
    son_s8                              q_rx_lev_min;              
    /*^ O,SON_MC_EUTRAN_Q_RX_LEV_MIN_PRESENT, B, -70, -22 ^*/
    son_s8                              p_max;                     
    /*^ O,SON_MC_EUTRAN_P_MAX_PRESENT, B, -30, 33 ^*/
    son_u8                              t_reselection;             
    /*^ O, SON_MC_EUTRAN_RESELECTION_PRESENT, H, 0, 7 ^*/
    son_speed_state_scale_factors_t     t_reselection_sf;          
    /*^ O, SON_MC_EUTRAN_RESELECTION_SF_PRESENT, N, 0, 0 ^*/
    son_u8                              threshX_high;              
    /*^ O,SON_MC_EUTRAN_CELL_THRESH_HIGH_PRESENT , H, 0, 31 ^*/
    son_u8                              threshX_low;               
    /*^ O,SON_MC_EUTRAN_CELL_THRESH_LOW_PRESENT, H, 0, 31 ^*/
    son_u8                              cell_reselection_priority; 
    /*^ O, SON_MC_EUTRAN_CELL_RESELECTION_PRIORITY_PRESENT, H, 0, 7 ^*/
    son_s8                              q_qualmin_r9;
    /*^ O, SON_MC_EUTRAN_Q_QUAL_MIN_R9, B, -34, -3 ^*/
    son_u8                              threshx_highq_r9;
    /*^ O, SON_MC_EUTRAN_THRESHX_HIGH_Q_R9, H, 0, 31 ^*/
    son_u8                              threshx_lowq_r9;
    /*^ O, SON_MC_EUTRAN_THRESHX_LOW_Q_R9, H, 0, 31 ^*/
    son_q_offset_range_et              cm_offset_frequency;
    /*^ O,SON_MC_EUTRAN_CM_OFFSET_FREQ_PRESENT, H, 0, 30 ^*/
    son_per_freq_pci_range_t            inter_freq_open_pci_range;
    /*^ O,SON_MC_INTER_FREQ_OPEN_PCI_RANGE_PRESENT, N, 0, 0 ^*/
    son_per_freq_pci_range_t            inter_freq_closed_pci_range;
    /*^ O,SON_MC_INTER_FREQ_CLOSED_PCI_RANGE_PRESENT, N, 0, 0 ^*/
    son_per_freq_pci_range_t            inter_freq_hybrid_pci_range;
    /*^ O,SON_MC_INTER_FREQ_HYBRID_PCI_RANGE_PRESENT, N, 0, 0 ^*/
}son_meas_config_eutran_t;

#define SON_UTRAN_PCI_FDD_RANGE_PRESENT 0x01

/*! \  son_utran_pci_fdd_range_t
 *  \brief Structure storing ranges of PCIs for which UTRA-FDD reporting is allowed 
 * \param bitmask
 * \param start         Starting PCI value
 * \param range         PCI range
 */
typedef struct _son_utran_pci_fdd_range_t
{
    son_u32                bitmask;  /*^ BITMASK ^*/
    son_u16                start;    
    /*^ M, 0, H, 0, 511 ^*/
    son_u16                range;    
    /*^ O, SON_UTRAN_PCI_FDD_RANGE_PRESENT, B, 2, 512 ^*/
}son_utran_pci_fdd_range_t;

#define SON_MC_UTRAN_FDD_OFFSET_FREQ_PRESENT                    0x01
#define SON_MC_UTRAN_FDD_CELL_RESELECTION_PRIORITY_PRESENT      0x02
#define SON_MC_UTRAN_FDD_THRESH_HIGH_PRESENT                    0x04
#define SON_MC_UTRAN_FDD_THRESH_LOW_PRESENT                     0x08
#define SON_MC_UTRAN_FDD_Q_RX_LEV_MIN_PRESENT                   0x10
#define SON_MC_UTRAN_FDD_P_MAX_PRESENT                          0x20
#define SON_MC_UTRAN_FDD_Q_QUAL_MIN_PRESENT                     0x40
#define SON_MC_UTRAN_FDD_CSG_REPORTING_CELL_PRESENT             0x80
#define SON_MC_UTRAN_THRESHX_HIGH_Q_R9                          0x100
#define SON_MC_UTRAN_THRESHX_LOW_Q_R9                           0x200

/*! \  son_meas_config_utran_fdd_t
 *  \brief Structure storing UE Measurement configuration for UTRAN (FDD) frequencies.
 * \param bitmask
 * \param uarfcn                        UTRA carrier frequency for which this 
 *                                      configuration is valid.
 * \param offset_frequency              Frequency specific offset to be applied
 *                                      when evaluating triggering conditions
 *                                      for measurement reporting.
 * \param cell_reselection_priority     Absolute priority of the concerned 
 *                                      carrier frequency/set of frequencies.
 * \param threshX_high                  Parameter "ThreshX, HighP"
 *  \param threshX_low                  Parameter "ThreshX, LowP"
 * \param q_rx_lev_min                  Parameter "Qrxlevmin"
 * \param p_max                         Maximum allowed transmission power on 
 *                                      the (uplink) carrier frequency
 * \param q_qual_min                    Parameter "Qqualmin"
 * \param num_csg_allowed_reporting_cell    Number of of physical cell 
 *                                          Ids for which UTRA-FDD reporting
 *                                          is allowed
 * \param csg_allowed_reporting_cell_list   List storing one or more ranges of 
 *                                          PCIs for which UTRA-FDD reporting
 *                                          is allowed
 */
typedef struct _son_meas_config_utran_fdd_t
{
    son_u32                             bitmask;                   /*^ BITMASK ^*/
    son_u16                             uarfcn;                    
    /*^ M, 0, H, 0,16383  ^*/
    son_s8                              offset_frequency;          
    /*^ O,SON_MC_UTRAN_FDD_OFFSET_FREQ_PRESENT, B, -15, 15 ^*/
    son_u8                              cell_reselection_priority; 
    /*^ O, SON_MC_UTRAN_FDD_CELL_RESELECTION_PRIORITY_PRESENT, H, 0, 7 ^*/
    son_u8                              threshX_high;              
    /*^ O, SON_MC_UTRAN_FDD_THRESH_HIGH_PRESENT, H, 0, 31 ^*/
    son_u8                              threshX_low;               
    /*^ O, SON_MC_UTRAN_FDD_THRESH_LOW_PRESENT, H, 0, 31 ^*/
    son_s8                              q_rx_lev_min;              
    /*^ O,SON_MC_UTRAN_FDD_Q_RX_LEV_MIN_PRESENT, B, -60, -13 ^*/
    son_s8                              p_max;                     
    /*^ O, SON_MC_UTRAN_FDD_P_MAX_PRESENT, B, -50, 33 ^*/
    son_s8                              q_qual_min;                
    /*^ O,SON_MC_UTRAN_FDD_Q_QUAL_MIN_PRESENT, B, -24, 0 ^*/
    son_u8                              num_csg_allowed_reporting_cell;

/*SPR_17664_start*/
    /*^ O, SON_MC_UTRAN_FDD_CSG_REPORTING_CELL_PRESENT , B, 1,SON_MAX_PCI_RANGE ^*/
/*SPR_17664_end*/
    son_utran_pci_fdd_range_t           csg_allowed_reporting_cell_list[SON_MAX_PCI_RANGE];
    /*^ O, SON_MC_UTRAN_FDD_CSG_REPORTING_CELL_PRESENT ,OCTET_STRING, VARIABLE ^*/
    son_u8                              threshx_highq_r9;
    /*^ O, SON_MC_UTRAN_THRESHX_HIGH_Q_R9, H, 0, 31 ^*/
    son_u8                              threshx_lowq_r9;
    /*^ O, SON_MC_UTRAN_THRESHX_LOW_Q_R9, H, 0, 31 ^*/
}son_meas_config_utran_fdd_t;

#define SON_MC_UTRAN_TDD_OFFSET_FREQ_PRESENT                    0x01
#define SON_MC_UTRAN_TDD_CELL_RESELECTION_PRIORITY_PRESENT      0x02
#define SON_MC_UTRAN_TDD_THRESH_HIGH_PRESENT                    0x04
#define SON_MC_UTRAN_TDD_THRESH_LOW_PRESENT                     0x08
#define SON_MC_UTRAN_TDD_Q_RX_LEV_MIN_PRESENT                   0x10
#define SON_MC_UTRAN_TDD_P_MAX_PRESENT                          0x20
/*! \  son_meas_config_utran_tdd_t
 *  \brief Structure storing UTRAN TDD cell specific parameters for UTRAN cell UE measurements
 * \param bitmask
 * \param uarfcn                        UTRA carrier frequency for which this 
 *                                      configuration is valid.
 * \param offset_frequency              Frequency specific offset to be applied
 *                                      when evaluating triggering conditions
 *                                      for measurement reporting.
 * \param cell_reselection_priority     Absolute priority of the concerned 
 *                                      carrier frequency/set of frequencies.
 * \param threshX_high                  Parameter "ThreshX, HighP"
 * \param threshX_low                   Parameter "ThreshX, LowP"
 * \param q_rx_lev_min                  Parameter "Qrxlevmin"
 * \param p_max                         Maximum allowed transmission power on 
 *                                      the (uplink) carrier frequency
 */
typedef struct _son_meas_config_utran_tdd_t
{
    son_u32                             bitmask;
    /*^ BITMASK ^*/
    son_u16                             uarfcn;                    
    /*^ M, 0, H, 0, 16383  ^*/
    son_s8                              offset_frequency;          
    /*^ O, SON_MC_UTRAN_TDD_OFFSET_FREQ_PRESENT, B, -15, 15 ^*/
    son_u8                              cell_reselection_priority; 
    /*^ O, SON_MC_UTRAN_TDD_CELL_RESELECTION_PRIORITY_PRESENT, H, 0, 7 ^*/
    son_u8                              threshX_high;              
    /*^ O, SON_MC_UTRAN_TDD_THRESH_HIGH_PRESENT, H, 0, 31 ^*/
    son_u8                              threshX_low;               
    /*^ O, SON_MC_UTRAN_TDD_THRESH_LOW_PRESENT, H, 0, 31 ^*/
    son_s8                              q_rx_lev_min;              
    /*^ O, SON_MC_UTRAN_TDD_Q_RX_LEV_MIN_PRESENT, B, -60, -13 ^*/
    son_s8                              p_max;                     
    /*^ O, SON_MC_UTRAN_TDD_P_MAX_PRESENT, B, -50, 33 ^*/
}son_meas_config_utran_tdd_t;

#define SON_UTRAN_RESELECTION_PRESENT    0x01
#define SON_UTRAN_RESELECTION_SF_PRESENT       0x02

/*! \  son_meas_config_utran_t
 *  \brief Structure storing UTRAN cell specific parameters for UTRAN cell UE measurements
 *  \param bitmask
 *  \param utran_fdd_list_size      Number of UTRA  FDD cells to find UE 
 *                                  measurement
 *  \param utran_fdd_list           List storing UTRAN FDD cell specific 
 *                                  parameters for each UTRAN cell
 *                                  UE measurements
 *  \param utran_tdd_list_size      Number of UTRA  TDD cells to find UE 
 *                                  measurement
 *  \param utran_tdd_list           List storing UTRAN TDD cell specific 
 *                                  parameters for each UTRAN cell UE
 *                                  measurements
 *  \param t_reselection            Cell reselection timer value for UTRAN
 *  \param t_reselection_sf         Speed dependent ScalingFactor for Cell 
 *                                  reselection timer value
 */
typedef struct _son_meas_config_utran_t
{
    son_u32                             bitmask;                   /*^ BITMASK ^*/
    son_u8                              utran_fdd_list_size;                                
    /* SPR-12875 Fix Starts */

/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_MAX_CONFIG_OBJECT ^*/
/*SPR_17664_end*/
    /* SPR-12875 Fix Ends */
    son_meas_config_utran_fdd_t         utran_fdd_list[SON_MAX_CONFIG_OBJECT];     
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    son_u8                              utran_tdd_list_size;                                
    /* SPR-12875 Fix Starts */
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_MAX_CONFIG_OBJECT ^*/
/*SPR_17664_end*/
    /* SPR-12875 Fix Ends */
    son_meas_config_utran_tdd_t         utran_tdd_list[SON_MAX_CONFIG_OBJECT];     
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    son_u8                              t_reselection;                                      
    /*^ O, SON_UTRAN_RESELECTION_PRESENT, H, 0, 7 ^*/
    son_speed_state_scale_factors_t     t_reselection_sf;                                   
    /*^ O, SON_UTRAN_RESELECTION_SF_PRESENT, N, 0, 0 ^*/        
}son_meas_config_utran_t;

#define SON_MC_GERAN_EXP_ARFCN_SET_PRESENT       0x01
#define SON_MC_GERAN_EQ_SP_ARFCN_SET_PRESENT     0x02
#define SON_MC_GERAN_ARFCN_BITMAP_SET_PRESSENT   0x04

/*! \  son_geran_following_arfcns_t
 *  \brief Structure storing GERAN carrier freqs GERAN cell UE measurements
 *  \param bitmask
 *  \param exp_arfcn_list_size       Number of ARFCNs
 *  \param exp_arfcn_list            List containing ARFCNs 
 *  \param arfcn_spacing             Spacing value
 *  \param num_of_following_arfcn    Number of equally spaced GERAN carrier freqs
 *  \param arfcn_bmp_list_size       Number of octets in bitmap
 *  \param arfcn_bmp_list             Bitmap representing ERAFCNs
 */
typedef struct _son_geran_following_arfcns_t
{
    son_u32 bitmask; /*^ BITMASK ^*/
   son_u8                   exp_arfcn_list_size;
/* SPR-18445 start */
   /*^ O, SON_MC_GERAN_EXP_ARFCN_SET_PRESENT, B, 1, SON_MAX_GERAN_EXP_ARFCN_LIST_SIZE ^*/
/* SPR-18445 stop */
   son_u16                  exp_arfcn_list[SON_MAX_GERAN_EXP_ARFCN_LIST_SIZE];
   /*^ O, SON_MC_GERAN_EXP_ARFCN_SET_PRESENT, OCTET_STRING, VARIABLE ^*/
   son_u8                   arfcn_spacing;
   /*^ O, SON_MC_GERAN_EQ_SP_ARFCN_SET_PRESENT, B, 1, 8  ^*/
   son_u8                  num_of_following_arfcn;
   /*^ O, SON_MC_GERAN_EQ_SP_ARFCN_SET_PRESENT, H, 0, 31 ^*/
   son_u8                   arfcn_bmp_list_size;
/*SPR_17664_start*/
   /*^ O, SON_MC_GERAN_ARFCN_BITMAP_SET_PRESSENT, B, 1, SON_MAX_NUM_OF_OCTETS_IN_VAR_BITMAP ^*/
/*SPR_17664_end*/
   son_u8                   arfcn_bmp_list[SON_MAX_NUM_OF_OCTETS_IN_VAR_BITMAP];
   /*^ O, SON_MC_GERAN_ARFCN_BITMAP_SET_PRESSENT, OCTET_STRING, VARIABLE ^*/
}son_geran_following_arfcns_t;

/*! \  son_geran_carrier_freqs_t
 *  \brief Structure storing GERAN carrier freqs GERAN cell UE measurements
 *  \param starting_arfcn             Starting ARFCN of the carrier freq set
 *  \param band_ind                   Indicates how to interpret an associated GERAN carrier ARFCN.
 *  \param following_arfcns           Set of remaining ARFCNs 
 */
typedef struct _son_geran_carrier_freqs_t
{
    son_u16                       starting_arfcn;
    /*^ M, 0, H, 0, 1023 ^*/
    son_geran_band_indicator_et   band_ind;
    /*^ M, 0, H, 0, 1 ^*/
    son_geran_following_arfcns_t  following_arfcns;
    /*^ M, 0, N, 0, 0 ^*/
}son_geran_carrier_freqs_t;

#define SON_GERAN_CFI_CELL_RESELECTION_PRIORITY_PRESENT         0x01
#define SON_GERAN_CFI_NCC_PERMITTED_PRESENT                     0x02
#define SON_GERAN_CFI_Q_RX_LEV_MIN_PRESENT                      0x04
#define SON_GERAN_CFI_MAX_ALLOWED_TRANS_PWR_PRESENT             0x08
#define SON_GERAN_CFI_THRESH_HIGH_PRESENT                       0x10
#define SON_GERAN_CFI_THRESH_LOW_PRESENT                        0x20
#define SON_GERAN_CFI_OFFSET_FREQ_PRESENT                       0x40

/*! \  son_geran_carrier_freq_info_t
 *  \brief Structure storing GERAN carrier freq information for GERAN cell UE measurements
 *  \param bitmask
 *  \param geran_car_freqs            Set of GERAN carrier freqs
 *
 *  \param cell_reselection_priority  Concerns the absolute priority of the 
 *                                    concerned carrier frequency/ set of frequencies (GERAN).
 *  \param ncc_permitted              Bit map, where bit N is set to "0" if a BCCH carrier
 *                                    with NCC = N-1 is not permitted for monitoring and set to
 *                                    "1" if the BCCH carrier with NCC = N-1 is permitted for monitoring
 *  \param q_rx_lev_min               Parameter .Qrxlevmin..Minimum required RX level
 *                                    in the GSM cell
 *  \param p_max_geran                Maximum allowed transmission power for GERAN 
 *                                    on an uplink carrier frequency.
 *  \param threshX_high               Parameter .ThreshX, HighP. 
 *  \param threshX_low                Parameter .ThreshX, LowP.
 *  \param offset_frequency           Indicates a frequency specific offset to be 
 *                                    applied when evaluating triggering conditions
 *                                    for measurement reporting
 */

typedef struct _son_geran_carrier_freq_info_t
{
    son_u32                      bitmask;  /*^ BITMASK ^*/
    son_geran_carrier_freqs_t      geran_car_freqs;
    /*^ M, 0, N, 0, 0 ^*/                   
    son_u8                       cell_reselection_priority;
    /*^ O, SON_GERAN_CFI_CELL_RESELECTION_PRIORITY_PRESENT, M, 0, 7 ^*/
    son_u8                       ncc_permitted;
    /*^ O, SON_GERAN_CFI_NCC_PERMITTED_PRESENT, N, 0, 0 ^*/ 
    son_u8                       q_rx_lev_min;
    /*^ O,SON_GERAN_CFI_Q_RX_LEV_MIN_PRESENT, H, 0, 45 ^*/ 
    son_u8                       p_max_geran;
    /*^ O, SON_GERAN_CFI_MAX_ALLOWED_TRANS_PWR_PRESENT, H, 0, 39 ^*/
    son_u8                       threshX_high;
    /*^ O, SON_GERAN_CFI_THRESH_HIGH_PRESENT, H, 0, 31 ^*/
    son_u8                       threshX_low;
    /*^ O, SON_GERAN_CFI_THRESH_LOW_PRESENT, H, 0, 31 ^*/
    son_s8                       offset_frequency;
    /*^ O, SON_GERAN_CFI_OFFSET_FREQ_PRESENT, B, -15, 15 ^*/
}son_geran_carrier_freq_info_t;

#define SON_MC_GERAN_RESELECTION_PRESENT            0x01
#define SON_MC_GERAN_RESELECTION_SF_PRESENT            0x02

/*! \  son_meas_config_geran_t
 *  \brief Structure storing GERAN cell specific parameters for GERAN cell UE measurements
 *  \param bitmask
 *  \param geran_freq_info_list_size Number of GERAN carrier freq infos given in the list
 *  \param geran_freq_info_list      List storing GERAN carrier freq info for UE measurement  
 *  \param t_reselection             Cell re-selection timer value for GERAN
 *  \param t_reselection_sf          Speed dependent ScalingFactor for Cell 
 *                                   re-selection timer value
 */
typedef struct _son_meas_config_geran_t
{
    son_u32                             bitmask;                   /*^ BITMASK ^*/
    son_u8                              geran_freq_info_list_size;                                
    /* SPR-12875 Fix Starts */
/*SPR_17664_start*/
    /*^ M, 0,H, 0, SON_MAX_CONFIG_OBJECT ^*/
/*SPR_17664_end*/
    /* SPR-12875 Fix Ends */
    son_geran_carrier_freq_info_t       geran_freq_info_list[SON_MAX_CONFIG_OBJECT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    son_u8                              t_reselection;                                      
    /*^ O, SON_MC_GERAN_RESELECTION_PRESENT, H, 0, 7 ^*/
    son_speed_state_scale_factors_t     t_reselection_sf;                                   
    /*^ O, SON_MC_GERAN_RESELECTION_SF_PRESENT, N, 0, 0 ^*/        
}son_meas_config_geran_t;

#define SON_MAX_CDMA_SYS_TIME 7

/*! \  son_cdma_sys_time_info_t
 *  \brief Structure storing CDMA System Time Information
 *  \param is_cdma_nw_sync      TRUE indicates that the networks are synchronised i.e.
 *                              there is no drift in the timing between E UTRA and CDMA2000.
 *                              FALSE indicates that the networks are not synchronised,
 *                              i.e. the timing between E-UTRA and CDMA2000 can drift.
 *  \param cdma_sys_time_size   CDMA2000 system time size
 *  \param cdma_sys_time        CDMA2000 system time corresponding to the SFN boundary at
 *                              or after the ending boundary of the SI-window in which
 *                              SystemInformationBlockType8 is transmitted. If synchronized
 *                              to CDMA2000 system time then the size is 39 bits and the unit
 *                              is 10 ms based on a 1.2288 Mcps chip rate.If not synchronized
 *                              then the size is 49 bits and the unit is [8 CDMA2000 chips
 *                              based on 1.2288 Mcps].
 */
typedef struct _son_cdma_sys_time_info_t
{
    son_bool_et is_cdma_nw_sync;
    /*^ M, 0, H, 0, 1 ^*/
    son_u8      cdma_sys_time_size;
/*SPR_17664_start*/
    /*^ M, 0, B, 5,SON_MAX_CDMA_SYS_TIME ^*/
/*SPR_17664_end*/
    son_u8      cdma_sys_time[SON_MAX_CDMA_SYS_TIME];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_cdma_sys_time_info_t;

#define SON_HRPD_PRE_REG_ZONE_ID_PRESENT      0x01
#define SON_HRPD_SEC_PRE_REG_ZONE_ID_PRESENT  0x02
#define SON_MAX_SEC_PRE_REG_ZONES        2

/*! \  son_pre_reg_hrpd_info_t
 *  \brief Structure storing CDMA pre-reg HRPD neighbor specific Info 
 *  \param bitmask                          - Bitmask value
 *  \param pre_reg_allowed                  - TRUE indicates that a UE shall perform a CDMA2000 HRPD 
 *                                            pre-registration if the UE does not have a valid / current pre-registration
 *  \param pre_reg_zone_id                  - ColorCode (see C.S0024-A [26], C.S0087-A [44]) of the 
 *                                            CDMA2000 Reference Cell corresponding to the HRPD sector 
 *                                            under the HRPD AN that is configured for this LTE cell.
 *                                            It is used to control when the UE should register or re-register 
 *  \param scndry_pre_reg_zone_id_list_size - Secondary Pre registration Zone Id List Size 
 *  \param scndry_pre_reg_zone_id_list      - List of SecondaryColorCodes (see C.S0024-A [26], C.S0087-A [44])
 *                                            of the CDMA2000 Reference Cell corresponding to the HRPD sector 
 *                                            under the HRPD AN that is configured for this LTE cell.
 *                                            They are used to control when the UE should re-register.
 */
typedef struct _son_pre_reg_hrpd_info_t
{
    son_u32              bitmask;            /*^ BITMASK ^*/
    son_bool_et          pre_reg_allowed;
    /*^ M, 0, H, 0, 1 ^*/
    son_u8               pre_reg_zone_id;
    /*^ O, SON_HRPD_PRE_REG_ZONE_ID_PRESENT, N, 0, 0 ^*/
    son_u8               scndry_pre_reg_zone_id_list_size;
/*SPR_17664_start*/
    /*^ O, SON_HRPD_SEC_PRE_REG_ZONE_ID_PRESENT, B, 1, SON_MAX_SEC_PRE_REG_ZONES ^*/
/*SPR_17664_end*/
    son_u8               scndry_pre_reg_zone_id_list[SON_MAX_SEC_PRE_REG_ZONES];
    /*^ O, SON_HRPD_SEC_PRE_REG_ZONE_ID_PRESENT, OCTET_STRING, VARIABLE ^*/
}son_pre_reg_hrpd_info_t;

/*! \  son_cdma_csfb_reg_param_1XRTT_t
 *  \brief Structure storing CDMA pre-reg HRPD neighbor specific Info 
 *  \param bitmask          - Bitmask value
 *  \param sid              - Used along with the nid as a pair to control when the UE 
 *                            should Register or Re-Register with the CDMA2000 1xRTT network.
 *  \param nid              - Used along with the sid as a pair to control when the UE 
 *                            should Register or Re-Register with the CDMA2000 1xRTT network.
 *  \param multiple_sid     - The CDMA2000 1xRTT Multiple SID storage indicator
 *  \param multiple_nid     - The CDMA2000 1xRTT Multiple NID storage indicator
 *  \param home_reg         - The CDMA2000 1xRTT Home registration indicator
 *  \param foreign_sid_reg  - The CDMA2000 1xRTT SID roamer registration indicator
 *  \param foreign_nid_reg  - The CDMA2000 1xRTT NID roamer registration indicator
 *  \param parameter_reg    - The CDMA2000 1xRTT Parameter-change registration indicator.
 *  \param power_up_reg     - The CDMA2000 1xRTT Power-up registration indicator.
 *  \param reg_period       - The CDMA2000 1xRTT Registration period.
 *  \param reg_zone         - The CDMA2000 1xRTT Registration zone.
 *  \param total_zone       - The CDMA2000 1xRTT Number of registration zones to be retained.
 *  \param zone_timer       - The CDMA2000 1xRTT Zone timer length.
 */
typedef struct _son_cdma_csfb_reg_param_1XRTT_t
{
    son_u16                                      sid;
    /*^ M, 0, N, 0, 0 ^*/
    son_u16                                      nid;
    /*^ M, 0, N, 0, 0 ^*/
    son_bool_et                                 multiple_sid;
    /*^ M, 0, H, 0, 1 ^*/
    son_bool_et                                 multiple_nid;
    /*^ M, 0, H, 0, 1 ^*/
    son_bool_et                                 home_reg;
    /*^ M, 0, H, 0, 1 ^*/
    son_bool_et                                 foreign_sid_reg;
    /*^ M, 0, H, 0, 1 ^*/
    son_bool_et                                 foreign_nid_reg;
    /*^ M, 0, H, 0, 1 ^*/
    son_bool_et                                 parameter_reg;
    /*^ M, 0, H, 0, 1 ^*/
    son_bool_et                                 power_up_reg;
    /*^ M, 0, H, 0, 1 ^*/
    son_u8                                      reg_period;
    /*^ M, 0, N, 0, 0 ^*/
    son_u16                                     reg_zone;
    /*^ M, 0, N, 0, 0 ^*/
    son_u8                                      total_zone;
    /*^ M, 0, N, 0, 0 ^*/
    son_u8                                      zone_timer;
    /*^ M, 0, N, 0, 0 ^*/
}son_cdma_csfb_reg_param_1XRTT_t;

/*! \  son_cdma_carrier_freq_info_t
 *  \brief Structure storing Carrier Frequency Information
 *  \param bitmask
 *  \param cdma_type                    The type of CDMA2000 network: CDMA2000 1xRTT or CDMA2000 HRPD.
 *  \param band_class                   Identifies CDMA2000 Band
 *  \param arfcn                        Identifies CDMA2000 arfcn on a band
 *  \param t_reselection                Parameter <93>TreselectionCDMA_HRPD<94> or <93>TreselectionCDMA_1xRTT<94>
 *                                      in TS 36.304
 *  \param threshX_high                 Parameter <93>ThreshX, HighP<94> in TS 36.304 [4]. This specifies the high
 *                                      threshold used in reselection towards this CDMA2000 band class expressed as
 *                                      an unsigned binary number equal to FLOOR (-2 x 10 x log10 Ec/Io) in units of 0.5 dB,
 *  \param threshX_low                  Parameter <93>ThreshX, LowP<94> in TS 36.304 [4]. This specifies the low
 *                                      threshold used in reselection towards this CDMA2000 band class expressed
 *                                      as an unsigned binary number equal to FLOOR (-2 x 10 x log10 Ec/Io) in units
 *                                      of 0.5 dB
 *  \param srch_wnd_size                The search window size is a CDMA2000 parameter to be used to assist in
 *                                      searching for the neighbouring pilots. For values see C.S0005-A [25,
 *                                      Table 2.6.6.2.1-1] and C.S0024-A [26, Table 8.7.6.2-4]. This field is
 *                                      required for a UE with rx-ConfigHRPD= `single' and/ or rx-Config1XRTT=
 *                                      `single' to perform handover, cell re-selection, UE measurement based
 *                                      redirection and enhanced 1xRTT CS fallback from E-UTRAN to CDMA2000
 *                                      according to this specification and TS 36.304
 *  \param t_reselection_sf             parameter <93>Speed dependent ScalingFactor for TreselectionCDMA-HRPD
 *                                      <94> or TreselectionCDMA-1xRTT<94> in TS 36.304
 *  \param cell_reselection_priority
 */
#define SON_CDMA_FREQ_OFFSET_FREQ_PRESENT                   0x01
#define SON_CDMA_FREQ_CDMA_SRCH_WNDO_SIZE_PRESENT           0x02
#define SON_CDMA_FREQ_CELL_RESELECTION_PRIORITY_PRESENT     0x04
typedef struct _son_cdma_carrier_freq_info_t
{
    son_u32 bitmask; /*^ BITMASK ^*/
    son_cdma2000_type_et                cdma_type;
    /*^ M, 0, H, 0, 1 ^*/
    son_cdma_band_class_et              band_class;
    /*^ M, 0, H, 0, 31 ^*/
    son_u16                             arfcn;
    /*^ M, 0, H, 0, 2047 ^*/
    son_u8                              threshX_high;
    /*^ M, 0, H, 0, 63 ^*/
    son_u8                              threshX_low;
    /*^ M, 0, H, 0, 63 ^*/
    son_s8                              offset_freq;
    /*^ O, SON_CDMA_FREQ_OFFSET_FREQ_PRESENT,B, -15, 15 ^*/
    son_u8                              srch_wnd_size;
    /*^ O, SON_CDMA_FREQ_CDMA_SRCH_WNDO_SIZE_PRESENT, H, 0, 15 ^*/
    son_u8                              cell_reselection_priority;
    /*^ O, SON_CDMA_FREQ_CELL_RESELECTION_PRIORITY_PRESENT, H, 0, 7 ^*/
}son_cdma_carrier_freq_info_t;

#define SON_CDMA_1XRTT_REG_PARAMS_PRESENT 0x01
#define SON_CDMA_1XRTT_LONG_CODE_PRESENT  0x02
#define SON_CDMA_1XRTT_RESELECTION_SF_PRESENT   0x04
#define SON_MAX_LONG_CODE_1XRTT_SIZE     6

/*! \  son_cdma_1xrtt_info_t
 *  \brief Structure storing CDMA XRTT neighbor specific Info 
 *  \param bitmask              - Bitmask value
 *  \param t_reselection        - Cell reselection timer value TreselectionRAT for CDMA. 
 *  \param xrtt_reg_params      - Contains the parameters the UE will use to determine 
 *                                if it should perform a CDMA2000 1xRTT 
 *                                Registration/Re-Registration. This field is included 
 *                                if either CSFB or enhanced CS fallback to CDMA2000 1xRTT 
 *                                is supported.
 *  \param cdma_1xrtt_long_code - The state of long code generation registers in CDMA2000 
 *                                1XRTT system at  ms, where t equals to the cdma-SystemTime.
 *                                This field is required for SRVCC handover and enhanced CS 
 *                                fallback to CDMA2000 1xRTT operation.
 *  \param t_reselection_sf     - Parameter Speed dependent ScalingFactor for TreselectionCDMA-HRPD or
 *                                TreselectionCDMA-1xRTT in TS 36.304
 */
typedef struct _son_cdma_1xrtt_info_t
{
    son_u32                             bitmask;    /*^ BITMASK ^*/
    son_u8                              t_reselection;
    /*^ M, 0 , H, 0, 7 ^*/
    son_cdma_csfb_reg_param_1XRTT_t     xrtt_reg_params;
    /*^ O, SON_CDMA_1XRTT_REG_PARAMS_PRESENT, N, 0, 0 ^*/
    son_u8                              cdma_1xrtt_long_code[SON_MAX_LONG_CODE_1XRTT_SIZE];
    /*^ O, SON_CDMA_1XRTT_LONG_CODE_PRESENT, OCTET_STRING, FIXED ^*/
    son_speed_state_scale_factors_t     t_reselection_sf;
    /*^ O, SON_CDMA_1XRTT_RESELECTION_SF_PRESENT, N, 0, 0 ^*/
}son_cdma_1xrtt_info_t;

/*! \  son_cdma_csfb_reg_param_1xrtt_r9_t
 *  \brief Structure storing CDMA XRTT neighbor CSFB registration params
 *  \param bitmask              - Bitmask value
 *  \param power_down_reg_r9    - The CDMA2000 1xRTT Power-down registration indicator.
 *                                If set to TRUE, the UE that has a valid / current CDMA2000 
 *                                1xRTT pre-registration will perform a CDMA2000 1xRTT power 
 *                                down registration when it is switched off.
 */
typedef struct _son_cdma_csfb_reg_param_1xrtt_r9_t
{
    son_u32     bitmask;    /*^ BITMASK ^*/
    son_bool_et power_down_reg_r9;  /*^ M, 0, H, 0, 1^*/
}son_cdma_csfb_reg_param_1xrtt_r9_t;

/*! \  son_ac_barring_config_1xrtt_t
 *  \brief Structure storing CDMA AC Barring Config info
 *  \param bitmask              - Bitmask value
 *  \param ac_barring_0_to_9_r9 - Parameter used for calculating the access class barring 
 *                                factor for access overload classes 0 through 9. 
 *                                It is the parameter PSIST in C.S0004-A [34] for access 
 *                                overload classes 0 through 9
 *  \param ac_barring_10_r9     - Parameter used for calculating the access class barring 
 *                                factor for access overload class N (N = 10 to 15).
 *  \param ac_barring_11_r9     - Parameter used for calculating the access class barring 
 *                                factor for access overload class N (N = 10 to 15).
 *  \param ac_barring_12_r9     - Parameter used for calculating the access class barring 
 *                                factor for access overload class N (N = 10 to 15).
 *  \param ac_barring_13_r9     - Parameter used for calculating the access class barring 
 *                                factor for access overload class N (N = 10 to 15).
 *  \param ac_barring_14_r9     - Parameter used for calculating the access class barring 
 *                                factor for access overload class N (N = 10 to 15).
 *  \param ac_barring_15_r9     - Parameter used for calculating the access class barring 
 *                                factor for access overload class N (N = 10 to 15).
 *  \param ac_barring_msg_r9    - Parameter used for modifying the access class barring 
 *                                factor for message transmissions.
 *  \param ac_barring_reg_r9    - Parameter used for modifying the access class barring 
 *                                factor for autonomous registrations
 *  \param ac_barring_emg_r9    - Parameter used for calculating the access class barring 
 *                                factor for emergency calls and emergency message transmissions 
 *                                for access overload classes 0 through 9.
 */
typedef struct _son_ac_barring_config_1xrtt_t
{
    son_u32 bitmask;    /*^ BITMASK ^*/
    son_u8  ac_barring_0_to_9_r9;
    /*^ M, 0, H, 0, 63 ^*/
    son_u8  ac_barring_10_r9;
    /*^ M, 0, H, 0, 7 ^*/
    son_u8  ac_barring_11_r9;
    /*^ M, 0, H, 0, 7 ^*/
    son_u8  ac_barring_12_r9;
    /*^ M, 0, H, 0, 7 ^*/
    son_u8  ac_barring_13_r9;
    /*^ M, 0, H, 0, 7 ^*/
    son_u8  ac_barring_14_r9;
    /*^ M, 0, H, 0, 7 ^*/
    son_u8  ac_barring_15_r9;
    /*^ M, 0, H, 0, 7 ^*/
    son_u8  ac_barring_msg_r9;
    /*^ M, 0, H, 0, 7 ^*/
    son_u8  ac_barring_reg_r9;
    /*^ M, 0, H, 0, 7 ^*/
    son_u8  ac_barring_emg_r9;
    /*^ M, 0, H, 0, 7 ^*/
}son_ac_barring_config_1xrtt_t;

#define SON_CDMA_HRPD_REG_PARAMS_PRESENT 0x01
#define SON_CDMA_HRPD_RESELECTION_PRESENT 0x02
#define SON_CDMA_HRPD_RESELECTION_SF_PRESENT 0x04

/*! \  son_cdma_hrpd_info_t
 *  \brief Structure storing CDMA XRTT neighbor CSFB registration params
 *  \param bitmask          - Bitmask value
 *  \param hrpd_reg_params  - To see pre-registered HRPD network NBR
 *  \param t_reselection    - Cell reselection timer value TreselectionRAT for CDMA. 
 *  \param t_reselection_sf - Parameter Speed dependent ScalingFactor for TreselectionCDMA-HRPD
 *                            or TreselectionCDMA-1xRTT in TS 36.304 [4]. If the field is not 
 *                            present, the UE behaviour is specified in TS 36.304 [4].
 */
typedef struct _son_cdma_hrpd_info_t
{
    son_u32                             bitmask; /*^ BITMASK ^*/
    son_pre_reg_hrpd_info_t             hrpd_reg_params;
    /*^ O, SON_CDMA_HRPD_REG_PARAMS_PRESENT, N, 0, 0 ^*/
    son_u8                              t_reselection;
    /*^ O, SON_CDMA_HRPD_RESELECTION_PRESENT, H, 0, 7 ^*/
    son_speed_state_scale_factors_t     t_reselection_sf;
    /*^ O, SON_CDMA_HRPD_RESELECTION_SF_PRESENT, N, 0, 0 ^*/
}son_cdma_hrpd_info_t;

#define SON_MEAS_CONFIG_CDMA_SYS_TIME_INFO_PRESENT              0x01
#define SON_MEAS_CONFIG_CDMA_SRCH_WNDO_SIZE_PRESENT             0x02
#define SON_MEAS_CONFIG_CDMA_PRE_REG_HRPD_INFO_PRESENT          0x04
#define SON_MEAS_CONFIG_CDMA_1X_RTT_INFO_PRESENT                0x08
#define SON_MEAS_CONFIG_CSFB_SUPPORT_FOR_DUAL_RX_UES_PRESENT    0x10
#define SON_MEAS_CONFIG_CSFB_REG_PARAM_1XRTT_PRESENT            0x20
#define SON_MEAS_CONFIG_AC_BARRING_CONFIG_1XRTT_PRESENT         0x40

/*! \  son_meas_config_cdma_t
 *  \brief Structure storing CDMA Meas config request
 *  \param bitmask                          - Bitmask value
 *  \param cdma_freq_info_list_size         - Size of CDMA2000 frequencies List
 *  \param cdma_freq_info_list              - List of CDMA2000 frequencies to be used for 
 *                                            neighbor detection via UE measurements
 *  \param sys_time_info                    - Information on CDMA2000 system time.
 *  \param srch_wnd_size                    - The search window size is a CDMA2000 parameter 
 *                                            to be used to assist in searching for the neighboring pilots.
 *  \param hrpd_info                        - Parameters applicable for interworking with CDMA2000 HRPD system.
 *  \param cdma_1xrtt_info                  - Parameters applicable for interworking with CDMA2000 1XRTT system.
 *  \param csfb_support_for_dual_Rx_Ues_r9  - Value TRUE indicates that the network supports dual Rx CSFB
 *  \param csfb_registration_param_1xrtt_r9 - Contains the parameters the UE will use to determine if it 
 *                                            should perform a CDMA2000 1xRTT Registration/Re-Registration.
 *  \param ac_barring_config_1xrtt_r9       - Contains the access class barring parameters the UE uses to 
 *                                            calculate the access class barring factor
 */
typedef struct _son_meas_config_cdma_t
{
    son_u32                             bitmask;            /*^ BITMASK ^*/
    son_u8                              cdma_freq_info_list_size; 
    /* SPR-12875 Fix Starts */
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_MAX_CONFIG_OBJECT^*/
/*SPR_17664_end*/
    /* SPR-12875 Fix Ends */
    son_cdma_carrier_freq_info_t        cdma_freq_info_list[SON_MAX_CONFIG_OBJECT]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    son_cdma_sys_time_info_t            sys_time_info; 
    /*^ O, SON_MEAS_CONFIG_CDMA_SYS_TIME_INFO_PRESENT, N, 0, 0 ^*/
    son_u8                              srch_wnd_size; 
    /*^ O, SON_MEAS_CONFIG_CDMA_SRCH_WNDO_SIZE_PRESENT, H, 0, 15 ^*/
    son_cdma_hrpd_info_t                hrpd_info; 
    /*^ O, SON_MEAS_CONFIG_CDMA_PRE_REG_HRPD_INFO_PRESENT, N, 0, 0 ^*/
    son_cdma_1xrtt_info_t               cdma_1xrtt_info;
    /*^ O, SON_MEAS_CONFIG_CDMA_1X_RTT_INFO_PRESENT, N, 0, 0 ^*/
    son_bool_et                         csfb_support_for_dual_Rx_Ues_r9;
    /*^ O, SON_MEAS_CONFIG_CSFB_SUPPORT_FOR_DUAL_RX_UES_PRESENT, H, 0, 1 ^*/
    son_cdma_csfb_reg_param_1xrtt_r9_t  csfb_registration_param_1xrtt_r9;
    /*^ O, SON_MEAS_CONFIG_CSFB_REG_PARAM_1XRTT_PRESENT, N, 0, 0 ^*/
    son_ac_barring_config_1xrtt_t       ac_barring_config_1xrtt_r9;
    /*^ O, SON_MEAS_CONFIG_AC_BARRING_CONFIG_1XRTT_PRESENT, N, 0, 0 ^*/
}son_meas_config_cdma_t;

/*! \  son_anr_meas_config_inter_rat_t
 *  \brief Structure storing Inter-Rat parameters for ANR UE measurements configurations
 *  \param utran_config_list        List of UTRAN frequency specific
 *                                  parameters for ANR related UE
 *                                  measurements
 *  \param geran_config_list        List of GERAN frequency specific
 *                                  parameters for ANR related UE
 *                                  measurements
 */
typedef struct _son_anr_meas_config_inter_rat_t
{
    son_meas_config_utran_t             utran_config_list;
    /*^ M, 0, N, 0, 0 ^*/
    son_meas_config_geran_t             geran_config_list;
    /*^ M, 0, N, 0, 0 ^*/
    son_meas_config_cdma_t              cdma_config_list;
    /*^ M, 0, N, 0, 0 ^*/
}son_anr_meas_config_inter_rat_t;

#define SON_ANR_GAP_OFFSET_GP0_PRESENT      0x01
#define SON_ANR_GAP_OFFSET_GP1_PRESENT      0x02

/*! \  son_setup_meas_gap_config_t
 *  \brief Structure for storing the setup gap  measurements  
 *  \param bitmask                      bitmask
 *  \param gap_offset_gp0              Corresponds to gap offset 
 *                                     of Gap Pattern Id .0. with MGRP = 40ms
 *  \param gap_offset_gp1              Corresponds to gap offset of Gap Pattern
 *                                     Id .1. with MGRP = 80ms
 */
typedef struct _son_setup_meas_gap_config_t
{
    son_u32 bitmask; /*^ BITMASK ^*/
    son_u8                         gap_offset_gp0;
     /*^ O, SON_ANR_GAP_OFFSET_GP0_PRESENT,H, 0, 39 ^*/
    son_u8                         gap_offset_gp1;
     /*^ O, SON_ANR_GAP_OFFSET_GP1_PRESENT,H, 0, 79 ^*/
}son_setup_meas_gap_config_t;

#define SON_ANR_SETUP_GAP_CONFIG_PRESENT      0x01

/*! \  son_gap_config_t
 *  \brief Structure storing the setup gap info  
 *  \param bitmask                      bitmask
 *  \param setup_gap_config             Specifies the measurement gap configuration
 *                                      and controls setup of measurement gaps.
 */
typedef struct _son_gap_config_t
{
   son_u32                        bitmask; /*^ BITMASK ^*/
   son_setup_meas_gap_config_t    setup_gap_config;
   /*^ O, SON_ANR_SETUP_GAP_CONFIG_PRESENT,N, 0, 0 ^*/
}son_gap_config_t;

#define SON_ANR_EUTRAN_GAP_CONFIG_PRESENT      0x01
#define SON_ANR_UTRAN_GAP_CONFIG_PRESENT       0x02
#define SON_ANR_GERAN_GAP_CONFIG_PRESENT       0x04
#define SON_ANR_CDMA2000_GAP_CONFIG_PRESENT    0x08

/*! \ son_meas_gap_config_t
 *  \brief Structure storing UE measurement configuration request message data
 *  \param bitmask                      bitmask
 *  \param eutran_gap_config            Specifies the measurement gap configuration
 *                                      for EUTRAN frequencies
 *  \param utran_gap_config             Specifies the measurement gap configuration
 *                                      for UTRAN frequencies.
 *                                      related measurements
 *  \param geran_gap_config             Specifies the measurement gap configuration
 *                                      for GERAN frequencies
 *  \param cdma2000_gap_config          Specifies the measurement gap configuration
 *                                      for CDMA200 frequencies.
 */
typedef struct _son_meas_gap_config_t
{
   son_u32            bitmask; /*^ BITMASK ^*/
   son_gap_config_t   eutran_gap_config;
   /*^ O, SON_ANR_EUTRAN_GAP_CONFIG_PRESENT,N, 0, 0 ^*/ 
   son_gap_config_t   utran_gap_config; 
   /*^ O, SON_ANR_UTRAN_GAP_CONFIG_PRESENT,N, 0, 0 ^*/ 
   son_gap_config_t   geran_gap_config; 
   /*^ O, SON_ANR_GERAN_GAP_CONFIG_PRESENT,N, 0, 0 ^*/ 
   son_gap_config_t   cdma2000_gap_config; 
   /*^ O, SON_ANR_CDMA2000_GAP_CONFIG_PRESENT,N, 0, 0 ^*/ 
}son_meas_gap_config_t;

#define SON_ANR_MEAS_GAP_CONFIG_PRESENT     0x01

/*! \  son_oam_anr_meas_config_req_t
 *  \brief Structure storing UE measurement configuration request message data
 *  \param bitmask                      bitmask
 *  \param src_cgi                      CGI of cell
 *  \param eutran_config_list_size      Number of E-UTRA frequencies to be configured for ANR
 *                                      related measurements
 *  \param eutran_config_list           List of EUTRAN frequency specific
 *                                      parameters for ANR related UE
 *                                      measurements
 *  \param other_rat_config_list        List of Inter-Rat frequency specific
 *                                      parameters for ANR related UE
 *                                      measurements
 *  \param meas_gap_config              Specifies the measurement gap configuration and 
 *                                      controls setup/ release of measurement gaps.
 */
typedef struct _son_oam_anr_meas_config_req_t
{
    son_u32                             bitmask; /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t      src_cgi;                                    
    /*^ M, 0, N, 0, 0 ^*/
    son_u8                              eutran_config_list_size;                    
    /* SPR-12875 Fix Starts */
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_MAX_CONFIG_OBJECT ^*/
/*SPR_17664_end*/
    /* SPR-12875 Fix Ends */
    son_meas_config_eutran_t            eutran_config_list[SON_MAX_CONFIG_OBJECT];  
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    son_anr_meas_config_inter_rat_t     other_rat_config_list;                          
    /*^ M, 0, N, 0, 0 ^*/
    son_meas_gap_config_t               meas_gap_config;
    /*^ O, SON_ANR_MEAS_GAP_CONFIG_PRESENT, N, 0, 0 ^*/
}son_oam_anr_meas_config_req_t;
/*^ API, SON_OAM_ANR_MEAS_CONFIG_REQ ^*/

/*! \  son_oam_anr_meas_config_res_t
 *  \brief Structure storing UE measurement configuration response message data
 *  \param src_cgi          CGI of cell
 *  \param result           Success/failure
 *  \param error_code       Cause (valid in case of FAILURE)
 */
typedef struct _son_oam_anr_meas_config_res_t
{
    son_intra_rat_global_cell_id_t      src_cgi;                                
    /*^ M, 0, N, 0, 0 ^*/
    son_return_et                       result;                                 
    /*^ M, 0, H, 0, 1 ^*/
    son_error_et                        error_code;                             
    /*^ M, 0, N, 0, 0 ^*/
}son_oam_anr_meas_config_res_t;

/*^ API, SON_OAM_ANR_MEAS_CONFIG_RESP ^*/

/*
 * SON_OAM_ANR_MODIFY_UE_COUNT_REQ
 */

/*! \  son_oam_anr_modify_ue_count_req_t
 *  \brief   Request to Modify the number of UEs to configure for UE 
 *      Measurement Reporting
 *  \param src_cgi      CGI of Cell
 *  \param ue_cnt       Number of UEs to configure for UE Measurement Reporting //SPR 18268
 */
typedef struct _son_oam_anr_modify_ue_count_req_t
{
    son_intra_rat_global_cell_id_t    src_cgi;    
    /*^ M, 0, N, 0, 0 ^*/
    son_u16                            ue_cnt;     
    /*^ M, 0, B, 1, 10 ^*/
}son_oam_anr_modify_ue_count_req_t;
/*^ API, SON_OAM_ANR_MODIFY_UE_COUNT_REQ  ^*/

/*
 * SON_OAM_ANR_MODIFY_UE_COUNT_RESP
 */

/*! \  son_oam_anr_modify_ue_count_resp_t
 *  \brief   Response to Modify the number of UEs to configure for UE
 *      Measurement Reporting Request
 *  \param   src_cgi    CGI of Cell
 *  \param   result     Success/failure
 *  \param   error_code Cause ( valid in case of FAILURE )
 */
typedef struct _son_oam_anr_modify_ue_count_resp_t
{
    son_intra_rat_global_cell_id_t  src_cgi;        
    /*^ M, 0, N, 0, 0 ^*/
    son_return_et                   result;         
    /*^ M, 0, H, 0, 1 ^*/
    son_error_et                    error_code;     
    /*^ M, 0, N, 0, 0 ^*/
}son_oam_anr_modify_ue_count_resp_t;    

/*^ API, SON_OAM_ANR_MODIFY_UE_COUNT_RESP ^*/

/*
 * SON_OAM_ANR_MODIFY_MEAS_REPORTING_INTERVAL_REQ
 */

/*! \  son_oam_anr_modify_meas_reporting_interval_req_t
 *  \brief   Request to modify the measurement Reporting Interval
 *  \param  src_cgi              CGI of Cell
 *  \param reporting_interval   Measurement Reporting Interval in minutes
 */
typedef struct _son_oam_anr_modify_meas_reporting_interval_req_t
{
    son_intra_rat_global_cell_id_t  src_cgi;    
    /*^ M, 0, N, 0, 0 ^*/
    son_u16                         reporting_interval; 
    /*^ M, 0, B, 60, 1440 ^*/
}son_oam_anr_modify_meas_reporting_interval_req_t;
/*^ API, SON_OAM_ANR_MODIFY_MEAS_REPORTING_INTERVAL_REQ  ^*/

/*
 * SON_OAM_ANR_MODIFY_MEAS_REPORTING_INTERVAL_RESP
 */

/*! \  son_oam_anr_modify_meas_reporting_interval_resp_t
 *  \brief   Response to Modify measurement Reporting Interval
 *  \param   src_cgi    CGI of Cell
 *  \param   result     Success/failure
 *  \param   error_code Cause ( valid in case of FAILURE )
 */
typedef struct _son_oam_anr_modify_meas_reporting_interval_resp_t
{
    son_intra_rat_global_cell_id_t  src_cgi;        /*^ M, 0, N, 0, 0 ^*/
    son_return_et                   result;         /*^ M, 0, H, 0, 1 ^*/
    son_error_et                    error_code;     /*^ M, 0, N, 0, 0 ^*/
}son_oam_anr_modify_meas_reporting_interval_resp_t;    

/*^ API, SON_OAM_ANR_MODIFY_MEAS_REPORTING_INTERVAL_RESP ^*/

/* REL1.1 CHANGES : STARTS */

/*
 *  SON_OAM_PCI_CONFLICT_ENABLE_DISABLE_REQ
 */

/*! \   son_oam_pci_conflict_enable_disable_req_t
 *  \brief    Request from OAM to enable/disable PCI optimization feature
 *  \param    pci_mode  - ENABLE/DISABLE
 */
typedef struct _son_oam_pci_conflict_enable_disable_req_t
{
    son_pci_mode_et             pci_mode; /*^ M, 0, H, 0, 1 ^*/
}son_oam_pci_conflict_enable_disable_req_t; 
/*^ API, SON_OAM_PCI_CONFLICT_ENABLE_DISABLE_REQ ^*/

/*
 *  SON_OAM_PCI_CONFLICT_ENABLE_DISABLE_RESP
 */

/*! \   son_oam_pci_conflict_enable_disable_resp_t 
 *  \brief    Response to OAM against PCI enable/disable request
 *  \param    result      - SUCCESS/FAILURE
 *  \param    error_code  - Valid in case of failure only
 */
typedef struct _son_oam_pci_conflict_enable_disable_resp_t
{
    son_return_et               result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et                error_code; /*^ M, 0, N, 0, 0 ^*/
}son_oam_pci_conflict_enable_disable_resp_t; 
/*^ API, SON_OAM_PCI_CONFLICT_ENABLE_DISABLE_RESP ^*/

/*
 *  SON_OAM_PCI_SELECTION_REQ
 */

/*! \   son_oam_pci_selection_req_t
 *  \brief    Request from OAM to select one unsed PCI from the list provided 
 *            by OAM
 *  \param    earfcn         - EARFCN at which unused PCI is to be selected
 *  \param    pci_list_size  - PCI list size specifying the no of PCIs in the 
 *            request message
 *  \param    pci_list       - List of PCIs
 *  \param    meas_bw_list_size  - Measurement BW list size specifying the no of meas bw in the 
 *            request message
 *  \param    meas_bw_list       - List of measurement bandwidths
 */
typedef struct _son_oam_pci_selection_req_t
{
    son_u32                 earfcn;                /*^ M, 0, N, 0, 0 ^*/
    son_u16                 pci_list_size;         /*^ M, 0, N, 0, 0 ^*/
    son_u16                 pci_list[SON_MAX_PCI]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    son_u8                  meas_bw_list_size;     /*^ M, 0, N, 0, 0 ^*/
    son_u8                  meas_bw_list[SON_MAX_MEAS_BW]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/ 
}son_oam_pci_selection_req_t; 
/*^ API, SON_OAM_PCI_SELECTION_REQ ^*/

/*
 *  SON_OAM_PCI_SELECTION_RESP
 */

/*! \   son_oam_pci_selection_resp_t
 *  \brief    Response to OAM after selecting one unsed PCI from the list 
 *            provided by OAM
 *  \param    selected_pci   - Selected PCI by SON(Valid in case of Success),Incase of failure its value will be 504
 *  \param    result         - SUCCESS/FAILURE
 *  \param    error_code     - Valid in case of failure only
 */
typedef struct _son_oam_pci_selection_resp_t
{
    son_u16                     selected_pci;    /*^ M, 0, H, 0, 504 ^*/ 
    son_return_et               result;          /*^ M, 0, H, 0, 1 ^*/
    son_error_et                error_code;      /*^ M, 0, N, 0, 0 ^*/
}son_oam_pci_selection_resp_t; 
/*^ API, SON_OAM_PCI_SELECTION_RESP ^*/ 

/*
 *  SON_OAM_PCI_RESELECTION_REQ
 */

/*! \   son_oam_pci_reselection_req_t
 *  \brief    Request from OAM to reselect one unsed PCI from the list provided 
 *            by OAM
 *  \param    src_cgi        - Source Cell ID  (SPR-12423 - Start - End)
 *  \param    earfcn         - earfcn
 *  \param    level          - level for searching
 *  \param    pci_list_size  - PCI list size specifying the no of PCIs in the 
 *            request message
 *  \param    pci_list       - List of PCIs
 */
typedef struct _son_oam_pci_reselection_req_t
{
    son_intra_rat_global_cell_id_t  src_cgi;       /*^ M, 0, N, 0, 0 ^*/
    son_u32                 earfcn;                /*^ M, 0, N, 0, 0 ^*/
    son_nbr_level_et level;                /*^ M, 0, H, 0, 1 ^*/
    son_u16                 pci_list_size;         /*^ M, 0, N, 0, 0 ^*/
    son_u16                 pci_list[SON_MAX_PCI]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/ 
}son_oam_pci_reselection_req_t; 
/*^ API, SON_OAM_PCI_RESELECTION_REQ ^*/

/*
 *  SON_OAM_PCI_RESELECTION_RESP
 */

/*! \   son_oam_pci_reselection_resp_t
 *  \brief    Response to OAM after reselecting one unsed PCI from the list 
 *            provided by OAM
 *  \param    src_cgi        - Source Cell ID  (SPR-12423 - Start - End)
 *  \param    reselected_pci   - Reselected PCI by SON(Valid in case of Success),Incase of failure its value will be 504
 *  \param    result         - SUCCESS/FAILURE
 *  \param    error_code     - Valid in case of failure only
 */
typedef struct _son_oam_pci_reselection_resp_t
{
    son_intra_rat_global_cell_id_t src_cgi;        /*^ M, 0, N, 0, 0 ^*/
    son_u16                     reselected_pci;    /*^ M, 0, H, 0, 504 ^*/ 
    son_return_et               result;            /*^ M, 0, H, 0, 1 ^*/
    son_error_et                error_code;        /*^ M, 0, N, 0, 0 ^*/
}son_oam_pci_reselection_resp_t; 
/*^ API, SON_OAM_PCI_RESELECTION_RESP ^*/ 

/*
 *  SON_OAM_PCI_CONFLICT_CONFUSION_IND
 */

/*! \   son_oam_pci_conflict_confusion_ind_t
 *  \brief    Indication to OAM after detecting PCI conflict by SON
 *  \param    src_cgi       - Source cell id (SPR-12596 - Start - End)
 *  \param    cgi_1         - Source cell id/Neighbor cell id
 *  \param    cgi_2         - Neighbor cell id
 *  \param    pci           - Conflicting PCI detected by SON
 *  \param    cause         - Conflict/Confusion
 */
typedef struct _son_oam_pci_conflict_confusion_ind_t
{
    son_global_cell_id_t        src_cgi;      /*^ M, 0, N, 0, 0 ^*/
    son_global_cell_id_t        cgi_1;        /*^ M, 0, N, 0, 0 ^*/
    son_global_cell_id_t        cgi_2;        /*^ M, 0, N, 0, 0 ^*/
    son_u16                     pci;          /*^ M, 0, H, 0, 503 ^*/
    son_pci_cause_et            cause;        /*^ M, 0, H, 0, 3 ^*/
}son_oam_pci_conflict_confusion_ind_t; 
/*^ API, SON_OAM_PCI_CONFLICT_CONFUSION_IND ^*/ 

/*
 *  SON_OAM_CARRIER_FREQ_SELECTION_REQ
 */

/*! \   son_oam_carrier_freq_selection_req_t
 *  \brief    Request from OAM to select least interference EARFCN from the list provided by OAM.
 *  \param    meas_bandwidth     - Measurement Bandwidth
 *  \param    num_earfcn         - Num of EARFCNs
 *  \param    earfcn             - List of EARFCNs
 */
typedef struct _son_oam_carrier_freq_selection_req_t
{
    son_u8              meas_bandwidth;    /*^ M, 0, B, 6, 100 ^*/
/*SPR_17664_start*/
    son_u8              num_earfcn;        /*^ M, 0, H, 0, SON_MAX_NO_EARFCN ^*/
/*SPR_17664_end*/
    son_u32             earfcn[SON_MAX_NO_EARFCN]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_carrier_freq_selection_req_t; 
/*^ API, SON_OAM_CARRIER_FREQ_SELECTION_REQ ^*/ 

/*
 *  SON_OAM_CARRIER_FREQ_SELECTION_RESP
 */

/*! \   son_oam_carrier_freq_selection_resp_t
 *  \brief    Response to OAM after selecting least interference frequency from the list 
 *            provided by OAM
 *  \param    selected_carrier_freq - Selected carrier frequency
 *  \param    result                - SUCCESS/FAILURE
 *  \param    error_code            - Valid in case of failure only
 */
typedef struct _son_oam_carrier_freq_selection_resp_t
{
    son_u16                 selected_carrier_freq;   /*^ M, 0, N, 0, 0 ^*/
    son_return_et           result;                  /*^ M, 0, H, 0, 1 ^*/
    son_error_et            error_code;              /*^ M, 0, N, 0, 0 ^*/
}son_oam_carrier_freq_selection_resp_t; 
/*^ API, SON_OAM_CARRIER_FREQ_SELECTION_RESP ^*/

/*
 *  SON_OAM_SET_NW_SCAN_CONFIG_CMD
 */

/*! \   son_oam_set_nw_scan_config_cmd_t
 *  \brief  Structure containg NMM Scan Configuration
 *  \param  force_scan_on_off   Forced Scan on/off
 *  \param  scan_time_interval  Scan interval
 */
typedef struct _son_oam_set_nw_scan_config_cmd_t
{
    son_u8      force_scan_on_off;        /*^ M, 0, H, 0, 1 ^*/
    son_u32     scan_time_interval;   /*^ M, 0, N, 0, 0 ^*/
}son_oam_set_nw_scan_config_cmd_t;
/*^ API, SON_OAM_SET_NW_SCAN_CONFIG_CMD ^*/

/* REL1.1 CHANGES : ENDS */

/* REL1.2 ES Changes : STARTS */

/*! \ son_global_enb_id_t
 *  \brief    Structure to define the global eNB Id 
 *  \param    enb_type - eNB Type i.e. HOME or MACRO
 *  \param    plmn_id  - Public Land Mobile Network Identifier
 *  \param    enb_id   - 28 bits Home eNB id or 20 bits Macro eNB id.
 *                       If eNB type is SON_HOME_ENB, this field is
 *                       equal to the 28 leftmost bits of the Cell
 *                       Identity contained in the E-UTRAN CGI of
 *                       the cell served by the eNB.If eNB type is
 *                       SON_MACRO_ENB, this field is equal to the 20
 *                       leftmost bits of the Cell Identity contained
 *                       in the E-UTRAN CGI of each cell served by the eNB.
 */
 typedef struct _son_global_enb_id_t
 {
    son_enb_type_et        enb_type;
    /*^M,0,B,1,2^*/

    son_cell_plmn_info_t   plmn_id;
    /*^M,0,N,0,0^*/

    son_u8                 enb_id[SON_ENB_ID_OCTET_SIZE];
    /*^M,0,OCTET_STRING,FIXED ^*/
 }son_global_enb_id_t;

/*
 * SON_OAM_ES_CELL_SWITCH_ON_IND
 */

/*! \ son_cell_on_off_status_t
 *  \brief Structure for son cell on off status
 *  \param cgi    - CGI of cell
 *  \param result     - Outcome(Success/Failure)
 *  \param error_code - indicates error type in case of Failure
 */
typedef struct _son_cell_on_off_status_t
{
    son_intra_rat_global_cell_id_t        cgi; /*^ M, 0, N, 0, 0 ^*/
    son_switch_on_off_cause_et            switch_on_off_cause; /*^ M, 0, N, 0, 0 ^*/
    son_return_et                         result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et                          error_code; /*^ M, 0, N, 0, 0 ^*/
}son_cell_on_off_status_t;

/*! \ son_oam_es_cell_switch_on_off_ind_t
 *  \brief Indication for Cell Switch ON/OFF.
 *  \param cell_on_status_list_size - Number of elememts in the cell 
 *                                            on status list
 *  \param cell_on_status_list      - List containing the status of each 
 *                                        cell in switch on/off indication
 */
typedef struct _son_oam_es_cell_switch_on_off_ind_t
{
/*SPR_17664_start*/
    son_u16                     cell_on_status_list_size; /*^ M, 0, B, 1, SON_MAX_CELL_SUPPORTED_AT_ENB ^*/
/*SPR_17664_end*/
    son_cell_on_off_status_t    cell_on_status_list[SON_MAX_CELL_SUPPORTED_AT_ENB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_es_cell_switch_on_off_ind_t;    /*^ API, SON_OAM_ES_CELL_SWITCH_ON_OFF_IND^*/

/* REL1.2 ES Changes : END */

/* REL1.2 ANR CHANGES : STARTS */

/*
 * SON_OAM_NR_DELETE_FROM_REMOVE_LIST_REQ
 */

/*! \ son_oam_nr_delete_from_remove_list_req_t
 *  \brief This is the request structure to delete NR entry in Remove List
 *  \param src_cgi              Source cell id
 *  \param trgt_cgi_list_size   No of neighbors to be deleted
 *  \param trgt_cgi_list        To be deleted list of neighbours. Max list size 256
 */
typedef struct _son_oam_nr_delete_from_remove_list_req_t
{
    son_intra_rat_global_cell_id_t src_cgi;    /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u16                        trgt_cgi_list_size;      /*^ M, 0, B, 1, SON_MAX_NO_NEIGHBOUR_CELLS ^*/
/*SPR_17664_end*/
    son_global_cell_id_t           trgt_cgi_list[SON_MAX_NO_NEIGHBOUR_CELLS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} son_oam_nr_delete_from_remove_list_req_t;    /*^ API, SON_OAM_NR_DELETE_FROM_REMOVE_LIST_REQ ^*/

/*
 * SON_OAM_NR_DELETE_FROM_REMOVE_LIST_RESP
 */

/*! \ son_oam_nr_delete_from_remove_list_resp_t
 *  \brief This is the response structure to delete NR entry in Remove List
 *  \param src_cgi                     Source cell id
 *  \param result                      Result of NR delete request
 *  \param error_code                  Error code
 *  \param trgt_cell_status_list_size  cell status list size
 *  \param trgt_cell_status_list       list of ECGI and error code of NRs which
 *                                     could not be deleted due to some internal errors.
 *                                     Max list size is 256. (Optional)
 */
typedef struct _son_oam_nr_delete_from_remove_list_resp_t
{
    son_intra_rat_global_cell_id_t  src_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_return_et                   result;     /*^ M, 0, H, 0, 2 ^*/
    son_error_et                    error_code; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
	son_u16                          trgt_cell_status_list_size;    /*^ M, 0, H, 0, SON_MAX_NO_NEIGHBOUR_CELLS ^*/
/*SPR_17664_end*/
    son_nbr_cell_status_t              trgt_cell_status_list[SON_MAX_NO_NEIGHBOUR_CELLS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} son_oam_nr_delete_from_remove_list_resp_t;    /*^ API, SON_OAM_NR_DELETE_FROM_REMOVE_LIST_RESP ^*/

/*
 *  SON_OAM_ANR_MODIFY_REMOVAL_ATTRIBUTES_REQ
 */

/*! \  son_oam_anr_modify_removal_attributes_req_t
 *  \brief    Request to modify removal attributes
 *  \param    src_cgi           - CGI of Cell
 *  \param    removal_criteria  - removal criteria for neighbors 
 */
typedef struct _son_oam_anr_modify_removal_attributes_req_t
{
    son_intra_rat_global_cell_id_t  src_cgi;            /*^ M, 0, N, 0, 0 ^*/
    son_neighbor_removal_info_t     removal_criteria;   /*^ M, 0, N, 0, 0 ^*/
}son_oam_anr_modify_removal_attributes_req_t;
/*^ API, SON_OAM_ANR_MODIFY_REMOVAL_ATTRIBUTES_REQ ^*/

/*
 *  SON_OAM_ANR_MODIFY_REMOVAL_ATTRIBUTES_RESP
 */

/*! \  son_oam_anr_modify_removal_attributes_resp_t
 *  \brief    Response to modify removal attributes
 *  \param    src_cgi     - CGI of Cell
 *  \param    result      - SUCCESS/FAILURE
 *  \param    error_code  - Valid in case of failure only
 */
typedef struct _son_oam_anr_modify_removal_attributes_resp_t
{
    son_intra_rat_global_cell_id_t  src_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_return_et                   result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et                    error_code; /*^ M, 0, N, 0, 0 ^*/
}son_oam_anr_modify_removal_attributes_resp_t;
/*^ API, SON_OAM_ANR_MODIFY_REMOVAL_ATTRIBUTES_RESP ^*/

/*
 *  SON_OAM_ANR_MODIFY_PCI_CONFUSION_CFG_REQ 
 */

/*! \  son_oam_anr_modify_pci_confusion_cfg_req_t
 *  \brief    Request to modify PCI confusion configuration
 *  \param    src_cgi     - CGI of Cell
 *  \param    pci_confusion_cfg  - configuration for PCI confusion/conflict
 */
typedef struct _son_oam_anr_modify_pci_confusion_cfg_req_t
{
    son_intra_rat_global_cell_id_t      src_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_neighbor_pci_confusion_info_t   pci_confusion_cfg;  
    /*^ M, 0, N, 0, 0 ^*/
}son_oam_anr_modify_pci_confusion_cfg_req_t;
/*^ API, SON_OAM_ANR_MODIFY_PCI_CONFUSION_CFG_REQ ^*/

/*
 *  SON_OAM_ANR_MODIFY_PCI_CONFUSION_CFG_RESP
 */

/*! \  son_oam_anr_modify_pci_confusion_cfg_resp_t
 *  \brief    Response to modify pci confusion configuration req
 *  \param    src_cgi     - CGI of Cell
 *  \param    result      - SUCCESS/FAILURE
 *  \param    error_code  - Valid in case of failure only
 */
typedef struct _son_oam_anr_modify_pci_confusion_cfg_resp_t
{
    son_intra_rat_global_cell_id_t      src_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_return_et                       result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et                        error_code; /*^ M, 0, N, 0, 0 ^*/
}son_oam_anr_modify_pci_confusion_cfg_resp_t;
/*^ API, SON_OAM_ANR_MODIFY_PCI_CONFUSION_CFG_RESP ^*/

/* SPR 20653 Fix Start */
/*
 * SON_OAM_ANR_MODIFY_ATTRIBUTE_REQ
 */

/*! \  son_oam_anr_modify_attribute_req_t
 *  \brief   Request to Modify ANR attributes 
 *  \param src_cgi        - CGI of Cell
 *  \param anr_attribute  - ANR attributes to be modified
 */
typedef struct _son_oam_anr_modify_attribute_req_t
{
    son_intra_rat_global_cell_id_t      src_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_oam_anr_modify_attribute_info_t anr_attribute; /*^ M, 0, N, 0, 0 ^*/
}son_oam_anr_modify_attribute_req_t;
/*^ API, SON_OAM_ANR_MODIFY_ATTRIBUTE_REQ  ^*/

/*
 * SON_OAM_ANR_MODIFY_ATTRIBUTE_RESP
 */

/*! \  son_oam_anr_modify_attribute_resp_t
 *  \brief   Response to Modify the ANR attributes
 *  \param   src_cgi        - CGI of Cell
 *  \param   result     Success/failure
 *  \param   error_code Cause ( valid in case of FAILURE )
 */
typedef struct _son_oam_anr_modify_attribute_resp_t
{
    son_intra_rat_global_cell_id_t      src_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_return_et                   result;         
    /*^ M, 0, H, 0, 1 ^*/
    son_error_et                    error_code;     
    /*^ M, 0, N, 0, 0 ^*/
}son_oam_anr_modify_attribute_resp_t;    

/*^ API, SON_OAM_ANR_MODIFY_ATTRIBUTE_RESP ^*/
/* SPR 20653 Fix End */

/*
 *  SON_OAM_ENABLE_DISABLE_X2_REQ
 */

/*! \  son_oam_enable_disable_x2_req_t
 *  \brief    Request to enable/disable X2
 *  \param    x2_enabled        - x2 enabled true/false 
 */
typedef struct _son_oam_enable_disable_x2_req_t
{
    son_bool_et                 x2_enabled; /*^ M, 0, H, 0, 1 ^*/
}son_oam_enable_disable_x2_req_t;
/*^ API, SON_OAM_ENABLE_DISABLE_X2_REQ ^*/

/*
 *  SON_OAM_ENABLE_DISABLE_X2_RESP
 */

/*! \  son_oam_enable_disable_x2_resp_t
 *  \brief    Response to enable/disable X2
 *  \param    result      - SUCCESS/FAILURE
 *  \param    error_code  - Valid in case of failure only
 */
typedef struct _son_oam_enable_disable_x2_resp_t
{
    son_return_et               result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et                error_code; /*^ M, 0, N, 0, 0 ^*/
}son_oam_enable_disable_x2_resp_t;
/*^ API, SON_OAM_ENABLE_DISABLE_X2_RESP ^*/

/*! \  son_gu_group_id_t
 *  \brief   Gu group id
 *  \param plmn_id       - PLMN Id
 *  \param mme_group_id  - MME group id
 */
typedef struct _son_gu_group_id_t
{
    son_cell_plmn_info_t   plmn_id; /*^ M, 0, N, 0, 0 ^*/
    son_u8                 mme_group_id[SON_MAX_MME_GRP_BYTES]; /*^ M,0,OCTET_STRING,FIXED ^*/
}son_gu_group_id_t;

/*! \  son_gu_group_id_list_t
 *  \brief   Gu group id List
 *  \param num_gu_group_id       - Number of group ids
 *  \param gu_id_list             -List of group ids
 */
typedef struct _son_gu_group_id_list_t
{
/*SPR_17664_start*/
    son_u8 num_gu_group_id; /*^ M,0,B,1,SON_MAX_ENB_POOLS ^*/
/*SPR_17664_end*/
    son_gu_group_id_t gu_id_list[SON_MAX_ENB_POOLS];   /*^ M,0,OCTET_STRING,VARIABLE ^*/
}son_gu_group_id_list_t;

/*! \son_served_cell_info_t
 *
 *  \brief  served cell information of neighbor eNB
 * \ param    bitmask          - Bitmask
 *  \param    pci              - PCI
 *  \param cell_id             - Serving Cell ID
 *  \param tac               - tac
 */
typedef struct _son_served_cell_info_t
{
    son_u32                             bitmask; /*^ BITMASK ^*/
    son_u16                             pci;  /*^ M, 0, H, 0, 503 ^*/
    son_u8                              cell_id; /*^ M, 0, H, 0, 7 ^*/
    son_u8                              tac[SON_TAC_OCTET_SIZE];
    /*^ M,0, OCTET_STRING, FIXED ^*/
}son_served_cell_info_t;

/*! \  son_nbour_cell_info_t
 *  \brief  Neighbor cell information
 * \ param  num_neighbour_cell  - Number of neighbor cells
 *  \param  neighbour_info_arr   -List of  Neighbor cell info  
 */
typedef struct _son_nbour_cell_info_t
{
/*SPR_17664_start*/
   son_u16                         num_neighbour_cell; /*^ M, 0, B, 1, SON_MAX_NO_INTRA_NBR_CELLS ^*/
/*SPR_17664_end*/
   son_intra_rat_neighbor_info_t   neighbour_info_arr[SON_MAX_NO_INTRA_NBR_CELLS];
   /*^ M, 0, OCTET_STRING,VARIABLE ^*/
}son_nbour_cell_info_t;

/*! \  son_served_cell_t
 *  \brief  served cell data of neighbor eNB
 *  \param  served_cell_info  - Served cell info
 *  \param  nbour_cell_info   - Neighbor cell info of the served cell 
 */
typedef struct _son_served_cell_t
{
  son_served_cell_info_t served_cell_info;
   /*^ M, 0, N, 0, 0 ^*/
  son_nbour_cell_info_t  nbour_cell_info;
  /*^ M, 0, N, 0, 0 ^*/
}son_served_cell_t;

/*! \  son_served_cell_info_arr_t
 *  \brief  served cell information of neighbor eNB
 *  \param num_served_cell       - Number of serverd cell info
 *  \param cell_info             -List of served cell info
 */
typedef struct _son_served_cell_info_arr_t
{
/*SPR_17664_start*/
  son_u16 num_served_cell; /*^ M,0,B,1,SON_MAX_SERVED_CELLS_AT_NBR_ENB ^*/
/*SPR_17664_end*/
  son_served_cell_t cell_info[SON_MAX_SERVED_CELLS_AT_NBR_ENB];
  /*^ M,0,OCTET_STRING,VARIABLE ^*/
}son_served_cell_info_arr_t;

/*
 *  SON_OAM_X2_LINK_UP_IND
 */

/*! \  son_oam_x2_link_up_ind_t
 *  \brief    X2 link up indication
 * \ param    bitmask          - Bitmask
 *  \param    nbr_enb_id        - neighbor eNB id
 * \param    served_cell_info  - Peer eNB served cell info
 *  \param    gu_group_id      - group id info (For Future use)
 */

#define SON_SERVED_CELL_INFO_PRESENT   0x01
#define SON_GU_GROUP_ID_LIST_PRESENT   0x02 
typedef struct _son_oam_x2_link_up_ind_t
{
    son_u32 bitmask; /*^BITMASK^*/
    son_global_enb_id_t         nbr_enb_id; /*^ M, 0, N, 0, 0 ^*/
    son_served_cell_info_arr_t  served_cell_info; /*^ O,SON_SERVED_CELL_INFO_PRESENT,N,0,0 ^*/
    son_gu_group_id_list_t gu_group_id; /*^ O,SON_GU_GROUP_ID_LIST_PRESENT,N,0,0 ^*/
}son_oam_x2_link_up_ind_t;
/*^ API, SON_OAM_X2_LINK_UP_IND ^*/

/*
 *  SON_OAM_X2_LINK_DOWN_IND
 */

/*! \  son_oam_x2_link_down_ind_t
 *  \brief    X2 link down indication
 *  \param    nbr_enb_id        - neighbor eNB id
 */
typedef struct _son_oam_x2_link_down_ind_t
{
    son_global_enb_id_t         nbr_enb_id; /*^ M, 0, N, 0, 0 ^*/
}son_oam_x2_link_down_ind_t;
/*^ API, SON_OAM_X2_LINK_DOWN_IND ^*/

/*
 *  SON_OAM_ANR_ENABLE_DISABLE_LIMITED_MODE_REQ
 */

/*! \  son_oam_anr_enable_disable_limited_mode_req_t
 *  \brief    Request to enable/disable limited mode
 *  \param    src_cgi           - CGI of Cell
 *  \param    limited_mode      - true/false
 */
typedef struct _son_oam_anr_enable_disable_limited_mode_req_t
{
    son_intra_rat_global_cell_id_t  src_cgi;        /*^ M, 0, N, 0, 0 ^*/
    son_bool_et                     limited_mode;   /*^ M, 0, H, 0, 1 ^*/
}son_oam_anr_enable_disable_limited_mode_req_t;
/*^ API, SON_OAM_ANR_ENABLE_DISABLE_LIMITED_MODE_REQ ^*/

/*
 *  SON_OAM_ANR_ENABLE_DISABLE_LIMITED_MODE_RESP
 */

/*! \  son_oam_anr_enable_disable_limited_mode_resp_t
 *  \brief    Response to enable/disable limted mode
 *  \param    src_cgi           - CGI of Cell
 *  \param    result      - SUCCESS/FAILURE
 *  \param    error_code  - Valid in case of failure only
 */
typedef struct _son_oam_anr_enable_disable_limited_mode_resp_t
{
    son_intra_rat_global_cell_id_t  src_cgi;    /*^ M, 0, N, 0, 0 ^*/
    son_return_et                   result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et                    error_code; /*^ M, 0, N, 0, 0 ^*/
}son_oam_anr_enable_disable_limited_mode_resp_t;
/*^ API, SON_OAM_ANR_ENABLE_DISABLE_LIMITED_MODE_RESP ^*/

/*! \ son_limited_mode_eutran_cell_info_t
 *  \brief    List of PCIs detected on a particular EARFCN
 *  \param    earfcn            - EUTRAN Frequency
 *  \param    pci_list_size     - Size of PCI List
 *  \param    pci_list          - PCI List
 */
typedef struct _son_limited_mode_eutran_cell_info_t
{
    son_u32 earfcn; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u16 pci_list_size; /*^ M, 0, H, 1, SON_MAX_PCI ^*/
/*SPR_17664_end*/
    son_u16 pci_list[SON_MAX_PCI]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_limited_mode_eutran_cell_info_t;

/*
 *  SON_OAM_ANR_LIMITED_MODE_PCI_INFO
 */

/*! \  son_oam_anr_limited_mode_pci_info_t
 *  \brief    Notification to OAM for the newly detected PCIs in limited mode
 *  \param    src_cgi                   - CGI of Cell
 *  \param    pci_earfcn_list_size      - Size of PCI List
 *  \param    pci_earfcn_list           - PCI List
 */
typedef struct _son_oam_anr_limited_mode_pci_info_t
{
    son_intra_rat_global_cell_id_t          src_cgi;    /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u8 pci_earfcn_list_size; /*^ M, 0, H, 0, SON_MAX_NO_INTRA_RAT_NEIGHBOUR_CELLS ^*/
/*SPR_17664_end*/
    son_limited_mode_eutran_cell_info_t     
            pci_earfcn_list[SON_MAX_NO_INTRA_RAT_NEIGHBOUR_CELLS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_anr_limited_mode_pci_info_t; /*^ API, SON_OAM_ANR_LIMITED_MODE_PCI_INFO ^*/

#define IPV4_ADD_PRESENT        0x01
#define IPV6_ADD_PRESENT        0x02

/*! \ son_enb_tnl_address_t
 *  \brief    Structure to define the Transport Layer Address 
 *  \param    bitmask - To indicate if the optional fields are present or not 
 *                      Only one of this bit should be set.Bit 1: ipv4_add and
 *                      Bit 2: ipv6_add
 *  \param    ipv4_add - 32bits IPV4 IP Address.
 *  \param    ipv6_add - 128bits IPV6 IP Address.
 */
 typedef struct _son_enb_tnl_address_t
{
    son_u32 bitmask; /*^BITMASK^*/
   son_u8    ipv4_add[SON_IPV4_SIZE];
   /*^O,IPV4_ADD_PRESENT,OCTET_STRING,FIXED^*/
   son_u8    ipv6_add[SON_IPV6_SIZE];
   /*^O,IPV6_ADD_PRESENT,OCTET_STRING,FIXED^*/
 }son_enb_tnl_address_t;

/*! \  son_tac_t
 *  \brief It is the data to keep tac value
 *  \param tac                          Tracking Area Code
 */
typedef struct _son_tac_t
{
  son_u8     tac[SON_TAC_OCTET_SIZE];
  /*^ M,0, OCTET_STRING, FIXED ^*/
}son_tac_t;

#define SON_ANR_ENB_TAC_PRESENT              0x01
#define SON_ANR_ENB_TNL_ADD_LIST_PRESENT     0x02
#define SON_ANR_ENB_X2_HO_STATUS_PRESENT     0x04
#define SON_ANR_ENB_CUR_X2_CONN_PRESENT      0x08

/*! \  son_neighbor_enb_info_t
 *  \brief It is the structure for eNB data
 *  \param bitmask                      bitmask
 *  \param enb_id                       neighbor eNB Id
 *  \param tac_list_size                Tracking Area Code List size
 *  \param tac_list                     Tracking Area Code List
 *  \param enb_tnl_address_list_size    TNL address list size
 *  \param enb_tnl_address_list         TNL address list
 *  \param x2_status                    X2 based HO status
 *  \param x2_connection_status         Current X2 connection status
 */

typedef struct _son_neighbor_enb_info_t
{
  son_u32                        bitmask; /*^BITMASK^*/
  son_global_enb_id_t            enb_id;  /*^M,0,N,0,0^*/
  son_u8                         tac_list_size;
/*SPR_17664_start*/
  /*^ O, SON_ANR_ENB_TAC_PRESENT,H,0,SON_MAX_SERVED_CELLS_AT_NBR_ENB ^*/
/*SPR_17664_end*/
  son_tac_t                      tac_list[SON_MAX_SERVED_CELLS_AT_NBR_ENB];
  /*^ O, SON_ANR_ENB_TAC_PRESENT, OCTET_STRING,VARIABLE ^*/
  son_u8                         enb_tnl_address_list_size;
/*SPR_17664_end*/
  /*^O,SON_ANR_ENB_TNL_ADD_LIST_PRESENT,B,1,SON_MAX_NO_ENB_X2_TLAS^*/
/*SPR_17664_end*/
  son_enb_tnl_address_t          enb_tnl_address_list[SON_MAX_NO_ENB_X2_TLAS];
  /*^O,SON_ANR_ENB_TNL_ADD_LIST_PRESENT,OCTET_STRING,VARIABLE^*/                                  
  son_x2_status_et               x2_status;
  /*^O,SON_ANR_ENB_X2_HO_STATUS_PRESENT,B,1,2^*/
  son_x2_connection_status_et    x2_connection_status;
  /*^O,SON_ANR_ENB_CUR_X2_CONN_PRESENT,B,1,10^*/
}son_neighbor_enb_info_t;

/*! \ son_enb_status_t
 *  \brief It is the structure for determining failure cause in
 *         add ,delete,update of information of given eNB Id.
 *  \param enb_id                       neighbor eNB Id
 *  \param error_code                   Failure Cause
 */
typedef struct _son_enb_status_t
{
    son_global_enb_id_t enb_id; /*^ M, 0, N, 0, 0 ^*/
  son_error_et        error_code;
  /*^ M, 0, N, 0, 0 ^*/
}son_enb_status_t;

/*
 *  SON_OAM_NR_ENB_ADD_REQ
 */

/*! \ son_oam_nr_enb_add_req_t
 *  \brief   Neighbor eNB Add Request received from OAM for SON module.
 *  \param   nbr_enb_list_size  - Neighbor eNB  list size
 *  \param   nbr_enb_list -   List of neighbor eNB to be added 
 */
typedef struct _son_oam_nr_enb_add_req_t
{
   son_u8                    nbr_enb_list_size; /*SPR-989-Number of eNB's increased to 32 from 3,modified the pup tool rule*/
/*SPR_17664_start*/
    /*^M, 0, B, 1, SON_MAX_NO_NEIGHBOUR_ENBS^*/
/*SPR_17664_end*/
   son_neighbor_enb_info_t   nbr_enb_list[SON_MAX_NO_NEIGHBOUR_ENBS];
   /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_nr_enb_add_req_t; /*^ API, SON_OAM_NR_ENB_ADD_REQ ^*/

/*
 *  SON_OAM_NR_ENB_ADD_RESP
 */

/*! \ son_oam_nr_enb_add_resp_t
 *  \brief   Neighbor eNB Add Response  from SON to OAM
 *  \param   result - Result of neighbor retrieval request
 *  \param   error_code - error code
 *  \param   nbr_enb_status_list_size - Number of elements in the 
 *                                      nbr_enb_status_list
 *  \param  nbr_enb_status_list - The list of the neighbor eNBs which
 *                     could not be added due to some internal errors
 */
typedef struct _son_oam_nr_enb_add_resp_t
{
  son_return_et     result;  /*^M, 0, H, 0, 2^*/
  son_error_et      error_code; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
  son_u8 nbr_enb_status_list_size; /*^M, 0, H, 0, SON_MAX_NO_NEIGHBOUR_ENBS^*/
/*SPR_17664_end*/
  son_enb_status_t  nbr_enb_status_list[SON_MAX_NO_NEIGHBOUR_ENBS];
  /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_nr_enb_add_resp_t; /*^ API, SON_OAM_NR_ENB_ADD_RESP ^*/

/*
 *  SON_OAM_NR_ENB_UPDATE_REQ
 */

/*! \ son_oam_nr_enb_update_req_t
 *  \brief   Neighbor eNB Update Request received from OAM for SON module.
 *  \param   nbr_enb_list_size  - Neighbor eNB  list size
 *  \param   nbr_enb_list -   List of neighbor eNB to be updated 
 */
typedef struct _son_oam_nr_enb_update_req_t
{
   son_u8                    nbr_enb_list_size; /*SPR-989-Number of eNB's increased to 32 from 3,modified the pup tool rule*/
/*SPR_17664_start*/
    /*^M, 0, B, 1, SON_MAX_NO_NEIGHBOUR_ENBS^*/
/*SPR_17664_end*/
   son_neighbor_enb_info_t   nbr_enb_list[SON_MAX_NO_NEIGHBOUR_ENBS];
   /*^ M, 0,OCTET_STRING, VARIABLE ^*/
}son_oam_nr_enb_update_req_t; /*^ API, SON_OAM_NR_ENB_UPDATE_REQ ^*/

/*
 *  SON_OAM_NR_ENB_UPDATE_RESP
 */

/*! \ son_oam_nr_enb_update_resp_t
 *  \brief   Neighbor eNB Update Response  from SON to OAM
 *  \param   result - Result of neighbor eNB udpation request
 *  \param   error_code - error code
 *  \param   nbr_enb_status_list_size - Number of elements in the 
 *                                      nbr_enb_status_list
 *  \param  nbr_enb_status_list - The list of the neighbor eNBs which
 *                     could not be updated due to some internal errors
 */
typedef struct _son_oam_nr_enb_update_resp_t
{
  son_return_et     result;  /*^M, 0, H, 0, 2^*/
  son_error_et      error_code; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
  son_u8 nbr_enb_status_list_size; /*^M, 0, H, 0, SON_MAX_NO_NEIGHBOUR_ENBS^*/
/*SPR_17664_end*/
  son_enb_status_t  nbr_enb_status_list[SON_MAX_NO_NEIGHBOUR_ENBS];
  /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_nr_enb_update_resp_t; /*^ API, SON_OAM_NR_ENB_UPDATE_RESP ^*/

/*
 *  SON_OAM_NR_ENB_DELETE_REQ
 */

/*! \ son_oam_nr_enb_delete_req_t
 *  \brief   Neighbor eNB Delete Request received from OAM for SON module.
 *  \param   nbr_enb_id_list_size  - Neighbor eNB Id list size
 *  \param   nbr_enb_id_list -   List of neighbor eNB to be Deleted 
 */
typedef struct _son_oam_nr_enb_delete_req_t
{
   son_u8                    nbr_enb_id_list_size; /*SPR-989-Number of eNB's increased to 32 from 3,modified the pup tool rule*/
/*SPR_17664_start*/
    /*^M, 0, B, 1, SON_MAX_NO_NEIGHBOUR_ENBS^*/
/*SPR_17664_end*/
   son_global_enb_id_t       nbr_enb_id_list[SON_MAX_NO_NEIGHBOUR_ENBS];
   /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_nr_enb_delete_req_t; /*^ API, SON_OAM_NR_ENB_DELETE_REQ ^*/

/*
 *  SON_OAM_NR_ENB_DELETE_RESP
 */

/*! \ son_oam_nr_enb_delete_resp_t
 *  \brief   Neighbor eNB Delete Response  from SON to OAM
 *  \param   result     - Result of neighbor eNB delete request
 *  \param   error_code - error code
 *  \param   nbr_enb_status_list_size - Number of elements in the 
 *                                      nbr_enb_status_list
 *  \param  nbr_enb_status_list - The list of the neighbor eNBs which
 *                     could not be deleted due to some internal errors
 */
typedef struct _son_oam_nr_enb_delete_resp_t
{
  son_return_et     result;  /*^M, 0, H, 0, 2^*/
  son_error_et      error_code; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
  son_u8 nbr_enb_status_list_size; /*^M, 0, H, 0, SON_MAX_NO_NEIGHBOUR_ENBS^*/
/*SPR_17664_end*/
  son_enb_status_t  nbr_enb_status_list[SON_MAX_NO_NEIGHBOUR_ENBS];
  /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_nr_enb_delete_resp_t; /*^ API, SON_OAM_NR_ENB_DELETE_RESP ^*/

/*
 *  SON_OAM_NR_ENB_RETRIEVE_REQ
 */

#define SON_RTRV_NBR_ENB_ID_PRESENT      0x01
#define SON_RTRV_X2_HO_STATUS_PRESENT    0x02
#define SON_RTRV_X2_CON_STATUS_PRESENT   0x04

/*! \ son_oam_nr_enb_retrieve_req_t
 *  \brief   Neighbor eNB Retrieve Request received from OAM for SON module.
 *  \param   bitmask - Bitmask
 *  \param   nbr_enb_id        -   neighbor eNB id to be retrieved 
 *  \param   x2_status         -   Indicates whether X2 connection can be 
 *                               used for HO or not with the corresponding eNB
 *  \param   x2_connection_status - Indicates the status of the X2 Connection 
 */
typedef struct _son_oam_nr_enb_retrieve_req_t
{
    son_u32 bitmask; /*^BITMASK^*/
  son_global_enb_id_t          nbr_enb_id;
  /*^O, SON_RTRV_NBR_ENB_ID_PRESENT, N, 0, 0^*/
  son_x2_status_et             x2_status;
  /*^O, SON_RTRV_X2_HO_STATUS_PRESENT, B, 1, 2^*/
  son_x2_connection_status_et  x2_connection_status;
  /*^O, SON_RTRV_X2_CON_STATUS_PRESENT, B, 1, 10^*/
}son_oam_nr_enb_retrieve_req_t; /*^ API, SON_OAM_NR_ENB_RETRIEVE_REQ ^*/

/*
 *  SON_OAM_NR_ENB_RETRIEVE_RESP
 */

/*! \ son_oam_nr_enb_retrieve_resp_t
 *  \brief   Neighbor eNB Retrieve Response  from SON to OAM
 *  \param   result            - Result of neighbor eNB delete request
 *  \param   error_code        - Error code
 *  \param   nbr_enb_list_size - Indicates the size of nbr_enb_list
 *  \param  nbr_enb_list       - List of neighbors eNBs and 
 *                               attribute values of each neighbor.
 */
typedef struct _son_oam_nr_enb_retrieve_resp_t
{
  son_return_et            result;  /*^M, 0, H, 0, 1^*/
    son_error_et error_code; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u8 nbr_enb_list_size; /*^M, 0, H, 0, SON_MAX_NO_NEIGHBOUR_ENBS ^*/ /*SPR-989-Number of eNB's increased to 32 from 3,modified the pup tool rule*/
/*SPR_17664_enb*/
  son_neighbor_enb_info_t  nbr_enb_list[SON_MAX_NO_NEIGHBOUR_ENBS];
  /*^ M, 0,OCTET_STRING, VARIABLE ^*/
}son_oam_nr_enb_retrieve_resp_t; /*^ API, SON_OAM_NR_ENB_RETRIEVE_RESP ^*/

/*! \ son_oam_proc_sup_resp_t
 *  \brief alive_status    - Ok ot Not Ok
 */
typedef struct _son_oam_proc_sup_resp_t
{
    son_module_sup_et alive_status; /*^ M, 0, N, 0, 0 ^*/
}son_oam_proc_sup_resp_t; /*^ API, SON_OAM_PROC_SUP_RESP ^*/

/* REL1.2 ANR CHANGES : ENDS */

/*
 *  SON_OAM_FEATURE_STATE_CHANGE_IND
 */

/*! \ son_oam_feature_state_change_ind_t
 *  \brief   Cell state change indication from SON to OAM
 *  \param   bitmask           -Bitmask
 *  \param   srv_cgi           - CGI of cell
 *  \param   module_id         - SON internal module ID
 *  \param   state             - SON feature state 
 *  \param   cause             - SON feature state change cause 
 */
typedef struct _son_oam_feature_state_change_ind_t
{
    son_u32 bitmask; /*^BITMASK^*/
    son_intra_rat_global_cell_id_t      srv_cgi;  /*^ M, 0, N, 0, 0 ^*/
    son_module_id_et                    module_id; /*^ M, 0, N, 0, 0 ^*/
    son_feature_state_et                state;    /*^ M, 0, N, 0, 0 ^*/
    son_feature_state_change_cause_et   cause;    /*^ M, 0, N, 0, 0 ^*/
}son_oam_feature_state_change_ind_t; /*^ API,  SON_OAM_FEATURE_STATE_CHANGE_IND ^*/

/* REL1.3 MRO CHANGES : STARTS */

/*
 * SON_OAM_MRO_ENABLE
 */

#define SON_MRO_ENABLE_SRV_CGI_PRESENT 0x01

/*! \  son_oam_mro_enable_req_t
 *  \brief MRO Enable Request received from OAM
 *  \param bitmask
 *  \param srv_cgi   CGI of cell on which MRO is to be enabled. In case no
 *                   cellId is present in the request then MRO has to be enabled
 *                   on all the cells of eNodeB
 */
typedef struct _son_oam_mro_enable_req_t
{
    son_u32                         bitmask;         /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t  srv_cgi;
    /*^ O, SON_MRO_ENABLE_SRV_CGI_PRESENT, N, 0, 0 ^*/
}son_oam_mro_enable_req_t;    /*^ API, SON_OAM_MRO_ENABLE_REQ  ^*/

/*! \  son_oam_mro_enable_resp_t
 *  \brief Response to MRO Enable Request
 *  \param result                 - Result
 *  \param error_code             - error code
 *  \param cell_status_list_size  - Number of elememts in the cell 
 *                                  status list
 *  \param cell_status_list       - List containing the status of each 
 *                                  cell whose disable request failed
 */
typedef struct _son_oam_mro_enable_resp_t
{
    son_return_et        result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et         error_code; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u16              cell_status_list_size; /*^ M, 0, H, 0, SON_MAX_CELL_SUPPORTED_AT_ENB ^*/
/*SPR_17664_end*/
    son_cell_status_t    cell_status_list[SON_MAX_CELL_SUPPORTED_AT_ENB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_mro_enable_resp_t; /*^ API, SON_OAM_MRO_ENABLE_RESP ^*/

/*
 * SON_OAM_MRO_DISABLE
 */

#define SON_MRO_DISABLE_SRV_CGI_PRESENT 0x01

/*! \  son_oam_mro_disable_req_t
 *  \brief MRO Disable Request Structure
 *  \param bitmask
 *  \param srv_cgi   CGI of cell on which MRO is to be disabled. In case no
 *                   cellId is present in the request then MRO has to be disabled
 *                   on all the cells of eNodeB
 */
typedef struct _son_oam_mro_disable_req_t
{
    son_u32                            bitmask;    /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t     srv_cgi;
    /*^ O, SON_MRO_DISABLE_SRV_CGI_PRESENT, N, 0, 0 ^*/
}son_oam_mro_disable_req_t;    /*^ API, SON_OAM_MRO_DISABLE_REQ  ^*/

/*! \  son_oam_mro_disable_resp_t
 *  \brief Response to MRO Disable Request
 *  \param result                 - Result
 *  \param error_code             - error code
 *  \param cell_status_list_size  - Number of elememts in the cell 
 *                                  status list
 *  \param cell_status_list       - List containing the status of each 
 *                                  cell whose disable request failed
 */
typedef struct _son_oam_mro_disable_resp_t
{
    son_return_et        result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et         error_code; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u16              cell_status_list_size; /*^ M, 0, H, 0, SON_MAX_CELL_SUPPORTED_AT_ENB ^*/
/*SPR_17664_end*/
    son_cell_status_t    cell_status_list[SON_MAX_CELL_SUPPORTED_AT_ENB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_mro_disable_resp_t;    /*^ API, SON_OAM_MRO_DISABLE_RESP  ^*/

/*
 * SON_OAM_MRO_MODIFY_CONFIG_PARAMS
 */

#define SON_MRO_MODE_PRESENT                                0x0001
#define SON_MRO_PARAM_PRESENT                               0x0002
#define SON_MRO_HO_FAILURE_ACCUMULATION_DURATION_PRESENT    0x0004
#define SON_MRO_DELTA_CIO_PRESENT                           0x0008 
#define SON_MRO_HO_ATTEMPTS_THRESHOLD_PRESENT                   0x0010
#define SON_MRO_HO_OPTIMIZATION_START_THRESHOLD_PRESENT         0x0020
#define SON_MRO_HO_OPTIMIZATION_STOP_THRESHOLD_PRESENT          0x0040
#define SON_MRO_HO_FAIL_RATE_FOR_OPTIMIZATION_PRESENT           0x0080
#define SON_MRO_TTT_PRESENT                                     0x0100
#define SON_MRO_DELTA_TTT_PRESENT                               0x0200
typedef struct _son_mro_eutran_config_param_t
{
    son_u32                         bitmask; /*^ BITMASK ^*/
    son_mode_et                     mro_mode;
    /*^ O, SON_MRO_MODE_PRESENT, H, 0, 1 ^*/
    son_mro_param_et                mro_param;
    /*^ O, SON_MRO_PARAM_PRESENT, H, 0, 2 ^*/
    son_u8                          mro_ho_failure_accumulation_duration;
    /*^ O, SON_MRO_HO_FAILURE_ACCUMULATION_DURATION_PRESENT, B, 5, 60 ^*/
    son_u8                          delta_cio;
    /*^ O, SON_MRO_DELTA_CIO_PRESENT, B, 1, 5 ^*/
    son_u16 mro_ho_attempts_threshold;
    /*^ O, SON_MRO_HO_ATTEMPTS_THRESHOLD_PRESENT, L, 1, 0 ^*/
    son_u16 mro_ho_optimization_start_threshold;
    /*^ O, SON_MRO_HO_OPTIMIZATION_START_THRESHOLD_PRESENT, B, 1, 1000 ^*/
    son_u16 mro_ho_optimization_stop_threshold;
    /*^ O, SON_MRO_HO_OPTIMIZATION_STOP_THRESHOLD_PRESENT, B, 1, 1000 ^*/
    son_u16 mro_ho_fail_rate_for_optimization;
    /*^ O, SON_MRO_HO_FAIL_RATE_FOR_OPTIMIZATION_PRESENT, B, 1, 1000 ^*/
    son_time_to_trigger_range_et ttt;
    /*^ O, SON_MRO_TTT_PRESENT, H, 0, 15 ^*/
    son_u8                          delta_ttt;
    /*^ O, SON_MRO_DELTA_TTT_PRESENT, B, 1, 5 ^*/   
}son_mro_eutran_config_param_t;

typedef struct _son_oam_mro_modify_config_params_req_t
{
    son_u32                        bitmask; /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t  srv_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_mro_eutran_config_param_t   eutran_config_params;
    /*^ M, 0, N, 0, 0 ^*/
} son_oam_mro_modify_config_params_req_t; /*^ API, SON_OAM_MRO_MODIFY_CONFIG_PARAM_REQ  ^*/

typedef struct _son_oam_mro_modify_config_params_resp_t
{
    son_intra_rat_global_cell_id_t  srv_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_return_et                   result;    /*^ M, 0, H, 0, 2 ^*/
    son_error_et                    error_code; /*^ M, 0, N, 0, 0 ^*/
}son_oam_mro_modify_config_params_resp_t; /*^ API, SON_OAM_MRO_MODIFY_CONFIG_PARAM_RESP ^*/

/*
 * SON_OAM_MRO_EUTRAN_TTT_UPDATE_IND   
 */

typedef struct _son_oam_mro_eutran_ttt_update_ind_t
{
    son_u32                        bitmask;
    /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t  srv_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_time_to_trigger_range_et    updated_ttt;
    /*^ M, 0, N, 0, 0 ^*/
}son_oam_mro_eutran_ttt_update_ind_t; /*^ API, SON_OAM_MRO_EUTRAN_TTT_UPDATE_IND ^*/

/*! \ son_mro_counter_info_t
 *  \brief  Stores HO counter info for each neighbour
 *  \param bitmask
 *  \param nbr_cgi  CGI of neighbour cell.
 *  \param num_too_early_ho Number of too early HO
 *  \param num_too_late_ho Number of too Late HO
 *  \param num_ho_to_wrong_cell_f Number of HO to wrong cell for failure cell
 *  \param num_ho_to_wrong_cell_r Number of HO to wrong cell for reestablisment cell.
 *  \param total_ho_attempted  Number of total HO attempted.
 */

typedef struct  _son_mro_counter_info_t
{
    son_u32                   bitmask;
    /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t  nbr_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_u16                   num_too_early_ho;
    /*^ M, 0, N, 0, 0 ^*/
    son_u16                   num_too_late_ho;
    /*^ M, 0, N, 0, 0 ^*/
    son_u16                   num_ho_to_wrong_cell_f;
    /*^ M, 0, N, 0, 0 ^*/
    son_u16                   num_ho_to_wrong_cell_r;
    /*^ M, 0, N, 0, 0 ^*/
    son_u16                   total_ho_attempted;
    /*^ M, 0, N, 0, 0 ^*/
}son_mro_counter_info_t;    

/*! \  son_oam_mro_ho_counter_report_t
 *  \brief HO counter report structure
 *  \param bitmask
 *  \param srv_cgi   CGI of cell on which MRO is enabled.
 *  \param neighbour_list_size  Number of neighbours 
 *  \param ho_counter_list      HO counter info for each neighbour
 */
typedef struct _son_oam_mro_ho_counter_report_t
{
    son_u32                         bitmask; 
    /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t  srv_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_u16                         neighbour_list_size; 
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_MAX_NO_NEIGHBOUR_CELLS ^*/
/*SPR_17664_end*/
    son_mro_counter_info_t  ho_counter_list[SON_MAX_NO_NEIGHBOUR_CELLS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_mro_ho_counter_report_t; /*^ API, SON_OAM_MRO_HO_COUNTER_REPORT ^*/

/* REL1.3 MLB CHANGES : STARTS */

#define SON_OAM_MLB_ENABLE_SRC_CGI_PRESENT 0x01

/*! \  son_oam_mlb_enable_req_t
 *  \brief MLB Enable Request Structure
 *  \param bitmask
 *  \param src_cgi   CGI of cell on which MLB is to be enabled. In case no
 *                   cellId is present in the request then MLB  has to be enabled
 *                   on all the cells of eNodeB
 */
typedef struct _son_oam_mlb_enable_req_t
{
   son_u32                        bitmask;
   /*^ BITMASK ^*/
   son_intra_rat_global_cell_id_t src_cgi;
   /*^ O, SON_OAM_MLB_ENABLE_SRC_CGI_PRESENT, N, 0, 0 ^*/
}son_oam_mlb_enable_req_t;
/*^ API, SON_OAM_MLB_ENABLE_REQ  ^*/

/*! \  son_oam_mlb_enable_resp_t
 *  \brief MLB Enable Response Structure
 *  \param result                 Success/Failure
 *  \param error_code             cause (Valid in case of failure)
 *  \param cell_status_list_size  Number of elements in the cell status list
 *  \param cell_status_list       List containing the status of each cell 
 *                                requested for MLB Enable.If successful the
 *                                list will be empty. Otherwise, it will
 *                                contain the list of cell id for which
 *                                MLB could not be enabled.
 */
typedef struct _son_oam_mlb_enable_resp_t
{
    son_return_et        result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et         error_code; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u16              cell_status_list_size; /*^ M, 0, H, 0, SON_MAX_CELL_SUPPORTED_AT_ENB ^*/
/*SPR_17664_end*/
    son_cell_status_t    cell_status_list[SON_MAX_CELL_SUPPORTED_AT_ENB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_mlb_enable_resp_t;
/*^ API, SON_OAM_MLB_ENABLE_RESP  ^*/

#define SON_OAM_MLB_DISABLE_SRC_CGI_PRESENT 0x01

/*! \  son_oam_mlb_disable_req_t
 *  \brief MLB Disable Request Structure
 *  \param bitmask
 *  \param src_cgi   CGI of cell on which MLB is to be disabled. In case no
 *                   cellId is present in the request then MLB  has to be disabled
 *                   on all the cells of eNodeB
 */
typedef struct _son_oam_mlb_disable_req_t
{
   son_u32                        bitmask;
   /*^ BITMASK ^*/
   son_intra_rat_global_cell_id_t src_cgi;
   /*^ O, SON_OAM_MLB_DISABLE_SRC_CGI_PRESENT, N, 0, 0 ^*/
}son_oam_mlb_disable_req_t;
/*^ API, SON_OAM_MLB_DISABLE_REQ  ^*/

/*! \  son_oam_mlb_enable_resp_t
 *  \brief MLB Disable Response Structure
 *  \param result                 Success/Failure
 *  \param error_code             cause (Valid in case of failure)
 *  \param cell_status_list_size  Number of elements in the cell status list
 *  \param cell_status_list       List containing the status of each cell 
 *                                requested for Disable. If successful the list
 *                                will be empty. Otherwise, it will
 *                                contain the list of cell id for
 *                                which MLB could not be disabled
 */
typedef struct _son_oam_mlb_disable_resp_t
{
    son_return_et        result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et         error_code; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u16              cell_status_list_size; /*^ M, 0, H, 0, SON_MAX_CELL_SUPPORTED_AT_ENB ^*/
/*SPR_17664_end*/
    son_cell_status_t    cell_status_list[SON_MAX_CELL_SUPPORTED_AT_ENB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_mlb_disable_resp_t;
/*^ API, SON_OAM_MLB_DISABLE_RESP  ^*/

#define SON_OAM_MLB_Q_OFF_MIN_PRESENT     0x01
#define SON_OAM_MLB_Q_OFF_MAX_PRESENT     0x02

/*! \  son_eutran_q_offset_config_t
 *  \brief Structre to contain the q-offset max and min measurement
 *  \param bitmask                Bitmask
 *  \param q_offset_min           Indicates minimum value up to 
 *                                which MLB can tune QOffset of
 *                                Intra-Rat Neighbors.
 *  \param q_offset_max           Indicates maximum value up to
 *                                which MLB can tune QOffset of
 *                                Intra-Rat Neighbors.
 */
typedef struct _son_eutran_q_offset_config_t
{
    son_u32 bitmask; /*^ BITMASK ^*/
   son_q_offset_range_et    q_offset_min;
   /*^ O, SON_OAM_MLB_Q_OFF_MIN_PRESENT, H, 0, 30 ^*/
   son_q_offset_range_et    q_offset_max;
   /*^ O, SON_OAM_MLB_Q_OFF_MAX_PRESENT, H, 0, 30 ^*/
}son_eutran_q_offset_config_t;

#define SON_OAM_MLB_EUTRA_Q_OFFSET_CONFIG_PRESENT 0x01
#define SON_OAM_MLB_MODE_PRESENT                  0x02

/*! \  son_oam_mlb_modify_attributes_req_t
 *  \brief  MLB Modify attributes request Structure
 *  \param bitmask                   Bitmask
 *  \param srv_cgi                   Serving cell CGI
 *  \param eutra_q_offset_config     Indicates Qoffset Configuration to 
 *                                   be used by MLB Algorithm for
 *                                   Qoffset Tuning.
 */
typedef struct _son_oam_mlb_modify_attributes_req_t
{
    son_u32                        bitmask;
    /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t srv_cgi;
    /*^ M,0, N, 0, 0 ^*/
    son_eutran_q_offset_config_t   eutra_q_offset_config;
    /*^ O,SON_OAM_MLB_EUTRA_Q_OFFSET_CONFIG_PRESENT, N, 0, 0 ^*/
    son_mode_et mlb_mode;
    /*^ O,SON_OAM_MLB_MODE_PRESENT, H, 0, 1 ^*/
}son_oam_mlb_modify_attributes_req_t;
/*^ API, SON_OAM_MLB_MODIFY_ATTRIBUTES_REQ ^*/

/*! \  son_oam_mlb_modify_attributes_resp_t
 *  \brief  MLB Modify attributes request Structure
 *  \param srv_cgi                   Serving cell CGI
 *  \param result                    Success/failure
 *  \param error_code                Cause ( valid in case of FAILURE )
 */
typedef struct _son_oam_mlb_modify_attributes_resp_t
{
    son_intra_rat_global_cell_id_t srv_cgi;
    /*^ M,0, N, 0, 0 ^*/
    son_return_et        result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et         error_code; /*^ M, 0, N, 0, 0 ^*/
}son_oam_mlb_modify_attributes_resp_t;
/*^ API, SON_OAM_MLB_MODIFY_ATTRIBUTES_RESP ^*/

/* REL1.3 MLB CHANGES : ENDS */ 

/*! \  son_oam_time_stamp_t
 *  \brief Stores the current time
 *  \param year     Year
 *  \param month    Month   
 *  \param day      Day
 *  \param hour     Hour
 *  \param min      Minute
 *  \param sec      Second
 */
typedef struct _son_oam_time_stamp_t
{
    son_u16 year;       /*^ M,0, N, 0, 0 ^*/
    son_u16 month;      /*^ M,0, B, 1, 12 ^*/
    son_u16 day;        /*^ M,0, B, 1, 31 ^*/
    son_u16 hour;       /*^ M,0, H, 0, 23 ^*/
    son_u16 min;        /*^ M,0, H, 0, 59 ^*/
    son_u16 sec;        /*^ M,0, H, 0, 59 ^*/
}son_oam_time_stamp_t;

/*! \enum son_oam_error_type_et
 *  \brief An enum that specifies Event Error Type
 */
typedef enum
{
    SON_OAM_SW_PROCESSING_ERROR = 1,
    SON_OAM_MEMORY_UNAVAILABLE_ERROR    
}son_oam_error_type_et;

/*! \enum son_oam_error_event_ids_et
 *  \brief An enum that specifies Event Error Ids
 */
typedef enum
{
    SON_OAM_MEMORY_ALLOCATION_FAIL = 1,
    SON_OAM_MEMORY_FREE_FAIL,          
    SON_OAM_TIMER_START_FAIL,    
    SON_OAM_TIMER_STOP_FAIL,    
    SON_OAM_SEND_FAIL,    
    SON_OAM_RECEIVE_FAIL    
}son_oam_error_event_ids_et;

/*! \enum son_oam_event_error_cause_et
 *  \brief An enum that specifies Event Error Cause
 */
typedef enum
{
    SON_OAM_INVALID_PARAMS = 1,
    SON_OAM_NO_FREE_BUFFER,          
    SON_OAM_TIMER_ALREADY_RUNNING,    
    SON_OAM_TIMER_ALREADY_STOP,    
    SON_OAM_INVALID_IPC_HANDLE,    
    SON_OAM_OTHER    
}son_oam_event_error_cause_et;

/*! \enum son_oam_event_severity_et
 *  \brief An enum that specifies Event Severity Type
 */
typedef enum
{
    SON_OAM_MINOR_ERROR = 1,
    SON_OAM_MAJOR_ERROR,          
    SON_OAM_CRITICAL_ERROR,    
    SON_OAM_INFORMATIONAL    
}son_oam_event_severity_et;

/*! \enum son_oam_event_class_et
 *  \brief An enum that specifies Event Class
 */
typedef enum
{
    SON_OAM_ERROR_EVENT = 1
}son_oam_event_class_et;

/*! \enum son_oam_event_class_et
 *  \brief An enum that specifies Event Sub-Class
 */
typedef enum
{
    SON_OAM_MEMORY_ERROR = 1,
    SON_OAM_TIMER_ERROR,
    SON_OAM_SOCKET_ERROR
}son_oam_event_subclass_et;

#define SON_OAM_EVENT_ERROR_CAUSE_PRESENT          0x01
#define SON_OAM_EVENT_SEVERITY_PRESENT             0x02
#define SON_OAM_EVENT_ASSOCIATED_DATA_PRESENT      0x03

/*! \  son_oam_error_info_t
 *  \brief son_oam_error_info_t message structure used to indicate the error reasons.
 *  \param bitmask      Bitmask 
 *  \param error_type   specifies the SON OAM Notification Event Error Type
 *  \param error_cause  specifies the SON OAM Notification Event Error Cause
 *  \param severity     specifies the SON OAM Notification Event Severity Type
 *  \param associated_data    It is a string contains additional information
 */
typedef struct _son_oam_error_info_t
{
    son_u32                   bitmask; /*^ BITMASK ^*/
    son_oam_error_type_et     error_type; /*^ M,0, N, 0, 0 ^*/
    son_oam_event_error_cause_et     error_cause; 
    /*^ O,SON_OAM_EVENT_ERROR_CAUSE_PRESENT, N, 0, 0 ^*/
    son_oam_event_severity_et     severity;
    /*^ O,SON_OAM_EVENT_SEVERITY_PRESENT, N, 0, 0 ^*/
    son_u8                        associated_data[SON_OAM_MAX_ASSOCIATED_DATA_LEN]; 
    /*^ O,SON_OAM_EVENT_ASSOCIATED_DATA_PRESENT, N, 0, 0 ^*/
}son_oam_error_info_t;

/*! \  son_oam_event_header_t
 *  \brief son_oam_event_header_t message structure used to indicate the event header.
 *  \param bitmask      Bitmask 
 *  \param error_type   specifies the SON OAM Notification Event Error Type
 *  \param error_cause  specifies the SON OAM Notification Event Error Cause
 *  \param severity     specifies the SON OAM Notification Event Severity Type
 *  \param associated_data    It is a string contains additional information
 */
typedef struct _son_oam_event_header_t
{
    son_oam_time_stamp_t     time_stamp; /*^ M,0, N, 0, 0 ^*/
    son_oam_event_class_et event_type; /*^ M,0, N, 0, 0 ^*/
    son_oam_event_subclass_et event_sub_type; /*^ M,0, N, 0, 0 ^*/
    son_u16 event_id; /*^ M,0, N, 0, 0 ^*/
}son_oam_event_header_t;

#define SON_OAM_EVENT_API_DATA_PRESENT   0x01

/*! \  son_oam_event_notification_t
 *  \brief SON_OAM_EVENT_NOTIFICATION  message structure used to indicate the system level issues/events.
 *  \param bitmask      Bitmask 
 *  \param time_stamp   Time at which event is raised
 *  \param event_id     Event identifier
 */
typedef struct _son_oam_event_notification_t
{
    son_u32                   bitmask; /*^ BITMASK ^*/
    son_oam_event_header_t    msg_header; /*^ M,0, N, 0, 0 ^*/
    son_oam_error_info_t      api_data; /*^ O,SON_OAM_EVENT_API_DATA_PRESENT, N, 0, 0 ^*/    
}son_oam_event_notification_t;
/*^ API, SON_OAM_EVENT_NOTIFICATION ^*/

/* SPR-10331 Fix Starts */
#define SON_OAM_GU_GROUP_ID_TO_ADD_LIST_PRESENT      0x01
#define SON_OAM_GU_GROUP_ID_TO_DELETE_LIST_PRESENT   0x02
/* SPR-10331 Fix Ends */
/*! \  son_oam_anr_gu_group_id_config_req_t
 *  \brief Structure defining data of the API to configure globally unique group ids for local eNB.
 *  \param bitmask      Bitmask 
 *  \param gu_group_id_to_add_list     Serving eNB GU Group Ids which have been newly added
 *  \param gu_group_id_to_delete_list  Serving eNB GU Group Ids which have been deleted
 */
typedef struct _son_oam_anr_gu_group_id_config_req_t
{
    /* SPR-10331 Fix Starts */
    son_u32                   bitmask; /*^ BITMASK ^*/
    son_gu_group_id_list_t    gu_group_id_to_add_list; 
    /*^ O, SON_OAM_GU_GROUP_ID_TO_ADD_LIST_PRESENT, N, 0, 0 ^*/
    son_gu_group_id_list_t    gu_group_id_to_delete_list;    
    /*^ O, SON_OAM_GU_GROUP_ID_TO_DELETE_LIST_PRESENT, N, 0, 0 ^*/
    /* SPR-10331 Fix Ends */
}son_oam_anr_gu_group_id_config_req_t; /*^ API, SON_OAM_ANR_GU_GROUP_ID_CONFIG_REQ ^*/

/*! \  son_oam_anr_gu_group_id_config_resp_t
 *  \brief   This API is a response to SON_OAM_ANR_GU_GROUP_ID_CONFIG_REQ
 *  \param   result     -     Success/Failure
 *  \param   error_code -     Cause (valid in case of Failure)
 */
typedef struct _son_oam_anr_gu_group_id_config_resp_t
{
    son_return_et               result;     /*^ M, 0, H, 0, 1 ^*/ 
    son_error_et                error_code; /*^ M, 0, N, 0, 0 ^*/
}son_oam_anr_gu_group_id_config_resp_t;    /*^ API, SON_OAM_ANR_GU_GROUP_ID_CONFIG_RESP ^*/

#define SON_OAM_ANR_NBR_DEF_NR_STATUS_PRESENT   0x01
#define SON_OAM_ANR_NBR_DEF_HO_STATUS_PRESENT   0x02
#define SON_OAM_ANR_NBR_DEF_X2_STATUS_PRESENT   0x04
#define SON_OAM_ANR_CSG_DEF_SCHEME_PRESENT      0x08
#define SON_OAM_ANR_PICO_DEF_SCHEME_PRESENT     0x10
#define SON_OAM_ANR_MACRO_DEF_SCHEME_PRESENT    0x20

/*! \  son_oam_anr_neighbor_default_config_req_t
 *  \brief SON_OAM_ANR_NEIGHBOR_DEFAULT_CONFIG_REQ
 *  \param  bitmask     - Bitmask
 *  \param  nr_status   - NR Status
 *  \param  ho_status   - HO Status
 *  \param  x2_status   - X2 Status 
 *  \param  csg_cell_intf_scheme - scheme type used for CSG cell
 *  \param  pico_cell_intf_scheme - scheme type used for pico cell
 *  \param  macro_cell_intf_scheme - scheme type used for macro cell
 */
typedef struct _son_oam_anr_neighbor_default_config_req_t
{
    son_u32             bitmask; /*^ BITMASK ^*/
    /* SPR_14100_start */
    son_nr_status_et    nr_status;
    /*^ O, SON_OAM_ANR_NBR_DEF_NR_STATUS_PRESENT, B, 1, 255 ^*/
    son_ho_status_et    ho_status;
    /*^ O, SON_OAM_ANR_NBR_DEF_HO_STATUS_PRESENT, B, 1, 255 ^*/
    son_x2_status_et    x2_status;
    /*^ O, SON_OAM_ANR_NBR_DEF_X2_STATUS_PRESENT, B, 1, 255 ^*/
    /* SPR_14100_end */
/* eICIC_PHASE_1_2_CHANGES_START */
    son_intf_scheme_type_et csg_cell_intf_scheme;
    /*^ O, SON_OAM_ANR_CSG_DEF_SCHEME_PRESENT, H, 0, 2 ^*/
    son_intf_scheme_type_et pico_cell_intf_scheme;
    /*^ O, SON_OAM_ANR_PICO_DEF_SCHEME_PRESENT, H, 0, 2 ^*/
    son_intf_scheme_type_et macro_cell_intf_scheme;
    /*^ O, SON_OAM_ANR_MACRO_DEF_SCHEME_PRESENT, H, 0, 2 ^*/
/* eICIC_PHASE_1_2_CHANGES_END */
}son_oam_anr_neighbor_default_config_req_t; /*^ API, SON_OAM_ANR_NEIGHBOR_DEFAULT_CONFIG_REQ ^*/

/*! \  son_oam_anr_neighbor_default_config_resp_t
 *  \brief   This API is a response to SON_OAM_ANR_NEIGHBOR_DEFAULT_CONFIG_REQ
 *  \param   result     -     Success/Failure
 *  \param   error_code -     Cause (valid in case of Failure)
 */
typedef struct _son_oam_anr_neighbor_default_config_resp_t
{
    son_return_et               result;     /*^ M, 0, H, 0, 1 ^*/ 
    son_error_et                error_code; /*^ M, 0, N, 0, 0 ^*/
}son_oam_anr_neighbor_default_config_resp_t;    /*^ API, SON_OAM_ANR_NEIGHBOR_DEFAULT_CONFIG_RESP ^*/

/*
 *  SON_OAM_CARRIER_FREQ_AND_DL_UL_POWER_SELECTION_REQ
 */

/*! \ son_oam_carrier_freq_and_dl_tx_power_req_t
 *  \brief   OAM sends this API to SON to request for the carrier frequency.
 *           reference signal power for downlink transmission and UE transmit power limit
 *  \param   meas_bandwidth    -  Measurement bandwidth (applicable to all the EARFCNs)
 *  \param   earfcn_list_size  -  Number of carrier frequencies/EARFCN
 *  \param   earfcn_list       -  List of EARFCN
 *  \param   p_tx_upp          -  Maximum downlink  referenceSignalPower (dBm)
 *  \param   p_tx_low          -  Minimum downlink  referenceSignalPower (dBm)
 *  \param   p_ue_max          -  Maximum UE transmit Power (dBm). Default 23dBm
 *  \param   p_ue_min          -  Minimum UE transmit Power (dBm). Default -40dBm
 */
typedef struct _son_oam_carrier_freq_and_dl_tx_power_req_t
{
    son_u8                       meas_bandwidth;
    /*^M, 0, B, 6, 100^*/
    son_u8                       earfcn_list_size;
/*SPR_17664_start*/
    /*^M, 0, B, 1, SON_MAX_NO_EARFCN^*/
/*SPR_17664_end*/
    /* Coverity 110820 Fix Start */
    son_u32                      earfcn_list[SON_MAX_NO_EARFCN];
    /* Coverity 110820 Fix End */

    /*^M, 0,OCTET_STRING, VARIABLE^*/
    son_s8                       p_tx_upp;
    /*^M, 0,B, -60, 50^*/
    son_s8                       p_tx_low;
    /*^M, 0, B, -60, 50^*/
    /* + SRR_17523_FIX */
    son_s8                        p_ue_max;
    /*^M, 0, B, -40, 33^*/
    son_s8                        p_ue_min;
    /*^M, 0, B, -40, 33^*/
    /* - SRR_17523_FIX */
}son_oam_carrier_freq_and_dl_tx_power_req_t;
/*^ API, SON_OAM_CARRIER_FREQ_AND_DL_UL_POWER_SELECTION_REQ ^*/

/*
 *  SON_OAM_CARRIER_FREQ_AND_DL_UL_POWER_SELECTION_RES
 */

#define SON_OAM_CARR_FREQ_DL_TX_PWR_DL_EARFCN_PRESENT         0x01
#define SON_OAM_CARR_FREQ_DL_TX_PWR_REF_SIGNAL_POWER_PRESENT  0x02
#define SON_OAM_CARR_FREQ_DL_TX_PWR_P_MAX_PRESENT             0x04

/*! \ son_oam_carrier_freq_and_dl_tx_power_resp_t
 *  \brief  SON sends this API to OAM in response
 *          to SON_OAM_CARRIER_FREQ_AND_DL_UL_POWER_SELECTION_REQ API.
 *  \param  bitmask                 - BITMASK.
 *  \param  result                  - This field indicates the result value
 *  \param  error_code              - This field indicated the failure cause.
 *  \param  dl_earfcn               - DL ARFCN of the selected carrier frequency.
 *  \param  reference_signal_power  - Calculated reference signal power  (dBm).
 *  \param  p_max                   - Calculated UE transmit power limit.
 */
typedef struct _son_oam_carrier_freq_and_dl_tx_power_resp_t
{
  son_u32                  bitmask; /*^BITMASK^*/
  son_return_et            result;
  /*^ M, 0, H, 0, 1 ^*/
  son_error_et             error_code;
  /*^ M, 0, N, 0, 0 ^*/
  son_u32                  dl_earfcn;
  /*^ O, SON_OAM_CARR_FREQ_DL_TX_PWR_DL_EARFCN_PRESENT,N, 0, 0 ^*/
  son_s8                   reference_signal_power;
  /*^ O, SON_OAM_CARR_FREQ_DL_TX_PWR_REF_SIGNAL_POWER_PRESENT,B, -60,50 ^*/
  son_s8                   p_max;
    /* + SPR_17523_FIX */
  /*^O,SON_OAM_CARR_FREQ_DL_TX_PWR_P_MAX_PRESENT,B, -40,33 ^*/
    /* - SPR_17523_FIX */
}son_oam_carrier_freq_and_dl_tx_power_resp_t;
/*^ API, SON_OAM_CARRIER_FREQ_AND_DL_UL_POWER_SELECTION_RES ^*/
                                                                                                     
typedef struct _son_oam_last_scan_timestamp_ind_t
{
    son_u32                     bitmask; /*^ BITMASK ^*/
    son_oam_time_stamp_t        time_stamp; /*^ M,0, N, 0, 0 ^*/
}son_oam_last_scan_timestamp_ind_t;   /*^ API,  SON_OAM_LAST_SCAN_TIMESTAMP_IND ^*/ 

/*SONMLB-- Centralized Support Start */

/*HO PARAMS START */
#define SON_OAM_MLB_OFFSET_ATTR_OFS_PRESENT 0x01
#define SON_OAM_MLB_OFFSET_ATTR_OCS_PRESENT 0x02
#define SON_OAM_MLB_OFFSET_ATTR_OFN_PRESENT 0x04
#define SON_OAM_MLB_OFFSET_ATTR_OFF_PRESENT 0x08
#define SON_OAM_MLB_OFFSET_ATTR_HYS_PRESENT 0x10

/*! \ cell_offset_attr_t
 *  \brief This struct is used for SON offset attribute info
 *  \param Bitmask
 *  \param ofs
 *  \param ocs
 *  \param ofn
 *  \param off
 *  \param hys
 */
typedef struct _cell_offset_attr_t
{
    son_u32 bitmask;
    /*^ BITMASK ^*/
    son_s8 ofs;
    /*^ O,SON_OAM_MLB_OFFSET_ATTR_OFS_PRESENT, B, -24, 24 ^*/
    son_s8 ocs;
    /*^ O,SON_OAM_MLB_OFFSET_ATTR_OCS_PRESENT, B, -24, 24 ^*/
    son_s8 ofn;
    /*^ O,SON_OAM_MLB_OFFSET_ATTR_OFN_PRESENT, B, -24, 24 ^*/
    son_s8 off;
    /*^ O,SON_OAM_MLB_OFFSET_ATTR_OFF_PRESENT, B, -24, 24 ^*/
    son_u8 hys;
    /*^ O,SON_OAM_MLB_OFFSET_ATTR_HYS_PRESENT, H, 0, 30 ^*/
} son_cell_offset_attr_t;

#define SON_OAM_MLB_SRC_RSRP_PRESENT  0x01
#define SON_OAM_MLB_TARGET_RSRP_PRESENT  0x02
#define SON_OAM_MLB_CELL_OFSSET_ATTR_PRESENT  0x04

/*! \ son_mlb_ho_params_t
 *  \brief This struct is used for SONMLB HO attribute info
 *  \param Bitmask
 *  \param src_rsrp serving cell rsrp
 *  \param trgt_rsrp neighbor cell rsrp
 *  \param offset_attr serving cell offset attributes w.r.t neighbor
 */
typedef struct _son_mlb_ho_params
{
    son_u32 bitmask;
    /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t trgt_cgi;
    /*^ M,0, N, 0, 0 ^*/
    son_u8 src_rsrp;
    /*^ O,SON_OAM_MLB_SRC_RSRP_PRESENT, H, 0, 97 ^*/
    son_u8 trgt_rsrp;
    /*^ O,SON_OAM_MLB_TARGET_RSRP_PRESENT, H, 0, 97 ^*/
    son_cell_offset_attr_t offset_attr;
    /*^ O,SON_OAM_MLB_CELL_OFSSET_ATTR_PRESENT, N, 0, 0 ^*/
} son_mlb_ho_params_t;
/*HO PARAMS END */

/*LOAD PARAMS START */

#define  MLB_TRGT_CELL_CAP_VAL_PRESENT  0x01

/*! \ son_mlb_load_params_t
 *  \brief This struct is used for SONMLB HO attribute info
 *  \param Bitmask
 *  \param trgt_cgi                  Neighbor cell CGI
 *  \param trgt_dl_comp_avl_cap_val  neighbor cell dl composite available capacity
 */
typedef struct _son_nbr_cell_load_info_t
{
    son_u32 bitmask;
    /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t trgt_cgi;
    /*^ M,0, N, 0, 0 ^*/
    son_u8 trgt_dl_comp_avl_cap_val;
    /*^ O,MLB_TRGT_CELL_CAP_VAL_PRESENT, H, 0, 100 ^*/
} son_nbr_cell_load_info_t;

#define  MLB_SRC_CELL_CAP_VAL_PRESENT   0x01

/*! \ son_mlb_load_params_t
 *  \brief This struct is used for SONMLB HO attribute info
 *  \param Bitmask
 *  \param src_dl_comp_avl_cap_val   serving cell dl composite available capacity
 *  \param count                     Number of neighbor cells
 *  \param nbr_cell_load_info        neighbor cell load info
 */
typedef struct _son_mlb_load_params_t
{
    son_u32 bitmask;
    /*^ BITMASK ^*/
    son_u8 src_dl_comp_avl_cap_val;
    /*^ O,MLB_SRC_CELL_CAP_VAL_PRESENT, H, 0, 100 ^*/
    son_u8 count;
/*SPR_17664_start*/
/*^ M, 0, H, 0, SON_MAX_NO_INTRA_NBR_CELLS ^*/
/*SPR_17664_end*/
    son_nbr_cell_load_info_t nbr_cell_load_info[SON_MAX_NO_INTRA_NBR_CELLS];
    /*^M, 0,OCTET_STRING, VARIABLE^*/
} son_mlb_load_params_t;
/*LOAD PARAMS END */

#define SON_OAM_MLB_HO_PARAMS_PRESENT    0x01
#define SON_OAM_MLB_LOAD_PARAMS_PRESENT  0x02

/*! \  son_oam_mlb_params_info_t
 *  \brief  MLB Params info to OAM when SON is in Centralized mode.
 *  \param bitmask                   Bitmask
 *  \param srv_cgi                   Serving cell CGI
 *  \param trgt_cgi                  Neighbor cell CGI
 *  \param son_mlb_ho_params_t       Indicates Handover params to
 *                                   be used by MLB Algorithm for
 *                                   Qoffset Tuning.
 *  \param son_mlb_load_params_t     Indicates load report params to
 *                                   be used by MLB Algorithm for
 *                                   Qoffset Tuning.
 */
typedef struct _son_oam_mlb_params_info
{
    son_u32 bitmask;
    /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t srv_cgi;
    /*^ M,0, N, 0, 0 ^*/
    son_mlb_ho_params_t ho_params;
    /*^ O,SON_OAM_MLB_HO_PARAMS_PRESENT, N, 0, 0 ^*/
    son_mlb_load_params_t load_params;
    /*^ O,SON_OAM_MLB_LOAD_PARAMS_PRESENT, N, 0, 0 ^*/
} son_oam_mlb_params_info_t;
/*^ API, SON_OAM_MLB_PRARAMS_INFO ^*/

#define SON_OAM_MLB_TRGT_CSG_ID_PRESENT  0x01
#define SON_OAM_MLB_PROPOSED_MOB_PARAMS_PRESENT  0x02

/*! \  son_oam_mlb_offset_change_req_t
 *  \brief  OAM sends this message to SONMLB in Centralized mode for setting offset value or
 *          negotiating the mobility params.
 *  \param bitmask                   Bitmask
 *  \param srv_cgi                   Serving cell CGI
 *  \param trgt_cgi                  Neighbor cell CGI
 *  \param trgt_csg_id_present       Indicates target CGI is present or not
 *  \param proposed_mobility_params  Indicates poposed mobility params
 *                                   present in case of connected mode MLB
 */
typedef struct _son_oam_mlb_offset_change_req_t
{
    son_u32 bitmask;
    /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t srv_cgi;
    /*^ M,0, N, 0, 0 ^*/
    son_intra_rat_global_cell_id_t trgt_cgi;
    /*^ M,0, N, 0, 0 ^*/
    son_bool_et trgt_csg_id_present;
    /*^ O,SON_OAM_MLB_TRGT_CSG_ID_PRESENT, N, 0, 0 ^*/
    son_s8 proposed_mobility_params;
    /*^ O,SON_OAM_MLB_PROPOSED_MOB_PARAMS_PRESENT, N, 0, 0 ^*/
} son_oam_mlb_offset_change_req_t;
/*^ API, SON_OAM_MLB_OFFSET_CHANGE_REQ ^*/

typedef struct _son_mob_param_range_t
{
    son_s8 lower_limit;
    /*^ M,0, N, 0, 0 ^*/
    son_s8 upper_limit;
    /*^ M,0, N, 0, 0 ^*/
}son_mob_param_range_t;

#define SON_OAM_MLB_TRGT_CGI_PRESENT        0x01
#define SON_OAM_MLB_MOBILITY_RANGE_PRESENT  0x02

/*! \  son_oam_mlb_offset_change_resp_t
 *  \brief  SONMLB sends this message in response of SON_OAM_MLB_OFFSET_CHANGE_REQ in Centralized mode
 *          to indicate success or failure or forwards the peer eNB response.
 *  \param bitmask                   Bitmask
 *  \param srv_cgi                   Serving cell CGI
 *  \param trgt_cgi                  Neighbor cell CGI
 *  \param mlb_status                Indicates SUCCESS or FAILURE
 *                                   SUCCESS: in case of Idle mode MLB or receives
 *                                   mobility change ack from peer eNB
 *  \param error_code                Indicates error code
 *  \param son_mob_param_range_t     present in case of negotiation fails with
 *                                   peer eNB with mobility change failure received.
 *                                   If mlb_status = FAILURE
 */
typedef struct _son_oam_mlb_offset_change_resp_t
{
    son_u32 bitmask;
    /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t srv_cgi;
    /*^ M,0, N, 0, 0 ^*/
    son_return_et mlb_status;
    /*^ M,0, N, 0, 0 ^*/
    son_error_et error_code;
    /*^ M, 0, N, 0, 0 ^*/
    son_intra_rat_global_cell_id_t trgt_cgi;
    /*^ O,SON_OAM_MLB_TRGT_CGI_PRESENT, N, 0, 0 ^*/
    son_mob_param_range_t mob_range;
    /*^ O,SON_OAM_MLB_MOBILITY_RANGE_PRESENT, N, 0, 0 ^*/
} son_oam_mlb_offset_change_resp_t;
/*^ API, SON_OAM_MLB_OFFSET_CHANGE_RESP ^*/

#define SON_OAM_MLB_PEER_PROPOSED_MOB_PARAMS_PRESENT  0x01

/*! \  son_oam_mlb_peer_offset_change_req_t
 *  \brief  At target eNB, SON-MLB on receiving MOBILITY CHANGE REQ from SON_X2 sends this message to
 *          OAM in Centralized mode for setting offset value or
 *          negotiating the mobility params.
 *  \param bitmask                   Bitmask
 *  \param src_cgi                   Source cell CGI
 *  \param trgt_cgi                  Target cell CGI
 *  \param proposed_mobility_params  Indicates poposed mobility params
 *                                   present in case of connected mode MLB
 */
typedef struct _son_oam_mlb_peer_offset_change_req_t
{
    son_u32 bitmask;
    /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t src_cgi;
    /*^ M,0, N, 0, 0 ^*/
    son_intra_rat_global_cell_id_t trgt_cgi;
    /*^ M,0, N, 0, 0 ^*/
    son_s8 proposed_mobility_params;
    /*^ O,SON_OAM_MLB_PEER_PROPOSED_MOB_PARAMS_PRESENT, N, 0, 0 ^*/
} son_oam_mlb_peer_offset_change_req_t;
/*^ API, SON_OAM_MLB_PEER_OFFSET_CHANGE_REQ ^*/

#define SON_OAM_MLB_SRC_CSG_ID_PRESENT           0x01
#define SON_OAM_MLB_PEER_MOBILITY_RANGE_PRESENT  0x02

/*! \  son_oam_mlb_peer_offset_change_resp_t
 *  \brief  At target eNB,OAM sends this message in response of
 *          SON_OAM_MLB_PEER_OFFSET_CHANGE_REQ in Centralized mode
 *          to indicate success or failure to the source eNB request.
 *  \param bitmask                   Bitmask
 *  \param src_cgi                   Source cell CGI
 *                                   (Same as src_cgi in SON_OAM_MLB_PEER_OFFSET_CHANGE_REQ)
 *  \param trgt_cgi                  Target cell CGI
 *  \param mlb_status                Indicates SUCCESS or FAILURE
 *                                   SUCCESS: in case of Idle mode MLB or receives
 *                                   mobility change ack from peer eNB
 *  \param src_csg_id_present        Indicates source CGI is present or not
 *  \param son_mob_param_range_t     present in case of negotiation fails with
 *                                   peer eNB with mobility change failure received.
 *                                   If mlb_status = FAILURE
 */
typedef struct _son_oam_mlb_peer_offset_change_resp_t
{
    son_u32 bitmask;
    /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t src_cgi; /*Same as src_cgi in SON_OAM_MLB_PEER_OFFSET_CHANGE_REQ*/
    /*^ M,0, N, 0, 0 ^*/
    son_return_et mlb_status;
    /*^ M,0, N, 0, 0 ^*/
    son_intra_rat_global_cell_id_t trgt_cgi;
    /*^ M,0, N, 0, 0 ^*/
    son_bool_et src_csg_id_present;
    /*^ O,SON_OAM_MLB_SRC_CSG_ID_PRESENT, N, 0, 0 ^*/
    son_mob_param_range_t mob_range;
    /*^ O,SON_OAM_MLB_PEER_MOBILITY_RANGE_PRESENT, N, 0, 0 ^*/
} son_oam_mlb_peer_offset_change_resp_t;
/*^ API, SON_OAM_MLB_PEER_OFFSET_CHANGE_RESP ^*/

/*SONMLB-- Centralized Support End */

#define SON_NBR_INTRA_RAT_GLOBAL_CELL_ID_PRESENT    0x01

/*! \ son_oam_mro_optimization_notification_t
 *  \brief  SON sends this API to OAM to indicate that MRO is not able to tune the mobility parameters to achieve the desired HO failure rate.
 *  \param  bitmask                 - BITMASK
 *  \param  srv_cgi                 - Serving cell CGI
 *  \param  nbr_cgi                 - Neighbor cell CGI
 *  \param  cause                   - Reason for sending this API
 *  \param  ho_failure_level        - Indicates the level at which MRO optimization is stopped.
 *  \param  ho_failure_rate         - Total HO Failure rate including (TOO Early HO Failure, TOO Late HO Failure, To Wrong Cell). If nbr_cgi is present then this is the total HO Failure between source cell and target cell for last accumulation duration. If nbr_cgi is not present then this is the total HO Failure between source cell and all its target cells for last accumulation duration. Actual value = Provided value / 100.
 */
typedef struct _son_oam_mro_optimization_notification_t
{
    son_u32 bitmask; /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t srv_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_intra_rat_global_cell_id_t nbr_cgi;
    /*^ O, SON_NBR_INTRA_RAT_GLOBAL_CELL_ID_PRESENT, N, 0, 0 ^*/
    son_mro_notification_cause_et cause;
    /*^ M, 0, L, 1, 0 ^*/
    son_mro_ho_failure_level_et ho_failure_level;
    /*^ M, 0, B, 1, 2 ^*/
    son_u16 ho_failure_rate;
    /*^ M, 0, B, 1, 10000 ^*/
}son_oam_mro_optimization_notification_t; /*^ API, SON_OAM_MRO_OPTIMIZATION_NOTIFICATION ^*/

#define SON_OAM_AUTO_SWITCH_OFF_LOAD_LEVEL      0x01
#define SON_OAM_AUTO_SWITCH_OFF_UE_COUNT        0x02

/*! \  son_oam_es_autonomous_switch_off_config_req_t
 *  \brief Request containing the configuration for Autonomous Switch off for ES cells
 *         whose id is present in the request or global configuration for all
 *         configured cells at eNodeB, if the cell id list size is 0
 *  \param bitmask - indicates the criteria params to be considered for autonomous
 *                   cell switch off: load level or active UE count or both
 *                   (Threshold for active UE count is already received in Init Config Req)
 *  \param src_cgi_list_size - list size for the no. of cells for which configuration is
 *                   present (= 0 for global configuration)
 *  \param cell_id - Cell Ids of the cells on which configuration is to be applied
 *  \param load_level_threshold - Threshold value of load level to trigger cell switch off
 *  \param timer_duration - Timer value to wait before triggering Cell Switch Off 
 */
typedef struct _son_oam_es_autonomous_switch_off_config_req_t
{
    son_u32 bitmask; /*^ BITMASK ^*/
    son_u8 src_cgi_list_size;
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_MAX_CELL_SUPPORTED_AT_ENB ^*/
/*SPR_17664_end*/
    son_intra_rat_global_cell_id_t cell_id[SON_MAX_CELL_SUPPORTED_AT_ENB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    son_u8 load_level_threshold;
    /*^O, SON_OAM_AUTO_SWITCH_OFF_LOAD_LEVEL, H, 0, 100 ^*/
    /* Fix 875 Start */
    son_timer_duration_t    timer_duration;
    /*^ M, 0, L, 1, 0 ^*/
    /* Fix 875 End */
}son_oam_es_autonomous_switch_off_config_req_t; /*^ API,  SON_OAM_ES_AUTONOMOUS_SWITCH_OFF_CONFIG_REQ ^*/

/*! \  son_oam_es_autonomous_switch_off_config_resp_t
 *  \brief Response to ES Autonomous Cell Switch off Config Request.
 *  \param result                 - Result
 *  \param error_code             - error code
 *  \param cell_status_list_size - Number of elememts in the cell
 *                                 status list
 *  \param cell_status_list      - List containing the status of each
 *                                 cell requested for Autonomous switch off
 */
typedef struct _son_oam_es_autonomous_switch_off_config_resp_t
{
    son_return_et result;    /*^ M, 0, H, 0, 1 ^*/
    son_error_et error_code; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u16 cell_status_list_size; /*^ M, 0, H, 0, SON_MAX_CELL_SUPPORTED_AT_ENB^*/
/*SPR_17664_end*/
    son_cell_status_t cell_status_list[SON_MAX_CELL_SUPPORTED_AT_ENB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_es_autonomous_switch_off_config_resp_t;    /*^ API, SON_OAM_ES_AUTONOMOUS_SWITCH_OFF_CONFIG_RESP ^*/

/*! \  son_oam_chk_health_req_t
 *  \brief Request structure for Health Monitoring
 *  \param bitmask
 */
typedef struct _son_oam_chk_health_req_t
{
    son_u32             bitmask;    /*^ BITMASK ^*/
}son_oam_chk_health_req_t;   /*^ API,   SON_OAM_CHK_HEALTH_REQ ^*/

/*! \  son_oam_chk_health_resp_t
 *  \brief Response structure for Health Monitoring
 *  \param bitmask
 */
typedef struct _son_oam_chk_health_resp_t
{
    son_u32             bitmask;    /*^ BITMASK ^*/
}son_oam_chk_health_resp_t;   /*^ API,  SON_OAM_CHK_HEALTH_RESP ^*/

/*
 * SON_OAM_ANR_MEAS_CONFIG_OBJ_REMOVE_REQ
 */

/*! \  son_oam_anr_meas_config_obj_remove_req_t
 *  \brief Structure storing measurement object removal request message data
 *  \param src_cgi                      CGI of cell
 *  \param earfcn_remove_list_size      Number of E-UTRA frequencies to be removed
 *  \param earfcn_remove_list           List of EUTRAN frequencies to be removed
 *  \param uarfcn_remove_list_size      Number of UTRAN frequencies to be removed
 *  \param uarfcn_remove_list           List of UTRAN frequencies to be removed
 *  \param geran_remove_list_size       Number of GERAN frequencies to be removed
 *  \param geran_remove_list            List of GERAN frequencies to be removed
 */
typedef struct _son_oam_anr_meas_config_obj_remove_req_t
{
    son_intra_rat_global_cell_id_t      src_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_u8                              earfcn_remove_list_size;
    /* SPR-12875 Fix Starts */
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_MAX_CONFIG_OBJECT ^*/
/*SPR_17664_end*/
    /* SPR-12875 Fix Ends */
    son_u16                             earfcn_remove_list[SON_MAX_CONFIG_OBJECT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
/*SPR_17664_start*/
    son_u8                              uarfcn_fdd_remove_list_size;
/*SPR_17664_end*/
    /* SPR-12875 Fix Starts */
    /*^ M, 0, H, 0, SON_MAX_CONFIG_OBJECT ^*/
    /* SPR-12875 Fix Ends */
    son_u16                             uarfcn_fdd_remove_list[SON_MAX_CONFIG_OBJECT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
/*SPR_17664_start*/
    son_u8                              uarfcn_tdd_remove_list_size;
/*SPR_17664_end*/
    /* SPR-12875 Fix Starts */
    /*^ M, 0, H, 0, SON_MAX_CONFIG_OBJECT ^*/
    /* SPR-12875 Fix Ends */
    son_u16                             uarfcn_tdd_remove_list[SON_MAX_CONFIG_OBJECT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    son_u8                              geran_remove_list_size;
    /* SPR-12875 Fix Starts */
/*SPR_17664_start*/
    /*^ M, 0, H, 0, SON_MAX_CONFIG_OBJECT ^*/
/*SPR_17664_end*/
    /* SPR-12875 Fix Ends */
    son_geran_carrier_freqs_t           geran_remove_list[SON_MAX_CONFIG_OBJECT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    U8                                  cdma_remove_list_size;
    /* SPR-12875 Fix Starts */
/* SPR-18445 start */
    /*^ M, 0, H, 0, MAX_SON_MEAS_CONFIG_OBJECT ^*/
/* SPR-18445 stop */
    /* SPR-12875 Fix Ends */
    son_cdma_carrier_freq_info_t        cdma_remove_list[MAX_SON_MEAS_CONFIG_OBJECT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_anr_meas_config_obj_remove_req_t;
/*^ API, SON_OAM_ANR_MEAS_CONFIG_OBJ_REMOVE_REQ ^*/


/*
 * SON_OAM_ANR_MEAS_CONFIG_OBJ_REMOVE_RESP
 */

/*! \  son_oam_anr_meas_config_obj_remove_res_t
 *  \brief Structure storing Measurement object removal response message data
 *  \param src_cgi          CGI of cell
 *  \param result           Success/failure
 *  \param error_code       Cause (valid in case of FAILURE)
 */
typedef struct _son_oam_anr_meas_config_obj_remove_res_t
{
    son_intra_rat_global_cell_id_t      src_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_return_et                       result;
    /*^ M, 0, H, 0, 1 ^*/
    son_error_et                        error_code;
    /*^ M, 0, N, 0, 0 ^*/
}son_oam_anr_meas_config_obj_remove_res_t;
/*^ API, SON_OAM_ANR_MEAS_CONFIG_OBJ_REMOVE_RESP ^*/

/* RACH OPT CHANGES : STARTS */

/*
 * SON_OAM_RACH_OPT_ENABLE
 */

#define SON_RACH_OPT_ENABLE_SRV_CGI_PRESENT 0x01

/*! \  son_oam_rach_opt_enable_req_t
 *  \brief RACH Optimization Enable Request received from OAM
 *  \param bitmask
 *  \param srv_cgi   ECGI of cell on which RACH Optimization is to be enabled. 
 *                   In case no cell Id is present in the request then 
 *                   RACH Optimization has to be enabled on all the cells 
 *                   of eNodeB
 */
typedef struct _son_oam_rach_opt_enable_req_t
{
    son_u32                         bitmask;         /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t  srv_cgi;
    /*^ O, SON_RACH_OPT_ENABLE_SRV_CGI_PRESENT, N, 0, 0 ^*/
}son_oam_rach_opt_enable_req_t;    /*^ API, SON_OAM_RACH_OPT_ENABLE_REQ  ^*/

/*! \  son_oam_rach_opt_enable_resp_t
 *  \brief Response to RACH Optimization Enable Request
 *  \param result                 - Result
 *  \param error_code             - error code
 *  \param cell_status_list_size  - Number of elememts in the cell 
 *                                  status list
 *  \param cell_status_list       - List containing the status of each 
 *                                  cell whose enable request failed
 */
typedef struct _son_oam_rach_opt_enable_resp_t
{
    son_return_et        result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et         error_code; /*^ M, 0, N, 0, 0 ^*/

/*SPR_17664_start*/
    son_u16              cell_status_list_size; /*^ M, 0, H, 0, SON_MAX_CELL_SUPPORTED_AT_ENB ^*/

/*SPR_17664_end*/
    son_cell_status_t    cell_status_list[SON_MAX_CELL_SUPPORTED_AT_ENB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_rach_opt_enable_resp_t; /*^ API, SON_OAM_RACH_OPT_ENABLE_RESP ^*/

/*
 * SON_OAM_RACH_OPT_DISABLE
 */

#define SON_RACH_OPT_DISABLE_SRV_CGI_PRESENT 0x01

/*! \  son_oam_rach_opt_disable_req_t
 *  \brief RACH Optimization Disable Request Structure
 *  \param bitmask
 *  \param srv_cgi   ECGI of cell on which RACH Optimization is to be disabled. 
 *                   In case no cell Id is present in the request then 
 *                   RACH Optimization has to be disabled on all the cells 
 *                   of eNodeB
 */
typedef struct _son_oam_rach_opt_disable_req_t
{
    son_u32                            bitmask;    /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t     srv_cgi;
    /*^ O, SON_RACH_OPT_DISABLE_SRV_CGI_PRESENT, N, 0, 0 ^*/
}son_oam_rach_opt_disable_req_t;    /*^ API, SON_OAM_RACH_OPT_DISABLE_REQ  ^*/

/*! \  son_oam_rach_opt_disable_resp_t
 *  \brief Response to RACH Optimization Disable Request
 *  \param result                 - Result
 *  \param error_code             - error code
 *  \param cell_status_list_size  - Number of elememts in the cell 
 *                                  status list
 *  \param cell_status_list       - List containing the status of each 
 *                                  cell whose disable request failed
 */
typedef struct _son_oam_rach_opt_disable_resp_t
{
    son_return_et        result;     /*^ M, 0, H, 0, 1 ^*/
    son_error_et         error_code; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u16              cell_status_list_size; /*^ M, 0, H, 0, SON_MAX_CELL_SUPPORTED_AT_ENB ^*/
/*SPR_17664_end*/
    son_cell_status_t    cell_status_list[SON_MAX_CELL_SUPPORTED_AT_ENB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_rach_opt_disable_resp_t;    /*^ API, SON_OAM_RACH_OPT_DISABLE_RESP  ^*/

/*
 * SON_OAM_RACH_OPT_MODIFY_CONFIG_PARAMS
 */

#define SON_RACHOPT_REPORT_PERIODICITY_PRESENT                      0x001
#define SON_RACHOPT_MIN_UE_RACH_REPORT_COUNT_PRESENT                0x002
#define SON_RACHOPT_UE_RACH_ATTEMPTS_THRESHOLD_PRESENT              0x004
#define SON_RACHOPT_START_THRESHOLD_PRESENT                         0x008
#define SON_RACHOPT_STOP_THRESHOLD_PRESENT                          0x010
#define SON_RACHOPT_DEGRADE_STOP_OFFSET_PRESENT                     0x020
#define SON_RACHOPT_MAX_ITERATIONS_TO_REACH_STOP_THRESHOLD_PRESENT  0x040
#define SON_RACHOPT_CONTENTION_THRESHOLD_PRESENT                    0x080
#define SON_RACHOPT_POWER_RAMPUP_THRESHOLD_PRESENT                  0x100
#define SON_RACHOPT_MIN_CFRA_PREAMBLES_PRESENT                      0x200

/*! \  son_rach_opt_oam_config_param_t
 *  \brief Strucutre storing various configurable parameters for RACH optimization
 *  \param bitmask
 *  \param rachopt_report_periodicity                       For Distributed RACH-OPT Mode, this parameter determines the interval of getting the UE Reports and L2 RACH stat reports from RRM and also defines the periodicity of running the RACH Optimization Algorithm. In case of Centralized Mode, this defines the periodicity of providing the RACH reports to Centralized SON entity
 *  \param rachopt_min_ue_rach_report_count                 Minimum number of UE RACH sample requested from RRM during each RACH Report Period required for RACH optimization Algorithm. In case of Centralized mode, these samples will be sent to Centralized SON entity.
 *  \param rachopt_ue_rach_attempts_threshold               RACH Attempts Threshold above which a UE is considered to have RACH problem and is taken into consideration while analyzing RACH data for RACH parameter tuning
 *  \param rachopt_start_threshold                          Threshold value of % UEs reporting RACH attempts greater than .rachopt_ue_rach_attempts_threshold.. This condition triggers RACH optimization algorithm
 *  \param rachopt_stop_threshold                           Threshold value of % UEs reporting RACH attempts greater than .rachopt_ue_rach_attempts_threshold.. Achieving this condition indicates objective of RACH algo is met and it should stop now.
 *  \param rachopt_degrade_stop_offset                      Percentage offset at which RACHOPT algo stops and notifies to OAM about failure if current % of UEs reporting more than .rachopt_ue_rach_attempts_threshold.RACH attempts increases by .rachopt_degrade_stop_offset.
 *  \param rachopt_max_iterations_to_reach_stop_threshold   Parameter defining maximum iterations RACH optimization algo will attempt to reach the stop threshold. If stop threshold is not achieved in these iterations, failure is notified to OAM.
 *  \param rachopt_contention_threshold                     Parameter defining % of UEs that report RACH attempts more than .rachopt_ue_rach_attempts_threshold. as well as well as contention_detected flag as true. This indicates excessive contention in network for RACH procedure.
 *  \param rachopt_power_rampup_threshold                   Parameter defining % of Cell Centre RACH samples indicating RACH attempts more than .rachopt_ue_rach_attempts_threshold. but no contention. This implies problem lies in intial received target power setting.
 *  \param rachopt_min_cfra_preambles                       Parameter defines the limit for CF preambles below which RACH-OPT algo shall not reduce the CF preambles.
 */
typedef struct _son_rach_opt_oam_config_param_t
{
    son_u32                     bitmask; /*^ BITMASK ^*/
    son_u16                     rachopt_report_periodicity; 
    /*^ O, SON_RACHOPT_REPORT_PERIODICITY_PRESENT, B, 1, 600 ^*/
    son_u16                     rachopt_min_ue_rach_report_count;
    /*^ O, SON_RACHOPT_MIN_UE_RACH_REPORT_COUNT_PRESENT, B, 10, 1000 ^*/
    son_u8                      rachopt_ue_rach_attempts_threshold;
    /*^ O, SON_RACHOPT_UE_RACH_ATTEMPTS_THRESHOLD_PRESENT, B, 2, 10 ^*/
    son_u16                     rachopt_start_threshold;
    /*^ O, SON_RACHOPT_START_THRESHOLD_PRESENT, B, 1, 1000 ^*/
    son_u16                     rachopt_stop_threshold;
    /*^ O, SON_RACHOPT_STOP_THRESHOLD_PRESENT, B, 1, 1000 ^*/
    son_u16                     rachopt_degrade_stop_offset;
    /*^ O, SON_RACHOPT_DEGRADE_STOP_OFFSET_PRESENT, B, 1, 1000 ^*/
    son_u8                      rachopt_max_iterations_to_reach_stop_threshold;
    /*^ O, SON_RACHOPT_MAX_ITERATIONS_TO_REACH_STOP_THRESHOLD_PRESENT, B, 1, 10 ^*/
    son_u8                      rachopt_contention_threshold;
    /*^ O, SON_RACHOPT_CONTENTION_THRESHOLD_PRESENT, B, 1, 100 ^*/
    son_u8                      rachopt_power_rampup_threshold;
    /*^ O, SON_RACHOPT_POWER_RAMPUP_THRESHOLD_PRESENT, B, 1, 100 ^*/
    son_u8                      rachopt_min_cfra_preambles;
    /*^ O, SON_RACHOPT_MIN_CFRA_PREAMBLES_PRESENT, B, 4, 60 ^*/
}son_rach_opt_oam_config_param_t;

/*! \  son_oam_rach_opt_modify_config_params_req_t
 *  \brief RACH Optimization Modify configuration parameters Request Structure
 *  \param bitmask
 *  \param srv_cgi              ECGI of cell for which configuratioin parameters needs to be modified.
 *  \param rach_config_params   Strucutre storing various configurable parameters for RACH optimization
 */
typedef struct _son_oam_rach_opt_modify_config_params_req_t
{
    son_u32                         bitmask; 
    /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t  srv_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_rach_opt_oam_config_param_t rach_config_params;
    /*^ M, 0, N, 0, 0 ^*/
}son_oam_rach_opt_modify_config_params_req_t; 
/*^ API, SON_OAM_RACH_OPT_MODIFY_CONFIG_PARAMS_REQ  ^*/

/*! \  son_oam_rach_opt_modify_config_params_resp_t
 *  \brief Response to RACH Optimization Modify configuration parameters Request
 *  \param bitmask
 *  \param srv_cgi              ECGI of cell for which configuratioin parameters are modified.
 *  \param result               SUCCESS/FAILURE
 *  \param error_code           Error cause (valid in case of Failure)
 *  \param failed_param_bitmask Bitmask to indicate which of the parameter updation failed
 */
typedef struct _son_oam_rach_opt_modify_config_params_resp_t
{
    son_intra_rat_global_cell_id_t  srv_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_return_et                   result;    /*^ M, 0, H, 0, 2 ^*/
    son_error_et                    error_code; /*^ M, 0, N, 0, 0 ^*/
    son_u32                         failed_param_bitmask;
    /*^ M, 0, N, 0, 0 ^*/
}son_oam_rach_opt_modify_config_params_resp_t; 
/*^ API, SON_OAM_RACH_OPT_MODIFY_CONFIG_PARAMS_RESP ^*/

/*
 * SON_OAM_RACH_OPT_MODIFY_RACH_OPTMODE
 */
/*! \  son_oam_rach_opt_modify_rach_optmode_req_t
 *  \brief Structure storing data of SON_OAM_RACH_OPT_MODIFY_RACH_OPTMODE_REQ to modify the RACH-OPT functionality from Centralized to Distributed and vice versa. 
 *  \param rachopt_mode Determines if the RACH-OPT shall be functioning in Centralized Mode or Distributed Mode. Default Mode: Distributed.
 */
typedef struct _son_oam_rach_opt_modify_rach_optmode_req_t
{
    son_mode_et                 rachopt_mode;
    /*^ M, 0, H, 0, 1 ^*/
}son_oam_rach_opt_modify_rach_optmode_req_t;
/*^ API, SON_OAM_RACH_OPT_MODIFY_RACH_OPTMODE_REQ ^*/

/*! \  son_oam_rach_opt_modify_rach_optmode_resp_t
 *  \brief Response to SON_OAM_RACH_OPT_MODIFY_RACH_OPTMODE_REQ message indicating whether it accepts modification of the RACH-OPT functionality mode or not.
 *  \param result           SUCCESS/FAILURE
 *  \param error_code       Error cause (valid in case of Failure)
 */
typedef struct _son_oam_rach_opt_modify_rach_optmode_resp_t
{
    son_return_et                   result;    /*^ M, 0, H, 0, 2 ^*/
    son_error_et                    error_code; /*^ M, 0, N, 0, 0 ^*/
}son_oam_rach_opt_modify_rach_optmode_resp_t;
/*^ API, SON_OAM_RACH_OPT_MODIFY_RACH_OPTMODE_RESP ^*/

/*
 * SON_OAM_RACH_OPT_FAILURE_NOTIFICATION
 */

/*! \  son_oam_rach_opt_failure_notification_t
 *  \brief Strucutre storing data of SON_OAM_RACH_OPT_FAILURE_NOTIFICATION sent to OAM when SON RACH-OPT is not able to optimize the RACH configuration parameters due to any reason. 
 *  \param bitmask      
 *  \param srv_cgi              ECGI of cell for which RACH optimization failed
 *  \param cause                Indicates baseline for computation of transmit power for random access transmission parameters.
 *  \param rach_failure_level   Number of non-dedicated random access preambles (Contention based)
 *  \param identified_opt_type  Number of non-dedicated random access preamble group A
 */
typedef struct _son_oam_rach_opt_failure_notification_t
{
    son_u32                                         bitmask; /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t                  srv_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_rach_opt_failure_cause_et                   cause;
    /*^ M, 0, B, 1, 255 ^*/
    son_rach_opt_failure_level_et                   rach_failure_level;
    /*^ M, 0, H, 0, 1 ^*/
    son_rach_opt_identified_optimization_type_et    identified_opt_type;
    /*^ M, 0, N, 0, 0 ^*/    
}son_oam_rach_opt_failure_notification_t; 
/*^ API, SON_OAM_RACH_OPT_FAILURE_NOTIFICATION ^*/

/*
   SON_OAM_RACH_OPT_PARAMETER_UPDATE_REQ
 */

#define SON_UPDATE_PREAMBLE_INIT_RCVD_TGT_PWR_PRESENT   0x01
#define SON_UPDATE_NUM_OF_RA_PREAMBLES_PRESENT          0x02
#define SON_UPDATE_SIZE_OF_RA_GROUP_A_PRESENT           0x04
#define SON_UPDATE_ROOT_SEQ_IDX_PRESENT                 0x08
#define SON_UPDATE_PRACH_FREQ_OFFSET_PRESENT            0x10
#define SON_UPDATE_PRACH_CONFIG_IDX_PRESENT             0x20

 /*! \ son_oam_rach_opt_update_req_param_t
 *  \brief Structure stroing the various parameters that RACH OPT can request to update to OAM.
 *  \param bitmask 
 *  \param preamble_initial_rcvd_target_power   Indicates baseline for computation of transmit power for random access transmission parameters.
 *  \param number_of_ra_preambles               Number of non dedicated random access preambles (Contention based)
 *  \param size_of_ra_group_a                   Number of non dedicated random access preamble group A
 *  \param root_sequence_index                  Logical root sequence index used to determine 64 physical RACH preamble sequences available in the cell
 *  \param prach_frequency_offset               The first physical resource block available for PRACH expressed as a physical resource block number
 *  \param prach_configuration_index            Provides index into the table defining PRACH resources within the frame. Only Applicable in TDD mode.
 */
typedef struct _son_oam_rach_opt_update_req_param_t
{
    son_u32                                 bitmask; /*^ BITMASK ^*/
    son_preamble_init_rec_target_pow_et     preamble_initial_rcvd_target_power; 
    /*^ O, SON_UPDATE_PREAMBLE_INIT_RCVD_TGT_PWR_PRESENT, H, 0, 15 ^*/
    son_oam_ra_preamble_count_et            number_of_ra_preambles;
    /*^ O, SON_UPDATE_NUM_OF_RA_PREAMBLES_PRESENT, H, 0, 15 ^*/
    son_oam_ra_preambles_group_a_size_et    size_of_ra_group_a;
    /*^ O, SON_UPDATE_SIZE_OF_RA_GROUP_A_PRESENT, H, 0, 14 ^*/
    son_u16                                 root_sequence_index;            
    /*^ O, SON_UPDATE_ROOT_SEQ_IDX_PRESENT, H, 0, 837 ^*/
    son_u8                                  prach_frequency_offset;         
    /*^ O, SON_UPDATE_PRACH_FREQ_OFFSET_PRESENT, H, 0, 94 ^*/
    son_u8                                  prach_configuration_index;      
    /*^ O, SON_UPDATE_PRACH_CONFIG_IDX_PRESENT, H, 0, 63 ^*/
}son_oam_rach_opt_update_req_param_t;

/*! \  son_oam_rach_opt_parameter_update_req_t
 *  \brief Structure stroing the data of SON_OAM_RACH_OPT_PARAMETER_UPDATE_REQ sent to OAM to suggest the modified value for one or more RACH configuration parameters.
 *  \param bitmask 
 *  \param srv_cgi      Serving cell CGI
 *  \param update_param Paramters to be updated
 */
typedef struct _son_oam_rach_opt_parameter_update_req_t
{
    son_u32                                 bitmask; /*^ BITMASK ^*/
    son_intra_rat_global_cell_id_t          srv_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_oam_rach_opt_update_req_param_t     update_param;
    /*^ M, 0, N, 0, 0 ^*/
}son_oam_rach_opt_parameter_update_req_t;
/*^ API, SON_OAM_RACH_OPT_PARAMETER_UPDATE_REQ ^*/

/*
   SON_OAM_RACH_OPT_UE_INFO_REPORT
 */ 

/*! \ son_strong_cell_info_t
 *  \brief Structure storing the neighbor cell data received in UE info report
 *  \param pci      PCI of the neighbor cell
 *  \param rsrp     RSRP of the neighbor cell
 */
typedef struct _son_strong_cell_info_t
{
    son_u16     pci;    /*^ M, 0, H, 0, 503 ^*/
    son_u8      rsrp;   /*^ M, 0, H, 0, 128 ^*/
}son_strong_cell_info_t;

/*! \ son_ue_prach_data_info_t 
 *  \brief Strucuture storing the PRACH data of a particular UE information sample 
 *  \param cause                    Cause for RACH procedure
 *  \param number_of_preambles_sent Number of preambles sent by this sample
 *  \param contention_detected      Flag indicating if contention was detected
 *  \param ue_location              Location of UE (center or edge)
 *  \param strong_cell_info_count   Number of strong cells reported by UE
 *  \param strong_cell_info         Data of reported strong cells 
 */
typedef struct _son_ue_prach_data_info_t
{
    son_prach_cause_et      cause;                  /*^ M, 0, B, 1, 3 ^*/
    son_u16                 number_of_preambles_sent;  /*^ M, 0, N, 0, 0 ^*/
    son_bool_et             contention_detected;     /*^ M, 0, H, 0, 1 ^*/
    son_ue_location_et      ue_location;            /*^ M, 0, H, 0, 1 ^*/
/*SPR_17664_start*/
    son_u8                  strong_cell_info_count; /*^ M, 0, H, 0, SON_STRONG_CELL_INFO_COUNT ^*/
/*SPR_17664_end*/
    son_strong_cell_info_t  strong_cell_info[SON_STRONG_CELL_INFO_COUNT];   
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_ue_prach_data_info_t;

/*! \ son_oam_rach_opt_ue_info_report_t
 *  \brief Structure storing data of SON_OAM_RACH_OPT_UE_INFO_REPORT send to OAM when RACH optimization algo is supposed to be run at some centralized SON entity and not at eNB SON. This API provides the periodic sample UE Information reports to Centralized entity.
 *  \param srv_cgi      Serving cell CGI
 *  \param count        Number of rach_data elements
 *  \param rach_data    Strucutre storing RACH data
 */
typedef struct _son_oam_rach_opt_ue_info_report_t
{
    son_intra_rat_global_cell_id_t  srv_cgi;
    /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
    son_u16                         count;     /*^ M, 0, H, 0, SON_MAX_NUMBER_OF_PRACH_SAMPLES ^*/
/*SPR_17664_end*/
    son_ue_prach_data_info_t        rach_data[SON_MAX_NUMBER_OF_PRACH_SAMPLES]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_rach_opt_ue_info_report_t;
/*^ API, SON_OAM_RACH_OPT_UE_INFO_REPORT ^*/

/*
   SON_OAM_RACH_OPT_L2_PERF_REPORT
 */ 
/*! \ son_oam_rach_opt_l2_perf_report_t
 *  \brief Structure storing data of SON_OAM_RACH_OPT_L2_PERF_REPORT send to OAM when RACH optimization algo is supposed to be run at some centralized SON entity and not at eNB SON. This API provides the periodic RACH Information generated at L2 to Centralized entity.
 *  \param src_cgi                                  Serving cell CGI
 *  \param num_of_l2_perf_accumulated_reports  Number of L2 Performance Reports accumulated
 *  \param total_rcvd_msg3_ra_preambles_cbra   Number of successfully detected RA Msg3 for CBRA
 *  \param total_rcvd_msg3_ra_preambles_cfra   Number of successfully completed CFRA procedures. Measured when first UL-SCH message is received after detection of CFRA preamble
 *  \param total_failed_ra_preambles_msg2_cbra Number of CBRA preambles for which no random access response (RA Msg2) was sent
 *  \param total_failed_ra_preambles_msg2_cfra Number of CFRA preambles for which no random access response (RA Msg2) was sent.
 *  \param total_allocated_ra_preambles      Number of allocated CFRA preambles
 *  \param total_succ_ra_ta                  Number of detected successful random access with a TA value  in the range 0..9%, 10..19%, 20..29% and so on
 *  \param total_unassigned_false_cfra                 The number of detected Contention Free Random Access preambles that are not allocated to any UE
 *  \param total_unassigned_sum_cfra                   The total number of unassigned Contention Free Random Access preambles at each PRACH occasion during the reporting period
 */
typedef struct _son_oam_rach_opt_l2_perf_report_t
{
    son_intra_rat_global_cell_id_t          src_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    son_u16 num_of_l2_perf_accumulated_reports; 
    /*^ M, 0, N, 0, 0 ^*/
    son_u64 total_rcvd_msg3_ra_preambles_cbra;
    /*^ M, 0, N, 0, 0 ^*/
    son_u64 total_rcvd_msg3_ra_preambles_cfra;
    /*^ M, 0, N, 0, 0 ^*/
    son_u64 total_failed_ra_preambles_msg2_cbra;
    /*^ M, 0, N, 0, 0 ^*/
    son_u64 total_failed_ra_preambles_msg2_cfra;
    /*^ M, 0, N, 0, 0 ^*/
    son_u64 total_allocated_ra_preambles;
    /*^ M, 0, N, 0, 0 ^*/
    son_u64 total_succ_ra_ta[MAX_TA_RANGE];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
    son_u64 total_unassigned_false_cfra;
    /*^ M, 0, N, 0, 0 ^*/
    son_u64 total_unassigned_sum_cfra;
    /*^ M, 0, N, 0, 0 ^*/
    /* SPR-18438 START */
    son_u64 num_random_access_preambles_group_a;
    /*^ M, 0, N, 0, 0 ^*/
    son_u64 num_random_access_preambles_group_b;
    /*^ M, 0, N, 0, 0 ^*/
    /* SPR-18438 END */
}son_oam_rach_opt_l2_perf_report_t;
/*^ API, SON_OAM_RACH_OPT_L2_PERF_REPORT ^*/

/* RACH OPT CHANGES : ENDS */

/* Rel 3.0 Cell delete support changes Start */

/*
 *  SON_OAM_CELL_DELETE_REQ
 */

/*! \ son_oam_cell_delete_req_t
 *  \brief   Delete request for the list of configured cells on SON from OAM
 *  \param   bitmask                - Bitmask
 *  \param   cell_delete_list_size  - No. of cells to be deleted on eNB
 *  \param   cell_delete_list       - List of the serving cells to be deleted
 */
typedef struct _son_oam_cell_delete_req_t
{
    son_u32                         bitmask; /*^BITMASK^*/
    son_u8                          cell_delete_list_size;
/* SPR-18445 start */
    /*^ M, 0, B, 1, SON_MAX_CELL_SUPPORTED_AT_ENB ^*/
/* SPR-18445 stop */
    son_intra_rat_global_cell_id_t  cell_delete_list[SON_MAX_CELL_SUPPORTED_AT_ENB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_cell_delete_req_t;
/*^ API, SON_OAM_CELL_DELETE_REQ ^*/

/*
 *  SON_OAM_CELL_DELETE_RESP
 */

/*! \  son_cell_delete_status_t
 *  \brief Structure for storing cell delete status
 *  \param cgi    - CGI of cell
 *  \param result - Result of cell delete request
 */
typedef struct _son_cell_delete_status_t
{
    son_intra_rat_global_cell_id_t        cgi; /*^ M, 0, N, 0, 0 ^*/
    son_return_et                         result; /*^ M, 0, N, 0, 0 ^*/
}son_cell_delete_status_t;

/*! \ son_oam_cell_delete_resp_t
 *  \brief   Cell Delete Response from SON to OAM
 *  \param   bitmask                - Bitmask
 *  \param   cell_status_list_size  - Number of cells which could
 *                                    not be deleted.
 *  \param   cell_status_list       - List containing the status of each
 *                                    cell requested for Deletion
 */
typedef struct _son_oam_cell_delete_resp_t
{
    son_u32                     bitmask; /*^BITMASK^*/
    son_u8                      cell_delete_status_list_size; 
/* SPR-18445 start */
    /*^M, 0, H, 0, SON_MAX_CELL_SUPPORTED_AT_ENB^*/
/* SPR-18445 stop */
    son_cell_delete_status_t    cell_delete_status_list[SON_MAX_CELL_SUPPORTED_AT_ENB];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_oam_cell_delete_resp_t;
/*^ API, SON_OAM_CELL_DELETE_RESP ^*/

/* Rel 3.0 Cell delete support changes Stop */
/*spr 22474 start*/
/* This structure provides the POOL stats */
typedef struct _son_PoolStatsT {
    /* the size of the buffer allocated in this pool */
    U32 bufSize; /*^ M, 0, N, 0, 0 ^*/
    /* the Total number of Buffers in this pool */
    U32 numOfBuf; /*^ M, 0, N, 0, 0 ^*/
    /* the Total number of Buffers allocated from this pool */
    U32 numOfAllocBuf; /*^ M, 0, N, 0, 0 ^*/
    /* the Peak of Total number of Buffers allocated from this pool during the system is running */
    U32 numOfPeakBuf; /*^ M, 0, N, 0, 0 ^*/
}son_PoolStats;

/* This structure provides the MEM POOL and MSG POOL stats */
typedef struct _son_MemPoolStatsT {
    /* This array is terminated when bufSize is 0 for an entry, other entries are not valid */
    son_PoolStats stats[NVARPOOL]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}son_MemPoolStats;

/* SON_OAM_GET_DEBUG_INFO_REQ */
/*! \typedef son_oam_get_debug_info_req_t 
 *  \brief   This API is sent from OAM to SON
 *           to request for debug stats
 */
typedef struct _son_oam_get_debug_info_req_t
{
   U16                   debug_type;/*^ M, 0, N, 0, 0 ^*/
   /*son_oam_debug_info_req_et*/
}son_oam_get_debug_info_req_t; /*^ API, SON_OAM_GET_DEBUG_INFO_REQ ^*/

/* SON_OAM_GET_DEBUG_INFO_RESP */
/*! \typedef son_oam_get_debug_info_resp_t
 *  \brief   This API is sent from SON to OAM
 *           in the response of son_oam_get_debug_info_req_t 
 */
#define SON_OAM_DEBUG_GET_MEM_POOL_PRESENT 0x01
#define SON_OAM_GET_DEBUG_INFO_FAILURE_CAUSE_PRESENT 0x02
typedef struct _son_oam_get_debug_info_resp_t
{
    son_u32  bitmask; /*^ BITMASK ^*/
    son_s32  response;/*^ M, 0, N, 0, 0 ^*/
    son_s32  fail_cause;/*^ O, SON_OAM_GET_DEBUG_INFO_FAILURE_CAUSE_PRESENT, N, 0, 0 ^*/
    /* son_oam_debug_info_fail_cause_et */
    son_MemPoolStats csplMemPoolStats;/*^ O, SON_OAM_DEBUG_GET_MEM_POOL_PRESENT, N, 0, 0 ^*/
}son_oam_get_debug_info_resp_t;/*^ API, SON_OAM_GET_DEBUG_INFO_RESP ^*/

/*spr 22474 end*/
#endif /* __SON_OAM_INTF__ */
