/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: s1ap_handlers.h,v 1.3 2010/03/24 09:50:45 gur18569 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains declarations of message,timer and
 *                     init handlers that are registered with CSPL for S1AP
 *                     module.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: s1ap_handlers.h,v $
 * Revision 1.3  2010/03/24 09:50:45  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.2  2009/12/28 05:09:42  gur18569
 * fixed indentation
 *
 * Revision 1.1.2.1  2009/11/25 13:11:02  gur18569
 * Shifted to level of rrc dir
 *
 * Revision 1.1.2.1  2009/10/23 16:11:39  gur18569
 * Initial version
 *
 *
 *
 ****************************************************************************/

#ifndef _S1AP_HANDLERS_H_
#define _S1AP_HANDLERS_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "rrc_defines.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
void* s1ap_init
(
 /* Pointer to S1AP stack initialization data. Unused */
 void *p_s1ap_init_data
 );

S32 s1ap_process_msg
(
 void *p_api,        /* Pointer to input API buffer */
 void *p_gl_ctx      /* Pointer to the S1AP global context data */
 );

void s1ap_process_timer_msg
(
 rrc_timer_t timer_id,       /* Identity of the expired timer */
 void        *p_timer_buf,   /* Pointer to data associated with the timer */
 void        *p_gl_ctx       /* Pointer to the S1AP global context data */
 );

#endif  /* _S1AP_HANDLERS_H_ */
