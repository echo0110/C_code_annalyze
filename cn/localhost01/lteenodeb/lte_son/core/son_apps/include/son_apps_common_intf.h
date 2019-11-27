/*! \file son_apps_common_intf.h
 *  \brief This file contains the definitions common for
 *         SONApps and its features
 *  \date May 15, 2012
 *  \author Sonal Jain
 */

/*!
 * \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_APPS_COMMON_INTF_H_
#define _SON_APPS_COMMON_INTF_H_

/*! \headerfile son_apps_intf.h <>
 */
#include <son_apps_intf.h>

/*! \def SONAPPS_MEAS_PERIOD
 *  \brief This macro defines the measurement period of network
 *         scan which is initiated for carrier frequency selection
 *         and PCI selection
 */
#define SONAPPS_MEAS_PERIOD 10

/*! \def SONAPPS_RETRY_COUNT
 *  \brief This macro defines the retry count for network
 *         scan which is initiated for carrier frequency selection
 *         and PCI selection
 */
#define SONAPPS_RETRY_COUNT 8

/*! \def SONAPPS_INVALID_VALUE
 *  \brief This macro defines an invalid value
 */
#define SONAPPS_INVALID_VALUE 65535

/*! \def SONAPPS_MAX_NO_VALID_BAND_ID
 *  \brief This macro stores the max no. of valid band Ids
 */
#define SONAPPS_MAX_NO_VALID_BAND_ID 35

/*! \def SONAPPS_MAX_NO_MEAS_BW
 *  \brief This macro stores the max no. of measurement bandwidth
 */
#define SONAPPS_MAX_NO_MEAS_BW       6

/*! \enum sonapps_features_et
 *  \brief An enum that specifies the SONApps features
 */
typedef enum
{
    SONAPPS_PCI = 0,
    SONAPPS_ACF_OAM,
    SONAPPS_ACF_APPS,
    SONAPPS_MAX_NO_FEATURES
}sonapps_features_et;

/*! \enum sonapps_acf_service_users_et
 *  \brief An enum that specifies the users using ACF functionality
 */
typedef enum
{
    ACF_SELECTION_REQ = 0,
    ACF_AND_POWER_REQ,
    SONAPPS_ACF_MAX_USERS
}sonapps_acf_service_users_et;

/*! \enum sonapps_pci_earfcn_val_et
 *  \brief An enum that specifies possible values for number of EARFCNs
 *         to be scanned by PCI feature
 */
typedef enum
{
    SONAPPS_NO_EARFCN_1 = 1,
    SONAPPS_NO_EARFCN_6 = 6,
}sonapps_pci_earfcn_val_et;

/*! \  sonapps_band_earfcn_map_table_t
 *  \brief Structure to store the Band_Id - Earfcn(Min, Max value) mapping
 *  \param  band_id           Band Id
 *  \param  min_earfcn_val    Min Earfcn value
 *  \param  max_earfcn_val    Max Earfcn value
 */
typedef struct _sonapps_band_earfcn_map_table_t
{
    son_u8 band_id;
    son_u16 min_earfcn_val;
    son_u16 max_earfcn_val;
}sonapps_band_earfcn_map_table_t;

/*! \enum sonapps_pci_status_et
 *  \brief An enum that specifies the enable/disable state of PCI feature
 */
typedef enum
{
    SONAPPS_PCI_DISABLED = 0,
    SONAPPS_PCI_ENABLED,
    SONAPPS_PCI_DISABLE_IN_PROGRESS,
    SONAPPS_PCI_ENABLE_IN_PROGRESS,
    SONAPPS_PCI_SEL_RESEL_IN_PROGRESS,
    SONAPPS_PCI_NUM_OF_STATES
}sonapps_pci_status_et;

/*! \enum son_apps_state_et
 *  \brief An enum that specifies the SONApps FSM states.
 */
typedef enum
{
    SONAPP_STATE_INIT,
    SONAPP_STATE_ACTIVE,
    SONAPP_STATE_SHUTDOWN,
    SONAPP_STATE_UNDEFINED = 0xFF
}son_apps_state_et;

/*! \  sonapps_tx_pwr_params_t
 *  \brief Structure to store dl tx power params received from oam
 *  \param p_tx_upp   Maximum downlink  referenceSignalPower (dBm).
 *  \param p_tx_low   Minimum downlink  referenceSignalPower (dBm).
 *  \param p_ue_max   Maximum downlink  referenceSignalPower (dBm). Default 23dBm.
 *  \param p_ue_min   Minimum downlink  referenceSignalPower (dBm). Default -40dBm
 */
typedef struct sonapps_tx_pwr_params_t
{
    son_s8 p_tx_upp;
    son_s8 p_tx_low;
    son_s8 p_ue_max;
    son_s8 p_ue_min;
}sonapps_tx_pwr_params_t;

