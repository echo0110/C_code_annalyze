/*****************************************************************************
**
**  FILE   :
**         File Having Signal Handler
**
******************************************************************************
**
**  FILE NAME :
**         sig_hand.c
**
**  DESCRIPTION :
**           This file contains Signal Handler Management.
**   DATE           NAME               REFERENCE         REASON
**   ----           ----               ---------         ------
**   20Dec,2002    Manish S          ---           Release 4.0 update
**
**   Copyright (C) 2006 Aricent Inc . All Rights Reserved
*****************************************************************************/

#include "sig_os.h"
#include "signal.h"


/* Signal Handler for Stack */
sigtran_void_t ( *g_app_sig_handler )( sigtran_U16bit ) = NULL;

/******************************************************************************
**  FUNCTION    : sigtran_sig_handler
**
**  DESCRIPTION : Default Signal Handler provided by the sigtran Stack for
**                Exception Handling.
**
**  RETURNS     : Nothing
******************************************************************************/
void sigtran_sig_handler( int sig )
{
    signal( SIGSEGV, NULL );
    signal( SIGBUS, NULL );
    signal( SIGILL, NULL );

    /* Generate a Core File */
    abort();

    /* Call the Application Handler for Cleanup */
    if( g_app_sig_handler != NULL )
    {
        g_app_sig_handler( sig );
    }
}

/******************************************************************************
**  FUNCTION    : sigtran_install_sig_handler
**
**  DESCRIPTION : Interface for the System to add a handler to be called after
**                default exception handling is complete.
**
**  RETURNS     : Nothing
******************************************************************************/
sigtran_return_t sigtran_install_sig_handler(
    sigtran_void_t ( *sig_handler )( sigtran_U16bit ),
    sigtran_error_t   *p_ecode )
{
    g_app_sig_handler = sig_handler;

    /* Install Signal Handler */
    signal( SIGSEGV, sigtran_sig_handler );
    signal( SIGBUS, sigtran_sig_handler );
    signal( SIGILL, sigtran_sig_handler );

    return SIGTRAN_SUCCESS;
}
