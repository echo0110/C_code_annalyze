/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcArithmetic.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:29 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description :
 *                  This file contains macro defintions to help in arithmetic 
 *                  operations. 
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRlcArithmetic.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:29  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:14:04  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.4  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.3  2009/02/06 15:03:49  gur20294
 * updated
 *
 * Revision 1.2  2009/01/29 05:35:22  gur20294
 * after code review comments incorporated
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 *
 ****************************************************************************/

#ifndef ARITHMETIC_H
#define ARITHMETIC_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcGlobalData.h"
#include "lteRlcDefs.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/


/* return 0(FALSE) if value is positive/zero otherwise negative i.e non-zero (TRUE) */
#define IS_NEGATIVE(value) ( ((value) & GETMASK_LEFT_ALIGNED(1)) )
/* return 0(FALSE) if value is negative otherwise non-zero (TRUE) i.e value is positive/zero */
#define IS_POSITIVE(value) (~( IS_NEGATIVE(value) ))
/* return absolute value of integer */
#define ABSOLUTE(value) ( IS_POSITIVE(value)?value:~(value)+1 )

#define EXP2(power) ((0x1U) << (power) )

#define MOD(ui32_val,ui32_modVal) \
    (ui32_val) % (ui32_modVal)

#define POWER(ui16_val, ui16_x) pow_wrapper(ui16_val,ui16_x) 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#endif  /* ARITHMETIC_H */