/*! \  son_apps_gb_context_t
 *  \brief Structure to store the global context of SONAPPS
 *  \param g_sonapps_state SONApps FSM state
 *  \param apps_log_enable Logging status of SONApps NW SCAN feature
 *  \param pci_log_enable  Logging status of PCI feature
 *  \param acf_log_enable  Logging status of ACF feature
 *  \param meas_bandwidth  Measurement bandwidth for scan to select PCI/ACF
 *  \param transaction_id  Store transaction id of PCI/ACF selection request
 *  \param pci_fsm_state   PCI FSM state
 *  \param pci_list_size   No. of PCIs received from OAM in PCI
 *                         Selection/Reselection request message
 *  \param pci_list        List of PCIs received from OAM in PCI
 *                         Selection/Reselection request message
 *  \param earfcn          EARFCN identifier
 *  \param acf_selection_progress_status Progress status of ACF selection
 *                                      request
 *  \param tx_power_params_from_oam  Required for dl tx power params received from oam
 *  \param log_level                    Apps module log level
 */
typedef struct _son_apps_gb_context_t
{
    son_apps_state_et g_sonapps_state;
    son_oam_log_on_off_et apps_log_enable;
    son_oam_log_on_off_et pci_log_enable;
    son_oam_log_on_off_et acf_log_enable;
    son_u8 meas_bandwidth;
    son_u16 transaction_id[SONAPPS_MAX_NO_FEATURES];
    sonapps_pci_status_et pci_fsm_state;
    son_u16 pci_selection_earfcn;
    son_u16 pci_list_size;
    son_u16                  *pci_list;
    son_u8 num_nw_scan_req_sent_for_pci;
    son_u8 num_nw_scan_rsp_rcvd_for_pci;
    son_u8 meas_bw_list_size;
    son_u8                   *meas_bw_list;
    son_bool_et in_use_pci_list[SON_MAX_PCI];
    son_u32 earfcn[SONAPPS_ACF_MAX_USERS];
    son_bool_et acf_selection_progress_status[SONAPPS_ACF_MAX_USERS];
    sonapps_tx_pwr_params_t tx_power_params_from_oam;
    sonapps_icic_config_params_t icic_params;
    /* SPR-13251 Fix Starts */
    son_u8 log_level;
    /* SPR-13251 Fix Ends */

    /*SPR-12944-Start*/
    son_bool_et nmm_support_present;
    /*SPR-12944-End*/
}son_apps_gb_context_t;

/*! \fn son_error_et sonapps_prepare_and_send_nw_scan_req(
 *      son_apps_gb_context_t   *p_sonapps_gb_context,
 *      son_void_t              *p_msg,
 *      son_feature_id_et       feature_id,
 *      STACKAPIHDR             *p_cspl_hdr
 * )
 *  \brief This function is used to prepare network scan request
 *         and send SONNMM_START_SCAN_REQ message to NMM.
 *  \param p_sonapps_gb_context - Pointer to global context structure
 *  \param p_msg                - Pointer to the message
 *  \param feature_id           - Feature id
 *  \param p_cspl_hdr           - Pointer to CSPL header
 */
son_error_et
sonapps_prepare_and_send_nw_scan_req
(
    son_apps_gb_context_t   *p_sonapps_gb_context,
    son_void_t              *p_msg,
    son_feature_id_et feature_id,
    STACKAPIHDR             *p_cspl_hdr
);

/*! \fn son_void_t sonapps_print_global_context(
 *      son_apps_gb_context_t   *p_sonapps_gb_context,
 *      son_bool_et             log_enable_disable
 *  )
 *  \brief This function prints the SONAPPS global context contents
 *  \param p_sonapps_gb_context - Pointer to global context structure
 *  \param log_enable_disable   - Logging status(On/Off) of the calling feature
 */
son_void_t
sonapps_print_global_context
(
    son_apps_gb_context_t   *p_sonapps_gb_context,
    son_oam_log_on_off_et log_enable_disable
);

/*! \fn son_u8 sonapps_find_band_id_for_earfcn(
 *      son_u32     earfcn,
 *     son_oam_log_on_off_et log_enable_disable
 *  )
 *  \brief This function is used to find the band id corresponding to the earfcn
 *  \param earfcn - EARFCN for which the Band Id is to be found
 *  \param log_enable_disable   - Logging status(On/Off) of the calling feature
 */
son_u8
sonapps_find_band_id_for_earfcn
(
    son_u32 earfcn,
    son_oam_log_on_off_et log_enable_disable
);

#endif  /*_SON_APPS_COMMON_INTF_H_ */
