/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2011 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: signal_handler.h,v  2011/01/27 06:00:00 gur19170 Exp $
 ****************************************************************************
 *
 *  File Description :
 *  This is the header file of signal_handler framework includes
 *  data structure used for signal_handler
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: signal_handler.h,v $
 * Initial version
 *
 ****************************************************************************/
#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H



typedef void (*sig_handler)(int);

/*
 * This function is to register signal handler for a signal
 */
UInt32 register_signal_handler(
        UInt32 sig_num,
        sig_handler handler);

/*
 * This function is used to register a signal handler for all
 * defined signal.
 */
/* + SPR 17439 */
UInt32 register_all_signal_handler(void);
/*
 * Used to modify core path.
 */
void modify_core_path(void);
/* - SPR 17439 */
#endif
