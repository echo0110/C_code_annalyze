/*! \file son_fapi_intf.h 
 *  \brief This file contains structures/defines of NMM-PHY intf for fapi.
 *  \date January 02, 2012 
 *  \author gur26210
 */
/*!
 *  \copyright {Copyright (c) 2009, Aricent Inc. All Rights Reserved}
*/

#ifndef _SON_NMM_FAPI_INTF_H_
#define _SON_NMM_FAPI_INTF_H_

/*! \headerfile son_nmm_fapi.h <>
*/

#include <son_nmm_fapi.h>


/*! \enum mib_status_et
 *  \brief An enum that specifies the MIB cell status.
 */
typedef enum
{
/*SPR_14770 Fix start*/
    MIB_OK,                                   /*!< cell status 0 */
    MIB_FAILED                              /*!< cell status 1 */
/*SPR_14770 Fix end*/
}mib_status_et;

/*! \enum phich_duration_et
 *  \brief An enum that specifies the duration type of the cell.
 */
typedef enum
{
    normal,                                 /*!< phich_duration 0 */
    extended                                /*!< phich_duration 1 */
}phich_duration_et;


/*! \enum phich_resource_et
 *  \brief An enum that specifies the size of phich_resource cell.
 */
typedef enum
{
    nmm_one_sixth,                              /*!< phich_resouce 0 */                               
    nmm_one_half,                               /*!< phich_resouce 1 */
    nmm_one,                                    /*!< phich_resouce 2 */
    nmm_two                                     /*!< phich_resouce 3 */
}phich_resource_et;

/*! \  vendor_specific_list_t
 *  \brief          Contains the vendor specific configuration parameters.
 *  \param type     Indicating the type of the value
 *  \param length   The length of actual value      
 *  \param value[MAX_VENDER_SPECIFIC_VALUE]      The actual value which will be set
 */

typedef struct _vendor_specific_list_t
{
    U32 type;            
    /*^ M, 0, N, 0, 0 ^*/
    U32 length;          
    /*^ M, 0, N, 0, 0 ^*/
    U8  value[MAX_VENDER_SPECIFIC_VALUE]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}vendor_specific_list_t;

/*! \  struct sib_info_t
 *  \brief              Contains the sib information parameters.
 *  \param sfn          The radio frame the data was received on
 *  \param sf           The subframe the data was received on.
 *  \param len          The length of the BCCH payload
 *  \param data[MAX_BCCH_SIZE]  The BCCH payload. The payload is mapped according to 36.212(Byte 0, bit 8 is mapped to a0).
 */

typedef struct _sib_info_t
{
    U16 sfn;                 
    /*^ M, 0, H, 0, 1023 ^*/
    U8  sf;                  
    /*^ M, 0, H, 0, 9 ^*/
    /*SPR_14769_16627 Fix start*/    
    U16 len;                 
    /*^ M, 0, H, 0, 2048 ^*/
    /*SPR_14769_16627 Fix end*/
    U8 data[MAX_BCCH_SIZE];  
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}sib_info_t;

/*! \  struct son_phy_rssi_meas_req_t
 *  \brief                Contains the parametes that will be sent to phy to obtain the rssi corresponding to the given earfcn's.
 *  \param freq_band_indicator The E-UTRA band for which the carrierList applies
 *  \param meas_period      The length of time the NMM measure RSSI over. In 10ms radio frames.
 *  \param num_earfcn       The number of carriers which should be measured
 *  \param earfcn           The list of carriers to be measured
 *  \param num_vendor_specific_list  The number of elements in the vendor_specific_list
 *  \param vendor_specific_list[MAX_VENDOR_LIST]      Specifies the vendor specific configuration.
 */

typedef struct _son_phy_rssi_meas_req_t
{
    U8 freq_band_indicator;                     
    /*^ M, 0, B, 1, MAX_FREQ_BAND_INDICATOR ^*/
    U8 meas_period;                             
    /*^ M, 0, N, 0, 0 ^*/
    U8 num_earfcn;                              
    /*^ M, 0, H, 0, 100 ^*/
    U32 earfcn[MAX_CARRIER_LIST];               
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    U8 num_vendor_specific_list;                
    /*^ M, 0, H, 0, 16 ^*/
    vendor_specific_list_t vendor_specific_list[MAX_VENDOR_LIST];  
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_phy_rssi_meas_req_t;/*^ API, SON_PHY_RSSI_MEAS_REQ ^*/

/*! \  struct son_phy_rssi_meas_ind_t
 *  \brief                  Contains the parameters that are required to store the values obtained from phy after sending the rssi request.
 *  \param rssi				The measured RSSI in dBm in steps of 1dB.
 *  \param earfcn			The earfcn for which cells should be measured.
 *  \param num_vendor_specific_list     The number of elements in the vendor_specific_list
 *  \param vendor_specific_list[MAX_VENDOR_LIST]    Specifies the vendor specific configuration.      
*/

typedef struct _son_phy_rssi_meas_ind_t
{
    U8 rssi;                                 
    /*^ M, 0, H, 0, 128 ^*/
    U32 earfcn;                              
    /*^ M, 0, N, 0, MAX_EARFCN ^*/
    U8 num_vendor_specific_list;             
    /*^ M, 0, H, 0, 16 ^*/
    vendor_specific_list_t vendor_specific_list[MAX_VENDOR_LIST];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_phy_rssi_meas_ind_t;/*^ API, SON_PHY_RSSI_MEAS_IND ^*/


/*! \  struct son_phy_rssi_meas_cnf_t
 *  \brief      This structure is used after getting the rssi confirmation from the phy.
 *  \param num_vendor_specific_list     The number of elements in the vendor_specific_list
 *  \param vendor_specific_list[MAX_VENDOR_LIST]    Specifies the vendor specific configuration.      
*/
typedef struct _son_phy_rssi_meas_cnf_t
{
    U8 num_vendor_specific_list;         
    /*^ M, 0, H, 0, 16 ^*/
    vendor_specific_list_t vendor_specific_list[MAX_VENDOR_LIST];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_phy_rssi_meas_cnf_t;/*^ API, SON_PHY_RSSI_MEAS_CNF ^*/


/*! \  struct son_phy_stop_rssi_meas_req_t
 *  \brief  Contains the parameters required to trigger the stop scan request to phy.    
 *  \param num_vendor_specific_list  The number of elements in the vendor_specific_list
 *  \param vendor_specific_list[MAX_VENDOR_LIST]    Specifies the vendor specific configuration.
*/
typedef struct _son_phy_stop_rssi_meas_req_t
{
    U8 num_vendor_specific_list;             
    /*^ M, 0, H, 0, 16 ^*/
    vendor_specific_list_t vendor_specific_list[MAX_VENDOR_LIST];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_phy_stop_rssi_meas_req_t;/*^ API, SON_PHY_STOP_MEAS_REQ ^*/


/*! \  struct son_phy_cell_search_req_t
 *  \brief  Contains the parameters required to trigger the cell search request to phy.
 *  \param earfcn            The earfcn for which cells should be measured
 *  \param exhaustive_searc  NMM should try to find all cells on the carrier
 *  \param num_pci_list      The number of cells in the pciList
 *  \param pci_list[MAX_PCI_LIST]          The list of cells which should be searched
 *  \param meas_bandwidth      The number of resource blocks which should be used
 *  \param meas_period       The length of time the NMM measure
 *  \param num_vendor_specific_list The number of elements in the vendor_specific_list
 *  \param vendor_specific_list[MAX_VENDOR_LIST]    Specifies the vendor specific configuration.
 */

typedef struct _son_phy_cell_search_req_t
{
    U32 earfcn;                                    
    /*^ M, 0, N, 0, MAX_EARFCN ^*/
    U8 exhaustive_search;   
    /*^ M, 0, H, 0, 1 ^*/
    U8 num_pci_list;       
    /*^ M, 0, H, 0, 50 ^*/
    U16 pci_list[MAX_PCI_LIST];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    U8 meas_bandwidth;         
    /*^ M, 0, B, 6, 100 ^*/
    U8 meas_period;           
    /*^ M, 0, L, 1, 0  ^*/
    U8 num_vendor_specific_list;    
    /*^ M, 0, H, 0, 16 ^*/
    vendor_specific_list_t vendor_specific_list[MAX_VENDOR_LIST];    
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_phy_cell_search_req_t;/*^ API, SON_PHY_CELL_SEARCH_REQ ^*/
    
    
/*! \  struct cell_found_list_t
 *  \brief   Contains the parameters required to store the rsrp values corresponding to the given pci's.
 *  \param rsrp				The measured RSRP values in dBm .
 *  \param pci				The physical identity of the cell which the NMM synchronized to. 
*/

typedef struct _cell_found_list_t
{
    U8 rsrp;                           
    /*^ M, 0, H, 0, 128 ^*/
    U16 pci;                           
    /*^ M, 0, H, 0, 503 ^*/
}cell_found_list_t;


/*! \  struct son_phy_cell_search_ind_t
 *  \brief Contains the parameters required to store the values obtained after sending the cell search request to phy.
 *  \param num_pci					The number of cells in the pciList. If 0 all cells on the carrier should be found.	    	
 *  \param cell_found_list[MAX_CELL_FOUND_LIST]     	The cell information from the NMM.
 *  \param num_vendor_specific_list The number of elements in the vendor_specific_list
 *  \param vendor_specific_list[MAX_VENDOR_LIST]    Specifies the vendor specific configuration     
*/
typedef struct _son_phy_cell_search_ind_t
{
    U8 num_pci;
    /*^ M, 0, H, 0, 16 ^*/
    cell_found_list_t cell_found_list[MAX_CELL_FOUND_LIST];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    U8 num_vendor_specific_list;                  
    /*^ M, 0, H, 0, 16 ^*/
    vendor_specific_list_t vendor_specific_list[MAX_VENDOR_LIST];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_phy_cell_search_ind_t;/*^ API, SON_PHY_CELL_SEARCH_IND ^*/


/*! \  struct son_phy_cell_search_cnf_t
 *  \brief This structure is used after getting the cell search confirmation from the phy.
 *  \param num_vendor_specific_list  The number of elements in the vendor_specific_list
 *  \param vendor_specific_list[MAX_VENDOR_LIST]    Specifies the vendor specific configuration
*/
typedef struct _son_phy_cell_search_cnf_t
{
    U8 num_vendor_specific_list;             
    /*^ M, 0, H, 0, 16 ^*/
    vendor_specific_list_t vendor_specific_list[MAX_VENDOR_LIST];/*For future use.*/
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_phy_cell_search_cnf_t;/*^ API, SON_PHY_CELL_SEARCH_CNF ^*/


/*! \  struct son_phy_stop_cell_search_req_t
 *  \brief Contains the parameters required to trigger the stop scan request to phy.
 *  \param num_vendor_specific_list  The number of elements in the vendor_specific_list
 *  \param vendor_specific_list[MAX_VENDOR_LIST]    Specifies the vendor specific configuration      
*/
typedef struct _son_phy_stop_cell_search_req_t
{
    U8 num_vendor_specific_list;             /*0- MAX_VENDOR_LIST */
    /*^ M, 0, H, 0, 16 ^*/
    vendor_specific_list_t vendor_specific_list[MAX_VENDOR_LIST];/*For future use.*/
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_phy_stop_cell_search_req_t;/*^ API, SON_PHY_STOP_CELL_SEARCH_REQ ^*/


/*! \  struc son_phy_pbch_config_req_t
 *  \brief  Contains the parametere required to send the pbch configuration request to phy.
 *  \param pci                    The physical cell identity 
 *  \param earfcn                 The EARFCN for the cell
 *  \param num_vendor_specific_list The number of elements in the vendor_specific_list
 *  \param vendor_specific_list[MAX_VENDOR_LIST]    Specifies the vendor specific configuration     
 */

typedef struct _son_phy_pbch_config_req_t
{
    U16 pci;                       
    /*^ M, 0, H, 0, 503 ^*/
    U32 earfcn;                    
    /*^ M, 0, N, 0, MAX_EARFCN ^*/
    U8 num_vendor_specific_list;   
    /*^ M, 0, H, 0, 16 ^*/
    vendor_specific_list_t vendor_specific_list[MAX_VENDOR_LIST];    
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_phy_pbch_config_req_t;/*^ API, SON_PHY_PBCH_CONFIG_REQ ^*/


/*! \  struct son_phy_pbch_config_cnf_t
 *  \brief This structure is used after getting the pbch configuration confirmation from the phy.
 *  \param num_vendor_specific_list  The number of elements in the vendor_specific_list
 *  \param vendor_specific_list[MAX_VENDOR_LIST]    Specifies the vendor specific configuration      
*/
typedef struct _son_phy_pbch_config_cnf_t
{
    U8 num_vendor_specific_list;       
    /*^ M, 0, H, 0, 16 ^*/
    vendor_specific_list_t vendor_specific_list[MAX_VENDOR_LIST];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_phy_pbch_config_cnf_t;/*^ API, SON_PHY_PBCH_CONFIG_CNF ^*/

/*! \  struct son_phy_pbch_data_ind_t
 *  \brief Contains the parameters required to store the configuration parameters for pbch.
 *  \param pci				The physical identity of the cell which the NMM should read the PBCH.
 *  \param cause			It indicates if MIB readind was successful or not. In case reading failed the next parameters are invalid.
 *  \param num_antenna			The number of transmit antennas of the cell.
 *  \param sfn				The radio frame the data was received on.
 *  \param phich_duration		The PHICH duration of the cell.
 *  \param phich_resource		The PHICH resource size of the cell.
 *  \param dl_sys_bandwidth		The DL transmission bandwidth in RBs.
 *  \param num_vendor_specific_list The number of elements in the vendor_specific_list
 *  \param vendor_specific_list[MAX_VENDOR_LIST]   Specifies the vendor specific configuration
*/

typedef struct _son_phy_pbch_data_ind_t_
{
    U16 pci;                                 
    /*^ M, 0, H, 0, 503 ^*/
    mib_status_et cause;                     
    /*^ M, 0, H, 0, 1 ^*/
    U8 num_antenna;                          
    /*^ M, 0, B, 1, 4 ^*/
    U16 sfn;                                 
    /*^ M, 0, H, 0, 1023 ^*/
    phich_duration_et phich_duration;        
    /*^ M, 0, H, 0, 1 ^*/
    phich_resource_et phich_resource;        
    /*^ M, 0, H, 0, 3 ^*/
    U8 dl_sys_bandwidth;                     
    /*^ M, 0, B, 6, 100 ^*/
    U8 num_vendor_specific_list;             
    /*^ M, 0, H, 0, 16 ^*/
    vendor_specific_list_t vendor_specific_list[MAX_VENDOR_LIST]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_phy_pbch_data_ind_t;/*^ API, SON_PHY_PBCH_DATA_IND ^*/


/*! \  struct son_phy_stop_pbch_req_t
 *  \brief Contains the parameters required to trigger the stop scan request to phy.
 *  \param num_vendor_specific_list  The number of elements in the vendor_specific_list
 *  \param vendor_specific_list[MAX_VENDOR_LIST]    Specifies the vendor specific configuration

 */
typedef struct _son_phy_stop_pbch_req_t
{
    U8 num_vendor_specific_list;  
    /*^ M, 0, H, 0, 16 ^*/
    vendor_specific_list_t vendor_specific_list[MAX_VENDOR_LIST];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_phy_stop_pbch_req_t;/*^ API, SON_PHY_STOP_PBCH_REQ ^*/


/*! \  struct son_phy_sib1_config_req_t
 *  \brief Contains the parameters requires to trigger the sib1 configuration request to phy.
 *  \param retry_count    The number of periods for which decoding of a SIB1 should be retried.
 *  \param num_vendor_specific_list The number of elements in the vendor_specific_list
 *  \param vendor_specific_list[MAX_VENDOR_LIST]    Specifies the vendor specific configuration
 */
typedef struct _son_phy_sib1_config_req_t
{
    U8 retry_count;                               
    /*^ M, 0, B, 1, 8 ^*/
    U8 num_vendor_specific_list;                  
    /*^ M, 0, H, 0, 16 ^*/
    vendor_specific_list_t vendor_specific_list[MAX_VENDOR_LIST];    
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_phy_sib1_config_req_t;/*^ API,SON_PHY_SIB1_CONFIG_REQ ^*/


/*! \  struct son_phy_sib1_config_cnf_t
 *  \brief This structure is used after getting the sib1 configuration confirmation from phy.
 *  \param num_vendor_specific_list  The number of elements in the vendor_specific_list
 *  \param vendor_specific_list[MAX_VENDOR_LIST]    Specifies the vendor specific configuration
*/
typedef struct _son_phy_sib1_config_cnf_t
{
    U8 num_vendor_specific_list;    
    /*^ M, 0, H, 0, 16 ^*/
    vendor_specific_list_t vendor_specific_list[MAX_VENDOR_LIST];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_phy_sib1_config_cnf_t;/*^ API, SON_PHY_SIB1_CONFIG_CNF ^*/



/*! \  struct son_phy_bcch_config_req_t
 *  \brief Contains the parameters required to trigger the bcch configuration request.
 *  \param num_si_periodicity_list      The number of periods for which decoding of a SIB1 should be retried
 *  \param si_periodicity_list          The SI periodicity of all requested SIs.
 *  \param si_window                    The SI window.
 *  \param retry_count                  The number of periods for which decoding of a SIB1 should be retried.
 *  \param num_vendor_specific_list     The number of elements in the vendor_specific_list
 *  \param vendor_specific_list[MAX_VENDOR_LIST]    Specifies the vendor specific configuration
 */
typedef struct _son_phy_bcch_config_req_t
{
    U8 num_si_periodicity_list;       
    /*^ M, 0, H, 0, 32 ^*/
    U16 si_periodicity_list[MAX_SI_LIST];  
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    U8 si_window;                          
    /*^ M, 0, B, 1, 40 ^*/
    U8 retry_count;                        
    /*^ M, 0, B, 1, 8 ^*/
    /*+SPR 18854 FIX*/
    U8     sub_frame_assignment; /* 0..6 (For TDD mode) */
    /*^ M, 0, H, 0, 6 ^*/
    U8     special_sub_frame_patterns; /* 0..8 (For TDD mode) */
    /*^ M, 0, H, 0, 8 ^*/
    /*-SPR 18854 FIX*/
    U8 num_vendor_specific_list;           
    /*^ M, 0, H, 0, 16 ^*/
    vendor_specific_list_t vendor_specific_list[MAX_VENDOR_LIST];    
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_phy_bcch_config_req_t;/*^ API, SON_PHY_BCCH_CONFIG_REQ ^*/


/*! \  struct son_phy_bcch_config_cnf_t
 *  \brief This structure is used after getting the bcch configuration from phy.
 *  \param num_vendor_specific_list  The number of elements in the vendor_specific_list
 *  \param vendor_specific_list[MAX_VENDOR_LIST]    Specifies the vendor specific configuration      
*/
typedef struct _son_phy_bcch_config_cnf_t
{
    U8 num_vendor_specific_list;  
    /*^ M, 0, H, 0, 16 ^*/
    vendor_specific_list_t vendor_specific_list[MAX_VENDOR_LIST];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_phy_bcch_config_cnf_t;/*^ API, SON_PHY_BCCH_CONFIG_CNF ^*/


/*! \  struct son_phy_bcch_data_ind_t
 *  \brief Contains the parameters required to store the canfiguration data received for bcch configuration request.
 *  \param num_bcch_data_list			The number of BCCHs in this message.
 *  \param sib_info[MAX_BCCH_DATA_LIST]		The variable of structure sib_info_t	
 *  \param num_vendor_specific_list The number of elements in the vendor_specific_list
 *  \param vendor_specific_list[MAX_VENDOR_LIST]    Specifies the vendor specific configuration
*/
typedef struct _son_phy_bcch_data_ind_t
{
    U8 num_bcch_data_list;                  
    /*^ M, 0, H, 0, 32 ^*/
    sib_info_t sib_info[MAX_BCCH_DATA_LIST];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    U8 num_vendor_specific_list;         
    /*^ M, 0, H, 0, 16 ^*/
    vendor_specific_list_t vendor_specific_list[MAX_VENDOR_LIST];  
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_phy_bcch_data_ind_t;/*^ API, SON_PHY_BCCH_DATA_IND ^*/


/*! \  struct son_phy_stop_bcch_req_t
 *  \brief Contains the parameters required to trigger the bcch stop scan request   
 *  \param num_vendor_specific_list  The number of elements in the vendor_specific_list
 *  \param vendor_specific_list[MAX_VENDOR_LIST]    Specifies the vendor specific configuration
*/
typedef struct _son_phy_stop_bcch_req_t
{
    U8 num_vendor_specific_list;   
    /*^ M, 0, H, 0, 16 ^*/
    vendor_specific_list_t vendor_specific_list[MAX_VENDOR_LIST];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}son_phy_stop_bcch_req_t;/*^ API, SON_PHY_STOP_BCCH_REQ ^*/
#endif
