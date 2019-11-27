/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcTmTxOperation.c,v 1.1.1.1.12.2.2.1 2010/09/21 15:50:25 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *  This file describes funtionality when delivering a new TMD PDU to lower layer
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcTmTxOperation.c,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:50:25  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:24:27  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.4.28.2  2010/07/17 11:49:35  gur23054
 * Review Comments fixed
 *
 * Revision 1.4.28.1  2010/06/18 07:31:02  gur19836
 * Corrected Indentation
 *
 * Revision 1.4  2009/04/09 15:23:02  gur19836
 * 100 ue related fixes
 *
 * Revision 1.3  2009/04/02 14:27:51  gur19140
 * changes for intg and peer to peer testing
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
 ****************************************************************************/


/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcTmTxOperation.h"
#include "lteRlcMacInterface.h"
#include "lteRlcUeContext.h"
#include "logging.h"
#include "lteRlcStat.h"
/****************************************************************************
 * Varibale Definition
 ****************************************************************************/
/****************************************************************************
 * External Variable Includes
 ****************************************************************************/
/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */






