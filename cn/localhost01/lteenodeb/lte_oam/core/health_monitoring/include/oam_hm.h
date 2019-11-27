/*****************************************************************************
 * File Name       : oam_hm.h
 *
 * Description     : This file contains includes required for Health Monitoring
 *
 * Revision History:
 *
 * Date           Author           Reference            Comments
 * -----          ------           ---------            --------
 * April,2012     Nitin Agarwal   OAM Design Document   Initial Version                
 * September,2013 Neeraj Kumar    OAM Design Doc        Changes related to HM
 *
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 ****************************************************************************/

#ifndef _OAM_HM_H_
#define _OAM_HM_H_

/****************************************************************************
 **  INCLUDES
 ****************************************************************************/

#include <oam_sim.h>
#include <oam_proto.h>
#include <oam_son_composer.h>

/****************************************************************************
 **  MACROS
 ****************************************************************************/
/*SPR 21369 Start*/
#define OAM_MAC_RESP_RECVD_MASK            0x01     
#define OAM_RLC_RESP_RECVD_MASK            0x02
#define OAM_PDCP_RESP_RECVD_MASK           0x04
#define OAM_ALL_RESP_RECVD_FOR_L2          0x07

#define OAM_RRM_RESP_RECVD_MASK            0x01  
#define OAM_SON_RESP_RECVD_MASK            0x02  
#define OAM_RRC_RESP_RECVD_MASK            0x04  
#define OAM_X2AP_RESP_RECVD_MASK           0x08  
#define OAM_S1AP_RESP_RECVD_MASK           0x10  
#define OAM_EGTPU_RESP_RECVD_MASK          0x20

#ifdef OAM_SON_ENABLE
#define OAM_ALL_RESP_RECVD                 0x03F  
#else
#define OAM_ALL_RESP_RECVD                 0x03D
#endif
/*SPR 21369 End*/

#define OAM_HM_ALARM_TEXT_SIZE 		   256 
/****************************************************************************
 **  ENUMERATIONS / STRUCTURES
 ****************************************************************************/
/*It defines Health Status for various modules*/
/*SPR 21369 Start*/
typedef struct
{
  UInt8 l2_send_flag;
  UInt8 l2_response_bitmask;  
}oam_l2_health_ping_resp_t;
/*SPR 21369 End*/
typedef struct
{
    UInt16 periodic_timer_val;                                     /* It is used for the time interval after 
                                                             which the health monitoring request is sent */
    UInt16 non_responsive_timer_val;
    /*SPR 21369 Start*/
    oam_bool_et l3_health_mon_flag;
    UInt8 enb_recieved_resp_bitmask;
    oam_l2_health_ping_resp_t oam_l2_health_ping_resp[OAM_MAX_NUM_L2_INSTANCE];
    /*SPR 21369 End*/
    UInt8  retry_count;
    oam_timer_t hm_periodic_timer;
    oam_timer_t hm_non_responsive_timer;
}oam_hm_context_t;

/****************************************************************************
 **  FUNCTION PROTOTYPES
 ****************************************************************************/
/* + 17439 */
oam_return_et
oam_health_monitoring_init(
void
);
/* - 17439 */

/* + 17439 */
void
oam_start_hm(
void
);
/* - 17439 */

oam_return_et
oam_hm_timer_expiry
(
    /*SPR 17777 fix*/
    void
);

oam_return_et 
oam_hm_call_system_state_change
(
    void
);
/* + 17439 */
oam_return_et 
oam_check_health_req_to_layers(
void
);
/* - 17439 */

void
oam_hm_recieved_resp_bitmask_update
(
   /*SPR 21369 Start*/
   void *p_api_buf
   /*SPR 21369 End*/
);
/* + 17439 */
oam_return_et
oam_send_check_health_req_to_rrm(
void
);
/* - 17439 */

oam_return_et
oam_send_check_health_req
(
    UInt16 api_id,
    UInt16 transaction_id,
    oam_module_id_t des_module,
    oam_error_code_et *p_error_code
);

/* + 17439 */
/*SPR 21369 Start */
oam_return_et
oam_send_check_health_req_for_l2
(
    UInt16 api_id,
    UInt16 transaction_id,
    oam_module_id_t des_module,
    oam_error_code_et *p_error_code,
    UInt8 l2_instance
);
/*SPR 21369 End */

void
oam_send_check_health_req_to_all(
void
);
/* - 17439 */

/* + 17439 */
void
oam_send_hm_req_to_layers(
void
);
/* - 17439 */
#endif /* _OAM_HM_H_ */

