/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcUmArithmetic.c,v 1.1.1.1.12.2.2.1 2010/09/21 15:50:27 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : this file describes the arithmetic operations for UM
 *                      Entity
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcUmArithmetic.c,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:50:27  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:24:29  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.3  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.2  2009/01/29 05:35:22  gur20294
 * after code review comments incorporated
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcUmArithmetic.h"

/*FDD_Warning_Fix */
unsigned int getAbs(int n);
/*FDD_Warning_Fix */
/* SPR 4440 Fix Start */
/****************************************************************************
 * Function Name  : isUmLTE
 * Inputs         : x,
 *                  y,
 *                  mBase,
 *                  modVal
 * Outputs        : None
 * Returns        : SUCCESS/FAILURE
 * Description    : The function will check that x is less than equal to y
 ****************************************************************************/

/* SPR 5599_129119 Fix Start */
UInt32 isUmLTE( UInt16 x,
/* SPR 5599_129119 Fix End */
                       UInt16 y,
                       SInt32 mBase,
                       UInt16 modVal
                      )
{
    return ( ((getAbs( x - mBase) ) & modVal)
                <= ((getAbs( y - mBase) ) & modVal) );
}
/* SPR 4440 Fix End */


/** SPR 6388 Changes Start **/

/****************************************************************************
 * Function Name  : isUmLT
 * Inputs         : x,
 *                  y,
 *                  mBase,
 *                  modVal
 * Outputs        : None
 * Returns        : SUCCESS/FAILURE
 * Description    : The function will check that x is less than y.
 ****************************************************************************/

/* SPR 5599_129119 Fix Start */
UInt32 isUmLT( UInt16 x,
/* SPR 5599_129119 Fix End */
                       UInt16 y,
                       SInt32 mBase,
                       UInt16 modVal
                      )
{
    return ( ((getAbs( x - mBase) ) & modVal)
                < ((getAbs( y - mBase) ) & modVal) );
}
/** SPR 6388 Changes End **/
