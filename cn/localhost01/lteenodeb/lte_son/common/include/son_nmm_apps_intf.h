/*! \file son_nmm_apps_intf.h
 *  \brief This file contains structures/definitions of NMM and SON internal modules interface.
 *  \date Feb 01, 2012
 *  \author Sandip Samal
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_NMM_APPS_INTF_H_
#define _SON_NMM_APPS_INTF_H_

/*! \headerfile son_defines.h <>
 */

#include <son_defines.h>

/*! \def SCAN_RESP_LIST_DEQUEUE(rat_type, p_nmm_scan_res_list, p_start_scan_res_data)
 *  \brief Macro that dequeue a node from the tail of the list
 */
#define SCAN_RESP_LIST_DEQUEUE(struct_type, p_nmm_scan_res_list, p_start_scan_res_data){ \
        YLNODE *snode = ylPopHead(p_nmm_scan_res_list); \
        p_start_scan_res_data = YMEMBEROF(struct_type, s_node, snode); \
} \


/*! \def SCAN_RESP_LIST_ENQUEUE(p_scan_resp_list, p_start_scan_res_data)
 *  \brief A macro specifies "ylPushTail( p_scan_resp_list, &(p_start_scan_res_data)->s_node )" ,use to enqueue the scan response .
 */
#define SCAN_RESP_LIST_ENQUEUE(p_scan_resp_list, p_start_scan_res_data) \
    ylPushTail( p_scan_resp_list, &(p_start_scan_res_data)->s_node )

/*! \def SIB_TYPE_3_PRES_FLAG
 *  \brief A macro defines the presence bit mask value of SIB3
 */
#define SIB_TYPE_3_PRES_FLAG               0x04

/*! \def SIB_TYPE_4_PRES_FLAG
 *  \brief A macro defines the presence bit mask value of SIB4
 */
#define SIB_TYPE_4_PRES_FLAG               0x08

/*! \def SIB_TYPE_5_PRES_FLAG
 *  \brief A macro defines the presence bit mask value of SIB5
 */
#define SIB_TYPE_5_PRES_FLAG               0x10

/*! \def SIB_TYPE_6_PRES_FLAG
 *  \brief A macro defines the presence bit mask value of SIB6
 */
#define SIB_TYPE_6_PRES_FLAG               0x20

/*! \def SIB_TYPE_9_PRES_FLAG
 *  \brief A macro defines the presence bit mask value of SIB9
 */
#define SIB_TYPE_9_PRES_FLAG               0x40

/*! \def FREQ_INFO_UL_BW_PRES_FLAG
 *  \brief A macro defines the uplink frequency bandwidth presence bit mask value
 */
#define FREQ_INFO_UL_BW_PRES_FLAG 0x80

/*! \def INTRA_FREQ_CELL_RESELECT_MEAS_BW_PRES_FLAG
 *  \brief A macro defines the intra frequency cell reselection meas bandwidth presence bit mask value
 */
#define INTRA_FREQ_CELL_RESELECT_MEAS_BW_PRES_FLAG 0x100

/*! \def CELL_ACCESS_INFO_CSG_ID_PRES_FLAG
 *  \brief A macro defines the cell access info csg ID presence bit mask value
 */
#define CELL_ACCESS_INFO_CSG_ID_PRES_FLAG  0x200

/*! \enum allowed_meas_bandwidth_e
 *  \brief An enum that specifies the allowed meas bandwidth.
 */
typedef enum
{
    mbw_6 = 6,                                          /*!<  Measured bandwidth value 6 */
    mbw_15 = 15,                                        /*!<  Measured bandwidth value 15 */
    mbw_25 = 25,                                        /*!<  Measured bandwidth value 25 */
    mbw_50 = 50,                                        /*!<  Measured bandwidth value 50 */
    mbw_75 = 75,                                        /*!<  Measured bandwidth value 75 */
    mbw_100 = 100                                       /*!<  Measured bandwidth value 100 */
}allowed_meas_bandwidth_e;

/* Start: CSR 00053250 */
/*! \  struct son_nmm_pci_list_per_earfcn_t
 *  \brief                                       Contains pci list per earfcn
 *  \param num_pci                               The number of cells in the pciList. If 0 all cells on the carrier should  be found.
 *  \param pci[SON_MAX_NO_PCI]                   The list of cells which should be searched
 */  
 
