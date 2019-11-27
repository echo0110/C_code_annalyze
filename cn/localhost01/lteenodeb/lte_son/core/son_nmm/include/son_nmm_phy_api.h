/*! \file son_nmm_phy_api.h
 *  \brief This file contains api between NMM and various hardwares.
 *  \date January 02, 2012
 *  \author gur26210
 */

/*!
 *  \copyright {Copyright (c) 2009, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_NMM_PHY_API_H_
#define _SON_NMM_PHY_API_H_

#ifdef __SON_FAPI__

/*! \headerfile son_nmm_fapi.h<>
 */
#include <son_nmm_fapi.h>

/*! \headerfile son_fapi_intf.h <>
 */

#include <son_fapi_intf.h>

/*! \headerfile son_fapi_composer.h <>
 */

#include <son_fapi_composer.h>

/*! \headerfile son_fapi_parser.h <>
 */

#include <son_fapi_parser.h>

#endif /* ifdef __SON_FAPI__ */

/* SPR 22925 Fix - */

/*! \fn extern son_return_et son_phy_send_son_phy_rssi_meas_req( son_phy_rssi_meas_req_t  *p_son_phy_rssi_meas_req)
 *  \brief This function sends the rssi measurement request to phy.
 *  \param *p_son_phy_rssi_meas_req  It is a variable of the structure "son_phy_rssi_meas_req_t".
 */

extern son_return_et
son_phy_send_son_phy_rssi_meas_req
(
    son_phy_rssi_meas_req_t  *p_son_phy_rssi_meas_req
);

/*! \fn extern son_return_et son_phy_send_son_phy_cell_search_req(son_phy_cell_search_req_t  *p_son_phy_cell_search_req)
 *  \brief This function sends the cell search request to phy.
 *  \param *p_son_phy_cell_search_req It is a variable of the structure "son_phy_cell_search_req_t".
 */

extern son_return_et
son_phy_send_son_phy_cell_search_req
(
    son_phy_cell_search_req_t  *p_son_phy_cell_search_req
);

/*! \fn extern son_return_et son_phy_send_son_phy_pbch_config_req(son_phy_pbch_config_req_t  *p_son_phy_pbch_config_req)
 *  \brief This function sends the pbch configuration request to phy.
 *  \param *p_son_phy_pbch_config_req It is a variable of the structure "son_phy_pbch_config_req_t".
 */

extern son_return_et
son_phy_send_son_phy_pbch_config_req
(
    son_phy_pbch_config_req_t  *p_son_phy_pbch_config_req
);

/*! \fn extern son_return_et son_phy_send_son_phy_sib1_config_req(son_phy_sib1_config_req_t  *p_son_phy_sib1_config_req)
 *  \brief This function sends the sib1 configuration request to phy.
 *  \param *p_son_phy_sib1_config_req It is a variable of the structure "son_phy_sib1_config_req_t".
 */

extern son_return_et
son_phy_send_son_phy_sib1_config_req
(
    son_phy_sib1_config_req_t  *p_son_phy_sib1_config_req
);

/*! \fn extern son_return_et son_phy_send_son_phy_bcch_config_req(son_phy_bcch_config_req_t  *p_son_phy_bcch_config_req)
 *  \brief This function sends the bcch configuration request to phy.
 *  \param *p_son_phy_bcch_config_req It is a variable of the structure "son_phy_bcch_config_req_t".
 */

extern son_return_et
son_phy_send_son_phy_bcch_config_req
(
    son_phy_bcch_config_req_t  *p_son_phy_bcch_config_req
);

/*! \fn extern son_return_et son_phy_send_son_phy_stop_rssi_meas_req(son_phy_stop_rssi_meas_req_t  *p_son_phy_stop_rssi_meas_req)
 *  \brief This function sends the stop rssi measurement request to phy.
 *  \param *p_son_phy_stop_rssi_meas_req It is a variable of the structure "son_phy_stop_rssi_meas_req_t".
 */

extern son_return_et
son_phy_send_son_phy_stop_rssi_meas_req
(
    son_phy_stop_rssi_meas_req_t  *p_son_phy_stop_rssi_meas_req
);

/*! \fn extern son_return_et son_phy_send_son_phy_stop_cell_search_req(son_phy_stop_cell_search_req_t  *p_son_phy_stop_cell_search_req)
 *  \brief This function sends the stop cell search request to phy.
 *  \param *p_son_phy_stop_cell_search_req It is a variable of the structure "son_phy_stop_cell_search_req_t".
 */

extern son_return_et
son_phy_send_son_phy_stop_cell_search_req
(
    son_phy_stop_cell_search_req_t  *p_son_phy_stop_cell_search_req
);

/*! \fn extern son_return_et son_phy_send_son_phy_stop_pbch_req(son_phy_stop_pbch_req_t  *p_son_phy_stop_pbch_req)
 *  \brief This function sends the stop pbch request to phy.
 *  \param *p_son_phy_stop_pbch_req It is a variable of the structure "son_phy_stop_pbch_req_t".
 */

extern son_return_et
son_phy_send_son_phy_stop_pbch_req
(
    son_phy_stop_pbch_req_t  *p_son_phy_stop_pbch_req
);

/*! \fn extern son_return_et son_phy_send_son_phy_stop_bcch_req(son_phy_stop_bcch_req_t  *p_son_phy_stop_bcch_req)
 *  \brief This function sends the stop bcch request to phy.
 *  \param *p_son_phy_stop_bcch_req It is a variable of the structure "son_phy_stop_bcch_req_t".
 */

extern son_return_et
son_phy_send_son_phy_stop_bcch_req
(
    son_phy_stop_bcch_req_t  *p_son_phy_stop_bcch_req
);

#endif /* ifndef _SON_NMM_PHY_API_H_ */
