/****************************************************************************
*
* ARICENT -
*
* Copyright (C) 2012 Aricent Inc. All Rights Reserved.
*
****************************************************************************
*
*
****************************************************************************
*
* File Description: Contains function for nmm fsm
*
****************************************************************************
* Revision Details
* ----------------
*
* Revision 0.1  2012/01/02 09:50:42  gur26210
* Initial code
*
****************************************************************************/

/****************************************************************************
*                  Standard Library Includes
****************************************************************************/

/****************************************************************************
*                  Project Includes
****************************************************************************/

#include <son_nmm_phy_api.h>

/****************************************************************************
*                  Private Definitions
****************************************************************************/

/*****************************************************************************
*                  global variables
*****************************************************************************/

/****************************************************************************
*                  Exported variables
****************************************************************************/

/***************************************************************************
 *                  Private Types
 ****************************************************************************/

typedef son_return_et (*son_nmm_phy_rssi_meas_req_t) \
    (
    son_phy_rssi_meas_req_t *,
    U16
    );
typedef son_return_et (*son_nmm_phy_cell_search_req_t) \
    (
    son_phy_cell_search_req_t *,
    U16
    );
typedef son_return_et (*son_nmm_phy_pbch_config_req_t) \
    (
    son_phy_pbch_config_req_t *,
    U16
    );
typedef son_return_et (*son_nmm_phy_sib1_config_req_t) \
    (
    son_phy_sib1_config_req_t *,
    U16
    );
typedef son_return_et (*son_nmm_phy_bcch_config_req_t) \
    (
    son_phy_bcch_config_req_t *,
    U16
    );
typedef son_return_et (*son_nmm_phy_stop_rssi_meas_req_t) \
    (
    son_phy_stop_rssi_meas_req_t *,
    U16
    );
typedef son_return_et (*son_nmm_phy_stop_cell_search_req_t) \
    (
    son_phy_stop_cell_search_req_t *,
    U16
    );
typedef son_return_et (*son_nmm_phy_stop_pbch_req_t) \
    (
    son_phy_stop_pbch_req_t  *,
    U16
    );
typedef son_return_et (*son_nmm_phy_stop_bcch_req_t) \
    (
    son_phy_stop_bcch_req_t *,
    U16
    );

#ifdef __SON_FAPI__

son_nmm_phy_rssi_meas_req_t son_nmm_phy_rssi_meas_req = \
    son_fapi_send_son_phy_rssi_meas_req;

son_nmm_phy_cell_search_req_t son_nmm_phy_cell_search_req = \
    son_fapi_send_son_phy_cell_search_req;

son_nmm_phy_pbch_config_req_t son_nmm_phy_pbch_config_req = \
    son_fapi_send_son_phy_pbch_config_req;

son_nmm_phy_sib1_config_req_t son_nmm_phy_sib1_config_req = \
    son_fapi_send_son_phy_sib1_config_req;

son_nmm_phy_bcch_config_req_t son_nmm_phy_bcch_config_req = \
    son_fapi_send_son_phy_bcch_config_req;

son_nmm_phy_stop_rssi_meas_req_t son_nmm_phy_stop_rssi_meas_req = \
    son_fapi_send_son_phy_stop_rssi_meas_req;

son_nmm_phy_stop_cell_search_req_t son_nmm_phy_stop_cell_search_req = \
    son_fapi_send_son_phy_stop_cell_search_req;

son_nmm_phy_stop_pbch_req_t son_nmm_phy_stop_pbch_req = \
    son_fapi_send_son_phy_stop_pbch_req;

son_nmm_phy_stop_bcch_req_t son_nmm_phy_stop_bcch_req = \
    son_fapi_send_son_phy_stop_bcch_req;

#endif /* ifdef __SON_FAPI__ */




/****************************************************************************
*                  Macro definitions
****************************************************************************/

/*****************************************************************************
 * Function Name  : son_phy_send_son_phy_rssi_meas_req
 * Inputs         : p_son_phy_rssi_meas_req
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed function processing
 *                  SON_SUCCESS     - Indicates successful function processing
 * Description    : This function sends rssi measurement request to PHY
 ****************************************************************************/
son_return_et
son_phy_send_son_phy_rssi_meas_req
(
    son_phy_rssi_meas_req_t  *p_son_phy_rssi_meas_req
)
{
    return son_nmm_phy_rssi_meas_req(p_son_phy_rssi_meas_req, SON_NMM_MODULE_ID);
}

/*****************************************************************************
 * Function Name  : son_phy_send_son_phy_cell_search_req
 * Inputs         : p_son_phy_cell_search_req
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed function processing
 *                  SON_SUCCESS     - Indicates successful function processing
 * Description    : This function sends cell search request to PHY
 ****************************************************************************/
son_return_et
son_phy_send_son_phy_cell_search_req
(
    son_phy_cell_search_req_t  *p_son_phy_cell_search_req
)
{
    return son_nmm_phy_cell_search_req(p_son_phy_cell_search_req, SON_NMM_MODULE_ID );
}

/*****************************************************************************
 * Function Name  : son_phy_send_son_phy_pbch_config_req
 * Inputs         : p_son_phy_pbch_config_req
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed function processing
 *                  SON_SUCCESS     - Indicates successful function processing
 * Description    : This function sends pbch configuration request to PHY
 ****************************************************************************/
son_return_et
son_phy_send_son_phy_pbch_config_req
(
    son_phy_pbch_config_req_t  *p_son_phy_pbch_config_req
)
{
    return son_nmm_phy_pbch_config_req(p_son_phy_pbch_config_req, SON_NMM_MODULE_ID );
}

/*****************************************************************************
 * Function Name  : son_phy_send_son_phy_sib1_config_req
 * Inputs         : p_son_phy_sib1_config_req
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed function processing
 *                  SON_SUCCESS     - Indicates successful function processing
 * Description    : This function sends sib1 configuration request to PHY
 ****************************************************************************/
son_return_et
son_phy_send_son_phy_sib1_config_req
(
    son_phy_sib1_config_req_t  *p_son_phy_sib1_config_req
)
{
    return son_nmm_phy_sib1_config_req(p_son_phy_sib1_config_req, SON_NMM_MODULE_ID );
}

/*****************************************************************************
 * Function Name  : son_phy_send_son_phy_bcch_config_req
 * Inputs         : p_son_phy_bcch_config_req
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed function processing
 *                  SON_SUCCESS     - Indicates successful function processing
 * Description    : This function sends bcch configuration request to PHY
 ****************************************************************************/
son_return_et
son_phy_send_son_phy_bcch_config_req
(
    son_phy_bcch_config_req_t  *p_son_phy_bcch_config_req
)
{
    return son_nmm_phy_bcch_config_req(p_son_phy_bcch_config_req, SON_NMM_MODULE_ID );
}

/*****************************************************************************
 * Function Name  : son_phy_send_son_phy_stop_rssi_meas_req
 * Inputs         : p_son_phy_stop_rssi_meas_req
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed function processing
 *                  SON_SUCCESS     - Indicates successful function processing
 * Description    : This function sends stop rssi measurement request to PHY
 ****************************************************************************/
son_return_et
son_phy_send_son_phy_stop_rssi_meas_req
(
    son_phy_stop_rssi_meas_req_t  *p_son_phy_stop_rssi_meas_req
)
{
    return son_nmm_phy_stop_rssi_meas_req(p_son_phy_stop_rssi_meas_req, SON_NMM_MODULE_ID);
}

/*****************************************************************************
 * Function Name  : son_phy_send_son_phy_stop_cell_search_req
 * Inputs         : p_son_phy_stop_cell_search_req
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed function processing
 *                  SON_SUCCESS     - Indicates successful function processing
 * Description    : This function sends stop cell search request to PHY
 ****************************************************************************/
son_return_et
son_phy_send_son_phy_stop_cell_search_req
(
    son_phy_stop_cell_search_req_t  *p_son_phy_stop_cell_search_req
)
{
    return son_nmm_phy_stop_cell_search_req(p_son_phy_stop_cell_search_req, SON_NMM_MODULE_ID);
}

/*****************************************************************************
 * Function Name  : son_phy_send_son_phy_stop_pbch_req
 * Inputs         : p_son_phy_stop_pbch_req
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed function processing
 *                  SON_SUCCESS     - Indicates successful function processing
 * Description    : This function sends stop pbch request to PHY
 ****************************************************************************/
son_return_et
son_phy_send_son_phy_stop_pbch_req
(
    son_phy_stop_pbch_req_t  *p_son_phy_stop_pbch_req
)
{
    return son_nmm_phy_stop_pbch_req(p_son_phy_stop_pbch_req, SON_NMM_MODULE_ID);
}

/*****************************************************************************
 * Function Name  : son_phy_send_son_phy_stop_bcch_req
 * Inputs         : p_son_phy_stop_bcch_req
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed function processing
 *                  SON_SUCCESS     - Indicates successful function processing
 * Description    : This function sends stop bcch request to PHY
 ****************************************************************************/
son_return_et
son_phy_send_son_phy_stop_bcch_req
(
    son_phy_stop_bcch_req_t  *p_son_phy_stop_bcch_req
)
{
    return son_nmm_phy_stop_bcch_req(p_son_phy_stop_bcch_req, SON_NMM_MODULE_ID);
}