typedef struct _son_nmm_pci_list_per_earfcn_t
{
    U16 num_pci;
    U16 pci[SON_MAX_NO_PCI];
}son_nmm_pci_list_per_earfcn_t;    
/*End: CSR 00053250 end */

/*! \  struct start_scan_req_t
 *  \brief                                       Contains parameters to trigger the start scan request.
 *  \param rat_type                              Variable defined the rat type(LTE, UMTS)
 *  \param meas_period                           The length of the time the NMM measures RSSI over. In 10ms radio frames
 *  \param is_meas_bandwidth_valid               Only one among meas_bandwidth and meas_bandwidth_per_earfcn will be used. Meas_bandwidth will be used only if num_earfcn is 0 and is_meas_bandwidth_valid is true. If num_earfcn is non-zero then m eas_bandwidth_per_earfcn must be provided
 *  \param meas_bandwidth                        The number of resource blocks which should be used for measuring RSRP.
 *  \param retry_count                           The number of SI windows for which decoding of a SI should be retried.The retries are counted per siPeriodicity.
 *  \param freq_band_id                          The E-UTRA band for which the carrier List applies.
 *  \param num_earfcn                            The number of carriers which should be measured. In case no carrier (value 0) is specified a ll carriers in the band shall be measured.
 *  \param earfcn[SON_MAX_NO_EARFCN]                 The list of carriers to be measured.
 *  \ meas_bandwidth_per_earfcn[SON_MAX_NO_EARFCN]   meas_bandwidth for each earfcn
 *  \param scan_timeout                         Specifies the time-out value in 
 *                                              seconds, measured from the start 
 *                                              of the REM scan, before the REM 
 *                                              scan will time out.
 */


/*SPR NO:10540 changes start */
typedef struct earfcn_pci_info_t
{
   U32                                                     earfcn;
   U8                                                       meas_bandwidth_per_earfcn;
   U16                                                     num_pci;
   U16                                                     pci[SON_MAX_NO_PCI];
}earfcn_pci_info_t;

typedef struct _start_scan_req_t
{
    son_rat_type_et                         rat_type;
    U8                                                      meas_period;
    U8                                                      is_meas_bandwidth_valid;
    U8                                                      meas_bandwidth;
    U8                                                      retry_count;
    U8                                                      freq_band_id;
    U16                                                    num_earfcn;
    earfcn_pci_info_t                        earfcn_pci_info[SON_MAX_NO_EARFCN];
    U32                                                    scan_timeout;
}start_scan_req_t;

/*SPR NO:10540 changes end */



/*! \  struct son_nmm_start_scan_req_t
 *  \brief                                       Contains parameters for scan request from internal SON modules to NMMCTL.
 *  \param transaction_id                        Transaction id
 *  \param originator_module_id                  Originator module id
 *  \param *p_start_scan_req                     Pointer to start_scan_req_t
 */

typedef struct _son_nmm_start_scan_req_t
{
    U16 transaction_id;
    U16 originator_module_id;
    start_scan_req_t    *p_start_scan_req;
} son_nmm_start_scan_req_t;

/*! \  struct son_nmm_periodic_nw_scan_req_t
 *  \brief                                       Contains parameters for scan request from internal SON modules to NMMCTL.
 *  \param transaction_id                        Transaction id
 *  \param scan_on_off                           Periodic scan ON/OFF option
 *  \param scan_time_interval                    Periodic scan interval
 *  \param *p_start_scan_req                     Pointer to start_scan_req_t
 */

typedef struct _son_nmm_periodic_nw_scan_req_t
{
    U16 transaction_id;
    U16 originator_module_id;
    U8 scan_on_off;
    U32 scan_time_interval;
    start_scan_req_t     *p_start_scan_req;
} son_nmm_periodic_nw_scan_req_t;

/*! \var typedef YLIST nmm_scan_res_list_t
 *  \brief A type definition for YLIST .
 */

typedef YLIST nmm_scan_res_list_t;

/*! \  struct   start_scan_res_t
 *  \brief                                       Contains parameter for scan response data.
 *  \param son_resp                              Response message obtained from SON
 *  \param son_error                             Error obtained from SON
 *  \param *p_nmm_scan_res_list                  Pointer to Eutran Scan Result list
 *  \param *p_nmm_scan_res_list                  Pointer to Utran Scan Result list
 */

