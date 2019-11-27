/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: ipr_cwmp_sig_handler.c $
 *
 *******************************************************************************/

#include<ipr_cwmp_sig_handler.h>

void sig_hdl(void)
{
#ifdef HAVE_IPSEC
     ENB_PRINTF ("Entering sig_hdl");
    have_tunnel = 0;
     ENB_PRINTF ("Extiing sig_hdl");
#endif
}
