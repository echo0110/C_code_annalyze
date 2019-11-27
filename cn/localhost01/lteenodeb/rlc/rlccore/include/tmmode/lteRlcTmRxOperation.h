/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcTmRxOperation.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:51 gur20491 Exp $
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
 * $Log: lteRlcTmRxOperation.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:51  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:16:34  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.2.28.1  2010/07/17 11:37:11  gur23054
 * Review Comments fixed
 *
 * Revision 1.2  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.1  2009/01/29 05:35:22  gur20294
 * after code review comments incorporated
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 *
 ****************************************************************************/

#ifndef RLCTMRXOPERATION_H
#define RLCTMRXOPERATION_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcTmdSdu.h"
#include "lteRlcEntityDef.h"

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
UInt32 rlcReceiveTmPduFromLowerLayer(RxTMEntity * pRxTMEntity, TmdRawSdu *pTmdRawSdu) ; 

#endif  
