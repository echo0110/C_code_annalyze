/*! \file son_apps_fsm.h
 *  \brief This file contains the states events and definitions for SON
 *         APPS FSM.
 *  \date February 19, 2012
 *  \author Shilpi Jain
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_APPS_FSM_H_
#define _SON_APPS_FSM_H_

/*! \headerfile son_apps_common_intf.h <>
 */
#include <son_apps_common_intf.h>

/*! \def SONAPPS_NUM_OF_STATES
 *  \brief This macro stores the number of SONApps FSM states
 */
#define SONAPPS_NUM_OF_STATES 3

/*! \def CELL_INFO_LIST_ENQUEUE(p_cell_info_data, p_cell_info_list)
 *  \brief This macro is used to push an element at the end of cell info list
 */
#define CELL_INFO_LIST_ENQUEUE(p_cell_info_data, \
                               p_cell_info_list) \
    ylPushTail( p_cell_info_list, \
    &(p_cell_info_data->cell_node))

/*! \def EARFCN_SCAN_LIST_ENQUEUE(p_earfcn_data, p_earfcn_list)
 *  \brief This macro is used to push an element at the end of earfcn scan res
 *  list
 */
#define EARFCN_SCAN_LIST_ENQUEUE(p_earfcn_data, \
                                 p_earfcn_list) \
    ylPushTail(p_earfcn_list, &(p_earfcn_data->earfcn_node))

/*! \enum son_apps_index_et
 *  \brief An enum that specifies the index value in SONApps FSM
 */
typedef enum
{
    APPS_IDX_SMIF_EVENT_PCI_ENABLE_DISABLE_REQ = 0,
    APPS_IDX_SONANR_EVENT_PCI_REG_DEREG_RES,
    APPS_IDX_SMIF_EVENT_PCI_SELECTION_REQ,
    APPS_IDX_SMIF_EVENT_PCI_RESELECTION_REQ,
    APPS_IDX_SONANR_EVENT_PCI_CONFLICT_CONFUSION_IND,
    APPS_IDX_SONANR_EVENT_PCI_IN_USE_LIST_RES,
    SONAPPS_PCI_NUM_OF_EVENTS,

    APPS_IDX_SMIF_EVENT_INIT_CONFIG_REQ = SONAPPS_PCI_NUM_OF_EVENTS,
    APPS_IDX_SMIF_EVENT_LOG_ENABLE_CMD,
    APPS_IDX_SMIF_EVENT_SET_LOG_LEVEL_CMD,
    APPS_IDX_SONAPPS_EVENT_NW_SCAN_REQ,
    APPS_IDX_SONAPPS_EVENT_PERIODIC_NW_SCAN_REQ,
    APPS_IDX_SONNMM_EVENT_START_SCAN_RES,
    APPS_IDX_SMIF_EVENT_SHUTDOWN_REQ,

    APPS_IDX_SONAPPS_EVENT_CARRIER_FREQ_SELECTION_REQ,
    /* Rel 1.4 Start */
    APPS_IDX_SONAPPS_EVENT_CARRIER_FREQ_DL_PARAMS_REQ,
    /* Rel 1.4 Stop */
    /* SPR-13251 Fix Starts */
    APPS_IDX_SONAPPS_EVENT_GET_LOG_LEVEL_REQ,
    /* SPR-13251 Fix Ends */

    SONAPPS_NUM_OF_EVENTS,

    APPS_IDX_NOT_AVAILABLE  = 0xFF
}son_apps_index_et;

/*! \a decl p_son_apps_facility_name
 *  \brief Variable to store SONApps facility name
 */
extern const son_8 *p_son_apps_facility_name;

/*! \a decl SON_APPS_FSM_STATES_NAMES
 *  \brief Pointer to array storing SONApps FSM state names
 */
extern const son_s8 *SON_APPS_FSM_STATES_NAMES[];

/*! \a decl sonapps_fsm_name
 *  \brief  Pointer to array used to specify SONApps FSM name
 */
extern const son_8 *sonapps_fsm_name;

/*! \fn son_s32 son_apps_process_msg(son_void_t *p_msg, son_void_t *p_cxt)
 *  \brief This function is the entry point for SON Apps module and is used
 *         to process different events.
 *  \param p_msg       Pointer to the message received
 *  \param p_cxt       Pointer to the SONApps global context data (Unused)
 */

S32
son_apps_process_msg
(
    son_void_t *p_msg,
    son_void_t *p_cxt
);

/*! \fn son_void_t * son_apps_init(son_void_t *p_son_apps_init_data)
 *  \brief This function is used to initialize the SON Apps module.
 *  \param p_son_apps_init_data     Pointer to Apps specific context data
 *                                  (Unused)
 */
son_void_t *
son_apps_init
(
    son_void_t *p_son_apps_init_data
);

/*! \fn son_buf_retain_status_et son_apps_process_event(son_void_t *p_msg,
 *   son_apps_gb_context_t *p_sonapps_gb_context)
 *  \brief This function is used to invoke the appropriate event handler
 *         based on the API Id and current FSM state of SONApps.
 *  \param p_msg                Pointer to the message received
 *  \param p_sonapps_gb_context Pointer to global context structure
 */
son_buf_retain_status_et
son_apps_process_event
(
    son_void_t *p_msg,
    son_apps_gb_context_t *p_sonapps_gb_context
);

/*! \fn son_void_t son_free_nmm_scan_res_msg_mem(nmm_scan_res_list_t
 * p_nmm_scan_res_list)
 *  \brief This function is used to clean up the memory used by
 *      scan resp list (received from SONNmm) in case of internal failure.
 *  \param p_nmm_scan_res_list  Pointer to the scan resp lis received from
 *                              SONNmm
 */
son_void_t
son_free_nmm_scan_res_msg_mem
(
    nmm_scan_res_list_t *p_nmm_scan_res_list
);

/*! \fn son_void_t son_apps_reset_context(son_apps_gb_context_t
 * p_sonapps_gb_context)
 *  \brief This function is used to reset SONApps global context structure
 *  \param p_sonapps_gb_context  Pointer to the global context structure
 */
son_void_t
son_apps_reset_context
(
    son_apps_gb_context_t *p_sonapps_gb_context
);

/*! \fn son_u8 calculate_apps_msg_index(son_u16 from, son_u16
 *  api, son_apps_gb_context_t *p_sonapps_gb_context)
 *  \brief  This function computes the index in the SONApps FSM for the
 *          given procedure code
 *  \param  from                  Source module id
 *  \param  api                   Procedure code of the message
 *  \param  p_sonapps_gb_context  Pointer to the global context structure
 */
son_u8
calculate_apps_msg_index
(
    son_u16 from,
    son_u16 api,
    son_apps_gb_context_t   *p_sonapps_gb_context
);

#endif /* _SON_APPS_FSM_H_ */
