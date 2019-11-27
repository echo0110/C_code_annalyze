/*! \file son_nmm_fapi.h 
 *  \brief This file contains  definitions for SON_FAPI 
 *  \date January 02, 2012
 *  \author gur26210
 */
/*!
 *  \copyright {Copyright (c) 2009, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_NMM_FAPI_H_
#define _SON_NMM_FAPI_H_

/*! \headerfile son_utils.h <>
*/

#include<son_utils.h>

/*! \def SON_FAPI_MODULE_ID
 *  \brief A macro that store a integer value 100 ,represent the module id for L1.
 */

#define SON_FAPI_MODULE_ID 8 

/*! \def MAX_CARRIER_LIST
 *  \brief A macro that store a integer value 100, represent the maximum number of carriers in the List.
 */
#define MAX_CARRIER_LIST    100     

/*! \def MAX_PCI_LIST
 *  \brief A macro that store a integer value 50, represent the maximum number of PCI's.
 */
#define MAX_PCI_LIST        50     

/*! \def MAX_VENDOR_LIST
 *  \brief A macro that store a integer value 16, represent maximum number of vendors sent in a message .
 */
#define MAX_VENDOR_LIST     16    

/*! \def MAX_SI_LIST
 *  \brief A macro that store a integer value 32.
 */
#define MAX_SI_LIST         32    

/*! \def MAX_BCCH_SIZE
 *  \brief A macro that store a integer value 2048 ,represent the maximum size of BCCH data.
 */
#define MAX_BCCH_SIZE       2048    
/*! \def MAX_CELL_FOUND_LIST
 *  \brief A macro that store a integer value 16, represent the maximum number of cells the can be found at a time.
 */

#define MAX_CELL_FOUND_LIST 16     

/*! \def MAX_BCCH_DATA_LIST
 *  \brief A macro that store a integer value 32, represent the size of list for BCCH data.
 */
#define MAX_BCCH_DATA_LIST  32    

/*! \def SON_PHY_INTERFACE_API_HEADER_SIZE
 *  \brief A macro that store a integer value 16, represent the api header  size.
 */
#define SON_PHY_INTERFACE_API_HEADER_SIZE 4


/*! \def MAX_VENDER_SPECIFIC_VALUE
 *  \brief A macro that store a integer value 2048, represent the maximum range for vendor data.
 */
/* Covertiy Fix 28 Sep 2015 Start */
#define MAX_VENDER_SPECIFIC_VALUE    256 
/* Covertiy Fix 28 Sep 2015 End */


/*! \def NMM_FSM_MSG_PLATFORM_BASE
 *  \brief A macro that store a integer value 50 , represent a base value for message id's.
 */
#define NMM_FSM_MSG_PLATFORM_BASE 50


/*! \def NMM_FSM_MSG_BASE
 *  \brief A maro that store a integer value "NMM_FSM_MSG_PLATFORM_BASE + 2" ,represent the base value for NMM FSM messages. 
 */
#define NMM_FSM_MSG_BASE     (NMM_FSM_MSG_PLATFORM_BASE + 2)

/*! \enum phy_nmm_msg_id_e 
 *  \brief An enum that specifies the Phy NMM message id's.
 */

typedef enum
{
    SON_PHY_RSSI_MEAS_IND = NMM_FSM_MSG_BASE, /*!<  Message id 52 */
    SON_PHY_RSSI_MEAS_CNF,                    /*!<  Message id 53 */
    SON_PHY_CELL_SEARCH_IND,                  /*!<  Message id 54 */
    SON_PHY_CELL_SEARCH_CNF,                  /*!<  Message id 55 */
    SON_PHY_PBCH_CONFIG_CNF,                  /*!<  Message id 56 */
    SON_PHY_PBCH_DATA_IND,                    /*!<  Message id 57 */
    SON_PHY_SIB1_CONFIG_CNF,                  /*!<  Message id 58 */
    SON_PHY_BCCH_CONFIG_CNF,                  /*!<  Message id 59 */
    SON_PHY_BCCH_DATA_IND,                    /*!<  Message id 60 */
    SON_PHY_RSSI_MEAS_REQ,                    /*!<  Message id 61 */
    SON_PHY_STOP_MEAS_REQ,                    /*!<  Message id 62 */
    SON_PHY_CELL_SEARCH_REQ,                  /*!<  Message id 63 */
    SON_PHY_STOP_CELL_SEARCH_REQ,             /*!<  Message id 64 */
    SON_PHY_PBCH_CONFIG_REQ,                  /*!<  Message id 65 */
    SON_PHY_STOP_PBCH_REQ,                    /*!<  Message id 66 */
    SON_PHY_SIB1_CONFIG_REQ,                  /*!<  Message id 67 */
    SON_PHY_BCCH_CONFIG_REQ,                  /*!<  Message id 68 */
    SON_PHY_STOP_BCCH_REQ,                    /*!<  Message id 69 */
/*+ SPR 18399*/
    SON_PHY_MAX_NUM_MSG_ID = SON_PHY_STOP_BCCH_REQ - NMM_FSM_MSG_BASE + 1  /*!<  Message id 18 */
/*- SPR 18399*/
}phy_nmm_msg_id_e;

/*! \fn void son_phy_construct_interface_api_header ( S8 *p_header, U8 msg_id, U8 vender_spc_length, U16 length )
 *  \brief                      This function contructs the api header.
 *  \param *p_header            The pointer to the header.
 *  \param msg_id               Represent the message id
 *  \param vender_spc_length    vender specific length
 *  \param length               The length of the message.
 */

void
son_phy_construct_interface_api_header
(
     U8 *p_header,
     U8 msg_id,
     U8 vender_spc_length,
     U16 length
);

/*! \enum nmm_fsm_event_e
 *  \brief An enum that specifies the NMM FSM events.
 */
typedef enum 
{
    NMM_START_SCAN_EVENT = SONNMM_START_SCAN_REQ  - NMM_FSM_EX_MSG_BASE,            /*!<  NMM FSM event 0 */
    NMM_STOP_SCAN_EVENT,                                                            /*!<  NMM FSM event 1 */
    NMM_RSSI_MEAS_IND_EVENT = SON_PHY_RSSI_MEAS_IND - NMM_FSM_MSG_BASE + 2,         /*!<  NMM FSM event 2 */
    NMM_RSSI_MEAS_CNF_EVENT ,                                                       /*!<  NMM FSM event 3 */
    NMM_CELL_SEARCH_IND_EVENT,                                                      /*!<  NMM FSM event 4 */
    NMM_CELL_SEARCH_CNF_EVENT,                                                      /*!<  NMM FSM event 5 */
    NMM_PBCH_CONFIG_CNF_EVENT,                                                      /*!<  NMM FSM event 6 */
    NMM_PBCH_DATA_IND_EVENT,                                                        /*!<  NMM FSM event 7 */
    NMM_SIB1_CONFIG_CNF_EVENT,                                                      /*!<  NMM FSM event 8 */
    NMM_BCCH_CONFIG_CNF_EVENT,                                                      /*!<  NMM FSM event 9 */
    NMM_BCCH_DATA_IND_EVENT,                                                        /*!<  NMM FSM event 10 */
    MAX_NUM_EVENT                                                                   /*!<  NMM FSM event 11 */
}nmm_fsm_event_e;

#endif