typedef struct _start_scan_res_t
{
    son_return_et son_resp;
    son_error_et son_error;
    nmm_scan_res_list_t  *p_eutran_scan_res_list;
    nmm_scan_res_list_t  *p_utran_scan_res_list;
}start_scan_res_t;

/*! \  struct son_nmm_start_scan_res_t
 *  \brief                                       Contains parameters for scan response from NMMCTL to internal SON modules.
 *  \param transaction_id                        Transaction id
 *  \param originator_module_id                  Originator module id
 *  \param start_scan_res                        Variable for start_scan_res_t.
 */

typedef struct _son_nmm_start_scan_res_t
{
    U16 transaction_id;
    U16 originator_module_id;
    start_scan_res_t start_scan_res;
} son_nmm_start_scan_res_t;

/*! \  struct plmn_id_t
 *  \brief                                       caontains plmn parameters
 *  \param mcc[SON_MCC_OCTET_SIZE]               mcc array
 *  \param num_mnc_digits                        Number of mnc digits
 *  \param mnc[SON_MAX_MNC_OCTET_SIZE]           mnc array
 */

typedef struct plmn_id_t
{
    U8 mcc[SON_MCC_OCTET_SIZE];
    U8 num_mnc_digits;
    U8 mnc[SON_MAX_MNC_OCTET_SIZE];
}plmn_id_t;

/*! \  struct intra_freq_neighbor_info_t
 *  \brief   Contains Neighbor Info read from SIB4 IntraFreqNeighCellList parameter
 *  \param  pci
 *  \param  cell_specific_offset
 */
typedef struct _intra_freq_neighbor_info_t
{
    son_u16 pci;
    son_q_offset_range_et cell_specific_offset;
}intra_freq_neighbor_info_t;

/*! \  struct carrier_freq_utran_fdd_threshX_Q_r9_t
 *  \brief   Structure defining the Squal threshold (in dB) used by the UE
 *  \param threshX_highQ_r9  This specifies the Squal threshold (in dB) used by the UE when reselecting towards a higher priority RAT/ frequency than the current serving frequency.
 *  \param threshX_LowQ_r9  This specifies the Squal threshold (in dB) used by the UE when reselecting towards a lower priority RAT/ frequency than the current serving frequency
 */

typedef struct _carrier_freq_utran_fdd_threshX_Q_r9_t
{
    U8 threshX_highQ_r9;
    U8 threshX_LowQ_r9;
}carrier_freq_utran_fdd_threshX_Q_r9_t;

/*! \  struct carrier_freq_utran_fdd_t
 *  \brief   Structure containing information about UTRA frequencies
 *  \param presence_bitmask  Bit mask
 *  \param carrier_freq
 *  \param cell_reselection_priority  This specifies the absolute priority for E-UTRAN frequeny or UTRAN frequency or group of GERAN frequencies or band class of CDMA2000 HRPD or band class of CDMA2000 1xRTT
 *  \param thresh_X_High  This specifies the Srxlev threshold (in dB) used by the UE when reselecting towards a higher priority RAT/ frequency than the current serving frequency
 *  \param thresh_X_Low   This specifies the Srxlev threshold (in dB) used by the UE when reselecting towards a lower priority RAT/ frequency than the current serving frequency
 *  \param q_Rx_Lev_Min   This specifies the minimum required Rx level in the cell in dBm.
 *  \param p_Max_UTRA
 *  \param q_Qual_min     This specifies the minimum required quality level in the cell in dB
 *  \param thresh_X_Q_r9  Structure defining the Squal threshold (in dB) used by the UE
 */

typedef struct  _carrier_freq_utran_fdd_t
{
    U16 presence_bitmask;
    U16 carrier_freq;
    U8 cell_reselection_priority;
    U8 thresh_X_High;
    U8 thresh_X_Low;
    S8 q_Rx_Lev_Min;
    S8 p_Max_UTRA;
    S8 q_Qual_min;
    carrier_freq_utran_fdd_threshX_Q_r9_t thresh_X_Q_r9;
}carrier_freq_utran_fdd_t;

