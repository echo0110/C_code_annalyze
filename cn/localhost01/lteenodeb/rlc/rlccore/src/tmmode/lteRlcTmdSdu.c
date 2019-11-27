/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcTmdSdu.c,v 1.1.1.1.12.2.2.1 2010/09/21 15:50:26 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : this file describes the operations on SDU queue.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcTmdSdu.c,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:50:26  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:24:27  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.3.28.1  2010/07/17 11:50:02  gur23054
 * Review Comments fixed
 *
 * Revision 1.3  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.2  2009/02/05 10:56:43  gur19836
 * CR comments fixed
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

#include "lteRlcTmdSdu.h"
#include "logging.h"
#include "alarm.h"

/****************************************************************************
 * Varibale Definition
 ****************************************************************************/
/****************************************************************************
 * External Variable Includes
 ****************************************************************************/
/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */





