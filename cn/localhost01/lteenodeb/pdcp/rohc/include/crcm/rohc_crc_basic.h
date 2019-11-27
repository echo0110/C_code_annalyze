/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_crc_basic.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the functions for Basic CRC Computation.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_crc_basic.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/28 04:59:56  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/


#ifndef _ROHC_CRC_BASIC_H_
#define _ROHC_CRC_BASIC_H_

#include "rohc_crc_ext_if.h"

rohc_U32bit_t crc_calculate(rohc_U8bit_t type, rohc_U8bit_t *, rohc_U32bit_t length, rohc_U32bit_t crc_init);

rohc_U8bit_t crc_get_polynom(rohc_U8bit_t type);

rohc_void_t crc_init_table(rohc_U8bit_t *table, rohc_U8bit_t polynum);

/* Tables to enable fast CRC computations */

rohc_void_t rohc_crc_init_table(rohc_U8bit_t *table, rohc_U8bit_t poly);

rohc_U8bit_t rohc_crc_calc_8(rohc_U8bit_t *buf, rohc_U32bit_t size, rohc_U32bit_t crc_init);

rohc_U8bit_t crc_calc_7(rohc_U8bit_t *buf, rohc_U32bit_t size, rohc_U32bit_t crc_init);

rohc_U8bit_t crc_calc_3(rohc_U8bit_t *buf, rohc_U32bit_t size, rohc_U32bit_t crc_init);

/* + SPR 17439 */
void init_crc(void);
/* - SPR 17439 */
#endif /*End of _ROHC_CRC_BASIC_H_*/