/*! \  struct plmn_identity_info_t
 *  \brief   Structure containing information about plmn identity
 *  \param   plmn_id   MCC and MNCs
 *  \param   cell_res_for_operator_use                   Cell reserved/unreserved
 */

typedef struct _nmm_plmn_identity_info_t
{
    plmn_id_t plmn_id;
    U8 cell_res_for_operator_use;
}nmm_plmn_identity_info_t;

#define SIB_TYPE_1_PRES_FLAG               0x01
#define SIB_TYPE_2_PRES_FLAG               0x02

/*! \  struct eutran_cell_num_t
 *  \brief                                             Contains cell parameters
 *  \param pci_id                                      The physical cell identity
 *  \param cell_identity[SON_CELL_ID_OCTET_SIZE]       Cell Identity read from SIB-1
 *  \param cell_barred                                 Cell barred
 *  \param csg_indication                              CSG indication from SIB-1
 *  \param csg_identity                                CSG Identity from SIB-1 . This is valid only if csg_indication is true
 *  \param reference_signal_power                      from SIB-2 - Actual value = reference_signal_power - 60
 *  \param rsrp                                        conversion to be applied . Actual RSRP = rsrp -144
 *  \param plmn_identity_info[SON_MAX_NUM_PLMN]        Array of plmn identity info
 *  \param tac[SON_TAC_OCTET_SIZE]                     Array of tac
 *  \param num_of_antenna                              Number of antennas
 *  \param dl_sys_bw                                   Num of DL RBs
 *  \param ul_sys_bw                                   Num of UL RBs
 *  \param allowed_meas_bandwidth                      Allowed measured bandwidth
 *  \param presence_antenna_port                       Presence antenna port
 *  \param num_intra_freq_neighbor                     Number of Inra Frequency Neighbors
 *  \param intra_freq_neighbor_list                    List of Inra Frequency Neighbors
 *  \param num_utran_fdd_carrier_freq                  Nuber of UTRA frequencies
 *  \param carrier_freq_utran_fdd_data                 Structure containing information about UTRA frequencies
 *  \param size_hnb_Name_str                           Size of home eNB name
 *  \param hnb_Name                                    Home eNB name
 */

typedef struct eutran_cell_num_t
{
    U32 presence_bitmask;
    U16 pci_id;
    U8 cell_Id[SON_CELL_ID_OCTET_SIZE];
    U8 cell_barred;
    U8 csg_indication;
    U8 csg_identity[SON_CSG_ID_OCTET_SIZE];
    /*SPR 13018 changes start */
    S8 reference_signal_power;
    /*SPR 13018 changes end */
    U8 rsrp;
    U8 num_plmn_id;
    U8 tac[SON_TAC_OCTET_SIZE] ;
    nmm_plmn_identity_info_t plmn_identity_info[SON_MAX_NUM_PLMN];
    U8 num_of_antenna;
    U8 dl_sys_bw;
    U8 ul_sys_bw;
    /* SPR 9781 Start */
    U8 allowed_meas_bandwidth;
    /* SPR 9781 End */
    son_bool_et presence_antenna_port;
    U8                      num_intra_freq_neighbor;
    intra_freq_neighbor_info_t  intra_freq_neighbor_list[SON_MAX_INTRA_FREQ_NEIGHBORS_IN_SIB4];
    U8 num_utran_fdd_carrier_freq;
    carrier_freq_utran_fdd_t carrier_freq_utran_fdd_data[256];
    U8  size_hnb_Name_str;
    U8  hnb_Name[256];

 }eutran_cell_num_t;

/*! \  struct utran_cell_num_t
 *  \brief                                             Contains cell parameters
 *  \param pci_id                                      The physical cell identity
 *  \param cell_identity[SON_CELL_ID_OCTET_SIZE]       Cell Identity read from SIB-1
 *  \param cell_barred                                 Cell barred
 *  \param csg_indication                              CSG indication from SIB-1
 *  \param csg_identity                                CSG Identity from SIB-1 . This is valid only if csg_indication is true
 *  \param rscp                                         -5 to 91
 *  \param ecno                                        0 to 49
 *  \param plmn_id[SON_MAX_NUM_PLMN]                   Array of plmn id's
 *  \param lac[SON_LAC_OCTET_SIZE]                     Array of lac
 *  \param rac                                         Rac
 *  \param num_of_antenna                              Number of antennas
 *  \param dl_sys_bw                                   Num of DL RBs {6,...,100}
 *  \param ul_sys_bw                                   Num of UL RBs
 *  \param allowed_meas_bandwidth                      Allowed measured bandwidth
 *  \param presence_antenna_port                       Presence antenna port
 */

