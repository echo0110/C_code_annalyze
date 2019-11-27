/******************************************************************************
 * *
 * *   FILE NAME   : rrc_x2apS1ap_intf.h
 * *
 * *   DESCRIPTION : This file contains basic X2AP-S1AP interface definitions.
 * *
 * *   DATE            AUTHOR         REFERENCE       REASON
 * *   29-JAN-2012    Simarjot Kaur   ---------       Initial
 * *
 * *   Copyright (c) 2013, Aricent Inc. All Rights Reserved
 * *
 * ******************************************************************************/

#ifndef _X2APS1AP_INTF_H_
#define _X2APS1AP_INTF_H_

#include "rrc_defines.h"
#include "rrc_intrl_api.h"
#include "x2ap_api.h"

#define RRC_X2APS1AP_MODULE_ID RRC_S1AP_MODULE_ID
typedef struct
{
	x2_enb_comm_info_t local_enb_info; /*^ M,0,N,0,0 ^*/
}x2ap_s1ap_comm_info_t; /*^ API, X2AP_S1AP_COMM_INFO ^*/

#endif /* RRC_X2APS1AP_INTF_H_ */
