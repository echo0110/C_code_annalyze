/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_led_def.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_led_def.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/28 05:20:12  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/


#ifndef _ROHC_LED_DEF_H_
#define _ROHC_LED_DEF_H_


#define INNER_IP_EXT_LIST_FID_U_MODE	ROHC_INNERIP_EXTHEADERS_LIST_FID - ROHC_INNERIP_EXTHEADERS_LIST_FID + ROHC_MODE_U - 1 /* 0 */
#define INNER_IP_EXT_LIST_FID_O_MODE	ROHC_INNERIP_EXTHEADERS_LIST_FID - ROHC_INNERIP_EXTHEADERS_LIST_FID + ROHC_MODE_O - 1 /* 1 */
#define INNER_IP_EXT_LIST_FID_R_MODE	ROHC_INNERIP_EXTHEADERS_LIST_FID - ROHC_INNERIP_EXTHEADERS_LIST_FID + ROHC_MODE_R - 1 /* 2 */

#define OUTER_IP_EXT_LIST_FID_U_MODE	ROHC_OUTERIP_EXTHEADERS_LIST_FID - ROHC_INNERIP_EXTHEADERS_LIST_FID + ROHC_MODE_U - 1 /* 4 */
#define OUTER_IP_EXT_LIST_FID_O_MODE	ROHC_OUTERIP_EXTHEADERS_LIST_FID - ROHC_INNERIP_EXTHEADERS_LIST_FID + ROHC_MODE_O - 1 /* 5 */
#define OUTER_IP_EXT_LIST_FID_R_MODE	ROHC_OUTERIP_EXTHEADERS_LIST_FID - ROHC_INNERIP_EXTHEADERS_LIST_FID + ROHC_MODE_R - 1 /* 6 */

#define CSRC_FID_U_MODE					ROHC_RTP_CSRC_LIST_FID - ROHC_INNERIP_EXTHEADERS_LIST_FID + ROHC_MODE_U - 1 /* 9 */
#define CSRC_FID_O_MODE					ROHC_RTP_CSRC_LIST_FID - ROHC_INNERIP_EXTHEADERS_LIST_FID + ROHC_MODE_O - 1 /* 10 */
#define CSRC_FID_R_MODE					ROHC_RTP_CSRC_LIST_FID - ROHC_INNERIP_EXTHEADERS_LIST_FID + ROHC_MODE_R - 1 /* 11 */

#define ENCODING_TYPE_0		(0U)
#define ENCODING_TYPE_1		(1U)
#define ENCODING_TYPE_2		(2U)
#define ENCODING_TYPE_3		(3U)

#define ROHC_GP_BIT			(6U)
#define ROHC_PS_BIT			(5U)
#define ROHC_ASEQ_BIT		(7U)
#define ROHC_ESEQ_BIT		(6U)
#define ROHC_GSEQ_BIT		(5U)
#define ROHC_CL_BIT			(8U)

#define ROHC_GRE_C_BIT		(0x80U)
#define ROHC_GRE_K_BIT		(0x20U)
#define ROHC_GRE_S_BIT		(0x10U)

#define ROHC_MAX_EXT_HDR_SIZE		(13U)
#define ROHC_MAX_EXT_PKT_HDR_SIZE	(25U)
#endif /* _ROHC_LED_DEF_H_ */
