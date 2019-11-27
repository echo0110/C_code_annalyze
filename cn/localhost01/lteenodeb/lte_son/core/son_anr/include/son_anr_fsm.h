/*! \file son_anr_fsm.h
 *  \brief This file contains the functions prototype for handling ANR Module
 *         State Machine
 *  \date  Feb, 2012
 *  \author Komal/Shekhar
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_ANR_FSM_H_
#define _SON_ANR_FSM_H_

/*! \headerfile son_defines.h <>
 */
#include <son_defines.h>

/*! \headerfile son_anr_intf.h <>
 */
#include <son_anr_intf.h>

/*! \fn son_buf_retain_status_et anr_shutdown_handler (son_u8 *p_msg,STACKAPIHDR *p_cspl_info)
 *  \brief This is the message handler for ANR module and is called
 *         when SMIF_SHUTDOWN_REQ message is received.
 *  \param p_msg       Pointer to the message received
 *  \param p_cspl_info Pointer to  the CSPL header Information.
 */
son_buf_retain_status_et
anr_shutdown_handler
(
    son_u8 *p_msg
);

/*! \fn son_buf_retain_status_et anr_init_state_handler (son_u8 *p_msg,STACKAPIHDR *p_cspl_hdr)
 *  \brief This is the message handler for ANR module and is called for
 *         any message received in INIT State
 *  \param p_msg       Pointer to the message received
 *  \param p_cspl_hdr  Pointer to the CSPL Header information.
 */
son_buf_retain_status_et
anr_init_state_handler
(
    son_u8 *p_msg,
    STACKAPIHDR *p_cspl_hdr
);

/*! \fn son_buf_retain_status_et anr_active_state_handler (son_u8 *p_msg,STACKAPIHDR *p_cspl_info)
 *  \brief This is the message handler for ANR module and is
 *         called for any message received in state  other than INIT and SHUTDOWN.
 *  \param p_msg       Pointer to the message received
 *  \param p_cspl_info  Pointer to the CSPL Header Information.
 */
son_buf_retain_status_et
anr_active_state_handler
(
    son_u8 *p_msg,
    STACKAPIHDR *p_cspl_info
);

#endif /* ifndef _SON_ANR_FSM_H_ */
