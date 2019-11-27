/******************************************************************************
 *
 *   FILE NAME:
 *       rrc_common_md.h
 *
 *   DESCRIPTION:
 *       This is the header file for the message dispatcher macros.
 *
 *   DATE            AUTHOR      REFERENCE       REASON
 *   01 Mar 2012     L3 Team     ---------       Initial
 *
 *   Copyright (c) 2012, Aricent Inc. All Rights Reserved
 *
 * ******************************************************************************/
#ifndef _RRC_COMMON_MD_H_
#define _RRC_COMMON_MD_H_

#include "rrc_defines.h"
/*SPR 21096 Start*/
	extern U8 L2_INSTANCE_ID[MAX_NUM_CELLS];
/*SPR 21096 End*/
#define RRC_GET_UECC_MODULE_ID(ue_id)\
        ((rrc_module_id_t)((ue_id >> 0x0D) + RRC_UECC_1_MODULE_ID))

/*BUG 604 changes start*/
/*#define RRC_GET_UECC_MODULE_ID_FROM_ENB(enb_ue_s1ap_id) \
        ((rrc_module_id_t)((((enb_ue_s1ap_id) & (0x0000FFFF)) >> 0x0D) + RRC_UECC_1_MODULE_ID))*/
/*BUG 604 changes stop*/
/*SPR 21096 Start*/

	#define RRC_GET_LL_MODULE_ID(base_module_id, cell_id)\
       	 ((rrc_module_id_t)(base_module_id + L2_INSTANCE_ID[cell_id]))

/*SPR 21096 End*/
#define RRC_GET_UECC_MODULE_INDEX(ue_id)\
        (ue_id >> 0x0D)

#define RRC_GET_UE_INDEX(ue_id)\
        (ue_id & 0x1FFF)

/*BUG 604 changes start*/
/*#define RRC_GET_UECC_MODULE_INDEX_FROM_ENB(enb_ue_s1ap_id) \
        ((rrc_module_id_t)(((enb_ue_s1ap_id) & (0x0000FFFF)) >> 0x0D))*/
/*BUG 604 changes stop*/
 

/*BUG 604 changes start*/
/*#define RRC_GET_UE_INDEX_FROM_ENB(enb_ue_s1ap_id)\
        ((enb_ue_s1ap_id) & (0x000001FFF))*/
/*BUG 604 changes stop*/

#define RRC_GET_UECC_MODULE_INDEX_FROM_X2AP(x2ap_ue_s1ap_id) \
            ((rrc_module_id_t)(((x2ap_ue_s1ap_id) & (0xFFFF)) >> 0x09) + RRC_UECC_1_MODULE_ID)

#endif /* _RRC_COMMON_MD_H_ */