typedef struct utran_cell_num_t
{
    son_utran_physical_cell_id_t pci_id;
    U8 cell_Id[SON_CELL_ID_OCTET_SIZE];
    U8 cell_barred;
    U8 csg_indication;
    U8 csg_identity[SON_CSG_ID_OCTET_SIZE];
    S8 rscp;
    U8 ecno;
    U8 num_plmn_id;
    plmn_id_t plmn_id[SON_MAX_NUM_PLMN];
    U8 lac[SON_LAC_OCTET_SIZE];
    U8 rac;
    U8 num_of_antenna;
    U8 dl_sys_bw;
    U8 ul_sys_bw;
    allowed_meas_bandwidth_e allowed_meas_bandwidth;
    son_bool_et presence_antenna_port;
}utran_cell_num_t;

/*! \  struct eutran_channel_num_t
 *  \brief                                 Contains frequency channel parameters
 *  \param earfcn                          EARFCN value
 *  \param rssi                            conversion to be applied . Actual RSSI = rssi -144
 *  \param num_of_cells                    Number of cells
 *  \param cell[SON_MAX_NO_PCI]            Cell array
 */
typedef struct _eutran_channel_num_t
{
    U32 earfcn;
    U8 rssi;
    U16 num_of_cells;
    eutran_cell_num_t cell[SON_MAX_NO_PCI];
}eutran_channel_num_t;

/*! \  struct utran_channel_num_t
 *  \brief                                 Contains frequency channel parameters
 *  \param uarfcn                          UARFCN value
 *  \param num_of_cells                    Number of cells
 *  \param cell[SON_MAX_NO_PCI]            Cell array
 */
typedef struct _utran_channel_num_t
{
    U16 uarfcn;
    U16 num_of_cells;
    utran_cell_num_t cell[SON_MAX_NO_PCI];
}utran_channel_num_t;

/*! \  struct start_scan_res_eutran_data_t
 *  \brief                                 Contains the data parameters for start scan response.
 *  \param s_node                          Pointer to data list
 *  \param scan_res_data                   Variable for eutran_channel_num_t structure
 */

typedef struct _start_scan_res_eutran_data_t
{
    YLNODE s_node;
    eutran_channel_num_t scan_res_data;
}start_scan_res_eutran_data_t;

/*! \  struct start_scan_res_utran_data_t
 *  \brief                                 Contains the data parameters for start scan response.
 *  \param s_node                          Pointer to data list
 *  \param scan_res_data                   Variable for utran_channel_num_t structure
 */

typedef struct _start_scan_res_utran_data_t
{
    YLNODE s_node;
    utran_channel_num_t scan_res_data;
}start_scan_res_utran_data_t;

/*! \  struct son_nmm_stop_scan_req_t
 *  \brief                                 Contains parameters for stop scan request from internal SON modules to NMMCTL
 *  \param transaction_id                  Represent the transaction ID
 */

typedef struct _son_nmm_stop_scan_req_t
{
    U16 transaction_id;
} son_nmm_stop_scan_req_t;

/*! \  struct stop_scan_res_t
 *  \brief                                 Contains the parameters for stop scan response
 *  \param son_resp                        Represent the son response
 *  \param son_error                       Represent the son error
 */

typedef struct _stop_scan_res_t
{
    son_return_et son_resp;
    son_error_et son_error;
}stop_scan_res_t;

/*! \  struct son_nmm_stop_scan_res_t
 *  \brief                                 Contains parameters for stop scan response from NMMCTL to internal SON modules
 *  \param transaction_id                  Represent the transaction ID
 *  \param stop_scan_res                   Variable for stop_scan_res_t structure
 */
typedef struct _son_nmm_stop_scan_res_t
{
    U16 transaction_id;
    stop_scan_res_t stop_scan_res;
} son_nmm_stop_scan_res_t;

#endif /* ifndef _SON_NMM_APPS_INTF_H_ */
