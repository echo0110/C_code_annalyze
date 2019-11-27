/******************************************************************************
*
*   FILE NAME   : rrc_s1apCommon_intf.h
*
*   DESCRIPTION : This file contains basic S1AP Common interface definitions.
*
*   DATE            AUTHOR         REFERENCE       REASON
*   06-SEPT-2012    Rajneesh Walia  ---------       Initial
*
*   Copyright (c) 2012, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef RRC_S1APCOMMON_INTF_H_
#define RRC_S1APCOMMON_INTF_H_

#include "rrc_defines.h"
#include "s1ap_api.h"

#define MAX_ADD_DYNAMIC_MME 15

typedef struct
{
	U16 bitmask; /*^ M,0,N,0,0 ^*/

	/* SPR_17664_start*/
	U8 num_mme; /*^ M,0,B,1,MAX_ADD_DYNAMIC_MME ^*/
	
	/* SPR_17664_end*/
	mme_comm_info_t mme_comm_info_arr[MAX_ADD_DYNAMIC_MME]; /*^ M,0,OCTET_STRING,VARIABLE ^*/
}s1ap_add_mme_req_t; /*^ API, S1AP_ADD_MME_REQ ^*/

typedef struct
{
	U16 bitmask; /*^ BITMASK ^*/
	U8 response; /*^ M,0,H,0,1 ^*/
}s1ap_add_mme_res_t; /*^ API, S1AP_ADD_MME_RES ^*/

#endif /* RRC_S1APCOMMON_INTF_H_ */
