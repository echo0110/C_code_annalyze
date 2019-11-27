/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcUmArithmetic.h,v 1.1.1.1.12.2.2.2 2010/10/14 05:58:38 gur19479 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *                  This file declares the UM operations.
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRlcUmArithmetic.h,v $
 * Revision 1.1.1.1.12.2.2.2  2010/10/14 05:58:38  gur19479
 * updated to fix purify issue
 *
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:54  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:16:37  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.5  2009/04/07 15:02:16  gur19140
 * peer to peer fixes
 *
 * Revision 1.4  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.3  2009/03/02 09:10:54  gur19140
 * Changes reqarding Zero copy,pdcp discard and execution legs.
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

#ifndef UMARITHMETIC_H
#define UMARITHMETIC_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcArithmetic.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/* SPR 4440 Fix Start */
/* SPR 5599_129119 Fix Start */
UInt32 isUmLTE( UInt16 x,
/* SPR 5599_129119 Fix End */
                       UInt16 y,
                       SInt32 mBase,
                       UInt16 modVal
                      );

/* SPR 4440 Fix End */

/* Start 128 UE: CPU Utilization 100% fix */
UInt32 isUmLT( UInt16 x,
               UInt16 y,
               SInt32 mBase,
               UInt16 modVal
             );
/* End 128 UE: CPU Utilization 100% fix */


#endif  
