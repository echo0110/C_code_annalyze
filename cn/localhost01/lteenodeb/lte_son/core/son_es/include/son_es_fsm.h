/*! \file son_es_fsm.h
 *  \brief This file contains the functions prototype for handling ES Module
 *         State Machine
 *  \date  Feb, 2012
 *  \author Atul/Reetesh
 */

/*!
 *   \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_ES_FSM_H_
#define _SON_ES_FSM_H_

/*! \fn son_buf_retain_status_et es_shutdown_handler (son_u8 *p_msg,STACKAPIHDR *p_cspl_hdr)
 *  \brief This is the message handler for ES module and is called
 *         when SMIF_SHUTDOWN_REQ message is received.
 *  \param p_msg       Pointer to the message received
 *  \param p_cspl_hdr Pointer to  the STACKAPIHDR structure
 */
son_buf_retain_status_et
es_shutdown_handler
(
    son_u8 *p_msg,
    STACKAPIHDR *p_cspl_hdr
);

/*! \fn son_buf_retain_status_et es_init_state_handler (son_u8 *p_msg,STACKAPIHDR *p_cspl_hdr)
 *  \brief This is the message handler for ES module and is called for
 *         any message received in INIT State
 *  \param p_msg       Pointer to the message received
 *  \param p_cspl_hdr  Pointer to  the STACKAPIHDR structure
 */
son_buf_retain_status_et
es_init_state_handler
(
    son_u8 *p_msg,
    STACKAPIHDR *p_cspl_hdr
);

/*! \fn son_buf_retain_status_et es_active_state_handler (son_u8 *p_msg,STACKAPIHDR *p_cspl_hdr)
 *  \brief This is the message handler for ES module and is
 *         called for any message received in state other than ACTIVE.
 *  \param p_msg       Pointer to the message received
 *  \param p_cspl_hdr  Pointer to  the STACKAPIHDR structure
 */
son_buf_retain_status_et
es_active_state_handler
(
    son_u8 *p_msg,
    STACKAPIHDR *p_cspl_hdr
);

#endif /* _SON_ES_FSM_H_ */
