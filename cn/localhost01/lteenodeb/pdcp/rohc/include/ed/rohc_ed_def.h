/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_ed_def.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains definitions for encoding and
 *                     decoding module.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_ed_def.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/28 05:15:10  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_ED_DEF_H_
#define _ROHC_ED_DEF_H_

#define ROHC_LOG_2(x)       log_wrapper(x)/log_wrapper(2)
#define ROHC_CEIL(x)     	ceil_wrapper(x)
#define ROHC_POWER_OF_2(x)  (1 << (x)) /* (1 << k) = 2 to the power of k */
/*SPR 20899 Fix Start*/
#define MULTIPLY_BY_TWO(x) ((x) << 1)
/*SPR 20899 Fix Stop*/
#define		 TSS_MAX					0xFFFFFFFF

#define		ROHC_MAX_SN_BITS		32
#define		ROHC_MAX_IP_ID_BITS		16
#define		ROHC_MAX_TS_BITS		32
#define		ROHC_MAX_MSN_BITS       16

#define		MAX_SN_BITS_IN_COMP_PKTTYP_1TO8		18
#define		MAX_ACK_BITS_IN_COMP_PKTTYP_1TO8    18

/*******************************************************************************
MACRO : ROHC_PUT_LSBS
*******************************************************************************
**  LOGIC :
**		Replaces bits_m of val_m in the orig_m. new_v_m is the output parameter.
**
*******************************************************************************/

#define ROHC_PUT_LSBS(new_v_m, orig_m, val_m, bits_m) \
do { \
new_v_m = (orig_m & ~((0x00000001UL << bits_m) - 1)) | val_m; \
}while(0)

/*******************************************************************************
MACRO : ROHC_ED_GET_CUR_INDEX
*******************************************************************************
**  LOGIC :
**		It returns the Current Value i.e.. end_index.
**
*******************************************************************************/

#define ROHC_ED_GET_CUR_INDEX(p_csc_m)\
	((p_csc_m->rohc_gsw.end_index))

#endif /* _ROHC_ED_DEF_H_ */
