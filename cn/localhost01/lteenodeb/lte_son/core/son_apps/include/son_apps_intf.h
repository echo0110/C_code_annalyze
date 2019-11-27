/*! \file son_apps_intf.h
 *  \brief This file contains the structure definitions for the
 *         messages exposed by SON APPS to other internal modules.
 *  \date February 20, 2012
 *  \author Shilpi Jain
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_APPS_INTF_H_
#define _SON_APPS_INTF_H_

/*! \headerfile son_types.h <>
 */
#include <son_types.h>

/*! \headerfile son_utils.h <>
 */
#include <son_utils.h>

/*! \  son_nmm_periodic_nw_scan_req_t from sonapps_periodic_nw_scan_req_t */
typedef son_nmm_periodic_nw_scan_req_t sonapps_periodic_nw_scan_req_t;

/*! \  struct sonapps_nw_scan_req_t
 *  \brief  Structure for Network scan request
 *  \param eutran_scan_req     Eutran scan Req
 *  \param utran_scan_req      Utran scan Req
 */
typedef struct _sonapps_nw_scan_req_t
{
    son_nmm_start_scan_req_t eutran_scan_req;
    son_nmm_start_scan_req_t utran_scan_req;
}sonapps_nw_scan_req_t;

/*! \  SON_LIST earfcn_scan_res_list_t
 *  \brief SCAN Response linked lists definitions using CSPL YLIST
 *   It contains nodes storing information for different earfcn values
 */
typedef SON_LIST earfcn_scan_res_list_t;

/*! \  SON_LIST cell_info_list_t
 *  \brief Cell Information linked list definitions using CSPL YLIST
 */
typedef SON_LIST cell_info_list_t;

/*! \  struct earfcn_data_t
 *  \brief Struture containing earfcn specific information determined by NMM Scan
 *  \param earfcn   EARFCN value
 *  \param rssi                 RSSI value (conversion to be applied.
 *  \param p_cell_info_list     Pointer to the cell information list
 */
typedef struct _earfcn_data_t
{
    son_u32 earfcn;
    son_u8 rssi;
    cell_info_list_t    *p_cell_info_list;
}earfcn_data_t;

/*! \  struct earfcn_scan_res_data_t
 *  \brief linked list nodes data definition containing information
 *       specific to one earfcn
 *  \param      earfcn_node     YLNODE structure storing the next and
 *                              previous pointer to the list nodes
 *  \param      earfcn_data     Data stored in the list nodes
 */
typedef struct _earfcn_scan_res_data_t
{
    SON_LIST_NODE earfcn_node;
    earfcn_data_t earfcn_data;
}earfcn_scan_res_data_t;

/*! \  struct cell_info_t
 *  \brief Structure containing cell specific information determined by NMM Scan
 *  \param bitmask                              Bitmask indicate if CSG Identity is
 *                                              present or not
 *  \param cell_Id[SON_CELL_ID_OCTET_SIZE]      Cell Identity read from SIB-1
 *  \param cell_barred                          Cell Barred
 *  \param csg_indication                       CSG indication
 *  \param csg_identity[SON_CSG_ID_OCTET_SIZE]  CSG Identity from SIB-1
 *  \param num_plmn_id                          Number of elements in Array of PLMN Id
 *  \param plmn_identity_info[SON_MAX_NUM_PLMN] Array of PLMN Identity info
 *  \param tac[SON_TAC_OCTET_SIZE]              array of tac values
 *  \param rsTxPower                            Calculated as: meas_bw*RSRP/RSSI
 *  \param dl_sys_bw                            Num of DL RBs(6,...,100)
 *  \param ul_sys_bw                            Num of UL RBs
 *  \param pci_id                               Physical cell identity
 *  \param rsrq                                 RSRQ value
 *  \param rsrp                                 RSRP value. Conversion to be applied.
 *                                               Actual RSRP = rsrp -144
 */
typedef struct _cell_info_t
{
    son_u16 bitmask;
    son_u8 cell_Id[SON_CELL_ID_OCTET_SIZE];
    son_u8 cell_barred;
    son_u8 csg_indication;
    son_u8 csg_identity[SON_CSG_ID_OCTET_SIZE];
    son_u8 num_plmn_id;
    son_plmn_identity_info_t plmn_identity_info[SON_MAX_NUM_PLMN];
    son_u8 tac[SON_TAC_OCTET_SIZE];
    /* SPR 13018 changes start */
    son_s8 rsTxPower;
    /* SPR 13018 changes start */
    son_u8 dl_sys_bw;
    son_u8 ul_sys_bw;
    son_u16 pci_id;
    son_u8 rsrq;
    son_u8 rsrp;
}cell_info_t;

/*! \  struct cell_info_data_t
 *  \brief linked list nodes data definition containing info about cells
 *  \param      cell_node       YLNODE structure storing the next and
 *                              previous pointer to the list nodes
 *  \param      cell_info       Data stored in the list nodes
 */
