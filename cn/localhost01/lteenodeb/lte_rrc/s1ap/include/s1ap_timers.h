/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: s1ap_timers.h,v 1.3 2010/03/24 09:50:45 gur18569 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This files contains all the declarations for Timer
 *                     buffers.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: s1ap_timers.h,v $
 * Revision 1.3  2010/03/24 09:50:45  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.3  2009/12/28 05:09:42  gur18569
 * fixed indentation
 *
 * Revision 1.1.2.2  2009/12/27 08:32:51  gur18569
 * removed tabs
 *
 * Revision 1.1.2.1  2009/11/25 13:11:02  gur18569
 * Shifted to level of rrc dir
 *
 * Revision 1.1.2.3  2009/11/23 15:04:10  gur18569
 * changed S1AP_TIMER_OAM_INIT_IND_DEFAULT_DURATION to 3 secs
 *
 * Revision 1.1.2.2  2009/10/23 16:11:39  gur18569
 * Initial version
 *
 * Revision 1.1.2.1  2009/10/15 08:41:09  gur18569
 * Initial revision::
 *
 *
 *
 ****************************************************************************/

#ifndef _S1AP_TIMERS_H_
#define _S1AP_TIMERS_H_

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
#define  S1AP_TIMER_OAM_INIT_IND_DEFAULT_DURATION       3000 /* in ms */
#define  S1AP_TIMER_S1_SETUP_RESP_DEFAULT_DURATION      1000 /* in ms */
#define  S1AP_TIMER_RESET_RESP_DEFAULT_DURATION         1000 /* in ms */
#define  S1AP_TIMER_CONN_RECOVERY_DEFAULT_DURATION      7000 /* in ms */
#define  S1AP_TIMER_PWS_DEFAULT_DURATION                3000 /* in ms */
#define  S1AP_TIMER_KILL_DEFAULT_DURATION               3000 /* in ms */


/****************************************************************************
 * Exported Types
 ****************************************************************************/

typedef enum 
{
    /* Timer started of OAM Init Ind
     */
    S1AP_OAM_INIT_TIMER, 

    /* Timer started for S1 setup request
     */
    S1AP_MME_S1_SETUP_RSP_TIMER, 

    /* Timer started upon reception of Connection
     * failure indication from SCTP.
     */
    S1AP_MME_SCTP_CONN_REC_TIMER,     

    /* Timer started upon failure to open a
     * SCTP association
     */
    S1AP_MME_SCTP_CONNECT_TIMER,

    /* Timer started for RESET procedure
     */
    S1AP_MME_RESET_TIMER,

    /* TImer started upon reception of time to wait IE in
     * S1 SETUP FAILURE
     */
    S1AP_MME_TIME_TO_WAIT_TIMER,

    /* Timer started for eNB configuration update response
     */
    S1AP_ENB_CONFIGURAION_UPDATE_TIMER,

    /* Timer started upon reception of time to wait IE in
     * eNB Configuration update failure message
     */
    S1AP_ENB_CONFIGURATION_TIME_TO_WAIT_TIMER,

    /* Timer started for Write Replace warning Procedure
     */
    S1AP_PWS_GUARD_TIMER,

    /* Timer started for KILL Request  Procedure
     */
    S1AP_KILL_GUARD_TIMER,

    /* Timer started for Health monitor  Procedure
     */
  /*SPR 15570 Fix Start*/
    S1AP_OAM_HEALTH_MONITOR_TIMER,
    /*Timer started for ongoing SCTP procedure on receiving error EINPROGRESS*/
    S1AP_SCTP_CONN_TIMER
  /*SPR 15570 Fix Stop*/


}s1ap_timer_id_et;

#pragma pack(push, 1)

typedef struct _s1ap_timer_buffer_t
{
    /* It identifies the type of request for which a timer is
     * started
     */
    s1ap_timer_id_et timer_type;

    /* This can be used to maintain any timer specific data*/
    void * p_timer_specific_data;

    /* If true timer buff is freed by timer handler
     */
    rrc_bool_et             auto_delete;

    /* MME index corresponding to which a timer is started.
     */
    U8 mme_id;
}s1ap_timer_buffer_t;

#pragma pack(pop)


/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#endif /* _S1AP_TIMERS_H_ */

