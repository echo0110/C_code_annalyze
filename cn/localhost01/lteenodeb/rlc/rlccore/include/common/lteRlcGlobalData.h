/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcGlobalData.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:33 gur20491 Exp $
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
 * $Log: lteRlcGlobalData.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:33  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:14:04  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.2.38.1  2010/06/17 10:59:42  gur19836
 * RLC AM Base Code Merged
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

#ifndef GLOBALDATA_H
#define GLOBALDATA_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcTypes.h"
#include "lteRlcDefs.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/
/*RLC Rel 2.0 Changes Start*/
extern ModuleLogDetail *rlcModuleLogDetail_p ; 
#define RLC_LOG(param1,...)
/*RLC Rel 2.0 Changes End*/

#endif  /* GLOBALDATA_H  */