typedef struct _cell_info_data_t
{
    SON_LIST_NODE cell_node;
    cell_info_t cell_info;
}cell_info_data_t;

/*! \  struct sonapps_nw_scan_res_t
 *  \brief Start scan response structure sent by SONApps to SON MgmtIfH module
 *  \param transaction_id       Identifier to map the request response messages
 *  \param result               Result (Success/Failure)
 *  \param error_code           Specifies error cause in case of Failure
 *  \param p_earfcn_list        Pointer to the scan response list containing one
 *                              node for each earfcn value
 */
typedef struct _sonapps_nw_scan_res_t
{
    son_u16 transaction_id;
    son_return_et result;
    son_error_et error_code;
    earfcn_scan_res_list_t          *p_earfcn_list;
}sonapps_nw_scan_res_t;

/*! \   sonapps_pci_conflict_enable_disable_req_t
 *  \brief    Request from OAM to enable/disable PCI optimization feature
 *  \param    transaction_id - Transaction ID of the request
 *  \param    pci_mode  - ENABLE/DISABLE
 */
typedef struct _sonapps_pci_conflict_enable_disable_req_t
{
    son_u16 transaction_id;
    son_oam_pci_conflict_enable_disable_req_t pci_enable_disable_req;
}sonapps_pci_conflict_enable_disable_req_t;

/*! \   sonapps_pci_conflict_enable_disable_res_t
 *  \brief    Response sent to SON MgmtIfH against PCI conflict enable/disable request
 *  \param    transaction_id - Transaction ID of the request for which the
 *                             response is being sent
 *  \param    result         - SUCCESS/FAILURE
 *  \param    error_code     - Valid in case of failure only
 */
typedef struct _sonapps_pci_conflict_enable_disable_res_t
{
    son_u16 transaction_id;
    son_oam_pci_conflict_enable_disable_resp_t pci_enable_disable_res;
}sonapps_pci_conflict_enable_disable_res_t;

/* header to be changed acc to oam intf.h */

/*! \   sonapps_pci_selection_req_t
 *  \brief    Request from SON MgmtIfH to APPS to select one unsed PCI from
 *            the PCI list and EARFCN provided by OAM at startup
 *  \param    transaction_id - Transaction ID of the request
 *  \param    pci_list_size  - PCI list size specifying the no of PCIs in the
 *            request message
 *  \param    pci_list       - List of PCIs
 */
typedef struct _sonapps_pci_selection_req_t
{
    son_u16 transaction_id;
    son_oam_pci_selection_req_t pci_selection_req;
}sonapps_pci_selection_req_t;

/*! \   sonapps_pci_selection_res_t
 *  \brief    Response for PCI Selection Request to SON MgmtIfH after selecting
 *            one unsed PCI from the list provided by OAM
 *  \param    transaction_id - Transaction ID of the request for which the
 *                             response is being sent
 *  \param    pci_selected   - Selected PCI by SON(Valid in case of Success)
 *  \param    result         - SUCCESS/FAILURE
 *  \param    error_code     - Valid in case of failure only
 */
typedef struct _sonapps_pci_selection_res_t
{
    son_u16 transaction_id;
    son_oam_pci_selection_resp_t pci_selection_res;
}sonapps_pci_selection_res_t;

/*! \   sonapps_pci_reselection_req_t
 *  \brief    Request from SON MgmtIfH to APPS to select one unsed PCI from
 *            the list provided by OAM
 *  \param    transaction_id - Transaction ID of the request
 *  \param    pci_list_size  - PCI list size specifying the no of PCIs in the
 *            request message
 *  \param    pci_list       - List of PCIs
 */
typedef struct _sonapps_pci_reselection_req_t
{
    son_u16 transaction_id;
    son_oam_pci_reselection_req_t pci_reselection_req;
}sonapps_pci_reselection_req_t;

/*! \   sonapps_pci_reselection_res_t
 *  \brief    Response to SON MgmtIfH after selecting one unsed PCI from the
 *            list provided by OAM
 *  \param    transaction_id - Transaction ID of the request for which the
 *                             response is being sent
 *  \param    pci_selected   - Selected PCI by SON(Valid in case of Success)
 *  \param    result         - SUCCESS/FAILURE
 *  \param    error_code     - Valid in case of failure only
 */
typedef struct _sonapps_pci_reselection_res_t
{
    son_u16 transaction_id;
    son_oam_pci_reselection_resp_t pci_reselection_res;
}sonapps_pci_reselection_res_t;

/*! \   sonapps_pci_conflict_confusion_ind_t
 *  \brief    Indication to SON MgmtIfH after receiving PCI conflict/confusion
 *            indication by ANR
 *  \param    serving_cell_id  - Source cell id
 *  \param    neighbor_cell_id - Neighbor cell id
 *  \param    pci              - conflict/confusion PCI detected by ANR
 */
typedef son_oam_pci_conflict_confusion_ind_t
    sonapps_pci_conflict_confusion_ind_t;

/*! \  struct sonapps_carrier_freq_selection_req_t
 *  \brief Carrier frequency selection request structure send by SMIF
 *         to SONApps module
 *  \param transaction_id       Identifier to map the request response messages
 *  \param meas_bandwidth       Measurement bandwidth
 *  \param num_earfcn           Number of carrier frequencies provided for
 *                              carrier frequency selection
 *  \param earfcn               List of EARFCNs
 */
typedef struct _sonapps_carrier_freq_selection_req_t
{
    son_u16 transaction_id;
    son_oam_carrier_freq_selection_req_t carrier_freq_selection_req;
}sonapps_carrier_freq_selection_req_t;

/*! \  struct sonapps_carrier_freq_selection_res_t
 *  \brief This is the response structure for carrier_freq selection request
 *  \param transaction_id     Transaction identifier
 *  \param carrier_freq       Selected carrier frequency
 *  \param result             Result (Success/Failure)
 *  \param error_code         Specifies error cause in case of Failure
 */
typedef struct _sonapps_carrier_freq_selection_res_t
{
    son_u16 transaction_id;
    son_oam_carrier_freq_selection_resp_t carrier_freq_selection_res;
}sonapps_carrier_freq_selection_res_t;

/*REL 1.4:START*/

/*! \  struct sonapps_carrier_freq_and_power_selection_req_t
 *  \brief Carrier frequency and power selection request structure send by SMIF
 *         to SONApps module
 *  \param transaction_id                     Identifier to map the request response messages.
 *  \param carrier_freq_dl_pwr_selection_req  carrier freq and dl power selection req msg
 */
typedef struct _sonapps_carrier_freq_and_power_selection_req_t
{
    son_u16 transaction_id;
    son_oam_carrier_freq_and_dl_tx_power_req_t carrier_freq_dl_pwr_selection_req;
}sonapps_carrier_freq_and_power_selection_req_t;

/*! \  struct sonapps_carrier_freq_and_power_selection_res_t
 *  \brief This is the response structure for carrier_freq and power selection request
 *  \param transaction_id                   Transaction identifier
 *  \param carrier_freq_selection_resp       carrier freq and power selection resp.
 */
typedef struct _sonapps_carrier_freq_and_power_selection_res_t
{
    son_u16 transaction_id;
    son_oam_carrier_freq_and_dl_tx_power_resp_t carrier_freq_dl_pwr_selection_resp;
}sonapps_carrier_freq_and_power_selection_res_t;

/*! \   son_apps_neighbor_cell_signal_power_t
 *  \brief    Neighboring cells signal power
 *  \param    pci_id - pci Id of cells
 *  \param    rsrq   - RSRQ
 *  \param    rsrp   - RSRP
 *  \param    reference_signal_power - Reference Signal Power
 */
typedef struct _son_apps_neighbor_cell_signal_power_t
{
    son_u16 pci_id; /*^ M, 0, H, 0, 503 ^*/
    son_u8 rsrq;  /*^ M, 0, H, 0, 34 ^*/
    son_u8 rsrp;  /*^ M, 0, H, 0, 128 ^*/
    /*SPR 13018 changes start */
    son_s8 reference_signal_power; /*^ M, 0, N, 0, 0 ^*/
    /*SPR 13018 changes end */
}son_apps_neighbor_cell_signal_power_t;

/*! \   son_carrier_freq_and_dl_power_params_resp_t
 *  \brief    Carrier frequency and DL power parameters
 *  \param    earfcn - Selected EARFCN
 *  \param    rssi   - RSSI
 *  \param    neighbor_cell_list_size - Neighbor cell list size
 *  \param    neighbor_cell_signal_power - neighboring cells signal power
 */
typedef struct _son_carrier_freq_and_dl_power_params_resp_t
{
    son_u32 earfcn;
    son_u8 rssi;
    son_u16 neighbor_cell_list_size;
    son_apps_neighbor_cell_signal_power_t neighbor_cell_signal_power[SON_MAX_NO_CELLS_PER_EARFCN];
}son_carrier_freq_and_dl_power_params_resp_t;

/*! \   son_apps_carrier_freq_and_dl_power_params_resp_t
 *  \brief    Response of carrier frequency and DL power parameter request
 *  \param    result - SUCCESS/FAILURE
 *  \param    error_code - Error code (valid in case of failure)
 *  \param    carrier_freq_and_dl_power_params_resp - Carrier frequency and other DL Power Parameters
 */
typedef struct _son_apps_carrier_freq_and_dl_power_params_resp_t
{
    son_return_et result;
    son_error_et error_code;
    son_carrier_freq_and_dl_power_params_resp_t carrier_freq_and_dl_power_params_resp;
}son_apps_carrier_freq_and_dl_power_params_resp_t;
/*REL 1.4:STOP*/

#endif /* _SON_APPS_INTF_H_ */
